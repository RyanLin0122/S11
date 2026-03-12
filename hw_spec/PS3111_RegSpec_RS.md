#### Topic: Register Space of RS Engine

**Version:** 0.3

**Date: June** 9, 2014

**by**: C.Y.

##

Table 1. Revision History

  --------------------- ---------- ------------ ----------------------------------------
   **Revision Number**   **Date**   **Author**              **Description**

          0.1.           2014/6/9      C.Y.              Modify RS OTF register
  --------------------- ---------- ------------ ----------------------------------------

## 1. Introduction

This module is RS encode/decode module.

1.1 Module Block Diagram

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image1.wmf)

The RS module has 4 internal SRAM. The buffer pointer table (BPT) is used to store all pointer map to all RS pages in DRAM. The RS parity buffer (PBUF) is used to store parity or syndrome. The block sequence (BSEQ) is used to store instructions for block trigger. The NMODE buffer is used to store data in normal mode correction.

1.2 Abbreviations

OTF RS on-the-fly

  ----------- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  PBUF        RS parity buffer.

  DPAGE       Data page. The pages for RS data.

  PPAGE       Parity page. The pages for RS parity. The RS module supports 253 DPAGEs/2 PPAGEs

  DBASE       Date base address. Base address for data region of each page and must align to 4k Byte boundary.

  SBASE       Spare base address. Base address for spare region of each page. Each 4K data corresponding to 16B spare data. RS module support 4 base address (DBASE_0/1/2/3 and SBASE_0/1/2/3). The selection of base address is set in tag of BSEQ or BPT and must align to 32Byte boundary.

  BP          Buffer pointer. Every buffer pointer is mapping to 4k data in DRAM and must align to 4k boundary.

  BPT         Buffer pointer table

  BSEQ        Block trigger sequence. Instructions used for block trigger.

  NMODE       RS normal mode.

  TBDMA       Table DMA. Transfer BPT or BSEQ table data with DRAM.

  PBPT        RS parity buffer pointer table

  PRC         RS parity ready count

  PEC         RS parity encode count

  PRF         RS parity ready flag

  PPF         RS program parity flag
  ----------- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

1.3 Frame Format

The RS module implement RS code with n = 255, k = 253, t = 1. It supports 253 DPAGEs/2 PPAGEs and can recover 2 page data.

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image2.png){width="6.690277777777778in" height="2.1638888888888888in"}

1.4 Memory Mapping

Every parity page contains data (16KB) and spares (4x16B). They map to parity data region and parity spare region in DRAM. Every 4KB parity data has 16B parity spare. These region are defined by parity data/spare base address(PDBASE and PSBASE). The parity data region can be in DRAM Zone 1, Zone 2, Zone 3, data buffer(DBUF) or share buffer (SBUF). The parity spare region can be in Zone2, SBUF or CPU DM. Both parity data region and parity spare region cannot across Zone boundary.

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image3.png){width="5.079166666666667in" height="1.729861111111111in"}

For every parity data address, the corresponding parity spare address can be obtained. Assume PageAddr is byte address for data and PageOffset is the data offset.

=\> PageAddr = DBASE + PageOffset

=\> SpareOffset = (PageOffset/256)

=\> SpareAddr = SBASE + SpareOffset

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image4.png){width="0.7166666666666667in" height="0.45625in"}

(1) In case of 4k page

> ![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image5.png){width="6.000694444444444in" height="1.6083333333333334in"}

(2) In case of 8k page

> ![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image6.png){width="6.000694444444444in" height="1.6083333333333334in"}

(3) In case of 16k page

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image7.png){width="6.000694444444444in" height="1.6083333333333334in"}

1.5 Encode

To generate RS parity, every data page(DPAGE) should be encoded. For k DPGAEs, the basic flow is as follows.

Init PBUF

For i = 0, 1, ..., k-1

PBUF = PBUF + ENC(DPAGE\[i\])

RS module supports encoding for arbitrary index of DPAGE. FW can set encoding start index (RS_ROW_IDX) and number of pages to encode (RS_PAGE_CNT). Therefore, the encoding for whole k DPAGEs can be split to several trigger.

Example:

A. Trigger all k pages

RS_ROW_IDX = 0

RS_PAGE_CNT = k

TRIG_ENC

B. Trigger first t pages, and then rest k-t pages

RS_ROW_IDX = 0

RS_PAGE_CNT = t

TRIG_ENC

// prepare next k-t DPAGEs data

RS_ROW_IDX = t

RS_PAGE_CNT = k-t

TRIG_ENC

C. Trigger last k-t pages, and then the first t pages

RS_ROW_IDX = t

RS_PAGE_CNT = k-t

TRIG_ENC

// prepare the t DPAGEs data

RS_ROW_IDX = 0

RS_PAGE_CNT = t

TRIG_ENC

1.6 Decode/Correct

Similar to encoding flow, all data pages and parity pages (PPAGEs) should be decoded to compute RS syndrome. Before RS decoding, FW needs set the index of error pages to RS_ERA_IDX. The RS_ERA_IDX_A/B correspond to 1st error page and 2nd error page. The error page number is set to RS_ERA_IDX_EN_NUM. The decode or correct trigger computes for 1 column data (4KB region). FW needs assign which column to process (RS_COL_IDX).

Once syndrome is obtained, the RS_COR module can generate the data of error pages. The basic flow is as follows.

set RS_ERA_IDX

Init PBUF

For i = 0, 1, ..., n-1

PBUF = PBUF + DEC(PAGE\[i\])

TRIG_COR

1.7 Buffer Pointer Setting

For each trigger, RS module must obtain BPs for DPAGEs and PPAGEs. Every BP maps to 4k data. For 16k page usage, one page has 4 BPs. The BPT can store all BPs for DPAGEs and PPAGEs. These BPs contains base address selection and address offset in DRAM space. Each 32-bit BP is in this format. The BASE_SEL indicates the index of base address. The OFT is the address offset. For data region, the DRAM address is computed as DBASE + OFT.

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image8.wmf)

Example:

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image9.wmf)

RS on-the-fly doesn't use DBASE_1\~3, so need to set bit 8 and bit 9 of the buffer pointer to zero.

One page data must has the same base address. For a 16K page, its 4 BPs must select the same base address (the BASE_SEL values are the same). If the BPs are continuous, RS module supports continue mode (RS_ONE_CONT or RS_ALL_CONT) for getting next BP.

The index of parity BPs are obtained by RS_DPAGE_NUM. Assume RS_DPAGE_NUM are set as default value (0xFD), the index of parity BPs (PBP0/1_IDX) are 0xFD and 0xFE. Denote all BPs in BPT as follows.

Here are some example for setting BPs.

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image10.wmf)

For encoding

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image11.wmf)

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image12.wmf)

For decoding

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image13.wmf)![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image14.wmf)

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image15.wmf)![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image16.wmf)

For correct

In correct mode, RS module will get BPs according to ERA index. The recovered data will write back to corresponding DRAM area.

Example:

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image17.wmf)

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image18.wmf)

1.8 Block Trigger Setting (removed BSEQ)

1.9 RS Normal Mode Correct

If the number of error pages is more than recoverable page number. The RS module can try correct pages using normal mode (NMODE). In NMODE, each RS codeword can correct 1B error. There are three phases to perform normal mode correction.

Decode: Generate syndrome

Correct: Generate error pattern and error location

NMODE_write: Write corrected vale to error pages

In decode phase, the ERA_IDX_EN_NUM should set to 0. All pages are needed to be decoded. In correct phase, the COR_NORMAL_MODE should set to 1. Other setting for decode and correct phase is the same as in erasure mode. In NMODE_write, FW has to indicate which PAGE has error by setting NMODE_IDXs and NMODE_ERR_IDX_NUM. If the error pages are more than maximum index number, NMODE_write can be triggered more than once.

Example:

Assume Page 4, 5 and 10 are error pages. The FW flow is as follows.

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image19.wmf){width="2.1527777777777777in" height="2.0520833333333335in"}

Assume Page 0, 1, 2, \...,7, 8, 9, and 10 are error pages.

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image20.wmf){width="2.1527777777777777in" height="3.3854166666666665in"}

1.10 Table DMA

To save/load BPT or BSEQ date with DRAM, the RS module support table DMA (TBDMA) function. FW should set the address in DRAM, select table, and data length. Then start TBDMA and wait transfer finish. The TBDMA supports transfer with DRAM Zone.

If target address is TEG_ADDR, the FW usage is as follows:

+------------------------------------------------------------------+------------------------------------------------------------------+
| Save BPT0                                                        | Load BPT0                                                        |
|                                                                  |                                                                  |
| ![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image21.wmf) | ![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image22.wmf) |
+------------------------------------------------------------------+------------------------------------------------------------------+
| Save BPT1                                                        | Load BPT1                                                        |
|                                                                  |                                                                  |
| ![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image23.wmf) | ![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image24.wmf) |
+------------------------------------------------------------------+------------------------------------------------------------------+

The AXI length is 128B.

