# SATA SSD Firmware Requirement Specification (Reverse-Engineered)

## 1. Document Summary

### 1.1 文件目的
本文件目的為根據 `src/` firmware source code 與 `hw_spec/` 硬體規格，重建可供 Firmware RD 重新實作的專案需求規格，強調「行為需求、模組責任、硬體介面契約、錯誤處理與可追溯性」。

### 1.2 分析範圍
- Firmware source: `src/`（main loop、SATA command、FTL、Flash queue、SMART/Security/Sanitize/DCO/HPA/vendor path）
- Hardware specification: `hw_spec/*.md`（APU/SATA、Doorbell queue、SS DMAC、FLH、DDR、SYS）

### 1.3 分析依據
- Observed（可直接證實）: 函式、狀態機、命令表、寄存器存取、thread model、硬體 register spec。
- Inferred（合理推導）: 模組交互責任、資料路徑切分、資源 ownership、時序依賴。
- Unknown/Ambiguous（證據不足）: 無完整設計文件處、條件編譯不同 SKU 的最終行為、部分 vendor/debug path 的量產適用性。

### 1.4 Confidence / Coverage Summary
- Command decode/dispatch coverage: **高**（`cmdtbl.h` + `mainTask` dispatch）。
- Runtime execution model: **高**（ThreadX 三執行緒與主迴圈可觀察）。
- FTL/GC/WL 細節演算法參數: **中**（大型條件編譯、部分策略需推導）。
- HW register bit-level access sequence: **中到高**（規格完整，但 FW 僅使用子集合）。
- Performance target（精確數值）: **低到中**（主要為 timeout/flow 行為，缺少正式 KPI 檔）。

---

## 2. Product / System Overview

### 2.1 產品定位與邊界
- FR-001（Observed）: 系統為 SATA SSD controller firmware，處理 ATA/SATA 命令、NAND/DDR 管理、FTL 映射與背景維護。  
- FR-002（Observed）: Host 介面為 ATA command set（含 NCQ、SMART、Security、Sanitize、DCO/HPA、vendor command path）。

### 2.2 系統組成與角色
- FR-003（Observed）: Host 透過 APU/SATA register space 提交命令，firmware 由 command table dispatch 對應 handler。  
- FR-004（Observed）: Controller 包含 CPU + ThreadX scheduler + HW blocks（APU、Doorbell、SS DMAC、FLH、DDR/ECC、SYS/PMU/RTT）。  
- FR-005（Inferred）: DRAM/DDR 承載快取、表格、queue、metadata；SRAM/內部 buffer 承載高頻 staging 與資料移轉。  
- FR-006（Observed）: NAND 操作由 FPU/FLH/MTQ/FQ 路徑驅動，並由 flaTask 排程到 CE/channel。

---

## 3. Firmware Architecture

### 3.1 分層架構
- FR-010（Inferred）: 建議分四層重建：
  1) Host protocol layer（ATA command decode/status/FIS 回覆）
  2) Control service layer（SMART/Security/Sanitize/Power/Reset）
  3) FTL/data management layer（L2P/P2L、GC/WL、cache policy）
  4) HW abstraction/driver layer（APU/DMAC/FLH/DDR/interrupt/doorbell）

### 3.2 主要 modules 與責任
- FR-011（Observed）: `mainTask` 為 host-facing 控制中樞，處理 reset、NCQ、單命令 dispatch、背景作業切換。
- FR-012（Observed）: `ftlTask` 處理 BufferQueue 到 FTL/GC/WL/映射更新。
- FR-013（Observed）: `flaTask` 管理 FlashQueue 與 CE-level 併發執行。
- FR-014（Observed）: `Satacmd.c` 為 ATA 基礎命令行為實作。
- FR-015（Observed）: `smart.c`、`Security.c`、`sanitize.c`、`DCO.c`、`HPA.c` 為 feature-set 獨立模組。
- FR-016（Observed）: `vender.c` 實作廠商命令、ISP/維修/測試/工廠流程。

### 3.3 模組依賴關係
- FR-017（Observed）: host command 由 mainTask dispatch 後，會控制 `gubStopFTL`、`HandleStopRW` 等同步機制，避免與 FTL/FQ 背景流程衝突。
- FR-018（Inferred）: FTL 與 Flash 層共享 BQ/FQ 結構，透過 state flag + queue depth 控制背壓。

