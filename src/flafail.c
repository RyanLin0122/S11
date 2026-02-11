
#include "Setup.h"
#include "Vardef.h"
#include "FTL.h"
#include "IO.h"

#include <string.h>
#include <stdlib.h>
// Get LUN address
#if ENABLE_iFSA
#define GET_LUN_ADDR(uliFSA)		(guoFlashUseType.B.btMultiDie ? ((uliFSA >> (gubDieBlockShiftLog + 16)) & 3) : 0)	//16=>block-FSA bit shift; 3=>(BIT0|BIT1);
#else
#define GET_LUN_ADDR(pFSA_B4)		((pFSA_B4 & BIT3) >> 3)
#endif

#define HB_FEATURE_FAIL_LIMIT		10			// for BiCs2 D5/D4 command

//PAGE TYPE
#define LOWER_PAGE 	1
#define MIDDLE_PAGE	2
#define UPPER_PAGE	3
#define EXTRA_PAGE	4

//SOFTBIT DATA
#define SOFTBIT_READ_HB_DATA 				0
#define SOFTBIT_SB1_DATA					1
#define SOFTBIT_SB2_DATA					2
#define SOFTBIT_SB3_DATA					3
#define SOFTBIT_LI_DATA					4
#define SOFTBIT_READ_HB_PLUS_1_DELTA_VTH_DATA 	5
#define SOFTBIT_READ_HB_SUB_1_DELTA_VTH_DATA 	6
#define SOFTBIT_READ_HB_PLUS_2_DELTA_VTH_DATA 	7
#define SOFTBIT_READ_HB_SUB_2_DELTA_VTH_DATA 	8
#define SOFTBIT_READ_HB_PLUS_3_DELTA_VTH_DATA 	9
#define SOFTBIT_READ_HB_SUB_3_DELTA_VTH_DATA 	10
#define SOFTBIT_READ_LI_DATA 				11
#define SOFTBIT_SEND_ALL_0_DATA                  12
#define SOFTBIT_READ_HB_DATA_MIDDLE_PAGE			14
#define IBF_BANK_LENGTH					5120

#define SOFTBIT_COMMAND_30                           0x00
#define SOFTBIT_COMMAND_3C                           0x01
#define SOFTBIT_COMMAND_A2_30                      0x02

#define SOFTBIT_DATAOUT_NO                           0x00
#define SOFTBIT_DATAOUT_YES                          0x01

UBYTE gubVoltage_A;
UBYTE gubVoltage_B;
UBYTE gubVoltage_C;
UBYTE gubVoltage_D;
UBYTE gubVoltage_E;
UBYTE gubCenterRank[2];
UBYTE gubCenterSet;



void flaCheckChannelFPUBusy(UBYTE ubgFQLinkIndex, UBYTE ubEnterSerialNumber, volatile REG_t *CheckChannelFREG, UBYTE ubFPUType)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	//volatile REG_t *CheckChannelFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubCheckChannel * 0x200));

	while ((CheckChannelFREG[FCTLL_FPU_TRIG].L & (TIMEOUT | SIGNOFF_BUSY | FPU_TRIG)) != 0) {
		if ((CheckChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
			VT->gulFPUTimeoutHandleDebugCnt++;
			if (ENABLE_DEBUG_CHECK_CHANNEL_FPU_TIMEOUT_UART) {
				UartString(" NFPUTO: ");
				Uart_Tx_DataHex(ubEnterSerialNumber);
			}
			ftlRecordFailLog(FLHErrLogType, ubEnterSerialNumber, ubgFQLinkIndex, BYTE_FPU_TIMEOUT, DONTCARE, ubFPUType);

			CheckChannelFREG[FCTLL_INT_RDY].L |= 0xFF;
			idle(0x1999998);
			// Clear all MTQ in channel
			CheckChannelFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
			CheckChannelFREG[FCTLL_MT_TRIG].B.B1 = 0;
			idle(0x1999998);

			U32 ulBak1 = CheckChannelFREG[FCTLL_MTQ_CFG].L;
			U32 ulBak2 = CheckChannelFREG[FCTLL_UNC_CFG].L;
			U32 ulBak3 = CheckChannelFREG[FCTLL_INT_CFG].L;
			U32 ulBak4 = CheckChannelFREG[FCTLL_RS_CFG].L;
			U32 ulBak5 = CheckChannelFREG[FCTLL_CHNL_SET].L;
			U32 ulBak6 = CheckChannelFREG[FCTLL_ZIP_CFG].L;
			U32 ulBak7 = CheckChannelFREG[FCTLL_DMA_CFG].L;
			U32 ulBak8 = CheckChannelFREG[FCTLL_CMP_CFG].L;
			U32 ulBak9 = CheckChannelFREG[FCTLL_FSA_SEL].L;
			U32 ulBak10 = CheckChannelFREG[FCTLL_SEED_INIT].L;
			U32 ulBak11 = CheckChannelFREG[FCTLL_OTHER_SET].L;
			U32 ulBak12 = CheckChannelFREG[FCTLL_INT_VCT].L;
			U32 ulBak13 = CheckChannelFREG[FCTLL_ZIP_MAP].L;
			U32 ulBak14 = CheckChannelFREG[FCTLL_IFSA0].L;
			U32 ulBak15 = CheckChannelFREG[FCTLL_L4K_SPR_ADR].L;
			U32 ulBak16 = CheckChannelFREG[FCTLL_IBF_CTL].L;
			U32 ulBak17 = CheckChannelFREG[FCTLL_CNT_ONE].L;
			U32 ulBak18 = CheckChannelFREG[FCTLL_RAW_DMA_ADR].L;
			U32 ulBak19 = CheckChannelFREG[FCTLL_FLH_SET].L;
			U32 ulBak20 = CheckChannelFREG[FCTLL_FDIV_CFG].L;

			U32 ulBak21 = FCONL[FCONL_SGN_MSK];
			U32 ulBak22 = FCONL[FCONL_TGL_TIME_CFG];
			U32 ulBak23 = FCONL[ FCONL_TGL_TIME_CFG_1];
			U32 ulBak24 = FCONL[FCONL_CRC_EN];
			U32 ulBak25 = FCONL[FCONL_FCE_ENB];

			gubFPUTimeout = 1;
			flaFIPTimeoutErrorHandle(FQ->ubCEIndex);
			if ((U32)(*&CheckChannelFREG) == (U32)(Flash_Reg_Base + FIP1)) {
				FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH1_EN);
			}
			else {
				FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH0_EN);
			}

			CheckChannelFREG[FCTLL_MTQ_CFG].L = ulBak1;
			CheckChannelFREG[FCTLL_UNC_CFG].L = ulBak2;
			CheckChannelFREG[FCTLL_INT_CFG].L = ulBak3;
			CheckChannelFREG[FCTLL_RS_CFG].L = ulBak4;
			CheckChannelFREG[FCTLL_CHNL_SET].L = ulBak5;
			CheckChannelFREG[FCTLL_ZIP_CFG].L = ulBak6;
			CheckChannelFREG[FCTLL_DMA_CFG].L = ulBak7;
			CheckChannelFREG[FCTLL_CMP_CFG].L = ulBak8;
			CheckChannelFREG[FCTLL_FSA_SEL].L = ulBak9;
			CheckChannelFREG[FCTLL_SEED_INIT].L = ulBak10;
			CheckChannelFREG[FCTLL_OTHER_SET].L = ulBak11;
			CheckChannelFREG[FCTLL_INT_VCT].L = ulBak12;
			CheckChannelFREG[FCTLL_ZIP_MAP].L = ulBak13;
			CheckChannelFREG[FCTLL_IFSA0].L = ulBak14;
			CheckChannelFREG[FCTLL_L4K_SPR_ADR].L = ulBak15;
			CheckChannelFREG[FCTLL_IBF_CTL].L = ulBak16;
			CheckChannelFREG[FCTLL_CNT_ONE].L = ulBak17;
			CheckChannelFREG[FCTLL_RAW_DMA_ADR].L = ulBak18;
			CheckChannelFREG[FCTLL_FLH_SET].L = ulBak19;
			CheckChannelFREG[FCTLL_FDIV_CFG].L = ulBak20;
			FCONL[FCONL_SGN_MSK] = ulBak21;
			FCONL[FCONL_TGL_TIME_CFG] = ulBak22;
			FCONL[FCONL_TGL_TIME_CFG_1] = ulBak23;
			FCONL[FCONL_CRC_EN] = ulBak24;
			FCONL[FCONL_FCE_ENB] = ulBak25;

			if (ENABLE_DEBUG_CHECK_CHANNEL_FPU_TIMEOUT_UART) {
				UartString("\nTrig: ");
				UartLongHex(CheckChannelFREG[FCTLL_FPU_TRIG].L);
			}
			break;
		}
	}
}

void flaFIPTimeoutErrorHandle(UBYTE ubCEIndex)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubTimeoutChannel = ubCEIndex & gubPlanesPerBurstMask;
	volatile REG_t *NonUncChannelFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (((ubTimeoutChannel + 1) & 1) * 0x200));
	U32 ulStartTime = gulOperationTime;

	// 另一個channel等到作完或MT stop為止
	while ((NonUncChannelFREG[FCTLL_MTQ_INF_0].L != 0) || (NonUncChannelFREG[FCTLL_MTQ_INF_1].L != 0) || ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (TIMEOUT | SIGNOFF_BUSY | FPU_TRIG)) != 0)) {
		xthal_dcache_line_invalidate(&gulOperationTime);
		if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) || (NonUncChannelFREG[FCTLL_INT_INF].L & INTR_BUSY) ||
		        (((gulOperationTime >= ulStartTime) && ((gulOperationTime - ulStartTime) > AUTOPOL_TIMEOUT_MILLISEC)) || ((gulOperationTime < ulStartTime) && ((0xFFFFFFFF - ulStartTime + gulOperationTime) > AUTOPOL_TIMEOUT_MILLISEC)))) {
			if (ENABLE_DEBUG_UART_FIP_RESET_IN_REBUILD_FQ) {
				UartString("\nAE");
			}
			VT->gulTimeoutHandle_ResetFIPButTwoChannelAllTimeout_DebugCnt++;
			break;
		}
	}

	UBYTE ubNeedToClearForceEmpty = 0;

	if ((NonUncChannelFREG[FCTLL_MTQ_CFG].L & FORCE_EMPTY_ALL) != FORCE_EMPTY_ALL) {
		ubNeedToClearForceEmpty = 1;

		// abort all channel for FW_RST
		NonUncChannelFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);
		while ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) { // already push MT also sure to done
			if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
				break;
			}
		}

		// 20151006 Wait pre load MT done for no INT in C70 POL bug
		UBYTE ubDummyRead = RSB[RSB_NMODE_IDX_0];
		ubDummyRead = RSB[RSB_NMODE_IDX_1];
		ubDummyRead = RSB[RSB_NMODE_IDX_2];
		ubDummyRead = RSB[RSB_NMODE_IDX_3];
		ubDummyRead = RSB[RSB_NMODE_IDX_4];
		while ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) { // pre load MT also sure to done
			if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
				break;
			}
		}

		// wait the channel true idle when force empty
		NonUncChannelFREG[FCTLL_DBG_INF].L = (0x0C);
		while ((GET_DBG_MTQ_STS(NonUncChannelFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(NonUncChannelFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2)) {
			if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
				break;
			}
		}
	}

	//FCONL[FCONL_FW_RESET] = 0x7FFCFCFC;
	//FCONL[FCONL_FW_RESET] = 0xFFFFFFFF;


	if (ubTimeoutChannel == 0) {
		FCONL[FCONL_FW_RESET] = 0xFFFEFEFE;
		FCONL[FCONL_FW_RESET] = 0xFFFFFFFF;
	}
	else {
		FCONL[FCONL_FW_RESET] = 0xFFFDFDFD;
		FCONL[FCONL_FW_RESET] = 0xFFFFFFFF;
	}

	//ZIP
	FCONL[FCONL_FW_RESET] = 0x7FFFFFFF;
	FCONL[FCONL_FW_RESET] = 0xFFFFFFFF;


	///0. Flash registers
	InitFlashRegister(0); // Set MTP and LLR table in flash IRAM
	if (gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
	{
		FCONL[FCONL_TMR_CTL] &= ~(TIMER_CPU_INTR_EN | TIMER_INTR_EN);
	}

	UBYTE ubi;
	for (ubi = 0; ubi < (MAX_CE_PER_CH * MAX_CHANNEL); ubi++) {
		FCONB_REMAPPING_CE[ubi] = gubReMappingCE[ubi];
	}

	FALLL[FCTLL_CNT_ONE] |= COUNT_MODE_EN;
	FALLL[FCTLL_CNT_ONE] &= (~COUNT_ONE_MODE);

	if (gubFlashMode == FLH_MODE_TOGGLE2) {
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
		flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
		FALLL[FCTLL_RESERVE] |= DETECT_DQSB_ENABLE;
		//       pFlaReg[FCTLL_IO_SET].L |= DIFFERENTIAL_IO_TYPE;
	}
	else if (gubFlashMode == FLH_MODE_TOGGLE1) {
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);
		//       pFlaReg[FCTLL_IO_SET].L &= SINGLE_END_IO_TYPE;
	}
	else if (gubFlashMode == FLH_MODE_ONFI_NVDDR2) {
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
		flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
		FALLL[FCTLL_RESERVE] |= DETECT_DQSB_ENABLE;
	}
	else if (gubFlashMode == FLH_MODE_ONFI_NVDDR) {
		FALLL[FCTLL_FLH_SET] |= FLA_ONFI_MODE;
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);
	}
	else {
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);
		//       pFlaReg[FCTLL_IO_SET].L &= SINGLE_END_IO_TYPE;
	}

	if (ENABLE_AC_TIMING_SETTING) {
		ACTiming_Setting(gubFlashClock, 0);
	}
	UBYTE ubDiePerChannel = gubDieNumber * (gubCENumber / gubPlanesPerBurst);

	if (((ubDiePerChannel >= 16) && ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz)))
	        || ((ubDiePerChannel >= 8) && (gubFlashClock == FLH_CLK_266MHz))
	        || (guoFlashDefaultType.B.bt8LC && ((ubDiePerChannel >= 8) && ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz))))
	        || (gODTSetting.ubODTEnable)) {
		FALLL[FCTLL_DYNAMIC_ODT] |= DYNAMIC_ODT_EN; //SET BIT0 of FCTLL_DYNAMIC_ODT 時才會開啟ODT(FlashIP自動決定什麼時候開啟 例如ToggleRead時)
	}

	/// Flash dll tracking
	flaDLLTracking(0, 1, 0); //read
	flaDLLTracking(1, 1, 0); //write
	if (ASIC && ENABLE_HIGH_SPEED) {
		fla_Set_Offset_DQS_Delay(gubFlashClock);        //Only Works with MP info setting
	}
	if (ubNeedToClearForceEmpty) {
		NonUncChannelFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);
	}
	idle(0x1999998);
}

UBYTE flaCheckMTFirstReadFailL4KDuetoFakeSignoffMaskFailorNot(UBYTE ubgFQLinkIndex)
{
	// return 1 if it's fake signoffmask fail, otherwise is real
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);

	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubIsFakeSignoffMaskFail = 1;

	// Get UNC fail map
	if (FQ->btUncCheckInRSTwoCh) {
		gulGetUNCFrameMapInMT = (1 << (FQ->ubFirstUncFrameNoMapInRSTwoCh));
	}
	else {
		gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
		gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(UNC_MAP);
		gulGetUNCFrameMapInMT = gFREG[FCTLL_MAP_INF].L;
	}

	UBYTE ubFailL4KNo = 0;
	UBYTE ubFailFrameNoInMT = 0;
	M_Get_Error_4K_Index(ubFailFrameNoInMT, gub4kEntrysPerPlane, gulGetUNCFrameMapInMT);

	ubFailL4KNo = flaReadFailRetryGetL4KFailNo(ubgFQLinkIndex, ubFailFrameNoInMT);


	// Get SignoffMask fail map
	UBYTE ubSignoffMaskFailL4KIndex = 0;
	U32 ulSignoffMaskFailFrameMapInMT = 0;
	if (FQ->btUncCheckInRSTwoCh) {
		ulSignoffMaskFailFrameMapInMT = FQ->ulFIPError.All;
	}
	else {
		gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
		gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(SIGNOFFMASK_MAP);
		ulSignoffMaskFailFrameMapInMT = gFREG[FCTLL_MAP_INF].L;
	}
	M_Get_Error_4K_Index(ubSignoffMaskFailL4KIndex, (gubMaxZipRate * gub4kEntrysPerPlane), ulSignoffMaskFailFrameMapInMT); // max: 16個L4K

	if (ubSignoffMaskFailL4KIndex < ubFailL4KNo) {
		// is real signoffmask fail
		ubIsFakeSignoffMaskFail = 0;
	}

	return ubIsFakeSignoffMaskFail;
}

void flaRebuildFQPhase(UBYTE ubgFQLinkIndex)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
#if (!BURNER)
#if (!RDT)
	// force empty to avoid program parity clear interrupt vector enable bit bug
	gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);

#if FORCE_AUTOPOL_TIMEOUT
	gulRandomValue = getIntRandValue(FORCE_AUTOPOL_TIMEOUT_DENOMINATOR);
	if (gulRandomValue < (FORCE_AUTOPOL_TIMEOUT_NUMERATOR)) {
		// to simulate FPU always busy
		goto Mark_ResetFIP;
	}
#endif
#if ENABLE_FIP_RESET_IN_PROGRAM_TIMEOUT
	U32 ulStartTime = gulOperationTime;
#endif
	// wait the channel idle for read retry
	while ((gFREG[FCTLL_FPU_TRIG].L & (TIMEOUT | SIGNOFF_BUSY | FPU_TRIG)) != 0) {

#if ENABLE_FIP_RESET_IN_PROGRAM_TIMEOUT
		xthal_dcache_line_invalidate(&gulOperationTime);
		// FPU is always busy... or Timeout occur...
		if (ENABLE_FIP_RESET_IN_PROGRAM_TIMEOUT &&
		        ((((gulOperationTime >= ulStartTime) && ((gulOperationTime - ulStartTime) > AUTOPOL_TIMEOUT_MILLISEC)) || ((gulOperationTime < ulStartTime) && ((0xFFFFFFFF - ulStartTime + gulOperationTime) > AUTOPOL_TIMEOUT_MILLISEC)))
		         || ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT))) {
		Mark_ResetFIP:
			if (ENABLE_DEBUG_UART_FIP_RESET_IN_PROGRAM_TIMEOUT || UART_AUTOPOL_TIMEOUT_HANDLING) {
				if (gubTimeoutEnterRebuildFQ) {
					UartString("\nTimeout! ");
				}
				else {
					UartString("\nLack8Byte! ");
				}
			}

			//UBYTE ubTimeoutChannel = (gFQI.gFQLink[ubgFQLinkIndex].ubCEIndex & gubPlanesPerBurstMask);

			gFREG[FCTLL_INT_RDY].L |= 0xFF;
			idle(0x1999998);
			// Clear all MTQ in channel
			gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
			gFREG[FCTLL_MT_TRIG].B.B1 = 0;
			idle(0x1999998);

			if (ENABLE_DEBUG_UART_FIP_RESET_IN_PROGRAM_TIMEOUT) {
				UartString("\nFW_RST3! ");
			}
			VT->gulTimeoutHandle_Rebuild_DebugCnt++;
			flaFIPTimeoutErrorHandle(gFQI.gFQLink[ubgFQLinkIndex].ubCEIndex);
		}
#endif
	}

	// wait the channel true idle when force empty
	gFREG[FCTLL_DBG_INF].L = (0x0C);
	while ((GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));

	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix 2nd program parity hang on bug

	gubEnterRS = 0;
	gubEnterSB = 0;
	gulReadFail_4KMAP = 0;
	gubRSclearMTQRebuildFQ = 0;

	// Clear all MTQ in channel
	gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
	gFREG[FCTLL_MT_TRIG].B.B1 = 0;

	if (!ENABLE_DEBUG_RESUME_STA_ERROR_BUG) {
		flaReadFailRetryForceIRDYtoReady(ubgFQLinkIndex);
	}

	gFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
	gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);

	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
		gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
	}

	// UDMA target must redo whether it's error or non-error, and we don't want to change the order of FQ, so we redo all those FQ in same channel
	flaReadFailRetryRebuildFQPhase(ubgFQLinkIndex);


	// debug INT Abort
	UWORD uwDepIndex = 0;
	for (uwDepIndex = 0; uwDepIndex < G_MT_QUEUE_DEPTH; uwDepIndex++) {
		if (gubINTTable[uwDepIndex].B.btAbort_Done) {
			if ((gMTQ->Depth[uwDepIndex].Para0x1C.BitMap.ubCEValue & gubPlanesPerBurstMask) == (gFQI.gFQLink[ubgFQLinkIndex].ubCEIndex & gubPlanesPerBurstMask)) {
				gubAbortDoneDepth = uwDepIndex;
				VT->gulAbortDoneAfterReBuildFQ_DebugCnt++;
			}
		}
	}

#endif
#endif
}

/*
	guwRUTSummaryLog[ ].B.uwUnit :		D3值.
	guwRUTSummaryLog[ ].B.ubUnitD1Link :	D1值. ( 位於gubD1LinkTable[ ]裡的第幾個Unit )
	gulRUTLog[ ].B.uwUnit :			位於guwRUT[ ][ ]裡的第幾個Unit.
*/

void flaMarkFail(UBYTE ubgFQLinkIndex, UBYTE ubFailPhase, UBYTE ubDiffBurstBank) // for program/erase FQ, ubDiffBurstBank代表實際fail的FQ跟傳入的ubgFQLinkIndex差幾個burstbank
{
#if (!RDT)
#if(!BURNER)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

#if DISABLE_FLAMARKFAIL==1
	return ;
#endif

	if (!ENABLE_RUT_SWAP) {
		M_CheckNonBurnerModeToDisableFunction();
	}
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubBurstBank;
	UWORD ubDiffFEntry;
	UWORD uwFailTargetPtr;
	U32 ulFailFEntry;

	U32 ulFEntry = FQ->ulFEntry;
	UBYTE ubTempD1LinkUnit = Default_D1_Unit;
	RUTLog_t ulRUTLogTmp;
	UWORD uwRUTLogIndex;

	M_AssertError(ubDiffBurstBank >= (2 * gubBurstsPerBank));

	if (guwRUTLogNum >= (RUT_LOG_NUM - (2 * gubCENumber * gubBurstsPerBank))) {	// 考慮erase unit一次會推CE Number個FQ, CE Number個FQ且是multi-plane erase, 另外再多留一倍空間存
		if ((FQ->uwFJobInfo & BIT_FJOBI_FAILHANDLE_RIGHTNOW) == 0) { // 後面的RUTLog只保留給BIT_FJOBI_FAILHANDLE_RIGHTNOW的FQ
			return; // 超過就不記了
		}
		else if (guwRUTLogNum >= RUT_LOG_NUM) {
			ftlRecordFailLog(FLHErrLogType, VT_RUTLOG_FULL_0xBF, DONTCARE, FQ->ubL4kIndex, DONTCARE, DONTCARE);
			return;
		}
	}

	if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {	// system area的unit進來不記
		return;
	}

	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask;
		if ((FQ->ubFJob == BYTE_FJOB_WRITE) && (ubDiffBurstBank > ubBurstBank)) {
			ubDiffFEntry = guw4kEntrysPerPage - (gub4kEntrysPerPlane * (gubBurstsPerBank - 1));	// 因為跨到previous page
			ubDiffFEntry += ((ubDiffBurstBank - 1) * gub4kEntrysPerPlane);	// 一個跨previous page的Entries + 剩下的DiffFEntry
		}
		else {
			ubDiffFEntry = ubDiffBurstBank * gub4kEntrysPerPlane;
		}
	}
	else {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog) & gubBurstsPerBankMask;
		if ((FQ->ubFJob == BYTE_FJOB_WRITE) && (ubDiffBurstBank > ubBurstBank)) {
			ubDiffFEntry = guw4kEntrysPerPage - (gub4kEntrysPerBurst * (gubBurstsPerBank - 1));	// 因為跨到previous page
			ubDiffFEntry += ((ubDiffBurstBank - 1) * gub4kEntrysPerBurst);	// 一個跨previous page的Entries + 剩下的DiffFEntry
		}
		else {
			ubDiffFEntry = ubDiffBurstBank * gub4kEntrysPerBurst;
		}
	}

	if (FQ->ubFJob == BYTE_FJOB_ERASE_UNIT) {	// erase unit送進來planeA的FEntry, 往後加到正在fail的FEntry
		ulFailFEntry = ulFEntry + ubDiffFEntry;
	}
	else {	// 其他job送進來的是後面plane的FEntry	// 往前減成正在fail的FEntry
		ulFailFEntry = ulFEntry - ubDiffFEntry;
	}

	uwFailTargetPtr = ulFailFEntry >> gub4kEntrysPerPlaneLog;

	if ((guwRUTSummaryLogNum >= (RUT_LOGINFO_NUM - 2)) && (FQ->btDirectHandling == 0)) {
		return ; //保留最後一個給 ftlDirectHandleFailUnit ,避免0x8F
	}

	M_AssertError(guwRUTLogNum >= RUT_LOG_NUM);

	if (guwRUTSummaryLogNum < RUT_LOGINFO_NUM) {

#if ENABLE_DEBUG_D1TOD3_ERR_HANDLE
		if ((guwRUTSummaryLogNum > 3) && ((FQ->uwFJobInfo & BIT_FJOBI_FAILHANDLE_RIGHTNOW) == 0) && (VT->gulFTLState.B.btDoingFlushD1 == 0)) {
			return; // 超過就不記了
		}
#endif


		//3 ============ 處理guwSummaryLog[ ] ======================
		guwRUTSummaryLogIndex = 0;
		while (1) {
			//[CC]: Erase Target 時，有可能是 D1 Unit 但還沒建完 D1 Link，所以若為 Erase 以 btD3 為準

				{
				if (guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btIsD1Unit != 0) {		//進來的是D3、但Log記的是D1.
					guwRUTSummaryLogIndex++;
					continue;
				}
				else {	//MLC FW or TLC D3 Cases.								//進來的是D3、Log[index]記的也是D3.
					if (FQ->uwFUnit != guwRUTSummaryLog[guwRUTSummaryLogIndex].B.uwUnit) {
						if (guwRUTSummaryLog[guwRUTSummaryLogIndex].B.uwUnit == 0xFFFF) {
							guwRUTSummaryLog[guwRUTSummaryLogIndex].B.uwUnit = FQ->uwFUnit;
							guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btHandleLogDone = 0;
							guwRUTSummaryLog[guwRUTSummaryLogIndex].B.ubUnitD1Link = Default_D1_Unit;
							if ((FQ->ubFJob == BYTE_FJOB_ERASE_BLOCK) || (FQ->ubFJob == BYTE_FJOB_ERASE_UNIT)) {
								guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btNeedCopyData = 0;	// Add free也會作清為0
							}
							else {
								guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btNeedCopyData = 1;
							}
							guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btIsD1Unit = 0;
							guwRUTSummaryLogNum++;

							if (ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING && FQ->FQPara.B.btRWTableUnit && (FQ->ubFJob == BYTE_FJOB_WRITE)
							        && (uwFailTargetPtr >= (gubTableHeadInfoPlaneCnt + gubSaveParityCntInSaveHeaderInfo))) {	// 發生program fail後要修改summary log為table unit program fail state
								guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btNeedCopyActiveTableUnit = 1;
							}

							UBYTE ubDie;
							for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
								if ((FQ->ubFJob == BYTE_FJOB_WRITE) && (VT->guwDieStartSFUnitIndex[ubDie] == FQ->uwFUnit)) {
									gubProgFailAtCopyUnit = 0xA2;
								}
							}
						}
						else {
							guwRUTSummaryLogIndex++;
							continue;
						}
					}
					else {
						if (ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING && FQ->FQPara.B.btRWTableUnit && (FQ->ubFJob == BYTE_FJOB_WRITE)
						        && (uwFailTargetPtr >= (gubTableHeadInfoPlaneCnt + gubSaveParityCntInSaveHeaderInfo))) {	// 若此table unit本來有read move/UNC, 則發生program fail後要修改summary log為table unit program fail state
							guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btNeedCopyActiveTableUnit = 1;
						}
						break;
					}
				}
			}
			break;
		}

		if (FQ->FQPara.B.btRWTableUnit) {
			guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btIsTableUnit	 = 1;
		}
		else {
			guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btIsTableUnit	 = 0;
		}

		if (FQ->uwFJobInfo & BIT_FJOBI_FAILHANDLE_RIGHTNOW) {	// 需要馬上處理RUT的FQ若fail (all erase FQ跟某些特殊的write FQ), 會拉起btSpecialCase
			if (ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING && FQ->FQPara.B.btRWTableUnit && (FQ->ubFJob == BYTE_FJOB_WRITE)
			        && (uwFailTargetPtr >= (gubTableHeadInfoPlaneCnt + gubSaveParityCntInSaveHeaderInfo))) {	// < (gubTableHeadInfoPlaneCnt + gubSaveParityCntInSaveHeaderInfo)的是直接重寫, 不需要copydata
				guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btSpecialCase = 0; 	// 需要將active table unit的資料搬到新的table unit, 所以不走special case flow
			}
			else {
				guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btSpecialCase = 1;
				guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btNeedCopyData = 0; // for specail case program
			}
		}
		else {
			guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btSpecialCase = 0;
		}
#if ENABLE_TABLE_VERIFY
		if (FQ->FQPara.B.btRWTableUnit && FQ->btTableVerify) {  //btNeedCopyActiveTableUnit TableVerify
			guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btNeedCopyActiveTableUnit = 1;
		}
#endif

#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
		if (ubFailPhase == AUTOPOL_TIMEOUT_FAIL) {
			guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btAutoPolTimeout = 1;
		}
#endif
		//3 ============ 處理gulRUTLog[ ] ======================
		ulRUTLogTmp.All = 0;
		ulRUTLogTmp.B.ubCEIndex = FQ->ubCEIndex;

		if (ENABLE_PLANE_CH_CE) {
			ubBurstBank = (ulFailFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask;
		}
		else {
			ubBurstBank = (ulFailFEntry >> gub4kEntrysPerBurstLog) & gubBurstsPerBankMask;
		}

		ulRUTLogTmp.B.ubBurstBank = ubBurstBank;

		if (ENABLE_DEBUG_UART) {
			UWORD uwFBlock;
			UWORD uwFPage;
			UBYTE ubPhyPlane;

			if ((TLC == 0) || (FQ->btD3 || (ubTempD1LinkUnit == Default_D1_Unit))) {
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex, (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
			}
			else {
				UBYTE ubTemp = ((FQ->ubFJob == BYTE_FJOB_ERASE_BLOCK) || (FQ->ubFJob == BYTE_FJOB_ERASE_UNIT)) ? (UBYTE)(FQ->uwFUnit) : ubTempD1LinkUnit;
				uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex, ((guwFWTotalUnitNum + (ubTemp * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
			}

			if (gubRUTMixPlaneEnable == 0) {
				ubPhyPlane = ubBurstBank;
			}
			else {
				ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
				uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
			}

			if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
				if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
					uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
				}
			}

			M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);

			if (guoFlashUseType.B.btFourPlane) {
				uwFBlock <<= 2;
				uwFBlock |= ubPhyPlane;
			}
			else {
				uwFBlock <<= 1;
				uwFBlock |= ubPhyPlane;
			}

			uwFPage = M_Divider(ulFailFEntry, DEF_4KEntryPerPage_Mcl_Div);

			UartString ("\nLaterBad:");
			if (FQ->ubFJob == BYTE_FJOB_READ) {
				UartString (" ReadFail");
			}
			else if (FQ->ubFJob == BYTE_FJOB_WRITE) {
				UartString (" ProgramFail");
			}
			else {
				UartString (" EraseFail");
			}
			UartString (" CE:");
			Uart_Tx_DataHex (FQ->ubCEIndex);
			UartString (" FBlock:");
			UartWordHex (uwFBlock);
			UartString (" FPage:");
			UartWordHex (uwFPage);
		}

		if ((ubFailPhase == 1) || (ubFailPhase == 0xFF)) {	// data HB fail, SB pass才算read move (fail phase 1), table unit只要進 HB 就算read move (fail phase 1 or 0xFF)
			ulRUTLogTmp.B.btNeedMarkBad = 0;
			FQ->btReadMove = 1;
		}
		else {	// 有進 RS就算bad blk (fail phase 0 or 2)
			ulRUTLogTmp.B.btNeedMarkBad = 1;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
			if (guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btAutoPolTimeout == 1) {
				ulRUTLogTmp.B.btNeedMarkBad = 0;	// autopol timeout unit is NOT bad block (even enter RS)
				FQ->btReadMove = 1;
			}
#endif
		}
		ulRUTLogTmp.B.ubFJob = FQ->ubFJob;
		ulRUTLogTmp.B.ubFailPhase = ubFailPhase;

		if (guwRUTSummaryLog[guwRUTSummaryLogIndex].B.btIsD1Unit == 0) {
			ulRUTLogTmp.B.uwUnit =  guwRUTSummaryLog[guwRUTSummaryLogIndex].B.uwUnit;
			ulRUTLogTmp.B.ubD1UnitIndex = 0;
			ulRUTLogTmp.B.btIsD1Unit = 0;
		}
		for (uwRUTLogIndex = 0; uwRUTLogIndex < guwRUTLogIndex; uwRUTLogIndex++) {
			//ignore btNeedMarkBad value
			if ((ulRUTLogTmp.All & 0xFF7FFFFF) == (gulRUTLog[uwRUTLogIndex].All & 0xFF7FFFFF)) {
				//btNeedMarkBad has higher priority
				gulRUTLog[uwRUTLogIndex].All |= ulRUTLogTmp.All;
				break;
			}
		}

		if (uwRUTLogIndex == guwRUTLogIndex) {
			gulRUTLog[uwRUTLogIndex].All = ulRUTLogTmp.All;
			guwRUTLogIndex++;
			guwRUTLogNum++;
			guwRUTSummaryLog[guwRUTSummaryLogIndex].B.uwRUTLogCnt++;
		}

		if ((FQ->ubFJob == BYTE_FJOB_WRITE) && FQ->FQPara.B.btRWTableUnit) {
			guwLastPFTableUnit = FQ->uwFUnit;
			guwLastPFTableTargetPtr = M_Divider((ulFailFEntry >> gub4kEntrysPerPlaneLog), DEF_PlanesPerPage_Mcl_Div);
			guoTableProgramFail_BM |= ((ULLONG)BIT0 << (M_Modular((ulFailFEntry >> gub4kEntrysPerPlaneLog), DEF_PlanesPerPage_Mod_Mask)));
		}
	}
#endif
#endif
}

void flaCheckWriteStatus (UBYTE ubgFQLinkIndex)
{

	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubCEIndex = FQ->ubCEIndex;
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	UBYTE ubBurstBank, ubFailBurstBank;
	UBYTE ubStatus;
	UBYTE ubStatus_cache;
	UBYTE ubMarkFailMap = 0;
	UBYTE ubNeedToClearForceEmpty = 0;

	M_VirtualToPhysicalCE(ubCEIndex);

	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = (FQ->ulFEntry  >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask;
	}
	else {
		ubBurstBank = (FQ->ulFEntry  >> gub4kEntrysPerBurstLog) & gubBurstsPerBankMask;
	}


	if ((gFREG[FCTLL_MTQ_CFG].L & FORCE_EMPTY_ALL) != FORCE_EMPTY_ALL) {
		// Only Stop MT when it is NOT force empty status, and prevent to resume MT too early.
		ubNeedToClearForceEmpty = 1;

		// force empty to avoid program parity clear interrupt vector enable bit bug
		gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);
	}

	// wait the channel idle for read retry
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	// wait the channel true idle when force empty
	gFREG[FCTLL_DBG_INF].L = (0x0C);
	while ((GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));

	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix 2nd program parity hang on bug

	gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
	gFREG[FCTLL_INT_VCT].L |= IntVector(FQ->ubDepth); // fix the next pre-load MT being incorrect recycled bug

	FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		//Only Set current channel
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] =   M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}

	// Wait flash interface ready, always check
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x1_FPU_POL_CheckWriteStatus, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}
	//gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
	//while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);

	// MT preload + FPU trig = Auto clear CE, so it need to set CE again

	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		//Only Set current channel
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] =   M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}


	if (FQ->ubCheckStatusPhase != STATUS_CHECK_ONE_FQ) {

	Mark_Check_PlaneA_PlaneB_Program:
	//MLC
		//Get Status

		gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		gFREG[FCTLL_PIO_CMD].L = 0x71;
		UWORD uwCnt;
		for (uwCnt = 0; uwCnt < 5; uwCnt++) {	// 1000, 500, 200, 150 140 ok, 120 fail => 100 ok
			// need dummy delay tWHR (add a dummy read register)。
			ubStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;	// for dealy tWHR
		}
		ubStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;

		if (ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) {
			UBYTE ubFailMode;	// 0: 不造fail, 1: 造bit1,0 fail, 2: 造bit2,0 fail, 3: 造bit2,1,0 fail

			// planeA, B是否造fail
			ubFailMode = getIntRandValue(3);
			//if (guoFlashUseType.B.btTwoPlane) { // 2-plane sample至少要造planeA/B其中一個fail, 4-plane sample 可以不造planeA/B fail
			ubFailMode++;
			//}

			if (ubFailMode == 1) {
				ubStatus |= (BIT1 | BIT0);	// planeA fail
			}
			if ((FQ->ubCheckStatusPhase != STATUS_CHECK_ON_CEW_2PLANE_DMA_CONTINUOUS)
			        && (FQ->ubCheckStatusPhase != STATUS_CHECK_ON_CEW_4PLANE_DMA5_CONTINUOUS) ) {
				if (ubFailMode == 2) {
					ubStatus |= (BIT2 | BIT0);	// planeB fail
				}
				if (ubFailMode == 3) {
					ubStatus |= (BIT2 | BIT1 | BIT0);	// planeA/B fail
				}
			}
			else {
				ubStatus |= (BIT1 | BIT0);	// planeA fail
			}
		}

		if ( ((ubStatus & BIT5) && (ubStatus & BIT0)) ||
		        ((ubStatus & BIT6) &&   ((ubStatus & BIT3) || (ubStatus & BIT4))) ) {
			if (((ubStatus & BIT5) && (ubStatus & BIT0))) {
				if (ubStatus & BIT1) {	// planeA fail
					ubFailBurstBank = 0;
					flaMarkFail(ubgFQLinkIndex, 0, ubBurstBank - ubFailBurstBank);
					ubMarkFailMap |= BIT0;
				}
				if (ubStatus & BIT2) {	// planeB fail
					if ((FQ->ubCheckStatusPhase != STATUS_CHECK_ON_CEW_2PLANE_DMA_CONTINUOUS)
					        && (FQ->ubCheckStatusPhase != STATUS_CHECK_ON_CEW_4PLANE_DMA5_CONTINUOUS) ) {
						ubFailBurstBank = 1;
						flaMarkFail(ubgFQLinkIndex, 0, ubBurstBank - ubFailBurstBank);
						ubMarkFailMap |= BIT1;
					}
				}
				gubPlaneABDebug = 1;	// PlaneA/B已經有發生error
			}

			//if (FQ->ubCheckStatusPhase >= STATUS_CHECK_ON_CEW_2PLANE_DMA_CONTINUOUS)  {
			if (FQ->ubCheckStatusPhase >= STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START)  {

				if (ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) {
					UBYTE ubFailMode;	// 0: 不造fail, 1: 造bit1,0 fail, 2: 造bit2,0 fail, 3: 造bit2,1,0 fail

					// previous planeA, B是否造fail
					ubFailMode = getIntRandValue(3);
					//if (guoFlashUseType.B.btTwoPlane) { // 2-plane sample至少要造planeA/B其中一個fail, 4-plane sample 可以不造planeA/B fail
					ubFailMode++;
					//}

					if (ubFailMode == 1) {
						ubStatus |= (BIT3 | BIT0);	// previous planeA fail
					}

					if (ubFailMode == 2) {
						ubStatus |= (BIT4 | BIT0);	// previous planeB fail
					}
					if (ubFailMode == 3) {
						ubStatus |= (BIT4 | BIT3 | BIT0);	// previous planeA/B fail
					}
				}

				if ( (ubStatus & BIT6) && ((ubStatus & BIT3) || (ubStatus & BIT4)) ) {
					if (ubStatus & BIT3) {	// previous planeA fail
						ubFailBurstBank =  0;
						if (FQ->ubCheckStatusPhase == STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START) {
							flaMarkFail(ubgFQLinkIndex, 0, ubBurstBank - ubFailBurstBank);
						}
						else {
							flaMarkFail(ubgFQLinkIndex, 0, (ubBurstBank + gubBurstsPerBank) - ubFailBurstBank);	// + gubBurstsPerBank代表錯在前一個page
						}
						ubMarkFailMap |= BIT2;
					}
					if (ubStatus & BIT4) {	// previous planeB fail
						ubFailBurstBank = 1;
						if (FQ->ubCheckStatusPhase == STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START) {
							flaMarkFail(ubgFQLinkIndex, 0, ubBurstBank - ubFailBurstBank);
						}
						else {
							flaMarkFail(ubgFQLinkIndex, 0, (ubBurstBank + gubBurstsPerBank) - ubFailBurstBank);	// + gubBurstsPerBank代表錯在前一個page
						}
						ubMarkFailMap |= BIT3;
					}
				}

			}
		}
		if (guoFlashUseType.B.btFourPlane) {

			if (FQ->ubCheckStatusPhase >= STATUS_CHECK_ON_CEW_4PLANE_DMA3_CONTINUOUS) {
			Mark_Check_PlaneC_PlaneD_Program:
				//Get Status

				gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
				gFREG[FCTLL_PIO_CMD].L = 0x73;
				UWORD uwCnt;
				for (uwCnt = 0; uwCnt < 5; uwCnt++) {	// 1000, 500, 200, 150 140 ok, 120 fail => 100 ok
					// need dummy delay tWHR (add a dummy read register)。
					ubStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;	// for dealy tWHR
				}
				ubStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;

				if (ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) {
					UBYTE ubFailMode;	// 0: 不造fail, 1: 造bit1,0 fail, 2: 造bit2,0 fail, 3: 造bit2,1,0 fail

					// planeA, B是否造fail
					ubFailMode = getIntRandValue(3);
					//if (guoFlashUseType.B.btTwoPlane) { // 2-plane sample至少要造planeA/B其中一個fail, 4-plane sample 可以不造planeA/B fail
					ubFailMode++;
					//}

					if (ubFailMode == 1) {
						ubStatus |= (BIT1 | BIT0);	// planeC fail
					}
					if (FQ->ubCheckStatusPhase != STATUS_CHECK_ON_CEW_4PLANE_DMA3_CONTINUOUS) {
						if (ubFailMode == 2) {
							ubStatus |= (BIT2 | BIT0);	// planeD fail
						}
						if (ubFailMode == 3) {
							ubStatus |= (BIT2 | BIT1 | BIT0);	// planeC/D fail
						}
					}
					else {
						ubStatus |= (BIT1 | BIT0);	// planeC fail
					}
				}

				if ( ((ubStatus & BIT5) && (ubStatus & BIT0)) ||
				        ((ubStatus & BIT6) &&   ((ubStatus & BIT3) || (ubStatus & BIT4))) ) {
					if (((ubStatus & BIT5) && (ubStatus & BIT0))) {
						if (ubStatus & BIT1) {
							/*
							if(FQ->ubCheckStatusPhase == STATUS_CHECK_ON_CEW_4PLANE_DMA5_CONTINUOUS) {
								// plane A fail
								ubFailBurstBank = 0;
								ubMarkFailMap |= BIT0;
							}else
							*/
							{
								// planeC fail
								ubFailBurstBank = 2;
								ubMarkFailMap |= BIT4;
							}
							if (ubBurstBank < ubFailBurstBank) { // ex 寫到Page N的planeA/B時發覺Page N-1的plane C fail
								flaMarkFail(ubgFQLinkIndex, 0, (ubBurstBank + gubBurstsPerBank) - ubFailBurstBank);
							}
							else {
								flaMarkFail(ubgFQLinkIndex, 0, ubBurstBank - ubFailBurstBank);
							}

						}
						if (ubStatus & BIT2) {
							if (FQ->ubCheckStatusPhase != STATUS_CHECK_ON_CEW_4PLANE_DMA3_CONTINUOUS) {
								/*
								if(FQ->ubCheckStatusPhase == STATUS_CHECK_ON_CEW_4PLANE_DMA6_CONTINUOUS) {
									// plane B fail
									ubFailBurstBank = 1;
									ubMarkFailMap |= BIT1;
								}else
								*/
								{
									// planeD fail
									ubFailBurstBank = 3;
									ubMarkFailMap |= BIT5;
								}
								if (ubBurstBank < ubFailBurstBank) {	// ex 寫到Page N的planeA/B時發覺Page N-1的plane D fail
									flaMarkFail(ubgFQLinkIndex, 0, (ubBurstBank + gubBurstsPerBank) - ubFailBurstBank);
								}
								else {
									flaMarkFail(ubgFQLinkIndex, 0, ubBurstBank - ubFailBurstBank);
								}

							}
						}
					}

					//if (FQ->ubCheckStatusPhase >= STATUS_CHECK_ON_CEW_4PLANE_DMA5_CONTINUOUS ) {
					if (FQ->ubCheckStatusPhase >= STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START ) {

						if (ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) {
							UBYTE ubFailMode;	// 0: 不造fail, 1: 造bit1,0 fail, 2: 造bit2,0 fail, 3: 造bit2,1,0 fail

							// previous planeC, D是否造fail
							ubFailMode = getIntRandValue(3);
							//if (guoFlashUseType.B.btTwoPlane) { // 2-plane sample至少要造planeA/B其中一個fail, 4-plane sample 可以不造planeA/B fail
							ubFailMode++;
							//}

							if (ubFailMode == 1) {
								ubStatus |= (BIT3 | BIT0);	// previous planeC fail
							}

							if (ubFailMode == 2) {
								ubStatus |= (BIT4 | BIT0);	// previous planeD fail
							}
							if (ubFailMode == 3) {
								ubStatus |= (BIT4 | BIT3 | BIT0);	// previous planeC/D fail
							}
						}

						if ( (ubStatus & BIT6) && ((ubStatus & BIT3) || (ubStatus & BIT4)) ) {
							if (ubStatus & BIT3) {
								/*
								if(FQ->ubCheckStatusPhase == STATUS_CHECK_ON_CEW_4PLANE_DMA5_CONTINUOUS) {
									// previous plane A fail
									ubFailBurstBank =  0;
									ubMarkFailMap |= BIT2;
								}else */
								{
									// previous planeC fail
									ubFailBurstBank =  2;
									ubMarkFailMap |= BIT6;
								}
								if (FQ->ubCheckStatusPhase == STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START) {
									flaMarkFail(ubgFQLinkIndex, 0, ubBurstBank - ubFailBurstBank);
								}
								else {
									flaMarkFail(ubgFQLinkIndex, 0, (ubBurstBank + gubBurstsPerBank) - ubFailBurstBank);	// + gubBurstsPerBank代表錯在前一個page
								}

							}
							if (ubStatus & BIT4) {
								/*
								if(FQ->ubCheckStatusPhase == STATUS_CHECK_ON_CEW_4PLANE_DMA6_CONTINUOUS) {
									// previous plane B fail
									ubFailBurstBank = 1;
									ubMarkFailMap |= BIT3;
								}else */
								{
									// previous planeD fail
									ubFailBurstBank = 3;
									ubMarkFailMap |= BIT7;
								}
								if (FQ->ubCheckStatusPhase == STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START) {
									flaMarkFail(ubgFQLinkIndex, 0, ubBurstBank - ubFailBurstBank);
								}
								else {
									flaMarkFail(ubgFQLinkIndex, 0, (ubBurstBank + gubBurstsPerBank) - ubFailBurstBank);	// + gubBurstsPerBank代表錯在前一個page
								}

							}
						}

					}
				}
			}
		}

	}
	else {
		// STATUS_CHECK_ONE_FQ 的 FQ 是Single Plane Program, 所以錯的是自己不用 Poll
		flaMarkFail(ubgFQLinkIndex, 0, 0);
		ubMarkFailMap = 0xFF;
	}

	//走進這條路是program status mismatch
	if (ubMarkFailMap == 0) {
		FQ->btPEFail = 0;
		VT->gulProgramMismatchCnt++;
		ftlRecordFailLog(FLHErrLogType, VT_PROGRAM_STATUS_MISMATCH_0xB1 , FQ->ubL4kIndex , ubStatus_cache , FQ->FQPara.B.btRWTableUnit, ubStatus);

		// MLC
		VT->gulProgramFailCnt_D3--;
	}
	//2 Send Reset CMD After Program Fail, Except D3 Suspend Mode

	if (B0KB && (FQ->ubD3WriteMode & D3_SUSPEND_EN) ) {
	}
	else {

		gFREG[FCTLL_FPU_ENTRY].L = (FPU_PTR_CFF);
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		for (gulTemp = 0; gulTemp < 10; gulTemp++) {
		}

		// Polling TRUE RDY
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
			flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x2_FPU_POL_CheckWriteStatus, gFREG, CHECK_FPU_POL_TIMEOUT);
		}
		else {
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		}
	}



	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
	}
	else {
		FCONL[FCONL_FCE_CLR] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Disable selected CE
	}

	gubINTTable[FQ->ubDepth].B.btAll_Done = 1;
	gubINTTable[FQ->ubDepth].B.btPartial_Done = 0;//preload next reuse mt (read action) will make partial done = 1 all done = 0 removeit

	gFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
	if (ubNeedToClearForceEmpty) {
		//gFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
		gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);//把模擬狀態復原
	}

	M_AssertError(ubMarkFailMap == 0);
}

void ftlRecordFailLog(UBYTE ubLogType, UBYTE ubAbnormalLoc, UBYTE ubgFQLinkIndex, UBYTE ubFailType, UBYTE ubTableSel, UBYTE ubFLHErrorMap)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubBurstBank;
	UBYTE ubError;
	UBYTE ubTimeoutLogPTR = VT->gubTimeoutLogPTR + VTLogEntryNum - VTTimeoutLogEntryNum;//shift
	volatile unsigned char ubTemperature = 0;

Mark_GetTemperature:

	//while (((*((volatile unsigned char*)0x04000311)) & 0x01) == 0);
	ubTemperature = ( (*((volatile unsigned char*)0x04000312)) & 0x1F  ); ///Get value
	//*((volatile unsigned char*)0x04000310) |= 0x40;    /// disable thermometer


	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = (FQ->ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask;
	}
	else {
		ubBurstBank = (FQ->ulFEntry >> gub4kEntrysPerBurstLog) & gubBurstsPerBankMask;
	}

	if ((ubLogType == FLHErrLogType) && ((ubAbnormalLoc < VT_FLUSHD1_ERR_D1_UNC_0x78) || (ubAbnormalLoc > VT_COPYD3UNIT_ERR_D3_ECC_DIO_0x81))) {	// FLH log
		UBYTE ubFailLogPTRTemp = VT->gubFailLogPTR ? (VT->gubFailLogPTR - 1) : (VTLogEntryNum - VTTimeoutLogEntryNum - 1);
		if ((VT->guoFailLog[ubFailLogPTRTemp].FLHLog.uwFUnit == FQ->uwFUnit)
		        && (VT->guoFailLog[ubFailLogPTRTemp].FLHLog.uwFEntry == (UWORD)FQ->ulFEntry)
		        && (VT->guoFailLog[ubFailLogPTRTemp].FLHLog.ubFailType == ubFailType)) {
			return;
		}
		else if ((ubAbnormalLoc == VT_TABLE_VERIFY_RETRY_0x9C) && (VT->guoFailLog[ubFailLogPTRTemp].FLHLog.uwFUnit == FQ->uwFUnit) && (VT->guoFailLog[ubFailLogPTRTemp].FLHLog.ubFailType == ubFailType)) {
			return;
		}

	}
	else if (ubLogType == FTLErrLogType) {	// FTL log
		UBYTE ubFailLogPTRTemp = VT->gubFailLogPTR ? (VT->gubFailLogPTR - 1) : (VTLogEntryNum - VTTimeoutLogEntryNum - 1);
		VT->guoFailLog[VT->gubFailLogPTR].FTLLog.ubFailType = ubFailType;
		if ((ubFailType == BYTE_ON_PROBATION) && (VT->guoFailLog[ubFailLogPTRTemp].FTLLog.ubAbnormalLoc == ubAbnormalLoc)) {
			flaDumpError(1, ubAbnormalLoc);
		}
	}
	else if ((ubLogType == NCQTimeoutLogType) || (ubLogType == ATATimeoutLogType)) {
		// Clear Timeout Log Content For Record
		memset(&VT->guoFailLog[ubTimeoutLogPTR], 0, sizeof(VT->guoFailLog[ubTimeoutLogPTR]));
	}

	// format for all log type
	if ((ubLogType != NCQTimeoutLogType) && (ubLogType != ATATimeoutLogType)) {
		VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubLogType = (ubLogType & 0xF);			// FLHLog.ubLogType: 4 bit
		VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubTemperature = (ubTemperature & 0xF);		// FLHLog.ubTemperature: 4 bit
		VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubPowerOnCnt = (VT->gulPowerOnCnt & 0xFF); 	// FLHLog.ubPowerOnCnt : 8 bit
	}
	else {
		VT->guoFailLog[ubTimeoutLogPTR].FLHLog.ubLogType = (ubLogType & 0xF);			// FLHLog.ubLogType: 4 bit
		VT->guoFailLog[ubTimeoutLogPTR].FLHLog.ubTemperature = (ubTemperature & 0xF);		// FLHLog.ubTemperature: 4 bit
		VT->guoFailLog[ubTimeoutLogPTR].FLHLog.ubPowerOnCnt = (VT->gulPowerOnCnt & 0xFF); 	// FLHLog.ubPowerOnCnt : 8 bit
	}

	if (ubLogType == FLHErrLogType) {
		VT->guoFailLog[VT->gubFailLogPTR].FLHLog.uwFTLState = (UWORD)(VT->gulFTLState.All & 0xFFFF); // FLHLog.uwFTLState: 16 bit
	}
	else if ((ubLogType == NCQTimeoutLogType) || (ubLogType == ATATimeoutLogType)) {
		VT->guoFailLog[VT->gubFailLogPTR].NCQTimeoutLog.ulFTLState = gulFTLStateForTimeout;//NCQTimeoutLog.ulFTLState, ATATimeoutLog.ulFTLState //3rd 4B
	}
	else {
		VT->guoFailLog[VT->gubFailLogPTR].FTLLog.ulFTLState = (VT->gulFTLState.All & 0xFFFFFFFF);	// FTLLog.ulFTLState; // 4B
	}

	switch (ubLogType) {
		case FLHErrLogType:
			VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFailType = ubFailType;	// FLHLog.ubFailType: 4 bit
			VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubTableSel = ubTableSel;	// FLHLog.ubTableSel: 4 bit
			VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFLHErrorMap = ubFLHErrorMap;
			VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubAbnormalLoc = ubAbnormalLoc;

			if ((ubAbnormalLoc >= VT_FLUSHD1_ERR_D1_UNC_0x78) && (ubAbnormalLoc <= VT_COPYD3UNIT_ERR_D3_ECC_DIO_0x81)) {
				// For Flush D1 Error Handle (FlushD1 & SwapRUT Cop D3 Unit)
				if (ENABLE_DEBUG_UART_CC) {
					UartString("\n $LOG FD1: ");
					Uart_Tx_DataHex(ubAbnormalLoc);
				}
				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.uwFUnit = (VT->gulFTLState.B.btNeedRUTReplace ? VT->guwGCGRTarget[VT->gubGCGRTargetIndex].B.uwTarget : guwFlushD1Fun_D3Unit);
				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.uwFEntry = (VT->gulFTLState.B.btNeedRUTReplace ? 0 : guwStartProgramOrderIndex);  // 沒有 FEntry, 用 Start Order 取代
				if (VT->guoFailLog[VT->gubFailLogPTR].FLHLog.uwFUnit < 4096) {
					VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubD1Link = gubD1LinkTable[VT->guoFailLog[VT->gubFailLogPTR].FLHLog.uwFUnit].B.ubD1Link;
				}
				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.btD3 = ((ubFailType == BYTE_UNC_FLUSHD1) ? 0 : 1);
			}
			else {
				if (ubFLHErrorMap == BIT_NormalErr) {
					VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFLHErrorMap = BIT_NormalErr;
					if ((FQ->ulFIPError.All & CRC16_ERROR) && (FQ->ubFJob & BYTE_FJOB_READ)) {
						VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFLHErrorMap |= BIT_Read_CRC16Err;
					}
					if ((FQ->ulFIPError.All & DECOMPRESSION_FAIL_INF) && (FQ->ubFJob & BYTE_FJOB_READ)) {
						VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFLHErrorMap |= BIT_Read_DecompressionErr;
					}
					if ((FQ->ulFIPError.All & COMPRESSION_FAIL_INF) && (FQ->ubFJob & BYTE_FJOB_WRITE)) {
						VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFLHErrorMap |= BIT_Program_CompressionErr;
					}
					if ((FQ->ulFIPError.All & LCA_FW_CMP_ERR_INF) && (FQ->ubFJob & BYTE_FJOB_READ)) {
						VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFLHErrorMap |= BIT_Read_LCAErr;
					}
					if ((FQ->ulFIPError.All & CRC32_ERROR) && ((FQ->ulFIPError.All & UNC_MAP_INF) == 0) && (FQ->ubFJob & BYTE_FJOB_READ)) {
						VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFLHErrorMap |= BIT_Read_CRC32Err;
					}
					if ((FQ->ulFIPError.All & ERASE_PAGE) && (FQ->ubFJob & BYTE_FJOB_READ)) {
						VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFLHErrorMap |= BIT_Read_ErasePageErr;
					}
					if (FQ->ulFIPError.All & FW_SETTING_ERR_INF) {
						VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubFLHErrorMap |= ((FQ->ubFJob & BYTE_FJOB_READ) ? BIT_Read_FWSettingErr : BIT_Program_FWSettingErr);
					}
				}

				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.uwFUnit = FQ->uwFUnit;
				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.uwFEntry = (UWORD)FQ->ulFEntry;
				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ubD1Link = (FQ->btD3 ? Default_D1_Unit : ((FQ->uwFJobInfo & BIT_FJOBI_RUT_D1LINK) ? FQ->ubFSAIndex : gubD1LinkTable[FQ->uwFUnit].B.ubD1Link));
				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.btD3 = FQ->btD3;
				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.btIsFastPage = (FQ->uwFUnit >= 4096 ? gubFastPageForCopyFromRUT : gulVC[FQ->uwFUnit].B.btFastPage);
				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ulVir4kIndex_L = FQ->ulSpare & 0xFF;
				VT->guoFailLog[VT->gubFailLogPTR].FLHLog.ulVir4kIndex_H = FQ->ulSpare >> 8; // 8: U32 ulVir4kIndex_L: 8;
			}

			break;
		case FTLErrLogType:
			VT->guoFailLog[VT->gubFailLogPTR].FTLLog.ubAbnormalLoc = ubAbnormalLoc;
			break;
		case DDRErrLogType:
			ubError = RAML[RAML_DDR3_ECC_CTRL] & 0xFF;
			if (ubError & Z1_UN_CORRECT_INT) {
				VT->guoFailLog[VT->gubFailLogPTR].DDRErrLog.ulFailAddr = RAML[RAML_DDR3_Z1_ECC_UN_CORRECT_ADDR];
			}
			if (ubError & Z2_ECC_ERROR_INT) {
				VT->guoFailLog[VT->gubFailLogPTR].DDRErrLog.ulFailAddr = RAML[RAML_DDR3_Z2_ECC_ERROR_ADDR];
			}
			if (ubError & Z2_UN_CORRECT_INT) {
				VT->guoFailLog[VT->gubFailLogPTR].DDRErrLog.ulFailAddr = RAML[RAML_DDR3_Z2_ECC_UN_CORRECT_ADDR];
			}
			if (ubError & Z3_ECC_ERROR_INT) {
				VT->guoFailLog[VT->gubFailLogPTR].DDRErrLog.ulFailAddr = RAML[RAML_DDR3_Z3_ECC_ERROR_ADDR];
			}
			if (ubError & Z3_UN_CORRECT_INT) {
				VT->guoFailLog[VT->gubFailLogPTR].DDRErrLog.ulFailAddr = RAML[RAML_DDR3_Z3_ECC_UN_CORRECT_ADDR];
			}
			VT->guoFailLog[VT->gubFailLogPTR].DDRErrLog.ulDD3_ECC_CTRL = RAML[RAML_DDR3_ECC_CTRL];
			VT->gubCountDDRINT = (gulCountDDRINT > 255) ? 255 : gulCountDDRINT;
			break;
		case ParityErrLogType:
			VT->guoFailLog[VT->gubFailLogPTR].ParityErrLog.ulSYS1L_INT_STS3 = SYS1L[SYS1L_INT_STS3];
			break;
		case NCQTimeoutLogType:
			VT->guoFailLog[ubTimeoutLogPTR].NCQTimeoutLog.ubTag = ubgFQLinkIndex;
			VT->guoFailLog[ubTimeoutLogPTR].NCQTimeoutLog.btLBAOverlap = gubLBAOverlap & BIT0;
			VT->guoFailLog[ubTimeoutLogPTR].NCQTimeoutLog.btIsWrite = ubFailType & BIT0;
			VT->guoFailLog[ubTimeoutLogPTR].NCQTimeoutLog.btLargeSize = (WR_NCQ_CMD_INFO[ubgFQLinkIndex].ulSectorCnt >= 0x1000) ? 1 : 0;
			VT->guoFailLog[ubTimeoutLogPTR].NCQTimeoutLog.ubECCnt_MinusOne = WR_NCQ_CMD_INFO[ubgFQLinkIndex].uwEC - 1;
			VT->guoFailLog[ubTimeoutLogPTR].NCQTimeoutLog.ulElapsedTime = M_CommandOperationTime(WR_NCQ_CMD_INFO[ubgFQLinkIndex].ulTrigTime);
			VT->guoFailLog[ubTimeoutLogPTR].NCQTimeoutLog.ulLBA = WR_NCQ_CMD_INFO[ubgFQLinkIndex].ulLBA;
			break;
		case ATATimeoutLogType:
			VT->guoFailLog[ubTimeoutLogPTR].ATATimeoutLog.ubCMD = ubgFQLinkIndex;
			VT->guoFailLog[ubTimeoutLogPTR].ATATimeoutLog.ubErrorCode = ubFailType;
			VT->guoFailLog[ubTimeoutLogPTR].ATATimeoutLog.ulElapsedTime = M_CommandOperationTime(gulHostCMDTriggerTime);
			break;
		default:
			M_AssertError(1);
			break;
	}

	if ((ubLogType != NCQTimeoutLogType) && (ubLogType != ATATimeoutLogType)) {
		VT->gubFailLogPTR++;
		//4*16 for Timeout Log
		if (VT->gubFailLogPTR >= VTLogEntryNum - VTTimeoutLogEntryNum) {
			VT->gubFailLogPTR = 0;
		}
		// Clear Fail Log Content For Next Record
		memset(&VT->guoFailLog[VT->gubFailLogPTR], 0, sizeof(VT->guoFailLog[VT->gubFailLogPTR]));
	}
	else {
		VT->gubTimeoutLogPTR++;
		if (VT->gubTimeoutLogPTR >= VTTimeoutLogEntryNum) {
			VT->gubTimeoutLogPTR = 0;
		}
	}
}



void flaReadFailFixDQSREMismatch(UBYTE ubgFQLinkIndex)
{
#if (!RDT)
	M_CheckBurnerModeToDisableFunction();
	// Read 1 physical frame. This step is to clear internal signal. => only program to cache and read from cache
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubCEIndex = FQ->ubCEIndex;
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	UBYTE ubL4KNum = 1;
	FSA_t * pFSA = (FSA_t *)gulFSABase[ubChannel];
	UBYTE ubDepth;
	M_GetMTDepth(ubDepth);
	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));

	if (ENABLE_iFSA) {
		gFREG[FCTLL_IFSA0].L = 0x00000000;
		gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;
	}
	else {
		pFSA[gubFSAIndex[ubChannel]].uword.W0 = 0x0000;
		pFSA[gubFSAIndex[ubChannel]].uword.W1 = 0x0000;
		pFSA[gubFSAIndex[ubChannel]].B.B4 = 0x00;

		//--------------- Address Gen Setting ---------------
		gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
		gFREG[FCTLL_FSA_SEL].L  = gubFSAIndex[ubChannel];
	}

	gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
	gFREG[FCTLL_RS_CFG].L &= (~RS_ON_THE_FLY_EN);

	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubL4KNum);

	gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
	gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
	gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);

	gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);

	gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME) ;
	gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubL4KNum) << 8);

	M_VirtualToPhysicalCE(ubCEIndex);
	UBYTE ubL4KCount = 0;
	volatile L4KTable16B *pL4KTable;
	gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_OFFSET;
	for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
		pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
		//set L4K table info
		pL4KTable->ulL4K_LCA = 0x00000000;
		pL4KTable->Para0x04.ulAll = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (TableZoneBase + ubL4KCount * 4096) / 512;	// data 隨便寫
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = MAX_ZCODE;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
	}
	FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] =   M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}
Mark_DummyProgram:
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C80_A5_DW;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x3_FPU_W_FixDQSREMismatch, gFREG, CHECK_FPU_DMA_W_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}


	// reassign CE,  MT stop後, 當MT preload + FPU read後會clear CE, 所以上面FPU trig後, CE有可能會被清掉, 這邊需再set CE一次, 之後無MT preload行為就不需要再重設
	FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;

	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] =   M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}

	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x4_FPU_POL_FixDQSREMismatch, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}


Mark_DummyRead:
	gFREG[FCTLL_ZIP_MAP].L = 0x000000FF;

	for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
		pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
		//set L4K table info
		pL4KTable->ulL4K_LCA = 0x00000000;
		pL4KTable->Para0x04.ulAll = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (TableZoneBase + ubL4KCount * 4096) / 512;
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0x00;	//都不讀上來, 避免蓋到原有buffer的data
	}
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C00_A5_C05_A2_CE0_DR;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x5_FPU_R_FixDQSREMismatch, gFREG, CHECK_FPU_DMA_R_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}

	//while (gubINTTable[ubDepth].All == 0); // use this to check

	// need clean msg dispatch
	gubINTTable[ubDepth].All = 0;
	M_AddMTDepth(ubDepth);

	FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
#endif
}

void flaReadDQSREMismatchHandling(UBYTE ubgFQLinkIndex)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	guoCE_FailMap |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(gFQI.gFQLink[ubgFQLinkIndex].ubCEIndex)));
	gulDQSREMismatchMap |= (BIT0 << gFQI.gFQLink[ubgFQLinkIndex].ubCEIndex);
	VT->gubReadDQSREMismatchCEMapCnt[gFQI.gFQLink[ubgFQLinkIndex].ubCEIndex]++;
}

void flaReadFailRetryReFillL4KTable(UBYTE ubgFQLinkIndex)
{
#if (!RDT)
	M_CheckBurnerModeToDisableFunction();
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubL4kNum = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum;

	volatile L4KTable16B * pubFillL4K_TABLE = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize)); // keep volatile??
	UBYTE ubL4KCount = 0;
	UBYTE ubCheckFailCount = 0;
	while (ubCheckFailCount < gub4kEntrysPerPlane) {
		if ((gulGetUNCFrameMapInMT >> ubCheckFailCount) & BIT0) { // fail one
			break;
		}
		ubCheckFailCount++;
	}
	ubCheckFailCount -= (FQ->ulFEntry & gub4kEntrysPerPlaneMask); // start frame maybe jump
	if (ENABLE_WP_DEBUG && (ubCheckFailCount >= ubL4kNum)) {
		flaLEDBlink(1);
	}
	else {
		M_AssertError(ubCheckFailCount >= ubL4kNum);
	}

	if (gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) {
		ubCheckFailCount = 0;
	}

	// Get first correct L4KBADR
	for (ubL4KCount = 0; ubL4KCount <= ubCheckFailCount; ubL4KCount++) {
		pubFillL4K_TABLE = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
	}


	if (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btZipBypass) {

		gMTQ->Depth[FQ->ubDepth].ulL4KBADR = (pubFillL4K_TABLE->Para0x0C.BitMap.ulL4K_BADR - ((ubCheckFailCount * 4096) / 512)); // get correct BADR
		for (ubL4KCount = 0; ubL4KCount < ubL4kNum; ubL4KCount++) {
			pubFillL4K_TABLE = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pubFillL4K_TABLE->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			if (gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) {
				//if (ubL4kNum == 1) {
				pubFillL4K_TABLE->Para0x08.BitMap.ubL4K_BUF_MASK = FQ->ulDataMask;
				//}
				pubFillL4K_TABLE->Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
			}
			else {
				pubFillL4K_TABLE->Para0x0C.BitMap.ulL4K_BADR = (gMTQ->Depth[FQ->ubDepth].ulL4KBADR + ((ubL4KCount * 4096) / 512));
			}
		}
	}
#endif
}

void flaReadFailRetryToshiba1ZnmSpecialFlow(void)
{
	//	M_CheckRDTModeToDisableFunction();
	U32 ulTemp = gFREG[FCTLL_FLH_SET].L;
	if (!guoFlashUseType.B.btSandisk) {
		gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
	}
	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_Asyn40, 6);
			//Fla_Set_Offset_DQS_Delay(0);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | gubClockDivValue);
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
			}
		}
	}


	{
		//Pre Conditon
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x5C;
		gFREG[FCTLL_PIO_CMD].B.B0 = 0xC5;

		//A
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x05;
		gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		gFREG[FCTLL_PIO_DAT].B.B0 = 0x78;

		//B
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x06;
		gFREG[FCTLL_PIO_DAT].B.B0 = 0x78;

		//C
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x07;
		gFREG[FCTLL_PIO_DAT].B.B0 = 0x78;
	}
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x26;
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x5D;

	if (!guoFlashUseType.B.btSandisk) {
		gFREG[FCTLL_FLH_SET].L = ulTemp;
	}
	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_400, 0xF);
			//Fla_Set_Offset_DQS_Delay(Toggle_400);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
				gFREG[FCTLL_FDIV_CFG].L = 0;
			}
		}
	}
}

void flaReadFailRetryToshiba1ZnmSpecialTerminateFlow(void)
{
	//	M_CheckRDTModeToDisableFunction();
	U32 ulTemp = gFREG[FCTLL_FLH_SET].L;
	if (!guoFlashUseType.B.btSandisk) {
		gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
	}
	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_Asyn40, 6);
			//Fla_Set_Offset_DQS_Delay(0);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | gubClockDivValue);
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
			}
		}
	}
	{
		// Terminate the Read Retry mode
		//A
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x05;
		gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		gFREG[FCTLL_PIO_DAT].B.B0 = 0;

		//B
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x06;
		gFREG[FCTLL_PIO_DAT].B.B0 = 0;

		//C
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x07;
		gFREG[FCTLL_PIO_DAT].B.B0 = 0;
	}
	if (!guoFlashUseType.B.btSandisk) {
		gFREG[FCTLL_FLH_SET].L = ulTemp;
	}
	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_400, 0xF);
			//Fla_Set_Offset_DQS_Delay(Toggle_400);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
				gFREG[FCTLL_FDIV_CFG].L = 0;
			}
		}
	}
}

void flaReadFailRetrySandiskFlow(UBYTE ubRetryCount, UWORD uwFJobInfo, UBYTE * ubExternalBuffer)
{
	//#if (!RDT)
	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_Asyn40, 6);
			//Fla_Set_Offset_DQS_Delay(0);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | gubClockDivValue);
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
			}
		}
	}

	UBYTE ubIsFastPage = (uwFJobInfo & BIT_FJOBI_FASTPAGE) ? 1 : 0;
	UBYTE * ubD3RetryBuffer = (ubExternalBuffer != NULL) ? ubExternalBuffer : &gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]];

	if ((guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0))  && (ubIsFastPage/* || ENABLE_ULTRAMLC*/)) {

		UBYTE ubAddr;
		UWORD uwData_1;
		UWORD uwData;

		gFREG[FCTLL_PIO_CMD].B.B0 = 0xEF;
		if (TLC) {
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x14;
		}
		else {
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x11;
		}
		gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);

		//P0
		if (  gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE ) {
			uwData_1 = (gubHardBitRetryParameterTable[ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE]]);
			uwData = ( ( uwData_1 << 8 ) +  ( uwData_1 ) );
			gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
		}
		else {
			gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE]];
		}

		//Set P1.P2.P3 to zero in SLC mode
		for (ubAddr = 1; ubAddr < (1 + 3); ubAddr++) {
			gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			if ( gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0x00);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
			}
		}

		for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
		}
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;           //Send 0x70 to check status
		while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);
		if (TLC == 0) {
			if (gubSandiskRetryType == 1) {
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x26;
			}
		}
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x5D;
		for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
		}


	}
	else {
		UWORD uwData_1;
		UWORD uwData;
		if (TLC) {
			UBYTE ubAddr;
			UBYTE ubLoop;


			for (ubLoop = 0; ubLoop < 2; ubLoop++) {
				//ubLoop = 0, Set P0 for MLC A, P1 for MLC B, P2 for MLC C, P3 for MLC D
				//ubLoop = 1, Set P0 for MLC E, P1 for MLC F, P2 for MLC G, P3 be ZERO
				gFREG[FCTLL_PIO_CMD].B.B0 = 0xEF;
				gFREG[FCTLL_PIO_ADR].B.B0 = (0x12 + ubLoop);

				for (ubAddr = 0; ubAddr < 4; ubAddr++) {
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);

					if (  gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE ) {
						if ((ubLoop * 4 + ubAddr) == 7) {
							gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0x00);
						}
						else {
							uwData_1 = ubD3RetryBuffer[(ubLoop * 4) + ubAddr];
							uwData = ( ( uwData_1 << 8 ) +  ( uwData_1 ) );
							gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
							//gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(gubSAN_15nm_ED3_TLC_SRS[ubRetryCount * 7 + ubLoop * 4 + ubAddr]);
						}
					}
					else {
						if ((ubLoop * 4 + ubAddr) == 7) {
							gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
						}
						else {
							gFREG[FCTLL_PIO_DAT].B.B0 = ubD3RetryBuffer[(ubLoop * 4) + ubAddr];
							//gFREG[FCTLL_PIO_DAT].B.B0 = gubSAN_15nm_ED3_TLC_SRS[ubRetryCount * 7 + ubLoop * 4 + ubAddr];
						}
					}
				}
				for (gulTemp = 0; gulTemp < 10; gulTemp++) {
				}
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;           //Send 0x70 to check status
				while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);
			}

		}
		else {
			gFREG[FCTLL_PIO_CMD].B.B0 = 0xEF;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x11;
			UBYTE ubPageType;
			U32 ulPageByiFSA = guliFSA[0];
			//guliFSA[0] = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((ulFEntry >> gub4kEntrysPerPageLog) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
			if (guoFlashUseType.B.btFourPlane) {
				ulPageByiFSA = ulPageByiFSA & 0x3FFF;
			}
			else {
				ulPageByiFSA = ulPageByiFSA & 0x7FFF;
			}
			if (guoFlashUseType.B.bt1024Page) {
				ulPageByiFSA = ulPageByiFSA << 1;
			}
			ulPageByiFSA = ulPageByiFSA >> gubSectorsPerPlaneLog;

			if ((ulPageByiFSA % 2) == 0) {
				if (ulPageByiFSA != 0) {
					ubPageType = UPPER_PAGE;
				}
				else {
					ubPageType = LOWER_PAGE;
				}
			}
			else {
				if (ulPageByiFSA != guwPagesPerUnitMask) {
					ubPageType = LOWER_PAGE;
				}
				else {
					ubPageType = UPPER_PAGE;
				}
			}
			if (ubPageType == LOWER_PAGE) {
				if ( gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE) {
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P0
					uwData_1 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + ((UWORD)ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE])];
					uwData = (uwData_1 << 8) + uwData_1;
					gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P1
					gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)0x00;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P2
					uwData_1 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + ((UWORD)ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] + 1)];
					uwData = (uwData_1 << 8) + uwData_1;
					gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P3
					gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)0x00;
				}
				else {
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P0
					gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + ((UWORD)ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE])];
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P1
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P2
					gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + ((UWORD)ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] + 1)];
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P3
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
				}
			}
			else {
				if (gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE) {
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P0
					gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)0x00;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P1
					uwData_1 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + ((UWORD)gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] + (UWORD)ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE])];
					uwData = (uwData_1 << 8) + uwData_1;
					gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P2
					gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)0x00;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P3
					uwData_1 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + ((UWORD)gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] + (UWORD)ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] + 1)];
					uwData = (uwData_1 << 8) + uwData_1;
					gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
				}
				else {
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P0
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P1
					gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + ((UWORD)gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] + (UWORD)ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE])];
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P2
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);//P3
					gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + ((UWORD)gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] + (UWORD)ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] + 1)];
				}
			}
			for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
			}
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
			while (gFREG[FCTLL_PIO_DAT].B.B0 != 0xE0);
			/*
			gubSandiskRetryType
			SLC 2 types  0->1
			Close Blk 4 types 2->5
			Open LP 4 types 6->9
			Open MLC 4 types 10->13
			*/
			if (gubSandiskRetryType == 5) { //魔術數字請參照spec
				flaSandiskSet_LMFLGFIX_NEXT(1);
			}
			else if ((gubSandiskRetryType == 12) || (gubSandiskRetryType == 13)) {
				flaSandiskSet_LMFLGFIX_NEXT(0);
			}
			if ((gubSandiskRetryType == 4) || (gubSandiskRetryType == 5) || (gubSandiskRetryType == 12) || (gubSandiskRetryType == 13)) {
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x25;
			}
			else if ((gubSandiskRetryType == 8) || (gubSandiskRetryType == 9)) {
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x2D;
			}
			if ((gubSandiskRetryType == 1) || (gubSandiskRetryType == 3) || (gubSandiskRetryType == 5) || (gubSandiskRetryType == 6) ||
			        (gubSandiskRetryType == 8) || (gubSandiskRetryType == 10) || (gubSandiskRetryType == 12)) {

				gFREG[FCTLL_PIO_CMD].B.B0 = 0x26;
			}
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x5D;


		}
	}


	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_400, 0xF);
			//Fla_Set_Offset_DQS_Delay(Toggle_400);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
				gFREG[FCTLL_FDIV_CFG].L = 0;
			}
		}
	}
	//#endif
}
void flaSandiskSet_LMFLGFIX_NEXT(UBYTE ubLMFLGFIX)
{
	UBYTE uwData_1;
	UBYTE uwData;

	gFREG[FCTLL_PIO_CMD].B.B0 = 0x5C;///////進入test mode
	gFREG[FCTLL_PIO_CMD].B.B0 = 0xC5;
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
	for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
	}
	uwData_1 = 0x01;
	uwData = (uwData_1 << 8) + uwData_1;
	if ( gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE) {
		gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
	}
	else {
		gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
	}
	for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
	}
	/////////////////////////////////////////
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;//C6開啟  C2歸0
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x22;
	for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
	}
	if (ubLMFLGFIX) {
		uwData_1 = 0xC6;
		uwData = (uwData_1 << 8) + uwData_1;
		if ( gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE) {
			gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
		}
		else {
			gFREG[FCTLL_PIO_DAT].B.B0 = 0xC6;
		}
	}
	else {
		uwData_1 = 0xC2;
		uwData = (uwData_1 << 8) + uwData_1;
		if ( gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE) {
			gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
		}
		else {
			gFREG[FCTLL_PIO_DAT].B.B0 = 0xC2;
		}
	}
	for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
	}
	///////////////////////////////////////
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;///////離開test mode
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
	for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
	}
	uwData_1 = 0x00;
	uwData = (uwData_1 << 8) + uwData_1;
	if ( gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE) {
		gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
	}
	else {
		gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
	}

}

void flaReadFailRetryToshibaFlow(UBYTE ubRetryCount, UWORD uwFJobInfo)
{
	//#if (!RDT)
	U32 ulTemp = gFREG[FCTLL_FLH_SET].L;
	UBYTE ubToggleModeTemp = gubToggleMode;
	if (!ENABLE_DEBUG_RETRY_NOT_SWITCH_LEGACY) {
		gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
		gubToggleMode = 0;
	}

	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_Asyn40, 6);
			//Fla_Set_Offset_DQS_Delay(0);

			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | gubClockDivValue);
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
			}
		}
	}

	UBYTE ubIsFastPage = (uwFJobInfo & BIT_FJOBI_FASTPAGE) ? 1 : 0;


	if (gubD3RetryCount_HR == 0) {
		//Pre Conditon
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x5C;
		gFREG[FCTLL_PIO_CMD].B.B0 = 0xC5;
	}

	if ((guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0))  && (ubIsFastPage/* || ENABLE_ULTRAMLC*/)) {
		{			
			if (guoFlashUseType.B.btToshiba1Ynm || guoFlashUseType.B.btToshiba1Znm) {

				if (!ENABLE_DEBUG_RETRY_PARAMETER) {
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x08;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(gubHardBitRetryParameterTable[(ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE])]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[(ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE])];
					}

					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x0D;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(gubHardBitRetryParameterTable[(ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE]) + 1]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[(ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE]) + 1];
					}
				}
				else {
					UBYTE ubRetryData;
					if (guoFlashUseType.B.btToshiba1Ynm) {
						// A19nm A2, ref: tA19-rr-v1.1-M2CMP01-060 A19nm pSLC Read Retry Sequence.pdf
						if (ubRetryCount == 0) {
							ubRetryData = 0x0C;
						}
						else if (ubRetryCount == 1) {
							ubRetryData = 0x78;
						}
						else if (ubRetryCount == 2) {
							ubRetryData = 0x70;
						}
						else if (ubRetryCount == 3) {
							ubRetryData = 0x18;
						}
						else if (ubRetryCount == 4) {
							ubRetryData = 0x74;
						}
						else if (ubRetryCount == 5) {
							ubRetryData = 0x6C;
						}
						else if (ubRetryCount == 6) {
							ubRetryData = 0x1C;
						}
						else if (ubRetryCount == 7) {
							ubRetryData = 0x7C;
						}
						else if (ubRetryCount == 8) {
							ubRetryData = 0x68;
						}
						else if (ubRetryCount == 9) {
							ubRetryData = 0x14;
						}
					}
					else if (guoFlashUseType.B.btToshiba1Znm) {
						// 15nm A2, ref: t15-rr-v0.3-M2CMP01-186_15nm MLC (pSLC mode) Read Retry Sequence Rev0.3.pdf
						if (ubRetryCount == 0) {
							ubRetryData = 0x78;
						}
						else if (ubRetryCount == 1) {
							ubRetryData = 0x08;
						}
						else if (ubRetryCount == 2) {
							ubRetryData = 0x10;
						}
						else if (ubRetryCount == 3) {
							ubRetryData = 0x18;
						}
						else if (ubRetryCount == 4) {
							ubRetryData = 0x1C;
						}
						else if (ubRetryCount == 5) {
							ubRetryData = 0x70;
						}
						else if (ubRetryCount == 6) {
							ubRetryData = 0x64;
						}
						else if (ubRetryCount == 7) {
							ubRetryData = 0x24;
						}
						else if (ubRetryCount == 8) {
							ubRetryData = 0x5C;
						}
						else if (ubRetryCount == 9) {
							ubRetryData = 0x54;
						}
					}

					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x08;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(ubRetryData);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData;
					}

					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x0D;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = 0;
					}
				}


				gFREG[FCTLL_PIO_CMD].B.B0 = 0xA2;

				if (guoFlashUseType.B.btToshiba1Ynm) {
					if (ubRetryCount == 6) {
						gFREG[FCTLL_PIO_CMD].B.B0 = 0xB3;
					}
				}

				gFREG[FCTLL_PIO_CMD].B.B0 = 0x5D;
			}
		}
	}
	else {
		{

			if (guoFlashUseType.B.btToshiba1Ynm || guoFlashUseType.B.btToshiba1Znm) {

				if (!ENABLE_DEBUG_RETRY_PARAMETER) {
					//A
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x04;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE])]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE])];
					}
					//B
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x05;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]) + 1]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]) + 1];
					}
					//C
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x06;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]) + 2]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]) + 2];
					}
					//D
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x07;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]) + 3]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]) + 3];
					}
					//E
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x0D;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]) + 4]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + (ubRetryCount * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]) + 4];
					}
				}
				else {
					UBYTE ubRetryData[4] = {0, 0, 0, 0};
					if (guoFlashUseType.B.btToshiba1Ynm) {
						// A19nm, ref: tA19-rr-v1.1-M2CMP01-060 A19nm MLC 128Gb Read Retry Sequence.pdf
						if (ubRetryCount == 0) {
							ubRetryData[0] = 0x04;
							ubRetryData[1] = 0x04;
							ubRetryData[2] = 0x7C;
							ubRetryData[3] = 0x00;
						}
						else if (ubRetryCount == 1) {
							ubRetryData[0] = 0x0C;
							ubRetryData[1] = 0x06;
							ubRetryData[2] = 0x78;
							ubRetryData[3] = 0x78;
						}
						else if (ubRetryCount == 2) {
							ubRetryData[0] = 0x7C;
							ubRetryData[1] = 0x00;
							ubRetryData[2] = 0x74;
							ubRetryData[3] = 0x76;
						}
						else if (ubRetryCount == 3) {
							ubRetryData[0] = 0x14;
							ubRetryData[1] = 0x08;
							ubRetryData[2] = 0x00;
							ubRetryData[3] = 0x00;
						}
						else if (ubRetryCount == 4) {
							ubRetryData[0] = 0x1C;
							ubRetryData[1] = 0x04;
							ubRetryData[2] = 0x7A;
							ubRetryData[3] = 0x78;
						}
						else if (ubRetryCount == 5) {
							ubRetryData[0] = 0x02;
							ubRetryData[1] = 0x78;
							ubRetryData[2] = 0x76;
							ubRetryData[3] = 0x74;
						}
						else if (ubRetryCount == 6) {
							ubRetryData[0] = 0x08;
							ubRetryData[1] = 0x76;
							ubRetryData[2] = 0x72;
							ubRetryData[3] = 0x72;
						}
						else if (ubRetryCount == 7) {
							ubRetryData[0] = 0x10;
							ubRetryData[1] = 0x7C;
							ubRetryData[2] = 0x70;
							ubRetryData[3] = 0x76;
						}
						else if (ubRetryCount == 8) {
							ubRetryData[0] = 0x18;
							ubRetryData[1] = 0x06;
							ubRetryData[2] = 0x08;
							ubRetryData[3] = 0x76;
						}
						else if (ubRetryCount == 9) {
							ubRetryData[0] = 0x78;
							ubRetryData[1] = 0x0C;
							ubRetryData[2] = 0x04;
							ubRetryData[3] = 0x00;
						}
					}
					else if (guoFlashUseType.B.btToshiba1Znm) {
						// 15nm, ref: t15-rr-v0.3-M2CMP01-138_15nm MLC Read Retry Sequence Rev0.3.pdf
						if (ubRetryCount == 0) {
							ubRetryData[0] = 0x00;
							ubRetryData[1] = 0x00;
							ubRetryData[2] = 0x00;
							ubRetryData[3] = 0x00;
						}
						else if (ubRetryCount == 1) {
							ubRetryData[0] = 0x02;
							ubRetryData[1] = 0x04;
							ubRetryData[2] = 0x02;
							ubRetryData[3] = 0x00;
						}
						else if (ubRetryCount == 2) {
							ubRetryData[0] = 0x7C;
							ubRetryData[1] = 0x00;
							ubRetryData[2] = 0x7C;
							ubRetryData[3] = 0x7C;
						}
						else if (ubRetryCount == 3) {
							ubRetryData[0] = 0x7A;
							ubRetryData[1] = 0x7C;
							ubRetryData[2] = 0x7A;
							ubRetryData[3] = 0x7C;
						}
						else if (ubRetryCount == 4) {
							ubRetryData[0] = 0x78;
							ubRetryData[1] = 0x00;
							ubRetryData[2] = 0x78;
							ubRetryData[3] = 0x7A;
						}
						else if (ubRetryCount == 5) {
							ubRetryData[0] = 0x7E;
							ubRetryData[1] = 0x02;
							ubRetryData[2] = 0x7E;
							ubRetryData[3] = 0x7A;
						}
						else if (ubRetryCount == 6) {
							ubRetryData[0] = 0x76;
							ubRetryData[1] = 0x04;
							ubRetryData[2] = 0x76;
							ubRetryData[3] = 0x02;
						}
						else if (ubRetryCount == 7) {
							ubRetryData[0] = 0x04;
							ubRetryData[1] = 0x00;
							ubRetryData[2] = 0x04;
							ubRetryData[3] = 0x78;
						}
						else if (ubRetryCount == 8) {
							ubRetryData[0] = 0x06;
							ubRetryData[1] = 0x00;
							ubRetryData[2] = 0x06;
							ubRetryData[3] = 0x76;
						}
						else if (ubRetryCount == 9) {
							ubRetryData[0] = 0x74;
							ubRetryData[1] = 0x7C;
							ubRetryData[2] = 0x74;
							ubRetryData[3] = 0x76;
						}
					}

					//A
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x04;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(ubRetryData[0]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData[0];
					}
					//B
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x05;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(ubRetryData[1]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData[1];
					}
					//C
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x06;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(ubRetryData[2]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData[2];
					}
					//D
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x07;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(ubRetryData[3]);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData[3];
					}
					//E
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x0D;
					if (gubToggleMode) {
						gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
					}
					else {
						gFREG[FCTLL_PIO_DAT].B.B0 = 0;
					}
				}

				if (guoFlashUseType.B.btToshiba1Ynm) {
					if (ubRetryCount == 6) {
						gFREG[FCTLL_PIO_CMD].B.B0 = 0xB3;
					}
				}

				if (!(uwFJobInfo & BIT_FJOBI_HR_SETINITIAL)) {
					if (gubD3RetryCount_HR == 0) {
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x26;
					}
					else {
						gFREG[FCTLL_PIO_CMD].B.B0 = 0xCD; // shoter busy time
					}

					gFREG[FCTLL_PIO_CMD].B.B0 = 0x5D;
				}
			}
		}
	}

	if (!ENABLE_DEBUG_RETRY_NOT_SWITCH_LEGACY) {
		gFREG[FCTLL_FLH_SET].L = ulTemp;
		gubToggleMode = ubToggleModeTemp;
	}

	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_400, 0xF);
			//Fla_Set_Offset_DQS_Delay(Toggle_400);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
				gFREG[FCTLL_FDIV_CFG].L = 0;
			}
		}
	}
	//#endif
}

void flaReadFailRetrySandiskTerminateFlow(UWORD uwFJobInfo)
{
	//#if (!RDT)
	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_Asyn40, 6);
			//Fla_Set_Offset_DQS_Delay(0);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | gubClockDivValue);
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
			}
		}
	}

	UBYTE ubIsFastPage = (uwFJobInfo & BIT_FJOBI_FASTPAGE) ? 1 : 0;
	// Terminate the Read Retry mode
	if ((guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0))  && (ubIsFastPage/* || ENABLE_ULTRAMLC*/)) {


		UBYTE ubAddr;

		gFREG[FCTLL_PIO_CMD].B.B0 = 0xEF;
		{
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x11;
		}
		gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);

		//Set P0.P1.P2.P3 to zero in SLC mode
		for (ubAddr = 0; ubAddr < 4; ubAddr++) {
			gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			if ( gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE ) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0x00);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
			}
		}

		for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
		}
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;           //Send 0x70 to check status
		while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);

		for (gulTemp = 0; gulTemp < 10; gulTemp++) {
		}
	}
	else {

		UBYTE ubAddr;
		UBYTE ubLoop;
		UBYTE ubLoopEnd = 1;
		if (TLC) {
			if ((uwFJobInfo == BIT_FJOBI_HR_SOFTBITRESET) && guoFlashUseType.B.btToshibaBiCs4) {
				ubLoopEnd = 6;
			}
			else {
				ubLoopEnd = 2;
			}
		}
		for (ubLoop = 0; ubLoop < ubLoopEnd; ubLoop++) {
			//TLC
			//add:0x12 => Set P0.P1.P2.P3 to zero in MLC_A,MLC_B,MLC_C,MLC_D
			//add:0x13 => Set P0.P1.P2.P3 to zero in MLC_E,MLC_F,MLC_G,reserved
			//MLC
			//add:0x11 => Set P0.P1.P2.P3 to zero
			gFREG[FCTLL_PIO_CMD].B.B0 = 0xEF;

			if (guoFlashUseType.B.btToshibaBiCs4 && (ubLoop >= 2)) {
				gFREG[FCTLL_PIO_ADR].B.B0 = (0x90 + ubLoop);  // 92 93 94 95
			}
			else if (TLC) {
				gFREG[FCTLL_PIO_ADR].B.B0 = (0x12 + ubLoop);
			}
			else {
				gFREG[FCTLL_PIO_ADR].B.B0 = 0x11;
			}
			gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);

			for (ubAddr = 0; ubAddr < 4; ubAddr++) {
				gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
				if ( gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE ) {
					gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0x00);
				}
				else {
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
				}
			}

			for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
			}
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;           //Send 0x70 to check status
			while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);
		}


		for (gulTemp = 0; gulTemp < 10; gulTemp++) {
		}
		if (guoFlashUseType.B.btToshibaBiCs3 || guoFlashUseType.B.btToshibaBiCs4) {
			UBYTE ubData;
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
			ubData = gFREG[FCTLL_PIO_DAT].B.B0;
			gFREG[FCTLL_PIO_CMD].B.B0 = 0xFF;
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;
			while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);
		}
	}

	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_400, 0xF);
			//Fla_Set_Offset_DQS_Delay(Toggle_400);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
				gFREG[FCTLL_FDIV_CFG].L = 0;
			}
		}
	}
	//#endif
}

void flaReadFailRetryToshibaTerminateFlow(UWORD uwFJobInfo)
{
	//#if (!RDT)
	U32 ulTemp = gFREG[FCTLL_FLH_SET].L;
	UBYTE ubToggleModeTemp = gubToggleMode;
	if (!ENABLE_DEBUG_RETRY_NOT_SWITCH_LEGACY) {
		gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
		gubToggleMode = 0;
	}

	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_Asyn40, 6);
			//Fla_Set_Offset_DQS_Delay(0);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | gubClockDivValue);
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
			}
		}
	}

	UBYTE ubIsFastPage = (uwFJobInfo & BIT_FJOBI_FASTPAGE) ? 1 : 0;
	// Terminate the Read Retry mode
	if ((guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0))  && (ubIsFastPage/* || ENABLE_ULTRAMLC*/)) {
		{
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x08;
			gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = 0;
			}

			if (guoFlashUseType.B.btToshiba1Ynm || guoFlashUseType.B.btToshiba1Znm) {
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
				gFREG[FCTLL_PIO_ADR].B.B0 = 0x0D;
				if (gubToggleMode) {
					gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
				}
				else {
					gFREG[FCTLL_PIO_DAT].B.B0 = 0;
				}
			}
		}
	}
	else {
		{
			//A
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x04;
			gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = 0;
			}
			//B
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x05;
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = 0;
			}
			//C
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x06;
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = 0;
			}
			//D
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x07;
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = 0;
			}

			if (guoFlashUseType.B.btToshiba1Ynm || guoFlashUseType.B.btToshiba1Znm) {
				//E
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
				gFREG[FCTLL_PIO_ADR].B.B0 = 0x0D;
				if (gubToggleMode) {
					gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
				}
				else {
					gFREG[FCTLL_PIO_DAT].B.B0 = 0;
				}
			}
		}
	}

	if (!ENABLE_DEBUG_RETRY_NOT_SWITCH_LEGACY) {
		gFREG[FCTLL_FLH_SET].L = ulTemp;
		gubToggleMode = ubToggleModeTemp;
	}

	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_400, 0xF);
			//Fla_Set_Offset_DQS_Delay(Toggle_400);

			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
				gFREG[FCTLL_FDIV_CFG].L = 0;
			}
		}
	}
	//#endif
}


UBYTE flaReadFailRetryCheckReadStatusInFail(UBYTE ubDepth , UBYTE ubALUIndex)
{
	//	M_CheckRDTModeToDisableFunctionReturnValid(0);
	UBYTE ubStatus_Return = 0; // 0: read ok  1: read fail

	while (gubINTTable[ubDepth].All == 0); // use this to check

	if ((gubINTTable[ubDepth].All & 0xF8) || gubFPUTimeout) {
		if ((gFREG[FCTLL_INT_INF].L & UNC_MAP_INF) != UNC_MAP_INF) { // no UNC
			if ((gFREG[FCTLL_INT_INF].L & CRC32_ERROR) != CRC32_ERROR) { // no CRC32 error
				if ((gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) != LCA_FW_CMP_ERR_INF) { // no LCA CMP error
					if ((gFREG[FCTLL_INT_INF].L & FW_SETTING_ERR_INF) != FW_SETTING_ERR_INF) { // no FW setting error
						if ((gFREG[FCTLL_INT_INF].L & OVER_ECC_THRESHOLD_INF) != OVER_ECC_THRESHOLD_INF) { // no EOT
							if (ENABLE_WP_DEBUG) {
								flaLEDBlink(0xFE);
							}
							else {
								gubWP_DEBUG = 0xFE;
								M_AssertError(1);
							}
							ubStatus_Return = 1;
						}
						else {
							VT->gsReadFailDebugCount.gulRetryECCOverTH_LogCount++;

							if (!ENABLE_DEBUG_FIX_MT_EOT_BUG) {
								if (ENABLE_WP_DEBUG) {
									flaLEDBlink(0xFD);
								}
								else {
									gubWP_DEBUG = 0xFD;
									M_AssertError(1);
								}
							}
						}
					}
					else {
						if (ENABLE_WP_DEBUG) {
							flaLEDBlink(0xFC);
						}
						else {
							gubWP_DEBUG = 0xFC;
							M_AssertError(1);
						}
						ubStatus_Return = 1;
					}
				}
				else {
					// Not open LCA compare when bypass retry but LCA error
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(0xFB);
					}
					else {
						gubWP_DEBUG = 0xFB;
						M_AssertError(1);
					}
					ubStatus_Return = 1;
				}
			}
			else {
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(0xFA);
				}
				else {
					gubWP_DEBUG = 0xFA;
					M_AssertError(1);
				}
				VT->gsReadFailDebugCount.gulRetryCRC32InCheckRead_LogCount++;
				ubStatus_Return = 1;
			}
		}
		else {
			ubStatus_Return = 1;
		}
		//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET); // fix Ping-Pong FIFO frame map mismatch cause trigger bit hang on bug
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	}
#if Read_Chk_ByReadScan
	else if ((gubRRDIRECTACESS == 0) && (gubReadDisturbCheck == 1) && (gubRetryCount_RD != 0xFF) && (gubEnterRS == 0) && (gubSwapRUT_RD == 0) && (gubRetryOK_RD != 0x5A) ) {
		UWORD uwECCBitInfo;
		uwECCBitInfo = gFREG[FCTLL_ECC_INFO].L & 0x3FF;
		gubRetryOK_RD = 1;
		//UartString("\n uwECCBitInfo");
		//UartWordHex(uwECCBitInfo);
		UWORD uwTempECCThreshold = ubALUIndex ? ECCA2Threshold : RDECCBITTH;
		if (uwECCBitInfo > uwTempECCThreshold) { //4k   ECC bits
			if (gubRetryCount_RD < gubRetryLimit_RD) {
				volatile L4KTable16B *pL4KTable;

				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (gubRetryFrameNum_RD * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubflaBuffer[(gubRetryFrameNum_RD * 4096)])) / 512;
				pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;

				ubStatus_Return = 1;
			}
			else {
				gubForceReadMove = 1;
				//gubRetryCount_RD=0xFF;
			}
		}
		else {
			//gubRetryCount_RD=0xFF;
		}
	}
	if ( (gubRRDIRECTACESS == 0) && (gubReadDisturbCheck == 1) && (gubRetryCount_RD != 0xFF) && (gubEnterRS == 0) && (gubSwapRUT_RD == 0) && (gubRetryCount_RD >= gubRetryLimit_RD) && (ubStatus_Return == 1) ) {
		if (gubRetryOK_RD == 1) {
			gubRetryOK_RD = 0x5A;
			gubRetryAgain_RD = 1;
			gubForceReadMove = 1;
			ubStatus_Return = 0;

		}
	}

#endif
	gubINTTable[ubDepth].All = 0;
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN);  // no need INT for polling

	return ubStatus_Return;

}



void flaReadFailRetryTriggerFPU(UBYTE ubALUIndex)
{



	if (ubALUIndex == ALU_A2) {

		// Magic ?
		if (gubEnterHB && (guoFlashUseType.B.btSandisk == 0) && (gubIsHBRetryLastNormalRead == 0)) {
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C30;
		}
		else {
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_CA2_C00_A5_C30;
		}
	}
	else {
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C30;
	}
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);


#if HV
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40; // To avoid read out fail
#else
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40_C00;
#endif
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x6_FPU_POL_TriggerFPU, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}

	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;



#if HV
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR; // To avoid read out fail
#else
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_DR;
#endif

	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x7_FPU_R_TriggerFPU, gFREG, CHECK_FPU_DMA_R_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}
}

void flaReadFailRetryCheckSendCmdStatus(UBYTE ubDepth, UBYTE ubRecycleDepth, UBYTE ubClearINT, UBYTE ubgFQLinkIndex)  	// ubgFQLinkIndex olny for program fail handling
{

	M_CheckBurnerModeToDisableFunction();
	//	M_CheckRDTModeToDisableFunction();
	while (gubINTTable[ubDepth].All == 0); // use this to check

	if (gubINTTable[ubDepth].B.btSTA) {


		// MLC
		VT->gulProgramFailCnt_D3++;

		gubPlaneABDebug = 0;

		flaCheckWriteStatus(ubgFQLinkIndex);

		//因為進 Check Write Status 會用 FQ 的 Depth 重填 INT VCT, 因此這邊要再填回來
		gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
		gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);


		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
			gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
		}
		VT->gulPreviousFQCacheProgramFailDebugCnt++;

		if (ENABLE_DEBUG_UART_CC) {
			UartString("\n Prev FQ Cache PF!");
		}
	}
	else if (gubINTTable[ubDepth].All & 0xF8) {
		if ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == 0) {
			if ((gFREG[FCTLL_INT_INF].L & CRC32_ERROR) == CRC32_ERROR) {
				VT->gsReadFailDebugCount.gulRetryCRC32InCheckSendCmd_LogCount++;
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
			}
			else if ((gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) == LCA_FW_CMP_ERR_INF) {
				VT->gsReadFailDebugCount.gulRetryLCAErrorInCheckSendCmd_LogCount++;
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
			}
			else if ((gFREG[FCTLL_INT_INF].L & FW_SETTING_ERR_INF) == FW_SETTING_ERR_INF) {
				VT->gsReadFailDebugCount.gulRetryFWSettingErrorInCheckSendCmd_LogCount++;
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
			}

			if ((gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) == SIGNOFFMASK_INF) {
				// 留到CHECK_DONE後進flaReadSignoffmaskFail處理
				gubTemp = 0;
			}
			else {
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
			}

			if (gFREG[FCTLL_INT_INF].L & COMPRESSION_FAIL_INF) {
				VT->gulProgramCompressionFailCnt++;
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
			}
			if (gFREG[FCTLL_INT_INF].L & FW_SETTING_ERR_INF) {
				VT->gulProgramFWSettingErrorCnt++;
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
			}
			if (gFREG[FCTLL_INT_INF].L & E3D_ERR_INF) {
				VT->gulProgramE3DErrorDebugCnt++;
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
			}
			if (gFREG[FCTLL_INT_INF].L & CRC16_ERROR) {
				VT->gulCRC16ErrorDebugCnt++;
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
			}
			if (gFREG[FCTLL_INT_INF].L & DECOMPRESSION_FAIL_INF) {
				VT->gulDeCompressionErrorDebugCnt++;
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
			}
		}
		//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET); // fix Ping-Pong FIFO frame map mismatch cause trigger bit hang on bug
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	}
	// only clear INT in rebuildFQ after clear all MTQ
	if (ENABLE_FIP_NO_HANG && (!ENABLE_DEBUG_RW) && ubClearINT) {
#if ENABLE_DO_AUTOPOL_TIMEOUT_HANDLING
		//if (ENABLE_FIP_RESET_IN_REBUILD_FQ && gubTimeoutEnterRebuildFQ && (gMTQ->Depth[ubDepth].Para0x1C.BitMap.uwFPUPtr == FPU_PTR_C70_POL_MK20) && (gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
		if (ENABLE_FIP_RESET_IN_REBUILD_FQ && gubTimeoutEnterRebuildFQ) {
			U32 ulStartTime = gulOperationTime;
			while (((gFREG[FCTLL_FPU_TRIG].L & (TIMEOUT | MT_BUSY | ANY_BUSY | SIGNOFF_BUSY | FPU_TRIG)) != 0) || (gFREG[FCTLL_INT_RDY].B.B0 != 0xFF) ) {
				xthal_dcache_line_invalidate(&gulOperationTime);
				if ((((gulOperationTime >= ulStartTime) && ((gulOperationTime - ulStartTime) > AUTOPOL_TIMEOUT_MILLISEC)) || ((gulOperationTime < ulStartTime) && ((0xFFFFFFFF - ulStartTime + gulOperationTime) > AUTOPOL_TIMEOUT_MILLISEC)))
				        || ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT)) {
					if (ENABLE_DEBUG_UART_FIP_RESET_IN_REBUILD_FQ) {
						UartString("\nFW_RST1! ");
					}
					//UBYTE ubTimeoutChannel = (gMTQ->Depth[ubDepth].Para0x1C.BitMap.ubCEValue & gubPlanesPerBurstMask);
					VT->gulTimeoutHandle_CheckCmdStatus_DebugCnt++;

					gFREG[FCTLL_INT_RDY].L |= 0xFF;
					idle(0x1999998);
					// Clear all MTQ in channel
					gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
					gFREG[FCTLL_MT_TRIG].B.B1 = 0;
					idle(0x1999998);
					flaFIPTimeoutErrorHandle(gMTQ->Depth[ubDepth].Para0x1C.BitMap.ubCEValue);
				}
			}
		}
#endif

		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
			// no hang
			VT->gulFIPNoHangDebugCnt++;
			gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
		}
	}
	gubINTTable[ubDepth].All = 0;
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
	if (ubRecycleDepth) {
		M_AddMTDepth(ubDepth);
	}
}

void flaReadFailRetryRebuildFQPhase(UBYTE ubgFQLinkIndex)
{

	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	FlashQueue_t *FQ_PTR;
	UBYTE ubDepth, ubCEIndex, ubCE_FQLinkIndex, ubCE_FQCount;
	UBYTE ubFailHandlingChannelNo = FQ->ubCEIndex & gubPlanesPerBurstMask;

	UBYTE ubForceFourPlaneProgram[MAX_CE];	// for B0KB flush D1, D3 four plane programming can not be interrupted
	for (ubCEIndex = 0; ubCEIndex < gubMTQTotalPhyCE; ubCEIndex++) {

		ubForceFourPlaneProgram[ubCEIndex] = 0;

		if (gubRSclearMTQRebuildFQ || ((ubCEIndex & gubPlanesPerBurstMask) == ubFailHandlingChannelNo)) {

			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubCEIndex & gubPlanesPerBurstMask) * 0x200));

			ubCE_FQLinkIndex = gFQI.ubFQIndex[ubCEIndex][gFQI.ubFQFirst[ubCEIndex]]; // get the first FQ in this CE
			UBYTE ubFirstReDo = 0;

			for (ubCE_FQCount = 0; ubCE_FQCount < gFQI.ubFQDoingNumber[ubCEIndex]; ) {
				FQ_PTR = &gFQI.gFQLink[ubCE_FQLinkIndex];

				if ((FQ_PTR->ubDepth != 0xFF && FQ_PTR->ubFQPhase != 0) || (ubForceFourPlaneProgram[ubCEIndex] == 1)) { // Check Phase != 0
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
					if (FQ_PTR->btUsingAutoPol) {
						// reset start time
						FQ_PTR->ulAutoPolStartTime = gulOperationTime;
					}
#endif
					if (FQ_PTR->ubFJob == BYTE_FJOB_READ) {

						UBYTE ubReadReDo = 0;

						if ((gubINTTable[FQ_PTR->ubDepth].B.btDMA_Done == 0) || (gubINTTable[FQ_PTR->ubDepth].B.btAbort_Done)) {
							ubReadReDo = 1;
						}

						if (ubReadReDo) {
							if (ubFirstReDo == 0) {

								// WAIT TRUE READY
								ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
								gMTQ->Depth[ubDepth].Para0x20.BitMap.uw2ndFPUPtr = FPU_PTR_C70_POL_MK20;
								gMTQ->Depth[ubDepth].Para0x20.BitMap.btbtIntVctEn2 = 1;
#if FORCE_AUTOPOL_TIMEOUT
								gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 0;
#else
								gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 1;
#endif
								flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_C70_POL_MK20, 0, 1, 0xFF);
								gubMTDepthPushNumber[ubCEIndex]--;
								flaReadFailRetryCheckSendCmdStatus(ubDepth, 1 , 1, DONTCARE);

								{
									// 0xFF
									ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

									flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_CFF, 1, 1, WAIT_TRUE_READY);

									gubMTDepthPushNumber[ubCEIndex]--;
									flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, DONTCARE);
								}
								ubFirstReDo = 1;
							}
#if ENABLE_RESET_RETRY_LEVEL
							if (FQ_PTR->ubBicsRetryLevelDepth != 0xFF && ((guoFlashUseType.B.btSandisk && TLC_BICS2) || ((!TLC) && (!guoFlashUseType.B.btSandisk)))) {
								gulKeepRetryLevelCE_BITMAP |= (BIT0 << ubCEIndex);
								gubINTTable[FQ_PTR->ubBicsRetryLevelDepth].All = 0;
								M_AddMTDepth(FQ_PTR->ubBicsRetryLevelDepth);
								FQ_PTR->ubBicsRetryLevelDepth = 0xFF;
							}
#endif
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
							if ((FQ->ubL4kIndex != gubTimeoutFQIndex) && (FQ_PTR->ubDepth == FQ->ubDepth))
#else
							if (FQ_PTR->ubDepth == FQ->ubDepth) // must except the UNC one
#endif
							{
								if (ENABLE_WP_DEBUG) {
									flaLEDBlink(0xF9);
								}
								else {
									gubWP_DEBUG = 0xF9;
									M_AssertError(1);
								}
							}

							// restore to initial phase
							if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
								gubMTDepthPushNumber[ubCEIndex] -= FQ_PTR->ubMTDepthCnt ;
								FQ_PTR->ubMTDepthCnt = 0;
							}
							FQ_PTR->ubFQPhase = 0;
							FQ_PTR->btNeedSetCEWhenTriggerDMA = 0;
							gFQI.ubXferPhase[ubCEIndex] = 0;
							gulNotFinishPhase[ubCEIndex] = 0;
							gubNoFQIn[ubCEIndex] = 0;
							gubINTTable[FQ_PTR->ubDepth].All = 0;
							gubFQNeedExecuteRightNow[ubCEIndex] = 0;
							FQ_PTR->btRedoFQ = 1;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
							FQ_PTR->btUsingAutoPol = 0;
#endif

							FQ_PTR->FQPara.B.btCanRemove = 0;
							if (ENABLE_MULTIPLANE_READ) {
								gubFQNotSequential[ubCEIndex] = 0;
								gubToCheckNextCmdSequence[ubCEIndex] = 0;
								if (FQ_PTR->ubRelativeDepth != 0xFF) {
									if (FQ_PTR->ubRelativeDepth != FQ_PTR->ubDepth) {
										gubINTTable[FQ_PTR->ubRelativeDepth].All = 0;
										M_AddMTDepth(FQ_PTR->ubRelativeDepth);
									}
									FQ_PTR->ubRelativeDepth = 0xFF;
								}
							}
						}
					}
					else {
						// WRITE or ERASE
						// Check FQ->ubDepth (Old Poll Fail function) & FQ->ubRelativeDepth
						if (ENABLE_MULTIPLANE_PROGRAM) {
							if (gubINTTable[FQ_PTR->ubDepth].B.btAll_Done == 0 || FQ_PTR->FQPara.B.btCanRemove == 0) {

								if (gubINTTable[FQ_PTR->ubRelativeDepth].B.btAll_Done) { // 80h, 11h, 15h

									if (FQ_PTR->ubWritePhaseRedoCmd == MARK_FPU_C80_C10C15 || FQ_PTR->ubWritePhaseRedoCmd == MARK_FPU_NonC80_C10C15) {

										if (FQ_PTR->btRSPatchUpWritePhaseCmd == 0) {
											// POL
											ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
#if (ENABLE_ULTRAMLC)
											gMTQ->Depth[ubDepth].Para0x20.BitMap.uw2ndFPUPtr = FPU_PTR_C70_POL_MK20;
											gMTQ->Depth[ubDepth].Para0x20.BitMap.btbtIntVctEn2 = 1;
#if FORCE_AUTOPOL_TIMEOUT
											gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 0;
#else
											gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 1;
#endif
#endif
											flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_C70_POL_MK20, 0, 1, 0xFF);


											gubMTDepthPushNumber[ubCEIndex]--;
											flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, DONTCARE);

											// POL
											ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

											flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, flaReadFailStatusFPUPointer(ubCE_FQLinkIndex), 0, 1, 0xFF);

											gubMTDepthPushNumber[ubCEIndex]--;
											flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, ubCE_FQLinkIndex);

											//gulRetry_RelativeFinishedRedoWrite_DebugCount++;
										}
										else {
											FQ_PTR->btRSPatchUpWritePhaseCmd = 0;
										}
									}
									else if (ubFirstReDo == 0) {
										//gulRetry_RelativeFinished10h_DebugCount++;

										if (FQ_PTR->btRSPatchUpWritePhaseCmd == 0) {

											if ((FQ_PTR->ubRedundantDepth == 0xFF) || (gubINTTable[FQ_PTR->ubRedundantDepth].B.btAll_Done == 0) ) {	//原本沒有補10h or 10h補了但還沒有做完時, 這邊才能再補10h
												// 10h for previous
												ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
#if (ENABLE_ULTRAMLC)
												{
													gMTQ->Depth[ubDepth].Para0x20.BitMap.uw2ndFPUPtr = FPU_PTR_C10;
													gMTQ->Depth[ubDepth].Para0x20.BitMap.btbtIntVctEn2 = 1;
#if FORCE_AUTOPOL_TIMEOUT
													gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 0;
#else
													gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 1;
#endif
													flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_C10, 1, 1, WAIT_TRUE_READY);
												}
#else
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
												gubDebugSendCmdFQIndex = ubCE_FQLinkIndex;
#endif
												flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_C10, 1, 1, WAIT_TRUE_READY);
#endif
												gubMTDepthPushNumber[ubCEIndex]--;
												flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, DONTCARE);

												// POL FAIL
												ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

												flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, flaReadFailStatusFPUPointer(ubCE_FQLinkIndex), 0, 1, 0xFF); // for 4 plane, 若有fail, 再下71h/73h去看哪個plane fail

												gubMTDepthPushNumber[ubCEIndex]--;
												flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, ubCE_FQLinkIndex);
											}
										}
										else {
											FQ_PTR->btRSPatchUpWritePhaseCmd = 0;
										}

										ubFirstReDo = 1;
									}
								Mark_Direct_Pol_True_Ready:
									// WAIT TRUE READY
									ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
									gMTQ->Depth[ubDepth].Para0x20.BitMap.uw2ndFPUPtr = FPU_PTR_C70_POL_MK20;
									gMTQ->Depth[ubDepth].Para0x20.BitMap.btbtIntVctEn2 = 1;
#if FORCE_AUTOPOL_TIMEOUT
									gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 0;
#else
									gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 1;
#endif
									flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_C70_POL_MK20, 0, 1, 0xFF);

									gubMTDepthPushNumber[ubCEIndex]--;
									flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, DONTCARE);
									{
										// 0xFF
										ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
										flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_CFF, 1, 1, WAIT_TRUE_READY);

										gubMTDepthPushNumber[ubCEIndex]--;
										flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, DONTCARE);
									}
									gubINTTable[FQ_PTR->ubDepth].B.btAll_Done = 1;
									FQ_PTR->FQPara.B.btCanRemove = 1;
									//M_AssertError(FQ_PTR->btCanRemove != 1);

									gFQI.ubXferPhase[ubCEIndex] = 0;
									gulNotFinishPhase[ubCEIndex] = 0;
									gubNoFQIn[ubCEIndex] = 0;
									gubFQNeedExecuteRightNow[ubCEIndex] = 0;
								}
								else {
									//gulRetry_RelativeNoFinishedRedoWrite_DebugCount++;

									if (FQ_PTR->ubRelativeDepth != FQ_PTR->ubDepth) {
										gubINTTable[FQ_PTR->ubDepth].All = 0;
										M_AddMTDepth(FQ_PTR->ubDepth);
									}

									if (FQ_PTR->ubRemainderDepth != 0xFF) {
										M_AssertError(FQ_PTR->ubRemainderDepth == FQ_PTR->ubDepth);
										M_AssertError(FQ_PTR->ubRemainderDepth == FQ_PTR->ubRelativeDepth);
										gubINTTable[FQ_PTR->ubRemainderDepth].All = 0;
										M_AddMTDepth(FQ_PTR->ubRemainderDepth);
										FQ_PTR->ubRemainderDepth = 0xFF;
									}

									if (FQ_PTR->ubRedundantDepth != 0xFF) {
										M_AssertError(FQ_PTR->ubRedundantDepth == FQ_PTR->ubDepth);
										M_AssertError(FQ_PTR->ubRedundantDepth == FQ_PTR->ubRelativeDepth);
										M_AssertError(FQ_PTR->ubRedundantDepth == FQ_PTR->ubRemainderDepth);
										gubINTTable[FQ_PTR->ubRedundantDepth].All = 0;
										M_AddMTDepth(FQ_PTR->ubRedundantDepth);
										FQ_PTR->ubRedundantDepth = 0xFF;
									}

									if (ubFirstReDo == 0) {
										//gulRetry_RelativeNoFinished10h_DebugCount++;

										if (FQ_PTR->ubWritePhaseRedoCmd == MARK_FPU_NonC80_C10C15 || FQ_PTR->ubWritePhaseRedoCmd == MARK_FPU_NonC80_NonC10C15) {

											if (FQ_PTR->btRSPatchUpWritePhaseCmd == 0) {

												// 10h for previous
#if (ENABLE_ULTRAMLC)
												{
													if ((FQ_PTR->ubRedundantDepth == 0xFF) || (gubINTTable[FQ_PTR->ubRedundantDepth].B.btAll_Done == 0) ) {	//原本沒有補10h or 10h補了但還沒有做完時, 這邊才能再補10h
														ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
														gMTQ->Depth[ubDepth].Para0x20.BitMap.uw2ndFPUPtr = FPU_PTR_C10;
														gMTQ->Depth[ubDepth].Para0x20.BitMap.btbtIntVctEn2 = 1;
#if FORCE_AUTOPOL_TIMEOUT
														gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 0;
#else
														gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 1;
#endif
														flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_C10, 1, 1, WAIT_TRUE_READY); // previous is not 10h/15h, is cache write

														gubMTDepthPushNumber[ubCEIndex]--;
														flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, DONTCARE);

														// POL FAIL
														ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
														flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, flaReadFailStatusFPUPointer(ubCE_FQLinkIndex), 0, 1, 0xFF); // for 4 plane, 若有fail, 再下71h/73h去看哪個plane fail
														gubMTDepthPushNumber[ubCEIndex]--;
														flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, ubCE_FQLinkIndex);
													}
												}
#endif

											}
											else {
												FQ_PTR->btRSPatchUpWritePhaseCmd = 0;
											}
										}
										{
											// WAIT TRUE READY
											ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
											gMTQ->Depth[ubDepth].Para0x20.BitMap.uw2ndFPUPtr = FPU_PTR_C70_POL_MK20;
											gMTQ->Depth[ubDepth].Para0x20.BitMap.btbtIntVctEn2 = 1;
#if FORCE_AUTOPOL_TIMEOUT
											gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 0;
#else
											gMTQ->Depth[ubDepth].Para0x20.BitMap.btSetInternalBusy2 = 1;
#endif
											flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_C70_POL_MK20, 0, 1, 0xFF);
											gubMTDepthPushNumber[ubCEIndex]--;
											flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, DONTCARE);
										}
										{
											// 0xFF
											ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

											flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, FPU_PTR_CFF, 1, 1, WAIT_TRUE_READY);

											gubMTDepthPushNumber[ubCEIndex]--;
											flaReadFailRetryCheckSendCmdStatus(ubDepth, 1, 1, DONTCARE);
										}
										ubFirstReDo = 1;
									}

#if ENABLE_RESET_RETRY_LEVEL
									if (FQ_PTR->ubBicsRetryLevelDepth != 0xFF && ((guoFlashUseType.B.btSandisk && TLC_BICS2) || ((!TLC) && (!guoFlashUseType.B.btSandisk)))) {
										gulKeepRetryLevelCE_BITMAP |= (BIT0 << ubCEIndex);
										gubINTTable[FQ_PTR->ubBicsRetryLevelDepth].All = 0;
										M_AddMTDepth(FQ_PTR->ubBicsRetryLevelDepth);
										FQ_PTR->ubBicsRetryLevelDepth = 0xFF;
									}
#endif
									// restore to initial phase
									if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
										gubMTDepthPushNumber[ubCEIndex] -= FQ_PTR->ubMTDepthCnt ;
										FQ_PTR->ubMTDepthCnt = 0;
									}
									FQ_PTR->ubFQPhase = 0;
									gFQI.ubXferPhase[ubCEIndex] = 0;
									gulNotFinishPhase[ubCEIndex] = 0;
									gubNoFQIn[ubCEIndex] = 0;
									gubFQNeedExecuteRightNow[ubCEIndex] = 0;
									FQ_PTR->FQPara.B.btCanRemove = 0;
									FQ_PTR->btRedoFQ = 1;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
									FQ_PTR->btUsingAutoPol = 0;
#endif


								}

							}
						}
						else {
							if (gubINTTable[FQ_PTR->ubRelativeDepth].B.btAbort_Done || gubINTTable[FQ_PTR->ubRelativeDepth].B.btPartial_Done) {
								if (ENABLE_WP_DEBUG) {
									flaLEDBlink(0xF8);
								}
								else {
									gubWP_DEBUG = 0xF8;
									M_AssertError(1);
								}
							}
							else if ((gubINTTable[FQ_PTR->ubRelativeDepth].B.btAll_Done == 0) || (gubINTTable[FQ_PTR->ubDepth].B.btAll_Done == 0)) {
								// Redo write
								gubINTTable[FQ_PTR->ubDepth].All = 0;
								M_AddMTDepth(FQ_PTR->ubDepth);

								if (FQ_PTR->ubRemainderDepth != 0xFF) {
									M_AssertError(FQ_PTR->ubRemainderDepth == FQ_PTR->ubDepth);
									M_AssertError(FQ_PTR->ubRemainderDepth == FQ_PTR->ubRelativeDepth);
									gubINTTable[FQ_PTR->ubRemainderDepth].All = 0;
									M_AddMTDepth(FQ_PTR->ubRemainderDepth);
									FQ_PTR->ubRemainderDepth = 0xFF;
								}

								if (FQ_PTR->ubRedundantDepth != 0xFF) {
									M_AssertError(FQ_PTR->ubRedundantDepth == FQ_PTR->ubDepth);
									M_AssertError(FQ_PTR->ubRedundantDepth == FQ_PTR->ubRelativeDepth);
									M_AssertError(FQ_PTR->ubRedundantDepth == FQ_PTR->ubRemainderDepth);
									gubINTTable[FQ_PTR->ubRedundantDepth].All = 0;
									M_AddMTDepth(FQ_PTR->ubRedundantDepth);
									FQ_PTR->ubRedundantDepth = 0xFF;
								}

								if (gubINTTable[FQ_PTR->ubRelativeDepth].B.btAll_Done) { // All_Done will be push before auto polling (no ECO case)
									// Redo only polling
									/*if (ENABLE_WP_DEBUG) {
										gulRetryUDMA_WritePOLerror_DebugCount++;
									}*/
									FQ_PTR->ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];

									flaSendCmd_CH(ubCE_FQLinkIndex, FQ_PTR->ubCEIndex, flaReadFailStatusFPUPointer(ubCE_FQLinkIndex), 0, 1, 0xFF);

									gubMTDepthPushNumber[ubCEIndex]--;
								}
								else {
									if (ENABLE_DEBUG_MTREMAINCNT_REG == 0) {
										gubMTDepthPushNumber[ubCEIndex] -= FQ_PTR->ubMTDepthCnt ;
										FQ_PTR->ubMTDepthCnt = 0;
									}
									FQ_PTR->ubFQPhase = 0;
									gFQI.ubXferPhase[ubCEIndex] = 0;
									gulNotFinishPhase[ubCEIndex] = 0;
									gubNoFQIn[ubCEIndex] = 0;
									gubFQNeedExecuteRightNow[ubCEIndex] = 0;
									FQ_PTR->FQPara.B.btCanRemove = 0;
									FQ_PTR->btRedoFQ = 1;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
									FQ_PTR->btUsingAutoPol = 0;
#endif
								}
#if ENABLE_RESET_RETRY_LEVEL
								if (FQ_PTR->ubBicsRetryLevelDepth != 0xFF && ((guoFlashUseType.B.btSandisk && TLC_BICS2) || ((!TLC) && (!guoFlashUseType.B.btSandisk)))) {
									gulKeepRetryLevelCE_BITMAP |= (BIT0 << ubCEIndex);
									gubINTTable[FQ_PTR->ubBicsRetryLevelDepth].All = 0;
									M_AddMTDepth(FQ_PTR->ubBicsRetryLevelDepth);
									FQ_PTR->ubBicsRetryLevelDepth = 0xFF;
								}
#endif
							}
						}
					}
				}
				ubCE_FQCount++;
				ubCE_FQLinkIndex = gFQI.ubFQIndex[ubCEIndex][((gFQI.ubFQFirst[ubCEIndex] + ubCE_FQCount) & FQ_NUM_MASK)];
			}

		}
		//M_AssertError(ubForceFourPlaneProgram[FQ_PTR->ubCEIndex] == 1);	// pseudo CE後面的CE沒有初始, 會因為沒初始的亂數而assert
		M_AssertError(ubForceFourPlaneProgram[ubCEIndex] == 1);
	}
	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubFailHandlingChannelNo * 0x200));
#if UART_AUTOPOL_TIMEOUT_HANDLING
	UartString("\nRebuild OK");
#endif
}

void flaReadFailRetryResetIBFPTR(UBYTE ubChannel, UBYTE ubDepth)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	if (ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
		FSA_t * pFSA = (FSA_t *)gulFSABase[ubChannel];
		UBYTE ubL4KCount;
		volatile L4KTable16B *pL4KTable;

		gFREG[FCTLL_OTHER_SET].L |= FORCE_FIX_DATA_TO_FLH;
		if (!ENABLE_iFSA) {
			gFREG[FCTLL_DMA_CFG].L &= (SEL_OTHER_FSA & ALU0_GROUP_SEL); // fix fw setting error bug at second call
			gFREG[FCTLL_FSA_SEL].L  = gubFSAIndex[ubChannel]; // fix fw setting error bug at second call
		}
		gFREG[FCTLL_ZIP_MAP].L = 0xFFFFFFFF;
		gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
		gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(4);
		gFREG[FCTLL_DMA_CFG].L &= TRANSFER_0_ECC_FRAME;
		gFREG[FCTLL_DMA_CFG].L |= (((UWORD)4) << 8);
		//Set FSA
		if (!ENABLE_iFSA) {
			pFSA[gubFSAIndex[ubChannel]].uword.W0 = 0;
			pFSA[gubFSAIndex[ubChannel]].uword.W1 = 0;
			pFSA[gubFSAIndex[ubChannel]].B.B4 = 0;
		}
		else {
			gFREG[FCTLL_IFSA0].L = 0;
		}

		for (ubL4KCount = 0; ubL4KCount < 4; ubL4KCount++) {
			pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + TEMP_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
			//set L4K table info
			pL4KTable->ulL4K_LCA = 0;
			pL4KTable->Para0x04.ulAll = 0;
			pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
			pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubflaBuffer[16384])) / 512;
			pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
			pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
		}
		gFREG[FCTLL_L4K_SPR_ADR].L = TEMP_L4K_TABLE_OFFSET;

		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;




#if HV
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR; // To avoid read out fail
#else
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_DR;//FPU_PTR_C00_A5_C05_A2_CE0_DR;
#endif


		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

		gFREG[FCTLL_OTHER_SET].L &= (~FORCE_FIX_DATA_TO_FLH);
		//FCONL[FCONL_FLH_FUNC] &= (~IBF_RING_MODE_EN);
	}
}

void flaReadSignoffmaskFail (UBYTE ubgFQLinkIndex)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubDepth = FQ->ubDepth;

	// force empty to avoid program parity clear interrupt vector enable bit bug
	gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);

	// wait the channel idle for read retry
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	// wait the channel true idle when force empty
	gFREG[FCTLL_DBG_INF].L = (0x0C);
	while ((GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));

	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix 2nd program parity hang on bug

	UBYTE ubFailL4KIndex = 0;
	gulReadFail_4KMAP = 0;
	U32 ulSignoffMaskFailFrameMapInMT = 0;
	if (FQ->btUncCheckInRSTwoCh) {
		ulSignoffMaskFailFrameMapInMT = FQ->ulFIPError.All;
	}
	else {
		gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
		gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(SIGNOFFMASK_MAP);
		ulSignoffMaskFailFrameMapInMT = gFREG[FCTLL_MAP_INF].L;
	}
	M_Get_Error_4K_Index(ubFailL4KIndex, (gubMaxZipRate * gub4kEntrysPerPlane), ulSignoffMaskFailFrameMapInMT); // max: 16個L4K
	gulReadFail_4KMAP |= ((U32)BIT0 << ubFailL4KIndex);

	UBYTE ubCheckRealSignoffMaskFail = 0;
	volatile L4KTable16B * pL4KTablePtr;
	U32 ulAddr = 0;
	UBYTE ubL4KIndex = 0;
	U32 uli = 0;
	for (ubL4KIndex = ubFailL4KIndex; ubL4KIndex < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubL4KIndex++) {

		pL4KTablePtr = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KIndex) * Spare4kSize));
		ulAddr = (pL4KTablePtr->Para0x0C.BitMap.ulL4K_BADR * 0x200);

		for (uli = 0; uli < 8; uli++) {
			if (pL4KTablePtr->Para0x08.BitMap.ubL4K_BUF_MASK & (BIT0 << uli)) {
				if (gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) {
					break;
				}
				else if ((pL4KTablePtr->Para0x04.BitMap.ubL4K_SPRV & (BIT0 << uli)) == 0) {
					break;
				}
			}
		}

		if (uli == 8) {
			continue;
		}

		if (ENABLE_DEBUG_UART_FAIL) {
			UartString("\nSignoffFailID ");
			Uart_Tx_DataHex(ubgFQLinkIndex);
			UartString(" LCA ");
			UartLongHex(pL4KTablePtr->ulL4K_LCA);
			UartString(" ProgramFrom ");
			Uart_Tx_DataHex(pL4KTablePtr->Para0x08.BitMap.ubL4K_FW2);
		}

		if (FQ->btSpareInvalid == 0) {
			VT->gulSignOffFail++;
		}

		ubCheckRealSignoffMaskFail = 1;
		FQ->btSpareInvalid = 1;

		if (FQ->FQPara.B.btUserData) {
			ulAddr += uli * 0x200;
			U32 ulFailLBA;
			volatile L4KTable16B * pL4KTablePtr2 = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubFailL4KIndex) * Spare4kSize));
			ulFailLBA =  pL4KTablePtr2->ulL4K_LCA + uli;
			flaReadFailRetrySetUNCAddr(ulAddr, ulFailLBA);
		}
		else if (VT->gulFTLState.B.btBG_Copy && gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btBufModeEn) { // BG Read FQ will read data to write buffer
			// 目前只有記第一個fail的frame...把後面也都標成fail吧
			ulAddr = (pL4KTablePtr->Para0x0C.BitMap.ulL4K_BADR - BUFFER2_WRITE_START_INDEX) >> SectorsPer4KLog; // write buffer 4k index
			gulBG_ReadFail4k_BitMap |= BIT0 << (ulAddr & 31);
			if (ENABLE_DEBUG_UART_BG) {
				UartString("\nf_BGR");
				Uart_Tx_DataHex(ulAddr);
			}
			if (ENABLE_HANDLE_BG_READ_FAIL == 0) {
				gubBG_Read_UNC = 1;
			}
		}
	}

	if (!ubCheckRealSignoffMaskFail) {
		if (ENABLE_WP_DEBUG) {
			flaLEDBlink(1);
		}
		else {
			M_AssertError(1);
		}
		FQ->btSpareInvalid = 0;
	}

Mark_RebuildFQ:
	// Clear all MTQ in channel
	gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
	gFREG[FCTLL_MT_TRIG].B.B1 = 0;

	if (!ENABLE_DEBUG_RESUME_STA_ERROR_BUG) {
		flaReadFailRetryForceIRDYtoReady(ubgFQLinkIndex);
	}

	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= ABORT_DONE_INTR_EN;

	gFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
	gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);

	if (Failed_Test_UNC || ENABLE_DEBUG_HW_FORCEREADFAIL) {
		gFREG[FCTLL_UNC_CFG].L = 0;
		gMTQ->Depth[ubDepth].Para0x00.BitMap.btForceReadFail = 0; // can remove
	}

	if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
		gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
	}

	// UDMA target must redo whether it's error or non-error, and we don't want to change the order of FQ, so we redo all those FQ in same channel
	flaReadFailRetryRebuildFQPhase(ubgFQLinkIndex);
#endif
}

UBYTE flaReadFailRetryGetL4KFailNo(UBYTE ubgFQLinkIndex, UBYTE ubFailFrameNoInMT)
{
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubStartFrameNo = FQ->ulFEntry & gub4kEntrysPerPlaneMask;
	UBYTE ubFailL4KNo = 0;

	if (gubEnterReadFailRetryCondition == ERASE_PAGE_ERR) {
		ubFailL4KNo = 0;
	}
	else if ((gubEnterReadFailRetryCondition == FW_SETTING_ERR_MAP) || (gubEnterReadFailRetryCondition == CRC16_ERR_MAP) || (gubEnterReadFailRetryCondition == DQSCNT_MISMATCH)) {
		volatile L4KTable16B * pL4KTablePtr;
		volatile L4KTable16B * pDUMMYL4K_BUFMASK_TABLE;
		UBYTE ubi = 0;
		for (ubFailL4KNo = 0; ubFailL4KNo < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubFailL4KNo++) {

			pDUMMYL4K_BUFMASK_TABLE = pL4KTablePtr = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubFailL4KNo) * Spare4kSize));
			if (gubFPUCORClearBufMask) {
				pDUMMYL4K_BUFMASK_TABLE = (volatile L4KTable16B *) (IRAM_BVCI_BASE + DUMMY_L4K_TABLE_OFFSET + (ubFailL4KNo * Spare4kSize));
			}
			for (ubi = 0; ubi < 8; ubi++) {
				if (pDUMMYL4K_BUFMASK_TABLE->Para0x08.BitMap.ubL4K_BUF_MASK & (BIT0 << ubi)) {
					if ((pL4KTablePtr->Para0x04.BitMap.ubL4K_SPRV & (BIT0 << ubi)) == 0) {
						break;
					}
				}
			}

			if (ubi < 8) {
				break;
			}
		}
	}
	else if ((gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) || (gubEnterReadFailRetryCondition == DECOMPRESSION_FAIL_MAP)) {
		ubFailL4KNo = (UBYTE)(gulGetUNCFrameMapInMT & 0xFF);
	}
	else {

		if (!gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btZipBypass) { // Get fail L4K

			if (ubFailFrameNoInMT != ubStartFrameNo) { // 有跨frame算fail L4K No的問題
				// fail L4K No 會>0

				UBYTE ubL4KCount = 0;
				volatile L4KTable16B *pL4KTable;

				//if ((!ENABLE_DEBUG_BYPASS_GEN_ZIPHEADER) || gulReadFail_4KMAP) {
				if (1) {
					VT->gsReadFailDebugCount.gulUNC_GetFailL4KNo_DebugCount++;

					UBYTE ubFailFrameNum = 0;
					while (ubFailFrameNum < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum) {
						if (gulGetUNCFrameMapInMT & (1 << (ubStartFrameNo + ubFailFrameNum))) {
							break;
						}
						ubFailFrameNum++;
					}
					if (ENABLE_DEBUG_RW && (ubFailFrameNum == gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum || (ubFailFrameNoInMT != (ubStartFrameNo + ubFailFrameNum)))) {
						if (ENABLE_WP_DEBUG) {
							flaLEDBlink(0xF7);
						}
						else {
							gubWP_DEBUG = 0xF7;
							M_AssertError(1);
						}
					}
					ubL4KCount = 0;
					U32 ulZCODE_SUM = 0;
					UBYTE ubCheckZipMap = 0;
					while (ubCheckZipMap < 8) {
						if (gMTQ->Depth[FQ->ubDepth].ulZipMap & (1 << ((ubStartFrameNo * 8) + ubCheckZipMap))) {
							break;
						}
						ubCheckZipMap++;
					}
					if (ubCheckZipMap == 8) {
						if (ENABLE_WP_DEBUG) {
							flaLEDBlink(0xF6);
						}
						else {
							gubWP_DEBUG = 0xF6;
							M_AssertError(1);
						}
					}
					ulZCODE_SUM = ubCheckZipMap * 512;

					while (ubL4KCount < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum) {
						pL4KTable = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
						ulZCODE_SUM += (((pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE - 1) * 512) + 1024);
						ubFailL4KNo = ubL4KCount;
						if (ulZCODE_SUM > (ubFailFrameNum * 4096)) {
							break;
						}
						ubL4KCount++;
					}
				}
				else { // old method use zip header to calculate

					// fail L4K No 會>0
					UBYTE ubZSector = 0;
					UBYTE ubL4KTag = 0;
					UBYTE ubSetL4KTag = 0;
					volatile P4KTable16B *P4KTable = (volatile P4KTable16B*)(IRAM_BVCI_BASE + TEMP_L4K_TABLE_OFFSET);
					while (ubZSector < 8) {
						if (gMTQ->Depth[FQ->ubDepth].ulZipMap & (1 << ((ubStartFrameNo * 8) + ubZSector))) {
							ubL4KTag = ((((UWORD)P4KTable->Para0x08.BitMap.uwP4k_Head) >> ubZSector) & 1);
							ubSetL4KTag = 1;
							break;
						}
						ubZSector++;
					}

					if (ENABLE_WP_DEBUG && (ubSetL4KTag == 0)) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(ubSetL4KTag == 0);
					}
					UBYTE ubCheck4K_IDX = 0;
					while (ubCheck4K_IDX < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum) {
						P4KTable = (volatile P4KTable16B*)(IRAM_BVCI_BASE + TEMP_L4K_TABLE_OFFSET + (ubCheck4K_IDX * Spare4kSize));
						while (ubZSector < 8) {
							if (gMTQ->Depth[FQ->ubDepth].ulZipMap & (1 << ((ubStartFrameNo + ubCheck4K_IDX) * 8 + ubZSector))) {

								if (((((UWORD)P4KTable->Para0x08.BitMap.uwP4k_Head) >> ubZSector) & 1) != ubL4KTag) {
									ubL4KTag = ((((UWORD)P4KTable->Para0x08.BitMap.uwP4k_Head) >> ubZSector) & 1);
									ubFailL4KNo++;
								}
							}
							else {
								break;
							}
							ubZSector++;
						}
						ubZSector = 0;
						if ((ubStartFrameNo + ubCheck4K_IDX) == (ubFailFrameNoInMT - 1)) {
							if (((((UWORD)P4KTable->Para0x08.BitMap.uwP4k_Head) >> 8) & 7) == 0) {
								ubFailL4KNo++; // no cross frame, need plus 1
							}
							break;
						}
						ubCheck4K_IDX++;
					}

					// Clear bufMask
					for (ubL4KCount = 0; ubL4KCount < ubFailL4KNo; ubL4KCount++) {
						pL4KTable = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
						pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK =  0;
					}
				}
			}
		}
		else {
			ubFailL4KNo = (ubFailFrameNoInMT - ubStartFrameNo);
		}
	}

	return ubFailL4KNo;
}

void flaReadFailRetryForceIRDYtoReady(UBYTE ubgFQLinkIndex)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	FlashQueue_t *FQ_PTR;
	UBYTE ubCEIndex, ubCE_FQLinkIndex, ubCE_FQCount;
	UBYTE ubFailHandlingChannelNo = FQ->ubCEIndex & gubPlanesPerBurstMask;

	for (ubCEIndex = 0; ubCEIndex < gubMTQTotalPhyCE; ubCEIndex++) {

		if (gubRSclearMTQRebuildFQ || ((ubCEIndex & gubPlanesPerBurstMask) == ubFailHandlingChannelNo)) {

			ubCE_FQLinkIndex = gFQI.ubFQIndex[ubCEIndex][gFQI.ubFQFirst[ubCEIndex]]; // get the first FQ in this CE
			UBYTE ubFirstReDo = 0;

			for (ubCE_FQCount = 0; ubCE_FQCount < gFQI.ubFQDoingNumber[ubCEIndex]; ) {
				FQ_PTR = &gFQI.gFQLink[ubCE_FQLinkIndex];

				if (FQ_PTR->ubDepth != 0xFF && FQ_PTR->ubFQPhase != 0) { // Check Phase != 0

					if (FQ_PTR->ubFJob == BYTE_FJOB_READ) {

						UBYTE ubReadReDo = 0;

						if ((gubINTTable[FQ_PTR->ubDepth].B.btAll_Done == 0) || (gubINTTable[FQ_PTR->ubDepth].B.btAbort_Done)) {
							ubReadReDo = 1;
						}

						if (ubReadReDo) {
							if (ubFirstReDo == 0) {
								// 20151006 fix STA bug [Work around]
								gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((FQ_PTR->ubCEIndex & gubPlanesPerBurstMask) * 0x200));
								gFREG[FCTLL_INT_RDY].L |= (BIT0 << (ubCEIndex >> gubPlanesPerBurstLog));
							}
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
							if ((FQ->ubL4kIndex != gubTimeoutFQIndex) && (FQ_PTR->ubDepth == FQ->ubDepth))
#else
							if (FQ_PTR->ubDepth == FQ->ubDepth) // must except the UNC one
#endif
							{
								if (ENABLE_WP_DEBUG) {
									flaLEDBlink(0xF5);
								}
								else {
									gubWP_DEBUG = 0xF5;
									M_AssertError(1);
								}
							}
						}
					}
					else {
						// WRITE or ERASE
						// Check FQ->ubDepth (Old Poll Fail function) & FQ->ubRelativeDepth
						if (ENABLE_MULTIPLANE_PROGRAM) {
							if (gubINTTable[FQ_PTR->ubDepth].B.btAll_Done == 0 || FQ_PTR->FQPara.B.btCanRemove == 0) {

								if (gubINTTable[FQ_PTR->ubRelativeDepth].B.btAll_Done) { // 80h, 11h, 15h
									// 20151022 fix STA bug [Workaround]
									gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((FQ_PTR->ubCEIndex & gubPlanesPerBurstMask) * 0x200));
									gFREG[FCTLL_INT_RDY].L |= (BIT0 << (ubCEIndex >> gubPlanesPerBurstLog));

								}
								else {
									if (ubFirstReDo == 0) {
										// 20151022 fix STA bug [Workaround]
										gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((FQ_PTR->ubCEIndex & gubPlanesPerBurstMask) * 0x200));
										gFREG[FCTLL_INT_RDY].L |= (BIT0 << (ubCEIndex >> gubPlanesPerBurstLog));
									}
								}

							}
						}
					}
				}
				ubCE_FQCount++;
				ubCE_FQLinkIndex = gFQI.ubFQIndex[ubCEIndex][((gFQI.ubFQFirst[ubCEIndex] + ubCE_FQCount) & FQ_NUM_MASK)];
			}

		}

	}

	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubFailHandlingChannelNo * 0x200));
}

UBYTE flaReadFailRetryFPUCORRecoverData(UBYTE ubgFQLinkIndex, UBYTE ubDepth, UBYTE ubFailFrameNoInMT)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	UBYTE ubStartFrameNo = FQ->ulFEntry & gub4kEntrysPerPlaneMask;
	UBYTE ubFrameNum = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum;

	UBYTE ubPass;
	UBYTE ubBackupCRC32Enable = ((FCONL[FCONL_CRC_EN] & CRC_32_EN) ? 1 : 0);

	ubPass = flaReadFailRetryDummyFPUCOR(ubgFQLinkIndex, ubDepth, ubFailFrameNoInMT);
	if (ubPass == 0) {
		// Dummy FPUCOR fail
		return ubPass;
	}


	// Re-Fill L4K table, because L4K table will be overwrite if some one is correctable when zipbypass
	if (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btZipBypass) {

		if (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btL4kBADRContModeEn) { // not handle
			if (ENABLE_WP_DEBUG) {
				flaLEDBlink(0xF4);
			}
			else {
				gubWP_DEBUG = 0xF4;
				M_AssertError(1);
			}
		}
		flaReadFailRetryReFillL4KTable(ubgFQLinkIndex);
	}


	if ((FQ->uwFJobInfo & BIT_FJOBI_BUFFERMODE) && (gubEnterReadFailRetryCondition != LCA_FW_CMP_FAIL_MAP)) {
		// Clear bufMask
		if (ENABLE_WP_DEBUG && (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btZipBypass)) {
			flaLEDBlink(1);
		}
		else {
			M_AssertError(gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btZipBypass);
		}
		volatile L4KTable16B * pL4KTablePtr;
		UBYTE ubL4KIndex = 0;
		UBYTE ubi = 0;
		for (ubL4KIndex = 0; ubL4KIndex < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubL4KIndex++) {

			pL4KTablePtr = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KIndex) * Spare4kSize));

			for (ubi = 0; ubi < 8; ubi++) {
				if (pL4KTablePtr->Para0x08.BitMap.ubL4K_BUF_MASK & (BIT0 << ubi)) {
					if (pL4KTablePtr->Para0x04.BitMap.ubL4K_SPRV & (BIT0 << ubi)) {
						pL4KTablePtr->Para0x08.BitMap.ubL4K_BUF_MASK &= (~(BIT0 << ubi));
						gubFPUCORClearBufMask = 1;
					}
				}
			}
		}
	}

	if (ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
		// Reset IBF PTR by FW to solve disable IBF_RING_MODE_EN will cause another channel IBF PTR error bug
		flaReadFailRetryResetIBFPTR(ubChannel, ubDepth);
	}
	else {
		if (ubChannel == 0) {
			FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH0_EN);
		}
		else {
			FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH1_EN);
		}
	}

	// Restore FPU
	if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE && gubRSNormalModeCorrectDataBefore) {
		// copy correct data & spare
		U32 ulSpareAddr = CorrectRSSpareStart + (gubRSCorrectPageIndex * Spare16kSize);
		memcpy((void *)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET), (void *)(ulSpareAddr + (ubStartFrameNo * Spare4kSize)), (ubFrameNum * Spare4kSize));
	}
	gFREG[FCTLL_BACK_STORE].W.W0 = (HB_RETRY_L4K_TABLE_OFFSET);
	if (gubEnterRS) {
		if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE) {
			gFREG[FCTLL_RAW_DMA_ADR].L = (U32)(CorrectRSDataStart + (gubRSCorrectPageIndex * 16384) + (ubStartFrameNo * 4096));
		}
		else {
			gFREG[FCTLL_RAW_DMA_ADR].L = (U32)(CorrectRSDataStart + (ubStartFrameNo * 4096));
		}
	}
	else {
		gFREG[FCTLL_RAW_DMA_ADR].L = ((U32)(&gubflaBuffer[0]));
	}
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(1, ubFrameNum, 1, 1);
	gFREG[FCTLL_FPU_ENTRY].L  = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);



	// Fill the FPU COR setting
	gFREG[FCTLL_L4K_SPR_ADR].L = L4K_TABLE_OFFSET + (FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize;
	if (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btBufModeEn) {
		gFREG[FCTLL_DMA_CFG].L |= FLH_BUF_MODE;
	}
	if (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btL4kBADRContModeEn) {
		gFREG[FCTLL_ZIP_CFG].L |= (L4K_BADR_CONTINUOUS_MODE);
		gFREG[FCTLL_DMA_ADR_0].L = gMTQ->Depth[FQ->ubDepth].ulL4KBADR;
	}
	gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
	if (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btCmpEn) {
		gFREG[FCTLL_CMP_CFG].L |= LCA_FW_COMPARE_EN;
		if (ENABLE_DEBUG_BB_ITEM_CROSS_FRAME_ISSUE && gubUNCandLCAfailCondition) {
			gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
		}
	}
#if ENABLE_DEBUG_EMULATOR
	gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
#endif

	// FPU COR
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_IBF_CTL].L &= CLR_LDPC_COR_MAP;
	gFREG[FCTLL_OTHER_SET].L &= (~BCH_COR_EN);
	if (gubEnterSB && (gubEnterRS == 0) && (gubEnterReadFailRetryCondition != CRC32_ERR_MAP)) {
		FCONL[FCONL_CRC_EN] &= (~CRC_32_EN); // SB trapping set must off crc32
	}

	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_IBF_CORRECT;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);


	while (gubINTTable[ubDepth].All == 0); // use this to check
	if (gubINTTable[ubDepth].All & 0xF8) {

		if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE && gubRSNormalModeCorrectDataBefore) {
			if (ENABLE_WP_DEBUG) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(1);
			}
		}

		if (ENABLE_DEBUG_UART_NES_RS && ENABLE_DEBUG_NES_READ_FAIL) {
			UartString(" debug_UNC2");
		}
		if (ENABLE_DEBUG_NES_READ_FAIL && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE)) {
			flaLEDBlink(1);
		}

		if (ENABLE_DEBUG_RS_VERIFY_DATA && gubRSDebug_NoGoldenToVerifyDATA && (!ENABLE_DEBUG_RS_NORMAL_MODE_FAIL_BLOCK)) {
			mDMAC_COPY( CorrectRSDataStart, (U32)&gubZ2Buffer[0], 16384, 0, 0, 0, 0, 1);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)((U32)&gubZ2Buffer[0]), 16384);
			}
			UartString("\nDC: ");
			_UCV ULLONG *pData = ((_UCV ULLONG *)((U32)&gubZ2Buffer[0]));
			UartLLongHex(pData[0]);

			mDMAC_COPY( VerifyDecodeRSDataStart, ((U32)&gubZ2Buffer[16384]), 16384, 0, 0, 0, 0, 1);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)((U32)&gubZ2Buffer[16384]), 16384);
			}
			UartString(" DG: ");
			pData = ((_UCV ULLONG *)((U32)&gubZ2Buffer[16384]));
			UartLLongHex(pData[0]);

			flaLEDBlink(5);
		}

		if (gubEnterRS && ((gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) || (gubEnterReadFailRetryCondition == CRC32_ERR_MAP) || (gubEnterReadFailRetryCondition == FW_SETTING_ERR_MAP))) {
			if (ENABLE_WP_DEBUG) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(1);
			}
		}

		if (!gubEnterRS) {
			if ((gFREG[FCTLL_INT_INF].L & CRC32_ERROR) == CRC32_ERROR) {
				VT->gsReadFailDebugCount.gulRetryCRC32InFPUCOR_LogCount++;
				if (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR) {
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(1);
					}
				}
			}
			else if ((gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) == LCA_FW_CMP_ERR_INF) {
				VT->gsReadFailDebugCount.gulRetryLCAErrorInFPUCOR_LogCount++;
				if (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR) {
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(1);
					}
				}
			}
			else if ((gFREG[FCTLL_INT_INF].L & FW_SETTING_ERR_INF) == FW_SETTING_ERR_INF) {
				VT->gsReadFailDebugCount.gulRetryFWSettingErrorInFPUCOR_LogCount++;
				if (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR) {
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(1);
					}
				}
			}
		}

		if (gFREG[FCTLL_INT_INF].L & (CRC32_ERROR | LCA_FW_CMP_ERR_INF | FW_SETTING_ERR_INF | DECOMPRESSION_FAIL_INF | CRC16_ERROR)) {
			ubPass = 0;
			if (gubEnterRS) {
				VT->gulRSRecoverFailCnt++;
			}
			else {
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(0xF3);
				}
				else {
					gubWP_DEBUG = 0xF3;
					if (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR) {
						M_AssertError(1);
					}
				}
			}
			// Restore setting for SB / RS
			gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
			gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
			gFREG[FCTLL_ZIP_CFG].L &= (~L4K_BADR_CONTINUOUS_MODE);
			gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);
			gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);

			// Refill L4K Table for HB read
			UBYTE ubL4KIndex = 0;
			UBYTE ubL4KNUM = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum;
			volatile L4KTable16B *pL4KTable;
			for (ubL4KIndex = 0; ubL4KIndex < ubL4KNUM; ubL4KIndex++) {
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KIndex * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubflaBuffer[(ubL4KIndex * 4096)])) / 512;
				pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			}

			if (gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) {
				flaReadFailRetryReFillLCA(ubgFQLinkIndex);
			}
		}
		else if (gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) {
			// 留到CHECK_DONE後進flaReadSignoffmaskFail處理
			gubTemp = 0;

			gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
			gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(SIGNOFFMASK_MAP);
			UBYTE ubFailL4KIndex = 0;
			U32 ulSignoffMaskFailFrameMapInMT = gFREG[FCTLL_MAP_INF].L;
			M_Get_Error_4K_Index(ubFailL4KIndex, (gubMaxZipRate * gub4kEntrysPerPlane), ulSignoffMaskFailFrameMapInMT); // max: 16個L4K

			UBYTE ubCheckRealSignoffMaskFail = 0;
			volatile L4KTable16B * pL4KTablePtr;
			U32 ulAddr = 0;
			UBYTE ubL4KIndex = 0;
			U32 uli = 0;
			for (ubL4KIndex = ubFailL4KIndex; ubL4KIndex < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubL4KIndex++) {

				pL4KTablePtr = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KIndex) * Spare4kSize));
				ulAddr = (pL4KTablePtr->Para0x0C.BitMap.ulL4K_BADR * 0x200);

				for (uli = 0; uli < 8; uli++) {
					if (pL4KTablePtr->Para0x08.BitMap.ubL4K_BUF_MASK & (BIT0 << uli)) {
						if (gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) {
							break;
						}
						else if ((pL4KTablePtr->Para0x04.BitMap.ubL4K_SPRV & (BIT0 << uli)) == 0) {
							break;
						}
					}
				}

				if (uli == 8) {
					continue;
				}

				if (ENABLE_DEBUG_UART_FAIL) {
					UartString("\nSignoffFailID ");
					Uart_Tx_DataHex(ubgFQLinkIndex);
					UartString(" LCA ");
					UartLongHex(pL4KTablePtr->ulL4K_LCA);
					UartString(" ProgramFrom ");
					Uart_Tx_DataHex(pL4KTablePtr->Para0x08.BitMap.ubL4K_FW2);
				}

				VT->gulReadRetryEnterRSSignOffFail++;
				VT->gulSignOffFail++;

				ubCheckRealSignoffMaskFail = 1;

				if (FQ->FQPara.B.btUserData) {
					U32 ulFailLBA;
					volatile L4KTable16B * pL4KTablePtr2 = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubFailL4KIndex) * Spare4kSize));
					ulFailLBA =  pL4KTablePtr2->ulL4K_LCA + uli;
					ulAddr += uli * 0x200;
					VT->gulReadRetryEnterRSUserDataSignOffFail++;
					flaReadFailRetrySetUNCAddr(ulAddr, ulFailLBA);
				}
				else {
					// BG Read FQ will read data to write buffer
					if (VT->gulFTLState.B.btBG_Copy && gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btBufModeEn) {
						ulAddr = (pL4KTablePtr->Para0x0C.BitMap.ulL4K_BADR - BUFFER2_WRITE_START_INDEX) >> SectorsPer4KLog; // write buffer 4k index
						gulBG_ReadFail4k_BitMap |= BIT0 << (ulAddr & 31);
						if (ENABLE_HANDLE_BG_READ_FAIL == 0) {
							gubBG_Read_UNC = 1;
						}
					}
					FQ->btSpareInvalid = 1;
				}
			}

			if (!ubCheckRealSignoffMaskFail) {
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
				FQ->btSpareInvalid = 0;
			}
		}
		else {
			if (ENABLE_WP_DEBUG) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(1);
			}
		}
		//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET);
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	}
	else if (gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) {
		VT->gulLCA_Retry_PassDebugCount++;
	}

	gubINTTable[ubDepth].All = 0;
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling

Mark_NoSignoffMaskFail:
	gFREG[FCTLL_IBF_CTL].L |= LDPC_COR_MAP(0x1F);
	gFREG[FCTLL_OTHER_SET].L |= (BCH_COR_EN);
	if (gubEnterSB && (gubEnterRS == 0) && (gubEnterReadFailRetryCondition != CRC32_ERR_MAP)) {
		if (ubBackupCRC32Enable)
			FCONL[FCONL_CRC_EN] |= (CRC_32_EN);
	}

	if (!ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
		if (ubChannel == 0) {
			FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH0_EN);
		}
		else {
			FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH1_EN);
		}
	}
	return ubPass;
#endif
}


// FOR ROW DATA RETRY USING BYPASS

void flaModifyHBRetryQueue(  UBYTE ubRetryBlockType , UBYTE  ubRetryQindex)
{
	UBYTE ubRetryQindex_temp;
	UBYTE ubRetryQindex_Cur_Pre;
	UBYTE ubRetryQindex_Cur_Next;
	if (ubRetryQindex != gHBRetryQI[ubRetryBlockType].ubLinkFirst) {
		if (ubRetryQindex == gHBRetryQI[ubRetryBlockType].ubLinkLast) {
			gHBRetryQI[ubRetryBlockType].ubLinkLast =  gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubPrevious;
		}

		ubRetryQindex_temp = gHBRetryQI[ubRetryBlockType].ubLinkFirst;

		ubRetryQindex_Cur_Pre = gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubPrevious;
		gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex_Cur_Pre].ubNext = gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubNext;
		if ( gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubNext != 0xFF ) {
			ubRetryQindex_Cur_Next = gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubNext;
			gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex_Cur_Next].ubPrevious = gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubPrevious;
		}

		gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubPrevious =  gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex_temp].ubPrevious;
		gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubNext = ubRetryQindex_temp;

		gHBRetryQI[ubRetryBlockType].ubLinkFirst = ubRetryQindex;

		gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex_temp].ubPrevious =  ubRetryQindex;

	}

}

void flaModifyHBRetryQueue_Last(  UBYTE ubRetryBlockType , UBYTE  ubRetryQindex )
{
	UBYTE ubRetryQindex_temp;
	UBYTE ubRetryQindex_Cur_Pre;
	UBYTE ubRetryQindex_Cur_Next;

	if (ubRetryQindex != gHBRetryQI[ubRetryBlockType].ubLinkLast) {

		if (ubRetryQindex == gHBRetryQI[ubRetryBlockType].ubLinkFirst) {
			gHBRetryQI[ubRetryBlockType].ubLinkFirst =  gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubNext;
		}
		ubRetryQindex_temp = gHBRetryQI[ubRetryBlockType].ubLinkLast;

		ubRetryQindex_Cur_Pre = gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubNext;
		gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex_Cur_Pre].ubPrevious = gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubPrevious;

		if ( gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubPrevious != 0xFF ) {
			ubRetryQindex_Cur_Next = gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubPrevious;
			gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex_Cur_Next].ubNext = gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubNext;
		}

		gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubNext =  gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex_temp].ubNext;
		gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex].ubPrevious = ubRetryQindex_temp;

		gHBRetryQI[ubRetryBlockType].ubLinkLast = ubRetryQindex;

		gHBRetryQI[ubRetryBlockType].HBRetryQueue[ubRetryQindex_temp].ubNext =  ubRetryQindex;

	}
}


#if TLC_NES_TEST_SB_FLOW
void NESGetIBFData(UBYTE ubIBFPtr, UBYTE ubDepth)
{
	// Get IBF
	gFREG[FCTLL_RAW_DMA_ADR].L = ((U32)(&gubZ2Buffer_NCS[0]));
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_BACK_STORE].W.W0 = HB_RETRY_L4K_TABLE_OFFSET;
	IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(ubIBFPtr, 1, 0, 1); //一次一個frame
	gFREG[FCTLL_FPU_ENTRY].L  = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);
	memcpy((void *)&gubZ2Buffer_NCS[0x1000], (void *)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET), 0x10);
	xthal_dcache_region_writeback_inv((void *)((U32)&gubZ2Buffer_NCS[0]), 8192);
}

#if TLC_NES_TEST_SB_FLOW
#define CE_DISABLE 0
#else
#define CE_DISABLE 1
#endif
void NESSendC9Data(UBYTE ubNES_C9_Address, UBYTE mode, UWORD num_byte)
{
	UWORD uwi;
	U32 ulTemp_FCONL_FCE_ENB;

	if (CE_DISABLE) {
		ulTemp_FCONL_FCE_ENB = FCONL[FCONL_FCE_ENB];
		FCONL[FCONL_FCE_ENB] = 0x00000000;
	}

	if (mode) {
		xthal_dcache_region_invalidate((void *)((U32)&gubZ2Buffer_NCS[0]), 8192);
	}
	gFREG[FCTLL_PIO_CMD].L = 0xC9;
	gFREG[FCTLL_PIO_CMD].L = 0x80;
	gFREG[FCTLL_PIO_ADR].L = 0;
	gFREG[FCTLL_PIO_ADR].L = 0;
	gFREG[FCTLL_PIO_ADR].L = ubNES_C9_Address;
	gFREG[FCTLL_PIO_ADR].L = num_byte;
	gFREG[FCTLL_PIO_ADR].L = (num_byte >> 8);

	gFREG[FCTLL_HS_MODE].L |= PIO_DAT_FIRST | PIO_DAT_LAST;

	gFREG[FCTLL_HS_MODE].L |= PIO_DAT_FIRST;
	gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First

	if ( gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE ) {
		for (uwi = 0 ; uwi < num_byte ; uwi += 2) {
			if (uwi == (num_byte - 2)) {
				gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;    //Last
			}

			if (mode == 0) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)((gubNES_C9_Data[uwi + 1] << 8) | (gubNES_C9_Data[uwi]));
			}
			else if (mode == 1) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)((gubZ2Buffer_NCS[uwi + 1] << 8) | (gubZ2Buffer_NCS[uwi]));
			}

			if (uwi == 0) {
				gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST));
			}
		}
	}
	else {
		for (uwi = 0 ; uwi < num_byte ; uwi++) {
			if (uwi == (num_byte - 1)) {
				gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;    //Last
			}

			if (mode == 0) {
				gFREG[FCTLL_PIO_DAT].B.B0 = gubNES_C9_Data[uwi];
			}
			else if (mode == 1) {
				gFREG[FCTLL_PIO_DAT].B.B0 = gubZ2Buffer_NCS[uwi];
			}

			if (uwi == 0) {
				gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST));
			}
		}
	}

	gFREG[FCTLL_HS_MODE].L |= PIO_DAT_FIRST | PIO_DAT_LAST;
	gFREG[FCTLL_PIO_CMD].L = 0x10;

#if (TLC_NES_TEST_SB_FLOW)
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;           //Send 0x70 to check status
	while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);
#endif
	if (CE_DISABLE) {
		FCONL[FCONL_FCE_ENB] = ulTemp_FCONL_FCE_ENB;
	}
}

void NESSendC9Data_CMD10(void)
{
	// get Error bit + iteration
	gubNES_C9_Data[0] = gFREG[FCTLL_ECC_INFO].L;
	gubNES_C9_Data[1] = ((gFREG[FCTLL_ECC_INFO].L) >> 8);
	gFREG[FCTLL_DBG_INF].L = 90;
	gubNES_C9_Data[2] = ((gFREG[FCTLL_DBG_INF].L) & 0x001FC000) >> 14;
	gubNES_C9_Data[3] = 0;
	if (FCONL[FCONL_LDPC_BFC_SET] & LDPC_COR_CONVERGE) {
		gubNES_C9_Data[4] = 1;
	}
	else {
		gubNES_C9_Data[4] = 0;
	}
	gubNES_C9_Data[5] = 0;
	//gubNES_C9_Address = 0x10; //R0:Error bit + iteration info
	NESSendC9Data(0x10, 0, 6);

}
void NESSendC9Data_CMD20(void)
{
	gubNES_C9_Data[0] = (gubCenterRank[0]);
	gubNES_C9_Data[1] = (gubCenterRank[1]);
	gubNES_C9_Data[2] = 0x00;
	gubNES_C9_Data[3] = 0x00;
	gubNES_C9_Data[4] = FCONW[FCONW_DSP_SB_DISTRI_00];
	gubNES_C9_Data[5] = FCONW[FCONW_DSP_SB_DISTRI_00] >> 8;
	gubNES_C9_Data[6] = FCONW[FCONW_DSP_SB_DISTRI_01];
	gubNES_C9_Data[7] = FCONW[FCONW_DSP_SB_DISTRI_01] >> 8;
	gubNES_C9_Data[8] = FCONW[FCONW_DSP_SB_DISTRI_02];
	gubNES_C9_Data[9] = FCONW[FCONW_DSP_SB_DISTRI_02] >> 8;
	gubNES_C9_Data[10] = FCONW[FCONW_DSP_SB_DISTRI_03];
	gubNES_C9_Data[11] = FCONW[FCONW_DSP_SB_DISTRI_03] >> 8;
	gubNES_C9_Data[12] = FCONW[FCONW_DSP_SB_DISTRI_04];
	gubNES_C9_Data[13] = FCONW[FCONW_DSP_SB_DISTRI_04] >> 8;
	gubNES_C9_Data[14] = FCONW[FCONW_DSP_SB_DISTRI_05];
	gubNES_C9_Data[15] = FCONW[FCONW_DSP_SB_DISTRI_05] >> 8;
	gubNES_C9_Data[16] = FCONW[FCONW_DSP_SB_DISTRI_06];
	gubNES_C9_Data[17] = FCONW[FCONW_DSP_SB_DISTRI_06] >> 8;
	gubNES_C9_Data[18] = FCONW[FCONW_DSP_SB_DISTRI_07];
	gubNES_C9_Data[19] = FCONW[FCONW_DSP_SB_DISTRI_07] >> 8;
	gubNES_C9_Data[20] = FCONW[FCONW_DSP_SB_DISTRI_08];
	gubNES_C9_Data[21] = FCONW[FCONW_DSP_SB_DISTRI_08] >> 8;
	gubNES_C9_Data[22] = FCONW[FCONW_DSP_SB_DISTRI_09];
	gubNES_C9_Data[23] = FCONW[FCONW_DSP_SB_DISTRI_09] >> 8;
	gubNES_C9_Data[24] = FCONW[FCONW_DSP_SB_DISTRI_10];
	gubNES_C9_Data[25] = FCONW[FCONW_DSP_SB_DISTRI_10] >> 8;
	gubNES_C9_Data[26] = FCONW[FCONW_DSP_SB_DISTRI_11];
	gubNES_C9_Data[27] = FCONW[FCONW_DSP_SB_DISTRI_11] >> 8;
	gubNES_C9_Data[28] = FCONW[FCONW_DSP_SB_DISTRI_12];
	gubNES_C9_Data[29] = FCONW[FCONW_DSP_SB_DISTRI_12] >> 8;
	gubNES_C9_Data[30] = FCONW[FCONW_DSP_SB_DISTRI_13];
	gubNES_C9_Data[31] = FCONW[FCONW_DSP_SB_DISTRI_13] >> 8;
	gubNES_C9_Data[32] = FCONW[FCONW_DSP_SB_DISTRI_14];
	gubNES_C9_Data[33] = FCONW[FCONW_DSP_SB_DISTRI_14] >> 8;
	gubNES_C9_Data[34] = FCONW[FCONW_DSP_SB_DISTRI_15];
	gubNES_C9_Data[35] = FCONW[FCONW_DSP_SB_DISTRI_15] >> 8;
	gubNES_C9_Data[36] = FCONW[FCONW_DSP_SB_DISTRI_16];
	gubNES_C9_Data[37] = FCONW[FCONW_DSP_SB_DISTRI_16] >> 8;
	gubNES_C9_Data[38] = FCONW[FCONW_DSP_SB_DISTRI_17];
	gubNES_C9_Data[39] = FCONW[FCONW_DSP_SB_DISTRI_17] >> 8;
	gubNES_C9_Data[40] = FCONW[FCONW_DSP_SB_DISTRI_18];
	gubNES_C9_Data[41] = FCONW[FCONW_DSP_SB_DISTRI_18] >> 8;
	gubNES_C9_Data[42] = FCONW[FCONW_DSP_SB_DISTRI_19];
	gubNES_C9_Data[43] = FCONW[FCONW_DSP_SB_DISTRI_19] >> 8;
	gubNES_C9_Data[44] = FCONW[FCONW_DSP_SB_DISTRI_20];
	gubNES_C9_Data[45] = FCONW[FCONW_DSP_SB_DISTRI_20] >> 8;
	gubNES_C9_Data[46] = FCONW[FCONW_DSP_SB_DISTRI_21];
	gubNES_C9_Data[47] = FCONW[FCONW_DSP_SB_DISTRI_21] >> 8;
	gubNES_C9_Data[48] = FCONW[FCONW_DSP_SB_DISTRI_22];
	gubNES_C9_Data[49] = FCONW[FCONW_DSP_SB_DISTRI_22] >> 8;
	gubNES_C9_Data[50] = FCONW[FCONW_DSP_SB_DISTRI_23];
	gubNES_C9_Data[51] = FCONW[FCONW_DSP_SB_DISTRI_23] >> 8;
	gubNES_C9_Data[52] = FCONW[FCONW_DSP_SB_DISTRI_24];
	gubNES_C9_Data[53] = FCONW[FCONW_DSP_SB_DISTRI_24] >> 8;
	gubNES_C9_Data[54] = FCONW[FCONW_DSP_SB_DISTRI_25];
	gubNES_C9_Data[55] = FCONW[FCONW_DSP_SB_DISTRI_25] >> 8;
	gubNES_C9_Data[56] = FCONW[FCONW_DSP_SB_DISTRI_26];
	gubNES_C9_Data[57] = FCONW[FCONW_DSP_SB_DISTRI_26] >> 8;
	gubNES_C9_Data[58] = FCONW[FCONW_DSP_SB_DISTRI_27];
	gubNES_C9_Data[59] = FCONW[FCONW_DSP_SB_DISTRI_27] >> 8;
	gubNES_C9_Data[60] = FCONW[FCONW_DSP_SB_DISTRI_28];
	gubNES_C9_Data[61] = FCONW[FCONW_DSP_SB_DISTRI_28] >> 8;
	gubNES_C9_Data[62] = FCONW[FCONW_DSP_SB_DISTRI_29];
	gubNES_C9_Data[63] = FCONW[FCONW_DSP_SB_DISTRI_29] >> 8;
	gubNES_C9_Data[64] = FCONW[FCONW_DSP_SB_DISTRI_30];
	gubNES_C9_Data[65] = FCONW[FCONW_DSP_SB_DISTRI_30] >> 8;
	gubNES_C9_Data[66] = FCONW[FCONW_DSP_SB_DISTRI_31];
	gubNES_C9_Data[67] = FCONW[FCONW_DSP_SB_DISTRI_31] >> 8;
	NESSendC9Data(0x20, 0, 68);

}

#endif


UBYTE flaReadFailHBRetry_SB(UBYTE ubRetry4K_IDX , UBYTE ubStartFrame , UBYTE ubDepth , UBYTE ubPageSel , UBYTE ubCEIndex )
{

	UBYTE ubHBRetryOK = 0;
	//gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
	//gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(UNC_MAP);

	//UartString("\nSB_HB:");

	gFREG[FCTLL_L4K_SPR_ADR].L = (HB_RETRY_L4K_TABLE_OFFSET + ( (ubRetry4K_IDX  - ubStartFrame) * Spare4kSize ));

	flaReadFailRetrySendResetCommand(ubDepth);


	SoftBitCorrectToshibaFlow(SOFTBIT_READ_HB_DATA, ubPageSel, 1);  // precondtion 做 HBSB 之前要下5CC5， 但upper page 只會在 LI page 前下  所以多用一個變數

	flaReadFailRetryTriggerFPU(0x00);

	//UartString("\nAddress:");
	//Uart_Tx_DataHex(ubPageSel );
	//UartLongHex(gFREG[FCTLL_FARG_COL].L);
	//UartLongHex(gFREG[FCTLL_FARG_ROW].L);

	if (!(flaReadFailRetryCheckReadStatusInFail(ubDepth, 0))) {
		ubHBRetryOK = 1;
		//if (!BURNER) {
		//	VT->gsReadFailDebugCount.gulRetryPassCount[(ubRetryCount + 1)]++;
		//}

		//gulReadFail_4KMAP_HR &= (~(1 << (gubStartFrameNo_HR + ubRetry4K_IDX)));
		//break;
	}
#if TLC_NES_TEST_SB_FLOW
#if MicronFlashOnly || YMTC_JGS
	if ((gubNESReadIBF == 0x01) && gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
	if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
	{
		NESGetIBFData(0, ubDepth);
		//gubNES_C9_Address = ubRetry4K_IDX; //R0:HB data
		NESSendC9Data(ubRetry4K_IDX, 1, guwNESDataLen); // mode1 => Z2 buffer
		NESSendC9Data_CMD10();
	}
#endif


	if (guoFlashUseType.B.btToshiba) {
		if (guoFlashUseType.B.btSandisk) {
			flaReadFailRetrySandiskTerminateFlow(0);
		}
		else {
			flaReadFailRetryToshibaTerminateFlow(0);
		}

		flaReadFailRetrySendResetCommand(ubDepth);
	}

	//UartString("\nHB R:");
	//Uart_Tx_DataHex( ubHBRetryOK);

	return ubHBRetryOK;
}


UBYTE flaReadFailHBRetry(U32 ulIRAM_L4K_TABLE_OFFSET, UBYTE ubStartFrameNo, UBYTE ubFrameNum, UBYTE ubIsFastPage, UBYTE ubDepth, UBYTE ubChannel, UBYTE ubALUIndex)
{
	//	M_CheckRDTModeToDisableFunctionReturnValid(0);
	UBYTE ubRetryCount = 0, ubHBRetryOK = 0;
	UBYTE ubRetry4K_IDX = 0;
	U32 ulReadFail_4KMAP = 0;
	UWORD uwFJobInfo = 0;
	FSA_t * pFSA = (FSA_t *)gulFSABase[ubChannel];
	UBYTE  ubRetryQindex;
	UBYTE ubPageSel = 0;
	UWORD uwPageIndex = (M_Divider(gulFQEntry_HR, DEF_4KEntryPerPage_Mcl_Div));

	gubEnterHB = 1;
	gubIsHBRetryLastNormalRead = 0;

	if (!gubEnterRS) {
		// NOTE: can't use (FORCE_ZIP_COR);
		if (ENABLE_WP_DEBUG && (gFREG[FCTLL_IBF_CTL].L & FORCE_ZIP_COR)) {
			flaLEDBlink(1);
		}
		else {
			M_AssertError(gFREG[FCTLL_IBF_CTL].L & FORCE_ZIP_COR);
		}
	}

	if (ubIsFastPage) {
		uwFJobInfo |= (BIT_FJOBI_FASTPAGE);
	}

	while (ubRetry4K_IDX < ubFrameNum) {
		ulReadFail_4KMAP |= (1 << (ubStartFrameNo + ubRetry4K_IDX)); // init fail 4K map
		ubRetry4K_IDX++;
	}

	gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
	gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(UNC_MAP);

	// 多檢查CRC32 map
	// LCA error, ZIP error.. 要到FPUCOR才會出現
	U32 ulUNC_AND_CRC32_ERRMAP_INF = gFREG[FCTLL_MAP_INF].L;
	gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
	gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(CRC32_ERR_MAP);
	ulUNC_AND_CRC32_ERRMAP_INF |= gFREG[FCTLL_MAP_INF].L;

	for (ubRetry4K_IDX = 0; ubRetry4K_IDX < ubFrameNum; ubRetry4K_IDX++) {
		if (ulUNC_AND_CRC32_ERRMAP_INF & (1 << (ubStartFrameNo + ubRetry4K_IDX))) {
			break;
		}
		else {
			ulReadFail_4KMAP &= (~(1 << (ubStartFrameNo + ubRetry4K_IDX)));
		}
	}

	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(1);
	gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME);
	gFREG[FCTLL_DMA_CFG].L |= (((UWORD)1) << 8);


	if (!ENABLE_DEBUG_NO_HB) {

		while (ubRetry4K_IDX < ubFrameNum) { // once read a 4K frame
#if Read_Chk_ByReadScan
			gubRetryFrameNum_RD = ubRetry4K_IDX;
#endif
			gFREG[FCTLL_ZIP_MAP].L = (0xFF << ((ubStartFrameNo + ubRetry4K_IDX) * 8));

			if (!ENABLE_iFSA) {
				pFSA[gubFSAIndex[ubChannel]].uword.W0 &= (~gub4kEntrysPerPlaneMask);
				pFSA[gubFSAIndex[ubChannel]].uword.W0 |= ((ubStartFrameNo + ubRetry4K_IDX) & gub4kEntrysPerPlaneMask);
			}
			else {
				guliFSA[0] &= (~gub4kEntrysPerPlaneMask);
				guliFSA[0] |= ((ubStartFrameNo + ubRetry4K_IDX) & gub4kEntrysPerPlaneMask);
				gFREG[FCTLL_IFSA0].L = guliFSA[0];
			}

			gFREG[FCTLL_L4K_SPR_ADR].L = (ulIRAM_L4K_TABLE_OFFSET + (ubRetry4K_IDX * Spare4kSize));

			if ((ENABLE_RS_NORMAL_MODE || (ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN && gubRS_FPU_ReadGetLDPC_BCH)) && gubEnterRS) { // refill L4K table due to enable FORCE_ZIP_COR during RS
				volatile L4KTable16B *pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubRetry4K_IDX * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0x00000000;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (gulRSDMAAddr + ubRetry4K_IDX * 4096) / 512;
				pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			}

			ubHBRetryOK = 0;

			if (!ENABLE_DEBUG_RETRY_SEQUENCE) {

				flaReadFailRetrySendResetCommand(ubDepth);

				if (guoFlashUseType.B.btToshiba1Znm && (!guoFlashUseType.B.btSandisk) && ((!guoFlashUseType.B.btA2Cmd) || ENABLE_DEBUG_FORCE_FASTPAGE_NONA2  || ((ubIsFastPage == 0) /* && (!ENABLE_ULTRAMLC)*/))) {

					flaReadFailRetryToshiba1ZnmSpecialFlow();

					flaReadFailRetryTriggerFPU(ubALUIndex);

					if (!(flaReadFailRetryCheckReadStatusInFail(ubDepth, ubALUIndex))) {
						ubHBRetryOK = 1;
						VT->gul1ZnmSpecialCaseRetryCnt++;
#if ENABLE_RECORD_RETRY_INFO
						if (!BURNER) {
							VT->gsReadFailDebugCount.gulRetryPassCount[11]++;
						}
#endif
						ulReadFail_4KMAP &= (~(1 << (ubStartFrameNo + ubRetry4K_IDX)));
					}

					flaReadFailRetryToshiba1ZnmSpecialTerminateFlow();

					flaReadFailRetrySendResetCommand(ubDepth);
				}
			Sandisk_Retry_Point_MLC:
				if (!ubHBRetryOK) {

					UBYTE ubRetryBlockType = 0;
					if ((guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0))  && (ubIsFastPage/* || ENABLE_ULTRAMLC*/)) {
						ubRetryBlockType = RETRY_D1_BLOCK_TYPE;
					}
					else {
						ubRetryBlockType = RETRY_D2D3_BLOCK_TYPE;
						if ((uwPageIndex % 2) == 0) {
							if (uwPageIndex != 0) {
								ubPageSel = UPPER_PAGE;
							}
							else {
								ubPageSel = LOWER_PAGE;
							}
						}
						else {
							if (uwPageIndex != guwPagesPerUnitMask) {
								ubPageSel = LOWER_PAGE;
							}
							else {
								ubPageSel = UPPER_PAGE;
							}
						}
					}

					// Check flash interface if mismatch
					if ((guoFlashUseType.B.btONFI && (gFREG[FCTLL_FLH_SET].B.B0 != FLA_ONFI_MODE))
					        || ((guoFlashUseType.B.btToggle || gubToggleMode) && (gFREG[FCTLL_FLH_SET].B.B0 != FLA_TOGGLE_MODE))) {
						if (ENABLE_WP_DEBUG) {
							flaLEDBlink(0xF1);
						}
						else {
							gubWP_DEBUG = 0xF1;
							M_AssertError(1);
						}
					}
#if Read_Chk_ByReadScan
					gubRetryOK_RD = 0;
					gubRetryAgain_RD = 0;
				Mark_ReadDisturbRetryAgain:
#endif
					for (ubRetryCount = 0; ubRetryCount < gubHardBitRetryGroupNumber[ubRetryBlockType]; ubRetryCount++) {

#if Read_Chk_ByReadScan
						gubRetryCount_RD = ubRetryCount;
						gubRetryLimit_RD = gubHardBitRetryGroupNumber[ubRetryBlockType] - 1;
#endif
						if ((ENABLE_RS_NORMAL_MODE || (ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN && gubRS_FPU_ReadGetLDPC_BCH)) && gubEnterRS) { // refill L4K table due to enable FORCE_ZIP_COR during RS
							volatile L4KTable16B *pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubRetry4K_IDX * Spare4kSize));
							//set L4K table info
							pL4KTable->ulL4K_LCA = 0x00000000;
							pL4KTable->Para0x04.ulAll = 0;
							pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
							pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
							pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (gulRSDMAAddr + ubRetry4K_IDX * 4096) / 512;
							pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
							pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
						}

						if (ENABLE_DEBUG_HB_RETRY_ALL) {
							if (ubRetryCount == gubDebugHBRetryForcePassPhase) {
								gFREG[FCTLL_UNC_CFG].L = 0;
							}
						}

						if (guoFlashUseType.B.btToshiba) {
							UBYTE  ubRetryGroup;
							if ( ubRetryBlockType == RETRY_D2D3_BLOCK_TYPE ) {
								if ( ubRetryCount == 0x00 ) {
									ubRetryQindex = gHBRetryQI[ubPageSel - 1].ubLinkFirst;
								}
								else {
									ubRetryQindex = gHBRetryQI[ubPageSel - 1].HBRetryQueue[ubRetryQindex].ubNext;
								}
								ubRetryGroup = gHBRetryQI[ubPageSel - 1].HBRetryQueue[ubRetryQindex].ubRetryIndex;
								if (guoFlashUseType.B.btSandisk) {
									flaReadFailRetrySandiskFlow(ubRetryGroup, uwFJobInfo, NULL);
								}
								else {
									gubD3RetryCount_HR = ubRetryCount;
									flaReadFailRetryToshibaFlow(ubRetryGroup, uwFJobInfo);
								}
							}
							else {
								if ( ubRetryCount == 0x00 ) {
									ubRetryQindex = gHBRetryQI[3].ubLinkFirst;
								}
								else {
									ubRetryQindex = gHBRetryQI[3].HBRetryQueue[ubRetryQindex].ubNext;
								}
								ubRetryGroup = gHBRetryQI[3].HBRetryQueue[ubRetryQindex].ubRetryIndex;
								if (guoFlashUseType.B.btSandisk) {
									flaReadFailRetrySandiskFlow(ubRetryGroup, uwFJobInfo, NULL);
								}
								else {
									gubD3RetryCount_HR = ubRetryCount;
									flaReadFailRetryToshibaFlow(ubRetryGroup, uwFJobInfo);
								}
							}

						}
						else {
							if (ENABLE_WP_DEBUG) {
								flaLEDBlink(0xFD);
							}
							else {
								gubWP_DEBUG = 0xFD;
								M_AssertError(1);
							}
						}

						flaReadFailRetryTriggerFPU(ubALUIndex);

						if (!(flaReadFailRetryCheckReadStatusInFail(ubDepth, ubALUIndex))) {

#if TLC_NES_TEST_SB_FLOW
							if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
							{
								NESGetIBFData(0, ubDepth);
								NESSendC9Data(ubRetry4K_IDX, 1, guwNESDataLen); //R0:HB data
								NESSendC9Data_CMD10();
							}
#endif


#if Read_Chk_ByReadScan
							if ( (gubRetryOK_RD == 0x5A ) && (gubRetryAgain_RD == 1)  ) {
								gubRetryAgain_RD = 0;
								goto Mark_ReadDisturbRetryAgain;
							}
#endif
							ubHBRetryOK = 1;
#if ENABLE_RECORD_RETRY_INFO
							if (!BURNER) {
								VT->gsReadFailDebugCount.gulRetryPassCount[(ubRetryCount + 1)]++;
							}
#endif
							if (guoFlashUseType.B.btToshiba) {
								if ( ubRetryBlockType == RETRY_D2D3_BLOCK_TYPE ) {
									flaModifyHBRetryQueue( ubPageSel - 1 ,  ubRetryQindex  );
								}
								else {
									flaModifyHBRetryQueue( 3 ,  ubRetryQindex  );//SLC Retry Modify Queue
								}
							}
							ulReadFail_4KMAP &= (~(1 << (ubStartFrameNo + ubRetry4K_IDX)));
							break;
						}

						else {

#if TLC_NES_TEST_SB_FLOW
							if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
							{
								NESGetIBFData(0, ubDepth);
								//R0:HB data
								NESSendC9Data(ubRetry4K_IDX, 1, guwNESDataLen); // mode1 => Z2 buffer
								NESSendC9Data_CMD10();
							}
#endif
						}
					}
					if (ubHBRetryOK == 0) {
						if (guoFlashUseType.B.btSandisk && ((gubSandiskRetryType != 1) && (gubSandiskRetryType != 5) && (gubSandiskRetryType != 9) && (gubSandiskRetryType != 13))) {//魔術數字請參照spec
							gubSandiskRetryType++;
							goto Sandisk_Retry_Point_MLC;
						}
					}

					if (guoFlashUseType.B.btToshiba) {
						if (guoFlashUseType.B.btSandisk) {
							flaReadFailRetrySandiskTerminateFlow(uwFJobInfo);
						}
						else {
							flaReadFailRetryToshibaTerminateFlow(uwFJobInfo);
						}

						flaReadFailRetrySendResetCommand(ubDepth);
					}
					else {
						if (ENABLE_WP_DEBUG) {
							flaLEDBlink(0xFD);
						}
						else {
							gubWP_DEBUG = 0xFD;
							M_AssertError(1);
						}
					}
				}
			}
			else {
				for (ubRetryCount = 0; ubRetryCount < 3; ubRetryCount++) {

					if ((ENABLE_RS_NORMAL_MODE || (ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN && gubRS_FPU_ReadGetLDPC_BCH)) && gubEnterRS) { // refill L4K table due to enable FORCE_ZIP_COR during RS
						volatile L4KTable16B *pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubRetry4K_IDX * Spare4kSize));
						//set L4K table info
						pL4KTable->ulL4K_LCA = 0x00000000;
						pL4KTable->Para0x04.ulAll = 0;
						pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
						pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
						pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (gulRSDMAAddr + ubRetry4K_IDX * 4096) / 512;
						pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
						pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
					}

					if (ENABLE_DEBUG_HB_RETRY_ALL) {
						gFREG[FCTLL_UNC_CFG].L = 0;
						if (getIntRandValue(100) >= 50) {
							if (getIntRandValue(100) >= 50) {
								gFREG[FCTLL_UNC_CFG].L |= (AUTOGEN_READ_FAIL_EN);
							}
							else {
								UBYTE ubLoop = 0;
								UBYTE ubLoopCount = 0;
								ubLoopCount = (getIntRandValue(4) + 1);
								while (ubLoop < ubLoopCount) {
									gFREG[FCTLL_UNC_CFG].L |= FORCE_BCH_UNCORRECTABLE_FRAME(1 << (getIntRandValue(4)));
									ubLoop++;
								}
							}
						}
					}

					flaReadFailRetryTriggerFPU(ubALUIndex);

					if (!(flaReadFailRetryCheckReadStatusInFail(ubDepth, ubALUIndex))) {
						ubHBRetryOK = 1;
#if ENABLE_RECORD_RETRY_INFO
						if (!BURNER) {
							VT->gsReadFailDebugCount.gulRetryPassCount[(ubRetryCount + 1)]++;
						}
#endif
						ulReadFail_4KMAP &= (~(1 << (ubStartFrameNo + ubRetry4K_IDX)));
						break;
					}
				}
			}


			if (0) {
			Mark_Normal_Read_Again:

				if ((ENABLE_RS_NORMAL_MODE || (ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN && gubRS_FPU_ReadGetLDPC_BCH)) && gubEnterRS) { // refill L4K table due to enable FORCE_ZIP_COR during RS
					volatile L4KTable16B *pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubRetry4K_IDX * Spare4kSize));
					//set L4K table info
					pL4KTable->ulL4K_LCA = 0x00000000;
					pL4KTable->Para0x04.ulAll = 0;
					pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
					pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
					pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (gulRSDMAAddr + ubRetry4K_IDX * 4096) / 512;
					pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
					pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				}

				gubIsHBRetryLastNormalRead = 1;

				if (ENABLE_DEBUG_HB_RETRY_ALL) {
					gFREG[FCTLL_UNC_CFG].L = 0;
				}

				if (ENABLE_HBRETRY_LAST_NORMAL_READ_SPEED_DOWN && (gubFlashClock != FLH_CLK_DEFAULT)) {	//Flash Clock尚未初始化前不降頻
					if (ASIC) {
						//FlaClkDiv(RW_Mode, Toggle_Asyn40, 6);
						//Fla_Set_Offset_DQS_Delay(0);
						if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
						{
							gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | gubClockDivValue);
							for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

							}
						}
					}
				}

				flaReadFailRetryTriggerFPU(ubALUIndex);

				if (ENABLE_HBRETRY_LAST_NORMAL_READ_SPEED_DOWN && (gubFlashClock != FLH_CLK_DEFAULT)) {	//Flash Clock尚未初始化前不降頻
					if (ASIC) {
						//FlaClkDiv(RW_Mode, Toggle_400, 0xF);
						//Fla_Set_Offset_DQS_Delay(Toggle_400);

						if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
						{
							gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
							for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

							}
							gFREG[FCTLL_FDIV_CFG].L = 0;
						}
					}
				}

				if (!(flaReadFailRetryCheckReadStatusInFail(ubDepth, ubALUIndex))) {
					if (!BURNER) {
						VT->gsReadFailDebugCount.gulHBRetry_NormalReadAgain_Pass_DebugCount++;
					}
					ulReadFail_4KMAP &= (~(1 << (ubStartFrameNo + ubRetry4K_IDX)));
				}
			}

			ubRetry4K_IDX++;
		}

	}

	if (gubEnterRS) {
		ubHBRetryOK = (UBYTE)(ulReadFail_4KMAP & 0xFF);
	}
	else {
		if (ulReadFail_4KMAP == 0) {
			ubHBRetryOK = 1;
			// not update gulReadFail_4KMAP
		}
		else {
			ubHBRetryOK = 0;
			gulReadFail_4KMAP = ulReadFail_4KMAP;
		}
	}

	return ubHBRetryOK;
}

void flaReadFailRetrySendResetCommand(UBYTE ubDepth)
{



	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;

	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x8_FPU_POL_SendResetCommand, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}

	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);
}

const U32 Polynomial = 0xEDB88320;
U32 crc32_bitwise_f(U32 init, const void * data, U32 length)
{
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);
	U32 crc = init;//0x28a6ce9a;
	UBYTE* current = (UBYTE*) data;
	U32 j;
	U32 ccc;

	ccc = 0;

	for (j = 0; j < 32; j++) {
		ccc |= (crc >> (31 - j) & 1) << j;
	}
	crc = ccc;

	while (length--) {
		crc ^= *current++;
		for (j = 0; j < 8; j++)
			crc = (crc >> 1) ^ (- (int)(crc & 1) & Polynomial);
	}

	ccc = 0;

	for (j = 0; j < 32; j++) {
		ccc |= (crc >> (31 - j) & 1) << j;
	}


	return ccc;
}

void flaReadFailRetryReGenP4KCRC32(UBYTE ubgFQLinkIndex, UBYTE ubDepth)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	UBYTE ubStartFrameNo = FQ->ulFEntry & gub4kEntrysPerPlaneMask;
	UBYTE ubFrameNum = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum;
	UBYTE ubProgramFrameNum = (ubStartFrameNo + ubFrameNum);


	if (ENABLE_iFSA) {
		gFREG[FCTLL_IFSA0].L = (gMTQ->Depth[FQ->ubDepth].Para0x2C.uliFSA0 & (~gub4kEntrysPerPlaneMask));
	}
	else {
		gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
		gFREG[FCTLL_FSA_SEL].L  = FQ->ubFSAIndex;
	}
	gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
	gFREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(FQ->ubALUIndex);

	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubProgramFrameNum);

	gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME) ;
	gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubProgramFrameNum) << 8);

	UBYTE ubL4KCount = 0;
	volatile L4KTable16B *pDummyProgramL4KTable;
	volatile L4KTable16B *pLCAL4KTable = (volatile L4KTable16B*)(HB_RETRY_L4K_TABLE_ADDR);
	gFREG[FCTLL_L4K_SPR_ADR].L = DUMMY_L4K_TABLE_OFFSET;
	// copy correct spare
	memcpy((void *)(IRAM_BASE + DUMMY_L4K_TABLE_OFFSET + (ubStartFrameNo * Spare4kSize)), (void *)(HB_RETRY_L4K_TABLE_ADDR), (ubFrameNum * Spare4kSize));
	for (ubL4KCount = 0; ubL4KCount < ubProgramFrameNum; ubL4KCount++) {
		pDummyProgramL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + DUMMY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
		//set L4K table info
		if (ubL4KCount < ubStartFrameNo) {
			pDummyProgramL4KTable->ulL4K_LCA = pLCAL4KTable->ulL4K_LCA;
		}
		pDummyProgramL4KTable->Para0x04.BitMap.ubL4K_SPRV = 0xFF;
		pDummyProgramL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
		pDummyProgramL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pDummyProgramL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubflaBuffer[(((ubL4KCount >= ubStartFrameNo) ? (ubL4KCount - ubStartFrameNo) : 0) * 4096)])) / 512;
		pDummyProgramL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = MAX_ZCODE;
	}
	FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;

Mark_DummyProgramDisableCE:
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C80_A5_DW;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x9_FPU_W_ReGenP4KCRC32, gFREG, CHECK_FPU_DMA_W_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}
	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);


	// Backup FPU
	gFREG[FCTLL_BACK_STORE].W.W0 = (DUMMY_L4K_TABLE_OFFSET);
	if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE) {
		gFREG[FCTLL_RAW_DMA_ADR].L = ((U32)RSFrameDataDecodeStart);
	}
	else {
		gFREG[FCTLL_RAW_DMA_ADR].L = ((U32)(&gubflaBuffer[16384]));
	}
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(ubStartFrameNo, ubFrameNum, 0, 1);
	gFREG[FCTLL_FPU_ENTRY].L  = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);


	// Re-Gen crc32
	U32 crc32;
	volatile L4KTable16B l4k_b = {0};
	volatile L4KTable16B *pCRC32L4KTable;
	volatile L4KTable16B *pRecoverL4KTable;
	for (ubL4KCount = 0; ubL4KCount < ubFrameNum; ubL4KCount++) {
		pCRC32L4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + DUMMY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
		pRecoverL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));

		l4k_b.Para0x04.BitMap.ubL4K_SPRV = pRecoverL4KTable->Para0x04.BitMap.ubL4K_SPRV;
		l4k_b.Para0x08.BitMap.ubL4K_BUF_MASK = pRecoverL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK;
		l4k_b.Para0x08.BitMap.uwNEXT_L4K_PTR = pRecoverL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR;

		crc32 = ((pCRC32L4KTable->Para0x0C.ulAll ^ crc32_bitwise_f(0x00000000, (const void*)&l4k_b, 12)) ^ 0x4710BB9C);

		pRecoverL4KTable->Para0x0C.ulAll = crc32;
	}

}

UBYTE flaReadFailRetryRSFailFPUCORagain(UBYTE ubgFQLinkIndex, UBYTE ubDepth)
{
	// Due to RS's sb trapping, do FPUCOR again
#if(!RDT && !BURNER)
	UBYTE ubPass = 0;
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	U32 ulTargetPlaneIndex = FQ->ulFEntry >> gub4kEntrysPerPlaneLog;
	U32 ulPhyPlaneIndex;
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	UBYTE ubStartFrameNo = FQ->ulFEntry & gub4kEntrysPerPlaneMask;
	UBYTE ubFrameNum = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum;

	VT->gulReadFailRetryRSFailFPUCORagain_DebugCnt++;

	if (ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
		// Reset IBF PTR by FW to solve disable IBF_RING_MODE_EN will cause another channel IBF PTR error bug
		flaReadFailRetryResetIBFPTR(ubChannel, ubDepth);
	}
	else {
		if (ubChannel == 0) {
			FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH0_EN);
		}
		else {
			FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH1_EN);
		}
	}
	gFREG[FCTLL_IBF_CTL].L |= LDPC_COR_MAP(0x1F);
	gFREG[FCTLL_OTHER_SET].L |= (BCH_COR_EN);

	gFREG[FCTLL_IBF_CTL].L |= (FORCE_ZIP_COR);

	// Get LDPC & BCH
	U32 ulDMAAddr = RSFrameDataDecodeStart;

	if (ENABLE_DEBUG_RS_NORMAL_MODE && ENABLE_DEBUG_HW_FORCEREADFAIL) {
		FALLL[FCTLL_UNC_CFG] = 0;
	}

	UWORD uwRSPlaneIndex = (guwRSErrorPlaneIndex[0] & 0x00FF);
	if (ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR_ITERATION || ENABLE_DEBUG_UART_NES_RS) {
		if (ENABLE_DEBUG_RS_NORMAL_MODE_UART || ENABLE_DEBUG_UART_NES_RS) {
			UartString(" uli: ");
			UartWordHex(uwRSPlaneIndex);
		}
	}

	if (FQ->FQPara.B.btRWTableUnit) {
		ulPhyPlaneIndex = (M_Multiplier((M_Divider(ulTargetPlaneIndex, DEF_TableParityPlanes_Mcl_Div)), DEF_TableParityPlanes_Mcl_Div)) + uwRSPlaneIndex;
	}
	else {
		U32 ulListIndex;
		Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);

		// total encode count / uwPlanesPerGroup = list index
		ulListIndex = (RS_FLAFAIL.ubTagIndex * PARITY_DATA_PAGE_NUM + uwRSPlaneIndex) / RS.uwPlanesPerGroup;

		M_AssertError(ulListIndex >= RS_MAX_LINK_COUNT_PER_GROUP);
		M_AssertError(RS_FLAFAIL.ulPageLinkList[RS_FLAFAIL.ubGroupIndex][ulListIndex] == 0xFFFFFFFF);

		// get start plane index
		ulPhyPlaneIndex = RS_FLAFAIL.ulPageLinkList[RS_FLAFAIL.ubGroupIndex][ulListIndex] * RS.uwPlanesPerGroup;
		// adjust to real plane offset
		ulPhyPlaneIndex += ((RS_FLAFAIL.ubTagIndex * PARITY_DATA_PAGE_NUM + uwRSPlaneIndex) % RS.uwPlanesPerGroup);
	}

	gubRS_FPU_ReadGetLDPC_BCH = 1;
	RS_FPU_Read(ulDMAAddr, ubgFQLinkIndex, ulPhyPlaneIndex, uwRSPlaneIndex, DONTCARE/**/, ubDepth); // read fail plane
	gubRS_FPU_ReadGetLDPC_BCH = 0;

	// Restore LDPC & BCH
	gFREG[FCTLL_RAW_DMA_ADR].L = ulDMAAddr + (ubStartFrameNo * IBF_BANK_LENGTH);
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(1, ubFrameNum, 1, 0);
	gFREG[FCTLL_FPU_ENTRY].L  = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);



	if (ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR) {
		if ((getIntRandValue(4) == 0) && FQ->FQPara.B.btUserData) {
			gubForceGenCRC32 = 1;
			if (!BURNER) {
				UartString("\nGenCRC32 ");
			}
			volatile P4KTable16B *pP4KTable = (volatile P4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET/* + (ubL4KCount * Spare4kSize)*/);
			pP4KTable->ulL4K_LCA = (~(pP4KTable->ulL4K_LCA));
		}
	}

	// Fill L4K table
	volatile L4KTable16B *pL4KTable;
	UBYTE ubL4KCount = 0;
	for (ubL4KCount = 0; ubL4KCount < ubFrameNum; ubL4KCount++) {
		pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + DUMMY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
		//set L4K table info
		pL4KTable->ulL4K_LCA = 0x00000000;
		pL4KTable->Para0x04.ulAll = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (CorrectRSDataStart + ((ubStartFrameNo + ubL4KCount) * 4096)) / 512;
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
	}
	gFREG[FCTLL_L4K_SPR_ADR].L = DUMMY_L4K_TABLE_OFFSET;


	// Restore FPU
	if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE && gubRSNormalModeCorrectDataBefore) {
		// copy correct data & spare
		U32 ulSpareAddr = CorrectRSSpareStart + (gubRSCorrectPageIndex * Spare16kSize);
		memcpy((void *)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET), (void *)(ulSpareAddr + (ubStartFrameNo * Spare4kSize)), (ubFrameNum * Spare4kSize));
	}
	gFREG[FCTLL_BACK_STORE].W.W0 = (HB_RETRY_L4K_TABLE_OFFSET);
	if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE) {
		gFREG[FCTLL_RAW_DMA_ADR].L = (U32)(CorrectRSDataStart + (gubRSCorrectPageIndex * 16384) + (ubStartFrameNo * 4096));
	}
	else {
		gFREG[FCTLL_RAW_DMA_ADR].L = (U32)(CorrectRSDataStart + (ubStartFrameNo * 4096));
	}
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(1, ubFrameNum, 1, 1);
	gFREG[FCTLL_FPU_ENTRY].L  = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);





	// Fill the FPU COR setting
	UBYTE ubRetry4K_IDX = 0;
	U32 ulRetryZIPMAP = 0;
	while (ubRetry4K_IDX < ubFrameNum) {
		ulRetryZIPMAP |= (0xFF << ((ubStartFrameNo + ubRetry4K_IDX) * 8));
		ubRetry4K_IDX++;
	}
	gFREG[FCTLL_ZIP_MAP].L = ulRetryZIPMAP;
	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubFrameNum);
	if (ENABLE_iFSA) {
		gFREG[FCTLL_IFSA0].L = gMTQ->Depth[FQ->ubDepth].Para0x2C.uliFSA0;
	}
	else {
		gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
		gFREG[FCTLL_FSA_SEL].L  = FQ->ubFSAIndex;
	}
	gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
	gFREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(FQ->ubALUIndex);

	// FPU COR
	gFREG[FCTLL_DMA_CFG].L &= TRANSFER_0_ECC_FRAME;
	gFREG[FCTLL_DMA_CFG].L |= (((UWORD)gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum) << 8);
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;


	//#if ENABLE_RUNTIME_SEEDINIT


	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_IBF_CORRECT;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);


	while (gubINTTable[ubDepth].All == 0); // use this to check

	if (gubINTTable[ubDepth].All & 0xF8) {
		VT->gulReadFailRetryRSFailFPUCORagain_Fail_DebugCnt++;
		if (ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN_UART) {
			UartString("\nAg_Fail");
		}
	}
	else {
		memcpy((void *)(HB_RETRY_L4K_TABLE_ADDR), (void *)(IRAM_BVCI_BASE + DUMMY_L4K_TABLE_OFFSET), (ubFrameNum * Spare4kSize));
		ubPass = 1;
		VT->gulReadFailRetryRSFailFPUCORagain_Pass_DebugCnt++;
		if (ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN_UART) {
			UartString("\nAg_Pass!");
		}
	}

	gubINTTable[ubDepth].All = 0;
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
	gFREG[FCTLL_IBF_CTL].L &= (~FORCE_ZIP_COR);

	return ubPass;
#endif

#if(RDT || BURNER)
	return 0;
#endif
}

UBYTE flaReadFailRetryDummyFPUCOR(UBYTE ubgFQLinkIndex, UBYTE ubDepth, UBYTE ubFailFrameNoInMT)
{
	// Due to LCA, CRC32 error, do the dummy FPUCOR
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	UBYTE ubPass = 1;
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubL4KNum = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum;
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	UBYTE ubStartFrameNo = FQ->ulFEntry & gub4kEntrysPerPlaneMask;
	UBYTE ubFrameNum = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum;
	UBYTE ubBackupCRC32Enable = ((FCONL[FCONL_CRC_EN] & CRC_32_EN) ? 1 : 0);

	UBYTE ubFirstFail = 1;
MARK_DUMMYFPUCOR:
	if (ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
		// Reset IBF PTR by FW to solve disable IBF_RING_MODE_EN will cause another channel IBF PTR error bug
		flaReadFailRetryResetIBFPTR(ubChannel, ubDepth);
	}
	else {
		if (ubChannel == 0) {
			FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH0_EN);
		}
		else {
			FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH1_EN);
		}
	}


	if ((gubEnterSB == 0) && (gubEnterRS == 0)) {

		flaReadFailRetryReGenP4KCRC32(ubgFQLinkIndex, ubDepth);

	}


	if (ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR || (ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN && gubEnterRS && ubFirstFail)) {
		if ((getIntRandValue(4) == 0) && FQ->FQPara.B.btUserData) {
			gubForceGenCRC32 = 1;
			if (!BURNER) {
				UartString("\nGenCRC32 ");
			}
			volatile P4KTable16B *pP4KTable = (volatile P4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET/* + (ubL4KCount * Spare4kSize)*/);
			if (ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN) {
				if (ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN_UART) {
					UartLongHex(pP4KTable->ulCRC32);
				}
				U32 ulGetBit = (1 << getIntRandValue(32));
				pP4KTable->ulCRC32 = ((pP4KTable->ulCRC32 & ulGetBit) == 0) ? (pP4KTable->ulCRC32 | ulGetBit) : (pP4KTable->ulCRC32 & (~ulGetBit));
				if (ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN_UART) {
					UartString("\nAfter ");
					UartLongHex(pP4KTable->ulCRC32);
				}
			}
			else {
				pP4KTable->ulL4K_LCA = (~(pP4KTable->ulL4K_LCA));
			}
		}
	}

	// Fill L4K table
	volatile L4KTable16B *pL4KTable;
	volatile L4KTable16B *pFQL4KTable;
	UBYTE ubL4KCount = 0;
	for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
		pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + DUMMY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
		pFQL4KTable = (volatile L4KTable16B*)(L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
		//set L4K table info
		if (gubEnterReadFailRetryCondition != LCA_FW_CMP_FAIL_MAP) {
			pL4KTable->ulL4K_LCA = pFQL4KTable->ulL4K_LCA;
		}
		else {
			pL4KTable->ulL4K_LCA = ((FQ->ulSpare + ubL4KCount) * 8);
		}
		pL4KTable->Para0x04.ulAll = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubflaBuffer[(16384 + (ubL4KCount * 4096))])) / 512;
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
		if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE) {
			pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0;
		}
		else {
			// Copy bufMask for signoffmask fail
			pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = pFQL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK;
		}
	}
	gFREG[FCTLL_L4K_SPR_ADR].L = DUMMY_L4K_TABLE_OFFSET;


	// Restore FPU
	if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE && gubRSNormalModeCorrectDataBefore) {
		// copy correct data & spare
		U32 ulSpareAddr = CorrectRSSpareStart + (gubRSCorrectPageIndex * Spare16kSize);
		memcpy((void *)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET), (void *)(ulSpareAddr + (ubStartFrameNo * Spare4kSize)), (ubFrameNum * Spare4kSize));
	}
	gFREG[FCTLL_BACK_STORE].W.W0 = (HB_RETRY_L4K_TABLE_OFFSET);
	if (gubEnterRS) {
		if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE) {
			gFREG[FCTLL_RAW_DMA_ADR].L = (U32)(CorrectRSDataStart + (gubRSCorrectPageIndex * 16384) + (ubStartFrameNo * 4096));
		}
		else {
			gFREG[FCTLL_RAW_DMA_ADR].L = (U32)(CorrectRSDataStart + (ubStartFrameNo * 4096));
		}
	}
	else {
		gFREG[FCTLL_RAW_DMA_ADR].L = ((U32)(&gubflaBuffer[0]));
	}
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(1, ubFrameNum, 1, 1);
	gFREG[FCTLL_FPU_ENTRY].L  = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);





	// Fill the FPU COR setting

	gFREG[FCTLL_CHNL_SET].L &= (~BYPASS_CONV);



	gFREG[FCTLL_ZIP_CFG].L &= (~BYPASS_ZIP);
	if (gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btZipBypass) {
		gFREG[FCTLL_ZIP_CFG].L |= (BYPASS_ZIP);
	}
	gFREG[FCTLL_ZIP_MAP].L = gMTQ->Depth[FQ->ubDepth].ulZipMap;
	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum);
	if (ENABLE_iFSA) {
		gFREG[FCTLL_IFSA0].L = gMTQ->Depth[FQ->ubDepth].Para0x2C.uliFSA0;
	}
	else {
		gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
		gFREG[FCTLL_FSA_SEL].L  = FQ->ubFSAIndex;
	}
	gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
	gFREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(FQ->ubALUIndex);
	gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
	if (FQ->FQPara.B.btUserData && gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btCmpEn) {
		gFREG[FCTLL_CMP_CFG].L |= LCA_FW_COMPARE_EN;
		if (ENABLE_DEBUG_BB_ITEM_CROSS_FRAME_ISSUE && gubUNCandLCAfailCondition) {
			gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
		}
	}
#if ENABLE_DEBUG_EMULATOR
	gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
#endif

	// FPU COR
	gFREG[FCTLL_DMA_CFG].L &= TRANSFER_0_ECC_FRAME;
	gFREG[FCTLL_DMA_CFG].L |= (((UWORD)gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum) << 8);
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_IBF_CTL].L &= CLR_LDPC_COR_MAP;
	gFREG[FCTLL_OTHER_SET].L &= (~BCH_COR_EN);
	if (gubEnterSB && (gubEnterRS == 0) && (gubEnterReadFailRetryCondition != CRC32_ERR_MAP)) {
		FCONL[FCONL_CRC_EN] &= (~CRC_32_EN); // SB trapping set must off crc32
	}

	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_IBF_CORRECT;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);


	while (gubINTTable[ubDepth].All == 0); // use this to check
	if (gubINTTable[ubDepth].All & 0xF8) {

		if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE && gubRSNormalModeCorrectDataBefore) {
			if (ENABLE_WP_DEBUG) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(1);
			}
		}

		if (ENABLE_DEBUG_UART_NES_RS && ENABLE_DEBUG_NES_READ_FAIL) {
			UartString(" debug_UNC1");
		}
		if (ENABLE_DEBUG_NES_READ_FAIL && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE)) {
			flaLEDBlink(1);
		}

		if (ENABLE_DEBUG_RS_VERIFY_DATA && gubRSDebug_NoGoldenToVerifyDATA && (!ENABLE_DEBUG_RS_NORMAL_MODE_FAIL_BLOCK)) {
			mDMAC_COPY( CorrectRSDataStart, (U32)&gubZ2Buffer[0], 16384, 0, 0, 0, 0, 1);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)((U32)&gubZ2Buffer[0]), 16384);
			}
			if (!BURNER) {
				UartString("\nDC: ");
			}
			_UCV ULLONG *pData = ((_UCV ULLONG *)((U32)&gubZ2Buffer[0]));
			UartLLongHex(pData[0]);

			mDMAC_COPY( VerifyDecodeRSDataStart, ((U32)&gubZ2Buffer[16384]), 16384, 0, 0, 0, 0, 1);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)((U32)&gubZ2Buffer[16384]), 16384);
			}
			if (!BURNER) {
				UartString(" DG: ");
			}
			pData = ((_UCV ULLONG *)((U32)&gubZ2Buffer[16384]));
			UartLLongHex(pData[0]);

			flaLEDBlink(5);
		}

		if (gubEnterRS && ((gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) || (gubEnterReadFailRetryCondition == CRC32_ERR_MAP) || (gubEnterReadFailRetryCondition == FW_SETTING_ERR_MAP))) {
			if (ENABLE_WP_DEBUG) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(1);
			}
		}

		if (!gubEnterRS) {
			if ((gFREG[FCTLL_INT_INF].L & CRC32_ERROR) == CRC32_ERROR) {
				if (ENABLE_DEBUG_UART_FAIL) {
					UartString("\nCRC32");
				}
				VT->gsReadFailDebugCount.gulRetryCRC32InDummyFPUCOR_LogCount++;
				if (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR) {
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(1);
					}
				}
			}
			else if ((gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) == LCA_FW_CMP_ERR_INF) {
				if (ENABLE_DEBUG_UART_FAIL) {
					UartString("\nLCA");
				}
				VT->gsReadFailDebugCount.gulRetryLCAErrorInDummyFPUCOR_LogCount++;
				if (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR) {
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(1);
					}
				}
			}
			else if ((gFREG[FCTLL_INT_INF].L & FW_SETTING_ERR_INF) == FW_SETTING_ERR_INF) {
				if (ENABLE_DEBUG_UART_FAIL) {
					UartString("\nFW_SETTING");
				}
				VT->gsReadFailDebugCount.gulRetryFWSettingErrorInDummyFPUCOR_LogCount++;
				if (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR) {
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(1);
					}
				}
			}
		}

		if (gFREG[FCTLL_INT_INF].L & (CRC32_ERROR | LCA_FW_CMP_ERR_INF | FW_SETTING_ERR_INF | DECOMPRESSION_FAIL_INF | CRC16_ERROR)) {
			ubPass = 0;
			if (gubEnterRS) {
				VT->gulRSRecoverFailCnt++;
			}
			else {
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(0xF3);
				}
				else {
					gubWP_DEBUG = 0xF3;
					if (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR) {
						M_AssertError(1);
					}
				}
			}
			// Restore setting for SB / RS
			gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
			gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
			gFREG[FCTLL_ZIP_CFG].L &= (~L4K_BADR_CONTINUOUS_MODE);
			gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);
			gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
			if (gFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) {
				flaReadFailRetryReFillLCA(ubgFQLinkIndex);
			}

			// Refill L4K Table for HB read
			UBYTE ubL4KIndex = 0;
			UBYTE ubL4KNUM = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum;
			volatile L4KTable16B *pL4KTable;
			for (ubL4KIndex = 0; ubL4KIndex < ubL4KNUM; ubL4KIndex++) {
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KIndex * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubflaBuffer[(ubL4KIndex * 4096)])) / 512;
				pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			}

			if (ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN && gubEnterRS && ubFirstFail) {
				// Check fail and do FPUCOR again
				ubFirstFail = 0;
				gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
				gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
				gubINTTable[ubDepth].All = 0;
				if (flaReadFailRetryRSFailFPUCORagain(ubgFQLinkIndex, ubDepth)) {
					// FPUCOR PASS
					ubPass = 1;
					goto MARK_DUMMYFPUCOR;
				}
			}
		}
		else {
			if (ENABLE_WP_DEBUG) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(1);
			}
		}
		//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET);
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	}

	gubINTTable[ubDepth].All = 0;
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling

	gFREG[FCTLL_IBF_CTL].L |= LDPC_COR_MAP(0x1F);
	gFREG[FCTLL_OTHER_SET].L |= (BCH_COR_EN);
	if (gubEnterSB && (gubEnterRS == 0) && (gubEnterReadFailRetryCondition != CRC32_ERR_MAP)) {
		if (ubBackupCRC32Enable)
			FCONL[FCONL_CRC_EN] |= (CRC_32_EN);
	}

	if (!ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
		if (ubChannel == 0) {
			FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH0_EN);
		}
		else {
			FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH1_EN);
		}
	}
#endif
	return ubPass;

#if(RDT)
	return 0;
#endif
}

void flaReadFailRetryReFillLCA(UBYTE ubgFQLinkIndex)
{
	M_CheckBurnerModeToDisableFunction();
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	volatile L4KTable16B * pubFillL4KLCA_TABLE = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize)); // keep volatile??
	UBYTE ubL4K_Index = 0;

	if (FQ->FQPara.B.btRWTableUnit || gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btZipBypass) {
		if (ENABLE_WP_DEBUG) {
			flaLEDBlink(0xF2);
		}

		for (ubL4K_Index = 0; ubL4K_Index < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubL4K_Index++) {
			pubFillL4KLCA_TABLE = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4K_Index) * Spare4kSize));
			pubFillL4KLCA_TABLE->ulL4K_LCA = (FQ->ulSpare);
		}
	}
	else {
		for (ubL4K_Index = 0; ubL4K_Index < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubL4K_Index++) {
			pubFillL4KLCA_TABLE = (volatile L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4K_Index) * Spare4kSize));
			pubFillL4KLCA_TABLE->ulL4K_LCA = ((FQ->ulSpare + ubL4K_Index) * 8);
		}
	}
}

void flaReadFailRetrySetUNCAddr(U32 ulAddr, U32 ulFailLBA)
{
	M_CheckBurnerModeToDisableFunction();

	if (ulAddr >= 0x2C020000) {
		ulAddr -= 0x20000;
	}

	UBYTE ubCheckSameUNC = 0;
	UBYTE ubi = 0;
	for (ubi = 0; ubi < gubUncPtr; ubi ++) {
		if (gulSATAUncAddr[ubi] == ulAddr) {
			ubCheckSameUNC = 1;
		}
	}

	if (gubUncPtr < FLA_READUNC_MAXLOG_CNT && (ubCheckSameUNC == 0)) {
		gulSATAUncAddr[gubUncPtr] = ulAddr;

		gulFailLBAforLog[gubUncPtr] = ulFailLBA;

		gubUncPtr++;
		if (gubUncPtr > VT->gubDebugUncPtr) {
			VT->gubDebugUncPtr = gubUncPtr;
		}
	}
	HL[HL_RD_UNC_ADDR] = ulAddr;
}

UBYTE flaReadFailRetryCheckFirstReadFailL4KDuetoUNCorSignoffMaskFail(UBYTE ubgFQLinkIndex, UBYTE ubFailFrameNoInMT)
{
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);

	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubFailL4KNo = 0;

	// Get UNC fail map
	ubFailL4KNo = flaReadFailRetryGetL4KFailNo(ubgFQLinkIndex, ubFailFrameNoInMT);


	// Get SignoffMask fail map
	UBYTE ubSignoffMaskFailL4KIndex = 0;
	U32 ulSignoffMaskFailFrameMapInMT = FQ->ulFIPError.All;
	M_Get_Error_4K_Index(ubSignoffMaskFailL4KIndex, (gubMaxZipRate * gub4kEntrysPerPlane), ulSignoffMaskFailFrameMapInMT); // max: 16個L4K


	if (ubSignoffMaskFailL4KIndex < ubFailL4KNo) {
		// is signoffmask fail
		ubFailL4KNo = ubSignoffMaskFailL4KIndex;
		FQ->btReadUNC = 0;
		FQ->btSpareInvalid = 1;
		VT->gulSignOffFail++;
	}

	return ubFailL4KNo;
}

#if ENABLE_AUTOPOL_TIMEOUT_HANDLING

UBYTE flaMTCheckIsAutoPollTimeout(UBYTE ubgFQLinkIndex)
{
	M_CheckRDTModeToDisableFunctionReturnValid(0);

	UBYTE ubCheckIsAutoPollTimeout = TRUE;
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	U32 ulFCE_ENB, ulExpect_FCE_ENB;
	UBYTE ubTimeoutCEIndex = 0xFF;
	UBYTE ubi;

	//Because poll status command done, CE enable will auto pull high by HW (MT_AUTO_CE_CLR)
	//In the moment FCONL[FCONL_FCE_ENB] will get all zero
	//So this while loop gets two times to avoid this condition
	do {
		ulFCE_ENB = FCONL[FCONL_FCE_ENB];
	}
	while (ulFCE_ENB != FCONL[FCONL_FCE_ENB]);


	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		ulExpect_FCE_ENB = M_MapToCEDecoder((FQ->ubCEIndex & gubPlanesPerBurstMask), gubCH_CEMappingToPCE[(FQ->ubCEIndex & gubPlanesPerBurstMask)][(FQ->ubCEIndex / gubPlanesPerBurst)]);

		if ((ulFCE_ENB & ulExpect_FCE_ENB) == ulExpect_FCE_ENB) {
			ubTimeoutCEIndex = FQ->ubCEIndex;
		}
	}
	else {
		for (ubi = 0; ubi < MAX_CE; ubi++) {
			if (ulFCE_ENB & BIT0) {
				//M_AssertError(ubTimeoutCEIndex != 0xFF);
				if (FQ->ubCEIndex == (((ubi & 7) << 1) + (ubi >> 3))) {
					ubTimeoutCEIndex = FQ->ubCEIndex;
					break;
				}
			}
			ulFCE_ENB >>= 1;
		}
	}

	if ((FQ->ubCEIndex != ubTimeoutCEIndex) || (FQ->ubL4kIndex != gFQI.ubFQIndex[ubTimeoutCEIndex][gFQI.ubFQFirst[ubTimeoutCEIndex]])) {
		ubCheckIsAutoPollTimeout = FALSE;
	}

	return ubCheckIsAutoPollTimeout;
}

void flaMTAutoPollTimeoutHandling(UBYTE ubgFQLinkIndex)
{
	M_CheckRDTModeToDisableFunction();

	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	gubTimeoutEnterRebuildFQ = 1;
	gubTimeoutFQIndex = FQ->ubL4kIndex;
#if FORCE_AUTOPOL_TIMEOUT
	if (gubDebugAutoPolFQIndex != gubTimeoutFQIndex) {
		UartString("\nDAFQ:");
		UartWordHex((UWORD)gubDebugAutoPolFQIndex);
		UartString("\nTOFQ:");
		UartWordHex((UWORD)gubTimeoutFQIndex);
	}
#endif

#if BURNER
	// MTQ manual abort
	gFREG[FCTLL_INT_CFG].L |= FORCE_DMA_ABORT;
#else
	gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL); //到flaRebuildFQPhase裡面放掉force empty
#endif

	gFREG[FCTLL_INT_RDY].L |= (BIT0 << (FQ->ubCEIndex >> gubPlanesPerBurstLog));

	// wait FIP ready
	//while ((gFREG[FCTLL_RESERVE].L & BIT0) != 1); // 拿掉FORCE_DMA_ABORT了

	// fix the future of copy data bug & remain 2 FQ (1 program parity + 1 read FQ) bug & the future mix of one and two(already finish first) parity bug
	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN));

	if ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) == 0) {
		// if FPU is idle...reset flash by PIO (有機會又poll的到)

		// wait the channel true idle when force empty
		gFREG[FCTLL_DBG_INF].L = (0x0C);
		while ((GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));

		UBYTE ubChannel = (FQ->ubCEIndex & gubPlanesPerBurstMask);
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(FQ->ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
		}
		else {
			FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (FQ->ubCEIndex / gubPlanesPerBurst); // Enable selected CE
		}


		gFREG[FCTLL_PIO_CMD].B.B0 = 0xFF;

		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
		}
		else {
			FCONL[FCONL_FCE_CLR] = (ubChannel * 8) + (FQ->ubCEIndex / gubPlanesPerBurst); // Disable selected CE
		}

		// delay 1ms+ to wait status ready
		idle(200000);
	}

	// clear all MTQ in channel
	gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
	gFREG[FCTLL_MT_TRIG].B.B1 = 0;

	// push a dummy MT , let reuseMT signal can be released
	UBYTE ubTempDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
	gMTQ->Depth[ubTempDepth].Para0x20.BitMap.uw2ndFPUPtr = FPU_PTR_C70_POL_MK20;
	gMTQ->Depth[ubTempDepth].Para0x20.BitMap.btbtIntVctEn2 = 0;
	gMTQ->Depth[ubTempDepth].Para0x20.BitMap.btSetInternalBusy2 = 0;
	flaSendCmd_CH(FQ->ubL4kIndex, FQ->ubCEIndex, FPU_PTR_C70_POL_MK20, 0, 0, 0xFF);
	gubMTDepthPushNumber[FQ->ubCEIndex]--;
	M_AddMTDepth(ubTempDepth);

#if FORCE_AUTOPOL_TIMEOUT
	// change polling mask to be 0x20, so that autopol can be pass
	IRAMB[FPU_PTR_C70_DAT_MK00 + 6] = 0x20;
#endif

#if BURNER
	// release MTQ abort
	gFREG[FCTLL_INT_CFG].L &= (~FORCE_DMA_ABORT);
#endif

	// delay 1ms+ to wait status ready
	idle(200000);

#if FORCE_AUTOPOL_TIMEOUT
	// after autopol is pass, change the mask back to 0x00
	IRAMB[FPU_PTR_C70_DAT_MK00 + 6] = 0x00;
	//M_AssertError(gubDebugAutoPolFPUSeqInUse == 0);
	gubDebugAutoPolFPUSeqInUse = 0;
	gubDebugAutoPolFQIndex = 0xFF;
	gMTQ->Depth[gubDebugAutoPolMTDepth].Para0x1C.BitMap.ubPolSeqSel = WAIT_TRUE_READY;
#endif

#if ENABLE_READ_AUTO_POL_TIMEOUT_HANDLING
	if (FQ->btPolFailTimeoutCnt == 0) {
		ftlRecordFailLog(FLHErrLogType, 0x1, ubgFQLinkIndex, BYTE_TIMEOUT_AUTOPOL, DONTCARE, BIT_NormalErr);
	}
	FQ->btPolFailTimeoutCnt++;
	if (FQ->ubFJob == BYTE_FJOB_READ) {
		if (gMTQ->Depth[FQ->ubDepth].Para0x00.BitMap.btSetInternalBusy) {
			VT->gul00_30ReadAutoPolTimeoutCnt++;
			if (FQ->btPolFailTimeoutCnt > 5) {
				VT->gul00_30ReadAutoPolTimeoutReach5LimitCnt++;
				gMTQ->Depth[FQ->ubDepth].Para0x00.BitMap.btSetInternalBusy = 0; // let 00-5Addr-30 打完就好(發partial done)
			}
		}
	}
	else {
		VT->gulAutoPolTimeoutCnt++;
	}
#else
	ftlRecordFailLog(FLHErrLogType, 0x1, ubgFQLinkIndex, BYTE_TIMEOUT_AUTOPOL, DONTCARE, BIT_NormalErr);
	VT->gulAutoPolTimeoutCnt++;
#endif
	// rebuild FQ
	flaRebuildFQPhase(ubgFQLinkIndex);
#if UART_AUTOPOL_TIMEOUT_HANDLING
	UartString(" (Timeout)");
#endif


	if (FQ->ubFJob == BYTE_FJOB_WRITE || FQ->ubFJob == BYTE_FJOB_ERASE_BLOCK || FQ->ubFJob == BYTE_FJOB_ERASE_UNIT) {
		FQ->btPEFail = 1;

		// doesn't mark bad but need to do copy unit
		flaMarkFail(ubgFQLinkIndex, AUTOPOL_TIMEOUT_FAIL, 0);

		// let it be All_Done
		gubINTTable[FQ->ubDepth].All = 0x01;
	}

#if BURNER
	if (FQ->ubFJob == BYTE_FJOB_READ) {
		gubINTTable[FQ->ubDepth].All = 0x01;
	}
#endif

	gubTimeoutEnterRebuildFQ = 0;
}

void flaReadFailRetryAutoPollTimeoutHandling(UBYTE ubgFQLinkIndex, U32 ulFCE_ENB, UBYTE ubAbnormalLoc)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	volatile REG_t *NonUncChannelFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (((ubChannel + 1) & 1) * 0x200));
	UBYTE ubTimeoutCEIndex = 0xFF;
	UBYTE ubi;
	UBYTE ubTempDepth;
	FlashQueue_t *TimuoutFQ;
	gubTimeoutEnterRebuildFQ = 1;

	for (ubi = 0; ubi < MAX_CE; ubi++) {
		if (ulFCE_ENB & BIT0) {
			M_AssertError(ubTimeoutCEIndex != 0xFF);
			ubTimeoutCEIndex = ((ubi & 7) << 1) + (ubi >> 3);
		}
		ulFCE_ENB >>= 1;
	}
	M_AssertError(ubTimeoutCEIndex == 0xFF);
	M_AssertError((ubTimeoutCEIndex & gubPlanesPerBurstMask) == ubChannel);
	// get CE first FQ, should be autopol timeout FQ
	gubTimeoutFQIndex = gFQI.ubFQIndex[ubTimeoutCEIndex][gFQI.ubFQFirst[ubTimeoutCEIndex]];
#if UART_AUTOPOL_TIMEOUT_HANDLING
	UartString(" CE:");
	UartWordHex(ubTimeoutCEIndex);
#endif
	TimuoutFQ = &gFQI.gFQLink[gubTimeoutFQIndex];
	M_AssertError(TimuoutFQ->ubCEIndex != ubTimeoutCEIndex);
	M_AssertError(TimuoutFQ->btUsingAutoPol == 0);
	M_AssertError(NonUncChannelFREG[FCTLL_INT_INF].L & INTR_BUSY);
	// MTQ manual abort
	//NonUncChannelFREG[FCTLL_INT_CFG].L |= FORCE_DMA_ABORT;
	NonUncChannelFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL); //到FPUCOR PASS or RS_Recover done在放掉force empty

	NonUncChannelFREG[FCTLL_INT_RDY].L |= (BIT0 << (TimuoutFQ->ubCEIndex >> gubPlanesPerBurstLog));

	// wait FIP ready
	//while ((NonUncChannelFREG[FCTLL_RESERVE].L & BIT0) != 1); // 拿掉FORCE_DMA_ABORT了

	// fix the future of copy data bug & remain 2 FQ (1 program parity + 1 read FQ) bug & the future mix of one and two(already finish first) parity bug
	NonUncChannelFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN));

	if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) == 0) {
		// if FPU is idle...reset flash by PIO (有機會又poll的到)

		// wait the channel true idle when force empty
		NonUncChannelFREG[FCTLL_DBG_INF].L = (0x0C);
		while ((GET_DBG_MTQ_STS(NonUncChannelFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(NonUncChannelFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));


		UBYTE ubChannel = (TimuoutFQ->ubCEIndex & gubPlanesPerBurstMask);
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(TimuoutFQ->ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
		}
		else {
			FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (TimuoutFQ->ubCEIndex / gubPlanesPerBurst); // Enable selected CE
		}


		NonUncChannelFREG[FCTLL_PIO_CMD].B.B0 = 0xFF;


		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
		}
		else {
			FCONL[FCONL_FCE_CLR] = (ubChannel * 8) + (TimuoutFQ->ubCEIndex / gubPlanesPerBurst); // Disable selected CE
		}

		// delay 1ms+ to wait status ready
		idle(200000);
	}

	// clear all MTQ in channel
	NonUncChannelFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
	NonUncChannelFREG[FCTLL_MT_TRIG].B.B1 = 0;

	// push a dummy MT , let reuseMT signal can be released
	ubTempDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];
	gMTQ->Depth[ubTempDepth].Para0x20.BitMap.uw2ndFPUPtr = FPU_PTR_C70_POL_MK20;
	gMTQ->Depth[ubTempDepth].Para0x20.BitMap.btbtIntVctEn2 = 0;
	gMTQ->Depth[ubTempDepth].Para0x20.BitMap.btSetInternalBusy2 = 0;
	flaSendCmd_CH(TimuoutFQ->ubL4kIndex, ubTimeoutCEIndex, FPU_PTR_C70_POL_MK20, 0, 0, 0xFF);
	gubMTDepthPushNumber[ubTimeoutCEIndex]--;
	M_AddMTDepth(ubTempDepth);

#if FORCE_AUTOPOL_TIMEOUT
	// change polling mask to be 0x20, so that autopol can be pass
	IRAMB[FPU_PTR_C70_DAT_MK00 + 6] = 0x20;
#endif
	// release MTQ abort
	//NonUncChannelFREG[FCTLL_INT_CFG].L &= (~FORCE_DMA_ABORT);

	// delay 1ms+ to wait status ready
	idle(200000);

	// make sure FPU & autopol is NOT busy
	if (((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (TIMEOUT | MT_BUSY | ANY_BUSY | SIGNOFF_BUSY | FPU_TRIG)) != 0)
	        || (NonUncChannelFREG[FCTLL_INT_RDY].B.B0 != 0xFF) ) {
		//UBYTE ubTimeoutChannel = ((ubChannel + 1) & gubPlanesPerBurstMask);

		// Clear all MTQ in channel
		NonUncChannelFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
		NonUncChannelFREG[FCTLL_MT_TRIG].B.B1 = 0;
		idle(0x1999998);

		if (ENABLE_DEBUG_UART_FIP_RESET_IN_PROGRAM_TIMEOUT || UART_AUTOPOL_TIMEOUT_HANDLING) {
			UartString("\nFW_RST4! ");
		}
		VT->gulTimeoutHandle_RetryNonUncChannelAutoPollTimeout_DebugCnt++;
		flaFIPTimeoutErrorHandle(TimuoutFQ->ubCEIndex);
	}

#if FORCE_AUTOPOL_TIMEOUT
	// after autopol is pass, change the mask back to 0x00
	IRAMB[FPU_PTR_C70_DAT_MK00 + 6] = 0x00;
	M_AssertError(gubDebugAutoPolFPUSeqInUse == 0);
	gubDebugAutoPolFPUSeqInUse = 0;
	gubDebugAutoPolFQIndex = 0xFF;
	gMTQ->Depth[gubDebugAutoPolMTDepth].Para0x1C.BitMap.ubPolSeqSel = WAIT_TRUE_READY;
#endif
	VT->gulAutoPolTimeoutCnt++;
	ftlRecordFailLog(FLHErrLogType, ubAbnormalLoc, gubTimeoutFQIndex, BYTE_TIMEOUT_AUTOPOL, DONTCARE, BIT_NormalErr);
	// clear MTQ, need rebuild later
	gubRSclearMTQRebuildFQ = 1;
	if (TimuoutFQ->ubFJob == BYTE_FJOB_WRITE) {
		TimuoutFQ->btPEFail = 1;
		// doesn't mark bad but need to do copy unit
		flaMarkFail(gubTimeoutFQIndex, AUTOPOL_TIMEOUT_FAIL, 0);
		gubINTTable[TimuoutFQ->ubDepth].All = 0x01;
	}
#if ENABLE_DEBUG_RW
	// if ENABLE_DEBUG_RW, flaSendCmd_CH will modify gFREG , restore here
	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
#endif
	gubTimeoutEnterRebuildFQ = 0;
}
#endif

void flaReadFailRetry(UBYTE ubgFQLinkIndex)
{
#if(!RDT)
#if (!BURNER)
	M_CheckBurnerModeToDisableFunction();
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubCEIndex = FQ->ubCEIndex;
	UBYTE ubResetNormalReadPass = 0;
#if TLC
	UBYTE ubD3LMU;

#if B0KB
	ubD3LMU = Micron_CheckPage((M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));

#else
	if ( TLC ) {
		ubD3LMU = ( ( (M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) % 3 ) + 1 );
	}
#endif
#endif
	if (ENABLE_DEBUG_UART_FAIL || ENABLE_DEBUG_UART_NES_RS || TLC_NES_TEST) {
		UartString("\nRFFQ: ");
		Uart_Tx_DataHex(ubgFQLinkIndex);
		if (ENABLE_DEBUG_UART_NES_RS) {
			UartString(" PlanePTR ");
			UartLongHex(FQ->ulFEntry >> gub4kEntrysPerPlaneLog);
		}
	}
#if	(ENABLE_RECORD_RETRY_INFO && ENABLE_RECORD_D1TOD3_READRETRY && (!BURNER))
	if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
		VT->gsReadFailDebugCount.gulRetryPassCount[RECORD_D1TOD3_INFO_BASE + 0] ++ ;			// D1 to D3 read fail retry count
		VT->gsReadFailDebugCount.gulRetryPassCount[RECORD_D1TOD3_INFO_BASE + 1] = FQ->uwFUnit;	// retry unit
		VT->gsReadFailDebugCount.gulRetryPassCount[RECORD_D1TOD3_INFO_BASE + 2] = FQ->ulFEntry;	// retry entry
	}
#endif

#if DUMP_READ_FAIL_RETRY_TIME==1
	U32 ulReadFailRetryTimeRecord[2];
	UBYTE ubEnterBunchRSRecover = 0;

	ulReadFailRetryTimeRecord[0] = gulOperationTime;
#endif

	if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE/* && gubEnterftlCopyUnit*/) {

	}
	else {
		gubEnterRSNormalMode = 0;
	}
	gubRSNormalModeCorrectDataBefore = 0;
	gubRSCorrectPageIndex = 0;
	gubEnterHB = 0;
	gubEnterRS = 0;
	gubEnterSB = 0;
	gubFPUCORClearBufMask = 0;
	if (ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR || ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN) {
		gubForceGenCRC32 = 0;
	}

	if (gubSSD_Init_State.B.btCleanRAMForReadRetryDone == 0) {
		mDMAC_SV(4, (U32)gubflaBuffer, 128 * 1024, 0, 0, 0, DMA_EN_SETCQ, 0); // FlaBuffer
		gubSSD_Init_State.B.btCleanRAMForReadRetryDone = 1;
	}
	gubFPUTimeout = 0;
	M_VirtualToPhysicalCE(ubCEIndex);

	// force empty to avoid program parity clear interrupt vector enable bit bug
	gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);

	// wait the channel idle for read retry
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	// wait the channel true idle when force empty
	gFREG[FCTLL_DBG_INF].L = (0x0C);
	while ((GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));


	if (Failed_Test_UNC || ENABLE_DEBUG_HW_FORCEREADFAIL) {
		U32 ulHWGenFailinMT = gFREG[FCTLL_UNC_CFG].L;
		gulWP_DEBUG_HWgenFail = ulHWGenFailinMT;
		gFREG[FCTLL_UNC_CFG].L = 0; //避免UltraDMA update FCTLL_UNC_CFG


		gubRetryGenFailPhase = 0;

		if (ENABLE_DEBUG_ALL_RETRY) {
			if (ENABLE_DEBUG_ALL_RETRY_EXCEPT_UNC) {
				gubRetryGenFailPhase = getIntRandValue(4);
			}
			else {
				if (FQ->FQPara.B.btRWTableUnit) {
					gubRetryGenFailPhase = getIntRandValue(4);
				}
				else {
					gubRetryGenFailPhase = getIntRandValue(8);
				}
#if ENABLE_GEN_FAIL==1
#if FORCE_GEN_FAIL_TO_RS_RECOVER==1
				gubRetryGenFailPhase = 3;	// RS
#else
				gubRetryGenFailPhase = (rand() & 3);	// HB, SB, RS
				// gubRetryGenFailPhase = (rand() % 3); // HB, SB
				// gubRetryGenFailPhase = (rand() % 2); // HB
#endif
#endif
			}

			if (ENABLE_DEBUG_READ_ERASEPAGE_ERROR) {
				gubEnterReadFailRetryCondition = ERASE_PAGE_ERR;
				gubRetryGenFailPhase = 3;
			}

			if (((FQ->uwFJobInfo & BIT_FJOBI_RS) != BIT_FJOBI_RS) && ((guoFlashUseType.B.btA2Cmd) && (!ENABLE_DEBUG_FORCE_FASTPAGE_NONA2) && (((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) == BIT_FJOBI_FASTPAGE) /* || (!ENABLE_ULTRAMLC)*/))) {
				gubRetryGenFailPhase = 0;
			}
			else if ((FQ->uwFJobInfo & BIT_FJOBI_RS) != BIT_FJOBI_RS) {

				gubRetryGenFailPhase = getIntRandValue(3);

			}
		}

		if (ENABLE_DEBUG_HB_RETRY_ALL) {
			gubRetryGenFailPhase = 1; // HB correct
			UartString("\n===");
			UBYTE ubRetryIsFastPage = (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) ? 1 : 0;
			if ((guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0))  && (ubRetryIsFastPage/* || ENABLE_ULTRAMLC*/)) {
				if ((gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] + 1) == 0) {
					flaLEDBlink(11);
				}

				gubDebugHBRetryForcePassPhase = getIntRandValue(gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] + 1); // +1 for last normal read again

				UartString("D1 ");
			}
			else {
				if ((gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] + 1) == 0) {
					flaLEDBlink(11);
				}
				gubDebugHBRetryForcePassPhase = getIntRandValue(gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] + 1); // +1 for last normal read again
				UartString("D3 ");
			}
			UartString("Force Pass: ");
			Uart_Tx_DataHex(gubDebugHBRetryForcePassPhase);
			UartString(" ===\n");
		}

		if (ENABLE_DEBUG_SOFTBITCORRECTION && ((!guoFlashUseType.B.btA2Cmd) || ENABLE_DEBUG_FORCE_FASTPAGE_NONA2  || (((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) == 0) /* && (!ENABLE_ULTRAMLC)*/))) {

			{

				gubRetryGenFailPhase = 2; // SB correct
			}
		}

		if (ENABLE_DEBUG_RS && (FQ->uwFJobInfo & BIT_FJOBI_RS)) {
			gubRetryGenFailPhase = 3; // RS correct
		}

		if (ENABLE_DEBUG_ALL_UNC) {
			if (getIntRandValue(100) >= 50) {
				gubRetryGenFailPhase = 4;
			}
			else {
				gubRetryGenFailPhase = 5;
			}
		}

		if (Failed_Test_UNC && (FQ->uwFJobInfo & BIT_FJOBI_BUFFERMODE) && (FQ->FQPara.B.btUserData)) {
			UBYTE ubForceGen = 0; //使在真正發生fail需retry時, fw也有可能造fail (不能一直造fail, 否則會不斷Unc,造成data compare error)
			ubForceGen = getIntRandValue(0xff);

			if (ulHWGenFailinMT || (ubForceGen == 0)) {

				if (ENABLE_KT_DEBUG) {
					if ((ulHWGenFailinMT == 0) && (ubForceGen == 0)) {
						UartString("\r\nAnother Err!");
					}
				}
				gubRetryGenFailPhase = 6; // All UNC
			}
		}
		else if (getIntRandValue(100) >= 50) {
			gubRetryGenFailPhase = 0; // Normal read again
		}

		if (gubRetryGenFailPhase > 0) {
			if (getIntRandValue(100) >= 50) {
				gFREG[FCTLL_UNC_CFG].L |= (AUTOGEN_READ_FAIL_EN);
			}
			else {
				UBYTE ubLoop = 0;
				UBYTE ubLoopCount = 0;
				ubLoopCount = (getIntRandValue(4) + 1);
				while (ubLoop < ubLoopCount) {
					gFREG[FCTLL_UNC_CFG].L |= FORCE_BCH_UNCORRECTABLE_FRAME(1 << (getIntRandValue(4)));
					ubLoop++;
				}
			}
		}
	}


	// Save fail P4K no.
	UBYTE ubStartFrameNo = FQ->ulFEntry & gub4kEntrysPerPlaneMask;
	UBYTE ubSignoffmaskFailandUNCatTheSameTime = 0;
	if (FQ->btUncCheckInRSTwoCh) {
		gulGetUNCFrameMapInMT = (1 << (FQ->ubFirstUncFrameNoMapInRSTwoCh));
		if (FQ->ulFIPError.All & SIGNOFFMASK_INF) {
			ubSignoffmaskFailandUNCatTheSameTime = 1;
		}
	}
	else {
		if (gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) {
			ubSignoffmaskFailandUNCatTheSameTime = 1;
			gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
			gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(SIGNOFFMASK_MAP);
			FQ->ulFIPError.W.uwSignoffMaskFailLog = gFREG[FCTLL_MAP_INF].W.W0;
		}

		if ((gubEnterReadFailRetryCondition == UNC_MAP) || (gubEnterReadFailRetryCondition == ERASE_PAGE_ERR)) {
			if ((gFREG[FCTLL_INT_INF].L & UNC_MAP_INF) == UNC_MAP_INF) {
				gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(UNC_MAP);
				gulGetUNCFrameMapInMT = gFREG[FCTLL_MAP_INF].L;
			}
			else {
				// erase page
				gulGetUNCFrameMapInMT = (1 << ubStartFrameNo); // erase page must all empty
			}
		}
		else if (gubEnterReadFailRetryCondition == CRC32_ERR_MAP) {
			gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
			gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(CRC32_ERR_MAP);
			gulGetUNCFrameMapInMT = gFREG[FCTLL_MAP_INF].L; // phy
		}
		else if (gubEnterReadFailRetryCondition == CRC16_ERR_MAP) {
			gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
			gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(CRC16_ERR_MAP); // phy 512
			gulGetUNCFrameMapInMT = 0;
			UBYTE ubi = 0;
			while (ubi < 32) {
				if ((gFREG[FCTLL_MAP_INF].L >> ubi) & BIT0) {
					gulGetUNCFrameMapInMT |= (BIT0 << (ubi / 8));
					break;
				}
			}
		}
		else if ((gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) || (gubEnterReadFailRetryCondition == DECOMPRESSION_FAIL_MAP) || (gubEnterReadFailRetryCondition == FW_SETTING_ERR_MAP)) {
			gulGetUNCFrameMapInMT = (1 << ubStartFrameNo); // no phy frame, assume 0 fail at first
		}
	}


	UBYTE ubFailFrameNoInMT = 0;
	if (gubEnterReadFailRetryCondition != DQSCNT_MISMATCH) {
		M_Get_Error_4K_Index(ubFailFrameNoInMT, gub4kEntrysPerPlane, gulGetUNCFrameMapInMT);
	}


	// Save fail L4K no.
	if (gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) {
		// ReFill LCA
		flaReadFailRetryReFillLCA(ubgFQLinkIndex);

		if (FQ->btUncCheckInRSTwoCh) {
			gulGetUNCFrameMapInMT = (1 << (FQ->ubFirstUncFrameNoMapInRSTwoCh));//FQ->ubFirstUncFrameNoMapInRSTwoCh;
		}
		else {
			gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
			gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(LCA_FW_CMP_FAIL_MAP);
			gulGetUNCFrameMapInMT = 0;
			M_Get_Error_4K_Index(gulGetUNCFrameMapInMT, (gubMaxZipRate * gub4kEntrysPerPlane), gFREG[FCTLL_MAP_INF].L); // max: 16個L4K
		}
	}
	else if (gubEnterReadFailRetryCondition == DECOMPRESSION_FAIL_MAP) {
		if (FQ->btUncCheckInRSTwoCh) {
			gulGetUNCFrameMapInMT = (1 << (FQ->ubFirstUncFrameNoMapInRSTwoCh));//FQ->ubFirstUncFrameNoMapInRSTwoCh;
		}
		else {
			gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
			gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(DECOMPRESSION_FAIL_MAP);
			gulGetUNCFrameMapInMT = 0;
			M_Get_Error_4K_Index(gulGetUNCFrameMapInMT, (gubMaxZipRate * gub4kEntrysPerPlane), gFREG[FCTLL_MAP_INF].L); // max: 16個L4K
		}
	}

	// fill the retry read setting
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	guwReadRetryUnit =  FQ->uwFUnit;
#if (Read_Chk_ByHostRead || Read_Chk_ByReadScan)
	gubRRDIRECTACESS = (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS);
#endif
	gFREG[FCTLL_INT_CFG].L &= (~ABORT_DONE_INTR_EN); // follow FLH RegSpec 4.19 MTQ Manual Abort
	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix the future of copy data bug & remain 2 FQ (1 program parity + 1 read FQ) bug & the future mix of one and two(already finish first) parity bug
	gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
	gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
	gFREG[FCTLL_ZIP_CFG].L &= (~L4K_BADR_CONTINUOUS_MODE);
	gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);
	gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
	gFREG[FCTLL_DMA_CFG].L &= (SEL_OTHER_FSA & ALU0_GROUP_SEL);
	gFREG[FCTLL_FSA_SEL].L  = gubFSAIndex[ubChannel];
	gFREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(FQ->ubALUIndex);


	FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;

	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}

	// retry read from flash to fla buffer
	FSA_t * pFSA = (FSA_t *)gulFSABase[ubChannel];
	UBYTE ubDepth;
	UBYTE ubRetry4K_IDX = 0;
	UBYTE ubHBRetryOK = 0;
	volatile L4KTable16B *pL4KTable;
	UBYTE ubFrameNum = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum;

	// ByPass read all without retry sequence to get UNC map
	gulReadFail_4KMAP = 0;
	UBYTE ubL4KNUM = ubFrameNum;
	U32 ulRetryZIPMAP = 0;
	while (ubRetry4K_IDX < ubFrameNum) {
		ulRetryZIPMAP |= (0xFF << ((ubStartFrameNo + ubRetry4K_IDX) * 8));
		gulReadFail_4KMAP |= (1 << (ubStartFrameNo + ubRetry4K_IDX)); // init fail 4K map
		ubRetry4K_IDX++;
	}
	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubL4KNUM);
	gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME);
	gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubFrameNum) << 8);

	M_GetMTDepth(ubDepth);
	gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
	gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
	//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET);
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
	gubINTTable[ubDepth].All = 0;
	gFREG[FCTLL_ZIP_MAP].L = ulRetryZIPMAP;

	//Set FSA
	UBYTE ubBurstBank, ubPhyPlane;
	UBYTE ubD1Unit = Default_D1_Unit;
	UWORD uwFBlock;
	U32 ulFEntry = FQ->ulFEntry ;
	gubReadFailRetryIsSLC = 0;


	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask;
	}
	else {
		ubBurstBank = (ulFEntry  >> gub4kEntrysPerBurstLog) & gubBurstsPerBankMask;
	}
	if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
		uwFBlock = FQ->uwFUnit;
		ubPhyPlane = ubBurstBank;
	}
	else {
		if ((TLC == 0) || (FQ->btD3 || (ubD1Unit == Default_D1_Unit))) {
			uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex, (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
		}
		else {
			uwFBlock = M_RUT2DTo1D(FQ->ubCEIndex, ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (FQ->ulFEntry  / gulFastPage4kEntrysPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);
		}
		if (gubRUTMixPlaneEnable == 0) {
			ubPhyPlane = ubBurstBank;
		}
		else {
			ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
			uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
		}
	}

	UBYTE ubDie = 0;
	if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
		ubDie = (uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ));
		if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
			uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
		}
	}
	M_Check_PseudoCE(FQ->ubCEIndex, uwFBlock);

Mark_ReadRetryLogPerDie:
	VT->gulReadRetryCnt ++;
	VT->gubPhyDieReadFail[(FQ->ubCEIndex * gubSelDieNumber) + ubDie] ++;
	if (VT->gubPhyDieReadFail[(FQ->ubCEIndex * gubSelDieNumber) + ubDie] == 0xFF) {
		UBYTE ubCE;
		for (ubCE = 0; ubCE < gubCENumber; ubCE ++) {
			for (ubDie = 0; ubDie < gubSelDieNumber; ubDie ++) {
				VT->gubPhyDieReadFail[(ubCE * gubSelDieNumber) + ubDie] >>= 7;
			}
		}
		VT->guwPhyDieReadFailShiftTime ++;
	}

	if (!ENABLE_iFSA) {
		if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
				gubReadFailRetryIsSLC = 1;

				pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));

			}
			else {
				pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
			}
		}
		else {
			pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
		}
		pFSA[gubFSAIndex[ubChannel]].uword.W1 = uwFBlock & 0xFFFF;
		pFSA[gubFSAIndex[ubChannel]].B.B4 = (uwFBlock & 0xFF0000) >> 16;
	}
	else {
		if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
				gubReadFailRetryIsSLC = 1;
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
			}
			else {
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage(((U32)(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
			}
		}
		else {
			{
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + (ulFEntry & gub4kEntrysPerPlaneMask));
			}
		}
		guliFSA[0] |= (uwFBlock & 0xFFFF) << 16;

		gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;
		gFREG[FCTLL_IFSA0].L = gMTQ->Depth[FQ->ubDepth].Para0x2C.uliFSA0 = guliFSA[0];
	}
	if (gubReadFailRetryIsSLC) {
		VT->gulSLCReadRetryCnt++;
	}

	UBYTE ubL4KCount = 0;
	for (ubL4KCount = 0; ubL4KCount < ubL4KNUM; ubL4KCount++) {
		pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
		//set L4K table info
		pL4KTable->ulL4K_LCA = 0;
		pL4KTable->Para0x04.ulAll = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubflaBuffer[(ubL4KCount * 4096)])) / 512;
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
	}
	gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_OFFSET;

	// Wait flash interface ready
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0xA_FPU_POL_ReadFailRetry, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}

	// MT preload + FPU trig = Auto clear CE, so it need to set CE again
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}

	if (ENABLE_MULTIPLANE_READ) {
		{
			flaReadFailRetrySendResetCommand(ubDepth);
		}
	}
	flaReadFailRetryTriggerFPU(FQ->ubALUIndex);


#if (Read_Chk_ByHostRead || Read_Chk_ByReadScan)
	gubForceReadMove = 0;
	gubRetryCount_RD = 0xFF;
#endif
	if (flaReadFailRetryCheckReadStatusInFail(ubDepth, FQ->ubALUIndex)) {
#if TLC_NES_TEST_SB_FLOW

		if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
		{
			UBYTE ubi;
			for (ubi = 0 ; ubi < ubL4KNUM ; ubi++) {
				NESGetIBFData(ubi, ubDepth);
				//R0:HB data
				NESSendC9Data(ubi, 1, guwNESDataLen); // mode1 => Z2 buffer
			}
			NESSendC9Data_CMD10();
		}
#endif
		if (!FQ->btThisIsErasePage) {

			gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
			gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(UNC_MAP);

			if (gubEnterReadFailRetryCondition == DQSCNT_MISMATCH) {

				if (gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) {
					ubSignoffmaskFailandUNCatTheSameTime = 1;
					gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
					gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(SIGNOFFMASK_MAP);
					FQ->ulFIPError.W.uwSignoffMaskFailLog = gFREG[FCTLL_MAP_INF].W.W0;
				}

				gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(UNC_MAP);
				gulGetUNCFrameMapInMT = gFREG[FCTLL_MAP_INF].L;
				M_Get_Error_4K_Index(ubFailFrameNoInMT, gub4kEntrysPerPlane, gulGetUNCFrameMapInMT);
			}

			UBYTE ubIsFastPage = (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) ? 1 : 0;


			if (guoFlashUseType.B.btSandisk) {

				UBYTE ubOpenBlock = 0;
				UBYTE ubLastWritePageIndex = 0;
				UBYTE ubErrPageIndex = 0;
				UBYTE ubOpenWLType = 0; //  0LP  1MLC
				/*
				gubSandiskRetryType
				SLC 2 types  0->1
				Close Blk 4 types 2->5
				Open LP 4 types 6->9
				Open MLC 4 types 10->13
				*/
				if (ubIsFastPage) {
					gubSandiskRetryType = 0;
				}
#if TLC_NES_TEST_SB_FLOW
				else if (gulParamPageInfo.JEDEC.btNandSupplyByEmulator && gubNESReadIBF) {
				}
#endif
				else {
					gubSandiskRetryType = 2;
					if (FQ->uwFUnit == VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget) {
						ubLastWritePageIndex = (VT->gulGRTargetPTR - 1) / gubPlanesPerPage;
						if (ubLastWritePageIndex < guwPagesPerUnitMask) {
							ubOpenBlock = 1;
							ubErrPageIndex = FQ->ulFEntry / guw4kEntrysPerPage;
							gubSandiskRetryType = 6;
							if ((ubLastWritePageIndex == 0) || (ubLastWritePageIndex & 1)) { //Last Page is Lower Page
								//(err page 0 or 1) (err page = last lower page)  err page is previous WL's lower page
								if ((ubLastWritePageIndex < 2) || (ubErrPageIndex == ubLastWritePageIndex) || (ubErrPageIndex == (ubLastWritePageIndex - 2))) {
									ubOpenWLType = 0;
								}
								else {
									ubOpenWLType = 1;
								}
							}
							else { //Last Page is Upper Page
								if (ubErrPageIndex == (ubLastWritePageIndex - 1)) {
									ubOpenWLType = 0;
								}
								else {
									ubOpenWLType = 1;
								}
							}
							if (ubOpenWLType) {
								gubSandiskRetryType = 10;
							}
						}
					}
				}
			}
			gulFQEntry_HR = FQ->ulFEntry;
			ubHBRetryOK = flaReadFailHBRetry(HB_RETRY_L4K_TABLE_OFFSET, ubStartFrameNo, ubFrameNum, ubIsFastPage, ubDepth, ubChannel, FQ->ubALUIndex);

#if (ENABLE_RESET_RETRY_LEVEL && (!TLC))
			if ((!guoFlashUseType.B.btSandisk) && (ubHBRetryOK) && (FQ->ubALUIndex != ALU_A2)) {
				gulMLCHBPassCnt++;
			}
#endif
		}
	}
	else if (gubFPUTimeout) {
		VT->gulRetryFPUTimeoutDebugCnt++;
	}
	else {
#if TLC_NES_TEST_SB_FLOW

		if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
		{
			UBYTE ubi;
			for (ubi = 0 ; ubi < ubL4KNUM ; ubi++) {
				NESGetIBFData(ubi, ubDepth);
				//R0:HB data
				NESSendC9Data(ubi, 1, guwNESDataLen); // mode1 => Z2 buffer
			}
			NESSendC9Data_CMD10();
		}
#endif
		if (FQ->btThisIsErasePage) {
			FQ->btThisIsErasePage = 0;
		}
		ubHBRetryOK = 1;
		ubResetNormalReadPass = 1;
#if (!Read_Chk_ByHostRead && !Read_Chk_ByReadScan)
		if (gubReadDisturbCheck && (gubReadDisturbMoveLevel == RDML_RESET_READ)/*等於Reset Read 就搬*/) {
			gubForceReadMove = 1;
		}
#endif
#if ENABLE_RECORD_RETRY_INFO
		VT->gsReadFailDebugCount.gulRetryPassCount[0]++;
#if	(ENABLE_RECORD_D1TOD3_READRETRY && (!BURNER))
		if (FQ->ubD3WriteMode & D3_SUSPEND_EN) {
			VT->gsReadFailDebugCount.gulRetryPassCount[RECORD_D1TOD3_READRETRY_BASE + 0]++;	// D1 to D3 normal read again pass
		}
#endif
#if ((ENABLE_RECORD_SLC_RETRY_INFO==1) && (!BURNER))
		if ( gubReadFailRetryIsSLC ) {
			VT->gsReadFailDebugCount.gulRetryPassCount[RECORD_SLC_READRETRY_BASE + 0]++;
		}
#endif
#endif
	}

	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
	}
	else {
		FCONL[FCONL_FCE_CLR] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Disable selected CE
	}
	volatile REG_t *NonUncChannelFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (((ubChannel + 1) & 1) * 0x200));
	UBYTE ubFailPhase;

	ubFailPhase = 0xFF;	// default設為0xFF, 若至少有HB retry fail才會記錄RUT log

	if (gubFPUTimeout) {
		goto MARK_ReadUNC;
	}

	if ((!ubHBRetryOK) && (!FQ->btThisIsErasePage)) {
	MARK_SoftBitCorrect:
		// Every HB retry read all fail

		ubFailPhase = 1;
		if (ENABLE_SOFTBITCORRECTION && ((!guoFlashUseType.B.btA2Cmd) || ENABLE_DEBUG_FORCE_FASTPAGE_NONA2  || (((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) == 0) /* && (!ENABLE_ULTRAMLC)*/))) {

			gubEnterSB = 1;
			if (!ENABLE_DEBUG_SBC_HANG_ON) {
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
				// check another channel autopol status
				if (((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (ANY_BUSY | FPU_TRIG)) != 0) && (NonUncChannelFREG[FCTLL_INT_RDY].B.B0 != 0xFF)
				        && (!(NonUncChannelFREG[FCTLL_INT_INF].B.B0 & INTR_BUSY))) {
					UBYTE ubAutopolDone = 0;
					U32 ulFCE_ENB;
					U32 ulTimeStart = gulOperationTime;
					while (1) {
						if (NonUncChannelFREG[FCTLL_INT_INF].B.B0 & INTR_BUSY) {
							// polling done
							ubAutopolDone = 1;
							break;
						}
						if (((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (MT_BUSY | ANY_BUSY | SIGNOFF_BUSY | FPU_TRIG)) == 0) && (NonUncChannelFREG[FCTLL_INT_RDY].B.B0 == 0xFF)) {
							// polling done
							ubAutopolDone = 1;
							break;
						}
#if FORCE_AUTOPOL_TIMEOUT
						// Check autopol timeout FPU sequence
						if (NonUncChannelFREG[FCTLL_FPU_ENTRY].L != FPU_PTR_C70_DAT_MK00)
#else
						// Check autopol timeout FPU sequence
						if ((NonUncChannelFREG[FCTLL_FPU_ENTRY].L != FPU_PTR_C70_DAT_MK20) && (NonUncChannelFREG[FCTLL_FPU_ENTRY].L != FPU_PTR_C70_DAT_MK40))
#endif
						{
							// polling done & FPU is changed
							ubAutopolDone = 1;
							break;
						}
						xthal_dcache_line_invalidate(&gulOperationTime);
						if (((gulOperationTime >= ulTimeStart) && ((gulOperationTime - ulTimeStart) > AUTOPOL_TIMEOUT_MILLISEC)) || ((gulOperationTime < ulTimeStart) && ((0xFFFFFFFF - ulTimeStart + gulOperationTime) > AUTOPOL_TIMEOUT_MILLISEC))) {
							//timeout occur... get current CE ENB
							ulFCE_ENB = FCONL[FCONL_FCE_ENB];
							if (ulFCE_ENB != 0) {
								break;
							}
							// get CE ENB timeout
							M_AssertError((gulOperationTime - ulTimeStart) > (2 * AUTOPOL_TIMEOUT_MILLISEC));
						}
					}

					if (ubAutopolDone == 0) {
#if UART_AUTOPOL_TIMEOUT_HANDLING
						UartString("\nMTStop0:");
						UartLongHex(ulFCE_ENB);
#endif
						flaReadFailRetryAutoPollTimeoutHandling(ubgFQLinkIndex, ulFCE_ENB, 2);
					}
				}
#endif
				// abort all channel for SB
				NonUncChannelFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);
				while ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) { // already push MT also sure to done
					if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
						break;
					}
				}

				// 20151006 Wait pre load MT done for no INT in C70 POL bug
				UBYTE ubDummyRead = RSB[RSB_NMODE_IDX_0];
				ubDummyRead = RSB[RSB_NMODE_IDX_1];
				ubDummyRead = RSB[RSB_NMODE_IDX_2];
				ubDummyRead = RSB[RSB_NMODE_IDX_3];
				ubDummyRead = RSB[RSB_NMODE_IDX_4];
				while ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) { // pre load MT also sure to done
					if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
						break;
					}
				}

				// wait the channel true idle when force empty
				NonUncChannelFREG[FCTLL_DBG_INF].L = (0x0C);
				while ((GET_DBG_MTQ_STS(NonUncChannelFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(NonUncChannelFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2)) {
					if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
						break;
					}
				}

				NonUncChannelFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN));  // fix 2nd program parity hang on bug
			}

			if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
			}
			SYS1L[SYS1L_INT_EN3] &= (~(FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
			if (SoftBitCorrect((U32)(&gubflaBuffer[0]), FQ->ubCEIndex, FQ->uwFUnit, FQ->ulFEntry, FQ->uwFJobInfo, gulReadFail_4KMAP, ubDepth)) {
				VT->gulReadSBCnt++; // softbit correct ok
				goto Mark_HBRetryOK;
			}
			else {
				VT->gsReadFailDebugCount.gulSoftBit_Fail_DebugCount++;

				// Fix IBUF parity error bug
				if (ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
					// Reset IBF PTR by FW to solve disable IBF_RING_MODE_EN will cause another channel IBF PTR error bug
					flaReadFailRetryResetIBFPTR(ubChannel, ubDepth);
				}
				else {
					if (ubChannel == 0) {
						FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH0_EN);
					}
					else {
						FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH1_EN);
					}
				}
			}
		}

	MARK_RS_Recover:
		// RS
		ubFailPhase = 2;
		if (FQ->uwFJobInfo & BIT_FJOBI_RS) {

			guwRSPlaneIndex = 0xFFFE; // used by RS_Get_RSPlaneIndex
			if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE/* && gubEnterftlCopyUnit*/) {

				if ((FQ->FQPara.B.btRWTableUnit == 0) && (gubEnterRSNormalMode != 0) && (FQ->uwFUnit == gsRSNormalModeCorBetterMap.uwFUnit)) {
					if (RS_Get_RSGroupTagFailMapIndex(ubgFQLinkIndex) == gsRSNormalModeCorBetterMap.uwRSGroupTag) {
						RS_Get_RSPlaneIndex(ubgFQLinkIndex);
						UBYTE ubi = 0;
						for (ubi = 0; ubi < RS_NORMAL_MODE_MAX_FPUCOR_SIZE; ubi++) {
							if (guwRSPlaneIndex == gsRSNormalModeCorBetterMap.uwRSNormalModeBetterMap[ubi]) {
								if ((!ENABLE_DEBUG_RS_OVERLAP) && (gubEnterSB == 0) && (ubi == 0)) {
									break;
								}
								if ((!ENABLE_DEBUG_RS_OVERLAP) && (gubEnterSB == 1) && (ubi > 0)) {
									break;
								}
								VT->gulRSNormalModeCorrectDataBeforeDebugCnt++;
								gubRSNormalModeCorrectDataBefore = 1;
								gubRSCorrectPageIndex = ubi;
							}
						}
					}
				}

				gubEnterRSNormalMode = 0;
			}

			UBYTE ubRSGroupTagFailMapIsSetUp = 0;
			if (ENABLE_SAVE_RS_RECOVER_TIME_DURING_FTLCOPYUNIT && gubEnterftlCopyUnit) {
				UWORD uwRSGroupTagFailMapIDX = RS_Get_RSGroupTagFailMapIndex(ubgFQLinkIndex);
				if (gubRSGroupTagFailMap[(uwRSGroupTagFailMapIDX / 8)] & (1 << (uwRSGroupTagFailMapIDX % 8))) {
					VT->gulRSGroupTagFailMapSetUpDebugCnt++;
					ubRSGroupTagFailMapIsSetUp = 1;
				}
			}

			if ((!ENABLE_DEBUG_NOT_ENCODE_PARITY) && ENABLE_RS_RECOVER && (ubRSGroupTagFailMapIsSetUp == 0)) {
			MARK_ENTER_RS_Recover:
				if (!ENABLE_DEBUG_RS_DMAC) {
					gubEnterRS = 1;
#if ENABLE_DEBUG_RS_VERIFY_DATA
					gubRSRecoverSpareFail = 0;
#endif
				}
				if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE && gubRSNormalModeCorrectDataBefore) {
					gubRSErrorNum = 0;
				}
				else {
					RS_Recover(ubgFQLinkIndex, ubDepth);
				}
				if (ENABLE_RS_NORMAL_MODE && (gubRSErrorNum > gubDebugMaxRSErrorNum)) {
					gubDebugMaxRSErrorNum = gubRSErrorNum;
				}
				if (ENABLE_DEBUG_UART_NES_RS) {
					UartString("\nAfterRS ErrPlaneNum: ");
					Uart_Tx_DataHex(gubRSErrorNum);
				}
				//if (gubRSErrorNum <= RS_MAX_ERROR_COUNT) {
				if ((gubRSErrorNum <= RS_MAX_ERROR_COUNT) ||
				        (ENABLE_RS_NORMAL_MODE && ((FQ->FQPara.B.btRWTableUnit && gubRSErrorNum <= RS_TABLE_MAX_ERROR_COUNT) || ((FQ->FQPara.B.btRWTableUnit == 0) && (gubRSErrorNum <= RS_MAX_ERROR_COUNT))))) {
					if (flaReadFailRetryFPUCORRecoverData(ubgFQLinkIndex, ubDepth, ubFailFrameNoInMT)) {
#if ENABLE_DEBUG_RS_VERIFY_DATA
						if (gubRSRecoverSpareFail) {
							gulRS_SpareFail_LDPC_TrappingSet_DebugCnt++;
						}
#endif
						gulReadFail_4KMAP = 0;
					}
					else if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE && gubRSNormalModeCorrectDataBefore) {
						gubRSNormalModeCorrectDataBefore = 0;
						gubRSCorrectPageIndex = 0;
						goto MARK_ENTER_RS_Recover;
					}
				}
				else {
					VT->gulRSRecoverFailCnt++;
				}
			}
			else if (gubEnterSB) {
				if (!ENABLE_DEBUG_SBC_HANG_ON) {
					NonUncChannelFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
					NonUncChannelFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);
				}
			}

			M_AddMTDepth(ubDepth);
			if (gulReadFail_4KMAP == 0) {
				VT->gulReadRSCnt++;
				if (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE) {
					ftlRecordFailLog(FLHErrLogType, 0x2, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_NormalErr);
					if (gubMPRW) {
						guoCE_FailMap |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(gFQI.gFQLink[FQ->ubL4kIndex].ubCEIndex)));
						if ( (GetRTT_milis() - gulMPRWCMDInStartTime) > MPRW_CMDTIMEOUT) {
							flaDumpError(0, VT_ASSERT_MPRW_RSFAIL_0x99);
						}
					}
				}
				if (!ENABLE_DEBUG_ALL_RETRY_EXCEPT_UNC) {
					if ((VT->gulFTLState.B.btDoingFlushD1) || ((VT->gulFTLState.B.btNeedRUTReplace == 0) && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE))) {
						flaMarkFail(ubgFQLinkIndex, ubFailPhase, 0);
					}
				}
				if (FQ->btSpareInvalid) {
					// 一開始 UNC, 經由HB, SB or RS救回來後是spareinvalid,
					// user data: 於flaReadFailRetryRecoverRSData回host UNC addr, 這邊下面重推FQ
					// non-user data: 於flaReadSignoffmaskFail處理跟重推FQ
					if (gubEnterSB) {
						SYS1L[SYS1L_INT_STS3] = (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);	//W1C after doing FPU COR
						if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
						}
						SYS1L[SYS1L_INT_EN3] |= (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);
					}
					return;
				}
			}
			else {
				if (ENABLE_SAVE_RS_RECOVER_TIME_DURING_FTLCOPYUNIT && gubEnterftlCopyUnit) {
					UWORD uwRSGroupTagFailMapIDX = RS_Get_RSGroupTagFailMapIndex(ubgFQLinkIndex);
					gubRSGroupTagFailMap[(uwRSGroupTagFailMapIDX / 8)] |= (1 << (uwRSGroupTagFailMapIDX % 8));
				}
				goto MARK_ReadUNC;
			}
		}
		else {
			VT->gsReadFailDebugCount.gulRetry_Fail_NotRS_DebugCount++;
			if (gubEnterSB) {
				if (!ENABLE_DEBUG_SBC_HANG_ON) {
					NonUncChannelFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
					NonUncChannelFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);
				}
			}
			M_AddMTDepth(ubDepth);
			gulReadFail_4KMAP = 0; // init value for ReadVerify (matt)
			if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE/* && gubEnterftlCopyUnit*/) {
				if (!ENABLE_DEBUG_RS_OVERLAP) {
					gubEnterRSNormalMode = 0; // HB fail, SB fail, no RS
				}
			}

		MARK_ReadUNC:
			if (ENABLE_DEBUG_UART_FAIL) {
				UartString(" UNC");
				Uart_Tx_DataHex(FQ->ubCEIndex);
			}
			if (ENABLE_DEBUG_UART_NES_RS && ENABLE_DEBUG_NES_READ_FAIL) {
				UartString(" debug_UNC3");
			}
			if (ENABLE_DEBUG_NES_READ_FAIL && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE)) {
				flaLEDBlink(1);
			}
#if ENABLE_TABLE_VERIFY
			if (((((VT->gulFTLState.B.btDoingFlushD1) || (VT->gulFTLState.B.btNeedRUTReplace == 0)) && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE)) || (FQ->btTableVerify)) && (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR || (gubForceGenCRC32 == 0)))
#else
			if (((VT->gulFTLState.B.btDoingFlushD1) || (VT->gulFTLState.B.btNeedRUTReplace == 0)) && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE) && (!ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR || (gubForceGenCRC32 == 0)))
#endif
			{
				ubFailPhase = 0;
				if (!ENABLE_DEBUG_ALL_RETRY_EXCEPT_UNC) {
#if TLC_NES_TEST_SB_FLOW

					if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
					{

					}
					else
#endif
					{
						flaMarkFail(ubgFQLinkIndex, ubFailPhase, 0);
					}
				}
				else {
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(1);
					}
				}
			}

			if (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE) {
				VT->gulTotalReadUNCCnt++;
				VT->gulReadUNCCnt_D3++;

				ftlRecordFailLog(FLHErrLogType, 0x3, FQ->ubL4kIndex, ((gubEnterReadFailRetryCondition == DQSCNT_MISMATCH) ? BYTE_DQS_MISSMATCH : DONTCARE), ((gubEnterReadFailRetryCondition == UNC_MAP) ? DONTCARE : gubEnterReadFailRetryCondition), BIT_NormalErr);

				if (gubMPRW) {
					guoCE_FailMap |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(gFQI.gFQLink[FQ->ubL4kIndex].ubCEIndex)));
					if ( (GetRTT_milis() - gulMPRWCMDInStartTime) > MPRW_CMDTIMEOUT) {
						flaDumpError(0, VT_ASSERT_MPRW_READFAIL_0x98);
					}
				}

			}
			if (gubEnterReadFailRetryCondition == UNC_MAP) {
				VT->gsReadFailDebugCount.gulReadUNC_DebugCount++;
			}
			else if (gubEnterReadFailRetryCondition == ERASE_PAGE_ERR) {
				VT->gulReadRetryErasePageRSFailCnt++;
				if (ENABLE_WP_DEBUG && (FQ->btCheckEmpty == 0)) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(FQ->btCheckEmpty == 0);
				}
			}
			else if ((gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) || (gubEnterReadFailRetryCondition == CRC32_ERR_MAP) || (gubEnterReadFailRetryCondition == FW_SETTING_ERR_MAP)) {
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(1);
				}
				if (gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) {
					VT->gulLCA_Retry_FailDebugCount++;
				}
			}
			FQ->btReadUNC = 1;

			if (ENABLE_WP_DEBUG && FQ->FQPara.B.btRWTableUnit) {
				flaLEDBlink(0xBB);
			}
#if ENABLE_TABLE_VERIFY
			else if (FQ->btTableVerify) {
				VT->guwVerifyFailCNT++;
			}
#endif
			else {
				if (FQ->FQPara.B.btRWTableUnit) {
					ftlRecordFailLog(FLHErrLogType, 0x3, FQ->ubL4kIndex, BYTE_UNC_READTABLE, FQ->FQPara.B.btRWTableUnit, BIT_NormalErr);
				}

				if ((FQ->uwFUnit == VT->guwTableTarget[VT->gubTableTargetIndex].B.uwTarget) && (VT->gulTableUNCResetCount < TABLE_UNC_RESET_THRESHOLD)) {
					VT->gulTableUNCResetCount++;
					M_AssertResetError(FQ->FQPara.B.btRWTableUnit, VT_ASSERT_TABLEUNIT_READUNC_0x6F);
				}
				else if ((FQ->FQPara.B.btRWTableUnit) && ((FQ->uwFUnit != VT->guwTableTarget[VT->gubTableTargetIndex].B.uwTarget))) {
					UBYTE ubi = 0;
					for (ubi = 0; ubi < gubTableUnitNum; ubi++) {
						if ((FQ->uwFUnit == VT->guwTableTarget[ubi].B.uwTarget)) {
							M_AssertCriticalError((VT->gulTableUNCBitMap & (BIT0 << ubi)), VT_ASSERT_TABLEUNIT_READUNC_0x6F);
							VT->gulTableUNCBitMap |= (BIT0 << ubi);
							M_AssertResetError(FQ->FQPara.B.btRWTableUnit, VT_ASSERT_TABLEUNIT_READUNC_0x6F);
						}
					}
				}
				else {
					M_AssertCriticalError(FQ->FQPara.B.btRWTableUnit, VT_ASSERT_TABLEUNIT_READUNC_0x6F);
				}
			}

			if (FQ->FQPara.B.btUserData || (VT->gulFTLState.B.btBG_Copy && gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btBufModeEn)) {
				UBYTE ubFailL4KNo = 0;
				if (ubSignoffmaskFailandUNCatTheSameTime) {
					if (ENABLE_WP_DEBUG && FQ->btSpareInvalid) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(FQ->btSpareInvalid);
					}
					ubFailL4KNo = flaReadFailRetryCheckFirstReadFailL4KDuetoUNCorSignoffMaskFail(ubgFQLinkIndex, ubFailFrameNoInMT);
				}
				else {
					ubFailL4KNo = flaReadFailRetryGetL4KFailNo(ubgFQLinkIndex, ubFailFrameNoInMT);
				}

				volatile L4KTable16B * pL4K_TABLE;
				UBYTE ubi = 0;
				U32 ulAddr = 0;
				UBYTE ubL4KIndex = 0;
				volatile L4KTable16B * pDUMMYL4K_BUFMASK_TABLE;
				if (FQ->btSpareInvalid) {
					// Is signoffmask fail
					UBYTE ubCheckRealSignoffMaskFail = 0;
					for (ubL4KIndex = ubFailL4KNo; ubL4KIndex < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubL4KIndex++) {
						pDUMMYL4K_BUFMASK_TABLE = pL4K_TABLE = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KIndex) * Spare4kSize));
						if (gubFPUCORClearBufMask) {
							pDUMMYL4K_BUFMASK_TABLE = (volatile L4KTable16B *) (IRAM_BVCI_BASE + DUMMY_L4K_TABLE_OFFSET + (ubL4KIndex * Spare4kSize));
						}
						ulAddr = (pL4K_TABLE->Para0x0C.BitMap.ulL4K_BADR * 0x200);


						for (ubi = 0; ubi < 8; ubi++) {
							if (pDUMMYL4K_BUFMASK_TABLE->Para0x08.BitMap.ubL4K_BUF_MASK & (BIT0 << ubi)) {
								if (gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) {
									break;
								}
								else if ((pL4K_TABLE->Para0x04.BitMap.ubL4K_SPRV & (BIT0 << ubi)) == 0) {
									break;
								}
							}
						}

						if (ubi == 8) {
							continue;
						}

						if (ENABLE_DEBUG_UART_FAIL) {
							UartString("\nSignoffFailID ");
							Uart_Tx_DataHex(ubgFQLinkIndex);
							UartString(" LCA ");
							UartLongHex(pL4K_TABLE->ulL4K_LCA);
							UartString(" ProgramFrom ");
							Uart_Tx_DataHex(pL4K_TABLE->Para0x08.BitMap.ubL4K_FW2);
						}

						if (FQ->btSpareInvalid == 0) {
							VT->gulSignOffFail++;
						}

						ubCheckRealSignoffMaskFail = 1;
						FQ->btSpareInvalid = 1;

						if (FQ->FQPara.B.btUserData) {
							U32 ulFailLBA =  (pL4K_TABLE->ulL4K_LCA + ubi);
							ulAddr += ubi * 0x200;
							flaReadFailRetrySetUNCAddr(ulAddr, ulFailLBA);
						}
						else if (VT->gulFTLState.B.btBG_Copy && gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btBufModeEn) { // BG Read FQ will read data to write buffer
							ulAddr = (pL4K_TABLE->Para0x0C.BitMap.ulL4K_BADR - BUFFER2_WRITE_START_INDEX) >> SectorsPer4KLog; // write buffer 4k index
							gulBG_ReadFail4k_BitMap |= BIT0 << (ulAddr & 31);
							if (ENABLE_HANDLE_BG_READ_FAIL == 0) {
								gubBG_Read_UNC = 1;
							}
						}
					}

					if (!ubCheckRealSignoffMaskFail) {
						if (ENABLE_WP_DEBUG) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(1);
						}
						FQ->btSpareInvalid = 0;
					}
				}
				else {
					// UNC, LCA, De-compression error etc...
					for (ubL4KIndex = ubFailL4KNo; ubL4KIndex < gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubL4KNum; ubL4KIndex++) {
						pDUMMYL4K_BUFMASK_TABLE = pL4K_TABLE = (volatile L4KTable16B *) (L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KIndex) * Spare4kSize));
						if (gubFPUCORClearBufMask) {
							pDUMMYL4K_BUFMASK_TABLE = (volatile L4KTable16B *) (IRAM_BVCI_BASE + DUMMY_L4K_TABLE_OFFSET + (ubL4KIndex * Spare4kSize));
						}

						for (ubi = 0; ubi < 8; ubi++) {
							if (pDUMMYL4K_BUFMASK_TABLE->Para0x08.BitMap.ubL4K_BUF_MASK & (BIT0 << ubi)) {
								if (gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) {
									break;
								}
								else if ((pL4K_TABLE->Para0x04.BitMap.ubL4K_SPRV & (BIT0 << ubi)) == 0) {
									break;
								}
							}
						}

						if (ubi == 8) {
							continue;
						}

						if (FQ->FQPara.B.btUserData) {
							U32 ulFailLBA =  (pL4K_TABLE->ulL4K_LCA + ubi);
							ulAddr = (pL4K_TABLE->Para0x0C.BitMap.ulL4K_BADR * 0x200);
							ulAddr += ubi * 0x200;
							flaReadFailRetrySetUNCAddr(ulAddr, ulFailLBA);
						}
						else if (VT->gulFTLState.B.btBG_Copy && gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.btBufModeEn) { // BG Read FQ will read data to write buffer
							ulAddr = (pL4K_TABLE->Para0x0C.BitMap.ulL4K_BADR - BUFFER2_WRITE_START_INDEX) >> SectorsPer4KLog; // write buffer 4k index
							gulBG_ReadFail4k_BitMap |= BIT0 << (ulAddr & 31);
							if (ENABLE_HANDLE_BG_READ_FAIL == 0) {
								gubBG_Read_UNC = 1;
							}
						}
					}
				}
			}
		}
	}
	else if (ubHBRetryOK) {
	Mark_HBRetryOK:
		if (ENABLE_WP_DEBUG && (((gFREG[FCTLL_IBF_CTL].L & LDPC_COR_MAP(0x1F)) == 0) || ((gFREG[FCTLL_OTHER_SET].L & BCH_COR_EN) == 0))) {
			flaLEDBlink(1);
		}
		else {
			M_AssertError((gFREG[FCTLL_IBF_CTL].L & LDPC_COR_MAP(0x1F)) == 0);
			M_AssertError((gFREG[FCTLL_OTHER_SET].L & BCH_COR_EN) == 0);
		}

		if (ENABLE_DEBUG_BYPASS_GEN_ZIPHEADER && ((gubEnterReadFailRetryCondition == LCA_FW_CMP_FAIL_MAP) || (gubEnterReadFailRetryCondition == CRC32_ERR_MAP) || (gubEnterReadFailRetryCondition == FW_SETTING_ERR_MAP))) {

			if (ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
				// Reset IBF PTR by FW to solve disable IBF_RING_MODE_EN will cause another channel IBF PTR error bug
				flaReadFailRetryResetIBFPTR(ubChannel, ubDepth);
			}
			else {
				if (ubChannel == 0) {
					FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH0_EN);
				}
				else {
					FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH1_EN);
				}
			}

			// Restore FPU
			gFREG[FCTLL_CHNL_SET].L &= (~BYPASS_CONV);
			gFREG[FCTLL_BACK_STORE].W.W0 = (HB_RETRY_L4K_TABLE_OFFSET);
			if (gubEnterRS) {
				gFREG[FCTLL_RAW_DMA_ADR].L = (U32)(CorrectRSDataStart + (ubStartFrameNo * 4096));
			}
			else {
				gFREG[FCTLL_RAW_DMA_ADR].L = ((U32)(&gubflaBuffer[0]));
			}
			//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
			gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
			IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(1, ubFrameNum, 1, 1);
			gFREG[FCTLL_FPU_ENTRY].L  = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

			flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

			// Gen header from FPU_COR
			if (!ENABLE_iFSA) {
				gFREG[FCTLL_FSA_SEL].L  = FQ->ubFSAIndex;
			}
			else {
				gFREG[FCTLL_IFSA0].L = gMTQ->Depth[FQ->ubDepth].Para0x2C.uliFSA0;
			}

			gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
			gFREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(FQ->ubALUIndex);
			gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
			gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubL4KNUM);
			gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME);
			gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubFrameNum) << 8);
			gFREG[FCTLL_ZIP_MAP].L = ulRetryZIPMAP;

			for (ubL4KCount = 0; ubL4KCount < ubL4KNUM; ubL4KCount++) {
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + TEMP_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)(&gubflaBuffer[16384])) / 512;
				pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			}
			gFREG[FCTLL_L4K_SPR_ADR].L = TEMP_L4K_TABLE_OFFSET;

			// FPU COR
			//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
			gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
			gFREG[FCTLL_IBF_CTL].L &= CLR_LDPC_COR_MAP;
			gFREG[FCTLL_OTHER_SET].L &= (~BCH_COR_EN);

			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_IBF_CORRECT;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

			flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

			gFREG[FCTLL_IBF_CTL].L |= LDPC_COR_MAP(0x1F);
			gFREG[FCTLL_OTHER_SET].L |= (BCH_COR_EN);
			if (!ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
				if (ubChannel == 0) {
					FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH0_EN);
				}
				else {
					FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH1_EN);
				}
			}
		}

		if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE/* && gubEnterftlCopyUnit*/) {
			if (!ENABLE_DEBUG_RS_OVERLAP) {
				gubEnterRSNormalMode = 0; // data recover by HB or SB, we need re-init
			}
		}
		if (flaReadFailRetryFPUCORRecoverData(ubgFQLinkIndex, ubDepth, ubFailFrameNoInMT)) {
			// FPUCOR PASS
			if (gubEnterSB && (gubEnterRS == 0)) {
				if (!ENABLE_DEBUG_SBC_HANG_ON) {
					NonUncChannelFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
					NonUncChannelFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);
				}
				// SB Pass 可以不用記 Fail Log
				//if ((gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE) && (FQ->btRecordFailLogByMyself == 0)) {	//BIT_FJOBI_WAITRESULT的在ftl later記
				//	ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_NormalErr);
				//}
			}
			else if (gubForceReadMove) {
				if (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE) {	//Read Distrub
					ftlRecordFailLog(FLHErrLogType, VT_READ_DISTURB_MOVE_0x82, FQ->ubL4kIndex, DONTCARE, DONTCARE, BIT_NormalErr);
				}
			}
			if (!ENABLE_DEBUG_ALL_RETRY_EXCEPT_UNC) {

#if ENABLE_TABLE_VERIFY
				if ((FQ->FQPara.B.btRWTableUnit || ((FQ->FQPara.B.btRWTableUnit == 0) && (ubFailPhase != 0xFF)) || gubForceReadMove ) && (FQ->btTableVerify)) {
					ftlRecordFailLog(FLHErrLogType, VT_TABLE_VERIFY_RETRY_0x9C, FQ->ubL4kIndex, BYTE_UNC_READTABLE, DONTCARE, BIT_NormalErr);
				}
				else if ((FQ->FQPara.B.btRWTableUnit || ((FQ->FQPara.B.btRWTableUnit == 0) && (ubFailPhase != 0xFF))
				          || gubForceReadMove /*Read Disturb Force Read Move*/
				         )
				         && ((VT->gulFTLState.B.btDoingFlushD1) || ((VT->gulFTLState.B.btNeedRUTReplace == 0) && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE))) )
#else
				if ((FQ->FQPara.B.btRWTableUnit || ((FQ->FQPara.B.btRWTableUnit == 0) && (ubFailPhase != 0xFF))
				        || gubForceReadMove /*Read Disturb Force Read Move*/
				    )
				        && ((VT->gulFTLState.B.btDoingFlushD1) || ((VT->gulFTLState.B.btNeedRUTReplace == 0) && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE))) )
#endif
				{
#if TLC_NES_TEST_SB_FLOW

					if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
					{

					}
					else
#endif
					{
						flaMarkFail(ubgFQLinkIndex, ubFailPhase, 0);	// TableUnit只要進 HB就算read move, DataUnit進到SB才算read move
					}
				}
				if ((FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) == 0) {	// for initflash時發生read fail
					if ((FQ->uwFUnit < VTAREA_UNITS) || (FQ->uwFUnit == VT->guwVTChildTarget.B.uwTarget) || (FQ->uwFUnit == VT->guwInitInfoTarget.B.uwTarget)) {
						FQ->btReadMove = 1;
					}
				}
			}

			gulReadFail_4KMAP = 0;
			M_AddMTDepth(ubDepth);
		}
		else {
			VT->gulFPUCorrectFailCnt++;
			if (gubEnterSB == 0) {
				goto MARK_SoftBitCorrect;
			}
			else if (gubEnterRS == 0) {
				goto MARK_RS_Recover;
			}
		}

		if (FQ->btSpareInvalid) {
			// 一開始 UNC, 經由HB, SB or RS救回來後是spareinvalid,
			// user data: 於flaReadFailRetryRestoreMTSettingAndFPUCOR回host UNC addr, 這邊下面重推FQ
			// non-user data: 於flaReadSignoffmaskFail處理跟重推FQ
			if (gubEnterSB) {
				SYS1L[SYS1L_INT_STS3] = (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);	//W1C after doing FPU COR
				if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
				}
				SYS1L[SYS1L_INT_EN3] |= (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);
			}
			return;
		}
	}
	else {
		// erase page
		gulReadFail_4KMAP = 0;
		M_AddMTDepth(ubDepth);
	}

	if (gubEnterSB) {
		SYS1L[SYS1L_INT_STS3] = (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);	//W1C after doing FPU COR
		if (ENABLE_WP_DEBUG && (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2))) {
			flaLEDBlink(1);
		}
		else {
			M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
		}
		SYS1L[SYS1L_INT_EN3] |= (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);
	}

	// Clear all MTQ in channel
	gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
	gFREG[FCTLL_MT_TRIG].B.B1 = 0;

	if (!ENABLE_DEBUG_RESUME_STA_ERROR_BUG) {
		flaReadFailRetryForceIRDYtoReady(ubgFQLinkIndex);
	}

	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= ABORT_DONE_INTR_EN;

	gFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
	gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);

	if (Failed_Test_UNC || ENABLE_DEBUG_HW_FORCEREADFAIL) {
		gFREG[FCTLL_UNC_CFG].L = 0;
		gMTQ->Depth[FQ->ubDepth].Para0x00.BitMap.btForceReadFail = 0;
	}

	if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
		gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
	}

	gubTimeoutEnterRebuildFQ = gubFPUTimeout;
	// UDMA target must redo whether it's error or non-error, and we don't want to change the order of FQ, so we redo all those FQ in same channel
	flaReadFailRetryRebuildFQPhase(ubgFQLinkIndex);
	gubTimeoutEnterRebuildFQ = 0;

#if ((TLC_BICS2 || (!TLC) || Hynix3DV6 || YMTC) && (!BURNER) && (!UFS))
	if (guoFlashUseType.B.btToshiba || guoFlashDefaultType.B.btHynix || guoFlashUseType.B.btYMTC) {

		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;

		if (UFS || gubGPIO_CE_Decoder) {
			FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
		}
		else {
			FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
		}
		UBYTE ubdie = GET_LUN_ADDR(guliFSA[0]);
		if ((guoFlashUseType.B.btToshibaBiCs2 || guoFlashUseType.B.btToshibaBiCs3 || guoFlashUseType.B.btToshibaBiCs4) && TLC_BICS2
		        && (!FQ->btThisIsErasePage) && (gubSSD_Init_State.B.ubState > 0) && (!ubResetNormalReadPass)) {
		}

		else if ((!TLC) && (!guoFlashUseType.B.btSandisk) && (!FQ->btThisIsErasePage) && (gubSSD_Init_State.B.ubState > 0) && (!ubResetNormalReadPass)) {

			if (FQ->ubALUIndex != ALU_A2) {

				if (gubEnterReadFailRetryCondition != DQSCNT_MISMATCH) {

					UWORD uwPageIndex = (M_Divider(FQ->ulFEntry, DEF_4KEntryPerPage_Mcl_Div));

					UBYTE ubPageSel, ubRetryGroup;

					if ((uwPageIndex % 2) == 0) {
						if (uwPageIndex != 0) {
							ubPageSel = UPPER_PAGE;
						}
						else {
							ubPageSel = LOWER_PAGE;
						}
					}
					else {
						if (uwPageIndex != guwPagesPerUnitMask) {
							ubPageSel = LOWER_PAGE;
						}
						else {
							ubPageSel = UPPER_PAGE;
						}
					}

					if (ubResetNormalReadPass) {
						ubRetryGroup = 0;
					}
					else {
						ubRetryGroup = gHBRetryQI[ubPageSel - 1].HBRetryQueue[gHBRetryQI[ubPageSel - 1].ubLinkFirst].ubRetryIndex;
					}

					gubD3RetryCount_HR = 0;
					flaReadFailRetryToshibaFlow(ubRetryGroup, BIT_FJOBI_HR_SETINITIAL);

#if ENABLE_RESET_RETRY_LEVEL
					gulKeepRetryLevelCE_BITMAP |= (BIT0 << ubCEIndex);
#endif
				}
				else {
#if ENABLE_RESET_RETRY_LEVEL
					gulKeepRetryLevelCE_BITMAP &= ~(BIT0 << ubCEIndex);
#endif
				}
			}

		}
	}
#endif


#if DUMP_READ_FAIL_RETRY_TIME==1
	if ( ubEnterBunchRSRecover ) {
		ulReadFailRetryTimeRecord[1] = gulOperationTime;
		UartString("\nRT: ");
		UartLongHex( (ulReadFailRetryTimeRecord[1] - ulReadFailRetryTimeRecord[0]) );
	}
#endif

#endif
#endif
}

void SoftBitCorrectSetDSPParam(UBYTE ubPageSel)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	FCONL[FCONL_LDPC_CFG] &= CLR_PAGE_SELECTION;
	{
		switch (ubPageSel) {
			case LOWER_PAGE:
				//Set gray code
				FCONL[FCONL_DSP_GRAY_CODE_0] = 0x33ADF3BF;
				FCONL[FCONL_DSP_GRAY_CODE_1] = 0xF8CE95BE;
				FCONL[FCONL_DSP_GRAY_CODE_2] = 0xA12A5B1A;
				FCONL[FCONL_DSP_GRAY_CODE_3] = 0x4008C853;
				FCONL[FCONL_DSP_GRAY_CODE_4] = 0x84ADE718;

				//Set LUT
				FCONL[FCONL_DSP_LUT_0] = 0xC300294D;
				FCONL[FCONL_DSP_LUT_1] = 0x18C30018;
				FCONL[FCONL_DSP_LUT_2] = 0x00294D00;
				FCONL[FCONL_DSP_LUT_3] = 0xC300294D;
				FCONL[FCONL_DSP_LUT_4] = 0x1CE10018;
				FCONL[FCONL_DSP_LUT_5] = 0x002B5F00;

				//Set param
				FCONL[FCONL_DSP_PARAM_0] = 0x00008418;
				FCONL[FCONL_DSP_PARAM_1] = 0x02012000;
				break;
			case MIDDLE_PAGE:
				FCONL[FCONL_LDPC_CFG] |= PAGE_SELECTION_MIDDLE;
				//Set gray code
				FCONL[FCONL_DSP_GRAY_CODE_0] = 0x719D6F9D;
				FCONL[FCONL_DSP_GRAY_CODE_1] = 0x952D8DA5;
				FCONL[FCONL_DSP_GRAY_CODE_2] = 0x5A502150;
				FCONL[FCONL_DSP_GRAY_CODE_3] = 0x61008CEF;
				FCONL[FCONL_DSP_GRAY_CODE_4] = 0x8CEFF79C;
				//Set LUT
				FCONL[FCONL_DSP_LUT_0] = 0xC300294D;
				FCONL[FCONL_DSP_LUT_1] = 0x18C30018;
				FCONL[FCONL_DSP_LUT_2] = 0x00294D00;
				FCONL[FCONL_DSP_LUT_3] = 0xC300294D;
				FCONL[FCONL_DSP_LUT_4] = 0x18C30018;
				FCONL[FCONL_DSP_LUT_5] = 0x00294D00;
				//Set param
				FCONL[FCONL_DSP_PARAM_0] = 0x00008418;
				FCONL[FCONL_DSP_PARAM_1] = 0x04022080;
				break;
			case UPPER_PAGE:
				FCONL[FCONL_LDPC_CFG] |= PAGE_SELECTION_UPPER;
				//Set gray code
				FCONL[FCONL_DSP_GRAY_CODE_0] = 0x719D6F9D;
				FCONL[FCONL_DSP_GRAY_CODE_1] = 0x952D8DA5;
				FCONL[FCONL_DSP_GRAY_CODE_2] = 0x5A502150;
				FCONL[FCONL_DSP_GRAY_CODE_3] = 0x61008CEF;
				FCONL[FCONL_DSP_GRAY_CODE_4] = 0x8CEFF79C;
				//Set LUT
				FCONL[FCONL_DSP_LUT_0] = 0xC300294D;
				FCONL[FCONL_DSP_LUT_1] = 0x18C30018;
				FCONL[FCONL_DSP_LUT_2] = 0x00294D00;
				FCONL[FCONL_DSP_LUT_3] = 0xB100252F;
				FCONL[FCONL_DSP_LUT_4] = 0x1AD10016;
				FCONL[FCONL_DSP_LUT_5] = 0x00231F00;
				//Set param
				FCONL[FCONL_DSP_PARAM_0] = 0x00008418;
				FCONL[FCONL_DSP_PARAM_1] = 0x04022080;
				break;
		}
	}
}

void SoftBitCorrectSetLLRTable(U32 LLRTablePtr, UBYTE ubPageSel) {
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	UBYTE ubIndex;
	U32 ulLLRAdr;
	ulLLRAdr = LLRTablePtr;
	switch (ubPageSel) {
		case LOWER_PAGE:
			ulLLRAdr += LLR_TABLE_LOWER_PAGE_OFFSET;
			break;
		case MIDDLE_PAGE:
			ulLLRAdr += LLR_TABLE_MIDDLE_PAGE_OFFSET;
			break;
		case UPPER_PAGE:
			ulLLRAdr += LLR_TABLE_UPPER_PAGE_OFFSET;
			break;
		default:
			//Uart_Printf("\n\rPage selection is error.");
			while (1);
	}
	U32 * pLLRTABLE = (U32 *) (ulLLRAdr);
	for (ubIndex = 0; ubIndex < 8; ubIndex++) {
		FCONL[FCONL_LLR_TABLE_0 + ubIndex] = pLLRTABLE[ubIndex];
	}
}

void SoftBitCorrectToshibaFlow(UBYTE ubSoftBitReadData, UBYTE ubPageSel, UBYTE ubPreCondition) {
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	U32 ulTemp = gFREG[FCTLL_FLH_SET].L;
	UBYTE ubToggleModeTemp = gubToggleMode;
	if (!ENABLE_DEBUG_RETRY_NOT_SWITCH_LEGACY) {
		if (!guoFlashUseType.B.btSandisk) {
			gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
			gubToggleMode = 0;
		}
	}

	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_Asyn40, 6);
			//Fla_Set_Offset_DQS_Delay(0);

			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | gubClockDivValue);
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
			}
		}
	}

	if ((!guoFlashUseType.B.btSandisk) && ((ubPageSel == UPPER_PAGE && ubSoftBitReadData == SOFTBIT_READ_LI_DATA) || (ubPageSel != UPPER_PAGE && ubSoftBitReadData == SOFTBIT_READ_HB_DATA) || ubPreCondition)) {
		//Pre Conditon
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x5C;
		gFREG[FCTLL_PIO_CMD].B.B0 = 0xC5;
	}

	// No need implement A2 in SB flow
	if (guoFlashUseType.B.btToshiba19nm) {
		// 19nm, ref: ??
		if (ENABLE_WP_DEBUG) {
			flaLEDBlink(0xFD);
		}
		else {
			gubWP_DEBUG = 0xFD;
			M_AssertError(1);
		}
	}
	else if (guoFlashUseType.B.btToshiba1Ynm || guoFlashUseType.B.btToshiba1Znm) {
		UBYTE ubRetryData[4] = {0, 0, 0, 0};
		if (guoFlashUseType.B.btToshiba1Ynm) {
			// A19nm, ref: Soft Bit Read for Toshiba 1X_1Y_1Z MLC.pptx
			if (ubSoftBitReadData == SOFTBIT_READ_HB_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						//gubVoltage_B = 0;
						ubRetryData[1] = ((0x64 + (gubCenterRank[0] * 8)) & 127); //gubDSP_Center_Table[gubCenterRank[0]];
						ubRetryData[3] = ((0x64 + (gubCenterRank[1] * 8)) & 127); //gubDSP_Center_Table[gubCenterRank[1]];
					}
					else {
						ubRetryData[2] = ((0x64 + (gubCenterRank[0] * 8)) & 127);
					}
				}
				/*else {
					// all zeros
				}*/
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_PLUS_1_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = ((0x6C + (gubCenterRank[0] * 8)) & 127);
						ubRetryData[3] = ((0x6C + (gubCenterRank[1] * 8)) & 127);
					}
					else {
						ubRetryData[2] = ((0x6C + (gubCenterRank[0] * 8)) & 127);
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x08;
						ubRetryData[3] = 0x08;
					}
					else {
						ubRetryData[2] = 0x08;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_SUB_1_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = ((0x5C + (gubCenterRank[0] * 8)) & 127);
						ubRetryData[3] = ((0x5C + (gubCenterRank[1] * 8)) & 127);
					}
					else {
						ubRetryData[2] = ((0x5C + (gubCenterRank[0] * 8)) & 127);
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x78;
						ubRetryData[3] = 0x78;
					}
					else {
						ubRetryData[2] = 0x78;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_PLUS_2_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = ((0x74 + (gubCenterRank[0] * 8)) & 127);
						ubRetryData[3] = ((0x74 + (gubCenterRank[1] * 8)) & 127);
					}
					else {
						ubRetryData[2] = ((0x74 + (gubCenterRank[0] * 8)) & 127);
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x10;
						ubRetryData[3] = 0x10;
					}
					else {
						ubRetryData[2] = 0x10;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_SUB_2_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = ((0x54 + (gubCenterRank[0] * 8)) & 127);
						ubRetryData[3] = ((0x54 + (gubCenterRank[1] * 8)) & 127);
					}
					else {
						ubRetryData[2] = ((0x54 + (gubCenterRank[0] * 8)) & 127);
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x70;
						ubRetryData[3] = 0x70;
					}
					else {
						ubRetryData[2] = 0x70;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_PLUS_3_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = ((0x7C + (gubCenterRank[0] * 8)) & 127);
						ubRetryData[3] = ((0x7C + (gubCenterRank[1] * 8)) & 127);
					}
					else {
						ubRetryData[2] = ((0x7C + (gubCenterRank[0] * 8)) & 127);
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x18;
						ubRetryData[3] = 0x18;
					}
					else {
						ubRetryData[2] = 0x18;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_SUB_3_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = ((0x4C + (gubCenterRank[0] * 8)) & 127);
						ubRetryData[3] = ((0x4C + (gubCenterRank[1] * 8)) & 127);
					}
					else {
						ubRetryData[2] = ((0x4C + (gubCenterRank[0] * 8)) & 127);
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x68;
						ubRetryData[3] = 0x68;
					}
					else {
						ubRetryData[2] = 0x68;
					}
				}
			}
			/*
			else if (ubSoftBitReadData == SOFTBIT_READ_LI_DATA) {
				// all zeros
			}
			*/
		}
		else if (guoFlashUseType.B.btToshiba1Znm) {
			// 15nm, ref: Soft-Bit Mapping and Shift Value of Soft-Bit Read for TSB 1Z TLC, 1Z MLC, A19 MLC, and 3D MLC_v2.pdf
			UBYTE ubLowerShiftValueTable[8] = {
				0x00, 0x74, 0x7A, 0x7E , 0x02, 0x06, 0x0C, 0x00
			};
			UBYTE ubUpperShiftValueTable1[8] = {
				0x00, 0x74, 0x7A, 0x7E, 0x02, 0x06, 0x0C, 0x00
			};
			UBYTE ubUpperShiftValueTable2[8] = {
				0x00, 0x74, 0x7A, 0x7E, 0x02, 0x06, 0x0C, 0x00
			};
			if (ubSoftBitReadData == SOFTBIT_READ_HB_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						if (ENABLE_WP_DEBUG && ((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7))) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7));
						}
						ubRetryData[1] = ubUpperShiftValueTable1[gubCenterRank[0]];
						ubRetryData[3] = ubUpperShiftValueTable2[gubCenterRank[1]];
					}
					else {
						if (ENABLE_WP_DEBUG && (gubCenterRank[0] > 7)) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(gubCenterRank[0] > 7);
						}
						ubRetryData[2] = ubLowerShiftValueTable[gubCenterRank[0]];//gubDSP_Center_Table[gubCenterRank[0]];
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_PLUS_1_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						if (ENABLE_WP_DEBUG && ((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7))) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7));
						}
						ubRetryData[1] =  (0x04 + ubUpperShiftValueTable1[gubCenterRank[0]]) & 0x7F;
						ubRetryData[3] =  (0x04 + ubUpperShiftValueTable2[gubCenterRank[1]]) & 0x7F;
					}
					else {
						if (ENABLE_WP_DEBUG && (gubCenterRank[0] > 7)) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(gubCenterRank[0] > 7);
						}
						ubRetryData[2] = (0x04 + ubLowerShiftValueTable[gubCenterRank[0]]) & 0x7F; //gubDSP_Center_Table[gubCenterRank[0]]; //((0x64 + (gubCenterRank[0] * 8)) & 127);
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x04;
						ubRetryData[3] = 0x04;
					}
					else {
						ubRetryData[2] = 0x04;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_SUB_1_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						if (ENABLE_WP_DEBUG && ((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7))) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7));
						}
						ubRetryData[1] =  (0x7C + ubUpperShiftValueTable1[gubCenterRank[0]]) & 0x7F;
						ubRetryData[3] =  (0x7C + ubUpperShiftValueTable2[gubCenterRank[1]]) & 0x7F;
					}
					else {
						if (ENABLE_WP_DEBUG && (gubCenterRank[0] > 7)) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(gubCenterRank[0] > 7);
						}
						ubRetryData[2] =  (0x7C + ubLowerShiftValueTable[gubCenterRank[0]]) & 0x7F; //gubDSP_Center_Table[gubCenterRank[0]];
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x7C;
						ubRetryData[3] = 0x7C;
					}
					else {
						ubRetryData[2] = 0x7C;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_PLUS_2_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						if (ENABLE_WP_DEBUG && ((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7))) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7));
						}
						ubRetryData[1] =  (0x08 + ubUpperShiftValueTable1[gubCenterRank[0]]) & 0x7F;
						ubRetryData[3] =  (0x08 + ubUpperShiftValueTable2[gubCenterRank[1]]) & 0x7F;
					}
					else {
						if (ENABLE_WP_DEBUG && (gubCenterRank[0] > 7)) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(gubCenterRank[0] > 7);
						}
						ubRetryData[2] =  (0x08 + ubLowerShiftValueTable[gubCenterRank[0]]) & 0x7F; //gubDSP_Center_Table[gubCenterRank[0]];
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x08;
						ubRetryData[3] = 0x08;
					}
					else {
						ubRetryData[2] = 0x08;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_SUB_2_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						if (ENABLE_WP_DEBUG && ((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7))) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7));
						}
						ubRetryData[1] =  (0x78 + ubUpperShiftValueTable1[gubCenterRank[0]]) & 0x7F;
						ubRetryData[3] =  (0x78 + ubUpperShiftValueTable2[gubCenterRank[1]]) & 0x7F;
					}
					else {
						if (ENABLE_WP_DEBUG && (gubCenterRank[0] > 7)) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(gubCenterRank[0] > 7);
						}
						ubRetryData[2] =  (0x78 + ubLowerShiftValueTable[gubCenterRank[0]]) & 0x7F; //gubDSP_Center_Table[gubCenterRank[0]];
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x78;
						ubRetryData[3] = 0x78;
					}
					else {
						ubRetryData[2] = 0x78;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_PLUS_3_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						if (ENABLE_WP_DEBUG && ((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7))) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7));
						}
						ubRetryData[1] =  (0x10 + ubUpperShiftValueTable1[gubCenterRank[0]]) & 0x7F;
						ubRetryData[3] =  (0x10 + ubUpperShiftValueTable2[gubCenterRank[1]]) & 0x7F;
					}
					else {
						if (ENABLE_WP_DEBUG && (gubCenterRank[0] > 7)) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(gubCenterRank[0] > 7);
						}
						ubRetryData[2] =  (0x10 + ubLowerShiftValueTable[gubCenterRank[0]]) & 0x7F; //gubDSP_Center_Table[gubCenterRank[0]];
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x10;
						ubRetryData[3] = 0x10;
					}
					else {
						ubRetryData[2] = 0x10;
					}
				}
			}
			else if (ubSoftBitReadData == SOFTBIT_READ_HB_SUB_3_DELTA_VTH_DATA) {

				if (gubCenterSet) { //gubCenterSet : Second round softbit decode
					if (ubPageSel == UPPER_PAGE) {
						if (ENABLE_WP_DEBUG && ((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7))) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError((gubCenterRank[0] > 7) || (gubCenterRank[1] > 7));
						}
						ubRetryData[1] =  (0x70 + ubUpperShiftValueTable1[gubCenterRank[0]]) & 0x7F;
						ubRetryData[3] =  (0x70 + ubUpperShiftValueTable2[gubCenterRank[1]]) & 0x7F;
					}
					else {
						if (ENABLE_WP_DEBUG && (gubCenterRank[0] > 7)) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(gubCenterRank[0] > 7);
						}
						ubRetryData[2] =  (0x70 + ubLowerShiftValueTable[gubCenterRank[0]]) & 0x7F; //gubDSP_Center_Table[gubCenterRank[0]];
					}
				}
				else {
					if (ubPageSel == UPPER_PAGE) {
						ubRetryData[1] = 0x70;
						ubRetryData[3] = 0x70;
					}
					else {
						ubRetryData[2] = 0x70;
					}
				}
			}
			/*
			else if (ubSoftBitReadData == SOFTBIT_READ_LI_DATA) {
				// all zeros
			}
			*/
		}
		if (guoFlashUseType.B.btSandisk) {
#if !TLC
			if (gubSandiskRetryType == 9 && ubPageSel == LOWER_PAGE) {
				ubRetryData[0] = ubRetryData[2];
				ubRetryData[2] = 0x00;
			}
#endif
			gFREG[FCTLL_PIO_CMD].B.B0 = 0xEF;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x11;
			UBYTE ubPData;
			UWORD uwData, uwData1;
			for (ubPData = 0; ubPData < 4; ubPData++) {
				gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
				if (gFREG[FCTLL_FLH_SET].B.B0 & FLA_TOGGLE_MODE) {
					uwData1 = ubRetryData[ubPData];
					uwData = (uwData1 << 8) + uwData1;
					gFREG[FCTLL_PIO_DAT].W.W0 = uwData;
				}
				else {
					gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData[ubPData];
				}
			}
			for (gulTemp = 0; gulTemp < 10; gulTemp++) {//tWB
			}
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
			while (gFREG[FCTLL_PIO_DAT].B.B0 != 0xE0);
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x25;
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x26;
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x5D;
		}
		else {
			//A
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x04;
			gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(ubRetryData[0]);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData[0];
			}
			//B
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x05;
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(ubRetryData[1]);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData[1];
			}
			//C
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x06;
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(ubRetryData[2]);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData[2];
			}
			//D
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x07;
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(ubRetryData[3]);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = ubRetryData[3];
			}
			//E
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x0D;
			if (gubToggleMode) {
				gFREG[FCTLL_PIO_DAT].W.W0 = (UWORD)(0);
			}
			else {
				gFREG[FCTLL_PIO_DAT].B.B0 = 0;
			}

			if (!(ubPageSel == UPPER_PAGE && ubSoftBitReadData == SOFTBIT_READ_LI_DATA)) {

				if (ubSoftBitReadData == SOFTBIT_READ_HB_DATA) {
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x26;
				}
				else {
					gFREG[FCTLL_PIO_CMD].B.B0 = 0xCD;
				}
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x5D;
			}
		}
	}
	if (!ENABLE_DEBUG_RETRY_NOT_SWITCH_LEGACY) {
		if (!guoFlashUseType.B.btSandisk) {
			gFREG[FCTLL_FLH_SET].L = ulTemp;
			gubToggleMode = ubToggleModeTemp;
		}
	}

	if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
		if (ASIC) {
			//FlaClkDiv(RW_Mode, Toggle_400, 0xF);
			//Fla_Set_Offset_DQS_Delay(Toggle_400);
			if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
				for (gulTemp = 0; gulTemp < 1000; gulTemp++) {

				}
				gFREG[FCTLL_FDIV_CFG].L = 0;
			}
		}
	}
#endif
}


UBYTE SoftBitCorrectFLHShiftReadDataFPU(UBYTE ubCEIndex, UWORD uwPageIndex, UBYTE ubPageSel, UBYTE ubReadFrameIndex, UBYTE ubStartFrame, UWORD uwFUnit, U32 ulFEntry, UWORD uwFJobInfo, UBYTE ubDepth) {
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);
	UBYTE ubChannel = ubCEIndex & gubPlanesPerBurstMask;
	U32 ulShiftPlusDeltaDataAdr, ulShiftSubDeltaDataAdr, ulHBDataAdr;
	UBYTE ubRound, ubRetryTimes, ubPhyPlane;
	FSA_t * pFSA = (FSA_t *)gulFSABase[ubChannel];
	UBYTE ubBurstBank;
	UWORD uwFBlock;
	U32 ulRead_RAW_DMA_BASE = ((gubEnterRS == 0) ? ((U32)(&gubflaBuffer[16384])) : (DataZoneBase + 0x200000 + (32768 * TOTAL_PARITY_GROUP_NUM)));


	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = (ulFEntry  >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask;
	}
	else {
		ubBurstBank = (ulFEntry  >> gub4kEntrysPerBurstLog) & gubBurstsPerBankMask;
	}
	if (uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
		uwFBlock = uwFUnit;
		ubPhyPlane = ubBurstBank;
	}
	else {
		uwFBlock = M_RUT2DTo1D(ubCEIndex , (uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
		if (gubRUTMixPlaneEnable == 0) {
			ubPhyPlane = ubBurstBank;
		}
		else {
			ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
			uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
		}
	}

	if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
		if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
			uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
		}
	}
	M_Check_PseudoCE(ubCEIndex, uwFBlock);


	if (uwFJobInfo & BIT_FJOBI_FASTPAGE)
	{
		if (ENABLE_WP_DEBUG) {
			flaLEDBlink(0xEF);
		}
		else {
			gubWP_DEBUG = 0xEF;
			M_AssertError(1);
		}
	}

	U32 ulTOP_TGL_TIME_CFG_BACKUP = FCONL[FCONL_TGL_TIME_CFG];
	U32 ulTOP_TGL_TIME_CFG_1_BACKUP = FCONL[FCONL_TGL_TIME_CFG_1];
	if (gubFlashClock <= FLH_CLK_100MHz) { // Fix read row data bug under 100MHz
		FCONL[FCONL_TGL_TIME_CFG] &= 0x00FFFFFF;
		FCONL[FCONL_TGL_TIME_CFG] |= (TOG_DAT_PSTH_PHASE_CNT(3) | TOG_DAT_CLH_PHASE_CNT(5));

		FCONL[FCONL_TGL_TIME_CFG_1] &= 0x00FFFFFF;
		FCONL[FCONL_TGL_TIME_CFG_1] |= (TOG_DAT_PSTH_PHASE_CNT(3) | TOG_DAT_CLH_PHASE_CNT(5));
	}

	{
		//Part 1:Determine whether read LI data
		if (ubPageSel == UPPER_PAGE) {
			//Uart_Printf("\n\rRead LI data.");
			UWORD uwLowerPageWL;

			if ((uwPageIndex == 2) || (uwPageIndex == 255)) {
				uwLowerPageWL = uwPageIndex - 2;
			}
			else {
				uwLowerPageWL = uwPageIndex - 3;
			}
			SoftBitCorrectToshibaFlow(SOFTBIT_READ_LI_DATA, ubPageSel, 0);

			//Set FSA
			if (!ENABLE_iFSA) {
				pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | (((uwLowerPageWL << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + ubReadFrameIndex);

			}
			else {
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | (((uwLowerPageWL << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + ubReadFrameIndex);
				guliFSA[0] |= (uwFBlock & 0xFFFF) << 16;
				gFREG[FCTLL_IFSA0].L = guliFSA[0];
			}

			gFREG[FCTLL_RAW_DMA_ADR].L = (ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * SOFTBIT_READ_LI_DATA));

			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C30;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40_C00;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
				flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x1F_FPU_POL_FLHShiftReadDataFPU, gFREG, CHECK_FPU_POL_TIMEOUT);
			}
			else {
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			}

			//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
			gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_RAW;//FPU_PTR_AG_RAW;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
				flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x20_FPU_R_FLHShiftReadDataFPU, gFREG, CHECK_FPU_DMA_R_TIMEOUT);
			}
			else {
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			}

			flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);
		}


		//Part 2:Read HB data
		//Uart_Printf("\n\rRead HB data.");



		SoftBitCorrectToshibaFlow(SOFTBIT_READ_HB_DATA, ubPageSel, 0);
	}


	//Set FSA
	if (!ENABLE_iFSA) {
		pFSA[gubFSAIndex[ubChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | (((uwPageIndex << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + ubReadFrameIndex);
	}
	else {

		{
			if ( (uwFJobInfo & BIT_FJOBI_FASTPAGE) && ((!guoFlashUseType.B.btA2Cmd) || ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 )) {
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | ((flaFastPage((uwPageIndex << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + ubReadFrameIndex);
			}
			else {
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | (((uwPageIndex << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0)) + ubReadFrameIndex);
			}
		}
		guliFSA[0] |= (uwFBlock & 0xFFFF) << 16;
		gFREG[FCTLL_IFSA0].L = guliFSA[0];
	}
	ulHBDataAdr = ulRead_RAW_DMA_BASE;



	gFREG[FCTLL_RAW_DMA_ADR].L = ulHBDataAdr;
	{
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C30;
	}
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);


	{
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40_C00;
	}
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x24_FPU_POL_FLHShiftReadDataFPU, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}


	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_RAW; //FPU_PTR_AG_RAW;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x26_FPU_R_FLHShiftReadDataFPU, gFREG, CHECK_FPU_DMA_R_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}


	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

	if (guoFlashUseType.B.btSandisk) {
		if ( (guoFlashUseType.B.btToshibaBiCs2 == 0x00) && (guoFlashUseType.B.btToshibaBiCs3 == 0x00)) {
			flaReadFailRetrySandiskTerminateFlow(uwFJobInfo);
		}
	}
	/*if(gubReplaceHB){
		CopyValue_DMAC(ZONE3_RS_RAW_DATA_RECOVERY_EXECUSE_BASE+uwPageIndex*4096*gubFramePerPage+gubReplaceHBTargetFrame*4096, ZONE3_HB_DATA_SECTION1_BASE+gubReplaceHBTargetFrame*5120, 4096, 0);
		memcpy(ZONE3_HB_DATA_SECTION1_BASE+gubReplaceHBTargetFrame*5120+4096,ZONE2_RS_RECOVERY_EXECUTION_BASE+uwPageIndex*16*gubFramePerPage+gubReplaceHBTargetFrame*16,16);
		gubReplaceHB = 0;
		gubReplaceHBTargetFrame = 0;
	}*/


	{
		ubRetryTimes = 3;

		//Part 3:Read delta data
		for (ubRound = 0; ubRound < ubRetryTimes; ubRound++) {
#if (!MicronUseSBSBR)
			if (ubRound == 0) { // HB PLUS/SUB 1 DELTA VTH
				ulShiftPlusDeltaDataAdr = ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * SOFTBIT_READ_HB_PLUS_1_DELTA_VTH_DATA);//ZONE3_HB_PLUS_1_DELTA_VTH_BASE;
				ulShiftSubDeltaDataAdr = ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * SOFTBIT_READ_HB_SUB_1_DELTA_VTH_DATA);//ZONE3_HB_SUB_1_DELTA_VTH_BASE;
			}
			else if (ubRound == 1) { // HB PLUS/SUB 2 DELTA VTH
				ulShiftPlusDeltaDataAdr = ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * SOFTBIT_READ_HB_PLUS_2_DELTA_VTH_DATA);//ZONE3_HB_PLUS_2_DELTA_VTH_BASE;
				ulShiftSubDeltaDataAdr = ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * SOFTBIT_READ_HB_SUB_2_DELTA_VTH_DATA);//ZONE3_HB_SUB_2_DELTA_VTH_BASE;
			}
			else if (ubRound == 2) { // HB PLUS/SUB 3 DELTA VTH
				ulShiftPlusDeltaDataAdr = ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * SOFTBIT_READ_HB_PLUS_3_DELTA_VTH_DATA);//ZONE3_HB_PLUS_3_DELTA_VTH_BASE;
				ulShiftSubDeltaDataAdr = ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * SOFTBIT_READ_HB_SUB_3_DELTA_VTH_DATA);//ZONE3_HB_SUB_3_DELTA_VTH_BASE;
			}
			//Uart_Printf("\n\rPlus %d delta",ubRound+1);
				SoftBitCorrectToshibaFlow((SOFTBIT_READ_HB_PLUS_1_DELTA_VTH_DATA + (ubRound * 2)), ubPageSel, 0);
			gFREG[FCTLL_RAW_DMA_ADR].L = ulShiftPlusDeltaDataAdr;
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C30;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40_C00;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
				flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x43_FPU_POL_FLHShiftReadDataFPU, gFREG, CHECK_FPU_POL_TIMEOUT);
			}
			else {
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			}

			//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
			gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_RAW;//FPU_PTR_AG_RAW;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
				flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x44_FPU_R_FLHShiftReadDataFPU, gFREG, CHECK_FPU_DMA_R_TIMEOUT);
			}
			else {
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			}

			flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

			/*uwSameCnt = uwDifferentCnt = 0;
			for(ubFrameIndex=0;ubFrameIndex<gubFramePerPage;ubFrameIndex++){
				for(uw4ByteIndex=0;uw4ByteIndex<(5120/4);uw4ByteIndex++){
					if(RAML[(ulHBDataAdr+5120*ubFrameIndex)/4+uw4ByteIndex]!=RAML[(ulShiftPlusDeltaDataAdr+5120*ubFrameIndex)/4+uw4ByteIndex]){
						uwDifferentCnt++;
						//Uart_Printf("\n\rFrame:%d  Data adr:0x%l  Data:0x%l",ubFrameIndex,((ulShiftPlusDeltaDataAdr+5120*ubFrameIndex)/4+uw4ByteIndex)*4, RAML[(ulShiftPlusDeltaDataAdr+5120*ubFrameIndex)/4+uw4ByteIndex]);
					}
					else{
						uwSameCnt++;
					}
				}
			}
			Uart_Printf("\n\rSame 4 bytes cnt:%d  Different 4 bytes cnt:%d",uwSameCnt,uwDifferentCnt);*/

			//Uart_Printf("\n\rSub %d delta",ubRound+1);
				SoftBitCorrectToshibaFlow((SOFTBIT_READ_HB_PLUS_1_DELTA_VTH_DATA + (ubRound * 2) + 1), ubPageSel, 0);

			gFREG[FCTLL_RAW_DMA_ADR].L = ulShiftSubDeltaDataAdr;
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C30;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40_C00;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
				flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x45_FPU_POL_FLHShiftReadDataFPU, gFREG, CHECK_FPU_POL_TIMEOUT);
			}
			else {
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			}

			//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
			gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_RAW;//FPU_PTR_AG_RAW;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
				flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x46_FPU_R_FLHShiftReadDataFPU, gFREG, CHECK_FPU_DMA_R_TIMEOUT);
			}
			else {
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			}

			flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);
			/*uwSameCnt = uwDifferentCnt = 0;
			for(ubFrameIndex=0;ubFrameIndex<gubFramePerPage;ubFrameIndex++){
				for(uw4ByteIndex=0;uw4ByteIndex<(5120/4);uw4ByteIndex++){
					if(RAML[(ulHBDataAdr+5120*ubFrameIndex)/4+uw4ByteIndex]!=RAML[(ulShiftSubDeltaDataAdr+5120*ubFrameIndex)/4+uw4ByteIndex]){
						uwDifferentCnt++;
						//Uart_Printf("\n\rFrame:%d  Data adr:0x%l  Data:0x%l",ubFrameIndex,((ulShiftSubDeltaDataAdr+5120*ubFrameIndex)/4+uw4ByteIndex)*4, RAML[(ulShiftSubDeltaDataAdr+5120*ubFrameIndex)/4+uw4ByteIndex]);
					}
					else{
						uwSameCnt++;
					}
				}
			}*/
			//Uart_Printf("\n\rSame 4 bytes cnt:%d  Different 4 bytes cnt:%d",uwSameCnt,uwDifferentCnt);
#else	// else of (!MicronUseSBSBR)
			M_AssertError(!guoFlashUseType.B.btIntelMicron);
			/*
			 *	Note : In Micron, the SBSBR result is XNOR, here we change DMA inversion to
			 *			convert XNOR data to be XOR data
			 */
			if (ubRound == 0) {
				// change inversion setting to convert XNOR result to be XOR
				gFREG[FCTLL_CHNL_SET].L ^= (BYPASS_INV);
			}

			// delta voltage = (round+1) * 10 * 10mV
			ubDelta = (ubRound + 1) * 10;

			for (ubi = 0; ubi < 4; ubi++) {
				ubPIOData[ubi] = ubDelta;
			}

			// feature address E1 adjust SBSBR of rL1 & rL3 for upper page
			ubFeatureAddr = 0xE1;
			flafailFeatureOperation(MODE_FAIL_RETRY_SET_FEATURE, ubChannel, ubFeatureAddr, ubPIOData, 4);
			/*
			  *	Note : there are 3 types of lower page
			  *		1. Lower page with word line finish programming, use rL2
			  *		2. MLC open lower page (word line doen't finish programming), use rLP
			  *		3. SLC page (include the case that upper page mark "-"  in shared page), use rSLC
			  */
			// feature address E3 adjust SBSBR of rLP & rSLC for lower page type 2,3
			ubFeatureAddr = 0xE3;
			flafailFeatureOperation(MODE_FAIL_RETRY_SET_FEATURE, ubChannel, ubFeatureAddr, ubPIOData, 4);

			// feature address E2 adjust SBSBR of rL2 for lower page type 1, only P1 & P2 data to be used
			ubFeatureAddr = 0xE2;
			ubPIOData[2] = 0;	// clear P3
			ubPIOData[3] = 0;	// clear P4
			flafailFeatureOperation(MODE_FAIL_RETRY_SET_FEATURE, ubChannel, ubFeatureAddr, ubPIOData, 4);

			// following address translation only can be used for Micron 1024 page
			M_AssertError(!guoFlashDefaultType.B.bt1024Page);

			// Micron SBSBR CMD cannot use high speed, so here use PIO
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;								// CMD 0x00	Read Mode
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;								// CA 0~7
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;								// CA 8~14
			gFREG[FCTLL_PIO_ADR].B.B0 = (UBYTE)((guliFSA[0] >> 4) & 0xFF);	// PA 0~7
			gFREG[FCTLL_PIO_ADR].B.B0 = (UBYTE)((guliFSA[0] >> 12) & 0xFF);	// PA 8~9 BA 10~15
			gFREG[FCTLL_PIO_ADR].B.B0 = (UBYTE)((guliFSA[0] >> 20) & 0xFF);	// BA 16~21 + LA0
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x34;								// CMD 0x34	SBSBR

			gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;								// CMD 0x70 READ STATUS
			while (gFREG[FCTLL_PIO_DAT].B.B0 != 0xE0);						// Wait Ready

			gFREG[FCTLL_RAW_DMA_ADR].L = ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * (SOFTBIT_SB1_DATA + ubRound));
			//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
			gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_RAW;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
				flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x47_FPU_R_FLHShiftReadDataFPU, gFREG, CHECK_FPU_DMA_R_TIMEOUT);
			}
			else {
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			}

			flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

			if (ubRound == (ubRetryTimes - 1)) {
				// restore initial inversion setting
				gFREG[FCTLL_CHNL_SET].L ^= (BYPASS_INV);
			}
#endif

		}
	}

	if (gubFlashClock <= FLH_CLK_100MHz) { // Fix read row data bug under 100MHz
		FCONL[FCONL_TGL_TIME_CFG] = ulTOP_TGL_TIME_CFG_BACKUP;
		FCONL[FCONL_TGL_TIME_CFG_1] = ulTOP_TGL_TIME_CFG_1_BACKUP;
	}
	return 0;
}

void GenSoftBit(void) {
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	UBYTE ubRound;
	U32 ulShiftPlusDeltaDataAdr, ulShiftSubDeltaDataAdr;
	U32 ulRead_RAW_DMA_BASE = ((gubEnterRS == 0) ? ((U32)(&gubflaBuffer[16384])) : (DataZoneBase + 0x200000 + (32768 * TOTAL_PARITY_GROUP_NUM)));

	for (ubRound = 0; ubRound < 3; ubRound++) {
		ulShiftPlusDeltaDataAdr = (ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * (SOFTBIT_READ_HB_PLUS_1_DELTA_VTH_DATA + (ubRound * 2))));
		ulShiftSubDeltaDataAdr = (ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * (SOFTBIT_READ_HB_PLUS_1_DELTA_VTH_DATA + (ubRound * 2) + 1)));
		mDMAC_XOR(ulShiftPlusDeltaDataAdr, ulShiftSubDeltaDataAdr, (ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * (SOFTBIT_SB1_DATA + ubRound))), IBF_BANK_LENGTH, 1, 0);
	}
}


UBYTE FLHSoftBitDecode(UBYTE ubPageSel, UBYTE ubDepth) {
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);
	U32 ulRead_RAW_DMA_BASE = ((gubEnterRS == 0) ? ((U32)(&gubflaBuffer[16384])) : (DataZoneBase + 0x200000 + (32768 * TOTAL_PARITY_GROUP_NUM)));
	gFREG[FCTLL_RAW_DMA_ADR].L = ulRead_RAW_DMA_BASE;

#if TLC_NES_TEST_SB_FLOW
	if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
	{
		UBYTE ii;

		for ( ii = 0 ; ii < 32 ; ii++ ) {
			gubNES_C9_Data[4 + ii] = FCONB[FCONB_LLR_TABLE_0 + ii];
		}
	}
#endif


	{

		if (ubPageSel == LOWER_PAGE) {
			IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(0, 4, 1, 0);
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
		}
		else if (ubPageSel == UPPER_PAGE) {
			mDMAC_COPY((ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * SOFTBIT_READ_LI_DATA)), (ulRead_RAW_DMA_BASE + (IBF_BANK_LENGTH * 4)), IBF_BANK_LENGTH, 0, 0, 0, 0, 1);
			IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(0, 5, 1, 0);
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
		}
	}
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

	//Soft correction
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_SOFTBIT_CORRECTION;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

#if TLC_NES_TEST_SB_FLOW
	if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
	{
		NESGetIBFData(4, ubDepth);

		if ( (gubCenterSet  == 1) || (gubCenterSet  == 3)   )

		{
			NESSendC9Data( (0x40 + gubNESSBFrameIndex) , 1 , guwNESDataLen);
		}
		else {
			NESSendC9Data( (0x30 + gubNESSBFrameIndex) , 1 , guwNESDataLen);
		}
		// get Error bit + iteration
		gubNES_C9_Data[0] = gFREG[FCTLL_ECC_INFO].L;
		gubNES_C9_Data[1] = ((gFREG[FCTLL_ECC_INFO].L) >> 8);
		gFREG[FCTLL_DBG_INF].L = 90;
		gubNES_C9_Data[2] = ((gFREG[FCTLL_DBG_INF].L) & 0x001FC000) >> 14;
		gubNES_C9_Data[3] = 0;
		if (FCONL[FCONL_LDPC_BFC_SET] & LDPC_COR_CONVERGE) {
			gubNES_C9_Data[36] = 1;
		}
		else {
			gubNES_C9_Data[36] = 0;
		}
		gubNES_C9_Data[37] = 0;
		//R0:Error bit + iteration info
		NESSendC9Data(0x11, 0, 38);
	}
#endif
	gFREG[FCTLL_DBG_INF].L = (0x40000028);
	if (((gFREG[FCTLL_LDPC_CFG].L & BCH_RESULT_UNCORRECTABLE) == 0) && ((gFREG[FCTLL_DBG_INF].L & BIT17) == 0)) {
		if (ENABLE_RS_NORMAL_MODE && ENABLE_DEBUG_RS_NORMAL_MODE && gubEnterRS && gubEnterRSNormalMode) {
			if (ENABLE_WP_DEBUG) {
				if (!ENABLE_DEBUG_RS_NORMAL_MODE_FAIL_BLOCK) {
					flaLEDBlink(1);
				}
			}
			else {
				M_AssertError(1);
			}
		}
		if ((gFREG[FCTLL_LDPC_CFG].L & LDPC_RESULT_UNC) == 0) {
			VT->gsReadFailDebugCount.gulWP_DEBUG_sb_TARPINGset++;
		}
		return TRUE;
	}


	if ((gubCenterSet == 0) || (gubCenterSet == 2))
	{

		{
			gubCenterRank[0] = ((FCONL[FCONL_DSP_CENTER] >> 15) & 0x1F) % 8;
			//Uart_Printf("\n\rCenter 0 :%d",(FCONL[FCONL_DSP_CENTER]>>15)&0x1F);
			if (ubPageSel == UPPER_PAGE) {
				gubCenterRank[1] = ((FCONL[FCONL_DSP_CENTER] >> 10) & 0x1F) % 8;
				//Uart_Printf("\n\rCenter 0 :%d",((FCONL[FCONL_DSP_CENTER]>>10)&0x1F)%8);
			}

#if TLC_NES_TEST_SB_FLOW

			if ( (gubNESReadIBF == 0x01) && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
			{
				NESSendC9Data_CMD20();
			}
#endif

		}
	}

	gubCenterSet++;
	return FALSE;
}

UBYTE SoftBitCorrect(U32 ulDMAAddr, UBYTE ubCEIndex, UWORD uwFUnit, U32 ulFEntry, UWORD uwFJobInfo, U32 ulReadFail_4KMAP_SBused, UBYTE ubDepth) {
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	UBYTE ubChannel = ubCEIndex & gubPlanesPerBurstMask;
	FSA_t * pFSA = (FSA_t *)gulFSABase[ubChannel];
	UBYTE ubPageSel = 0, ubFrameIndex, ubRound;
	UWORD uwPageIndex = (M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
	UBYTE ubFrameCorrectFlag = 0;
	UBYTE ubStartFrameNo = ulFEntry & gub4kEntrysPerPlaneMask;
	UBYTE ubCEIndex_virtual;
	UBYTE ubCoarseHBPass;
	U32 ul_CHNL_SET_temp = gFREG[FCTLL_CHNL_SET].L;
	U32 ul_CHNL_RS_CFG = gFREG[FCTLL_RS_CFG].L;
	gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix the future of copy data bug & remain 2 FQ (1 program parity + 1 read FQ) bug & the future mix of one and two(already finish first) parity bug
	if (ENABLE_DEBUG_UART_FAIL || TLC_NES_TEST) {
		UartString("\r\n***SB ");
	}

	if ((ENABLE_RS_NORMAL_MODE || (ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN && gubRS_FPU_ReadGetLDPC_BCH)) && gubEnterRS) {
		gFREG[FCTLL_IBF_CTL].L &= (~FORCE_ZIP_COR);
	}

	if (ENABLE_DEBUG_HW_FORCEREADFAIL && gubRetryGenFailPhase < 3) {
		gFREG[FCTLL_UNC_CFG].L = 0;
	}

	ubCEIndex_virtual = ubCEIndex;
	M_VirtualToPhysicalCE(ubCEIndex);

	// SoftBit not need reset IBF PTR
	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(1);
	gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME);
	gFREG[FCTLL_DMA_CFG].L |= (((UWORD)1) << 8);


	if (guoFlashUseType.B.bt4LC) {
		if ((uwPageIndex % 2) == 0) {
			if (uwPageIndex != 0) {
				ubPageSel = UPPER_PAGE;
			}
			else {
				ubPageSel = LOWER_PAGE;
			}
		}
		else {
			if (uwPageIndex != guwPagesPerUnitMask) {
				ubPageSel = LOWER_PAGE;
			}
			else {
				ubPageSel = UPPER_PAGE;
			}
		}
	}
	else {
		if (ENABLE_WP_DEBUG) {
			flaLEDBlink(1);
		}
		else {
			M_AssertError(1);
		}
	}

	SoftBitCorrectSetDSPParam(ubPageSel);
	FCONL[FCONL_LDPC_CFG] |= (AUTO_UPDATE_LLR_ENABLE | LDPC_CORRECT_DIS_ENABLE | DSP_ENABLE);

	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}

	for (ubFrameIndex = 0; ubFrameIndex < gub4kEntrysPerPlane; ubFrameIndex++) {

#if TLC_NES_TEST_SB_FLOW
		gubNESSBFrameIndex =  ubFrameIndex;
#endif

		if (ulReadFail_4KMAP_SBused & (BIT0 << ubFrameIndex)) {

			gFREG[FCTLL_ZIP_MAP].L = (0xFF << (ubFrameIndex * 8));


			for (ubRound = 0; ubRound < 2; ubRound++) {
				//Uart_Printf("\n\rRound %d with default LLR table of frame %d",ubRound, ubFrameIndex);
				FCONL[FCONL_LDPC_CFG] &= CLR_LDPC_DECODE_MODE;
				if (ubRound == 0) {
					gubCenterSet = 0;
					gubCenterRank[0] = 0;
					gubCenterRank[1] = 0;

					SoftBitCorrectSetLLRTable(TSB_A19_LLR1_BASE, ubPageSel);

				}
				else {
					SoftBitCorrectSetLLRTable(TSB_A19_LLR1_BASE, ubPageSel);
				}
				ubCoarseHBPass = SoftBitCorrectFLHShiftReadDataFPU(ubCEIndex_virtual, uwPageIndex, ubPageSel, ubFrameIndex, ubStartFrameNo, uwFUnit, ulFEntry, uwFJobInfo, ubDepth);

				{

					GenSoftBit();
					if (ubPageSel == LOWER_PAGE) {
						FCONL[FCONL_LDPC_CFG] |= LDPC_DECODE_MODE_3;
					}
					else if (ubPageSel == UPPER_PAGE) {
						FCONL[FCONL_LDPC_CFG] |= LDPC_DECODE_MODE_4;
					}
				}


					//First decode
					ubFrameCorrectFlag = ((gubFPUTimeout == 1) ? 0 : FLHSoftBitDecode(ubPageSel, ubDepth));
					if (ubFrameCorrectFlag) {
						break;
					}
				//Uart_Printf("\n\rRound %d with adaptive LLR table of frame %d",ubRound, ubFrameIndex);

				//Second decode use adaptive llr table
				ubFrameCorrectFlag = ((gubFPUTimeout == 1) ? 0 : FLHSoftBitDecode(ubPageSel, ubDepth));
				if (ubFrameCorrectFlag) {
					//if (ENABLE_WP_DEBUG) {
					//}
					break;
				}

#if (TLC == 0)
				if ( ubRound == 0x00 ) {
#if TLC_NES_TEST_SB_FLOW
					//if (gubNESReadIBF) {
					FCONL[FCONL_LDPC_CFG] &= CLR_LDPC_DECODE_MODE;
					if (gubEnterRS && gubRSReadingParityPage) {
						gFREG[FCTLL_CHNL_SET].L &= (~BYPASS_CONV);
						//gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
					}
					if ( flaReadFailHBRetry_SB(   ubFrameIndex , ubStartFrameNo , ubDepth ,  ubPageSel , ubCEIndex) ) {
						if (gubEnterRS && gubRSReadingParityPage) {
							gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
						}
						goto SB_HB_OK;
					}
					if (gubEnterRS && gubRSReadingParityPage) {
						gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
					}
#endif
				}

#endif







			}
		if (guoFlashUseType.B.btToshiba) {

			if (guoFlashUseType.B.btSandisk) {
				flaReadFailRetrySandiskTerminateFlow(uwFJobInfo);
			}
			else {
				flaReadFailRetryToshibaTerminateFlow(uwFJobInfo);
			}
			flaReadFailRetrySendResetCommand(ubDepth);
		}
		else {
			if (ENABLE_WP_DEBUG) {
				flaLEDBlink(0xFD);
			}
			else {
				M_AssertError(0xFD);
			}
		}

		FCONL[FCONL_LDPC_CFG] &= CLR_LDPC_DECODE_MODE;
		if (ubFrameCorrectFlag) {
			gFREG[FCTLL_BACK_STORE].W.W0 = (HB_RETRY_L4K_TABLE_OFFSET) + ((ubFrameIndex - ubStartFrameNo) * Spare4kSize);
			IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(4, 1, 0, 1);
			gFREG[FCTLL_RAW_DMA_ADR].L = ulDMAAddr + ((ubFrameIndex - ubStartFrameNo) * 4096);
			//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
			gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

			flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

			if (gubEnterRS && gubRSReadingParityPage) {
				VT->gsReadFailDebugCount.gulWP_DEBUG_SBReadParityCount++;

				if (ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
					// Reset IBF PTR by FW to solve disable IBF_RING_MODE_EN will cause another channel IBF PTR error bug
					flaReadFailRetryResetIBFPTR(ubChannel, ubDepth);
				}
				else {
					if (ubChannel == 0) {
						FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH0_EN);
					}
					else {
						FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH1_EN);
					}
				}

				// Restore FPU
				//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
				gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
				gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
				IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(1, 1, 1, 1);
				gFREG[FCTLL_FPU_ENTRY].L  = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
				gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

				flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);


				// Fill L4K table
				volatile L4KTable16B *pL4KTable;
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + ((ubFrameIndex - ubStartFrameNo) * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (ulDMAAddr + ((ubFrameIndex - ubStartFrameNo) * 4096)) / 512;
				pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
				gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_OFFSET + ((ubFrameIndex - ubStartFrameNo) * Spare4kSize);

				// Fill the FPU COR setting
				gFREG[FCTLL_CHNL_SET].L &= (~BYPASS_CONV);
				if (ENABLE_iFSA) {
					guliFSA[0] &= (~gub4kEntrysPerPlaneMask);
					guliFSA[0] |= ubFrameIndex;
					gFREG[FCTLL_IFSA0].L = guliFSA[0];
				}
				else {
					pFSA[gubFSAIndex[ubChannel]].uword.W0 &= (~gub4kEntrysPerPlaneMask);
					pFSA[gubFSAIndex[ubChannel]].uword.W0 |= ubFrameIndex;
				}

				// FPU COR
				//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
				gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
				gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
				gFREG[FCTLL_IBF_CTL].L &= CLR_LDPC_COR_MAP;
				gFREG[FCTLL_OTHER_SET].L &= (~BCH_COR_EN);
				FCONL[FCONL_CRC_EN] &= (~CRC_32_EN); // parity page must off crc32


				gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_IBF_CORRECT;
				gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

				while (gubINTTable[ubDepth].All == 0); // use this to check
				if (gubINTTable[ubDepth].All & 0xF8) {

					VT->gulRSRecoverFailCnt++;
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(0xF3);
					}
					else {
						gubWP_DEBUG = 0xF3;
					}
					M_AssertError(1);

					//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET);
					gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
				}

				gubINTTable[ubDepth].All = 0;
				gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling

				gFREG[FCTLL_IBF_CTL].L |= LDPC_COR_MAP(0x1F);
				gFREG[FCTLL_OTHER_SET].L |= (BCH_COR_EN);

				if ((!ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE)) {
					if (ubChannel == 0) {
						FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH0_EN);
					}
					else {
						FCONL[FCONL_FLH_FUNC] |= (IBUF_RING_CH1_EN);
					}
				}
			}
		SB_HB_OK:
			ulReadFail_4KMAP_SBused &= (~(BIT0 << ubFrameIndex));
		}
	}
}

if (gubGPIO_CE_Decoder)
{
	FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
}
else {
	FCONL[FCONL_FCE_CLR] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Disable selected CE
}

if ((ENABLE_RS_NORMAL_MODE || (ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN && gubRS_FPU_ReadGetLDPC_BCH)) && gubEnterRS) {
	gFREG[FCTLL_IBF_CTL].L |= (FORCE_ZIP_COR);
	if (ENABLE_RS_NORMAL_MODE && ENABLE_RS_NORMAL_MODE_FPUCOR_BY_FRAME) {
		gulRSReadFail_4KMAP = ulReadFail_4KMAP_SBused;
	}
}

UBYTE ubPass = 0;
if (ulReadFail_4KMAP_SBused == 0) {
	ubPass = 1;
	// not update gulReadFail_4KMAP
}
else {
	ubPass = 0;
	if (!gubEnterRS) {
		gulReadFail_4KMAP = ulReadFail_4KMAP_SBused;
	}
}
//UBYTE ubPass = (ulReadFail_4KMAP_SBused == 0) ? 1 : 0;
gFREG[FCTLL_CHNL_SET].L = ul_CHNL_SET_temp;
gFREG[FCTLL_RS_CFG].L = ul_CHNL_RS_CFG; // fix the future of copy data bug & remain 2 FQ (1 program parity + 1 read FQ) bug & the future mix of one and two(already finish first) parity bug

return ubPass;
#endif
#if (RDT)
return 0;
#endif
}

UBYTE RS_FPU_Read(U32 ulDMAAddr, UBYTE ubgFQLinkIndex, U32 ulTargetPlaneIndex, UWORD uwRSPlaneIndex, UBYTE ubMTerrorCheckInRSTwoCh, UBYTE ubDepth) {
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
#if (!BURNER)
	UBYTE ubBypassReadOK = 1;
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UWORD uwFBlock;
	UBYTE ubRSCEIndex;
	UBYTE ubCEIndex;
	UBYTE ubD1Unit = Default_D1_Unit;
	UBYTE ubStartFrameNo = 0;
	UBYTE ubFrameNum = 4;
	UBYTE ubIsFastPage = (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) ? 1 : 0;
	UBYTE ubALUIndex = FQ->ubALUIndex;
	U32 ulSpareAddr;

	gulRSDMAAddr = ulDMAAddr;


	if (ENABLE_PLANE_CH_CE) {

		ubRSCEIndex = M_Modular((ulTargetPlaneIndex >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);

	}
	else {

		ubRSCEIndex = (ulTargetPlaneIndex & gubPlanesPerBurstMask) + ((M_Modular((ulTargetPlaneIndex >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);

	}
	ubCEIndex = ubRSCEIndex;
	M_VirtualToPhysicalCE(ubCEIndex);

	UBYTE ubRSChannel = ubRSCEIndex & gubPlanesPerBurstMask;

	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubRSChannel * 0x200));
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x48_FQ_RS_FPU_Read, gFREG, CHECK_FQ_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0); // make sure MT stop
	}

	if ((!ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR) && ENABLE_DEBUG_RS_READ_SOFTBITCORRECT && ((!guoFlashUseType.B.btA2Cmd) || ENABLE_DEBUG_FORCE_FASTPAGE_NONA2  || (((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) == 0) /* && (!ENABLE_ULTRAMLC)*/))) {
		if (gubEnterRSNormalMode == 0) {
			//if (uwRSPlaneIndex == 0) {
			//if ((((rand()) & 3) == 0) && ((uwRSPlaneIndex < (guwEncodeDNum - 2)) || (gubRSNotEncodeDone == 1))) {
			if (getIntRandValue(4) == 0) {
				gFREG[FCTLL_UNC_CFG].L = 0;
				VT->gsReadFailDebugCount.gulWP_DEBUG_GenRSdoSBCount++;

				if (getIntRandValue(2) == 0) {
					gFREG[FCTLL_UNC_CFG].L |= (AUTOGEN_READ_FAIL_EN);
				}
				else {
					UBYTE ubLoop = 0;
					UBYTE ubLoopCount = 0;
					ubLoopCount = (getIntRandValue(4) + 1);
					while (ubLoop < ubLoopCount) {
						gFREG[FCTLL_UNC_CFG].L |= FORCE_BCH_UNCORRECTABLE_FRAME(1 << (getIntRandValue(4)));
						ubLoop++;
					}
				}
			}
		}
	}

	gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
	gFREG[FCTLL_RS_CFG].L &= (~RS_ON_THE_FLY_EN);
	gFREG[FCTLL_ZIP_MAP].L = 0xFFFFFFFF;

	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubFrameNum);
	//Set FSA
	FSA_t * pFSA = (FSA_t *)gulFSABase[ubRSChannel];
	UBYTE ubBurstBank, ubPhyPlane;
	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = ulTargetPlaneIndex & gubBurstsPerBankMask;
	}
	else {
		ubBurstBank =  (ulTargetPlaneIndex >> gubPlanesPerBurstLog) & gubBurstsPerBankMask;
	}
	if (FQ->uwFJobInfo & BIT_FJOBI_DIRECTACESS) {
		uwFBlock = FQ->uwFUnit;
		ubPhyPlane = ubBurstBank;
	}
	else {
		if ((TLC == 0 ) || (FQ->btD3 || (ubD1Unit == Default_D1_Unit))) {
			uwFBlock = M_RUT2DTo1D(ubRSCEIndex , (FQ->uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
		}
		else {
			uwFBlock = M_RUT2DTo1D(ubRSCEIndex , ((guwFWTotalUnitNum + (ubD1Unit * D1_UNIT_NUM_PER_LINK) + (ulTargetPlaneIndex / guwFastPagePlanesPerUnit)) << gubBurstsPerBankLog) + ubBurstBank);

		}
		if (gubRUTMixPlaneEnable == 0) {
			ubPhyPlane = ubBurstBank;
		}
		else {
			ubPhyPlane = (uwFBlock & gubBurstsPerBankMask);
			uwFBlock = (uwFBlock >> gubBurstsPerBankLog);
		}
	}

	if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
		if ((uwFBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwFBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
			uwFBlock = (  ((uwFBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwFBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
		}
	}
	M_Check_PseudoCE(ubRSCEIndex, uwFBlock);

	if (!ENABLE_iFSA) {
		if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {

				pFSA[gubFSAIndex[ubRSChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | (((M_Divider(ulTargetPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0));

			}
			else {

				pFSA[gubFSAIndex[ubRSChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | (flaFastPage(((M_Divider(ulTargetPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0));

			}
		}
		else {
			pFSA[gubFSAIndex[ubRSChannel]].uword.W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | (((M_Divider(ulTargetPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0));
		}
		pFSA[gubFSAIndex[ubRSChannel]].uword.W1 = uwFBlock & 0xFFFF;
		pFSA[gubFSAIndex[ubRSChannel]].B.B4 = (uwFBlock & 0xFF0000) >> 16;

		//--------------- Address Gen Setting ---------------
		gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
		gFREG[FCTLL_FSA_SEL].L  = gubFSAIndex[ubRSChannel];
	}
	else {
		if ((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) || (TLC && (FQ->btD3 == 0) && (ubD1Unit != Default_D1_Unit))) {
			if (guoFlashUseType.B.btA2Cmd && (ENABLE_DEBUG_FORCE_FASTPAGE_NONA2 == 0)) {
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | (((M_Divider(ulTargetPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0));
			}
			else {
				guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15)) | (flaFastPage(((M_Divider(ulTargetPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) << gubSectorsPerPlaneLog)) >> (guoFlashUseType.B.bt1024Page ? 1 : 0));
			}
		}
		else {

			guliFSA[0] = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | (((M_Divider(ulTargetPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.bt1024Page ? 1 : 0));
		}
		guliFSA[0] |= (uwFBlock & 0xFFFF) << 16;

		gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;
		gFREG[FCTLL_IFSA0].L = guliFSA[0];
	}

	gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
	gFREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(FQ->ubALUIndex);


	UBYTE ubL4KCount = 0;
	volatile L4KTable16B *pL4KTable;
	gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_OFFSET;
	for (ubL4KCount = 0; ubL4KCount < ubFrameNum; ubL4KCount++) {
		pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
		//set L4K table info
		pL4KTable->ulL4K_LCA = 0x00000000;
		pL4KTable->Para0x04.ulAll = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (ulDMAAddr + ubL4KCount * 4096) / 512;
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
	}
	FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs

	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT( gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}
	// Wait flash interface ready
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x49_FPU_POL_RS_FPU_Read, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}

	// MT preload + FPU trig = Auto clear CE, so it need to set CE again

	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT( gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}

	if (ENABLE_MULTIPLANE_READ) {
		{
			flaReadFailRetrySendResetCommand(ubDepth);
		}
	}



	if (FQ->ubALUIndex == ALU_A2)
	{
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_CA2_C00_A5_C30;
	}
	else {

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C30;
	}

	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME) ;
	gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubFrameNum) << 8);


	{
#if HV
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40;
#else
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40_C00;
#endif

	}
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x4A_FPU_POL_RS_FPU_Read, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}


	if ((ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN || ENABLE_RS_NORMAL_MODE) && gubRS_FPU_ReadGetLDPC_BCH) {
		gFREG[FCTLL_RAW_DMA_ADR].L = ulDMAAddr;
		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
		// Read RAW
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_RAW;//FPU_PTR_AG_RAW;

	}
	else {
		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;



#if HV
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR; // To avoid read out fail
#else
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_DR;
#endif
	}
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x4C_FPU_R_RS_FPU_Read, gFREG, CHECK_FPU_DMA_R_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	}

	while (gubINTTable[ubDepth].All == 0); // use this to check

	if ((gubINTTable[ubDepth].All & 0xF8) || gubFPUTimeout) {

		/*if (gFREG[FCTLL_INT_INF].L & CRC16_ERR_INF) {
			gFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
			gFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(CRC16_FAIL_MAP);
			M_AssertError(1);
		}*/
		RS_FPU_FAIL_CONDITION_t ulRS_FPU_Fail_Condition;

		ulRS_FPU_Fail_Condition.BitMap.ubPlaneNoInPage = M_Modular(ulTargetPlaneIndex, DEF_PlanesPerPage_Mod_Mask);

		if (ENABLE_DEBUG_RS_NORMAL_MODE_UART || ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN_UART) {
			if (gubRS_FPU_ReadGetLDPC_BCH) {
				UartString("\nNormalReadFail");
			}
		}

		if (ENABLE_DEBUG_UART_NES_RS) {
			UartString("\nRS_FPU_FAIL: ");
			UartWordHex(uwRSPlaneIndex);
		}

		if (ENABLE_RS_NORMAL_MODE && ENABLE_RS_NORMAL_MODE_FPUCOR_BY_FRAME) {
			gulRSReadFail_4KMAP = 0xF;
		}

		if (gFREG[FCTLL_INT_INF].L & UNC_MAP_INF) {

			if (ENABLE_WP_DEBUG && (gFREG[FCTLL_INT_INF].L & ERASE_PAGE)) {
				flaLEDBlink(1);
			}
			else if (gFREG[FCTLL_INT_INF].L & ERASE_PAGE) {
				ulRS_FPU_Fail_Condition.BitMap.btReadErasePage = 1;
#if ((FORCE_AUTOPOL_TIMEOUT==0) && (B0KB==0))
				M_AssertError(1);
#endif
			}

			gubINTTable[ubDepth].All = 0;
			gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
			UBYTE ubHBRetryFailFrameMap = 0;

			if ((ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN || ENABLE_RS_NORMAL_MODE) && gubRS_FPU_ReadGetLDPC_BCH) {
				ubHBRetryFailFrameMap = 0xF;
			}
			else {
				gulFQEntry_HR = FQ->ulFEntry;
				ubHBRetryFailFrameMap = flaReadFailHBRetry(HB_RETRY_L4K_TABLE_OFFSET, ubStartFrameNo, ubFrameNum, ubIsFastPage, ubDepth, ubRSChannel, ubALUIndex);
			}
			ulRS_FPU_Fail_Condition.BitMap.btReadUNC = 1;
			ulRS_FPU_Fail_Condition.BitMap.btUNCFrameMap = (ubHBRetryFailFrameMap & 0xF);
			if (ENABLE_RS_NORMAL_MODE && ENABLE_RS_NORMAL_MODE_FPUCOR_BY_FRAME) {
				gulRSReadFail_4KMAP = ubHBRetryFailFrameMap;
			}
			if (ubHBRetryFailFrameMap == 0) {
				if (ENABLE_DEBUG_RS_NORMAL_MODE && gubEnterRSNormalMode && (!ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR)) {
					if (!ENABLE_DEBUG_RS_NORMAL_MODE_FAIL_BLOCK) {
						flaLEDBlink(1);
					}
				}
				//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
				gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
				if ((!ubMTerrorCheckInRSTwoCh) && (ubRSChannel != (FQ->ubCEIndex & gubPlanesPerBurstMask))) {

					if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
						gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
					}
				}
				goto MARK_RS_FPU_OK;
			}
			else if (ENABLE_RS_NORMAL_MODE && gubEnterRSNormalMode && (!gubRS_FPU_ReadGetLDPC_BCH) && (uwRSPlaneIndex == guwRSErrorPlaneIndex[0])) {

				UBYTE ubCheckPass = 0;
				U32 ulReadFail_4KMAP_BAK = gulReadFail_4KMAP;
				UBYTE ubi = 0;
				UBYTE ubUNCframeNo = 0;
				for (ubi = 0; ubi < gub4kEntrysPerPlane; ubi++) {
					if (gulReadFail_4KMAP & (1 << ubi)) {
						ubUNCframeNo = 1;
					}
					if (ubUNCframeNo) {
						ulReadFail_4KMAP_BAK |= (1 << ubi);
					}
				}
				if ((ulReadFail_4KMAP_BAK & ubHBRetryFailFrameMap) == 0) {
					ubCheckPass = 1;
				}

				if (ubCheckPass) {
					VT->gulRSNormalModeHBSB_PartialCorrectCount++;
					//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
					gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
					if ((!ubMTerrorCheckInRSTwoCh) && (ubRSChannel != (FQ->ubCEIndex & gubPlanesPerBurstMask))) {
						if (gFREG[FCTLL_INT_INF].L & INTR_BUSY) {
							gFREG[FCTLL_INT_INF].L &= (FREE_INTERRUPT);
						}
					}
					goto MARK_RS_FPU_OK;
				}
			}

			if (ENABLE_DEBUG_RS_NORMAL_MODE && ENABLE_DEBUG_RS_READ_SOFTBITCORRECT && gubEnterRSNormalMode) {
				/*UBYTE ubIsErrorPlane = 0;
				UBYTE ubi = 0;
				for (ubi = 0; ubi < gubOriRSErrorNum; ubi++) {
					if (uwRSPlaneIndex == guwRSErrorPlaneIndex[ubi]) {
						ubIsErrorPlane = 1;
					}
				}
				if (ubIsErrorPlane) {
					//UartString("\nIsError2");
					if (ENABLE_WP_DEBUG && (gFREG[FCTLL_UNC_CFG].L == 0)) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(gFREG[FCTLL_UNC_CFG].L == 0);
					}
				}*/
			}
			else if (ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR) {

			}
			else if (ENABLE_DEBUG_RS_READ_SOFTBITCORRECT) {
				gFREG[FCTLL_UNC_CFG].L = 0;
			}

			if (ENABLE_SOFTBITCORRECTION && ((!guoFlashUseType.B.btA2Cmd) || ENABLE_DEBUG_FORCE_FASTPAGE_NONA2  || (((FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) == 0) /* && (!ENABLE_ULTRAMLC)*/)))
			{
				//SYS1L[SYS1L_INT_STS3] = (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);	//W1C after doing SoftBitCorrect
				//M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
				//SYS1L[SYS1L_INT_EN3] &= (~(FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));

				UBYTE ubSoftBitCorrectOK = 0;

				if ((!(ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN || ENABLE_RS_NORMAL_MODE))
				        || ((ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN || ENABLE_RS_NORMAL_MODE) && (!gubRS_FPU_ReadGetLDPC_BCH))) {
					ubSoftBitCorrectOK = SoftBitCorrect(ulDMAAddr, ubRSCEIndex, FQ->uwFUnit, ((U32)ulTargetPlaneIndex << gub4kEntrysPerPlaneLog), FQ->uwFJobInfo, ubHBRetryFailFrameMap, ubDepth);
				}


				//SYS1L[SYS1L_INT_STS3] = (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);	//W1C after doing SoftBitCorrect
				//M_AssertError(SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2));
				//SYS1L[SYS1L_INT_EN3] |= (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2);

				if (ubSoftBitCorrectOK) {
					if (ENABLE_DEBUG_RS_NORMAL_MODE && gubEnterRSNormalMode && (!ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR)) {
						if (!ENABLE_DEBUG_RS_NORMAL_MODE_FAIL_BLOCK) {
							flaLEDBlink(1);
						}
					}
					//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
					gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
					if ((!ubMTerrorCheckInRSTwoCh) && (ubRSChannel != (FQ->ubCEIndex & gubPlanesPerBurstMask))) {

						if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
							gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
						}
					}
					VT->gsReadFailDebugCount.gulWP_DEBUG_RSdoSBCount++; // softbit correct ok
					goto MARK_RS_FPU_OK;
				}
				else {
					if (ENABLE_DEBUG_RS_NORMAL_MODE && ENABLE_DEBUG_RS_READ_SOFTBITCORRECT && gubEnterRSNormalMode) {
						UBYTE ubIsErrorPlane = 0;
						UBYTE ubi = 0;
						for (ubi = 0; ubi < gubOriRSErrorNum; ubi++) {
							if (uwRSPlaneIndex == guwRSErrorPlaneIndex[ubi]) {
								ubIsErrorPlane = 1;
							}
						}
						if (!ubIsErrorPlane) {
							flaLEDBlink(1);
						}
					}
					else if (ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR) {

					}
					else if (ENABLE_DEBUG_RS_READ_SOFTBITCORRECT) {
						flaLEDBlink(1);
					}
				}
			}
		}
		else if (gFREG[FCTLL_INT_INF].L & (CRC32_ERROR | LCA_FW_CMP_ERR_INF | FW_SETTING_ERR_INF | DECOMPRESSION_FAIL_INF | CRC16_ERROR)) {
			ulRS_FPU_Fail_Condition.BitMap.uwFAIL_INT_INF = (UWORD)((gFREG[FCTLL_INT_INF].L & 0xFFFF0000) >> 16);
			if (ENABLE_WP_DEBUG) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(1);
			}
		}
		else if (gFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) {
			if (ENABLE_DEBUG_RS_NORMAL_MODE && gubEnterRSNormalMode) {
				flaLEDBlink(1);
			}
			//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET);
			gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			if ((!ubMTerrorCheckInRSTwoCh) && (ubRSChannel != (FQ->ubCEIndex & gubPlanesPerBurstMask))) {

				if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
					gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
				}
			}
			goto MARK_RS_FPU_OK;
		}

		//gFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET);
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		if ((!ubMTerrorCheckInRSTwoCh) && (ubRSChannel != (FQ->ubCEIndex & gubPlanesPerBurstMask))) {

			if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
				gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
			}
		}

		if (ENABLE_DEBUG_RS_NORMAL_MODE_UART) {
			UartString("\nRSFail");
		}

		if (ENABLE_DEBUG_UART_NES_RS) {
			UartString(" F_FAIL ");
		}

		gsTableRSErrorFrameBitMap.ulAll = ulRS_FPU_Fail_Condition.ulAll;

		if (ENABLE_RS_NORMAL_MODE) {
			if (gubEnterRSNormalMode == 0) {
				if (ENABLE_DEBUG_RS_VERIFY_DATA) {
					if (uwRSPlaneIndex != guwRSErrorPlaneIndex[0]) {
						if (ENABLE_DEBUG_RS_NORMAL_MODE_UART) {
							UartString(" NotErrPlane");
						}
						if (gubRSErrorNum < RS_MAX_ERROR_COUNT) { // set Error Plane Index
							guwRSErrorPlaneIndex[gubRSErrorNum] = uwRSPlaneIndex;
						}
						gubRSErrorNum++;
					}
					else {
						if (ENABLE_DEBUG_RS_NORMAL_MODE_UART) {
							UartString(" ErrPlane");
						}
						gubRSDebug_NoGoldenToVerifyDATA = 1;
						gulRSDebug_NoGoldenToVerifyDATA_DebugCount++;
					}
				}
				else {
					if (gubRSErrorNum < RS_MAX_ERROR_COUNT) { // set Error Plane Index
						guwRSErrorPlaneIndex[gubRSErrorNum] = uwRSPlaneIndex;
					}
					gubRSErrorNum++;
				}
			}
		}
		else {
			if (ENABLE_DEBUG_RS_VERIFY_DATA) {
				if (uwRSPlaneIndex != guwRSErrorPlaneIndex[gubRSErrorNum - 1]) {
					if (gubRSErrorNum < RS_MAX_ERROR_COUNT) { // set Error Plane Index
						guwRSErrorPlaneIndex[gubRSErrorNum] = uwRSPlaneIndex;
					}
					gubRSErrorNum++;
				}
				else {
					gubRSDebug_NoGoldenToVerifyDATA = 1;
					gulRSDebug_NoGoldenToVerifyDATA_DebugCount++;
				}
			}
			else {
				if (gubRSErrorNum < RS_MAX_ERROR_COUNT) { // set Error Plane Index
					guwRSErrorPlaneIndex[gubRSErrorNum] = uwRSPlaneIndex;
				}
				gubRSErrorNum++;
			}
		}
		//UartString("UNC2: ");
		//UartWordHex(uwRSPlaneIndex);
		//UartString("\n");
		ubBypassReadOK = 0;

		if (ENABLE_RS_NORMAL_MODE && gubEnterRSNormalMode && (!gubRS_FPU_ReadGetLDPC_BCH)) {
			// move spare from IRAM to 0x62000000
			ulSpareAddr = CorrectRSSpareStart + (((ulDMAAddr - CorrectRSDataStart) / 16384) * Spare16kSize);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate((void *)ulSpareAddr, Spare16kSize);
			}
			memcpy((void *)(ulSpareAddr), (void *)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET), Spare16kSize);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)ulSpareAddr, Spare16kSize);
			}
		}
	}
	else {
	MARK_RS_FPU_OK:
		if (ENABLE_DEBUG_RS_NORMAL_MODE_UART) {
			if (gubRS_FPU_ReadGetLDPC_BCH) {
				UartString("\nNormalReadOK");
			}
		}

		if ((!(ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN || ENABLE_RS_NORMAL_MODE))
		        || ((ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN || ENABLE_RS_NORMAL_MODE) && (!gubRS_FPU_ReadGetLDPC_BCH))) {
			// move spare from IRAM to 0x62000000

			if (ENABLE_RS_NORMAL_MODE) {
				if (ulDMAAddr == VerifyDecodeRSDataStart) {
					ulSpareAddr = VerifyDecodeRSSpareStart;
				}
				else {
					ulSpareAddr = CorrectRSSpareStart + (((ulDMAAddr - CorrectRSDataStart) / 16384) * Spare16kSize);
				}
			}
			else {
				ulSpareAddr = (ulDMAAddr == (U32)VerifyDecodeRSDataStart) ? ((U32)VerifyDecodeRSSpareStart) : ((U32)RSFrameSpareDecodeStart);
			}
			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate((void *)ulSpareAddr, Spare16kSize);
			}
			memcpy((void *)(ulSpareAddr), (void *)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET), Spare16kSize);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)ulSpareAddr, Spare16kSize);
			}
		}
	}

	// need clean msg dispatch
	gubINTTable[ubDepth].All = 0;
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
	//M_AddMTDepth(ubDepth);

	FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs

	return ubBypassReadOK;
#endif
#endif
#if(RDT)
	return 0;
#endif
}


U32 RS_Get_FrameStartPhyPlaneIndex(U32 ulTargetPlaneIndex, UWORD uwRSGroupMaxEncodePlaneNum, UBYTE ubIdleTagNum) {
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	U32 ulPhyPlaneIndex = 0;

	if (ubIdleTagNum == 0) {
		// guwRSPlanesPerPage is 2 (1CE 1Plane) or 4
		ulPhyPlaneIndex = (guwRSPlanesPerPage * ((M_Divider(ulTargetPlaneIndex, DEF_RSPlanesPerPage_Mcl_Div)) % USERDATA_RS_Group_Num));
	}
	else {
		UBYTE ubTotalGroupRSPlanesPerPageToPARITYNUM = (M_Divider((PARITY_DATA_PAGE_NUM + guwRSPlanesPerPageMask), DEF_RSPlanesPerPage_Mcl_Div));

		if ((ulTargetPlaneIndex / uwRSGroupMaxEncodePlaneNum) == 0) {
			if (((ulTargetPlaneIndex / (guwRSPlanesPerPage * USERDATA_RS_Group_Num)) % ubTotalGroupRSPlanesPerPageToPARITYNUM) == (ubTotalGroupRSPlanesPerPageToPARITYNUM - 1)) {
				if ((M_Modular(ulTargetPlaneIndex, DEF_RSPlanesPerPage_Mod_Mask)) < (guwRSPlanesPerPage - (ubIdleTagNum * ((ulTargetPlaneIndex / uwRSGroupMaxEncodePlaneNum) + 1)))) {
					ulPhyPlaneIndex = (M_Multiplier((M_Divider(ulTargetPlaneIndex, DEF_RSPlanesPerPage_Mcl_Div)), DEF_RSPlanesPerPage_Mcl_Div));
					ulPhyPlaneIndex -= (guwRSPlanesPerPage * USERDATA_RS_Group_Num * (ubTotalGroupRSPlanesPerPageToPARITYNUM - 1));
				}
				else {
					ulPhyPlaneIndex = ((M_Multiplier((M_Divider((ulTargetPlaneIndex + guwRSPlanesPerPage), DEF_RSPlanesPerPage_Mcl_Div)), DEF_RSPlanesPerPage_Mcl_Div)) - (ubIdleTagNum * ((ulTargetPlaneIndex / uwRSGroupMaxEncodePlaneNum) + 1)));
				}
			}
			else {
				ulPhyPlaneIndex = (M_Multiplier((M_Divider(ulTargetPlaneIndex, DEF_RSPlanesPerPage_Mcl_Div)), DEF_RSPlanesPerPage_Mcl_Div));
				ulPhyPlaneIndex -= (guwRSPlanesPerPage * USERDATA_RS_Group_Num * ((ulTargetPlaneIndex / (guwRSPlanesPerPage * USERDATA_RS_Group_Num)) % ubTotalGroupRSPlanesPerPageToPARITYNUM));
			}
		}
		else {
			if (((ulTargetPlaneIndex / (guwRSPlanesPerPage * USERDATA_RS_Group_Num)) % ubTotalGroupRSPlanesPerPageToPARITYNUM) == (ubTotalGroupRSPlanesPerPageToPARITYNUM - 1)) {
				if ((M_Modular(ulTargetPlaneIndex, DEF_RSPlanesPerPage_Mod_Mask)) < (guwRSPlanesPerPage - (ubIdleTagNum * ((ulTargetPlaneIndex / uwRSGroupMaxEncodePlaneNum) + 1)))) {
					ulPhyPlaneIndex = (M_Multiplier((M_Divider(ulTargetPlaneIndex, DEF_RSPlanesPerPage_Mcl_Div)), DEF_RSPlanesPerPage_Mcl_Div));
					ulPhyPlaneIndex -= uwRSGroupMaxEncodePlaneNum;
					ulPhyPlaneIndex += (guwRSPlanesPerPage - (ubIdleTagNum * (ulTargetPlaneIndex / uwRSGroupMaxEncodePlaneNum)));
				}
				else {
					ulPhyPlaneIndex = ((M_Multiplier((M_Divider((ulTargetPlaneIndex + guwRSPlanesPerPage), DEF_RSPlanesPerPage_Mcl_Div)), DEF_RSPlanesPerPage_Mcl_Div)) - (ubIdleTagNum * ((ulTargetPlaneIndex / uwRSGroupMaxEncodePlaneNum) + 1)));
				}
			}
			else {
				ulPhyPlaneIndex = (M_Multiplier((M_Divider(ulTargetPlaneIndex, DEF_RSPlanesPerPage_Mcl_Div)), DEF_RSPlanesPerPage_Mcl_Div));
				ulPhyPlaneIndex -= (guwRSPlanesPerPage * (USERDATA_RS_Group_Num * (((ulTargetPlaneIndex / (guwRSPlanesPerPage * USERDATA_RS_Group_Num)) % ubTotalGroupRSPlanesPerPageToPARITYNUM) + 1)));
				ulPhyPlaneIndex += (guwRSPlanesPerPage - (ubIdleTagNum * (ulTargetPlaneIndex / uwRSGroupMaxEncodePlaneNum)));
			}
		}
	}

	return ulPhyPlaneIndex;
#endif
#if(RDT)
	return 0;
#endif
}

UWORD RS_Get_RSGroupTagFailMapIndex(UBYTE ubgFQLinkIndex) {
#if(!RDT && !BURNER)
	UWORD uwRSGroupTagFailMapIndex = 0;
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	U32 ulTargetPlaneIndex = FQ->ulFEntry >> gub4kEntrysPerPlaneLog;


	Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);
	uwRSGroupTagFailMapIndex = (UWORD)RS_FLAFAIL.ubGroupIndex * RS_CHANGE_TAG_COUNT_PER_GROUP + RS_FLAFAIL.ubTagIndex;

	if (ENABLE_WP_DEBUG && ((uwRSGroupTagFailMapIndex >= (MAX_RS_GROUP_TAG_FAIL_MAP_LENGTH * 8)))) {
		flaLEDBlink(1);
	}
	else {
		M_AssertError((uwRSGroupTagFailMapIndex >= (MAX_RS_GROUP_TAG_FAIL_MAP_LENGTH * 8)));
	}
	return uwRSGroupTagFailMapIndex;
#else // RDT or BURNER, just return 0
	return 0;
#endif
}

void RS_Flush_MultiPlaneProgram(UBYTE ubDepth) {
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	UBYTE ubCEIndex, ubCE_FQLinkIndex, ubCE_FQCount,/*ubDepth, */ ubRSChannel;
	FlashQueue_t *FQ_PTR;
	for (ubCEIndex = 0; ubCEIndex < gubMTQTotalPhyCE; ubCEIndex++) {

		ubCE_FQLinkIndex = gFQI.ubFQIndex[ubCEIndex][gFQI.ubFQFirst[ubCEIndex]]; // get the first FQ in this CE
		UBYTE ubFirstReDo = 0;

		for (ubCE_FQCount = 0; ubCE_FQCount < gFQI.ubFQDoingNumber[ubCEIndex]; ) {
			FQ_PTR = &gFQI.gFQLink[ubCE_FQLinkIndex];

			if (FQ_PTR->ubDepth != 0xFF && FQ_PTR->ubFQPhase != 0) { // Check Phase != 0

				if (FQ_PTR->ubFJob == BYTE_FJOB_WRITE) {
					// WRITE
					// Check FQ->ubDepth (Old Poll Fail function) & FQ->ubRelativeDepth
					if (gubINTTable[FQ_PTR->ubDepth].B.btAll_Done == 0 || FQ_PTR->FQPara.B.btCanRemove == 0) {

						ubRSChannel  = ubCEIndex & gubPlanesPerBurstMask;
						gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubRSChannel * 0x200));

						if (gubINTTable[FQ_PTR->ubRelativeDepth].B.btAll_Done) { // 80h, 11h, 15h

							FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = 0;

							if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
							{
								FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
							}
							else {
								FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
							}

							/*
							Don't to effect 10h for previous
							// Wait flash interface ready
							gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
							gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
							while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

							// MT preload + FPU trig = Auto clear CE, so it need to set CE again
							FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
							*/
							if (FQ_PTR->ubWritePhaseRedoCmd == MARK_FPU_C80_C10C15 || FQ_PTR->ubWritePhaseRedoCmd == MARK_FPU_NonC80_C10C15) {

								FQ_PTR->btRSPatchUpWritePhaseCmd = 1;

								// POL
								//M_GetMTDepth(ubDepth);
								//gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
								//gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);

								//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
								gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
								gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
								gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
								gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
								if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
									flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x51_FPU_POL_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_POL_TIMEOUT);
								}
								else {
									while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
								}
								flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

								// MT preload + FPU trig = Auto clear CE, so it need to set CE again

								if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
								{
									FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
								}
								else {
									FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
								}
								// POL FAIL
								//M_GetMTDepth(ubDepth);
								//gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
								//gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);

								//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
								gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
								gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
								gFREG[FCTLL_FPU_ENTRY].L = flaReadFailStatusFPUPointer(ubCE_FQLinkIndex);
								gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
								if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
									flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x52_FPU_POL_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_POL_TIMEOUT);
								}
								else {
									while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
								}
								flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, ubCE_FQLinkIndex);

								//gulRetry_RelativeFinishedRedoWrite_DebugCount++;

							}
							else if (ubFirstReDo == 0) {

								FQ_PTR->btRSPatchUpWritePhaseCmd = 1;

								//gulRetry_RelativeFinished10h_DebugCount++;

								// 10h for previous
								//M_GetMTDepth(ubDepth);
								//gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
								//gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);


								if ((FQ_PTR->ubRedundantDepth == 0xFF) || (gubINTTable[FQ_PTR->ubRedundantDepth].B.btAll_Done == 0) ) {	//原本沒有補10h or 10h補了但還沒有做完時, 這邊才能再補10h

									//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
									gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
									gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;

									gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C10;

									gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
									if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
										flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x53_FPU_W_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_DMA_W_TIMEOUT);
									}
									else {
										while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
									}
									flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);
								}

								// MT preload + FPU trig = Auto clear CE, so it need to set CE again

								if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
								{
									FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
								}
								else {
									FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
								}

								gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN);
								gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
								gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
								if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
									flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x54_FPU_POL_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_POL_TIMEOUT);
								}
								else {
									while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
								}

								// MT preload + FPU trig = Auto clear CE, so it need to set CE again

								if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
								{
									FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
								}
								else {
									FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
								}
								// POL FAIL
								//M_GetMTDepth(ubDepth);
								//gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
								//gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);

								//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
								gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
								gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
								gFREG[FCTLL_FPU_ENTRY].L = flaReadFailStatusFPUPointer(ubCE_FQLinkIndex);
								gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
								if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
									flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x55_FPU_POL_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_POL_TIMEOUT);
								}
								else {
									while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
								}
								flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, ubCE_FQLinkIndex);

								ubFirstReDo = 1;
							}
							//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
							gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
							gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
							gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
							gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
							if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
								flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x56_FPU_POL_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_POL_TIMEOUT);
							}
							else {
								while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
							}
							flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

							{
								// MT preload + FPU trig = Auto clear CE, so it need to set CE again
								if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
								{
									FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
								}
								else {
									FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
								}
								// 0xFF
								gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN);
								flaReadFailRetrySendResetCommand(ubDepth);
							}
							if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
							{
								FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = 0;
							}
							else {
								FCONL[FCONL_FCE_CLR] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Disable selected CE
							}
						}
						else {

							if (ubFirstReDo == 0) {
								//gulRetry_RelativeNoFinished10h_DebugCount++;

								FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = 0;
								if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
								{
									FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
								}
								else {
									FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
								}
								/*
								Don't to effect 10h for previous
								// Wait flash interface ready
								gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
								gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
								while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

								// MT preload + FPU trig = Auto clear CE, so it need to set CE again
								FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
								*/
								if (FQ_PTR->ubWritePhaseRedoCmd == MARK_FPU_NonC80_C10C15 || FQ_PTR->ubWritePhaseRedoCmd == MARK_FPU_NonC80_NonC10C15) {

									FQ_PTR->btRSPatchUpWritePhaseCmd = 1;

									// 10h for previous
									//M_GetMTDepth(ubDepth);
									//gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
									//gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
										if ((FQ_PTR->ubRedundantDepth == 0xFF) || (gubINTTable[FQ_PTR->ubRedundantDepth].B.btAll_Done == 0) ) {	//原本沒有補10h or 10h補了但還沒有做完時, 這邊才能再補10h

											//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
											gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
											gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
											gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C10;
											gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
											if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
												flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x57_FPU_W_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_DMA_W_TIMEOUT);
											}
											else {
												while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
											}
											flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);
										}

										// MT preload + FPU trig = Auto clear CE, so it need to set CE again
										if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
										{
											FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
										}
										else {
											FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
										}
										gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN);
										gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
										gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
										if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
											flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x58_FPU_POL_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_POL_TIMEOUT);
										}
										else {
											while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
										}

										// MT preload + FPU trig = Auto clear CE, so it need to set CE again
										if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
										{
											FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
										}
										else {
											FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
										}
										// POL FAIL
										//M_GetMTDepth(ubDepth);
										//gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
										//gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);

										//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
										gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
										gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
										gFREG[FCTLL_FPU_ENTRY].L = flaReadFailStatusFPUPointer(ubCE_FQLinkIndex);
										gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
										if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
											flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x59_FPU_POL_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_POL_TIMEOUT);
										}
										else {
											while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
										}
										flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, ubCE_FQLinkIndex);
								}
								//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
								gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
								gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
								gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
								gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
								if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
									flaCheckChannelFPUBusy(gubDebugFQLinkIndex, SN_0x5A_FPU_POL_RS_Flush_MultiPlaneProgram, gFREG, CHECK_FPU_POL_TIMEOUT);
								}
								else {
									while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
								}
								flaReadFailRetryCheckSendCmdStatus(ubDepth, 0, 0, DONTCARE);

								{
									// MT preload + FPU trig = Auto clear CE, so it need to set CE again
									if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
									{
										FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubRSChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
									}
									else {
										FCONL[FCONL_FCE_SET] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
									}
									// 0xFF
									gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN);
									flaReadFailRetrySendResetCommand(ubDepth);
								}
								if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
								{
									FCONB[FCONB_FCE_ENB_CH0 + ubRSChannel] = 0;
								}
								else {
									FCONL[FCONL_FCE_CLR] = (ubRSChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Disable selected CE
								}
								ubFirstReDo = 1;
							}
						}

					}
				}
			}
			ubCE_FQCount++;
			ubCE_FQLinkIndex = gFQI.ubFQIndex[ubCEIndex][((gFQI.ubFQFirst[ubCEIndex] + ubCE_FQCount) & FQ_NUM_MASK)];
		}
	}
}

void RS_CheckEncodeDPageMap(UBYTE FQRWTableUnit, UBYTE ubRSFrameIndex, UBYTE ubIdleTagNum, UWORD uwRSGroupMaxEncodePlaneNum, UWORD uwDataParityDummyPlanesNum, UBYTE ubPARITY_GROUP_NUM) {
#if (!RDT && !BURNER)
	UBYTE ubFQLinkIndex, ubFQCount = 0;
	FlashQueue_t *FQTemp;
	U32 ulTargetPlaneIndexTemp, ulRSPlaneIndexTemp;
	for (ubFQLinkIndex = gFQI.ubFQLinkFirst; ubFQCount < gFQI.ubFQLinkNumber; (ubFQLinkIndex = FQTemp->ubNext)) {
		FQTemp = &gFQI.gFQLink[ubFQLinkIndex];
		if ((FQTemp->ubFJob == BYTE_FJOB_WRITE) && (FQTemp->ubRSFrameIndex == ubRSFrameIndex)) {
			if ((gubINTTable[FQTemp->ubRelativeDepth].All & 0xFA) != 0) {
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(0xE7);
				}
				else {
					gubWP_DEBUG = 0xE7;
					M_AssertError(1);
				}
			}
			if (gubINTTable[FQTemp->ubRelativeDepth].B.btAll_Done == 0) {
				if (gubINTTable[FQTemp->ubRelativeDepth].B.btDMA_Done) {
					if (ENABLE_WP_DEBUG) {
						flaLEDBlink(0xE6);
					}
					else {
						gubWP_DEBUG = 0xE6;
						M_AssertError(1);
					}
				}
				// not encode yet
				ulTargetPlaneIndexTemp = FQTemp->ulFEntry >> gub4kEntrysPerPlaneLog;

				if (FQRWTableUnit) {
					ulRSPlaneIndexTemp = (M_Modular(ulTargetPlaneIndexTemp, DEF_TableParityPlanes_Mod_Mask));
				}
				else {
					if (!ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT) {

						Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndexTemp);
						ulRSPlaneIndexTemp = RS_FLAFAIL.uwPlaneOffset;

					}
					else {
						ulRSPlaneIndexTemp = ((M_Divider((ulTargetPlaneIndexTemp % (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM )), DEF_PlanesPerPage_Mcl_Div)) / (ubPARITY_GROUP_NUM));
						ulRSPlaneIndexTemp = (M_Multiplier(ulRSPlaneIndexTemp, DEF_PlanesPerPage_Mcl_Div)) + (M_Modular(ulTargetPlaneIndexTemp, DEF_PlanesPerPage_Mod_Mask));
					}
				}

				gubEncodedDPageMap[(ulRSPlaneIndexTemp >> 3)] &= (~(BIT0 << (ulRSPlaneIndexTemp & 7)));
			}
		}
		ubFQCount++;
	}
#endif
}

void RS_Get_RSPlaneIndex(UBYTE ubgFQLinkIndex) {
#if(!RDT && !BURNER)
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	U32 ulTargetPlaneIndex = FQ->ulFEntry >> gub4kEntrysPerPlaneLog;


	if (!ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT) {

		Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);
		guwRSPlaneIndex = RS_FLAFAIL.uwPlaneOffset;

		//guwRSPlaneIndex = (ulTargetPlaneIndex - ((ulTargetPlaneIndex >> gubRSPlanesPerPageLog) << gubRSPlanesPerPageLog));
		//guwRSPlaneIndex += (guwRSPlanesPerPage * ((ulTargetPlaneIndex / (guwRSPlanesPerPage * USERDATA_RS_Group_Num)) & 1));
		//guwRSPlaneIndex += (ubIdleTagNum * (ulTargetPlaneIndex / uwRSGroupMaxEncodePlaneNum));
	}
#endif
}

void RS_Recover(UBYTE ubgFQLinkIndex, UBYTE ubDepth) {
#if(!RDT && !BURNER)
	M_CheckBurnerModeToDisableFunction();
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	volatile REG_t *NonUncChannelFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (((ubChannel + 1) & 1) * 0x200));
	UBYTE ubMTerrorCheckInRSTwoCh = 0; // all channel MT error stop?
	if (gulParamPageInfo.ONFI.btNandSupplyByEmulator) {
		UartString("\nRS_Recover");
	}


	if (ENABLE_DEBUG_SBC_HANG_ON || (gubEnterSB == 0)) {
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
		// check another channel autopol status
		if (((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (ANY_BUSY | FPU_TRIG)) != 0) && (NonUncChannelFREG[FCTLL_INT_RDY].B.B0 != 0xFF)
		        && (!(NonUncChannelFREG[FCTLL_INT_INF].B.B0 & INTR_BUSY))) {
			UBYTE ubAutopolDone = 0;
			U32 ulFCE_ENB;
			U32 ulTimeStart = gulOperationTime;
			while (1) {
				if (NonUncChannelFREG[FCTLL_INT_INF].B.B0 & INTR_BUSY) {
					// polling done
					ubAutopolDone = 1;
					break;
				}
				if (((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (MT_BUSY | ANY_BUSY | SIGNOFF_BUSY | FPU_TRIG)) == 0) && (NonUncChannelFREG[FCTLL_INT_RDY].B.B0 == 0xFF)) {
					// polling done
					ubAutopolDone = 1;
					break;
				}
#if FORCE_AUTOPOL_TIMEOUT
				// Check autopol timeout FPU sequence
				if (NonUncChannelFREG[FCTLL_FPU_ENTRY].L != FPU_PTR_C70_DAT_MK00)
#else
				// Check autopol timeout FPU sequence
				if ((NonUncChannelFREG[FCTLL_FPU_ENTRY].L != FPU_PTR_C70_DAT_MK20) && (NonUncChannelFREG[FCTLL_FPU_ENTRY].L != FPU_PTR_C70_DAT_MK40))
#endif
				{
					// polling done & FPU is changed
					ubAutopolDone = 1;
					break;
				}
				xthal_dcache_line_invalidate(&gulOperationTime);
				if (((gulOperationTime >= ulTimeStart) && ((gulOperationTime - ulTimeStart) > AUTOPOL_TIMEOUT_MILLISEC)) || ((gulOperationTime < ulTimeStart) && ((0xFFFFFFFF - ulTimeStart + gulOperationTime) > AUTOPOL_TIMEOUT_MILLISEC))) {
					//timeout occur... get current CE ENB
					ulFCE_ENB = FCONL[FCONL_FCE_ENB];
					if (ulFCE_ENB != 0) {
						break;
					}
					// get CE ENB timeout
					M_AssertError((gulOperationTime - ulTimeStart) > (2 * AUTOPOL_TIMEOUT_MILLISEC));
				}
			}

			if (ubAutopolDone == 0) {
#if UART_AUTOPOL_TIMEOUT_HANDLING
				UartString("\nMTStop1:");
				UartLongHex(ulFCE_ENB);
#endif
				flaReadFailRetryAutoPollTimeoutHandling(ubgFQLinkIndex, ulFCE_ENB, 3);
			}
		}
#endif

		// abort all channel for RS
		NonUncChannelFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);
		//NonUncChannelFREG[FCTLL_INT_CFG].L |= FORCE_DMA_ABORT; // follow FLH RegSpec 4.19 MTQ Manual Abort
		while ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) { // already push MT also sure to done
			if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
				break;
			}
		}

		// 20151006 Wait pre load MT done for no INT in C70 POL bug
		UBYTE ubDummyRead = RSB[RSB_NMODE_IDX_0];
		ubDummyRead = RSB[RSB_NMODE_IDX_1];
		ubDummyRead = RSB[RSB_NMODE_IDX_2];
		ubDummyRead = RSB[RSB_NMODE_IDX_3];
		ubDummyRead = RSB[RSB_NMODE_IDX_4];
		while ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) { // pre load MT also sure to done
			if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
				break;
			}
		}

		// wait the channel true idle when force empty
		NonUncChannelFREG[FCTLL_DBG_INF].L = (0x0C);
		while ((GET_DBG_MTQ_STS(NonUncChannelFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(NonUncChannelFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2)) {
			if ((NonUncChannelFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
				break;
			}
		}
	}

	// Fill RS recover setting
	gFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
	gFREG[FCTLL_ZIP_CFG].L &= (~(RING_MODE | L4K_BADR_CONTINUOUS_MODE));
	gFREG[FCTLL_RS_CFG].L &= (~RS_XOR_ENCODE); // 20150916 fix decode fail
	if (ENABLE_DEBUG_HW_FORCEREADFAIL && gubRetryGenFailPhase < 4) {
		gFREG[FCTLL_UNC_CFG].L = 0;
	}

	NonUncChannelFREG[FCTLL_INT_CFG].L &= (~ABORT_DONE_INTR_EN); // follow FLH RegSpec 4.19 MTQ Manual Abort
	NonUncChannelFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN | RS_XOR_ENCODE));  // 20150916 fix decode fail
	NonUncChannelFREG[FCTLL_ZIP_CFG].L &= (~(RING_MODE | L4K_BADR_CONTINUOUS_MODE));
	NonUncChannelFREG[FCTLL_CMP_CFG].L &= (~LCA_FW_COMPARE_EN);
	NonUncChannelFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);

	NonUncChannelFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
	NonUncChannelFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
	NonUncChannelFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
	gubINTTable[ubDepth].All = 0;


	if ((NonUncChannelFREG[FCTLL_INT_INF].L & INTR_BUSY)/* && (NonUncChannelFREG[FCTLL_MAP_CFG].L & (CURRENT_FIFO_HAS_NEW | NEXT_FIFO_HAS_NEW))*/) {

		UBYTE ubFIFOInfoCnt = 0, ubTempFIFOScanIndex = 0;

		ubFIFOInfoCnt = (NonUncChannelFREG[FCTLL_MAP_CFG].L & NEXT_FIFO_HAS_NEW) ? 2 : 1;

		if (ENABLE_DEBUG_UART_CC) {
			UartString("\n NonUNC FIFO Cnt : ");
			Uart_Tx_DataHex(ubFIFOInfoCnt);
		}

		for (ubTempFIFOScanIndex = 0; ubTempFIFOScanIndex < ubFIFOInfoCnt; ubTempFIFOScanIndex++) {

			ubMTerrorCheckInRSTwoCh = 1;

			NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
			NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(INT_VCT_MAP);
			UBYTE ubMTerrorDepth = NonUncChannelFREG[FCTLL_MAP_INF].B.B0;
			UBYTE ubMaxErrorFrameNum = 0;
			UBYTE ubSignOffMaskFail = 0;
			UWORD uwSignOffMaskFailMap = 0;
			if (NonUncChannelFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(SIGNOFFMASK_MAP);
				uwSignOffMaskFailMap = NonUncChannelFREG[FCTLL_MAP_INF].W.W0;
				ubSignOffMaskFail = 1;
			}

			if (NonUncChannelFREG[FCTLL_INT_INF].L & STA_ERR_INF) { // 1st priority due to other map also setup at the same time of HW bug
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(STA_ERR_MAP);
				ubMaxErrorFrameNum = 1; // STA
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & LCA_FW_CMP_ERR_INF) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(LCA_FW_CMP_FAIL_MAP);
				ubMaxErrorFrameNum = 16; // Logi
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & DECOMPRESSION_FAIL_INF) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(DECOMPRESSION_FAIL_MAP);
				ubMaxErrorFrameNum = 16; // Logi
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & CRC16_ERROR) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(CRC16_ERR_MAP);
				ubMaxErrorFrameNum = 32; // Logi
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & FW_SETTING_ERR_INF) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(FW_SETTING_ERR_MAP);
				ubMaxErrorFrameNum = 6; // Zip
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & UNC_MAP_INF) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(UNC_MAP);
				ubMaxErrorFrameNum = 4; // Phy
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & CRC32_ERROR) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(CRC32_ERR_MAP);
				ubMaxErrorFrameNum = 4; // Phy
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & SIGNOFFMASK_INF) {
				ubMaxErrorFrameNum = 16; // Logi
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & ERASE_PAGE) {
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & E3D_ERR_INF) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(E3D_ERR_MAP);
				ubMaxErrorFrameNum = 16; // Logi
			}
			else if (NonUncChannelFREG[FCTLL_INT_INF].L & COMPRESSION_FAIL_INF) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L &= CLEAR_PINGPONG_FIFO_FRAMEMAP;
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= PINGPONG_FIFO_FRAMEMAP_SEL(COMPRESSION_FAIL_MAP);
				ubMaxErrorFrameNum = 16; // Logi
			}
			else {
				// over ecc, PFA not turn on now
				if (ENABLE_WP_DEBUG) {
					flaLEDBlink(0xEE);
				}
				else {
					M_AssertError(1);
					gubWP_DEBUG = 0xEE;
				}
			}

			UBYTE ubFirstError4KIndex = 0;
			M_Get_Error_4K_Index(ubFirstError4KIndex, ubMaxErrorFrameNum, NonUncChannelFREG[FCTLL_MAP_INF].L);

			// Find the UNC FQ
			UBYTE ubFQLinkIndex, ubFQCount = 0;
			FlashQueue_t *FQTemp;
			for (ubFQLinkIndex = gFQI.ubFQLinkFirst; ubFQCount < gFQI.ubFQLinkNumber; (ubFQLinkIndex = FQTemp->ubNext)) {
				FQTemp = &gFQI.gFQLink[ubFQLinkIndex];
				if ((FQTemp->ubDepth == ubMTerrorDepth) || (FQTemp->ubRelativeDepth == ubMTerrorDepth)) {

					if (ENABLE_DEBUG_UART_CC) {
						UartString(" Found!");
					}

					FQTemp->ulFIPError.All = NonUncChannelFREG[FCTLL_INT_INF].L;
					FQTemp->btUncCheckInRSTwoCh = 1;
					if (ubSignOffMaskFail) { // For SignOffMask error map (user data)
						FQTemp->ulFIPError.W.uwSignoffMaskFailLog = uwSignOffMaskFailMap;
					}
					FQTemp->ubFirstUncFrameNoMapInRSTwoCh = ubFirstError4KIndex;
					break;
				}
				ubFQCount++;
			}

			if ((ubTempFIFOScanIndex == 0) && (ubFIFOInfoCnt == 2)) {
				NonUncChannelFREG[FCTLL_MAP_CFG].L |= SWITCH_PINGPONG_FIFO_TO_NEXT_FRAME_MAP;
			}
		}
		//NonUncChannelFREG[FCTLL_INT_INF].L &= (MAP_FIFO_POINTER_RESET);
		NonUncChannelFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	}

	if (ENABLE_MULTIPLANE_PROGRAM) {
		// fix erase page bug  20150710
		RS_Flush_MultiPlaneProgram(ubDepth);
	}

	UBYTE ubPARITY_GROUP_NUM = USERDATA_RS_Group_Num;
	if (FQ->FQPara.B.btRWTableUnit) {
		VT->gulRWTableUnitRetryFailEnterRS++;
		ubPARITY_GROUP_NUM = Table_RS_Group_Num;
	}
	UBYTE ubParityPlaneNum;
	U32 ulTargetPlaneIndex, uli;
	UWORD uwDataPlaneNum, uwTotalRSPlaneNum;
	ulTargetPlaneIndex = FQ->ulFEntry >> gub4kEntrysPerPlaneLog;

	// Calculate one IdleTag numbers in a group
	M_AssertCriticalError((guwRSPlanesPerPage == 0), VT_ASSERT_RS_RECOVER_0x41);
	UBYTE ubIdleTagNum = 0;
	ubIdleTagNum = (guwRSPlanesPerPage - (PARITY_DATA_PAGE_NUM % guwRSPlanesPerPage));

	// Calculate total plane numbers in a group with IdleTag
	UWORD uwRSGroupMaxEncodePlaneNum = ((PARITY_DATA_PAGE_NUM + ubIdleTagNum) * USERDATA_RS_Group_Num);




	UBYTE ubRSFrameIndex;
	if (FQ->FQPara.B.btRWTableUnit) {
		ubRSFrameIndex = (M_Divider(ulTargetPlaneIndex, DEF_TableParityPlanes_Mcl_Div)) & (ubPARITY_GROUP_NUM - 1);
	}
	else {
		if (!ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT) {

			Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);
			ubRSFrameIndex = RS.ubTagMAP[RS_FLAFAIL.ubGroupIndex][RS_FLAFAIL.ubTagIndex];

		}
		else {
			ubRSFrameIndex = (M_Divider(ulTargetPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) & (ubPARITY_GROUP_NUM - 1);
		}
	}
	UBYTE ubParityInRAM = 0; // Encode done but not program parity (20150411)
	gubNotEncodeDone = 0;
	gsTableRSErrorFrameBitMap.ulAll = 0;
	uwDataPlaneNum = guwParityPlanesNum - 2;
	ubParityPlaneNum = 2;
	UWORD uwDataParityDummyPlanesNum = guwParityPlanesNum;
	if (ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT) {
		UBYTE ubDummyPlanesNum = 0;
		if (ENABLE_USE_255RS_DPAGE) {
			// uwDataPlaneNum + 2 parity pages + 1 dummy page
			ubDummyPlanesNum = 1;
			uwDataParityDummyPlanesNum = uwDataPlaneNum + ubParityPlaneNum + ubDummyPlanesNum;
		}
	}

	if (ENABLE_DEBUG_UART_FAIL || TLC_NES_TEST) {
		UartString("\nRS");
	}
	U32 ulPhyPlaneIndex;

	if ((VT->gubGRTargetIndex < 16) && (FQ->uwFUnit == VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget)) {

		VT->gsReadFailDebugCount.gulRSErrorInWrittingDB_DebugCount++;

		if (!ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT) {
			if (((gulRSEncodeStartPlaneNo[ubRSFrameIndex] == 0xFFFFFFFF) && ulTargetPlaneIndex <= VT->gulGRTargetPTR) ||
			        (ulTargetPlaneIndex >= gulRSEncodeStartPlaneNo[ubRSFrameIndex])) {
				gubNotEncodeDone = 1;
				RSL[RSL_TAG_SEL] = ubRSFrameIndex;
				while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubRSFrameIndex);
				UBYTE ubEncodedDPageNum = RSB[RSB_TAG_PEC];
				if (ubEncodedDPageNum == 0) {
					gubNotEncodeDone = 0;

					U32 ulPlanesPerUnitThisBlock = gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.btFastPage ? guwFastPagePlanesPerUnit : gulPlanesPerUnit;

					// here ulPhyPlaneIndex is checking start index
					ulPhyPlaneIndex = (ulPlanesPerUnitThisBlock == gulPlanesPerUnit) ? RS.ulLastPlaneIndex[RS.ub1stEndLastGroup] : RS.ulLastPlaneIndexFastPage[RS.ub1stEndLastGroupFastPage];
					//someone is just encode done and locate at the end of unit, we can not get rs data plane number by PEC, need calculating
					if (VT->gulGRTargetPTR >= ulPhyPlaneIndex) {
						UBYTE ubTargetTagIndex;
						// Get group index of target plane index, use to find last plane index
						Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);
						ubTargetTagIndex = RS_FLAFAIL.ubTagIndex;
						// To generate start plane index and plane offset of last plane index
						if (ulPlanesPerUnitThisBlock == guwFastPagePlanesPerUnit) {
							ulPhyPlaneIndex = RS.ulLastPlaneIndexFastPage[RS_FLAFAIL.ubGroupIndex] - 1;
						}
						else {
							ulPhyPlaneIndex = RS.ulLastPlaneIndex[RS_FLAFAIL.ubGroupIndex] - 1;
						}
						Generate_RS_MAP_Index_by_Plane_flafail(ulPhyPlaneIndex);
						if (ubTargetTagIndex == RS_FLAFAIL.ubTagIndex) {
							M_AssertError(RS_FLAFAIL.uwPlaneOffset < 1);
							// uwPlaneOffset of last plane minus 1 is uwDataPlaneNum
							uwDataPlaneNum = RS_FLAFAIL.uwPlaneOffset - 1;
						}
					}

					//gulRSErrorInSameArea_EncodeDone_WrittingDB_DebugCount++;
					VT->gsReadFailDebugCount.gulRSErrorInWrittingDB_RSREG_JUDGE_ENCCNT_DONE_DebugCount++;
				}
				else {
					VT->gsReadFailDebugCount.gulRSErrorInWrittingDB_RSREG_JUDGE_ENCCNT_NOTDONE_DebugCount++;
					//gulRSErrorInSameArea_NotEncodeDone_WrittingDB_DebugCount++;
				}
				//UartString("\nDecode Writting DB\n");
			}
		}
		else {
			if ((ulTargetPlaneIndex / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) == (VT->gulGRTargetPTR / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM))) {
				gubNotEncodeDone = 1;
				RSL[RSL_TAG_SEL] = ubRSFrameIndex;
				while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubRSFrameIndex);
				UBYTE ubEncodedDPageNum = RSB[RSB_TAG_PEC];
				if (ubEncodedDPageNum == 0) {
					gubNotEncodeDone = 0;
					//gulRSErrorInSameArea_EncodeDone_WrittingDB_DebugCount++;
				}
				else {
					//gulRSErrorInSameArea_NotEncodeDone_WrittingDB_DebugCount++;
				}
				//UartString("\nDecode Writting DB\n");
			}
			else if (((VT->gulGRTargetPTR / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) > 0) && ((ulTargetPlaneIndex / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) == ((VT->gulGRTargetPTR / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) - 1))) {
				//check remain FQ have parity unprog or not
				UBYTE ubFQLinkIndex, ubFQCount = 0;
				FlashQueue_t *FQTemp;
				for (ubFQLinkIndex = gFQI.ubFQLinkFirst; ubFQCount < gFQI.ubFQLinkNumber; (ubFQLinkIndex = FQTemp->ubNext)) {
					FQTemp = &gFQI.gFQLink[ubFQLinkIndex];
					if ((FQTemp->ubFJob == BYTE_FJOB_WRITE)/* && (FQTemp->ubFQPhase != 0) */ && (FQTemp->ubRSFrameIndex == ubRSFrameIndex) && (FQTemp->btParity)) {
						if (((FQTemp->ulFEntry >> gub4kEntrysPerPlaneLog) / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) == (ulTargetPlaneIndex / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM))) {
							if ((gubINTTable[FQTemp->ubRelativeDepth].All & 0xFA) != 0) {
								if (ENABLE_WP_DEBUG) {
									flaLEDBlink(0xED);
								}
								else {
									gubWP_DEBUG = 0xED;
									M_AssertError(1);
								}
							}
							if (gubINTTable[FQTemp->ubRelativeDepth].B.btAll_Done == 0) {
								if (gubINTTable[FQTemp->ubRelativeDepth].B.btDMA_Done) {
									if (ENABLE_WP_DEBUG) {
										flaLEDBlink(0xEC);
									}
									else {
										gubWP_DEBUG = 0xEC;
										M_AssertError(1);
									}
								}
								// not prog parity yet
								ubParityInRAM = 1;
							}
						}
					}
					ubFQCount++;
				}
				if (ubParityInRAM) {
					//gulRSErrorInLastArea_ParityInRAM_WrittingDB_DebugCount++;
					gubNotEncodeDone = 1;
				}
				else {
					//gulRSErrorInLastArea_ParityNotInRAM_WrittingDB_DebugCount++;
					gubNotEncodeDone = 0;
				}
				//UartString("\nDecode Writting DB 2\n");
			}
			else {
				//gulRSErrorInLastTwoAreaAgo_WrittingDB_DebugCount++;
				//UartString("\nDecode static BLK\n");
			}
		}

	}
	else if ((VT->gubGCGRTargetIndex < MAX_GCGRUNIT) && (FQ->uwFUnit == VT->guwGCGRTarget[VT->gubGCGRTargetIndex].B.uwTarget)) {

		VT->gsReadFailDebugCount.gulRSErrorInCopingGB_DebugCount++;
		ubRSFrameIndex += GC_RSFrameStartIndex;

		if (!ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT) {
			if (((gulRSEncodeStartPlaneNo[ubRSFrameIndex] == 0xFFFFFFFF) && ulTargetPlaneIndex <= VT->gulGCGRTargetPTR) ||
			        (ulTargetPlaneIndex >= gulRSEncodeStartPlaneNo[ubRSFrameIndex])) {
				gubNotEncodeDone = 1;
				RSL[RSL_TAG_SEL] = ubRSFrameIndex;
				while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubRSFrameIndex);
				UBYTE ubEncodedDPageNum = RSB[RSB_TAG_PEC];
				if (ubEncodedDPageNum == 0) {
					gubNotEncodeDone = 0;

					U32 ulPlanesPerUnitThisBlock = gulVC[VT->guwGCGRTarget[VT->gubGCGRTargetIndex].B.uwTarget].B.btFastPage ? guwFastPagePlanesPerUnit : gulPlanesPerUnit;

					if (VT->guwGCGRTarget[VT->gubGCGRTargetIndex].B.uwTarget != Default_Unit) {
						if (VT->guwGCGRTarget[VT->gubGCGRTargetIndex].B.uwTarget >= 4096) {
							ulPlanesPerUnitThisBlock = gubFastPageForCopyFromRUT ? guwFastPagePlanesPerUnit : gulPlanesPerUnit;
						}
					}

					// here ulPhyPlaneIndex is checking start index
					ulPhyPlaneIndex = (ulPlanesPerUnitThisBlock == gulPlanesPerUnit) ? RS.ulLastPlaneIndex[RS.ub1stEndLastGroup] : RS.ulLastPlaneIndexFastPage[RS.ub1stEndLastGroupFastPage];
					//someone is just encode done and locate at the end of unit, we can not get rs data plane number by PEC, need calculating
					if (VT->gulGCGRTargetPTR >= ulPhyPlaneIndex) {
						UBYTE ubTargetTagIndex;
						// Get group index of target plane index, use to find last plane index
						Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);
						ubTargetTagIndex = RS_FLAFAIL.ubTagIndex;

						// To generate start plane index and plane offset of last plane index
						if (ulPlanesPerUnitThisBlock == guwFastPagePlanesPerUnit) {
							ulPhyPlaneIndex = RS.ulLastPlaneIndexFastPage[RS_FLAFAIL.ubGroupIndex] - 1;
						}
						else {
							ulPhyPlaneIndex = RS.ulLastPlaneIndex[RS_FLAFAIL.ubGroupIndex] - 1;
						}
						Generate_RS_MAP_Index_by_Plane_flafail(ulPhyPlaneIndex);
						if (ubTargetTagIndex == RS_FLAFAIL.ubTagIndex) {
							M_AssertError(RS_FLAFAIL.uwPlaneOffset < 1);
							// uwPlaneOffset of last plane minus 1 is uwDataPlaneNum
							uwDataPlaneNum = RS_FLAFAIL.uwPlaneOffset - 1;
						}
					}

					VT->gsReadFailDebugCount.gulRSErrorInCopingGB_RSREG_JUDGE_ENCCNT_DONE_DebugCount++;
					//gulRSErrorInSameArea_EncodeDone_CopingGB_DebugCount++;
				}
				else {
					VT->gsReadFailDebugCount.gulRSErrorInCopingGB_RSREG_JUDGE_ENCCNT_NOTDONE_DebugCount++;
					//gulRSErrorInSameArea_NotEncodeDone_CopingGB_DebugCount++;
				}
				//UartString("\nDecode Coping DB\n");
			}
		}
		else {
			if ((ulTargetPlaneIndex / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) == (VT->gulGCGRTargetPTR / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM))) {
				gubNotEncodeDone = 1;
				RSL[RSL_TAG_SEL] = ubRSFrameIndex;
				while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubRSFrameIndex);
				UBYTE ubEncodedDPageNum = RSB[RSB_TAG_PEC];
				if (ubEncodedDPageNum == 0) {
					gubNotEncodeDone = 0;
					//gulRSErrorInSameArea_EncodeDone_CopingGB_DebugCount++;
				}
				else {
					//gulRSErrorInSameArea_NotEncodeDone_CopingGB_DebugCount++;
				}
				//UartString("\nDecode Coping GB\n");
			}
			else if (((VT->gulGCGRTargetPTR / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) > 0) && ((ulTargetPlaneIndex / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) == ((VT->gulGCGRTargetPTR / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) - 1))) {
				//check remain FQ have parity unprog or not
				UBYTE ubFQLinkIndex, ubFQCount = 0;
				FlashQueue_t *FQTemp;
				for (ubFQLinkIndex = gFQI.ubFQLinkFirst; ubFQCount < gFQI.ubFQLinkNumber; (ubFQLinkIndex = FQTemp->ubNext)) {
					FQTemp = &gFQI.gFQLink[ubFQLinkIndex];
					if ((FQTemp->ubFJob == BYTE_FJOB_WRITE)/* && (FQTemp->ubFQPhase != 0)*/ && (FQTemp->ubRSFrameIndex == ubRSFrameIndex) && (FQTemp->btParity)) {
						if (((FQTemp->ulFEntry >> gub4kEntrysPerPlaneLog) / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) == (ulTargetPlaneIndex / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM))) {
							if ((gubINTTable[FQTemp->ubRelativeDepth].All & 0xFA) != 0) {
								if (ENABLE_WP_DEBUG) {
									flaLEDBlink(0xEB);
								}
								else {
									gubWP_DEBUG = 0xEB;
									M_AssertError(1);
								}
							}
							if (gubINTTable[FQTemp->ubRelativeDepth].B.btAll_Done == 0) {
								if (gubINTTable[FQTemp->ubRelativeDepth].B.btDMA_Done) {
									if (ENABLE_WP_DEBUG) {
										flaLEDBlink(0xEA);
									}
									else {
										gubWP_DEBUG = 0xEA;
										M_AssertError(1);
									}
								}
								// not prog parity yet
								ubParityInRAM = 1;
							}
						}
					}
					ubFQCount++;
				}
				if (ubParityInRAM) {
					//gulRSErrorInLastArea_ParityInRAM_CopingGB_DebugCount++;
					gubNotEncodeDone = 1;
				}
				else {
					//gulRSErrorInLastArea_ParityNotInRAM_CopingGB_DebugCount++;
					gubNotEncodeDone = 0;
				}

				//UartString("\nDecode Coping GB 2\n");
			}
			else {
				//gulRSErrorInLastTwoAreaAgo_CopingGB_DebugCount++;
				//UartString("\nDecode static BLK\n");
			}
		}
	}
	else if (FQ->FQPara.B.btRWTableUnit) {

		uwDataPlaneNum = guwTableParityPlanesNum - 1;
		ubParityPlaneNum = 1;
		uwDataParityDummyPlanesNum = guwTableParityPlanesNum;
		ubRSFrameIndex += Table_RSFrameStartIndex;

		// check Table
		UWORD uwActiveTableTarget;
		if (VT->gulFTLState.B.btNeedCloseTableTarget) {
			uwActiveTableTarget = VT->guwCloseTableTarget.B.uwTarget;
		}
		else {
			uwActiveTableTarget = VT->guwTableTarget[VT->gubTableTargetIndex].B.uwTarget;
		}
		if (FQ->uwFUnit == uwActiveTableTarget) {

			if ((ulTargetPlaneIndex / (guwTableParityPlanesNum * ubPARITY_GROUP_NUM)) == (VT->guwTableTargetPTR / (guwTableParityPlanesNum * ubPARITY_GROUP_NUM))) {
				gubNotEncodeDone = 1;
				RSL[RSL_TAG_SEL] = ubRSFrameIndex;
				while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubRSFrameIndex);
				UBYTE ubEncodedDPageNum = RSB[RSB_TAG_PEC];
				if (ubEncodedDPageNum == 0) {
					gubNotEncodeDone = 0;
					VT->gsReadFailDebugCount.gulRSErrorInSameArea_EncodeDone_WrittingTB_DebugCount++;
				}
				else {
					VT->gsReadFailDebugCount.gulRSErrorInSameArea_NotEncodeDone_WrittingTB_DebugCount++;
				}
				//UartString("\nDecode Writting TB\n");
			}
			else if (((VT->guwTableTargetPTR / (guwTableParityPlanesNum * ubPARITY_GROUP_NUM)) > 0) && ((ulTargetPlaneIndex / (guwTableParityPlanesNum * ubPARITY_GROUP_NUM)) == ((VT->guwTableTargetPTR / (guwTableParityPlanesNum * ubPARITY_GROUP_NUM)) - 1))) {
				//check remain FQ have parity unprog or not
				UBYTE ubFQLinkIndex, ubFQCount = 0;
				FlashQueue_t *FQTemp;
				for (ubFQLinkIndex = gFQI.ubFQLinkFirst; ubFQCount < gFQI.ubFQLinkNumber; (ubFQLinkIndex = FQTemp->ubNext)) {
					FQTemp = &gFQI.gFQLink[ubFQLinkIndex];
					if ((FQTemp->ubFJob == BYTE_FJOB_WRITE)/* && (FQTemp->ubFQPhase != 0)*/ && (FQTemp->ubRSFrameIndex == ubRSFrameIndex) && (FQTemp->btParity)) {
						if (((FQTemp->ulFEntry >> gub4kEntrysPerPlaneLog) / (guwTableParityPlanesNum * ubPARITY_GROUP_NUM)) == (ulTargetPlaneIndex / (guwTableParityPlanesNum * ubPARITY_GROUP_NUM))) {
							if ((gubINTTable[FQTemp->ubRelativeDepth].All & 0xFA) != 0) {
								if (ENABLE_WP_DEBUG) {
									flaLEDBlink(0xE9);
								}
								else {
									gubWP_DEBUG = 0xE9;
									M_AssertError(1);
								}
							}
							if (gubINTTable[FQTemp->ubRelativeDepth].B.btAll_Done == 0) {
								if (gubINTTable[FQTemp->ubRelativeDepth].B.btDMA_Done) {
									if (ENABLE_WP_DEBUG) {
										flaLEDBlink(0xE8);
									}
									else {
										gubWP_DEBUG = 0xE8;
										M_AssertError(1);
									}
								}
								// not prog parity yet
								ubParityInRAM = 1;
							}
						}
					}
					ubFQCount++;
				}
				if (ubParityInRAM) {
					VT->gsReadFailDebugCount.gulRSErrorInLastArea_ParityInRAM_WrittingTB_DebugCount++;
					gubNotEncodeDone = 1;
				}
				else {
					VT->gsReadFailDebugCount.gulRSErrorInLastArea_ParityNotInRAM_WrittingTB_DebugCount++;
					gubNotEncodeDone = 0;
				}

				//UartString("\nDecode Writting TB 2\n");
			}
			else {
				VT->gsReadFailDebugCount.gulRSErrorInLastTwoAreaAgo_WrittingTB_DebugCount++;
				//UartString("\nDecode static BLK\n");
			}

		}
		else {
			VT->gsReadFailDebugCount.gulRSErrorInNotUsingBlock_StaticTB_DebugCount++;
		}
	}
	else {
		U32 ulPlanesPerUnitThisBlock = (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) ? guwFastPagePlanesPerUnit : gulPlanesPerUnit;

		U32 ulListIndex;
		ulPhyPlaneIndex = (FQ->uwFJobInfo & BIT_FJOBI_FASTPAGE) ? (RS.ulLastPlaneIndexFastPage[RS.ub1stStartLastGroupFastPage] - 1) : (RS.ulLastPlaneIndex[RS.ub1stStartLastGroup] - 1);
		// get tag index of  last plane index of 1st last group
		Generate_RS_MAP_Index_by_Plane_flafail(ulPhyPlaneIndex);

		// total encode page count before this ubTagIndex / uwPlanesPerGroup = start page link index
		ulListIndex = (RS_FLAFAIL.ubTagIndex * PARITY_DATA_PAGE_NUM) / RS.uwPlanesPerGroup;

		M_AssertError(ulListIndex >= RS_MAX_LINK_COUNT_PER_GROUP);
		M_AssertError(RS_FLAFAIL.ulPageLinkList[RS_FLAFAIL.ubGroupIndex][ulListIndex] == 0xFFFFFFFF);

		// find start plane index of 1st last plane group, and need to calculate uwDataPlaneNum again after this index
		ulPhyPlaneIndex = RS_FLAFAIL.ulPageLinkList[RS_FLAFAIL.ubGroupIndex][ulListIndex] * RS.uwPlanesPerGroup;

		if ( ulTargetPlaneIndex >= ulPhyPlaneIndex) {
			UBYTE ubTargetTagIndex;
			// Get group index of target plane index, use to find last plane index
			Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);
			ubTargetTagIndex = RS_FLAFAIL.ubTagIndex;
			// To generate start plane index and plane offset of last plane index
			if (ulPlanesPerUnitThisBlock == guwFastPagePlanesPerUnit) {
				ulPhyPlaneIndex = RS.ulLastPlaneIndexFastPage[RS_FLAFAIL.ubGroupIndex] - 1;
			}
			else {
				ulPhyPlaneIndex = RS.ulLastPlaneIndex[RS_FLAFAIL.ubGroupIndex] - 1;
			}
			Generate_RS_MAP_Index_by_Plane_flafail(ulPhyPlaneIndex);
			if (ubTargetTagIndex == RS_FLAFAIL.ubTagIndex) {
				M_AssertError(RS_FLAFAIL.uwPlaneOffset < 1);
				// uwPlaneOffset of last plane minus 1 is uwDataPlaneNum
				uwDataPlaneNum = RS_FLAFAIL.uwPlaneOffset - 1;
			}
		}

		VT->gsReadFailDebugCount.gulRSErrorInNotUsingBlock_StaticBLK_DebugCount++;
		//gulRSErrorInNotUsingBlock_StaticBLK_DebugCount++;
		//uwDataPlaneNum = guwParityPlanesNum - 2;
		//ubParityPlaneNum = 2;
		//UartString("\nDecode static BLK\n");
	}
	uwTotalRSPlaneNum = uwDataPlaneNum + ubParityPlaneNum;
	if (ENABLE_WP_DEBUG && (uwDataPlaneNum > 253)) {
		flaLEDBlink(1);
	}
	else {
		M_AssertError(uwDataPlaneNum > 253);
	}

	UWORD uwRSPlaneIndex;
	if (FQ->FQPara.B.btRWTableUnit) {
		uwRSPlaneIndex = (M_Modular(ulTargetPlaneIndex, DEF_TableParityPlanes_Mod_Mask));
	}
	else {
		if (ENABLE_RS_NORMAL_MODE && ENABLE_SAVETIME_RS_NORMAL_MODE && guwRSPlaneIndex != 0xFFFE) {
			uwRSPlaneIndex = guwRSPlaneIndex;
		}
		else {
			if (!ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT) {

				Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);
				uwRSPlaneIndex = RS_FLAFAIL.uwPlaneOffset;

				//uwRSPlaneIndex = (ulTargetPlaneIndex - ((ulTargetPlaneIndex >> gubRSPlanesPerPageLog) << gubRSPlanesPerPageLog));
				//uwRSPlaneIndex += (guwRSPlanesPerPage * ((ulTargetPlaneIndex / (guwRSPlanesPerPage * USERDATA_RS_Group_Num)) & 1));
				//uwRSPlaneIndex += (ubIdleTagNum * (ulTargetPlaneIndex / uwRSGroupMaxEncodePlaneNum));
			}
			else {
				uwRSPlaneIndex = ((M_Divider((ulTargetPlaneIndex % (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM )), DEF_PlanesPerPage_Mcl_Div)) / (ubPARITY_GROUP_NUM));
				uwRSPlaneIndex = (M_Multiplier((U32)uwRSPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (M_Modular(ulTargetPlaneIndex, DEF_PlanesPerPage_Mod_Mask));
			}
		}
	}

	gubRSErrorNum = 0;
	if (ENABLE_RS_NORMAL_MODE && ENABLE_RS_NORMAL_MODE_FPUCOR_BY_FRAME) {
		gubRSErrorPlaneUNCFrameMap[0] = (gulReadFail_4KMAP & 0xF);
	}
	guwRSErrorPlaneIndex[gubRSErrorNum++] = uwRSPlaneIndex;
	if (FQ->FQPara.B.btRWTableUnit) {
		if (ENABLE_RS_NORMAL_MODE && ENABLE_RS_NORMAL_MODE_FPUCOR_BY_FRAME) {
			gubRSErrorPlaneUNCFrameMap[1] = (0xF);
		}
		guwRSErrorPlaneIndex[gubRSErrorNum++] = (uwTotalRSPlaneNum); // error to 2nd parity
	}
	UWORD uwErrorPageIndex2 = 0; // only test
	if (ENABLE_DEBUG_RS_MAX_ERROR) {
		uwErrorPageIndex2 = getIntRandValue(uwTotalRSPlaneNum); // only tests
		//uwErrorPageIndex2 = 0;//(uwTotalRSPlaneNum-1);
		if (!FQ->FQPara.B.btRWTableUnit) {
			RSL[RSL_TAG_SEL] = ubRSFrameIndex;
			while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubRSFrameIndex);
			if (((gubNotEncodeDone == 0) && (PARITY_DATA_PAGE_NUM > 3)) || (gubNotEncodeDone && (RSB[RSB_TAG_PEC] > 1))) {
				while (uwErrorPageIndex2 == uwRSPlaneIndex) {
					uwErrorPageIndex2 = getIntRandValue(uwTotalRSPlaneNum);
				}
			}
			UartString("\nErrP2: ");
			UartWordHex(uwErrorPageIndex2);
			UartString("\n");
		}
	}

	SaveRSparity(0);
	if (!ENABLE_DEBUG_PBUF_TEMP_CLEAR) {
		RSL[RSL_IP_CTL] |= (RS_ST_CLEAR);
		RSL[RSL_IP_CTL] &= (~RS_ST_CLEAR);
	}
	volatile U32 * pBPT = RSL_BPT_BASE_0;
	UBYTE ub4K_IDX = 0;
	U32 ulDMAAddr = 0;
	U32 ulSpareAddr = 0;
	RSL[RSL_MODE_0] |= RS_PBUF_INIT;
	RSL[RSL_MODE_0] &= (~RS_PBUF_LOAD);
	RSL[RSL_MODE_0] &= (~RS_PBUF_SAVE);
	RSL[RSL_BP_CONF] &= (~RS_OFFSET_BYPASS);
	RSL[RSL_DBASE0] = RSFrameDataDecodeStart;
	RSL[RSL_SBASE0] = RSFrameSpareDecodeStart;
	RSB[RSB_DPAGE_NUM] = 0xFD;
	if (ENABLE_RS_NORMAL_MODE && (!FQ->FQPara.B.btRWTableUnit)) {
		RSB[RSB_ERA_IDX_EN_NUM] = 0;
	}
	else {
		RSB[RSB_ERA_IDX_EN_NUM] = gubRSErrorNum;
	}
	RSB[RSB_ERA_IDX_A] = guwRSErrorPlaneIndex[0];
	if (gubRSErrorNum == 2) {
		RSB[RSB_ERA_IDX_B] = (guwRSErrorPlaneIndex[1] >= uwDataPlaneNum) ? (253 - uwDataPlaneNum + guwRSErrorPlaneIndex[1]) : guwRSErrorPlaneIndex[1];
	}
	if (ENABLE_DEBUG_RS_VERIFY_DATA) {
		gubRSDebug_NoGoldenToVerifyDATA = 0;
	}

	// conversion bypass when read data page, and enable conversion when read parity page
	FALLL[FCTLL_CHNL_SET] |= BYPASS_CONV;

	if (ENABLE_RS_NORMAL_MODE) {
		FALLL[FCTLL_IBF_CTL] |= FORCE_ZIP_COR; // normal mode maybe need
	}
	UBYTE ubBypassReadOK = 0;


	if (ENABLE_DEBUG_HW_FORCEREADFAIL) {
		NonUncChannelFREG[FCTLL_UNC_CFG].L = 0;
		if (gubRetryGenFailPhase > 4) {
			if (getIntRandValue(2) == 0) {
				NonUncChannelFREG[FCTLL_UNC_CFG].L |= (AUTOGEN_READ_FAIL_EN);
			}
			else {
				UBYTE ubLoop = 0;
				UBYTE ubLoopCount = 0;
				ubLoopCount = (getIntRandValue(4) + 1);
				while (ubLoop < ubLoopCount) {
					NonUncChannelFREG[FCTLL_UNC_CFG].L |= FORCE_BCH_UNCORRECTABLE_FRAME(1 << (getIntRandValue(4)));
					ubLoop++;
				}
			}
		}
	}

#if ENALBE_DEBUG_NES_CHANGE_ERROR_SETTING
	flaNandEmulatorVenderFPUErase(0x14, 0x14, ubDepth);
#endif
	gubRSReadingParityPage = 0;
	if (gubNotEncodeDone) {
		// rebuild encode map
		RSL[RSL_TAG_SEL] = ubRSFrameIndex;
		while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubRSFrameIndex);
		memset(gubEncodedDPageMap, 0xFF, ((PARITY_DATA_PAGE_NUM + 7) >> 3)); // void * memset ( void * ptr, int value, size_t num );
		UBYTE ubEncodedDPageNum = RSB[RSB_TAG_PEC];

		if (ENABLE_WP_DEBUG && (ubEncodedDPageNum == 0)) {
			flaLEDBlink(1);
		}
		else {
			M_AssertError(ubEncodedDPageNum == 0);
		}

		RS_CheckEncodeDPageMap(FQ->FQPara.B.btRWTableUnit, ubRSFrameIndex, ubIdleTagNum, uwRSGroupMaxEncodePlaneNum, uwDataParityDummyPlanesNum, ubPARITY_GROUP_NUM);

#if ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR
		//UWORD uwDebugRSNormalModeErrorPlaneNo[RS_MAX_ERROR_COUNT] = {0};
		UBYTE ubDebugGenRSNormalModeErrorNum = (ENABLE_DEBUG_RS_NORMAL_MODE_LARGE_ERROR_PAGE == 1) ? 12 : 3;
		if (ubEncodedDPageNum > ubDebugGenRSNormalModeErrorNum) {
			if (ENABLE_DEBUG_RS_NORMAL_MODE_UART) {
				UartString("\nDbgErrNum = ");
				Uart_Tx_DataHex(ubDebugGenRSNormalModeErrorNum);
			}
			UWORD uwRandValue = 0;
			UBYTE ubCheckSameValue = 0;
			U32 ulj = 0;
			for (uli = 0; uli < ubDebugGenRSNormalModeErrorNum; uli++) {
				uwRandValue = getIntRandValue(ubEncodedDPageNum);
				while (1) {
					ubCheckSameValue = 0;
					if (uwRandValue == uwRSPlaneIndex) {
						ubCheckSameValue = 1;
					}
					if (ubCheckSameValue == 0) {
						for (ulj = 0; ulj < uli; ulj++) {
							if ((uwRandValue == guwDebugRSNormalModeErrorPlaneNo[ulj])) {
								ubCheckSameValue = 1;
								break;
							}
						}
					}
					if (ubCheckSameValue == 0) {
						break;
					}
					uwRandValue = getIntRandValue(ubEncodedDPageNum);
				}
				guwDebugRSNormalModeErrorPlaneNo[uli] = uwRandValue;
				if (ENABLE_DEBUG_RS_NORMAL_MODE_UART) {
					UartString(" ");
					UartWordHex(guwDebugRSNormalModeErrorPlaneNo[uli]);
				}
			}
		}
#endif

		if (ENABLE_DEBUG_UART_NES_RS) {
			UartString(" Para: ");
			Uart_Tx_DataHex(FQ->FQPara.All);
			UartString(" Tag: ");
			Uart_Tx_DataHex(ubRSFrameIndex);
			UartString(" DPageNum: ");
			Uart_Tx_DataHex(ubEncodedDPageNum);
		}

		uli = 0;
		ulSpareAddr = RSFrameSpareDecodeStart;
		UWORD uwCheckEncodeCount = 0;
		while (uwCheckEncodeCount < ubEncodedDPageNum) { // only read data page
			if (gubEncodedDPageMap[uli >> 3] & (BIT0 << (uli & 7))) { // already encode

				if (FQ->FQPara.B.btRWTableUnit) {
					ulPhyPlaneIndex = (M_Multiplier((M_Divider(ulTargetPlaneIndex, DEF_TableParityPlanes_Mcl_Div)), DEF_TableParityPlanes_Mcl_Div)) + uli;
				}
				else {
					if (!ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT) {

						U32 ulListIndex;
						Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);

						// total encode count / uwPlanesPerGroup = list index
						ulListIndex = (RS_FLAFAIL.ubTagIndex * PARITY_DATA_PAGE_NUM + uli) / RS.uwPlanesPerGroup;

						M_AssertError(ulListIndex >= RS_MAX_LINK_COUNT_PER_GROUP);
						M_AssertError(RS_FLAFAIL.ulPageLinkList[RS_FLAFAIL.ubGroupIndex][ulListIndex] == 0xFFFFFFFF);

						// get start plane index
						ulPhyPlaneIndex = RS_FLAFAIL.ulPageLinkList[RS_FLAFAIL.ubGroupIndex][ulListIndex] * RS.uwPlanesPerGroup;
						// adjust to real plane offset
						ulPhyPlaneIndex += ((RS_FLAFAIL.ubTagIndex * PARITY_DATA_PAGE_NUM + uli) % RS.uwPlanesPerGroup);

					}
					else {
						ulPhyPlaneIndex = (ulTargetPlaneIndex / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) * (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM);
						ulPhyPlaneIndex += ((M_Divider(uli, DEF_PlanesPerPage_Mcl_Div)) * (gubPlanesPerPage * ubPARITY_GROUP_NUM)) + (M_Multiplier((ubRSFrameIndex & (ubPARITY_GROUP_NUM - 1)), DEF_PlanesPerPage_Mcl_Div)) + (M_Modular(uli, DEF_PlanesPerPage_Mod_Mask));
					}
				}

				if (ENABLE_DEBUG_RS_MAX_ERROR) {
					if ((!FQ->FQPara.B.btRWTableUnit) && uli == uwErrorPageIndex2) {
						// force UNC
						FALLL[FCTLL_UNC_CFG] |= (0x0000000F);
					}
					else if (uli == uwRSPlaneIndex) { // normal mode maybe need
						// force UNC
						FALLL[FCTLL_UNC_CFG] |= (0x0000000F);
					}
				}

				if (ENABLE_DEBUG_RS_VERIFY_DATA) {
					if (uli == uwRSPlaneIndex) {
						// clear force UNC
						FALLL[FCTLL_UNC_CFG] = 0;
						if (ENABLE_WP_DEBUG && (ulPhyPlaneIndex != ulTargetPlaneIndex)) {
							flaLEDBlink(1);
						}
						else {
							M_AssertError(ulPhyPlaneIndex != ulTargetPlaneIndex);
						}
						ulDMAAddr = VerifyDecodeRSDataStart;
					}
					else {
#if ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR
						if (!FQ->FQPara.B.btRWTableUnit) {
							UBYTE ubDebugi = 0;
							for (ubDebugi = 0; ubDebugi < ubDebugGenRSNormalModeErrorNum; ubDebugi++) {
								if (guwDebugRSNormalModeErrorPlaneNo[ubDebugi] == uli) {
									FALLL[FCTLL_UNC_CFG] |= AUTOGEN_READ_FAIL_EN;
									if (ENABLE_DEBUG_RS_NORMAL_MODE_UART) {
										UartString("\nRead_RS: ");
										UartLongHex(uli);
									}
								}
							}
						}
#endif
						ulDMAAddr = RSFrameDataDecodeStart;
					}
					ubBypassReadOK = RS_FPU_Read(ulDMAAddr, ubgFQLinkIndex, ulPhyPlaneIndex, uli, ubMTerrorCheckInRSTwoCh, ubDepth);	// read other plane for RS
					if (((!ENABLE_RS_NORMAL_MODE) && (gubRSErrorNum > RS_MAX_ERROR_COUNT))
					        || (ENABLE_RS_NORMAL_MODE && ((FQ->FQPara.B.btRWTableUnit && gubRSErrorNum > RS_TABLE_MAX_ERROR_COUNT) || (gubRSErrorNum > RS_MAX_ERROR_COUNT)))) {
						goto Mark_RS_Restore;
					}
					if (ubBypassReadOK && (uli != uwRSPlaneIndex)) {
						// decode
						//if (ENABLE_DCACHE) {
						//xthal_dcache_region_writeback_inv((void *)ulSpareAddr, Spare16kSize);
						//}
						RSB[RSB_PAGE_CNT] = 1;
						RSB[RSB_ROW_IDX] = uli;
						for (ub4K_IDX = 0; ub4K_IDX < gub4kEntrysPerPlane; ub4K_IDX++) {
							pBPT[(uli * 4 + ub4K_IDX)] = (ub4K_IDX * 4096);
							RSB[RSB_COL_IDX] = ub4K_IDX;
							RSL[RSL_TRIG] |= RS_TRIG_DEC;
							while ((RSB[RSB_IP_CTL] & RS_DEC_INTR) == 0); //polling equal 1
							RSB[RSB_IP_CTL] |= RS_DEC_INTR; // write 1 clear
						}
						RSL[RSL_MODE_0] &= (~RS_PBUF_INIT);
					}
				}
				else {
					if (uli != uwRSPlaneIndex) {
#if ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR
						if (!FQ->FQPara.B.btRWTableUnit) {
							UBYTE ubDebugi = 0;
							for (ubDebugi = 0; ubDebugi < ubDebugGenRSNormalModeErrorNum; ubDebugi++) {
								if (guwDebugRSNormalModeErrorPlaneNo[ubDebugi] == uli) {
									FALLL[FCTLL_UNC_CFG] |= AUTOGEN_READ_FAIL_EN;
								}
							}
						}
#endif
						ulDMAAddr = RSFrameDataDecodeStart;
						ubBypassReadOK = RS_FPU_Read(ulDMAAddr, ubgFQLinkIndex, ulPhyPlaneIndex, uli, ubMTerrorCheckInRSTwoCh, ubDepth);	// read other plane for RS
						if (((!ENABLE_RS_NORMAL_MODE) && (gubRSErrorNum > RS_MAX_ERROR_COUNT))
						        || (ENABLE_RS_NORMAL_MODE && ((FQ->FQPara.B.btRWTableUnit && gubRSErrorNum > RS_TABLE_MAX_ERROR_COUNT) || (gubRSErrorNum > RS_MAX_ERROR_COUNT)))) {
							goto Mark_RS_Restore;
						}
						if (ubBypassReadOK) {
							// decode
							//if (ENABLE_DCACHE) {
							//xthal_dcache_region_writeback_inv((void *)ulSpareAddr, Spare16kSize);
							//}
							RSB[RSB_PAGE_CNT] = 1;
							RSB[RSB_ROW_IDX] = uli;
							for (ub4K_IDX = 0; ub4K_IDX < gub4kEntrysPerPlane; ub4K_IDX++) {
								pBPT[(uli * 4 + ub4K_IDX)] = (ub4K_IDX * 4096);
								RSB[RSB_COL_IDX] = ub4K_IDX;
								RSL[RSL_TRIG] |= RS_TRIG_DEC;
								while ((RSB[RSB_IP_CTL] & RS_DEC_INTR) == 0); //polling equal 1
								RSB[RSB_IP_CTL] |= RS_DEC_INTR; // write 1 clear
							}
							RSL[RSL_MODE_0] &= (~RS_PBUF_INIT);
						}
					}
				}

				if (ENABLE_DEBUG_RS_MAX_ERROR) {
					if ((!FQ->FQPara.B.btRWTableUnit) && uli == uwErrorPageIndex2) {
						// clear force UNC
						FALLL[FCTLL_UNC_CFG] = 0;
					}
					else if (uli == uwRSPlaneIndex) {  // normal mode maybe need
						// clear force UNC
						FALLL[FCTLL_UNC_CFG] = 0;
					}
				}

				if ((!FQ->FQPara.B.btRWTableUnit) && ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR) {
					// clear force UNC
					FALLL[FCTLL_UNC_CFG] = 0;
				}
				uwCheckEncodeCount++;
			}
			uli++;
		}

		// decode PARITY
		// copy parity from dram
		if (ENABLE_DEBUG_RS_DMAC) {
			mDMAC_COPY( (U32)(&guoRSFrameBits[(ubRSFrameIndex * 32 * 1024) / 8]), RSFrameDataDecodeStart, 32 * 1024, 0, 0, 0, 0, 1);
			U32 ulSourceAddr = (U32)(&gulRSFrameSpareBits[(ubRSFrameIndex * 128) / 4]);
			U32 ulTargetAddr = RSFrameSpareDecodeStart;
			U32 ulByteLength = 128;
			if (ENABLE_DCACHE) {
				// Z2 need flush and use original DDR addr
				if ((ulSourceAddr >= Z2_DDR_BASE) && (ulSourceAddr < Z3_DDR_BASE)) {
					xthal_dcache_region_writeback_inv((void *)ulSourceAddr, ulByteLength);
				}
				if ((ulTargetAddr >= Z2_DDR_BASE) && (ulTargetAddr < Z3_DDR_BASE)) {
					xthal_dcache_region_invalidate((void *)ulTargetAddr, ulByteLength);
				}
			}
			memcpy((void *)ulTargetAddr, (void *)ulSourceAddr, ulByteLength);


			ulSpareAddr = RSFrameSpareDecodeStart;
			//if (ENABLE_DCACHE) {
			//xthal_dcache_region_writeback_inv((void *)ulSpareAddr, ulByteLength);
			//}
		}
		else {
			ulSpareAddr = (U32)(&gulRSFrameSpareBits[(ubRSFrameIndex * 128) / 4]);
			RSL[RSL_DBASE0] = (U32)(&guoRSFrameBits[(ubRSFrameIndex * 32 * 1024) / 8]);
			RSL[RSL_SBASE0] = ulSpareAddr;
			//if (ENABLE_DCACHE) {
			//xthal_dcache_region_writeback_inv((void *)ulSpareAddr, 128);
			//}
		}

		RSB[RSB_PAGE_CNT] = 2; // plus 2 parity page
		RSB[RSB_ROW_IDX] = 0xFD;
		for (ub4K_IDX = 0; ub4K_IDX < gub4kEntrysPerPlane; ub4K_IDX++) {
			pBPT[(0xFD * 4 + ub4K_IDX)] = (ub4K_IDX) * 4096;
			pBPT[(0xFE * 4 + ub4K_IDX)] = (ub4K_IDX + 4) * 4096;
			RSB[RSB_COL_IDX] = ub4K_IDX;
			RSL[RSL_TRIG] |= RS_TRIG_DEC;
			while ((RSB[RSB_IP_CTL] & RS_DEC_INTR) == 0); //polling equal 1
			RSB[RSB_IP_CTL] |= RS_DEC_INTR; // write 1 clear
		}
		RSL[RSL_MODE_0] &= (~RS_PBUF_INIT);

		// gubRSErrorNum:     跟著normal read, FPUCOR pass而減少
		// gubOriRSErrorNum: Backup 進 RS normal mode前的gubRSErrorNum
		gubOriRSErrorNum = gubRSErrorNum;
		UBYTE ubErrorPlanePtr = 0;
		{
		MARK_RS_ERASURE_MODE_NOTENCODEDONE:
			if (ENABLE_DEBUG_UART_FAIL) {
				UartString("\nEnterEraser_1");
			}
			if (ENABLE_WP_DEBUG && (gubRSErrorNum > 2)) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(gubRSErrorNum > 2);
			}

			if (gubEnterRSNormalMode) {
				if (ENABLE_DEBUG_UART_NES_RS) {
					UartString(" Nre-arr ");
				}
				RSL[RSL_MODE_0] &= (~RS_COR_NORMAL_MODE);
				// re-arrange ErrorPlaneIndex
				UBYTE ubCheckValid = 0;
				UBYTE ubNewErrorPlanePtr = 0;
				for (ubErrorPlanePtr = 0; ubErrorPlanePtr < gubOriRSErrorNum; ubErrorPlanePtr++) {
					if ((guwRSErrorPlaneIndex[ubErrorPlanePtr] & BIT_RS_INVALID_ERROR) == 0) {
						ubCheckValid = 1;
					}
					if (ubCheckValid) {
						guwRSErrorPlaneIndex[ubNewErrorPlanePtr] = guwRSErrorPlaneIndex[ubErrorPlanePtr];
						ubNewErrorPlanePtr++;
						ubCheckValid = 0;
					}
				}
				if (ENABLE_WP_DEBUG && (gubRSErrorNum < ubNewErrorPlanePtr)) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(gubRSErrorNum < ubNewErrorPlanePtr);
				}
			}

			RSB[RSB_ERA_IDX_EN_NUM] = gubRSErrorNum;

			RSB[RSB_ERA_IDX_A] = guwRSErrorPlaneIndex[0];

			if (gubRSErrorNum == 2) {
				RSB[RSB_ERA_IDX_B] = (guwRSErrorPlaneIndex[1] >= uwDataPlaneNum) ? (253 - uwDataPlaneNum + guwRSErrorPlaneIndex[1]) : guwRSErrorPlaneIndex[1];
			}

			ulSpareAddr = CorrectRSSpareStart;
			//if (ENABLE_DCACHE) {
			//xthal_dcache_region_invalidate((void *)ulSpareAddr, (Spare16kSize * gubRSErrorNum));
			//}

			RSB[RSB_ROW_IDX] = 0;
			RSB[RSB_PAGE_CNT] = 0xFD;
			RSL[RSL_DBASE0] = (CorrectRSDataStart);
			RSL[RSL_SBASE0] = (ulSpareAddr);
			for (ub4K_IDX = 0; ub4K_IDX < gub4kEntrysPerPlane; ub4K_IDX++) {

				pBPT[(guwRSErrorPlaneIndex[0] * 4) + ub4K_IDX] = (ub4K_IDX * 4096);

				if (gubRSErrorNum == 2) {
					if (guwRSErrorPlaneIndex[1] >= uwDataPlaneNum) {
						pBPT[((253 - uwDataPlaneNum + guwRSErrorPlaneIndex[1]) * 4) + ub4K_IDX] = ((ub4K_IDX + 4) * 4096);
					}
					else {
						pBPT[(guwRSErrorPlaneIndex[1] * 4) + ub4K_IDX] = ((ub4K_IDX + 4) * 4096);
					}
				}
				RSB[RSB_COL_IDX] = ub4K_IDX;
				RSL[RSL_TRIG] |= RS_TRIG_COR;
				while ((RSB[RSB_IP_CTL] & RS_COR_INTR) == 0); //polling equal 1
				RSB[RSB_IP_CTL] |= RS_COR_INTR; // write 1 clear
			}
		}
	}
	else {
		ulSpareAddr = RSFrameSpareDecodeStart;
		gubRSReadingParityPage = 0;

#if ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR
		//UWORD uwDebugRSNormalModeErrorPlaneNo[RS_MAX_ERROR_COUNT] = {0};
		for (uli = 0; uli < RS_MAX_ERROR_COUNT; uli++) {
			guwDebugRSNormalModeErrorPlaneNo[uli] = 0xFFE;
		}
		UBYTE ubDebugGenRSNormalModeErrorNum = (ENABLE_DEBUG_RS_NORMAL_MODE_LARGE_ERROR_PAGE == 1) ? 12 : 3;
		if (((ENABLE_DEBUG_RS_NORMAL_MODE_COR_PARITY_PAGE == 1) ? uwTotalRSPlaneNum : uwDataPlaneNum) > ubDebugGenRSNormalModeErrorNum) {
			if (ENABLE_DEBUG_RS_NORMAL_MODE_UART || ENABLE_DEBUG_RS_NORMAL_MODE_UART_PARITY || ENABLE_DEBUG_RS_NORMAL_MODE_COR_PARITY_PAGE) {
				UartString("\nDbgErrNum = ");
				Uart_Tx_DataHex(ubDebugGenRSNormalModeErrorNum);
			}
			UWORD uwRandValue = 0;
			UBYTE ubCheckSameValue = 0;
			U32 ulj = 0;
			for (uli = 0; uli < ubDebugGenRSNormalModeErrorNum; uli++) {
				if (ENABLE_DEBUG_SAVETIME_RS_NORMAL_MODE) {
					uwRandValue = ((uwRSPlaneIndex + 1 + uli) % uwDataPlaneNum);
				}
				else {
					if (ENABLE_DEBUG_RS_NORMAL_MODE_COR_PARITY_PAGE) {
						uwRandValue = getIntRandValue(uwTotalRSPlaneNum);
					}
					else {
						uwRandValue = getIntRandValue(uwDataPlaneNum);
					}
					while (1) {
						ubCheckSameValue = 0;
						if (uwRandValue == uwRSPlaneIndex) {
							ubCheckSameValue = 1;
						}
						if (ubCheckSameValue == 0) {
							for (ulj = 0; ulj < uli; ulj++) {
								if ((uwRandValue == guwDebugRSNormalModeErrorPlaneNo[ulj])) {
									ubCheckSameValue = 1;
									break;
								}
							}
						}
						if (ubCheckSameValue == 0) {
							break;
						}
						if (ENABLE_DEBUG_RS_NORMAL_MODE_COR_PARITY_PAGE) {
							uwRandValue = getIntRandValue(uwTotalRSPlaneNum);
						}
						else {
							uwRandValue = getIntRandValue(uwDataPlaneNum);
						}
					}
					/*if (ENABLE_DEBUG_RS_NORMAL_MODE_COR_PARITY_PAGE) {
						if (uli == 0) {
							uwRandValue = uwDataPlaneNum;
						}
						else if (uli == 1) {
							uwRandValue = uwDataPlaneNum + 1;
						}
					}*/
				}
				guwDebugRSNormalModeErrorPlaneNo[uli] = uwRandValue;
				if (ENABLE_DEBUG_RS_NORMAL_MODE_UART || ENABLE_DEBUG_RS_NORMAL_MODE_UART_PARITY || ENABLE_DEBUG_RS_NORMAL_MODE_COR_PARITY_PAGE) {
					UartString(" ");
					UartWordHex(guwDebugRSNormalModeErrorPlaneNo[uli]);
				}
				if (uwRandValue >= uwDataPlaneNum) {
					VT->gsReadFailDebugCount.gulRSDebug_NormalModeGenParityPageError_DebugCount++;
				}
			}
		}
#endif

		for (uli = 0; uli < uwTotalRSPlaneNum; uli++) {

			if (FQ->FQPara.B.btRWTableUnit) {
				ulPhyPlaneIndex = (M_Multiplier((M_Divider(ulTargetPlaneIndex, DEF_TableParityPlanes_Mcl_Div)), DEF_TableParityPlanes_Mcl_Div)) + uli;
			}
			else {
				if (!ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT) {

					U32 ulListIndex;
					Generate_RS_MAP_Index_by_Plane_flafail(ulTargetPlaneIndex);
					// total encode plane count / uwPlanesPerGroup = list index
					ulListIndex = (RS_FLAFAIL.ubTagIndex * PARITY_DATA_PAGE_NUM + uli) / RS.uwPlanesPerGroup;
					M_AssertError(ulListIndex >= RS_MAX_LINK_COUNT_PER_GROUP);
					M_AssertError(RS_FLAFAIL.ulPageLinkList[RS_FLAFAIL.ubGroupIndex][ulListIndex] == 0xFFFFFFFF);

					// get start plane index
					ulPhyPlaneIndex = RS_FLAFAIL.ulPageLinkList[RS_FLAFAIL.ubGroupIndex][ulListIndex] * RS.uwPlanesPerGroup;
					// adjust to real plane offset
					ulPhyPlaneIndex += ((RS_FLAFAIL.ubTagIndex * PARITY_DATA_PAGE_NUM + uli) % RS.uwPlanesPerGroup);

				}
				else {
					ulPhyPlaneIndex = (ulTargetPlaneIndex / (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM)) * (uwDataParityDummyPlanesNum * ubPARITY_GROUP_NUM);
					ulPhyPlaneIndex += ((M_Divider(uli, DEF_PlanesPerPage_Mcl_Div)) * (gubPlanesPerPage * ubPARITY_GROUP_NUM)) + (M_Multiplier((ubRSFrameIndex & (ubPARITY_GROUP_NUM - 1)), DEF_PlanesPerPage_Mcl_Div)) + (M_Modular(uli, DEF_PlanesPerPage_Mod_Mask));
				}
			}


			if (uli >= uwDataPlaneNum) {
				// read parity
				gubRSReadingParityPage = 1;
				FALLL[FCTLL_CHNL_SET] &= (~BYPASS_CONV);
				FCONL[FCONL_CRC_EN] &= (~CRC_32_EN); // read parity page must off crc32
				FCONL[FCONL_SGN_MSK] &= (~SIGNOFF_MASK_EN); // read parity page must off signoffmask
			}

			if (ENABLE_DEBUG_RS_MAX_ERROR) {
				if ((!FQ->FQPara.B.btRWTableUnit) && uli == uwErrorPageIndex2) {
					// force UNC
					FALLL[FCTLL_UNC_CFG] |= (0x0000000F);
				}
				else if (uli == uwRSPlaneIndex) { // normal mode maybe need
					// force UNC
					FALLL[FCTLL_UNC_CFG] |= (0x0000000F);
				}
			}

			if (ENABLE_DEBUG_RS_VERIFY_DATA) {
				if (uli == uwRSPlaneIndex) {
					// clear force UNC
					FALLL[FCTLL_UNC_CFG] = 0;
					if (ENABLE_WP_DEBUG && (ulPhyPlaneIndex != ulTargetPlaneIndex)) {
						flaLEDBlink(1);
					}
					else {
						M_AssertError(ulPhyPlaneIndex != ulTargetPlaneIndex);
					}
					ulDMAAddr = VerifyDecodeRSDataStart;
				}
				else {
#if ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR
					if (!FQ->FQPara.B.btRWTableUnit) {
						UBYTE ubDebugi = 0;
						for (ubDebugi = 0; ubDebugi < ubDebugGenRSNormalModeErrorNum; ubDebugi++) {
							if (guwDebugRSNormalModeErrorPlaneNo[ubDebugi] == uli) {
								FALLL[FCTLL_UNC_CFG] |= AUTOGEN_READ_FAIL_EN;
								if (ENABLE_DEBUG_RS_NORMAL_MODE_UART || ENABLE_DEBUG_RS_NORMAL_MODE_UART_PARITY) {
									UartString("\nRead_RS: ");
									UartLongHex(uli);
								}
							}
						}
					}
#endif
					ulDMAAddr = RSFrameDataDecodeStart;
				}
				ubBypassReadOK = RS_FPU_Read(ulDMAAddr, ubgFQLinkIndex, ulPhyPlaneIndex, uli, ubMTerrorCheckInRSTwoCh, ubDepth);	// read other plane for RS
				if (((!ENABLE_RS_NORMAL_MODE) && (gubRSErrorNum > RS_MAX_ERROR_COUNT))
				        || (ENABLE_RS_NORMAL_MODE && ((FQ->FQPara.B.btRWTableUnit && gubRSErrorNum > RS_TABLE_MAX_ERROR_COUNT) || (gubRSErrorNum > RS_MAX_ERROR_COUNT)))) {
					goto Mark_RS_Restore;
				}
				if (ubBypassReadOK && (uli != uwRSPlaneIndex)) {
					// decode
					//if (ENABLE_DCACHE) {
					//xthal_dcache_region_writeback_inv((void *)ulSpareAddr, Spare16kSize);
					//}
					RSB[RSB_PAGE_CNT] = 1;
					RSB[RSB_ROW_IDX] =  (uli >= uwDataPlaneNum) ? (253 - uwDataPlaneNum + uli) : uli;
					for (ub4K_IDX = 0; ub4K_IDX < gub4kEntrysPerPlane; ub4K_IDX++) {
						if (uli >= uwDataPlaneNum) {
							pBPT[((253 - uwDataPlaneNum + uli) * 4 + ub4K_IDX)] = (ub4K_IDX * 4096);
						}
						else {
							pBPT[(uli * 4 + ub4K_IDX)] = (ub4K_IDX * 4096);
						}
						RSB[RSB_COL_IDX] = ub4K_IDX;
						RSL[RSL_TRIG] |= RS_TRIG_DEC;
						while ((RSB[RSB_IP_CTL] & RS_DEC_INTR) == 0); //polling equal 1
						RSB[RSB_IP_CTL] |= RS_DEC_INTR; // write 1 clear
					}
					RSL[RSL_MODE_0] &= (~RS_PBUF_INIT);
				}
			}
			else {
				if (uli != uwRSPlaneIndex) {
#if ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR
					if (!FQ->FQPara.B.btRWTableUnit) {
						UBYTE ubDebugi = 0;
						for (ubDebugi = 0; ubDebugi < ubDebugGenRSNormalModeErrorNum; ubDebugi++) {
							if (guwDebugRSNormalModeErrorPlaneNo[ubDebugi] == uli) {
								FALLL[FCTLL_UNC_CFG] |= AUTOGEN_READ_FAIL_EN;
							}
						}
					}
#endif
					ulDMAAddr = RSFrameDataDecodeStart;
					ubBypassReadOK = RS_FPU_Read(ulDMAAddr, ubgFQLinkIndex, ulPhyPlaneIndex, uli, ubMTerrorCheckInRSTwoCh, ubDepth);	// read other plane for RS
					if (((!ENABLE_RS_NORMAL_MODE) && (gubRSErrorNum > RS_MAX_ERROR_COUNT))
					        || (ENABLE_RS_NORMAL_MODE && ((FQ->FQPara.B.btRWTableUnit && gubRSErrorNum > RS_TABLE_MAX_ERROR_COUNT) || (gubRSErrorNum > RS_MAX_ERROR_COUNT)))) {
						goto Mark_RS_Restore;
					}
					if (ubBypassReadOK) {
						// decode
						//if (ENABLE_DCACHE) {
						//xthal_dcache_region_writeback_inv((void *)ulSpareAddr, Spare16kSize);
						//}
						RSB[RSB_PAGE_CNT] = 1;
						RSB[RSB_ROW_IDX] =  (uli >= uwDataPlaneNum) ? (253 - uwDataPlaneNum + uli) : uli;
						for (ub4K_IDX = 0; ub4K_IDX < gub4kEntrysPerPlane; ub4K_IDX++) {
							if (uli >= uwDataPlaneNum) {
								pBPT[((253 - uwDataPlaneNum + uli) * 4 + ub4K_IDX)] = (ub4K_IDX * 4096);
							}
							else {
								pBPT[(uli * 4 + ub4K_IDX)] = (ub4K_IDX * 4096);
							}
							RSB[RSB_COL_IDX] = ub4K_IDX;
							RSL[RSL_TRIG] |= RS_TRIG_DEC;
							while ((RSB[RSB_IP_CTL] & RS_DEC_INTR) == 0); //polling equal 1
							RSB[RSB_IP_CTL] |= RS_DEC_INTR; // write 1 clear
						}
						RSL[RSL_MODE_0] &= (~RS_PBUF_INIT);
					}
				}
			}


			if (ENABLE_DEBUG_RS_MAX_ERROR) {
				if ((!FQ->FQPara.B.btRWTableUnit) && uli == uwErrorPageIndex2) {
					// clear force UNC
					FALLL[FCTLL_UNC_CFG] = 0;
				}
				else if (uli == uwRSPlaneIndex) { // normal mode maybe need
					// clear force UNC
					FALLL[FCTLL_UNC_CFG] = 0;
				}
			}

			if ((!FQ->FQPara.B.btRWTableUnit) && ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR) {
				// clear force UNC
				FALLL[FCTLL_UNC_CFG] = 0;
			}
		}


		// gubRSErrorNum:     跟著normal read, FPUCOR pass而減少
		// gubOriRSErrorNum: Backup 進 RS normal mode前的gubRSErrorNum
		gubOriRSErrorNum = gubRSErrorNum;
		UBYTE ubErrorPlanePtr = 0;
		{
		MARK_RS_ERASURE_MODE_ENCODEDONE:
			if (ENABLE_DEBUG_UART_FAIL) {
				UartString("\nEnterEraser_2");
			}
			if (ENABLE_WP_DEBUG && (gubRSErrorNum > 2)) {
				flaLEDBlink(1);
			}
			else {
				M_AssertError(gubRSErrorNum > 2);
			}

			if (gubEnterRSNormalMode) {
				RSL[RSL_MODE_0] &= (~RS_COR_NORMAL_MODE);
				// re-arrange ErrorPlaneIndex
				UBYTE ubCheckValid = 0;
				UBYTE ubNewErrorPlanePtr = 0;
				for (ubErrorPlanePtr = 0; ubErrorPlanePtr < gubOriRSErrorNum; ubErrorPlanePtr++) {
					if ((guwRSErrorPlaneIndex[ubErrorPlanePtr] & BIT_RS_INVALID_ERROR) == 0) {
						ubCheckValid = 1;
					}
					if (ubCheckValid) {
						guwRSErrorPlaneIndex[ubNewErrorPlanePtr] = guwRSErrorPlaneIndex[ubErrorPlanePtr];
						ubNewErrorPlanePtr++;
						ubCheckValid = 0;
					}
				}
				if (ENABLE_WP_DEBUG && (gubRSErrorNum < ubNewErrorPlanePtr)) {
					flaLEDBlink(1);
				}
				else {
					M_AssertError(gubRSErrorNum < ubNewErrorPlanePtr);
				}
			}

			//correct
			RSB[RSB_ERA_IDX_EN_NUM] = gubRSErrorNum;
			RSB[RSB_ERA_IDX_A] = guwRSErrorPlaneIndex[0];
			if (gubRSErrorNum == 2) {
				RSB[RSB_ERA_IDX_B] = (guwRSErrorPlaneIndex[1] >= uwDataPlaneNum) ? (253 - uwDataPlaneNum + guwRSErrorPlaneIndex[1]) : guwRSErrorPlaneIndex[1];
			}

			ulSpareAddr = CorrectRSSpareStart;
			//if (ENABLE_DCACHE) {
			//xthal_dcache_region_invalidate((void *)ulSpareAddr, (Spare16kSize * gubRSErrorNum));
			//}

			RSB[RSB_ROW_IDX] = 0;
			RSB[RSB_PAGE_CNT] = 0xFD;
			RSL[RSL_DBASE0] = CorrectRSDataStart;
			RSL[RSL_SBASE0] = ulSpareAddr;
			for (ub4K_IDX = 0; ub4K_IDX < gub4kEntrysPerPlane; ub4K_IDX++) {
				pBPT[(guwRSErrorPlaneIndex[0] * 4) + ub4K_IDX] = (ub4K_IDX * 4096);
				if (gubRSErrorNum == 2) {
					if (guwRSErrorPlaneIndex[1] >= uwDataPlaneNum) {
						pBPT[((253 - uwDataPlaneNum + guwRSErrorPlaneIndex[1]) * 4) + ub4K_IDX] = ((ub4K_IDX + 4) * 4096);
					}
					else {
						pBPT[(guwRSErrorPlaneIndex[1] * 4) + ub4K_IDX] = ((ub4K_IDX + 4) * 4096);
					}
				}
				RSB[RSB_COL_IDX] = ub4K_IDX;
				RSL[RSL_TRIG] |= RS_TRIG_COR;
				while ((RSB[RSB_IP_CTL]&RS_COR_INTR) == 0); //polling equal 1
				RSB[RSB_IP_CTL] |= RS_COR_INTR; // write 1 clear
			}
		}
	}

MARK_RS_Verify:
	ulSpareAddr = CorrectRSSpareStart;
#if ENABLE_DEBUG_RS_VERIFY_DATA
	if (gubRSDebug_NoGoldenToVerifyDATA == 0) {
		Debug_RS_VERIFY_DATA(ubgFQLinkIndex, ubMTerrorCheckInRSTwoCh, 1, ubDepth);
	}
#endif

Mark_RS_Restore:
	//verify passed
	RSL[RSL_DBASE0] = (U32)(&guoRSFrameBits[0]);
	RSL[RSL_SBASE0] = (U32)(&gulRSFrameSpareBits[0]);
	RSB[RSB_PAGE_CNT] = 1;
	RSB[RSB_DPAGE_NUM] = 0xFD;
	for (ub4K_IDX = 0; ub4K_IDX < gub4kEntrysPerPlane; ub4K_IDX++) {
		pBPT[(0xFD * 4 + ub4K_IDX)] = 0;
		pBPT[(0xFE * 4 + ub4K_IDX)] = 0;
	}
	//FQ->ubReadSource = 0xFF;

	// 20150909
	RSB[RSB_COL_IDX] = 0;
	if (!ENABLE_DEBUG_RS_PRE_SL) {
		RSL[RSL_PBUF_CTL] |= (RS_RRE_SAVE_LOAD_EN);
	}



	FALLL[FCTLL_CHNL_SET] &= (~BYPASS_CONV);
	FCONL[FCONL_CRC_EN] |= (CRC_32_EN); // read parity page must off crc32
	FCONL[FCONL_SGN_MSK] |= (SIGNOFF_MASK_EN); // read parity page must off signoffmask
	FALLL[FCTLL_IBF_CTL] &= (~FORCE_ZIP_COR);
	if (ENABLE_RS_NORMAL_MODE) {
		RSL[RSL_MODE_0] &= (~RS_COR_NORMAL_MODE);
	}
	//resume global variable
	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));


	if (((!ENABLE_RS_NORMAL_MODE) && (gubRSErrorNum <= RS_MAX_ERROR_COUNT))
	        || (ENABLE_RS_NORMAL_MODE && ((FQ->FQPara.B.btRWTableUnit && gubRSErrorNum <= RS_TABLE_MAX_ERROR_COUNT) || (gubRSErrorNum <= RS_MAX_ERROR_COUNT)))) {
		// copy correct data & spare
		ulSpareAddr = CorrectRSSpareStart;
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)ulSpareAddr, Spare16kSize);
		}

		UBYTE ubFrameNum = gMTQ->Depth[FQ->ubDepth].Para0x24.BitMap.ubFrameNum;
		UBYTE ubStartFrameNo = FQ->ulFEntry & gub4kEntrysPerPlaneMask;
		memcpy((void *)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET), (void *)(ulSpareAddr + (ubStartFrameNo * Spare4kSize)), (ubFrameNum * Spare4kSize));
		if (ENABLE_DEBUG_RS_DMAC) {
			mDMAC_COPY( (U32)(CorrectRSDataStart + (ubStartFrameNo * 4096)), (U32)&gubflaBuffer[0], (ubFrameNum * 4096), 0, 0, 0, 0, 1);
		}
	}
	gFREG[FCTLL_ZIP_CFG].L |= (RING_MODE);

	// Clear all MTQ in channel
	if (!ubMTerrorCheckInRSTwoCh) {
		// UDMA must redo
		NonUncChannelFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
		NonUncChannelFREG[FCTLL_MT_TRIG].B.B1 = 0;
		gubRSclearMTQRebuildFQ = 1;
	}

	// resume
	NonUncChannelFREG[FCTLL_UNC_CFG].L = 0;
	NonUncChannelFREG[FCTLL_ZIP_CFG].L |= (RING_MODE | L4K_BADR_CONTINUOUS_MODE);
	NonUncChannelFREG[FCTLL_INT_CFG].L |= (ABORT_DONE_INTR_EN);

	NonUncChannelFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
	NonUncChannelFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);

#endif
	//#if(RDT)
	//	return 0;
	//#endif
}



///============================
void FlaFlashDeRandomize(U32 * ulSourceBuferPtr, U32 * ulResultBuferPtr, UBYTE ubFrame ) {

	M_CheckRDTModeToDisableFunction();

	/// first part : send data into IBF, max 16K data
	unsigned char ubChannel = 0;
	unsigned char ubBuffModEn = 0;
	unsigned char ubL4KCount;
	unsigned long ulSpareAddrBackup, ulALUCelBackUp, ulConvInvBackUp;

	volatile L4KTable16B *pL4KTable;




	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel) * 0x200));

	unsigned long ulBackUp1 = FCONL[FCONL_FLH_FUNC];
	unsigned long ulBackUp2 = gFREG[FCTLL_RAW_DMA_ADR].L;
	unsigned long ulBackUp3 = gFREG[FCTLL_ZIP_MAP].L;
	unsigned long ulBackUp4 = gFREG[FCTLL_ZIP_CFG].L;
	unsigned long ulBackUp5 = gFREG[FCTLL_FSA_SEL].L;
	unsigned long ulBackUp6 = gFREG[FCTLL_DMA_CFG].L;
	unsigned char ubBackUpByte1 = gFREG[FCTLL_FSA_SEL].B.B0;
	unsigned long ulBackUp7 = gFREG[FCTLL_IFSA0].L;
	unsigned long ulBackUp8 = gFREG[FCTLL_INT_INF].L;
	unsigned long ulBackUp9 = gFREG[FCTLL_INT_CFG].L;
	unsigned long ulBackUp10 = gFREG[FCTLL_CHNL_SET].L;
	unsigned long ulBackUp11 = gFREG[FCTLL_DMA_ADR_0].L;
	unsigned long ulBackUp12 = gFREG[FCTLL_IBF_CTL].L;
	unsigned long ulBackUp13 = gFREG[FCTLL_OTHER_SET].L;
	unsigned long ulBackUp14 = FCONL[FCONL_CRC_EN];
	unsigned long ulBackUp15 = gFREG[FCTLL_CMP_CFG].L;
	unsigned long ulBackUp16 = gFREG[FCTLL_SEED_INIT].L;


	///New Added
	gFREG[FCTLL_OTHER_SET].L &= 0x0000FFFF;


	UBYTE ubDepth;
	M_GetMTDepth(ubDepth);
	gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
	gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);

	if (ubChannel == 0) {
		FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH0_EN);
	}
	else {
		FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH1_EN);
	}

	//	gFREG[FCTLL_SEED_INIT].L=0;
	//	gFREG[FCTLL_OTHER_SET].L |= 0x02000020;  // bit 9 25 set 1
	//	gFREG[FCTLL_OTHER_SET].L &= 0xFE00FFFF;  // bit 16~24 set 0

	ulConvInvBackUp = gFREG[FCTLL_CHNL_SET].L;


	gFREG[FCTLL_BACK_STORE].W.W0 = (TEMP_L4K_TABLE_OFFSET);    /// IRAM_ADDRRESS 0x134 set 50001040
	gFREG[FCTLL_RAW_DMA_ADR].L = ((U32)(&ulSourceBuferPtr[0]));  /// Set buffer addr


	for (ubL4KCount = 0; ubL4KCount < ubFrame; ubL4KCount++) {
		pL4KTable = (volatile L4KTable16B*)(0x040211A0 + (ubL4KCount * Spare4kSize));
		//set L4K table info
		pL4KTable->ulL4K_LCA = 0;
		pL4KTable->Para0x04.ulAll = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = 0;
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0x0;
	}

	ulSpareAddrBackup = gFREG[FCTLL_L4K_SPR_ADR].L;
	gFREG[FCTLL_L4K_SPR_ADR].L = 0x11A0;
	gFREG[FCTLL_ZIP_MAP].L = 0xFFFFFFFF;
	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubFrame);

	gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);
	gFREG[FCTLL_DMA_CFG].L |= TRANSFER_4_ECC_FRAME;
	gFREG[FCTLL_OTHER_SET].L &= 0xFF03FFFF;
	gFREG[FCTLL_CMP_CFG].L &= 1;
	gFREG[FCTLL_SEED_INIT].L &= 0;
	gFREG[FCTLL_CHNL_SET].L &= BYPASS_INV;     // Bypass inversion

	////Select ALU
	ulALUCelBackUp = gFREG[FCTLL_DMA_CFG].L;
	gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
	/// set iFSA to page 0
	gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;
	gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
	gFREG[FCTLL_FSA_SEL].B.B0  = 0;
	gFREG[FCTLL_IFSA0].L = 0;


	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	IRAMB[FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION] = SET_FPU_IBF_RESTORE_BACKUP(1, ubFrame, 1, 1);
	gFREG[FCTLL_FPU_ENTRY].L  = FPU_PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);


	/// second part : DeRandomize
	gFREG[FCTLL_CHNL_SET].L &= (~BYPASS_CONV);     // Enable conversion

	gFREG[FCTLL_ZIP_CFG].L |= (BYPASS_ZIP);

	////Select ALU
	gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
	/// set iFSA to page 0
	gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;
	gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
	gFREG[FCTLL_FSA_SEL].B.B0  = 0;
	gFREG[FCTLL_IFSA0].L = 0;


	///  填L4K Table及 L4K Table in IRAM addr    (usable address is between 0x500011A0~1200)
	//for (ubL4KCount = 0; ubL4KCount < ubL4KNUM; ubL4KCount++) {
	for (ubL4KCount = 0; ubL4KCount < ubFrame; ubL4KCount++) {
		pL4KTable = (volatile L4KTable16B*)(0x040211A0 + (ubL4KCount * Spare4kSize));
		//set L4K table info
		pL4KTable->ulL4K_LCA = 0;
		pL4KTable->Para0x04.ulAll = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ( ((U32)(&ulResultBuferPtr[0])) + ubL4KCount * 4096) / 512;
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
	}
	//gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_ADDR;
	gFREG[FCTLL_L4K_SPR_ADR].L = 0x11A0;
	gFREG[FCTLL_DMA_ADR_0].L = ((U32)(&ulResultBuferPtr[0]));

	gFREG[FCTLL_ZIP_MAP].L = 0xFFFFFFFF;
	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubFrame);
	//
	if (gFREG[FCTLL_DMA_CFG].L & (FLH_BUF_MODE)) {
		ubBuffModEn = 1;
	}

	gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);


	gFREG[FCTLL_DMA_CFG].L &= TRANSFER_0_ECC_FRAME;
	gFREG[FCTLL_DMA_CFG].L |= ( ubFrame << 8);
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_IBF_CTL].L &= CLR_LDPC_COR_MAP;
	gFREG[FCTLL_OTHER_SET].L &= (~BCH_COR_EN);
	FCONL[FCONL_CRC_EN] &= (~CRC_32_EN); // read parity page must off crc32
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_IBF_CORRECT;


	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	/// directly free interrupt(only may happen when cmp LCA error, which can be ignored) and reset map
	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
		gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
	}
	gubINTTable[ubDepth].All = 0;
	M_AddMTDepth(ubDepth);

	//// after all done, return the value(LDPC ECC CRC...etc)
	gFREG[FCTLL_CHNL_SET].L = ulConvInvBackUp;
	gFREG[FCTLL_L4K_SPR_ADR].L = ulSpareAddrBackup;
	gFREG[FCTLL_DMA_CFG].L = ulALUCelBackUp;

	FCONL[FCONL_FLH_FUNC] = ulBackUp1;
	gFREG[FCTLL_RAW_DMA_ADR].L = ulBackUp2;
	gFREG[FCTLL_ZIP_MAP].L =   ulBackUp3;
	gFREG[FCTLL_ZIP_CFG].L = ulBackUp4 ;
	gFREG[FCTLL_FSA_SEL].L = ulBackUp5;
	gFREG[FCTLL_DMA_CFG].L = ulBackUp6;
	gFREG[FCTLL_FSA_SEL].B.B0 = ubBackUpByte1;
	gFREG[FCTLL_IFSA0].L = ulBackUp7;
	gFREG[FCTLL_INT_INF].L = ulBackUp8;
	gFREG[FCTLL_INT_CFG].L =   ulBackUp9;
	gFREG[FCTLL_CHNL_SET].L = ulBackUp10;
	gFREG[FCTLL_DMA_ADR_0].L = ulBackUp11;
	gFREG[FCTLL_IBF_CTL].L = ulBackUp12;
	gFREG[FCTLL_OTHER_SET].L =   ulBackUp13;
	FCONL[FCONL_CRC_EN] =   ulBackUp14;
	gFREG[FCTLL_CMP_CFG].L = ulBackUp15;
	gFREG[FCTLL_SEED_INIT].L = ulBackUp16;
}

void flaEraseFailHandle(UBYTE ubgFQLinkIndex) {

	//M_CheckBurnerModeToDisableFunction();
	FlashQueue_t *FQ = &gFQI.gFQLink[ubgFQLinkIndex];
	UBYTE ubChannel = FQ->ubCEIndex & gubPlanesPerBurstMask;
	UBYTE ubCEIndex = FQ->ubCEIndex;

	UBYTE ubNeedToClearForceEmpty = 0;

	if ((gFREG[FCTLL_MTQ_CFG].L & FORCE_EMPTY_ALL) != FORCE_EMPTY_ALL) {
		// Only Stop MT when it is NOT force empty status, and prevent to resume MT too early.
		ubNeedToClearForceEmpty = 1;

		// force empty to avoid program parity clear interrupt vector enable bit bug
		gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);
	}
	// force empty to avoid program parity clear interrupt vector enable bit bug
	//gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);//模擬這個 CH 為空的狀態

	// wait the channel idle for read retry
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);//確認沒有FPU在執行

	// wait the channel true idle when force empty
	gFREG[FCTLL_DBG_INF].L = (0x0C);
	while ((GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE1) && (GET_DBG_MTQ_STS(gFREG[FCTLL_DBG_INF].L) != DBG_MTQ_IDLE_CASE2));

	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix 2nd program parity hang on bug

	M_VirtualToPhysicalCE(ubCEIndex);

	FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;

	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}
	gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
	gFREG[FCTLL_INT_VCT].L |= IntVector(FQ->ubDepth);
	// Polling TRUE RDY
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x6D_FPU_POL_flaEraseFailHandle, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
			if ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
				break;
			}
		}
	}

	/*

	*/


	// MT preload + FPU trig = Auto clear CE, so it need to set CE again
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]); // Enable selected CE
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
	}
		{
		// CMD RESET

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
			if ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
				break;
			}
		}
	}
	// Polling TRUE RDY
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	if (ENABLE_CHECK_CHANNEL_FPU_TIMEOUT) {
		flaCheckChannelFPUBusy(ubgFQLinkIndex, SN_0x6E_FPU_POL_flaEraseFailHandle, gFREG, CHECK_FPU_POL_TIMEOUT);
	}
	else {
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
			if ((gFREG[FCTLL_FPU_TRIG].L & TIMEOUT) == TIMEOUT) {
				break;
			}
		}
	}

	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
	{
		FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = 0;
	}
	else {
		FCONL[FCONL_FCE_CLR] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Disable selected CE
	}
	gubINTTable[FQ->ubDepth].All &= 0x0F;//只保留ALL DONE 等正常資訊

	gubINTTable[FQ->ubDepth].B.btAll_Done = 1;
	gubINTTable[FQ->ubDepth].B.btPartial_Done = 0;//preload next reuse mt (read action) will make partial done = 1 all done = 0 removeit

	if (ubNeedToClearForceEmpty) {
		gFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
		gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);//把模擬狀態復原
	}
	//gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);//把模擬狀態復原

	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
		gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
	}

}




