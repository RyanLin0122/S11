
#include "setup.h"
#include "Vardef.h"
#include "ftl.h"
#include "io.h"
#include <string.h>
#include <stdlib.h>

void FlushCache(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	HandleStopRW(0);

	if (ENABLE_WRITEPROTECT && (gubIsWriteProtect & (BIT_FLASH_INIT_FAIL_WRITEPROTECT | BIT_ERR_UNIT_WRITEPROTECT))) {
		return;
	}

	if (gubFlushMode >= BYTE_STANDBY_FLUSH) {
		gubSavingEveryThingBeforeReset = 1;
		gubWearLevelingCount = MAX_WearLeveling_Num;
		while (gubSavingEveryThingBeforeReset) {
			M_SwitchTask();
		}

	Mark_FlushCache_End:
		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}
		gubFlushMode = 0;
	}
	else if (gubFlushMode == BYTE_SATACMD_FLUSH) {
		gubFlushCacheDoing = 1;
		while (gubFlushCacheDoing) {
			M_SwitchTask();
		}
		gubFlushMode = 0;
	}
#endif
}

void FlushCacheCmd()
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect) {
			gubErrorCode = SET_ABRT;
			return;
		}
	}

	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || (gubLBA48BitCmdSupport == 0 && HB[HB_COMMAND] == 0xEA)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}
	if (gubGUARANTEE_FLUSH) {
		if (ENABLE_DEBUG_UART) {
			UartString("\nGFlush");
		}
		gubFlushMode = BYTE_SATACMD_FLUSH;
	}
	else if (gubSWIFTFLUSH) {
		if (ENABLE_DEBUG_UART) {
			UartString("\nSFlush");
		}
		return;
	}
	else {
		if (ENABLE_DEBUG_UART) {
			UartString("\nCFlush");
		}
		gubFlushCache_StopBG = 1;
	}
	FlushCache();
#endif
}
void WriteSectorFUA(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubWriteCacheEnable_Temp;

	if (((IDT.W84 & BIT6) == 0)
	        || (AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || ((gubCmdCode == 0xCE) && ((IDT.W59 & BIT8) == 0))
	        || ((gubCmdCode == 0xCE) && ((IDT.W59 & 0x00FF) == 0))
	        || (gubLBA48BitCmdSupport == 0)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	ubWriteCacheEnable_Temp = gubWriteCacheEnable;
	gubWriteCacheEnable = 0;

	WriteSectors();
	HandleStopRW(0);

	gubWriteCacheEnable = ubWriteCacheEnable_Temp;
#endif
}

void ReadVerify()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || ((gubLBA48BitCmdSupport == 0) && (HB[HB_COMMAND] == 0x42))
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	//RMA_FLOW
	if (gubIsWriteProtect & (BIT_FLASH_INIT_FAIL_WRITEPROTECT | BIT_ERR_UNIT_WRITEPROTECT)) {
		return;
	}


	CheckID();
	if (gubErrorCode) {
		return;
	}

	HandleStopRW(0);
	M_SetFlagSettingNone();
	BufferQueue_t *BQ;
	U32 ulLBA;
	U32 ulSectorCnt;
	U32 uli;
	UBYTE ubScanBySector = 0;

	ulLBA = gulLBA.All;
	ulSectorCnt = gulSectorCnt;

	//find the unc vir4k index
	gulReadFail_4KMAP = 0;
	gubFTL_Only_HandleBQ = 1;
	U32 ulCurrentVir4KIndex;
	U32 ulCurrentDataMask;
	U32 ulCurrentLBA;
	U32 ulStartVir4KIndex = (ulLBA >> SectorsPer4KLog);
	U32 ulEndVir4KIndex = ((ulLBA + ulSectorCnt - 1) >> SectorsPer4KLog);

	if (ENABLE_DEBUG_UART_ATA_CMD) {
		UartString("\r\nulLBA: ");
		UartLongHex(ulLBA);
		UartString(" ulSectorCnt: ");
		UartLongHex(ulSectorCnt);
		UartString(" ulStartVir4KIndex: ");
		UartLongHex(ulStartVir4KIndex);
		UartString(" ulEndVir4KIndex: ");
		UartLongHex(ulEndVir4KIndex);
	}

	for (ulCurrentVir4KIndex = ulStartVir4KIndex; ulCurrentVir4KIndex <= ulEndVir4KIndex; ulCurrentVir4KIndex++) {
		ulCurrentDataMask = 0xFF;
		if (ulCurrentVir4KIndex == ulStartVir4KIndex) {
			if (ulLBA % SectorsPer4K) {
				ulCurrentDataMask = (~((BIT0 << (ulLBA % SectorsPer4K)) - 1)) & ulCurrentDataMask;
			}
		}

		if (ulCurrentVir4KIndex == ulEndVir4KIndex ) {
			if ((ulLBA + ulSectorCnt) % SectorsPer4K) {
				ulCurrentDataMask = ((BIT0 << ((ulLBA + ulSectorCnt) % SectorsPer4K)) - 1) & ulCurrentDataMask;
			}
		}
		if (ENABLE_DEBUG_UART_ATA_CMD) {
			UartString("\r\nCurrent4K: ");
			UartLongHex(ulCurrentVir4KIndex);
			UartString(" CurrentMask: ");
			UartLongHex(ulCurrentDataMask);
		}

		while (gBQI.ubLinkNum >= BQ_NUM) {
#if  ENABLE_READ_CHECK_FUNCTION
			if (gulNCQRDCmdTriggerCnt) {
				if (DCCMSATAB[SATAB_RD_CNT]) {
					if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
						guwForceRead_Info->uwQueueQuota = 1;
					}
				}
			}
#endif
			M_DoneEC_Check(1);
		}
		gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
		BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
		BQ->btWrite = 0;
		BQ->ulVir4kIndex = ulCurrentVir4KIndex;
		BQ->ulDataMask = ulCurrentDataMask;
		BQ->uwBufferIndex = 0;
		BQ->ubRBQIIndex = 0xFF;
		BQ->ubDataInDRamMap = 0;
		BQ->btPreRead = 0;
		BQ->btPartialFromFlash = 0;
		BQ->ub4kNum = 1;
		BQ->ulSpareValid = 0;
		BQ->ubThisBQisBG = 1;  // Non-data cmd
		M_AddToBufferQueue(gBQI.ubLinkActive);
		while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}

		if (gulReadFail_4KMAP) {
			gulReadFail_4KMAP = 0;
			ubScanBySector = 1;
			break;
		}
	}

	//find the unc LBA in that UNC vir4K.
	if (ubScanBySector) {
		gulReadFail_4KMAP = 0;
		for (uli = 0; uli < SectorsPer4K; uli++) {
			ulCurrentLBA = ((ulCurrentVir4KIndex << SectorsPer4KLog) + uli);
			if ((ulCurrentLBA >= ulLBA) && (ulCurrentLBA < (ulLBA + ulSectorCnt))) {
				if (ENABLE_DEBUG_UART_ATA_CMD) {
					UartString("\r\nRead LBA: ");
					UartLongHex(uli);
				}

				while (gBQI.ubLinkNum >= BQ_NUM) {
#if  ENABLE_READ_CHECK_FUNCTION
					if (gulNCQRDCmdTriggerCnt) {
						if (DCCMSATAB[SATAB_RD_CNT]) {
							if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
								guwForceRead_Info->uwQueueQuota = 1;
							}
						}
					}
#endif
					M_DoneEC_Check(1);
				}
				gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
				BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
				BQ->btWrite = 0;
				BQ->ulVir4kIndex = ulCurrentVir4KIndex;
				BQ->ulDataMask = BIT0 << uli;
				BQ->uwBufferIndex = 0;
				BQ->ubRBQIIndex = 0xFF;
				BQ->ubDataInDRamMap = 0;
				BQ->btPreRead = 0;
				BQ->btPartialFromFlash = 0;
				BQ->ub4kNum = 1;
				BQ->ulSpareValid = 0;
				BQ->ubThisBQisBG = 1;  // Non-data cmd
				M_AddToBufferQueue(gBQI.ubLinkActive);
				while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
					M_SwitchTask();
				}

				if (gulReadFail_4KMAP) {
					gulUNCLBA = ulCurrentLBA;
					gubErrorCode = SET_UNC;
					gulReadFail_4KMAP = 0;
					VT->gulNonDataUNCCnt++;

					if (ENABLE_DEBUG_UART_ATA_CMD) {
						UartString("\r\nUNC LBA: ");
						UartLongHex(ulCurrentLBA);
					}
					break;
				}
			}
		}
	}
	mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 0);//強迫DMAC 全部做完
	gubFTL_Only_HandleBQ = 0;
	M_SetFlagSettingSHF();
}




void WriteUncorrectable(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();


	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect) {
			gubErrorCode = SET_ABRT;
			return;
		}
	}

	if (((IDT.W119 & BIT2) == 0)
	        || ((IDT.W120 & BIT2) == 0)
	        || (AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || (gubLBA48BitCmdSupport == 0)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}


	gubWUNCDoing = 1;
	switch (HB[HB_FEATURE]) {
		case 0x55:
			break;

			//case 0x5A:
			//	break;

			//case 0xA5:
			//	break;

		case 0xAA:
			break;

		default:
			gubErrorCode = SET_ABRT;
			//UartString("\nABRT");
			return;
	}



	CheckID();
	if (gubErrorCode) {
		return;
	}

	HandleStopRW(0);
	M_SetFlagSettingNone();



	U32 WUNC_LBA = 0;
	U32 WUNC_SectorCnt = 0;

	WUNC_LBA = gulLBA.All;
	WUNC_SectorCnt = gulSectorCnt;


	U32 ulVir4kIndex_Start = (WUNC_LBA) >> SectorsPer4KLog;
	U32 ulVir4kIndex_End = (WUNC_LBA + WUNC_SectorCnt ) >> SectorsPer4KLog;

	U32 ulPlane_Start = (WUNC_LBA) >> gubSectorsPerPlaneLog;
	U32 ulPlane_End = (WUNC_LBA + WUNC_SectorCnt) >> gubSectorsPerPlaneLog;

	U32 ulPlane_Count = 0;
	U32 ulSV_BitMap, ulSV_BitMapStart, ulSV_Len;
	U32 ulTemp = 0;
	UBYTE ubi = 0;
	UBYTE ubW4kNum = gub4kEntrysPerPlane;


	if (((WUNC_LBA + WUNC_SectorCnt ) & SectorsPer4KMask) == 0) {
		ulVir4kIndex_End = ulVir4kIndex_End - 1;
	}
	if (((WUNC_LBA + WUNC_SectorCnt ) & gubSectorsPerPlaneMask) == 0) {
		ulPlane_End = ulPlane_End - 1;
	}


	// 一次處理一個plane(16K), 並且要對齊16K
	for (ulPlane_Count = ulPlane_Start; ulPlane_Count <= ulPlane_End; ulPlane_Count++) {

		ReadBQInformation_t *RBQI;
		BufferQueue_t *BQ;
		U32 ulSpareValid_Before, ulSpareValid_After;
		UBYTE ubgBQI_Link_Index;

		if (((ulPlane_Count + 1)* gub4kEntrysPerPlane) > ulVir4kIndex_End) {
			ubW4kNum = ulVir4kIndex_End - (ulPlane_Count * gub4kEntrysPerPlane) + 1;
		}

		// 填R BQ
		while (gBQI.ubLinkNum >= BQ_NUM) M_SwitchTask();
		gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
		BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
		ubgBQI_Link_Index = gBQI.ubLinkActive;
		BQ->btWrite = 1;
		BQ->ulVir4kIndex = ulPlane_Count * gub4kEntrysPerPlane;
		BQ->ubDataInDRamMap = 0;
		BQ->ulDataMask = 0xFFFFFFFF;
		BQ->uwBufferIndex = 0;
		BQ->ubRBQIIndex = 0xFF;
		BQ->btPartialFromFlash = 0;
		BQ->btNeedLoadAlign = 1;
		BQ->btSpecialData = 0;
		BQ->btNeedCheckSpecialData = 0;
		BQ->btFlushWBQ = 0;
		BQ->ub4kNum =  ubW4kNum;
		BQ->ulSpareValid = 0;
		BQ->ubThisBQisBG = 1;  // Non-data cmd
		M_AddToBufferQueue(gBQI.ubLinkActive);

		// Read cmd: check BQ, FQ done
		while (BQ->ubRBQIIndex == 0xFF) {
			M_SwitchTask();
		}
		while ((BQ->ubRBQIIndex) != 0xFF) {
			RBQI = &gRBQI[BQ->ubRBQIIndex];

			if (RBQI->ubPhase == BYTE_BQR_BQ_Done)
				break;
			M_SwitchTask();
		}
		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}


		// change
		ulSpareValid_Before = gBQI.BufferQueue[ubgBQI_Link_Index].ulSpareValid;
		ulSpareValid_After = 0;

		for (ubi = 0; ubi < gub4kEntrysPerPlane; ubi++) {

			U32 SpareValid_Each = (ulSpareValid_Before & ((0xFF) << (ubi << 3))) >> (ubi << 3); // ubi*8
			U32 ulVir4kIndex_Each = (ulPlane_Count * gub4kEntrysPerPlane)  + ubi;

			if ((ulVir4kIndex_Each >= ulVir4kIndex_Start) && (ulVir4kIndex_Each <= ulVir4kIndex_End)) {
				ulSV_BitMap = 0;

				if (ulVir4kIndex_Each == ulVir4kIndex_Start) {
					ulSV_BitMapStart = (WUNC_LBA & SectorsPer4KMask);
					if (ulVir4kIndex_Start == ulVir4kIndex_End) {
						ulSV_Len = WUNC_SectorCnt;
					}
					else {
						ulSV_Len = SectorsPer4K - ulSV_BitMapStart;
					}
				}
				else if (ulVir4kIndex_Each == ulVir4kIndex_End) {
					ulSV_BitMapStart = 0;
					ulSV_Len = ((WUNC_LBA + WUNC_SectorCnt) & SectorsPer4KMask);
					if (ulSV_Len == 0) {
						ulSV_Len = 8;
					}
				}
				else {
					ulSV_BitMapStart = 0;
					ulSV_Len = SectorsPer4K;
				}

				for (ulTemp = 0; ulTemp < ulSV_Len; ulTemp++) {
					ulSV_BitMap |= (BIT0 << (ulSV_BitMapStart + ulTemp));
				}

				ulSV_BitMap = ((~ulSV_BitMap) & (0xFF));
				ulSpareValid_After = ulSpareValid_After | ((SpareValid_Each & ulSV_BitMap) << (ubi << 3));
			}
			else {
				ulSpareValid_After = ulSpareValid_After | (SpareValid_Each << (ubi << 3));
			}
		}

		gBQI.BufferQueue[ubgBQI_Link_Index].ulSpareValid = ulSpareValid_After;


		// 填Write BQ
		while ((gBQI.ubLinkNum + 1) >= BQ_NUM) M_SwitchTask();
		gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
		BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
		ubgBQI_Link_Index = gBQI.ubLinkActive;

		BQ->btWrite = 1;
		BQ->ulVir4kIndex =  ulPlane_Count * gub4kEntrysPerPlane;
		BQ->ubDataInDRamMap = 0;
		BQ->ulDataMask = 0;
		BQ->uwBufferIndex = 0;
		BQ->ubRBQIIndex = 0xFF;
		BQ->btPartialFromFlash = 0;
		BQ->btNeedLoadAlign = 0;
		BQ->btSpecialData = 0;
		BQ->btNeedCheckSpecialData = 0;
		BQ->btFlushWBQ = 0;
		BQ->ub4kNum = ubW4kNum;
		BQ->ulSpareValid = 0;
		for (ubi = 0; ubi < ubW4kNum; ubi++) {
			BQ->ubZcode[ubi] = 7;
		}
		BQ->ulSectorCnt  = ubW4kNum * 7;
		BQ->ubThisBQisBG = 1;  // Non-data cmd
		M_AddToBufferQueue(gBQI.ubLinkActive);
		gBQI.ubWLinkNum++;

		// Write cmd: check FQ done
		gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;


		while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber ) {
			M_SwitchTask();
		}
		gBQI.ulHostWriteWaitBufferFlag = 0;
	}

	mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 0);//強迫DMAC 全部做完
	M_SetFlagSettingSHF();
	gubWUNCDoing = 0;

#endif
	return;
}

#if ENABLE_OLD_TRIM
void Trim(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (gubSSD_Init_State.B.btCleanRAMForWriteDone == 0) {
		ftlClearRAMForGR(gulStartClearGRAddress, 1);
	}

	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect) {
			gubErrorCode = SET_ABRT;
			return;
		}
	}

	if ((gubTrimSupport == 0)
	        || (AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || (AtaCfg.uwDCOState & SET_DCO_TRIM)
	        || ((HW[HW_FEATURE] & CHK_BIT0) == 0)
	        || (HW[HW_SECTOR_CNT] > MAXDSMCNT)
	        || ((HW[HW_SECTOR_CNT] & 0xFFFF) == 0)
	        || (gubLBA48BitCmdSupport == 0)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	gubNeedChkDoneTag = 1;

#if ENABLE_TRIM
	U32 ulTrimStartTime = gulOperationTime;
	U32 ulTrimEndTime;

	HandleStopRW(0);
	M_SetFlagSettingNone();

	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN; //because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	if ( gubPIOTirm ) {
		// PIO
		HB[HB_CMD_INFO_LOWBYTE] &= (~H_PIO_DMA_CMD); // bit0 :   0=PIO, 1=DMA
		gubPIOTirm = 0;
	}
	U32 ulTrimSectorCnt, ulTriggerSectorCnt, ulEntryPTR = 0;
	ULLONG uoTrimLBA;
	UWORD uwTrimSecCnt_Entry;
	ULLONG uoTrimLBA_Entry;


	gulSectorCnt = HL[HL_BUF_TRG_SEC];

	/*if (AtaCfg.uwDCOState & SET_DCO_TRIM) {
	            goto Mark_ReturnFail;
	    }
	    else if ((HB_Features & CHK_BIT0) == 0) {
	            goto Mark_ReturnFail;
	    }
	    else if (gulSectorCnt > MAXDSMCNT) {
	            goto Mark_ReturnFail;
	    }
	    else if ((gulSectorCnt & 0xFFFF) == 0) {
	            goto Mark_ReturnFail; // for TSB
	            //goto Mark_Return;
	    }
	    else if (gubIsWriteProtect) {
	            goto Mark_ReturnFail;
	}*/



	HL[HL_BUF_BASE] = BUFFER3_BASE / 4096;
	HL[HL_BUF_R_EP] = BUFFER3_READ_END;
	HL[HL_BUF_W_EP] = BUFFER3_WRITE_END;
	HL[HL_BUF_R_SHF] = BUFFER3_READ_START;
	HL[HL_BUF_W_SHF] = BUFFER3_WRITE_START;

	HL[HL_BUF_OFST] = (HL[HL_CMD_S_LBA] & 0x7);
	ULLONG *uoBuffer =  (ULLONG*)(BUFFER3_BASE + (HL[HL_CMD_S_LBA] & 0x7) * 512);
	//HL[HL_BUF_OFST] = 0;
	//ULLONG *uoBuffer = (ULLONG*)BUFFER3_BASE;

Mark_TrimStart:
	if (gulSectorCnt > BUFFER3_NUM) {
		ulTriggerSectorCnt = BUFFER3_NUM;
		HW[HW_SECTOR_CNT] = BUFFER3_NUM;
		HL[HL_BUF_TRG_SEC] = BUFFER3_NUM;
		gulSectorCnt -= BUFFER3_NUM;
	}
	else {
		ulTriggerSectorCnt = gulSectorCnt;
		HW[HW_SECTOR_CNT] = gulSectorCnt;
		HL[HL_BUF_TRG_SEC] = gulSectorCnt;
		gulSectorCnt = 0;
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)uoBuffer, ulTriggerSectorCnt * 512);//because we set MAX TRIM CNT = 8, the offset plus DCACHE to invalidate will not over 16K size
	}
	HL[HL_TQ_CONTENT] = 0;
	// polling 0x24 表示沒有data傳輸中. 若是有下comreset, 會因為discard redundant放棄此次trigger, 便不會卡住 (也不需再一次APU_Reset)
	// 但是因為如果下reset的話, 後續的動作無須處理可以直接return, 所以這邊還是用0x28, 如果成立才須去看hardreset變數
	while (HL[HL_CMD_REMAIN_SEC]) {
		//若是先發生comreset, 會因為H_REG_XHOLD而無法進來, 所以當HardReset = 1時, 表示事先進來此function才收到reset, 顧無須再reset, 直接離開
		if (gubHardReset) {
			gubNeedChkDoneTag = 0; //可能在此function開頭被舉為1
			goto Mark_Return;
		}
	}

Mark_AssignValue:
	uoTrimLBA = (ULLONG)(uoBuffer[ulEntryPTR] & 0x0000FFFFFFFFFFFFULL);
	ulTrimSectorCnt = 0;
	if (guwTrimRemainder4KNum || guwTrimRemainderSectorCnt) {
		if (uoTrimLBA == (((gulTrimRemainderStartLCA + guwTrimRemainder4KNum) << SectorsPer4KLog) + guwTrimRemainderSectorCnt)) {
			uoTrimLBA = gulTrimRemainderStartLCA << SectorsPer4KLog;
			ulTrimSectorCnt = (guwTrimRemainder4KNum << SectorsPer4KLog) + guwTrimRemainderSectorCnt;
			guwTrimRemainderSectorCnt = 0;
			guwTrimRemainder4KNum = 0;
		}
	}
Mark_AssignValue2:
	uoTrimLBA_Entry  = (ULLONG)(uoBuffer[ulEntryPTR] & 0x0000FFFFFFFFFFFFULL);
	uwTrimSecCnt_Entry = (UWORD)((ULLONG)(uoBuffer[(ulEntryPTR)] & 0xFFFF000000000000ULL) >> 48);
	ulTrimSectorCnt += uwTrimSecCnt_Entry;

	if (ENABLE_DEBUG_UART_TRIM) {
		UartString("\r\nulEntryPTR: 0x");
		UartLongHex(ulEntryPTR);
		UartString(" LBA: 0x");
		UartLLongHex(uoTrimLBA_Entry);
		UartString(" Sec: 0x");
		UartWordHex(uwTrimSecCnt_Entry);
	}

	ulEntryPTR++;

	if (((ulEntryPTR - 1) == ((ulTriggerSectorCnt * 512) / 8))) {
		if (gulSectorCnt) {
			goto Mark_TrimStart;
		}
		else {
			// put return code here.
			//gubNeedDoTrimMap = gubActiveTaskMap;
			gubTest = 0;
			goto Mark_Return;
		}
	}
	else {

		if ((ULLONG)(uoTrimLBA_Entry + uwTrimSecCnt_Entry) > gulMediaSize.All) {
			if (uwTrimSecCnt_Entry != 0) {
				// out of range fail
				if (ENABLE_DEBUG_UART_TRIM) {
					UartString("\r\nTrimMarkFail");
				}
				goto Mark_ReturnFail;
			}
		}

		if ((ULLONG)(uoTrimLBA + ulTrimSectorCnt) == (uoBuffer[ulEntryPTR] & 0x0000FFFFFFFFFFFFULL)) {
			// continue lba
			if (ulEntryPTR != ((ulTriggerSectorCnt * 512) / 8)) {
				goto Mark_AssignValue2;
			}
		}
	}

	// temp
	if (ulTrimSectorCnt == 0) {
		goto Mark_AssignValue;
	}

Mark_StartTrim:

	if (ENABLE_TRIM) {
		if (ENABLE_DEBUG_UART_TRIM) {
			UartString("\nTrimStartLBA: ");
			UartLLongHex(uoTrimLBA);
			UartString("   TrimLength: ");
			UartLongHex(ulTrimSectorCnt);
		}
		gubTrimTableNotCleaned = 1;
		UWORD uwCompensation4k;

		/*if (ulTrimLBA&gubSectorsPerPlaneMask) {  // cut front sectors
		        uwCompensationSector=(gubSectorsPerPlane-(ulTrimLBA&gubSectorsPerPlaneMask));
		        if (ulTrimSectorCnt>uwCompensationSector) {
		                ulTrimLBA+=uwCompensationSector;
		                ulTrimSectorCnt-=uwCompensationSector;
		    }
		}*/
		U32 ulTrimStart4kIndex = uoTrimLBA / 8, ulTrimEnd4kIndex = (uoTrimLBA + ulTrimSectorCnt) / 8;
		if (uoTrimLBA & 7) {
			ulTrimStart4kIndex++;
		}

		U32 ulTrim4kEntrysCnt = 0;
		if (ulTrimEnd4kIndex > ulTrimStart4kIndex) {
			ulTrim4kEntrysCnt = (ulTrimEnd4kIndex - ulTrimStart4kIndex);
		}
		// 1. Trim 結尾為MediaSize 2. MediaSize不Align 4K  3. Trim SC必須大於 "不Align 4k的SC"
		if (((uoTrimLBA + ulTrimSectorCnt) == gulMediaSize.All)
		        && (gulMediaSize.All & SectorsPer4KMask)
		        && (ulTrimSectorCnt >= (gulMediaSize.All & SectorsPer4KMask))) {
			ulTrim4kEntrysCnt++;
		}
		if (ulTrimStart4kIndex % guw4KEntrysPerTrimbit) {
			uwCompensation4k = (guw4KEntrysPerTrimbit - (ulTrimStart4kIndex % guw4KEntrysPerTrimbit));
			if (ulTrim4kEntrysCnt > uwCompensation4k) {
				ulTrimStart4kIndex += uwCompensation4k;
				ulTrim4kEntrysCnt -= uwCompensation4k;
			}
		}
		// handle one trim entry data
		if ((ulTrim4kEntrysCnt >= guw4KEntrysPerTrimbit) && (ulTrimEnd4kIndex >= ulTrimStart4kIndex)) {
			//UWORD uwBurstIndex = ((((ulTrimLBA&gulSectorsPergUnitMask)>>gubSectorsPerBurstLog))>>gubBanksPerPageLog); // simplify to burst bank0
			//ULONG ulTrimEntryIndex = (((((ulTrimLBA>>gubSectorsPergUnitLog)<<gubBurstsPerUnitLog)+uwBurstIndex)<<gubSectorsPerPlaneLog)>>gubSectorsPerTrimEntryLog);
			U32 ulTrimEntryIndex ;
			ulTrimEntryIndex = ((ulTrimStart4kIndex) / guw4KEntrysPerTrimbit);
			while (1) {   //可以改成一次處理4Bytes的trim table...
				UBYTE ubTemp;
				if (ENABLE_DEBUG_TRIM_BIT) {
					ubTemp = gubTrimTable[ulTrimEntryIndex / 8];
				}
				gubTrimTable[ulTrimEntryIndex / 8] |= (BIT0 << (ulTrimEntryIndex & 7));
				if (ENABLE_DEBUG_TRIM_BIT && (ubTemp != gubTrimTable[ulTrimEntryIndex / 8])) {
					gulRemainTrimbit++;
				}
				ulTrimStart4kIndex += guw4KEntrysPerTrimbit;
				ulTrim4kEntrysCnt -= guw4KEntrysPerTrimbit;
				if (ulTrim4kEntrysCnt < guw4KEntrysPerTrimbit) {
					break;
				}
				ulTrimEntryIndex++;
			}
		}
		//只要4k不足以Align TrimBit都記錄餘數
		if (ulTrimEnd4kIndex >= ulTrimStart4kIndex) {
			//以LCA + 4k Num + SC形式處理, SC只用來接續下一筆Trim Command, Trim只處理LCA + 4k Num
			gulTrimRemainderStartLCA = ulTrimStart4kIndex;
			if (ulTrim4kEntrysCnt && (ulTrim4kEntrysCnt <= (guw4KEntrysPerTrimbit - (ulTrimStart4kIndex % guw4KEntrysPerTrimbit)))) {
				guwTrimRemainder4KNum = ulTrim4kEntrysCnt;
			}
			else {
				guwTrimRemainder4KNum = 0;
			}
			if (ulTrimSectorCnt > (((gulTrimRemainderStartLCA + guwTrimRemainder4KNum) << SectorsPer4KLog) - uoTrimLBA)) {
				guwTrimRemainderSectorCnt = (UWORD)ulTrimSectorCnt - (((gulTrimRemainderStartLCA + guwTrimRemainder4KNum) << SectorsPer4KLog) - uoTrimLBA);
			}
			else {
				guwTrimRemainderSectorCnt = 0;
			}
		}
		/*if (ulTrimLBA & guwSectorsPerTrimEntrybitMask) {
		        uwCompensationSector = (guwSectorsPerTrimEntrybit - (ulTrimLBA & guwSectorsPerTrimEntrybitMask));
		        while (ulTrimSectorCnt >= guwSectorsPerTrimEntrybit) {
		                ulTrimLBA += uwCompensationSector;
		                ulTrimSectorCnt -= uwCompensationSector;
		        }
		}


		// handle one trim entry data
		if (ulTrimSectorCnt >= guwSectorsPerTrimEntrybit) {
		        //UWORD uwBurstIndex = ((((ulTrimLBA&gulSectorsPergUnitMask)>>gubSectorsPerBurstLog))>>gubBanksPerPageLog); // simplify to burst bank0
		        //ULONG ulTrimEntryIndex = (((((ulTrimLBA>>gubSectorsPergUnitLog)<<gubBurstsPerUnitLog)+uwBurstIndex)<<gubSectorsPerPlaneLog)>>gubSectorsPerTrimEntryLog);
		        U32 ulTrimEntryIndex ;
		        ulTrimEntryIndex = ((ulTrimLBA) >> gubSectorsPerTrimEntrybitLog);
		        while (1) {   //可以改成一次處理4Bytes的trim table...
		                gubTrimTable[ulTrimEntryIndex / 8] |= (BIT0 << (ulTrimEntryIndex & 7));
		                ulTrimLBA += guwSectorsPerTrimEntrybit;
		                ulTrimSectorCnt -= guwSectorsPerTrimEntrybit;
		                if (ulTrimSectorCnt < guwSectorsPerTrimEntrybit) {
		                        break;
		                }
		                ulTrimEntryIndex++;
		        }
		}*/
	}
	goto Mark_AssignValue;

Mark_ReturnFail:
	gubErrorCode = SET_ABRT;
