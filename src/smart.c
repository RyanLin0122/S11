
#define _SMART_

#include "setup.h"
#include "Vardef.h"
#include "ftl.h"
#include <string.h>
#include "CMDTBL.H"
#include "global/_global.h"

extern UWORD SerialTable[10];
extern UWORD VerTable[4];
extern UWORD ModelTable[20];


void CalCheckSum(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	BUFB_BASE[511] = 0;

	UBYTE ubSizeUnit = 0; //1B
	UBYTE ubCmdUnit = 1;
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}
	while (SEB[SEB_SQ_WR_CNT] < 1);
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 0] = SE0_ChkSum | ubCmdUnit << 6 | ubSizeUnit << 8;
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 1] = (U32)BUFB_BASE; //source address
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 2] = 0x200; //byte count
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 3] = 0;
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 4] = 0; //pattern_L
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 5] = 0; //pattern_H
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 6] = 0; //bMSK0_L
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 7] = 0; //bMSK0_H
	SEB[SEB_SQ_WPIU] = 1;
	while (SEB[SEB_CQ_RD_CNT] < 1);
	SEB[SEB_CQ_RPIU] = 1;
	gubTemp = SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + 1];

	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;


	if (ENABLE_DEBUG_RW) {          // check by SW
		UBYTE ubTemp = 0;
		for (guwTemp = 0; guwTemp < 511; guwTemp++) {
			ubTemp += BUFB_BASE[guwTemp];
		}
		M_AssertError(gubTemp != ubTemp);
	}
	//BUF0_BASE[511] = (~ubTemp) + 1;
	BUFB_BASE[511] = (~gubTemp) + 1;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}
#endif
}

UBYTE CheckCheckSum(void)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunctionReturnValid(1);

	UBYTE ubHostCheckSum = BUFB_BASE[511];
	UBYTE ubCulCheckSum = 0;
	BUFB_BASE[511] = 0;

	UBYTE ubSizeUnit = 0; //1B
	UBYTE ubCmdUnit = 1;
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}
	while (SEB[SEB_SQ_WR_CNT] < 1);
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 0] = SE0_ChkSum | ubCmdUnit << 6 | ubSizeUnit << 8;
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 1] = (U32)BUFB_BASE; //source address
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 2] = 0x200; //byte count
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 3] = 0;
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 4] = 0; //pattern_L
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 5] = 0; //pattern_H
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 6] = 0; //bMSK0_L
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 7] = 0; //bMSK0_H
	SEB[SEB_SQ_WPIU] = 1;
	while (SEB[SEB_CQ_RD_CNT] < 1);
	SEB[SEB_CQ_RPIU] = 1;
	gubTemp = SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + 1];

	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;


	if (ENABLE_DEBUG_RW) {          // check by SW
		UBYTE ubTemp = 0;
		for (guwTemp = 0; guwTemp < 511; guwTemp++) {
			ubTemp += BUFB_BASE[guwTemp];
		}
		M_AssertError(gubTemp != ubTemp);
	}

	BUFB_BASE[511] = ubHostCheckSum;
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}

	ubCulCheckSum =  (~gubTemp) + 1;

	if (ubCulCheckSum != ubHostCheckSum) {
		return 1;
	}
	return 0;

#endif
#if(RDT)
	return 0;
#endif
}

UBYTE Get_SMART_Attribute_Value_Worst(UWORD  uwValueWorst_MP, SMART_BADBLOCK_INFO BadBlockInfo, SMART_ERASECOUNT_INFO EraseCountInfo, SMART_FAILCOUNT_INFO FailCountInfo, UBYTE ubTemperature)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);

	UBYTE ubValueWorst = 0;
	U32 ulAllottedEraseCnt_D3 = gulMaxPECycle_D3;
	U32 ulTotalAllowedFail;
	//////////////// need get from VT  /////////////////
	UBYTE ubReadErrorRate;
	UBYTE ubBadBlockPercent;

	///////////////////////////////////////////////////

	ulTotalAllowedFail = (M_Multiplier((U32)guwBadBlockBoundary, DEF_PlanesPerPage_Mcl_Div)) * gubSelTotalGroups;

	switch (uwValueWorst_MP) {
		case 0x0000 :
			ubValueWorst = 0x64;
			break;
		case 0x0100 ://read error rate
			// Read Error Rate
			// Fomula: 100 * (total sent number of sectors to host - number of ECC error) / total sent number of sectors to host
			// Initial:100   min:1  max:100
			//the definition is not common sense...
			if (VT->guoTotalHostReadSectorCnt) {
				ubReadErrorRate = (UBYTE)((ULLONG)100 * (VT->guoTotalHostReadSectorCnt - VT->gulHostReadECCCnt) / VT->guoTotalHostReadSectorCnt);
			}
			else {
				ubReadErrorRate = 100;
			}
			ubValueWorst = ubReadErrorRate;
			break;
		case 0x0101 ://BadBlockPercent
			if (BadBlockInfo.uwMaxLaterBadCntPerPlane >= (guwBadBlockBoundary - BadBlockInfo.uwMaxEarlyBadCntPerPlane)) {
				ubBadBlockPercent = 100;
			}
			else {
				ubBadBlockPercent = (UBYTE)((ULLONG)BadBlockInfo.uwMaxLaterBadCntPerPlane * 100 / (guwBadBlockBoundary - BadBlockInfo.uwMaxEarlyBadCntPerPlane));
			}
			ubValueWorst = ubBadBlockPercent;
			break;
		case 0x0102 ://100 - BadBlockPercent
			if (BadBlockInfo.uwMaxLaterBadCntPerPlane >= (guwBadBlockBoundary - BadBlockInfo.uwMaxEarlyBadCntPerPlane)) {
				ubBadBlockPercent = 100;
			}
			else {
				ubBadBlockPercent = (UBYTE)((ULLONG)BadBlockInfo.uwMaxLaterBadCntPerPlane * 100 / (guwBadBlockBoundary - BadBlockInfo.uwMaxEarlyBadCntPerPlane));
			}
			ubValueWorst = 100 - ubBadBlockPercent;
			break;
		case 0x0103 ://SSD life left
			if ( EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)(ulAllottedEraseCnt_D3 - EraseCountInfo.TypeD3.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D3);
			}
			ubValueWorst = gubTemp;
			break;
		case 0x0104 ://100 - SSD life left
			if ( EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)(ulAllottedEraseCnt_D3 - EraseCountInfo.TypeD3.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D3);
			}
			ubValueWorst = 100 - gubTemp;
			break;

		case 0x0105 ://100 - read error rate
			// Read Error Rate
			// Fomula: 100 * (total sent number of sectors to host - number of ECC error) / total sent number of sectors to host
			// Initial:100   min:1  max:100
			if (VT->guoTotalHostReadSectorCnt) {
				ubReadErrorRate = (UBYTE)((ULLONG)100 * (VT->guoTotalHostReadSectorCnt - VT->gulHostReadECCCnt) / VT->guoTotalHostReadSectorCnt);
			}
			else {
				ubReadErrorRate = 100;
			}
			ubValueWorst = 100 - ubReadErrorRate;
			break;
		case 0x0106 ://SSD life left (v2)
			if ((EraseCountInfo.TypeD3.ulAvgEraseCnt + VT->gulPreviousMaxEraseCnt_D3) >= ulAllottedEraseCnt_D3) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)(ulAllottedEraseCnt_D3 - (EraseCountInfo.TypeD3.ulAvgEraseCnt + VT->gulPreviousMaxEraseCnt_D3)) * 100 / ulAllottedEraseCnt_D3);
			}
			ubValueWorst = gubTemp;
			break;
		case 0x0107 ://100 - SSD life left (v2)
			if ((EraseCountInfo.TypeD3.ulAvgEraseCnt + VT->gulPreviousMaxEraseCnt_D3) >= ulAllottedEraseCnt_D3) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)(ulAllottedEraseCnt_D3 - (EraseCountInfo.TypeD3.ulAvgEraseCnt + VT->gulPreviousMaxEraseCnt_D3)) * 100 / ulAllottedEraseCnt_D3);
			}
			ubValueWorst = 100 - gubTemp;
			break;

		case 0x0108 ://BadBlockPercent(CE)
			if (BadBlockInfo.uwMaxLaterBadCntPerCE >= (((guwBadBlockBoundary << gubBurstsPerBankLog)*gubSelDieNumber) - BadBlockInfo.uwMaxEarlyBadCntPerCE)) {
				ubBadBlockPercent = 100;
			}
			else {
				ubBadBlockPercent = (UBYTE)((ULLONG)BadBlockInfo.uwMaxLaterBadCntPerCE * 100 / (((guwBadBlockBoundary << gubBurstsPerBankLog) * gubSelDieNumber) - BadBlockInfo.uwMaxEarlyBadCntPerCE));
			}
			ubValueWorst = ubBadBlockPercent;
			break;
		case 0x0109 ://100 - BadBlockPercent(CE)
			if (BadBlockInfo.uwMaxLaterBadCntPerCE >= (((guwBadBlockBoundary << gubBurstsPerBankLog)*gubSelDieNumber) - BadBlockInfo.uwMaxEarlyBadCntPerCE)) {
				ubBadBlockPercent = 100;
			}
			else {
				ubBadBlockPercent = (UBYTE)((ULLONG)BadBlockInfo.uwMaxLaterBadCntPerCE * 100 / (((guwBadBlockBoundary << gubBurstsPerBankLog) * gubSelDieNumber) - BadBlockInfo.uwMaxEarlyBadCntPerCE));
			}
			ubValueWorst = 100 - ubBadBlockPercent;
			break;
		case 0x010A ://SSD life left, min = 1
			if ( EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
				gubTemp = 1;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)(ulAllottedEraseCnt_D3 - EraseCountInfo.TypeD3.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D3);
			}
			ubValueWorst = gubTemp;
			break;
		case 0x0200 ://ProgramFailPercent
			if (FailCountInfo.ulTotalProgramFailCnt >= ulTotalAllowedFail) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)(FailCountInfo.ulTotalProgramFailCnt * 100 / ulTotalAllowedFail);
			}
			ubValueWorst = gubTemp;
			break;
		case 0x0201 ://100 - ProgramFailPercent
			if ( FailCountInfo.ulTotalProgramFailCnt >= ulTotalAllowedFail) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)FailCountInfo.ulTotalProgramFailCnt * 100 / ulTotalAllowedFail);
			}
			ubValueWorst = 100 - gubTemp;
			break;
		case 0x0202 ://EraseFailPercent
			if ( FailCountInfo.ulTotalEraseFailCnt >= ulTotalAllowedFail) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)FailCountInfo.ulTotalEraseFailCnt * 100 / ulTotalAllowedFail);
			}
			ubValueWorst = gubTemp;
			break;
		case 0x0203 ://100 - EraseFailPercent
			if ( FailCountInfo.ulTotalEraseFailCnt >= ulTotalAllowedFail) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)FailCountInfo.ulTotalEraseFailCnt * 100 / ulTotalAllowedFail);
			}
			ubValueWorst = 100 - gubTemp;
			break;
		case 0x0204 ://erase percent, re-define, same as 0104h
			if ( EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
				gubTemp = 100;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)EraseCountInfo.TypeD3.ulAvgEraseCnt * 100 / ulAllottedEraseCnt_D3);
			}
			ubValueWorst = gubTemp;
			break;

		case 0x0205 ://100-erase percent, re-define, same as 0103h
			if (EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
				gubTemp = 100;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)EraseCountInfo.TypeD3.ulAvgEraseCnt * 100 / ulAllottedEraseCnt_D3);
			}
			ubValueWorst = 100 - gubTemp;
			break;

		case 0x0206 ://ProgramFailPercent (by planebank)
			if (FailCountInfo.uwMaxProgramFailCntPerPlane >= guwBadBlockBoundary) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)(FailCountInfo.uwMaxProgramFailCntPerPlane * 100 / guwBadBlockBoundary);
			}
			ubValueWorst = gubTemp;
			break;
		case 0x0207 ://100 - ProgramFailPercent (by planebank)
			if ( FailCountInfo.uwMaxProgramFailCntPerPlane >= guwBadBlockBoundary) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)(FailCountInfo.uwMaxProgramFailCntPerPlane * 100 / guwBadBlockBoundary);
			}
			ubValueWorst = 100 - gubTemp;
			break;
		case 0x0208 ://EraseFailPercent	(by planebank)
			if ( FailCountInfo.uwMaxEraseFailCntPerPlane >= guwBadBlockBoundary) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)(FailCountInfo.uwMaxEraseFailCntPerPlane * 100 / guwBadBlockBoundary);
			}
			ubValueWorst = gubTemp;
			break;
		case 0x0209 ://100 - EraseFailPercent (by planebank)
			if ( FailCountInfo.uwMaxEraseFailCntPerPlane >= guwBadBlockBoundary) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)(FailCountInfo.uwMaxEraseFailCntPerPlane * 100 / guwBadBlockBoundary);
			}
			ubValueWorst = 100 - gubTemp;
			break;
		case 0x020A :
			if ( EraseCountInfo.TypeD3.ulMaxEraseCnt >= ulAllottedEraseCnt_D3) {
				gubTemp = 100;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)EraseCountInfo.TypeD3.ulMaxEraseCnt * 100 / ulAllottedEraseCnt_D3);
			}
			ubValueWorst = gubTemp;
			break;

		case 0x020B :
			if (EraseCountInfo.TypeD3.ulMaxEraseCnt >= ulAllottedEraseCnt_D3) {
				gubTemp = 100;
			}
			else {
				gubTemp = (UBYTE)((ULLONG)EraseCountInfo.TypeD3.ulMaxEraseCnt * 100 / ulAllottedEraseCnt_D3);
			}
			ubValueWorst = 200 - gubTemp;
			break;

		case 0x020C ://B0 bad block rate V2, B1B2B3B4B5 0
			ubValueWorst = BadBlockInfo.ubMaxLaterBadRatePerPlane;
			break;

		case 0x020D ://B0 bad block rate V2, B1B2B3B4B5 0
			if (BadBlockInfo.ubMaxLaterBadRatePerPlane < 100) {
				ubValueWorst = 100 - BadBlockInfo.ubMaxLaterBadRatePerPlane;
			}
			else {
				ubValueWorst = 0;
			}
			break;


		case 0x0300 ://100 - Current Temperature
			if (ubTemperature < 100) {
				ubValueWorst = 100 - ubTemperature;
			}
			else {
				ubValueWorst = 0;
			}
			break;
		case 0x0301 ://100 - Highest Temperature value
			if (AtaCfg.ubTemperatureMax < 100) {
				ubValueWorst = 100 - AtaCfg.ubTemperatureMax;
			}
			else {
				ubValueWorst = 0;
			}
			break;
		case 0x0302 ://100 - constant Temperature value
			ubValueWorst = 100 - 30;
			break;

		case 0x0310 ://Current Temperature
			ubValueWorst = ubTemperature;
			break;

		case 0x0311 ://Highest Temperature value
			ubValueWorst = AtaCfg.ubTemperatureMax;
			break;

		case 0x0312 ://constant Temperature value
			ubValueWorst = 30;
			break;

		case 0x0313 ://128 - Current Temperature
			if (ubTemperature < 128) {
				ubValueWorst = 128 - ubTemperature;
			}
			else {
				ubValueWorst = 0;
			}
			break;

		case 0x0314 ://128 - Highest Temperature value
			if (AtaCfg.ubTemperatureMax < 128) {
				ubValueWorst = 128 - AtaCfg.ubTemperatureMax;
			}
			else {
				ubValueWorst = 0;
			}
			break;
		case 0x0400://non-4K aligned write percent
			if (AtaCfg.ulWriteNon4KAlignedCmdCnt >= AtaCfg.ulWriteTotalCmdCnt) {
				ubValueWorst = 100;
			}
			else {
				if (AtaCfg.ulWriteTotalCmdCnt) {
					ubValueWorst = (UBYTE)((ULLONG)AtaCfg.ulWriteNon4KAlignedCmdCnt * 100 / AtaCfg.ulWriteTotalCmdCnt);
				}
				else {
					ubValueWorst = 0;
				}
			}

			break;
		default :
			ubValueWorst = 0xFF;
			break;
	}

	return ubValueWorst;
#endif
#if(RDT)
	return 0;
#endif
}


ULLONG Get_SMART_Attribute_Data(UWORD  uwData_MP, SMART_BADBLOCK_INFO BadBlockInfo, SMART_ERASECOUNT_INFO EraseCountInfo,  SMART_FAILCOUNT_INFO FailCountInfo, UBYTE ubTemperature)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);
	ULLONG uoData = 0;
	UWORD uwi;
	UBYTE ubBadBlockPercent;
	U32 ulSpareBlockCount;
	U32 ulPhyErrCount;
	U32 ulAllottedEraseCnt_D3 = gulMaxPECycle_D3;
	U32 ulAllottedEraseCnt_D1 = gulMaxPECycle_D1;
	U32 ulRightNowTimeInMinutes = (U32)(GetRTT_m());

	//////////////// need get from VT  /////////////////
	ULLONG uoHostReadSector = VT->guoTotalHostReadSectorCnt;
	ULLONG uoHostWriteSector = VT->guoTotalHostWriteSectorCnt;
	ULLONG uoNandReadSector = VT->guoTotalNandReadSectorCnt;

#if TLC
	ULLONG uoNandWriteSector_D1 = (VT->guoTotalNandWritePlaneCnt_D1Data * gubSectorsPerPlane);
	ULLONG uoNandWriteSector = ((	VT->guoTotalNandWritePlaneCnt_D3SLCTable
	                                + 	VT->guoTotalNandWritePlaneCnt_D3SLCData
	                                + 	VT->guoTotalNandWritePlaneCnt_D3TLCData)
	                            * gubSectorsPerPlane);
#else
	ULLONG uoNandWriteSector = VT->guoTotalNandWriteSectorCnt;