### 3.4 Control Path / Data Path
- FR-019（Observed）: Control path：command decode → feature pre-check（security/sanitize/write protect）→ handler → status/error register update。
- FR-020（Observed）: Data path：host sectors → BQ → FTL map/allocate → FQ issue → FLH/NAND → completion/D2H。

---

## 4. Boot / Initialization / Bring-up Flow

### 4.1 Power-on 到 Ready
- FR-030（Observed）: `main()` 先處理 GPIO/低功耗腳位、DDR type 偵測、RTT interrupt 初始化，再 `tx_kernel_enter()`。
- FR-031（Observed）: `tx_application_define()` 建立 3 個 thread：mainTask/ftlTask/flaTask。
- FR-032（Observed）: `InitFPUEntry()` 將 FPU microcode/entry 載入 IRAM 並依 NAND variant 指定 FPU pointer table。
- FR-033（Observed）: `ftlTask` 在 `BYTE_INIT_START_SCAN` 觸發 `ftlInitFlash()`，完成 scan/init state。
- FR-034（Observed）: mainTask 完成 SATA register init 後才允許進入常態 command loop。

### 4.2 初始化順序與相依性
- FR-035（Inferred）: 初始化順序需確保：
  1) 基礎 clock/interrupt/RTT
  2) SATA/APU register defaults
  3) FTL metadata scan/recover
  4) NCQ/auto D2H enable
- FR-036（Observed）: reset 後需重新執行 `Init_Reset()`、`Sata_InterfaceReg_Init()` 並依條件重啟 NCQ。

### 4.3 Reset / Re-init
- FR-037（Observed）: 支援 soft reset、hard reset、link lost recovery；重置時先停止 BG/RW，再重建 host register state。
- FR-038（Observed）: Reset flow 需清除/重設 command completion 相關旗標、錯誤碼、D2H 模式。

---

## 5. Runtime Execution Model

### 5.1 Task model / scheduler
- FR-050（Observed）: 系統使用 ThreadX cooperative switching（顯式 `M_SwitchTask()` 讓出 CPU）。
- FR-051（Observed）: 三執行緒同優先序（main/ftl/fla）且無 time slice，行為近似 cooperative multitasking。

### 5.2 ISR / polling model
- FR-052（Observed）: 使用 xtensa interrupt handler 註冊 SATA/DDR/RTT/stack/parity 等 ISR。
- FR-053（Observed）: 重要流程以 polling + flag（`gubHostCmdIn`, `HB_REG_REQ`, queue count）與 ISR 混合。

### 5.3 Background jobs
- FR-054（Observed）: mainTask 在 host idle 或可中斷點執行 BG copy/cleanGR/GC/WL；收到 host command 需可 preempt/stop BG。
- FR-055（Observed）: FTL 僅在無前景衝突時進入 GC/WL 條件分支。

### 5.4 Concurrency / shared resource ownership
- FR-056（Inferred）: Host-facing registers 由 mainTask 擁有；mapping/BQ 由 ftlTask 擁有；NAND issue queue/FQ 由 flaTask 擁有。
- FR-057（Observed）: 共享資源以 global flag + queue counters + stop tokens 協調，未見一般 mutex；重建時需保持同等原子保護語義。

---

## 6. Functional Requirements

### 6.1 SATA command handling
- FR-100（Observed）: 必須提供 256-entry command decode table，未實作命令回 ABRT。  
- FR-101（Observed）: 必須支援 read/write/read multiple/write multiple/read/write DMA/flush cache/FUA。  
- FR-102（Observed）: 必須支援 Identify (`0xEC`)、Set Features (`0xEF`)、Power commands (`0xE0~0xE6`)。  
- FR-103（Observed）: 必須支援 TRIM/DSM (`0x06`)。  
- FR-104（Observed）: 必須支援 SMART (`0xB0`)、Security (`0xF1~0xF6`)、Sanitize (`0xB4`)、DCO (`0xB1`)、HPA (`0x27/0x37/0xF8/0xF9`)。  
- FR-105（Observed）: 命令完成需根據 `gubNeedSendCmdComplete` 與 error code 送 D2H/更新 HB status/error。

### 6.2 FTL / mapping policy
- FR-110（Observed）: 必須維護 L2P/P2L/GR/GC table 與 in-RAM index/inverse 結構。
- FR-111（Observed）: host write 先進 BQ，再經 FTL 決策形成 FQ/NAND 實際操作。
- FR-112（Inferred）: mapping policy 為 page-level + group/resource-table 混合管理（依變數命名與 GCGR 結構）。