Mark_Return:
	// call clean GR here, make sure not exceed 600us
	gubTrimTableNotCleaned = 0;
	if (gubTrimTableNotCleaned && (gubPartialCleanGRDoing == 0) && (gubPartialCloseTargetDoing == 0) && (VT->gulGRTargetPTR < (M_Multiplier((guwPagesPerUnit - Fill_SharePage_Reserved), DEF_PlanesPerPage_Mcl_Div))) && (VT->gulGRTargetPTR != 0)) {
		P2LTable_t *ulCurrentGRBase;
		ulCurrentGRBase = (P2LTable_t*)&ActiveGRTable[VT->gulGRTableStartIndex[VT->gubGRTargetIndex]];
		if ((gulRandomZcodeZoneBegin == gulZoneBBegin) && (gubZcodeB != 0xFF)) {
			gulZoneBBegin = VT->gulGRTableL4kPTR;
		}
		UWORD uwL2PGroupIndex;
		U32 ulZByteNumSum;
		U32 ulStartIndex;
		U32 ulEndIndex;
		U32 ulGRTableL4kIndex;
		U32 ulGRTargetP4kEntryPTR = (VT->gulGRTargetPTR << gub4kEntrysPerPlaneLog) + (VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget * gul4kEntrysPerUnit);
		U32 ulUnitShift = VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget * gul4kEntrysPerUnit;
		U32 ulTempGRIndex = (VT->gubGRTargetIndex == 0) ? (VT->gulGRTableTempStartIndex - GRTable_Max_Collect_Num) : 0;
		// 重頭掃一次正在寫的GR unit的P2L Table, 與L2P Table比, 確保一個LCA在一個unit中只有一個valid的位置
		// 此條件滿足的話, 在GC時做P2L與L2P比對時, 只需比對unit index, 不需比entry & zbyte offset
		if ((VT->gubGRTargetIndex == 0) && (VT->gulGRTableTempStartIndex != GRTable_Max_Collect_Num)) {
			ulZByteNumSum = 0;
			ulStartIndex = 0;
			ulEndIndex = VT->gulGRTableL4kPTR;
		}
		else {
			ulZByteNumSum = gulRandomZcodeZoneBegin_PhyEntry - (VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget * gulSectorsPerUnit);
			ulStartIndex = gulRandomZcodeZoneBegin;
			ulEndIndex = gulZoneBBegin;
		}
		for (ulGRTableL4kIndex = ulStartIndex; ulGRTableL4kIndex < ulEndIndex ; ulGRTableL4kIndex++) {
			P2LTable_t ulGRTemp = ulCurrentGRBase[ulGRTableL4kIndex];
			U32 ulGRTempVir4kIndex = (gubL2P_FORMAT_MODE) ? ulGRTemp.L2P_FORMAT_MODE1.ulVir4kIndex : ulGRTemp.L2P_FORMAT_MODE0.ulVir4kIndex;
			if (ulGRTempVir4kIndex == gulTable_Empty) {
				continue;
			}
			if ((ulGRTempVir4kIndex & gulMask_NoValidData) != gulMask_NoValidData) {
				UBYTE ubCheckL2P = 0;
				UBYTE ubCheckGRLink = 0;
				uwL2PGroupIndex = ulGRTempVir4kIndex >> gubL2PEntrysPerL2PGroupLog;
				// 針對GR Unit還沒寫滿就cleanGR需要特殊處理......
				if (ulGRTableL4kIndex < ulTempGRIndex) {
					if ((guwL2P_InRamIndex[uwL2PGroupIndex] & TableInTempMask) < guwWriteModeTableNum_ReadModeStartIndex) {
						ubCheckL2P = 1;
					}
					else {
						ubCheckGRLink = 1;
					}
				}
				else if (ulGRTemp.L2P_FORMAT_MODE0.btNeedCleanGRUpdateL2P == 0) {
					ubCheckL2P = 1;
				}

				if (ubCheckL2P) {
					U32 ulEntry = ulZByteNumSum >> SectorsPer4KLog;
					ulEntry = ulUnitShift + ulEntry;
					UBYTE ubOffset = ulZByteNumSum & SectorsPer4KMask;
					U32 ulVir4kIndexInL2POffset = (ulGRTempVir4kIndex & guwL2PEntrysPerL2PGroupMask);
					if (ENABLE_DEBUG_RW) {
						M_AssertError(guwL2P_InRamIndex[uwL2PGroupIndex] == TableNotInRAM);
					}
					L2PTable = (L2PTable_t*)&gulL2PBase[(guwL2P_InRamIndex[uwL2PGroupIndex] & TableInTempMask) * guwL2PEntrysPerL2PGroup];
					U32 ulL2PFEntry;
					UBYTE ubL2POffset;
					UBYTE ubL2PZByteNum;
					M_ACC_PARSE_L2P(L2PTable[ulVir4kIndexInL2POffset].All, ulL2PFEntry, ubL2POffset, ubL2PZByteNum);
					if ((ulL2PFEntry != ulEntry) || (ubL2POffset != ubOffset)) {
						if (gubL2P_FORMAT_MODE) {
							ulCurrentGRBase[ulGRTableL4kIndex].L2P_FORMAT_MODE1.ulVir4kIndex = gulTable_Invalid;
						}
						else {
							ulCurrentGRBase[ulGRTableL4kIndex].L2P_FORMAT_MODE0.ulVir4kIndex = gulTable_Invalid;
						}
					}
				}
				else if (ubCheckGRLink) {
					UBYTE ubSearchLinkLayer =  gulTable_L2P[uwL2PGroupIndex].B.ubGRSearchLinkState;
					U32 ulTotalL2PCntShiftLayer = (gulTotalL2PGroupCntPowerOf2 >> ubSearchLinkLayer);
					UBYTE ubSubGroupIndex = (( ulGRTempVir4kIndex & guwL2PEntrysPerL2PGroupMask) << ubSearchLinkLayer ) >> gubL2PEntrysPerL2PGroupLog; //Index/ (Group/layer)
					U32 ulSearchLinkIndex = (ulTotalL2PCntShiftLayer * ubSubGroupIndex) + (uwL2PGroupIndex % ulTotalL2PCntShiftLayer);
					U32 ulHeadVir4kIndex = (gubL2P_FORMAT_MODE) ? gulGRTable[GRSearchTable[ulSearchLinkIndex].ulHead].L2P_FORMAT_MODE1.ulVir4kIndex : gulGRTable[GRSearchTable[ulSearchLinkIndex].ulHead].L2P_FORMAT_MODE0.ulVir4kIndex;
					if ((ulHeadVir4kIndex >> gubL2PEntrysPerL2PGroupLog) == uwL2PGroupIndex) {
						U32 ulGRSearchTableCheckNum = 0;
						U32 ulCheckTableIndex = GRSearchTable[ulSearchLinkIndex].ulHead;
						while (ulGRSearchTableCheckNum < GRSearchTable[ulSearchLinkIndex].uwMemberCount) {
							U32 ulCheckVir4kIndex = (gubL2P_FORMAT_MODE) ? gulGRTable[ulCheckTableIndex].L2P_FORMAT_MODE1.ulVir4kIndex : gulGRTable[ulCheckTableIndex].L2P_FORMAT_MODE0.ulVir4kIndex;
							if (ulGRTempVir4kIndex == ulCheckVir4kIndex) {
								if (gubL2P_FORMAT_MODE) {
									ulCurrentGRBase[ulGRTableL4kIndex].L2P_FORMAT_MODE1.ulVir4kIndex = gulTable_Invalid;
								}
								else {
									ulCurrentGRBase[ulGRTableL4kIndex].L2P_FORMAT_MODE0.ulVir4kIndex = gulTable_Invalid;
								}
								break;
							}
							ulCheckTableIndex = GRSearchTableNext[ulCheckTableIndex];
							ulGRSearchTableCheckNum ++;
						}
					}
				}
			}
			UBYTE ubGRZByteNum;
			if (gubL2P_FORMAT_MODE == 1) {
				ubGRZByteNum = ulGRTemp.L2P_FORMAT_MODE1.ubZByteNum;
				ubGRZByteNum = ((ubGRZByteNum * 2) + 1);
			}
			else {
				ubGRZByteNum = ulGRTemp.L2P_FORMAT_MODE0.ubZByteNum;
			}
			ulZByteNumSum += (ubGRZByteNum + 1);
		}
		if (gubZcodeB == 0xFF) {
			//原本全部都是zone A, middle clean前設定 gulRandomZcodeZoneBegin_PhyEntry之後是random區
			gulRandomZcodeZoneBegin =  VT->gulGRTableL4kPTR;
			gulRandomZcodeZoneBegin_PhyEntry = ulGRTargetP4kEntryPTR << SectorsPer4KLog;
		}
		gulZoneBBegin = gulRandomZcodeZoneBegin;
		gubZcodeB = 8;
		gulZoneBBegin_PhyEntry = ulGRTargetP4kEntryPTR << SectorsPer4KLog;

		gulGRMiddleCleanGRL4KPTR = VT->gulGRTableL4kPTR;
		gubCleanGRinMiddle = 1;
		gulGRTemp4KPTR = VT->gulGRTargetPTR << gub4kEntrysPerPlaneLog;
		//VT->gulGRTableL4kNum += VT->gulGRTableL4kPTR;

		ActiveGRTable = gulGRTableTemp;
		mSE_SV(4, (U32)gulGRTableTemp, TempGRTableSize, gulTable_Empty, 0);


		UBYTE ubFastPage = gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.btFastPage;

		//先以Toshiba 排法計算
		if ((!TLC) && (ubFastPage == 0)) {
			UWORD uwPageIndex = M_Divider((VT->gulGRTargetPTR - 1), DEF_PlanesPerPage_Mcl_Div);
#if MicronFlashOnly
			M_AssertError(uwPageIndex > (guwPagesPerUnit - Fill_SharePage_Reserved));
#else
			M_AssertError(uwPageIndex > (guwSectorsPerPage - 6));
#endif
			U32 ulNextPageAPtr;
#if MicronFlashOnly
			ulNextPageAPtr = M_Multiplier((uwPageIndex + 64), DEF_PlanesPerPage_Mcl_Div);
#else
			if (uwPageIndex & BIT0) {
				ulNextPageAPtr = M_Multiplier((uwPageIndex + 4), DEF_PlanesPerPage_Mcl_Div);
			}
			else {
				ulNextPageAPtr = M_Multiplier((uwPageIndex + 3), DEF_PlanesPerPage_Mcl_Div);
			}
#endif
			ftlSaveDummy(ulNextPageAPtr, FromXferDataIn, 1);
			gubNeedCheckSeq = 0; // Stop seq check when dummy
		}
		gulExcecute4kTrimNum = 0;
		gubCleanGRinMiddle = 0xFE;

		while (gubTrimTableNotCleaned) {
			VT->gulGRTableL4kPTR = 0;
			ftlCleanGRTable(BIT_CleanGR | BIT_Finish_In_One_Time);
			gubCleanGRinMiddle = 1;
			ulTrimEndTime = gulOperationTime;
			if ((ulTrimEndTime < ulTrimStartTime) || ((ulTrimEndTime - ulTrimStartTime) > 600)) {
				break;
			}
		}
		//ftlSwapTable(BYTE_EC_Save_Mode, 0);
	}
	//ulTrimTime = RTT_Get_Value(1);
	//UartLongHex(ulTrimTime);
	/*while (gBQI.ubLinkNum>=(BQ_NUM/2)) {
	        M_SwitchTask();
	}*/
	// <<< restore buffersetting...


	if (gubDDRMode) {
		HL[HL_BUF_BASE] = BUFFER2_BASE / 4096;
		HL[HL_BUF_R_EP] = BUFFER2_WRITE_END;
		HL[HL_BUF_W_SHF] = 0;
		HL[HL_BUF_W_EP] = SDRAM_CACHE_4KNUM - 1;
	}
	else {
		HL[HL_BUF_BASE] = BUFFER2_BASE / 4096;
		HL[HL_BUF_R_EP] = BUFFER2_READ_END;
		HL[HL_BUF_W_EP] = BUFFER2_WRITE_END;
		HL[HL_BUF_R_SHF] = BUFFER2_READ_START;
		HL[HL_BUF_W_SHF] = BUFFER2_WRITE_START;
	}

	M_CheckDoneTag();
	gulSATA_MSG_TYPE3[0].All = 0;

	while (HL[HL_AXI_FIS_STAT]&BIT23);
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;

	if (gubHardReset == 0) {
		M_SetFlagSettingSHF();
	}

#endif
#endif
}
#else
/// =================================================================================================
void Trim(void)
{
#if !BURNER
	if (ENABLE_DEBUG_UART_TRIM) {
		UartString("\r\nT_CMD  ");
	}

	gubCalledWithHostCMD = 0;
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect) {
			gubErrorCode = SET_ABRT;
			return;
		}
	}

	if (gubSSD_Init_State.B.btCleanRAMForWriteDone == 0) {
		ftlClearRAMForGR(gulStartClearGRAddress, 1);
	}

	if ((gubTrimSupport == 0)
	        || (AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || (AtaCfg.uwDCOState & SET_DCO_TRIM)
	        || ((HW[HW_FEATURE] & CHK_BIT0) == 0)
	        || (HW[HW_SECTOR_CNT] > MAXDSMCNT)
	        || ((HW[HW_SECTOR_CNT] & 0xFFFF) == 0)
	        || (gubLBA48BitCmdSupport == 0)
	        || ((IDT.W59 & BIT12) && (AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		UartString("T_CMD_ERR \r\n");
		return;
	}

	gubNeedChkDoneTag = 1;


#if(ENABLE_TRIM)

	HandleStopRW(0);

	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN; //because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	if ( gubPIOTirm ) {
		// PIO
		HB[HB_CMD_INFO_LOWBYTE] &= (~H_PIO_DMA_CMD); // bit0 :   0=PIO, 1=DMA
		gubPIOTirm = 0;
	}
	U32 ulTrimSectorCnt, ulTriggerSectorCnt, ulEntryPTR = 0;
	ULLONG uoTrimLBA;
	UWORD uwTrimSecCnt_Entry;
	ULLONG uoTrimLBA_Entry;
	UBYTE ubCount = 0;
	UBYTE ubCount2 = 0;
	UWORD uwL2PGroupIndexCnt;
	U32 ulTotalL2PGroupTrimed;
	UWORD uwStartL2PGroup;
	UWORD uwEndL2PGroup;

	gulSectorCnt = HL[HL_BUF_TRG_SEC];
	M_SetFlagSettingNone();
	HL[HL_BUF_BASE] = BUFFER3_BASE / 4096;
	HL[HL_BUF_R_EP] = BUFFER3_READ_END;
	HL[HL_BUF_W_EP] = BUFFER3_WRITE_END;
	HL[HL_BUF_R_SHF] = BUFFER3_READ_START;
	HL[HL_BUF_W_SHF] = BUFFER3_WRITE_START;

	HL[HL_BUF_OFST] = (HL[HL_CMD_S_LBA] & 0x7);
	ULLONG *uoBuffer =  (ULLONG*)(BUFFER3_BASE + (HL[HL_CMD_S_LBA] & 0x7) * 512);

	/// Init Global Variable
	for (; ubCount < D_TRIM_TABLE_NUMBER ; ubCount++) {
		gulTrimEnd4kIndex[ubCount] = 0;
		gulTrimStart4kIndex[ubCount] = 0;
	}
	ubCount = 0;
	/// ============== start to gather LBA and SecCnt number in Buffer=============

Mark_TrimStart:
	if (gulSectorCnt > BUFFER3_NUM) {
		ulTriggerSectorCnt = BUFFER3_NUM;
		HW[HW_SECTOR_CNT] = BUFFER3_NUM;
		HL[HL_BUF_TRG_SEC] = BUFFER3_NUM;
		gulSectorCnt -= BUFFER3_NUM;
	}
	else {
		ulTriggerSectorCnt = gulSectorCnt;
		HW[HW_SECTOR_CNT] = gulSectorCnt;
		HL[HL_BUF_TRG_SEC] = gulSectorCnt;
		gulSectorCnt = 0;
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)uoBuffer, ulTriggerSectorCnt * 512);
	}
	HL[HL_TQ_CONTENT] = 0;
	// polling 0x24 表示沒有data傳輸中. 若是有下comreset, 會因為discard redundant放棄此次trigger, 便不會卡住 (也不需再一次APU_Reset)
	// 但是因為如果下reset的話, 後續的動作無須處理可以直接return, 所以這邊還是用0x28, 如果成立才須去看hardreset變數
	while (HL[HL_CMD_REMAIN_SEC]) {
		//若是先發生comreset, 會因為H_REG_XHOLD而無法進來, 所以當HardReset = 1時, 表示事先進來此function才收到reset, 顧無須再reset, 直接離開
		if (gubHardReset) { //表示進來後才收到reset
			goto Mark_Return;
		}
	}

Mark_AssignValue:
	uoTrimLBA = (ULLONG)(uoBuffer[ulEntryPTR] & 0x0000FFFFFFFFFFFFULL);
	ulTrimSectorCnt = 0;
Mark_AssignValue2:
	uoTrimLBA_Entry  = (ULLONG)(uoBuffer[ulEntryPTR] & 0x0000FFFFFFFFFFFFULL);
	uwTrimSecCnt_Entry = (UWORD)((ULLONG)(uoBuffer[(ulEntryPTR)] & 0xFFFF000000000000ULL) >> 48);
	ulTrimSectorCnt += uwTrimSecCnt_Entry;

	if ((ULLONG)(uoTrimLBA_Entry + uwTrimSecCnt_Entry) > gulMediaSize.All) {
		if (uwTrimSecCnt_Entry != 0) {
			// out of range fail
			goto Mark_ReturnFail;
		}
	}
	ulEntryPTR++;
	if ((ulEntryPTR == ((ulTriggerSectorCnt * 512) / 8))) {
		if (gulSectorCnt) {
			goto Mark_TrimStart;
		}
		else {
			// put return code here.
			//gubNeedDoTrimMap = gubActiveTaskMap;
			gubTest = 0;
			goto Mark_Return;
		}
	}
	else if ((ULLONG)(uoTrimLBA + ulTrimSectorCnt) == (uoBuffer[ulEntryPTR] & 0x0000FFFFFFFFFFFFULL)) {
		// continue lba
		if (ulEntryPTR != (((ulTriggerSectorCnt * 512) / 8) - 1)) {
			goto Mark_AssignValue2;
		}
	}

	if (ulTrimSectorCnt == 0) {
		goto Mark_AssignValue;
	}

	/// ============== start to check alignment =============

Mark_StartTrim:
	if (ENABLE_TRIM) {
		if (ENABLE_DEBUG_UART_TRIM) {
			UartString("\r\nTrimStartLBA: ");
			UartLLongHex(uoTrimLBA);
			UartString("\r\nTrimLength: ");
			UartLongHex(ulTrimSectorCnt);
			UartString("\r\nStartTime: ");
			UartLongHex(gulOperationTime);
		}
		//
		//					UartString("\r\nTrimLength: ");
		//					UartLongHex(ulTrimSectorCnt);
		//					UartString("\r\nStartTime: ");
		//					UartLongHex(gulOperationTime);
		//					UartString("\r\n");


		/// deal with unaligned 4K start sectors.
		U32 ulTrimStart4kIndex = (ULONG)(uoTrimLBA >> 3);
		if ((uoTrimLBA & 7) != 0) {
			ulTrimStart4kIndex++;
			/// if there's no alignment of 4K sectors, use Zero() to deal with.
			gubNoAlignSecCntForTrim = 8 - (uoTrimLBA & 7) ;
			gulNoAlignStartLBAForTrim = (ULONG)(uoTrimLBA);
			Zero();
		}


		/// deal with unaligned 4K End sectors.
		U32 ulTrimEnd4kIndex = (((ULONG)(uoTrimLBA)) + ulTrimSectorCnt - 1);
		if ((ulTrimEnd4kIndex & 7) != 7) {
			/// if there's no alignment of 4K sectors, use Zero() to deal with.
			gubNoAlignSecCntForTrim = (ulTrimEnd4kIndex & 7) + 1;
			gulNoAlignStartLBAForTrim = (ulTrimEnd4kIndex & 0xFFFFFFF8 );
			Zero();
			ulTrimEnd4kIndex = ((ulTrimEnd4kIndex >> 3) - 1);
		}
		else {
			ulTrimEnd4kIndex = (ulTrimEnd4kIndex >> 3);
		}


		//// set
		gulTrimEnd4kIndex[ubCount] = ulTrimEnd4kIndex;
		gulTrimStart4kIndex[ubCount] = ulTrimStart4kIndex;
		ubCount++;

		if (ubCount == D_TRIM_TABLE_NUMBER) {

			///Debug usage



			///count how many L2Pgroups needed to be trimed
			ulTotalL2PGroupTrimed = 0;
			for (ubCount2 = 0; ubCount2 < ubCount; ubCount2++) {
				uwStartL2PGroup = gulTrimStart4kIndex[ubCount2] >> gubL2PEntrysPerL2PGroupLog;
				uwEndL2PGroup = (gulTrimEnd4kIndex[ubCount2] >>  gubL2PEntrysPerL2PGroupLog) + 1;

				for (uwL2PGroupIndexCnt = uwStartL2PGroup; uwL2PGroupIndexCnt < uwEndL2PGroup; uwL2PGroupIndexCnt++) {
					ulTotalL2PGroupTrimed++;
				}
			}
			gulTotalL2PGroupTrimed = ulTotalL2PGroupTrimed;
			gubNeedFlushL2PInTrim = 0;
			//			UartString("\r\ngulTotalL2PGroupTrimed:");
			//			UartLongHex(gulTotalL2PGroupTrimed);
			///====count how many L2Pgroups needed to be trimed End
			gubWaitFTLtoSwapL2P = ubCount;
			while (gubWaitFTLtoSwapL2P) {
				M_SwitchTask();
			}
			ubCount = 0;
		}

	}
	goto Mark_AssignValue;

Mark_ReturnFail:
	gubErrorCode = SET_ABRT;
Mark_Return:
	gubErrorCode = 0;


	///Final SwitchTask
	if (ubCount > 0) {

		///count how many L2Pgroups needed to be trimed
		ulTotalL2PGroupTrimed = 0;
		for (ubCount2 = 0; ubCount2 < ubCount; ubCount2++) {
			uwStartL2PGroup = gulTrimStart4kIndex[ubCount2] >> gubL2PEntrysPerL2PGroupLog;
			uwEndL2PGroup = (gulTrimEnd4kIndex[ubCount2] >>  gubL2PEntrysPerL2PGroupLog) + 1;

			for (uwL2PGroupIndexCnt = uwStartL2PGroup; uwL2PGroupIndexCnt < uwEndL2PGroup; uwL2PGroupIndexCnt++) {
				ulTotalL2PGroupTrimed++;
			}
		}
		gulTotalL2PGroupTrimed = ulTotalL2PGroupTrimed;
		gubNeedFlushL2PInTrim = 0;
		//		UartString("\r\ngulTotalL2PGroupTrimed:");
		//		UartLongHex(gulTotalL2PGroupTrimed);
		///====count how many L2Pgroups needed to be trimed End

		gubWaitFTLtoSwapL2P = ubCount;
		while (gubWaitFTLtoSwapL2P) {
			M_SwitchTask();
		}
		ubCount = 0;
	}


	/// ========== Final Check=============
	//
	//				if(ENABLE_DEBUG_UART_TRIM){
	//					for(uwIndexCheckingRec = 0; uwIndexCheckingRec < guwWriteModeTableNum_ReadModeStartIndex; uwIndexCheckingRec++)
	//					{
	//
	//						if(guwL2P_InRamIndex[gulInRamInverseBackUp[uwIndexCheckingRec].B.ulL2PGroupIndex] >= guwWriteModeTableNum_ReadModeStartIndex)
	//						{
	//							UartString("Wrong L2PGroup 3\r\n");
	//							UartString("Index:");
	//							UartWordHex(uwIndexCheckingRec);
	//							UartString("  InRamIndex:");
	//							UartLongHex(guwL2P_InRamIndex[gulInRamInverseBackUp[uwIndexCheckingRec].B.ulL2PGroupIndex]);
	//							UartString("\r\n");
	//							while(1);
	//						}
	//					}
	//				}
	/// ========== Final Check Done=============
	if (gubDDRMode) {
		HL[HL_BUF_BASE] = BUFFER2_BASE / 4096;
		HL[HL_BUF_R_EP] = BUFFER2_WRITE_END;
		HL[HL_BUF_W_SHF] = 0;
		HL[HL_BUF_W_EP] = SDRAM_CACHE_4KNUM - 1;
	}
	else {
		HL[HL_BUF_BASE] = BUFFER2_BASE / 4096;
		HL[HL_BUF_R_EP] = BUFFER2_READ_END;
		HL[HL_BUF_W_EP] = BUFFER2_WRITE_END;
		HL[HL_BUF_R_SHF] = BUFFER2_READ_START;
		HL[HL_BUF_W_SHF] = BUFFER2_WRITE_START;
	}
	if (gubHardReset == 0) {
		M_SetFlagSettingSHF();
		M_CheckDoneTag();
	}
	gulSATA_MSG_TYPE3[0].All = 0;
	while (HL[HL_AXI_FIS_STAT]&BIT23);
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;


#endif /// end of #if ENABLE_TRIM
#endif /// end of #if(!RDT)

	gubCalledWithHostCMD = 1;
#endif
	//			 SYS0B[SYS0B_GPIO_CTRL_GPIO6]=(SYS0B_CR_GPIO_PU_X_1|SYS0B_CR_GPIO_PU_X_0|SYS0B_CR_GPIO_OE|0);  ///GPIO6  Low
	//			*((volatile unsigned char*)0x04000202)|=0x01;

	//			UartString("\r\n EndTime: ");
	//			UartLongHex(gulOperationTime);
	//			UartString("\r\n");


}
#endif  /// End of OLD_TRIM or D-Trim

void WriteSectors(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	if (ENABLE_WRITEPROTECT) {
		if (((!gubWriteProtectWriteDummy) && gubIsWriteProtect) || (gubIsWriteProtect & BIT_ERR_UNIT_WRITEPROTECT)) { //writeprotect且不收dummy
			gubErrorCode = SET_ABRT;
			return;
		}
	}

	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || ((gubNCQSupport == 0) && (HB[HB_COMMAND] == 0x61))
	        || ((gubLBA48BitCmdSupport == 0) && (HB[HB_COMMAND]  == 0x34 || HB[HB_COMMAND]  == 0x35))
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))
	        || ((gubFormFactor != FORM_FACTOR_CFAST) && (HB[HB_COMMAND] == 0x38))) {

		gubErrorCode = SET_ABRT;
		return;
	}
	gulLastWriteTime	= GetRTT_m();

	if ((gubNCQINTRError || gubHardReset || gubLinkLost)) { //有LBA out of range/Link Lost/ hardReset就不chk doneTag
		gubNeedChkDoneTag = 0;
		goto Discard_Redundant_NCQWCMD;
	}
	else {
		gubNeedChkDoneTag = 1;
	}

	if (ENABLE_KT_DEBUG) {
		if (gubNCQINTRError) {
			UartString("\r\n Abort W");
			return;
		}
	}
	HL[HL_HAT_MISC] |= H_TQ_BY_ORDER;
	gubSSD_Init_State.B.ubState = BYTE_INIT_PRELOAD_L2P;
	gubFlushCache_StopBG = 0;

	U32 ulLBA, ulSectorCnt;
	U32 ulTriggerSectorCnt;
	UWORD uwBufferPTR;
	UBYTE ub4kNum;
	U32 ulDataMask;
	UBYTE ubDDRMode;
	UBYTE ub4kTransferDone = 0;
	U32 ulWriteBufferMask;
	U32 ulWriteBufferStartIndex;
	UWORD uwEC;
	UBYTE ubTrigCMDCount = 0;
	UBYTE ubPreTag = 0xFF;
	UBYTE ubSeq = 0;
	UBYTE ubNOCMDCnt = 0;
	UBYTE ubStopWrite = 0;
	WR_NCQ_INFO ulNCQ_INFO_FailCheck;


	WR_NCQ_INFO ulNCQ_INFO;
	UBYTE ubNCQNextWRTag = 0xFF;
	UWORD ubDDR_TQ_4KCOUNT = 0;
	UBYTE ubSeqCMD = 0;
	UBYTE ubTempMultiTrigTag;

	ulNCQ_INFO.All = HL[HL_WR_NCQ_INFO];

	CheckID();
	guw4KRandomCacheReadCount = 0;
	gul4KRandomCacheRead = 0;
	if (gubErrorCode) {
		ReturnErr();
		return;
	}
	if (ENABLE_D2HSDBByHW && gubWriteCacheEnable && (gubWriteProtectWriteDummy || (!gubIsWriteProtect))) { //Enable Write Hardware D2H
		M_EnableHWD2HSDB_W();
		gubNeedSendCmdComplete = 0;
	}
	else {
		M_DisableHWSDB_NCQW();
		gubNeedSendCmdComplete = 1;
	}

	if (ENABLE_DEBUG_RW && (gulLBA.All == 0x77A701)) {
		gubTemp = 1;
	}

	M_AssertError( (gubDDRMode == 0) && (guwDataInSDR4kNum > 0));
	ubDDRMode = gubDDRMode;


	if (gubDDRMode) {
		ulWriteBufferMask = SDRAM_CACHE_MASK;
		ulWriteBufferStartIndex = 0;
	}
	else {
		ulWriteBufferMask = BUFFER2_WRITE_MASK;
		ulWriteBufferStartIndex = BUFFER2_WRITE_START_INDEX;
	}
	gubSeqR = 0;
	if (gubPreRead) {
		while (gBQI.ubWLinkNum || gFQI.ubFQLinkNumber_W || gubLoadAlignDoing) {
			M_SwitchTask();
		}
		gubClearPreRead = 1;
		guoFWRead4kBufferFlag = 0;
		gubPreReadCheckCount = 0;
		M_SetFlagSettingNone();
		AXIL[AXIL_SET_START_FLAG] = 0;
		AXIL[AXIL_SET_FLAG_LENGTH] = gulBuffer2ReadNum;
		AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
		while (gBQI.ubRLinkNum || gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}
		M_Clear_UNC_Addr();
		gubPreRead = 0;
		gulPreReadTriggerSectorCnt = 0;
		mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 0);//強迫DMAC 全部做完
		M_SetFlagSettingSHF();
		gubClearPreRead = 0;
	}
	gulPreReadStartLBA++;


#if ENABLE_PFAIL
	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];
	UBYTE ubDDT_Enable = 1;

	gBQI.uwWrite_buffer_tq = 0U;
	ftl_cal_write_buffer_window();
#endif
	if (gubWriteProtectWriteDummy && gubIsWriteProtect) {
		while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}
		M_SetFlagSettingNone();//因為接下來的WRITE不會塞BQ為了收DATA不被FLAG給卡下
		AXIL[AXIL_SET_START_FLAG] = 0;
		AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM;
		AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
	}
	ulNCQ_INFO.All = HL[HL_WR_NCQ_INFO];
MARK_SET_TQ:

#if ENABLE_PFAIL
	ub4kNum = gBQI.ubForce_clean_wbq_threshold;
	while (0 == gBQI.uwWrite_buffer_remain) {
		_DEBUG_TRACE((0xFE << 24) | (0xA << 16) | gBQI.uwWrite_tq_2_bq);
		//if (gBQI.ubLinkNum)
		{
			M_SwitchTask();
		}

		ftl_cal_write_buffer_window();
		if ((gBQI.uwWrite_buffer_remain > 0U) || (gBQI.uwWrite_buffer_tq > 0) /*|| (gBQI.ubForce_clean_wbq_threshold == gBQI.uwWrite_buffer_window)*/ || (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK)) {
			gBQI.ubForce_clean_wbq_threshold = ub4kNum;
			if (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK) {
				if (gubNeedSendCmdComplete) {
					gubErrorCode = SET_ABRT;
					gubAbort_CMD = 1;
				}
				goto Discard_Redundant_NCQWCMD;
			}
			break;
		}
		//else if ((0x3F == ulNCQ_INFO.B.ubNCQ_NEXT_WR) || (gBQI.uwWrite_tq_2_bq>0))
		{
			gBQI.ubForce_clean_wbq_threshold = gBQI.uwWrite_buffer_window;
		}

		/* When encounter minimum window, 'load alignment BQ' should be released to free single plane. */
		if ((gBQI.uwWrite_buffer_window >= gub4kEntrysPerPlane) && (gubKeepLoadAlignmentBQNum)) {
			gBQI.ubLinkNum += 2;
			gBQI.ubWLinkNum++;
			gBQI.ubLinkFTLLast = gBQI.ubLinkLastUse;
			gubKeepLoadAlignmentBQLink = 0xFF;
			gubKeepLoadAlignmentBQNum = 0;
			guwNonAlignBufferPTR = 0xFFFF;
			ubSeq = 0; /* Cancel sequential write. */
		}
	}
