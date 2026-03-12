#### Topic: Register Space of sdram controller

Version: 0.4

**Date:** Mar 1, 2012

**by**: Philip Huang

##

Table 1. Revision History

+-----------------------+------------+--------------+--------------------------------------------+
| > **Revision Number** | > **Date** | > **Author** | > **Description**                          |
+=======================+============+==============+============================================+
| 0.1                   | 2012/3/01  | AaronMa      | Initial Draft from DDR3                    |
+-----------------------+------------+--------------+--------------------------------------------+
| 0.4                   | 2013/2/6   | Kevin        | Add ECC register                           |
+-----------------------+------------+--------------+--------------------------------------------+
| 0.5                   | 2014/06/17 | Kevin        | Add block diagram and function description |
+-----------------------+------------+--------------+--------------------------------------------+
| 0.6                   | 2014/07/01 | Philip       | Modify 6Ch \[27:25\] from PS5007           |
+-----------------------+------------+--------------+--------------------------------------------+
| 0.7                   | 2014/07/10 | Kevin        | Modify the valid address formula           |
+-----------------------+------------+--------------+--------------------------------------------+
| 0.8                   | 2015/04/28 | Kevin        | Add sub-decode usage limitation, page33    |
+-----------------------+------------+--------------+--------------------------------------------+
|                       |            |              |                                            |
+-----------------------+------------+--------------+--------------------------------------------+

1.  Feature

The DDR controller is used to control DRAM devices as well as to access the data stored on these devices. Provide multiple AXI interface for AXI master and support DFI 2.1 standard for DDR PHY. Data is transferred and protect with CRC and ECC. The data error massage will report to CPU via interrupt flag.

+-----------------------+-------------------------------------------------------+
| Item                  | Content                                               |
+=======================+:======================================================+
| Basic function        | Read/write DRAM data in the unit of AXI packet length |
|                       |                                                       |
|                       | Enhance data reliability by CRC/ECC                   |
+-----------------------+-------------------------------------------------------+
| CRC/ECC               | 1\. Zone1 : 1Byte protect with ECC                    |
|                       |                                                       |
|                       | 2\. Zone2 : 32Byte protect with CRC and ECC           |
|                       |                                                       |
|                       | 3\. Zone3 : E3D on : 514Byte protect with CRC and ECC |
|                       |                                                       |
|                       | E3D off : 512Byte protect with CRC and ECC            |
+-----------------------+-------------------------------------------------------+
| Internal bus protocol | 1\. AXI interface                                     |
|                       |                                                       |
|                       | 2\. 1 channel with data bus width = 64bit             |
|                       |                                                       |
|                       | 4\. support AXI packet length = 1B,32B,128B,512B,514B |
+-----------------------+-------------------------------------------------------+
| Error report          | Issue interrupt to CPU and report the error massage:  |
|                       |                                                       |
|                       | 1.error type (CRC, ECC correctable/uncorrect)         |
|                       |                                                       |
|                       | 2.error location                                      |
|                       |                                                       |
|                       | 3.error occur number                                  |
|                       |                                                       |
|                       | 4.error AXI master ID                                 |
+-----------------------+-------------------------------------------------------+
| DDR phy interface     | DFI 2.1                                               |
+-----------------------+-------------------------------------------------------+
| DDR/SDR               | Support                                               |
|                       |                                                       |
| Retention             |                                                       |
+-----------------------+-------------------------------------------------------+
| Support DRAM devie    | - DDR                                                 |
|                       |                                                       |
|                       | 1.  DATA RATE = 800\~1066 Mbps                        |
|                       |                                                       |
|                       | 2.  1 Rank                                            |
|                       |                                                       |
|                       | 3.  DRAM BUS: 16 Bits                                 |
|                       |                                                       |
|                       | 4.  DRAM Max Size : 512MB                             |
|                       |                                                       |
|                       | - SDR                                                 |
|                       |                                                       |
|                       | 1.  DATA RATE = 400 Mbps                              |
|                       |                                                       |
|                       | 2.  1 Rank                                            |
|                       |                                                       |
|                       | 3.  DRAM BUS: 16 Bits                                 |
|                       |                                                       |
|                       | 4.  DRAM Max Size : 32MB                              |
+-----------------------+-------------------------------------------------------+

: Table 1 DDR controller feature

2.  Block Diagram

## DDRC block diagram

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image1.emf)

## Clock description

  -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  Signal Name       Max Freq (Mhz)   Description
  ----------------- ---------------- ------------------------------------------------------------------------------------------------------------------------------------------------------------
  SYS_CLK           200              System clock, AXI interface is synchronous to this clock

  DFI_CLK           200/266          This clock originates in the ClkSync module. Deviation from the 50-percent duty cycle on this clock reduces the timing margin.200 for SDR and 266 for DDR3

  R_CLK             200              This clock originates from the pad "XMEMCKDMY" via PHY and SDLL, for SDR latch data only.
  -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## Signal description

3.  Function Description

## CRC/ECC protection overview

DDRC provide 3 kinds of CRC/ECC protection function, they are protect_1B, protect_32B, and protect_514B. And the Addr Decoder will map the data to DRAM space called Zone1, Zone2, and Zone3. The Zone configuration is programmable via register setting.

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image2.emf)

## Zone data protect enable and disable

For each Zone, user can enable or disable the Zone data protect via setting system register (refer to PS3111_RegSpec_xx.x.doc).

## Zone size setting

User can set the Zone size via setting system register (refer to PS3111_RegSpec_xx.x.doc). Base on the Zone size setting, the valid/invalid area is decided and for each Zone, user only can use the valid area, and the invalid area is not allow to access. If user touch the invalid area, DDRC will issue the invalid_access interrupt to CPU. Therefore, firmware need to the Zone usage carefully.

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image3.emf)

## Zone AXI data format limitation

The table indicates the AXI data format limitation for each Zone in the different protection mode.

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image4.emf)

## Zone1 data format

The diagram indicates the Zone1 data format. Zone1 only support ECC protection, and 1B data will protect by extra 1B ECC code.

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image5.emf)

## Zone2 data format

The diagram indicates the Zone2 data format. Zone2 support CRC and ECC protection, and 32B data will protect by extra 2B CRC code and 2B ECC code.

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image6.emf)

## Zone3 data format

The diagram indicates the Zone3 data format. Zone3 support CRC and ECC protection. When E3D enabel, 514B data will protect by extra 2B CRC code and 4B ECC code. When E3D enabel, 512B data will protect by extra 2B CRC code and 4B ECC code.

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image7.emf)

## Data Error Report

When data CRC or ECC checking error happen, DDRC will issue the interrupt to CPU and record the error status and message in the register as below diagram.

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image8.emf)

## MEM Interface

It is similar as AXI interface. AXI command Write and Read path are combined into MEM_CMD_REQ, MEM_CM_ACK, and MEM_WR_N, MEM_ADDR, MEM_LEN. Read or Write direction is controlled by MEM_WR_N. MEM_DOUT_VALID, MEM_DOUT_READY, and MEM_DOUT are as same as AXI Data Write path. MEM_DIN_VALID, MEM_DIN_READY, and MEM_DIN are as same as AXI DATA Read path.

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image9.emf)

## DDR MCTRL Block

Read or Write data in MEM interface transforms to DFI protocol in DDR MCTRL Block. Besides, DDR MCTRL block also supports Self-Refresh, Auto-Refresh, Mode register Set, MWR, MRR, Deep power down, Power down, and ZQ Calibration that DRAM needs.

There are many functions support below.

1.  Read or Write data access is in MEM interface.

2.  Issuing Self-Refresh, Auto-Refresh, Mode register Set, MWR, MRR, Deep power down, and Power down are controlled by Register Offset \[0x6010\]. Every time only one command can be issued. And Before issuing one of the command please make sure Bit0\~9 are zero, and it means last command is finished.

3.  Issuing Auto-Refresh once is controlled by Register Offset \[0x6010\] Bit0. Or you can issue Auto-Refresh by setting Offset \[0x6000\] Bit0 one, and then it will issue by itself in the period.

4.  Issuing ZQ Calibration once is controlled by Register Offset \[0x6010\] Bit6. Or you can issue ZQ Calibration by setting Offset \[0x601C\] Bit15 one, and then it will issue by itself in the period.

<!-- -->

4.  Register Spec

Offset:0400_5000

##### DDR3 REF & DBG CFG register (Offset 000h)

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                      | Default | Attributes |
+:=====:+===================================================================================================================+:=======:+:==========:+
| 31:29 | Reserved                                                                                                          | 0b      | R          |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| 28    | Reserved for using in simulation (for low frequency)                                                              | 0b      | RW         |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27    | Reserved                                                                                                          | 0b      | R          |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| 26:24 | Reserved                                                                                                          | 0b      | RW         |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23    | Reserved                                                                                                          | 0b      | R          |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| 22:16 | **DB_DEL\[6:0\]: It set selection of Debug**                                                                      | 0b      | RW         |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15:9  | Reserved                                                                                                          | 0b      | R          |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| 8:4   | Reserved for using in simulation(define FPGA, PHISON_PHY, LPDDR2, BL4, x32 )                                      | 0b      | R          |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3:1   | Reserved                                                                                                          | 0b      | R          |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0     | **REN: CBR auto refresh enable**                                                                                  | 0b      | RW         |
|       |                                                                                                                   |         |            |
|       | 0: Disable                                                                                                        |         |            |
|       |                                                                                                                   |         |            |
|       | 1: Enable CBR auto refresh is issued when the internal counter is reached setting value of DDR2 REF CNT registers |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3_REF CNT register (Offset 004h)

Default Value:100C_00A2h

+-------+--------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                             | Default | Attributes |
+:=====:+==========================================================================+:=======:+:==========:+
| 31:29 | Reserved                                                                 | 0b      | R          |
+-------+--------------------------------------------------------------------------+---------+------------+
| 28    | **CBRSEL: It set which refresh counter is used**                         | 1b      | RW         |
|       |                                                                          |         |            |
|       | 0: Use CNT0                                                              |         |            |
|       |                                                                          |         |            |
|       | 1: Use CNT1                                                              |         |            |
+-------+--------------------------------------------------------------------------+---------+------------+
| 27:25 | Reserved                                                                 | 0b      | R          |
+-------+--------------------------------------------------------------------------+---------+------------+
| 24:16 | **CNT1: It set the number of the CBR auto refresh cycle**                | Ch      | RW         |
|       |                                                                          |         |            |
|       | Initial value 0xC becomes 3.840 us at the time of DDR_CLK=20ns(50MHz)    |         |            |
|       |                                                                          |         |            |
|       | Time is DDR_CLK\*CNT1\*16                                                |         |            |
+-------+--------------------------------------------------------------------------+---------+------------+
| 15:9  | Reserved                                                                 | 0b      | R          |
+-------+--------------------------------------------------------------------------+---------+------------+
| 8:0   | **CNT0: It set the number of the CBR auto refresh cycle**                | A2h     | RW         |
|       |                                                                          |         |            |
|       | Initial value 0xA2 becomes 3.888 us at thr time of DDR_CLK=1.5ns(666MHz) |         |            |
|       |                                                                          |         |            |
|       | Time is DDR_CLK\*CNT0\*16                                                |         |            |
+-------+--------------------------------------------------------------------------+---------+------------+

##### DDR3_ODT MUX register (Offset 008h)

Default Value:0000_8421h