### 6.3 Garbage Collection / Wear Leveling
- FR-120（Observed）: FTL state flags `btNeedCleanGR / btNeedCloseTarget / btNeedWearLeveling` 驅動背景 GC/WL。
- FR-121（Observed）: 若有前景 NCQ/RW 競爭，GC/WL 應延後或中斷以優先 host I/O。

### 6.4 Bad block management
- FR-130（Observed）: 系統維護 early/later bad block 與 per-CE/per-plane 統計（SMART/vendor path）。
- FR-131（Inferred）: init scan 與 runtime fail handling 共同更新 bad-block 視圖與可用單元池。

### 6.5 ECC / data integrity / metadata
- FR-140（Observed）: DDR zone ECC/CRC interrupt 必須啟用並處理；錯誤需可統計與上報。
- FR-141（Observed）: parity/stack monitor/DDR error 需有 ISR 路徑，部分嚴重錯誤觸發 assert/reset。
- FR-142（Inferred）: metadata consistency 依靠 flush/stop-RW/recovery 流程與 table persist。

### 6.6 Cache / buffer / DMA / descriptor
- FR-150（Observed）: 必須具備 host buffer RW path（`RWBuffer` 等）與 DMA/PIO 條件分流。
- FR-151（Observed）: BQ/FQ 需支援 4K 粒度 buffer usage/order/check flag，避免 out-of-order data corruption。

### 6.7 Power-loss handling
- FR-160（Observed）: 若 `ENABLE_PFAIL`，必須在 PFAIL 狀態進入 lock/stop-RW/BG stop，並保護在途命令一致性。
- FR-161（Inferred）: 需保留可恢復 metadata 最小一致點（至少 command completion 邊界）。

### 6.8 Boot / firmware update / recovery
- FR-170（Observed）: 提供 ISP jump/prog/verify vendor path（`Vender_Isp_*`）。
- FR-171（Observed）: 支援 reset/re-init 與 devslp wake flow。

### 6.9 Logging / debug / telemetry
- FR-180（Observed）: SMART logs、command log、vendor SMART/diagnostic、fail log 必須保留。
- FR-181（Observed）: 需支援 GPIO/UART debug hook 與 timeout/fail event 記錄。

---

## 7. Non-Functional Requirements

- NFR-001（Observed）: 命令處理需具 timeout 監控（ATA op time threshold）與 fail log。  
- NFR-002（Inferred）: 前景 I/O latency 優先於背景 GC/WL。  
- NFR-003（Observed）: 記憶體使用需遵循既有 static table/buffer 佈局（大量 global memory region）。  
- NFR-004（Observed）: 需符合 DDR ECC/CRC 保護與中斷回報機制。  
- NFR-005（Inferred）: 可靠度目標為「reset/link event 後可回到 command-ready 且不破壞 host protocol correctness」。  
- NFR-006（Observed）: 高度可配置（compile-time macros）且需支援多 NAND vendor/geometry。

---

## 8. Hardware Usage Specification

### 8.1 HW blocks
- HW-001（Observed）: SYS block（clock/PMU/GPIO/RTT/WDT/UART）
- HW-002（Observed）: SATA APU register block（command/status/error/link/PHY/event counters）
- HW-003（Observed）: Doorbell circular queues（DMAC/SS/FLH/RS/SATA completion queues）
- HW-004（Observed）: SS DMAC（submission/completion descriptor path）
- HW-005（Observed）: FLH/Flash controller（FPU trigger, MTQ, ECC/LDPC related controls）
- HW-006（Observed）: DDR controller（Zone1/2/3 ECC/CRC and interrupt registers）

### 8.2 寄存器與 bit-field 操作規範
- HW-010（Observed）: W1C/RW1C/RW1CL 語義需依 spec 嚴格遵循，避免誤清除事件。  
- HW-011（Observed）: Doorbell queue base address 必須 alignment 條件與 queue reset/control sequence。  
- HW-012（Observed）: SATA interrupt enable/status 與 error interrupt 需分離配置。  
- HW-013（Observed）: DDR3_ECC_CTRL 中 zone error clear/int-enable/status bit 需配對處理。  
- HW-014（Inferred）: FLH/DMAC 操作需先配置 descriptor/buffer，再 trigger queue pointer/doorbell。

