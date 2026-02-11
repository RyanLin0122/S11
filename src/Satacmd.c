#include "setup.h"
#include "Vardef.h"
#include "ftl.h"
#include "io.h"
#include <string.h>
#include <stdlib.h>

void ReturnErr(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	gubNeedSendCmdComplete = 1;
	HB[HB_ERROR] = gubErrorCode; // Report to error register
	HB[HB_STATUS] |= SET_ERR;    // Set Error bit in the Status Register
#endif
}

void NoOpCmd(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	gubErrorCode = SET_ABRT;
#endif
}

void ReCalibrate(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	HB[HB_SECTOR_CNT] = 0;
	HB[HB_LBA_M] = 0;
	HB[HB_LBA_H] = 0;
	HB[HB_DEVICE] &= 0xf0;

	if (HB[HB_DEVICE] & CHK_LBA) {
		HB[HB_LBA_L] = 0;
	}
	else {
		HB[HB_LBA_L] = 1;
	}
#endif
}

void SetMultiple (void)    // Default disable Read/Write Multiple (Hardware or SReset)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubSectorCnt = HB[HB_SECTOR_CNT];

	if (ENABLE_DEBUG_UART_ATA_CMD) {
		UartString("\r\nSetMultiple");
		UartString("SecCnt: ");
		Uart_Tx_DataHex(ubSectorCnt);
	}

	if (((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))
	        || (ubSectorCnt > MAXMULTICNT)) {

		gubErrorCode = SET_ABRT;
		return;
	}

	HandleStopRW(0);

	switch (ubSectorCnt) {
		case 0:
			IDT.W59 &= 0xFE00;
			HW[HW_SYS_CTRL] &= 0x8FFF; //clear bit12,13,14
			break;
		case 1:
			IDT.W59 &= 0xFE00;
			IDT.W59 |= SET_BIT8;
			IDT.W59 |= 0x0001;
			HW[HW_SYS_CTRL] &= 0x8FFF; //clear bit12,13,14
			break;
		case 2:
			IDT.W59 &= 0xFE00;
			IDT.W59 |= SET_BIT8;
			IDT.W59 |= 0x0002;
			HW[HW_SYS_CTRL] &= 0x8FFF; //clear bit12,13,14
			HW[HW_SYS_CTRL] |= H_MUL_PIO_DRQ_SIZE_1KB;
			break;
		case 4:
			IDT.W59 &= 0xFE00;
			IDT.W59 |= SET_BIT8;
			IDT.W59 |= 0x0004;
			HW[HW_SYS_CTRL] &= 0x8FFF; //clear bit12,13,14
			HW[HW_SYS_CTRL] |= H_MUL_PIO_DRQ_SIZE_2KB;
			break;
		case 8:
			IDT.W59 &= 0xFE00;
			IDT.W59 |= SET_BIT8;
			IDT.W59 |= 0x0008;
			HW[HW_SYS_CTRL] &= 0x8FFF; //clear bit12,13,14
			HW[HW_SYS_CTRL] |= H_MUL_PIO_DRQ_SIZE_4KB;
			break;
		case 16:
			IDT.W59 &= 0xFE00;
			IDT.W59 |= SET_BIT8;
			IDT.W59 |= 0x0010;
			HW[HW_SYS_CTRL] &= 0x8FFF; //clear bit12,13,14
			HW[HW_SYS_CTRL] |= H_MUL_PIO_DRQ_SIZE_8KB;
			break;
		default:
			gubErrorCode = SET_ABRT;
			break;
	}
#endif
}


void ReadMultiple(void)   // number of Sector Cnt defined by SetMulti command.
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (ENABLE_DEBUG_UART_ATA_CMD) {
		UartString("\r\nRead Multiple ");
		UartString("IDT59: ");
		UartWordHex(IDT.W59);
	}

	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || (gubLBA48BitCmdSupport == 0 && HB[HB_COMMAND] == 0x29)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	if ((IDT.W59 & BIT8) && (IDT.W59 & (0x00FF))) {
		ReadSectors();
	}
	else {
		NotImplement();
	}
#endif
}


void WriteMultiple(void)   // number of Sector Cnt defined by SetMulti command.
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (ENABLE_DEBUG_UART_ATA_CMD) {
		UartString("\r\nWrite Multiple ");
		UartString("IDT59: ");
		UartWordHex(IDT.W59);
	}

	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || ((IDT.W59 & BIT8) == 0)
	        || ((IDT.W59 & 0x00FF) == 0)
	        || (gubLBA48BitCmdSupport == 0 && HB[HB_COMMAND] == 0x39)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	WriteSectors();

#endif
}

void Diagnostics(void)   // Just Return 0x80 (Slave Error in True IDE Mode)  or  0x01 (No Error)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	HandleStopRW(0);

	HB[HB_ERROR] = 0x01;
	HB[HB_SECTOR_CNT] = 0x01;
	HB[HB_LBA_L] = 0x01;
	HB[HB_LBA_M] = 0x00;
	HB[HB_LBA_H] = 0x00;
	HB[HB_DEVICE] = 0x00;  // After add DRV_HD & ERR , Can Configure already.
	HB[HB_STATUS] = 0x50;
	HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
	while (HB[HB_REG_REQ]&H_REG_REQ);

	HB[HB_ERROR] = 0x00;
	gubNeedSendCmdComplete = 0;
	gubErrorCode = 0;
#endif
}

void InitDrvPara(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	HandleStopRW(0);

	UWORD uwSecPerCyl;
	unsigned char ubParameterChange = 0;
	UBYTE ubSectorCnt = HB[HB_SECTOR_CNT];
	if (ubSectorCnt == 0) {
		//        gubErrorCode=SET_ABRT;
		//       ReturnErr();
		ubSectorCnt++;
	}

	gubCurSec = ubSectorCnt;
	gubCurHead = (( HB[HB_DEVICE] & 0x0F) + 1);

	if ((gubCurSec == gubMaxSec) && (gubCurHead == gubMaxHead)) {
		//            ubParameterChange = 0;
		guwCurCyl = guwMaxCyl;
		gulCurrentSize.All = ((unsigned long)guwMaxCyl) * gubMaxHead * gubMaxSec;
	}
	else {
		ubParameterChange = 1;
	}

	if (ubParameterChange) {
		uwSecPerCyl = ((unsigned int)gubCurSec) * ((unsigned int)gubCurHead);
		//--- ATA4 page.20 ---//
		// Word 1 shall contain the number of user-addressable logical cylinders in the default CHS translation.  If
		// the content of words (61:60) is less than 16,514,064 then the content of word 1 shall be greater than or
		// equal to one and less than or equal to 65,535.  If the content of words (61:60) is greater than or equal to
		// 16,514,064 then the content of word 1 shall be equal to 16,383.

		// [(The content of word 1) * (the content of word 3) * (the content of word 6)] shall be less than or equal to 16,514,064.

		if (gulMediaSize.All > 16514064) {
			if (  (((unsigned long)16514064) / uwSecPerCyl) < 0x0000FFFF ) {
				guwCurCyl = ((unsigned long)16514064) / uwSecPerCyl;
			}
			else {
				guwCurCyl = 0xFFFF;
			}
		}
		else {
			if (  (gulMediaSize.All / uwSecPerCyl) < 0x0000FFFF ) {
				guwCurCyl = gulMediaSize.All / uwSecPerCyl;
			}
			else {
				guwCurCyl = 0xFFFF;
			}
		}

		/*gulCurrentSize.All = gulMediaSize.All/uwSecPerCyl;
		if (gulCurrentSize.All>65535) guwCurCyl = 65535;
		else guwCurCyl = gulCurrentSize.All;*/

		gulCurrentSize.All = ((unsigned long)guwCurCyl) * ((unsigned long)uwSecPerCyl);

	}
	HB[HB_MAX_HEAD] = gubCurHead;
	HB[HB_MAX_SECTOR] = gubCurSec;
	HW[HW_MAX_CYLINDER] = guwCurCyl;
	// Words (58:57) shall equal [(new content of word 54) . (new content of word 55) . (new content of word 56)] = Current capacity in sectors
	HL[HL_MAX_LBA_CHS] = gulCurrentSize.All;
#endif
}

void SetStandbyTimer (void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	gubStandyByTimerState = HB[HB_SECTOR_CNT];
	if (gubStandyByTimerState == 0) {}                                          // Standby time disabled
	else if (gubStandyByTimerState < 0xF1) {
		gulStandByJudgeInSecond = (U32)gubStandyByTimerState * 5;
	}
	else if (gubStandyByTimerState < 0xFC) {
		gulStandByJudgeInSecond = (U32)((gubStandyByTimerState - 240) * 30 * 60); //30~330 mins
	}
	else if (gubStandyByTimerState == 0xFC) {
		gulStandByJudgeInSecond = (U32)(21 * 60);                             // 21 mins
	}
	else if (gubStandyByTimerState == 0xFD) {
		gulStandByJudgeInSecond = (U32)(8 * 60 * 60);                        // 8~12 hours
	}
	else if (gubStandyByTimerState == 0xFE) {
		gubStandyByTimerState = 0;                                              // Reserved
	}
	else { // 0xFF
		gulStandByJudgeInSecond = (U32)((21 * 60) + 15);                      // 21 min 15 s
	}
#endif
}
void Standby(void)
{
#if(RDT)
	HandleStopRW(0);
#endif
	if ((HB[HB_FEATURE] == 0x00) && (HB[HB_SECTOR_CNT] == 0x6f) && (HB[HB_LBA_L] == 0xfe) && (HB[HB_LBA_M] == 0xef) && (HB[HB_LBA_H] == 0xfa)) {
		UBYTE ubEnableAPKey = 1;

		if (ubEnableAPKey) {
			gubAP_enable = 1;
			Sata_InterfaceReg_Init();
			HB[HB_ERROR] = NO_ERROR;
			//HB_LBA_Low=0x01;//inform host AP key enable success
			//HB_Sector_Count=0x01;//inform host AP key enable success
		}
		else {
			gubErrorCode = SET_ABRT;
		}
	}
	else {
#if(!RDT)
		U32 ulStartTime = gulOperationTime;
		M_CheckBurnerModeToDisableFunction();

		if (ENABLE_WRITEPROTECT) {
			if (gubIsWriteProtect) {
				gubErrorCode = SET_ABRT;
				ReturnErr();
				return;

			}
		}

		if (ENABLE_DEBUG_UART) {
			UartString("\nStandBy");
		}

		if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
			gubErrorCode = SET_ABRT;
			return;
		}

		HandleStopRW(0);

		if (gubCmdCode == 0xE2) {
			// STANDBY
			SetStandbyTimer();
		}
		if (gubCmdCode == 0xE0) {
			// STANDBY IMMEDIATE
			if (gubOfflineMode & SMART_M_TIMERENABLE) {
				SMART_SuspendOffLineDoing();
				SaveAtaConfig();
			}
		}

		//VT->gulFTLState.B.btLast_CMD_Not_StandBy = 0; //等到ftlBG_Flow Mark_Save_All_Info_For_PowerOff, 才更新VT
#if DEBUG_INSTEAD_STANDBY_BY_NORMAL_FLUSH==1
		gubFlushMode = BYTE_SATACMD_FLUSH;
#else
		gubFlushMode = BYTE_SATASTANDBY_FLUSH;
#endif
		FlushCache();
		gubAP_enable = 0;
		gubPowerMode = PWR_STANBY;
		//gubDisableWriteCacheCount = 10;
		if (ENABLE_DEBUG_UART) {
			UartString("\nStandByCost");
			U32 ulCostTime = gulOperationTime;
			if (ulCostTime < ulStartTime) {
				ulCostTime += (0xFFFFFFFF - ulStartTime);
			}
			else {
				ulCostTime -= ulStartTime;
			}
			UartLongHex(ulCostTime);
		}
#endif
	}
}

void Idle(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubIdleImmediateWithUnload;

	if ((gubCmdCode == 0xE1)
	        && ((HB[HB_FEATURE] == 0x44) && (HL[HL_LBA_L] == 0x554E4C) && ((HB[HB_DEVICE] & 0x0F) == 0x00))) {

		if (IDT.W84 & BIT13) {
			ubIdleImmediateWithUnload = 1;
		}
		else {
			gubErrorCode = SET_ABRT;
			return;
		}
	}
	else {
		ubIdleImmediateWithUnload = 0;
	}

	if (((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))
	        && (ubIdleImmediateWithUnload == 0)) {

		gubErrorCode = SET_ABRT;
		return;
	}

	HandleStopRW(0);

	if (gubCmdCode == 0xE3) {
		// Idle
		SetStandbyTimer();
	}
	if (gubCmdCode == 0xE1) {
		//Idle Immediate
		if (gubOfflineMode & SMART_M_TIMERENABLE) {
			SMART_SuspendOffLineDoing();
		}

		if (ubIdleImmediateWithUnload) {//idle immediate with unload feature command
			//If the device processes an IDLE IMMEDIATE command with UNLOAD (see 7.14) that returns command
			//completion without error, then the sanitize operation shall be suspended. The sanitize operation shall be
			//resumed after the processing of a software reset, a hardware reset, or any new command except IDLE
			//IMMEDIATE command with UNLOAD.
			AtaCfg.gSanitizeOverwrite.ubSuspend = 1;
			HB[HB_LBA_L] = 0xC4;//normal outputs (upload feature)
		}
	}
	gubPowerMode = PWR_IDLE;
#endif
}

void ChkPwrMode(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	HandleStopRW(0);

	if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
		gubErrorCode = SET_ABRT;
		return;
	}

	if (((gubPowerMode == PWR_ACTIVE) || (gubPowerMode == PWR_IDLE)) && (gubStandyByTimerState != 0)) {
		if ((GetRTT_milis() - guoCMDInStartTime ) > ((ULLONG)gulStandByJudgeInSecond * 1000 )) {
			gubPowerMode = PWR_STANBY;
		}
	}

	HB[HB_SECTOR_CNT] = gubPowerMode;
#endif
}

void Sleep(void)    // as suspend mode
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
		gubErrorCode = SET_ABRT;
		return;
	}

	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect) {
			gubErrorCode = SET_ABRT;
			return;
		}
	}

	HandleStopRW(0);
	if (gubOfflineMode & SMART_M_TIMERENABLE) {
		SMART_StopOffLineDoing();
		SaveAtaConfig();
	}

	gubPowerMode = PWR_SLEEP;
	//VT->gulFTLState.B.btLast_CMD_Not_StandBy = 0; //等到ftlBG_Flow Mark_Save_All_Info_For_PowerOff, 才更新VT
	gubFlushMode = BYTE_SATASTANDBY_FLUSH;
	FlushCache();

	gulSleepTimeInMinutes = GetRTT_m(); //先記錄開始 Sleep 的當下時間, 等到 Reset 醒來時再去計算總共 Sleep 經過時間

#if(!BURNER)
	gubIgnoreFlag = 1;
	gubAutoD2HBackup = (HW[HW_AUTO_FIS_CTRL] & 0x3F);
	M_DisableHWD2HSDB();
	M_DisableHWD2H_NCQ();
#endif
#endif
}

void AtaIdentifyDrive(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	HandleStopRW(0);
	gubNeedSendCmdComplete = 0;
	Load_AtaIdenDrv();  // Load ATA identify drive parameters [0x0000 - 0x00FF].
	RWBuffer(1);
#endif
#if(RDT)
	HandleStopRW(0);
	gubNeedSendCmdComplete = 0;
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)BUFL_BASE[0], 512 * 32);
	}
	Load_AtaIdenDrv();  // Load ATA identify drive parameters [0x0000 - 0x00FF].
	///////Lin
	//UBYTE *gubZ2Buffer = (UBYTE*)(TableZoneBase + 0x4AF000);
	//	flaInfoBlockSetting( (SYSTEM_GROUP_t *)(U32)(&gubZ2Buffer[M_SysInfo_InfoBlk]));
	//寫入serial number
	//flaInfoBlockSetting( (SYSTEM_GROUP_t *)(U32)((TableZoneBase + 0x4AF000 + 0x400 * 32)));
	///////Lin
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv(BUFB_BASE, 512);
	}
	RWBuffer(1);
#endif
}
void Zero(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	U32 ulPattern;
	BufferQueue_t *BQ;
	UBYTE ub4kIndex;
	UBYTE ub4kNum;
	U32 ubTotalSectors;
	UWORD uwBufferPTR;
	U32 ulBuffer;
	U32 ulDataMask;
	UWORD uwSecCnt;
	U32 ulStartLBA;
	U32 ulCurrentLBA;

	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || ((gubLBA48BitCmdSupport == 0) && (HB[HB_COMMAND] == 0x42))
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))
	        || (gubZeroSupport == 0)) {

		gubErrorCode = SET_ABRT;
		return;
	}
	if (gubIsWriteProtect) {
		gubErrorCode = SET_ABRT;
		return;
	}
	HandleStopRW(0);
	M_SetFlagSettingNone();

	ulPattern = 0;
	uwBufferPTR = 256;
	ulBuffer = SRAM_BASE + uwBufferPTR * 512;
#if(!BURNER)  // 1st of #if(!BURNER)
	if (gubCalledWithHostCMD) {
#endif  // End of 1st of #if(!BURNER)

		uwSecCnt = HW[HW_SECTOR_CNT];
		ulStartLBA = HL[HL_LBA_L];
		if (((ULLONG)ulStartLBA + uwSecCnt) > gulMediaSize.All) {
			gubErrorCode |= ID_ERR;
			return;
		}

#if(!BURNER) // 2nd of #if(!BURNER)
	}
	else {
		uwSecCnt = (gubNoAlignSecCntForTrim & 0x00FF) ;
		ulStartLBA = gulNoAlignStartLBAForTrim;
	}
#endif // End of 2nd of #if(!BURNER)

	ulCurrentLBA = ulStartLBA;


	while (uwSecCnt) {
		mDMAC_SV(4, ulBuffer, gub4kEntrysPerPlane * 4096, ((ulCurrentLBA >> gubSectorsPerPlaneLog) << gubSectorsPerPlaneLog), 0, 0, DMA_EN_SETCQ, 1);
		UWORD uwTriggerSectorCnt;
		UBYTE ubNeedLoadAlign = 0;
		if (ulCurrentLBA & SectorsPer4KMask) {   // not alignment
			uwTriggerSectorCnt = gubSectorsPerPlane - (ulCurrentLBA & gubSectorsPerPlaneMask);
			if (uwTriggerSectorCnt > uwSecCnt) {
				uwTriggerSectorCnt = uwSecCnt;
				ub4kNum = ((ulCurrentLBA + uwSecCnt) >> SectorsPer4KLog) - (ulCurrentLBA >> SectorsPer4KLog);
				if ((ulCurrentLBA + uwSecCnt)&SectorsPer4KMask) {
					ub4kNum++;
				}
			}
			else {
				ub4kNum = gub4kEntrysPerPlane - ((ulCurrentLBA & gubSectorsPerPlaneMask) >> SectorsPer4KLog);
			}
			ubNeedLoadAlign = 1;
		}
		else {  // alignment
			if (uwSecCnt < (gubSectorsPerPlane - (ulCurrentLBA & gubSectorsPerPlaneMask))) {
				uwTriggerSectorCnt = uwSecCnt;
				ub4kNum = uwTriggerSectorCnt >> SectorsPer4KLog;
				if (uwTriggerSectorCnt & SectorsPer4KMask) {
					ub4kNum++;
					ubNeedLoadAlign = 1;
				}
			}
			else {
				uwTriggerSectorCnt = gubSectorsPerPlane - (ulCurrentLBA & gubSectorsPerPlaneMask);
				ub4kNum = uwTriggerSectorCnt >> SectorsPer4KLog;
			}
		}
		if ((ulCurrentLBA + uwTriggerSectorCnt)&SectorsPer4KMask) {
			ulDataMask = gulMaskTable[ulCurrentLBA & SectorsPer4KMask] & (~gulMaskTable[(uwTriggerSectorCnt + (ulCurrentLBA & SectorsPer4KMask))&gubSectorsPerPlaneMask]);
		}
		else {
			ulDataMask = gulMaskTable[ulCurrentLBA & SectorsPer4KMask];
		}

		if (ubNeedLoadAlign) {
			while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
			gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
			BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
			BQ->btWrite = 1;
			BQ->ulVir4kIndex = ulCurrentLBA / 8;
			BQ->ubDataInDRamMap = 0;
			BQ->ulDataMask = ~ulDataMask;
			BQ->uwBufferIndex = uwBufferPTR;
			BQ->ubRBQIIndex = 0xFF;
			BQ->btPartialFromFlash = 0;
			BQ->btNeedLoadAlign = 1;
			BQ->btSpecialData = 0;
			BQ->btNeedCheckSpecialData = 0;
			BQ->btFlushWBQ = 0;
			BQ->ub4kNum = ub4kNum;
			BQ->ulSpareValid = 0;
			M_AddToBufferQueue(gBQI.ubLinkActive);
		}

		while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
		gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
		BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
		BQ->btWrite = 1;
		BQ->ulVir4kIndex = ulCurrentLBA / 8;
		BQ->ubDataInDRamMap = 0;
		BQ->ulDataMask = ulDataMask;
		BQ->uwBufferIndex = uwBufferPTR;

		ubTotalSectors = 0;
		for (ub4kIndex = 0; ub4kIndex < ub4kNum; ub4kIndex++) {
			if (ubNeedLoadAlign) {
				BQ->ubZcode[ub4kIndex] = 7;
			}
			else {
				UBYTE ubZcode = 1;
				if (gubLZSS_RESOLUTION == 1) { // 1K
					ubZcode = 1;
				}
				else if (gubLZSS_RESOLUTION == 2) { // 2K
					ubZcode = 3;
				}
				else if (gubLZSS_RESOLUTION == 3) { // 4K
					ubZcode = 7;
				}
				BQ->ubZcode[ub4kIndex] = ubZcode;
			}
			ubTotalSectors += BQ->ubZcode[ub4kIndex];
		}
		BQ->btNeedCheckSpecialData = 1;
		BQ->btSpecialData = 1;
		BQ->ulSectorCnt = ubTotalSectors;
		BQ->btNeedLoadAlign = 0;
		BQ->btFlushWBQ = 0;
		BQ->ub4kNum = ub4kNum;
		BQ->ulSpareValid = 0;
		BQ->ubThisBQisBG = 1;

		M_AddToBufferQueue(gBQI.ubLinkActive);
		gBQI.ubWLinkNum++;
		gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
		while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}
		gBQI.ulHostWriteWaitBufferFlag = 0;
		if (gubIsWriteProtect) {
			HL[HL_LBA_L] = ulCurrentLBA;
			gubErrorCode = SET_UNC;
			VT->gulNonDataUNCCnt++;

			if (ENABLE_DEBUG_UART_ATA_CMD) {
				UartString("\r\nUNC LBA: ");
				UartLongHex(ulCurrentLBA);
			}
			break;
		}

		uwSecCnt -= uwTriggerSectorCnt;
		ulCurrentLBA += uwTriggerSectorCnt;

	}

	M_SetFlagSettingSHF();

}