#endif
	//========================================================//
	U32 ulPowerOnStartTimeInHour = (VT->gulPowerOnTimeInMinutes + (U32)(ulRightNowTimeInMinutes - gulLastSavedTimeInMinutes)) / UPDATE_TIME_INTERVAL_MIN;
	U32 ulPowerOnCount = VT->gulPowerOnCnt;
	U32 ulReadMoveTableCount = VT->gulRUTMappingUpdateCnt;
	U32 ulCmdECCCount = VT->gulHostReadECCCnt;
	U32 ulHostReadE3DCount = VT->gulHostReadE3DCnt;
	U32 ulCmdTimeOutCount = 0;
	U32 ulWriteCRCCnt = VT->gulWriteCRCCnt;
	U32 ulReadCRCCnt = VT->gulReadCRCCnt;
	UWORD uwSystemUpdateCount = VT->guwSystemUpdateCount;
	UWORD uwFWUpdateCount = VT->guwFWUpdateCount;
	///////////////////////////////////////////////////
	UWORD uwBadBlockBoundary = guwBadBlockBoundary;
	U32 ulAlpha = gulMaxPECycle_D3;
	UBYTE ubBeta = 10;
	UBYTE ubGamma = 100;
	UWORD uwTemp1, uwTemp2, uwTemp3;
	switch (uwData_MP) {
		case 0x0001 :
			uoData = 0;
			break;
		case 0x0002 :
			uoData = 0x646464646464ULL;
			break;
			//-----------------------------------------------//
		case 0x0100 ://ECC
			//the format belows may use B0,B1,B2,B3
			uoData = (ULLONG)ulCmdECCCount;
			break;
		case 0x0101 ://B0B1 0, B2 ECC, B3B4B5 0
			//the format belows may use B2,B3,B4,B5
			uoData = (ULLONG)((ulCmdECCCount) << 16);
			break;
		case 0x0102 ://UNC Error Count (same as 0x0500..., due to re-definition in document)
			uoData = (ULLONG)FailCountInfo.ulTotalReadFailCnt;
			break;
		case 0x0103 ://SATA PHY error count, SATA PHY Event Counter = 02h (R_ERRp response for Data FIS) + 05h (R_ERRp response for non-Data FIS), 02h=03h+04h, 05h=06h+07h
			ulPhyErrCount = 0;
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_1] >> 8) & 0xFF; // 03h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_1] >> 16) & 0xFF; // 04h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_1] >> 24) & 0xFF; // 06h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_2]) & 0xFF; // 07h
			uoData = (ULLONG)ulPhyErrCount;
			break;
		case 0x0104 ://SATA PHY error count, reserve
			uoData = (ULLONG)(ulWriteCRCCnt + ulReadCRCCnt);
			break;
		case 0x0105 ://SATA PHY error count = 01+03+04+06+07+08+09+0A+0F+10+12+13
			ulPhyErrCount = 0;
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_1]) & 0xFF; // 01h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_1] >> 8) & 0xFF; // 03h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_1] >> 16) & 0xFF; // 04h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_1] >> 24) & 0xFF; // 06h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_2]) & 0xFF; // 07h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_2] >> 8) & 0xFF; // 08h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_2] >> 16) & 0xFF; // 09h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_2] >> 24) & 0xFF; // 0Ah
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_3]) & 0xFF; // 0Fh
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_3] >> 8) & 0xFF; // 10h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_3] >> 16) & 0xFF; // 12h
			ulPhyErrCount += (HL[HL_PHY_EVT_CNT_3] >> 24) & 0xFF; // 13h
			uoData = (ULLONG)ulPhyErrCount;
			break;
		case 0x0106 ://SATA E3D error count
			uoData = (ULLONG)ulHostReadE3DCount;
			break;
			//-----------------------------------------------//
		case 0x0200 ://Total Erase Count
		case 0x0220 :
			if (uwData_MP & BIT5) {
				uoData = (ULLONG)EraseCountInfo.TypeD1.ulTotalEraseCnt;
			}
			else {
				uoData = (ULLONG)EraseCountInfo.TypeD3.ulTotalEraseCnt;
			}
			break;
		case 0x0201 ://Max Erase Count
		case 0x0221 :
			if (uwData_MP & BIT5) {
				uoData = (ULLONG)EraseCountInfo.TypeD1.ulMaxEraseCnt;
			}
			else {
				uoData = (ULLONG)EraseCountInfo.TypeD3.ulMaxEraseCnt;
			}
			break;
		case 0x0202 ://Avg Erase Count
		case 0x0222 :
			if (uwData_MP & BIT5) {
				uoData = (ULLONG)EraseCountInfo.TypeD1.ulAvgEraseCnt;
			}
			else {
				uoData = (ULLONG)EraseCountInfo.TypeD3.ulAvgEraseCnt;
			}

			break;
		case 0x0203 ://B0B1 Avg Erase Cnt, B2B3 Max Earse Cnt, B4B5 0
		case 0x0223 :
			if (uwData_MP & BIT5) {
				uwTemp1 = (UWORD)((EraseCountInfo.TypeD1.ulAvgEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD1.ulAvgEraseCnt);
				uwTemp2 = (UWORD)((EraseCountInfo.TypeD1.ulMaxEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD1.ulMaxEraseCnt);
			}
			else {
				uwTemp1 = (UWORD)((EraseCountInfo.TypeD3.ulAvgEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD3.ulAvgEraseCnt);
				uwTemp2 = (UWORD)((EraseCountInfo.TypeD3.ulMaxEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD3.ulMaxEraseCnt);
			}
			uoData = (ULLONG)(uwTemp1 | (uwTemp2 << 16));
			break;
		case 0x0204 ://B0B1 Max Erase Cnt, B2B3 Avg Erase Cnt, B4B5 0
		case 0x0224 :
			if (uwData_MP & BIT5) {
				uwTemp1 = (UWORD)((EraseCountInfo.TypeD1.ulAvgEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD1.ulAvgEraseCnt);
				uwTemp2 = (UWORD)((EraseCountInfo.TypeD1.ulMaxEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD1.ulMaxEraseCnt);
			}
			else {
				uwTemp1 = (UWORD)((EraseCountInfo.TypeD3.ulAvgEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD3.ulAvgEraseCnt);
				uwTemp2 = (UWORD)((EraseCountInfo.TypeD3.ulMaxEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD3.ulMaxEraseCnt);
			}
			uoData = (ULLONG)(uwTemp2 | (uwTemp1 << 16));
			break;
		case 0x0205 ://B0B1 Min Erase Cnt, B2B3 Max Erase Cnt, B4B5 Avg Erase Cnt
		case 0x0225 :
			if (uwData_MP & BIT5) {
				uwTemp1 = (UWORD)((EraseCountInfo.TypeD1.ulAvgEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD1.ulAvgEraseCnt);
				uwTemp2 = (UWORD)((EraseCountInfo.TypeD1.ulMaxEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD1.ulMaxEraseCnt);
				uwTemp3 = (UWORD)((EraseCountInfo.TypeD1.ulMinEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD1.ulMinEraseCnt);
			}
			else {
				uwTemp1 = (UWORD)((EraseCountInfo.TypeD3.ulAvgEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD3.ulAvgEraseCnt);
				uwTemp2 = (UWORD)((EraseCountInfo.TypeD3.ulMaxEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD3.ulMaxEraseCnt);
				uwTemp3 = (UWORD)((EraseCountInfo.TypeD3.ulMinEraseCnt > 0xFFFF) ? 0xFFFF : EraseCountInfo.TypeD3.ulMinEraseCnt);
			}
			uoData = (ULLONG)(uwTemp3 | (uwTemp2 << 16) | ((ULLONG)uwTemp1 << 32));
			break;
		case 0x0206 ://Avg Erase Count(Worst)
		case 0x0226 :
			if (uwData_MP & BIT5) {
				uoData = (ULLONG)EraseCountInfo.TypeD1.ulWorstAvgEraseCnt;
			}
			else {
				uoData = (ULLONG)EraseCountInfo.TypeD3.ulWorstAvgEraseCnt;
			}
			break;
		case 0x0207 ://B0B1 EraseCount Max raw value, B2 EraseCnt Avg, B3 Alternative block rest number, B4B5 0
			//B0B1
			if ( EraseCountInfo.TypeD3.ulMaxEraseCnt <= ulAlpha ) {
				uoData = 0;
			}
			else if ( EraseCountInfo.TypeD3.ulMaxEraseCnt <= (ulAlpha + ubGamma) ) {
				uoData = 1;
			}
			else {
				uoData = EraseCountInfo.TypeD3.ulMaxEraseCnt;
			}

			//B2
			if ( EraseCountInfo.TypeD3.ulAvgEraseCnt <= ulAlpha ) {
				uoData |= (0 << 16);
			}
			else {
				gubTemp = (UBYTE)(1 + ((EraseCountInfo.TypeD3.ulAvgEraseCnt - ulAlpha) / ubGamma));
				uoData |= (gubTemp << 16);
			}

			//B3
			if (BadBlockInfo.ulTotalEarlyLaterBadCnt >= guwBadBlockBoundary) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)(guwBadBlockBoundary - BadBlockInfo.ulTotalEarlyLaterBadCnt);
			}

			if (gubTemp >= ubBeta) {
				uoData |= (0 << 24);
			}
			else {
				uoData |= (gubTemp << 24);
			}

			break;
		case 0x0208 ://B0 Wear Range Delta, B1B2B3B4B5 0
		case 0x0228 :
			// [(Most Wear Block – Least Wear Block) / Max. P/E cycles] x 100
			if (uwData_MP & BIT5) {
				if (EraseCountInfo.TypeD1.ulMaxEraseCnt > EraseCountInfo.TypeD1.ulAvgEraseCnt) {
					gubTemp = (UBYTE)((ULLONG)(EraseCountInfo.TypeD1.ulMaxEraseCnt - EraseCountInfo.TypeD1.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D1);
				}
				else {
					gubTemp = 0;
				}
			}
			else {
				if (EraseCountInfo.TypeD3.ulMaxEraseCnt > EraseCountInfo.TypeD3.ulAvgEraseCnt) {
					gubTemp = (UBYTE)((ULLONG)(EraseCountInfo.TypeD3.ulMaxEraseCnt - EraseCountInfo.TypeD3.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D3);
				}
				else {
					gubTemp = 0;
				}

			}
			uoData = (ULLONG)gubTemp;
			break;
		case 0x0209 ://B0 Current Percent Life Used, B1B2B3B4B5 0
		case 0x0229 :
			if (uwData_MP & BIT5) {
				if ( EraseCountInfo.TypeD1.ulAvgEraseCnt >= ulAllottedEraseCnt_D1) {
					gubTemp = 100;
				}
				else {
					gubTemp = (UBYTE)(100 - ((ULLONG)(ulAllottedEraseCnt_D1 - EraseCountInfo.TypeD1.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D1));
				}
			}
			else {
				if ( EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
					gubTemp = 100;
				}
				else {
					gubTemp = (UBYTE)(100 - ((ULLONG)(ulAllottedEraseCnt_D3 - EraseCountInfo.TypeD3.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D3));
				}

			}
			uoData = (ULLONG)gubTemp;
			break;
		case 0x020A ://B0 SSD life left, B1B2B3B4B5 0
		case 0x022A :
			if (uwData_MP & BIT5) {
				if ( EraseCountInfo.TypeD1.ulAvgEraseCnt >= ulAllottedEraseCnt_D1) {
					gubTemp = 0;
				}
				else {
					gubTemp = (UBYTE)((ULLONG)(ulAllottedEraseCnt_D1 - EraseCountInfo.TypeD1.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D1);
				}
			}
			else {
				if ( EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
					gubTemp = 0;
				}
				else {
					gubTemp = (UBYTE)((ULLONG)(ulAllottedEraseCnt_D3 - EraseCountInfo.TypeD3.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D3);
				}
			}
			uoData = (ULLONG)gubTemp;
			break;
		case 0x020B ://min erase count
		case 0x022B :
			if (uwData_MP & BIT5) {
				uoData = (ULLONG)EraseCountInfo.TypeD1.ulMinEraseCnt;
			}
			else {
				uoData = (ULLONG)EraseCountInfo.TypeD3.ulMinEraseCnt;
			}
			break;

		case 0x020C ://total erase count in block
		case 0x022C :
			if (uwData_MP & BIT5) {
				uoData = (ULLONG)EraseCountInfo.TypeD1.ulTotalEraseCnt *  gubPlanesPerPage;
			}
			else {
				uoData = (ULLONG)EraseCountInfo.TypeD3.ulTotalEraseCnt *  gubPlanesPerPage;
			}
			break;
		case 0x020D ://B0 Current Percent Life Used V2, B1B2B3B4B5 0
			if ((EraseCountInfo.TypeD3.ulAvgEraseCnt + VT->gulPreviousMaxEraseCnt_D3) >= ulAllottedEraseCnt_D3) {
				gubTemp = 100;
			}
			else {
				gubTemp = (UBYTE)(100 - (((ULLONG)(ulAllottedEraseCnt_D3 - (EraseCountInfo.TypeD3.ulAvgEraseCnt + VT->gulPreviousMaxEraseCnt_D3)) * 100) / ulAllottedEraseCnt_D3));
			}

			uoData = (ULLONG)gubTemp;
			break;
		case 0x020E ://B0 SSD life left V2, B1B2B3B4B5 0
			if ((EraseCountInfo.TypeD3.ulAvgEraseCnt + VT->gulPreviousMaxEraseCnt_D3) >= ulAllottedEraseCnt_D3) {
				gubTemp = 0;
			}
			else {
				gubTemp = (UBYTE)(((ULLONG)(ulAllottedEraseCnt_D3 - (EraseCountInfo.TypeD3.ulAvgEraseCnt + VT->gulPreviousMaxEraseCnt_D3)) * 100) / ulAllottedEraseCnt_D3);
			}

			uoData = (ULLONG)gubTemp;
			break;
		case 0x020F : //TSB return fix erase count = 0, but this is same as 0x0001
		case 0x022F :
			uoData = 0;
			break;
		case 0x0210 ://B0B1: 100-SSD life left, B2B3: max bad block percent, B4B5: 0
		case 0x0230 :
			if (uwData_MP & BIT5) {
				if ( EraseCountInfo.TypeD1.ulAvgEraseCnt >= ulAllottedEraseCnt_D1) {
					uoData = 100;
				}
				else {
					uoData = (UBYTE)(100 - (((ULLONG)(ulAllottedEraseCnt_D1 - EraseCountInfo.TypeD1.ulAvgEraseCnt) * 100) / ulAllottedEraseCnt_D1));
				}
			}
			else {
				if ( EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
					uoData = 100;
				}
				else {
					uoData = (UBYTE)(100 - (((ULLONG)(ulAllottedEraseCnt_D3 - EraseCountInfo.TypeD3.ulAvgEraseCnt) * 100) / ulAllottedEraseCnt_D3));
				}

			}

			gubTemp  = (BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane > guwBadBlockBoundary ? 100 : (UBYTE)((ULLONG)BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane * 100 / guwBadBlockBoundary));
			uoData |= (gubTemp << 16);

			break;

		case 0x0211 ://B0B1: SSD life left, B2B3: max bad block percent, B4B5: 0
		case 0x0231 :
			if (uwData_MP & BIT5) {
				if ( EraseCountInfo.TypeD1.ulAvgEraseCnt >= ulAllottedEraseCnt_D1) {
					uoData = 0;
				}
				else {
					uoData = (UBYTE)(((ULLONG)(ulAllottedEraseCnt_D1 - EraseCountInfo.TypeD1.ulAvgEraseCnt) * 100) / ulAllottedEraseCnt_D1);
				}
			}
			else {
				if ( EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
					uoData = 0;
				}
				else {
					uoData = (UBYTE)(((ULLONG)(ulAllottedEraseCnt_D3 - EraseCountInfo.TypeD3.ulAvgEraseCnt) * 100) / ulAllottedEraseCnt_D3);
				}

			}

			gubTemp  = (BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane > guwBadBlockBoundary ? 100 : (UBYTE)((ULLONG)BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane * 100 / guwBadBlockBoundary));
			uoData |= (gubTemp << 16);

			break;

		case 0x0212 ://Erase Count in Spec
		case 0x0232 :
			if (uwData_MP & BIT5) {
				uoData = (ULLONG)ulAllottedEraseCnt_D1;
			}
			else {
				uoData = (ULLONG)ulAllottedEraseCnt_D3;
			}
			break;
		case 0x0213 ://B0 SSD life left, B1B2B3B4B5 0, min = 1
		case 0x0233 :
			if (uwData_MP & BIT5) {
				if ( EraseCountInfo.TypeD1.ulAvgEraseCnt >= ulAllottedEraseCnt_D1) {
					gubTemp = 1;
				}
				else {
					gubTemp = (UBYTE)((ULLONG)(ulAllottedEraseCnt_D1 - EraseCountInfo.TypeD1.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D1);
				}
			}
			else {
				if ( EraseCountInfo.TypeD3.ulAvgEraseCnt >= ulAllottedEraseCnt_D3) {
					gubTemp = 1;
				}
				else {
					gubTemp = (UBYTE)((ULLONG)(ulAllottedEraseCnt_D3 - EraseCountInfo.TypeD3.ulAvgEraseCnt) * 100 / ulAllottedEraseCnt_D3);
				}
			}
			uoData = (ULLONG)gubTemp;
			break;
			//-----------------------------------------------//
		case 0x0300 ://Total Bad (early+later) Block Count
			uoData = (ULLONG)BadBlockInfo.ulTotalEarlyLaterBadCnt;
			break;
		case 0x0301 ://Total Early Bad Blcok Count
			uoData = (ULLONG)BadBlockInfo.ulTotalEarlyBadCnt;
			break;
		case 0x0302 ://Total Later Bad Blcok Count
			uoData = (ULLONG)BadBlockInfo.ulTotalLaterBadCnt;
			break;
		case 0x0303 ://Bad Block Boundary (CE)
			uoData = (ULLONG)(uwBadBlockBoundary << gubBurstsPerBankLog);
			break;
		case 0x0304 ://Bad Block Boundary (Plane)
			uoData = (ULLONG)uwBadBlockBoundary;
			break;
		case 0x0305 ://Spare Block Count (CE)
			ulSpareBlockCount  = (BadBlockInfo.uwMaxEarlyLaterBadCntPerCE > ((uwBadBlockBoundary << gubBurstsPerBankLog) * gubSelDieNumber) ? 0 : (((uwBadBlockBoundary << gubBurstsPerBankLog) * gubSelDieNumber) - BadBlockInfo.uwMaxEarlyLaterBadCntPerCE));
			uoData = (ULLONG)ulSpareBlockCount;
			break;
		case 0x0306 ://Spare Block Count (Plane)
			ulSpareBlockCount  = ( BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane > guwBadBlockBoundary ? 0 : (guwBadBlockBoundary - BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane));
			uoData = (ULLONG)ulSpareBlockCount;
			break;
		case 0x0307 ://later bad block (Worst, CE)
			uoData = (ULLONG)BadBlockInfo.uwMaxLaterBadCntPerCE;
			break;
		case 0x0308 ://later bad block (Worst, plane)
			uoData = (ULLONG)BadBlockInfo.uwMaxLaterBadCntPerPlane;
			break;
		case 0x0309 ://early bad block (Worst, CE)
			uoData = (ULLONG)BadBlockInfo.uwMaxEarlyBadCntPerCE;
			break;
		case 0x030A ://early bad block (Worst, Plane)
			uoData = (ULLONG)BadBlockInfo.uwMaxEarlyBadCntPerPlane;
			break;
		case 0x030B ://B0 bad block rate, B1B2B3B4B5 0
			ubBadBlockPercent = (BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane >= guwBadBlockBoundary) ? 100 :  ((BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane * 100) / guwBadBlockBoundary);
			uoData = (ULLONG)ubBadBlockPercent;
			break;
		case 0x030C ://B0B1 Total Early Bad Blcok Count, B2B3 0, B4B5 Total Later Bad Blcok Count
			uwTemp1 = (UWORD)((BadBlockInfo.ulTotalEarlyBadCnt > 0xFFFF) ? 0xFFFF : BadBlockInfo.ulTotalEarlyBadCnt);
			uwTemp2 = (UWORD)((BadBlockInfo.ulTotalLaterBadCnt > 0xFFFF) ? 0xFFFF : BadBlockInfo.ulTotalLaterBadCnt);
			uoData = (ULLONG)(uwTemp1 | ((ULLONG)uwTemp2 << 32));
			break;
		case 0x030D ://B0 Current Percent Spares
			//the definition is not common sense...
			ulSpareBlockCount  = ( BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane > guwBadBlockBoundary ? 0 : (guwBadBlockBoundary - BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane));
			gubTemp = (UBYTE)(((ULLONG)ulSpareBlockCount * 100 / guwBadBlockBoundary));
			uoData = (ULLONG)gubTemp;
			break;
		case 0x030E ://B0 100-Current Percent Spares
			ulSpareBlockCount  = ( BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane > guwBadBlockBoundary ? 0 : (guwBadBlockBoundary - BadBlockInfo.uwMaxEarlyLaterBadCntPerPlane));
			gubTemp = (UBYTE)(100 - (ULLONG)ulSpareBlockCount * 100 / guwBadBlockBoundary);
			uoData = (ULLONG)gubTemp;
			break;
		case 0x030F ://B0 bad block rate V2, B1B2B3B4B5 0
			ubBadBlockPercent = BadBlockInfo.ubMaxLaterBadRatePerPlane;
			uoData = (ULLONG)ubBadBlockPercent;
			break;
		case 0x0310 ://Spare Blocks Available(Plane)
			uoData = (ULLONG)guwInitialSpareBlocks;
			break;
		case 0x0311 ://Remaining Spare Blocks(Plane)
			ulSpareBlockCount = GetMinRemainingSpareBlocksPerPlane();
			uoData = (ULLONG)ulSpareBlockCount;
			break;
			//-----------------------------------------------//
		case 0x0400 ://Host Write (Sectors)
			uoData = uoHostWriteSector;
			break;
		case 0x0401 ://Host Write (MB)
			uoData = uoHostWriteSector >> 11; // devide 2048
			break;
		case 0x0402 ://Host Write (GB)
			uoData = uoHostWriteSector >> 21;
			break;
		case 0x0403 ://Host Write ( 65536 Sectors, 32MB)
			uoData = uoHostWriteSector >> 16;
			break;
		case 0x0404 ://Host Write (16MB)
			uoData = uoHostWriteSector >> 15;
			break;
		case 0x0410 ://NAND Write (Sectors)
			uoData = uoNandWriteSector;
			break;
		case 0x0411 ://Host Read (MB)
			uoData = uoNandWriteSector >> 11;
			break;
		case 0x0412 ://NAND Write (GB)
			uoData = uoNandWriteSector >> 21;
			break;
		case 0x0413 ://NAND Write (65536 Sectors, 32MB)
			uoData = uoNandWriteSector >> 16;
			break;
		case 0x0414 ://NAND Write (16MB)
			uoData = uoNandWriteSector >> 15;
			break;
		case 0x0420 ://Host Read (Sectors)
			uoData = uoHostReadSector;
			break;
		case 0x0421 ://Host Read (MB)
			uoData = uoHostReadSector >> 11;
			break;
		case 0x0422 ://Host Read (GB)
			uoData = uoHostReadSector >> 21;
			break;
		case 0x0423 ://Host Read (65536 Sectors, 32MB)
			uoData = uoHostReadSector >> 16;
			break;
		case 0x0424 ://Host Read (16MB)
			uoData = uoHostReadSector >> 15;
			break;
		case 0x0430 ://NAND Read (Sectors)
			uoData = uoNandReadSector;
			break;
		case 0x0431 ://NAND Read (MB)
			uoData = uoNandReadSector >> 11;
			break;
		case 0x0432 ://NAND Read (GB)
			uoData = uoNandReadSector >> 21;
			break;
		case 0x0433 ://NAND Read (65536 Sectors, 32MB)
			uoData = uoNandReadSector >> 16;
			break;
		case 0x0434 ://NAND Read (16MB)
			uoData = uoNandReadSector >> 15;
			break;
#if TLC
			//======================D1 Nand Write========================================//
		case 0x0440 ://NAND Write (Sectors)
			uoData = uoNandWriteSector_D1;
			break;
		case 0x0441 ://Host Write (MB)
			uoData = uoNandWriteSector_D1 >> 11;
			break;
		case 0x0442 ://NAND Write (GB)
			uoData = uoNandWriteSector_D1 >> 21;
			break;
		case 0x0443 ://NAND Write (65536 Sectors, 32MB)
			uoData = uoNandWriteSector_D1 >> 16;
			break;
		case 0x0444 ://NAND Write (16MB)
			uoData = uoNandWriteSector_D1 >> 15;
			break;
			//==========================================================================//
#endif
			//-----------------------------------------------//
		case 0x0500 ://Read Fail Count
			uoData = (ULLONG)FailCountInfo.ulTotalReadFailCnt;
			break;
		case 0x0501 ://Program Fail Count
			uoData = (ULLONG)FailCountInfo.ulTotalProgramFailCnt;
			break;
		case 0x0502 ://Erase Failure Count
			uoData = (ULLONG)FailCountInfo.ulTotalEraseFailCnt;
			break;
		case 0x0503 ://Read Fail Count
			uoData = (ULLONG)FailCountInfo.uwMaxReadFailCntPerPlane;
			break;
		case 0x0504 ://Program Fail Count
			uoData = (ULLONG)FailCountInfo.uwMaxProgramFailCntPerPlane;
			break;
		case 0x0505 ://Erase Failure Count
			uoData = (ULLONG)FailCountInfo.uwMaxEraseFailCntPerPlane;
			break;
		case 0x0506 ://RS Recovery Mark Bad Count
			uoData = (ULLONG)FailCountInfo.ulTotalRSRecoveryMarkBadCnt;
			break;
			//-----------------------------------------------//
		case 0x0600 ://Power on hours
			uoData = (ULLONG)ulPowerOnStartTimeInHour;
			break;
		case 0x0607 ://Power on/off cycles
			uoData = (ULLONG)ulPowerOnCount;
			break;
		case 0x0608 ://Unexpected Power Loss Count (non standby)
			uoData = (ULLONG)VT->gulNonStandByPowerDownCnt;
			break;
		case 0x0609 ://Unexpected Power Loss Count
			uoData = (ULLONG)VT->gulPowerCyclingCnt;
			break;
			//-----------------------------------------------//
		case 0x0700 ://Device Capacity
			uoData = gulNativeSize;
			break;
		case 0x0701 ://User Capacity
			uoData = gulMediaSize.All;
			break;
		case 0x0702 ://B0 Protect Mode, B1B2B3B4B5 0, protect mode: 00:RW 03:WP
			uoData = (ULLONG)(gubIsWriteProtect ? 0x03 : 0);
			break;
		case 0x0703 ://B0 Protect Mode, B1B2B3B4B5 0, protect mode: 00:RW 02:WP
			uoData = (ULLONG)(gubIsWriteProtect ? 0x02 : 0);
			break;
		case 0x0704 ://FW Code Copy Count
			uoData = (ULLONG)uwFWUpdateCount;
			break;
		case 0x0705 ://System Table Copy Count
			uoData = (ULLONG)uwSystemUpdateCount;
			break;
		case 0x0706 ://ReadMoveTableCnt
			uoData = (ULLONG)ulReadMoveTableCount;
			break;
		case 0x0707 ://Command Timeout Count
			uoData = (ULLONG)ulCmdTimeOutCount;
			break;
		case 0x0708 ://Guarantee flush info, B0B2B3B4B5 0, B1: 0:disable, 1:enable
			uoData = (ULLONG)((ULLONG)gubGUARANTEE_FLUSH << 8);
			break;
		case 0x0709 ://B0 Protect Mode, B1B2B3B4B5 0, protect mode: 00:RW 03:GPIO WP, 07:TRUE WP
			if (gubIsWriteProtect & BIT_TRULY_WRITEPROTECT) {
				uoData = 7;
			}
			else if (gubIsWriteProtect & BIT_GPIO_WRITEPROTECT) {
				uoData = 3;
			}
			else {
				uoData = 0;
			}
			break;
		case 0x070A://SMART offline scan UNC Count
			uoData = (ULLONG)gulSmartOfflineUNCCount;
			break;
		case 0x070B://non 4K aligned cmd count
			uoData = (ULLONG)AtaCfg.ulWriteNon4KAlignedCmdCnt;
			break;
			//-----------------------------------------------//
		case 0x0800 ://B0B1 Current Temp, B2B3 Min Temp, B4B5 MaxTemp
			uoData = (ULLONG)(ubTemperature | ((ULLONG)AtaCfg.ubTemperatureMin << 16) | ((ULLONG)AtaCfg.ubTemperatureMax << 32));
			break;
		case 0x0801 ://B0B1 Current Temp, B2B3 0, B4B5 0
			uoData = (ULLONG)(ubTemperature);
			break;
		case 0x0802 ://B0B1 Current Temp, B2 Min Temp, B3 Max Temp, B4B5 0
			uoData = (ULLONG)(ubTemperature | ((ULLONG)AtaCfg.ubTemperatureMin << 16) | ((ULLONG)AtaCfg.ubTemperatureMax << 24));
			break;
			//-----------------------------------------------//
		case 0xF000 : //return total valid count
			uoData = 0;
			for (uwi = 0; uwi < guwFWTotalUnitNum; uwi++) {
				if (gulVC[uwi].B.ulValidCount != DefaultVC) {
					uoData += gulVC[uwi].B.ulValidCount;
				}
			}
			break;

		case 0xF001 : //return total valid LCA count
			uoData = ((ULLONG)VT->gulTotalValidLCACnt << 3);
			break;

		case 0xF010 : //HB retry count
			uoData = (ULLONG)(VT->gulReadRetryCnt);
			break;

		case 0xF011 : //SB retry count
			uoData = (ULLONG)(VT->gulReadSBCnt);
			break;

		case 0xF012 : //RS retry count
			uoData = (ULLONG)(VT->gulReadRSCnt);
			break;

		case 0xF013 : //RS Recover Fail count
			uoData = (ULLONG)(VT->gulRSRecoverFailCnt);
			break;
			//-----------------------------------------------//
		default :
			uoData = 0xFFFFFFFFFFFFFFFFULL;
			break;
	}

	uoData &= 0x0000FFFFFFFFFFFFULL; // 6 byte data
	return uoData;
#endif
#if(RDT)
	return 0;
#endif
}

void Set_SMART_Attribute_Entry(SMART_Attribute_Entry* AttributeArray, SMART_BADBLOCK_INFO* BadBlockInfo)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubIndex = 0;
	UBYTE ubID, ubFlag[2], ubValue, ubWorst, ubThreshold;
	ULLONG uoData;
	UBYTE ubSaveVT;

	mSE_SV(4, (U32)AttributeArray,  512, 0, 0);
	HandleStopRW(1);
	flaLoadSystemAreaSet(0, gubSystemSet, (U32)gubHostTable, MARK_SYS);
	MP_SMART_Attribute_Entry * MP_Parameter = (MP_SMART_Attribute_Entry *)(&BUFB_BASE[0x400]);
	mSE_COPY((U32)(&gubHostTable[M_SysInfo_SmartMPBlk]), (U32) MP_Parameter, 0x200);
	guwInitialSpareBlocks = ((UWORD*)gubHostTable)[(M_SysInfo_uwInitialSpareBlocks / 2)];

	SMART_ERASECOUNT_INFO EraseCountInfo;
	//SMART_BADBLOCK_INFO BadBlockInfo;
	SMART_FAILCOUNT_INFO FailCountInfo;
	UBYTE ubTemperature;

	ubTemperature = I2C_Temperature(0);
	ubSaveVT = 1;
	if ( (ubTemperature > AtaCfg.ubTemperatureMax) && (ubTemperature < 0x80) && (AtaCfg.ubTemperatureMax < 0x80)) {
		AtaCfg.ubTemperatureMax = ubTemperature;
	}
	else if ( (ubTemperature > AtaCfg.ubTemperatureMax) && (ubTemperature >= 0x80) && (AtaCfg.ubTemperatureMax >= 0x80)) {
		AtaCfg.ubTemperatureMax = ubTemperature;
	}
	else if ((ubTemperature < 0x80) && (AtaCfg.ubTemperatureMax >= 0x80)) {
		AtaCfg.ubTemperatureMax = ubTemperature;
	}
	else if ((ubTemperature < AtaCfg.ubTemperatureMin) && (ubTemperature < 0x80) && (AtaCfg.ubTemperatureMin < 0x80)) {
		AtaCfg.ubTemperatureMin = ubTemperature;
	}
	else if ( (ubTemperature < AtaCfg.ubTemperatureMin) && (ubTemperature >= 0x80) && (AtaCfg.ubTemperatureMin >= 0x80)) {
		AtaCfg.ubTemperatureMin = ubTemperature;
	}
	else if ((ubTemperature >= 0x80) && (AtaCfg.ubTemperatureMin < 0x80)) {
		AtaCfg.ubTemperatureMin = ubTemperature;
	}
	else {
		ubSaveVT = 0;
	}

	CalculateEraseCountInfo(&EraseCountInfo);
	if (ENABLE_RUT2DBT) {
		CalculateFailAndBadCountInfo(&FailCountInfo, BadBlockInfo);
	}
	else {
		gubErrorCode = SET_ABRT;
		return;
	}
	gubStopFTL = 0;

	for (ubIndex = 0; ubIndex < AttributeTotalEntry; ubIndex++) {
		ubID = MP_Parameter[ubIndex].ubID;
		if (ubID != 0) {
			ubFlag[0] = MP_Parameter[ubIndex].ubFlag[0];
			ubFlag[1] = MP_Parameter[ubIndex].ubFlag[1];
			ubValue = Get_SMART_Attribute_Value_Worst(MP_Parameter[ubIndex].uwValue, *BadBlockInfo, EraseCountInfo, FailCountInfo, ubTemperature);
			ubWorst = Get_SMART_Attribute_Value_Worst(MP_Parameter[ubIndex].uwWorst, *BadBlockInfo, EraseCountInfo, FailCountInfo, ubTemperature);
			uoData = Get_SMART_Attribute_Data(MP_Parameter[ubIndex].uwData, *BadBlockInfo, EraseCountInfo, FailCountInfo, ubTemperature);
			ubThreshold = MP_Parameter[ubIndex].ubThreshold;
		}
		else {
			ubFlag[0] = 0;
			ubFlag[1] = 0;
			ubValue = 0;
			ubWorst = 0;
			uoData = 0;
			ubThreshold = 0;
		}

		AttributeArray[ubIndex].ubID = ubID;
		AttributeArray[ubIndex].ubFlag[0] = ubFlag[0];
		AttributeArray[ubIndex].ubFlag[1] = ubFlag[1];
		AttributeArray[ubIndex].ubValue = ubValue;
		AttributeArray[ubIndex].ubWorst = ubWorst;
		AttributeArray[ubIndex].ubRaw[0] = (UBYTE)(uoData);
		AttributeArray[ubIndex].ubRaw[1] = (UBYTE)(uoData >> 8);
		AttributeArray[ubIndex].ubRaw[2] = (UBYTE)(uoData >> 16);
		AttributeArray[ubIndex].ubRaw[3] = (UBYTE)(uoData >> 24);
		AttributeArray[ubIndex].ubRaw[4] = (UBYTE)(uoData >> 32);
		AttributeArray[ubIndex].ubRaw[5] = (UBYTE)(uoData >> 40);
		AttributeArray[ubIndex].ubReserved = 0;
		AttributeArray[ubIndex].ubThreshold = ubThreshold;
	}

	//利用SMART Array剩餘的32byte(byte 480~511)儲存客製化資訊,由MP在開卡時填入
	ubIndex = AttributeTotalEntry;
	memcpy(&AttributeArray[(ubIndex)], &BUFB_BASE[0x400 + (ubIndex * 16)], 32);

	if (ubSaveVT) {
		if (ENABLE_WRITEPROTECT) {
			if (((gubIsWriteProtect & BIT_TRULY_WRITEPROTECT) == FALSE) && ((gubIsWriteProtect & BIT_ERR_UNIT_WRITEPROTECT) == FALSE) && ((gubIsWriteProtect & BIT_FLASH_INIT_FAIL_WRITEPROTECT) == FALSE)) {
				SaveAtaConfig();
			}
		}
		else {
			SaveAtaConfig();
		}
	}

#endif
}

void CalculateEraseCountInfo(SMART_ERASECOUNT_INFO *EraseCountInfo)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UWORD uwUnitIndex;
	U32 ulUnitEraseCnt;

	//only know erase count per unit
	EraseCountInfo->TypeD3.ulMaxEraseCnt = 0;
	EraseCountInfo->TypeD3.ulMinEraseCnt = 0xFFFF;
	EraseCountInfo->TypeD3.uwMaxEraseCnt_Index = 0;
	EraseCountInfo->TypeD3.uwMinEraseCnt_Index = 0;

	UWORD uwTotalUnitNumExcludeVT = 0;

	U32 ulSumEC = 0;
	for (uwUnitIndex = 0; uwUnitIndex < guwFWTotalUnitNum; uwUnitIndex++) {
		if ((uwUnitIndex == VT->guwVTAreaFreeTarget[0].B.uwTarget)
		        || (uwUnitIndex == VT->guwVTAreaFreeTarget[1].B.uwTarget)
		        || (uwUnitIndex == VT->guwVTAreaFreeTarget[2].B.uwTarget)
		        || (uwUnitIndex == VT->guwVTAreaFreeTarget[3].B.uwTarget)) {

			continue;
		}

		ulUnitEraseCnt = gulEC_D3[uwUnitIndex].B.ulEraseCount;
		ulSumEC += gulEC_D3[uwUnitIndex].B.ulEraseCount;

		if (ulUnitEraseCnt > EraseCountInfo->TypeD3.ulMaxEraseCnt) {
			EraseCountInfo->TypeD3.ulMaxEraseCnt = ulUnitEraseCnt;
			EraseCountInfo->TypeD3.uwMaxEraseCnt_Index = uwUnitIndex;
		}

		if (ulUnitEraseCnt < EraseCountInfo->TypeD3.ulMinEraseCnt) {
			EraseCountInfo->TypeD3.ulMinEraseCnt = ulUnitEraseCnt;
			EraseCountInfo->TypeD3.uwMinEraseCnt_Index = uwUnitIndex;
		}

		uwTotalUnitNumExcludeVT++;
	}

	EraseCountInfo->TypeD3.ulTotalEraseCnt = ulSumEC;
	if (uwTotalUnitNumExcludeVT) {
		EraseCountInfo->TypeD3.ulAvgEraseCnt = ulSumEC / uwTotalUnitNumExcludeVT;
	}
	EraseCountInfo->TypeD3.ulWorstAvgEraseCnt = EraseCountInfo->TypeD3.ulAvgEraseCnt; //due to erase in unit, every block's erase count in that unit is the same, I don't know what to return..., so return avg erase cnt.

	if (TLC) {
		ulSumEC = 0;
		EraseCountInfo->TypeD1.ulMaxEraseCnt = 0;
		EraseCountInfo->TypeD1.ulMinEraseCnt = 0xFFFF;
		EraseCountInfo->TypeD1.uwMaxEraseCnt_Index = 0;
		EraseCountInfo->TypeD1.uwMinEraseCnt_Index = 0;

		for (uwUnitIndex = 0; uwUnitIndex < VT->gubD1UnitNum; uwUnitIndex++) {
			ulUnitEraseCnt = gulEC_D1[uwUnitIndex].B.ulEraseCount;
			ulSumEC += gulEC_D1[uwUnitIndex].B.ulEraseCount;

			if (ulUnitEraseCnt > EraseCountInfo->TypeD1.ulMaxEraseCnt) {
				EraseCountInfo->TypeD1.ulMaxEraseCnt = ulUnitEraseCnt;
				EraseCountInfo->TypeD1.uwMaxEraseCnt_Index = uwUnitIndex;
			}

			if (ulUnitEraseCnt < EraseCountInfo->TypeD1.ulMinEraseCnt) {
				EraseCountInfo->TypeD1.ulMinEraseCnt = ulUnitEraseCnt;
				EraseCountInfo->TypeD1.uwMinEraseCnt_Index = uwUnitIndex;
			}
		}
		EraseCountInfo->TypeD1.ulTotalEraseCnt = ulSumEC;
		if (VT->gubD1UnitNum) {
			EraseCountInfo->TypeD1.ulAvgEraseCnt = ulSumEC / VT->gubD1UnitNum;
		}
		EraseCountInfo->TypeD1.ulWorstAvgEraseCnt = EraseCountInfo->TypeD1.ulAvgEraseCnt; //due to erase in unit, every block's erase count in that unit is the same, I don't know what to return..., so return avg erase cnt.
	}
	else {
		EraseCountInfo->TypeD1.ulTotalEraseCnt = 0xFFFFFFFF;
		EraseCountInfo->TypeD1.ulAvgEraseCnt = 0xFFFFFFFF;
		EraseCountInfo->TypeD1.ulWorstAvgEraseCnt = 0xFFFFFFFF;
		EraseCountInfo->TypeD1.ulMaxEraseCnt = 0xFFFFFFFF;
		EraseCountInfo->TypeD1.ulMinEraseCnt = 0xFFFFFFFF;
		EraseCountInfo->TypeD1.uwMaxEraseCnt_Index = 0xFFFF;
		EraseCountInfo->TypeD1.uwMinEraseCnt_Index = 0xFFFF;
	}

	if (ENABLE_DEBUG_UART_ATA_CMD) {
		UartString("\r\nVT totalEC: 0x");
		UartLongHex(VT->gulTotalEraseCount);
		UartString("\r\nSumEC: 0x");
		UartLongHex(ulSumEC);
	}
#endif
}

////======================================
void CalculateFailAndBadCountInfo(SMART_FAILCOUNT_INFO *FailCountInfo, SMART_BADBLOCK_INFO *BadBlockInfo)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	U32 ulEntryPerDBTPlane; // 一個plane存了幾個block state......
	U32 ulDBTEntryIndex;
	U32 ulCount;
	UBYTE *ubDBTBuffer = (UBYTE*) gubHostTable;
	UWORD *uwReadFailCntPerPlane = (UWORD *)&BUFB_BASE[0x1000];
	UWORD *uwProgramFailCntPerPlane = (UWORD *)&BUFB_BASE[0x1200];
	UWORD *uwEraseFailCntPerPlane = (UWORD *)&BUFB_BASE[0x1400];
	UWORD *uwEarlyBadCntPerPlane = (UWORD *)&BUFB_BASE[0x1600];
	UWORD *uwLaterBadCntPerPlane = (UWORD *)&BUFB_BASE[0x1800];
	UWORD *uwEarlyBadCntPerCE = (UWORD *)&BUFB_BASE[0x1A00];
	UWORD *uwLaterBadCntPerCE = (UWORD *)&BUFB_BASE[0x1A20];
	UWORD *uwRSRecoveryMarkBadCntPerPlane = (UWORD *)&BUFB_BASE[0x1C00];
	UBYTE *ubLaterBadRatePerPlane = (UBYTE *)&BUFB_BASE[0x1E00];

	U32 ulTotalBlockNum;
	U32 ulTotalBlksPerPlaneBank;

	ulEntryPerDBTPlane = (U32)(gubSectorsPerPlane * 512);
	ulTotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;
	ulTotalBlksPerPlaneBank = gubSelDieNumber * (gubBurstsPerBank) * (gubCEsPerPage);

	FailCountInfo->uwMaxReadFailCntPerPlane = 0;
	FailCountInfo->uwMaxProgramFailCntPerPlane = 0;
	FailCountInfo->uwMaxEraseFailCntPerPlane = 0;
	FailCountInfo->ulTotalReadFailCnt = 0;
	FailCountInfo->ulTotalProgramFailCnt = 0;
	FailCountInfo->ulTotalEraseFailCnt = 0;
	FailCountInfo->ulTotalRSRecoveryMarkBadCnt = 0;
	FailCountInfo->ubMaxReadFailCntPerPlane_Index = 0;
	FailCountInfo->ubMaxProgramFailCntPerPlane_Index = 0;
	FailCountInfo->ubMaxEraseFailCntPerPlane_Index = 0;

	BadBlockInfo->uwMaxEarlyBadCntPerPlane = 0;
	BadBlockInfo->uwMaxLaterBadCntPerPlane = 0;
	BadBlockInfo->uwMaxEarlyLaterBadCntPerPlane = 0;
	BadBlockInfo->uwMaxEarlyBadCntPerCE = 0;
	BadBlockInfo->uwMaxLaterBadCntPerCE = 0;
	BadBlockInfo->uwMaxEarlyLaterBadCntPerCE = 0;
	BadBlockInfo->ulTotalLaterBadCnt = 0;
	BadBlockInfo->ulTotalEarlyBadCnt = 0;
	BadBlockInfo->ulTotalEarlyLaterBadCnt = 0;
	BadBlockInfo->ubMaxEarlyBadCntPerPlane_Index = 0;
	BadBlockInfo->ubMaxLaterBadCntPerPlane_Index = 0;
	BadBlockInfo->ubMaxEarlyLaterBadCntPerPlane_Index = 0;
	BadBlockInfo->ubMaxEarlyBadCntPerCE_Index = 0;
	BadBlockInfo->ubMaxLaterBadCntPerCE_Index = 0;
	BadBlockInfo->ubMaxEarlyLaterBadCntPerCE_Index = 0;

	BadBlockInfo->ubMaxLaterBadRatePerPlane = 0;
	BadBlockInfo->ubMaxLaterBadRatePerPlane_Index = 0;

	for (ulCount = 0; ulCount < 256; ulCount++) {
		uwReadFailCntPerPlane[ulCount] = 0;
		uwProgramFailCntPerPlane[ulCount] = 0;
		uwEraseFailCntPerPlane[ulCount] = 0;
		uwEarlyBadCntPerPlane[ulCount] = 0;
		uwLaterBadCntPerPlane[ulCount] = 0;
	}

	for (ulCount = 0; ulCount < 16; ulCount++) {
		uwEarlyBadCntPerCE[ulCount] = 0;
		uwLaterBadCntPerCE[ulCount] = 0;
	}

	for (ulCount = 0; ulCount < ulTotalBlockNum; ulCount++) {
		ulDBTEntryIndex = (ulCount % ulEntryPerDBTPlane);
		if (ulDBTEntryIndex == 0) {
			ftlLoadDBT(ulCount, (U32)&ubDBTBuffer[0]); // load single plane
		}

		/// EarlyBad Cnt
		if (ubDBTBuffer[ulDBTEntryIndex] & 0xC0) {
			BadBlockInfo->ulTotalEarlyBadCnt++;
			uwEarlyBadCntPerPlane[(ulCount % ulTotalBlksPerPlaneBank)]++;
			uwEarlyBadCntPerCE[(ulCount >> gubBurstsPerBankLog) % gubCEsPerPage]++;// Record per CE (only for plane-CH-CE order)
		}

		//LaterBad Cnt
		if (ubDBTBuffer[ulDBTEntryIndex] & 0x30) {
			///Read Fail Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & DBT_MARK_READ_FAIL) {
				FailCountInfo->ulTotalReadFailCnt++;
				uwReadFailCntPerPlane[(ulCount % ulTotalBlksPerPlaneBank)]++;
			}

			///Prog Fail Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & DBT_MARK_WRITE_FAIL) {
				FailCountInfo->ulTotalProgramFailCnt++;
				uwProgramFailCntPerPlane[(ulCount % ulTotalBlksPerPlaneBank)]++;
			}

			///Erase Fail Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & DBT_MARK_ERASE_FAIL) {
				FailCountInfo->ulTotalEraseFailCnt++;
				uwEraseFailCntPerPlane[(ulCount % ulTotalBlksPerPlaneBank)]++;
			}

			///RS Recovery Mark Bad Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & DBT_MARK_RS_RECOVERY) {
				FailCountInfo->ulTotalRSRecoveryMarkBadCnt++;
				uwRSRecoveryMarkBadCntPerPlane[(ulCount % ulTotalBlksPerPlaneBank)]++;
			}
			BadBlockInfo->ulTotalLaterBadCnt++;
			uwLaterBadCntPerPlane[(ulCount % ulTotalBlksPerPlaneBank)]++;
			uwLaterBadCntPerCE[(ulCount >> gubBurstsPerBankLog) % gubCEsPerPage]++;// Record per CE (only for plane-CH-CE order)
		}

	}

	for (ulCount = 0; ulCount < ulTotalBlksPerPlaneBank; ulCount++) {
		if (uwReadFailCntPerPlane[ulCount] > FailCountInfo->uwMaxReadFailCntPerPlane) {
			FailCountInfo->uwMaxReadFailCntPerPlane = (UWORD)uwReadFailCntPerPlane[ulCount];
			FailCountInfo->ubMaxReadFailCntPerPlane_Index  = (UBYTE)(ulCount + 1);
		}

		if (uwProgramFailCntPerPlane[ulCount] > FailCountInfo->uwMaxProgramFailCntPerPlane) {
			FailCountInfo->uwMaxProgramFailCntPerPlane = (UWORD)uwProgramFailCntPerPlane[ulCount];
			FailCountInfo->ubMaxProgramFailCntPerPlane_Index  = (UBYTE)(ulCount + 1);
		}

		if (uwEraseFailCntPerPlane[ulCount] > FailCountInfo->uwMaxEraseFailCntPerPlane) {
			FailCountInfo->uwMaxEraseFailCntPerPlane = (UWORD)uwEraseFailCntPerPlane[ulCount];
			FailCountInfo->ubMaxEraseFailCntPerPlane_Index  = (UBYTE)(ulCount + 1);
		}

		if (uwEarlyBadCntPerPlane[ulCount] > BadBlockInfo->uwMaxEarlyBadCntPerPlane) {
			BadBlockInfo->uwMaxEarlyBadCntPerPlane = (UWORD)uwEarlyBadCntPerPlane[ulCount];
			BadBlockInfo->ubMaxEarlyBadCntPerPlane_Index  = (UBYTE)(ulCount + 1);
		}

		if (uwLaterBadCntPerPlane[ulCount] > BadBlockInfo->uwMaxLaterBadCntPerPlane) {
			BadBlockInfo->uwMaxLaterBadCntPerPlane = (UWORD)uwLaterBadCntPerPlane[ulCount];
			BadBlockInfo->ubMaxLaterBadCntPerPlane_Index  = (UBYTE)(ulCount + 1);
		}

		if ((uwEarlyBadCntPerPlane[ulCount] + uwLaterBadCntPerPlane[ulCount]) > BadBlockInfo->uwMaxEarlyLaterBadCntPerPlane) {
			BadBlockInfo->uwMaxEarlyLaterBadCntPerPlane = (UWORD)(uwEarlyBadCntPerPlane[ulCount] + uwLaterBadCntPerPlane[ulCount]);
			BadBlockInfo->ubMaxEarlyLaterBadCntPerPlane_Index  = (UBYTE)(ulCount + 1);
		}

		ubLaterBadRatePerPlane[ulCount] = (uwLaterBadCntPerPlane[ulCount] >= (guwBadBlockBoundary - uwEarlyBadCntPerPlane[ulCount])) ? 100 :  ((uwLaterBadCntPerPlane[ulCount] * 100) / (guwBadBlockBoundary - uwEarlyBadCntPerPlane[ulCount]));
		if (ubLaterBadRatePerPlane[ulCount] > BadBlockInfo->ubMaxLaterBadRatePerPlane) {
			BadBlockInfo->ubMaxLaterBadRatePerPlane = (UBYTE)ubLaterBadRatePerPlane[ulCount];
			BadBlockInfo->ubMaxLaterBadRatePerPlane_Index  = (UBYTE)(ulCount + 1);
		}
	}

	for (ulCount = 0; ulCount < gubCEsPerPage; ulCount++) {

		if (uwEarlyBadCntPerCE[ulCount] > BadBlockInfo->uwMaxEarlyBadCntPerCE) {
			BadBlockInfo->uwMaxEarlyBadCntPerCE = (UWORD)uwEarlyBadCntPerCE[ulCount];
			BadBlockInfo->ubMaxEarlyBadCntPerCE_Index  = (UBYTE)(ulCount);
		}

		if (uwLaterBadCntPerCE[ulCount] > BadBlockInfo->uwMaxLaterBadCntPerCE) {
			BadBlockInfo->uwMaxLaterBadCntPerCE = (UWORD)uwLaterBadCntPerCE[ulCount];
			BadBlockInfo->ubMaxLaterBadCntPerCE_Index  = (UBYTE)(ulCount);
		}

		if ((uwEarlyBadCntPerCE[ulCount] + uwLaterBadCntPerCE[ulCount]) > BadBlockInfo->uwMaxEarlyLaterBadCntPerCE) {
			BadBlockInfo->uwMaxEarlyLaterBadCntPerCE = (UWORD)(uwEarlyBadCntPerCE[ulCount] + uwLaterBadCntPerCE[ulCount]);
			BadBlockInfo->ubMaxEarlyLaterBadCntPerCE_Index  = (UBYTE)(ulCount);
		}
	}

	BadBlockInfo->ulTotalEarlyLaterBadCnt = BadBlockInfo->ulTotalEarlyBadCnt + BadBlockInfo->ulTotalLaterBadCnt;
#endif
}
void SMART()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (((IDT.W82 & BIT0) == 0)
	        || (AtaCfg.uwDCOState & SET_DCO_SMART)
	        || ( (HB[HB_LBA_M] != 0x4F) || (HB[HB_LBA_H] != 0xC2) ) // smart key always should be 0xC24F in LBA bit 23~08
	        || (((IDT.W85 & CHK_BIT0) == 0) && (HB[HB_FEATURE] != 0xD8))// smart is disable and this CMD is not smart enable operations
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN) && (HB[HB_FEATURE] != 0xD5) && (HB[HB_FEATURE] != 0xDA))) {

		gubErrorCode = SET_ABRT;
		return;
	}
	if (ENABLE_DEBUG_UART_WRITEPROTECT) {
		UartString("\r\nSmart: ");
		Uart_Tx_DataHex(HB[HB_FEATURE]);
	}
	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect && ((HB[HB_FEATURE] == 0xD4) || (HB[HB_FEATURE] == 0xD6) || (HB[HB_FEATURE] == 0xD9))) {// (HB[HB_FEATURE] == 0xD8) dont abrt
			gubErrorCode = SET_ABRT;
			return;
		}
	}
	gubStopFTL = 1;
	HandleStopRW(0);

	if (ENABLE_DEBUG_UART_ATA_CMD) {
		UartString("\r\nSmart: ");
		Uart_Tx_DataHex(HB[HB_FEATURE]);
	}
	gubErrorCode = NO_ERROR;

	// Ref. ATA-8-ACS Table 60 — SMART Feature field values
	switch (HB[HB_FEATURE]) {
		case 0xD0 :             //D0 SMART READ DATA
			gubNeedSendCmdComplete = 0;
			SMART_Read_Data();
			break;
		case 0xD1 :
			gubNeedSendCmdComplete = 0;
			SMART_Read_Threshold();
			break;
		case 0xD2 :             //D2 SMART ENABLE/DISABLE Attribute autosave
			if (HB[HB_SECTOR_CNT] == 0xF1) { //smart enable attribute autosave
				//SMART_ATTRIBUTE_AS = 1; //平常就有在存了，所以只收command不做事
			}
			else if (HB[HB_SECTOR_CNT] == 0x00) { //smart disable attribute autosave
				//SMART_ATTRIBUTE_AS = 0;
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

		case 0xD3:		//D3 SMART Save attribute values
			//do nothing.
			break;

		case 0xD4 :             //D4 SMART Execute Off-Line Immediate
			if (AtaCfg.uwDCOState & SET_DCO_SELFTEST) {
				gubErrorCode = SET_ABRT;
			}
			else {
				if (gubSMARTExecuteOfflineEnable) {
					SMART_Off_Line();
				}
				else {
					gubErrorCode = SET_ABRT;
				}
			}
			break;

		case 0xD5 :// SMART READ LOG
			gubNeedSendCmdComplete = 0;
			SMART_Read_Log();
			break;

		case 0xD6 :// SMART WRITE LOG
			SMART_Write_Log();
			break;

		case 0xD8 : // Smart Enable
			IDT.W85 |= SET_BIT0;
			AtaCfg.ubSmartState = 1;
			//RMA_FLOW
			if (!((gubIsWriteProtect & BIT_TRULY_WRITEPROTECT) || (gubIsWriteProtect & BIT_ERR_UNIT_WRITEPROTECT) || (gubIsWriteProtect & BIT_FLASH_INIT_FAIL_WRITEPROTECT))) {
				SaveAtaConfig();
			}
			break;
		case 0xD9 : // Smart Disable
			IDT.W85 &= CLR_BIT0;
			AtaCfg.ubSmartState = 0;
			SMART_StopOffLineDoing();
			SaveAtaConfig();
			break;

		case 0xDA : // SMART RETURN STATUS
			if (SMART_Return_Status()) {
				HB[HB_LBA_H] = 0x2C;
				HB[HB_LBA_M] = 0xF4;
			}
			break;

		case 0xDB:			//DB SMART ENABLE/DISABLE Attribute auto offline
			if (HB[HB_SECTOR_CNT] == 0xF8) { //smart enable attribute auto offline
				//SMART_ATTRIBUTE_AS = 1; //平常就有在存了，所以只收command不做事
			}
			else if (HB[HB_SECTOR_CNT] == 0x00) { //smart disable attribute auto offline
				//SMART_ATTRIBUTE_AS = 0;
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

		default :
			gubErrorCode = SET_ABRT;
			break;
	}
	gubStopFTL = 0;
#endif
}

void SMART_RealScanLBA()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	// ubMode,   0: 做到完  不收command    1: 做新command...

	HandleStopRW(0);
	M_SetFlagSettingNone();

	UBYTE ubScanStatus = SMART_SCAN_STATUS_PROGRESS;
	UBYTE ubSpan;
	UBYTE ubSkipScan = 0;

	if (AtaCfg.gSMARTSelective.ubOffLineDoing) {
		//after power-cycle, need wait uwPending Time to set SMART_FeatureFlag_OfflineScanActive do offline scan
		if (GetRTT_s() < (AtaCfg.gSMARTSelective.uwPendingTime * 60)) {
			AtaCfg.gSMARTSelective.uwFeatureFlags &= ~(SMART_FeatureFlag_OfflineScanActive);
			M_SetFlagSettingSHF();
			return;
		}
		else {
			AtaCfg.gSMARTSelective.uwFeatureFlags |= (SMART_FeatureFlag_OfflineScanActive);
		}
	}

	M_DebugGPIO8_ON_LED();
	gubFTL_Only_HandleBQ = 1;
	while ((gubOfflineMode || AtaCfg.gSMARTSelective.ubOffLineDoing)
	        && (!( HB[HB_REMAIN_VLD_NCQ_NUM] || gubHostCmdIn || gubHardReset || gubSoftReset))) {
		//只要沒有command就會進來繼續做。但遇到ubmode==0時做到一半，此時若收到command，也會繼續做。
		//但若遇到ubmode==1時做到一半，若收到command，則不會繼續做。

		if (gubDEVSLPEnable && (HW[HW_SATA_INFO]&H_DEVSLP)) {
			ubScanStatus = SMART_SCAN_STATUS_DEVSLP;
			break;
		}

		if (gubOfflineMode & SMART_M_SELECTIVESELFTEST) {
			if (gulSMARTSelfTestCurrentLBA > AtaCfg.gSMARTSelective.ulEndLBA[AtaCfg.gSMARTSelective.uwCurrentSpan]) {
				do {
					AtaCfg.gSMARTSelective.uwCurrentSpan++;
				}
				while ((AtaCfg.gSMARTSelective.uwCurrentSpan < 5) && (AtaCfg.gSMARTSelective.ulStartLBA[AtaCfg.gSMARTSelective.uwCurrentSpan] == 0) && (AtaCfg.gSMARTSelective.ulEndLBA[AtaCfg.gSMARTSelective.uwCurrentSpan] == 0));

				if ( AtaCfg.gSMARTSelective.uwCurrentSpan < 5) {
					gulSMARTSelfTestCurrentLBA = AtaCfg.gSMARTSelective.ulStartLBA[AtaCfg.gSMARTSelective.uwCurrentSpan];
				}
				else {
					ubScanStatus = SMART_SCAN_STATUS_DONE;
					break;
				}
			}
		}
		else if (AtaCfg.gSMARTSelective.ubOffLineDoing ) {
			ubSkipScan = 0;

			if (gulSMARTSelfTestCurrentLBA >= gulMediaSize.All) {
				ubScanStatus = SMART_SCAN_STATUS_DONE;
				break;
			}

			for (ubSpan = 0; ubSpan < 5; ubSpan++) {
				if (AtaCfg.gSMARTSelective.ulStartLBA[ubSpan] || AtaCfg.gSMARTSelective.ulEndLBA[ubSpan]) {
					if ((gulSMARTSelfTestCurrentLBA >= AtaCfg.gSMARTSelective.ulStartLBA[ubSpan]) && (gulSMARTSelfTestCurrentLBA <= AtaCfg.gSMARTSelective.ulEndLBA[ubSpan])) {
						ubSkipScan = 1;
						break;
					}
				}
			}
			if (ubSkipScan) {
				gulSMARTSelfTestCurrentLBA++;
				continue;
			}
		}
		else if (gubSMARTExtendScanAllEnable && (gubOfflineMode & SMART_M_EXTENDEDSELFTEST)) {
			if (gulSMARTSelfTestCurrentLBA >= gulMediaSize.All) {
				ubScanStatus = SMART_SCAN_STATUS_DONE;
				break;
			}
		}
		else {
			if (gulSMARTSelfTestCurrentLBA >= gulMediaSize.All) {
				gulSMARTSelfTestCurrentLBA = 0;
			}
			gulSmartCurrentTime = (U32)(GetRTT_s() - gulSmartScanStartTime);
			if (gulSmartCurrentTime > gulSmartNeedScanTimeInterval) {
				ubScanStatus = SMART_SCAN_STATUS_DONE;
				break;
			}
		}

		if (ENABLE_DEBUG_UART_ATA_CMD) {
			UartString("\r\nScanning LBA: ");
			UartLongHex(gulSMARTSelfTestCurrentLBA);
		}

		if (SMART_SELFTEST_REALSCANMODE) {
			gulReadFail_4KMAP = 0;
			BufferQueue_t *BQ;
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
			BQ->ulVir4kIndex = gulSMARTSelfTestCurrentLBA / 8;
			BQ->ulDataMask = BIT0 << (gulSMARTSelfTestCurrentLBA % 8);
			BQ->uwBufferIndex = 0;
			BQ->ubRBQIIndex = 0xFF;
			BQ->ubDataInDRamMap = 0;
			BQ->btPreRead = 0;
			BQ->btPartialFromFlash = 0;
			BQ->ub4kNum = 1;
			BQ->ulSpareValid = 0;
			BQ->ubThisBQisBG = 1;
			M_AddToBufferQueue(gBQI.ubLinkActive);
			while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
				M_SwitchTask();
			}

			//--------------- UNC------------------//
			if (gulReadFail_4KMAP) { //UNC
				gulSmartOfflineUNCCount++;
				gulReadFail_4KMAP = 0;
				ubScanStatus = SMART_SCAN_STATUS_FAIL;
				break;
			}
			//----------------------------------------//
		}
		else {
			idle(0x100);
		}

		gulSMARTSelfTestCurrentLBA += 1;
	}
	gubFTL_Only_HandleBQ = 0;
	M_DebugGPIO8_OFF_LED();

	if ((ubScanStatus == SMART_SCAN_STATUS_DONE) || (ubScanStatus == SMART_SCAN_STATUS_FAIL) || (ubScanStatus == SMART_SCAN_STATUS_DEVSLP)) {
		// done and no unc.
		HandleStopRW(0);

		if (gubOfflineMode & SMART_M_OFFLINEROUTINE) {
			if (ubScanStatus == SMART_SCAN_STATUS_DONE) {
				AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_COMPLETED;
			}
			else if (ubScanStatus == SMART_SCAN_STATUS_FAIL) {
				gulSMARTSelfTestCurrentLBA++;
				AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_ABORTEDBYUNC;
			}
			else { //devslp
				AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_ABORTED;
			}
		}
		else if (AtaCfg.gSMARTSelective.ubOffLineDoing) {
			// 1. stop  off line
			// 2. pending=0, active=0
			// 3. status = completed
			// 4. short lba set to CurrentLBA,  下次用short self-test就可以立刻掃到...
			AtaCfg.gSMARTSelective.uwFeatureFlags &= ~(SMART_FeatureFlag_OfflineScanPending | SMART_FeatureFlag_OfflineScanActive);
			AtaCfg.gSMARTSelective.ubOffLineDoing = 0;

			if (ubScanStatus == SMART_SCAN_STATUS_DONE) {
				AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_COMPLETED;
			}
			else if (ubScanStatus == SMART_SCAN_STATUS_FAIL) {
				gulSMARTSelfTestCurrentLBA++;
				AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_ABORTEDBYUNC;
			}
			else { //devslp
				AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_ABORTED;
			}
		}
		else {
			UBYTE  ubPercent = 0;
			if (ubScanStatus == SMART_SCAN_STATUS_DONE) {
				AtaCfg.ubSelfTestStatus = SMART_STATUS_SELFTEST_COMPLETED;
			}
			else if (ubScanStatus == SMART_SCAN_STATUS_FAIL) {
				AtaCfg.ubSelfTestStatus = SMART_STATUS_SELFTEST_ABORTEDBYUNC | ubPercent;
			}
			else { //devslp
				AtaCfg.ubSelfTestStatus = SMART_STATUS_SELFTEST_ABORTEDBYRESET | ubPercent;
			}

			SetSmartLog_CmdDone();

			if (ubScanStatus == SMART_SCAN_STATUS_FAIL) {
				gulSMARTSelfTestCurrentLBA++;
			}

			if (gubOfflineMode & SMART_M_SELECTIVESELFTEST) {
				if ((ubScanStatus == SMART_SCAN_STATUS_DONE) && (AtaCfg.gSMARTSelective.uwFeatureFlags & SMART_FeatureFlag_PerformOfflineScan)) {
					// 1. pending=1, active=1
					// 2. off line doing
					// 3. all scan.
					AtaCfg.gSMARTSelective.uwFeatureFlags |= (SMART_FeatureFlag_OfflineScanPending | SMART_FeatureFlag_OfflineScanActive);
					AtaCfg.gSMARTSelective.ubOffLineDoing = SELECTIVE_OFFLINEDOING;
					gulSMARTSelfTestCurrentLBA = 0;
					AtaCfg.gSMARTSelective.uwCurrentSpan = 0xAA00;// set the span under test to a value greater than five
				}
			}
		}

		AtaCfg.ulSMARTSelfTestStartLBA = gulSMARTSelfTestCurrentLBA;
		SaveAtaConfig();
		gubOfflineMode = 0;
	}

	mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 0);//強迫DMAC 全部做完
	M_SetFlagSettingSHF();
#endif
}


void SMART_Read_Data()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubi;
	if ((gubOfflineMode & SMART_M_TIMERENABLE) && ((gubOfflineMode & SMART_M_SUSPEND) == 0)) {
		if ((gubOfflineMode & SMART_M_SHORTSELFTEST)
		        || ((gubSMARTExtendScanAllEnable == 0) && (gubOfflineMode & SMART_M_EXTENDEDSELFTEST))
		        || (gubOfflineMode & SMART_M_CONVEYANCESELFTEST)) {

			gulSmartCurrentTime = (U32)(GetRTT_s() - gulSmartScanStartTime);
			if (gulSmartCurrentTime > gulSmartNeedScanTimeInterval) {
				AtaCfg.ubSelfTestStatus = SMART_STATUS_SELFTEST_COMPLETED;
				SetSmartLog_CmdDone();
				AtaCfg.ulSMARTSelfTestStartLBA = gulSMARTSelfTestCurrentLBA;
				SaveAtaConfig();
				gubOfflineMode = 0;
			}
		}
	}

	//spec沒規定smart attribute data structure 0~361byte要幹麻, 但通常
	//0~1byte為revision number, 2~361共有30個entry，每個entry有12byte
	//客戶會有tool來顯示smart attribute (12byte不含threshold, threshold需下smart read threshold)
	SMART_BADBLOCK_INFO BadBlockInfo;
	SMART_Attribute_Entry *AttributeEntry = (SMART_Attribute_Entry*)(&BUFB_BASE[512]);
	Set_SMART_Attribute_Entry(AttributeEntry, &BadBlockInfo);

	//memset(BUFB_BASE, 0x00, 512);
	mDMAC_SV(4, (U32)BUFB_BASE, 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	BUFW_BASE[0] = 0x0010;
	for ( ubi = 0; ubi < AttributeTotalEntry; ubi++) {//有些人會說超過30個entry之後怎麼沒了…
		memcpy(&BUFB_BASE[(ubi * 12) + 2], &AttributeEntry[ubi], 12);
	}


	//when Offline is done, gubOfflineMode would be cleared to zero.
	//======================================================================
	if ((gubOfflineMode & SMART_M_OFFLINEROUTINE)  || AtaCfg.gSMARTSelective.ubOffLineDoing) { // off-line routine
		if ((gubOfflineMode & SMART_M_SUSPEND) || (AtaCfg.gSMARTSelective.ubOffLineDoing & SELECTIVE_OFFLINESUSPEND)) {
			BUFB_BASE[362] = SMART_STATUS_OFFLINE_SUSPEND;
		}
		else {
			BUFB_BASE[362] = SMART_STATUS_OFFLINE_PROGRESS;
		}
	}
	else {
		BUFB_BASE[362] = AtaCfg.ubOfflineStatus;
	}

	UBYTE  ubPercent = 0;
	if (gubOfflineMode & SMART_M_SHORTSELFTEST) { // Short self-test
		// bits[3:0] : Percent Self-Test Remaining
		// bits[7:4] : Self-test Execution Status
		//因為當完成時，gubOfflineMode=0，因此能進到這裡的都是未完成的

		if (gulSmartNeedScanTimeInterval) {
			ubPercent = (8 * (gulSmartNeedScanTimeInterval - gulSmartCurrentTime) / gulSmartNeedScanTimeInterval) + 1;
		}
		else {
			ubPercent = 1;
		}
		BUFB_BASE[363] = SMART_STATUS_SELFTEST_PROGRESS | ubPercent;

	}
	else if (gubOfflineMode & SMART_M_EXTENDEDSELFTEST) { // Extend self-test ，全掃
		if (gubSMARTExtendScanAllEnable) {
			if (gulMediaSize.All) {
				ubPercent = (8 * (gulMediaSize.All - gulSMARTSelfTestCurrentLBA) / gulMediaSize.All) + 1;
			}
			else {
				ubPercent = 1;
			}
		}
		else {
			if (gulSmartNeedScanTimeInterval) {
				ubPercent = (8 * (gulSmartNeedScanTimeInterval - gulSmartCurrentTime) / gulSmartNeedScanTimeInterval) + 1;
			}
			else {
				ubPercent = 1;
			}
		}

		BUFB_BASE[363] = SMART_STATUS_SELFTEST_PROGRESS | ubPercent; //finish it in 20 sec
	}
	else if (gubOfflineMode & SMART_M_CONVEYANCESELFTEST) { // Conveyance self test
		if (gulSmartNeedScanTimeInterval) {
			ubPercent = (8 * (gulSmartNeedScanTimeInterval - gulSmartCurrentTime) / gulSmartNeedScanTimeInterval) + 1;
		}
		else {
			ubPercent = 1;
		}

		BUFB_BASE[363] = SMART_STATUS_SELFTEST_PROGRESS | ubPercent; //finish it in 20 sec
	}
	else if (gubOfflineMode & SMART_M_SELECTIVESELFTEST) { //Selective self test
		U32 ulScannedLBANum = 0;
		U32 ulTotalLBANum = 0;

		for (ubi = 0; ubi <= AtaCfg.gSMARTSelective.uwCurrentSpan; ubi++) {
			if (ubi == AtaCfg.gSMARTSelective.uwCurrentSpan) {
				ulScannedLBANum += (gulSMARTSelfTestCurrentLBA - AtaCfg.gSMARTSelective.ulStartLBA[ubi]);
			}
			else {
				ulScannedLBANum += (AtaCfg.gSMARTSelective.ulEndLBA[ubi] - AtaCfg.gSMARTSelective.ulStartLBA[ubi]);
			}
		}

		for (ubi = 0; ubi < 5; ubi++) {
			ulTotalLBANum += (AtaCfg.gSMARTSelective.ulEndLBA[ubi] - AtaCfg.gSMARTSelective.ulStartLBA[ubi]);
		}

		ubPercent = (8 * (ulTotalLBANum - ulScannedLBANum) / ulTotalLBANum) + 1;
		BUFB_BASE[363] = SMART_STATUS_SELFTEST_PROGRESS | ubPercent; //finish it in 20 sec
	}
	else {
		BUFB_BASE[363] = AtaCfg.ubSelfTestStatus;
	}

	BUFW_BASE[364 >> 1] = guwSMARTOfflineTimeSecond;// collection time in second


	if (gubSMARTExecuteOfflineEnable == 0) {//if DCO not support to SMART self-test
		BUFB_BASE[367] = 0;  //disable Off-line data collection capability
	}
	else {

		if (gubSMARTExecuteOfflineEnable)
		{
			BUFB_BASE[367] |= (SET_BIT0); //SMART capability,enable Off-line data collection capability.BIT6 means that selective self test has implemented   BIT5 means that CONVEYANCE SELF-TEST IMPLEMENTEDHC-20131114
		}

		if ((AtaCfg.uwDCOState & SET_DCO_SELFTEST) == 0) {
			if (ENABLE_SMART_OFFLINE_READ) {
				BUFB_BASE[367] |= BIT3;
			}

			if (ENABLE_SMART_SELFTEST_SHORT_EXTEND_SELFTEST) {
				BUFB_BASE[367] |= BIT4;
			}
		}

		if ( ENABLE_SMART_SELFTEST_CONVEYANCESELFTEST) {
			BUFB_BASE[367] |= BIT5;
		}

		if ( ENABLE_SMART_SELFTEST_SELECTIVESELFTEST) {
			BUFB_BASE[367] |= BIT6;
		}
	}


	BUFW_BASE[368 >> 1] |= (BIT0 | BIT1); //support SMART enable/Enable attribute autosave

	if ((AtaCfg.uwDCOState & SET_DCO_LOG) == 0) {
		BUFB_BASE[370] = 0x01;  // support error log command
	}

	// for sn=01h,81h
	BUFB_BASE[372] = gubSMARTShortSelfTestTimeMin;
	// for sn=02h,82h
	if ((guwSMARTExtendedSelfTestTimeMin) <= 0xFF) {
		BUFB_BASE[373] = guwSMARTExtendedSelfTestTimeMin;
	}
	else {
		BUFB_BASE[373] = 0xFF;
	}
	// for sn=03h,83h
	if ( ENABLE_SMART_SELFTEST_CONVEYANCESELFTEST) {
		BUFB_BASE[374] = gubSMARTConveyanceSelfTestTimeMin;
	}
	BUFB_BASE[375] = (UBYTE)(guwSMARTExtendedSelfTestTimeMin);
	BUFB_BASE[376] = (UBYTE)((guwSMARTExtendedSelfTestTimeMin) >> 8);

	CalCheckSum();

	RWBuffer(1);
#endif
}


void SMART_Read_Threshold()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubi;

	// Ref ATA 3 Table 17 and Table18
	SMART_Attribute_Entry *AttributeEntry = (SMART_Attribute_Entry*)(&BUFB_BASE[512]);
	SMART_BADBLOCK_INFO BadBlockInfo;
	Set_SMART_Attribute_Entry(AttributeEntry, &BadBlockInfo);

	mDMAC_SV(4, (U32)BUFB_BASE, 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	BUFW_BASE[0] = 0x0010;
	for (ubi = 0; ubi < AttributeTotalEntry; ubi++) {
		BUFB_BASE[(ubi * 12) + 2 + 0] = AttributeEntry[ubi].ubID;
		BUFB_BASE[(ubi * 12) + 2 + 1] = AttributeEntry[ubi].ubThreshold;
	}
	CalCheckSum();

	RWBuffer(1);
#endif
}

UBYTE SMART_Return_Status()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);
	UBYTE ubi;
	SMART_Attribute_Entry *AttributeEntry = (SMART_Attribute_Entry*)(&BUFB_BASE[512]);
	SMART_BADBLOCK_INFO BadBlockInfo;
	Set_SMART_Attribute_Entry(AttributeEntry, &BadBlockInfo);

	UBYTE ubTemperature;
	UBYTE ubNegativeFlag = 0;//負數flag
	ubTemperature = I2C_Temperature(0);

	//不符合spec規定溫度在這裡先檢查
	if (gubIndustrial_Specification == 1) { //工規
		if (ubTemperature >= 128) {
			ubNegativeFlag = 1;
		}
		if ((ubTemperature > gubTemperatureMax) && (ubTemperature < gubTemperatureMin)) //包含>85度和<-40度
			return 1;
	}
	else {
		if (ubTemperature > gubTemperatureMax) //包含>70度和零下溫度
			return 1;
	}
	for (ubi = 0; ubi < AttributeTotalEntry; ubi++) {
		if (AttributeEntry[ubi].ubThreshold == 0) {
			// always pass
		}
		else if (AttributeEntry[ubi].ubThreshold == 0xFF) {
			// always fail
			return 1;
		}
		else {
			if (gulSmartStatusRuleMap & (BIT0 << ubi)) {
				if ( AttributeEntry[ubi].ubValue > AttributeEntry[ubi].ubThreshold ) {
					return 1;
				}
			}
			else {
				if ( AttributeEntry[ubi].ubValue < AttributeEntry[ubi].ubThreshold ) {
					if (ubNegativeFlag == 1 ) {
						continue;
					}
					return 1;
				}
			}
		}
	}

	return 0;
#endif
#if(RDT)
	return 0;
#endif
}



void SMART_SuspendOffLineDoing()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (gubOfflineMode & SMART_M_TIMERENABLE) {
		gubOfflineMode |= SMART_M_SUSPEND;
		gulSmartCurrentTime = (U32)(GetRTT_s() - gulSmartScanStartTime);
	}
	else if (AtaCfg.gSMARTSelective.ubOffLineDoing) {
		AtaCfg.gSMARTSelective.ubOffLineDoing |= SELECTIVE_OFFLINESUSPEND;
	}
}
void SMART_StopOffLineDoing()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (gubOfflineMode & SMART_M_TIMERENABLE) {
		if (gubOfflineMode & SMART_M_OFFLINEROUTINE) { // off-line routine
			AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_ABORTED; //Abort by Sleep
		}
		else if (gubOfflineMode & (SMART_M_SHORTSELFTEST | SMART_M_EXTENDEDSELFTEST | SMART_M_CONVEYANCESELFTEST | SMART_M_SELECTIVESELFTEST)) { // self-test
			AtaCfg.ubSelfTestStatus = SMART_STATUS_SELFTEST_ABORTEDBYHOST; //Abort by Sleep
			SetSmartLog_CmdDone();
		}
	}
	else if (AtaCfg.gSMARTSelective.ubOffLineDoing) {
		AtaCfg.gSMARTSelective.uwFeatureFlags &= ~(SMART_FeatureFlag_OfflineScanPending | SMART_FeatureFlag_OfflineScanActive);
		AtaCfg.gSMARTSelective.ubOffLineDoing = 0;
		AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_ABORTED;
	}

	AtaCfg.ulSMARTSelfTestStartLBA = gulSMARTSelfTestCurrentLBA;

	gubOfflineMode = 0;
}

