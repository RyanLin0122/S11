# Traceability Matrix

| Requirement ID | Requirement Summary | Evidence Source | Evidence Type | Confidence | Notes |
|---|---|---|---|---|---|
| FR-030 | `main()` does early GPIO/RTT setup then enters ThreadX kernel | `src/main.c` (`main`, `tx_kernel_enter`) | Observed | High | boot entry |
| FR-031 | Three threads are created: mainTask/ftlTask/flaTask | `src/main.c` (`tx_application_define`) | Observed | High | runtime model anchor |
| FR-032 | FPU microcode/entry loaded to IRAM and variant pointers set | `src/main.c` (`InitFPUEntry`) | Observed | High | NAND-specific init |
| FR-033 | FTL init gated by `gubSSD_Init_State` and `ftlInitFlash` | `src/ftl.c` (`ftlTask`) | Observed | High | init state machine basis |
| SM-INIT-001 | init state blocks normal BQ processing until scan done | `src/ftl.c` (`while state < BYTE_INIT_SCAN_DONE`) | Observed | High | explicit loop gate |
| FR-050 | Cooperative scheduling via explicit `M_SwitchTask()` | `src/main.c`, `src/ftl.c`, `src/Flash.c` | Observed | High | no preemptive slices logic |
| FR-052 | ISR registration for SATA/DDR/RTT/parity/stack monitor | `src/main.c` (`Enable_*_Interrupt`) | Observed | High | xtos handlers |
| IC-OWN-001 | Host register ownership concentrated in main task completion path | `src/main.c` command completion block (`HB_STATUS/HB_ERROR/HB_REG_REQ`) | Observed | High | ownership evidence |
| IC-BQ-001 | `BufferQueue_t` field contract (link/payload/flags/maps) | `src/Vardef.h` (`BufferQueue_t`) | Observed | High | concrete struct layout |
| IC-BQ-002 | BQ queue topology with first/last/num counters | `src/Vardef.h` (`BufferQueueInfo_t`) | Observed | High | queue ownership structure |
| IC-FQ-001 | `FlashQueue_t` field contract (job/phase/target/buffer/retry) | `src/Vardef.h` (`FlashQueue_t`) | Observed | High | concrete struct layout |
| IC-FQ-002 | CE-level FQ dispatch depth control | `src/Flash.c` (`Mark_AssignFQToEachCE`, `ubFQDoingNumber`) | Observed | High | scheduler evidence |
| IC-BP-002 | BG work is stopped when host command arrives | `src/main.c` (`M_StopBG_Copy`, command path) | Observed | High | preemption policy |
| IC-RB-001 | reset requires stop BG/RW before re-init | `src/main.c` (`ResetCmdIn`, reset branch) | Observed | High | rollback boundary |
| FR-100 | 256-entry command decode table | `src/cmdtbl.h` (`SataCmdTable[256]`) | Observed | High | opcode dispatch map |
| FR-104 | SMART/Security/Sanitize/DCO/HPA command families implemented | `src/cmdtbl.h`; `src/smart.c`; `src/Security.c`; `src/sanitize.c`; `src/DCO.c`; `src/HPA.c` | Observed | High | feature coverage |
| SM-SAN-001 | sanitize crypto/block erase unsupported (ABRT) | `src/sanitize.c` (feature `0x0011/0x0012`) | Observed | High | code-spec discrepancy |
| SM-SAN-003 | sanitize state/value changes persisted | `src/sanitize.c` (`SaveAtaConfig`) | Observed | High | persistent state |
| SM-SEC-001 | Security state bits (EN/LOCK/FROZEN/PWEXCE) | `src/Vardef.h` (`SET_SEC_*`) | Observed | High | state encoding |
| SM-PF-001 | PFAIL PRE_LOCK -> LOCKED stop flow | `src/main.c` pfail switch case | Observed | Medium | compile-time dependent |
| SM-PF-002 | PFAIL states enum and release to init | `src/FTL.h` (`PFAIL_STATE_*`), `src/main.c` | Observed | High | explicit constants |
| HW-001 | SYS IP base addresses and domains | `hw_spec/RegSpec_SYS.md` (base address table) | Observed | High | hardware map |
| HW-002 | SATA APU register space/interrupt model | `hw_spec/PS3111_RegSpec_APU.md` section 3 | Observed | High | host controller contract |
| HW-003 | Doorbell queue register model and alignment/pointer rules | `hw_spec/RegSpec_Doorbell.md` section 3 | Observed | High | queue hardware contract |
| RSEQ-DDR-001 | DDR ECC/CRC zone protection and interrupt status/clear model | `hw_spec/PS3111_RegSpec_DDR.md`; `src/main.c` DDR interrupt setup | Observed | High | sw+hw alignment |
| HW-010 | W1C/RW1C semantics are mandatory access rules | `hw_spec/RegSpec_SYS.md`; `hw_spec/RegSpec_Doorbell.md` | Observed | High | register safety |
| DS-004 | FTL background state bitfield definition | `src/Vardef.h` (`FTL_t`) | Observed | High | GC/WL/flush state source |
| POL-001 | RTT initialization uses 1ms cadence (`limit=1000`) | `src/main.c` (`RTT_Set_Limit(1,1000)`) | Observed | High | timing parameter |
| POL-002 | autopoll timeout constant is 10000 ms | `src/Setup.h` (`AUTOPOL_TIMEOUT_MILLISEC`) | Observed | High | threshold evidence |
| POL-004 | init state constants enumerated | `src/Setup.h` (`BYTE_INIT_*`) | Observed | High | state-machine constants |
| PM-001 | persistent ATA config and metadata requirement | `src/sanitize.c`/`src/smart.c`/`src/DCO.c` (`SaveAtaConfig`) + table structures in `src/FTL.h` | Inferred | Medium | on-media exact layout unknown |
| TV-010 | reset during I/O validation required by observed stop/reinit logic | `src/main.c` reset handling blocks | Inferred | Medium | derived verification rule |
| DSC-003 | many ATA opcodes remain not implemented | `src/cmdtbl.h` (`NotImplement`) | Observed | High | compatibility scope |
