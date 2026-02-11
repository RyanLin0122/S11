#include "setup.h"
#include "Vardef.h"
#include "ftl.h"
#include "CMDTBL.H"

#include <IO.h>
#include <string.h>
#include <stdlib.h>

void InitSanitize(UBYTE ubMode)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (((AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN) && (ubMode == 1))
	        || (AtaCfg.ubSanitizeState == SD4_SANITIZE_OPERATION_SUCCESS)
	        || ((AtaCfg.ubSanitizeState == SD3_SANITIZE_OPERATION_FAIL) && (ubMode == 2))) {

		//SD0
		AtaCfg.ubSanitizeState = SD0_SANITIZE_IDLE;
		AtaCfg.ubSanitizeValue &= ~(SET_SANITIZE_FAILURE_MODE_POLICY_VALUE);
	}

	if (ubMode == 1) {
		//power on reset/comreset with SSP disable
		AtaCfg.ubSanitizeValue &= ~(SET_SANITIZE_ANTIFREEZE_VALUE);
	}

	if (AtaCfg.ubSanitizeState == SD2_SANITIZE_IN_PROGRESS) {
		//need not consider SD3_SANITIZE_OPERATION_FAIL or SD4_SANITIZE_OPERATION_SUCCESS here, because it has become SD0_SANITIZE_IDLE.

		M_DisableHWD2H_NCQ();
	}
}

void Sanitize()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubStateTemp = AtaCfg.ubSanitizeState;
	UBYTE ubValueTemp = AtaCfg.ubSanitizeValue;

	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect && ((HW[HW_FEATURE] == 0x0000) || (HW[HW_FEATURE] == 0x0014) || (HW[HW_FEATURE] == 0x0020) || (HW[HW_FEATURE] == 0x0040))) {
			gubErrorCode = SET_ABRT;
			return;
		}
	}

	switch (HW[HW_FEATURE]) {
		case 0x0000:
			Sanitize_Status();
			break;
		case 0x0011:
			//crypto scramble
			gubErrorCode = SET_ABRT;
			HB[HB_LBA_L] = 0x02;//error reason: inalid or unsupported value in the Sanitize Device FEATURE field.
			break;
		case 0x0012:
			//block erase
			gubErrorCode = SET_ABRT;
			HB[HB_LBA_L] = 0x02;//error reason: inalid or unsupported value in the Sanitize Device FEATURE field.
			break;
		case 0x0014:
			Sanitize_Overwrite();
			break;
		case 0x0020:
			Sanitize_Freeze();
			break;
		case 0x0040:
			Sanitize_AntiFreeze();
			break;
		default:
			gubErrorCode = SET_ABRT;
			HB[HB_LBA_L] = 0x02;//error reason: inalid or unsupported value in the Sanitize Device FEATURE field.
			break;
	}

	if ((ubStateTemp != AtaCfg.ubSanitizeState) || (ubValueTemp != AtaCfg.ubSanitizeValue)) {
		SaveAtaConfig();
	}
#endif
}

void Sanitize_Freeze()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (((IDT.W59 & BIT12) != BIT12)
	        || (gubLBA48BitCmdSupport == 0)
	        || (HL[HL_LBA_L] != 0x46724C6B)
	        || (AtaCfg.ubSanitizeValue & SET_SANITIZE_ANTIFREEZE_VALUE)
	        || (AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) //SD1_SANITIZE_FROZEN need abort, and SD2,SD3,SD4 also need abort due to 4.16.6
	        || ((AtaCfg.ubSecMode & SET_SEC_LOCK) && ((AtaCfg.ubSanitizeValue & SET_RESTRICTED_SANITIZE_OVERRIDES_SECURITY) == 0))) {

		gubErrorCode = SET_ABRT;
		if (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN) {
			HB[HB_LBA_L] = 0x03;//error reason: Device is in the SD1
		}
		else if (AtaCfg.ubSanitizeValue & SET_SANITIZE_ANTIFREEZE_VALUE) {
			HB[HB_LBA_L] = 0x04;//error reason: SANITIZE FREEZE LOCK command failed as a result of the Sanitize Antifreeze Lock value being set to one.
		}
		else {
			HB[HB_LBA_L] = 0x00;//error reason: reason not reported or sanitize device command failed.
		}

		return;
	}

	AtaCfg.ubSanitizeState = SD1_SANITIZE_FROZEN;
	Sanitize_NormalOutput();
#endif
}

