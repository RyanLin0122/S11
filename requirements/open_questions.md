# Open Questions / Ambiguities

## OQ-001: FTL On-media Metadata Layout
- 問題：L2P/P2L/GR/GC 相關 table 的 on-media layout/versioning/CRC policy 未在單一文件明確定義。
- 影響：重建 firmware 時，無法僅憑現有資料保證與原 FW 完全相容的媒體格式。
- 建議：補充 metadata schema 文件（header、entry format、superblock/recovery marker）。

## OQ-002: SKU Macro Freeze List
- 問題：`Setup.h` 宏非常多，但缺少產品 SKU 與 macro 組合對照表。
- 影響：新團隊無法確定量產配置（哪些功能為 debug-only、factory-only）。
- 建議：輸出每 SKU 的 build manifest（macro set + NAND ID list + capacity bins）。

## OQ-003: GC/WL Quantitative Policy
- 問題：可觀察到 GC/WL 觸發旗標，但門檻值、分級策略、不同負載下切換條件不完整。
- 影響：可重現功能但不易重現相同性能/耐久行為。
- 建議：補齊 policy 表（free block watermark, WL trigger, hysteresis）。

## OQ-004: Power-loss Guarantee Envelope
- 問題：有 PFAIL state machine 但缺少外部硬體 hold-up 條件與驗證規範（最短保護時間、電壓曲線）。
- 影響：無法定義資料完整性保證邊界。
- 建議：補硬體/韌體聯合規格（capacitor sizing, GPIO trigger timing, shutdown budget）。

## OQ-005: Vendor Command Productization Scope
- 問題：`vender.c` 含大量診斷/工廠/維修命令，難以區分量產對外暴露與內部限定。
- 影響：重建版本可能誤暴露危險命令或缺失必要維修能力。
- 建議：建立命令白名單（shipping / manufacturing / RMA / disabled）。

## OQ-006: hw_spec vs FW Used Subset
- 問題：硬體規格提供完整 register capability，但 FW 實際只用子集合。
- 影響：重建時若啟用不必要功能可能導致不可預期 side effects。
- 建議：建立「used-register profile」與 reset default 比對。

## OQ-007: Performance Targets
- 問題：source 可看到 timeout 與一些 flow control，但缺乏明確 KPI（IOPS、latency、QoS、WAF）。
- 影響：無法驗收新實作是否達成原產品性能。
- 建議：由 PM/系統驗證提供正式 performance requirement baseline。

## OQ-008: Firmware Update Safety Protocol
- 問題：存在 `Vender_Isp_*` 路徑，但版本相容、rollback、電源中斷恢復策略未完整敘明。
- 影響：更新失敗風險與 recovery 流程不可驗證。
- 建議：補齊 FOTA/ISP state machine 與 fail-safe 規格。