### 8.3 HW/SW handshake
- HW-020（Observed）: HB register request (`HB_REG_REQ`) 採 FW 設定 + polling 清除。  
- HW-021（Observed）: command/data trigger bit 需在錯誤/重置狀況下清除，防止下一命令判斷錯誤。  
- HW-022（Observed）: NCQ/D2H 自動化功能在 reset/sanitize/security state 下需條件啟閉。

---

## 9. Algorithms and Data Structures

- ALG-001（Observed）: 命令 dispatch 使用 256-entry function pointer table。  
- ALG-002（Observed）: FTL 以 BQ（Buffer Queue）收斂 host IO，再轉換 FQ（Flash Queue）發送 NAND 操作。  
- ALG-003（Observed）: FQ scheduler 以 CE 維度分配服務，包含 phase/depth/order 控制與特殊對齊策略。  
- ALG-004（Inferred）: GC/WL 決策根據 FTL state flag + host load（NCQ/queue counters）決定進入時機。  
- ALG-005（Observed）: SMART/Sanitize/Security 均先檢查 key/state/feature bits，失敗即 ABRT 並給 error reason。

---

## 10. Error Handling and Recovery

- ERR-001（Observed）: 錯誤分類至少包含 ATA protocol error（ABRT/IDNF/UNC）、link/reset error、ECC/parity error、NAND operation fail。  
- ERR-002（Observed）: command error 需更新 HB status/error 並依情況記錄 command log。  
- ERR-003（Observed）: reset/link lost 時需先停止 BG/RW，再重建 host interface state。  
- ERR-004（Observed）: parity/stack monitor/DDR ECC 重大錯誤可導向 assert 或 reset flow。  
- ERR-005（Inferred）: NAND timeout/retry level 與 read-retry table 為可配置策略，需對應不同 NAND variant。

---

## 11. Configuration and Variants

- CFG-001（Observed）: compile-time 巨量巨集控制（BURNER/RDT/TLC/vendor-specific/PFAIL/debug）。  
- CFG-002（Observed）: NAND variant（Micron/Hynix/YMTC/BICS）影響 FPU sequence、retry table、plane mode、timing。  
- CFG-003（Observed）: 部分特性（NCQ、DIPM、temperature control、prefetch、autopol timeout）可啟閉。  
- CFG-004（Unknown）: 各 SKU 最終宏組合與產品線 mapping 未在 repo 完整列出。

---

## 12. External and Internal Interfaces

- IF-001（Observed）: Host-visible interface 為 ATA register semantics + command completion/D2H behavior。  
- IF-002（Observed）: Internal API 以跨檔 global function + shared structs 為主（非嚴格封裝）。  
- IF-003（Inferred）: 重建實作建議抽象化為 module API（HostCmd, FTLCore, FlashSched, HwIf）以降低全域耦合。

---

## 13. Assumptions, Unknowns, and Ambiguities

- UNK-001: FTL mapping metadata on-media 版面（完整欄位與版本管理）未集中於單一清楚規格。  
- UNK-002: GC/WL 精確觸發門檻與各 NAND 世代 tuning 值分散於大量條件編譯。  
- UNK-003: Vendor command 子命令中哪些屬量產正式需求、哪些僅工廠/debug，需 PM/FAE 確認。  
- UNK-004: 部分 power-loss 路徑需搭配外部硬體條件（電容 hold-up、GPIO trigger）才能完整驗證。  
- UNK-005: 某些 hw_spec 定義功能在 FW 中未必使用，實際 required subset 需再 freeze。

---

## 14. Discrepancies (Code vs Spec)

- DSC-001（Observed）: Sanitize command path僅實作 overwrite/freeze/antifreeze/status；crypto scramble/block erase在 code 直接 ABRT。  
- DSC-002（Inferred）: hw_spec 提供較廣泛 queue/register 能力；FW 實際使用 subset，未見全面啟用證據。  
- DSC-003（Observed）: 許多標準 ATA opcode 在 command table 標記 NotImplement。

---

## 15. Re-implementation Guidance (Normative)

- REQ-900（Normative）: 新實作必須先對齊 command-visible behavior（status/error/identify/smart/security/sanitize），再逐步對齊效能優化。
- REQ-901（Normative）: 先建立可驗證的三執行緒模型（host/ftl/flash）與 queue handoff，再導入 GC/WL/錯誤復原策略。
- REQ-902（Normative）: 硬體層須使用明確 register access wrapper（W1C/RW1C 保護）與 timeout policy，避免 race。
- REQ-903（Normative）: 所有 Observed requirement 需有單元/整合測試覆蓋；Inferred requirement 需透過 trace log 與量測確認。
