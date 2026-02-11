#include "IO.h"
#include "Vardef.h"
#include "Setup.h"
#include "FTL.h"

#if PH_SQL_FUNC
#define _HEADER_ADDRMSK_  0x03
#define _ADDR_CNT_        6
#define _FTA_CNT_         5
#define _DMA_BUFFER_K_    9
#define IBF_BANK_LENGTH 5120
void NandVToolOperationSetting(void)
{
	if (gOperationSetting.B.btIsConversion) {
		gFREG[FCTLL_CHNL_SET].L &= ~(U32)BYPASS_CONV;
	}
	else {
		gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
	}
	if (gOperationSetting.B.btIsInversion) {
		gFREG[FCTLL_CHNL_SET].L &= ~(U32)BYPASS_INV;
		gFREG[FCTLL_CHNL_SET].L |= BYPASS_INV;
	}
	else {
		gFREG[FCTLL_CHNL_SET].L |= BYPASS_INV;
	}
	if (gOperationSetting.B.btIsEnableECC) {
		gFREG[FCTLL_OTHER_SET].L |= BCH_COR_EN;
		gFREG[FCTLL_LDPC_CFG].L |= LDPC_COR_EN;
		FCONL[FCONL_CRC_EN] |= CRC_32_EN;
	}
	else {
		gFREG[FCTLL_OTHER_SET].L &= ~(U32)BCH_COR_EN;
		gFREG[FCTLL_LDPC_CFG].L &= ~(U32)LDPC_COR_EN;
		FCONL[FCONL_CRC_EN] &= ~(U32)CRC_32_EN;
	}
	if (gOperationSetting.B.btIsCheckSpare) {
		gFREG[FCTLL_CMP_CFG].L |= LCA_FW_COMPARE_EN;
	}
	else {
		gFREG[FCTLL_CMP_CFG].L &= ~(U32)LCA_FW_COMPARE_EN;
	}
}

