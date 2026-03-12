# SATA SSD Firmware Requirement Specification (Reverse-Engineered)

## 1. Document Summary

### 1.1 文件目的
本文件目的為根據 `src/` firmware source code 與 `hw_spec/` 硬體規格，重建可供 Firmware RD 重新實作的專案需求規格，強調「行為需求、模組責任、介面契約、硬體驅動時序、錯誤處理與可追溯性」。

### 1.2 分析範圍
- Firmware source: `src/`（main loop、SATA command、FTL、Flash queue、SMART/Security/Sanitize/DCO/HPA/vendor path）
- Hardware specification: `hw_spec/*.md`（APU/SATA、Doorbell queue、SS DMAC、FLH、DDR、SYS）

### 1.3 證據分類規則
- **Observed**：可直接從 source/spec 驗證。  
- **Inferred**：由多處觀察推導而來。  
- **Unknown/Ambiguous**：證據不足或相互矛盾，不得當成已定需求。

### 1.4 Confidence / Coverage Summary
- Command decode/dispatch：高
- Runtime task/queue model：高
- BQ/FQ 結構欄位：高（結構體可直接觀察）
- State machine 細節：中到高（sanitize/security/pfail 明確；部分流程需推導）
- On-media metadata format：低（核心缺口）

---

## 2. Product / System Overview

- FR-001（Observed）: 系統為 SATA SSD controller firmware，負責 ATA/SATA protocol、FTL 映射、NAND I/O 排程、背景維護與資料保護。  
- FR-002（Observed）: Host-visible interface 為 ATA command register model + D2H/FIS 回覆語義。  
- FR-003（Observed）: 系統採三工作執行緒（mainTask / ftlTask / flaTask）協作模型。  
- FR-004（Observed）: NAND 與 DRAM 為主要資料路徑，SRAM/queue 用於 staging 與排程控制。

---

## 3. Firmware Architecture

### 3.1 分層
- FR-010（Inferred）: Host Protocol Layer（command decode/status/error）
- FR-011（Observed）: Service Layer（SMART/Security/Sanitize/DCO/HPA/Power）
- FR-012（Observed）: Data Management Layer（BQ/L2P/P2L/GC/WL）
- FR-013（Observed）: Flash Execution Layer（FQ/MTQ/FPU/FLH）
- FR-014（Observed）: HW Interface Layer（APU/Doorbell/DMAC/DDR/SYS interrupt）

### 3.2 控制與資料路徑
- FR-015（Observed）: Control path = `SataCmdTable[opcode]` dispatch + handler precondition checks。  
- FR-016（Observed）: Data path = Host CMD → BQ enqueue → FTL transform/map → FQ scheduling → NAND execute → completion。

---

## 4. Boot / Initialization / Bring-up Flow

- FR-030（Observed）: `main()` 設定 GPIO 省電腳位、RTT timer interrupt、必要全域旗標後進入 `tx_kernel_enter()`。  
- FR-031（Observed）: `tx_application_define()` 建立 mainTask/ftlTask/flaTask。  
- FR-032（Observed）: `InitFPUEntry()` 將 FPU code/entry 載入 IRAM 並依 NAND variant 填入程式序列 pointer。  
- FR-033（Observed）: `ftlTask` 在 `BYTE_INIT_START_SCAN` 呼叫 `ftlInitFlash()`，完成後進入 steady state。  
- FR-034（Observed）: 主流程 reset/re-init 後需 `Init_Reset()` + SATA register re-init + 條件式 NCQ re-enable。

### 4.1 Init state machine（可實作）
| State | Entry | Exit | Trigger | Timeout | Illegal transition handling | Side effects |
|---|---|---|---|---|---|---|
| INIT_START_SCAN (`BYTE_INIT_START_SCAN`) | power-on/reset 後 | INIT_VT_RUT_VTC_DONE..INIT_SCAN_DONE | `ftlInitFlash()` progress | Unknown | assert/fail log | disable normal host I/O acceptance |
| INIT_VT_RUT_VTC_DONE | scan phase complete | INIT_TABLE_COPY_DONE | table build/copy phase enter | Unknown | stay/retry | VT/RUT/VTC in-memory valid |
| INIT_TABLE_COPY_DONE | copy done | INIT_TABLE_DONE | copy finalize | Unknown | stay/retry | table cache staged |
| INIT_TABLE_DONE | table ready | INIT_SCAN_DONE | finalize success | Unknown | assert | logical mapping available |
| INIT_SCAN_DONE (`BYTE_INIT_SCAN_DONE`) | init complete | runtime | task loop steady | N/A | N/A | normal I/O + BG eligible |