void Sanitize_AntiFreeze()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (((IDT.W59 & (BIT10 | BIT12)) != (BIT10 | BIT12))
	        || (gubLBA48BitCmdSupport == 0)
	        || (HL[HL_LBA_L] != 0x416E7469)
	        || (AtaCfg.ubSanitizeValue & SET_SANITIZE_ANTIFREEZE_VALUE)
	        || (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN)
	        || ((AtaCfg.ubSecMode & SET_SEC_LOCK) && ((AtaCfg.ubSanitizeValue & SET_RESTRICTED_SANITIZE_OVERRIDES_SECURITY) == 0))) {

		gubErrorCode = SET_ABRT;
		if (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN) {
			HB[HB_LBA_L] = 0x03;//error reason: Device is in the SD1
		}
		else {
			HB[HB_LBA_L] = 0x00;//error reason: reason not reported or sanitize device command failed.
		}

		return;
	}

	AtaCfg.ubSanitizeValue |= SET_SANITIZE_ANTIFREEZE_VALUE;
	Sanitize_NormalOutput();
#endif
}

void Sanitize_Status()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubClearSanitizeOperationFailedBit;

	if (HW[HW_SECTOR_CNT] & BIT0) {
		ubClearSanitizeOperationFailedBit = 1;
	}
	else {
		ubClearSanitizeOperationFailedBit = 0;
	}

	if (((IDT.W59 & BIT12) != BIT12)
	        || (gubLBA48BitCmdSupport == 0)
	        || ((ubClearSanitizeOperationFailedBit == 1) && ((AtaCfg.ubSanitizeValue & SET_SANITIZE_FAILURE_MODE_POLICY_VALUE) == 0))) {

		gubErrorCode = SET_ABRT;
		HB[HB_LBA_L] = 0x00;//error reason: reason not reported or sanitize device command failed.
		return;
	}

	if (AtaCfg.ubSanitizeState == SD3_SANITIZE_OPERATION_FAIL) {
		if (ubClearSanitizeOperationFailedBit == 1) {
			//Here the Failure Mode Policy value is non-zero
			InitSanitize(2); //SD3:SD0
		}
		else {
			gubErrorCode = SET_ABRT;
			//error reason: Sanitize command unsuccessful - the sanitize operation completed with
			//physical sectors that are available to be allocated for user data that not successfully sanitized.
			HB[HB_LBA_L] = 0x01;
			return;
		}

	}
	else if (AtaCfg.ubSanitizeState == SD4_SANITIZE_OPERATION_SUCCESS) {
		InitSanitize(0); //SD4:SD0
	}
	else {
	}

	Sanitize_NormalOutput();
#endif
}

void Sanitize_Overwrite()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubOverwriteCount;
	UBYTE ubFailureBit;

	if (HW[HW_SECTOR_CNT] & BIT4) {
		ubFailureBit = 1;
	}
	else {
		ubFailureBit = 0;
	}

	if (((IDT.W59 & (BIT12 | BIT14)) != (BIT12 | BIT14))
	        || (gubLBA48BitCmdSupport == 0)
	        || (!(HB[HB_LBA_M_EXP] == 0x57 && HB[HB_LBA_H_EXP] == 0x4F ))
	        || (!((gubDeferredCodeSet[0] == 0xFF) && (gubDeferredCodeSet[1] == 0xFF)))
	        || ((AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN) || (AtaCfg.ubSanitizeState == SD2_SANITIZE_IN_PROGRESS))
	        || (((AtaCfg.ubSanitizeState == SD3_SANITIZE_OPERATION_FAIL) || (AtaCfg.ubSanitizeState == SD4_SANITIZE_OPERATION_SUCCESS)) && ((AtaCfg.ubSanitizeValue & SET_SANITIZE_FAILURE_MODE_POLICY_VALUE) == 0) && ubFailureBit)
	        || ((AtaCfg.ubSecMode & SET_SEC_LOCK) && ((AtaCfg.ubSanitizeValue & SET_RESTRICTED_SANITIZE_OVERRIDES_SECURITY) == 0))
	        || ((AtaCfg.ubSecMode & SET_SEC_LOCK) && ((AtaCfg.ubSanitizeValue & SET_RESTRICTED_SANITIZE_OVERRIDES_SECURITY) && ubFailureBit))) {

		gubErrorCode = SET_ABRT;

		if (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN) {
			HB[HB_LBA_L] = 0x03;//error reason: Device is in the SD1
		}
		else {
			HB[HB_LBA_L] = 0x00;//error reason: reason not reported or sanitize device command failed.
		}

		return;
	}

	if (AtaCfg.ubSanitizeState == SD0_SANITIZE_IDLE) {
		if (ubFailureBit) { //is an indication of how the failure mode bit was set in sanitize operation command that caused the transition SD0:SD2.
			AtaCfg.ubSanitizeValue |= SET_SANITIZE_FAILURE_MODE_POLICY_VALUE;
		}
		else {
			AtaCfg.ubSanitizeValue &= ~SET_SANITIZE_FAILURE_MODE_POLICY_VALUE;
		}
	}

	AtaCfg.ubSanitizeState = SD2_SANITIZE_IN_PROGRESS;
	M_DisableHWD2H_NCQ();

	AtaCfg.ubSanitizeValue &= ~SET_SANITIZE_OPERATION_COMPLETED_WITHOUT_ERROR_VALUE;//cleared to zero when sanitize device state machine transitions to the SD2.

	AtaCfg.gSanitizeOverwrite.ulPattern = HL[HL_LBA_L];

	if (HW[HW_SECTOR_CNT] & BIT7) {
		AtaCfg.gSanitizeOverwrite.ubNeedInvert = 1;
	}
	else {
		AtaCfg.gSanitizeOverwrite.ubNeedInvert = 0;
	}

	ubOverwriteCount = (HW[HW_SECTOR_CNT] & 0x000F);
	if (ubOverwriteCount == 0) {
		ubOverwriteCount = 16;
	}

	AtaCfg.gSanitizeOverwrite.ubCurrentCount = 0;
	AtaCfg.gSanitizeOverwrite.ubEndCount = ubOverwriteCount;

	Sanitize_NormalOutput();
}

