# IO.h 完整巨集定義與用途開發文件（全量）

> 本文件依 `src/IO.h` **自動抽取所有 `#define`** 產生，目標是完整列出每個巨集在對應條件編譯下的數值（或運算式）與用途說明。

## 1. 統計總覽

- 巨集總數：**3798**
- `FPU_PTR_*`：**396**
- `MARK_FPU_*`：**12**
- `SET_FPU_*`：**3**
- 暫存器/位址類：**483**
- 其他：**2904**

## 2. 條件編譯分支說明

- `Hynix3DV6 || YMTC_JGS`：多數 `FPU_PTR_*` 以 `FPU_SEQ_PTR16(PTR_XXX)` 映射，值來自序列表索引。
- `Hynix`：多數 `FPU_PTR_*` 使用 `前一段位址 + 前一段長度` 的鏈式寫法。
- `ELSE` 分支：多數 `FPU_PTR_*` 為固定十六進位位址。
- `MicronFlashOnly / B0KB / FORCE_AUTOPOL_TIMEOUT / TLC`：控制 Micron 與 TLC 特化路徑。

## 3. 全量巨集清單（逐項列出數值與用途）

| 行號 | 條件組態 | 巨集名稱 | 數值/運算式 | 用途說明 |
|---:|---|---|---|---|
| 2 | `!defined(__IO_H__)` | `__IO_H__` | `(empty)` | 通用常數/硬體欄位定義 |
| 5 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_CFF` | `FPU_SEQ_PTR16(PTR_CFF)` | FPU 命令序列指標；註解：===== FFh ===== |
| 8 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C70_POL_MK40` | `FPU_SEQ_PTR16(PTR_C70_POL_MK40)` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, cache ready, for FPU ===== |
| 9 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C70_POL_MK40_C00` | `FPU_SEQ_PTR16(PTR_C70_POL_MK40_C00)` | FPU 輪詢/狀態讀取流程指標 |
| 12 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C70_POL_MK20` | `FPU_SEQ_PTR16(PTR_C70_POL_MK20)` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, true ready, for FPU ===== |
| 13 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C70_POL_MK20_C00` | `FPU_SEQ_PTR16(PTR_C70_POL_MK20_C00)` | FPU 輪詢/狀態讀取流程指標 |
| 16 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C70_DAT_MK40` | `FPU_SEQ_PTR16(PTR_C70_DAT_MK40)` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, cache ready(true ready), for autopolling ===== |
| 17 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C70_DAT_MK20` | `FPU_SEQ_PTR16(PTR_C70_DAT_MK20)` | FPU 輪詢/狀態讀取流程指標 |
| 20 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C71_DAT_MK40` | `FPU_SEQ_PTR16(PTR_C71_DAT_MK40)` | FPU 輪詢/狀態讀取流程指標；註解：===== 71h, cache ready(true ready), for autopolling ===== |
| 21 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C71_DAT_MK20` | `FPU_SEQ_PTR16(PTR_C71_DAT_MK20)` | FPU 輪詢/狀態讀取流程指標 |
| 24 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C73_DAT_MK40` | `FPU_SEQ_PTR16(PTR_C73_DAT_MK40)` | FPU 輪詢/狀態讀取流程指標；註解：===== 73h, cache ready(true ready), for autopolling ===== |
| 25 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C73_DAT_MK20` | `FPU_SEQ_PTR16(PTR_C73_DAT_MK20)` | FPU 輪詢/狀態讀取流程指標 |
| 28 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C70_DAT_MK0F` | `FPU_SEQ_PTR16(PTR_C70_DAT_MK0F)` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, one plane fail (pA) ===== |
| 32 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C71_DAT_MK1F` | `FPU_SEQ_PTR16(PTR_C71_DAT_MK1F)` | FPU 輪詢/狀態讀取流程指標；註解：#define FPU_PTR_C70_DAT_MK03 \| ===== 71h, 2 planes fail (pA/pB) ===== |
| 35 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C73_DAT_MK1F` | `FPU_SEQ_PTR16(PTR_C73_DAT_MK1F)` | FPU 輪詢/狀態讀取流程指標；註解：===== 73h, 2 planes fail (pC/pD) ===== |
| 38 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_AG_CA2_C60_A3_CD0)` | FPU Erase/Block 操作流程指標；註解：=====  (A2h) 60h xxx D0h ===== |
| 39 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 41 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 42 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 44 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_AG_CA2_C60_A3_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 45 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C60_A3_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_AG_C60_A3_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 48 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C80_A5_DW` | `FPU_SEQ_PTR16(PTR_AG_CA2_C80_A5_DW)` | FPU Program/DMA 寫入流程指標；註解：===== (A2h) 80h xxxxx DMA ===== |
| 49 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C80_A5_DW` | `FPU_SEQ_PTR16(PTR_AG_C80_A5_DW)` | FPU Program/DMA 寫入流程指標 |
| 51 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C80_A5_DW_C11` | `FPU_SEQ_PTR16(PTR_AG_CA2_C80_A5_DW_C11)` | FPU Program/DMA 寫入流程指標 |
| 53 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C80_A5_DW_C11` | `FPU_SEQ_PTR16(PTR_AG_C80_A5_DW_C11)` | FPU Program/DMA 寫入流程指標 |
| 54 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C11` | `FPU_SEQ_PTR16(PTR_C11)` | FPU 命令序列指標 |
| 57 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C80_A5_DW_C15` | `FPU_SEQ_PTR16(PTR_AG_CA2_C80_A5_DW_C15)` | FPU Program/DMA 寫入流程指標；註解：===== (A2h) 80h xxxxx DMA 15h |
| 58 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C80_A5_DW_C15` | `FPU_SEQ_PTR16(PTR_AG_C80_A5_DW_C15)` | FPU Program/DMA 寫入流程指標 |
| 61 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C80_A5_DW_C10` | `FPU_SEQ_PTR16(PTR_AG_CA2_C80_A5_DW_C10)` | FPU Program/DMA 寫入流程指標；註解：=====  (A2h) 80h xxxxx DMA 10h ===== |
| 62 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C80_A5_DW_C10` | `FPU_SEQ_PTR16(PTR_AG_C80_A5_DW_C10)` | FPU Program/DMA 寫入流程指標 |
| 63 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C10` | `FPU_SEQ_PTR16(PTR_C10)` | FPU 命令序列指標 |
| 65 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C81_A5_DW_C11` | `FPU_SEQ_PTR16(PTR_AG_CA2_C81_A5_DW_C11)` | FPU Program/DMA 寫入流程指標 |
| 66 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C81_A5_DW_C11` | `FPU_SEQ_PTR16(PTR_AG_C81_A5_DW_C11)` | FPU Program/DMA 寫入流程指標 |
| 68 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C81_A5_DW_C15` | `FPU_SEQ_PTR16(PTR_AG_CA2_C81_A5_DW_C15)` | FPU Program/DMA 寫入流程指標 |
| 69 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C81_A5_DW_C15` | `FPU_SEQ_PTR16(PTR_AG_C81_A5_DW_C15)` | FPU Program/DMA 寫入流程指標 |
| 71 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C81_A5_DW_C10` | `FPU_SEQ_PTR16(PTR_AG_CA2_C81_A5_DW_C10)` | FPU Program/DMA 寫入流程指標 |
| 72 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C81_A5_DW_C10` | `FPU_SEQ_PTR16(PTR_AG_C81_A5_DW_C10)` | FPU Program/DMA 寫入流程指標 |
| 75 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15` | `FPU_SEQ_PTR16(PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15)` | FPU 輪詢/狀態讀取流程指標；註解：===== (A2h) 11h waitDummy 81h xxxxx DMA 15h |
| 76 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15` | `FPU_SEQ_PTR16(PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15)` | FPU 輪詢/狀態讀取流程指標 |
| 77 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_C15` | `FPU_SEQ_PTR16(PTR_C11_C70_POL_MK40_C81_A5_DW_C15)` | FPU 輪詢/狀態讀取流程指標 |
| 80 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW` | `FPU_SEQ_PTR16(PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW)` | FPU 輪詢/狀態讀取流程指標；註解：===== (A2h) 11h waitDummy 81h xxxxx DMA |
| 81 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW` | `FPU_SEQ_PTR16(PTR_AG_C11_C70_POL_MK40_C81_A5_DW)` | FPU 輪詢/狀態讀取流程指標 |
| 82 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C11_C70_POL_MK40_C81_A5_DW` | `FPU_SEQ_PTR16(PTR_C11_C70_POL_MK40_C81_A5_DW)` | FPU 輪詢/狀態讀取流程指標 |
| 85 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C00_A5_C30` | `FPU_SEQ_PTR16(PTR_AG_CA2_C00_A5_C30)` | FPU Read/提交命令流程指標；註解：===== (A2h) 00h xxxxx 30h |
| 86 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C30` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C30)` | FPU Read/提交命令流程指標 |
| 87 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C00_A5_C30` | `FPU_SEQ_PTR16(PTR_C00_A5_C30)` | FPU Read/提交命令流程指標 |
| 90 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `FPU_SEQ_PTR16(PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30)` | FPU 輪詢/狀態讀取流程指標；註解：--------------------- |
| 91 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30)` | FPU 輪詢/狀態讀取流程指標 |
| 92 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `FPU_SEQ_PTR16(PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30)` | FPU 輪詢/狀態讀取流程指標 |
| 93 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30)` | FPU 輪詢/狀態讀取流程指標 |
| 94 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `FPU_SEQ_PTR16(PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30)` | FPU 輪詢/狀態讀取流程指標 |
| 95 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30)` | FPU 輪詢/狀態讀取流程指標 |
| 97 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C00_A5_C31` | `FPU_SEQ_PTR16(PTR_AG_CA2_C00_A5_C31)` | FPU Read/提交命令流程指標 |
| 98 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C31` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C31)` | FPU Read/提交命令流程指標 |
| 100 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `FPU_SEQ_PTR16(PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31)` | FPU 輪詢/狀態讀取流程指標 |
| 101 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31)` | FPU 輪詢/狀態讀取流程指標 |
| 102 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `FPU_SEQ_PTR16(PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31)` | FPU 輪詢/狀態讀取流程指標 |
| 103 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31)` | FPU 輪詢/狀態讀取流程指標 |
| 104 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `FPU_SEQ_PTR16(PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31)` | FPU 輪詢/狀態讀取流程指標 |
| 105 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31)` | FPU 輪詢/狀態讀取流程指標 |
| 107 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C00_A5_C32` | `FPU_SEQ_PTR16(PTR_AG_CA2_C00_A5_C32)` | FPU Read/提交命令流程指標 |
| 108 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C32` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C32)` | FPU Read/提交命令流程指標 |
| 110 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_CA2_C31` | `FPU_SEQ_PTR16(PTR_CA2_C31)` | FPU Read/提交命令流程指標 |
| 111 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C31` | `FPU_SEQ_PTR16(PTR_C31)` | FPU Read/提交命令流程指標 |
| 113 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_CA2_C3F` | `FPU_SEQ_PTR16(PTR_CA2_C3F)` | FPU 命令序列指標 |
| 114 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C3F` | `FPU_SEQ_PTR16(PTR_C3F)` | FPU 命令序列指標 |
| 116 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `FPU_SEQ_PTR16(PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30)` | FPU Erase/Block 操作流程指標 |
| 117 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `FPU_SEQ_PTR16(PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30)` | FPU Erase/Block 操作流程指標 |
| 118 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `FPU_SEQ_PTR16(PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_C30)` | FPU Erase/Block 操作流程指標 |
| 119 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `FPU_SEQ_PTR16(PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30)` | FPU Erase/Block 操作流程指標 |
| 120 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_C30` | `FPU_SEQ_PTR16(PTR_AG_CA2_C60_A3_AG_C60_A3_C30)` | FPU Erase/Block 操作流程指標 |
| 121 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C60_A3_AG_C60_A3_C30` | `FPU_SEQ_PTR16(PTR_AG_C60_A3_AG_C60_A3_C30)` | FPU Erase/Block 操作流程指標 |
| 124 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_DR` | `FPU_SEQ_PTR16(PTR_AG_DR)` | FPU 命令序列指標；註解：for fixed data |
| 125 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_DR` | `FPU_SEQ_PTR16(PTR_AG_C00_DR)` | FPU Read/提交命令流程指標 |
| 127 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C00_A5_C05_A2_CE0_DR` | `FPU_SEQ_PTR16(PTR_AG_C00_A5_C05_A2_CE0_DR)` | FPU Read/提交命令流程指標 |
| 128 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_C00_A5_C05_A2_CE0_DR` | `FPU_SEQ_PTR16(PTR_C00_A5_C05_A2_CE0_DR)` | FPU Read/提交命令流程指標 |
| 131 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_NOP` | `FPU_SEQ_PTR16(PTR_NOP)` | FPU 命令序列指標；註解：for doing dummy MT after UNC |
| 134 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_IBF_CORRECT` | `FPU_SEQ_PTR16(PTR_AG_IBF_CORRECT)` | FPU 錯誤修正/原始存取流程指標；註解：for error handle |
| 135 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION` | `FPU_SEQ_PTR16(PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION)` | FPU 錯誤修正/原始存取流程指標 |
| 137 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `SET_FPU_IBF_RESTORE_BACKUP(ubIBFPtr, ubFrameNum, ubDirection, ubMode)` | `(((ubIBFPtr&0x7)<<5)\|((ubFrameNum&0x7)<<2)\|((ubDirection&0x1)<<1)\|(ubMode&0x1))` | FPU 參數組合/封包輔助巨集 |
| 138 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_RAW` | `FPU_SEQ_PTR16(PTR_AG_RAW)` | FPU 錯誤修正/原始存取流程指標 |
| 139 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_SOFTBIT_CORRECTION` | `FPU_SEQ_PTR16(PTR_SOFTBIT_CORRECTION)` | FPU 錯誤修正/原始存取流程指標 |
| 141 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_CC6_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_CC6_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 142 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_CC6_AG_C60_A3_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 143 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_CC6_AG_C60_A3_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 145 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 146 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_AG_C60_A3_CD1_AG_C60_A3_CD0` | `FPU_SEQ_PTR16(PTR_AG_C60_A3_CD1_AG_C60_A3_CD0)` | FPU Erase/Block 操作流程指標 |
| 147 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `FPU_PTR_CFC` | `FPU_SEQ_PTR16(PTR_CFC)` | FPU 命令序列指標 |
| 153 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `MARK_FPU_C80_C10C15` | `0` | FPU 流程分類標記常數 |
| 154 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `MARK_FPU_C80_NonC10C15` | `1` | FPU 流程分類標記常數 |
| 155 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `MARK_FPU_NonC80_C10C15` | `2` | FPU 流程分類標記常數 |
| 156 | `!defined(__IO_H__) && Hynix3DV6 \|\| YMTC_JGS` | `MARK_FPU_NonC80_NonC10C15` | `3` | FPU 流程分類標記常數 |
| 161 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_CFF` | `0x0000` | FPU 命令序列指標；註解：===== FFh ===== |
| 162 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_CFF_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導） |
| 165 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_POL_MK40` | `(FPU_PTR_CFF+FPU_PTR_CFF_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, cache ready, for FPU ===== |
| 166 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_POL_MK40_LEN` | `8` | FPU 輪詢/狀態讀取流程指標 |
| 168 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_POL_MK40_C00` | `(FPU_PTR_C70_POL_MK40+FPU_PTR_C70_POL_MK40_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 169 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_POL_MK40_C00_LEN` | `10` | FPU 輪詢/狀態讀取流程指標 |
| 172 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_POL_MK20` | `(FPU_PTR_C70_POL_MK40_C00+FPU_PTR_C70_POL_MK40_C00_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, true ready, for FPU ===== |
| 173 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_POL_MK20_LEN` | `8` | FPU 輪詢/狀態讀取流程指標 |
| 174 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_POL_MK20_C00` | `(FPU_PTR_C70_POL_MK20+FPU_PTR_C70_POL_MK20_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 175 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_POL_MK20_C00_LEN` | `10` | FPU 輪詢/狀態讀取流程指標 |
| 178 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_DAT_MK40` | `(FPU_PTR_C70_POL_MK20_C00+FPU_PTR_C70_POL_MK20_C00_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, cache ready(true ready), for autopolling ===== |
| 179 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_DAT_MK40_LEN` | `12` | FPU 輪詢/狀態讀取流程指標 |
| 180 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_DAT_MK20` | `(FPU_PTR_C70_DAT_MK40+FPU_PTR_C70_DAT_MK40_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 181 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_DAT_MK20_LEN` | `10` | FPU 輪詢/狀態讀取流程指標 |
| 184 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C71_DAT_MK40` | `(FPU_PTR_C70_DAT_MK20+FPU_PTR_C70_DAT_MK20_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== 71h, cache ready(true ready), for autopolling ===== |
| 185 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C71_DAT_MK40_LEN` | `10` | FPU 輪詢/狀態讀取流程指標 |
| 186 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C71_DAT_MK20` | `(FPU_PTR_C71_DAT_MK40+FPU_PTR_C71_DAT_MK40_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 187 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C71_DAT_MK20_LEN` | `12` | FPU 輪詢/狀態讀取流程指標 |
| 190 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C73_DAT_MK40` | `(FPU_PTR_C71_DAT_MK20+FPU_PTR_C71_DAT_MK20_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== 73h, cache ready(true ready), for autopolling ===== |
| 191 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C73_DAT_MK40_LEN` | `10` | FPU 輪詢/狀態讀取流程指標 |
| 192 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C73_DAT_MK20` | `(FPU_PTR_C73_DAT_MK40+FPU_PTR_C73_DAT_MK40_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 193 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C73_DAT_MK20_LEN` | `10` | FPU 輪詢/狀態讀取流程指標 |
| 196 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_DAT_MK0F` | `(FPU_PTR_C73_DAT_MK20+FPU_PTR_C73_DAT_MK20_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, one plane fail (pA) ===== |
| 197 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C70_DAT_MK0F_LEN` | `12` | FPU 輪詢/狀態讀取流程指標 |
| 201 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C71_DAT_MK1F` | `(FPU_PTR_C70_DAT_MK0F+FPU_PTR_C70_DAT_MK0F_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：#define FPU_PTR_C70_DAT_MK03 \| ===== 71h, 2 planes fail (pA/pB) ===== |
| 202 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C71_DAT_MK1F_LEN` | `10` | FPU 輪詢/狀態讀取流程指標 |
| 205 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C73_DAT_MK1F` | `(FPU_PTR_C71_DAT_MK1F+FPU_PTR_C71_DAT_MK1F_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== 73h, 2 planes fail (pC/pD) ===== |
| 206 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C73_DAT_MK1F_LEN` | `10` | FPU 輪詢/狀態讀取流程指標 |
| 209 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_CD0` | `(FPU_PTR_C73_DAT_MK1F+FPU_PTR_C73_DAT_MK1F_LEN)` | FPU Erase/Block 操作流程指標；註解：=====  (A2h) 60h xxx D0h ===== |
| 210 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_CD0_LEN` | `12+4` | FPU Erase/Block 操作流程指標 |
| 211 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_CD0` | `(FPU_PTR_AG_CA2_C60_A3_CD0+FPU_PTR_AG_CA2_C60_A3_CD0_LEN)` | FPU Erase/Block 操作流程指標 |
| 212 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_CD0_LEN` | `10+4` | FPU Erase/Block 操作流程指標 |
| 214 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0` | `(FPU_PTR_AG_C60_A3_CD0+FPU_PTR_AG_C60_A3_CD0_LEN)` | FPU Erase/Block 操作流程指標 |
| 215 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0_LEN` | `30+4` | FPU Erase/Block 操作流程指標 |
| 216 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0` | `(FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0+FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0_LEN)` | FPU Erase/Block 操作流程指標 |
| 217 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0_LEN` | `28+4` | FPU Erase/Block 操作流程指標 |
| 219 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0` | `(FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0+FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0_LEN)` | FPU Erase/Block 操作流程指標 |
| 220 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0_LEN` | `18+4` | FPU Erase/Block 操作流程指標 |
| 221 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_CD0` | `(FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0+FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0_LEN)` | FPU Erase/Block 操作流程指標 |
| 222 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_CD0_LEN` | `16+4` | FPU Erase/Block 操作流程指標 |
| 225 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C80_A5_DW` | `(FPU_PTR_AG_C60_A3_AG_C60_A3_CD0+FPU_PTR_AG_C60_A3_AG_C60_A3_CD0_LEN)` | FPU Program/DMA 寫入流程指標；註解：===== (A2h) 80h xxxxx DMA ===== |
| 226 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C80_A5_DW_LEN` | `14+4` | FPU Program/DMA 寫入流程指標 |
| 227 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C80_A5_DW` | `(FPU_PTR_AG_CA2_C80_A5_DW+FPU_PTR_AG_CA2_C80_A5_DW_LEN)` | FPU Program/DMA 寫入流程指標 |
| 228 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C80_A5_DW_LEN` | `12+4` | FPU Program/DMA 寫入流程指標 |
| 230 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C80_A5_DW_C11` | `(FPU_PTR_AG_C80_A5_DW+FPU_PTR_AG_C80_A5_DW_LEN)` | FPU Program/DMA 寫入流程指標 |
| 231 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C80_A5_DW_C11_LEN` | `16+4` | FPU Program/DMA 寫入流程指標 |
| 233 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C80_A5_DW_C11` | `(FPU_PTR_AG_CA2_C80_A5_DW_C11+FPU_PTR_AG_CA2_C80_A5_DW_C11_LEN)` | FPU Program/DMA 寫入流程指標 |
| 234 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C80_A5_DW_C11_LEN` | `10+4` | FPU Program/DMA 寫入流程指標 |
| 235 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C11` | `(FPU_PTR_AG_C80_A5_DW_C11+FPU_PTR_AG_C80_A5_DW_C11_LEN)` | FPU 命令序列指標 |
| 236 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C11_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導） |
| 239 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C80_A5_DW_C15` | `(FPU_PTR_C11+FPU_PTR_C11_LEN)` | FPU Program/DMA 寫入流程指標；註解：===== (A2h) 80h xxxxx DMA 15h |
| 240 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C80_A5_DW_C15_LEN` | `16+4` | FPU Program/DMA 寫入流程指標 |
| 241 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C80_A5_DW_C15` | `(FPU_PTR_AG_CA2_C80_A5_DW_C15+FPU_PTR_AG_CA2_C80_A5_DW_C15_LEN)` | FPU Program/DMA 寫入流程指標 |
| 242 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C80_A5_DW_C15_LEN` | `14+4` | FPU Program/DMA 寫入流程指標 |
| 245 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C80_A5_DW_C10` | `(FPU_PTR_AG_C80_A5_DW_C15+FPU_PTR_AG_C80_A5_DW_C15_LEN)` | FPU Program/DMA 寫入流程指標；註解：=====  (A2h) 80h xxxxx DMA 10h ===== |
| 246 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C80_A5_DW_C10_LEN` | `16+4` | FPU Program/DMA 寫入流程指標 |
| 247 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C80_A5_DW_C10` | `(FPU_PTR_AG_CA2_C80_A5_DW_C10+FPU_PTR_AG_CA2_C80_A5_DW_C10_LEN)` | FPU Program/DMA 寫入流程指標 |
| 248 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C80_A5_DW_C10_LEN` | `10+4` | FPU Program/DMA 寫入流程指標 |
| 249 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C10` | `(FPU_PTR_AG_C80_A5_DW_C10+FPU_PTR_AG_C80_A5_DW_C10_LEN)` | FPU 命令序列指標 |
| 250 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C10_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導） |
| 252 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C81_A5_DW_C11` | `(FPU_PTR_C10+FPU_PTR_C10_LEN)` | FPU Program/DMA 寫入流程指標 |
| 253 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C81_A5_DW_C11_LEN` | `16` | FPU Program/DMA 寫入流程指標 |
| 254 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C81_A5_DW_C11` | `(FPU_PTR_AG_CA2_C81_A5_DW_C11+FPU_PTR_AG_CA2_C81_A5_DW_C11_LEN)` | FPU Program/DMA 寫入流程指標 |
| 255 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C81_A5_DW_C11_LEN` | `14` | FPU Program/DMA 寫入流程指標 |
| 257 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C81_A5_DW_C15` | `(FPU_PTR_AG_C81_A5_DW_C11+FPU_PTR_AG_C81_A5_DW_C11_LEN)` | FPU Program/DMA 寫入流程指標 |
| 258 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C81_A5_DW_C15_LEN` | `16` | FPU Program/DMA 寫入流程指標 |
| 259 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C81_A5_DW_C15` | `(FPU_PTR_AG_CA2_C81_A5_DW_C15+FPU_PTR_AG_CA2_C81_A5_DW_C15_LEN)` | FPU Program/DMA 寫入流程指標 |
| 260 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C81_A5_DW_C15_LEN` | `14` | FPU Program/DMA 寫入流程指標 |
| 262 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C81_A5_DW_C10` | `(FPU_PTR_AG_C81_A5_DW_C15+FPU_PTR_AG_C81_A5_DW_C15_LEN)` | FPU Program/DMA 寫入流程指標 |
| 263 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C81_A5_DW_C10_LEN` | `16` | FPU Program/DMA 寫入流程指標 |
| 264 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C81_A5_DW_C10` | `(FPU_PTR_AG_CA2_C81_A5_DW_C10+FPU_PTR_AG_CA2_C81_A5_DW_C10_LEN)` | FPU Program/DMA 寫入流程指標 |
| 265 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C81_A5_DW_C10_LEN` | `14` | FPU Program/DMA 寫入流程指標 |
| 268 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15` | `(FPU_PTR_AG_C81_A5_DW_C10+FPU_PTR_AG_C81_A5_DW_C10_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== (A2h) 11h waitDummy 81h xxxxx DMA 15h |
| 269 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15_LEN` | `24` | FPU 輪詢/狀態讀取流程指標 |
| 270 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15` | `(FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15+FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 271 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15_LEN` | `2` | FPU 輪詢/狀態讀取流程指標 |
| 272 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_C15` | `(FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15+FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 273 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_C15_LEN` | `20` | FPU 輪詢/狀態讀取流程指標 |
| 276 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW` | `(FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_C15+FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_C15_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== (A2h) 11h waitDummy 81h xxxxx DMA |
| 277 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_LEN` | `22` | FPU 輪詢/狀態讀取流程指標 |
| 278 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW` | `(FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW+FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 279 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_LEN` | `2` | FPU 輪詢/狀態讀取流程指標 |
| 280 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C11_C70_POL_MK40_C81_A5_DW` | `(FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW+FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 281 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_LEN` | `18` | FPU 輪詢/狀態讀取流程指標 |
| 284 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C30` | `(FPU_PTR_C11_C70_POL_MK40_C81_A5_DW+FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_LEN)` | FPU Read/提交命令流程指標；註解：===== (A2h) 00h xxxxx 30h |
| 285 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C30_LEN` | `12+4` | FPU Read/提交命令流程指標 |
| 286 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C30` | `(FPU_PTR_AG_CA2_C00_A5_C30+FPU_PTR_AG_CA2_C00_A5_C30_LEN)` | FPU Read/提交命令流程指標 |
| 287 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C30_LEN` | `2` | FPU Read/提交命令流程指標 |
| 288 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C00_A5_C30` | `(FPU_PTR_AG_C00_A5_C30+FPU_PTR_AG_C00_A5_C30_LEN)` | FPU Read/提交命令流程指標 |
| 289 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C00_A5_C30_LEN` | `8+4` | FPU Read/提交命令流程指標 |
| 292 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `(FPU_PTR_C00_A5_C30+FPU_PTR_C00_A5_C30_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：--------------------- |
| 293 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN` | `54+4` | FPU 輪詢/狀態讀取流程指標 |
| 294 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30+FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 295 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN` | `52+4` | FPU 輪詢/狀態讀取流程指標 |
| 296 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30+FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 297 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN` | `40+4` | FPU 輪詢/狀態讀取流程指標 |
| 298 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30+FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 299 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN` | `38+4` | FPU 輪詢/狀態讀取流程指標 |
| 300 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30+FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 301 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN` | `26+4` | FPU 輪詢/狀態讀取流程指標 |
| 302 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30+FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 303 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN` | `24+4` | FPU 輪詢/狀態讀取流程指標 |
| 305 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C31` | `(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30+FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30_LEN)` | FPU Read/提交命令流程指標 |
| 306 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C31_LEN` | `12` | FPU Read/提交命令流程指標 |
| 307 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C31` | `(FPU_PTR_AG_CA2_C00_A5_C31+FPU_PTR_AG_CA2_C00_A5_C31_LEN)` | FPU Read/提交命令流程指標 |
| 308 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C31_LEN` | `10` | FPU Read/提交命令流程指標 |
| 310 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `(FPU_PTR_AG_C00_A5_C31+FPU_PTR_AG_C00_A5_C31_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 311 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN` | `54` | FPU 輪詢/狀態讀取流程指標 |
| 312 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31+FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 313 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN` | `52` | FPU 輪詢/狀態讀取流程指標 |
| 314 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31+FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 315 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN` | `40` | FPU 輪詢/狀態讀取流程指標 |
| 316 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31+FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 317 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN` | `38` | FPU 輪詢/狀態讀取流程指標 |
| 318 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31+FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 319 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN` | `26` | FPU 輪詢/狀態讀取流程指標 |
| 320 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31+FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN)` | FPU 輪詢/狀態讀取流程指標 |
| 321 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN` | `24` | FPU 輪詢/狀態讀取流程指標 |
| 323 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32` | `(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31+FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31_LEN)` | FPU Read/提交命令流程指標 |
| 324 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C00_A5_C32_LEN` | `12+4` | FPU Read/提交命令流程指標 |
| 325 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32` | `(FPU_PTR_AG_CA2_C00_A5_C32+FPU_PTR_AG_CA2_C00_A5_C32_LEN)` | FPU Read/提交命令流程指標 |
| 326 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C32_LEN` | `10+4` | FPU Read/提交命令流程指標 |
| 328 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_CA2_C31` | `(FPU_PTR_AG_C00_A5_C32+FPU_PTR_AG_C00_A5_C32_LEN)` | FPU Read/提交命令流程指標 |
| 329 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_CA2_C31_LEN` | `2` | FPU Read/提交命令流程指標 |
| 330 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C31` | `(FPU_PTR_CA2_C31+FPU_PTR_CA2_C31_LEN)` | FPU Read/提交命令流程指標 |
| 331 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C31_LEN` | `4` | FPU Read/提交命令流程指標 |
| 333 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_CA2_C3F` | `(FPU_PTR_C31+FPU_PTR_C31_LEN)` | FPU 命令序列指標 |
| 334 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_CA2_C3F_LEN` | `2` | FPU 序列長度定義（供位址鏈式推導） |
| 335 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C3F` | `(FPU_PTR_CA2_C3F+FPU_PTR_CA2_C3F_LEN)` | FPU 命令序列指標 |
| 336 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C3F_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導） |
| 338 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `(FPU_PTR_C3F+FPU_PTR_C3F_LEN)` | FPU Erase/Block 操作流程指標 |
| 339 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30_LEN` | `30` | FPU Erase/Block 操作流程指標 |
| 340 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `(FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30+FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30_LEN)` | FPU Erase/Block 操作流程指標 |
| 341 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30_LEN` | `28` | FPU Erase/Block 操作流程指標 |
| 342 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `(FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30+FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30_LEN)` | FPU Erase/Block 操作流程指標 |
| 343 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_C30_LEN` | `24` | FPU Erase/Block 操作流程指標 |
| 344 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `(FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_C30+FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_C30_LEN)` | FPU Erase/Block 操作流程指標 |
| 345 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30_LEN` | `22` | FPU Erase/Block 操作流程指標 |
| 346 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_C30` | `(FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30+FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30_LEN)` | FPU Erase/Block 操作流程指標 |
| 347 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_C30_LEN` | `18` | FPU Erase/Block 操作流程指標 |
| 348 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_C30` | `(FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_C30+FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_C30_LEN)` | FPU Erase/Block 操作流程指標 |
| 349 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C60_A3_AG_C60_A3_C30_LEN` | `16` | FPU Erase/Block 操作流程指標 |
| 352 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_DR` | `(FPU_PTR_AG_C60_A3_AG_C60_A3_C30+FPU_PTR_AG_C60_A3_AG_C60_A3_C30_LEN)` | FPU 命令序列指標；註解：for fixed data |
| 353 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_DR_LEN` | `6` | FPU 序列長度定義（供位址鏈式推導） |
| 354 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_DR` | `(FPU_PTR_AG_DR+FPU_PTR_AG_DR_LEN)` | FPU Read/提交命令流程指標 |
| 355 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_DR_LEN` | `10` | FPU Read/提交命令流程指標 |
| 357 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C05_A2_CE0_DR` | `(FPU_PTR_AG_C00_DR+FPU_PTR_AG_C00_DR_LEN)` | FPU Read/提交命令流程指標 |
| 358 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_C00_A5_C05_A2_CE0_DR_LEN` | `2` | FPU Read/提交命令流程指標 |
| 359 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C00_A5_C05_A2_CE0_DR` | `(FPU_PTR_AG_C00_A5_C05_A2_CE0_DR+FPU_PTR_AG_C00_A5_C05_A2_CE0_DR_LEN)` | FPU Read/提交命令流程指標 |
| 360 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_C00_A5_C05_A2_CE0_DR_LEN` | `16` | FPU Read/提交命令流程指標 |
| 363 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_NOP` | `(FPU_PTR_C00_A5_C05_A2_CE0_DR+FPU_PTR_C00_A5_C05_A2_CE0_DR_LEN)` | FPU 命令序列指標；註解：for doing dummy MT after UNC |
| 364 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_NOP_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導） |
| 367 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_IBF_CORRECT` | `(FPU_PTR_NOP+FPU_PTR_NOP_LEN)` | FPU 錯誤修正/原始存取流程指標；註解：for error handle |
| 368 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_IBF_CORRECT_LEN` | `6` | FPU 錯誤修正/原始存取流程指標 |
| 369 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION` | `(FPU_PTR_AG_IBF_CORRECT+FPU_PTR_AG_IBF_CORRECT_LEN)` | FPU 錯誤修正/原始存取流程指標 |
| 370 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION_LEN` | `4` | FPU 錯誤修正/原始存取流程指標 |
| 372 | `!defined(__IO_H__) && ELIF(Hynix)` | `SET_FPU_IBF_RESTORE_BACKUP(ubIBFPtr, ubFrameNum, ubDirection, ubMode)` | `(((ubIBFPtr&0x7)<<5)\|((ubFrameNum&0x7)<<2)\|((ubDirection&0x1)<<1)\|(ubMode&0x1))` | FPU 參數組合/封包輔助巨集 |
| 373 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_RAW` | `(FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION+FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION_LEN)` | FPU 錯誤修正/原始存取流程指標 |
| 374 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_AG_RAW_LEN` | `6` | FPU 錯誤修正/原始存取流程指標 |
| 376 | `!defined(__IO_H__) && ELIF(Hynix)` | `FPU_PTR_SOFTBIT_CORRECTION` | `(FPU_PTR_AG_RAW+FPU_PTR_AG_RAW_LEN)` | FPU 錯誤修正/原始存取流程指標 |
| 379 | `!defined(__IO_H__) && ELIF(Hynix) && TLC` | `FPU_PTR_SOFTBIT_CORRECTION_LEN` | `8` | FPU 錯誤修正/原始存取流程指標 |
| 380 | `!defined(__IO_H__) && ELIF(Hynix) && TLC` | `FPU_PTR_CC6_AG_C60_A3_CD0` | `(FPU_PTR_SOFTBIT_CORRECTION+FPU_PTR_SOFTBIT_CORRECTION_LEN)` | FPU Erase/Block 操作流程指標 |
| 381 | `!defined(__IO_H__) && ELIF(Hynix) && TLC` | `FPU_PTR_CC6_AG_C60_A3_CD0_LEN` | `12` | FPU Erase/Block 操作流程指標 |
| 382 | `!defined(__IO_H__) && ELIF(Hynix) && TLC` | `FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0` | `(FPU_PTR_CC6_AG_C60_A3_CD0+FPU_PTR_CC6_AG_C60_A3_CD0_LEN)` | FPU Erase/Block 操作流程指標 |
| 383 | `!defined(__IO_H__) && ELIF(Hynix) && TLC` | `FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0_LEN` | `18` | FPU Erase/Block 操作流程指標 |
| 384 | `!defined(__IO_H__) && ELIF(Hynix) && TLC` | `FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0` | `FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0 /* Currently not support 4plane TLC treatment. */` | FPU Erase/Block 操作流程指標 |
| 386 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && Hynix` | `FPU_PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0` | `(FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0+FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0_LEN)` | FPU Erase/Block 操作流程指標 |
| 387 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && Hynix` | `FPU_PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0_LEN` | `28+4` | FPU Erase/Block 操作流程指標 |
| 388 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && Hynix` | `FPU_PTR_AG_C60_A3_CD1_AG_C60_A3_CD0` | `(FPU_PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0+FPU_PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0_LEN)` | FPU Erase/Block 操作流程指標 |
| 389 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && Hynix` | `FPU_PTR_AG_C60_A3_CD1_AG_C60_A3_CD0_LEN` | `26+4` | FPU Erase/Block 操作流程指標 |
| 390 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && Hynix` | `FPU_PTR_CFC` | `(FPU_PTR_AG_C60_A3_CD1_AG_C60_A3_CD0+FPU_PTR_AG_C60_A3_CD1_AG_C60_A3_CD0_LEN)` | FPU 命令序列指標 |
| 391 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && Hynix` | `FPU_PTR_CFC_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導）；註解：(FPU_Length-FPU_PTR_CFC) |
| 393 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && ELSE` | `FPU_PTR_C00_A5_CAE` | `(FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0+FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0_LEN)` | FPU Read/提交命令流程指標 |
| 394 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && ELSE` | `FPU_PTR_C00_A5_CAE_LEN` | `8` | FPU Read/提交命令流程指標 |
| 395 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && ELSE` | `FPU_PTR_C3D` | `(FPU_PTR_C00_A5_CAE+FPU_PTR_C00_A5_CAE_LEN)` | FPU 命令序列指標 |
| 396 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && ELSE` | `FPU_PTR_C3D_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導） |
| 397 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && ELSE` | `FPU_PTR_C05_A5_CE0_RAW` | `(FPU_PTR_C3D+FPU_PTR_C3D_LEN)` | FPU 錯誤修正/原始存取流程指標 |
| 398 | `!defined(__IO_H__) && ELIF(Hynix) && TLC && ELSE` | `FPU_PTR_C05_A5_CE0_RAW_LEN` | `12` | FPU 錯誤修正/原始存取流程指標；註解：(FPU_Length-FPU_PTR_C05_A5_CE0_RAW) |
| 401 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE` | `FPU_PTR_SOFTBIT_CORRECTION_LEN` | `4` | FPU 錯誤修正/原始存取流程指標 |
| 404 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW` | `(FPU_PTR_SOFTBIT_CORRECTION+FPU_PTR_SOFTBIT_CORRECTION_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== 11h waitDummy 80h xxxxx DMA |
| 405 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_LEN` | `10` | FPU 輪詢/狀態讀取流程指標 |
| 407 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C15` | `(FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW+FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_LEN)` | FPU 輪詢/狀態讀取流程指標；註解：===== 11h waitDummy 80h xxxxx DMA 15h |
| 408 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C15_LEN` | `22` | FPU 輪詢/狀態讀取流程指標 |
| 410 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_CFC` | `(FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C15+FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C15_LEN)` | FPU 命令序列指標；註解：===== FCh ===== |
| 411 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_CFC_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導） |
| 413 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_CDA` | `(FPU_PTR_CFC+FPU_PTR_CFC_LEN)` | FPU 命令序列指標；註解：===== (DAh) ===== |
| 414 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_CDA_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導） |
| 416 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_CDF` | `(FPU_PTR_CDA+FPU_PTR_CDA_LEN)` | FPU 命令序列指標；註解：===== (DFh) ===== |
| 417 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && MicronFlashOnly` | `FPU_PTR_CDF_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導）；註解：(FPU_Length-FPU_PTR_CDF) |
| 419 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && ELSE` | `FPU_PTR_C00_A5_CAE` | `(FPU_PTR_SOFTBIT_CORRECTION+FPU_PTR_SOFTBIT_CORRECTION_LEN)` | FPU Read/提交命令流程指標 |
| 420 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && ELSE` | `FPU_PTR_C00_A5_CAE_LEN` | `8` | FPU Read/提交命令流程指標 |
| 421 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && ELSE` | `FPU_PTR_C3D` | `(FPU_PTR_C00_A5_CAE+FPU_PTR_C00_A5_CAE_LEN)` | FPU 命令序列指標 |
| 422 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && ELSE` | `FPU_PTR_C3D_LEN` | `4` | FPU 序列長度定義（供位址鏈式推導） |
| 423 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && ELSE` | `FPU_PTR_C05_A5_CE0_RAW` | `(FPU_PTR_C3D+FPU_PTR_C3D_LEN)` | FPU 錯誤修正/原始存取流程指標 |
| 424 | `!defined(__IO_H__) && ELIF(Hynix) && ELSE && ELSE` | `FPU_PTR_C05_A5_CE0_RAW_LEN` | `12` | FPU 錯誤修正/原始存取流程指標；註解：(FPU_Length-FPU_PTR_C05_A5_CE0_RAW) |
| 429 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_CFF` | `0x0000` | FPU 命令序列指標；註解：===== FFh ===== |
| 432 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C70_POL_MK40` | `0x0004` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, cache ready, for FPU ===== |
| 433 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C70_POL_MK40_C00` | `0x000C` | FPU 輪詢/狀態讀取流程指標 |
| 436 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C70_POL_MK20` | `0x0016` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, true ready, for FPU ===== |
| 437 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C70_POL_MK20_C00` | `0x001E` | FPU 輪詢/狀態讀取流程指標 |
| 440 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C70_DAT_MK40` | `0x0028` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, cache ready(true ready), for autopolling ===== |
| 441 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C70_DAT_MK20` | `0x0034` | FPU 輪詢/狀態讀取流程指標 |
| 444 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C71_DAT_MK40` | `0x003E` | FPU 輪詢/狀態讀取流程指標；註解：===== 71h, cache ready(true ready), for autopolling ===== |
| 445 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C71_DAT_MK20` | `0x0048` | FPU 輪詢/狀態讀取流程指標 |
| 448 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C73_DAT_MK40` | `0x0054` | FPU 輪詢/狀態讀取流程指標；註解：===== 73h, cache ready(true ready), for autopolling ===== |
| 449 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C73_DAT_MK20` | `0x005E` | FPU 輪詢/狀態讀取流程指標 |
| 452 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C70_DAT_MK0F` | `0x0068` | FPU 輪詢/狀態讀取流程指標；註解：===== 70h, one plane fail (pA) ===== |
| 456 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C71_DAT_MK1F` | `0x0074` | FPU 輪詢/狀態讀取流程指標；註解：#define FPU_PTR_C70_DAT_MK03 \| ===== 71h, 2 planes fail (pA/pB) ===== |
| 459 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C73_DAT_MK1F` | `0x007E` | FPU 輪詢/狀態讀取流程指標；註解：===== 73h, 2 planes fail (pC/pD) ===== |
| 462 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C60_A3_CD0` | `0x0088` | FPU Erase/Block 操作流程指標；註解：=====  (A2h) 60h xxx D0h ===== |
| 463 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C60_A3_CD0` | `0x0094` | FPU Erase/Block 操作流程指標 |
| 465 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0` | `0x009E` | FPU Erase/Block 操作流程指標 |
| 466 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0` | `0x00BC` | FPU Erase/Block 操作流程指標 |
| 468 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_CD0` | `0x00D8` | FPU Erase/Block 操作流程指標 |
| 469 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C60_A3_AG_C60_A3_CD0` | `0x00EA` | FPU Erase/Block 操作流程指標 |
| 472 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C80_A5_DW` | `0x00FA` | FPU Program/DMA 寫入流程指標；註解：===== (A2h) 80h xxxxx DMA ===== |
| 473 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C80_A5_DW` | `0x0108` | FPU Program/DMA 寫入流程指標 |
| 475 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C80_A5_DW_C11` | `0x0114` | FPU Program/DMA 寫入流程指標 |
| 477 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C80_A5_DW_C11` | `0x0124` | FPU Program/DMA 寫入流程指標 |
| 478 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C11` | `0x012E` | FPU 命令序列指標 |
| 481 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C80_A5_DW_C15` | `0x0132` | FPU Program/DMA 寫入流程指標；註解：===== (A2h) 80h xxxxx DMA 15h |
| 482 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C80_A5_DW_C15` | `0x0142` | FPU Program/DMA 寫入流程指標 |
| 485 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C80_A5_DW_C10` | `0x0150` | FPU Program/DMA 寫入流程指標；註解：=====  (A2h) 80h xxxxx DMA 10h ===== |
| 486 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C80_A5_DW_C10` | `0x0160` | FPU Program/DMA 寫入流程指標 |
| 487 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C10` | `0x016A` | FPU 命令序列指標 |
| 489 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C81_A5_DW_C11` | `0x016E` | FPU Program/DMA 寫入流程指標 |
| 490 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C81_A5_DW_C11` | `0x017E` | FPU Program/DMA 寫入流程指標 |
| 492 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C81_A5_DW_C15` | `0x018C` | FPU Program/DMA 寫入流程指標 |
| 493 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C81_A5_DW_C15` | `0x019C` | FPU Program/DMA 寫入流程指標 |
| 495 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C81_A5_DW_C10` | `0x01AA` | FPU Program/DMA 寫入流程指標 |
| 496 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C81_A5_DW_C10` | `0x01BA` | FPU Program/DMA 寫入流程指標 |
| 499 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15` | `0x01C8` | FPU 輪詢/狀態讀取流程指標；註解：===== (A2h) 11h waitDummy 81h xxxxx DMA 15h |
| 500 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15` | `0x01E0` | FPU 輪詢/狀態讀取流程指標 |
| 501 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_C15` | `0x01E2` | FPU 輪詢/狀態讀取流程指標 |
| 504 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW` | `0x01F6` | FPU 輪詢/狀態讀取流程指標；註解：===== (A2h) 11h waitDummy 81h xxxxx DMA |
| 505 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW` | `0x020C` | FPU 輪詢/狀態讀取流程指標 |
| 506 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C11_C70_POL_MK40_C81_A5_DW` | `0x020E` | FPU 輪詢/狀態讀取流程指標 |
| 509 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C00_A5_C30` | `0x0220` | FPU Read/提交命令流程指標；註解：===== (A2h) 00h xxxxx 30h |
| 510 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C30` | `0x022C` | FPU Read/提交命令流程指標 |
| 511 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C00_A5_C30` | `0x022E` | FPU Read/提交命令流程指標 |
| 514 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `0x0236` | FPU 輪詢/狀態讀取流程指標；註解：--------------------- |
| 515 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `0x026C` | FPU 輪詢/狀態讀取流程指標 |
| 516 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `0x02A0` | FPU 輪詢/狀態讀取流程指標 |
| 517 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `0x02C8` | FPU 輪詢/狀態讀取流程指標 |
| 518 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `0x02EE` | FPU 輪詢/狀態讀取流程指標 |
| 519 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30` | `0x0308` | FPU 輪詢/狀態讀取流程指標 |
| 521 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C00_A5_C31` | `0x0320` | FPU Read/提交命令流程指標 |
| 522 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C31` | `0x032C` | FPU Read/提交命令流程指標 |
| 524 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `0x0336` | FPU 輪詢/狀態讀取流程指標 |
| 525 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `0x036C` | FPU 輪詢/狀態讀取流程指標 |
| 526 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `0x03A0` | FPU 輪詢/狀態讀取流程指標 |
| 527 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `0x03C8` | FPU 輪詢/狀態讀取流程指標 |
| 528 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `0x03EE` | FPU 輪詢/狀態讀取流程指標 |
| 529 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31` | `0x0408` | FPU 輪詢/狀態讀取流程指標 |
| 531 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C00_A5_C32` | `0x0420` | FPU Read/提交命令流程指標 |
| 532 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C32` | `0x042C` | FPU Read/提交命令流程指標 |
| 534 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_CA2_C31` | `0x0436` | FPU Read/提交命令流程指標 |
| 535 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C31` | `0x0438` | FPU Read/提交命令流程指標 |
| 537 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_CA2_C3F` | `0x043C` | FPU 命令序列指標 |
| 538 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C3F` | `0x043E` | FPU 命令序列指標 |
| 540 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `0x0442` | FPU Erase/Block 操作流程指標 |
| 541 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `0x0460` | FPU Erase/Block 操作流程指標 |
| 542 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `0x047C` | FPU Erase/Block 操作流程指標 |
| 543 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30` | `0x0494` | FPU Erase/Block 操作流程指標 |
| 544 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_CA2_C60_A3_AG_C60_A3_C30` | `0x04AA` | FPU Erase/Block 操作流程指標 |
| 545 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C60_A3_AG_C60_A3_C30` | `0x04BC` | FPU Erase/Block 操作流程指標 |
| 548 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_DR` | `0x04CC` | FPU 命令序列指標；註解：for fixed data |
| 549 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_DR` | `0x04D2` | FPU Read/提交命令流程指標 |
| 551 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_C00_A5_C05_A2_CE0_DR` | `0x04DC` | FPU Read/提交命令流程指標 |
| 552 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_C00_A5_C05_A2_CE0_DR` | `0x04DE` | FPU Read/提交命令流程指標 |
| 555 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_NOP` | `0x04EE` | FPU 命令序列指標；註解：for doing dummy MT after UNC |
| 558 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_IBF_CORRECT` | `0x04F2` | FPU 錯誤修正/原始存取流程指標；註解：for error handle |
| 559 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION` | `0x04F8` | FPU 錯誤修正/原始存取流程指標 |
| 564 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW` | `0x528` | FPU 輪詢/狀態讀取流程指標；註解：===== 11h waitDummy 80h xxxxx DMA |
| 567 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C15` | `0x53C` | FPU 輪詢/狀態讀取流程指標；註解：===== 11h waitDummy 80h xxxxx DMA 15h |
| 570 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C10` | `0x5BC` | FPU 輪詢/狀態讀取流程指標；註解：===== 11h waitDummy 80h xxxxx DMA 10h |
| 573 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_CFC` | `0x554` | FPU 命令序列指標；註解：===== FCh ===== |
| 576 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_CDA` | `0x558` | FPU 命令序列指標；註解：===== (DAh) ===== |
| 579 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_CDF` | `0x55C` | FPU 命令序列指標；註解：===== (DFh) ===== |
| 582 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C00_A5_C35` | `0x560` | FPU Read/提交命令流程指標；註解：===== 00 xxxxx 35 |
| 585 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C85_A5_C10` | `0x56A` | FPU 命令序列指標；註解：===== |
| 588 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C85_A5_C11_C70_POL_MK40` | `0x576` | FPU 輪詢/狀態讀取流程指標；註解：===== |
| 591 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C35` | `0x588` | FPU 輪詢/狀態讀取流程指標；註解：===== |
| 593 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C33_A5_C30` | `0x5D2` | FPU Read/提交命令流程指標；註解：===== |
| 596 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C06_A5_CE0` | `0x5DC` | FPU 命令序列指標；註解：===== |
| 598 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB` | `FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C35` | `0x5E6` | FPU 輪詢/狀態讀取流程指標；註解：==== |
| 602 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB && FORCE_AUTOPOL_TIMEOUT` | `FPU_PTR_C70_DAT_MK00` | `0x5FE` | FPU 輪詢/狀態讀取流程指標 |
| 603 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB && FORCE_AUTOPOL_TIMEOUT` | `FPU_PTR_AG_C80_A5_DW_CFC` | `0x608` | FPU Program/DMA 寫入流程指標 |
| 604 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && B0KB && FORCE_AUTOPOL_TIMEOUT` | `FPU_PTR_AG_C00_A5_CFC` | `0x616` | FPU Read/提交命令流程指標 |
| 610 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && ELSE` | `FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW` | `0x506` | FPU 輪詢/狀態讀取流程指標；註解：===== 11h waitDummy 80h xxxxx DMA |
| 612 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && ELSE` | `FPU_PTR_AG_C11_C70_POL_MK40_C80_A5_DW_C15` | `0x51A` | FPU 輪詢/狀態讀取流程指標；註解：===== 11h waitDummy 80h xxxxx DMA 15h |
| 614 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && ELSE` | `FPU_PTR_CFC` | `0x530` | FPU 命令序列指標；註解：===== FCh ===== |
| 616 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && ELSE` | `FPU_PTR_CDA` | `0x534` | FPU 命令序列指標；註解：===== (DAh) ===== |
| 618 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && ELSE` | `FPU_PTR_CDF` | `0x538` | FPU 命令序列指標；註解：===== (DFh) ===== |
| 620 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && ELSE && FORCE_AUTOPOL_TIMEOUT` | `FPU_PTR_C70_DAT_MK00` | `0x53C` | FPU 輪詢/狀態讀取流程指標 |
| 621 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && ELSE && FORCE_AUTOPOL_TIMEOUT` | `FPU_PTR_AG_C80_A5_DW_CFC` | `0x546` | FPU Program/DMA 寫入流程指標 |
| 622 | `!defined(__IO_H__) && ELSE && MicronFlashOnly && ELSE && FORCE_AUTOPOL_TIMEOUT` | `FPU_PTR_AG_C00_A5_CFC` | `0x554` | FPU Read/提交命令流程指標 |
| 628 | `!defined(__IO_H__) && ELSE` | `SET_FPU_IBF_RESTORE_BACKUP(ubIBFPtr, ubFrameNum, ubDirection, ubMode)` | `(((ubIBFPtr&0x7)<<5)\|((ubFrameNum&0x7)<<2)\|((ubDirection&0x1)<<1)\|(ubMode&0x1))` | FPU 參數組合/封包輔助巨集 |
| 629 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_AG_RAW` | `0x04FC` | FPU 錯誤修正/原始存取流程指標 |
| 630 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_SOFTBIT_CORRECTION` | `0x0502` | FPU 錯誤修正/原始存取流程指標 |
| 631 | `!defined(__IO_H__) && ELSE` | `MARK_FPU_C80_C10C15` | `0` | FPU 流程分類標記常數 |
| 632 | `!defined(__IO_H__) && ELSE` | `MARK_FPU_C80_NonC10C15` | `1` | FPU 流程分類標記常數 |
| 633 | `!defined(__IO_H__) && ELSE` | `MARK_FPU_NonC80_C10C15` | `2` | FPU 流程分類標記常數 |
| 634 | `!defined(__IO_H__) && ELSE` | `MARK_FPU_NonC80_NonC10C15` | `3` | FPU 流程分類標記常數 |
| 636 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_CC6_AG_C60_A3_CD0` | `0x50AU` | FPU Erase/Block 操作流程指標 |
| 637 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0` | `0x516U` | FPU Erase/Block 操作流程指標 |
| 638 | `!defined(__IO_H__) && ELSE` | `FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0` | `FPU_PTR_CC6_AG_C60_A3_AG_C60_A3_CD0 /* Currently not support 4plane TLC treatment. */` | FPU Erase/Block 操作流程指標 |
| 641 | `!defined(__IO_H__) && ELSE && Hynix` | `FPU_PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0` | `0x0528` | FPU Erase/Block 操作流程指標 |
| 642 | `!defined(__IO_H__) && ELSE && Hynix` | `FPU_PTR_AG_C60_A3_CD1_AG_C60_A3_CD0` | `0x052A` | FPU Erase/Block 操作流程指標 |
| 643 | `!defined(__IO_H__) && ELSE && Hynix` | `FPU_PTR_CFC` | `0x0544` | FPU 命令序列指標 |
| 647 | `!defined(__IO_H__) && ELSE && TLC` | `FPU_PTR_C00_A5_CAE` | `0x0528` | FPU Read/提交命令流程指標 |
| 648 | `!defined(__IO_H__) && ELSE && TLC` | `FPU_PTR_C3D` | `0x0530` | FPU 命令序列指標 |
| 649 | `!defined(__IO_H__) && ELSE && TLC` | `FPU_PTR_C05_A5_CE0_RAW` | `0x0534` | FPU 錯誤修正/原始存取流程指標 |
| 651 | `!defined(__IO_H__) && ELSE && ELSE` | `FPU_PTR_C00_A5_CAE` | `0x0506` | FPU Read/提交命令流程指標 |
| 652 | `!defined(__IO_H__) && ELSE && ELSE` | `FPU_PTR_C3D` | `0x050E` | FPU 命令序列指標 |
| 653 | `!defined(__IO_H__) && ELSE && ELSE` | `FPU_PTR_C05_A5_CE0_RAW` | `0x0512` | FPU 錯誤修正/原始存取流程指標 |
| 657 | `!defined(__IO_H__)` | `MARK_FPU_C80_C10C15` | `0` | FPU 流程分類標記常數 |
| 658 | `!defined(__IO_H__)` | `MARK_FPU_C80_NonC10C15` | `1` | FPU 流程分類標記常數 |
| 659 | `!defined(__IO_H__)` | `MARK_FPU_NonC80_C10C15` | `2` | FPU 流程分類標記常數 |
| 660 | `!defined(__IO_H__)` | `MARK_FPU_NonC80_NonC10C15` | `3` | FPU 流程分類標記常數 |
| 664 | `!defined(__IO_H__)` | `SYSB` | `((volatile unsigned char *)0x04000000)` | 系統暫存器/記憶體映射位址定義 |
| 665 | `!defined(__IO_H__)` | `SYSW` | `((volatile unsigned short int *)0x04000000)` | 系統暫存器/記憶體映射位址定義 |
| 666 | `!defined(__IO_H__)` | `SYSL` | `((volatile unsigned int *)0x04000000)` | 系統暫存器/記憶體映射位址定義 |
| 667 | `!defined(__IO_H__)` | `SYSLL` | `((volatile unsigned long long *)0x04000000)` | 系統暫存器/記憶體映射位址定義 |
| 669 | `!defined(__IO_H__)` | `SYSTEM_REG` | `0x04000000` | 系統暫存器/記憶體映射位址定義 |
| 675 | `!defined(__IO_H__)` | `SYS0B` | `((volatile unsigned char *)0x04000000)` | 系統暫存器/記憶體映射位址定義；註解：PS3111 SYSTEM 0 Register Define \| =================================================== \| System0 Control Register |
| 676 | `!defined(__IO_H__)` | `SYS0W` | `((volatile unsigned short int *)0x04000000)` | 系統暫存器/記憶體映射位址定義 |
| 677 | `!defined(__IO_H__)` | `SYS0L` | `((volatile unsigned int *)0x04000000)` | 系統暫存器/記憶體映射位址定義 |
| 678 | `!defined(__IO_H__)` | `SYS0O` | `((volatile unsigned long long *)0x04000000)` | 系統暫存器/記憶體映射位址定義 |
| 680 | `!defined(__IO_H__)` | `SYS0B_HWCFG` | `(0x00)` | 系統暫存器/記憶體映射位址定義 |
| 681 | `!defined(__IO_H__)` | `SYS0L_GPIO_LH` | `(0x04>>2)` | 系統暫存器/記憶體映射位址定義 |
| 682 | `!defined(__IO_H__)` | `SYS0W_GPIO_LEFT` | `(0x04>>1)` | 系統暫存器/記憶體映射位址定義 |
| 683 | `!defined(__IO_H__)` | `SYS0B_RRAM_BANK_SEL` | `0x30` | 系統暫存器/記憶體映射位址定義 |
| 684 | `!defined(__IO_H__)` | `SYS0B_RRAM_PAR_ERR` | `0x34` | 系統暫存器/記憶體映射位址定義 |
| 685 | `!defined(__IO_H__)` | `SYS0B_FSIM_EN` | `0x38` | 系統暫存器/記憶體映射位址定義 |
| 687 | `!defined(__IO_H__)` | `SYS0B_SIM_CTRL0_B2` | `(0x42)` | 系統暫存器/記憶體映射位址定義 |
| 688 | `!defined(__IO_H__)` | `SYS0L_SIM_CTRL0` | `(0x40>>2)` | 系統暫存器/記憶體映射位址定義 |
| 689 | `!defined(__IO_H__)` | `SIMREG_CMD_D2H` | `BIT0` | 通用常數/硬體欄位定義 |
| 690 | `!defined(__IO_H__)` | `SIMREG_DLMC_PREFORMAT` | `BIT1` | 通用常數/硬體欄位定義 |
| 691 | `!defined(__IO_H__)` | `SIMREG_COMRESET_JUMP` | `BIT2` | 通用常數/硬體欄位定義 |
| 692 | `!defined(__IO_H__)` | `SIMREG_SATA_PHY_INIT` | `(0x80)` | 通用常數/硬體欄位定義 |
| 694 | `!defined(__IO_H__)` | `SYS0L_SIM_CTRL1` | `(0x44>>2)` | 系統暫存器/記憶體映射位址定義 |
| 696 | `!defined(__IO_H__)` | `SIMREG_VENDER_FORCE_BOOT_CODE` | `(0x02)` | 依原始註解：B0 : Load Code Mode；註解：B0 : Load Code Mode |
| 697 | `!defined(__IO_H__)` | `SIMREG_DEVSLP_LOAD_CODE` | `(0x01)` | 通用常數/硬體欄位定義 |
| 698 | `!defined(__IO_H__)` | `SIMREG_NORMAL_LOAD_CODE` | `(0x00)` | 通用常數/硬體欄位定義 |
| 700 | `!defined(__IO_H__)` | `SIMREG_LOAD_FUNCTION_DONE` | `(0xAA)` | 依原始註解：B1 : Load Function Done；註解：B1 : Load Function Done |
| 701 | `!defined(__IO_H__)` | `SIMREG_FW_DEMAND_LOAD_FUNCTION` | `(0xCC)` | 通用常數/硬體欄位定義 |
| 703 | `!defined(__IO_H__)` | `SIMREG_SET_FEATURE_DONE` | `(0xBB)` | 依原始註解：B2 : Set Feature Mark；註解：B2 : Set Feature Mark |
| 704 | `!defined(__IO_H__)` | `SIMREG_SET_FEATURE_DONE2` | `(0x22)` | 通用常數/硬體欄位定義 |
| 706 | `!defined(__IO_H__)` | `SIMREG_DRAM_RETENTION` | `(0xDD)` | 依原始註解：B3 : DDR retention mode；註解：B3 : DDR retention mode |
| 707 | `!defined(__IO_H__)` | `SYS0L_MEM_RME` | `(0x50>>2)` | 系統暫存器/記憶體映射位址定義 |
| 708 | `!defined(__IO_H__)` | `CR_RRAM_RME` | `BIT16` | 通用常數/硬體欄位定義 |
| 709 | `!defined(__IO_H__)` | `SR_PD1_RAM_RME_1` | `BIT9` | 通用常數/硬體欄位定義 |
| 710 | `!defined(__IO_H__)` | `SR_PD1_RAM_RME_0` | `BIT8` | 通用常數/硬體欄位定義 |
| 711 | `!defined(__IO_H__)` | `SR_PD1_ROM_RME` | `BIT0` | 通用常數/硬體欄位定義 |
| 714 | `!defined(__IO_H__)` | `SYS0L_PAD_CTRL_REG0` | `(0x100>>2)` | 系統暫存器/記憶體映射位址定義；註解：/////////////  GPIO |
| 715 | `!defined(__IO_H__)` | `CR_TMS_S_CB` | `BIT31` | 通用常數/硬體欄位定義 |
| 716 | `!defined(__IO_H__)` | `CR_TMS_SR` | `BIT30` | 通用常數/硬體欄位定義 |
| 717 | `!defined(__IO_H__)` | `CR_TMS_DV_1` | `BIT29` | 通用常數/硬體欄位定義 |
| 718 | `!defined(__IO_H__)` | `CR_TMS_DV_0` | `BIT28` | 通用常數/硬體欄位定義 |
| 719 | `!defined(__IO_H__)` | `XTMS_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 720 | `!defined(__IO_H__)` | `CR_DEVSLP_PD` | `BIT22` | 通用常數/硬體欄位定義 |
| 721 | `!defined(__IO_H__)` | `CR_DEVSLP_PU_1` | `BIT21` | 通用常數/硬體欄位定義 |
| 722 | `!defined(__IO_H__)` | `CR_DEVSLP_PU_0` | `BIT20` | 通用常數/硬體欄位定義 |
| 723 | `!defined(__IO_H__)` | `CR_DEVSLP_VT_1` | `BIT19` | 通用常數/硬體欄位定義 |
| 724 | `!defined(__IO_H__)` | `CR_DEVSLP_VT_0` | `BIT18` | 通用常數/硬體欄位定義 |
| 725 | `!defined(__IO_H__)` | `XDEVSLP_I` | `BIT17` | 通用常數/硬體欄位定義 |
| 726 | `!defined(__IO_H__)` | `CR_FLH_HZ_N` | `BIT0` | 通用常數/硬體欄位定義 |
| 728 | `!defined(__IO_H__)` | `SYS0L_PAD_CTRL_REG1` | `(0x104>>2)` | 系統暫存器/記憶體映射位址定義 |
| 729 | `!defined(__IO_H__)` | `CR_TDO_S_CB` | `BIT31` | 通用常數/硬體欄位定義 |
| 730 | `!defined(__IO_H__)` | `CR_TDO_SR` | `BIT30` | 通用常數/硬體欄位定義 |
| 731 | `!defined(__IO_H__)` | `CR_TDO_DV_1` | `BIT29` | 通用常數/硬體欄位定義 |
| 732 | `!defined(__IO_H__)` | `CR_TDO_DV_0` | `BIT28` | 通用常數/硬體欄位定義 |
| 733 | `!defined(__IO_H__)` | `XTDO_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 735 | `!defined(__IO_H__)` | `CR_TDI_S_CB` | `BIT23` | 通用常數/硬體欄位定義 |
| 736 | `!defined(__IO_H__)` | `CR_TDI_SR` | `BIT22` | 通用常數/硬體欄位定義 |
| 737 | `!defined(__IO_H__)` | `CR_TDI_DV_1` | `BIT21` | 通用常數/硬體欄位定義 |
| 738 | `!defined(__IO_H__)` | `CR_TDI_DV_0` | `BIT20` | 通用常數/硬體欄位定義 |
| 739 | `!defined(__IO_H__)` | `XTDI_I` | `BIT17` | 通用常數/硬體欄位定義 |
| 741 | `!defined(__IO_H__)` | `CR_TCK_S_CB` | `BIT15` | 通用常數/硬體欄位定義 |
| 742 | `!defined(__IO_H__)` | `CR_TCK_SR` | `BIT14` | 通用常數/硬體欄位定義 |
| 743 | `!defined(__IO_H__)` | `CR_TCK_DV_1` | `BIT13` | 通用常數/硬體欄位定義 |
| 744 | `!defined(__IO_H__)` | `CR_TCK_DV_0` | `BIT12` | 通用常數/硬體欄位定義 |
| 745 | `!defined(__IO_H__)` | `XTCK_I` | `BIT9` | 通用常數/硬體欄位定義 |
| 747 | `!defined(__IO_H__)` | `CR_TRSTB_S_CB` | `BIT7` | 通用常數/硬體欄位定義 |
| 748 | `!defined(__IO_H__)` | `CR_TRSTB_SR` | `BIT6` | 通用常數/硬體欄位定義 |
| 749 | `!defined(__IO_H__)` | `CR_TRSTB_DV_1` | `BIT5` | 通用常數/硬體欄位定義 |
| 750 | `!defined(__IO_H__)` | `CR_TRSTB_DV_0` | `BIT4` | 通用常數/硬體欄位定義 |
| 751 | `!defined(__IO_H__)` | `XTRSTB_I` | `BIT1` | 通用常數/硬體欄位定義 |
| 753 | `!defined(__IO_H__)` | `SYS0L_GPIO_CTRL0` | `(0x108>>2)` | 系統暫存器/記憶體映射位址定義 |
| 754 | `!defined(__IO_H__)` | `GPIO_output_driving_balance` | `BIT3` | 周邊控制暫存器/位元欄位定義 |
| 755 | `!defined(__IO_H__)` | `GPIO_GATING` | `BIT2` | 周邊控制暫存器/位元欄位定義 |
| 756 | `!defined(__IO_H__)` | `GPIO_BI_DIRECTION` | `BIT0` | 周邊控制暫存器/位元欄位定義 |
| 758 | `!defined(__IO_H__)` | `SYS0W_GPIO_DATA` | `(0x10c>>1)` | 系統暫存器/記憶體映射位址定義 |
| 759 | `!defined(__IO_H__)` | `SYS0L_GPIO_CTRL1` | `(0x110>>2)` | 系統暫存器/記憶體映射位址定義 |
| 760 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO0` | `0x110` | 系統暫存器/記憶體映射位址定義 |
| 761 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO1` | `0x111` | 系統暫存器/記憶體映射位址定義 |
| 762 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO2` | `0x112` | 系統暫存器/記憶體映射位址定義 |
| 763 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO3` | `0x113` | 系統暫存器/記憶體映射位址定義 |
| 765 | `!defined(__IO_H__)` | `SYS0L_GPIO_CTRL2` | `(0x114>>2)` | 系統暫存器/記憶體映射位址定義 |
| 766 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO4` | `0x114` | 系統暫存器/記憶體映射位址定義 |
| 767 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO5` | `0x115` | 系統暫存器/記憶體映射位址定義 |
| 768 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO6` | `0x116` | 系統暫存器/記憶體映射位址定義 |
| 769 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO7` | `0x117` | 系統暫存器/記憶體映射位址定義 |
| 770 | `!defined(__IO_H__)` | `SYS0L_GPIO_CTRL3` | `(0x118>>2)` | 系統暫存器/記憶體映射位址定義 |
| 771 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO8` | `0x118` | 系統暫存器/記憶體映射位址定義 |
| 772 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO9` | `0x119` | 系統暫存器/記憶體映射位址定義 |
| 773 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO10` | `0x11A` | 系統暫存器/記憶體映射位址定義 |
| 774 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO11` | `0x11B` | 系統暫存器/記憶體映射位址定義 |
| 775 | `!defined(__IO_H__)` | `SYS0L_GPIO_CTRL4` | `(0x11C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 776 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO12` | `0x11C` | 系統暫存器/記憶體映射位址定義 |
| 777 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO13` | `0x11D` | 系統暫存器/記憶體映射位址定義 |
| 778 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO14` | `0x11E` | 系統暫存器/記憶體映射位址定義 |
| 779 | `!defined(__IO_H__)` | `SYS0B_GPIO_CTRL_GPIO15` | `0x11F` | 系統暫存器/記憶體映射位址定義 |
| 781 | `!defined(__IO_H__)` | `SYS0B_CR_GPIO_PD` | `BIT6` | 系統暫存器/記憶體映射位址定義；註解：following are used only for SYS0B_GPIO_CTRL_GPIOxx |
| 782 | `!defined(__IO_H__)` | `SYS0B_CR_GPIO_PU_X_1` | `BIT5` | 系統暫存器/記憶體映射位址定義 |
| 783 | `!defined(__IO_H__)` | `SYS0B_CR_GPIO_PU_X_0` | `BIT4` | 系統暫存器/記憶體映射位址定義 |
| 784 | `!defined(__IO_H__)` | `SYS0B_CR_GPIO_OE` | `BIT2` | 系統暫存器/記憶體映射位址定義 |
| 785 | `!defined(__IO_H__)` | `SYS0B_XGPIO_I` | `BIT1` | 系統暫存器/記憶體映射位址定義 |
| 786 | `!defined(__IO_H__)` | `SYS0B_CR_GPIO_O` | `BIT0` | 系統暫存器/記憶體映射位址定義 |
| 789 | `!defined(__IO_H__)` | `SYS0L_SYS_MUX_CTRL0` | `(0x200>>2)` | 系統暫存器/記憶體映射位址定義；註解：////////MUX |
| 790 | `!defined(__IO_H__)` | `CR_MUX_PHY_SPDSEL` | `BIT8` | 通用常數/硬體欄位定義 |
| 791 | `!defined(__IO_H__)` | `CR_MUX_SPI_CS_OE` | `BIT15` | 通用常數/硬體欄位定義 |
| 792 | `!defined(__IO_H__)` | `SYS0B_CR_MUX_JTAG_SEL` | `0x200` | 系統暫存器/記憶體映射位址定義；註解：2bits |
| 793 | `!defined(__IO_H__)` | `SYS0W_CR_MUX_GPIO` | `(0x202>>1)` | 系統暫存器/記憶體映射位址定義；註解：12bits |
| 795 | `!defined(__IO_H__)` | `SYS0L_SYS_MUX_CTRL1` | `(0x204>>2)` | 系統暫存器/記憶體映射位址定義 |
| 796 | `!defined(__IO_H__)` | `CR_MUX_FLH_WPB` | `BIT5` | 通用常數/硬體欄位定義 |
| 797 | `!defined(__IO_H__)` | `CR_MUX_FLH_COSO` | `BIT6` | 通用常數/硬體欄位定義 |
| 798 | `!defined(__IO_H__)` | `CR_MUX_FLH_ODT` | `BIT7` | 通用常數/硬體欄位定義 |
| 799 | `!defined(__IO_H__)` | `SYS0B_CR_MUX_DB` | `0x206` | 系統暫存器/記憶體映射位址定義；註解：5bits |
| 801 | `!defined(__IO_H__)` | `SYS0L_SYS_MUX_CTRL2` | `(0x208>>2)` | 系統暫存器/記憶體映射位址定義 |
| 802 | `!defined(__IO_H__)` | `SYS0W_XDB_PSEL` | `(0x210>>1)` | 系統暫存器/記憶體映射位址定義；註解：8bits |
| 809 | `!defined(__IO_H__)` | `SYS0L_VDT_CTRL` | `(0x300>>2)` | 系統暫存器/記憶體映射位址定義；註解：#define SET_XDB_PSEL(val)            (SYS0W[SYS0W_XDB_PSEL] \|= (val & 0x0FFF)) \| #define SET_XDB_TSEL(val)            (SYS0B[SYS0B_XDB_TSEL] \|= ((val<<4) & 0xF0)) \| /////////////  PLL |
| 810 | `!defined(__IO_H__)` | `XVDT_TEST_OUT` | `BIT31` | 通用常數/硬體欄位定義 |
| 811 | `!defined(__IO_H__)` | `XVDT_FIO_PG` | `BIT23` | 通用常數/硬體欄位定義 |
| 812 | `!defined(__IO_H__)` | `XVDT_FLH_PG` | `BIT22` | 通用常數/硬體欄位定義 |
| 813 | `!defined(__IO_H__)` | `XVDT_EFU_PG` | `BIT21` | 通用常數/硬體欄位定義 |
| 814 | `!defined(__IO_H__)` | `XVDT_IO0_PG` | `BIT20` | 通用常數/硬體欄位定義 |
| 815 | `!defined(__IO_H__)` | `XVDT_PD0_PG` | `BIT19` | 通用常數/硬體欄位定義 |
| 816 | `!defined(__IO_H__)` | `XVDT_PD1_PG` | `BIT18` | 通用常數/硬體欄位定義 |
| 817 | `!defined(__IO_H__)` | `XVDT_PD2_PG` | `BIT17` | 通用常數/硬體欄位定義 |
| 818 | `!defined(__IO_H__)` | `SR_FLH_FIO_PG` | `BIT16` | 通用常數/硬體欄位定義 |
| 819 | `!defined(__IO_H__)` | `CR_VDT_FIO_PD` | `BIT15` | 通用常數/硬體欄位定義 |
| 820 | `!defined(__IO_H__)` | `CR_VDT_FLH_PD` | `BIT14` | 通用常數/硬體欄位定義 |
| 821 | `!defined(__IO_H__)` | `CR_VDT_PD2_PD` | `BIT10` | 通用常數/硬體欄位定義 |
| 822 | `!defined(__IO_H__)` | `CR_VDT_PD1_PD` | `BIT9` | 通用常數/硬體欄位定義 |
| 823 | `!defined(__IO_H__)` | `CR_VDT_IO0_PD` | `BIT8` | 通用常數/硬體欄位定義 |
| 824 | `!defined(__IO_H__)` | `CR_VDT_FIO_CFG` | `BIT7` | 通用常數/硬體欄位定義 |
| 825 | `!defined(__IO_H__)` | `CR_VDT_FLH_CFG` | `BIT6` | 通用常數/硬體欄位定義 |
| 826 | `!defined(__IO_H__)` | `CR_VDT_PD2_CFG` | `BIT2` | 通用常數/硬體欄位定義 |
| 827 | `!defined(__IO_H__)` | `CR_VDT_PD1_CFG` | `BIT1` | 通用常數/硬體欄位定義 |
| 828 | `!defined(__IO_H__)` | `SYS0B_CR_VDT_TEST_SEL` | `0x303` | 系統暫存器/記憶體映射位址定義 |
| 829 | `!defined(__IO_H__)` | `SET_CR_VDT_TEST_SEL(val)` | `(SYS0B[SYS0B_CR_VDT_TEST_SEL] \|= (val & 0x0F))` | 通用常數/硬體欄位定義 |
| 832 | `!defined(__IO_H__)` | `SYS0L_VDT_TRIM` | `(0x304>>2)` | 系統暫存器/記憶體映射位址定義 |
| 833 | `!defined(__IO_H__)` | `CR_VDT_TRIM_VLD` | `BIT31` | 通用常數/硬體欄位定義 |
| 834 | `!defined(__IO_H__)` | `SYS0W_CR_VDT_TS_TRIM` | `(0x304>>1)` | 系統暫存器/記憶體映射位址定義 |
| 835 | `!defined(__IO_H__)` | `SET_CR_VDT_TS_TRIM(val)` | `(SYS0W[SYS0W_CR_VDT_TS_TRIM] \|= (val & 0x01FF))` | 通用常數/硬體欄位定義 |
| 836 | `!defined(__IO_H__)` | `SYS0B_CR_VDT_TRIM` | `0x305` | 系統暫存器/記憶體映射位址定義 |
| 837 | `!defined(__IO_H__)` | `SET_CR_VDT_TRIM(val)` | `(SYS0B[SYS0B_CR_VDT_TRIM] \|= (val & 0x7E))` | 通用常數/硬體欄位定義 |
| 839 | `!defined(__IO_H__)` | `SYS0L_VDT_TS_CTRL` | `(0x310>>2)` | 系統暫存器/記憶體映射位址定義 |
| 840 | `!defined(__IO_H__)` | `CR_VDT_TS_BYPASS_MODE` | `BIT31` | 通用常數/硬體欄位定義 |
| 841 | `!defined(__IO_H__)` | `XVDT_TS_TEMP_HIGH` | `BIT15` | 通用常數/硬體欄位定義 |
| 842 | `!defined(__IO_H__)` | `XVDT_TS_TCODE_RDY` | `BIT8` | 通用常數/硬體欄位定義 |
| 843 | `!defined(__IO_H__)` | `CR_VDT_TS_MODE` | `BIT7` | 通用常數/硬體欄位定義 |
| 844 | `!defined(__IO_H__)` | `CR_VDT_TS_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 845 | `!defined(__IO_H__)` | `SYS0B_CR_VDT_TS_TEMP_SEL` | `0x313` | 系統暫存器/記憶體映射位址定義 |
| 846 | `!defined(__IO_H__)` | `SET_CR_VDT_TS_TEMP_SEL(val)` | `(SYS0B[SYS0B_CR_VDT_TS_TEMP_SEL] \|= (val & 0x0F))` | 通用常數/硬體欄位定義 |
| 847 | `!defined(__IO_H__)` | `SYS0B_XVDT_TS_TCODE` | `0x312` | 系統暫存器/記憶體映射位址定義 |
| 848 | `!defined(__IO_H__)` | `SET_XVDT_TS_TCODE(val)` | `(SYS0B[SYS0B_XVDT_TS_TCODE ] \|= (val & 0x1F))` | 通用常數/硬體欄位定義 |
| 850 | `!defined(__IO_H__)` | `SYS0L_REG_CFG` | `(0x314>>2)` | 系統暫存器/記憶體映射位址定義 |
| 851 | `!defined(__IO_H__)` | `CR_REG_CFG_1` | `BIT9` | 系統暫存器/記憶體映射位址定義 |
| 852 | `!defined(__IO_H__)` | `CR_REG_CFG_0` | `BIT8` | 系統暫存器/記憶體映射位址定義 |
| 853 | `!defined(__IO_H__)` | `SYS0B_CR_PLL_TRIM` | `0x314` | 系統暫存器/記憶體映射位址定義 |
| 854 | `!defined(__IO_H__)` | `SET_CR_PLL_TRIM(val)` | `(SYS0B[SYS0B_CR_PLL_TRIM ] \|= ((val<<4) & 0x70))` | 通用常數/硬體欄位定義 |
| 855 | `!defined(__IO_H__)` | `SYS0B_CR_REG_TRIM` | `0x314` | 系統暫存器/記憶體映射位址定義 |
| 856 | `!defined(__IO_H__)` | `SET_CR_REG_TRIM(val)` | `(SYS0B[SYS0B_CR_REG_TRIM ] \|= (val & 0x07))` | 系統暫存器/記憶體映射位址定義 |
| 858 | `!defined(__IO_H__)` | `SYS0L_DDR_CTRL` | `(0x318>>2)` | 系統暫存器/記憶體映射位址定義 |
| 859 | `!defined(__IO_H__)` | `SYS0B_CR_DDR_TYPE` | `0x319` | 系統暫存器/記憶體映射位址定義 |
| 860 | `!defined(__IO_H__)` | `SET_CR_DDR_TYPE_MRAM` | `(SYS0B[SYS0B_CR_DDR_TYPE] = 0x00)` | 通用常數/硬體欄位定義 |
| 861 | `!defined(__IO_H__)` | `SET_CR_DDR_TYPE_DDR3` | `(SYS0B[SYS0B_CR_DDR_TYPE] = 0x40)` | 通用常數/硬體欄位定義 |
| 862 | `!defined(__IO_H__)` | `SET_CR_DDR_TYPE_SDR_1_8` | `(SYS0B[SYS0B_CR_DDR_TYPE] = 0x80)` | 通用常數/硬體欄位定義 |
| 863 | `!defined(__IO_H__)` | `SET_CR_DDR_TYPE_SDR_3_3` | `(SYS0B[SYS0B_CR_DDR_TYPE] = 0xC0)` | 通用常數/硬體欄位定義 |
| 864 | `!defined(__IO_H__)` | `CR_DDR_DPD` | `BIT7` | 通用常數/硬體欄位定義 |
| 866 | `!defined(__IO_H__)` | `SYS0L_PLL_CFG0` | `(0x31C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 867 | `!defined(__IO_H__)` | `SYS0B_CR_PLL0_BAND` | `0x31E` | 系統暫存器/記憶體映射位址定義 |
| 868 | `!defined(__IO_H__)` | `SET_CR_PLL0_BAND(val)` | `(SYS0B[SYS0B_CR_PLL0_BAND] \|= (val & 0x03))` | 通用常數/硬體欄位定義 |
| 870 | `!defined(__IO_H__)` | `SYS0L_PLL_CFG1` | `(0x320>>2)` | 系統暫存器/記憶體映射位址定義 |
| 871 | `!defined(__IO_H__)` | `SYS0B_CR_PLL1_BAND` | `0x323` | 系統暫存器/記憶體映射位址定義 |
| 872 | `!defined(__IO_H__)` | `SET_CR_PLL1_BAND(val)` | `(SYS0B[SYS0B_CR_PLL1_BAND] \|= (val & 0x03))` | 通用常數/硬體欄位定義 |
| 874 | `!defined(__IO_H__)` | `SYS0L_PLL0_CTRL` | `(0x320>>2)` | 系統暫存器/記憶體映射位址定義 |
| 875 | `!defined(__IO_H__)` | `DISABLE_PLL(sel)` | `(SYS0B[0x320 + sel << 3] &=  ~PLL_ENABLE)` | 通用常數/硬體欄位定義 |
| 876 | `!defined(__IO_H__)` | `ENABLE_PLL(sel)` | `(SYS0B[0x320 + sel << 3] \|=  PLL_ENABLE)` | 通用常數/硬體欄位定義 |
| 879 | `!defined(__IO_H__)` | `SYS0L_PLL_CFG2` | `(0x324>>2)` | 系統暫存器/記憶體映射位址定義 |
| 880 | `!defined(__IO_H__)` | `CR_PLL_EN_VCMID_0` | `BIT20` | 通用常數/硬體欄位定義 |
| 881 | `!defined(__IO_H__)` | `CR_PLL_CHPMP_DIS_0` | `BIT21` | 通用常數/硬體欄位定義 |
| 882 | `!defined(__IO_H__)` | `CR_PLL_RST_0` | `BIT22` | 通用常數/硬體欄位定義 |
| 883 | `!defined(__IO_H__)` | `CR_PLL_DIS_0` | `BIT23` | 通用常數/硬體欄位定義 |
| 884 | `!defined(__IO_H__)` | `CR_PLL_EN_VCMID_1` | `BIT28` | 通用常數/硬體欄位定義 |
| 885 | `!defined(__IO_H__)` | `CR_PLL_CHPMP_DIS_1` | `BIT29` | 通用常數/硬體欄位定義 |
| 886 | `!defined(__IO_H__)` | `CR_PLL_RST_1` | `BIT30` | 通用常數/硬體欄位定義 |
| 887 | `!defined(__IO_H__)` | `CR_PLL_DIS_1` | `BIT31` | 通用常數/硬體欄位定義 |
| 890 | `!defined(__IO_H__)` | `SYS0L_PLL0_DIV` | `(0x328>>2)` | 系統暫存器/記憶體映射位址定義 |
| 891 | `!defined(__IO_H__)` | `SYS0L_PLL1_DIV` | `(0x32c>>2)` | 系統暫存器/記憶體映射位址定義 |
| 893 | `!defined(__IO_H__)` | `SET_PLL_DIVN1(sel,val)` | `(SYS0B[0x324+(sel<<3)] \|= (val & 0x0F))` | 通用常數/硬體欄位定義 |
| 894 | `!defined(__IO_H__)` | `SET_PLL_DIVN2(sel,val)` | `(SYS0B[0x324+(sel<<3)] \|= ((val<<4) & 0xF0))` | 通用常數/硬體欄位定義 |
| 895 | `!defined(__IO_H__)` | `SET_PLL_DIVN3(sel,val)` | `(SYS0B[0x325+(sel<<3)] \|= (val & 0x0F))` | 通用常數/硬體欄位定義 |
| 896 | `!defined(__IO_H__)` | `SET_PLL_DIVNS(sel,val)` | `(SYS0B[0x326+(sel<<3)] \|= (val & 0x0F))` | 通用常數/硬體欄位定義 |
| 897 | `!defined(__IO_H__)` | `SET_PLL_DIVNM(sel,val)` | `(SYS0B[0x327+(sel<<3)] \|= (val & 0xFF))` | 通用常數/硬體欄位定義 |
| 900 | `!defined(__IO_H__)` | `SYS0L_PHY_MISC_0` | `(0x330>>2)` | 系統暫存器/記憶體映射位址定義 |
| 901 | `!defined(__IO_H__)` | `SYS0L_PHY_MISC_1` | `(0x334>>2)` | 系統暫存器/記憶體映射位址定義 |
| 902 | `!defined(__IO_H__)` | `SYS0L_PHY_MISC_2` | `(0x338>>2)` | 系統暫存器/記憶體映射位址定義 |
| 903 | `!defined(__IO_H__)` | `SYS0L_PHY_MISC_3` | `(0x340>>2)` | 系統暫存器/記憶體映射位址定義 |
| 904 | `!defined(__IO_H__)` | `SYS0L_PHY_MISC_4` | `(0x344>>2)` | 系統暫存器/記憶體映射位址定義 |
| 907 | `!defined(__IO_H__)` | `SYS0L_SATA_CTRL` | `(0x350>>2)` | 系統暫存器/記憶體映射位址定義 |
| 908 | `!defined(__IO_H__)` | `CR_SATA_GEN_SEL` | `BIT7` | 通用常數/硬體欄位定義 |
| 909 | `!defined(__IO_H__)` | `SYS0B_CR_SATA_GEN_F` | `0x350` | 系統暫存器/記憶體映射位址定義 |
| 910 | `!defined(__IO_H__)` | `SET_CR_SATA_GEN_F(sel,val)` | `(SYS0B[SYS0B_CR_SATA_GEN_F] \|= ((val<<4) & 0x20))` | 通用常數/硬體欄位定義 |
| 912 | `!defined(__IO_H__)` | `SYS0L_CR_PHY_TRIM` | `(0x358>>2)` | 系統暫存器/記憶體映射位址定義 |
| 913 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN1_0` | `(0x360>>2)` | 系統暫存器/記憶體映射位址定義 |
| 914 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN1_1` | `(0x364>>2)` | 系統暫存器/記憶體映射位址定義 |
| 915 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN1_2` | `(0x368>>2)` | 系統暫存器/記憶體映射位址定義 |
| 916 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN1_3` | `(0x36c>>2)` | 系統暫存器/記憶體映射位址定義 |
| 917 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN1_4` | `(0x370>>2)` | 系統暫存器/記憶體映射位址定義 |
| 918 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN1_5` | `(0x374>>2)` | 系統暫存器/記憶體映射位址定義 |
| 919 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN2_0` | `(0x380>>2)` | 系統暫存器/記憶體映射位址定義 |
| 920 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN2_1` | `(0x384>>2)` | 系統暫存器/記憶體映射位址定義 |
| 921 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN2_2` | `(0x388>>2)` | 系統暫存器/記憶體映射位址定義 |
| 922 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN2_3` | `(0x38c>>2)` | 系統暫存器/記憶體映射位址定義 |
| 923 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN2_4` | `(0x390>>2)` | 系統暫存器/記憶體映射位址定義 |
| 924 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN2_5` | `(0x394>>2)` | 系統暫存器/記憶體映射位址定義 |
| 925 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN3_0` | `(0x3A0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 926 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN3_1` | `(0x3A4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 927 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN3_2` | `(0x3A8>>2)` | 系統暫存器/記憶體映射位址定義 |
| 928 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN3_3` | `(0x3Ac>>2)` | 系統暫存器/記憶體映射位址定義 |
| 929 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN3_4` | `(0x3B0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 930 | `!defined(__IO_H__)` | `SYS0L_PHY_PMA_GEN3_5` | `(0x3B4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 931 | `!defined(__IO_H__)` | `SYS0L_PHY_PCS_SATA0` | `(0x3C0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 932 | `!defined(__IO_H__)` | `SYS0L_PHY_PCS_SATA1` | `(0x3C4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 933 | `!defined(__IO_H__)` | `SYS0L_PHY_PCS_SATA2` | `(0x3C8>>2)` | 系統暫存器/記憶體映射位址定義 |
| 934 | `!defined(__IO_H__)` | `SYS0L_PHY_PCS_SATA3` | `(0x3CC>>2)` | 系統暫存器/記憶體映射位址定義 |
| 937 | `!defined(__IO_H__)` | `CLK_SET_DIV(gen_sel,val)` | `(SYS0B[SYS0L_CLK_DIV_VAL0 + gen_sel] = (val &0xF))` | 依原始註解：////////////////////////////////  CLK；註解：////////////////////////////////  CLK |
| 939 | `!defined(__IO_H__)` | `SYS0L_CLK_CFG0` | `(0x400>>2)` | 系統暫存器/記憶體映射位址定義 |
| 940 | `!defined(__IO_H__)` | `CR_FLL_VAL` | `BIT24` | 通用常數/硬體欄位定義 |
| 941 | `!defined(__IO_H__)` | `SR_FLL_VAL` | `BIT16` | 通用常數/硬體欄位定義 |
| 942 | `!defined(__IO_H__)` | `PHY_REF_CKEN` | `BIT7` | 通用常數/硬體欄位定義 |
| 943 | `!defined(__IO_H__)` | `SR_PLL_LOCK_PLL1` | `BIT1` | 通用常數/硬體欄位定義 |
| 944 | `!defined(__IO_H__)` | `SR_PLL_LOCK_PLL0` | `BIT0` | 通用常數/硬體欄位定義 |
| 946 | `!defined(__IO_H__)` | `SYS0L_FLL_CNY_VAL` | `(0x408>>2)` | 系統暫存器/記憶體映射位址定義 |
| 947 | `!defined(__IO_H__)` | `CR_OP_FLL` | `BIT0` | 通用常數/硬體欄位定義 |
| 949 | `!defined(__IO_H__)` | `SYS0L_RXP_CLKSW_CTRL` | `(0x410>>2)` | 系統暫存器/記憶體映射位址定義 |
| 950 | `!defined(__IO_H__)` | `CR_SEL_9` | `BIT25` | 通用常數/硬體欄位定義 |
| 951 | `!defined(__IO_H__)` | `CR_SEL_8` | `BIT24` | 通用常數/硬體欄位定義 |
| 952 | `!defined(__IO_H__)` | `CR_SEL_7` | `BIT23` | 通用常數/硬體欄位定義 |
| 953 | `!defined(__IO_H__)` | `CR_SEL_6` | `BIT22` | 通用常數/硬體欄位定義 |
| 954 | `!defined(__IO_H__)` | `CR_SEL_5` | `BIT21` | 通用常數/硬體欄位定義 |
| 955 | `!defined(__IO_H__)` | `CR_SEL_4` | `BIT20` | 通用常數/硬體欄位定義 |
| 956 | `!defined(__IO_H__)` | `CR_SEL_3` | `BIT19` | 通用常數/硬體欄位定義 |
| 957 | `!defined(__IO_H__)` | `CR_SEL_2` | `BIT18` | 通用常數/硬體欄位定義 |
| 958 | `!defined(__IO_H__)` | `CR_SEL_1` | `BIT17` | 通用常數/硬體欄位定義 |
| 959 | `!defined(__IO_H__)` | `CR_SEL_0` | `BIT16` | 通用常數/硬體欄位定義 |
| 960 | `!defined(__IO_H__)` | `CR_REF_SEL` | `BIT7` | 通用常數/硬體欄位定義 |
| 962 | `!defined(__IO_H__)` | `SYS0L_CLK_EN_CTRL` | `(0x414>>2)` | 系統暫存器/記憶體映射位址定義 |
| 963 | `!defined(__IO_H__)` | `CR_FLH_CKEN` | `BIT24` | 通用常數/硬體欄位定義 |
| 964 | `!defined(__IO_H__)` | `CR_DDRP_CKEN` | `BIT23` | 通用常數/硬體欄位定義 |
| 965 | `!defined(__IO_H__)` | `CR_DDRC_CKEN` | `BIT22` | 通用常數/硬體欄位定義 |
| 966 | `!defined(__IO_H__)` | `CR_SPI_CKEN` | `BIT21` | 通用常數/硬體欄位定義 |
| 967 | `!defined(__IO_H__)` | `CR_SEC_CKEN` | `BIT20` | 通用常數/硬體欄位定義 |
| 968 | `!defined(__IO_H__)` | `CR_WDT_CKEN` | `BIT15` | 通用常數/硬體欄位定義 |
| 969 | `!defined(__IO_H__)` | `CR_UART_CKEN` | `BIT14` | 周邊控制暫存器/位元欄位定義 |
| 970 | `!defined(__IO_H__)` | `CR_I2CM_CKEN` | `BIT13` | 通用常數/硬體欄位定義 |
| 971 | `!defined(__IO_H__)` | `CR_CPU_CKEN` | `BIT6` | 通用常數/硬體欄位定義 |
| 973 | `!defined(__IO_H__)` | `SYS0L_XTAL_PD` | `(0x418>>2)` | 系統暫存器/記憶體映射位址定義 |
| 974 | `!defined(__IO_H__)` | `CR_XTAL_PD` | `BIT7` | 通用常數/硬體欄位定義 |
| 979 | `!defined(__IO_H__)` | `SYS0L_CLK_DIV_VAL0` | `(0x420>>2)` | 系統暫存器/記憶體映射位址定義 |
| 980 | `!defined(__IO_H__)` | `SYS0B_CR_DIV_0` | `0x420` | 系統暫存器/記憶體映射位址定義 |
| 981 | `!defined(__IO_H__)` | `CR_DIV_3` | `BIT24` | 通用常數/硬體欄位定義 |
| 982 | `!defined(__IO_H__)` | `CR_DIV_2` | `BIT16` | 通用常數/硬體欄位定義 |
| 983 | `!defined(__IO_H__)` | `CR_DIV_1` | `BIT8` | 通用常數/硬體欄位定義 |
| 984 | `!defined(__IO_H__)` | `CR_DIV_0` | `BIT0` | 通用常數/硬體欄位定義 |
| 986 | `!defined(__IO_H__)` | `SYS0L_CLK_DIV_VAL1` | `(0x424>>2)` | 系統暫存器/記憶體映射位址定義 |
| 987 | `!defined(__IO_H__)` | `SYS0B_CLK_DIV_7` | `0x427` | 系統暫存器/記憶體映射位址定義 |
| 988 | `!defined(__IO_H__)` | `SYS0B_CLK_DIV_6` | `0x426` | 系統暫存器/記憶體映射位址定義 |
| 989 | `!defined(__IO_H__)` | `SYS0B_CLK_DIV_5` | `0x425` | 系統暫存器/記憶體映射位址定義 |
| 990 | `!defined(__IO_H__)` | `SYS0B_CLK_DIV_4` | `0x424` | 系統暫存器/記憶體映射位址定義 |
| 993 | `!defined(__IO_H__)` | `SYS0L_CLK_DIV_VAL2` | `(0x428>>2)` | 系統暫存器/記憶體映射位址定義 |
| 994 | `!defined(__IO_H__)` | `SYS0B_CLK_DIV_9` | `0x429` | 系統暫存器/記憶體映射位址定義 |
| 995 | `!defined(__IO_H__)` | `SYS0B_CLK_DIV_8` | `0x428` | 系統暫存器/記憶體映射位址定義 |
| 998 | `!defined(__IO_H__)` | `SYS0L_CLKSW_CTRL0` | `(0x430>>2)` | 系統暫存器/記憶體映射位址定義 |
| 999 | `!defined(__IO_H__)` | `CR_SEL_3_3` | `BIT31` | 通用常數/硬體欄位定義 |
| 1000 | `!defined(__IO_H__)` | `CR_SEL_3_2` | `BIT30` | 通用常數/硬體欄位定義 |
| 1001 | `!defined(__IO_H__)` | `CR_SEL_3_1` | `BIT29` | 通用常數/硬體欄位定義 |
| 1002 | `!defined(__IO_H__)` | `CR_SEL_3_0` | `BIT28` | 通用常數/硬體欄位定義 |
| 1004 | `!defined(__IO_H__)` | `CR_SEL_2_3` | `BIT23` | 通用常數/硬體欄位定義 |
| 1005 | `!defined(__IO_H__)` | `CR_SEL_2_2` | `BIT22` | 通用常數/硬體欄位定義 |
| 1006 | `!defined(__IO_H__)` | `CR_SEL_2_1` | `BIT21` | 通用常數/硬體欄位定義 |
| 1007 | `!defined(__IO_H__)` | `CR_SEL_2_0` | `BIT20` | 通用常數/硬體欄位定義 |
| 1009 | `!defined(__IO_H__)` | `CR_SEL_1_3` | `BIT15` | 通用常數/硬體欄位定義 |
| 1010 | `!defined(__IO_H__)` | `CR_SEL_1_2` | `BIT14` | 通用常數/硬體欄位定義 |
| 1011 | `!defined(__IO_H__)` | `CR_SEL_1_1` | `BIT13` | 通用常數/硬體欄位定義 |
| 1012 | `!defined(__IO_H__)` | `CR_SEL_1_0` | `BIT12` | 通用常數/硬體欄位定義 |
| 1014 | `!defined(__IO_H__)` | `CR_SEL_0_3` | `BIT7` | 通用常數/硬體欄位定義 |
| 1015 | `!defined(__IO_H__)` | `CR_SEL_0_2` | `BIT6` | 通用常數/硬體欄位定義 |
| 1016 | `!defined(__IO_H__)` | `CR_SEL_0_1` | `BIT5` | 通用常數/硬體欄位定義 |
| 1017 | `!defined(__IO_H__)` | `CR_SEL_0_0` | `BIT4` | 通用常數/硬體欄位定義 |
| 1020 | `!defined(__IO_H__)` | `SYS0L_CLKSW_CTRL1` | `(0x434>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1021 | `!defined(__IO_H__)` | `CR_SEL_7_3` | `BIT31` | 通用常數/硬體欄位定義 |
| 1022 | `!defined(__IO_H__)` | `CR_SEL_7_2` | `BIT30` | 通用常數/硬體欄位定義 |
| 1023 | `!defined(__IO_H__)` | `CR_SEL_7_1` | `BIT29` | 通用常數/硬體欄位定義 |
| 1024 | `!defined(__IO_H__)` | `CR_SEL_7_0` | `BIT28` | 通用常數/硬體欄位定義 |
| 1026 | `!defined(__IO_H__)` | `CR_SEL_6_3` | `BIT23` | 通用常數/硬體欄位定義 |
| 1027 | `!defined(__IO_H__)` | `CR_SEL_6_2` | `BIT22` | 通用常數/硬體欄位定義 |
| 1028 | `!defined(__IO_H__)` | `CR_SEL_6_1` | `BIT21` | 通用常數/硬體欄位定義 |
| 1029 | `!defined(__IO_H__)` | `CR_SEL_6_0` | `BIT20` | 通用常數/硬體欄位定義 |
| 1031 | `!defined(__IO_H__)` | `CR_SEL_5_3` | `BIT15` | 通用常數/硬體欄位定義 |
| 1032 | `!defined(__IO_H__)` | `CR_SEL_5_2` | `BIT14` | 通用常數/硬體欄位定義 |
| 1033 | `!defined(__IO_H__)` | `CR_SEL_5_1` | `BIT13` | 通用常數/硬體欄位定義 |
| 1034 | `!defined(__IO_H__)` | `CR_SEL_5_0` | `BIT12` | 通用常數/硬體欄位定義 |
| 1036 | `!defined(__IO_H__)` | `CR_SEL_4_3` | `BIT7` | 通用常數/硬體欄位定義 |
| 1037 | `!defined(__IO_H__)` | `CR_SEL_4_2` | `BIT6` | 通用常數/硬體欄位定義 |
| 1038 | `!defined(__IO_H__)` | `CR_SEL_4_1` | `BIT5` | 通用常數/硬體欄位定義 |
| 1039 | `!defined(__IO_H__)` | `CR_SEL_4_0` | `BIT4` | 通用常數/硬體欄位定義 |
| 1041 | `!defined(__IO_H__)` | `SYS0B_CLKSW_SEL7` | `(0x437)` | 系統暫存器/記憶體映射位址定義 |
| 1043 | `!defined(__IO_H__)` | `SYS0L_CLKSW_CTRL2` | `(0x438>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1044 | `!defined(__IO_H__)` | `CR_SEL_9_3` | `BIT15` | 通用常數/硬體欄位定義 |
| 1045 | `!defined(__IO_H__)` | `CR_SEL_9_2` | `BIT14` | 通用常數/硬體欄位定義 |
| 1046 | `!defined(__IO_H__)` | `CR_SEL_9_1` | `BIT13` | 通用常數/硬體欄位定義 |
| 1047 | `!defined(__IO_H__)` | `CR_SEL_9_0` | `BIT12` | 通用常數/硬體欄位定義 |
| 1049 | `!defined(__IO_H__)` | `CR_SEL_8_3` | `BIT7` | 通用常數/硬體欄位定義 |
| 1050 | `!defined(__IO_H__)` | `CR_SEL_8_2` | `BIT6` | 通用常數/硬體欄位定義 |
| 1051 | `!defined(__IO_H__)` | `CR_SEL_8_1` | `BIT5` | 通用常數/硬體欄位定義 |
| 1052 | `!defined(__IO_H__)` | `CR_SEL_8_0` | `BIT4` | 通用常數/硬體欄位定義 |
| 1057 | `!defined(__IO_H__)` | `SYS0L_PD_CTRL` | `(0x500>>2)` | 系統暫存器/記憶體映射位址定義；註解：/////////////////Power Management Register |
| 1058 | `!defined(__IO_H__)` | `CR_PMU_WU_F` | `BIT31` | 通用常數/硬體欄位定義 |
| 1059 | `!defined(__IO_H__)` | `CR_PMU_PD_F` | `BIT15` | 通用常數/硬體欄位定義 |
| 1060 | `!defined(__IO_H__)` | `CR_PMU_VDT_PD` | `BIT3` | 通用常數/硬體欄位定義 |
| 1061 | `!defined(__IO_H__)` | `CR_PMU_REF_GCK` | `BIT2` | 通用常數/硬體欄位定義 |
| 1062 | `!defined(__IO_H__)` | `SYS0B_CR_PMU_I2C_WU` | `0x502` | 系統暫存器/記憶體映射位址定義 |
| 1063 | `!defined(__IO_H__)` | `SET_CR_PMU_I2C_WU(val)` | `(SYS0B[SYS0B_CR_PMU_I2C_WU] \|= ((val << 4) & 0xF0))` | 通用常數/硬體欄位定義 |
| 1064 | `!defined(__IO_H__)` | `SYS0B_CR_PMU_I2C_PD` | `0x500` | 系統暫存器/記憶體映射位址定義 |
| 1065 | `!defined(__IO_H__)` | `SET_CR_PMU_I2C_PD(val)` | `(SYS0B[SYS0B_CR_PMU_I2C_PD] \|= ((val << 4) & 0xF0))` | 通用常數/硬體欄位定義 |
| 1067 | `!defined(__IO_H__)` | `SYS0L_WAKEUP_FLAG` | `(0x504>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1068 | `!defined(__IO_H__)` | `CR_FW_T_F` | `BIT31` | 通用常數/硬體欄位定義 |
| 1069 | `!defined(__IO_H__)` | `WAKEUP_F` | `BIT0` | 通用常數/硬體欄位定義 |
| 1071 | `!defined(__IO_H__)` | `SYS0L_COP_CMD_ADR0` | `(0x508>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1072 | `!defined(__IO_H__)` | `SYS0W_CR_ADR_FW` | `(0x508>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1073 | `!defined(__IO_H__)` | `SYS0W_CR_ADR_CKG` | `(0x50A>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1075 | `!defined(__IO_H__)` | `SYS0L_COP_CMD_ADR1` | `(0x50C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1076 | `!defined(__IO_H__)` | `SYS0W_CR_ADR_SD` | `(0x50C>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1077 | `!defined(__IO_H__)` | `SYS0W_CR_ADR_WU` | `(0x50E>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1079 | `!defined(__IO_H__)` | `SYS0L_RST_CTRL` | `(0x514>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1080 | `!defined(__IO_H__)` | `CR_RST_N_FLH` | `BIT24` | 通用常數/硬體欄位定義 |
| 1081 | `!defined(__IO_H__)` | `CR_RST_N_DDRP` | `BIT23` | 通用常數/硬體欄位定義 |
| 1082 | `!defined(__IO_H__)` | `CR_RST_N_DDRC` | `BIT22` | 通用常數/硬體欄位定義 |
| 1083 | `!defined(__IO_H__)` | `CR_RST_N_SPI` | `BIT21` | 通用常數/硬體欄位定義 |
| 1084 | `!defined(__IO_H__)` | `CR_RST_N_SEC` | `BIT20` | 通用常數/硬體欄位定義 |
| 1085 | `!defined(__IO_H__)` | `CR_RST_N_DMAC` | `BIT19` | 周邊控制暫存器/位元欄位定義 |
| 1086 | `!defined(__IO_H__)` | `CR_RST_N_SS` | `BIT18` | 通用常數/硬體欄位定義 |
| 1087 | `!defined(__IO_H__)` | `CR_RST_N_DBUF` | `BIT16` | 通用常數/硬體欄位定義 |
| 1088 | `!defined(__IO_H__)` | `CR_RST_N_WDT` | `BIT15` | 通用常數/硬體欄位定義 |
| 1089 | `!defined(__IO_H__)` | `CR_RST_N_UART` | `BIT14` | 周邊控制暫存器/位元欄位定義 |
| 1090 | `!defined(__IO_H__)` | `CR_RST_N_I2CM` | `BIT13` | 通用常數/硬體欄位定義 |
| 1091 | `!defined(__IO_H__)` | `CR_RST_N_SATA` | `BIT12` | 通用常數/硬體欄位定義 |
| 1092 | `!defined(__IO_H__)` | `CR_RST_WHOLE_CHIP` | `BIT7` | 通用常數/硬體欄位定義 |
| 1093 | `!defined(__IO_H__)` | `CR_RST_CPU_F` | `BIT6` | 通用常數/硬體欄位定義 |
| 1095 | `!defined(__IO_H__)` | `SYS0L_MODE_EN` | `(0x518>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1096 | `!defined(__IO_H__)` | `CR_CPU_STALL` | `BIT24` | 通用常數/硬體欄位定義 |
| 1097 | `!defined(__IO_H__)` | `MD_ICE_EN` | `BIT8` | 通用常數/硬體欄位定義 |
| 1098 | `!defined(__IO_H__)` | `MD_EA1_EN` | `BIT6` | 通用常數/硬體欄位定義 |
| 1099 | `!defined(__IO_H__)` | `MD_EA2_EN` | `BIT5` | 通用常數/硬體欄位定義 |
| 1100 | `!defined(__IO_H__)` | `MD_EA3_EN` | `BIT4` | 通用常數/硬體欄位定義 |
| 1101 | `!defined(__IO_H__)` | `MD_EA4_EN` | `BIT3` | 通用常數/硬體欄位定義 |
| 1102 | `!defined(__IO_H__)` | `MD_PRAM_EN` | `BIT0` | 通用常數/硬體欄位定義 |
| 1104 | `!defined(__IO_H__)` | `SYS0L_WU_SEL` | `(0x51C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1105 | `!defined(__IO_H__)` | `CR_GPIO_DET_EN` | `BIT9` | 周邊控制暫存器/位元欄位定義 |
| 1106 | `!defined(__IO_H__)` | `CR_DEVSLP_DET_EN` | `BIT8` | 通用常數/硬體欄位定義 |
| 1107 | `!defined(__IO_H__)` | `SYS0B_CR_GPIO_DBC_SEL` | `0x51C` | 系統暫存器/記憶體映射位址定義 |
| 1108 | `!defined(__IO_H__)` | `SET_CR_GPIO_DBC_SEL(val)` | `(SYS0B[SYS0B_CR_GPIO_DBC_SEL] \|= ((val<<4) & 0x30))` | 周邊控制暫存器/位元欄位定義 |
| 1109 | `!defined(__IO_H__)` | `SYS0B_CR_GPIO_DET_SEL` | `0x51C` | 系統暫存器/記憶體映射位址定義 |
| 1110 | `!defined(__IO_H__)` | `SET_CR_GPIO_DET_SEL(val)` | `(SYS0B[SYS0B_CR_GPIO_DET_SEL] \|= (val & 0x0F))` | 周邊控制暫存器/位元欄位定義 |
| 1111 | `!defined(__IO_H__)` | `SYS0W_CR_WAKE_EN` | `(0x51E>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1112 | `!defined(__IO_H__)` | `SET_CR_WAKE_EN(val)` | `(SYS0W[SYS0W_CR_WAKE_EN] \|= (val & 0x0FFF))` | 通用常數/硬體欄位定義 |
| 1115 | `!defined(__IO_H__)` | `SYS0L_PG_CTRL` | `(0x520>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1116 | `!defined(__IO_H__)` | `PMU_SATA_MSK_REQ` | `BIT31` | 通用常數/硬體欄位定義 |
| 1117 | `!defined(__IO_H__)` | `SR_SATA_STS` | `BIT28` | 通用常數/硬體欄位定義 |
| 1118 | `!defined(__IO_H__)` | `PMU_SATA_MSK` | `BIT24` | 通用常數/硬體欄位定義 |
| 1119 | `!defined(__IO_H__)` | `CR_IO0_PG` | `BIT15` | 通用常數/硬體欄位定義 |
| 1120 | `!defined(__IO_H__)` | `CR_PD1_PG` | `BIT14` | 通用常數/硬體欄位定義 |
| 1121 | `!defined(__IO_H__)` | `CR_PD2_PG` | `BIT13` | 通用常數/硬體欄位定義 |
| 1122 | `!defined(__IO_H__)` | `CR_FIO_PG_PD0` | `BIT7` | 通用常數/硬體欄位定義 |
| 1123 | `!defined(__IO_H__)` | `CR_FLH_PG_PD0` | `BIT6` | 通用常數/硬體欄位定義 |
| 1124 | `!defined(__IO_H__)` | `CR_FIO_PG_PD1` | `BIT5` | 通用常數/硬體欄位定義 |
| 1125 | `!defined(__IO_H__)` | `CR_FLH_PG_PD1` | `BIT4` | 通用常數/硬體欄位定義 |
| 1126 | `!defined(__IO_H__)` | `CR_FIO_PG_PD2` | `BIT3` | 通用常數/硬體欄位定義 |
| 1127 | `!defined(__IO_H__)` | `CR_FLH_PG_PD2` | `BIT2` | 通用常數/硬體欄位定義 |
| 1129 | `!defined(__IO_H__)` | `SYS0L_PMU_CTRL0` | `(0x524>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1130 | `!defined(__IO_H__)` | `SPI_BZ_F` | `BIT31` | 通用常數/硬體欄位定義 |
| 1131 | `!defined(__IO_H__)` | `DMAC_BZ_F` | `BIT23` | 周邊控制暫存器/位元欄位定義 |
| 1132 | `!defined(__IO_H__)` | `SR_DMAC_STS` | `BIT20` | 周邊控制暫存器/位元欄位定義 |
| 1133 | `!defined(__IO_H__)` | `PMU_DMAC_CMD` | `BIT16` | 周邊控制暫存器/位元欄位定義 |
| 1134 | `!defined(__IO_H__)` | `I2CM_BZ_F` | `BIT15` | 通用常數/硬體欄位定義 |
| 1135 | `!defined(__IO_H__)` | `PMU_I2CM_CMD` | `BIT8` | 通用常數/硬體欄位定義 |
| 1136 | `!defined(__IO_H__)` | `SYS0B_PMU_FLL_CMD` | `0x524` | 系統暫存器/記憶體映射位址定義 |
| 1137 | `!defined(__IO_H__)` | `SET_PMU_FLL_CMD(val)` | `(SYS0B[SYS0B_PMU_FLL_CMD] \|= (val & 0x03))` | 通用常數/硬體欄位定義 |
| 1141 | `!defined(__IO_H__)` | `SYS0L_MEMPD_CTRL` | `(0x52C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1142 | `!defined(__IO_H__)` | `CR_OP_PD2_REG_PD` | `BIT31` | 系統暫存器/記憶體映射位址定義 |
| 1143 | `!defined(__IO_H__)` | `CR_OP_VDT_WU` | `BIT30` | 通用常數/硬體欄位定義 |
| 1144 | `!defined(__IO_H__)` | `CR_OP_PD0RAM_LS` | `BIT23` | 通用常數/硬體欄位定義 |
| 1145 | `!defined(__IO_H__)` | `CR_OP_PD0RAM_DS` | `BIT22` | 通用常數/硬體欄位定義 |
| 1146 | `!defined(__IO_H__)` | `CR_OP_PD1_RAM_PD` | `BIT21` | 通用常數/硬體欄位定義 |
| 1147 | `!defined(__IO_H__)` | `CR_OP_PD2_RAM_PD` | `BIT20` | 通用常數/硬體欄位定義 |
| 1148 | `!defined(__IO_H__)` | `CR_OP_PD1_RAM_DS` | `BIT19` | 通用常數/硬體欄位定義 |
| 1149 | `!defined(__IO_H__)` | `CR_OP_PD2_RAM_DS` | `BIT18` | 通用常數/硬體欄位定義 |
| 1150 | `!defined(__IO_H__)` | `CR_OP_PD2_PD` | `BIT9` | 通用常數/硬體欄位定義 |
| 1151 | `!defined(__IO_H__)` | `CR_OP_PD1_PD` | `BIT8` | 通用常數/硬體欄位定義 |
| 1153 | `!defined(__IO_H__)` | `bRRAM(index)` | `* ( (volatile unsigned char* ) 0x04000600+(  index              ))` | 依原始註解：//////////////////RRAM；註解：//////////////////RRAM |
| 1154 | `!defined(__IO_H__)` | `wRRAM(index)` | `* ( (volatile unsigned short*) 0x04000600+(  index>>1           ))` | 通用常數/硬體欄位定義 |
| 1155 | `!defined(__IO_H__)` | `dRRAM(index)` | `* ( (volatile unsigned int*) 0x04000600+(  index>>2           ))` | 通用常數/硬體欄位定義 |
| 1156 | `!defined(__IO_H__)` | `qRRAM(index)` | `* ( (volatile unsigned long long int*) 0x04000600+(  index>>3           ))` | 通用常數/硬體欄位定義 |
| 1161 | `!defined(__IO_H__)` | `bRTT48(index)` | `* ( (volatile unsigned char* ) 0x04000700+(  index              ))` | 依原始註解：///////////////RTT48；註解：///////////////RTT48 |
| 1162 | `!defined(__IO_H__)` | `wRTT48(index)` | `* ( (volatile unsigned short*) 0x04000700+(  index>>1           ))` | 通用常數/硬體欄位定義 |
| 1163 | `!defined(__IO_H__)` | `dRTT48(index)` | `* ( (volatile unsigned int*) 0x04000700+(  index>>2           ))` | 通用常數/硬體欄位定義 |
| 1164 | `!defined(__IO_H__)` | `qRTT48(index)` | `* ( (volatile unsigned long long int*) 0x04000700+(  index>>3           ))` | 通用常數/硬體欄位定義 |
| 1167 | `!defined(__IO_H__)` | `SYS0L_RTT_48BIT_VLD` | `(0x700>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1168 | `!defined(__IO_H__)` | `SYS0L_RTT_48BIT_OFS` | `(0x704>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1169 | `!defined(__IO_H__)` | `SYS0L_RTT_48BIT_LMTL` | `(0x710>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1170 | `!defined(__IO_H__)` | `SYS0L_RTT_48BIT_LMTH` | `(0x714>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1171 | `!defined(__IO_H__)` | `SYS0L_RTT_48BIT_CNTL` | `(0x720>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1172 | `!defined(__IO_H__)` | `SYS0L_RTT_48BIT_CNTH` | `(0x724>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1174 | `!defined(__IO_H__)` | `RTT_Timeout_48Bit_enable` | `(SYS0L[SYS0L_RTT_48BIT_VLD]\|=BIT7)` | 通用常數/硬體欄位定義 |
| 1175 | `!defined(__IO_H__)` | `RTT_Timeout_48Bit_disable` | `(SYS0L[SYS0L_RTT_48BIT_VLD]&=~BIT7)` | 通用常數/硬體欄位定義 |
| 1176 | `!defined(__IO_H__)` | `RTT_Set_Offset_48Bit(val)` | `(SYS0L[SYS0L_RTT_48BIT_OFS] = (unsigned long)(val) )` | 通用常數/硬體欄位定義 |
| 1177 | `!defined(__IO_H__)` | `RTT_Get_Offset_48Bit` | `(SYS0L[SYS0L_RTT_48BIT_OFS])` | 通用常數/硬體欄位定義 |
| 1178 | `!defined(__IO_H__)` | `RTT_Set_Limit_48Bit_Low(val)` | `(SYS0L[SYS0L_RTT_48BIT_LMTL] = (unsigned long)(val) )` | 通用常數/硬體欄位定義 |
| 1179 | `!defined(__IO_H__)` | `RTT_Set_Limit_48Bit_High(val)` | `(SYS0W[SYS0L_RTT_48BIT_LMTH] = (unsigned short)(val) )` | 通用常數/硬體欄位定義 |
| 1180 | `!defined(__IO_H__)` | `RTT_Get_Limit_48Bit_Low` | `(SYS0L[SYS0L_RTT_48BIT_LMTL])` | 通用常數/硬體欄位定義 |
| 1181 | `!defined(__IO_H__)` | `RTT_Get_Limit_48Bit_High` | `(SYS0L[SYS0L_RTT_48BIT_LMTH])` | 通用常數/硬體欄位定義 |
| 1182 | `!defined(__IO_H__)` | `RTT_Get_Value_48Bit_Low` | `(SYS0L[SYS0L_RTT_48BIT_CNTL] )` | 通用常數/硬體欄位定義 |
| 1183 | `!defined(__IO_H__)` | `RTT_Get_Value_48Bit_High` | `(SYS0L[SYS0L_RTT_48BIT_CNTH] )` | 通用常數/硬體欄位定義 |
| 1187 | `!defined(__IO_H__)` | `SYS0L_LZSS_CTRL` | `(0x1020>>2)` | 系統暫存器/記憶體映射位址定義；註解：////////////// MISCH |
| 1188 | `!defined(__IO_H__)` | `CR_LZSS_BE_EN` | `BIT2` | 通用常數/硬體欄位定義 |
| 1189 | `!defined(__IO_H__)` | `SYS0B_CR_LZSS_CFG` | `0x1020` | 系統暫存器/記憶體映射位址定義 |
| 1190 | `!defined(__IO_H__)` | `SET_CR_LZSS_CFG(val)` | `(SYS0B[SYS0B_CR_LZSS_CFG] \|= (val & 0x03))` | 通用常數/硬體欄位定義 |
| 1192 | `!defined(__IO_H__)` | `SYS0L_RAM_RME` | `(0x1040>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1193 | `!defined(__IO_H__)` | `CR_PD0_RAM_RME` | `BIT0` | 通用常數/硬體欄位定義 |
| 1195 | `!defined(__IO_H__)` | `SYS0L_RAM_RM` | `(0x1044>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1196 | `!defined(__IO_H__)` | `SYS0B_CR_PD0_RAM_RM` | `(0x1044)` | 系統暫存器/記憶體映射位址定義 |
| 1197 | `!defined(__IO_H__)` | `SET_CR_LZSS_CFG(val)` | `(SYS0B[SYS0B_CR_LZSS_CFG] \|= (val & 0x03))` | 通用常數/硬體欄位定義 |
| 1205 | `!defined(__IO_H__)` | `SYS1B` | `((volatile unsigned char *)0x04002000)` | 系統暫存器/記憶體映射位址定義；註解：=================================================== \| PS3111 SYSTEM 1 Register Define \| =================================================== |
| 1206 | `!defined(__IO_H__)` | `SYS1W` | `((volatile unsigned short int *)0x04002000)` | 系統暫存器/記憶體映射位址定義 |
| 1207 | `!defined(__IO_H__)` | `SYS1L` | `((volatile unsigned int *)0x04002000)` | 系統暫存器/記憶體映射位址定義 |
| 1208 | `!defined(__IO_H__)` | `SYS1O` | `((volatile unsigned long long *)0x04002000)` | 系統暫存器/記憶體映射位址定義 |
| 1210 | `!defined(__IO_H__)` | `SYS1W_CR_GPIO_LH` | `(0x04>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1211 | `!defined(__IO_H__)` | `GET_CR_GPIO_LH` | `(SYS1W[SYS1W_CR_GPIO_LH])` | 周邊控制暫存器/位元欄位定義 |
| 1213 | `!defined(__IO_H__)` | `SYS1L_PRAM_EN` | `(0x10>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1214 | `!defined(__IO_H__)` | `FLH_IRAM_REPAIR_EN` | `BIT0` | 通用常數/硬體欄位定義 |
| 1215 | `!defined(__IO_H__)` | `FLH_IBF1_REPAIR_EN` | `BIT1` | 通用常數/硬體欄位定義 |
| 1216 | `!defined(__IO_H__)` | `FLH_IBF2_REPAIR_EN` | `BIT2` | 通用常數/硬體欄位定義 |
| 1217 | `!defined(__IO_H__)` | `FLH_IBF3_REPAIR_EN` | `BIT3` | 通用常數/硬體欄位定義 |
| 1218 | `!defined(__IO_H__)` | `FLH_IBF4_REPAIR_EN` | `BIT4` | 通用常數/硬體欄位定義 |
| 1219 | `!defined(__IO_H__)` | `FLH_IBF5_REPAIR_EN` | `BIT5` | 通用常數/硬體欄位定義 |
| 1220 | `!defined(__IO_H__)` | `FLH_IBF6_REPAIR_EN` | `BIT6` | 通用常數/硬體欄位定義 |
| 1221 | `!defined(__IO_H__)` | `FLH_IBF7_REPAIR_EN` | `BIT7` | 通用常數/硬體欄位定義 |
| 1222 | `!defined(__IO_H__)` | `FLH_IBF8_REPAIR_EN` | `BIT8` | 通用常數/硬體欄位定義 |
| 1223 | `!defined(__IO_H__)` | `FLH_IBF9_REPAIR_EN` | `BIT9` | 通用常數/硬體欄位定義 |
| 1224 | `!defined(__IO_H__)` | `FLH_IBF10_REPAIR_EN` | `BIT10` | 通用常數/硬體欄位定義 |
| 1225 | `!defined(__IO_H__)` | `FLH_RS_RAM_PBUF0_REPAIR_EN` | `BIT11` | 通用常數/硬體欄位定義 |
| 1226 | `!defined(__IO_H__)` | `FLH_RS_RAM_PBUF1_REPAIR_EN` | `BIT12` | 通用常數/硬體欄位定義 |
| 1227 | `!defined(__IO_H__)` | `FLH_RS_RAM_PBUF2_REPAIR_EN` | `BIT13` | 通用常數/硬體欄位定義 |
| 1228 | `!defined(__IO_H__)` | `FLH_RS_RAM_PBUF3_REPAIR_EN` | `BIT14` | 通用常數/硬體欄位定義 |
| 1229 | `!defined(__IO_H__)` | `DBUF0_REPAIR_EN` | `BIT15` | 通用常數/硬體欄位定義 |
| 1230 | `!defined(__IO_H__)` | `DBUF1_REPAIR_EN` | `BIT16` | 通用常數/硬體欄位定義 |
| 1231 | `!defined(__IO_H__)` | `DBUF2_REPAIR_EN` | `BIT17` | 通用常數/硬體欄位定義 |
| 1232 | `!defined(__IO_H__)` | `DBUF3_REPAIR_EN` | `BIT18` | 通用常數/硬體欄位定義 |
| 1234 | `!defined(__IO_H__)` | `SYS1L_RPRAM_CFG0` | `(0x14>>2)` | 系統暫存器/記憶體映射位址定義；註解：BIT[0:7] -	FAIL_BITSEQ of Flash IRAM |
| 1236 | `!defined(__IO_H__)` | `SYS1L_RPRAM_CFG1` | `(0x18>>2)` | 系統暫存器/記憶體映射位址定義；註解：[31:8]: -	FAIL_BITSEQ of Flash IBF \| -	FAIL_BITSEQ of Flash IBF |
| 1237 | `!defined(__IO_H__)` | `SYS1L_RPRAM_CFG2` | `(0x1C>>2)` | 系統暫存器/記憶體映射位址定義；註解：BIT[12:0]:-	FAIL_BITSEQ of Flash IBF |
| 1239 | `!defined(__IO_H__)` | `SYS1L_RPRAM_CFG3` | `(0x20>>2)` | 系統暫存器/記憶體映射位址定義；註解：[31:13]:-	FAIL_BITSEQ of Flash RS RAM PBUF \| BIT[12:0]-	FAIL_BITSEQ of Flash RS RAM PBUF |
| 1241 | `!defined(__IO_H__)` | `SYS1L_RPRAM_CFG4` | `(0x24>>2)` | 系統暫存器/記憶體映射位址定義；註解：[31:13]-	FAIL_BITSEQ of DBUF \| BIT[4:0]:-	FAIL_BITSEQ of DBUF |
| 1244 | `!defined(__IO_H__)` | `SIM_STS0` | `0x40` | 通用常數/硬體欄位定義 |
| 1245 | `!defined(__IO_H__)` | `SIM_STS2` | `0x48` | 通用常數/硬體欄位定義 |
| 1246 | `!defined(__IO_H__)` | `SET_CR_SIM_STS(sel,val)` | `(SYS1B[SIM_STS0+(sel)) = (val & 0xFF])` | 依原始註解：sel : 0~7；註解：sel : 0~7 |
| 1247 | `!defined(__IO_H__)` | `GET_CR_SIM_STS(sel)` | `(SYS1B[SIM_STS0+(sel)])` | 通用常數/硬體欄位定義 |
| 1248 | `!defined(__IO_H__)` | `GET_CR_SIM_STS_INV(sel)` | `(SYS1B[SIM_STS2+(7-sel)])` | 依原始註解：sel : 0~7；註解：sel : 0~7 |
| 1252 | `!defined(__IO_H__)` | `SYS1L_PAD_XFDAT_I` | `(0x100>>2)` | 系統暫存器/記憶體映射位址定義；註解：/////////////PADC |
| 1253 | `!defined(__IO_H__)` | `SYS1W_XFDAT_I` | `(0x100>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1255 | `!defined(__IO_H__)` | `SYS1L_PAD_XFCEB_I` | `(0x108>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1256 | `!defined(__IO_H__)` | `SYS1W_XFCEB_I` | `(0x108>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1258 | `!defined(__IO_H__)` | `SYS1L_PAD_FCEB_OE` | `(0x10c>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1259 | `!defined(__IO_H__)` | `SYS1W_PAD_FCEB_OE` | `(0x10c>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1261 | `!defined(__IO_H__)` | `SYS1L_PAD_FLH_CTRL` | `(0x110>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1262 | `!defined(__IO_H__)` | `CR_VREF_REN` | `BIT7` | 依原始註解：1:enable；註解：1:enable |
| 1263 | `!defined(__IO_H__)` | `CR_FLH_EV` | `BIT3` | 依原始註解：1:1.8V  0:3.3V；註解：1:1.8V  0:3.3V |
| 1264 | `!defined(__IO_H__)` | `CR_FLH_GB_PHY1` | `BIT2` | 依原始註解：1:active；註解：1:active |
| 1265 | `!defined(__IO_H__)` | `CR_FLH_GB_PHY0` | `BIT1` | 通用常數/硬體欄位定義 |
| 1266 | `!defined(__IO_H__)` | `CR_FLH_RSEL` | `BIT0` | 依原始註解：1:1K  0:75K；註解：1:1K  0:75K |
| 1268 | `!defined(__IO_H__)` | `SYS1L_PAD_FRDY_CFG` | `(0x114>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1269 | `!defined(__IO_H__)` | `XFRDY_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1270 | `!defined(__IO_H__)` | `CR_FRDY_SR` | `BIT23` | 通用常數/硬體欄位定義 |
| 1271 | `!defined(__IO_H__)` | `CR_FRDY_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1272 | `!defined(__IO_H__)` | `SYS1B_CR_FRDY_DV` | `0x115` | 系統暫存器/記憶體映射位址定義 |
| 1273 | `!defined(__IO_H__)` | `SET_CR_FRDY_DV(val)` | `(SYS1B[SYS1B_CR_FRDY_DV] \|= ((val <<4)  &  0xC0 ))` | 通用常數/硬體欄位定義 |
| 1274 | `!defined(__IO_H__)` | `SYS1B_CR_FRDY_PU` | `0x114` | 系統暫存器/記憶體映射位址定義 |
| 1275 | `!defined(__IO_H__)` | `SET_CR_FRDY_PU(val)` | `(SYS1B[SYS1B_CR_FRDY_PU] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1278 | `!defined(__IO_H__)` | `SYS1L_PAD_FWPB_CFG` | `(0x118>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1279 | `!defined(__IO_H__)` | `XFWPB_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1280 | `!defined(__IO_H__)` | `CR_FWPB_SR` | `BIT23` | 通用常數/硬體欄位定義 |
| 1281 | `!defined(__IO_H__)` | `CR_FWPB_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1282 | `!defined(__IO_H__)` | `CR_FWPB_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1283 | `!defined(__IO_H__)` | `CR_FWPB_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1284 | `!defined(__IO_H__)` | `SYS1B_CR_FWPB_DV` | `0x119` | 系統暫存器/記憶體映射位址定義 |
| 1285 | `!defined(__IO_H__)` | `SET_CR_FWPB_DV(val)` | `(SYS1B[SYS1B_CR_FWPB_DV] \|= ((val <<4)  &  0xC0 ))` | 通用常數/硬體欄位定義 |
| 1286 | `!defined(__IO_H__)` | `SYS1B_CR_FWPB_PU` | `0x114` | 系統暫存器/記憶體映射位址定義 |
| 1287 | `!defined(__IO_H__)` | `SET_CR_FWPB_PU(val)` | `(SYS1B[SYS1B_CR_FWPB_PU] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1290 | `!defined(__IO_H__)` | `SYS1L_PAD_FCEB_CFG` | `(0x11C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1291 | `!defined(__IO_H__)` | `CR_FCEB_SR` | `BIT23` | 通用常數/硬體欄位定義 |
| 1292 | `!defined(__IO_H__)` | `CR_FCEB_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1293 | `!defined(__IO_H__)` | `CR_FCEB_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1294 | `!defined(__IO_H__)` | `CR_FCEB_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1295 | `!defined(__IO_H__)` | `SYS1B_CR_FCEB_DV` | `0x119` | 系統暫存器/記憶體映射位址定義 |
| 1296 | `!defined(__IO_H__)` | `SET_CR_FCEB_DV(val)` | `(SYS1B[SYS1B_CR_FCEB_DV] \|= ((val <<4)  &  0xC0 ))` | 通用常數/硬體欄位定義 |
| 1297 | `!defined(__IO_H__)` | `SYS1B_CR_FCEB_PU` | `0x114` | 系統暫存器/記憶體映射位址定義 |
| 1298 | `!defined(__IO_H__)` | `SET_CR_FCEB_PU(val)` | `(SYS1B[SYS1B_CR_FCEB_PU] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1301 | `!defined(__IO_H__)` | `SYS1L_PAD_FCLE0_CFG` | `(0x120>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1302 | `!defined(__IO_H__)` | `CR_FCLE0_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1303 | `!defined(__IO_H__)` | `XFCLE0_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1304 | `!defined(__IO_H__)` | `CR_FCLE0_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1305 | `!defined(__IO_H__)` | `CR_FCLE0_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1306 | `!defined(__IO_H__)` | `CR_FCLE0_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1307 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE0_ODT_X` | `0x123` | 系統暫存器/記憶體映射位址定義 |
| 1308 | `!defined(__IO_H__)` | `SET_CR_FCLE0_ODT_X(val)` | `(SYS1B[SYS1B_CR_FCLE0_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1309 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE0_SR_X` | `0x122` | 系統暫存器/記憶體映射位址定義 |
| 1310 | `!defined(__IO_H__)` | `SET_CR_FCLE0_SR_X(val)` | `(SYS1B[SYS1B_CR_FCLE0_SR_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1311 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE0_DV_X` | `0x121` | 系統暫存器/記憶體映射位址定義 |
| 1312 | `!defined(__IO_H__)` | `SET_CR_FCLE0_DV_X(val)` | `(SYS1B[SYS1B_CR_FCLE0_DV_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1313 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE0_DT_X` | `0x121` | 系統暫存器/記憶體映射位址定義 |
| 1314 | `!defined(__IO_H__)` | `SET_CR_FCLE0_DT_X(val)` | `(SYS1B[SYS1B_CR_FCLE0_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1315 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE0_PU` | `0x120` | 系統暫存器/記憶體映射位址定義 |
| 1316 | `!defined(__IO_H__)` | `SET_CR_FCLE0_PU(val)` | `(SYS1B[SYS1B_CR_FCLE0_PU] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1318 | `!defined(__IO_H__)` | `SYS1L_PAD_FCLE1_CFG` | `(0x124>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1319 | `!defined(__IO_H__)` | `CR_FCLE1_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1320 | `!defined(__IO_H__)` | `XFCLE1_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1321 | `!defined(__IO_H__)` | `CR_FCLE1_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1322 | `!defined(__IO_H__)` | `CR_FCLE1_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1323 | `!defined(__IO_H__)` | `CR_FCLE1_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1324 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE1_ODT_X` | `0x127` | 系統暫存器/記憶體映射位址定義 |
| 1325 | `!defined(__IO_H__)` | `SET_CR_FCLE1_ODT_X(val)` | `(SYS1B[SYS1B_CR_FCLE1_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1326 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE1_SR_X` | `0x126` | 系統暫存器/記憶體映射位址定義 |
| 1327 | `!defined(__IO_H__)` | `SET_CR_FCLE1_SR_X(val)` | `(SYS1B[SYS1B_CR_FCLE1_SR_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1328 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE1_DV_X` | `0x125` | 系統暫存器/記憶體映射位址定義 |
| 1329 | `!defined(__IO_H__)` | `SET_CR_FCLE1_DV_X(val)` | `(SYS1B[SYS1B_CR_FCLE1_DV_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1330 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE1_DT_X` | `0x125` | 系統暫存器/記憶體映射位址定義 |
| 1331 | `!defined(__IO_H__)` | `SET_CR_FCLE1_DT_X(val)` | `(SYS1B[SYS1B_CR_FCLE1_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1332 | `!defined(__IO_H__)` | `SYS1B_CR_FCLE1_PU` | `0x124` | 系統暫存器/記憶體映射位址定義 |
| 1333 | `!defined(__IO_H__)` | `SET_CR_FCLE1_PU(val)` | `(SYS1B[SYS1B_CR_FCLE1_PU] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1335 | `!defined(__IO_H__)` | `SYS1L_PAD_FALE0_CFG` | `(0x140>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1336 | `!defined(__IO_H__)` | `CR_FALE0_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1337 | `!defined(__IO_H__)` | `XFALE0_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1338 | `!defined(__IO_H__)` | `CR_FALE0_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1339 | `!defined(__IO_H__)` | `CR_FALE0_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1340 | `!defined(__IO_H__)` | `CR_FALE0_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1341 | `!defined(__IO_H__)` | `SYS1B_CR_FALE0_ODT_X` | `0x143` | 系統暫存器/記憶體映射位址定義 |
| 1342 | `!defined(__IO_H__)` | `SET_CR_FALE0_ODT_X(val)` | `(SYS1B[SYS1B_CR_FALE0_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1343 | `!defined(__IO_H__)` | `SYS1B_CR_FALE0_SR_X` | `0x142` | 系統暫存器/記憶體映射位址定義 |
| 1344 | `!defined(__IO_H__)` | `SET_CR_FALE0_SR_X(val)` | `(SYS1B[SYS1B_CR_FALE0_SR_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1345 | `!defined(__IO_H__)` | `SYS1B_CR_FALE0_DV_X` | `0x141` | 系統暫存器/記憶體映射位址定義 |
| 1346 | `!defined(__IO_H__)` | `SET_CR_FALE0_DV_X(val)` | `(SYS1B[SYS1B_CR_FALE0_DV_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1347 | `!defined(__IO_H__)` | `SYS1B_CR_FALE0_DT_X` | `0x141` | 系統暫存器/記憶體映射位址定義 |
| 1348 | `!defined(__IO_H__)` | `SET_CR_FALE0_DT_X(val)` | `(SYS1B[SYS1B_CR_FALE0_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1349 | `!defined(__IO_H__)` | `SYS1B_CR_FALE0_PU` | `0x140` | 系統暫存器/記憶體映射位址定義 |
| 1350 | `!defined(__IO_H__)` | `SET_CR_FALE0_PU(val)` | `(SYS1B[SYS1B_CR_FALE0_PU] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1352 | `!defined(__IO_H__)` | `SYS1L_PAD_FALE1_CFG` | `(0x144>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1353 | `!defined(__IO_H__)` | `CR_FALE1_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1354 | `!defined(__IO_H__)` | `XFALE1_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1355 | `!defined(__IO_H__)` | `CR_FALE1_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1356 | `!defined(__IO_H__)` | `CR_FALE1_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1357 | `!defined(__IO_H__)` | `CR_FALE1_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1358 | `!defined(__IO_H__)` | `SYS1B_CR_FALE1_ODT_X` | `0x147` | 系統暫存器/記憶體映射位址定義 |
| 1359 | `!defined(__IO_H__)` | `SET_CR_FALE1_ODT_X(val)` | `(SYS1B[SYS1B_CR_FALE1_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1360 | `!defined(__IO_H__)` | `SYS1B_CR_FALE1_SR_X` | `0x146` | 系統暫存器/記憶體映射位址定義 |
| 1361 | `!defined(__IO_H__)` | `SET_CR_FALE1_SR_X(val)` | `(SYS1B[SYS1B_CR_FALE1_SR_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1362 | `!defined(__IO_H__)` | `SYS1B_CR_FALE1_DV_X` | `0x145` | 系統暫存器/記憶體映射位址定義 |
| 1363 | `!defined(__IO_H__)` | `SET_CR_FALE1_DV_X(val)` | `(SYS1B[SYS1B_CR_FALE1_DV_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1364 | `!defined(__IO_H__)` | `SYS1B_CR_FALE1_DT_X` | `0x145` | 系統暫存器/記憶體映射位址定義 |
| 1365 | `!defined(__IO_H__)` | `SET_CR_FALE1_DT_X(val)` | `(SYS1B[SYS1B_CR_FALE1_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1366 | `!defined(__IO_H__)` | `SYS1B_CR_FALE1_PU` | `0x144` | 系統暫存器/記憶體映射位址定義 |
| 1367 | `!defined(__IO_H__)` | `SET_CR_FALE1_PU(val)` | `(SYS1B[SYS1B_CR_FALE1_PU] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1369 | `!defined(__IO_H__)` | `SYS1L_PAD_FWEB0_CFG` | `(0x160>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1370 | `!defined(__IO_H__)` | `CR_FWEB0_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1371 | `!defined(__IO_H__)` | `XFWEB0_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1372 | `!defined(__IO_H__)` | `CR_FWEB0_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1373 | `!defined(__IO_H__)` | `CR_FWEB0_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1374 | `!defined(__IO_H__)` | `CR_FWEB0_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1375 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB0_ODT_X` | `0x163` | 系統暫存器/記憶體映射位址定義 |
| 1376 | `!defined(__IO_H__)` | `SET_CR_FWEB0_ODT_X(val)` | `(SYS1B[SYS1B_CR_FWEB0_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1377 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB0_SR_X` | `0x162` | 系統暫存器/記憶體映射位址定義 |
| 1378 | `!defined(__IO_H__)` | `SET_CR_FWEB0_SR_X(val)` | `(SYS1B[SYS1B_CR_FWEB0_SR_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1379 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB0_DV_X` | `0x161` | 系統暫存器/記憶體映射位址定義 |
| 1380 | `!defined(__IO_H__)` | `SET_CR_FWEB0_DV_X(val)` | `(SYS1B[SYS1B_CR_FWEB0_DV_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1381 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB0_DT_X` | `0x161` | 系統暫存器/記憶體映射位址定義 |
| 1382 | `!defined(__IO_H__)` | `SET_CR_FWEB0_DT_X(val)` | `(SYS1B[SYS1B_CR_FWEB0_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1383 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB0_PU` | `0x160` | 系統暫存器/記憶體映射位址定義 |
| 1384 | `!defined(__IO_H__)` | `SET_CR_FWEB0_PU(val)` | `(SYS1B[SYS1B_CR_FWEB0_PU] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1386 | `!defined(__IO_H__)` | `SYS1L_PAD_FWEB1_CFG` | `(0x164>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1387 | `!defined(__IO_H__)` | `CR_FWEB1_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1388 | `!defined(__IO_H__)` | `XFWEB1_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1389 | `!defined(__IO_H__)` | `CR_FWEB1_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1390 | `!defined(__IO_H__)` | `CR_FWEB1_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1391 | `!defined(__IO_H__)` | `CR_FWEB1_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1392 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB1_ODT_X` | `0x167` | 系統暫存器/記憶體映射位址定義 |
| 1393 | `!defined(__IO_H__)` | `SET_CR_FWEB1_ODT_X(val)` | `(SYS1B[SYS1B_CR_FWEB1_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1394 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB1_SR_X` | `0x166` | 系統暫存器/記憶體映射位址定義 |
| 1395 | `!defined(__IO_H__)` | `SET_CR_FWEB1_SR_X(val)` | `(SYS1B[SYS1B_CR_FWEB1_SR_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1396 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB1_DV_X` | `0x165` | 系統暫存器/記憶體映射位址定義 |
| 1397 | `!defined(__IO_H__)` | `SET_CR_FWEB1_DV_X(val)` | `(SYS1B[SYS1B_CR_FWEB1_DV_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1398 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB1_DT_X` | `0x165` | 系統暫存器/記憶體映射位址定義 |
| 1399 | `!defined(__IO_H__)` | `SET_CR_FWEB1_DT_X(val)` | `(SYS1B[SYS1B_CR_FWEB1_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1400 | `!defined(__IO_H__)` | `SYS1B_CR_FWEB1_PU` | `0x164` | 系統暫存器/記憶體映射位址定義 |
| 1401 | `!defined(__IO_H__)` | `SET_CR_FWEB1_PU(val)` | `(SYS1B[SYS1B_CR_FWEB1_PU] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1403 | `!defined(__IO_H__)` | `SYS1L_PAD_FREB0_CFG` | `(0x180>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1404 | `!defined(__IO_H__)` | `CR_FREB0_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1405 | `!defined(__IO_H__)` | `XFREB0_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1406 | `!defined(__IO_H__)` | `CR_FREB0_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1407 | `!defined(__IO_H__)` | `CR_FREB0_PU_X` | `BIT4` | 通用常數/硬體欄位定義 |
| 1408 | `!defined(__IO_H__)` | `CR_FREB0_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1409 | `!defined(__IO_H__)` | `CR_FREB0_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1410 | `!defined(__IO_H__)` | `SYS1B_CR_FREB0_ODT_X` | `0x183` | 系統暫存器/記憶體映射位址定義 |
| 1411 | `!defined(__IO_H__)` | `SET_CR_FREB0_ODT_X(val)` | `(SYS1B[SYS1B_CR_FREB0_ODT_X] \|= ((val <<4)  &  0x30 ])` | 通用常數/硬體欄位定義 |
| 1412 | `!defined(__IO_H__)` | `SYS1B_CR_FREB0_SR_X` | `0x182` | 系統暫存器/記憶體映射位址定義 |
| 1413 | `!defined(__IO_H__)` | `SET_CR_FREB0_SR_X(val)` | `(SYS1B[SYS1B_CR_FREB0_SR_X] \|= ((val <<4)  &  0xF0 ])` | 通用常數/硬體欄位定義 |
| 1414 | `!defined(__IO_H__)` | `SYS1B_CR_FREB0_DV_X` | `0x181` | 系統暫存器/記憶體映射位址定義 |
| 1415 | `!defined(__IO_H__)` | `SET_CR_FREB0_DV_X(val)` | `(SYS1B[SYS1B_CR_FREB0_DV_X] \|= ((val <<4)  &  0xF0 ])` | 通用常數/硬體欄位定義 |
| 1416 | `!defined(__IO_H__)` | `SYS1B_CR_FREB0_DT_X` | `0x181` | 系統暫存器/記憶體映射位址定義 |
| 1417 | `!defined(__IO_H__)` | `SET_CR_FREB0_DT_X(val)` | `(SYS1B[SYS1B_CR_FREB0_DT_X] \|= (val  &  0x0F ])` | 通用常數/硬體欄位定義 |
| 1420 | `!defined(__IO_H__)` | `SYS1L_PAD_FREB1_CFG` | `(0x184>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1421 | `!defined(__IO_H__)` | `CR_FREB1_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1422 | `!defined(__IO_H__)` | `XFREB1_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1423 | `!defined(__IO_H__)` | `CR_FREB1_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1424 | `!defined(__IO_H__)` | `CR_FREB1_PU_X` | `BIT4` | 通用常數/硬體欄位定義 |
| 1425 | `!defined(__IO_H__)` | `CR_FREB1_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1426 | `!defined(__IO_H__)` | `CR_FREB1_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1427 | `!defined(__IO_H__)` | `SYS1B_CR_FREB1_ODT_X` | `0x187` | 系統暫存器/記憶體映射位址定義 |
| 1428 | `!defined(__IO_H__)` | `SET_CR_FREB1_ODT_X(val)` | `(SYS1B[SYS1B_CR_FREB1_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1429 | `!defined(__IO_H__)` | `SYS1B_CR_FREB1_SR_X` | `0x186` | 系統暫存器/記憶體映射位址定義 |
| 1430 | `!defined(__IO_H__)` | `SET_CR_FREB1_SR_X(val)` | `(SYS1B[SYS1B_CR_FREB1_SR_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1431 | `!defined(__IO_H__)` | `SYS1B_CR_FREB1_DV_X` | `0x185` | 系統暫存器/記憶體映射位址定義 |
| 1432 | `!defined(__IO_H__)` | `SET_CR_FREB1_DV_X(val)` | `(SYS1B[SYS1B_CR_FREB1_DV_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1433 | `!defined(__IO_H__)` | `SYS1B_CR_FREB1_DT_X` | `0x185` | 系統暫存器/記憶體映射位址定義 |
| 1434 | `!defined(__IO_H__)` | `SET_CR_FREB1_DT_X(val)` | `(SYS1B[SYS1B_CR_FREB1_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1437 | `!defined(__IO_H__)` | `SYS1L_PAD_FDQS0_CFG` | `(0x1A0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1438 | `!defined(__IO_H__)` | `CR_FDQS0_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1439 | `!defined(__IO_H__)` | `XFDQS0_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1440 | `!defined(__IO_H__)` | `XDQSB0_I` | `BIT24` | 通用常數/硬體欄位定義 |
| 1441 | `!defined(__IO_H__)` | `CR_FDQS0_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1442 | `!defined(__IO_H__)` | `CR_FDQS0_PU_X` | `BIT4` | 通用常數/硬體欄位定義 |
| 1443 | `!defined(__IO_H__)` | `CR_FDQS0_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1444 | `!defined(__IO_H__)` | `CR_FDQS0_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1445 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS0_ODT_X` | `0x1A3` | 系統暫存器/記憶體映射位址定義 |
| 1446 | `!defined(__IO_H__)` | `SET_CR_FDQS0_ODT_X(val)` | `(SYS1B[SYS1B_CR_FDQS0_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1447 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS0_SR_X` | `0x1A2` | 系統暫存器/記憶體映射位址定義 |
| 1448 | `!defined(__IO_H__)` | `SET_CR_FDQS0_SR_X(val)` | `(SYS1B[SYS1B_CR_FDQS0_SR_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1449 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS0_SRB_X` | `0x1A2` | 系統暫存器/記憶體映射位址定義 |
| 1450 | `!defined(__IO_H__)` | `SET_CR_FDQS0_SRB_X(val)` | `(SYS1B[SYS1B_CR_FDQS0_SRB_X] \|= ((val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1451 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS0_DV_X` | `0x1A1` | 系統暫存器/記憶體映射位址定義 |
| 1452 | `!defined(__IO_H__)` | `SET_CR_FDQS0_DV_X(val)` | `(SYS1B[SYS1B_CR_FDQS0_DV_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1453 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS0_DT_X` | `0x1A1` | 系統暫存器/記憶體映射位址定義 |
| 1454 | `!defined(__IO_H__)` | `SET_CR_FDQS0_DT_X(val)` | `(SYS1B[SYS1B_CR_FDQS0_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1456 | `!defined(__IO_H__)` | `SET_ODT_150ohm` | `0` | 通用常數/硬體欄位定義 |
| 1457 | `!defined(__IO_H__)` | `SET_ODT_100ohm` | `1` | 通用常數/硬體欄位定義 |
| 1458 | `!defined(__IO_H__)` | `SET_ODT_75ohm` | `2` | 通用常數/硬體欄位定義 |
| 1459 | `!defined(__IO_H__)` | `SET_ODT_50ohm` | `3` | 通用常數/硬體欄位定義 |
| 1461 | `!defined(__IO_H__)` | `SYS1L_PAD_FDQS1_CFG` | `(0x1A4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1462 | `!defined(__IO_H__)` | `CR_FDQS1_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1463 | `!defined(__IO_H__)` | `XFDQS1_I` | `BIT25` | 通用常數/硬體欄位定義 |
| 1464 | `!defined(__IO_H__)` | `XDQSB0_I` | `BIT24` | 通用常數/硬體欄位定義 |
| 1465 | `!defined(__IO_H__)` | `CR_FDQS1_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1466 | `!defined(__IO_H__)` | `CR_FDQS1_PU_X` | `BIT4` | 通用常數/硬體欄位定義 |
| 1467 | `!defined(__IO_H__)` | `CR_FDQS1_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1468 | `!defined(__IO_H__)` | `CR_FDQS1_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1469 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS1_ODT_X` | `0x1A7` | 系統暫存器/記憶體映射位址定義 |
| 1470 | `!defined(__IO_H__)` | `SET_CR_FDQS1_ODT_X(val)` | `(SYS1B[SYS1B_CR_FDQS1_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1471 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS1_SR_X` | `0x1A6` | 系統暫存器/記憶體映射位址定義 |
| 1472 | `!defined(__IO_H__)` | `SET_CR_FDQS1_SR_X(val)` | `(SYS1B[SYS1B_CR_FDQS1_SR_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1473 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS1_SRB_X` | `0x1A6` | 系統暫存器/記憶體映射位址定義 |
| 1474 | `!defined(__IO_H__)` | `SET_CR_FDQS1_SRB_X(val)` | `(SYS1B[SYS1B_CR_FDQS1_SRB_X] \|= ((val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1475 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS1_DV_X` | `0x1A5` | 系統暫存器/記憶體映射位址定義 |
| 1476 | `!defined(__IO_H__)` | `SET_CR_FDQS1_DV_X(val)` | `(SYS1B[SYS1B_CR_FDQS1_DV_X] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1477 | `!defined(__IO_H__)` | `SYS1B_CR_FDQS1_DT_X` | `0x1A5` | 系統暫存器/記憶體映射位址定義 |
| 1478 | `!defined(__IO_H__)` | `SET_CR_FDQS1_DT_X(val)` | `(SYS1B[SYS1B_CR_FDQS1_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1480 | `!defined(__IO_H__)` | `SYS1L_PAD_FDAT0_CFG` | `(0x1C0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1481 | `!defined(__IO_H__)` | `CR_FDAT0_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1482 | `!defined(__IO_H__)` | `CR_FDAT0_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1483 | `!defined(__IO_H__)` | `CR_FDAT0_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1484 | `!defined(__IO_H__)` | `CR_FDAT0_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1485 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT0_ODT_X` | `0x1C3` | 系統暫存器/記憶體映射位址定義 |
| 1486 | `!defined(__IO_H__)` | `SET_CR_FDAT0_ODT_X(val)` | `(SYS1B[SYS1B_CR_FDAT0_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1487 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT0_DT_X` | `0x1C1` | 系統暫存器/記憶體映射位址定義 |
| 1488 | `!defined(__IO_H__)` | `SET_CR_FDAT0_DT_X(val)` | `(SYS1B[SYS1B_CR_FDAT0_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1489 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT0_PU_X` | `0x1C0` | 系統暫存器/記憶體映射位址定義 |
| 1490 | `!defined(__IO_H__)` | `SET_CR_FDAT0_PU_X(val)` | `(SYS1B[SYS1B_CR_FDAT0_PU_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1492 | `!defined(__IO_H__)` | `SYS1L_PAD_FDAT1_CFG` | `(0x1C4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1493 | `!defined(__IO_H__)` | `CR_FDAT1_ODT_EN` | `BIT27` | 通用常數/硬體欄位定義 |
| 1494 | `!defined(__IO_H__)` | `CR_FDAT1_PD` | `BIT6` | 通用常數/硬體欄位定義 |
| 1495 | `!defined(__IO_H__)` | `CR_FDAT1_CO` | `BIT1` | 通用常數/硬體欄位定義 |
| 1496 | `!defined(__IO_H__)` | `CR_FDAT1_SO` | `BIT0` | 通用常數/硬體欄位定義 |
| 1497 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT1_ODT_X` | `0x1C3` | 系統暫存器/記憶體映射位址定義 |
| 1498 | `!defined(__IO_H__)` | `SET_CR_FDAT1_ODT_X(val)` | `(SYS1B[SYS1B_CR_FDAT1_ODT_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1499 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT1_DT_X` | `0x1C1` | 系統暫存器/記憶體映射位址定義 |
| 1500 | `!defined(__IO_H__)` | `SET_CR_FDAT1_DT_X(val)` | `(SYS1B[SYS1B_CR_FDAT1_DT_X] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1501 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT1_PU_X` | `0x1C0` | 系統暫存器/記憶體映射位址定義 |
| 1502 | `!defined(__IO_H__)` | `SET_CR_FDAT1_PU_X(val)` | `(SYS1B[SYS1B_CR_FDAT1_PU_X] \|= ((val <<4)  &  0x30 ))` | 通用常數/硬體欄位定義 |
| 1504 | `!defined(__IO_H__)` | `SYS1L_PAD_FDATA_DVSR0` | `(0x1E0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1505 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X3` | `0x1E3` | 系統暫存器/記憶體映射位址定義 |
| 1506 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X3(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X3] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1507 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X3` | `0x1E3` | 系統暫存器/記憶體映射位址定義 |
| 1508 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X3(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X3] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1509 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X2` | `0x1E2` | 系統暫存器/記憶體映射位址定義 |
| 1510 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X2(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X2] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1511 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X2` | `0x1E2` | 系統暫存器/記憶體映射位址定義 |
| 1512 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X2(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X2] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1513 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X1` | `0x1E1` | 系統暫存器/記憶體映射位址定義 |
| 1514 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X1(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X1] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1515 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X1` | `0x1E1` | 系統暫存器/記憶體映射位址定義 |
| 1516 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X1(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X1] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1517 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X0` | `0x1E0` | 系統暫存器/記憶體映射位址定義 |
| 1518 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X0(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X0] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1519 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X0` | `0x1E0` | 系統暫存器/記憶體映射位址定義 |
| 1520 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X0(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X0] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1522 | `!defined(__IO_H__)` | `SYS1L_PAD_FDATA_DVSR1` | `(0x1E4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1523 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X7` | `0x1E7` | 系統暫存器/記憶體映射位址定義 |
| 1524 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X7(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X7] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1525 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X7` | `0x1E7` | 系統暫存器/記憶體映射位址定義 |
| 1526 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X7(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X7] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1527 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X6` | `0x1E6` | 系統暫存器/記憶體映射位址定義 |
| 1528 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X6(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X6] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1529 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X6` | `0x1E6` | 系統暫存器/記憶體映射位址定義 |
| 1530 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X6(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X6] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1531 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X5` | `0x1E5` | 系統暫存器/記憶體映射位址定義 |
| 1532 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X5(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X5] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1533 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X5` | `0x1E5` | 系統暫存器/記憶體映射位址定義 |
| 1534 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X5(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X5] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1535 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X4` | `0x1E4` | 系統暫存器/記憶體映射位址定義 |
| 1536 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X4(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X4] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1537 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X4` | `0x1E4` | 系統暫存器/記憶體映射位址定義 |
| 1538 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X4(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X4] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1540 | `!defined(__IO_H__)` | `SYS1L_PAD_FDATA_DVSR2` | `(0x1E8>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1541 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_XB` | `0x1EB` | 系統暫存器/記憶體映射位址定義 |
| 1542 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_XB(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_XB] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1543 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_XB` | `0x1EB` | 系統暫存器/記憶體映射位址定義 |
| 1544 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_XB(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_XB] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1545 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_XA` | `0x1EA` | 系統暫存器/記憶體映射位址定義 |
| 1546 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_XA(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_XA] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1547 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_XA` | `0x1EA` | 系統暫存器/記憶體映射位址定義 |
| 1548 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_XA(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_XA] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1549 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X9` | `0x1E9` | 系統暫存器/記憶體映射位址定義 |
| 1550 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X9(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X9] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1551 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X9` | `0x1E9` | 系統暫存器/記憶體映射位址定義 |
| 1552 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X9(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X9] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1553 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_X8` | `0x1E8` | 系統暫存器/記憶體映射位址定義 |
| 1554 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_X8(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_X8] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1555 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_X8` | `0x1E8` | 系統暫存器/記憶體映射位址定義 |
| 1556 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_X8(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_X8] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1559 | `!defined(__IO_H__)` | `SYS1L_PAD_FDATA_DVSR3` | `(0x1EC>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1560 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_XF` | `0x1EF` | 系統暫存器/記憶體映射位址定義 |
| 1561 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_XF(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_XF] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1562 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_XF` | `0x1EF` | 系統暫存器/記憶體映射位址定義 |
| 1563 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_XF(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_XF] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1564 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_XE` | `0x1EE` | 系統暫存器/記憶體映射位址定義 |
| 1565 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_XE(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_XE] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1566 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_XE` | `0x1EE` | 系統暫存器/記憶體映射位址定義 |
| 1567 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_XE(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_XE] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1568 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_XD` | `0x1ED` | 系統暫存器/記憶體映射位址定義 |
| 1569 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_XD(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_XD] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1570 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_XD` | `0x1ED` | 系統暫存器/記憶體映射位址定義 |
| 1571 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_XD(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_XD] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1572 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_DV_XC` | `0x1EC` | 系統暫存器/記憶體映射位址定義 |
| 1573 | `!defined(__IO_H__)` | `SET_CR_FDAT_DV_XC(val)` | `(SYS1B[SYS1B_CR_FDAT_DV_XC] \|= ((val <<4)  &  0xF0 ))` | 通用常數/硬體欄位定義 |
| 1574 | `!defined(__IO_H__)` | `SYS1B_CR_FDAT_SR_XC` | `0x1EC` | 系統暫存器/記憶體映射位址定義 |
| 1575 | `!defined(__IO_H__)` | `SET_CR_FDAT_SR_XC(val)` | `(SYS1B[SYS1B_CR_FDAT_SR_XC] \|= (val  &  0x0F ))` | 通用常數/硬體欄位定義 |
| 1580 | `!defined(__IO_H__)` | `SYS1L_EDGE_DET_EN` | `(0x200>>2)` | 系統暫存器/記憶體映射位址定義；註解：/////////////GPIO |
| 1581 | `!defined(__IO_H__)` | `CR_JTAG_DET_EN` | `BIT4` | 通用常數/硬體欄位定義 |
| 1582 | `!defined(__IO_H__)` | `SYS1B_CR_VDT_DET_EN` | `0x203` | 系統暫存器/記憶體映射位址定義 |
| 1583 | `!defined(__IO_H__)` | `SET_CR_VDT_DET_EN(val)` | `(SYS1B[SYS1B_CR_VDT_DET_EN] \|= (val  &  0x1F ))` | 通用常數/硬體欄位定義 |
| 1584 | `!defined(__IO_H__)` | `SYS1B_CR_TS_DET_EN` | `0x202` | 系統暫存器/記憶體映射位址定義 |
| 1585 | `!defined(__IO_H__)` | `SET_CR_TS_DET_EN(val)` | `(SYS1B[SYS1B_CR_TS_DET_EN] \|= (val  &  0x03 ))` | 通用常數/硬體欄位定義 |
| 1586 | `!defined(__IO_H__)` | `SYS1B_CR_PLL_LOCK_DET_EN` | `0x201` | 系統暫存器/記憶體映射位址定義 |
| 1587 | `!defined(__IO_H__)` | `SET_CR_PLL_LOCK_DET_EN(val)` | `(SYS1B[SYS1B_CR_PLL_LOCK_DET_EN] \|= (val  &  0x07 ))` | 通用常數/硬體欄位定義 |
| 1588 | `!defined(__IO_H__)` | `SYS1B_CR_GPIO_DET_EN` | `0x200` | 系統暫存器/記憶體映射位址定義 |
| 1589 | `!defined(__IO_H__)` | `SET_CR_GPIO_DET_EN(val)` | `(SYS1B[SYS1B_CR_GPIO_DET_EN] \|= (val  &  0x0F ))` | 周邊控制暫存器/位元欄位定義 |
| 1592 | `!defined(__IO_H__)` | `SYS1L_GPIO_DET_SEL` | `(0x204>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1593 | `!defined(__IO_H__)` | `SYS1B_CR_GPIO_DET_SEL_3` | `0x207` | 系統暫存器/記憶體映射位址定義 |
| 1594 | `!defined(__IO_H__)` | `SET_CR_GPIO_DET_SEL_3(val)` | `(SYS1B[SYS1B_CR_GPIO_DET_SEL_3] \|= (val  &  0x0F ))` | 周邊控制暫存器/位元欄位定義 |
| 1595 | `!defined(__IO_H__)` | `SYS1B_CR_GPIO_DET_SEL_2` | `0x206` | 系統暫存器/記憶體映射位址定義 |
| 1596 | `!defined(__IO_H__)` | `SET_CR_GPIO_DET_SEL_2(val)` | `(SYS1B[SYS1B_CR_GPIO_DET_SEL_2] \|= (val  &  0x0F ))` | 周邊控制暫存器/位元欄位定義 |
| 1597 | `!defined(__IO_H__)` | `SYS1B_CR_GPIO_DET_SEL_1` | `0x205` | 系統暫存器/記憶體映射位址定義 |
| 1598 | `!defined(__IO_H__)` | `SET_CR_GPIO_DET_SEL_1(val)` | `(SYS1B[SYS1B_CR_GPIO_DET_SEL_1] \|= (val  &  0x0F ))` | 周邊控制暫存器/位元欄位定義 |
| 1599 | `!defined(__IO_H__)` | `SYS1B_CR_GPIO_DET_SEL_0` | `0x204` | 系統暫存器/記憶體映射位址定義 |
| 1600 | `!defined(__IO_H__)` | `SET_CR_GPIO_DET_SEL_0(val)` | `(SYS1B[SYS1B_CR_GPIO_DET_SEL_0] \|= (val  &  0x0F ))` | 周邊控制暫存器/位元欄位定義 |
| 1602 | `!defined(__IO_H__)` | `SYS1L_CNT_VAL0` | `(0x208>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1603 | `!defined(__IO_H__)` | `SYS1B_CNT_VAL0` | `0x208` | 系統暫存器/記憶體映射位址定義 |
| 1604 | `!defined(__IO_H__)` | `SYS1B_CNT_VAL1` | `0x209` | 系統暫存器/記憶體映射位址定義 |
| 1605 | `!defined(__IO_H__)` | `SYS1B_CNT_VAL2` | `0x20A` | 系統暫存器/記憶體映射位址定義 |
| 1606 | `!defined(__IO_H__)` | `SYS1B_CNT_VAL3` | `0x20B` | 系統暫存器/記憶體映射位址定義 |
| 1608 | `!defined(__IO_H__)` | `SYS1L_CNT_VAL1` | `(0x20C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1609 | `!defined(__IO_H__)` | `SYS1B_CNT_VAL4` | `0x20C` | 系統暫存器/記憶體映射位址定義 |
| 1610 | `!defined(__IO_H__)` | `SYS1B_CNT_VAL5` | `0x20D` | 系統暫存器/記憶體映射位址定義 |
| 1611 | `!defined(__IO_H__)` | `SYS1B_CNT_VAL6` | `0x20E` | 系統暫存器/記憶體映射位址定義 |
| 1612 | `!defined(__IO_H__)` | `SYS1B_CNT_VAL7` | `0x20F` | 系統暫存器/記憶體映射位址定義 |
| 1615 | `!defined(__IO_H__)` | `SYS1L_RNG_CTRL` | `(0x300>>2)` | 系統暫存器/記憶體映射位址定義；註解：/////////////RNG |
| 1616 | `!defined(__IO_H__)` | `RNG_RUN_ENABLE` | `BIT7` | 通用常數/硬體欄位定義 |
| 1617 | `!defined(__IO_H__)` | `RNG_RNG_ENABLE` | `BIT6` | 通用常數/硬體欄位定義 |
| 1618 | `!defined(__IO_H__)` | `RNG_MODE_NOISE_GENERATOR` | `0x00` | 通用常數/硬體欄位定義 |
| 1619 | `!defined(__IO_H__)` | `RNG_MODE_NOISE_PAD` | `0x01` | 通用常數/硬體欄位定義 |
| 1620 | `!defined(__IO_H__)` | `RNG_MODE_RANDOM_METHOD` | `0x11` | 依原始註解：0x1x；註解：0x1x |
| 1623 | `!defined(__IO_H__)` | `SYS1L_RNG_REGL` | `(0x308>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1624 | `!defined(__IO_H__)` | `SYS1L_RNG_REGH` | `(0x30C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1628 | `!defined(__IO_H__)` | `SYS1L_EFU_MODE_SEL` | `(0x400>>2)` | 系統暫存器/記憶體映射位址定義；註解：///////////////  EFUSE |
| 1629 | `!defined(__IO_H__)` | `SYS1B_EFU_MODE_SEL` | `0x400` | 系統暫存器/記憶體映射位址定義 |
| 1630 | `!defined(__IO_H__)` | `SET_EFU_MODE_SEL(val)` | `(SYS1B[SYS1B_EFU_MODE_SEL] \|= (val  &  0x07 ))` | 通用常數/硬體欄位定義 |
| 1632 | `!defined(__IO_H__)` | `SYS1L_EFU_RDY` | `(0x404>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1633 | `!defined(__IO_H__)` | `EFU_READY` | `BIT0` | 通用常數/硬體欄位定義 |
| 1635 | `!defined(__IO_H__)` | `SYS1L_EFU_STRB` | `(0x408>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1636 | `!defined(__IO_H__)` | `SYS1B_READ_EFU_INTVL` | `0x40B` | 系統暫存器/記憶體映射位址定義 |
| 1637 | `!defined(__IO_H__)` | `SET_READ_EFU_INTVL(val)` | `(SYS1B[SYS1B_READ_EFU_INTVL] \|= (val  &  0x03 ))` | 通用常數/硬體欄位定義 |
| 1638 | `!defined(__IO_H__)` | `SYS1B_READ_EFU_WIDTH` | `0x40A` | 系統暫存器/記憶體映射位址定義 |
| 1639 | `!defined(__IO_H__)` | `SET_READ_EFU_WIDTH(val)` | `(SYS1B[SYS1B_READ_EFU_WIDTH] \|= (val  &  0x03 ))` | 通用常數/硬體欄位定義 |
| 1640 | `!defined(__IO_H__)` | `SYS1B_PRGM_STRB_INTVL` | `0x409` | 系統暫存器/記憶體映射位址定義 |
| 1641 | `!defined(__IO_H__)` | `SYS1B_PRGM_STRB_WIDTH` | `0x408` | 系統暫存器/記憶體映射位址定義 |
| 1643 | `!defined(__IO_H__)` | `SYS1L_PROG_WORD_REG` | `(0x40C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1645 | `!defined(__IO_H__)` | `SYS1L_EFU_PROG_START` | `(0x414>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1646 | `!defined(__IO_H__)` | `PROGRAM_MODE_START` | `BIT0` | 通用常數/硬體欄位定義 |
| 1648 | `!defined(__IO_H__)` | `SYS1L_EFU_CTRL` | `(0x430>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1649 | `!defined(__IO_H__)` | `EFUC_CSB` | `BIT22` | 通用常數/硬體欄位定義 |
| 1650 | `!defined(__IO_H__)` | `EFUC_LOAD` | `BIT21` | 通用常數/硬體欄位定義 |
| 1651 | `!defined(__IO_H__)` | `EFUC_CPGENB` | `BIT20` | 通用常數/硬體欄位定義 |
| 1652 | `!defined(__IO_H__)` | `EFUC_STROBE` | `BIT19` | 通用常數/硬體欄位定義 |
| 1653 | `!defined(__IO_H__)` | `EFUC_PS` | `BIT18` | 通用常數/硬體欄位定義 |
| 1654 | `!defined(__IO_H__)` | `EFUC_PD` | `BIT17` | 通用常數/硬體欄位定義 |
| 1655 | `!defined(__IO_H__)` | `EFUC_ENPOR` | `BIT16` | 通用常數/硬體欄位定義 |
| 1656 | `!defined(__IO_H__)` | `SYS1W_EFUC_A` | `(0x430>>1)` | 系統暫存器/記憶體映射位址定義 |
| 1657 | `!defined(__IO_H__)` | `SET_EFUC_A` | `(SYS1W[SYS1W_EFUC_A] \|= (val  &  0x03FF ))` | 通用常數/硬體欄位定義 |
| 1659 | `!defined(__IO_H__)` | `SYS1B_EFU_STS` | `0x43C` | 系統暫存器/記憶體映射位址定義 |
| 1660 | `!defined(__IO_H__)` | `GET_EFU_STS` | `(SYS1B[SYS1B_EFU_STS] & 0x01)` | 通用常數/硬體欄位定義 |
| 1662 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD0` | `(0x440>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1663 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD1` | `(0x444>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1664 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD2` | `(0x448>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1665 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD3` | `(0x44C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1666 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD4` | `(0x450>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1667 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD5` | `(0x454>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1668 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD6` | `(0x458>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1669 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD7` | `(0x45C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1670 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD8` | `(0x460>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1671 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD9` | `(0x464>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1672 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD10` | `(0x468>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1673 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD11` | `(0x46C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1674 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD12` | `(0x470>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1675 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD13` | `(0x474>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1676 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD14` | `(0x478>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1677 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD15` | `(0x47C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1678 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD16` | `(0x480>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1679 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD17` | `(0x484>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1680 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD18` | `(0x488>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1681 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD19` | `(0x48C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1682 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD20` | `(0x490>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1683 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD21` | `(0x494>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1684 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD22` | `(0x498>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1685 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD23` | `(0x49C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1686 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD24` | `(0x4A0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1687 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD25` | `(0x4A4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1688 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD26` | `(0x4A8>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1689 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD27` | `(0x4AC>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1690 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD28` | `(0x4B0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1691 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD29` | `(0x4B4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1692 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD30` | `(0x4B8>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1693 | `!defined(__IO_H__)` | `SYS1L_PROGRAM_WORD31` | `(0x4BC>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1695 | `!defined(__IO_H__)` | `SYS1L_CMP_PW0` | `(0x4C0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1696 | `!defined(__IO_H__)` | `SYS1L_CMP_PW1` | `(0x4C4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1697 | `!defined(__IO_H__)` | `SYS1L_CMP_PW2` | `(0x4C8>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1698 | `!defined(__IO_H__)` | `SYS1L_CMP_PW3` | `(0x4CC>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1699 | `!defined(__IO_H__)` | `SYS1L_CMP_PW4` | `(0x4D0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1700 | `!defined(__IO_H__)` | `SYS1L_CMP_PW5` | `(0x4D4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1701 | `!defined(__IO_H__)` | `SYS1L_CMP_PW6` | `(0x4D8>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1702 | `!defined(__IO_H__)` | `SYS1L_CMP_PW7` | `(0x4DC>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1703 | `!defined(__IO_H__)` | `SYS1L_CMP_PW8` | `(0x4E0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1704 | `!defined(__IO_H__)` | `SYS1L_CMP_PW9` | `(0x4E4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1705 | `!defined(__IO_H__)` | `SYS1L_CMP_PW10` | `(0x4E8>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1706 | `!defined(__IO_H__)` | `SYS1L_CMP_PW11` | `(0x4EC>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1707 | `!defined(__IO_H__)` | `SYS1L_CMP_PW12` | `(0x4F0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1708 | `!defined(__IO_H__)` | `SYS1L_CMP_PW13` | `(0x4F4>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1709 | `!defined(__IO_H__)` | `SYS1L_CMP_PW14` | `(0x4F8>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1710 | `!defined(__IO_H__)` | `SYS1L_CMP_PW15` | `(0x4FC>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1715 | `!defined(__IO_H__)` | `SYS1L_I2C_GP1_SLAVE_ADDR` | `(0x500>>2)` | 系統暫存器/記憶體映射位址定義；註解：///////////////  I2C |
| 1716 | `!defined(__IO_H__)` | `SYS1L_I2C_GP1_OP_REG` | `(0x504>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1717 | `!defined(__IO_H__)` | `SYS1L_I2C_GP1_CTRL` | `(0x50C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1718 | `!defined(__IO_H__)` | `I2C_START` | `BIT0` | 通用常數/硬體欄位定義 |
| 1719 | `!defined(__IO_H__)` | `I2C_ENABLE_DONE_INT` | `BIT1` | 通用常數/硬體欄位定義 |
| 1720 | `!defined(__IO_H__)` | `I2C_HS_MODE` | `BIT2` | 通用常數/硬體欄位定義 |
| 1721 | `!defined(__IO_H__)` | `I2C_COMBINED_MODE` | `BIT3` | 通用常數/硬體欄位定義 |
| 1722 | `!defined(__IO_H__)` | `I2C_BUF_SELECT` | `BIT4` | 通用常數/硬體欄位定義 |
| 1723 | `!defined(__IO_H__)` | `I2C_PD_CTRL` | `BIT5` | 通用常數/硬體欄位定義 |
| 1724 | `!defined(__IO_H__)` | `I2C_ADDRESS10_ENABLE` | `BIT6` | 通用常數/硬體欄位定義 |
| 1725 | `!defined(__IO_H__)` | `I2C_READ` | `BIT7` | 依原始註解：Write is set to 0；註解：Write is set to 0 |
| 1726 | `!defined(__IO_H__)` | `I2C_WRITE` | `0` | 依原始註解：Write is set to 0；註解：Write is set to 0 |
| 1727 | `!defined(__IO_H__)` | `SYS1B_I2C_GP1_DATA_CNT` | `0x50D` | 系統暫存器/記憶體映射位址定義 |
| 1728 | `!defined(__IO_H__)` | `SYS1B_I2C_GP1_OP_CNT` | `0x50E` | 系統暫存器/記憶體映射位址定義 |
| 1729 | `!defined(__IO_H__)` | `SYS1L_I2C_GP1_NACK` | `(0x510>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1730 | `!defined(__IO_H__)` | `GP1_DNACK` | `BIT1` | 通用常數/硬體欄位定義 |
| 1731 | `!defined(__IO_H__)` | `GP1_ANACK` | `BIT0` | 通用常數/硬體欄位定義 |
| 1732 | `!defined(__IO_H__)` | `SYS1L_I2C_GP1_CLK_DIV_CNT` | `(0x514>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1736 | `!defined(__IO_H__)` | `I2C_Slave_Address_Register(ubGroupsel)` | `(SYS1L_I2C_GP1_SLAVE_ADDR + (ubGroupsel-1)*(0x20>>2))` | 通用常數/硬體欄位定義 |
| 1737 | `!defined(__IO_H__)` | `I2C_OP_Register(ubGroupsel)` | `(SYS1L_I2C_GP1_OP_REG + (ubGroupsel-1)*(0x20>>2))` | 通用常數/硬體欄位定義 |
| 1738 | `!defined(__IO_H__)` | `I2C_Ctrl_Register(ubGroupsel)` | `(SYS1L_I2C_GP1_CTRL + (ubGroupsel-1)*(0x20>>2))` | 通用常數/硬體欄位定義 |
| 1739 | `!defined(__IO_H__)` | `I2C_DATA_Count(ubGroupsel)` | `(SYS1B_I2C_GP1_DATA_CNT + (ubGroupsel-1)*(0x20>>2))` | 通用常數/硬體欄位定義 |
| 1740 | `!defined(__IO_H__)` | `I2C_OP_Count(ubGroupsel)` | `(SYS1B_I2C_GP1_OP_CNT + (ubGroupsel-1)*(0x20>>2))` | 通用常數/硬體欄位定義 |
| 1741 | `!defined(__IO_H__)` | `I2C_NACK(ubGroupsel)` | `(SYS1L_I2C_GP1_NACK + (ubGroupsel-1)*(0x20>>2))` | 通用常數/硬體欄位定義 |
| 1742 | `!defined(__IO_H__)` | `I2C_CLK_DIV_CNT(ubGroupsel,val)` | `(SYS1B[SYS1L_I2C_GP1_CLK_DIV_CNT+ (ubGroupsel-1)*(0x20>>2)] = (unsigned char)(val))` | 通用常數/硬體欄位定義 |
| 1746 | `!defined(__IO_H__)` | `SYS1L_I2C_INT_REG` | `(0x518>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1747 | `!defined(__IO_H__)` | `I2C_INT_EVENT` | `BIT0` | 通用常數/硬體欄位定義 |
| 1748 | `!defined(__IO_H__)` | `I2C_GP4_DONE_INT_EN` | `BIT3` | 通用常數/硬體欄位定義 |
| 1749 | `!defined(__IO_H__)` | `I2C_GP3_DONE_INT_EN` | `BIT4` | 通用常數/硬體欄位定義 |
| 1750 | `!defined(__IO_H__)` | `I2C_GP2_NACK_INT_EN` | `BIT5` | 通用常數/硬體欄位定義 |
| 1751 | `!defined(__IO_H__)` | `I2C_GP1_NACK_INT_EN` | `BIT6` | 通用常數/硬體欄位定義 |
| 1752 | `!defined(__IO_H__)` | `I2C_ENABLE` | `BIT7` | 通用常數/硬體欄位定義 |
| 1754 | `!defined(__IO_H__)` | `SYS1L_I2C_BUF0_REG` | `(0x580>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1755 | `!defined(__IO_H__)` | `SYS1L_I2C_BUF1_REG` | `(0x5C0>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1756 | `!defined(__IO_H__)` | `I2C_BUFF_Register(ubBufsel)` | `(SYS1L_I2C_BUF0_REG + (ubBufsel-1)*0x40)` | 通用常數/硬體欄位定義 |
| 1758 | `!defined(__IO_H__)` | `TMP101_Temperature_Register` | `0` | 依原始註解：Read Only；註解：Read Only |
| 1759 | `!defined(__IO_H__)` | `TMP101_Configuration_Register` | `1` | 依原始註解：Read/Write；註解：Read/Write |
| 1760 | `!defined(__IO_H__)` | `TMP101_TLOW_Register` | `2` | 依原始註解：Read/Write (HIGH-LIMIT REGISTER)；註解：Read/Write (HIGH-LIMIT REGISTER) |
| 1761 | `!defined(__IO_H__)` | `TMP101_THIGH_Register` | `3` | 依原始註解：Read/Write (LOW-LIMIT REGISTER)；註解：Read/Write (LOW-LIMIT REGISTER) |
| 1763 | `!defined(__IO_H__)` | `HS_MODE` | `BIT2` | 依原始註解：FS_MODE  for BIT2 set 0；註解：FS_MODE  for BIT2 set 0 |
| 1767 | `!defined(__IO_H__)` | `RTT_SCALE_33NS` | `0` | 依原始註解：///////////////////   RTT；註解：///////////////////   RTT |
| 1768 | `!defined(__IO_H__)` | `RTT_SCALE_100NS` | `1` | 通用常數/硬體欄位定義 |
| 1769 | `!defined(__IO_H__)` | `RTT_SCALE_1US` | `2` | 通用常數/硬體欄位定義 |
| 1770 | `!defined(__IO_H__)` | `RTT_SCALE_1MS` | `3` | 通用常數/硬體欄位定義 |
| 1772 | `!defined(__IO_H__)` | `SYS1L_RTT_CTRL` | `(0x600>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1773 | `!defined(__IO_H__)` | `SYS1B_RTT1_CTRL` | `(0x600)` | 系統暫存器/記憶體映射位址定義 |
| 1774 | `!defined(__IO_H__)` | `SYS1B_RTT2_CTRL` | `(0x601)` | 系統暫存器/記憶體映射位址定義 |
| 1775 | `!defined(__IO_H__)` | `SYS_RTTAURST` | `BIT6` | 系統暫存器/記憶體映射位址定義；註解：RTT auto reset               0:auto reset,   1:stop   	(0) |
| 1776 | `!defined(__IO_H__)` | `SYS_RTTEN` | `BIT7` | 系統暫存器/記憶體映射位址定義；註解：enable RTT timer             0:Disable,      1:Enable 	(0) |
| 1777 | `!defined(__IO_H__)` | `SYS_CNT_CLEAR` | `BIT4` | 系統暫存器/記憶體映射位址定義；註解：counter clear |
| 1778 | `!defined(__IO_H__)` | `SYS_RTT_SCALE` | `BIT0` | 系統暫存器/記憶體映射位址定義；註解：scale =0: 33.33ns ; 1=100ns; 2=1us; 3=1ms |
| 1779 | `!defined(__IO_H__)` | `SYS1L_COMB_CTRL` | `(0x604>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1780 | `!defined(__IO_H__)` | `SYS_RTT_COMBINE` | `BIT5` | 系統暫存器/記憶體映射位址定義；註解：1=  RTT1  RTT2 X@ RTT1, (bit ƱqӪ  32bit ܦ@ 64 bit  counter) |
| 1783 | `!defined(__IO_H__)` | `SYS1L_RT1_TO_LIMIT` | `(0x610>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1784 | `!defined(__IO_H__)` | `SYS1L_RT2_TO_LIMIT` | `(0x614>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1786 | `!defined(__IO_H__)` | `SYS1L_RT1_CNT` | `(0x620>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1787 | `!defined(__IO_H__)` | `SYS1L_RT2_CNT` | `(0x624>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1791 | `!defined(__IO_H__)` | `RTT_Set_Limit(rtt_sel, val)` | `(SYS1L[SYS1L_RTTSR( (rtt_sel) )] = (unsigned long)(val) )` | 通用常數/硬體欄位定義 |
| 1792 | `!defined(__IO_H__)` | `RTT_Get_Limit(rtt_sel)` | `(SYS1L[SYS1L_RTTSR( (rtt_sel) )])` | 通用常數/硬體欄位定義 |
| 1793 | `!defined(__IO_H__)` | `SYS1L_RTTSR(N)` | `(SYS1L_RT1_TO_LIMIT+(N-1))` | 系統暫存器/記憶體映射位址定義 |
| 1795 | `!defined(__IO_H__)` | `RTT_Set_Value(rtt_sel, val)` | `(SYS1L[SYS1L_RTTCNT( (rtt_sel) )] = (unsigned long)(val) )` | 通用常數/硬體欄位定義 |
| 1796 | `!defined(__IO_H__)` | `RTT_Get_Value(rtt_sel)` | `(SYS1L[SYS1L_RTTCNT( (rtt_sel) )])` | 通用常數/硬體欄位定義 |
| 1797 | `!defined(__IO_H__)` | `SYS1L_RTTCNT(N)` | `(SYS1L_RT1_CNT+(N-1))` | 系統暫存器/記憶體映射位址定義 |
| 1800 | `!defined(__IO_H__) && (!RDT)` | `RTT1_Stop()` | `(SYS1L[SYS1L_RTT_CTRL] = (SYS1L[SYS1L_RTT_CTRL] & (~SYS_RTTEN)) \| SYS_CNT_CLEAR)` | 通用常數/硬體欄位定義 |
| 1801 | `!defined(__IO_H__) && (!RDT)` | `RTT1_Start()` | `(SYS1L[SYS1L_RTT_CTRL] \|= SYS_RTTEN)` | 通用常數/硬體欄位定義 |
| 1804 | `!defined(__IO_H__)` | `SYS1L_WDT_CTRL` | `(0x700>>2)` | 系統暫存器/記憶體映射位址定義；註解：///////////////////WDT |
| 1805 | `!defined(__IO_H__)` | `WDT_KEEP_DATA(val)` | `(SYS1L[0x702] = (unsigned char)(val) )` | 通用常數/硬體欄位定義 |
| 1806 | `!defined(__IO_H__)` | `WDT_TIMEOUT_FLAG` | `BIT14` | 通用常數/硬體欄位定義 |
| 1807 | `!defined(__IO_H__)` | `WDT_ENABLE` | `BIT7` | 通用常數/硬體欄位定義 |
| 1808 | `!defined(__IO_H__)` | `WDT_RESET_CPU_ONLY` | `BIT2` | 通用常數/硬體欄位定義 |
| 1809 | `!defined(__IO_H__)` | `WDT_RESET_WHOLE_CHIP_BUT_SATA` | `BIT1` | 通用常數/硬體欄位定義 |
| 1810 | `!defined(__IO_H__)` | `WDT_RESET_WHOLE_CHIP` | `BIT0` | 通用常數/硬體欄位定義 |
| 1812 | `!defined(__IO_H__)` | `SYS1L_WDT_TO_LMT` | `(0x710>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1813 | `!defined(__IO_H__)` | `WDT_Set_Limit(val)` | `(SYS1L[SYS1L_WDT_TO_LMT] = (unsigned long)(val) )` | 通用常數/硬體欄位定義 |
| 1814 | `!defined(__IO_H__)` | `WDT_Get_Limit` | `(SYS1L[SYS1L_WDT_TO_LMT])` | 通用常數/硬體欄位定義 |
| 1816 | `!defined(__IO_H__)` | `SYS1L_WDT_CNT` | `(0x720>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1817 | `!defined(__IO_H__)` | `WDT_Get_Value` | `(SYS1L[SYS1L_WDT_CNT])` | 通用常數/硬體欄位定義 |
| 1821 | `!defined(__IO_H__)` | `SYS1L_DZ1_DSA` | `(0x1000>>2)` | 系統暫存器/記憶體映射位址定義；註解：////////////////////MISCH |
| 1822 | `!defined(__IO_H__)` | `SYS1L_DZ1_DLEN` | `(0x1004>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1823 | `!defined(__IO_H__)` | `SYS1L_DZ2_DSA` | `(0x1008>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1824 | `!defined(__IO_H__)` | `SYS1L_DZ2_DLEN` | `(0x100c>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1825 | `!defined(__IO_H__)` | `SYS1L_DZ3_DSA` | `(0x1010 >>2)` | 系統暫存器/記憶體映射位址定義 |
| 1826 | `!defined(__IO_H__)` | `SYS1L_DZ3_DLEN` | `(0x1014>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1827 | `!defined(__IO_H__)` | `SYS1L_SZ1_ADDRESS` | `(0x1018>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1828 | `!defined(__IO_H__)` | `SYS1L_SZ2_ADDRESS` | `(0x1020>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1830 | `!defined(__IO_H__)` | `SYS1L_DBUF_PMU_EN` | `(0x102C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1831 | `!defined(__IO_H__)` | `CR_DBUF_PMU_EN` | `BIT7` | 通用常數/硬體欄位定義 |
| 1833 | `!defined(__IO_H__)` | `SYS1L_ECC_EN` | `(0x1030>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1834 | `!defined(__IO_H__)` | `CR_SZ_EN` | `BIT23` | 通用常數/硬體欄位定義 |
| 1835 | `!defined(__IO_H__)` | `CR_Z1_ECC_EN` | `BIT15` | 通用常數/硬體欄位定義 |
| 1836 | `!defined(__IO_H__)` | `CR_Z2_ECC_EN` | `BIT14` | 通用常數/硬體欄位定義 |
| 1837 | `!defined(__IO_H__)` | `CR_Z3_ECC_EN` | `BIT13` | 通用常數/硬體欄位定義 |
| 1838 | `!defined(__IO_H__)` | `CR_E3D_EN` | `BIT7` | 通用常數/硬體欄位定義 |
| 1840 | `!defined(__IO_H__)` | `SYS1L_MODE_EN` | `(0x1038>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1841 | `!defined(__IO_H__)` | `MD_VDT_TEST` | `BIT23` | 通用常數/硬體欄位定義 |
| 1842 | `!defined(__IO_H__)` | `MD_IO_TEST` | `BIT22` | 通用常數/硬體欄位定義 |
| 1843 | `!defined(__IO_H__)` | `MD_MEM_TEST` | `BIT21` | 通用常數/硬體欄位定義 |
| 1844 | `!defined(__IO_H__)` | `MD_DEV_TRI` | `BIT15` | 通用常數/硬體欄位定義 |
| 1845 | `!defined(__IO_H__)` | `MD_SPD_TEST` | `BIT14` | 通用常數/硬體欄位定義 |
| 1846 | `!defined(__IO_H__)` | `MD_EFU_TEST` | `BIT13` | 通用常數/硬體欄位定義 |
| 1847 | `!defined(__IO_H__)` | `MD_PHY_TEST` | `BIT12` | 通用常數/硬體欄位定義 |
| 1848 | `!defined(__IO_H__)` | `MD_DDR_TEST` | `BIT11` | 通用常數/硬體欄位定義 |
| 1849 | `!defined(__IO_H__)` | `MD_DLL_TEST` | `BIT10` | 通用常數/硬體欄位定義 |
| 1850 | `!defined(__IO_H__)` | `MD_PLL_TEST` | `BIT9` | 通用常數/硬體欄位定義 |
| 1851 | `!defined(__IO_H__)` | `MD_PTM_TEST` | `BIT8` | 通用常數/硬體欄位定義 |
| 1852 | `!defined(__IO_H__)` | `MD_NOR_EN` | `BIT7` | 通用常數/硬體欄位定義 |
| 1853 | `!defined(__IO_H__)` | `MD_EA1_EN` | `BIT6` | 通用常數/硬體欄位定義 |
| 1854 | `!defined(__IO_H__)` | `MD_EA2_EN` | `BIT5` | 通用常數/硬體欄位定義 |
| 1855 | `!defined(__IO_H__)` | `ME_EA3_EN` | `BIT4` | 通用常數/硬體欄位定義 |
| 1856 | `!defined(__IO_H__)` | `ME_EA4_EN` | `BIT3` | 通用常數/硬體欄位定義 |
| 1858 | `!defined(__IO_H__)` | `SYS1L_CPU_MODE` | `(0x103C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1859 | `!defined(__IO_H__)` | `SR_CPU_PWAITMODE` | `BIT7` | 通用常數/硬體欄位定義 |
| 1860 | `!defined(__IO_H__)` | `SR_CPU_XOCDMODE` | `BIT6` | 通用常數/硬體欄位定義 |
| 1861 | `!defined(__IO_H__)` | `SYS1B_SR_HW_VER` | `0x103E` | 系統暫存器/記憶體映射位址定義 |
| 1862 | `!defined(__IO_H__)` | `SYS1B_SR_HWMD` | `0x103B` | 系統暫存器/記憶體映射位址定義 |
| 1863 | `!defined(__IO_H__)` | `GET_SR_HWMD` | `(SYS1L[SYS1B_SR_HWMD] & 0xF8)` | 通用常數/硬體欄位定義 |
| 1865 | `!defined(__IO_H__)` | `SYS1L_PD1_RAM_RME` | `(0x1040>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1866 | `!defined(__IO_H__)` | `SYS1L_PD2_RAM_RME0` | `(0x1044>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1867 | `!defined(__IO_H__)` | `SYS1L_PD2_RAM_RME1` | `(0x1048>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1868 | `!defined(__IO_H__)` | `SYS1L_PD1_RAM_RM0` | `(0x1060>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1869 | `!defined(__IO_H__)` | `SYS1L_PD1_RAM_RM1` | `(0x1064>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1870 | `!defined(__IO_H__)` | `SYS1L_PD2_RAM_RM0` | `(0x1068>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1871 | `!defined(__IO_H__)` | `SYS1L_PD2_RAM_RM1` | `(0x106C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1872 | `!defined(__IO_H__)` | `SYS1L_PD2_RAM_RM2` | `(0x1070>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1873 | `!defined(__IO_H__)` | `SYS1L_PD2_RAM_RM3` | `(0x1074>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1878 | `!defined(__IO_H__)` | `SYS1L_INT_STS0` | `(0x1110>>2)` | 系統暫存器/記憶體映射位址定義；註解：//////////////////// Interrupt |
| 1879 | `!defined(__IO_H__)` | `SYS1L_INT_EN0` | `(0x1140>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1880 | `!defined(__IO_H__)` | `DDR_INT` | `BIT22` | 通用常數/硬體欄位定義 |
| 1881 | `!defined(__IO_H__)` | `RS_INT` | `BIT9` | 通用常數/硬體欄位定義 |
| 1882 | `!defined(__IO_H__)` | `I2C_INT` | `BIT7` | 通用常數/硬體欄位定義 |
| 1883 | `!defined(__IO_H__)` | `SATA_INT` | `BIT5` | 通用常數/硬體欄位定義 |
| 1884 | `!defined(__IO_H__)` | `UART_INT` | `BIT3` | 周邊控制暫存器/位元欄位定義 |
| 1887 | `!defined(__IO_H__)` | `SYS1L_INT_STS1` | `(0x1118>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1888 | `!defined(__IO_H__)` | `SYS1L_INT_EN1` | `(0x1148>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1889 | `!defined(__IO_H__)` | `STACK_MONITOR_0` | `BIT24` | 通用常數/硬體欄位定義 |
| 1890 | `!defined(__IO_H__)` | `STACK_MONITOR_1` | `BIT25` | 通用常數/硬體欄位定義 |
| 1891 | `!defined(__IO_H__)` | `STACK_MONITOR_2` | `BIT26` | 通用常數/硬體欄位定義 |
| 1892 | `!defined(__IO_H__)` | `STACK_MONITOR_3` | `BIT27` | 通用常數/硬體欄位定義 |
| 1893 | `!defined(__IO_H__)` | `WDT_INT` | `BIT16` | 通用常數/硬體欄位定義 |
| 1894 | `!defined(__IO_H__)` | `RTT1_INT_1` | `BIT8` | 通用常數/硬體欄位定義 |
| 1895 | `!defined(__IO_H__)` | `RTT1_INT_2` | `BIT9` | 通用常數/硬體欄位定義 |
| 1896 | `!defined(__IO_H__)` | `RTT0_INT` | `BIT0` | 通用常數/硬體欄位定義 |
| 1898 | `!defined(__IO_H__)` | `SYS1L_INT_STS2` | `(0x111C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1899 | `!defined(__IO_H__)` | `SYS1L_INT_EN2` | `(0x114C>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1902 | `!defined(__IO_H__)` | `SYS1L_INT_STS3` | `(0x1120>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1903 | `!defined(__IO_H__)` | `SYS1L_INT_EN3` | `(0x1150>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1904 | `!defined(__IO_H__)` | `FLH_PE_INT_BPTRAM` | `BIT16` | 通用常數/硬體欄位定義 |
| 1905 | `!defined(__IO_H__)` | `FLH_PE_INT_NMBUF` | `BIT17` | 通用常數/硬體欄位定義 |
| 1906 | `!defined(__IO_H__)` | `FLH_PE_INT_PBUF0` | `BIT18` | 通用常數/硬體欄位定義 |
| 1907 | `!defined(__IO_H__)` | `FLH_PE_INT_PBUF1` | `BIT19` | 通用常數/硬體欄位定義 |
| 1908 | `!defined(__IO_H__)` | `FLH_PE_INT_PBUF2` | `BIT20` | 通用常數/硬體欄位定義 |
| 1909 | `!defined(__IO_H__)` | `FLH_PE_INT_PBUF3` | `BIT21` | 通用常數/硬體欄位定義 |
| 1910 | `!defined(__IO_H__)` | `FLH_PE_INT_IRAM` | `BIT22` | 通用常數/硬體欄位定義 |
| 1911 | `!defined(__IO_H__)` | `FLH_PE_INT_TRAM` | `BIT23` | 通用常數/硬體欄位定義 |
| 1912 | `!defined(__IO_H__)` | `FLH_PE_INT_BRAM_1` | `BIT24` | 通用常數/硬體欄位定義 |
| 1913 | `!defined(__IO_H__)` | `FLH_PE_INT_BRAM_2` | `BIT25` | 通用常數/硬體欄位定義 |
| 1914 | `!defined(__IO_H__)` | `FLH_PE_INT_IBF_1` | `BIT26` | 通用常數/硬體欄位定義 |
| 1915 | `!defined(__IO_H__)` | `FLH_PE_INT_IBF_2` | `BIT27` | 通用常數/硬體欄位定義 |
| 1916 | `!defined(__IO_H__)` | `FLH_PE_INT_ENCSRAM_1` | `BIT28` | 通用常數/硬體欄位定義 |
| 1917 | `!defined(__IO_H__)` | `FLH_PE_INT_ENCSRAM_2` | `BIT29` | 通用常數/硬體欄位定義 |
| 1918 | `!defined(__IO_H__)` | `FLH_PE_INT_ZIP` | `BIT30` | 通用常數/硬體欄位定義 |
| 1920 | `!defined(__IO_H__)` | `DBUF3_PE_INT` | `BIT13` | 通用常數/硬體欄位定義 |
| 1921 | `!defined(__IO_H__)` | `DBUF2_PE_INT` | `BIT12` | 通用常數/硬體欄位定義 |
| 1922 | `!defined(__IO_H__)` | `DBUF1_PE_INT` | `BIT11` | 通用常數/硬體欄位定義 |
| 1923 | `!defined(__IO_H__)` | `DBUF0_PE_INT` | `BIT10` | 通用常數/硬體欄位定義 |
| 1924 | `!defined(__IO_H__)` | `DDR_PE_INT` | `BIT8` | 通用常數/硬體欄位定義 |
| 1925 | `!defined(__IO_H__)` | `CPU_PE_INT` | `BIT7` | 通用常數/硬體欄位定義 |
| 1926 | `!defined(__IO_H__)` | `SATA_PE_INT` | `BIT6` | 通用常數/硬體欄位定義 |
| 1927 | `!defined(__IO_H__)` | `DMAC_PE_INT` | `BIT5` | 周邊控制暫存器/位元欄位定義 |
| 1928 | `!defined(__IO_H__)` | `RRAM_PE_INT` | `BIT0` | 通用常數/硬體欄位定義 |
| 1932 | `!defined(__IO_H__)` | `SYS1L_INT_STS4` | `(0x1124>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1933 | `!defined(__IO_H__)` | `SYS1L_INT_EN4` | `(0x1154>>2)` | 系統暫存器/記憶體映射位址定義 |
| 1934 | `!defined(__IO_H__)` | `TS_INT_TCODE_RISING_EDGE` | `BIT28` | 通用常數/硬體欄位定義 |
| 1935 | `!defined(__IO_H__)` | `TS_INT_TCODE_FALLING_EDGE` | `BIT29` | 通用常數/硬體欄位定義 |
| 1936 | `!defined(__IO_H__)` | `TS_INT_TEMP_RISING_EDGE` | `BIT30` | 通用常數/硬體欄位定義 |
| 1937 | `!defined(__IO_H__)` | `TS_INT_TEMP_FALLING_EDGE` | `BIT31` | 通用常數/硬體欄位定義 |
| 1939 | `!defined(__IO_H__)` | `VDT_INT_FIO_RISING_EDGE` | `BIT16` | 通用常數/硬體欄位定義 |
| 1940 | `!defined(__IO_H__)` | `VDT_INT_FIO_FALLING_EDGE` | `BIT17` | 通用常數/硬體欄位定義 |
| 1941 | `!defined(__IO_H__)` | `VDT_INT_IO0_RISING_EDGE` | `BIT18` | 通用常數/硬體欄位定義 |
| 1942 | `!defined(__IO_H__)` | `VDT_INT_IO0_FALLING_EDGE` | `BIT19` | 通用常數/硬體欄位定義 |
| 1943 | `!defined(__IO_H__)` | `VDT_INT_PD1_RISING_EDGE` | `BIT20` | 通用常數/硬體欄位定義 |
| 1944 | `!defined(__IO_H__)` | `VDT_INT_PD1_FALLING_EDGE` | `BIT21` | 通用常數/硬體欄位定義 |
| 1945 | `!defined(__IO_H__)` | `VDT_INT_PD2_RISING_EDGE` | `BIT22` | 通用常數/硬體欄位定義 |
| 1946 | `!defined(__IO_H__)` | `VDT_INT_PD2_FALLING_EDGE` | `BIT23` | 通用常數/硬體欄位定義 |
| 1947 | `!defined(__IO_H__)` | `VDT_FLH_PD2_RISING_EDGE` | `BIT24` | 通用常數/硬體欄位定義 |
| 1948 | `!defined(__IO_H__)` | `VDT_FLH_PD2_FALLING_EDGE` | `BIT25` | 通用常數/硬體欄位定義 |
| 1950 | `!defined(__IO_H__)` | `GPIO_INT_SEL0_RISING_EDGE` | `BIT8` | 周邊控制暫存器/位元欄位定義 |
| 1951 | `!defined(__IO_H__)` | `GPIO_INT_SEL0_FALLING_EDGE` | `BIT9` | 周邊控制暫存器/位元欄位定義 |
| 1952 | `!defined(__IO_H__)` | `GPIO_INT_SEL1_RISING_EDGE` | `BIT10` | 周邊控制暫存器/位元欄位定義 |
| 1953 | `!defined(__IO_H__)` | `GPIO_INT_SEL1_FALLING_EDGE` | `BIT11` | 周邊控制暫存器/位元欄位定義 |
| 1954 | `!defined(__IO_H__)` | `GPIO_INT_SEL2_RISING_EDGE` | `BIT12` | 周邊控制暫存器/位元欄位定義 |
| 1955 | `!defined(__IO_H__)` | `GPIO_INT_SEL2_FALLING_EDGE` | `BIT13` | 周邊控制暫存器/位元欄位定義 |
| 1956 | `!defined(__IO_H__)` | `GPIO_INT_SEL3_RISING_EDGE` | `BIT14` | 周邊控制暫存器/位元欄位定義 |
| 1957 | `!defined(__IO_H__)` | `GPIO_INT_SEL3_FALLING_EDGE` | `BIT15` | 周邊控制暫存器/位元欄位定義 |
| 1959 | `!defined(__IO_H__)` | `PMU_INT_DEVSLP_FALLING_EDGE` | `BIT4` | 通用常數/硬體欄位定義 |
| 1960 | `!defined(__IO_H__)` | `PMU_INT_DEVSLP_RISING_EDGE` | `BIT5` | 通用常數/硬體欄位定義 |
| 1961 | `!defined(__IO_H__)` | `PMU_INT_PD0GPIO_FALLING_EDGE` | `BIT6` | 周邊控制暫存器/位元欄位定義 |
| 1962 | `!defined(__IO_H__)` | `PMU_INT_PD0GPIO_RISING_EDGE` | `BIT7` | 周邊控制暫存器/位元欄位定義 |
| 1964 | `!defined(__IO_H__)` | `JTAG_INT_RISING_EDGE` | `BIT3` | 通用常數/硬體欄位定義 |
| 1966 | `!defined(__IO_H__)` | `PLL0_INT_RISING_EDGE` | `BIT0` | 通用常數/硬體欄位定義 |
| 1967 | `!defined(__IO_H__)` | `PLL1_INT_RISING_EDGE` | `BIT1` | 通用常數/硬體欄位定義 |
| 1968 | `!defined(__IO_H__)` | `PLL_SATA_PHY_INT_RISING_EDGE` | `BIT2` | 通用常數/硬體欄位定義 |
| 1971 | `!defined(__IO_H__)` | `UART_Reg_STARTADDRESS` | `0x04003200` | 周邊控制暫存器/位元欄位定義；註解：///////////////// Uart |
| 1972 | `!defined(__IO_H__)` | `UARTO` | `((volatile unsigned long long *)UART_Reg_STARTADDRESS)` | 周邊控制暫存器/位元欄位定義 |
| 1973 | `!defined(__IO_H__)` | `UARTW` | `((volatile unsigned short *)UART_Reg_STARTADDRESS)` | 周邊控制暫存器/位元欄位定義 |
| 1974 | `!defined(__IO_H__)` | `UARTB` | `((volatile unsigned char *)UART_Reg_STARTADDRESS)` | 周邊控制暫存器/位元欄位定義 |
| 1975 | `!defined(__IO_H__)` | `UARTL` | `((volatile unsigned long *)UART_Reg_STARTADDRESS)` | 周邊控制暫存器/位元欄位定義 |
| 1977 | `!defined(__IO_H__)` | `UARTL_UART_CTRL` | `(0x00>>2)` | 周邊控制暫存器/位元欄位定義 |
| 1978 | `!defined(__IO_H__)` | `CLR_BAUD_RATE` | `0xFF000000` | 通用常數/硬體欄位定義 |
| 1979 | `!defined(__IO_H__)` | `BAUD_RATE(x)` | `((unsigned long )x&0x00FFFFFF)` | 通用常數/硬體欄位定義 |
| 1980 | `!defined(__IO_H__)` | `OOB_EN` | `BIT24` | 通用常數/硬體欄位定義 |
| 1981 | `!defined(__IO_H__)` | `RX_RST` | `BIT31` | 通用常數/硬體欄位定義 |
| 1982 | `!defined(__IO_H__)` | `UARTL_UART_TRIG` | `((0x04)>>2)` | 周邊控制暫存器/位元欄位定義 |
| 1983 | `!defined(__IO_H__)` | `OOB_RX_BUF_NOT_EMPTY` | `BIT8` | 通用常數/硬體欄位定義 |
| 1984 | `!defined(__IO_H__)` | `TX_TRIG` | `BIT0` | 通用常數/硬體欄位定義 |
| 1985 | `!defined(__IO_H__)` | `UARTL_TX_PKT_CFG` | `((0x0C)>>2)` | 周邊控制暫存器/位元欄位定義 |
| 1986 | `!defined(__IO_H__)` | `TX_PKT_TYP(x)` | `((unsigned char )x&0xFF)` | 通用常數/硬體欄位定義 |
| 1987 | `!defined(__IO_H__)` | `TX_PAYLOAD_LEN(x)` | `(((unsigned short )x<<8)&0xFF00)` | 通用常數/硬體欄位定義 |
| 1988 | `!defined(__IO_H__)` | `UARTB_TX_PAYLOAD_LEN` | `((0x0D)>>0)` | 周邊控制暫存器/位元欄位定義 |
| 1989 | `!defined(__IO_H__)` | `UARTB_TX_PAYLOAD` | `(0x10)` | 周邊控制暫存器/位元欄位定義 |
| 1990 | `!defined(__IO_H__)` | `UARTLL_TX_PAYLOAD` | `(0x10 >> 3)` | 周邊控制暫存器/位元欄位定義 |
| 1991 | `!defined(__IO_H__)` | `UARTL_OOB_UR_INT_EN` | `((0x50)>>2)` | 周邊控制暫存器/位元欄位定義 |
| 1993 | `!defined(__IO_H__)` | `UARTL_OOB_Receive_Packet_INFO` | `(0x54 >> 2)` | 周邊控制暫存器/位元欄位定義；註解：#define TX_CMPLT_INT_EN           0x800 |
| 1994 | `!defined(__IO_H__)` | `UARTB_OOB_RX_Payload` | `(0x60 >> 0)` | 周邊控制暫存器/位元欄位定義 |
| 1995 | `!defined(__IO_H__)` | `UARTLL_OOB_RX_Payload` | `(0x60 >> 3)` | 周邊控制暫存器/位元欄位定義 |
| 1996 | `!defined(__IO_H__)` | `UARTL_UR_INT_STATUS` | `(0x70 >> 2)` | 周邊控制暫存器/位元欄位定義 |
| 1997 | `!defined(__IO_H__)` | `UARTL_RX_FIFO_DATA` | `((0x58)>>2)` | 周邊控制暫存器/位元欄位定義 |
| 1998 | `!defined(__IO_H__)` | `RX_FIFO_EMPTY` | `BIT8` | 通用常數/硬體欄位定義 |
| 1999 | `!defined(__IO_H__)` | `UARTB_SERIAL_DATA` | `(0x74)` | 周邊控制暫存器/位元欄位定義 |
| 2000 | `!defined(__IO_H__)` | `UARTL_S_FIFO_CTLST` | `(0x78>>2)` | 周邊控制暫存器/位元欄位定義 |
| 2001 | `!defined(__IO_H__)` | `TX_SERIAL_FIFO_EN` | `BIT8` | 通用常數/硬體欄位定義 |
| 2002 | `!defined(__IO_H__)` | `S_FIFO_EMPTY` | `BIT1` | 通用常數/硬體欄位定義 |
| 2003 | `!defined(__IO_H__)` | `S_FIFO_FULL` | `BIT0` | 通用常數/硬體欄位定義 |
| 2006 | `!defined(__IO_H__)` | `PKT_CRC_ERR_INT_EN` | `BIT0` | 通用常數/硬體欄位定義 |
| 2007 | `!defined(__IO_H__)` | `PKT_LEN_ERR_INT_EN` | `BIT1` | 通用常數/硬體欄位定義 |
| 2008 | `!defined(__IO_H__)` | `PKT_FRAMING_ERR_INT_EN` | `BIT2` | 通用常數/硬體欄位定義 |
| 2009 | `!defined(__IO_H__)` | `PKT_TO_ERR_INT_EN` | `BIT3` | 通用常數/硬體欄位定義 |
| 2010 | `!defined(__IO_H__)` | `PKT_START_ERR_INT_EN` | `BIT4` | 通用常數/硬體欄位定義 |
| 2011 | `!defined(__IO_H__)` | `PKT_RX_INT_EN` | `BIT5` | 通用常數/硬體欄位定義 |
| 2012 | `!defined(__IO_H__)` | `PKT_TX_CMPLT_INT_EN` | `BIT6` | 通用常數/硬體欄位定義 |
| 2013 | `!defined(__IO_H__)` | `RX_FRAME_ERR_INT_EN` | `BIT8` | 通用常數/硬體欄位定義 |
| 2014 | `!defined(__IO_H__)` | `RX_OVFLW_INT_EN` | `BIT9` | 通用常數/硬體欄位定義 |
| 2015 | `!defined(__IO_H__)` | `RX_NOT_EMPTY_INT_EN` | `BIT10` | 通用常數/硬體欄位定義 |
| 2016 | `!defined(__IO_H__)` | `TX_CMPLT_INT_EN` | `BIT11` | 通用常數/硬體欄位定義 |
| 2018 | `!defined(__IO_H__)` | `UART_OOB_RECEIVE_MAX_LEN` | `18` | 周邊控制暫存器/位元欄位定義 |
| 2019 | `!defined(__IO_H__)` | `UART_OOB_SET_DATA` | `0x81` | 周邊控制暫存器/位元欄位定義 |
| 2020 | `!defined(__IO_H__)` | `UART_OOB_QUERY_DATA` | `0x82` | 周邊控制暫存器/位元欄位定義 |
| 2021 | `!defined(__IO_H__)` | `UART_OOB_SEND_LOOPBACK_DATA` | `0x87` | 周邊控制暫存器/位元欄位定義 |
| 2028 | `!defined(__IO_H__)` | `SPIB` | `((volatile unsigned char *)0x04004000)` | 依原始註解：=================================================== \| PS3111 SPI Register Define；註解：=================================================== \| PS3111 SPI Register Define \| =================================================== |
| 2029 | `!defined(__IO_H__)` | `SPIW` | `((volatile unsigned short int *)0x04004000)` | 通用常數/硬體欄位定義 |
| 2030 | `!defined(__IO_H__)` | `SPIL` | `((volatile unsigned int *)0x04004000)` | 通用常數/硬體欄位定義 |
| 2031 | `!defined(__IO_H__)` | `SPIO` | `((volatile unsigned long long *)0x04004000)` | 通用常數/硬體欄位定義 |
| 2034 | `!defined(__IO_H__)` | `SPIL_CFG` | `(0x00>>2)` | 通用常數/硬體欄位定義 |
| 2035 | `!defined(__IO_H__)` | `Dummy_Byte_Enable` | `BIT8` | 通用常數/硬體欄位定義 |
| 2036 | `!defined(__IO_H__)` | `SPI_BUF_MODE` | `BIT7` | 通用常數/硬體欄位定義 |
| 2037 | `!defined(__IO_H__)` | `SPI_CS_N_output_value` | `BIT6` | 通用常數/硬體欄位定義 |
| 2038 | `!defined(__IO_H__)` | `SPI_CS_N_Switch` | `BIT5` | 依原始註解：0: Control by IP   1: Control by FW  default(0)；註解：0: Control by IP   1: Control by FW  default(0) |
| 2039 | `!defined(__IO_H__)` | `DMA_read_write_direction` | `BIT0` | 周邊控制暫存器/位元欄位定義；註解：0: spi read   1: spi write  default(0) |
| 2042 | `!defined(__IO_H__)` | `SPIL_START_ADD` | `(0x04>>2)` | 通用常數/硬體欄位定義 |
| 2043 | `!defined(__IO_H__)` | `SPI_Start_Address_Register_SET(X)` | `(SPIL[SPIL_START_ADD] = (X & 0x00FFFFFF))` | 通用常數/硬體欄位定義 |
| 2046 | `!defined(__IO_H__)` | `SPIL_DMA_BCNT` | `(0x08>>2)` | 周邊控制暫存器/位元欄位定義 |
| 2048 | `!defined(__IO_H__)` | `SPI_DMA_Byte_Counts_SET(X)` | `(SPIL[SPIL_DMA_BCNT] = (X & 0x00FFFFFF))` | 周邊控制暫存器/位元欄位定義；註解：Clock Cycle Count |
| 2051 | `!defined(__IO_H__)` | `SPIL_PAGE_SIZE` | `(0x0C>>2)` | 通用常數/硬體欄位定義 |
| 2052 | `!defined(__IO_H__)` | `SPI_Page_Size_SET(X)` | `(SPIL[SPIL_PAGE_SIZE] = (X & 0x00000FFF))` | 通用常數/硬體欄位定義 |
| 2055 | `!defined(__IO_H__)` | `SPIL_DMA_RAM_ADDR` | `(0x10>>2)` | 周邊控制暫存器/位元欄位定義 |
| 2056 | `!defined(__IO_H__)` | `SPI_DMA_RAM_Address_SET(X)` | `(SPIL[SPIL_DMA_RAM_ADDR] = (X & 0x00000FFF))` | 周邊控制暫存器/位元欄位定義 |
| 2059 | `!defined(__IO_H__)` | `SPIL_CMD_LEN` | `(0x14>>2)` | 通用常數/硬體欄位定義 |
| 2060 | `!defined(__IO_H__)` | `SPI_Start_Address_Length_SET(X)` | `(SPIL[SPIL_CMD_LEN] \|= ((X << 16) & 0x000F0000))` | 通用常數/硬體欄位定義 |
| 2061 | `!defined(__IO_H__)` | `SPI_Command_4_Length_SET(X)` | `(SPIL[SPIL_CMD_LEN] \|= ((X << 12) & 0x0000F000))` | 通用常數/硬體欄位定義 |
| 2062 | `!defined(__IO_H__)` | `SPI_Command_3_Length_SET(X)` | `(SPIL[SPIL_CMD_LEN] \|= ((X << 8) & 0x00000F00))` | 通用常數/硬體欄位定義 |
| 2063 | `!defined(__IO_H__)` | `SPI_Command_2_Length_SET(X)` | `(SPIL[SPIL_CMD_LEN] \|= ((X << 4) & 0x000000F0))` | 通用常數/硬體欄位定義 |
| 2064 | `!defined(__IO_H__)` | `SPI_Command_1_Length_SET(X)` | `(SPIL[SPIL_CMD_LEN] \|= ((X << 0) & 0x0000000F))` | 通用常數/硬體欄位定義 |
| 2067 | `!defined(__IO_H__)` | `SPIL_CMD_1` | `(0x18>>2)` | 通用常數/硬體欄位定義 |
| 2068 | `!defined(__IO_H__)` | `SPIL_CMD_2` | `(0x1C>>2)` | 通用常數/硬體欄位定義 |
| 2069 | `!defined(__IO_H__)` | `SPIL_CMD_3` | `(0x20>>2)` | 通用常數/硬體欄位定義 |
| 2070 | `!defined(__IO_H__)` | `SPIL_CMD_4` | `(0x24>>2)` | 通用常數/硬體欄位定義 |
| 2071 | `!defined(__IO_H__)` | `SPIL_CMD_8765` | `(0x28>>2)` | 通用常數/硬體欄位定義 |
| 2072 | `!defined(__IO_H__)` | `SPI_Command_8_SET(X)` | `(SPIL[SPIL_CMD_8765] \|= ((X << 24) & 0xFF000000))` | 通用常數/硬體欄位定義 |
| 2073 | `!defined(__IO_H__)` | `SPI_Command_7_SET(X)` | `(SPIL[SPIL_CMD_8765] \|= ((X << 16) & 0x00FF0000))` | 通用常數/硬體欄位定義 |
| 2074 | `!defined(__IO_H__)` | `SPI_Command_6_SET(X)` | `(SPIL[SPIL_CMD_8765] \|= ((X << 8) & 0x0000FF00))` | 通用常數/硬體欄位定義 |
| 2075 | `!defined(__IO_H__)` | `SPI_Command_5_SET(X)` | `(SPIL[SPIL_CMD_8765] \|= ((X << 0) & 0x000000FF))` | 通用常數/硬體欄位定義 |
| 2078 | `!defined(__IO_H__)` | `SPIL_CMD_ORDER_L` | `(0x2C>>2)` | 通用常數/硬體欄位定義 |
| 2079 | `!defined(__IO_H__)` | `SPI_Command_Order_8_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_L] &= (0x0FFFFFFF))` | 通用常數/硬體欄位定義 |
| 2080 | `!defined(__IO_H__)` | `SPI_Command_Order_7_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_L] &= (0xF0FFFFFF))` | 通用常數/硬體欄位定義 |
| 2081 | `!defined(__IO_H__)` | `SPI_Command_Order_6_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_L] &= (0xFF0FFFFF))` | 通用常數/硬體欄位定義 |
| 2082 | `!defined(__IO_H__)` | `SPI_Command_Order_5_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_L] &= (0xFFF0FFFF))` | 通用常數/硬體欄位定義 |
| 2083 | `!defined(__IO_H__)` | `SPI_Command_Order_4_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_L] &= (0xFFFF0FFF))` | 通用常數/硬體欄位定義 |
| 2084 | `!defined(__IO_H__)` | `SPI_Command_Order_3_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_L] &= (0xFFFFF0FF))` | 通用常數/硬體欄位定義 |
| 2085 | `!defined(__IO_H__)` | `SPI_Command_Order_2_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_L] &= (0xFFFFFF0F))` | 通用常數/硬體欄位定義 |
| 2086 | `!defined(__IO_H__)` | `SPI_Command_Order_1_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_L] &= (0xFFFFFFF0))` | 通用常數/硬體欄位定義 |
| 2088 | `!defined(__IO_H__)` | `SPI_Command_Order_8_SET(X)` | `(SPIL[SPIL_CMD_ORDER_L] \|= ((X << 28) & 0xF0000000))` | 通用常數/硬體欄位定義 |
| 2089 | `!defined(__IO_H__)` | `SPI_Command_Order_7_SET(X)` | `(SPIL[SPIL_CMD_ORDER_L] \|= ((X << 24) & 0x0F000000))` | 通用常數/硬體欄位定義 |
| 2090 | `!defined(__IO_H__)` | `SPI_Command_Order_6_SET(X)` | `(SPIL[SPIL_CMD_ORDER_L] \|= ((X << 20) & 0x00F00000))` | 通用常數/硬體欄位定義 |
| 2091 | `!defined(__IO_H__)` | `SPI_Command_Order_5_SET(X)` | `(SPIL[SPIL_CMD_ORDER_L] \|= ((X << 16) & 0x000F0000))` | 通用常數/硬體欄位定義 |
| 2092 | `!defined(__IO_H__)` | `SPI_Command_Order_4_SET(X)` | `(SPIL[SPIL_CMD_ORDER_L] \|= ((X << 12) & 0x0000F000))` | 通用常數/硬體欄位定義 |
| 2093 | `!defined(__IO_H__)` | `SPI_Command_Order_3_SET(X)` | `(SPIL[SPIL_CMD_ORDER_L] \|= ((X << 8) & 0x00000F00))` | 通用常數/硬體欄位定義 |
| 2094 | `!defined(__IO_H__)` | `SPI_Command_Order_2_SET(X)` | `(SPIL[SPIL_CMD_ORDER_L] \|= ((X << 4) & 0x000000F0))` | 通用常數/硬體欄位定義 |
| 2095 | `!defined(__IO_H__)` | `SPI_Command_Order_1_SET(X)` | `(SPIL[SPIL_CMD_ORDER_L] \|= ((X << 0) & 0x0000000F))` | 通用常數/硬體欄位定義 |
| 2098 | `!defined(__IO_H__)` | `SPIL_CMD_ORDER_H` | `(0x30>>2)` | 通用常數/硬體欄位定義 |
| 2099 | `!defined(__IO_H__)` | `SPI_Command_Order_16_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_H] &= (0x0FFFFFFF))` | 通用常數/硬體欄位定義 |
| 2100 | `!defined(__IO_H__)` | `SPI_Command_Order_15_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_H] &= (0xF0FFFFFF))` | 通用常數/硬體欄位定義 |
| 2101 | `!defined(__IO_H__)` | `SPI_Command_Order_14_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_H] &= (0xFF0FFFFF))` | 通用常數/硬體欄位定義 |
| 2102 | `!defined(__IO_H__)` | `SPI_Command_Order_13_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_H] &= (0xFFF0FFFF))` | 通用常數/硬體欄位定義 |
| 2103 | `!defined(__IO_H__)` | `SPI_Command_Order_12_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_H] &= (0xFFFF0FFF))` | 通用常數/硬體欄位定義 |
| 2104 | `!defined(__IO_H__)` | `SPI_Command_Order_11_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_H] &= (0xFFFFF0FF))` | 通用常數/硬體欄位定義 |
| 2105 | `!defined(__IO_H__)` | `SPI_Command_Order_10_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_H] &= (0xFFFFFF0F))` | 通用常數/硬體欄位定義 |
| 2106 | `!defined(__IO_H__)` | `SPI_Command_Order_9_CLEAR()` | `(SPIL[SPIL_CMD_ORDER_H] &= (0xFFFFFFF0))` | 通用常數/硬體欄位定義 |
| 2108 | `!defined(__IO_H__)` | `SPI_Command_Order_16_SET(X)` | `(SPIL[SPIL_CMD_ORDER_H] \|= ((X << 28) & 0xF0000000))` | 通用常數/硬體欄位定義 |
| 2109 | `!defined(__IO_H__)` | `SPI_Command_Order_15_SET(X)` | `(SPIL[SPIL_CMD_ORDER_H] \|= ((X << 24) & 0x0F000000))` | 通用常數/硬體欄位定義 |
| 2110 | `!defined(__IO_H__)` | `SPI_Command_Order_14_SET(X)` | `(SPIL[SPIL_CMD_ORDER_H] \|= ((X << 20) & 0x00F00000))` | 通用常數/硬體欄位定義 |
| 2111 | `!defined(__IO_H__)` | `SPI_Command_Order_13_SET(X)` | `(SPIL[SPIL_CMD_ORDER_H] \|= ((X << 16) & 0x000F0000))` | 通用常數/硬體欄位定義 |
| 2112 | `!defined(__IO_H__)` | `SPI_Command_Order_12_SET(X)` | `(SPIL[SPIL_CMD_ORDER_H] \|= ((X << 12) & 0x0000F000))` | 通用常數/硬體欄位定義 |
| 2113 | `!defined(__IO_H__)` | `SPI_Command_Order_11_SET(X)` | `(SPIL[SPIL_CMD_ORDER_H] \|= ((X << 8) & 0x00000F00))` | 通用常數/硬體欄位定義 |
| 2114 | `!defined(__IO_H__)` | `SPI_Command_Order_10_SET(X)` | `(SPIL[SPIL_CMD_ORDER_H] \|= ((X << 4) & 0x000000F0))` | 通用常數/硬體欄位定義 |
| 2115 | `!defined(__IO_H__)` | `SPI_Command_Order_9_SET(X)` | `(SPIL[SPIL_CMD_ORDER_H] \|= ((X << 0) & 0x0000000F))` | 通用常數/硬體欄位定義 |
| 2117 | `!defined(__IO_H__)` | `SPIL_TRIG` | `(0x34>>2)` | 通用常數/硬體欄位定義 |
| 2118 | `!defined(__IO_H__)` | `Read_Data_location` | `BIT1` | 通用常數/硬體欄位定義 |
| 2119 | `!defined(__IO_H__)` | `SPI_DMA_trigger` | `BIT0` | 周邊控制暫存器/位元欄位定義 |
| 2121 | `!defined(__IO_H__)` | `SPIL_RDATA0` | `(0x38>>2)` | 通用常數/硬體欄位定義 |
| 2122 | `!defined(__IO_H__)` | `SPI_Read_Data_byte3_GET` | `(   (SPIL[SPIL_RDATA0] \|= 0xFF000000) >> 24  )` | 通用常數/硬體欄位定義 |
| 2123 | `!defined(__IO_H__)` | `SPI_Read_Data_byte2_GET` | `(   (SPIL[SPIL_RDATA0] \|= 0x00FF0000) >> 16  )` | 通用常數/硬體欄位定義 |
| 2124 | `!defined(__IO_H__)` | `SPI_Read_Data_byte1_GET` | `(   (SPIL[SPIL_RDATA0] \|= 0x0000FF00) >> 8   )` | 通用常數/硬體欄位定義 |
| 2125 | `!defined(__IO_H__)` | `SPI_Read_Data_byte0_GET` | `(   (SPIL[SPIL_RDATA0] \|= 0x000000FF)        )` | 通用常數/硬體欄位定義 |
| 2127 | `!defined(__IO_H__)` | `SPI_Read_Data_byte3_SET(X)` | `(SPIL[SPIL_RDATA0] \|= ((X << 24) & 0xFF000000))` | 通用常數/硬體欄位定義 |
| 2128 | `!defined(__IO_H__)` | `SPI_Read_Data_byte2_SET(X)` | `(SPIL[SPIL_RDATA0] \|= ((X << 16) & 0x00FF0000))` | 通用常數/硬體欄位定義 |
| 2129 | `!defined(__IO_H__)` | `SPI_Read_Data_byte1_SET(X)` | `(SPIL[SPIL_RDATA0] \|= ((X << 8) & 0x0000FF00))` | 通用常數/硬體欄位定義 |
| 2130 | `!defined(__IO_H__)` | `SPI_Read_Data_byte0_SET(X)` | `(SPIL[SPIL_RDATA0] \|= ((X << 0) & 0x000000FF))` | 通用常數/硬體欄位定義 |
| 2132 | `!defined(__IO_H__)` | `SPIL_RDATA1` | `(0x3C>>2)` | 通用常數/硬體欄位定義 |
| 2133 | `!defined(__IO_H__)` | `SPI_Read_Data_byte7_GET` | `(   (SPIL[SPIL_RDATA1] \|= 0xFF000000) >> 24  )` | 通用常數/硬體欄位定義 |
| 2134 | `!defined(__IO_H__)` | `SPI_Read_Data_byte6_GET` | `(   (SPIL[SPIL_RDATA1] \|= 0x00FF0000) >> 16  )` | 通用常數/硬體欄位定義 |
| 2135 | `!defined(__IO_H__)` | `SPI_Read_Data_byte5_GET` | `(   (SPIL[SPIL_RDATA1] \|= 0x0000FF00) >> 8   )` | 通用常數/硬體欄位定義 |
| 2136 | `!defined(__IO_H__)` | `SPI_Read_Data_byte4_GET` | `(   (SPIL[SPIL_RDATA1] \|= 0x000000FF)        )` | 通用常數/硬體欄位定義 |
| 2138 | `!defined(__IO_H__)` | `SPI_Read_Data_byte7_SET(X)` | `(SPIL[SPIL_RDATA1] \|= ((X << 24) & 0xFF000000))` | 通用常數/硬體欄位定義 |
| 2139 | `!defined(__IO_H__)` | `SPI_Read_Data_byte6_SET(X)` | `(SPIL[SPIL_RDATA1] \|= ((X << 16) & 0x00FF0000))` | 通用常數/硬體欄位定義 |
| 2140 | `!defined(__IO_H__)` | `SPI_Read_Data_byte5_SET(X)` | `(SPIL[SPIL_RDATA1] \|= ((X << 8) & 0x0000FF00))` | 通用常數/硬體欄位定義 |
| 2141 | `!defined(__IO_H__)` | `SPI_Read_Data_byte4_SET(X)` | `(SPIL[SPIL_RDATA1] \|= ((X << 0) & 0x000000FF))` | 通用常數/硬體欄位定義 |
| 2151 | `!defined(__IO_H__)` | `DDR_REG` | `0x04005000` | 系統暫存器/記憶體映射位址定義；註解：=================================================== \| PS3111 DDR Register Define \| =================================================== |
| 2153 | `!defined(__IO_H__)` | `RAMB` | `((volatile unsigned char*)DDR_REG)` | 通用常數/硬體欄位定義 |
| 2154 | `!defined(__IO_H__)` | `RAMW` | `((volatile unsigned short *)DDR_REG)` | 通用常數/硬體欄位定義 |
| 2155 | `!defined(__IO_H__)` | `RAML` | `((volatile unsigned long *)DDR_REG)` | 通用常數/硬體欄位定義 |
| 2156 | `!defined(__IO_H__)` | `RAMO` | `((volatile unsigned long long *)DDR_REG)` | 通用常數/硬體欄位定義 |
| 2159 | `!defined(__IO_H__)` | `RAML_DDR3_REF_DBG_CFG` | `(0x00>>2)` | 通用常數/硬體欄位定義 |
| 2160 | `!defined(__IO_H__)` | `RAMB_DB_DEL` | `(0x02)` | 通用常數/硬體欄位定義 |
| 2161 | `!defined(__IO_H__)` | `REN` | `BIT0` | 通用常數/硬體欄位定義 |
| 2163 | `!defined(__IO_H__)` | `RAML_DDR3_REF_CNT` | `(0x04>>2)` | 通用常數/硬體欄位定義 |
| 2164 | `!defined(__IO_H__)` | `CBRSEL` | `BIT28` | 通用常數/硬體欄位定義 |
| 2165 | `!defined(__IO_H__)` | `RAMB_CNT1` | `(0x06)` | 通用常數/硬體欄位定義 |
| 2166 | `!defined(__IO_H__)` | `RAMB_CNT0` | `(0x04)` | 通用常數/硬體欄位定義 |
| 2168 | `!defined(__IO_H__)` | `RAML_DDR3_ODT_MUX` | `(0x08>>2)` | 通用常數/硬體欄位定義 |
| 2169 | `!defined(__IO_H__)` | `RAMB_RK3_ODT` | `(0x09)` | 依原始註解：only _BIT 15:12；註解：only _BIT 15:12 |
| 2170 | `!defined(__IO_H__)` | `RAMB_RK2_ODT` | `(0x09)` | 依原始註解：only _BIT 11:8；註解：only _BIT 11:8 |
| 2171 | `!defined(__IO_H__)` | `RAMB_RK1_ODT` | `(0x08)` | 依原始註解：only _BIT 7:4；註解：only _BIT 7:4 |
| 2172 | `!defined(__IO_H__)` | `RAMB_RK0_ODT` | `(0x08)` | 依原始註解：only _BIT 3:0；註解：only _BIT 3:0 |
| 2174 | `!defined(__IO_H__)` | `RAML_DDR3_RANK_Timing_Parameter` | `(0x0C>>2)` | 通用常數/硬體欄位定義 |
| 2175 | `!defined(__IO_H__)` | `RAMB_R2W_DIFRK` | `(0x0D)` | 依原始註解：only _BIT 15:12；註解：only _BIT 15:12 |
| 2176 | `!defined(__IO_H__)` | `RAMW_TWTR_AL_DIFRK` | `(0x0C>>1)` | 依原始註解：only _BIT 8:4；註解：only _BIT 8:4 |
| 2177 | `!defined(__IO_H__)` | `RAMB_TCCD_DIFRK` | `(0x0C)` | 依原始註解：only _BIT 3:0；註解：only _BIT 3:0 |
| 2179 | `!defined(__IO_H__)` | `RAML_DDR3_MEM_INIT` | `(0x10>>2)` | 通用常數/硬體欄位定義 |
| 2180 | `!defined(__IO_H__)` | `RAMW_MRS_ADD` | `(0x12>>1)` | 通用常數/硬體欄位定義 |
| 2181 | `!defined(__IO_H__)` | `CLR_CMD` | `BIT15` | 通用常數/硬體欄位定義 |
| 2182 | `!defined(__IO_H__)` | `RAMB_MRS_BA` | `(0x11)` | 依原始註解：only _BIT 14:12；註解：only _BIT 14:12 |
| 2183 | `!defined(__IO_H__)` | `RAMB_MRS_RANK` | `(0x11)` | 依原始註解：only _BIT 11:10；註解：only _BIT 11:10 |
| 2184 | `!defined(__IO_H__)` | `EXI_DRST` | `BIT9` | 通用常數/硬體欄位定義 |
| 2185 | `!defined(__IO_H__)` | `DRST` | `BIT8` | 通用常數/硬體欄位定義 |
| 2186 | `!defined(__IO_H__)` | `DPDN` | `BIT7` | 通用常數/硬體欄位定義 |
| 2187 | `!defined(__IO_H__)` | `ZQC` | `BIT6` | 通用常數/硬體欄位定義 |
| 2188 | `!defined(__IO_H__)` | `EXI` | `BIT5` | 通用常數/硬體欄位定義 |
| 2189 | `!defined(__IO_H__)` | `PDN` | `BIT4` | 通用常數/硬體欄位定義 |
| 2190 | `!defined(__IO_H__)` | `SRF` | `BIT3` | 通用常數/硬體欄位定義 |
| 2191 | `!defined(__IO_H__)` | `PRE` | `BIT2` | 通用常數/硬體欄位定義 |
| 2192 | `!defined(__IO_H__)` | `MRS` | `BIT1` | 通用常數/硬體欄位定義 |
| 2193 | `!defined(__IO_H__)` | `REF` | `BIT0` | 通用常數/硬體欄位定義 |
| 2195 | `!defined(__IO_H__)` | `RAML_DDR3_MEMC_CFG1` | `(0x14>>2)` | 通用常數/硬體欄位定義 |
| 2196 | `!defined(__IO_H__)` | `RAMB_TRTP` | `(0x17)` | 依原始註解：only _BIT 31:30；註解：only _BIT 31:30 |
| 2197 | `!defined(__IO_H__)` | `RAMB_TRAS` | `(0x17)` | 依原始註解：only _BIT 28:24；註解：only _BIT 28:24 |
| 2198 | `!defined(__IO_H__)` | `RAMB_TRFC` | `(0x16)` | 通用常數/硬體欄位定義 |
| 2199 | `!defined(__IO_H__)` | `RAML_TWTR` | `(0x14>>2)` | 依原始註解：only _BIT 29,15:12；註解：only _BIT 29,15:12 |
| 2200 | `!defined(__IO_H__)` | `RAMB_TWR` | `(0x15)` | 依原始註解：only _BIT 11:8；註解：only _BIT 11:8 |
| 2201 | `!defined(__IO_H__)` | `RAMB_TRP` | `(0x14)` | 依原始註解：only _BIT 7:4；註解：only _BIT 7:4 |
| 2202 | `!defined(__IO_H__)` | `RAMB_TRCD` | `(0x14)` | 依原始註解：only _BIT 3:0；註解：only _BIT 3:0 |
| 2204 | `!defined(__IO_H__)` | `RAML_DDR3_MEMC_CFG2` | `(0x18>>2)` | 通用常數/硬體欄位定義 |
| 2205 | `!defined(__IO_H__)` | `DDR2` | `BIT31` | 通用常數/硬體欄位定義 |
| 2206 | `!defined(__IO_H__)` | `BANK` | `BIT30` | 通用常數/硬體欄位定義 |
| 2207 | `!defined(__IO_H__)` | `RAMB_RANK` | `(0x1B)` | 依原始註解：only _BIT 29:28；註解：only _BIT 29:28 |
| 2208 | `!defined(__IO_H__)` | `RAMB_ROW` | `(0x1A)` | 依原始註解：only _BIT 22:20；註解：only _BIT 22:20 |
| 2209 | `!defined(__IO_H__)` | `RAMB_COL` | `(0x1A)` | 依原始註解：only _BIT 17:16；註解：only _BIT 17:16 |
| 2210 | `!defined(__IO_H__)` | `RAMB_TRRD` | `(0x19)` | 依原始註解：only _BIT 14:12；註解：only _BIT 14:12 |
| 2211 | `!defined(__IO_H__)` | `CL_ADJUST3` | `BIT11` | 通用常數/硬體欄位定義 |
| 2212 | `!defined(__IO_H__)` | `CL_ADJUST2_0` | `(0x19)` | 依原始註解：only _BIT 10:8；註解：only _BIT 10:8 |
| 2213 | `!defined(__IO_H__)` | `CWLSEL` | `BIT7` | 通用常數/硬體欄位定義 |
| 2214 | `!defined(__IO_H__)` | `CWL` | `(0x18)` | 依原始註解：only _BIT 6:4；註解：only _BIT 6:4 |
| 2215 | `!defined(__IO_H__)` | `CL` | `(0x18)` | 依原始註解：only _BIT 3:0；註解：only _BIT 3:0 |
| 2217 | `!defined(__IO_H__)` | `RAML_DDR3_MEMC_CFG3` | `(0x1C>>2)` | 通用常數/硬體欄位定義 |
| 2218 | `!defined(__IO_H__)` | `ZQCSEN` | `BIT15` | 通用常數/硬體欄位定義 |
| 2220 | `!defined(__IO_H__)` | `RAML_DDR3_MEMC_CFG4` | `(0x20>>2)` | 通用常數/硬體欄位定義 |
| 2221 | `!defined(__IO_H__)` | `DFICKDEN` | `BIT31` | 通用常數/硬體欄位定義 |
| 2222 | `!defined(__IO_H__)` | `WL` | `BIT1` | 通用常數/硬體欄位定義 |
| 2223 | `!defined(__IO_H__)` | `RL` | `BIT0` | 通用常數/硬體欄位定義 |
| 2225 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_TRIGGER` | `(0x24>>2)` | 通用常數/硬體欄位定義 |
| 2226 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_TRIGGER_1` | `(0x100>>2)` | 通用常數/硬體欄位定義 |
| 2227 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_TRIGGER_2` | `(0x140>>2)` | 通用常數/硬體欄位定義 |
| 2228 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_TRIGGER_3` | `(0x180>>2)` | 通用常數/硬體欄位定義 |
| 2230 | `!defined(__IO_H__)` | `clear_BIST1` | `BIT24` | 通用常數/硬體欄位定義 |
| 2231 | `!defined(__IO_H__)` | `BIST_SET_ERROR` | `BIT20` | 通用常數/硬體欄位定義 |
| 2232 | `!defined(__IO_H__)` | `BIST_FAIL` | `BIT8` | 通用常數/硬體欄位定義 |
| 2233 | `!defined(__IO_H__)` | `Shift_4MB` | `BIT1` | 通用常數/硬體欄位定義 |
| 2234 | `!defined(__IO_H__)` | `BIST_TRIGGER` | `BIT0` | 通用常數/硬體欄位定義 |
| 2236 | `!defined(__IO_H__)` | `RAML_DDR3_PHY_CFG` | `(0x28>>2)` | 通用常數/硬體欄位定義 |
| 2237 | `!defined(__IO_H__)` | `phycnt_odd_lch` | `BIT24` | 通用常數/硬體欄位定義 |
| 2238 | `!defined(__IO_H__)` | `X32` | `BIT20` | 通用常數/硬體欄位定義 |
| 2239 | `!defined(__IO_H__)` | `dqin32_neg` | `BIT16` | 通用常數/硬體欄位定義 |
| 2240 | `!defined(__IO_H__)` | `phycnt_reset` | `BIT12` | 通用常數/硬體欄位定義 |
| 2241 | `!defined(__IO_H__)` | `DDR_CTRL_EMPTY` | `BIT9` | 通用常數/硬體欄位定義 |
| 2242 | `!defined(__IO_H__)` | `DDR_SV_STOP` | `BIT8` | 通用常數/硬體欄位定義 |
| 2243 | `!defined(__IO_H__)` | `SW_RESET` | `BIT4` | 通用常數/硬體欄位定義 |
| 2245 | `!defined(__IO_H__)` | `RAML_LPDDR2_MRR_DATA` | `(0x2C>>2)` | 通用常數/硬體欄位定義 |
| 2246 | `!defined(__IO_H__)` | `MRR_VALID` | `BIT16` | 通用常數/硬體欄位定義 |
| 2247 | `!defined(__IO_H__)` | `MRR_DIN` | `(0x2C)` | 通用常數/硬體欄位定義 |
| 2249 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_START_ADDR` | `(0x58>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2250 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_START_ADDR_1` | `(0x104>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2251 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_START_ADDR_2` | `(0x144>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2252 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_START_ADDR_3` | `(0x184>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2253 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_END_ADDR` | `(0x5C>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2254 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_END_ADDR_1` | `(0x108>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2255 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_END_ADDR_2` | `(0x148>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2256 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_END_ADDR_3` | `(0x188>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2257 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_RD_START_TIME` | `(0x60>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2258 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_RD_START_TIME_1` | `(0x10c>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2259 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_RD_START_TIME_2` | `(0x14c>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2260 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_RD_START_TIME_3` | `(0x18c>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2261 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_START_DATA` | `(0x64>>2)` | 通用常數/硬體欄位定義 |
| 2262 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_START_DATA_1` | `(0x110>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2263 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_START_DATA_2` | `(0x150>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2264 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_START_DATA_3` | `(0x190>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2265 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_TOG_DATA` | `(0x68>>2)` | 通用常數/硬體欄位定義 |
| 2266 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_CFG` | `(0x6C>>2)` | 通用常數/硬體欄位定義 |
| 2267 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_CFG_1` | `(0x114>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2268 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_CFG_2` | `(0x154>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2269 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_CFG_3` | `(0x194>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2270 | `!defined(__IO_H__)` | `RW_PERIOD_ADJUST` | `BIT24` | 通用常數/硬體欄位定義 |
| 2271 | `!defined(__IO_H__)` | `BIST_LEN_FIX_EN` | `BIT0` | 通用常數/硬體欄位定義 |
| 2272 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_DLY` | `(0x70>>2)` | 通用常數/硬體欄位定義 |
| 2273 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_DLY_1` | `(0x118>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2274 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_DLY_2` | `(0x158>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2275 | `!defined(__IO_H__)` | `RAML_DDR3_BIST_DLY_3` | `(0x198>>2)` | 依原始註解：only _BIT 31:3；註解：only _BIT 31:3 |
| 2276 | `!defined(__IO_H__)` | `BIST_RD_RDM_DLY_EN` | `BIT16` | 通用常數/硬體欄位定義 |
| 2277 | `!defined(__IO_H__)` | `BIST_WR_RDM_DLY_EN` | `BIT0` | 通用常數/硬體欄位定義 |
| 2279 | `!defined(__IO_H__)` | `RAML_DDR3_ECC_CTRL` | `(0x80>>2)` | 通用常數/硬體欄位定義 |
| 2280 | `!defined(__IO_H__)` | `Z3_ECC_ERROR_CLR_1` | `BIT30` | 通用常數/硬體欄位定義 |
| 2281 | `!defined(__IO_H__)` | `Z3_CRC_ERROR_CLR_1` | `BIT29` | 通用常數/硬體欄位定義 |
| 2282 | `!defined(__IO_H__)` | `Z2_ECC_ERROR_CLR_1` | `BIT27` | 通用常數/硬體欄位定義 |
| 2283 | `!defined(__IO_H__)` | `Z2_CRC_ERROR_CLR_1` | `BIT26` | 通用常數/硬體欄位定義 |
| 2284 | `!defined(__IO_H__)` | `Z1_ECC_ERROR_CLR_1` | `BIT24` | 通用常數/硬體欄位定義 |
| 2285 | `!defined(__IO_H__)` | `Z3_UN_CORRECT_CLR_0` | `BIT23` | 通用常數/硬體欄位定義 |
| 2286 | `!defined(__IO_H__)` | `Z3_ECC_ERROR_CLR_0` | `BIT22` | 通用常數/硬體欄位定義 |
| 2287 | `!defined(__IO_H__)` | `Z3_CRC_ERROR_CLR_0` | `BIT21` | 通用常數/硬體欄位定義 |
| 2288 | `!defined(__IO_H__)` | `Z2_UN_CORRECT_CLR_0` | `BIT20` | 通用常數/硬體欄位定義 |
| 2289 | `!defined(__IO_H__)` | `Z2_ECC_ERROR_CLR_0` | `BIT19` | 通用常數/硬體欄位定義 |
| 2290 | `!defined(__IO_H__)` | `Z2_CRC_ERROR_CLR_0` | `BIT18` | 通用常數/硬體欄位定義 |
| 2291 | `!defined(__IO_H__)` | `Z1_UN_CORRECT_CLR_0` | `BIT17` | 通用常數/硬體欄位定義 |
| 2292 | `!defined(__IO_H__)` | `Z1_ECC_ERROR_CLR_0` | `BIT16` | 通用常數/硬體欄位定義 |
| 2293 | `!defined(__IO_H__)` | `Z3_UN_CORRECT_INT_EN` | `BIT15` | 通用常數/硬體欄位定義 |
| 2294 | `!defined(__IO_H__)` | `Z3_ECC_ERROR_INT_EN` | `BIT14` | 通用常數/硬體欄位定義 |
| 2295 | `!defined(__IO_H__)` | `Z3_CRC_ERROR_INT_EN` | `BIT13` | 通用常數/硬體欄位定義 |
| 2296 | `!defined(__IO_H__)` | `Z2_UN_CORRECT_INT_EN` | `BIT12` | 通用常數/硬體欄位定義 |
| 2297 | `!defined(__IO_H__)` | `Z2_ECC_ERROR_INT_EN` | `BIT11` | 通用常數/硬體欄位定義 |
| 2298 | `!defined(__IO_H__)` | `Z2_CRC_ERROR_INT_EN` | `BIT10` | 通用常數/硬體欄位定義 |
| 2299 | `!defined(__IO_H__)` | `Z1_UN_CORRECT_INT_EN` | `BIT9` | 通用常數/硬體欄位定義 |
| 2300 | `!defined(__IO_H__)` | `Z1_ECC_ERROR_INT_EN` | `BIT8` | 通用常數/硬體欄位定義 |
| 2301 | `!defined(__IO_H__)` | `Z3_UN_CORRECT_INT` | `BIT7` | 通用常數/硬體欄位定義 |
| 2302 | `!defined(__IO_H__)` | `Z3_ECC_ERROR_INT` | `BIT6` | 通用常數/硬體欄位定義 |
| 2303 | `!defined(__IO_H__)` | `Z3_CRC_ERROR_INT` | `BIT5` | 通用常數/硬體欄位定義 |
| 2304 | `!defined(__IO_H__)` | `Z2_UN_CORRECT_INT` | `BIT4` | 通用常數/硬體欄位定義 |
| 2305 | `!defined(__IO_H__)` | `Z2_ECC_ERROR_INT` | `BIT3` | 通用常數/硬體欄位定義 |
| 2306 | `!defined(__IO_H__)` | `Z2_CRC_ERROR_INT` | `BIT2` | 通用常數/硬體欄位定義 |
| 2307 | `!defined(__IO_H__)` | `Z1_UN_CORRECT_INT` | `BIT1` | 通用常數/硬體欄位定義 |
| 2308 | `!defined(__IO_H__)` | `Z1_ECC_ERROR_INT` | `BIT0` | 通用常數/硬體欄位定義 |
| 2311 | `!defined(__IO_H__)` | `RAML_DDR3_Z1_ECC_ERROR_ADDR` | `(0x84>>2)` | 通用常數/硬體欄位定義 |
| 2312 | `!defined(__IO_H__)` | `RAML_DDR3_Z1_ECC_ERROR_NUMBER` | `(0x88>>2)` | 通用常數/硬體欄位定義 |
| 2313 | `!defined(__IO_H__)` | `RAML_DDR3_Z1_ECC_ERROR_ARID` | `(0x8C>>2)` | 通用常數/硬體欄位定義 |
| 2314 | `!defined(__IO_H__)` | `RAML_DDR3_Z1_ECC_UN_CORRECT_ADDR` | `(0x90>>2)` | 通用常數/硬體欄位定義 |
| 2315 | `!defined(__IO_H__)` | `RAML_DDR3_Z1_ECC_UN_CORRECT_NUMBER` | `(0x94>>2)` | 通用常數/硬體欄位定義 |
| 2316 | `!defined(__IO_H__)` | `RAML_DDR3_Z1_ECC_UN_CORRECT_ARID` | `(0x98>>2)` | 通用常數/硬體欄位定義 |
| 2318 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_CRC_ERROR_ADDR` | `(0x9C>>2)` | 通用常數/硬體欄位定義 |
| 2319 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_CRC_ERROR_NUMBER` | `(0xA0>>2)` | 通用常數/硬體欄位定義 |
| 2320 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_CRC_ERROR_ARID` | `(0xA4>>2)` | 通用常數/硬體欄位定義 |
| 2321 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_ECC_ERROR_ADDR` | `(0xA8>>2)` | 通用常數/硬體欄位定義 |
| 2322 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_ECC_ERROR_NUMBER` | `(0xAC>>2)` | 通用常數/硬體欄位定義 |
| 2323 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_ECC_ERROR_ARID` | `(0xB0>>2)` | 通用常數/硬體欄位定義 |
| 2324 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_ECC_UN_CORRECT_ADDR` | `(0xB4>>2)` | 通用常數/硬體欄位定義 |
| 2325 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_ECC_UN_CORRECT_NUMBER` | `(0xB8>>2)` | 通用常數/硬體欄位定義 |
| 2326 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_ECC_UN_CORRECT_ARID` | `(0xBC>>2)` | 通用常數/硬體欄位定義 |
| 2329 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_CRC_ERROR_ADDR` | `(0xC0>>2)` | 通用常數/硬體欄位定義 |
| 2330 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_CRC_ERROR_NUMBER` | `(0xC4>>2)` | 通用常數/硬體欄位定義 |
| 2331 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_CRC_ERROR_ARID` | `(0xC8>>2)` | 通用常數/硬體欄位定義 |
| 2332 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_ECC_ERROR_ADDR` | `(0xCC>>2)` | 通用常數/硬體欄位定義 |
| 2333 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_ECC_ERROR_NUMBER` | `(0xD0>>2)` | 通用常數/硬體欄位定義 |
| 2334 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_ECC_ERROR_ARID` | `(0xD4>>2)` | 通用常數/硬體欄位定義 |
| 2335 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_ECC_UN_CORRECT_ADDR` | `(0xD8>>2)` | 通用常數/硬體欄位定義 |
| 2336 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_ECC_UN_CORRECT_NUMBER` | `(0xDC>>2)` | 通用常數/硬體欄位定義 |
| 2337 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_ECC_UN_CORRECT_ARID` | `(0xE0>>2)` | 通用常數/硬體欄位定義 |
| 2339 | `!defined(__IO_H__)` | `RAML_DDR3_ZONE_STATUS` | `(0xE4>>2)` | 通用常數/硬體欄位定義 |
| 2340 | `!defined(__IO_H__)` | `ZONE_INVALID_CLR` | `BIT16` | 通用常數/硬體欄位定義 |
| 2341 | `!defined(__IO_H__)` | `Z3_READ_INVALID_INT_EN` | `BIT13` | 通用常數/硬體欄位定義 |
| 2342 | `!defined(__IO_H__)` | `Z3_WRITE_INVALID_INT_EN` | `BIT12` | 通用常數/硬體欄位定義 |
| 2343 | `!defined(__IO_H__)` | `Z2_READ_INVALID_INT_EN` | `BIT11` | 通用常數/硬體欄位定義 |
| 2344 | `!defined(__IO_H__)` | `Z2_WRITE_INVALID_INT_EN` | `BIT10` | 通用常數/硬體欄位定義 |
| 2345 | `!defined(__IO_H__)` | `Z1_READ_INVALID_INT_EN` | `BIT9` | 通用常數/硬體欄位定義 |
| 2346 | `!defined(__IO_H__)` | `Z1_WRITE_INVALID_INT_EN` | `BIT8` | 通用常數/硬體欄位定義 |
| 2347 | `!defined(__IO_H__)` | `Z3_READ_INVALID_INT` | `BIT5` | 通用常數/硬體欄位定義 |
| 2348 | `!defined(__IO_H__)` | `Z3_WRITE_INVALID_INT` | `BIT4` | 通用常數/硬體欄位定義 |
| 2349 | `!defined(__IO_H__)` | `Z2_READ_INVALID_INT` | `BIT3` | 通用常數/硬體欄位定義 |
| 2350 | `!defined(__IO_H__)` | `Z2_WRITE_INVALID_INT` | `BIT2` | 通用常數/硬體欄位定義 |
| 2351 | `!defined(__IO_H__)` | `Z1_READ_INVALID_INT` | `BIT1` | 通用常數/硬體欄位定義 |
| 2352 | `!defined(__IO_H__)` | `Z1_WRITE_INVALID_INT` | `BIT0` | 通用常數/硬體欄位定義 |
| 2354 | `!defined(__IO_H__)` | `RAML_DDR3_ROUND_ROBIN_WATC` | `(0xE8>>2)` | 通用常數/硬體欄位定義 |
| 2355 | `!defined(__IO_H__)` | `RAMW_REG_WAITC_WR` | `(0xE8>>1)` | 系統暫存器/記憶體映射位址定義 |
| 2356 | `!defined(__IO_H__)` | `RAMW_REG_WAITC_RD` | `(0xEA>>1)` | 系統暫存器/記憶體映射位址定義 |
| 2358 | `!defined(__IO_H__)` | `RAML_DDR3_SRMAC_DBG_SEL` | `(0xEC>>2)` | 通用常數/硬體欄位定義 |
| 2359 | `!defined(__IO_H__)` | `RAMW_SRMAC_DBG_SEL` | `(0xEC>>1)` | 通用常數/硬體欄位定義 |
| 2360 | `!defined(__IO_H__)` | `RAMB_R_SRAMC_REST` | `0xEF` | 依原始註解：BIT28:29；註解：BIT28:29 |
| 2361 | `!defined(__IO_H__)` | `R_SRAMC_REST` | `BIT30` | 通用常數/硬體欄位定義 |
| 2362 | `!defined(__IO_H__)` | `R_SRAMC_RREADY_OPT` | `BIT31` | 通用常數/硬體欄位定義 |
| 2364 | `!defined(__IO_H__)` | `RAML_DDR3_PAR_ERR_EN` | `(0xF0>>2)` | 通用常數/硬體欄位定義 |
| 2365 | `!defined(__IO_H__)` | `PAR_ERR_EN_0` | `BIT0` | 通用常數/硬體欄位定義 |
| 2367 | `!defined(__IO_H__)` | `RAMLL_DDR3_FIFO_ERR_FLAG` | `(0xF4>>3)` | 依原始註解：BIT0:41；註解：BIT0:41 |
| 2369 | `!defined(__IO_H__)` | `RAML_DDR3_REFQ` | `(0x1C0>>2)` | 通用常數/硬體欄位定義 |
| 2370 | `!defined(__IO_H__)` | `RAMB_NUM_REFQ` | `(0x1C0)` | 依原始註解：BIT0:3；註解：BIT0:3 |
| 2371 | `!defined(__IO_H__)` | `RAMB_T_ALLREF` | `(0x1C1)` | 依原始註解：ALL 8 BITS；註解：ALL 8 BITS |
| 2372 | `!defined(__IO_H__)` | `RAMB_ALLCYC_REF` | `(0x1C2)` | 依原始註解：BIT0:2；註解：BIT0:2 |
| 2374 | `!defined(__IO_H__)` | `RAML_DDR3_PFM_0` | `(0x1C4>>2)` | 通用常數/硬體欄位定義 |
| 2375 | `!defined(__IO_H__)` | `PFM_EN` | `BIT0` | 通用常數/硬體欄位定義 |
| 2376 | `!defined(__IO_H__)` | `PFM_CPT` | `BIT1` | 通用常數/硬體欄位定義 |
| 2377 | `!defined(__IO_H__)` | `RAMB_PFM_BGRP` | `0x1C4` | 依原始註解：BIT2:3；註解：BIT2:3 |
| 2379 | `!defined(__IO_H__)` | `RAML_PFM_T_AC` | `(0x1C8>>2)` | 通用常數/硬體欄位定義 |
| 2380 | `!defined(__IO_H__)` | `RAML_PFM_T_ALL` | `(0x1CC>>2)` | 通用常數/硬體欄位定義 |
| 2382 | `!defined(__IO_H__)` | `RAML_DDR3_HIPRIOR_ID0` | `(0x1E8>>2)` | 通用常數/硬體欄位定義 |
| 2383 | `!defined(__IO_H__)` | `RAMB_HIPRIOT_ID0` | `0x1E8` | 通用常數/硬體欄位定義 |
| 2384 | `!defined(__IO_H__)` | `RAMB_HIPRIOT_ID1` | `0x1E9` | 通用常數/硬體欄位定義 |
| 2385 | `!defined(__IO_H__)` | `RAMB_HIPRIOT_ID2` | `0x1EA` | 通用常數/硬體欄位定義 |
| 2386 | `!defined(__IO_H__)` | `RAMB_HIPRIOT_ID3` | `0x1EB` | 通用常數/硬體欄位定義 |
| 2389 | `!defined(__IO_H__)` | `RAML_DDR3_HIPRIOR_ID1` | `(0x1EC>>2)` | 通用常數/硬體欄位定義 |
| 2390 | `!defined(__IO_H__)` | `RAMB_HIPRIOT_ID4` | `0x1Ec` | 通用常數/硬體欄位定義 |
| 2391 | `!defined(__IO_H__)` | `RAMB_HIPRIOT_ID5` | `0x1ED` | 通用常數/硬體欄位定義 |
| 2392 | `!defined(__IO_H__)` | `RAMB_HIPRIOT_ID6` | `0x1EE` | 通用常數/硬體欄位定義 |
| 2393 | `!defined(__IO_H__)` | `RAMB_HIPRIOT_ID7` | `0x1EF` | 通用常數/硬體欄位定義 |
| 2395 | `!defined(__IO_H__)` | `RAML_DDR3_Z1_VALID_ADDR` | `(0x1D0>>2)` | 通用常數/硬體欄位定義 |
| 2396 | `!defined(__IO_H__)` | `RAML_DDR3_Z2_VALID_ADDR` | `(0x1D4>>2)` | 通用常數/硬體欄位定義 |
| 2397 | `!defined(__IO_H__)` | `RAML_DDR3_Z3_VALID_ADDR` | `(0x1D8>>2)` | 通用常數/硬體欄位定義 |
| 2401 | `!defined(__IO_H__)` | `RAML_DDR3_DRAM_CLOCK_GATE` | `(0x200>>2)` | 通用常數/硬體欄位定義 |
| 2402 | `!defined(__IO_H__)` | `MEMINITDONE` | `BIT22` | 通用常數/硬體欄位定義 |
| 2403 | `!defined(__IO_H__)` | `CTL_RST_N` | `BIT21` | 通用常數/硬體欄位定義 |
| 2404 | `!defined(__IO_H__)` | `PRESETN` | `BIT20` | 通用常數/硬體欄位定義 |
| 2405 | `!defined(__IO_H__)` | `RET_EN` | `BIT19` | 通用常數/硬體欄位定義 |
| 2406 | `!defined(__IO_H__)` | `RET_EN_I` | `BIT18` | 通用常數/硬體欄位定義 |
| 2407 | `!defined(__IO_H__)` | `RET_EN_N_I` | `BIT17` | 通用常數/硬體欄位定義 |
| 2408 | `!defined(__IO_H__)` | `DFIINITSTART` | `BIT16` | 通用常數/硬體欄位定義 |
| 2409 | `!defined(__IO_H__)` | `RAMB_DFIDATABYTEDISABLE` | `0x201` | 依原始註解：BIT1:4；註解：BIT1:4 |
| 2410 | `!defined(__IO_H__)` | `DFIDRAMCLKDISABLE` | `BIT8` | 通用常數/硬體欄位定義 |
| 2411 | `!defined(__IO_H__)` | `DFIINITCOMPLETE` | `BIT6` | 通用常數/硬體欄位定義 |
| 2416 | `!defined(__IO_H__)` | `BIST_Data_Sequential` | `0` | 依原始註解：Data Mode :；註解：Data Mode : |
| 2417 | `!defined(__IO_H__)` | `BIST_Data_Random` | `1` | 通用常數/硬體欄位定義 |
| 2418 | `!defined(__IO_H__)` | `BIST_Data_Toggle` | `2` | 通用常數/硬體欄位定義 |
| 2420 | `!defined(__IO_H__)` | `BIST_INCR` | `0` | 依原始註解：WRAP :；註解：WRAP : |
| 2421 | `!defined(__IO_H__)` | `BIST_WRAP` | `1` | 通用常數/硬體欄位定義 |
| 2422 | `!defined(__IO_H__)` | `BIST_WRAP_INCR_Random` | `2` | 通用常數/硬體欄位定義 |
| 2424 | `!defined(__IO_H__)` | `BIST_Len_Asign` | `0` | 依原始註解：Length Slection :；註解：Length Slection : |
| 2425 | `!defined(__IO_H__)` | `BIST_Len_0_3_15_63` | `1` | 通用常數/硬體欄位定義 |
| 2426 | `!defined(__IO_H__)` | `BIST_Len_15_63` | `2` | 通用常數/硬體欄位定義 |
| 2427 | `!defined(__IO_H__)` | `BIST_Len_64` | `3` | 通用常數/硬體欄位定義 |
| 2428 | `!defined(__IO_H__)` | `BIST_Len_63` | `4` | 通用常數/硬體欄位定義 |
| 2429 | `!defined(__IO_H__)` | `BIST_Len_0_3` | `5` | 通用常數/硬體欄位定義 |
| 2431 | `!defined(__IO_H__)` | `BIST_Internal` | `0` | 依原始註解：Address Mode :；註解：Address Mode : |
| 2432 | `!defined(__IO_H__)` | `BIST_Address_Sequential` | `1` | 通用常數/硬體欄位定義 |
| 2433 | `!defined(__IO_H__)` | `BIST_Address_Random` | `2` | 通用常數/硬體欄位定義 |
| 2434 | `!defined(__IO_H__)` | `BIST_Address_Increase` | `3` | 通用常數/硬體欄位定義 |
| 2439 | `!defined(__IO_H__)` | `AXI_REG` | `(SYSTEM_REG+0x00006000)` | 系統暫存器/記憶體映射位址定義；註解：=================================================== \| PS3111 AXI arbiter Define \| =================================================== |
| 2441 | `!defined(__IO_H__)` | `AXIB` | `((volatile UBYTE *)AXI_REG)` | 通用常數/硬體欄位定義 |
| 2442 | `!defined(__IO_H__)` | `AXIW` | `((volatile UWORD *)AXI_REG)` | 通用常數/硬體欄位定義 |
| 2443 | `!defined(__IO_H__)` | `AXIL` | `((volatile U32 *)AXI_REG)` | 通用常數/硬體欄位定義 |
| 2445 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_31_0` | `(0x0 >> 2)` | 通用常數/硬體欄位定義 |
| 2446 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_63_32` | `(0x4 >> 2)` | 通用常數/硬體欄位定義 |
| 2447 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_95_64` | `(0x8 >> 2)` | 通用常數/硬體欄位定義 |
| 2448 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_127_96` | `(0xc >> 2)` | 通用常數/硬體欄位定義 |
| 2449 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_159_128` | `(0x10 >> 2)` | 通用常數/硬體欄位定義 |
| 2450 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_191_160` | `(0x14 >> 2)` | 通用常數/硬體欄位定義 |
| 2451 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_223_192` | `(0x18 >> 2)` | 通用常數/硬體欄位定義 |
| 2452 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_255_224` | `(0x1C >> 2)` | 通用常數/硬體欄位定義 |
| 2453 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_287_256` | `(0x20 >> 2)` | 通用常數/硬體欄位定義 |
| 2454 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_319_288` | `(0x24 >> 2)` | 通用常數/硬體欄位定義 |
| 2455 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_351_320` | `(0x28 >> 2)` | 通用常數/硬體欄位定義 |
| 2456 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_383_352` | `(0x2C >> 2)` | 通用常數/硬體欄位定義 |
| 2457 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_415_384` | `(0x30 >> 2)` | 通用常數/硬體欄位定義 |
| 2458 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_447_416` | `(0x34 >> 2)` | 通用常數/硬體欄位定義 |
| 2459 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_480_448` | `(0x38 >> 2)` | 通用常數/硬體欄位定義 |
| 2460 | `!defined(__IO_H__)` | `AXIL_FLH_FLAG_511_481` | `(0x3C >> 2)` | 通用常數/硬體欄位定義 |
| 2464 | `!defined(__IO_H__)` | `AXIL_FLAG_SET` | `(0x80 >> 2)` | 通用常數/硬體欄位定義 |
| 2465 | `!defined(__IO_H__)` | `AXIW_FLAG_SET` | `(0x80 >> 1)` | 通用常數/硬體欄位定義 |
| 2466 | `!defined(__IO_H__)` | `HOST1_AS` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2467 | `!defined(__IO_H__)` | `FLH_AS` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2468 | `!defined(__IO_H__)` | `TDMAC0_AS` | `SET_BIT3` | 周邊控制暫存器/位元欄位定義 |
| 2469 | `!defined(__IO_H__)` | `HOST1_CHK` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2470 | `!defined(__IO_H__)` | `FLH_CHK` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2471 | `!defined(__IO_H__)` | `FLH_QRYRND_CHK` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2472 | `!defined(__IO_H__)` | `TDMAC0_CHK` | `SET_BIT11` | 周邊控制暫存器/位元欄位定義 |
| 2473 | `!defined(__IO_H__)` | `Flag_SW_256K` | `(CLR_BIT16&CLR_BIT17)` | 通用常數/硬體欄位定義 |
| 2474 | `!defined(__IO_H__)` | `Flag_SW_192K_64K` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 2475 | `!defined(__IO_H__)` | `Flag_SW_128K_128K` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 2477 | `!defined(__IO_H__)` | `AXIW_FLAG_AND` | `(0x84>>1)` | 通用常數/硬體欄位定義 |
| 2478 | `!defined(__IO_H__)` | `FLH_FLAG_31_0_logic_and` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2479 | `!defined(__IO_H__)` | `FLH_FLAG_63_32_logic_and` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2480 | `!defined(__IO_H__)` | `FLH_FLAG_95_64_logic_and` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2481 | `!defined(__IO_H__)` | `FLH_FLAG_127_96_logic_and` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2482 | `!defined(__IO_H__)` | `FLH_FLAG_159_128_logic_and` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2483 | `!defined(__IO_H__)` | `FLH_FLAG_191_160_logic_and` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2484 | `!defined(__IO_H__)` | `FLH_FLAG_223_192_logic_and` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2485 | `!defined(__IO_H__)` | `FLH_FLAG_255_224_logic_and` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2486 | `!defined(__IO_H__)` | `FLH_FLAG_287_256_logic_and` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2487 | `!defined(__IO_H__)` | `FLH_FLAG_319_288_logic_and` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2488 | `!defined(__IO_H__)` | `FLH_FLAG_351_320_logic_and` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2489 | `!defined(__IO_H__)` | `FLH_FLAG_383_352_logic_and` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2490 | `!defined(__IO_H__)` | `FLH_FLAG_415_384_logic_and` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2491 | `!defined(__IO_H__)` | `FLH_FLAG_447_416_logic_and` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2492 | `!defined(__IO_H__)` | `FLH_FLAG_480_448_logic_and` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2493 | `!defined(__IO_H__)` | `FLH_FLAG_511_481_logic_and` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 2495 | `!defined(__IO_H__)` | `AXIW_FLAG_OR` | `(0x88>>1)` | 通用常數/硬體欄位定義 |
| 2496 | `!defined(__IO_H__)` | `FLH_FLAG_31_0_logic_or` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2497 | `!defined(__IO_H__)` | `FLH_FLAG_63_32_logic_or` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2498 | `!defined(__IO_H__)` | `FLH_FLAG_95_64_logic_or` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2499 | `!defined(__IO_H__)` | `FLH_FLAG_127_96_logic_or` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2500 | `!defined(__IO_H__)` | `FLH_FLAG_159_128_logic_or` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2501 | `!defined(__IO_H__)` | `FLH_FLAG_191_160_logic_or` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2502 | `!defined(__IO_H__)` | `FLH_FLAG_223_192_logic_or` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2503 | `!defined(__IO_H__)` | `FLH_FLAG_255_224_logic_or` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2504 | `!defined(__IO_H__)` | `FLH_FLAG_287_256_logic_or` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2505 | `!defined(__IO_H__)` | `FLH_FLAG_319_288_logic_or` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2506 | `!defined(__IO_H__)` | `FLH_FLAG_351_320_logic_or` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2507 | `!defined(__IO_H__)` | `FLH_FLAG_383_352_logic_or` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2508 | `!defined(__IO_H__)` | `FLH_FLAG_415_384_logic_or` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2509 | `!defined(__IO_H__)` | `FLH_FLAG_447_416_logic_or` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2510 | `!defined(__IO_H__)` | `FLH_FLAG_480_448_logic_or` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2511 | `!defined(__IO_H__)` | `FLH_FLAG_511_481_logic_or` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 2513 | `!defined(__IO_H__)` | `AXIL_SET_SINGLE_FLAG` | `(0xA0 >> 2)` | 通用常數/硬體欄位定義 |
| 2515 | `!defined(__IO_H__)` | `AXIL_CLEAR_SINGLE_FLAG` | `(0xA4 >> 2)` | 通用常數/硬體欄位定義 |
| 2517 | `!defined(__IO_H__)` | `AXIL_CLEAR_ARRANGEMENT_FLAG` | `(0xA8 >> 2)` | 通用常數/硬體欄位定義 |
| 2519 | `!defined(__IO_H__)` | `AXIL_SET_ARRANGEMENT_FLAG` | `(0xAC >> 2)` | 通用常數/硬體欄位定義 |
| 2521 | `!defined(__IO_H__)` | `AXIL_SET_START_FLAG` | `(0xB0 >> 2)` | 通用常數/硬體欄位定義 |
| 2523 | `!defined(__IO_H__)` | `AXIL_SET_FLAG_LENGTH` | `(0xB4 >> 2)` | 通用常數/硬體欄位定義 |
| 2525 | `!defined(__IO_H__)` | `AXIL_DDR_WEIGHT_0` | `(0xC4>>2)` | 通用常數/硬體欄位定義 |
| 2527 | `!defined(__IO_H__)` | `AXIL_DDR_WEIGHT_1` | `(0xC8>>2)` | 通用常數/硬體欄位定義 |
| 2529 | `!defined(__IO_H__)` | `AXIL_DBUF_WEIGHT` | `(0xCC>>2)` | 通用常數/硬體欄位定義 |
| 2531 | `!defined(__IO_H__)` | `AXIL_FIRST_START_ADDRESS` | `(0xE0 >> 2)` | 通用常數/硬體欄位定義 |
| 2532 | `!defined(__IO_H__)` | `AXIL_FIRST_END_ADDRESS` | `(0xF0 >> 2)` | 通用常數/硬體欄位定義 |
| 2534 | `!defined(__IO_H__)` | `AXIL_SECOND_START_ADDRESS` | `(0xE4 >> 2)` | 通用常數/硬體欄位定義 |
| 2535 | `!defined(__IO_H__)` | `AXIL_SECOND_END_ADDRESS` | `(0xF4 >> 2)` | 通用常數/硬體欄位定義 |
| 2537 | `!defined(__IO_H__)` | `AXIL_SR_WDOG_PCIE` | `(0x140 >> 2)` | 通用常數/硬體欄位定義 |
| 2539 | `!defined(__IO_H__)` | `AXIL_SR_WDOG_FLH` | `(0x144 >> 2)` | 通用常數/硬體欄位定義 |
| 2541 | `!defined(__IO_H__)` | `AXIL_SR_WDOG_RS` | `(0x148 >> 2)` | 通用常數/硬體欄位定義 |
| 2543 | `!defined(__IO_H__)` | `AXIL_SR_WDOG_SS` | `(0x14C >> 2)` | 通用常數/硬體欄位定義 |
| 2545 | `!defined(__IO_H__)` | `AXIL_SR_WAIT_PCIE` | `(0x178 >> 2)` | 通用常數/硬體欄位定義 |
| 2547 | `!defined(__IO_H__)` | `AXIL_SR_WAIT_FLH` | `(0x17C >> 2)` | 通用常數/硬體欄位定義 |
| 2549 | `!defined(__IO_H__)` | `AXIL_SR_WAIT_RS` | `(0x180 >> 2)` | 通用常數/硬體欄位定義 |
| 2551 | `!defined(__IO_H__)` | `AXIL_SR_WAIT_SS` | `(0x184 >> 2)` | 通用常數/硬體欄位定義 |
| 2556 | `!defined(__IO_H__)` | `SATA_REG` | `(SYSTEM_REG+0x00007000)` | 系統暫存器/記憶體映射位址定義；註解：=================================================== \| PS3111 SATA Register Define \| =================================================== |
| 2557 | `!defined(__IO_H__)` | `HB` | `((volatile UBYTE *)SATA_REG)` | 通用常數/硬體欄位定義 |
| 2558 | `!defined(__IO_H__)` | `HW` | `((volatile UWORD *)SATA_REG)` | 通用常數/硬體欄位定義 |
| 2559 | `!defined(__IO_H__)` | `HL` | `((volatile U32 *)SATA_REG)` | 通用常數/硬體欄位定義 |
| 2561 | `!defined(__IO_H__)` | `HL_INT_FLAG` | `(0x00 >> 2)` | 通用常數/硬體欄位定義 |
| 2562 | `!defined(__IO_H__)` | `H_BUF_DONE_INT` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2563 | `!defined(__IO_H__)` | `H_RCV_CMD_INT` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2564 | `!defined(__IO_H__)` | `H_SRST_CLR_INT` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2565 | `!defined(__IO_H__)` | `H_PLLLOCK_CHG_INT` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2566 | `!defined(__IO_H__)` | `H_LNK_COMRI_INT` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2567 | `!defined(__IO_H__)` | `H_LNK_COMWAKE_INT` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2568 | `!defined(__IO_H__)` | `H_PHYRDY_CHG_INT` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2569 | `!defined(__IO_H__)` | `H_LNK_ESTB_CHG_INT` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2570 | `!defined(__IO_H__)` | `H_LNK_REQPRCV_INT` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2571 | `!defined(__IO_H__)` | `H_LNK_REQSRCV_INT` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2572 | `!defined(__IO_H__)` | `H_TRS_BISTRCV_INT` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2573 | `!defined(__IO_H__)` | `H_MZ_DONE_INT` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2574 | `!defined(__IO_H__)` | `H_ERR_EVT_INT` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2575 | `!defined(__IO_H__)` | `H_CMD_FINISH_INT` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2576 | `!defined(__IO_H__)` | `H_HAT_CLR_INT` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2577 | `!defined(__IO_H__)` | `H_NCQ_CMD_EXPR_INT` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 2578 | `!defined(__IO_H__)` | `H_RCV_CTRL_INT` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 2579 | `!defined(__IO_H__)` | `H_DIPM_TT_INT` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 2580 | `!defined(__IO_H__)` | `H_P2S_WC_TT_INT` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 2581 | `!defined(__IO_H__)` | `H_AUTO_P2S_TT_INT` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 2584 | `!defined(__IO_H__)` | `HL_INT_EN` | `(0x04 >> 2)` | 通用常數/硬體欄位定義 |
| 2585 | `!defined(__IO_H__)` | `H_BUF_DONE_INT_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2586 | `!defined(__IO_H__)` | `H_RCV_CMD_INT_EN` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2587 | `!defined(__IO_H__)` | `H_SRST_CLR_INT_EN` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2588 | `!defined(__IO_H__)` | `H_PLLLOCK_CHG_INT_EN` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2589 | `!defined(__IO_H__)` | `H_LNK_COMRI_INT_EN` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2590 | `!defined(__IO_H__)` | `H_LNK_COMWAKE_INT_EN` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2591 | `!defined(__IO_H__)` | `H_PHYRDY_CHG_INT_EN` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2592 | `!defined(__IO_H__)` | `H_LNK_ESTB_CHG_INT_EN` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2593 | `!defined(__IO_H__)` | `H_LNK_REQPRCV_INT_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2594 | `!defined(__IO_H__)` | `H_LNK_REQSRCV_INT_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2595 | `!defined(__IO_H__)` | `H_TRS_BISTRCV_INT_EN` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2596 | `!defined(__IO_H__)` | `H_MZ_DONE_INT_EN` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2597 | `!defined(__IO_H__)` | `H_ERR_EVT_INT_EN` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2598 | `!defined(__IO_H__)` | `H_ERR_EVT_INT_DIS` | `CLR_BIT12` | 通用常數/硬體欄位定義 |
| 2599 | `!defined(__IO_H__)` | `H_CMD_FINISH_INT_EN` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2600 | `!defined(__IO_H__)` | `H_HAT_CLR_INT_EN` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2601 | `!defined(__IO_H__)` | `H_NCQ_CMD_EXPR_INT_EN` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 2602 | `!defined(__IO_H__)` | `H_RCV_CTRL_INT_EN` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 2603 | `!defined(__IO_H__)` | `H_RCV_CTRL_INT` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 2604 | `!defined(__IO_H__)` | `H_DIPM_TT_INT` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 2605 | `!defined(__IO_H__)` | `H_P2S_WC_TT_INT` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 2606 | `!defined(__IO_H__)` | `H_AUTO_P2S_TT_INT` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 2608 | `!defined(__IO_H__)` | `HB_FEATURE` | `0x08` | 通用常數/硬體欄位定義 |
| 2609 | `!defined(__IO_H__)` | `HW_FEATURE` | `(0x08>>1)` | 通用常數/硬體欄位定義 |
| 2611 | `!defined(__IO_H__)` | `HB_FEATURE_EXP` | `0x09` | 通用常數/硬體欄位定義 |
| 2613 | `!defined(__IO_H__)` | `HB_SECTOR_CNT` | `0x0A` | 通用常數/硬體欄位定義 |
| 2614 | `!defined(__IO_H__)` | `HW_SECTOR_CNT` | `(0x0A>>1)` | 通用常數/硬體欄位定義 |
| 2616 | `!defined(__IO_H__)` | `HB_SECTOR_CNT_EXP` | `0x0B` | 通用常數/硬體欄位定義 |
| 2618 | `!defined(__IO_H__)` | `HB_LBA_L` | `0x0C` | 通用常數/硬體欄位定義 |
| 2619 | `!defined(__IO_H__)` | `HW_LBA_L` | `(0x0C>>1)` | 通用常數/硬體欄位定義 |
| 2620 | `!defined(__IO_H__)` | `HL_LBA_L` | `(0x0C>>2)` | 通用常數/硬體欄位定義 |
| 2622 | `!defined(__IO_H__)` | `HB_LBA_M` | `0x0D` | 通用常數/硬體欄位定義 |
| 2624 | `!defined(__IO_H__)` | `HB_LBA_H` | `0x0E` | 通用常數/硬體欄位定義 |
| 2626 | `!defined(__IO_H__)` | `HB_LBA_L_EXP` | `0x0F` | 通用常數/硬體欄位定義 |
| 2628 | `!defined(__IO_H__)` | `HB_LBA_M_EXP` | `0x10` | 通用常數/硬體欄位定義 |
| 2629 | `!defined(__IO_H__)` | `HW_LBA_M_EXP` | `(0x10>>1)` | 通用常數/硬體欄位定義 |
| 2631 | `!defined(__IO_H__)` | `HB_LBA_H_EXP` | `0x11` | 通用常數/硬體欄位定義 |
| 2633 | `!defined(__IO_H__)` | `HB_DEVICE` | `0x12` | 通用常數/硬體欄位定義 |
| 2638 | `!defined(__IO_H__)` | `CHK_FUA` | `CHK_BIT7` | 依原始註解：When the FUA bit is set to one regardless of whether volatile and/or non-volatil；註解：When the FUA bit is set to one regardless of whether volatile and/or non-volatile write caching in the device is enabled or not, \| the user data shall be written to non-volatile media before command completion is reported. \| When the FUA bit is cleared to zero the device may return command completion before the data is written to the non-volatile media. \| Set Forced Unit Access (FUA) |
| 2641 | `!defined(__IO_H__)` | `HB_COMMAND` | `0x13` | 通用常數/硬體欄位定義 |
| 2643 | `!defined(__IO_H__)` | `HB_STATUS` | `0x14` | 通用常數/硬體欄位定義 |
| 2644 | `!defined(__IO_H__)` | `SET_ICRC` | `SET_BIT7` | 依原始註解：Set Bad BlocK detected；註解：Set Bad BlocK detected |
| 2645 | `!defined(__IO_H__)` | `SET_BBK` | `SET_BIT7` | 依原始註解：Set Bad BlocK detected；註解：Set Bad BlocK detected |
| 2646 | `!defined(__IO_H__)` | `SET_UNC` | `SET_BIT6` | 依原始註解：Set UN-Correctable data；註解：Set UN-Correctable data |
| 2647 | `!defined(__IO_H__)` | `SET_WP` | `SET_BIT6` | 依原始註解：Set write protect for write cmd；註解：Set write protect for write cmd |
| 2648 | `!defined(__IO_H__)` | `SET_MC` | `SET_BIT5` | 依原始註解：Set Media Changed；註解：Set Media Changed |
| 2649 | `!defined(__IO_H__)` | `SET_IDNF` | `SET_BIT4` | 依原始註解：ID Not Found；註解：ID Not Found |
| 2650 | `!defined(__IO_H__)` | `SET_MCR` | `SET_BIT3` | 依原始註解：Media Changed Requested；註解：Media Changed Requested |
| 2651 | `!defined(__IO_H__)` | `SET_ABRT` | `SET_BIT2` | 依原始註解：ABoRTed command；註解：ABoRTed command |
| 2652 | `!defined(__IO_H__)` | `SET_TK0NF` | `SET_BIT1` | 依原始註解：TracK 0 Not Found；註解：TracK 0 Not Found |
| 2653 | `!defined(__IO_H__)` | `SET_AMNF` | `SET_BIT0` | 依原始註解：Address Mark Not Found；註解：Address Mark Not Found |
| 2654 | `!defined(__IO_H__)` | `SET_ERR` | `SET_BIT0` | 依原始註解：Set	 ATA Command Error；註解：Set	 ATA Command Error |
| 2656 | `!defined(__IO_H__)` | `HB_ERROR` | `0x15` | 通用常數/硬體欄位定義 |
| 2657 | `!defined(__IO_H__)` | `NO_ERROR` | `0x00` | 依原始註解：NO Error；註解：NO Error |
| 2658 | `!defined(__IO_H__)` | `CMD_ABORT` | `0x04` | 依原始註解：COMMAND ABORT；註解：COMMAND ABORT |
| 2659 | `!defined(__IO_H__)` | `ID_ERR` | `0x10` | 依原始註解：sector ID not found；註解：sector ID not found |
| 2660 | `!defined(__IO_H__)` | `DATA_ERR` | `0x40` | 依原始註解：ECC error return;；註解：ECC error return; |
| 2661 | `!defined(__IO_H__)` | `CRC_ERR` | `0x84` | 依原始註解：CRC ERROR；註解：CRC ERROR |
| 2662 | `!defined(__IO_H__)` | `CHK_LBA` | `CHK_BIT6` | 通用常數/硬體欄位定義 |
| 2663 | `!defined(__IO_H__)` | `SET_ICRC` | `SET_BIT7` | 依原始註解：Set Bad BlocK detected；註解：Set Bad BlocK detected |
| 2664 | `!defined(__IO_H__)` | `SET_BBK` | `SET_BIT7` | 依原始註解：Set Bad BlocK detected；註解：Set Bad BlocK detected |
| 2665 | `!defined(__IO_H__)` | `SET_UNC` | `SET_BIT6` | 依原始註解：Set UN-Correctable data；註解：Set UN-Correctable data |
| 2666 | `!defined(__IO_H__)` | `SET_WP` | `SET_BIT6` | 依原始註解：Set write protect for write cmd；註解：Set write protect for write cmd |
| 2667 | `!defined(__IO_H__)` | `SET_MC` | `SET_BIT5` | 依原始註解：Set Media Changed；註解：Set Media Changed |
| 2668 | `!defined(__IO_H__)` | `SET_IDNF` | `SET_BIT4` | 依原始註解：ID Not Found；註解：ID Not Found |
| 2669 | `!defined(__IO_H__)` | `SET_MCR` | `SET_BIT3` | 依原始註解：Media Changed Requested；註解：Media Changed Requested |
| 2670 | `!defined(__IO_H__)` | `SET_ABRT` | `SET_BIT2` | 依原始註解：ABoRTed command；註解：ABoRTed command |
| 2671 | `!defined(__IO_H__)` | `SET_TK0NF` | `SET_BIT1` | 依原始註解：TracK 0 Not Found；註解：TracK 0 Not Found |
| 2672 | `!defined(__IO_H__)` | `SET_AMNF` | `SET_BIT0` | 依原始註解：Address Mark Not Found；註解：Address Mark Not Found |
| 2674 | `!defined(__IO_H__)` | `HB_CONTROL` | `0x16` | 通用常數/硬體欄位定義 |
| 2676 | `!defined(__IO_H__)` | `HL_SACTIVE` | `(0x18 >> 2)` | 通用常數/硬體欄位定義 |
| 2678 | `!defined(__IO_H__)` | `HL_AUXILIARY` | `(0xF8 >> 2)` | 通用常數/硬體欄位定義 |
| 2680 | `!defined(__IO_H__)` | `HB_FLAG_CTRL` | `(0x1D)` | 通用常數/硬體欄位定義 |
| 2681 | `!defined(__IO_H__)` | `H_TSP_DATA_CHK` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2682 | `!defined(__IO_H__)` | `H_ZRF_CHK` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2683 | `!defined(__IO_H__)` | `H_TSP_IDLE_SEL_8_clk_delay` | `~(SET_BIT4\|SET_BIT5)` | 通用常數/硬體欄位定義 |
| 2684 | `!defined(__IO_H__)` | `H_TSP_IDLE_SEL_16_clk_delay` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2685 | `!defined(__IO_H__)` | `H_TSP_IDLE_SEL_32_clk_delay` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2686 | `!defined(__IO_H__)` | `H_TSP_IDLE_SEL_64_clk_delay` | `(SET_BIT4\|SET_BIT5)` | 通用常數/硬體欄位定義 |
| 2687 | `!defined(__IO_H__)` | `H_EXLNK` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2688 | `!defined(__IO_H__)` | `H_SATA_FIFO_CHK` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2689 | `!defined(__IO_H__)` | `H_DDT_EN` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2690 | `!defined(__IO_H__)` | `H_RD_TRG_DONE_SEL` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2692 | `!defined(__IO_H__)` | `HW_CMD_INFO_NCQ_VLD` | `(0x1C>>1)` | 通用常數/硬體欄位定義 |
| 2693 | `!defined(__IO_H__)` | `H_NCQ_VLD_RD_EXIST` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2694 | `!defined(__IO_H__)` | `H_NCQ_VLD_WR_EXIST` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2696 | `!defined(__IO_H__)` | `HW_CMD_INFO` | `(0x1E >> 1)` | 通用常數/硬體欄位定義 |
| 2697 | `!defined(__IO_H__)` | `HB_CMD_INFO_LOWBYTE` | `0x1E` | 通用常數/硬體欄位定義 |
| 2698 | `!defined(__IO_H__)` | `HB_CMD_INFO_HIGHBYTE` | `0x1F` | 通用常數/硬體欄位定義 |
| 2699 | `!defined(__IO_H__)` | `H_NCQ_LBA_OVLP` | `SET_BIT15` | 依原始註解：Bit15: Ending LBA Out of Range: LBA + Sec_Cnt  (RO)；註解：Bit15: Ending LBA Out of Range: LBA + Sec_Cnt  (RO) |
| 2700 | `!defined(__IO_H__)` | `H_LBA_ERR` | `SET_BIT14` | 依原始註解：Bit14: Starting LBA Out of Range: LBA Only       (RO)；註解：Bit14: Starting LBA Out of Range: LBA Only       (RO) |
| 2701 | `!defined(__IO_H__)` | `H_DUP_NCQ_TAG` | `SET_BIT13` | 依原始註解：Bit13: NCQ Duplicate Command Tag Received        (RO)；註解：Bit13: NCQ Duplicate Command Tag Received        (RO) |
| 2702 | `!defined(__IO_H__)` | `H_RCV_NCQ_TAG` | `(SET_BIT8\|SET_BIT9 \|SET_BIT10 \|SET_BIT11 \|SET_BIT12 )` | 通用常數/硬體欄位定義 |
| 2703 | `!defined(__IO_H__)` | `H_MUL_PIO_CMD` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2704 | `!defined(__IO_H__)` | `H_CONT_LBA_CHK_CMD` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2705 | `!defined(__IO_H__)` | `H_LBA_CONT` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2706 | `!defined(__IO_H__)` | `H_2BSEC_CMD` | `SET_BIT4` | 依原始註解：Bit 4: 2 Bytes Sector Count Command Received     (RW)；註解：Bit 4: 2 Bytes Sector Count Command Received     (RW) |
| 2707 | `!defined(__IO_H__)` | `H_LBA48_CMD` | `SET_BIT3` | 依原始註解：Bit 3: LBA28(0)/LBA48(1) Command Received     (RW)；註解：Bit 3: LBA28(0)/LBA48(1) Command Received     (RW) |
| 2708 | `!defined(__IO_H__)` | `H_WR_CMD` | `SET_BIT2` | 依原始註解：Bit 2:  Read(0)/Write(1) Command Received     (RW)；註解：Bit 2:  Read(0)/Write(1) Command Received     (RW) |
| 2709 | `!defined(__IO_H__)` | `H_NCQ_CMD` | `SET_BIT1` | 依原始註解：Bit 1: Non-NCQ(0)/NCQ(1) Command Received     (RW)；註解：Bit 1: Non-NCQ(0)/NCQ(1) Command Received     (RW) |
| 2710 | `!defined(__IO_H__)` | `H_PIO_DMA_CMD` | `SET_BIT0` | 周邊控制暫存器/位元欄位定義；註解：Bit 0:     PIO(0)/DMA(1) Command Received     (RW) |
| 2712 | `!defined(__IO_H__)` | `HB_NCQ_SET` | `0x21` | 通用常數/硬體欄位定義 |
| 2713 | `!defined(__IO_H__)` | `H_NCQ_SET_SEL` | `SET_BIT7` | 依原始註解：Bit7: "1" --> bit 6 and 5 could be writen；註解：Bit7: "1" --> bit 6 and 5 could be writen |
| 2714 | `!defined(__IO_H__)` | `H_NCQ_SET_SEL_CLR` | `CLR_BIT7` | 依原始註解：Bit7: "1" --> bit 6 and 5 could be writen；註解：Bit7: "1" --> bit 6 and 5 could be writen |
| 2715 | `!defined(__IO_H__)` | `H_SDB_FIS_N` | `SET_BIT6` | 依原始註解：Bit6: "N" Bit value in SetDevBit FIS    (RW)；註解：Bit6: "N" Bit value in SetDevBit FIS    (RW) |
| 2716 | `!defined(__IO_H__)` | `H_DMA_SUP_A` | `SET_BIT5` | 周邊控制暫存器/位元欄位定義；註解：Bit5: "A" Bit value in DMA Setup FIS , Auto Active (RW) |
| 2717 | `!defined(__IO_H__)` | `H_DMA_SUP_A_DIS` | `CLR_BIT5` | 周邊控制暫存器/位元欄位定義 |
| 2718 | `!defined(__IO_H__)` | `H_CUR_TAG` | `(SET_BIT0\|SET_BIT1\|SET_BIT2\|SET_BIT3\|SET_BIT4)` | 通用常數/硬體欄位定義 |
| 2721 | `!defined(__IO_H__)` | `HW_SYS_CTRL` | `(0x22 >> 1)` | 依原始註解：Bit4:0		Current Processing TAG	(CURR_TAG)；註解：Bit4:0		Current Processing TAG	(CURR_TAG) |
| 2724 | `!defined(__IO_H__)` | `H_FW_ASR_CLINTCLR` | `SET_BIT15` | 依原始註解：Bit15		FW Send REG_CLINTCLR to clear OOB_MINT & OOB_CINT (): \| Bit14:12	Multiple；註解：Bit15		FW Send REG_CLINTCLR to clear OOB_MINT & OOB_CINT (): \| Bit14:12	Multiple PIO DRQ Size (MUL_PIO_DRQ_SIZE)	(default 0.5KB) |
| 2725 | `!defined(__IO_H__)` | `H_MUL_PIO_DRQ_SIZE_1KB` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2726 | `!defined(__IO_H__)` | `H_MUL_PIO_DRQ_SIZE_2KB` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2727 | `!defined(__IO_H__)` | `H_MUL_PIO_DRQ_SIZE_4KB` | `(SET_BIT13\|SET_BIT12)` | 通用常數/硬體欄位定義 |
| 2728 | `!defined(__IO_H__)` | `H_MUL_PIO_DRQ_SIZE_8KB` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2729 | `!defined(__IO_H__)` | `H_AUTO_CLINTCLR` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2731 | `!defined(__IO_H__)` | `H_REG_GENMAX_GEN1` | `SET_BIT8` | 系統暫存器/記憶體映射位址定義；註解：default GEN3 |
| 2732 | `!defined(__IO_H__)` | `H_REG_GENMAX_GEN2` | `SET_BIT9` | 系統暫存器/記憶體映射位址定義 |
| 2733 | `!defined(__IO_H__)` | `H_REG_GENMAX_GEN3` | `(SET_BIT9\|SET_BIT8)` | 系統暫存器/記憶體映射位址定義 |
| 2734 | `!defined(__IO_H__)` | `H_LBA48_IGR_CHS` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2735 | `!defined(__IO_H__)` | `H_WR_MSGQ_EN` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2736 | `!defined(__IO_H__)` | `H_BLK_FIS_EN` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2737 | `!defined(__IO_H__)` | `H_EC_CNTQ_EN` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2738 | `!defined(__IO_H__)` | `H_DONE_TAGQ_EN` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2739 | `!defined(__IO_H__)` | `H_DONE_TAG_MODE` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2740 | `!defined(__IO_H__)` | `H_REG_SATA_EN` | `SET_BIT1` | 系統暫存器/記憶體映射位址定義 |
| 2741 | `!defined(__IO_H__)` | `H_LZSS` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2743 | `!defined(__IO_H__)` | `HW_DATA_TRG` | `(0x24 >> 1)` | 通用常數/硬體欄位定義 |
| 2744 | `!defined(__IO_H__)` | `HB_DATA_TRG` | `0x24` | 通用常數/硬體欄位定義 |
| 2745 | `!defined(__IO_H__)` | `H_HOLD_NCQ_DIS` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 2746 | `!defined(__IO_H__)` | `H_HOLD_NCQ_EN` | `CLR_BIT15` | 通用常數/硬體欄位定義 |
| 2747 | `!defined(__IO_H__)` | `H_SATA_FIFO_HOLD_THR_42DWS` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2748 | `!defined(__IO_H__)` | `H_SATA_FIFO_HOLD_THR_44DWS` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2749 | `!defined(__IO_H__)` | `H_SATA_FIFO_HOLD_THR_46DWS` | `(SET_BIT11\|SET_BIT10)` | 通用常數/硬體欄位定義 |
| 2750 | `!defined(__IO_H__)` | `H_CMD_ABRTREQ` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2751 | `!defined(__IO_H__)` | `H_CMD_ENDREQ` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2752 | `!defined(__IO_H__)` | `H_DMA_PAUSED` | `SET_BIT7` | 周邊控制暫存器/位元欄位定義；註解：H_DMA_PAUSE move to 0xA0 Bit8 |
| 2753 | `!defined(__IO_H__)` | `H_TRG_INFO` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2754 | `!defined(__IO_H__)` | `H_READ_VFY_TRG` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2755 | `!defined(__IO_H__)` | `H_APU_RST` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2756 | `!defined(__IO_H__)` | `H_DATA_TRG` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2759 | `!defined(__IO_H__)` | `HW_AUTO_FIS_CTRL` | `(0x26 >> 1)` | 通用常數/硬體欄位定義 |
| 2761 | `!defined(__IO_H__)` | `H_NONDATA_RETRY_SET_4T` | `SET_BIT14` | 依原始註解：default 0 time；註解：default 0 time |
| 2762 | `!defined(__IO_H__)` | `H_NONDATA_RETRY_SET_8T` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 2763 | `!defined(__IO_H__)` | `H_NONDATA_RETRY_SET_NONSTOP` | `(SET_BIT15\|SET_BIT14)` | 通用常數/硬體欄位定義 |
| 2764 | `!defined(__IO_H__)` | `H_WR_BLK_EN` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2765 | `!defined(__IO_H__)` | `H_E_STATUS_SEL_8h50` | `CLR_BIT12` | 通用常數/硬體欄位定義 |
| 2766 | `!defined(__IO_H__)` | `H_E_STATUS_SEL_8hD0` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2767 | `!defined(__IO_H__)` | `H_NCQ_ASR_XHOLD_EN` | `CLR_BIT11` | 通用常數/硬體欄位定義 |
| 2768 | `!defined(__IO_H__)` | `H_NCQ_ASR_XHOLD_DIS` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2769 | `!defined(__IO_H__)` | `H_DERR_ASR_APU_RST_EN` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2770 | `!defined(__IO_H__)` | `H_DERR_ASR_APU_RST_DIS` | `CLR_BIT10` | 通用常數/硬體欄位定義 |
| 2771 | `!defined(__IO_H__)` | `H_SRST_ASR_APU_RST_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2772 | `!defined(__IO_H__)` | `H_SRST_ASR_APU_RST_DIS` | `CLR_BIT9` | 通用常數/硬體欄位定義 |
| 2773 | `!defined(__IO_H__)` | `H_COMRI_ASR_APU_RST_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2774 | `!defined(__IO_H__)` | `H_COMRI_ASR_APU_RST_DIS` | `CLR_BIT8` | 通用常數/硬體欄位定義 |
| 2776 | `!defined(__IO_H__)` | `H_NCQ_AUTO_REG_EN` | `SET_BIT5` | 系統暫存器/記憶體映射位址定義；註解：default	response with R_ERR |
| 2777 | `!defined(__IO_H__)` | `H_NCQ_AUTO_REG_DIS` | `CLR_BIT5` | 系統暫存器/記憶體映射位址定義 |
| 2778 | `!defined(__IO_H__)` | `H_NCQ_AUTO_SDB_W_EN` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2779 | `!defined(__IO_H__)` | `H_NCQ_AUTO_SDB_W_DIS` | `CLR_BIT4` | 通用常數/硬體欄位定義 |
| 2780 | `!defined(__IO_H__)` | `H_NCQ_AUTO_SDB_R_EN` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2781 | `!defined(__IO_H__)` | `H_NCQ_AUTO_SDB_R_DIS` | `CLR_BIT3` | 通用常數/硬體欄位定義 |
| 2782 | `!defined(__IO_H__)` | `H_DMA_AUTO_REG_W_EN` | `SET_BIT2` | 系統暫存器/記憶體映射位址定義 |
| 2783 | `!defined(__IO_H__)` | `H_DMA_AUTO_REG_W_DIS` | `CLR_BIT2` | 系統暫存器/記憶體映射位址定義 |
| 2784 | `!defined(__IO_H__)` | `H_DMA_AUTO_REG_R_EN` | `SET_BIT1` | 系統暫存器/記憶體映射位址定義 |
| 2785 | `!defined(__IO_H__)` | `H_DMA_AUTO_REG_R_DIS` | `CLR_BIT1` | 系統暫存器/記憶體映射位址定義 |
| 2786 | `!defined(__IO_H__)` | `H_PIO_AUTO_REG_W_EN` | `SET_BIT0` | 系統暫存器/記憶體映射位址定義 |
| 2787 | `!defined(__IO_H__)` | `H_PIO_AUTO_REG_W_DIS` | `CLR_BIT0` | 系統暫存器/記憶體映射位址定義 |
| 2788 | `!defined(__IO_H__)` | `REG_ALL_AUTO_FIS_EN` | `(0x001f)` | 通用常數/硬體欄位定義 |
| 2789 | `!defined(__IO_H__)` | `REG_ALL_AUTO_FIS_DIS` | `(~0x001f)` | 通用常數/硬體欄位定義 |
| 2791 | `!defined(__IO_H__)` | `HL_BUF_TRG_SEC` | `(0x28 >> 2)` | 通用常數/硬體欄位定義 |
| 2793 | `!defined(__IO_H__)` | `HL_CMD_REMAIN_SEC` | `(0x2C >> 2)` | 通用常數/硬體欄位定義 |
| 2795 | `!defined(__IO_H__)` | `HL_BUF_BASE` | `(0x30 >> 2)` | 通用常數/硬體欄位定義 |
| 2796 | `!defined(__IO_H__)` | `HL_BUF_BASE2` | `(0x328 >> 2)` | 通用常數/硬體欄位定義 |
| 2805 | `!defined(__IO_H__)` | `HL_BUF_OFST` | `(0x34 >> 2)` | 依原始註解：: \| F_FFFFh: 32'hFFFF_F000 \| Unit:4KB；註解：: \| F_FFFFh: 32'hFFFF_F000 \| Unit:4KB |
| 2807 | `!defined(__IO_H__)` | `HL_BUF_R_EP` | `(0x2E0 >> 2)` | 通用常數/硬體欄位定義 |
| 2809 | `!defined(__IO_H__)` | `HL_BUF_W_EP` | `(0x2E4 >> 2)` | 通用常數/硬體欄位定義 |
| 2811 | `!defined(__IO_H__)` | `HL_BUF_R_SHF` | `(0x2E8 >> 2)` | 通用常數/硬體欄位定義 |
| 2813 | `!defined(__IO_H__)` | `HL_BUF_W_SHF` | `(0x2EC >> 2)` | 通用常數/硬體欄位定義 |
| 2815 | `!defined(__IO_H__)` | `HL_BUF_CUR_OFST` | `(0x2D4 >> 2)` | 通用常數/硬體欄位定義 |
| 2817 | `!defined(__IO_H__)` | `HW_TRIM` | `(0x38 >> 1)` | 通用常數/硬體欄位定義 |
| 2818 | `!defined(__IO_H__)` | `H_TRIM_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2819 | `!defined(__IO_H__)` | `H_TRIM_DIS` | `CLR_BIT0` | 通用常數/硬體欄位定義 |
| 2820 | `!defined(__IO_H__)` | `H_TRIM_ERR` | `SET_BIT8` | 依原始註解：W1C；註解：W1C |
| 2822 | `!defined(__IO_H__)` | `HB_REG_REQ` | `0x3A` | 系統暫存器/記憶體映射位址定義 |
| 2823 | `!defined(__IO_H__)` | `H_REG_REQ` | `SET_BIT0` | 系統暫存器/記憶體映射位址定義 |
| 2824 | `!defined(__IO_H__)` | `H_REG_IBIT` | `SET_BIT1` | 系統暫存器/記憶體映射位址定義 |
| 2825 | `!defined(__IO_H__)` | `H_REG_IBIT_CLR` | `CLR_BIT1` | 系統暫存器/記憶體映射位址定義 |
| 2827 | `!defined(__IO_H__)` | `HB_SDB_REQ` | `0x3B` | 通用常數/硬體欄位定義 |
| 2828 | `!defined(__IO_H__)` | `H_SDB_REQ` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2829 | `!defined(__IO_H__)` | `H_SDB_IBIT` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2831 | `!defined(__IO_H__)` | `HW_ERR_CTRL` | `(0x3C >> 1)` | 通用常數/硬體欄位定義 |
| 2832 | `!defined(__IO_H__)` | `H_FORCE_HOLD_EN` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2833 | `!defined(__IO_H__)` | `H_FORCE_HOLD_DIS` | `CLR_BIT14` | 通用常數/硬體欄位定義 |
| 2834 | `!defined(__IO_H__)` | `H_SMART_TMR_CTRL_EN` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2835 | `!defined(__IO_H__)` | `H_SMART_TMR_CTRL_DIS` | `CLR_BIT13` | 通用常數/硬體欄位定義 |
| 2836 | `!defined(__IO_H__)` | `H_CLR_DUP_CMD_LOG` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2837 | `!defined(__IO_H__)` | `H_UPD_L_CMD_ONLY` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2838 | `!defined(__IO_H__)` | `H_BLK_TRG` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2839 | `!defined(__IO_H__)` | `H_BLK_TRG_DIS` | `CLR_BIT10` | 通用常數/硬體欄位定義 |
| 2840 | `!defined(__IO_H__)` | `H_DIS_EXCEED_DATA` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2841 | `!defined(__IO_H__)` | `H_FORCE_SIZE_ERR` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2842 | `!defined(__IO_H__)` | `H_REG_MASK_DMAT` | `SET_BIT7` | 系統暫存器/記憶體映射位址定義 |
| 2843 | `!defined(__IO_H__)` | `H_REG_XHOLD` | `SET_BIT6` | 系統暫存器/記憶體映射位址定義 |
| 2844 | `!defined(__IO_H__)` | `H_REG_XHOLD_CLR` | `CLR_BIT6` | 系統暫存器/記憶體映射位址定義 |
| 2845 | `!defined(__IO_H__)` | `H_REG_CRC_DIR_RECV` | `SET_BIT4` | 系統暫存器/記憶體映射位址定義 |
| 2846 | `!defined(__IO_H__)` | `H_REG_CRC_DIR_SENT` | `CLR_BIT4` | 系統暫存器/記憶體映射位址定義 |
| 2847 | `!defined(__IO_H__)` | `H_REG_CRC_DATAERR_EN` | `SET_BIT3` | 系統暫存器/記憶體映射位址定義 |
| 2848 | `!defined(__IO_H__)` | `H_REG_CRC_DATAERR_DIS` | `CLR_BIT3` | 系統暫存器/記憶體映射位址定義 |
| 2849 | `!defined(__IO_H__)` | `H_REG_CRC_REGERR_EN` | `SET_BIT2` | 系統暫存器/記憶體映射位址定義 |
| 2850 | `!defined(__IO_H__)` | `H_REG_CRC_REGERR_DIS` | `CLR_BIT2` | 系統暫存器/記憶體映射位址定義 |
| 2851 | `!defined(__IO_H__)` | `H_REG_CONTOFF_EN` | `SET_BIT1` | 系統暫存器/記憶體映射位址定義 |
| 2852 | `!defined(__IO_H__)` | `H_REG_CONTOFF_DIS` | `CLR_BIT1` | 系統暫存器/記憶體映射位址定義 |
| 2855 | `!defined(__IO_H__)` | `HW_SATA_INFO` | `(0x3E >> 1)` | 通用常數/硬體欄位定義 |
| 2856 | `!defined(__IO_H__)` | `LNK_ESTSPD` | `(SET_BIT15\|SET_BIT14)` | 通用常數/硬體欄位定義 |
| 2857 | `!defined(__IO_H__)` | `LNK_ESTSPD_1_5GB` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2858 | `!defined(__IO_H__)` | `LNK_ESTSPD_3GB` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 2859 | `!defined(__IO_H__)` | `LNK_ESTSPD_6GB` | `(SET_BIT15\|SET_BIT14)` | 通用常數/硬體欄位定義 |
| 2860 | `!defined(__IO_H__)` | `H_COMRI_ACT` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2861 | `!defined(__IO_H__)` | `H_HW_DENY_DIPM` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2862 | `!defined(__IO_H__)` | `H_APU_RST_STATUS` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2863 | `!defined(__IO_H__)` | `H_MCLKMSK` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2864 | `!defined(__IO_H__)` | `H_LNK_ESTABLISH` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2865 | `!defined(__IO_H__)` | `H_PLLLOCK` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2866 | `!defined(__IO_H__)` | `H_TRS_IDLEST` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2867 | `!defined(__IO_H__)` | `H_LNK_IDLEST` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2868 | `!defined(__IO_H__)` | `H_LNK_PHYRDY` | `SET_BIT5` | 依原始註解：Bit 5:  Current PHYRDY State(RO)；註解：Bit 5:  Current PHYRDY State(RO) |
| 2869 | `!defined(__IO_H__)` | `H_LNK_BUSYDENY` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2870 | `!defined(__IO_H__)` | `H_LNK_PMDENY` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2871 | `!defined(__IO_H__)` | `H_SLUMBER` | `SET_BIT2` | 依原始註解：Bit 2:  In Slumber State (RO)；註解：Bit 2:  In Slumber State (RO) |
| 2872 | `!defined(__IO_H__)` | `H_PARTIAL` | `SET_BIT1` | 依原始註解：Bit 1:  In Partial State (RO)；註解：Bit 1:  In Partial State (RO) |
| 2873 | `!defined(__IO_H__)` | `H_DEVSLP` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2875 | `!defined(__IO_H__)` | `HL_ERR_INT_EN` | `(0x40 >> 2)` | 通用常數/硬體欄位定義 |
| 2876 | `!defined(__IO_H__)` | `H_CMD_TMO_INT_EN` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 2877 | `!defined(__IO_H__)` | `H_CMD_TMO_INT_DIS` | `CLR_BIT31` | 通用常數/硬體欄位定義 |
| 2878 | `!defined(__IO_H__)` | `H_PHY_EVT_CNT_OV_INT_EN` | `SET_BIT30` | 通用常數/硬體欄位定義 |
| 2879 | `!defined(__IO_H__)` | `H_RD_UNC_INT_EN` | `SET_BIT29` | 通用常數/硬體欄位定義 |
| 2880 | `!defined(__IO_H__)` | `H_RCV_UNEXP_CMD_INT_EN` | `SET_BIT28` | 通用常數/硬體欄位定義 |
| 2881 | `!defined(__IO_H__)` | `H_SRAM_UNOVFLOW_INT_EN` | `SET_BIT27` | 通用常數/硬體欄位定義 |
| 2882 | `!defined(__IO_H__)` | `H_FIFO_UNOVFLOW_INT_EN` | `SET_BIT26` | 通用常數/硬體欄位定義 |
| 2883 | `!defined(__IO_H__)` | `H_TRS_DRERREND_INT_EN` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 2884 | `!defined(__IO_H__)` | `H_TRS_FISACK_SYNC_INT_EN` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 2885 | `!defined(__IO_H__)` | `H_TRS_DACK_SIZERR_INT_EN` | `SET_BIT22` | 通用常數/硬體欄位定義 |
| 2886 | `!defined(__IO_H__)` | `H_TRS_DACK_DMAT_INT_EN` | `SET_BIT21` | 周邊控制暫存器/位元欄位定義 |
| 2887 | `!defined(__IO_H__)` | `H_TRS_SYNCRCV_INT_EN` | `SET_BIT20` | 通用常數/硬體欄位定義 |
| 2888 | `!defined(__IO_H__)` | `H_H2D_ND_RERR_NCRC_INT_EN` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 2889 | `!defined(__IO_H__)` | `H_H2D_ND_RERR_CRC_INT_EN` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 2890 | `!defined(__IO_H__)` | `H_H2D_D_RERR_NCRC_INT_EN` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 2891 | `!defined(__IO_H__)` | `H_H2D_D_RERR_CRC_INT_EN` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 2892 | `!defined(__IO_H__)` | `H_H2D_RERR_NCRC_INT_EN` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 2893 | `!defined(__IO_H__)` | `H_H2D_RERR_CRC_INT_EN` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2894 | `!defined(__IO_H__)` | `H_D2H_ND_RETRY_INT_EN` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2895 | `!defined(__IO_H__)` | `H_H2D_ND_RERR_INT_EN` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2896 | `!defined(__IO_H__)` | `H_D2H_ND_RERR_INT_EN` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2897 | `!defined(__IO_H__)` | `H_H2D_D_RERR_INT_EN` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2898 | `!defined(__IO_H__)` | `H_D2H_D_RERR_INT_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2899 | `!defined(__IO_H__)` | `H_TRS_DF_ERR_INT_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2900 | `!defined(__IO_H__)` | `H_TRS_DT_ERR_INT_EN` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2901 | `!defined(__IO_H__)` | `H_LNK_DS_ERR_INT_EN` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2902 | `!defined(__IO_H__)` | `H_LNK_DH_ERR_INT_EN` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2903 | `!defined(__IO_H__)` | `H_LNK_NOCOMERR_INT_EN` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2904 | `!defined(__IO_H__)` | `H_LNK_DD_ERR_INT_EN` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2905 | `!defined(__IO_H__)` | `H_LNK_DB_ERR_INT_EN` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2906 | `!defined(__IO_H__)` | `H_RD_UNC_4K_INT_EN` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2907 | `!defined(__IO_H__)` | `H_E3D_INT_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2915 | `!defined(__IO_H__)` | `DATAFIS_CRC` | `(H_D2H_D_RERR_INT_EN\|H_H2D_D_RERR_INT_EN\|H_H2D_RERR_CRC_INT_EN\|H_H2D_D_RERR_CRC_INT_EN\|H_H2D_D_RERR_NCRC_INT_EN)` | 依原始註解：CRC 0   0   0   0   0   0   0   1   1   0   1   0   0   0   1   1   0   0   0   ；註解：CRC 0   0   0   0   0   0   0   1   1   0   1   0   0   0   1   1   0   0   0   0   0   0   0   0   0 \| 0               0               3               4               6               0               0 |
| 2922 | `!defined(__IO_H__)` | `FORCE_ESCAPE` | `(H_TRS_SYNCRCV_INT_EN\|H_TRS_DACK_DMAT_INT_EN\|H_TRS_FISACK_SYNC_INT_EN)` | 依原始註解：0               b               0               0               0               ；註解：0               b               0               0               0               0               0 |
| 2925 | `!defined(__IO_H__)` | `HL_ERR_INT_FLAG` | `(0x44 >> 2)` | 通用常數/硬體欄位定義 |
| 2926 | `!defined(__IO_H__)` | `H_CMD_TMO_INT` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 2927 | `!defined(__IO_H__)` | `H_PHY_EVT_CNT_OV_INT` | `SET_BIT30` | 通用常數/硬體欄位定義 |
| 2928 | `!defined(__IO_H__)` | `H_RD_UNC_INT` | `SET_BIT29` | 通用常數/硬體欄位定義 |
| 2929 | `!defined(__IO_H__)` | `H_RCV_UNEXP_CMD_INT` | `SET_BIT28` | 通用常數/硬體欄位定義 |
| 2930 | `!defined(__IO_H__)` | `H_SRAM_UNOVFLOW_INT` | `SET_BIT27` | 通用常數/硬體欄位定義 |
| 2931 | `!defined(__IO_H__)` | `H_FIFO_UNOVFLOW_INT` | `SET_BIT26` | 通用常數/硬體欄位定義 |
| 2932 | `!defined(__IO_H__)` | `H_TRS_DRERREND_INT` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 2933 | `!defined(__IO_H__)` | `H_TRS_FISACK_SYNC_INT` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 2934 | `!defined(__IO_H__)` | `H_TRS_DACK_SIZERR_INT` | `SET_BIT22` | 通用常數/硬體欄位定義 |
| 2935 | `!defined(__IO_H__)` | `H_TRS_DACK_DMAT_INT` | `SET_BIT21` | 周邊控制暫存器/位元欄位定義 |
| 2936 | `!defined(__IO_H__)` | `H_TRS_SYNCRCV_INT` | `SET_BIT20` | 通用常數/硬體欄位定義 |
| 2937 | `!defined(__IO_H__)` | `H_H2D_ND_RERR_NCRC_INT` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 2938 | `!defined(__IO_H__)` | `H_H2D_ND_RERR_CRC_INT` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 2939 | `!defined(__IO_H__)` | `H_H2D_D_RERR_NCRC_INT` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 2940 | `!defined(__IO_H__)` | `H_H2D_D_RERR_CRC_INT` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 2941 | `!defined(__IO_H__)` | `H_H2D_RERR_NCRC_INT` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 2942 | `!defined(__IO_H__)` | `H_H2D_RERR_CRC_INT` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2943 | `!defined(__IO_H__)` | `H_D2H_ND_RETRY_INT` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 2944 | `!defined(__IO_H__)` | `H_H2D_ND_RERR_INT` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2945 | `!defined(__IO_H__)` | `H_D2H_ND_RERR_INT` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2946 | `!defined(__IO_H__)` | `H_H2D_D_RERR_INT` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2947 | `!defined(__IO_H__)` | `H_D2H_D_RERR_INT` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2948 | `!defined(__IO_H__)` | `H_TRS_DF_ERR_INT` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2949 | `!defined(__IO_H__)` | `H_TRS_DT_ERR_INT` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2950 | `!defined(__IO_H__)` | `H_LNK_DS_ERR_INT` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 2951 | `!defined(__IO_H__)` | `H_LNK_DH_ERR_INT` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2952 | `!defined(__IO_H__)` | `H_LNK_NOCOMERR_INT` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2953 | `!defined(__IO_H__)` | `H_LNK_DD_ERR_INT` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2954 | `!defined(__IO_H__)` | `H_LNK_DB_ERR_INT` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2955 | `!defined(__IO_H__)` | `H_RD_UNC_4K_INT` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 2956 | `!defined(__IO_H__)` | `H_E3D_INT` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 2958 | `!defined(__IO_H__)` | `HL_HAT_MISC` | `(0x48 >> 2)	/* Need to Change */` | 通用常數/硬體欄位定義 |
| 2961 | `!defined(__IO_H__)` | `H_DIPM_BLK_EN` | `SET_BIT13` | 依原始註解：Bit31:16	HW Auto Trigger Disable Idle Timer	(HW_AUTO_TRIG_IDLE_THR) \| Bit15:12	R；註解：Bit31:16	HW Auto Trigger Disable Idle Timer	(HW_AUTO_TRIG_IDLE_THR) \| Bit15:12	Reserved (Prohibited) |
| 2962 | `!defined(__IO_H__)` | `H_TQ_TRG_DLY_SEL_1ms` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2963 | `!defined(__IO_H__)` | `H_TQ_BY_ORDER` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2964 | `!defined(__IO_H__)` | `H_PHYRDYN_CLR_HAT_N` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2965 | `!defined(__IO_H__)` | `H_HAT_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2966 | `!defined(__IO_H__)` | `H_HAT_DIS` | `CLR_BIT8` | 通用常數/硬體欄位定義 |
| 2967 | `!defined(__IO_H__)` | `H_LAST_CMD_TYPE` | `SET_BIT6\|SET_BIT7` | 通用常數/硬體欄位定義 |
| 2968 | `!defined(__IO_H__)` | `H_OOB_RCV_INT_SEL_RISING` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 2969 | `!defined(__IO_H__)` | `H_OOB_RCV_INT_SEL_FALLING` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 2970 | `!defined(__IO_H__)` | `H_AUTO_SDB_SEL` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 2971 | `!defined(__IO_H__)` | `H_PHY_EVT_CNT_00A_CTRL` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 2972 | `!defined(__IO_H__)` | `H_DMA_ACT_FIS_BLK_EN` | `SET_BIT1` | 周邊控制暫存器/位元欄位定義 |
| 2973 | `!defined(__IO_H__)` | `H_DMA_ACT_FIS_BLK_DIS` | `CLR_BIT1` | 周邊控制暫存器/位元欄位定義 |
| 2974 | `!defined(__IO_H__)` | `H_BUF_NEARFUL_REG_OUT_EN` | `SET_BIT0` | 系統暫存器/記憶體映射位址定義 |
| 2975 | `!defined(__IO_H__)` | `H_BUF_NEARFUL_REG_OUT_DIS` | `CLR_BIT0` | 系統暫存器/記憶體映射位址定義 |
| 2977 | `!defined(__IO_H__)` | `HW_HAT_IDLE_THR` | `(0x4A >> 1)` | 通用常數/硬體欄位定義 |
| 2979 | `!defined(__IO_H__)` | `HW_PM_CTRL` | `(0x4C >> 1)` | 通用常數/硬體欄位定義 |
| 2980 | `!defined(__IO_H__)` | `H_REG_APSREQ` | `SET_BIT15` | 系統暫存器/記憶體映射位址定義 |
| 2981 | `!defined(__IO_H__)` | `H_HW_AUTO_DIPM_EN` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 2982 | `!defined(__IO_H__)` | `H_HW_AUTO_DIPM_DIS` | `CLR_BIT14` | 通用常數/硬體欄位定義 |
| 2983 | `!defined(__IO_H__)` | `H_HW_AUTO_DIPM_P` | `SET_BIT13` | 依原始註解：0 is Slumber, 1 is Partial；註解：0 is Slumber, 1 is Partial |
| 2984 | `!defined(__IO_H__)` | `H_HW_AUTO_DIPM_S` | `CLR_BIT13` | 通用常數/硬體欄位定義 |
| 2985 | `!defined(__IO_H__)` | `H_CR_OOB_WAKE_EN` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 2986 | `!defined(__IO_H__)` | `H_CR_OOB_WAKE_DIS` | `CLR_BIT12` | 通用常數/硬體欄位定義 |
| 2987 | `!defined(__IO_H__)` | `H_CR_OOBN_IPMACK_EN` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 2988 | `!defined(__IO_H__)` | `H_CR_OOBN_IPMACK_DIS` | `CLR_BIT11` | 通用常數/硬體欄位定義 |
| 2989 | `!defined(__IO_H__)` | `H_FORCE_IPM_NAK` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 2990 | `!defined(__IO_H__)` | `H_COMRI_ACT_DENY_IPM` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 2991 | `!defined(__IO_H__)` | `H_AUTO_ACK_HIPM` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 2992 | `!defined(__IO_H__)` | `H_IPM_EN` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 2993 | `!defined(__IO_H__)` | `H_IPM_DIS` | `CLR_BIT7` | 通用常數/硬體欄位定義 |
| 2994 | `!defined(__IO_H__)` | `H_REG_PMCANCEL` | `SET_BIT6` | 系統暫存器/記憶體映射位址定義 |
| 2995 | `!defined(__IO_H__)` | `H_REG_PMREQ_S` | `SET_BIT5` | 系統暫存器/記憶體映射位址定義 |
| 2996 | `!defined(__IO_H__)` | `H_REG_PMREQ_P` | `SET_BIT4` | 系統暫存器/記憶體映射位址定義 |
| 2997 | `!defined(__IO_H__)` | `H_REG_PMNAK_S` | `SET_BIT3` | 系統暫存器/記憶體映射位址定義 |
| 2998 | `!defined(__IO_H__)` | `H_REG_PMACK_S` | `SET_BIT2` | 系統暫存器/記憶體映射位址定義 |
| 2999 | `!defined(__IO_H__)` | `H_REG_PMNAK_P` | `SET_BIT1` | 系統暫存器/記憶體映射位址定義 |
| 3000 | `!defined(__IO_H__)` | `H_REG_PMACK_P` | `SET_BIT0` | 系統暫存器/記憶體映射位址定義 |
| 3002 | `!defined(__IO_H__)` | `HW_DIPM_TIMER_THR` | `(0x4E >> 1)` | 周邊控制暫存器/位元欄位定義 |
| 3005 | `!defined(__IO_H__)` | `H_DIPM_AUTO_TMR_UNIT_SEL_us` | `CLR_BIT7` | 依原始註解：bit 15:8		DIPM auto wake from Partial and enter Slumber timer Threshold	(DIPM_AU；註解：bit 15:8		DIPM auto wake from Partial and enter Slumber timer Threshold	(DIPM_AUTO_WAKE_THEN_SLUMBER_THR) \| DIPM Auto Timer Unit Select (DIPM_AUTO_TMR_UNIT_SEL_US) 	is defined in SPEC	 0:us	1:ms \| Bit 7:  IPM  Enable (RW) , default IPM ON |
| 3006 | `!defined(__IO_H__)` | `H_DIPM_AUTO_TMR_UNIT_SEL_ms` | `SET_BIT7` | 依原始註解：Bit 7:  IPM Disable (RW)；註解：Bit 7:  IPM Disable (RW) |
| 3009 | `!defined(__IO_H__)` | `HB_DIPM_AUTO_P2S_THR` | `(0x4F )` | 依原始註解：bit 6:0		DIPM Auto Timer Unit Select	(DIPM_AUTO_TMR_UNIT_SEL)；註解：bit 6:0		DIPM Auto Timer Unit Select	(DIPM_AUTO_TMR_UNIT_SEL) |
| 3011 | `!defined(__IO_H__)` | `HW_AUTOP2S_THR` | `(0x52 >> 1)` | 通用常數/硬體欄位定義 |
| 3013 | `!defined(__IO_H__)` | `HW_CONT_LBA_TAG` | `(0x50 >> 1)` | 通用常數/硬體欄位定義 |
| 3015 | `!defined(__IO_H__)` | `H_CUR_TRG_NCQ_CONT` | `CHK_BIT8` | 依原始註解：Bit15:9		Reserved；註解：Bit15:9		Reserved |
| 3019 | `!defined(__IO_H__)` | `HL_CMD_TSF_SEC` | `(0x54 >> 2)` | 依原始註解：Bit7:5		Reserved \| Bit4:0		H_CONT_LBA_TAG；註解：Bit7:5		Reserved \| Bit4:0		H_CONT_LBA_TAG |
| 3021 | `!defined(__IO_H__)` | `HL_LAST_END_LBA` | `(0x58 >> 2)` | 通用常數/硬體欄位定義 |
| 3023 | `!defined(__IO_H__)` | `HL_MAX_LBA_CHS` | `(0x5C >> 2)` | 通用常數/硬體欄位定義 |
| 3025 | `!defined(__IO_H__)` | `HL_MAX_LBA` | `(0x60 >> 2)` | 通用常數/硬體欄位定義 |
| 3027 | `!defined(__IO_H__)` | `HB_MAX_HEAD` | `0x66` | 通用常數/硬體欄位定義 |
| 3031 | `!defined(__IO_H__)` | `HB_MAX_SECTOR` | `0x67` | 依原始註解：Bit7:5		Reserved \| Bit4:0		Maximum Head	(MAX_HEAD)；註解：Bit7:5		Reserved \| Bit4:0		Maximum Head	(MAX_HEAD) |
| 3033 | `!defined(__IO_H__)` | `HW_MAX_CYLINDER` | `(0x68 >> 1)` | 通用常數/硬體欄位定義 |
| 3035 | `!defined(__IO_H__)` | `HW_CMD_LBA_INFO` | `(0x6a >> 1)` | 通用常數/硬體欄位定義 |
| 3036 | `!defined(__IO_H__)` | `H_AES_LBA_ERR` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 3037 | `!defined(__IO_H__)` | `H_LBA_MODE` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 3038 | `!defined(__IO_H__)` | `H_LBA_E_ERR` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 3039 | `!defined(__IO_H__)` | `H_LBA_S_ERR` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 3040 | `!defined(__IO_H__)` | `H_LBA28_E_ERR` | `SET_BIT4` | 依原始註解：END address out of range；註解：END address out of range |
| 3041 | `!defined(__IO_H__)` | `H_LBA28_S_ERR` | `SET_BIT3` | 依原始註解：Start address out of range；註解：Start address out of range |
| 3042 | `!defined(__IO_H__)` | `H_C2L_E_ERR` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 3043 | `!defined(__IO_H__)` | `H_C2L_S_ERR` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3044 | `!defined(__IO_H__)` | `H_C2L_ACT` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3045 | `!defined(__IO_H__)` | `LBA_Command_Out_of_Range` | `(H_LBA28_S_ERR\|H_LBA28_E_ERR\|H_LBA_S_ERR\|H_LBA_E_ERR)` | 通用常數/硬體欄位定義 |
| 3046 | `!defined(__IO_H__)` | `CHS_Command_Out_of_Range` | `(H_C2L_S_ERR\|H_C2L_E_ERR)` | 通用常數/硬體欄位定義 |
| 3048 | `!defined(__IO_H__)` | `HL_CMD_S_LBA` | `(0x6c >> 2)` | 通用常數/硬體欄位定義 |
| 3050 | `!defined(__IO_H__)` | `HB_AES_CTRL` | `0x70` | 通用常數/硬體欄位定義 |
| 3053 | `!defined(__IO_H__)` | `H_AES_OTF_FCK` | `SET_BIT3` | 依原始註解：Bit7:4		Reserved \| Bit3:1		AES Key Index (AES_KEY_IDX)；註解：Bit7:4		Reserved \| Bit3:1		AES Key Index (AES_KEY_IDX) |
| 3054 | `!defined(__IO_H__)` | `H_AES_OTF_CK_EN` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 3055 | `!defined(__IO_H__)` | `H_AES_CHK_LBA_EN` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3056 | `!defined(__IO_H__)` | `H_AES_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3057 | `!defined(__IO_H__)` | `H_AES_DIS` | `CLR_BIT0` | 通用常數/硬體欄位定義 |
| 3060 | `!defined(__IO_H__)` | `HW_AXI_CTRL` | `(0x72 >> 1)` | 通用常數/硬體欄位定義 |
| 3062 | `!defined(__IO_H__)` | `H_TRG_DATA_EQ_DW` | `SET_BIT11` | 依原始註解：Bit15:9		Reserved；註解：Bit15:9		Reserved |
| 3063 | `!defined(__IO_H__)` | `H_TRG_DATA_EQ_W` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3064 | `!defined(__IO_H__)` | `H_TRG_DATA_EQ_B` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3065 | `!defined(__IO_H__)` | `H_FORCE_APU_RST_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3066 | `!defined(__IO_H__)` | `H_FORCE_APU_RST_DIS` | `CLR_BIT8` | 通用常數/硬體欄位定義 |
| 3067 | `!defined(__IO_H__)` | `H_WR_DLY_FIFO_TSF` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 3068 | `!defined(__IO_H__)` | `H_WR_WAIT_AXI_FLUSH` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 3069 | `!defined(__IO_H__)` | `H_AXI_WR_BURST_EN` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3070 | `!defined(__IO_H__)` | `H_AXI_WR_BURST_DIS` | `CLR_BIT1` | 通用常數/硬體欄位定義 |
| 3071 | `!defined(__IO_H__)` | `H_AXI_RD_BURST_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3072 | `!defined(__IO_H__)` | `H_AXI_RD_BURST_DIS` | `CLR_BIT0` | 通用常數/硬體欄位定義 |
| 3074 | `!defined(__IO_H__)` | `HL_RD_UNC_ADDR` | `(0x74 >> 2)` | 通用常數/硬體欄位定義 |
| 3076 | `!defined(__IO_H__)` | `HL_MZ0_EQ_DATA` | `(0x78 >> 2)` | 通用常數/硬體欄位定義 |
| 3078 | `!defined(__IO_H__)` | `HL_MZ1_EQ_DATA` | `(0x7C >> 2)` | 通用常數/硬體欄位定義 |
| 3080 | `!defined(__IO_H__)` | `HL_MZ2_EQ_DATA` | `(0x80 >> 2)` | 通用常數/硬體欄位定義 |
| 3082 | `!defined(__IO_H__)` | `HL_MZ0_SEC_ZERO` | `(0x84 >> 2)` | 通用常數/硬體欄位定義 |
| 3084 | `!defined(__IO_H__)` | `HL_MZ0_SEC_ZERO` | `(0x84 >> 2)` | 通用常數/硬體欄位定義 |
| 3086 | `!defined(__IO_H__)` | `HL_MZ1_SEC_ZERO` | `(0x88 >> 2)` | 通用常數/硬體欄位定義 |
| 3088 | `!defined(__IO_H__)` | `HL_MZ2_SEC_ZERO` | `(0x8C >> 2)` | 通用常數/硬體欄位定義 |
| 3090 | `!defined(__IO_H__)` | `HL_MZ0_COM_INFO` | `(0x90 >> 2)` | 通用常數/硬體欄位定義 |
| 3091 | `!defined(__IO_H__)` | `H_MZ0_INFO_VLD` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 3092 | `!defined(__IO_H__)` | `H_MZ0_EQ` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3093 | `!defined(__IO_H__)` | `H_MZ0_COM_N` | `0x3FFFF` | 通用常數/硬體欄位定義 |
| 3095 | `!defined(__IO_H__)` | `HL_MZ1_COM_INFO` | `(0x94 >> 2)` | 通用常數/硬體欄位定義 |
| 3096 | `!defined(__IO_H__)` | `H_MZ1_INFO_VLD` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 3097 | `!defined(__IO_H__)` | `H_MZ1_EQ` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3098 | `!defined(__IO_H__)` | `H_MZ1_COM_N` | `0x3FFFF` | 通用常數/硬體欄位定義 |
| 3100 | `!defined(__IO_H__)` | `HL_MZ2_COM_INFO` | `(0x98 >> 2)` | 通用常數/硬體欄位定義 |
| 3101 | `!defined(__IO_H__)` | `H_MZ2_INFO_VLD` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 3102 | `!defined(__IO_H__)` | `H_MZ2_EQ` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3103 | `!defined(__IO_H__)` | `H_MZ2_COM_N` | `0x3FFFF` | 通用常數/硬體欄位定義 |
| 3105 | `!defined(__IO_H__)` | `HL_CMD_ZERO_INFO` | `(0x9C >> 2)` | 通用常數/硬體欄位定義 |
| 3107 | `!defined(__IO_H__)` | `HW_MZ_CTRL` | `(0xA0 >> 1)` | 通用常數/硬體欄位定義 |
| 3108 | `!defined(__IO_H__)` | `H_E3D_ERR` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 3109 | `!defined(__IO_H__)` | `H_TRG_ALL_ZERO` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 3110 | `!defined(__IO_H__)` | `H_UNC_TQ_IDLE` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 3111 | `!defined(__IO_H__)` | `H_TQ_WR_OVLP_EN` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3112 | `!defined(__IO_H__)` | `H_TQ_OVLP_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3113 | `!defined(__IO_H__)` | `H_DMA_PAUSE` | `SET_BIT8` | 周邊控制暫存器/位元欄位定義 |
| 3114 | `!defined(__IO_H__)` | `H_MZ_DONE_INT_Q2E` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 3115 | `!defined(__IO_H__)` | `H_MZ_DONE_INT_Q3` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 3116 | `!defined(__IO_H__)` | `H_E3D_EN` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 3117 | `!defined(__IO_H__)` | `H_E3D_DIS` | `CLR_BIT5` | 通用常數/硬體欄位定義 |
| 3118 | `!defined(__IO_H__)` | `H_E3D_BLK_DATA` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 3119 | `!defined(__IO_H__)` | `H_MZ_PAUSE` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 3120 | `!defined(__IO_H__)` | `H_MZ_ABORT` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 3121 | `!defined(__IO_H__)` | `H_MZ_DIR` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3122 | `!defined(__IO_H__)` | `H_MZ_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3124 | `!defined(__IO_H__)` | `HB_MZ_LEN` | `(0xA3 )` | 通用常數/硬體欄位定義 |
| 3126 | `!defined(__IO_H__)` | `HL_MZ_BASE` | `(0xA4 >> 2)` | 通用常數/硬體欄位定義 |
| 3128 | `!defined(__IO_H__)` | `HL_MZ0_NUM` | `(0xA8 >> 2)` | 通用常數/硬體欄位定義 |
| 3129 | `!defined(__IO_H__)` | `H_MZ0_VLD` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3131 | `!defined(__IO_H__)` | `HL_MZ1_NUM` | `(0xAC >> 2)` | 通用常數/硬體欄位定義 |
| 3132 | `!defined(__IO_H__)` | `H_MZ1_VLD` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3134 | `!defined(__IO_H__)` | `HL_MZ2_NUM` | `(0xB0 >> 2)` | 通用常數/硬體欄位定義 |
| 3135 | `!defined(__IO_H__)` | `H_MZ2_VLD` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3137 | `!defined(__IO_H__)` | `HL_RD_UNC_LBA` | `(0xB4 >> 2)` | 通用常數/硬體欄位定義 |
| 3139 | `!defined(__IO_H__)` | `HB_MZ_TSF_BLK` | `(0xB8)` | 通用常數/硬體欄位定義 |
| 3140 | `!defined(__IO_H__)` | `H_MZ_TSF_BLK` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3142 | `!defined(__IO_H__)` | `HL_MZ_VLD_SEC` | `(0xBC >> 2)` | 通用常數/硬體欄位定義 |
| 3144 | `!defined(__IO_H__)` | `HL_AES_CHK_LBA_S` | `(0xE8 >> 2)` | 通用常數/硬體欄位定義 |
| 3146 | `!defined(__IO_H__)` | `HL_AES_CHK_LBA_E` | `(0xEC >> 2)` | 通用常數/硬體欄位定義 |
| 3148 | `!defined(__IO_H__)` | `HB_AES_CHK_LBA_TRG` | `(0xF0 )` | 通用常數/硬體欄位定義 |
| 3149 | `!defined(__IO_H__)` | `H_AES_CHK_LBA_TRG` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3151 | `!defined(__IO_H__)` | `HB_AES_CHK_LBA_GRP` | `(0xF1 )` | 通用常數/硬體欄位定義 |
| 3152 | `!defined(__IO_H__)` | `H_AES_CHK_LBA_GRP` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3154 | `!defined(__IO_H__)` | `HB_AES_CHK_LBA_SUM` | `(0xF2 )` | 通用常數/硬體欄位定義 |
| 3155 | `!defined(__IO_H__)` | `H_AES_CHK_LBA_SUM` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3157 | `!defined(__IO_H__)` | `HL_AES_CHK_LBA_LEN` | `(0xF4 >> 2)` | 通用常數/硬體欄位定義 |
| 3159 | `!defined(__IO_H__)` | `HB_W_HOLD_THR` | `(0x2DC )` | 通用常數/硬體欄位定義 |
| 3161 | `!defined(__IO_H__)` | `HL_ZRF_BASE` | `(0x2F0 >> 2)` | 通用常數/硬體欄位定義 |
| 3163 | `!defined(__IO_H__)` | `HL_R_SEC_SUM` | `(0x2F8 >> 2)` | 通用常數/硬體欄位定義 |
| 3165 | `!defined(__IO_H__)` | `HL_W_SEC_SUM` | `(0x2FC >> 2)` | 通用常數/硬體欄位定義 |
| 3167 | `!defined(__IO_H__)` | `HL_R_SUM` | `(0x320 >> 2)` | 通用常數/硬體欄位定義 |
| 3169 | `!defined(__IO_H__)` | `HL_W_SUM` | `(0x324 >> 2)` | 通用常數/硬體欄位定義 |
| 3171 | `!defined(__IO_H__)` | `HB_NCQ_RD_SER` | `(0xC0 )` | 通用常數/硬體欄位定義 |
| 3173 | `!defined(__IO_H__)` | `HB_FLH_CN` | `(0xC1 )` | 通用常數/硬體欄位定義 |
| 3174 | `!defined(__IO_H__)` | `H_FLH_CN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3176 | `!defined(__IO_H__)` | `HB_TAG_F_CH0` | `(0xC4 )` | 通用常數/硬體欄位定義 |
| 3178 | `!defined(__IO_H__)` | `HB_TAG_F_CH1` | `(0xC5 )` | 通用常數/硬體欄位定義 |
| 3180 | `!defined(__IO_H__)` | `HB_TAG_F_CH2` | `(0xC6 )` | 通用常數/硬體欄位定義 |
| 3182 | `!defined(__IO_H__)` | `HB_TAG_F_CH3` | `(0xC7 )` | 通用常數/硬體欄位定義 |
| 3184 | `!defined(__IO_H__)` | `HB_TAG_F_CH4` | `(0xC8 )` | 通用常數/硬體欄位定義 |
| 3186 | `!defined(__IO_H__)` | `HB_TAG_F_CH5` | `(0xC9 )` | 通用常數/硬體欄位定義 |
| 3188 | `!defined(__IO_H__)` | `HB_TAG_F_CH6` | `(0xCA )` | 通用常數/硬體欄位定義 |
| 3190 | `!defined(__IO_H__)` | `HB_TAG_F_CH7` | `(0xCB )` | 通用常數/硬體欄位定義 |
| 3192 | `!defined(__IO_H__)` | `HL_NCQ_LBA_OVLP_TAG` | `(0xCC >> 2)` | 通用常數/硬體欄位定義 |
| 3194 | `!defined(__IO_H__)` | `HL_NCQ_VLD_BIT` | `(0xD0 >> 2)` | 位元遮罩/旗標定義 |
| 3196 | `!defined(__IO_H__)` | `HL_VLD_NCQ_NUM` | `(0xD4 >> 2)` | 通用常數/硬體欄位定義 |
| 3198 | `!defined(__IO_H__)` | `HB_REMAIN_VLD_NCQ_NUM` | `0xD4` | 通用常數/硬體欄位定義 |
| 3199 | `!defined(__IO_H__)` | `HB_REMAIN_VLD_TQ_NUM` | `0xD5` | 通用常數/硬體欄位定義 |
| 3200 | `!defined(__IO_H__)` | `HB_QD1_CNT` | `0xD6` | 通用常數/硬體欄位定義 |
| 3201 | `!defined(__IO_H__)` | `HB_NCQ_OVLP_NUM` | `0xD7` | 通用常數/硬體欄位定義 |
| 3203 | `!defined(__IO_H__)` | `HL_TQ_CTRL` | `(0xD8 >> 2)` | 通用常數/硬體欄位定義 |
| 3204 | `!defined(__IO_H__)` | `H_TQ_CHK_SEC_NUM` | `0xD8` | 通用常數/硬體欄位定義 |
| 3205 | `!defined(__IO_H__)` | `H_TQ_FORCE_CHK_SEC` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3206 | `!defined(__IO_H__)` | `NCQ_Q_CMD_CHK_SEC_CNT_EN_DIS` | `CLR_BIT8` | 通用常數/硬體欄位定義 |
| 3207 | `!defined(__IO_H__)` | `H_ACC_SDB_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3208 | `!defined(__IO_H__)` | `H_ACC_SDB_DIS` | `CLR_BIT9` | 通用常數/硬體欄位定義 |
| 3209 | `!defined(__IO_H__)` | `H_TQ_FORCE_CHK_TRG` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3210 | `!defined(__IO_H__)` | `HB_ACC_SDB_NUM` | `0xDA` | 通用常數/硬體欄位定義 |
| 3211 | `!defined(__IO_H__)` | `HB_ACC_SDB_IDLE` | `0xDB` | 通用常數/硬體欄位定義 |
| 3213 | `!defined(__IO_H__)` | `HW_NCQ_DLY_FIS` | `(0xDC >> 1)` | 通用常數/硬體欄位定義 |
| 3214 | `!defined(__IO_H__)` | `H_DLY_SDB_FIS_EN` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 3215 | `!defined(__IO_H__)` | `H_DLY_SDB_FIS_DIS` | `CLR_BIT6` | 通用常數/硬體欄位定義 |
| 3216 | `!defined(__IO_H__)` | `H_DLY_DMA_SUP_FIS_EN` | `SET_BIT7` | 周邊控制暫存器/位元欄位定義 |
| 3217 | `!defined(__IO_H__)` | `H_DLY_DMA_SUP_FIS_DIS` | `CLR_BIT7` | 周邊控制暫存器/位元欄位定義 |
| 3219 | `!defined(__IO_H__)` | `HB_NCQ_DLY_CMD_THR` | `0xDD` | 通用常數/硬體欄位定義 |
| 3221 | `!defined(__IO_H__)` | `HW_TQ_TRG_DLY` | `(0xDE >>1)` | 通用常數/硬體欄位定義 |
| 3223 | `!defined(__IO_H__)` | `HW_CMD_REMAIN_EC` | `(0xE0 >> 1)` | 通用常數/硬體欄位定義 |
| 3225 | `!defined(__IO_H__)` | `NCQ_LBA_Base` | `(SATA_REG+0x100)` | 通用常數/硬體欄位定義 |
| 3226 | `!defined(__IO_H__)` | `NCQ_SEC_Base` | `(SATA_REG+0x180)` | 通用常數/硬體欄位定義 |
| 3227 | `!defined(__IO_H__)` | `HL_NCQ_LBA` | `(0x100>>2)` | 通用常數/硬體欄位定義 |
| 3228 | `!defined(__IO_H__)` | `HL_NCQ_INFO` | `(0x180>>2)` | 通用常數/硬體欄位定義 |
| 3229 | `!defined(__IO_H__)` | `HL_NCQ_AUXILIARY` | `(0x500>>2)` | 通用常數/硬體欄位定義 |
| 3230 | `!defined(__IO_H__)` | `HW_NCQ_ENTRY_COUNT` | `(0x340>>1)` | 通用常數/硬體欄位定義 |
| 3232 | `!defined(__IO_H__)` | `NLL` | `((volatile U32 *)NCQ_LBA_Base)` | 通用常數/硬體欄位定義 |
| 3235 | `!defined(__IO_H__)` | `NSL` | `((volatile U32 *)NCQ_SEC_Base)` | 依原始註解：31:0 H_NCQN_LBA；註解：31:0 H_NCQN_LBA |
| 3236 | `!defined(__IO_H__)` | `H_NCQN_DEVICE` | `0xFF000000` | 通用常數/硬體欄位定義 |
| 3237 | `!defined(__IO_H__)` | `H_NCQN_EOFST` | `0x00700000` | 通用常數/硬體欄位定義 |
| 3238 | `!defined(__IO_H__)` | `H_NCQN_FUA` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 3239 | `!defined(__IO_H__)` | `H_NCQN_PRIO` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 3240 | `!defined(__IO_H__)` | `H_NCQN_WR` | `SET_BIT18` | 依原始註解：1: write CMD; 0: Read CMD；註解：1: write CMD; 0: Read CMD |
| 3241 | `!defined(__IO_H__)` | `H_NCQN_VLD` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 3242 | `!defined(__IO_H__)` | `H_NCQN_SEC_CNT` | `0x0000ffff` | 通用常數/硬體欄位定義 |
| 3244 | `!defined(__IO_H__)` | `NAUXILIARY` | `((volatile U32 *)(SATA_REG+0x500))` | 通用常數/硬體欄位定義 |
| 3246 | `!defined(__IO_H__)` | `NEC` | `((volatile UWORD *)(SATA_REG+0x340))` | 通用常數/硬體欄位定義 |
| 3250 | `!defined(__IO_H__)` | `HW_TQ_INFO` | `(0xE2 >> 1)` | 通用常數/硬體欄位定義 |
| 3251 | `!defined(__IO_H__)` | `HB_CUR_TQ_NUM` | `0xE2` | 通用常數/硬體欄位定義 |
| 3252 | `!defined(__IO_H__)` | `HB_TQ_ACT` | `0xE3` | 通用常數/硬體欄位定義 |
| 3253 | `!defined(__IO_H__)` | `H_TQ_ACT` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3255 | `!defined(__IO_H__)` | `HL_TQ_VLD_BITS` | `(0xE4 >> 2)` | 通用常數/硬體欄位定義 |
| 3257 | `!defined(__IO_H__)` | `HL_TQ_DONE_TAG` | `(0xFC >> 2)` | 通用常數/硬體欄位定義 |
| 3259 | `!defined(__IO_H__)` | `HB_TQ_RD_SUM` | `(0x330)` | 通用常數/硬體欄位定義 |
| 3261 | `!defined(__IO_H__)` | `HB_TQ_WR_SUM` | `(0x331)` | 通用常數/硬體欄位定義 |
| 3263 | `!defined(__IO_H__)` | `HL_TQ_VLD_TAG` | `(0x334 >> 2)` | 通用常數/硬體欄位定義 |
| 3265 | `!defined(__IO_H__)` | `HL_TQ_CONTENT` | `(0x380 >> 2)` | 通用常數/硬體欄位定義 |
| 3266 | `!defined(__IO_H__)` | `H_TQN_UPD_PTR` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 3267 | `!defined(__IO_H__)` | `H_BUF_BASE2` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 3268 | `!defined(__IO_H__)` | `H_MULTI_TRG` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 3269 | `!defined(__IO_H__)` | `H_SEND_ZERO_DATA` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 3274 | `!defined(__IO_H__)` | `HL_TQ_CONTENT_31` | `(0x3FC >> 2)` | 依原始註解：22:18 H_TQN_TAG: \| 23    H_TQN_UPD_PTR \| 31:24 H_TQN_EC；註解：22:18 H_TQN_TAG: \| 23    H_TQN_UPD_PTR \| 31:24 H_TQN_EC |
| 3276 | `!defined(__IO_H__)` | `HL_CON_LBA_CHK_LBA` | `(0x48C>>2)` | 通用常數/硬體欄位定義 |
| 3278 | `!defined(__IO_H__)` | `HL_RD_NCQ_INFO` | `(0x490>>2)` | 通用常數/硬體欄位定義 |
| 3279 | `!defined(__IO_H__)` | `HL_WR_NCQ_INFO` | `(0x494>>2)` | 通用常數/硬體欄位定義 |
| 3280 | `!defined(__IO_H__)` | `HB_RD_NCQ_INFO` | `(0x490)` | 通用常數/硬體欄位定義 |
| 3281 | `!defined(__IO_H__)` | `HB_WR_NCQ_INFO` | `(0x494)` | 通用常數/硬體欄位定義 |
| 3282 | `!defined(__IO_H__)` | `H_NCQ_NEXT` | `0x3F00000` | 通用常數/硬體欄位定義 |
| 3283 | `!defined(__IO_H__)` | `H_NCQ_NEXT_WR` | `0xFC000` | 通用常數/硬體欄位定義 |
| 3284 | `!defined(__IO_H__)` | `H_NCQ_NEXT_RD` | `0xFC000` | 通用常數/硬體欄位定義 |
| 3285 | `!defined(__IO_H__)` | `H_NCQ_NEXT_WR_4KALIGN` | `0x3F00` | 通用常數/硬體欄位定義 |
| 3286 | `!defined(__IO_H__)` | `H_NCQ_NEXT_RD_4KALIGN` | `0x3F00` | 通用常數/硬體欄位定義 |
| 3287 | `!defined(__IO_H__)` | `H_CON_LBA_CHK_TRG` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 3288 | `!defined(__IO_H__)` | `H_CON_LBA_CHK_CON` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 3289 | `!defined(__IO_H__)` | `H_CON_LBA_CHK_TAG` | `0x3F` | 通用常數/硬體欄位定義 |
| 3293 | `!defined(__IO_H__)` | `HL_ZINFO_BASE` | `(0x498>>2)` | 通用常數/硬體欄位定義 |
| 3295 | `!defined(__IO_H__)` | `HL_DONE_TAG_BASE` | `(0x49C>>2)` | 通用常數/硬體欄位定義 |
| 3297 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_5_1` | `(0x200 >> 2)` | 通用常數/硬體欄位定義 |
| 3298 | `!defined(__IO_H__)` | `HB_LOG5_FEA` | `(0x200)` | 通用常數/硬體欄位定義 |
| 3299 | `!defined(__IO_H__)` | `HB_LOG5_FEA_EXT` | `(0x201)` | 通用常數/硬體欄位定義 |
| 3300 | `!defined(__IO_H__)` | `HB_LOG5_CNT` | `(0x202)` | 通用常數/硬體欄位定義 |
| 3301 | `!defined(__IO_H__)` | `HB_LOG5_CNT_EXT` | `(0x203)` | 通用常數/硬體欄位定義 |
| 3302 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_5_2` | `(0x204 >> 2)` | 通用常數/硬體欄位定義 |
| 3303 | `!defined(__IO_H__)` | `HB_LOG5_LBAL` | `(0x204)` | 通用常數/硬體欄位定義 |
| 3304 | `!defined(__IO_H__)` | `HB_LOG5_LBAM` | `(0x205)` | 通用常數/硬體欄位定義 |
| 3305 | `!defined(__IO_H__)` | `HB_LOG5_LBAH` | `(0x206)` | 通用常數/硬體欄位定義 |
| 3306 | `!defined(__IO_H__)` | `HB_LOG5_LBAL_EXT` | `(0x207)` | 通用常數/硬體欄位定義 |
| 3307 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_5_3` | `(0x208 >> 2)` | 通用常數/硬體欄位定義 |
| 3308 | `!defined(__IO_H__)` | `HB_LOG5_LBAM_EXT` | `(0x208)` | 通用常數/硬體欄位定義 |
| 3309 | `!defined(__IO_H__)` | `HB_LOG5_LBAH_EXT` | `(0x209)` | 通用常數/硬體欄位定義 |
| 3310 | `!defined(__IO_H__)` | `HB_LOG5_DEV` | `(0x20A)` | 通用常數/硬體欄位定義 |
| 3311 | `!defined(__IO_H__)` | `HB_LOG5_CMD` | `(0x20B)` | 通用常數/硬體欄位定義 |
| 3312 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_5_4` | `(0x20C >> 2)` | 通用常數/硬體欄位定義 |
| 3313 | `!defined(__IO_H__)` | `HB_LOG5_CTL` | `(0x20C)` | 通用常數/硬體欄位定義 |
| 3314 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_4_1` | `(0x210 >> 2)` | 通用常數/硬體欄位定義 |
| 3315 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_4_2` | `(0x214 >> 2)` | 通用常數/硬體欄位定義 |
| 3316 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_4_3` | `(0x218 >> 2)` | 通用常數/硬體欄位定義 |
| 3317 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_4_4` | `(0x21C >> 2)` | 通用常數/硬體欄位定義 |
| 3319 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_3_1` | `(0x220 >> 2)` | 通用常數/硬體欄位定義 |
| 3320 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_3_2` | `(0x224 >> 2)` | 通用常數/硬體欄位定義 |
| 3321 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_3_3` | `(0x228 >> 2)` | 通用常數/硬體欄位定義 |
| 3322 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_3_4` | `(0x22C >> 2)` | 通用常數/硬體欄位定義 |
| 3324 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_2_1` | `(0x230 >> 2)` | 通用常數/硬體欄位定義 |
| 3325 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_2_2` | `(0x234 >> 2)` | 通用常數/硬體欄位定義 |
| 3326 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_2_3` | `(0x238 >> 2)` | 通用常數/硬體欄位定義 |
| 3327 | `!defined(__IO_H__)` | `HB_LOG2_CMD` | `(0x23B)` | 通用常數/硬體欄位定義 |
| 3328 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_2_4` | `(0x23C >> 2)` | 通用常數/硬體欄位定義 |
| 3330 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_1_1` | `(0x240 >> 2)` | 通用常數/硬體欄位定義 |
| 3331 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_1_2` | `(0x244 >> 2)` | 通用常數/硬體欄位定義 |
| 3332 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_1_3` | `(0x248 >> 2)` | 通用常數/硬體欄位定義 |
| 3333 | `!defined(__IO_H__)` | `HL_DUPE_CMD_BLK_1_4` | `(0x24C >> 2)` | 通用常數/硬體欄位定義 |
| 3335 | `!defined(__IO_H__)` | `HL_RCV_CMD_TMR_5` | `(0x250 >> 2)` | 通用常數/硬體欄位定義 |
| 3336 | `!defined(__IO_H__)` | `HL_RCV_CMD_TMR_4` | `{0x254 >> 2)` | 通用常數/硬體欄位定義 |
| 3337 | `!defined(__IO_H__)` | `HL_RCV_CMD_TMR_3` | `(0x258 >> 2)` | 通用常數/硬體欄位定義 |
| 3338 | `!defined(__IO_H__)` | `HL_RCV_CMD_TMR_2` | `(0x25C >> 2)` | 通用常數/硬體欄位定義 |
| 3339 | `!defined(__IO_H__)` | `HL_RCV_CMD_TMR_1` | `(0x260 >> 2)` | 通用常數/硬體欄位定義 |
| 3341 | `!defined(__IO_H__)` | `HL_CUR_SMART_TMR` | `(0x264 >> 2)` | 通用常數/硬體欄位定義 |
| 3343 | `!defined(__IO_H__)` | `HW_SCT_W_TMR` | `(0x268 >> 1)` | 通用常數/硬體欄位定義 |
| 3345 | `!defined(__IO_H__)` | `HW_SCT_R_TMR` | `(0x26A >> 1)` | 通用常數/硬體欄位定義 |
| 3347 | `!defined(__IO_H__)` | `HB_SCT_NCQ_WR_SUM` | `(0x26C)` | 通用常數/硬體欄位定義 |
| 3349 | `!defined(__IO_H__)` | `HB_SCT_NCQ_RD_SUM` | `(0x26D)` | 通用常數/硬體欄位定義 |
| 3351 | `!defined(__IO_H__)` | `HW_SCT_W1_TMR` | `(0x308 >> 1)` | 通用常數/硬體欄位定義 |
| 3353 | `!defined(__IO_H__)` | `HW_SCT_R1_TMR` | `(0x30A >> 1)` | 通用常數/硬體欄位定義 |
| 3355 | `!defined(__IO_H__)` | `HW_SCT_W2_TMR` | `(0x30C >> 1)` | 通用常數/硬體欄位定義 |
| 3357 | `!defined(__IO_H__)` | `HW_SCT_R2_TMR` | `(0x30E >> 1)` | 通用常數/硬體欄位定義 |
| 3359 | `!defined(__IO_H__)` | `HL_SCT_W1_TMO_CNT` | `(0x310 >> 2)` | 通用常數/硬體欄位定義 |
| 3361 | `!defined(__IO_H__)` | `HL_SCT_R1_TMO_CNT` | `(0x314 >> 2)` | 通用常數/硬體欄位定義 |
| 3363 | `!defined(__IO_H__)` | `HL_SCT_W2_TMO_CNT` | `(0x318 >> 2)` | 通用常數/硬體欄位定義 |
| 3365 | `!defined(__IO_H__)` | `HL_SCT_R2_TMO_CNT` | `(0x31C >> 2)` | 通用常數/硬體欄位定義 |
| 3367 | `!defined(__IO_H__)` | `HL_DBG_CTRL` | `(0x270 >> 2)` | 通用常數/硬體欄位定義 |
| 3368 | `!defined(__IO_H__)` | `H_APU_ARC_INFX_DBG_EN` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 3369 | `!defined(__IO_H__)` | `H_DBG_HLDW_SEL` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3370 | `!defined(__IO_H__)` | `H_DBG_PORT_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3371 | `!defined(__IO_H__)` | `H_SCT_HW_BLK_EN` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 3373 | `!defined(__IO_H__)` | `HL_DBG_PORT` | `(0x274 >> 2)` | 通用常數/硬體欄位定義 |
| 3375 | `!defined(__IO_H__)` | `HL_AXI_FIS_STAT` | `(0x278 >> 2)` | 通用常數/硬體欄位定義 |
| 3376 | `!defined(__IO_H__)` | `H_H_LAST_TRG_WR` | `SET_BIT27` | 通用常數/硬體欄位定義 |
| 3377 | `!defined(__IO_H__)` | `H_LAST_TRG_TYPE` | `(SET_BIT25\|SET_BIT26)` | 通用常數/硬體欄位定義 |
| 3378 | `!defined(__IO_H__)` | `H_Receive_SAME_CMD` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3379 | `!defined(__IO_H__)` | `H_PIO_SUP_sending` | `SET_BIT21` | 通用常數/硬體欄位定義 |
| 3380 | `!defined(__IO_H__)` | `H_DMA_ACTIVE_sending` | `SET_BIT20` | 周邊控制暫存器/位元欄位定義 |
| 3381 | `!defined(__IO_H__)` | `H_DMA_SUP_sending` | `SET_BIT19` | 周邊控制暫存器/位元欄位定義 |
| 3382 | `!defined(__IO_H__)` | `H_SDB_sending` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 3383 | `!defined(__IO_H__)` | `H_D2H_sending` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 3384 | `!defined(__IO_H__)` | `H_NCQ_UPD_BUSY` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 3386 | `!defined(__IO_H__)` | `HB_HAT_DLY` | `0x27C` | 依原始註解：HW Auto Trigger Delay, 1 unit is 128T system clock；註解：HW Auto Trigger Delay, 1 unit is 128T system clock |
| 3388 | `!defined(__IO_H__)` | `HB_HAT_CMD_NUM` | `0x27E` | 通用常數/硬體欄位定義 |
| 3390 | `!defined(__IO_H__)` | `HL_PHY_EVT_CNT_1` | `(0x280 >> 2)` | 通用常數/硬體欄位定義 |
| 3391 | `!defined(__IO_H__)` | `HL_PHY_EVT_CNT_2` | `(0x284 >> 2)` | 通用常數/硬體欄位定義 |
| 3392 | `!defined(__IO_H__)` | `HL_PHY_EVT_CNT_3` | `(0x288 >> 2)` | 通用常數/硬體欄位定義 |
| 3394 | `!defined(__IO_H__)` | `HL_PHY_EVT_CNT_CTRL` | `(0x28C >> 2)` | 通用常數/硬體欄位定義 |
| 3396 | `!defined(__IO_H__)` | `HW_NCQ_ORDER_INFO` | `(0x290 >> 1)` | 通用常數/硬體欄位定義 |
| 3397 | `!defined(__IO_H__)` | `H_NCQN_SZ` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3398 | `!defined(__IO_H__)` | `H_NCQN_WRITE` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3399 | `!defined(__IO_H__)` | `H_NCQ_ORDERN_VLD` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3400 | `!defined(__IO_H__)` | `H_NCQN_TAG` | `0x1F` | 通用常數/硬體欄位定義 |
| 3405 | `!defined(__IO_H__)` | `HB_NCQ_ORDER_0_DLY_CNT` | `(0x2D0)` | 依原始註解：4:0  NCQ Order 0 Tag \| 8    NCQ Order 0 Valid \| 9    NCQ Order 0 R/W	0:Read  1:W；註解：4:0  NCQ Order 0 Tag \| 8    NCQ Order 0 Valid \| 9    NCQ Order 0 R/W	0:Read  1:WriteBuffer |
| 3423 | `!defined(__IO_H__)` | `HB_NCQ_ORDER_VLD_SUM` | `(0x2D1)` | 依原始註解：/* #define SATAB_LOG4_FEA      							0x200 #define SATAB_LOG4_FEA_EXT  							0；註解：/* #define SATAB_LOG4_FEA      							0x200 #define SATAB_LOG4_FEA_EXT  							0x201 #define SATAB_LOG4_CNT      							0x202 #define SATAB_LOG4_CNT_EXT  							0x203 #define SATAB_LOG4_LBAL     							0x204 #define SATAB_LOG4_LBAM     							0x205 #define SATAB_LOG4_LBAH     							0x206 #define SATAB_LOG4_LBAL_EXT 							0x207 #define SATAB_LOG4_LBAM_EXT 							0x208 #define SATAB_LOG4_LBAH_EXT 							0x209 #define SATAB_LOG4_DEV      							0x20A #define SATAB_LOG4_CMD      							0x20B #define SATAB_LOG4_CTL      							0x20C */ |
| 3425 | `!defined(__IO_H__)` | `HW_NCQ_ORDER_RD_CHK` | `(0x2D2>>1)` | 通用常數/硬體欄位定義 |
| 3426 | `!defined(__IO_H__)` | `H_NCQ_R_CHK_SZ_SEL` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3427 | `!defined(__IO_H__)` | `H_NCQ_R_CHK_SZ` | `0xFF` | 通用常數/硬體欄位定義 |
| 3429 | `!defined(__IO_H__)` | `HL_E3D_CRC` | `(0x2D8>>2)` | 通用常數/硬體欄位定義 |
| 3430 | `!defined(__IO_H__)` | `HW_CRC_GOLDEN` | `(0x2DA>>1)` | 通用常數/硬體欄位定義 |
| 3431 | `!defined(__IO_H__)` | `HW_CRC_RESULT` | `(0x2D8>>1)` | 通用常數/硬體欄位定義 |
| 3433 | `!defined(__IO_H__)` | `HL_E3D_FAIL_LBA` | `(0x2F4>>2)` | 通用常數/硬體欄位定義 |
| 3435 | `!defined(__IO_H__)` | `HL_AES_GN_S` | `(0x400>>2)` | 通用常數/硬體欄位定義 |
| 3437 | `!defined(__IO_H__)` | `HL_AES_GN_E` | `(0x404>>2)` | 通用常數/硬體欄位定義 |
| 3439 | `!defined(__IO_H__)` | `HL_AES_RLOCK` | `(0x480>>2)` | 通用常數/硬體欄位定義 |
| 3441 | `!defined(__IO_H__)` | `HL_AES_WLOCK` | `(0x484>>2)` | 通用常數/硬體欄位定義 |
| 3443 | `!defined(__IO_H__)` | `HL_PHY_MISC1` | `(0x300>>2)` | 通用常數/硬體欄位定義 |
| 3444 | `!defined(__IO_H__)` | `H_BIST_LBK` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 3445 | `!defined(__IO_H__)` | `H_BIST_EXT` | `SET_BIT22` | 通用常數/硬體欄位定義 |
| 3446 | `!defined(__IO_H__)` | `H_BIST_EN` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 3447 | `!defined(__IO_H__)` | `H_BIST_RST` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 3448 | `!defined(__IO_H__)` | `H_BIST_FORCEPHYRDY` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3449 | `!defined(__IO_H__)` | `H_DISERREN` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 3450 | `!defined(__IO_H__)` | `H_DECERREN` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 3451 | `!defined(__IO_H__)` | `H_BSTERRCLR` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 3452 | `!defined(__IO_H__)` | `H_OOB_STOP` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 3453 | `!defined(__IO_H__)` | `H_WAKE_STOP` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 3455 | `!defined(__IO_H__)` | `HL_PHY_MISC2` | `(0x304>>2)` | 通用常數/硬體欄位定義 |
| 3456 | `!defined(__IO_H__)` | `H_SATA_BIST_ACT` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3457 | `!defined(__IO_H__)` | `HB_BISTHEAD` | `0x305` | 通用常數/硬體欄位定義 |
| 3458 | `!defined(__IO_H__)` | `HW_ERRCNT` | `(0x306>>1)` | 通用常數/硬體欄位定義 |
| 3464 | `!defined(__IO_H__)` | `RSL` | `((volatile unsigned long * ) (0x04000000+0x8000))` | 依原始註解：=================================================== \| PS3111 RS Register Define ；註解：=================================================== \| PS3111 RS Register Define \| =================================================== |
| 3465 | `!defined(__IO_H__)` | `RSB` | `((volatile unsigned char * ) (0x04000000+0x8000))` | 通用常數/硬體欄位定義 |
| 3467 | `!defined(__IO_H__)` | `RSL_MODE_0` | `(0x00>>2)` | 通用常數/硬體欄位定義 |
| 3468 | `!defined(__IO_H__)` | `RS_COR_NORMAL_MODE` | `BIT20` | 通用常數/硬體欄位定義 |
| 3469 | `!defined(__IO_H__)` | `RS_BPT_SEL` | `BIT16` | 通用常數/硬體欄位定義 |
| 3470 | `!defined(__IO_H__)` | `RS_16K_PAGE` | `(BIT9)\|(BIT8)` | 通用常數/硬體欄位定義 |
| 3471 | `!defined(__IO_H__)` | `RS_8K_PAGE` | `BIT9` | 通用常數/硬體欄位定義 |
| 3472 | `!defined(__IO_H__)` | `RS_4K_PAGE` | `BIT8` | 通用常數/硬體欄位定義 |
| 3473 | `!defined(__IO_H__)` | `RS_CLR_PAGE` | `(CLR_BIT8)&(CLR_BIT9)` | 通用常數/硬體欄位定義 |
| 3474 | `!defined(__IO_H__)` | `RS_SAVE_PBUF_XOR` | `BIT4` | 通用常數/硬體欄位定義 |
| 3475 | `!defined(__IO_H__)` | `RS_PBUF_INIT` | `BIT2` | 通用常數/硬體欄位定義 |
| 3476 | `!defined(__IO_H__)` | `RS_PBUF_LOAD` | `BIT1` | 通用常數/硬體欄位定義 |
| 3477 | `!defined(__IO_H__)` | `RS_PBUF_SAVE` | `BIT0` | 通用常數/硬體欄位定義 |
| 3479 | `!defined(__IO_H__)` | `RSL_MODE_1` | `(0x04>>2)` | 通用常數/硬體欄位定義 |
| 3480 | `!defined(__IO_H__)` | `RS_DPAGE_NUM(X)` | `((X&0xFF)<<16)` | 通用常數/硬體欄位定義 |
| 3481 | `!defined(__IO_H__)` | `RS_NMODE_ERR_IDX_NUM(X)` | `((X&0xF)<<8)` | 通用常數/硬體欄位定義 |
| 3482 | `!defined(__IO_H__)` | `RS_ERA_IDX_EN_NUM(X)` | `(X&0x3)` | 通用常數/硬體欄位定義 |
| 3483 | `!defined(__IO_H__)` | `RSB_ERA_IDX_EN_NUM` | `(0x04)` | 通用常數/硬體欄位定義 |
| 3484 | `!defined(__IO_H__)` | `RSB_NMODE_ERR_IDX_NUM` | `(0x05)` | 通用常數/硬體欄位定義 |
| 3485 | `!defined(__IO_H__)` | `RSB_DPAGE_NUM` | `(0x06)` | 通用常數/硬體欄位定義 |
| 3489 | `!defined(__IO_H__)` | `RSL_BP_CONF` | `(0x08>>2)` | 通用常數/硬體欄位定義 |
| 3490 | `!defined(__IO_H__)` | `RS_OFFSET_BYPASS` | `BIT26` | 通用常數/硬體欄位定義 |
| 3491 | `!defined(__IO_H__)` | `RS_CLR_CONT` | `(CLR_BIT25)&(CLR_BIT24)` | 通用常數/硬體欄位定義 |
| 3492 | `!defined(__IO_H__)` | `RS_ONE_CONT` | `BIT25` | 通用常數/硬體欄位定義 |
| 3493 | `!defined(__IO_H__)` | `RS_ALL_CONT` | `BIT24` | 通用常數/硬體欄位定義 |
| 3494 | `!defined(__IO_H__)` | `RSB_ROW_IDX` | `(0x08)` | 通用常數/硬體欄位定義 |
| 3495 | `!defined(__IO_H__)` | `RSB_COL_IDX` | `(0x09)` | 通用常數/硬體欄位定義 |
| 3496 | `!defined(__IO_H__)` | `RSB_PAGE_CNT` | `(0x0A)` | 通用常數/硬體欄位定義 |
| 3498 | `!defined(__IO_H__)` | `RSL_TRIG` | `(0x0C>>2)` | 通用常數/硬體欄位定義 |
| 3499 | `!defined(__IO_H__)` | `RS_TRIG_TBDMA` | `BIT9` | 周邊控制暫存器/位元欄位定義 |
| 3500 | `!defined(__IO_H__)` | `RS_TRIG_BLOCK` | `BIT8` | 通用常數/硬體欄位定義 |
| 3501 | `!defined(__IO_H__)` | `RS_ENC_BSY` | `BIT5` | 通用常數/硬體欄位定義 |
| 3502 | `!defined(__IO_H__)` | `RS_TRIG_BSY` | `BIT4` | 通用常數/硬體欄位定義 |
| 3503 | `!defined(__IO_H__)` | `RS_TRIG_NMODE_WRITE` | `BIT3` | 通用常數/硬體欄位定義 |
| 3504 | `!defined(__IO_H__)` | `RS_TRIG_COR` | `BIT2` | 通用常數/硬體欄位定義 |
| 3505 | `!defined(__IO_H__)` | `RS_TRIG_DEC` | `BIT1` | 通用常數/硬體欄位定義 |
| 3506 | `!defined(__IO_H__)` | `RS_TRIG_ENC` | `BIT0` | 通用常數/硬體欄位定義 |
| 3507 | `!defined(__IO_H__)` | `RSB_TQ_NUM` | `0x0E` | 通用常數/硬體欄位定義 |
| 3509 | `!defined(__IO_H__)` | `RSL_ERA_IDX` | `(0x10>>2)` | 通用常數/硬體欄位定義 |
| 3510 | `!defined(__IO_H__)` | `RSB_ERA_IDX_A` | `(0x10)` | 通用常數/硬體欄位定義 |
| 3511 | `!defined(__IO_H__)` | `RSB_ERA_IDX_B` | `(0x11)` | 通用常數/硬體欄位定義 |
| 3513 | `!defined(__IO_H__)` | `RSL_IP_CTL` | `(0x14>>2)` | 通用常數/硬體欄位定義 |
| 3514 | `!defined(__IO_H__)` | `RSB_IP_CTL` | `(0x14)` | 通用常數/硬體欄位定義 |
| 3515 | `!defined(__IO_H__)` | `RS_ST_CLEAR` | `BIT31` | 通用常數/硬體欄位定義 |
| 3516 | `!defined(__IO_H__)` | `RS_INTQ_EN` | `BIT18` | 通用常數/硬體欄位定義 |
| 3517 | `!defined(__IO_H__)` | `RS_TBDMA_INTR_EN` | `BIT17` | 周邊控制暫存器/位元欄位定義 |
| 3518 | `!defined(__IO_H__)` | `RS_INTR_EN` | `BIT16` | 通用常數/硬體欄位定義 |
| 3520 | `!defined(__IO_H__)` | `RS_TBDMA_INTR` | `BIT5` | 周邊控制暫存器/位元欄位定義 |
| 3521 | `!defined(__IO_H__)` | `RS_BLOCK_INTR` | `BIT4` | 通用常數/硬體欄位定義 |
| 3522 | `!defined(__IO_H__)` | `RS_NMODE_WRITE_INTR` | `BIT3` | 通用常數/硬體欄位定義 |
| 3523 | `!defined(__IO_H__)` | `RS_COR_INTR` | `BIT2` | 通用常數/硬體欄位定義 |
| 3524 | `!defined(__IO_H__)` | `RS_DEC_INTR` | `BIT1` | 通用常數/硬體欄位定義 |
| 3525 | `!defined(__IO_H__)` | `RS_ENC_INTR` | `BIT0` | 通用常數/硬體欄位定義 |
| 3527 | `!defined(__IO_H__)` | `RSL_DBASE0` | `(0x18>>2)` | 通用常數/硬體欄位定義 |
| 3528 | `!defined(__IO_H__)` | `RSL_SBASE0` | `(0x1C>>2)` | 通用常數/硬體欄位定義 |
| 3530 | `!defined(__IO_H__)` | `RSL_ROT_START_0` | `(0x20>>2)` | 通用常數/硬體欄位定義 |
| 3531 | `!defined(__IO_H__)` | `RSL_ROT_LEN_0` | `(0x24>>2)` | 通用常數/硬體欄位定義 |
| 3532 | `!defined(__IO_H__)` | `RSL_ROT_START_1` | `(0x28>>2)` | 通用常數/硬體欄位定義 |
| 3533 | `!defined(__IO_H__)` | `RSL_ROT_LEN_1` | `(0x2C>>2)` | 通用常數/硬體欄位定義 |
| 3535 | `!defined(__IO_H__)` | `RSL_NMODE_IDX_0` | `(0x38>>2)` | 通用常數/硬體欄位定義 |
| 3536 | `!defined(__IO_H__)` | `RSB_NMODE_IDX_0` | `(0x38)` | 通用常數/硬體欄位定義 |
| 3537 | `!defined(__IO_H__)` | `RSB_NMODE_IDX_1` | `(0x39)` | 通用常數/硬體欄位定義 |
| 3538 | `!defined(__IO_H__)` | `RSB_NMODE_IDX_2` | `(0x3A)` | 通用常數/硬體欄位定義 |
| 3539 | `!defined(__IO_H__)` | `RSB_NMODE_IDX_3` | `(0x3B)` | 通用常數/硬體欄位定義 |
| 3541 | `!defined(__IO_H__)` | `RSL_NMODE_IDX_1` | `(0x3C>>2)` | 通用常數/硬體欄位定義 |
| 3542 | `!defined(__IO_H__)` | `RSB_NMODE_IDX_4` | `(0x3C)` | 通用常數/硬體欄位定義 |
| 3543 | `!defined(__IO_H__)` | `RSB_NMODE_IDX_5` | `(0x3D)` | 通用常數/硬體欄位定義 |
| 3544 | `!defined(__IO_H__)` | `RSB_NMODE_IDX_6` | `(0x3E)` | 通用常數/硬體欄位定義 |
| 3545 | `!defined(__IO_H__)` | `RSB_NMODE_IDX_7` | `(0x3F)` | 通用常數/硬體欄位定義 |
| 3547 | `!defined(__IO_H__)` | `RSL_TBDMA_ADDR` | `(0x40>>2)` | 周邊控制暫存器/位元欄位定義 |
| 3549 | `!defined(__IO_H__)` | `RSL_TBDMA_CTL` | `(0x44>>2)` | 周邊控制暫存器/位元欄位定義 |
| 3550 | `!defined(__IO_H__)` | `RSB_TBDMA_LEN` | `(0x44)` | 周邊控制暫存器/位元欄位定義 |
| 3551 | `!defined(__IO_H__)` | `RS_TBDMA_WR` | `BIT12` | 周邊控制暫存器/位元欄位定義；註解：1:save table to DRAM, 0: load table from DRAM |
| 3552 | `!defined(__IO_H__)` | `RS_CLR_TBDMA` | `(CLR_BIT8)&(CLR_BIT9)` | 周邊控制暫存器/位元欄位定義 |
| 3553 | `!defined(__IO_H__)` | `RS_TBDMA_BPT1` | `BIT8` | 周邊控制暫存器/位元欄位定義 |
| 3554 | `!defined(__IO_H__)` | `RS_TBDMA_BSEQ` | `BIT9` | 周邊控制暫存器/位元欄位定義 |
| 3556 | `!defined(__IO_H__)` | `RSL_DBASE1` | `(0x48>>2)` | 通用常數/硬體欄位定義 |
| 3557 | `!defined(__IO_H__)` | `RSL_SBASE1` | `(0x4C>>2)` | 通用常數/硬體欄位定義 |
| 3558 | `!defined(__IO_H__)` | `RSL_DBASE2` | `(0x50>>2)` | 通用常數/硬體欄位定義 |
| 3559 | `!defined(__IO_H__)` | `RSL_SBASE2` | `(0x54>>2)` | 通用常數/硬體欄位定義 |
| 3560 | `!defined(__IO_H__)` | `RSL_DBASE3` | `(0x58>>2)` | 通用常數/硬體欄位定義 |
| 3561 | `!defined(__IO_H__)` | `RSL_SBASE3` | `(0x5C>>2)` | 通用常數/硬體欄位定義 |
| 3563 | `!defined(__IO_H__)` | `RSB_TAG_CONFIG_0` | `(0x60 >> 0)` | 依原始註解：must use assignment (RSB = tag), can't use RSB \|= tag. (due to BIT4 is RW1C)；註解：must use assignment (RSB = tag), can't use RSB \|= tag. (due to BIT4 is RW1C) |
| 3564 | `!defined(__IO_H__)` | `RS_TAG_VALID` | `BIT7` | 通用常數/硬體欄位定義 |
| 3565 | `!defined(__IO_H__)` | `RS_TAG_PPF` | `BIT6` | 依原始註解：RO；註解：RO |
| 3566 | `!defined(__IO_H__)` | `RS_TAG_PRF` | `BIT5` | 依原始註解：RO；註解：RO |
| 3567 | `!defined(__IO_H__)` | `RS_TAG_SEL_READONLY` | `0x1F` | 依原始註解：RO；註解：RO |
| 3569 | `!defined(__IO_H__)` | `RSB_TAG_PRC` | `(0x61 >> 0)` | 通用常數/硬體欄位定義 |
| 3570 | `!defined(__IO_H__)` | `RSB_TAG_PEC` | `(0x62 >> 0)` | 通用常數/硬體欄位定義 |
| 3572 | `!defined(__IO_H__)` | `RSL_TAG_CONFIG_1` | `(0x64 >> 2)` | 通用常數/硬體欄位定義 |
| 3573 | `!defined(__IO_H__)` | `SET_RS_TAG_PBP(X)` | `((U32)X&0xFFFFFFFF)` | 通用常數/硬體欄位定義 |
| 3575 | `!defined(__IO_H__)` | `RSL_PBUF_CTL` | `(0x68 >> 2)` | 通用常數/硬體欄位定義 |
| 3576 | `!defined(__IO_H__)` | `RS_PRE_LOAD_EN` | `BIT27` | 依原始註解：default 1；註解：default 1 |
| 3577 | `!defined(__IO_H__)` | `RS_PRE_SAVE_EN` | `BIT26` | 依原始註解：default 1；註解：default 1 |
| 3578 | `!defined(__IO_H__)` | `RS_RRE_SAVE_LOAD_EN` | `BIT25` | 依原始註解：default 0；註解：default 0 |
| 3579 | `!defined(__IO_H__)` | `RS_FORCE_E3D_ZERO` | `BIT24` | 依原始註解：default 1；註解：default 1 |
| 3580 | `!defined(__IO_H__)` | `RS_FORCE_SAVE_RS_PBUF1` | `BIT19` | 通用常數/硬體欄位定義 |
| 3581 | `!defined(__IO_H__)` | `RS_FORCE_SAVE_RS_PBUF0` | `BIT18` | 通用常數/硬體欄位定義 |
| 3582 | `!defined(__IO_H__)` | `RS_LOCK_RS_PBUF1` | `BIT17` | 通用常數/硬體欄位定義 |
| 3583 | `!defined(__IO_H__)` | `RS_LOCK_RS_PBUF0` | `BIT16` | 通用常數/硬體欄位定義 |
| 3584 | `!defined(__IO_H__)` | `RS_PBUF1_Status` | `BIT15` | 通用常數/硬體欄位定義 |
| 3586 | `!defined(__IO_H__)` | `RS_PBUF1_RS_TAG_NUM` | `(0x1F << 8)` | 依原始註解：BIT14-13	reserved；註解：BIT14-13	reserved |
| 3587 | `!defined(__IO_H__)` | `RS_PBUF0_Status` | `BIT7` | 通用常數/硬體欄位定義 |
| 3589 | `!defined(__IO_H__)` | `RS_PBUF0_RS_TAG_NUM` | `(0x1F)` | 依原始註解：BIT6-5	reserved；註解：BIT6-5	reserved |
| 3591 | `!defined(__IO_H__)` | `RSL_PBP_OFFSET` | `(0x6C >> 2)` | 通用常數/硬體欄位定義 |
| 3592 | `!defined(__IO_H__)` | `SET_RS_PBP_OFFSET(X)` | `((U32)X&0xFFFFFFFF)` | 通用常數/硬體欄位定義 |
| 3594 | `!defined(__IO_H__)` | `RSL_TAG_VLD_CTL` | `(0x70 >> 2)` | 通用常數/硬體欄位定義 |
| 3595 | `!defined(__IO_H__)` | `RS_PRE_SAVE_LOAD_BUSY` | `BIT31` | 通用常數/硬體欄位定義 |
| 3596 | `!defined(__IO_H__)` | `RS_TAG_VALID_CLR` | `BIT8` | 依原始註解：RW1C；註解：RW1C |
| 3598 | `!defined(__IO_H__)` | `RSL_INTQ_BASE` | `(0x74 >> 2)` | 通用常數/硬體欄位定義 |
| 3599 | `!defined(__IO_H__)` | `SET_RS_INTQ_BASE(X)` | `((U32)X&0xFFFFFFFF)` | 通用常數/硬體欄位定義 |
| 3601 | `!defined(__IO_H__)` | `RSL_TAG_SEL` | `(0x78 >> 2)` | 通用常數/硬體欄位定義 |
| 3602 | `!defined(__IO_H__)` | `RS_TAG_SEL` | `(0x1F)` | 通用常數/硬體欄位定義 |
| 3604 | `!defined(__IO_H__)` | `RSL_HANG` | `(0x7C >> 2)` | 通用常數/硬體欄位定義 |
| 3605 | `!defined(__IO_H__)` | `RS_TAG_VALID_SET` | `BIT0` | 通用常數/硬體欄位定義 |
| 3607 | `!defined(__IO_H__)` | `RSL_DONE_FLAG_0` | `(0x80>>2)` | 通用常數/硬體欄位定義 |
| 3608 | `!defined(__IO_H__)` | `RSL_DONE_FLAG_1` | `(0x84>>2)` | 通用常數/硬體欄位定義 |
| 3609 | `!defined(__IO_H__)` | `RSL_DONE_FLAG_2` | `(0x88>>2)` | 通用常數/硬體欄位定義 |
| 3610 | `!defined(__IO_H__)` | `RSL_DONE_FLAG_3` | `(0x8C>>2)` | 通用常數/硬體欄位定義 |
| 3611 | `!defined(__IO_H__)` | `RSL_DONE_FLAG_4` | `(0x90>>2)` | 通用常數/硬體欄位定義 |
| 3613 | `!defined(__IO_H__)` | `RSL_BPT_BASE_0` | `((volatile unsigned long * ) (SYSTEM_REG+0x30000))` | 通用常數/硬體欄位定義 |
| 3614 | `!defined(__IO_H__)` | `RSB_BPT_BASE_0` | `((volatile unsigned char * ) (SYSTEM_REG+0x30000))` | 通用常數/硬體欄位定義 |
| 3615 | `!defined(__IO_H__)` | `RS_BPT0_SIZE` | `4096` | 通用常數/硬體欄位定義 |
| 3617 | `!defined(__IO_H__)` | `RSL_BPT_BASE_1` | `((volatile unsigned long * ) (SYSTEM_REG+0x31000))` | 通用常數/硬體欄位定義 |
| 3618 | `!defined(__IO_H__)` | `RSB_BPT_BASE_1` | `((volatile unsigned char * ) (SYSTEM_REG+0x31000))` | 通用常數/硬體欄位定義 |
| 3619 | `!defined(__IO_H__)` | `RS_BPT1_SIZE` | `4096` | 通用常數/硬體欄位定義 |
| 3625 | `!defined(__IO_H__)` | `Flash_Reg_Base` | `(0x04009000)` | 依原始註解：=================================================== \| PS3111 FLH Module Register；註解：=================================================== \| PS3111 FLH Module Register Define \| =================================================== |
| 3626 | `!defined(__IO_H__)` | `Flash_Top_Reg_Base` | `(0x04009600)` | 通用常數/硬體欄位定義 |
| 3627 | `!defined(__IO_H__)` | `Flash_DLL_Reg_Base` | `(0x04009800)` | 通用常數/硬體欄位定義 |
| 3628 | `!defined(__IO_H__)` | `FIP0` | `0x00000000` | 依原始註解：offset IP0；註解：offset IP0 |
| 3629 | `!defined(__IO_H__)` | `FIP1` | `0x00000200` | 依原始註解：offset IP1；註解：offset IP1 |
| 3630 | `!defined(__IO_H__)` | `FALL` | `0x00000400` | 依原始註解：offset All IP；註解：offset All IP |
| 3631 | `!defined(__IO_H__)` | `FTOP` | `0x00000600` | 依原始註解：offset Top Register；註解：offset Top Register |
| 3632 | `!defined(__IO_H__)` | `FDLL` | `0x00000800` | 依原始註解：offset of Flash Delay Line and mDLL Register；註解：offset of Flash Delay Line and mDLL Register |
| 3634 | `!defined(__IO_H__)` | `FCTLB` | `((_UCV unsigned char *)	(Flash_Reg_Base))` | 通用常數/硬體欄位定義 |
| 3635 | `!defined(__IO_H__)` | `FCTLW` | `((_UCV unsigned short *)(Flash_Reg_Base))` | 通用常數/硬體欄位定義 |
| 3636 | `!defined(__IO_H__)` | `FCTLL` | `((_UCV unsigned long *)(Flash_Reg_Base))` | 通用常數/硬體欄位定義 |
| 3637 | `!defined(__IO_H__)` | `FCTLO` | `((_UCV unsigned long long *)(Flash_Reg_Base))` | 通用常數/硬體欄位定義 |
| 3638 | `!defined(__IO_H__)` | `FALLB` | `((_UCV unsigned char *)(Flash_Reg_Base+FALL))` | 通用常數/硬體欄位定義 |
| 3639 | `!defined(__IO_H__)` | `FALLW` | `((_UCV unsigned short *)(Flash_Reg_Base+FALL))` | 通用常數/硬體欄位定義 |
| 3640 | `!defined(__IO_H__)` | `FALLL` | `((_UCV unsigned long *)(Flash_Reg_Base+FALL))` | 通用常數/硬體欄位定義 |
| 3641 | `!defined(__IO_H__)` | `FALLO` | `((_UCV unsigned long long *)(Flash_Reg_Base+FALL))` | 通用常數/硬體欄位定義 |
| 3642 | `!defined(__IO_H__)` | `FCONB` | `((_UCV unsigned char *)(Flash_Reg_Base+FTOP))` | 通用常數/硬體欄位定義 |
| 3643 | `!defined(__IO_H__)` | `FCONW` | `((_UCV unsigned short *)(Flash_Reg_Base+FTOP))` | 通用常數/硬體欄位定義 |
| 3644 | `!defined(__IO_H__)` | `FCONL` | `((_UCV unsigned long *)(Flash_Reg_Base+FTOP))` | 通用常數/硬體欄位定義 |
| 3645 | `!defined(__IO_H__)` | `FCONO` | `((_UCV unsigned long long *)(Flash_Reg_Base+FTOP))` | 通用常數/硬體欄位定義 |
| 3646 | `!defined(__IO_H__)` | `FDLLB` | `((_UCV unsigned char *)(Flash_Reg_Base+FDLL))` | 通用常數/硬體欄位定義 |
| 3647 | `!defined(__IO_H__)` | `FDLLW` | `((_UCV unsigned short *)(Flash_Reg_Base+FDLL))` | 通用常數/硬體欄位定義 |
| 3648 | `!defined(__IO_H__)` | `FDLLL` | `((_UCV unsigned long *)(Flash_Reg_Base+FDLL))` | 通用常數/硬體欄位定義 |
| 3649 | `!defined(__IO_H__)` | `FDLLO` | `((_UCV unsigned long long *)(Flash_Reg_Base+FDLL))` | 通用常數/硬體欄位定義 |
| 3651 | `!defined(__IO_H__)` | `FDLL_1` | `0x00000A00` | 依原始註解：/*FDLL_1: 2nd DLL Control(For Program)*/ \| 2nd DLL Contorl；註解：/*FDLL_1: 2nd DLL Control(For Program)*/ \| 2nd DLL Contorl |
| 3653 | `!defined(__IO_H__)` | `FDLL_1B` | `((_UCV unsigned char *)(Flash_Reg_Base+FDLL_1))` | 通用常數/硬體欄位定義 |
| 3654 | `!defined(__IO_H__)` | `FDLL_1W` | `((_UCV unsigned short *)(Flash_Reg_Base+FDLL_1))` | 通用常數/硬體欄位定義 |
| 3655 | `!defined(__IO_H__)` | `FDLL_1L` | `((_UCV unsigned long *)(Flash_Reg_Base+FDLL_1))` | 通用常數/硬體欄位定義 |
| 3656 | `!defined(__IO_H__)` | `FDLL_1O` | `((_UCV unsigned long long *)(Flash_Reg_Base+FDLL_1))` | 通用常數/硬體欄位定義 |
| 3659 | `!defined(__IO_H__)` | `CE0` | `0` | 通用常數/硬體欄位定義 |
| 3660 | `!defined(__IO_H__)` | `CE1` | `1` | 通用常數/硬體欄位定義 |
| 3661 | `!defined(__IO_H__)` | `CE2` | `2` | 通用常數/硬體欄位定義 |
| 3662 | `!defined(__IO_H__)` | `CE3` | `3` | 通用常數/硬體欄位定義 |
| 3663 | `!defined(__IO_H__)` | `CE4` | `4` | 通用常數/硬體欄位定義 |
| 3664 | `!defined(__IO_H__)` | `CE5` | `5` | 通用常數/硬體欄位定義 |
| 3665 | `!defined(__IO_H__)` | `CE6` | `6` | 通用常數/硬體欄位定義 |
| 3666 | `!defined(__IO_H__)` | `CE7` | `7` | 通用常數/硬體欄位定義 |
| 3667 | `!defined(__IO_H__)` | `CE8` | `8` | 通用常數/硬體欄位定義 |
| 3668 | `!defined(__IO_H__)` | `CE9` | `9` | 通用常數/硬體欄位定義 |
| 3669 | `!defined(__IO_H__)` | `CE10` | `10` | 通用常數/硬體欄位定義 |
| 3670 | `!defined(__IO_H__)` | `CE11` | `11` | 通用常數/硬體欄位定義 |
| 3671 | `!defined(__IO_H__)` | `CE12` | `12` | 通用常數/硬體欄位定義 |
| 3672 | `!defined(__IO_H__)` | `CE13` | `13` | 通用常數/硬體欄位定義 |
| 3673 | `!defined(__IO_H__)` | `CE14` | `14` | 通用常數/硬體欄位定義 |
| 3674 | `!defined(__IO_H__)` | `CE15` | `15` | 通用常數/硬體欄位定義 |
| 3675 | `!defined(__IO_H__)` | `CE16` | `16` | 通用常數/硬體欄位定義 |
| 3676 | `!defined(__IO_H__)` | `CE17` | `17` | 通用常數/硬體欄位定義 |
| 3677 | `!defined(__IO_H__)` | `CE18` | `18` | 通用常數/硬體欄位定義 |
| 3678 | `!defined(__IO_H__)` | `CE19` | `19` | 通用常數/硬體欄位定義 |
| 3679 | `!defined(__IO_H__)` | `CE20` | `20` | 通用常數/硬體欄位定義 |
| 3680 | `!defined(__IO_H__)` | `CE21` | `21` | 通用常數/硬體欄位定義 |
| 3681 | `!defined(__IO_H__)` | `CE22` | `22` | 通用常數/硬體欄位定義 |
| 3682 | `!defined(__IO_H__)` | `CE23` | `23` | 通用常數/硬體欄位定義 |
| 3683 | `!defined(__IO_H__)` | `CE24` | `24` | 通用常數/硬體欄位定義 |
| 3684 | `!defined(__IO_H__)` | `CE25` | `25` | 通用常數/硬體欄位定義 |
| 3685 | `!defined(__IO_H__)` | `CE26` | `26` | 通用常數/硬體欄位定義 |
| 3686 | `!defined(__IO_H__)` | `CE27` | `27` | 通用常數/硬體欄位定義 |
| 3687 | `!defined(__IO_H__)` | `CE28` | `28` | 通用常數/硬體欄位定義 |
| 3688 | `!defined(__IO_H__)` | `CE29` | `29` | 通用常數/硬體欄位定義 |
| 3689 | `!defined(__IO_H__)` | `CE30` | `30` | 通用常數/硬體欄位定義 |
| 3690 | `!defined(__IO_H__)` | `CE31` | `31` | 通用常數/硬體欄位定義 |
| 3692 | `!defined(__IO_H__)` | `CH0` | `0` | 通用常數/硬體欄位定義 |
| 3693 | `!defined(__IO_H__)` | `CH1` | `1` | 通用常數/硬體欄位定義 |
| 3694 | `!defined(__IO_H__)` | `CH2` | `2` | 通用常數/硬體欄位定義 |
| 3695 | `!defined(__IO_H__)` | `CH3` | `3` | 通用常數/硬體欄位定義 |
| 3696 | `!defined(__IO_H__)` | `CH4` | `4` | 通用常數/硬體欄位定義 |
| 3697 | `!defined(__IO_H__)` | `CH5` | `5` | 通用常數/硬體欄位定義 |
| 3698 | `!defined(__IO_H__)` | `CH6` | `6` | 通用常數/硬體欄位定義 |
| 3699 | `!defined(__IO_H__)` | `CH7` | `7` | 通用常數/硬體欄位定義 |
| 3700 | `!defined(__IO_H__)` | `CH_ALL` | `2` | 通用常數/硬體欄位定義 |
| 3701 | `!defined(__IO_H__)` | `CH_TOP` | `3` | 通用常數/硬體欄位定義 |
| 3705 | `!defined(__IO_H__)` | `FCTLL_PIO_CMD` | `(0x00)` | 依原始註解：=================================================== \| PS3111 FLH CTRL Register D；註解：=================================================== \| PS3111 FLH CTRL Register Define \| =================================================== |
| 3706 | `!defined(__IO_H__)` | `FCTLL_PIO_ADR` | `(0x04>>2)` | 通用常數/硬體欄位定義 |
| 3707 | `!defined(__IO_H__)` | `FCTLL_PIO_DAT` | `(0x08>>2)` | 通用常數/硬體欄位定義 |
| 3708 | `!defined(__IO_H__)` | `FCTLL_DMA_ADR_0` | `(0x0C>>2)` | 周邊控制暫存器/位元欄位定義 |
| 3712 | `!defined(__IO_H__)` | `FCTLL_IO_SET` | `(0x1C>>2)` | 依原始註解：#define FCTLL_DMA_ADR_1		(0x10>>2) \| #define FCTLL_DMA_ADR_2		(0x14>>2) \| #defin；註解：#define FCTLL_DMA_ADR_1		(0x10>>2) \| #define FCTLL_DMA_ADR_2		(0x14>>2) \| #define FCTLL_DMA_ADR_3		(0x18>>2) |
| 3713 | `!defined(__IO_H__)` | `TIE_FLH_WP_HIGH` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 3714 | `!defined(__IO_H__)` | `FLH_PHY_IF_RESET` | `SET_BIT30` | 通用常數/硬體欄位定義 |
| 3715 | `!defined(__IO_H__)` | `RB_NEG_EDGE_MONITOR` | `SET_BIT26` | 通用常數/硬體欄位定義 |
| 3716 | `!defined(__IO_H__)` | `DIFFERENTIAL_IO_TYPE` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 3717 | `!defined(__IO_H__)` | `SINGLE_END_IO_TYPE` | `CLR_BIT25` | 通用常數/硬體欄位定義 |
| 3718 | `!defined(__IO_H__)` | `FORCE_FLA_ALE_HIGH` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3719 | `!defined(__IO_H__)` | `FORCE_FLA_CLE_HIGH` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 3720 | `!defined(__IO_H__)` | `FORCE_FLA_WEN_LOW` | `CLR_BIT22` | 通用常數/硬體欄位定義 |
| 3721 | `!defined(__IO_H__)` | `FORCE_FLA_REN_LOW` | `CLR_BIT21` | 通用常數/硬體欄位定義 |
| 3722 | `!defined(__IO_H__)` | `F_IO_H_GPO_MODE_EN` | `SET_BIT20` | 通用常數/硬體欄位定義 |
| 3723 | `!defined(__IO_H__)` | `F_IO_L_GPO_MODE_EN` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 3725 | `!defined(__IO_H__)` | `FCTLL_ZIP_MAP` | `(0x20>>2)` | 通用常數/硬體欄位定義 |
| 3726 | `!defined(__IO_H__)` | `FCTLL_MT_TRIG` | `(0x24>>2)` | 通用常數/硬體欄位定義 |
| 3727 | `!defined(__IO_H__)` | `MTP_RELOAD` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 3728 | `!defined(__IO_H__)` | `QUEUE_0_BUSY` | `BIT16` | 通用常數/硬體欄位定義 |
| 3729 | `!defined(__IO_H__)` | `CLEAR_QUEUE_7` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 3730 | `!defined(__IO_H__)` | `CLEAR_QUEUE_6` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 3731 | `!defined(__IO_H__)` | `CLEAR_QUEUE_5` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 3732 | `!defined(__IO_H__)` | `CLEAR_QUEUE_4` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 3733 | `!defined(__IO_H__)` | `CLEAR_QUEUE_3` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 3734 | `!defined(__IO_H__)` | `CLEAR_QUEUE_2` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3735 | `!defined(__IO_H__)` | `CLEAR_QUEUE_1` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3736 | `!defined(__IO_H__)` | `CLEAR_QUEUE_0` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3737 | `!defined(__IO_H__)` | `PUSH_FCTL_MT_ADR7_TO_MTQ7` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 3738 | `!defined(__IO_H__)` | `PUSH_FCTL_MT_ADR6_TO_MTQ6` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 3739 | `!defined(__IO_H__)` | `PUSH_FCTL_MT_ADR5_TO_MTQ5` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 3740 | `!defined(__IO_H__)` | `PUSH_FCTL_MT_ADR4_TO_MTQ4` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 3741 | `!defined(__IO_H__)` | `PUSH_FCTL_MT_ADR3_TO_MTQ3` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 3742 | `!defined(__IO_H__)` | `PUSH_FCTL_MT_ADR2_TO_MTQ2` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 3743 | `!defined(__IO_H__)` | `PUSH_FCTL_MT_ADR1_TO_MTQ1` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3744 | `!defined(__IO_H__)` | `PUSH_FCTL_MT_ADR0_TO_MTQ0` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3746 | `!defined(__IO_H__)` | `FCTLL_MT_ADR_0` | `(0x28>>2)` | 通用常數/硬體欄位定義 |
| 3747 | `!defined(__IO_H__)` | `FCTLL_MT_ADR_1` | `(0x2C>>2)` | 通用常數/硬體欄位定義 |
| 3748 | `!defined(__IO_H__)` | `FCTLL_MT_ADR_2` | `(0x30>>2)` | 通用常數/硬體欄位定義 |
| 3749 | `!defined(__IO_H__)` | `FCTLL_MT_ADR_3` | `(0x34>>2)` | 通用常數/硬體欄位定義 |
| 3750 | `!defined(__IO_H__)` | `MTP_FORCE_EMPTY_EN` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 3751 | `!defined(__IO_H__)` | `MTQ_DELAY_FUNC_EN` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 3752 | `!defined(__IO_H__)` | `MTQ_GROUP_LOCK` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 3753 | `!defined(__IO_H__)` | `MTQ_LOW_PRIORITY_G2` | `CLR_BIT16` | 通用常數/硬體欄位定義 |
| 3754 | `!defined(__IO_H__)` | `MTQ_HIGH_PRIORITY_G1` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 3756 | `!defined(__IO_H__)` | `FCTLL_FTA_ROW` | `(0x38>>2)` | 通用常數/硬體欄位定義 |
| 3757 | `!defined(__IO_H__)` | `FCTLL_FTA_COL` | `(0x3C>>2)` | 通用常數/硬體欄位定義 |
| 3758 | `!defined(__IO_H__)` | `FCTLL_DMA_CFG` | `(0x40>>2)` | 周邊控制暫存器/位元欄位定義 |
| 3759 | `!defined(__IO_H__)` | `FLH_BUF_MODE` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 3760 | `!defined(__IO_H__)` | `DIS_QRY_AND_SGN_IN_BUF_MODE` | `SET_BIT30` | 通用常數/硬體欄位定義 |
| 3761 | `!defined(__IO_H__)` | `BYPASS_BVALID_CHECK_WHEN_NVEM_REQ` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 3762 | `!defined(__IO_H__)` | `DQS_GPO_MODE_SWITCH` | `SET_BIT22` | 通用常數/硬體欄位定義 |
| 3763 | `!defined(__IO_H__)` | `SEND_SINGLE_DUMMY_DQS` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 3764 | `!defined(__IO_H__)` | `SEND_DUMMY_DQS_DATA` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 3765 | `!defined(__IO_H__)` | `TRANSFER_0_ECC_FRAME` | `0xFFFFF8FF` | 通用常數/硬體欄位定義 |
| 3766 | `!defined(__IO_H__)` | `TRANSFER_1_ECC_FRAME` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3767 | `!defined(__IO_H__)` | `TRANSFER_4_ECC_FRAME` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3768 | `!defined(__IO_H__)` | `TRANSFER_ECC_FRAME(frameCnt)` | `(((UBYTE)(frameCnt)&0x07)<<8)` | 依原始註解：[10:8]；註解：[10:8] |
| 3769 | `!defined(__IO_H__)` | `ALU_GROUP_SEL(ALU_SEL)` | `(((UBYTE)(ALU_SEL)&0x03)<<4)` | 依原始註解：[5:4]；註解：[5:4] |
| 3770 | `!defined(__IO_H__)` | `ALU0_GROUP_SEL` | `(CLR_BIT5&CLR_BIT4)` | 通用常數/硬體欄位定義 |
| 3771 | `!defined(__IO_H__)` | `ALU1_GROUP_SEL` | `0x00000010` | 通用常數/硬體欄位定義 |
| 3772 | `!defined(__IO_H__)` | `ALU2_GROUP_SEL` | `0x00000020` | 通用常數/硬體欄位定義 |
| 3773 | `!defined(__IO_H__)` | `ALU3_GROUP_SEL` | `0x00000030` | 通用常數/硬體欄位定義 |
| 3774 | `!defined(__IO_H__)` | `SEL_FSA0` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 3775 | `!defined(__IO_H__)` | `SEL_OTHER_FSA` | `CLR_BIT3` | 通用常數/硬體欄位定義 |
| 3777 | `!defined(__IO_H__)` | `ULTRA_DMA_EN` | `SET_BIT1` | 周邊控制暫存器/位元欄位定義；註解：#define ALL_SPARE_AS_SPARE0		SET_BIT2 |
| 3778 | `!defined(__IO_H__)` | `SEL_TIME_CFG_1` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3780 | `!defined(__IO_H__)` | `FCTLL_OTHER_SET` | `(0x44>>2)` | 通用常數/硬體欄位定義 |
| 3781 | `!defined(__IO_H__)` | `FRAME_START_PTR_CLR` | `0xF3FFFFFF` | 通用常數/硬體欄位定義 |
| 3782 | `!defined(__IO_H__)` | `FRAME_START_PTR_0` | `0` | 通用常數/硬體欄位定義 |
| 3783 | `!defined(__IO_H__)` | `FRAME_START_PTR(X)` | `(((UBYTE)(X)&0x03)<<26)` | 通用常數/硬體欄位定義 |
| 3784 | `!defined(__IO_H__)` | `REF_SEED_FROM_OTHER` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 3785 | `!defined(__IO_H__)` | `REF_SEED_VALUE(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 3786 | `!defined(__IO_H__)` | `BRAM_INSERT_ERR` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 3787 | `!defined(__IO_H__)` | `IRAM_INSERT_ERR` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 3788 | `!defined(__IO_H__)` | `BCH_COR_EN` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3789 | `!defined(__IO_H__)` | `MTQ_GROUP_PRIORITY_FUNC_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3790 | `!defined(__IO_H__)` | `FORCE_FIX_DATA_TO_FLH` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3791 | `!defined(__IO_H__)` | `FIX_DATA_VALUE(X)` | `((UBYTE)(X))` | 通用常數/硬體欄位定義 |
| 3794 | `!defined(__IO_H__)` | `FCTLL_FSA_SEL` | `(0x48>>2)` | 通用常數/硬體欄位定義 |
| 3795 | `!defined(__IO_H__)` | `IFSA_EN` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 3796 | `!defined(__IO_H__)` | `FSA2_COUNTER(X)` | `(((UBYTE)(X)&0x03)<<16)` | 通用常數/硬體欄位定義 |
| 3797 | `!defined(__IO_H__)` | `FSA2_SEL(X)` | `(((UBYTE)(X)&0x7F)<<8)` | 通用常數/硬體欄位定義 |
| 3798 | `!defined(__IO_H__)` | `FSA_SEL(X)` | `((UBYTE)(X)&0x7F)` | 通用常數/硬體欄位定義 |
| 3800 | `!defined(__IO_H__)` | `FCTLL_UNC_CFG` | `(0x4C>>2)` | 通用常數/硬體欄位定義 |
| 3801 | `!defined(__IO_H__)` | `FORCE_CRC_FAIL_ALL` | `(0xF << 18)` | 通用常數/硬體欄位定義 |
| 3802 | `!defined(__IO_H__)` | `AUTOGEN_PROGRAM_FAIL_EN` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 3803 | `!defined(__IO_H__)` | `AUTOGEN_READ_FAIL_EN` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 3804 | `!defined(__IO_H__)` | `FORCE_LDPC_OVERTHRESHOLD_FRAME(X)` | `(((UBYTE)(X)&0x0F)<<12)` | 通用常數/硬體欄位定義 |
| 3805 | `!defined(__IO_H__)` | `FORCE_LDPC_UNCORRECTABLE_FRAME(X)` | `(((UBYTE)(X)&0x0F)<<8)` | 通用常數/硬體欄位定義 |
| 3806 | `!defined(__IO_H__)` | `FORCE_BCH_OVERTHRESHOLD_FRAME(X)` | `(((UBYTE)(X)&0x0F)<<4)` | 通用常數/硬體欄位定義 |
| 3807 | `!defined(__IO_H__)` | `FORCE_BCH_UNCORRECTABLE_FRAME(X)` | `((UBYTE)(X)&0x0F)` | 通用常數/硬體欄位定義 |
| 3809 | `!defined(__IO_H__)` | `FCTLL_CNT_ONE` | `(0x50>>2)` | 通用常數/硬體欄位定義 |
| 3810 | `!defined(__IO_H__)` | `COUNT_ONE_MODE` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 3811 | `!defined(__IO_H__)` | `COUNT_MODE_EN` | `SET_BIT30` | 通用常數/硬體欄位定義 |
| 3812 | `!defined(__IO_H__)` | `CNT_MASK` | `0x0001FFFF` | 通用常數/硬體欄位定義 |
| 3814 | `!defined(__IO_H__)` | `FCTLL_ECC_INFO` | `(0x54>>2)` | 通用常數/硬體欄位定義 |
| 3815 | `!defined(__IO_H__)` | `LDPC_MAX_BFC` | `(0x000003FF)` | 通用常數/硬體欄位定義 |
| 3816 | `!defined(__IO_H__)` | `CORRECTABLE` | `0x00` | 通用常數/硬體欄位定義 |
| 3817 | `!defined(__IO_H__)` | `UNCORRECTABLE` | `0x01` | 通用常數/硬體欄位定義 |
| 3819 | `!defined(__IO_H__)` | `FCTLL_INT_VCT` | `(0x58>>2)` | 通用常數/硬體欄位定義 |
| 3820 | `!defined(__IO_H__)` | `IntVector(X)` | `(((U32)(X)&0xFFFFFF)<<8)` | 通用常數/硬體欄位定義 |
| 3821 | `!defined(__IO_H__)` | `ErrorTargetCPU(X)` | `(((UBYTE)(X)&0x07)<<3)` | 通用常數/硬體欄位定義 |
| 3822 | `!defined(__IO_H__)` | `NormalTargetCPU(X)` | `((UBYTE)(X)&0x07))` | 通用常數/硬體欄位定義 |
| 3825 | `!defined(__IO_H__)` | `FCTLL_CMP_CFG` | `(0x60>>2)` | 依原始註解：#define FCTLL_SPR_ADR		(0x5C>>2)；註解：#define FCTLL_SPR_ADR		(0x5C>>2) |
| 3826 | `!defined(__IO_H__)` | `LCA_FW_CMP_PHISON_SPARE_FORMAT` | `CLR_BIT1` | 通用常數/硬體欄位定義 |
| 3827 | `!defined(__IO_H__)` | `LCA_FW_CMP_TOSHIBA_SPARE_FORMAT` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3828 | `!defined(__IO_H__)` | `LCA_FW_COMPARE_EN` | `BIT0` | 通用常數/硬體欄位定義 |
| 3830 | `!defined(__IO_H__)` | `FCTLL_INT_INF` | `(0x64>>2)` | 通用常數/硬體欄位定義 |
| 3831 | `!defined(__IO_H__)` | `CRC16_ERROR` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 3832 | `!defined(__IO_H__)` | `DECOMPRESSION_FAIL_INF` | `SET_BIT30` | 通用常數/硬體欄位定義 |
| 3833 | `!defined(__IO_H__)` | `COMPRESSION_FAIL_INF` | `SET_BIT29` | 通用常數/硬體欄位定義 |
| 3834 | `!defined(__IO_H__)` | `STA_ERR_INF` | `SET_BIT28` | 通用常數/硬體欄位定義 |
| 3835 | `!defined(__IO_H__)` | `LCA_FW_CMP_ERR_INF` | `SET_BIT26` | 通用常數/硬體欄位定義 |
| 3836 | `!defined(__IO_H__)` | `CRC32_ERROR` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 3837 | `!defined(__IO_H__)` | `OVER_ECC_THRESHOLD_INF` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3838 | `!defined(__IO_H__)` | `SIGNOFFMASK_INF` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 3839 | `!defined(__IO_H__)` | `UNC_MAP_INF` | `SET_BIT22` | 通用常數/硬體欄位定義 |
| 3840 | `!defined(__IO_H__)` | `ERASE_PAGE` | `SET_BIT20` | 通用常數/硬體欄位定義 |
| 3841 | `!defined(__IO_H__)` | `FW_SETTING_ERR_INF` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 3842 | `!defined(__IO_H__)` | `E3D_ERR_INF` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 3843 | `!defined(__IO_H__)` | `MAP_FIFO_POINTER_RESET_BYTE1` | `CLR_BIT0` | 通用常數/硬體欄位定義 |
| 3845 | `!defined(__IO_H__)` | `INTR_BUSY` | `SET_BIT0` | 依原始註解：#define MAP_FIFO_POINTER_RESET		CLR_BIT8  // \`N: H Long 覡hMBIT8 \|~ BIT0 g；註解：#define MAP_FIFO_POINTER_RESET		CLR_BIT8  // \`N: H Long 覡hMBIT8 \|~ BIT0 g |
| 3846 | `!defined(__IO_H__)` | `FREE_INTERRUPT` | `CLR_BIT0` | 通用常數/硬體欄位定義 |
| 3848 | `!defined(__IO_H__)` | `FCTLL_MAP_CFG` | `(0x68>>2)` | 通用常數/硬體欄位定義 |
| 3849 | `!defined(__IO_H__)` | `ERASED_PAGE_FLAG_EN` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 3850 | `!defined(__IO_H__)` | `CRC16_FAIL_FAIL_WRITE_EN` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 3851 | `!defined(__IO_H__)` | `DECOMPRESSION_FAIL_WRITE_EN` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 3852 | `!defined(__IO_H__)` | `COMPRESSION_FAIL_WRITE_EN` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 3853 | `!defined(__IO_H__)` | `LCA_FW_CMP_FAIL_WRITE_EN` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 3854 | `!defined(__IO_H__)` | `CRC_ERR_WRITE_EN` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 3855 | `!defined(__IO_H__)` | `OVER_ECC_THRESHOLD_ERR_WRITE_EN` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 3856 | `!defined(__IO_H__)` | `SIGNOFFMASK_WRITE_EN` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3857 | `!defined(__IO_H__)` | `UNC_WRITE_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3858 | `!defined(__IO_H__)` | `INT_VCT_WRITE_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3860 | `!defined(__IO_H__)` | `NEXT_FIFO_HAS_NEW` | `BIT6` | 通用常數/硬體欄位定義 |
| 3861 | `!defined(__IO_H__)` | `CURRENT_FIFO_HAS_NEW` | `BIT5` | 通用常數/硬體欄位定義 |
| 3863 | `!defined(__IO_H__)` | `CLEAR_PINGPONG_FIFO_FRAMEMAP` | `0xFFFFFFE1` | 通用常數/硬體欄位定義 |
| 3864 | `!defined(__IO_H__)` | `PINGPONG_FIFO_FRAMEMAP_SEL(X)` | `(((UBYTE)(X)&0x0F)<<1)` | 通用常數/硬體欄位定義 |
| 3866 | `!defined(__IO_H__)` | `DQSCNT_MISMATCH` | `100` | 依原始註解：=================== FW define ===================；註解：=================== FW define =================== |
| 3867 | `!defined(__IO_H__)` | `ERASE_PAGE_ERR` | `101` | 通用常數/硬體欄位定義 |
| 3869 | `!defined(__IO_H__)` | `E3D_ERR_MAP` | `13` | 依原始註解：=================== HW define ===================；註解：=================== HW define =================== |
| 3870 | `!defined(__IO_H__)` | `FW_SETTING_ERR_MAP` | `12` | 通用常數/硬體欄位定義 |
| 3871 | `!defined(__IO_H__)` | `CRC16_ERR_MAP` | `10` | 通用常數/硬體欄位定義 |
| 3872 | `!defined(__IO_H__)` | `DECOMPRESSION_FAIL_MAP` | `9` | 通用常數/硬體欄位定義 |
| 3873 | `!defined(__IO_H__)` | `COMPRESSION_FAIL_MAP` | `8` | 通用常數/硬體欄位定義 |
| 3874 | `!defined(__IO_H__)` | `STA_ERR_MAP` | `7` | 通用常數/硬體欄位定義 |
| 3875 | `!defined(__IO_H__)` | `LCA_FW_CMP_FAIL_MAP` | `5` | 通用常數/硬體欄位定義 |
| 3876 | `!defined(__IO_H__)` | `CRC32_ERR_MAP` | `4` | 通用常數/硬體欄位定義 |
| 3877 | `!defined(__IO_H__)` | `OVER_ECC_THRESHOLD_ERR_MAP` | `3` | 通用常數/硬體欄位定義 |
| 3878 | `!defined(__IO_H__)` | `SIGNOFFMASK_MAP` | `2` | 通用常數/硬體欄位定義 |
| 3879 | `!defined(__IO_H__)` | `UNC_MAP` | `1` | 通用常數/硬體欄位定義 |
| 3880 | `!defined(__IO_H__)` | `INT_VCT_MAP` | `0` | 通用常數/硬體欄位定義 |
| 3881 | `!defined(__IO_H__)` | `SWITCH_PINGPONG_FIFO_TO_NEXT_FRAME_MAP` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3883 | `!defined(__IO_H__)` | `FCTLL_MAP_INF` | `(0x6C>>2)` | 通用常數/硬體欄位定義 |
| 3884 | `!defined(__IO_H__)` | `FCTLL_CMP_ADR` | `(0x70>>2)` | 通用常數/硬體欄位定義 |
| 3885 | `!defined(__IO_H__)` | `FCTLL_RBY_INF` | `(0x74>>2)` | 通用常數/硬體欄位定義 |
| 3886 | `!defined(__IO_H__)` | `FCTLB_RBY_INF` | `(0x74>>0)` | 通用常數/硬體欄位定義 |
| 3887 | `!defined(__IO_H__)` | `FLH_CE_RBY_AND` | `BIT8` | 通用常數/硬體欄位定義 |
| 3889 | `!defined(__IO_H__)` | `FCTLL_CMP_FW` | `(0x78>>2)` | 通用常數/硬體欄位定義 |
| 3890 | `!defined(__IO_H__)` | `CMP_FW_MASK(X)` | `(((UWORD)(X))<<16)` | 通用常數/硬體欄位定義 |
| 3892 | `!defined(__IO_H__)` | `FCTLL_FLH_SET` | `(0x7C>>2)` | 通用常數/硬體欄位定義 |
| 3893 | `!defined(__IO_H__)` | `FLA_ONFI_MODE` | `(SET_BIT0\|SET_BIT1)` | 通用常數/硬體欄位定義 |
| 3894 | `!defined(__IO_H__)` | `FLA_TOGGLE_MODE` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3895 | `!defined(__IO_H__)` | `FLA_16BIT_MODE` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3896 | `!defined(__IO_H__)` | `FLA_LEGACY_MODE` | `(CLR_BIT0&CLR_BIT1)` | 通用常數/硬體欄位定義 |
| 3898 | `!defined(__IO_H__)` | `FCTLL_HS_MODE` | `(0x80>>2)` | 通用常數/硬體欄位定義 |
| 3899 | `!defined(__IO_H__)` | `PIO_DAT_LAST` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 3900 | `!defined(__IO_H__)` | `PIO_DAT_FIRST` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 3901 | `!defined(__IO_H__)` | `DQS_GPO_VALUE` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3902 | `!defined(__IO_H__)` | `DQS_GPO_MODE` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 3903 | `!defined(__IO_H__)` | `PIO_DQS_VALUE` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3904 | `!defined(__IO_H__)` | `PIO_DQS_OUT_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3906 | `!defined(__IO_H__)` | `FCTLL_INT_CFG` | `(0x84>>2)` | 通用常數/硬體欄位定義 |
| 3907 | `!defined(__IO_H__)` | `CLEAR_INTR_VEC_EN` | `SET_BIT28` | 通用常數/硬體欄位定義 |
| 3908 | `!defined(__IO_H__)` | `AUTOPOLLING_CHECK_NOT_STOP_MTQ` | `SET_BIT27` | 通用常數/硬體欄位定義 |
| 3909 | `!defined(__IO_H__)` | `ERR_STATUS_NOT_STOP_MTQ` | `SET_BIT26` | 通用常數/硬體欄位定義 |
| 3910 | `!defined(__IO_H__)` | `OVER_ECC_NOT_STOP_MTQ` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 3911 | `!defined(__IO_H__)` | `FORCE_DMA_ABORT` | `SET_BIT24` | 周邊控制暫存器/位元欄位定義 |
| 3913 | `!defined(__IO_H__)` | `ALL_DONE_INTR_EN` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 3914 | `!defined(__IO_H__)` | `PARTIAL_DONE_INTR_EN` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 3915 | `!defined(__IO_H__)` | `DMA_DONE_INTR_EN` | `SET_BIT13` | 周邊控制暫存器/位元欄位定義 |
| 3916 | `!defined(__IO_H__)` | `ABORT_DONE_INTR_EN` | `SET_BIT12` | 通用常數/硬體欄位定義 |
| 3917 | `!defined(__IO_H__)` | `AUTOPOLLING_FAIL_INTR_EN` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 3918 | `!defined(__IO_H__)` | `LCA_FW_CMP_INTR_EN` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3919 | `!defined(__IO_H__)` | `ERASED_PAGE_INTR_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3920 | `!defined(__IO_H__)` | `ZIP_FAIL_INTR_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3921 | `!defined(__IO_H__)` | `UNZIP_FAIL_INTR_EN` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 3922 | `!defined(__IO_H__)` | `CRC16_FAIL_INTR_EN` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 3923 | `!defined(__IO_H__)` | `OVER_ECC_INTR_EN` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 3924 | `!defined(__IO_H__)` | `CRC32_FAIL_INTR_EN` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 3925 | `!defined(__IO_H__)` | `ERR_STATUS_INTR_EN` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 3926 | `!defined(__IO_H__)` | `SIGNOFF_MASK_FAIL_INTR_EN` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 3927 | `!defined(__IO_H__)` | `UNCOR_ECC_INTR_EN` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3928 | `!defined(__IO_H__)` | `INTR_VEC_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3930 | `!defined(__IO_H__)` | `FCTLL_POL_SEQ_0` | `(0x88>>2)` | 通用常數/硬體欄位定義 |
| 3931 | `!defined(__IO_H__)` | `FCTLL_POL_SEQ_1` | `(0x8C>>2)` | 通用常數/硬體欄位定義 |
| 3932 | `!defined(__IO_H__)` | `FCTLL_SEED_INIT` | `(0x90>>2)` | 通用常數/硬體欄位定義 |
| 3933 | `!defined(__IO_H__)` | `FCTLL_POL_SEQ_2` | `(0x94>>2)` | 通用常數/硬體欄位定義 |
| 3934 | `!defined(__IO_H__)` | `FCTLL_POL_SEQ_3` | `(0x98>>2)` | 通用常數/硬體欄位定義 |
| 3935 | `!defined(__IO_H__)` | `FCTLL_BDC_CTL` | `(0x9C>>2)` | 通用常數/硬體欄位定義 |
| 3936 | `!defined(__IO_H__)` | `BAD_COL_GRP_SIZE_64B` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 3937 | `!defined(__IO_H__)` | `BAD_COL_GRP_SIZE_128B` | `CLR_BIT31` | 通用常數/硬體欄位定義 |
| 3938 | `!defined(__IO_H__)` | `BAD_COL_RANDOM_VALUE` | `CLR_BIT26` | 通用常數/硬體欄位定義 |
| 3939 | `!defined(__IO_H__)` | `BAD_COL_FW_DEFINE_VALUE` | `SET_BIT26` | 通用常數/硬體欄位定義 |
| 3940 | `!defined(__IO_H__)` | `BAD_COL_WITH_DMA_EN` | `SET_BIT25` | 周邊控制暫存器/位元欄位定義 |
| 3941 | `!defined(__IO_H__)` | `BAD_COL_COLLECT_TRIG` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 3942 | `!defined(__IO_H__)` | `BAD_COL_VAL_FLASH_WRITE_DMA(X)` | `(((UBYTE)(X)&0xFF)<<16)` | 周邊控制暫存器/位元欄位定義 |
| 3943 | `!defined(__IO_H__)` | `BAD_COL_COLLECT_LEN(X)` | `((UWORD)(X)&0xFFFF)` | 通用常數/硬體欄位定義 |
| 3945 | `!defined(__IO_H__)` | `FCTLL_RESERVE` | `(0xA0>>2)` | 通用常數/硬體欄位定義 |
| 3946 | `!defined(__IO_H__)` | `DETECT_DQSB_ENABLE` | `BIT21` | 通用常數/硬體欄位定義 |
| 3947 | `!defined(__IO_H__)` | `DQSB_IS_TOGGLED` | `BIT20` | 通用常數/硬體欄位定義 |
| 3949 | `!defined(__IO_H__)` | `FCTLL_BDC_SET` | `(0xA4>>2)` | 通用常數/硬體欄位定義 |
| 3950 | `!defined(__IO_H__)` | `BAD_COL_GROUP_SEL(X)` | `(((U32)(X)&0x3F)<<24)` | 通用常數/硬體欄位定義 |
| 3951 | `!defined(__IO_H__)` | `COMP_VAL_FOR_BAD_COL_COLLECT_FUNC(X)` | `((UBYTE)(X))` | 通用常數/硬體欄位定義 |
| 3952 | `!defined(__IO_H__)` | `FCTLL_POL_SEQ_4` | `(0xA8>>2)` | 通用常數/硬體欄位定義 |
| 3953 | `!defined(__IO_H__)` | `FCTLL_POL_SEQ_5` | `(0xAC>>2)` | 通用常數/硬體欄位定義 |
| 3954 | `!defined(__IO_H__)` | `FCTLL_POL_SEQ_6` | `(0xB0>>2)` | 通用常數/硬體欄位定義 |
| 3955 | `!defined(__IO_H__)` | `FCTLL_POL_SEQ_7` | `(0xB4>>2)` | 通用常數/硬體欄位定義 |
| 3956 | `!defined(__IO_H__)` | `FCTLL_FARG_ROW` | `(0xB8>>2)` | 通用常數/硬體欄位定義 |
| 3957 | `!defined(__IO_H__)` | `FCTLL_FARG_COL` | `(0xBC>>2)` | 通用常數/硬體欄位定義 |
| 3958 | `!defined(__IO_H__)` | `FCTLL_FSA_ADR` | `(0xC0>>2)` | 通用常數/硬體欄位定義 |
| 3959 | `!defined(__IO_H__)` | `FCTLL_CHNL_SET` | `(0xC4>>2)` | 通用常數/硬體欄位定義 |
| 3960 | `!defined(__IO_H__)` | `BUF_VLD_GEN_FRM_PTR_AND_FRM_NUM` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 3961 | `!defined(__IO_H__)` | `PHISON_RANDOMIZER` | `CLR_BIT29` | 通用常數/硬體欄位定義 |
| 3963 | `!defined(__IO_H__)` | `MT_AUTO_CE_CLR` | `SET_BIT28` | 依原始註解：#define TOSHIBA_RANDOMIZER		SET_BIT29；註解：#define TOSHIBA_RANDOMIZER		SET_BIT29 |
| 3965 | `!defined(__IO_H__)` | `DUMMY_DATA_LEN(X)` | `(((UWORD)(X)&0x07FF)<<16)` | 依原始註解：#define SPR_LOC_SP0_START_GO_ON		SET_BIT27；註解：#define SPR_LOC_SP0_START_GO_ON		SET_BIT27 |
| 3966 | `!defined(__IO_H__)` | `CRC_NO_CHECK_IN_FCON_CRC_EN` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 3967 | `!defined(__IO_H__)` | `CRC_FROM_CRC_MODULE` | `CLR_BIT14` | 通用常數/硬體欄位定義 |
| 3968 | `!defined(__IO_H__)` | `CRC_FROM_FCON_CRC_FORCE` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 3969 | `!defined(__IO_H__)` | `SPARE_CONV_EN` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 3970 | `!defined(__IO_H__)` | `FLA_EDO_MODE` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3971 | `!defined(__IO_H__)` | `BYPASS_CONV` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3972 | `!defined(__IO_H__)` | `BYPASS_INV` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3974 | `!defined(__IO_H__)` | `FCTLL_CPU_INT_CFG` | `(0xC8>>2)` | 通用常數/硬體欄位定義 |
| 3975 | `!defined(__IO_H__)` | `ERR_STATUS_CPU_INTR_EN` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 3976 | `!defined(__IO_H__)` | `AUTOPOLLING_FAIL_CPU_INTR_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 3977 | `!defined(__IO_H__)` | `ERASED_PAGE_CPU_INTR_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 3978 | `!defined(__IO_H__)` | `ZIP_FAIL_CPU_INTR_EN` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 3979 | `!defined(__IO_H__)` | `UNZIP_FAIL_CPU_INTR_EN` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 3980 | `!defined(__IO_H__)` | `CRC16_FAIL_CPU_INTR_EN` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 3981 | `!defined(__IO_H__)` | `UNC_CPU_INTR_EN` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 3982 | `!defined(__IO_H__)` | `SIGNOFF_MASK_FAIL_CPU_INTR_EN` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 3983 | `!defined(__IO_H__)` | `OVER_ERR_CPU_INTR_EN` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 3984 | `!defined(__IO_H__)` | `CRC32_FAIL_CPU_INTR_EN` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 3985 | `!defined(__IO_H__)` | `LCA_FW_CMP_CPU_INTR_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3987 | `!defined(__IO_H__)` | `FCTLL_INT_RDY` | `(0xCC>>2)` | 通用常數/硬體欄位定義 |
| 3988 | `!defined(__IO_H__)` | `FCTLL_FPU_TRIG` | `(0xD0>>2)` | 通用常數/硬體欄位定義 |
| 3989 | `!defined(__IO_H__)` | `TIMEOUT` | `BIT7` | 通用常數/硬體欄位定義 |
| 3990 | `!defined(__IO_H__)` | `MT_BUSY` | `BIT4` | 通用常數/硬體欄位定義 |
| 3991 | `!defined(__IO_H__)` | `ANY_BUSY` | `BIT3` | 通用常數/硬體欄位定義 |
| 3992 | `!defined(__IO_H__)` | `SRQ_AVL` | `BIT2` | 通用常數/硬體欄位定義 |
| 3993 | `!defined(__IO_H__)` | `SIGNOFF_BUSY` | `BIT1` | 通用常數/硬體欄位定義 |
| 3994 | `!defined(__IO_H__)` | `FPU_TRIG` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 3996 | `!defined(__IO_H__)` | `FCTLL_FPU_ENTRY` | `(0xD4>>2)` | 通用常數/硬體欄位定義 |
| 3997 | `!defined(__IO_H__)` | `FCTLL_MT_ADR_4` | `(0xD8>>2)` | 通用常數/硬體欄位定義 |
| 3998 | `!defined(__IO_H__)` | `FCTLL_MT_ADR_5` | `(0xDC>>2)` | 通用常數/硬體欄位定義 |
| 3999 | `!defined(__IO_H__)` | `FCTLL_MT_ADR_6` | `(0xE0>>2)` | 通用常數/硬體欄位定義 |
| 4000 | `!defined(__IO_H__)` | `FCTLL_MT_ADR_7` | `(0xE4>>2)` | 通用常數/硬體欄位定義 |
| 4001 | `!defined(__IO_H__)` | `FCTLL_FAIL_CNT` | `(0xE8>>2)` | 通用常數/硬體欄位定義 |
| 4002 | `!defined(__IO_H__)` | `FCTLL_MTQ_CFG` | `(0xEC>>2)` | 通用常數/硬體欄位定義 |
| 4003 | `!defined(__IO_H__)` | `FORCE_EMPTY_ALL` | `0x000000FF` | 通用常數/硬體欄位定義 |
| 4004 | `!defined(__IO_H__)` | `CLEAR_FORCE_EMPTY_ALL` | `0xFFFFFF00` | 通用常數/硬體欄位定義 |
| 4005 | `!defined(__IO_H__)` | `FORCE_EMPTY_MTQ0` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4006 | `!defined(__IO_H__)` | `FORCE_EMPTY_MTQ1` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4007 | `!defined(__IO_H__)` | `FORCE_EMPTY_MTQ2` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 4008 | `!defined(__IO_H__)` | `FORCE_EMPTY_MTQ3` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 4009 | `!defined(__IO_H__)` | `EMPTYMASK_MTQ0` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 4010 | `!defined(__IO_H__)` | `EMPTYMASK_MTQ1` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 4011 | `!defined(__IO_H__)` | `EMPTYMASK_MTQ2` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 4012 | `!defined(__IO_H__)` | `EMPTYMASK_MTQ3` | `SET_BIT11` | 通用常數/硬體欄位定義 |
| 4015 | `!defined(__IO_H__)` | `FCTLB_MTQ_CFG` | `(0xEC)` | 通用常數/硬體欄位定義 |
| 4022 | `!defined(__IO_H__)` | `FCTLL_MTQ_INF_0` | `(0xF8>>2)` | 依原始註解：#define CLEAR_CNV_KEY		0x00000000 \| #define SECTOR_KEY(X)		(((UWORD)X&0xFFFF)<<1；註解：#define CLEAR_CNV_KEY		0x00000000 \| #define SECTOR_KEY(X)		(((UWORD)X&0xFFFF)<<16) \| #define PAGE_KEY(X) 		((UWORD)X&0xFFFF) |
| 4023 | `!defined(__IO_H__)` | `GET_QUEUE_NON_EXT_NUM(mt_ptr_dif,queue)` | `((UBYTE)((((U32)(mt_ptr_dif))>>(((UBYTE)(queue))*8))&0xFF))` | 通用常數/硬體欄位定義 |
| 4024 | `!defined(__IO_H__)` | `FCTLL_DBG_INF` | `(0xFC>>2)` | 通用常數/硬體欄位定義 |
| 4025 | `!defined(__IO_H__)` | `GET_DBG_MTQ_STS(X)` | `((((U32)X)&0x01F00000)>>20)` | 通用常數/硬體欄位定義 |
| 4026 | `!defined(__IO_H__)` | `DBG_MTQ_IDLE_CASE1` | `0` | 通用常數/硬體欄位定義 |
| 4027 | `!defined(__IO_H__)` | `DBG_MTQ_IDLE_CASE2` | `0x13` | 通用常數/硬體欄位定義 |
| 4029 | `!defined(__IO_H__)` | `FCTLL_SEED_MODE` | `(0x100>>2)` | 通用常數/硬體欄位定義 |
| 4030 | `!defined(__IO_H__)` | `CNV_SECTOR_BASE_FORMAT` | `CLR_BIT3` | 通用常數/硬體欄位定義 |
| 4031 | `!defined(__IO_H__)` | `CNV_FRAME_BASE_FORMAT` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 4032 | `!defined(__IO_H__)` | `CONVERSION_RELOAD_EN` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 4033 | `!defined(__IO_H__)` | `LOAD_NEW_SEED_CONVERSION_FOR_EACH_512B` | `(CLR_BIT0&CLR_BIT1)` | 通用常數/硬體欄位定義 |
| 4034 | `!defined(__IO_H__)` | `LOAD_NEW_SEED_CONVERSION_FOR_EACH_1K` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4035 | `!defined(__IO_H__)` | `LOAD_NEW_SEED_CONVERSION_FOR_EACH_2K` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4037 | `!defined(__IO_H__)` | `FCTLL_ERASE_CFG` | `(0x104>>2)` | 通用常數/硬體欄位定義 |
| 4038 | `!defined(__IO_H__)` | `ERASE_SUSPEND_FUNC_EN` | `SET_BIT30` | 通用常數/硬體欄位定義 |
| 4039 | `!defined(__IO_H__)` | `MTQ_TIMEOUT_STOP_EN` | `SET_BIT29` | 通用常數/硬體欄位定義 |
| 4040 | `!defined(__IO_H__)` | `CLEAR_TIMEOUT_FLAG_STATUS` | `SET_BIT28` | 通用常數/硬體欄位定義 |
| 4041 | `!defined(__IO_H__)` | `EXPECT_TIMEOUT_VALUE(X)` | `((U32)(X)&0x0FFFFFFF)` | 通用常數/硬體欄位定義 |
| 4043 | `!defined(__IO_H__)` | `FCTLL_ERASE_SEQ` | `(0x108>>2)` | 通用常數/硬體欄位定義 |
| 4044 | `!defined(__IO_H__)` | `FCTLL_MGM_CFG` | `(0x10C>>2)` | 通用常數/硬體欄位定義 |
| 4045 | `!defined(__IO_H__)` | `INCREASE_MGM(X)` | `(((UBYTE)(X)&0x3F)<<1)` | 通用常數/硬體欄位定義 |
| 4046 | `!defined(__IO_H__)` | `MGM_FUNC_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4048 | `!defined(__IO_H__)` | `FCTLL_MTQ_INF_1` | `(0x110>>2)` | 通用常數/硬體欄位定義 |
| 4049 | `!defined(__IO_H__)` | `FCTLL_MTQ_DLY` | `(0x114>>2)` | 通用常數/硬體欄位定義 |
| 4050 | `!defined(__IO_H__)` | `CANCEL_MTQ_DELAY(MTQNum)` | `(((UBYTE)(BIT0<<(MTQNum))&0xFF)<<24)` | 通用常數/硬體欄位定義 |
| 4051 | `!defined(__IO_H__)` | `MTQ_DELAY(X)` | `((U32)(X)&0xFFFFFF)` | 通用常數/硬體欄位定義 |
| 4053 | `!defined(__IO_H__)` | `FCTLL_FDIV_CFG` | `(0x118>>2)` | 通用常數/硬體欄位定義 |
| 4054 | `!defined(__IO_H__)` | `FLH_CLK_DIV_EN` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 4055 | `!defined(__IO_H__)` | `FLH_CLK_DIV_VALUE(X)` | `((UBYTE)(X)&0x3F)` | 通用常數/硬體欄位定義 |
| 4057 | `!defined(__IO_H__)` | `FCTLL_NVME_CFG_0` | `(0x11C>>2)` | 通用常數/硬體欄位定義 |
| 4058 | `!defined(__IO_H__)` | `FCTLL_NVME_CFG_1` | `(0x120>>2)` | 通用常數/硬體欄位定義 |
| 4059 | `!defined(__IO_H__)` | `DESCRIPTOR_INDEX_FUNC_EN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 4060 | `!defined(__IO_H__)` | `NVME_1ST_DESCRIPTOR_INDEX(X)` | `((UWORD)(X)&0x1FF)` | 通用常數/硬體欄位定義 |
| 4061 | `!defined(__IO_H__)` | `NVME_2ND_DESCRIPTOR_INDEX(X)` | `(((UWORD)(X)&0x1FF)<<9)` | 通用常數/硬體欄位定義 |
| 4062 | `!defined(__IO_H__)` | `NVME_3RD_DESCRIPTOR_INDEX(X)` | `(((UWORD)(X)&0x1FF)<<18)` | 通用常數/硬體欄位定義 |
| 4063 | `!defined(__IO_H__)` | `NVME_4TH_DESCRIPTOR_INDEX(X)` | `((UWORD)(X)&0x1FF)` | 通用常數/硬體欄位定義 |
| 4065 | `!defined(__IO_H__)` | `FCTLL_LDPC_MASK` | `(0x124>>2)` | 通用常數/硬體欄位定義 |
| 4066 | `!defined(__IO_H__)` | `FCTLL_L4K_SPR_ADR` | `(0x128>>2)` | 通用常數/硬體欄位定義 |
| 4067 | `!defined(__IO_H__)` | `FCTLL_ZIP_CFG` | `(0x12C>>2)` | 通用常數/硬體欄位定義 |
| 4068 | `!defined(__IO_H__)` | `RING_MODE` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 4069 | `!defined(__IO_H__)` | `SET_FW3(X)` | `(((UBYTE)(X)&0xFF)<<8)` | 通用常數/硬體欄位定義 |
| 4070 | `!defined(__IO_H__)` | `CLR_FW3` | `(0xFFFF00FF)` | 通用常數/硬體欄位定義 |
| 4071 | `!defined(__IO_H__)` | `L4K_BADR_CONTINUOUS_MODE` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 4072 | `!defined(__IO_H__)` | `BYPASS_ZIP` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 4073 | `!defined(__IO_H__)` | `CLEAR_L4K_NUM` | `0xFFFFFFC0` | 通用常數/硬體欄位定義 |
| 4074 | `!defined(__IO_H__)` | `L4K_NUM(X)` | `((UBYTE)(X)&0x3F)` | 通用常數/硬體欄位定義 |
| 4076 | `!defined(__IO_H__)` | `FCTLL_RAW_DMA_ADR` | `(0x130>>2)` | 周邊控制暫存器/位元欄位定義 |
| 4077 | `!defined(__IO_H__)` | `FCTLL_BACK_STORE` | `(0x134>>2)` | 通用常數/硬體欄位定義 |
| 4078 | `!defined(__IO_H__)` | `FRAME_PTR(X)` | `(((UBYTE)(X)&0x1F)<<2)` | 通用常數/硬體欄位定義 |
| 4079 | `!defined(__IO_H__)` | `DRAM2IBF` | `CLR_BIT1` | 通用常數/硬體欄位定義 |
| 4080 | `!defined(__IO_H__)` | `IBF2DRAM` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4081 | `!defined(__IO_H__)` | `BACK_STORE_TRIG` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4083 | `!defined(__IO_H__)` | `FCTLL_RS_CFG` | `(0x138>>2)` | 通用常數/硬體欄位定義 |
| 4084 | `!defined(__IO_H__)` | `RS_ONE_PARITY_PAGE_EN` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 4085 | `!defined(__IO_H__)` | `CLEAR_RS_PAGENUM` | `0xFF00FFFF` | 通用常數/硬體欄位定義 |
| 4086 | `!defined(__IO_H__)` | `RS_PAGENUM(X)` | `(((UBYTE)(X)&0xFF)<<16)` | 通用常數/硬體欄位定義 |
| 4087 | `!defined(__IO_H__)` | `CLEAR_RS_TAG` | `0xFFFFF0FF` | 通用常數/硬體欄位定義 |
| 4088 | `!defined(__IO_H__)` | `RS_TAG_FPU(X)` | `(((UBYTE)(X)&0x0F)<<8)` | 通用常數/硬體欄位定義 |
| 4089 | `!defined(__IO_H__)` | `RS_DEC_PEC` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 4090 | `!defined(__IO_H__)` | `RS_PROG_SECOND_PARITY` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 4091 | `!defined(__IO_H__)` | `RS_LASTPAGE` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 4092 | `!defined(__IO_H__)` | `RS_PROGRAM_PARITY_EN` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 4093 | `!defined(__IO_H__)` | `RS_ON_THE_FLY_EN` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4094 | `!defined(__IO_H__)` | `RS_NORMAL_ENCODE` | `CLR_BIT0` | 通用常數/硬體欄位定義 |
| 4095 | `!defined(__IO_H__)` | `RS_XOR_ENCODE` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4096 | `!defined(__IO_H__)` | `FCTLB_RS_PAGENUM` | `(0x139>>0)` | 通用常數/硬體欄位定義 |
| 4098 | `!defined(__IO_H__)` | `FCTLL_LDPC_CFG` | `(0x13C>>2)` | 通用常數/硬體欄位定義 |
| 4099 | `!defined(__IO_H__)` | `BCH_RESULT_UNCORRECTABLE` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 4100 | `!defined(__IO_H__)` | `LDPC_RESULT_UNC` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 4101 | `!defined(__IO_H__)` | `LDPC_COR_EN` | `SET_BIT13` | 通用常數/硬體欄位定義 |
| 4103 | `!defined(__IO_H__)` | `FCTLL_RD_CNT` | `(0x140>>2)` | 通用常數/硬體欄位定義 |
| 4104 | `!defined(__IO_H__)` | `FCTLL_RD2_CNT` | `(0x144>>2)` | 通用常數/硬體欄位定義 |
| 4105 | `!defined(__IO_H__)` | `FCTLL_DYNAMIC_ODT` | `(0x148>>2)` | 通用常數/硬體欄位定義 |
| 4106 | `!defined(__IO_H__)` | `ODT_FORCE_VALUE` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 4107 | `!defined(__IO_H__)` | `ODT_FORCE_ENABLE` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4108 | `!defined(__IO_H__)` | `DYNAMIC_ODT_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4110 | `!defined(__IO_H__)` | `FCTLL_IBF_CTL` | `(0x14C>>2)` | 通用常數/硬體欄位定義 |
| 4111 | `!defined(__IO_H__)` | `FORCE_ZIP_COR` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 4112 | `!defined(__IO_H__)` | `CLR_LDPC_COR_MAP` | `0xFFFFFFE0` | 通用常數/硬體欄位定義 |
| 4113 | `!defined(__IO_H__)` | `LDPC_COR_MAP(X)` | `(((UBYTE)X)&0x1F)` | 通用常數/硬體欄位定義 |
| 4115 | `!defined(__IO_H__)` | `FCTLL_IFSA0` | `(0x154>>2)` | 通用常數/硬體欄位定義 |
| 4116 | `!defined(__IO_H__)` | `FCTLL_IFSA1` | `(0x158>>2)` | 通用常數/硬體欄位定義 |
| 4117 | `!defined(__IO_H__)` | `FCTLL_IFSA2` | `(0x15C>>2)` | 通用常數/硬體欄位定義 |
| 4118 | `!defined(__IO_H__)` | `FCTLL_IFSA3` | `(0x160>>2)` | 通用常數/硬體欄位定義 |
| 4120 | `!defined(__IO_H__)` | `FCTLL_E3D_ERR_CNT` | `(0x180>>2)` | 通用常數/硬體欄位定義 |
| 4121 | `!defined(__IO_H__)` | `CLR_E3D_ERR_CNT` | `BIT31` | 通用常數/硬體欄位定義 |
| 4122 | `!defined(__IO_H__)` | `E3D_ERR_CNT` | `0x7FFFFFFF` | 通用常數/硬體欄位定義 |
| 4125 | `!defined(__IO_H__)` | `FALLL_ZIP_MAP` | `(0x20>>2)` | 通用常數/硬體欄位定義 |
| 4126 | `!defined(__IO_H__)` | `FALLL_MT_TRIG` | `(0x24>>2)` | 通用常數/硬體欄位定義 |
| 4127 | `!defined(__IO_H__)` | `FALLL_DMA_CFG` | `(0x40>>2)` | 周邊控制暫存器/位元欄位定義 |
| 4128 | `!defined(__IO_H__)` | `FALLL_OTHER_SET` | `(0x44>>2)` | 通用常數/硬體欄位定義 |
| 4129 | `!defined(__IO_H__)` | `FALLL_FSA_SEL` | `(0x48>>2)` | 通用常數/硬體欄位定義 |
| 4130 | `!defined(__IO_H__)` | `FALLL_CNT_ONE` | `(0x50>>2)` | 通用常數/硬體欄位定義 |
| 4131 | `!defined(__IO_H__)` | `FALLL_INT_INF` | `(0x64>>2)` | 通用常數/硬體欄位定義 |
| 4132 | `!defined(__IO_H__)` | `FALLL_MAP_CFG` | `(0x68>>2)` | 通用常數/硬體欄位定義 |
| 4133 | `!defined(__IO_H__)` | `FALLL_HS_MODE` | `(0x80>>2)` | 通用常數/硬體欄位定義 |
| 4134 | `!defined(__IO_H__)` | `FALLL_INT_CFG` | `(0x84>>2)` | 通用常數/硬體欄位定義 |
| 4135 | `!defined(__IO_H__)` | `FALLL_POL_SEQ_0` | `(0x88>>2)` | 通用常數/硬體欄位定義 |
| 4136 | `!defined(__IO_H__)` | `FALLL_POL_SEQ_1` | `(0x8C>>2)` | 通用常數/硬體欄位定義 |
| 4137 | `!defined(__IO_H__)` | `FALLL_POL_SEQ_2` | `(0x94>>2)` | 通用常數/硬體欄位定義 |
| 4138 | `!defined(__IO_H__)` | `FALLL_POL_SEQ_3` | `(0x98>>2)` | 通用常數/硬體欄位定義 |
| 4139 | `!defined(__IO_H__)` | `FALLL_POL_SEQ_4` | `(0xA8>>2)` | 通用常數/硬體欄位定義 |
| 4140 | `!defined(__IO_H__)` | `FALLL_POL_SEQ_5` | `(0xAC>>2)` | 通用常數/硬體欄位定義 |
| 4141 | `!defined(__IO_H__)` | `FALLL_POL_SEQ_6` | `(0xB0>>2)` | 通用常數/硬體欄位定義 |
| 4142 | `!defined(__IO_H__)` | `FALLL_POL_SEQ_7` | `(0xB4>>2)` | 通用常數/硬體欄位定義 |
| 4143 | `!defined(__IO_H__)` | `FALLL_CHNL_SET` | `(0xC4>>2)` | 通用常數/硬體欄位定義 |
| 4144 | `!defined(__IO_H__)` | `FALLL_CPU_INT_CFG` | `(0xC8>>2)` | 通用常數/硬體欄位定義 |
| 4145 | `!defined(__IO_H__)` | `FALLL_MTQ_CFG` | `(0xEC>>2)` | 通用常數/硬體欄位定義 |
| 4146 | `!defined(__IO_H__)` | `FALLL_DBG_INF` | `(0xFC>>2)` | 通用常數/硬體欄位定義 |
| 4147 | `!defined(__IO_H__)` | `FALLL_ZIP_CFG` | `(0x12C>>2)` | 通用常數/硬體欄位定義 |
| 4148 | `!defined(__IO_H__)` | `FALLL_LDPC_CFG` | `(0x13C>>2)` | 通用常數/硬體欄位定義 |
| 4153 | `!defined(__IO_H__)` | `FCONL_INFO` | `(0x00>>2)` | 依原始註解：=================================================== \| PS3111 FLH CONFG Register ；註解：=================================================== \| PS3111 FLH CONFG Register Define \| =================================================== |
| 4154 | `!defined(__IO_H__)` | `FCONL_MODE_SET` | `(0x04>>2)` | 通用常數/硬體欄位定義 |
| 4155 | `!defined(__IO_H__)` | `CPU_WAIT_MODE_COUNTER` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 4156 | `!defined(__IO_H__)` | `TERMI_COEF(X)` | `(((UBYTE)(X)&7F)<<24)` | 通用常數/硬體欄位定義 |
| 4157 | `!defined(__IO_H__)` | `TERMI_EN` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 4158 | `!defined(__IO_H__)` | `LENGTH_EN` | `SET_BIT22` | 通用常數/硬體欄位定義 |
| 4159 | `!defined(__IO_H__)` | `CS_ACC_EN` | `SET_BIT21` | 通用常數/硬體欄位定義 |
| 4160 | `!defined(__IO_H__)` | `ACC_EN` | `SET_BIT20` | 通用常數/硬體欄位定義 |
| 4161 | `!defined(__IO_H__)` | `COR_CLK_ALWAYS_EN` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 4162 | `!defined(__IO_H__)` | `ECC_CLK_ALWAYS_EN` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 4164 | `!defined(__IO_H__)` | `FCONB_FCE_ENB` | `((_UCV unsigned char *)(Flash_Reg_Base+FTOP+0x08))` | 通用常數/硬體欄位定義 |
| 4165 | `!defined(__IO_H__)` | `FCONO_FCE_ENB` | `(0x08>>3)` | 通用常數/硬體欄位定義 |
| 4166 | `!defined(__IO_H__)` | `FCONL_FCE_ENB` | `(0x08>>2)` | 通用常數/硬體欄位定義 |
| 4167 | `!defined(__IO_H__)` | `FCONB_FCE_ENB_CH0` | `(0x08>>0)` | 通用常數/硬體欄位定義 |
| 4168 | `!defined(__IO_H__)` | `FCONB_FCE_ENB_CH1` | `(0x09>>0)` | 通用常數/硬體欄位定義 |
| 4169 | `!defined(__IO_H__)` | `FCONL_FCE_ENB_1` | `(0x0C>>2)` | 通用常數/硬體欄位定義 |
| 4170 | `!defined(__IO_H__)` | `FCONL_SMP_CFG` | `(0x18>>2)` | 通用常數/硬體欄位定義 |
| 4171 | `!defined(__IO_H__)` | `TOTAL_SMP_NUM_CLR` | `(0xFFFFFF81)` | 通用常數/硬體欄位定義 |
| 4172 | `!defined(__IO_H__)` | `TOTAL_SMP_NUM(X)` | `(((UBYTE)(X)&0x3F)<<1)` | 通用常數/硬體欄位定義 |
| 4173 | `!defined(__IO_H__)` | `TOTAL_SMP_FUNC_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4175 | `!defined(__IO_H__)` | `FCONL_FORMAT` | `(0x1C>>2)` | 通用常數/硬體欄位定義 |
| 4176 | `!defined(__IO_H__)` | `FCONL_IRC_CTL` | `(0x28>>2)` | 通用常數/硬體欄位定義 |
| 4177 | `!defined(__IO_H__)` | `IRAM_INITIAL_TRIG` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 4178 | `!defined(__IO_H__)` | `IRAM_COPY_TRIG` | `SET_BIT7` | 通用常數/硬體欄位定義 |
| 4179 | `!defined(__IO_H__)` | `IRAM_COPY_LEN(X)` | `((UBYTE)(X)&0x7F)` | 通用常數/硬體欄位定義 |
| 4181 | `!defined(__IO_H__)` | `FCONL_IRC_ADR` | `(0x2C>>2)` | 通用常數/硬體欄位定義 |
| 4182 | `!defined(__IO_H__)` | `FCONW_IRC_SRC_ADR` | `(0x2C>>1)` | 通用常數/硬體欄位定義 |
| 4183 | `!defined(__IO_H__)` | `FCONW_IRC_DST_ADR` | `(0x2E>>1)` | 通用常數/硬體欄位定義 |
| 4185 | `!defined(__IO_H__)` | `FCONL_CNT_ERR_SET` | `(0x30>>2)` | 通用常數/硬體欄位定義 |
| 4186 | `!defined(__IO_H__)` | `FCONL_CNT_ERR` | `(0x34>>2)` | 通用常數/硬體欄位定義 |
| 4187 | `!defined(__IO_H__)` | `FCONL_FLH_FUNC` | `(0x38>>2)` | 通用常數/硬體欄位定義 |
| 4188 | `!defined(__IO_H__)` | `IBUF_RING_EN` | `SET_BIT29` | 通用常數/硬體欄位定義 |
| 4189 | `!defined(__IO_H__)` | `IBUF_RING_CH0_EN` | `SET_BIT29` | 通用常數/硬體欄位定義 |
| 4190 | `!defined(__IO_H__)` | `FLH_USE_IBUF_RING` | `SET_BIT28` | 通用常數/硬體欄位定義 |
| 4191 | `!defined(__IO_H__)` | `DQS_INPUT_EVENT_MASK_PRE_STATE_EN` | `SET_BIT21` | 通用常數/硬體欄位定義 |
| 4192 | `!defined(__IO_H__)` | `AXI_READ_ENHANCE_EN` | `SET_BIT20` | 通用常數/硬體欄位定義 |
| 4193 | `!defined(__IO_H__)` | `AXI_RUN_ROBIN_EN` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 4194 | `!defined(__IO_H__)` | `ECC_DMP_EN` | `SET_BIT18` | 通用常數/硬體欄位定義 |
| 4195 | `!defined(__IO_H__)` | `DATA_TO_RAM_WHEN_CRC_FAIL` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 4196 | `!defined(__IO_H__)` | `DATA_TO_RAM_WHEN_UECC` | `SET_BIT16` | 通用常數/硬體欄位定義 |
| 4197 | `!defined(__IO_H__)` | `RDM_REF_ROW_ADR_5_TO_0` | `0x00000C00` | 通用常數/硬體欄位定義 |
| 4198 | `!defined(__IO_H__)` | `RDM_REF_ROW_ADR_6_TO_0` | `0x00000800` | 通用常數/硬體欄位定義 |
| 4199 | `!defined(__IO_H__)` | `RDM_REF_ROW_ADR_7_TO_0` | `0x00000400` | 通用常數/硬體欄位定義 |
| 4200 | `!defined(__IO_H__)` | `RDM_REF_ROW_ADR_8_TO_0` | `0x00000000` | 通用常數/硬體欄位定義 |
| 4201 | `!defined(__IO_H__)` | `IBUF_RING_CH1_EN` | `SET_BIT6` | 通用常數/硬體欄位定義 |
| 4202 | `!defined(__IO_H__)` | `MT_TABLE_CE_NORMAL_MODE` | `CLR_BIT4` | 通用常數/硬體欄位定義 |
| 4203 | `!defined(__IO_H__)` | `MT_TABLE_CE_NUMBER_MODE` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 4204 | `!defined(__IO_H__)` | `SIGNOFF_MASK_WITH_BUF_VALID_WRITE_BACK` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 4206 | `!defined(__IO_H__)` | `E3D_FORCE_VALUE` | `SET_BIT1` | 依原始註解：#define SIGNOFF_4K_MODE				SET_BIT2；註解：#define SIGNOFF_4K_MODE				SET_BIT2 |
| 4207 | `!defined(__IO_H__)` | `E3D_FORCE_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4209 | `!defined(__IO_H__)` | `FCONL_AXIS_ADR` | `(0x3C>>2)` | 通用常數/硬體欄位定義 |
| 4210 | `!defined(__IO_H__)` | `FCONL_MNT_CFG` | `(0x40>>2)` | 通用常數/硬體欄位定義 |
| 4211 | `!defined(__IO_H__)` | `MDM_COUNTER_VALUE(X)` | `(((UWORD)(X)&0xFFFF)<<16)` | 通用常數/硬體欄位定義 |
| 4212 | `!defined(__IO_H__)` | `MDM_STEP_VALUE(X)` | `(((UWORD)(X)&0x7FFF)<<1)` | 通用常數/硬體欄位定義 |
| 4214 | `!defined(__IO_H__)` | `MT_DMA_MONITOR_FUNC_EN` | `SET_BIT0` | 周邊控制暫存器/位元欄位定義 |
| 4216 | `!defined(__IO_H__)` | `FCONL_SGN_MSK` | `(0x44>>2)` | 通用常數/硬體欄位定義 |
| 4218 | `!defined(__IO_H__)` | `SIGNOFF_MASK_EN` | `SET_BIT0` | 依原始註解：#define SIGNOFF_MASK_VALUE			SET_BIT1；註解：#define SIGNOFF_MASK_VALUE			SET_BIT1 |
| 4220 | `!defined(__IO_H__)` | `FCONL_MNT_CNT` | `(0x48>>2)` | 通用常數/硬體欄位定義 |
| 4221 | `!defined(__IO_H__)` | `FCONL_RTY_CFG` | `(0x4C>>2)` | 通用常數/硬體欄位定義 |
| 4222 | `!defined(__IO_H__)` | `FLH_POST_SIM_TGL_CFG` | `0x00400000` | 通用常數/硬體欄位定義 |
| 4224 | `!defined(__IO_H__)` | `FCONL_SPECIAL` | `(0x50>>2)` | 通用常數/硬體欄位定義 |
| 4225 | `!defined(__IO_H__)` | `ERROR_INJECTION_OF_PARITY_CHECK_ZRAM` | `SET_BIT30` | 通用常數/硬體欄位定義 |
| 4226 | `!defined(__IO_H__)` | `MT_QUERY_FLAG_EN` | `SET_BIT29` | 通用常數/硬體欄位定義 |
| 4227 | `!defined(__IO_H__)` | `CLR_ECC_THRESHOLD_NUM` | `(0xFC00FFFF)` | 通用常數/硬體欄位定義 |
| 4228 | `!defined(__IO_H__)` | `SET_ECC_THRESHOLD_NUM(X)` | `((X&0x3FF)<<16)` | 通用常數/硬體欄位定義 |
| 4229 | `!defined(__IO_H__)` | `MT_AUTO_POLL_RDY` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 4230 | `!defined(__IO_H__)` | `PFA_FUNC_EN` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 4231 | `!defined(__IO_H__)` | `MCU_WAIT_COUNTER_VALUE(X)` | `((UBYTE)(X))` | 通用常數/硬體欄位定義 |
| 4232 | `!defined(__IO_H__)` | `FCONB_SPECIAL` | `(0x50)` | 通用常數/硬體欄位定義 |
| 4233 | `!defined(__IO_H__)` | `FCONW_SPECIAL_H` | `(0x52>>1)` | 通用常數/硬體欄位定義 |
| 4234 | `!defined(__IO_H__)` | `ECC_THRESHOLD_NUM(X)` | `((UWORD)(X))` | 通用常數/硬體欄位定義 |
| 4236 | `!defined(__IO_H__)` | `FCONL_LGC_TIME_CFG` | `(0x64>>2)` | 通用常數/硬體欄位定義 |
| 4237 | `!defined(__IO_H__)` | `LEGACY_DAT_PST_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<20)` | 通用常數/硬體欄位定義 |
| 4238 | `!defined(__IO_H__)` | `LEGACY_DAT_PRE_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<16)` | 通用常數/硬體欄位定義 |
| 4239 | `!defined(__IO_H__)` | `LEGACY_ADR_PST_PHASE_CNT(X)` | `(((UBYTE)(X)&0x07)<<13)` | 通用常數/硬體欄位定義 |
| 4240 | `!defined(__IO_H__)` | `LEGACY_ADR_WEN_PHASE_CNT(X)` | `(((UBYTE)(X)&0x03)<<11)` | 通用常數/硬體欄位定義 |
| 4241 | `!defined(__IO_H__)` | `LEGACY_ADR_PRE_PHASE_CNT(X)` | `(((UBYTE)(X)&0x07)<<8)` | 通用常數/硬體欄位定義 |
| 4242 | `!defined(__IO_H__)` | `LEGACY_CMD_PST_PHASE_CNT(X)` | `(((UBYTE)(X)&0x07)<<5)` | 通用常數/硬體欄位定義 |
| 4243 | `!defined(__IO_H__)` | `LEGACY_CMD_WEN_PHASE_CNT(X)` | `(((UBYTE)(X)&0x03)<<3)` | 通用常數/硬體欄位定義 |
| 4244 | `!defined(__IO_H__)` | `LEGACY_CMD_PRE_PHASE_CNT(X)` | `((UBYTE)(X)&0x07)` | 通用常數/硬體欄位定義 |
| 4246 | `!defined(__IO_H__)` | `FCONL_TGL_TIME_CFG` | `(0x68>>2)` | 通用常數/硬體欄位定義 |
| 4247 | `!defined(__IO_H__)` | `TOG_DAT_CLH_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<28)` | 通用常數/硬體欄位定義 |
| 4248 | `!defined(__IO_H__)` | `TOG_DAT_PSTH_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<24)` | 通用常數/硬體欄位定義 |
| 4249 | `!defined(__IO_H__)` | `TOG_DAT_PST_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<20)` | 通用常數/硬體欄位定義 |
| 4250 | `!defined(__IO_H__)` | `TOG_DAT_PRE_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<16)` | 通用常數/硬體欄位定義 |
| 4251 | `!defined(__IO_H__)` | `TOG_ADR_PST_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<12)` | 通用常數/硬體欄位定義 |
| 4252 | `!defined(__IO_H__)` | `TOG_ADR_PRE_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<8)` | 通用常數/硬體欄位定義 |
| 4253 | `!defined(__IO_H__)` | `TOG_CMD_PST_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<4)` | 通用常數/硬體欄位定義 |
| 4254 | `!defined(__IO_H__)` | `TOG_CMD_PRE_PHASE_CNT(X)` | `((UBYTE)(X)&0x0F)` | 通用常數/硬體欄位定義 |
| 4255 | `!defined(__IO_H__)` | `FLH_POST_SIM_TGL_TIME` | `0x00009797` | 通用常數/硬體欄位定義 |
| 4257 | `!defined(__IO_H__)` | `FCONL_ONF_TIME_CFG` | `(0x6C>>2)` | 通用常數/硬體欄位定義 |
| 4258 | `!defined(__IO_H__)` | `TOG_ADR_WEN_PHASE_CNT(X)` | `(((UBYTE)(X)&0x07)<<28)` | 通用常數/硬體欄位定義 |
| 4259 | `!defined(__IO_H__)` | `TOG_CMD_WEN_PHASE_CNT(X)` | `(((UBYTE)(X)&0x07)<<24)` | 通用常數/硬體欄位定義 |
| 4260 | `!defined(__IO_H__)` | `ONFI_tCDQSS_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<20)` | 通用常數/硬體欄位定義 |
| 4261 | `!defined(__IO_H__)` | `ONFI_tCDQSS_EN` | `SET_BIT19` | 通用常數/硬體欄位定義 |
| 4262 | `!defined(__IO_H__)` | `ONFI_DAT_RTN_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<16)` | 通用常數/硬體欄位定義 |
| 4263 | `!defined(__IO_H__)` | `ONFI_DAT_PST_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<12)` | 通用常數/硬體欄位定義 |
| 4264 | `!defined(__IO_H__)` | `ONFI_DAT_PRE_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<8)` | 通用常數/硬體欄位定義 |
| 4265 | `!defined(__IO_H__)` | `ONFI_ADR_PRE_PHASE_CNT(X)` | `(((UBYTE)(X)&0x0F)<<4)` | 通用常數/硬體欄位定義 |
| 4266 | `!defined(__IO_H__)` | `ONFI_CMD_PRE_PHASE_CNT(X)` | `((UBYTE)(X)&0x0F)` | 通用常數/硬體欄位定義 |
| 4268 | `!defined(__IO_H__)` | `FCONL_LGC_TIME_CFG_1` | `(0x70>>2)` | 通用常數/硬體欄位定義 |
| 4269 | `!defined(__IO_H__)` | `FCONL_TGL_TIME_CFG_1` | `(0x74>>2)` | 通用常數/硬體欄位定義 |
| 4270 | `!defined(__IO_H__)` | `FCONL_ONF_TIME_CFG_1` | `(0x78>>2)` | 通用常數/硬體欄位定義 |
| 4275 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_0` | `(0x84>>2)` | 依原始註解：#define FCONL_SPARE_LEN			(0x7C>>2) \| #define FCONL_SPR_LEN_H		(0x80>>2)；註解：#define FCONL_SPARE_LEN			(0x7C>>2) \| #define FCONL_SPR_LEN_H		(0x80>>2) |
| 4276 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_1` | `(0x88>>2)` | 通用常數/硬體欄位定義 |
| 4277 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_2` | `(0x8C>>2)` | 通用常數/硬體欄位定義 |
| 4278 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_3` | `(0x90>>2)` | 通用常數/硬體欄位定義 |
| 4279 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_4` | `(0x94>>2)` | 通用常數/硬體欄位定義 |
| 4280 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_5` | `(0x98>>2)` | 通用常數/硬體欄位定義 |
| 4281 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_6` | `(0x9C>>2)` | 通用常數/硬體欄位定義 |
| 4282 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_7` | `(0xA0>>2)` | 通用常數/硬體欄位定義 |
| 4283 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_8` | `(0xA4>>2)` | 通用常數/硬體欄位定義 |
| 4284 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_9` | `(0xA8>>2)` | 通用常數/硬體欄位定義 |
| 4285 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_10` | `(0xAC>>2)` | 通用常數/硬體欄位定義 |
| 4286 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_11` | `(0xB0>>2)` | 通用常數/硬體欄位定義 |
| 4287 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_12` | `(0xB4>>2)` | 通用常數/硬體欄位定義 |
| 4288 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_13` | `(0xB8>>2)` | 通用常數/硬體欄位定義 |
| 4289 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_14` | `(0xBC>>2)` | 通用常數/硬體欄位定義 |
| 4290 | `!defined(__IO_H__)` | `FCONL_FCE_SEL_15` | `(0xC0>>2)` | 通用常數/硬體欄位定義 |
| 4291 | `!defined(__IO_H__)` | `FCONB_REMAPPING_CE` | `((_UCV unsigned char *)(Flash_Reg_Base+FTOP+0x84))` | 通用常數/硬體欄位定義 |
| 4293 | `!defined(__IO_H__)` | `FCONL_D2I_IRADR` | `(0xC4>>2)` | 通用常數/硬體欄位定義 |
| 4294 | `!defined(__IO_H__)` | `FCONL_D2I_DRADR` | `(0xC8>>2)` | 通用常數/硬體欄位定義 |
| 4295 | `!defined(__IO_H__)` | `FCONL_D2I_CTL` | `(0xCC>>2)` | 通用常數/硬體欄位定義 |
| 4296 | `!defined(__IO_H__)` | `D2I_PHISON_SPARE_FORMAT` | `CLR_BIT5` | 通用常數/硬體欄位定義 |
| 4297 | `!defined(__IO_H__)` | `D2I_TOSHIBA_SPARE_FORMAT` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 4298 | `!defined(__IO_H__)` | `D2I_2K_SPARE_FORMAT` | `CLR_BIT4` | 通用常數/硬體欄位定義 |
| 4299 | `!defined(__IO_H__)` | `D2I_512B_SPARE_FORMAT` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 4300 | `!defined(__IO_H__)` | `DIRECT_128B_MODE` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 4301 | `!defined(__IO_H__)` | `DIRECT_512B_MODE` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 4302 | `!defined(__IO_H__)` | `IRAM2DRAM` | `CLR_BIT1` | 通用常數/硬體欄位定義 |
| 4303 | `!defined(__IO_H__)` | `DRAM2IRAM` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4304 | `!defined(__IO_H__)` | `D2I_I2D_TRIG` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4306 | `!defined(__IO_H__)` | `FCONL_CRC_EN` | `(0xD0>>2)` | 通用常數/硬體欄位定義 |
| 4307 | `!defined(__IO_H__)` | `CRC_16_EN` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4308 | `!defined(__IO_H__)` | `CRC_32_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4310 | `!defined(__IO_H__)` | `FCONL_CRC_FORCE` | `(0xD4>>2)` | 通用常數/硬體欄位定義 |
| 4311 | `!defined(__IO_H__)` | `FCONL_TMR_CTL` | `(0xD8>>2)` | 通用常數/硬體欄位定義 |
| 4312 | `!defined(__IO_H__)` | `TIMER_CPU_INTR_EN` | `SET_BIT25` | 周邊控制暫存器/位元欄位定義 |
| 4313 | `!defined(__IO_H__)` | `TIMER_INTR_EN` | `SET_BIT24` | 周邊控制暫存器/位元欄位定義 |
| 4314 | `!defined(__IO_H__)` | `TIMER_CNT_LEN(X)` | `((U32)(X)&0xFFFFFF)` | 周邊控制暫存器/位元欄位定義 |
| 4318 | `!defined(__IO_H__)` | `FCONL_ALU_BAS` | `(0xE0>>2)` | 依原始註解：#define FCONL_SPARE_LEN_1	(0xDC>>2)；註解：#define FCONL_SPARE_LEN_1	(0xDC>>2) |
| 4319 | `!defined(__IO_H__)` | `FCONL_MGM_INF` | `(0xE4>>2)` | 通用常數/硬體欄位定義 |
| 4320 | `!defined(__IO_H__)` | `FCONL_MT_STS` | `(0xE8>>2)` | 通用常數/硬體欄位定義 |
| 4321 | `!defined(__IO_H__)` | `FCONL_MT_CFG` | `(0xEC>>2)` | 通用常數/硬體欄位定義 |
| 4322 | `!defined(__IO_H__)` | `MTP_GROUP` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 4323 | `!defined(__IO_H__)` | `RS_PROG_PARITY` | `SET_BIT30` | 通用常數/硬體欄位定義 |
| 4324 | `!defined(__IO_H__)` | `RS_OTFENC_EN` | `SET_BIT29` | 通用常數/硬體欄位定義 |
| 4325 | `!defined(__IO_H__)` | `RS_TAG_MT(X)` | `(((UBYTE)(X)&0x1F)<<24)` | 通用常數/硬體欄位定義 |
| 4326 | `!defined(__IO_H__)` | `REUSEMT_STOP` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 4327 | `!defined(__IO_H__)` | `REUSEMT_AUTO_TRIG` | `SET_BIT22` | 通用常數/硬體欄位定義 |
| 4328 | `!defined(__IO_H__)` | `MT_LOCK` | `SET_BIT21` | 通用常數/硬體欄位定義 |
| 4329 | `!defined(__IO_H__)` | `QUEUE_INDEX(X)` | `(((UBYTE)( (gubPlanesPerBurst == 1)?((X)<<1):(X))&0x0F)<<16)` | 通用常數/硬體欄位定義 |
| 4330 | `!defined(__IO_H__)` | `MT_INDEX(X)` | `((UBYTE)(X)&0xFF)` | 通用常數/硬體欄位定義 |
| 4333 | `!defined(__IO_H__)` | `FCONL_ADR_GEN` | `(0xF0>>2)` | 通用常數/硬體欄位定義 |
| 4334 | `!defined(__IO_H__)` | `ADR_GEN_TLC_MODE_EN` | `SET_BIT23` | 通用常數/硬體欄位定義 |
| 4335 | `!defined(__IO_H__)` | `FRAME_PTR_CATCH_MASK_0x0F` | `0x0000F000` | 通用常數/硬體欄位定義 |
| 4336 | `!defined(__IO_H__)` | `FRAME_PTR_CATCH_MASK_0x07` | `0x00007000` | 通用常數/硬體欄位定義 |
| 4337 | `!defined(__IO_H__)` | `FRAME_PTR_CATCH_MASK_0x03` | `0x00003000` | 通用常數/硬體欄位定義 |
| 4338 | `!defined(__IO_H__)` | `FTA_EQUAL_FSA` | `SET_BIT10` | 通用常數/硬體欄位定義 |
| 4339 | `!defined(__IO_H__)` | `COL_ADR_AUTO_GEN` | `SET_BIT9` | 通用常數/硬體欄位定義 |
| 4340 | `!defined(__IO_H__)` | `FRAME_PTR_UPDATE_EN` | `SET_BIT8` | 通用常數/硬體欄位定義 |
| 4341 | `!defined(__IO_H__)` | `FRAME_PTR_UPDATE_NEXT_WHEN_DMA_DONE` | `SET_BIT7` | 周邊控制暫存器/位元欄位定義 |
| 4342 | `!defined(__IO_H__)` | `FRAME_PTR_CATCH_COL_ADR(X)` | `((UBYTE)(X)&0x3F)` | 通用常數/硬體欄位定義 |
| 4344 | `!defined(__IO_H__)` | `FCONL_ADR_GEN_STOP` | `(0xF4>>2)` | 通用常數/硬體欄位定義 |
| 4345 | `!defined(__IO_H__)` | `FCONW_ADR_GEN_STOP` | `(0xF4>>1)` | 通用常數/硬體欄位定義 |
| 4346 | `!defined(__IO_H__)` | `ALU_STOP_CHECK_EN` | `SET_BIT15` | 通用常數/硬體欄位定義 |
| 4347 | `!defined(__IO_H__)` | `ALU_STOP_CHECK_DAT` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 4348 | `!defined(__IO_H__)` | `ALU_STOP_COMP_ADR(X)` | `(((UBYTE)(X)&0x3F)<<8)` | 通用常數/硬體欄位定義 |
| 4349 | `!defined(__IO_H__)` | `ALU_STOP_PTR(X)` | `((UBYTE)(X)&0x07)` | 通用常數/硬體欄位定義 |
| 4351 | `!defined(__IO_H__)` | `FCONL_FCE_SET` | `(0xF8>>2)` | 通用常數/硬體欄位定義 |
| 4352 | `!defined(__IO_H__)` | `FCONL_FCE_CLR` | `(0xFC>>2)` | 通用常數/硬體欄位定義 |
| 4353 | `!defined(__IO_H__)` | `SEL_CE0_CE31` | `CLR_BIT5` | 通用常數/硬體欄位定義 |
| 4354 | `!defined(__IO_H__)` | `SEL_CE32_CE63` | `SET_BIT5` | 通用常數/硬體欄位定義 |
| 4356 | `!defined(__IO_H__)` | `FCONL_MGM_SET` | `(0x100>>2)` | 通用常數/硬體欄位定義 |
| 4357 | `!defined(__IO_H__)` | `MT_GROUP_INFO_CLEAR` | `SET_BIT14` | 通用常數/硬體欄位定義 |
| 4358 | `!defined(__IO_H__)` | `MT_GROUP_INFO_CLEAR_SEL(X)` | `(((UBYTE)(X)&0x3F)<<8)` | 通用常數/硬體欄位定義 |
| 4359 | `!defined(__IO_H__)` | `MT_GROUP_INFO_SEL_GROUP_0_TO_7` | `(CLR_BIT0&CLR_BIT1&CLR_BIT2)` | 通用常數/硬體欄位定義 |
| 4360 | `!defined(__IO_H__)` | `MT_GROUP_INFO_SEL_GROUP_8_TO_15` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4361 | `!defined(__IO_H__)` | `MT_GROUP_INFO_SEL_GROUP_16_TO_23` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4362 | `!defined(__IO_H__)` | `MT_GROUP_INFO_SEL_GROUP_24_TO_31` | `(SET_BIT0\|SET_BIT1)` | 通用常數/硬體欄位定義 |
| 4363 | `!defined(__IO_H__)` | `MT_GROUP_INFO_SEL_GROUP_32_TO_35` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 4365 | `!defined(__IO_H__)` | `FCONL_FW_RESET` | `(0x10C>>2)` | 通用常數/硬體欄位定義 |
| 4366 | `!defined(__IO_H__)` | `FCONL_LDPC_CFG` | `(0x110>>2)` | 通用常數/硬體欄位定義 |
| 4367 | `!defined(__IO_H__)` | `AUTO_UPDATE_LLR_ENABLE` | `BIT28` | 通用常數/硬體欄位定義 |
| 4368 | `!defined(__IO_H__)` | `LDPC_CORRECT_DIS_ENABLE` | `BIT27` | 依原始註解：Default 1；註解：Default 1 |
| 4369 | `!defined(__IO_H__)` | `DSP_ENABLE` | `BIT26` | 通用常數/硬體欄位定義 |
| 4370 | `!defined(__IO_H__)` | `CLR_PAGE_SELECTION` | `(CLR_BIT25&CLR_BIT24)` | 通用常數/硬體欄位定義 |
| 4371 | `!defined(__IO_H__)` | `PAGE_SELECTION_LOWER` | `CLR_PAGE_SELECTION` | 通用常數/硬體欄位定義 |
| 4372 | `!defined(__IO_H__)` | `PAGE_SELECTION_MIDDLE` | `BIT24` | 通用常數/硬體欄位定義 |
| 4373 | `!defined(__IO_H__)` | `PAGE_SELECTION_UPPER` | `BIT25` | 通用常數/硬體欄位定義 |
| 4374 | `!defined(__IO_H__)` | `CLR_LDPC_DECODE_MODE` | `(CLR_BIT6&CLR_BIT5&CLR_BIT4)` | 依原始註解：0xFFFFFF8F //(CLR_BIT6&CLR_BIT5&CLR_BIT4)；註解：0xFFFFFF8F //(CLR_BIT6&CLR_BIT5&CLR_BIT4) |
| 4375 | `!defined(__IO_H__)` | `LDPC_DECODE_MODE_0` | `CLR_LDPC_DECODE_MODE` | 通用常數/硬體欄位定義 |
| 4376 | `!defined(__IO_H__)` | `LDPC_DECODE_MODE_1` | `BIT4` | 通用常數/硬體欄位定義 |
| 4377 | `!defined(__IO_H__)` | `LDPC_DECODE_MODE_2` | `BIT5` | 通用常數/硬體欄位定義 |
| 4378 | `!defined(__IO_H__)` | `LDPC_DECODE_MODE_3` | `(BIT5\|BIT4)` | 通用常數/硬體欄位定義 |
| 4379 | `!defined(__IO_H__)` | `LDPC_DECODE_MODE_4` | `BIT6` | 通用常數/硬體欄位定義 |
| 4380 | `!defined(__IO_H__)` | `CLR_LDPC_MODE` | `(0xFFFFFFF1)` | 通用常數/硬體欄位定義 |
| 4381 | `!defined(__IO_H__)` | `LDPC_MODE(X)` | `(((UBYTE)(X)&0x07)<<1)` | 通用常數/硬體欄位定義 |
| 4382 | `!defined(__IO_H__)` | `LDPC_4K_MODE0` | `0` | 依原始註解：496B per 4K LDPC, 16k page need at least 1984 bytes of spare (496*4 )；註解：496B per 4K LDPC, 16k page need at least 1984 bytes of spare (496*4 ) |
| 4383 | `!defined(__IO_H__)` | `LDPC_4K_MODE1` | `1` | 依原始註解：440B per 4K LDPC, 16k page need at least 1760 bytes of spare (440*4 )；註解：440B per 4K LDPC, 16k page need at least 1760 bytes of spare (440*4 ) |
| 4384 | `!defined(__IO_H__)` | `LDPC_4K_MODE2` | `2` | 依原始註解：420B per 4K LDPC, 16k page need at least 1680 bytes of spare (420*4 )；註解：420B per 4K LDPC, 16k page need at least 1680 bytes of spare (420*4 ) |
| 4385 | `!defined(__IO_H__)` | `LDPC_4K_MODE3` | `3` | 依原始註解：360B per 4K LDPC, 16k page need at least 1440 bytes of spare (360*4 )；註解：360B per 4K LDPC, 16k page need at least 1440 bytes of spare (360*4 ) |
| 4386 | `!defined(__IO_H__)` | `LDPC_4K_MODE4` | `4` | 依原始註解：272B per 4K LDPC, 16k page need at least 1088 bytes of spare (272*4 )；註解：272B per 4K LDPC, 16k page need at least 1088 bytes of spare (272*4 ) |
| 4388 | `!defined(__IO_H__)` | `LDPC_MODE0` | `(~(BIT1\|BIT2\|BIT3))` | 依原始註解：496B；註解：496B |
| 4389 | `!defined(__IO_H__)` | `LDPC_MODE1` | `BIT1` | 依原始註解：440B；註解：440B |
| 4390 | `!defined(__IO_H__)` | `LDPC_MODE2` | `BIT2` | 依原始註解：420B；註解：420B |
| 4391 | `!defined(__IO_H__)` | `LDPC_MODE3` | `(BIT1\|BIT2)` | 依原始註解：360B；註解：360B |
| 4392 | `!defined(__IO_H__)` | `LDPC_MODE4` | `BIT3` | 依原始註解：272B；註解：272B |
| 4393 | `!defined(__IO_H__)` | `LDPC_CORRECT_CIRCUIT_CLEAR` | `CLR_BIT0` | 通用常數/硬體欄位定義 |
| 4405 | `!defined(__IO_H__)` | `FCONL_LDPC_BFC_SET` | `(0x114>>2)` | 依原始註解：/* move here from channel reg #define LDPC_LOWER_PAGE_SEL		(CLR_BIT11&CLR_BIT12)；註解：/* move here from channel reg #define LDPC_LOWER_PAGE_SEL		(CLR_BIT11&CLR_BIT12) #define LDPC_MIDDLE_PAGE_SEL		SET_BIT11 #define LDPC_UPPER_PAGE_SEL		SET_BIT12 #define LDPC_MAX_ITERATION_NUM(X)	(((UBYTE)X&0x7F)<<4) #define LDPC_DECODE_MODE_ONLY_HB		(CLR_BIT0&CLR_BIT1&CLR_BIT2) #define LDPC_DECODE_MODE_HB_SB1			SET_BIT0 #define LDPC_DECODE_MODE_HB_SB1_SB2		SET_BIT1 #define LDPC_DECODE_MODE_HB_SB1_SB2_SB3		(SET_BIT0\|SET_BIT1) #define LDPC_DECODE_MODE_HB_SB1_SB2_SB3_SB4	SET_BIT2 */ |
| 4406 | `!defined(__IO_H__)` | `LDPC_COR_CONVERGE` | `BIT19` | 通用常數/硬體欄位定義 |
| 4407 | `!defined(__IO_H__)` | `CNT_BFC_SEL(X)` | `(((UBYTE)(X)&0x3F)<<8)` | 通用常數/硬體欄位定義 |
| 4408 | `!defined(__IO_H__)` | `FCONL_LDPC_BFC` | `(0x118>>2)` | 通用常數/硬體欄位定義 |
| 4409 | `!defined(__IO_H__)` | `FCONL_PAGE_CFG` | `(0x11C>>2)` | 通用常數/硬體欄位定義 |
| 4410 | `!defined(__IO_H__)` | `FCONL_CLR_PAGE_CFG` | `(~(BIT0\|BIT1))` | 通用常數/硬體欄位定義 |
| 4411 | `!defined(__IO_H__)` | `PHYSICAL_4KPAGE` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4412 | `!defined(__IO_H__)` | `PHYSICAL_8KPAGE` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4413 | `!defined(__IO_H__)` | `PHYSICAL_16KPAGE` | `(SET_BIT0\|SET_BIT1)` | 通用常數/硬體欄位定義 |
| 4415 | `!defined(__IO_H__)` | `FCONL_MT_ADR_BASE` | `(0x120>>2)` | 通用常數/硬體欄位定義 |
| 4416 | `!defined(__IO_H__)` | `MT_GLOBAL_TRIG_EN` | `SET_BIT22` | 通用常數/硬體欄位定義 |
| 4417 | `!defined(__IO_H__)` | `MT_TABLE_SIZE(X)` | `((((X)/8)&0xF)<<18)` | 依原始註解：unit: 8 bytes；註解：unit: 8 bytes |
| 4418 | `!defined(__IO_H__)` | `INT_INDEX_MODE` | `SET_BIT17` | 通用常數/硬體欄位定義 |
| 4419 | `!defined(__IO_H__)` | `MT_BASE_ADDR(X)` | `((UWORD)(X)&0xFFFF)` | 通用常數/硬體欄位定義 |
| 4421 | `!defined(__IO_H__)` | `FCONL_SYS_INTQ_BASE` | `(0x124>>2)` | 通用常數/硬體欄位定義 |
| 4423 | `!defined(__IO_H__)` | `FCON_FLHCLK_SPLITE_EN` | `(0x128>>2)` | 通用常數/硬體欄位定義 |
| 4424 | `!defined(__IO_H__)` | `SPLITE_WR_CLK_EN` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4425 | `!defined(__IO_H__)` | `FCONL_MTQ_INF_0` | `(0x12C>>2)` | 通用常數/硬體欄位定義 |
| 4426 | `!defined(__IO_H__)` | `FCONL_MTQ_INF_1` | `(0x130>>2)` | 通用常數/硬體欄位定義 |
| 4427 | `!defined(__IO_H__)` | `FCONL_MTQ_INF_2` | `(0x134>>2)` | 通用常數/硬體欄位定義 |
| 4428 | `!defined(__IO_H__)` | `FCONL_MTQ_INF_3` | `(0x138>>2)` | 通用常數/硬體欄位定義 |
| 4429 | `!defined(__IO_H__)` | `FCONL_ERASE_PAGE_TH` | `(0x13C>>2)` | 通用常數/硬體欄位定義 |
| 4431 | `!defined(__IO_H__)` | `FCONL_LLR_TABLE_0` | `(0x140>>2)` | 通用常數/硬體欄位定義 |
| 4432 | `!defined(__IO_H__)` | `FCONB_LLR_TABLE_0` | `(0x140>>0)` | 通用常數/硬體欄位定義 |
| 4434 | `!defined(__IO_H__)` | `FCONL_DSP_PARAM_0` | `(0x160>>2)` | 通用常數/硬體欄位定義 |
| 4435 | `!defined(__IO_H__)` | `FCONL_DSP_PARAM_1` | `(0x164>>2)` | 通用常數/硬體欄位定義 |
| 4437 | `!defined(__IO_H__)` | `FCONL_DSP_GRAY_CODE_0` | `(0x168>>2)` | 通用常數/硬體欄位定義 |
| 4438 | `!defined(__IO_H__)` | `FCONL_DSP_GRAY_CODE_1` | `(0x16C>>2)` | 通用常數/硬體欄位定義 |
| 4439 | `!defined(__IO_H__)` | `FCONL_DSP_GRAY_CODE_2` | `(0x170>>2)` | 通用常數/硬體欄位定義 |
| 4440 | `!defined(__IO_H__)` | `FCONL_DSP_GRAY_CODE_3` | `(0x174>>2)` | 通用常數/硬體欄位定義 |
| 4441 | `!defined(__IO_H__)` | `FCONL_DSP_GRAY_CODE_4` | `(0x178>>2)` | 通用常數/硬體欄位定義 |
| 4443 | `!defined(__IO_H__)` | `FCONL_DSP_LUT_0` | `(0x17C>>2)` | 通用常數/硬體欄位定義 |
| 4444 | `!defined(__IO_H__)` | `FCONL_DSP_LUT_1` | `(0x180>>2)` | 通用常數/硬體欄位定義 |
| 4445 | `!defined(__IO_H__)` | `FCONL_DSP_LUT_2` | `(0x184>>2)` | 通用常數/硬體欄位定義 |
| 4446 | `!defined(__IO_H__)` | `FCONL_DSP_LUT_3` | `(0x188>>2)` | 通用常數/硬體欄位定義 |
| 4447 | `!defined(__IO_H__)` | `FCONL_DSP_LUT_4` | `(0x18C>>2)` | 通用常數/硬體欄位定義 |
| 4448 | `!defined(__IO_H__)` | `FCONL_DSP_LUT_5` | `(0x190>>2)` | 通用常數/硬體欄位定義 |
| 4450 | `!defined(__IO_H__)` | `FCONL_DSP_CENTER` | `(0x194>>2)` | 通用常數/硬體欄位定義 |
| 4452 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_00` | `(0x198>>1)` | 通用常數/硬體欄位定義 |
| 4453 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_01` | `(0x19A>>1)` | 通用常數/硬體欄位定義 |
| 4454 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_02` | `(0x19C>>1)` | 通用常數/硬體欄位定義 |
| 4455 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_03` | `(0x19E>>1)` | 通用常數/硬體欄位定義 |
| 4456 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_04` | `(0x1A0>>1)` | 通用常數/硬體欄位定義 |
| 4457 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_05` | `(0x1A2>>1)` | 通用常數/硬體欄位定義 |
| 4458 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_06` | `(0x1A4>>1)` | 通用常數/硬體欄位定義 |
| 4459 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_07` | `(0x1A6>>1)` | 通用常數/硬體欄位定義 |
| 4460 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_08` | `(0x1A8>>1)` | 通用常數/硬體欄位定義 |
| 4461 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_09` | `(0x1AA>>1)` | 通用常數/硬體欄位定義 |
| 4462 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_10` | `(0x1AC>>1)` | 通用常數/硬體欄位定義 |
| 4463 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_11` | `(0x1AE>>1)` | 通用常數/硬體欄位定義 |
| 4464 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_12` | `(0x1B0>>1)` | 通用常數/硬體欄位定義 |
| 4465 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_13` | `(0x1B2>>1)` | 通用常數/硬體欄位定義 |
| 4466 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_14` | `(0x1B4>>1)` | 通用常數/硬體欄位定義 |
| 4467 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_15` | `(0x1B6>>1)` | 通用常數/硬體欄位定義 |
| 4469 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_16` | `(0x1B8>>1)` | 通用常數/硬體欄位定義 |
| 4470 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_17` | `(0x1BA>>1)` | 通用常數/硬體欄位定義 |
| 4471 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_18` | `(0x1BC>>1)` | 通用常數/硬體欄位定義 |
| 4472 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_19` | `(0x1BE>>1)` | 通用常數/硬體欄位定義 |
| 4473 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_20` | `(0x1C0>>1)` | 通用常數/硬體欄位定義 |
| 4474 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_21` | `(0x1C2>>1)` | 通用常數/硬體欄位定義 |
| 4475 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_22` | `(0x1C4>>1)` | 通用常數/硬體欄位定義 |
| 4476 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_23` | `(0x1C6>>1)` | 通用常數/硬體欄位定義 |
| 4477 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_24` | `(0x1C8>>1)` | 通用常數/硬體欄位定義 |
| 4478 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_25` | `(0x1CA>>1)` | 通用常數/硬體欄位定義 |
| 4479 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_26` | `(0x1CC>>1)` | 通用常數/硬體欄位定義 |
| 4480 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_27` | `(0x1CE>>1)` | 通用常數/硬體欄位定義 |
| 4481 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_28` | `(0x1D0>>1)` | 通用常數/硬體欄位定義 |
| 4482 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_29` | `(0x1D2>>1)` | 通用常數/硬體欄位定義 |
| 4483 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_30` | `(0x1D4>>1)` | 通用常數/硬體欄位定義 |
| 4484 | `!defined(__IO_H__)` | `FCONW_DSP_SB_DISTRI_31` | `(0x1D6>>1)` | 通用常數/硬體欄位定義 |
| 4489 | `!defined(__IO_H__)` | `UMC55SP` | `1` | 依原始註解：=================================================== \| PS3111 FLH DCTL Register D；註解：=================================================== \| PS3111 FLH DCTL Register Define \| =================================================== |
| 4490 | `!defined(__IO_H__)` | `RENESAS` | `0` | 通用常數/硬體欄位定義 |
| 4493 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_DLL_CTL` | `(0x00>>2)` | 通用常數/硬體欄位定義 |
| 4494 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_SEARCH_MODE(X)` | `(((UBYTE)(X)&0x03)<<30)` | 通用常數/硬體欄位定義 |
| 4495 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_TRACK_FILTER(X)` | `(((UBYTE)(X)&0x03)<<28)` | 通用常數/硬體欄位定義 |
| 4502 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_TRACK_RESULT(X)` | `(((UWORD)(X)&0x01FF)<<8)` | 依原始註解：/*#define DEGREE_MODE_90	0x04000000 #define DEGREE_MODE_67P5		0x03000000 #define；註解：/*#define DEGREE_MODE_90	0x04000000 #define DEGREE_MODE_67P5		0x03000000 #define DEGREE_MODE_45		0x02000000 #define DEGREE_MODE_22P5		0x01000000 #define DEGREE_MODE_112P5	0x05000000 #define DEGREE_MODE_135		0x06000000*/ |
| 4503 | `!defined(__IO_H__) && (UMC55SP)` | `DLY_CELL_R_SEL_VALUE(X)` | `(((UBYTE)(X)&0x03)<<6)` | 通用常數/硬體欄位定義 |
| 4504 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_EN` | `SET_BIT4` | 通用常數/硬體欄位定義 |
| 4505 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_RSTJ_RST` | `CLR_BIT2` | 通用常數/硬體欄位定義 |
| 4506 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_RSTJ_NORMAL` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 4507 | `!defined(__IO_H__) && (UMC55SP)` | `FLH_STROBE_MODE` | `(CLR_BIT1&CLR_BIT0)` | 通用常數/硬體欄位定義 |
| 4508 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_STROBE_MODE` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4509 | `!defined(__IO_H__) && (UMC55SP)` | `FW_CTL_SDLL_MODE` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4511 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_MDLL_STA` | `(0x04>>2)` | 通用常數/硬體欄位定義 |
| 4512 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_UNDERFLOW` | `SET_BIT3` | 通用常數/硬體欄位定義 |
| 4513 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_OVERFLOW` | `SET_BIT2` | 通用常數/硬體欄位定義 |
| 4514 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_FAIL` | `SET_BIT1` | 通用常數/硬體欄位定義 |
| 4515 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_LOCK` | `SET_BIT0` | 通用常數/硬體欄位定義 |
| 4517 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLW_AUTO_LOAD_PERIOD` | `(0x08>>1)` | 通用常數/硬體欄位定義 |
| 4518 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLB_LOAD_MDLL_TO_SDLL` | `(0x0A)` | 通用常數/硬體欄位定義 |
| 4519 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_LOAD_CTL` | `(0x08>>2)` | 通用常數/硬體欄位定義 |
| 4520 | `!defined(__IO_H__) && (UMC55SP)` | `AUTO_LOAD_TRIG` | `SET_BIT31` | 通用常數/硬體欄位定義 |
| 4521 | `!defined(__IO_H__) && (UMC55SP)` | `MDLL_AVG_REG_LOAD_MDLL` | `SET_BIT30` | 系統暫存器/記憶體映射位址定義 |
| 4522 | `!defined(__IO_H__) && (UMC55SP)` | `LOAD_WITH_DMA_TRIG_EN` | `SET_BIT28` | 周邊控制暫存器/位元欄位定義 |
| 4523 | `!defined(__IO_H__) && (UMC55SP)` | `AUTO_EN_MDLL_EN` | `SET_BIT26` | 通用常數/硬體欄位定義 |
| 4524 | `!defined(__IO_H__) && (UMC55SP)` | `AUTO_RESET_MDLL_EN` | `SET_BIT25` | 通用常數/硬體欄位定義 |
| 4525 | `!defined(__IO_H__) && (UMC55SP)` | `AUTO_LOAD_EN` | `SET_BIT24` | 通用常數/硬體欄位定義 |
| 4526 | `!defined(__IO_H__) && (UMC55SP)` | `AUTO_LOAD_DIS` | `CLR_BIT24` | 通用常數/硬體欄位定義 |
| 4528 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_OFFSET` | `(0x0C>>2)` | 通用常數/硬體欄位定義 |
| 4529 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_OFFSET_WRITE(X)` | `(((U32)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4530 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_OFFSET_READ(X)` | `((U32)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4532 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_PHASE_R_CTL` | `(0x10>>2)` | 通用常數/硬體欄位定義 |
| 4533 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_PHASE_W_CTL` | `(0x14>>2)` | 通用常數/硬體欄位定義 |
| 4534 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_CH7(X)` | `(((UBYTE)(X)&0x07)<<21)` | 通用常數/硬體欄位定義 |
| 4535 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_CH6(X)` | `(((UBYTE)(X)&0x07)<<18)` | 通用常數/硬體欄位定義 |
| 4536 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_CH5(X)` | `(((UBYTE)(X)&0x07)<<15)` | 通用常數/硬體欄位定義 |
| 4537 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_CH4(X)` | `(((UBYTE)(X)&0x07)<<12)` | 通用常數/硬體欄位定義 |
| 4538 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_CH3(X)` | `(((UBYTE)(X)&0x07)<<9)` | 通用常數/硬體欄位定義 |
| 4539 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_CH2(X)` | `(((UBYTE)(X)&0x07)<<6)` | 通用常數/硬體欄位定義 |
| 4540 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_CH1(X)` | `(((UBYTE)(X)&0x07)<<3)` | 通用常數/硬體欄位定義 |
| 4541 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_CH0(X)` | `((UBYTE)(X)&0x07)` | 通用常數/硬體欄位定義 |
| 4542 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_90` | `0x04` | 通用常數/硬體欄位定義 |
| 4543 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_67P5` | `0x03` | 通用常數/硬體欄位定義 |
| 4544 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_45` | `0x02` | 通用常數/硬體欄位定義 |
| 4545 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_22P5` | `0x01` | 通用常數/硬體欄位定義 |
| 4546 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_112P5` | `0x05` | 通用常數/硬體欄位定義 |
| 4547 | `!defined(__IO_H__) && (UMC55SP)` | `DEGREE_135` | `0x06` | 通用常數/硬體欄位定義 |
| 4549 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLW_GCK_DLY` | `(0x1C>>1)` | 通用常數/硬體欄位定義 |
| 4550 | `!defined(__IO_H__) && (UMC55SP)` | `GCK_R_SEL(X)` | `(((UBYTE)(X)&0x03)<<12)` | 通用常數/硬體欄位定義 |
| 4551 | `!defined(__IO_H__) && (UMC55SP)` | `GCK_SEL(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4553 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SEL_VAL_0` | `(0x20>>2)` | 通用常數/硬體欄位定義 |
| 4554 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLW_SEL_VAL_0` | `(0x20>>1)` | 通用常數/硬體欄位定義 |
| 4555 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_WRITE_CH0(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4556 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_READ_CH0(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4558 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SEL_VAL_1` | `(0x24>>2)` | 通用常數/硬體欄位定義 |
| 4559 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLW_SEL_VAL_1` | `(0x24>>1)` | 通用常數/硬體欄位定義 |
| 4560 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_WRITE_CH1(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4561 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_READ_CH1(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4563 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SEL_VAL_2` | `(0x28>>2)` | 通用常數/硬體欄位定義 |
| 4564 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_WRITE_CH2(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4565 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_READ_CH2(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4567 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SEL_VAL_3` | `(0x2C>>2)` | 通用常數/硬體欄位定義 |
| 4568 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_WRITE_CH3(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4569 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_READ_CH3(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4571 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SEL_VAL_4` | `(0x30>>2)` | 通用常數/硬體欄位定義 |
| 4572 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_WRITE_CH4(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4573 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_READ_CH4(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4575 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SEL_VAL_5` | `(0x34>>2)` | 通用常數/硬體欄位定義 |
| 4576 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_WRITE_CH5(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4577 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_READ_CH5(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4579 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SEL_VAL_6` | `(0x38>>2)` | 通用常數/硬體欄位定義 |
| 4580 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_WRITE_CH6(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4581 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_READ_CH6(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4583 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SEL_VAL_7` | `(0x3C>>2)` | 通用常數/硬體欄位定義 |
| 4584 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_WRITE_CH7(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4585 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_VALUE_READ_CH7(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4587 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_VAL_0` | `(0x40>>2)` | 通用常數/硬體欄位定義 |
| 4588 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_WR_CH0(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4589 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_RD_CH0(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4591 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_VAL_1` | `(0x44>>2)` | 通用常數/硬體欄位定義 |
| 4592 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_WR_CH1(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4593 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_RD_CH1(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4595 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_VAL_2` | `(0x48>>2)` | 通用常數/硬體欄位定義 |
| 4596 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_WR_CH2(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4597 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_RD_CH2(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4599 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_VAL_3` | `(0x4C>>2)` | 通用常數/硬體欄位定義 |
| 4600 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_WR_CH3(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4601 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_RD_CH3(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4603 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_VAL_4` | `(0x50>>2)` | 通用常數/硬體欄位定義 |
| 4604 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_WR_CH4(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4605 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_RD_CH4(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4607 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_VAL_5` | `(0x54>>2)` | 通用常數/硬體欄位定義 |
| 4608 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_WR_CH5(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4609 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_RD_CH5(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4611 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_VAL_6` | `(0x58>>2)` | 通用常數/硬體欄位定義 |
| 4612 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_WR_CH6(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4613 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_RD_CH6(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4615 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_VAL_7` | `(0x5C>>2)` | 通用常數/硬體欄位定義 |
| 4616 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_WR_CH7(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4617 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_VAL_RD_CH7(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4621 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_OFFSET_0` | `(0x60>>2)` | 通用常數/硬體欄位定義 |
| 4622 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_WR_CH0(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4623 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_RD_CH0(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4625 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_OFFSET_1` | `(0x64>>2)` | 通用常數/硬體欄位定義 |
| 4626 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_WR_CH1(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4627 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_RD_CH1(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4629 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_OFFSET_2` | `(0x68>>2)` | 通用常數/硬體欄位定義 |
| 4630 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_WR_CH2(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4631 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_RD_CH2(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4633 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_OFFSET_3` | `(0x6C>>2)` | 通用常數/硬體欄位定義 |
| 4634 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_WR_CH3(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4635 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_RD_CH3(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4637 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_OFFSET_4` | `(0x70>>2)` | 通用常數/硬體欄位定義 |
| 4638 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_WR_CH4(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4639 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_RD_CH4(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4641 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_OFFSET_5` | `(0x74>>2)` | 通用常數/硬體欄位定義 |
| 4642 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_WR_CH5(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4643 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_RD_CH5(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4645 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_OFFSET_6` | `(0x78>>2)` | 通用常數/硬體欄位定義 |
| 4646 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_WR_CH6(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4647 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_RD_CH6(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4649 | `!defined(__IO_H__) && (UMC55SP)` | `FDLLL_SDLL_OFFSET_7` | `(0x7C>>2)` | 通用常數/硬體欄位定義 |
| 4650 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_WR_CH7(X)` | `(((UWORD)(X)&0x01FF)<<16)` | 通用常數/硬體欄位定義 |
| 4651 | `!defined(__IO_H__) && (UMC55SP)` | `SDLL_DLY_OFFSET_RD_CH7(X)` | `((UWORD)(X)&0x01FF)` | 通用常數/硬體欄位定義 |
| 4654 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_DLL_CTL` | `(0x00>>2)` | 通用常數/硬體欄位定義 |
| 4655 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_UPDATEA` | `BIT29` | 通用常數/硬體欄位定義 |
| 4656 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_UPDATEB` | `BIT12` | 通用常數/硬體欄位定義 |
| 4658 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_DLL_CTL_1` | `(0x04>>2)` | 通用常數/硬體欄位定義 |
| 4659 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_EN` | `BIT31` | 通用常數/硬體欄位定義 |
| 4660 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_RSTB` | `BIT30` | 通用常數/硬體欄位定義 |
| 4661 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FLH_STROBE_MODE` | `(CLR_BIT28&CLR_BIT29)` | 通用常數/硬體欄位定義 |
| 4662 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FW_CTL_SDLL_MODE` | `SET_BIT28` | 通用常數/硬體欄位定義 |
| 4663 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_PHDETFLT` | `BIT27` | 通用常數/硬體欄位定義 |
| 4664 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_LOAD_EN_7` | `BIT19` | 通用常數/硬體欄位定義 |
| 4665 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_LOAD_EN_6` | `BIT18` | 通用常數/硬體欄位定義 |
| 4666 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_LOAD_EN_5` | `BIT17` | 通用常數/硬體欄位定義 |
| 4667 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_LOAD_EN_4` | `BIT16` | 通用常數/硬體欄位定義 |
| 4668 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_LOAD_EN_3` | `BIT15` | 通用常數/硬體欄位定義 |
| 4669 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_LOAD_EN_2` | `BIT14` | 通用常數/硬體欄位定義 |
| 4670 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_LOAD_EN_1` | `BIT13` | 通用常數/硬體欄位定義 |
| 4671 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_LOAD_EN_0` | `BIT12` | 通用常數/硬體欄位定義 |
| 4673 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_MDLL_STA` | `(0x08>>2)` | 通用常數/硬體欄位定義 |
| 4674 | `!defined(__IO_H__) && ELIF((RENESAS))` | `MDLL_OVER_FLOW_SA` | `BIT4` | 通用常數/硬體欄位定義 |
| 4675 | `!defined(__IO_H__) && ELIF((RENESAS))` | `MDLL_UNDER_FLOW_SA` | `BIT3` | 通用常數/硬體欄位定義 |
| 4676 | `!defined(__IO_H__) && ELIF((RENESAS))` | `MDLL_OVER_FLOW` | `BIT2` | 通用常數/硬體欄位定義 |
| 4677 | `!defined(__IO_H__) && ELIF((RENESAS))` | `MDLL_UNDER_FLOW` | `BIT1` | 通用常數/硬體欄位定義 |
| 4678 | `!defined(__IO_H__) && ELIF((RENESAS))` | `MDLL_LOCK` | `BIT0` | 通用常數/硬體欄位定義 |
| 4680 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_MDLL_STA_1` | `(0x0C>>2)` | 通用常數/硬體欄位定義 |
| 4681 | `!defined(__IO_H__) && ELIF((RENESAS))` | `MDLL_OVER_FLOW_SB` | `BIT17` | 通用常數/硬體欄位定義 |
| 4682 | `!defined(__IO_H__) && ELIF((RENESAS))` | `MDLL_UNDER_FLOW_SB` | `BIT16` | 通用常數/硬體欄位定義 |
| 4684 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_TMR` | `(0x10>>2)` | 通用常數/硬體欄位定義 |
| 4685 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_TRIG_LOAD_EN` | `BIT9` | 通用常數/硬體欄位定義 |
| 4686 | `!defined(__IO_H__) && ELIF((RENESAS))` | `CR_TIMER_LOAD_ENABLE` | `BIT8` | 周邊控制暫存器/位元欄位定義 |
| 4688 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLW_GCK_DLY` | `(0x14>>1)` | 通用常數/硬體欄位定義 |
| 4689 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_SEL_VAL_0` | `(0x18>>2)` | 通用常數/硬體欄位定義 |
| 4690 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_SEL_VAL_1` | `(0x1C>>2)` | 通用常數/硬體欄位定義 |
| 4691 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_SEL_VAL_2` | `(0x20>>2)` | 通用常數/硬體欄位定義 |
| 4692 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_SEL_VAL_3` | `(0x24>>2)` | 通用常數/硬體欄位定義 |
| 4693 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_SEL_VAL_4` | `(0x28>>2)` | 通用常數/硬體欄位定義 |
| 4694 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_SEL_VAL_5` | `(0x2C>>2)` | 通用常數/硬體欄位定義 |
| 4695 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_SEL_VAL_6` | `(0x30>>2)` | 通用常數/硬體欄位定義 |
| 4696 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_SEL_VAL_7` | `(0x34>>2)` | 通用常數/硬體欄位定義 |
| 4697 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_VAL_0` | `(0x38>>2)` | 通用常數/硬體欄位定義 |
| 4698 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_VAL_1` | `(0x3C>>2)` | 通用常數/硬體欄位定義 |
| 4699 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_VAL_2` | `(0x40>>2)` | 通用常數/硬體欄位定義 |
| 4700 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_VAL_3` | `(0x44>>2)` | 通用常數/硬體欄位定義 |
| 4701 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_VAL_4` | `(0x48>>2)` | 通用常數/硬體欄位定義 |
| 4702 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_VAL_5` | `(0x4C>>2)` | 通用常數/硬體欄位定義 |
| 4703 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_VAL_6` | `(0x50>>2)` | 通用常數/硬體欄位定義 |
| 4704 | `!defined(__IO_H__) && ELIF((RENESAS))` | `FDLLL_SDLL_VAL_7` | `(0x54>>2)` | 通用常數/硬體欄位定義 |
| 4711 | `!defined(__IO_H__)` | `DCCM0_BANK0_STARADDRESS` | `0x5C0C0000` | 依原始註解：=================================================== \| PS3111 DMAC & SS & SATA Do；註解：=================================================== \| PS3111 DMAC & SS & SATA Doorbell Define \| =================================================== |
| 4712 | `!defined(__IO_H__)` | `DCCM0_BANK1_STARADDRESS` | `0x5C0C8000` | 通用常數/硬體欄位定義 |
| 4713 | `!defined(__IO_H__)` | `DCCM1_BANK0_STARADDRESS` | `0x5C0D0000` | 通用常數/硬體欄位定義 |
| 4714 | `!defined(__IO_H__)` | `DCCM1_BANK1_STARADDRESS` | `0x5C0D4000` | 通用常數/硬體欄位定義 |
| 4716 | `!defined(__IO_H__)` | `SEB` | `((volatile unsigned char * ) (DCCM0_BANK0_STARADDRESS))` | 通用常數/硬體欄位定義 |
| 4717 | `!defined(__IO_H__)` | `SEL` | `((volatile unsigned long * ) (DCCM0_BANK0_STARADDRESS))` | 通用常數/硬體欄位定義 |
| 4718 | `!defined(__IO_H__)` | `DMACB` | `((volatile unsigned char * ) (DCCM0_BANK0_STARADDRESS))` | 周邊控制暫存器/位元欄位定義 |
| 4719 | `!defined(__IO_H__)` | `DMACL` | `((volatile unsigned long * ) (DCCM0_BANK0_STARADDRESS))` | 周邊控制暫存器/位元欄位定義 |
| 4720 | `!defined(__IO_H__)` | `DCCMSATAB` | `((volatile unsigned char * ) (DCCM0_BANK0_STARADDRESS))` | 通用常數/硬體欄位定義 |
| 4721 | `!defined(__IO_H__)` | `DCCMSATAL` | `((volatile unsigned long * ) (DCCM0_BANK0_STARADDRESS))` | 通用常數/硬體欄位定義 |
| 4723 | `!defined(__IO_H__)` | `DCCM0_BANK0_BASE_BYTE` | `((volatile unsigned char *)DCCM0_BANK0_STARADDRESS)` | 通用常數/硬體欄位定義 |
| 4724 | `!defined(__IO_H__)` | `DCCM0_BANK0_BASE_LONG` | `((volatile unsigned long *)DCCM0_BANK0_STARADDRESS)` | 通用常數/硬體欄位定義 |
| 4726 | `!defined(__IO_H__) && RDT` | `TxMainStackSize` | `3072` | 通用常數/硬體欄位定義 |
| 4727 | `!defined(__IO_H__) && RDT` | `TxFTLStackSize` | `1536` | 依原始註解：2816；註解：2816 |
| 4728 | `!defined(__IO_H__) && RDT` | `TxFLAStackSize` | `1536` | 通用常數/硬體欄位定義 |
| 4730 | `!defined(__IO_H__) && ELSE` | `TxMainStackSize` | `1696` | 依原始註解：1792 // PPS test pettern will hit stack overflow while CMD RW w/o retry；註解：1792 // PPS test pettern will hit stack overflow while CMD RW w/o retry |
| 4731 | `!defined(__IO_H__) && ELSE` | `TxFTLStackSize` | `2912` | 依原始註解：2816 //SVN2213kҦպAstacktmWTSB TLC, קKmainzstack；註解：2816 //SVN2213kҦպAstacktmWTSB TLC, קKmainzstack |
| 4732 | `!defined(__IO_H__) && ELSE` | `TxFLAStackSize` | `2048` | 通用常數/硬體欄位定義 |
| 4734 | `!defined(__IO_H__)` | `TxTotalStackSize` | `(TxMainStackSize + TxFTLStackSize + TxFLAStackSize)` | 依原始註解：total 5k；註解：total 5k |
| 4735 | `!defined(__IO_H__)` | `TxThreadStackArea0` | `DCCM0_BANK1_STARADDRESS` | 通用常數/硬體欄位定義 |
| 4736 | `!defined(__IO_H__)` | `TxThreadStackArea1` | `(TxThreadStackArea0 + TxMainStackSize)` | 通用常數/硬體欄位定義 |
| 4737 | `!defined(__IO_H__)` | `TxThreadStackArea2` | `(TxThreadStackArea1 + TxFTLStackSize)` | 通用常數/硬體欄位定義 |
| 4748 | `!defined(__IO_H__)` | `MaxSQCnt_SE` | `4` | 依原始註解：SATA  	Zinfo : 	1 * 64 \| Flash	Message : 	1 * 512 \| ============================；註解：SATA  	Zinfo : 	1 * 64 \| Flash	Message : 	1 * 512 \| ============================================ |
| 4749 | `!defined(__IO_H__)` | `MaxCQCnt_SE` | `4` | 通用常數/硬體欄位定義 |
| 4750 | `!defined(__IO_H__)` | `MaxSQBUF` | `(MaxSQCnt_SE*8)` | 通用常數/硬體欄位定義 |
| 4751 | `!defined(__IO_H__)` | `MaxSQBUFMASK` | `(MaxSQBUF-1)` | 通用常數/硬體欄位定義 |
| 4752 | `!defined(__IO_H__)` | `MaxCQBUF` | `(MaxCQCnt_SE*8)` | 通用常數/硬體欄位定義 |
| 4753 | `!defined(__IO_H__)` | `MaxCQBUFMASK` | `(MaxCQBUF-1)` | 通用常數/硬體欄位定義 |
| 4755 | `!defined(__IO_H__)` | `MaxSQCnt_DMAC` | `16` | 周邊控制暫存器/位元欄位定義 |
| 4756 | `!defined(__IO_H__)` | `MaxCQCnt_DMAC` | `16` | 周邊控制暫存器/位元欄位定義 |
| 4758 | `!defined(__IO_H__)` | `MaxQLength_SATA` | `128` | 通用常數/硬體欄位定義 |
| 4759 | `!defined(__IO_H__)` | `Spare4kSize` | `16` | 通用常數/硬體欄位定義 |
| 4760 | `!defined(__IO_H__)` | `Spare16kSize` | `(Spare4kSize*4)` | 通用常數/硬體欄位定義 |
| 4762 | `!defined(__IO_H__)` | `DOOR_BELL_STARTADDRESS` | `(DCCM0_BANK1_STARADDRESS + TxTotalStackSize)` | 通用常數/硬體欄位定義 |
| 4763 | `!defined(__IO_H__)` | `Quue_Addr_DMAC_SQ` | `DOOR_BELL_STARTADDRESS` | 周邊控制暫存器/位元欄位定義 |
| 4764 | `!defined(__IO_H__)` | `Quue_Addr_DMAC_CQ` | `(Quue_Addr_DMAC_SQ + (32 * MaxSQCnt_DMAC))` | 周邊控制暫存器/位元欄位定義 |
| 4765 | `!defined(__IO_H__)` | `Quue_Addr_SS_SQ` | `(Quue_Addr_DMAC_CQ + (8 * MaxCQCnt_DMAC))` | 通用常數/硬體欄位定義 |
| 4766 | `!defined(__IO_H__)` | `Quue_Addr_SS_CQ` | `(Quue_Addr_SS_SQ + (32 * MaxSQCnt_SE))` | 通用常數/硬體欄位定義 |
| 4767 | `!defined(__IO_H__)` | `Quue_Addr_SATA_DoneTag` | `(Quue_Addr_SS_CQ + (32 * MaxCQCnt_SE))` | 通用常數/硬體欄位定義 |
| 4768 | `!defined(__IO_H__)` | `Quue_Addr_SATA_Zinfo` | `(Quue_Addr_SATA_DoneTag + MaxQLength_SATA)` | 通用常數/硬體欄位定義 |
| 4769 | `!defined(__IO_H__)` | `Quue_Addr_FLH` | `(Quue_Addr_SATA_Zinfo + 64 + 16)` | 依原始註解：64byte zinfo + 4byte msg_type3  //+4 nƳ；註解：64byte zinfo + 4byte msg_type3  //+4 nƳ |
| 4771 | `!defined(__IO_H__)` | `DMACL_SQBaseAddr` | `((volatile unsigned long *)Quue_Addr_DMAC_SQ)` | 周邊控制暫存器/位元欄位定義；註解：Flash INTTable\`@512B. door bell\`@F1616Byte |
| 4772 | `!defined(__IO_H__)` | `DMACB_CQBaseAddr` | `((volatile unsigned char *)Quue_Addr_DMAC_CQ)` | 周邊控制暫存器/位元欄位定義 |
| 4773 | `!defined(__IO_H__)` | `DMACL_CQBaseAddr` | `((volatile unsigned long *)Quue_Addr_DMAC_CQ)` | 周邊控制暫存器/位元欄位定義 |
| 4774 | `!defined(__IO_H__)` | `SEL_SQBaseAddr` | `((volatile unsigned long *)Quue_Addr_SS_SQ)` | 通用常數/硬體欄位定義 |
| 4775 | `!defined(__IO_H__)` | `SEL_CQBaseAddr` | `((volatile unsigned long *)Quue_Addr_SS_CQ)` | 通用常數/硬體欄位定義 |
| 4777 | `!defined(__IO_H__)` | `SEB_SQ_WPIU` | `(0x4C)` | 通用常數/硬體欄位定義 |
| 4778 | `!defined(__IO_H__)` | `SEB_CQ_RPIU` | `(0x70)` | 通用常數/硬體欄位定義 |
| 4779 | `!defined(__IO_H__)` | `SEB_SQ_CTRL` | `(0x57)` | 通用常數/硬體欄位定義 |
| 4780 | `!defined(__IO_H__)` | `SEB_CQ_CTRL` | `(0x77)` | 通用常數/硬體欄位定義 |
| 4781 | `!defined(__IO_H__)` | `SEB_SQ_WR_CNT` | `(0x4e)` | 通用常數/硬體欄位定義 |
| 4782 | `!defined(__IO_H__)` | `SEB_CQ_RD_CNT` | `(0x72)` | 通用常數/硬體欄位定義 |
| 4784 | `!defined(__IO_H__)` | `DMACB_SQ_WPIU` | `(0x0C)` | 周邊控制暫存器/位元欄位定義 |
| 4785 | `!defined(__IO_H__)` | `DMACB_CQ_RPIU` | `(0x30)` | 周邊控制暫存器/位元欄位定義 |
| 4786 | `!defined(__IO_H__)` | `DMACB_SQ_CTRL` | `(0x17)` | 周邊控制暫存器/位元欄位定義 |
| 4787 | `!defined(__IO_H__)` | `DMACB_CQ_CTRL` | `(0x37)` | 周邊控制暫存器/位元欄位定義 |
| 4788 | `!defined(__IO_H__)` | `DMACB_SQ_WR_CNT` | `(0x0e)` | 周邊控制暫存器/位元欄位定義 |
| 4789 | `!defined(__IO_H__)` | `DMACB_CQ_RD_CNT` | `(0x32)` | 周邊控制暫存器/位元欄位定義 |
| 4791 | `!defined(__IO_H__)` | `SATAL_DONETAG` | `(0xC8>>2)` | 通用常數/硬體欄位定義 |
| 4792 | `!defined(__IO_H__)` | `SATAB_WPIU` | `(0xCC)` | 通用常數/硬體欄位定義 |
| 4793 | `!defined(__IO_H__)` | `SATAB_RPIU` | `(0xD0)` | 通用常數/硬體欄位定義 |
| 4794 | `!defined(__IO_H__)` | `SATAB_CTRL` | `(0xD7)` | 通用常數/硬體欄位定義 |
| 4795 | `!defined(__IO_H__)` | `SATAB_WR_CNT` | `(0xCE)` | 通用常數/硬體欄位定義 |
| 4796 | `!defined(__IO_H__)` | `SATAB_RD_CNT` | `(0xD2)` | 通用常數/硬體欄位定義 |
| 4798 | `!defined(__IO_H__)` | `DONETAG_PTR` | `((volatile unsigned long *)(DCCM0_BANK0_STARADDRESS+0xC8))` | 通用常數/硬體欄位定義 |
| 4799 | `!defined(__IO_H__)` | `DONETAG` | `((volatile unsigned char *)*DONETAG_PTR)` | 通用常數/硬體欄位定義 |
| 4801 | `!defined(__IO_H__)` | `ACCL_BaseAddr` | `((volatile unsigned long *)(DCCM0_BANK0_STARADDRESS + 0x210))` | 通用常數/硬體欄位定義 |
| 4802 | `!defined(__IO_H__)` | `ACCB_BaseAddr` | `((volatile unsigned char *)(DCCM0_BANK0_STARADDRESS + 0x210))` | 通用常數/硬體欄位定義 |
| 4804 | `!defined(__IO_H__)` | `ACC_L2P` | `0` | 通用常數/硬體欄位定義 |
| 4805 | `!defined(__IO_H__)` | `ACC_PCA` | `(0x4>>2)` | 通用常數/硬體欄位定義 |
| 4806 | `!defined(__IO_H__)` | `ACC_OFFSET` | `(0x8)` | 通用常數/硬體欄位定義 |
| 4807 | `!defined(__IO_H__)` | `ACC_LENGTH` | `(0x9)` | 通用常數/硬體欄位定義 |
| 4808 | `!defined(__IO_H__)` | `ACC_MODE` | `(0xA)` | 通用常數/硬體欄位定義 |
| 4809 | `!defined(__IO_H__)` | `ACC_MASK` | `(0xB)` | 通用常數/硬體欄位定義 |
| 4810 | `!defined(__IO_H__)` | `ACC_ZIPMAP` | `(0xC>>2)` | 通用常數/硬體欄位定義 |
| 4816 | `!defined(__IO_H__)` | `SYSB_MUX_CTRL1` | `(0x204>>0)` | 系統暫存器/記憶體映射位址定義；註解：====================================== \| /** *	System MUX Control (ODT) */ |
| 4817 | `!defined(__IO_H__)` | `CR_MUX_FLH_ODT` | `BIT7` | 通用常數/硬體欄位定義 |
| 4823 | `!defined(__IO_H__)` | `SYSL_PAD_FDQS0_CFG` | `(0x21A0>>2)` | 系統暫存器/記憶體映射位址定義；註解：-------------------------------------------------------------- \| /* *	PAD Control Registers */ \| CH0 |
| 4824 | `!defined(__IO_H__)` | `SET_FDQS0_ODT` | `BIT27` | 通用常數/硬體欄位定義 |
| 4825 | `!defined(__IO_H__)` | `SET_ODT_VALUE(x)` | `(x<<28)` | 通用常數/硬體欄位定義 |
| 4826 | `!defined(__IO_H__)` | `SYSL_PAD_FDQS1_CFG` | `(0x21A4>>2)` | 系統暫存器/記憶體映射位址定義；註解：CH1 |
| 4827 | `!defined(__IO_H__)` | `SET_FDQS1_ODT` | `BIT27` | 通用常數/硬體欄位定義 |
| 4828 | `!defined(__IO_H__)` | `SYSL_PAD_FDAT0_CFG` | `(0x21C0>>2)` | 系統暫存器/記憶體映射位址定義；註解：CH0 |
| 4829 | `!defined(__IO_H__)` | `SET_FDAT0_ODT` | `BIT27` | 通用常數/硬體欄位定義 |
| 4830 | `!defined(__IO_H__)` | `SYSL_PAD_FDAT1_CFG` | `(0x21C4>>2)` | 系統暫存器/記憶體映射位址定義；註解：CH1 |
| 4831 | `!defined(__IO_H__)` | `SET_FDAT1_ODT` | `BIT27` | 通用常數/硬體欄位定義 |
| 4834 | `!defined(__IO_H__)` | `Toggle_Asyn40` | `0` | 依原始註解：/* Toggle Mode */；註解：/* Toggle Mode */ |
| 4835 | `!defined(__IO_H__)` | `Toggle_200` | `1` | 通用常數/硬體欄位定義 |
| 4836 | `!defined(__IO_H__)` | `Toggle_400` | `2` | 通用常數/硬體欄位定義 |
| 4837 | `!defined(__IO_H__)` | `Toggle_533` | `3` | 通用常數/硬體欄位定義 |
| 4840 | `!defined(__IO_H__)` | `ReadMode` | `0` | 通用常數/硬體欄位定義 |
| 4841 | `!defined(__IO_H__)` | `ProgramMode` | `1` | 通用常數/硬體欄位定義 |
| 4842 | `!defined(__IO_H__)` | `RW_Mode` | `2` | 通用常數/硬體欄位定義 |
| 4843 | `!defined(__IO_H__)` | `No_Track` | `3` | 通用常數/硬體欄位定義 |
| 4846 | `!defined(__IO_H__)` | `TH58TEG7DDJ` | `0` | 依原始註解：/* Flash Product Part ID */ \| (Old)；註解：/* Flash Product Part ID */ \| (Old) |
| 4847 | `!defined(__IO_H__)` | `TH58TEG8DDJ` | `0` | 依原始註解：(Old)；註解：(Old) |
| 4848 | `!defined(__IO_H__)` | `TH58TFG9DFK` | `1` | 依原始註解：A19 (New) (Total 16 CEs)；註解：A19 (New) (Total 16 CEs) |
| 4849 | `!defined(__IO_H__)` | `TH58TFG8DDL` | `0` | 依原始註解：1ZG8 (*1 chip)；註解：1ZG8 (*1 chip) |
| 4850 | `!defined(__IO_H__)` | `TH58TFG9DDL` | `0` | 依原始註解：1ZG9 (*1 chip)；註解：1ZG9 (*1 chip) |
| 4851 | `!defined(__IO_H__)` | `TH58TFT0DDL` | `0` | 依原始註解：Also for (1ZG9*2 chip)；註解：Also for (1ZG9*2 chip) |
| 4852 | `!defined(__IO_H__)` | `TH58TFG9DDL_X4` | `0` | 依原始註解：1ZG9 (*4 chip)；註解：1ZG9 (*4 chip) |
| 4855 | `!defined(__IO_H__)` | `USE_PAD_DRIVE_VALUE` | `2` | 依原始註解：3// 2；註解：3// 2 |
| 4856 | `!defined(__IO_H__)` | `USE_FLH_ODT_VALUE` | `1` | 依原始註解：3// 1；註解：3// 1 |
| 4859 | `!defined(__IO_H__) && TH58TEG7DDJ` | `DLL_SDLL_OFFSET_READ_CH0_VALUE` | `34` | 通用常數/硬體欄位定義 |
| 4860 | `!defined(__IO_H__) && TH58TEG7DDJ` | `DLL_SDLL_OFFSET_READ_CH1_VALUE` | `508` | 通用常數/硬體欄位定義 |
| 4861 | `!defined(__IO_H__) && TH58TEG7DDJ` | `DLL_SDLL_OFFSET_READ533_CH0_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4862 | `!defined(__IO_H__) && TH58TEG7DDJ` | `DLL_SDLL_OFFSET_READ533_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4863 | `!defined(__IO_H__) && TH58TEG7DDJ` | `DLL_SDLL1_OFFSET_WRITE_CH0_VALUE` | `5` | 通用常數/硬體欄位定義 |
| 4864 | `!defined(__IO_H__) && TH58TEG7DDJ` | `DLL_SDLL1_OFFSET_WRITE_CH1_VALUE` | `4` | 通用常數/硬體欄位定義 |
| 4865 | `!defined(__IO_H__) && TH58TEG7DDJ` | `DLL_SDLL1_OFFSET_WRITE533_CH0_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4866 | `!defined(__IO_H__) && TH58TEG7DDJ` | `DLL_SDLL1_OFFSET_WRITE533_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4868 | `!defined(__IO_H__) && ELIF(TH58TEG8DDJ)` | `DLL_SDLL_OFFSET_READ_CH0_VALUE` | `34` | 通用常數/硬體欄位定義 |
| 4869 | `!defined(__IO_H__) && ELIF(TH58TEG8DDJ)` | `DLL_SDLL_OFFSET_READ_CH1_VALUE` | `508` | 通用常數/硬體欄位定義 |
| 4870 | `!defined(__IO_H__) && ELIF(TH58TEG8DDJ)` | `DLL_SDLL_OFFSET_READ533_CH0_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4871 | `!defined(__IO_H__) && ELIF(TH58TEG8DDJ)` | `DLL_SDLL_OFFSET_READ533_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4872 | `!defined(__IO_H__) && ELIF(TH58TEG8DDJ)` | `DLL_SDLL1_OFFSET_WRITE_CH0_VALUE` | `2` | 通用常數/硬體欄位定義 |
| 4873 | `!defined(__IO_H__) && ELIF(TH58TEG8DDJ)` | `DLL_SDLL1_OFFSET_WRITE_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4874 | `!defined(__IO_H__) && ELIF(TH58TEG8DDJ)` | `DLL_SDLL1_OFFSET_WRITE533_CH0_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4875 | `!defined(__IO_H__) && ELIF(TH58TEG8DDJ)` | `DLL_SDLL1_OFFSET_WRITE533_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4877 | `!defined(__IO_H__) && ELIF(TH58TFG9DFK)` | `DLL_SDLL_OFFSET_READ_CH0_VALUE` | `41` | 通用常數/硬體欄位定義 |
| 4878 | `!defined(__IO_H__) && ELIF(TH58TFG9DFK)` | `DLL_SDLL_OFFSET_READ_CH1_VALUE` | `15` | 通用常數/硬體欄位定義 |
| 4879 | `!defined(__IO_H__) && ELIF(TH58TFG9DFK)` | `DLL_SDLL_OFFSET_READ533_CH0_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4880 | `!defined(__IO_H__) && ELIF(TH58TFG9DFK)` | `DLL_SDLL_OFFSET_READ533_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4881 | `!defined(__IO_H__) && ELIF(TH58TFG9DFK)` | `DLL_SDLL1_OFFSET_WRITE_CH0_VALUE` | `507` | 通用常數/硬體欄位定義 |
| 4882 | `!defined(__IO_H__) && ELIF(TH58TFG9DFK)` | `DLL_SDLL1_OFFSET_WRITE_CH1_VALUE` | `506` | 通用常數/硬體欄位定義 |
| 4883 | `!defined(__IO_H__) && ELIF(TH58TFG9DFK)` | `DLL_SDLL1_OFFSET_WRITE533_CH0_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4884 | `!defined(__IO_H__) && ELIF(TH58TFG9DFK)` | `DLL_SDLL1_OFFSET_WRITE533_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4886 | `!defined(__IO_H__) && ELIF(TH58TFG8DDL)` | `DLL_SDLL_OFFSET_READ_CH0_VALUE` | `16` | 通用常數/硬體欄位定義 |
| 4887 | `!defined(__IO_H__) && ELIF(TH58TFG8DDL)` | `DLL_SDLL_OFFSET_READ_CH1_VALUE` | `509` | 通用常數/硬體欄位定義 |
| 4888 | `!defined(__IO_H__) && ELIF(TH58TFG8DDL)` | `DLL_SDLL_OFFSET_READ533_CH0_VALUE` | `17` | 通用常數/硬體欄位定義 |
| 4889 | `!defined(__IO_H__) && ELIF(TH58TFG8DDL)` | `DLL_SDLL_OFFSET_READ533_CH1_VALUE` | `4` | 通用常數/硬體欄位定義 |
| 4890 | `!defined(__IO_H__) && ELIF(TH58TFG8DDL)` | `DLL_SDLL1_OFFSET_WRITE_CH0_VALUE` | `511` | 通用常數/硬體欄位定義 |
| 4891 | `!defined(__IO_H__) && ELIF(TH58TFG8DDL)` | `DLL_SDLL1_OFFSET_WRITE_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4892 | `!defined(__IO_H__) && ELIF(TH58TFG8DDL)` | `DLL_SDLL1_OFFSET_WRITE533_CH0_VALUE` | `511` | 通用常數/硬體欄位定義 |
| 4893 | `!defined(__IO_H__) && ELIF(TH58TFG8DDL)` | `DLL_SDLL1_OFFSET_WRITE533_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4895 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL)` | `DLL_SDLL_OFFSET_READ_CH0_VALUE` | `11` | 通用常數/硬體欄位定義 |
| 4896 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL)` | `DLL_SDLL_OFFSET_READ_CH1_VALUE` | `508` | 通用常數/硬體欄位定義 |
| 4897 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL)` | `DLL_SDLL_OFFSET_READ533_CH0_VALUE` | `14` | 通用常數/硬體欄位定義 |
| 4898 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL)` | `DLL_SDLL_OFFSET_READ533_CH1_VALUE` | `5` | 通用常數/硬體欄位定義 |
| 4899 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL)` | `DLL_SDLL1_OFFSET_WRITE_CH0_VALUE` | `510` | 通用常數/硬體欄位定義 |
| 4900 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL)` | `DLL_SDLL1_OFFSET_WRITE_CH1_VALUE` | `1` | 通用常數/硬體欄位定義 |
| 4901 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL)` | `DLL_SDLL1_OFFSET_WRITE533_CH0_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4902 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL)` | `DLL_SDLL1_OFFSET_WRITE533_CH1_VALUE` | `1` | 通用常數/硬體欄位定義 |
| 4904 | `!defined(__IO_H__) && ELIF(TH58TFT0DDL)` | `DLL_SDLL_OFFSET_READ_CH0_VALUE` | `18` | 通用常數/硬體欄位定義 |
| 4905 | `!defined(__IO_H__) && ELIF(TH58TFT0DDL)` | `DLL_SDLL_OFFSET_READ_CH1_VALUE` | `505` | 通用常數/硬體欄位定義 |
| 4906 | `!defined(__IO_H__) && ELIF(TH58TFT0DDL)` | `DLL_SDLL_OFFSET_READ533_CH0_VALUE` | `25` | 通用常數/硬體欄位定義 |
| 4907 | `!defined(__IO_H__) && ELIF(TH58TFT0DDL)` | `DLL_SDLL_OFFSET_READ533_CH1_VALUE` | `6` | 通用常數/硬體欄位定義 |
| 4908 | `!defined(__IO_H__) && ELIF(TH58TFT0DDL)` | `DLL_SDLL1_OFFSET_WRITE_CH0_VALUE` | `510` | 通用常數/硬體欄位定義 |
| 4909 | `!defined(__IO_H__) && ELIF(TH58TFT0DDL)` | `DLL_SDLL1_OFFSET_WRITE_CH1_VALUE` | `1` | 通用常數/硬體欄位定義 |
| 4910 | `!defined(__IO_H__) && ELIF(TH58TFT0DDL)` | `DLL_SDLL1_OFFSET_WRITE533_CH0_VALUE` | `511` | 通用常數/硬體欄位定義 |
| 4911 | `!defined(__IO_H__) && ELIF(TH58TFT0DDL)` | `DLL_SDLL1_OFFSET_WRITE533_CH1_VALUE` | `1` | 通用常數/硬體欄位定義 |
| 4913 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL_X4)` | `DLL_SDLL_OFFSET_READ_CH0_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4914 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL_X4)` | `DLL_SDLL_OFFSET_READ_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4915 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL_X4)` | `DLL_SDLL_OFFSET_READ533_CH0_VALUE` | `49` | 通用常數/硬體欄位定義 |
| 4916 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL_X4)` | `DLL_SDLL_OFFSET_READ533_CH1_VALUE` | `48` | 通用常數/硬體欄位定義 |
| 4917 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL_X4)` | `DLL_SDLL1_OFFSET_WRITE_CH0_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4918 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL_X4)` | `DLL_SDLL1_OFFSET_WRITE_CH1_VALUE` | `0` | 通用常數/硬體欄位定義 |
| 4919 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL_X4)` | `DLL_SDLL1_OFFSET_WRITE533_CH0_VALUE` | `509` | 通用常數/硬體欄位定義 |
| 4920 | `!defined(__IO_H__) && ELIF(TH58TFG9DDL_X4)` | `DLL_SDLL1_OFFSET_WRITE533_CH1_VALUE` | `487` | 通用常數/硬體欄位定義 |
| 4927 | `!defined(__IO_H__)` | `RAMB_START` | `((volatile unsigned char *)0x00000000)` | 依原始註解：=====================BURNER=================；註解：=====================BURNER================= |
| 4928 | `!defined(__IO_H__)` | `RAMW_START` | `((volatile unsigned short int *)0x00000000)` | 通用常數/硬體欄位定義 |
| 4929 | `!defined(__IO_H__)` | `RAML_START` | `((volatile unsigned int *)0x00000000)` | 通用常數/硬體欄位定義 |
| 4930 | `!defined(__IO_H__)` | `RAMLL_START` | `((volatile unsigned long long *)0x00000000)` | 通用常數/硬體欄位定義 |

## 4. 閱讀與維護建議

1. 先看「條件組態」欄位，確認該巨集在哪個 build 分支生效。
2. 若數值為運算式（例如 `A+B`），請同時核對依賴巨集與 `*_LEN`。
3. 同名巨集可能在不同條件分支重複定義，這是預期行為。
4. 本文件為全量清單；若 `IO.h` 更新，建議重新產生並檢查差異。