void NandVToolCmdParser(void)
{
	UBYTE* ubCmdBufPtr;
	UWORD* uwVenderFPUEntry = (UWORD*)(0x50000000 + BURNER_FIX_FPU_LENGTH);
	UWORD uwCmdBufOffset;
	UWORD uwFPUOperation = 0;
	UBYTE ubParentCode, ubChildCode;
	UBYTE ubL4Bit, ubR4Bit;
	UBYTE ubFPUOffset = 0;
	UBYTE ubDepth;
	UBYTE ubRetrySetFeature = 0;
	U32 ulTOP_TGL_TIME_CFG_BACKUP = FCONL[FCONL_TGL_TIME_CFG];
	U32 ulTOP_TGL_TIME_CFG_1_BACKUP = FCONL[FCONL_TGL_TIME_CFG_1];
	//gFREG[FCTLL_INT_CFG].L &= (~ABORT_DONE_INTR_EN); // follow FLH RegSpec 4.19 MTQ Manual Abort
	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // turn off RS
	gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
	gFREG[FCTLL_ZIP_CFG].L &= (~L4K_BADR_CONTINUOUS_MODE);
	gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);

	M_GetMTDepth(ubDepth);
	gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
	gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
	gubINTTable[ubDepth].All = 0;
	ubCmdBufPtr = gubNandVToolCmdBuffer + 3;
	uwCmdBufOffset = 0;
	UWORD uwPrintIndex;
	UBYTE ubFPUIndex;
	if (ENABLE_DEBUG_UART_PH_SQL) {
		UartString("\n\r=\n\r");
		for (uwPrintIndex = 0; ; uwPrintIndex++) {
			Uart_Tx_DataHex(ubCmdBufPtr[uwPrintIndex]);
			if (ubCmdBufPtr[uwPrintIndex] == 0xAF) {
				break;
			}
		}
	}
	while (1) {
		ubParentCode = ubCmdBufPtr[uwCmdBufOffset];
		uwCmdBufOffset++;
		ubL4Bit = ubParentCode & 0xF0;
		ubR4Bit = ubParentCode & 0x0F;
		switch (ubL4Bit) {
			case 0xC0:
				if (ubR4Bit < 0xA) {
					ubChildCode = ubCmdBufPtr[uwCmdBufOffset];
					if ((ubChildCode == 0xFF) && (Hynix || MicronFlashOnly) && (gFREG[FCTLL_FLH_SET].L & FLA_TOGGLE_MODE)) {
						ubChildCode &= 0xFC;
					}
					uwCmdBufOffset++;
					if (ubParentCode == 0xC0) {
						uwFPUOperation = 0x2000 | ubChildCode;
						uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
						ubFPUOffset++;
					}
					else if (ubParentCode == 0xC1) {
						uwFPUOperation = 0x2000 | ubChildCode;
						uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
						ubFPUOffset++;
					}
					else if (ubParentCode == 0xC2) {
						gFREG[FCTLL_PIO_CMD].B.B0 = ubChildCode;
#if TLC_BICS2
						if ((ubChildCode == 0xD4) || (ubChildCode == 0xD5)) {
							ubRetrySetFeature = 1;
						}
#elif MicronFlashOnly
						if ((ubChildCode == 0xD5) || (ubChildCode == 0xEF)) {
							ubRetrySetFeature = 1;
						}
#endif
						if (ENABLE_DEBUG_UART_PH_SQL) {
							UartString("\n\rC:");
							Uart_Tx_DataHex(ubChildCode);
						}
					}
					else if (ubParentCode == 0xC3) {
						uwFPUOperation = 0x2000 | ubChildCode;
						uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
						ubFPUOffset++;
					}
					else {
						flaLEDBlink(ubParentCode);
					}
				}
				else {
					if (ubParentCode == 0xCB) { //wait busy to ready
						U32 ulDelay;
						if (gubFLHIPNoBusyPin) {
							ulDelay = 0;
							gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;
							while (gFREG[FCTLL_PIO_DAT].B.B0 != 0xE0) {
								ulDelay++;
								if (ulDelay > 0xFFFFFF) {
									break;
								}
							}
							gulBusyTimer += ulDelay;
						}
						else {
							ulDelay = 0xFF;
							while (ulDelay > 0) {
								ulDelay--;
							}
							while ((gFREG[FCTLL_RBY_INF].L) != 0x000001FF) {
								gulBusyTimer++;
							};
							gulBusyTimer += 0xFF;
						}
						gulNandVToolTimerCntBuffer[0] = gulBusyTimer;
					}
					else if (ubParentCode == 0xCC) { //DMA Commit
						uwFPUOperation = 0xF000;
						uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
						if (ENABLE_DEBUG_UART_PH_SQL) {
							UartString("\n\rF:");
							for (ubFPUIndex = 0; ubFPUIndex <= ubFPUOffset; ubFPUIndex++) {
								UartWordHex(uwVenderFPUEntry[ubFPUIndex]);
							}
						}
						ubFPUOffset = 0;
						gFREG[FCTLL_FPU_ENTRY].L = (U32)uwVenderFPUEntry;
						gFREG[FCTLL_FPU_TRIG].L |= FPU_TRIG;
						while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
					}
					else if (ubParentCode == 0xCD) { //Delay
						UBYTE ubDelay = 0;
						ubChildCode = ubCmdBufPtr[uwCmdBufOffset];
						uwCmdBufOffset++;
						while (ubDelay < ubChildCode) {
							ubDelay++;
						}
					}
					else if (ubParentCode == 0xCE) {
						goto  LeaveCmdParser;
					}
					else if (ubParentCode == 0xCF) {
						ubChildCode = ubCmdBufPtr[uwCmdBufOffset];
						uwCmdBufOffset++;
						if (ENABLE_DEBUG_UART_PH_SQL) {
							UartString("\n\rI:");
							Uart_Tx_DataHex(ubChildCode);
						}
						if (ubChildCode) {
							Set_FlashIP_InterFace(1);//HS_MODE
						}
						else {
							Set_FlashIP_InterFace(0);
						}
					}
					else {
						flaLEDBlink(ubParentCode);
					}
				}
				break;
			case 0xA0:
				if (ubParentCode == 0xA0) {
					uwFPUOperation = 0x3500;
					uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
					ubFPUOffset++;
				}
				else if (ubParentCode == 0xA2) {
					uwFPUOperation = 0x3200;//Col
					uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
					ubFPUOffset++;
				}
				else if (ubParentCode == 0xA3) {
					uwFPUOperation = 0x3300;//Row
					uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
					ubFPUOffset++;
				}
				else if (ubParentCode == 0xAA) { //Use same adr
					uwFPUOperation = 0x3500;
					uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
					ubFPUOffset++;
				}
				else if (ubParentCode == 0xAB) { //PIO Adr
					ubChildCode = ubCmdBufPtr[uwCmdBufOffset];
					uwCmdBufOffset++;
					gFREG[FCTLL_PIO_ADR].B.B0 = ubChildCode;
					if (ENABLE_DEBUG_UART_PH_SQL) {
						UartString("\n\rA:");
						Uart_Tx_DataHex(ubChildCode);
					}
				}
				else if (ubParentCode == 0xAC) { //Increse Page
					gubNandVToolAdr[2]++;
					gFREG[FCTLL_FTA_ROW].B.B0 = gubNandVToolAdr[2];
				}
				else if (ubParentCode == 0xAD) { //Increase Block
					gubNandVToolAdr[3]++;
					gFREG[FCTLL_FTA_ROW].B.B1 = gubNandVToolAdr[3];
					if (!gubNandVToolAdr) {
						gubNandVToolAdr[4]++;
						gFREG[FCTLL_FTA_ROW].B.B2 = gubNandVToolAdr[4];
					}
				}
				else {
					flaLEDBlink(ubParentCode);
				}
				break;
			case 0xD0:
				if (ubParentCode == 0xDA) { //DMA read out size
					U32 ulZipMap = 0;
					L4KTable16B *pL4KTable;
					UBYTE ubL4KIndex;
					ubChildCode = ubCmdBufPtr[uwCmdBufOffset];
					uwCmdBufOffset++;
					UBYTE ubL4KNum;
					UBYTE ubMoveData = 0;
					//Fill DMA setting
					gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME);
					uwFPUOperation = 0x103C;//insert delay
					uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
					ubFPUOffset++;
					if (gOperationSetting.B.btIsShowECC) { //ECC will correct data
						gFREG[FCTLL_DMA_CFG].L |= (((UWORD)1) << 8);
						gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
						gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(1);
						uwFPUOperation = 0x6000;//insert Dma read
						ubL4KNum = ubChildCode >> 2;
						//Fill L4K table
						for (ubL4KIndex = 0; ubL4KIndex < ubL4KNum; ubL4KIndex++) {
							ulZipMap |= (0xFF << (ubL4KIndex * 8));
							pL4KTable = (L4KTable16B*)(HB_RETRY_L4K_TABLE_ADDR + (ubL4KIndex * Spare4kSize));
							//set L4K table info
							pL4KTable->ulL4K_LCA = 0x31111113;
							pL4KTable->Para0x04.BitMap.ulL4K_FW = 0xBB3111;
							pL4KTable->Para0x04.BitMap.ubL4K_SPRV = 0xFF;
							pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0xBB;
							pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
							pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubNandVToolDataBuffer[(ubL4KIndex * 4096)])) / 512;
							pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
							pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
						}
						gFREG[FCTLL_ZIP_MAP].L = ulZipMap;
					}
					else { //Read raw data
						if (gubFlashClock <= FLH_CLK_100MHz) { // Fix read row data bug under 100MHz
							FCONL[FCONL_TGL_TIME_CFG] &= 0x00FFFFFF;
							FCONL[FCONL_TGL_TIME_CFG] |= (TOG_DAT_PSTH_PHASE_CNT(3) | TOG_DAT_CLH_PHASE_CNT(5));

							FCONL[FCONL_TGL_TIME_CFG_1] &= 0x00FFFFFF;
							FCONL[FCONL_TGL_TIME_CFG_1] |= (TOG_DAT_PSTH_PHASE_CNT(3) | TOG_DAT_CLH_PHASE_CNT(5));
						}
						ubL4KNum =  ubChildCode * (U32)1024 / guwDMAFrameSize;
						gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubL4KNum) << 8);
						if (ENABLE_DCACHE) {
							xthal_dcache_region_invalidate((void *)gubNandVToolRawBuffer, ubL4KNum * IBF_BANK_LENGTH);
						}
						gFREG[FCTLL_RAW_DMA_ADR].L = (U32)gubNandVToolRawBuffer;
						ubMoveData = 1;

						if (Hynix) { // for hynix d3 count 1
							UBYTE ubRedundantOPCode;//HW在做 0xA000 會看0xA000之前的 8組 OP Code 第一組OP Code的bit0必須要0
							for (ubRedundantOPCode = 0; ubRedundantOPCode < 8; ubRedundantOPCode++) {
								uwFPUOperation = 0x0000;
								uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
								ubFPUOffset++;
							}
						}
						uwFPUOperation = 0xA000;//Inser Dma raw
					}
					uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
					ubFPUOffset++;
					uwFPUOperation = 0xF000;//End FPU
					uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
					if (ENABLE_DEBUG_UART_PH_SQL) {
						UartString("\n\rF:");
						for (ubFPUIndex = 0; ubFPUIndex <= ubFPUOffset; ubFPUIndex++) {
							UartWordHex(uwVenderFPUEntry[ubFPUIndex]);
						}
					}
					ubFPUOffset = 0;
					if (ENABLE_PH_SQL_DMA_HIGH_SPEED) {
						flaSwitchClock(gubFlashClockMax);
						if (ENABLE_DEBUG_UART_PH_SQL) {
							UartString("\n\rDMA C:");
							Uart_Tx_DataHex(gubFlashClockMax);
						}
					}

					//DMA read/raw
					if (gOperationSetting.B.btIsShowECC) {
						for (ubL4KIndex = 0; ubL4KIndex < ubL4KNum; ubL4KIndex++) {
							//gFREG[FCTLL_FTA_COL].W.W0 = ubL4KIndex*guwDMAFrameSize;
							gFREG[FCTLL_OTHER_SET].L &= FRAME_START_PTR_CLR;
							gFREG[FCTLL_OTHER_SET].L |= FRAME_START_PTR(ubL4KIndex);
							gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_ADDR + ubL4KIndex * Spare4kSize;
							//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
							gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
							gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
							gFREG[FCTLL_FPU_ENTRY].L = (U32)uwVenderFPUEntry;
							gFREG[FCTLL_FPU_TRIG].L |= FPU_TRIG;
							while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
							while (gubINTTable[ubDepth].All == 0); // use this to check
							if (gOperationSetting.B.btIsCheckSpare) {
								if (gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) {
									gubNandVToolECCBuffer[ubL4KIndex * 4 + 2] = 1;
								}
								else {
									gubNandVToolECCBuffer[ubL4KIndex * 4 + 2] = 0;
								}
							}
							else {
								gubNandVToolECCBuffer[ubL4KIndex * 4 + 2] = 0;
							}
							if (ENABLE_DEBUG_UART_PH_SQL) {
								UartString("\n\rTAdrR:");
								UartLongHex(gFREG[FCTLL_FARG_ROW].L);
								UartString("\n\rTAdrC:");
								UartLongHex(gFREG[FCTLL_FARG_COL].L);
							}
							if (gOperationSetting.B.btIsShowECC) {
								gubNandVToolECCBuffer[ubL4KIndex * 4 + 0] = gFREG[FCTLL_ECC_INFO].B.B0;
								gubNandVToolECCBuffer[ubL4KIndex * 4 + 1] = gFREG[FCTLL_ECC_INFO].B.B1;
								gubNandVToolECCBuffer[ubL4KIndex * 4 + 3] = 0;
							}
							if (gubINTTable[ubDepth].All & 0xF0) {
								gubINTTable[ubDepth].All = 0;
								gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN);  // no need INT for polling

								if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
									gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
								}
							}
						}
						gFREG[FCTLL_OTHER_SET].L &= FRAME_START_PTR_CLR;
					}
					else {
						//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
						gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
						gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
						gFREG[FCTLL_FPU_ENTRY].L = (U32)uwVenderFPUEntry;
						gFREG[FCTLL_FPU_TRIG].L |= FPU_TRIG;
						while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
						while (gubINTTable[ubDepth].All == 0); // use this to check
						if (gubINTTable[ubDepth].All & 0xF0) {
							flaLEDBlink(gubINTTable[ubDepth].All);
						}
						if (gubFlashClock <= FLH_CLK_100MHz) { // Fix read row data bug under 100MHz
							FCONL[FCONL_TGL_TIME_CFG] = ulTOP_TGL_TIME_CFG_BACKUP;
							FCONL[FCONL_TGL_TIME_CFG_1] = ulTOP_TGL_TIME_CFG_1_BACKUP;
						}
					}
					if (ENABLE_PH_SQL_DMA_HIGH_SPEED) {
						flaSwitchClock(gubFlashClock);
						if (ENABLE_DEBUG_UART_PH_SQL) {
							UartString("\n\rDMA_D_C:");
							Uart_Tx_DataHex(gubFlashClock);
						}
					}
					if (ubMoveData) {
						for (ubL4KIndex = 0; ubL4KIndex < ubL4KNum; ubL4KIndex++) {
							if (guwDMAFrameSize & 31) { //not align 32 bytes
								memcpy((void *)(&gubNandVToolDataBuffer[ubL4KIndex * guwDMAFrameSize]), (void *)(&gubNandVToolRawBuffer[ubL4KIndex * IBF_BANK_LENGTH]), guwDMAFrameSize);
							}
							else {
								mDMAC_COPY((U32)(&gubNandVToolRawBuffer[ubL4KIndex * IBF_BANK_LENGTH]), (U32)(&gubNandVToolDataBuffer[ubL4KIndex * guwDMAFrameSize]), guwDMAFrameSize, 0, 0, 0, 0, 1);
							}
						}
#if (MicronFlashOnly||Hynix)
						if (gulNandPhysicalPageSize > (ubL4KIndex * guwDMAFrameSize)) {
							// phsical page size is larger than total DMA frame size, read out the remain part by PIO
							U32 ulRemainByte = gulNandPhysicalPageSize - (ubL4KIndex * guwDMAFrameSize);
							U32 ulTransferCnt = ulRemainByte;
							U32 ulTansferIndex;
							UBYTE* ubNandTempBuffer = (UBYTE*)(gubNandVToolRawBuffer);
							UWORD* uwNandTempBuffer = (UWORD*)(gubNandVToolRawBuffer);

							if (guoFlashUseType.B.btToggle || guoFlashUseType.B.btONFI) {
								ulTransferCnt >>= 1;
							}

							// change column address
							gFREG[FCTLL_PIO_CMD].B.B0 = 0x05;
							gFREG[FCTLL_PIO_ADR].B.B0 = (ubL4KIndex * guwDMAFrameSize) & 0xFF;
							gFREG[FCTLL_PIO_ADR].B.B0 = ((ubL4KIndex * guwDMAFrameSize) >> 8) & 0xFF;
							gFREG[FCTLL_PIO_CMD].B.B0 = 0xE0;

							// check read
							idle(512);
							//gFREG[FCTLL_PIO_CMD].L = 0x70;
							//while (gFREG[FCTLL_PIO_DAT].B.B0 != 0xE0) {
							//}

							// read out remain data
							for (ulTansferIndex = 0; ulTansferIndex < ulTransferCnt; ulTansferIndex++) {
								gFREG[FCTLL_HS_MODE].L &= ~(PIO_DAT_FIRST | PIO_DAT_LAST);
								if (ulTansferIndex == 0) {
									gFREG[FCTLL_HS_MODE].L |= PIO_DAT_FIRST;
								}
								if (ulTansferIndex == (ulTransferCnt - 1)) {
									gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;
								}
								// here using gubNandVToolRawBuffer
								if (guoFlashUseType.B.btToggle || guoFlashUseType.B.btONFI) {
									uwNandTempBuffer[ulTansferIndex] = gFREG[FCTLL_PIO_DAT].W.W0;
								}
								else {
									ubNandTempBuffer[ulTansferIndex] = gFREG[FCTLL_PIO_DAT].B.B0;
								}
							}
							gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);

							// copy remain data to gubNandVToolSequencialRawBuffer
							if ((ulRemainByte & 31) || (guwDMAFrameSize & 31)) { //not align 32 bytes
								memcpy((void *)(&gubNandVToolDataBuffer[ubL4KIndex * guwDMAFrameSize]), (void*)gubNandVToolRawBuffer, ulRemainByte);
							}
							else {
								mDMAC_COPY((U32)gubNandVToolRawBuffer, (U32)(&gubNandVToolDataBuffer[ubL4KIndex * guwDMAFrameSize]), ulRemainByte, 0, 0, 0, 0, 1);
							}
						}