void Sanitize_Overwrite_Offline()
{
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	if (AtaCfg.ubSanitizeState == SD2_SANITIZE_IN_PROGRESS) {
		U32 ulEndLBA;
		U32 ulPattern;
		BufferQueue_t *BQ;
		UBYTE ub4kIndex;
		UBYTE ub4kNum;
		U32 ubTotalSectors;
		UWORD uwBufferPTR;
		U32 ulBuffer;
		U32 ulDataMask;

		HandleStopRW(0);
		M_SetFlagSettingNone();

		if (ENABLE_FAST_SANITIZE_TEST) {
			ulEndLBA = FAST_SANITIZE_SIZE - 1;
		}
		else {
			ulEndLBA = gulMediaSize.All - 1;
		}

		ulPattern = AtaCfg.gSanitizeOverwrite.ulPattern;
		uwBufferPTR = 256;
		ulBuffer = SRAM_BASE + uwBufferPTR * 512;

		M_DebugGPIO8_ON_LED();
		while ((AtaCfg.gSanitizeOverwrite.ubCurrentCount < AtaCfg.gSanitizeOverwrite.ubEndCount)
		        && (!( HB[HB_REMAIN_VLD_NCQ_NUM] || gubHostCmdIn || gubHardReset || gubSoftReset))
		        && (AtaCfg.gSanitizeOverwrite.ubSuspend == 0)) {

			if ((gulSanitizeCurrentLBA + gubSectorsPerPlane) <= (ulEndLBA + 1)) {
				ub4kNum = gub4kEntrysPerPlane;
				ulDataMask = 0xFFFFFFFF;
			}
			else {
				ub4kNum = (ulEndLBA  - gulSanitizeCurrentLBA) / 8 + 1;
				ulDataMask = (BIT0 << (ub4kNum * 8)) - 1;
			}

			if (ENABLE_DEBUG_UART_ATA_CMD) {
				UartString("\r\nS Cnt: ");
				Uart_Tx_DataHex(AtaCfg.gSanitizeOverwrite.ubCurrentCount);
				UartString(" S LBA: ");
				UartLongHex(gulSanitizeCurrentLBA);
			}

			if (AtaCfg.gSanitizeOverwrite.ubNeedInvert) {
				if (AtaCfg.gSanitizeOverwrite.ubCurrentCount % 2) {
					mDMAC_SV(4, ulBuffer, ub4kNum * 4096, gulSanitizeCurrentLBA, ~ulPattern, 0, DMA_EN_SETCQ, 1);
				}
				else {
					mDMAC_SV(4, ulBuffer, ub4kNum * 4096, gulSanitizeCurrentLBA, ulPattern, 0, DMA_EN_SETCQ, 1);
				}
			}
			else {
				mDMAC_SV(4, ulBuffer, ub4kNum * 4096, gulSanitizeCurrentLBA, ulPattern, 0, DMA_EN_SETCQ, 1);
			}

			while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
			gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
			BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
			BQ->btWrite = 1;
			BQ->ulVir4kIndex = gulSanitizeCurrentLBA / 8;
			BQ->ubDataInDRamMap = 0;
			BQ->ulDataMask = ulDataMask;
			BQ->uwBufferIndex = uwBufferPTR;

			ubTotalSectors = 0;
			for (ub4kIndex = 0; ub4kIndex < ub4kNum; ub4kIndex++) {
				BQ->ubZcode[ub4kIndex] = 7;
				ubTotalSectors += 7;
			}
			BQ->btSpecialData = 0;
			BQ->btNeedCheckSpecialData = 0;
			BQ->ulSectorCnt = ubTotalSectors;
			BQ->btNeedLoadAlign = 0;
			BQ->btFlushWBQ = 0;
			BQ->ub4kNum = ub4kNum;
			BQ->ulSpareValid = 0;
			BQ->ubThisBQisBG = 1;
			M_AddToBufferQueue(gBQI.ubLinkActive);
			gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
			while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
				M_SwitchTask();
			}
			gulSanitizeCurrentLBA += gubSectorsPerPlane;
			if (gulSanitizeCurrentLBA > ulEndLBA) {
				gulSanitizeCurrentLBA = 0;
				AtaCfg.gSanitizeOverwrite.ubCurrentCount++;
				SaveAtaConfig();
				break;
			}
		}
		M_DebugGPIO8_OFF_LED();

		if (AtaCfg.gSanitizeOverwrite.ubCurrentCount == AtaCfg.gSanitizeOverwrite.ubEndCount) {
			//sanitize done
			if ((!gubIsWriteProtect) && (AtaCfg.gSanitizeOverwrite.ubCurrentCount >= 2)) {
				//success
				AtaCfg.ubSanitizeValue |= SET_SANITIZE_OPERATION_COMPLETED_WITHOUT_ERROR_VALUE;
				AtaCfg.ubSanitizeState = SD4_SANITIZE_OPERATION_SUCCESS;
				M_DisableHWD2H_NCQ();

				if (AtaCfg.ubSanitizeValue & SET_RESTRICTED_SANITIZE_OVERRIDES_SECURITY) {
					AtaCfg.ubSecMode &= ~(SET_SEC_EN | SET_SEC_LOCK | SET_SEC_MAX);
				}

				//The device shall clear the pseudo uncorrectable error and the flagged uncorrectable error of all logical sectors, if:
				//a) an OVERWRITE EXT command (see 7.33.4) returns command complete without an error; or
				//b) a BLOCK ERASE EXT command (see 7.33.2) returns command complete without an error.

				//wait write unc
			}
			else {
				//fail
				AtaCfg.ubSanitizeState = SD3_SANITIZE_OPERATION_FAIL;
				M_DisableHWD2H_NCQ();
			}
			SaveAtaConfig();
		}

		mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 0);//強迫DMAC 全部做完
		M_SetFlagSettingSHF();
	}
}

