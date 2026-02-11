# S11

SATA SSD Firmware。

## 專案總覽（overview）
S11 是一套面向 SATA SSD 控制器的韌體專案，核心涵蓋：
- SATA 指令接收與資料路徑管理。
- FTL（Flash Translation Layer）位址轉換與初始化。
- NAND Flash 操作、錯誤處理、壞塊/失效情境管理。
- 安全與資料清除（sanitize / security）流程。
- RDT/DDR/SMART 等維運與診斷功能。

## 完整專案目錄架構
```text
S11/
├── README.md
└── src/
    ├── DCO.c
    ├── FTL.h
    ├── Flash.c
    ├── HPA.c
    ├── IO.h
    ├── Ph_SQL.c
    ├── RDT.c
    ├── RDT.h
    ├── RDTVarDef.h
    ├── RW.c
    ├── S11_FPU.h
    ├── Satacmd.c
    ├── Security.c
    ├── Setup.h
    ├── STYLETOCK.BAT
    ├── Vardef.h
    ├── cmdtbl.h
    ├── ddr.c
    ├── ddr.h
    ├── flafail.c
    ├── flainit.c
    ├── ftl.c
    ├── ftlcommon.c
    ├── ftlinit.c
    ├── ftltool.c
    ├── header.h
    ├── main.c
    ├── make.bat
    ├── parasoft.h
    ├── sanitize.c
    ├── smart.c
    └── vender.c
```

## 功能模組與職責說明

### 1) 系統啟動與平台設定
- `main.c`：韌體進入點、執行緒/中斷/時脈與系統初始化流程。
- `Setup.h`、`header.h`、`Vardef.h`：全域設定、平台常數與共用宣告。

### 2) SATA 指令與主機介面
- `Satacmd.c`：ATA/SATA 指令分派與流程控制。
- `cmdtbl.h`：命令表與命令處理對應定義。
- `RW.c`：讀寫命令資料路徑與傳輸流程。
- `IO.h`：I/O 介面與硬體存取輔助巨集/宣告。

### 3) FTL（Flash Translation Layer）核心
- `ftl.c`：FTL 核心邏輯（LBA 與實體快閃位址對應）。
- `ftlinit.c`、`flainit.c`：FTL/Flash 相關初始化與開機重建流程。
- `ftlcommon.c`：FTL 共用函式與通用工具。
- `FTL.h`、`ftltool.c`：FTL 資料結構與輔助工具。

### 4) NAND Flash 管理
- `Flash.c`：NAND 操作流程（讀/寫/擦/狀態）。
- `flafail.c`：Flash 失敗情境處理、錯誤路徑與復原輔助。
- `vender.c`：不同 Flash vendor 相容/參數處理。

### 5) 容量與 LBA 空間管理
- `HPA.c`：HPA（Host Protected Area）容量可見區域控制。
- `DCO.c`：DCO（Device Configuration Overlay）裝置能力/配置限制。

### 6) 安全與資料清除
- `Security.c`：ATA Security 相關機制（鎖定/解鎖/權限流程）。
- `sanitize.c`：資料清除（sanitize）流程實作。

### 7) 健康監控與維運
- `smart.c`：SMART 資訊統計與回報。
- `Ph_SQL.c`：韌體內部維護/統計資料處理（專案內部使用模組）。

### 8) 記憶體/除錯/診斷
- `ddr.c`、`ddr.h`：DDR 相關設定與控制。
- `RDT.c`、`RDT.h`、`RDTVarDef.h`：RDT 診斷/測試資料結構與流程。
- `S11_FPU.h`：FPU（Flash Program/Pattern Unit）相關宣告。

### 9) 建置與靜態檢查支援
- `make.bat`、`STYLETOCK.BAT`：Windows 批次建置/檢查流程。
- `parasoft.h`：靜態分析（Parasoft）相關抑制或設定宣告。

---

> 備註：本 README 依照檔名與程式分工進行模組化整理；若後續新增子系統（例如背景 GC、WL、PLP、NV cache 管理），建議同步擴充本節以維持可維護性。