void SMART_Off_Line()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ub_SubCmd;

	ub_SubCmd = HB[HB_LBA_L];

	// ATA8 - Table 61 — SMART EXECUTE OFF-LINE IMMEDIATE Subcommands
	if ( ((ub_SubCmd == 0x00) && ENABLE_SMART_OFFLINE_READ)
	        || ((ub_SubCmd == 0x01 || ub_SubCmd == 0x81) && ENABLE_SMART_SELFTEST_SHORT_EXTEND_SELFTEST)
	        || ((ub_SubCmd == 0x02 || ub_SubCmd == 0x82) && ENABLE_SMART_SELFTEST_SHORT_EXTEND_SELFTEST)
	        || ((ub_SubCmd == 0x03 || ub_SubCmd == 0x83) && ENABLE_SMART_SELFTEST_CONVEYANCESELFTEST)
	        || ((ub_SubCmd == 0x04 || ub_SubCmd == 0x84) && ENABLE_SMART_SELFTEST_SELECTIVESELFTEST)) {

		SMART_StopOffLineDoing();

		if (ub_SubCmd != 0x00) { //offline routine do not save log
			SetSmartLog_CmdRecv(ub_SubCmd);
		}

		gubOfflineMode = SMART_M_TIMERENABLE | (BIT0 << (ub_SubCmd & 0x0F));
		gulSmartScanStartTime = GetRTT_s();
		gulSmartOfflineUNCCount = 0;
		gulSmartNeedScanTimeInterval = 0;
		gulSmartCurrentTime = 0;

		if (ub_SubCmd == 0x00) {
			if (guwSMARTOfflineTimeSecond > gubSMARTBufferTimeSecond) {
				gulSmartNeedScanTimeInterval = guwSMARTOfflineTimeSecond - gubSMARTBufferTimeSecond;
			}
		}
		else if ((ub_SubCmd & 0x0F)  == 0x01) {//Short self-test routine in captive mode
			if (gubSMARTShortSelfTestTimeMin * 60 > gubSMARTBufferTimeSecond) {
				gulSmartNeedScanTimeInterval = gubSMARTShortSelfTestTimeMin * 60 - gubSMARTBufferTimeSecond;
			}
		}
		else if ((ub_SubCmd & 0x0F)  == 0x02) {//Extended self-test routine in captive mode
			if (gubSMARTExtendScanAllEnable) {
				gulSMARTSelfTestCurrentLBA = 0;
				AtaCfg.ulSMARTSelfTestStartLBA = 0;
			}
			else {
				if (guwSMARTExtendedSelfTestTimeMin * 60 > gubSMARTBufferTimeSecond) {
					gulSmartNeedScanTimeInterval = guwSMARTExtendedSelfTestTimeMin * 60 - gubSMARTBufferTimeSecond;
				}
			}
		}
		else if ((ub_SubCmd & 0x0F)  == 0x03) { //Conveyance
			if (gubSMARTConveyanceSelfTestTimeMin * 60 > gubSMARTBufferTimeSecond) {
				gulSmartNeedScanTimeInterval = gubSMARTConveyanceSelfTestTimeMin * 60 - gubSMARTBufferTimeSecond;
			}
		}
		else if ((ub_SubCmd & 0x0F)  == 0x04) {//selective self-test routine in captive mode
			// ................
			if ( AtaCfg.ubSmart09hPage == 0) {
				gubOfflineMode = 0;
				gubErrorCode = SET_ABRT;
			}
			else {
				AtaCfg.gSMARTSelective.ubOffLineDoing = 0;
				AtaCfg.gSMARTSelective.uwCurrentSpan = 0;

				gulSMARTSelfTestCurrentLBA = AtaCfg.gSMARTSelective.ulStartLBA[AtaCfg.gSMARTSelective.uwCurrentSpan];
				while ((AtaCfg.gSMARTSelective.uwCurrentSpan < 5) && (AtaCfg.gSMARTSelective.ulStartLBA[AtaCfg.gSMARTSelective.uwCurrentSpan] == 0) && (AtaCfg.gSMARTSelective.ulEndLBA[AtaCfg.gSMARTSelective.uwCurrentSpan] == 0)) {
					AtaCfg.gSMARTSelective.uwCurrentSpan++;
					gulSMARTSelfTestCurrentLBA = AtaCfg.gSMARTSelective.ulStartLBA[AtaCfg.gSMARTSelective.uwCurrentSpan];
				}

				if (AtaCfg.gSMARTSelective.uwCurrentSpan >= 5) { // if all span's start and end lba = 0, then start offline scaning
					AtaCfg.ubSelfTestStatus = SMART_STATUS_SELFTEST_COMPLETED;
					SetSmartLog_CmdDone();
					if ( AtaCfg.gSMARTSelective.uwFeatureFlags & SMART_FeatureFlag_PerformOfflineScan) {
						// 1. pending=1, active=1
						// 2. off line doing
						// 3. all scan.
						AtaCfg.gSMARTSelective.uwFeatureFlags |= (SMART_FeatureFlag_OfflineScanPending | SMART_FeatureFlag_OfflineScanActive);
						AtaCfg.gSMARTSelective.ubOffLineDoing = SELECTIVE_OFFLINEDOING;
						gulSMARTSelfTestCurrentLBA = 0;
						AtaCfg.ulSMARTSelfTestStartLBA = 0;
						AtaCfg.gSMARTSelective.uwCurrentSpan = 0xAA00;// set the span under test to a value greater than five
						gubOfflineMode = 0;
						SaveAtaConfig();
					}
					else {
						AtaCfg.gSMARTSelective.uwFeatureFlags &= ~(SMART_FeatureFlag_OfflineScanPending | SMART_FeatureFlag_OfflineScanActive);
						AtaCfg.gSMARTSelective.ubOffLineDoing = 0;
						AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_COMPLETED; //complete subcmd 0x00
						gubOfflineMode = 0;
						SaveAtaConfig();
					}
				}
			}
		}

		if (ub_SubCmd & BIT7) { //captive mode
			SMART_RealScanLBA();
		}
		else { //offline mode
			HB[HB_LBA_H] = 0x00;//return this value in FIS means that subcommand specified an off-line routine including an off-line self-test routine, and captive mode keeps C24F
			HB[HB_LBA_M] = 0x01;
		}

		SaveAtaConfig();
	}
	else if (ub_SubCmd == 0x7f) { // Abort off-line mode self-test routine
		SMART_StopOffLineDoing();
		SaveAtaConfig();
	}
	else if ( (ub_SubCmd == 0xF0) && ENABLE_SMART_SELFMERGE) {
		SMART_Self_Merge();
	}
	else {
		gubErrorCode = SET_ABRT;

	}