+-------+---------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                          | Default | Attributes |
+:=====:+=======================================================================================+:=======:+:==========:+
| 31:28 | **DQPIN_SL: Select which spaces of 2 bytes for DRAM 16BITS Mode**                     | 0b      | RW         |
|       |                                                                                       |         |            |
|       | 0: {rank1,rank0}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 1: {rank2,rank0}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 2: {rank3,rank0}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 3: {rank2,rank1}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 4: {rank3,rank1}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 5: {rank3,rank2}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 6: {rank0,rank1}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 7: {rank0,rank2}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 8: {rank0,rank3}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 9: {rank1,rank2}                                                                      |         |            |
|       |                                                                                       |         |            |
|       | 10: {rank1,rank3}                                                                     |         |            |
|       |                                                                                       |         |            |
|       | Others : {rank2,rank3}                                                                |         |            |
|       |                                                                                       |         |            |
|       | **DQPIN_SL: Select which spaces of 4 bytes for DRAM 32BITS Mode**                     |         |            |
|       |                                                                                       |         |            |
|       | 0: { rank3,rank2,rank1,rank0}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 1: { rank3,rank1,rank2,rank0}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 2: { rank2,rank1,rank3,rank0}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 3: { rank3,rank0,rank2,rank1}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 4: { rank2,rank0,rank3,rank1}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 5: { rank1,rank0,rank3,rank2}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 6: { rank3,rank2,rank0,rank1}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 7: { rank3,rank1,rank0,rank2}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 8: { rank2,rank1,rank0,rank3}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 9: { rank3,rank0,rank1,rank2}                                                         |         |            |
|       |                                                                                       |         |            |
|       | 10: { rank2,rank0,rank1,rank3}                                                        |         |            |
|       |                                                                                       |         |            |
|       | Others : { rank1,rank0,rank2,rank3}                                                   |         |            |
+-------+---------------------------------------------------------------------------------------+---------+------------+
| 27:23 | Reserved                                                                              | 0b      | R          |
+-------+---------------------------------------------------------------------------------------+---------+------------+
| 22:20 | **ODT_DYM_POST:**                                                                     | 0h      | RW         |
|       |                                                                                       |         |            |
|       | 0: No ODT turn off late. Turn off time is the same as Synchronize ODT.                |         |            |
|       |                                                                                       |         |            |
|       | 1: ODT turn off late 1 cycle                                                          |         |            |
|       |                                                                                       |         |            |
|       | 2: ODT turn off late 2 cycle                                                          |         |            |
|       |                                                                                       |         |            |
|       | 3: ODT turn off late 3 cycle                                                          |         |            |
|       |                                                                                       |         |            |
|       | others: ODT turn off late 4 cycle                                                     |         |            |
+-------+---------------------------------------------------------------------------------------+---------+------------+
| 19    | Reserved                                                                              | 0b      | R          |
+-------+---------------------------------------------------------------------------------------+---------+------------+
| 18:16 | **ODT_DYM_PRE:**                                                                      | 0h      | RW         |
|       |                                                                                       |         |            |
|       | 0: No ODT turn on early. Turn on time is the same as Synchronize ODT.                 |         |            |
|       |                                                                                       |         |            |
|       | 1: ODT turn on early 1 cycle                                                          |         |            |
|       |                                                                                       |         |            |
|       | 2: ODT turn on early 2 cycle                                                          |         |            |
|       |                                                                                       |         |            |
|       | 3: ODT turn on early 3 cycle                                                          |         |            |
|       |                                                                                       |         |            |
|       | others: ODT turn on early 4 cycle                                                     |         |            |
+-------+---------------------------------------------------------------------------------------+---------+------------+
| 15:12 | **RK3_ODT: It set relationship of rank1 odt with write all rank**                     | 8h      | RW         |
|       |                                                                                       |         |            |
|       | 0000: rank3 odt turn off                                                              |         |            |
|       |                                                                                       |         |            |
|       | 0001: rank3 odt turn on when write rank0                                              |         |            |
|       |                                                                                       |         |            |
|       | 0010: rank3 odt turn on when write rank1                                              |         |            |
|       |                                                                                       |         |            |
|       | 0011: rank3 odt turn on when write rank0 or write rank1                               |         |            |
|       |                                                                                       |         |            |
|       | 0100: rank3 odt turn on when write rank2                                              |         |            |
|       |                                                                                       |         |            |
|       | 0101: rank3 odt turn on when write rank0 or write rank2                               |         |            |
|       |                                                                                       |         |            |
|       | 0110: rank3 odt turn on when write rank1 or write rank2                               |         |            |
|       |                                                                                       |         |            |
|       | 0111: rank3 odt turn on when write rank0 or write rank1 or write rank2                |         |            |
|       |                                                                                       |         |            |
|       | **.**                                                                                 |         |            |
|       |                                                                                       |         |            |
|       | **.**                                                                                 |         |            |
|       |                                                                                       |         |            |
|       | 1111: rank3 odt turn on when write rank0 or write rank1 or write rank2 or write rank3 |         |            |
+-------+---------------------------------------------------------------------------------------+---------+------------+
| 11:8  | **RK2_ODT: It set relationship of rank1 odt with write all rank**                     | 4h      | RW         |
|       |                                                                                       |         |            |
|       | 0000: rank2 odt turn off                                                              |         |            |
|       |                                                                                       |         |            |
|       | 0001: rank2 odt turn on when write rank0                                              |         |            |
|       |                                                                                       |         |            |
|       | 0010: rank2 odt turn on when write rank1                                              |         |            |
|       |                                                                                       |         |            |
|       | 0011: rank2 odt turn on when write rank0 or write rank1                               |         |            |
|       |                                                                                       |         |            |
|       | 0100: rank2 odt turn on when write rank2                                              |         |            |
|       |                                                                                       |         |            |
|       | 0101: rank2 odt turn on when write rank0 or write rank2                               |         |            |
|       |                                                                                       |         |            |
|       | 0110: rank2 odt turn on when write rank1 or write rank2                               |         |            |
|       |                                                                                       |         |            |
|       | 0111: rank2 odt turn on when write rank0 or write rank1 or write rank2                |         |            |
|       |                                                                                       |         |            |
|       | **.**                                                                                 |         |            |
|       |                                                                                       |         |            |
|       | **.**                                                                                 |         |            |
|       |                                                                                       |         |            |
|       | 1111: rank2 odt turn on when write rank0 or write rank1 or write rank2 or write rank3 |         |            |
+-------+---------------------------------------------------------------------------------------+---------+------------+
| 7:4   | **RK1_ODT: It set relationship of rank1 odt with write all rank**                     | 2h      | RW         |
|       |                                                                                       |         |            |
|       | 0000: rank1 odt turn off                                                              |         |            |
|       |                                                                                       |         |            |
|       | 0001: rank1 odt turn on when write rank0                                              |         |            |
|       |                                                                                       |         |            |
|       | 0010: rank1 odt turn on when write rank1                                              |         |            |
|       |                                                                                       |         |            |
|       | 0011: rank1 odt turn on when write rank0 or write rank1                               |         |            |
|       |                                                                                       |         |            |
|       | 0100: rank1 odt turn on when write rank2                                              |         |            |
|       |                                                                                       |         |            |
|       | 0101: rank1 odt turn on when write rank0 or write rank2                               |         |            |
|       |                                                                                       |         |            |
|       | 0110: rank1 odt turn on when write rank1 or write rank2                               |         |            |
|       |                                                                                       |         |            |
|       | 0111: rank1 odt turn on when write rank0 or write rank1 or write rank2                |         |            |
|       |                                                                                       |         |            |
|       | **.**                                                                                 |         |            |
|       |                                                                                       |         |            |
|       | **.**                                                                                 |         |            |
|       |                                                                                       |         |            |
|       | 1111: rank1 odt turn on when write rank0 or write rank1 or write rank2 or write rank3 |         |            |
+-------+---------------------------------------------------------------------------------------+---------+------------+
| 3:0   | **RK0_ODT: It set relationship of rank0 odt with write all rank**                     | 1h      | RW         |
|       |                                                                                       |         |            |
|       | 0000: rank0 odt turn off                                                              |         |            |
|       |                                                                                       |         |            |
|       | 0001: rank0 odt turn on when write rank0                                              |         |            |
|       |                                                                                       |         |            |
|       | 0010: rank0 odt turn on when write rank1                                              |         |            |
|       |                                                                                       |         |            |
|       | 0011: rank0 odt turn on when write rank0 or write rank1                               |         |            |
|       |                                                                                       |         |            |
|       | 0100: rank0 odt turn on when write rank2                                              |         |            |
|       |                                                                                       |         |            |
|       | 0101: rank0 odt turn on when write rank0 or write rank2                               |         |            |
|       |                                                                                       |         |            |
|       | 0110: rank0 odt turn on when write rank1 or write rank2                               |         |            |
|       |                                                                                       |         |            |
|       | 0111: rank0 odt turn on when write rank0 or write rank1 or write rank2                |         |            |
|       |                                                                                       |         |            |
|       | **.**                                                                                 |         |            |
|       |                                                                                       |         |            |
|       | **.**                                                                                 |         |            |
|       |                                                                                       |         |            |
|       | 1111: rank0 odt turn on when write rank0 or write rank1 or write rank2 or write rank3 |         |            |
+-------+---------------------------------------------------------------------------------------+---------+------------+

##### DDR3_RANK Timing Parameter register (Offset 00Ch)

Default Value:0000_20c1h

+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                                                                                  | Default | Attributes |
+:=====:+===============================================================================================================================================================================================================+:=======:+:==========:+
| 31:16 | Reserved                                                                                                                                                                                                      | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | **R2W_DIFRK: It set wait interval when from Read data to Write data in different ranks memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                                  | 2h      | RW         |
|       |                                                                                                                                                                                                               |         |            |
|       | 0000: 0 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0001: 1 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 1111: 15 cycles                                                                                                                                                                                               |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 11:9  | Reserved                                                                                                                                                                                                      | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 8:4   | **TWTR_AL_DIFRK: It set cycle of TWTR add AL in different ranks memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                                                         | Ch      | RW         |
|       |                                                                                                                                                                                                               |         |            |
|       | 0_0000: 0 cycles                                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0_0001: 1 cycles                                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 1_1111: 31 cycles                                                                                                                                                                                             |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3:0   | **TCCD_DIFRK: It set wait interval when from Read data to Read data or from Write data to Write data in different ranks memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)** | 1h      | RW         |
|       |                                                                                                                                                                                                               |         |            |
|       | **If SDR Mode,**                                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0000: 8 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0001: 9 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 1111: 23 cycles                                                                                                                                                                                               |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | **If DDR3 (bit31 of register\[0x18\] is 0x0),**                                                                                                                                                               |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0000: 4 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0001: 5 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 1111: 19 cycles                                                                                                                                                                                               |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | **If LPDDR2 (bit31 of register\[0x18\] is 0x1) and BL2(bit22\~21 of register\[0x28\] is 0x0),**                                                                                                               |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0000: 1 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0001: 2 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 1111: 16 cycles                                                                                                                                                                                               |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | **If LPDDR2 (bit31 of register\[0x18\] is 0x1) and BL4(bit22\~21 of register\[0x28\] is 0x1)**                                                                                                                |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0000: 2 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0001: 3 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 1111: 17 cycles                                                                                                                                                                                               |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | **If LPDDR2 (bit31 of register\[0x18\] is 0x1) and BL8(bit22\~21 of register\[0x28\] is 0x2 or 0x3)**                                                                                                         |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0000: 4 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 0001: 5 cycle                                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | .                                                                                                                                                                                                             |         |            |
|       |                                                                                                                                                                                                               |         |            |
|       | 1111: 19 cycles                                                                                                                                                                                               |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3_MEM\_ INIT register (Offset 010h)

Default Value: 0000_0000h

+-------+-----------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                    | Default | Attributes |
+:=====:+=================================================================+:=======:+:==========:+
| 31:16 | **If DDR3 (bit31 of register\[0x18\] is 0x0),**                 | 0b      | RW         |
|       |                                                                 |         |            |
|       | **MRS_ADD: Address of Mode Register Setting**                   |         |            |
|       |                                                                 |         |            |
|       | **If LPDDR2 (bit31 of register\[0x18\] is 0x1),**               |         |            |
|       |                                                                 |         |            |
|       | **MRS_ADD \[15:8\]: OP7\~OP0 of MRW**                           |         |            |
|       |                                                                 |         |            |
|       | **MRS_ADD \[ 7: 0\]: MA7\~MA0 of MRW/MRR**                      |         |            |
+-------+-----------------------------------------------------------------+---------+------------+
| 15    | **CLR_CMD: Clear all waiting command**                          | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+
| 14:12 | **If DDR3 (bit31 of register\[0x18\] is 0x0),**                 | 0b      | RW         |
|       |                                                                 |         |            |
|       | **MRS_BA: Bank address of Mode Register Setting**               |         |            |
|       |                                                                 |         |            |
|       | **If LPDDR2 (bit31 of register\[0x18\] is 0x1),**               |         |            |
|       |                                                                 |         |            |
|       | **xx0 : Mode Register Write**                                   |         |            |
|       |                                                                 |         |            |
|       | **xx1 : Mode Register Read**                                    |         |            |
+-------+-----------------------------------------------------------------+---------+------------+
| 11:10 | **MRS_RANK: Rank address of Mode Register Setting**             | 0b      | RW         |
+-------+-----------------------------------------------------------------+---------+------------+
| 9     | **EXI_DRST: Exit DDR3 reset**                                   | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+
| 8     | **DRST: Entry DDR3 reset**                                      | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+
| 7     | **DPDN: Entry Deep Power down**                                 | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+
| 6     | **ZQC: Long ZQ calibration**                                    | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+
| 5     | **EXI_PDN: Exit Deep Power down or Power down or Self-refresh** | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+
| 4     | **PDN: Entry Power down**                                       | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+
| 3     | **SRF: Entry Self refresh**                                     | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+
| 2     | **PRE: Precharge all**                                          | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+
| 1     | **If DDR3 (bit31 of register\[0x18\] is 0x0),**                 | 0b      | RW1        |
|       |                                                                 |         |            |
|       | **MRS: Mode register setting**                                  |         |            |
|       |                                                                 |         |            |
|       | **If LPDDR2 (bit31 of register\[0x18\] is 0x1),**               |         |            |
|       |                                                                 |         |            |
|       | **MRS: Mode Register Write/Read**                               |         |            |
+-------+-----------------------------------------------------------------+---------+------------+
| 0     | **REF: Auto refresh**                                           | 0b      | RW1        |
+-------+-----------------------------------------------------------------+---------+------------+

Note: Only one command permission once.

##### DDR3_MEMC_CFG1 register (Offset 014h)

Default Value: 4C25_C544h

+-------+-------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                              | Default | Attributes |
+:=====:+===========================================================================================================================================+:=======:+:==========:+
| 31:30 | **TRTP: It set TRTP of the using memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                    | 1b      | RW         |
|       |                                                                                                                                           |         |            |
|       | **When BL=1(BL4)**                                                                                                                        |         |            |
|       |                                                                                                                                           |         |            |
|       | 00: 6 cycles                                                                                                                              |         |            |
|       |                                                                                                                                           |         |            |
|       | 01: 7 cycles                                                                                                                              |         |            |
|       |                                                                                                                                           |         |            |
|       | 10: 8 cycles                                                                                                                              |         |            |
|       |                                                                                                                                           |         |            |
|       | 11: 9 cycles                                                                                                                              |         |            |
|       |                                                                                                                                           |         |            |
|       | **SDR mode or Others**                                                                                                                    |         |            |
|       |                                                                                                                                           |         |            |
|       | 00: 8 cycles                                                                                                                              |         |            |
|       |                                                                                                                                           |         |            |
|       | 01: 9 cycles                                                                                                                              |         |            |
|       |                                                                                                                                           |         |            |
|       | 10: 10 cycles                                                                                                                             |         |            |
|       |                                                                                                                                           |         |            |
|       | 11: 11 cycles                                                                                                                             |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 29    | **Please reference TWTR_AL**                                                                                                              |         | RW         |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 28:24 | **TRAS: It set TRAS of the using memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                    | Ch      | RW         |
|       |                                                                                                                                           |         |            |
|       | 0_0000: It is setting prohibited                                                                                                          |         |            |
|       |                                                                                                                                           |         |            |
|       | 0_0001: 1 cycles                                                                                                                          |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | 1_1111: 31ycles                                                                                                                           |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:16 | **TRFC: It set TRFC of the using memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                    | 25h     | RW         |
|       |                                                                                                                                           |         |            |
|       | 0000_0000: It is setting prohibited                                                                                                       |         |            |
|       |                                                                                                                                           |         |            |
|       | 0000_0001: 1cycles                                                                                                                        |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | 1111_1111: 255ycles                                                                                                                       |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 29,   | **TWTR_AL: It set cycle of TWTR add AL of the using memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)** | 0Ch     | RW         |
|       |                                                                                                                                           |         |            |
| 15:12 | 0_0000: 0 cycles                                                                                                                          |         |            |
|       |                                                                                                                                           |         |            |
|       | 0_0001: 1 cycles                                                                                                                          |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | 1_1111: 31 cycles                                                                                                                         |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 11:8  | **TWR: It set TWR of the using memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                      | 5h      | RW         |
|       |                                                                                                                                           |         |            |
|       | 0000: It is setting prohibited                                                                                                            |         |            |
|       |                                                                                                                                           |         |            |
|       | 0001: 1 cycles                                                                                                                            |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | 1111: 15 cycles                                                                                                                           |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4   | **TRP: It set TRP of the using memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                      | 4h      | RW         |
|       |                                                                                                                                           |         |            |
|       | 0000: It is setting prohibited                                                                                                            |         |            |
|       |                                                                                                                                           |         |            |
|       | 0001: It is setting prohibited                                                                                                            |         |            |
|       |                                                                                                                                           |         |            |
|       | 0002: It is setting prohibited                                                                                                            |         |            |
|       |                                                                                                                                           |         |            |
|       | 0003: It is setting prohibited                                                                                                            |         |            |
|       |                                                                                                                                           |         |            |
|       | 0004: 4 cycles                                                                                                                            |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | 1111: 15 cycles                                                                                                                           |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3:0   | **TRCD: It set TRCD of the using memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                    | 4h      | RW         |
|       |                                                                                                                                           |         |            |
|       | 0000: It is setting prohibited                                                                                                            |         |            |
|       |                                                                                                                                           |         |            |
|       | 0001: 1 cycles                                                                                                                            |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | .                                                                                                                                         |         |            |
|       |                                                                                                                                           |         |            |
|       | 1111: 15 cycles                                                                                                                           |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3\_ MEMC_CFG2 register (Offset 018h)