#ifdef _BURNER_MODE_

UWORD VerTable[4] = {
	('S' << 8) | 'B',  ('B' << 8) | FW_REVISION1,  (FW_REVISION2 << 8) | FW_REVISION3,  (FW_REVISION4 << 8) | FW_REVISION5,
};


UWORD SerialTable[10] = {
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
};

UWORD ModelTable[20] = {
	('S' << 8) | 'A',  ('T' << 8) | 'A',  ('B' << 8) | 'U',  ('R' << 8) | 'N', (' ' << 8) | ' ',
	(' ' << 8) | 'S',  ('B' << 8) | ' ',  (' ' << 8) | ' ',  (' ' << 8) | ' ', (' ' << 8) | ' ',
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
};

#elif RDT//RDT_MODE_
//#ifdef _RDT_MODE_
UWORD VerTable[4] = {
	('R' << 8) | 'D',  ('T' << 8) | FW_REVISION1,  (FW_REVISION2 << 8) | FW_REVISION3,  (FW_REVISION4 << 8) | FW_REVISION5,
};


UWORD SerialTable[10] = {
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
};

UWORD ModelTable[20] = {
	('S' << 8) | 'A',  ('T' << 8) | 'A',  ('F' << 8) | 'I',  ('R' << 8) | 'M', (' ' << 8) | ' ',
	(' ' << 8) | 'S',  ('9' << 8) | 'R',  ('D' << 8) | 'T',  ('T' << 8) | 'E', ('S' << 8) | 'T',
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
};

#else
UWORD VerTable[4] = {
	('S' << 8) | 'B',  ('F' << 8) | FW_REVISION1,  (FW_REVISION2 << 8) | FW_REVISION3,  (FW_REVISION4 << 8) | FW_REVISION5,
};


UWORD SerialTable[10] = {
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
};

UWORD ModelTable[20] = {
	('S' << 8) | 'A',  ('T' << 8) | 'A',  ('F' << 8) | 'I',  ('R' << 8) | 'M', (' ' << 8) | ' ',
	(' ' << 8) | 'S',  ('1' << 8) | '1',  (' ' << 8) | ' ',  (' ' << 8) | ' ', (' ' << 8) | ' ',
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
	0x2020, 0x2020, 0x2020, 0x2020, 0x2020,
};

UWORD WPVerTable[4] = {
	('S' << 8) | 'B',  ('F' << 8) | FW_REVISION1,  (FW_REVISION2 << 8) | FW_REVISION3,  ('W' << 8) | FW_REVISION5,
};

UWORD WPModelTable[10] = {
	('S' << 8) | 'A',  ('T' << 8) | 'A',  ('F' << 8) | 'I',  ('R' << 8) | 'M', (' ' << 8) | ' ',
	(' ' << 8) | 'S',  ('1' << 8) | '1',  (' ' << 8) | ' ',  (' ' << 8) | ' ', (' ' << 8) | ' ',
};

#endif
//#endif

void Load_AtaIdenDrv()
{
	//	M_CheckRDTModeToDisableFunction();
	if (ENABLE_DEBUG_UART_ATA_CMD) {
		UartString("\r\nIdentify Device");
	}

	UWORD uwi;
	UBYTE *ubTemp_BUF;
	UWORD *uwTemp_BUF;
	U32 *ulTemp_BUF;

	if ((guwErrorID & BIT15) && BURNER) {
		ubTemp_BUF = ( UBYTE*)BUFFER2_BASE;
		uwTemp_BUF = (UWORD *)BUFFER2_BASE;
		ulTemp_BUF = (U32 *)BUFFER2_BASE;
	}
	else {
		ubTemp_BUF = (UBYTE *)BUFFER3_BASE;
		uwTemp_BUF = (UWORD *)BUFFER3_BASE;
		ulTemp_BUF = (U32 *)BUFFER3_BASE;
	}

	memset((void *)ubTemp_BUF, 0x00, 512);
	//mSE_SV(8, (U32)ubTemp_BUF, 512, 0, 0);
#if(RDT)
	ubTemp_BUF[282] = gubstart_end_mark;

#endif
#if (RDT_BURNER)
	ubTemp_BUF[282] = 0xF0; //RDT endmark
#endif

	if (gubEnableCFastIdentifyDeviceWord0Request) {
		uwTemp_BUF[0] = 0x848A;
	}
	else {
		uwTemp_BUF[0] = 0x0040;
	}


	uwTemp_BUF[1] = guwMaxCyl;
	uwTemp_BUF[2] = 0xC837;

	uwTemp_BUF[3] = gubMaxHead;
	//BUF0W_BASE[4] = ((unsigned short)gubMaxSec << 9);
	//BUF0W_BASE[5] = 0x0200;
	uwTemp_BUF[6] = gubMaxSec;
	//BUF0W_BASE[7] = gulMediaSize.INTMODE.INT2;
	//BUF0W_BASE[8] = gulMediaSize.INTMODE.INT1;

	for (uwi = 0; uwi < 10; uwi++) {
		uwTemp_BUF[10 + uwi] = SerialTable[uwi]; //10-19:Serial number,
	}

	for (uwi = 0; uwi < 4; uwi++) {
		uwTemp_BUF[23 + uwi] = VerTable[uwi]; //23-26:Version,
	}

	for (uwi = 0; uwi < 20; uwi++) {
		uwTemp_BUF[27 + uwi] = ModelTable[uwi]; //27-46:Model number,
	}

#if (!BURNER)
	if (ENABLE_WRITEPROTECT && gubIsWriteProtect) {
		for (uwi = 0; uwi < 4; uwi++) {
			uwTemp_BUF[23 + uwi] = WPVerTable[uwi]; //23-26:Version,
		}

		for (uwi = 0; uwi < 10; uwi++) {
			uwTemp_BUF[27 + uwi] = WPModelTable[uwi]; //27-36:Model number,
		}

		for (uwi = 0; uwi < 10; uwi++) {
			uwTemp_BUF[37 + uwi] = 0x2020; //37-46:Model number,
		}
	}
#endif

	// bit 8 : DMA Supported(first disable for if define udma and mdma disable)
	uwTemp_BUF[47] = 0x8000 | MAXMULTICNT; //
	uwTemp_BUF[48] = 0x4000;
	uwTemp_BUF[49] = IDT.W49;

	uwTemp_BUF[50] = 0x4000; //
	uwTemp_BUF[53] = 0x0007; // 53 support MDMA setting
	uwTemp_BUF[54] = guwCurCyl;
	uwTemp_BUF[55] = gubCurHead;
	uwTemp_BUF[56] = gubCurSec;
	uwTemp_BUF[57] = gulCurrentSize.INTMODE.INT1;
	uwTemp_BUF[58] = gulCurrentSize.INTMODE.INT2;
	uwTemp_BUF[59] = IDT.W59;

	if (gulMediaSize.All >= 0x0fffffff) { // 2^28bit  ,
		ulTemp_BUF[60 >> 1] = 0x0fffffff;
	}
	else {
		ulTemp_BUF[60 >> 1] = gulMediaSize.All;
	}

	uwTemp_BUF[63] = IDT.W63;
	uwTemp_BUF[64] = 0x0003; // 64 set PIO mode 4
	uwTemp_BUF[65] = 0x0078; // 65 DMA transfer cycle time
	uwTemp_BUF[66] = 0x0078; // 66 DMA transfer cycle time

	uwTemp_BUF[67] = 0x0078; // 67 PIO transfer cycle time
	uwTemp_BUF[68] = 0x0078; // 68 PIO transfer cycle time
	uwTemp_BUF[69] = IDT.W69;

	if (gubNCQSupport) {
		if (IDT.W76 & CHK_BIT8) { //support NCQ
			uwTemp_BUF[75] = 32 - 1;
		}
	}


	uwTemp_BUF[76] = IDT.W76;

	IDT.W77 &= ~(BIT1 | BIT2);
	if ((HW[HW_SATA_INFO] & LNK_ESTSPD) == LNK_ESTSPD_3GB) { // 1: Sata Gen2 link now
		IDT.W77 |= SET_BIT2;
	}
	else if ((HW[HW_SATA_INFO] & LNK_ESTSPD) == LNK_ESTSPD_1_5GB) {
		IDT.W77 |= SET_BIT1;
	}
	else if ((HW[HW_SATA_INFO] & LNK_ESTSPD) == LNK_ESTSPD_6GB) {
		IDT.W77 |= (SET_BIT1 | SET_BIT2);
	}
	uwTemp_BUF[77] = IDT.W77;
	uwTemp_BUF[78] = IDT.W78;
	uwTemp_BUF[79] = IDT.W79;

	uwTemp_BUF[80] = 0x0FF8;	 // 80 support ATA version, supports ACS-4(BIT11)
	//BUFW_BASE[81] = 0x0110;	//Minor version number
	uwTemp_BUF[82] = IDT.W82;
	uwTemp_BUF[83] = IDT.W83;
	uwTemp_BUF[84] = IDT.W84;

	//Bit 14,13,12,10,4,3 of word 85 is a copy of bit of word 82.
	uwTemp_BUF[85] = (IDT.W85 & 0x8BE5) | (IDT.W82 & 0x7418) | (AtaCfg.ubSecMode & SET_SEC_EN);
	//Bit 13,12,11,10,6,2,0 of word 86 is a copy of bit of word 83.
	uwTemp_BUF[86] = ((IDT.W86 & 0xC3BA) | (IDT.W83 & 0x3C45));
	//Bit 13,8,6,5,1,0      of word 87 is a copy of bit of word 84.
	uwTemp_BUF[87] = ((IDT.W87 & 0xDE9C) | (IDT.W84 & 0x2163));
	uwTemp_BUF[88] = IDT.W88;


	if (APM_Support) {
		uwTemp_BUF[91] = IDT.W91;
	}

#if(!RDT)
	if (gubLBA48BitCmdSupport) {
		ulTemp_BUF[100 >> 1] = gulMediaSize.All;
	}
#endif

	uwTemp_BUF[106] = 0x4000;


	if (gubWWNSupport) {
		uwTemp_BUF[108] = guwWWN[0];
		uwTemp_BUF[109] = guwWWN[1];
		uwTemp_BUF[110] = guwWWN[2];
		uwTemp_BUF[111] = guwWWN[3];
	}

	uwTemp_BUF[119] = IDT.W119;//for Bit 14,4,2,0  119&120 is same.
	uwTemp_BUF[120] = IDT.W120;//Bit 4,3,2 of word 120 is a copy of bit of word 119.


	if (gubSecuritySupport) {
		uwTemp_BUF[89] = NORMAL_SECURITY_TIME;     // 89 Security Erase Time
		uwTemp_BUF[90] = ENHANCED_SECURITY_TIME;      // 90 Enhance Security Erase Time
		uwTemp_BUF[92] = AtaCfg.uwSecMasterID;
		uwTemp_BUF[128] = (AtaCfg.ubSecMode & 0x3f);
		//only security enable & cap is Max, report max
		if (AtaCfg.ubSecMode & SET_SEC_MAX) {
			uwTemp_BUF[128] |= CHK_SEC_CAP_MAX;
		}
	}


	if (gubTrimSupport) {
		//BUFW_BASE[69] = SET_BIT14;
		uwTemp_BUF[105] = MAXDSMCNT; // max entry sector
		uwTemp_BUF[169] = 0x0001; // support trim
	}

	if (gubIDW129_159Enable) {
		memcpy((void *)(&uwTemp_BUF[129]), (void*)(guwIDW129_159), sizeof(guwIDW129_159));
	}


	if (gubFormFactor == FORM_FACTOR_CFAST) {
		uwTemp_BUF[161] = IDT.W161;
		uwTemp_BUF[165] = IDT.W165;   // 代表最高溫跟最低溫
		uwTemp_BUF[167] = 0;
		//		uwTemp_BUF[167] = (0x6000 | BIT0);//Streaming Performance Control feature set is supported, and revision is 6.0
	}

	//Device Nominal Form Factor 2.5 inch 0x0003, less than 1.8 inch 0x0005
	uwTemp_BUF[168] = IDT.W168;
	uwTemp_BUF[209] = 0x4000;
	uwTemp_BUF[217] = 0x0001; // SSD setting
#if (!BURNER)
	if (gubSupportSATA) {
		uwTemp_BUF[222] = (0x1001 | (gubSupportSATA << 1));//Support SATA By InfoBlock
	}
	else
#endif
	{
		uwTemp_BUF[222] = 0x10FF;//SATA Rev 3.2
	}
	uwTemp_BUF[234] = DLMC_MINSECTORCNT;
	uwTemp_BUF[235] = DLMC_MAXSECTORCNT;


	if (gubDCOSupport) {
#if(!RDT)
		if (AtaCfg.uwDCOState & SET_DCO_SET) {
			uwTemp_BUF[63] &= AtaCfg.IDM_W63; // Cheney101217
			uwTemp_BUF[69] &= AtaCfg.IDM_W69;
			uwTemp_BUF[76] &= AtaCfg.IDM_W76;
			uwTemp_BUF[78] &= AtaCfg.IDM_W78;
			uwTemp_BUF[79] &= AtaCfg.IDM_W79;
			uwTemp_BUF[82] &= AtaCfg.IDM_W82_85;
			uwTemp_BUF[83] &= AtaCfg.IDM_W83_86;
			uwTemp_BUF[84] &= AtaCfg.IDM_W84_87;
			uwTemp_BUF[85] &= AtaCfg.IDM_W82_85;
			uwTemp_BUF[86] &= AtaCfg.IDM_W83_86;
			uwTemp_BUF[87] &= AtaCfg.IDM_W84_87;
			uwTemp_BUF[88] &= AtaCfg.IDM_W88; // Cheney101217
			uwTemp_BUF[89] &= AtaCfg.IDM_W_Security;
			uwTemp_BUF[90] &= AtaCfg.IDM_W_Security;
			uwTemp_BUF[92] &= AtaCfg.IDM_W_Security;
			uwTemp_BUF[128] &= AtaCfg.IDM_W_Security;
			uwTemp_BUF[105] &= AtaCfg.IDM_W_Trim;
			uwTemp_BUF[169] &= AtaCfg.IDM_W_Trim;
		}
#endif
	}


	uwTemp_BUF[255] = 0x00A5; //Signature

	if (!BURNER) {
		CalCheckSum();
	}

}