- SM-INIT-001（Observed）: `gubSSD_Init_State.B.ubState < BYTE_INIT_SCAN_DONE` 時 ftlTask 不進入正常 BQ 處理迴圈。  
- SM-INIT-002（Observed）: preformat 模式可改變 init 完成後行為（等待 preformat 清除）。

---

## 5. Runtime Execution Model

- FR-050（Observed）: cooperative 多執行緒，透過 `M_SwitchTask()` 顯式讓出 CPU。  
- FR-051（Observed）: mainTask 主導 host command/reset/pfail 事件；ftlTask 主導 BQ/GC/WL；flaTask 主導 FQ/CE dispatch。  
- FR-052（Observed）: ISR 用於 SATA/DDR/RTT/Parity/Stack monitor，主邏輯仍大量 polling+flag。

### 5.1 Ownership 規範
- IC-OWN-001（Observed）: **Host register ownership** 屬 mainTask（HB/HW command completion path）。  
- IC-OWN-002（Observed）: **BufferQueue ownership** 屬 ftlTask（建立/消費/重排/轉 FQ）。  
- IC-OWN-003（Observed）: **FlashQueue ownership** 屬 flaTask（按 CE phase 實際執行）。  
- IC-OWN-004（Inferred）: cross-task 只允許透過旗標/queue counter + state bit 協調，不允許跨層直接覆寫對方私有鏈結結構。

---

## 6. Interface Contracts（新增：可直接實作）

## 6.1 mainTask -> ftlTask -> flaTask handoff contract

### 6.1.1 BQ Entry Contract
- IC-BQ-001（Observed）: `BufferQueue_t` 必備欄位：
  - link: `ubNext/ubPrevious`
  - payload id: `uwBufferIndex`, `ulVir4kIndex`, `ulSectorCnt`
  - data validity: `ulDataMask`, `ulSpareValid`, `ubDataInDRamMap`
  - mode flags: `btWrite`, `btNeedLoadAlign`, `btSpecialData`, `btNeedCheckSpecialData`, `btPreRead`, `btFlushWBQ`, `ubThisBQisBG`
  - shape: `ub4kNum`, `ubZcode[]`
  - binding: `ubRBQIIndex`。
- IC-BQ-002（Observed）: BQ 是雙向 ring/list，`ubLinkFirst/Last/Num` 為有效性來源；`ubLinkNum==0` 表示前景 queue 空。
- IC-BQ-003（Observed）: ftlTask 消費 BQ 後可能：
  1) 轉成 FQ（正常 I/O path）
  2) 保留等待（special data/load align/table in RAM）
  3) 移除/重排（flush/abort/reset path）。
- IC-BQ-004（Inferred）: BQ buffer 釋放責任在「成功轉成 FQ 並完成必要 bookkeeping」之後，由 ftlTask/flaTask 協作清理旗標。

### 6.1.2 FQ Entry Contract
- IC-FQ-001（Observed）: `FlashQueue_t` 必備欄位：
  - link: `ubPrevious/ubNext`
  - target: `ubCEIndex`, `uwFUnit`, `ulFEntry`
  - job: `ubFJob`, `uwFJobInfo`, `ubFQPhase`, `btUNServed`
  - transfer: `ubL4kIndex`, `ubL4kNum`, `uw4KInFQMap`, `ulRAMAddr`, `ulDataMask`, `ulSpare`
  - buffer-order safety: `ubUseBufferOrder[]`, `ubBufferInedex_R`
  - retry/error: `ulFIPError`, `ubCheckStatusPhase`, `btPEFail`, `btReadUNC`。
- IC-FQ-002（Observed）: flaTask 以 CE 維度控制 `ubFQDoingNumber[]`，條件滿足才把 `btUNServed` FQ 轉為執行中。
- IC-FQ-003（Observed）: completion 條件為 phase 到達 done/error terminal，並更新 queue depth/可用 link。