#endif
	_ASSERT(5, ((!gBQI.ubWrite_TQ_lock) && ((gBQI.uwWrite_tq_2_bq + gBQI.uwWrite_buffer_tq) > gBQI.uwWrite_buffer_window)));
	while ((gubNCQDoing == 0)  || ((ulNCQ_INFO.B.ubNCQ_CMD_VLD_SUM != 0) && (ulNCQ_INFO.B.ubNCQ_NEXT_WR != 0x3F) && (WR_NCQ_CMD_INFO[ulNCQ_INFO.B.ubNCQ_NEXT_WR].uwEC_BQDone == WR_NCQ_CMD_INFO[ulNCQ_INFO.B.ubNCQ_NEXT_WR].uwEC))) {
#if ENABLE_PFAIL
		if (0 == gBQI.uwWrite_buffer_remain) {
			/* No more TQ. */
			break;
		}
#endif
		if (gubLBAOverlap) {
#if ENABLE_PFAIL
			gBQI.ubLBAOverlap = ENABLE_PFAIL;
			//UartString("\nOVER");
#endif
			if (ulNCQ_INFO.B.ubNCQ_NEXT != ulNCQ_INFO.B.ubNCQ_NEXT_WR) {
				break;
			}
		}
		if (gulNCQCMDDebug & ((U32)BIT0 << ulNCQ_INFO.B.ubNCQ_NEXT_WR)) {
			break;
		}
		if ((gubFirstCMDPending == 0) && (ulNCQ_INFO.B.ubNCQ_NEXT_WR != ulNCQ_INFO.B.ubNCQ_NEXT)) {
			if (gubNeedChkDoneTag) { //=0的話表示中間表示發生fail/Reset, 不需再考慮First Cmd Pending
				gubFirstCMDPending = 1;
				gulFirstCMDTimeSave = gulOperationTime;
			}
		}
		else if (((gubFirstCMDPending == 1) && ((gulOperationTime - gulFirstCMDTimeSave) > PENDING_CMD_THRESHOLD)) /* || gBQI.ubWrite_TQ_lock */) {
			ubStopWrite = 1;
			break;
		}
		//stop trig
		if (ubDDRMode) {
			if (((SDRAM_CACHE_4KNUM - guwDataInSDR4kNum - ubDDR_TQ_4KCOUNT) <= 2) || (ubDDR_TQ_4KCOUNT >= (MAX_WRITE_TRIG_4K_NUM - 2))) { //Cache Full or Trig Count > 256K (DCCM Zcode Info 64 * 4K)
				break;
			}
		}
		else {
			UWORD uwSRamBufferPTRTemp = guwSRamBufferPTR;
			if (ubSeq && ((WR_NCQ_CMD_INFO[ubPreTag].ulLBA + WR_NCQ_CMD_INFO[ubPreTag].ulSectorCnt) & SectorsPer4KMask)) {
				uwSRamBufferPTRTemp =  ((uwSRamBufferPTRTemp - SectorsPer4K) & ulWriteBufferMask) + ulWriteBufferStartIndex;
			}
			if ((guwNonAlignBufferPTR != 0xFFFF) && ((uwSRamBufferPTRTemp == guwNonAlignBufferPTR) || ((((guwNonAlignBufferPTR - uwSRamBufferPTRTemp) & BUFFER2_WRITE_MASK) / 8) <= 2))) {
				if (gulNCQWRCmdTriggerCnt) {
					break;
				}
				else { // clear load alignment BQ
					if (ENABLE_KEEP_WBQ) {
						if (gubKeepWBQNum) {
							gBQI.ubLinkNum += gubKeepWBQNum;
							gBQI.ubWLinkNum += gubKeepWBQNum;
							gubKeepWBQNum = 0;
							gubKeepWBQSectorCnt = 0;
							gubKeepWBQ4KCnt = 0;
							gulWriteBufferBitMap = 0;
							UBYTE ubi;
							for (ubi = 0; ubi < 32; ubi++) {
								gubWriteBufferTableHead[ubi] = Default_WB_Index;
								gubWriteBufferTableNext[ubi] = Default_WB_Index;
							}
							if (gubKeepLoadAlignmentBQNum) {
								gubFirstKeepBQLink = gubKeepLoadAlignmentBQLink;
								gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
							}
							else {
								gBQI.ubLinkFTLLast = gBQI.ubLinkLastUse;
							}
						}
					}

#if (0==ENABLE_PFAIL)
					_DEBUG_TRACE((0xFE << 24) | (0xB << 16) | gBQI.uwWrite_tq_2_bq);
					while (gBQI.ubLinkNum) {
						gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
						M_SwitchTask();
					}
					gBQI.ulHostWriteWaitBufferFlag  = 0;
					ftl_cal_write_buffer_window();
#endif
					if (gubKeepLoadAlignmentBQNum) {
						if (ubSeq == 0) {
							gBQI.ubLinkNum += 2;
							gBQI.ubWLinkNum++;
							if (ENABLE_KEEP_WBQ) {
								gubKeepWBQNum = 0;
								gubKeepWBQSectorCnt = 0;
								gubKeepWBQ4KCnt = 0;
								gulWriteBufferBitMap = 0;
								UBYTE ubi;
								for (ubi = 0; ubi < 32; ubi++) {
									gubWriteBufferTableHead[ubi] = Default_WB_Index;
									gubWriteBufferTableNext[ubi] = Default_WB_Index;
								}
							}
							gBQI.ubLinkFTLLast = gBQI.ubLinkLastUse;
							gubKeepLoadAlignmentBQLink = 0xFF;
							gubKeepLoadAlignmentBQNum = 0;
							//guwNonAlignBufferPTR = 0xFFFF;

							_DEBUG_TRACE((0xFE << 24) | (0xF << 16) | gBQI.uwWrite_tq_2_bq);

							while (gBQI.ubLinkNum) {
								gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
								M_SwitchTask();
							}
							gBQI.ulHostWriteWaitBufferFlag = 0;
							guwNonAlignBufferPTR = 0xFFFF;
#if ENABLE_PFAIL
							do {
								ftl_cal_write_buffer_window();
								if ((gBQI.uwWrite_buffer_remain > 0) || (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK)) { /* Wait for tq lock. */
									if (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK) {
										_DEBUG_TRACE((0xFD << 24) | (0x1 << 16) | (gBQI.uwWrite_buffer_tq + gBQI.uwWrite_tq_2_bq));
										if (gubNeedSendCmdComplete) {
											gubErrorCode = SET_ABRT;
											gubAbort_CMD = 1;
										}
										goto Discard_Redundant_NCQWCMD;
									}
									break;
								}
								M_SwitchTask();

							}
							while (1);
#endif
						}
						else if ((gBQI.BufferQueue[gBQI.ubLinkLastUse].uwBufferIndex != uwSRamBufferPTRTemp) || (gBQI.BufferQueue[gubKeepLoadAlignmentBQLink].ulDataMask & BIT0)) {
							guwNonAlignBufferPTR = gBQI.BufferQueue[gBQI.ubLinkLastUse].uwBufferIndex;
						}
						else {
							guwNonAlignBufferPTR = 0xFFFF;
						}
					}
#if (0==ENABLE_PFAIL)
					else {
						guwNonAlignBufferPTR = 0xFFFF; /* All BQ cleared, so reset ptr. */
					}
#endif
				}
			}
		}
		// set CMD Info
		if (gubNCQDoing) {
			ubNCQNextWRTag = ulNCQ_INFO.B.ubNCQ_NEXT_WR;
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulTrigTime = gulOperationTime;
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA = NLL[ubNCQNextWRTag];
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt = NSL[ubNCQNextWRTag] & 0x0000FFFF;
			if (WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt == 0) {
				WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt = 65536;
			}
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR = ((ubDDRMode || (guwDataInSDR4kNum > 0)) ? guwSDRBufferPTR : guwSRamBufferPTR);
			if (ubDDRMode == 0) {
#if ENABLE_PFAIL
				if (ubSeq && gubKeepLoadAlignmentBQNum && (gulKeepLoadAlignmentBQLBA == WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA))
#else
				if (ubSeq)
#endif
				{
					if (WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA & SectorsPer4KMask) {
						if ((ubPreTag != ubNCQNextWRTag) && ((WR_NCQ_CMD_INFO[ubPreTag].ulLBA + WR_NCQ_CMD_INFO[ubPreTag].ulSectorCnt) != WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA)) {
							if (ENABLE_DEBUG_NCQ_UART_W) {
								UartString("\nPreTag");
								Uart_Tx_DataHex(ubPreTag);
								UartString(" CurrentTag");
								Uart_Tx_DataHex(ubNCQNextWRTag);
							}
							M_AssertError(1);
						}
						WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR = ((WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR - SectorsPer4K) & ulWriteBufferMask) + ulWriteBufferStartIndex;
					}
					ubSeqCMD = 1;
					gubWSeq = 1;
				}
				else {
					ubSeqCMD = 0;
					if (WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt > 128) {
						gubWSeq = 1;
					}
					else {
						gubWSeq = 0;
					}
#if ENABLE_PFAIL
					if ((ubPreTag != 0xFF) && (WR_NCQ_CMD_INFO[ubPreTag].ulLBA == WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA)) {
						gBQI.ubLBAOverlap = ENABLE_PFAIL;
						//UartString("\nOVER2");
					}
#endif
				}
			}
			uwEC = NEC[ubNCQNextWRTag];
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC = uwEC;
#if (0==ENABLE_PFAIL)
			if (uwEC > 0xFE) {
				uwEC = 0;
			}
#endif
		}
		else {
			ubNCQNextWRTag = 0;
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulTrigTime = gulOperationTime;
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA = ulLBA = gulLBA.All;
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt = ulSectorCnt = gulSectorCnt;
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR = ((ubDDRMode || (guwDataInSDR4kNum > 0)) ? guwSDRBufferPTR : guwSRamBufferPTR);
			if (WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt == 0) {
				WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt = 65536;
			}
			uwEC = ((ulSectorCnt + (ulLBA & SectorsPer4KMask)) / 8);
			if ((ulSectorCnt + (ulLBA & SectorsPer4KMask)) & SectorsPer4KMask) {
				uwEC ++;
			}
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC = uwEC;
#if (0==ENABLE_PFAIL)
			if (uwEC > 0xFE) {
				uwEC = 0;
			}
#endif
			if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {
				gulLastWCmdLBA = ulLBA;
				guwLastWCmdSector = ulSectorCnt;
			}
		}
		// cppcheck-suppress arrayIndexOutOfBoundsCond
		//[cppcheck][ Either the condition 'ubNCQNextWRTag==63' is redundant or the array 'WR_NCQ_CMD_INFO[32]' is accessed at index 63, which is out of bounds.]
		UBYTE ubNCQNextWRTag_Temp;
		ubNCQNextWRTag_Temp = ubNCQNextWRTag;
		if ((WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA >= gulMediaSize.All) || (ubNCQNextWRTag_Temp == 0x3f)) {  //NeedReview
			//if ((WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA >= gulMediaSize.All) || (ubNCQNextWRTag == 0x3f)) {  //NeedReview
			ulNCQ_INFO_FailCheck.All = HL[HL_WR_NCQ_INFO];
			if (ulNCQ_INFO_FailCheck.B.ubNCQ_CMD_VLD_SUM || (ulNCQ_INFO_FailCheck.B.ubNCQ_NEXT_WR != 0x3F) || gubNeedChkDoneTag) { //未被清掉
				flaLEDBlink(0x1245);
			}
			if (gubNCQDoing == 0) {
				gubErrorCode = ID_ERR;
			}
			goto Discard_Redundant_NCQWCMD;
		}
		if (gubNeedWaitTQDone) {
			gubNeedWaitTQDone = 0;
			if (ulNCQ_INFO.B.ubNCQ_NEXT != ulNCQ_INFO.B.ubNCQ_NEXT_WR) {
				WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_BQDone = WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC;
				break;
			}
		}
		if (WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt >= 0x1000) {
			gubLargeSizeCmdCount++;
		}

		VT->guoTotalHostWriteSectorCnt += WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt;

		UWORD uwBufferEndPTR;
		uwBufferEndPTR = ((( WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA & SectorsPer4KMask) + WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR +  WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt) & ulWriteBufferMask) + ulWriteBufferStartIndex;
		if (ubDDRMode || (guwDataInSDR4kNum > 0)) {
			guwSDRBufferPTR = (((uwBufferEndPTR & (~SectorsPer4KMask)) + ((uwBufferEndPTR & SectorsPer4KMask) ? SectorsPer4K : 0)) & ulWriteBufferMask) + ulWriteBufferStartIndex;
		}
		else {
			guwSRamBufferPTR = (((uwBufferEndPTR & (~SectorsPer4KMask)) + ((uwBufferEndPTR & SectorsPer4KMask) ? SectorsPer4K : 0)) & ulWriteBufferMask) + ulWriteBufferStartIndex;
		}

		gulNCQCMDCnt ++;
		gulNCQWRCmdTriggerCnt++;
		gulWriteCmdsCount++;
		ubTrigCMDCount++;
		ubPreTag = ubNCQNextWRTag;
		ubNOCMDCnt = 0;

		//total cmd cnt
		if (AtaCfg.ulWriteTotalCmdCnt < 0xFFFFFFFF) {
			AtaCfg.ulWriteTotalCmdCnt++;
		}

		if (( WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA & SectorsPer4KMask) && (AtaCfg.ulWriteNon4KAlignedCmdCnt < 0xFFFFFFFF)) {
			AtaCfg.ulWriteNon4KAlignedCmdCnt++;
		}

	MARK_MULTI_TRIG:
		_ASSERT(0x1, (0U == gBQI.uwWrite_buffer_remain) || (gBQI.uwWrite_buffer_remain > gBQI.uwWrite_buffer_window));
		_ASSERT(0x2, (gBQI.ubWrite_TQ_lock & BG_TQ_LOCK));

		if (ubDDRMode || (guwDataInSDR4kNum > 0)) {
			if (((guwDataInSDR4kNum + WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC + ubDDR_TQ_4KCOUNT) > SDRAM_CACHE_4KNUM) || ((ubDDR_TQ_4KCOUNT + WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC) > MAX_WRITE_TRIG_4K_NUM)) {
				gubMultiTrigTag = ubNCQNextWRTag;
				if ((guwDataInSDR4kNum + WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC + ubDDR_TQ_4KCOUNT) > SDRAM_CACHE_4KNUM) {
					uwEC = SDRAM_CACHE_4KNUM - guwDataInSDR4kNum - ubDDR_TQ_4KCOUNT;
					if ((ubDDR_TQ_4KCOUNT + uwEC) > MAX_WRITE_TRIG_4K_NUM) {
						uwEC = MAX_WRITE_TRIG_4K_NUM - ubDDR_TQ_4KCOUNT;
					}
				}
				else {
					uwEC = MAX_WRITE_TRIG_4K_NUM - ubDDR_TQ_4KCOUNT;
				}

				HL[HL_TQ_CONTENT] = (H_MULTI_TRG | (uwEC << 24) | H_TQN_UPD_PTR | (ubNCQNextWRTag << 18) | H_BUF_BASE2 | (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
				WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_BQDone = 0;
				WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_MultiTrig = uwEC;
				gulNCQCMDDebug |= ((U32)BIT0 << ubNCQNextWRTag);
				if (ENABLE_DEBUG_NCQ_UART_W) {
					UartLongHex(gulNCQCMDCnt);
					UartString("    W	Tag: ");
					Uart_Tx_DataHex(ubNCQNextWRTag);
					UartString("	  LBA:");
					UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA);
					UartString("	   SecCnt:");
					UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt);
					UartString("	   ToTalEC:");
					UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC);
					UartString("	   EC:");
					UartWordHex(uwEC);
					UartString("	   PTR:");
					UartWordHex((WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
					UartString("	   4KCount:");
					UartWordHex(guwDataInSDR4kNum + ubDDR_TQ_4KCOUNT);
					UartString("	   M");
					if (ubSeq) {
						UartString("	S");
					}
					UartString("\n");
				}
				break;
			}
		}
		else {
			if (guwNonAlignBufferPTR != 0xFFFF) {
				UWORD uwBeginBufferPTR = WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR;
#if ENABLE_PFAIL
				if (((uwBeginBufferPTR < guwNonAlignBufferPTR) && (guwNonAlignBufferPTR < uwBufferEndPTR)) || \
				        ((guwNonAlignBufferPTR < uwBufferEndPTR) && (uwBufferEndPTR < uwBeginBufferPTR)) || \
				        ((uwBufferEndPTR < uwBeginBufferPTR) && (uwBeginBufferPTR < guwNonAlignBufferPTR)) || \
				        (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC >= gBQI.uwWrite_buffer_remain))
#else
				if (((uwBeginBufferPTR < guwNonAlignBufferPTR) && (guwNonAlignBufferPTR < uwBufferEndPTR)) || ((guwNonAlignBufferPTR < uwBufferEndPTR) && (uwBufferEndPTR < uwBeginBufferPTR)) || ((uwBufferEndPTR < uwBeginBufferPTR) && (uwBeginBufferPTR < guwNonAlignBufferPTR)) || (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC >= BUFFER2_WRITE_4KNUM))
#endif
				{
					gubMultiTrigTag = ubNCQNextWRTag;
					uwEC = (((guwNonAlignBufferPTR - uwBeginBufferPTR) & BUFFER2_WRITE_MASK) / 8);
					//M_AssertError(uwEC == 0);
					if (uwEC == 0) {
						uwEC  = BUFFER2_WRITE_4KNUM - 1;
					}
#if ENABLE_PFAIL
					if (uwEC > gBQI.uwWrite_buffer_remain) {
						uwEC = gBQI.uwWrite_buffer_remain;
					}
#endif
					HL[HL_TQ_CONTENT] = (H_MULTI_TRG | (uwEC << 24) | H_TQN_UPD_PTR | (ubNCQNextWRTag << 18) | (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
#if ENABLE_PFAIL
					gBQI.uwWrite_buffer_remain -= uwEC;
					gBQI.uwWrite_buffer_tq += uwEC;
#endif
					WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_BQDone = 0;
					WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_MultiTrig = uwEC;
					gulNCQCMDDebug |= ((U32)BIT0 << ubNCQNextWRTag);
					_DEBUG_TRACE((0x80 << 24) | (uwEC << 16) | (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
					if (ENABLE_DEBUG_NCQ_UART_W) {
						UartLongHex(gulNCQCMDCnt);
						UartString("    W	Tag: ");
						Uart_Tx_DataHex(ubNCQNextWRTag);
						UartString("	   LBA:");
						UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA);
						UartString("	   SecCnt:");
						UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt);
						UartString("	   TotalEC:");
						UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC);
						UartString("	   PTR:");
						UartWordHex((WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
						UartString("	   NalignPTR:");
						UartWordHex(guwNonAlignBufferPTR / 8);
						UartString("	   EC_T:");
						UartWordHex(uwEC);
						UartString("		M");
						if (ubSeq) {
							UartString("	S");
						}
						UartString("\n");
					}
					break;
				}
			}
			else {
				if (((ubSeq == 0) && (WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA & SectorsPer4KMask)) || ((WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA + WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt) & SectorsPer4KMask)) {
					//if (WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA & SectorsPer4KMask) {
					if (WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA & SectorsPer4KMask) {
						guwNonAlignBufferPTR = WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR;
					}
					else {
						guwNonAlignBufferPTR = ((WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR + (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC * 8)) & ulWriteBufferMask) + ulWriteBufferStartIndex;
					}

#if ENABLE_PFAIL
					if (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC > gBQI.uwWrite_buffer_remain)
#else
					if (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC > BUFFER2_WRITE_4KNUM)

#endif
					{
						gubMultiTrigTag = ubNCQNextWRTag;
#if ENABLE_PFAIL
						uwEC = gBQI.uwWrite_buffer_remain;
#else
						uwEC = BUFFER2_WRITE_4KNUM;
#endif
						HL[HL_TQ_CONTENT] = (H_MULTI_TRG | (uwEC << 24) | H_TQN_UPD_PTR | (ubNCQNextWRTag << 18) | (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
#if ENABLE_PFAIL
						gBQI.uwWrite_buffer_remain = 0;
						gBQI.uwWrite_buffer_tq += uwEC;
#endif
						WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_BQDone = 0;
						WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_MultiTrig = uwEC;
						gulNCQCMDDebug |= ((U32)BIT0 << ubNCQNextWRTag);
						_DEBUG_TRACE((0x81 << 24) | (uwEC << 16) | (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
						if (ENABLE_DEBUG_NCQ_UART_W) {
							UartLongHex(gulNCQCMDCnt);
							UartString("    W	Tag: ");
							Uart_Tx_DataHex(ubNCQNextWRTag);
							UartString("	   LBA:");
							UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA);
							UartString("	   SecCnt:");
							UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt);
							UartString("	   TotalEC:");
							UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC);
							UartString("	   PTR:");
							UartWordHex((WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
							UartString("	   NalignPTR:");
							UartWordHex(guwNonAlignBufferPTR / 8);
							UartString("	   EC_T:");
							UartWordHex(uwEC);
							UartString("		M");
							if (ubSeq) {
								UartString("	S");
							}
							UartString("\n");
						}
						break;
					}
				}
			}
		}

#if ENABLE_PFAIL
		if (uwEC > gBQI.uwWrite_buffer_remain) {
			uwEC = gBQI.uwWrite_buffer_remain;
			gubMultiTrigTag = ubNCQNextWRTag;
			HL[HL_TQ_CONTENT] = (H_MULTI_TRG | (uwEC << 24) | H_TQN_UPD_PTR | (ubNCQNextWRTag << 18) | ((ubDDRMode || (guwDataInSDR4kNum > 0)) ? H_BUF_BASE2 : 0) | (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
			gBQI.uwWrite_buffer_remain = 0U;
			gBQI.uwWrite_buffer_tq += uwEC;
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_BQDone = 0;
			WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_MultiTrig = uwEC;
			gulNCQCMDDebug |= ((U32)BIT0 << ubNCQNextWRTag);
			_DEBUG_TRACE((0x82 << 24) | (uwEC << 16) | (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
			break;
		}
#endif

		HL[HL_TQ_CONTENT] = H_TQN_UPD_PTR | (ubNCQNextWRTag << 18) | (uwEC << 24) | ((ubDDRMode || (guwDataInSDR4kNum > 0)) ? H_BUF_BASE2 : 0) | (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8);
#if ENABLE_PFAIL
		gBQI.uwWrite_buffer_remain -= uwEC;
		gBQI.uwWrite_buffer_tq += uwEC;
#endif
		WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC_BQDone = 0;
		ubDDR_TQ_4KCOUNT += WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC;
		gulNCQCMDDebug |= ((U32)BIT0 << ubNCQNextWRTag);
		_DEBUG_TRACE((0x83 << 24) | (uwEC << 16) | (WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));

		if (Failed_Test_CRC_WR) {
			UBYTE ubFailGen;
			ubFailGen = getIntRandValue(0x100);	//rand() & 0xff

			if (ubFailGen == 0) {
				HW[HW_ERR_CTRL] |= (H_REG_CRC_DATAERR_EN | H_REG_CRC_DIR_RECV);
			}
		}

		if (ENABLE_DEBUG_NCQ_UART_W) {
			UartLongHex(gulNCQCMDCnt);
			UartString("    W	Tag: ");
			Uart_Tx_DataHex(ubNCQNextWRTag);
			UartString("	   LBA:");
			UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA);
			UartString("	   SecCnt:");
			UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt);
			UartString("	   TotalEC:");
			UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC);
			UartString("	   PTR:");
			UartWordHex((WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwBufferPTR / 8));
			UartString("	   NalignPTR:");
			UartWordHex(guwNonAlignBufferPTR / 8);
			if (ubSeq) {
				UartString("	S");
			}
			UartString("\n");
		}
		if (gubIsWriteProtect && ENABLE_DEBUG_UART_WRITEPROTECT) {
			UartString("  NCQ:");
			Uart_Tx_DataHex(gubNCQDoing);
			UartString("  Cache:");
			Uart_Tx_DataHex(gubWriteCacheEnable);
			UartString("  LBA:");
			UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA);
			UartString("  SecCnt:");
			UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt);
			UartString("  TotalEC:");
			UartLongHex(WR_NCQ_CMD_INFO[ubNCQNextWRTag].uwEC);
		}
#if ENABLE_PFAIL
		if ((gubNCQDoing == 0) || (gubWriteCacheEnable == 0)
		        || ((!gubWriteProtectWriteDummy) && gubIsWriteProtect) || (0U == gBQI.uwWrite_buffer_remain) || (gBQI.ubWrite_TQ_lock))

#else
		if ((gubNCQDoing == 0) || (gubWriteCacheEnable == 0)
		        || ((!gubWriteProtectWriteDummy) && gubIsWriteProtect))

#endif
		{
			if (gubIsWriteProtect && ENABLE_DEBUG_UART_WRITEPROTECT) {
				UartString("\n\rBreakWriteByWriteProtect");
				UartString("  NCQ:");
				Uart_Tx_DataHex(gubNCQDoing);
				UartString("  Cache:");
				Uart_Tx_DataHex(gubWriteCacheEnable);
			}
			break;
		}

		ulNCQ_INFO.All = HL[HL_WR_NCQ_INFO];
		if (ulNCQ_INFO.B.ubNCQ_NEXT_WR  != 0x3F) {
			HL[HL_CON_LBA_CHK_LBA] = WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA + WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt;
			while (HL[HL_WR_NCQ_INFO] & H_CON_LBA_CHK_TRG);
			ulNCQ_INFO.All = HL[HL_WR_NCQ_INFO];

			if ((gubLBAOverlap == 0) && (ulNCQ_INFO.B.ubCON_LBA_TAG != 0x3F)) {
				if (ENABLE_KEEP_LOADALIGN_BQ) {
					ubSeq = 1;
				}
				ulNCQ_INFO.B.ubNCQ_NEXT_WR = ulNCQ_INFO.B.ubCON_LBA_TAG;
			}
			else {
				ubSeq = 0;
			}
		}
	}


	UBYTE ubSetBQTag;
	while (1) {
		U32 ulRemainSectorCnt;
		SATA_MSG_TYPE3 ulSATA_MSG_TYPE3;
		UBYTE ubMultiTrigDone = 0;
		if (gubNCQDoing == 0) {
			ulRemainSectorCnt = HL[HL_CMD_REMAIN_SEC];
		}
		ulSATA_MSG_TYPE3.All = gulSATA_MSG_TYPE3[0].All;
		if (gubNCQDoing) {

			ubSetBQTag = ulSATA_MSG_TYPE3.B.ubCurrentTag;
		}
		else {
			ubSetBQTag = 0;
		}
		gulFTLStateForTimeout |= VT->gulFTLState.All;
	MARK_CHECK_DATA:
		if (DCCMSATAB[SATAB_RD_CNT]) {
			/*
			Question:當RD_CNT的值從0變1的時候，是表示當前TAG的TQ已經收完資料，那此時的CRC檢查是已經完成還是只有資料Ready?
			Ans:當TQ完成時, CRC已檢查完了 (Jianhao)
			*/
			if (gubNCQDoing) {
				ubSetBQTag = (*DONETAG);
				gubLastDoneTag = ubSetBQTag;
			}
			else {
				ubSetBQTag = 0;
				gubLastDoneTag = 0x3F;
			}

			if (ubSetBQTag == gubMultiTrigTag) {
				gubMultiTrigTag = 0xFF;
			}
			gulNCQCMDDebug &= (~((U32)BIT0 << ubSetBQTag));
			gubNCQDoingTag = ubSetBQTag;
			if (WR_NCQ_CMD_INFO[ubSetBQTag].ulSectorCnt >= 0x1000) {
				gubLargeSizeCmdCount--;
			}
			if (((M_CommandOperationTime(WR_NCQ_CMD_INFO[ubSetBQTag].ulTrigTime)) > guwNCQTimeoutThreshold)) {
				if (ENABLE_DEBUG_NCQ_UART_W) {
					UartString(" W Command Timeout Tag:");
					Uart_Tx_DataHex(ubSetBQTag);
					UartString("	   LBA:");
					UartLongHex(WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA);
					UartString("	   SecCnt:");
					UartLongHex(WR_NCQ_CMD_INFO[ubSetBQTag].ulSectorCnt);
					UartString("	   ECDone:");
					UartWordHex(WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone);
					UartString("	   Elapsed:");
					UartLongHex(M_CommandOperationTime(WR_NCQ_CMD_INFO[ubSetBQTag].ulTrigTime));
					UartString("	   *\n");
				}

				ftlRecordFailLog(NCQTimeoutLogType, 0x1, ubSetBQTag, IsWriteNCQ, DONTCARE, DONTCARE);
			}
			ulLBA =  WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA + (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone ? ((WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone * 8) - (WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA & SectorsPer4KMask)) : 0);
			if (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone == WR_NCQ_CMD_INFO[ubSetBQTag].uwEC) { // do not need to set BQ
				_DEBUG_TRACE((0x40 << 24) | (ubSetBQTag << 16) | ((UWORD)gubPfail_state << 8) | WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone | 0x1000U);
				ulSectorCnt = 0;
				ubSetBQTag = 0xFF;
				if (ENABLE_DEBUG_NCQ_UART_W) {
					UartString("																		");
					UartString("	   *\n");
				}
			}
			else {
				ulSectorCnt = WR_NCQ_CMD_INFO[ubSetBQTag].ulSectorCnt - (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone ? (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone * 8 - (WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA & SectorsPer4KMask)) : 0);
				uwBufferPTR = (((ulLBA & SectorsPer4KMask) + WR_NCQ_CMD_INFO[ubSetBQTag].uwBufferPTR + (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone * 8)) & ulWriteBufferMask) + ulWriteBufferStartIndex;
				M_AssertError((ulSectorCnt / 8) > WR_NCQ_CMD_INFO[ubSetBQTag].uwEC);
				WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone = WR_NCQ_CMD_INFO[ubSetBQTag].uwEC;
				if (ENABLE_DEBUG_NCQ_UART_W) {
					UartString("																		Tag:");
					Uart_Tx_DataHex(ubSetBQTag);
					UartString("	   ECDone:");
					UartWordHex(WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone);
					UartString("	   *\n");
				}
				_DEBUG_TRACE((0x41 << 24) | (ubSetBQTag << 16) | ((UWORD)gubPfail_state << 8) | WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone | 0x5000U);
			}
			gulNCQWRCmdTriggerCnt--;
			M_AssertError((gulNCQWRCmdTriggerCnt == 0) && (gulNCQCMDDebug != 0));
			DCCMSATAB[SATAB_RPIU] = 1;

			if (ENABLE_DEBUG_CHECK_LOADALIGN_E3D) {
				U32 uli;
				for (uli = 0; uli < ulSectorCnt; uli++) {
					mDMAC_E3D((uwBufferPTR + uli) * 512, 512, ulLBA + uli, 1);
				}
			}
			gulWRWaitCnt = 0;
			if (gubLBAOverlap && (gubNeedWaitTQDone == 0)) {
				if (HB[HB_NCQ_OVLP_NUM] == 0) {
					gubLBAOverlap = 0;
					if (ENABLE_D2HSDBByHW) { //Enable Write Hardware D2H
						M_EnableHWD2HSDB_W();
					}
					if (HB[HB_NCQ_OVLP_NUM]) {
						//UartString(" D ");
						gubLBAOverlap = 1;
						gubNeedWaitTQDone = 1;
					}
				}
			}
#if ENABLE_PFAIL
			HB[HB_FLAG_CTRL] = ubDDT_temp;
			HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
			ubDDT_Enable = 1;
#endif
		}
		else if (gulNCQWRCmdTriggerCnt && ((gubNCQDoing == 0) || (gubLastDoneTag == 0x3F) || (gubLastDoneTag == ubSetBQTag) || (ulSATA_MSG_TYPE3.B.ubLatestDoneTag ==  ulSATA_MSG_TYPE3.B.ubCurrentTag))
		         && ((gubMultiTrigTag != ubSetBQTag) || (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone != WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_MultiTrig))
		         && (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone != WR_NCQ_CMD_INFO[ubSetBQTag].uwEC)) {
			if ((gubNCQDoing == 0) || ((HB[HB_TQ_ACT]&H_TQ_ACT) && ((HB[HB_NCQ_SET] & 0x1F) == ulSATA_MSG_TYPE3.B.ubCurrentTag))) {
				ulRemainSectorCnt = HL[HL_CMD_REMAIN_SEC];
				if (gubNeedChkDoneTag == 0) {
					goto Discard_Redundant_NCQWCMD;
				}
				ulLBA = WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA;
				U32 ulSectorCntDone = (WR_NCQ_CMD_INFO[ubSetBQTag].ulSectorCnt - ulRemainSectorCnt);
				UWORD uwEC_Done = ((ulSectorCntDone + (ulLBA & SectorsPer4KMask)) / 8);
				U32 ulSectorTrig = ((WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_MultiTrig * 8) - (ulLBA & SectorsPer4KMask));
				if ((ulRemainSectorCnt < WR_NCQ_CMD_INFO[ubSetBQTag].ulSectorCnt) && (uwEC_Done > WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone)) {
					////////////////////
					UBYTE ubHandleEC = 0;
					UBYTE ubCheckEC;
					if (gubMultiTrigTag == ubSetBQTag) {
						ubHandleEC = (uwEC_Done  -  WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone);
					}
					else {
						if (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone == 0) {
							ubCheckEC = (gub4kEntrysPerPlane - ((WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA / 8) & gub4kEntrysPerPlaneMask));
							if ((uwEC_Done  -  WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone) < ubCheckEC) {
								goto MARK_CHECK_FAIL;
							}
							else {
								ubHandleEC = ubCheckEC;
							}
						}
						else {
							ubCheckEC = 4;
							if ((uwEC_Done  -  WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone) < ubCheckEC) {
								goto MARK_CHECK_FAIL;
							}
							else {
								ubHandleEC = (uwEC_Done  -  WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone) & 0xFC; //4的倍數
							}
						}
					}
					if ((WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone + ubHandleEC) > WR_NCQ_CMD_INFO[ubSetBQTag].uwEC) {
						gulTemp = ulRemainSectorCnt;
						M_AssertError(1);
					}
					////////////////////
					ulLBA = WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA + (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone ? ((WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone * 8) - (WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA & SectorsPer4KMask)) : 0);
					ulSectorCnt = (ubHandleEC * 8) - (ulLBA & SectorsPer4KMask);
					uwBufferPTR = (((ulLBA & SectorsPer4KMask) + WR_NCQ_CMD_INFO[ubSetBQTag].uwBufferPTR + (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone * 8)) & ulWriteBufferMask) + ulWriteBufferStartIndex;
					WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone += ubHandleEC;
					_DEBUG_TRACE((0x42 << 24) | (ubSetBQTag << 16) | ((UWORD)gubPfail_state << 8) | WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone | 0x2000U);

					if (ENABLE_DEBUG_NCQ_UART_W) {
						UartString("																		");
						Uart_Tx_DataHex(ubSetBQTag);
						UartString("	   ");
						UartWordHex(WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone);
						UartString("	   M=L\n");
					}
				}
				else if ((((ulSectorTrig - ulSectorCntDone) < 16) || (ulSectorTrig < 16)) && (ubMultiTrigDone == 0)) {
					//ubMultiTrigDone = 1;
					goto MARK_CHECK_FAIL;
				}
				else {
					/*ubSetBQTag = 0xFF;
					M_SwitchTask();*/
					goto MARK_CHECK_FAIL;
				}
			}
			else {
				goto MARK_CHECK_FAIL;
			}
		}
		else if ((gubLastDoneTag == ulSATA_MSG_TYPE3.B.ubLatestDoneTag)
		         && (ulSATA_MSG_TYPE3.B.ubLatestDoneTag !=  ulSATA_MSG_TYPE3.B.ubCurrentTag)
		         && (ulSATA_MSG_TYPE3.B.uwReceived_EC > WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone)
		         && (ulSATA_MSG_TYPE3.B.uwReceived_EC != WR_NCQ_CMD_INFO[ubSetBQTag].uwEC)) {
			/////////////
			UBYTE ubHandleEC = 0;
			UBYTE ubCheckEC;

			if (gubMultiTrigTag == ubSetBQTag) {
				ubHandleEC = (ulSATA_MSG_TYPE3.B.uwReceived_EC -  WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone);
			}
			else {
				if (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone == 0) {
					ubCheckEC = (gub4kEntrysPerPlane - ((WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA / 8) & gub4kEntrysPerPlaneMask));
					if ((ulSATA_MSG_TYPE3.B.uwReceived_EC -  WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone) < ubCheckEC) {
						goto MARK_CHECK_FAIL;
					}
					else {
						ubHandleEC = ubCheckEC;
					}
				}
				else {
					ubCheckEC = 4;
					if ((ulSATA_MSG_TYPE3.B.uwReceived_EC -  WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone) < ubCheckEC) {
						goto MARK_CHECK_FAIL;
					}
					else {
						ubHandleEC = (ulSATA_MSG_TYPE3.B.uwReceived_EC -  WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone) & 0xFC; //4的倍數
					}
				}
			}
			/////////////
			ulLBA = WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA + (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone ? ((WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone * 8) - (WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA & SectorsPer4KMask)) : 0);
			if (gubNCQDoing == 0) {
				if (ulSATA_MSG_TYPE3.B.uwReceived_EC != ((WR_NCQ_CMD_INFO[ubSetBQTag].ulSectorCnt - ulRemainSectorCnt + (WR_NCQ_CMD_INFO[ubSetBQTag].ulLBA & SectorsPer4KMask)) / 8 )) {
					goto MARK_CHECK_FAIL;
				}
			}
			ulSectorCnt = (ubHandleEC * 8) - (ulLBA & SectorsPer4KMask);
			uwBufferPTR = (((ulLBA & SectorsPer4KMask) + WR_NCQ_CMD_INFO[ubSetBQTag].uwBufferPTR + (WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone * 8)) & ulWriteBufferMask) + ulWriteBufferStartIndex;
			WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone += ubHandleEC;
			_DEBUG_TRACE((0x43 << 24) | (ubSetBQTag << 16) | ((UWORD)gubPfail_state << 8) | WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone | 0x3000U);

			if (ENABLE_DEBUG_NCQ_UART_W) {
				UartString("																		");
				Uart_Tx_DataHex(ubSetBQTag);
				UartString("	   ");
				UartWordHex(ulSATA_MSG_TYPE3.B.uwReceived_EC);
				UartString("	   ");
				UartLongHex(ulLBA);
				UartString("	   ");
				UartLongHex(ulSectorCnt);
				UartString("\n");
			}
		}
		else {
		MARK_CHECK_FAIL:
			ubTempMultiTrigTag = gubMultiTrigTag;   //SATA_ISR Issue (gubMultiTrigTag will be set to 0xff)
			if (gubMultiTrigTag != 0xFF) {
				if ((gubNCQDoing == 0) || ((HB[HB_NCQ_SET] & 0x1F) == ubTempMultiTrigTag)) {
					if ((WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig - WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_BQDone) <= 2) {
						ulLBA = WR_NCQ_CMD_INFO[ubTempMultiTrigTag].ulLBA;
						U32 ulSectorCntDone = (WR_NCQ_CMD_INFO[ubTempMultiTrigTag].ulSectorCnt - HL[HL_CMD_REMAIN_SEC]);
						U32 ulSectorTrig = ((WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig * 8) - (ulLBA & SectorsPer4KMask));
						UWORD uwRemainEC = (WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC - WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig);
						UBYTE ubBlockEC;
						ubMultiTrigDone = 0;
						if ((ulSectorTrig - ulSectorCntDone) < 16) {
#if ENABLE_PFAIL
							if ( /*(ulSectorTrig < 16) || (uwRemainEC<=2) || */(WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig == WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_BQDone)) {
								ubMultiTrigDone = 1;
								ubBlockEC = (((ulSectorTrig - ulSectorCntDone) + (ulLBA & SectorsPer4KMask)) / 8);
								if (gubNCQDoing) {
									/* Only switch on/off for NCQ write; write DMA ext will send more DMA-activate if switch on/off. */
									HB[HB_FLAG_CTRL] = ubDDT_temp;
									HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
									ubDDT_Enable = 1;
								}
							}
							else if (ubDDT_Enable) {
								_DEBUG_TRACE((0x44 << 24) | (ubSetBQTag << 16) | ((UWORD)gubPfail_state << 8) | WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone | 0x4000U);
								HB[HB_FLAG_CTRL] &= (~H_DDT_EN);//because here pass sectors one by one (DDT needs 8k one time)
								HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);
								ubDDT_Enable = 0;
							}
#else
							if ( (ulSectorTrig < 16) || (WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_BQDone == ((ulSectorCntDone + (ulLBA & SectorsPer4KMask)) / 8))) {
								ubMultiTrigDone = 1;
								ubBlockEC = (((ulSectorTrig - ulSectorCntDone) + (ulLBA & SectorsPer4KMask)) / 8);
							}
#endif

						}
						if (ubMultiTrigDone) {
							if (ubDDRMode || (guwDataInSDR4kNum > 0)) {
								//UWORD uwRemainEC = (WR_NCQ_CMD_INFO[gubMultiTrigTag].uwEC - WR_NCQ_CMD_INFO[gubMultiTrigTag].uwEC_MultiTrig);
								UWORD uwMultiTrigBufPTR = ((H_BUF_BASE2 | WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwBufferPTR / 8) + WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig) & SDRAM_CACHE_4KNUM_MASK;

								if ((guwDataInSDR4kNum + ubBlockEC + uwRemainEC) > SDRAM_CACHE_4KNUM) {
									uwEC = SDRAM_CACHE_4KNUM - guwDataInSDR4kNum - ubBlockEC;
									if (uwEC > MAX_WRITE_TRIG_4K_NUM) {
										uwEC = MAX_WRITE_TRIG_4K_NUM;
									}
									HL[HL_TQ_CONTENT_31] = (H_MULTI_TRG | (uwEC << 24) | H_TQN_UPD_PTR | (ubTempMultiTrigTag << 18) | H_BUF_BASE2 | uwMultiTrigBufPTR);
									WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig += uwEC;
									if (ENABLE_DEBUG_NCQ_UART_W) {
										UartString("    W	Tag: ");
										Uart_Tx_DataHex(gubMultiTrigTag);
										UartString("		");
										Uart_Tx_DataHex(ubBlockEC);
										UartString("		");
										UartLongHex(ulSectorTrig);
										UartString("		");
										UartLongHex(ulSectorCntDone);
										UartString("		");
										UartWordHex(uwEC);
										UartString("		");
										UartWordHex(uwMultiTrigBufPTR);
										UartString("		M2");
										UartString("\n");
									}
								}
								else if (uwRemainEC > MAX_WRITE_TRIG_4K_NUM) {
									uwEC = MAX_WRITE_TRIG_4K_NUM;
									HL[HL_TQ_CONTENT_31] = (H_MULTI_TRG | (uwEC << 24) | H_TQN_UPD_PTR | (ubTempMultiTrigTag << 18) | H_BUF_BASE2 | uwMultiTrigBufPTR);
									WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig += uwEC;
									if (ENABLE_DEBUG_NCQ_UART_W) {
										UartString("    W	Tag: ");
										Uart_Tx_DataHex(gubMultiTrigTag);
										UartString("		");
										UartWordHex(uwEC);
										UartString("		");
										UartWordHex(uwMultiTrigBufPTR);
										UartString("		M1");
										UartString("\n");
									}
								}
								else {
									uwEC = uwRemainEC;
									HL[HL_TQ_CONTENT_31] = (H_MULTI_TRG | (uwEC << 24) | H_TQN_UPD_PTR | (ubTempMultiTrigTag << 18) | H_BUF_BASE2 | uwMultiTrigBufPTR);
									WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig = WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC;
									gubMultiTrigTag = 0xFF;
									if (ENABLE_DEBUG_NCQ_UART_W) {
										UartString("						F");
										UartString("		");
										UartWordHex(uwMultiTrigBufPTR);
										UartString("\n");
									}
								}
							}
							else {
								if (WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_BQDone) {
#if ENABLE_PFAIL
									UWORD uwMultiTrigBufPTR = (((WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwBufferPTR / 8) + WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig) & BUFFER2_WRITE_4KNUM_MASK) + BUFFER2_WRITE_4K_START_INDEX;
#endif
									if (ENABLE_KEEP_WBQ) {
										if (gubKeepWBQNum) {
											gBQI.ubLinkNum += gubKeepWBQNum;
											gBQI.ubWLinkNum += gubKeepWBQNum;
											gubKeepWBQNum = 0;
											gubKeepWBQSectorCnt = 0;
											gubKeepWBQ4KCnt = 0;
											gulWriteBufferBitMap = 0;
											UBYTE ubi;
											for (ubi = 0; ubi < 32; ubi++) {
												gubWriteBufferTableHead[ubi] = Default_WB_Index;
												gubWriteBufferTableNext[ubi] = Default_WB_Index;
											}
											if (gubKeepLoadAlignmentBQNum) {
												gubFirstKeepBQLink = gubKeepLoadAlignmentBQLink;
												gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
											}
											else {
												gBQI.ubLinkFTLLast = gBQI.ubLinkLastUse;
											}
										}
									}
#if (0==ENABLE_PFAIL)
									while (gBQI.ubLinkNum) {
										gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
										M_SwitchTask();
									}
									gBQI.ulHostWriteWaitBufferFlag  = 0;
#else
									if ((uwRemainEC > 0) && (0 == gBQI.uwWrite_buffer_remain)) {
										_DEBUG_TRACE((0xFE << 24) | (0xC << 16) | gBQI.uwWrite_tq_2_bq);

										ub4kNum = gBQI.ubForce_clean_wbq_threshold;
										do {
											gBQI.ubForce_clean_wbq_threshold = gub4kEntrysPerPlane;
											M_SwitchTask();

											ftl_cal_write_buffer_window();
											if ((gBQI.uwWrite_buffer_remain > 0) ||  (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK)) { /* Wait for tq lock. */
												gBQI.ubForce_clean_wbq_threshold = ub4kNum;
												if (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK) {
													_DEBUG_TRACE((0xFD << 24) | (0x2 << 16) | (gBQI.uwWrite_buffer_tq + gBQI.uwWrite_tq_2_bq));
													goto Discard_Redundant_NCQWCMD;
												}
												break;
											}

										}
										while (1);
									}

									if  (uwRemainEC > gBQI.uwWrite_buffer_remain) {
										uwEC = gBQI.uwWrite_buffer_remain;
										HL[HL_TQ_CONTENT_31] = (H_MULTI_TRG | (uwEC << 24) | H_TQN_UPD_PTR | (ubTempMultiTrigTag << 18) /* | ((ubDDRMode || (guwDataInSDR4kNum > 0)) ? H_BUF_BASE2 : 0)*/ | uwMultiTrigBufPTR);
										gBQI.uwWrite_buffer_remain = 0;
										gBQI.uwWrite_buffer_tq += uwEC;
										_DEBUG_TRACE((0x84 << 24) | (uwEC << 16) | uwMultiTrigBufPTR);
										WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig += uwEC;
									}
									else
#endif
									{
										if (uwRemainEC > 0) {
											HL[HL_TQ_CONTENT_31] &= 0x00FFFFFF;
#if ENABLE_PFAIL
											gBQI.uwWrite_buffer_remain -= uwRemainEC;
											gBQI.uwWrite_buffer_tq += uwRemainEC;
#endif
											_DEBUG_TRACE((0x85 << 24) | (uwRemainEC << 16) | uwMultiTrigBufPTR);
										}
										WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC_MultiTrig = WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwEC;
										if ((ubSeqCMD == 0) && (WR_NCQ_CMD_INFO[ubTempMultiTrigTag].ulLBA  & SectorsPer4KMask)) {
											guwNonAlignBufferPTR = WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwBufferPTR;
										}
										else if ((WR_NCQ_CMD_INFO[ubTempMultiTrigTag].ulLBA + WR_NCQ_CMD_INFO[ubTempMultiTrigTag].ulSectorCnt) & SectorsPer4KMask) {
											UWORD uwBufferEndPTR = ((( WR_NCQ_CMD_INFO[ubTempMultiTrigTag].ulLBA & SectorsPer4KMask) + WR_NCQ_CMD_INFO[ubTempMultiTrigTag].uwBufferPTR +  WR_NCQ_CMD_INFO[ubTempMultiTrigTag].ulSectorCnt) & BUFFER2_WRITE_MASK) + BUFFER2_WRITE_START_INDEX;
											guwNonAlignBufferPTR = uwBufferEndPTR & (~(UWORD)SectorsPer4KMask);
										}
										else {
											guwNonAlignBufferPTR = 0xFFFF;
										}
#if (0==ENABLE_PFAIL)
										/* no switch tag for pfail. */
										gubMultiTrigTag = 0xFF;
#endif
										if (ENABLE_DEBUG_NCQ_UART_W) {
											UartString("						F");
											UartString("\n");
										}
									}
								}
							}
						}
					}
				}
			}


			ubSetBQTag = 0xFF;
			gulWRWaitCnt++;
			if (ENABLE_KEEP_WBQ) {
				UBYTE ubLastBQ4kNum = 0;
				if (gubKeepLoadAlignmentBQNum) {
					ubLastBQ4kNum = gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum;
				}
				if ((gubKeepWBQSectorCnt >= gubSectorsPerPlane) || ((gubKeepWBQ4KCnt + ubLastBQ4kNum) >= 11)) {
					gBQI.ubLinkNum += gubKeepWBQNum;
					gBQI.ubWLinkNum += gubKeepWBQNum;
					gubKeepWBQNum = 0;
					gubKeepWBQSectorCnt = 0;
					gubKeepWBQ4KCnt = 0;
					gulWriteBufferBitMap = 0;
					UBYTE ubi;
					for (ubi = 0; ubi < 32; ubi++) {
						gubWriteBufferTableHead[ubi] = Default_WB_Index;
						gubWriteBufferTableNext[ubi] = Default_WB_Index;
					}
					if (gubKeepLoadAlignmentBQNum) {
						gubFirstKeepBQLink = gubKeepLoadAlignmentBQLink;
						gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
					}
					else {
						gBQI.ubLinkFTLLast = gBQI.ubLinkLastUse;
					}
				}
			}
			/*if (gulWRWaitCnt == 100) {
				gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
				gulWRWaitCnt = 0;
			}*/
			M_SwitchTask();
			ftl_cal_write_buffer_window();

		Discard_Redundant_NCQWCMD:
			//			if (gulNCQWRCmdTriggerCnt < 32) {
			ulNCQ_INFO.All = HL[HL_WR_NCQ_INFO];
			if (gulNCQWRCmdTriggerCnt == 0) {
				ubNOCMDCnt++;
				if (gubLBAOverlap && ((ulNCQ_INFO.B.ubNCQ_NEXT != 0x3F) && (ulNCQ_INFO.B.ubNCQ_NEXT_WR != ulNCQ_INFO.B.ubNCQ_NEXT))) {
					break;
				}
#if ENABLE_PFAIL
				else if ((gubNCQDoing == 0) || ubStopWrite || (gubWriteCacheEnable == 0)
				         || (((ulNCQ_INFO.B.ubNCQ_NEXT != 0x3F) && (ulNCQ_INFO.B.ubNCQ_NEXT_WR == 0x3F)) || \
				             (gubHostCmdIn) || (ubNOCMDCnt > 100)) || ((!gubWriteProtectWriteDummy) && gubIsWriteProtect) || (gBQI.ubWrite_TQ_lock))
#else
				else if ((gubNCQDoing == 0) || ubStopWrite || (gubWriteCacheEnable == 0)
				         || (((ulNCQ_INFO.B.ubNCQ_NEXT != 0x3F) && (ulNCQ_INFO.B.ubNCQ_NEXT_WR == 0x3F)) || \
				             (gubHostCmdIn) || (ubNOCMDCnt > 100)) || ((!gubWriteProtectWriteDummy) && gubIsWriteProtect))

#endif

				{
					break;
				}
				else {
					if (ulNCQ_INFO.B.ubNCQ_NEXT_WR != 0x3F) {
						HL[HL_CON_LBA_CHK_LBA] = WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA + WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt;
						while (HL[HL_WR_NCQ_INFO] & H_CON_LBA_CHK_TRG);

						ulNCQ_INFO.All = HL[HL_WR_NCQ_INFO];
						if ((gubLBAOverlap == 0) && (ulNCQ_INFO.B.ubCON_LBA_TAG != 0x3F)) {
							if (ENABLE_KEEP_LOADALIGN_BQ) {
								ubSeq = 1;
							}
							ulNCQ_INFO.B.ubNCQ_NEXT_WR = ulNCQ_INFO.B.ubCON_LBA_TAG;
						}
						else {
							ubSeq = 0;
						}
					}
					goto MARK_SET_TQ;
				}
			}
			//避免遇到error 無法離開
#if ENABLE_PFAIL
			else if (((gubNeedChkDoneTag == 0) && (DCCMSATAB[SATAB_RD_CNT] == 0)) || ((0 == gBQI.uwWrite_buffer_tq) && (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK) && (ubSetBQTag != 0xFF)))
#else
			else if ((gubNeedChkDoneTag == 0) && (DCCMSATAB[SATAB_RD_CNT] == 0)) //DCCMSATAB[SATAB_RD_CNT] == 0 表示DoneTag為0 --> 尚未收滿data, 可以Discard掉
#endif
			{
				UBYTE ubclearCnt;
#if ENABLE_PFAIL
				//_ASSERT(0x9, ((gubNeedChkDoneTag == 1) && (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK) && (gBQI.uwWrite_buffer_tq > 0)));
				if ((gubNeedChkDoneTag == 0) && (DCCMSATAB[SATAB_RD_CNT] == 0)) {
					_DEBUG_TRACE((0xAA << 24) | (0x0A));
					gBQI.uwWrite_buffer_tq = 0;
				}
				else if ((0 == gBQI.uwWrite_buffer_tq) && (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK)) {

					//_DEBUG_TRACE((0xB0<<24) | ((WR_NCQ_CMD_INFO[(HB[HB_NCQ_SET] & H_CUR_TAG)].uwEC_BQDone)<<8) | (HB[HB_NCQ_SET] & H_CUR_TAG));
#if (!DEBUG_ULTRAMLC_POWER_FAIL_BG_COPY && DEBUG_ULTRAMLC_POWER_FAIL)
					_DEBUG_TRACE((0xB0 << 24) | ((WR_NCQ_CMD_INFO[ubSetBQTag].uwEC_BQDone) << 8) | ubSetBQTag);
#endif
					//_ASSERT(0x40, (ubSetBQTag==0xFF));
					//UartString("\nAbort!");
					HW[HW_DATA_TRG] |= (H_CMD_ABRTREQ);
					HW[HW_ERR_CTRL] |= (H_REG_XHOLD /*| H_BLK_TRG*/);
					APU_Reset();
					gubErrorCode = CMD_ABORT;
					gubAbort_CMD = 1;

					if (gubNCQDoing) {
						gubNCQINTRError = 1;
						gubErrorTag = ubSetBQTag; //現在做到的
						U32 ulLBA = (NLL[gubErrorTag] + HL[HL_BUF_TRG_SEC] - HL[HL_CMD_REMAIN_SEC]);

						memset(NCQ_READ_LOG_EXT_ERR, 0x0, 16);
						NCQ_READ_LOG_EXT_ERR[0]  = gubErrorTag;
						NCQ_READ_LOG_EXT_ERR[2]  = 0x41;
						NCQ_READ_LOG_EXT_ERR[3]  = (HB[HB_ERROR] | CMD_ABORT);
						NCQ_READ_LOG_EXT_ERR[4]  = (UBYTE)ulLBA;
						NCQ_READ_LOG_EXT_ERR[5]  = (UBYTE)(ulLBA >> 8);
						NCQ_READ_LOG_EXT_ERR[6]  = (UBYTE)(ulLBA >> 16);
						NCQ_READ_LOG_EXT_ERR[7]  = (((UBYTE)((NSL[gubErrorTag] >> 16)&BIT0)) << 7); //H_NCQN_FUA
						NCQ_READ_LOG_EXT_ERR[8]  = (UBYTE)(ulLBA >> 24);
						//NCQ_READ_LOG_EXT_ERR[12] = (UBYTE)NCQ_TAG_INFO_LONG[gubErrorTag].All;
						//NCQ_READ_LOG_EXT_ERR[13] = (UBYTE)(NCQ_TAG_INFO_LONG[gubErrorTag].All>>8);
						NCQ_READ_LOG_EXT_ERR[12] = (UBYTE)HL[HL_CMD_REMAIN_SEC];
						NCQ_READ_LOG_EXT_ERR[13] = (UBYTE)(HL[HL_BUF_TRG_SEC] >> 8);
					}
					/*
					HW[HW_ERR_CTRL] &= (~H_BLK_TRG);
					HL[HL_SACTIVE] = 0;
					HB[HB_ERROR] = CMD_ABORT;
					HB[HB_STATUS] = (0x41);    //used status 0x40 for NCQ
					HB[HB_SDB_REQ] = H_SDB_REQ | H_SDB_IBIT;
					while (HB[HB_SDB_REQ] & H_SDB_REQ);
					*/
				}
#endif
				for (ubclearCnt = 0; ubclearCnt < 32; ubclearCnt++) {
					WR_NCQ_CMD_INFO[ubclearCnt].uwEC_BQDone = WR_NCQ_CMD_INFO[ubclearCnt].uwEC;
				}
				gulNCQWRCmdTriggerCnt = 0;
				gulNCQCMDDebug  = 0;
				gubMultiTrigTag = 0xFF;
				gubLargeSizeCmdCount = 0;

				/*清掉Error或之前的DoneTag
				if (DCCMSATAB[SATAB_RD_CNT]) {
					DCCMSATAB[SATAB_RPIU] = DCCMSATAB[SATAB_RD_CNT];
				}
				*/

				//因為發生fail後會下APU_Reset, 所以要清掉對應的LastdoneTag, 避免塞BQ時拿錯
				gubLastDoneTag = 0x3F;
				gulSATA_MSG_TYPE3[0].All = 0;

#if ENABLE_PFAIL
				HB[HB_FLAG_CTRL] = ubDDT_temp;
				HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
#endif
				if (gubWriteProtectWriteDummy && gubIsWriteProtect) {
					M_SetFlagSettingSHF();
				}
				return;
			}
#if ENABLE_PFAIL
			else if (gubNCQDoing && (gubMultiTrigTag == 0xFF) && gubWriteCacheEnable && (ubStopWrite == 0) && (gubWriteProtectWriteDummy || (!gubIsWriteProtect)) && !(gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK))
#else
			else if (gubNCQDoing && (gubMultiTrigTag == 0xFF) && gubWriteCacheEnable && (ubStopWrite == 0) && (gubWriteProtectWriteDummy || (!gubIsWriteProtect)))
#endif

			{
				if (ulNCQ_INFO.B.ubNCQ_NEXT_WR != 0x3F) {
					HL[HL_CON_LBA_CHK_LBA] = WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulLBA + WR_NCQ_CMD_INFO[ubNCQNextWRTag].ulSectorCnt;
					while (HL[HL_WR_NCQ_INFO] & H_CON_LBA_CHK_TRG);

					ulNCQ_INFO.All = HL[HL_WR_NCQ_INFO];
					if ((gubLBAOverlap == 0) && (ulNCQ_INFO.B.ubCON_LBA_TAG != 0x3F)) {
						if (ENABLE_KEEP_LOADALIGN_BQ) {
							ubSeq = 1;
						}
						ulNCQ_INFO.B.ubNCQ_NEXT_WR = ulNCQ_INFO.B.ubCON_LBA_TAG;
					}
					else {
						ubSeq = 0;
					}
				}
				goto MARK_SET_TQ;
			}
			//			}
		}

	MARK_SET_BQ:
		if (ubSetBQTag != 0xFF) {
			if (gubSDRFlushDoing) {
				if (ubDDRMode && (gubPreviousWriteCmdMode == 0)) {
					guwSDRFlushEnd4kIndex = (guwSDRFlushStart4kIndex + guwFlush4kCount) & SDRAM_CACHE_4KNUM_MASK;
					gubSDRFlushDoing = 0;
				}
				else if ((ubDDRMode == 0) && (gubPreviousWriteCmdMode == 1)) {
					guwSDRTotalFlush4kNum = guwDataInSDR4kNum + guwFlush4kCount;
				}
			}
			UBYTE ubCheckSATALink = 0;
			if (gubNCQWRCmdTriggerPassCnt) {
				gubNCQWRCmdTriggerPassCnt--;
			}
			else {
				if (gubForceStop) { //only link lost
					idle(200);//有機會發生CRC時delay一段時間讓HW計算完再判斷
					if ((gubForceStop == BIT0) && (HW[HW_SATA_INFO] & (H_LNK_ESTABLISH))) {
						gubForceStop = 0;
					}
					else if ((HW[HW_SATA_INFO] & (H_LNK_ESTABLISH))) {
						ubCheckSATALink = 1;
						gubForceStop = 0;
					}
					else {
						ubCheckSATALink = 1;
					}
				}
			}
			if (ubCheckSATALink || (gubIsWriteProtect && gubWriteProtectWriteDummy)) {
				ulSectorCnt = 0;
			}
			else {
				while (1) {
					BufferQueue_t *BQ;
					UBYTE ubNeedLoadAlign = 0;
					UBYTE ubFlushKeepBQ = 0;
#if ENABLE_PFAIL
					/* 1EC/1BQ, to avoid "1,1,3" case:ftl will dummy, can't guarantee 4EC/plane. */
					if (ulLBA & SectorsPer4KMask) {
						if (ulSectorCnt < (SectorsPer4K - (ulLBA & SectorsPer4KMask))) {
							ulTriggerSectorCnt = ulSectorCnt;
						}
						else {
							ulTriggerSectorCnt = (SectorsPer4K - (ulLBA & SectorsPer4KMask));
						}
						ubNeedLoadAlign = 1;
					}
					else {
						if (ulSectorCnt < SectorsPer4K) {
							ulTriggerSectorCnt = ulSectorCnt;
							ubNeedLoadAlign = 1;
						}
						else {
							ulTriggerSectorCnt = SectorsPer4K;
						}
					}
					ub4kNum = (ulTriggerSectorCnt + SectorsPer4KMask) >> SectorsPer4KLog;
					_ASSERT(0x8, ub4kNum > 1);
#else
					if (ulLBA & SectorsPer4KMask) {   // not alignment
						ulTriggerSectorCnt = gubSectorsPerPlane - (ulLBA & gubSectorsPerPlaneMask);
						if (ulTriggerSectorCnt > ulSectorCnt) {
							ulTriggerSectorCnt = ulSectorCnt;
							ub4kNum = ((ulLBA + ulSectorCnt) >> SectorsPer4KLog) - (ulLBA >> SectorsPer4KLog);
							if ((ulLBA + ulSectorCnt)&SectorsPer4KMask) {
								ub4kNum++;
							}
						}
						else {
							ub4kNum = gub4kEntrysPerPlane - ((ulLBA & gubSectorsPerPlaneMask) >> SectorsPer4KLog);
						}
						ubNeedLoadAlign = 1;
					}
					else {  // alignment
						if (ulSectorCnt < (gubSectorsPerPlane - (ulLBA & gubSectorsPerPlaneMask))) {
							ulTriggerSectorCnt = ulSectorCnt;
							ub4kNum = ulTriggerSectorCnt >> SectorsPer4KLog;
							if (ulTriggerSectorCnt & SectorsPer4KMask) {
								ub4kNum++;
								ubNeedLoadAlign = 1;
							}
						}
						else {
							ulTriggerSectorCnt = gubSectorsPerPlane - (ulLBA & gubSectorsPerPlaneMask);
							ub4kNum = ulTriggerSectorCnt >> SectorsPer4KLog;
						}
					}
#endif
					if ((ulLBA + ulTriggerSectorCnt)&SectorsPer4KMask) {
						ulDataMask = gulMaskTable[ulLBA & SectorsPer4KMask] & (~gulMaskTable[(ulTriggerSectorCnt + (ulLBA & SectorsPer4KMask))&gubSectorsPerPlaneMask]);
					}
					else {
						ulDataMask = gulMaskTable[ulLBA & SectorsPer4KMask];
					}


					if (ubDDRMode || (guwDataInSDR4kNum > 0)) {
						if (((guwDataInSDR4kNum + ub4kNum) >= (SDRAM_CACHE_4KNUM - 2))  || ((ubDDRMode == 0) && (gubSDRFlushDoing == 0))) { //flush table
							goto Mark_Flush_SDR;
						}
						UBYTE ub4kIndex;
						for (ub4kIndex = 0; ub4kIndex < ub4kNum; ub4kIndex++) {
							UWORD uwTableIndex = (uwBufferPTR / 8 + ub4kIndex) & SDRAM_CACHE_4KNUM_MASK;
							U32 ulVir4kIndex;
							UWORD uwL1Entry;
							UWORD uwL2Entry;
							UWORD uwL2BlockIndex;
							if (ENABLE_READ_CACHE) {
								ulVir4kIndex =  SDR_DataEntryTable[uwTableIndex].ulVir4kIndex;
								uwL1Entry = ulVir4kIndex >> gubDataCacheL2PL1TablePerEntry4kNumLog;
								uwL2Entry = ulVir4kIndex & gulDataCacheL2PTableEntryL2Mask;
								uwL2BlockIndex = guwDataCacheL2PTable_L1[uwL1Entry];
								if (guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry] == uwTableIndex) {
									guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry] = 0xFFFF;
									gubSDRL2PEntryCount[uwL2BlockIndex]--;
									if (gubSDRL2PEntryCount[uwL2BlockIndex] == 0) {
										guwFreeSDRL2PTableLink[guwSDRFreeLinkTail] = uwL2BlockIndex;
										guwSDRFreeLinkTail = uwL2BlockIndex;
										guwSDRFreeTableBlockNum++;
										guwDataCacheL2PTable_L1[uwL1Entry] = 0xFFFF;
									}
								}
							}
							ulVir4kIndex =  (ulLBA / 8) + ub4kIndex;
							uwL1Entry = ulVir4kIndex >> gubDataCacheL2PL1TablePerEntry4kNumLog;
							uwL2Entry = ulVir4kIndex & gulDataCacheL2PTableEntryL2Mask;
							uwL2BlockIndex = guwDataCacheL2PTable_L1[uwL1Entry];
							if ( gubZINFO[uwTableIndex & 63].B.btSpecialData ) {
								SDR_DataEntryTable[uwTableIndex].btSpecialData = 1;
							}
							else {
								SDR_DataEntryTable[uwTableIndex].btSpecialData = 0;
							}
							SDR_DataEntryTable[uwTableIndex].ubMask = 0;
							if (uwL2BlockIndex != 0xFFFF) {
								if (guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry] != 0xFFFF) {
									if (((~ulDataMask) >> (ub4kIndex * 8)) & 0xFF) {
										mDMAC_COPY((U32)&gubSDRCache[guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry] * 4 * 1024], (U32)&gubSDRCache[uwTableIndex * 4 * 1024], 4096, (ulVir4kIndex << SectorsPer4KLog), (0xFFFFFF00 | (((~ulDataMask) >> (ub4kIndex * 8)) & 0xFF)), 1, 0, 1);
									}
									SDR_DataEntryTable[uwTableIndex].ubMask = SDR_DataEntryTable[guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry]].ubMask;
									SDR_DataEntryTable[uwTableIndex].btSpecialData = 0;
								}
								else {
									gubSDRL2PEntryCount[uwL2BlockIndex]++;
								}
							}
							else {
								M_AssertError(guwSDRFreeTableBlockNum < 1);
								guwDataCacheL2PTable_L1[uwL1Entry] = uwL2BlockIndex = guwSDRFreeLinkHead;
								guwSDRFreeLinkHead = guwFreeSDRL2PTableLink[guwSDRFreeLinkHead];
								guwSDRFreeTableBlockNum --;
								gubSDRL2PEntryCount[uwL2BlockIndex]++;
							}
							guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry] = uwTableIndex;
							UBYTE ubZcode =  gubZINFO[uwTableIndex & 63].B.ubZCode;
							if (ubZcode != MAX_ZCODE) {
								if (gubLZSS_RESOLUTION == 1) { // 1K
									ubZcode = (ubZcode * 2) + 1;
								}
								else if (gubLZSS_RESOLUTION == 2) { // 2K
									ubZcode = 3;
								}
							}
							SDR_DataEntryTable[uwTableIndex].ubMask |= (ulDataMask >> (ub4kIndex * 8)) & 0xFF;
							SDR_DataEntryTable[uwTableIndex].ubZCode = ubZcode;
							SDR_DataEntryTable[uwTableIndex].ulVir4kIndex = ulVir4kIndex;
							SDR_DataEntryTable[uwTableIndex].btDataValid = 1;
						}
						uwBufferPTR = ((uwBufferPTR + ulTriggerSectorCnt) & SDRAM_CACHE_MASK);
						guwDataInSDR4kNum += ub4kNum;
						if (ubDDRMode == 0) {
							guwSDRTotalFlush4kNum += ub4kNum;
						}
					}
					else {
						UBYTE ub4kIndex;
						UBYTE ubTotalSectors = 0;
						// add to BQ
						if (ubNeedLoadAlign) {
#if ENABLE_PFAIL
							while ((gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum) >= (BQ_NUM - 2)) {
								_DEBUG_TRACE((0xFE << 24) | (0xD << 16) | gBQI.uwWrite_tq_2_bq);
								M_SwitchTask();
								ftl_cal_write_buffer_window();
							}
#else
							while ((gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum) >= (BQ_NUM - 2))M_SwitchTask();
#endif
							gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
							BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
							BQ->btWrite = 1;
							BQ->ulVir4kIndex = ulLBA / 8;
							BQ->ubDataInDRamMap = 0;
							if ((ulLBA + ulTriggerSectorCnt)&SectorsPer4KMask) {
								BQ->ulDataMask = ~(gulMaskTable[ulLBA & SectorsPer4KMask] & (~gulMaskTable[(ulTriggerSectorCnt + (ulLBA & SectorsPer4KMask))&gubSectorsPerPlaneMask]));
							}
							else {
								BQ->ulDataMask = ~gulMaskTable[ulLBA & SectorsPer4KMask];
							}
							if (uwBufferPTR & SectorsPer4KMask) {
								BQ->uwBufferIndex = uwBufferPTR & (~SectorsPer4KMask);
							}
							else {
								BQ->uwBufferIndex = uwBufferPTR;
							}
							BQ->ubRBQIIndex = 0xFF;
							BQ->btPartialFromFlash = 0;
							BQ->btNeedLoadAlign = 1;
#if ENABLE_PFAIL
							BQ->ubNeedLoadAlignCopyFromWB = 0;
#endif
							BQ->btSpecialData = 0;
							BQ->btNeedCheckSpecialData = 0;
							BQ->btFlushWBQ = 0;
							BQ->ub4kNum = ub4kNum;
							BQ->ulSpareValid = 0;
							BQ->btCopyFromWB = 0;
							M_AddToBufferQueue(gBQI.ubLinkActive);
						}
#if ENABLE_PFAIL
						while ((gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum) >= BQ_NUM) {
							_DEBUG_TRACE((0xFE << 24) | (0xE << 16) | gBQI.uwWrite_tq_2_bq);
							M_SwitchTask();
							ftl_cal_write_buffer_window();
						}
#else
						while ((gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum) >= BQ_NUM)M_SwitchTask();
#endif
						gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
						BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
						BQ->btWrite = 1;
						BQ->ulVir4kIndex = ulLBA / 8;
						BQ->ubDataInDRamMap = 0;
						if ((ulLBA + ulTriggerSectorCnt)&SectorsPer4KMask) {
							BQ->ulDataMask = gulMaskTable[ulLBA & SectorsPer4KMask] & (~gulMaskTable[(ulTriggerSectorCnt + (ulLBA & SectorsPer4KMask))&gubSectorsPerPlaneMask]);
						}
						else {
							BQ->ulDataMask = gulMaskTable[ulLBA & SectorsPer4KMask];
						}
						if (uwBufferPTR & SectorsPer4KMask) {
							BQ->uwBufferIndex = uwBufferPTR & (~SectorsPer4KMask);
						}
						else {
							BQ->uwBufferIndex = uwBufferPTR;
						}

						UWORD uwBufferPTRTemp = uwBufferPTR; //It must be initialized here for gubEnable_Special_Data_Workaround mode 2
						UBYTE ubSpecialData = gubEnableSpecialData ? 1 : 0;
						for (ub4kIndex = 0; ub4kIndex < ub4kNum; ub4kIndex++) {
							UBYTE ubZcode =  gubZINFO[(((uwBufferPTR >> SectorsPer4KLog) + ub4kIndex) & BUFFER2_WRITE_4KNUM_MASK) + BUFFER2_WRITE_4K_START_INDEX].B.ubZCode;
							if (ubZcode != MAX_ZCODE) {
								if (gubLZSS_RESOLUTION == 0) {
									if (ubZcode == 6) {
										ubZcode = 7;
									}
								}
								else if (gubLZSS_RESOLUTION == 1) { // 1K
									ubZcode = (ubZcode * 2) + 1;
								}
								else if (gubLZSS_RESOLUTION == 2) { // 2K
									ubZcode = 3;
								}
								else if (gubLZSS_RESOLUTION == 3) { // 4K
									ubZcode = 7;
								}
							}
#if ENABLE_DEBUG_WAF_KAO
							gulZipCount[ubZcode]++;
#endif
							BQ->ubZcode[ub4kIndex] = ubZcode;
							ubTotalSectors += BQ->ubZcode[ub4kIndex];
							if (ubSpecialData && (gubZINFO[(((uwBufferPTR >> SectorsPer4KLog) + ub4kIndex) & BUFFER2_WRITE_4KNUM_MASK) + BUFFER2_WRITE_4K_START_INDEX].B.btSpecialData == 0)) {
								ubSpecialData = 0;
							}
						}

						if ((gubEnable_Special_Data_Workaround == 1) && ((ulLBA & SectorsPer4KMask) || ((ulLBA + ulTriggerSectorCnt) & SectorsPer4KMask)) ) {
							ubSpecialData = 0;
						}

						BQ->btSpecialData = ubSpecialData;
						BQ->btNeedCheckSpecialData = ubSpecialData;
						BQ->ulSectorCnt = ubTotalSectors;
						BQ->btNeedLoadAlign = 0;
#if ENABLE_PFAIL
						BQ->ubNeedLoadAlignCopyFromWB = 0;
#endif
						BQ->btFlushWBQ = 0;
						BQ->ub4kNum = ub4kNum;
						BQ->ulSpareValid = 0;
						BQ->btCopyFromWB = 0;
						M_AddToBufferQueue(gBQI.ubLinkActive);
						gBQI.ubWLinkNum++;
						uwBufferPTR = ((uwBufferPTR + ulTriggerSectorCnt) & BUFFER2_WRITE_MASK) + BUFFER2_WRITE_START_INDEX;
						ub4kTransferDone += ub4kNum;
#if ENABLE_PFAIL
						gBQI.uwWrite_tq_2_bq += ub4kNum;
						gBQI.uwWrite_buffer_tq -= ub4kNum;
						_ASSERT(7, ((!gBQI.ubWrite_TQ_lock) && ((gBQI.uwWrite_tq_2_bq + gBQI.uwWrite_buffer_tq) > gBQI.uwWrite_buffer_window)));
#endif

						UBYTE ubOld4kHit = 0;
						U32 ulVir4kIndex =  ulLBA / 8;
						UBYTE ub4kinBQIndex;

						if (ENABLE_KEEP_WBQ) {
							//Search WriteBuffer Table
							for (ub4kinBQIndex = 0; ub4kinBQIndex < ub4kNum; ub4kinBQIndex++) {
								UBYTE ubWBufferTableIndex = (ulVir4kIndex + ub4kinBQIndex) & BUFFER2_WRITE_4KNUM_MASK;
								UBYTE ubWriteBufferPTR = gubWriteBufferTableHead[ubWBufferTableIndex];
								while (Default_WB_Index != ubWriteBufferPTR) {
									if ((ulVir4kIndex + ub4kinBQIndex) == gulWriteBufferVir4KTable[ubWriteBufferPTR]) {
										ubOld4kHit |= (BIT0 << ub4kinBQIndex);
										break;
									}
									ubWriteBufferPTR = gubWriteBufferTableNext[ubWriteBufferPTR];
								}
								/*if (ubOld4kHit) {
								break;
								}*/
							}
						}

						if (gubKeepLoadAlignmentBQNum) {
							gBQI.ubLinkNum += 2;
							gBQI.ubWLinkNum++;
							UBYTE ubLoadAlignMentBQLink1 = gubKeepLoadAlignmentBQLink;
							UBYTE ubLoadAlignMentBQLink2 = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubPrevious;
							UBYTE ubWriteBQLink1 = gBQI.BufferQueue[gubKeepLoadAlignmentBQLink].ubNext;
							UBYTE ubWriteBQLink2 = gBQI.ubLinkLastUse;
							UBYTE ubClearCache = 0;
							if ((ulLBA == gulKeepLoadAlignmentBQLBA) && (gulKeepLoadAlignmentBQBufferIndex == gBQI.BufferQueue[ubWriteBQLink2].uwBufferIndex)) {
								if (0) {
									UartString("\n");
									Uart_Tx_DataHex(ubWriteBQLink1);
									UartString(" ");
									Uart_Tx_DataHex(ubWriteBQLink2);
									UartString(" ");
									UartLongHex(gBQI.BufferQueue[ubWriteBQLink1].ulVir4kIndex);
									UartString(" ");
									UartLongHex(gBQI.BufferQueue[ubWriteBQLink2].ulVir4kIndex);
									UartString(" ");
									UartLongHex(gBQI.BufferQueue[ubWriteBQLink1].ulDataMask);
									UartString(" ");
									UartLongHex(gBQI.BufferQueue[ubWriteBQLink2].ulDataMask);
									UartString(" ");
									Uart_Tx_DataHex(gBQI.BufferQueue[ubWriteBQLink1].ub4kNum);
									UartString(" ");
									Uart_Tx_DataHex(gBQI.BufferQueue[ubWriteBQLink2].ub4kNum);
								}

								if ((gubEnable_Special_Data_Workaround == 0xF7)
								        && ubSpecialData && (gBQI.BufferQueue[ubWriteBQLink1].btSpecialData) && (uwBufferPTRTemp & SectorsPer4KMask)) {
									UWORD uwBufferIndexTemp = gBQI.BufferQueue[ubWriteBQLink2].uwBufferIndex;
									while (uwBufferIndexTemp != uwBufferPTRTemp) {
										if (mDMAC_CMP((U32)(&BUF2_BYTE_BASE[uwBufferIndexTemp * 512]), (U32)(&BUF2_BYTE_BASE[uwBufferPTRTemp * 512]), 512)) {
											gBQI.BufferQueue[ubWriteBQLink1].btSpecialData = 0;
											gBQI.BufferQueue[ubWriteBQLink1].btNeedCheckSpecialData = 0;
											gBQI.BufferQueue[ubWriteBQLink2].btSpecialData = 0;
											gBQI.BufferQueue[ubWriteBQLink2].btNeedCheckSpecialData = 0;
											ubSpecialData = 0;
											break;
										}
										uwBufferIndexTemp++;
									}
								}

								gBQI.BufferQueue[ubWriteBQLink1].ulDataMask |= (gBQI.BufferQueue[ubWriteBQLink2].ulDataMask << ((gBQI.BufferQueue[ubWriteBQLink2].ulVir4kIndex - gBQI.BufferQueue[ubWriteBQLink1].ulVir4kIndex) * 8));
								gBQI.BufferQueue[ubWriteBQLink1].btSpecialData &= ubSpecialData;
								gBQI.BufferQueue[ubWriteBQLink1].btNeedCheckSpecialData &= ubSpecialData;
								UBYTE ub4kInBQIndex;
								for (ub4kInBQIndex = 1; ub4kInBQIndex < ub4kNum; ub4kInBQIndex++) {
									gBQI.BufferQueue[ubWriteBQLink1].ubZcode[gBQI.BufferQueue[ubWriteBQLink1].ub4kNum + ub4kInBQIndex - 1] = gBQI.BufferQueue[ubWriteBQLink2].ubZcode[ub4kInBQIndex];
									gBQI.BufferQueue[ubWriteBQLink1].ulSectorCnt += gBQI.BufferQueue[ubWriteBQLink2].ubZcode[ub4kInBQIndex];
								}
								gBQI.BufferQueue[ubWriteBQLink1].ub4kNum = (gBQI.BufferQueue[ubWriteBQLink2].ulVir4kIndex - gBQI.BufferQueue[ubWriteBQLink1].ulVir4kIndex) + ub4kNum;
								M_RemoveWriteBufferQueue(ubWriteBQLink2);
#if ENABLE_PFAIL
								gBQI.uwWrite_tq_2_bq -= ub4kNum;
#endif
								gBQI.ubWLinkNum--;
								if (0) {
									UartString(" => ");
									UartLongHex(gBQI.BufferQueue[ubWriteBQLink1].ulVir4kIndex);
									UartString(" ");
									UartLongHex(gBQI.BufferQueue[ubWriteBQLink1].ulDataMask);
									UartString(" ");
									Uart_Tx_DataHex(gBQI.BufferQueue[ubWriteBQLink1].ub4kNum);
								}
								if (ubNeedLoadAlign) {
									gBQI.BufferQueue[ubLoadAlignMentBQLink1].ulDataMask = ~gBQI.BufferQueue[ubWriteBQLink1].ulDataMask;
									gBQI.BufferQueue[ubLoadAlignMentBQLink1].ub4kNum = (gBQI.BufferQueue[ubLoadAlignMentBQLink2].ulVir4kIndex - gBQI.BufferQueue[ubLoadAlignMentBQLink1].ulVir4kIndex) + ub4kNum;
									M_RemoveWriteBufferQueue(ubLoadAlignMentBQLink2);
									U32 ulCheckDataMask = 0xFFFFFFFF >> ((gub4kEntrysPerPlane - gBQI.BufferQueue[ubLoadAlignMentBQLink1].ub4kNum) * 8);
									if ((gBQI.BufferQueue[ubLoadAlignMentBQLink1].ulDataMask & ulCheckDataMask) == 0) {
										M_RemoveWriteBufferQueue(ubLoadAlignMentBQLink1);
										ubNeedLoadAlign = 0;
										ubClearCache = 1;
									}
								}
								ubOld4kHit &= (~BIT0);
							}
							else {
								if (ENABLE_KEEP_WBQ) {
									if (ENABLE_KEEP_WBQ_WITHOUT_LOADALIGN_COLLECT) {
										gBQI.ubLinkNum += gubKeepWBQNum;
										gBQI.ubWLinkNum += gubKeepWBQNum;
										gubKeepWBQNum = 0;
										gubKeepWBQSectorCnt = 0;
										gubKeepWBQ4KCnt = 0;
										gulWriteBufferBitMap = 0;
										UBYTE ubi;
										for (ubi = 0; ubi < 32; ubi++) {
											gubWriteBufferTableHead[ubi] = Default_WB_Index;
											gubWriteBufferTableNext[ubi] = Default_WB_Index;
										}
										if (gubKeepLoadAlignmentBQNum) {
											gubFirstKeepBQLink = gubKeepLoadAlignmentBQLink;
											gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
										}
										else {
											gBQI.ubLinkFTLLast = gBQI.ubLinkLastUse;
										}
									}
									else if ((ubOld4kHit == 0) && gubKeepWBQNum) {
										//if (ubOld4kHit == 0) {
										//[cppcheck][Identical inner 'if' condition is always true.]
										{
											MoveLink(gubKeepLoadAlignmentBQLink);
											gubKeepWBQNum ++;
											gubKeepWBQSectorCnt += ( gBQI.BufferQueue[gBQI.ubLinkLastUse].ulSectorCnt + gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum);
											gubKeepWBQ4KCnt += gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum;
											gBQI.ubLinkNum--;
											gBQI.ubWLinkNum--;
										}
									}
									else {
										ubClearCache = 1;
									}
								}
							}
							gubKeepLoadAlignmentBQLink = 0xFF;
							gubKeepLoadAlignmentBQNum = 0;
							if (ENABLE_KEEP_WBQ && ubClearCache) {
								UBYTE ub4kNumIndex;
								U32 ulV4kIndex = gBQI.BufferQueue[ubWriteBQLink1].ulVir4kIndex;
								UBYTE ubWriteBufferPTRIndex = ((gBQI.BufferQueue[ubWriteBQLink1].uwBufferIndex / SectorsPer4K) - BUFFER2_WRITE_4K_START_INDEX) & BUFFER2_WRITE_4KNUM_MASK;
								for (ub4kNumIndex = 0; ub4kNumIndex < gBQI.BufferQueue[ubWriteBQLink1].ub4kNum; ub4kNumIndex++) {
									UBYTE ubCurrentBufferIndex;
									UBYTE ubPreWBBufferIndex;
									if (gulWriteBufferBitMap & (BIT0 << ubWriteBufferPTRIndex)) {
										//UBYTE ubPre4kIndex = gulWriteBufferVir4KTable[ubWriteBufferPTRIndex];
										UBYTE ubIndex = ulV4kIndex & 31;
										M_AssertError(Default_WB_Index == gubWriteBufferTableHead[ubIndex]);
										ubCurrentBufferIndex = gubWriteBufferTableHead[ubIndex];
										ubPreWBBufferIndex = Default_WB_Index;
										while ( Default_WB_Index != ubCurrentBufferIndex ) {
											if (ubWriteBufferPTRIndex == ubCurrentBufferIndex) {
												if (Default_WB_Index == ubPreWBBufferIndex) {
													gubWriteBufferTableHead[ubIndex] = gubWriteBufferTableNext[ubCurrentBufferIndex];
												}
												else {
													gubWriteBufferTableNext[ubPreWBBufferIndex] = gubWriteBufferTableNext[ubCurrentBufferIndex];
												}
												gubWriteBufferTableNext[ubCurrentBufferIndex] = Default_WB_Index;
												gulWriteBufferBitMap &= (~((U32)BIT0 << ubWriteBufferPTRIndex));
												break;
											}
											ubPreWBBufferIndex = ubCurrentBufferIndex;
											ubCurrentBufferIndex = gubWriteBufferTableNext[ubCurrentBufferIndex];
										}
									}
									ubWriteBufferPTRIndex = (ubWriteBufferPTRIndex + 1) & BUFFER2_WRITE_4KNUM_MASK;
									ulV4kIndex++;
								}
							}
						}
						if (ENABLE_KEEP_WBQ) {
							UBYTE ubPreBQ =  gBQI.BufferQueue[gBQI.ubLinkLastUse].ubPrevious;
							UBYTE ubNeedFlush = 0;
							if (ENABLE_KEEP_WBQ_WITHOUT_LOADALIGN_COLLECT && gBQI.BufferQueue[ubPreBQ].btWrite && gBQI.BufferQueue[ubPreBQ].btNeedLoadAlign) {
								ubNeedFlush = 1;
							}
							if ((gubKeepWBQSectorCnt >= gubSectorsPerPlane) || (gubKeepWBQ4KCnt >= 11) || ubOld4kHit || ubNeedFlush) {
								gBQI.ubLinkNum += gubKeepWBQNum;
								gBQI.ubWLinkNum += gubKeepWBQNum;
								gubKeepWBQNum = 0;
								gubKeepWBQSectorCnt = 0;
								gubKeepWBQ4KCnt = 0;
								gulWriteBufferBitMap = 0;
								ubFlushKeepBQ = 1;
								UBYTE ubi;
								for (ubi = 0; ubi < 32; ubi++) {
									gubWriteBufferTableHead[ubi] = Default_WB_Index;
									gubWriteBufferTableNext[ubi] = Default_WB_Index;
								}
								if (gubKeepLoadAlignmentBQNum) {
									gubFirstKeepBQLink = gubKeepLoadAlignmentBQLink;
									gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
								}
								else {
									gBQI.ubLinkFTLLast = gBQI.ubLinkLastUse;
								}

								if (ENABLE_DEBUG_UART_YH) {
									UBYTE ubLink =  gBQI.ubLinkFirst;
									UartString("\n2 BQNum:");
									Uart_Tx_DataHex(gBQI.ubLinkNum);
									UartString(" KeepW:");
									Uart_Tx_DataHex(gubKeepWBQNum);
									UartString(" KeepL:");
									Uart_Tx_DataHex(gubKeepLoadAlignmentBQNum);
									UartString(" LastUse:");
									Uart_Tx_DataHex(gBQI.ubLinkLastUse);
									UartString(" FTLLast:");
									Uart_Tx_DataHex(gBQI.ubLinkFTLLast);
									UartString("  ");
									for (ubi = 0; ubi < (gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum); ubi++) {
										if ((ubi == (gBQI.ubLinkNum - 1)) && (ubLink != gBQI.ubLinkFTLLast)) {
											flaLEDBlink(0);
										}
										UartString(" ");
										Uart_Tx_DataHex(ubLink);
										ubLink =  gBQI.BufferQueue[ubLink].ubNext;
									}
									if (gBQI.BufferQueue[ubLink].ubPrevious != gBQI.ubLinkLastUse) {
										flaLEDBlink(0);
									}
								}
							}

							//=============== Tony ====================
							if (gubKeepWBQNum || (!((ubNeedLoadAlign == 0) && ((gBQI.BufferQueue[gBQI.ubLinkLastUse].ulSectorCnt + gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum) ==  gubSectorsPerPlane)))) {
								UBYTE ub4kNumIndex;
								UBYTE ubCurrentBufferIndex;
								UBYTE ubPreWBBufferIndex;
								UBYTE ubWriteBufferPTRIndex = ((gBQI.BufferQueue[gBQI.ubLinkLastUse].uwBufferIndex / SectorsPer4K) - BUFFER2_WRITE_4K_START_INDEX) & BUFFER2_WRITE_4KNUM_MASK;
								U32   ulVir4KIndex = gBQI.BufferQueue[gBQI.ubLinkLastUse].ulVir4kIndex;


								for (ub4kNumIndex = 0; ub4kNumIndex < gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum; ub4kNumIndex++) {
									UBYTE ubPre4kIndex = gulWriteBufferVir4KTable[ubWriteBufferPTRIndex];
									gulWriteBufferVir4KTable[ubWriteBufferPTRIndex] = ulVir4KIndex;       // Vir4K = LBA / 8
									gubWriteBufferDataMaskTable[ubWriteBufferPTRIndex] = (gBQI.BufferQueue[gBQI.ubLinkLastUse].ulDataMask >> (ub4kNumIndex * 8)) & 0xFF;

									UBYTE ubWBufferTableIndex = gulWriteBufferVir4KTable[ubWriteBufferPTRIndex] & 31;         // Vir4K % 32
									if (ENABLE_DEBUG_UART_YT) {
										UartString("\nWBPTRIndex:");
										UartWordHex((UWORD) ubWriteBufferPTRIndex);
										UartString("\tVir4K:");
										UartLongHex((U32)gulWriteBufferVir4KTable[ubWriteBufferPTRIndex]);
										UartString("\tWBufferTableIndex:");
										UartWordHex((UWORD) ubWBufferTableIndex);
										UartString("\n");
									}
									if (gulWriteBufferBitMap & (BIT0 << ubWriteBufferPTRIndex)) {
										//UBYTE ubPre4kIndex = gulWriteBufferVir4KTable[ubWriteBufferPTRIndex];
										UBYTE ubIndex = ubPre4kIndex & 31;
										M_AssertError(Default_WB_Index == gubWriteBufferTableHead[ubIndex]);
										ubCurrentBufferIndex = gubWriteBufferTableHead[ubIndex];
										ubPreWBBufferIndex = Default_WB_Index;
										while ( Default_WB_Index != ubCurrentBufferIndex ) {
											if (ubWriteBufferPTRIndex == ubCurrentBufferIndex) {
												if (Default_WB_Index == ubPreWBBufferIndex) {
													gubWriteBufferTableHead[ubIndex] = gubWriteBufferTableNext[ubCurrentBufferIndex];
												}
												else {
													gubWriteBufferTableNext[ubPreWBBufferIndex] = gubWriteBufferTableNext[ubCurrentBufferIndex];
												}
												gubWriteBufferTableNext[ubCurrentBufferIndex] = Default_WB_Index;
												break;
											}
											ubPreWBBufferIndex = ubCurrentBufferIndex;
											ubCurrentBufferIndex = gubWriteBufferTableNext[ubCurrentBufferIndex];
										}
									}
									gulWriteBufferBitMap |= BIT0 << ubWriteBufferPTRIndex;
									gubWriteBufferTableNext[ubWriteBufferPTRIndex] = Default_WB_Index;


									if (Default_WB_Index == gubWriteBufferTableHead[ubWBufferTableIndex]) {
										gubWriteBufferTableHead[ubWBufferTableIndex] = ubWriteBufferPTRIndex;
									}
									else {
										UBYTE ubNotUpdate = 0;
										ubCurrentBufferIndex = gubWriteBufferTableHead[ubWBufferTableIndex];
										ubPreWBBufferIndex = Default_WB_Index;
										while (Default_WB_Index != ubCurrentBufferIndex) {
											if ( gulWriteBufferVir4KTable[ubWriteBufferPTRIndex] == gulWriteBufferVir4KTable[ubCurrentBufferIndex]) {
												if (ubWriteBufferPTRIndex == ubCurrentBufferIndex ) {
													ubNotUpdate = 1;
													break;
												}
												else {
													if (ubPreWBBufferIndex == Default_WB_Index) {
														gubWriteBufferTableHead[ubWBufferTableIndex] = gubWriteBufferTableNext[ubCurrentBufferIndex];
													}
													else {
														gubWriteBufferTableNext[ubPreWBBufferIndex] = gubWriteBufferTableNext[ubCurrentBufferIndex];
													}
													gubWriteBufferTableNext[ubCurrentBufferIndex] = Default_WB_Index;
													ubCurrentBufferIndex = ubPreWBBufferIndex;
													if ( Default_WB_Index == gubWriteBufferTableHead[ubWBufferTableIndex]) {
														break;
													}
												}
											}
											ubPreWBBufferIndex = ubCurrentBufferIndex;
											ubCurrentBufferIndex = gubWriteBufferTableNext[ubCurrentBufferIndex];

										}
										if (ubNotUpdate == 0) {
											if ( Default_WB_Index == gubWriteBufferTableHead[ubWBufferTableIndex]) {
												gubWriteBufferTableHead[ubWBufferTableIndex] = ubWriteBufferPTRIndex;
											}
											else {
												gubWriteBufferTableNext[ubPreWBBufferIndex] = ubWriteBufferPTRIndex;
											}
										}
									}

									ubWriteBufferPTRIndex = (ubWriteBufferPTRIndex + 1) & BUFFER2_WRITE_4KNUM_MASK;
									ulVir4KIndex++;

								}

								//Debug...
								//if (ENABLE_DEBUG_UART_YT) {
								if (0) {
									UBYTE ubi;
									UBYTE ubNext;
									for (ubi = 0; ubi < 32; ubi++) {
										if (gubWriteBufferTableHead[ubi] != Default_WB_Index) {
											UartString("\n index:");
											UartWordHex((UWORD)ubi);
											UartString("\t\t");
											UartLongHex((U32)gulWriteBufferVir4KTable[gubWriteBufferTableHead[ubi]]);
											ubNext = gubWriteBufferTableNext[gubWriteBufferTableHead[ubi]];
											while ( Default_WB_Index != ubNext ) {
												UartString(" => ");
												UartLongHex((U32)gulWriteBufferVir4KTable[ubNext]);
												ubNext = gubWriteBufferTableNext[ubNext];
											}
											UartString("\n");
										}
									}
								}
							}
							//=============================================================================================================================
						}
					}
					ulSectorCnt -= ulTriggerSectorCnt;
					if ((ulSectorCnt == 0) && (gubSDRFlushDoing == 0)) {
#if ENABLE_PFAIL
						if (ENABLE_KEEP_LOADALIGN_BQ && ubNeedLoadAlign && ubSeq && ((ulTriggerSectorCnt + ulLBA)&SectorsPer4KMask))
#else
						if (ENABLE_KEEP_LOADALIGN_BQ && ubNeedLoadAlign && ((ulTriggerSectorCnt + ulLBA)&SectorsPer4KMask))
#endif
						{
							M_AssertError(gubKeepLoadAlignmentBQNum);
							gBQI.ubLinkNum -= 2;
							gBQI.ubWLinkNum--;
							gubKeepLoadAlignmentBQLink =  gBQI.BufferQueue[gBQI.ubLinkLastUse].ubPrevious;
							gulKeepLoadAlignmentBQLBA = ulLBA + ulTriggerSectorCnt;
							gulKeepLoadAlignmentBQBufferIndex =  ((gBQI.BufferQueue[gBQI.ubLinkLastUse].uwBufferIndex + ((gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum - 1 ) * 8))& BUFFER2_WRITE_MASK) + BUFFER2_WRITE_START_INDEX;
							gubKeepLoadAlignmentBQNum = 2;
							if (ENABLE_KEEP_WBQ) {
								if (gubKeepWBQNum == 0) {
									gubFirstKeepBQLink = gubKeepLoadAlignmentBQLink;
								}
								if (gubFirstKeepBQLink != gBQI.ubLinkFirst) {
									gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
								}
							}
						}
						else if (ENABLE_KEEP_WBQ) {
							if (gubKeepWBQNum || (!((ubNeedLoadAlign == 0) && ((gBQI.BufferQueue[gBQI.ubLinkLastUse].ulSectorCnt + gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum) ==  gubSectorsPerPlane)))) {
								UBYTE ubMoveLink = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubPrevious;
								if (gubKeepWBQNum == 0) {
									gubFirstKeepBQLink = gBQI.ubLinkLastUse;
								}
								if (gubFirstKeepBQLink != gBQI.ubLinkFirst) {
									gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
								}
								if (ubNeedLoadAlign && gBQI.BufferQueue[ubMoveLink].btNeedLoadAlign) {
									if (ENABLE_KEEP_WBQ_WITHOUT_LOADALIGN_COLLECT == 0) {
										MoveLink(ubMoveLink);
									}
									else {
										gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
									}
								}

								gubKeepWBQNum ++;
								gubKeepWBQSectorCnt += ( gBQI.BufferQueue[gBQI.ubLinkLastUse].ulSectorCnt + gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum);
								gubKeepWBQ4KCnt += gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum;
								gBQI.ubLinkNum--;
								gBQI.ubWLinkNum--;
							}
						}
						break;
					}
					else if (ENABLE_KEEP_WBQ) {
						if (gubKeepWBQNum || (!((ubNeedLoadAlign == 0) && ((gBQI.BufferQueue[gBQI.ubLinkLastUse].ulSectorCnt + gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum) ==  gubSectorsPerPlane)))) {
							UBYTE ubMoveLink = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubPrevious;
							if (gubKeepWBQNum == 0) {
								gubFirstKeepBQLink = gBQI.ubLinkLastUse;
							}
							if (gubFirstKeepBQLink != gBQI.ubLinkFirst) {
								gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
							}
							if (ubNeedLoadAlign && gBQI.BufferQueue[ubMoveLink].btNeedLoadAlign) {
								if (ENABLE_KEEP_WBQ_WITHOUT_LOADALIGN_COLLECT == 0) {
									MoveLink(ubMoveLink);
								}
								else {
									gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
								}
							}

							gubKeepWBQNum ++;
							gubKeepWBQSectorCnt += ( gBQI.BufferQueue[gBQI.ubLinkLastUse].ulSectorCnt + gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum);
							gubKeepWBQ4KCnt += gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum;
							gBQI.ubLinkNum--;
							gBQI.ubWLinkNum--;
							if (ENABLE_DEBUG_UART_YH) {
								UBYTE ubi;
								UBYTE ubLink =  gBQI.ubLinkFirst;
								UartString("\n0 BQNum:");
								Uart_Tx_DataHex(gBQI.ubLinkNum);
								UartString(" KeepW:");
								Uart_Tx_DataHex(gubKeepWBQNum);
								UartString(" KeepL:");
								Uart_Tx_DataHex(gubKeepLoadAlignmentBQNum);
								UartString(" LastUse:");
								Uart_Tx_DataHex(gBQI.ubLinkLastUse);
								UartString(" FTLLast:");
								Uart_Tx_DataHex(gBQI.ubLinkFTLLast);
								UartString("  ");
								for (ubi = 0; ubi < (gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum); ubi++) {
									if ((ubi == (gBQI.ubLinkNum - 1)) && (ubLink != gBQI.ubLinkFTLLast)) {
										flaLEDBlink(0);
									}
									UartString(" ");
									Uart_Tx_DataHex(ubLink);
									ubLink =  gBQI.BufferQueue[ubLink].ubNext;
								}
								if (gBQI.BufferQueue[ubLink].ubPrevious != gBQI.ubLinkLastUse) {
									flaLEDBlink(0);
								}
							}
						}
					}
					ulLBA += ulTriggerSectorCnt;
					if (gubSDRFlushDoing) {
					Mark_Flush_SDR:
						if (gubSDRFlushDoing == 0) {
							guwSDRTotalFlush4kNum = guwDataInSDR4kNum;
							guwSDRFlushStart4kIndex = guwSDRFlushEnd4kIndex;
							if (ubDDRMode) {
								guwSDRFlushEnd4kIndex = (uwBufferPTR / 8);
							}
							else {
								guwSDRFlushEnd4kIndex = (guwSDRBufferPTR / 8);
							}
							guwFlush4kCount = 0;
							gubSDRFlushDoing = 1;
						}
						UWORD uwi;
						for (uwi = 0 ;  (uwi < 16) && ((guwFlush4kCount + uwi) < guwSDRTotalFlush4kNum) ; uwi++) {
							UWORD uwFlushData4KIndex = (guwSDRFlushStart4kIndex + guwFlush4kCount + uwi) & SDRAM_CACHE_4KNUM_MASK;
							if (SDR_DataEntryTable[uwFlushData4KIndex].btDataValid) {
								if (SDR_DataEntryTable[uwFlushData4KIndex].ubMask != 0xFF) {
									while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
									gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
									BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
									BQ->btWrite = 1;
									BQ->ulVir4kIndex = SDR_DataEntryTable[uwFlushData4KIndex].ulVir4kIndex;
									BQ->ulDataMask = ~(SDR_DataEntryTable[uwFlushData4KIndex].ubMask);
									BQ->uwBufferIndex = uwFlushData4KIndex;
									BQ->ubDataInDRamMap = 1;
									BQ->btNeedLoadAlign = 1;
									BQ->btSpecialData = 0;
									BQ->btNeedCheckSpecialData = 0;
									BQ->btPartialFromFlash = 0;
									BQ->ub4kNum = 1;
									BQ->btFlushWBQ = 0;
									BQ->ubRBQIIndex = 0xFF;
									BQ->ulSpareValid = 0;
									M_AddToBufferQueue(gBQI.ubLinkActive);
								}
							}
						}
						for (uwi = 0 ; (uwi < 16) && (guwFlush4kCount < guwSDRTotalFlush4kNum) ; uwi++, guwFlush4kCount++) {
							UWORD uwFlushData4KIndex = (guwSDRFlushStart4kIndex + guwFlush4kCount) & SDRAM_CACHE_4KNUM_MASK;
							if (SDR_DataEntryTable[uwFlushData4KIndex].btDataValid) {
								while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
								gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
								BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
								BQ->btWrite = 1;
								BQ->ulVir4kIndex = SDR_DataEntryTable[uwFlushData4KIndex].ulVir4kIndex;
								BQ->ulDataMask = SDR_DataEntryTable[uwFlushData4KIndex].ubMask;
								BQ->uwBufferIndex = uwFlushData4KIndex;
								BQ->ubDataInDRamMap = 1;
								BQ->btNeedLoadAlign = 0;
								BQ->btSpecialData = SDR_DataEntryTable[uwFlushData4KIndex].btSpecialData;
								BQ->btNeedCheckSpecialData = SDR_DataEntryTable[uwFlushData4KIndex].btSpecialData;
								BQ->ubZcode[0] = SDR_DataEntryTable[uwFlushData4KIndex].ubZCode;
								BQ->ulSectorCnt = SDR_DataEntryTable[uwFlushData4KIndex].ubZCode;
								BQ->ub4kNum = 1;
								BQ->btFlushWBQ = 0;
								BQ->ulSpareValid = 0;
								M_AddToBufferQueue(gBQI.ubLinkActive);
								gBQI.ubWLinkNum++;
								if (ENABLE_READ_CACHE == 0) {
									U32 ulVir4kIndex =  SDR_DataEntryTable[uwFlushData4KIndex].ulVir4kIndex;
									UWORD uwL1Entry = ulVir4kIndex >> gubDataCacheL2PL1TablePerEntry4kNumLog;
									UWORD uwL2Entry = ulVir4kIndex & gulDataCacheL2PTableEntryL2Mask;
									UWORD uwL2BlockIndex = guwDataCacheL2PTable_L1[uwL1Entry];
									if (guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry] == uwFlushData4KIndex) {
										guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry] = 0xFFFF;
										gubSDRL2PEntryCount[uwL2BlockIndex]--;
										if (gubSDRL2PEntryCount[uwL2BlockIndex] == 0) {
											guwFreeSDRL2PTableLink[guwSDRFreeLinkTail] = uwL2BlockIndex;
											guwSDRFreeLinkTail = uwL2BlockIndex;
											guwSDRFreeTableBlockNum++;
											guwDataCacheL2PTable_L1[uwL1Entry] = 0xFFFF;
										}
									}
								}
							}
							guwDataInSDR4kNum--;
						}
						//wait data program done...
						while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
						gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
						BufferQueue_t *BQ;
						BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
						BQ->btWrite = 1;
						BQ->btFlushWBQ = 1;
						BQ->ulSpareValid = 0;
						M_AddToBufferQueue(gBQI.ubLinkActive);
						while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
							M_SwitchTask();
						}
						if (guwFlush4kCount == guwSDRTotalFlush4kNum) {
							if (ubDDRMode == 0) {
								guwSDRFlushEnd4kIndex = (guwSDRBufferPTR / 8);
								uwBufferPTR = (((ulLBA & SectorsPer4KMask) + guwSRamBufferPTR) & BUFFER2_WRITE_MASK) + BUFFER2_WRITE_START_INDEX;
							}
							gubSDRFlushDoing = 0;
						}
						if (ulSectorCnt == 0) {
							if (((gubSDRFlushDoing) && (((gubNCQDoing == 0) && (gubHostCmdIn == 0)) || (gubNCQDoing && ((ulNCQ_INFO.B.ubNCQ_CMD_VLD_SUM - ubTrigCMDCount) == 0))))) {
								goto Mark_Flush_SDR;
							}
							else {
								break;
							}
						}
					}
				}
			}
		}
		gubPreviousWriteCmdMode = ubDDRMode;
	}
	gubFirstCMDPending = 0;