#if 0
{
	UWORD uwi;

	memset(BUFB_BASE, 0x00, 512);




	//if (InfoPageB[IPB_Seting0]&CHK_BIT1)
	//{
	//	  BUFW_BASE[0] = InfoPageW[IPW_Word0];
	// }
	// else
	{
		//			Word	OM	SP	FV
		//-----------------------------------------------------------------
		//	word	0	M	B				General configuration
		//						F	15		0 = ATA device
		//						X	14:8	Retired
		//						X	7:6 		Obsolete
		//						X	5:3 		Retired
		//						V	2		Response incomplete
		//						X	1		Retired
		//						F	0		Reserved
		BUFW_BASE[0] = 0x0040;
	}

	//			Word	OM	SP	FV
	//	word	1			X			Obsolete
	BUFW_BASE[1] = guwMaxCyl;

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			2	O	B	V			Specific configuration
	BUFW_BASE[2] = 0xC837;

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			3			X			Obsolete
	BUFW_BASE[3] = gubMaxHead;

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			4-5 			X			Retired
	//BUFW_BASE[4] = ((unsigned short)gubMaxSec << 9);
	//BUFW_BASE[5] = 0x0200;

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			6			X			Obsolete
	BUFW_BASE[6] = gubMaxSec;

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			7-8 						Reserved for assignment by the CompactFlash. Association
	//BUFW_BASE[7] = gulMediaSize.INTMODE.INT2;
	//BUFW_BASE[8] = gulMediaSize.INTMODE.INT1;

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			9			X			Retired

	for (uwi = 0; uwi < 10; uwi++) {
		//			Word	OM	SP	FV
		//-----------------------------------------------------------------
		//		   10-19	M	B	F			Serial number (ATA string)
		//BUFW_BASE[10 + uwi] = SerialTable[uwi]; //10-19:Serial number,
		BUFW_BASE[10 + uwi] = 0x2020;
	}

	for (uwi = 0; uwi < 6; uwi++) {
		//BUFW_BASE[10 + uwi] = (FWDateVerStr[uwi*2+1])|(FWDateVerStr[uwi*2]<<8);
	}
	//BUFW_BASE[15] |= 0x0020;	// Clear FW date last Byte

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//		   20-21			X			Retired
	//			22			X			Obsolete
	//		   23-26	M	B	F			Firmware revision (ATA string)

	//if (InfoPageB[IPB_Seting0]&CHK_BIT0)
	//{
	//	  memcpy(BUFB_BASE+46, ((UBYTE *)(INFO_BASE))+IPB_Word23, 8);
	// }
	//else
	/*{
		//memcpy(BUFB_BASE+46, (&Infotable[IPB_Word23>>1]), 8);
		for (uwi = 0; uwi < 4; uwi++)
		{
			  BUFW_BASE[23 + uwi] = (FWTimeVerStr[uwi*2+1])|(FWTimeVerStr[uwi*2]<<8);//0x2020;//VerTable[uwi]; //23-26:Version,
		}

	}*/
	UWORD VerTable[4] = {
		('S' << 8) | '9',  ('B' << 8) | FW_REVISION1,  (FW_REVISION2 << 8) | FW_REVISION3,  (FW_REVISION4 << 8) | FW_REVISION5,
	};
	for (uwi = 0; uwi < 4; uwi++) {
		BUFW_BASE[23 + uwi] = VerTable[uwi]; //23-26:Version,
	}


	for (uwi = 0; uwi < 20; uwi++) {
		//			Word	OM	SP	FV
		//-----------------------------------------------------------------
		//		   27-46	M	B	F			Model number (ATA string)
		BUFW_BASE[27 + uwi] = ModelTable[uwi]; //27-46:Model number,
	}

	//if (SATA.gubInfoblockvalid)//
	{
		// word 10-19, Serial number
		//	  memcpy(BUFB_BASE+20, ((UBYTE *)(INFO_BASE))+IPB_Snumber, 20);
		// word 27-46, Model number,
		//	  memcpy(BUFB_BASE+54, ((UBYTE *)(INFO_BASE))+IPB_Mnumber, 40);
	}

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			47	M					Multiple Count
	//						F	15-8	80h
	//						R	7-0 	00h = Reserved
	//									01h-10h = Maximum number of sectors that shall be
	//									transferred per interrupt on READ/WRITE MULTIPLE
	//									commands
	//									11h-FFh = Reserved
	BUFW_BASE[47] = 0x8000 | MAXMULTICNT; //

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			48	O	B				Trusted Computing feature set options
	//						F	15		Shall be cleared to zero
	//						F	14		Shall be set to one
	//							13:1	Reserved for the Trusted Computing Group
	//						F	0		1=Trusted Computing feature set is supported

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			49	M					Capabilities
	//							15:14	Reserved for the IDENTIFY PACKET DEVICE command.
	//					B	F	13		1 = Standby timer values as specified in this standard are supported
	//									0 = Standby timer values shall be managed by the device
	//							12		Reserved for the IDENTIFY PACKET DEVICE command.
	//						F	11		Shall be set to one
	//						F	10		Shall be set to one
	//					B	F	9		1 = LBA supported
	//					P	F	8		1 = DMA supported.
	//						X	7:0 		Retired
	BUFW_BASE[49] = SET_BIT8 | SET_BIT9 | SET_BIT11 | SET_BIT10; //

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			50	M					Capabilities
	//						F	15		Shall be cleared to zero.
	//						F	14		Shall be set to one.
	//						F	13:2	Reserved.
	//						X	1		Obsolete
	//					B	F	0		Shall be set
	BUFW_BASE[48] = 0x4000; //
	BUFW_BASE[50] = 0x4000; //

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//		   51-52			X			Obsolete

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			53	M					Field validity
	//						R	15-3	Reserved
	//						F	2		1=the fields reported in word 88 are valid
	//									0=the fields reported in word 88 are not valid
	//						F	1		1=the fields reported in words (70:64) are valid
	//									0=the fields reported in words (70:64) are not valid
	//						F	0		Obsolete
	BUFW_BASE[53] = 0x0007; // 53 support MDMA setting

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//		   54-58	X					Obsolete
	BUFW_BASE[54] = guwCurCyl;
	BUFW_BASE[55] = gubCurHead;
	BUFW_BASE[56] = gubCurSec;
	BUFW_BASE[57] = gulCurrentSize.INTMODE.INT1;
	BUFW_BASE[58] = gulCurrentSize.INTMODE.INT2;

	//if (IDT.W59)
	{
		//			Word	OM	SP	FV
		//-----------------------------------------------------------------
		//			59	M		F	15:9	Reserved
		//					B	V	8		1 = Multiple sector setting is valid
		//					B	V	7:0 		Current setting for number of logical sectors that shall be transferred per
		//									DRQ data block on READ/WRITE Multiple commands
		BUFW_BASE[59] = IDT.W59 | 0x0100;
	}

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//		   60-61	M	B	F			Total number of user addressable logical sectors (DWord)
	if (gulMediaSize.All >= 0x0fffFFff) { // 2^28bit  ,
		BUFL_BASE[60 >> 1] = 0x0fffFFff;
	}
	else {
		BUFL_BASE[60 >> 1] = gulMediaSize.All - 1;
	}


	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			63	M					Multiword DMA transfer
	//						F	15:11	Reserved
	//					P	V	10		1 = Multiword DMA mode 2 is selected
	//					P				0 = Multiword DMA mode 2 is not selected
	//					P	V	9		1 = Multiword DMA mode 1 is selected
	//					P				0 = Multiword DMA mode 1 is not selected
	//					P	V	8		1 = Multiword DMA mode 0 is selected
	//					P				0 = Multiword DMA mode 0 is not selected
	//						F	7:3 	Reserved
	//							2		1 = Multiword DMA mode 2 and below are supported
	//						F	1		1 = Multiword DMA mode 1 and below are supported
	//						F	0		1 = Multiword DMA mode 0 is supported
	BUFW_BASE[63] = IDT.W63 | 0x07; // Define DMA or PIO Mode Here !

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			64	M					PIO transfer modes supported
	//						F	15:8	Reserved
	//					P	F	7:2 		PIO modes supported
	//						F	1-0 	PIO modes 3 and 4 supported
	BUFW_BASE[64] = 0x0003; // 64 set PIO mode 4

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			65	M					Minimum Multiword DMA transfer cycle time per word
	//						F	15-0	Cycle time in nanoseconds
	BUFW_BASE[65] = 0x0078; // 65 DMA transfer cycle time

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			66	M					Manufacturer’s recommended Multiword DMA transfer cycle time
	//						F	15-0	Cycle time in nanoseconds
	BUFW_BASE[66] = 0x0078; // 66 DMA transfer cycle time

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			67	M					Minimum PIO transfer cycle time without flow control
	//						F	15-0	Cycle time in nanoseconds
	BUFW_BASE[67] = 0x0078; // 67 PIO transfer cycle time

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			68	M					Minimum PIO transfer cycle time with IORDY flow control
	//						F	15-0	Cycle time in nanoseconds
	BUFW_BASE[68] = 0x0078; // 68 PIO transfer cycle time

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			 69    Additional Supported (see 7.17.7.30)
	//O     S    F 15  1 = CFast Specification Support
	//O     B    F 14  1 = Deterministic data in trimmed LBA range(s) is supported
	//O     B    F 13  1 = Long Physical Sector Alignment Error Reporting Control is supported
	//O     B    F 12  1 = DEVICE CONFIGURATION IDENTIFY DMA and DEVICE CONFIGURATION SET DMA are supported
	//O     B    F 11  1 = READ BUFFER DMA is supported
	//O     B    F 10  1 = WRITE BUFFER DMA is supported
	//O     B    F 9  1 = SET MAX SET PASSWORD DMA and SET MAX UNLOCK DMA are supported
	//O     B    F 8  1 = DOWNLOAD MICROCODE DMA is supported
	//			   7  Reserved for IEEE 1667
	//M     B    F 6  0 = Optional ATA device 28-bit commands supported
	//			   5  1 = Trimmed LBA range(s) returning zeroed data is supported
	//			   4  1 = Device Encrypts All User Data
	//             3  1 = Extended Number of User Addressable Sectors is supported
	//		       2:0  Reserved

	BUFW_BASE[69] = (SET_BIT10 | SET_BIT11);


	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//		  71-74 						Reserved for the IDENTIFY PACKET DEVICE command.

#ifdef NCQ_Support
	//if (IDT.W76 & CHK_BIT8) //support NCQ
	{

		//			Word	OM	SP	FV
		//-----------------------------------------------------------------
		//			75	O					Queue depth
		//						R	15-5	Reserved
		//						F	4-0 		Maximum queue depth - 1
		BUFW_BASE[75] = 31;
	}
#endif

	//if (InfoPageB[IPB_FW_Support]==0x01)
	//{ // Toshiba
	// }
	//else
	{
		//			Word	OM	SP	FV
		//-----------------------------------------------------------------
		//			77	O					Serial ATA Additional capabilities
		//						R	15-7	Reserved
		//						F	6		Supports RECEIVE FPDMA QUEUED and SEND FPDMA QUEUED commands
		//						F	5		Supports NCQ Queue Management Command
		//						F	4		Supports NCQ Streaming
		//						V	3-1 		Coded value indicating current negotiated Serial ATA signal speed
		//						F	0		Shall be cleared to zero
		/*if ((HW[HW_SATA_INFO]&(CHK_BIT15|CHK_BIT14))==(CHK_BIT15|CHK_BIT14))
		{
			BUFW_BASE[77] = SET_BIT2|SET_BIT1;	//Speed is Gen 3
		}
		else if((HW[HW_SATA_INFO]&(CHK_BIT15|CHK_BIT14))==(CHK_BIT15))
		{
			BUFW_BASE[77] = SET_BIT2;
		}
		else
		{*/
		BUFW_BASE[77] = SET_BIT1;
		//}
	}

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			76	O					Serial ATA capabilities
	//						F	15		Supports READ LOG DMA EXT as equivalent to READ LOG EXT
	//						F	14		Supports Device Automatic Partial to Slumber transitions
	//						F	13		Supports Host Automatic Partial to Slumber transitions
	//						F	12		Supports Native Command Queuing priority information
	//						F	11		Supports Unload while NCQ commands outstanding
	//						F	10		Supports Phy event counters
	//						F	9		Supports receipt of host-initiated interface power management requests
	//						F	8		Supports Native Command Queuing
	//						R	7-4 		Reserved for future Serial ATA signaling speed grades
	//						F	3		1 = Supports Serial ATA Gen3 signaling speed (6.0 Gbps)
	//						F	2		1 = Supports Serial ATA Gen2 signaling speed (3.0 Gbps)
	//						F	1		1 = Supports Serial ATA Gen1 signaling speed (1.5 Gbps)
	//						F	0		Shall be cleared to zero
	BUFW_BASE[76] = IDT.W76 | SET_BIT1 | SET_BIT2;

#ifdef HIPM_Support
	BUFW_BASE[76] |= SET_BIT9;
#endif


	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			78	O					Serial ATA features supported
	//						R	15-8	Reserved
	//						F	7		1 = Supports NCQ Autosense
	//						F	6		1 = Supports software settings preservation
	//						F	5		1 = Hardware Feature Control is supported
	//						F	4		1 = Supports in-order data delivery
	//						F	3		1 = Device supports initiating interface power management
	//						F	2		1 = Supports DMA Setup Auto-Activate optimization
	//						F	1		1 = Supports non-zero buffer offsets in DMA Setup FIS
	//						F	0		Shall be cleared to zero
	//BUFW_BASE[78] = IDT.W78;
	BUFW_BASE[78] = SET_BIT2 | SET_BIT3 | IDT.W78;					//care

#if SSP_Support
	BUFW_BASE[78] = SET_BIT6; //SSP Support
#endif

#ifdef NCQ_Support
	//if (InfoPageB[IPB_ATA_Feature]&CHK_BIT0)
	{
		BUFW_BASE[76] |= SET_BIT8;
		BUFW_BASE[78] |= SET_BIT2; //Auto-Activate optimization Support
	}
#endif

#ifdef DIPM_Support
	//if (InfoPageB[IPB_ATA_Feature]&CHK_BIT6)
	//{
	BUFW_BASE[78] |= SET_BIT3;
	//}
#endif
	//*/


	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			79	O					Serial ATA features enabled
	//						R	15-8	Reserved
	//						V	7		1 = Device Automatic Partial to Slumber transitions enabled
	//						V	6		1 = Software settings preservation enabled
	//						V	5		1 = Hardware Feature Control is enabled
	//						V	4		1 = In-order data delivery enabled
	//						V	3		1 = Device initiating interface power management enabled
	//						V	2		1 = DMA Setup Auto-Activate optimization enabled
	//						V	1		1 = Non-zero buffer offsets in DMA Setup FIS enabled
	//						F	0		Shall be cleared to zero
	BUFW_BASE[79] = IDT.W79;

	//if (InfoPageB[IPB_FW_Support]==0x01)
	//{ // Toshiba
	//	  BUFW_BASE[80] = 0x01F8;	   // 80 support ATA version
	//}
	//else
	{
		//			Word	OM	SP	FV
		//-----------------------------------------------------------------
		//			80	M	B				Major version number
		//									0000h or FFFFh = device does not report version
		//						F			15:9 Reserved
		//					B	F	8		1 = supports ATA8-ACS
		//					B	F	7		1 = supports ATA/ATAPI-7
		//					B	F	6		1 = supports ATA/ATAPI-6
		//					B	F	5		1 = supports ATA/ATAPI-5
		//					B	F	4		1 = supports ATA/ATAPI-4
		//						X	3		Obsolete
		//						X	2		Obsolete
		//						X	1		Obsolete
		//						F	0		Reserved
		BUFW_BASE[80] = 0x03F8;	 // 80 support ATA version

		//			Word	OM	SP	FV
		//-----------------------------------------------------------------
		//			81	M	B	F			Minor version number
		BUFW_BASE[81] = 0x0110;
	}

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			82	M					Commands and feature sets supported
	//						X	15		Obsolete
	//					B	F	14		1 = NOP command supported
	//					B	F	13		1 = READ BUFFER command supported
	//					B	F	12		1 = WRITE BUFFER command supported
	//						X	11		Obsolete
	//					B	F	10		1 = Host Protected Area feature set supported
	//					N	F	9		1 = DEVICE RESET command supported
	//					B	F	8		1 = SERVICE interrupt supported
	//					B	F	7		1 = release interrupt supported
	//					B	F	6		1 = read look-ahead supported
	//					B	F	5		1 = volatile write cache supported
	//					B	F	4		Shall be cleared to zero to indicate that the PACKET feature set is not supported.
	//					B	F	3		1 = mandatory Power Management feature set supported
	//						X	2		Obsolete
	//					B	F	1		1 = Security feature set supported
	//					B	F	0		1 = SMART feature set supported
	BUFW_BASE[82] = SET_BIT6 | SET_BIT5 | SET_BIT12 | SET_BIT13; //IDT.W82;

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			83	M					Commands and feature sets supported
	//						F	15		Shall be cleared to zero
	//						F	14		Shall be set to one
	//					B	F	13		1 = FLUSH CACHE EXT command supported
	//					B	F	12		1 = mandatory FLUSH CACHE command supported
	//					B	F	11		1 = Device Configuration Overlay feature set supported
	//					B	F	10		1 = 48-bit Address feature set supported
	//					B	F	9		1 = Automatic Acoustic Management feature set supported
	//					B	F	8		1 = SET MAX security extension supported
	//							7		Reserved for technical report INCITS TR27-2001
	//					B	F	6		1 = SET FEATURES subcommand required to spin-up after power-up
	//					B	F	5		1 = Power-Up In Standby feature set supported
	//						X	4		Obsolete
	//					B	F	3		1 = Advanced Power Management feature set supported
	//					N	F	2		1 = CFA feature set supported
	//					B	F	1		1 = READ/WRITE DMA QUEUED supported
	//					B	F	0		1 = DOWNLOAD MICROCODE command supported
	BUFW_BASE[83] = SET_BIT12 | SET_BIT14; //IDT.W83;
#ifdef LBA48BIT_Support
	BUFW_BASE[83] |= (SET_BIT10 | SET_BIT13);
#endif
	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			84	M					Commands and feature sets supported
	//						F	15		Shall be cleared to zero
	//						F	14		Shall be set to one
	//					B	F	13		1 = IDLE IMMEDIATE with UNLOAD FEATURE supported
	//							12		Reserved for technical report INCITS TR37-2004
	//							11		Reserved for technical report INCITS TR37-2004
	//						X	10:9		1 = Obsolete
	//					B	F	8		1 = 64-bit World wide name supported
	//					B	F	7		1 = WRITE DMA QUEUED FUA EXT command supported
	//					B	F	6		1 = WRITE DMA FUA EXT and WRITE MULTIPLE FUA EXT commands supported
	//					B	F	5		1 = General Purpose Logging feature set supported
	//					B	F	4		1 = Streaming feature set supported
	//					N	F	3		1 = Media Card Pass Through Command feature set supported
	//					B	F	2		1 = Media serial number supported
	//					B	F	1		1 = SMART self-test supported
	//					B	F	0		1 = SMART error logging supported
	BUFW_BASE[84] = SET_BIT14 | SET_BIT5 | SET_BIT0 | SET_BIT1 | SET_BIT6 | SET_BIT8 | SET_BIT13; //IDT.W84;

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			85	M					Commands and feature sets supported or enabled
	//						X	15		Obsolete
	//					B	F	14		1 = NOP command supported
	//					B	F	13		1 = READ BUFFER command supported
	//					B	F	12		1 = WRITE BUFFER command supported
	//						X	11		Obsolete
	//					B	V	10		1 = Host Protected Area has been established
	//									(i.e., the maximum LBA is less than the maximum native LBA)
	//					N	F	9		1 = DEVICE RESET command supported
	//					B	V	8		1 = SERVICE interrupt enabled
	//					B	V	7		1 = release interrupt enabled
	//					B	V	6		1 = read look-ahead enabled
	//					B	V	5		1 = volatile write cache enabled
	//						F	4		Shall be cleared to zero to indicate that the PACKET feature set is not supported.
	//					B	F	3		Shall be set to one to indicate that the Mandatory Power Management feature set is supported.
	//						X	2		Obsolete
	//					B	V	1		1 = Security feature set enabled
	//					B	V	0		1 = SMART feature set enabled
	//Bit 14,13,12,10,4,3 of word 85 is a copy of bit of word 82.		delete?
	BUFW_BASE[85] = SET_BIT6 | SET_BIT5 | SET_BIT12 | SET_BIT13; //(IDT.W85 & 0x8BE5) | (IDT.W82 & 0x7418) | (AtaCfg.ubSecMode & SET_SEC_EN);

#if SMART_Support
	BUFW_BASE[82] |= SET_BIT0;
	BUFW_BASE[84] |= SET_BIT1 | SET_BIT0;
	//Bit 14,13,12,10,4,3 of word 85 is a copy of bit of word 82.
	// if(AtaCfg.uwDCOState & SET_DCO_SET){
	BUFW_BASE[85] = AtaCfg.ubSmartState;
	//	 }else{
	//IDT.W85 |= SET_BIT0;
	//		AtaCfg.ubSmartState = 1;
	//	 }
#endif

#if WrCache_Support
	BUFW_BASE[82] |= SET_BIT5;
	BUFW_BASE[85] |= SET_BIT5;//enable WR Cache
#endif

#if HPA_Support
	BUFW_BASE[82] |= SET_BIT10;
	BUFW_BASE[83] |= SET_BIT8;
#endif



	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			86	M					Commands and feature sets supported or enabled
	//					B	F	15		1 = Words 120:119 are valid
	//						F	14		Reserved
	//					B	F	13		1 = FLUSH CACHE EXT command supported
	//					B	F	12		1 = FLUSH CACHE command supported
	//					B	F	11		1 = Device Configuration Overlay supported
	//					B	F	10		1 = 48-bit Address features set supported
	//					B	V	9		1 = Automatic Acoustic Management feature set enabled
	//					B	V	8		1 = SET MAX security extension enabled by SET MAX SET PASSWORD
	//							7		Reserved for technical report INCITS TR27-2001
	//					B	F	6		1 = SET FEATURES subcommand required to spin-up after power-up
	//					B	V	5		1 = Power-Up In Standby feature set enabled
	//						X	4		Obsolete
	//					B	V	3		1 = Advanced Power Management feature set enabled
	//					N	F	2		1 = CFA feature set supported
	//					B	F	1		1 = READ/WRITE DMA QUEUED command supported
	//					B	F	0		1 = DOWNLOAD MICROCODE command supported
	//Bit 13,12,11,10,6,2,0 of word 86 is a copy of bit of word 83. delete?
	BUFW_BASE[86] = SET_BIT12;//((IDT.W86 & 0xC3BA) | (IDT.W83 & 0x3C45));
#ifdef LBA48BIT_Support
	BUFW_BASE[86] |= (SET_BIT10 | SET_BIT13);
#endif

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			87	M					Commands and feature sets supported or enabled
	//						F	15		Shall be cleared to zero
	//						F	14		Shall be set to one
	//					B	F	13		1 = IDLE IMMEDIATE with UNLOAD FEATURE supported
	//							12		Reserved for technical report INCITS TR37-2004
	//							11		Reserved for technical report INCITS TR37-2004
	//						X	10:9	Obsolete
	//					B	F	8		1 = 64 bit World wide name supported
	//					B	F	7		1 = WRITE DMA QUEUED FUA EXT command supported
	//					B	F	6		1 = WRITE DMA FUA EXT and WRITE MULTIPLE FUA EXT commands supported
	//					B	F	5		1 = General Purpose Logging feature set supported
	//						X	4		Obsolete
	//					N	V	3		1 = Media Card Pass Through Command feature set supported
	//					B	V	2		1 = Media serial number is valid
	//					B	F	1		1 = SMART self-test supported
	//					B	F	0		1 = SMART error logging supported
	//Bit 13,8,6,5,1,0		of word 87 is a copy of bit of word 84. 	delete?
	BUFW_BASE[87] =  SET_BIT14 | SET_BIT5 | SET_BIT0 | SET_BIT1 | SET_BIT8 | SET_BIT13; //((IDT.W87 & 0xDE9C) | (IDT.W84 & 0x2163));

	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			88	O					Ultra DMA modes
	//						F	15		Reserved
	//					P	V	14		1 = Ultra DMA mode 6 is selected
	//					P				0 = Ultra DMA mode 6 is not selected
	//					P	V	13		1 = Ultra DMA mode 5 is selected
	//					P				0 = Ultra DMA mode 5 is not selected
	//					P	V	12		1 = Ultra DMA mode 4 is selected
	//					P				0 = Ultra DMA mode 4 is not selected
	//					P	V	11		1 = Ultra DMA mode 3 is selected
	//					P				0 = Ultra DMA mode 3 is not selected
	//					P	V	10		1 = Ultra DMA mode 2 is selected
	//					P				0 = Ultra DMA mode 2 is not selected
	//					P	V	9		1 = Ultra DMA mode 1 is selected
	//					P				0 = Ultra DMA mode 1 is not selected
	//					P	V	8		1 = Ultra DMA mode 0 is selected
	//					P				0 = Ultra DMA mode 0 is not selected
	//						F	7		Reserved
	//					P	F	6		1 = Ultra DMA mode 6 and below are supported
	//						F	5		1=Ultra DMA mode 5 and below are supported
	//						F	4		1=Ultra DMA mode 4 and below are supported
	//						F	3		1=Ultra DMA mode 3 and below are supported
	//						F	2		1=Ultra DMA mode 2 and below are supported
	//						F	1		1=Ultra DMA mode 1 and below are supported
	//						F	0		1=Ultra DMA mode 0 is supported
	BUFW_BASE[88] = 0x007f | IDT.W88; //IDT.W88;



	//			Word	OM	SP	FV
	//-----------------------------------------------------------------
	//			93			V			COMRESET result. The contents of this word shall be cleared to zero.
	//			94	O					Current automatic acoustic management value
	//					B	F	15:8	Vendor's recommended acoustic management value.
	//					B	V	7:0 		Current automatic acoustic management value.
	//			95	O	B	F			Stream Minimum Request Size
	//			96	O	B	V			Streaming Transfer Time - DMA
	//			97	O	B	V			Streaming Access Latency - DMA and PIO
	//		   98-99	O	B	F			Streaming Performance Granularity (DWord)
	//		100-103 	O	B	V			Total Number of User Addressable Sectors for the 48-bit Address feature set (QWord)
	//			104 	O	B	V			Streaming Transfer Time - PIO
	//			105 			F			Reserved


	BUFW_BASE[235] = 0x0080; // download microcode max sector count






	BUFW_BASE[255] = 0x00A5; //Signature


	CalCheckSum();

}

#endif

void ReadBuffer(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();

	if ((((IDT.W69 & BIT11) == 0) && (HB[HB_COMMAND] == 0xE9))
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	HandleStopRW(0);

	if (HB[HB_COMMAND] == 0xE4) {
		gubNeedSendCmdComplete = 0;
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}

	RWBuffer(1);
#endif
}


void WriteBuffer(void)  // Debug Command Write Buffer Use only
{
	//#if(!RDT)
	//	M_CheckRDTModeToDisableFunction();
	M_DisableHWD2HSDB();

	if ((((IDT.W69 & BIT10) == 0) && (HB[HB_COMMAND] == 0xEB))
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	HandleStopRW(0);
	
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 1536);
	}

	RWBuffer(1);
	if (gubAP_enable) {
		mDMAC_XOR(BUFFER3_BASE, (BUFFER3_BASE + 0x200), (BUFFER3_BASE + 0x400), 0x200, 1, 0);
		U32 CRC = BUFL_BASE[(0x600 >> 2) - 1];
		BUFL_BASE[(0x600 >> 2) - 1] = 0;
		if (CRC != mDMAC_CRC((BUFFER3_BASE + 0x400), 0x200, 0, 1, (DMA_CRC_DCCM)) ) {
			gubAP_enable = 0;
		}	
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 1536);
		}
	}



	//#endif
}


void NotImplement(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	gubErrorCode = SET_ABRT;
	HB[HB_ERROR]  = SET_ABRT;
	HB[HB_STATUS] |= SET_ERR;
	HB[HB_LBA_L] = 1;
	HW[HW_SECTOR_CNT] = 1;
#endif
}

void Seek(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	while (HW[HW_CMD_LBA_INFO] & H_C2L_ACT);

	// only check start.
	if (HW[HW_CMD_LBA_INFO] & H_C2L_S_ERR) {
		gubErrorCode |= ID_ERR;
		return;
	}

	HandleStopRW(0);

	gulLBA.All = HL[HL_CMD_S_LBA];
	if (gulMediaSize.All > 0x0FFFFFFF) {
		if (gulLBA.All >= 0x0FFFFFFF) {
			gubErrorCode |= ID_ERR;
		}
	}
	else {
		if (gulLBA.All >= gulMediaSize.All) {
			gubErrorCode |= ID_ERR;
		}
	}
#endif
}