### 6.1.3 Ownership transfer & completion
- IC-XFER-001（Observed）: `BQ -> FQ` 轉移時，source BQ 不再允許 mainTask 修改 payload，僅可被 reset/abort 控制邏輯回收。  
- IC-XFER-002（Observed）: `FQ completion` 後才可釋放對應 buffer-order 保護位與 FW read buffer flag。  
- IC-XFER-003（Inferred）: 同一 logical 4K 不得同時被兩個 active FQ 以衝突方向（read/write）持有。

### 6.1.4 Backpressure contract
- IC-BP-001（Observed）: queue 滿載或順序不符時，ftlTask/flaTask 透過 continue/yield 保留 entry，等待下次排程。  
- IC-BP-002（Observed）: 有前景命令或 reset/pfail 事件時，BG copy/GC/WL 必須可被停止（`M_StopBG_Copy`, `HandleStopRW`）。  
- IC-BP-003（Inferred）: host front-end 若觀察到 queue 無法前進，需維持 protocol-level busy，不得回報虛假完成。

### 6.1.5 Abort/Reset rollback contract
- IC-RB-001（Observed）: soft/hard reset 進入 `ResetCmdIn()` 前需先 stop BG 或 stop RW。  
- IC-RB-002（Observed）: reset 後 `HB_STATUS/HB_ERROR/gubErrorCode/gubNeedSendCmdComplete` 回到一致初始語義。  
- IC-RB-003（Inferred）: rollback 最小集合至少包含 command-facing 狀態、queue trigger bits、NCQ enable state、FTL stop token。

---

## 7. Functional Requirements

### 7.1 SATA command handling
- FR-100（Observed）: 必須提供 256-entry command decode table，未實作命令回 ABRT。  
- FR-101（Observed）: 必須支援 read/write/read multiple/write multiple/read/write DMA/flush cache/FUA。  
- FR-102（Observed）: 必須支援 Identify、Set Features、Power commands。  
- FR-103（Observed）: 必須支援 TRIM (`0x06`)。  
- FR-104（Observed）: 必須支援 SMART/Security/Sanitize/DCO/HPA。  
- FR-105（Observed）: command completion 由 HB register request + status/error 寫回實現。

### 7.2 FTL/GC/WL
- FR-110（Observed）: 需維護 L2P/P2L/GR/GC 相關表。  
- FR-111（Observed）: host write 先進 BQ，後續由 ftlTask 轉 FQ。  
- FR-120（Observed）: `btNeedCleanGR / btNeedCloseTarget / btNeedWearLeveling` 決定背景作業。  
- FR-121（Observed）: 前景 NCQ/RW 活躍時，GC/WL 應讓路。

### 7.3 Security/Sanitize/DCO/HPA/SMART
- FR-130（Observed）: sanitize/security state 會直接 gate 多數 ATA 命令（不符則 ABRT）。  
- FR-131（Observed）: SMART 子命令依 feature code 分流，部分命令不送一般 cmd-complete（直接資料路徑）。  
- FR-132（Observed）: DCO/HPA 會影響 media size 與 feature reporting，有 lock/freeze 邏輯。

---

## 8. State Machine Specification（新增：完整轉移規則）

### 8.1 SATA Reset/Link state machine
| State | Entry condition | Exit condition | Trigger | Timeout | Illegal handling | Persistent behavior |
|---|---|---|---|---|---|---|
| RS_IDLE | 正常運行 | soft/hard/link lost | reset flags set | N/A | N/A | keep ATA cfg |
| RS_PREPARE_STOP | 收到 reset/link lost | BG/RW quiesced | `M_StopBG_Copy` or `HandleStopRW` done | Unknown | assert/log and force stop path | freeze new host cmd handling |
| RS_REINIT | stop 完成 | SATA/HB registers reinit done | `Init_Reset` + `Sata_InterfaceReg_Init` | wait on `HB_REG_REQ` clear | if timeout => error/reset escalate | optionally re-enable NCQ |
| RS_RESUME | reinit done | return RS_IDLE | flags clear and cmd path reopened | N/A | N/A | clear transient error flags |

- SM-RST-001（Observed）: `gubSoftReset` / `gubHardReset` / `gubLinkLost` 三路共用相同 stop-then-reinit骨架。  
- SM-RST-002（Observed）: hard reset path包含 PHY ready wait與 COMRESET 計數更新。  
- SM-RST-003（Observed）: reset 後需重建 HB status = ready/no-error。

