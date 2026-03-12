# Traceability Matrix

| Requirement ID | Requirement Summary | Evidence Source | Evidence Type | Confidence | Notes |
|---|---|---|---|---|---|
| FR-030 | `main()` initializes GPIO/RTT then enters kernel | `src/main.c` (`main`, `tx_kernel_enter`) | Observed | High | Boot skeleton clear |
| FR-031 | Three ThreadX tasks created | `src/main.c` (`tx_application_define`) | Observed | High | mainTask/ftlTask/flaTask |
| FR-033 | FTL init scan gated by init state | `src/ftl.c` (`ftlTask`, `ftlInitFlash`) | Observed | High | state machine gate |
| FR-034 | SATA register init before runtime loop | `src/main.c` (`mainTask`, `InitSATARegister`) | Observed | High | host-facing readiness |
| FR-037 | soft/hard reset flow with re-init | `src/main.c` (`ResetCmdIn`) | Observed | High | includes NCQ re-enable condition |
| FR-050 | Cooperative switching via `M_SwitchTask` | `src/main.c`, `src/ftl.c`, `src/Flash.c` | Observed | High | explicit yield style |
| FR-052 | ISR registration for SATA/DDR/RTT/parity/stack | `src/main.c` (`Enable_*_Interrupt`) | Observed | High | xtos APIs |
| FR-054 | BG jobs stopped when host command arrives | `src/main.c` (`mainTask`, `M_StopBG_Copy`) | Observed | High | preemption behavior |
| FR-056 | Ownership split main/FTL/FLA | task bodies in `src/main.c`/`src/ftl.c`/`src/Flash.c` | Inferred | Medium | not formally documented |
| FR-100 | 256-entry SATA command table | `src/cmdtbl.h` (`SataCmdTable[256]`) | Observed | High | direct decode table |
| FR-101 | Read/write/flush/FUA commands implemented | `src/cmdtbl.h`, `src/Satacmd.c` | Observed | High | ATA opcodes mapped |
| FR-103 | TRIM supported | `src/cmdtbl.h` opcode `0x06 -> Trim` | Observed | High | handler outside file |
| FR-104 | SMART/Security/Sanitize/DCO/HPA support | `src/cmdtbl.h`; `src/smart.c`; `src/Security.c`; `src/sanitize.c`; `src/DCO.c`; `src/HPA.c` | Observed | High | feature families present |
| FR-105 | Command completion by HB status/error + REG_REQ | `src/main.c` command completion block | Observed | High | host protocol semantics |
| FR-110 | L2P/P2L/GC-related data structures exist | `src/FTL.h` extern tables | Observed | High | core mapping artifacts |
| FR-120 | GC/WL driven by FTL state bits | `src/ftl.c` checks `btNeedCleanGR/btNeedWearLeveling` | Observed | High | explicit condition |
| FR-130 | Bad block statistics in SMART/vendor flows | `src/smart.c`, `src/vender.c` | Observed | Medium | multiple data paths |
| FR-140 | DDR ECC/CRC interrupt path | `src/main.c` (`Enable_DDR_Interrupt`), `hw_spec/PS3111_RegSpec_DDR.md` | Observed | High | sw+hw match |
| FR-150 | Buffer DMA/PIO path via RWBuffer | `src/Satacmd.c` (`RWBuffer`, ATA cmd handlers) | Observed | High | command data transfer |
| FR-160 | Power-fail state machine hook | `src/main.c` (`gubPfail_state` handling) | Observed | Medium | compile-time dependent |
| FR-170 | Firmware update/recovery vendor path | `src/vender.c` (`Vender_Isp_*`, `ISP_JUMP`) | Observed | High | maintenance channel |
| NFR-001 | ATA timeout monitored/logged | `src/main.c` (`guwATATimeoutThreshold`, `ftlRecordFailLog`) | Observed | High | latency watchdog |
| NFR-004 | DDR reliability relies on zone ECC/CRC | `hw_spec/PS3111_RegSpec_DDR.md` (zone format/error interrupt) | Observed | High | HW contract |
| HW-001 | SYS block address map | `hw_spec/RegSpec_SYS.md` table of base addresses | Observed | High | foundational map |
| HW-002 | APU SATA register space & interrupts | `hw_spec/PS3111_RegSpec_APU.md` section 3 | Observed | High | host interface register contract |
| HW-003 | Doorbell queue architecture | `hw_spec/RegSpec_Doorbell.md` section 3 | Observed | High | queue control/ptr rules |
| HW-004 | SS DMAC descriptor/queue behavior | `hw_spec/PS3111_RegSpec_SS_DMAC.md` | Observed | Medium | details large, subset used |
| HW-005 | FLH block provides flash/ecc/ldpc controls | `hw_spec/PS3111_RegSpec_FLH.md` | Observed | Medium | FW usage subset |
| HW-010 | W1C/RW1C semantics required | `hw_spec/RegSpec_SYS.md`; `hw_spec/RegSpec_Doorbell.md` | Observed | High | register-access safety |
| HW-020 | HB register request handshake | `src/main.c` + APU register model | Observed | High | repeated polling pattern |
| ALG-001 | Function-pointer command dispatch | `src/cmdtbl.h`; `src/main.c` (`SataCmdTable[ubCmdCode]()`) | Observed | High | dispatch core |
| ALG-002 | BQ -> FQ pipeline | `src/ftl.c`; `src/Flash.c` | Observed | High | data path decomposition |
| ALG-003 | CE-level FQ scheduling | `src/Flash.c` (`Mark_AssignFQToEachCE`) | Observed | Medium | complex logic |
| ERR-001 | Multi-domain error categories present | `src/main.c`; `src/Satacmd.c`; `src/smart.c` | Inferred | Medium | inferred taxonomy |
| CFG-001 | Extensive compile-time feature flags | `src/Setup.h` | Observed | High | SKU adaptability |
| CFG-002 | NAND-vendor-specific algorithm switching | `src/Setup.h`; `src/main.c` FPU init; retry tables in `src/FTL.h` | Observed | High | strong conditionality |
| DSC-001 | Sanitize crypto/block erase unsupported in FW | `src/sanitize.c` (`0x0011/0x0012 => ABRT`) | Observed | High | explicit mismatch risk |