#endif
}

void SMART_Self_Merge()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ( ENABLE_SMART_SELFMERGE) {
		if (ENABLE_SMART_SELFMERGE_DUMMY) {
			HB[HB_LBA_L] = 0;
			return;
		}
	}
#endif
}

void SMART_Read_Log()
{
#if(!BURNER)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ub_Index;
	UWORD uw_LogAdr;

	//memset(BUFB_BASE, 0x00, 512);
	mDMAC_SV(4, (U32)BUFB_BASE, 512, 0, 0, 0, DMA_EN_SETCQ, 1);

	// Ref. ATA-8-ACS Table A.2 — Log address definitio
	switch (HB[HB_LBA_L]) {
		case 0x00 :   //Log directory, maximum 1 page, read only
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if (HB[HB_SECTOR_CNT] != HOSTTABLE_SMARTLOG0_PAGESIZE) {
				gubErrorCode = SET_ABRT;
			}
			else {
				BUFW_BASE[0x00] = 1; //log version

				if ((AtaCfg.uwDCOState & SET_DCO_LOG) == 0)
				{
					BUFW_BASE[0x01] = HOSTTABLE_SMARTLOG1_PAGESIZE; //log address 1
					BUFW_BASE[0x02] = HOSTTABLE_SMARTLOG2_PAGESIZE; //log address 2
				}

				BUFW_BASE[0x04] = HOSTTABLE_SMARTLOG4_PAGESIZE; //log address 4
				BUFW_BASE[0x06] = HOSTTABLE_SMARTLOG6_PAGESIZE; //log address 6

				if ( ENABLE_SMART_SELFTEST_SELECTIVESELFTEST) {
					BUFW_BASE[0x09] = HOSTTABLE_SMARTLOG9_PAGESIZE; //log address 9
				}

				BUFW_BASE[0x30] = HOSTTABLE_SMARTLOG30_PAGESIZE; //log address 30h

				for (ub_Index = 0x80; ub_Index < 0xA0; ub_Index++) {
					// Page maximum is 16
					BUFW_BASE[ub_Index] = HOSTTABLE_SMARTLOG80to9F_PAGESIZE;
				}

				if (ENABLE_DCACHE) {
					xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
				}

				RWBuffer(1);
			}

			break;

		case 0x01 :// Summary SMART Error Log, maximum 1 page, read only
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))
			{
				gubErrorCode = SET_ABRT;
			}
			else if (HB[HB_SECTOR_CNT] != HOSTTABLE_SMARTLOG1_PAGESIZE) {
				gubErrorCode = SET_ABRT;
			}
			else if (AtaCfg.uwDCOState & SET_DCO_LOG) {
				gubErrorCode = SET_ABRT;
			}
			else {
				SummaryErrLog();
				RWBuffer(1);
			}

			break;

		case 0x02 :
			// Comprehensive SMART Error Log, read only, default 0x33
			// maximum 16,383 (0x3FFF) page, but fewer page is ok
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))
			{
				gubErrorCode = SET_ABRT;
			}
			else if ((HB[HB_SECTOR_CNT] == 0) || (HB[HB_SECTOR_CNT] > HOSTTABLE_SMARTLOG2_PAGESIZE)) {
				gubErrorCode = SET_ABRT;
			}
			else if (AtaCfg.uwDCOState & SET_DCO_LOG) {
				gubErrorCode = SET_ABRT;
			}
			else {
				GetLogSector(HOSTTABLE_SMARTLOG2_ADR);
			}
			break;
		case 0x04:
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ((HB[HB_SECTOR_CNT] == 0) || (HB[HB_SECTOR_CNT] > HOSTTABLE_SMARTLOG4_PAGESIZE)) {
				gubErrorCode = SET_ABRT;
			}
			else {
				DeviceStatisticsLog(0);
			}
			break;

		case 0x06 :// SMART Self-Test Log, maximum 1 page, read only
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if (HB[HB_SECTOR_CNT] == HOSTTABLE_SMARTLOG6_PAGESIZE) {
				GetLogSector(HOSTTABLE_SMARTLOG6_ADR);
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

		case 0x09 : //Selective Self-Test log
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ( (HB[HB_SECTOR_CNT] == HOSTTABLE_SMARTLOG9_PAGESIZE) && ENABLE_SMART_SELFTEST_SELECTIVESELFTEST) {
				GetLogSector(HOSTTABLE_SMARTLOG9_ADR);
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

		case 0x30 : // Identify Device Data Log
			if ((HB[HB_SECTOR_CNT] == 0) || (HB[HB_SECTOR_CNT] > HOSTTABLE_SMARTLOG30_PAGESIZE)) {
				gubErrorCode = SET_ABRT;
			}
			else {
				IdentifyDeviceDataLog(0);
			}

			break;
		case 0xDF : // TSB Read Serial Customer ID
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if (HB[HB_SECTOR_CNT] == 0) {
				gubErrorCode = SET_ABRT;
			}
			else {
				memcpy(&BUFB_BASE[0], &TSBSerialTable[0], sizeof(TSBSerialTable));

				if (ENABLE_DCACHE) {
					xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
				}

				RWBuffer(1);
			}
			break;
		default :
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ((HB[HB_LBA_L] < 0x80) || (HB[HB_LBA_L] > 0x9F)) {
				gubErrorCode = SET_ABRT;
			}
			else if ((HB[HB_SECTOR_CNT] == 0) || (HB[HB_SECTOR_CNT] > HOSTTABLE_SMARTLOG80to9F_PAGESIZE)) {
				gubErrorCode = SET_ABRT;
			}
			else {
				// Ref. ATA-8-ACS A.9 Host Vendor Specific Logs
				// maximum 16 pages, read or written
				// Write by SMART WRITE LOG, Read by READ LOG EXT

				uw_LogAdr = HOSTTABLE_SMARTLOG80_ADR + ((HB[HB_LBA_L] - 0x80) << 4);
				ReadHostLog(uw_LogAdr);
			}

			break;

	}

#endif
}