### 8.2 Sanitize state machine
| State | Entry | Exit | Legal trigger | Illegal trigger handling |
|---|---|---|---|---|
| SD0_IDLE | init/reset completion | SD1/SD2 | sanitize freeze / overwrite start | ABRT + reason code |
| SD1_FROZEN | sanitize freeze command | SD0 (power-on/comreset policy) | init mode reset condition | all sanitize mutation cmd ABRT |
| SD2_IN_PROGRESS | overwrite started | SD3/SD4 | overwrite completion/failure | conflicting cmd ABRT, NCQ disabled |
| SD3_FAIL | overwrite fail | SD0 (mode-based clear) | sanitize status clear policy | restricted commands ABRT |
| SD4_SUCCESS | overwrite success | SD0 (mode-based clear) | init/policy reset | unsupported cmd ABRT |

- SM-SAN-001（Observed）: 不支援 crypto scramble / block erase（feature 0x0011/0x0012 直接 ABRT）。  
- SM-SAN-002（Observed）: sanitize feature 需特定 key/state/lock 條件，不符回 ABRT 並寫 reason code LBA_L。  
- SM-SAN-003（Observed）: sanitize state/value 變化後需 `SaveAtaConfig()`。

### 8.3 Security state machine
| State | Entry | Exit | Trigger | Illegal handling |
|---|---|---|---|---|
| SEC_DISABLED | default or disable pass | SEC_ENABLED_UNLOCKED | set password | wrong protocol => ABRT |
| SEC_ENABLED_UNLOCKED | set pass success | SEC_LOCKED / SEC_FROZEN / SEC_DISABLED | power cycle/lock, freeze, disable pass | ABRT |
| SEC_LOCKED | security lock active | SEC_ENABLED_UNLOCKED | unlock command with correct pass | wrong pass => pw exceed/error path |
| SEC_FROZEN | freeze lock | reset path only | security freeze cmd | mutation cmd ABRT |

- SM-SEC-001（Observed）: security mode使用 `AtaCfg.ubSecMode` bit flags（EN/LOCK/FROZEN/PWEXCE）。  
- SM-SEC-002（Observed）: locked/frozen 狀態會 gate 多個 ATA 命令。

### 8.4 PFAIL state machine
| State | Entry | Exit | Trigger | Timeout | Illegal handling |
|---|---|---|---|---|---|
| INIT(0) | boot | PRE_LOCK | pfail event | N/A | N/A |
| PRE_LOCK(1) | pfail asserted | LOCKED | stop BG + stop RW success | runtime-dependent | if stop fail => force stop + log |
| LOCKED(2) | PRE_LOCK done | PRE_RELEASE | RDT/test release condition | observed test gate `>=250` tick in RDT path | stay locked |
| PRE_RELEASE(3) | release requested | INIT | release handling complete | N/A | clear-to-init fallback |

- SM-PF-001（Observed）: PRE_LOCK 會停止 BG copy、HandleStopRW、鎖住前景流程。  
- SM-PF-002（Observed）: PRE_RELEASE 會把 `gubPfail_state` 清回 0。

### 8.5 Background GC/WL suspend/resume
- SM-BG-001（Observed）: 若 host cmd in / NCQ active / reset event，必須優先停止 BG copy。  
- SM-BG-002（Observed）: 只有在 queue 與 command 允許條件下，FTL 才可執行 cleanGR/GC/WL。  
- SM-BG-003（Inferred）: BG suspend 邊界為「不新增不可中斷 NAND write phase」，resume 從下一可排程點重入。

---

## 9. Hardware Usage Specification（升級為 driver-ready sequence）

### 9.1 Required HW blocks
- HW-001（Observed）: SYS (clock/PMU/RTT/GPIO/WDT/UART)
- HW-002（Observed）: SATA APU register space
- HW-003（Observed）: Doorbell queue block
- HW-004（Observed）: SS DMAC
- HW-005（Observed）: FLH/Flash controller
- HW-006（Observed）: DDR controller + ECC/CRC

### 9.2 Register programming sequences