void SetFeature(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubSectorCnt = HB[HB_SECTOR_CNT];

	if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)
	        && (HB[HB_FEATURE] != 0x07)) { //shall abort other than (h) set features PUIS feature set device spin-up subcommand

		gubErrorCode = SET_ABRT;
		return;
	}

	HandleStopRW(0);
	if (gubIsWriteProtect && ENABLE_DEBUG_UART_WRITEPROTECT) {
		UartString("\n\rSetFea:");
		Uart_Tx_DataHex(HB[HB_FEATURE]);
	}
	switch (HB[HB_FEATURE]) {
		case ENWRCACHE: //0x02
			if (IDT.W82 & BIT5) {
				IDT.W85 |= SET_BIT5; //volatile write cache is enabled
				gubWriteCacheEnable = 1;
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

		case DISWRCACHE: //0x82
			if (IDT.W82 & BIT5) {
				IDT.W85 &= CLR_BIT5;
				if (ENABLE_UART_PFAIL_SAME_LBA) {
					UartString("\nDisableWC");
				}
				gubWriteCacheEnable = 0;
#if ((!TLC) && (!MicronFlashOnly))
				gubFillMLCGRTarget = 1;
				while (gubFillMLCGRTarget) {
					M_SwitchTask();
				}
#endif
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

		case APM_EN:   //  0x05
			if (IDT.W83 & BIT3) {
				if ((ubSectorCnt == 0x00) || (ubSectorCnt == 0xff)) {
					gubErrorCode = SET_ABRT;
				}
				else {
					if ( ubSectorCnt < 0x80) {
						// 1~0x7F
						gubPowerMode = PWR_STANBY;
					}
					IDT.W86 |= SET_BIT3;
					IDT.W91 = ubSectorCnt;// 0x00< gulSecCnt < 0xff
				}
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

		case APM_DIS:   //  0x85
			if (IDT.W83 & BIT3) {
				IDT.W86 &= CLR_BIT3;
				IDT.W91 = 0x0000;
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

			//By Working Draft INCITS Technical Report  T13  1825DT Revision 3  19 October 2005
			//4.2.2     Serial Device Behavior
			//If a Serial ATA device supports this feature, then the feature is always enabled and subcommand codes
			//5Fh and DFh are accepted by the device without changing its behavior.
			//IDENTIFY DEVICE word 120, bit 0 is not changeable on a Serial SATA device

		case ENRDLAHD: //0xAA
			IDT.W85 |= SET_BIT6;
			break;

		case DISRDLAHD: //0x55
			IDT.W85 &= CLR_BIT6;
			break;
		case ENACTPM: //0x09
			if ( (gubFormFactor == FORM_FACTOR_CFAST) && (ubSectorCnt > 0x10) && (ubSectorCnt < 0x29)) {
				gubACTPMValue = ubSectorCnt;
				gubACTPMEnable = 1;
				IDT.W161 |= SET_BIT12;
			}
			else	{
				gubErrorCode = SET_ABRT;
				gubACTPMEnable = 0;
				IDT.W161 &= CLR_BIT12;
			}
			break;
		case DISACTPM: //0x89
			gubACTPMEnable = 0;
			IDT.W161 &= CLR_BIT12;
			break;
		case EnableSATAFeature:  // SATA   0x10
		case DisableSATAFeature:  // SATA   0x90
			switch (ubSectorCnt) {
#if AutoActivate_Support
				case 0x02: //DMA Setup FIS Auto_Activate optimization
					if (gubNCQSupport && (IDT.W78 & BIT2)) {
						HB[HB_NCQ_SET] |= H_NCQ_SET_SEL; //Bit7舉起表示之後寫的值只對bit5bit6作用
						if (HB[HB_FEATURE] & CHK_BIT7) {
							HB[HB_NCQ_SET] &= H_DMA_SUP_A_DIS;
							IDT.W79 &= CLR_BIT2;

						}
						else {
							HB[HB_NCQ_SET] |= H_DMA_SUP_A;
							IDT.W79 |= SET_BIT2;
						}
					}
					else {
						gubErrorCode = SET_ABRT;
					}
					break;
#endif
				case 0x03:   // 0x03 DIPM
					if (IDT.W78 & BIT3) {
						if (AtaCfg.uwDCOState & SET_DCO_IPM) {
							gubErrorCode = SET_ABRT;
						}
						else if (HB[HB_FEATURE]  & CHK_BIT7) { /// Close it so need to CLR bits
							gubDevIPMON = 0;
							gubDIPMP2SEnable = 0;
							HW[HW_PM_CTRL] &=  ~(H_HW_AUTO_DIPM_EN);
							IDT.W79 &= CLR_BIT3;

							//							if (DIPM_Support && DIPM_APS) {
							//								HW[HW_AUTOP2S_THR] = 0x3E8;//                10 ms    ( S9 is 10 ms )
							//							}
							HW[HW_PM_CTRL] &= ~H_REG_APSREQ;
							IDT.W79 &= CLR_BIT7;

						}
						else {
							gubDevIPMON = 1;
							gubDIPMP2SEnable = 0;
							IDT.W79 |= SET_BIT3;

							/*======= TC7_ALL, Phase 1 : Counts to H_DIPM_AUTO_TMR_THR and enter P or S =======*/
							HW[HW_PM_CTRL] &= 0xFF00;					//1)  Clear all ACK & NAK,
							HW[HW_PM_CTRL] |= H_IPM_EN;			//	only set IPM
							HW[HW_PM_CTRL] &= ~(H_FORCE_IPM_NAK);	//2)  Clear IPM_NAK  ...sometimes it's set unexpectedly
							HW[HW_DIPM_TIMER_THR] = (0x80 | (gubDIPM_THRESHOLD & 0x7F));				//	 0x4E:                                  3 ms    ( S9 is 2 ms :0x56 )
							HB[HB_DIPM_AUTO_P2S_THR] = gubDIPM_P2S_THRESHOLD;		//                                20 ms  (partial wakeup, ask for wake up and slumber
							HW[HW_AUTOP2S_THR] = 0;				//4)  Clear 052h
							if (gubDEVSLPEnable == 1) {
								HB[HB_DIPM_AUTO_P2S_THR] = 0;
							}

							HW[HW_SATA_INFO] |= SET_BIT12;			//5)  Clear DIPM-denied state  //W1C
							HW[HW_PM_CTRL] |= H_HW_AUTO_DIPM_P;		//6)  Make HW send P request
							if (gubENABLE_HW_DIPM && gubDIPMSupport) {
								HW[HW_PM_CTRL] |=  (H_HW_AUTO_DIPM_EN);
							}
						}
					}
					else {
						gubErrorCode = SET_ABRT;
					}
					break;

				case 0x06:         // 0x06  (Software Setting Preservation)
					if (IDT.W78 & BIT6) {
						if (AtaCfg.uwDCOState & SET_DCO_SSP) {
							gubErrorCode = SET_ABRT;
						}
						else if (HB[HB_FEATURE]& CHK_BIT7) {
							IDT.W79 &= CLR_BIT6;
						}
						else {
							IDT.W79 |= SET_BIT6;
						}
					}
					else {
						gubErrorCode = SET_ABRT;
					}
					break;
				case 0x07:   // 0x07 Device Automatic Partial to Slumber transitions
					if (IDT.W76 & BIT14) {
						if (HB[HB_FEATURE] & CHK_BIT7) {
							//							if (DIPM_Support && DIPM_APS) {
							//								HW[HW_AUTOP2S_THR] = 0x3E8;
							//							}

							gubDIPMP2SEnable = 0;
							HW[HW_PM_CTRL] &= ~H_REG_APSREQ;
							HW[HW_AUTOP2S_THR] = 0;				// Clear 052h so that there's no AUTOP2S
							HB[HB_DIPM_AUTO_P2S_THR] = gubDIPM_P2S_THRESHOLD;		//                                20 ms  (partial wakeup, ask for wake up and slumber
							if (gubDEVSLPEnable == 1) {
								HB[HB_DIPM_AUTO_P2S_THR] = 0;
							}
							IDT.W79 &= CLR_BIT7;
						}
						else {
							if (!gubDevIPMON) {
								gubErrorCode = SET_ABRT;
							}
							else {
								//								if (DIPM_Support && DIPM_APS) {
								//									HW[HW_AUTOP2S_THR] = 0;
								//								}

								gubDIPMP2SEnable = 1;
								if (gubENABLE_HW_DIPM && gubDIPMSupport) {
									HW[HW_PM_CTRL] |=  (H_HW_AUTO_DIPM_EN);
								}
								IDT.W79 |= SET_BIT7;

								/*======= TC7_ALL, Phase 1 : Counts to H_DIPM_AUTO_TMR_THR and enter P or S =======*/
								HW[HW_PM_CTRL] &= 0xFF00;					//1)  Clear all ACK & NAK,
								HW[HW_PM_CTRL] |= H_IPM_EN;			//	only set IPM
								HW[HW_PM_CTRL] &= ~(H_FORCE_IPM_NAK);	//2)  Clear IPM_NAK  ...sometimes it's set unexpectedly

								HW[HW_DIPM_TIMER_THR] = (0x80 | (gubDIPM_THRESHOLD & 0x7F));			//3)  Set DIPM threshold,
								HB[HB_DIPM_AUTO_P2S_THR] = 0;
								HW[HW_AUTOP2S_THR] = guwDIPM_AUTOP2S_THRESHOLD;			//                10 ms    ( S9 is 10 ms )


								HW[HW_SATA_INFO] |= SET_BIT12;			//5)  Clear DIPM-denied state  //W1C
								HW[HW_PM_CTRL] |= H_HW_AUTO_DIPM_P;		//6)  Make HW send P request


								//								HW[HW_PM_CTRL] |= H_REG_APSREQ;
								IDT.W79 |= SET_BIT7;
							}
						}
					}
					else {
						gubErrorCode = SET_ABRT;
					}
					break;
				case 0x09:   // 0x09 DEVSLP
					if (IDT.W78 & BIT8) {
						if (((IDT.W78 & BIT12) == 0) && ((IDT.W77 & BIT8) == 0)) {
							if (HB[HB_FEATURE]  & CHK_BIT7) {
								//gubDEVSLP = 0;
								//SW_Interrupt_Enable &= ~(BIT3 | BIT4); //for DEVSLP interrupt
								//SB_Interrupt_Wake_Control &= ~(BIT4 | BIT5); //for DEVSLP wake up
								//SW_System_Control &= ~BIT4;
								//SW_System_Control_2 |= BIT4;
								//HW_System_Control &= ~Device_Sleep_Function_Enable;
								gubDEVSLPEnable = 0;
								HB[HB_DIPM_AUTO_P2S_THR] = gubDIPM_P2S_THRESHOLD;
								*((volatile unsigned short*)0x0400051C) &= 0xFEFF;
								IDT.W79 &= CLR_BIT8;
							}
							else {
								/*
								// Case of floating platform issue: 0=No Ignore, 1=All Ignore, 2=Smart Ignore
								if (gubFloatingPlatform_IgnoreDEVSLPMainFunction == 0) {
									//gubDEVSLP = 1;
									gubDEVSLP = 0;
									SB_Interrupt_Event |= (BIT3 | BIT4); // W1C
									SW_Interrupt_Enable |= (BIT3 | BIT4); //for DEVSLP interrupt
									SB_Interrupt_Wake_Control |= (BIT4 | BIT5); //for DEVSLP wake up
									SW_System_Control |= BIT4;
									SW_System_Control_2 &= ~BIT4;
									HW_System_Control |= (Device_Sleep_Function_Enable | Device_Sleep_to_Reduced_Power_State_Enable);
								}
								else {
									if (gubFloatingPlatform_IgnoreDEVSLPMainFunction == 2) {
										// ??
										SW_System_Control |= BIT4; // OSC enable
										SW_System_Control_2 &= ~BIT4; // After OSC starting, clear the mask to let devslp pin signal display on CPU
										for (gulTemp = 0; gulTemp < 16500; gulTemp++) {}; // delay 50 us

										if ((SL_Hardware_Setting & BIT24) == 0) {
											gubDEVSLP = 0;
											SB_Interrupt_Event |= (BIT3 | BIT4); // W1C
											SW_Interrupt_Enable |= (BIT3 | BIT4); //for DEVSLP interrupt
											SB_Interrupt_Wake_Control |= (BIT4 | BIT5); //for DEVSLP wake up
											HW_System_Control |= (Device_Sleep_Function_Enable | Device_Sleep_to_Reduced_Power_State_Enable);
										}
									}
								}
								*/


								if ((SYS0L[SYS0L_PAD_CTRL_REG0] & XDEVSLP_I)) { // high before set feature, so ignore the CMD and keep closed.
									//UartString("H bdfore DEVSLP sefF!");
									gubDEVSLPEnable = 0;
									HB[HB_DIPM_AUTO_P2S_THR] = gubDIPM_P2S_THRESHOLD;
									*((volatile unsigned short*)0x0400051C) &= 0xFEFF;
								}
								else {
									HB[HB_DIPM_AUTO_P2S_THR] = 0;
									gubDEVSLPEnable = 1;
									*((volatile unsigned short*)0x0400051C) |= 0x100;
									IDT.W79 |= SET_BIT8;
								}
							}
						}
						else {
							gubErrorCode = SET_ABRT;
						}
					}
					else {
						gubErrorCode = SET_ABRT;
					}
					break;

				case 0x0B://power disable feature enabled/disabled
					if (IDT.W78 & BIT12) {
						if (HB[HB_FEATURE]  & CHK_BIT7) {//disable
							if (IDT.W77 & BIT8) { //always enable bit
								IDT.W79 |= SET_BIT10;
							}
							else {
								IDT.W79 &= CLR_BIT10;
							}
						}
						else { //enable
							if ((IDT.W79 & BIT8) == 0) { //devslp enable bit
								IDT.W79 |= SET_BIT10;
							}
							else {
								gubErrorCode = SET_ABRT;
							}
						}
					}
					else {
						gubErrorCode = SET_ABRT;
					}
					break;

				default:
					gubErrorCode = SET_ABRT;
					break;
			}
			break;


		case SetXferMode:   // 0x03// 0x03 Table 53 ?Transfer modes
			switch (ubSectorCnt & 0xF8) {
				case 0x00:
					// (bit 0~2)
					// gubTemp 0 : PIO default mode
					//         1 : PIO default mode, disable IORDY
					// other     : not define (set Aborted)

					if ((ubSectorCnt & 0x07) > 0x01) {
						gubErrorCode = SET_ABRT;
					}
					break;

				case 0x08:
					// (bit 0~2)
					// gubTemp 0 : PIO mode 0
					//         1 : PIO mode 1
					//         2 : PIO mode 2
					//         3 : PIO mode 3
					//         4 : PIO mode 4
					// other     : not define (set Aborted)
					if ((ubSectorCnt & 0x07) > 0x04) {
						gubErrorCode = SET_ABRT;
					}
					break;

				case 0x20:
					// (bit 0~2)
					// gubTemp 0 : Multiword DMA mode 0
					//         1 : Multiword DMA mode 1
					//         2 : Multiword DMA mode 2
					// other     : not define (set Aborted)
					if (IDT.W49 & BIT8) {
						gXferMode.ubDMASelectMode[SEL_MDMA] = (ubSectorCnt & 0x07);
						gXferMode.ubDMASelectMode[SEL_UDMA] = 0;
						gXferMode.ubDefaultDMAType = SEL_MDMA;

						if (gXferMode.ubDMASelectMode[SEL_MDMA] <= gXferMode.ubDMASupportMode[SEL_MDMA]) {
							if ( (AtaCfg.uwDCOState & SET_DCO_MDMA) &&
							        ((AtaCfg.IDM_W63 & (BIT0 << gXferMode.ubDMASelectMode[SEL_MDMA])) == 0) ) {
								// Multiword DMA is not supported ?
								// this mode is not supported
								gubErrorCode = SET_ABRT;
							}
							else {
								// clear Multiword DMA modes selected flag
								IDT.W63 &= 0x00ff;
								// clear Ultra DMA modes selected flag
								IDT.W88 &= 0x00ff;
								// set this mode selected for Multiword DMA
								IDT.W63 |= (BIT8 << gXferMode.ubDMASelectMode[SEL_MDMA]);
							}
						}
						else {
							gubErrorCode = SET_ABRT;
						}
					}
					else {
						gubErrorCode = SET_ABRT;
					}
					break;
				case 0x40:
					// (bit 0~2)
					// gubTemp 0 : Ultra DMA mode 0
					//         1 : Ultra DMA mode 1
					//                              ...
					//         6 : Ultra DMA mode 6
					// other     : not define (set Aborted)
					if (IDT.W49 & BIT8) {
						gXferMode.ubDMASelectMode[SEL_UDMA] = (ubSectorCnt & 0x07);
						gXferMode.ubDMASelectMode[SEL_MDMA] = 0;
						gXferMode.ubDefaultDMAType = SEL_UDMA;

						if (gXferMode.ubDMASelectMode[SEL_UDMA]  <= gXferMode.ubDMASupportMode[SEL_UDMA]) {
							if ( (AtaCfg.uwDCOState & SET_DCO_UDMA) &&
							        ((AtaCfg.IDM_W88 & (BIT0 << gXferMode.ubDMASelectMode[SEL_UDMA])) == 0) ) {
								// Ultra DMA is not supported ?
								// this mode is not supported

								gubErrorCode = SET_ABRT;
							}
							else {
								// clear Multiword DMA modes selected flag
								IDT.W63 &= 0x00ff;
								// clear Ultra DMA modes selected flag
								IDT.W88 &= 0x00ff;
								// set this mode selected for Ultra DMA
								IDT.W88 |= (BIT8 << gXferMode.ubDMASelectMode[SEL_UDMA]);
							}
						}
						else {
							gubErrorCode = SET_ABRT;
						}
					}
					else {
						gubErrorCode = SET_ABRT;
					}
					break;

				default:
					gubErrorCode = SET_ABRT;
					break;
			}
			break;

		case EnRevert:     //  0xCC
			gubRTD_enable = 1;
			break;

		case DisRevert:    //  0x66 disable Reverting to Power-On Defaults
			gubRTD_enable = 0;
			break;

		default:
			gubErrorCode = SET_ABRT;
			break;
	}
	if (gubIsWriteProtect && ENABLE_DEBUG_UART_WRITEPROTECT) {
		UartString("  ErrCode:");
		Uart_Tx_DataHex(gubErrorCode);
	}
#endif
}
void RWBuffer(U32 ulXferCnt)
{
	//#if(!RDT)
	//	M_CheckRDTModeToDisableFunction();
	if (gubHardReset == 1) {
		return;
	}

	M_SetFlagSettingNone();

	gubNeedChkDoneTag = 1;
	U32 ulTemp_address, ulTemp_R_EP, ulTemp_W_EP, ulTemp_R_SHF, ulTemp_W_SHF;
	if ((guwErrorID & BIT15) && BURNER) {
		ulTemp_address = BUFFER2_BASE;
		ulTemp_R_EP = BUFFER2_READ_END;
		ulTemp_W_EP = BUFFER2_WRITE_END;
		ulTemp_R_SHF = BUFFER2_READ_START;
		ulTemp_W_SHF = BUFFER2_WRITE_START;
	}
	else {
		ulTemp_address = BUFFER3_BASE;
		ulTemp_R_EP = BUFFER3_READ_END;
		ulTemp_W_EP = BUFFER3_WRITE_END;
		ulTemp_R_SHF = BUFFER3_READ_START;
		ulTemp_W_SHF = BUFFER3_WRITE_START;
	}

	HL[HL_BUF_BASE] = ulTemp_address / 4096;
	HL[HL_BUF_R_EP] = ulTemp_R_EP;
	HL[HL_BUF_W_EP] = ulTemp_W_EP;
	HL[HL_BUF_R_SHF] = ulTemp_R_SHF;
	HL[HL_BUF_W_SHF] = ulTemp_W_SHF;
	M_AssertError(ulXferCnt > BUFFER3_NUM);

	HL[HL_BUF_OFST] = 0;
	HW[HW_SECTOR_CNT] = ulXferCnt;
	HL[HL_BUF_TRG_SEC] = ulXferCnt;
	//HL[HL_LAST_END_LBA] =0;
	HL[HL_TQ_CONTENT] = 0;
	// polling 0x24 表示沒有data傳輸中. 若是有下comreset, 會因為discard redundant放棄此次trigger, 便不會卡住 (也不需再一次APU_Reset)
	// 但是因為如果下reset的話, 後續的動作無須處理可以直接return, 所以這邊還是用0x28, 如果成立才須去看hardreset變數
	while (HL[HL_CMD_REMAIN_SEC]) {
		//若是先發生comreset, 會因為H_REG_XHOLD而無法進來, 所以當HardReset = 1時, 表示事先進來此function才收到reset, 顧無須再reset, 直接離開
		if (gubHardReset) {
			goto Restore_BufferSetting;
		}
	}

	M_SetFlagSettingSHF();
#if(!RDT)
	M_CheckDoneTag();
#endif

Restore_BufferSetting:
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

	gulSATA_MSG_TYPE3[0].All = 0;
	// >>> restore buffersetting...
	// >>> restore buffersetting...
	//#endif
}

void RWBuffer_forSMARTLog(U32 ulXferCnt, U32 ulOffset)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	M_SetFlagSettingNone();

	HL[HL_BUF_BASE] = BUFFER3_BASE / 4096;
	HL[HL_BUF_R_EP] = BUFFER3_READ_END;
	HL[HL_BUF_W_EP] = BUFFER3_WRITE_END;
	HL[HL_BUF_R_SHF] = BUFFER3_READ_START;
	HL[HL_BUF_W_SHF] = BUFFER3_WRITE_START;
	M_AssertError(ulXferCnt > BUFFER3_NUM);

	HL[HL_BUF_OFST] = ulOffset;
	//HW[HW_SECTOR_CNT] = ulXferCnt;
	HL[HL_BUF_TRG_SEC] = ulXferCnt;
	//HL[HL_LAST_END_LBA] =0;
	HL[HL_TQ_CONTENT_31] = 0;
	// polling 0x24 表示沒有data傳輸中. 若是有下comreset, 會因為discard redundant放棄此次trigger, 便不會卡住 (也不需再一次APU_Reset)
	// 但是因為如果下reset的話, 後續的動作無須處理可以直接return, 所以這邊還是用0x28, 如果成立才須去看hardreset變數
	while (HL[HL_CMD_REMAIN_SEC]) {
		//若是先發生comreset, 會因為H_REG_XHOLD而無法進來, 所以當HardReset or SoftReset = 1時, 表示事先進來此function才收到reset, 顧無須再reset, 直接離開
		if (gubHardReset || gubSoftReset) {
			break;
		}
	}

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

	if (gubHardReset == 0) {
		M_SetFlagSettingSHF();
	}
#endif
}

void SaveAtaConfig(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	memcpy(&BUFB_BASE[0], &AtaCfg, sizeof(ATACONFIG));
	Save_LogSector(HOSTTABLE_ATACFG_ADR);
#endif
}

void DLMC_ErrorDetect(UBYTE ubErrorCode)
{
	HB[HB_LBA_L] = ubErrorCode;
}

void Download_MicroCode(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UWORD uwSectorCnt;
	UWORD uwCmdSectorCnt;
	UBYTE ubCodeBlockIndex;
	UWORD uwInfoChecksum, uwInfoChecksumWithDummy;
	FlashQueue_t *FQ;
	//UBYTE ubNewSysCodeUnit[2];
	//UBYTE ubNewSysCodeBurst[2];
	//UWORD uwNewSysCodeBlock[2];

#if ENABLE_TEMP_MICROCODE_FORCE_TO_FLASH
	UBYTE ubi;
	UBYTE ubTmpCodeUnit, ubTmpCodeBurst;
#endif

	UBYTE ubNewSysCodeSet[2];
	UWORD uwNewSysCodeBlock[2][MAX_CHANNEL];
	UBYTE ubSysCodeUnit, ubSysCodeBurst;
	UBYTE ubNewSysCodeUnit, ubNewSysCodeBurst;
	UBYTE ubDeferredCodeUnit, ubDeferredCodeBurst;

	UBYTE ubDepth;
	UBYTE ubL4kLink;
	UBYTE ubCEIndex;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	L4KTable16B pL4KTable;
	UBYTE ubL4KCount = 0;
	volatile L4KTable16B *pL4KTablePtr;
	UWORD uwSectionPlaneIndex;
	U32 ulPlaneIndex;
	UBYTE ubSectionIndex;
	UWORD uwPlaneNum[8];
	UWORD uwBufferOffset;
	UBYTE ubMode;
	UBYTE ubCodeVersion;
	UWORD uwTotalTransferSectorCnt;
	//UBYTE *ubMicroCodeBuffer = (UBYTE *)gMicroCodeBuffer;
	//UWORD *uwMicroCodeBuffer =  (UWORD *)gMicroCodeBuffer;
	//U32 *ulMicroCodeBuffer = (U32 *)gMicroCodeBuffer;
	UBYTE *ubMicroCodeBuffer = (UBYTE *)gMicroCodeBuffer;
	UWORD *uwMicroCodeBuffer =  (UWORD *)gMicroCodeBuffer;
	U32 *ulMicroCodeBuffer = (U32 *)gMicroCodeBuffer;

	UBYTE ubFWSecNumber;
	UBYTE ubFUNCSecNumber;
	UWORD uwDivide16K_Quotient;
	UWORD uwDivide16K_Remainder;
	UWORD uwTransferSectorCnt = 0;
	UWORD uwSectorPTR;
	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];
	U32 ulMicroCodeSectionOffset;
	U32 ulFCONL_FLH_FUNC_Temp;
	U32 ulLDPC_CFG_Temp;

	UBYTE ubNeedDoEraseUnitFlag = 0;

	UBYTE ubNeedRemoveSet = 0xFF;
	UBYTE ubNotReceiveData = 0;
	/*UBYTE ubNeedRemoveUnit = 0xFF;
	UBYTE ubNeedRemoveBurst = 0xFF;*/

	UBYTE ubOffsetInSector = (UBYTE)(HL[HL_CMD_S_LBA] & 0x07);
	U32 ulStartTime = (U32)(GetRTT_s());
	U32 ulCurrentTime;
	U32 ulLastUpdateTime = 0;
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
	UBYTE ubMode0xE_Flag = 0;
	UBYTE ubMode0xF_Flag = 0;
#endif
	UBYTE ubLMU;

	if (ENABLE_DEBUG_UART_DLMC) {
		UartString("\r\nDLMC");
	}

	if (((IDT.W83 & BIT0) == 0)
	        || (((IDT.W69 & BIT8) == 0) && (HB[HB_COMMAND] == 0x93))
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		DLMC_ErrorDetect(1);
		return;
	}


	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect) {
			gubErrorCode = SET_ABRT;
			DLMC_ErrorDetect(32);
			return;
		}
	}

	ubMode = HB[HB_FEATURE];
	HB[HB_SECTOR_CNT_EXP] = HB[HB_LBA_L];
	if (ubMode == 0x0F) {
		uwSectorCnt = 0;  //0xF activate, Non-data cmd 強制設 SC=0 以免誤進 MicroCodeTrigger 導致 FW 卡住
	}
	else {
		uwSectorCnt = (UWORD)(HW[HW_SECTOR_CNT]);
	}
	uwCmdSectorCnt = uwSectorCnt;
	uwBufferOffset = (UWORD)(HB[HB_LBA_M] + (HB[HB_LBA_H] << 8) );

	if (HB[HB_COMMAND] == 0x93) {
		HB[HB_CMD_INFO_LOWBYTE] |= SET_BIT0; //dma command is received
	}

	if (ENABLE_DEBUG_UART_DLMC) {
		UartString(" ubMode: ");
		Uart_Tx_DataHex(ubMode);
		UartString(" uwCmdSectorCnt: ");
		UartWordHex(uwCmdSectorCnt);
		UartString(" uwBufferOffset: ");
		UartWordHex(uwBufferOffset);
	}

	uwDivide16K_Quotient = uwCmdSectorCnt / 32;
	uwDivide16K_Remainder = uwCmdSectorCnt % 32;

	if (((DLMC_Feature0x03_Support == 0) && (ubMode == 0x03))
	        || ((DLMC_Feature0x07_Support == 0) && (ubMode == 0x07))
	        || ((DLMC_Feature0x0E0x0F_Support == 0) && (ubMode == 0x0E || ubMode == 0x0F))
	        || ((uwCmdSectorCnt > DLMC_MAXSECTORCNT || uwCmdSectorCnt < DLMC_MINSECTORCNT) && (uwCmdSectorCnt != 0) && (ubMode != 0x0F))
	        || ((ubMode != 0x03) && (ubMode != 0x07) && (ubMode != 0x0E) && (ubMode != 0x0F))) {

		DLMC_InitialSetting();
		gubErrorCode = SET_ABRT;
		DLMC_ErrorDetect(2);

		return;
	}

	if ((uwCmdSectorCnt == 0) && (ubMode != 0x0F)) { //Non-Data transfer protocol shall be used, should not be considered as an error.
		return;
	}

	gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;	// Force clean BQ link, to avoid GC-BQ-DLMC deadlock.

	//HandleStopRW might trigger GC, so call this function before while, and wait GC completed
	HandleStopRW(0);

	while ((VT->gulFTLState.B.btFlushCache == 0) && (VT->gulFTLState.B.btNeedCloseTarget || VT->gulFTLState.B.btNeedWearLeveling || VT->gulFTLState.B.btNeedCloseTableTarget || ((gubLockReadMode != LOCK_READ_MODE_NOBODY) && (gubLockReadMode != LOCK_READ_MODE_DLMC)))) {
		if (ENABLE_DEBUG_UART_DLMC) {
			ulCurrentTime = (U32)(GetRTT_s()) - ulStartTime;
			if (ulCurrentTime > ulLastUpdateTime) {
				UartString("\r\nGC = ");
				Uart_Tx_DataHex(VT->gulFTLState.B.btNeedCloseTarget);
				UartString(" WL = ");
				Uart_Tx_DataHex(VT->gulFTLState.B.btNeedWearLeveling);
				UartString(" Flush = ");
				Uart_Tx_DataHex(VT->gulFTLState.B.btFlushCache);
				UartString(" RM = ");
				Uart_Tx_DataHex(gubLockReadMode);
				UartString(" time = 0x");
				UartLongHex(ulCurrentTime);

				ulLastUpdateTime = ulCurrentTime;
			}
		}

		M_SwitchTask();
	}

	HandleStopRW(1);//must be called after polling GC, or the FTL may hang up.

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN;//because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	uwTotalTransferSectorCnt = (gubHeaderTransferSectorCnt + gulBurnerTransferSectorCnt + gulMicroCodeTransferSectorCnt + gubUpdateSysInfoTransferSectorCnt);

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void*)&BUFB_BASE[0], 0x4000);
	}