void SMART_Write_Log()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UWORD uwi ;
	UWORD uw_LogAdr;

	HB[HB_CMD_INFO_LOWBYTE] |= SET_BIT2; //set write direction
	switch (HB[HB_LBA_L]) {
		case 0x09 :
			if (ENABLE_SMART_SELFTEST_SELECTIVESELFTEST) {
				if (gubOfflineMode & SMART_M_SELECTIVESELFTEST) {//The host should not write the Selective self-test log while the processing of a SMART Selective self-test routine is in progress
					gubErrorCode = SET_ABRT;
				}
				else if (HB[HB_SECTOR_CNT] == HOSTTABLE_SMARTLOG9_PAGESIZE) {

					UBYTE ubFail = WriteLogCheckSum(HOSTTABLE_SMARTLOG9_ADR);

					if (ubFail == 0) {
						for (uwi = 0; uwi < 5; uwi++) {
							AtaCfg.gSMARTSelective.ulStartLBA[uwi] =  (BUFB_BASE[uwi * 16 + 5] << 24) | (BUFB_BASE[uwi * 16 + 4] << 16) | (BUFB_BASE[uwi * 16 + 3] << 8) | BUFB_BASE[uwi * 16 + 2];
							AtaCfg.gSMARTSelective.ulEndLBA[uwi] =  (BUFB_BASE[uwi * 16 + 13] << 24) | (BUFB_BASE[uwi * 16 + 12] << 16) | (BUFB_BASE[uwi * 16 + 11] << 8) | BUFB_BASE[uwi * 16 + 10];

							if ( (AtaCfg.gSMARTSelective.ulStartLBA[uwi] > AtaCfg.gSMARTSelective.ulEndLBA[uwi])
							        || (AtaCfg.gSMARTSelective.ulStartLBA[uwi] >= gulMediaSize.All)
							        || (AtaCfg.gSMARTSelective.ulEndLBA[uwi] >= gulMediaSize.All)) {
								ubFail = 1;
								break;
							}
						}
						AtaCfg.gSMARTSelective.uwFeatureFlags = (BUFB_BASE[503] << 8) | BUFB_BASE[502];
						AtaCfg.gSMARTSelective.uwPendingTime  = (BUFB_BASE[509] << 8) | BUFB_BASE[508];
						AtaCfg.gSMARTSelective.uwCurrentSpan = 0;
					}


					AtaCfg.gSMARTSelective.ubOffLineDoing = 0;

					if (ubFail) {
						AtaCfg.ubSmart09hPage = 0;
						gubErrorCode = SET_IDNF;
					}
					else {
						if (AtaCfg.ubSmart09hPage == 0) {
							AtaCfg.ubSmart09hPage = 1;
						}
					}


					SaveAtaConfig();

				}
				else {
					gubErrorCode = SET_ABRT;
				}
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;
		default :
			// most logs are RO.

			if ((HB[HB_LBA_L] < 0x80) || (HB[HB_LBA_L] > 0x9F)) {
				// not support
				gubErrorCode = SET_ABRT;
			}
			else if ((HB[HB_SECTOR_CNT] == 0) || (HB[HB_SECTOR_CNT] > HOSTTABLE_SMARTLOG80to9F_PAGESIZE)) {
				// zero is illegal and Page maximum is 16
				gubErrorCode = SET_ABRT;
			}
			else {
				// Ref. ATA-8-ACS A.9 Host Vendor Specific Logs
				uw_LogAdr = HOSTTABLE_SMARTLOG80_ADR + ((HB[HB_LBA_L] - 0x80) << 4);
				WriteHostLog(uw_LogAdr);
			}
			break;
	}
#endif
}

void Read_LOG_EXT(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((gubLBA48BitCmdSupport == 0)
	        || (((IDT.W119 & SET_BIT3) == 0) && (HB[HB_COMMAND] == 0x47))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	HandleStopRW(0);

	UBYTE ub_Index;
	UWORD uw_LogAdr;
	UWORD uwPage = HB[HB_LBA_M] | (HB[HB_LBA_M_EXP] << 8);
	UWORD uwSecCnt = HB[HB_SECTOR_CNT] | (HB[HB_SECTOR_CNT_EXP] << 8);

	//memset(BUFB_BASE, 0x00, 512);
	mDMAC_SV(4, (U32)BUFB_BASE, 512, 0, 0, 0, DMA_EN_SETCQ, 0);

	if (HB[HB_COMMAND] == 0x47) {
		HB[HB_CMD_INFO_LOWBYTE] |= H_PIO_DMA_CMD; //dma command is received
	}
	else {
		gubNeedSendCmdComplete = 0;
	}

	switch (HB[HB_LBA_L]) {
		case 0x00:   // General purpose Log Directory
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ((uwSecCnt != HOSTTABLE_SMARTLOG0_PAGESIZE) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG0_PAGESIZE) ) {
				gubErrorCode = SET_ABRT;
			}
			else {
				// Ref. ATA-8-ACS Table A.3 — General Purpose Log Directory
				BUFW_BASE[0x00] = 1; //log version

				if ((AtaCfg.uwDCOState & SET_DCO_LOG) == 0) {
					BUFW_BASE[0x03] = HOSTTABLE_SMARTLOG3_PAGESIZE; //log address 3
				}

				BUFW_BASE[0x04] = HOSTTABLE_SMARTLOG4_PAGESIZE; //log address 4
				BUFW_BASE[0x07] = HOSTTABLE_SMARTLOG7_PAGESIZE; //log address 7
				BUFW_BASE[0x11] = HOSTTABLE_SMARTLOG11_PAGESIZE; //log address 11h
				if (gubNCQSupport) {
					BUFW_BASE[0x10] = HOSTTABLE_SMARTLOG10_PAGESIZE; //log address 10h
				}
				BUFW_BASE[0x30] = HOSTTABLE_SMARTLOG30_PAGESIZE; //log address 30h

				if (gubFormFactor == FORM_FACTOR_CFAST) {
					BUFW_BASE[0x31] = HOSTTABLE_SMARTLOG31_PAGESIZE; //log address 31
				}

				for (ub_Index = 0x80; ub_Index < 0xA0; ub_Index++) {
					// Page maximum is 16
					BUFW_BASE[ub_Index] = HOSTTABLE_SMARTLOG80to9F_PAGESIZE;
				}

				if (ENABLE_DCACHE) {
					xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
				}
				RWBuffer(1);
			}
			break;

		case 0x03 : // Extended Comprehensive SMART Error Log, read only, default 0x40
			// maximum 16,383 (0x3FFF) page, but fewer page is ok
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ((IDT.W85 & CHK_BIT0) == 0) {
				// smart is disable, the feature set associated with a requested log is disabled, device shall return command aborted
				gubErrorCode = SET_ABRT;
			}
			else if ( (uwSecCnt == 0) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG3_PAGESIZE)) {
				// can't exceed 64 page or ?? or zero is illegal
				gubErrorCode = SET_ABRT;
			}
			else if (AtaCfg.uwDCOState & SET_DCO_LOG) {
				gubErrorCode = SET_ABRT;
			}
			else {
				uw_LogAdr = HOSTTABLE_SMARTLOG3_ADR + uwPage;
				GetLogSector(uw_LogAdr);
			}
			break;

		case 0x04:
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ( (uwSecCnt == 0) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG4_PAGESIZE)) {
				gubErrorCode = SET_ABRT;
			}
			else {
				DeviceStatisticsLog(uwPage);
			}
			break;

		case 0x07 : // Extended SMART Self-Test Log
			//if (((HB_LBA_Mid + HB_SECCNT) > SMART_LOG7_PAGESIZE) || (HB_LBA_MidEXP) || (HB_SECCNT == 0)) { //20110512
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ((IDT.W85 & CHK_BIT0) == 0) {
				// smart is disable, the feature set associated with a requested log is disabled, device shall return command aborted
				gubErrorCode = SET_ABRT;
			}
			else if ( (uwSecCnt == 0) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG7_PAGESIZE)) {
				// can't exceed 1 page or ?? or zero is illegal
				gubErrorCode = SET_ABRT;
			}
			else {
				uw_LogAdr = HOSTTABLE_SMARTLOG7_ADR + uwPage;
				GetLogSector(uw_LogAdr);
			}

			break;
			// Log Page 10h NCQ Command Error
			// Ref. ATA-8-ACS Table A.14 — NCQ Command Error Log

		case 0x10:   // Log Page 10h
			if ( (uwSecCnt == 0) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG10_PAGESIZE)) {
				gubErrorCode = SET_ABRT;
			}
			else if (gubNCQSupport) {
				if (ENABLE_KT_DEBUG_UART)
					UartString("\r\n RLog");
				if (gubSoftReset | gubHardReset) {
					break;
				}

				if (gubNCQINTRError) {
					gubNCQINTRError = 0;

					if (HW[HW_ERR_CTRL] &H_BLK_TRG) {
						HW[HW_ERR_CTRL] &= (~H_BLK_TRG);
					}

					//因為待會要回SDB, 怕中間又有NCQ近來, 結果回到他的, 之所以不在前面回, 是怕停太久, host會有不能用NCQ之類的誤判, 另外,這個SDB會舉所有的Tag
					HW[HW_DATA_TRG] |= H_HOLD_NCQ_DIS;
					HL[HL_SACTIVE] = 0xFFFFFFFF; // set all ncq tag active
					HB[HB_STATUS] = (0x40);    //used status 0x40 for NCQ
					HB[HB_SDB_REQ] = H_SDB_REQ;
					while (HB[HB_SDB_REQ] & H_SDB_REQ);
					HW[HW_DATA_TRG] &= (~H_HOLD_NCQ_DIS); // disable hold

					if (ENABLE_D2HSDBByHW) {
						if (((AtaCfg.ubSecMode & SET_SEC_LOCK) == 0) && (gubPowerMode != PWR_SLEEP) && (gubIsWriteProtect == FALSE) && ((AtaCfg.ubSanitizeState == SD0_SANITIZE_IDLE) || (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN)) && (gubNCQSupport)) {
							M_EnableHWD2H_NCQ();
						}
					}
				}

				memcpy(BUFB_BASE, NCQ_READ_LOG_EXT_ERR, 16);
				if (ENABLE_DEBUG_UART_WRITEPROTECT) {
					UartString("\n\rReadLog ErrTag:");
					Uart_Tx_DataHex(gubErrorTag);
					UartString("  Code:");
					Uart_Tx_DataHex(NCQ_READ_LOG_EXT_ERR[3]);
					UartString("  Byte2:");
					Uart_Tx_DataHex(NCQ_READ_LOG_EXT_ERR[2]);
				}
				CalCheckSum();
				RWBuffer(1);
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

		case 0x011 : // Extended SMART Self-Test Log
			//if (((HB_LBA_Mid + HB_SECCNT) > SMART_LOG7_PAGESIZE) || (HB_LBA_MidEXP) || (HB_SECCNT == 0)) { //20110512
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ( (uwSecCnt == 0) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG11_PAGESIZE)) {
				// can't exceed 1 page or ?? or zero is illegal
				gubErrorCode = SET_ABRT;
			}
			else {
				PhyEventCountersLog();
				RWBuffer(1);
			}

			break;

		case 0x30 : // Identify Device Data Log
			if ( (uwSecCnt == 0) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG30_PAGESIZE)) {
				// can't exceed 1 page or ?? or zero is illegal
				gubErrorCode = SET_ABRT;
			}
			else {
				uw_LogAdr = uwPage;
				IdentifyDeviceDataLog(uw_LogAdr);
			}
			break;

		case 0x31:
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ( (uwSecCnt == 0) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG31_PAGESIZE)) {
				gubErrorCode = SET_ABRT;
			}
			else {
				if (gubFormFactor == FORM_FACTOR_CFAST) {
					CFALog(uwPage);
				}
				else {
					gubErrorCode = SET_ABRT;
				}
			}
			break;

		default :
			if ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN)) {
				gubErrorCode = SET_ABRT;
			}
			else if ((HB[HB_LBA_L] < 0x80) || (HB[HB_LBA_L] > 0x9F)) {
				gubErrorCode = SET_ABRT;
			}
			else if ( (uwSecCnt == 0) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG80to9F_PAGESIZE)) {
				// Page maximum is 16
				gubErrorCode = SET_ABRT;
			}
			else {
				if ((IDT.W85 & CHK_BIT0) == 0) {
					// If the feature set associated with a requested log is disabled, the device shall return command aborted.
					gubErrorCode = SET_ABRT;
				}
				else {
					uw_LogAdr = HOSTTABLE_SMARTLOG80_ADR + ((HB[HB_LBA_L] - 0x80) << 4) + uwPage;
					ReadHostLog(uw_LogAdr);
				}
			}
			break;

	}
#endif
}

void Write_LOG_EXT(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || ( ((IDT.W119 & SET_BIT3) == 0) && (HB[HB_COMMAND] == 0x57))
	        || (gubLBA48BitCmdSupport == 0)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

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

	UWORD uw_LogAdr;
	UWORD uwPage = HB[HB_LBA_M] | (HB[HB_LBA_M_EXP] << 8);
	UWORD uwSecCnt = HB[HB_SECTOR_CNT] | (HB[HB_SECTOR_CNT_EXP] << 8);

	if (HB[HB_COMMAND] == 0x57) {
		HB[HB_CMD_INFO_LOWBYTE] |= (H_PIO_DMA_CMD | H_WR_CMD); //dma command is received
	}

	if ((HB[HB_LBA_L] < 0x80) || (HB[HB_LBA_L] > 0x9F)) {
		gubErrorCode = SET_ABRT;
	}
	else if ((uwSecCnt == 0) || ((uwPage + uwSecCnt) > HOSTTABLE_SMARTLOG80to9F_PAGESIZE)) { //20110512
		gubErrorCode = SET_ABRT;
	}
	else {
		if ((IDT.W85 & CHK_BIT0) == 0) {
			// smart is disable, the feature set associated with a requested log is disabled, device shall return command aborted
			gubErrorCode = SET_ABRT;
		}
		else {
			uw_LogAdr = HOSTTABLE_SMARTLOG80_ADR + ((HB[HB_LBA_L] - 0x80) << 4) + HB[HB_LBA_M];
			WriteHostLog(uw_LogAdr);
		}
	}
#endif
}

void SetSmartLog_CmdDone()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	U32 ulRightNowTimeInMinutes = (U32)(GetRTT_m());
	UWORD uwPowerOnHours = (UWORD)((VT->gulPowerOnTimeInMinutes + (U32)(ulRightNowTimeInMinutes - gulLastSavedTimeInMinutes)) / UPDATE_TIME_INTERVAL_MIN); //ATA spec use word to log, but this may be overflow when power-on 7.48 years.

	//  LOG 6
	if (AtaCfg.ubSmart06hIndex == 21) {
		AtaCfg.ubSmart06hIndex = 0;
	}
	Load_LogSector(HOSTTABLE_SMARTLOG6_ADR);
	BUFB_BASE[(AtaCfg.ubSmart06hIndex * 24) + 2 + 1] = AtaCfg.ubSelfTestStatus;
	BUFB_BASE[(AtaCfg.ubSmart06hIndex * 24) + 2 + 2] = uwPowerOnHours;
	BUFB_BASE[(AtaCfg.ubSmart06hIndex * 24) + 2 + 3] = uwPowerOnHours >> 8;

	//  If the test passed or the test failed for some reason other than an
	//  uncorrectable logical sector, the value of this field is undefined.
	BUFB_BASE[(AtaCfg.ubSmart06hIndex * 24) + 2 + 5] = gulSMARTSelfTestCurrentLBA;
	BUFB_BASE[(AtaCfg.ubSmart06hIndex * 24) + 2 + 6] = gulSMARTSelfTestCurrentLBA >> 8;
	BUFB_BASE[(AtaCfg.ubSmart06hIndex * 24) + 2 + 7] = gulSMARTSelfTestCurrentLBA >> 16;
	BUFB_BASE[(AtaCfg.ubSmart06hIndex * 24) + 2 + 8] = gulSMARTSelfTestCurrentLBA >> 24;

	Save_LogSector(HOSTTABLE_SMARTLOG6_ADR);

	AtaCfg.ubSmart06hIndex++;


	//  LOG 7
	if (AtaCfg.ubSmart07hIndex == 19) {
		AtaCfg.ubSmart07hIndex = 0;
	}
	Load_LogSector(HOSTTABLE_SMARTLOG7_ADR);
	BUFB_BASE[(AtaCfg.ubSmart07hIndex * 26) + 4 + 1] = AtaCfg.ubSelfTestStatus;
	BUFB_BASE[(AtaCfg.ubSmart07hIndex * 26) + 4 + 2] = uwPowerOnHours;
	BUFB_BASE[(AtaCfg.ubSmart07hIndex * 26) + 4 + 3] = uwPowerOnHours >> 8;

	//  If the test passed or the test failed for some reason other than an
	//  uncorrectable logical sector, the value of this field is undefined.
	BUFB_BASE[(AtaCfg.ubSmart07hIndex * 26) + 4 + 5] = gulSMARTSelfTestCurrentLBA;
	BUFB_BASE[(AtaCfg.ubSmart07hIndex * 26) + 4 + 6] = gulSMARTSelfTestCurrentLBA >> 8;
	BUFB_BASE[(AtaCfg.ubSmart07hIndex * 26) + 4 + 7] = gulSMARTSelfTestCurrentLBA >> 16;
	BUFB_BASE[(AtaCfg.ubSmart07hIndex * 26) + 4 + 8] = gulSMARTSelfTestCurrentLBA >> 24;
	BUFB_BASE[(AtaCfg.ubSmart07hIndex * 26) + 4 + 9] = 0;
	BUFB_BASE[(AtaCfg.ubSmart07hIndex * 26) + 4 + 10] = 0;

	Save_LogSector(HOSTTABLE_SMARTLOG7_ADR);

	AtaCfg.ubSmart07hIndex++;

#endif
}

void SetSmartLog_CmdRecv(UBYTE ub_SubCmd)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubLog6Index, ubLog7Index ;

	if (AtaCfg.ubSmart06hIndex == 21) {
		ubLog6Index = 0;
	}
	else {
		ubLog6Index = AtaCfg.ubSmart06hIndex;
	}
	Load_LogSector(HOSTTABLE_SMARTLOG6_ADR);
	BUFB_BASE[(ubLog6Index * 24) + 2 + 0] = ub_SubCmd;
	Save_LogSector(HOSTTABLE_SMARTLOG6_ADR);

	if (AtaCfg.ubSmart06hPage == 0) {//有記過log，就舉1，不會再變0
		AtaCfg.ubSmart06hPage = 1;
	}


	if (AtaCfg.ubSmart07hIndex == 19) {
		ubLog7Index = 0;
	}
	else {
		ubLog7Index = AtaCfg.ubSmart07hIndex;
	}
	Load_LogSector(HOSTTABLE_SMARTLOG7_ADR);
	BUFB_BASE[(ubLog7Index * 26) + 4 + 0] = ub_SubCmd;
	Save_LogSector(HOSTTABLE_SMARTLOG7_ADR);
	if (AtaCfg.ubSmart07hPage == 0) {
		AtaCfg.ubSmart07hPage = 1;
	}
#endif
}

void ReadHostLog(UWORD uw_LogAddr)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN;//because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	gubNeedChkDoneTag = 1;

	UWORD uwi ;

	for ( uwi = 0; uwi < gulSectorCnt; uwi++) {
		Load_LogSector(uw_LogAddr + uwi);
		RWBuffer_forSMARTLog(1, 0);
	}

	M_CheckDoneTag();
	gulSATA_MSG_TYPE3[0].All = 0;
	while (HL[HL_AXI_FIS_STAT]&BIT23);
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
#endif
}

void WriteHostLog(UWORD uw_LogAddr)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];
	UBYTE ubOffset = 0;

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN;//because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	gubNeedChkDoneTag = 1;

	UWORD uwi ;

	for ( uwi = 0; uwi < gulSectorCnt; uwi++) {
		ubOffset = (UBYTE)((HL[HL_CMD_S_LBA] + uwi) & 0x07);
		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate((void *)&BUFB_BASE[ubOffset * 512], 512);
		}
		RWBuffer_forSMARTLog(1, (U32)ubOffset);
		if (ubOffset) {
			mSE_COPY((U32)(&BUFB_BASE[ubOffset * 512]), (U32)BUFB_BASE, 512);
		}
		Save_LogSector(uw_LogAddr + uwi);
	}

	M_CheckDoneTag();
	gulSATA_MSG_TYPE3[0].All = 0;

	while (HL[HL_AXI_FIS_STAT]&BIT23);
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
#endif
}

void Load_LogSector(UWORD uw_LogAddr)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	gSwapHostTable.B.ubJob = BYTE_HOST_Load_Mode;
	gSwapHostTable.B.uwHostTableIndex = uw_LogAddr;
	HandleStopRW(1);
	while (gSwapHostTable.B.ubJob) {
		M_SwitchTask();
	}
	gubStopFTL = 0;
}

void Save_LogSector(UWORD uw_LogAddr)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	gSwapHostTable.B.ubJob = BYTE_HOST_Save_Mode;
	gSwapHostTable.B.uwHostTableIndex = uw_LogAddr;
	HandleStopRW(1);
	while (gSwapHostTable.B.ubJob) {
		M_SwitchTask();
	}
	gubStopFTL = 0;
}

void GetLogSector(unsigned short uw_LogAddr)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	U32 uli;
	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN;//because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	gubNeedChkDoneTag = 1;

	for (uli = 0; uli < gulSectorCnt; uli++) {
		Load_LogSector(uw_LogAddr + uli);
		if (uw_LogAddr == HOSTTABLE_SMARTLOG2_ADR) { //First Log Page
			BUFB_BASE[0] = 0x01;
			if (AtaCfg.ubSmart02hPage) {
				BUFB_BASE[1] = AtaCfg.ubSmart02hIndex;
			}
			// Ref. ATA-8-ACS Table A.5 — Comprehensive error log
			BUFW_BASE[452 >> 1] = AtaCfg.uwSmart02hErrCnt;
		}
		else if (uw_LogAddr == HOSTTABLE_SMARTLOG3_ADR) { //First Log Page
			BUFB_BASE[0] = 0x01;
			if (AtaCfg.ubSmart03hPage) {
				BUFW_BASE[1] = AtaCfg.uwSmart03hIndex ;
			}
			// Ref. ATA-8-ACS Table A.7 — Extended Comprehensive SMART error log
			BUFW_BASE[500 >> 1] = AtaCfg.uwSmart03hErrCnt;
		}
		else if (uw_LogAddr == HOSTTABLE_SMARTLOG6_ADR) { //First Log Page
			BUFB_BASE[0] = 0x01;
			if (AtaCfg.ubSmart06hPage) {
				// Ref. ATA-8-ACS Table A.20 — Self-test log data structure
				BUFB_BASE[508] = AtaCfg.ubSmart06hIndex ;
			}
		}
		else if (uw_LogAddr == HOSTTABLE_SMARTLOG7_ADR) { //First Log Page
			BUFB_BASE[0] = 0x01;
			if (AtaCfg.ubSmart07hPage) {
				// Ref. ATA-8-ACS Table A.12 — Extended Self-test log data structure
				BUFW_BASE[1] = AtaCfg.ubSmart07hIndex ;
			}
		}
		else if (uw_LogAddr == HOSTTABLE_SMARTLOG9_ADR) { //First Log Page;
			if (AtaCfg.ubSmart09hPage) {
				// Current span under test
				// off-line doing = 6
				// end = 0
				if (AtaCfg.gSMARTSelective.ubOffLineDoing || gubOfflineMode & SMART_M_SELECTIVESELFTEST) {
					BUFL_BASE[492 >> 2] = gulSMARTSelfTestCurrentLBA;
					BUFL_BASE[496 >> 2] = 0;
					BUFW_BASE[500 >> 1] = AtaCfg.gSMARTSelective.uwCurrentSpan + 1; //1,2,3,4,5
				}
				else {
					BUFL_BASE[492 >> 2] = 0;
					BUFL_BASE[496 >> 2] = 0;
					BUFW_BASE[500 >> 1] = 0; //1,2,3,4,5
				}
				BUFW_BASE[502 >> 1] = AtaCfg.gSMARTSelective.uwFeatureFlags;
			}
		}
		CalCheckSum();
		RWBuffer_forSMARTLog(1, 0);
	}

	M_CheckDoneTag();
	gulSATA_MSG_TYPE3[0].All = 0;
	while (HL[HL_AXI_FIS_STAT]&BIT23);
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
#endif
}