#if ENABLE_PFAIL
	if (((gubWriteCacheEnable == 0) || gubIsWriteProtect) && (0U == gubErrorCode) && (0U == (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK)))
#else
	if ((gubWriteCacheEnable == 0) || gubIsWriteProtect)
#endif
	{
		HandleStopRW(0);
	}
	gubLargeSizeCmdCount = 0;
	if (ENABLE_KEEP_WBQ && (gulWriteBufferBitMap == 0) && gubKeepLoadAlignmentBQNum) {
		UBYTE ub4kNumIndex;
		UBYTE ubCurrentBufferIndex;
		UBYTE ubPreWBBufferIndex;
		UBYTE ubWriteBufferPTRIndex = ((gBQI.BufferQueue[gBQI.ubLinkLastUse].uwBufferIndex / SectorsPer4K) - BUFFER2_WRITE_4K_START_INDEX) & BUFFER2_WRITE_4KNUM_MASK;
		U32   ulVir4KIndex = gBQI.BufferQueue[gBQI.ubLinkLastUse].ulVir4kIndex;
		for (ub4kNumIndex = 0; ub4kNumIndex < gBQI.BufferQueue[gBQI.ubLinkLastUse].ub4kNum; ub4kNumIndex++) {
			gulWriteBufferVir4KTable[ubWriteBufferPTRIndex] = ulVir4KIndex;       // Vir4K = LBA / 8
			gubWriteBufferDataMaskTable[ubWriteBufferPTRIndex] = (gBQI.BufferQueue[gBQI.ubLinkLastUse].ulDataMask >> (ub4kNumIndex * 8)) & 0xFF;
			UBYTE ubWBufferTableIndex = gulWriteBufferVir4KTable[ubWriteBufferPTRIndex] & 31;         // Vir4K % 32
			gulWriteBufferBitMap |= BIT0 << ubWriteBufferPTRIndex;
			gubWriteBufferTableNext[ubWriteBufferPTRIndex] = Default_WB_Index;

			if (Default_WB_Index == gubWriteBufferTableHead[ubWBufferTableIndex]) {
				gubWriteBufferTableHead[ubWBufferTableIndex] = ubWriteBufferPTRIndex;
			}
			else {
				ubCurrentBufferIndex = gubWriteBufferTableHead[ubWBufferTableIndex];
				ubPreWBBufferIndex = Default_WB_Index;
				while (Default_WB_Index != ubCurrentBufferIndex) {
					ubPreWBBufferIndex = ubCurrentBufferIndex;
					ubCurrentBufferIndex = gubWriteBufferTableNext[ubCurrentBufferIndex];
				}
				if ( Default_WB_Index == gubWriteBufferTableHead[ubWBufferTableIndex]) {
					gubWriteBufferTableHead[ubWBufferTableIndex] = ubWriteBufferPTRIndex;
				}
				else {
					gubWriteBufferTableNext[ubPreWBBufferIndex] = ubWriteBufferPTRIndex;
				}
			}
			ubWriteBufferPTRIndex = (ubWriteBufferPTRIndex + 1) & BUFFER2_WRITE_4KNUM_MASK;
			ulVir4KIndex++;
		}
	}