Mark_Check_DLMC_Protocol:

	if (ubMode == 0x03) { //download segment untill all transfer done, then activate it
		if (gubLastDLMCMode == 0x03) {
			if (uwTotalTransferSectorCnt) {
				if (uwBufferOffset == 0) { //back to offset 0
					DLMC_InitialSetting();
				}
				else if (uwBufferOffset != uwTotalTransferSectorCnt) {
					gubDLMCStep = ERROR_DETECT;
					DLMC_ErrorDetect(3);
				}
				else { // transfer next segment
				}
			}
			else {
				DLMC_InitialSetting();
			}
		}
		else { //DL7:DL3
			if (uwBufferOffset != 0) {
				gubDLMCStep = ERROR_DETECT;
				DLMC_ErrorDetect(4);
			}
			else {
				DLMC_InitialSetting();
			}
		}

	}
	else if (ubMode == 0x07) { //download all, and activate it
		if (uwBufferOffset != 0) {
			gubDLMCStep = ERROR_DETECT;
			DLMC_ErrorDetect(5);
		}
		else {
			DLMC_InitialSetting();
		}
	}
	else if (ubMode == 0x0E) { //download segment, no activate
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
		ubMode0xE_Flag = 1;
#endif
		if (gubLastDLMCMode == 0x0E) {
			if (uwTotalTransferSectorCnt) {
				if (uwBufferOffset == 0) { //back to offset 0
					DLMC_InitialSetting();
				}
				else if (uwBufferOffset != uwTotalTransferSectorCnt) {
					gubDLMCStep = ERROR_DETECT;
					DLMC_ErrorDetect(6);
				}
				else { // transfer next segment
				}
			}
			else {
				DLMC_InitialSetting();
			}
		}
		else { //DL4:DL6
			if (uwBufferOffset != 0) {
				gubDLMCStep = ERROR_DETECT;
				DLMC_ErrorDetect(7);
			}
			else {
				DLMC_InitialSetting();
			}
		}
	}
	else if (ubMode == 0x0F) { //activate 0x0E download
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
		ubMode0xF_Flag = 1;
#endif
		if (((gubDLMCStep >= HEADER_TRANSFERDONE) && (gubDLMCStep < SAVE_INFOBLOCK_DONE)) || ((gubDeferredCodeSet[0] == 0xFF) && (gubDeferredCodeSet[1] == 0xFF))) {
			//if ((gubDeferredCodeSet[0] == 0xFF) && (gubDeferredCodeSet[1] == 0xFF)) { //millar: S10 do so.
			//a.has updated microcode data that has not been saved.
			//b.does not have deffered microcode data. or
			//c.is unable to activate the deffered microcode data.
			gubDLMCStep = ERROR_DETECT;
			DLMC_ErrorDetect(8);


			for (ubCodeBlockIndex = 0; ubCodeBlockIndex < 2; ubCodeBlockIndex++) {
				if (gubDeferredCodeSet[ubCodeBlockIndex] != 0xFF) {
					for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
						M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
						FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
						FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
						FQ->btD3 = 1;
#else
						FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif

						// Must Send Reset CMD(0xFF) After Erase Fail
						FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

						ubDeferredCodeUnit = (UBYTE)(guwSystemBlockRemappingTable[gubDeferredCodeSet[ubCodeBlockIndex]][ubCEIndex] >> gubBurstsPerBankLog);
						ubDeferredCodeBurst = (UBYTE)(guwSystemBlockRemappingTable[gubDeferredCodeSet[ubCodeBlockIndex]][ubCEIndex] & gubBurstsPerBankMask);

						FQ->uwFUnit = ubDeferredCodeUnit;
						if (ENABLE_PLANE_CH_CE) {
							FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubDeferredCodeBurst) << gub4kEntrysPerPlaneLog);
						}
						else {
							FQ->ulFEntry = ((ubCEIndex + (ubDeferredCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
						}
						FQ->ubRelativeDepth = ubDepth;
						FQ->ubDepth = 0xFF;
						FQ->btUNServed = 1;
						FQ->btCheckEmpty = 0;

						gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
						gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
						gFQI.ubFQLinkNumber++;
						gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

						while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
							M_SwitchTask();
						}
						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
					}

					ftlAddToSystemSetFreeTable(gubDeferredCodeSet[ubCodeBlockIndex]);
				}
			}

			gubDeferredCodeSet[0] = 0xFF;
			gubDeferredCodeSet[1] = 0xFF;
		}
		else {
			gubDLMCStep = GOING_TO_ACTIVATE_MICROCODE; //ready to activate
		}
	}
	else {
		gubDLMCStep = ERROR_DETECT;
		DLMC_ErrorDetect(9);

	}

	if ((gubLockReadMode != LOCK_READ_MODE_DLMC) && ((gubDLMCStep >= MICROCODE_16K_HEADER_TRANSFERDONE) && (gubDLMCStep < SAVE_MICROCODE_DONE))) {
#if ENABLE_TEMP_MICROCODE_FORCE_TO_FLASH

		if (gubMicroCodeTmpSet != 0xFF) {
			ftlSwapL2PTable(GC_LOCK_L2P_FOR_DLMC, (BYTE_Lock_Read_Mode | LOCK_READ_MODE_DLMC));

			ubTmpCodeUnit = guwSystemBlockRemappingTable[gubMicroCodeTmpSet][0] >> gubBurstsPerBankLog;
			ubTmpCodeBurst = guwSystemBlockRemappingTable[gubMicroCodeTmpSet][0] & gubBurstsPerBankMask;

			//Read 896K from Flash, each 16KB, 0xE0000/0x4000 = 56
			for (ubi = 0; ubi < 56; ubi++) {
				if (ftlReadSinglePlane(ubTmpCodeUnit, ubi, 0, (U32)&ubMicroCodeBuffer[ubi * 0x4000], CODE_LCA_TEMP, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubTmpCodeBurst)) {
					gubDLMCStep = ERROR_DETECT;
					DLMC_ErrorDetect(34);
				}
			}

			ftl_Erase_Single_Plane((U32)ubTmpCodeUnit, ubTmpCodeBurst, 0, BIT_FJOBI_FASTPAGE);

			ftlAddToSystemSetFreeTable(gubMicroCodeTmpSet);
			gubMicroCodeTmpSet = 0xFF;
		}
		else {
			gubDLMCStep = ERROR_DETECT;
			DLMC_ErrorDetect(35);
		}
#else
		//when in step 3,4,5,6, need confirm the L2P READ MODE RAM is locked by DLMC
		gubDLMCStep = ERROR_DETECT;
		DLMC_ErrorDetect(33);
#endif
	}
	if (gubDLMCStep == ERROR_DETECT) {
		/*
		Tester Driver Issue:
			如果在收Data前就知道要回Abort就不收Data
			如果收了Data後才知道要回Abort就必須收完Data
		*/
		ubNotReceiveData = 1;
	}
	gubLastDLMCMode = ubMode;
	//*************************************
	//1.read header (get burner size)
	//*************************************
Mark_Read_Header:

	if (gubDLMCStep == 0) {
		while ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) {
			M_MicroCodeTrigger();  //如果 HRST or SRST, M_MicroCodeTrigger裡面會 break 掉外面的 while loop, 且將 gubDLMCStep 設為 ERROR_DETECT

			if (gubHeaderTransferSectorCnt == 0) {
				if ((BUFB_BASE[0 + uwSectorPTR] == 0x50) && (BUFB_BASE[1 + uwSectorPTR] == 0x53) && (BUFB_BASE[2 + uwSectorPTR] == 0x0C) && (BUFB_BASE[3 + uwSectorPTR] == 0x27) && (BUFB_BASE[53 + uwSectorPTR] == 0x33)) {
					DLMC_InitialSetting();
					break;
				}
				else if ((BUFB_BASE[0 + uwSectorPTR] == 0x50) && (BUFB_BASE[1 + uwSectorPTR] == 0x53) && (BUFB_BASE[2 + uwSectorPTR] == 0x0C) && (BUFB_BASE[3 + uwSectorPTR] == 0x27) && (BUFB_BASE[5 + uwSectorPTR] == FW_REVISION2) && (BUFB_BASE[DLMC_FW_Version_2 + uwSectorPTR] == FW_REVISION3) && (BUFB_BASE[DLMC_FW_Version_4 + uwSectorPTR] == FW_REVISION5)) {
					gulBurnerBinFileSectors = ((BUFB_BASE[7 + uwSectorPTR] << 24) + (BUFB_BASE[8 + uwSectorPTR] << 16) + (BUFB_BASE[9 + uwSectorPTR] << 8) + BUFB_BASE[10 + uwSectorPTR] ) / 512;
					gulMicroCodeBinFileSectors = ((BUFB_BASE[14 + uwSectorPTR] << 24) + (BUFB_BASE[15 + uwSectorPTR] << 16) + (BUFB_BASE[16 + uwSectorPTR] << 8) + BUFB_BASE[17 + uwSectorPTR] ) / 512;
					gulUpdateSystemInfoMap = BUFL_BASE[(32 + uwSectorPTR) >> 2];
					gubUpdateInfoBlock = BUFB_BASE[DLMC_With_Infoblock + uwSectorPTR];

					if (gubUpdateInfoBlock) {
						uwInfoChecksum = (BUFB_BASE[DLMC_Infoblock_Checksum_High_Byte + uwSectorPTR] << 8) + BUFB_BASE[DLMC_Infoblock_Checksum_Low_Byte + uwSectorPTR];
						uwInfoChecksumWithDummy = (BUFB_BASE[DLMC_InfoblockWithDummy_Checksum_High_Byte + uwSectorPTR] << 8) + BUFB_BASE[DLMC_InfoblockWithDummy_Checksum_Low_Byte + uwSectorPTR];
					}
					if (BUFB_BASE[13 + uwSectorPTR] == 0xDD) {
						gubDoPreformatAfterDLMC = 1;
					}
					else {
						if (BUFB_BASE[23 + uwSectorPTR] == VT_VERSION) {
							gubDoPreformatAfterDLMC = 0;
							if (BUFB_BASE[13 + uwSectorPTR] == 0xDC) {
								//Add option, Do SearchMixPlane
								ftlSearchMixPlaneInFreeBlockTable();
								ftlSaveTable(&VT->guwTableTarget[VT->gubTableTargetIndex], (U32)gulEC_D3, 0, BYTE_Save_EC_Mode);
								VT->gubTableTargetSyncIndex = VT->gubTableTargetIndex;
								VT->guwTableTargetSyncPTR = VT->guwTableTargetPTR;
								ftlSaveVT(0, VT_DLMC_ECUPDATE_0xDA);//Special VTSerialNumber
							}
						}
						else {
							gubDLMCStep = ERROR_DETECT;
							DLMC_ErrorDetect(22);
							break;
						}
					}

					if ((ubMode == 0x07) && (uwCmdSectorCnt < (gulBurnerBinFileSectors + gulMicroCodeBinFileSectors + 1))) {
						gubDLMCStep = ERROR_DETECT;
						DLMC_ErrorDetect(30);
						break;
					}



				}
				else {
					gubDLMCStep = ERROR_DETECT;
					DLMC_ErrorDetect(10);
					break;
				}
			}

			uwSectorPTR += 512;
			gubHeaderTransferSectorCnt ++;

			if (gubHeaderTransferSectorCnt == BINFILE_HEADER_SECTOR) {
				gubDLMCStep = HEADER_TRANSFERDONE;
				break;
			}
		}

	}

	//*************************************
	//2.read burner (just read out burner, no use)
	//*************************************
Mark_Read_Burner:

	if (gubDLMCStep == HEADER_TRANSFERDONE) {
		if (gulBurnerBinFileSectors) {
			while ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) {
				M_MicroCodeTrigger();

				uwSectorPTR += 512;
				if (uwSectorPTR >= 16 * 1024) {
					uwSectorPTR = 0;
				}

				gulBurnerTransferSectorCnt ++;
				if (gulBurnerTransferSectorCnt == gulBurnerBinFileSectors) {
					gubDLMCStep = BURNER_TRANSFERDONE;
					break;
				}
			}
		}
		else {
			gubDLMCStep = BURNER_TRANSFERDONE;
		}
	}

	//*************************************
	//3.read FW 16KB bin header
	//*************************************
Mark_Read_FW_16KB_Bin_Header:

	if (gubDLMCStep == BURNER_TRANSFERDONE) {
		while ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) {
			M_MicroCodeTrigger();

			if (gulMicroCodeTransferSectorCnt == 0) {
				gulBurnerSeedOrigin = 0;

				for (ubSectionIndex = 0; ubSectionIndex < 8; ubSectionIndex++) {
					gulSectionInformation[BINHEADER_WRITE_ADDR][ubSectionIndex] = BUFL_BASE[(ubSectionIndex << 1) + (uwSectorPTR >> 2)]; // write destination address
					gulSectionInformation[BINHEADER_READ_ADDR][ubSectionIndex] = BUFL_BASE[16 + (ubSectionIndex << 1) + (uwSectorPTR >> 2)]; // header read destination address
					gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex] = BUFL_BASE[1 + (ubSectionIndex << 1) + (uwSectorPTR >> 2)]; // each section size
				}
			}

			if (gulMicroCodeTransferSectorCnt >= 2) {
				gulBurnerSeedOrigin ^= BUFL_BASE[(gulMicroCodeTransferSectorCnt - 2) + uwSectorPTR / 4];
			}

			uwSectorPTR += 512;
			if (uwSectorPTR >= 16 * 1024) {
				uwSectorPTR = 0;
			}

			gulMicroCodeTransferSectorCnt ++;
			if (gulMicroCodeTransferSectorCnt == MICROCODE_HEADER_SECTOR) {
				gubDLMCStep = MICROCODE_16K_HEADER_TRANSFERDONE;
				gulCurrentSeed = gulBurnerSeedOrigin;

				if (gubLockReadMode == LOCK_READ_MODE_NOBODY) {
					ftlSwapL2PTable(GC_LOCK_L2P_FOR_DLMC, (BYTE_Lock_Read_Mode | LOCK_READ_MODE_DLMC));
				}
				else {
					if (gubLockReadMode != LOCK_READ_MODE_DLMC) {
						//if locked by non-DLMC, return abort.
						gubDLMCStep = ERROR_DETECT;
						DLMC_ErrorDetect(11);
					}
					else {
						//has been locked by DLMC.
					}
				}

				break;
			}
		}

	}

	//*************************************
	//4.read FW (FW ICCM/DCCM, ICODE, DDR, FUNC ICCM/DCCM)
	//*************************************