void PhyEventCountersLog()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	UWORD uwCounter;


	// byte [5:4] , Command failed and ICRC error bit set to one in Error register
	// bit[15]=0, [14:12]=1 , [11:0]=0x001
	uwCounter = (HL[HL_PHY_EVT_CNT_1] >> 0) & 0xFF;
	BUFW_BASE[4 >> 1] = BIT12 | (0x001);
	BUFW_BASE[6 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);

	// byte [9:8] , R_ERRP response for Device-to-Host Data FIS
	// bit[15]=0, [14:12]=1 , [11:0]=0x003
	uwCounter = (HL[HL_PHY_EVT_CNT_1] >> 8) & 0xFF;
	BUFW_BASE[8 >> 1] = BIT12 | (0x003);
	BUFW_BASE[10 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);


	// byte [13:12] , R_ERRP response for Host-to-Device Data FIS
	// bit[15]=0, [14:12]=1 , [11:0]=0x004
	uwCounter = (HL[HL_PHY_EVT_CNT_1] >> 16) & 0xFF;
	BUFW_BASE[12 >> 1] = BIT12 | (0x004);
	BUFW_BASE[14 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);


	// byte [17:16] , R_ERRP response for Device-to-Host non-Data FIS
	// bit[15]=0, [14:12]=1 , [11:0]=0x006
	uwCounter = (HL[HL_PHY_EVT_CNT_1] >> 24) & 0xFF;
	BUFW_BASE[16 >> 1] = BIT12 | (0x006);
	BUFW_BASE[18 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);

	// byte [21:20] , R_ERRP response for Host-to-Device non-Data FIS
	// bit[15]=0, [14:12]=1 , [11:0]=0x007
	uwCounter = (HL[HL_PHY_EVT_CNT_2] >> 0) & 0xFF;
	BUFW_BASE[20 >> 1] = BIT12 | (0x007);
	BUFW_BASE[22 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);

	// byte [25:24] , Device-to-Host non-Data FIS retries
	// bit[15]=0, [14:12]=1 , [11:0]=0x008
	uwCounter = (HL[HL_PHY_EVT_CNT_2] >> 8) & 0xFF;
	BUFW_BASE[24 >> 1] = BIT12 | (0x008);
	BUFW_BASE[26 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);


	// byte [29:28] , Transitions from drive PHYRDY to drive PHYRDYn
	// bit[15]=0, [14:12]=2 , [11:0]=0x009
	uwCounter = (HL[HL_PHY_EVT_CNT_2] >> 16) & 0xFF;
	BUFW_BASE[28 >> 1] = (2 << 12) | (0x009);
	BUFW_BASE[30 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);
	BUFW_BASE[32 >> 1] = 0;

	// byte [35:34] ,Signature Device-to-Host Register FISes sent due to a COMRESET
	// bit[15]=0, [14:12]=2 , [11:0]=0x00A
	uwCounter = (HL[HL_PHY_EVT_CNT_2] >> 24) & 0xFF;
	BUFW_BASE[34 >> 1] = (2 << 12) | (0x00A);
	BUFW_BASE[36 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);
	BUFW_BASE[38 >> 1] = 0;


	// byte [41:40] , R_ERRP response for Host-to-Device Data FIS due to CRC errors
	// bit[15]=0, [14:12]=1 , [11:0]=0x00F
	uwCounter = (HL[HL_PHY_EVT_CNT_3] >> 0) & 0xFF;
	BUFW_BASE[40 >> 1] = BIT12 | (0x00F);
	BUFW_BASE[42 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);

	// byte [45:44] , R_ERRP response for Host-to-Device Data FIS due to non-CRC errors
	// bit[15]=0, [14:12]=1 , [11:0]=0x010
	uwCounter = (HL[HL_PHY_EVT_CNT_3] >> 8) & 0xFF;
	BUFW_BASE[44 >> 1] = BIT12 | (0x010);
	BUFW_BASE[46 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);

	// byte [49:48] , R_ERRP response for Host-to-Device non-Data FIS due to CRC errors
	// bit[15]=0, [14:12]=1 , [11:0]=0x012
	uwCounter = (HL[HL_PHY_EVT_CNT_3] >> 16) & 0xFF;
	BUFW_BASE[48 >> 1] = BIT12 | (0x012);
	BUFW_BASE[50 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);

	// byte [53:52] , R_ERRP response for Host-to-Device non-Data FIS due to non-CRC errors
	// bit[15]=0, [14:12]=1 , [11:0]=0x013
	uwCounter = (HL[HL_PHY_EVT_CNT_3] >> 24) & 0xFF;
	BUFW_BASE[52 >> 1] = BIT12 | (0x013);
	BUFW_BASE[54 >> 1] = (uwCounter == 0xFF ? 0xFFFF : uwCounter);

	CalCheckSum();


	if ( HB[HB_FEATURE] & BIT0) {
		// If the device receives a BIST Activate FIS, then the device shall reset all SATA Phy event counters to their reset
		// value (see SATA 2.6).
		// If the SATA Phy Event Counters log is read and bit 0 in the Feature field is set to one, then the device shall return
		// the current counter values for the command and then reset all Phy event counter values.
		HL[HL_PHY_EVT_CNT_1] = 0;
		HL[HL_PHY_EVT_CNT_2] = 0;
		HL[HL_PHY_EVT_CNT_3] = 0;
	}

}

void CFALog(UWORD uwLogPage)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	U32 uli;
	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN; //because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	gubNeedChkDoneTag = 1;

	for (uli = 0; uli < gulSectorCnt; uli++) {
		mSE_SV(8, (U32)BUFB_BASE, 512, 0, 0);

		switch (uwLogPage + uli) {
			case 0:
				CFALog_Page0_LogPage();
				break;
			case 1:
				CFALog_Page1_PerformanceControl();
				break;
			case 2:
				CFALog_Page2_PowerDescription();
				break;
			case 3:
				CFALog_Page3_PowerProfile();
				break;
			default:
				break;
		}

		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
		}
		RWBuffer_forSMARTLog(1, 0);
	}

	M_CheckDoneTag();
	gulSATA_MSG_TYPE3[0].All = 0;
	while (HL[HL_AXI_FIS_STAT]&BIT23);
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
#endif
}

void CFALog_Page0_LogPage()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	BUFW_BASE[0] = 0x0010;//CFA log version
	BUFW_BASE[1] = 0x0001;//Number of Performance Control description records
	BUFW_BASE[2] = 0x0002;//Number of Power Description
	BUFW_BASE[3] = 0x0001;//Number of Power Profile

#endif
}

void CFALog_Page1_PerformanceControl()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	//customer said
	BUFW_BASE[0] = 0x0001;//type, 0=invalid, 1=write performance record, 2=read performance record
	BUFW_BASE[1] = 0x0003;//PR
	BUFW_BASE[2] = 0x0020;//AU
	BUFW_BASE[3] = 0x0001;//OFS
#endif

}
void CFALog_Page2_PowerDescription()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	BUFB_BASE[0] = 0x11;  //Supported ACTPM Level
	BUFB_BASE[8] = 0x28;  //Supported ACTPM Level

#endif

}

void CFALog_Page3_PowerProfile()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (gubACTPMEnable)	{
		BUFB_BASE[0] = gubACTPMValue;
	}
	else	{
		BUFB_BASE[0] = 0x00;
	}
	BUFW_BASE[1] = 0x0;  //IDT 160
	BUFL_BASE[1] = 0x0;	 //Active Power  
	BUFL_BASE[3] = 0x0;  //Slumber
	BUFL_BASE[4] = 0x0;  //DevSLP
#endif
}

void DeviceStatisticsLog(UWORD uwLogPage)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	U32 uli;
	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN; //because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	gubNeedChkDoneTag = 1;

	for (uli = 0; uli < gulSectorCnt; uli++) {
		mSE_SV(8, (U32)BUFB_BASE, 512, 0, 0);

		switch (uwLogPage + uli) {
			case 0:
				DeviceStatisticsLog_Page0_LogPage();
				break;
			case 1:
				DeviceStatisticsLog_Page1_General();
				break;
			case 4:
				DeviceStatisticsLog_Page4_GeneralErrors();
				break;
			case 5:
				DeviceStatisticsLog_Page5_Temperature();
				break;
			case 6:
				DeviceStatisticsLog_Page6_Transport();
				break;
			case 7:
				DeviceStatisticsLog_Page7_SSD();
				break;
			default:
				break;
		}

		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
		}
		RWBuffer_forSMARTLog(1, 0);
	}

	M_CheckDoneTag();
	gulSATA_MSG_TYPE3[0].All = 0;
	while (HL[HL_AXI_FIS_STAT]&BIT23);
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
#endif
}

void DeviceStatisticsLog_Page0_LogPage()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	//byte0~7
	//63:24 Reserved
	//23:16 LOG PAGE NUMBER field, shall be set to 00h
	//15:0 REVISION NUMBER field (Word), shall be set to 0001h
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x00;

	//after byte8
	BUFB_BASE[8] = 0x06; // Number of entries (n) in the following list.
	BUFB_BASE[9] = 0x00;
	BUFB_BASE[10] = 0x01;
	BUFB_BASE[11] = 0x04;
	BUFB_BASE[12] = 0x05;
	BUFB_BASE[13] = 0x06;
	BUFB_BASE[14] = 0x07;
#endif
}

void DeviceStatisticsLog_Page1_General()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	U32 ulRightNowTimeInMinutes = (U32)(GetRTT_m());
	//Device Statistics Information Header
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x01;//log page number field, shall be set to 01h

	//Lifetime Power-On Resets
	BUFL_BASE[2] = VT->gulPowerOnCnt;
	BUFB_BASE[15] = 0xC0;	//device statistic flag

	//Power-on Hours
	BUFL_BASE[4] = (VT->gulPowerOnTimeInMinutes + (U32)(ulRightNowTimeInMinutes - gulLastSavedTimeInMinutes)) / UPDATE_TIME_INTERVAL_MIN;
	BUFB_BASE[23] = 0xC0;	//device statistic flag

	//Logical Sectors Written
	BUFL_BASE[6] = (U32)(VT->guoTotalHostWriteSectorCnt);
	BUFW_BASE[14] = (UWORD)(VT->guoTotalHostWriteSectorCnt >> 32);
	BUFB_BASE[31] = 0xC0;	//device statistic flag

	//Number of Write Commands

	//Logical Sectors Read
	BUFL_BASE[10] = (U32)(VT->guoTotalHostReadSectorCnt);
	BUFW_BASE[22] = (UWORD)(VT->guoTotalHostReadSectorCnt >> 32);
	BUFB_BASE[47] = 0xC0;	//device statistic flag

	//Number of Read Commands

	//Date and Time TimeStamp

	//Pending Error Count

	//Workload Utilization

	//Utilization Usage Rate

#endif

}

void DeviceStatisticsLog_Page4_GeneralErrors()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	//byte 0~7
	//63:24 Reserved
	//23:16 LOG PAGE NUMBER field, shall be set to 04h
	//15:0 REVISION NUMBER field (Word), shall be set to 0001h
	//Device Statistics Information Header
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x04;//log page number field, shall be set to 01h

	//byte8~15 Number of Reported Uncorrectable Errors
	//63:56 DEVICE STATISTICS FLAGS field (see table 204)
	//55:32 Reserved
	//31:0 Number of Reported Uncorrectable Errors (DWord)
	BUFL_BASE[2] = VT->gulHostReadECCCnt;
	BUFB_BASE[15] = 0xC0;	//device statistic flag

	//byte16~23 Number of Resets Between Command Acceptance and Command Completion
	//63:56 DEVICE STATISTICS FLAGS field (see table 204)
	//55:32 Reserved
	//31:0 Number of Resets Between Acceptance and Command Completion

	if (ENABLE_RESET_CNT) {
		BUFL_BASE[4] = (U32)gubResetCountDuringCmd;
		BUFB_BASE[23] = 0xC0;	//device statistic flag
	}

#endif
}

void DeviceStatisticsLog_Page5_Temperature()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubTemperature;
	UBYTE ubSaveVT;
	ubTemperature = I2C_Temperature(0);
	ubSaveVT = 1;
	if ( (ubTemperature > AtaCfg.ubTemperatureMax) && (ubTemperature < 0x80) && (AtaCfg.ubTemperatureMax < 0x80)) {
		AtaCfg.ubTemperatureMax = ubTemperature;
	}
	else if ( (ubTemperature > AtaCfg.ubTemperatureMax) && (ubTemperature >= 0x80) && (AtaCfg.ubTemperatureMax >= 0x80)) {
		AtaCfg.ubTemperatureMax = ubTemperature;
	}
	else if ((ubTemperature < 0x80) && (AtaCfg.ubTemperatureMax >= 0x80)) {
		AtaCfg.ubTemperatureMax = ubTemperature;
	}
	else if ((ubTemperature < AtaCfg.ubTemperatureMin) && (ubTemperature < 0x80) && (AtaCfg.ubTemperatureMin < 0x80)) {
		AtaCfg.ubTemperatureMin = ubTemperature;
	}
	else if ( (ubTemperature < AtaCfg.ubTemperatureMin) && (ubTemperature >= 0x80) && (AtaCfg.ubTemperatureMin >= 0x80)) {
		AtaCfg.ubTemperatureMin = ubTemperature;
	}
	else if ((ubTemperature >= 0x80) && (AtaCfg.ubTemperatureMin < 0x80)) {
		AtaCfg.ubTemperatureMin = ubTemperature;
	}
	else {
		ubSaveVT = 0;
	}

	if (ubSaveVT) {
		if (ENABLE_WRITEPROTECT) {
			if ((gubIsWriteProtect & BIT_TRULY_WRITEPROTECT) == FALSE) {
				SaveAtaConfig();
			}
		}
		else {
			SaveAtaConfig();
		}
	}

	mSE_SV(8, (U32)BUFB_BASE, 512, 0, 0);
	//byte 0~7
	//63:24 Reserved
	//23:16 LOG PAGE NUMBER field, shall be set to 05h
	//15:0 REVISION NUMBER field (Word), shall be set to 0001h
	//Device Statistics Information Header
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x05;//log page number field, shall be set to 01h

	//byte 8~15
	//63:56 DEVICE STATISTICS FLAGS field (see table 204)
	//55:8 Reserved
	//7:0 Current Temperature (signed byte)
	BUFB_BASE[8] = ubTemperature;
	BUFB_BASE[15] = 0xC0;	//device statistic flag

	//byte32~39
	//63:56 DEVICE STATISTICS FLAGS field (see table 204)
	//55:8 Reserved
	//7:0 Highest Temperature (signed byte)
	BUFB_BASE[32] = AtaCfg.ubTemperatureMax;
	BUFB_BASE[39] = 0xC0;	//device statistic flag

	//byte40~47
	//63:56 DEVICE STATISTICS FLAGS field (see table 204)
	//55:8 Reserved
	//7:0 Lowest Temperature (signed byte)
	BUFB_BASE[40] = AtaCfg.ubTemperatureMin;
	BUFB_BASE[47] = 0xC0;	//device statistic flag

#endif
}

void DeviceStatisticsLog_Page6_Transport()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	//byte0~7
	//63:24 Reserved
	//23:16 LOG PAGE NUMBER field, shall be set to 06h
	//15:0 REVISION NUMBER field (Word), shall be set to 0001h
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x06;//log page number field, shall be set to 01h

	//byte8~15
	//63:56 DEVICE STATISTICS FLAGS field (see table 204)
	//55:32 Reserved
	//31:0 Number of hardware resets (DWord)
	BUFL_BASE[2] = (U32)VT->gulCOMResetCnt;
	BUFB_BASE[15] = 0xC0;	//device statistic flag

	//byte24~31
	//63:56 DEVICE STATISTICS FLAGS field (see table 204)
	//55:32 Reserved
	//31:0 Number of Interface CRC Errors (DWord)
	BUFL_BASE[6] = (U32)(VT->gulWriteCRCCnt + VT->gulReadCRCCnt);
	BUFB_BASE[31] = 0xC0;	//device statistic flag

#endif
}

void DeviceStatisticsLog_Page7_SSD()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	SMART_ERASECOUNT_INFO EraseCountInfo;
	CalculateEraseCountInfo(&EraseCountInfo);
	U32 ulAllottedEraseCnt_D3 = gulMaxPECycle_D3;
	UBYTE ubTemp;

	if ( EraseCountInfo.TypeD3.ulAvgEraseCnt > ulAllottedEraseCnt_D3) {
		ubTemp = 0;
	}
	else {
		ubTemp = ((EraseCountInfo.TypeD3.ulAvgEraseCnt * 100) / ulAllottedEraseCnt_D3);
	}

	//byte0~7
	//63:24 Reserved
	//23:16 LOG PAGE NUMBER field, shall be set to 07h
	//15:0 REVISION NUMBER field (Word), shall be set to 0001h
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x07;//log page number field, shall be set to 07h

	//byte8~15
	BUFB_BASE[8] = ubTemp;
	BUFB_BASE[15] = 0xC0;
#endif
}
void SummaryErrLog(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	UBYTE ub_02Index, ub_LogCnt, ubi;

	// save last five error
	if (AtaCfg.ubSmart02hPage) { //error exist
		// .... 一點都不直覺的東西....
		//   ub_02Index : 倒數第五筆的index
		//   ub_LogCnt : 表示有幾筆

		if (AtaCfg.ubSmart02hIndex < 5) {
			if (AtaCfg.ubSmart02hPage == 1) {
				//  小於5筆  而且只有一個page
				//  所以就是 0~ubSmart02hIndex ....
				ub_02Index = 0;
				ub_LogCnt = AtaCfg.ubSmart02hIndex;
			}
			else {
				// 小於5筆  而且page !=1
				// 表示 已經寫滿從頭開始 寫了
				//
				// ex. ubSmart02hIndex = 3 , 最後五筆為  253 254 0 1 2
				//  所以 ub_02Index = 253
				ub_02Index = (HOSTTABLE_SMARTLOG2_PAGESIZE * 5) + AtaCfg.ubSmart02hIndex - 5; //ub_02Index = 250 + VT->AtaCfg.ubSmart02hIndex;
				ub_LogCnt = 5;
			}
		}
		else {
			// normal case, log 超過5比 則取倒數五筆 ....
			ub_02Index = AtaCfg.ubSmart02hIndex - 5;
			ub_LogCnt = 5;
		}
		ubi = 0;


		if (AtaCfg.ubSmart02hIndex == 0) {
			AtaCfg.ubSmart01hIndex = AtaCfg.ubSmart02hIndex;
		}
		else {
			// only 1~5 valid
			AtaCfg.ubSmart01hIndex = ((AtaCfg.ubSmart02hIndex - 1) % 5) + 1;
		}

		//memset(BUFB_BASE, 0x00, 512); //clear buffer A
		//memset(&BUFB_BASE[512], 0x00, 512); //clear buffer B
		mDMAC_SV(4, (U32)BUFB_BASE, 512 * 2, 0, 0, 0, DMA_EN_SETCQ, 1);

		do {
			if ( ub_02Index == (HOSTTABLE_SMARTLOG2_PAGESIZE * 5)) {
				ub_02Index = 0;
			}

			if ((ubi == 0) || ((ub_02Index % 5) == 0)) {
				Load_LogSector(HOSTTABLE_SMARTLOG2_ADR + (ub_02Index / 5)); //load to buffer A
			}

			// copy valid info to buffer B, one data structure is 90 Byte
			//memcpy((unsigned char *)(BUF_START_ADDRESS+((90*uw_i)+2+512)), (unsigned char *)(BUF_START_ADDRESS+((90*(ub_02Index%5))+2)), 90);
			memcpy(&BUFB_BASE[(90 * (ub_02Index % 5)) + 2 + 512], &BUFB_BASE[(90 * (ub_02Index % 5)) + 2], 90);
			ubi++;
			ub_02Index++;
		}
		while (ubi < ub_LogCnt);

		//copy valid info to buffer A
		memcpy(BUFB_BASE, &BUFB_BASE[512], 512);
		// Device error count
		BUFW_BASE[452 >> 1] = AtaCfg.uwSmart02hErrCnt;
		BUFB_BASE[0] = 0x01;
		// ub_LogCnt only 0~5 is valid
		BUFB_BASE[1] = AtaCfg.ubSmart01hIndex;
		CalCheckSum();

	}
	else {
		BUFB_BASE[0] = 0x01;
		BUFB_BASE[511] = 0xff; //checksum
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}

}

void RecordCmdLog()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	// The Device Error Count field shall contain the total number of errors attributable to the device that have been
	// reported by the device during the life of the device (e.g., these errors shall include Uncorrectable errors, ID Not
	// Found errors for which the LBA requested was valid, servo errors, write fault errors) .  The device error count
	// shall not include errors attributed to the receipt of faulty commands (e.g., command codes not implemented by
	// the device or requests with invalid parameters or invalid LBAs) .  If the maximum value for this field is reached,
	// the count shall remain at the maximum value when additional errors are encountered and logged.


	UBYTE ubi;
	UBYTE *BUFB_BASE_1 = &BUFB_BASE[512];
	UWORD *BUFW_BASE_1 = (UWORD *)&BUFB_BASE[512];
	U32 *BUFL_BASE_1 = (U32 *) &BUFB_BASE[512];
	UBYTE ubPM = gubLastPowerMode;

	UWORD uwCmdDone_Hours = GetRTT_h(); // error發生就會進 來這個function,所以順便一進 來就去抓時間....


	//memset(BUFB_BASE_1, 0, 512);
	mDMAC_SV(4, (U32)BUFB_BASE_1, 512, 0, 0, 0, DMA_EN_SETCQ, 1);

	// update comprehensive SMART (28 bit mode) Ref. ATA-8-ACS A.4
	if ((HB[HB_CMD_INFO_LOWBYTE] & H_LBA48_CMD) == 0) {
		// Ref. ATA-8-ACS Table A.24 — Command data structure
		for (ubi = 0; ubi < 5; ubi++) {
			//  4:now
			// If the command data structure represents a command or software reset, the content of the command data
			// structure shall be as shown in table A.24.  If the command data structure represents a hardware reset, the
			// content of byte n shall be FFh, the content of bytes n+1 through n+7 are vendor specific, and the content of bytes
			// n+8 through n+11 shall contain the timestamp.
			// hw自己做掉了, HB_LOG0_CTL: comreset=0xFF , softreset=0x04
			BUFB_BASE_1[(ubi * 12) + 0] = *(volatile unsigned char *) (&HB[HB_LOG5_CTL] + ((ubi) << 4));
			BUFB_BASE_1[(ubi * 12) + 1] = *(volatile unsigned char *) (&HB[HB_LOG5_FEA] + ((ubi) << 4));
			BUFB_BASE_1[(ubi * 12) + 2] = *(volatile unsigned char *) (&HB[HB_LOG5_CNT] + ((ubi) << 4));
			BUFB_BASE_1[(ubi * 12) + 3] = *(volatile unsigned char *) (&HB[HB_LOG5_LBAL] + ((ubi) << 4));
			BUFB_BASE_1[(ubi * 12) + 4] = *(volatile unsigned char *) (&HB[HB_LOG5_LBAM] + ((ubi) << 4));
			BUFB_BASE_1[(ubi * 12) + 5] = *(volatile unsigned char *) (&HB[HB_LOG5_LBAH] + ((ubi) << 4));
			BUFB_BASE_1[(ubi * 12) + 6] = *(volatile unsigned char *) (&HB[HB_LOG5_DEV] + ((ubi) << 4));
			BUFB_BASE_1[(ubi * 12) + 7] = *(volatile unsigned char *) (&HB[HB_LOG5_CMD] + ((ubi) << 4));

			if ( (BUFB_BASE_1[(ubi * 12) + 0] == 0xFF) || ( BUFB_BASE_1[(ubi * 12) + 0] == 0x04)) {
				BUFB_BASE_1[(ubi * 12) + 1] = 0xd5;
				BUFB_BASE_1[(ubi * 12) + 2] = 0x01;
				BUFB_BASE_1[(ubi * 12) + 3] = 0x01;
				BUFB_BASE_1[(ubi * 12) + 4] = 0x00;
				BUFB_BASE_1[(ubi * 12) + 5] = 0x00;
				BUFB_BASE_1[(ubi * 12) + 6] = 0x00;
			}

			// 前4個cmd隨便填時間...
			// 因為不想要特地去存cmd近來的時間...
			BUFL_BASE_1[((ubi * 12) + 8) >> 2] = *(&HL[HL_RCV_CMD_TMR_5] + ubi); // byte[8:11]

		}
		// Ref. ATA-8-ACS Table A.25 — Error data structure
		BUFB_BASE_1[60 + 0] = 0; //Reserved
		BUFB_BASE_1[60 + 1] = HB[HB_ERROR];
		BUFB_BASE_1[60 + 2] = BUFB_BASE_1[(4 * 12) + 2] ;
		BUFB_BASE_1[60 + 3] = BUFB_BASE_1[(4 * 12) + 3] ;
		BUFB_BASE_1[60 + 4] = BUFB_BASE_1[(4 * 12) + 4] ;
		BUFB_BASE_1[60 + 5] = BUFB_BASE_1[(4 * 12) + 5] ;
		BUFB_BASE_1[60 + 6] = BUFB_BASE_1[(4 * 12) + 6] ;
		BUFB_BASE_1[60 + 7] = HB[HB_STATUS];

		// Ref. ATA-8-ACS Table A.26 — State field values
		if ( gubOfflineMode & (SMART_M_OFFLINEROUTINE | SMART_M_SHORTSELFTEST | SMART_M_EXTENDEDSELFTEST | SMART_M_CONVEYANCESELFTEST | SMART_M_SELECTIVESELFTEST)) {
			BUFB_BASE_1[60 + 27] = 0x04;
		}
		else {
			if (ubPM == PWR_SLEEP) {
				BUFB_BASE_1[60 + 27] = 0x01;
			}
			else if (ubPM == PWR_STANBY) {
				BUFB_BASE_1[60 + 27] = 0x02;
			}
			else {
				BUFB_BASE_1[60 + 27] = 0x03;
			}
		}

		//  gubTimeStampIndex = 2, 可以寫的是2,表示1是最新的(now)
		//  2 3 4 0 1
		BUFW_BASE_1[(60 + 28) >> 1] = uwCmdDone_Hours; // hours = ((ms/1000)/60)/60 ;


		// ubSmart02hIndex maximum is 51*5 = 255,
		// ubSmart02hIndex = 255,表示0~254,現在要寫255所以寫0...寫完ubSmart02hIndex=1
		if (AtaCfg.ubSmart02hIndex == (HOSTTABLE_SMARTLOG2_PAGESIZE * 5)) {
			AtaCfg.ubSmart02hIndex = 0;
		}

		Load_LogSector(HOSTTABLE_SMARTLOG2_ADR + AtaCfg.ubSmart02hIndex / 5); //load to buffer A ,  1page = 5 error log data structure.
		memcpy(&BUFB_BASE[(90 * (AtaCfg.ubSmart02hIndex % 5)) + 2], &BUFB_BASE_1[0], 90);
		Save_LogSector(HOSTTABLE_SMARTLOG2_ADR + AtaCfg.ubSmart02hIndex / 5);

		// ubSmart02hPage maximum is 51
		if (((AtaCfg.ubSmart02hIndex % 5) == 0) && (AtaCfg.ubSmart02hPage < HOSTTABLE_SMARTLOG2_PAGESIZE)) {
			AtaCfg.ubSmart02hPage++;
		}
		AtaCfg.ubSmart02hIndex++;


		// remain at the maximum value is 0xFFFF
		if (AtaCfg.uwSmart02hErrCnt != 0xFFFF) {
			AtaCfg.uwSmart02hErrCnt++;
		}

	}



	//update Ext Comprehensive SMART (28/48 bit mode)
	// Ref. ATA-8-ACS Table A.9 — Command data structure
	for (ubi = 0; ubi < 5; ubi++) {
		// If the command data structure represents a command or software reset, the content of the command data
		// structure shall be as shown in table A.24.  If the command data structure represents a hardware reset, the
		// content of byte n shall be FFh, the content of bytes n+1 through n+7 are vendor specific, and the content of bytes
		// n+8 through n+11 shall contain the timestamp.
		// hw自己做掉了, HB_LOG0_CTL: comreset=0xFF , softreset=0x04
		BUFB_BASE_1[(ubi * 18) + 0] = *(volatile unsigned char *) (&HB[HB_LOG5_CTL] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 1] = *(volatile unsigned char *) (&HB[HB_LOG5_FEA] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 2] = *(volatile unsigned char *) (&HB[HB_LOG5_FEA_EXT] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 3] = *(volatile unsigned char *) (&HB[HB_LOG5_CNT] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 4] = *(volatile unsigned char *) (&HB[HB_LOG5_CNT_EXT] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 5] = *(volatile unsigned char *) (&HB[HB_LOG5_LBAL] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 6] = *(volatile unsigned char *) (&HB[HB_LOG5_LBAL_EXT] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 7] = *(volatile unsigned char *) (&HB[HB_LOG5_LBAM] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 8] = *(volatile unsigned char *) (&HB[HB_LOG5_LBAM_EXT] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 9] = *(volatile unsigned char *) (&HB[HB_LOG5_LBAH] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 10] = *(volatile unsigned char *) (&HB[HB_LOG5_LBAH_EXT] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 11] = *(volatile unsigned char *) (&HB[HB_LOG5_DEV] + ((ubi) << 4));
		BUFB_BASE_1[(ubi * 18) + 12] = *(volatile unsigned char *) (&HB[HB_LOG5_CMD] + ((ubi) << 4));
		//BUF1_BASE[(ub_i * 18)+13] =;// reserved

		if ( (BUFB_BASE_1[(ubi * 18) + 0] == 0xFF) || ( BUFB_BASE_1[(ubi * 18) + 0] == 0x04)) {
			BUFB_BASE_1[(ubi * 18) + 1] = 0xd5;
			BUFB_BASE_1[(ubi * 18) + 2] = 0;
			BUFB_BASE_1[(ubi * 18) + 3] = 0x01;
			BUFB_BASE_1[(ubi * 18) + 4] = 0;
			BUFB_BASE_1[(ubi * 18) + 5] = 0x01;
			BUFB_BASE_1[(ubi * 18) + 6] = 0;
			BUFB_BASE_1[(ubi * 18) + 7] = 0x00;
			BUFB_BASE_1[(ubi * 18) + 8] = 0;
			BUFB_BASE_1[(ubi * 18) + 9] = 0x00;
			BUFB_BASE_1[(ubi * 18) + 10] = 0;
			BUFB_BASE_1[(ubi * 18) + 11] = 0x00;
		}

		// 前4個cmd隨便填時間...
		// 因為不想要特地去存cmd近來的時間...
		//BUFL_BASE_1[((ub_i * 18)+14)>>2] = uwCmdDone_MiliSeconds-(5-ub_i);
		U32 ulTime = *(&HL[HL_RCV_CMD_TMR_5	] + ubi);
		BUFB_BASE_1[((ubi * 18) + 14)] = ulTime;
		BUFB_BASE_1[((ubi * 18) + 15)] = ulTime >> 8;
		BUFB_BASE_1[((ubi * 18) + 16)] = ulTime >> 16;
		BUFB_BASE_1[((ubi * 18) + 17)] = ulTime >> 24;
	}
	BUFB_BASE_1[90 + 0] = BUFB_BASE_1[(4 * 18) + 0];
	BUFB_BASE_1[90 + 1] = HB[HB_ERROR];
	memcpy(&BUFB_BASE_1[90 + 2] , &BUFB_BASE_1[(4 * 18) + 3], 9);
	BUFB_BASE_1[90 + 11] = HB[HB_STATUS];



	if ( gubOfflineMode & (SMART_M_OFFLINEROUTINE | SMART_M_SHORTSELFTEST | SMART_M_EXTENDEDSELFTEST | SMART_M_CONVEYANCESELFTEST | SMART_M_SELECTIVESELFTEST)) {
		BUFB_BASE_1[90 + 31] = 0x04;
	}
	else {
		if (ubPM == PWR_SLEEP) {
			BUFB_BASE_1[90 + 31] = 0x01;
		}
		else if (ubPM == PWR_STANBY) {
			BUFB_BASE_1[90 + 31] = 0x02;
		}
		else {
			BUFB_BASE_1[90 + 31] = 0x03;
		}
	}
	BUFW_BASE_1[(90 + 32) >> 1] = uwCmdDone_Hours;


	// Ref. ATA-8-ACS Table A.7 — Extended Comprehensive SMART error log
	// maximum 64*4 = 256 , SMART_LOG3_PAGESIZE=64
	if ( AtaCfg.uwSmart03hIndex == (HOSTTABLE_SMARTLOG3_PAGESIZE * 4)) {
		AtaCfg.uwSmart03hIndex = 0;
	}
	Load_LogSector(HOSTTABLE_SMARTLOG3_ADR + AtaCfg.uwSmart03hIndex / 4); //load to buffer A
	memcpy( &BUFB_BASE[(124 * (AtaCfg.uwSmart03hIndex % 4)) + 4], &BUFB_BASE_1[0], 124);
	Save_LogSector(HOSTTABLE_SMARTLOG3_ADR + AtaCfg.uwSmart03hIndex / 4);


	// Is ubSmart03hPage maximum 64 ??
	if (((AtaCfg.uwSmart03hIndex % 4) == 0) && (AtaCfg.ubSmart03hPage < HOSTTABLE_SMARTLOG3_PAGESIZE)) {
		AtaCfg.ubSmart03hPage++;
	}
	AtaCfg.uwSmart03hIndex++;



	if (AtaCfg.uwSmart03hErrCnt != 0xFFFF) {
		AtaCfg.uwSmart03hErrCnt++;
	}

	SaveAtaConfig();