#endif
					}
				}
				else if (ubParentCode == 0xDB) {
					ubChildCode = ubCmdBufPtr[uwCmdBufOffset];
					uwCmdBufOffset++;
					if (ENABLE_DEBUG_UART_PH_SQL) {
						UartString("\n\rR:");
					}
					UBYTE ubTransferCnt;
					UBYTE ubTansferIndex;
					UWORD uwData;
					ubTransferCnt = ubChildCode;
					U32 ulFLHInterfaceTemp = gFREG[FCTLL_FLH_SET].L;
					if ((!ubRetrySetFeature) && (!Hynix)) {
						gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
					}
					for (ubTansferIndex = 0; ubTansferIndex < ubTransferCnt; ubTansferIndex++) {
						gFREG[FCTLL_HS_MODE].L &= ~(PIO_DAT_FIRST | PIO_DAT_LAST);
						if (ubTansferIndex == 0) {
							gFREG[FCTLL_HS_MODE].L |= PIO_DAT_FIRST;
						}
						if (ubTansferIndex == (ubTransferCnt - 1)) {
							gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;
						}
						if (gFREG[FCTLL_FLH_SET].L & FLA_TOGGLE_MODE) {
							uwData = gFREG[FCTLL_PIO_DAT].W.W0;
							if (ENABLE_DEBUG_UART_PH_SQL) {
								UartWordHex(uwData);
								UartString("  ");
							}
						}
						else {
							uwData = gFREG[FCTLL_PIO_DAT].B.B0;
							if (ENABLE_DEBUG_UART_PH_SQL) {
								Uart_Tx_DataHex((UBYTE)uwData);
								UartString("  ");
							}
						}
						gubNandVToolDataBuffer[ubTansferIndex] = (UBYTE)uwData;//gFREG[FCTLL_PIO_DAT].B.B0;
					}
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					if ((!ubRetrySetFeature) && (!Hynix)) {
						gFREG[FCTLL_FLH_SET].L = ulFLHInterfaceTemp;
					}
				}
				else if (ubParentCode == 0xDC) {
					L4KTable16B *pL4KTable;
					UBYTE ubL4KIndex;
					UBYTE ubL4KNum;
					U32 ulTransferCnt;
					if (gOperationSetting.B.btIsShowECC) {
						ubL4KNum = HB[HB_LBA_H] >> 2;
						//Fill DMA setting
						gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME);
						gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubL4KNum) << 8);
						gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
						gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubL4KNum);
						//Fill L4K table
						for (ubL4KIndex = 0; ubL4KIndex < ubL4KNum; ubL4KIndex++) {
							pL4KTable = (L4KTable16B*)(HB_RETRY_L4K_TABLE_ADDR + (ubL4KIndex * Spare4kSize));
							//set L4K table info
							pL4KTable->ulL4K_LCA = 0x31111113;
							pL4KTable->Para0x04.BitMap.ulL4K_FW = 0xBB3111;
							pL4KTable->Para0x04.BitMap.ubL4K_SPRV = 0xFF;
							pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0xBB;
							pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
							pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubNandVToolDataBuffer[(ubL4KIndex * 4096)])) / 512;
							pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 7;
							pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
						}
						gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_ADDR;
						//DMA write
						uwFPUOperation = 0x103C;//insert delay
						uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
						ubFPUOffset++;
						uwFPUOperation = 0x6100;//insert Dma write
						uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
						ubFPUOffset++;
						uwFPUOperation = 0xF000;//End FPU
						if (ENABLE_DEBUG_UART_PH_SQL) {
							UartString("\n\rF:");
							for (ubFPUIndex = 0; ubFPUIndex <= ubFPUOffset; ubFPUIndex++) {
								UartWordHex(uwVenderFPUEntry[ubFPUIndex]);
							}
						}
						uwVenderFPUEntry[ubFPUOffset] =  uwFPUOperation;
						ubFPUOffset = 0;
						//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
						gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
						gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
						gFREG[FCTLL_FPU_ENTRY].L = (U32)uwVenderFPUEntry;
						gFREG[FCTLL_FPU_TRIG].L |= FPU_TRIG;
						while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
						while (gubINTTable[ubDepth].All == 0); // use this to check
						if (gubINTTable[ubDepth].All & 0xF0) {
							flaLEDBlink(gubINTTable[ubDepth].All);
						}
					}
					else {
						U32 ulTansferIndex;
						if (guoFlashUseType.B.btToggle || guoFlashUseType.B.btONFI) {
							ulTransferCnt = (HB[HB_LBA_H] * (U32)1024) / 2;
						}
						else {
							ulTransferCnt = (HB[HB_LBA_H] * (U32)1024);
						}
						for (ulTansferIndex = 0; ulTansferIndex < ulTransferCnt; ulTansferIndex++) {
							gFREG[FCTLL_HS_MODE].L &= ~(PIO_DAT_FIRST | PIO_DAT_LAST);
							if (ulTansferIndex == 0) {
								gFREG[FCTLL_HS_MODE].L |= PIO_DAT_FIRST;
							}
							if (ulTansferIndex == (ulTransferCnt - 1)) {
								gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;
							}
							if (guoFlashUseType.B.btToggle || guoFlashUseType.B.btONFI) {
								gFREG[FCTLL_PIO_DAT].W.W0 = guwNandVToolDataBuffer[ulTansferIndex];
							}
							else {
								gFREG[FCTLL_PIO_DAT].B.B0 = gubNandVToolDataBuffer[ulTansferIndex];
							}
						}
						gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					}
				}
				else if (ubParentCode == 0xDE) {
					ubChildCode = ubCmdBufPtr[uwCmdBufOffset];
					uwCmdBufOffset++;
					U32 ulFLHInterfaceTemp = gFREG[FCTLL_FLH_SET].L;
					if ((!ubRetrySetFeature) && (!Hynix)) {
						gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
					}
					if (ENABLE_DEBUG_UART_PH_SQL) {
						UartString("\n\rW:");

					}
					if (gFREG[FCTLL_FLH_SET].L & FLA_TOGGLE_MODE) {
						UWORD uwData;
						uwData = ((UWORD)ubChildCode << 8) | ubChildCode;
						gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
						if (ENABLE_DEBUG_UART_PH_SQL) {
							UartWordHex(uwData);
						}
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = ubChildCode;
						if (ENABLE_DEBUG_UART_PH_SQL) {
							Uart_Tx_DataHex(ubChildCode);
						}
					}
					if ((!ubRetrySetFeature) && (!Hynix)) {
						gFREG[FCTLL_FLH_SET].L = ulFLHInterfaceTemp;
					}

				}
				else {
					flaLEDBlink(ubParentCode);
				}
				break;
			case 0xB0://Bad Column Setting
				break;
		}
	}
