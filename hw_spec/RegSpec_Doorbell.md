Topic: Doorbell Register Space

Version: 0.1

Date: MARCH 17th, 2014

By: JP Wang

Revision History

+---------------------+------------+------------+------------------------------------------------------------+
| **Revision Number** | **Date**   | **Author** | **Description**                                            |
+---------------------+------------+------------+------------------------------------------------------------+
| 0.1                 | 2014/03/17 | JP Wang    | 1\. First version.                                         |
+---------------------+------------+------------+------------------------------------------------------------+
| 0.2                 | 2014/03/31 | JP Wang    | Change address offset of WR_CNT/RPIU                       |
+---------------------+------------+------------+------------------------------------------------------------+
| 0.3                 | 2014/07/16 | JP Wang    | Change bit-width of WPIU, WR_CNT,RPIU,RD_CNT,              |
|                     |            |            |                                                            |
|                     |            |            | BUF_LEN and DATA_SIZE                                      |
+---------------------+------------+------------+------------------------------------------------------------+
| 0.4                 | 2014/07/28 | JP Wang    | Support DATA_SIZE 1/2/4/8/16/32/64/128B                    |
+---------------------+------------+------------+------------------------------------------------------------+
| 0.5                 | 2014/08/11 | JP Wang    | Add RS_CQ and SATA_CQ                                      |
+---------------------+------------+------------+------------------------------------------------------------+
| 0.6                 | 2014/08/20 | JP Wang    | Only support one SATA_CQ                                   |
+---------------------+------------+------------+------------------------------------------------------------+
| 0.7                 | 2014/08/25 | JP Wang    | 1\. Change EMPTY_MASK control                              |
|                     |            |            |                                                            |
|                     |            |            | 2\. Add accelerator at doorbell memory space               |
+---------------------+------------+------------+------------------------------------------------------------+
| 0.8                 | 2014/09/11 | JP Wang    | While WPIU/RPIU is written, WR_CNT/RD_CNT will be updated. |
+---------------------+------------+------------+------------------------------------------------------------+
| 0.9                 | 2014/09/12 | JenYu Wang | Add Appendix: VQ control flow                              |
+---------------------+------------+------------+------------------------------------------------------------+
| 1.0                 | 2014/09/18 | JP Wang    | Add stack overflow monitor                                 |
+---------------------+------------+------------+------------------------------------------------------------+
| 1.1                 | 2014/10/28 | JP Wang    | Add inbound request priority control                       |
+---------------------+------------+------------+------------------------------------------------------------+
|                     |            |            |                                                            |
+---------------------+------------+------------+------------------------------------------------------------+
|                     |            |            |                                                            |
+---------------------+------------+------------+------------------------------------------------------------+
|                     |            |            |                                                            |
+---------------------+------------+------------+------------------------------------------------------------+
|                     |            |            |                                                            |
+---------------------+------------+------------+------------------------------------------------------------+

INDEX