#if ENABLE_PFAIL
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
#endif
	if (gubWriteProtectWriteDummy && gubIsWriteProtect) {
		M_SetFlagSettingSHF();
	}
#endif
}

void DoneEC_Check(UBYTE ubEnableSwitchTask)
{
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	UWORD uw4kBufferIndex;
	if (gulNCQRDCmdTriggerCnt) {
		UWORD uwECDone;
		if (DCCMSATAB[SATAB_RD_CNT]) {
			while (DCCMSATAB[SATAB_RD_CNT]) {
				M_AssertError(gulNCQRDCmdTriggerCnt == 0);
				UBYTE ubDoneTag = *DONETAG;
				if (gubNCQDoing == 0) {
					ubDoneTag = 0;
				}
				UWORD uw4kIndex;
				uwECDone = WR_NCQ_CMD_INFO[ubDoneTag].uwEC - WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone;
				for (uw4kIndex = 0; uw4kIndex < uwECDone; uw4kIndex++) {
					uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubDoneTag].uwBufferPTR + WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone + uw4kIndex)&gulBuffer2Read4KMask);
					guoFWRead4kBufferFlag &= (~((ULLONG)BIT0 << uw4kBufferIndex));
				}
				if (ENABLE_DEBUG_NCQ_UART_R) {
					UartString("  -");
					Uart_Tx_DataHex(ubDoneTag);
					UartString("  ");
					UartLLongHex(guoFWRead4kBufferFlag);
				}
				WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone = WR_NCQ_CMD_INFO[ubDoneTag].uwEC;
				if (uwECDone && gubPreRead && ((gubPreReadStartTag == 0xFF) || (ubDoneTag == gubPreReadStartTag))) {
					if ((WR_NCQ_CMD_INFO[ubDoneTag].ulLBA + WR_NCQ_CMD_INFO[ubDoneTag].ulSectorCnt)&SectorsPer4KMask) {
						uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubDoneTag].uwBufferPTR + WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone - 1)&gulBuffer2Read4KMask);
						guoFWRead4kBufferFlag |= ((ULLONG)BIT0 << uw4kBufferIndex);
						if (ENABLE_DEBUG_NCQ_UART_R) {
							UartString("  A:");
							UartWordHex(uw4kBufferIndex);
							UartString("  ");
							UartWordHex(WR_NCQ_CMD_INFO[ubDoneTag].uwBufferPTR);
							UartString("  ");
							UartWordHex(WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone);
							UartString("  ");
							UartWordHex(gulBuffer2Read4KMask);
							UartString("  ");
							UartWordHex(((WR_NCQ_CMD_INFO[ubDoneTag].uwBufferPTR + WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone - 1)&gulBuffer2Read4KMask));
						}
					}
					gubPreReadStartTag = 0xFF;
				}
				if (gubPreRead && ((gubPreReadTQListFirst != gubPreReadTQListLast) || (gulNCQRDCmdTriggerCnt == 32))) {
					M_AssertError(ubDoneTag != gubPreReadTQList[gubPreReadTQListFirst]);
					gubPreReadTQListFirst = (gubPreReadTQListFirst + 1) & 31;
				}
				if (gubNCQDoing) {
					gubLastDoneTag = ubDoneTag;
				}
				else {
					gubLastDoneTag = 0x3F;
				}
				if ((M_CommandOperationTime(WR_NCQ_CMD_INFO[ubDoneTag].ulTrigTime)) > guwNCQTimeoutThreshold) {
					if (ENABLE_DEBUG_NCQ_UART_R) {
						UartString(" R Command Timeout Tag:");
						Uart_Tx_DataHex(ubDoneTag);
						UartString("	   LBA:");
						UartLongHex(WR_NCQ_CMD_INFO[ubDoneTag].ulLBA);
						UartString("	   SecCnt:");
						UartLongHex(WR_NCQ_CMD_INFO[ubDoneTag].ulSectorCnt);
						UartString("	   ECDone:");
						UartWordHex(WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone);
						UartString("	   Elapsed:");
						UartLongHex(M_CommandOperationTime(WR_NCQ_CMD_INFO[ubDoneTag].ulTrigTime));
						UartString("	   *\n");
					}
					ftlRecordFailLog(NCQTimeoutLogType, 0x1, ubDoneTag, IsReadNCQ, DONTCARE, DONTCARE);
				}
				DCCMSATAB[SATAB_RPIU] = 1;
				gulNCQRDCmdTriggerCnt--;
				gubCmdDoneCount++;
				gulNCQCMDDebug &= (~((U32)BIT0 << ubDoneTag));
				if (gubSeqCmdCnt) {
					gubSeqCmdCnt--;
				}
				if (gubLBAOverlap && (gubNeedWaitTQDone == 0)) {
					if (HB[HB_NCQ_OVLP_NUM] == 0) {
						if ((gubPreRead == 0) && (gubSeqR == 0) && (gubSeqCmdCnt == 0)) {
							HL[HL_HAT_MISC] &= (~H_TQ_BY_ORDER);
						}
						gubLBAOverlap = 0;
						gubNeedWaitTQDone = 0;
						if (ENABLE_D2HSDBByHW == 1) {
							HW[HW_AUTO_FIS_CTRL] |= (H_DMA_AUTO_REG_R_EN | H_NCQ_AUTO_SDB_R_EN);
						}
						if (HB[HB_NCQ_OVLP_NUM]) {
							gubLBAOverlap = 1;
							gubNeedWaitTQDone = 1;
						}
					}
				}
			}
		}
		else {
			if (gul4KRandomCacheRead == 0) {
				SATA_MSG_TYPE3 ulSATA_MSG_TYPE3;
				ulSATA_MSG_TYPE3.All = gulSATA_MSG_TYPE3[0].All;
				UBYTE ubCurrentTag;
				if (gubNCQDoing) {
					ubCurrentTag = ulSATA_MSG_TYPE3.B.ubCurrentTag;
				}
				else {
					ubCurrentTag = 0;
				}
				if (WR_NCQ_CMD_INFO[ubCurrentTag].uwEC != WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone) {
					if ((gubNCQDoing == 0) || ((gubPreRead == 0) || (ubCurrentTag == gubPreReadTQList[gubPreReadTQListFirst]))) {
						if (gubNCQDoing && (ubCurrentTag != gubLastDoneTag) && (gubLastDoneTag != 0x3F)) {
							if (ulSATA_MSG_TYPE3.B.uwReceived_EC > WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone) {
								uwECDone = ulSATA_MSG_TYPE3.B.uwReceived_EC - WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone;
								UWORD uw4kIndex;
								for (uw4kIndex = 0; uw4kIndex < uwECDone; uw4kIndex++) {
									uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubCurrentTag].uwBufferPTR + WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone + uw4kIndex)&gulBuffer2Read4KMask);
									guoFWRead4kBufferFlag &= (~((ULLONG)BIT0 << uw4kBufferIndex));
								}
								WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone = ulSATA_MSG_TYPE3.B.uwReceived_EC;
								if ((WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone == WR_NCQ_CMD_INFO[ubCurrentTag].uwEC) && gubPreRead && ((gubPreReadStartTag == 0xFF) || (ubCurrentTag == gubPreReadStartTag))) {
									if ((WR_NCQ_CMD_INFO[ubCurrentTag].ulLBA + WR_NCQ_CMD_INFO[ubCurrentTag].ulSectorCnt)&SectorsPer4KMask) {
										uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubCurrentTag].uwBufferPTR + WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone - 1)&gulBuffer2Read4KMask);
										guoFWRead4kBufferFlag |= ((ULLONG)BIT0 << uw4kBufferIndex);
										if (ENABLE_DEBUG_NCQ_UART_R) {
											UartString("  B:");
											UartWordHex(uw4kBufferIndex);
										}
									}
									gubPreReadStartTag = 0xFF;
								}
							}
						}
						else {
							if ((gubNCQDoing == 0) || ((HB[HB_TQ_ACT]&H_TQ_ACT) && ((HB[HB_NCQ_SET] & 0x1F) == ubCurrentTag))) {
								U32 ulRemainSectorCnt = HL[HL_CMD_REMAIN_SEC];
								U32 ulSectorCntDone = (WR_NCQ_CMD_INFO[ubCurrentTag].ulSectorCnt - ulRemainSectorCnt);
								uwECDone = ((ulSectorCntDone + (WR_NCQ_CMD_INFO[ubCurrentTag].ulLBA & SectorsPer4KMask)) / 8);
								if ((ulRemainSectorCnt < WR_NCQ_CMD_INFO[ubCurrentTag].ulSectorCnt) && (uwECDone > WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone)) {
									uwECDone = uwECDone - WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone;
									UWORD uw4kIndex;
									for (uw4kIndex = 0; uw4kIndex < uwECDone; uw4kIndex++) {
										uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubCurrentTag].uwBufferPTR + WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone + uw4kIndex)&gulBuffer2Read4KMask);
										guoFWRead4kBufferFlag &= (~((ULLONG)BIT0 << uw4kBufferIndex));
									}
									WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone += uwECDone;
									if ((WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone == WR_NCQ_CMD_INFO[ubCurrentTag].uwEC) && gubPreRead && ((gubPreReadStartTag == 0xFF) || (ubCurrentTag == gubPreReadStartTag))) {
										if ((WR_NCQ_CMD_INFO[ubCurrentTag].ulLBA + WR_NCQ_CMD_INFO[ubCurrentTag].ulSectorCnt)&SectorsPer4KMask) {
											uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubCurrentTag].uwBufferPTR + WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone - 1)&gulBuffer2Read4KMask);
											guoFWRead4kBufferFlag |= ((ULLONG)BIT0 << uw4kBufferIndex);
											if (ENABLE_DEBUG_NCQ_UART_R) {
												UartString("  C:");
												UartWordHex(uw4kBufferIndex);
											}
										}
										gubPreReadStartTag = 0xFF;
									}
								}
							}
						}
					}
				}
			}
			if (ubEnableSwitchTask) {
				M_SwitchTask();
			}
		}
	}
	else if (ubEnableSwitchTask) {
		M_SwitchTask();
	}
}