Mark_Read_FW:

	if (gubDLMCStep == MICROCODE_16K_HEADER_TRANSFERDONE) {
		while ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) {
			M_MicroCodeTrigger();

			mDMAC_SV(1, (U32)&ubMicroCodeBuffer[0xF8000], 512, gulCurrentSeed, 0, 0, DMA_EN_RAND | DMA_EN_SETCQ, 0);//use gubHostTable
			mDMAC_XOR((U32)&ubMicroCodeBuffer[0xF8000], (U32)&BUFB_BASE[uwSectorPTR], (U32)&ubMicroCodeBuffer[(gubSectorsPerPlane + gulMicroCodeTransferSectorCnt - MICROCODE_HEADER_SECTOR) * 512], 512, 1, 0);
			gulCurrentSeed++;

			uwSectorPTR += 512;
			if (uwSectorPTR >= 16 * 1024) {
				uwSectorPTR = 0;
			}

			gulMicroCodeTransferSectorCnt ++;

			if (gulMicroCodeTransferSectorCnt == gulMicroCodeBinFileSectors ) {
				if (gubDLMCReturnDummyAbort && (((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) && (gubUpdateInfoBlock == 0))) {
					gubDLMCStep = ERROR_DETECT;
					DLMC_ErrorDetect(29);
				}
				else {
					gubDLMCStep = MICROCODE_TRANSFERDONE;
				}

				break;
			}
		}
	}

	//*************************************
	//5.Copy InfoBlock to Buffer
	//*************************************
Mark_Copy_Infoblock_To_Buffer:

	if (gubDLMCStep == MICROCODE_TRANSFERDONE) {
		if (gubUpdateInfoBlock) {
			while ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) {
				M_MicroCodeTrigger();
				UWORD uwTemp = 0;
				for (guwTemp = 0; guwTemp < 512; guwTemp++) { //Checksum for info block & dummy sector
					uwTemp += BUFB_BASE[uwSectorPTR + guwTemp];
				}
				uwTemp = (~uwTemp) + 1;

				//InfoBlock Checksum Pass
				if ( (gubInfoBlockTransferSectorCnt == 0) && (uwTemp == uwInfoChecksum)) {
					mSE_COPY((U32)&BUFB_BASE[uwSectorPTR], (U32)&guwEarlyBadCntPerPlane[0], 512);//Use guwEarlyBadCntPerPlane RAM for DLMC
				}
				//InfoBlock Checksum Pass
				else if ((gubInfoBlockTransferSectorCnt == 1) && ((uwTemp + uwInfoChecksum) == uwInfoChecksumWithDummy)) {
					//Donothing
				}
				else {
					if (ENABLE_DEBUG_UART_DLMC) {
						UartString("\r\nDLMC Err: InfoChecksum Fail, State:");
						Uart_Tx_DataHex(gubInfoBlockTransferSectorCnt);
						UartString("  Checksum");
						UartWordHex(uwTemp);
						UartString("  infoChecksum");
						UartWordHex(uwInfoChecksum);
						UartString("  ChecksumWithDummy");
						UartWordHex(uwInfoChecksumWithDummy);
					}
					gubDLMCStep = ERROR_DETECT;
					DLMC_ErrorDetect(31);
					break;
				}
				uwSectorPTR += 512;
				if (uwSectorPTR >= 16 * 1024) {
					uwSectorPTR = 0;
				}
				gubInfoBlockTransferSectorCnt++;

				if (gubInfoBlockTransferSectorCnt == BINFILE_INFOBLOCK_SECTOR) {
					if (gubDLMCReturnDummyAbort && ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0))) {
						gubDLMCStep = ERROR_DETECT;
						DLMC_ErrorDetect(32);
					}
					else {
						gubDLMCStep = INFOBLOCK_TRANSFERDONE;
					}
					break;
				}
			}
			if (gubInfoBlockTransferSectorCnt < BINFILE_INFOBLOCK_SECTOR) {
				gubDLMCStep = ERROR_DETECT;
				DLMC_ErrorDetect(36);
			}
		}
		else {
			gubDLMCStep = INFOBLOCK_TRANSFERDONE;
		}
	}

	//*************************************
	//6.check CRC
	//*************************************
Mark_Check_CRC:

	if (gubDLMCStep == INFOBLOCK_TRANSFERDONE) {
		if (ENABLE_BURNER_USE_RANDOMIZER_DECODE) {
			UBYTE ubDeRanCtr;
			UBYTE ubi;

			ubDeRanCtr = 0;
			for (ubSectionIndex = 0; ubSectionIndex < 8; ubSectionIndex++) {
				for (ubi = 0; ubi < (gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex] / 0x4000); ubi++) {
					FlaFlashDeRandomize((U32*)&ubMicroCodeBuffer[gubSectorsPerPlane * 512 + (ubDeRanCtr * 0x4000)], (U32*)&BUFB_BASE[0], 4);
					mSE_COPY((U32)&BUFB_BASE[0], (U32)&ubMicroCodeBuffer[gubSectorsPerPlane * 512 + (ubDeRanCtr * 0x4000)], 0x4000);
					ubDeRanCtr++;
				}
			}
		}

		ulMicroCodeSectionOffset = 0;
		for (ubSectionIndex = 0; ubSectionIndex < 8; ubSectionIndex++) {
			if (gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex]) {
				if (mDMAC_CRC((U32)&ubMicroCodeBuffer[ulMicroCodeSectionOffset + gubSectorsPerPlane * 512], gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex], Burner_CRC32_Seed, 1, (DMA_CRC_CHK | DMA_CRC_LEN_8 | DMA_CRC_DCCM)) ) {
					//error
					gubDLMCStep = ERROR_DETECT;
					DLMC_ErrorDetect(12);
					break;
				}
				else {
					ulMicroCodeSectionOffset +=  gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex];
				}
			}
		}

		if (gubDLMCStep != ERROR_DETECT) {
			if (gubSystemSetFreeCount >= 2) {
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
				// Due to bootcode restriction, B0KB only can be scanned 2 units.
				// Although L06B can be scanned up to 4 units, but constraint other unit(SYS/DBT) not to use 0-3 units
				// will decrease unit 4-7's lifetime cycle more quickly.
				// So B0KB/L06B don't need to get new system block, only access unit 0 and 1.

				/*ubNewSysCodeUnit[0] = 0xFF;
				ubNewSysCodeUnit[1] = 0xFF;
				ubNewSysCodeBurst[0] = 0xFF;
				ubNewSysCodeBurst[1] = 0xFF;*/

				ubNewSysCodeSet[0] = 0xFF;
				ubNewSysCodeSet[1] = 0xFF;
				uwNewSysCodeBlock[0][0] = 0xFFFF;
				uwNewSysCodeBlock[0][1] = 0xFFFF;
				uwNewSysCodeBlock[1][0] = 0xFFFF;
				uwNewSysCodeBlock[1][1] = 0xFFFF;

#else
				ubNewSysCodeSet[0] = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_HEAD);
				ubNewSysCodeSet[1] = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_HEAD);
				uwNewSysCodeBlock[0][0] = guwSystemBlockRemappingTable[ubNewSysCodeSet[0]][0];
				uwNewSysCodeBlock[0][1] = guwSystemBlockRemappingTable[ubNewSysCodeSet[0]][1];
				uwNewSysCodeBlock[1][0] = guwSystemBlockRemappingTable[ubNewSysCodeSet[1]][0];
				uwNewSysCodeBlock[1][1] = guwSystemBlockRemappingTable[ubNewSysCodeSet[1]][1];

#endif
				gubDLMCStep = SYSINFO_TRANSFERDONE;
			}
			else {
				gubDLMCStep = ERROR_DETECT;
				DLMC_ErrorDetect(13);
			}
		}
	}
	//*************************************
	//7.update system info
	//*************************************
Mark_Update_System_Info:

	if (gubDLMCStep == SYSINFO_TRANSFERDONE) {
		if (gulUpdateSystemInfoMap) {
			while ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) {
				M_MicroCodeTrigger();
				mSE_COPY((U32)&BUFB_BASE[uwSectorPTR], (U32)&ubMicroCodeBuffer[0xF0000 + gubUpdateSysInfoTransferSectorCnt * 512], 512);

				uwSectorPTR += 0x200;
				if (uwSectorPTR >= 16 * 1024) {
					uwSectorPTR = 0;
				}
				gubUpdateSysInfoTransferSectorCnt++;

				if (gubUpdateSysInfoTransferSectorCnt == BINFILE_SYSINFO_SECTOR) {
					if (gulUpdateSystemInfoMap) {
						DLMC_UpdateSystemInfo(gulUpdateSystemInfoMap);
					}
					gubDLMCStep = ALL_TRANSFERDONE;
					break;
				}
			}
		}
		else {
			gubDLMCStep = ALL_TRANSFERDONE;
		}

	}


	//*************************************
	//7.Save Microcode to Flash
	//*************************************
Mark_Save_Microcode_To_Flash:

	if (gubDLMCStep == ALL_TRANSFERDONE) {
		// Turn on randomizer to read old code.
		flaMicronRandomizer(1);

		ulFCONL_FLH_FUNC_Temp = FCONL[FCONL_FLH_FUNC];
#if !ENABLE_TLC_PROGRAM_CODEBLOCK2
		FCONL[FCONL_FLH_FUNC] |= RDM_REF_ROW_ADR_5_TO_0;
#endif

		ulLDPC_CFG_Temp = FCONL[FCONL_LDPC_CFG];
		FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
		FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);

		ulPlaneIndex = 0;
		ubCEIndex = 0;
		//----------------- read old code unit to see its version-------------------//
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

		FQ->ulRAMAddr = (U32)&ubMicroCodeBuffer[0];
		FQ->btCheckEmpty = 1;
		FQ->btThisIsErasePage = 0;
		FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
		FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
		FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif

		ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] >> gubBurstsPerBankLog);
		ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] & gubBurstsPerBankMask);
		FQ->uwFUnit = ubSysCodeUnit;
		FQ->ulFEntry = ((ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog;
		FQ->ubDepth = ubDepth;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btUNServed = 1;

		gulPara0x24.BitMap.ubFrameNum = 1;
		gulPara0x24.BitMap.btBufModeEn = 0;
		gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
		gulPara0x24.BitMap.ubL4KNum = 1;
		gulPara0x24.BitMap.btZipBypass = 1;
		gulPara0x24.BitMap.btCmpEn = 0;
		gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
		gMTQ->Depth[ubDepth].ulZipMap = 0x000000FF;

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate( (void *)&ubMicroCodeBuffer[0], 4096);
		}

		for (ubL4KCount = 0; ubL4KCount < 1; ubL4KCount++) {
			pL4KTablePtr = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA = 0x00000000;
			pL4KTable.Para0x04.ulAll = 0;
			pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
			pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
			*pL4KTablePtr = pL4KTable;
		}

		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
#if Hynix
		FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
		while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
#if Hynix
		FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
		if (FQ->btReadUNC) {
			DLMC_ErrorDetect(14);
			FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
			gubDLMCStep = ERROR_DETECT;

			goto Mark_End;
		}

		FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

		//old and new microcode need to program in different version
		pL4KTablePtr = (volatile L4KTable16B*)( L4K_TABLE_ADDR + (FQ->ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
		if (pL4KTablePtr->ulL4K_LCA == HEADER_LCA) {
			ubCodeVersion = (UBYTE)((pL4KTablePtr->Para0x04.BitMap.ulL4K_FW) >> 8);
			if ((ubCodeVersion & 0xF0) == 0xA0) {
				ubCodeVersion = 0xA0 + ((ubCodeVersion + 1) & 0x0F);
			}
			else {
				//flaLEDBlink(1);
				DLMC_ErrorDetect(23);
				gubDLMCStep = ERROR_DETECT;
				goto Mark_End;
			}
		}

		//----------------- update 4K header -------------------//
		for (ubSectionIndex = 0; ubSectionIndex < 8; ubSectionIndex++) {
			if (gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex]) {
				if (ubSectionIndex < 4) {
					ubFWSecNumber = ubSectionIndex + 1;
				}
				else {
					ubFUNCSecNumber++;
				}

				//(Page number Must be in pair)
				uwPlaneNum[ubSectionIndex] = ((gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex]) / (gubSectorsPerPlane * 512));
				if (gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex] % (gubSectorsPerPlane * 512)) {
					if (uwPlaneNum[ubSectionIndex] & BIT0) {
						uwPlaneNum[ubSectionIndex]++;
					}
					else {
						uwPlaneNum[ubSectionIndex] += 2;
					}
				}
				else {
					if (uwPlaneNum[ubSectionIndex] & BIT0) {
						uwPlaneNum[ubSectionIndex]++;
					}
				}
			}
			else {
				uwPlaneNum[ubSectionIndex] = 0;
			}
		}

		if (ubMicroCodeBuffer[0] == 'I' && ubMicroCodeBuffer[1] == 'D' ) {
			ubMicroCodeBuffer[HEADERB_FW_REVISION_ID] = ubCodeVersion;
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))

			uwMicroCodeBuffer[HEADERW_CH0_CODE0_Block] = guwSysCodeBlock[0][0]; //uwStartBlock + (ubVersion * 2 * ubNext);
			uwMicroCodeBuffer[HEADERW_CH0_CODE1_Block] = guwSysCodeBlock[1][0]; //uwStartBlock + ubNext + (ubVersion * 2 * ubNext);
			uwMicroCodeBuffer[HEADERW_CH1_CODE0_Block] = guwSysCodeBlock[0][1]; //uwStartBlock + (ubVersion * 2 * ubNext);
			uwMicroCodeBuffer[HEADERW_CH1_CODE1_Block] = guwSysCodeBlock[1][1]; //uwStartBlock + ubNext + (ubVersion * 2 * ubNext);
#else

			//uwNewSysCodeBlock[][]前面已經填好了
			uwMicroCodeBuffer[HEADERW_CH0_CODE0_Block] = uwNewSysCodeBlock[0][0]; //uwStartBlock + (ubVersion * 2 * ubNext);
			uwMicroCodeBuffer[HEADERW_CH0_CODE1_Block] = uwNewSysCodeBlock[1][0]; //uwStartBlock + ubNext + (ubVersion * 2 * ubNext);
			uwMicroCodeBuffer[HEADERW_CH1_CODE0_Block] = uwNewSysCodeBlock[0][1]; //uwStartBlock + (ubVersion * 2 * ubNext);
			uwMicroCodeBuffer[HEADERW_CH1_CODE1_Block] = uwNewSysCodeBlock[1][1]; //uwStartBlock + ubNext + (ubVersion * 2 * ubNext);

#endif

			ubMicroCodeBuffer[HEADERB_FW_START_SECTION] = 0;
			ubMicroCodeBuffer[HEADERB_FW_SECTION_NUM] = ubFWSecNumber; // Done in previous section

			uwMicroCodeBuffer[HEADERW_S0_PAGES] = uwPlaneNum[0];
			uwMicroCodeBuffer[HEADERW_S1_PAGES] = uwPlaneNum[1];
			uwMicroCodeBuffer[HEADERW_S2_PAGES] = uwPlaneNum[2];
			uwMicroCodeBuffer[HEADERW_S3_PAGES] = uwPlaneNum[3];

			uwMicroCodeBuffer[HEADERW_S0_START_PAGE] = 1;
			uwMicroCodeBuffer[HEADERW_S1_START_PAGE] = uwPlaneNum[0]  + 1;
			uwMicroCodeBuffer[HEADERW_S2_START_PAGE] = uwPlaneNum[0] + uwPlaneNum[1]  + 1;
			uwMicroCodeBuffer[HEADERW_S3_START_PAGE] = uwPlaneNum[0] + uwPlaneNum[1]  + uwPlaneNum[2] + 1;

			if (Enable_Only_Header == 0) {
				ulMicroCodeBuffer[HEADERL_S0_DESTINATION] = (gulSectionInformation[1][0]);
#if YMTC_JGS
#if ENABLE_TLC_PROGRAM_CODEBLOCK && (!ENABLE_TLC_SPI_BURNER)
				ulMicroCodeBuffer[HEADERL_S1_DESTINATION] = (gulSectionInformation[1][1]);
#else
				ulMicroCodeBuffer[HEADERL_S1_DESTINATION] = 0x2c002000;
#endif

#elif UFS
				ulMicroCodeBuffer[HEADERL_S1_DESTINATION] = 0x2c002000;

#else
				ulMicroCodeBuffer[HEADERL_S1_DESTINATION] = (gulSectionInformation[1][1]);
#endif
				ulMicroCodeBuffer[HEADERL_S2_DESTINATION] = gulSectionInformation[1][2];
				ulMicroCodeBuffer[HEADERL_S3_DESTINATION] = gulSectionInformation[1][3];
			}


			ulMicroCodeBuffer[HEADERL_S0_SIZE] = gulSectionInformation[2][0];   // fill in the real size
			ulMicroCodeBuffer[HEADERL_S1_SIZE] = gulSectionInformation[2][1];
			ulMicroCodeBuffer[HEADERL_S2_SIZE] = gulSectionInformation[2][2];
			ulMicroCodeBuffer[HEADERL_S3_SIZE] = gulSectionInformation[2][3];

			if ((Enable_Function_Code) && (gulSectionInformation[0][4] != 0)) {
				ubMicroCodeBuffer[HEADERB_FUNCTION_START_SECTION] = 4;
				ubMicroCodeBuffer[HEADERB_FUNCTION_SECTION_NUM] = ubFUNCSecNumber;

				uwMicroCodeBuffer[HEADERW_S4_PAGES] = uwPlaneNum[4];
				uwMicroCodeBuffer[HEADERW_S4_START_PAGE] = uwPlaneNum[0] + uwPlaneNum[1] + uwPlaneNum[2] + uwPlaneNum[3] + 1; ///0:ICCM 1:DCCM 2:ICODE 3:DBUF
				ulMicroCodeBuffer[HEADERL_S4_DESTINATION] = gulSectionInformation[1][4];
				ulMicroCodeBuffer[HEADERL_S4_SIZE] = gulSectionInformation[2][4];

				uwMicroCodeBuffer[HEADERW_S5_PAGES] = uwPlaneNum[5];
				uwMicroCodeBuffer[HEADERW_S5_START_PAGE] = uwPlaneNum[0] + uwPlaneNum[1] + uwPlaneNum[2] + uwPlaneNum[3] + uwPlaneNum[4] + 1; ///0:ICCM 1:DCCM 2:ICODE 3:DBUF
				ulMicroCodeBuffer[HEADERL_S5_DESTINATION] = gulSectionInformation[1][5];
				ulMicroCodeBuffer[HEADERL_S5_SIZE] = gulSectionInformation[2][5];

				ubMicroCodeBuffer[HEADERB_LOAD_FUNCTION_ENABLE] = ENABLE;
			}
			else {
				ubMicroCodeBuffer[HEADERB_LOAD_FUNCTION_ENABLE] = DISABLE;
			}

			/*uwMicroCodeBuffer[HEADERW_ALU_RULE_W1] = 0x4181;
			uwMicroCodeBuffer[HEADERW_ALU_RULE_W2] = 0x4598;
			uwMicroCodeBuffer[HEADERW_ALU_RULE_W3] = 0x6617;*/

			volatile UWORD *puwALU = (UWORD*)(&IRAMB[ALU_BASE_0]);
			UWORD uwPageIndex;
			for (uwPageIndex = 0; uwPageIndex < 8; uwPageIndex ++) {
				if (ENABLE_iFSA) {
					if (uwPageIndex) {
						uwMicroCodeBuffer[ HEADERW_ALU_RULE_W0 + uwPageIndex] = puwALU[uwPageIndex] - 0x2000 + (0x400 * gubBurstsPerBankLog);
					}
					else {
						uwMicroCodeBuffer[ HEADERW_ALU_RULE_W0 + uwPageIndex] = puwALU[uwPageIndex] - 0x2000;
					}
				}
				else {
					if (uwPageIndex) {
						uwMicroCodeBuffer[ HEADERW_ALU_RULE_W0 + uwPageIndex] = puwALU[uwPageIndex] + (0x400 * gubBurstsPerBankLog);
					}
					else {
						uwMicroCodeBuffer[ HEADERW_ALU_RULE_W0 + uwPageIndex] = puwALU[uwPageIndex];
					}
				}
			}

#if (MicronFlashOnly||YMTC)
#if ENABLE_TLC_PROGRAM_CODEBLOCK2
			uwMicroCodeBuffer[ HEADERW_ALU_RULE_W0] = 0x1508;
#else
			uwMicroCodeBuffer[ HEADERW_ALU_RULE_W0] = 0x1507;
#endif
#endif

#pragma flush

			ulMicroCodeBuffer[(4096 - 4) / 4] = mDMAC_CRC((U32)gMicroCodeBuffer, 1 * 4096, HEADER_CRC32_SEED, 1, (DMA_CRC_LEN_8 | DMA_CRC_DCCM));
		}
		else {
			//flaLEDBlink(0x01);
			DLMC_ErrorDetect(24);
			gubDLMCStep = ERROR_DETECT;

			goto Mark_End;
		}

		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv(gMicroCodeBuffer, 4096);
		}

		//-------------- program to new code block --------------//

		UBYTE ubIndex;
		for (ubIndex = 0; ubIndex < 2; ubIndex++) {
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))

			U32 ulRET = 0xFFFF;
			// Micron mode DLMC from unit 1 to unit 0. (always keep unit 0 safe)

			ubCodeBlockIndex = (1 - ubIndex);
			if ((ubMode0xE_Flag == 1) && (ubCodeBlockIndex == 0)) {
				continue;			// Micron mode for mode 0xE special handle, only replace unit 1 for to-be-updated code unit.
			}
			//scan unit

#else
			ubCodeBlockIndex = ubIndex;
#endif
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				//erase

#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
				//scan unit