Default Value: 0021_0013h

+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                                                   | Default | Attributes |
+:=====:+================================================================================================================================================================================+:=======:+:==========:+
| 31    | **DDR2: Select LPDDR2 or DDR3.**                                                                                                                                               | 0b      | RW         |
|       |                                                                                                                                                                                |         |            |
|       | 0: DDR3                                                                                                                                                                        |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 1: LPDDR2 (not support for PS3111)                                                                                                                                             |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 30    | **BANK: It set bank width of the using memory.**                                                                                                                               | 0b      | RW         |
|       |                                                                                                                                                                                |         |            |
|       | 0: 8 banks                                                                                                                                                                     |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 1: 4 banks                                                                                                                                                                     |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 29:28 | **RANK: It set rank width of the using memory.**                                                                                                                               | 0b      | RW         |
|       |                                                                                                                                                                                |         |            |
|       | 00: 1 rank                                                                                                                                                                     |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 01: 2 ranks (not support for PS3111)                                                                                                                                           |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 10: 3 ranks (not support for PS3111)                                                                                                                                           |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 11: 4 ranks (not support for PS3111)                                                                                                                                           |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27:26 | Reserved                                                                                                                                                                       | 0b      | R          |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 25:24 | **SFI_RDVALID_LAT: SFI SDR Read Valid latency.**                                                                                                                               | 1b      | RW         |
|       |                                                                                                                                                                                |         |            |
|       | 00: 5                                                                                                                                                                          |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 01: 6                                                                                                                                                                          |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 10: 7                                                                                                                                                                          |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 11: 8                                                                                                                                                                          |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23    | **SDR_D_VALID_f_en:** can be selected falling "we" for aysc-fifo.                                                                                                              | 0b      | R          |
|       |                                                                                                                                                                                |         |            |
|       | It can be combined by "SFI_RDVALID_LAT" for coarse\[2:0\]                                                                                                                      |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 22:20 | **ROW: It set low address width of the using memory.**                                                                                                                         | 2h      | RW         |
|       |                                                                                                                                                                                |         |            |
|       | 000: 12 bits                                                                                                                                                                   |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 001: 13 bits                                                                                                                                                                   |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 010: 14 bits                                                                                                                                                                   |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 011: 15 bits                                                                                                                                                                   |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 100: 16 bits                                                                                                                                                                   |         |            |
|       |                                                                                                                                                                                |         |            |
|       | Others: It is setting prohibited                                                                                                                                               |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 19    | Reserved                                                                                                                                                                       | 0b      | R          |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 18:16 | **COL: It set column address width of the using memory.**                                                                                                                      | 1h      | RW         |
|       |                                                                                                                                                                                |         |            |
|       | 000: 8 bits                                                                                                                                                                    |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 001: 9 bits                                                                                                                                                                    |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 010: 10 bits                                                                                                                                                                   |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 011: 11 bits                                                                                                                                                                   |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 1xx: 5bits (MRAM)                                                                                                                                                              |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15    | Reserved                                                                                                                                                                       | 0b      | R          |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 14:12 | **TRRD: It set TRRD of the using memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                                                         | 0b      | RW         |
|       |                                                                                                                                                                                |         |            |
|       | 000: 4 cycles                                                                                                                                                                  |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 001: 5 cycles                                                                                                                                                                  |         |            |
|       |                                                                                                                                                                                |         |            |
|       | .                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                |         |            |
|       | .                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 111: 11 cycles                                                                                                                                                                 |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 11    | Reserved                                                                                                                                                                       | 0b      | R          |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 10:8  | **CL_ADJUST\[2:0\]: It adjust CAS latency of the using memory for internal clock latching correctly.**                                                                         | 0b      | RW         |
|       |                                                                                                                                                                                |         |            |
|       | 000: CL                                                                                                                                                                        |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 001: CL-3                                                                                                                                                                      |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 010: CL-2                                                                                                                                                                      |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 011: CL-1                                                                                                                                                                      |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 100: CL+1                                                                                                                                                                      |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 101: CL+2                                                                                                                                                                      |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 110: CL+3                                                                                                                                                                      |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 111: CL                                                                                                                                                                        |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4   | **CWL: It read CAS write latency by Mode register setting. It is ignored when CWLSEL is 0x1. The unit is memory** **clock cycle/(It is 2 times as large as DFICLK frequency)** | 1h      | R          |
|       |                                                                                                                                                                                |         |            |
|       | 0001: 1 cycles                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                |         |            |
|       | .                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                |         |            |
|       | .                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 1011: 11 cycles                                                                                                                                                                |         |            |
|       |                                                                                                                                                                                |         |            |
|       | Others: It is setting prohibited                                                                                                                                               |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3:0   | **CL: It read CAS latency by Mode register setting. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                                              | 3h      | R          |
|       |                                                                                                                                                                                |         |            |
|       | 0010: 2 cycles                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                |         |            |
|       | .                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                |         |            |
|       | .                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                |         |            |
|       | 1011: 11 cycles                                                                                                                                                                |         |            |
|       |                                                                                                                                                                                |         |            |
|       | Others: It is setting prohibited                                                                                                                                               |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3\_ MEMC_CFG3 register (Offset 01Ch)

Default Value: 01f0_7020h

+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                                     | Default | Attributes |
+:=====:+==================================================================================================================================================================+:=======:+:==========:+
| 31    | Reserved                                                                                                                                                         | 0b      | R          |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 30:28 | **CWL_ADJUST\[2:0\]: It adjust CAS Write latency of the using memory for internal clock latching correctly.**                                                    | 0b      | RW         |
|       |                                                                                                                                                                  |         |            |
|       | 000: CWL                                                                                                                                                         |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 001: CWL-3                                                                                                                                                       |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 010: CWL-2                                                                                                                                                       |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 011: CWL-1                                                                                                                                                       |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 100: CWL+1                                                                                                                                                       |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 101: CWL+2                                                                                                                                                       |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 110: CWL+3                                                                                                                                                       |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 111: CWL                                                                                                                                                         |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27:25 | Reserved                                                                                                                                                         | 0h      | R          |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 24:20 | **TFAW: It set four active bank window. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)**                                            | 1fh     | RW         |
|       |                                                                                                                                                                  |         |            |
|       | 00000: 16 cycles                                                                                                                                                 |         |            |
|       |                                                                                                                                                                  |         |            |
|       | .                                                                                                                                                                |         |            |
|       |                                                                                                                                                                  |         |            |
|       | .                                                                                                                                                                |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 11111: 47 cycles                                                                                                                                                 |         |            |
|       |                                                                                                                                                                  |         |            |
|       | Others: It is setting prohibited                                                                                                                                 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 19:16 | Reserved                                                                                                                                                         | 0h      | R          |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15    | **ZQCSEN: It set automatic publication of ZQCS of the DDR3 memory. Please set 0 during memory initialization.**                                                  | 0b      | RW         |
|       |                                                                                                                                                                  |         |            |
|       | 0: Not published                                                                                                                                                 |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 1: Published                                                                                                                                                     |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 14:12 | **ZQCST: It set time interval publishing ZQCS**                                                                                                                  | 7h      | RW         |
|       |                                                                                                                                                                  |         |            |
|       | 000: It is setting prohibited                                                                                                                                    |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 001: Published once every one CBR auto refresh                                                                                                                   |         |            |
|       |                                                                                                                                                                  |         |            |
|       | .                                                                                                                                                                |         |            |
|       |                                                                                                                                                                  |         |            |
|       | .                                                                                                                                                                |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 111: Published once every seven CBR auto refresh                                                                                                                 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 11:8  | Reserved                                                                                                                                                         | 0b      | R          |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4   | **R2W: It set wait interval when from Read data to Write data in the using memory. The unit is memory clock cycle/(It is 2 times as large as DFICLK frequency)** | 2h      | RW         |
|       |                                                                                                                                                                  |         |            |
|       | 0000: 0 cycle                                                                                                                                                    |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 0001: 1 cycle                                                                                                                                                    |         |            |
|       |                                                                                                                                                                  |         |            |
|       | .                                                                                                                                                                |         |            |
|       |                                                                                                                                                                  |         |            |
|       | .                                                                                                                                                                |         |            |
|       |                                                                                                                                                                  |         |            |
|       | 1111: 15 cycles                                                                                                                                                  |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3:0   | Reserved                                                                                                                                                         | 0b      | R          |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3\_ MEMC_CFG4 register (Offset 020h)

Default Value: 0000_0003h

+-------+-----------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                          | Default | Attributes |
+:=====:+=======================================================================================================================+:=======:+:==========:+
| 31    | **DFICKDEN: DRAM Clock Disable**                                                                                      | 0b      | R          |
|       |                                                                                                                       |         |            |
|       | 0: DRAM Clock enable                                                                                                  |         |            |
|       |                                                                                                                       |         |            |
|       | 1: DRAM Clock disable                                                                                                 |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 31:21 | Reserved                                                                                                              | 0b      | R          |
+-------+-----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 20:16 | **tphy_wrlat: DFI parameter: It read tphy_wrlat. Please set it depending on WL(write lantency) of connected memory.** | 0h      | R          |
|       |                                                                                                                       |         |            |
|       | Synopsys_DDR_PHY:                                                                                                     |         |            |
|       |                                                                                                                       |         |            |
|       | **tphy_wrlat** : (WL-3)/2 and ignore floating value                                                                   |         |            |
|       |                                                                                                                       |         |            |
|       | ex. 5'd3 : WL=9 or 10 (tphy_wrlat=3)                                                                                  |         |            |
|       |                                                                                                                       |         |            |
|       | Renesas DDR PHY:                                                                                                      |         |            |
|       |                                                                                                                       |         |            |
|       | DDR3:                                                                                                                 |         |            |
|       |                                                                                                                       |         |            |
|       | **tphy_wrlat** : WL/2 and ignore floating value                                                                       |         |            |
|       |                                                                                                                       |         |            |
|       | ex. 5'd3 : WL=6 or 7 (tphy_wrlat=3)                                                                                   |         |            |
|       |                                                                                                                       |         |            |
|       | LPDDR2:                                                                                                               |         |            |
|       |                                                                                                                       |         |            |
|       | **tphy_wrlat** : (WL+1)/2 and ignore floating value                                                                   |         |            |
|       |                                                                                                                       |         |            |
|       | ex. 5'd3 : WL=5 or 6 (tphy_wrlat=3)                                                                                   |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15:13 | Reserved                                                                                                              | 0b      | R          |
+-------+-----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 12:8  | **trddata_en: DFI parameter: It read trddata_en. Please set it depending on RL(read lantency) of using memory.**      | 0h      | R          |
|       |                                                                                                                       |         |            |
|       | Synopsys_DDR_PHY:                                                                                                     |         |            |
|       |                                                                                                                       |         |            |
|       | **trddata_en** : (RL-3)/2 and ignore floating value                                                                   |         |            |
|       |                                                                                                                       |         |            |
|       | ex. 5'd3 : RL=9 or 10 (trddata_en =3)                                                                                 |         |            |
|       |                                                                                                                       |         |            |
|       | Renesas DDR PHY:                                                                                                      |         |            |
|       |                                                                                                                       |         |            |
|       | DDR3:                                                                                                                 |         |            |
|       |                                                                                                                       |         |            |
|       | **trddata_en** : (RL-2)/2 and ignore floating value                                                                   |         |            |
|       |                                                                                                                       |         |            |
|       | ex. 5'd3 : RL=8 or 9 (trddata_en =3)                                                                                  |         |            |
|       |                                                                                                                       |         |            |
|       | LPDDR2:                                                                                                               |         |            |
|       |                                                                                                                       |         |            |
|       | **trddata_en** : (RL-1)/2 and ignore floating value                                                                   |         |            |
|       |                                                                                                                       |         |            |
|       | ex. 5'd3 : RL=7 or 8 (trddata_en =3)                                                                                  |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:2   | Reserved                                                                                                              | 0b      | R          |
+-------+-----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1     | **WL: Write latency**                                                                                                 | 1b      | R          |
|       |                                                                                                                       |         |            |
|       | 0: even WL                                                                                                            |         |            |
|       |                                                                                                                       |         |            |
|       | 1: odd WL                                                                                                             |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0     | **RL: Read latency**                                                                                                  | 1b      | R          |
|       |                                                                                                                       |         |            |
|       | 0: even RL                                                                                                            |         |            |
|       |                                                                                                                       |         |            |
|       | 1: odd RL                                                                                                             |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3\_ BIST_TRIGGER register (Offset 024h)

Default Value: 0000_0002h