void ReadSectors(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();
	//RMA_FLOW
	if (gubIsWriteProtect & (BIT_ERR_UNIT_WRITEPROTECT | BIT_FLASH_INIT_FAIL_WRITEPROTECT)) {
		SDR_RW_DMA();
		return;
	}


	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || (gubNCQSupport == 0 && HB[HB_COMMAND]  == 0x60)
	        || (gubLBA48BitCmdSupport == 0 && (HB[HB_COMMAND]  == 0x24 || HB[HB_COMMAND]  == 0x25))
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}
	U32 ulRightNow = GetRTT_m();
	if (((ulRightNow - gulLastWriteTime) > 5) || ((ulRightNow - gulLastEnterBGTime) > 60)) {
		gubBusyNow = 1;
	}
	else {
		gubBusyNow = 0;
	}
	UWORD uwBufferIndex = guwReadBufferPTR; //Notice, this must be put here where is before goto Discard_Redundant_NCQRCMD

	if ((gubNCQINTRError || gubHardReset || gubLinkLost)) {
		gubNeedChkDoneTag = 0;
		goto Discard_Redundant_NCQRCMD;
	}
	else {
		gubNeedChkDoneTag = 1;
	}

	if (ENABLE_KT_DEBUG) {
		if (gubNCQINTRError) {
			UartString("\r\n Abort R");
			return;
		}
	}

	if (ENABLE_KEEP_WBQ == 0) {
		if (gubKeepLoadAlignmentBQNum) {
			gBQI.ubLinkNum += 2;
			gBQI.ubWLinkNum++;
			gubKeepLoadAlignmentBQLink = 0xFF;
			gubKeepLoadAlignmentBQNum = 0;
		}
		guwNonAlignBufferPTR = 0xFFFF;
	}

	gubSeqCmdCnt = 0;

	if (gubLBAOverlap == 0) {
		HL[HL_HAT_MISC] &= (~H_TQ_BY_ORDER);
	}
	U32 ulLBA, ulSectorCnt;
	BufferQueue_t *BQ;
	U32 ulVir4kIndex;
	UWORD uwPreReadStartBufferIndex;
	UBYTE ubPreReadDataMode = 0;
	U32 ulEndLBA;
	UBYTE ub4kNum;
	UBYTE ubPreReadHit = 0;
	UBYTE ubNCQTag;
	UWORD uwEC;
	RD_NCQ_INFO ulNCQ_INFO, ulNCQ_INFO_FailCheck;
	UBYTE ubTrigTQ = 1;
	UBYTE ubEnableSwitchTask = 1;
	UBYTE ubStopRead = 0;
	UBYTE ubZeroDataStopPreRead = 0;

	//M_SetFlagSettingSHF();
	if (gubSDRFlushDoing) {
		guwSDRFlushEnd4kIndex = (guwSDRFlushStart4kIndex + guwFlush4kCount) & SDRAM_CACHE_4KNUM_MASK;
		gubSDRFlushDoing = 0;
	}
	if (ENABLE_D2HSDBByHW == 1) {
		// (not NCQ)&&(not DMA)&&(not Write)&&(not notRW), means PIO read when send D2H by FW.
		M_EnableHWD2HSDB_R();
		gubNeedSendCmdComplete = 0;
	}
	if (ENABLE_DEBUG_RW && (gulLBA.All == 0x1CF8585)) {
		gubTemp = 1;
	}
	//guwNonAlignBufferPTR = 0xFFFF;

	if (gubNCQDoing) {
		ulNCQ_INFO.All = HL[HL_RD_NCQ_INFO];
		ubNCQTag = ulNCQ_INFO.B.ubNCQ_NEXT_RD;
		ulLBA = NLL[ubNCQTag];

		if ((ulLBA >= gulMediaSize.All) || (ubNCQTag == 0x3f)) {
			goto Discard_Redundant_NCQRCMD;
		}

		ulSectorCnt = NSL[ubNCQTag] & 0x0000FFFF;
		if (ulSectorCnt == 0) {
			ulSectorCnt = 65536;
		}
		uwEC = NEC[ubNCQTag];
		WR_NCQ_CMD_INFO[ubNCQTag].ulTrigTime = gulOperationTime;
		WR_NCQ_CMD_INFO[ubNCQTag].ulLBA = ulLBA;
		WR_NCQ_CMD_INFO[ubNCQTag].ulSectorCnt = ulSectorCnt;
		WR_NCQ_CMD_INFO[ubNCQTag].uwEC = uwEC;
		WR_NCQ_CMD_INFO[ubNCQTag].uwEC_BQDone = 0;
		if (uwEC > 0xFE) {
			uwEC = 0;
		}
		if (ENABLE_DEBUG_NCQ_UART_R) {
			UartString("\n");
			UartLongHex(gulNCQRDCmdTriggerCnt);
			UartString("    R	Tag: ");
			Uart_Tx_DataHex(ubNCQTag);
			UartString("	   LBA:");
			UartLongHex(ulLBA);
			UartString("	   SecCnt:");
			UartLongHex(ulSectorCnt);
		}
		gulNCQCMDCnt ++;
		gulNCQCMDDebug |= ((U32)BIT0 << ubNCQTag);
		goto Mark_CHECK_TABLE_ALLZERO;
	}
	else {
		CheckID();
		ubNCQTag = 0;
		ulLBA = gulLBA.All;

		if (gubErrorCode) {
			//gubErrorCode = ID_ERR;
			goto Discard_Redundant_NCQRCMD;
		}

		ulSectorCnt = gulSectorCnt;
		if (ulSectorCnt == 0) {
			ulSectorCnt = 65536;
		}
		uwEC = ((ulSectorCnt + (ulLBA & SectorsPer4KMask)) / 8);
		if ((ulSectorCnt + (ulLBA & SectorsPer4KMask)) & SectorsPer4KMask) {
			uwEC ++;
		}
		WR_NCQ_CMD_INFO[0].ulTrigTime = gulOperationTime;
		WR_NCQ_CMD_INFO[0].ulLBA = ulLBA;
		WR_NCQ_CMD_INFO[0].ulSectorCnt = ulSectorCnt;
		WR_NCQ_CMD_INFO[0].uwEC = uwEC;
		WR_NCQ_CMD_INFO[0].uwEC_BQDone = 0;
		if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {
			gulLastRCmdLBA = ulLBA;
			guwLastRCmdSector = ulSectorCnt;
		}
		if (ENABLE_DEBUG_NCQ_UART_R) {
			UartLongHex(gulNCQCMDCnt);
			UartString("    R	Tag: ");
			UartString("FF");
			UartString("	   LBA:");
			UartLongHex(ulLBA);
			UartString("	   SecCnt:");
			UartLongHex(ulSectorCnt);
		}
	}
	if (gubIsWriteProtect && ENABLE_DEBUG_UART_WRITEPROTECT) {
		UartString("  LBA:");
		UartLongHex(ulLBA);
		UartString("  SecCnt:");
		UartLongHex(ulSectorCnt);
		UartString("  TotalEC:");
		UartLongHex(uwEC);
	}
	while (1) {
	MARK_READ_CMD_LOOP:
		ulEndLBA = ulLBA + ulSectorCnt;
		ubPreReadHit = 0;
		gulFTLStateForTimeout |= VT->gulFTLState.All;
		if (gubPreRead) {
			if (((ulLBA == gulPreReadStartLBA) || (ubTrigTQ == 0)) && (ulEndLBA <= gulMediaSize.All)) {
				ubPreReadHit = 1;
				uwPreReadStartBufferIndex = guwPreReadStartBufferPTR;
				uwBufferIndex =   guwPreReadEndBufferPTR;
				U32 ulLastFQPlaneIndex = (gulLastFQEntry >> gub4kEntrysPerPlaneLog) + 1;
				if (ENABLE_PLANE_CH_CE) {
					if (ubTrigTQ) {
						gulPreReadStartLBA = ulEndLBA;
						if (ulEndLBA < gulPreReadEndLBA) {
							guwPreReadStartBufferPTR = (uwPreReadStartBufferIndex + (((ulSectorCnt + (ulLBA & SectorsPer4KMask))) & (~SectorsPer4KMask)))&gulBufferr2ReadMask;
							if (( gulPreReadEndLBA - gulPreReadStartLBA) < gulPreReadSectors) {
								ulSectorCnt = gulPreReadSectors + ((gubBurstsPerBank - (ulLastFQPlaneIndex & gubBurstsPerBankMask)) << gubSectorsPerPlaneLog);
							}
							else if (( gulPreReadEndLBA - gulPreReadStartLBA) < 1024) {
								ulSectorCnt =  ((gubBurstsPerBank - (ulLastFQPlaneIndex & gubBurstsPerBankMask)) << gubSectorsPerPlaneLog);
							}
							else {
								ulSectorCnt = 0;
							}
						}
						else {
							ulSectorCnt = (gulPreReadStartLBA -  gulPreReadEndLBA) + gulPreReadSectors;
							if ((gulPreReadStartLBA - gulPreReadEndLBA) & gubSectorsPerPlaneMask) {
								ulSectorCnt += (gubSectorsPerPlane - ((gulPreReadStartLBA - gulPreReadEndLBA)&gubSectorsPerPlaneMask));
							}
							ulSectorCnt += (((gubBurstsPerBank - (ulLastFQPlaneIndex & gubBurstsPerBankMask) - ((gulPreReadStartLBA -  gulPreReadEndLBA) >> gubSectorsPerPlaneLog)) & gubBurstsPerBankMask) << gubSectorsPerPlaneLog);
						}
					}
					else {

						ulSectorCnt = gulPreReadSectors + ((gubBurstsPerBank - (ulLastFQPlaneIndex & gubBurstsPerBankMask)) << gubSectorsPerPlaneLog);
					}
				}
				else {
					if (ubTrigTQ) {
						gulPreReadStartLBA = ulEndLBA;
						if (ulEndLBA < gulPreReadEndLBA) {
							guwPreReadStartBufferPTR = (uwPreReadStartBufferIndex + (((ulSectorCnt + (ulLBA & SectorsPer4KMask))) & (~SectorsPer4KMask)))&gulBufferr2ReadMask;
							if (( gulPreReadEndLBA - gulPreReadStartLBA) < gulPreReadSectors) {
								ulSectorCnt = gulPreReadSectors + ((gubPlanesPerBank - (ulLastFQPlaneIndex & gubPlanesPerBankMask)) << gubSectorsPerPlaneLog);
							}
							else {
								ulSectorCnt =  ((gubPlanesPerBank - (ulLastFQPlaneIndex & gubPlanesPerBankMask)) << gubSectorsPerPlaneLog);
							}
						}
						else {
							ulSectorCnt = (gulPreReadStartLBA -  gulPreReadEndLBA) + gulPreReadSectors;
							if ((gulPreReadStartLBA - gulPreReadEndLBA) & gubSectorsPerPlaneMask) {
								ulSectorCnt += (gubSectorsPerPlane - ((gulPreReadStartLBA - gulPreReadEndLBA)&gubSectorsPerPlaneMask));
							}
							ulSectorCnt += (((gubPlanesPerBank - (ulLastFQPlaneIndex & gubPlanesPerBankMask) - ((gulPreReadStartLBA -  gulPreReadEndLBA) >> gubSectorsPerPlaneLog)) & gubPlanesPerBankMask) << gubSectorsPerPlaneLog);
						}
					}
					else {

						ulSectorCnt = gulPreReadSectors + ((gubPlanesPerBank - (ulLastFQPlaneIndex & gubPlanesPerBankMask)) << gubSectorsPerPlaneLog);
					}
				}
				gulPreReadTriggerSectorCnt = gulPreReadEndLBA + ulSectorCnt - gulPreReadStartLBA;
				while (gubLimitPrereadCount && (gulPreReadTriggerSectorCnt > BUFFER2_Read_NUM)) {
					ulSectorCnt -= (U32)gubSectorsPerPlane;
					gulPreReadTriggerSectorCnt -= (U32)gubSectorsPerPlane;
				}
				gulLastFQEntry = 0;
				ulLBA = gulPreReadEndLBA;
				gulPreReadEndLBA = (ulLBA + ulSectorCnt) ;
			}
			else {
			MarkStopPreRead:
				if (ENABLE_DEBUG_NCQ_UART_R) {
					UartString("\nPreRead Stop  ");
					UartLongHex(gulNCQRDCmdTriggerCnt);
				}
				while (gulNCQRDCmdTriggerCnt) {
#if  ENABLE_READ_CHECK_FUNCTION
					//if (gulNCQRDCmdTriggerCnt) {
					//[cppcheck][Identical inner 'if' condition is always true.]
					{
						if (DCCMSATAB[SATAB_RD_CNT]) {
							if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
								guwForceRead_Info->uwQueueQuota = 1;
							}
						}
					}
#endif
					DoneEC_Check(ubEnableSwitchTask);
					if (gubNeedChkDoneTag == 0) {
						goto Discard_Redundant_NCQRCMD;
					}
				}
				while (gBQI.ubWLinkNum || gFQI.ubFQLinkNumber_W || gubLoadAlignDoing) {
					M_SwitchTask();
				}
				gubClearPreRead = 1;
				gubSeqR = 0;
				gubPreReadCheckCount = 0;
				guoFWRead4kBufferFlag = 0;
				M_SetFlagSettingNone();
				AXIL[AXIL_SET_START_FLAG] = 0;
				AXIL[AXIL_SET_FLAG_LENGTH] = gulBuffer2ReadNum;
				AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
				while (gBQI.ubRLinkNum || gFQI.ubFQLinkNumber) {
					M_SwitchTask();
				}
				M_Clear_UNC_Addr();
				gubPreRead = 0;
				gulPreReadTriggerSectorCnt = 0;
				mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 0);//強迫DMAC 全部做完
				M_SetFlagSettingSHF();
				gubClearPreRead = 0;
				if (ENABLE_DEBUG_NCQ_UART_R) {
					UartString("  @");
					UartLLongHex(guoFWRead4kBufferFlag);
					UartString("\n");
				}
				if (gubLBAOverlap == 0) {
					HL[HL_HAT_MISC] &= (~H_TQ_BY_ORDER);
				}
				if (ubZeroDataStopPreRead) {
					ubZeroDataStopPreRead = 0;
					goto MARK_NEXT_READ_CMD;
				}
			}
		}


		if (ENABLE_READ_DISTURB_PREREAD && (gubPreRead == 0)) {
			//if ((gubPreReadCheckCount >= 3) || (ulSectorCnt > 64)) {
			//if ((ulLBA == gulPreReadStartLBA) && (gubPreReadCheckCount >= 3)) {
			if ((ulLBA == gulPreReadStartLBA) && (gubPreReadCheckCount >= 3) && ((gubNCQDoing == 0) || (ulNCQ_INFO.B.ubNCQ_CMD_VLD_SUM == 1)) && (ulEndLBA <= gulMediaSize.All)) {
				while (1) {
					if (gulNCQRDCmdTriggerCnt == 0) {
						break;
					}
					else if (gubNeedChkDoneTag == 0) {
						goto Discard_Redundant_NCQRCMD;
					}
#if  ENABLE_READ_CHECK_FUNCTION
					if (gulNCQRDCmdTriggerCnt) {
						if (DCCMSATAB[SATAB_RD_CNT]) {
							if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
								guwForceRead_Info->uwQueueQuota = 1;
							}
						}
					}
#endif
					DoneEC_Check(ubEnableSwitchTask);
				}
				HL[HL_HAT_MISC] |= H_TQ_BY_ORDER;
				gubPreRead = 1;
				gulPreReadTriggerSectorCnt = ((gubSectorsPerPlane - (ulEndLBA & gubSectorsPerPlaneMask)) + gulPreReadSectors);
				while (gubLimitPrereadCount && (gulPreReadTriggerSectorCnt > BUFFER2_Read_NUM)) {
					gulPreReadTriggerSectorCnt -= (U32)gubSectorsPerPlane;
				}
				ulSectorCnt += (U32)gulPreReadTriggerSectorCnt;
				gulPreReadEndLBA = (ulLBA + ulSectorCnt) ;
				gubPreReadStartTag = ubNCQTag;
				gubPreReadTQListFirst = gubPreReadTQListLast;
			}
			else if (ulLBA == gulPreReadStartLBA) {
				gubPreReadCheckCount++;
			}
			else {
				gubPreReadCheckCount = 0;
			}
			gulPreReadStartLBA = ulEndLBA ;
		}
		if (ubTrigTQ) {
			if (gubSeqR) {
				if (WR_NCQ_CMD_INFO[ubNCQTag].ulLBA != gulPreviousEndLBA) {
					gubNonSeqCount++;
					if (gubNonSeqCount > 1) {
						gubSeqR = 0;
					}
				}
				else {
					gubNonSeqCount = 0;
				}
			}
			else {
				if ((WR_NCQ_CMD_INFO[ubNCQTag].ulSectorCnt > 128) || (WR_NCQ_CMD_INFO[ubNCQTag].ulLBA == gulPreviousEndLBA)) {
					while (1) {
						if (gulNCQRDCmdTriggerCnt == 0) {
							break;
						}
						else if (gubNeedChkDoneTag == 0) {
							goto Discard_Redundant_NCQRCMD;
						}
#if  ENABLE_READ_CHECK_FUNCTION
						if (gulNCQRDCmdTriggerCnt) {
							if (DCCMSATAB[SATAB_RD_CNT]) {
								if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
									guwForceRead_Info->uwQueueQuota = 1;
								}
							}
						}
#endif
						DoneEC_Check(ubEnableSwitchTask);
					}
					HL[HL_HAT_MISC] |= H_TQ_BY_ORDER;
					gubSeqR = 1;

					gubNonSeqCount = 0;
					gubSeqCmdCnt = 0;
				}
			}
			gulPreviousEndLBA = (WR_NCQ_CMD_INFO[ubNCQTag].ulLBA + WR_NCQ_CMD_INFO[ubNCQTag].ulSectorCnt);
		}

		if (gubNeedChkDoneTag == 0) {
			goto Discard_Redundant_NCQRCMD;
		}
		if (gubNeedWaitTQDone && ubTrigTQ) {
			WR_NCQ_CMD_INFO[ubNCQTag].uwEC_BQDone = WR_NCQ_CMD_INFO[ubNCQTag].uwEC;
			gulNCQCMDDebug &= (~((U32)BIT0 << ubNCQTag));
			if (gubPreRead) {
				if (gubPreReadStartTag != ubNCQTag) {
					gulPreReadEndLBA = ulLBA;
					guwPreReadStartBufferPTR = uwPreReadStartBufferIndex;
				}
				else {
					gubPreRead = 0;
					gulPreReadTriggerSectorCnt = 0;
				}
				gulPreReadStartLBA = WR_NCQ_CMD_INFO[ubNCQTag].ulLBA;
				ulSectorCnt = 0;
			}
			goto MARK_NEXT_READ_CMD;
		}

		if (ubTrigTQ) { // ERIC: add into TQ content
			UWORD uwBufferPTR = ((ubPreReadHit ? uwPreReadStartBufferIndex : uwBufferIndex) / 8);
			if (gubNCQDoing) {
				HL[HL_TQ_CONTENT] = H_TQN_UPD_PTR | (ubNCQTag << 18) | (uwEC << 24) | uwBufferPTR;
			}
			else {
				HL[HL_TQ_CONTENT] = H_TQN_UPD_PTR | uwBufferPTR;
			}
			if (gubSeqR) {
				gubSeqCmdCnt++;
			}
			if (ENABLE_DEBUG_NCQ_UART_R) {
				if (ubPreReadHit) {
					UartString("  P");
				}
				else {
					UartString("   ");
				}
				UartString("  ");
				Uart_Tx_DataHex(uwBufferPTR);
				UartString("  ");
				UartLongHex(gulPreReadStartLBA);
				UartString("  ");
				UartLongHex(gulPreReadEndLBA);
				UartString("\n");
			}
			if (Failed_Test_CRC_RD) {
				UWORD uwfailGen;
				if (ENABLE_KT_DEBUG_MOREFAIL)
					uwfailGen = getIntRandValue(0x100);	//rand() & 0xff
				else
					uwfailGen = getIntRandValue(0x1000);	//rand() & 0xfff

				if (uwfailGen == 0) {
					HW[HW_ERR_CTRL] |= H_REG_CRC_DATAERR_EN;
				}
			}
			gulNCQRDCmdTriggerCnt++;
			WR_NCQ_CMD_INFO[ubNCQTag].uwBufferPTR = uwBufferPTR;
			if (gubPreRead) {
				gubPreReadTQList[gubPreReadTQListLast] = ubNCQTag;
				gubPreReadTQListLast = (gubPreReadTQListLast + 1) & 31;
			}

			if (WR_NCQ_CMD_INFO[ubNCQTag].uwEC == 1) {
				guw4KRandomCacheReadCount++;
			}
			else {
				guw4KRandomCacheReadCount = 0;
				gul4KRandomCacheRead = 0;
			}
			if (guw4KRandomCacheReadCount > 500) {
				gul4KRandomCacheRead = 1;
			}
			gulPreviousLBA = WR_NCQ_CMD_INFO[ubNCQTag].ulLBA;
			VT->guoTotalHostReadSectorCnt +=  WR_NCQ_CMD_INFO[ubNCQTag].ulSectorCnt;
		}

		ulVir4kIndex = ulLBA / 8;
		ubPreReadDataMode = 0;

		while (ulSectorCnt) {
			U32 ulDataMask;
			U32 ulReadFromFlashMask = 0;
			U32 ulCopyFromDRamMask = 0;
			U32 ulSignBufferFlagMask = 0;
			UBYTE ubReadFromFlashFirst4kIndex;
			UBYTE ubCopyFromDRamFirst4kIndex = 0;
			UWORD uwReadFromFlashBufferIndex;
			UWORD uwCopyFromDramBufferIndex;
			U32 ulReadFromFlashVir4kIndex;
			U32 ulCopyFromDRamVir4kIndex;
			UBYTE ubNeedToLoadFromFlash = 0;
			UBYTE ubNeedToCopyFromDram = 0;
			UBYTE ubPartialReadFromFlash = 0;
			UBYTE ubPreReadStart;
			UBYTE ubMatchMap = 0;
			UBYTE ubNeedSearchCount = 0;
			UBYTE ub4kIndex;
			UWORD uwTableIndex[4];
			U32 ulDDRMask[4];
			//Write Buffer
			UBYTE ubWBPTR[4];
			UBYTE ubWBufferTableIndex;
			UBYTE ubWriteBufferPTR;
			for (ub4kIndex = 0; ub4kIndex < gub4kEntrysPerPlane; ub4kIndex++) {
				//if (((ulLBA + ulSectorCnt) / 8) >= (ulVir4kIndex + ub4kIndex)) {
				if ((((ulLBA + ulSectorCnt) / 8) >= (ulVir4kIndex + ub4kIndex)) && (((ulVir4kIndex + ub4kIndex) >> gub4kEntrysPerPlaneLog) == (ulVir4kIndex >> gub4kEntrysPerPlaneLog))) {
					ubNeedSearchCount++;
					if (guwDataInSDR4kNum) {
						UWORD uwL1Entry = (ulVir4kIndex + ub4kIndex) >> gubDataCacheL2PL1TablePerEntry4kNumLog;
						UWORD uwL2Entry = ((ulVir4kIndex + ub4kIndex) & gulDataCacheL2PTableEntryL2Mask);
						UWORD uwL2BlockIndex = guwDataCacheL2PTable_L1[uwL1Entry];
						uwTableIndex[ub4kIndex] = guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry];
						if ((uwL2BlockIndex != 0xFFFF) && ( uwTableIndex[ub4kIndex] != 0xFFFF)) {
							ubMatchMap |= (BIT0 << ub4kIndex);
							ulDDRMask[ub4kIndex] = (U32)SDR_DataEntryTable[uwTableIndex[ub4kIndex]].ubMask;
						}
					}
					else if (ENABLE_KEEP_WBQ) {
						ubWBufferTableIndex = (ulVir4kIndex + ub4kIndex) & BUFFER2_WRITE_4KNUM_MASK;
						ubWriteBufferPTR = gubWriteBufferTableHead[ubWBufferTableIndex];
						//Search WriteBuffer Table
						while (Default_WB_Index != ubWriteBufferPTR) {
							if ((ulVir4kIndex + ub4kIndex) == gulWriteBufferVir4KTable[ubWriteBufferPTR]) {
								ubWBPTR[ub4kIndex] = ubWriteBufferPTR;
								uwTableIndex[ub4kIndex] = ubWriteBufferPTR;
								ubMatchMap |= (BIT0 << ub4kIndex);
								ulDDRMask[ub4kIndex] = (U32)gubWriteBufferDataMaskTable[ubWriteBufferPTR];
								break;
							}
							ubWriteBufferPTR = gubWriteBufferTableNext[ubWriteBufferPTR];
						}
					}
				}
				else {
					break;
				}
			}
			ub4kNum = 0;
			for (ub4kIndex = 0; (ub4kIndex < ubNeedSearchCount) && (ulSectorCnt > 0); ub4kIndex++) {
				if (ulLBA >= gulPreReadStartLBA) {
					if ((gulPreReadStartLBA / 8) == (ulLBA / 8)) {
						guwPreReadStartBufferPTR = (uwBufferIndex + (ub4kNum * 8))&gulBufferr2ReadMask;
					}
					ubPreReadDataMode = 1;
				}
				ubPreReadStart = 0;
				ulDataMask = gulMaskTable[ulLBA & SectorsPer4KMask];
				if (gubPreRead && (ubPreReadDataMode == 0) && ((gulPreReadStartLBA / 8) == (ulLBA / 8))) {
					ubPreReadStart = 1;
					guwPreReadStartBufferPTR = (uwBufferIndex + (ub4kNum * 8))&gulBufferr2ReadMask;
				}
				else {
					if (((ulLBA & SectorsPer4KMask) + ulSectorCnt) < SectorsPer4K) {
						ulDataMask &= (~gulMaskTable[(ulSectorCnt + ulLBA)&SectorsPer4KMask]);
					}
				}
				UBYTE ubSDRHitAll = 0;
				UBYTE ubWBHitAll = 0;
				ulSignBufferFlagMask |= ((ulDataMask & 0xFF) << (ub4kIndex * 8));
				if (guwDataInSDR4kNum) {
					if ((ubMatchMap & (BIT0 << ub4kIndex)) && (ulDataMask & ulDDRMask[ub4kIndex] & 0xFF)) {
						if (ulDataMask & (~ulDDRMask[ub4kIndex]) & 0xFF) {
							if (ubNeedToLoadFromFlash == 0) {
								ubReadFromFlashFirst4kIndex = ub4kIndex;
								uwReadFromFlashBufferIndex = uwBufferIndex;
								ulReadFromFlashVir4kIndex = ulVir4kIndex;
							}
							ubPartialReadFromFlash = 1;
							ubNeedToLoadFromFlash = 1;
							ulReadFromFlashMask |= ((ulDataMask & (~ulDDRMask[ub4kIndex]) & 0xFF) << (ub4kIndex * 8));
							ulDataMask &= ulDDRMask[ub4kIndex];
							ulCopyFromDRamMask |= ((ulDataMask & 0xFF) << (ub4kIndex * 8));
						}
						else {
							ubSDRHitAll = 1;
							ulCopyFromDRamVir4kIndex = ulVir4kIndex;
							uwCopyFromDramBufferIndex = uwBufferIndex;
							ubCopyFromDRamFirst4kIndex = ub4kIndex;
						}
						ubNeedToCopyFromDram = 1;
					}
					else {
						if (ubNeedToLoadFromFlash == 0) {
							ubReadFromFlashFirst4kIndex = ub4kIndex;
							uwReadFromFlashBufferIndex = uwBufferIndex;
							ulReadFromFlashVir4kIndex = ulVir4kIndex;
						}
						ubNeedToLoadFromFlash = 1;
						ulReadFromFlashMask |= ((ulDataMask & 0xFF) << (ub4kIndex * 8));
					}
				}
				else {
					if ((ubMatchMap & (BIT0 << ub4kIndex)) && (ulDataMask & ulDDRMask[ub4kIndex] & 0xFF)) {
						if (ulDataMask & (~ulDDRMask[ub4kIndex]) & 0xFF) {
							// Need Load Alignment
							if (ENABLE_DEBUG_UART_YT) {
								//UartString("\n Need Load From Flash...");
							}

							if (ubNeedToLoadFromFlash == 0) {
								ubReadFromFlashFirst4kIndex = ub4kIndex;
								uwReadFromFlashBufferIndex = uwBufferIndex;
								ulReadFromFlashVir4kIndex = ulVir4kIndex;
							}
							ubPartialReadFromFlash = 1;
							ubNeedToLoadFromFlash = 1;
							ulReadFromFlashMask |= ((ulDataMask & (~ulDDRMask[ub4kIndex]) & 0xFF) << (ub4kIndex * 8));   // flash
							ulDataMask &= ulDDRMask[ub4kIndex];
							ulCopyFromDRamMask |= ((ulDataMask & 0xFF) << (ub4kIndex * 8));                                 //Buffer
						}
						else {
							ubWBHitAll = 1;
							if (ubNeedToLoadFromFlash) {
								ulDataMask &= ulDDRMask[ub4kIndex];
								ulCopyFromDRamMask |= ((ulDataMask & 0xFF) << (ub4kIndex * 8));                                 //Buffer
							}
							ulCopyFromDRamVir4kIndex = ulVir4kIndex;
							uwCopyFromDramBufferIndex = uwBufferIndex;
							ubCopyFromDRamFirst4kIndex = ub4kIndex;
						}
						ubNeedToCopyFromDram = 1;
					}
					else {
						if (ubNeedToLoadFromFlash == 0) {
							ubReadFromFlashFirst4kIndex = ub4kIndex;
							uwReadFromFlashBufferIndex = uwBufferIndex;
							ulReadFromFlashVir4kIndex = ulVir4kIndex;
						}
						ubNeedToLoadFromFlash = 1;
						ulReadFromFlashMask |= ((ulDataMask & 0xFF) << (ub4kIndex * 8));
					}
				}
				ub4kNum++;
				if (ubWBHitAll || ubSDRHitAll || (ub4kIndex == (ubNeedSearchCount - 1)) || (((ulLBA & SectorsPer4KMask) + ulSectorCnt) <= SectorsPer4K) || ((ulVir4kIndex & guwL2PEntrysPerL2PGroupMask) == guwL2PEntrysPerL2PGroupMask)) {
					//if (ubSDRHitAll || (ub4kIndex == (ubNeedSearchCount - 1)) || (((ulLBA & SectorsPer4KMask) + ulSectorCnt) <= SectorsPer4K) || ((ulVir4kIndex & guwL2PEntrysPerL2PGroupMask) == guwL2PEntrysPerL2PGroupMask) || (ubPreReadStart || (((gulPreReadStartLBA & SectorsPer4KMask) == 0) && ((gulPreReadStartLBA / 8) == ((ulLBA / 8) + 1))))) {
					//if ((ub4kIndex == (ubNeedSearchCount - 1)) || (((ulLBA & gubSectorsPer4KMask) + ulSectorCnt) <= gubSectorsPer4K) || ((ulVir4kIndex & guwL2PEntrysPerL2PGroupMask) == guwL2PEntrysPerL2PGroupMask) || (ubPreReadStart || (((gulPreReadStartLBA & gubSectorsPer4KMask) == 0) && ((gulPreReadStartLBA / 8) == ((ulLBA / 8) + 1))))) {
					if (ubNeedToLoadFromFlash) {
						if (gubPreRead && ((gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum) >= BQ_NUM) && ((ulReadFromFlashVir4kIndex * 8) >= gulPreReadStartLBA)) {
							gulPreReadEndLBA = (ulReadFromFlashVir4kIndex * 8);
							ulSectorCnt = 0;
							uwBufferIndex = uwReadFromFlashBufferIndex;
							goto MARK_FINISH_SET_BQ;
						}
						while ((gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum) >= BQ_NUM) {
							gubRBQFull = 1;
#if  ENABLE_READ_CHECK_FUNCTION
							if (gulNCQRDCmdTriggerCnt) {
								if (DCCMSATAB[SATAB_RD_CNT]) {
									if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
										guwForceRead_Info->uwQueueQuota = 1;
									}
								}
							}
#endif
							DoneEC_Check(ubEnableSwitchTask);
							gubRBQFull = 0;
						}
						//while (gBQI.ubLinkNum >= 8)M_DoneEC_Check();
						gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
						BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
						BQ->btWrite = 0;
						BQ->ulVir4kIndex = ulReadFromFlashVir4kIndex;
						BQ->ulDataMask = ulReadFromFlashMask >> (ubReadFromFlashFirst4kIndex * 8);
						BQ->uwBufferIndex = uwReadFromFlashBufferIndex;
						BQ->ubRBQIIndex = 0xFF;
						BQ->ubDataInDRamMap = 0;
						BQ->btPreRead = ubPreReadDataMode;
						BQ->btPartialFromFlash = ubPartialReadFromFlash;
						BQ->ub4kNum = (ub4kIndex - ubReadFromFlashFirst4kIndex + 1);
						if (ubSDRHitAll || ubWBHitAll) {
							BQ->ub4kNum--;
						}
						BQ->ulSpareValid = 0;
						BQ->btCopyFromWB = 0;
						BQ->ubSerialNum = gubBQSerialNum;
						gubBQSerialNum++;
						UBYTE ubMoveLink = gBQI.ubLinkActive;
						gBQI.ubRLinkNum++;
						M_AddToBufferQueue(gBQI.ubLinkActive);
						if (ENABLE_KEEP_WBQ && (gubKeepWBQNum || gubKeepLoadAlignmentBQNum)) {
							MoveLink(ubMoveLink);
						}
						uwBufferIndex = (uwBufferIndex + (BQ->ub4kNum << SectorsPer4KLog))&gulBufferr2ReadMask;
						ubNeedToLoadFromFlash = 0;
						if (gulReadZeroDataCnt > 16) {
							M_SwitchTask();
						}
					}
					if (ubNeedToCopyFromDram) {
						if (ubPartialReadFromFlash) {
							while ((gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum) >= BQ_NUM) {
#if  ENABLE_READ_CHECK_FUNCTION
								if (gulNCQRDCmdTriggerCnt) {
									if (DCCMSATAB[SATAB_RD_CNT]) {
										if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
											guwForceRead_Info->uwQueueQuota = 1;
										}
									}
								}
#endif
								DoneEC_Check(ubEnableSwitchTask);
							}
							gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
							BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
							BQ->btWrite = 0;
							BQ->ulVir4kIndex = ulReadFromFlashVir4kIndex;
							BQ->uwBufferIndex = uwReadFromFlashBufferIndex;
							BQ->ubRBQIIndex = 0xFF;
							BQ->ubDataInDRamMap = 0;
							BQ->ub4kNum = 0;
							UBYTE ubFirst4kIndex;
							UBYTE ubLast4kIndex;
							ubFirst4kIndex = ubReadFromFlashFirst4kIndex;
							if (ubSDRHitAll || ubWBHitAll) {
								ubLast4kIndex = ub4kIndex - 1;
							}
							else {
								ubLast4kIndex = ub4kIndex;
							}
							BQ->btPreRead = ubPreReadDataMode;
							BQ->ulDataMask = ulCopyFromDRamMask >> (ubReadFromFlashFirst4kIndex * 8);
							UBYTE ub4kIndex2;
							for (ub4kIndex2 = ubFirst4kIndex ; ub4kIndex2 <= ubLast4kIndex ; ub4kIndex2++) {
								if ((ubMatchMap & (BIT0 << ub4kIndex2)) && ((BQ->ulDataMask >> ((ub4kIndex2 - ubFirst4kIndex) * 8)) & 0xFF)) {
									BQ->u.uwDataInDRamIndex[ub4kIndex2 - ubFirst4kIndex] = uwTableIndex[ub4kIndex2];
								}
								BQ->ubDataInDRamMap |= (BIT0 << (ub4kIndex2 - ubFirst4kIndex));
								BQ->ub4kNum++;
							}
							BQ->btPartialFromFlash = 1;
							BQ->ulSpareValid = 0;
							BQ->btCopyFromWB = 1;
							BQ->ubSerialNum = gubBQSerialNum;
							gubBQSerialNum++;
							UBYTE ubMoveLink = gBQI.ubLinkActive;
							gBQI.ubRLinkNum++;
							M_AddToBufferQueue(gBQI.ubLinkActive);
							if (ENABLE_KEEP_WBQ && (gubKeepWBQNum || gubKeepLoadAlignmentBQNum)) {
								MoveLink(ubMoveLink);
							}
						}
						if (ubWBHitAll) {
							while ((gBQI.ubLinkNum + gubKeepWBQNum + gubKeepLoadAlignmentBQNum) >= BQ_NUM) {
#if  ENABLE_READ_CHECK_FUNCTION
								if (gulNCQRDCmdTriggerCnt) {
									if (DCCMSATAB[SATAB_RD_CNT]) {
										if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
											guwForceRead_Info->uwQueueQuota = 1;
										}
									}
								}
#endif
								DoneEC_Check(ubEnableSwitchTask);
							}
							gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
							BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
							BQ->btWrite = 0;
							BQ->ulVir4kIndex = ulCopyFromDRamVir4kIndex;
							BQ->uwBufferIndex = uwBufferIndex;
							BQ->ubRBQIIndex = 0xFF;
							BQ->ubDataInDRamMap = 0;
							if ((ubMatchMap & (BIT0 << ub4kIndex)) && ((ulSignBufferFlagMask >> (ub4kIndex * 8)) & 0xFF)) {
								BQ->u.uwDataInDRamIndex[0] = ubWBPTR[ub4kIndex];
								BQ->ubDataInDRamMap |= BIT0;
							}
							BQ->btPreRead = ubPreReadDataMode;
							BQ->ulDataMask = (ulSignBufferFlagMask >> (8 * ub4kIndex)) & 0xFF;
							BQ->btPartialFromFlash = 0;
							BQ->ub4kNum = 1;
							BQ->ulSpareValid = 0;
							BQ->btCopyFromWB = 1;
							UBYTE ubMoveLink = gBQI.ubLinkActive;
							gBQI.ubRLinkNum++;
							M_AddToBufferQueue(gBQI.ubLinkActive);
							if (ENABLE_KEEP_WBQ && (gubKeepWBQNum || gubKeepLoadAlignmentBQNum)) {
								MoveLink(ubMoveLink);
							}
							uwBufferIndex = (uwBufferIndex + SectorsPer4K)&gulBufferr2ReadMask;
						}


						if (ubSDRHitAll) {
							while (gBQI.ubLinkNum >= BQ_NUM) {
#if  ENABLE_READ_CHECK_FUNCTION
								if (gulNCQRDCmdTriggerCnt) {
									if (DCCMSATAB[SATAB_RD_CNT]) {
										if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
											guwForceRead_Info->uwQueueQuota = 1;
										}
									}
								}
#endif
								DoneEC_Check(ubEnableSwitchTask);
							}
							gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
							BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
							BQ->btWrite = 0;
							BQ->ulVir4kIndex = ulCopyFromDRamVir4kIndex;
							BQ->uwBufferIndex = uwBufferIndex;
							BQ->ubRBQIIndex = 0xFF;
							BQ->ubDataInDRamMap = 0;
							if ((ubMatchMap & (BIT0 << ub4kIndex)) && ((ulSignBufferFlagMask >> (ub4kIndex * 8)) & 0xFF)) {
								BQ->u.uwDataInDRamIndex[0] = uwTableIndex[ub4kIndex];
								BQ->ubDataInDRamMap |= BIT0;
							}
							BQ->btPreRead = ubPreReadDataMode;
							BQ->ulDataMask = (ulSignBufferFlagMask >> (8 * ub4kIndex)) & 0xFF;
							BQ->btPartialFromFlash = 0;
							BQ->ub4kNum = 1;
							BQ->ulSpareValid = 0;
							BQ->btCopyFromWB = 0;
							M_AddToBufferQueue(gBQI.ubLinkActive);
							uwBufferIndex = (uwBufferIndex + SectorsPer4K)&gulBufferr2ReadMask;
						}
						ubNeedToCopyFromDram = 0;
					}
					ubCopyFromDRamFirst4kIndex = ub4kIndex + 1;
					ulCopyFromDRamMask = 0;
					ubPartialReadFromFlash = 0;
					ub4kNum = 0;
				}

				ulVir4kIndex += 1;
				if (((ulLBA & SectorsPer4KMask) + ulSectorCnt) <= SectorsPer4K) {
					ulSectorCnt = 0;
				}
				else {
					ulSectorCnt -= (SectorsPer4K - (ulLBA & SectorsPer4KMask));
				}
				ulLBA = (ulLBA | SectorsPer4KMask) + 1;
			}
		}
	MARK_FINISH_SET_BQ:
		if (gubPreRead ) {
			guwPreReadEndBufferPTR = uwBufferIndex;
			if (guwPreReadEndBufferPTR & SectorsPer4KMask) {
				guwPreReadEndBufferPTR = ((guwPreReadEndBufferPTR & (~(UWORD)SectorsPer4KMask)) + SectorsPer4K)&gulBufferr2ReadMask;
			}
		}
		U32 ulWaitCnt = 0;
	MARK_NEXT_READ_CMD:
		if (gubNCQDoing) { //如果有set
			HL[HL_CON_LBA_CHK_LBA] = gulPreReadStartLBA;//之後再討論
			while (HL[HL_RD_NCQ_INFO] & H_CON_LBA_CHK_TRG);

			ulNCQ_INFO.All = HL[HL_RD_NCQ_INFO];
			if (gubLBAOverlap) {
				if (gubNeedWaitTQDone) {
				MARK_CMD_POLLING:
					while (gulNCQRDCmdTriggerCnt) {
						if (gubNeedChkDoneTag == 0) {
							goto Discard_Redundant_NCQRCMD;
						}
#if  ENABLE_READ_CHECK_FUNCTION
						if (gulNCQRDCmdTriggerCnt) {
							if (DCCMSATAB[SATAB_RD_CNT]) {
								if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
									guwForceRead_Info->uwQueueQuota = 1;
								}
							}
						}
#endif
						DoneEC_Check(ubEnableSwitchTask);
					}
					if (ENABLE_READ_DISTURB_TQ_ORDER && gubReadDisturbNeedWaitTQDone && gubReadDisturbStartPollingCMD) {
						gubReadDisturbNeedWaitTQDone = 0;
					}
					else {
						gubNeedWaitTQDone = 0;
					}
					HL[HL_HAT_MISC] |= H_TQ_BY_ORDER;
				}
				if ((ulNCQ_INFO.B.ubNCQ_NEXT != ulNCQ_INFO.B.ubNCQ_NEXT_RD) && (0 == gubReadDisturbStartPollingCMD)) {
					break;
				}
				gubReadDisturbStartPollingCMD = 0;
			}