void Sanitize_NormalOutput()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	UWORD uwTemp;
	U32 ulMediaSizeTemp;

	//15 SANITIZE OPERATION COMPLETED WITHOUT ERROR bit – the contents of the Sanitize
	//Operation Completed Without Error value (see 4.16.7)
	//14 1 = the device is in the SD2: Sanitize Operation In Progress state (see 4.16.10.4)
	//0 = the device is not in the SD2: Sanitize Operation In Progress state
	//13 1 = the device is in the SD1: Sanitize Frozen state (see 4.16.10.3)
	//0 = the device is not in the SD1: Sanitize Frozen state
	//12 SANITIZE ANTIFREEZE bit – the contents of the Sanitize Antifreeze value (see 4.16.9)

	uwTemp = 0;
	if (AtaCfg.ubSanitizeValue & SET_SANITIZE_ANTIFREEZE_VALUE) {
		uwTemp |= BIT12;
	}

	if (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN) {
		uwTemp |= BIT13;
	}

	if (AtaCfg.ubSanitizeState == SD2_SANITIZE_IN_PROGRESS) {
		uwTemp |= BIT14;
	}

	if (AtaCfg.ubSanitizeValue & SET_SANITIZE_OPERATION_COMPLETED_WITHOUT_ERROR_VALUE) {
		uwTemp |= BIT15;
	}

	HW[HW_SECTOR_CNT] = uwTemp;
	if (AtaCfg.ubSanitizeState == SD2_SANITIZE_IN_PROGRESS) {
		if (ENABLE_FAST_SANITIZE_TEST) {
			ulMediaSizeTemp = FAST_SANITIZE_SIZE;
		}
		else {
			ulMediaSizeTemp = gulMediaSize.All;
		}

		HL[HL_LBA_L] = (U32)((ULLONG)65536 * ((ULLONG)ulMediaSizeTemp * AtaCfg.gSanitizeOverwrite.ubCurrentCount + gulSanitizeCurrentLBA) / ((ULLONG)ulMediaSizeTemp * AtaCfg.gSanitizeOverwrite.ubEndCount));
	}
	else {
		HL[HL_LBA_L] = 0xFFFF;
	}
#endif
}