[1 Introduction 4](#introduction)

[2 Register Type 5](#register-type)

[3 doorbell Register Description 6](#doorbell-register-description)

[3.1 Circular Queue 0 (DMAC Submission Queue) 6](#circular-queue-0-dmac-submission-queue)

[3.1.1 Queue Base Address Register (Offset 03h\~00h) 6](#queue-base-address-register-offset-03h00h)

[3.1.2 Write Pointer Register (Offset 07h\~04h) 6](#write-pointer-register-offset-07h04h)

[3.1.3 Read Pointer Register (Offset 0Bh\~08h) 6](#read-pointer-register-offset-0bh08h)

[3.1.4 Write Pointer Increase Unit Register (Offset 0Dh\~0Ch) 6](#write-pointer-increase-unit-register-offset-0dh0ch)

[3.1.5 Writable Count Register (Offset 0Fh\~0Eh) 7](#writable-count-register-offset-0fh0eh)

[3.1.6 Read Pointer Increase Unit Register (Offset 11h\~10h) 7](#read-pointer-increase-unit-register-offset-11h10h)

[3.1.7 Readable Count Register (Offset 13h\~12h) 7](#readable-count-register-offset-13h12h)

[3.1.8 Buffer Length Register (Offset 15h \~14h) 7](#buffer-length-register-offset-15h-14h)

[3.1.9 Data Size Register (Offset 16h) 8](#data-size-register-offset-16h)

[3.1.10 Control Register (Offset 17h) 8](#control-register-offset-17h)

[3.2 Circular Queue 1 (DMAC Completion Queue) 9](#circular-queue-1-dmac-completion-queue)

[3.2.1 Queue Base Address Register (Offset 23h\~20h) 9](#queue-base-address-register-offset-23h20h)

[3.2.2 Write Pointer Register (Offset 27h\~24h) 9](#write-pointer-register-offset-27h24h)

[3.2.3 Read Pointer Register (Offset 2Bh\~28h) 9](#read-pointer-register-offset-2bh28h)

[3.2.4 Write Pointer Increase Unit Register (Offset 2Dh\~2Ch) 9](#write-pointer-increase-unit-register-offset-2dh2ch)

[3.2.5 Writable Count Register (Offset 2Fh\~2Eh) 9](#writable-count-register-offset-2fh2eh)

[3.2.6 Read Pointer Increase Unit Register (Offset 31h\~30h) 10](#read-pointer-increase-unit-register-offset-31h30h)

[3.2.7 Readable Count Register (Offset 33h\~32h) 10](#readable-count-register-offset-33h32h)

[3.2.8 Buffer Length Register (Offset 35h \~34h) 10](#buffer-length-register-offset-35h-34h)

[3.2.9 Data Size Register (Offset 36h) 11](#data-size-register-offset-36h)

[3.2.10 Control Register (Offset 37h) 11](#control-register-offset-37h)

[3.3 Circular Queue 2 (SS Submission Queue) 12](#circular-queue-2-ss-submission-queue)

[3.3.1 Queue Base Address Register (Offset 43h\~40h) 12](#queue-base-address-register-offset-43h40h)

[3.3.2 Write Pointer Register (Offset 47h\~44h) 12](#write-pointer-register-offset-47h44h)

[3.3.3 Read Pointer Register (Offset 4Bh\~48h) 12](#read-pointer-register-offset-4bh48h)

[3.3.4 Write Pointer Increase Unit Register (Offset 4Dh\~4Ch) 12](#write-pointer-increase-unit-register-offset-4dh4ch)

[3.3.5 Writable Count Register (Offset 4Fh\~4Eh) 12](#writable-count-register-offset-4fh4eh)

[3.3.6 Read Pointer Increase Unit Register (Offset 51h\~50h) 13](#read-pointer-increase-unit-register-offset-51h50h)

[3.3.7 Readable Count Register (Offset 53h\~52h) 13](#readable-count-register-offset-53h52h)

[3.3.8 Buffer Length Register (Offset 55h \~54h) 13](#buffer-length-register-offset-55h-54h)

[3.3.9 Data Size Register (Offset 56h) 14](#data-size-register-offset-56h)

[3.3.10 Control Register (Offset 57h) 14](#control-register-offset-57h)

[3.4 Circular Queue 3 (SS Completion Queue) 15](#circular-queue-3-ss-completion-queue)

[3.4.1 Queue Base Address Register (Offset 63h\~60h) 15](#queue-base-address-register-offset-63h60h)

[3.4.2 Write Pointer Register (Offset 67h\~64h) 15](#write-pointer-register-offset-67h64h)

[3.4.3 Read Pointer Register (Offset 6Bh\~68h) 15](#read-pointer-register-offset-6bh68h)

[3.4.4 Write Pointer Increase Unit Register (Offset 6Dh\~6Ch) 15](#write-pointer-increase-unit-register-offset-6dh6ch)

[3.4.5 Writable Count Register (Offset 6Fh\~6Eh) 16](#writable-count-register-offset-6fh6eh)

[3.4.6 Read Pointer Increase Unit Register (Offset 71h\~70h) 16](#read-pointer-increase-unit-register-offset-71h70h)

[3.4.7 Readable Count Register (Offset 73h\~72h) 16](#readable-count-register-offset-73h72h)

[3.4.8 Buffer Length Register (Offset 75h \~74h) 16](#buffer-length-register-offset-75h-74h)

[3.4.9 Data Size Register (Offset 76h) 17](#data-size-register-offset-76h)

[3.4.10 Control Register (Offset 77h) 17](#control-register-offset-77h)

[3.5 Circular Queue 4 (FLH MSG Completion Queue) 18](#circular-queue-4-flh-msg-completion-queue)

[3.5.1 Queue Base Address Register (Offset 83h\~80h) 18](#queue-base-address-register-offset-83h80h)

[3.5.2 Write Pointer Register (Offset 87h\~84h) 18](#write-pointer-register-offset-87h84h)

[3.5.3 Read Pointer Register (Offset 8Bh\~88h) 18](#read-pointer-register-offset-8bh88h)

[3.5.4 Write Pointer Increase Unit Register (Offset 8Dh\~8Ch) 18](#write-pointer-increase-unit-register-offset-8dh8ch)

[3.5.5 Writable Count Register (Offset 8Fh\~8Eh) 19](#writable-count-register-offset-8fh8eh)

[3.5.6 Read Pointer Increase Unit Register (Offset 91h\~90h) 19](#read-pointer-increase-unit-register-offset-91h90h)

[3.5.7 Readable Count Register (Offset 93h\~92h) 19](#readable-count-register-offset-93h92h)

[3.5.8 Buffer Length Register (Offset 95h \~94h) 19](#buffer-length-register-offset-95h-94h)

[3.5.9 Data Size Register (Offset 96h) 20](#data-size-register-offset-96h)

[3.5.10 Control Register (Offset 97h) 20](#control-register-offset-97h)

[3.6 Circular Queue 5 (RS MSG Completion Queue) 21](#circular-queue-5-rs-msg-completion-queue)

[3.6.1 Queue Base Address Register (Offset A3h\~A0h) 21](#queue-base-address-register-offset-a3ha0h)

[3.6.2 Write Pointer Register (Offset A7h\~A4h) 21](#write-pointer-register-offset-a7ha4h)

[3.6.3 Read Pointer Register (Offset ABh\~A8h) 21](#read-pointer-register-offset-abha8h)

[3.6.4 Write Pointer Increase Unit Register (Offset ADh\~ACh) 21](#write-pointer-increase-unit-register-offset-adhach)

[3.6.5 Writable Count Register (Offset AFh\~AEh) 22](#writable-count-register-offset-afhaeh)

[3.6.6 Read Pointer Increase Unit Register (Offset B1h\~B0h) 22](#read-pointer-increase-unit-register-offset-b1hb0h)

[3.6.7 Readable Count Register (Offset B3h\~B2h) 22](#readable-count-register-offset-b3hb2h)

[3.6.8 Buffer Length Register (Offset B5h \~B4h) 22](#buffer-length-register-offset-b5h-b4h)

[3.6.9 Data Size Register (Offset B6h) 23](#data-size-register-offset-b6h)

[3.6.10 Control Register (Offset B7h) 23](#control-register-offset-b7h)

[3.7 Circular Queue 6 (SATA MSG Completion Queue) 24](#circular-queue-6-sata-msg-completion-queue)

[3.7.1 Queue Base Address Register (Offset C3h\~C0h) 24](#queue-base-address-register-offset-c3hc0h)

[3.7.2 Write Pointer Register (Offset C7h\~C4h) 24](#write-pointer-register-offset-c7hc4h)

[3.7.3 Read Pointer Register (Offset CBh\~C8h) 24](#read-pointer-register-offset-cbhc8h)

[3.7.4 Write Pointer Increase Unit Register (Offset CDh\~CCh) 24](#write-pointer-increase-unit-register-offset-cdhcch)

[3.7.5 Writable Count Register (Offset CFh\~CEh) 25](#writable-count-register-offset-cfhceh)

[3.7.6 Read Pointer Increase Unit Register (Offset D1h\~D0h) 25](#read-pointer-increase-unit-register-offset-d1hd0h)

[3.7.7 Readable Count Register (Offset D3h\~D2h) 25](#readable-count-register-offset-d3hd2h)

[3.7.8 Buffer Length Register (Offset D5h \~D4h) 25](#buffer-length-register-offset-d5h-d4h)

[3.7.9 Data Size Register (Offset D6h) 26](#data-size-register-offset-d6h)

[3.7.10 Control Register (Offset D7h) 26](#control-register-offset-d7h)

[3.8 Circular Queue N (7h \<= N \<= Fh) 27](#circular-queue-n-7h-n-fh)

[3.8.1 Queue Base Address Register (Offset 03h\~00h + (N x 20h)) 27](#queue-base-address-register-offset-03h00h-n-x-20h)

[3.8.2 Write Pointer Register (Offset 07h\~04h + (N x 20h)) 27](#write-pointer-register-offset-07h04h-n-x-20h)

[3.8.3 Read Pointer Register (Offset 0Bh\~08h + (N x 20h)) 27](#read-pointer-register-offset-0bh08h-n-x-20h)

[3.8.4 Write Pointer Increase Unit Register (Offset 0Dh\~0Ch + (N x 20h)) 27](#write-pointer-increase-unit-register-offset-0dh0ch-n-x-20h)

[3.8.5 Writable Count Register (Offset 0Fh\~0Eh + (N x 20h)) 28](#writable-count-register-offset-0fh0eh-n-x-20h)

[3.8.6 Read Pointer Increase Unit Register (Offset 11h\~10h + (N x 20h)) 28](#read-pointer-increase-unit-register-offset-11h10h-n-x-20h)

[3.8.7 Readable Count Register (Offset 13h\~12h + (N x 20h)) 28](#readable-count-register-offset-13h12h-n-x-20h)

[3.8.8 Buffer Length Register (Offset 15h \~14h + (N x 20h)) 28](#buffer-length-register-offset-15h-14h-n-x-20h)

[3.8.9 Data Size Register (Offset 16h + (N x 20h)) 29](#data-size-register-offset-16h-n-x-20h)

[3.8.10 Control Register (Offset 17h + (N x 20h)) 29](#control-register-offset-17h-n-x-20h)

[3.9 Empty Mask Control 30](#empty-mask-control)

[3.9.1 Empty Mask Control Register (Offset 200h) 30](#empty-mask-control-register-offset-200h)

[3.10 Acc Control 31](#acc-control)

[3.10.1 L2P Register (Offset 213h \~ 210h) 31](#l2p-register-offset-213h-210h)

[3.10.2 PCA Register (Offset 217h \~ 214h) 31](#pca-register-offset-217h-214h)

[3.10.3 Offset Register (Offset 218h) 31](#offset-register-offset-218h)

[3.10.4 Length Register (Offset 219h) 31](#length-register-offset-219h)

[3.10.5 Mode Register (Offset 21Ah) 31](#mode-register-offset-21ah)

[3.10.6 Mask Register (Offset 21Bh) 32](#mask-register-offset-21bh)

[3.10.7 Zipmap Register (Offset 21Fh \~ 21Ch) 32](#zipmap-register-offset-21fh-21ch)

[3.11 Stack Overflow Monitor 33](#stack-overflow-monitor)

[3.11.1 MON_ADDR0 Register (Offset 223h \~ 220h) 33](#mon_addr0-register-offset-223h-220h)

[3.11.2 MON_ADDR1 Register (Offset 227h \~ 224h) 33](#mon_addr1-register-offset-227h-224h)

[3.11.3 MON_ADDR2 Register (Offset 22Bh \~ 228h) 33](#mon_addr2-register-offset-22bh-228h)

[3.11.4 MON_ADDR3 Register (Offset 22Fh \~ 22Ch) 33](#mon_addr3-register-offset-22fh-22ch)

[3.11.5 MON_EN Register (Offset 230h) 33](#mon_en-register-offset-230h)

[3.12 Inbound Request Priority Control (Offset 240h) 34](#inbound-request-priority-control-offset-240h)

[4 Appendix 35](#appendix)

[4.1 VQ CONTROL FLOW 35](#vq-control-flow)

# Introduction

This module is used to control submission queue, completion queue and queue empty mask control.

# Register Type

  --------------- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  **Attribute**   **Description**

  HwInit          Read-only register: Register bits are read-only and cannot be altered by software. Register bits may be initialized by hardware mechanisms such as pin strapping or serial EEPROM.

  RO              Read-only register: Register bits are read-only and cannot be altered by software. Register bits may be initialized by hardware mechanisms such as pin strapping or serial EEPROM.

  RW              Read-Write register: Register bits are read-write and may be either set or cleared by software to the desired state.

  RW1C            Read-only status, Write-1-to-clear status register: Register bits indicate status when read, a set bit indicating a status event may be cleared by writing a 1. Writing a 0 to RW1C bits has no effect.

  RWL             Read-Write or Locked register: Register bits are read-write and may be either set or cleared by software to the desired state when its Lock bit is cleared. A register bit becomes read only after its Lock bit is set.

  RW1CL           Write-1-to-clear status register or Locked register: Register bits are read-write and may be either set or cleared by software to the desired state when its Lock bit is cleared. While Lock bit is set, register bits indicate status when read, a set bit indicating a status event may be cleared by writing a 1. Writing a 0 to RW1CL bits has no effect.
  --------------- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

# doorbell Register Description

## Circular Queue 0 (DMAC Submission Queue) {#circular-queue-0-dmac-submission-queue .樣式-標題-2}

## Queue Base Address Register (Offset 03h\~00h) {#queue-base-address-register-offset-03h00h .樣式-標題-2}

Abbreviation: **QBASE_ADR**

Register Index: 03h\~00h

Default Value: 00h

Attribute: RW

+:-----+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                | Default | Attributes |
+------+-----------------------------------------------------------------------------+---------+------------+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

## Write Pointer Register (Offset 07h\~04h) {#write-pointer-register-offset-07h04h .樣式-標題-2}

Abbreviation: **WPTR**

Register Index: 07h\~04h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Write pointer of circular queue.                 00h       RO
  ------ ------------------------------------------------ --------- ------------

## Read Pointer Register (Offset 0Bh\~08h) {#read-pointer-register-offset-0bh08h .樣式-標題-2}

Abbreviation: **RPTR**

Register Index: 0Bh\~08h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Read pointer of circular queue.                  00h       RO
  ------ ------------------------------------------------ --------- ------------

## Write Pointer Increase Unit Register (Offset 0Dh\~0Ch) {#write-pointer-increase-unit-register-offset-0dh0ch .樣式-標題-2}

Abbreviation: **WPIU**

Register Index: 0Dh\~0Ch

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **WPIU: (**FW triggers this field to maintain WPTR**)**                                        | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 2\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 3\. Only When low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Writable Count Register (Offset 0Fh\~0Eh) {#writable-count-register-offset-0fh0eh .樣式-標題-2}

Abbreviation: **WR_CNT**

Register Index: 0Fh \~ 0Eh

Default Value: 00h

Attribute: RO

+:------+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                | Default | Attributes |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                     | 00h     | RO         |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 11:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|       |                                                                             |         |            |
|       | Indicate that how many amount of data can be written to the circular queue. |         |            |
+-------+-----------------------------------------------------------------------------+---------+------------+

## Read Pointer Increase Unit Register (Offset 11h\~10h) {#read-pointer-increase-unit-register-offset-11h10h .樣式-標題-2}

Abbreviation: **RPIU**

Register Index: 11h\~10h

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **RPIU: (**DMAC triggers this field to maintain RPTR**)**                                      | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Readable Count Register (Offset 13h\~12h) {#readable-count-register-offset-13h12h .樣式-標題-2}

Abbreviation: **RD_CNT**

Register Index: 13h\~12h

Default Value: 00h

Attribute: RO

+:------+:---------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                               | Default | Attributes |
+-------+----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                    | 00h     | RO         |
+-------+----------------------------------------------------------------------------+---------+------------+
| 11:0  | **RD_CNT:**                                                                | 00h     | RO         |
|       |                                                                            |         |            |
|       | Indicate that how many amount of data can be read from the circular queue. |         |            |
+-------+----------------------------------------------------------------------------+---------+------------+

## Buffer Length Register (Offset 15h \~14h) {#buffer-length-register-offset-15h-14h .樣式-標題-2}

Abbreviation: **BUF_LEN**

Register Index: 15h \~14h

Default Value: 00h

Attribute: RW

+:------+:-------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 15:13 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 12:0  | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (1Byte) , 2 \<= BUF_LEN \<= 4096                             |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (2Byte) , 2 \<= BUF_LEN \<= 2048                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (4Byte) , 2 \<= BUF_LEN \<= 1024                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | f\. DATA_SIZE = 5 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | g\. DATA SIZE = 6 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | h\. DATA SIZE = 7 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

## Data Size Register (Offset 16h) {#data-size-register-offset-16h .樣式-標題-2}

Abbreviation: **DATA_SIZE**

Register Index: 16h

Default Value: 00h

Attribute: RW

+:----+:-------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                       | Default | Attributes |
+-----+--------------------------------------------------------------------+---------+------------+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

## Control Register (Offset 17h) {#control-register-offset-17h .樣式-標題-2}

Abbreviation: **CTRL**

Register Index: 17h

Default Value: 08h

Attribute: RW

+:----+:-----------------------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue 1 (DMAC Completion Queue) {#circular-queue-1-dmac-completion-queue .樣式-標題-2}

## Queue Base Address Register (Offset 23h\~20h) {#queue-base-address-register-offset-23h20h .樣式-標題-2}

Abbreviation: **QBASE_ADR**

Register Index: 23h\~20h

Default Value: 00h

Attribute: RW

+:-----+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                | Default | Attributes |
+------+-----------------------------------------------------------------------------+---------+------------+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

## Write Pointer Register (Offset 27h\~24h) {#write-pointer-register-offset-27h24h .樣式-標題-2}

Abbreviation: **WPTR**

Register Index: 27h\~24h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Write pointer of circular queue.                 00h       RO
  ------ ------------------------------------------------ --------- ------------

## Read Pointer Register (Offset 2Bh\~28h) {#read-pointer-register-offset-2bh28h .樣式-標題-2}

Abbreviation: **RPTR**

Register Index: 2Bh\~28h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Read pointer of circular queue.                  00h       RO
  ------ ------------------------------------------------ --------- ------------

## Write Pointer Increase Unit Register (Offset 2Dh\~2Ch) {#write-pointer-increase-unit-register-offset-2dh2ch .樣式-標題-2}

Abbreviation: **WPIU**

Register Index: 2D\~2Ch

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **WPIU: (**DMAC triggers this field to maintain WPTR**)**                                      | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Writable Count Register (Offset 2Fh\~2Eh) {#writable-count-register-offset-2fh2eh .樣式-標題-2}

Abbreviation: **WR_CNT**

Register Index: 2Fh \~ 2Eh

Default Value: 00h

Attribute: RO

+:------+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                | Default | Attributes |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                     | 00h     | RO         |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 11:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|       |                                                                             |         |            |
|       | Indicate that how many amount of data can be written to the circular queue. |         |            |
+-------+-----------------------------------------------------------------------------+---------+------------+

## Read Pointer Increase Unit Register (Offset 31h\~30h) {#read-pointer-increase-unit-register-offset-31h30h .樣式-標題-2}

Abbreviation: **RPIU**

Register Index: 31h\~30h

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **RPIU: (**FW triggers this field to maintain RPTR**)**                                        | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Readable Count Register (Offset 33h\~32h) {#readable-count-register-offset-33h32h .樣式-標題-2}

Abbreviation: **RD_CNT**

Register Index: 33h\~32h

Default Value: 00h

Attribute: RO

+:------+:---------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                               | Default | Attributes |
+-------+----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                    | 00h     | RO         |
+-------+----------------------------------------------------------------------------+---------+------------+
| 11:0  | **RD_CNT:**                                                                | 00h     | RO         |
|       |                                                                            |         |            |
|       | Indicate that how many amount of data can be read from the circular queue. |         |            |
+-------+----------------------------------------------------------------------------+---------+------------+

## Buffer Length Register (Offset 35h \~34h) {#buffer-length-register-offset-35h-34h .樣式-標題-2}

Abbreviation: **BUF_LEN**

Register Index: 35h \~34h

Default Value: 00h

Attribute: RW

+:------+:-------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 15:13 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 12:0  | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (1Byte) , 2 \<= BUF_LEN \<= 4096                             |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (2Byte) , 2 \<= BUF_LEN \<= 2048                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (4Byte) , 2 \<= BUF_LEN \<= 1024                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | f\. DATA_SIZE = 5 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | g\. DATA SIZE = 6 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | h\. DATA SIZE = 7 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

## Data Size Register (Offset 36h) {#data-size-register-offset-36h .樣式-標題-2}

Abbreviation: **DATA_SIZE**

Register Index: 36h

Default Value: 00h

Attribute: RW

+:----+:-------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                       | Default | Attributes |
+-----+--------------------------------------------------------------------+---------+------------+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

## Control Register (Offset 37h) {#control-register-offset-37h .樣式-標題-2}

Abbreviation: **CTRL**

Register Index: 37h

Default Value: 08h

Attribute: RW

+:----+:-----------------------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue 2 (SS Submission Queue) {#circular-queue-2-ss-submission-queue .樣式-標題-2}

## Queue Base Address Register (Offset 43h\~40h) {#queue-base-address-register-offset-43h40h .樣式-標題-2}

Abbreviation: **QBASE_ADR**

Register Index: 43h\~40h

Default Value: 00h

Attribute: RW

+:-----+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                | Default | Attributes |
+------+-----------------------------------------------------------------------------+---------+------------+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

## Write Pointer Register (Offset 47h\~44h) {#write-pointer-register-offset-47h44h .樣式-標題-2}

Abbreviation: **WPTR**

Register Index: 47h\~44h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Write pointer of circular queue.                 00h       RO
  ------ ------------------------------------------------ --------- ------------

## Read Pointer Register (Offset 4Bh\~48h) {#read-pointer-register-offset-4bh48h .樣式-標題-2}

Abbreviation: **RPTR**

Register Index: 4Bh\~48h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Read pointer of circular queue.                  00h       RO
  ------ ------------------------------------------------ --------- ------------

## Write Pointer Increase Unit Register (Offset 4Dh\~4Ch) {#write-pointer-increase-unit-register-offset-4dh4ch .樣式-標題-2}

Abbreviation: **WPIU**

Register Index: 4Dh\~4Ch

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **WPIU: (**FW triggers this field to maintain WPTR**)**                                        | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Writable Count Register (Offset 4Fh\~4Eh) {#writable-count-register-offset-4fh4eh .樣式-標題-2}

Abbreviation: **WR_CNT**

Register Index: 4Fh \~ 4Eh

Default Value: 00h

Attribute: RO

+:------+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                | Default | Attributes |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                     | 00h     | RO         |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 11:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|       |                                                                             |         |            |
|       | Indicate that how many amount of data can be written to the circular queue. |         |            |
+-------+-----------------------------------------------------------------------------+---------+------------+

## Read Pointer Increase Unit Register (Offset 51h\~50h) {#read-pointer-increase-unit-register-offset-51h50h .樣式-標題-2}

Abbreviation: **RPIU**

Register Index: 51h\~50h

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **RPIU: (**SS triggers this field to maintain RPTR**)**                                        | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Readable Count Register (Offset 53h\~52h) {#readable-count-register-offset-53h52h .樣式-標題-2}

Abbreviation: **RD_CNT**

Register Index: 53h\~52h

Default Value: 00h

Attribute: RO

+:------+:---------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                               | Default | Attributes |
+-------+----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                    | 00h     | RO         |
+-------+----------------------------------------------------------------------------+---------+------------+
| 11:0  | **RD_CNT:**                                                                | 00h     | RO         |
|       |                                                                            |         |            |
|       | Indicate that how many amount of data can be read from the circular queue. |         |            |
+-------+----------------------------------------------------------------------------+---------+------------+

## Buffer Length Register (Offset 55h \~54h) {#buffer-length-register-offset-55h-54h .樣式-標題-2}

Abbreviation: **BUF_LEN**

Register Index: 55h \~54h

Default Value: 00h

Attribute: RW

+:------+:-------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 15:10 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 9:0   | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (1Byte) , 2 \<= BUF_LEN \<= 4096                             |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (2Byte) , 2 \<= BUF_LEN \<= 2048                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (4Byte) , 2 \<= BUF_LEN \<= 1024                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | f\. DATA_SIZE = 5 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | g\. DATA SIZE = 6 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | h\. DATA SIZE = 7 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

## Data Size Register (Offset 56h) {#data-size-register-offset-56h .樣式-標題-2}

Abbreviation: **DATA_SIZE**

Register Index: 56h

Default Value: 00h

Attribute: RW

+:----+:-------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                       | Default | Attributes |
+-----+--------------------------------------------------------------------+---------+------------+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

## Control Register (Offset 57h) {#control-register-offset-57h .樣式-標題-2}

Abbreviation: **CTRL**

Register Index: 57h

Default Value: 08h

Attribute: RW

+:----+:-----------------------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue 3 (SS Completion Queue) {#circular-queue-3-ss-completion-queue .樣式-標題-2}

## Queue Base Address Register (Offset 63h\~60h) {#queue-base-address-register-offset-63h60h .樣式-標題-2}

Abbreviation: **QBASE_ADR**

Register Index: 63h\~60h

Default Value: 00h

Attribute: RW

+:-----+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                | Default | Attributes |
+------+-----------------------------------------------------------------------------+---------+------------+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

## Write Pointer Register (Offset 67h\~64h) {#write-pointer-register-offset-67h64h .樣式-標題-2}

Abbreviation: **WPTR**

Register Index: 67h\~64h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Write pointer of circular queue.                 00h       RO
  ------ ------------------------------------------------ --------- ------------

## Read Pointer Register (Offset 6Bh\~68h) {#read-pointer-register-offset-6bh68h .樣式-標題-2}

Abbreviation: **RPTR**

Register Index: 6Bh\~68h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Read pointer of circular queue.                  00h       RO
  ------ ------------------------------------------------ --------- ------------

## Write Pointer Increase Unit Register (Offset 6Dh\~6Ch) {#write-pointer-increase-unit-register-offset-6dh6ch .樣式-標題-2}

Abbreviation: **WPIU**

Register Index: 6Dh\~6Ch

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **WPIU: (**SS triggers this field to maintain WPTR**)**                                        | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Writable Count Register (Offset 6Fh\~6Eh) {#writable-count-register-offset-6fh6eh .樣式-標題-2}

Abbreviation: **WR_CNT**

Register Index: 6Fh \~ 6Eh

Default Value: 00h

Attribute: RO

+:------+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                | Default | Attributes |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                     | 00h     | RO         |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 11:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|       |                                                                             |         |            |
|       | Indicate that how many amount of data can be written to the circular queue. |         |            |
+-------+-----------------------------------------------------------------------------+---------+------------+

## Read Pointer Increase Unit Register (Offset 71h\~70h) {#read-pointer-increase-unit-register-offset-71h70h .樣式-標題-2}

Abbreviation: **RPIU**

Register Index: 71h\~70h

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **RPIU: (**FW triggers this field to maintain RPTR**)**                                        | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Readable Count Register (Offset 73h\~72h) {#readable-count-register-offset-73h72h .樣式-標題-2}

Abbreviation: **RD_CNT**

Register Index: 73h\~72h

Default Value: 00h

Attribute: RO

+:------+:---------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                               | Default | Attributes |
+-------+----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                    | 00h     | RO         |
+-------+----------------------------------------------------------------------------+---------+------------+
| 11:0  | **RD_CNT:**                                                                | 00h     | RO         |
|       |                                                                            |         |            |
|       | Indicate that how many amount of data can be read from the circular queue. |         |            |
+-------+----------------------------------------------------------------------------+---------+------------+

## Buffer Length Register (Offset 75h \~74h) {#buffer-length-register-offset-75h-74h .樣式-標題-2}

Abbreviation: **BUF_LEN**

Register Index: 75h \~74h

Default Value: 00h

Attribute: RW

+:------+:-------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 15:13 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 12:0  | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (1Byte) , 2 \<= BUF_LEN \<= 4096                             |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (2Byte) , 2 \<= BUF_LEN \<= 2048                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (4Byte) , 2 \<= BUF_LEN \<= 1024                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | f\. DATA_SIZE = 5 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | g\. DATA SIZE = 6 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | h\. DATA SIZE = 7 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

## Data Size Register (Offset 76h) {#data-size-register-offset-76h .樣式-標題-2}

Abbreviation: **DATA_SIZE**

Register Index: 76h

Default Value: 00h

Attribute: RW

+:----+:-------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                       | Default | Attributes |
+-----+--------------------------------------------------------------------+---------+------------+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

## Control Register (Offset 77h) {#control-register-offset-77h .樣式-標題-2}

Abbreviation: **CTRL**

Register Index: 77h

Default Value: 08h

Attribute: RW

+:----+:-----------------------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue 4 (FLH MSG Completion Queue) {#circular-queue-4-flh-msg-completion-queue .樣式-標題-2}

## Queue Base Address Register (Offset 83h\~80h) {#queue-base-address-register-offset-83h80h .樣式-標題-2}

Abbreviation: **QBASE_ADR**

Register Index: 83h\~80h

Default Value: 00h

Attribute: RW

+:-----+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                | Default | Attributes |
+------+-----------------------------------------------------------------------------+---------+------------+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

## Write Pointer Register (Offset 87h\~84h) {#write-pointer-register-offset-87h84h .樣式-標題-2}

Abbreviation: **WPTR**

Register Index: 87h\~84h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Write pointer of circular queue.                 00h       RO
  ------ ------------------------------------------------ --------- ------------

## Read Pointer Register (Offset 8Bh\~88h) {#read-pointer-register-offset-8bh88h .樣式-標題-2}

Abbreviation: **RPTR**

Register Index: 8Bh\~88h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Read pointer of circular queue.                  00h       RO
  ------ ------------------------------------------------ --------- ------------

## Write Pointer Increase Unit Register (Offset 8Dh\~8Ch) {#write-pointer-increase-unit-register-offset-8dh8ch .樣式-標題-2}

Abbreviation: **WPIU**

Register Index: 8Dh\~8Ch

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **WPIU: (**FLH MSG triggers this field to maintain WPTR**)**                                   | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Writable Count Register (Offset 8Fh\~8Eh) {#writable-count-register-offset-8fh8eh .樣式-標題-2}

Abbreviation: **WR_CNT**

Register Index: 8Fh \~ 8Eh

Default Value: 00h

Attribute: RO

+:------+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                | Default | Attributes |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                     | 00h     | RO         |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 11:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|       |                                                                             |         |            |
|       | Indicate that how many amount of data can be written to the circular queue. |         |            |
+-------+-----------------------------------------------------------------------------+---------+------------+

## Read Pointer Increase Unit Register (Offset 91h\~90h) {#read-pointer-increase-unit-register-offset-91h90h .樣式-標題-2}

Abbreviation: **RPIU**

Register Index: 91h\~90h

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **RPIU: (**FW triggers this field to maintain RPTR**)**                                        | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Readable Count Register (Offset 93h\~92h) {#readable-count-register-offset-93h92h .樣式-標題-2}

Abbreviation: **RD_CNT**

Register Index: 93h\~92h

Default Value: 00h

Attribute: RO

+:------+:---------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                               | Default | Attributes |
+-------+----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                    | 00h     | RO         |
+-------+----------------------------------------------------------------------------+---------+------------+
| 11:0  | **RD_CNT:**                                                                | 00h     | RO         |
|       |                                                                            |         |            |
|       | Indicate that how many amount of data can be read from the circular queue. |         |            |
+-------+----------------------------------------------------------------------------+---------+------------+

## Buffer Length Register (Offset 95h \~94h) {#buffer-length-register-offset-95h-94h .樣式-標題-2}

Abbreviation: **BUF_LEN**

Register Index: 95h \~94h

Default Value: 00h

Attribute: RW

+:------+:-------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 15:13 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 12:0  | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (1Byte) , 2 \<= BUF_LEN \<= 4096                             |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (2Byte) , 2 \<= BUF_LEN \<= 2048                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (4Byte) , 2 \<= BUF_LEN \<= 1024                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | f\. DATA_SIZE = 5 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | g\. DATA SIZE = 6 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | h\. DATA SIZE = 7 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

## Data Size Register (Offset 96h) {#data-size-register-offset-96h .樣式-標題-2}

Abbreviation: **DATA_SIZE**

Register Index: 96h

Default Value: 00h

Attribute: RW

+:----+:-------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                       | Default | Attributes |
+-----+--------------------------------------------------------------------+---------+------------+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

## Control Register (Offset 97h) {#control-register-offset-97h .樣式-標題-2}

Abbreviation: **CTRL**

Register Index: 97h

Default Value: 08h

Attribute: RW

+:----+:-----------------------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue 5 (RS MSG Completion Queue) {#circular-queue-5-rs-msg-completion-queue .樣式-標題-2}

## Queue Base Address Register (Offset A3h\~A0h) {#queue-base-address-register-offset-a3ha0h .樣式-標題-2}

Abbreviation: **QBASE_ADR**

Register Index: A3h\~A0h

Default Value: 00h

Attribute: RW

+:-----+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                | Default | Attributes |
+------+-----------------------------------------------------------------------------+---------+------------+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

## Write Pointer Register (Offset A7h\~A4h) {#write-pointer-register-offset-a7ha4h .樣式-標題-2}

Abbreviation: **WPTR**

Register Index: A7h\~A4h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Write pointer of circular queue.                 00h       RO
  ------ ------------------------------------------------ --------- ------------

## Read Pointer Register (Offset ABh\~A8h) {#read-pointer-register-offset-abha8h .樣式-標題-2}

Abbreviation: **RPTR**

Register Index: ABh\~A8h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Read pointer of circular queue.                  00h       RO
  ------ ------------------------------------------------ --------- ------------

## Write Pointer Increase Unit Register (Offset ADh\~ACh) {#write-pointer-increase-unit-register-offset-adhach .樣式-標題-2}

Abbreviation: **WPIU**

Register Index: ADh\~ACh

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **WPIU: (**FLH MSG triggers this field to maintain WPTR**)**                                   | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Writable Count Register (Offset AFh\~AEh) {#writable-count-register-offset-afhaeh .樣式-標題-2}

Abbreviation: **WR_CNT**

Register Index: AFh \~ AEh

Default Value: 00h

Attribute: RO

+:------+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                | Default | Attributes |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                     | 00h     | RO         |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 11:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|       |                                                                             |         |            |
|       | Indicate that how many amount of data can be written to the circular queue. |         |            |
+-------+-----------------------------------------------------------------------------+---------+------------+

## Read Pointer Increase Unit Register (Offset B1h\~B0h) {#read-pointer-increase-unit-register-offset-b1hb0h .樣式-標題-2}

Abbreviation: **RPIU**

Register Index: B1h\~B0h

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **RPIU: (**FW triggers this field to maintain RPTR**)**                                        | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Readable Count Register (Offset B3h\~B2h) {#readable-count-register-offset-b3hb2h .樣式-標題-2}

Abbreviation: **RD_CNT**

Register Index: B3h\~B2h

Default Value: 00h

Attribute: RO

+:------+:---------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                               | Default | Attributes |
+-------+----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                    | 00h     | RO         |
+-------+----------------------------------------------------------------------------+---------+------------+
| 11:0  | **RD_CNT:**                                                                | 00h     | RO         |
|       |                                                                            |         |            |
|       | Indicate that how many amount of data can be read from the circular queue. |         |            |
+-------+----------------------------------------------------------------------------+---------+------------+

## Buffer Length Register (Offset B5h \~B4h) {#buffer-length-register-offset-b5h-b4h .樣式-標題-2}

Abbreviation: **BUF_LEN**

Register Index: B5h \~B4h

Default Value: 00h

Attribute: RW

+:------+:-------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 15:13 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 12:0  | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (1Byte) , 2 \<= BUF_LEN \<= 4096                             |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (2Byte) , 2 \<= BUF_LEN \<= 2048                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (4Byte) , 2 \<= BUF_LEN \<= 1024                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | f\. DATA_SIZE = 5 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | g\. DATA SIZE = 6 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | h\. DATA SIZE = 7 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

## Data Size Register (Offset B6h) {#data-size-register-offset-b6h .樣式-標題-2}

Abbreviation: **DATA_SIZE**

Register Index: B6h

Default Value: 00h

Attribute: RW

+:----+:-------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                       | Default | Attributes |
+-----+--------------------------------------------------------------------+---------+------------+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

## Control Register (Offset B7h) {#control-register-offset-b7h .樣式-標題-2}

Abbreviation: **CTRL**

Register Index: B7h

Default Value: 08h

Attribute: RW

+:----+:-----------------------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue 6 (SATA MSG Completion Queue) {#circular-queue-6-sata-msg-completion-queue .樣式-標題-2}

## Queue Base Address Register (Offset C3h\~C0h) {#queue-base-address-register-offset-c3hc0h .樣式-標題-2}

Abbreviation: **QBASE_ADR**

Register Index: C3h\~C0h

Default Value: 00h

Attribute: RW

+:-----+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                | Default | Attributes |
+------+-----------------------------------------------------------------------------+---------+------------+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

## Write Pointer Register (Offset C7h\~C4h) {#write-pointer-register-offset-c7hc4h .樣式-標題-2}

Abbreviation: **WPTR**

Register Index: C7h\~C4h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Write pointer of circular queue.                 00h       RO
  ------ ------------------------------------------------ --------- ------------

## Read Pointer Register (Offset CBh\~C8h) {#read-pointer-register-offset-cbhc8h .樣式-標題-2}

Abbreviation: **RPTR**

Register Index: CBh\~C8h

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Read pointer of circular queue.                  00h       RO
  ------ ------------------------------------------------ --------- ------------

## Write Pointer Increase Unit Register (Offset CDh\~CCh) {#write-pointer-increase-unit-register-offset-cdhcch .樣式-標題-2}

Abbreviation: **WPIU**

Register Index: CDh\~CCh

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **WPIU: (**FLH MSG triggers this field to maintain WPTR**)**                                   | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Writable Count Register (Offset CFh\~CEh) {#writable-count-register-offset-cfhceh .樣式-標題-2}

Abbreviation: **WR_CNT**

Register Index: CFh \~ CEh

Default Value: 00h

Attribute: RO

+:------+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                | Default | Attributes |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                     | 00h     | RO         |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 11:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|       |                                                                             |         |            |
|       | Indicate that how many amount of data can be written to the circular queue. |         |            |
+-------+-----------------------------------------------------------------------------+---------+------------+

## Read Pointer Increase Unit Register (Offset D1h\~D0h) {#read-pointer-increase-unit-register-offset-d1hd0h .樣式-標題-2}

Abbreviation: **RPIU**

Register Index: D1h\~D0h

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **RPIU: (**FW triggers this field to maintain RPTR**)**                                        | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Readable Count Register (Offset D3h\~D2h) {#readable-count-register-offset-d3hd2h .樣式-標題-2}

Abbreviation: **RD_CNT**

Register Index: D3h\~D2h

Default Value: 00h

Attribute: RO

+:------+:---------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                               | Default | Attributes |
+-------+----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                    | 00h     | RO         |
+-------+----------------------------------------------------------------------------+---------+------------+
| 11:0  | **RD_CNT:**                                                                | 00h     | RO         |
|       |                                                                            |         |            |
|       | Indicate that how many amount of data can be read from the circular queue. |         |            |
+-------+----------------------------------------------------------------------------+---------+------------+

## Buffer Length Register (Offset D5h \~D4h) {#buffer-length-register-offset-d5h-d4h .樣式-標題-2}

Abbreviation: **BUF_LEN**

Register Index: D5h \~D4h

Default Value: 00h

Attribute: RW

+:------+:-------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 15:13 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 12:0  | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (1Byte) , 2 \<= BUF_LEN \<= 4096                             |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (2Byte) , 2 \<= BUF_LEN \<= 2048                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (4Byte) , 2 \<= BUF_LEN \<= 1024                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | f\. DATA_SIZE = 5 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | g\. DATA SIZE = 6 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | h\. DATA SIZE = 7 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

## Data Size Register (Offset D6h) {#data-size-register-offset-d6h .樣式-標題-2}

Abbreviation: **DATA_SIZE**

Register Index: D6h

Default Value: 00h

Attribute: RW

+:----+:-------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                       | Default | Attributes |
+-----+--------------------------------------------------------------------+---------+------------+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

## Control Register (Offset D7h) {#control-register-offset-d7h .樣式-標題-2}

Abbreviation: **CTRL**

Register Index: D7h

Default Value: 08h

Attribute: RW

+:----+:-----------------------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue N (7h \<= N \<= Fh) {#circular-queue-n-7h-n-fh .樣式-標題-2}

## Queue Base Address Register (Offset 03h\~00h + (N x 20h)) {#queue-base-address-register-offset-03h00h-n-x-20h .樣式-標題-2}

Abbreviation: **QBASE_ADR**

Register Index: 03h\~00h + (N x 20h)

Default Value: 00h

Attribute: RW

+:-----+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                | Default | Attributes |
+------+-----------------------------------------------------------------------------+---------+------------+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

## Write Pointer Register (Offset 07h\~04h + (N x 20h)) {#write-pointer-register-offset-07h04h-n-x-20h .樣式-標題-2}

Abbreviation: **WPTR**

Register Index: 07h\~04h + (N x 20h)

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Write pointer of circular queue.                 00h       RO
  ------ ------------------------------------------------ --------- ------------

## Read Pointer Register (Offset 0Bh\~08h + (N x 20h)) {#read-pointer-register-offset-0bh08h-n-x-20h .樣式-標題-2}

Abbreviation: **RPTR**

Register Index: 0Bh\~08h + (N x 20h)

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   Read pointer of circular queue.                  00h       RO
  ------ ------------------------------------------------ --------- ------------

## Write Pointer Increase Unit Register (Offset 0Dh\~0Ch + (N x 20h)) {#write-pointer-increase-unit-register-offset-0dh0ch-n-x-20h .樣式-標題-2}

Abbreviation: **WPIU**

Register Index: 0Dh\~0Ch + (N x 20h)

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **WPIU:**                                                                                      | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Writable Count Register (Offset 0Fh\~0Eh + (N x 20h)) {#writable-count-register-offset-0fh0eh-n-x-20h .樣式-標題-2}

Abbreviation: **WR_CNT**

Register Index: 0Fh \~ 0Eh + (N x 20h)

Default Value: 00h

Attribute: RO

+:------+:----------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                | Default | Attributes |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                     | 00h     | RO         |
+-------+-----------------------------------------------------------------------------+---------+------------+
| 11:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|       |                                                                             |         |            |
|       | Indicate that how many amount of data can be written to the circular queue. |         |            |
+-------+-----------------------------------------------------------------------------+---------+------------+

## Read Pointer Increase Unit Register (Offset 11h\~10h + (N x 20h)) {#read-pointer-increase-unit-register-offset-11h10h-n-x-20h .樣式-標題-2}

Abbreviation: **RPIU**

Register Index: 11h\~10h + (N x 20h)

Default Value: 00h

Attribute: RW

+:------+:-----------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                                   | Default | Attributes |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                                        | 00h     | RO         |
+-------+------------------------------------------------------------------------------------------------+---------+------------+
| 11:0  | **RPIU:**                                                                                      | 00h     | RW         |
|       |                                                                                                |         |            |
|       | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                           |         |            |
|       |                                                                                                |         |            |
|       | 2\. Only When low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|       |                                                                                                |         |            |
|       | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                          |         |            |
+-------+------------------------------------------------------------------------------------------------+---------+------------+

## Readable Count Register (Offset 13h\~12h + (N x 20h)) {#readable-count-register-offset-13h12h-n-x-20h .樣式-標題-2}

Abbreviation: **RD_CNT**

Register Index: 13h\~12h + (N x 20h)

Default Value: 00h

Attribute: RO

+:------+:---------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                               | Default | Attributes |
+-------+----------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserve                                                                    | 00h     | RO         |
+-------+----------------------------------------------------------------------------+---------+------------+
| 11:0  | **RD_CNT:**                                                                | 00h     | RO         |
|       |                                                                            |         |            |
|       | Indicate that how many amount of data can be read from the circular queue. |         |            |
+-------+----------------------------------------------------------------------------+---------+------------+

## Buffer Length Register (Offset 15h \~14h + (N x 20h)) {#buffer-length-register-offset-15h-14h-n-x-20h .樣式-標題-2}

Abbreviation: **BUF_LEN**

Register Index: 15h \~14h + (N x 20h)

Default Value: 00h

Attribute: RW

+:------+:-------------------------------------------------------------------------------+:--------+:-----------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 15:13 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 12:0  | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (1Byte) , 2 \<= BUF_LEN \<= 4096                             |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (2Byte) , 2 \<= BUF_LEN \<= 2048                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (4Byte) , 2 \<= BUF_LEN \<= 1024                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | f\. DATA_SIZE = 5 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | g\. DATA SIZE = 6 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | h\. DATA SIZE = 7 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

## Data Size Register (Offset 16h + (N x 20h)) {#data-size-register-offset-16h-n-x-20h .樣式-標題-2}

Abbreviation: **DATA_SIZE**

Register Index: 16h + (N x 20h)

Default Value: 00h

Attribute: RW

+:----+:-------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                       | Default | Attributes |
+-----+--------------------------------------------------------------------+---------+------------+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

## Control Register (Offset 17h + (N x 20h)) {#control-register-offset-17h-n-x-20h .樣式-標題-2}

Abbreviation: **CTRL**

Register Index: 17h + (N x 20h)

Default Value: 08h

Attribute: RW

+:----+:-----------------------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Empty Mask Control {#empty-mask-control .樣式-標題-2}

## Empty Mask Control Register (Offset 200h) {#empty-mask-control-register-offset-200h .樣式-標題-2}

Abbreviation: **EMPTY_MASK**

Register Index: 200h

Default Value: 00h

Attribute: RW

+:-----+:-------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                                     | Default | Attributes |
+------+--------------------------------------------------------------------------------------------------+---------+------------+
| 15:0 | **EMPTY_MASK :**                                                                                 | 00h     | RW         |
|      |                                                                                                  |         |            |
|      | Indicate if non-empty event of circular complete queue can wake cpu up .                         |         |            |
|      |                                                                                                  |         |            |
|      | 1\. EMPTY_MASK\[0\] = 0:                                                                         |         |            |
|      |                                                                                                  |         |            |
|      | Non-empty event of dmac completion queue can wake cpu up.                                        |         |            |
|      |                                                                                                  |         |            |
|      | 2\. EMPTY_MASK\[1\] = 0:                                                                         |         |            |
|      |                                                                                                  |         |            |
|      | Non-empty event of ss completion queue can wake cpu up.                                          |         |            |
|      |                                                                                                  |         |            |
|      | 3\. EMPTY_MASK\[2\] = 0:                                                                         |         |            |
|      |                                                                                                  |         |            |
|      | Non-empty event of flh msg dispatch completion queue can wake cpu up.                            |         |            |
|      |                                                                                                  |         |            |
|      | 4\. EMPTY_MASK\[3\] = 0:                                                                         |         |            |
|      |                                                                                                  |         |            |
|      | Non-empty event of rs msg dispatch completion queue can wake cpu up.                             |         |            |
|      |                                                                                                  |         |            |
|      | 5\. EMPTY_MASK\[4\] = 0:                                                                         |         |            |
|      |                                                                                                  |         |            |
|      | Non-empty event of sata msg dispatch completion queue can wake cpu up.                           |         |            |
|      |                                                                                                  |         |            |
|      | 6\. EMPTY_MASK\[N\] = 0 (5h\<= N \<= Fh):                                                        |         |            |
|      |                                                                                                  |         |            |
|      | Non-empty event of circular complete queue N can wake cpu up.                                    |         |            |
|      |                                                                                                  |         |            |
|      | Note:                                                                                            |         |            |
|      |                                                                                                  |         |            |
|      | While calling the tie operation REQ_STALL_BLOCK(),following sources will wake cpu up.            |         |            |
|      |                                                                                                  |         |            |
|      | a\) Non-empty event of dmac completion queue wake cpu up : REQ_STALL_BLOCK() return dataout 0x1. |         |            |
|      |                                                                                                  |         |            |
|      | b\) Non-empty event of ss completion queue can wake cpu up :                                     |         |            |
|      |                                                                                                  |         |            |
|      | REQ_STALL_BLOCK() return dataout 0x2.                                                            |         |            |
|      |                                                                                                  |         |            |
|      | c\) Non-empty event of flh msg dispatch completion queue can wake cpu up :                       |         |            |
|      |                                                                                                  |         |            |
|      | REQ_STALL_BLOCK() return dataout 0x4.                                                            |         |            |
|      |                                                                                                  |         |            |
|      | d\) Non-empty event of rs msg dispatch completion queue can wake cpu up :                        |         |            |
|      |                                                                                                  |         |            |
|      | REQ_STALL_BLOCK() return dataout 0x8.                                                            |         |            |
|      |                                                                                                  |         |            |
|      | e\) Non-empty event of sata msg dispatch completion queue can wake cpu up :                      |         |            |
|      |                                                                                                  |         |            |
|      | REQ_STALL_BLOCK() return dataout 0x10.                                                           |         |            |
|      |                                                                                                  |         |            |
|      | f\) Interrupt can wake cpu up :                                                                  |         |            |
|      |                                                                                                  |         |            |
|      | REQ_STALL_BLOCK() return dataout 0x0.                                                            |         |            |
+------+--------------------------------------------------------------------------------------------------+---------+------------+

## Acc Control {#acc-control .樣式-標題-2}

## L2P Register (Offset 213h \~ 210h) {#l2p-register-offset-213h-210h .樣式-標題-2}

Abbreviation: **L2P**

Register Index: 213h \~ 210h

Default Value: 00h

Attribute: RW

+:-----+:-------------------------------------------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                                                         | Default | Attributes |
+------+--------------------------------------------------------------------------------------+---------+------------+
| 31:0 | L2P :                                                                                | 00h     | RW         |
|      |                                                                                      |         |            |
|      | Only when 4B L2P are written, logical address to physical address will be triggered. |         |            |
+------+--------------------------------------------------------------------------------------+---------+------------+

1.

## PCA Register (Offset 217h \~ 214h)

Abbreviation: **PCA**

Register Index: 217h \~ 214h

Default Value: 00h

Attribute: RW

+:-----+:-----------------------------------------------------+:--------+:-----------+
| Bit  | Descriptions                                         | Default | Attributes |
+------+------------------------------------------------------+---------+------------+
| 31:0 | PCA : Physical cluster address                       | 00h     | RW         |
|      |                                                      |         |            |
|      | Only when 4B PCA are written, L2P will be generated. |         |            |
+------+------------------------------------------------------+---------+------------+

1.

<!-- -->

3.

## Offset Register (Offset 218h)

Abbreviation: **OFFSET**

Register Index: 218h

Default Value: 00h

Attribute: RW

  ----- ------------------------------------------------ --------- ------------
  Bit   Descriptions                                     Default   Attributes

  7:0   Offset                                           00h       RW
  ----- ------------------------------------------------ --------- ------------

## Length Register (Offset 219h)

Abbreviation: **LEN**

Register Index: 219h

Default Value: 00h

Attribute: RW

  ----- ------------------------------------------------ --------- ------------
  Bit   Descriptions                                     Default   Attributes

  7:0   Length                                           00h       RW
  ----- ------------------------------------------------ --------- ------------

1.

<!-- -->

6.
7.
8.

## Mode Register (Offset 21Ah)

Abbreviation: **MODE**

Register Index: 21Ah

Default Value: 00h

Attribute: RW

+:----+:------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                          | Default | Attributes |
+-----+-------------------------------------------------------+---------+------------+
| 7:0 | Mode :                                                | 00h     | RW         |
|     |                                                       |         |            |
|     | Indicates which L2P format is chosen to be processed. |         |            |
|     |                                                       |         |            |
|     | a\) Mode = 0 :                                        |         |            |
|     |                                                       |         |            |
|     | L2P format= PCA (26b) + OFFSET (3b) + LEN (3b)        |         |            |
|     |                                                       |         |            |
|     | b\) Mode = 1 :                                        |         |            |
|     |                                                       |         |            |
|     | L2P format= PCA (28b) + OFFSET (2b) + LEN (2b)        |         |            |
+-----+-------------------------------------------------------+---------+------------+

1.

<!-- -->

10.
11.
12.
13.

## Mask Register (Offset 21Bh)

Abbreviation: **MASK**

Register Index: 21Bh

Default Value: 00h

Attribute: RW

  ----- ------------------------------------------------ --------- ------------
  Bit   Descriptions                                     Default   Attributes

  7:0   Mask                                             00h       RW
  ----- ------------------------------------------------ --------- ------------

## Zipmap Register (Offset 21Fh \~ 21Ch)

Abbreviation: **ZIPMAP**

Register Index: 21Fh \~ 21Ch

Default Value: 00h

Attribute: RO

  ------ ------------------------------------------------ --------- ------------
  Bit    Descriptions                                     Default   Attributes

  31:0   zip map                                          00h       RO
  ------ ------------------------------------------------ --------- ------------

## Stack Overflow Monitor {#stack-overflow-monitor .樣式-標題-2}

## MON_ADDR0 Register (Offset 223h \~ 220h) {#mon_addr0-register-offset-223h-220h .樣式-標題-2}

Abbreviation: **MON_ADDR0**

Register Index: 223h \~ 220h

Default Value: 00h

Attribute: RW

  ------ ------------------------------------------------------------- --------- ------------
  Bit    Descriptions                                                  Default   Attributes

  31:0   Monitor address is used to detect if stack overflow occurs.   00h       RW
  ------ ------------------------------------------------------------- --------- ------------

## MON_ADDR1 Register (Offset 227h \~ 224h)

Abbreviation: **MON_ADDR1**

Register Index: 227h \~ 224h

Default Value: 00h

Attribute: RW

  ------ ------------------------------------------------------------- --------- ------------
  Bit    Descriptions                                                  Default   Attributes

  31:0   Monitor address is used to detect if stack overflow occurs.   00h       RW
  ------ ------------------------------------------------------------- --------- ------------

## MON_ADDR2 Register (Offset 22Bh \~ 228h)

Abbreviation: **MON_ADDR2**

Register Index: 22Bh \~ 228h

Default Value: 00h

Attribute: RW

  ------ ------------------------------------------------------------- --------- ------------
  Bit    Descriptions                                                  Default   Attributes

  31:0   Monitor address is used to detect if stack overflow occurs.   00h       RW
  ------ ------------------------------------------------------------- --------- ------------

1.

<!-- -->

4.
5.

## MON_ADDR3 Register (Offset 22Fh \~ 22Ch)

Abbreviation: **MON_ADDR3**

Register Index: 22Fh \~ 22Ch

Default Value: 00h

Attribute: RW

  ------ ------------------------------------------------------------- --------- ------------
  Bit    Descriptions                                                  Default   Attributes

  31:0   Monitor address is used to detect if stack overflow occurs.   00h       RW
  ------ ------------------------------------------------------------- --------- ------------

## MON_EN Register (Offset 230h)

Abbreviation: **MON_EN**

Register Index: 230h

Default Value: 00h

Attribute: RW

  ----- --------------------------------------------------------------------------------------------------------- --------- ------------
  Bit   Descriptions                                                                                              Default   Attributes

  7:4   Reserve                                                                                                   00h       RO

  3     While this enable bit = 1, stack overflow monitor function will detect if MON_ADDR3 occurs at dccm bus.   00h       RW

  2     While this enable bit = 1, stack overflow monitor function will detect if MON_ADDR2 occurs at dccm bus.   00h       RW

  1     While this enable bit = 1, stack overflow monitor function will detect if MON_ADDR1 occurs at dccm bus.   00h       RW

  0     While this enable bit = 1, stack overflow monitor function will detect if MON_ADDR0 occurs at dccm bus.   00h       RW
  ----- --------------------------------------------------------------------------------------------------------- --------- ------------

## Inbound Request Priority Control (Offset 240h) {#inbound-request-priority-control-offset-240h .樣式-標題-2}

Abbreviation: **INBOUND_REQ_PRI**

Register Index: 240h

Default Value: 00h

Attribute: RW

+:----+:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+:--------+:-----------+
| Bit | Descriptions                                                                                                                                                                                                                                                   | Default | Attributes |
+-----+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1:0 | PIReqPriority\[1:0\]  % of Data RAM Bandwidth Allocated to Inbound PIF                                                                                                                                                                                         | 00h     | RW         |
|     |                                                                                                                                                                                                                                                                |         |            |
|     | 11           100%                                                                                                                                                                                                                                              |         |            |
|     |                                                                                                                                                                                                                                                                |         |            |
|     | 10 Approximately         25%                                                                                                                                                                                                                                   |         |            |
|     |                                                                                                                                                                                                                                                                |         |            |
|     | 01 Approximately         25%                                                                                                                                                                                                                                   |         |            |
|     |                                                                                                                                                                                                                                                                |         |            |
|     | 00 Approximately         25%                                                                                                                                                                                                                                   |         |            |
|     |                                                                                                                                                                                                                                                                |         |            |
|     | **Note:** Use high-priority inbound PIF (PIReqPriority=2'b11) accesses with caution as issuing a long sequence of back-to-back high-priority inbound PIF accesses may stall the Xtensa processor indefinitely if they both compete for the same data RAM bank. |         |            |
|     |                                                                                                                                                                                                                                                                |         |            |
|     | Please refer to page 255 of xtensa_lx6_data_book.pdf.                                                                                                                                                                                                          |         |            |
+-----+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

# Appendix

## VQ CONTROL FLOW

+:------------------+:----------------------------------+:----------------------------------+:-----------------------------------+:-----------------------------------+:-----------------------------------+:-----------------------------+:-----------------------------+
| (SQ_vld,CQ_vld)   | DMAC                              | SS                                | FLH                                | RS                                 | SATA                               | SQ                           | CQ                           |
+-------------------+-----------------------------------+-----------------------------------+------------------------------------+------------------------------------+------------------------------------+------------------------------+------------------------------+
| (rising, rising)  | Active                            | Active                            | Active                             | Active                             | Active                             | Following three registers can be set only when valid=0:     |
|                   |                                   |                                   |                                    |                                    |                                    |                                                             |
|                   |                                   |                                   |                                    |                                    |                                    | 1\. Queue base address                                      |
|                   |                                   |                                   |                                    |                                    |                                    |                                                             |
|                   |                                   |                                   |                                    |                                    |                                    | 2\. Buffer length                                           |
|                   |                                   |                                   |                                    |                                    |                                    |                                                             |
|                   |                                   |                                   |                                    |                                    |                                    | 3\. Data size                                               |
|                   |                                   |                                   |                                    |                                    |                                    |                                                             |
|                   |                                   |                                   |                                    |                                    |                                    | Others can be set at anytime                                |
+-------------------+-----------------------------------+-----------------------------------+------------------------------------+------------------------------------+------------------------------------+                                                             |
| (falling, 1)\*    | Complete the ongoing transition, but do not fetch new command.        | X                                  | X                                  | X                                  |                                                             |
+-------------------+-----------------------------------+-----------------------------------+------------------------------------+------------------------------------+------------------------------------+                                                             |
| (1, falling)      | X                                 | X                                 | X                                  | X                                  | X                                  |                                                             |
+-------------------+-----------------------------------+-----------------------------------+------------------------------------+------------------------------------+------------------------------------+                                                             |
| (0, falling)\*\*  | 1.  Complete the ongoing AXI command and corresponding R/W packet.    | 1.  Send an abort command to stop the FLH/RS/SATA generating new command into message dispatch queue.        |                                                             |
|                   |                                                                       |                                                                                                              |                                                             |
|                   | 2.  Clear internal queues of DMAC/SS.                                 | 2.  Wait for a period of time (µs) to complete message transfer in FLH/RS/SATA message dispatch queue.       |                                                             |
|                   |                                                                       |                                                                                                              |                                                             |
|                   |                                                                       | 3.  Then set CQ_vld to low.                                                                                  |                                                             |
+-------------------+-----------------------------------+-----------------------------------+------------------------------------+------------------------------------+------------------------------------+                                                             |
| (rising, 1)\*\*\* | Active                            | Active                            | Active                             | Active                             | Active                             |                                                             |
+-------------------+-----------------------------------+-----------------------------------+------------------------------------+------------------------------------+------------------------------------+-------------------------------------------------------------+

Note:

X : Don't care.

SQ_vld: Valid queue (VQ) signal of submission queue.

CQ_vld: Valid queue (VQ) signal of completion queue.

FLH and RS have its own CQ, but they share the same message dispatcher by using an arbiter.

\*: Pause state: Complete the on-going transition, but do not fetch new command.

\*\*: CQ_vld must fall after SQ_vld fell, i.e.: (falling, 1)→(0, falling).

\*\*\*: Resume: Resume from the paused state.
