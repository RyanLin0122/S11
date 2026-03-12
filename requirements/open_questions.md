# Open Questions / Ambiguities (Post-Revision)

## OQ-001: On-media metadata binary format (critical)
- 已補「最低持久化契約」(PM-001~PM-005)，但仍缺原廠精確格式：
  - superblock/header binary layout
  - version migration rule
  - checksum/CRC polynomial
  - checkpoint/journal record encoding
  - clean/dirty close marker 實際位元位置
- 影響：可重建功能行為，但無法保證 media binary compatibility。

## OQ-002: GC/WL quantitative thresholds
- 已補觸發流程與 suspend/resume 規則，但仍缺數值：
  - free block watermark/hysteresis
  - WL trigger/budget
  - critical GC threshold
- 影響：效能/耐久曲線可能偏離原 FW。

## OQ-003: Timeout budget matrix
- 已補 timeout handling 原則與已觀察常數（RTT 1ms、autopol 10s、PFAIL test 250 tick），仍缺：
  - HB_REG_REQ polling timeout
  - queue drain timeout
  - reset retry budget
  - sanitize operation timeout policy

## OQ-004: SKU macro freeze manifest
- `Setup.h` 宏極多，仍缺產品 SKU 對照：
  - 哪些宏為 shipping
  - 哪些為 factory/RMA/debug
  - NAND ID -> macro profile 映射

## OQ-005: Vendor command exposure policy
- 仍需確認 `vender.c` 子命令白名單：
  - shipping enabled
  - manufacturing only
  - disabled in production

## OQ-006: Hardware used-register profile
- 已補 driver sequence，但仍需確認「實際啟用 register subset」：
  - APU/Doorbell/DMAC/FLH/DDR 使用清單
  - reset default 假設
  - 各 register side-effect 風險

## OQ-007: Performance acceptance criteria
- 驗證章節已補 test matrix 類別，但仍缺正式 KPI：
  - IOPS/latency/QoS
  - SMART 指標驗收上下限
  - WAF/endurance acceptance

## OQ-008: Firmware update fail-safe protocol
- 已確認存在 ISP path，但仍缺正式更新契約：
  - image compatibility check
  - rollback/dual-image policy
  - update interrupted by reset/pfail 的可恢復規則