LeaveCmdParser:
	M_AddMTDepth(ubDepth);
	return;
}

void NandVToolPreSettingParser(void)
{
	UBYTE* ubCmdBufPtr;
	UBYTE* ubAdrBufPtr;
	UWORD uwCmdBufOffset = 0;
	UBYTE ubAdrBufOffset = 0;
	UBYTE ubL4Bit, ubR4Bit;
	UBYTE ubAddrToken = 0;
	M_SetFlagSettingNone();
	ubCmdBufPtr = gubNandVToolCmdBuffer;
	if (ubCmdBufPtr[uwCmdBufOffset] == 0xCA) { //0xCA command access token
		ubAdrBufPtr = gubNandVToolCmdBuffer + ubCmdBufPtr[++uwCmdBufOffset] + 3;
		gOperationSetting.All = ubCmdBufPtr[++uwCmdBufOffset];
		NandVToolOperationSetting();
		//Handle Adr
		do {
			ubL4Bit = ubAdrBufPtr[ubAdrBufOffset] & 0xF0;
			ubR4Bit = ubAdrBufPtr[ubAdrBufOffset] & 0x0F;
			switch (ubL4Bit) {
				case 0xA0:
					ubAddrToken++;
					if (ubR4Bit == 0) {
						memcpy(&gubNandVToolAdr[0] , &ubAdrBufPtr[++ubAdrBufOffset], _ADDR_CNT_);
						ubAdrBufOffset = ubAdrBufOffset + _ADDR_CNT_;
					}
					else if (ubR4Bit == 0xA) {
						//Use same address
						ubAdrBufOffset++;
					}
					else if (ubR4Bit == 0xC) {
						gubNandVToolAdr[2] = (gubNandVToolAdr[2] + 1)&guwPagesPerUnit;
						ubAdrBufOffset++;
					}
					else if (ubR4Bit == 0xD) { //Not Sure
						gubNandVToolAdr[3] = gubNandVToolAdr[3] + 1;
						ubAdrBufOffset++;
					}
					else {
						gubNandVToolAdr[--ubR4Bit]++;
						ubAdrBufOffset++;
					}
					break;
				case 0xB0:
					ubAdrBufOffset++;
					break;

			}
			if (ubAddrToken == gOperationSetting.B.btAdrTokenCnt) {
				if (ubAdrBufPtr[ubAdrBufOffset] == 0xAE) {
					gFREG[FCTLL_FTA_COL].B.B0 = gubNandVToolAdr[0];
					gFREG[FCTLL_FTA_COL].B.B1 = gubNandVToolAdr[1];
					gFREG[FCTLL_FTA_ROW].B.B0 = gubNandVToolAdr[2];
					gFREG[FCTLL_FTA_ROW].B.B1 = gubNandVToolAdr[3];
					gFREG[FCTLL_FTA_ROW].B.B2 = gubNandVToolAdr[4];
					if (ENABLE_DEBUG_UART_PH_SQL) {
						UBYTE ubADRIndex;
						UartString("\n\rAdr:");
						for (ubADRIndex = 0; ubADRIndex < 5; ubADRIndex++) {
							Uart_Tx_DataHex(gubNandVToolAdr[ubADRIndex]);
						}
					}
					ubAdrBufOffset++;
					NandVToolCmdParser();
				}
			}
		}
		while (ubAdrBufPtr[ubAdrBufOffset] != 0xAF);
	}
	else {

	}
}