1.11 Encode Trigger Queue (RS on-the fly doesn't use)

1.12 Buffer Pointer Rotation (RS on-the fly doesn't use)

1.13 Buffer Flag Support (RS on-the fly doesn't use)

1.14 RS On-The-Fly Encode Parity Parameter Management Table(RS_OTFENC_PPMT)

RS_OTFENC_PPMT is used to records each RS tag's status during on-the-fly encode process. It contains 32 RS tags. Every RS tag maps to a RS frame.

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image25.png){width="5.074305555555555in" height="2.6680555555555556in"}

The parameters for each RS tag are as follows (detail usage descriptions are in the following section).

(1) Valid bit:

> Valid bit means the parameters in RS_OTFENC_PPMT are valid or not. If Valid bit is asserted, the parameters of RS tag are valid. Otherwise, the parameters of RS tag are not guaranteed.

(2) Parity Buffer Pointer Table(PBPT)

> Parity buffer pointer table is used to store all pointers map to all RS parities of RS tag0\~31 in DRAM.

(3) Parity Ready Count(PRC)

> Parity ready count is used to store DPAGE value for RS tag.

(4) Parity Encode Count(PEC)

> Parity Encode Count indicates how many times this parity encoded.

(5) Parity Ready Flag(PRF)

> Parity ready flag indicates the parity is ready encoded. The ready Parity can be programmed into NAND flash. If PEC is equal to PRC, PRF will be set.

(6) Program Parity Flag(PPF)

> Program parity flag is used to indicate the parity is going to be programmed into NAND flash. In other words, the instruction of program parity is already in MTQ.

1.14.1 Valid bit

Valid bit is a RW1C status register(Please refer to section 2). It can only set by HW while a RS tag whose Valid is 0 is going to be encoded. Valid bit can be clear by HW or FW. HW will clear Valid bit while a RS tag's parity is programmed to NAND Flash. FW can also clear Valid bit manually by CPU.

Example:

(1) Set Valid bit:

> Assume there are 8 RS frames data in DRAM. The 8 RS frames map to RS tag 0\~7 seperatelly. Valid bit of RS tag 0\~7 are all 0. When FW fill MTQ to program the 8 RS frames data into Nand flash, HW will set Valid bit for each RS tag while executing MT instruction.
>
> ![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image26.png){width="6.413194444444445in" height="3.859027777777778in"}

(2) Clear Valid bit by HW.

> HW will clear RS tag's Valid bit while the corresponding parity is programmed into Nand Flash.
>
> ![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image27.png){width="6.393055555555556in" height="3.7in"}

(3) Clear Valid bit by FW.

> In case of FW clear Valid bits manually, FW must check whether the program instructions for RS tag in MT are already executed(encoded). The information can be known by check Parity Encode Count(PEC). If PEC is equal to the number of page program instructions FW has put in MT, then FW can clear Valid bit of the corresponding RS tag.
>
> ![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image28.png){width="6.686805555555556in" height="5.198611111111111in"}

1.14.2 Parity Buffer Pointer Setting

FW must set Parity Buffer Pointer in advance(please refer to 2004h, RS_TAG_CONFIG_1) for each RS tag which is going to be encoded. For a 4K, 8K or 16K page, every PBP maps to 4K, 8K or 16K parity data seperately. These PBPs contains base address selection and address offset in DRAM space. Each 32-bit PBP is in this format. The BASE_SEL indicates the index of base address. The OFT is the address offset. For data region, the DRAM address is computed as PDBASE + OFT.

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image29.png){width="4.544444444444444in" height="0.8840277777777777in"}

Example:

RS_TAG_PDBASE_0 = 0xA0000000; RS_TAG_PSBASE_0 = 0x80000000;

RS_TAG_PDBASE_1 = 0xB0000000; RS_TAG_PSBASE_1 = 0x81000000;

PBP = 0x00602**[0]{.underline}**00

- Parity data address = 0xA0000000 + 0x00602000;

- Pairty spare address = 0x80000000 + 0x0000C040;

> PBP = 0x00602**[1]{.underline}**00

- Parity data address = 0xB0000000 + 0x00602000;

- Pairty spare address = 0x81000000 + 0x0000C040;

  1.14.3 PRF, PEC, PRC and Last Page Setting

FW must set Last Page flag in MT parameters if the RS page is the last page of RS tag. According to the Last Page flag, the corresponding PRC would be set to RS page number plus 1.

Example:

For a data program instruction with Last Page flag ***asserted*** in MTQ, the parameters for RS OTF Encode are as follows,

(1) *RS_OTFENC_RS_TAG = N*

(2) *RS_OTFENC_RS_PAGE_NUM = M*

(3) *RS_OTFENC_LAST_PAGE = 1*

(4) *RS_OTFENC_PROG_PARITY = 0*

(5) *RS_OTFENC_ENABLE = 1*

- HW will set the PRC value to **M+1**. (M is the last RS page number of RS tag N)

PRF is a RO status register. PRF will be set while PEC is equal to PRC.

Example:

(A) RS tag = 0, PRC = 127, PEC = 127

- PRF = 1

(B) RS tag = 0, PRC = 8, PEC = 3

- PRF = 0

  1.14.4 Program Parity Flag(PPF) Setting

Program parity flag is used to indicates the RS tag's parity which is going to be programmed. This flag is put in MT parameters (please refer to section 1.6). FW needs to set PPF if the MT instruction is a program parity command.

Example:

For a program parity command in MTQ, the MT parameters for RS OTF Encode is like as below,

(1) *RS_OTFENC_RS_TAG = N*

(2) *RS_OTFENC_RS_PAGE_NUM = ignore*

(3) *RS_OTFENC_LAST_PAGE = 0*

(4) *RS_OTFENC_PROG_PARITY = 1*

(5) *RS_OTFENC_ENABLE = 0*

- HW will set the PPF to 1 for RS tag N.

- If PRF flag of RS tag N is asserted, HW will program parity directly. Otherwise, HW will wait until PRF is set.

  1.15 MT parameters for RS on-the-fly encode

FW needs to fill below parameters in MT to let RS on-the-fly encode process normally. Data pages in the same RS frame must has the same RS tag. RS_LAST_PAGE must be set ONLY in the last data page of RS frame. RS_PROG_PARITY is used to indicates the MT insruction is a program parity command. RS_OTFENC_ENABLE is used to indicates whether this MT instruction needs to to do RS encoded or not.

+-----+---------------------------+----------------------+-----------+----------------------------------------------------------------------------+
| No. | Category                  | Name                 | Bit width | comment                                                                    |
+-----+---------------------------+----------------------+-----------+----------------------------------------------------------------------------+
| 1   | RS OTF Encode information | RS_TAG               | \[4:0\]   | Indicates the RS tag number                                                |
+-----+                           +----------------------+-----------+----------------------------------------------------------------------------+
| 2   |                           | RS_PAGE_NUM          | \[7:0\]   | Indicates the RS page number                                               |
+-----+                           +----------------------+-----------+----------------------------------------------------------------------------+
| 3   |                           | RS_LAST_PAGE         |           | Indicates the DPAGE value of RS tag                                        |
+-----+---------------------------+----------------------+-----------+----------------------------------------------------------------------------+
| 4   | RS OTF Encode control     | RS_PROG_PARITY       |           | Indicates whether this instruction is program parity or not                |
+-----+                           +----------------------+-----------+----------------------------------------------------------------------------+
| 5   |                           | RS\_ PROG_2ND_PARITY |           | Indicates whether this instruction is second program parity or not         |
+-----+                           +----------------------+-----------+----------------------------------------------------------------------------+
| 6   |                           | RS_OTFENC_ENABLE     |           | Indicates whether this MT instruction needs to do RS encode or not.        |
+-----+---------------------------+----------------------+-----------+----------------------------------------------------------------------------+
| 7   | XOR OTF Encode control    | XOR_OTFENC_ENABLE    |           | Indicates whether this MT instruction needs to do XOR encode or RS encode. |
+-----+---------------------------+----------------------+-----------+----------------------------------------------------------------------------+

Example:

For TLC block, a RS frame(maps to RS tag 0) with the correponding MT parameters is list below.

[RS_TAG, RS_PAGE_NUM, RS_LAST_PAGE, RS_PROG_PARITY, RS_OTFENC_ENABLE. XOR_OTFENC_ENABLE]{.underline}

**0x00,0x01,0x0,0x0,0x1,0x0**

**0x00,0x02,0x0,0x0,0x1,0x0**

**0x00,0x03,0x0,0x0,0x1,0x0**

**0x00,0x04,0x0,0x0,0x1,0x0**

**0x00,0x05,0x0,0x0,0x1,0x0**

**0x00,0x06,0x0,0x0,0x1,0x0**

**0x00,0x07,0x0,0x0,0x1,0x0**

**0x00,0x08,0x0,0x0,0x1,0x0**

**0x00,0x09,0x0,0x0,0x1,0x0**

**0x00,0x0A,0x0,0x0,0x1,0x0**

**0x00,0x0B,0x0,0x0,0x1,0x0**

**0x00,0x0C,0x0,0x0,0x1,0x0**

**0x00,0x0D,0x0,0x0,0x1,0x0**

**0x00,0x0E,0x0,0x0,0x1,0x0**

**0x00,0x0F,0x0,0x0,0x1,0x0**

**0x00,0x10,0x0,0x0,0x1,0x0**

**0x00,0x11,0x0,0x0,0x1,0x0**

**0x00,0x12,0x0,0x0,0x1,0x0**

**0x00,0x13,0x0,0x0,0x1,0x0**

**0x00,0x14,0x0,0x0,0x1,0x0**

**0x00,0x15,0x0,0x0,0x1,0x0**

**0x00,0x16,0x0,0x0,0x1,0x0**

**0x00,0x17,0x0,0x0,0x1,0x0**

**0x00,0x18,0x0,0x0,0x1,0x0**

**0x00,0x19,0x0,0x0,0x1,0x0**

**0x00,0x1A,0x0,0x0,0x1,0x0**

**0x00,0x1B,0x0,0x0,0x1,0x0**

**0x00,0x1C,0x0,0x0,0x1,0x0**

**0x00,0x1D,0x0,0x0,0x1,0x0**

**0x00,0x1E,0x0,0x0,0x1,0x0**

**0x00,0x1F,0x0,0x0,0x1,0x0**

**0x00,0x20,0x0,0x0,0x1,0x0**

**0x00,0x21,0x0,0x0,0x1,0x0**

**0x00,0x22,0x0,0x0,0x1,0x0**

**0x00,0x23,0x0,0x0,0x1,0x0**

**0x00,0x24,0x0,0x0,0x1,0x0**

**0x00,0x25,0x0,0x0,0x1,0x0**

**0x00,0x26,0x0,0x0,0x1,0x0**

**0x00,0x27,0x0,0x0,0x1,0x0**

**0x00,0x28,0x0,0x0,0x1,0x0**

**0x00,0x29,0x0,0x0,0x1,0x0**

**0x00,0x2A,0x0,0x0,0x1,0x0**

**0x00,0x2B,0x0,0x0,0x1,0x0**

**0x00,0x2C,0x0,0x0,0x1,0x0**

**0x00,0x2D,0x0,0x0,0x1,0x0**

**0x00,0x2E,0x0,0x0,0x1,0x0**

**0x00,0x2F,0x0,0x0,0x1,0x0**

**0x00,0x30,0x0,0x0,0x1,0x0**

**0x00,0x31,0x0,0x0,0x1,0x0**

**0x00,0x32,0x0,0x0,0x1,0x0**

**0x00,0x33,0x0,0x0,0x1,0x0**

**0x00,0x34,0x0,0x0,0x1,0x0**

**0x00,0x35,0x0,0x0,0x1,0x0**

**0x00,0x36,0x0,0x0,0x1,0x0**

**0x00,0x37,0x0,0x0,0x1,0x0**

**0x00,0x38,0x0,0x0,0x1,0x0**

**0x00,0x39,0x0,0x0,0x1,0x0**

**0x00,0x3A,0x0,0x0,0x1,0x0**

**0x00,0x3B,0x0,0x0,0x1,0x0**

**0x00,0x3C,0x0,0x0,0x1,0x0**

**0x00,0x3D,0x0,0x0,0x1,0x0**

**0x00,0x3E,0x0,0x0,0x1,0x0**

**0x00,0x3F,0x0,0x0,0x1,0x0**

**0x00,0x40,0x0,0x0,0x1,0x0**

**0x00,0x41,0x0,0x0,0x1,0x0**

**0x00,0x42,0x0,0x0,0x1,0x0**

**0x00,0x43,0x0,0x0,0x1,0x0**

**0x00,0x44,0x0,0x0,0x1,0x0**

**0x00,0x45,0x0,0x0,0x1,0x0**

**0x00,0x46,0x0,0x0,0x1,0x0**

**0x00,0x47,0x0,0x0,0x1,0x0**

**0x00,0x48,0x0,0x0,0x1,0x0**

**0x00,0x49,0x0,0x0,0x1,0x0**

**0x00,0x4A,0x0,0x0,0x1,0x0**

**0x00,0x4B,0x0,0x0,0x1,0x0**

**0x00,0x4C,0x0,0x0,0x1,0x0**

**0x00,0x4D,0x0,0x0,0x1,0x0**

**0x00,0x4E,0x0,0x0,0x1,0x0**

**0x00,0x4F,0x0,0x0,0x1,0x0**

**0x00,0x50,0x0,0x0,0x1,0x0**

**0x00,0x51,0x0,0x0,0x1,0x0**

**0x00,0x52,0x0,0x0,0x1,0x0**

**0x00,0x53,0x0,0x0,0x1,0x0**

**0x00,0x54,0x0,0x0,0x1,0x0**

**0x00,0x55,0x0,0x0,0x1,0x0**

**0x00,0x56,0x0,0x0,0x1,0x0**

**0x00,0x57,0x0,0x0,0x1,0x0**

**0x00,0x58,0x0,0x0,0x1,0x0**

**0x00,0x59,0x0,0x0,0x1,0x0**

**0x00,0x5A,0x0,0x0,0x1,0x0**

**0x00,0x5B,0x0,0x0,0x1,0x0**

**0x00,0x5C,0x0,0x0,0x1,0x0**

**0x00,0x5D,0x0,0x0,0x1,0x0**

**0x00,0x5E,0x0,0x0,0x1,0x0**

**0x00,0x5F,0x0,0x0,0x1,0x0**

**0x00,0x60,0x0,0x0,0x1,0x0**

**0x00,0x61,0x0,0x0,0x1,0x0**

**0x00,0x62,0x0,0x0,0x1,0x0**

**0x00,0x63,0x0,0x0,0x1,0x0**

**0x00,0x64,0x0,0x0,0x1,0x0**

**0x00,0x65,0x0,0x0,0x1,0x0**

**0x00,0x66,0x0,0x0,0x1,0x0**

**0x00,0x67,0x0,0x0,0x1,0x0**

**0x00,0x68,0x0,0x0,0x1,0x0**

**0x00,0x69,0x0,0x0,0x1,0x0**

**0x00,0x6A,0x0,0x0,0x1,0x0**

**0x00,0x6B,0x0,0x0,0x1,0x0**

**0x00,0x6C,0x0,0x0,0x1,0x0**

**0x00,0x6D,0x0,0x0,0x1,0x0**

**0x00,0x6E,0x0,0x0,0x1,0x0**

**0x00,0x6F,0x0,0x0,0x1,0x0**

**0x00,0x70,0x0,0x0,0x1,0x0**

**0x00,0x71,0x0,0x0,0x1,0x0**

**0x00,0x72,0x0,0x0,0x1,0x0**

**0x00,0x73,0x0,0x0,0x1,0x0**

**0x00,0x74,0x0,0x0,0x1,0x0**

**0x00,0x75,0x0,0x0,0x1,0x0**

**0x00,0x76,0x0,0x0,0x1,0x0**

**0x00,0x77,0x0,0x0,0x1,0x0**

**0x00,0x78,0x0,0x0,0x1,0x0**

**0x00,0x79,0x0,0x0,0x1,0x0**

**0x00,0x7A,0x0,0x0,0x1,0x0**

**0x00,0x7B,0x0,0x0,0x1,0x0**

**0x00,0x7C,0x0,0x0,0x1,0x0**

**0x00,0x7D,0x0,0x0,0x1,0x0**

**0x00,0x7E,0x1,0x0,0x1,0x0**

**0x00,0x7F,0x0,0x1,0x0,0x0**

**0x00,0x80,0x0,0x1,0x0,0x0**

Example:

For TLC block, an XOR frame(maps to RS tag 0) with the correponding MT parameters is list below.

[RS_TAG, RS_PAGE_NUM, RS_LAST_PAGE, RS_PROG_PARITY, RS_OTFENC_ENABLE. XOR_OTFENC_ENABLE]{.underline}

**0x00,0x01,0x0,0x0,0x1,0x1**

**0x00,0x02,0x0,0x0,0x1,0x1**

**0x00,0x03,0x0,0x0,0x1,0x1**

**0x00,0x04,0x0,0x0,0x1,0x1**

**0x00,0x05,0x0,0x0,0x1,0x1**

**0x00,0x06,0x0,0x0,0x1,0x1**

**0x00,0x07,0x0,0x0,0x1,0x1**

**0x00,0x08,0x0,0x0,0x1,0x1**

**0x00,0x09,0x0,0x0,0x1,0x1**

**0x00,0x0A,0x0,0x0,0x1,0x1**

**0x00,0x0B,0x0,0x0,0x1,0x1**

**0x00,0x0C,0x0,0x0,0x1,0x1**

**0x00,0x0D,0x0,0x0,0x1,0x1**

**0x00,0x0E,0x0,0x0,0x1,0x1**

**0x00,0x0F,0x0,0x0,0x1,0x1**

**0x00,0x10,0x0,0x0,0x1,0x1**

**0x00,0x11,0x0,0x0,0x1,0x1**

**0x00,0x12,0x0,0x0,0x1,0x1**

**0x00,0x13,0x0,0x0,0x1,0x1**

**0x00,0x14,0x0,0x0,0x1,0x1**

**0x00,0x15,0x0,0x0,0x1,0x1**

**0x00,0x16,0x0,0x0,0x1,0x1**

**0x00,0x17,0x0,0x0,0x1,0x1**

**0x00,0x18,0x0,0x0,0x1,0x1**

**0x00,0x19,0x0,0x0,0x1,0x1**

**0x00,0x1A,0x0,0x0,0x1,0x1**

**0x00,0x1B,0x0,0x0,0x1,0x1**

**0x00,0x1C,0x0,0x0,0x1,0x1**

**0x00,0x1D,0x0,0x0,0x1,0x1**

**0x00,0x1E,0x0,0x0,0x1,0x1**

**0x00,0x1F,0x0,0x0,0x1,0x1**

**0x00,0x20,0x0,0x0,0x1,0x1**

**0x00,0x21,0x0,0x0,0x1,0x1**

**0x00,0x22,0x0,0x0,0x1,0x1**

**0x00,0x23,0x0,0x0,0x1,0x1**

**0x00,0x24,0x0,0x0,0x1,0x1**

**0x00,0x25,0x0,0x0,0x1,0x1**

**0x00,0x26,0x0,0x0,0x1,0x1**

**0x00,0x27,0x0,0x0,0x1,0x1**

**0x00,0x28,0x0,0x0,0x1,0x1**

**0x00,0x29,0x0,0x0,0x1,0x1**

**0x00,0x2A,0x0,0x0,0x1,0x1**

**0x00,0x2B,0x0,0x0,0x1,0x1**

**0x00,0x2C,0x0,0x0,0x1,0x1**

**0x00,0x2D,0x0,0x0,0x1,0x1**

**0x00,0x2E,0x0,0x0,0x1,0x1**

**0x00,0x2F,0x0,0x0,0x1,0x1**

**0x00,0x30,0x0,0x0,0x1,0x1**

**0x00,0x31,0x0,0x0,0x1,0x1**

**0x00,0x32,0x0,0x0,0x1,0x1**

**0x00,0x33,0x0,0x0,0x1,0x1**

**0x00,0x34,0x0,0x0,0x1,0x1**

**0x00,0x35,0x0,0x0,0x1,0x1**

**0x00,0x36,0x0,0x0,0x1,0x1**

**0x00,0x37,0x0,0x0,0x1,0x1**

**0x00,0x38,0x0,0x0,0x1,0x1**

**0x00,0x39,0x0,0x0,0x1,0x1**

**0x00,0x3A,0x0,0x0,0x1,0x1**

**0x00,0x3B,0x0,0x0,0x1,0x1**

**0x00,0x3C,0x0,0x0,0x1,0x1**

**0x00,0x3D,0x0,0x0,0x1,0x1**

**0x00,0x3E,0x0,0x0,0x1,0x1**

**0x00,0x3F,0x0,0x0,0x1,0x1**

**0x00,0x40,0x0,0x0,0x1,0x1**

**0x00,0x41,0x0,0x0,0x1,0x1**

**0x00,0x42,0x0,0x0,0x1,0x1**

**0x00,0x43,0x0,0x0,0x1,0x1**

**0x00,0x44,0x0,0x0,0x1,0x1**

**0x00,0x45,0x0,0x0,0x1,0x1**

**0x00,0x46,0x0,0x0,0x1,0x1**

**0x00,0x47,0x0,0x0,0x1,0x1**

**0x00,0x48,0x0,0x0,0x1,0x1**

**0x00,0x49,0x0,0x0,0x1,0x1**

**0x00,0x4A,0x0,0x0,0x1,0x1**

**0x00,0x4B,0x0,0x0,0x1,0x1**

**0x00,0x4C,0x0,0x0,0x1,0x1**

**0x00,0x4D,0x0,0x0,0x1,0x1**

**0x00,0x4E,0x0,0x0,0x1,0x1**

**0x00,0x4F,0x0,0x0,0x1,0x1**

**0x00,0x50,0x0,0x0,0x1,0x1**

**0x00,0x51,0x0,0x0,0x1,0x1**

**0x00,0x52,0x0,0x0,0x1,0x1**

**0x00,0x53,0x0,0x0,0x1,0x1**

**0x00,0x54,0x0,0x0,0x1,0x1**

**0x00,0x55,0x0,0x0,0x1,0x1**

**0x00,0x56,0x0,0x0,0x1,0x1**

**0x00,0x57,0x0,0x0,0x1,0x1**

**0x00,0x58,0x0,0x0,0x1,0x1**

**0x00,0x59,0x0,0x0,0x1,0x1**

**0x00,0x5A,0x0,0x0,0x1,0x1**

**0x00,0x5B,0x0,0x0,0x1,0x1**

**0x00,0x5C,0x0,0x0,0x1,0x1**

**0x00,0x5D,0x0,0x0,0x1,0x1**

**0x00,0x5E,0x0,0x0,0x1,0x1**

**0x00,0x5F,0x0,0x0,0x1,0x1**

**0x00,0x60,0x0,0x0,0x1,0x1**

**0x00,0x61,0x0,0x0,0x1,0x1**

**0x00,0x62,0x0,0x0,0x1,0x1**

**0x00,0x63,0x0,0x0,0x1,0x1**

**0x00,0x64,0x0,0x0,0x1,0x1**

**0x00,0x65,0x0,0x0,0x1,0x1**

**0x00,0x66,0x0,0x0,0x1,0x1**

**0x00,0x67,0x0,0x0,0x1,0x1**

**0x00,0x68,0x0,0x0,0x1,0x1**

**0x00,0x69,0x0,0x0,0x1,0x1**

**0x00,0x6A,0x0,0x0,0x1,0x1**

**0x00,0x6B,0x0,0x0,0x1,0x1**

**0x00,0x6C,0x0,0x0,0x1,0x1**

**0x00,0x6D,0x0,0x0,0x1,0x1**

**0x00,0x6E,0x0,0x0,0x1,0x1**

**0x00,0x6F,0x0,0x0,0x1,0x1**

**0x00,0x70,0x0,0x0,0x1,0x1**

**0x00,0x71,0x0,0x0,0x1,0x1**

**0x00,0x72,0x0,0x0,0x1,0x1**

**0x00,0x73,0x0,0x0,0x1,0x1**

**0x00,0x74,0x0,0x0,0x1,0x1**

**0x00,0x75,0x0,0x0,0x1,0x1**

**0x00,0x76,0x0,0x0,0x1,0x1**

**0x00,0x77,0x0,0x0,0x1,0x1**

**0x00,0x78,0x0,0x0,0x1,0x1**

**0x00,0x79,0x0,0x0,0x1,0x1**

**0x00,0x7A,0x0,0x0,0x1,0x1**

**0x00,0x7B,0x0,0x0,0x1,0x1**

**0x00,0x7C,0x0,0x0,0x1,0x1**

**0x00,0x7D,0x0,0x0,0x1,0x1**

**0x00,0x7E,0x0,0x0,0x1,0x1**

**0x00,0x7F,0x1,0x0,0x1,0x1**

**0x00,0x80,0x0,0x1,0x0,0x1**

1.16 RS decode/correct

For RS decode/correct, FW can select PBUF0 or PBUF1 to be used. Before trigger RS decode/correct, FW needs to check which one is empty and use it. FW needs to set "Force Save" PBUF0 to DRAM if the two PBUFs are all full.

FW needs to ensure that RS decode and RS OTF encode will not occur at the same time.

FW Flow:

(i) Wait for the program instructions in MTQ are all finished and ensure that RS OTF encode is also stop.

(ii) Force Save Parity Puffer 0 to DRAM if the two PBUFs are all full.

(iii) Clear RS_PBUF0_Status

(iv) RS decode and correct.

(v) Issue MTQ and resume RS Tag N for OTF encode.

## 2. Configuration Register Type

  ----------- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  Attribute   Description

  HwInit      Read-only register: Register bits are read-only and cannot be altered by software. Register bits may be initialized by hardware mechanisms such as pin strapping or serial EEPROM.

  RO          Read-only register: Register bits are read-only and cannot be altered by software. Register bits may be initialized by hardware mechanisms such as pin strapping or serial EEPROM.

  RW          Read-Write register: Register bits are read-write and may be either set or cleared by software to the desired state.

  RW1C        Read-only status, Write-1-to-clear status register: Register bits indicate status when read, a set bit indicating a status event may be cleared by writing a 1. Writing a 0 to RW1C bits has no effect.

  RWL         Read-Write or Locked register: Register bits are read-write and may be either set or cleared by software to the desired state when its Lock bit is cleared. A register bit becomes read only after its Lock bit is set.

  RW1CL       Write-1-to-clear status register or Locked register: Register bits are read-write and may be either set or cleared by software to the desired state when its Lock bit is cleared. While Lock bit is set, register bits indicate status when read, a set bit indicating a status event may be cleared by writing a 1. Writing a 0 to RW1CL bits has no effect.

  RsvdP       Reserved and Preserved: Reserved for future RW implementations; software must preserve value read for writes to bits.

  RsvdZ       Reserved and Zero: Reserved for future RW1C implementations; software must use 0 for writes to bits.
  ----------- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

##  3. Configuration Space

+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:-------------------------------:+:------:+
| 31                              | 30                              | 29                              | 28                              | 27                              | 26                              | 25                              | 24                              | 23                              | 22                              | 21                              | 20                              | 19                              | 18                              | 17                              | 16                              | 15                              | 14                              | 13                              | 12                              | 11                              | 10                              | 9                               | 8                               | 7                               | 6                               | 5                               | 4                               | 3                               | 2                               | 1                               | 0                               |        |
+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+---------------------------------+--------+
| RS_MODE_0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | 000h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_MODE_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | 004h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_BP_CONF                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 008h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_TRIG                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       | 00Ch   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_ERA_IDX                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 010h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_IP_CTL                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | 014h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_DBASE_0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 018h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_SBASE_0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 01Ch   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_ROT_START_0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                | 020h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_ROT_LEN_0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  | 024h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_ROT_START_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                | 028h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_ROT_LEN_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  | 02Ch   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_NMODE_IDX_0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                | 038h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_NMODE_IDX_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                | 03Ch   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_TBDMA_ADDR                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 | 040h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_TBDMA_CTL                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  | 044h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_DBASE_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 048h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_SBASE_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 04Ch   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_DBASE_2                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 050h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_SBASE_2                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 054h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_DBASE_3                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 058h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_SBASE_3                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 05Ch   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_TAG_CONFIG_0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 060h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_TAG_CONFIG_1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 064h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_PBUF_CTL                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   | 068h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_PBP_OFFSET                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 | 06Ch   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_TAG_VLD_CTL                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                | 070h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_INTQ_BASE                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  | 074h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_TAG_SEL                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 078h   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_HANG                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       | 07Ch   |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_BPT0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       | 30000h |
|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               |        |
|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 30FECh |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+
| RS_BPT1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       | 31000h |
|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               |        |
|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 31FECh |
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+--------+

3.1 RS Configuration Space

##### 3.1.1 RS_MODE_0 Register (Offset 000h)

Register Index: 000h \~003h

Default Value: 00000000h

Attribute: RO/RW

+:-----:+:------------------------------------------------------------------------------------:+:-------:+:----------:+
| Bit   | Descriptions                                                                         | Default | Attributes |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 31:21 | **Reserved**                                                                         | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 20    | **RS_COR_NORMAL_MODE**                                                               | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Set correct module normal mode                                                       |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 19:17 | **Reserved**                                                                         | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 16    | **RS_BPT_SEL**                                                                       | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Buffer pointer table (BPT) select                                                    |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 15:10 | **Reserved**                                                                         | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 9:8   | **RS_PAGESIZE**                                                                      | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Page size:                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 11: 16k                                                                              |         |            |
|       |                                                                                      |         |            |
|       | 10: 8k                                                                               |         |            |
|       |                                                                                      |         |            |
|       | other: 4k                                                                            |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 7:5   | **Reserved**                                                                         | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| ~~4~~ | **~~RS_SAVE_PBUF_XOR~~**                                                             | ~~0b~~  | ~~RW~~     |
|       |                                                                                      |         |            |
|       | ~~Save PBUF xor result to PPAGE\[0\]~~                                               |         |            |
|       |                                                                                      |         |            |
|       | **~~(RS otf do't use it, keep zero for it)~~**                                       |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 3     | **Reserved**                                                                         | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 2     | **RS_PBUF_INIT**                                                                     | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Trigger enc/dec will clear PBUF                                                      |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| ~~1~~ | **~~RS_PBUF_LOAD~~**                                                                 | ~~0b~~  | ~~RW~~     |
|       |                                                                                      |         |            |
|       | ~~Load PBUF from PPAGE before enc. If RS_PBUF_INIT is set, the PBUF will be clear.~~ |         |            |
|       |                                                                                      |         |            |
|       | **~~(RS otf do't use it, keep zero for it)~~**                                       |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| ~~0~~ | **~~PS_PBUF_SAVE~~**                                                                 | ~~0b~~  | ~~RW~~     |
|       |                                                                                      |         |            |
|       | ~~Save PBUF to PPAGE after enc.~~                                                    |         |            |
|       |                                                                                      |         |            |
|       | **~~(RS otf do't use it, keep zero for it)~~**                                       |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+

##### 3.1.2 RS_MODE_1 Register (Offset 004h)

Register Index: 004h \~007h

Default Value: 007F0000h

Attribute: RO/RW

+:-----:+:-----------------------------------------:+:-------:+:----------:+
| Bit   | Descriptions                              | Default | Attributes |
+-------+-------------------------------------------+---------+------------+
| 31:24 | **Reserved**                              | 0h      | RW         |
+-------+-------------------------------------------+---------+------------+
| 23:16 | **RS_DPAGE_NUM**                          | FDh     | RW         |
|       |                                           |         |            |
|       | Set the number of DPAGE.                  |         |            |
+-------+-------------------------------------------+---------+------------+
| 15:12 | **Reserved**                              | 0b      | RW         |
+-------+-------------------------------------------+---------+------------+
| 11:8  | **RS_NMODE_ERR_IDX_NUM**                  | 0b      | RW         |
|       |                                           |         |            |
|       | Set number of valid NMODE_ERR_IDX (0 - 8) |         |            |
+-------+-------------------------------------------+---------+------------+
| 7:2   | **Reserved**                              | 0b      | RW         |
+-------+-------------------------------------------+---------+------------+
| 1:0   | **RS_ERA_IDX_EN_NUM**                     | 0b      | RW         |
|       |                                           |         |            |
|       | Set number of valid ERA_IDX. (0 - 2)      |         |            |
+-------+-------------------------------------------+---------+------------+

##### 3.1.2 RS_BP_CONF Register (Offset 008h)

Register Index: 008h \~ 00Dh

Default Value: 00000000h

Attribute: RO/RW

+:-----:+:-----------------------------------------------------:+:-------:+:----------:+
| Bit   | Descriptions                                          | Default | Attributes |
+-------+-------------------------------------------------------+---------+------------+
| 31:26 | **Reserved**                                          | 0b      | RW         |
+-------+-------------------------------------------------------+---------+------------+
| 26    | **RS_OFFSET_BYPASS**                                  | 0b      | RW         |
|       |                                                       |         |            |
|       | Bypass offset for decode                              |         |            |
+-------+-------------------------------------------------------+---------+------------+
| 25    | **Reserved**                                          | 0b      | RW         |
+-------+-------------------------------------------------------+---------+------------+
| 24    | **RS_ALL_CONT**                                       | 0b      | RW         |
|       |                                                       |         |            |
|       | All-page-continue mode for buffer pointer.            |         |            |
+-------+-------------------------------------------------------+---------+------------+
| 23:16 | **RS_PAGE_CNT**                                       | 0b      | RW         |
|       |                                                       |         |            |
|       | Number of page for encode/decode(RS OTF encode set 1) |         |            |
+-------+-------------------------------------------------------+---------+------------+
| 9:8   | **RS_COL_IDX**                                        | 0b      | RW         |
|       |                                                       |         |            |
|       | Column index for decode                               |         |            |
+-------+-------------------------------------------------------+---------+------------+
| 7:0   | **RS_ROW_IDX**                                        | 0b      | RW         |
|       |                                                       |         |            |
|       | Row index for encode/decode                           |         |            |
+-------+-------------------------------------------------------+---------+------------+

##### 3.1.3 RS_TRIG Register (Offset 00Ch)

Register Index: 00Ch \~ 00Fh

Default Value: 00000000h

Attribute: RW

+:---------:+:----------------------------------------------:+:-------:+:----------:+
| Bit       | Descriptions                                   | Default | Attributes |
+-----------+------------------------------------------------+---------+------------+
| 31:10     | **Reserved**                                   | 0b      | RO         |
+-----------+------------------------------------------------+---------+------------+
| ~~17:16~~ | **~~RS_TQ_NUM~~**                              | ~~0b~~  | ~~RO~~     |
|           |                                                |         |            |
|           | **~~(RS otf do't use it, keep zero for it)~~** |         |            |
+-----------+------------------------------------------------+---------+------------+
| 15:10     | **Reserved**                                   | 0b      | RO         |
+-----------+------------------------------------------------+---------+------------+
| 9         | **RS_TRIG_TBDMA**                              | 0b      | RW         |
+-----------+------------------------------------------------+---------+------------+
| ~~8~~     | **~~RS_TRIG_BLOCK~~**                          | ~~0b~~  | ~~RW~~     |
|           |                                                |         |            |
|           | **~~(RS otf do't use it, keep zero for it)~~** |         |            |
+-----------+------------------------------------------------+---------+------------+
| 7:6       | **Reserved**                                   | 0b      | RO         |
+-----------+------------------------------------------------+---------+------------+
| 5         | **RS_ENC_BSY**                                 | 0b      | RO         |
|           |                                                |         |            |
|           | The signal will be zero when encode done       |         |            |
+-----------+------------------------------------------------+---------+------------+
| 4         | **RS_TRIG_BSY**                                | 0b      | RO         |
|           |                                                |         |            |
|           | The signal is only used for debug              |         |            |
+-----------+------------------------------------------------+---------+------------+
| 3         | **RS_TRIG_NMODE_WRITE**                        | 0b      | RW         |
|           |                                                |         |            |
|           | The signal will be zero when nmode write done  |         |            |
+-----------+------------------------------------------------+---------+------------+
| 2         | **RS_TRIG_COR**                                | 0b      | RW         |
|           |                                                |         |            |
|           | The signal will be zero when correct done      |         |            |
+-----------+------------------------------------------------+---------+------------+
| 1         | **RS_TRIG_DEC**                                | 0b      | RW         |
|           |                                                |         |            |
|           | The signal will be zero when decode done       |         |            |
+-----------+------------------------------------------------+---------+------------+
| 0         | **RS_TRIG_ENC**                                | 0b      | RW         |
|           |                                                |         |            |
|           | The signal is only used for debug              |         |            |
+-----------+------------------------------------------------+---------+------------+

##### 3.1.4 RS_ERA_IDX Register (Offset 010h)

Register Index: 010h \~ 013h

Default Value: 00000000h

Attribute: RW

  ------- ---------------------------------------- --------- ------------
    Bit                 Descriptions                Default   Attributes

   31:16                **Reserved**                  0b          RW

   15:8               **RS_ERA_IDX_B**                0b          RW

    7:0               **RS_ERA_IDX_A**                0b          RW
  ------- ---------------------------------------- --------- ------------

##### 3.1.5 RS_IP_CTL Register (Offset 014h)

Register Index: 014h \~ 017h

Default Value: 00000000h

Attribute: RW

+:-----:+:-----------------------------------------------------------------------------------------------------------------------------------------------------------:+:-------:+:----------:+
| Bit   | Descriptions                                                                                                                                                | Default | Attributes |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 31    | **RS_ST_CLEAR**                                                                                                                                             | 0b      | RW         |
|       |                                                                                                                                                             |         |            |
|       | Force clear RS module state machine. This bit is for debug and should be set to 0.                                                                          |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 30:19 | **Reserved**                                                                                                                                                | 0b      | RW         |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 18    | **RS_INTQ_EN**                                                                                                                                              | 1b      | RW         |
|       |                                                                                                                                                             |         |            |
|       | 0: disable massage dispatch                                                                                                                                 |         |            |
|       |                                                                                                                                                             |         |            |
|       | 1: enable massage dispatch                                                                                                                                  |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 17    | **RS_TBDMA_INTR_EN**                                                                                                                                        | 0b      | RW         |
|       |                                                                                                                                                             |         |            |
|       | Enable RS TBDMA interrupt. If interrupt is enabled, CPU will receive interrupt signal after operation finish.                                               |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 16    | **RS_INTR_EN**                                                                                                                                              | 0b      | RW         |
|       |                                                                                                                                                             |         |            |
|       | Enable RS operation interrupt (for block, NMODE write, correct, decode). If interrupt is enabled, CPU will receive interrupt signal after operation finish. |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15:8  | **Reserved**                                                                                                                                                | 0b      | RW         |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:6   | **Reserved**                                                                                                                                                | 0b      | RO         |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 5     | **RS_TBDMA_INTR**                                                                                                                                           | 0b      | RW1C       |
|       |                                                                                                                                                             |         |            |
|       | RS trig TBDMA done interrupt                                                                                                                                |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| ~~4~~ | **~~RS_BLOCK_INTR~~**                                                                                                                                       | ~~0b~~  | ~~RW1C~~   |
|       |                                                                                                                                                             |         |            |
|       | ~~RS trig block done interrupt~~                                                                                                                            |         |            |
|       |                                                                                                                                                             |         |            |
|       | **~~(RS otf do't use it, keep zero for it)~~**                                                                                                              |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3     | **RS_NMODE_WRITE_INTR**                                                                                                                                     | 0b      | RW1C       |
|       |                                                                                                                                                             |         |            |
|       | RS trig NMODE write done interrupt                                                                                                                          |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2     | **RS_COR_INTR**                                                                                                                                             | 0b      | RW1C       |
|       |                                                                                                                                                             |         |            |
|       | RS trig correct done interrupt                                                                                                                              |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1     | **RS_DEC_INTR**                                                                                                                                             | 0b      | RW1C       |
|       |                                                                                                                                                             |         |            |
|       | RS trig decode done interrupt                                                                                                                               |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0     | **RS_ENC_INTR**                                                                                                                                             | 0b      | RW1C       |
|       |                                                                                                                                                             |         |            |
|       | RS trig encode done interrupt                                                                                                                               |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

##### 3.1.6 RS_DBASE_0 Register (Offset 018h)

Register Index: 018h \~ 01Bh

Default Value: 00000000h

Attribute: RW

+:----:+:-------------------------------------------------------:+:-------:+:----------:+
| Bit  | Descriptions                                            | Default | Attributes |
+------+---------------------------------------------------------+---------+------------+
| 31:0 | **DBASE_0**                                             | 0b      | RW         |
|      |                                                         |         |            |
|      | Base address for data region, it should be 4KByte align |         |            |
+------+---------------------------------------------------------+---------+------------+

##### 3.1.7 RS_SBASE_0 Register (Offset 01Ch)

Register Index: 01Ch \~ 01Fh

Default Value: 00000000h

Attribute: RW

+:----:+:--------------------------------------------------------:+:-------:+:----------:+
| Bit  | Descriptions                                             | Default | Attributes |
+------+----------------------------------------------------------+---------+------------+
| 31:0 | **SBASE_0**                                              | 0b      | RW         |
|      |                                                          |         |            |
|      | Base address for spare region, it should be 32Byte align |         |            |
+------+----------------------------------------------------------+---------+------------+

##### 3.1.8 RS_ROT_START_0 Register (Offset 020h) (RS on-the fly doesn't use)

##### 3.1.9 RS_ROT_LEN_0 Register (Offset 024h) (RS on-the fly doesn't use)

##### 3.1.10 RS_ROT_START_1 Register (Offset 028h) (RS on-the fly doesn't use)

##### 3.1.11 RS_ROT_LEN_1 Register (Offset 02Ch) (RS on-the fly doesn't use)

##### 3.1.12 RS_NMODE_IDX_0 Register (Offset 038h)

Register Index: 038h \~ 03Bh

Default Value: 00000000h

Attribute: RW

  ------- ---------------------------------------- --------- ------------
    Bit                 Descriptions                Default   Attributes

   31:24           **RS_NMOME_ERR_IDX_3**             0b          RW

   23:16           **RS_NMOME_ERR_IDX_2**             0b          RW

   15:8            **RS_NMOME_ERR_IDX_1**             0b          RW

    7:0            **RS_NMOME_ERR_IDX_0**             0b          RW
  ------- ---------------------------------------- --------- ------------

##### 3.1.13 RS_NMODE_IDX_1 Register (Offset 03Ch)

Register Index: 03Ch \~ 03Fh

Default Value: 00000000h

Attribute: RW

  ------- ---------------------------------------- --------- ------------
    Bit                 Descriptions                Default   Attributes

   31:24           **RS_NMOME_ERR_IDX_7**             0b          RW

   23:16           **RS_NMOME_ERR_IDX_6**             0b          RW

   15:8            **RS_NMOME_ERR_IDX_5**             0b          RW

    7:0            **RS_NMOME_ERR_IDX_4**             0b          RW
  ------- ---------------------------------------- --------- ------------

##### 3.1.14 RS_TBDMA_ADDR Register (Offset 040h)

Register Index: 040h \~ 043h

Default Value: 00000000h

Attribute: RW

+:----:+:------------------------------------------------------------------------------:+:-------:+:----------:+
| Bit  | Descriptions                                                                   | Default | Attributes |
+------+--------------------------------------------------------------------------------+---------+------------+
| 31:0 | **RS_TBDMA_ADDR**                                                              | 0b      | RW         |
|      |                                                                                |         |            |
|      | DRAM addresses for TBDMA save/load data. This address must align to 128 bytes. |         |            |
+------+--------------------------------------------------------------------------------+---------+------------+

##### 3.1.15 RS_TBDMA_CTL Register (Offset 044h)

Register Index: 044h \~ 047h

Default Value: 00000000h

Attribute: RW

+:-----:+:-------------------------------------------------------------------------------------------------------------:+:-------:+:----------:+
| Bit   | Descriptions                                                                                                  | Default | Attributes |
+-------+---------------------------------------------------------------------------------------------------------------+---------+------------+
| 31:24 | **Reserved**                                                                                                  | 0b      | RW         |
+-------+---------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:13 | **Reserved**                                                                                                  | 0b      | RW         |
+-------+---------------------------------------------------------------------------------------------------------------+---------+------------+
| 12    | **RS_TBDMA_WR**                                                                                               | 0b      | RW         |
|       |                                                                                                               |         |            |
|       | Set TBDMA direction.                                                                                          |         |            |
|       |                                                                                                               |         |            |
|       | 1b: Save table data to DRAM                                                                                   |         |            |
|       |                                                                                                               |         |            |
|       | 0b: Load table data from DRAM                                                                                 |         |            |
+-------+---------------------------------------------------------------------------------------------------------------+---------+------------+
| 11:10 | **Reserved**                                                                                                  | 0b      | RW         |
+-------+---------------------------------------------------------------------------------------------------------------+---------+------------+
| 9:8   | **RS_TBDMA_SEL**                                                                                              | 0b      | RW         |
|       |                                                                                                               |         |            |
|       | Select table to transfer data.                                                                                |         |            |
|       |                                                                                                               |         |            |
|       | 01b: BPT1                                                                                                     |         |            |
|       |                                                                                                               |         |            |
|       | 00b: BPT0                                                                                                     |         |            |
+-------+---------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:6   | **Reserved**                                                                                                  | 0b      | RW         |
+-------+---------------------------------------------------------------------------------------------------------------+---------+------------+
| 5:0   | **RS_TBDMA_LEN**                                                                                              | 0b      | RW         |
|       |                                                                                                               |         |            |
|       | Set transfer data length. The length unit is 128B. Ex. To transfer 1KB data, this register should set to 08h. |         |            |
+-------+---------------------------------------------------------------------------------------------------------------+---------+------------+

##### 3.1.16 RS_DBASE_1 Register (Offset 048h) (RS on-the fly doesn't use)

##### 3.1.17 RS_SBASE_1 Register (Offset 04Ch) (RS on-the fly doesn't use)

##### 3.1.18 RS_DBASE_2 Register (Offset 050h) (RS on-the fly doesn't use)

##### 3.1.19 RS_SBASE_2 Register (Offset 054h) (RS on-the fly doesn't use)

##### 3.1.20 RS_DBASE_3 Register (Offset 058h) (RS on-the fly doesn't use)

##### 3.1.21 RS_SBASE_3 Register (Offset 05Ch) (RS on-the fly doesn't use)

##### 3.1.22 RS_TAG_CONFIG_0 (Offset 060h)

Register Index: 060h \~ 063h

Default Value: 00000000h

Attribute: RW

+:-----:+:-------------------------------------------------:+:-------:+:----------:+
| Bit   | Descriptions                                      | Default | Attributes |
+-------+---------------------------------------------------+---------+------------+
| 31:24 | **Reserved**                                      | 0b      | RW         |
+-------+---------------------------------------------------+---------+------------+
| 23:16 | **RS_TAG_PEC**                                    | 0b      | RW         |
|       |                                                   |         |            |
|       | Parity Encode Count (PEC) of selected RS tag.     |         |            |
+-------+---------------------------------------------------+---------+------------+
| 15:8  | **RS_TAG_PRC**                                    | 0b      | RW         |
|       |                                                   |         |            |
|       | Parity Ready Count (PRC) of selected RS tag.      |         |            |
+-------+---------------------------------------------------+---------+------------+
| 7     | **RS_TAG_VALID**                                  | 0b      | RO         |
|       |                                                   |         |            |
|       | Valid bit of selected RS tag.                     |         |            |
+-------+---------------------------------------------------+---------+------------+
| 6     | **RS_TAG_PPF**                                    | 0b      | RO         |
|       |                                                   |         |            |
|       | Program Parity Flag (PPF) of selected RS tag.     |         |            |
+-------+---------------------------------------------------+---------+------------+
| 5     | **RS_TAG_PRF**                                    | 0b      | RO         |
|       |                                                   |         |            |
|       | Parity Ready Flag (PRF) of selected RS tag.       |         |            |
+-------+---------------------------------------------------+---------+------------+
| 4:0   | **RS_TAG_SEL**                                    | 0b      | RO         |
|       |                                                   |         |            |
|       | RS tag select.                                    |         |            |
|       |                                                   |         |            |
|       | Example:                                          |         |            |
|       |                                                   |         |            |
|       | If RS_TAG_SEL = 4'd0, RS_TAG0 will be selected.   |         |            |
|       |                                                   |         |            |
|       | If RS_TAG_SEL = 4'd1, RS_TAG1 will be selected.   |         |            |
|       |                                                   |         |            |
|       | .....                                             |         |            |
|       |                                                   |         |            |
|       | If RS_TAG_SEL = 4'd31, RS_TAG15 will be selected. |         |            |
+-------+---------------------------------------------------+---------+------------+

##### 3.1.23 RS_TAG_CONFIG_1 (Offset 064h)

Register Index: 064h \~ 067h

Default Value: 00000000h

Attribute: RW

+:----:+:-----------------------------------------------:+:-------:+:----------:+
| Bit  | Descriptions                                    | Default | Attributes |
+------+-------------------------------------------------+---------+------------+
| 31:0 | **RS_TAG_PBP**                                  | 0b      | RO         |
|      |                                                 |         |            |
|      | Parity Buffer Pointer (PBP) of selected RS tag. |         |            |
+------+-------------------------------------------------+---------+------------+

##### 3.1.24 RS_PBUF_CTL Register (Offset 068h)

Register Index: 068h \~ 06Bh

Default Value: 00000000h

Attribute: RW

+:-----:+:--------------------------------------------------------------------------------------------------------------------:+:-------:+:----------:+
| Bit   | Descriptions                                                                                                         | Default | Attributes |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 31:28 | **Reserved**                                                                                                         | 0b      | RW         |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27    | **PRE_LOAD_EN**                                                                                                      | 1b      | RW         |
|       |                                                                                                                      |         |            |
|       | 0 : only disable PRE_LOAD                                                                                            |         |            |
|       |                                                                                                                      |         |            |
|       | 1 : enable PRE_LOAD                                                                                                  |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 26    | **PRE_SAVE_EN**                                                                                                      | 1b      | RW         |
|       |                                                                                                                      |         |            |
|       | 0 : only disable PRE_SAVE                                                                                            |         |            |
|       |                                                                                                                      |         |            |
|       | 1 : enable PRE_SAVE                                                                                                  |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 25    | **PRE_SL_EN**                                                                                                        | 0b      | RW         |
|       |                                                                                                                      |         |            |
|       | 0 : disable PRE_SAVE_LOAD                                                                                            |         |            |
|       |                                                                                                                      |         |            |
|       | 1 : enable PRE_SAVE_LOAD                                                                                             |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 24    | **FORCE_E3D_ZERO**                                                                                                   | 1b      | RW         |
|       |                                                                                                                      |         |            |
|       | 0 : No force E3D                                                                                                     |         |            |
|       |                                                                                                                      |         |            |
|       | 1 : Force E3D data to zero when AXI read dram                                                                        |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:20 | **Reserved**                                                                                                         | 0b      | RW         |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 19    | **FORCE_SAVE_RS_PBUF1**                                                                                              | 0b      | RW         |
|       |                                                                                                                      |         |            |
|       | Force save PBUF1 data to DRAM.                                                                                       |         |            |
|       |                                                                                                                      |         |            |
|       | Write 1 to force save PBUF1 data to DRAM. Auto clear when PBUF1 data is already save to DRAM.                        |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 18    | **FORCE_SAVE_RS_PBUF0**                                                                                              | 0b      | RW         |
|       |                                                                                                                      |         |            |
|       | Force save PBUF0 data to DRAM.                                                                                       |         |            |
|       |                                                                                                                      |         |            |
|       | Write 1 to force save PBUF0 data to DRAM. Auto clear when PBUF0 data is already save to DRAM.                        |         |            |
|       |                                                                                                                      |         |            |
|       | The address for the parity of two pages is mapping to (RS_DPAGE_NUM + 1) and (RS_DPAGE_NUM + 2)                      |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 17    | **LOCK_RS_PBUF1**                                                                                                    | 0b      | RW         |
|       |                                                                                                                      |         |            |
|       | 0 : PBUF1 is unlocked.                                                                                               |         |            |
|       |                                                                                                                      |         |            |
|       | 1 : PBUF1 is locked. PBUF1 will not be used. The register can't be turned on when encode and PRE_SL_EN is turned on. |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 16    | **LOCK_RS_PBUF0**                                                                                                    | 0b      | RW         |
|       |                                                                                                                      |         |            |
|       | 0 : PBUF0 is unlocked.                                                                                               |         |            |
|       |                                                                                                                      |         |            |
|       | 1 : PBUF0 is locked. PBUF0 will not be used. The register can't be turned on when encode and PRE_SL_EN is turned on. |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15    | **RS_PBUF1_Status**                                                                                                  | 0b      | RW         |
|       |                                                                                                                      |         |            |
|       | 0: PBUF1 is empty                                                                                                    |         |            |
|       |                                                                                                                      |         |            |
|       | 1: PBUF1 is occupied                                                                                                 |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 14:13 | **Reserved**                                                                                                         | 0b      | RW         |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 12:8  | **RS_PBUF1_RS_TAG_NUM**                                                                                              | 0b      | RW         |
|       |                                                                                                                      |         |            |
|       | RS_PBUF1_RS_TAG_NUM indicates the RS tag number which PUBF1 is mapping..                                             |         |            |
|       |                                                                                                                      |         |            |
|       | Only valid when RS_PBUF1_Status is 1.                                                                                |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7     | **RS_PBUF0_Status**                                                                                                  | 0b      | RW         |
|       |                                                                                                                      |         |            |
|       | 0: PBUF0 is empty                                                                                                    |         |            |
|       |                                                                                                                      |         |            |
|       | 1: PBUF0 is occupied                                                                                                 |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 6:5   | **Reserved**                                                                                                         | 0b      | RW         |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+
| 4:0   | **RS_PBUF0_RS_TAG_NUM**                                                                                              | 0b      | RW         |
|       |                                                                                                                      |         |            |
|       | RS_PBUF0_RS_TAG_NUM indicates the RS tag number which PUBF0 is mapping..                                             |         |            |
|       |                                                                                                                      |         |            |
|       | Only valid when RS_PBUF0_Status is 1.                                                                                |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------+---------+------------+

##### 3.1.25 RS_PBP_OFFSET Register (Offset 06Ch)

Register Index: 06Ch \~ 06Fh

Default Value: 00000000h

Attribute: RW

+:----:+:-----------------------------------------------------:+:-------:+:----------:+
| Bit  | Descriptions                                          | Default | Attributes |
+------+-------------------------------------------------------+---------+------------+
| 31:0 | **RS_PBP_OFFSET**                                     | 8000h   | RW         |
|      |                                                       |         |            |
|      | Offset for Parity buffer pointer for different RS TAG |         |            |
+------+-------------------------------------------------------+---------+------------+

##### 3.1.26 RS_TAG_VLD_CTL Register (Offset 070h)

Register Index: 070h \~ 073h

Default Value: 00000000h

Attribute: RW

+:----:+:-------------------------------------------------------------------:+:-------:+:----------:+
| Bit  | Descriptions                                                        | Default | Attributes |
+------+---------------------------------------------------------------------+---------+------------+
| 31   | **PRE_SAVE_LOAD_BUSY**                                              | 0b      | RO         |
|      |                                                                     |         |            |
|      | 0: RS is not busy                                                   |         |            |
|      |                                                                     |         |            |
|      | 1: RS is busy for save and load                                     |         |            |
+------+---------------------------------------------------------------------+---------+------------+
| 30:9 | **Reserved**                                                        | 0b      | RW         |
+------+---------------------------------------------------------------------+---------+------------+
| 8    | **RS_TAG_VALID_CLR**                                                | 0b      | RW1C       |
|      |                                                                     |         |            |
|      | Valid bit of selected RS tag.                                       |         |            |
|      |                                                                     |         |            |
|      | Write 1 to clear **RS_TAG_VALID** to 0.                             |         |            |
|      |                                                                     |         |            |
|      | If want to give up the RS tag, use the register to clear the VALID. |         |            |
+------+---------------------------------------------------------------------+---------+------------+
| 7:0  | **Reserved**                                                        | 0b      | RW         |
+------+---------------------------------------------------------------------+---------+------------+

##### 3.1.27 RS_INTQ_BASE Register (Offset 074h)

Register Index: 074h \~ 077h

Default Value: 00000000h

Attribute: RW

+:----:+:------------------------------------------------:+:-------:+:----------:+
| Bit  | Descriptions                                     | Default | Attributes |
+------+--------------------------------------------------+---------+------------+
| 31:0 | **RS_INTQ_BASE**                                 | 0b      | RW         |
|      |                                                  |         |            |
|      | The based address for sent interrupt to system . |         |            |
+------+--------------------------------------------------+---------+------------+

##### 3.1.28 RS_TAG_SEL Register (Offset 078h)

Register Index: 078h \~ 07Bh

Default Value: 00000000h

Attribute: RW

+:----:+:-------------------------------------------------:+:-------:+:----------:+
| Bit  | Descriptions                                      | Default | Attributes |
+------+---------------------------------------------------+---------+------------+
| 31:5 | **Reserved**                                      | 0b      | RW         |
+------+---------------------------------------------------+---------+------------+
| 4:0  | **RS_TAG_SEL**                                    | 0b      | RW         |
|      |                                                   |         |            |
|      | RS tag select.                                    |         |            |
|      |                                                   |         |            |
|      | Example:                                          |         |            |
|      |                                                   |         |            |
|      | If RS_TAG_SEL = 4'd0, RS_TAG0 will be selected.   |         |            |
|      |                                                   |         |            |
|      | If RS_TAG_SEL = 4'd1, RS_TAG1 will be selected.   |         |            |
|      |                                                   |         |            |
|      | .....                                             |         |            |
|      |                                                   |         |            |
|      | If RS_TAG_SEL = 4'd31, RS_TAG15 will be selected. |         |            |
+------+---------------------------------------------------+---------+------------+

##### 3.1.29 RS_HANG Register (Offset 07Ch)

Register Index: 07Ch \~ 07Fh

Default Value: 00000000h

Attribute: RW

+:-----:+:-----------------------------------------------------------------:+:-------:+:----------:+
| Bit   | Descriptions                                                      | Default | Attributes |
+-------+-------------------------------------------------------------------+---------+------------+
| 30:27 | **Reserved**                                                      | 0b      | RW         |
+-------+-------------------------------------------------------------------+---------+------------+
| 26:24 | **RS_HANG**                                                       | 0b      | RO         |
|       |                                                                   |         |            |
|       | **The debug register is used when FLH hang**                      |         |            |
|       |                                                                   |         |            |
|       | bit0: RS hang at internal axi                                     |         |            |
|       |                                                                   |         |            |
|       | bit1: RS hang at pre save                                         |         |            |
|       |                                                                   |         |            |
|       | bit2: RS hang at pre load                                         |         |            |
+-------+-------------------------------------------------------------------+---------+------------+
| 23:1  | **Reserved**                                                      | 0b      | RW         |
+-------+-------------------------------------------------------------------+---------+------------+
| 0     | **RS_TAG_VALID_SET**                                              | 0b      | RW         |
|       |                                                                   |         |            |
|       | Write value to set **RS_TAG_VALID** of selected RS tag.           |         |            |
|       |                                                                   |         |            |
|       | If want to restart the RS tag, use the register to set the VALID. |         |            |
+-------+-------------------------------------------------------------------+---------+------------+

3.2 RS BPT Space

BPT contains all BPs for 255 pages. Please refer to section 1.6 for BPT usage. The table layout is as follows.

  ---------------- ---------------- ---------------- ---------------- ------
      127 : 96         95 : 64          63 : 32           31 : 0

    BP\[0\]\[3\]     BP\[0\]\[2\]     BP\[0\]\[1\]     BP\[0\]\[0\]    000h

    BP\[1\]\[3\]     BP\[1\]\[2\]     BP\[1\]\[1\]     BP\[1\]\[0\]    010h

    BP\[2\]\[3\]     BP\[2\]\[2\]     BP\[2\]\[1\]     BP\[2\]\[0\]    020h

    BP\[3\]\[3\]     BP\[3\]\[2\]     BP\[3\]\[1\]     BP\[3\]\[0\]    030h

                                                                       \...

                                                                       \...

                                                                       \...

   BP\[251\]\[3\]   BP\[251\]\[2\]   BP\[251\]\[1\]   BP\[251\]\[0\]   FB0h

   BP\[252\]\[3\]   BP\[252\]\[2\]   BP\[252\]\[1\]   BP\[252\]\[0\]   FC0h

   BP\[253\]\[3\]   BP\[253\]\[2\]   BP\[253\]\[1\]   BP\[253\]\[0\]   FD0h

   BP\[254\]\[3\]   BP\[254\]\[2\]   BP\[254\]\[1\]   BP\[254\]\[0\]   FE0h
  ---------------- ---------------- ---------------- ---------------- ------

3.3 RS BSEQ Space (removed BSEQ)

3.4 RS OTF Encode - Parity Parameter Management Table (PPMT) setting

By setting control register, RS_TAG_SEL for the corresponding RS_TAG, FW can set/get the parameters/status of the selected RS_TAG.

+:---------------------------:+:--------------:+:----------:+:-----------------:+:------------------:+:--------------:+:--------------:+
| **RS_TAG_SEL (60h\[3:0\])** | **Valid**      | **PBPT**   | **PRC**           | **PEC**            | **PRF**        | **PPF**        |
|                             |                |            |                   |                    |                |                |
|                             | **(60h\[4\])** | **(64h)**  | **(60h\[11:8\])** | **(60h\[15:12\])** | **(60h\[5\])** | **(60h\[6\])** |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **0 (RS TAG 0)**            | Valid\[0\]     | PBPT\[0\]  | PRC\[0\]          | PEC\[0\]           | PRF\[0\]       | PPF\[0\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **1 (RS TAG 1)**            | Valid\[1\]     | PBPT\[1\]  | PRC\[1\]          | PEC\[1\]           | PRF\[1\]       | PPF\[1\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **2 (RS TAG 2)**            | Valid\[2\]     | PBPT\[2\]  | PRC\[2\]          | PEC\[2\]           | PRF\[2\]       | PPF\[2\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **3 (RS TAG 3)**            | Valid\[3\]     | PBPT\[3\]  | PRC\[3\]          | PEC\[3\]           | PRF\[3\]       | PPF\[3\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **4 (RS TAG 4)**            | Valid\[4\]     | PBPT\[4\]  | PRC\[4\]          | PEC\[4\]           | PRF\[4\]       | PPF\[4\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **5 (RS TAG 5)**            | Valid\[5\]     | PBPT\[5\]  | PRC\[5\]          | PEC\[5\]           | PRF\[5\]       | PPF\[5\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **6 (RS TAG 6)**            | Valid\[6\]     | PBPT\[6\]  | PRC\[6\]          | PEC\[6\]           | PRF\[6\]       | PPF\[6\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **7 (RS TAG 7)**            | Valid\[7\]     | PBPT\[7\]  | PRC\[7\]          | PEC\[7\]           | PRF\[7\]       | PPF\[7\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **8 (RS TAG 8)**            | Valid\[8\]     | PBPT\[8\]  | PRC\[8\]          | PEC\[8\]           | PRF\[8\]       | PPF\[8\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **9 (RS TAG 9)**            | Valid\[9\]     | PBPT\[9\]  | PRC\[9\]          | PEC\[9\]           | PRF\[9\]       | PPF\[9\]       |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **10 (RS TAG 10)**          | Valid\[10\]    | PBPT\[10\] | PRC\[10\]         | PEC\[10\]          | PRF\[10\]      | PPF\[10\]      |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **11 (RS TAG 11)**          | Valid\[11\]    | PBPT\[11\] | PRC\[11\]         | PEC\[11\]          | PRF\[11\]      | PPF\[11\]      |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **12 (RS TAG 12)**          | Valid\[12\]    | PBPT\[12\] | PRC\[12\]         | PEC\[12\]          | PRF\[12\]      | PPF\[12\]      |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **13 (RS TAG 13)**          | Valid\[13\]    | PBPT\[13\] | PRC\[13\]         | PEC\[13\]          | PRF\[13\]      | PPF\[13\]      |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **14 (RS TAG 14)**          | Valid\[14\]    | PBPT\[14\] | PRC\[14\]         | PEC\[14\]          | PRF\[14\]      | PPF\[14\]      |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+
| **15 (RS TAG 15)**          | Valid\[15\]    | PBPT\[15\] | PRC\[15\]         | PEC\[15\]          | PRF\[15\]      | PPF\[15\]      |
+-----------------------------+----------------+------------+-------------------+--------------------+----------------+----------------+

##  4. Limitation

4.1 RS Page Size

The RS module supports at most 253 DPAGEs/2 PPAGEs and can recover 2 page data.

![](D:\AI_DEV\Output\PS3111_RegSpec_RS_assets/media/image31.png){width="3.859722222222222in" height="2.136111111111111in"}

4.2 RS Buffer Pointer

Each buffer pointer in BPT and base address must align to 4K address boundary.

4.3 TBDMA Address

The TBDMA target address (RS_TBDMA_ADDR) must align to 128B boundary and must in DRAM Zone, share buffer (SBUF) or DCCM.

4.4 Base Address

In BP no continue mode, one page data must has the same base address. For a 16K page, its 4 BPs must select the same base address (the BASE_SEL values are the same). The BPs of page k are BP\[k\]\[0\], BP\[k\]\[1\], BP\[k\]\[2\] and BP\[k\]\[3\]. The BASE_SEL of these BPs must be the same.

4.5 Rotation (RS on-the fly doesn't use)

## 5. Appendix

5.1 XOR Page (RS on-the fly doesn't use)

5.2 MT Parameter for RS On-the-fly Encode

+-----+---------------------------+-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+
| No. | Category                  | Name                    | Bit width | comment                                                                                                                           |
+-----+---------------------------+-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+
| 1   | RS OTF Encode information | RS_OTFENC_RS_TAG        | \[7:0\]   | Indicates the RS tag number                                                                                                       |
+-----+                           +-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+
| 2   |                           | RS_OTFENC_RS_PAGE_IDX   | \[7:0\]   | Indicates the RS page index                                                                                                       |
+-----+                           +-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+
| 3   |                           | RS_OTFENC_LAST_PAGE     |           | Indicates the DPAGE value of RS tag                                                                                               |
+-----+                           +-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+
| 4   |                           | RS\_ OTFENC_DIS_PAR_CNT |           | Indicates whether this instruction clear part of the parity or not.                                                               |
+-----+---------------------------+-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+
| 5   | RS OTF Encode control     | RS_OTFENC_PROG_PARITY   |           | Indicates whether this instruction is program parity or not. Need to turn off RS_OTFENC_ENABLE when turn on RS_OTFENC_PROG_PARITY |
+-----+                           +-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+
| 6   |                           | RS\_ PROG_2ND_PARITY    |           | Indicates whether this instruction is second program parity or not                                                                |
+-----+                           +-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+
| 7   |                           | RS_OTFENC_ENABLE        |           | Indicates whether this MT instruction needs to do RS encode or not.                                                               |
+-----+---------------------------+-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+
| 8   | XOR OTF Encode control    | XOR_OTFENC_ENABLE       |           | Indicates whether this MT instruction needs to do XOR encode or RS encode.                                                        |
+-----+---------------------------+-------------------------+-----------+-----------------------------------------------------------------------------------------------------------------------------------+

5.2 BSEQ for TLC block (removed BSEQ)

5.3 Pre-save and pre-load

At case 1, RS will monitor MTQ's RS-OTF enable and RS tag when the status of PBUF0 and PBUF1 are all not empty. If RS tag of PBUF is not exist at MTQ at case1, RS engine will do save data from PBUF to DRAM.

Ex for case 1:

CH0 CH1

MTQ0 MTQ1 MTQ2 MTQ3 MTQ0 MTQ1 MTQ2 MTQ3

10 10 10 10 10 10 10 10 {RS enable, RS tag}

10 10 10 10 10 10 10 11 FLH arbiter select RS tag0, PBUF0 is used by Tag0, PBUF1 is empty

10 10 10 10 10 10 11 11

10 10 10 10 10 11 11 11

10 10 10 10 11 11 11 11

10 10 10 11 11 11 11 11

10 10 11 11 11 11 11 11

10 11 11 11 11 11 11 11

11 11 11 11 11 11 11 11 FLH arbiter select RS tag0, Tag0 isn't exist at MTQ, but PBUF1 is empty, so don't do pre-save.

11 11 11 11 11 11 11 11 FLH arbiter select RS tag1, PBUF0 and PBUF1 are not empty and Tag0 isn't exist at MTQ, RS engine do pre-save PBUF0.

At case 2, RS will monitor MTQ's RS-OTF enable and RS tag when the status of PBUF0 or PBUF1 are empty and PBUF0 and PBUF1 are not all empty. If RS tag of MTQ is not exist at PUBF at case2, RS engine will do load data to PBUF from DRAM. For case2.

Ex for case 2:

CH0 CH1

MTQ0 MTQ1 MTQ2 MTQ3 MTQ0 MTQ1 MTQ2 MTQ3

10 10 10 10 10 10 10 10 {RS enable, RS tag}, PBUF0 and PBUF1 are all empty, so don't do pre-load

10 10 10 10 10 10 10 11 FLH arbiter select RS tag0, PBUF0 is used by Tag0, PBUF1 is empty, RS engine do pre-load Tag1 to PBUF1

10 10 10 10 10 10 11 11

10 10 10 10 10 11 11 11

10 10 10 10 11 11 11 11

10 10 10 11 11 11 11 11

10 10 11 11 11 11 11 11

10 11 11 11 11 11 11 11

11 11 11 11 11 11 11 11

11 11 11 11 11 11 11 11

FW can set register to turn off this function, FW need to turn off the register before RS decode and correct.

5.4 Interrupt for program parity done

Use RS_INTQ_BASE to set the based address of system RAM.

RS's interrupt for program parity done :

  ----------- ------------ -------- ----------- -------- ------------
  36          35           34:25    24          23:9     3:0

  NA          INDEX_MODE   NA       PROG_DONE   NA       RS_TAG/IDX
  ----------- ------------ -------- ----------- -------- ------------