#endif
}

UBYTE WriteLogCheckSum(UWORD uw_LogSec)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);

	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];
	UBYTE ubOffset = 0;
	UBYTE ubFail = 0;
	UWORD uwi = 0;

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN;//because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	gubNeedChkDoneTag = 1;

	HB[HB_CMD_INFO_LOWBYTE] |= SET_BIT2; //set write direction

	for ( uwi = 0; uwi < gulSectorCnt; uwi++) {
		ubOffset = (UBYTE)((HL[HL_CMD_S_LBA] + uwi) & 0x07);
		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate((void *)&BUFB_BASE[ubOffset * 512], 512);
		}
		RWBuffer_forSMARTLog(1, (U32)ubOffset);
		if (ubOffset) {
			mSE_COPY((U32)(&BUFB_BASE[ubOffset * 512]), (U32)BUFB_BASE, 512);
		}
	}

	if (CheckCheckSum()) {
		ubFail = 1;
	}
	else {
		ubFail = 0;
		Save_LogSector(uw_LogSec);
	}

	//M_CheckDoneTag();
	while (ENABLE_READ_DONETAG && (DCCMSATAB[SATAB_RD_CNT] == 0)) {
		if (gubNeedChkDoneTag == 0) {
			VT->gulDoneTagMiss++;
			break;
		}
	}
	DCCMSATAB[SATAB_RPIU] = DCCMSATAB[SATAB_RD_CNT];
	gubLastDoneTag = 0x3F;

	gulSATA_MSG_TYPE3[0].All = 0;

	while (HL[HL_AXI_FIS_STAT]&BIT23);
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;

	return ubFail;
#endif
#if(RDT)
	return 0;
#endif
}

void IdentifyDeviceDataLog(UWORD uwLogPage)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	U32 uli;
	UBYTE ubDDT_temp = HB[HB_FLAG_CTRL];
	UWORD uwWRBLK_temp = HW[HW_AUTO_FIS_CTRL];

	HB[HB_FLAG_CTRL] &= ~H_DDT_EN;//because here pass sectors one by one (DDT needs 8k one time)
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	gubNeedChkDoneTag = 1;

	for (uli = 0; uli < gulSectorCnt; uli++) {
		mSE_SV(8, (U32)BUFB_BASE, 512, 0, 0);

		switch (uwLogPage + uli) {
			case 0:
				IdentifyDeviceDataLog_Page0_LogPage();
				break;
			case 1:
				Load_AtaIdenDrv();
				break;
			case 2:
				IdentifyDeviceDataLog_Page2_Capacity();
				break;
			case 3:
				IdentifyDeviceDataLog_Page3_SupportedCapabilities();
				break;
			case 4:
				IdentifyDeviceDataLog_Page4_CurrentSettings();
				break;
			case 5:
				IdentifyDeviceDataLog_Page5_ATA_Strings();
				break;
			case 6:
				IdentifyDeviceDataLog_Page6_Security();
				break;
			case 8:
				IdentifyDeviceDataLog_Page8_Serial_ATA();
				break;
			default:
				break;
		}

		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
		}
		RWBuffer_forSMARTLog(1, 0);
	}

	M_CheckDoneTag();
	gulSATA_MSG_TYPE3[0].All = 0;
	while (HL[HL_AXI_FIS_STAT]&BIT23);
	HB[HB_FLAG_CTRL] = ubDDT_temp;
	HW[HW_AUTO_FIS_CTRL] = uwWRBLK_temp;
}

void IdentifyDeviceDataLog_Page0_LogPage()
{

	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x00;
	BUFB_BASE[3] = 0x00;
	BUFB_BASE[6] = 0x00;
	BUFB_BASE[7] = 0x00;


	BUFB_BASE[8] = 0x08; // Number of entries (n) in the following list.
	BUFB_BASE[9] = 0x00;
	BUFB_BASE[10] = 0x01;
	BUFB_BASE[11] = 0x02;
	BUFB_BASE[12] = 0x03;
	BUFB_BASE[13] = 0x04;
	BUFB_BASE[14] = 0x05;
	BUFB_BASE[15] = 0x06;
	BUFB_BASE[16] = 0x08;
}
void IdentifyDeviceDataLog_Page2_Capacity()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	// Byte 0-7
	// 15:0 Revision number. Shall be set to 0001h.
	// 23:16 Page Number. Shall be set to 02h
	// 62:24 Reserved
	// 63 Shall be set to one.
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x02;
	BUFB_BASE[7] = 0x80;

	// Byte 8-15
	// 63 Shall be set to one.
	// 62:48 Reserved
	// 47:0 ACCESSIBLE CAPACITY field (see 9.11.4.2)

	// Accessible Max Address Configuration feature set並沒有支援   填也白填....
	BUFB_BASE[8] = gulNativeSize;
	BUFB_BASE[9] = gulNativeSize >> 8;
	BUFB_BASE[10] = gulNativeSize >> 16;
	BUFB_BASE[11] = gulNativeSize >> 24;
	BUFB_BASE[15] = 0x80;

	// Byte16-23
	// 63 Contents of the QWord are valid
	// 62 LOGICAL TO PHYSICAL SECTOR RELATIONSHIP SUPPORTED bit (see 9.11.4.3.1)		106 13
	// 61 LOGICAL SECTOR SIZE SUPPORTED bit (see 9.11.4.3.2)					106 12
	// 60:22 Reserved
	// 21:20 ALIGNMENT ERROR REPORTING field (see 9.11.4.3.3)					49 0:1
	// 19:16 LOGICAL TO PHYSICAL SECTOR RELATIONSHIP field (see 9.11.4.3.4)			106 0:3
	// 15:0 LOGICAL SECTOR OFFSET field (see 9.11.4.3.5)						209 0:13


	// Byte 24-31
	// 63 Contents of the QWord are valid
	// 62..32 Reserved
	//  31..0 LOGICAL SECTOR SIZE field (see 9.11.4.4)

	// Byte 32-39
	// 63 Contents of the QWord are valid
	// 62:0 BUFFER SIZE field (see 9.11.4.5)
}

void IdentifyDeviceDataLog_Page3_SupportedCapabilities()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubTemp = 0;
	// Byte 0-7
	// 63 Shall be set to one.
	// 62:24 Reserved
	// 23:16 Page Number. Shall be set to 03h.
	// 15:0 Revision number. Shall be set to 0001h.
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x03;
	BUFB_BASE[7] = 0x80;

	// BYTE 8-15
	// 63 Shall be set to one.
	//62:50 Reserved
	//51 DEFINITIVE ENDING PATTERN SUPPORTED bit (see 9.11.5.2.44)
	//50 DATA SET MANAGEMENT XL SUPPORTED bit (see 9.11.5.2.43)
	//49 SET SECTOR CONFIGURATION SUPPORTED bit (see 9.11.5.2.42)
	//48 ZERO EXT SUPPORTED bit (see 9.11.5.2.41)

	//Byte 15
	BUFB_BASE[15] = BIT7;
	//Byte 14
	if (gubZeroSupport) {
		BUFB_BASE[14] |= BIT0;
	}

	// --- Byte 13 ---//
	// 47 SUCCESSFUL NCQ COMMAND SENSE DATA SUPPORTED bit (see 9.11.5.2.40)
	// 46 DLC SUPPORTED bit (see 9.11.5.2.39)
	// 45 REQUEST SENSE DEVICE FAULT SUPPORTED bit (see 9.11.5.2.38)		沒支援  沒得判斷...以後有在說 之後加的人肯定不會加這裡...  唉...
	// 44 DSN SUPPORTED bit (see 9.11.5.2.37)					119 bit 9
	// 43 LOW POWER STANDBY SUPPORTED bit (see 9.11.5.2.36)			沒支援  沒得判斷...以後有在說 之後加的人肯定不會加這裡...  唉...
	// 42 SET EPC POWER SOURCE SUPPORTED bit (see 9.11.5.2.35)		沒支援  沒得判斷...以後有在說 之後加的人肯定不會加這裡...  唉...
	// 41 AMAX ADDR SUPPORTED bit (see 9.11.5.2.34)				119 bit 8
	// 40 Reserved for CFA (see 9.11.5.2.39)						69 bit 15
	ubTemp = 0;
	if ( IDT.W69 & BIT15) {
		ubTemp |= BIT0;
	}
	if ( IDT.W119 & BIT8) {
		ubTemp |= BIT1;
	}
	if ( IDT.W119 & BIT9) {
		ubTemp |= BIT4;
	}
	BUFB_BASE[13] = ubTemp;


	// --- Byte 12 ---//
	// 39 DRAT SUPPORTED bit (see 9.11.5.2.2)					69 bit 14
	// 38 LPS MISALIGNMENT REPORTING SUPPORTED bit (see 9.11.5.2.3)		69 bit 13
	// 37 Reserved
	// 36 READ BUFFER DMA SUPPORTED bit (see 9.11.5.2.4)				69 bit 11
	// 35 WRITE BUFFER DMA SUPPORTED bit (see 9.11.5.2.5)			69 bit 10
	// 34 Reserved
	// 33 DOWNLOAD MICROCODE DMA SUPPORTED bit (see 9.11.5.2.6)		69 bit 8
	// 32 28-BIT SUPPORTED bit (see 9.11.5.2.7)					69 bit 6
	ubTemp = 0;
	if ( IDT.W69 & BIT6) {
		ubTemp |= BIT0;
	}
	if ( IDT.W69 & BIT8) {
		ubTemp |= BIT1;
	}
	if ( IDT.W69 & BIT10) {
		ubTemp |= BIT3;
	}
	if ( IDT.W69 & BIT11) {
		ubTemp |= BIT4;
	}
	if ( IDT.W69 & BIT13) {
		ubTemp |= BIT6;
	}
	if ( IDT.W69 & BIT14) {
		ubTemp |= BIT7;
	}

	if (AtaCfg.uwDCOState & SET_DCO_TRIM) {
		ubTemp &= CLR_BIT7;
	}

	BUFB_BASE[12] = ubTemp;


	// --- Byte 11 ---//
	// 31 RZAT SUPPORTED bit (see 9.11.5.2.8)					69 bit 5
	// 30 Reserved
	// 29 NOP SUPPORTED bit (see 9.11.5.2.9)						82 bit 14
	// 28 READ BUFFER SUPPORTED bit (see 9.11.5.2.10)				82 bit 13
	// 27 WRITE BUFFER SUPPORTED bit (see 9.11.5.2.11)				82 bit 12
	// 26 Reserved
	// 25 READ LOOK-AHEAD SUPPORTED bit (see 9.11.5.2.12)			82 bit6
	// 24 VOLATILE WRITE CACHE SUPPORTED bit (see 9.11.5.2.13)			82 bit5
	ubTemp = 0;
	if ( IDT.W82 & BIT5 ) {
		ubTemp |= BIT0;
	}
	if ( IDT.W82 & BIT6 ) {
		ubTemp |= BIT1;
	}
	if ( IDT.W82 & BIT12 ) {
		ubTemp |= BIT3;
	}
	if ( IDT.W82 & BIT13 ) {
		ubTemp |= BIT4;
	}
	if ( IDT.W82 & BIT14 ) {
		ubTemp |= BIT5;
	}
	//  69 bit 5...
	if ( IDT.W69 & BIT5) {
		ubTemp |= BIT7;
	}

	if (AtaCfg.uwDCOState & SET_DCO_TRIM) {
		ubTemp &= CLR_BIT7;
	}

	BUFB_BASE[11] = ubTemp;


	// --- Byte 10 ---//
	// 23 SMART bit (see 9.11.5.2.14)							82 bit0
	// 22 FLUSH CACHE EXT SUPPORTED bit (see 9.11.5.2.15)			83 bit 13
	// 21 Reserved
	// 20 48-BIT SUPPORTED bit (see 9.11.5.2.16)					83 bit10
	// 19 Reserved
	// 18 SPIN-UP SUPPORTED bit (see 9.11.5.2.17)					83 bit6
	// 17 PUIS SUPPORTED bit (see 9.11.5.2.18)					83 bit5
	// 16 APM SUPPORTED bit (see 9.11.5.2.19)					83 bit3
	ubTemp = 0;
	if ( IDT.W83 & BIT3) {
		ubTemp |= BIT0;
	}
	if ( IDT.W83 & BIT5) {
		ubTemp |= BIT1;
	}
	if ( IDT.W83 & BIT6) {
		ubTemp |= BIT2;
	}
	if ( IDT.W83 & BIT10) {
		ubTemp |= BIT4;
	}
	if ( IDT.W83 & BIT13) {
		ubTemp |= BIT6;
	}
	if ( IDT.W82 & BIT0) {
		ubTemp |= BIT7;
	}

	if (AtaCfg.uwDCOState & SET_DCO_SMART) {
		ubTemp &= CLR_BIT7;
	}

	BUFB_BASE[10] = ubTemp;


	// --- Byte 9 ---//
	// 15 Reserved for CFA (see 9.11.5.2.39)						83 bit2
	// 14 DOWNLOAD MICROCODE SUPPORTED bit (see 9.11.5.2.20)			83 bit0
	// 13 UNLOAD SUPPORTED bit (see 9.11.5.2.21)					84 bit13
	// 12 WRITE FUA EXT SUPPORTED bit (see 9.11.5.2.22)				84 bit6
	// 11 GPL SUPPORTED bit (see 9.11.5.2.23)					84 bit5
	// 10 STREAMING SUPPORTED bit (see 9.11.5.2.24)				84 bit4
	// 9 Reserved
	// 8 SMART SELF-TEST SUPPORTED bit (see 9.11.5.2.25)			84 bit1
	ubTemp = 0;
	if (IDT.W84 & BIT1) {
		ubTemp |= BIT0;
	}
	ubTemp |=  ((IDT.W84 & (BIT4 | BIT5 | BIT6)) >> 4) << 2;

	if (IDT.W84 & BIT13) {
		ubTemp |= BIT5;
	}
	if (IDT.W83 & BIT0) {
		ubTemp |= BIT6;
	}
	if (IDT.W83 & BIT2) {
		ubTemp |= BIT7;
	}

	if (AtaCfg.uwDCOState & SET_DCO_SELFTEST) {
		ubTemp &= CLR_BIT0;
	}

	BUFB_BASE[9] = ubTemp;

	// --- Byte 8 ---//
	// 7 SMART ERROR LOGGING SUPPORTED bit (see 9.11.5.2.26) 			84 bit0
	// 6 EPC SUPPORTED bit (see 9.11.5.2.27)						119 bit7
	// 5 SENSE DATA SUPPORTED bit (see 9.11.5.2.28)				119 bit6
	// 4 FREE-FALL SUPPORTED bit (see 9.11.5.2.29)					119 bit5
	// 3 DM MODE 3 SUPPORTED bit (see 9.11.5.2.30)				119 bit4
	// 2 GPL DMA SUPPORTED bit (see 9.11.5.2.31)					119 bit3
	// 1 WRITE UNCORRECTABLE SUPPORTED bit (see 9.11.5.2.32)			119 bit2
	// 0 WRV SUPPORTED bit (see 9.11.5.2.33)					119 bit1
	ubTemp = 0;
	ubTemp |= (IDT.W119 & 0xFF) >> 1;   // bit 1~7

	if ( IDT.W84 & BIT0) {
		ubTemp |= BIT7;
	}

	if (AtaCfg.uwDCOState & SET_DCO_LOG) {
		ubTemp &= CLR_BIT7;
	}

	BUFB_BASE[8] = ubTemp;


	// Byte 16-23
	// 63 Contents of the QWord are valid
	// 62:35 Reserved

	// byte 20, bit 0,1,2
	//  34 DM OFFSETS DEFERRED SUPPORTED bit (see 9.11.5.3.1)      0Eh,0Fh
	// 33 DM IMMEDIATE SUPPORTED bit (see 9.11.5.3.2)                  07h
	// 32 DM OFFSETS IMMEDIATE SUPPORTED bit (see 9.11.5.3.3)     03h, 119 bit4

	if ((IDT.W119 & BIT4) && DLMC_Feature0x03_Support) {
		BUFB_BASE[20] |= BIT0;
	}

	if (DLMC_Feature0x07_Support) {
		BUFB_BASE[20] |= BIT1;
	}

	if (DLMC_Feature0x0E0x0F_Support) {
		BUFB_BASE[20] |= BIT2;
	}
	//--- byte 16 -17
	// 15:0 DM MINIMUM TRANSFER SIZE field (see 9.11.5.3.5)
	BUFB_BASE[16] = (UBYTE)(DLMC_MINSECTORCNT);
	BUFB_BASE[17] = (UBYTE)(DLMC_MINSECTORCNT >> 8);

	// ----- byte 18 -19
	// 31:16 DM MAXIMUM TRANSFER SIZE field (see 9.11.5.3.4)
	BUFB_BASE[18] = (UBYTE)(DLMC_MAXSECTORCNT);
	BUFB_BASE[19] = (UBYTE)(DLMC_MAXSECTORCNT >> 8);

	BUFB_BASE[23] = 0x80;

	// Byte 24-31
	// 63 Shall be set to one
	// 62:16 Reserved
	// 15:0 NOMINAL MEDIA ROTATION RATE field (see 9.11.5.4)
	BUFB_BASE[24] = 0x01;
	BUFB_BASE[31] = 0x80;

	// Byte 32-39
	// 63 Contents of the QWord are valid
	// 62:4 Reserved
	// 3:0 NOMINAL FORM FACTOR field (see 9.11.5.5)
	BUFB_BASE[32] = IDT.W168;
	BUFB_BASE[39] = 0x80;




	// 40..47 QWord Write-Read-Verify Sector Count Mode 3 Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:32 Reserved
	// 31:0 WRV MODE 3 COUNT field (see 9.11.5.6)				word 210-211




	// 48..55 QWord Write-Read-Verify Sector Count Mode 2 Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:32 Reserved
	// 31:0 WRV MODE 2 COUNT field (see 9.11.5.7)				word 212-213




	// 56..71 DQWord World wide name Bit Meaning
	// 127 Shall be set to one
	// 126:64 Reserved
	// 63:0 WORLD WIDE NAME field (see 9.11.5.8)
	BUFB_BASE[71] = 0x80;

	if (gubWWNSupport) {
		BUFB_BASE[56] = guwWWN[0];
		BUFB_BASE[57] = guwWWN[0] >> 8;
		BUFB_BASE[58] = guwWWN[1];
		BUFB_BASE[59] = guwWWN[1] >> 8;
		BUFB_BASE[60] = guwWWN[2];
		BUFB_BASE[61] = guwWWN[2] >> 8;
		BUFB_BASE[62] = guwWWN[3];
		BUFB_BASE[63] = guwWWN[3] >> 8;
	}

	// 72..79 QWord DATA SET MANAGEMENT Bit Meaning
	// 63 Shall be set to one
	// 62:16 Reserved
	// 15:8 LOGICAL BLOCK MARKUPS SUPPORTED field (see 9.11.5.9.1)     ???? 0
	// 7:1 Reserved
	// 0 TRIM SUPPORTED bit (see 9.11.5.9.2)					169 bit 0

	if ((AtaCfg.uwDCOState & SET_DCO_TRIM) == 0) {
		if (gubTrimSupport) {
			BUFB_BASE[72] = BIT0;
		}
	}
	BUFB_BASE[79] = 0x80;


	// 80..95 DQWord Utilization Per Unit Time (see 9.11.5.10) Bit Meaning
	// 127 Contents of the DQWord are valid
	// 126:120 Reserved
	// 119:112 UTILIZATION TYPE field (see 9.11.5.10.2)
	// 111:104 UTILIZATION UNITS field (see 9.11.5.10.3)
	// 103:96 UTILIZATION INTERVAL field (see 9.11.5.10.4)
	// 95:64 Reserved
	// 63:32 UTILIZATION B field (see 9.11.5.10.5)
	// 31:0 UTILIZATION A field (see 9.11.5.10.5)


	// 96..103 QWord Utilization Usage Rate Support Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:24 Reserved
	// 23 SETTING RATE BASIS SUPPORTED bit (see 9.11.5.11.1)
	// 22:9 Reserved
	// 8 SINCE POWER ON RATE BASIS SUPPORTED bit (see 9.11.5.11.2)
	// 7:5 Reserved
	// 4 POWER ON HOURS RATE BASIS SUPPORTED bit (see 9.11.5.11.3)
	// 3:1 Reserved
	// 0 DATE/TIME RATE BASIS SUPPORTED bit (see 9.11.5.11.4)


}