#### RSEQ-APU-001: Command completion handshake
1. 寫 `HB_STATUS` 與 `HB_ERROR`。  
2. 置位 `HB_REG_REQ`（必要時含 `H_REG_IBIT`）。  
3. Poll `HB_REG_REQ` 直到硬體清除。  
4. 僅在成功握手後標記 command finish。  
- Timeout handling: Unknown（需平台定義），建議設 guard timer 並記錄 fail log。

#### RSEQ-RST-001: Reset re-initialization
1. 停止 BG / RW (`M_StopBG_Copy` / `HandleStopRW`)。  
2. `Init_Reset()`。  
3. `Sata_InterfaceReg_Init()`。  
4. 重新送一次 `HB_REG_REQ` 並等待 clear。  
5. 依 security/power/sanitize/NCQ 條件重啟 HW D2H NCQ。

#### RSEQ-DDR-001: ECC interrupt enable/clear
1. 清除 pending interrupt status。  
2. 設定 `DDR3_ECC_CTRL` 的 zone error interrupt enable bits。  
3. 註冊 DDR ISR。  
4. 發生錯誤時使用對應 clear bits 進行 W1C/RW clear sequence。  
- Precondition: 需符合 DDR spec 的 empty/安全切換條件。

#### RSEQ-DBQ-001: Doorbell queue setup
1. 設定 QBASE（依 spec alignment）。  
2. 設定 WPTR/RPTR/PIU。  
3. 控制 register enable/reset queue。  
4. FW/HW 依 readable/writable count 進行門鈴更新。  
- Race rule（Inferred）: queue pointer 更新需由單一 owner 執行，避免雙寫。

### 9.3 Register access safety rules
- HW-010（Observed）: 必須遵循 W1C/RW1C/RW1CL 語義。  
- HW-011（Observed）: queue base address 必須符合 alignment。  
- HW-012（Observed）: interrupt ack 需先讀判斷來源再 W1C，避免錯清。  
- HW-013（Inferred）: 所有 polling 必須具 timeout + fail path（assert/log/reset escalation）。

---

## 10. Data Structures and Persistence Contract（新增）

### 10.1 In-memory data structure contracts
- DS-001（Observed）: `BufferQueue_t` 為 host request staging 的 source-of-truth；index key = queue link + vir4k index。  
- DS-002（Observed）: `ReadBQInformation_t` 是 read-side table/search context，生命周期綁定 BQ。  
- DS-003（Observed）: `FlashQueue_t` 是 NAND execution descriptor，source-of-truth 在 flaTask。  
- DS-004（Observed）: `FTL_t` bitfield 為背景工作與 flush/GC/WL 的全域協調 state。  
- DS-005（Observed）: `FlashQueueInfo_t` / `BufferQueueInfo_t` 維持 link first/last/num 與 active depth。

### 10.2 Consistency rules
- DS-010（Observed）: queue count 與 link topology 必須一致；不一致應視為嚴重錯誤。  
- DS-011（Observed）: buffer-order (`ubUseBufferOrder`) mismatch 時不得觸發資料搬移。  
- DS-012（Inferred）: 對同一 logical page 更新 L2P/P2L 時需具原子性邊界（至少 command completion boundary）。

### 10.3 On-media persistent metadata contract
> 下列為「可重建最低需求」，但原始精確格式仍有 Unknown。

- PM-001（Observed+Unknown）: 系統必須持久化至少：L2P/P2L 映射、bad block state、RUT/VT/init info、ATA config。  
- PM-002（Inferred）: 需有 metadata header（magic/version/length/checksum/sequence）以支援 recovery 選主。  
- PM-003（Inferred）: 需有 clean/dirty close marker，決定 boot 時走正常載入或 rebuild/scanning。  
- PM-004（Inferred）: 需有 journal/checkpoint 機制保證 reset/pfail 後至少回到最後一致點。  
- PM-005（Unknown）: 原 FW superblock layout、entry encoding、CRC polynomial 未完全可見，需人工補件。

---

## 11. Timing / Threshold / Policy Specification（新增）

### 11.1 Observed constants
- POL-001（Observed）: RTT1 init limit 設為 `1000`（1ms tick）。  
- POL-002（Observed）: `AUTOPOL_TIMEOUT_MILLISEC = 10000`。  
- POL-003（Observed）: `PFAIL` 測試路徑存在 `>=250` 計時條件（RDT）。  
- POL-004（Observed）: init state 枚舉固定為 `BYTE_INIT_START_SCAN..BYTE_INIT_PRELOAD_L2P`。