void Set_FlashIP_InterFace(UBYTE ubMode)
{
	if (ubMode == 0) {
		gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
		guoFlashUseType.B.btToggle = 0;
		guoFlashUseType.B.btONFI = 0;
	}
	else if (ubMode == 1) {
		if (guoFlashDefaultType.B.btToggle) {
			gFREG[FCTLL_FLH_SET].L |= FLA_TOGGLE_MODE;
			guoFlashUseType.B.btToggle = 1;
			guoFlashUseType.B.btONFI = 0;
		}
		else if (guoFlashDefaultType.B.btONFI) {
			gFREG[FCTLL_FLH_SET].L |= FLA_ONFI_MODE;
			guoFlashUseType.B.btToggle = 0;
			guoFlashUseType.B.btONFI = 1;
		}
	}
}

void NandVToolSetFLHIP(void)
{
	UWORD uwLDPCParitySize = 0xFF;
	UBYTE ubChannel;
	if (gubNandVToolCmdBuffer[0] == 0) {
		for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
			gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
			for (gulTemp = 0; gulTemp < 1000;) {
				gulTemp++;
			}
			gFREG[FCTLL_FDIV_CFG].L = 0;
		}
	}
	else if (gubNandVToolCmdBuffer[0] == 0xFF) {
		//Dont care
	}
	else {
		for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
			gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(gubNandVToolCmdBuffer[0] & 0xF));
			for (gulTemp = 0; gulTemp < 1000;) {
				gulTemp++;
			}
		}
	}

	switch (gubNandVToolCmdBuffer[1]) {
		case 0xFF:
			break;
		case 0:
			FCONL[FCONL_PAGE_CFG] &= FCONL_CLR_PAGE_CFG;
			FCONL[FCONL_PAGE_CFG] |= PHYSICAL_4KPAGE;
			break;
		case 1:
			FCONL[FCONL_PAGE_CFG] &= FCONL_CLR_PAGE_CFG;
			FCONL[FCONL_PAGE_CFG] |= PHYSICAL_8KPAGE;
			break;
		case 2:
			FCONL[FCONL_PAGE_CFG] &= FCONL_CLR_PAGE_CFG;
			FCONL[FCONL_PAGE_CFG] |= PHYSICAL_16KPAGE;
			break;
	}
	switch (gubNandVToolCmdBuffer[2]) {
		case 0xFF:
			break;
		case LDPC_4K_MODE0:
			FCONL[FCONL_LDPC_CFG] &= CLR_LDPC_MODE;
			FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);
			uwLDPCParitySize = 496;
			break;
		case LDPC_4K_MODE1:
			FCONL[FCONL_LDPC_CFG] &= CLR_LDPC_MODE;
			FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE1);
			uwLDPCParitySize = 440;
			break;
		case LDPC_4K_MODE2:
			FCONL[FCONL_LDPC_CFG] &= CLR_LDPC_MODE;
			FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE2);
			uwLDPCParitySize = 420;
			break;
		case LDPC_4K_MODE3:
			FCONL[FCONL_LDPC_CFG] &= CLR_LDPC_MODE;
			FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE3);
			uwLDPCParitySize = 360;
			break;
		case LDPC_4K_MODE4:
			FCONL[FCONL_LDPC_CFG] &= CLR_LDPC_MODE;
			FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE4);
			uwLDPCParitySize = 272;
			break;
	}
	if (uwLDPCParitySize == 0xFF) {
		//Dont care
	}
	else {
		guwDMAFrameSize = 4096 + 16 + 32 + uwLDPCParitySize;
	}
}

