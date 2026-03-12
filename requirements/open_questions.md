# Open Questions / Ambiguities (Post-Revision)

## OQ-001: On-media metadata binary format (critical)
- 已補最低持久化契約（L2P/P2L/bad block/RUT/VT/ATA cfg），但仍缺原廠精確 binary 格式：
  - superblock/header layout
  - version migration rule
  - CRC/checksum polynomial
  - checkpoint/journal record encoding
  - clean/dirty close marker 位元位置
- 影響：可重建功能，但無法保證 media binary compatibility。

## OQ-002: GC/WL quantitative thresholds
- 已從 code 補到部分 policy（前景優先、BG 可中斷），但仍缺核心數值：
  - free block watermark/hysteresis
  - WL trigger/budget
  - critical GC threshold
- 影響：效能/耐久曲線可能偏離原 FW。

## OQ-003: Timeout budgets still unresolved
- 已確認常數：ATA/NCQ timeout 8000ms、autopol 10000ms、temp NACK 2000ms。
- 仍缺：
  - `HB_REG_REQ` polling timeout 上限
  - queue drain timeout 上限
  - reset retry budget
  - sanitize whole-operation timeout 上限

## OQ-004: SKU macro freeze manifest
- `Setup.h` 宏極多，仍缺產品 SKU 對照：
  - shipping / factory / RMA / debug 宏組合
  - NAND ID -> macro profile 映射

## OQ-005: Hardware used-register side-effect profile
- 已補 critical register sequence，但仍缺：
  - 全量「實際使用 register 清單」
  - reset default 假設對照
  - 各 register side-effect/race 風險清單

## OQ-006: Performance acceptance criteria
- 驗證矩陣已補，但仍缺正式 KPI：
  - IOPS/latency/QoS
  - SMART 指標驗收上下限
  - WAF/endurance acceptance

## OQ-007: Firmware update rollback policy
- 已找到 DLMC header/version/checksum 驗證與錯誤碼中止流程。
- 仍缺：
  - dual-image / rollback 正式策略
  - update 中斷（reset/pfail）後保證點定義