+-------+---------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                | Default | Attributes |
+:=====:+=============================================================================================+:=======:+:==========:+
| 31:25 | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 24    | **clear_BIST1**                                                                             | 0b      | RW1        |
|       |                                                                                             |         |            |
|       | 0: Reset BIST is done or no reset BIST happens                                              |         |            |
|       |                                                                                             |         |            |
|       | 1: BIST1 setting reset, it return 0 after 512 clock                                         |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 23:21 | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 20    | **BIST_SET_ERROR**                                                                          | 0b      | R          |
|       |                                                                                             |         |            |
|       | 0: No Fail                                                                                  |         |            |
|       |                                                                                             |         |            |
|       | 1: BIST setting violation causes BIST Fail                                                  |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 19:18 | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 17:16 | **BIST_MODE**                                                                               | 0b      | RW         |
|       |                                                                                             |         |            |
|       | 0: DRAM Size test mode (the shortest time)                                                  |         |            |
|       |                                                                                             |         |            |
|       | 1: Squential address BIST mode                                                              |         |            |
|       |                                                                                             |         |            |
|       | 2: Random address BIST mode                                                                 |         |            |
|       |                                                                                             |         |            |
|       | 3: Add constant number for next start address BIST mode depending BIST_NUM_NXT_STADDR value |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 15:9  | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 8     | **BIST_FAIL**                                                                               | 0b      | R          |
|       |                                                                                             |         |            |
|       | 0: No Fail                                                                                  |         |            |
|       |                                                                                             |         |            |
|       | 1: Fail                                                                                     |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 7:2   | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 1     | **Shift_4MB**                                                                               | 1b      | RW         |
|       |                                                                                             |         |            |
|       | 0: No shift BIST TEST Address                                                               |         |            |
|       |                                                                                             |         |            |
|       | 1: Shift BIST TEST Address 16MB                                                             |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 0     | **BIST_TRIGGER**                                                                            | 0b      | RW1        |
|       |                                                                                             |         |            |
|       | It turn 0 after BIST is done or assign clear_BIST1=1                                        |         |            |
|       |                                                                                             |         |            |
|       | 0: No Enable or BIST Test Done                                                              |         |            |
|       |                                                                                             |         |            |
|       | 1: Enable BIST Test                                                                         |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+

##### DDR3\_ PHY_CFG & Controller reset & reservice stop register (Offset 028h)

Default Value: 0040_0201h

+-------+--------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                           | Default | Attributes |
+:=====:+========================================================+:=======:+:==========:+
| 31:30 | **DFIWR_DISABLE:**                                     | 0b      | RW         |
|       |                                                        |         |            |
|       | 00: No Write command in DFI even or odd is disable.    |         |            |
|       |                                                        |         |            |
|       | 01: Write command in DFI even is disable.              |         |            |
|       |                                                        |         |            |
|       | 10: Write command in DFI odd is disable.               |         |            |
|       |                                                        |         |            |
|       | 11: It is setting prohibited                           |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 29:28 | **DFIRD_DISABLE:**                                     | 0b      | RW         |
|       |                                                        |         |            |
|       | 00: No Read command in DFI even or odd is disable.     |         |            |
|       |                                                        |         |            |
|       | 01: Read command in DFI even is disable.               |         |            |
|       |                                                        |         |            |
|       | 10: Read command in DFI odd is disable.                |         |            |
|       |                                                        |         |            |
|       | 11: It is setting prohibited                           |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 27:24 | **DFI_RD_CONSTAINT:**                                  | 0b      | RW         |
|       |                                                        |         |            |
|       | 000: No seem DFI read command constaint                |         |            |
|       |                                                        |         |            |
|       | 001: Seem DFI read command at least 2 cycle            |         |            |
|       |                                                        |         |            |
|       | 010: Seem DFI read command at least 3 cycle            |         |            |
|       |                                                        |         |            |
|       | .                                                      |         |            |
|       |                                                        |         |            |
|       | .                                                      |         |            |
|       |                                                        |         |            |
|       | 111: Seem DFI read command at least 8 cycle            |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 24    | **phycnt_odd_lch:**                                    | 0b      | RW         |
|       |                                                        |         |            |
|       | 0: DQ IN is latched when PHY counter number is even.   |         |            |
|       |                                                        |         |            |
|       | 1: DQ IN is latched when PHY counter number is odd.    |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 23    | **DFIADDR_KP_N:**                                      | 0b      | RW         |
|       |                                                        |         |            |
|       | 0: DRAM address keeps last address in NOP command      |         |            |
|       |                                                        |         |            |
|       | 1: DRAM address don't keep last address in NOP command |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 22:21 | **BL:**                                                | 2b      | RW         |
|       |                                                        |         |            |
|       | 0: It is setting prohibited                            |         |            |
|       |                                                        |         |            |
|       | 1: Burth Length 4                                      |         |            |
|       |                                                        |         |            |
|       | 2: Burth Length 8                                      |         |            |
|       |                                                        |         |            |
|       | 3: It is setting prohibited                            |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 20    | **X32:**                                               | 0b      | RW         |
|       |                                                        |         |            |
|       | 0: x16                                                 |         |            |
|       |                                                        |         |            |
|       | 1: x32                                                 |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 19:17 | **rddata_latency:**                                    | 0b      | RW         |
|       |                                                        |         |            |
|       | 0: No delay                                            |         |            |
|       |                                                        |         |            |
|       | 1: Delay 1 cycle                                       |         |            |
|       |                                                        |         |            |
|       | 2: Delay 2 cycle                                       |         |            |
|       |                                                        |         |            |
|       | 3: Delay 3 cycle                                       |         |            |
|       |                                                        |         |            |
|       | 4: Delay 4 cycle                                       |         |            |
|       |                                                        |         |            |
|       | 5: Delay 5 cycle                                       |         |            |
|       |                                                        |         |            |
|       | 6: Delay 6 cycle                                       |         |            |
|       |                                                        |         |            |
|       | 7: Delay 7 cycle                                       |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 16    | **dqin32_neg:**                                        | 0b      | RW         |
|       |                                                        |         |            |
|       | 0: DQ IN latch by positive edge of internal clock      |         |            |
|       |                                                        |         |            |
|       | 1: DQ IN latch by negative edge of internal clock      |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 15:14 | Reserved                                               | 0b      | R          |
+-------+--------------------------------------------------------+---------+------------+
| 13    | **DFIPHYUPD_ENB:**                                     | 0b      | RW         |
|       |                                                        |         |            |
|       | 0: Enable DFIPHYUPDREQ                                 |         |            |
|       |                                                        |         |            |
|       | 1: Mask DFIPHYUPDREQ                                   |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 12    | **phycnt_reset:**                                      | 0b      | RW         |
|       |                                                        |         |            |
|       | 0: Exit reset counter of DFIRDDATAVALID                |         |            |
|       |                                                        |         |            |
|       | 1: reset counter of DFIRDDATAVALID                     |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 11:10 | Reserved                                               | 0b      | R          |
+-------+--------------------------------------------------------+---------+------------+
| 9     | **DDR_CTRL_EMPTY:**                                    | 1b      | R          |
|       |                                                        |         |            |
|       | 0: Some service are in DDR Controller                  |         |            |
|       |                                                        |         |            |
|       | 1: No any service is in DDR Controller                 |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 8     | **DDR_SV_STOP:**                                       | 0b      | RW         |
|       |                                                        |         |            |
|       | 0: Continue DRAM service                               |         |            |
|       |                                                        |         |            |
|       | 1: Stop DRAM service                                   |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 7:5   | Reserved                                               | 0b      | R          |
+-------+--------------------------------------------------------+---------+------------+
| 4     | **SW_RESET: Reset DRAM Controller**                    | 0b      | RW         |
|       |                                                        |         |            |
|       | 0: Exit DRAM controller software reset                 |         |            |
|       |                                                        |         |            |
|       | 1: Entry DRAM controller software reset                |         |            |
+-------+--------------------------------------------------------+---------+------------+
| 3:0   | Reserved                                               | 1b      | R          |
+-------+--------------------------------------------------------+---------+------------+

Note;when before entering self-refresh or DPD mode, or before enable/disable ECC, it need to wait until **DDR_CTRL_EMPTY** = 1

##### LPDDR2\_ MRR_DATA register (Offset 02Ch)

Default Value: 0000_0000h

+-------+--------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                             | Default | Attributes |
+:=====:+==========================================================================+:=======:+:==========:+
| 31:17 | Reserved                                                                 | 0b      | R          |
+-------+--------------------------------------------------------------------------+---------+------------+
| 16    | **MRR \_VALID**                                                          | 0b      | R          |
|       |                                                                          |         |            |
|       | 0: No new MMR value is in this register or last MMR value has been read. |         |            |
|       |                                                                          |         |            |
|       | 1: There are a new MMR value not be read yet.                            |         |            |
+-------+--------------------------------------------------------------------------+---------+------------+
| 15:8  | Reserved                                                                 | 0b      | R          |
+-------+--------------------------------------------------------------------------+---------+------------+
| 7:0   | **MRR_DIN**: Mode Register Read Value                                    | 0b      | R          |
+-------+--------------------------------------------------------------------------+---------+------------+

##### DDR3\_ BIST_START_ADDR register (Offset 058h)

Default Value: 0000_0000h

+------+-----------------------------------------+---------+------------+
| Bit  | Descriptions                            | Default | Attributes |
+:====:+=========================================+:=======:+:==========:+
| 31:3 | **BIST_START_ADDR**                     | 0b      | RW         |
|      |                                         |         |            |
|      | BIST Start address of BIST_MODE=1 or =2 |         |            |
|      |                                         |         |            |
|      | (unit: 4 words)                         |         |            |
+------+-----------------------------------------+---------+------------+
| 2:0  | Reserved                                | 0b      | R          |
+------+-----------------------------------------+---------+------------+

##### DDR3\_ BIST_END_ADDR register (Offset 05Ch)

Default Value: 03ff_fff0h

+------+---------------------------------------+-----------+------------+
| Bit  | Descriptions                          | Default   | Attributes |
+:====:+=======================================+:=========:+:==========:+
| 31:3 | **BIST_END_ADDR**                     | 3ff_fff0b | RW         |
|      |                                       |           |            |
|      | BIST End address of BIST_MODE=1 or =2 |           |            |
|      |                                       |           |            |
|      | (unit: 4 words)                       |           |            |
+------+---------------------------------------+-----------+------------+
| 2:0  | Reserved                              | 0b        | R          |
+------+---------------------------------------+-----------+------------+

##### DDR3\_ BIST_RD_START_TIME register (Offset 060h)

Default Value: 0200_0000h

+------+--------------------------------------------------------------------------+-----------+------------+
| Bit  | Descriptions                                                             | Default   | Attributes |
+:====:+==========================================================================+:=========:+:==========:+
| 31:3 | **BIST_RD_START_WRPOINT**                                                | 200_0000b | RW         |
|      |                                                                          |           |            |
|      | BIST start reading when BIST write address at the value of this register |           |            |
|      |                                                                          |           |            |
|      | (unit: 4 words)                                                          |           |            |
+------+--------------------------------------------------------------------------+-----------+------------+
| 2:0  | Reserved                                                                 | 0b        | R          |
+------+--------------------------------------------------------------------------+-----------+------------+

##### DDR3\_ BIST_START_DATA register (Offset 064h)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **BIST \_START_DATA**                 | 0b      | RW         |
|      |                                       |         |            |
|      | BIST start access data                |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3\_ BIST_TOG_DATA register (Offset 068h)

Default Value: ffff_ffffh