void NandVToolGetFLHIP(void)
{
	UWORD uwLDPCParitySize;
	U32 ulPageSizeSetting;
	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (0 * 0x200));
	if (gFREG[FCTLL_FDIV_CFG].L == 0) {
		gubNandVToolCmdBuffer[0] = 0;
	}
	else {
		gubNandVToolCmdBuffer[0] = gFREG[FCTLL_FDIV_CFG].B.B0 & 0x3F;
	}
	ulPageSizeSetting = FCONL[FCONL_PAGE_CFG] & 0x3;
	switch (ulPageSizeSetting) {
		case PHYSICAL_4KPAGE:
			gubNandVToolCmdBuffer[1] = 0;//4K
			break;
		case PHYSICAL_8KPAGE:
			gubNandVToolCmdBuffer[1] = 1;//8K
			break;
		case PHYSICAL_16KPAGE:
			gubNandVToolCmdBuffer[1] = 2;//16K
			break;
	}
	uwLDPCParitySize = guwDMAFrameSize - 16 - 32 - 4096;
	switch (uwLDPCParitySize) {
		case 496:
			gubNandVToolCmdBuffer[2] = 0;
			break;
		case 440:
			gubNandVToolCmdBuffer[2] = 1;
			break;
		case 420:
			gubNandVToolCmdBuffer[2] = 2;
			break;
		case 360:
			gubNandVToolCmdBuffer[2] = 3;
			break;
		case 272:
			gubNandVToolCmdBuffer[2] = 4;
			break;
	}
	if (gubIsPHSQLClockSet == FALSE) {
		flaSetFlashMode();
		flaSet_ODT_DRIVE();
		if (ENABLE_PH_SQL_DMA_HIGH_SPEED) {
			if (gubFlashMode == FLH_MODE_TOGGLE2) {
				gubFlashClockMax = FLH_CLK_200MHz;
			}
			else {
				gubFlashClockMax = gubFlashClock;
			}
		}
		if (gubFlashClock > FLH_CLK_100MHz) {
			gubFlashClock = FLH_CLK_100MHz;
		}
		flaSwitchClock(gubFlashClock);
		gubIsPHSQLClockSet = TRUE;
		if (gFREG[FCTLL_RBY_INF].L == 0) {
			gubFLHIPNoBusyPin = TRUE;
		}
	}
}
#endif