### 11.2 Runtime policy constraints
- POL-010（Observed）: host command 到達時 BG copy 必須可中斷。  
- POL-011（Observed）: sanitize/security lock 狀態下，多數資料命令需 ABRT。  
- POL-012（Inferred）: queue wait/polling 必須具超時後可診斷行為（log + reset path）。

### 11.3 Unknown policy parameters（不得腦補）
- POL-U-001: GC free-block watermark / hysteresis 數值。
- POL-U-002: WL 觸發與 budget。
- POL-U-003: 各 NAND read-retry 次數/曲線最終量產表。
- POL-U-004: sanitize progress update cadence/timeout acceptance。
- POL-U-005: power-fail hold-up 物理時間與最小保護寫入窗口。

---

## 12. Error Handling and Recovery

- ERR-001（Observed）: 錯誤類別至少含 protocol（ABRT/UNC/IDNF）、link/reset、DDR ECC/CRC/parity、NAND PF/timeout。  
- ERR-002（Observed）: 錯誤回報路徑需寫 HB status/error 並依條件記錄 command/fail log。  
- ERR-003（Observed）: reset/link lost 先 stop 再 re-init，不可直接帶著在途 BG 狀態切換。  
- ERR-004（Observed）: parity/stack monitor 可導向 assert/reset。  
- ERR-005（Inferred）: fail escalation 應至少包含 retry budget、abort、controller reset 三層。

---

## 13. Configuration and Variants

- CFG-001（Observed）: compile-time 宏控制非常多（BURNER/RDT/PFAIL/TLC/vendor/debug）。  
- CFG-002（Observed）: NAND vendor/tech 影響 FPU sequence/retry table/plane policy。  
- CFG-003（Observed）: SKU 若未 freeze macro set，行為差異可非常大。  
- CFG-004（Unknown）: 量產 SKU 對照表缺失，需補 build manifest。

---

## 14. Verification Specification（新增）

### 14.1 Host command acceptance matrix
- TV-001（Observed）: 每個 ATA opcode 必須驗證「合法條件 / ABRT 條件 / status/error register」。
- TV-002（Observed）: sanitize/security lock/freeze 狀態下，驗證命令 gating 是否符合預期。
- TV-003（Observed）: SMART read/write log、identify、flush、trim、FUA 必須有 golden response 檢查。

### 14.2 Concurrency/reset/pfail matrix
- TV-010（Observed）: reset during I/O：驗證 stop-RW、HB 狀態回復、命令完成語義一致。  
- TV-011（Observed）: host command during BG：驗證 BG suspend/resume 與資料一致性。  
- TV-012（Observed）: pfail PRE_LOCK/LOCKED/PRE_RELEASE 轉移與資料面停機邊界。

### 14.3 HW fault injection matrix
- TV-020（Observed）: DDR ECC/CRC interrupt 注入與 clear/ack 流程。  
- TV-021（Observed）: parity/stack monitor interrupt path。  
- TV-022（Inferred）: queue pointer race/overflow/underflow 防護測試。

### 14.4 Performance & endurance acceptance
- TV-030（Unknown）: IOPS/latency/QoS/WAF 目標值需外部規格補充。  
- TV-031（Inferred）: 至少建立前景優先（BG可中斷）與 timeout 無死鎖的驗證門檻。

---

## 15. Discrepancies (Code vs Spec)

- DSC-001（Observed）: sanitize crypto scramble / block erase 在 code path 為 ABRT，不等於完整 sanitize feature set。  
- DSC-002（Inferred）: hw_spec 能力 > firmware實際使用子集合。  
- DSC-003（Observed）: 多數 ATA opcode 仍 NotImplement。

---

## 16. Re-implementation Guidance (Normative)

- REQ-900: 先鎖定 command-visible correctness（status/error/identify/smart/security/sanitize）。
- REQ-901: 先重建 BQ/FQ/ownership/rollback 契約，再優化 GC/WL performance。
- REQ-902: driver 層必須實作 register sequence + timeout + ack/race 規則。
- REQ-903: Unknown 區（on-media format、policy thresholds、SKU macros）在補件前不得自行假設為最終需求。