#if ENABLE_READ_CHECK_FUNCTION
			else if (ENABLE_READ_DISTURB_TQ_ORDER && gubReadDisturbNeedWaitTQDone && guwForceRead_Info->uwQueuePushCnt && guwForceRead_Info->uwQueueQuota) {
				gubReadDisturbStartPollingCMD = 1;
				goto MARK_CMD_POLLING;
			}
#endif
			if (ulNCQ_INFO.B.ubNCQ_CMD_VLD_SUM && (ulNCQ_INFO.B.ubNCQ_NEXT_RD != 0x3F)) {
				if ((gubFirstCMDPending == 0) && (ulNCQ_INFO.B.ubNCQ_NEXT_RD != ulNCQ_INFO.B.ubNCQ_NEXT)) {
					if (gubNeedChkDoneTag) { //=0的話表示中間表示發生fail/Reset, 不需再考慮First Cmd Pending
						gubFirstCMDPending = 1;
						gulFirstCMDTimeSave = gulOperationTime;
					}
				}
				else if ((gubFirstCMDPending == 1) && ((gulOperationTime - gulFirstCMDTimeSave) > PENDING_CMD_THRESHOLD)) {
					ubStopRead = 1;
					break;
				}

				if ((gubLBAOverlap == 0) && (ulNCQ_INFO.B.ubCON_LBA_TAG != 0x3F)) {
					ubNCQTag = ulNCQ_INFO.B.ubCON_LBA_TAG;
				}
				else {//Read CMD NOT CON
					ulWaitCnt++;
					if (gubPreRead && (ulWaitCnt < 5)) {
						goto MARK_NEXT_READ_CMD;
					}
					ubNCQTag = ulNCQ_INFO.B.ubNCQ_NEXT_RD;
				}
				if ((gulNCQCMDDebug & ((ULLONG)BIT0 << ubNCQTag))) {
					break;
				}
#if (ENABLE_READ_DISTURB_LIMITED_HOST_READ && ENABLE_READ_CHECK_FUNCTION)
				if (((guwForceRead_Info->uwQueuePushCnt >= THRESHOLD_FORCE_READ)
				        && guwForceRead_Info->uwQueueQuota
				        /*&& gubForceReadMoveDoing*/
				        && gBQI.ubRLinkNum)) {
					if (ENABLE_LIMITED_HOST_READ_UART) {
						UartString("\nN_Force_P");
					}

					M_SwitchTask();
					break;
				}
#endif
				ulLBA = NLL[ubNCQTag];
				if ((ulLBA >= gulMediaSize.All) || (ubNCQTag == 0x3f)) {
					ulNCQ_INFO_FailCheck.All = HL[HL_RD_NCQ_INFO];
					if (ulNCQ_INFO_FailCheck.B.ubNCQ_CMD_VLD_SUM || (ulNCQ_INFO_FailCheck.B.ubNCQ_NEXT_RD != 0x3F) || gubNeedChkDoneTag) { //未被清掉
						flaLEDBlink(0x1246);
					}
					goto Discard_Redundant_NCQRCMD;
				}
				ulSectorCnt = NSL[ubNCQTag] & 0x0000FFFF;
				if (ulSectorCnt == 0) {
					ulSectorCnt = 65536;
				}
				uwEC = NEC[ubNCQTag];
				WR_NCQ_CMD_INFO[ubNCQTag].ulTrigTime = gulOperationTime;
				WR_NCQ_CMD_INFO[ubNCQTag].ulLBA = ulLBA;
				WR_NCQ_CMD_INFO[ubNCQTag].ulSectorCnt = ulSectorCnt;
				WR_NCQ_CMD_INFO[ubNCQTag].uwEC = uwEC;
				WR_NCQ_CMD_INFO[ubNCQTag].uwEC_BQDone = 0;
				if (uwEC > 0xFE) {
					uwEC = 0;
				}
				if (uwEC > 8) {
					ubEnableSwitchTask = 0;
#if  ENABLE_READ_CHECK_FUNCTION
					if (gulNCQRDCmdTriggerCnt) {
						if (DCCMSATAB[SATAB_RD_CNT]) {
							if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
								guwForceRead_Info->uwQueueQuota = 1;
							}
						}
					}
#endif
					DoneEC_Check(ubEnableSwitchTask);
					ubEnableSwitchTask = 1;
				}
				if (ENABLE_DEBUG_NCQ_UART_R) {
					UartLongHex(gulNCQRDCmdTriggerCnt);
					UartString("    R	Tag: ");
					Uart_Tx_DataHex(ubNCQTag);
					UartString("	   ");
					UartLongHex(ulLBA);
					UartString("	   ");
					UartLongHex(ulSectorCnt);
					if (ulNCQ_INFO.B.ubCON_LBA_TAG != 0x3F) {
						UartString("  C");
					}
				}
				gulNCQCMDCnt++;
				gulNCQCMDDebug |= ((U32)BIT0 << ubNCQTag);
			Mark_CHECK_TABLE_ALLZERO:
				if (gulReadZeroDataCnt > 128) {
					if ((gBQI.ubWLinkNum == 0) && (gubKeepWBQNum == 0)  && (gubKeepLoadAlignmentBQNum == 0)) {
						U32 ulL2PGroupIndex1ForZeroBM = (WR_NCQ_CMD_INFO[ubNCQTag].ulLBA / 8) >> (gubL2PEntrysPerL2PGroupLog - 2);
						U32 ulL2PGroupIndex2ForZeroBM = ((WR_NCQ_CMD_INFO[ubNCQTag].ulLBA + WR_NCQ_CMD_INFO[ubNCQTag].ulSectorCnt  - 1) / 8) >> (gubL2PEntrysPerL2PGroupLog - 2);
						UBYTE ubTableAllZero = 1;
						U32 ulL2PGroupIndexForZeroBM;
						for (ulL2PGroupIndexForZeroBM = ulL2PGroupIndex1ForZeroBM; ulL2PGroupIndexForZeroBM <= ulL2PGroupIndex2ForZeroBM; ulL2PGroupIndexForZeroBM++ ) {
							if ((gulTable_L2P[(ulL2PGroupIndexForZeroBM >> 2)].B.ubAllZero_BM & (BIT0 << (ulL2PGroupIndexForZeroBM & 3))) == 0) {
								ubTableAllZero = 0;
								break;
							}
						}

						if (ubTableAllZero) {
							if (gubNeedChkDoneTag == 0) {
								goto Discard_Redundant_NCQRCMD;
							}
							if (gubNeedWaitTQDone) {
								WR_NCQ_CMD_INFO[ubNCQTag].uwEC_BQDone = WR_NCQ_CMD_INFO[ubNCQTag].uwEC;
								gulNCQCMDDebug &= (~((U32)BIT0 << ubNCQTag));
								goto MARK_NEXT_READ_CMD;
							}
							HL[HL_TQ_CONTENT] = H_TQN_UPD_PTR | H_BUF_BASE2 | (ubNCQTag << 18) | (uwEC << 24) | H_SEND_ZERO_DATA;
							WR_NCQ_CMD_INFO[ubNCQTag].uwEC_BQDone = WR_NCQ_CMD_INFO[ubNCQTag].uwEC;
							gulNCQRDCmdTriggerCnt++;
							gulReadZeroDataCnt += WR_NCQ_CMD_INFO[ubNCQTag].uwEC;
							if (ENABLE_DEBUG_NCQ_UART_R) {
								UartString("  ZERO1\n");
							}
							if (gubPreRead) {
								ubZeroDataStopPreRead = 1;
								goto MarkStopPreRead;
							}
							else {
								goto MARK_NEXT_READ_CMD;
							}
						}
						else {
							UBYTE ubAllZero = 1;
							UWORD uw4Kindex;
							for (uw4Kindex = 0; uw4Kindex < WR_NCQ_CMD_INFO[ubNCQTag].uwEC; uw4Kindex++) {
								U32 ulVir4kIndex = ((WR_NCQ_CMD_INFO[ubNCQTag].ulLBA / 8) + uw4Kindex);
								UBYTE ubHit = 0;
								if (gubSkipDataInGRCheck == 0) {
									M_DataInGR_Check(ulVir4kIndex, ubHit);
								}
								if (ubHit) {
									ubAllZero = 0;
									break;
								}
								else {
									UWORD uwL2PGroupIndex = ulVir4kIndex >> gubL2PEntrysPerL2PGroupLog;
									UWORD uwInRAMIndex = guwL2P_InRamIndex[uwL2PGroupIndex];
									if ((uwInRAMIndex != TableNotInRAM) && gulL2P_InRamInverse[uwInRAMIndex].B.btValid && ((guwL2P_InRamIndex[uwL2PGroupIndex]&TableInTemp) == 0)) {//In RAM有找到
										U32 ulL2POffset = ((uwInRAMIndex & TableInTempMask) << gubL2PEntrysPerL2PGroupLog) + (ulVir4kIndex & guwL2PEntrysPerL2PGroupMask);
										if (gulL2PBase[ulL2POffset].All != gulTable_Invalid) {
											ubAllZero = 0;
											break;
										}
									}
									else {
										ubAllZero = 0;
										break;
									}
								}
							}
							if (ubAllZero) {
								if (gubNeedChkDoneTag == 0) {
									goto Discard_Redundant_NCQRCMD;
								}
								if (gubNeedWaitTQDone) {
									WR_NCQ_CMD_INFO[ubNCQTag].uwEC_BQDone = WR_NCQ_CMD_INFO[ubNCQTag].uwEC;
									gulNCQCMDDebug &= (~((U32)BIT0 << ubNCQTag));
									goto MARK_NEXT_READ_CMD;
								}
								HL[HL_TQ_CONTENT] = H_TQN_UPD_PTR  | H_BUF_BASE2 | (ubNCQTag << 18) | (uwEC << 24) | H_SEND_ZERO_DATA;
								WR_NCQ_CMD_INFO[ubNCQTag].uwEC_BQDone = WR_NCQ_CMD_INFO[ubNCQTag].uwEC;
								gulNCQRDCmdTriggerCnt++;
								gulReadZeroDataCnt += WR_NCQ_CMD_INFO[ubNCQTag].uwEC;
								if (ENABLE_DEBUG_NCQ_UART_R) {
									UartString("  ZERO2\n");
								}
								if (gubPreRead) {
									ubZeroDataStopPreRead = 1;
									goto MarkStopPreRead;
								}
								else {
									goto MARK_NEXT_READ_CMD;
								}
							}
							else {
								gulReadZeroDataCnt = 0;
							}
						}
					}
				}
				ubTrigTQ = 1;
			}
			else {
				break;

			}
		}
		else {
			break;
		}
		/*else if(ulNCQ_INFO.B.ubNCQ_CMD_VLD_SUM){
			break;
		}
		else{
			if(gubPreRead){
				ulWaitCnt++;
				if(ulWaitCnt<1000){
					goto MARK_NEXT_READ_CMD;
				}
				break;
			}
			break;
		}*/
	}


	if (ENABLE_DEBUG_NCQ_UART_R || ENABLE_LIMITED_HOST_READ_UART) {
		UartLongHex(gulNCQRDCmdTriggerCnt);
	}

	gubCmdDoneCount = 0;
	while (gulNCQRDCmdTriggerCnt) {
		if (gubNeedChkDoneTag) {
#if  ENABLE_READ_CHECK_FUNCTION
			if (gulNCQRDCmdTriggerCnt) {
				if (DCCMSATAB[SATAB_RD_CNT]) {
					if ((0 == guwForceRead_Info->uwQueueQuota) && (0 == gubForceReadMoveDoing)) {
						guwForceRead_Info->uwQueueQuota = 1;
					}
				}
			}
#endif
			DoneEC_Check(ubEnableSwitchTask);
			if (gubNCQDoing && (ubStopRead == 0)) {
				if (gubCmdDoneCount) {
					goto MARK_NEXT_READ_CMD;
				}
				else {
					if (gubPreRead && (gBQI.ubLinkNum < 8) && (gFQI.ubFQLinkNumber < 16)) {
						if ((!gubLimitPrereadCount) || (gubLimitPrereadCount && ((gulPreReadTriggerSectorCnt + (U32)gubSectorsPerPlane) <= BUFFER2_Read_NUM))) {
							/*
							* Limit precount to pass test case:iometer outstanding i/o:1, repeated bytes, all-in-1.
							* Preread over 256 sect will overlap buffer flag, which fail write BQ with load alignment.
							*/
							ubTrigTQ = 0;
							goto MARK_READ_CMD_LOOP;
						}
					}
				}
			}
		}
		else {
		Discard_Redundant_NCQRCMD:
			/* 有些driver遇到fail會下softReset*/
			M_SetFlagSettingNone(); //避免timeOut, 無法從R/W回到main作fail handling
			AXIL[AXIL_SET_START_FLAG] = 0;
			AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //因為關掉autoSet/autoCheck, 可能w的FQ做完後對應的flag沒清
			AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;

			// UartString("\r\n Waiting for EC DoneTag 5 but Error occurs");
			// ubNCQTag.B.uwReceived_EC表示做完的EC
			UBYTE ubclearCnt;
			for (ubclearCnt = 0; ubclearCnt < 32; ubclearCnt++) {
				WR_NCQ_CMD_INFO[ubclearCnt].uwEC_BQDone = WR_NCQ_CMD_INFO[ubclearCnt].uwEC;
			}
			//WR_NCQ_CMD_INFO[gulSATA_MSG_TYPE3[0].B.ubCurrentTag].uwEC_BQDone = WR_NCQ_CMD_INFO[gulSATA_MSG_TYPE3[0].B.ubCurrentTag].uwEC;


			gubSeqR = 0;
			guoFWRead4kBufferFlag = 0;
			//gubNCQDoing = 0;
			//flag 清成0的動作在ISR中已完成
			//M_SetFlagSettingNone();
			//AXIL[AXIL_SET_START_FLAG] = 0;
			//AXIL[AXIL_SET_FLAG_LENGTH] = gulBuffer2ReadNum;
			//AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
			while (gBQI.ubRLinkNum || gFQI.ubFQLinkNumber) {
				M_SwitchTask();
			}
			M_Clear_UNC_Addr();
			//M_SetFlagSettingSHF(); //ResetCmd中處理
			gubPreRead = 0;
			gulPreReadTriggerSectorCnt = 0;
			gulNCQRDCmdTriggerCnt = 0;
			gulNCQCMDDebug  = 0;

			//清掉Error或之前的DoneTag
			if (DCCMSATAB[SATAB_RD_CNT]) {
				DCCMSATAB[SATAB_RPIU] = DCCMSATAB[SATAB_RD_CNT];
			}

			//因為發生fail後會下APU_Reset, 所以要清掉對應的LastdoneTag, 避免塞BQ時拿錯
			gubLastDoneTag = 0x3F;
			gulSATA_MSG_TYPE3[0].All = 0;

		}
	}

	if (gubNCQDoing && (gubLBAOverlap == 0) && (ubStopRead == 0) && (gubNeedChkDoneTag != 0)) {
		U32 ulCount = 1000;
		while (ulCount > 3) {
			ulNCQ_INFO.All = HL[HL_RD_NCQ_INFO];
			if (ulNCQ_INFO.B.ubNCQ_CMD_VLD_SUM  && (ulNCQ_INFO.B.ubNCQ_NEXT_RD != 0x3F)) {
				goto MARK_NEXT_READ_CMD;
			}
			else if (ulNCQ_INFO.B.ubNCQ_CMD_VLD_SUM  && (ulNCQ_INFO.B.ubNCQ_NEXT_RD == 0x3F)) {
				break;
			}
			else if (gubHostCmdIn) {
				break;
			}
			M_SwitchTask();
			ulCount--;
		}
	}




	guwReadBufferPTR = uwBufferIndex;
	if (ENABLE_DEBUG_NCQ_UART_R) {
		UartString("\n");
	}
	if (gubNCQDoing) {
		if (gubPreRead) {
			while (gBQI.ubWLinkNum || gFQI.ubFQLinkNumber_W || gubLoadAlignDoing) {
				M_SwitchTask();
			}
			gubClearPreRead = 1;
			gubPreReadCheckCount = 0;
			guoFWRead4kBufferFlag = 0;
			M_SetFlagSettingNone();
			AXIL[AXIL_SET_START_FLAG] = 0;
			AXIL[AXIL_SET_FLAG_LENGTH] = gulBuffer2ReadNum;
			AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
			while (gBQI.ubRLinkNum || gFQI.ubFQLinkNumber) {
				M_SwitchTask();
			}
			M_Clear_UNC_Addr();
			gubPreRead = 0;
			gulPreReadTriggerSectorCnt = 0;
			mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 0);//強迫DMAC 全部做完
			M_SetFlagSettingSHF();
			gubClearPreRead = 0;
		}
	}
	gubSeqR = 0;
	gubFirstCMDPending = 0;
	gulSATA_MSG_TYPE3[0].All = 0;
	if (gubNeedChkDoneTag == 0) {
		while ((HB[HB_TQ_ACT]&H_TQ_ACT) || HB[HB_REMAIN_VLD_TQ_NUM]);
		idle(10);
		if (DCCMSATAB[SATAB_RD_CNT]) {
			DCCMSATAB[SATAB_RPIU] = DCCMSATAB[SATAB_RD_CNT];
		}
	}
	HL[HL_HAT_MISC] |= H_TQ_BY_ORDER;
