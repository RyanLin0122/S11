Topic: Register Space of FLHC Engine

**Version:** 2.9

**Date:** , 2014

**by**:

##

Revision History

+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| > **Date** | > **Revision Number** | > **Author**                  | > **Description**                                                                                                                                                                     |
+============+=======================+===============================+=======================================================================================================================================================================================+
| 2014/06/09 | v1.0                  | CY, Matt Wu, YHWang           | Add LDPC, ZIP, and RS on-the-fly registers                                                                                                                                            |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | V1.1                  | CY, Matt Wu, YHWang, YH Huang | 1.  Add [FCON_PAGE_CFG](#FCON_PAGE_CFG)                                                                                                                                               |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Add [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)                                                                                                                                         |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Add [FCON_SYS_INTQ_BASE](#FCON_SYS_INTQ_BASE)                                                                                                                                     |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Add [FCTL_RD_CNT](#FCTL_RD_CNT), [FCTL_RD2_CNT](#FCTL_RD2_CNT)                                                                                                                    |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 5.  Add [FCTL_DYNAMIC_ODT](#FCTL_DYNAMIC_ODT)                                                                                                                                         |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | V1.2                  |                               | 1.  Modify flash DMA format                                                                                                                                                           |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Add [FCON_FLH_FUNC](#FCON_FLH_FUNC)\[28\],\[4\]                                                                                                                                   |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | V1.3                  |                               | 1.  Update MT table                                                                                                                                                                   |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Remove redundant registers                                                                                                                                                        |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | V1.4                  |                               | 1.  Remove 0xF0 FCTL_RD_CNT                                                                                                                                                           |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Fix MT address index mode example FCTL_MT_ADR\_\*                                                                                                                                 |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Redefine [FCTL_MAP_CFG](#FCTL_MAP_CFG)                                                                                                                                            |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Modify [FCTL_INT_CFG](#FCTL_INT_CFG)                                                                                                                                              |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 5.  Add [FCTL_INT_CFG_1](#FCTL_INT_CFG_1)                                                                                                                                             |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 6.  Modify [MT TABLE](#MT_TABLE)                                                                                                                                                      |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 7.  Redefine [FCTL_UNC_CFG](#FCTL_UNC_CFG)                                                                                                                                            |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 8.  Redefine [FCTL_RS_CFG](#FCTL_RS_CFG)                                                                                                                                              |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 9.  Add [FCON_MT_CFG](#FCON_MT_CFG)                                                                                                                                                   |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 10. Add [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[21:18\]                                                                                                                                |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 11. Modify [FCTL_BDC_SET](#FCTL_BDC_SET)                                                                                                                                              |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 12. Modify [FCTL_INT_VCT](#FCTL_INT_VCT)                                                                                                                                              |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 13. Add [FCON_MTQ_INF\_\*](#FCON_MTQ_INF_0)                                                                                                                                           |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 14. Add [FCON_MT_STS](#FCON_MT_STS)                                                                                                                                                   |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 15. Remove [FCON_IRC_ADR_1](#FCON_IRC_ADR_1)                                                                                                                                          |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 1.5                   |                               | 1.  Modify IRC transfer unit from 64bytes to MT_TABLE_SIZE                                                                                                                            |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Add Initial IRAM function to FCON_IRC_CTL\[8\]                                                                                                                                    |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Combine FCON_AXIS_ADR\* to [FCON_AXIS_ADR](#FCON_AXIS_ADR)                                                                                                                        |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Modify [FCON_SPECIAL](#FCON_SPECIAL)                                                                                                                                              |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 5.  Add FCON_ERASE_PAG                                                                                                                                                                |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 1.6                   |                               | 1.  Add chapter [4.24 Read nand Flash soft data](#read-nand-flash-soft-data)                                                                                                          |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | [4.25 Backup-Restore IBF](#backup-restore-ibf)                                                                                                                                        |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Modify [FCTL_BACK_STORE](#FCTL_BACK_RESTORE)                                                                                                                                      |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Add STA CPU interrupt enable to [FCTL_INT_CFG_1](#FCTL_INT_CFG_1)                                                                                                                 |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Modify MT trigger bit format [FCON_MT_CFG](#FCON_MT_CFG)                                                                                                                          |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 5.  Add PFA_FUNCTION_ENABLE to [FCON_SPECIAL\[15\]](#FCON_SPECIAL)                                                                                                                    |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 6.  Add auto generate UNC to [FCTL_UNC_CFG\[16\]](#FCTL_UNC_CFG)                                                                                                                      |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 7.  Add auto generate program fail to [FCTL_UNC_CFG\[17\]](#FCTL_UNC_CFG)                                                                                                             |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 8.  Add RS_ONE_PARITY_EN to [FCTL_RS_CFG\[24\]](#FCTL_RS_CFG)                                                                                                                         |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 1.7                   |                               | 1.  Add [internal FSA function](#address-generation) and modify register                                                                                                              |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | [FCTL_FSA_SEL](#FCTL_FSA_SEL)\[18\], [FCTL_IFSA\*](#FCTL_IFSA0)                                                                                                                       |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Modify MT TABLE                                                                                                                                                                   |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Modify [FCON_SPECIAL](#FCON_SPECIAL)                                                                                                                                              |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Add FCON\_ FLHCLK_SPLITE_EN                                                                                                                                                       |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 1.8                   |                               | 1.  Add force_fix_data function(MT TABLE, FCTL_OTHER \_SET\[8:0\])                                                                                                                    |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Add l4k_badr_cnti_mode_en into MT table                                                                                                                                           |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 1.9                   |                               | 1.  Add FCTL_IBF_CTL                                                                                                                                                                  |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.0                   |                               | 1.  Modify RS Tag number from 16 to 32                                                                                                                                                |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Add timeout flag to [FCTL_FPU_TRIG\[7\]](#FCTL_FPU_TRIG)                                                                                                                          |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Add [FCTL_ECC_DBG](#FCTL_ECC_DBG)                                                                                                                                                 |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Add MTQ delay function to [FCON_MT_CFG](#FCON_MT_CFG)\[12\]                                                                                                                       |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 5.  Move page_selection, maximum iteration number, decoding parameter, LDPC decode mode from [FCTL_LDPC_CFG](#FCTL_LDPC_CFG) to [FCON_LDPC_CFG](#FCON_LDPC_CFG)                       |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 6.  Add SB Maximum iteration number to [FCON_LDPC_CFG](#FCON_LDPC_CFG)                                                                                                                |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.1                   |                               | 1.  Modify [FCTL_ZIP_CFG](#FCTL_ZIP_CFG) and add ZIP Ring mode function                                                                                                               |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Change defalt value [FCON_LDPC_CFG](#FCON_LDPC_CFG)                                                                                                                               |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Add description of force fix data(FCTL_OTHER \_SET\[8\])                                                                                                                          |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Modify [DLL_DLL_CTL](#DLL_DLL_CTL)\[7:6\]                                                                                                                                         |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 5.  Add DSP register(FCON 0x140\~0x1d7)                                                                                                                                               |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 6.  Add [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[27:26\]                                                                                                                                      |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 7.  Remove [FCTL_UNC_CFG](#FCTL_UNC_CFG)\[15:4\]                                                                                                                                      |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 8.  [Add FPU sequence](#fpu-sequence-function)                                                                                                                                        |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.2                   |                               | 1.  Add [FTCL_FPU_INT_VCT](#FCTL_FPU_INT_VCT)                                                                                                                                         |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Add BCH/LDPC result of Soft bit correct to [FCTL_LDPC_CFG](#FCTL_LDPC_CFG)                                                                                                        |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Add chapter 4.26 FPU correct from IBF                                                                                                                                             |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Add Internal Buffer ring enable to [FCON_FLH_FUNC](#FCON_FLH_FUNC)\[29\]                                                                                                          |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.3                   |                               | 1.  Add Current MAP FIFO pointer to FCTL_MAP_CFG\[7\]                                                                                                                                 |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Add MAP FIFO Pointer Reset to FCTL_INT_INF \[8\]                                                                                                                                  |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Add PFA_ERR_MAP and STA_ERR_MAP to FCTL_MAP_CFG\[14\]\[15\] and FCTL_MAP_CFG\[27\]\[28\]                                                                                          |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Modify description of DLL_DLL_CTL                                                                                                                                                 |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 5.  Moify Chapter 4.2 Randomize R/W Flow                                                                                                                                              |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 6.  Modify MT table                                                                                                                                                                   |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 7.  Modify Chapter 1 System Block                                                                                                                                                     |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 8.  Modify Chapter4.6 TLC rule                                                                                                                                                        |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.4                   |                               | 1.  Add chapter 4.27 4.28                                                                                                                                                             |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Modify [FCTL_INT_INF](#FCTL_INT_INF), [FCTL_MAP_CFG](#FCTL_MAP_CFG), [FCTL_INT_CFG](#FCTL_INT_CFG), [FCTL_INT_CFG_1](#FCTL_INT_CFG_1) for FW sets ZIP setting error and E3D error |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.5                   |                               | 1.  Add [FCTL_ZIP_DMA_ST](#FCTL_ZIP_DMA_ST)                                                                                                                                           |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Add [FCTL_E3D_ERR_CNT](#FCTL_E3D_ERR_CNT)                                                                                                                                         |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Add [FCTL_DMA_STS_0](#FCTL_DMA_STS_0)                                                                                                                                             |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Add Auto update LLR after correct done to [FCON_LDPC_CFG\[28\]](#FCON_LDPC_CFG)                                                                                                   |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.6                   |                               | 1.  Add [FCON_FLH_FUNC](#FCON_FLH_FUNC)\[31:30\] \[27\]                                                                                                                               |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  [FCTL_INT_CFG](#FCTL_INT_CFG)\[17\] E3D interrupt is forbidden to enable                                                                                                          |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 3.  Add chapter 4.29                                                                                                                                                                  |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 4.  Modify 4.19                                                                                                                                                                       |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 5.  Modify 4.10 4.5                                                                                                                                                                   |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 6.  Add chapter 4.30                                                                                                                                                                  |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.7                   |                               | 1.  Change register default value [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[27:26\], [FCON_RTY_CFG](#FCON_RTY_CFG)\[30\]\[22\], [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[22:21\]                 |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | [FCTL_ZIP_CFG](#FCTL_ZIP_CFG)\[26\],                                                                                                                                                  |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | [FCTL_DMA_CFG](#FCTL_DMA_CFG)\[1\]                                                                                                                                                    |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.8                   |                               | 1.  Re-define [FCTL_IO_SET](#FCTL_IO_SET)\[25\]                                                                                                                                       |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | I/O pad type select                                                                                                                                                                   |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 0b: indicate single-end I/O type.                                                                                                                                                     |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 1b: indicate differential I/O type                                                                                                                                                    |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|            | 2.9                   |                               | 1.  Add detect DQSB function [FCTL_RESERVE](#FCTL_RESERVE)\[21:20\]                                                                                                                   |
|            |                       |                               |                                                                                                                                                                                       |
|            |                       |                               | 2.  Add flow after MT abort(chapter 4.19)                                                                                                                                             |
+------------+-----------------------+-------------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

1.  System Block

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image1.emf)

The Flash controller is used to control multiple NAND Flash devices as well as to access the data stored on these devices. Data is transferred using DMA, forgoing the need to use the ARC processor. Data is transferred from flash to memory in units of ECC frames. Up to 24 frames can be transferred per DMA transfer trigger.

+-------------------------+------------------------------------------------------------------------+
| Item                    | Content                                                                |
+=========================+:=======================================================================+
| Basic function          | Read/write flash data in the unit of sector                            |
|                         |                                                                        |
|                         | Enhance data reliability by ECC                                        |
|                         |                                                                        |
|                         | Randomize content for flash data                                       |
+-------------------------+------------------------------------------------------------------------+
| ECC                     | BCH supports 4k Bytes data protect 16 bit                              |
|                         |                                                                        |
|                         | LDPC supports 5 modes                                                  |
+-------------------------+------------------------------------------------------------------------+
| Data format             | 4k-byte data + 16-byte spare + 32-byte BCH parity + n-byte LDPC parity |
+-------------------------+------------------------------------------------------------------------+
| Support flash interface | Legacy                                                                 |
|                         |                                                                        |
|                         | Toggle 1.0 / Toggle 2.0                                                |
|                         |                                                                        |
|                         | ONFI 3.0                                                               |
+-------------------------+------------------------------------------------------------------------+

: Table 1 Flash controller feature

2.  Data Flow

## 2.1. DMA flash write with E3D (one 2K frame)

0\. Before DMA, need prepare LBA and FW usage in the IRAM of FIP

1\. After DMA trigger AXI interface will read from DRAM (512+2)\*4 package

2\. The data of package will store in IBUF(512B) & IRAM(E3D)

3\. RAM controller will move data to ECC domain, IBUF move 2k data then move IRAM 14B data(E3D & LCA & FW)

4\. ECC encoder will encode BCH, it will encode data 2k & E3D & FW & CRC-32

5\. As 4., at the same time, data will be calculated by CRC, it will calculate data 2k, E3D, LCA, FW, after CRC-32 be generate, it also be ECC encode

6\. After ECC & CRC operation, all data will be random by randomize module, the data include data 2k, E3D, LCA, FW, CRC-32, & BCH parity

7\. The data after random, it will transfer to flash domain

8\. Flash DMA module will generate interface signal to write data to flash

9\. Before write data, it will check the column is bad column or not, if yes, it will send dummy write to flash

## 2.2. DMA flash read with E3D (one 2K frame)

1\. Flash DMA module generate interface signal to read flash data

2\. It will check bad column, if yes, HW will do dummy read, the bad column data will be ignore

3\. After bypass bad column, bit counter will be counter 1 or 0 number

4\. The data will send to ECC domain and de-randomize

5\. After de-randomize, it will calculate CRC-32, if ECC no error, the result will send to CRC check to check fail or pass

6\. As 5., at the same time, de-randomize data also to do ECC decode

7\. As 5., at the same time, de-randomize data also transfer to system domain

8\. The data will be place in the IBUF(2k data) & IRAM(E3D & LCA & FW), then wait correct finish

9\. After ECC decode, if syndrome unequal all 0, it means ECC error, the syndrome will be send to COR module to calculate error location & CRC-32

10\. When correct calculate done, it will output CRC-32 with error, it will send to CRC check to check CRC pass or fail

10\. After correct calculate done, HW will correct error in the IBUF & IRAM

11\. After correct data, the IBUF & E3D of IRAM is no error(ECC is correct), it will send (512+2)\*4 package to DRAM

3.  FIP Pin Overview

  --------------------------------------------------------------------------------------------------------------------------
  **Pin/Group**          **Clk Domain**   **I/O**   **Width**                          **Description**
  ---------------------- ---------------- --------- ---------------------------------- -------------------------------------
  **ClkRst**             **　**           **　**    **　**                             **clock & reset pin**

  FLH_CLK_2              FLH_CLK_2        I         1                                  flash clock div2

  FLH_RSTJ_2             FLH_CLK_2        I         1                                  flash clock div2 rstj

  FLH_CLK                FLH_CLK          I         FIP_CH_NUM\*1                      flasah domain clock

  FLH_RSTJ               FLH_CLK          I         FIP_CH_NUM\*1                      flash domain rstj

  ECC_CLK                ECC_CLK          I         1                                  ecc domain clock

  ECC_RSTJ               ECC_CLK          I         1                                  ecc domain rstj

  SYS_CLK                SYS_CLK          I         1                                  system domain clock

  SYS_RSTJ               SYS_CLK          I         1                                  system domain rstj

  COR_CLK                COR_CLK          I         1                                  correct domain clock

  COR_RSTJ               COR_CLK          I         1                                  correct domain rstj

  **MCU**                　               　        　                                 **MCU interface**

  UP_FCON_CSJ            SYS_CLK          I         1                                  top register access CS

  UP_FALL_CSJ            SYS_CLK          I         1                                  all channel register access CS

  UP_FCTL_CSJ            SYS_CLK          I         FIP_CH_NUM\*1                      channel register access CS

  UP_BCR_CSJ             SYS_CLK          I         FIP_CH_NUM\*1                      BCRAM memory access CS

  UP_IRAM_CSJ            SYS_CLK          I         1                                  IRAM memory access CS

  UP_DCTL_CSJ            SYS_CLK          I         1                                  DLL control register access CS

  UP_WRJ                 SYS_CLK          I         4                                  UP access WR

  UP_RDJ                 SYS_CLK          I         1                                  UP access RD

  UP_ADR                 SYS_CLK          I         32                                 UP access ADR

  UP_DAT                 SYS_CLK          I         32                                 UP access DAT

  FIP_UP_WAT             SYS_CLK          O         1                                  UP wait signal

  FIP_UP_FCON_DAT        SYS_CLK          O         32                                 UP top register output data

  FIP_UP_FALL_DAT        SYS_CLK          O         32                                 UP all channel register output data

  FIP_UP_IRAM_DAT        SYS_CLK          O         32                                 UP IRAM output data

  FIP_UP_DCTL_DAT        SYS_CLK          O         32                                 UP DLL register output data

  FIP_UP_FCTL_DAT        SYS_CLK          O         FIP_CH_NUM\*32                     UP channel register output data

  FIP_UP_BCR_DOUT        SYS_CLK          O         FIP_CH_NUM\*32                     UP channel register output data

  **FLH**                　               　        　                                 **Flash interface**

  IO_I                   FLH_CLK          I         FIP_CH_NUM\*16                     flash io data input

  DQS_I                  FLH_CLK          I         FIP_CH_NUM\*1                      flash io dqs input

  RBY_I                  FLH_CLK          I         FLH_CE_NUM\*FIP_CH_NUM\*1          flash io rdy input

  CE_O                   FLH_CLK          O         FIP_CH_NUM\*FLH_CE_NUM             flash io CE output

  CLE_O                  FLH_CLK          O         FIP_CH_NUM\*1                      flash io cle

  ALE_O                  FLH_CLK          O         FIP_CH_NUM\*1                      flash io ale

  RE_O                   FLH_CLK          O         FIP_CH_NUM\*1                      flash io re

  WE_O                   FLH_CLK          O         FIP_CH_NUM\*1                      flash io we

  DQS_O                  FLH_CLK          O         FIP_CH_NUM\*1                      flash io DQS output

  DQS_OE                 FLH_CLK          O         FIP_CH_NUM\*1                      flash io DQS output enable

  WP_O                   FLH_CLK          O         FIP_CH_NUM\*1                      flash io write protect

  IO_O                   FLH_CLK          O         FIP_CH_NUM\*16                     flash io data output

  IO_OE                  FLH_CLK          O         FIP_CH_NUM\*16                     flash io data output enable

  **IBF**                　               　        　                                 **Internal Buffer**

  IBF_DOUT               SYS_CLK          I         RAM_DAT_WTH\*FIP_CH_NUM\*4         internal data buffer data output

  FIP_IBF_CEN            SYS_CLK          O         FIP_CH_NUM\*4                      internal data buffer CS

  FIP_IBF_WEN            SYS_CLK          O         FIP_CH_NUM\*4                      internal data buffer WE

  FIP_IBF_WBN            SYS_CLK          O         RAM_DAT_WTH\*FIP_CH_NUM\*4         internal data buffer WE BIT SEL

  FIP_IBF_ADR            SYS_CLK          O         IBF_ADR_WTH - 2 \* FIP_CH_NUM\*4   internal data buffer ADDR

  FIP_IBF_DIN            SYS_CLK          O         RAM_DAT_WTH \* FIP_CH_NUM\*4       internal data buffer data input

  **IRAM**               　               　        　                                 **Internal SRAM interface**

  IRAM_DOUT              SYS_CLK          I         RAM_DAT_WTH                        IRAM data output

  FIP_IRAM_CEN           SYS_CLK          O         1                                  IRAM CS

  FIP_IRAM_WEN           SYS_CLK          O         1                                  IRAM WE

  FIP_IRAM_WBN           SYS_CLK          O         RAM_DAT_WTH                        IRAM WE BIT SEL

  FIP_IRAM_ADR           SYS_CLK          O         RAM_ADR_WTH                        IRAM ADDR

  FIP_IRAM_DIN           SYS_CLK          O         RAM_DAT_WTH                        IRAM data input

  **BCRAM**              　               　        　                                 **Bad column RAM interface**

  BRAM_DOUT              FLH_CLK          I         FIP_CH_NUM\*BCR_DAT_WTH            BCRAM data output

  FIP_BRAM_CEN           FLH_CLK          O         FIP_CH_NUM\*1                      BCRAM CS

  FIP_BRAM_WEN           FLH_CLK          O         FIP_CH_NUM\*1                      BCRAM WE

  FIP_BRAM_WBN           FLH_CLK          O         FIP_CH_NUM\*BCR_DAT_WTH            BCRAM WE BIT SEL

  FIP_BRAM_ADR           FLH_CLK          O         FIP_CH_NUM\*BCR_ADR_WTH            BCRAM ADDR

  FIP_BRAM_DIN           FLH_CLK          O         FIP_CH_NUM\*BCR_DAT_WTH            BCRAM data input

  **OTHER**

  SYS_E3D_EN             SYS_CLK          I         1                                  system e3d enable

  SYS_DBUF_DSA           SYS_CLK          I         32                                 data buffer data source addr

  SYS_DRAM_ZONE3_START   SYS_CLK          I         32                                 dram zone3 region start ptr

  SYS_DRAM_ZONE3_END     SYS_CLK          I         32                                 dram zone3 region end ptr

  QRY_ACK                SYS_CLK          I         FIP_CH_NUM\*1                      buffer check ack

  FIP_QRY_DIR            SYS_CLK          O         FIP_CH_NUM\*1                      buffer check direction

  FIP_QRY_REQ            SYS_CLK          O         FIP_CH_NUM\*1                      buffer check request

  FIP_QRY_NUM            SYS_CLK          O         FIP_CH_NUM\*RAM_BUF_WTH            buffer check number

  SGN_ACK                SYS_CLK          I         FIP_CH_NUM\*1                      signoff ack

  FIP_SGN_DIR            SYS_CLK          O         FIP_CH_NUM\*1                      signoff direction

  FIP_SGN_REQ            SYS_CLK          O         FIP_CH_NUM\*1                      signoff request

  FIP_SGN_NUM            SYS_CLK          O         FIP_CH_NUM\*RAM_BUF_WTH            signoff buffer number

  IRAM_PARITY_ERR        SYS_CLK          O         1                                  IRAM parity check errorr

  BRAM_PARITY_ERR        SYS_CLK          O         1                                  BRAM parity check errorr

  TRAM_PARITY_ERR        SYS_CLK          O         1                                  TRAM parity check errorr

  IBF_PARITY_ERR         SYS_CLK          O         1                                  IBF parity check errorr

  TEST_MODE              SYS_CLK          I         1                                  test mode

  FIP_DBG                SYS_CLK          O         48 \* FIP_CH_NUM                   debug port output

  **AXIM**               　               　        　                                 **AXI Master interface**

  AXIM_ARVALID           SYS_CLK          I         1                                  AXIM ARVALID

  AXIM_ARADDR            SYS_CLK          I         AXI_ADR_WTH                        AXIM ARADDR

  AXIM_ARLEN             SYS_CLK          I         AXI_LEN_WTH                        AXIMARLEN

  AXIM_ARID              SYS_CLK          I         AXI_ID_WTH                         AXIM ARID

  AXIM_RREADY            SYS_CLK          I         1                                  AXIM RREADY

  AXIM_AWVALID           SYS_CLK          I         1                                  AXIM AWVALID

  AXIM_AWADDR            SYS_CLK          I         AXI_ADR_WTH                        AXIM AWADDR

  AXIM_AWLEN             SYS_CLK          O         AXI_LEN_WTH                        AXIM AWLEN

  AXIM_WVALID            SYS_CLK          O         1                                  AXIM WVALID

  AXIM_WSTB              SYS_CLK          O         AXI_STB_WTH                        AXIM WSTB

  AXIM_WLAST             SYS_CLK          O         1                                  AXIM WLAST

  AXIM_WDATA             SYS_CLK          O         AXI_DAT_WTH                        AXIM WDATA

  FIP_AXIM_ARREADY       SYS_CLK          O         1                                  AXIM ARREADY

  FIP_AXIM_RID           SYS_CLK          O         AXI_ID_WTH                         AXIM RID

  FIP_AXIM_RVALID        SYS_CLK          O         1                                  AXIM RVALID

  FIP_AXIM_RLAST         SYS_CLK          O         1                                  AXIM RLAST

  FIP_AXIM_RDATA         SYS_CLK          O         AXI_DAT_WTH                        AXIM RDATA

  FIP_AXIM_AWREADY       SYS_CLK          O         1                                  AXIM AWREADY

  FIP_AXIM_WREADY        SYS_CLK          O         1                                  AXIM WREADY

  **AXIS**               　               　        　                                 **SRAM AXI interface**

  SRAM_ARREADY           SYS_CLK          I         1                                  AXI ARREADY

  SRAM_RID               SYS_CLK          I         AXI_ID_WTH                         AXI RID

  SRAM_RVALID            SYS_CLK          I         1                                  AXI RVALID

  SRAM_RLAST             SYS_CLK          I         1                                  AXI RLAST

  SRAM_RDATA             SYS_CLK          I         AXI_DAT_WTH                        AXI RDATA

  SRAM_AWREADY           SYS_CLK          I         1                                  AXI AWREADY

  SRAM_WREADY            SYS_CLK          I         1                                  AXI WREADY

  FIP_SRAM_ARVALID       SYS_CLK          O         1                                  AXI ARVALID

  FIP_SRAM_ARADDR        SYS_CLK          O         AXI_ADR_WTH                        AXI ARADDR

  FIP_SRAM_ARLEN         SYS_CLK          O         AXI_LEN_WTH                        AXI ARLEN

  FIP_SRAM_ARID          SYS_CLK          O         AXI_ID_WTH                         AXI ARID

  FIP_SRAM_RREADY        SYS_CLK          O         1                                  AXI RREADY

  FIP_SRAM_AWVALID       SYS_CLK          O         1                                  AXI AWVALID

  FIP_SRAM_AWADDR        SYS_CLK          O         AXI_ADR_WTH                        AXI AWADDR

  FIP_SRAM_AWLEN         SYS_CLK          O         AXI_LEN_WTH                        AXI AWLEN

  FIP_SRAM_WVALID        SYS_CLK          O         1                                  AXI WVALID

  FIP_SRAM_WSTB          SYS_CLK          O         AXI_STB_WTH                        AXI WSTB

  FIP_SRAM_WLAST         SYS_CLK          O         1                                  AXI WLAST

  FIP_SRAM_WDATA         SYS_CLK          O         AXI_DAT_WTH                        AXI WDATA

  **DLL**                　               　        　                                 **DLL interface**

  MDLL_AMC                                I         1

  MDLL_SMC                                I         1

  MDLL_SIN                                I         1

  MDLL_BUNRI                              I         1

  MDLL_TEST                               I         1

  MDLL_TBI                                I         6

  MDLL_SOUT                               O         6

  MDLL_TBO                                O         1

  LGCSDLL_AMC                             I         FIP_CH_NUM\*1

  LGCSDLL_BUNRI                           I         FIP_CH_NUM\*1

  LGCSDLL_SIN                             I         FIP_CH_NUM\*1

  LGCSDLL_SMC                             I         FIP_CH_NUM\*1

  LGCSDLL_TBI                             I         FIP_CH_NUM\*4

  LGCSDLL_TEST                            I         FIP_CH_NUM\*1

  LGCSDLL_SOUT                            O         FIP_CH_NUM\*1

  LGCSDLL_TBO                             O         FIP_CH_NUM\*2

  TGLSDLLR_AMC                            I         FIP_CH_NUM\*1

  TGLSDLLR_BUNRI                          I         FIP_CH_NUM\*1

  TGLSDLLR_SIN                            I         FIP_CH_NUM\*1

  TGLSDLLR_SMC                            I         FIP_CH_NUM\*1

  TGLSDLLR_TBI                            I         FIP_CH_NUM\*4

  TGLSDLLR_TEST                           I         FIP_CH_NUM\*1

  TGLSDLLR_SOUT                           O         FIP_CH_NUM\*1

  TGLSDLLR_TB                             O         FIP_CH_NUM\*2

  TGLSDLLW_AMC                            I         FIP_CH_NUM\*1

  TGLSDLLW_BUNRI                          I         FIP_CH_NUM\*1

  TGLSDLLW_SIN                            I         FIP_CH_NUM\*1

  TGLSDLLW_SMC                            I         FIP_CH_NUM\*1

  TGLSDLLW_TBI                            I         FIP_CH_NUM\*4

  TGLSDLLW_TEST                           I         FIP_CH_NUM\*1

  TGLSDLLW_SOUT                           O         FIP_CH_NUM\*1

  TGLSDLLW_TB                             O         FIP_CH_NUM\*2

  **INT**                　               　        　                                 **INT interface**

  INT_ACK                SYS_CLK          I         FIP_CH_NUM\*1                      interrupt interface ACK

  FIP_INT_REQ            SYS_CLK          O         FIP_CH_NUM\*1                      interrupt interface REQ

  FIP_INT_VCT            SYS_CLK          O         FIP_CH_NUM\*INT_NUM_WTH            interrupt interface vector

  **TSRAM**              　               　        　                                 **D2I FIFO TWO PORT SRAM**

  TSRAM_DOUT             SYS_CLK          I         TRAM_DAT_WTH                       ram data out

  FIP_TSRAM_REN          SYS_CLK          O         1                                  ram read enable

  FIP_TSRAM_WEN          SYS_CLK          O         1                                  ram write enable

  FIP_TSRAM_WBN          SYS_CLK          O         TRAM_DAT_WTH                       ram write bit select

  FIP_TSRAM_RADR         SYS_CLK          O         TRAM_ADR_WTH                       ram read address

  FIP_TSRAM_WADR         SYS_CLK          O         TRAM_ADR_WTH                       ram write address

  FIP_TSRAM_DIN          SYS_CLK          O         TRAM_DAT_WTH                       ram data in
  --------------------------------------------------------------------------------------------------------------------------

Function Description

## 4.1. Flash DMA Data Format

In this project, Flash IP includes Compression(ZIP). So, the data format is divided into two parts, physical and logical. The program DMA should be full-page program. FW should set [FCON_PAGE_CFG](#FCON_PAGE_CFG)\[1:0\] based on nand and make sure the compressed data size meets the page size.

- **Physical view**

Flash DMA transfer unit is frame, the frame have three parts: data, spare, Parity. The data format is fixed. There is not option to change.

**Data :** 4K physical data

**Spare(p4k table) :** 16B(12B + crc32)

This table content is generated by HW.

+------------------------------------------------------+
| P4K_LCA0                                             |
+================+==================+==================+
| P4K_FW                            | P4K_SPRV         |
+----------------+------------------+------------------+
| FW3            | P4K_HEAD         | P4K_FW_2         |
+----------------+------------------+------------------+
| P4K_CRC32                                            |
+------------------------------------------------------+

**Parity :**

BCH : 32B

LDPC : Based on [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[3:1\]

Mode 0 : 495 B (HW automatically pad to 496B)

Mode 1 : 440 B

Mode 2 : 420 B

Mode 3 : 360 B

Mode 4 : 272 B

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image2.emf){width="6.559722222222222in" height="1.136111111111111in"}

- **Logical view**

The data in system buffer is compressed and transferred to Flash IP internal buffer.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image3.emf){width="4.615972222222222in" height="0.8243055555555555in"}

L4K table format:

+-----------------------------------------------------------+
| iRAM                                                      |
+==============+==============+==============+==============+
| 0            | 1            | 2            | 3            |
+--------------+--------------+--------------+--------------+
| L4K_LCA                                                   |
+--------------------------------------------+--------------+
| L4K_FW                                     | L4K_SPRV     |
+--------------+-----------------------------+--------------+
| BUF_MASK     | NEXT_L4K_PTR(Link_mode)     | L4K_FW_2     |
+--------------+-----------------------------+--------------+
| L4K_BADR                                   | L4K_ZCODE    |
+--------------------------------------------+--------------+

+------------------------------------------------------------------------------------------------------------+
| L4K_BADR: Buffer Address for this L4K(unit:512B).                                                          |
+============================================================================================================+
| L4K_LCA: LCA for this L4K.                                                                                 |
+------------------------------------------------------------------------------------------------------------+
| L4K_FW, L4K_FW_2: FW data for this L4K.                                                                    |
+------------------------------------------------------------------------------------------------------------+
| L4K_SPRV: Spare Valid for this L4K.                                                                        |
+------------------------------------------------------------------------------------------------------------+
| L4K_ZCODE: Compression ratio for this L4K. acceptable range 1\~7                                           |
|                                                                                                            |
| zcode size : bit0 - bit2                                                                                   |
|                                                                                                            |
|                     4K -\> (value+1)\*0.5K                                                                 |
|                                                                                                            |
| force zocde en: bit6                                                                                       |
|                                                                                                            |
|            fix pattern en: bit7                                                                            |
|                                                                                                            |
| Ex: zcode = 3 logical 4k is compressed into 2k                                                             |
+------------------------------------------------------------------------------------------------------------+
| BUF_MASK: The same of buf_valid.(just for flash read , program case not be support cause zip compression.) |
+------------------------------------------------------------------------------------------------------------+

## 4.2. Randomize R/W Flow

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image4.emf){width="6.695138888888889in" height="2.0364009186351706in"}

When flash write, the data pass through conversion first, and then calculate ECC parity, then before send to flash, all data (data, spare, ECC parity) will be reverse. When flash read, the data will be reverse, the direct to calculate ECC syndrome, the before store to SRAM, the data will be pass through conversion. The randomize repeats rule unit is block. The block with different page and different 1KB have different randomize rule, and the block with same page, same 1KB have same randomize rule.

The Flash controller supports the ability to randomize and/or invert the data stored into flash when writing data into the flash and will reverse the process when reading data from the flash. Data randomization and inversion is enabled by default and can be disabled by setting [FCTL_CHNL_SET\[9\]](#FCTL_CHNL_SET) for randomization and [FCTL_CHNL_SET\[8\]](#FCTL_CHNL_SET) for inversion. When data randomization and inversion are both enabled during a write to flash, data randomization is performed before data inversion. Conversely, data inversion is negated before data randomization during a read from flash. Note that the data randomization and inversion configurations for a read from a flash location must be the same as the configuration for when the data was written into that flash location to ensure the correctness of the data read.

**Data Randomization:**

Data randomization is to XORs the data in the Data region with a pseudo random value. By default, the pseudo random value generated is dependent on the page number of the data, the ECC frame number of the data, and [FCTL_SEED_INIT\[22:0\]](#FCTL_SEED_INIT). [FCTL_SEED_INIT\[22:0\]](#FCTL_SEED_INIT) can be any firmware defined value.

Configuring [FCON_FLH_FUNC\[11:10\]](#FCON_FLH_FUNC) changes the number of pages per block that the Flash controller will perceive in the flash. As a result, this changes the number of bits in the flash address used for generating the pseudo random number. The randomization rule can also be made to depend on [FCTL_OTHER_SET\[24:16\]](#FCTL_OTHER_SET) instead of the actual page number of the data by setting [FCTL_OTHER_SET\[25\]](#FCTL_OTHER_SET).

Note: The same randomization rule must be used in order to read the correct data. There are no flags to indicate that the rule used for writing and reading data is different. An ECC error will not be raised as ECC is performed after the data is randomized, not before.

**Data Inversion:**

Enabling data inversion will invert the bits of the entire ECC frame (Data, Spare, and ECC parity) before it is written to flash. The entire ECC frame will be inverted again to negate the original inversion when reading from flash.

Note:

If FW reads an erase page, different settings lead to different results.

  --------------------------------------------------------------------------
           inverse   randomize   Zip bypass
  -------- --------- ----------- ------------ ------------------------------
  Case 1   1         1           1            Get random data

  Case 2   1         1           0            Decompression fail

  Case 3   1         0           1            Get all 0 data

  Case 4   1         0           0            Decompression fail

  Case 5   0         NA          NA           Uncorrectable
  --------------------------------------------------------------------------

The randomize rule have three source, it like below figure

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image5.emf)

One is FTA, the FTA is the address for command sequence transfer, it can generate by address generation or from FTA register directly, this is default setting.

One is TLC mode, the source also from FTA, but it will do a W/L mapping table transform, rule is (page addr\*3 + (w/l ptr)) % 256, w/l ptr indicate L page = 0, M page = 1, U page = 2, the selection need enable [FCON_ADR_GEN\[23\]](#FCON_ADR_GEN).

Others is directly from register [FCTL_OTHER_SET\[24:16\]](#FCTL_OTHER_SET), the selection need enable [FCTL_OTHER_SET\[25\]](#FCTL_OTHER_SET).

## 4.3. Flash Interface

In this flash controller, we support three type flash interface, legacy, toggle and ONFI. Legacy mode is SDR data transfer, and toggle/ONFI modes are DDR data transfer. The I/O pins include ALE, CLE, WE, RE, DATA, and toggle/ONFI need DQS pin to support DDR data transfer. FIP can use register, FCON_LGC_TIME_CFG, FCON_TGL_TIME_CFG, FCON_ONF_TIME_CFG, FCON_LGC_TIME_CFG_1, FCON_TGL_TIME_CFG_1, FCON_ONF_TIME_CFG_1 to control FIP send waveform timing, below figure show configuration part, each register all in TOP register, total support two set group can configuration, each channel has a select register, [FCTL_DMA_CFG \[0\]](#FCTL_DMA_CFG) decide which one set to usage.

Legacy mode CMD/ADR/DAT waveform like below, in Legacy mode, internal flash clock is 1X frequency then outside RE/WE send to flash.

![Legacy mode command waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image6.emf)

![Legacy mode address waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image7.emf)

![Legacy mode data read/write waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image8.emf)

![Legacy data read with EDO waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image9.png){alt="lgc_rdat_edo" width="5.288194444444445in" height="1.0958333333333334in"}

Toggle mode CMD/ADR/DAT waveform like below:

![Toggle mode command waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image10.emf)

![Toggle mode address waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image11.emf)

![Toggle mode data read/write waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image12.emf)

ONFI mode CMD/ADR/DAT waveform like below,

![ONFI mode command waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image13.emf)

![ONFI mode address waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image14.emf)

![ONFI mode data read/write waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image15.emf)

Below is a table for minimum value of speed

LEGACY:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image16.emf){width="5.031944444444444in" height="2.0479166666666666in"}

TOGGLE 2.0:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image17.emf){width="5.54375in" height="2.1756944444444444in"}

ONFI:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image18.emf){width="5.575694444444444in" height="1.6159722222222221in"}

## 4.4.Flash PIO DAT

To send multiple PIO DAT, the register for PIO DAT FIRST/LAST should be set. These registers are at [FCTL_HS_MODE\[13:12\]](#FCTL_HS_MODE). By default, FIRST and LAST are set. The PIO read/write in different mode should be set as follows. To ensure the PIO state operates normally, the FIRST/LAST must be set in correct order.

// PIO one byte read

FCTL_HS_MODE\[13:12\]=0x3; // FIRST and LAST

data\[0\]=FCTL_PIO_DAT;

// PIO one byte write

FCTL_HS_MODE\[13:12\]=0x3; // FIRST and LAST

FCTL_PIO_DAT=data\[0\];

// PIO multi byte read

FCTL_HS_MODE\[13:12\]=0x1; // FIRST

for(i=0;i\<LEN;i++){

if(i==LEN-1) FCTL_HS_MODE\[13:12\]=0x2; // LAST

data\[i\]=FCTL_PIO_DAT;

if(i==0) FCTL_HS_MODE\[13:12\]=0x0;

}

FCTL_HS_MODE\[13:12\]=0x3; // set to default value

// PIO multi byte write

FCTL_HS_MODE\[13:12\]=0x1; // FIRST

for(i=0;i\<LEN;i++){

if(i==LEN-1) FCTL_HS_MODE\[13:12\]=0x2; // LAST

FCTL_PIO_DAT=data\[i\];

if(i==0) FCTL_HS_MODE\[13:12\]=0x0;

}

FCTL_HS_MODE\[13:12\]=0x3; // set to default value

Note: TOGGLE and ONFI mode must be write and read 16bits unit.

## 4.5. FIP RAM Relation

FIP have two RAM by itself usage, one is BRAM for store bad column location information, another is IRAM for store MTP, spare, FSA, ALU, FPU, one bad column group is 128/64 bytes, it can store 64/32 column location, decide by [FCTL_BDC_SET\[30:24\]](#FCTL_BDC_SET).

> ![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image19.emf)
>
> ![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image20.emf)

IRAM can store much information and the information can indicate start address location by register.

1.  Start address of Spare indicate by FCTL_SPR_ADR, store spare0\~31;

2.  Start address of FSA indicate by FCTL_FSA_BAS, store FSA0\~31, and use [FCTL_FSA_SEL\[5:0\]](#FCTL_FSA_SEL) to select which one FSA;

3.  Start address of ALU indicate by FCON_ALU_BAS, store ALU0\~3, and use [FCTL_DMA_CFG\[5:4\]](#FCTL_DMA_CFG) to select which one ALU;

4.  Start address of FPU indicate by FCTL_FPU_ENTRY and until the information of the RAM is END phase, we call it is a FPU sequence;

5.  Start address of MTP indicate by FCTL_MT_ADR\_\*, and one MTP have 64 bytes.

Note: Above start address location except FCTL_FPU_ENTRY should follow 8bytes align to co-operate the RAM bus width. FCTL_FPU_ENTRY should follow 2bytes align to co-operate the RAM bus width.

## 4.6. Address Generation

The Flash Target Address (FTA) is the actual address in the flash where the data for a flash command will be read from or written into. The Flash controller provides an address generating machine to obtain the FTA from the Flash Source Address (FSA) based on rules specified in the AGR. The generated row address of the FTA will be stored in FCTL_FARG_ROW and the column address in FCTL_FARG_COL. The corresponding byte in FCTL_FARG_ROW and FCTL_FARG_COL will be sent to the flash during the corresponding phase in a flash command.

Below shows the layout of the FSA and ALU

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image21.emf)

The Layout of FSA and ALU

The FTA can be generated from the FSA by using the rules specified in the ALU by performing the following steps:

1.  Specify the ALU and FSA that will be used.

2.  Specify the ALU group to use in [FCTL_DMA_CFG\[5:4\]](#FCTL_DMA_CFG).

3.  Clear [FCTL_DMA_CFG\[3\]](#FCTL_DMA_CFG) to use FSA0 or set [FCTL_DMA_CFG\[3\]](#FCTL_DMA_CFG) to use the FSA specified in [FCTL_FSA_SEL\[5:0\]](#FCTL_FSA_SEL).

4.  Use ADR_GEN phase of FPU (no register directly trigger, if need to ADR_GEN, need create ADR_GEN phase in FPU sequence)

5.  The target column address will be found in FCTL_FARG_COL and the target row address will be found in FCTL_FARG_ROW

The ALU has 3 fields: Source Bit, Target Bit, and Mapping Bit Length. The Source Bit field specifies the starting bit of the copy source in the FSA. The Target Bit field specifies the starting bit of the copy destination in the FTA. The Mapping Bit Length field specifies the number of bits to copy from FSA to FTA. The number of ALU the address generating machine will iterate over for an ALU group is specified in [FCON_ADR_GEN_STOP\[2:0\]](#FCON_ADR_GEN_STOP).

Below shows an example of how the address generating machine works.

![cid:image001.png@01CE8176.3248F810](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image22.png){width="6.74375in" height="3.984027777777778in"}

Address generation example

**Columns address generation:**

Column address do not generate from ALU rule, it catch 4bit of FSA to frame pointer, and use the frame ptr to calculate column address and auto update [FCTL_OTHER_SET\[30:26\]](#FCTL_OTHER_SET) (if [FCON_ADR_GEN\[8\]](#FCON_ADR_GEN) as 1'b1).

Example:

FSA as 00_00_00_00_03h, and [FCON_ADR_GEN\[5:0\]](#FCON_ADR_GEN) as 6'h0

Address generate catch frame pointer is 3h, so it will calculate column address of frame 3 to FCTL_FARG_COL

Relation function with address generate

ALU execute all, address generate can use one bit of FSA to decide run all ALU or not, [FCON_ADR_GEN_STOP\[15\]](#FCON_ADR_GEN_STOP) is enable bit, when enable, [FCON_ADR_GEN_STOP\[13:8\]](#FCON_ADR_GEN_STOP) indicate bit location in FSA, when this bit value same as [FCON_ADR_GEN_STOP\[14\]](#FCON_ADR_GEN_STOP), it will execute 8 ALU, if not, it will execute n+1 ALU (n= [FCON_ADR_GEN_STOP\[2:0\]](#FCON_ADR_GEN_STOP)).

When [FCON_ADR_GEN\[10\]](#FCON_ADR_GEN) as 1'b1, address generate will not generate by ALU, it only move FSA to FTA directly, and column address will replace the part of column address after the move operation.

When over two ADR_GEN description in FPU sequence, [FCTL_FSA_SEL\[13:8\]](#FCTL_FSA_SEL) is indicated that second FSA will reference to select. [FCTL_FSA_SEL\[7:6\]](#FCTL_FSA_SEL) is indicated that second FSA counter to select second FSA when this counter as 2'h3.

**TLC rule address generation:**

TLC address mode is selected for address generation, if [FCON_ADR_GEN\[23\]](#FCON_ADR_GEN) as 1'b1. The randomize rule will not directly catch FTA\[23:16\], it will do equation below

> Random rule = (FTA\[23:16\]\*3+TLC_PTR)%512 if FCON_FLH_FUNC\[11:10\] ==0
>
> Random rule = (FTA\[23:16\]\*3+TLC_PTR)%256 if FCON_FLH_FUNC\[11:10\] ==1
>
> Random rule = (FTA\[23:16\]\*3+TLC_PTR)%128 if FCON_FLH_FUNC\[11:10\] ==2
>
> Random rule = (FTA\[23:16\]\*3+TLC_PTR)%64 otherwise

TLC_PTR is indicate L/M/U page, the value will from FSA, [FCON_ADR_GEN\[22:17\]](#FCON_ADR_GEN) will indicate location in the FSA.

List table is below

  -------------------------------------
  W/L ADDR   L page   M page   U page
  ---------- -------- -------- --------
  0          0        1        2

  1          3        4        5

  2          6        7        8

  3          9        10       11

  4          12       13       14

  5          15       16       17

  6          18       19       20

  7          21       22       23

  8          24       25       26

  9          27       28       29

  10         30       31       32

  11         33       34       35

  12         36       37       38

  13         39       40       41

  14         42       43       44

  15         45       46       47

  16         48       49       50

  17         51       52       53

  18         54       55       56

  19         57       58       59

  20         60       61       62

  21         63       64       65

  22         66       67       68

  23         69       70       71

  24         72       73       74

  25         75       76       77

  26         78       79       80

  27         81       82       83

  28         84       85       86

  29         87       88       89

  30         90       91       92

  31         93       94       95

  32         96       97       98

  33         99       100      101

  34         102      103      104

  35         105      106      107

  36         108      109      110

  37         111      112      113

  38         114      115      116

  39         117      118      119

  40         120      121      122

  41         123      124      125

  42         126      127      128

  43         129      130      131

  44         132      133      134

  45         135      136      137

  46         138      139      140

  47         141      142      143

  48         144      145      146

  49         147      148      149

  50         150      151      152

  51         153      154      155

  52         156      157      158

  53         159      160      161

  54         162      163      164

  55         165      166      167

  56         168      169      170

  57         171      172      173

  58         174      175      176

  59         177      178      179

  60         180      181      182

  61         183      184      185

  62         186      187      188

  63         189      190      191

  64         192      193      194

  65         195      196      197

  66         198      199      200

  67         201      202      203

  68         204      205      206

  69         207      208      209

  70         210      211      212

  71         213      214      215

  72         216      217      218

  73         219      220      221

  74         222      223      224

  75         225      226      227

  76         228      229      230

  77         231      232      233

  78         234      235      236

  79         237      238      239

  80         240      241      242

  81         243      244      245

  82         246      247      248

  83         249      250      251

  84         252      253      254

  85         255      0        1
  -------------------------------------

**MTQ address generation limitation:**

There are two MTQ including command and DMA MTQ separately. Firstly, command FPU is executed that ADR_GEN generates row address for NAND flash and randomizer. Second DMA FPU need to execute ADR_GEN as well. Because randomizer will check row address as conversion rule, DMA FPU must configure row address again with ADR_GEN in command and DMA MTQ separately condition.

Example error case:

Queue0: event_1, event_2.

event_1: (ADR_GEN, Cmd05, Adr5, CmdE0, DLY10, END)

event_2: (DMA_READ END)

Example corrects case:

Queue0: event_1, event_2.

event_1: (ADR_GEN, Cmd05, Adr5, CmdE0, DLY10, END)

event_2: (ADR_GEN, DMA_READ, END)

**Internal FSA function**

FIP adds a function for fetching FSA from controller register.

It is enabled by [FCTL_FSA_SEL](#FCTL_FSA_SEL)\[18\] = 1.

Ex.

If Fpu_seq is ( CMD00, ADR_GEN, ADR5, CMD32, CMD00, ADR_GEN, ADR5, CMD30)

The 1st address gen will use {[FCTL_IFSA0](#FCTL_IFSA0),8'b0}

The 2nd address gen will use {[FCTL_IFSA1](#FCTL_IFSA1),8'b0}

## 4.7. Ultra DMA Function

Because BCH algorithms in flash read data operation, it has two step operation, one is decode, another is correct, decode can operate in data transfer, when transfer done, it can use syndrome to check data is error or not, if it is error, it will need correction operation to correct data or indicate uncorrectable, so the total DMA done decide by correction done not data transfer done.

![**Ultra ECC functions disable:**](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image23.emf)

But in correction operation, Flash busy idle, the Flash bus still can do some command or data transfer, so it need to let DMA done bit to indicate data transfer done. It can trigger next DMA to enhance performance when [FCTL_DMA_CFG\[1\]](#FCTL_DMA_CFG) as 1'b1. So when last DMA is read, it will wait the DMA done and ECC busy done.

![**Ultra ECC functions enable:**](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image24.emf)

## 4.8. Bad Column Flash R/W DMA

In TSB 3xnm or 2xnm D3/ED3, the congenital column defect is not replace by a good column. The controller needs bad column function to handle the extend column system.

FW needs enter flash test mode before bad column collect, the enter test mode command is

> 14h-F7h-68h-55h-addr(00h)-data(01h)
>
> 5Ch-C5h-55h-addr(00h)-data(01h)

Leave test mode command is FFh

Bad column collect

![**Bad column collection for legacy:**](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image25.emf)

![**Bad column collection for toggle:**](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image26.emf)

To detect flash bad column information, controller needs to read out 1 page + 48 bytes data. The bad column output data is always in 2Bytes unit even in legacy mode. If data is FEh FEh, it means the 2 bytes (Even/Odd) are good. If data is FFhFEh, FEhFFh or FEhFFh, it means the 2 bytes (Even/Odd) are bad column. Maximally, the controller can record 48 bad column locations.

Maximally, the controller can record 48 bad column locations.

The good data value can be set by register FCTL_BDC_SET. The default good data value is 8'hFE. FW can set other value for bad column use.

After 69-05-adr\*5-E0, FW can trig [FCTL_BDC_CTL\[24\]](#FCTL_BDC_CTL) to read 1 page + 48 data, and the length is set by [FCTL_BDC_CTL\[15:0\]](#FCTL_BDC_CTL). When collect done, the bad column locations will be collected to one of BRAM0\~7 by selected [FCTL_BDC_SET\[29:24\]](#FCTL_BDC_SET).

**DMA r/w with bad column:**

After bad column collect, FW enable [FCTL_BDC_CTL\[25\]](#FCTL_BDC_CTL). When DMA write, if the byte location is bad column, the byte will be replaced with a fix value by using [FCTL_BDC_CTL\[23:16\]](#FCTL_BDC_CTL), and the data will shift 1 byte.

![Bad column data transfer waveform](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image27.emf)

When DMA read, if the byte location is bad column, the byte will be ignored to SRAM and ECC decoder.

**Column address generates:**

Because bad column will change the DMA data location at flash, so when DMA start is not at sector 0, the column address will be effect by bad column.

FW need trig BCC_TRIG of FPU sequence, and HW will calculate how much bad column in this sector, the auto column address generation will generate column address with bad column information.

## 4.9. FPU Sequence Function

For application by flash command, FIP can define the command sequence in IRAM, and then use FPU pointer to indicate which one command sequence need send to flash.

The FPU code in IRAM is 16bit unit, and total has below:

- NOP (0x0)

  ---------------------------------------------------------
  Bit              15 --- 12             11 -- 0
  --------------- ----------- -----------------------------
  Value/Field         0x0             Not Used (NU)

  ---------------------------------------------------------

> No operation

- DLY (0x1)

  --------------------------------------------------------
  Bit              15 --- 12  11 --- 8        7 -- 0
  --------------- ----------- ---------- -----------------
  Value/Field         0x1     NU               CYCLE

  --------------------------------------------------------

> Delay CYCLE\*FCLK time.

- CMD (0x2)

  --------------------------------------------------------------------
  Bit              15 --- 12   11 --- 10  9       8          7 -- 0
  --------------- ----------- ----------- ------- ---------- ---------
  Value/Field         0x2         NU      DQS_H   DQS_OE     DAT

  --------------------------------------------------------------------

> Send command (DAT).
>
> DQS_OE = 0: Disable DQS output
>
> DQS_OE = 1: Enable DQS output
>
> DQS_H = 0: DQS output low when DQS_OE = 1
>
> DQS_H = 1: DQS output high when DQS_OE = 1

- ADR (0x3)

  --------------------------------------------------------
  Bit              15 --- 12  11 --- 8        7 -- 0
  --------------- ----------- ---------- -----------------
  Value/Field         0x3     NUM               DAT

  --------------------------------------------------------

> Send address.
>
> NUM is the number of address to send
>
> NUM=1: send one address (DAT)
>
> NUM=2: send column address for FTA
>
> NUM=3: send row address for FTA
>
> NUM=4: send 4 address for FTA (2 rows and 2 columns)
>
> NUM=5: send row and column address for FTA

- DAT (0x4)

  --------------------------------------------------------
  Bit              15 --- 12  11 --- 8        7 -- 0
  --------------- ----------- ---------- -----------------
  Value/Field         0x4     MOD              DAT_L

  Bit              15 --- 12  11 --- 8        7 -- 0

  Value/Field         0x4     NU               DAT_H
  --------------------------------------------------------

> Send/receive data.
>
> The MOD is used to select data.

  ---------------------------------------------------------
  MOD                11          10      9        8
  -------------- ----------- ----------- -------- ---------
  Field             Last        First    Ext      R/W

  Normal data         1           1      0        0
  ---------------------------------------------------------

> The Ext bit extends the code to 32 bits, and DAT_H is high byte data.
>
> W/R=1: send DAT_L (or DAT_H and DAT_L)
>
> W/R=0: read data. As Ext == 1'b1, this function is used to read data and compare. If comparing fail, this instruction will assert interrupt to notify MCU.
>
> Where INT = (read data & MASK) != DAT_L, the MASK = DAT_H.
>
> Note: the extend 32 bits code cannot across 8 bytes boundary in SRAM, if use ext to cross two 8bytes boundary, please use NOP to 8bytes align
>
> For check program fail when auto polling, three DAT command are needed.

  -------------------------------------------------------------------------
  Bit           15 --- 12     11 --- 8    7 -- 0
  ------------- ------------- ----------- ---------------------------------
  Value/Field   0x4           1110b       DAT_L

  Bit           15 --- 12     11 --- 8    7 -- 0

  Value/Field   0x4           0010b       DAT_H (poll RBY bit)

  Bit           15 --- 12     11 --- 8    7 -- 0

  Value/Field   0x4           0000b       DAT_H_2 (poll program fail bit)
  -------------------------------------------------------------------------

> EX:
>
> Polling sequence:
>
> 0x4E00
>
> 0x4240
>
> 0x4001
>
> 0x4E00 is used to read flash status
>
> 0x4240 is used to check RBY and extend to check program fail bit
>
> The rule of check RBY : (read data & MASK) != DAT_L,
>
> the MASK = DAT_H.
>
> 0x4001 is used to check program fail bit
>
> The rule of check fail bit : (read data & MASK) != DAT_L,
>
> the MASK = DAT_H_2.
>
> if the flash status is 0xE0 🡺 FIP send INT_VCT to system with [PFA](#PFA) = 0
>
> 0xE0 & 0x01 = 0x00, 0x00 == 0x00;
>
> if the flash status is 0xE1 🡺 FIP send INT_VCT to system with [PFA](#PFA) = 1
>
> 0xE1 & 0x01 = 0x01, 0x01 != 0x00;
>
> [FCTL_INT_CFG\[27\]](#FCTL_INT_CFG) is interrupt enable bit and [FCTL_INT_CFG_1\[9\]](#FCTL_INT_CFG_1) is CPU interrupt enable bit.

[Note:]{.underline} [pfa_int_en](#pfa_int_en) [must be set 1 when polling sequence using three DAT command to check program fail.]{.underline}

> Note: the extend 48 bits code cannot across 8 bytes boundary in SRAM, if use ext to cross two 8bytes boundary, please use NOP to 8bytes align

- POL (0x5)

  ----------------------------------------------------------
  Bit              15 --- 12  11 --- 8          7 -- 0
  --------------- ----------- ------------ -----------------
  Value/Field         0x5     STATUS             MASK

  ----------------------------------------------------------

> Polling RBY pin or status.
>
> STAUS=1: polling status with MASK
>
> STAUS=0: polling RBY pin

- DMA (0x6)

  ------------------------------------------------------------------------
  Bit              15 --- 12  11 --- 8      7 --- 4         3 --- 0
  --------------- ----------- ------------- --------------- --------------
  Value/Field         0x6     DMY_EN,W/R    Not Used (NU)   Not Used(NU)

  ------------------------------------------------------------------------

> To trigger DMA read/write data
>
> Bit 8
>
> W/R=1: DMA write
>
> W/R=0: DMA read
>
> Bit 9
>
> DMY phase enable

- ADR_GEN (0x7)

  ---------------------------------------------------------------------------
  Bit              15 --- 12  11 -- 6         5 -- 4          3 -- 0
  --------------- ----------- --------------- --------------- ---------------
  Value/Field         0x7     Not Used (NU)   Not Used (NU)   Not Used (NU)

  ---------------------------------------------------------------------------

> To trigger address generate

- BCC_TRIG (0x8)

  --------------------------------------------------------------------------
  Bit              15 -- 12  11 -- 6         5 -- 4          3 -- 0
  --------------- ---------- --------------- --------------- ---------------
  Value/Field        0x8     Not Used (NU)   Not Used (NU)   Not Used (NU)

  --------------------------------------------------------------------------

> To trigger bad column address calculator

- RTY_DAT (0x9)

  -------------------------------------------------------
  Bit              15 -- 12  11 -- 6             7-0
  --------------- ---------- --------------- ------------
  Value/Field        0x9     Not Used (NU)       Data

  -------------------------------------------------------

> Retry data for retry command usage
>
> ![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image28.emf)

- RAW (0xA)

  --------------------------------------------------------------------------
  Bit              15 -- 12  11 -- 6         5 -- 4          3 -- 0
  --------------- ---------- --------------- --------------- ---------------
  Value/Field        0xA     Not Used (NU)   Not Used (NU)   Not Used (NU)

  --------------------------------------------------------------------------

> To trigger read flash raw data to DRAM

- SBC (0xc)

  -------------------------------------------------------------
  Bit               15 -- 12   11 -- 6               7-0
  --------------- ------------ --------------- ----------------
  Value/Field         0xc      Not Used (NU)    Not Used (NU)

  -------------------------------------------------------------

> The decode mode set at [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[6:4\]. For Soft bit correct, the acceptable values are 1 to 4

- BR (0xD)

  --------------------------------------------------------------------------
  Bit           15 -- 12   11:8   7:5       4:2         1           0
  ------------- ---------- ------ --------- ----------- ----------- --------
  Value/Field   0xD        NU     IBF ptr   Frame num   Direction   MODE

  --------------------------------------------------------------------------

- END (0xF)

  ------------------------------------------------------
  Bit              15 --- 12           11 -- 0
  --------------- ----------- --------------------------
  Value/Field         0xF           Not Used (NU)

  ------------------------------------------------------

> End custom sequence

In application, it will define all command sequence in the IRAM, below is a for example:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image29.png){width="5.368055555555555in" height="5.54375in"}

The address pointer is number of command sequence, set the number to FCTL_FPU_ENTRY and trigger DMA; it will send command sequence by the address pointer to flash.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image30.png){width="5.520138888888889in" height="4.9118055555555555in"}

The delay value for timing setting

Legacy:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image31.emf){width="4.104166666666667in" height="1.4083333333333334in"}

Toggle:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image32.emf){width="5.759722222222222in" height="1.0in"}

ONFi:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image33.emf){width="5.759722222222222in" height="1.336111111111111in"}

Example:

Relation register

FCTL_FPU_TRIG : fpu directly trigger bit

FCTL_FPU_ENTRY : fpu base adr

IRAM content:

  ----------------------------------------------------------------
               76           54           32           10
  ------------ ------------ ------------ ------------ ------------
  \@800        F000         1014         20ff         0000

  \@808        1014         0000         3578         2080

  \@810        F000         2010         6100         0000

  \@818        1014         2030         3587         2000

  \@820        0000         F000         6000         F000
  ----------------------------------------------------------------

IOW FCTL_FPU_ENTRY 00000800

IOW FCTL_FPU_TRIG 00000001

WAIT FCTL_FPU_TRIG 00000000

- It will send cmdFF-dly14-end

IOW FCTL_FPU_ENTRY 00000808

IOW FCTL_FPU_TRIG 00000001

WAIT FCTL_FPU_TRIG 00000000

- It will send cmd80-adr5-null-dly14-null-dmawrite-cmd10-end

IOW FCTL_FPU_ENTRY 00000818

IOW FCTL_FPU_TRIG 00000001

WAIT FCTL_FPU_TRIG 00000000

- It will send cmd00-adr5-cmd30-dly14-end

IOW FCTL_FPU_ENTRY 00000822

IOW FCTL_FPU_TRIG 00000001

WAIT FCTL_FPU_TRIG 00000000

- It will send DMA read-end

Note:

Wait bit pattern:

Register: FCTL_FPU_TRIG (Offset D0h)

Bit descriptions:

For bit3 (ANY_BSY), If have any MT entry on MTQ, the bit3 will always be \"1\". Bit3 is combine with 0xF8 (FCTL_MT_PTR_DIF/ MT Pointer Different Register). When bit3 and 0xF8 are 0, it is meaning all CMD or MTQ action on Flash channel are finish.

For bit0 of FCTL_FPU_TRIG is only mean \"HW FPU trig\" is done (HW send all the CMD to flash is finish)

For bit1 is combine with Signoff and ECC (ECC calculate done, whether it is correctable or uncorrectable), if either is not finished, bit 1 will be \"1\".

So, when you want to know FPU and Signoff, ECC are done, please polling bit0 and bit1 to \"0\". If you want to know FPU, Signoff, ECC are done and can be trig next FPU, please polling bit0, bit1 are \"0\" and bit2 is \"1\". For bit 1, if Signoff function is disabled, this bit only means ECC calculate done.

## 4.10. Signoff Mask Function

The Flash controller can use the data stored in the Spare region of the ECC frames to mask the buffer flags. While operating in buffer mode, the buffer flag can be masked when the data is transferred from the flash and into the SRAM. The mask is used to determine if the valid data flag will be set to 1 automatically or left untouched when data is transferred into the SRAM. A bit value of 0 in the mask means that the setting of the valid data flag is masked. That is, the flag will be untouched. Note that valid data flag checking will still occur when the data is masked.

[FCON_SGN_MSK\[0\]](#FCON_SPR_VALID_FUNC) to enable signoff mask

The mask rule is BUF_MASK = 1 & L4K_SPRV = 0, the corresponding flag will be masked.

+-------------------------------------------------------------+
| L4K_LCA                                                     |
+==================+=========================+================+
| L4K_FW                                     | L4K_SPRV       |
+------------------+-------------------------+----------------+
| L4K_ZCODE        | NEXT_L4K_PTR(Link_mode) | L4K_FW_2       |
+------------------+-------------------------+----------------+
| L4K_BADR                                   | BUF_MASK       |
+--------------------------------------------+----------------+

Example:

Buffer mode enable.

[FCON_SGN_MSK\[0\]](#FCON_SPR_VALID_FUNC) = 1

BUF_MASK= 8'b0000_1111 L4K_SPRV = 'b0101_0101

After read, the flag becomes 'b0000_0101.

If flag is masked inside a L4K, FIP will push the corresponding L4K bit map to [FCTL_MAP_INF](#FCTL_MAP_INF)

## 4.11. Digital PLL for DQS Delay

The main function of DLL is to delay DQS and write data clock in ONFI / Toggle flash interface. The best delay time is 1/4 phase clock cycle time, so HW insert a digital PLL to lock 1/4 phase clock.

DLL_REG module control the delay value output to each SDLL. It is designed for these main functions.

1.  Select SDLL load method

2.  Select shift phase (22.5 --- 135 degree)

3.  Control MDLL setting

4.  Check load SDLL condition and Auto load SDLL delay

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image34.emf)

Note: To use the DLL lock delay phase, if input clock is too slow, it occur lock fail. For example, delay cell maximum delay is 10ns, the input clock slow then 2\*10ns, it will lock fail.

Auto load SDLL delay

With auto load function enabled, the SDLL delay value will be updated periodically. This update period is defined by register LOAD_CTL\[15:0\] and counted by FCLK_DIV2. The period can be set as default or larger (ex 0xF000).

Load with DMA start

With auto load function enabled, the SDLL delay value will be updated every time DMA starts. This function is set by register LOAD_CTL\[28\].

Setting flow: (use auto load with trigger mode)

> // Enable DLL auto load
>
> DLL_CTL\[2\]=0; // reset MDLL
>
> DLL_CTL\[2\]=1;
>
> DLL_CTL\[4\]=1;
>
> DLL_CTL\[1:0\]=FLH_STROBE_MODE;
>
> LOAD_CTL\[30\]=1; // stop average function
>
> // start MDLL tracking
>
> while(MDLL_STA\[0\]==0); // wait lock
>
> if(MDLL_STA\[1\]){
>
> if(MDLL_STA\[3\]){ // underflow
>
> SEL_VAL=MIN_VALUE;
>
> SEL_VAL_1=MIN_VALUE;
>
> }
>
> else{ // overflow
>
> SEL_VAL=MAX_VALUE;
>
> SEL_VAL_1=MAX_VALUE;
>
> }
>
> DLL_CTL\[1:0\]=FLH_DIRECT_MODE;
>
> DLL_CTL\[4\]=0;
>
> }
>
> else{
>
> LOAD_CTL\[23:16\]=0xFF // FW load MDLL value
>
> LOAD_CTL\[30\]=0; // start average function
>
> LOAD_CTL\[28\]=1; // start load with DMA function
>
> }

It support another upload function is period mode, but the default mode use trigger mode.

Setting flow: (use auto load with period mode)

> // Enable DLL auto load
>
> DLL_CTL\[2\]=0; // reset MDLL
>
> DLL_CTL\[2\]=1;
>
> DLL_CTL\[4\]=1;
>
> DLL_CTL\[1:0\]=FLH_STROBE_MODE;
>
> LOAD_CTL\[31:24\]=0xC1; // enable auto load
>
> while(MDLL_STA\[0\]==0); // wait lock
>
> if(MDLL_STA\[1\]){
>
> LOAD_CTL\[24\]=0; // disable auto load
>
> if(MDLL_STA\[3\]){ // underflow
>
> SEL_VAL=MIN_VALUE;
>
> SEL_VAL_1=MIN_VALUE;
>
> }
>
> else{ // overflow
>
> SEL_VAL=MAX_VALUE;
>
> SEL_VAL_1=MAX_VALUE;
>
> }
>
> DLL_CTL\[1:0\]=FLH_DIRECT_MODE;
>
> DLL_CTL\[4\]=0;
>
> }
>
> else{
>
> LOAD_CTL\[30\]=0; // start average function
>
> }

The ideal delay setting table as below:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image35.png){width="4.527777777777778in" height="0.8881944444444444in"}

## 4.12. Register Dump

FIP support one path can backup or upload IRAM, BRAM, FCON register, FCTL register to/from system RAM, BRAM transfer unit is 128bytes, others is 256bytes; control register are FCON_RGD_CTL and FCON_RGD_ADR.

A notice is, when dump operation, all CPU access FIP operation will disable, so it cannot access FIP in dump operation.

Top and channel register has some register need be disable in dump, so dump operation will mask the register, below is those register:

FCTL_PIO_CMD, FCTL_PIO_ADR, FCTL_PIO_DAT,

FCON_FCE_SET, FCON_FCE_CLR, FCON_RGD_ADR, FCON_RGD_CTL

Example:

FCON_RGD_ADR\[31:0\] = 0000_0200h

FCON_RGD_CTL\[1\] = 0b

FCON_RGD_CTL\[2\] = 0b

FCON_RGD_CTL\[7:4\] = 8h

FCON_RGD_CTL\[0\] = 1b

Wait FCON_RGD_CTL\[0\] = 0b

Top register will dump to SRAM 0000_0200h

FCON_RGD_ADR\[31:0\] = 0000_0200h

FCON_RGD_CTL\[1\] = 1b

FCON_RGD_CTL\[2\] = 0b

FCON_RGD_CTL\[7:4\] = 8h

FCON_RGD_CTL\[0\] = 1b

Wait FCON_RGD_CTL\[0\] = 0b

SRAM 0000_0200h 256Byte will be dump to Top register

Note: Not support register dump, because no AXI path to DRAM

## 4.13. Interrupt

FIP can send a interrupt vector to system level when a FPU done, [FCTL_INT_CFG\[0\]](#FCTL_INT_CFG) is enable bit, the vector value indicate in [FCTL_INT_VCT\[15:8\]](#FCTL_INT_VCT) and extend vector value indicate in [FCTL_INT_VCT\[31:16\]](#FCTL_INT_VCT), system level need have a interrupt vector queue to collect vector, in order to avoid information lost.

FIP just send interrupt vector, no vector queue and not to decide interrupt CPU or not

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image36.png){width="6.0in" height="2.4in"}

Interrupt vector format

  ----------------------------------------------------------------------------------------------------
  27           26           25                    24         23:9                   8:0
  ------------ ------------ --------------------- ---------- ---------------------- ------------------
  ABORT_DONE   DMA_DONE     PARTIAL_DONE          ALL_DONE   VCT\[23:9\]/Reserved   VCT\[8:0\]/INDEX

  35           34:32        31                    30         29                     28

  INDEX_MODE   TARGET_CPU   []{#PFA .anchor}PFA   MT_STOP    STA                    EOT

                                                                                    36

                                                                                    CPU_INT
  ----------------------------------------------------------------------------------------------------

1.  INDEX: If INDEX_MODE = 1, interrupt vector\[8:0\] means the interrupt index, FW can get 8-bit information from system.

  -------------------------------------------------------------------------
  5           4           3            2          1              0
  ----------- ----------- ------------ ---------- -------------- ----------
  STA         EOT         ABORT_DONE   DMA_DONE   PARTIAL_DONE   ALL_DONE

                                                  7              6

                                                  PFA            MT_STOP
  -------------------------------------------------------------------------

2.  VCT: If INDEX_MODE = 0, interrupt vector\[23:0\] means VCT, FW can get 32-but information from system.

  --------------------------------------------------------------------------------------------
  27           26          25             24         23:9                   8:0
  ------------ ----------- -------------- ---------- ---------------------- ------------------
  ABORT_DONE   DMA_DONE    PARTIAL_DONE   ALL_DONE   VCT\[23:9\]/Reserved   VCT\[8:0\]/INDEX

                           31             30         29                     28

                           PFA            MT_STOP    STA                    EOT
  --------------------------------------------------------------------------------------------

3.  ALL_DONE : Second MT of reused MT done or normal MT (not reused MT) done

4.  PARTIAL_DONE : first MT of reused MT done

5.  ABORT_DOME : MT abort occur

6.  EOT : Over error threshold occur

7.  STA : status compare fail occur

8.  MT_STOP: LCA compare fail, Timeout, CRC16 fail, CRC32 fail, signoff mask, UNC, Unzip fail, Zip fail, ~~erased page,~~ erased suspend.

9.  PFA: Program fail when auto polling

10. INDEX_MODE: [FCON_MT_ADR_BASE\[17\]](#FCON_MT_ADR_BASE), using index mode for interrupt message dispatch

11. CPU_INT : EOT, Status fail with auto polling, MT_STOP

**The interrupt can be defined disable or enable:**

[FCON_TMR_CTL\[24\]](#FCON_TMR_CTL) : timer interrupt enable

[FCTL_INT_CFG\[15\]](#FCTL_INT_CFG) : ALL_DONE interrupt enable

[FCTL_INT_CFG\[14\]](#FCTL_INT_CFG) : PARTIAL_DONE interrupt enable

[FCTL_INT_CFG\[13\]](#FCTL_INT_CFG) : DMA_DONE interrupt enable

[FCTL_INT_CFG\[12\]](#FCTL_INT_CFG) : ABORT_DONE interrupt enable

[FCTL_INT_CFG\[11\]](#FCTL_INT_CFG) : Status fail with auto polling interrupt enable

[FCTL_INT_CFG\[10\]](#FCTL_INT_CFG) : LCA or FW compare fail interrupt enable

[FCTL_INT_CFG\[9\]](#FCTL_INT_CFG) : Erased page interrupt enable

[FCTL_INT_CFG\[8\]](#FCTL_INT_CFG) : Zip fail interrupt enable

[FCTL_INT_CFG\[7\]](#FCTL_INT_CFG) : Unzip fail interrupt enable

[FCTL_INT_CFG\[6\]](#FCTL_INT_CFG) : CRC16 fail interrupt enable

[FCTL_INT_CFG\[5\]](#FCTL_INT_CFG) : over error interrupt enable

[FCTL_INT_CFG\[4\]](#FCTL_INT_CFG) : CRC32 fail interrupt enable

[FCTL_INT_CFG\[3\]](#FCTL_INT_CFG) : compare fail interrupt enable

[FCTL_INT_CFG\[2\]](#FCTL_INT_CFG) : signoff mask interrupt enable

[FCTL_INT_CFG\[1\]](#FCTL_INT_CFG) : Uncorrectable interrupt enable

**The CPU interrupt can be defined disable or enable:**

[FCON_TMR_CTL\[25\]](#FCON_TMR_CTL) : timer CPU interrupt enable

[FCTL_INT_CFG_1\[10\]](#FCTL_INT_CFG_1) : Program fail when auto polling CPU interrupt enable

[FCTL_INT_CFG_1\[9\]](#FCTL_INT_CFG_1) : Status fail with auto polling CPU interrupt enable

[FCTL_INT_CFG_1\[8\]](#FCTL_INT_CFG_1) : Erased page CPU interrupt enable

[FCTL_INT_CFG_1\[7\]](#FCTL_INT_CFG_1) : Zip CPU interrupt enable

[FCTL_INT_CFG_1\[6\]](#FCTL_INT_CFG_1) : Unzip CPU interrupt enable

[FCTL_INT_CFG_1\[5\]](#FCTL_INT_CFG_1) : CRC16 fail CPU interrupt enable

[FCTL_INT_CFG_1\[4\]](#FCTL_INT_CFG_1) : Uncorrectable ECC CPU interrupt enable

[FCTL_INT_CFG_1\[3\]](#FCTL_INT_CFG_1) : Signoff mask CPU interrupt enable

[FCTL_INT_CFG_1\[2\]](#FCTL_INT_CFG_1) : Over error CPU interrupt enable

[FCTL_INT_CFG_1\[1\]](#FCTL_INT_CFG_1) : CRC32 fail CPU interrupt enable

[FCTL_INT_CFG_1\[0\]](#FCTL_INT_CFG_1) : Compare LCA FW CPU interrupt enable

System queue setting information as below

1.  Normal TARGET_CPU: For 8 CPU system when DMA done without fail case, MT has 3 bit to indicate which CPU needs to accept the current interrupt event.0: CPU0, 1:CPU1..., 7:CPU7

2.  Error TARGET_CPU: For 8 CPU system when interrupt with fail case, MT has 3 bit to indicate which CPU needs to accept the current interrupt event.0: CPU0, 1:CPU1..., 7:CPU7

1\. CHN_NUM is 8 bits defined as total channel number of flash IP.

2\. DAT_WTH is 67 bits defined as the extend interrupt response format.

The fail case occur and the enable bit be enable, MTQ will be stop, and [FCTL_INT_INF\[0\]](#FCTL_INT_INF) will set to high. System can into error handle, and it also continue next MTQ, just clear [FCTL_INT_INF\[0\]](#FCTL_INT_INF).

**Timer interrupts:**

FIP add a timer in each channel, control register is FCON_TMR_CTL, when [FCON_TMR_CTL\[24\]](#FCON_TMR_CTL) enable, and timer counter equal [FCON_TMR_CTL\[23:0\]](#FCON_TMR_CTL), FIP will send a time out interrupt, and interrupt vector\[26\] will be set.

Timer counter start count is FPU operation trigger, and stop is FPU done, if FPU DMA stuck, it will time out. The interrupt data format as below.

## 4.14. Multiple Trigger Queue

In order to FW usage, it needs a queue to collect trigger event, and then trigger it one by one, to reduce register, we create 8 trigger group in the hardware, each trigger have some parameter in the internal SRAM. When the state machine receive trigger event, it will load the parameter to register group, then wait finish to do next trigger event.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image37.png){width="5.288194444444445in" height="3.74375in"}

The MT state machine will decide which one trigger event need to operation, and read parameter form internal SRAM first, the load to channel register, after parameter update. Then trigger custom sequence to do flash DMA read/write or send command sequence, when finish this trigger, hardware will check next trigger event operation or not, the simple multiple trigger queue flow show below:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image38.emf)

The trigger event result (ECC information) will store to each register, and the MT parameter have two format, one is fix format, include CE value, FPU pointer, BUF_VALID,..., another is define register location information, this can support more flexible to update MT parameter.

**Fix format (0x04 b31=0)**

**Detail format description:**

+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| Offset | Name                  | Bit range | Function define                        | Mapping Register                       |
+========+=======================+===========+========================================+========================================+
| 0x00   | fpu_en                | 0         | enable FPU trigger in MT\              | 　                                     |
|        |                       |           | 1b: enable                             |                                        |
| 　     |                       |           |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | pfa_int_en            | 1         | poll fail interrupt enable\            | FCTL_INT_CFG\[11\]                     |
|        |                       |           | 1b: enable                             |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | auto gen fail for R   | 2         | auto generate DMA read UNC             | FCTL_UNC_CFG\[16\]                     |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | auto gen fail for W   | 3         | auto generate program fail             | FCTL_UNC_CFG\[17\]                     |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | busy                  | 4         | trigger MT status as internal busy\    | 　                                     |
|        |                       |           | 1b: set internal busy                  |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | Query check           | 5         | MT query flag enable for flash read    | 　                                     |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | sta_no_stop           | 6         | status error don\'t stop MT\           | FCTL_INT_CFG\[26\]                     |
|        |                       |           | 1b: enable                             |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | es_en                 | 7         | enable erase suspend function\         | FCTL_ERASE_CFG\[30\]                   |
|        |                       |           | 1b: enable                             |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | bc_en                 | 8         | enable bad column function\            | FCTL_BAD_COL_CTL\[25\]                 |
|        |                       |           | 1b: enable                             |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | bc_group              | 15:9      | select bad column group                | FCTL_BAD_COL_SET\[30:24\]              |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | reserved              | 31:16     |                                        |                                        |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x04   | conv_page             | 8:0       | page for conversion                    | FCTL_OTHER_SET\[24:16\]                |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | conv_bps              | 9         | bypass conversion                      | FCTL_CHNL_SET\[9\]                     |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | conv_page_en          | 10        | enable conversion bypass\              | FCTL_OTHER_SET\[25\]                   |
|        |                       |           | 1b: enable                             |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | alu_sel               | 12:11     | select ALU group                       | FCTL_DMA_CFG\[5:4\]                    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | Ultra dma disable     | 13        | 1b: this MT will not execute ultra dma | 　                                     |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | int_vct_en            | 14        | enable interrupt vector response\      | FCTL_INT_CFG\[0\]                      |
|        |                       |           | 1b: enable                             |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | iFSA_en               | 15        | Intenal FSA enable                     | [FCTL_FSA_SEL](#FCTL_FSA_SEL)\[18\]    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | rs_page               | 23:16     | rs page number                         | FCTL_RS_CFG\[23:16\]                   |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | rs_encode_mode        | 24        | 0b: nornal mode 1b: XOR mode           | FCTL_RS_CFG\[0\]                       |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | rs_last_page          | 25        | last page valid for RS frame           | FCTL_RS_CFG\[3\]                       |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | rs_parity_2nd_page    | 26        | 1b: second page of rs parity           | FCTL_RS_CFG\[4\]                       |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | rs_dis_par_cnt        | 27        | 1b:RS decrease PEC count               | FCTL_RS_CFG\[5\]                       |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | rs_one_parity_en      | 28        | rs one parity rnable                   | FCTL_RS_CFG\[24\]                      |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | reserved              | 30:29     | 　                                     | 　                                     |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | write_reg_mode        | 31        | 　                                     | 　                                     |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x08   | nor_tar_cpu           | 2:0       | normal target cpu of interrupt         | FCTL_INT_VCT\[2:0\]                    |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | err_tar_cpu           | 5:3       | error target cpu of interrupt          | FCTL_INT_VCT\[5:3\]                    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | reserved              | 7:6       |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | int_vct               | 31:8      | interrupt vector information           | FCTL_INT_VCT\[31:8\]                   |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x0C   | L4K_BADR              | 31:0      | Buffer pointer and copy buffer pointer | FCTL_DMA_ADR_0                         |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x10   | fsa_sel               | 6:0       | select FSA group                       | FCTL_FSA_SEL\[6:0\]                    |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | fsa2_sel              | 13:7      | select 2nd FSA group                   | FCTL_FSA_SEL\[14:8\]                   |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | fsa2_cnt              | 15:14     | 2nd FSA group counter                  | FCTL_FSA_SEL\[7:6\]                    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | fw_3                  | 23:16     | fw_3                                   | FCTL_ZIP_CFG\[15:8\]                   |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | reserved              | 31:24     |                                        |                                        |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x14   | seed_init             | 31:0      | randomize block seed                   | FCTL_SEED_INIT                         |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x18   | bit_map               | 31:0      | zip bit map                            | FCTL_ZIP_MAP\[31:0\]                   |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x1C   | fpu_ptr               | 15:0      | FPU data pointer in IRAM               | FCTL_FPU_ADR\[15:0\]                   |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | ce_value              | 23:16     | CE value                               | FCON_FCE_ENB\[7:0\]                    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | ce_delay              | 27:24     | delay after CE change (for tCS)        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | pol_seq_sel           | 30:28     | select polling sequence                | 3\'h0: select FCTL_POL_SEQ_0\          |
|        |                       |           |                                        | 3\'h1: select FCTL_POL_SEQ_1\          |
|        |                       |           |                                        | ...\                                   |
|        |                       |           |                                        | 3\'h7: select FCTL_POL_SEQ_7           |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | reserved              | 31        |                                        |                                        |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x20   | fpu_ptr#2             | 15:0      | FPU data pointer in IRAM               | FCTL_FPU_ADR\[15:0\]                   |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | fsa_sel#2             | 22:16     | select FSA group                       | FCTL_FSA_SEL\[6:0\]                    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | reserved              | 23        |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | busy#2                | 24        | trigger MT status as internal busy\    |                                        |
|        |                       |           | 1b: set internal busy                  |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | int_vct_en#2          | 25        | enable interrupt vector response\      | FCTL_INT_CFG\[0\]                      |
|        |                       |           | 1b: enable                             |                                        |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x24   | l4k_spr_ptr           | 15:0      | Indicate L4K table address in IRAM\    | FCTL_L4K_SPR_ADR\[15:0\]               |
|        |                       |           | unit is Bytes, but need 8Bytes align   |                                        |
| 　     |                       |           |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | l4k_num               | 21:16     | l4k number                             | FCTL_ZIP_CFG\[5:0\]                    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | reserved              | 23:22     |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | frame_num             | 26:24     | DMA data frame number                  | FCTL_DMA_CFG\[10:8\]                   |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | zip_bps               | 27        | zip_bypass                             | FCTL_ZIP_CFG\[6\]                      |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | cmp_en                | 28        | enable compare LCA/FW function\        | FCTL_CMP_CFG\[0\]                      |
|        |                       |           | 1b: enable                             |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | l4k_badr_cont_mode_en | 29        | L4K_BADR continous mode enable         | 　FCTL_ZIP_CFG\[7\]                    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | force_fix_data        | 30        | force fix data enbale                  | [FCTL_OTHER_SET](#FCTL_OTHER_SET)\[8\] |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | buf_mode              | 31        | buffer mode\                           | FCTL_DMA_CFG\[31\]                     |
|        |                       |           | 1b: enable                             |                                        |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x28   | ce_select_mode        | 0         | select CE mode\                        |                                        |
|        |                       |           | 0b: CE value\                          |                                        |
| 　     |                       |           | 1b: CE number                          |                                        |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
|        |                       |           |                                        |                                        |
| 　     |                       |           |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | mgm_en                | 1         | enable mt group function\              | FCTL_MGM_CFG\[0\]                      |
|        |                       |           | 1b: enable                             |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | mgm_grp               | 7:2       | select mt group                        | FCTL_MGM_CFG\[6:1\]                    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | io_typ                | 8         | select IO pad type                     | FCTL_IO_SET\[25\]                      |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | fclk_div_en           | 9         | 　                                     | FCTL_FCLK_DIV\[6\]                     |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | fclk_div              | 15:10     | 　                                     | FCTL_FCLK_DIV\[5:0\]                   |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | flh_typ               | 17:16     | flash interface type                   | FCTL_FLH_SET\[1:0\]                    |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | tim_cfg_sel           | 18        | select timing config\                  | FCTL_DMA_CFG\[0\]                      |
|        |                       |           | 0b: select FCON\_\*\_TIME_CFG\         |                                        |
|        |                       |           | 1b: select FCON\_\*\_TIME_CFG_1        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | up_pol_seq            | 19        | update polling sequence in MT\         | up_pol_seq                             |
|        |                       |           | 1b: enable                             |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | update_ce             | 20        | update CE value\                       | update_ce                              |
|        |                       |           | 1b: update FCON_FCE_ENB                |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | reserved              | 23:21     |                                        |                                        |
|        +-----------------------+-----------+----------------------------------------+----------------------------------------+
|        | fta\[7:0\]            | 31:24     | flash target address                   | FCTL_FTA_COL\[7:0\]                    |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x2c   | iFSA0                 | 31:0      |                                        | FCTL_IFSA0                             |
|        |                       |           |                                        |                                        |
|        |                       |           |                                        | {FCTL_FTA_ROW\[23:0\],\                |
|        |                       |           |                                        | FCTL_FTA_COL\[15:8\]}                  |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x30   | iFSA1                 | 31:0      | 　                                     | FCTL_IFSA1　                           |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x34   | iFSA2                 | 31:0      |                                        | FCTL_IFSA2                             |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x38   | iFSA3                 | 31:0      | 　                                     | FCTL_IFSA3                             |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+
| 0x3c   | iFSA0#2               | 31:0      |                                        | FCTL_IFSA0                             |
+--------+-----------------------+-----------+----------------------------------------+----------------------------------------+

(\*1) If this bit is 1, FW can trigger the same MT table two times in order.

HW will executed the FPU sequence of fpu_ptr and fpu_ptr#2.

**Flexible format (0x04 b31=1)**

**Detail format description:**

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image39.emf){width="6.688194444444444in" height="2.615972222222222in"}

For reduce reload MT parameter overhead time, it will design a pipeline machine, in order to keep the pipeline parameter, all domain parameter will use to keep each trigger parameter including CE value.

Example:

Relation register

FCTL_MT_TRIG : MTQ trigger bit

b7 : queue 7 push bit

b6 : queue 6 push bit

b5 : queue 5 push bit

b4 : queue 4 push bit

b3 : queue 3 push bit

b2 : queue 2 push bit

b1 : queue 1 push bit

b0 : queue 0 push bit

FCTL_MT_ADR_0 : MTQ base adr for queue0

FCTL_MT_ADR_1 : MTQ base adr for queue1

FCTL_MT_ADR_2 : MTQ base adr for queue2

FCTL_MT_ADR_3 : MTQ base adr for queue3

FCTL_MT_ADR_4 : MTQ base adr for queue4

FCTL_MT_ADR_5 : MTQ base adr for queue5

FCTL_MT_ADR_6 : MTQ base adr for queue6

FCTL_MT_ADR_7 : MTQ base adr for queue7

**MT special function:**

For performance issue, FIP add a decide rule in MTQ operation, when MTQ_ADR bit\[0\] is high, finish this queue operation, it will first decide next MTP at the same queue,

Example:

Queue0 : event_1,event_2,...

Queue1 : event_8,event_9,...

Queue2 : event_16,event_17,...

Queue3 : event_24,event_25,...

If MTQ_ADR of event_1 address bit0 is high, when event_1 operation done, the next event will be select event_2, if event_2 address bit0 is high, the next event will be select event_3, if not equal high, it maybe select event 3 or 8 or 16 or 24.

**MT parameter for CE:**

Because CE of MT parameter is at FCON, FCON is common by each channel, so MT cannot directly update 32bit value to FCON_FCE_ENB, so we only allot 8bit for one channel.

Mapping rule is below:

Channel 0 MT only update [FCON_FCE_ENB\[7:0\]](#FCON_FCE_ENB)

Channel 1 MT only update [FCON_FCE_ENB\[15:8\]](#FCON_FCE_ENB)

Channel 2 MT only update [FCON_FCE_ENB\[23:16\]](#FCON_FCE_ENB)

Channel 3 MT only update [FCON_FCE_ENB\[31:24\]](#FCON_FCE_ENB)

Channel 4 MT only update [FCON_FCE_ENB_1\[7:0\]](#FCON_FCE_ENB_1)

Channel 5 MT only update [FCON_FCE_ENB_1\[15:8\]](#FCON_FCE_ENB_1)

Channel 6 MT only update [FCON_FCE_ENB_1\[23:16\]](#FCON_FCE_ENB_1)

Channel 7 MT only update [FCON_FCE_ENB_1\[31:24\]](#FCON_FCE_ENB_1)

Example:

If MTP_0x3C\[7:0\] = 0x02 in CE value mode,

or MTP_0x3C\[7:0\] = 0x01 in CE number mode

MTQ of channel 0, is mean select 2nd CE of channel 0. This is equivalent to CE1 in FCON_FCE_ENB.\
MTQ of channel 1, is mean select 2nd CE of channel 1. This is equivalent to CE9 in FCON_FCE_ENB.\
MTQ of channel 2, is mean select 2nd CE of channel 2. This is equivalent to CE17 in FCON_FCE_ENB.\
MTQ of channel 3, is mean select 2nd CE of channel 3. This is equivalent to CE25 in FCON_FCE_ENB.\
MTQ of channel 4, is mean select 2nd CE of channel 4. This is equivalent to CE33 in FCON_FCE_ENB_1.\
MTQ of channel 5, is mean select 2nd CE of channel 5. This is equivalent to CE41 in FCON_FCE_ENB_1.\
MTQ of channel 6, is mean select 2nd CE of channel 6. This is equivalent to CE49 in FCON_FCE_ENB_1.\
MTQ of channel 7, is mean select 2nd CE of channel 7. This is equivalent to CE57 in FCON_FCE_ENB_1.

All channel\'s MTP_0x3C\[7:0\] should used in with MTP when ce_select_mode = 0

0\~7 value of MTP_0x3C\[7:0\] when ce_select_mode = 1.

Note: fpu_en of MT format always should set 1'b1 in fix mode.

## 4.15. Auto Status Polling RDY with MT

Application of MTQ is co-operate by flash ready/busy behavior, when send command sequence to this flash, it will let the flash into busy, the MTQ of this flash will be mask, the next command sequence will not be execute. FIP will check the ready/busy is busy or not, if not busy, the next command sequence will be executed. So FIP need each flash ready/busy pin to decide which one queue is busy

Base operation like below:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image40.wmf)

The arbiter rule of MTQ is run-robin, the priority is

1.  T1\>T2\>T3\>T4

2.  T2\>T3\>T4\>T1

3.  T3\>T4\>T1\>T2

4.  T4\>T1\>T2\>T3

Each clock cycle, we will change the priority, so execute probability of each queue is equal.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image41.png){width="3.536111111111111in" height="3.2958333333333334in"}

**Auto send read status to update internal RDY:**

When system cannot support each RDY of flash, or the flash is multiple die, only support one CE and one RDY, the FIP cannot operation like above.

So FIP support a function, FIP create internal RDY bit information, use the RDY bit to indicate which one flash busy.

MTP have a parameter call "busy", location in MTP_0x00\[6\], when this MTP be execute, flash will be set busy, the "busy" bit need descript 1, and the internal RDY information of this queue will be set one.

When [FCON_SPECIAL\[8\]](#FCON_SPECIAL) as 1'b1, FIP will use the rising event to clear internal RDY, the rising event will from flash RDY pin, when [FCON_SPECIAL\[8\]](#FCON_SPECIAL) as 1'b0, when internal RDY be set, the queue will be force auto send read status, it will send read status to flash and check status is ready or busy, the read status sequence is also FPU sequence, and the entry pointer define in FCTL_POL_SEQ_ENTRY\_\*. DAT FPU sequence is adopted as read status sequence, and the compared condition as below.

Where RDY = (read data & MASK) != DAT_L, the MASK = DAT_H.

Work example like below:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image42.png){width="6.0in" height="4.120138888888889in"}

Polling sequence will change as below behavior:

## 4.16. CRC Function

FIP support two kinds of CRC check, CRC-16 and CRC-32.

- CRC-32

> If CRC-32 function be enable([FCON_CRC_EN\[0\]](#FCON_CRC_EN) as 1'b1), FIP will calculate CRC by physical data(4KB) and p4k table(first 12B). The result CRC parity (4B) will be placed after spare. These (4K+12+4) bytes are protected by BCH.
>
> When flash read, FIP will check CRC result is pass or fail and show in [FCTL_MAP_INF](#FCTL_MAP_INF), each bit indicate each CRC status of ECC frame, when ECC error occur, correct circuit will create new CRC parity at correct operation, FIP can check CRC status again, so FIP do not read output data again to re-calculate CRC parity.

If this function is disabled, FIP will not check CRC status and the physical data format keeps 4K+16+BCH+LDPC (still generate CRC parity to P4K, but not check the result).

- CRC-16

This function is for HW end to end check.

When flash read, ECC engine will generate CRC-16 per 512B physical data. ZIP will check the CRC-16.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image44.emf){width="3.85625in" height="1.863888888888889in"}

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image45.emf){width="5.6722222222222225in" height="1.1041666666666667in"}

## 4.17. Memory Copy Function

For enhance CPU setting parameter performance, IRAM data copy machine addition, one is IRAM to IRAM, we call "IRC", another is dram to/from IRAM, we call "D2I".

**IRC function:**

This function is for MT parameter, first we need prepare a default setting of MTP in the IRAM, like below:

> ![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image46.emf){width="5.768055555555556in" height="2.4402777777777778in"}

Set register:

1.  Set default MTP location to source address by FCON_IRC_ADR_0\[15:0\], 8Bytes align necessary

2.  Set destination address by FCON_IRC_ADR_0\[31:16\], 8Bytes align necessary, transfer unit is MT_TABLE_SIZE( FCON_MT_ADR_BASE\[21:18\])\*8 Bytes

3.  Set number to [FCON_IRC_CTL\[6:0\]](#FCON_IRC_CTL), address offset is MT_TABLE_SIZE \*8 Bytes

4.  Trigger [FCON_IRC_CTL\[7\]](#FCON_IRC_CTL)

> When trigger done, it will number of MTP in the destination address.
>
> Ex:
>
> MT_TABLE_SIZE = 56Bytes
>
> FCON_IRC_CTL\[6:0\] = 0x03
>
> FCON_IRC_ADR_0\[15:0\] = 0x0000
>
> FCON_IRC_ADR_0\[31:16\] = 0x0200
>
> After trigger FCON_IRC_CTL\[7\],

3 MTP will in 0x0200, 0x0238, and 0x0270

**D2I function:**

This function is for spare collect or place, in system design, they want prepare a lot of spare information in Dram, size is 512Bytes, format is below:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image47.emf){width="5.768055555555556in" height="1.25625in"}

It will mapping to IRAM spare region, size is 2Kbytes, and format is below:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image48.emf){width="5.768055555555556in" height="1.7756944444444445in"}

D2I function is 512Bytes to/from 2Kbytes copy machine, flow is

1.  Set DRAM address to FCON_D2I_DRADR

2.  Set IRAM address to FCON_D2I_IRADR

3.  Set direction to [FCON_D2I_CTL\[1\]](#FCON_D2I_CTL)

4.  Trigger [FCON_D2I_CTL\[0\]](#FCON_D2I_CTL)

D2I function is also support 512bytes to/from 512bytes, or 128bytes to/from 128bytes direct move function, setting like below:

> 512 🡨🡪 2K : FCON_D2I_CTL\[2\] = 0, FCON_D2I_CTL\[3\] = 0
>
> 512 🡨🡪 512 : FCON_D2I_CTL\[2\] = 1, FCON_D2I_CTL\[3\] = 0
>
> 128 🡨🡪 128 : FCON_D2I_CTL\[2\] = 1, FCON_D2I_CTL\[3\] = 1

## 4.18. Flash CE Remapping Matrix

MTP include CE control, and each MT update need independent, so each channel MT only can update fix CE control bit,

Channel 0 MT only update [FCON_FCE_ENB\[7:0\]](#FCON_FCE_ENB)

Channel 1 MT only update [FCON_FCE_ENB\[15:8\]](#FCON_FCE_ENB)

Channel 2 MT only update [FCON_FCE_ENB\[23:16\]](#FCON_FCE_ENB)

Channel 3 MT only update [FCON_FCE_ENB\[31:24\]](#FCON_FCE_ENB)

Channel 4 MT only update [FCON_FCE_ENB_1\[7:0\]](#FCON_FCE_ENB_1)

Channel 5 MT only update [FCON_FCE_ENB_1\[15:8\]](#FCON_FCE_ENB_1)

Channel 6 MT only update [FCON_FCE_ENB_1\[23:16\]](#FCON_FCE_ENB_1)

Channel 7 MT only update [FCON_FCE_ENB_1\[31:24\]](#FCON_FCE_ENB_1)

But each channel only control some CE, so we add a remapping matrix between control register and CE module port, system can use matrix to re-map CE, so each can control each CE and the same MT, maximum CE number is 8.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image49.emf)

For example:

FCON_FCE_SEL_0\~ FCON_FCE_SEL_16 rule is below

70004980 00000000 18081000 38302820 19091101

70004990 39312921 1A0A1202 3A322A22 1B0B1303

700049A0 3B332B23 1C0C1404 3C342C24 1D0D1505

700049B0 3D352D25 1E0E1606 3E362E26 1F0F1707

700049C0 3F372F27 00000000 00000000 00000004

When FCON_FCE_ENB=0x1 -\> BIT 0 valid -\> FCE 0 will be enable when

FCON_FCE_ENB=0x2 -\> BIT 1 valid -\> FCE 8 will be enable when

FCON_FCE_ENB=0x4 -\> BIT 2 valid -\> FCE 16 will be enable when

FCON_FCE_ENB=0x8 -\> BIT 3 valid -\> FCE 24 will be enable

Because,

\"FCON_FCE_SEL_0\" 4 byte is direct mapping to Flash CE 0,1,2,3

\"FCON_FCE_SEL_1\" 4 byte is direct mapping to Flash CE 4,5,6,7 \....

\"FCON_FCE_SEL_16\" 4 byte is direct mapping to Flash CE 60,61,62,63

For example

When flash scan and get the CE2, CE 8, CE 15, CE 18 are valid,

it want to used 0x01,0x02,0x04,0x08 to mapping,

Register FCON_FCE_SEL, should be set to

FCON_FCE_ENB = 0x01, BIT0 -\> FCON_FCE_SEL_0 = 0xFF00FFFF FCON_FCE_ENB

= 0x02, BIT1 -\> FCON_FCE_SEL_2 = 0xFFFFFF01 FCON_FCE_ENB = 0x04, BIT2

FCON_FCE_SEL_3 = 0x02FFFFFF FCON_FCE_ENB = 0x08, BIT3 -\>

FCON_FCE_SEL_4 = 0xFF03FFFF

For example

If FCON_FCE_SEL_0 = 0x160C031D,

It is mean

To set FCON_FCE_ENB = 0x400000, -\> BIT 22 valid , the Flash FCE 3 will be enable

To set FCON_FCE_ENB = 0x1000, -\> BIT 12 valid , the Flash FCE 2 will be enable

To set FCON_FCE_ENB = 0x8, -\> BIT 3 valid , the Flash FCE 1 will be enable

To set FCON_FCE_ENB = 0x20000000, -\> BIT 29 valid , the Flash FCE 0 will be enable

## 4.19. MTQ Manual Abort

When system want to directly stop MT, it can set some register to stop MTQ, but MTQ just stop next FPU event to start, so system need wait some signal to wait HW idle, please follow below flow:

1.  Set [FCTL_INT_CFG\[24\]](#FCTL_INT_CFG)

2.  Polling [FCTL_FPU_TRIG\[0\]](#FCTL_FPU_TRIG) and [FCTL_FPU_TRIG\[1\]](#FCTL_FPU_TRIG) equal 0

> Polling [FCTL_RESERVE](#FCTL_RESERVE)\[0\] equal 1

3.  Now, FPU is idle and MT is stop. FW clears [FCTL_INT_CFG](#FCTL_INT_CFG)\[12\] and then do anything using FPU or PIO

4.  Clear MTQ number if you want by using [FCTL_MT_TRIG\[15:8\]](#FCTL_MT_TRIG)(optional)

> Set [FCTL_MT_TRIG\[15:8\]](#FCTL_MT_TRIG) as 1'b1 to clear every MTPs of MTQ. MTQ status is empty, and new MTP need to push into MTQ.
>
> If [FCTL_MT_TRIG\[15:8\]](#FCTL_MT_TRIG) set as 0. MTQ status will keep previous MTP, and next MTP of MTQ will be executed after abort status is released.

5.  Set [FCTL_MT_TRIG\[31\]](#FCTL_MT_TRIG) to softly reset MT control

6.  Clear [FCTL_INT_CFG\[24\]](#FCTL_INT_CFG)

If MTQ stop is occur uncorrectable, or CRC fail, or FPU compare fail, or signoff mask occur, over error occur.

> The operation flow is:

1.  Receive error interrupt vector.

2.  Polling [FCTL_FPU_TRIG\[0\]](#FCTL_FPU_TRIG) and [FCTL_FPU_TRIG\[1\]](#FCTL_FPU_TRIG) equal 0.

3.  HW true idle, clear [FCTL_INT_CFG](#FCTL_INT_CFG)\[12\] and then enter FW error handle.

4.  Clear [FCTL_INT_INF\[0\]](#FCTL_INT_INF) to continue MTQ.

Note: For MTQ abort process:

1.  FW need to set FCON_FCE_ENB for each FPU to confirm CE correctly .

## 4.20. CNT ONE Counter

In flash read, it will a counter to count input data number of 1/0 bit, the enable bit is [FCTL_CNT_ONE\[30\]](#FCTL_CNT_ONE), if enable, when reach DMA data read start, the counter will be reset, and then counter data number of 1/0 bit, [FCTL_CNT_ONE\[31\]](#FCTL_CNT_ONE) can control count 0 or 1, and the result will be store in [FCTL_CNT_ONE\[17:0\]](#FCTL_CNT_ONE).

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image50.png){width="6.279861111111111in" height="1.6569444444444446in"}

## 4.21. AXI Slave Function

AXI slave support two function:

1.  Mapping mode: MT data move is based on 512 bytes to 512 bytes.

2.  Remapping mode: Spare data move is based on 512 bytes to 2K bytes.

AXI remapping range function of IRAM that is defined by using FCON_AXIS_ADR.

And this function is enabled by[FCON_SPECIAL](#FCON_SPECIAL)\[12\],FCON_AXIS_ADR\[15:0\] is indicated that the start remapping address, and FCON_AXIS_ADR\[31:16\] is indicated that the end remapping end address.

Out of remapping range is defined as mapping mode, and [FCON_SPECIAL\[12\]](#FCON_SPECIAL) is to check AXI slave address whether it is out of range or not.

## 4.22. LCA FW Compare Function

Data compare format is based on 2K data unit, and expect data format is consists of \"LCA0(4B) + LCA1(4B)\" in the 8 bytes align IRAM. Expect data compare is fetched sequential from the start address of IRAM defined in FCTL_CMP_ADR register by Channel control register and MT control register.

MASK data is 2 bytes defined as the bit mask information of FW bytes in [FCTL_CMP_FW\[31:16\]](#FCTL_CMP_FW). Compare function only work for DMA read. IRAM size only support below 64KBytes capacity. The compare function control flow and data flow as below.

LCA_FW compare function check LCA and FW of spare data whether it is correct with expect data or not, and the function enable is set by [FCTL_CMP_CFG\[0\]](#FCTL_CMP_CFG). Extend interrupt format bit 25 will be set 1 if it occur compare fail. The interrupt equation is as below:

INT = CMP_EN & ((LCA_spare != LCA_expect) \|\| (MASK & FW_spare != 0))

LCA_spare is indicated that data from spare value.

FW_spare is indicated that data from spare value.

LCA_expect is indicated that data from IRAM.

MASK is set by FCTL_CMP_FW.

The FW data based on 4K format remapping HW data based on 2K format is as below:

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image51.emf){width="6.759722222222222in" height="3.063888888888889in"}

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image52.emf){width="2.5597222222222222in" height="2.1680555555555556in"}

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image53.emf){width="2.736111111111111in" height="6.472222222222222in"}

## 4.23. MTQ Force Empty

MTQ arbiter chose one entry from the not empty queues. As the below, empty status MTQ2 will be not executed by MTQ arbiter.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image54.png){width="3.7756944444444445in" height="3.879861111111111in"}

According to this conception, MTQ force empty function could stop the non-empty MTQ. Therefore, the execution of MTQ entry will be stopped at the FW requested position if FW enable this function. Each entry has set request bit to the force empty register. Control setting are the following.

1.  Add "set request bit to the force empty register" to the MTQ entry format.

2.  Add [FCTL_MTQ_CFG\[15:8\]](#FCTL_MTQ_CFG) "mask register" for the above "set request bit to the force empty register".

3.  Add [FCTL_MTQ_CFG\[7:0\]](#FCTL_MTQ_CFG) "force empty register" to force modify the empty/not empty signal from each queue.

Rule as below:

1.  set = !mask & set_request

2.  clear = !set_request;

3.  if (set) begin

> Force_empty \<= ture
>
> end else if (clear) begin
>
> Force_empty \<= false
>
> end

Note: CPU can set or clear force empty [FCTL_MTQ_CFG\[7:0\]](#FCTL_MTQ_CFG) register directly. When CPU set force empty status for MTQ, 0xF8 (FCTL_MT_PTR_DIF/ MT Pointer Different Register) can be checked whether current MTP is finished or not.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image55.png){width="6.463888888888889in" height="4.847916666666666in"}

## Read nand Flash soft data

> FIP supports a new function to read raw data from nand flash to DRAM. It can be used to read LDPC soft data to do soft bit correct.
>
> Step1. set [FCTL_DMA_CFG](#FCTL_DMA_CFG)\[10:8\] to frame number
>
> Step2. set [FCTL_RAW_DMA_ADR](#FCTL_RAW_DMA_ADR) to DRAM target address
>
> Step3. Trigger an opcode=0xA FPU sequence and wait fpu_bsy =0
>
> RAW (0xA)

  --------------------------------------------------------------------------
  Bit              15 -- 12  11 -- 6         5 -- 4          3 -- 0
  --------------- ---------- --------------- --------------- ---------------
  Value/Field        0xA     Not Used (NU)   Not Used (NU)   Not Used (NU)

  --------------------------------------------------------------------------

> To trigger read flash raw data to DRAM
>
> Each frame is stored in different offset as shown below.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image56.emf){width="6.695138888888889in" height="5.729800962379702in"}

## Backup-Restore IBF

In this FIP, the flash dma frame is temporarily buffered in IBF(internal buffer). For each channel, there are 5 banks of IBF. These IBF can't be read/written by CPU. FIP supports a function to transfer the IBF data to/from DRAM or IRAM. This function is applied to error handling such as LDPC soft bit correct and RS decode.

Step1. set [FCTL_RAW_DMA_ADR](#FCTL_RAW_DMA_ADR) to DRAM target/source address

Step2. If MODE=1 ,set [FCTL_BACK_STORE](#FCTL_BACK_RESTORE)\[31:16\] to IRAM targer/source address

Step3. Trigger an opcode=0xD FPU sequence and wait fpu_bsy =0

> BR (0xD)

  --------------------------------------------------------------------------
  Bit           15 -- 12   11:8   7:5       4:2         1           0
  ------------- ---------- ------ --------- ----------- ----------- --------
  Value/Field   0xD        NU     IBF ptr   Frame num   Direction   MODE

  --------------------------------------------------------------------------

> To backup/restore data between IBF and DRAM(IRAM)
>
> **IBF_PTR** : point to the start IBF.
>
> **Frame num**: indicate frame how many size to be transferred
>
> **Direction**: 1 means restore frame to IBF
>
> 0 means backup frame to DRAM/IRAM
>
> **MODE**: 1 only transfer DATA 4KB(to/from DRAM) + SPARE 16B(to/from IRAM)

1.  transfer 4KB + 16B + ECC(to/from DRAM)

\*if IBF_PTR + Frame num is bigger than 5, it will rotate back to IBF0

MODE = 0 (Ex. IBF_PTR = 3 ,Frame num = 4)

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image57.emf){width="5.796209536307962in" height="4.541891951006124in"}

MODE = 1 (Ex. IBF_PTR = 3 ,Frame num = 4)

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image58.emf){width="5.876971784776903in" height="4.00778980752406in"}

## FPU correct from IBF

In this FIP, the flash dma frame is temporarily buffered in IBF(internal buffer). For each channel, there are 5 banks of IBF. FIP supports a function to correct and decompress the user data and spare data from the IBF to DRAM and IRAM. The setting for this function is same as the normal read DMA.

Step1. Use Backup-Restore IBF to restore user data and spare data to IBF (PTR=1).

Step2. set all L4k for decompression, and all the settings are the same as normal read DMA.

Step3. Set LDPC_COR_MAP to disable or enable LDPC correct for each IBF. Set FCTL_OTHER \_SET\[10\] to disable or enable BCH correct. When use FPU_COR for the data from RS erasure mode, the LDPC and BCH should be turned off. When use FPU_COR for the data from RS normal mode, the LDPC and BCH should be turned on.

Step4. Trigger an opcode=0xB FPU sequence and wait fpu_bsy =0 ans srq_bsy=0

FPU COR (0xB)

  -----------------------------------------------------------------
  Bit           15 -- 12   11: 0
  ------------- ---------- ----------------------------------------
  Value/Field   0xB        NU

  -----------------------------------------------------------------

To correct and decompress the user data and spare data from the IBF to DRAM(IRAM)

## Hard Bit Retry

If a read DMA uncorrectable occurred, HW shows some information in [FCTL_MAP_CFG](#FCTL_MAP_CFG), [FCTL_MAP_INF](#FCTL_MAP_INF) and interrupt. FW can perform hard bit retry to recover data. But because of data compression, the error-free data from flash is not meaningful to FW. Thus, some control flow should be followed.

Initial setting

1.  Set zip bypass([FCTL_ZIP_CFG](#FCTL_ZIP_CFG)\[6\]) = 1

2.  Set conversion bypass ([FCTL_CHNL_SET](#FCTL_CHNL_SET)\[9\]) = 1

3.  Set FORCE_ZIP_COR ([FCTL\_](#FCTL_DYNAMIC_ODT)IBF_CTL\[9\]) = 1

Step1. Decide which physical frame should be read

> Ex. Physical frame 1 is uncorrectable and there is a logical frame cross physical frame 0 and frame 1. The frame 0 should be read.

Step2. Prepare L4K table and do Read Retry

Step3. Get uncorrectable frame map.

Step4. If there remains uncorrectable frame, go back to Step1.

Otherwise, all the error-free physical data are in DRAM. Then, go to Step5.

Step5. Do FPU(0xD) to restore data to IBF(ptr = 1).

Step6. Set the initial setting to original DMA setting.

Step7. Do FPU(0xB) to get logical data (reference chapter 4.26)

\<Note1\> For step 2 and step 6, FW also has to set the l4k num, zip map and some other settings.

\<Note2\> The purpose of step 2\~4 is to get error-free physical data. Any methods which achieve this are acceptable.

## Soft Bit Correct

To perform soft-bit decode, HW supports a FPU and some register.

A simple control flow is below

Initial setting

1.  Set decode mode([FCON_LDPC_CFG](#FCON_LDPC_CFG)\[6:4\]),

> page selection ([FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]),
>
> dsp enable([FCON_LDPC_CFG](#FCON_LDPC_CFG)\[26\]).

2.  Set LLR table (FCON_LLR_TABLE\_\*)

3.  Set FCON_DSP_PARAM\_\*

4.  Set FCON_DSP_GRAY_CODE\_\*

5.  Set FCON_DSP_LUT\_\*

Step1. Do FPU(0xA) to get all soft bit data (replace FPU(0x6) by FPU(0xA) in a dma FPU)

Step2. Do FPU(0xD) to restore data to IBF (reference chapter 4.25)

Step3. Do FPU(0xC)

Step4. [FCTL_LDPC_CFG](#FCTL_LDPC_CFG)\[15:14\] shows the result of BCH/LDPC

Step5. FCON_DSP_CENTER and [FCON_DSP_DISTRIBUTION_0](#FCON_DSP_DISTRIBUTION_0) shows the result of DSP.

> \<Note1\> FPU(0xC) is allowed to be executed in only one channel.

## HW DMA phase for FW debug

FIP provide some HW internal signal ([FCTL_DMA_STS_0](#FCTL_DMA_STS_0)) for FW debug when FIP hang.

When program flash, the data flow from system buffer to flash can be divided into three phases:

1.  ZIP 🡺 FIP internal buffer (IBF)

2.  FIP internal buffer (IBF) 🡺 flash interface

3.  Send INT to system and wait INT_ACK from system

> An example of normal 16KB program as shown below:
>
> ![C:\\Documents and Settings\\matt_wu\\Local Settings\\Temporary Internet Files\\Content.Word\\prog.png](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image59.png){width="6.695138888888889in" height="1.4509087926509185in"}

ZIP_IBF_STS\[3:0\] : when P4K data transfer from ZIP to IBF done, the corresponding bit will be set to high

IBF_FLH_STS\[3:0\] : when P4K data from IBF transfer to FLH done, the corresponding bit will be set to high

FLH_FRM_CNT: current P4K number transferring on flash interface

INT_PHS: when DMA done, FIP will send INT to system and wait INT_ACK

Flash_dma_phs: data transfer on flash interface

When read flash, the data flow from flash to system can be divided into three phases:

1.  flash 🡺 FIP internal buffer (IBF)

2.  FIP internal buffer (IBF) 🡺 ZIP

3.  Send INT to system and wait INT_ACK from system

An example of normal 16KB read as shown below:

![C:\\Documents and Settings\\matt_wu\\Local Settings\\Temporary Internet Files\\Content.Word\\read.png](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image60.png){width="6.695138888888889in" height="1.4812970253718285in"}

More examples of different frm_ptr and frm_num :

![D:\\yh_wang\\Desktop\\擷取.PNG](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image61.png){width="6.679166666666666in" height="1.9430555555555555in"}

![D:\\yh_wang\\Desktop\\擷取1.PNG](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image62.png){width="6.688888888888889in" height="2.0375in"}

![D:\\yh_wang\\Desktop\\擷取2.PNG](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image63.png){width="6.679166666666666in" height="2.0in"}

ZIP_IBF_STS\[3:0\] : when P4K data transfer from flash to IBF done, the corresponding bit will be set to high

IBF_FLH_STS\[3:0\] : when P4K data from IBF transfer to ZIP done, the corresponding bit will be set to high

Ultra_dma_phs: two dma overlap period (one is ECC decoding, the other is data transfer)

##  Buffer rotation

HW supports two groups of buffer region for rotation. It works if [FCTL_ZIP_CFG](#FCTL_ZIP_CFG)\[7\] \[16\] are enabled. Some settings are in system(reference PS3111_RegSpec_FLAG for First Start Address Register, Second Start Address Register, Flag End Address Register , Flag2 End Address Register).

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image64.emf)

5\. Register spec

The control register of FIP have three parts, flash TOP (FTOP), flash each channel (FCTL), and flash all channel (FALL).

Because each channel control independent, so if FIP is N channel, the FIP will have N register group for FCTL; FALL register is a fictitious register, when write all channel, it write to each channel register at the same time, it is in order to reduce CPU access time to setting same configuration for each channel. So all channel only have one, and register define will same as each channel, FALL also use channel mask to setting some channel register; FTOP is some common setting register, it usually setting in initial and not change in future operation, or all channel is use same setting, the kind of register will be place in top.

DLL controller register only one too, common setting for each channel,

## 5.1. Register of Each Channel

The address map of control register for each channel in below

  -------------------------------------------------------------------------------------
  Offset   Name                                      Mode   Function
  -------- ----------------------------------------- ------ ---------------------------
  0x00     [FCTL_PIO_CMD](#FCTL_PIO_CMD)             rw

  0x04     [FCTL_PIO_ADR](#FCTL_PIO_ADR)             rw

  0x08     [FCTL_PIO_DAT](#FCTL_PIO_DAT)             rw

  0x0C     [FCTL_DMA_ADR_0](#FCTL_DMA_ADR_0)         rw

  0x10     [FCTL_DMA_ADR_1](#FCTL_DMA_ADR_1)         rw

  0x14     [FCTL_DMA_ADR_2](#FCTL_DMA_ADR_2)         rw

  0x18     [FCTL_DMA_ADR_3](#FCTL_DMA_ADR_3)         rw

  0x1C     [FCTL_IO_SET](#FCTL_IO_SET)               rw

  0x20     [FCTL_ZIP_MAP](#FCTL_ZIP_MAP)             rw

  0x24     [FCTL_MT_TRIG](#FCTL_MT_TRIG)             rw

  0x28     [FCTL_MT_ADR_0](#FCTL_MT_ADR_0)           rw

  0x2C     [FCTL_MT_ADR_1](#FCTL_MT_ADR_1)           rw

  0x30     [FCTL_MT_ADR_2](#FCTL_MT_ADR_2)           rw

  0x34     [FCTL_MT_ADR_3](#FCTL_MT_ADR_3)           rw

  0x38     [FCTL_FTA_ROW](#FCTL_ROW_ADR)             rw

  0x3C     [FCTL_FTA_COL](#FCTL_COL_ADR)             rw

  0x40     [FCTL_DMA_CFG](#FCTL_DMA_CFG)             rw

  0x44     [FCTL_OTHER_SET](#FCTL_OTHER_SET)         rw

  0x48     [FCTL_FSA_SEL](#FCTL_FSA_SEL)             rw

  0x4C     [FCTL_UNC_CFG](#FCTL_UNC_CFG)             rw

  0x50     [FCTL_CNT_ONE](#FCTL_CNT_ONE)             rw

  0x54     [FCTL_ECC_INF](#FCTL_ECC_INF)             r

  0x58     [FCTL_INT_VCT](#FCTL_INT_VCT)             rw

  0x5C     [FCTL_SPR_ADR](#FCTL_SPR_ADR)             rw

  0x60     [FCTL_CMP_CFG](#FCTL_CMP_CFG)             rw

  0x64     [FCTL_INT_INF](#FCTL_INT_INF)             rw

  0x68     [FCTL_MAP_CFG](#FCTL_MAP_CFG)             rw

  0x6C     [FCTL_MAP_INF](#FCTL_MAP_INF)             r

  0x70     [FCTL_CMP_ADR](#FCTL_CMP_ADR)             rw

  0x74     [FCTL_RBY_INF](#FCTL_RBY_INF)             r

  0x78     [FCTL_CMP_FW](#FCTL_CMP_FW)               rw

  0x7C     [FCTL_FLH_SET](#FCTL_FLH_SET)             rw

  0x80     [FCTL_HS_MODE](#FCTL_HS_MODE)             rw

  0x84     [FCTL_INT_CFG](#FCTL_INT_CFG)             rw

  0x88     [FCTL_POL_SEQ_0](#FCTL_POL_SEQ_0)         rw

  0x8C     [FCTL_POL_SEQ_1](#FCTL_POL_SEQ_1)         rw

  0x90     [FCTL_SEED_INIT](#FCTL_SEED_INIT)         rw

  0x94     [FCTL_POL_SEQ_2](#FCTL_POL_SEQ_2)         rw

  0x98     [FCTL_POL_SEQ_3](#FCTL_POL_SEQ_3)         rw

  0x9C     [FCTL_BDC_CTL](#FCTL_BDC_CTL)             rw

  0xA0     [FCTL_RESERVE](#FCTL_RESERVE)             rw

  0xA4     [FCTL_BDC_SET](#FCTL_BDC_SET)             rw

  0xA8     [FCTL_POL_SEQ_4](#FCTL_POL_SEQ_4)         rw

  0xAC     [FCTL_POL_SEQ_5](#FCTL_POL_SEQ_5)         rw

  0xB0     [FCTL_POL_SEQ_6](#FCTL_POL_SEQ_6)         rw

  0xB4     [FCTL_POL_SEQ_7](#FCTL_POL_SEQ_7)         rw

  0xB8     [FCTL_FARG_ROW](#FCTL_FARG_ROW)           r

  0xBC     [FCTL_FARG_COL](#FCTL_FARG_COL)           r

  0xC0     [FCTL_FSA_ADR](#FCTL_FSA_ADR)             rw

  0xC4     [FCTL_CHNL_SET](#FCTL_CHNL_SET)           rw

  0xC8     [FCTL_INT_CFG_1](#FCTL_INT_CFG_1)

  0xCC     [FCTL_INT_RDY](#FCTL_INT_RDY)             rw

  0xD0     [FCTL_FPU_TRIG](#FCTL_FPU_TRIG)           rw

  0xD4     [FCTL_FPU_ENTRY](#FCTL_FPU_ENTRY)         rw

  0xD8     [FCTL_MT_ADR_4](#FCTL_MT_ADR_4)           rw

  0xDC     [FCTL_MT_ADR_5](#FCTL_MT_ADR_5)           rw

  0xE0     [FCTL_MT_ADR_6](#FCTL_MT_ADR_6)           rw

  0xE4     [FCTL_MT_ADR_7](#FCTL_MT_ADR_7)           rw

  0xE8     [FCTL_FAIL_CNT](#FCTL_FAIL_CNT)           rw

  0xEC     [FCTL_MTQ_CFG](#FCTL_MTQ_CFG)             rw

  0xF0

  0xF4     [FCTL_RAND_KEY](#FCTL_CNV_CFG)            rw

  0xF8     [FCTL_MTQ_INF_0](#FCTL_MTQ_INF_0)         rw

  0xFC     [FCTL_DBG_INF](#FCTL_DBG_INF)             r

  0x100    [FCTL_SEED_MODE](#FCTL_SEED_MODE)         rw

  0x104    [FCTL_ERASE_CFG](#FCTL_ERASE_CFG)         rw

  0x108    [FCTL_ERASE_SEQ](#FCTL_ERASE_SEQ)         rw

  0x10C    [FCTL_MGM_CFG](#FCTL_MGM_CFG)             rw

  0x110    [FCTL_MTQ_INF_1](#FCTL_MTQ_INF_1)         rw

  0x114    [FCTL_MTQ_DLY](#FCTL_MTQ_DLY)             rw

  0x118    [FCTL_FDIV_CFG](#FCTL_FDIV_CFG)           rw

  0x11C    [FCTL_NVME_CFG_0](#FCTL_NVME_CFG_0)       rw

  0x120    [FCTL_NVME_CFG_1](#FCTL_NVME_CFG_1)       rw

  0x124    [FCTL_LDPC_MASK](#FCTL_LDPC_MASK)         rw

  0x128    [FCTL_L4K_SPR_ADR](#FCTL_L4K_SPR_ADR)     rw

  0x12c    [FCTL_ZIP_CFG](#FCTL_ZIP_CFG)             rw

  0x130    [FCTL_RAW_DMA_ADR](#FCTL_RAW_DMA_ADR)     rw

  0x134    [FCTL_BACK_STORE](#FCTL_BACK_RESTORE)     rw

  0x138    [FCTL_RS_CFG](#FCTL_RS_CFG)               rw

  0x13c    [FCTL_LDPC_CFG](#FCTL_LDPC_CFG)           rw

  0x140    [FCTL_RD_CNT](#FCTL_RD_CNT)               RO

  0x144    [FCTL_RD2_CNT](#FCTL_RD2_CNT)             RO

  0x148    [FCTL_DYNAMIC_ODT](#FCTL_DYNAMIC_ODT)     rw

  0x14c    [FCTL\_](#FCTL_DYNAMIC_ODT)IBF_CTL        rw

  0x150    [FCTL_ECC_DBG](#FCTL_ECC_DBG)             RO

  0x154    [FCTL_IFSA0](#FCTL_IFSA0)                 rw

  0x158    [FCTL_IFSA1](#FCTL_IFSA1)                 rw

  0x15C    [FCTL_IFSA2](#FCTL_IFSA2)                 rw

  0x160    [FCTL_IFSA3](#FCTL_IFSA3)                 rw

  0x164    [FCTL_FPU_INT_VCT](#FCTL_FPU_INT_VCT)     RO

  0x168    [FCTL_DMA_DONE_VCT](#FCTL_DMA_DONE_VCT)   RO

  0x16C    [FCTL_FRONT_VCT](#FCTL_FRONT_VCT)         RO

  0x170    [FCTL_BACK_VCT](#FCTL_BACK_VCT)           RO

  0x174

  0x178

  0x17c    [FCTL_ZIP_DMA_ST](#FCTL_ZIP_DMA_ST)       RO

  0x180    [FCTL_E3D_ERR_CNT](#FCTL_E3D_ERR_CNT)     RO

  0x184    [FCTL_DMA_STS_0](#FCTL_DMA_STS_0)         RO
  -------------------------------------------------------------------------------------

  : Register list for internal channel setting

## 5.2. Details specification for Register of each channel

Register: []{#FCTL_PIO_CMD .anchor}FCTL_PIO_CMD/PIO Command Register (Offset 00h)

Register Index: 00h \~ 03h

Default Value: 0000_0000h

+------+-------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                  | Default | Attributes |
+:====:+===============================================================================+:=======:+:==========:+
| 31:8 | Reserved                                                                      | 00h     | RsvdP      |
+------+-------------------------------------------------------------------------------+---------+------------+
| 7:0  | Direct PIO command for flash interface                                        | 00h     | RW         |
|      |                                                                               |         |            |
|      | MCU can write this register to send command to flash interface,               |         |            |
|      |                                                                               |         |            |
|      | Ex. MCU write 10h to this register, flash interface will be send command 10h. |         |            |
+------+-------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_PIO_ADR .anchor}FCTL_PIO_ADR/PIO Address Register (Offset 04h)

Register Index: 04h \~ 07h

Default Value: 0000_0000h

+------+-------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                  | Default | Attributes |
+:====:+===============================================================================+:=======:+:==========:+
| 31:8 | Reserved                                                                      | 00h     | RsvdP      |
+------+-------------------------------------------------------------------------------+---------+------------+
| 7:0  | Direct PIO address for flash interface                                        | 00h     | RW         |
|      |                                                                               |         |            |
|      | MCU can write this register to send address to flash interface,               |         |            |
|      |                                                                               |         |            |
|      | Ex. MCU write 10h to this register, flash interface will be send address 10h. |         |            |
+------+-------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_PIO_DAT .anchor}FCTL_PIO_DAT/PIO Address Register (Offset 08h)

Register Index: 08h \~ 0Bh

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                                                       | Default | Attributes |
+:=====:+====================================================================================================================================================================================+:=======:+:==========:+
| 31:16 | Reserved                                                                                                                                                                           | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15:0  | Direct PIO data for flash interface                                                                                                                                                | 00h     | RW         |
|       |                                                                                                                                                                                    |         |            |
|       | MCU can read or write this register to send read / write data to flash interface,                                                                                                  |         |            |
|       |                                                                                                                                                                                    |         |            |
|       | Ex. MCU write 10h to this register, flash interface will be send write data 10h, if read this register, flash interface will be send read data, then MCU will get this data value. |         |            |
+-------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_DMA_ADR_0 .anchor}FCTL_DMA_ADR_0/ DMA Address 0 Register (Offset 0Ch)

Register Index: 0Ch \~ 0Fh

Default Value: 0000_0000h

+------+----------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                   | Default | Attributes |
+:====:+================================================================+:=======:+:==========:+
| 31:0 | DMA start buffer address 0                                     | 00h     | RW         |
|      |                                                                |         |            |
|      | indicate SRAM location, unit is Bytes, but need 512Bytes align |         |            |
|      |                                                                |         |            |
|      | Multi buffer pointer mode disable,                             |         |            |
|      |                                                                |         |            |
|      | and it will indicate SRAM location frame0\~31                  |         |            |
|      |                                                                |         |            |
|      | Multi buffer pointer mode enable,                              |         |            |
|      |                                                                |         |            |
|      | and it will indicate SRAM location                             |         |            |
|      |                                                                |         |            |
|      | Frame0 for 1K mode,                                            |         |            |
|      |                                                                |         |            |
|      | Frame0\~1 for 2K mode,                                         |         |            |
|      |                                                                |         |            |
|      | Frame0\~3 for 4K mode,                                         |         |            |
|      |                                                                |         |            |
|      | Frame0\~7 for 8K mode                                          |         |            |
+------+----------------------------------------------------------------+---------+------------+

Register: []{#FCTL_DMA_ADR_1 .anchor}~~FCTL_DMA_ADR_1/ DMA Address 1 Register (Offset 10h)~~

Register Index: 10h \~ 13h

Default Value: 0000_0000h

+------+--------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                       | Default | Attributes |
+:====:+====================================================================+:=======:+:==========:+
| 31:0 | ~~DMA start buffer address 1~~                                     | 00h     | RW         |
|      |                                                                    |         |            |
|      | ~~indicate SRAM location, unit is Bytes, but need 512Bytes align~~ |         |            |
|      |                                                                    |         |            |
|      | ~~Multi buffer pointer mode enable,~~                              |         |            |
|      |                                                                    |         |            |
|      | ~~and it will indicate SRAM location~~                             |         |            |
|      |                                                                    |         |            |
|      | ~~Frame1 for 1K mode,~~                                            |         |            |
|      |                                                                    |         |            |
|      | ~~Frame2\~3 for 2K mode,~~                                         |         |            |
|      |                                                                    |         |            |
|      | ~~Frame4\~7 for 4K mode,~~                                         |         |            |
|      |                                                                    |         |            |
|      | ~~Frame8\~15 for 8K mode,~~                                        |         |            |
+------+--------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_DMA_ADR_2 .anchor}~~FCTL_DMA_ADR_2/ DMA Address 2 Register (Offset 14h~~)

Register Index: 14h \~ 17h

Default Value: 0000_0000h

+------+--------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                       | Default | Attributes |
+:====:+====================================================================+:=======:+:==========:+
| 31:0 | ~~DMA start buffer address 2~~                                     | 00h     | RW         |
|      |                                                                    |         |            |
|      | ~~indicate SRAM location, unit is Bytes, but need 512Bytes align~~ |         |            |
|      |                                                                    |         |            |
|      | ~~Multi buffer pointer mode enable,~~                              |         |            |
|      |                                                                    |         |            |
|      | ~~and it will indicate SRAM location~~                             |         |            |
|      |                                                                    |         |            |
|      | ~~Frame2 for 1K mode,~~                                            |         |            |
|      |                                                                    |         |            |
|      | ~~Frame4\~5 for 2K mode,~~                                         |         |            |
|      |                                                                    |         |            |
|      | ~~Frame8\~11 for 4K mode,~~                                        |         |            |
|      |                                                                    |         |            |
|      | ~~Frame16\~23 for 8K mode,~~                                       |         |            |
+------+--------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_DMA_ADR_3 .anchor}~~FCTL_DMA_ADR_3/ DMA Address 3 Register (Offset 18h)~~

Register Index: 18h \~ 1Bh

Default Value: 0000_0000h

+------+--------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                       | Default | Attributes |
+:====:+====================================================================+:=======:+:==========:+
| 31:0 | ~~DMA start buffer address 3~~                                     | 00h     | RW         |
|      |                                                                    |         |            |
|      | ~~indicate SRAM location, unit is Bytes, but need 512Bytes align~~ |         |            |
|      |                                                                    |         |            |
|      | ~~Multi buffer pointer mode enable,~~                              |         |            |
|      |                                                                    |         |            |
|      | ~~and it will indicate SRAM location~~                             |         |            |
|      |                                                                    |         |            |
|      | ~~Frame3 for 1K mode,~~                                            |         |            |
|      |                                                                    |         |            |
|      | ~~Frame6\~7 for 2K mode,~~                                         |         |            |
|      |                                                                    |         |            |
|      | ~~Frame12\~15 for 4K mode,~~                                       |         |            |
|      |                                                                    |         |            |
|      | ~~Frame24\~31 for 8K mode,~~                                       |         |            |
+------+--------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_IO_SET .anchor}FCTL_IO_SET /Flash IO Setting Register (Offset 1Ch)

Register Index: 1Ch \~ 1Fh

Default Value: 0260_0000h

+-------+-----------------------------------------------------+---------+------------+
| Bit   | Descriptions                                        | Default | Attributes |
+:=====:+=====================================================+:=======:+:==========:+
| 31    | FLH_WP: flash interface write protect               | 0b      | RW         |
|       |                                                     |         |            |
|       | 0b: FLH_WP tie to 1'b0                              |         |            |
|       |                                                     |         |            |
|       | 1b: FLH_WP tie to 1'b1                              |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 30    | Flash Phy interface reset                           | 0h      | RsvdP      |
+-------+-----------------------------------------------------+---------+------------+
| 29:27 | Reserved                                            | 0h      | RsvdP      |
+-------+-----------------------------------------------------+---------+------------+
| 26    | Ready/busy negative edge monitor                    | 0b      | RW         |
|       |                                                     |         |            |
|       | 0b : clear by ready/busy negative edge event        |         |            |
|       |                                                     |         |            |
|       | 1b : set to monitor ready/busy negative edge event  |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 25    | I/O pad type select                                 | 1b      | RW         |
|       |                                                     |         |            |
|       | 0b: indicate single-end I/O type.                   |         |            |
|       |                                                     |         |            |
|       | 1b: indicate differential I/O type                  |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 24    | Force flash ALE signal to high                      | 0b      | RW         |
|       |                                                     |         |            |
|       | 0b: normal operation,                               |         |            |
|       |                                                     |         |            |
|       | 1b: force F_ALE output 1                            |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 23    | Force flash CLE signal to high                      | 0b      | RW         |
|       |                                                     |         |            |
|       | 0b: normal operation,                               |         |            |
|       |                                                     |         |            |
|       | 1b: force F_CLE output 1                            |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 22    | Force flash WEN signal to low                       | 1b      | RW         |
|       |                                                     |         |            |
|       | 0b: force F_WEN output 0,                           |         |            |
|       |                                                     |         |            |
|       | 1b: normal operation                                |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 21    | Force flash REN signal to low                       | 1b      | RW         |
|       |                                                     |         |            |
|       | 0b: force F_REN output 0,                           |         |            |
|       |                                                     |         |            |
|       | 1b: normal operation                                |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 20    |                                                     | 0b      | RW         |
+-------+-----------------------------------------------------+---------+------------+
| 19    | Flash low bus data signal direct output mode enable | 0b      | RW         |
|       |                                                     |         |            |
|       | 0b : disable                                        |         |            |
|       |                                                     |         |            |
|       | 1b : F_IO\[7 :0\] GPO mode enable                   |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 18:16 | Reserved                                            | 0h      | RsvdP      |
+-------+-----------------------------------------------------+---------+------------+
|       |                                                     |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 7:0   | F_IO\[7:0\] GPO value                               | 00h     | RW         |
|       |                                                     |         |            |
|       | low bus data value for direct output mode           |         |            |
+-------+-----------------------------------------------------+---------+------------+

Register: []{#FCTL_ZIP_MAP .anchor}FCTL_ZIP_MAP / ZIP MAP Register (Offset 20h)

Register Index: 20h \~ 23h

Default Value: FFFF_FFFFh

+-------+--------------------------------------------+------------+------------+
| Bit   | Descriptions                               | Default    | Attributes |
+:=====:+============================================+:==========:+:==========:+
| 31:0  | Compression valid data flag                | FFFF_FFFFh | RW         |
|       |                                            |            |            |
|       | Bit0: 1st 512bytes                         |            |            |
|       |                                            |            |            |
|       | ....                                       |            |            |
|       |                                            |            |            |
|       | Bit31: 31st 512bytes                       |            |            |
|       |                                            |            |            |
|       | 1: this 512 bytes should be un-compress    |            |            |
+-------+--------------------------------------------+------------+------------+

Register: []{#FCTL_MT_TRIG .anchor}FCTL_MT_TRIG / Multiple Trigger Bit (Offset 24h)

Register Index: 24h \~ 27h

Default Value: 0000_0000h

+-------+--------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                         | Default | Attributes |
+:=====:+======================================================================================+:=======:+:==========:+
| 31    | MTP reload signal                                                                    | 0b      | RW         |
|       |                                                                                      |         |            |
|       | When abort manual then trigger FPU, need set this bit to reload MTP                  |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 30:24 | Reserved                                                                             | 00h     | RsvdP      |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 23    | 0b : queue 7 idle                                                                    | 0b      | RO         |
|       |                                                                                      |         |            |
|       | 1b: queue 7 busy                                                                     |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 22    | 0b : queue 6 idle                                                                    | 0b      | RO         |
|       |                                                                                      |         |            |
|       | 1b: queue 6 busy                                                                     |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 21    | 0b : queue 5 idle                                                                    | 0b      | RO         |
|       |                                                                                      |         |            |
|       | 1b: queue 5 busy                                                                     |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 20    | 0b : queue 4 idle                                                                    | 0b      | RO         |
|       |                                                                                      |         |            |
|       | 1b: queue 4 busy                                                                     |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 19    | 0b : queue 3 idle                                                                    | 0b      | RO         |
|       |                                                                                      |         |            |
|       | 1b: queue 3 busy                                                                     |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 18    | 0b : queue 2 idle                                                                    | 0b      | RO         |
|       |                                                                                      |         |            |
|       | 1b: queue 2 busy                                                                     |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 17    | 0b : queue 1 idle                                                                    | 0b      | RO         |
|       |                                                                                      |         |            |
|       | 1b: queue 1 busy                                                                     |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 16    | 0b : queue 0 idle                                                                    | 0b      | RO         |
|       |                                                                                      |         |            |
|       | 1b : queue 0 busy                                                                    |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 15    | Clear queue 7, need write 1 then write 0 manual                                      | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 14    | Clear queue 6, need write 1 then write 0 manual                                      | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 13    | Clear queue 5, need write 1 then write 0 manual                                      | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 12    | Clear queue 4, need write 1 then write 0 manual                                      | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 11    | Clear queue 3, need write 1 then write 0 manual                                      | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 10    | Clear queue 2, need write 1 then write 0 manual                                      | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 9     | Clear queue 1, need write 1 then write 0 manual                                      | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 8     | Clear queue 0, need write 1 then write 0 manual                                      | 0b      | RW         |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 7     | Multiple trigger queue7 push bit                                                     | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Write 1 to push FCTL_MT_ADR_7 to MT queue 7                                          |         |            |
|       |                                                                                      |         |            |
|       | Read value                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 0b: queue 7 can push task event                                                      |         |            |
|       |                                                                                      |         |            |
|       | 1b: queue 7 full                                                                     |         |            |
|       |                                                                                      |         |            |
|       | MTP cannot push directly and FCTL_MT_PTR_DIF should be checked how much MTP in queue |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 6     | Multiple trigger queue 6 push bit                                                    | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Write 1 to push FCTL_MT_ADR_6 to MT queue 6                                          |         |            |
|       |                                                                                      |         |            |
|       | Read value                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 0b: queue 6 can push task event                                                      |         |            |
|       |                                                                                      |         |            |
|       | 1b: queue 6 full                                                                     |         |            |
|       |                                                                                      |         |            |
|       | MTP cannot push directly and FCTL_MT_PTR_DIF should be checked how much MTP in queue |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 5     | Multiple trigger queue 5 push bit                                                    | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Write 1 to push FCTL_MT_ADR_5 to MT queue 5                                          |         |            |
|       |                                                                                      |         |            |
|       | Read value                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 0b: queue 5 can push task event                                                      |         |            |
|       |                                                                                      |         |            |
|       | 1b: queue 5 full                                                                     |         |            |
|       |                                                                                      |         |            |
|       | MTP cannot push directly and FCTL_MT_PTR_DIF should be checked how much MTP in queue |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 4     | Multiple trigger queue 4 push bit                                                    | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Write 1 to push FCTL_MT_ADR_4 to MT queue 4                                          |         |            |
|       |                                                                                      |         |            |
|       | Read value                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 0b: queue 4 can push task event                                                      |         |            |
|       |                                                                                      |         |            |
|       | 1b: queue 4 full                                                                     |         |            |
|       |                                                                                      |         |            |
|       | MTP cannot push directly and FCTL_MT_PTR_DIF should be checked how much MTP in queue |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 3     | Multiple trigger queue 3 push bit                                                    | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Write 1 to push FCTL_MT_ADR_3 to MT queue 3                                          |         |            |
|       |                                                                                      |         |            |
|       | Read value                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 0b: queue 3 can push task event                                                      |         |            |
|       |                                                                                      |         |            |
|       | 1b: queue 3 full                                                                     |         |            |
|       |                                                                                      |         |            |
|       | MTP cannot push directly and FCTL_MT_PTR_DIF should be checked how much MTP in queue |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 2     | Multiple trigger queue 2 push bit                                                    | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Write 1 to push FCTL_MT_ADR_2 to MT queue 2                                          |         |            |
|       |                                                                                      |         |            |
|       | Read value                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 0b: queue 2 can push task event                                                      |         |            |
|       |                                                                                      |         |            |
|       | 1b: queue 2 full                                                                     |         |            |
|       |                                                                                      |         |            |
|       | MTP cannot push directly and FCTL_MT_PTR_DIF should be checked how much MTP in queue |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 1     | Multiple trigger queue 1 push bit                                                    | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Write 1 to push FCTL_MT_ADR_1 to MT queue 1                                          |         |            |
|       |                                                                                      |         |            |
|       | Read value                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 0b: queue 1 can push task event                                                      |         |            |
|       |                                                                                      |         |            |
|       | 1b: queue 1 full                                                                     |         |            |
|       |                                                                                      |         |            |
|       | MTP cannot push directly and FCTL_MT_PTR_DIF should be checked how much MTP in queue |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 0     | Multiple trigger queue 0 push bit                                                    | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Write 1 to push FCTL_MT_ADR_0 to MT queue 0                                          |         |            |
|       |                                                                                      |         |            |
|       | Read value                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 0b: queue 0 can push task event                                                      |         |            |
|       |                                                                                      |         |            |
|       | 1b: queue 0 full                                                                     |         |            |
|       |                                                                                      |         |            |
|       | MTP cannot push directly and FCTL_MT_PTR_DIF should be checked how much MTP in queue |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MT_ADR_0 .anchor}FCTL_MT_ADR_0/ MT Address 0 Register (Offset 28h)

Register Index: 28h \~ 2Bh

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                 | Default | Attributes |
+:=====:+==============================================================================+:=======:+:==========:+
| 31:20 | Reserved                                                                     | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 19    | MTP Force empty request enable                                               | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 18    | MTP delay function enable                                                    | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 17    | MTP group priority function enable                                           | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable, unlock status                                                   |         |            |
|       |                                                                              |         |            |
|       | 1b: enable, lock status                                                      |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 16    | MTP group priority definition                                                | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: group 2, low priority                                                    |         |            |
|       |                                                                              |         |            |
|       | 1b: group 1, high priority                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 15:0  | If [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[17\] = 0                           | 00h     | RW         |
|       |                                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align                 |         |            |
|       |                                                                              |         |            |
|       | If FCON_MT_ADR_BASE\[17\] = 1                                                |         |            |
|       |                                                                              |         |            |
|       | the iram address is value \* 72 + FCON_MT_ADR_BASE\[15:0\]                   |         |            |
|       |                                                                              |         |            |
|       | Ex. If the target MT table is at 0x1120 and FCON_MT_ADR_BASE\[15:0\]=0x1000, |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 0 : FW sets 0x1120                                  |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 1 : FW sets 0x4                                     |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MT_ADR_1 .anchor}FCTL_MT_ADR_1/ MT Address 1 Register (Offset 2Ch)

Register Index: 2Ch \~ 2Fh

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                 | Default | Attributes |
+:=====:+==============================================================================+:=======:+:==========:+
| 31:20 | Reserved                                                                     | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 19    | Force empty request enable                                                   | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 18    | MTQ delay function enable                                                    | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 17    | MTQ group priority function enable                                           | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable,un lock status                                                   |         |            |
|       |                                                                              |         |            |
|       | 1b: enable, lock status                                                      |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 16    | MTQ group priority definition                                                | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: group 2, low priority                                                    |         |            |
|       |                                                                              |         |            |
|       | 1b: group 1, high priority                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 15:0  | If [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[17\] = 0                           | 00h     | RW         |
|       |                                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align                 |         |            |
|       |                                                                              |         |            |
|       | If FCON_MT_ADR_BASE\[17\] = 1                                                |         |            |
|       |                                                                              |         |            |
|       | the iram address is value \* 72 + FCON_MT_ADR_BASE\[15:0\]                   |         |            |
|       |                                                                              |         |            |
|       | Ex. If the target MT table is at 0x1120 and FCON_MT_ADR_BASE\[15:0\]=0x1000, |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 0 : FW sets 0x1120                                  |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 1 : FW sets 0x4                                     |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MT_ADR_2 .anchor}FCTL_MT_ADR_2/ MT Address 2 Register (Offset 30h)

Register Index: 30h \~ 33h

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                 | Default | Attributes |
+:=====:+==============================================================================+:=======:+:==========:+
| 31:20 | Reserved                                                                     | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 19    | Force empty request enable                                                   | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 18    | MTQ delay function enable                                                    | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 17    | MTQ group priority function enable                                           | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable, unlock status                                                   |         |            |
|       |                                                                              |         |            |
|       | 1b: enable, lock status                                                      |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 16    | MTQ group priority definition                                                | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: group 2, low priority                                                    |         |            |
|       |                                                                              |         |            |
|       | 1b: group 1, high priority                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 15:0  | If [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[17\] = 0                           | 00h     | RW         |
|       |                                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align                 |         |            |
|       |                                                                              |         |            |
|       | If FCON_MT_ADR_BASE\[17\] = 1                                                |         |            |
|       |                                                                              |         |            |
|       | the iram address is value \* 72 + FCON_MT_ADR_BASE\[15:0\]                   |         |            |
|       |                                                                              |         |            |
|       | Ex. If the target MT table is at 0x1120 and FCON_MT_ADR_BASE\[15:0\]=0x1000, |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 0 : FW sets 0x1120                                  |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 1 : FW sets 0x4                                     |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MT_ADR_3 .anchor}FCTL_MT_ADR_3/ MT Address 3 Register (Offset 34h)

Register Index: 34h \~ 37h

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                 | Default | Attributes |
+:=====:+==============================================================================+:=======:+:==========:+
| 31:20 | Reserved                                                                     | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 19    | Force empty request enable                                                   | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 18    | MTQ delay function enable                                                    | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 17    | MTQ group priority function enable                                           | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable, unlock status                                                   |         |            |
|       |                                                                              |         |            |
|       | 1b: enable, lock status                                                      |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 16    | MTQ group priority definition                                                | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: group 2, low priority                                                    |         |            |
|       |                                                                              |         |            |
|       | 1b: group 1, high priority                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 15:0  | If [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[17\] = 0                           | 00h     | RW         |
|       |                                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align                 |         |            |
|       |                                                                              |         |            |
|       | If FCON_MT_ADR_BASE\[17\] = 1                                                |         |            |
|       |                                                                              |         |            |
|       | the iram address is value \* 72 + FCON_MT_ADR_BASE\[15:0\]                   |         |            |
|       |                                                                              |         |            |
|       | Ex. If the target MT table is at 0x1120 and FCON_MT_ADR_BASE\[15:0\]=0x1000, |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 0 : FW sets 0x1120                                  |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 1 : FW sets 0x4                                     |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_ROW_ADR .anchor}FCTL_FTA_ROW /Flash Target Row Address Register (Offset 38h)

Register Index: 38h \~ 3Bh

Default Value: 0000_0000h

  --------------------------------------------------------------------------------------------------------------------------------
    Bit    Descriptions                                                                                     Default    Attributes
  -------- ----------------------------------------------------------------------------------------------- ---------- ------------
   31:24   Reserved                                                                                           00h        RsvdP

   23:16   Address phase 5^rd^ address byte value and randomize rule will reference this register value       00h          RW

    15:8   Address phase 4^rd^ address byte value and randomize rule will reference this register value       00h          RW

    7:0    Address phase 3^rd^ address byte value, and randomize rule will reference this register value      00h          RW
  --------------------------------------------------------------------------------------------------------------------------------

Register: []{#FCTL_COL_ADR .anchor}FCTL_FTA_COL /Flash Target Column Address Register (Offset 3Ch)

Register Index: 3Ch \~ 3Fh

Default Value: 0000_0000h

  -------------------------------------------------------------------------------------------------------------------------------
    Bit   Descriptions                                                                                     Default    Attributes
  ------- ----------------------------------------------------------------------------------------------- ---------- ------------
   31:16  Reserved                                                                                           00h        RsvdP

   15:8   Address phase 2^nd^ address byte value, and randomize rule will reference this register value      00h          RW

    7:0   Address phase 1^st^ address byte value, and randomize rule will reference this register value      00h          RW
  -------------------------------------------------------------------------------------------------------------------------------

Register: []{#FCTL_DMA_CFG .anchor}FCTL_DMA_CFG /Flash DMA Configure Register (Offset 40h)

Register Index: 40h \~ 43h

Default Value: 0000_0002h

+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                  | Default | Attributes |
+:=====:+===============================================================================================+:=======:+:==========:+
| 31    | Buffer mode mean DMA will enable buffer rotation, check buffer valid, and signoff buffer flag | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 0b: copy mode                                                                                 |         |            |
|       |                                                                                               |         |            |
|       | 1b: buf mode                                                                                  |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 30    | Reserved                                                                                      | 0b      | RsvdP      |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 29:24 | Reserved                                                                                      | 00h     | RsvdP      |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 23    | Rserved                                                                                       | 0b      | Rsvd       |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 22    | DQS GPO MODE SWITCH                                                                           | 0b      | RW         |
|       |                                                                                               |         |            |
|       | direct input mode value for DQS pin                                                           |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 21:19 | Reserved                                                                                      | 00h     | RsvdP      |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 18    | DQS single dummy trigger to send single DQS data                                              |         |            |
|       |                                                                                               |         |            |
|       | 1b: trigger for one dqs                                                                       |         |            |
|       |                                                                                               |         |            |
|       | No need to wait finish                                                                        |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 17    | DQS dummy busy status                                                                         | 0b      | RO         |
|       |                                                                                               |         |            |
|       | 0b: idle                                                                                      |         |            |
|       |                                                                                               |         |            |
|       | 1b: start to send DQS dummy data                                                              |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 16    | DQS dummy trigger to send DQS data                                                            | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 0b: disable                                                                                   |         |            |
|       |                                                                                               |         |            |
|       | 1b: enable                                                                                    |         |            |
|       |                                                                                               |         |            |
|       | Need to wait DQS dummy busy status                                                            |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 15    | Multi buffer pointer mode enable bit                                                          | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 0b: use FCTL_DMA_ADR_0                                                                        |         |            |
|       |                                                                                               |         |            |
|       | 1b: use FCTL_DMA_ADR_0\~7                                                                     |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 14    | DQS check status whether it is triggered or not when reading flash.                           | 0b      | RO         |
|       |                                                                                               |         |            |
|       | 0b: non-triggered                                                                             |         |            |
|       |                                                                                               |         |            |
|       | 1b: triggered                                                                                 |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 13    | DQS check status clear                                                                        | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 0b: disable                                                                                   |         |            |
|       |                                                                                               |         |            |
|       | 1b: enable                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 12:11 | Reserved                                                                                      |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 10:8  | BCH frame number transfer set.                                                                | 00h     | RW         |
|       |                                                                                               |         |            |
|       | 1h: transfer 1 BCH frame                                                                      |         |            |
|       |                                                                                               |         |            |
|       | 4h: transfer 4 BCH frame                                                                      |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 7:6   | Multi buffer pointer mode select                                                              | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 00b: 8 ECC frame                                                                              |         |            |
|       |                                                                                               |         |            |
|       | 01b: 4 ECC frame                                                                              |         |            |
|       |                                                                                               |         |            |
|       | 10b: 2 ECC frame                                                                              |         |            |
|       |                                                                                               |         |            |
|       | 11b: 1 ECC frame                                                                              |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 5:4   | ALU group select                                                                              | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 00b: ALU0 group select for address generate                                                   |         |            |
|       |                                                                                               |         |            |
|       | 01b: ALU1 group select for address generate                                                   |         |            |
|       |                                                                                               |         |            |
|       | 10b: ALU2 group select for address generate                                                   |         |            |
|       |                                                                                               |         |            |
|       | 11b: ALU3 group select for address generate                                                   |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 3     | FSA selection                                                                                 | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 0b: select other FSA, define at FCTL_FSA_SEL                                                  |         |            |
|       |                                                                                               |         |            |
|       | 1b: always select FSA 0                                                                       |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 2     | Spare region selection                                                                        | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 0b: spare from each spare region.                                                             |         |            |
|       |                                                                                               |         |            |
|       | 1b: all spare byte same as sector 0                                                           |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 1     | Ultra DMA                                                                                     | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 0b: disable                                                                                   |         |            |
|       |                                                                                               |         |            |
|       | 1b: enable                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 0     | Timing configure select                                                                       | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 0b: select FCON\_\*\_TIME_CFG                                                                 |         |            |
|       |                                                                                               |         |            |
|       | 1b: select FCON\_\*\_TIME_CFG_1                                                               |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_OTHER_SET .anchor}FCTL_OTHER \_SET /Others Setting Register (Offset 44h)

Register Index: 44h \~ 47h

Default Value: 0000_0478h

+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                                                                             | Default | Attributes |
+:=====:+==========================================================================================================================================================================================================+:=======:+:==========:+
| 31:28 | Reserved                                                                                                                                                                                                 | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27:26 | Frame start pointer                                                                                                                                                                                      | 0h      | RW         |
|       |                                                                                                                                                                                                          |         |            |
|       | This pointer is define which ECC frame number will be transferred, and it will effect randomize rule.                                                                                                    |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 25    | Choose the source for one of the reference seed used in generating the pseudo                                                                                                                            | 0b      | RW         |
|       |                                                                                                                                                                                                          |         |            |
|       | 0b: Reference seed from FCTL\_ ROW_ADR\[8:0\]                                                                                                                                                            |         |            |
|       |                                                                                                                                                                                                          |         |            |
|       | 1b: Reference seed from FCTL_OTHER_SET\[24:16\]                                                                                                                                                          |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 24:16 | Reference seed value used for generating the pseudo random value used in data randomization                                                                                                              | 00h     | RW         |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15    | Reserved                                                                                                                                                                                                 | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 14    | Conversion poly rotate                                                                                                                                                                                   | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 13    | Error configuration of parity checks ENCSRAM.                                                                                                                                                            | 0b      | RW         |
|       |                                                                                                                                                                                                          |         |            |
|       | 0b: disable error insert                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                          |         |            |
|       | 1b: enable error insert                                                                                                                                                                                  |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 12    | Error configuration of parity checks BRAM.                                                                                                                                                               | 0b      | RW         |
|       |                                                                                                                                                                                                          |         |            |
|       | 0b: disable error insert                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                          |         |            |
|       | 1b: enable error insert                                                                                                                                                                                  |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 11    | Error configuration of parity checks Internal Buffer RAM.                                                                                                                                                | 0b      | RW         |
|       |                                                                                                                                                                                                          |         |            |
|       | 0b: disable error insert                                                                                                                                                                                 |         |            |
|       |                                                                                                                                                                                                          |         |            |
|       | 1b: enable error insert                                                                                                                                                                                  |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 10    | BCH correct function enable                                                                                                                                                                              | 1b      | RW         |
|       |                                                                                                                                                                                                          |         |            |
|       | 0b: disable correct function.                                                                                                                                                                            |         |            |
|       |                                                                                                                                                                                                          |         |            |
|       | 1b: Auto correct enable                                                                                                                                                                                  |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 9     | MTQ group priority function enable                                                                                                                                                                       | 0b      | RW         |
|       |                                                                                                                                                                                                          |         |            |
|       | 0b: disable                                                                                                                                                                                              |         |            |
|       |                                                                                                                                                                                                          |         |            |
|       | 1b: enable                                                                                                                                                                                               |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 8     | Data transfer path force fix data                                                                                                                                                                        | 0b      | RW         |
|       |                                                                                                                                                                                                          |         |            |
|       | 0b: normal data path transfer                                                                                                                                                                            |         |            |
|       |                                                                                                                                                                                                          |         |            |
|       | 1b: Force fix data to DRAM. Spare is all 'hff. If this function is enabled, conversion should be disabled and zip bypass enable and compare LCA disable. HW automatically disable CRC/BCH/LDPC function. |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7:0   | Fix data value for FCTL_OTHER_SET\[8\] and L4K BUF_MASK usage                                                                                                                                            | 78h     | RW         |
|       |                                                                                                                                                                                                          |         |            |
|       | When flash read, it need to enable FCTL_OTHER_SET\[8\] and set FCTL_BUF_VALID high to force fix data read to SRAM                                                                                        |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_FSA_SEL .anchor}FCTL_FSA_SEL /Source Address Select Register (Offset 48h)

Register Index: 48h \~ 4Bh

Default Value: 0000_0000h

+-------+---------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                              | Default | Attributes |
+:=====:+===========================================================================+:=======:+:==========:+
| 31:19 | Reserved                                                                  | 00h     | RsvdP      |
+-------+---------------------------------------------------------------------------+---------+------------+
| 18    | Internal FSA enable                                                       | 0       | RW         |
+-------+---------------------------------------------------------------------------+---------+------------+
| 17:16 | 2^nd^ FSA counter for multi address generate                              | 00b     | RW         |
|       |                                                                           |         |            |
|       | 2^nd^ FSA selection will be referred when counter value as 2'h3           |         |            |
+-------+---------------------------------------------------------------------------+---------+------------+
| 15    | Reserved                                                                  | 0b      | RsvdP      |
+-------+---------------------------------------------------------------------------+---------+------------+
| 14:8  | 2^nd^ FSA selection in FPU                                                | 00h     | RW         |
|       |                                                                           |         |            |
|       | 0h: FSA0 select,                                                          |         |            |
|       |                                                                           |         |            |
|       | \...                                                                      |         |            |
|       |                                                                           |         |            |
|       | 1Fh: FSA31 select                                                         |         |            |
|       |                                                                           |         |            |
|       | 3Fh: FSA63                                                                |         |            |
|       |                                                                           |         |            |
|       | When multi address generate, 2^nd^ FSA selection will be referred.        |         |            |
+-------+---------------------------------------------------------------------------+---------+------------+
| 7     | Reserved                                                                  | 0b      | RsvdP      |
+-------+---------------------------------------------------------------------------+---------+------------+
| 6:0   | 1^st^ FSA selection in FPU                                                | 0h      | RW         |
|       |                                                                           |         |            |
|       | 0h: FSA0 select,                                                          |         |            |
|       |                                                                           |         |            |
|       | \...                                                                      |         |            |
|       |                                                                           |         |            |
|       | 1Fh: FSA31 select                                                         |         |            |
|       |                                                                           |         |            |
|       | 3Fh: FSA63                                                                |         |            |
|       |                                                                           |         |            |
|       | When FCTL_DMA_CFG\[3\] set 0, FSA will reference this register to select. |         |            |
+-------+---------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_UNC_CFG .anchor}FCTL_UNC_CFG /Uncorrectable Configure Register (Offset 4Ch)

Register Index: 4Ch \~ 4Fh

Default Value: 0000_0000h

+-------+-----------------------------------------------------+---------+------------+
| Bit   | Descriptions                                        | Default | Attributes |
+:=====:+=====================================================+:=======:+:==========:+
| 31:0  | reserved                                            | 00h     | RW         |
+-------+-----------------------------------------------------+---------+------------+
| 17    | Auto generate program fail when DMA write           | 0h      | RW         |
+-------+-----------------------------------------------------+---------+------------+
| 16    | Auto generate UNC when DMA read                     | 0h      | RW         |
+-------+-----------------------------------------------------+---------+------------+
|       |                                                     |         |            |
+-------+-----------------------------------------------------+---------+------------+
|       |                                                     |         |            |
+-------+-----------------------------------------------------+---------+------------+
|       |                                                     |         |            |
+-------+-----------------------------------------------------+---------+------------+
| 3:0   | Force BCH uncorrectable frame                       | 00h     | RW         |
|       |                                                     |         |            |
|       | Ex \[3\] = 1, the frame 3 will report uncorrectable |         |            |
+-------+-----------------------------------------------------+---------+------------+

Register: []{#FCTL_CNT_ONE .anchor}FCTL_CNT_ONE / Count One Register (Offset 50h)

Register Index: 50h \~ 53h

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31    | Counter value                              | 0b      | RW         |
|       |                                            |         |            |
|       | 0b: count zero                             |         |            |
|       |                                            |         |            |
|       | 1b: count one                              |         |            |
+-------+--------------------------------------------+---------+------------+
| 30    | Counter mode enable                        | 0b      | RW         |
|       |                                            |         |            |
|       | 0b: disable                                |         |            |
|       |                                            |         |            |
|       | 1b: count one enable                       |         |            |
+-------+--------------------------------------------+---------+------------+
| 31:16 | Reserved                                   | 00h     | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 17:0  | Count one function result                  | 0h      | RO         |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_ECC_INF .anchor}FCTL_ECC_INF /ECC Information Register (Offset 54h)

Register Index: 54h \~ 57h

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:10 | Reserved                                   | 0h      | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 9:0   | LDPC_MAX_BFC,                              | 0000h   | RO         |
|       |                                            |         |            |
|       | maximum LDPC bitflip number,               |         |            |
|       |                                            |         |            |
|       | '3FFh' uncorrectable occur                 |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_INT_VCT .anchor}FCTL_INT_VCT /Interrupt Vector Configure Register (Offset 58h)

Register Index: 58h \~ 5Bh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:8  | Interrupt vector information               | 00h     | RW         |
|       |                                            |         |            |
|       | INT_INDEX_MODE = 1, index = \[16:8\]       |         |            |
|       |                                            |         |            |
|       | INT_INDEX_MODE = 0, int_vct = \[31:8\]     |         |            |
+-------+--------------------------------------------+---------+------------+
| 7     | Reserved                                   | 0b      | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 5:3   | Reserved                                   | 0h      | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 2:0   | Reserved                                   | 0h      | RsvdP      |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_SPR_ADR .anchor}FCTL_SPR_ADR /Spare Address Register (Offset 5Ch)

Register Index: 5Ch \~ 5Fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   Reserved                                         00h        RsvdP

  -----------------------------------------------------------------------------

Register: []{#FCTL_CMP_CFG .anchor}FCTL_CMP_CFG /Compare Configure Register (Offset 60h)

Register Index: 60h \~ 63h

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                          | Default | Attributes |
+:=====:+=======================================================================================================+:=======:+:==========:+
| 31:2  | Reserved                                                                                              | 00h     | RsvdP      |
+-------+-------------------------------------------------------------------------------------------------------+---------+------------+
| 1     | Reserved                                                                                              | 0b      | RsvdP      |
+-------+-------------------------------------------------------------------------------------------------------+---------+------------+
| 0     | LCA_FW compare function check LCA and FW of spare data whether it is correct with expect data or not. | 0b      | RW         |
|       |                                                                                                       |         |            |
|       | 0b: disable                                                                                           |         |            |
|       |                                                                                                       |         |            |
|       | 1b: enable                                                                                            |         |            |
+-------+-------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_INT_INF .anchor}FCTL_INT_INF / Interrupt Information Register (Offset 64h)

Register Index: 64h \~ 67h

Default Value: 0000_0000h

+-------+------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                               | Default | Attributes |
+:=====:+============================================================+:=======:+:==========:+
| 31:18 | Frame map update status of Ping-Pong FIFO                  | 00h     | RO         |
|       |                                                            |         |            |
|       | Read:                                                      |         |            |
|       |                                                            |         |            |
|       | 0b: no-update                                              |         |            |
|       |                                                            |         |            |
|       | 1b: update                                                 |         |            |
|       |                                                            |         |            |
|       | Bit18:E3D error map                                        |         |            |
|       |                                                            |         |            |
|       | Bit19:fw setting error                                     |         |            |
|       |                                                            |         |            |
|       | Bit20:erase page                                           |         |            |
|       |                                                            |         |            |
|       | Bit21: interrupt vector map                                |         |            |
|       |                                                            |         |            |
|       | Bit22: uncorrectable map                                   |         |            |
|       |                                                            |         |            |
|       | Bit23: signoff mask map                                    |         |            |
|       |                                                            |         |            |
|       | Bit24: over ECC error threshold map                        |         |            |
|       |                                                            |         |            |
|       | Bit25: CRC error map                                       |         |            |
|       |                                                            |         |            |
|       | Bit26: compare LCA FW error map                            |         |            |
|       |                                                            |         |            |
|       | Bit27: PFA error map                                       |         |            |
|       |                                                            |         |            |
|       | Bit28: STA error map                                       |         |            |
|       |                                                            |         |            |
|       | Bit29: Compression fail error map                          |         |            |
|       |                                                            |         |            |
|       | Bit30: Decompression fail error map                        |         |            |
|       |                                                            |         |            |
|       | Bit31: CRC16 fail error map                                |         |            |
+-------+------------------------------------------------------------+---------+------------+
| 17:9  | Reserved                                                   | 0b      | RsvdP      |
+-------+------------------------------------------------------------+---------+------------+
| 8     | MAP FIFO Pointer Reset                                     | 1b      | RW         |
|       |                                                            |         |            |
|       | Write 0 to reset MAP FIFO pointer                          |         |            |
+-------+------------------------------------------------------------+---------+------------+
| 7:1   | Reserved                                                   | 00h     | RsvdP      |
+-------+------------------------------------------------------------+---------+------------+
| 0     | 1b: interrupt busy mean MT stop because fail case occur    | 0b      | RW         |
|       |                                                            |         |            |
|       | Write 0 to free interrupt and clear stop bit, let MTQ work |         |            |
+-------+------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MAP_CFG .anchor}FCTL_MAP_CFG / Frame Mapping Configure Register (Offset 68h)

Register Index: 68h \~ 6Bh

Default Value: 0000_0000h

+-------+----------------------------------------------------+---------+------------+
| Bit   | Descriptions                                       | Default | Attributes |
+:=====:+====================================================+:=======:+:==========:+
| 31:22 | Reserved                                           |         |            |
+-------+----------------------------------------------------+---------+------------+
| 21:8  | Ping-Pong FIFO write frame map when fail occur     | 00h     | RW         |
|       |                                                    |         |            |
|       | 0b: disable                                        |         |            |
|       |                                                    |         |            |
|       | 1b: enable                                         |         |            |
|       |                                                    |         |            |
|       | Bit8: interrupt vector map                         |         |            |
|       |                                                    |         |            |
|       | Bit9: uncorrectable map                            |         |            |
|       |                                                    |         |            |
|       | Bit10: signoff mask map                            |         |            |
|       |                                                    |         |            |
|       | Bit11: over ECC error threshold map                |         |            |
|       |                                                    |         |            |
|       | Bit12: CRC error map                               |         |            |
|       |                                                    |         |            |
|       | Bit13: compare LCA FW error map                    |         |            |
|       |                                                    |         |            |
|       | Bit14: PFA error map                               |         |            |
|       |                                                    |         |            |
|       | Bit15: STA error map                               |         |            |
|       |                                                    |         |            |
|       | Bit16: Compression fail                            |         |            |
|       |                                                    |         |            |
|       | Bit17: Decompression fail                          |         |            |
|       |                                                    |         |            |
|       | Bit18: CRC16 fail                                  |         |            |
|       |                                                    |         |            |
|       | Bit19:Erase page flag                              |         |            |
|       |                                                    |         |            |
|       | Bit20:fw setting error fail                        |         |            |
|       |                                                    |         |            |
|       | Bit21:E3D error map                                |         |            |
+-------+----------------------------------------------------+---------+------------+
| 7     | Current MAP FIFO pointer                           | 0h      | RO         |
+-------+----------------------------------------------------+---------+------------+
| 6:5   | Ping-Pong FIFO information                         | 0h      | RO         |
|       |                                                    |         |            |
|       | 01b: current FIFO has new frame map information    |         |            |
|       |                                                    |         |            |
|       | 10b: next FIFO has new frame map information       |         |            |
+-------+----------------------------------------------------+---------+------------+
| 4:1   | Ping-Pong FIFO is selected which frame map         | 0h      | RW         |
|       |                                                    |         |            |
|       | 000b: interrupt vector map                         |         |            |
|       |                                                    |         |            |
|       | 001b: uncorrectable map(physical frame)            |         |            |
|       |                                                    |         |            |
|       | 010b: signoff mask map(logical frame)              |         |            |
|       |                                                    |         |            |
|       | 011b: over ECC error threshold map(physical frame) |         |            |
|       |                                                    |         |            |
|       | 100b: CRC error map(physical frame)                |         |            |
|       |                                                    |         |            |
|       | 101b: compare LCA FW error map(logical frame)      |         |            |
|       |                                                    |         |            |
|       | 110b: PFA error map                                |         |            |
|       |                                                    |         |            |
|       | 111b: STA err_map                                  |         |            |
|       |                                                    |         |            |
|       | 1000b: Compression fail map(logical frame)         |         |            |
|       |                                                    |         |            |
|       | 1001b: Decompression fail mapzip_fifo              |         |            |
|       |                                                    |         |            |
|       | 1010b: CRC16 error map                             |         |            |
|       |                                                    |         |            |
|       | 1100b:FW setting error map(ZIP setting)            |         |            |
|       |                                                    |         |            |
|       | 1101b:E3D error map(logical frame)                 |         |            |
+-------+----------------------------------------------------+---------+------------+
| 0     | Switch Ping-Pong FIFO to next frame map            | 0b      | RW         |
+-------+----------------------------------------------------+---------+------------+

Register: []{#FCTL_MAP_INF .anchor}FCTL_MAP_INF / Frame Mapping Information Register (Offset 6Ch)

Register Index: 6Ch \~ 6Fh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:0  | Frame map information                      | 00h     | RO         |
|       |                                            |         |            |
|       | bit0: frame 0 status                       |         |            |
|       |                                            |         |            |
|       | bit1: frame 1 status                       |         |            |
|       |                                            |         |            |
|       | ...                                        |         |            |
|       |                                            |         |            |
|       | bit31: frame 31 status                     |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_CMP_ADR .anchor}FCTL_CMP_ADR / Compare Configure Register (Offset 70h)

Register Index: 70h \~ 73h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   Reserved                                         00h        RsvdP

  -----------------------------------------------------------------------------

Register: []{#FCTL_RBY_INF .anchor}FCTL_RBY_INF / RBY Information Register (Offset 74h)

Register Index: 74h \~ 77h

Default Value: 0000_01FFh

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:9   Reserved                                         00h        RsvdP

     8    FLH_CE_RBY AND signal                             1b          RO

    7:0   FLH_CE_RBY                                       FFh          RO
  -----------------------------------------------------------------------------

Register: []{#FCTL_CMP_FW .anchor}FCTL_CMP_FW /Compare FW Register (Offset 78h)

Register Index: 78h \~ 7Bh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:16  Reserved                                         00h        RsvdP

   15:0   Reserved                                         00h        RsvdP
  -----------------------------------------------------------------------------

Register: []{#FCTL_FLH_SET .anchor}FCTL_FLH_SET /Flash Interface Set Register (Offset 7Ch)

Register Index: 7Ch \~ 7Fh

Default Value: 0000_3000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:2  | Reserved                                   | 00h     | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 1:0   | Flash interface type                       | 00b     | RW         |
|       |                                            |         |            |
|       | 11b: onfi                                  |         |            |
|       |                                            |         |            |
|       | 10b: toggle                                |         |            |
|       |                                            |         |            |
|       | 00b: legacy                                |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_HS_MODE .anchor}FCTL_HS_MODE /High Speed Mode Register (Offset 80h)

Register Index: 80h \~ 83h

Default Value: 0000_3000h

+-----------+--------------------------------------------+---------+------------+
| Bit       | Descriptions                               | Default | Attributes |
+:=========:+============================================+:=======:+:==========:+
| 31:14     | Reserved                                   | 00h     | RsvdP      |
+-----------+--------------------------------------------+---------+------------+
| 13        | PIO DAT LAST                               | 1b      | RW         |
+-----------+--------------------------------------------+---------+------------+
| 12        | PIO DAT FIRST                              | 1b      | RW         |
+-----------+--------------------------------------------+---------+------------+
| ~~11:10~~ | ~~Flash interface type~~                   | ~~00b~~ | ~~RW~~     |
|           |                                            |         |            |
|           | ~~11b: onfi~~                              |         |            |
|           |                                            |         |            |
|           | ~~10b: toggle~~                            |         |            |
|           |                                            |         |            |
|           | ~~01b: 16bit mode~~                        |         |            |
|           |                                            |         |            |
|           | ~~00b: legacy~~                            |         |            |
+-----------+--------------------------------------------+---------+------------+
| 9         | Reserved                                   | 0b      | RsvdP      |
+-----------+--------------------------------------------+---------+------------+
| 8         | DQS GPO value                              | 0b      | RW         |
+-----------+--------------------------------------------+---------+------------+
| 7         | DQS GPO MODE                               | 0b      | RW         |
|           |                                            |         |            |
|           | Direct output mode value for DQS pin       |         |            |
+-----------+--------------------------------------------+---------+------------+
| 6:2       | Reserved                                   | 0h      | RsvdP      |
+-----------+--------------------------------------------+---------+------------+
| 1         | PIO DQS value                              | 0b      | RW         |
+-----------+--------------------------------------------+---------+------------+
| 0         | PIO DQS output enable                      | 0b      | RW         |
+-----------+--------------------------------------------+---------+------------+

Register: []{#FCTL_INT_CFG .anchor}FCTL_INT_CFG / Interrupt Configure Register (Offset 84h)

Register Index: 84h \~ 87h

Default Value: 0000_0000h

+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                  | Default | Attributes |
+:=====:+===============================================================================================================================================+:=======:+:==========:+
| 31:29 | Reserved                                                                                                                                      | 0b      | RsvdP      |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 28    | Reserved                                                                                                                                      | 0b      | RsvdP      |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27    | When auto polling check status interrupt occur,                                                                                               | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | MT process can choice whether it will stop or not.                                                                                            |         |            |
|       |                                                                                                                                               |         |            |
|       | 0b: disable, and stop MT process                                                                                                              |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable, and not stop MT process                                                                                                           |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 26    | When error status interrupt occur,                                                                                                            | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | MT process can choice whether it will stop or not.                                                                                            |         |            |
|       |                                                                                                                                               |         |            |
|       | 0b: disable, and stop MT process                                                                                                              |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable, and not stop MT process                                                                                                           |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 25    | When over error interrupt occur,                                                                                                              | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | MT process can choice whether it will stop or not                                                                                             |         |            |
|       |                                                                                                                                               |         |            |
|       | 0b: disable, and stop MT process                                                                                                              |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable, and not stop MT process                                                                                                           |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 24    | Force DMA abort                                                                                                                               | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: idle                                                                                                                                      |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: abort MTQ, write 1 then wait FPU done that should be checked FCTL_FPU_TRIG\[0\] and FCTL_FPU_TRIG\[1\] status whether it is ready or not. |         |            |
|       |                                                                                                                                               |         |            |
|       | Clear this bit and FCTL_INT_INF bit0 to let MTQ on going                                                                                      |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:18 | Reserved                                                                                                                                      | 0b      | RsvdP      |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 17    | E3D error interrupt enable(forbidden to enable)                                                                                               | 0b      | RW         |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 16    | FW setting error interrupt enable                                                                                                             | 0b      | RW         |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15    | ALL_DONE interrupt enable                                                                                                                     | 0b      | RW         |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 14    | PARTIAL_DONE interrupt enable                                                                                                                 | 0b      | RW         |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 13    | DMA_DONE interrupt enable                                                                                                                     | 0b      | RW         |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 12    | ABORT_DONE interrupt enable                                                                                                                   | 0b      | RW         |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 11    | Status fail with auto polling interrupt enable                                                                                                | 0b      | RW         |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 10    | Compare LCA FW interrupt enable                                                                                                               | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 9     | Erased page interrupt enable                                                                                                                  | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
|       |                                                                                                                                               |         |            |
|       | This interrupt will not stop MT.                                                                                                              |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 8     | ZIP interrupt enable                                                                                                                          | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7     | Unzip interrupt enable                                                                                                                        | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 6     | CRC16 interrupt enable                                                                                                                        | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 5     | Over error interrupt enable                                                                                                                   | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 4     | CRC32 fail interrupt enable                                                                                                                   | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3     | Error status interrupt enable                                                                                                                 | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2     | Signoff mask interrupt enable                                                                                                                 | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1     | Uncorrectable ECC interrupt enable                                                                                                            | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 0b: disable                                                                                                                                   |         |            |
|       |                                                                                                                                               |         |            |
|       | 1b: enable                                                                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0     | Interrupt vector enable0b: disable                                                                                                            | 0b      | RW         |
|       |                                                                                                                                               |         |            |
|       | 1b:                                                                                                                                           |         |            |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_POL_SEQ_0 .anchor}FCTL_POL_SEQ_0 / Poll Sequence for Queue 0 Register (Offset 88h)

Register Index: 88h \~ 8Bh

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------------
    Bit   Descriptions                                                   Default    Attributes
  ------- ------------------------------------------------------------- ---------- ------------
   31:0   FPU sequence entry for polling ready 0 status in MT control      00h          RW

  ---------------------------------------------------------------------------------------------

Register: []{#FCTL_POL_SEQ_1 .anchor}FCTL_POL_SEQ_1 / Poll Sequence for Queue 1 Register (Offset 8Ch)

Register Index: 8Ch \~ 8Fh

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------------
    Bit   Descriptions                                                   Default    Attributes
  ------- ------------------------------------------------------------- ---------- ------------
   31:0   FPU sequence entry for polling ready 1 status in MT control      00h          RW

  ---------------------------------------------------------------------------------------------

Register: []{#FCTL_SEED_INIT .anchor}FCTL_SEED_INIT /Randomize Block Seed Register (Offset 90h)

Register Index: 90h \~ 93h

Default Value: 0000_0000h

+-------+--------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                           | Default | Attributes |
+:=====:+========================================================+:=======:+:==========:+
| 31:0  | When conversion poly select as PHS randomizer          | 0000h   | RW         |
|       |                                                        |         |            |
|       | It means randomize block seed that                     |         |            |
|       |                                                        |         |            |
|       | set value to extend randomize rule for different block |         |            |
|       |                                                        |         |            |
|       | When conversion poly select as TSB randomizer          |         |            |
|       |                                                        |         |            |
|       | It means ERASESEQ_KEY that                             |         |            |
|       |                                                        |         |            |
|       | mapping to ERASESEQ_KEY of TSB randomizer IP           |         |            |
+-------+--------------------------------------------------------+---------+------------+

Register: []{#FCTL_POL_SEQ_2 .anchor}FCTL_POL_SEQ_2 / Poll Sequence for Queue 2 Register (Offset 94h)

Register Index: 94h \~ 97h

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------------
    Bit   Descriptions                                                   Default    Attributes
  ------- ------------------------------------------------------------- ---------- ------------
   31:0   FPU sequence entry for polling ready 2 status in MT control      00h          RW

  ---------------------------------------------------------------------------------------------

Register: []{#FCTL_POL_SEQ_3 .anchor}FCTL_POL_SEQ_3 / Poll Sequence for Queue 3 Register (Offset 98h)

Register Index: 98h \~ 9Bh

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------------
    Bit   Descriptions                                                   Default    Attributes
  ------- ------------------------------------------------------------- ---------- ------------
   31:0   FPU sequence entry for polling ready 3 status in MT control      00h          RW

  ---------------------------------------------------------------------------------------------

Register: []{#FCTL_BDC_CTL .anchor}FCTL_BDC_CTL /Bad Column Control Register (Offset 9Ch)

Register Index: 9Ch \~ 9Fh

Default Value: 0000_0000h

+-------+------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                         | Default | Attributes |
+:=====:+======================================================+:=======:+:==========:+
| 31    | Bad column group size select                         | 0b      | RW         |
|       |                                                      |         |            |
|       | 0b: 128B                                             |         |            |
|       |                                                      |         |            |
|       | 1b: 64B                                              |         |            |
+-------+------------------------------------------------------+---------+------------+
| 30    | Bad column calculator status                         | 0b      | RO         |
|       |                                                      |         |            |
|       | 0b: idle                                             |         |            |
|       |                                                      |         |            |
|       | 1b: busy                                             |         |            |
+-------+------------------------------------------------------+---------+------------+
| 29:27 | Reserved                                             | 0h      | RsvdP      |
+-------+------------------------------------------------------+---------+------------+
| 26    | Bad column random value                              | 0b      | RW         |
|       |                                                      |         |            |
|       | 0b: write random data for bad column value           |         |            |
|       |                                                      |         |            |
|       | 1b: write FCTL_BDC_CTL\[23:16\] for bad column value |         |            |
+-------+------------------------------------------------------+---------+------------+
| 25    | Bad column with DMA enable                           | 0b      | RW         |
+-------+------------------------------------------------------+---------+------------+
| 24    | Bad column collection trigger bit and status         | 0b      | RW         |
|       |                                                      |         |            |
|       | 0b: disable, and idle status                         |         |            |
|       |                                                      |         |            |
|       | 1b: enable, and busy status                          |         |            |
+-------+------------------------------------------------------+---------+------------+
| 23:16 | Bad column value for flash write DMA                 | 00h     | RW         |
+-------+------------------------------------------------------+---------+------------+
| 15:0  | Bad column collection length                         | 00h     | RW         |
+-------+------------------------------------------------------+---------+------------+

Register: []{#FCTL_RESERVE .anchor}FCTL_RESERVE/RESERVE Register (Offset A0h)

Register Index: A0h \~ A3h

Default Value: 0000_0000h

+-------+-----------------------------------------------------------+------------+------------+
| Bit   | Descriptions                                              | Default    | Attributes |
+:=====:+===========================================================+:==========:+:==========:+
| 31:22 | Reserved                                                  | 0000_0000h | RsvdP      |
+-------+-----------------------------------------------------------+------------+------------+
| 21    | Detect DQSB enable                                        | 0b         | RW         |
+-------+-----------------------------------------------------------+------------+------------+
| 20    | Only if bit21=1 and DQSB is toggled,Bit20 shows 1         | 0b         | RO         |
|       |                                                           |            |            |
|       | Otherwise ,Bit20 shows 0                                  |            |            |
+-------+-----------------------------------------------------------+------------+------------+
| 19:1  | Reserved                                                  | 0000_0000h | RsvdP      |
+-------+-----------------------------------------------------------+------------+------------+
| 0     | If FW manual abort MT, FW should wait this signal to be 1 | 0b         | RO         |
+-------+-----------------------------------------------------------+------------+------------+

Register: []{#FCTL_BDC_SET .anchor}FCTL_BDC_SET /Bad Column SET Register (Offset A4h)

Register Index: A4h \~ A7h

Default Value: 0000_00FEh

+-------+------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                 | Default | Attributes |
+:=====:+==============================================================================+:=======:+:==========:+
| 31    | Reserved                                                                     | 0h      | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 30:24 | Bad column group select                                                      | 0h      | RW         |
+-------+------------------------------------------------------------------------------+---------+------------+
| 23:8  | Reserved                                                                     | 0h      | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 7:0   | Compare value for Bad column collection function                             | FEh     | RW         |
|       |                                                                              |         |            |
|       | When collect data non-equal compare value, this byte will be mark bad column |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_POL_SEQ_4 .anchor}FCTL_POL_SEQ_4 / Poll Sequence for Queue 4 Register (Offset A8h)

Register Index: A8h \~ ABh

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------------
    Bit   Descriptions                                                   Default    Attributes
  ------- ------------------------------------------------------------- ---------- ------------
   31:0   FPU sequence entry for polling ready 4 status in MT control      00h          RW

  ---------------------------------------------------------------------------------------------

Register: []{#FCTL_POL_SEQ_5 .anchor}FCTL_POL_SEQ_5 / Poll Sequence for Queue 5 Register (Offset Ach)

Register Index: Ach \~ AFh

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------------
    Bit   Descriptions                                                   Default    Attributes
  ------- ------------------------------------------------------------- ---------- ------------
   31:0   FPU sequence entry for polling ready 5 status in MT control      00h          RW

  ---------------------------------------------------------------------------------------------

Register: []{#FCTL_POL_SEQ_6 .anchor}FCTL_POL_SEQ_6 / Poll Sequence for Queue 6 Register (Offset B0h)

Register Index: B0h \~ B3h

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------------
    Bit   Descriptions                                                   Default    Attributes
  ------- ------------------------------------------------------------- ---------- ------------
   31:0   FPU sequence entry for polling ready 6 status in MT control      00h          RW

  ---------------------------------------------------------------------------------------------

Register: []{#FCTL_POL_SEQ_7 .anchor}FCTL_POL_SEQ_7 / Poll Sequence for Queue 7 Register (Offset B4h)

Register Index: B4h \~ B7h

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------------
    Bit   Descriptions                                                   Default    Attributes
  ------- ------------------------------------------------------------- ---------- ------------
   31:0   FPU sequence entry for polling ready 7 status in MT control      00h          RW

  ---------------------------------------------------------------------------------------------

Register: []{#FCTL_FARG_ROW .anchor}FCTL_FARG_ROW /Flash Argument Row Address Register (Offset B8h)

Register Index: B8h \~ BBh

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                              | Default | Attributes |
+:=====:+===========================================================================================+:=======:+:==========:+
| 31:24 | Reserved                                                                                  | 00h     | RsvdP      |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 23:16 | Address phase 5^rd^ address byte value, randomize rule will reference this register value | 00h     | RO         |
|       |                                                                                           |         |            |
|       | The register is show row address after address generation function                        |         |            |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 15:8  | Address phase 4^rd^ address byte value, randomize rule will reference this register value | 00h     | RO         |
|       |                                                                                           |         |            |
|       | The register is show row address after address generation function                        |         |            |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 7:0   | Address phase 3^rd^ address byte value, randomize rule will reference this register value | 00h     | RO         |
|       |                                                                                           |         |            |
|       | The register is show row address after address generation function                        |         |            |
+-------+-------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_FARG_COL .anchor}FCTL_FARG_COL /Flash Argument Column Address Register (Offset BCh)

Register Index: BCh \~ BFh

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                              | Default | Attributes |
+:=====:+===========================================================================================+:=======:+:==========:+
| 31:16 | Reserved                                                                                  | 00h     | RsvdP      |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 15:8  | Address phase 2^nd^ address byte value, randomize rule will reference this register value | 00h     | RO         |
|       |                                                                                           |         |            |
|       | The register is show row address after address generation function                        |         |            |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 7:0   | Address phase 1^st^ address byte value, randomize rule will reference this register value | 00h     | RO         |
|       |                                                                                           |         |            |
|       | The register is show row address after address generation function                        |         |            |
+-------+-------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_FSA_ADR .anchor}FCTL_FSA_ADR /FSA Address Base Register (Offset C0h)

Register Index: C0h \~ C3h

Default Value: 0000_0000h

+-------+--------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                 | Default | Attributes |
+:=====:+==============================================================+:=======:+:==========:+
| 31:0  | FSA address base                                             | 00h     | RW         |
|       |                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align |         |            |
+-------+--------------------------------------------------------------+---------+------------+

Register: []{#FCTL_CHNL_SET .anchor}FCTL_CHNL_SET /Channel Setting Register (Offset C4h)

Register Index: C4h \~ C7h

Default Value: 0000_0000h

+-------+----------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                         | Default | Attributes |
+:=====:+======================================================================+:=======:+:==========:+
| 31    | Reserved                                                             | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------+---------+------------+
| 30    | Reserved                                                             | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------+---------+------------+
| 29    | Reserved                                                             | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------+---------+------------+
| 28    | MT_AUTO_CE_CLR                                                       | 0b      | RW         |
|       |                                                                      |         |            |
|       | 0b: disable                                                          |         |            |
|       |                                                                      |         |            |
|       | 1b:MT will auto clear CE when FPU done                               |         |            |
+-------+----------------------------------------------------------------------+---------+------------+
| 27    | Reserved                                                             | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------+---------+------------+
| 26:16 | Dummy data length                                                    | 00h     | RW         |
+-------+----------------------------------------------------------------------+---------+------------+
| 15    | Reserved                                                             | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------+---------+------------+
| 14    | Reserved                                                             | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------+---------+------------+
| 13:12 | Spare mode                                                           | 0h      | RW         |
|       |                                                                      |         |            |
|       | 00b: each frame spare region is offset 8 bytes                       |         |            |
|       |                                                                      |         |            |
|       | 01b: first frame spare region is offset 16 bytes, others are 8 bytes |         |            |
|       |                                                                      |         |            |
|       | 10b: each frame spare region is offset 16 bytes                      |         |            |
|       |                                                                      |         |            |
|       | 11b: each frame spare region is offset 256bytes                      |         |            |
+-------+----------------------------------------------------------------------+---------+------------+
| 11    | Reserved                                                             | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------+---------+------------+
| 10    | Flash EDO mode                                                       | 0b      | RW         |
|       |                                                                      |         |            |
|       | 0b: non-EDO                                                          |         |            |
|       |                                                                      |         |            |
|       | 1b: EDO                                                              |         |            |
+-------+----------------------------------------------------------------------+---------+------------+
| 9     | Conversion bypass                                                    | 0b      | RW         |
|       |                                                                      |         |            |
|       | 0b: conversion                                                       |         |            |
|       |                                                                      |         |            |
|       | 1b: bypass conversion                                                |         |            |
+-------+----------------------------------------------------------------------+---------+------------+
| 8     | Inversion bypass                                                     | 0b      | RW         |
|       |                                                                      |         |            |
|       | 0b: inversion                                                        |         |            |
|       |                                                                      |         |            |
|       | 1b: bypass inversion                                                 |         |            |
+-------+----------------------------------------------------------------------+---------+------------+
| 7     | ECC frame data length 512B                                           | 0b      | RW         |
|       |                                                                      |         |            |
|       | 0b: disable                                                          |         |            |
|       |                                                                      |         |            |
|       | 1b: enable                                                           |         |            |
+-------+----------------------------------------------------------------------+---------+------------+
| 6:0   | Frame ECC protect bit                                                | 00h     | RW         |
|       |                                                                      |         |            |
|       | Support 1\~120                                                       |         |            |
+-------+----------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_INT_CFG_1 .anchor}FCTL_INT_CFG_1 / CPU INT Configure Register (Offset C8h)

Register Index: C8h \~ CBh

Default Value: 0000_0000h

  ------------------------------------------------------------------------------------
    Bit   Descriptions                                          Default    Attributes
  ------- ---------------------------------------------------- ---------- ------------
   31:13  Reserved                                                00h        RsvdP

    12    E3D error CPU interrupt enable                           0h          RW

    11    FW setting error CPU interrupt enable                    0h          RW

    10    Status compare fail (STA) CPU interrupt enable           0h          RW

     9    Status fail with auto polling CPU interrupt enable       0h          RW

     8    Erased page CPU interrupt enable                         0h          RW

     7    Zip CPU interrupt enable                                 0h          RW

     6    Unzip CPU interrupt enable                               0h          RW

     5    CRC16 fail CPU interrupt enable                          0h          RW

     4    Uncorrectable ECC CPU interrupt enable                   0h          RW

     3    Signoff mask CPU interrupt enable                        0h          RW

     2    Over error CPU interrupt enable                          0h          RW

     1    CRC32 fail CPU interrupt enable                          0h          RW

     0    Compare LCA FW CPU interrupt enable                      0h          RW
  ------------------------------------------------------------------------------------

Register: []{#FCTL_INT_RDY .anchor}FCTL_INT_RDY / Internal RDY Register (Offset CCh)

Register Index: CCh \~ CFh

Default Value: 0000_00FFh

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:8  | Reserved                                   | 00h     | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 7:0   | Internal RDY status of MTQ                 | FFh     | RW         |
|       |                                            |         |            |
|       | 0b: busy status                            |         |            |
|       |                                            |         |            |
|       | 1b: ready status                           |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_FPU_TRIG .anchor}FCTL_FPU_TRIG / FPU Trigger Register (Offset D0h)

Register Index: D0h \~ D3h

Default Value: 0000_0004h

+-------+----------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                     | Default | Attributes |
+:=====:+==================================================================================+:=======:+:==========:+
| 31:5  | Reserved                                                                         | 00h     | RsvdP      |
+-------+----------------------------------------------------------------------------------+---------+------------+
| 7     | Timeout occur                                                                    | 0b      | RO         |
+-------+----------------------------------------------------------------------------------+---------+------------+
| 6     |                                                                                  |         |            |
+-------+----------------------------------------------------------------------------------+---------+------------+
| 5     |                                                                                  |         |            |
+-------+----------------------------------------------------------------------------------+---------+------------+
| 4     | MT_BSY                                                                           | 0b      | RO         |
|       |                                                                                  |         |            |
|       | 0b: MT is idle                                                                   |         |            |
|       |                                                                                  |         |            |
|       | 1b: MT decoding MT Process until FPU trigger is executed                         |         |            |
+-------+----------------------------------------------------------------------------------+---------+------------+
| 3     | ANY_BSY                                                                          | 0b      | RO         |
|       |                                                                                  |         |            |
|       | 0b: FPU is idle                                                                  |         |            |
|       |                                                                                  |         |            |
|       | 1b: FPU have some busy                                                           |         |            |
+-------+----------------------------------------------------------------------------------+---------+------------+
| 2     | SRQ_AVL                                                                          | 1b      | RO         |
|       |                                                                                  |         |            |
|       | For ultra DMA, the pipeline is two,                                              |         |            |
|       |                                                                                  |         |            |
|       | So, if it is low, it mean pipeline is full, it can't not trigger next FPU manual |         |            |
+-------+----------------------------------------------------------------------------------+---------+------------+
| 1     | Signoff busy, this is FPU final stage operation,                                 | 0b      | RO         |
|       |                                                                                  |         |            |
|       | This bit done is mean ECC correct done                                           |         |            |
+-------+----------------------------------------------------------------------------------+---------+------------+
| 0     | FPU sequence trigger bit                                                         | 0b      | RW         |
|       |                                                                                  |         |            |
|       | 0b: idle, and this bit is mean FPU done                                          |         |            |
|       |                                                                                  |         |            |
|       | 1b: start FPU sequence, and this bit is mean FPU busy status                     |         |            |
+-------+----------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_FPU_ENTRY .anchor}FCTL_FPU_ENTRY / FPU Entry Address Register (Offset D4h)

Register Index: D4h \~ D7h

Default Value: 0000_0000h

+-------+--------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                 | Default | Attributes |
+:=====:+==============================================================+:=======:+:==========:+
| 31:0  | FPU address base                                             | 00h     | RW         |
|       |                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 2Bytes align |         |            |
+-------+--------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MT_ADR_4 .anchor}FCTL_MT_ADR_4/ MT Address 4 Register (Offset D8h)

Register Index: D8h \~ DBh

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                 | Default | Attributes |
+:=====:+==============================================================================+:=======:+:==========:+
| 31:20 | Reserved                                                                     | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 19    | Force empty request enable                                                   | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 18    | MTQ delay function enable                                                    | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 17    | MTQ group priority function enable                                           | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable, unlock status                                                   |         |            |
|       |                                                                              |         |            |
|       | 1b: enable, lock status                                                      |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 16    | MTQ group priority definition                                                | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: group 2, low priority                                                    |         |            |
|       |                                                                              |         |            |
|       | 1b: group 1, high priority                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 15:0  | If [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[17\] = 0                           | 00h     | RW         |
|       |                                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align                 |         |            |
|       |                                                                              |         |            |
|       | If FCON_MT_ADR_BASE\[17\] = 1                                                |         |            |
|       |                                                                              |         |            |
|       | the iram address is value \* 72 + FCON_MT_ADR_BASE\[15:0\]                   |         |            |
|       |                                                                              |         |            |
|       | Ex. If the target MT table is at 0x1120 and FCON_MT_ADR_BASE\[15:0\]=0x1000, |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 0 : FW sets 0x1120                                  |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 1 : FW sets 0x4                                     |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MT_ADR_5 .anchor}FCTL_MT_ADR_5/ MT Address 5 Register (Offset DCh)

Register Index: DCh \~ DFh

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                 | Default | Attributes |
+:=====:+==============================================================================+:=======:+:==========:+
| 31:20 | Reserved                                                                     | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 19    | Force empty request enable                                                   | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 18    | MTQ delay function enable                                                    | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 17    | MTQ group priority function enable                                           | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable, unlock status                                                   |         |            |
|       |                                                                              |         |            |
|       | 1b: enable, lock status                                                      |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 16    | MTQ group priority definition                                                | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: group 2, low priority                                                    |         |            |
|       |                                                                              |         |            |
|       | 1b: group 1, high priority                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 15:0  | If [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[17\] = 0                           | 00h     | RW         |
|       |                                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align                 |         |            |
|       |                                                                              |         |            |
|       | If FCON_MT_ADR_BASE\[17\] = 1                                                |         |            |
|       |                                                                              |         |            |
|       | the iram address is value \* 72 + FCON_MT_ADR_BASE\[15:0\]                   |         |            |
|       |                                                                              |         |            |
|       | Ex. If the target MT table is at 0x1120 and FCON_MT_ADR_BASE\[15:0\]=0x1000, |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 0 : FW sets 0x1120                                  |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 1 : FW sets 0x4                                     |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MT_ADR_6 .anchor}FCTL_MT_ADR_6/ MT Address 6 Register (Offset E0h)

Register Index: E0h \~ E3h

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                 | Default | Attributes |
+:=====:+==============================================================================+:=======:+:==========:+
| 31:20 | Reserved                                                                     | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 19    | Force empty request enable                                                   | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 18    | MTQ delay function enable                                                    | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 17    | MTQ group priority function enable                                           | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable, unlock status                                                   |         |            |
|       |                                                                              |         |            |
|       | 1b: enable, lock status                                                      |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 16    | MTQ group priority definition                                                | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: group 2, low priority                                                    |         |            |
|       |                                                                              |         |            |
|       | 1b: group 1, high priority                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 15:0  | If [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[17\] = 0                           | 00h     | RW         |
|       |                                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align                 |         |            |
|       |                                                                              |         |            |
|       | If FCON_MT_ADR_BASE\[17\] = 1                                                |         |            |
|       |                                                                              |         |            |
|       | the iram address is value \* 72 + FCON_MT_ADR_BASE\[15:0\]                   |         |            |
|       |                                                                              |         |            |
|       | Ex. If the target MT table is at 0x1120 and FCON_MT_ADR_BASE\[15:0\]=0x1000, |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 0 : FW sets 0x1120                                  |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 1 : FW sets 0x4                                     |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MT_ADR_7 .anchor}FCTL_MT_ADR_7/ MT Address 7 Register (Offset E4h)

Register Index: E4h \~ E7h

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                 | Default | Attributes |
+:=====:+==============================================================================+:=======:+:==========:+
| 31:20 | Reserved                                                                     | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------------+---------+------------+
| 19    | Force empty request enable                                                   | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 18    | MTQ delay function enable                                                    | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable                                                                  |         |            |
|       |                                                                              |         |            |
|       | 1b: enable                                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 17    | MTQ group priority function enable                                           | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: disable, unlock status                                                   |         |            |
|       |                                                                              |         |            |
|       | 1b: enable, lock status                                                      |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 16    | MTQ group priority definition                                                | 0b      | RW         |
|       |                                                                              |         |            |
|       | 0b: group 2, low priority                                                    |         |            |
|       |                                                                              |         |            |
|       | 1b: group 1, high priority                                                   |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+
| 15:0  | If [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[17\] = 0                           | 00h     | RW         |
|       |                                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align                 |         |            |
|       |                                                                              |         |            |
|       | If FCON_MT_ADR_BASE\[17\] = 1                                                |         |            |
|       |                                                                              |         |            |
|       | the iram address is value \* 72 + FCON_MT_ADR_BASE\[15:0\]                   |         |            |
|       |                                                                              |         |            |
|       | Ex. If the target MT table is at 0x1120 and FCON_MT_ADR_BASE\[15:0\]=0x1000, |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 0 : FW sets 0x1120                                  |         |            |
|       |                                                                              |         |            |
|       | FCON_MT_ADR_BASE\[17\] = 1 : FW sets 0x4                                     |         |            |
+-------+------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_FAIL_CNT .anchor}FCTL_FAIL_CNT/ Interrupt Faill Counter Register (Offset E8h)

Register Index: E8h \~ EBh

Default Value: 0000_0000h

  ------------------------------------------------------------------------------------------------
    Bit   Descriptions                                                      Default    Attributes
  ------- ---------------------------------------------------------------- ---------- ------------
   31:8   Reserved                                                            00h        RsvdP

    7:0   Interrupt fail counter when Interrupt request with fail reason      00h          RW
  ------------------------------------------------------------------------------------------------

Register: []{#FCTL_MTQ_CFG .anchor}FCTL_MTQ_CFG /MT Queue Configure (Offset ECh)

Register Index: ECh \~ EFh

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                           | Default | Attributes |
+:=====:+========================================================================+:=======:+:==========:+
| 31:16 | Reserved                                                               | 00h     | RsvdP      |
+-------+------------------------------------------------------------------------+---------+------------+
| 15:8  | Mask register, mask request bit for the force empty register           | 0h      | RW         |
|       |                                                                        |         |            |
|       | 0b: disable, not mask status                                           |         |            |
|       |                                                                        |         |            |
|       | 1b: enable, mask status                                                |         |            |
|       |                                                                        |         |            |
|       | bit0: mask/not mask MTQ0 request bit                                   |         |            |
|       |                                                                        |         |            |
|       | bit1: mask/not mask MTQ1 request bit                                   |         |            |
|       |                                                                        |         |            |
|       | ...                                                                    |         |            |
|       |                                                                        |         |            |
|       | bit7: mask/not mask MTQ7 request bit                                   |         |            |
+-------+------------------------------------------------------------------------+---------+------------+
| 7:0   | Force empty register, force the empty/not empty signal from each queue | 00h     | RW         |
|       |                                                                        |         |            |
|       | 0b: disable, not empty status                                          |         |            |
|       |                                                                        |         |            |
|       | 1b: enable, empty status                                               |         |            |
|       |                                                                        |         |            |
|       | bit0: force MTQ0 empty/not empty signal                                |         |            |
|       |                                                                        |         |            |
|       | bit1: force MTQ1 empty/not empty signal                                |         |            |
|       |                                                                        |         |            |
|       | ...                                                                    |         |            |
|       |                                                                        |         |            |
|       | bit7: force MTQ7 empty/not empty signal                                |         |            |
+-------+------------------------------------------------------------------------+---------+------------+

Register: FCTL_RD_CNT /Read Counter Register (Offset F0h)

Register Index: F0h \~ F3h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:16  Flash DQS counter                                00h          RO

   15:0   Flash RE counter                                 00h          RO
  -----------------------------------------------------------------------------

Register: []{#FCTL_CNV_CFG .anchor}FCTL_RAND_KEY / Conversion Configure Register (Offset F4h)

Register Index: F4h \~ F7h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:16  Mapping to SECT_KEY of TSB randomizer IP         00h          RW

   15:0   Mapping to PAGE_KEY of TSB randomizer IP         00h          RW
  -----------------------------------------------------------------------------

Register: []{#FCTL_MTQ_INF_0 .anchor}FCTL_MTQ_INF_0 / MT Queue Information 0 Register (Offset F8h)

Register Index: F8h \~ FBh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  MT queue 3 non-execute event number              00h          RO

   23:16  MT queue 2 non-execute event number              00h          RO

   15:8   MT queue 1 non-execute event number              00h          RO

    7:0   MT queue 0 non-execute event number              00h          RO
  -----------------------------------------------------------------------------

Register: []{#FCTL_DBG_INF .anchor}FCTL_DBG_INF/ Debug Information Register (Offset FCh)

Register Index: FCh \~ FFh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:0  | Write : select which debug port            | 00h     | RW         |
|       |                                            |         |            |
|       | 0: dbg_inf = BCF_DBG;                      |         |            |
|       |                                            |         |            |
|       | 1: dbg_inf = RWG_DBG;                      |         |            |
|       |                                            |         |            |
|       | 2: dbg_inf = FDMA_DBG;                     |         |            |
|       |                                            |         |            |
|       | 3: dbg_inf = EDMA_DBG;                     |         |            |
|       |                                            |         |            |
|       | 4: dbg_inf = RDMA_DBG;                     |         |            |
|       |                                            |         |            |
|       | 5: dbg_inf = FAG_DBG;                      |         |            |
|       |                                            |         |            |
|       | 6: dbg_inf = FPU_DBG;                      |         |            |
|       |                                            |         |            |
|       | 7: dbg_inf = MT_DBG;                       |         |            |
|       |                                            |         |            |
|       | 8 : dbg_inf = SRQ_DBG_0;                   |         |            |
|       |                                            |         |            |
|       | 9 : dbg_inf = SRQ_DBG_1;                   |         |            |
|       |                                            |         |            |
|       | 10: dbg_inf = SRQ_DBG_2;                   |         |            |
|       |                                            |         |            |
|       | Read : read out debug port value           |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_SEED_MODE .anchor}FCTL_SEED_MODE / SEED Mode Register (Offset 100h)

Register Index: 100h \~ 103h

Default Value: 0000_0004h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:4   Reserved                                         00h        RsvdP

     3    Reserved                                          0b        RsvdP

     2    Reserved                                          1b        RsvdP

    1:0   Reserved                                         00b        RsvdP
  -----------------------------------------------------------------------------

Register: []{#FCTL_ERASE_CFG .anchor}FCTL_ERASE_CFG / Erase Configure Register (Offset 104h)

Register Index: 104h \~ 107h

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                    | Default | Attributes |
+:=====:+=================================================================================================================================================+:=======:+:==========:+
| 31    | Time out flag status                                                                                                                            | 0b      | RO         |
|       |                                                                                                                                                 |         |            |
|       | Need to clear by FW when time out occur,                                                                                                        |         |            |
|       |                                                                                                                                                 |         |            |
|       | and MTQ will be resumed to execute next MTP                                                                                                     |         |            |
|       |                                                                                                                                                 |         |            |
|       | 0b: non-time out                                                                                                                                |         |            |
|       |                                                                                                                                                 |         |            |
|       | 1b: time out, MTQ will be stopped,                                                                                                              |         |            |
|       |                                                                                                                                                 |         |            |
|       | and send abort request to MTQ when status read command is executed                                                                              |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 30    | Erase suspend function enable                                                                                                                   | 0b      | RW         |
|       |                                                                                                                                                 |         |            |
|       | 0b: disable                                                                                                                                     |         |            |
|       |                                                                                                                                                 |         |            |
|       | 1b: enable                                                                                                                                      |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 29    | MTQ time out stop enable                                                                                                                        | 0b      | RW         |
|       |                                                                                                                                                 |         |            |
|       | MTQ check whether it is stop or not, when time out occur. If (FCTL_ERASE_CFG\[31\] & FCTL_ERASE_CFG\[30\] & FCTL_ERASE_CFG\[29\]) then MTQ stop |         |            |
|       |                                                                                                                                                 |         |            |
|       | 0b: disable                                                                                                                                     |         |            |
|       |                                                                                                                                                 |         |            |
|       | 1b: enable                                                                                                                                      |         |            |
|       |                                                                                                                                                 |         |            |
|       | This bit will be set by status read command to enable MTQ stop after erase suspend time out.                                                    |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 28    | Time out flag status clear                                                                                                                      | 0b      | WO         |
|       |                                                                                                                                                 |         |            |
|       | 0b: disable                                                                                                                                     |         |            |
|       |                                                                                                                                                 |         |            |
|       | 1b: enable, to clear time out flag status and abort status                                                                                      |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27:0  | Expect time out value                                                                                                                           | 00b     | RW         |
|       |                                                                                                                                                 |         |            |
|       | When time out counter equal expect time out value,                                                                                              |         |            |
|       |                                                                                                                                                 |         |            |
|       | FPU erase suspend sequence will be executed and                                                                                                 |         |            |
|       |                                                                                                                                                 |         |            |
|       | Time out status will be set as 1\'b1                                                                                                            |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_ERASE_SEQ .anchor}FCTL_ERASE_SEQ / Erase Sequence Register (Offset 108h)

Register Index: 108h \~ 10Bh

Default Value: 0000_0000h

  --------------------------------------------------------------------------------------
    Bit   Descriptions                                            Default    Attributes
  ------- ------------------------------------------------------ ---------- ------------
   31:0   Erase sequence entry for erase suspend in MT control      00h          RW

  --------------------------------------------------------------------------------------

Register: []{#FCTL_MGM_CFG .anchor}FCTL_MGM_CFG / MT Group Monitor Register (Offset 108h)

Register Index: 10Ch \~ 10Fh

Default Value: 0000_0000h

+-------+----------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                   | Default | Attributes |
+:=====:+================================================================+:=======:+:==========:+
| 31:7  | Reserved                                                       | 00h     | RsvdP      |
+-------+----------------------------------------------------------------+---------+------------+
| 6:1   | Mapping MT group monitor which group monitor will be increased | 00h     | RW         |
|       |                                                                |         |            |
|       | 0h: MT group monitor 0 will be increased                       |         |            |
|       |                                                                |         |            |
|       | 1h: MT group monitor 1 will be increased                       |         |            |
|       |                                                                |         |            |
|       | ...                                                            |         |            |
|       |                                                                |         |            |
|       | 35h: MT group monitor 35 will be increased                     |         |            |
+-------+----------------------------------------------------------------+---------+------------+
| 0     | MT group monitor function enable                               | 0b      | RW         |
|       |                                                                |         |            |
|       | 0b: disable                                                    |         |            |
|       |                                                                |         |            |
|       | 1b: enable                                                     |         |            |
+-------+----------------------------------------------------------------+---------+------------+

Register: []{#FCTL_MTQ_INF_1 .anchor}FCTL_MTQ_INF_1 / MT Queue Information 1 Register (Offset 110h)

Register Index: 110h \~ 113h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
          Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  MT queue 7 non-execute event number              00h          RO

   23:16  MT queue 6 non-execute event number              00h          RO

   15:8   MT queue 5 non-execute event number              00h          RO

    7:0   MT queue 4 non-execute event number              00h          RO
  -----------------------------------------------------------------------------

Register: []{#FCTL_MTQ_DLY .anchor}FCTL_MTQ_DLY / MT Queue Delay Register (Offset 114h)

Register Index: 114h \~ 117h

Default Value: 0000_0000h

+-------+----------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                   | Default | Attributes |
+:=====:+================================================================================================================+:=======:+:==========:+
| 31:24 | MT delay function cancel                                                                                       | 00h     | RW         |
|       |                                                                                                                |         |            |
|       | Cancel current delay period and apply new delay period immediately                                             |         |            |
|       |                                                                                                                |         |            |
|       | bit0: cancel MTQ0                                                                                              |         |            |
|       |                                                                                                                |         |            |
|       | bit1: cancel MTQ1                                                                                              |         |            |
|       |                                                                                                                |         |            |
|       | ...                                                                                                            |         |            |
|       |                                                                                                                |         |            |
|       | bit7: cancel MTQ7                                                                                              |         |            |
+-------+----------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:0  | MTQ delay is indicated how many system clock cycles will next MTP be waited to trigger after current MTP done. | 00h     | RW         |
+-------+----------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_FDIV_CFG .anchor}FCTL_FDIV_CFG / Flash Clock Divider Configure Register (Offset 118h)

Register Index: 118h \~ 11Bh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:7  | Reserved                                   | 00h     | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 6     | Flash clock divider enable                 | 0b      | RW         |
|       |                                            |         |            |
|       | 0b: disable                                |         |            |
|       |                                            |         |            |
|       | 1b: enable                                 |         |            |
+-------+--------------------------------------------+---------+------------+
| 5:0   | Flash clock divider value                  | 0h      | RW         |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_NVME_CFG_0 .anchor}FCTL_NVME_CFG_0 / MT Queue Information 1 Register (Offset 11Ch)

Register Index: 11Ch \~ 11Fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:27  Reserved                                         00h        RsvdP

   26:18  Reserved                                         00h        RsvdP

   17:9   Reserved                                         00h        RsvdP

    8:0   Reserved                                         00h        RsvdP
  -----------------------------------------------------------------------------

Register: []{#FCTL_LDPC_MASK .anchor}FCTL_LDPC_MASK / LDPC Mask Register (Offset 124h)

Register Index: 124h \~ 127h

Default Value: 0000_0000h

+-------+---------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                  | Default | Attributes |
+:=====:+===============================================================+:=======:+:==========:+
| 31:16 | Reserved                                                      | 00h     | RW         |
+-------+---------------------------------------------------------------+---------+------------+
| 15:0  | Mask LDPC frame. For hard-bit retry.                          | 00h     | RW         |
|       |                                                               |         |            |
|       | If bit-N is 1, the (N+1)-th LDPC frame will not be refreshed. |         |            |
+-------+---------------------------------------------------------------+---------+------------+

Register: []{#FCTL_L4K_SPR_ADR .anchor}FCTL_L4K_SPR_ADR / L4K Spare Address Register (Offset 128h)

Register Index: 128h \~ 12Bh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:0  | Indicate L4K table address in IRAM         | 00h     | RW         |
|       |                                            |         |            |
|       | unit is Bytes, but need 8Bytes align       |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_ZIP_CFG .anchor}FCTL_ZIP_CFG / ZIP Config Register (Offset 12Ch)

Register Index: 12Ch \~ 12Fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:17  Reserved                                         00h          RW

    16    ZIP Ring mode                                    00h          RW

   15:8   FW_3                                             00h          RW

     7    L4k BADR continuous mode                          0h          RW

     6    ZIP bypass enable                                 1b          RW

    5:0   L4K num                                          00h          RW
  -----------------------------------------------------------------------------

Register: []{#FCTL_RAW_DMA_ADR .anchor}FCTL_RAW_DMA_ADR / RAW DMA Address Register (Offset 130h)

Register Index: 130h \~ 133h

Default Value: 0000_0000h

+-------+-------------------------------------------------+---------+------------+
| Bit   | Descriptions                                    | Default | Attributes |
+:=====:+=================================================+:=======:+:==========:+
| 31:0  | Indicate DRAM address                           | 00h     | RW         |
|       |                                                 |         |            |
|       | For read flash soft bit data from flash to DRAM |         |            |
+-------+-------------------------------------------------+---------+------------+

Register: []{#FCTL_BACK_RESTORE .anchor}FCTL_BACK_RESTORE / Back Restore Register (Offset 134h)

Register Index: 134h \~ 137h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:16  Reserved                                         00h        RsvdP

   15:0   Backup-Restore IRAM address                      00h          RW
  -----------------------------------------------------------------------------

Register: []{#FCTL_RS_CFG .anchor}FCTL_RS_CFG / RS Config Register (Offset 138h)

Register Index: 138h \~ 13Bh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:25 | Reserved                                   | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 24    | RS one parity mode enable                  | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 23:16 | RS page number for one RS frame            | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 12:8  | RS tag, 0\~31                              | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 5     | 1b: RS decrease PEC count                  | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 4     | 1b: RS program second parity page to FLH   | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 3     | Last page valid for RS frame               | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 2     | Program parity enable                      | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 1     | On-the-fly enable                          | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 0     | Encode mode                                | 00h     | RW         |
|       |                                            |         |            |
|       | 0:normorl mode                             |         |            |
|       |                                            |         |            |
|       | 1:XOR mode                                 |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_LDPC_CFG .anchor}FCTL_LDPC_CFG / LDPC Config Register (Offset 13Ch)

Register Index: 13Ch \~ 13Fh

Default Value: 0000_0320h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:16 | Reserved                                   | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 15    | BCH result of Soft bit correct             |         | RO         |
|       |                                            |         |            |
|       | 1: Uncorrectable                           |         |            |
|       |                                            |         |            |
|       | 0:correctable                              |         |            |
+-------+--------------------------------------------+---------+------------+
| 14    | LDPC result of Soft bit correct            |         | RO         |
|       |                                            |         |            |
|       | 1:LDPC success                             |         |            |
|       |                                            |         |            |
|       | 0:LDPC fail                                |         |            |
+-------+--------------------------------------------+---------+------------+
| 13    | LDPC Correct enable                        | 1h      | RW         |
+-------+--------------------------------------------+---------+------------+
| 12:0  | Reserved                                   | 00h     | RsvdP      |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_RD_CNT .anchor}FCTL_RD_CNT (Offset 140h)

Register Index: 140h \~ 143h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:16  Reserved                                         00h        RsvdP

   15:0   RE counter                                       00h          RO
  -----------------------------------------------------------------------------

Register: []{#FCTL_RD2_CNT .anchor}FCTL_RD2_CNT (Offset 144h)

Register Index: 144h \~ 147h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:16  DQS rising edge counter                          00h          RO

   15:0   DQS falling edge counter                         00h          RO
  -----------------------------------------------------------------------------

Register: FCTL_DYNA[]{#FCTL_DYNAMIC_ODT .anchor}MIC_ODT / Dynamic ODT Config Register (Offset 148h)

Register Index: 148h \~ 14Bh

Default Value: 0000_0000h

+-------+--------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                     | Default | Attributes |
+:=====:+==================================================================================================+:=======:+:==========:+
| 31:3  | Reserved                                                                                         | 00h     | RO         |
+-------+--------------------------------------------------------------------------------------------------+---------+------------+
| 2     | ODT_FORCE_VALUE                                                                                  | 0h      | RW         |
|       |                                                                                                  |         |            |
|       | If ODT_FORCE_ENABLE is 1, the ODT function is enabled based on ODT_FORCE_VALUE                   |         |            |
+-------+--------------------------------------------------------------------------------------------------+---------+------------+
| 1     | ODT_FORCE_ENABLE                                                                                 | 0h      | RW         |
+-------+--------------------------------------------------------------------------------------------------+---------+------------+
| 0     | DYNAMIC_ODT_EN: Enable Dynamic ODT function                                                      | 0h      | RW         |
|       |                                                                                                  |         |            |
|       | This bit is valid when ODT_FORCE_EN=0.                                                           |         |            |
|       |                                                                                                  |         |            |
|       | 0: ODT control pin is controlled by ODT_FORCE_ENABLE and ODT_FORCE_ENABLE.                       |         |            |
|       |                                                                                                  |         |            |
|       | 1: ODT control pin is set to '1' when TOGGLE/ONFI read and set to '0' when not TOGGLE/ONFI read. |         |            |
+-------+--------------------------------------------------------------------------------------------------+---------+------------+

Register: [FCTL\_](#FCTL_DYNAMIC_ODT)IBF_CTL / Internal buffer control Register (Offset 14Ch)

Register Index: 14Ch \~ 14Fh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+=======+============================================+=========+============+
| 9     | FORCE_ZIP_COR                              | 0h      | RW         |
|       |                                            |         |            |
|       | 0:normal mode                              |         |            |
|       |                                            |         |            |
|       | 1:ZIP always dump data to dram             |         |            |
+-------+--------------------------------------------+---------+------------+
| 4:0   | LDPC_COR_MAP                               | 1fh     | RW         |
|       |                                            |         |            |
|       | LDPC correct each 4K data of IBF           |         |            |
|       |                                            |         |            |
|       | Bit0: for first 4K                         |         |            |
|       |                                            |         |            |
|       | 0: disable LDPC correct                    |         |            |
|       |                                            |         |            |
|       | 1: enable LDPC correct                     |         |            |
|       |                                            |         |            |
|       | Bit1: for second 4K                        |         |            |
|       |                                            |         |            |
|       | Bit2: for third 4K                         |         |            |
|       |                                            |         |            |
|       | Bit3: for Fourth 4K                        |         |            |
|       |                                            |         |            |
|       | Bit4: for Fifth 4K                         |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCTL_ECC_DBG .anchor}FCTL_ECC_DBG (Offset 150h)

Register Index: 150h \~ 153h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:8   RsvdP                                           0000h      Reserved

    7:4   LDPC current correct count                        0h          RO

    3:0   LDPC current encode/decode count                  0h          RO
  -----------------------------------------------------------------------------

Register: []{#FCTL_IFSA0 .anchor}FCTL_IFSA0 / Internal FSA0 Register (Offset 154h)

Register Index: 154h \~ 157h

Default Value: 0000_0000h

+-------+-----------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                            | Default | Attributes |
+:=====:+=========================================================================================+:=======:+:==========:+
| 31:0  | Internal FSA0                                                                           | 0h      | RW         |
|       |                                                                                         |         |            |
|       | If [FCTL_FSA_SEL](#FCTL_FSA_SEL)\[18\] = 1, HW fetches FSA from here rather than IRAM . |         |            |
|       |                                                                                         |         |            |
|       | This register is for the 1st address gen fpu                                            |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_IFSA1 .anchor}FCTL_IFSA1 / Internal FSA1 Register (Offset 158h)

Register Index: 158h \~ 15bh

Default Value: 0000_0000h

+-------+-----------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                            | Default | Attributes |
+:=====:+=========================================================================================+:=======:+:==========:+
| 31:0  | Internal FSA1                                                                           | 0h      | RW         |
|       |                                                                                         |         |            |
|       | If [FCTL_FSA_SEL](#FCTL_FSA_SEL)\[18\] = 1, HW fetches FSA from here rather than IRAM . |         |            |
|       |                                                                                         |         |            |
|       | This register is for the 2nd address gen fpu                                            |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_IFSA2 .anchor}FCTL_IFSA2 / Internal FSA2 Register (Offset 15Ch)

Register Index: 15Ch \~ 15fh

Default Value: 0000_0000h

+-------+-----------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                            | Default | Attributes |
+:=====:+=========================================================================================+:=======:+:==========:+
| 31:0  | Internal FSA2                                                                           | 0h      | RW         |
|       |                                                                                         |         |            |
|       | If [FCTL_FSA_SEL](#FCTL_FSA_SEL)\[18\] = 1, HW fetches FSA from here rather than IRAM . |         |            |
|       |                                                                                         |         |            |
|       | This register is for the 3rd address gen fpu                                            |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_IFSA3 .anchor}FCTL_IFSA3 / Internal FSA3 Register (Offset 160h)

Register Index: 160h \~ 163h

Default Value: 0000_0000h

+-------+-----------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                            | Default | Attributes |
+:=====:+=========================================================================================+:=======:+:==========:+
| 31:0  | Internal FSA3                                                                           | 0h      | RW         |
|       |                                                                                         |         |            |
|       | If [FCTL_FSA_SEL](#FCTL_FSA_SEL)\[18\] = 1, HW fetches FSA from here rather than IRAM . |         |            |
|       |                                                                                         |         |            |
|       | This register is for the 4th address gen fpu                                            |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_FPU_INT_VCT .anchor}FCTL_FPU_INT_VCT Register (Offset 164h)

Register Index: 164h \~ 167h

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                      | Default | Attributes |
+:=====:+===================================================================+:=======:+:==========:+
| 31:24 | RsvdP                                                             | 0000h   | Reserved   |
+-------+-------------------------------------------------------------------+---------+------------+
| 23:0  | Current FPU INT_VCT/Index                                         | 00h     | RO         |
|       |                                                                   |         |            |
|       | When timeout occur, FW can get timeout FPU index/interrupt vector |         |            |
+-------+-------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_DMA_DONE_VCT .anchor}FCTL_DMA_DONE_VCT/ Register (Offset 168h)

Register Index: 168h \~ 16Bh

Default Value: 0000_0000h

+-------+---------------------------------------------------------------------+---------+------------+
| Bit   |                                          Descriptions               | Default | Attributes |
+:=====:+=====================================================================+:=======:+:==========:+
| 31:24 | RsvdP                                                               | 0000h   | Reserved   |
+-------+---------------------------------------------------------------------+---------+------------+
| 23:0  | DMA done INT_VCT/Index                                              | 00h     | RO         |
|       |                                                                     |         |            |
|       | When R/W DMA done, HW will update interrupt vector to this register |         |            |
+-------+---------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_FRONT_VCT .anchor}FCTL_FRONT_VCT/ Register (Offset 16Ch)

Register Index: 16Ch \~ 16Fh

Default Value: 0000_0000h

+-------+--------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   |                                          Descriptions                                                                                            | Default | Attributes |
+:=====:+==================================================================================================================================================+:=======:+:==========:+
| 31:24 | RsvdP                                                                                                                                            | 0000h   | Reserved   |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:0  | Front DMA INT_VCT/Index                                                                                                                          | 00h     | RO         |
|       |                                                                                                                                                  |         |            |
|       | This register shows the interrupt vector of front (previous) DMA. If FCTL_FRONT_VCT!= FCTL_BACK_VCT, the HW is processing correct/decompression. |         |            |
+-------+--------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_BACK_VCT .anchor}FCTL_BACK_VCT/ Register (Offset 170h)

Register Index: 170h \~ 173h

Default Value: 0000_0000h

+-------+---------------------------------------------------------------+---------+------------+
| Bit   |                                          Descriptions         | Default | Attributes |
+:=====:+===============================================================+:=======:+:==========:+
| 31:24 | RsvdP                                                         | 0000h   | Reserved   |
+-------+---------------------------------------------------------------+---------+------------+
| 23:0  | Back DMA INT_VCUDMA_PHST/Index                                | 00h     | RO         |
|       |                                                               |         |            |
|       | This register shows the interrupt vector of back(current) DMA |         |            |
+-------+---------------------------------------------------------------+---------+------------+

Register: / Register (Offset 174h)

Register Index: 174h \~ 177h

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------
    Bit                                            Descriptions    Default    Attributes
  ------- ------------------------------------------------------- ---------- ------------
   31:0   RsvdP                                                     0000h      Reserved

  ---------------------------------------------------------------------------------------

Register: / Register (Offset 178h)

Register Index: 178h \~ 17bh

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------
    Bit                                            Descriptions    Default    Attributes
  ------- ------------------------------------------------------- ---------- ------------
   31:0   RsvdP                                                     0000h      Reserved

  ---------------------------------------------------------------------------------------

Register: []{#FCTL_ZIP_DMA_ST .anchor}FCTL_ZIP_DMA_ST/ Register (Offset 17Ch)

Register Index: 17Ch \~ 17Fh

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------------+---------+------------+
| Bit   |                                          Descriptions                   | Default | Attributes |
+:=====:+=========================================================================+:=======:+:==========:+
| 31:16 | RsvdP                                                                   | 0000h   | Reserved   |
+-------+-------------------------------------------------------------------------+---------+------------+
| 15:0  | ZIP finish L4K map                                                      | 00h     | RO         |
|       |                                                                         |         |            |
|       | Bitn =1 : n-th l4k of [FCTL_FRONT_VCT](#FCTL_FRONT_VCT) DMA is finsihed |         |            |
+-------+-------------------------------------------------------------------------+---------+------------+

Register: []{#FCTL_E3D_ERR_CNT .anchor}FCTL_E3D_ERR_CNT/ Register (Offset 180h)

Register Index: 180h \~ 183h

Default Value: 0000_0000h

  ---------------------------------------------------------------------------------------
    Bit                                            Descriptions   Default    Attributes
  ------- ------------------------------------------------------- ---------- ------------
    31    Clear \[30:0\]

   30:0   Counter of E3D fail(unit : Logical 4 K)                 0000h      Reserved

  ---------------------------------------------------------------------------------------

Register: []{#FCTL_DMA_STS_0 .anchor}FCTL_DMA_STS_0/ Register (Offset 184h)

Register Index: 184h \~ 187h

Default Value: 0000_0000h

+-------+-----------------------------------------------------------------------------------+---------+------------+
| Bit   |                                          Descriptions                             | Default | Attributes |
+:=====:+===================================================================================+:=======:+:==========:+
| 31:24 | RsvdP                                                                             | 0h      | Reserved   |
+-------+-----------------------------------------------------------------------------------+---------+------------+
| 23:20 | IBF_ZIP_STS (only valid when read flash and INT_PHS                               | 0h      | RO         |
|       |                                                                                   |         |            |
|       | = 0)                                                                              |         |            |
|       |                                                                                   |         |            |
|       | Ex: 4'b0001 🡺 first frame data of this DMA in IBF has transferred to ZIP          |         |            |
|       |                                                                                   |         |            |
|       | 4'b0011 🡺first and second frame data of this DMA in IBF have transferred to ZIP   |         |            |
+-------+-----------------------------------------------------------------------------------+---------+------------+
| 19:16 | FLH_IBF_STS (only valid when read flash and INT_PHS                               | 0h      | RO         |
|       |                                                                                   |         |            |
|       | = 0)                                                                              |         |            |
|       |                                                                                   |         |            |
|       | Ex: 4'b0001 🡺 first frame data of this DMA in NAND has transferred to IBF         |         |            |
|       |                                                                                   |         |            |
|       | 4'b0011 🡺 first and second frame data of this DMA in NAND have transferred to IBF |         |            |
+-------+-----------------------------------------------------------------------------------+---------+------------+
| 15:12 | IBF_FLH_STS (only valid when program flash and INT_PHS                            | 0h      | RO         |
|       |                                                                                   |         |            |
|       | = 0)                                                                              |         |            |
|       |                                                                                   |         |            |
|       | Ex: 4'b0001 🡺 first frame data of this DMA in IBF has transfer to NAND            |         |            |
|       |                                                                                   |         |            |
|       | 4'b0011 🡺 first and second frame data of this DMA in IBF have transfer to NAND    |         |            |
+-------+-----------------------------------------------------------------------------------+---------+------------+
| 11:8  | ZIP_IBF_STS (only valid when program flash and INT_PHS                            | 0h      | RO         |
|       |                                                                                   |         |            |
|       | = 0)                                                                              |         |            |
|       |                                                                                   |         |            |
|       | Ex: 4'b0001 🡺 first frame data of this DMA has transferred to IBF from ZIP        |         |            |
|       |                                                                                   |         |            |
|       | 4'b0011 🡺 first and second frames data of this DMA have transferred to IBF from   |         |            |
|       |                                                                                   |         |            |
|       | ZIP                                                                               |         |            |
+-------+-----------------------------------------------------------------------------------+---------+------------+
| 7:5   | RsvdP                                                                             | 0h      | Reserved   |
+-------+-----------------------------------------------------------------------------------+---------+------------+
| 4     | INT_PHS                                                                           | 0h      | RO         |
|       |                                                                                   |         |            |
|       | 1'b1 : send interrupt phase                                                       |         |            |
+-------+-----------------------------------------------------------------------------------+---------+------------+
| 3:2   | Current frame number of Flash DMA (only valid when flash_dma_phs is high)         | 0h      | RO         |
+-------+-----------------------------------------------------------------------------------+---------+------------+
| 1     | Flash_dma_phs                                                                     | 0h      | RO         |
|       |                                                                                   |         |            |
|       | 1'b1 : data transferring on flash interface                                       |         |            |
+-------+-----------------------------------------------------------------------------------+---------+------------+
| 0     | Ultra DMA phase                                                                   | 0h      | RO         |
+-------+-----------------------------------------------------------------------------------+---------+------------+
|       |                                                                                   |         |            |
+-------+-----------------------------------------------------------------------------------+---------+------------+

## 5.3. Register OF TOP

The address map of control registers for Top in below

  ----------------------------------------------------------------------------------------------
  Offset   Name                                                  Mode   Function
  -------- ----------------------------------------------------- ------ ------------------------
  0x00     [FCON_INFO](#FCON_INFO)                               rw

  0x04     [FCON_MODE_SET](#FCON_MODE_SET)                       rw

  0x08     [FCON_FCE_ENB](#FCON_FCE_ENB)                         rw

  0x0C     [FCON_FCE_ENB_1](#FCON_FCE_ENB_1)                     rw

  0x18     [FCON_SMP_CFG](#FCON_SMP_CFG)                         rw

  0x1C     [FCON_FORMAT](#FCON_FORMAT)                           rw

  0x20

  0x24

  0x28     [FCON_IRC_CTL](#FCON_IRC_CTL)                         rw

  0x2C     [FCON_IRC_ADR_0](#FCON_IRC_ADR_0)                     rw

  0x30     [FCON_CNT_ERR_SET](#FCON_CNT_ERR_SET)                 rw

  0x34     [FCON_CNT_ERR](#FCON_CNT_ERR)                         rw

  0x38     [FCON_FLH_FUNC](#FCON_FLH_FUNC)                       rw

  0x3C     [FCON_AXIS_ADR](#FCON_AXIS_ADR)                       rw

  0x40     [FCON_MNT_CFG](#FCON_MNT_CFG)                         rw

  0x44     [FCON_SGN_MSK](#FCON_SPR_VALID_FUNC)                  rw

  0x48     [FCON_MNT_CNT](#FCON_MNT_CNT)                         rw

  0x4C     [FCON_RTY_CFG](#FCON_RTY_CFG)                         rw

  0x50     [FCON_SPECIAL](#FCON_SPECIAL)                         rw

  0x64     [FCON_LGC_TIME_CFG](#FCON_LGC_TIME_CFG)               rw

  0x68     [FCON_TGL_TIME_CFG](#FCON_TGL_TIME_CFG)               rw

  0x6C     [FCON_ONF_TIME_CFG](#FCON_ONF_TIME_CFG)               rw

  0x70     [FCON_LGC_TIME_CFG_1](#FCON_LGC_TIME_CFG_1)           rw

  0x74     [FCON_TGL_TIME_CFG_1](#FCON_TGL_TIME_CFG_1)           rw

  0x78     [FCON_ONF_TIME_CFG_1](#FCON_ONF_TIME_CFG_1)           rw

  0x7C                                                           rw

  0x80                                                           rw

  0x84     [FCON_FCE_SEL_0](#FCON_FCE_SEL_0)                     rw

  0x88     [FCON_FCE_SEL_1](#FCON_FCE_SEL_1)                     rw

  0x8C     [FCON_FCE_SEL_2](#FCON_FCE_SEL_2)                     rw

  0x90     [FCON_FCE_SEL_3](#FCON_FCE_SEL_3)                     rw

  0x94     [FCON_FCE_SEL_4](#FCON_FCE_SEL_4)                     rw

  0x98     [FCON_FCE_SEL_5](#FCON_FCE_SEL_5)                     rw

  0x9C     [FCON_FCE_SEL_6](#FCON_FCE_SEL_6)                     rw

  0xA0     [FCON_FCE_SEL_7](#FCON_FCE_SEL_7)                     rw

  0xA4     [FCON_FCE_SEL_8](#FCON_FCE_SEL_8)                     rw

  0xA8     [FCON_FCE_SEL_9](#FCON_FCE_SEL_9)                     rw

  0xAC     [FCON_FCE_SEL_10](#FCON_FCE_SEL_10)                   rw

  0xB0     [FCON_FCE_SEL_11](#FCON_FCE_SEL_11)                   rw

  0xB4     [FCON_FCE_SEL_12](#FCON_FCE_SEL_12)                   rw

  0xB8     [FCON_FCE_SEL_13](#FCON_FCE_SEL_13)                   rw

  0xBC     [FCON_FCE_SEL_14](#FCON_FCE_SEL_14)                   rw

  0xC0     [FCON_FCE_SEL_15](#FCON_FCE_SEL_15)                   rw

  0xC4     [FCON_D2I_IRADR](#FCON_D2I_IRADR)                     rw

  0xC8     [FCON_D2I_DRADR](#FCON_D2I_DRADR)                     rw

  0xCC     [FCON_D2I_CTL](#FCON_D2I_CTL)                         rw

  0xD0     [FCON_CRC_EN](#FCON_CRC_EN)                           rw

  0xD4     [FCON_CRC_FORCE](#FCON_CRC_FORCE)                     rw

  0xD8     [FCON_TMR_CTL](#FCON_TMR_CTL)                         rw

  0xDC                                                           rw

  0xE0     [FCON_ALU_BAS](#FCON_ALU_BAS)                         rw

  0xE4     [FCON_MGM_INF](#FCON_ROT_LEN)                         rw

  0xE8     [FCON_MT_STS](#FCON_MT_STS)                           rw

  0xEC     [FCON_MT_CFG](#FCON_MT_CFG)                           rw

  0xF0     [FCON_ADR_GEN](#FCON_ADR_GEN)                         rw

  0xF4     [FCON_ADR_GEN_STOP](#FCON_ADR_GEN_STOP)               rw

  0xF8     [FCON_FCE_SET](#FCON_FCE_SET)                         w

  0xFC     [FCON_FCE_CLR](#FCON_FCE_CLR)                         w

  0x100    [FCON_MGM_SEL](#FCON_ROT_SAT)                         rw

  0x104                                                          rw

  0x108    [FCON_IRC_ADR_1](#FCON_IRC_ADR_1)                     rw

  0x10C    [FCON_FW_RESET](#FCON_FW_RESET)                       rw

  0x110    [FCON_LDPC_CFG](#FCON_LDPC_CFG)                       rw

  0x114    [FCON_LDPC_BFC_SET](#FCON_LDPC_BFC_SET)               rw

  0x118    FCON_LDPC_BFC                                         rw

  0x11C    [FCON_PAGE_CFG](#FCON_PAGE_CFG)                       rw

  0x120    [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)                 rw

  0x124    [FCON_SYS_INTQ_BASE](#FCON_SYS_INTQ_BASE)             rw

  0x128    [FCON\_ FLHCLK_SPLITE_EN](#FCON_FLHCLK_SPLITE_EN)

  0x12C    [FCON_MTQ_INF_0](#FCON_MTQ_INF_0)                     ro

  0x130    [FCON_MTQ_INF_1](#FCON_MTQ_INF_1)                     ro

  0x134    [FCON_MTQ_INF_2](#FCON_MTQ_INF_2)                     ro

  0x138    [FCON_MTQ_INF_3](#FCON_MTQ_INF_3)                     ro

  0x13c    [FCON_ERASE_PAG](#FCON_ERASE_PAG)                     rw

  0x140    [FCON_LLR_TABLE_0](#FCON_LLR_TABLE_0)                 rw

  0x144    FCON_LLR_TABLE_4                                      rw

  0x148    FCON_LLR_TABLE_8                                      rw

  0x14c    FCON_LLR_TABLE_12                                     rw

  0x150    FCON_LLR_TABLE_16                                     rw

  0x154    FCON_LLR_TABLE_20                                     rw

  0x158    FCON_LLR_TABLE_24                                     rw

  0x15c    FCON_LLR_TABLE_28                                     rw

  0x160    [FCON_DSP_PARAM_0](#FCON_DSP_PARAM_0)                 rw

  0x164    FCON_DSP_PARAM_1                                      rw

  0x168    [FCON_DSP_GRAY_CODE_0](#FCON_DSP_GRAY_CODE_0)         rw

  0x16c    FCON_DSP_GRAY_CODE_1                                  rw

  0x170    FCON_DSP_GRAY_CODE_2                                  rw

  0x174    FCON_DSP_GRAY_CODE_3                                  rw

  0x178    FCON_DSP_GRAY_CODE_4                                  rw

  0x17c    [FCON_DSP_LUT_0](#FCON_DSP_LUT_0)                     rw

  0x180    FCON_DSP_LUT_1                                        rw

  0x184    FCON_DSP_LUT_2                                        rw

  0x188    FCON_DSP_LUT_3                                        rw

  0x18c    FCON_DSP_LUT_4                                        rw

  0x190    FCON_DSP_LUT_5                                        rw

  0x194    FCON_DSP_CENTER                                       ro

  0x198    [FCON_DSP_DISTRIBUTION_0](#FCON_DSP_DISTRIBUTION_0)   ro

  0x19c    FCON_DSP_DISTRIBUTION_1                               ro

  0x1a0    FCON_DSP_DISTRIBUTION_2                               ro

  0x1a4    FCON_DSP_DISTRIBUTION_3                               ro

  0x1a8    FCON_DSP_DISTRIBUTION_4                               ro

  0x1ac    FCON_DSP_DISTRIBUTION_5                               ro

  0x1b0    FCON_DSP_DISTRIBUTION_6                               ro

  0x1b4    FCON_DSP_DISTRIBUTION_7                               ro

  0x1b8    FCON_DSP_DISTRIBUTION_8                               ro

  0x1bc    FCON_DSP_DISTRIBUTION_9                               ro

  0x1c0    FCON_DSP_DISTRIBUTION_10                              ro

  0x1c4    FCON_DSP_DISTRIBUTION_11                              ro

  0x1c8    FCON_DSP_DISTRIBUTION_12                              ro

  0x1cc    FCON_DSP_DISTRIBUTION_13                              ro

  0x1d0    FCON_DSP_DISTRIBUTION_14                              ro

  0x1d4    FCON_DSP_DISTRIBUTION_15                              ro
  ----------------------------------------------------------------------------------------------

  : Register list for top setting

## 5.4. Details specification for Register of TOP

Register: []{#FCON_INFO .anchor}FCON_INFO /Channel Information Register (Offset 00h)

Register Index: 00h \~ 03h

Default Value: 0000_0000h

+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                                                                 | Default | Attributes |
+:=====:+==============================================================================================================================================================================================+:=======:+:==========:+
| 31:24 | Reserved                                                                                                                                                                                     | 0b      | RsvdP      |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 23:16 | Flash channel select flag                                                                                                                                                                    | 00h     | RW         |
|       |                                                                                                                                                                                              |         |            |
|       | Flash channel select, when set 1 the all register will be mask for MCU read/write for channel,                                                                                               |         |            |
|       |                                                                                                                                                                                              |         |            |
|       | Ex. MCU want to use all register to write channel 0 and 3, but not write channel 1 and 2, it can set this register to 06h, then use all register to write, the channel 1 and 2 will be mask. |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15:0  | Reserved                                                                                                                                                                                     | 00h     | RsvdP      |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_MODE_SET .anchor}FCON_MODE_SET /Mode Set Register (Offset 04h)

Register Index: 04h \~ 07h

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31    | CPU wait mode                              | 0b      | RW         |
|       |                                            |         |            |
|       | 0b:handshake mode                          |         |            |
|       |                                            |         |            |
|       | 1b:counter mode                            |         |            |
+-------+--------------------------------------------+---------+------------+
| 30:24 | Correct control pin                        | 00h     | RW         |
|       |                                            |         |            |
|       | TERMI_COEF                                 |         |            |
+-------+--------------------------------------------+---------+------------+
| 23    | Correct control pin                        | 0b      | RW         |
|       |                                            |         |            |
|       | TERMI_EN                                   |         |            |
+-------+--------------------------------------------+---------+------------+
| 22    | Correct control pin                        | 0b      | RW         |
|       |                                            |         |            |
|       | LENGTH_EN                                  |         |            |
+-------+--------------------------------------------+---------+------------+
| 21    | Correct control pin                        | 0b      | RW         |
|       |                                            |         |            |
|       | CS_ACC_EN                                  |         |            |
+-------+--------------------------------------------+---------+------------+
| 20    | Correct control pin                        | 0b      | RW         |
|       |                                            |         |            |
|       | ACC_EN                                     |         |            |
+-------+--------------------------------------------+---------+------------+
| 19    | Reserved                                   | 0b      | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 18    | Correct CLK control                        | 0b      | RW         |
|       |                                            |         |            |
|       | 0b: Correct CLK enable at operation        |         |            |
|       |                                            |         |            |
|       | 1b: Correct CLK always enable              |         |            |
+-------+--------------------------------------------+---------+------------+
| 17    | ECC CLK control                            | 0b      | RW         |
|       |                                            |         |            |
|       | 0b: ECC CLK enable at operation            |         |            |
|       |                                            |         |            |
|       | 1b: ECC CLK always enable                  |         |            |
+-------+--------------------------------------------+---------+------------+
| 16:0  | Reserved                                   | 00h     | RsvdP      |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCON_FCE_ENB .anchor}FCON_FCE_ENB /FCE Control Register (Offset 08h)

Register Index: 08h \~ 0Bh

Default Value: 0000_0000h

+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------+------------+
| Bit   | Descriptions                                                                                                                                                               | Default    | Attributes |
+:=====:+============================================================================================================================================================================+:==========:+:==========:+
| 31:0  | FnCE0 \~ 31 Enable, IO output is register                                                                                                                                  | 0000_0000h | RW         |
|       |                                                                                                                                                                            |            |            |
|       | 0b: CE = 1                                                                                                                                                                 |            |            |
|       |                                                                                                                                                                            |            |            |
|       | 1b: CE = 0                                                                                                                                                                 |            |            |
|       |                                                                                                                                                                            |            |            |
|       | FnCE control bit, the bit is mapping to FnCE pin one by one, but the mapping will be effect FCON_FCE_SEL\_\*. The value change also can use FCON_FCE_SET and FCON_FCE_CLR. |            |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------+------------+

Register: []{#FCON_FCE_ENB_1 .anchor}FCON_FCE_ENB_1 /FCE Control 1 Register (Offset 0Ch)

Register Index: 0Ch \~ 0Fh

Default Value: 0000_0000h

+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------+------------+
| Bit   | Descriptions                                                                                                                                                               | Default    | Attributes |
+:=====:+============================================================================================================================================================================+:==========:+:==========:+
| 31:0  | FnCE32 \~ 63 Enable, IO output is register                                                                                                                                 | 0000_0000h | RW         |
|       |                                                                                                                                                                            |            |            |
|       | 0b: CE = 1                                                                                                                                                                 |            |            |
|       |                                                                                                                                                                            |            |            |
|       | 1b: CE = 0                                                                                                                                                                 |            |            |
|       |                                                                                                                                                                            |            |            |
|       | FnCE control bit, the bit is mapping to FnCE pin one by one, but the mapping will be effect FCON_FCE_SEL\_\*. The value change also can use FCON_FCE_SET and FCON_FCE_CLR. |            |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------+------------+

Register: []{#FCON_SMP_CFG .anchor}FCON_SMP_CFG / Semaphore Configure Register (Offset 18h)

Register Index: 18h \~ 1Bh

Default Value: 0000_0000h

+-------+--------------------------------------------------+---------+------------+
| Bit   | Descriptions                                     | Default | Attributes |
+:=====:+==================================================+:=======:+:==========:+
| 31:14 | Reserved                                         | 00h     | RsvdP      |
+-------+--------------------------------------------------+---------+------------+
| 13:8  | Current semaphore number                         | 00h     | RO         |
+-------+--------------------------------------------------+---------+------------+
| 7     | Reserved                                         | 00h     | RsvdP      |
+-------+--------------------------------------------------+---------+------------+
| 6:1   | Total semaphore number                           | 00h     | RW         |
|       |                                                  |         |            |
|       | When semaphore number equal 0, MTQ will be abort |         |            |
+-------+--------------------------------------------------+---------+------------+
| 0     | Total semaphore number function enable           | 0b      | RW         |
|       |                                                  |         |            |
|       | 0b: disable                                      |         |            |
|       |                                                  |         |            |
|       | 1b: enable                                       |         |            |
+-------+--------------------------------------------------+---------+------------+

Register: []{#FCON_FORMAT .anchor}FCON_FORMAT /Flash ECC Format Register (Offset 1Ch)

Register Index: 1Ch \~ 1Fh

Default Value: 0000_0000h

+-------+-------------------------------------------------+---------+------------+
| Bit   | Descriptions                                    | Default | Attributes |
+:=====:+=================================================+:=======:+:==========:+
| 31:16 | Reserved                                        | 00h     | RsvdP      |
+-------+-------------------------------------------------+---------+------------+
| 15    | Short length enable                             | 0b      | RW         |
|       |                                                 |         |            |
|       | 0b: disable                                     |         |            |
|       |                                                 |         |            |
|       | 1b: enable, length reference FCON_FORMAT\[8:0\] |         |            |
+-------+-------------------------------------------------+---------+------------+
| 14:11 | Reserved                                        | 00h     | RsvdP      |
+-------+-------------------------------------------------+---------+------------+
| 10:0  | Short length                                    | 00h     | RW         |
|       |                                                 |         |            |
|       | 8h: 8bytes                                      |         |            |
|       |                                                 |         |            |
|       | 10h: 16bytes                                    |         |            |
|       |                                                 |         |            |
|       | 20h: 32bytes                                    |         |            |
|       |                                                 |         |            |
|       | 40h: 64bytes                                    |         |            |
|       |                                                 |         |            |
|       | 80h: 128bytes                                   |         |            |
|       |                                                 |         |            |
|       | 100h: 256bytes                                  |         |            |
|       |                                                 |         |            |
|       | Others : only usage by DMA data                 |         |            |
+-------+-------------------------------------------------+---------+------------+

Register: []{#FCON_IRC_CTL .anchor}FCON_IRC_CTL / IRAM Copy Control Register (Offset 28h)

Register Index: 28h \~ 2Bh

Default Value: 0000_0000h

+-------+--------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                 | Default | Attributes |
+:=====:+==============================================================================================================+:=======:+:==========:+
| 31:8  | Reserved                                                                                                     | 0000h   | RsvdP      |
+-------+--------------------------------------------------------------------------------------------------------------+---------+------------+
| 8     | IRAM initial trigger                                                                                         | 0b      | RW         |
|       |                                                                                                              |         |            |
|       | 0b: idle                                                                                                     |         |            |
|       |                                                                                                              |         |            |
|       | 1b: trigger IRAM initial (clear IRAM value to 0), auto clear when done                                       |         |            |
+-------+--------------------------------------------------------------------------------------------------------------+---------+------------+
| 7     | IRAM copy trigger                                                                                            | 0b      | RW         |
|       |                                                                                                              |         |            |
|       | 0b: idle                                                                                                     |         |            |
|       |                                                                                                              |         |            |
|       | 1b: trigger copy machine, auto clear when done                                                               |         |            |
+-------+--------------------------------------------------------------------------------------------------------------+---------+------------+
| 6:0   | IRAM copy length, unit is MT_TABLE_SIZE( FCON_MT_ADR_BASE\[21:18\])                                          | 00h     | RW         |
|       |                                                                                                              |         |            |
|       | Ex. Set 2 and MT_TABLE_SIZE 56Bytes, it will copy 56 Bytes from FCON_IRC_ADR_0 to FCON_IRC_ADR_1 56\*2 Bytes |         |            |
+-------+--------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_IRC_ADR_0 .anchor}FCON_IRC_ADR_0 / IRAM Copy Address 0 Register (Offset 2Ch)

Register Index: 2Ch \~ 2Fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:16  IRAM copy destination start address             0000h         RW

   15:0   IRAM copy source start address                  0000h         RW
  -----------------------------------------------------------------------------

Register: []{#FCON_CNT_ERR_SET .anchor}FCON_CNT_ERR_SET / Count Error Set Register (Offset 30h)

Register Index: 30h \~ 33h

Default Value: 0000_0000h

+-------+----------------------------------------------------+---------+------------+
| Bit   | Descriptions                                       | Default | Attributes |
+:=====:+====================================================+:=======:+:==========:+
| 31:4  | Reserved                                           | 00h     | RO         |
+-------+----------------------------------------------------+---------+------------+
| 3:0   | CNT_ERR_SEL                                        | 0h      | RW         |
|       |                                                    |         |            |
|       | 0h: select cnt_err_0 =\> count 0 error             |         |            |
|       |                                                    |         |            |
|       | 1h: select cnt_err_1 =\> count 1\~31 error         |         |            |
|       |                                                    |         |            |
|       | 2h: select cnt_err_2 =\> count 32\~63 error        |         |            |
|       |                                                    |         |            |
|       | 3h: select cnt_err_3 =\> count 64\~95 error        |         |            |
|       |                                                    |         |            |
|       | 4h: select cnt_err_4 =\> count 96\~127 error       |         |            |
|       |                                                    |         |            |
|       | 5h: select cnt_err_5 =\> count 128\~159 error      |         |            |
|       |                                                    |         |            |
|       | 6h: select cnt_err_6 =\> count 160\~191 error      |         |            |
|       |                                                    |         |            |
|       | 7h: select cnt_err_7 =\> count 192\~223 error      |         |            |
|       |                                                    |         |            |
|       | 8h: select cnt_err_8 =\> count 226 error           |         |            |
|       |                                                    |         |            |
|       | 9h: select cnt_err_9 =\> count uncorrectable error |         |            |
+-------+----------------------------------------------------+---------+------------+

Register: []{#FCON_CNT_ERR .anchor}FCON_CNT_ERR / Count Error Register (Offset 34h)

Register Index: 34h \~ 37h

Default Value: 0000_0000h

+-------+-----------------------------------------------------+---------+------------+
| Bit   | Descriptions                                        | Default | Attributes |
+:=====:+=====================================================+:=======:+:==========:+
| 31:0  | Read / write will reference FCON_CNT_ERR_SET\[3:0\] | 00h     | RW         |
|       |                                                     |         |            |
|       | 0h: select cnt_err_0 =\> count 0 error              |         |            |
|       |                                                     |         |            |
|       | 1h: select cnt_err_1 =\> count 1\~31 error          |         |            |
|       |                                                     |         |            |
|       | 2h: select cnt_err_2 =\> count 32\~63 error         |         |            |
|       |                                                     |         |            |
|       | 3h: select cnt_err_3 =\> count 64\~95 error         |         |            |
|       |                                                     |         |            |
|       | 4h: select cnt_err_4 =\> count 96\~127 error        |         |            |
|       |                                                     |         |            |
|       | 5h: select cnt_err_5 =\> count 128\~159 error       |         |            |
|       |                                                     |         |            |
|       | 6h: select cnt_err_6 =\> count 160\~191 error       |         |            |
|       |                                                     |         |            |
|       | 7h: select cnt_err_7 =\> count 192\~223 error       |         |            |
|       |                                                     |         |            |
|       | 8h: select cnt_err_8 =\> count 226 error            |         |            |
|       |                                                     |         |            |
|       | 9h: select cnt_err_9 =\> count uncorrectable error  |         |            |
+-------+-----------------------------------------------------+---------+------------+

Register: []{#FCON_FLH_FUNC .anchor}FCON_FLH_FUNC /Flash Function Register (Offset 38h)

Register Index: 38h \~ 3Bh

Default Value: 1CC8_0000h

+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                     | Default | Attributes |
+:=====:+==================================================================================================================+:=======:+:==========:+
| 31    | Enable MT flash clock divider wait successfully change clock.If set 1, HW will start fpu after change clock done | 1       | RW         |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 30    | Auto clear internal flag(uncorrectable,over threshold,crc32,erase frame) of each internal buffer                 | 1       | RW         |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 29    | Internal Buffer ring enable                                                                                      | 0h      | RW         |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 28    | Flash IP internal buffer ring enable, applied in flash read                                                      | 0h      | RsvdP      |
|       |                                                                                                                  |         |            |
|       | 1'b1 : HW sequentially use internal buffer                                                                       |         |            |
|       |                                                                                                                  |         |            |
|       | 1'b0 : each read DMA starts at first internal buffer                                                             |         |            |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27    | Wait additional 64 system cycle to guarantee CLOCK static if MT divides clock.                                   | 0       | RW         |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 26:22 | Reserved                                                                                                         |         | RsvdP      |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 21    | DQS input event mask pre state                                                                                   | 0b      | RW         |
|       |                                                                                                                  |         |            |
|       | 0b: disable                                                                                                      |         |            |
|       |                                                                                                                  |         |            |
|       | 1b: enable                                                                                                       |         |            |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 20    | AXI read enhance enable                                                                                          | 0b      | RW         |
|       |                                                                                                                  |         |            |
|       | 0b: disable                                                                                                      |         |            |
|       |                                                                                                                  |         |            |
|       | 1b: enable                                                                                                       |         |            |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 19    | AXI run robin enable                                                                                             | 1b      | RW         |
|       |                                                                                                                  |         |            |
|       | 0b: disable                                                                                                      |         |            |
|       |                                                                                                                  |         |            |
|       | 1b: enable                                                                                                       |         |            |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 18    | Reserved                                                                                                         | 0b      | RsvdP      |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
|       |                                                                                                                  |         |            |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
|       |                                                                                                                  | 0b      |            |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 15:12 | Reserved                                                                                                         | 0b      | RsvdP      |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 11:10 | set randomize page address region                                                                                | 00b     | RW         |
|       |                                                                                                                  |         |            |
|       | 00b: reference FCTL_FTA_ROW\[8:0\]                                                                               |         |            |
|       |                                                                                                                  |         |            |
|       | 01b: reference FCTL_FTA_ROW\[7:0\]                                                                               |         |            |
|       |                                                                                                                  |         |            |
|       | 10b: reference FCTL_FTA_ROW\[6:0\]                                                                               |         |            |
|       |                                                                                                                  |         |            |
|       | 11b: reference FCTL_FTA_ROW\[5:0\]                                                                               |         |            |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 9:5   | Reserved                                                                                                         | 0b      | RsvdP      |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 4     | Meaning of CE in MT table                                                                                        | 0b      | RsvdP      |
|       |                                                                                                                  |         |            |
|       | 1'b1: the CE number is counted across channel                                                                    |         |            |
|       |                                                                                                                  |         |            |
|       | Ex. The ce of channel 0 are 0,2,4,6                                                                              |         |            |
|       |                                                                                                                  |         |            |
|       | The ce of channel 0 are 1,3,5,7                                                                                  |         |            |
|       |                                                                                                                  |         |            |
|       | 1'b0: Normal                                                                                                     |         |            |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3     | Reserved                                                                                                         | 0b      | RsvdP      |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2     | Signoff 4k mode                                                                                                  | 0b      | RW         |
|       |                                                                                                                  |         |            |
|       | 0b: disable                                                                                                      |         |            |
|       |                                                                                                                  |         |            |
|       | 1b: enable                                                                                                       |         |            |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1     | Reserved                                                                                                         | 0b      | RsvdP      |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0     | Reserved                                                                                                         | 0b      | RsvdP      |
+-------+------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_AXIS_ADR .anchor}FCON_AXIS_ADR / AXI Slave Address (Offset 3Ch)

Register Index: 3Ch \~ 3Fh

Default Value: 1_FFFFh

  -----------------------------------------------------------------------------------------
    Bit   Descriptions                                               Default    Attributes
  ------- --------------------------------------------------------- ---------- ------------
   31:6   End address of IRAM remapping mode by AXI slave using      1_FFFFh        RW

   15:0   Start address of IRAM remapping mode by AXI slave using    1_FFFFh        RW
  -----------------------------------------------------------------------------------------

Register: []{#FCON_MNT_CFG .anchor}FCON_MNT_CFG / MT Monitor Configure (Offset 40h)

Register Index: 40h \~ 43h

Default Value: 0000_0000h

+-------+------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                       | Default | Attributes |
+:=====:+====================================================================================+:=======:+:==========:+
| 31:16 | MDM counter value                                                                  | 00h     | RW         |
+-------+------------------------------------------------------------------------------------+---------+------------+
| 15:1  | MDM step value                                                                     | 00h     | RW         |
|       |                                                                                    |         |            |
|       | When MDM timer equal MDM interval, MDM counter will be increased by MDM step value |         |            |
+-------+------------------------------------------------------------------------------------+---------+------------+
| 0     | MT DMA Monitor function enable                                                     | 0b      | RW         |
|       |                                                                                    |         |            |
|       | MDM timer value will be increased in every system clock domain                     |         |            |
|       |                                                                                    |         |            |
|       | 0b: disable                                                                        |         |            |
|       |                                                                                    |         |            |
|       | 1b: enable                                                                         |         |            |
+-------+------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_SPR_VALID_FUNC .anchor}FCON_SGN_MSK /Signoff Mask Function Register (Offset 44h)

Register Index: 44h \~ 47h

Default Value: 0000_0000h

+-------+---------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                  | Default | Attributes |
+:=====:+===============================================================+:=======:+:==========:+
| 31:16 | Reserved                                                      | 00h     | RsvdP      |
+-------+---------------------------------------------------------------+---------+------------+
| 15:14 | Signoff mask location in spare extend                         | 0h      | RW         |
|       |                                                               |         |            |
|       | Location can extend to 24bytes                                |         |            |
|       |                                                               |         |            |
|       | 00b: byte 0\~7                                                |         |            |
|       |                                                               |         |            |
|       | 01b: byte 8\~15                                               |         |            |
|       |                                                               |         |            |
|       | 10b: byte 16\~24                                              |         |            |
+-------+---------------------------------------------------------------+---------+------------+
| 13    | 0b: bit0\~31                                                  | 0b      | RW         |
|       |                                                               |         |            |
|       | 1b:bit32\~63                                                  |         |            |
+-------+---------------------------------------------------------------+---------+------------+
| 12:8  | Signoff mask location in spare                                | 00h     | RW         |
|       |                                                               |         |            |
|       | This is bit location                                          |         |            |
|       |                                                               |         |            |
|       | Ex. select spare byte 2, bit2 & 3, the value must set 18      |         |            |
+-------+---------------------------------------------------------------+---------+------------+
| 7:2   | Reserved                                                      | 0h      | RsvdP      |
+-------+---------------------------------------------------------------+---------+------------+
| 1     | Signoff mask value                                            | 0b      | RW         |
|       |                                                               |         |            |
|       | 0b: spare value is 0, data is not valid(signoff will be mask) |         |            |
|       |                                                               |         |            |
|       | 1b: spare value is 1, data is not valid(signoff will be mask) |         |            |
+-------+---------------------------------------------------------------+---------+------------+
| 0     | Signoff mask function enable                                  | 0b      | RW         |
|       |                                                               |         |            |
|       | 0b: disable                                                   |         |            |
|       |                                                               |         |            |
|       | 1b: enable                                                    |         |            |
+-------+---------------------------------------------------------------+---------+------------+

Register: []{#FCON_MNT_CNT .anchor}FCON_MNT_CNT / IRAM Counter (Offset 48h)

Register Index: 48h \~ 4Bh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:0  | MDM interval value                         | 00h     | RW         |
|       |                                            |         |            |
|       | How long will MDM counter be increased     |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCON_RTY_CFG .anchor}FCON_RTY_CFG /Retry Configure Register (Offset 4Ch)

Register Index: 4Ch \~ 4Fh

Default Value: 0000_1414h

  ---------------------------------------------------------------------------------
    Bit   Descriptions                                       Default    Attributes
  ------- ------------------------------------------------- ---------- ------------
    31    Reserved                                             00h        RsvdP

    30    Toggle/ONFI dat PST phase count\[4\] for CFG_1        1b          RW

    29    Toggle/ONFI dat PRE phase count\[4\] for CFG_1        0b          RW

   28:27  Toggle cmd/adr PST phase count\[4:3\] for CFG_1      00b          RW

   26:25  Toggle cmd/adr WEN phase count\[4:3\] for CFG_1      00b          RW

    24    Toggle cmd/adr PRE phase count\[4\] for CFG_1         0b          RW

    23    Reserved                                              0b        RsvdP

    22    Toggle/ONFI dat PST phase count\[4\]                  1b          RW

    21    Toggle /ONFI dat PRE phase count\[4\]                 0b          RW

   20:19  Toggle cmd/adr PST phase count\[4:3\]                00b          RW

   18:17  Toggle cmd/adr WEN phase count\[4:3\]                00b          RW

    16    Toggle cmd/adr PRE phase count\[4\]                   0b          RW

   15:13  Reserved                                              0h        RsvdP

   12:8   FPU retry WH count                                   14h          RW

    7:5   Reserved                                              0h        RsvdP

    4:0   FPU retry WP count                                   14h          RW
  ---------------------------------------------------------------------------------

Register: []{#FCON_SPECIAL .anchor}FCON_SPECIAL /Special Function Register (Offset 50h)

Register Index: 50h \~ 53h

Default Value: 0000_0000h

+-------+-----------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                            | Default | Attributes |
+:=====:+=========================================================================================+:=======:+:==========:+
| 31    | Reserved                                                                                | 00h     | RsvdP      |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 30    | Error configuration of parity checks ZRAM.                                              | 0b      | RW         |
|       |                                                                                         |         |            |
|       | 0b: disable error insert                                                                |         |            |
|       |                                                                                         |         |            |
|       | 1b: enable error insert                                                                 |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 29    | MT query flag enable                                                                    | 1h      | RW         |
|       |                                                                                         |         |            |
|       | It is the overall enable bit.                                                           |         |            |
|       |                                                                                         |         |            |
|       | 0b: Query check in MT always disabled                                                   |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 28:26 | Query flag cnt                                                                          | 4h      | RW         |
|       |                                                                                         |         |            |
|       | If qry ack is not responsed less than it, MT arbiter will ignore currect MT.            |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 25:16 | Over ECC error threshold number                                                         | 00h     | RW         |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 15    | PFA_FUNCTION_ENABLE                                                                     | 0b      | RW         |
|       |                                                                                         |         |            |
|       | 0b: FPU CMD can't support poll fail function                                            |         |            |
|       |                                                                                         |         |            |
|       | 1b: FPU CMD can support poll fail function                                              |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 14    | Reserved                                                                                | 0h      | RsvdP      |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 13    | Reserved                                                                                | 0b      | RsvdP      |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 12    | AXIS remapping enable                                                                   | 0b      | RW         |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 11    | Error configuration of parity checks XRAM.                                              | s       | RW         |
|       |                                                                                         |         |            |
|       | 0b: disable error insert                                                                |         |            |
|       |                                                                                         |         |            |
|       | 1b: enable error insert                                                                 |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 10    | Error configuration of parity checks IRAM.                                              | 0b      | RW         |
|       |                                                                                         |         |            |
|       | 0b: disable error insert                                                                |         |            |
|       |                                                                                         |         |            |
|       | 1b: enable error insert                                                                 |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 9     | PAD zero-bit with dummy data in the end of parity                                       | 0b      | RW         |
|       |                                                                                         |         |            |
|       | 0b: disable                                                                             |         |            |
|       |                                                                                         |         |            |
|       | 1b: enable                                                                              |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 8     | MT AUTO POLL RDY                                                                        | 0b      | RW         |
|       |                                                                                         |         |            |
|       | 0b: auto send read status to update internal RBY status                                 |         |            |
|       |                                                                                         |         |            |
|       | 1b: use RDY PIN to update internal RBY status                                           |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+
| 7:0   | MCU wait counter value                                                                  | 00h     | RW         |
|       |                                                                                         |         |            |
|       | For MCU read/write flash register usage, set wait counter value, it depend on flash CLK |         |            |
+-------+-----------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_LGC_TIME_CFG .anchor}FCON_LGC_TIME_CFG /Legacy Timing Configure Register (Offset 64h)

Register Index: 64h \~ 67h

Default Value: 0053_2A2Bh

  ---------------------------------------------------------------------------------
    Bit   Descriptions                                       Default    Attributes
  ------- ------------------------------------------------- ---------- ------------
    31    The high bit for legacy address WEN phase count      00h          RW

    30    The high bit for legacy command WEN phase count      00h          RW

   29:24  Reserved                                             00h        RsvdP

   23:20  Legacy data PST count                                 5h          RW

   19:16  Legacy data PRE count                                 3h          RW

   15:13  legacy address PST count                              1h          RW

   12:11  legacy address WEN phase count                        1h          RW

   10:8   legacy address PRE count                              2h          RW

    7:5   legacy command PST count                              1h          RW

    4:3   legacy command WEN phase count                        1h          RW

    2:0   legacy command PRE count                              3h          RW
  ---------------------------------------------------------------------------------

Register: []{#FCON_TGL_TIME_CFG .anchor}FCON_TGL_TIME_CFG /Toggle Timing Configure Register (Offset 68h)

Register Index: 68h \~ 6Bh

Default Value: A9FF_5525h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:28  toggle data CLH phase count                       Ah          RW

   27:24  toggle data PSTH phase count                      9h          RW

   23:20  toggle data PST phase count                       Fh          RW

   19:16  toggle data PRE phase count                       Fh          RW

   14:12  Toggle adr PST phase count                        5h          RW

   11:8   Toggle adr PRE phase count                        5h          RW

    6:4   Toggle cmd PST phase count                        2h          RW

    3:0   Toggle cmd PRE phase count                        5h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_ONF_TIME_CFG .anchor}FCON_ONF_TIME_CFG /ONFI Timing Configure Register (Offset 6Ch)

Register Index: 6Ch \~ 6Fh

Default Value: 4401_A955h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
    31    Reserved                                          0b        RsvdP

   30:28  toggle adr WEN phase count                        4h          RW

    27    Reserved                                          0b        RsvdP

   26:24  toggle cmd WEN phase count                        4h          RW

   23:20  ONFI tCDQSS phase count                           0h          RW

    19    ONFI tCDQSS enable                                0b          RW

   18:16  ONFI data RTN phase count                         1h          RW

   15:12  ONFI data PST phase count                         Ah          RW

   11:8   ONFI data PRE phase count                         9h          RW

    7:4   ONFI adr PRE phase count                          5h          RW

    3:0   ONFI cmd PRE phase count                          5h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_LGC_TIME_CFG_1 .anchor}FCON_LGC_TIME_CFG_1 /Legacy Timing Configure 1 Register (Offset 70h)

Register Index: 70h \~ 73h

Default Value: 0053_2A2Bh

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  Reserved                                         00h        RsvdP

   23:20  Legacy data PST count                             5h          RW

   19:16  Legacy data PRE count                             3h          RW

   15:13  legacy address PST count                          1h          RW

   12:11  legacy address WEN phase count                    1h          RW

   10:8   legacy address PRE count                          2h          RW

    7:5   legacy command PST count                          1h          RW

    4:3   legacy command WEN phase count                    1h          RW

    2:0   legacy command PRE count                          3h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_TGL_TIME_CFG_1 .anchor}FCON_TGL_TIME_CFG_1 /Toggle Timing Configure 1 Register (Offset 74h)

Register Index: 74h \~ 77h

Default Value: A9FF_5525h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:28  toggle data CLH phase count                       Ah          RW

   27:24  toggle data PSTH phase count                      9h          RW

   23:20  toggle data PST phase count                       Fh          RW

   19:16  toggle data PRE phase count                       Fh          RW

   14:12  Toggle adr PST phase count                        5h          RW

   11:8   Toggle adr PRE phase count                        5h          RW

    6:4   Toggle cmd PST phase count                        2h          RW

    3:0   Toggle cmd PRE phase count                        5h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_ONF_TIME_CFG_1 .anchor}FCON_ONF_TIME_CFG_1 /ONFI Timing Configure 1 Register (Offset 78h)

Register Index: 78h \~ 7Bh

Default Value: 4401_A955h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
    31    Reserved                                          0b        RsvdP

   30:28  toggle adr WEN phase count                        4h          RW

    27    Reserved                                          0b        RsvdP

   26:24  toggle cmd WEN phase count                        4h          RW

   23:20  ONFI tCDQSS phase count                           0h          RW

    19    ONFI tDQSS enable                                 0b          RW

   18:16  ONFI data RTN phase count                         1h          RW

   15:12  ONFI data PST phase count                         Ah          RW

   11:8   ONFI data PRE phase count                         9h          RW

    7:4   ONFI adr PRE phase count                          5h          RW

    3:0   ONFI cmd PRE phase count                          5h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_0 .anchor}FCON_FCE_SEL_0 /FCE Remapping 0 Register (Offset 84h)

Register Index: 84h \~ 87h

Default Value: 0302_0100h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 3                                    3h          RW

   23:16  remapping CE 2                                    2h          RW

   15:8   remapping CE 1                                    1h          RW

    7:0   remapping CE 0                                    0h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_1 .anchor}FCON_FCE_SEL_1 /FCE Remapping 1 Register (Offset 88h)

Register Index: 88h \~ 8Bh

Default Value: 0706_0504h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 7                                    7h          RW

   23:16  remapping CE 6                                    6h          RW

   15:8   remapping CE 5                                    5h          RW

    7:0   remapping CE 4                                    4h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_2 .anchor}FCON_FCE_SEL_2/FCE Remapping 2 Register (Offset 8Ch)

Register Index: 8Ch \~ 8Fh

Default Value: 0B0A_0908h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 11                                   Bh          RW

   23:16  remapping CE 10                                   Ah          RW

   15:8   remapping CE 9                                    9h          RW

    7:0   remapping CE 8                                    8h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_3 .anchor}FCON_FCE_SEL_3 /FCE Remapping 3 Register (Offset 90h)

Register Index: 90h \~ 93h

Default Value: 0F0E_0D0Ch

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 15                                   Fh          RW

   23:16  remapping CE 14                                   Eh          RW

   15:8   remapping CE 13                                   Dh          RW

    7:0   remapping CE 12                                   Ch          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_4 .anchor}FCON_FCE_SEL_4 /FCE Remapping 4 Register (Offset 94h)

Register Index: 94h \~ 97h

Default Value: 1312_1110h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 19                                  13h          RW

   23:16  remapping CE 18                                  12h          RW

   15:8   remapping CE 17                                  11h          RW

    7:0   remapping CE 16                                  10h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_5 .anchor}FCON_FCE_SEL_5 /FCE Remapping 5 Register (Offset 98h)

Register Index: 98h \~ 9Bh

Default Value: 1716_1514h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 23                                  17h          RW

   23:16  remapping CE 22                                  16h          RW

   15:8   remapping CE 21                                  15h          RW

    7:0   remapping CE 20                                  14h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_6 .anchor}FCON_FCE_SEL_6 /FCE Remapping 6 Register (Offset 9Ch)

Register Index: 9Ch \~ 9Fh

Default Value: 1B1A_1918h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 27                                  1Bh          RW

   23:16  remapping CE 26                                  1Ah          RW

   15:8   remapping CE 25                                  19h          RW

    7:0   remapping CE 24                                  18h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_7 .anchor}FCON_FCE_SEL_7 /FCE Remapping 7 Register (Offset A0h)

Register Index: A0h \~ A3h

Default Value:1F1E \_1D1Ch

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 31                                  1Fh          RW

   23:16  remapping CE 30                                  1Eh          RW

   15:8   remapping CE 29                                  1Dh          RW

    7:0   remapping CE 28                                  1Ch          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_8 .anchor}FCON_FCE_SEL_8 /FCE Remapping 8 Register (Offset A4h)

Register Index: A4h \~ A7h

Default Value: 2322_2120h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 35                                  23h          RW

   23:16  remapping CE 34                                  22h          RW

   15:8   remapping CE 33                                  21h          RW

    7:0   remapping CE 32                                  20h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_9 .anchor}FCON_FCE_SEL_9 /FCE Remapping 9 Register (Offset A8h)

Register Index: A8h \~ ABh

Default Value: 2726_2524h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 39                                  27h          RW

   23:16  remapping CE 38                                  26h          RW

   15:8   remapping CE 37                                  25h          RW

    7:0   remapping CE 36                                  24h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_10 .anchor}FCON_FCE_SEL_10 /FCE Remapping 10 Register (Offset ACh)

Register Index: ACh \~ AFh

Default Value: 2b2a_2928h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 43                                  2Bh          RW

   23:16  remapping CE 42                                  2Ah          RW

   15:8   remapping CE 41                                  29h          RW

    7:0   remapping CE 40                                  28h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_11 .anchor}FCON_FCE_SEL_11 /FCE Remapping 11 Register (Offset B0h)

Register Index: B0h \~ B3h

Default Value: 2F2E_2D2Ch

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 47                                  2Fh          RW

   23:16  remapping CE 46                                  2Eh          RW

   15:8   remapping CE 45                                  2Dh          RW

    7:0   remapping CE 44                                  2Ch          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_12 .anchor}FCON_FCE_SEL_12 /FCE Remapping 12 Register (Offset B4h)

Register Index: B4h \~ B7h

Default Value: 3332_3130h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 51                                  33h          RW

   23:16  remapping CE 50                                  32h          RW

   15:8   remapping CE 49                                  31h          RW

    7:0   remapping CE 48                                  30h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_13 .anchor}FCON_FCE_SEL_13 /FCE Remapping 13 Register (Offset B8h)

Register Index: B8h \~ BBh

Default Value: 3736_3534h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 55                                  37h          RW

   23:16  remapping CE 54                                  36h          RW

   15:8   remapping CE 53                                  35h          RW

    7:0   remapping CE 52                                  34h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_14 .anchor}FCON_FCE_SEL_14 /FCE Remapping 14 Register (Offset BCh)

Register Index: BCh \~BFh

Default Value: 3B3A_3938h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 59                                  3Bh          RW

   23:16  remapping CE 58                                  3Ah          RW

   15:8   remapping CE 57                                  39h          RW

    7:0   remapping CE 56                                  38h          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_FCE_SEL_15 .anchor}FCON_FCE_SEL_15 /FCE Remapping 15 Register (Offset C0h)

Register Index: C0h \~ C3h

Default Value: 3F3E_3D3Ch

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  remapping CE 63                                  3Fh          RW

   23:16  remapping CE 62                                  3Eh          RW

   15:8   remapping CE 61                                  3Dh          RW

    7:0   remapping CE 60                                  3Ch          RW
  -----------------------------------------------------------------------------

Register: []{#FCON_D2I_IRADR .anchor}FCON_D2I_IRADR /D2I IRAM Address Register (Offset C4h)

Register Index: C4h \~ C7h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DRAM/IRAM copy IRAM address                      00h          RW

  -----------------------------------------------------------------------------

Register: []{#FCON_D2I_DRADR .anchor}FCON_D2I_DRADR /D2I Dram Address Register (Offset C8h)

Register Index: C8h \~ CBh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DRAM/IRAM copy DRAM address                      00h          RW

  -----------------------------------------------------------------------------

Register: []{#FCON_D2I_CTL .anchor}FCON_D2I_CTL /D2I Control Register (Offset CCh)

Register Index: CCh \~ CFh

Default Value: 0000_0004h

+-------+---------------------------------------------------+---------+------------+
| Bit   | Descriptions                                      | Default | Attributes |
+:=====:+===================================================+:=======:+:==========:+
| 31:6  | Reserved                                          | 0h      | RsvdP      |
+-------+---------------------------------------------------+---------+------------+
| 5     | Reserved                                          | 0b      | RsvdP      |
+-------+---------------------------------------------------+---------+------------+
| 4     | Reserved                                          | 0b      | RsvdP      |
+-------+---------------------------------------------------+---------+------------+
| 3     | 128 bytes D2I enable                              | 0b      | RW         |
|       |                                                   |         |            |
|       | 0b: disable                                       |         |            |
|       |                                                   |         |            |
|       | 1b: enable 128Bytes \<-\>128Bytes,                |         |            |
|       |                                                   |         |            |
|       | need enable direct mode FCON_D2I_CTL\[2\] as 1'b1 |         |            |
+-------+---------------------------------------------------+---------+------------+
| 2     | Direct mode                                       | 1b      | RW         |
|       |                                                   |         |            |
|       | 0b: 512Bytes \<-\>2kBytes                         |         |            |
|       |                                                   |         |            |
|       | 1b: enable 512Bytes \<-\> 512Bytes mode           |         |            |
+-------+---------------------------------------------------+---------+------------+
| 1     | DRAM/IRAM copy                                    | 0b      | RW         |
|       |                                                   |         |            |
|       | 0b: IRAM to DRAM                                  |         |            |
|       |                                                   |         |            |
|       | 1b: DRAM to IRAM                                  |         |            |
+-------+---------------------------------------------------+---------+------------+
| 0     | DRAM/IRAM copy trigger bit, auto clear when done  | 0b      | RW         |
+-------+---------------------------------------------------+---------+------------+

Register: []{#FCON_CRC_EN .anchor}FCON_CRC_EN /CRC Enable Register (Offset D0h)

Register Index: D0h \~ D3h

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:2  | Reserved                                   | 00h     | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 1     | CRC-16 enable                              | 1b      | RW         |
|       |                                            |         |            |
|       | 0b: disable CRC-16                         |         |            |
|       |                                            |         |            |
|       | 1b: enable CRC-16                          |         |            |
+-------+--------------------------------------------+---------+------------+
| 0     | CRC-32 enable                              | 1b      | RW         |
|       |                                            |         |            |
|       | 0b: disable CRC-32                         |         |            |
|       |                                            |         |            |
|       | 1b: enable CRC-32                          |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCON_CRC_FORCE .anchor}FCON_CRC_FORCE /CRC Force Register (Offset D4h)

Register Index: D4h \~ D7h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   Reserved                                         00h        RsvdP

  -----------------------------------------------------------------------------

Register: []{#FCON_TMR_CTL .anchor}FCON_TMR_CTL / Timer Control Register (Offset D8h)

Register Index: D8h \~ DBh

Default Value: 00FF_FFFFh

+-------+--------------------------------------------+----------+------------+
| Bit   | Descriptions                               | Default  | Attributes |
+:=====:+============================================+:========:+:==========:+
| 31:26 | Reserved                                   | 00h      | RsvdP      |
+-------+--------------------------------------------+----------+------------+
| 25    | Timer CPU interrupt function enable        | 0b       | RW         |
|       |                                            |          |            |
|       | 0b: disable                                |          |            |
|       |                                            |          |            |
|       | 1b: enable                                 |          |            |
+-------+--------------------------------------------+----------+------------+
| 24    | Timer interrupt function enable            | 0b       | RW         |
|       |                                            |          |            |
|       | 0b: disable                                |          |            |
|       |                                            |          |            |
|       | 1b: enable                                 |          |            |
+-------+--------------------------------------------+----------+------------+
| 23:0  | Timer counter length                       | FF_FFFFh | RW         |
+-------+--------------------------------------------+----------+------------+

Register: []{#FCON_ALU_BAS .anchor}FCON_ALU_BAS /Spare Address Register (Offset E0h)

Register Index: E0h \~ E3h

Default Value: 0000_0000h

+-------+--------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                 | Default | Attributes |
+:=====:+==============================================================+:=======:+:==========:+
| 31:0  | ALU address base                                             | 00h     | RW         |
|       |                                                              |         |            |
|       | indicate IRAM location, unit is Bytes, but need 8Bytes align |         |            |
+-------+--------------------------------------------------------------+---------+------------+

Register: []{#FCON_ROT_LEN .anchor}FCON_MGM_INF / MT Group Monitor Register (Offset E4h)

Register Index: E4h \~ E7h

Default Value: 0000_0000h

+-------+----------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                             | Default | Attributes |
+:=====:+==========================================================+:=======:+:==========:+
| 31:0  | MT group information 0\~35 select by FCON_MGM_SEL\[2:0\] | 00h     | RO         |
|       |                                                          |         |            |
|       | Each 4bits represent one group information               |         |            |
+-------+----------------------------------------------------------+---------+------------+

Register: []{#FCON_MT_STS .anchor}FCON_MT_STS/ MT Status Register (Offset E8h)

Register Index: E8h \~ EBh

Default Value: 0000_0000h

+-------+--------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                         | Default | Attributes |
+:=====:+======================================================================================+:=======:+:==========:+
| 31:16 | Read bit-N                                                                           | 0b      | RO         |
|       |                                                                                      |         |            |
|       | 0b : queue N-16 idle                                                                 |         |            |
|       |                                                                                      |         |            |
|       | 1b : queue N-16 busy                                                                 |         |            |
|       |                                                                                      |         |            |
|       | Ex. Read bit 17, the value means status of queue 1                                   |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+
| 15:0  | Write bit-N                                                                          | 0b      | RW         |
|       |                                                                                      |         |            |
|       | Clear queue N, need write 1 then write 0 manual                                      |         |            |
|       |                                                                                      |         |            |
|       | Read bit-N                                                                           |         |            |
|       |                                                                                      |         |            |
|       | 0b: queue N can push task event                                                      |         |            |
|       |                                                                                      |         |            |
|       | 1b: queue N full                                                                     |         |            |
|       |                                                                                      |         |            |
|       | MTP cannot push directly and FCON_MT_PTR_DIF should be checked how much MTP in queue |         |            |
|       |                                                                                      |         |            |
|       | Ex. Write bit 4=1, clear queue 4                                                     |         |            |
|       |                                                                                      |         |            |
|       | Read bit 5, the value means status of queue 5                                        |         |            |
+-------+--------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_MT_CFG .anchor}FCON_MT_CFG / MT Config Register (Offset ECh)

Register Index: ECh \~ EFh

Default Value: 0000_0000h

+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                  | Default | Attributes |
+:=====:+===============================================================================================+:=======:+:==========:+
|       |                                                                                               |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 31    | MTP group priority definition                                                                 | 0       | RW         |
|       |                                                                                               |         |            |
|       | 0b: group 2, low priority                                                                     |         |            |
|       |                                                                                               |         |            |
|       | 1b: group 1, high priority                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 30    | RS_PROG_PARITY                                                                                | 0       | RW         |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 29    | RS_OTFENC_EN                                                                                  | 0       | RW         |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 28:24 | RS_TAG                                                                                        | 00h     | RW         |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 23    | ReuseMT Disable                                                                               | 0       | RW         |
|       |                                                                                               |         |            |
|       |    If the MT index is designed for reused.                                                    |         |            |
|       |                                                                                               |         |            |
|       |       First trigger: set 1'b0                                                                 |         |            |
|       |                                                                                               |         |            |
|       |       Last trigger: set 1'b1                                                                  |         |            |
|       |                                                                                               |         |            |
|       |   If the MT index is not designed for reused                                                  |         |            |
|       |                                                                                               |         |            |
|       |      Set 1'b1                                                                                 |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 22    | ReuseMT auto trigger                                                                          | 0       | RW         |
|       |                                                                                               |         |            |
|       | HW help to auto trigger 2 reuseMT. If this bit set, then ignore ReuseMT parameter select bit. |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 21    | MT Priority lock                                                                              | 0       | RW         |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 20    | Force empty                                                                                   | 0       | RW         |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 19:16 | Queue index                                                                                   | 0000b   | RW         |
|       |                                                                                               |         |            |
|       | 'd0: queue0 of ch0 'd1:queue0 of ch1                                                          |         |            |
|       |                                                                                               |         |            |
|       | 'd2: queue1 of ch0 'd3:queue1 of ch1                                                          |         |            |
|       |                                                                                               |         |            |
|       | ..... ....                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 15:13 | Reserved                                                                                      | 0       | RsvdP      |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 12    | MTQ delay function enable                                                                     | 0b      | RW         |
|       |                                                                                               |         |            |
|       | 0b: disable                                                                                   |         |            |
|       |                                                                                               |         |            |
|       | 1b: enable                                                                                    |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 11:9  | Reserved                                                                                      | 0       | RsvdP      |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+
| 8:0   | MT index                                                                                      | 000h    | RW         |
|       |                                                                                               |         |            |
|       | Ex. If the target MT table is at 0x1120 and FCON_MT_ADR_BASE\[15:0\]=0x1000,                  |         |            |
|       |                                                                                               |         |            |
|       | > [FCON_MT_ADR_BASE](#FCON_MT_ADR_BASE)\[21:18\] = 0x9                                        |         |            |
|       |                                                                                               |         |            |
|       | FW sets 0x4 to trigger this MT                                                                |         |            |
+-------+-----------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_ADR_GEN .anchor}FCON_ADR_GEN /Address Generate Setting Register (Offset F0h)

Register Index: F0h \~ F3h

Default Value: 0100_F000h

+-------+-----------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                              | Default | Attributes |
+:=====:+===========================================================+:=======:+:==========:+
| 31:24 | Reserved                                                  | 0b      | RsvdP      |
+-------+-----------------------------------------------------------+---------+------------+
| 23    | ADR_GEN TLC mode enable                                   | 0b      | RW         |
|       |                                                           |         |            |
|       | Rule : (FTA\[23:16\]\*3+TLC_PTR)%256                      |         |            |
+-------+-----------------------------------------------------------+---------+------------+
| 22:17 | TLC_PTR_LOC                                               | 00h     | RW         |
|       |                                                           |         |            |
|       | Location for catch TLC_PTR in the FSA                     |         |            |
+-------+-----------------------------------------------------------+---------+------------+
| 16:12 | Frame pointer catch mask                                  | 1Fh     | RW         |
+-------+-----------------------------------------------------------+---------+------------+
| 11    | FSA catch mode                                            | 0b      | RW         |
|       |                                                           |         |            |
|       | 0b: FSA = \[39:0\]                                        |         |            |
|       |                                                           |         |            |
|       | 1b: FSA = {\[47:32\],\[23:0\]}                            |         |            |
+-------+-----------------------------------------------------------+---------+------------+
| 10    | FTA = FSA after address generate                          | 0b      | RW         |
+-------+-----------------------------------------------------------+---------+------------+
| 9     | column address auto generation                            | 0b      | RW         |
|       |                                                           |         |            |
|       | it will use frame pointer of FSA to create column address |         |            |
+-------+-----------------------------------------------------------+---------+------------+
| 8     | Frame pointer update enable                               | 0b      | RW         |
|       |                                                           |         |            |
|       | 0b: no update                                             |         |            |
|       |                                                           |         |            |
|       | 1b: auto update frame pointer after address generate      |         |            |
+-------+-----------------------------------------------------------+---------+------------+
| 7     | Frame pointer update next when DMA done                   | 0b      | RW         |
|       |                                                           |         |            |
|       | 0b: no update                                             |         |            |
|       |                                                           |         |            |
|       | 1b: auto update next frame pointer after DMA done         |         |            |
+-------+-----------------------------------------------------------+---------+------------+
| 6     | Reserved                                                  | 0b      | RsvdP      |
+-------+-----------------------------------------------------------+---------+------------+
| 5:0   | Frame pointer catch at FSA for column address generate    | 00h     | RW         |
+-------+-----------------------------------------------------------+---------+------------+

Register: []{#FCON_ADR_GEN_STOP .anchor}FCON_ADR_GEN_STOP /Address Generate Stop Register (Offset F4h)

Register Index: F4h \~ F7h

Default Value: 0000_0000h

+-------+------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                               | Default | Attributes |
+:=====:+============================================================+:=======:+:==========:+
| 31:16 | Reserved                                                   | 0b      | RsvdP      |
+-------+------------------------------------------------------------+---------+------------+
| 15    | Flash address ALU stop check function                      | 0b      | RW         |
|       |                                                            |         |            |
|       | 0b: disable                                                |         |            |
|       |                                                            |         |            |
|       | 1b: enable                                                 |         |            |
+-------+------------------------------------------------------------+---------+------------+
| 14    | Flash address ALU stop check data                          | 0b      | RW         |
+-------+------------------------------------------------------------+---------+------------+
| 13:8  | Flash address ALU stop compare address, source bit address | 00h     | RW         |
+-------+------------------------------------------------------------+---------+------------+
| 7:3   | Reserved                                                   | 00h     | RsvdP      |
+-------+------------------------------------------------------------+---------+------------+
| 2:0   | Flash address ALU stop pointer N+1                         | 0b      | RW         |
+-------+------------------------------------------------------------+---------+------------+

Register: []{#FCON_FCE_SET .anchor}FCON_FCE_SET /Set CE by Number Register (Offset F8h)

Register Index: F8h \~ FBh

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                    | Default | Attributes |
+:=====:+=================================================================================================================================================+:=======:+:==========:+
| 31:6  | Reserved                                                                                                                                        | 00h     | RsvdP      |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 5     | Set 1 to set CE63\~32                                                                                                                           | 0b      | WO         |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 4:0   | Write number to set CE                                                                                                                          | 00h     | WO         |
|       |                                                                                                                                                 |         |            |
|       | this register is not exist, it can write number to set FCON_FCE_ENB of number, Ex. write 23 in this register, FCON_FCE_ENB\[23\] will be set 1. |         |            |
+-------+-------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_FCE_CLR .anchor}FCON_FCE_CLR /Clear CE by Number Register (Offset FCh)

Register Index: FCh \~ FFh

Default Value: 0000_0000h

+-------+---------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                      | Default | Attributes |
+:=====:+===================================================================================================================================================+:=======:+:==========:+
| 31:6  | Reserved                                                                                                                                          | 00h     | RsvdP      |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 5     | Set 1 to clear CE63\~32                                                                                                                           | 0b      | WO         |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 4:0   | Write number to clear CE                                                                                                                          | 00h     | WO         |
|       |                                                                                                                                                   |         |            |
|       | this register is not exist, it can write number to set FCON_FCE_ENB of number, Ex. write 23 in this register, FCON_FCE_ENB\[23\] will be clear 0. |         |            |
+-------+---------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_ROT_SAT .anchor}FCON_MGM_SET / MT Group Monitor Set Register (Offset 100h)

Register Index: 100h \~ 103h

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:15 | Reserved                                   | 00h     | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 14    | MT group information clear                 | 0b      | RW         |
|       |                                            |         |            |
|       | 0b: disable                                |         |            |
|       |                                            |         |            |
|       | 1b: enable                                 |         |            |
+-------+--------------------------------------------+---------+------------+
| 13:8  | MT group information select for clear      | 0h      | RW         |
|       |                                            |         |            |
|       | 0h: clear group 0                          |         |            |
|       |                                            |         |            |
|       | 1h: clear group 1                          |         |            |
|       |                                            |         |            |
|       | ...                                        |         |            |
|       |                                            |         |            |
|       | 23h: clear group 35                        |         |            |
+-------+--------------------------------------------+---------+------------+
| 7:3   | Reserved                                   | 0h      | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 2:0   | MT group information select                | 0h      | RW         |
|       |                                            |         |            |
|       | 0h: select group 0\~7                      |         |            |
|       |                                            |         |            |
|       | 1h: select group 8\~15                     |         |            |
|       |                                            |         |            |
|       | 2h: select group 16\~23                    |         |            |
|       |                                            |         |            |
|       | 3h: select group 24\~31                    |         |            |
|       |                                            |         |            |
|       | 4h: select group 32\~35                    |         |            |
+-------+--------------------------------------------+---------+------------+

~~Register: FCON_AXIS_ADR_1 / AXI Slave Address 1 (Offset 104h)~~

~~Register Index: 104h \~ 107h~~

~~Default Value: 1_FFFFh~~

  -----------------------------------------------------------------------------------------------
   ~~Bit~~   ~~Descriptions~~                                       ~~Default~~   ~~Attributes~~
  ---------- ----------------------------------------------------- ------------- ----------------
   ~~31:0~~  ~~End address of IRAM remapping mode by AXI slave~~    ~~1_FFFFh~~       ~~RW~~

  -----------------------------------------------------------------------------------------------

~~Register:~~ []{#FCON_IRC_ADR_1 .anchor}~~FCON_IRC_ADR_1 / IRAM Copy Address 1 Register (Offset 108h)~~

~~Register Index: 108h \~ 10Bh~~

~~Default Value: 0000_0000h~~

  ---------------------------------------------------------------------------------------
   ~~Bit~~   ~~Descriptions~~                               ~~Default~~   ~~Attributes~~
  ---------- --------------------------------------------- ------------- ----------------
   ~~31:0~~  ~~IRAM copy destination start address~~         ~~0000h~~        ~~RW~~

  ---------------------------------------------------------------------------------------

Register: []{#FCON_FW_RESET .anchor}FCON_FW_RESET / FW Reset Register (Offset 10Ch)

Register Index: 10Ch \~ 10Fh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31    | ZIP reset                                  | 0h      | RW         |
|       |                                            |         |            |
|       | 0b: disable                                |         |            |
|       |                                            |         |            |
|       | 1b: enable                                 |         |            |
+-------+--------------------------------------------+---------+------------+
| 30    | RS reset                                   |         | RW         |
|       |                                            |         |            |
|       | 0b: disable                                |         |            |
|       |                                            |         |            |
|       | 1b: enable                                 |         |            |
+-------+--------------------------------------------+---------+------------+
| 29:24 | Reserved                                   | 3Fh     | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 23:16 | System clock domain reset                  | FFh     | RW         |
|       |                                            |         |            |
|       | 0b: disable                                |         |            |
|       |                                            |         |            |
|       | 1b: enable                                 |         |            |
+-------+--------------------------------------------+---------+------------+
| 15:8  | ECC clock domain reset                     | FFh     | RW         |
|       |                                            |         |            |
|       | 0b: disable                                |         |            |
|       |                                            |         |            |
|       | 1b: enable                                 |         |            |
+-------+--------------------------------------------+---------+------------+
| 7:0   | Flash clock domain reset                   | FFh     | RW         |
|       |                                            |         |            |
|       | 0b: disable                                |         |            |
|       |                                            |         |            |
|       | 1b: enable                                 |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCON_LDPC_CFG .anchor}FCON_LDPC_CFG /LDPC_CFG Register (Offset 110h)

Register Index: 110h \~ 113h

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:29 | Reserved                                   | 0h      | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 28    | Auto update LLR after correct done         | 0h      | RW         |
+-------+--------------------------------------------+---------+------------+
| 27    | LDPC Correct DIS_ENABLE                    | 1b      | Rw         |
+-------+--------------------------------------------+---------+------------+
| 26    | DSP Enable                                 | 1b      | rw         |
+-------+--------------------------------------------+---------+------------+
| 25:24 | page selection                             | 0h      | RW         |
|       |                                            |         |            |
|       | 0: Low page                                |         |            |
|       |                                            |         |            |
|       | 1: Middle page                             |         |            |
|       |                                            |         |            |
|       | 2: Upper page                              |         |            |
+-------+--------------------------------------------+---------+------------+
| 23    | Reserved                                   |         | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 22:16 | SB Maximum iteration number                | 3Ch     | RW         |
+-------+--------------------------------------------+---------+------------+
| 15    | Reserved                                   |         | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 14:8  | Maximum iteration number                   | 64h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 7     | Decoding parameter                         | 0h      | RW         |
+-------+--------------------------------------------+---------+------------+
| 6:4   | LDPC decode mode                           | 00h     | RW         |
|       |                                            |         |            |
|       | 0: HB                                      |         |            |
|       |                                            |         |            |
|       | 1: HB+SB1                                  |         |            |
|       |                                            |         |            |
|       | 2: HB+SB1+SB2                              |         |            |
|       |                                            |         |            |
|       | 3: HB+SB1+SB2+SB3                          |         |            |
|       |                                            |         |            |
|       | 4: HB+SB1+SB2+SB3+SB4                      |         |            |
+-------+--------------------------------------------+---------+------------+
| 3:1   | LDPC MODE                                  | 0h      | RW         |
|       |                                            |         |            |
|       | Based on nand                              |         |            |
+-------+--------------------------------------------+---------+------------+
| 0     | LDPC correct circuit FW clear              | 1h      | RW         |
|       |                                            |         |            |
|       | Write 0 clear, auto set 1                  |         |            |
+-------+--------------------------------------------+---------+------------+

Register:[]{#FCON_LDPC_BFC_SET .anchor} FCON_LDPC_BFC_SET / LDPC Bit-Flip Cnt Set Register (Offset 114h)

Register Index: 114h \~ 117h

Default Value: 0000_0000h

+-------+-------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                          | Default | Attributes |
+:=====:+=======================================================+:=======:+:==========:+
| 31:20 | Reserved                                              | 00h     | RsvdP      |
+-------+-------------------------------------------------------+---------+------------+
| 19    | LDPC Correct converge                                 | 0b      | RO         |
|       |                                                       |         |            |
|       | 1 : Converge                                          |         |            |
|       |                                                       |         |            |
|       | 0 : not converge. HW will not correct this ECC frame. |         |            |
+-------+-------------------------------------------------------+---------+------------+
| 18:4  | Over bit-flip cnt threshold number                    | 00h     | RW         |
+-------+-------------------------------------------------------+---------+------------+
| 3:0   | CNT_BFC_SEL                                           | 0h      | RW         |
|       |                                                       |         |            |
|       | 0h: select cnt_bfc_0 =\> count 0 bfc                  |         |            |
|       |                                                       |         |            |
|       | 1h: select cnt_bfc_1 =\> count 1\~15 bfc              |         |            |
|       |                                                       |         |            |
|       | 2h: select cnt_bfc_2 =\> count 16\~31 bfc             |         |            |
|       |                                                       |         |            |
|       | 3h: select cnt_bfc_3 =\> count 32\~47 bfc             |         |            |
|       |                                                       |         |            |
|       | 4h: select cnt_bfc_4 =\> count 48\~63 bfc             |         |            |
|       |                                                       |         |            |
|       | 5h: select cnt_bfc_5 =\> count 64\~79 bfc             |         |            |
|       |                                                       |         |            |
|       | 6h: select cnt_bfc_6 =\> count 80\~95 bfc             |         |            |
|       |                                                       |         |            |
|       | 7h: select cnt_bfc_7 =\> count 96\~111 bfc            |         |            |
|       |                                                       |         |            |
|       | 8h: select cnt_bfc_8 =\> count 112\~120 bfc           |         |            |
|       |                                                       |         |            |
|       | 9h: select cnt_bfc_9 =\> count uncorrectable bfc      |         |            |
+-------+-------------------------------------------------------+---------+------------+

Register: FCON_LDPC_BFC / Bit-Flip Cnt Register (Offset 118h)

Register Index: 118h \~ 11Bh

Default Value: 0000_0000h

+-------+---------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                            | Default | Attributes |
+:=====:+=========================================================+:=======:+:==========:+
| 31:0  | Read / write will reference FCON\_ LDPC_BFC_SET \[3:0\] | 00h     | RO         |
|       |                                                         |         |            |
|       | 0h: select cnt_bfc_0 =\> count 0 bfc                    |         |            |
|       |                                                         |         |            |
|       | 1h: select cnt_bfc_1 =\> count 1\~15 bfc                |         |            |
|       |                                                         |         |            |
|       | 2h: select cnt_bfc_2 =\> count 16\~31 bfc               |         |            |
|       |                                                         |         |            |
|       | 3h: select cnt_bfc_3 =\> count 32\~47 bfc               |         |            |
|       |                                                         |         |            |
|       | 4h: select cnt_bfc_4 =\> count 48\~63 bfc               |         |            |
|       |                                                         |         |            |
|       | 5h: select cnt_bfc_5 =\> count 64\~79 bfc               |         |            |
|       |                                                         |         |            |
|       | 6h: select cnt_bfc_6 =\> count 80\~95 bfc               |         |            |
|       |                                                         |         |            |
|       | 7h: select cnt_bfc_7 =\> count 96\~111 bfc              |         |            |
|       |                                                         |         |            |
|       | 8h: select cnt_bfc_8 =\> count 112\~120 bfc             |         |            |
|       |                                                         |         |            |
|       | 9h: select cnt_bfc_9 =\> count uncorrectable bfc        |         |            |
+-------+---------------------------------------------------------+---------+------------+

Register: []{#FCON_PAGE_CFG .anchor}FCON_PAGE_CFG Register (Offset 11Ch)

Register Index: 11Ch \~ 11Fh

Default Value: 0000_0000h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+:==========:+
| 31:2  | Reserved                                   | 0h      | RsvdP      |
+-------+--------------------------------------------+---------+------------+
| 1:0   | physical page size                         | 0h      | RW         |
|       |                                            |         |            |
|       | 2'b00 : 4 KB page                          |         |            |
|       |                                            |         |            |
|       | 2'b01 : 4 KB page                          |         |            |
|       |                                            |         |            |
|       | 2'b10 : 8 KB page                          |         |            |
|       |                                            |         |            |
|       | 2'b11 : 16 KB page                         |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#FCON_MT_ADR_BASE .anchor}FCON_MT_ADR_BASE Register (Offset 120h)

Register Index: 120h \~ 123h

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                              | Default | Attributes |
+:=====:+===========================================================================================+:=======:+:==========:+
| 31:23 | Reserved                                                                                  | 0h      | RsvdP      |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 22    | MT global trigger enable                                                                  | 1b      | RW         |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 21:18 | MT table size(unit is 8byte)                                                              | 8h      | RsvdP      |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 17    | Int_index_mode                                                                            | 0h      | RW         |
|       |                                                                                           |         |            |
|       | 1 =\> index mode, system push INT_VCT into system queue according to the index in INT_VCT |         |            |
|       |                                                                                           |         |            |
|       | 0 =\> not index mode                                                                      |         |            |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 16    | Reserved                                                                                  | 0h      | RsvdP      |
+-------+-------------------------------------------------------------------------------------------+---------+------------+
| 15:0  | MT base address of IRAM                                                                   | 00h     | RW         |
+-------+-------------------------------------------------------------------------------------------+---------+------------+

Register: []{#FCON_SYS_INTQ_BASE .anchor}FCON_SYS_INTQ_BASE Register (Offset 124h)

Register Index: 124h \~ 127

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   Interrupt queue base address of system           00h          RW

  -----------------------------------------------------------------------------

Register: []{#FCON_FLHCLK_SPLITE_EN .anchor}FCON\_ FLHCLK_SPLITE_EN Register (Offset 128h)

Register Index: 128h \~ 12B

Default Value: 0000_0000h

+-------+-------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                      | Default | Attributes |
+:=====:+===================================================================+:=======:+:==========:+
| 31:1  | Reserved                                                          | 0000h   | RsvdP      |
+-------+-------------------------------------------------------------------+---------+------------+
| 0     | Splite read/write FLH_CLK enable                                  | 1h      | RW         |
|       |                                                                   |         |            |
|       | System provides 2 FLH_CLK source such as FLH_CLK_W and FLH_CLK_R. |         |            |
|       |                                                                   |         |            |
|       | 0: FLH_CLK always refer to FLH_CLK_R.                             |         |            |
|       |                                                                   |         |            |
|       | 1: FLH_CLK switch to FLH_CLK_R when read DMA.                     |         |            |
|       |                                                                   |         |            |
|       |   FLH_CLK switch to FLH_CLK_W when write DMA                      |         |            |
+-------+-------------------------------------------------------------------+---------+------------+

Register: []{#FCON_MTQ_INF_0 .anchor}FCON_MTQ_INF_0 / MT Queue Information 0 Register (Offset 12Ch)

Register Index: 12Ch \~ 12Fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  Global MT queue 3 non-execute event number       00h          RO

   23:16  Global MT queue 2 non-execute event number       00h          RO

   15:8   Global MT queue 1 non-execute event number       00h          RO

    7:0   Global MT queue 0 non-execute event number       00h          RO
  -----------------------------------------------------------------------------

Register: []{#FCON_MTQ_INF_1 .anchor}FCON_MTQ_INF_1/ MT Queue Information 1 Register (Offset 130h)

Register Index: 130h \~ 133h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  Global MT queue 7 non-execute event number       00h          RO

   23:16  Global MT queue 6 non-execute event number       00h          RO

   15:8   Global MT queue 5 non-execute event number       00h          RO

    7:0   Global MT queue 4 non-execute event number       00h          RO
  -----------------------------------------------------------------------------

Register: []{#FCON_MTQ_INF_2 .anchor}FCON_MTQ_INF_2/ MT Queue Information 2 Register (Offset 134h)

Register Index: 134h \~ 137h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  Global MT queue 11 non-execute event number      00h          RO

   23:16  Global MT queue 10 non-execute event number      00h          RO

   15:8   Global MT queue 9 non-execute event number       00h          RO

    7:0   Global MT queue 8 non-execute event number       00h          RO
  -----------------------------------------------------------------------------

Register: []{#FCON_MTQ_INF_3 .anchor}FCON_MTQ_INF_3/ MT Queue Information 3 Register (Offset 138h)

Register Index: 138h \~ 13Bh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:24  Global MT queue 15 non-execute event number      00h          RO

   23:16  Global MT queue 14 non-execute event number      00h          RO

   15:8   Global MT queue 13 non-execute event number      00h          RO

    7:0   Global MT queue 12 non-execute event number      00h          RO
  -----------------------------------------------------------------------------

Register: []{#FCON_ERASE_PAG .anchor}FCON_ERASE_PAG/ Erase page Register (Offset 13Ch)

Register Index: 13Ch \~ 13Fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
  Bit     Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
  31:16   Reserved                                      00h        RsvdP

  15:0    Erase page threshold                          00h        RW
  -----------------------------------------------------------------------------

Register: []{#FCON_LLR_TABLE_0 .anchor}FCON_LLR_TABLE_0/ Erase page Register (Offset 140h)

Register Index: 140h \~ 143h

Default Value: 0302_0100h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:29  Reserved                                      00h        RsvdP

   28:24  LLR table\[00011\]                            3h         RW

   23:21  Reserved                                      00h        RsvdP

   20:16  LLR table\[00010\]                            2h         RW

   15:13  Reserved                                      00h        RsvdP

   12:8   LLR table\[00001\]                            1h         RW

    7:5   Reserved                                      00h        RsvdP

    4:0   LLR table\[00000\]                            0h         RW
  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

> Ex [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b0

The register represents Low Page LLR table

> Ex. [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b1

The register represents Middle Page LLR table

> Ex. [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b2

The register represents Upper Page LLR table

Register: FCON_LLR_TABLE_4/ LLR Table Register (Offset 144h)

Register Index: 144h \~ 147h

Default Value: 0706_0504h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:29  Reserved                                      00h        RsvdP

   28:24  LLR table\[00111\]                            7h         RW

   23:21  Reserved                                      00h        RsvdP

   20:16  LLR table\[00110\]                            6h         RW

   15:13  Reserved                                      00h        RsvdP

   12:8   LLR table\[00101\]                            5h         RW

    7:5   Reserved                                      00h        RsvdP

    4:0   LLR table\[00100\]                            4h         RW
  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

> Ex [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b0

The register represents Low Page LLR table

Register: FCON_LLR_TABLE_8/ LLR Table Register (Offset 148h)

Register Index: 148h \~ 14bh

Default Value: 0b0a_0908h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:29  Reserved                                      00h        RsvdP

   28:24  LLR table\[01011\]                            bh         RW

   23:21  Reserved                                      00h        RsvdP

   20:16  LLR table\[01010\]                            ah         RW

   15:13  Reserved                                      00h        RsvdP

   12:8   LLR table\[01001\]                            9h         RW

    7:5   Reserved                                      00h        RsvdP

    4:0   LLR table\[01000\]                            8h         RW
  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

> Ex [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b0

The register represents Low Page LLR table

Register: FCON_LLR_TABLE_12/ LLR Table Register (Offset 14ch)

Register Index: 14ch \~ 14fh

Default Value: 0f0e_0d0ch

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:29  Reserved                                      00h        RsvdP

   28:24  LLR table\[01111\]                            fh         RW

   23:21  Reserved                                      00h        RsvdP

   20:16  LLR table\[01110\]                            eh         RW

   15:13  Reserved                                      00h        RsvdP

   12:8   LLR table\[01101\]                            dh         RW

    7:5   Reserved                                      00h        RsvdP

    4:0   LLR table\[01100\]                            ch         RW
  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

> Ex [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b0

The register represents Low Page LLR table

Register: FCON_LLR_TABLE_16/ LLR Table Register (Offset 150h)

Register Index: 150h \~ 153h

Default Value: 1312_1110h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:29  Reserved                                      00h        RsvdP

   28:24  LLR table\[10011\]                            13h        RW

   23:21  Reserved                                      00h        RsvdP

   20:16  LLR table\[10010\]                            12h        RW

   15:13  Reserved                                      00h        RsvdP

   12:8   LLR table\[10001\]                            11h        RW

    7:5   Reserved                                      00h        RsvdP

    4:0   LLR table\[10000\]                            10h        RW
  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

> Ex [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b0

The register represents Low Page LLR table

Register: FCON_LLR_TABLE_20/ LLR Table Register (Offset 154h)

Register Index: 154h \~ 157h

Default Value: 1716_1514h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:29  Reserved                                      00h        RsvdP

   28:24  LLR table\[10111\]                            17h        RW

   23:21  Reserved                                      00h        RsvdP

   20:16  LLR table\[10110\]                            16h        RW

   15:13  Reserved                                      00h        RsvdP

   12:8   LLR table\[10101\]                            15h        RW

    7:5   Reserved                                      00h        RsvdP

    4:0   LLR table\[10100\]                            14h        RW
  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

> Ex [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b0

The register represents Low Page LLR table

Register: FCON_LLR_TABLE_24/ LLR Table Register (Offset 158h)

Register Index: 158h \~ 15bh

Default Value: 1b1a_1918h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:29  Reserved                                      00h        RsvdP

   28:24  LLR table\[11011\]                            1bh        RW

   23:21  Reserved                                      00h        RsvdP

   20:16  LLR table\[11010\]                            1ah        RW

   15:13  Reserved                                      00h        RsvdP

   12:8   LLR table\[11001\]                            19h        RW

    7:5   Reserved                                      00h        RsvdP

    4:0   LLR table\[11000\]                            18h        RW
  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

> Ex [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b0

The register represents Low Page LLR table

Register: FCON_LLR_TABLE_28/ LLR Table Register (Offset 15ch)

Register Index: 15ch \~ 15fh

Default Value: 1f1e_1d1ch

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:29  Reserved                                      00h        RsvdP

   28:24  LLR table\[11111\]                            1fh        RW

   23:21  Reserved                                      00h        RsvdP

   20:16  LLR table\[11110\]                            1eh        RW

   15:13  Reserved                                      00h        RsvdP

   12:8   LLR table\[11101\]                            1dh        RW

    7:5   Reserved                                      00h        RsvdP

    4:0   LLR table\[11100\]                            1ch        RW
  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

> Ex [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\] == 2'b0

The register represents Low Page LLR table

Register: []{#FCON_DSP_PARAM_0 .anchor}FCON_DSP_PARAM_0/ DSP PARAM 0 Register (Offset 160h)

Register Index: 160h \~ 163h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP parameter\[31:0\]                         0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_PARAM_1/ DSP PARAM 1 Register (Offset 164h)

Register Index: 164h \~ 167h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:28  Reserved                                      00h        RsvdP

   27:0   DSP parameter\[59:32\]                        0h         RW
  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: []{#FCON_DSP_GRAY_CODE_0 .anchor}FCON_DSP_GRAY_CODE_0/ DSP GRAY CODE 0 Register (Offset 168h)

Register Index: 168h \~ 16bh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP GRAY CODE 0                               0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_GRAY_CODE_1/ DSP GRAY CODE 1 Register (Offset 16ch)

Register Index: 16ch \~ 16fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP GRAY CODE 1                               0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_GRAY_CODE_2/ DSP GRAY CODE 2 Register (Offset 170h)

Register Index: 170h \~ 173h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP GRAY CODE 2                               0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_GRAY_CODE_3/ DSP GRAY CODE 3 Register (Offset 174h)

Register Index: 174h \~ 177h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP GRAY CODE 3                               0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_GRAY_CODE_4/ DSP GRAY CODE 4 Register (Offset 178h)

Register Index: 178h \~ 17bh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP GRAY CODE 4                               0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: []{#FCON_DSP_LUT_0 .anchor}FCON_DSP_LUT_0/ DSP LUT 0 Register (Offset 17ch)

Register Index: 17ch \~ 17fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP LUT 0                                     0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_LUT_1/ DSP LUT 1 Register (Offset 180h)

Register Index: 180h \~ 183h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP LUT 1                                     0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_LUT_2/ DSP LUT 2 Register (Offset 184h)

Register Index: 184h \~ 187h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP LUT 2                                     0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_LUT_3/ DSP LUT 3 Register (Offset 188h)

Register Index: 188h \~ 18bh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP LUT 3                                     0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_LUT_4/ DSP LUT 4 Register (Offset 18ch)

Register Index: 18ch \~ 18fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP LUT 4                                     0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_LUT_5/ DSP LUT 5 Register (Offset 190h)

Register Index: 190h \~ 193h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP LUT 5                                     0h         RW

  -----------------------------------------------------------------------------

- The register RW is switched by [FCON_LDPC_CFG](#FCON_LDPC_CFG)\[25:24\]

Register: FCON_DSP_CENTER/ DSP CENTER Register (Offset 194h)

Register Index: 194h \~ 197h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   reserved                                      0h         RsvdP

   19:0   DSP Center                                               RO
  -----------------------------------------------------------------------------

Register: []{#FCON_DSP_DISTRIBUTION_0 .anchor}FCON_DSP_DISTRIBUTION_0/ DSP DISTRIBUTION 0 Register (Offset 198h)

Register Index: 198h \~ 19bh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[31:0\]                                 RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_1/ DSP DISTRIBUTION 1 Register (Offset 19ch)

Register Index: 19ch \~ 19fh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[63:32\]                                RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_2/ DSP DISTRIBUTION 2 Register (Offset 1a0h)

Register Index: 1a0h \~ 1a3h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[95:64\]                                RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_3/ DSP DISTRIBUTION 3 Register (Offset 1 a4h)

Register Index: 1a4h \~ 1a7h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[127:96\]                               RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_4/ DSP DISTRIBUTION 4 Register (Offset 1a8h)

Register Index: 1a8h \~ 1abh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[159:128\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_5/ DSP DISTRIBUTION 5 Register (Offset 1ach)

Register Index: 1ach \~ 1afh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[191:160\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_6/ DSP DISTRIBUTION 6 Register (Offset 1b0h)

Register Index: 1b0h \~ 1b3h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[223:192\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_7/ DSP DISTRIBUTION 7 Register (Offset 1 b4h)

Register Index: 1b4h \~ 1b7h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[255:224\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_8/ DSP DISTRIBUTION 8 Register (Offset 1b8h)

Register Index: 1b8h \~ 1bbh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[287:256\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_9/ DSP DISTRIBUTION 9 Register (Offset 1bch)

Register Index: 1bch \~ 1bfh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[319:288\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_10/ DSP DISTRIBUTION 10 Register (Offset 1c0h)

Register Index: 1c0h \~ 1c3h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[351:320\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_11/ DSP DISTRIBUTION 11 Register (Offset 1 c4h)

Register Index: 1c4h \~ 1c7h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[383:352\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_12/ DSP DISTRIBUTION 12 Register (Offset 1c8h)

Register Index: 1c8h \~ 1cbh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[415:384\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_13/ DSP DISTRIBUTION 13 Register (Offset 1cch)

Register Index: 1cch \~ 1cfh

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[447:416\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_14/ DSP DISTRIBUTION 14 Register (Offset 1d0h)

Register Index: 1d0h \~ 1d3h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[479:448\]                              RO

  -----------------------------------------------------------------------------

Register: FCON_DSP_DISTRIBUTION_15/ DSP DISTRIBUTION 15 Register (Offset 1d4h)

Register Index: 1d4h \~ 1d7h

Default Value: 0000_0000h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- --------------------------------------------- ---------- ------------
   31:0   DSP Distribution\[511:480\]                              RO

  -----------------------------------------------------------------------------

## 5.5. Register of digital PLL

The address map of digital PLL in below

U40LP MDLL guarantee the slowest MDLL clock tracking is 80MHz which means the slowest data rate for clock tracking is 80MBPS.

  ---------------------------------------------------------------------------------
  Offset   Name                                  Mode   Function
  -------- ------------------------------------- ------ ---------------------------
  0x00     [DLL_DLL_CTL](#DLL_DLL_CTL)           rw

  0x04     [DLL_MDLL_STA](#DLL_MDLL_STA)         r

  0x08     [DLL_LOAD_CTL](#DLL_LOAD_CTL)         rw

  0x0C     [DLL_SDLL_OFFSET](#DLL_SDLL_OFFSET)   rw

  0x10     [DLL_PHASE_R_CTL](#DLL_PHASE_R_CTL)

  0x14     [DLL_PHASE_W_CTL](#DLL_PHASE_W_CTL)

  0x18

  0x1C     [DLL_GCK_DLY](#DLL_GCK_DLY)           rw

  0x20     [DLL_SEL_VAL_0](#DLL_SEL_VAL_0)       rw

  0x24     [DLL_SEL_VAL_1](#DLL_SEL_VAL_1)       rw

  0x28     [DLL_SEL_VAL_2](#DLL_SEL_VAL_2)       rw

  0x2C     [DLL_SEL_VAL_3](#DLL_SEL_VAL_3)       rw

  0x30     [DLL_SEL_VAL_4](#DLL_SEL_VAL_4)       rw

  0x34     [DLL_SEL_VAL_5](#DLL_SEL_VAL_5)       rw

  0x38     [DLL_SEL_VAL_6](#DLL_SEL_VAL_6)       rw

  0x3C     [DLL_SEL_VAL_7](#DLL_SEL_VAL_7)       rw

  0x40     [DLL_SDLL_VAL_0](#DLL_SDLL_VAL_0)     r

  0x44     [DLL_SDLL_VAL_1](#DLL_SDLL_VAL_1)     r

  0x48     [DLL_SDLL_VAL_2](#DLL_SDLL_VAL_2)     r

  0x4C     [DLL_SDLL_VAL_3](#DLL_SDLL_VAL_3)     r

  0x30     [DLL_SDLL_VAL_4](#DLL_SDLL_VAL_4)     r

  0x54     [DLL_SDLL_VAL_5](#DLL_SDLL_VAL_5)     r

  0x58     [DLL_SDLL_VAL_6](#DLL_SDLL_VAL_6)     r

  0x5C     [DLL_SDLL_VAL_7](#DLL_SDLL_VAL_7)     r

  0x60     DLL_SDLL_OFFSET_0                     rw

  0x64     DLL_SDLL_OFFSET_1                     rw

  0x68     DLL_SDLL_OFFSET_2                     rw

  0x6C     DLL_SDLL_OFFSET_3                     rw

  0x70     DLL_SDLL_OFFSET_4                     rw

  0x74     DLL_SDLL_OFFSET_5                     rw

  0x78     DLL_SDLL_OFFSET_6                     rw

  0x7C     DLL_SDLL_OFFSET_7                     rw

  0x80     SDLL_TEST_CTL_0

  0x84     SDLL_TEST_CTL_0

  0x88     SDLL_TEST_CTL_0

  0x8C     SDLL_TEST_CTL_0

  0x90     SDLL_TEST_CTL_0

  0x94     SDLL_TEST_CTL_0

  0x98     DLL_W_LGC_SEL_VAL0_1                  rw

  0x9C     DLL_W_LGC_SEL_VAL2_3                  rw

  0xA0     DLL_W_LGC_SEL_VAL4_5                  rw

  0xA4     DLL_W_LGC_SEL_VAL6_7                  rw
  ---------------------------------------------------------------------------------

  : Register list for digital PLL

## 5.6. Details specification for register of digital PLL

Register: []{#DLL_DLL_CTL .anchor}DLL_DLL_CTL/ DLL Control Register (Offset 00h)

Register Index: 00h \~ 03h

Default Value: 0400_0032h

+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                                                                                             | Default | Attributes |
+:=====:+==========================================================================================================================================================+:=======:+:==========:+
| 31:30 | MDLL search mode                                                                                                                                         | 00b     | RW         |
|       |                                                                                                                                                          |         |            |
|       | For test_mode usage                                                                                                                                      |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 29:28 | MDLL track filter                                                                                                                                        | 00b     | RW         |
|       |                                                                                                                                                          |         |            |
|       | For test_mode usage                                                                                                                                      |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 27:19 | Reserved                                                                                                                                                 | 0b      | RW         |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 18    | MDLL Delay Line Configuration-EN_EXTRA for Legacy SDLL_W delay of Channel 1.                                                                             | 0b      | RW         |
|       |                                                                                                                                                          |         |            |
|       | This value is value if flash type equals to Legacy mode and SDLLW_AUTO_SWITCH=1.                                                                         |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 17    | MDLL Delay Line Configuration-EN_EXTRA for Legacy SDLL_W delay of Channel 0.                                                                             | 0b      | RW         |
|       |                                                                                                                                                          |         |            |
|       | This value is value if flash type equals to Legacy mode and SDLLW_AUTO_SWITCH=1.                                                                         |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 16:8  | MDLL tracking result offset\[8:0\]                                                                                                                       | 00h     | RW         |
|       |                                                                                                                                                          |         |            |
|       | Offset for MDLL tracking result, before average                                                                                                          |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 7     | MDLL Delay Line Configuration-EN_EXTRA                                                                                                                   | 0b      | RW         |
|       |                                                                                                                                                          |         |            |
|       | This configuration is always valid if SDLLW_AUTO_SWITCH=0.                                                                                               |         |            |
|       |                                                                                                                                                          |         |            |
|       | If SDLLW_AUTO_SWITCH=1, this configuration is valid if flash type does not equal to Legacy mode.                                                         |         |            |
|       |                                                                                                                                                          |         |            |
|       | Extra delay for Delay Line module.                                                                                                                       |         |            |
|       |                                                                                                                                                          |         |            |
|       | After EN_EXTRA is set, the MDLL needs to be re-tracking.                                                                                                 |         |            |
|       |                                                                                                                                                          |         |            |
|       | 1: increase large delay value as following SPEC:                                                                                                         |         |            |
|       |                                                                                                                                                          |         |            |
|       |   Min(FF corner for 1.2V, -40degreeC): 2ns                                                                                                               |         |            |
|       |                                                                                                                                                          |         |            |
|       |   Typ(TT corner for 1.1V, 25degreeC):3.4ns                                                                                                               |         |            |
|       |                                                                                                                                                          |         |            |
|       |   Max(SS corner for 1.0V, 125degreeC):7.2ns                                                                                                              |         |            |
|       |                                                                                                                                                          |         |            |
|       | 0:disable for large delay value increase.                                                                                                                |         |            |
|       |                                                                                                                                                          |         |            |
|       | Usage limitation:                                                                                                                                        |         |            |
|       |                                                                                                                                                          |         |            |
|       | According to SDLL_SPEC, SDLL EN_EXTRA contribute delay for 3.4ns which means the max tracking frequency of EN_EXTRA=1 is 1/(2x3.4+0.29ns)=143MHz.        |         |            |
|       |                                                                                                                                                          |         |            |
|       | The min. tracking frequency of EN_EXTRA=0 is 1/2x(0.29+0.048x128+0.024+0.012)=77MHz                                                                      |         |            |
|       |                                                                                                                                                          |         |            |
|       | Conclusion:                                                                                                                                              |         |            |
|       |                                                                                                                                                          |         |            |
|       | If data rate slower than 100MBPS, EN_EXTRA has to be set as 0x1.                                                                                         |         |            |
|       |                                                                                                                                                          |         |            |
|       | If data rate faster than 100MBPS, EN_EXTRA has to be set as 0x0.                                                                                         |         |            |
|       |                                                                                                                                                          |         |            |
|       |                                                                                                                                                          |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 6     | MDLL Delay Line Configuration-CFG_R                                                                                                                      | 1b      | RW         |
|       |                                                                                                                                                          |         |            |
|       | 0: Disable filter enhancement                                                                                                                            |         |            |
|       |                                                                                                                                                          |         |            |
|       | 1: Enable filter enhancement                                                                                                                             |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 5     | SDLLW_AUTO_SWITCH                                                                                                                                        | 1b      | RW         |
|       |                                                                                                                                                          |         |            |
|       | Due to MT has function to change FLASH_TYPE on the fly between LEGACY, TOGGLE, and ONFI.                                                                 |         |            |
|       |                                                                                                                                                          |         |            |
|       | SDLL_W needs to change values between FW configured parameters and MDLL tracking values when TOGGLE/ONFI turns to LEGACY or LEGACY turns to TOGGLE/ONFI. |         |            |
|       |                                                                                                                                                          |         |            |
|       | 0: SDLL_W always input MDLL tracking delay values.                                                                                                       |         |            |
|       |                                                                                                                                                          |         |            |
|       | 1:SDLL_W receive MDLL tracking delay values when FLASH_TYPE=TOGGLE or ONFI.                                                                              |         |            |
|       |                                                                                                                                                          |         |            |
|       | SDLL_W receive DLL_W_LGC_SEL_VAL\* when FLASH_TYPE=LEGACY or L16.                                                                                        |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 4     | MDLL_EN                                                                                                                                                  | 1b      | RW         |
|       |                                                                                                                                                          |         |            |
|       | 0b: idle                                                                                                                                                 |         |            |
|       |                                                                                                                                                          |         |            |
|       | 1b : MDLL circuit start tracking                                                                                                                         |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3     | Reserved                                                                                                                                                 | 0b      | RW         |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2     | MDLL_RSTJ                                                                                                                                                | 0b      | RW         |
|       |                                                                                                                                                          |         |            |
|       | 0b: MDLL circuit be reset                                                                                                                                |         |            |
|       |                                                                                                                                                          |         |            |
|       | 1b:normal                                                                                                                                                |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1:0   | Dll mode                                                                                                                                                 | 10b     | RW         |
|       |                                                                                                                                                          |         |            |
|       | This field control when to apply new SDLL value to each SDLL.                                                                                            |         |            |
|       |                                                                                                                                                          |         |            |
|       | 00: FLH_STROBE mode                                                                                                                                      |         |            |
|       |                                                                                                                                                          |         |            |
|       | 01: MDLL_STROBE mode (prohibit to use)                                                                                                                   |         |            |
|       |                                                                                                                                                          |         |            |
|       | 1x: FW direct control SDLL mode                                                                                                                          |         |            |
+-------+----------------------------------------------------------------------------------------------------------------------------------------------------------+---------+------------+

Register: []{#DLL_MDLL_STA .anchor}DLL_MDLL_STA/ DLL Status Register (Offset 04h)

Register Index: 04h \~ 07h

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default    Attributes
  ------- -------------------------------------------- ---------- ------------
   31:17  Reserved                                        00b          RO

   16:8   MDLL tracking result                             0h          RO

    7:4   Reserved                                         0h          RO

     3    MDLL underflow                                   0b          RO

     2    MDLL overflow                                    0b          RO

     1    MDLL FAIL                                        0b          RO

     0    MDLL LOCK                                        0b          RO
  ----------------------------------------------------------------------------

Register: []{#DLL_LOAD_CTL .anchor}DLL_LOAD_CTL/ SDLL Load Control Register (Offset 08h)

Register Index: 08h \~ 0Bh

Default Value: 4000_8000h

+-------+-------------------------------------------------+---------+------------+
| Bit   | Descriptions                                    | Default | Attributes |
+:=====:+=================================================+:=======:+:==========:+
| 31    | Trigger auto load                               | 0b      | RW         |
|       |                                                 |         |            |
|       | For period mode usage                           |         |            |
+-------+-------------------------------------------------+---------+------------+
| 30    | MDLL average register load MDLL value directory | 1b      | RW         |
|       |                                                 |         |            |
|       | 0b: wait TC descript                            |         |            |
+-------+-------------------------------------------------+---------+------------+
| 29    | Reserved                                        | 0b      | RW         |
+-------+-------------------------------------------------+---------+------------+
| 28    | Enable load with DMA Trigger                    | 0b      | RW         |
|       |                                                 |         |            |
|       | 0b: disable                                     |         |            |
|       |                                                 |         |            |
|       | 1b: enable trigger mode for dly value update    |         |            |
+-------+-------------------------------------------------+---------+------------+
| 27    | Reserved                                        | 0b      | RW         |
+-------+-------------------------------------------------+---------+------------+
| 26    | Enable auto en mdll                             | 0b      | RW         |
|       |                                                 |         |            |
|       | For period mode usage                           |         |            |
+-------+-------------------------------------------------+---------+------------+
| 25    | Enable auto reset mdll                          | 0b      | RW         |
|       |                                                 |         |            |
|       | For period mode usage                           |         |            |
+-------+-------------------------------------------------+---------+------------+
| 24    | Enable auto load                                | 0b      | RW         |
|       |                                                 |         |            |
|       | 0b: disable                                     |         |            |
|       |                                                 |         |            |
|       | 1b: enable period mode for dly value update     |         |            |
+-------+-------------------------------------------------+---------+------------+
| 23:16 | Load once MDLL result to SDLL                   | 00h     | RW         |
|       |                                                 |         |            |
|       | \[16\] load ch0 ... \[23\] load ch7             |         |            |
|       |                                                 |         |            |
|       | 0b: idle                                        |         |            |
|       |                                                 |         |            |
|       | 1b: update dly value once                       |         |            |
+-------+-------------------------------------------------+---------+------------+
| 15:0  | Auto load period                                | 8000h   | RW         |
|       |                                                 |         |            |
|       | For period mode usage                           |         |            |
+-------+-------------------------------------------------+---------+------------+

Register: []{#DLL_SDLL_OFFSET .anchor}DLL_SDLL_OFFSET/ SDLL Offset Register (Offset 0Ch)

Register Index: 0Ch \~ 0Fh

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RW

   24:16  SDLL Offset for write path                      00h     RW

   15:9   Reserved                                         0h     RW

    8:0   SDLL Offset for read path                       00h     RW
  ----------------------------------------------------------------------------

Register: []{#DLL_PHASE_R_CTL .anchor}DLL_PHASE_R_CTL/ DLL Read PHASE Register (Offset 10h)

Register Index: 10h \~ 13h

Default Value: 0092_4924h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+============+
| 31:24 | Reserved                                   | 0h      | R          |
+-------+--------------------------------------------+---------+------------+
| 23:21 | Degree for Ch7 Read mode                   | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 20:18 | Degree for Ch6 Read mode                   | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 17:15 | Degree for Ch5 Read mode                   | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 14:12 | Degree for Ch4 Read mode                   | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 11:9  | Degree for Ch3 Read mode                   | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 8:6   | Degree for Ch2 Read mode                   | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 5:3   | Degree for Ch1 Read mode                   | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 2:0   | Degree for Ch0 Read mode                   | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#DLL_PHASE_W_CTL .anchor}DLL_PHASE_W_CTL/ DLL Write PHASE Register (Offset 14h)

Register Index: 14h \~ 17h

Default Value: 0092_4924h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+============+
| 31:24 | Reserved                                   | 0h      | R          |
+-------+--------------------------------------------+---------+------------+
| 23:21 | Degree for Ch7 Write mode                  | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 20:18 | Degree for Ch6 Write mode                  | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 17:15 | Degree for Ch5 Write mode                  | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 14:12 | Degree for Ch4 Write mode                  | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 11:9  | Degree for Ch3 Write mode                  | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 8:6   | Degree for Ch2 Write mode                  | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 5:3   | Degree for Ch1 Write mode                  | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+
| 2:0   | Degree for Ch0 Write mode                  | 4h      | RW         |
|       |                                            |         |            |
|       | 100b: 90 degree                            |         |            |
|       |                                            |         |            |
|       | 011b: 67.5 degree                          |         |            |
|       |                                            |         |            |
|       | 010b: 45 degree                            |         |            |
|       |                                            |         |            |
|       | 001b: 22.5 degree                          |         |            |
|       |                                            |         |            |
|       | 101b: 112.5 degree                         |         |            |
|       |                                            |         |            |
|       | 110b: 135 degree                           |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#DLL_GCK_DLY .anchor}DLL_GCK_DLY/ Legacy read Delay Register (Offset 1Ch)

Register Index: 1Ch \~ 1Fh

Default Value: 0000_0010h

+-------+--------------------------------------------+---------+------------+
| Bit   | Descriptions                               | Default | Attributes |
+:=====:+============================================+:=======:+============+
| 31:16 | Reserved                                   | 0h      | RO         |
+-------+--------------------------------------------+---------+------------+
| 15:14 | Reserved                                   | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 13:12 | Legacy read delay cell usage               | 0h      | RW         |
|       |                                            |         |            |
|       | GCK_R_SEL                                  |         |            |
+-------+--------------------------------------------+---------+------------+
| 11:9  | Reserved                                   | 00h     | RW         |
+-------+--------------------------------------------+---------+------------+
| 8:0   | Legacy read delay cell usage               | 10h     | RW         |
|       |                                            |         |            |
|       | GCK_SEL                                    |         |            |
+-------+--------------------------------------------+---------+------------+

Register: []{#DLL_SEL_VAL_0 .anchor}DLL_SEL_VAL_0/ SDLL Direct Value 0 Register (Offset 20h)

Register Index: 20h \~ 23h

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RW

   24:16  CH0 SDLL direct value for write path            3ffh    RW

   15:9   Reserved                                         0h     RW

    8:0   CH0 SDLL direct value for read path             3ffh    RW
  ----------------------------------------------------------------------------

Register: []{#DLL_SEL_VAL_1 .anchor}DLL_SEL_VAL_1/ SDLL Direct Value 1 Register (Offset 24h)

Register Index: 24h \~ 27h

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RW

   24:16  CH1 SDLL direct value for write path            3ffh    RW

   15:9   Reserved                                         0h     RW

    8:0   CH1 SDLL direct value for read path             3ffh    RW
  ----------------------------------------------------------------------------

Register: []{#DLL_SEL_VAL_2 .anchor}DLL_SEL_VAL_2/ SDLL Direct Value 2 Register (Offset 28h)

Register Index: 28h\~ 2Bh

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RW

   24:16  CH2 SDLL direct value for write path            80h     RW

   15:9   Reserved                                         0h     RW

    8:0   CH2 SDLL direct value for read path             80h     RW
  ----------------------------------------------------------------------------

Register: []{#DLL_SEL_VAL_3 .anchor}DLL_SEL_VAL_3/ SDLL Direct Value 3 Register (Offset 2Ch)

Register Index: 2Ch \~ 2Fh

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RW

   24:16  CH3 SDLL direct value for write path            80h     RW

   15:9   Reserved                                         0h     RW

    8:0   CH3 SDLL direct value for read path             80h     RW
  ----------------------------------------------------------------------------

Register: []{#DLL_SEL_VAL_4 .anchor}DLL_SEL_VAL_4/ SDLL Direct Value 4 Register (Offset 30h)

Register Index: 30h \~ 33h

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RW

   24:16  CH4 SDLL direct value for write path            80h     RW

   15:9   Reserved                                         0h     RW

    8:0   CH4 SDLL direct value for read path             80h     RW
  ----------------------------------------------------------------------------

Register: []{#DLL_SEL_VAL_5 .anchor}DLL_SEL_VAL_5/ SDLL Direct Value 5 Register (Offset 34h)

Register Index: 34h \~ 37h

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RW

   24:16  CH5 SDLL direct value for write path            80h     RW

   15:9   Reserved                                         0h     RW

    8:0   CH5 SDLL direct value for read path             80h     RW
  ----------------------------------------------------------------------------

Register: []{#DLL_SEL_VAL_6 .anchor}DLL_SEL_VAL_6/ SDLL Direct Value 6 Register (Offset 38h)

Register Index: 38h \~ 3Bh

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RW

   24:16  CH6 SDLL direct value for write path            80h     RW

   15:9   Reserved                                         0h     RW

    8:0   CH6 SDLL direct value for read path             80h     RW
  ----------------------------------------------------------------------------

Register: []{#DLL_SEL_VAL_7 .anchor}DLL_SEL_VAL_7/ SDLL Direct Value 7 Register (Offset 3Ch)

Register Index: 3Ch \~ 3Fh

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RW

   24:16  CH7 SDLL direct value for write path            80h     RW

   15:9   Reserved                                         0h     RW

    8:0   CH7 SDLL direct value for read path             80h     RW
  ----------------------------------------------------------------------------

Register: []{#DLL_SDLL_VAL_0 .anchor}DLL_SDLL_VAL_0/ SDLL Value with Cell 0 Register (Offset 40h)

Register Index: 40h \~ 43h

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH0 SDLL delay cell value for write path        80h     RO

   15:9   Reserved                                         0h     RO

    8:0   CH0 SDLL delay cell value for read path         80h     RO
  ----------------------------------------------------------------------------

Register: []{#DLL_SDLL_VAL_1 .anchor}DLL_SDLL_VAL_1/ SDLL Value with Cell 1 Register (Offset 44h)

Register Index: 44h \~ 47h

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH1 SDLL delay cell value for write path        80h     RO

   15:9   Reserved                                         0h     RO

    8:0   CH1 SDLL delay cell value for read path         80h     RO
  ----------------------------------------------------------------------------

Register: []{#DLL_SDLL_VAL_2 .anchor}DLL_SDLL_VAL_2/ SDLL Value with Cell 2 Register (Offset 48h)

Register Index: 48h \~ 4Bh

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH2 SDLL delay cell value for write path        80h     RO

   15:9   Reserved                                         0h     RO

    8:0   CH2 SDLL delay cell value for read path         80h     RO
  ----------------------------------------------------------------------------

Register: []{#DLL_SDLL_VAL_3 .anchor}DLL_SDLL_VAL_3/ SDLL Value with Cell 3 Register (Offset 4Ch)

Register Index: 4Ch \~ 4Fh

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH3 SDLL delay cell value for write path        80h     RO

   15:9   Reserved                                         0h     RO

    8:0   CH3 SDLL delay cell value for read path         80h     RO
  ----------------------------------------------------------------------------

Register: []{#DLL_SDLL_VAL_4 .anchor}DLL_SDLL_VAL_4/ SDLL Value with Cell 4 Register (Offset 50h)

Register Index: 50h \~ 53h

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH4 SDLL delay cell value for write path        80h     RO

   15:9   Reserved                                         0h     RO

    8:0   CH4 SDLL delay cell value for read path         80h     RO
  ----------------------------------------------------------------------------

Register: []{#DLL_SDLL_VAL_5 .anchor}DLL_SDLL_VAL_5/ SDLL Value with Cell 5 Register (Offset 54h)

Register Index: 54h \~ 57h

Default Value: 0080_0080h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH5 SDLL delay cell value for write path        80h     RO

   15:9   Reserved                                         0h     RO

    8:0   CH5 SDLL delay cell value for read path         80h     RO
  ----------------------------------------------------------------------------

Register: []{#DLL_SDLL_VAL_6 .anchor}DLL_SDLL_VAL_6/ SDLL Value with Cell 6 Register (Offset 58h)

Register Index: 58h \~ 5Bh

Default Value: 0080_0080h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default   Attributes
  ------- --------------------------------------------- ---------- ------------
   31:25  Reserved                                          0h     RO

   24:16  CH6 SDLL delay cell value for write path         80h     RO

   15:9   Reserved                                          0h     RO

    8:0   CH6 SDLL delay cell value for read path          80h     RO
  -----------------------------------------------------------------------------

Register: []{#DLL_SDLL_VAL_7 .anchor}DLL_SDLL_VAL_7/ SDLL Value with Cell 7 Register (Offset 5Ch)

Register Index: 5Ch \~ 5Fh

Default Value: 0080_0080h

  -----------------------------------------------------------------------------
    Bit   Descriptions                                   Default   Attributes
  ------- --------------------------------------------- ---------- ------------
   31:25  Reserved                                          0h     RO

   24:16  CH7 SDLL delay cell value for write path         80h     RO

   15:9   Reserved                                          0h     RO

    8:0   CH7 SDLL delay cell value for read path          80h     RO
  -----------------------------------------------------------------------------

Register: DLL_SDLL_OFFSET_0/ SDLL Value with Cell 0 Register (Offset 60h)

Register Index: 60h \~ 63h

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH0 SDLL delay offset value for write path      00h     WO

   15:9   Reserved                                         0h     RO

    8:0   CH0 SDLL delay offset value for read path       00h     WO
  ----------------------------------------------------------------------------

Register: DLL_SDLL_OFFSET_1/ SDLL Value with Cell 1 Register (Offset 64h)

Register Index: 64h \~ 67h

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH1 SDLL delay offset value for write path      00h     WO

   15:9   Reserved                                         0h     RO

    8:0   CH1 SDLL delay offset value for read path       00h     WO
  ----------------------------------------------------------------------------

Register: DLL_SDLL_OFFSET_2/ SDLL Value with Cell 2 Register (Offset 60h)

Register Index: 60h \~ 63h

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH2 SDLL delay offset value for write path      00h     RW

   15:9   Reserved                                         0h     RO

    8:0   CH2 SDLL delay offset value for read path       00h     RW
  ----------------------------------------------------------------------------

Register: DLL_SDLL_OFFSET_3/ SDLL Value with Cell 3 Register (Offset 64h)

Register Index: 64h \~ 67h

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH3 SDLL delay offset value for write path      00h     RW

   15:9   Reserved                                         0h     RO

    8:0   CH3 SDLL delay offset value for read path       00h     RW
  ----------------------------------------------------------------------------

Register: DLL_SDLL_OFFSET_4/ SDLL Value with Cell 4 Register (Offset 68h)

Register Index: 68h \~ 6Bh

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH4 SDLL delay offset value for write path      00h     RW

   15:9   Reserved                                         0h     RO

    8:0   CH4 SDLL delay offset value for read path       00h     RW
  ----------------------------------------------------------------------------

Register: DLL_SDLL_OFFSET_5/ SDLL Value with Cell 5 Register (Offset 6Ch)

Register Index: 6Ch \~ 6Fh

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH5 SDLL delay offset value for write path      00h     RW

   15:9   Reserved                                         0h     RO

    8:0   CH5 SDLL delay offset value for read path       00h     RW
  ----------------------------------------------------------------------------

Register: DLL_SDLL_OFFSET_6/ SDLL Value with Cell 6 Register (Offset 70h)

Register Index: 70h \~ 73h

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH6 SDLL delay offset value for write path      00h     RW

   15:9   Reserved                                         0h     RO

    8:0   CH6 SDLL delay offset value for read path       00h     RW
  ----------------------------------------------------------------------------

Register: DLL_SDLL_OFFSET_7/ SDLL Value with Cell 7 Register (Offset 74h)

Register Index: 74h \~ 77h

Default Value: 0000_0000h

  ----------------------------------------------------------------------------
    Bit   Descriptions                                  Default   Attributes
  ------- -------------------------------------------- ---------- ------------
   31:25  Reserved                                         0h     RO

   24:16  CH7 SDLL delay offset value for write path      00h     RW

   15:9   Reserved                                         0h     RO

    8:0   CH7 SDLL delay offset value for read path       00h     RW
  ----------------------------------------------------------------------------

Register: DLL_LGC_SEL_VAL0_1/ SDLLW LEGACY Direct Value 0/1 Register (Offset 98h)

Register Index: 98h \~ 9Bh

Default Value: 0016_0016h

+-------+---------------------------------------------------+---------+------------+
| Bit   | Descriptions                                      | Default | Attributes |
+:=====:+===================================================+:=======:+============+
| 31:25 | Reserved                                          | 0h      | RO         |
+-------+---------------------------------------------------+---------+------------+
| 24:16 | DLL_LGC_SEL_VAL1                                  | 16h     | RW         |
|       |                                                   |         |            |
|       | Only valid when SDLLW_AUTO_SWITCH=1.              |         |            |
|       |                                                   |         |            |
|       | CH0 SDLL_W direct value for legacy/L16 write path |         |            |
+-------+---------------------------------------------------+---------+------------+
| 15:9  | Reserved                                          | 0h      | RO         |
+-------+---------------------------------------------------+---------+------------+
| 8:0   | DLL_LGC_SEL_VAL0                                  | 16h     | RW         |
|       |                                                   |         |            |
|       | Only valid when SDLLW_AUTO_SWITCH=1.              |         |            |
|       |                                                   |         |            |
|       | CH0 SDLL_W direct value for legacy/L16 write path |         |            |
+-------+---------------------------------------------------+---------+------------+

Register: DLL_LGC_SEL_VAL2_3/ SDLLW LEGACY Direct Value 2/3 Register (Offset 9Ch)

Register Index: 9Ch \~ 9Fh

Default Value: 0016_0016h

+-------+---------------------------------------------------+---------+------------+
| Bit   | Descriptions                                      | Default | Attributes |
+:=====:+===================================================+:=======:+============+
| 31:25 | Reserved                                          | 0h      | RO         |
+-------+---------------------------------------------------+---------+------------+
| 24:16 | DLL_LGC_SEL_VAL3                                  | 16h     | RW         |
|       |                                                   |         |            |
|       | Only valid when SDLLW_AUTO_SWITCH=1.              |         |            |
|       |                                                   |         |            |
|       | CH3 SDLL_W direct value for legacy/L16 write path |         |            |
+-------+---------------------------------------------------+---------+------------+
| 15:9  | Reserved                                          | 0h      | RO         |
+-------+---------------------------------------------------+---------+------------+
| 8:0   | DLL_LGC_SEL_VAL2                                  | 16h     | RW         |
|       |                                                   |         |            |
|       | Only valid when SDLLW_AUTO_SWITCH=1.              |         |            |
|       |                                                   |         |            |
|       | CH2 SDLL_W direct value for legacy/L16 write path |         |            |
+-------+---------------------------------------------------+---------+------------+

Register: DLL_LGC_SEL_VAL4_5/ SDLLW LEGACY Direct Value 4/5 Register (Offset A0h)

Register Index: A0h \~ A3h

Default Value: 0016_0016h

+-------+---------------------------------------------------+---------+------------+
| Bit   | Descriptions                                      | Default | Attributes |
+:=====:+===================================================+:=======:+============+
| 31:25 | Reserved                                          | 0h      | RO         |
+-------+---------------------------------------------------+---------+------------+
| 24:16 | DLL_LGC_SEL_VAL5                                  | 16h     | RW         |
|       |                                                   |         |            |
|       | Only valid when SDLLW_AUTO_SWITCH=1.              |         |            |
|       |                                                   |         |            |
|       | CH5 SDLL_W direct value for legacy/L16 write path |         |            |
+-------+---------------------------------------------------+---------+------------+
| 15:9  | Reserved                                          | 0h      | RO         |
+-------+---------------------------------------------------+---------+------------+
| 8:0   | DLL_LGC_SEL_VAL4                                  | 16h     | RW         |
|       |                                                   |         |            |
|       | Only valid when SDLLW_AUTO_SWITCH=1.              |         |            |
|       |                                                   |         |            |
|       | CH4 SDLL_W direct value for legacy/L16 write path |         |            |
+-------+---------------------------------------------------+---------+------------+

Register: DLL_LGC_SEL_VAL6_7/ SDLLW LEGACY Direct Value 6/7 Register (Offset 9Ch)

Register Index: 9Ch \~ 9Fh

Default Value: 0016_0016h

+-------+---------------------------------------------------+---------+------------+
| Bit   | Descriptions                                      | Default | Attributes |
+:=====:+===================================================+:=======:+============+
| 31:25 | Reserved                                          | 0h      | RO         |
+-------+---------------------------------------------------+---------+------------+
| 24:16 | DLL_LGC_SEL_VAL7                                  | 16h     | RW         |
|       |                                                   |         |            |
|       | Only valid when SDLLW_AUTO_SWITCH=1.              |         |            |
|       |                                                   |         |            |
|       | CH7 SDLL_W direct value for legacy/L16 write path |         |            |
+-------+---------------------------------------------------+---------+------------+
| 15:9  | Reserved                                          | 0h      | RO         |
+-------+---------------------------------------------------+---------+------------+
| 8:0   | DLL_LGC_SEL_VAL6                                  | 16h     | RW         |
|       |                                                   |         |            |
|       | Only valid when SDLLW_AUTO_SWITCH=1.              |         |            |
|       |                                                   |         |            |
|       | CH6 SDLL_W direct value for legacy/L16 write path |         |            |
+-------+---------------------------------------------------+---------+------------+

6\. Appendix

## 6.1. FALL limitation

FALL register is a fictitious register, and it write to all channel register at the same time. However, there is writing limitation that it cannot write register for bit unit. When FALL register is triggered to write all channel register, 32 bits data of channel 0 register is read and change data which bit want to write. Others bits data will not be changed. FALL register write 32 bits changed data to each channel register at the same time.

Therefore, all channel register will be written as same as changed data of channel 0 register. It is possible that unchanged data of channel 0 register affect others channel register. In order to avoid overwrite case. FALL register is used to write data only for 32 bits unit.

For example, FCTL_INT_CFG \[6\] is indicated that force MTQ abort, and FALL register to write it as 1 bit. FCTL_INT_CFG \[23:16\] will be affected to change data as same as channel 0 register.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image65.png){width="5.975694444444445in" height="2.272222222222222in"}

For example, FALL register to write FCTL_INT_CFG \[6\] as 0 bit. FCTL_INT_CFG \[0\] of Channel 0 register is enable status. Therefore, channel 1 register also changed to enable status.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image66.png){width="5.959722222222222in" height="2.191666666666667in"}

## 6.2. FARG_BCF_INDEX Bug

**The Error Case**

When bad column function is enabled, DMA need the starting index of frame information (START_INDEX). START_INDEX value is according to SYSIF_FRM_PTR register as index selection as the Figure 1. However MTQ process is executed to load next MTP parameter and waits current MTP process done. It is possible that pre-load SYSIF_FRM_PTR parameter affect current MTP process parameter before START_INDEX is executed as the Figure 2.

The reason that START_INDEX does not do pipeline to latch SYSIF_FRM_PTR parameter of current MTP. Therefore, frm_ptr is used to latch SYSIF_FRM_PTR parameter and keep current MTP parameter to avoid incorrect data by next MTP.

1' LD_NEW_TRIG: current MTP is executed.

2' MT_FPU_TRIG: FPU of current MTP is triggered

3'FARG_BCF_INDEX\[SYSIF_FRM_PTR\]: index is controlled by SYSIF_FRM_PTR

4' START_INDEX: as same as FARG_BCF_INDEX

5' adr_ini: FARG_BCF_INDEX is latched by adr_ini

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image67.png){width="6.65625in" height="1.4243055555555555in"}

Figure 1

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image68.png){width="6.663888888888889in" height="1.4319444444444445in"}

Figure 2

**HW Solution**

Hardware can fix the problem by ECO. frm_ptr is used to latch SYSIF_FRM_PTR data and SYSIF_FRM_PTR index selection is changed as frm_ptr. When pre-load MTP is executed, it can avoid the case that current MTP parameter is changed by next MTP parameter as the Figure 3.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image69.png){width="6.663888888888889in" height="1.4479166666666667in"}

## 6.3. CNV_SEED (Erase Key) Bug

**The Error Case**

When flash read or write, the data pass through conversion function. Conversion function can be made to depend on the setting of HW or FW mode. HW mode will calculate PAGE key、SECTOR key. FW mode can also set PAGE key、SECTOR key and SEED key (Erase Key) directly. SEED key can be configured in MTP parameter to change conversion function. Each MTP can set SEED key according to different flash data.

In pre-load MTP process, next MTP parameter can be loaded to wait current MTP process whether it is finish or not as the Figure 1. However, it is possible that pre-load SYSIF_CNV_BLK_SED parameter of next MTP affect current MTP process parameter before CNV_SEED (Erase Key) is executed. The reason that CNV_SEED does not do pipeline to latch SYSIF_CNV_BLK_SED parameter of current MTP.

1' LD_NEW_TRIG:

> Next MTP is pre-load.

2' MT_FPU_TRIG:

> FPU of current MTP is triggered

3' SYSIF_CNV_BLK_SED:

> FCTL_SEED_INIT control register value

4' CNV_SEED (Erase Key):

Current FPU use the conversion SEED key

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image70.png){width="6.688194444444444in" height="1.2in"}

Figure 1

**HW Solution**

5'cnv_blk_sed:

SYSIF_CNV_BLK_SED is latched for current MTP

Hardware can fix the problem by ECO. The cnv_blk_sed FIFO is added to latch SYSIF_CNV_BLK_SED parameter and keep current MTP parameter to avoid incorrect data by next MTP as the Figure 2.

![](D:\AI_DEV\Output\PS3111_RegSpec_FLH_assets/media/image71.png){width="6.688194444444444in" height="1.136111111111111in"}

## 6.4. MDLL OFFSET Decision Methodology

1\. DLL_SDLL_OFFSET_N\[8:0\] as 0x00

2\. read DLL_SDLL_VAL_N value as the clock tracking value without offset data for all channel.

3.execute write and read, then compare data. PASS if compare equivalent and FAIL if compare non-equivalent.

4\. DLL_SDLL_OFFSET_N\[8:0\] as 0x100

5\. Increase DLL_SDLL_OFFSET_N\[8:0\] by 1 and go to step 3 if DLL_SDLL_OFFSET_N\[8:0\] is not equal to 0x1FF. Record all result as the following format:

Offset_value: 012\...\...\...\...AB(B+1)\...\....CD\...\...

Compare result: FFF\...\...\..... FP P P\.....P FF\.....

6.The offset value at point B and point C is the boundary offset values.

7.The golden DLL_SDLL_OFFSET_N\[8:0\]= (DLL_SDLL_OFFSET_B\[8:0\]+ DLL_SDLL_OFFSET_C\[8:0\])/2.