+-----+-------------------------------------------------------------------+---------+------------+
| Bit | Descriptions                                                      | Default | Attributes |
+:===:+===================================================================+:=======:+:==========:+
| 31  | **BIST \_TOG_DATAPIN\[31\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ31 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ31 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 30  | **BIST \_TOG_DATAPIN\[30\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ30 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ30 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 29  | **BIST \_TOG_DATAPIN\[29\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ29 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ29 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 28  | **BIST \_TOG_DATAPIN\[28\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ28 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ28 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 27  | **BIST \_TOG_DATAPIN\[27\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ27 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ27 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 26  | **BIST \_TOG_DATAPIN\[26\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ26 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ26 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 25  | **BIST \_TOG_DATAPIN\[25\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ25 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ25 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 24  | **BIST \_TOG_DATAPIN\[24\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ24 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ24 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 23  | **BIST \_TOG_DATAPIN\[23\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ23 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ23 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 22  | **BIST \_TOG_DATAPIN\[22\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ22 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ22 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 21  | **BIST \_TOG_DATAPIN\[21\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ21 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ21 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 20  | **BIST \_TOG_DATAPIN\[20\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ20 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ20 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 19  | **BIST \_TOG_DATAPIN\[19\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ19 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ19 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 18  | **BIST \_TOG_DATAPIN\[18\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ18 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ18 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 17  | **BIST \_TOG_DATAPIN\[17\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ17 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ17 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 16  | **BIST \_TOG_DATAPIN\[16\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ16 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ16 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 15  | **BIST \_TOG_DATAPIN\[15\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ15 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ15 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 14  | **BIST \_TOG_DATAPIN\[14\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ14 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ14 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 13  | **BIST \_TOG_DATAPIN\[13\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ13 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ13 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 12  | **BIST \_TOG_DATAPIN\[12\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ12 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ12 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 11  | **BIST \_TOG_DATAPIN\[11\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ11 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ11 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 10  | **BIST \_TOG_DATAPIN\[10\]**                                      | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ10 will always keep value depend on BIST_START_DATA |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ10 will change next time.                           |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 9   | **BIST \_TOG_DATAPIN\[9\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ9 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ9 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 8   | **BIST \_TOG_DATAPIN\[8\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ8 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ8 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 7   | **BIST \_TOG_DATAPIN\[7\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ7 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ7 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 6   | **BIST \_TOG_DATAPIN\[6\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ6 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ6 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 5   | **BIST \_TOG_DATAPIN\[5\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ5 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ5 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 4   | **BIST \_TOG_DATAPIN\[4\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ4 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ4 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 3   | **BIST \_TOG_DATAPIN\[3\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ3 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ3 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 2   | **BIST \_TOG_DATAPIN\[2\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ2 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ2 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 1   | **BIST \_TOG_DATAPIN\[1\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ1 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ1 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+
| 0   | **BIST \_TOG_DATAPIN\[0\]**                                       | 1b      | RW         |
|     |                                                                   |         |            |
|     | 0: DRAM PIN DQ0 will always keep value depend on BIST_START_DATA  |         |            |
|     |                                                                   |         |            |
|     | 1: DRAM PIN DQ0 will change next time.                            |         |            |
+-----+-------------------------------------------------------------------+---------+------------+

##### DDR3\_ BIST_CFG register (Offset 06Ch)

Default Value: 0140_00f1h

+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                                              | Default | Attributes |
+:=====:+===========================================================================================================================================================================+:=======:+:==========:+
| 31:30 | Reserved                                                                                                                                                                  | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 29:28 | **BIST_WRAP**                                                                                                                                                             | 0b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0: BURST Type is only INCR                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: BUSRT Type is only WRAP                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: BUSRT Type is INCR or WRAP                                                                                                                                             |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3: BUSRT Type is INCR or WRAP                                                                                                                                             |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27:25 | **ECC_LEN_TP**                                                                                                                                                            | 0b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0: Keep Length setting by **BIST_LEN_FIX_EN** and **BIST_LEN**                                                                                                            |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: ECC LEN=0,3,15,63 random happen                                                                                                                                        |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: ECC LEN=3,15,63 random happen                                                                                                                                          |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3: Only LEN=64 happen                                                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 4: Only ECC LEN=63 happen                                                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 5: ECC LEN=0, 3 happen                                                                                                                                                    |         |            |
|       |                                                                                                                                                                           |         |            |
|       | others: not support                                                                                                                                                       |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 24    | **RW_PERIOD_ADJUST**                                                                                                                                                      | 1b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0: no read/write period adjust                                                                                                                                            |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: If write overflow read next cycle, write will pause. And potentially **BIST_WR_DLY** will add 1 in every 32 times write pause happens.                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | Or if read overflow write next cycle, read will pause. And potentially **BIST_RD_DLY** will add 1 in every 32 times read pause happens.                                   |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:20 | **BIST_NUM_NXT_STADDR**                                                                                                                                                   | 100b    | RW         |
|       |                                                                                                                                                                           |         |            |
|       | **IF BIST_MODE=2, value of BIST_NUM_NXT_STADDR is generated by Random generator.**                                                                                        |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 0: Next address is last end address plus 0x8 (unit:Byte)                                                                                                                  |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: Next address is last end address plus 0x10 (unit:Byte)                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: Next address is last end address plus 0x20 (unit:Byte)                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3: Next address is last end address plus 0x40 (unit:Byte)                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 4: Next address is last end address plus 0x80 (unit:Byte)                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | **.**                                                                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 10: Next address is last end address plus 0x2000 (unit:Byte)                                                                                                              |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 11: Next address is last end address plus 0x4000 (unit:Byte)                                                                                                              |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 12\~15: Next address is last end address plus 0x800 (unit:Byte)                                                                                                           |         |            |
|       |                                                                                                                                                                           |         |            |
|       | **IF BIST_MODE=3,**                                                                                                                                                       |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 0: Next address is last start address plus 0x8 (unit:Byte)                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: Next address is last start address plus 0x10 (unit:Byte)                                                                                                               |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: Next address is last start address plus 0x20 (unit:Byte)                                                                                                               |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3: Next address is last start address plus 0x40 (unit:Byte)                                                                                                               |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 4: Next address is last start address plus 0x80 (unit:Byte)                                                                                                               |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | **.**                                                                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 15: Next address is last start address plus 0x40000 (unit:Byte)                                                                                                           |         |            |
|       |                                                                                                                                                                           |         |            |
|       | **Note: In BIST_MODE=3 if BIST_NUM_NXT_STADDR setting value is smarter BIST length to let BIST write twice or read twice in the same memory address, it will be failed.** |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 19    | Reserved                                                                                                                                                                  | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 18:12 | **cnt_wcycle**                                                                                                                                                            | 0b      | R          |
|       |                                                                                                                                                                           |         |            |
|       | 0: BIST WR is at 1st cycle                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: BIST WR is at 2nd cycle                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: BIST WR is at 3rd cycle                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 127: BIST WR is at 127th cycle                                                                                                                                            |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 11    | Reserved                                                                                                                                                                  | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 10:8  | **BIST_CYCLE**                                                                                                                                                            | 0b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0:BIST 1 cycle from BIST_START_ADDR to BIST_END_ADDR                                                                                                                      |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1:BIST 2 cycles from BIST_START_ADDR to BIST_END_ADDR                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2:BIST 4 cycles from BIST_START_ADDR to BIST_END_ADDR                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3:BIST 8 cycles from BIST_START_ADDR to BIST_END_ADDR                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 7:BIST 128 cycles from BIST_START_ADDR to BIST_END_ADDR                                                                                                                   |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4   | **BIST_LEN**                                                                                                                                                              | fh      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | BIST fixed Length                                                                                                                                                         |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3     | Reserved                                                                                                                                                                  | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2:1   | **BIST_DATAMODE**                                                                                                                                                         | 0b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0:Sequential data mode                                                                                                                                                    |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1:Random data mode                                                                                                                                                        |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2:Toggle data mode                                                                                                                                                        |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3:Not support                                                                                                                                                             |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0     | **BIST_LEN_FIX_EN**                                                                                                                                                       | 1b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0:BIST Length Random                                                                                                                                                      |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1:BIST Length Fixed depending on BIST_LEN                                                                                                                                 |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3\_ BIST_DLY register (Offset 070h)

Default Value: 0000_0000h

+-------+------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                         | Default | Attributes |
+:=====:+======================================================+:=======:+:==========:+
| 31:17 | **BIST_RD_DLY**                                      | 0b      | RW         |
|       |                                                      |         |            |
|       | Delay cycle number next read                         |         |            |
+-------+------------------------------------------------------+---------+------------+
| 16    | **BIST_RD_RDM_DLY_EN**                               | 0b      | RW         |
|       |                                                      |         |            |
|       | 0:Delay n cycles next read depending on BIST_RD_DLY  |         |            |
|       |                                                      |         |            |
|       | 1:Delay random cycles next read                      |         |            |
+-------+------------------------------------------------------+---------+------------+
| 15:1  | **BIST_WR_DLY**                                      | 0b      | RW         |
|       |                                                      |         |            |
|       | Delay cycle number next write                        |         |            |
+-------+------------------------------------------------------+---------+------------+
| 0     | **BIST_WR_RDM_DLY_EN**                               | 0b      | RW         |
|       |                                                      |         |            |
|       | 0:Delay n cycles next write depending on BIST_WR_DLY |         |            |
|       |                                                      |         |            |
|       | 1:Delay random cycles next write                     |         |            |
+-------+------------------------------------------------------+---------+------------+

##### SDR_SDLL_control register (Offset 07Ch)

Default Value: 0000_0000h

+-------+-----------------------------------------------+---------+------------+
| Bit   | Descriptions                                  | Default | Attributes |
+:=====:+===============================================+:=======:+:==========:+
| 31    | **by_pass**                                   | 0b      | RW         |
|       |                                               |         |            |
|       | 0:normal                                      |         |            |
|       |                                               |         |            |
|       | 1:by pass the SDLL                            |         |            |
+-------+-----------------------------------------------+---------+------------+
| 30    | **RDDATA_fall_latch**                         | 0b      | RW         |
|       |                                               |         |            |
|       | 0:SDR DQ latch by rising edge by R_CLK        |         |            |
|       |                                               |         |            |
|       | 1:SDR DQ latch by falling edge by R_CLK       |         |            |
+-------+-----------------------------------------------+---------+------------+
| 29    | Reserved                                      | 0b      | R          |
+-------+-----------------------------------------------+---------+------------+
| 28    | **SDR_SDLL_CFG_R,adjust for SDLL**            | 1b      | RW         |
+-------+-----------------------------------------------+---------+------------+
| 27:25 | Reserved                                      | 000b    | R          |
+-------+-----------------------------------------------+---------+------------+
| 24    | **SDR_SDLL_EN_EXTRA,adjust for SDLL**         | 0b      | RW         |
+-------+-----------------------------------------------+---------+------------+
| 23    | **EN_SDLL**                                   | 1b      | RW         |
|       |                                               |         |            |
|       | 0:disable the SDLL for SDR                    |         |            |
|       |                                               |         |            |
|       | 1:enable the SDLL for SDR                     |         |            |
+-------+-----------------------------------------------+---------+------------+
| 22:16 | **SDR_SDLL_SEL\[8:2\],adjust delay for SDLL** | 0h      | RW         |
+-------+-----------------------------------------------+---------+------------+
| 15:9  | Reserved                                      | 0h      | R          |
+-------+-----------------------------------------------+---------+------------+
| 8     | **SDR_SDLL_SEL\[1\] ,adjust delay for SDLL**  | 0b      | RW         |
+-------+-----------------------------------------------+---------+------------+
| 7:1   | Reserved                                      | 0h      | R          |
+-------+-----------------------------------------------+---------+------------+
| 0     | **SDR_SDLL_SEL\[0\] ,adjust delay for SDLL**  | 0b      | RW         |
+-------+-----------------------------------------------+---------+------------+

##### DDR3_ECC_CTRL register (Offset 080h)

Default Value: 0000_0000h

+-----+---------------------------------------+---------+------------+
| Bit | Descriptions                          | Default | Attributes |
+:===:+=======================================+:=======:+:==========:+
| 31  | **Z3_UN_CORRECT_CLR_1**               | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z3_UN_CORRECT_NUMBER**              |         |            |
+-----+---------------------------------------+---------+------------+
| 30  | **Z3_ECC_ERROR_CLR_1**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z3_ECC_ERROR_NUMBER**               |         |            |
+-----+---------------------------------------+---------+------------+
| 29  | **Z3_CRC_ERROR_CLR_1**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z3_CRC_ERROR_NUMBER**               |         |            |
+-----+---------------------------------------+---------+------------+
| 28  | **Z2_UN_CORRECT_CLR_1**               | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z2_UN_CORRECT_NUMBER**              |         |            |
+-----+---------------------------------------+---------+------------+
| 27  | **Z2_ECC_ERROR_CLR_1**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z2_ECC_ERROR_NUMBER**               |         |            |
+-----+---------------------------------------+---------+------------+
| 26  | **Z2_CRC_ERROR_CLR_1**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z2_CRC_ERROR_NUMBER**               |         |            |
+-----+---------------------------------------+---------+------------+
| 25  | **Z1_UN_CORRECT_CLR_1**               | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z1_UN_CORRECT_NUMBER**              |         |            |
+-----+---------------------------------------+---------+------------+
| 24  | **Z1_ECC_ERROR_CLR_1**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z1_ECC_ERROR_NUMBER**               |         |            |
+-----+---------------------------------------+---------+------------+
| 23  | **Z3_UN_CORRECT_CLR_0**               | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z3_UN_CORRECT_INT**                 |         |            |
|     |                                       |         |            |
|     | **Z3_UN_CORRECT_ADDR**                |         |            |
|     |                                       |         |            |
|     | **Z3_UN_CORRECT_ARID**                |         |            |
+-----+---------------------------------------+---------+------------+
| 22  | **Z3_ECC_ERROR_CLR_0**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z3_ECC_ERROR_INT**                  |         |            |
|     |                                       |         |            |
|     | **Z3_ECC_ERROR_ADDR**                 |         |            |
|     |                                       |         |            |
|     | **Z3_ECC_ERROR_ARID**                 |         |            |
+-----+---------------------------------------+---------+------------+
| 21  | **Z3_CRC_ERROR_CLR_0**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z3_CRC_ERROR_INT**                  |         |            |
|     |                                       |         |            |
|     | **Z3_CRC_ERROR_ADDR**                 |         |            |
|     |                                       |         |            |
|     | **Z3_CRC_ERROR_ARID**                 |         |            |
+-----+---------------------------------------+---------+------------+
| 20  | **Z2_UN_CORRECT_CLR_0**               | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z2_UN_CORRECT_INT**                 |         |            |
|     |                                       |         |            |
|     | **Z2_UN_CORRECT_ADDR**                |         |            |
|     |                                       |         |            |
|     | **Z2_UN_CORRECT_ARID**                |         |            |
+-----+---------------------------------------+---------+------------+
| 19  | **Z2_ECC_ERROR_CLR_0**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z2_ECC_ERROR_INT**                  |         |            |
|     |                                       |         |            |
|     | **Z2_ECC_ERROR_ADDR**                 |         |            |
|     |                                       |         |            |
|     | **Z2_ECC_ERROR_ARID**                 |         |            |
+-----+---------------------------------------+---------+------------+
| 18  | **Z2_CRC_ERROR_CLR_0**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z2_CRC_ERROR_INT**                  |         |            |
|     |                                       |         |            |
|     | **Z2_CRC_ERROR_ADDR**                 |         |            |
|     |                                       |         |            |
|     | **Z2_CRC_ERROR_ARID**                 |         |            |
+-----+---------------------------------------+---------+------------+
| 17  | **Z1_UN_CORRECT_CLR_0**               | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z1_UN_CORRECT_INT**                 |         |            |
|     |                                       |         |            |
|     | **Z1_UN_CORRECT_ADDR**                |         |            |
|     |                                       |         |            |
|     | **Z1_UN_CORRECT_ARID**                |         |            |
+-----+---------------------------------------+---------+------------+
| 16  | **Z1_ECC_ERROR_CLR_0**                | 0b      | RW         |
|     |                                       |         |            |
|     | Will clear :                          |         |            |
|     |                                       |         |            |
|     | **Z1_ECC_ERROR_INT**                  |         |            |
|     |                                       |         |            |
|     | **Z1_ECC_ERROR_ADDR**                 |         |            |
|     |                                       |         |            |
|     | **Z1_ECC_ERROR_ARID**                 |         |            |
+-----+---------------------------------------+---------+------------+
| 15  | **Z3_UN_CORRECT_INT_EN**              | 0b      | RW         |
+-----+---------------------------------------+---------+------------+
| 14  | **Z3_ECC_ERROR_INT_EN**               | 0b      | RW         |
+-----+---------------------------------------+---------+------------+
| 13  | **Z3_CRC_ERROR_INT_EN**               | 0b      | RW         |
+-----+---------------------------------------+---------+------------+
| 12  | **Z2_UN_CORRECT_INT_EN**              | 0b      | RW         |
+-----+---------------------------------------+---------+------------+
| 11  | **Z2_ECC_ERROR_INT_EN**               | 0b      | RW         |
+-----+---------------------------------------+---------+------------+
| 10  | **Z2_CRC_ERROR_INT_EN**               | 0b      | RW         |
+-----+---------------------------------------+---------+------------+
| 9   | **Z1_UN_CORRECT_INT_EN**              | 0b      | RW         |
+-----+---------------------------------------+---------+------------+
| 8   | **Z1_ECC_ERROR_INT_EN**               | 0b      | RW         |
+-----+---------------------------------------+---------+------------+
| 7   | **Z3_UN_CORRECT_INT**                 | 0b      | R          |
+-----+---------------------------------------+---------+------------+
| 6   | **Z3_ECC_ERROR_INT**                  | 0b      | R          |
+-----+---------------------------------------+---------+------------+
| 5   | **Z3_CRC_ERROR_INT**                  | 0b      | R          |
+-----+---------------------------------------+---------+------------+
| 4   | **Z2_UN_CORRECT_INT**                 | 0b      | R          |
+-----+---------------------------------------+---------+------------+
| 3   | **Z2_ECC_ERROR_INT**                  | 0b      | R          |
+-----+---------------------------------------+---------+------------+
| 2   | **Z2_CRC_ERROR_INT**                  | 0b      | R          |
+-----+---------------------------------------+---------+------------+
| 1   | **Z1_UN_CORRECT_INT**                 | 0b      | R          |
+-----+---------------------------------------+---------+------------+
| 0   | **Z1_ECC_ERROR_INT**                  | 0b      | R          |
+-----+---------------------------------------+---------+------------+

##### DDR3_Z1_ECC_ERROR_ADDR register (Offset 084h)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **Z1_ECC_ERROR_ADDR**                 | 0b      | R          |
|      |                                       |         |            |
|      | **(Unit : Byte)**                     |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3_Z1_ECC_ERROR_NUMBER register (Offset 088h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z1_ECC_ERROR_NUMBER**                     0b          R

  ----------------------------------------------------------------------

##### DDR3_Z1_ECC_ERROR_ARID register (Offset 08Ch)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z1_ECC_ERROR_ARID**                       0b          R

  ----------------------------------------------------------------------

##### DDR3_Z1_ECC_UN_CORRECT_ADDR register (Offset 090h)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **Z1_ECC_UN_CORRECT_ADDR**            | 0b      | R          |
|      |                                       |         |            |
|      | **(Unit : Byte)**                     |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3_Z1_ECC_UN_CORRECT_NUMBER register (Offset 094h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z1_ECC_UN_CORRECT_NUMBER**                0b          R

  ----------------------------------------------------------------------

##### DDR3_Z1_ECC_UN_CORRECT_ARID register (Offset 098h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z1_ECC_UN_CORRECT_ARID**                  0b          R

  ----------------------------------------------------------------------

##### DDR3_Z2_CRC_ERROR_ADDR register (Offset 09Ch)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **Z2_ECC_ERROR_ADDR**                 | 0b      | R          |
|      |                                       |         |            |
|      | **(Unit : Byte)**                     |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3_Z2_CRC_ERROR_NUMBER register (Offset 0A0h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z2_CRC_ERROR_NUMBER**                     0b          R

  ----------------------------------------------------------------------

##### DDR3_Z2_CRC_ERROR_ARID register (Offset 0A4h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z2_CRC_ERROR_ARID**                       0b          R

  ----------------------------------------------------------------------

##### DDR3_Z2_ECC_ERROR_ADDR register (Offset 0A8h)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **Z2_ECC_ERROR_ADDR**                 | 0b      | R          |
|      |                                       |         |            |
|      | **(Unit : Byte)**                     |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3_Z2_ECC_ERROR_NUMBER register (Offset 0ACh)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z2_ECC_ERROR_NUMBER**                     0b          R

  ----------------------------------------------------------------------

##### DDR3_Z2_ECC_ERROR_ARID register (Offset 0B0h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z2_ECC_ERROR_ARID**                       0b          R

  ----------------------------------------------------------------------

##### DDR3_Z2_ECC_UN_CORRECT_ADDR register (Offset 0B4h)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **Z2_ECC_UN_CORRECT_ADDR**            | 0b      | R          |
|      |                                       |         |            |
|      | **(Unit : Byte)**                     |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3_Z2_ECC_UN_CORRECT_NUMBER register (Offset 0B8h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z2_ECC_UN_CORRECT_NUMBER**                0b          R

  ----------------------------------------------------------------------

##### DDR3_Z2_ECC_UN_CORRECT_ARID register (Offset 0BCh)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z2_ECC_UN_CORRECT_ARID**                  0b          R

  ----------------------------------------------------------------------

##### DDR3_Z3_CRC_ERROR_ADDR register (Offset 0C0h)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **Z3_ECC_ERROR_ADDR**                 | 0b      | R          |
|      |                                       |         |            |
|      | **(Unit : Byte)**                     |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3_Z3_CRC_ERROR_NUMBER register (Offset 0C4h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z3_CRC_ERROR_NUMBER**                     0b          R

  ----------------------------------------------------------------------

##### DDR3_Z3_CRC_ERROR_ARID register (Offset 0C8h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z3_CRC_ERROR_ARID**                       0b          R

  ----------------------------------------------------------------------

##### DDR3_Z3_ECC_ERROR_ADDR register (Offset 0CCh)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **Z3_ECC_ERROR_ADDR**                 | 0b      | R          |
|      |                                       |         |            |
|      | **(Unit : Byte)**                     |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3_Z3_ECC_ERROR_NUMBER register (Offset 0D0h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z3_ECC_ERROR_NUMBER**                     0b          R

  ----------------------------------------------------------------------

##### DDR3_Z3_ECC_ERROR_ARID register (Offset 0D4h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z3_ECC_ERROR_ARID**                       0b          R

  ----------------------------------------------------------------------

##### DDR3_Z3_ECC_UN_CORRECT_ADDR register (Offset 0D8h)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **Z3_ECC_UN_CORRECT_ADDR**            | 0b      | R          |
|      |                                       |         |            |
|      | **(Unit : Byte)**                     |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3_Z3_ECC_UN_CORRECT_NUMBER register (Offset 0DCh)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z3_ECC_UN_CORRECT_NUMBER**                0b          R

  ----------------------------------------------------------------------

##### DDR3_Z3_ECC_UN_CORRECT_ARID register (Offset 0E0h)

Default Value: 0000_0000h

  ----------------------------------------------------------------------
   Bit   Descriptions                              Default   Attributes
  ------ ---------------------------------------- --------- ------------
   31:0  **Z3_ECC_UN_CORRECT_ARID**                  0b          R

  ----------------------------------------------------------------------

##### DDR3_ZONE_STATUS register (Offset 0E4h)

Default Value: 0000_0000h

+-------+---------------------------------------+---------+------------+
| Bit   | Descriptions                          | Default | Attributes |
+:=====:+=======================================+:=======:+:==========:+
| 31:17 | **Reserved**                          | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 16    | **ZONE_INVALID_CLR**                  | 0b      | RW         |
|       |                                       |         |            |
|       | Will clear :                          |         |            |
|       |                                       |         |            |
|       | **Z1_WRITE_INVALID_INT**              |         |            |
|       |                                       |         |            |
|       | **Z1_READ_INVLAID_INT**               |         |            |
|       |                                       |         |            |
|       | **Z2_WRITE_INVALID_INT**              |         |            |
|       |                                       |         |            |
|       | **Z2_READ_INVLAID_INT**               |         |            |
|       |                                       |         |            |
|       | **Z3_WRITE_INVALID_INT**              |         |            |
|       |                                       |         |            |
|       | **Z3_READ_INVLAID_INT**               |         |            |
+-------+---------------------------------------+---------+------------+
| 15    | **Reserved**                          | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 14    | **Reserved**                          | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 13    | **Z3_READ_INVALID_INT_EN**            | 0b      | RW         |
+-------+---------------------------------------+---------+------------+
| 12    | **Z3_WRITE_INVALID_INT_EN**           | 0b      | RW         |
+-------+---------------------------------------+---------+------------+
| 11    | **Z2_READ_INVALID_INT_EN**            | 0b      | RW         |
+-------+---------------------------------------+---------+------------+
| 10    | **Z2_WRITE_INVALID_INT_EN**           | 0b      | RW         |
+-------+---------------------------------------+---------+------------+
| 9     | **Z1_READ_INVALID_INT_EN**            | 0b      | RW         |
+-------+---------------------------------------+---------+------------+
| 8     | **Z1_WRITE_INVALID_INT_EN**           | 0b      | RW         |
+-------+---------------------------------------+---------+------------+
| 7     | **Reserved**                          | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 6     | **Reserved**                          | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 5     | **Z3_READ_INVALID_INT**               | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 4     | **Z3_WRITE_INVALID_INT**              | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 3     | **Z2_READ_INVALID_INT**               | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 2     | **Z2_WRITE_INVALID_INT**              | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 1     | **Z1_READ_INVALID_INT**               | 0b      | R          |
+-------+---------------------------------------+---------+------------+
| 0     | **Z1_WRITE_INVALID_INT**              | 0b      | R          |
+-------+---------------------------------------+---------+------------+

Zone usage limitation:

1.  each Zone length can't be zero.

2.  The start address for each must be Z1 \< Z2 \<Z3

3.  Don't allow the empty sapce between each Zone. Each Zone space must be continuous.

4.  Only support all Zone ECC enable or disable.

![](D:\AI_DEV\Output\PS3111_RegSpec_DDR_assets/media/image10.emf)

Note:

1.  There could be gap (empty space) existed between Z1/Z2, Z2/Z3.

2.  Usage limitation : In general, the gap is not allowed to write, but for some special case, if FW write the gap once, it should write valid space once and then DDR can be written normally.

##### DDR3_ROUND_ROBIN_WATC register (Offset 0E8h)

Default Value: 0000_0000h

+-------+--------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                         | Default | Attributes |
+:=====:+======================================================================================+:=======:+:==========:+
| 31    | REG_ROBIN_EN, round robin enable                                                     | 1b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 30:26 | Reserved                                                                             | 0b      | R          |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 25:16 | REG_WAITC_RD:                                                                        | A0h     | RW         |
|       |                                                                                      |         |            |
|       | AXI Read max waiting cycle (unit: 4 sys_clock cycle)                                 |         |            |
|       |                                                                                      |         |            |
|       | Waiting cycle larger, priority is lower                                              |         |            |
|       |                                                                                      |         |            |
|       | If hope read/write throughput the same, suggest WAITC_RD/ WAITC_WR ratio is around 5 |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 15:10 | Reserved                                                                             | 0b      | R          |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 9:0   | REG_WAITC_WR:                                                                        | 20h     | RW         |
|       |                                                                                      |         |            |
|       | AXI Write max waiting cycle (unit: 4 sys_clock cycle)                                |         |            |
|       |                                                                                      |         |            |
|       | Waiting cycle larger, priority is lower                                              |         |            |
|       |                                                                                      |         |            |
|       | If hope read/write throughput the same, suggest WAITC_RD/ WAITC_WR ratio is around 5 |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+

AXI master vs AXI channel:

+---------------------+--------------------------------------+
| AXI channel number  | AXI master                           |
+=====================+======================================+
| 1                   | PCIE (for PS5006) / SATA(for PS3110) |
|                     |                                      |
|                     | TDMAC0                               |
|                     |                                      |
|                     | TDMAC1                               |
|                     |                                      |
|                     | TDMAC2                               |
|                     |                                      |
|                     | TDMAC3                               |
|                     |                                      |
|                     | RS                                   |
|                     |                                      |
|                     | SS0                                  |
|                     |                                      |
|                     | SS1                                  |
|                     |                                      |
|                     | SEC                                  |
+---------------------+--------------------------------------+
| 2                   | FLH                                  |
|                     |                                      |
|                     | CPU0_Z3                              |
|                     |                                      |
|                     | CPU1_Z3                              |
|                     |                                      |
|                     | CPU2_Z3                              |
|                     |                                      |
|                     | CPU3_Z3                              |
+---------------------+--------------------------------------+
| 3                   | CPU0_DDR                             |
|                     |                                      |
|                     | CPU1_DDR                             |
|                     |                                      |
|                     | CPU2_DDR                             |
|                     |                                      |
|                     | CPU3_DDR                             |
+---------------------+--------------------------------------+

##### DDR3_SRMAC_DBG_SEL register (Offset 0ECh)

Default Value: 0000_0000h

+-------+------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                               | Default | Attributes |
+:=====:+============================================================+:=======:+:==========:+
| 31    | **r_sramc_rready_opt**                                     | 0b      | RW         |
|       |                                                            |         |            |
|       | **1: All channel RREADY always 1**                         |         |            |
|       |                                                            |         |            |
|       | **0: Depend on dynamic fifo full**                         |         |            |
+-------+------------------------------------------------------------+---------+------------+
| 30    | **r_sramc_rest\[2\], reset_value insert**                  | 0b      | RW         |
+-------+------------------------------------------------------------+---------+------------+
| 29:28 | **r_sramc_rest\[1:0\], reset_value**                       | 00b     | RW         |
|       |                                                            |         |            |
|       | **00: 7'h30 (3x512B)**                                     |         |            |
|       |                                                            |         |            |
|       | **01: 7'h10 (1x512B)**                                     |         |            |
|       |                                                            |         |            |
|       | **10: 7'h20 (2x512B)**                                     |         |            |
|       |                                                            |         |            |
|       | **11: 7'h40 (4x512B)**                                     |         |            |
+-------+------------------------------------------------------------+---------+------------+
| 27:9  | **Reserved**                                               | 0b      | R          |
+-------+------------------------------------------------------------+---------+------------+
| 8:0   | SRMAC_DBG_SEL, sramc (axi_ch_arbitor) debug port selection | 0b      | RW         |
+-------+------------------------------------------------------------+---------+------------+

##### DDR3_PAR_ERR_EN register (Offset 0F0h)

Default Value: 0000_0000h

  ------------------------------------------------------------------------------------------------
   Bit   Descriptions                                                        Default   Attributes
  ------ ------------------------------------------------------------------ --------- ------------
   31:1  **Reserved**                                                          0b          R

    0    PAR_ERR_EN_0, sram parity error injection enable, just for debug      0b          RW
  ------------------------------------------------------------------------------------------------

##### DDR3_FIFO_ERR_FLAG register (Offset 0F4h)

Default Value: 0000_0000h

  -------------------------------------------------------------------------------
   Bit   Descriptions                                       Default   Attributes
  ------ ------------------------------------------------- --------- ------------
   31:0  FIFO_ERR_FLAG\[31:0\], fifo push/pop error flag      0b          R

  -------------------------------------------------------------------------------

##### DDR3_FIFO_ERR_FLAG register (Offset 0F8h)

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------
    Bit   Descriptions                                        Default   Attributes
  ------- -------------------------------------------------- --------- ------------
   31:10  **Reserved**                                          0b          R

    9:0   FIFO_ERR_FLAG\[41:32\], fifo push/pop error flag      0b          R
  ---------------------------------------------------------------------------------

##### DDR3\_ BIST register1 (Offset 100h)2 (Offset 140h) 3(Offset 180h)

Default Value: 0000_0000h

+-------+---------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                | Default | Attributes |
+:=====:+=============================================================================================+:=======:+:==========:+
| 31:25 | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 24    | **clear_BIST1**                                                                             | 0b      | RW1        |
|       |                                                                                             |         |            |
|       | 0: Reset BIST is done or no reset BIST happens                                              |         |            |
|       |                                                                                             |         |            |
|       | 1: BIST1 setting reset, it return 0 after 512 clock                                         |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 33:21 | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 20    | **BIST_SET_ERROR**                                                                          | 0b      | R          |
|       |                                                                                             |         |            |
|       | 0: No Fail                                                                                  |         |            |
|       |                                                                                             |         |            |
|       | 1: BIST setting violation causes BIST Fail                                                  |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 19:18 | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 17:16 | **BIST_MODE**                                                                               | 0b      | RW         |
|       |                                                                                             |         |            |
|       | 0: DRAM Size test mode (the shortest time)                                                  |         |            |
|       |                                                                                             |         |            |
|       | 1: Squential address BIST mode                                                              |         |            |
|       |                                                                                             |         |            |
|       | 2: Random address BIST mode                                                                 |         |            |
|       |                                                                                             |         |            |
|       | 3: Add constant number for next start address BIST mode depending BIST_NUM_NXT_STADDR value |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 15:9  | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 8     | **BIST_FAIL**                                                                               | 0b      | R          |
|       |                                                                                             |         |            |
|       | 0: No Fail                                                                                  |         |            |
|       |                                                                                             |         |            |
|       | 1: Fail                                                                                     |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 7:2   | Reserved                                                                                    | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 1     | **Shift_4MB**                                                                               | 1b      | RW         |
|       |                                                                                             |         |            |
|       | 0: No shift BIST TEST Address                                                               |         |            |
|       |                                                                                             |         |            |
|       | 1: Shift BIST TEST Address 4MB                                                              |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+
| 0     | **BIST_TRIGGER**                                                                            | 0b      | RW1        |
|       |                                                                                             |         |            |
|       | It turn 0 after BIST is done or assign clear_BIST1=1                                        |         |            |
|       |                                                                                             |         |            |
|       | 0: No Enable or BIST Test Done                                                              |         |            |
|       |                                                                                             |         |            |
|       | 1: Enable BIST Test                                                                         |         |            |
+-------+---------------------------------------------------------------------------------------------+---------+------------+

##### DDR3\_ BIST register1 (Offset 104h)2 (Offset 144h) 3(Offset 184h)

Default Value: 0000_0000h

+------+-----------------------------------------+---------+------------+
| Bit  | Descriptions                            | Default | Attributes |
+:====:+=========================================+:=======:+:==========:+
| 31:3 | **BIST_START_ADDR**                     | 0b      | RW         |
|      |                                         |         |            |
|      | BIST Start address of BIST_MODE=1 or =2 |         |            |
|      |                                         |         |            |
|      | (unit: 4 words)                         |         |            |
+------+-----------------------------------------+---------+------------+
| 2:0  | Reserved                                | 0b      | R          |
+------+-----------------------------------------+---------+------------+

##### DDR3\_ BIST register 1(Offset 108h)2 (Offset 148h) 3(Offset 188h)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:3 | **BIST_END_ADDR**                     | 0b      | RW         |
|      |                                       |         |            |
|      | BIST End address of BIST_MODE=1 or =2 |         |            |
|      |                                       |         |            |
|      | (unit: 4 words)                       |         |            |
+------+---------------------------------------+---------+------------+
| 2:0  | Reserved                              | 0b      | R          |
+------+---------------------------------------+---------+------------+

##### DDR3\_ BIST register 1(Offset 10Ch)2 (Offset 14Ch)3 (Offset 18Ch)

Default Value: 0000_0000h

+------+--------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                             | Default | Attributes |
+:====:+==========================================================================+:=======:+:==========:+
| 31:3 | **BIST_RD_START_WRPOINT**                                                | 0b      | RW         |
|      |                                                                          |         |            |
|      | BIST start reading when BIST write address at the value of this register |         |            |
|      |                                                                          |         |            |
|      | (unit: 4 words)                                                          |         |            |
+------+--------------------------------------------------------------------------+---------+------------+
| 2:0  | Reserved                                                                 | 0b      | R          |
+------+--------------------------------------------------------------------------+---------+------------+

##### DDR3\_ BIST register 1(Offset 110h) 2(Offset 150h)3 (Offset 190h)

Default Value: 0000_0000h

+------+---------------------------------------+---------+------------+
| Bit  | Descriptions                          | Default | Attributes |
+:====:+=======================================+:=======:+:==========:+
| 31:0 | **BIST \_START_DATA**                 | 0b      | RW         |
|      |                                       |         |            |
|      | BIST start access data                |         |            |
+------+---------------------------------------+---------+------------+

##### DDR3\_ BIST register1 (Offset 114h)2 (Offset 154h) 3(Offset 194h)

Default Value: 0000_00f1h

+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                                              | Default | Attributes |
+:=====:+===========================================================================================================================================================================+:=======:+:==========:+
| 31:30 | Reserved                                                                                                                                                                  | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 29:28 | **BIST_WRAP**                                                                                                                                                             | 0b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0: BURST Type is only INCR                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: BUSRT Type is only WRAP                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: BUSRT Type is INCR or WRAP                                                                                                                                             |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3: BUSRT Type is INCR or WRAP                                                                                                                                             |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27:25 | **ECC_LEN_TP**                                                                                                                                                            | 0b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0: Keep Length setting by **BIST_LEN_FIX_EN** and **BIST_LEN**                                                                                                            |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: ECC LEN=0,3,15,63 random happen                                                                                                                                        |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: ECC LEN=3,15,63 random happen                                                                                                                                          |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3: Only LEN=64 happen                                                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 4: Only ECC LEN=63 happen                                                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 5: Only ECC LEN=15 happen                                                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | others: not support                                                                                                                                                       |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 24    | **RW_PERIOD_ADJUST**                                                                                                                                                      | 1b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0: no read/write period adjust                                                                                                                                            |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: If write overflow read next cycle, write will pause. And potentially **BIST_WR_DLY** will add 1 in every 32 times write pause happens.                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | Or if read overflow write next cycle, read will pause. And potentially **BIST_RD_DLY** will add 1 in every 32 times read pause happens.                                   |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:20 | **BIST_NUM_NXT_STADDR**                                                                                                                                                   | 100b    | RW         |
|       |                                                                                                                                                                           |         |            |
|       | **IF BIST_MODE=2,**                                                                                                                                                       |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 0: Next address is last end address plus 0x8 (unit:Byte)                                                                                                                  |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: Next address is last end address plus 0x10 (unit:Byte)                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: Next address is last end address plus 0x20 (unit:Byte)                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3: Next address is last end address plus 0x40 (unit:Byte)                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 4: Next address is last end address plus 0x80 (unit:Byte)                                                                                                                 |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | **.**                                                                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 10: Next address is last end address plus 0x2000 (unit:Byte)                                                                                                              |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 11: Next address is last end address plus 0x4000 (unit:Byte)                                                                                                              |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 12\~15: Next address is last end address plus 0x800 (unit:Byte)                                                                                                           |         |            |
|       |                                                                                                                                                                           |         |            |
|       | **IF BIST_MODE=3,**                                                                                                                                                       |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 0: Next address is last start address plus 0x8 (unit:Byte)                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: Next address is last start address plus 0x10 (unit:Byte)                                                                                                               |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: Next address is last start address plus 0x20 (unit:Byte)                                                                                                               |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3: Next address is last start address plus 0x40 (unit:Byte)                                                                                                               |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 4: Next address is last start address plus 0x80 (unit:Byte)                                                                                                               |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | **.**                                                                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 15: Next address is last start address plus 0x40000 (unit:Byte)                                                                                                           |         |            |
|       |                                                                                                                                                                           |         |            |
|       | **Note: In BIST_MODE=3 if BIST_NUM_NXT_STADDR setting value is smarter BIST length to let BIST write twice or read twice in the same memory address, it will be failed.** |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 19    | Reserved                                                                                                                                                                  | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 18:12 | **cnt_wcycle**                                                                                                                                                            | 0b      | R          |
|       |                                                                                                                                                                           |         |            |
|       | 0: BIST WR is at 1st cycle                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1: BIST WR is at 2nd cycle                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2: BIST WR is at 3rd cycle                                                                                                                                                |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 127: BIST WR is at 127th cycle                                                                                                                                            |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 11    | Reserved                                                                                                                                                                  | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 10:8  | **BIST_CYCLE**                                                                                                                                                            | 0b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0:BIST 1 cycle from BIST_START_ADDR to BIST_END_ADDR                                                                                                                      |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1:BIST 2 cycles from BIST_START_ADDR to BIST_END_ADDR                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2:BIST 4 cycles from BIST_START_ADDR to BIST_END_ADDR                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3:BIST 8 cycles from BIST_START_ADDR to BIST_END_ADDR                                                                                                                     |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | .                                                                                                                                                                         |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 7:BIST 128 cycles from BIST_START_ADDR to BIST_END_ADDR                                                                                                                   |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:4   | **BIST_LEN**                                                                                                                                                              | fh      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | BIST fixed Length                                                                                                                                                         |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3     | Reserved                                                                                                                                                                  | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2:1   | **BIST_DATAMODE**                                                                                                                                                         | 0b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0:Sequential data mode                                                                                                                                                    |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1:Random data mode                                                                                                                                                        |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 2:Toggle data mode                                                                                                                                                        |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 3:Not support                                                                                                                                                             |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0     | **BIST_LEN_FIX_EN**                                                                                                                                                       | 1b      | RW         |
|       |                                                                                                                                                                           |         |            |
|       | 0:BIST Length Random                                                                                                                                                      |         |            |
|       |                                                                                                                                                                           |         |            |
|       | 1:BIST Length Fixed depending on BIST_LEN                                                                                                                                 |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3\_ BIST register1 (Offset 118h) 2(Offset 158h) 3(Offset 198h)

Default Value: 0000_0000h

+-------+------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                         | Default | Attributes |
+:=====:+======================================================+:=======:+:==========:+
| 31:17 | **BIST_RD_DLY**                                      | 0b      | RW         |
|       |                                                      |         |            |
|       | Delay cycle number next read                         |         |            |
+-------+------------------------------------------------------+---------+------------+
| 16    | **BIST_RD_RDM_DLY_EN**                               | 0b      | RW         |
|       |                                                      |         |            |
|       | 0:Delay n cycles next read depending on BIST_RD_DLY  |         |            |
|       |                                                      |         |            |
|       | 1:Delay random cycles next read                      |         |            |
+-------+------------------------------------------------------+---------+------------+
| 15:1  | **BIST_WR_DLY**                                      | 0b      | RW         |
|       |                                                      |         |            |
|       | Delay cycle number next write                        |         |            |
+-------+------------------------------------------------------+---------+------------+
| 0     | **BIST_WR_RDM_DLY_EN**                               | 0b      | RW         |
|       |                                                      |         |            |
|       | 0:Delay n cycles next write depending on BIST_WR_DLY |         |            |
|       |                                                      |         |            |
|       | 1:Delay random cycles next write                     |         |            |
+-------+------------------------------------------------------+---------+------------+

##### DDR3_REFQ register (Offset 1C0h)

Default Value: 0000_3000h

+-------+---------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                      | Default | Attributes |
+:=====:+===================================================================================================+:=======:+:==========:+
| 31:18 | **Reserved**                                                                                      | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------+---------+------------+
| 17:16 | **ALLCYC_REF: All cycle number of auto-refresh in one round**                                     | 0b      | RW         |
|       |                                                                                                   |         |            |
|       | 0: 8192 cycles                                                                                    |         |            |
|       |                                                                                                   |         |            |
|       | 1: 4096 cycles                                                                                    |         |            |
|       |                                                                                                   |         |            |
|       | others: 16384 cycles                                                                              |         |            |
+-------+---------------------------------------------------------------------------------------------------+---------+------------+
| 15:8  | **T_ALLREF: Time limitation of All auto-refresh done in one round**                               | 30h     | RW         |
|       |                                                                                                   |         |            |
|       | T_ALLREF\*2\*(2\^18)\*T(DRAM clock frequecy)                                                      |         |            |
|       |                                                                                                   |         |            |
|       | 0h: Prohibit                                                                                      |         |            |
|       |                                                                                                   |         |            |
|       | 1h: 655.36us(DDR1600)                                                                             |         |            |
|       |                                                                                                   |         |            |
|       | 2h: 1310.72us(DDR1600)                                                                            |         |            |
|       |                                                                                                   |         |            |
|       | **.**                                                                                             |         |            |
|       |                                                                                                   |         |            |
|       | **.**                                                                                             |         |            |
|       |                                                                                                   |         |            |
|       | 30h: 31457.28us (DDR1600) (+9\*4.0us=31493.28us)\<32ms                                            |         |            |
|       |                                                                                                   |         |            |
|       | 31h: 32112.64us (DDR1600)                                                                         |         |            |
|       |                                                                                                   |         |            |
|       | **.**                                                                                             |         |            |
|       |                                                                                                   |         |            |
|       | **.**                                                                                             |         |            |
|       |                                                                                                   |         |            |
|       | 61h: 63569.92us (DDR1600) (+9\*7.9us=63641.02us)\<64ms                                            |         |            |
|       |                                                                                                   |         |            |
|       | 62h: 64225.28us (DDR1600)                                                                         |         |            |
|       |                                                                                                   |         |            |
|       | **.**                                                                                             |         |            |
|       |                                                                                                   |         |            |
|       | **.**                                                                                             |         |            |
+-------+---------------------------------------------------------------------------------------------------+---------+------------+
| 7     | **Reserved**                                                                                      | 0b      | R          |
+-------+---------------------------------------------------------------------------------------------------+---------+------------+
| 6:5   | **ALLCYC_REF: All cycle number of auto-refresh in one round**                                     | 0b      | RW         |
|       |                                                                                                   |         |            |
|       | 0: 8192 cycles                                                                                    |         |            |
|       |                                                                                                   |         |            |
|       | 1: 4096 cycles                                                                                    |         |            |
|       |                                                                                                   |         |            |
|       | others: 16384 cycles                                                                              |         |            |
+-------+---------------------------------------------------------------------------------------------------+---------+------------+
| 6:4   | **RLS_MULT_REF: release multi-refresh cycles once in the free time if NUM_REFQ is larger than 2** | 0b      | RW         |
|       |                                                                                                   |         |            |
|       | 0: Release 1 refresh once or until refresh queue is empty                                         |         |            |
|       |                                                                                                   |         |            |
|       | 1: Release 2 refresh once or until refresh queue is empty                                         |         |            |
|       |                                                                                                   |         |            |
|       | 2: Release 3 refresh once or until refresh queue is empty                                         |         |            |
|       |                                                                                                   |         |            |
|       | 3: Release 4 refresh once or until refresh queue is empty                                         |         |            |
|       |                                                                                                   |         |            |
|       | 4: Release 5 refresh once or until refresh queue is empty                                         |         |            |
|       |                                                                                                   |         |            |
|       | 5: Release 6 refresh once or until refresh queue is empty                                         |         |            |
|       |                                                                                                   |         |            |
|       | 6: Release 7 refresh once or until refresh queue is empty                                         |         |            |
|       |                                                                                                   |         |            |
|       | 7: Release 8 refresh once or until refresh queue is empty                                         |         |            |
+-------+---------------------------------------------------------------------------------------------------+---------+------------+
| 3:0   | **NUM_REFQ**                                                                                      | 0b      | RW         |
|       |                                                                                                   |         |            |
|       | 0: No auto-refresh queue                                                                          |         |            |
|       |                                                                                                   |         |            |
|       | 1: max number of auto-refresh queue is 1                                                          |         |            |
|       |                                                                                                   |         |            |
|       | 2: max number of auto-refresh queue is 2                                                          |         |            |
|       |                                                                                                   |         |            |
|       | 3: max number of auto-refresh queue is 3                                                          |         |            |
|       |                                                                                                   |         |            |
|       | 4: max number of auto-refresh queue is 4                                                          |         |            |
|       |                                                                                                   |         |            |
|       | 5: max number of auto-refresh queue is 5                                                          |         |            |
|       |                                                                                                   |         |            |
|       | 6: max number of auto-refresh queue is 6                                                          |         |            |
|       |                                                                                                   |         |            |
|       | 7: max number of auto-refresh queue is 7                                                          |         |            |
|       |                                                                                                   |         |            |
|       | others: max number of auto-refresh queue is 8 (because max is 9\*tREFI)                           |         |            |
+-------+---------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3_PFM_0 register (Offset 1C4h)

Default Value: 0000_3000h

+------+------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                               | Default | Attributes |
+:====:+============================================================+:=======:+:==========:+
| 31:4 | **Reserved**                                               | 0b      | R          |
+------+------------------------------------------------------------+---------+------------+
| 3:2  | **PFM_BGRP: performance bit group**                        | 0b      | RW         |
|      |                                                            |         |            |
|      | 0: bit 31\~0                                               |         |            |
|      |                                                            |         |            |
|      | 1: bit 63\~32                                              |         |            |
|      |                                                            |         |            |
|      | 2: bit 95\~64                                              |         |            |
|      |                                                            |         |            |
|      | 3: bit 127\~96                                             |         |            |
+------+------------------------------------------------------------+---------+------------+
| 1    | **PFM_CPT : performance capture value (return back to 0)** | 0b      | W1O        |
|      |                                                            |         |            |
|      | 0: no capture                                              |         |            |
|      |                                                            |         |            |
|      | 1: capture and value is not changed                        |         |            |
+------+------------------------------------------------------------+---------+------------+
| 0    | **PFM_EN : performance calculator enable**                 | 0b      | RW         |
|      |                                                            |         |            |
|      | 0: Disable                                                 |         |            |
|      |                                                            |         |            |
|      | 1: Enable                                                  |         |            |
+------+------------------------------------------------------------+---------+------------+

##### DDR3_PFM_0 register (Offset 1C8h)

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------
   Bit   Descriptions                                         Default   Attributes
  ------ --------------------------------------------------- --------- ------------
   31:0  **PFM_T_AC: 16B Data access times when PFM_EN=1**      0b          R

  ---------------------------------------------------------------------------------

##### DDR3_PFM_0 register (Offset 1CCh)

Default Value: 0000_0000h

  ----------------------------------------------------------------------------------
   Bit   Descriptions                                          Default   Attributes
  ------ ---------------------------------------------------- --------- ------------
   31:0  **PFM_T_ALL: DFI clock cycle times when PFM_EN=1**      0b          R

  ----------------------------------------------------------------------------------

##### DDR3_HIPRIOR_ID0 register (Offset 1E8h)

Default Value: 0000_3000h

+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                         | Default | Attributes |
+:=====:+======================================================================================================================================================+:=======:+:==========:+
| 31:24 | **HIPRIOT_ID3 : High priority read ID 3**                                                                                                            | ffh     | RW         |
|       |                                                                                                                                                      |         |            |
|       | ##### If read ID is the same as HIPRIOT_ID3, read command is prior to write command when bit\[31\] of DDR3_ROUND_ROBIN_WATC register (Offset 0E8h)=1 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:16 | **HIPRIOT_ID2 : High priority read ID 2**                                                                                                            | ffh     | RW         |
|       |                                                                                                                                                      |         |            |
|       | ##### If read ID is the same as HIPRIOT_ID2, read command is prior to write command when bit\[31\] of DDR3_ROUND_ROBIN_WATC register (Offset 0E8h)=1 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15:8  | **HIPRIOT_ID1 : High priority read ID 1**                                                                                                            | ffh     | RW         |
|       |                                                                                                                                                      |         |            |
|       | ##### If read ID is the same as HIPRIOT_ID1, read command is prior to write command when bit\[31\] of DDR3_ROUND_ROBIN_WATC register (Offset 0E8h)=1 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:0   | **HIPRIOT_ID0 : High priority read ID 0**                                                                                                            | ffh     | RW         |
|       |                                                                                                                                                      |         |            |
|       | ##### If read ID is the same as HIPRIOT_ID0, read command is prior to write command when bit\[31\] of DDR3_ROUND_ROBIN_WATC register (Offset 0E8h)=1 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3_HIPRIOR_ID1 register (Offset 1ECh)

Default Value: 0000_3000h

+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                         | Default | Attributes |
+:=====:+======================================================================================================================================================+:=======:+:==========:+
| 31:24 | **HIPRIOT_ID7 : High priority read ID 7**                                                                                                            | ffh     | RW         |
|       |                                                                                                                                                      |         |            |
|       | ##### If read ID is the same as HIPRIOT_ID7, read command is prior to write command when bit\[31\] of DDR3_ROUND_ROBIN_WATC register (Offset 0E8h)=1 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:16 | **HIPRIOT_ID6 : High priority read ID 6**                                                                                                            | ffh     | RW         |
|       |                                                                                                                                                      |         |            |
|       | ##### If read ID is the same as HIPRIOT_ID6, read command is prior to write command when bit\[31\] of DDR3_ROUND_ROBIN_WATC register (Offset 0E8h)=1 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15:8  | **HIPRIOT_ID5 : High priority read ID 5**                                                                                                            | ffh     | RW         |
|       |                                                                                                                                                      |         |            |
|       | ##### If read ID is the same as HIPRIOT_ID5, read command is prior to write command when bit\[31\] of DDR3_ROUND_ROBIN_WATC register (Offset 0E8h)=1 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:0   | **HIPRIOT_ID4 : High priority read ID 4**                                                                                                            | ffh     | RW         |
|       |                                                                                                                                                      |         |            |
|       | ##### If read ID is the same as HIPRIOT_ID4, read command is prior to write command when bit\[31\] of DDR3_ROUND_ROBIN_WATC register (Offset 0E8h)=1 |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

##### DDR3_Z1_VALID_ADDR register (Offset 1D0h)

Default Value: 0000_0000h

+------+------------------------------------------------------------------------------------------+-------------+------------+
| Bit  | Descriptions                                                                             | Default     | Attributes |
+:====:+==========================================================================================+:===========:+:==========:+
| 31:0 | Z1_VALID_ADDR\[31:0\], zone1 valid address boundary                                      | 0xFFFF_FFFF | W/R        |
|      |                                                                                          |             |            |
|      | Z1_VALID_ADDR\[31:0\] = Z1 start addr + (Z1 length \* 1/2) -- ddr addr base, unit : byte |             |            |
+------+------------------------------------------------------------------------------------------+-------------+------------+

\*ddr addr base = 0x8000_0000

\*512B is for the boundary gap

\*If AXI packet address \> Z1_VALID_ADDR, the Z1 invalid flag will assert.

##### DDR3_Z2_VALID_ADDR register (Offset 1D4h)

Default Value: 0000_0000h

+------+--------------------------------------------------------------------------------------------+-------------+------------+
| Bit  | Descriptions                                                                               | Default     | Attributes |
+:====:+============================================================================================+:===========:+:==========:+
| 31:0 | Z2_VALID_ADDR\[31:0\], zone1 valid address boundary                                        | 0xFFFF_FFFF | W/R        |
|      |                                                                                            |             |            |
|      | Z2_VALID_ADDR\[31:0\] = Z2 start addr + (Z2 length \* 32/36) -- ddr addr base, unit : byte |             |            |
+------+--------------------------------------------------------------------------------------------+-------------+------------+

\*ddr addr base = 0x8000_0000

\*512B is for the boundary gap

\*to avoid the overflow issue, please divide by 36 first, than multiply 32

\*If AXI packet address \> Z2_VALID_ADDR, the Z2 invalid flag will assert.

##### DDR3_Z3_VALID_ADDR register (Offset 1D8h)

Default Value: 0000_0000h

+------+----------------------------------------------------------------------------------------------+-------------+------------+
| Bit  | Descriptions                                                                                 | Default     | Attributes |
+:====:+==============================================================================================+:===========:+:==========:+
| 31:0 | Z3_VALID_ADDR\[31:0\], zone1 valid address boundary                                          | 0xFFFF_FFFF | W/R        |
|      |                                                                                              |             |            |
|      | Z3_VALID_ADDR\[31:0\] = Z3 start addr + (Z3 length \* 512/520) -- ddr addr base, unit : byte |             |            |
+------+----------------------------------------------------------------------------------------------+-------------+------------+

\*ddr addr base = 0x8000_0000

\*512B is for the boundary gap

\*to avoid the overflow issue, please divide by 520 first, than multiply 512

\*If AXI packet address \> Z3_VALID_ADDR, the Z3 invalid flag will assert.

For example,

If

SYS_DZ1_DSA = 32\'h8000_0000

SYS_DZ1_DLEN= 32\'h0002_0000

SYS_DZ2_DSA = 32\'h8002_0000

SYS_DZ2_DLEN= 32\'h0002_0000

SYS_DZ3_DSA = 32\'h8004_0000

SYS_DZ3_DLEN= 32\'h0002_0000

You need to set:

Z1_VALID_ADDR = 32'h0000_FE00

Z2_VALID_ADDR = 32'h0003_C500

Z3_VALID_ADDR = 32'h0005_F600

##### DDR3\_ DRAM_CLOCK_GATE register (Offset 200h)

Default Value: 0001_80bfh(PS5006)

Default Value: 0001_8080h(PS3110)

+-------+----------------------------------------------+-------------+------------+
| Bit   | Descriptions                                 | Default     | Attributes |
+:=====:+==============================================+:===========:+:==========:+
| 31:23 | Reserved                                     | 0b          | R          |
+-------+----------------------------------------------+-------------+------------+
| 22    | **MEMINITDONE**                              | 0b          | R          |
|       |                                              |             |            |
|       | 0: DDR not Initialization complete           |             |            |
|       |                                              |             |            |
|       | 1: DDR Initialization complete               |             |            |
+-------+----------------------------------------------+-------------+------------+
| 21    | **ctl_rst_n : PHY controller resetn**        | 1b          | RW         |
+-------+----------------------------------------------+-------------+------------+
| 20    | **presetn: PHY register resetn**             | 1b          | RW         |
+-------+----------------------------------------------+-------------+------------+
| yyp19 | **ret_en : PHY retention enable signal**     | 1b          | RW         |
+-------+----------------------------------------------+-------------+------------+
| 18    | **ret_en_i : PHY retention enable signal**   | 0b          | RW         |
+-------+----------------------------------------------+-------------+------------+
| 17    | **ret_en_n_i : PHY retention enable signal** | 1b          | RW         |
+-------+----------------------------------------------+-------------+------------+
| 16    | **DFIINITSTART**                             | 1b          | RW         |
|       |                                              |             |            |
|       | 0: DFIINIT disable.                          |             |            |
|       |                                              |             |            |
|       | 1: DFIINIT start.                            |             |            |
+-------+----------------------------------------------+-------------+------------+
| 15:13 | Reserved                                     | 4h          | RW         |
+-------+----------------------------------------------+-------------+------------+
| 12:9  | **DFIDATABYTEDISABLE**                       | 0b          | RW         |
|       |                                              |             |            |
|       | 0000: No DATA bus disable                    |             |            |
|       |                                              |             |            |
|       | 0001:Disable Byte1 DATA bus                  |             |            |
|       |                                              |             |            |
|       | 0010:Disable Byte2 DATA bus                  |             |            |
|       |                                              |             |            |
|       | 0100:Disable Byte3 DATA bus                  |             |            |
|       |                                              |             |            |
|       | 1000:Disable Byte4 DATA bus                  |             |            |
|       |                                              |             |            |
|       | 1111:Disable all Byte DATA bus               |             |            |
+-------+----------------------------------------------+-------------+------------+
| 8     | **DFIDRAMCLKDISABLE**                        | 0b          | RW         |
|       |                                              |             |            |
|       | 0: DRAM Clock is not gated.                  |             |            |
|       |                                              |             |            |
|       | 1: DRAM Clock is gated.                      |             |            |
+-------+----------------------------------------------+-------------+------------+
| 7     | Reserved                                     | 1b          | RW         |
+-------+----------------------------------------------+-------------+------------+
| 6     | **DFIINITCOMPLETE**                          | 0b          | R          |
|       |                                              |             |            |
|       | 0: DDR PHY not Initialization complete       |             |            |
|       |                                              |             |            |
|       | 1: DDR PHY Initialization complete           |             |            |
+-------+----------------------------------------------+-------------+------------+
| 5:0   | Reserved                                     | 3fb(PS5006) | R          |
|       |                                              |             |            |
|       |                                              | 0b(PS3110)  |            |
+-------+----------------------------------------------+-------------+------------+