#if FixShallowErase
				ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] >> gubBurstsPerBankLog);
				ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] & gubBurstsPerBankMask);

				ulRET = ftlScanUnit_Binary((BIT_D3_TARGET | BIT_DIRECT_ACCESS), ubSysCodeUnit);
				U32 ulRET2 = ((ulRET / gubPlanesPerPage + 1) * gubPlanesPerPage) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst;

				if ((ulRET2 < (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div) - 1)) && (ulRET2 != 0)) {
					ftlForceSaveDummy(ulRET2, (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div)), ubSysCodeUnit, (FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE | FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE | FORCE_SAVE_DUMMY_DIRECT_ACCESS), D3_AREA, VT_FORCE_DUMMY_0xD9);
				}
				// wait all FQ done.
				while (gFQI.ubFQLinkNumber) {
					M_SwitchTask();
				}
#endif
#endif
				FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
				FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);
				ulPlaneIndex = 0;
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
				FQ->btD3 = 1;
#else
				FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif

				// Must Send Reset CMD(0xFF) After Erase Fail
				FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))

				ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] >> gubBurstsPerBankLog);
				ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] & gubBurstsPerBankMask);

				FQ->uwFUnit = ubSysCodeUnit;
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = ((ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
				}
#else
				ubNewSysCodeUnit = (UBYTE)(uwNewSysCodeBlock[ubCodeBlockIndex][ubCEIndex] >> gubBurstsPerBankLog);
				ubNewSysCodeBurst = (UBYTE)(uwNewSysCodeBlock[ubCodeBlockIndex][ubCEIndex] & gubBurstsPerBankMask);

				FQ->uwFUnit = ubNewSysCodeUnit;
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubNewSysCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = ((ubCEIndex + (ubNewSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
				}
#endif

				FQ->ubRelativeDepth = ubDepth;
				FQ->ubDepth = 0xFF;
				FQ->btUNServed = 1;
				FQ->btCheckEmpty = 0;

				gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gFQI.ubFQLinkNumber++;
				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

				while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}

				if (FQ->btPEFail) {
					DLMC_ErrorDetect(15);
					FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
					gubDLMCStep = ERROR_DETECT;
					ubNeedDoEraseUnitFlag = 1;
					ubNeedRemoveSet = ubNewSysCodeSet[ubCodeBlockIndex];// micron不用remove

					goto Mark_End;
				}

				FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
#if ((MicronFlashOnly||YMTC))
			}
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				ulPlaneIndex = 0;
#endif
				//----------------- program 4K header -------------------//
				for (ubLMU = 0; ubLMU < (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1); ubLMU++) {
					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->ulRAMAddr = (U32)&ubMicroCodeBuffer[0];
					FQ->ubFJob = BYTE_FJOB_WRITE;

#if ENABLE_TLC_PROGRAM_CODEBLOCK
					FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
					FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif




#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
					ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] >> gubBurstsPerBankLog);
					ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] & gubBurstsPerBankMask);

					FQ->uwFUnit = ubSysCodeUnit;
					if (ENABLE_PLANE_CH_CE) {
						FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex + ubLMU, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
					}
					else {
						FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex + ubLMU, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
					}
#else
					ubNewSysCodeUnit = (UBYTE)(uwNewSysCodeBlock[ubCodeBlockIndex][ubCEIndex] >> gubBurstsPerBankLog);
					ubNewSysCodeBurst = (UBYTE)(uwNewSysCodeBlock[ubCodeBlockIndex][ubCEIndex] & gubBurstsPerBankMask);

					FQ->uwFUnit = ubNewSysCodeUnit;
					if (ENABLE_PLANE_CH_CE) {
						FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex + ubLMU, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubNewSysCodeBurst) << gub4kEntrysPerPlaneLog);
					}
					else {
						FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex + ubLMU, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubNewSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
					}
#endif
					FQ->ubRelativeDepth = ubDepth;
					FQ->ubDepth = 0xFF;
					FQ->ubL4kIndex = ubL4kLink;
					FQ->btUNServed = 1;

#if ENABLE_TLC_PROGRAM_CODEBLOCK
					FQ->btD3 = 1;
					FQ->ubD3WriteMode = (D3_SUSPEND_EN) | D3_WRITE_FINE | (D3_WRITE_LOWDER << ubLMU);
					FQ->btTLCProgramCodeBlock = 1;
#endif
					gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
					gulPara0x24.BitMap.btBufModeEn = 0;
					gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
					gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
					gulPara0x24.BitMap.btZipBypass = 1;
					gulPara0x24.BitMap.btCmpEn = 0;
					gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
					gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

					for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
						pL4KTablePtr = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
						//set L4K table info
						pL4KTable.ulL4K_LCA = HEADER_LCA;
						pL4KTable.Para0x04.BitMap.ulL4K_FW = ((ubCodeBlockIndex + ubCEIndex * 2) + (ubCodeVersion << 8));
						pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
						pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
						pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
						pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0xAB;
						pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
						pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0x7;
						*pL4KTablePtr = pL4KTable;
					}

					gFQI.ubFQLinkNumber++;
					gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
#if Hynix
					FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
					// wait CE read done...
					while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
						M_SwitchTask();
					}
#if Hynix
					FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
					if (FQ->btPEFail) {
						DLMC_ErrorDetect(16);
						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
						gubDLMCStep = ERROR_DETECT;
						ubNeedDoEraseUnitFlag = 1;
						ubNeedRemoveSet = ubNewSysCodeSet[ubCodeBlockIndex];

						goto Mark_End;
					}

					FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
				}

				//----------------- 4K header read verification -------------------//
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

				FQ->ulRAMAddr = (U32)&ubMicroCodeBuffer[4096]; //read to 2nd 4K to verify, maybe debug easily
				FQ->btCheckEmpty = 1;
				FQ->btThisIsErasePage = 0;
				FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
				FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))

				FQ->uwFUnit = ubSysCodeUnit;
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
				}
#else

				FQ->uwFUnit = ubNewSysCodeUnit;
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubNewSysCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubNewSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
				}
#endif
				FQ->ubDepth = ubDepth;
				FQ->ubL4kIndex = ubL4kLink;
				FQ->btUNServed = 1;

				gulPara0x24.BitMap.ubFrameNum = 1;
				gulPara0x24.BitMap.btBufModeEn = 0;
				gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
				gulPara0x24.BitMap.ubL4KNum = 1;
				gulPara0x24.BitMap.btZipBypass = 1;
				gulPara0x24.BitMap.btCmpEn = 0;
				gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gMTQ->Depth[ubDepth].ulZipMap = 0xFF;

				if (ENABLE_DCACHE) {
					xthal_dcache_region_invalidate((void *)&ubMicroCodeBuffer[4096], 4096);
				}

				for (ubL4KCount = 0; ubL4KCount < 1; ubL4KCount++) {
					pL4KTablePtr = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
					//set L4K table info
					pL4KTable.ulL4K_LCA = 0x00000000;
					pL4KTable.Para0x04.ulAll = 0;
					pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
					pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
					pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
					pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
					*pL4KTablePtr = pL4KTable;
				}

				gFQI.ubFQLinkNumber++;
				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

#if Hynix
				FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
				while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}
#if Hynix
				FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
				if (FQ->btReadUNC) {
					DLMC_ErrorDetect(17);
					FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
					gubDLMCStep = ERROR_DETECT;
					ubNeedDoEraseUnitFlag = 1;
					ubNeedRemoveSet = ubNewSysCodeSet[ubCodeBlockIndex];

					goto Mark_End;
				}

				FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

				if (FQ->btThisIsErasePage) {
					//flaLEDBlink(0x03);
					DLMC_ErrorDetect(25);
					gubDLMCStep = ERROR_DETECT;
					ubNeedDoEraseUnitFlag = 1;
					ubNeedRemoveSet = ubNewSysCodeSet[ubCodeBlockIndex];

					goto Mark_End;
				}
				pL4KTablePtr = (volatile L4KTable16B*)( L4K_TABLE_ADDR + (FQ->ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
				if (pL4KTablePtr->ulL4K_LCA != HEADER_LCA) {
					//flaLEDBlink(0x04);
					DLMC_ErrorDetect(26);
					gubDLMCStep = ERROR_DETECT;
					ubNeedDoEraseUnitFlag = 1;
					ubNeedRemoveSet = ubNewSysCodeSet[ubCodeBlockIndex];

					goto Mark_End;
				}

				//----------------- program MicroCode-------------------//
				ulPlaneIndex ++;
				FCONL[FCONL_LDPC_CFG] = ulLDPC_CFG_Temp ;

				for (ubSectionIndex = 0; ubSectionIndex < 8; ubSectionIndex++) {
					for (uwSectionPlaneIndex = 0; uwSectionPlaneIndex < uwPlaneNum[ubSectionIndex]; uwSectionPlaneIndex++) {
						if (ENABLE_DCACHE) {
							xthal_dcache_region_writeback_inv((void *)&ubMicroCodeBuffer[ulPlaneIndex * gubSectorsPerPlane * 512], gubSectorsPerPlane * 512);
						}

						for (ubLMU = 0; ubLMU < (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1); ubLMU++) {
							//program
							M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
							FQ->ulRAMAddr = (U32)&ubMicroCodeBuffer[ulPlaneIndex * gubSectorsPerPlane * 512];
							FQ->ubFJob = BYTE_FJOB_WRITE;
							FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))

							FQ->uwFUnit = ubSysCodeUnit;
							if (ENABLE_PLANE_CH_CE) {
								FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex * (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1) + ubLMU, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
							}
							else {
								FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
							}
#else

							FQ->uwFUnit = ubNewSysCodeUnit;
							if (ENABLE_PLANE_CH_CE) {
								FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex * (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1) + ubLMU, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubNewSysCodeBurst) << gub4kEntrysPerPlaneLog);
							}
							else {
								FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubNewSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
							}
#endif
							FQ->ubRelativeDepth = ubDepth;
							FQ->ubDepth = 0xFF;
							FQ->ubL4kIndex = ubL4kLink;
							FQ->btUNServed = 1;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
							FQ->btD3 = 1;
							FQ->ubD3WriteMode = (D3_SUSPEND_EN) | D3_WRITE_FINE | (D3_WRITE_LOWDER << ubLMU);
							FQ->btTLCProgramCodeBlock = 1;
#endif

							gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
							gulPara0x24.BitMap.btBufModeEn = 0;
							gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
							gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
							gulPara0x24.BitMap.btZipBypass = 1;
							gulPara0x24.BitMap.btCmpEn = 0;
							gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
							gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

							for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
								pL4KTablePtr = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
								//set L4K table info
								pL4KTable.ulL4K_LCA = CODE_LCA;
								pL4KTable.Para0x04.BitMap.ulL4K_FW = ((ubSectionIndex + 1) + (ubCodeVersion << 8));
								pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
								pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
								pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
								pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0xAB;
								pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
								pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0x7;
								*pL4KTablePtr = pL4KTable;
							}

							gFQI.ubFQLinkNumber++;
							gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
#if Hynix
							FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
							// wait CE read done...
							while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
								M_SwitchTask();
							}
#if Hynix
							FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
							if (FQ->btPEFail) {
								DLMC_ErrorDetect(18);
								FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
								gubDLMCStep = ERROR_DETECT;
								ubNeedDoEraseUnitFlag = 1;
								ubNeedRemoveSet = ubNewSysCodeSet[ubCodeBlockIndex];

								goto Mark_End;
							}

							FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

						}
						//----------------- MicroCode read verification-------------------//
						M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

						FQ->ulRAMAddr = (U32)&ubMicroCodeBuffer[ulPlaneIndex * gubSectorsPerPlane * 512];
						FQ->btCheckEmpty = 1;
						FQ->btThisIsErasePage = 0;
						FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
						FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
						FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
						FQ->uwFUnit = ubSysCodeUnit;
						if (ENABLE_PLANE_CH_CE) {
							FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex * (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1), DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
						}
						else {
							FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
						}
#else
						FQ->uwFUnit = ubNewSysCodeUnit;
						if (ENABLE_PLANE_CH_CE) {
							FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex * (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1), DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubNewSysCodeBurst) << gub4kEntrysPerPlaneLog);
						}
						else {
							FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubNewSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
						}
#endif
						FQ->ubDepth = ubDepth;
						FQ->ubL4kIndex = ubL4kLink;
						FQ->btUNServed = 1;

						gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
						gulPara0x24.BitMap.btBufModeEn = 0;
						gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
						gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
						gulPara0x24.BitMap.btZipBypass = 1;
						gulPara0x24.BitMap.btCmpEn = 0;
						gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
						gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
						gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

						if (ENABLE_DCACHE) {
							xthal_dcache_region_invalidate((void *)&ubMicroCodeBuffer[ulPlaneIndex * gubSectorsPerPlane * 512], gubSectorsPerPlane * 512);
						}

						for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
							pL4KTablePtr = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
							//set L4K table info
							pL4KTable.ulL4K_LCA = 0x00000000;
							pL4KTable.Para0x04.ulAll = 0;
							pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
							pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
							pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
							pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
							*pL4KTablePtr = pL4KTable;
						}

						gFQI.ubFQLinkNumber++;
						gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

#if Hynix
						FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
						while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
							M_SwitchTask();
						}
#if Hynix
						FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
						if (FQ->btReadUNC) {
							DLMC_ErrorDetect(19);
							FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
							gubDLMCStep = ERROR_DETECT;
							ubNeedDoEraseUnitFlag = 1;
							ubNeedRemoveSet = ubNewSysCodeSet[ubCodeBlockIndex];

							goto Mark_End;
						}

						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

						if (FQ->btThisIsErasePage) {
							//flaLEDBlink(0x03);
							DLMC_ErrorDetect(27);
							gubDLMCStep = ERROR_DETECT;
							ubNeedDoEraseUnitFlag = 1;
							ubNeedRemoveSet = ubNewSysCodeSet[ubCodeBlockIndex];

							goto Mark_End;
						}
						pL4KTablePtr = (volatile L4KTable16B*)( L4K_TABLE_ADDR + (FQ->ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
						if (pL4KTablePtr->ulL4K_LCA != CODE_LCA) {
							DLMC_ErrorDetect(28);
							gubDLMCStep = ERROR_DETECT;
							ubNeedDoEraseUnitFlag = 1;
							ubNeedRemoveSet = ubNewSysCodeSet[ubCodeBlockIndex];

							goto Mark_End;
						}

						ulPlaneIndex++;
					}
				}


#if ENABLE_TLC_PROGRAM_CODEBLOCK2
				U32 ulDummyPage;
				mDMAC_SV( 4, (U32)&ubMicroCodeBuffer[0xFC000], 16384, 0x31113111, 0xffffffff, 0, DMA_EN_SETCQ, 0);
				for (ulDummyPage = ulPlaneIndex * 3 ; ulDummyPage < guwPagesPerUnit; ulDummyPage += 3) {
					ftlWriteSinglePlaneTLC(ubSysCodeUnit, ulDummyPage, ubCEIndex,	(U32)&ubMicroCodeBuffer[0xFC000], CODE_LCA, 0xFFFFFFFF, (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSysCodeBurst);
				}

#else
				if (gubFlashID[0] == 0xb5) {
					U32 ulDummyPage;
					mDMAC_SV( 4, (U32)&ubMicroCodeBuffer[0xFC000], 16384, 0x31113111, 0xffffffff, 0, DMA_EN_SETCQ, 0);
					for (ulDummyPage = ulPlaneIndex ; ulDummyPage < guwPagesPerUnit; ulDummyPage++) {
						ftlWriteSinglePlane(ubSysCodeUnit, ulDummyPage, ubCEIndex,  (U32)&ubMicroCodeBuffer[0xFC000], CODE_LCA, 0xFFFFFFFF, (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSysCodeBurst);
					}
				}
#endif
			}
		}

		//-------------- replacing deferred microcode  --------------//
		for (ubCodeBlockIndex = 0; ubCodeBlockIndex < 2; ubCodeBlockIndex++) {
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
			if (gubDeferredCodeSet[ubCodeBlockIndex] == gubSysCodeSet[ubCodeBlockIndex]) {
				continue;
			}
#endif
			if (gubDeferredCodeSet[ubCodeBlockIndex] != 0xFF) {
				for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
					FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
					FQ->btD3 = 1;
#else
					FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif

					// Must Send Reset CMD(0xFF) After Erase Fail
					FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

					ubDeferredCodeUnit = (UBYTE)(guwSystemBlockRemappingTable[gubDeferredCodeSet[ubCodeBlockIndex]][ubCEIndex] >> gubBurstsPerBankLog);
					ubDeferredCodeBurst = (UBYTE)(guwSystemBlockRemappingTable[gubDeferredCodeSet[ubCodeBlockIndex]][ubCEIndex] & gubBurstsPerBankMask);

					FQ->uwFUnit = ubDeferredCodeUnit;
					if (ENABLE_PLANE_CH_CE) {
						FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubDeferredCodeBurst) << gub4kEntrysPerPlaneLog);
					}
					else {
						FQ->ulFEntry = ((ubCEIndex + (ubDeferredCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
					}
					FQ->ubRelativeDepth = ubDepth;
					FQ->ubDepth = 0xFF;
					FQ->btUNServed = 1;
					FQ->btCheckEmpty = 0;

					gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
					gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
					gFQI.ubFQLinkNumber++;
					gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

					while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
						M_SwitchTask();
					}

					if (FQ->btPEFail) {
						DLMC_ErrorDetect(20);
						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
						gubDLMCStep = ERROR_DETECT;

						goto Mark_End;
					}

					FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
				}

				ftlAddToSystemSetFreeTable(gubDeferredCodeSet[ubCodeBlockIndex]);

			}
		}
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
		gubDeferredCodeSet[0] = gubSysCodeSet[0];
		gubDeferredCodeSet[1] = gubSysCodeSet[1];

#else
		gubDeferredCodeSet[0] = ubNewSysCodeSet[0];
		gubDeferredCodeSet[1] = ubNewSysCodeSet[1];

#endif

		gubDLMCStep = SAVE_MICROCODE_DONE;

		ftlSwapL2PTable(guwLockReadNum, BYTE_Free_Read_Mode);

		FCONL[FCONL_FLH_FUNC] = ulFCONL_FLH_FUNC_Temp;

		if (ENABLE_DEBUG_UART_DLMC) {
			UartString("\r\nDownload new microcode, codeversion: ");
			Uart_Tx_DataHex(ubCodeVersion);
		}
	}
	//*************************************
	//6.1.Turn off Micron Randomizer
	//*************************************
	flaMicronRandomizer(0);
	//*************************************
	//7.Save InfoBlock
	//*************************************
Mark_Save_InfoBlock:

	if ((gubDLMCStep == SAVE_MICROCODE_DONE)) {
		if (gubUpdateInfoBlock) {
			UWORD uwCheck = VT->guwSystemUpdateCount;
			Vender_Direct_Write_Info(1, (U32)&guwEarlyBadCntPerPlane[0]);
			if (VT->guwSystemUpdateCount > uwCheck) {
				gubDLMCStep = SAVE_INFOBLOCK_DONE;
			}
			else { //Fail
				gubDLMCStep = ERROR_DETECT;
				DLMC_ErrorDetect(33);
			}
		}
		else {
			gubDLMCStep = SAVE_INFOBLOCK_DONE;
		}

	}
	//*************************************
	//8.Activate Microcode
	//*************************************
Mark_Activate_Microcode:

	if ((gubDLMCStep == SAVE_INFOBLOCK_DONE) && (ubMode == 0x03 || ubMode == 0x07)) {
		gubDLMCStep = GOING_TO_ACTIVATE_MICROCODE;
	}

Mark_End:

	//*************************************
	//9.Return status or Error handle
	//*************************************
	if (gubDLMCStep < ALL_TRANSFERDONE) {
		HB[HB_SECTOR_CNT] = 0x01;	//ATA device is expecting more download microcode commands to follow
	}
	else if (gubDLMCStep == GOING_TO_ACTIVATE_MICROCODE) {

#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
		if (ubMode0xF_Flag == 1) {						// cmd flow : 0xE -> 0xF.  Exist two versions FW: Unit0[AA]__Unit1[AB]. AB is new one.
			flaMicronRandomizer(1);
			ulFCONL_FLH_FUNC_Temp = FCONL[FCONL_FLH_FUNC];
#if !ENABLE_TLC_PROGRAM_CODEBLOCK2
			FCONL[FCONL_FLH_FUNC] |= RDM_REF_ROW_ADR_5_TO_0;
#endif

			// Mode E transfer Mode F was not garentee uwPlaneNum Value, Need re-Calculate uwPlaneNum.
			for (ubSectionIndex = 0; ubSectionIndex < 8; ubSectionIndex++) {
				if (gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex]) {
					//(Page number Must be in pair)
					uwPlaneNum[ubSectionIndex] = ((gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex]) / (gubSectorsPerPlane * 512));
					if (gulSectionInformation[BINHEADER_SECTION_SIZE][ubSectionIndex] % (gubSectorsPerPlane * 512)) {
						if (uwPlaneNum[ubSectionIndex] & BIT0) {
							uwPlaneNum[ubSectionIndex]++;
						}
						else {
							uwPlaneNum[ubSectionIndex] += 2;
						}
					}
					else {
						if (uwPlaneNum[ubSectionIndex] & BIT0) {
							uwPlaneNum[ubSectionIndex]++;
						}
					}
				}
				else {
					uwPlaneNum[ubSectionIndex] = 0;
				}
			}

			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				U32 ulL4K_LCA_Clone = 0;
				U32 ulL4K_FW_Clone = 0;
				//erase
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
				FQ->btD3 = 1;
#else
				FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif



#if (TLC || ENABLE_ULTRAMLC)
				// Must Send Reset CMD(0xFF) After Erase Fail
				FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;
#endif
				ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] >> gubBurstsPerBankLog);
				ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] & gubBurstsPerBankMask);

				FQ->uwFUnit = ubSysCodeUnit;
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = ((ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
				}
				FQ->ubRelativeDepth = ubDepth;
				FQ->ubDepth = 0xFF;
				FQ->btUNServed = 1;
				FQ->btCheckEmpty = 0;
#if TLC
				FQ->btD3 = 1;
#endif
				gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gFQI.ubFQLinkNumber++;
				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

				while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}

				FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

				//Code Header
				ulPlaneIndex = 0;
				ubL4KNum = 1;

				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ulRAMAddr = (U32)&ubMicroCodeBuffer[0]; //(U32)&gubZ2Buffer[0];
				FQ->btCheckEmpty = 1;
				FQ->btThisIsErasePage = 0;
				FQ->ubFJob = BYTE_FJOB_READ;
				FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);

				ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[1][ubCEIndex] >> gubBurstsPerBankLog);
				ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[1][ubCEIndex] & gubBurstsPerBankMask);

				FQ->uwFUnit = ubSysCodeUnit;			// New version FW.
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
				}
				FQ->ubDepth = ubDepth;
				FQ->ubL4kIndex = ubL4kLink;
				FQ->btUNServed = 1;