void IdentifyDeviceDataLog_Page4_CurrentSettings()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubTemp = 0;
	// 0..7 QWord Current Settings page information header.
	// Bit Meaning
	// 63 Shall be set to one.
	// 62:24 Reserved
	// 23:16 Page Number. Shall be set to 04h.
	// 15:0 Revision number. Shall be set to 0001h.
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x04;
	BUFB_BASE[3] = 0x00;
	BUFB_BASE[7] = 0x80;

	// 8..15 QWord Current Settings
	// Bit Meaning
	// 63 Shall be set to one.
	BUFB_BASE[15] = 0x80;
	// 62:17 Reserved
	// 16 DSN ENABLED bit (see 9.11.6.2.2)				120 bit 9
	ubTemp = 0;
	if ( IDT.W120 & BIT9) {
		ubTemp |= BIT0;
	}
	BUFB_BASE[10] = ubTemp;

	// --- BYTE 9 ---//
	// 15 EPC ENABLED bit (see 9.11.6.2.3)				120 bit 7
	// 14 Reserved
	// 13 VOLATILE WRITE CACHE ENABLED bit (see 9.11.6.2.4)	85 bit 5
	// 12 Reserved
	// 11 REVERTING TO DEFAULTS ENABLED bit (see 9.11.6.2.5)    gubRTD_enable
	// 10 SENSE DATA ENABLED bit (see 9.11.6.2.6)			120 bit 6
	// 9 Reserved
	// 8 NON-VOLATILE WRITE CACHE bit (see 9.11.6.2.7)		69 bit 2
	ubTemp = 0;
	if ( IDT.W69 & BIT2) {
		ubTemp |= BIT0;
	}
	if ( IDT.W120 & BIT6) {
		ubTemp |= BIT2;
	}
	if (gubRTD_enable) {
		ubTemp |= BIT3;
	}
	if ( IDT.W85 & BIT5) {
		ubTemp |= BIT5;
	}
	if ( IDT.W120 & BIT7) {
		ubTemp |= BIT7;
	}
	BUFB_BASE[9] = ubTemp;


	// --- BYTE 8 ---//
	// 7 READ LOOK-AHEAD ENABLED bit (see 9.11.6.2.8)		85 bit 6
	// 6 SMART ENABLED bit (see 9.11.6.2.9)				85 bit 0
	// 5 Reserved
	// 4 Reserved
	// 3 PUIS ENABLED bit (see 9.11.6.2.10)				86 bit 5
	// 2 APM ENABLED bit (see 9.11.6.2.11)				86 bit 3
	// 1 FREE-FALL ENABLED bit (see 9.11.6.2.12)			120 bit5
	// 0 WRV ENABLED bit (see 9.11.6.2.13)				120 bit 1
	ubTemp = 0;
	if ( IDT.W120 & BIT1) {
		ubTemp |= BIT0;
	}
	if ( IDT.W120 & BIT5) {
		ubTemp |= BIT1;
	}
	if ( IDT.W86 & BIT3) {
		ubTemp |= BIT2;
	}
	if ( IDT.W86 & BIT5) {
		ubTemp |= BIT3;
	}
	if ( IDT.W85 & BIT0) {
		ubTemp |= BIT6;
	}
	if ( IDT.W85 & BIT6) {
		ubTemp |= BIT7;
	}
	BUFB_BASE[8] = ubTemp;



	// 16..23 QWord Feature Settings
	// Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:16 reserved
	// 17:16 POWER SOURCE field (see 9.11.6.3.1)		0
	// 15:8 APM LEVEL field (see 9.11.6.3.2)			91   0:7
	// 7:0 WRV MODE field (see 9.11.6.3.3)			220  0:7
	BUFB_BASE[23] = 0x80;
	BUFB_BASE[16] = 0;
	BUFB_BASE[17] = IDT.W91;



	// 24..31 QWord DMA Host Interface Sector Times
	// Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:16 Reserved
	// 15:0 DMA SECTOR TIME field (see 9.11.6.4)		w96



	// 32..39 QWord PIO Host Interface Sector Times
	// Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:16 Reserved
	// 15:0 PIO SECTOR TIME field (see 9.11.6.5)\		w104

	// 40..47 QWord Streaming minimum request size
	// Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:16 Reserved
	// 15:0 STREAM MIN REQUEST SIZE field (see 9.11.6.6)		w95


	// 48..55 QWord Streaming access latency
	// Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:16 Reserved
	// 15:0 STREAM ACCESS LATENCY field (see 9.11.6.7)		w97


	// 56..63 QWord Streaming Performance Granularity
	// Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:32 Reserved
	// 31:0 STREAM GRANULARITY field (see 9.11.6.8)		w98-99


	// 64..71 QWord Free-fall Control Sensitivity
	// Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:16 Reserved
	// 7:0 FREE-FALL SENSITIVITY field (see 9.11.6.9)		w53  15:8


	// 72..79 QWord Device Maintenance Schedule
	// Bit Meaning
	// 63 Contents of the QWord are valid
	// 62:48 Reserved
	// 47:32 TIME SCHEDULED FOR DEVICE MAINTENANCE field (see 9.11.6.10.1)
	// 31:16 TIME TO PERFORMANCE DEGRADATION field (see 9.11.6.10.2)
	// 15:0 MINIMUM INACTIVE TIME field (see 9.11.6.10.3)


	//80..511 Reserved
}

void IdentifyDeviceDataLog_Page5_ATA_Strings()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UWORD uwi;
	// Strings
	// 0..7 QWord Strings page information header.
	//   Bit Meaning
	//   63 Shall be set to one.
	//   62:24 Reserved
	//   23:16 Page Number. Shall be set to 05h.
	//   15:0 Revision number. Shall be set to 0001h.
	// 8..27 ATA String SERIAL NUMBER field (see 9.11.7.2)
	// 28..31 Reserved
	// 32..39 ATA String FIRMWARE REVISION field (see 9.11.7.3)
	// 40..47 Reserved
	// 48..87 ATA String MODEL NUMBER field (see 9.11.7.4)
	// 88..95 Reserved
	// 96..103 ATA String ADDITIONAL PRODUCT IDENTIFIER field (see 9.11.7.5)
	// 104..511 Reserved
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;
	BUFB_BASE[2] = 0x05;
	BUFB_BASE[7] = BIT7;

	for (uwi = 0; uwi < 10; uwi++) {
		BUFB_BASE[8 + uwi * 2 + 0] = SerialTable[uwi]; //10-19:Serial number,
		BUFB_BASE[8 + uwi * 2 + 1] = SerialTable[uwi] >> 8; //10-19:Serial number,
	}

	for (uwi = 0; uwi < 4; uwi++) {
		BUFB_BASE[32 + uwi * 2 + 0] = VerTable[uwi]; //23-26:Version,
		BUFB_BASE[32 + uwi * 2 + 1] = VerTable[uwi] >> 8; //23-26:Version,
	}

	for (uwi = 0; uwi < 20; uwi++) {
		BUFB_BASE[48 + uwi * 2 + 0] = ModelTable[uwi]; //27-46:Model number,
		BUFB_BASE[48 + uwi * 2 + 1] = ModelTable[uwi] >> 8; //27-46:Model number,

	}
#endif
}

void IdentifyDeviceDataLog_Page6_Security()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubTemp = 0;

	// 0..7 QWord Security page information header.
	// Bit Meaning
	// 63 Shall be set to one.
	// 62:24 Reserved
	// 23:16 Page Number. Shall be set to 06h.
	// 15:0 Revision number. Shall be set to 0001h.
	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;

	BUFB_BASE[2] = 0x06;
	BUFB_BASE[7] = 0x80;

	if (IDT.W82 & BIT1) {
		// 8..15 QWord Master Password Identifier
		// Bit Meaning
		// 63 Contents of the QWord are valid.
		// 62:16 Reserved
		// 15:0 MASTER PASSWORD IDENTIFIER field (see 9.11.8.2)
		if ((AtaCfg.uwDCOState & SET_DCO_SEC) == 0) {
			BUFB_BASE[8] = AtaCfg.uwSecMasterID;
			BUFB_BASE[9] = AtaCfg.uwSecMasterID >> 8;
		}
		BUFB_BASE[15] = 0x80;

		// 16..23 QWord Security Status
		// Bit Meaning
		// 63 Contents of the QWord are valid
		// 62:7 Reserved
		// 6 SECURITY SUPPORTED bit (see 9.11.8.3.1)				128 bit 0
		// 5 MASTER PASSWORD CAPABILITY bit (see 9.11.8.3.2)		128 bit 8
		// 4 ENHANCED SECURITY ERASE SUPPORTED bit (see 9.11.8.3.3)	128 bit 5
		// 3 SECURITY COUNT EXPIRED bit (see 9.11.8.3.4)			128 bit 4
		// 2 SECURITY FROZEN bit (see 9.11.8.3.5)				128 bit 3
		// 1 SECURITY LOCKED bit (see 9.11.8.3.6)				128 bit 2
		// 0 SECURITY ENABLED bit (see 9.11.8.3.7)				128 bit 1

		ubTemp = 0;


		if ((AtaCfg.uwDCOState & SET_DCO_SEC) == 0) {
			if (AtaCfg.ubSecMode & SET_SEC_EN) {
				ubTemp |= BIT0;
			}

			if (AtaCfg.ubSecMode & SET_SEC_LOCK) {
				ubTemp |= BIT1;
			}

			if (AtaCfg.ubSecMode & SET_SEC_FROZEN) {
				ubTemp |= BIT2;
			}

			if (AtaCfg.ubSecMode & SET_SEC_PWEXCE) {
				ubTemp |= BIT3;
			}

			if (AtaCfg.ubSecMode & SET_SEC_ENHANCE_ERASE) {
				ubTemp |= BIT4;
			}

			if (AtaCfg.ubSecMode & SET_SEC_MAX) {
				ubTemp |= BIT5;
			}

			if (AtaCfg.ubSecMode & SET_SEC_SUPPORT) {
				ubTemp |= BIT6;
			}
		}

		BUFB_BASE[16] = ubTemp;
		BUFB_BASE[23] = 0x80;


		// 24..31 QWord Time required for an Enhanced Erase mode SECURITY ERASE UNIT command
		// Bit Meaning
		// 63 Contents of the QWord are valid
		// 62:16 Reserved
		// 15 ENHANCED SECURITY ERASE TIME FORMAT bit (see 9.11.8.4)				90 15
		// 14:0 ENHANCED SECURITY ERASE TIME field (see 9.11.8.4)					90  0:7

		if ((AtaCfg.uwDCOState & SET_DCO_SEC) == 0) {
			if (AtaCfg.ubSecMode & SET_SEC_ENHANCE_ERASE) {
				BUFW_BASE[24 / 2] = ENHANCED_SECURITY_TIME;
			}
		}
		BUFB_BASE[31] = 0x80;

		// 32..39 QWord Time required for a Normal Erase mode SECURITY ERASE UNIT command
		// Bit Meaning
		// 63 Contents of the QWord are valid
		// 62:16 Reserved
		// 15 NORMAL SECURITY ERASE TIME FORMAT bit (see 9.11.8.5)				89 15
		// 14:0 NORMAL SECURITY ERASE TIME field (see 9.11.8.5)					89 0:7

		if ((AtaCfg.uwDCOState & SET_DCO_SEC) == 0) {
			BUFW_BASE[32 / 2] = NORMAL_SECURITY_TIME;   // 89 Security Erase Time
		}
		BUFB_BASE[39] = 0x80;


		// 40..47 QWord Trusted Computing feature set
		// Bit Meaning
		// 63 Contents of the QWord are valid
		// 62:1 Reserved
		// 0 TRUSTED COMPUTING SUPPORTED bit (see 9.11.8.6)			48 0


		// 48..55 QWord Security Capabilities
		// Bit Meaning
		// 63 Contents of the QWord are valid
		// 62:8 Reserved
		// 7 RESTRICTED_SANITIZE_OVERRIDES_SECURITY (see.9.11.8.7.8)
		// 6 ACS-3 COMMANDS ALLOWED BY SANITIZE bit (see 9.11.8.7.7)		59 11
		// 5 SANITIZE ANTIFREEZE LOCK SUPPORTED bit (see 9.11.8.7.5)		59 10
		// 4 BLOCK ERASE SUPPORTED bit (see 9.11.8.7.1)				59 15
		// 3 OVERWRITE SUPPORTED bit (see 9.11.8.7.2)					59 14
		// 2 CRYPTO SCRAMBLE SUPPORTED bit (see 9.11.8.7.3)				59 13
		// 1 SANITIZE SUPPORTED bit (see 9.11.8.7.4)					59 12
		// 0 ENCRYPT ALL SUPPORTED bit (see 9.11.8.7.6)				69  4
		ubTemp = 0;
		if (IDT.W69 & BIT4) {
			ubTemp |= BIT0;
		}
		if (IDT.W59 & BIT12) {
			ubTemp |= BIT1;
		}
		if (IDT.W59 & BIT13) {
			ubTemp |= BIT2;
		}
		if (IDT.W59 & BIT14) {
			ubTemp |= BIT3;
		}
		if (IDT.W59 & BIT15) {
			ubTemp |= BIT4;
		}
		if (IDT.W59 & BIT10) {
			ubTemp |= BIT5;
		}
		if (IDT.W59 & BIT11) {
			ubTemp |= BIT6;
		}
		if (AtaCfg.ubSanitizeState & SET_RESTRICTED_SANITIZE_OVERRIDES_SECURITY) {
			ubTemp |= BIT7;
		}
		BUFB_BASE[48] = ubTemp;

		BUFB_BASE[55] = 0x80;

		// 56..511 Reserved
	}
}

void IdentifyDeviceDataLog_Page8_Serial_ATA()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	//
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

	BUFB_BASE[0] = 0x01;
	BUFB_BASE[1] = 0x00;

	BUFB_BASE[2] = 0x08;
	BUFB_BASE[7] = BIT7;

	ULLONG uoTemp = 0;
	// byte 8-15
	// SATA spec
	// 63  Shall be set to one.
	// 62:29 Reserved
	// 28  DIPM SSP PRESERVATION SUPPORTED   						// 78 bit 10
	// 27 Reserved
	// 26  DEVSLEEP TO REDUCEDPWRSTATE CAPABILITY SUPPORTED  , 			// 77 bit 7

	// 25  DEVICE SLEEP SUPPORTED  								// 78 bit 8
	// 24 NCQ AUTOSENSE SUPPORTED bit							// 78 bit 7
	// 23 SOFTWARE SETTINGS PRESERVATION SUPPORTED bit 				// 78 bit 6
	// 22 HARDWARE FEATURE CONTROL SUPPORTED bit 					// 78 bit 5
	// 21 IN-ORDER DATA DELIVERY SUPPORTED bit 						// 78 bit 4
	// 20 DEVICE INITIATED POWER MANAGEMENT SUPPORTED bit 				// 78 bit 3
	// 19 DMA SETUP AUTO-ACTIVATION SUPPORTED bit 					// 78 bit 2
	// 18 NON-ZERO BUFFER OFFSETS SUPPORTED bit 					// 78 bit 1

	// 17 SEND AND RECEIVE QUEUED COMMANDS SUPPORTED bit 				// 77 bit 6
	// 16 NCQ QUEUE MANAGEMENT COMMAND SUPPORTED bit 				// 77 bit 5
	// 15 NCQ STREAMING SUPPORTED bit 							// 77 bit 4

	// 14 READ LOG DMA EXT AS EQUIVALENT TO READ LOG EXT SUPPORTED bit 		// 76 bit 15
	// 13 DEVICE AUTOMATIC PARTIAL TO SLUMBER TRANSITIONS SUPPORTED bit 	// 76 bit 14
	// 12 HOST AUTOMATIC PARTIAL TO SLUMBER TRANSITIONS SUPPORTED bit 		// 76 bit 13
	// 11 NCQ PRIORITY INFORMATION SUPPORTED bit 					// 76 bit 12
	// 10 UNLOAD WHILE NCQ COMMANDS ARE OUTSTANDING SUPPORTED bit 		// 76 bit 11
	// 9 SATA PHY EVENT COUNTERS LOG SUPPORTED bit 					// 76 bit 10
	// 8 RECEIPT OF HOST INITIATED POWER MANAGEMENT REQUESTS SUPPORTED bit	// 76 bit 9
	// 7 NCQ FEATURE SET SUPPORTED bit  							// 76 bit 8
	// 6:3 Reserved for Serial ATA 								// 76 bit 4-7
	// 2 SATA GEN3 SIGNALING SPEED SUPPORTED bit 					// 76 bit 3
	// 1 SATA GEN2 SIGNALING SPEED SUPPORTED bit 					// 76 bit 2
	// 0 SATA GEN1 SIGNALING SPEED SUPPORTED bit 					// 76 bit 1

	uoTemp = 0;
	uoTemp = ((ULLONG)IDT.W76 >> 1);

	uoTemp |= ((((ULLONG)IDT.W77 & 0x70) >> 4) << 15);
	uoTemp |= ((((ULLONG)IDT.W78 & 0x1FE) >> 1) << 18);
	uoTemp |= ((((ULLONG)IDT.W77 & BIT7) >> 7) << 26);

	uoTemp |= ((((ULLONG)IDT.W78 & BIT10) >> 10) << 28);
	uoTemp |= ((ULLONG)BIT0 << 63);

	if (AtaCfg.uwDCOState & SET_DCO_IPM) {
		uoTemp &= (CLR_BIT8 & CLR_BIT20);
	}

	if (AtaCfg.uwDCOState & SET_DCO_SSP) {
		uoTemp &= (CLR_BIT23);
	}

	BUFL_BASE[8 >> 2] = uoTemp;
	BUFL_BASE[12 >> 2] = uoTemp >> 32;


	// byte 16-23
	// 63 Shall be set to one
	// 62:11 Reserved
	// 10  DEVICE SLEEP ENABLED  (sata spec)					// 79 bit 8
	// 9 AUTOMATIC PARTIAL TO SLUMBER TRANSITIONS ENABLED bit		// 79 bit 7
	// 8 SOFTWARE SETTINGS PRESERVATION ENABLED bit 				// 79 bit 6
	// 7 HARDWARE FEATURE CONTROL IS ENABLED bit 				// 79 bit 5
	// 6 IN-ORDER DATA DELIVERY ENABLED bit 					// 79 bit 4
	// 5 DEVICE INITIATED POWER MANAGEMENT ENABLED bit 			// 79 bit 3
	// 4 DMA SETUP AUTO-ACTIVATION ENABLED bit 					// 79 bit 2
	// 3 NON-ZERO BUFFER OFFSETS ENABLED bit 					// 79 bit 1
	// 2:0 CURRENT NEGOTIATED SERIAL ATA SIGNAL SPEED field    		// 77 bit1-3
	uoTemp = 0;

	uoTemp |= ((((ULLONG)IDT.W77 & 0xE) >> 1) );
	uoTemp |= ((((ULLONG)IDT.W79 & 0x1FE) >> 1) << 3);
	uoTemp |= ((ULLONG)BIT0 << 63);

	if (AtaCfg.uwDCOState & SET_DCO_IPM) {
		uoTemp &= (CLR_BIT5);
	}

	if (AtaCfg.uwDCOState & SET_DCO_SSP) {
		uoTemp &= (CLR_BIT8);
	}

	BUFL_BASE[16 >> 2] = (U32)uoTemp;
	BUFL_BASE[20 >> 2] = (U32)(uoTemp >> 32);

	// byte 48-55
	// 63  DEVSLP TIMING VARIABLES SUPPORTED (see 13.7.9.4.1)
	// 62:16 Reserved
	// 15:8  DEVSLEEP EXIT TIMEOUT (DETO) (see 13.7.9.4.2)
	// 7:5 Reserved
	// 4:0  MINIMUM DEVSLP ASSERTION TIME (MDAT) (see 13.7.9.4.3)

	if (IDT.W78 & BIT8) {
		BUFB_BASE[49] = DEVSLP_EXIT_TIMEOUT;//devslp exit timeout in ms
		BUFB_BASE[55] |= BIT7;
	}


}

#if (Hynix3DV6 || Hynix3DV5)
UBYTE Hynix3D_SetParameter(UBYTE ubAddr, UBYTE ubVal)
{
	//Set Parameter
	UWORD uwCount = 0;
	gFREG[FCTLL_PIO_CMD].L = 0x36;
	gFREG[FCTLL_PIO_ADR].L = ubAddr;
	gFREG[FCTLL_PIO_DAT].W.W0 = ubVal;
	gFREG[FCTLL_PIO_CMD].L = 0x16;
	gFREG[FCTLL_PIO_CMD].L = 0x70;
	while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0) {
		if (++uwCount > 40000) {
			return 1;
		}
	}
	return 0;
}

void Hynix3D_GetParameter(UBYTE ubAddr, UBYTE *ubData)
{


	UBYTE ubVal;
	UWORD uWVal;
	//Get Parameter
	gFREG[FCTLL_PIO_CMD].L = 0x37;
	gFREG[FCTLL_PIO_ADR].L = ubAddr;
	idle(10);
	*ubData = gFREG[FCTLL_PIO_DAT].B.B0;
}

UBYTE PollingStatusByDie(UBYTE ubDiePara)
{

	UWORD uwCount = 0;
	gFREG[FCTLL_PIO_CMD].L = 0x78;
	gFREG[FCTLL_PIO_ADR].L = 0x00;
	gFREG[FCTLL_PIO_ADR].L = 0x00;
	gFREG[FCTLL_PIO_ADR].L = ubDiePara;
	while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0) {
		if (++uwCount > 40000) {
			return 1;
		}
	}
	return 0;
}
#endif

UBYTE I2C_Temperature(UBYTE ubMode) // Mode=0 return Temperature, Mode=1 return thermal sensor exist or not
{
#if(!RDT)
	UBYTE ubTemperature = 33;
	UBYTE ubAddrSelect = 0x48;    /// TMP101 slave addr
	UBYTE ubTryThermoSensorCnt = 0;

	if ((!BURNER) && (ubMode == 0) && (gubThermalSensorMode == THERMAL_SENSOR_MODE_33)) {
		return 33;
	}
	else if ((!BURNER) && (ubMode == 0) && (gubThermalSensorMode == THERMAL_SENSOR_MODE_INNER)) {
		volatile unsigned long ulCount2 = 40000;

		*((volatile unsigned char*)0x04002202) |= 0x01;
		//*((volatile unsigned char*)0x04003157) |= 0x10;	//Using the register will occur double exception

		*((volatile unsigned char*)0x04000310) |= 0x80;
		*((volatile unsigned char*)0x04000310) &= 0xBF; /// enable thermometer

		while (ulCount2) {
			//		if( *((volatile unsigned char*)0x04000311) == 0x01 )
			if (( *((volatile unsigned char*)0x04003127) & 0x10 ) != 0) {

				ubTemperature = ( (*((volatile unsigned char*)0x04000312)) & 0x1F  ); ///Get value
				*((volatile unsigned char*)0x04000310) |= 0x40;    /// disable thermometer

				while ( (*((volatile unsigned char*)0x04003127) && 0x10) != 0 ) { ///W1c
					*((volatile unsigned char*)0x04003127) |=  0x10;
				}
				ulCount2 = 0;
				//							Uart_Printf("temperature : %d \r\n",ultemp);
			}
			else {
				ulCount2--;
			}
		}
		if (ubTemperature > 4) {
			ubTemperature = 10 * (ubTemperature - 4);
		}
		else {
			ubTemperature = 0;
		}
		return ubTemperature;
	}
	else if ((!BURNER) && (ubMode == 0) && (gubThermalSensorMode == THERMAL_SENSOR_MODE_MANUAL)) {
		return gubFAKETemperatureValue;
	}
	else {
		if (ubMode) {
			ubTemperature = THEMAL_SENSOR_NOT_EXIST;
		}
		else {
			//It had been get temperature fail after power cycle
			if (gubGetTempFailFlag) {
#if (!BURNER)
				return gubGetTempFailValue;
#else
				return TEMPERATURE_NACK_FAIL_RETURN_VALUE;	// -128
#endif
			}
		}
		SYS0L[SYS0L_SYS_MUX_CTRL0] &= ~(BIT27);
		SYS0L[SYS0L_SYS_MUX_CTRL0] |= BIT26;



		SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] &= 0xFFFFFF00;
		SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] |= (U32)(ubAddrSelect);

	MARK_RetryI2CThermoSensor:
		SYS1L[SYS1L_I2C_BUF0_REG] = 0; //clear buffer 0
		SYS1L[SYS1L_I2C_GP1_OP_REG] = 0;//read only

		////the master device must issue an Hs-mode master
		////code (00001XXX) as the first byte after a START condition
		////to switch the bus to high-speed operation

		SYS1L[SYS1L_I2C_GP1_CTRL] = ((1 << 16) | (2 << 8) | (0 << 4) | I2C_READ); // OP_CNT=1  DATA_CNT=2 BUFF=0 DIR=read | SPEED;

		SYS1L[SYS1L_I2C_GP1_CTRL] |= I2C_START; //Trigger

		while ((SYS1L[SYS1L_I2C_GP1_CTRL] & I2C_START) == 1); //Victor: if there is no I2C slave, maybe it will stuck at here.
		if ((SYS1L[SYS1L_I2C_GP1_NACK] & GP1_ANACK) == 0) {
			if (ubMode) {
				ubTemperature = THEMAL_SENSOR_EXIST; // 0x01 mean thermal sensor exist
			}
			else {
				U32 ulNACKTimer = gulOperationTime;
				U32 ulCTimer;
				while (SYS1L[SYS1L_I2C_GP1_NACK] & GP1_DNACK) {
					//Timeout Issue
					xthal_dcache_line_invalidate(&gulOperationTime); //Avoid Compiler Optimize
					ulCTimer = gulOperationTime;
					if (ulCTimer < ulNACKTimer) {
						ulCTimer += (0xFFFFFFFF - ulNACKTimer);
					}
					else {
						ulCTimer -= ulNACKTimer;
					}
					if (ulCTimer > TEMPERATURE_NACK_TIMEOUT) {
						//NACK Timeout -> Record VT & Set Temp Fail Flag
#if (!BURNER)
						VT->guwTempNACKTimeoutCount++;
#endif
						gubGetTempFailFlag = 1;
						//return Error Temp
#if (!BURNER)
						return gubGetTempFailValue;
#else
						return TEMPERATURE_NACK_FAIL_RETURN_VALUE;  // -128
#endif
					}
				}
				ubTemperature = SYS1B[SYS1L_I2C_BUF0_REG << 2];
			}
		}
		else {

			if ( ubTryThermoSensorCnt == 0) {
				ubTryThermoSensorCnt++;
				ubAddrSelect = 0x4A;      ///LM75BD  slave addr
				SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] &= 0xFFFFFF00;
				SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] |= (U32)(ubAddrSelect);
				goto MARK_RetryI2CThermoSensor;

			}
			else {
				if (ubMode) {
					ubTemperature = THEMAL_SENSOR_NOT_EXIST; // 0x00 mean thermal sensor doesn't exist
				}
				else {
					ubTemperature = 33;//if there is no thermal sensor, return 33C
				}
			}

		}

		return ubTemperature;
	}

#endif
#if(RDT)
	return 0;
#endif
}

ULLONG GetRTT_micros()
{
	U32 ulTimeL;
	U32 ulTimeH;
	ULLONG uoTimeMicrosecond;

	//clock use 1 MHz
	//Thomas: need load low 4byte, then load high 4byte (when load low 4byte, HW will lock high 4byte value)
	ulTimeL = SYSL[SYS0L_RTT_48BIT_CNTL];
	ulTimeH = SYSL[SYS0L_RTT_48BIT_CNTH];//use to check overflow
	uoTimeMicrosecond = (ULLONG)(ulTimeL + ((ULLONG)ulTimeH << 32));

	return uoTimeMicrosecond;
}

ULLONG GetRTT_milis()
{
	return (GetRTT_micros() / 1000);
}
ULLONG GetRTT_s()
{
	return (GetRTT_micros() / 1000000);
}
ULLONG GetRTT_m()
{
	return (GetRTT_micros() / 60000000);
}
ULLONG GetRTT_h()
{
	return (GetRTT_micros() / 60000000 / 60);
}

#if(!RDT)
void RTT1_SetTime_ReStart()
{
	RTT1_Stop();
	RTT1_Start();
}

void RTT1Clock_Init(UBYTE ubTimeScale)
{
	SYS1L[SYS1L_RTT_CTRL] = (SYS_RTTAURST | SYS_CNT_CLEAR | ubTimeScale);
	RTT_Set_Limit(1, 0xFFFFFFFF);
}

#endif
/***************************************************************************
* NAME:         void Read LOG EXT (void) - 0x2F
* DESCRIPTION:
*       This routine executes Device return Log to Host while NCQ had return Error.
*       This Command will cause the device to abort any command remaining in the queue.
*        log page 10h is not valid after a SoftReset or COMRESET.
***************************************************************************/