#endif
}
void MoveLink(UBYTE ubMoveLink)
{
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();
	UBYTE ubTarget;
	UBYTE ubMoveLinkPrevious = gBQI.BufferQueue[ubMoveLink].ubPrevious;
	UBYTE ubMoveLinkNext = gBQI.BufferQueue[ubMoveLink].ubNext;
	UBYTE ubBeforeLoadAlign = 0;
	UBYTE ubPreKeep =  gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
	if ( (gBQI.BufferQueue[ubMoveLink].btWrite == 0) && gubKeepWBQNum && (gBQI.ubLinkNum > 1) && (gBQI.BufferQueue[ubPreKeep].btWrite && gBQI.BufferQueue[ubPreKeep].btNeedLoadAlign)) {
		if (ENABLE_KEEP_WBQ_WITHOUT_LOADALIGN_COLLECT) {
			ubTarget = ubPreKeep;
		}
		else {
			ubTarget = gubFirstLoadAlignmentBQLink;
		}
		ubBeforeLoadAlign = 1;
	}
	else {
		ubTarget = gubFirstKeepBQLink;
	}
	if (gBQI.BufferQueue[ubMoveLink].btWrite && gBQI.BufferQueue[ubMoveLink].btNeedLoadAlign) {
		UBYTE ubPreTarget =  gBQI.BufferQueue[ubTarget].ubPrevious;
		if ((ubTarget == gBQI.ubLinkFirst) || (ubTarget == ubMoveLinkNext) || (gBQI.BufferQueue[ubPreTarget].btWrite && (gBQI.BufferQueue[ubPreTarget].btNeedLoadAlign == 0))) {
			gubFirstLoadAlignmentBQLink = ubMoveLink;
			/*UartString("\nFirstL: ");
			Uart_Tx_DataHex(gubFirstLoadAlignmentBQLink);*/
		}
	}
	if (ubMoveLink == gBQI.ubLinkLast) {
		gBQI.ubLinkLast = ubMoveLinkPrevious;
	}
	if (ubTarget == gBQI.ubLinkFirst) {
		gBQI.ubLinkFirst = ubMoveLink;
	}

	gBQI.BufferQueue[ubMoveLinkPrevious].ubNext = ubMoveLinkNext;
	gBQI.BufferQueue[ubMoveLinkNext].ubPrevious = ubMoveLinkPrevious;

	gBQI.BufferQueue[gBQI.BufferQueue[ubTarget].ubPrevious].ubNext = ubMoveLink;
	gBQI.BufferQueue[ubMoveLink].ubPrevious = gBQI.BufferQueue[ubTarget].ubPrevious;
	gBQI.BufferQueue[ubTarget].ubPrevious = ubMoveLink;
	gBQI.BufferQueue[ubMoveLink].ubNext = ubTarget;
	if (ubMoveLink == gBQI.ubLinkLastUse) {
		gBQI.ubLinkLastUse = ubMoveLinkPrevious;
	}
	/*if(ubBeforeLoadAlign == 0){
		gBQI.ubLinkFTLLast = ubMoveLink;
	}
	else{*/
	gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
	//}
}
void FlushSDR(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();
	UWORD uwi;
	if (gubSDRFlushDoing == 0) {
		guwSDRFlushStart4kIndex = guwSDRFlushEnd4kIndex;
		guwFlush4kCount = 0;
	}
	guwSDRTotalFlush4kNum = guwDataInSDR4kNum + guwFlush4kCount;
	while (guwDataInSDR4kNum) {
		for (uwi = 0 ; (uwi < 16) && ((guwFlush4kCount + uwi) < guwSDRTotalFlush4kNum) ; uwi++) {
			UWORD uwFlushData4KIndex = (guwSDRFlushStart4kIndex + guwFlush4kCount + uwi) & SDRAM_CACHE_4KNUM_MASK;
			if (SDR_DataEntryTable[uwFlushData4KIndex].btDataValid) {
				if (SDR_DataEntryTable[uwFlushData4KIndex].ubMask != 0xFF) {
					BufferQueue_t *BQ;
					while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
					gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
					BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
					BQ->btWrite = 1;
					BQ->ulVir4kIndex = SDR_DataEntryTable[uwFlushData4KIndex].ulVir4kIndex;
					BQ->ulDataMask = ~(SDR_DataEntryTable[uwFlushData4KIndex].ubMask);
					BQ->uwBufferIndex = uwFlushData4KIndex;
					BQ->ubDataInDRamMap = 1;
					BQ->btNeedLoadAlign = 1;
					BQ->btSpecialData = 0;
					BQ->btNeedCheckSpecialData = 0;
					BQ->btPartialFromFlash = 0;
					BQ->ub4kNum = 1;
					BQ->btFlushWBQ = 0;
					BQ->ubRBQIIndex = 0xFF;
					BQ->ulSpareValid = 0;
					M_AddToBufferQueue(gBQI.ubLinkActive);
				}
			}
		}
		for ( uwi = 0 ; (uwi < 16) && (guwFlush4kCount < guwSDRTotalFlush4kNum) ; guwFlush4kCount++, uwi++) {
			UWORD uwFlushData4KIndex = (guwSDRFlushStart4kIndex + guwFlush4kCount) & SDRAM_CACHE_4KNUM_MASK;
			if (SDR_DataEntryTable[uwFlushData4KIndex].btDataValid) {
				BufferQueue_t *BQ;
				while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
				gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
				BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
				BQ->btWrite = 1;
				BQ->ulVir4kIndex = SDR_DataEntryTable[uwFlushData4KIndex].ulVir4kIndex;
				BQ->ulDataMask = SDR_DataEntryTable[uwFlushData4KIndex].ubMask;
				BQ->uwBufferIndex = uwFlushData4KIndex;
				BQ->ubDataInDRamMap = 1;
				BQ->btNeedLoadAlign = 0;
				BQ->btSpecialData = SDR_DataEntryTable[uwFlushData4KIndex].btSpecialData;
				BQ->btNeedCheckSpecialData = SDR_DataEntryTable[uwFlushData4KIndex].btSpecialData;
				BQ->ubZcode[0] = SDR_DataEntryTable[uwFlushData4KIndex].ubZCode;
				BQ->ulSectorCnt = SDR_DataEntryTable[uwFlushData4KIndex].ubZCode;
				BQ->ub4kNum = 1;
				BQ->btFlushWBQ = 0;
				BQ->ulSpareValid = 0;
				M_AddToBufferQueue(gBQI.ubLinkActive);
				gBQI.ubWLinkNum++;

				U32 ulVir4kIndex =  SDR_DataEntryTable[uwFlushData4KIndex].ulVir4kIndex;
				UWORD uwL1Entry = ulVir4kIndex >> gubDataCacheL2PL1TablePerEntry4kNumLog;
				UWORD uwL2Entry = ulVir4kIndex & gulDataCacheL2PTableEntryL2Mask;
				UWORD uwL2BlockIndex = guwDataCacheL2PTable_L1[uwL1Entry];
				if (guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry] == uwFlushData4KIndex) {
					guwDataCacheL2PTable_L2[(uwL2BlockIndex << gubDataCacheL2PTablePerBlockEntryNumLog) + uwL2Entry] = 0xFFFF;
					gubSDRL2PEntryCount[uwL2BlockIndex]--;
					if (gubSDRL2PEntryCount[uwL2BlockIndex] == 0) {
						guwFreeSDRL2PTableLink[guwSDRFreeLinkTail] = uwL2BlockIndex;
						guwSDRFreeLinkTail = uwL2BlockIndex;
						guwSDRFreeTableBlockNum++;
						guwDataCacheL2PTable_L1[uwL1Entry] = 0xFFFF;
					}
				}
			}
			guwDataInSDR4kNum--;
		}
	}
	guwSDRFlushEnd4kIndex = (guwSDRBufferPTR / 8);
	while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
		if (gBQI.ubLinkNum && ( !gBQI.BufferQueue[gBQI.ubLinkLastUse].btFlushWBQ)) {
			BufferQueue_t *BQ;
			while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
			gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
			BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
			BQ->btWrite = 1;
			BQ->btFlushWBQ = 1;
			BQ->ulSpareValid = 0;
			M_AddToBufferQueue(gBQI.ubLinkActive);
		}
		M_SwitchTask();
	}
	gubSDRFlushDoing = 0;
#endif
}
void CheckID(void)
{
	M_CheckBurnerModeToDisableFunction();
	U32 ulSecCnt;

	while (HW[HW_CMD_LBA_INFO]&H_C2L_ACT);

	gulLBA.All = (HL[HL_CMD_S_LBA]);

	if ((HW[HW_CMD_INFO] & H_NCQ_CMD) == 0) {

		//DEVSLP一醒來 HW 還沒準備好 Host 立刻 R/W, 正確的 LBA 還是會舉 H_LBA_ERR (因為 HL_MAX_LBA & HL_MAX_LBA_CHS = 0) -> 第一筆錯誤 CMD 多用 FW 卡一道
		if (gubFirstCMDIn && (HW[HW_CMD_INFO] & H_LBA_ERR)) {

			ulSecCnt = HL[HL_BUF_TRG_SEC];

			//0b: CHS address / 1b: LBA address
			if (HW[HW_CMD_LBA_INFO] & H_LBA_MODE) { //LBA address
				if (HB[HB_CMD_INFO_LOWBYTE] & H_LBA48_CMD) {  //48-bit
					if ((gulLBA.All >= gulMediaSize.All) || ((gulLBA.All + ulSecCnt) >= gulMediaSize.All)) {
						gubErrorCode |= ID_ERR;
					}
				}
				else { //28-bit
					if (gulMediaSize.All >= 0x0fffffff) {
						if ((gulLBA.All >= 0x0fffffff) || ((gulLBA.All + ulSecCnt) >= 0x0fffffff)) {
							gubErrorCode |= ID_ERR;
						}
					}
					else {
						if ((gulLBA.All >= gulMediaSize.All) || ((gulLBA.All + ulSecCnt) >= gulMediaSize.All)) {
							gubErrorCode |= ID_ERR;
						}
					}
				}
			}
			else { //CHS address
				if ((((HB[HB_LBA_H] << 8) + HB[HB_LBA_M]) >= guwCurCyl) // received Cylinder >= Maximum Cylinder
				        || (HB[HB_LBA_L] > gubCurSec) || (HB[HB_LBA_L] == 0) // received SecNum > Maximum Sec,or received SecNum is zero
				        || ((HB[HB_DEVICE] & 0x0F) >= gubCurHead)) {  // received Head >= Maximum Head
					gubErrorCode |= ID_ERR;
				}
				else if ((gulLBA.All >= gulCurrentSize.All) || ((gulLBA.All + ulSecCnt) >= gulCurrentSize.All)) {  // 正常情況下 gulCurrentSize.All = 0xFBFC10, 有做過 InitDrvPara 才會改變 -> gulCurrentSize.All <= 0xFBFC10
					gubErrorCode |= ID_ERR;
				}
			}

			gubFirstCMDIn = 0;

		}
		else {
			// LBA mode check out of range
			if (HW[HW_CMD_LBA_INFO]&LBA_Command_Out_of_Range) {
				if (HB[HB_CMD_INFO_LOWBYTE] & H_LBA48_CMD) {
					// 48 bit
					gubErrorCode |= ID_ERR;
				}
				else {
					// 28 bit
					if (gulMediaSize.All > 0x0fffffff) {
						// hardware issue : gulLBA.All+ ulSecCnt = 0x0fff_ffff return fail too
						ulSecCnt = HL[HL_BUF_TRG_SEC];
						if ((gulLBA.All + ulSecCnt) > 0x0fffffff) {
							gubErrorCode |= ID_ERR;
						}
					}
					else {
						gubErrorCode |= ID_ERR;
					}
				}
			}
			else if ((HW[HW_CMD_LBA_INFO]&CHS_Command_Out_of_Range) ) {
				gubErrorCode |= ID_ERR;
			}
		}
	}

}
void TrimAll()
{
#if ((!RDT)&&(!BURNER))
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	if (gubSSD_Init_State.B.btCleanRAMForWriteDone == 0) {
		ftlClearRAMForGR(gulStartClearGRAddress, 1);
	}

#if ENABLE_TRIM
#if ENABLE_NEW_TRIMALL
	//UBYTE ubFastPageBackup = VT->guwGRTarget[VT->gubGRTargetIndex].B.btFastPage;
	mSE_SV(4, (U32)gubTrimTable, (gulTotalL2PGroupCnt * guwTrimBytesPerL2PGroup) , 0, 0);// clear all trim
	gubTrimAll = 1;
	gubCleanGRInTrimAll = 1;
	while (gubTrimAll) {
		M_SwitchTask();
	}
	gubCleanGRInTrimAll = 0;
#endif
	gubFlushMode = BYTE_STANDBY_FLUSH;
	FlushCache();
	if (ENABLE_DEBUG_UART_TRIM) {
		UartString("\nTrimAll");
	}
#if ENABLE_NEW_TRIMALL
	UWORD uwUnitIndex;
	UBYTE ubSkip;
	UBYTE ubi;
	FTarget_t uwTarget;


	//1. create new table L2P  (keep EC & HOST DATA)
	//clear table L2P
	mSE_MSV(4, (U32)gulTable_L2P, (gulTotalL2PGroupCnt  * 4), (TableNotInFlash | TableAllZero | SearchLinkDefault), 0, 0xFFFFFFFF, 0, 1);

	//clear table P2L bitmap
	mDMAC_SV(4, (U32)gubTable_P2L_BitMap, 32 * 1024, 0, 0, 0, DMA_EN_SETCQ, 0);

	//claer table VC
	mSE_SV(2, (U32)guwTable_VC, MAX_TABLEUNIT * 2, 0, 0);

	//keep VC table & set the corresponding table VC

	gulTemp = (((U32)gulTable_L2P[gulTableIndex_VC].B.ubFUnitIndex) * guwTableP2LBitMapSize) + (gulTable_L2P[gulTableIndex_VC].B.uwFEntry >> gub4kEntrysPerPlaneLog);
	gubTable_P2L_BitMap[gulTemp / 8] |= (BIT0 << (gulTemp & 7));
	guwTable_VC[gulTable_L2P[gulTableIndex_VC].B.ubFUnitIndex]++;


	//keep EC table & set the corresponding table VC
	gulTemp = (((U32)gulTable_L2P[gulTableIndex_EC].B.ubFUnitIndex) * guwTableP2LBitMapSize) + (gulTable_L2P[gulTableIndex_EC].B.uwFEntry >> gub4kEntrysPerPlaneLog);
	gubTable_P2L_BitMap[gulTemp / 8] |= (BIT0 << (gulTemp & 7));
	guwTable_VC[gulTable_L2P[gulTableIndex_EC].B.ubFUnitIndex]++;

	//keep host table & set the corresponding table VC
	for (ubi = 0; ubi < gubHostTable_GroupNum; ubi++) {
		gulTemp = (((U32)gulTable_L2P[gulTableIndex_HOST + ubi].B.ubFUnitIndex) * guwTableP2LBitMapSize) + (gulTable_L2P[gulTableIndex_HOST + ubi].B.uwFEntry >> gub4kEntrysPerPlaneLog);
		gubTable_P2L_BitMap[gulTemp / 8] |= (BIT0 << (gulTemp & 7));
		guwTable_VC[gulTable_L2P[gulTableIndex_HOST + ubi].B.ubFUnitIndex]++;
	}


	//clear guwActiveTable_P2L
	for (uwUnitIndex = 0; uwUnitIndex < guwFastPagePlanesPerUnit; uwUnitIndex++) {
		if (guwActiveTable_P2L[uwUnitIndex] < 0xFF00)
			guwActiveTable_P2L[uwUnitIndex] = 0xFFFF;
	}


	ftlClearRAMForGR((U32)gulGRTable, 0);


	for (uwUnitIndex = VTAREA_UNITS; uwUnitIndex < guwFWTotalUnitNum; uwUnitIndex ++) {
		if ((gulVC[uwUnitIndex].B.ulValidCount != DefaultVC) && (uwUnitIndex != VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget)) {
			gulEC_D3[uwUnitIndex].B.btUsedDataUnit = 1;
		}
	}

	ftlCopyTable(VT->gubTableTargetIndex);

	mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 1);//強迫DMAC 全部做完
	gubSSD_Init_State.B.btCleanRAMForWriteDone = 1;
	gubSSD_Init_State.B.ubState = BYTE_INIT_TABLE_COPY_DONE;


	//clear inram info
	mDMAC_SV(2, (U32)guwL2P_InRamIndex, 120 * 1024, 0, 0x7FFF, 0, DMA_EN_SETCQ, 0);
#if !B0KB
	mDMAC_SV(4, (U32)gulL2P_InRamInverse, L2PInRamInverseSize - 0x8000, 0, NoL2PTableHere, 0, DMA_EN_SETCQ, 0);
#else
	mDMAC_SV(4, (U32)gulL2P_InRamInverse, L2PInRamInverseSize , 0, NoL2PTableHere, 0, DMA_EN_SETCQ, 0);
#endif


	//2.add freeQ base on VC table
	for (uwUnitIndex = VTAREA_UNITS; uwUnitIndex < guwFWTotalUnitNum; uwUnitIndex++) {
		if (gulVC[uwUnitIndex].B.ulValidCount == DefaultVC) {
			continue;
		}
		else {
			ubSkip = 0;
			for (ubi = 0; ubi < gubTableUnitNum; ubi ++) {
				if (VT->guwTableTarget[ubi].B.uwTarget == uwUnitIndex) {
					ubSkip = 1;
					break;
				}
			}
			if (ubSkip ||
			        (VT->guwRUTTarget.B.uwTarget == uwUnitIndex) ||
			        (VT->guwInitInfoTarget.B.uwTarget == uwUnitIndex) ||
			        (VT->guwVTChildTarget.B.uwTarget == uwUnitIndex)) {
				continue;
			}
		}
		gulVC[uwUnitIndex].B.ulValidCount = DefaultVC;
#if MLC
		gulVC[uwUnitIndex].B.btFastPage = 0;
#endif
		uwTarget.B.uwTarget = uwUnitIndex;
		VT->gulDebugInfo[0] = 32; //AddFreeQ Caller Num
		ftlAddToUserFreeTable(&uwTarget, D3);

#if TLC
		if (gubD1LinkTable[uwUnitIndex].B.ubD1Link != Default_D1_Unit) { //if D1 link
			uwTarget.B.uwTarget = gubD1LinkTable[uwUnitIndex].B.ubD1Link;
			ftlAddToUserFreeTable(&uwTarget, D1);//add D1 to free
			gubD1LinkTable[uwUnitIndex].B.ubD1Link = Default_D1_Unit; //kill D1 Link
		}
#endif

	}

	VT->gulTotalValidLCACnt = 0;

	// get new GR target
	VT->gubGRTargetIndex = 0;

	if (TLC && ENABLE_MULTI_DIE && guoFlashUseType.B.btMultiDie && gubParaInitialized && (gubSelDieNumber > 1)) {
		UBYTE ubDieMaxNumber = 0;
		UBYTE ubDieIndex;
		UBYTE ubGetMinECFreeBlock = 0;
		// 都拿D1 Die Count最多的
		// 下次flush 會 flush 減少的那個
		for (ubDieIndex = 1; ubDieIndex < gubSelDieNumber; ubDieIndex++) {

			if (VT->gubD1DieCount[ubDieIndex] <= FreeD1CountThatNeedWriteData + 1) {
				ubGetMinECFreeBlock = 1;
				break;
			}
			if (VT->gubD1DieCount[ubDieIndex] > VT->gubD1DieCount[ubDieMaxNumber]) {
				ubDieMaxNumber = ubDieIndex;
			}
		}
		if (ubGetMinECFreeBlock) {
			ftlGetUserFreeBlock(&VT->guwGRTarget[VT->gubGRTargetIndex], 0, D3);
		}
		else {
			if (ftlGetUserFreeBlockByDie(&VT->guwGRTarget[VT->gubGRTargetIndex], ubDieMaxNumber, D3) == 0) {
				ftlGetUserFreeBlock(&VT->guwGRTarget[VT->gubGRTargetIndex], 0, D3);
			}
		}
	}
	else {
		ftlGetUserFreeBlock(&VT->guwGRTarget[VT->gubGRTargetIndex], 0, D3);
	}

#if TLC
	UBYTE ubDieByD3;
	M_GetDieNumber(ubDieByD3, VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget, D3);

	if (ftlGetUserFreeBlockByDie(&uwTarget, ubDieByD3, D1) == 0) {
		M_AssertError(1);
	}

	if (ftlEraseTarget(&uwTarget, D1, TRUE)) {
		M_AssertError(1);
	}
#else
	ftlEraseTarget(&VT->guwGRTarget[VT->gubGRTargetIndex], D3, TRUE);
#endif

	//create D1 link
#if TLC
	gubD1LinkTable[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.ubD1Link = (UBYTE)uwTarget.B.uwTarget;
	gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.btInD1Unit = 1;
	gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.ulValidCount = 0;
	VT->guwGRTarget[VT->gubGRTargetIndex].B.btFastPage = 0;
#else
	gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.ulValidCount = 0;
	gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.btFastPage = 1;
	VT->guwGRTarget[VT->gubGRTargetIndex].B.btFastPage = 1;
#endif


	//3. SLC Pool Size
	VT->gulSLCPoolSize = VT->gulSLCPoolSizeMax;
	gulSLCPoolSize_Sync = VT->gulSLCPoolSize;

	//4.save VC table
	ftlSaveTable(&VT->guwTableTarget[VT->gubTableTargetIndex], (U32)gulVC, 0, BYTE_Save_VC_Mode);
	VT->gubTableTargetSyncIndex = VT->gubTableTargetIndex;
	VT->guwTableTargetSyncPTR = VT->guwTableTargetPTR;

	//5. save VT
	ftlSaveVT(0, VT_TRIM_ALL_DONE_0x95);
#elif(ENABLE_OLD_TRIM==1)

	mSE_SV(4, (U32)gubTrimTable, (gulTotalL2PGroupCnt * guwTrimBytesPerL2PGroup) , 0xFFFFFFFF, 0);
	if (ENABLE_DEBUG_TRIM_BIT) {
		gulRemainTrimbit = (gulTotalL2PGroupCnt * guwTrimBytesPerL2PGroup) * 8;
	}
	gulExcecute4kTrimNum = 0;
	gubTrimTableNotCleaned = 1;
	gubCleanGRInTrimAll = 1;
	while (gubTrimTableNotCleaned) {
		M_SwitchTask();
	}
	gubCleanGRInTrimAll = 0;
#else
	gubCalledWithHostCMD = 0;

	U32 ulTrimLBA = 0;
	U32 ulTrimSectorCnt = 0x1F0001;
	UBYTE ubCount = 0;

	/// Init Global Variable
	for (; ubCount < D_TRIM_TABLE_NUMBER; ubCount++) {
		gulTrimEnd4kIndex[ubCount] = 0;
		gulTrimStart4kIndex[ubCount] = 0;
	}
	ubCount = 0;


	while (ulTrimLBA < gulMediaSize.All) {

		/// deal with unaligned 4K start sectors.
		U32 ulTrimStart4kIndex = (ulTrimLBA >> 3);
		if ((ulTrimLBA & 7) != 0) {
			ulTrimStart4kIndex++;
			/// if there's no alignment of 4K sectors, use Zero() to deal with.
			gubNoAlignSecCntForTrim = 8 - (ulTrimLBA & 7) ;
			gulNoAlignStartLBAForTrim = (ulTrimLBA);
			Zero();
		}

		/// deal with unaligned 4K End sectors.
		U32 ulTrimEnd4kIndex = (((ulTrimLBA)) + ulTrimSectorCnt - 1);
		if ((ulTrimEnd4kIndex & 7) != 7) {
			/// if there's no alignment of 4K sectors, use Zero() to deal with.
			gubNoAlignSecCntForTrim = (ulTrimEnd4kIndex & 7) + 1;
			gulNoAlignStartLBAForTrim = (ulTrimEnd4kIndex & 0xFFFFFFF8 );
			Zero();
			ulTrimEnd4kIndex = ((ulTrimEnd4kIndex >> 3) - 1);
		}
		else {
			ulTrimEnd4kIndex = (ulTrimEnd4kIndex >> 3);
		}


		//// set
		gulTrimEnd4kIndex[ubCount] = ulTrimEnd4kIndex;
		gulTrimStart4kIndex[ubCount] = ulTrimStart4kIndex;
		ubCount++;

		if (ubCount == D_TRIM_TABLE_NUMBER) {
			gubWaitFTLtoSwapL2P = ubCount;
			while (gubWaitFTLtoSwapL2P) {
				M_SwitchTask();
			}
			ubCount = 0;
		}

		/// Add LBA

		ulTrimLBA += ulTrimSectorCnt;
		if ((gulMediaSize.All - ulTrimLBA) < ulTrimSectorCnt) {
			ulTrimSectorCnt = (gulMediaSize.All - ulTrimLBA);
		}
	}

	if (ubCount > 0) {
		gubWaitFTLtoSwapL2P = ubCount;
		while (gubWaitFTLtoSwapL2P) {
			M_SwitchTask();
		}
		ubCount = 0;
	}

	gubCalledWithHostCMD = 1;
#endif  // End of #if(ENABLE_OLD_TRIM==1)
#endif	// End of #if ENABLE_TRIM
#endif  // End of #if (!RDT)
}