#if TLC
				FQ->btD3 = 1;
#endif
				gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
				gulPara0x24.BitMap.btBufModeEn = 0;
				gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
				gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
				gulPara0x24.BitMap.btZipBypass = 1;
				gulPara0x24.BitMap.btCmpEn = 0;
				gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gMTQ->Depth[ubDepth].ulZipMap = 0xFF;

				if (ENABLE_DCACHE) {
					xthal_dcache_region_invalidate((void*)FQ->ulRAMAddr, ubL4KNum * 4096);
				}

				for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
					//set L4K table info
					pL4KTable.ulL4K_LCA = 0x00000000;
					pL4KTable.Para0x04.ulAll = 0;
					pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
					pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
					pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
					pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
					*pL4KTablePtr = pL4KTable;
				}

				gFQI.ubFQLinkNumber++;
				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

				while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}

				FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (FQ->ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));

				ulL4K_LCA_Clone = pL4KTablePtr->ulL4K_LCA;
				ulL4K_FW_Clone = pL4KTablePtr->Para0x04.BitMap.ulL4K_FW;

				//program
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ulRAMAddr = (U32)&ubMicroCodeBuffer[0];//(U32)&gubZ2Buffer[0];
				FQ->ubFJob = BYTE_FJOB_WRITE;
				FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);

				ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] >> gubBurstsPerBankLog);
				ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] & gubBurstsPerBankMask);

				FQ->uwFUnit = ubSysCodeUnit;		// old version FW.
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
				}
				FQ->ubRelativeDepth = ubDepth;
				FQ->ubDepth = 0xFF;
				FQ->ubL4kIndex = ubL4kLink;
				FQ->btUNServed = 1;
#if TLC
				FQ->btD3 = 1;
#endif
				gulPara0x24.BitMap.ubFrameNum = gub4kEntrysPerPlane;
				gulPara0x24.BitMap.btBufModeEn = 0;
				gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
				gulPara0x24.BitMap.ubL4KNum = gub4kEntrysPerPlane;
				gulPara0x24.BitMap.btZipBypass = 1;
				gulPara0x24.BitMap.btCmpEn = 0;
				gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

				for (ubL4KCount = 0; ubL4KCount < gub4kEntrysPerPlane; ubL4KCount++) {
					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
					//set L4K table info

					pL4KTable.ulL4K_LCA = ulL4K_LCA_Clone;
					pL4KTable.Para0x04.BitMap.ulL4K_FW = ((ulL4K_FW_Clone & 0xFF00) | (0 + ubCEIndex * 2));
					pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
					pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
					pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
					pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0xAB;
					pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
					pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0x7;
					*pL4KTablePtr = pL4KTable;
				}

				gFQI.ubFQLinkNumber++;
				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

				while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}
				FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

				ulPlaneIndex++;
				ubL4KNum = gub4kEntrysPerPlane;

				//Code body
				for (ubSectionIndex = 0; ubSectionIndex < 8; ubSectionIndex++) {
					for (uwSectionPlaneIndex = 0; uwSectionPlaneIndex < uwPlaneNum[ubSectionIndex]; uwSectionPlaneIndex++) {
						//for (ulPlaneIndex = 0; ulPlaneIndex < uwPlaneNum[1]; ulPlaneIndex++) {

						M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
						FQ->ulRAMAddr = (U32)&ubMicroCodeBuffer[0];	//(U32)&gubZ2Buffer[0];
						FQ->btCheckEmpty = 1;
						FQ->btThisIsErasePage = 0;
						FQ->ubFJob = BYTE_FJOB_READ;
						FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);

						ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[1][ubCEIndex] >> gubBurstsPerBankLog);
						ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[1][ubCEIndex] & gubBurstsPerBankMask);

						FQ->uwFUnit = ubSysCodeUnit;			// New version FW.
						if (ENABLE_PLANE_CH_CE) {
							FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
						}
						else {
							FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
						}
						FQ->ubDepth = ubDepth;
						FQ->ubL4kIndex = ubL4kLink;
						FQ->btUNServed = 1;
#if TLC
						FQ->btD3 = 1;
#endif
						gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
						gulPara0x24.BitMap.btBufModeEn = 0;
						gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
						gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
						gulPara0x24.BitMap.btZipBypass = 1;
						gulPara0x24.BitMap.btCmpEn = 0;
						gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
						gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
						gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

						if (ENABLE_DCACHE) {
							xthal_dcache_region_invalidate((void*)FQ->ulRAMAddr, ubL4KNum * 4096);
						}

						for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
							pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
							//set L4K table info
							pL4KTable.ulL4K_LCA = 0x00000000;
							pL4KTable.Para0x04.ulAll = 0;
							pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
							pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
							pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
							pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
							*pL4KTablePtr = pL4KTable;
						}

						gFQI.ubFQLinkNumber++;
						gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

						while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
							M_SwitchTask();
						}

						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
						pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (FQ->ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));

						ulL4K_LCA_Clone = pL4KTablePtr->ulL4K_LCA;
						ulL4K_FW_Clone = pL4KTablePtr->Para0x04.BitMap.ulL4K_FW;

						//program
						M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
						FQ->ulRAMAddr = (U32)&ubMicroCodeBuffer[0];//(U32)&gubZ2Buffer[0];
						FQ->ubFJob = BYTE_FJOB_WRITE;
						FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);

						ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] >> gubBurstsPerBankLog);
						ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] & gubBurstsPerBankMask);

						FQ->uwFUnit = ubSysCodeUnit;		// old version FW.
						if (ENABLE_PLANE_CH_CE) {
							FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
						}
						else {
							FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
						}
						FQ->ubRelativeDepth = ubDepth;
						FQ->ubDepth = 0xFF;
						FQ->ubL4kIndex = ubL4kLink;
						FQ->btUNServed = 1;
#if TLC
						FQ->btD3 = 1;
#endif
						gulPara0x24.BitMap.ubFrameNum = gub4kEntrysPerPlane;
						gulPara0x24.BitMap.btBufModeEn = 0;
						gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
						gulPara0x24.BitMap.ubL4KNum = gub4kEntrysPerPlane;
						gulPara0x24.BitMap.btZipBypass = 1;
						gulPara0x24.BitMap.btCmpEn = 0;
						gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
						gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

						for (ubL4KCount = 0; ubL4KCount < gub4kEntrysPerPlane; ubL4KCount++) {
							pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
							//set L4K table info

							pL4KTable.ulL4K_LCA = ulL4K_LCA_Clone;
							pL4KTable.Para0x04.BitMap.ulL4K_FW = ulL4K_FW_Clone;
							pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
							pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
							pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
							pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0xAB;
							pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
							pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0x7;
							*pL4KTablePtr = pL4KTable;
						}

						gFQI.ubFQLinkNumber++;
						gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

						while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
							M_SwitchTask();
						}

						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
						ulPlaneIndex++;
					}
				}
			}

			FCONL[FCONL_FLH_FUNC] = ulFCONL_FLH_FUNC_Temp;
			//*************************************
			//Turn off Micron Randomizer
			//*************************************
			flaMicronRandomizer(0);
		}
#endif
		//以下為Micro之外的flash要erase掉原本的codeblock
		for (ubCodeBlockIndex = 0; ubCodeBlockIndex < 2; ubCodeBlockIndex++) {
#if ((MicronFlashOnly) && (MICRON_NEW_BOOTCODE == 0))
			continue;
#else
			if (gubSysCodeSet[ubCodeBlockIndex] != 0xFF) {
				for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
					FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
					FQ->btD3 = 1;
#else
					FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif

					// Must Send Reset CMD(0xFF) After Erase Fail
					FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

					ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] >> gubBurstsPerBankLog);
					ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] & gubBurstsPerBankMask);

					FQ->uwFUnit = ubSysCodeUnit;
					if (ENABLE_PLANE_CH_CE) {
						FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
					}
					else {
						FQ->ulFEntry = ((ubCEIndex + (ubSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
					}
					FQ->ubRelativeDepth = ubDepth;
					FQ->ubDepth = 0xFF;
					FQ->btUNServed = 1;
					FQ->btCheckEmpty = 0;

					gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
					gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
					gFQI.ubFQLinkNumber++;
					gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

					while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
						M_SwitchTask();
					}

					if (FQ->btPEFail) {
						DLMC_ErrorDetect(21);
						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
						gubDLMCStep = ERROR_DETECT;

						goto Mark_End;
					}

					FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
				}

				ftlAddToSystemSetFreeTable(gubSysCodeSet[ubCodeBlockIndex]);
			}
#endif
		}

		//read dummy
		while ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) {
			M_MicroCodeTrigger();
		}

		VT->guwFWUpdateCount++;
		VT->gulFTLState.B.btFlushCache = 0;//if want to save VT, need set to 0.

		if (gubDoPreformatAfterDLMC) {
			SYSL[SYS0L_SIM_CTRL0] |= (SIMREG_DLMC_PREFORMAT); //use as a tag for doing preformat after DLMC.
		}

		DLMC_InitialSetting();
		HB[HB_SECTOR_CNT] = 0x02; //ATA device has applied the new microcode

		if ((uwCmdSectorCnt != 0) && (ubMode != 0x0F)) { //non-data 0x0F mode or CmdSectorCnt == 0 mode should not check done tag
			M_CheckDoneTag();
			gulSATA_MSG_TYPE3[0].All = 0;
		}

		while (HL[HL_AXI_FIS_STAT]&BIT23);

		HB[HB_FLAG_CTRL] = ubDDT_temp;
		HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
		gubStopFTL = 0;
		gubFlushMode = BYTE_STANDBY_FLUSH;
		FlushCache();
		SYSL[SYS0L_SIM_CTRL0] |= SIMREG_CMD_D2H; //use as a tag for command D2H, but comreset D2H
		ReservedData->ubSecMode = AtaCfg.ubSecMode;
		ReservedData->ubScanVTReboot = 0;
		ReservedData->CRC = mDMAC_CRC((U32)ReservedData, 512, ReservedData_SEED, 1, (DMA_CRC_LEN_8 | DMA_CRC_DCCM));
		Vender_Isp_Jump(0);
	}
	else if (gubDLMCStep == SAVE_INFOBLOCK_DONE) {
		gubDLMCStep = 0;
		gubLastDLMCMode = 0;
		gubHeaderTransferSectorCnt = 0;
		gulBurnerTransferSectorCnt = 0;
		gulMicroCodeTransferSectorCnt = 0;
		HB[HB_SECTOR_CNT] = 0x03; //All segments of the updated microcode data have been received and saved, and the device is waiting for activation of the updated microcode data.
	}
	else if (gubDLMCStep == ERROR_DETECT) {
		DLMC_InitialSetting();

		if (ubNeedDoEraseUnitFlag) {
			for (ubCodeBlockIndex = 0; ubCodeBlockIndex < 2; ubCodeBlockIndex++) {
				if (ubNewSysCodeSet[ubCodeBlockIndex] != 0xFF) {
					for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
						M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
						FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
						FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
						FQ->btD3 = 1;
#else
						FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif

						// Must Send Reset CMD(0xFF) After Erase Fail
						FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

						ubNewSysCodeUnit = (UBYTE)(uwNewSysCodeBlock[ubCodeBlockIndex][ubCEIndex] >> gubBurstsPerBankLog);
						ubNewSysCodeBurst = (UBYTE)(uwNewSysCodeBlock[ubCodeBlockIndex][ubCEIndex] & gubBurstsPerBankMask);

						FQ->uwFUnit = ubNewSysCodeUnit;
						if (ENABLE_PLANE_CH_CE) {
							FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubNewSysCodeBurst) << gub4kEntrysPerPlaneLog);
						}
						else {
							FQ->ulFEntry = ((ubCEIndex + (ubNewSysCodeBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
						}
						FQ->ubRelativeDepth = ubDepth;
						FQ->ubDepth = 0xFF;
						FQ->btUNServed = 1;
						FQ->btCheckEmpty = 0;

						gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
						gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
						gFQI.ubFQLinkNumber++;
						gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

						while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
							M_SwitchTask();
						}

						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
					}

					if (ubNeedRemoveSet != ubNewSysCodeSet[ubCodeBlockIndex]) {
						ftlAddToSystemSetFreeTable(ubNewSysCodeSet[ubCodeBlockIndex]);
					}
				}
			}
		}

		if ((!gubHardReset) && (!gubSoftReset)) { //Reset來不需要回 DLMC cmd ABRT
			gubErrorCode = SET_ABRT;
		}

	}

	if (ubNotReceiveData && ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0))) {
		//跟WriterScetor打sync escape處理方式相同
		gubNeedChkDoneTag = 0;
		HW[HW_DATA_TRG] |= (H_CMD_ABRTREQ);
		HW[HW_ERR_CTRL] |= (H_REG_XHOLD);
		APU_Reset();//不加PPS4-4會Fail
	}
	else {
		//read dummy
		while ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) {
			M_MicroCodeTrigger();
		}
	}
	if ((uwCmdSectorCnt != 0) && (ubMode != 0x0F)) { //non-data 0x0F mode or CmdSectorCnt == 0 mode should not check done tag
		M_CheckDoneTag();
		gulSATA_MSG_TYPE3[0].All = 0;
	}

	gubStopFTL = 0;

	while (HL[HL_AXI_FIS_STAT]&BIT23);//表示當下有CMD還在做
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
#endif
}


void DLMC_InitialSetting()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

#if ENABLE_TEMP_MICROCODE_FORCE_TO_FLASH
	UBYTE ubTmpCodeUnit, ubTmpCodeBurst;

	if (gubMicroCodeTmpSet != 0xFF) {

		ubTmpCodeUnit = guwSystemBlockRemappingTable[gubMicroCodeTmpSet][0] >> gubBurstsPerBankLog;
		ubTmpCodeBurst = guwSystemBlockRemappingTable[gubMicroCodeTmpSet][0] & gubBurstsPerBankMask;
		ftl_Erase_Single_Plane((U32)ubTmpCodeUnit, ubTmpCodeBurst, 0, BIT_FJOBI_FASTPAGE);

		ftlAddToSystemSetFreeTable(gubMicroCodeTmpSet);
		gubMicroCodeTmpSet = 0xFF;
	}
#endif

	gubDLMCStep = 0;
	gubLastDLMCMode = 0;
	gubHeaderTransferSectorCnt = 0;
	gulBurnerTransferSectorCnt = 0;
	gulMicroCodeTransferSectorCnt = 0;
	gubUpdateSysInfoTransferSectorCnt = 0;
	gubDoPreformatAfterDLMC = 0;
	gulUpdateSystemInfoMap = 0;
#endif
}

void DLMC_UpdateSystemInfo(U32 ulUpdateSystemInfoMap)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UWORD uwOldSystemBlock[2];
	UBYTE ubOldSystemSet;

	UBYTE ubUnit, ubBurst;

	UBYTE *ubUpdateBuffer = gubHostTable;//use gubHostTable
	UBYTE *ubMicroCodeBuffer = (UBYTE *)gMicroCodeBuffer;

	UBYTE ubCEIndex;
	U32 uli;

	HandleStopRW(1);

	uwOldSystemBlock[0] = guwSystemBlock[0];
	uwOldSystemBlock[1] = guwSystemBlock[1];
	ubOldSystemSet = gubSystemSet;

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		ubUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
		ubBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
		if (ftlReadSinglePlane(ubUnit, 0, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_SYS,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY), 4, 0, ubBurst) == 0) {
			break;
		}
	}

	if ((ulUpdateSystemInfoMap & (BIT6 | BIT7 | BIT8 | BIT9)) == (BIT6 | BIT7 | BIT8 | BIT9)) { //DPS is updated
		ubUpdateBuffer[M_SysInfo_gubDPSTableValid] = 1;
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ubUpdateBuffer, 512 * 32);
	}

	for (uli = 0; uli < 32; uli++) {
		if (ulUpdateSystemInfoMap & (BIT0 << uli)) {
			mSE_COPY((U32)&ubMicroCodeBuffer[(0xF0000 + uli * 512)], (U32)&ubUpdateBuffer[uli * 512], 512);
		}
	}

	gubSystemSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_TAIL);
	guwSystemBlock[0] = guwSystemBlockRemappingTable[gubSystemSet][0];
	guwSystemBlock[1] = guwSystemBlockRemappingTable[gubSystemSet][1];

	gulSystemVersion = (gulSystemVersion & 0xFFFFFFF0) + ((gulSystemVersion + 1) & 0x0000000F);//ex: 0x112233 -> 0x112234, 0x11223F->0x112230

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		ubUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
		ubBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
		ftlWriteSinglePlane(ubUnit, 0, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_SYS, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubBurst);
	}

	for (uli = 1; uli <  3; uli ++) {
		ubUnit = (UBYTE)(uwOldSystemBlock[0] >> gubBurstsPerBankLog);
		ubBurst = (UBYTE)(uwOldSystemBlock[0] & gubBurstsPerBankMask);
		ftlReadSinglePlane(ubUnit, uli, 0, (U32)&ubUpdateBuffer[0], MARK_DPST,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubBurst);
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
			ubUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
			ubBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
			ftlWriteSinglePlane(ubUnit, uli, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_DPST, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubBurst);
		}
	}

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		ubUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
		ubBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
		for (uli = 3; uli <  guwFastPagePagesPerUnit; uli ++) {
			ftlWriteSinglePlane(ubUnit, uli, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_SYS, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubBurst);
		}
		UBYTE ubFQIndex = gFQI.ubFQLinkFree;
		FlashQueue_t *FQ;
		UBYTE ubDepth, ubL4kLink;
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
		FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE);

		// Must Send Reset CMD(0xFF) After Erase Fail
		FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

		ubUnit = (UBYTE)(uwOldSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
		ubBurst = (UBYTE)(uwOldSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
		FQ->uwFUnit = ubUnit;
		if (ENABLE_PLANE_CH_CE) {
			FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubBurst) << gub4kEntrysPerPlaneLog);
		}
		else {
			FQ->ulFEntry = (((ubCEIndex & gubPlanesPerBurstMask) + (ubBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
		}
		FQ->ubRelativeDepth = ubDepth;
		FQ->ubDepth = 0xFF;
		FQ->btUNServed = 1;
		FQ->btCheckEmpty = 0;
		gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

		while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

		/*
		if (gFQI.gFQLink[ubFQIndex].btPEFail) {
			ftlPreformatShowError(ubCEIndex, ubOldSystemUnit, 0x42);
		}
		*/
	}

	ftlAddToSystemSetFreeTable(ubOldSystemSet);
	VT->guwSystemUpdateCount++;
	gubStopFTL = 0;

#endif
}

void SDR_RW_DMA(void)
{
#if(!RDT)
	//RMA_FLOW
	if (gubIsWriteProtect & (BIT_ERR_UNIT_WRITEPROTECT | BIT_FLASH_INIT_FAIL_WRITEPROTECT)) {
	}
	else {
		M_CheckNonBurnerModeToDisableFunction();
	}

	U32 ulLBA;
	U32 ulStartAddr;
	U32 ulEndAddr;
	U32 ulSDRSize;
	U32 HL_BUF_BASE_Temp = HL[HL_BUF_BASE];
	U32 HL_BUF_W_EP_Temp = HL[HL_BUF_W_EP];
	U32 HL_BUF_W_SHF_Temp = HL[HL_BUF_W_SHF];
	U32 HL_BUF_R_EP_Temp = HL[HL_BUF_R_EP];
	U32 HL_BUF_R_SHF_Temp = HL[HL_BUF_R_SHF];
	U32 HL_BUF_OFST_Temp = HL[HL_BUF_OFST];
	UWORD uwMSG_temp = (HW[HW_SYS_CTRL] & (H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
	UBYTE ubTag = 0;

	HW[HW_SYS_CTRL] &= (~(H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
	ulLBA = ((HB[HB_DEVICE] & 0x0F) << 24) | (HB[HB_LBA_H] << 16) | (HB[HB_LBA_M]) << 8 | (HB[HB_LBA_L]);
	ulStartAddr = (1 + ((SYS1L[SYS1L_DZ2_DSA] + 0x80000) >> 12)) << 12; //可以測z2的起始位址，需4k align
	ulEndAddr = (SYS1L[SYS1L_DZ2_DSA] + (SYS1L[SYS1L_DZ2_DLEN] / 36 * 32) ) >> 12 << 12;
	ulSDRSize = ulEndAddr - ulStartAddr;
	if (gubNCQDoing) {
		ubTag = (UBYTE)((HL[HL_RD_NCQ_INFO] >> 20) & 0x3F);
	}
	HW[HW_SECTOR_CNT] = gulSectorCnt;
	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	HL[HL_BUF_BASE] = ulStartAddr >> 12;
	HL[HL_BUF_R_EP] = ulSDRSize >> 12;
	HL[HL_BUF_W_EP] = ulSDRSize >> 12;
	HL[HL_BUF_R_SHF] = 0;
	HL[HL_BUF_W_SHF] = 0;
	HL[HL_BUF_OFST] = ulLBA;

	HL[HL_TQ_CONTENT] = ubTag << 18;
	while (HL[HL_CMD_REMAIN_SEC]);
	while (HL[HL_AXI_FIS_STAT]&BIT23);

	HL[HL_BUF_BASE] = HL_BUF_BASE_Temp;
	HL[HL_BUF_W_EP] = HL_BUF_W_EP_Temp;
	HL[HL_BUF_W_SHF] = HL_BUF_W_SHF_Temp;
	HL[HL_BUF_R_EP] = HL_BUF_R_EP_Temp;
	HL[HL_BUF_R_SHF] = HL_BUF_R_SHF_Temp;
	HL[HL_BUF_OFST] = HL_BUF_OFST_Temp;
	HW[HW_SYS_CTRL] |= uwMSG_temp;
#endif
}

