#include "global/_types.h"
#include "setup.h"
#include "Vardef.h"
#include "io.h"
#include "ftl.h"
#include "RDT.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <xtensa/corebits.h>

#if (RDT_BURNER)

#define MARK_ERR_LOG 0x3FB15566
#define MARK_SDR_LOG 0x3FB11357
#define UNINITIALIZED_VALUE 0xFFFF
#define MAX_ERR_LOG_PAGE_COUNT 86

UWORD guwRDTErrorLogPageIndex = 0;
U32 gulRDTErrorLogUnit = UNINITIALIZED_VALUE;
U32 gulRDTErrorLogPageCount = UNINITIALIZED_VALUE;
U32 gulRDTErrorLogPB = UNINITIALIZED_VALUE;
U32 gulRDTSDRLogUnit = UNINITIALIZED_VALUE;
U32 gulRDTSDRLogPB = UNINITIALIZED_VALUE;

void FindRDTLogBlock(U32 ulSramAddr, U32 ulTargetLCA, U32 *ulFoundUnit, U32 *ulFoundPB)
{
	// Only check page 0
	UBYTE ubPageIndex = 0;

	UBYTE ubDepth, ubFQIndex, ubL4kLink, ubL4KNum = gub4kEntrysPerPlane;
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;
	U32 ubUnitCount = 0, ubPlaneBank = 0;

	for (ubUnitCount = 0; ubUnitCount < (256 / gubCENumber); ++ubUnitCount) {
		for (ubPlaneBank = 0; ubPlaneBank < (gubCENumber * gubBurstsPerBank); ubPlaneBank += gubBurstsPerBank) {
			// only check plane 0
			UWORD uwFEntryindex = ubPageIndex * (gubBurstsPerBank * gubCENumber * ubL4KNum) + (ubPlaneBank * ubL4KNum);

			UBYTE ubCEIndex = 0;
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular(((uwFEntryindex >> gub4kEntrysPerPlaneLog) >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = ((uwFEntryindex >> gub4kEntrysPerPlaneLog) & gubPlanesPerBurstMask) + ((M_Modular(((uwFEntryindex >> gub4kEntrysPerPlaneLog) >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}

#if 0
			UartString("\r\nCE : ");
			UartLLongHex(ubCEIndex);
#endif

			ubFQIndex = gFQI.ubFQLinkFree;
			FlashQueue_t *FQ;

			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ulRAMAddr = ulSramAddr;
			FQ->btCheckEmpty = 1;
			FQ->btThisIsErasePage = 0;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
			FQ->uwFUnit = ubUnitCount;
			FQ->ulFEntry = uwFEntryindex;
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;

			FQ->btUNServed = 1;
			FQ->btD3 = 1;
			FQ->FQPara.B.btConvBypass = 0;
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;//ubL4KNum=4
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

#if 0
			UartString("\nUnit:");
			UartWordHex(FQ->uwFUnit);
			UartString("\nPB:");
			UartWordHex(ubPlaneBank);
			UartString(" FEntry:");
			UartLongHex(FQ->ulFEntry);
#endif

			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate((void *)ulSramAddr, ubL4KNum * 4096);
			}

			UBYTE ubL4KCount = 0;
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {///ubL4KNum=00000004, 1page
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = 0x00000000;
				//pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
				pL4KTable.Para0x04.BitMap.ulL4K_FW = 0xFF;
				pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((ulSramAddr + (ubL4KCount * 4096)/* + 16384 * ubPageIndex*/) / 512);
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

			while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}

			gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

			pL4KTablePtr = (L4KTable16B*)(L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
#if 0
			UartString("\r\n LCA:");
			UartLongHex(pL4KTablePtr->ulL4K_LCA);
#endif

			if (pL4KTablePtr->ulL4K_LCA == ulTargetLCA) {
				*ulFoundUnit = ubUnitCount;
				*ulFoundPB = ubPlaneBank;
#if 1
				UartString("\r\n Unit:");
				UartLongHex(*ulFoundUnit);
				UartString("\r\n PB:");
				UartLongHex(*ulFoundPB);
				UartString("\r\n");
#endif
				return;
			}
		}//end for loop 2 ubUnitCount
	}
}

UBYTE ReadRDTLogBlock(U32 ulSramAddr, U32 ulUnit, U32 ulPB, U32 ulPage)
{
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	UWORD uwFEntryindex = ulPage * (gubBurstsPerBank * gubCENumber * ubL4KNum) + (ulPB * ubL4KNum);

	UBYTE ubCEIndex = ulPB / gubBurstsPerBank;
	UBYTE ubDepth, ubFQIndex, ubL4kLink;
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;

	ubFQIndex = gFQI.ubFQLinkFree;
	FlashQueue_t *FQ;

	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
	FQ->ulRAMAddr = ulSramAddr/* + 16384 * ulPage*/;//non-use
	FQ->btCheckEmpty = 1;
	FQ->btThisIsErasePage = 0;
	FQ->ubFJob = BYTE_FJOB_READ;
	FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
	FQ->uwFUnit = ulUnit;
	FQ->ulFEntry = uwFEntryindex;
	FQ->ubDepth = ubDepth;
	FQ->ubL4kIndex = ubL4kLink;

	FQ->btUNServed = 1;
	FQ->btD3 = 1;
	FQ->FQPara.B.btConvBypass = 0;
	gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
	gulPara0x24.BitMap.btBufModeEn = 0;
	gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
	gulPara0x24.BitMap.ubL4KNum = ubL4KNum;//ubL4KNum=4
	gulPara0x24.BitMap.btZipBypass = 1;
	gulPara0x24.BitMap.btCmpEn = 0;
	gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
	gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
	gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

#if 0
	UartString("\nUnit:");
	UartWordHex(FQ->uwFUnit);
	UartString(" FEntry:");
	UartLongHex(FQ->ulFEntry);
#endif

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)ulSramAddr, ubL4KNum * 4096);
	}
	UBYTE ubL4KCount = 0;
	for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {///ubL4KNum=00000004, 1page
		pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
		//set L4K table info
		pL4KTable.ulL4K_LCA = 0x00000000;
		//pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
		pL4KTable.Para0x04.BitMap.ulL4K_FW = 0xFF;
		pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0;
		pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((ulSramAddr + (ubL4KCount * 4096) + 16384 * 0/*ulPage*/) / 512);
		pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
		pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
		*pL4KTablePtr = pL4KTable;
	}
	gFQI.ubFQLinkNumber++;
	gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

	while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
		M_SwitchTask();
	}

	gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

	return 0;
}


void Vender_RDT_GetBadBlkLog(void)
{
	UWORD uwWRBLK_temp = (HW[HW_AUTO_FIS_CTRL] & H_WR_BLK_EN);
	UBYTE ubDDT_temp = (HB[HB_FLAG_CTRL] & H_DDT_EN);
	UWORD uwMSG_temp = (HW[HW_SYS_CTRL] & (H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
	HB[HB_FLAG_CTRL] &= (~H_DDT_EN);
	HW[HW_SYS_CTRL] &= (~(H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	U32 ulTempBufStart[2], ulTempBufEnd[2], ulTempBase;

	ulTempBase = HL[HL_BUF_BASE];
	ulTempBufStart[0] = HL[HL_BUF_R_SHF];
	ulTempBufStart[1] = HL[HL_BUF_W_SHF];
	ulTempBufEnd[0] = HL[HL_BUF_R_EP];
	ulTempBufEnd[1] = HL[HL_BUF_W_EP];

	HL[HL_BUF_BASE] = BUFFER3_BASE / 4096;
	HL[HL_BUF_R_EP] = BUFFER3_READ_END;//(BUFFER2_READ_START+(32/8)-1); //
	HL[HL_BUF_W_EP] = BUFFER3_WRITE_END;//(BUFFER2_READ_START+(32/8)-1); //
	HL[HL_BUF_R_SHF] = BUFFER3_READ_START; //0;
	HL[HL_BUF_W_SHF] = BUFFER3_WRITE_START; //0;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)BUFL_BASE[0], 512 * 128);
	}

	if (gulRDTErrorLogUnit == UNINITIALIZED_VALUE) {
		FindRDTLogBlock(BUFFER3_BASE, MARK_ERR_LOG, &gulRDTErrorLogUnit, &gulRDTErrorLogPB);
	}

	if (gulRDTErrorLogUnit != UNINITIALIZED_VALUE) {

		if (gulRDTErrorLogPageCount == UNINITIALIZED_VALUE) {
			gulRDTErrorLogPageCount = (gulRDTErrorLogPageCount < MAX_ERR_LOG_PAGE_COUNT) ? gulRDTErrorLogPageCount : MAX_ERR_LOG_PAGE_COUNT;
		}

		UWORD uwPageSize = 0x4000;
#if Hynix_3D
		uwPageSize = 0x2000;
#endif
		UBYTE ubPageOffset = 0;
		for (ubPageOffset = 0; (ubPageOffset < 4) && ((guwRDTErrorLogPageIndex + ubPageOffset) < gulRDTErrorLogPageCount); ++ubPageOffset) {
#if 0
			UartString("Page : ");
			UartLongHex(guwRDTErrorLogPageIndex + ubPageOffset);
			UartString("\r\n");
#endif
			ReadRDTLogBlock(BUFFER3_BASE + (ubPageOffset * uwPageSize), gulRDTErrorLogUnit, gulRDTErrorLogPB, guwRDTErrorLogPageIndex + ubPageOffset);
		}
		guwRDTErrorLogPageIndex += 4;
		if (guwRDTErrorLogPageIndex >= gulRDTErrorLogPageCount) {
			guwRDTErrorLogPageIndex = 0;
		}
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv(BUFB_BASE, 512);
	}

	HL[HL_BUF_OFST] = 0;
	HW[HW_SECTOR_CNT] = gulSectorCnt;
	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	HB[HB_DATA_TRG] = H_DATA_TRG;
	while (HB[HB_DATA_TRG]&H_DATA_TRG);
	HB[HB_FLAG_CTRL] |= ubDDT_temp;
	HW[HW_SYS_CTRL] |= uwMSG_temp;
	HW[HW_AUTO_FIS_CTRL] |= uwWRBLK_temp;
}

void Vender_RDT_GetTimeStampLog(void)
{
	UWORD uwWRBLK_temp = (HW[HW_AUTO_FIS_CTRL] & H_WR_BLK_EN);
	UBYTE ubDDT_temp = (HB[HB_FLAG_CTRL] & H_DDT_EN);
	UWORD uwMSG_temp = (HW[HW_SYS_CTRL] & (H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
	HB[HB_FLAG_CTRL] &= (~H_DDT_EN);
	HW[HW_SYS_CTRL] &= (~(H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	U32 ulTempBufStart[2], ulTempBufEnd[2], ulTempBase;

	ulTempBase = HL[HL_BUF_BASE];
	ulTempBufStart[0] = HL[HL_BUF_R_SHF];
	ulTempBufStart[1] = HL[HL_BUF_W_SHF];
	ulTempBufEnd[0] = HL[HL_BUF_R_EP];
	ulTempBufEnd[1] = HL[HL_BUF_W_EP];

	HL[HL_BUF_BASE] = BUFFER3_BASE / 4096;
	HL[HL_BUF_R_EP] = BUFFER3_READ_END;//(BUFFER2_READ_START+(32/8)-1); //
	HL[HL_BUF_W_EP] = BUFFER3_WRITE_END;//(BUFFER2_READ_START+(32/8)-1); //
	HL[HL_BUF_R_SHF] = BUFFER3_READ_START; //0;
	HL[HL_BUF_W_SHF] = BUFFER3_WRITE_START; //0;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)BUFL_BASE[0], 512 * 128);
	}

	if (gulRDTSDRLogUnit == UNINITIALIZED_VALUE) {
		FindRDTLogBlock(BUFFER3_BASE, MARK_SDR_LOG, &gulRDTSDRLogUnit, &gulRDTSDRLogPB);
	}

	if (gulRDTSDRLogUnit != UNINITIALIZED_VALUE) {
		/*
		if (gulRDTErrorLogPageCount == UNINITIALIZED_VALUE) {
		    ReadRDTLogBlock(BUFFER3_BASE, gulRDTSDRLogUnit, gulRDTSDRLogPB, guwFastPagePagesPerUnit - 1);
		    gulRDTErrorLogPageCount = (*(UBYTE *)BUFFER3_BASE);
		    gulRDTErrorLogPageCount = (gulRDTErrorLogPageCount < MAX_ERR_LOG_PAGE_COUNT) ? gulRDTErrorLogPageCount : MAX_ERR_LOG_PAGE_COUNT;
		}
		*/
		// temperature log is stored at page 1 of the SDR log
		ReadRDTLogBlock(BUFFER3_BASE, gulRDTSDRLogUnit, gulRDTSDRLogPB, 1);
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv(BUFB_BASE, 512);
	}

	HL[HL_BUF_OFST] = 0;
	HW[HW_SECTOR_CNT] = gulSectorCnt;
	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	HB[HB_DATA_TRG] = H_DATA_TRG;
	while (HB[HB_DATA_TRG]&H_DATA_TRG);
	HB[HB_FLAG_CTRL] |= ubDDT_temp;
	HW[HW_SYS_CTRL] |= uwMSG_temp;
	HW[HW_AUTO_FIS_CTRL] |= uwWRBLK_temp;
}

void Vender_RDT_GetSDRLog(void)
{
	UWORD uwWRBLK_temp = (HW[HW_AUTO_FIS_CTRL] & H_WR_BLK_EN);
	UBYTE ubDDT_temp = (HB[HB_FLAG_CTRL] & H_DDT_EN);
	UWORD uwMSG_temp = (HW[HW_SYS_CTRL] & (H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
	HB[HB_FLAG_CTRL] &= (~H_DDT_EN);
	HW[HW_SYS_CTRL] &= (~(H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
	HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

	U32 ulTempBufStart[2], ulTempBufEnd[2], ulTempBase;

	ulTempBase = HL[HL_BUF_BASE];
	ulTempBufStart[0] = HL[HL_BUF_R_SHF];
	ulTempBufStart[1] = HL[HL_BUF_W_SHF];
	ulTempBufEnd[0] = HL[HL_BUF_R_EP];
	ulTempBufEnd[1] = HL[HL_BUF_W_EP];

	HL[HL_BUF_BASE] = BUFFER3_BASE / 4096;
	HL[HL_BUF_R_EP] = BUFFER3_READ_END;//(BUFFER2_READ_START+(32/8)-1); //
	HL[HL_BUF_W_EP] = BUFFER3_WRITE_END;//(BUFFER2_READ_START+(32/8)-1); //
	HL[HL_BUF_R_SHF] = BUFFER3_READ_START; //0;
	HL[HL_BUF_W_SHF] = BUFFER3_WRITE_START; //0;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)BUFL_BASE[0], 512 * 128);
	}

	if (gulRDTSDRLogUnit == UNINITIALIZED_VALUE) {
		FindRDTLogBlock(BUFFER3_BASE, MARK_SDR_LOG, &gulRDTSDRLogUnit, &gulRDTSDRLogPB);
	}

	if (gulRDTSDRLogUnit != UNINITIALIZED_VALUE) {
		/*
		if (gulRDTErrorLogPageCount == UNINITIALIZED_VALUE) {
		    ReadRDTLogBlock(BUFFER3_BASE, gulRDTSDRLogUnit, gulRDTSDRLogPB, guwFastPagePagesPerUnit - 1);
		    gulRDTErrorLogPageCount = (*(UBYTE *)BUFFER3_BASE);
		    gulRDTErrorLogPageCount = (gulRDTErrorLogPageCount < MAX_ERR_LOG_PAGE_COUNT) ? gulRDTErrorLogPageCount : MAX_ERR_LOG_PAGE_COUNT;
		}
		*/
		ReadRDTLogBlock(BUFFER3_BASE, gulRDTSDRLogUnit, gulRDTSDRLogPB, 0);
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv(BUFB_BASE, 512);
	}

	HL[HL_BUF_OFST] = 0;
	HW[HW_SECTOR_CNT] = gulSectorCnt;
	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	HB[HB_DATA_TRG] = H_DATA_TRG;
	while (HB[HB_DATA_TRG]&H_DATA_TRG);
	HB[HB_FLAG_CTRL] |= ubDDT_temp;
	HW[HW_SYS_CTRL] |= uwMSG_temp;
	HW[HW_AUTO_FIS_CTRL] |= uwWRBLK_temp;
}
#endif

//for ISP_Build_Header
extern UWORD SerialTable[10];
#define IBF_BANK_LENGTH					5120

#if Hynix
void FlaIssueHynixDaCmd(U32 ch, U32 ce, U32 cmd)
{

	// Note: issue command DAh or BEh to nand, and inform fpl current nand write/read mode to determine use or not use A2 command.
	//       It only changes the single plane read/write flow
#if 1
	if ( gulParamPageInfo.JEDEC.btNandSupplyByEmulator == 0 ) {
		if (guoFlashDefaultType.B.bt8LC && guoFlashUseType.B.btHynix) {
			volatile REG_t *FREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ch);
			gubDaCmdEnable = (cmd == 0xDA) ? (TRUE) : (FALSE);
			FCONL[FCONL_FCE_ENB] |= ce;
			UBYTE ubDieOffset = (flaCheckLogValue((U32)(guwDieBlockShift >> 8)) + (gubPlanesPerPageLog - gubPlanesPerBurstLog));
			UBYTE ubDie;

			if (guoFlashDefaultType.B.btHynix3DV5) {
				ubDieOffset = 5;
			}
			else if ((guoFlashDefaultType.B.btHynix3DV6)) {
				ubDieOffset = 6;
			}

			for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
				FREG[FCTLL_PIO_CMD].L = 0x78;
				FREG[FCTLL_PIO_ADR].L = 0x00;
				FREG[FCTLL_PIO_ADR].L = 0x00;
				FREG[FCTLL_PIO_ADR].L = (ubDie << ubDieOffset);
#if !(Hynix3DV6 || Hynix3DV5)
				FREG[FCTLL_PIO_CMD].L = 0x36;
#if !Hynix_16nm
				FREG[FCTLL_PIO_ADR].L = 0x96;
				PIO_WDATA(0x04, FREG);
#endif
				FREG[FCTLL_PIO_ADR].L = 0x71;

				if (gubDaCmdEnable) {
					PIO_WDATA(0x01, FREG);
				}
				else {
					PIO_WDATA(0x00, FREG);
				}

#if !Hynix_16nm
				FREG[FCTLL_PIO_ADR].L = 0x96;
				PIO_WDATA(0x00, FREG);
#endif
				FREG[FCTLL_PIO_CMD].L = 0x16;
#endif
				FREG[FCTLL_PIO_CMD].L = cmd;
				if ((ch == FALL) && (ce != 0xFFFFFFFF)) {
					break;
				}
			}
			FCONL[FCONL_FCE_ENB] &= (~ce);
		}
		else {
			gubDaCmdEnable = FALSE;
		}
	}
#endif
}
#endif

UBYTE ISP_Prog_DummyData(U32 ulUnitIndex, UWORD uwPlane,  UBYTE ubCEIndex, UBYTE ubL4KNum)
{
	M_CheckRDTModeToDisableFunctionReturnValid(0);

	UBYTE *ubBUF_16Kshift = (UBYTE *)(BUFFER3_BASE + 0x4000);
	UWORD uwPlane_PTR;
	for (uwPlane_PTR = uwPlane; uwPlane_PTR < guwFastPagePagesPerUnit; uwPlane_PTR++) {
		if (ftlWriteSinglePlane(ulUnitIndex, uwPlane_PTR, ubCEIndex, (U32)&ubBUF_16Kshift[0], 0x3FA000FF, 0, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), ubL4KNum, 0, 0)) {
			ftlPreformatShowError(ubCEIndex, ulUnitIndex, 0x12);
		}
	}
	return 0;

}

void M_SETOP(UBYTE ubOP, U32 ulValue, U32 ulValue2, U32 ulDes, UBYTE ubType, U32 ulOPAddr)
{
	M_CheckRDTModeToDisableFunction();

	RAMB_START[ulOPAddr] = ubOP;
	RAMB_START[(ulOPAddr + 1)] = ubType;
	RAML_START[(ulOPAddr + 4) / 4] = ulValue;
	RAML_START[(ulOPAddr + 8) / 4] = ulDes;
	RAML_START[(ulOPAddr + 12) / 4] = ulValue2;
}

//UBYTE i;


UWORD FlaGenerateSetFeatureOP_Code(U32 ulStartAddr)
{
	M_CheckNonBurnerModeToDisableFunctionReturnValid(0)

	UWORD uwOffset = 0;
	UBYTE ubchannel;
	UBYTE ubDiePerChannel = gubDieNumber * (gubCENumber / gubPlanesPerBurst);
	//only for TSB
	//=============================
	if (UFS) {
		gubFeature_ADR = 0x80;
		gubFeature_B0 = 0x00;
		M_SETOP(OP_AND1, 0x04009400 + (FCTLL_FLH_SET << 2), FLA_LEGACY_MODE, 0x04009400 + (FCTLL_FLH_SET << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	else {
		if (gubSupportToggle2 && (gubToggleMode == 1)) {
			gubFeature_ADR = 0x02;
			gubFeature_B0 = 0x07;
			M_SETOP(OP_OR1, 0x04009400 + (FCTLL_FLH_SET << 2), FLA_TOGGLE_MODE, 0x04009400 + (FCTLL_FLH_SET << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
		}
		else if (gubSupportToggle2 && (gubToggleMode == 0)) {
			gubFeature_ADR = 0x80;
			gubFeature_B0 = 0x00;
			M_SETOP(OP_AND1, 0x04009400 + (FCTLL_FLH_SET << 2), FLA_LEGACY_MODE, 0x04009400 + (FCTLL_FLH_SET << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
		}
		else if ((gubInterface == INTERFACE_TOGGLE1) && (gubToggleMode == 0)) {
			gubFeature_ADR = 0x80;
			gubFeature_B0 = 0x00;
			M_SETOP(OP_AND1, 0x04009400 + (FCTLL_FLH_SET << 2), FLA_LEGACY_MODE, 0x04009400 + (FCTLL_FLH_SET << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
		}
		else {
			return 0;
		}
	}
	//================================

	if (0) {     //gubGPIO_CE_Decoder之後再用
		M_SETOP(OP_SET, 0x8080, 0x00, 0x04009600 + (FCONL_FCE_ENB << 2), OP_4B, ulStartAddr + uwOffset);
	}
	else {
		M_SETOP(OP_SET, 0x0101, 0x00, 0x04009600 + (FCONL_FCE_ENB << 2), OP_4B, ulStartAddr + uwOffset);
	}
	uwOffset += 0x10;
	/*if (gubFlashID[0] == 0xAD) {
		// 為了hynix set feature後load code fail, 重新scan header又再一次set feature
		M_SETOP(OP_SET, 0xFF, 0x00, 0x04009400 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_WHILE_AND_E, 0x04009000 + (FCTLL_RBY_INF << 2), FLH_CE_RBY_AND, 0x00, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_WHILE_AND_E, 0x04009200 + (FCTLL_RBY_INF << 2), FLH_CE_RBY_AND, 0x00, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009400 + (FCTLL_FLH_SET << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}*/
	for (ubchannel = 0; ubchannel < gubPlanesPerBurst; ubchannel++) {
		M_SETOP(OP_SET, 0xEF, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, gubFeature_ADR, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_ADR << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_AND1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), CLR_BIT13, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, gubFeature_B0, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_AND1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), CLR_BIT12, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), PIO_DAT_LAST, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), PIO_DAT_FIRST, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	M_SETOP(OP_SET, 0x70, 0x00, 0x04009400 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_WHILE_AND_NE, 0x04009000 + (FCTLL_PIO_DAT << 2), 0xE0, 0xE0, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_WHILE_AND_NE, 0x04009200 + (FCTLL_PIO_DAT << 2), 0xE0, 0xE0, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_OR1, 0x04009000 + (FCTLL_FLH_SET << 2), FLA_TOGGLE_MODE, 0x04009000 + ubchannel * 0x200 + (FCTLL_FLH_SET << 2), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_OR1, 0x04009200 + (FCTLL_FLH_SET << 2), FLA_TOGGLE_MODE, 0x04009000 + ubchannel * 0x200 + (FCTLL_FLH_SET << 2), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	/*if (gubInterface == INTERFACE_NVDDR2) {
		if (gubFlashID[0] == 0xAD) { // Hynix
			M_SETOP(OP_SET, 0xFF, 0x00, 0x04009400 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_WHILE_AND_E, 0x04009000 + (FCTLL_RBY_INF << 2), FLH_CE_RBY_AND, 0x00, OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_WHILE_AND_E, 0x04009200 + (FCTLL_RBY_INF << 2), FLH_CE_RBY_AND, 0x00, OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_SET, 0x36, 0x00, 0x04009400 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			if (guwFlashType.Hynix.btF20Bdie) {
				M_SETOP(OP_SET, 0xE1, 0x00, 0x04009400 + (FCTLL_PIO_ADR << 2), OP_4B, ulStartAddr + uwOffset);
				uwOffset += 0x10;
			}
			else if (guwFlashType.Hynix.btF16Edie) {
				M_SETOP(OP_SET, 0x61, 0x00, 0x04009400 + (FCTLL_PIO_ADR << 2), OP_4B, ulStartAddr + uwOffset);
				uwOffset += 0x10;
			}
			M_SETOP(OP_SET, 0x40, 0x00, 0x04009400 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_SET, 0x16, 0x00, 0x04009400 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_WHILE_AND_E, 0x04009000 + (FCTLL_RBY_INF << 2), FLH_CE_RBY_AND, 0x00, OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_WHILE_AND_E, 0x04009200 + (FCTLL_RBY_INF << 2), FLH_CE_RBY_AND, 0x00, OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
		}
		M_SETOP(OP_SET, 0xEF, 0x00, 0x04009400 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, gubFeature1_ADR, 0x00, 0x04009400 + (FCTLL_PIO_ADR << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_AND1, 0x04009400 + (FCTLL_HS_MODE << 2), CLR_BIT13, 0x04009400 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, gubFeature1_B0, 0x00, 0x04009400 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_AND1, 0x04009400 + (FCTLL_HS_MODE << 2), CLR_BIT12, 0x04009400 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009400 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009400 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04009400 + (FCTLL_HS_MODE << 2), PIO_DAT_LAST, 0x04009400 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009400 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04009400 + (FCTLL_HS_MODE << 2), PIO_DAT_FIRST, 0x04009400 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_WHILE_AND_E, 0x04009000 + (FCTLL_RBY_INF << 2), FLH_CE_RBY_AND, 0x00, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_WHILE_AND_E, 0x04009200 + (FCTLL_RBY_INF << 2), FLH_CE_RBY_AND, 0x00, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	else*/ if (((gubInterface == INTERFACE_TOGGLE2) && (gubToggleMode == 0)) || gubInterface == INTERFACE_NVDDR2) {

		gubFeature1_B0 = (BIT2 | BIT1 | BIT0);
		gubFeature1_ADR = 0x02;
		for (ubchannel = 0; ubchannel < gubPlanesPerBurst; ubchannel++) {

			M_SETOP(OP_SET, 0xEF, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_SET, gubFeature1_ADR, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_ADR << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_AND1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), CLR_BIT13, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_SET, gubFeature1_B0, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_AND1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), CLR_BIT12, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_SET, 0x00, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_SET, 0x00, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_OR1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), PIO_DAT_LAST, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_SET, 0x00, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
			M_SETOP(OP_OR1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), PIO_DAT_FIRST, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
		}
		M_SETOP(OP_SET, 0x70, 0x00, 0x04009400 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_WHILE_AND_NE, 0x04009000 + (FCTLL_PIO_DAT << 2), 0xE0, 0xE0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_WHILE_AND_NE, 0x04009200 + (FCTLL_PIO_DAT << 2), 0xE0, 0xE0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}

	M_SETOP(OP_SET, 0x00, 0x00, 0x04009600 + (FCONL_FCE_ENB << 2), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;

	// NVDDR -> ONFI,  NVDDR2 -> Toggle
	if (gubInterface == INTERFACE_NVDDR) {
		M_SETOP(OP_OR1, 0x04009400 + (FCTLL_FLH_SET << 2), FLA_ONFI_MODE, 0x04009400 + (FCTLL_FLH_SET << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	else {
		M_SETOP(OP_OR1, 0x04009400 + (FCTLL_FLH_SET << 2), FLA_TOGGLE_MODE, 0x04009400 + (FCTLL_FLH_SET << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		if ((gubInterface == INTERFACE_TOGGLE1) || (gubInterface == INTERFACE_TOGGLE2)) {
			M_SETOP(OP_SET, 0xBB, 0x00, 0x04000000 + (SYS0L_SIM_CTRL1 << 2) + 2, OP_1B, ulStartAddr + uwOffset);
			uwOffset += 0x10;
		}
	}

	UBYTE ubFeature_ADR = 0x10;
#if MicronFlashOnly
	UBYTE ubFeature_B0 = 0x28;		// temp use NVDDR2 400 MB/s
#else
	UBYTE ubFeature_B0 = 0x06;
#endif

	if (0) {     //gubGPIO_CE_Decoder之後再用
		M_SETOP(OP_SET, 0x8080, 0x00, 0x04009600 + (FCONL_FCE_ENB << 2), OP_4B, ulStartAddr + uwOffset);
	}
	else {
		M_SETOP(OP_SET, 0x0101, 0x00, 0x04009600 + (FCONL_FCE_ENB << 2), OP_4B, ulStartAddr + uwOffset);
	}
	uwOffset += 0x10;

	for (ubchannel = 0; ubchannel < gubPlanesPerBurst; ubchannel++) {
		M_SETOP(OP_SET, 0xEF, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, ubFeature_ADR, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_ADR << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_AND1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), CLR_BIT13, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, ubFeature_B0, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_AND1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), CLR_BIT12, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), PIO_DAT_LAST, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00, 0x00, 0x04009000 + ubchannel * 0x200 + (FCTLL_PIO_DAT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), PIO_DAT_FIRST, 0x04009000 + ubchannel * 0x200 + (FCTLL_HS_MODE << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	M_SETOP(OP_SET, 0x70, 0x00, 0x04009400 + (FCTLL_PIO_CMD << 2), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_WHILE_AND_NE, 0x04009000 + (FCTLL_PIO_DAT << 2), 0xE0, 0xE0, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_WHILE_AND_NE, 0x04009200 + (FCTLL_PIO_DAT << 2), 0xE0, 0xE0, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;

	M_SETOP(OP_SET, 0x00, 0x00, 0x04009600 + (FCONL_FCE_ENB << 2), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;

	M_SETOP(OP_SET, 0x02804020, 0x00, 0x04002100 + 0x14, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_SET, 0x01804041, 0x00, 0x04002100 + 0x18, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_SET, 0x00804041, 0x00, 0x04002100 + 0x1c, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;

	if (ubDiePerChannel >= 8) {
		M_SETOP(OP_SET, 0x00F0C041, 0x00, 0x04002100 + 0x20, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00F0C041, 0x00, 0x04002100 + 0x24, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00F0C041, 0x00, 0x04002100 + 0x40, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00F0C041, 0x00, 0x04002100 + 0x44, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F0C011, 0x00, 0x04002100 + 0x60, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F0C011, 0x00, 0x04002100 + 0x64, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F0C011, 0x00, 0x04002100 + 0x80, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F0C011, 0x00, 0x04002100 + 0x84, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00FFC041, 0x00, 0x04002100 + 0xA0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00FFC041, 0x00, 0x04002100 + 0xA4, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00000041, 0x00, 0x04002100 + 0xC0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00000041, 0x00, 0x04002100 + 0xC4, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0xCFCFCFCF, 0x00, 0x04002100 + 0xE0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0xCFCFCFCF, 0x00, 0x04002100 + 0xE4, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0xCFCFCFCF, 0x00, 0x04002100 + 0xE8, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0xCFCFCFCF, 0x00, 0x04002100 + 0xEC, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	else if (ubDiePerChannel >= 4) { //Zout == 25ohm
		M_SETOP(OP_SET, 0x00F08041, 0x00, 0x04002100 + 0x20, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00F08041, 0x00, 0x04002100 + 0x24, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00F08041, 0x00, 0x04002100 + 0x40, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00F08041, 0x00, 0x04002100 + 0x44, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F08011, 0x00, 0x04002100 + 0x60, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F08011, 0x00, 0x04002100 + 0x64, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F08011, 0x00, 0x04002100 + 0x80, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F08011, 0x00, 0x04002100 + 0x84, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00FF8041, 0x00, 0x04002100 + 0xA0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00FF8041, 0x00, 0x04002100 + 0xA4, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00000041, 0x00, 0x04002100 + 0xC0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00000041, 0x00, 0x04002100 + 0xC4, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x8F8F8F8F, 0x00, 0x04002100 + 0xE0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x8F8F8F8F, 0x00, 0x04002100 + 0xE4, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x8F8F8F8F, 0x00, 0x04002100 + 0xE8, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x8F8F8F8F, 0x00, 0x04002100 + 0xEC, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	else {
		M_SETOP(OP_SET, 0x00F04041, 0x00, 0x04002100 + 0x20, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00F04041, 0x00, 0x04002100 + 0x24, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00F04041, 0x00, 0x04002100 + 0x40, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00F04041, 0x00, 0x04002100 + 0x44, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F04011, 0x00, 0x04002100 + 0x60, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F04011, 0x00, 0x04002100 + 0x64, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F04011, 0x00, 0x04002100 + 0x80, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x02F04011, 0x00, 0x04002100 + 0x84, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00FF4041, 0x00, 0x04002100 + 0xA0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00FF4041, 0x00, 0x04002100 + 0xA4, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00000041, 0x00, 0x04002100 + 0xC0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x00000041, 0x00, 0x04002100 + 0xC4, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x4F4F4F4F, 0x00, 0x04002100 + 0xE0, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x4F4F4F4F, 0x00, 0x04002100 + 0xE4, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x4F4F4F4F, 0x00, 0x04002100 + 0xE8, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_SET, 0x4F4F4F4F, 0x00, 0x04002100 + 0xEC, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}

	if (((ubDiePerChannel >= 16) && ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz))) ||
	        ((ubDiePerChannel >= 8) && (gubFlashClock == FLH_CLK_266MHz)) ||
	        (TLC && ((ubDiePerChannel >= 8) && ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz))))) {
		M_SETOP(OP_OR1, 0x04009400 + (FCTLL_DYNAMIC_ODT << 2), DYNAMIC_ODT_EN, 0x04009400 + (FCTLL_DYNAMIC_ODT << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04000000 + (SYSL_PAD_FDQS0_CFG << 2), (SET_ODT_VALUE(SET_ODT_100ohm)), 0x04000000 + (SYSL_PAD_FDQS0_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04000000 + (SYSL_PAD_FDAT0_CFG << 2), (SET_ODT_VALUE(SET_ODT_100ohm)), 0x04000000 + (SYSL_PAD_FDAT0_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04000000 + (SYSL_PAD_FDQS1_CFG << 2), (SET_ODT_VALUE(SET_ODT_100ohm)), 0x04000000 + (SYSL_PAD_FDQS1_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04000000 + (SYSL_PAD_FDAT1_CFG << 2), (SET_ODT_VALUE(SET_ODT_100ohm)), 0x04000000 + (SYSL_PAD_FDAT1_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	else if (ubDiePerChannel >= 32) {
		FALLL[FCTLL_DYNAMIC_ODT] |= DYNAMIC_ODT_EN; //SET BIT0 of FCTLL_DYNAMIC_ODT 時才會開啟ODT(FlashIP自動決定什麼時候開啟 例如ToggleRead時)
		M_SETOP(OP_OR1, 0x04000000 + (SYSL_PAD_FDQS0_CFG << 2), (SET_ODT_VALUE(3)), 0x04000000 + (SYSL_PAD_FDQS0_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04000000 + (SYSL_PAD_FDAT0_CFG << 2), (SET_ODT_VALUE(3)), 0x04000000 + (SYSL_PAD_FDAT0_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04000000 + (SYSL_PAD_FDQS1_CFG << 2), (SET_ODT_VALUE(3)), 0x04000000 + (SYSL_PAD_FDQS1_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04000000 + (SYSL_PAD_FDAT1_CFG << 2), (SET_ODT_VALUE(3)), 0x04000000 + (SYSL_PAD_FDAT1_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}

	if ((gubInterface == INTERFACE_TOGGLE2) || (gubInterface == INTERFACE_NVDDR2)) {
		M_SETOP(OP_OR1, 0x04009400 + (FCTLL_IO_SET << 2), DIFFERENTIAL_IO_TYPE, 0x04009400 + (FCTLL_IO_SET << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04002000 + (SYS1L_PAD_FDAT0_CFG << 2), ((CR_FDAT0_CO )), 0x04002000 + (SYS1L_PAD_FDAT0_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_OR1, 0x04002000 + (SYS1L_PAD_FDAT1_CFG << 2), ((CR_FDAT1_CO )), 0x04002000 + (SYS1L_PAD_FDAT1_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	else {
		M_SETOP(OP_AND1, 0x04009400 + (FCTLL_IO_SET << 2), SINGLE_END_IO_TYPE, 0x04009400 + (FCTLL_IO_SET << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_AND1, 0x04002000 + (SYS1L_PAD_FDAT0_CFG << 2), (~(CR_FDAT0_CO )), 0x04002000 + (SYS1L_PAD_FDAT0_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
		M_SETOP(OP_AND1, 0x04002000 + (SYS1L_PAD_FDAT1_CFG << 2), (~(CR_FDAT1_CO )), 0x04002000 + (SYS1L_PAD_FDAT1_CFG << 2), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;
	}
	//UartString("Set feature uwOffset : \n") ;
	//UartLongHex(uwOffset) ;
	return (uwOffset / 16);
}

UWORD FlaGenerateInitialDRAM_OP_Code(U32 ulStartAddr)
{
	M_CheckNonBurnerModeToDisableFunctionReturnValid(0)
	//  M_SETOP(OP_SET, 0x8080, 0x00, 0x04009600 + (FCONL_FCE_ENB << 2), OP_4B, ulStartAddr + uwOffset);
	unsigned char ddr_type, ddr_size, ddr_bank;

	UWORD uwOffset = 0;
	ddr_type = gubDram_Type;//gubDram_Type////GPIO_Latch
	ddr_bank = gubDram_Bank ;
	ddr_size = gubDram_Size ; //gubDram_Size ;
	//     UartString("dMISCH(ECC_EN) : \n") ;
	//    UartLongHex(dMISCH(ECC_EN)) ;
	//  UartString("&dMISCH(ECC_EN) : \n") ;
	//  UartLongHex(&dMISCH(ECC_EN)) ;
	//            UartString("uwOffset : \n") ;

	M_SETOP(OP_SET,  (BIT7), 0x00, (0x04003000 + 0x30), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;  ////Disable ECC and Enable E3D  //dMISCH(ECC_EN) = CR_E3D_ENABLE;
	M_SETOP(OP_OR1, (0x04000400 + 0x14), BIT23, (0x04000400 + 0x14), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;  //DDR _PHY_CLKEN  dCLK(CLK_EN_CTRL) |= CR_DDRP_CKEN;

	///Added Test by Victor
	M_SETOP(OP_SET,  (0x00), 0x00, (0x04000000 + 0x318), OP_1B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_SET,  (0xc8), 0x00, (0x04000000 + 0x530), OP_1B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	M_SETOP(OP_WHILE_AND_NE,  (0x04000000 + 0x533), 0xFF, 0x00, OP_1B, ulStartAddr + uwOffset);
	uwOffset += 0x10;
	///


	////ddr_phy_init(ddr_type);
	if (ddr_type == SDR)
		M_SETOP(OP_SET, 0x80, 0x00, 0x04000319, OP_1B, ulStartAddr + uwOffset);
	//*((volatile unsigned char*)(0x04000319)) = 0x80;//select mode=SDR
	else
		M_SETOP(OP_SET, 0x40, 0x00, 0x04000319, OP_1B, ulStartAddr + uwOffset);
	//*((volatile unsigned char*)(0x04000319))= 0x40; //set ddr type = DDR3/MRAM
	uwOffset += 0x10;


	M_SETOP(OP_SET, 0x80c34ac8, 0x00, (0x04005000 + 0x468), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //   dDDR(0x468) = 0x80f54ac8;
	M_SETOP(OP_DELAY, 0x00, 0x00, 10, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //  idle(10);
	M_SETOP(OP_SET, 0x0ccfc000, 0x00, (0x04005000 + 0x478), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;  //  dDDR(0x478) = 0x0ccfc000;
	M_SETOP(OP_DELAY, 0x00, 0x00, 10, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //  idle(10);
	M_SETOP(OP_SET, 0x80c36ac8, 0x00, (0x04005000 + 0x468), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;  //dDDR(0x468) = 0x80f56ac8;
	M_SETOP(OP_DELAY, 0x00, 0x00, 10, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //  idle(10);
	M_SETOP(OP_SET, 0x80c3eac8, 0x00, (0x04005000 + 0x468), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;  //dDDR(0x468) = 0x80f5eac8;
	M_SETOP(OP_DELAY, 0x00, 0x00, 10, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //  idle(10);
	M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x46c), 0x00000004, 0x00000004, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;//while ( (dDDR(0x46c)&0x00000004)!=0x00000004){ //wait pll lock
	M_SETOP(OP_SET, 0xa0c3eac8, 0x00, (0x04005000 + 0x468), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;  //dDDR(0x468) = 0xa0f5eac8;   //DLL_RSTB              =CFG46[29];
	if (ddr_type == SDR) {
		M_SETOP(OP_DELAY, 0x00, 0x00, 100, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//  idle(100);
	}
	else {
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x46c), 0x00000002, 0x00000002, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x46c)&0x00000002)!=0x00000002){ //wait dll lock
	}
	M_SETOP(OP_SET, 0xe0c3eac8, 0x00, (0x04005000 + 0x468), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; // dDDR(0x468) = 0xe0f5eac8;
	M_SETOP(OP_DELAY, 0x00, 0x00, 100, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;//  idle(100);
	M_SETOP(OP_SET, 0x60c3eac8, 0x00, (0x04005000 + 0x468), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; // dDDR(0x468) = 0x60f5eac8;
	M_SETOP(OP_DELAY, 0x00, 0x00, 100, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;//  idle(100);
	M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x200), 0x00000040, 0x00000040, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;//while ( (dDDR(0x200) & 0x00000040) != 0x00000040) { //wait CR_DFIINITCOMPLETE
	M_SETOP(OP_SET, 0x000383e0, 0x00, (0x04005000 + 0x45C), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x45C) = 0x00000ff8;
	M_SETOP(OP_DELAY, 0x00, 0x00, 10, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;// 	idle(10);
	//// end ddr_phy_init
	////
	//// ddr_controller_init
	M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
	M_SETOP(OP_SET, 0x00000200, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x010) = 0x00000200; //set RESETB=1
	M_SETOP(OP_DELAY, 0x00, 0x00, 200, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;// 	idle(200);
	M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
	M_SETOP(OP_SET, 0x00000020, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x010) = 0x00000020; //set CKE=1
	if (ddr_type == MRAM) {
		M_SETOP(OP_SET, 0x05240000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
		//dDDR(0x018) = 0x05240000; // set ROW=14bit COL=5bit
		uwOffset += 0x10;
	}
	else if (ddr_type == SDR) { // ddr_size == SIZE_32MB
		if (ddr_size == SIZE_32MB)
			M_SETOP(OP_SET, 0x44910000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
		//dDDR(0x018) = 0x44910000; // set ROW=13bit COL=9bit   //{COARSE[1:0],fine}=[25:23]=3'b001
		else if (ddr_size == SIZE_16MB)
			M_SETOP(OP_SET, 0x44810000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
		//dDDR(0x018) = 0x44810000; // set ROW=12bit COL=9bit   //{COARSE[1:0],fine}=[25:23]=3'b001
		else
			M_SETOP(OP_SET, 0x44800000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
		//dDDR(0x018) = 0x44800000; // set ROW=12bit COL=8bit   //{COARSE[1:0],fine}=[25:23]=3'b001
		uwOffset += 0x10; //dDDR(0x018) = 0x44910000; // set ROW=13bit COL=9bit   //{COARSE[1:0],fine}=[25:23]=3'b001
	}
	else if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {// 8bank , 512MB
		if (ddr_bank == 1) {
			if (ddr_size == SIZE_512MB)
				M_SETOP(OP_SET, 0x45420000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x018)=0x45420000; // set ROW=16bit COL=10bit
			else if (ddr_size == SIZE_256MB)
				M_SETOP(OP_SET, 0x45320000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x018)=0x45320000; // set ROW=15bit COL=10bit
			else //(ddr_size=SIZE_128MB)
				M_SETOP(OP_SET, 0x45220000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			// dDDR(0x018)=0x45220000; // set ROW=14bit COL=10bit
		}
		else { // 8bank
			if (ddr_size == SIZE_512MB)
				M_SETOP(OP_SET, 0x05320000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x018) = 0x05320000; // set ROW=15bit COL=10bit
			else if (ddr_size == SIZE_256MB)
				M_SETOP(OP_SET, 0x05220000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x018) = 0x05220000; // set ROW=14bit COL=10bit
			else //(ddr_size=SIZE_128MB)
				M_SETOP(OP_SET, 0x05120000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x018) = 0x05120000; // set ROW=13bit COL=10bit
		}
		uwOffset += 0x10; //dDDR(0x018) = 0x05320000; // set ROW=15bit COL=10bit
	}
	M_SETOP(OP_DELAY, 0x00, 0x00, 50, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;// 	idle(50);
	M_SETOP(OP_SET, 0x01490000, 0x00, (0x04005000 + 0x028), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x028) = 0x01490000; // set BL8 16bit
	M_SETOP(OP_DELAY, 0x00, 0x00, 50, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;// 	idle(50);
	M_SETOP(OP_SET, 0x0000b144, 0x00, (0x04005000 + 0x00c), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x00c) = 0x0000b144; // R2W_DIFRK=11,TWTR_AL_DIFRK=20,TCCD_DIFRK=4
	M_SETOP(OP_DELAY, 0x00, 0x00, 50, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;// 	idle(50);
	if (ddr_type == SDR) {
		M_SETOP(OP_SET, 0x09102233, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x014) = 0x09102233;
	}
	else if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		if (ddr_bank == 1) { //4 bank
			if (ddr_size == SIZE_512MB)
				M_SETOP(OP_SET, 0x1bffff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x014) = 0x1bffff88; // // TRTP=4,TRAS=1b,TRFC=ff TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else if (ddr_size == SIZE_256MB)
				M_SETOP(OP_SET, 0x1b8cff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x014)=0x1b8cff88; // TRTP=4,TRAS=1b,TRFC=8c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else
				M_SETOP(OP_SET, 0x1b57ff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x014)=0x1b57ff88; // TRTP=4,TRAS=1b,TRFC=57 TWTR_AL=f,TWR=f,TRP=8,TRCD=8
		}
		else {
			if (ddr_size == SIZE_512MB)
				M_SETOP(OP_SET, 0x1b8bff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x014)=0x1b8cff88; // TRTP=4,TRAS=1b,TRFC=8c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else if (ddr_size == SIZE_256MB)
				M_SETOP(OP_SET, 0x1b57ff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x014)=0x1b57ff88; // TRTP=4,TRAS=1b,TRFC=57 TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else
				M_SETOP(OP_SET, 0x1b3cff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			//dDDR(0x014)=0x1b3cff88; // TRTP=4,TRAS=1b,TRFC=3c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
		}
		uwOffset += 0x10; //dDDR(0x014) = 0x1b8bff88;  TRTP=6,TRAS=15,TRFC=120 TWTR_AL=5,TWR=5,TRP=5,TRCD=5
	}
	M_SETOP(OP_DELAY, 0x00, 0x00, 50, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;// 	idle(50);
	M_SETOP(OP_SET, 0x00007080, 0x00, (0x04005000 + 0x01c), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x01c) = 0x00007080; //TFAW=40, R2W=8
	//// END ddr_controller_init
	////
	////  MRS_Setting
	if (ddr_type == SDR) {
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
		M_SETOP(OP_SET, 0x00000004, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x010) = 0x00000004; //PRA
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
		M_SETOP(OP_SET, 0x00000001, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x010) = 0x00000001; //AutoRefresh
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
		M_SETOP(OP_SET, 0x00000001, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x010) = 0x00000001; //AutoRefresh
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
		M_SETOP(OP_SET, 0x00330002, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x010) = 0x00330002; //MR0 CAS latency=3
	}
	else {
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
		if (ddr_type == DDR3_400) {
			M_SETOP(OP_SET, 0x00002002, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //dDDR(0x010) = 0x00002002; //MR2,CWL=5
		}
		else {
			M_SETOP(OP_SET, 0x00082002, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //dDDR(0x010) = 0x00082002; //MR2,CWL=6
		}
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
		M_SETOP(OP_SET, 0x00003002, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x010) = 0x00003002; //MR3
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
		M_SETOP(OP_SET, 0x00041002, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x010) = 0x00041002; //MR1
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
		if (ddr_type == DDR3_400) {
			M_SETOP(OP_SET, 0x19200002, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //dDDR(0x010) = 0x19200002; //MR0 CAS latency=6
		}
		else {
			M_SETOP(OP_SET, 0x19400002, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; // dDDR(0x010)=0x19400002; //MR0 CAS latency=8
		}
		M_SETOP(OP_WHILE_AND_NE, (0x04005000 + 0x010), 0x000003ff, 0x00000000, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees
		M_SETOP(OP_SET, 0x00000040, 0x00, (0x04005000 + 0x010), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x010) = 0x00000040; //MR0
		M_SETOP(OP_DELAY, 0x00, 0x00, 200, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10;// 	idle(200);
	}
	////End MRS_Setting
	////
	////Auto_Refresh()


	M_SETOP(OP_SET, 0x00090018, 0x00, (0x04005000 + 0x004), OP_4B, ulStartAddr + uwOffset);
	// =24*16*5ns =1920ns dDDR(0x004)=0x00090018;
	uwOffset += 0x10;
	M_SETOP(OP_SET, 0x00000001, 0x00, (0x04005000 + 0x000), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x000) = 0x00000001;
	////End Auto_Refresh()
	////
	if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		M_SETOP(OP_SET, 0x3000ffff, 0x00, (0x04005000 + 0x43c), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x43c)=0x3000ffff; //default is 0x0fc0_0000,to adjust read mode DQ and adjust write mode DQ
		M_SETOP(OP_SET, 0x00000000, 0x00, (0x04005000 + 0x438), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x438)=0x00000000; //default is 0xffff_ffff , to adjust write mode DQ
		///// Test added
		M_SETOP(OP_SET, 0x1ecfc002, 0x00, 0x04005478, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //*((volatile unsigned long*)0x04005478) = 0x1ecfc002;
		M_SETOP(OP_SET, 0x11b6db00, 0x00, 0x04005458, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //*((volatile unsigned long*)0x04005458) = 0x11b6db00;
		M_SETOP(OP_WHILE_AND_NE, 0x04005028, 0x0200, 0x0200, OP_2B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while((*((volatile unsigned short*)0x04005028) & 0x0200) != 0x0200); //check ddr idle
		M_SETOP(OP_WHILE_AND_NE, 0x04005010, 0x03ff, 0, OP_2B, ulStartAddr + uwOffset);
		uwOffset += 0x10;//while((*((volatile unsigned short*)0x04005010) & 0x03ff) != 0); //check command free
		M_SETOP(OP_SET, 0x00041002, 0x00, 0x04005010, OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; // *((volatile unsigned long*)0x04005010)=0x00041002; // 40
		//7.CLK add 100ps delay from ideal 1/4T location DDR3 only
		M_SETOP(OP_SET, 0x00000000, 0x00, (0x04005000 + 0x410), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x410)=0x00000000;
		M_SETOP(OP_SET, 0xffff0000, 0x00, (0x04005000 + 0x414), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x414)=0xffff0000;
		M_SETOP(OP_SET, 0x038e0000, 0x00, (0x04005000 + 0x418), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x418)=0x038e0000;
		M_SETOP(OP_SET, 0x040e38e0, 0x00, (0x04005000 + 0x450), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x450)=0x040e38e0;
		M_SETOP(OP_SET, 0x00ff0000, 0x00, (0x04005000 + 0x474), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x474)=0x00ff0000;


		//////////////////////////////////////////////////////////
		//
		///////Test added done.
		//
		//ScanDelayLine_DDR3
		////insert MDQS0_G, DQS1_G ,DQS0_Fine, DQS1_Fine
		M_SETOP(OP_AND1, (0x04005000 + 0x47c), 0xffffff88, (0x04005000 + 0x47c), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x47c) &= (0xffffff88);      //Clean DQS_G0 & DQS_G1
		M_SETOP(OP_OR1, (0x04005000 + 0x47c), 0x01 * gubDQS0_G, (0x04005000 + 0x47c), OP_1B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //bDDR(0x47c) |= 0x01 * (Index_Gate_DQS0);
		M_SETOP(OP_OR1, (0x04005000 + 0x47c), 0x10 * gubDQS1_G, (0x04005000 + 0x47c), OP_1B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //bDDR(0x47c) |= 0x10 * (Index_Gate_DQS1);
		////Set_DDR3_DQS_Fine
		M_SETOP(OP_AND1, (0x04005000 + 0x44C), 0xF03FFFFF, (0x04005000 + 0x44C), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x44C) &= 0xF03FFFFF;
		M_SETOP(OP_AND1, (0x04005000 + 0x42C), 0xF00F3FFF, (0x04005000 + 0x42C), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x42C) &= 0xF00F3FFF;
		if (guwDQS0_Fine > 20) {
			M_SETOP(OP_OR1, (0x04005000 + 0x42c), 0x00004000, (0x04005000 + 0x42c), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //dDDR(0x42c) |= 0x00004000;
		}
		if (guwDQS1_Fine > 20) {
			M_SETOP(OP_OR1, (0x04005000 + 0x42c), 0x00008000, (0x04005000 + 0x42c), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //dDDR(0x42c) |= 0x00004000;
		}
		M_SETOP(OP_OR1, (0x04005000 + 0x44C), gul_temp1, (0x04005000 + 0x44C), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x44c) |= ultemp;
		M_SETOP(OP_AND1, (0x04005000 + 0x42C), 0xF00FFFFF, (0x04005000 + 0x42C), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x42C) &= 0xF00FFFFF;
		M_SETOP(OP_OR1, (0x04005000 + 0x42C), gul_temp2, (0x04005000 + 0x42C), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x42C) |= ((uwtemp1<<16) & 0xFFFF0000);

		if (ddr_bank == 1) { //4 bank
			if (ddr_size == SIZE_512MB)
				M_SETOP(OP_SET, 0x45420000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			else if (ddr_size == SIZE_256MB)
				M_SETOP(OP_SET, 0x45320000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			else //(ddr_size=SIZE_128MB)
				M_SETOP(OP_SET, 0x45220000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //dDDR(0x018) = 0x45220000; // set ROW=14bit COL=10bit
		}
		else { //4 bank
			if (ddr_size == SIZE_512MB)
				M_SETOP(OP_SET, 0x05320000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			else if (ddr_size == SIZE_256MB)
				M_SETOP(OP_SET, 0x05220000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			else //(ddr_size=SIZE_128MB)
				M_SETOP(OP_SET, 0x05120000, 0x00, (0x04005000 + 0x018), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //dDDR(0x018) = 0x05120000; // set ROW=13bit COL=10bit
		}

		if (ddr_bank == 1) { //4 bank
			if (ddr_size == SIZE_512MB)
				M_SETOP(OP_SET, 0x1bffff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			else if (ddr_size == SIZE_256MB)
				M_SETOP(OP_SET, 0x1b8cff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			else //(ddr_size=SIZE_128MB)
				M_SETOP(OP_SET, 0x1b57ff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //dDDR(0x014) = 0x1b57ff88; // TRTP=4,TRAS=1b,TRFC=57 TWTR_AL=f,TWR=f,TRP=8,TRCD=8
		}
		else { //4 bank
			if (ddr_size == SIZE_512MB)
				M_SETOP(OP_SET, 0x1b8cff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			else if (ddr_size == SIZE_256MB)
				M_SETOP(OP_SET, 0x1b57ff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			else //(ddr_size=SIZE_128MB)
				M_SETOP(OP_SET, 0x1b3cff88, 0x00, (0x04005000 + 0x014), OP_4B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //dDDR(0x014) = 0x1b3cff88; // TRTP=4,TRAS=1b,TRFC=3c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
		}

	}
	if (ddr_type == SDR) {
		M_SETOP(OP_SET, 0x00004010, 0x00, (0x04005000 + 0x464), OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x464)=0x00004010; //default is 0x00002010

		M_SETOP(OP_AND1, (0x04005000 + 0x1A) , ~(0x0380), (0x04005000 + 0x1A) , OP_2B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //wDDR(0x1A) &= ~(0x0380);
		M_SETOP(OP_OR1, (0x04005000 + 0x1A), ((gub_SDR_Corse << 7) & 0x0380), (0x04005000 + 0x1A) , OP_2B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //wDDR(0x1A) |= ((gub_SDR_Corse << 7) & 0x0380);
		M_SETOP(OP_AND1, (0x04005000 + 0x7C) , 0xFF800000, (0x04005000 + 0x7C) , OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x7C) &= 0xFF800000;
		M_SETOP(OP_OR1, (0x04005000 + 0x7C) , gul_temp3, (0x04005000 + 0x7C)  , OP_4B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //dDDR(0x7C) |= (((uwMaxSuccessFine >> 2) << 16) | ((uwMaxSuccessFine & 0x2) << 7) | (uwMaxSuccessFine & 0x1));

		M_SETOP(OP_AND1, (0x04005000 + 0x01A) , 0x80, (0x04005000 + 0x01A) , OP_1B, ulStartAddr + uwOffset);
		uwOffset += 0x10; //bDDR(0x01A) &= 0x80;           /// default value 0 : 8MB
		if (ddr_size == SIZE_32MB) {
			M_SETOP(OP_OR1, (0x04005000 + 0x01A) , 0x11, (0x04005000 + 0x01A)  , OP_1B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //bDDR(0x01A) |= 0x11;
		} // set ROW=13bit COL=9bit   //{COARSE[1:0],fine}=[25:23]=3'b001
		else if (ddr_size == SIZE_16MB) {
			M_SETOP(OP_OR1, (0x04005000 + 0x01A) , 0x01, (0x04005000 + 0x01A)  , OP_1B, ulStartAddr + uwOffset);
			uwOffset += 0x10; //bDDR(0x01A) |= 0x01;
		} // set ROW=12bit COL=9bit   //{COARSE[1:0],fine}=[25:23]=3'b001

	}
	M_SETOP(OP_WHILE_AND_E, (0x04005000 + 0x028), BIT9, 0x00000000, OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10;//while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfrees


	M_SETOP(OP_SET, 0x00000000, 0x00, (0x04002000 + (SYS1L_ECC_EN << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;

	M_SETOP(OP_SET, DDR_BASE, 0x00, (0x04002000 + (SYS1L_DZ1_DSA << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;
	M_SETOP(OP_SET, Z1_Size, 0x00, (0x04002000 + (SYS1L_DZ1_DLEN << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;
	M_SETOP(OP_SET, DDR_BASE + Z1_Size, 0x00, (0x04002000 + (SYS1L_DZ2_DSA << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;
	M_SETOP(OP_SET, Z2_Size, 0x00, (0x04002000 + (SYS1L_DZ2_DLEN << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;
	M_SETOP(OP_SET, DDR_BASE + Z1_Size + Z2_Size, 0x00, (0x04002000 + (SYS1L_DZ3_DSA << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;
	M_SETOP(OP_SET, Z3_Size, 0x00, (0x04002000 + (SYS1L_DZ3_DLEN << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;
	M_SETOP(OP_SET, SZ1_DDR_BASE, 0x00, (0x04002000 + (SYS1L_SZ1_ADDRESS << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;
	M_SETOP(OP_SET, SZ2_DDR_BASE, 0x00, (0x04002000 + (SYS1L_SZ2_ADDRESS << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;
	M_SETOP(OP_SET, 0, 0x00, (0x04002000 + (SYS1L_ECC_EN << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;
	M_SETOP(OP_SET, (CR_E3D_EN | CR_Z1_ECC_EN | CR_Z2_ECC_EN | CR_Z3_ECC_EN), 0x00, (0x04002000 + (SYS1L_ECC_EN << 2)), OP_4B, ulStartAddr + uwOffset);
	uwOffset += 0x10; //dDDR(0x44c) = ultemp;

	//UartString("Set DDR uwOffset : \n") ;
	//UartLongHex(uwOffset) ;
	return (uwOffset / 16);
	/*



				// Scan Delay Line
				if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {

	///// Test added

					*((volatile unsigned long*)0x04005478) = 0x1ecfc002;
					*((volatile unsigned long*)0x04005458) = 0x11b6db00;

					while((*((volatile unsigned short*)0x04005028) & 0x0200) != 0x0200); //check ddr idle
					while((*((volatile unsigned short*)0x04005010) & 0x03ff) != 0); //check command free

					*((volatile unsigned long*)0x04005010)=00061002; //34ohm

	///////Test added done.
					ubResult = ScanDelayLine_DDR3();
					ddr_bank = Scan_bank();
				}
				if (ddr_type == SDR) {
					ubResult =ScanDelayLine_SDR();
				}

				ddr_size = Scan_size(ddr_type);             	// scan ddr size
				ddr_controller_init(ddr_type, ddr_size, ddr_bank, 1);   	// set ctrller with new ddr size
				gubDram_Size = ddr_size;
				gulDram_Size=(ddr_size<4)? pow(2,(ddr_size+2))*0x100000 : pow(2,(ddr_size+3))*0x100000;
				if (ddr_type == SDR) {				// 因為ddr_controller_init會動到ScanDelayLine_SDR裡設定0x1A的位置, 所以再重新寫一次
					wDDR(0x1A) &= ~(0x0380);
					wDDR(0x1A) |= ((gub_SDR_Corse << 7) & 0x0380);
				}


				gubDram_Type = ddr_type;
				gubDram_Bank = ddr_bank;

				if(ubResult == 0x0)
					{return RET_OK;}
				else
					{return RET_ERR;}
	}*/
}


UBYTE FlaGenerateHeader(UBYTE ubProgramAgain, U32 ulDBTAddr)
{
	M_CheckNonBurnerModeToDisableFunctionReturnValid(0);
#if BURNER
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;

	U32 ultempA;
	U32 ulAddressRegion = (FCONL[FCONL_FLH_FUNC] & RDM_REF_ROW_ADR_5_TO_0);
	UBYTE ubChannel,  ubL4kLink, ubDepth, ubFQIndex, ubBurst;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	UBYTE ubCodeVersion = 0xAA;
	UBYTE ubVersion, ubFrameBitMask;
	UBYTE ubCodeBlockIndex;
	UWORD uwOP_Num;
	UWORD uwPageIndex;
	U32 ulFreeUnitPTR, ulFreeblock;
	UWORD uwFWSecNumber;
	UWORD uwFUNCSecNumber;
	UWORD uwPageNum[8];
	UWORD uwPageIndexInSection;
	//U32 ulActualCodeSizeWithDummyBytes[8];
	volatile UWORD *puwALU = (UWORD*)(&IRAMB[ALU_BASE_0]);
	UBYTE *ubBUF_16Kshift = (UBYTE*)( BUFFER3_BASE + 0x4000);
	UBYTE *ubBUF_32Kshift = (UBYTE*)(BUFFER3_BASE + 0x8000);
	UBYTE *ubDBTBuffer = (UBYTE *)ulDBTAddr; //(UBYTE*)(&gulGRTable[0]);
	///initial
	memset(&uwPageNum, 0x00, (sizeof(unsigned short) * 8));
	//memset(&ulActualCodeSizeWithDummyBytes, 0x00, (sizeof(unsigned long) * 8));

	uwFWSecNumber = 0;
	uwFUNCSecNumber = 0;

	for (ultempA = 0; ultempA < 8; ultempA++) { //// 4 sections ahead
		if (gulSectionInformation[0][ultempA] != 0) {
			if (ultempA < 4) {
				//					uwFWSecNumber +=1;
				uwFWSecNumber = ultempA + 1;
			}
			else {
				uwFUNCSecNumber += 1;
			}
			/////// Cal The actual Page Number  (Page number Must be in pair)
			uwPageNum[ultempA] = ((gulSectionInformation[2][ultempA]) / (gubSectorsPerPlane * 512));
			if (gulSectionInformation[2][ultempA] % (gubSectorsPerPlane * 512)) {
				if (uwPageNum[ultempA] & BIT0)
					uwPageNum[ultempA]++;
				else
					uwPageNum[ultempA] = uwPageNum[ultempA] + 2;
			}
			else {
				if (uwPageNum[ultempA] & BIT0)
					uwPageNum[ultempA]++;
			}
			///////Cal The Real memory space needed to put ICCM & DCCM
			//ulActualCodeSizeWithDummyBytes[ultempA] = (gubSectorsPerPlane * 512) * uwPageNum[ultempA];
		}
	}

	guwSysCodeBlock[0][0] = 0xFFFF;
	guwSysCodeBlock[0][1] = 0xFFFF;
	guwSysCodeBlock[1][0] = 0xFFFF;
	guwSysCodeBlock[1][1] = 0xFFFF;
	gubSysCodeSet[0] = 0xFF;
	gubSysCodeSet[1] = 0xFF;

	mDMAC_SV(1, (U32)&ubBUF_16Kshift[0], (U32)gubSectorsPerPlane * 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	UBYTE ubbadblock, ubj, ubPlaneBank, ubGetFreeBlock, ubCEIndex, ubSysBlkCounter = 0;
	U32 ulStartUnit[MAX_CHANNEL] = {0, 0};
	UBYTE ubStartBurst[MAX_CHANNEL] = {0, 0};
	UWORD uwselectedBlock[MAX_CHANNEL] = {0xFFFF, 0xFFFF};

	while (1) {
		uwselectedBlock[0] = 0xFFFF;
		uwselectedBlock[1] = 0xFFFF;
		if (gubSystemArea_RemappingRule == Rule_UnitPlaneA) { //依照舊rule只挑planeA的systemUnit
			ubBurst = 0;
			for (ulFreeUnitPTR = ulStartUnit[0]; ulFreeUnitPTR < SYSTEMAREA_DEFAULT_SCAN_RANGE; ulFreeUnitPTR++) {
				ubbadblock = 0;
				ulFreeblock = (M_Multiplier((ulFreeUnitPTR << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) ;
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (ENABLE_PLANE_CH_CE) {
						ubPlaneBank = (ubCEIndex << gubBurstsPerBankLog) + ubBurst;
					}
					else {
						ubPlaneBank = ubCEIndex;
					}
					if (gubRDTDone & DBT_Exist) {
						if (ubDBTBuffer[ulFreeblock + ubPlaneBank] & 0xF0) {
							ubbadblock |= BIT0;
						}
						//在RDT_BURNER時,因為Burner會markbad, 所以這邊的DBT有可能會跟Preformat的時候的DBT不一樣
						//導致在這邊挑block的時候無法兩個channel同時挑到DBT block或者是無法同時挑到Systemblock
						//這裡要防止code寫進DBT Block or System Block
						if ((((ulFreeUnitPTR << gubBurstsPerBankLog) + ubBurst) == guwDBTBlock[ubCEIndex])
						        || (((ulFreeUnitPTR << gubBurstsPerBankLog) + ubBurst) == guwSystemBlock[ubCEIndex])) {
							ubbadblock |= BIT0;
						}
					}
					else {
						if (ENABLE_SANDISK_CHECKEARLYBADBYPROG) {
							ubbadblock |= ftlGoodBlock_SandiskCheckByProgFail(ulFreeUnitPTR, ubPlaneBank);
						}
						else {
#if MicronFlashOnly
							ubbadblock |= ftlGoodBlock(ulFreeUnitPTR, ubPlaneBank, (U32)&ubBUF_16Kshift[0], 1, GOODBLK_MLC);
#else
							ubbadblock |= ftlGoodBlock(ulFreeUnitPTR, ubPlaneBank, (U32)&ubBUF_16Kshift[0], 1);
#endif
						}
					}

					if (ubbadblock != 0)
						break;
				}

				if (ubbadblock == 0) {
					//UartString("\nGBlock:");
					//UartLongHex((ulFreeUnitPTR << gubBurstsPerBankLog) + ubBurst);
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						uwselectedBlock[ubCEIndex] = (UWORD)((ulFreeUnitPTR << gubBurstsPerBankLog) + ubBurst);
					}
					ulStartUnit[0] = ulFreeUnitPTR + 1;
					break;
				}
			}
			if (ulFreeUnitPTR == SYSTEMAREA_DEFAULT_SCAN_RANGE) {
				ftlPreformatShowError(0, ulFreeUnitPTR, 0x80);
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				return TERMINAL_FAIL;
			}
		}
		else if (gubSystemArea_RemappingRule == Rule_Set) { //可以挑plane A,B
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				ubGetFreeBlock = 0;
				for (ulFreeUnitPTR = ulStartUnit[ubCEIndex]; ulFreeUnitPTR < SYSTEMAREA_DEFAULT_SCAN_RANGE; ulFreeUnitPTR++) {
					for (ubBurst = ubStartBurst[ubCEIndex]; ubBurst < gubBurstsPerBank; ubBurst++) {
						ubbadblock = 0;
						ulFreeblock = (M_Multiplier((ulFreeUnitPTR << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) ;
						if (ENABLE_PLANE_CH_CE) {
							ubPlaneBank = (ubCEIndex << gubBurstsPerBankLog) + ubBurst;
						}
						else {
							ubPlaneBank = ubCEIndex;
						}

						if (gubRDTDone & DBT_Exist) {
							if (ubDBTBuffer[ulFreeblock + ubPlaneBank] & 0xF0) {
								ubbadblock |= BIT0;
							}
							//在RDT_BURNER時,因為Burner會markbad, 所以這邊的DBT有可能會跟Preformat的時候的DBT不一樣
							//導致在這邊挑block的時候無法兩個channel同時挑到DBT block或者是無法同時挑到Systemblock
							//這裡要防止code寫進DBT Block or System Block
							if ((((ulFreeUnitPTR << gubBurstsPerBankLog) + ubBurst) == guwDBTBlock[ubCEIndex])
							        || (((ulFreeUnitPTR << gubBurstsPerBankLog) + ubBurst) == guwSystemBlock[ubCEIndex])) {
								ubbadblock |= BIT0;
							}
						}
						else {
							if (ENABLE_SANDISK_CHECKEARLYBADBYPROG) {
								ubbadblock |= ftlGoodBlock_SandiskCheckByProgFail(ulFreeUnitPTR, ubPlaneBank);
							}
							else {
#if MicronFlashOnly
								ubbadblock |= ftlGoodBlock(ulFreeUnitPTR, ubPlaneBank, (U32)&ubBUF_16Kshift[0], 1, GOODBLK_MLC);
#else
								ubbadblock |= ftlGoodBlock(ulFreeUnitPTR, ubPlaneBank, (U32)&ubBUF_16Kshift[0], 1);
#endif
							}
						}

						if (ubbadblock == 0) {
							/*UartString("\n CE:");
							Uart_Tx_DataHex(ubCEIndex);
							UartString(" Block:");
							UartLongHex((ulFreeUnitPTR << gubBurstsPerBankLog) + ubBurst);*/
							uwselectedBlock[ubCEIndex] = (UWORD)((ulFreeUnitPTR << gubBurstsPerBankLog) + ubBurst);
							if (ubBurst == gubBurstsPerBank - 1) {
								ulStartUnit[ubCEIndex] = ulFreeUnitPTR + 1;
								ubStartBurst[ubCEIndex] = 0;
							}
							else {
								ulStartUnit[ubCEIndex] = ulFreeUnitPTR;
								ubStartBurst[ubCEIndex] = ubBurst + 1;
							}
							ubGetFreeBlock = 1;//換下一個CE挑
							break;
						}
						else {
							ubStartBurst[ubCEIndex] = 0;//不用保留的時候要先清掉,不然再進去busrt內層迴圈開頭會錯
						}
					}
					if (ubGetFreeBlock) { //換下一個CE挑
						break;
					}
				}
				if (ulFreeUnitPTR == SYSTEMAREA_DEFAULT_SCAN_RANGE) {
					ftlPreformatShowError(ubCEIndex, ulFreeUnitPTR, 0x80);
					SetFailCEtoSystemInfo(ubCEIndex);
					return TERMINAL_FAIL;
				}
			}
		}

#if (MicronFlashOnly && (MICRON_NEW_BOOTCODE == 0))
		if ((ubSysBlkCounter == 0) || (ubSysBlkCounter == 1)) {
			guwSysCodeBlock[ubSysBlkCounter][0] = uwselectedBlock[0];
			guwSysCodeBlock[ubSysBlkCounter][1] = uwselectedBlock[1];
		}
#else
		if ((ubSysBlkCounter == 0) || (ubSysBlkCounter == 2)) {
			guwSysCodeBlock[(ubSysBlkCounter >> 1)][0] = uwselectedBlock[0];
			guwSysCodeBlock[(ubSysBlkCounter >> 1)][1] = uwselectedBlock[1];
		}
#endif
		ubSysBlkCounter++;

#if (MicronFlashOnly && (MICRON_NEW_BOOTCODE == 0))
#if (B0KB)   //這裡在卡code block的數量
		if (ubSysBlkCounter == 2) {//挑code set 0,1去寫
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				if ((gubCENumber == 5) || (gubCENumber == 10) || (gubCENumber == 11)) {//B0KB Force to 2 plane
					if (guwSysCodeBlock[(ubSysBlkCounter - 1)][ubCEIndex] >= (4 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[(ubSysBlkCounter - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
				else {
					if (guwSysCodeBlock[(ubSysBlkCounter - 1)][ubCEIndex] >= (2 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[(ubSysBlkCounter - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
#elif (!B0KB)//此判斷給L06B //B16 define出來後需歸類跟B0KB一樣
		if (ubSysBlkCounter == 2) {//挑code set 0,1去寫
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				if (guwSysCodeBlock[(ubSysBlkCounter - 1)][ubCEIndex] >= (4 << gubBurstsPerBankLog)) {
					ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[(ubSysBlkCounter - 1)][ubCEIndex], 0x87);
					SetFailCEtoSystemInfo(ubCEIndex);
					return TERMINAL_FAIL;
				}
			}
		}
#endif
		if (ubSysBlkCounter == 2) break;
#elif Hynix  //Hynix//如果有特殊case以後再額外寫
		if (guwPagesPerUnit == 128) {
			if (ubSysBlkCounter == 3) {//挑code set 0,2去寫
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSysCodeBlock[(ubSysBlkCounter - 1) >> 1][ubCEIndex] >= (63 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[(ubSysBlkCounter - 1) >> 1][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
		else if (guwPagesPerUnit == 256) {
			if (ubSysBlkCounter == 3) {//挑code set 0,2去寫
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSysCodeBlock[(ubSysBlkCounter - 1) >> 1][ubCEIndex] >= (32 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[(ubSysBlkCounter - 1) >> 1][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
		if (ubSysBlkCounter == 3) break;
#else  //for Toshiba
		if (gubBurstsPerBank == 2) {
			if (ubSysBlkCounter == 3) {//挑code set 0,2去寫
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSysCodeBlock[(ubSysBlkCounter - 1) >> 1][ubCEIndex] >= (32 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[(ubSysBlkCounter - 1) >> 1][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
		else if (gubBurstsPerBank == 4) {
			if (ubSysBlkCounter == 3) {//挑code set 0,2去寫
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSysCodeBlock[(ubSysBlkCounter - 1) >> 1][ubCEIndex] >= (16 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[(ubSysBlkCounter - 1) >> 1][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
		if (ubSysBlkCounter == 3) break;
#endif
	}

	if (gubRDTDone & DBT_Exist) {//要有DBT才有辦法檢查set內block和setNumber

		if (gubSystemArea_RemappingRule == Rule_Set) { //可以挑plane A,B
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				ubDBTBuffer[(guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog) * gubPlanesPerPage * gubDieNumber + ubCEIndex * gubBurstsPerBank + (guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask)] = 0x1f;
				ubDBTBuffer[(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog) * gubPlanesPerPage * gubDieNumber + ubCEIndex * gubBurstsPerBank + (guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask)] = 0x1f;
			}
		}

		if (gubPlanesPerBurst > 1) {
			//check CodeBlk 挑得是否成對
			UWORD uwOtherBlockInSet;
			for (ubj = 0; ubj < 2; ubj++) { //check 2組codeset
				uwOtherBlockInSet = ftlGetAnotherSystemBlockInSet(Mode_ByDBTinRAM, ((U32)&ubDBTBuffer[0]), 0, guwSysCodeBlock[ubj][0]);
				if (guwSysCodeBlock[ubj][1] != uwOtherBlockInSet) {
					ftlPreformatShowError(SystemArea_Error_CodeSet, guwSysCodeBlock[ubj][1], 0x86);
					return TERMINAL_FAIL;
				}
			}
		}
		//不論1ch,2ch都要取得2個codeset
		for (ubj = 0; ubj < 2; ubj++) {
			gubSysCodeSet[ubj] = ftlGetSystemSetNumber(Mode_ByDBTinRAM, ((U32)&ubDBTBuffer[0]), 0, guwSysCodeBlock[ubj][0]);
			if (gubSysCodeSet[ubj] == 0xFF) {
				ftlPreformatShowError(SystemArea_Error_CodeSet, gubSysCodeSet[ubj], 0x84);
				return TERMINAL_FAIL;
			}
		}

		if (gubSystemArea_RemappingRule == Rule_Set) { //可以挑plane A,B
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				ubDBTBuffer[(guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog) * gubPlanesPerPage * gubDieNumber + ubCEIndex * gubBurstsPerBank + (guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask)] = 0x0;
				ubDBTBuffer[(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog) * gubPlanesPerPage * gubDieNumber + ubCEIndex * gubBurstsPerBank + (guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask)] = 0x0;
			}
		}

	}


	UartString("\n");
#if Hynix
	FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
	// Start to program and read verify
#if !ENABLE_TLC_PROGRAM_CODEBLOCK2
	FCONL[FCONL_FLH_FUNC] |= RDM_REF_ROW_ADR_5_TO_0;
#endif
	// To turn on micron randomizer when program header and code block.
	flaMicronRandomizer(1);

	for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
		//UartString("	   start channel:");
		//UartWordHex(ubChannel);
		//UartString("	   *\n");
		for (ubVersion = 0; ubVersion < 1; ubVersion++) {
			uwOP_Num = 0;

			//			mDMAC_SV_4B(DBUF_BASE, 32768, 0, DISABLE);
			mDMAC_SV(1, (U32)&BUFB_BASE[0], 512 * 8, 0, 0, 0, DMA_EN_SETCQ, 1);

			BUFB_BASE[0] = 'I';
			BUFB_BASE[1] = 'D';
			BUFB_BASE[ HEADERB_FW_REVISION_ID] = ubCodeVersion + 0x11 * ubVersion;
			BUFB_BASE[ HEADERB_LDPC_MODE] = gubLDPC_Mode_Test;

			BUFW_BASE[ HEADERW_CH0_CODE0_Block] = guwSysCodeBlock[0][0];
			BUFW_BASE[ HEADERW_CH0_CODE1_Block] = guwSysCodeBlock[1][0];
			BUFW_BASE[ HEADERW_CH1_CODE0_Block] = guwSysCodeBlock[0][1];
			BUFW_BASE[ HEADERW_CH1_CODE1_Block] = guwSysCodeBlock[1][1];

			BUFW_BASE[ HEADERW_PAGE_PER_BLOCK] = guwPagesPerUnit;
			BUFB_BASE[ HEADERB_PAGE_PER_BLOCK_LOG] = gubPagesPerUnitLog;
			BUFB_BASE[ HEADERB_SECTOR_PER_PAGE_LOG] = gubSectorsPerPlaneLog;

			//BUFB_BASE[ HEADERB_LOAD_FUNCTION_ENABLE] = BIT0;

			//Column Addres Gen Setting
			ubFrameBitMask = (1 << (gubSectorsPerPlaneLog - 3)) - 1;
			FCONL[FCONL_ADR_GEN] &= (~((U32)0x1f << 12));
			FCONL[FCONL_ADR_GEN] |= (ubFrameBitMask << 12);
			// Set FCON Address Gen Rule
			FCONL[FCONL_ADR_GEN] &= (~((U32)(0x3F | FTA_EQUAL_FSA))); //Catch mask setting
			FCONL[FCONL_ADR_GEN] |= COL_ADR_AUTO_GEN | FRAME_PTR_UPDATE_EN;

			for (uwPageIndex = 0; uwPageIndex < 8; uwPageIndex ++) {
				if (ENABLE_iFSA) {
					if (uwPageIndex) {
						BUFW_BASE[ HEADERW_ALU_RULE_W0 + uwPageIndex] = puwALU[uwPageIndex] - 0x2000 + (0x400 * gubBurstsPerBankLog);
					}
					else {
						BUFW_BASE[ HEADERW_ALU_RULE_W0 + uwPageIndex] = puwALU[uwPageIndex] - 0x2000;
					}
				}
				else {
					if (uwPageIndex) {
						BUFW_BASE[ HEADERW_ALU_RULE_W0 + uwPageIndex] = puwALU[uwPageIndex] + (0x400 * gubBurstsPerBankLog);
					}
					else {
						BUFW_BASE[ HEADERW_ALU_RULE_W0 + uwPageIndex] = puwALU[uwPageIndex];
					}
				}
			}

#if (MicronFlashOnly || YMTC)
#if ENABLE_TLC_PROGRAM_CODEBLOCK2
			BUFW_BASE[ HEADERW_ALU_RULE_W0] = 0x1508;
#else
			BUFW_BASE[ HEADERW_ALU_RULE_W0] = 0x1507;

#endif

#endif
			BUFL_BASE[ HEADERL_ADR_GEN] = FCONL[FCONL_ADR_GEN] & 0xFFFFFFC0;
			BUFL_BASE[ HEADERL_ADR_GEN_STOP] = FCONL[FCONL_ADR_GEN_STOP];
			BUFL_BASE[ HEADERL_FCON_FLH_FUNC] = FCONL[FCONL_FLH_FUNC];
			BUFL_BASE[ HEADERL_FCTL_CHNL_SET] = FALLL[FCTLL_CHNL_SET];

			// ========================================================================================================================================================


			//==============================================================================================================================================================================================
			if (Enable_Skip_CRC_Check) {
				BUFB_BASE[ HEADERB_LOAD_CODE_CRC32_CHK] = CODE_CRC32_SKIP;
			}
			else {
				BUFB_BASE[ HEADERB_LOAD_CODE_CRC32_CHK] = 0xFF;
			}

			BUFB_BASE[ HEADERB_LOAD_CODE_FRAME_PER_PAGE] = gub4kEntrysPerPlane;


			//			DBUF_4Byte[ HEADERL_ICCM_CRC] = mDMAC_CRC(DBUF_BASE +  (ubL4KNum)*0x1000, ulCodeSizeICCM, CODE_CRC32_SEED, DMA_CRC_DCCM);
			//			DBUF_4Byte[ HEADERL_DCCM_CRC] = mDMAC_CRC(DBUF_BASE + (ubL4KNum)*0x1000 + ulCodeSizeICCM, ulCodeSizeDCCM, CODE_CRC32_SEED, DMA_CRC_DCCM);
			////			DBUF_4Byte[ HEADERL_ICODE_CRC] = DBUF_4Byte[ HEADERL_ICCM_CRC];



			BUFB_BASE[HEADERB_FW_START_SECTION] = 0;
			BUFB_BASE[HEADERB_FW_SECTION_NUM] = uwFWSecNumber; // Done in previous section

			BUFW_BASE[HEADERW_S0_PAGES] = uwPageNum[0];
			BUFW_BASE[HEADERW_S1_PAGES] = uwPageNum[1];
			BUFW_BASE[HEADERW_S2_PAGES] = uwPageNum[2];
			BUFW_BASE[HEADERW_S3_PAGES] = uwPageNum[3];

			BUFW_BASE[HEADERW_S0_START_PAGE] = 1;

			BUFW_BASE[HEADERW_S1_START_PAGE] = uwPageNum[0]  + 1;

			BUFW_BASE[HEADERW_S2_START_PAGE] = uwPageNum[0] + uwPageNum[1]  + 1;

			BUFW_BASE[HEADERW_S3_START_PAGE] = uwPageNum[0] + uwPageNum[1]  + uwPageNum[2] + 1;


			if (Enable_Only_Header == 0) {
				BUFL_BASE[HEADERL_S0_DESTINATION] = (gulSectionInformation[1][0]);
#if YMTC_JGS
#if ENABLE_TLC_PROGRAM_CODEBLOCK && (!ENABLE_TLC_SPI_BURNER)
				BUFL_BASE[HEADERL_S1_DESTINATION] = (gulSectionInformation[1][1]);
#else
				BUFL_BASE[HEADERL_S1_DESTINATION] = 0x2c002000;
#endif
#elif UFS
				BUFL_BASE[HEADERL_S1_DESTINATION] = 0x2c002000;

#else
				BUFL_BASE[HEADERL_S1_DESTINATION] = (gulSectionInformation[1][1]);
#endif
				BUFL_BASE[HEADERL_S2_DESTINATION] = gulSectionInformation[1][2];
				BUFL_BASE[HEADERL_S3_DESTINATION] = gulSectionInformation[1][3];
			}


			BUFL_BASE[ HEADERL_S0_SIZE] = gulSectionInformation[2][0];   // fill in the real size
			BUFL_BASE[ HEADERL_S1_SIZE] = gulSectionInformation[2][1];
			BUFL_BASE[ HEADERL_S2_SIZE] = gulSectionInformation[2][2];
			BUFL_BASE[ HEADERL_S3_SIZE] = gulSectionInformation[2][3];

			if ((Enable_Function_Code) && (gulSectionInformation[0][4] != 0)) {
				BUFB_BASE[HEADERB_FUNCTION_START_SECTION] = 4;
				BUFB_BASE[HEADERB_FUNCTION_SECTION_NUM] = uwFUNCSecNumber;

				BUFW_BASE[HEADERW_S4_PAGES] = uwPageNum[4];
				BUFW_BASE[HEADERW_S4_START_PAGE] = uwPageNum[0] + uwPageNum[1] + uwPageNum[2] + uwPageNum[3] + 1; ///0:ICCM 1:DCCM 2:ICODE 3:DBUF
				BUFL_BASE[HEADERL_S4_DESTINATION] = gulSectionInformation[1][4];
				BUFL_BASE[HEADERL_S4_SIZE] = gulSectionInformation[2][4];

				BUFW_BASE[HEADERW_S5_PAGES] = uwPageNum[5];
				BUFW_BASE[HEADERW_S5_START_PAGE] = uwPageNum[0] + uwPageNum[1] + uwPageNum[2] + uwPageNum[3] + uwPageNum[4] + 1; ///0:ICCM 1:DCCM 2:ICODE 3:DBUF
				BUFL_BASE[HEADERL_S5_DESTINATION] = gulSectionInformation[1][5];
				BUFL_BASE[HEADERL_S5_SIZE] = gulSectionInformation[2][5];

				BUFB_BASE[ HEADERB_LOAD_FUNCTION_ENABLE] = ENABLE;
			}
			else {
				BUFB_BASE[ HEADERB_LOAD_FUNCTION_ENABLE] = DISABLE;
			}

			guwCodeSectionStart[0] = BUFW_BASE[HEADERW_S0_START_PAGE];
			guwCodeSectionStart[1] = BUFW_BASE[HEADERW_S1_START_PAGE];
			guwCodeSectionStart[2] = BUFW_BASE[HEADERW_S2_START_PAGE];
			guwCodeSectionStart[3] = BUFW_BASE[HEADERW_S3_START_PAGE];
			guwCodeSectionStart[4] = BUFW_BASE[HEADERW_S4_START_PAGE];
			guwCodeSectionStart[5] = BUFW_BASE[HEADERW_S5_START_PAGE];

#if YMTC_JGS
#if ENABLE_TLC_PROGRAM_CODEBLOCK
			BUFB_BASE[HEADERB_DBUF_TO_DCCM_ENABLE1] = DISABLE;
#else
			BUFB_BASE[ HEADERB_DBUF_TO_DCCM_ENABLE1] = ENABLE;
#endif
#elif UFS
			BUFB_BASE[HEADERB_DBUF_TO_DCCM_ENABLE1] = ENABLE;

#else
			BUFB_BASE[ HEADERB_DBUF_TO_DCCM_ENABLE1] = DISABLE;
#endif

#if ENABLE_TLC_PROGRAM_CODEBLOCK && (!ENABLE_TLC_PROGRAM_CODEBLOCK2)
			BUFB_BASE[HEADERB_DA_CH0] = 1;
			BUFB_BASE[HEADERB_DA_CH1] = 1;
			BUFB_BASE[HEADERB_SLC_COMMAND] = 0xDA;
#endif


			BUFL_BASE[HEADERL_SOURCE_ADDR1] = (0x2c002000);
			BUFL_BASE[ HEADERL_COPY_LEN1] = 0x8000;
			BUFB_BASE[HEADERB_DBUF_TO_DCCM_ENABLE2] = DISABLE;
			BUFL_BASE[ HEADERL_SOURCE_ADDR2] = (0x2c000000);
			BUFL_BASE[ HEADERL_COPY_LEN2] = 0x40 << 9;


			if ((gubFlashID[0] == 0x98) || (gubFlashID[0] == 0x45)) {
				if (guoFlashUseType.B.bt4LC) { //因為burner沒有用A2也沒有用fast page...之後修
					for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
						BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex * 2;
					}
				}
				else if (guoFlashUseType.B.btPseudoPlaneEn == TRUE) {
					for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
						BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = (uwPageIndex / 2) + (BIT8 * (uwPageIndex & BIT0));
					}
				}
				else {
					for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
						BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
					}
				}
			}
#if MicronFlashOnly
			else if ((gubFlashID[0] == 0x2C) || (gubFlashID[0] == 0x89) || (gubFlashID[0] == 0xB5)) {
				if (guoFlashUseType.B.bt8LC) {
					// B85T. 0,1,2,3,4,5,8,9,14,15,20,21,26,27......
					/*
					for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
						if ( uwPageIndex < 6) {
							BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
						}
						else {
							BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = ((uwPageIndex >> 1) - 3) * 6 + 8 + (uwPageIndex & BIT0);
						}
					}
					*/
					// B0KB SLC mode
					for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
						BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
					}
				}
				else {
					if (gubFastPageType == 0x4B) {
						//L85A, L85C, L84D. 0,1,2,3,4,5,8,9,12,13,16,17
						for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
							if (uwPageIndex > 5) {
								BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = (uwPageIndex << 1) - 4 - (uwPageIndex & BIT0);
							}
							else {
								BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
							}
						}
					}
					else if (gubFastPageType == 0x4C) {
						// L06B.
						for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {				// L06B, fastpage support 512 pages.
							if (uwPageIndex > 63) {
								BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = (uwPageIndex << 1) - 63;
							}
							else {
								BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
							}
						}
					}
					else {
						// L95B. 0,1,2,3,4,5,7,8,10,11,14,15
						for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
							if ( uwPageIndex < 6) {
								BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
							}
							else if ( uwPageIndex < 8) {
								BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex + 1;
							}
							else {
								BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = ((uwPageIndex >> 1) << 1) - 6 + uwPageIndex;
							}
						}
					}
				}
			}
#endif
#if Hynix
			else if (gubFlashID[0] == 0xAD) {
				// F16 E-Die. 0, 1, 3, 5, 7, 9......
				if (gubFastPageType == 0x21) {
					for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
						if (uwPageIndex > 1) {
							BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = (uwPageIndex << 1) - 1;
						}
						else {
							BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
						}
					}
				}
				// F20 B-Die
				else if (gubFastPageType == 0x4A) {
					for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
						if (uwPageIndex > 1) {
							BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = (uwPageIndex << 1) - 2 - (uwPageIndex & BIT0);
						}
						else {
							BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
						}
					}
				}
				else if (guoFlashUseType.B.bt8LC) {
					for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
						BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
					}
				}
			}
#endif
#if YMTC
			else if (gubFlashID[0] == 0x9B) {
				for (uwPageIndex = 0; uwPageIndex < HEADER_MAX_FAST_PG; uwPageIndex ++) {
#if ENABLE_TLC_PROGRAM_CODEBLOCK2
					BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex * 3;
#else
					BUFW_BASE[ HEADERW_FAST_PAGE(uwPageIndex)] = uwPageIndex;
#endif
				}
			}
#endif

#if YMTC
			if (guoFlashDefaultType.B.btYMTC) {
				// to let boot-code does not use A2 command to load code block
				BUFB_BASE[ HEADERB_A2_SUPPORT] = 0;
			}
			else
#elif Hynix
			if (guoFlashDefaultType.B.btHynix) {
				// to let boot-code does not use A2 command to load code block
				BUFB_BASE[ HEADERB_A2_SUPPORT] = 0;
			}
			else
#endif
			{
				if (gubFlashID[0] == 0xb5) {
					BUFB_BASE[ HEADERB_A2_SUPPORT] = 0;
				}
				else {
					BUFB_BASE[ HEADERB_A2_SUPPORT] = guoFlashUseType.B.btA2Cmd;
				}
			}

#if UFS
			UBYTE ubUFSDaultMode = (gubUFSDefaultMode_CE & UFS_Sample_Default_Mode_Mask);
			UBYTE ubForceSetFeature = 0;
			if ((ubUFSDaultMode != ((gubUFSDefaultMode_CE & UFS_Sample_Exist_Mask) >> 4)) && (ubUFSDaultMode != 0)) { //default nequl all toggle/legacy
				gubInterface = INTERFACE_TOGGLE1;
				ubForceSetFeature = 1;
			}

			if (gubHeaderSetFeature || ubForceSetFeature)
#else
			if (gubHeaderSetFeature)
#endif
			{
				// Pre OP Code Set Feature
				BUFB_BASE[ HEADERB_INTERFACE_MODE] = gubInterface;
				BUFL_BASE[ HEADERL_PRE_OP_START_ADDRESS] = AON_BASE + HEADER_OP_START;
				BUFW_BASE[ HEADERW_PRE_OP_NUM] = FlaGenerateSetFeatureOP_Code(BUFFER3_BASE + HEADER_OP_START);
				uwOP_Num += BUFW_BASE[ HEADERW_PRE_OP_NUM];
				BUFB_BASE[ HEADERB_FLASH_CLOCK] = gubFlashClock;
				BUFB_BASE[ HEADERB_FEATURE_GCMD] = 0xEE;
				if (gubInterface == INTERFACE_NVDDR2) {
					BUFB_BASE[ HEADERB_FEATURE_ADR] = gubFeature1_ADR;
					BUFB_BASE[ HEADERB_FEATURE_B0] = gubFeature1_B0;
				}
				else if ((gubInterface == INTERFACE_TOGGLE2) && (gubToggleMode == 0)) {
					BUFB_BASE[ HEADERB_FEATURE_ADR] = gubFeature1_ADR;
					BUFB_BASE[ HEADERB_FEATURE_B0] = gubFeature1_B0;
				}
				else {
					BUFB_BASE[ HEADERB_FEATURE_ADR] = gubFeature_ADR;
					BUFB_BASE[ HEADERB_FEATURE_B0] = gubFeature_B0;
				}
				BUFB_BASE[ HEADERB_FEATURE_B1] = 0x00;
				BUFB_BASE[ HEADERB_FEATURE_B2] = 0x00;
				BUFB_BASE[ HEADERB_FEATURE_B3] = 0x00;
				BUFL_BASE[ HEADERL_FEATURE_DELAY] = 0;
				// yk說第一次遇到RDY不接是micron......
				// Rice:70 E0 00在hynix會影響到後面get feature的值
				if ((gubFlashID[0] == 0x2C) || (gubFlashID[0] == 0x89)) {

					BUFB_BASE[ HEADERB_FEATURE_POLLING_WAY] = POLLING_70;
				}
				else {
					BUFB_BASE[ HEADERB_FEATURE_POLLING_WAY] = POLLING_RDY;
				}
			}
			else {
				BUFW_BASE[ HEADERW_PRE_OP_NUM] = 0;
				BUFB_BASE[ HEADERB_FEATURE_GCMD] = NOT_CHECK_FEATURE;
			}


			// Initial DDR After Load ICCM and DCCM

			if (Initial_DDR) {
				if (0) { //Enable_InitialDDR_JustBeforeLoad
					BUFB_BASE[HEADERB_INITIAL_DDR_JUSTBEFORE_LOAD] = ENABLE; //In LoadCode function, check initial DRAM or not per section before loading code
					for (ultempA = 0; ultempA < 4; ultempA++) {
						if ((gulSectionInformation[1][ultempA]) >= 0x60000000) {
							BUFB_BASE[HEADERB_NEED_INITIAL_DDR_SECTION] = (UBYTE)ultempA; //The section that need DDR initial
						}
					}

					if (0) { //Enable_InitialDDR_JustBeforeLoad
						BUFB_BASE[HEADERB_DEFAULT_DRAM_INIT2_ENABLE] = ENABLE;
						// Default Function
						BUFL_BASE[HEADERL_DRAM_DELAYLINE] = ((U32*)(0x400543C))[0];
#ifdef FOR_BOOT_IC
						//For DDR
						BUFB_BASE[HEADERB_DQS0_G] = 0x01;
						BUFB_BASE[HEADERB_DQS1_G] = 0x01;
						BUFW_BASE[HEADERW_DQS0_FINE] = 0x000D;
						BUFW_BASE[HEADERW_DQS1_FINE] = 0x000D;
						BUFB_BASE[HEADERB_DRAM_TYPE] = SDR;
						BUFB_BASE[HEADERB_DRAM_SIZE] = SIZE_32MB;

						//						DBUF_Byte[HEADERB_DQS0_G] = gubDQS0_G;
						//						DBUF_Byte[HEADERB_DQS1_G] = gubDQS1_G;
						//						DBUF_2Byte[HEADERW_DQS0_FINE] = guwDQS0_Fine;
						//						DBUF_2Byte[HEADERW_DQS1_FINE] = guwDQS1_Fine;
						//					DBUF_Byte[HEADERB_DRAM_TYPE] = gubGPIO_DRAM_Type;
						//					DBUF_Byte[HEADERB_DRAM_SIZE] = gubDram_Size;

						//For SDR
						BUFB_BASE[HEADERB_SDR_SCAN_CORASE] = 0x02;
						BUFW_BASE[HEADERW_SDR_SCAN_FINE] = 0x010A;

						//					DBUF_Byte[HEADERB_SDR_SCAN_CORASE] = gubSDR_Corase;
						//					DBUF_2Byte[HEADERB_SDR_SCAN_FINE] = guwSDR_Fine;
#else
						BUFB_BASE[HEADERB_DRAM_TYPE] = DDR3_400;
						BUFB_BASE[HEADERB_DRAM_SIZE] = SIZE_512MB;
#endif

					}
					else { // OP code to init
						//BUFL_BASE[ HEADERL_DDR_OP_START_ADDRESS] = AON_BASE + HEADER_OP_START + ((U32)(uwOP_Num * 0x10));
						//BUFW_BASE[ HEADERW_DDR_OP_NUM] = FlaGenerateInitialDRAM_OP_Code(DBUF_BASE + HEADER_OP_START + ((ULONG)(uwOP_Num * 0x10)), gubDQS0_G, gubDQS1_G, guwDQS0_Fine, guwDQS1_Fine, gubSDR_Corase, guwSDR_Fine);
					}
				}
				else if (Enable_Default_DDR_Init) {
					BUFB_BASE[HEADERB_DEFAULT_DRAM_INIT_ENABLE] = ENABLE; //Initial DRAM before entering LoadCode Function
					// Default Function
					BUFL_BASE[HEADERL_DRAM_DELAYLINE] =  ((U32*)(0x400543C))[0];
					//#ifdef FOR_BOOT_IC
					//For DDR


					BUFB_BASE[HEADERB_DQS0_G] = gubDQS0_G;
					BUFB_BASE[HEADERB_DQS1_G] = gubDQS1_G;
					BUFW_BASE[HEADERW_DQS0_FINE] = guwDQS0_Fine;
					BUFW_BASE[HEADERW_DQS1_FINE] = guwDQS1_Fine;
					BUFB_BASE[HEADERB_DRAM_TYPE] = gubDram_Type;
					BUFB_BASE[HEADERB_DRAM_SIZE] = gubDram_Size;
					BUFB_BASE[HEADERB_DDR_Bank] = gubDram_Bank;
					//For SDR
					//					BUFB_BASE[HEADERB_SDR_SCAN_CORASE] = 0x02;
					//					BUFW_BASE[HEADERW_SDR_SCAN_FINE] = 0x010A;

					BUFB_BASE[HEADERB_SDR_SCAN_CORASE] = gub_SDR_Corse;
					BUFW_BASE[HEADERW_SDR_SCAN_FINE] = guw_SDR_Fine;
					//#else
					//					BUFB_BASE[HEADERB_DRAM_TYPE] = DDR3_400;
					//					BUFB_BASE[HEADERB_DRAM_SIZE] = SIZE_512MB;
					//#endif
				}
				else {
					// Post OP Code
					BUFL_BASE[ HEADERL_POST_OP_START_ADDRESS] = AON_BASE + HEADER_OP_START + ((U32)(uwOP_Num * 0x10));
					BUFW_BASE[ HEADERW_POST_OP_NUM] =  FlaGenerateInitialDRAM_OP_Code(BUFFER3_BASE + HEADER_OP_START + ((U32)(uwOP_Num * 0x10)));


				}
				if (Enable_DRAM_ECC) {
					BUFB_BASE[HEADERB_ECC_ENABLE] = ((CR_Z1_ECC_EN | CR_Z2_ECC_EN | CR_Z3_ECC_EN | CR_E3D_EN) >> 8);
					// Z1, Z2, Z3 Setting......
#ifdef FOR_BOOT_IC
					U32 ulDRAM_Size[7] = {0x200000, 0x800000, 0x1000000, 0x2000000, 0x80000000, 0x10000000, 0x20000000};
					BUFL_BASE[HEADERL_Z1_BASE] = DDR_BASE;
					BUFL_BASE[HEADERL_Z2_BASE] = DDR_BASE + Z1_size;
					BUFL_BASE[HEADERL_Z3_BASE] = DDR_BASE + Z1_size + (ulDRAM_Size[gubDram_Size] - Z1_size) / 2;
					BUFL_BASE[HEADERL_Z1_SIZE] = Z1_size;
					BUFL_BASE[HEADERL_Z2_SIZE] = (ulDRAM_Size[gubDram_Size] - Z1_size) / 2;
					BUFL_BASE[HEADERL_Z3_SIZE] = ulDRAM_Size[gubDram_Size] - Z1_size - BUFL_BASE[HEADERL_Z2_SIZE];
					BUFL_BASE[HEADERL_Z1_LIMIT] = Z1_size / 2;
					BUFL_BASE[HEADERL_Z2_LIMIT] = BUFL_BASE[HEADERL_Z2_BASE] + (BUFL_BASE[HEADERL_Z2_SIZE] / 36 * 32) - DRAM_BASE;
					BUFL_BASE[HEADERL_Z3_LIMIT] = BUFL_BASE[HEADERL_Z3_BASE] + (BUFL_BASE[HEADERL_Z3_SIZE] / 520 * 512) - DRAM_BASE;
#else //// For FW usgae

					BUFL_BASE[HEADERL_Z1_BASE] = DDR_BASE;
					BUFL_BASE[HEADERL_Z2_BASE] = DDR_BASE + Z1_Size;
					BUFL_BASE[HEADERL_Z3_BASE] = DDR_BASE + Z1_Size + Z2_Size;
					BUFL_BASE[HEADERL_Z1_SIZE] = Z1_Size;
					BUFL_BASE[HEADERL_Z2_SIZE] = Z2_Size;
					BUFL_BASE[HEADERL_Z3_SIZE] = Z3_Size;
					BUFL_BASE[HEADERL_Z1_LIMIT] = Z1_size / 2;
					BUFL_BASE[HEADERL_Z2_LIMIT] = BUFL_BASE[HEADERL_Z2_BASE] + (BUFL_BASE[HEADERL_Z2_SIZE] / 36 * 32) - DDR_BASE;
					BUFL_BASE[HEADERL_Z3_LIMIT] = BUFL_BASE[HEADERL_Z3_BASE] + (BUFL_BASE[HEADERL_Z3_SIZE] / 520 * 512) - DDR_BASE;
#endif

				}
				else {
					BUFB_BASE[HEADERB_ECC_ENABLE] = DISABLE;
				}
			}
			else {
				//BUFB_BASE[HEADERB_DEFAULT_DRAM_INIT_ENABLE] = ENABLE; //Initial DRAM before entering LoadCode Function
				BUFW_BASE[ HEADERW_POST_OP_NUM] = 0;
			}
			//////////////////// DDR done, setFeature .Beaware that FlaGenerateSetFeatureOP_Code() has a condition dependent on DBUF_Byte[HEADERB_DRAM_TYPE];


			// Change Voltage ??
			BUFB_BASE[ HEADERB_ENABLE_SET_VOLTAGE] = DISABLE;


			// Change PLL Divider??
			BUFB_BASE[ HEADERB_ENABLE_SET_PLL0] = DISABLE;
			if (gubHeaderSetFeature) {
				BUFB_BASE[ HEADERB_ENABLE_SET_PLL1] = ENABLE;
				switch (gubFlashClock) {

					case FLH_CLK_50MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0x28010C6C; //DIVS=1, DIVM=40   //PLL1 DIVN1,2,3 = 12, 6, 12 (no FW-routine)
						break;

					case FLH_CLK_83MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0x64030646; //DIVS=3, DIVM=100   //PLL1 DIVN1,2,3 = 6, 4, 6 (no FW-routine)
						break;

					case FLH_CLK_100MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0xA0030868; //DIVS=1, DIVM=40   //PLL1 DIVN1,2,3 = 12, 6, 12 (no FW-routine)
						break;

					case FLH_CLK_150MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0x96030565; //DIVS=3, DIVM=100   //PLL1 DIVN1,2,3 = 3, 4, 3 (no FW-routine)
						break;

					case FLH_CLK_166MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0x64030343; //DIVS=3, DIVM=100   //PLL1 DIVN1,2,3 = 3, 4, 3 (no FW-routine)
						break;

					case FLH_CLK_180MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0x30010564; //DIVS=1, DIVM=48   //PLL1 DIVN1,2,3 = 4, 6, 5 (no FW-routine)
						break;

					case FLH_CLK_200MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0xA0030564; //DIVS=3, DIVM=100   //PLL1 DIVN1,2,3 = 6, 4, 6 (no FW-routine)
						break;

					case FLH_CLK_266MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0xA0030563; //DIVS=3, DIVM=100   //PLL1 DIVN1,2,3 = 3, 4, 3 (no FW-routine)
						break;

					default://40MHz
						BUFL_BASE[HEADERL_PLL1_DIV] = 0x28010F6F; //DIVS=1, DIVM=40   //PLL1 DIVN1,2,3 = 15, 6, 15
						break;
				}
			}
			else {
				BUFB_BASE[ HEADERB_ENABLE_SET_PLL1] = DISABLE;
				switch (gubFlashClock) {

					case FLH_CLK_50MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0x28010C6C; //DIVS=1, DIVM=40   //PLL1 DIVN1,2,3 = 12, 6, 12 (no FW-routine)
						break;

					case FLH_CLK_83MHz:
						BUFL_BASE[HEADERL_PLL1_DIV ] = 0x64030646; //DIVS=3, DIVM=100   //PLL1 DIVN1,2,3 = 6, 4, 6 (no FW-routine)
						break;

					case FLH_CLK_100MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0xA0030868; //DIVS=1, DIVM=40   //PLL1 DIVN1,2,3 = 12, 6, 12 (no FW-routine)
						break;

					case FLH_CLK_180MHz:
						BUFL_BASE[HEADERL_PLL1_DIV] = 0x30010564; //DIVS=1, DIVM=48   //PLL1 DIVN1,2,3 = 4, 6, 5 (no FW-routine)
						break;

					default://40MHz
						BUFL_BASE[HEADERL_PLL1_DIV] = 0x28010F6F; //DIVS=1, DIVM=40   //PLL1 DIVN1,2,3 = 15, 6, 15
						break;
				}
			}
			////default
			//			DBUF_4Byte[HEADERL_PLL1_DIV] = 0x28010F6F;
			//          DBUF_Byte[HEADERB_PLL1_BAND] = 0x03;
			////accerated
			//    DBUF_4Byte[HEADERB_PLL1_DIV] = 0x64030646;
			//    DBUF_Byte[HEADERB_PLL1_BAND] = 0x03;
			//// Test toggle 200

			BUFB_BASE[HEADERB_PLL1_BAND] = 0x04;


			// Set PLL Source
			BUFL_BASE[HEADERL_CLKSW_CTRL0]  = 0x00008090;
			BUFL_BASE[HEADERL_CLKSW_CTRL1]  = 0xC0C0A0D0;
			BUFL_BASE[HEADERL_CLKSW_CTRL2]  = 0x0000C0C0;

			// Set Clock Divider
			BUFB_BASE[ HEADERB_SYSDIV_ENABLE] = DISABLE;

			BUFB_BASE[ HEADERB_FCORDIV_ENABLE] = DISABLE;

			BUFB_BASE[ HEADERB_FECCDIV_ENABLE] = DISABLE;

			//			DBUF_Byte[ HEADERB_FWRITEDIV_ENABLE] = DISABLE;
			//			DBUF_Byte[ HEADERB_FWRITEDIV_VAL] = 0xF;
			//
			//			DBUF_Byte[ HEADERB_FREADDIV_ENABLE] = DISABLE;
			//			DBUF_Byte[ HEADERB_FREADDIV_VAL] = 0xF;
			//
			//			DBUF_Byte[ HEADERB_FCH0DIV_ENABLE] = DISABLE;
			//			DBUF_Byte[ HEADERB_FCH0DIV_VAL] = 0xF;
			//
			//			DBUF_Byte[ HEADERB_FCH1DIV_ENABLE] = DISABLE;
			//			DBUF_Byte[ HEADERB_FCH1DIV_VAL] = 0xF;
			////For SIM accerate clk
			BUFB_BASE[ HEADERB_FWRITEDIV_ENABLE] = ENABLE;
			BUFB_BASE[ HEADERB_FWRITEDIV_VAL] = SYS0B[0x426];

			BUFB_BASE[ HEADERB_FREADDIV_ENABLE] = ENABLE;
			BUFB_BASE[ HEADERB_FREADDIV_VAL] = SYS0B[0x427];

			BUFB_BASE[ HEADERB_FCH0DIV_ENABLE] = ENABLE;
			BUFB_BASE[ HEADERB_FCH0DIV_VAL] = SYS0B[0x428];

			BUFB_BASE[ HEADERB_FCH1DIV_ENABLE] = ENABLE;
			BUFB_BASE[ HEADERB_FCH1DIV_VAL] = SYS0B[0x429];


			// Set MDLL Setting
#if BURNER
			//			DBUF_Byte[ HEADERB_MDLL_AUTO] = DISABLE;
			//			DBUF_4Byte[ HEADERL_SDLL_CH0] = 0x01FF01FF;
			//			DBUF_4Byte[ HEADERL_SDLL_CH1] = 0x01FF01FF;

			////For SIM accerate clk
			BUFB_BASE[ HEADERB_MDLL_AUTO] = ENABLE;
			BUFB_BASE[HEADERB_MDLL_77_143] = EXTRA_EN_77;

#else
			// for FPGA
			BUFB_BASE[ HEADERB_MDLL_AUTO] = DISABLE;
			BUFL_BASE[ HEADERL_SDLL_CH0] = FDLLL[FDLLL_SEL_VAL_0];
			BUFL_BASE[ HEADERL_SDLL_CH1] = FDLLL[FDLLL_SEL_VAL_1];
#endif

			// PAD
			BUFB_BASE[ HEADERB_ENABLE_SET_PAD] = DISABLE;

			// Second Scan Header
			BUFB_BASE[ HEADERB_ENABLE_SCAN_HEADER] = Enable_SecondHeader_Scan;
			BUFB_BASE[ HEADERB_TIMES_SCAN_HEADER] = 16;
			BUFB_BASE[ HEADERB_LIMIT_BLOCK_MASK] = 0xF;

			BUFL_BASE[(4096 - 4) / 4] = mDMAC_CRC(BUFFER3_BASE, 1 * 4096, HEADER_CRC32_SEED, 1, (DMA_CRC_LEN_8 | DMA_CRC_DCCM));


			//uwPageNumTotal = uwPageNum[0] + uwPageNum[1] + uwPageNum[2] + uwPageNum[3] + uwPageNum[4] + uwPageNum[5] + uwPageNum[6] + uwPageNum[7];

			for (ubCodeBlockIndex = 0; ubCodeBlockIndex < 2; ubCodeBlockIndex ++) {
				FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
				FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);
				UWORD uwPlane = 0;

				mDMAC_SV(1, (U32)&ubBUF_16Kshift[0], 512 * gubSectorsPerPlane, 0, 0, 0, DMA_EN_SETCQ, 1);
#if MicronFlashOnly
				mDMAC_COPY((U32)&BUFB_BASE[0], (U32)&ubBUF_16Kshift[0], 512 * gubSectorsPerPlane, 0, 0, 0, 0, 1);
#else
				mDMAC_COPY((U32)&BUFB_BASE[0], (U32)&ubBUF_16Kshift[0], 512 * 8, 0, 0, 0, 0, 1);
#endif

				UBYTE ubCEIndex = ubChannel & gubPlanesPerBurstMask;
				U32 ulSysCodeUnit = (U32)(guwSysCodeBlock[ubVersion * 2 + ubCodeBlockIndex][ubChannel] >> gubBurstsPerBankLog);
				UBYTE ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[ubVersion * 2 + ubCodeBlockIndex][ubChannel] & gubBurstsPerBankMask);

				ubFQIndex = gFQI.ubFQLinkFree;
				FlashQueue_t *FQ;
#if TLC
				if (0 == uwPlane) {
#if Hynix
					gubDaCmdEraseEnable = (guoFlashDefaultType.B.btHynix && guoFlashUseType.B.bt8LC) ? (TRUE) : (FALSE);
#endif

#if ENABLE_TLC_PROGRAM_CODEBLOCK
					if (ftl_Erase_Single_Plane(ulSysCodeUnit, ubSysCodeBurst, ubCEIndex, 0))
#else
					if (ftl_Erase_Single_Plane(ulSysCodeUnit, ubSysCodeBurst, ubCEIndex, ((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0)))
#endif
					{
#if Hynix
						gubDaCmdEraseEnable = FALSE;
#endif
						if (RDT_BURNER) {
							U32 ulBadBlockIndex = (U32)(M_Multiplier((ulSysCodeUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSysCodeBurst;
							ubDBTBuffer[ulBadBlockIndex] = (RDT_BURNER_MARKBAD | DBT_MARK_ERASE_FAIL);

							FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
							FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
							FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
							flaMicronRandomizer(0);
							return LATERBAD_FAIL;
						}
						else {
							ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[ubVersion * 2 + ubCodeBlockIndex][ubChannel], 0x60);
							SetFailCEtoSystemInfo(ubCEIndex);
							return TERMINAL_FAIL;
						}
					}
#if Hynix
					gubDaCmdEraseEnable = FALSE;
#endif
				}
#endif

				UBYTE ubLMU;
				UBYTE ubL4KCount = 0;
				for (ubLMU = 0; ubLMU < (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1); ubLMU++) {

					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->ulRAMAddr = ((U32)&ubBUF_16Kshift[0]);
					FQ->ubFJob = BYTE_FJOB_WRITE;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
					FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
					FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif
					FQ->uwFUnit = (UWORD)ulSysCodeUnit;
					if (ENABLE_PLANE_CH_CE) {
						FQ->ulFEntry = (((M_Multiplier((U32)(uwPlane + ubLMU), DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
					}
					else {
						FQ->ulFEntry = (((M_Multiplier((U32)(uwPlane + ubLMU), DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex) << gub4kEntrysPerPlaneLog);
					}

					FQ->ubRelativeDepth = ubDepth;
					FQ->ubDepth = 0xFF;
					FQ->ubL4kIndex = ubL4kLink;
					FQ->btUNServed = 1;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
					FQ->btD3 = 1;
					FQ->ubD3WriteMode = (D3_SUSPEND_EN) | D3_WRITE_FINE | (D3_WRITE_LOWDER << ubLMU);
					FQ->btTLCProgramCodeBlock = 1;
#endif
#if (TLC || MicronFlashOnly)
					gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
#else
					gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
#endif
					gulPara0x24.BitMap.btBufModeEn = 0;
					gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
					gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
					gulPara0x24.BitMap.btZipBypass = 1;
					gulPara0x24.BitMap.btCmpEn = 0;
					gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
					gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

#if (TLC || MicronFlashOnly)
					for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++)
#else
					for (ubL4KCount = 0; ubL4KCount < 1; ubL4KCount++)
#endif
					{
						pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
						//set L4K table info
						pL4KTable.ulL4K_LCA = HEADER_LCA;
						pL4KTable.Para0x04.BitMap.ulL4K_FW = (ubCodeBlockIndex + ubChannel * 2) + ((ubCodeVersion +  0x11 * ubVersion) << 8);
						pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
						pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0x7;
						pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
						pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
						pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0xAB;
						pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((((U32)&ubBUF_16Kshift[0]) + (ubL4KCount * 4096) ) / 512);
						*pL4KTablePtr = pL4KTable;
					}
					gFQI.ubFQLinkNumber++;
					gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

					// wait program done...
					while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
						M_SwitchTask();
					}
					FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

					if (gFQI.gFQLink[ubFQIndex].btPEFail) {
						if (RDT_BURNER) {
							U32 ulBadBlockIndex = (U32)(M_Multiplier((ulSysCodeUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSysCodeBurst;
							ubDBTBuffer[ulBadBlockIndex] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);

							M_SwitchTask();
							FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
							FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
							FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
							flaMicronRandomizer(0);
							return LATERBAD_FAIL;
						}
						else {
							ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[ubVersion * 2 + ubCodeBlockIndex][ubChannel], 0x12);
							SetFailCEtoSystemInfo(ubCEIndex);
							return TERMINAL_FAIL;
						}
					}
					M_SwitchTask();
				}
				//FALLL[FCTLL_IBF_CTL] |= FORCE_ZIP_COR; // normal mode maybe need
				ubFQIndex = gFQI.ubFQLinkFree;
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

				FQ->ulRAMAddr = ((U32)&ubBUF_16Kshift[0]);

				FQ->btCheckEmpty = 1;
				FQ->btThisIsErasePage = 0;
				FQ->ubFJob = BYTE_FJOB_READ;

#if ENABLE_TLC_PROGRAM_CODEBLOCK
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
				FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif



				FQ->uwFUnit = (UWORD)ulSysCodeUnit;
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((M_Multiplier((U32)uwPlane, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = (((M_Multiplier((U32)uwPlane, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex) << gub4kEntrysPerPlaneLog);
				}
				FQ->ubDepth = ubDepth;
				FQ->ubL4kIndex = ubL4kLink;
				FQ->btUNServed = 1;
				FQ->FQPara.B.btConvBypass = 0;

				gulPara0x24.BitMap.ubFrameNum = 1;
				gulPara0x24.BitMap.btBufModeEn = 0;
				gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
				gulPara0x24.BitMap.ubL4KNum = 1;
				gulPara0x24.BitMap.btZipBypass = 1;
				gulPara0x24.BitMap.btCmpEn = 0;
				gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gMTQ->Depth[ubDepth].ulZipMap = 0x000000FF;

				for (ubL4KCount = 0; ubL4KCount < 1; ubL4KCount++) {
					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
					//set L4K table info
					pL4KTable.ulL4K_LCA = 0x00000000;
					pL4KTable.Para0x04.ulAll = 0;
					pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
					pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((((U32)&ubBUF_16Kshift[0]) + (ubL4KCount * 4096) ) / 512);
					pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
					pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
					*pL4KTablePtr = pL4KTable;
				}

				gFQI.ubFQLinkNumber++;
				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;


				while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}

				gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

				if (FQ->btThisIsErasePage) { //沒被寫過
					if (RDT_BURNER) {
						U32 ulBadBlockIndex = (U32)(M_Multiplier((ulSysCodeUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSysCodeBurst;
						ubDBTBuffer[ulBadBlockIndex] = (RDT_BURNER_MARKBAD | DBT_MARK_READ_FAIL);

						M_SwitchTask();
						FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
						FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
						FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
						flaMicronRandomizer(0);
						return LATERBAD_FAIL;
					}
					else {
						ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[ubVersion * 2 + ubCodeBlockIndex][ubChannel], 0x21);
						SetFailCEtoSystemInfo(ubCEIndex);
						//SetInitialFailID(0x1, (BIT0 << ubCEIndex));
						return TERMINAL_FAIL;
					}
				}


				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
				if (pL4KTablePtr->ulL4K_LCA != HEADER_LCA) {
					if (RDT_BURNER) {
						U32 ulBadBlockIndex = (U32)(M_Multiplier((ulSysCodeUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSysCodeBurst;
						ubDBTBuffer[ulBadBlockIndex] = (RDT_BURNER_MARKBAD | DBT_MARK_READ_FAIL);

						M_SwitchTask();
						FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
						FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
						FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
						flaMicronRandomizer(0);
						return LATERBAD_FAIL;
					}
					else {
						ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[ubVersion * 2 + ubCodeBlockIndex][ubChannel], 0x24);
						SetFailCEtoSystemInfo(ubCEIndex);
						//SetInitialFailID(0x5, (BIT0 << ubCEIndex));
						return TERMINAL_FAIL;
					}
				}

				M_SwitchTask();
				FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);

				uwPlane += ENABLE_TLC_PROGRAM_CODEBLOCK2 ? 3 : 1;


				//UBYTE ubSection = 0;

				//	uwPageNumAdded = uwPageNum[0];
				//	uwBufferFlag = 0;
				//	uwMaxPageIndexSection = 0;

				U32 ulBufferStart, ulremainSec, ulAddress_ptr;
				for (ultempA = 0; ultempA < 8; ultempA++) { //// 4 sections ahead
				SkipZeroSections:
					if (gulSectionInformation[2][ultempA] == 0) {
						continue;
					}
					ulremainSec = gulSectionInformation[2][ultempA] / 512;
					ulAddress_ptr = gulSectionInformation[0][ultempA];
					for (uwPageIndexInSection = 0; uwPageIndexInSection < uwPageNum[ultempA]; uwPageIndexInSection++) {
						if (ulremainSec >= gubSectorsPerPlane) {
							ulBufferStart = (U32)ubBUF_16Kshift;
							mDMAC_SV(1, (U32)&ubBUF_16Kshift[0], 512 * gubSectorsPerPlane, 0, 0, 0, DMA_EN_SETCQ, 1);
							mDMAC_COPY(ulAddress_ptr, (U32)&ubBUF_16Kshift[0], 512 * gubSectorsPerPlane, 0, 0, 0, 0, 1);
							//ulAddress_ptr += gubSectorsPerPlane;
							ulremainSec -= gubSectorsPerPlane;
							ulAddress_ptr += gubSectorsPerPlane * 512;
						}
						else {
							ulBufferStart = (U32)ubBUF_16Kshift;
							mDMAC_SV(1, (U32)&ubBUF_16Kshift[0], 512 * gubSectorsPerPlane, 0, 0, 0, DMA_EN_SETCQ, 1);
							mDMAC_COPY(ulAddress_ptr, (U32)&ubBUF_16Kshift[0], 512 * ulremainSec, 0, 0, 0, 0, 1);
							ulremainSec = 0;
						}
						//UartString("	   plane:");
						//UartWordHex(uwPlane);
						//UartString("	   *\n");
#if ENABLE_TLC_PROGRAM_CODEBLOCK2
						if (ftlWriteSinglePlaneTLC(ulSysCodeUnit, uwPlane, ubCEIndex, ulBufferStart, CODE_LCA, ((ultempA + 1) + ((ubCodeVersion + 0x11 * ubVersion) << 8)), (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSysCodeBurst))
#else
						if (ftlWriteSinglePlane(ulSysCodeUnit, uwPlane, ubCEIndex, ulBufferStart, CODE_LCA, ((ultempA + 1) + ((ubCodeVersion + 0x11 * ubVersion) << 8)), (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSysCodeBurst))
#endif
						{
							if (RDT_BURNER) {
								U32 ulBadBlockIndex = (U32)(M_Multiplier((ulSysCodeUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSysCodeBurst;
								ubDBTBuffer[ulBadBlockIndex] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);

								FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
								FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
								flaMicronRandomizer(0);
								return LATERBAD_FAIL;
							}
							else {
								ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[ubVersion * 2 + ubCodeBlockIndex][ubChannel], 0x12);
								SetFailCEtoSystemInfo(ubCEIndex);
								return TERMINAL_FAIL;
							}
						}

						UBYTE ubstatus;
#if ENABLE_TLC_PROGRAM_CODEBLOCK2
						ubstatus = ftlReadSinglePlane(ulSysCodeUnit, uwPlane, ubCEIndex, (U32)&ubBUF_32Kshift[0], CODE_LCA,  (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSysCodeBurst);
#else
						ubstatus = ftlReadSinglePlane(ulSysCodeUnit, uwPlane, ubCEIndex, (U32)&ubBUF_32Kshift[0], CODE_LCA,  (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSysCodeBurst);
#endif
						if (ubstatus == Read_Erase_Page) {
							if (RDT_BURNER) {
								U32 ulBadBlockIndex = (U32)(M_Multiplier((ulSysCodeUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSysCodeBurst;
								ubDBTBuffer[ulBadBlockIndex] = (RDT_BURNER_MARKBAD | DBT_MARK_READ_FAIL);
								return LATERBAD_FAIL;
							}
							else {
								//SetInitialFailID(0x1, (BIT0 << ubCEIndex));
								ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[ubVersion * 2 + ubCodeBlockIndex][ubChannel], 0x21);
								SetFailCEtoSystemInfo(ubCEIndex);

								FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
								FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
								flaMicronRandomizer(0);
								return TERMINAL_FAIL;
							}
						}
						else if (ubstatus == Read_Spear_Mismatch) {
							if (RDT_BURNER) {
								U32 ulBadBlockIndex = (U32)(M_Multiplier((ulSysCodeUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSysCodeBurst;
								ubDBTBuffer[ulBadBlockIndex] = (RDT_BURNER_MARKBAD | DBT_MARK_READ_FAIL);

								FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
								FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
								flaMicronRandomizer(0);
								return LATERBAD_FAIL;
							}
							else {
								//SetInitialFailID(0x5, (BIT0 << ubCEIndex));
								ftlPreformatShowError(ubCEIndex, guwSysCodeBlock[ubVersion * 2 + ubCodeBlockIndex][ubChannel], 0x24);
								SetFailCEtoSystemInfo(ubCEIndex);
								return TERMINAL_FAIL;
							}
						}
						uwPlane += ENABLE_TLC_PROGRAM_CODEBLOCK2 ? 3 : 1;
					}
				}
#if (!RDT_BURNER)
#if ENABLE_TLC_PROGRAM_CODEBLOCK2
				U32 ulDummyPage;
				mDMAC_SV( 4, SRAM_BASE + 256 * 512, ((gubSectorsPerPlane >> 3) * 0x1000), 0x31113111, 0xffffffff, 0, DMA_EN_SETCQ, 0);
				for (ulDummyPage = uwPlane ; ulDummyPage < guwPagesPerUnit; ulDummyPage += 3) {
					ftlWriteSinglePlaneTLC(ulSysCodeUnit, ulDummyPage, ubCEIndex,	SRAM_BASE + 256 * 512, CODE_LCA, ((ultempA + 1) + ((ubCodeVersion + 0x11 * ubVersion) << 8)), (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSysCodeBurst);
				}
#else
				if (gubFlashID[0] == 0xb5) {
					U32 ulDummyPage;
					mDMAC_SV( 4, SRAM_BASE + 256 * 512, ((gubSectorsPerPlane >> 3) * 0x1000), 0x31113111, 0xffffffff, 0, DMA_EN_SETCQ, 0);
					for (ulDummyPage = uwPlane ; ulDummyPage < guwPagesPerUnit; ulDummyPage++) {
						ftlWriteSinglePlane(ulSysCodeUnit, ulDummyPage, ubCEIndex,	SRAM_BASE + 256 * 512, CODE_LCA, ((ultempA + 1) + ((ubCodeVersion + 0x11 * ubVersion) << 8)), (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSysCodeBurst);
					}
				}
#endif

#endif
			}
		}

	}
	FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
	FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
	flaMicronRandomizer(0);
	return NORMAL;
	//return ubFail;
#endif
}

UBYTE FlaPrepareHeader_Code(UBYTE ubProgramAgain)
{
	M_CheckNonBurnerModeToDisableFunctionReturnValid(0)
	UBYTE ubProgFlashResult, ubCEIndex;
	UBYTE ubNeedUpdateDBT = FALSE;
	UBYTE *ubDBTBuffer = (UBYTE *)(&GCGRSearchTable[0]);
	U32 ulFreeblock, ulTotalBlockNum;
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state

	gubHeaderSetFeature = HB[HB_LBA_H];
	/*if (Enable_Set_Feature) {
		if ((gubFlashID[0] == 0x98) || (gubFlashID[0] == 0x45)) {
			if ((gubInterface != FLH_MODE_LEGACY)) {
				//if (Enable_Force_Toggle1) {
				//gubInterface = INTERFACE_TOGGLE1;
				//}
				//if (flaSetToggleMode(gubInterface, 0) == RET_OK) {
				//gubHeaderSetFeature = TRUE;
				//}
			}
		}
		else {
			if ((gubFlashMode == INTERFACE_NVDDR)||Enable_Force_NVDDR) {
				if (FlaSetONFIMode(0, gulParamPageInfo.ONFI.ubNVDDRTimingMode) == RET_OK) {
					gubHeaderSetFeature = TRUE;
					gubInterface = INTERFACE_NVDDR;
					Uart_Printf("Set Feature %d Pass\r\n", INTERFACE_NVDDR);

				}
			}
			else if (gubFlashMode == INTERFACE_NVDDR2) {
				if (FlaSetONFIMode(1, gulParamPageInfo.ONFI.ubNVDDR2TimingMode) == RET_OK) {
					gubHeaderSetFeature = TRUE;
					Uart_Printf("Set Feature %d Pass\r\n", gubInterface);
				}
			}
			else if (gubFlashMode == INTERFACE_NVDDR2_WITHOUTTOGGLE2) {
				if (FlaSetONFIMode(2, gulParamPageInfo.ONFI.ubNVDDR2TimingMode) == RET_OK) {
					gubHeaderSetFeature = TRUE;
					Uart_Printf("Set Feature %d Pass\r\n", gubInterface);
				}
			}
	}
		  }*/
	//1.先把整張DBT拉上RAM
	if ( RDT_FindDBTBlock_Task()) {
		gubRDTDone = DBT_Checked;
	}
	else {
		gubRDTDone = (DBT_Checked | DBT_Exist);
	}

	if (gubRDTDone & DBT_Exist) {//把整張DBT拉上RAM
		mDMAC_SV(1, (U32)ubDBTBuffer, ulEntryPerDBTPlane * 32, 0, 0xFFFFFFFF, 0, DMA_EN_SETCQ, 1);
		ulFreeblock = 0;
		ulTotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;

		while (ulTotalBlockNum > ulFreeblock) {
			//loadDBTtoBuffer
			if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[ulFreeblock])) {
				ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				return TERMINAL_FAIL;
			}
			ulFreeblock += ulEntryPerDBTPlane;
		}
	}
	//2.ProgramCodeBlock
	do {
		ubProgFlashResult = FlaGenerateHeader(ubProgramAgain, (U32)&ubDBTBuffer[0]);
		if (RDT_BURNER && ubProgFlashResult == LATERBAD_FAIL) {	//會進這邊代表還會再進一次FlaGenerateHeader
			U32 ulSysCodeUnit;
			UBYTE ubSysCodeBurst, ubCodeBlockIndex;

			ubNeedUpdateDBT = TRUE;
			for (ubCodeBlockIndex = 0 ; ubCodeBlockIndex < 2 ; ubCodeBlockIndex++) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					ulSysCodeUnit = (U32)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] >> gubBurstsPerBankLog);
					ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[ubCodeBlockIndex][ubCEIndex] & gubBurstsPerBankMask);
					ftl_Erase_Single_Plane(ulSysCodeUnit, ubSysCodeBurst, ubCEIndex, ((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0));
				}
			}
		}
	}
	while (RDT_BURNER && ubProgFlashResult == LATERBAD_FAIL);//LATERBAD_FAIL就繼續找下一個(FW BURNER不會回傳LATERBAD_FAIL)

	//3.Upadate DBT
#if RDT_BURNER
	if ((ubNeedUpdateDBT == TRUE) && (ubProgFlashResult == NORMAL)) {
		UBYTE ubProgramNewDBTState, ubi;

		for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
			gubLaterBadCount_ForDBT[ubi] = 0;
		}
		do {
			UartString("\nUpdateDBT");
			//每次進一次SaveDBTSet都會從freeQ拿一個set去寫DBT,拿完了就沒了
			ubProgramNewDBTState = SaveDBTSet((U32)&ubDBTBuffer[0]);
		}
		while (ubProgramNewDBTState == LATERBAD_FAIL);

		ubProgFlashResult = ubProgramNewDBTState;
	}
#endif

	return ubProgFlashResult;
}

UBYTE RDT_FindDBTBlock_Task()
{
#if(!RDT)
	//M_CheckBurnerModeToDisableFunctionReturnValid(1);     //dummyFW need to find DBT created by RDT during preformat
	M_CheckRDTModeToDisableFunctionReturnValid(0);
	HandleStopRW(1);
	U32 ulVenderUnit;
	UBYTE ubDBTGetCount = 0;
	UBYTE ubCEIndex, ubDepth, ubFQIndex, ubL4kLink, ubL4KNum = 1, ubBurst;;
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;

	guwDBTBlock[0] = 0xFFFF;
	guwDBTBlock[1] = 0xFFFF;
	gubDBTSet = 0xFF;
	for (ulVenderUnit = 0; ulVenderUnit < SYSTEMAREA_DEFAULT_SCAN_RANGE ; ulVenderUnit++) {
		for (ubBurst = 0; ubBurst < gubBurstsPerBank; ubBurst++) {
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				if (guwDBTBlock[ubCEIndex] == 0xFFFF) {
					ubFQIndex = gFQI.ubFQLinkFree;
					FlashQueue_t *FQ;
					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->ulRAMAddr = BUFFER3_BASE + 0x4000;
					FQ->btCheckEmpty = 1;
					FQ->ubFJob = BYTE_FJOB_READ;
					FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
					FQ->uwFUnit = ulVenderUnit;
					if (ENABLE_PLANE_CH_CE) {
						FQ->ulFEntry = (((U32)ubCEIndex << gubBurstsPerBankLog) + ubBurst)  << gub4kEntrysPerPlaneLog;
					}
					else {
						FQ->ulFEntry =  (U32)ubCEIndex << gub4kEntrysPerPlaneLog;
					}
					FQ->ubDepth = ubDepth;
					FQ->ubL4kIndex = ubL4kLink;
					FQ->btUNServed = 1;
#if TLC
					gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
#else
					gulPara0x24.BitMap.ubFrameNum = 1;
#endif
					gulPara0x24.BitMap.btBufModeEn = 0;
					gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
					gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
					gulPara0x24.BitMap.btZipBypass = 1;
					gulPara0x24.BitMap.btCmpEn = 0;
					gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
					gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
					gMTQ->Depth[ubDepth].ulZipMap = 0x000000FF;

					UBYTE ubL4KCount = 0;
					for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
						pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
						//set L4K table info
						pL4KTable.ulL4K_LCA = 0x00000000;
						pL4KTable.Para0x04.ulAll = 0;
						pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
						pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((BUFFER3_BASE + 0x4000 + (ubL4KCount * 4096)) / 512);
						pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
						pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
						*pL4KTablePtr = pL4KTable;
					}
					gFQI.ubFQLinkNumber++;
					gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;


					while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
						M_SwitchTask();
					}
					if ((gubINTTable[FQ->ubDepth].All & 0xF0)) {
						FCTLL[FCTLL_INT_INF + (((ubCEIndex & gubPlanesPerBurstMask) * 0x200) >> 2)] = 0;
						gubINTTable[FQ->ubDepth].All &= 0x0F;
					}
					gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
					if ((pL4KTablePtr->ulL4K_LCA == MARK_DBT)) {
						guwDBTBlock[ubCEIndex] = (UWORD)((ulVenderUnit << gubBurstsPerBankLog) + ubBurst);
						ubDBTGetCount++;

						if (ubDBTGetCount == MAX_CHANNEL) { //2ch都找到了
							//UartString("\nDBT_2");
							ubCEIndex = (guwDBTBlock[0] != 0xFFFF) ? 0 : 1; //決定用那一個CE為主去算Set Number
							gubDBTSet = ftlGetSystemSetNumber(Mode_ByDBTinFlash, ((U32)(BUFFER3_BASE + 0x4000)), ubCEIndex, guwDBTBlock[ubCEIndex]);
							gubStopFTL = 0;
							return 0;
						}
					}
				}
			}
		}
	}

	gubStopFTL = 0;
	if (ubDBTGetCount == 0) { //兩個channel都沒有找到
		//UartString("\n DBT_0");
		return 1;
	}
	else {//只找到1ch
		//UartString("\nDBT_1");
		ubCEIndex = (guwDBTBlock[0] != 0xFFFF) ? 0 : 1; //決定用那一個CE為主去算Set Number
		gubDBTSet = ftlGetSystemSetNumber(Mode_ByDBTinFlash, ((U32)(BUFFER3_BASE + 0x4000)), ubCEIndex, guwDBTBlock[ubCEIndex]);
		return 0;
	}
#endif
#if(RDT)
	return 0;
#endif
}
#if EnableProductHistory

UBYTE FindPHBlockByDBT()
{
	U32 ulVenderUnit, ulDBTindex;
	UBYTE *ubDBTBuffer = (UBYTE *)(BUFFER3_BASE + 0x4000);
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	UBYTE ubFind = 0;
	UBYTE ubplanebank;
	UBYTE ubtemp;


	for (  ulVenderUnit = PHStartUnit;  ulVenderUnit < PHEndUnit; ulVenderUnit++) {
		for (  ubplanebank = 0;  ubplanebank < gubBurstsPerBank * gubCENumber ; ubplanebank++) {

			ulDBTindex = ulVenderUnit * gubBurstsPerBank * gubCENumber * gubDieNumber + ubplanebank;

			if (((ulVenderUnit == PHStartUnit) && (ubplanebank == 0)) || ((ulDBTindex % ulEntryPerDBTPlane) == 0)) {
				if (ftlLoadDBT(ulDBTindex, (U32)&ubDBTBuffer[0])) {
					//ftlPreformatShowError(0, gubDBTUnit, 0x28);
					return 1;
				}
			}

			if (ubDBTBuffer[(ulDBTindex % ulEntryPerDBTPlane)] == PHMarkBad) {
				guwPHUnit[ubFind].uwUnit = ulVenderUnit;
				guwPHUnit[ubFind].ubPlanebank = ubplanebank;
				gubPHmatch[ubFind] |= Bit0_FindByDBT;
				ubFind++;
			}

			if (ubFind == PHUnitNum) {
				return 0;
			}
		}
	}

	if (ubFind < PHUnitNum) {
		//DBT找出來的ph<2,用SCAN的,歸零
		for (ubtemp = 0; ubtemp < PHUnitNum; ubtemp++) {
			guwPHUnit[ubtemp].uwUnit = PH_Default;
			guwPHUnit[ubtemp].ubPlanebank = 0xff;
			guwPHUnit[ubtemp].uwPagePTR = PH_Default;
			gubPHmatch[ubtemp] = 0 ;
		}
		return 1;
	}

	return 0;

}
void PHexchange()
{
	UWORD uwphtemp;
	UBYTE ubphtemp;

	uwphtemp = guwPHUnit[0].uwUnit;
	guwPHUnit[0].uwUnit = guwPHUnit[1].uwUnit;
	guwPHUnit[1].uwUnit = uwphtemp;

	ubphtemp = guwPHUnit[0].ubPlanebank;
	guwPHUnit[0].ubPlanebank = guwPHUnit[1].ubPlanebank;
	guwPHUnit[1].ubPlanebank = ubphtemp;

	uwphtemp = guwPHUnit[0].uwPagePTR;
	guwPHUnit[0].uwPagePTR = guwPHUnit[1].uwPagePTR;
	guwPHUnit[1].uwPagePTR = uwphtemp;

	ubphtemp = gubPHmatch[0];
	gubPHmatch[0] = gubPHmatch[1];
	gubPHmatch[1] = ubphtemp;

}

UBYTE ScanPHBlock_Task(UBYTE ubmode)
{
	UBYTE ubtemp;
	//UBYTE ubtemp2[2];
	UBYTE ubCEIndex;
	UWORD ulPlanePTR;
	//UBYTE ubPlaneBank;
	FlashQueue_t *FQ;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;

	//ubmode 0 有abort
	//ubmode 1 不abort

	for (ubtemp = 0; ubtemp < PHUnitNum; ubtemp++) {
		guwPHUnit[ubtemp].uwPagePTR = PH_Default;
		if (ubmode == 0) {
			if ((guwPHUnit[ubtemp].uwUnit == PH_Default) || (guwPHUnit[ubtemp].ubPlanebank == 0xff)) {
				return 1;
			}
		}
	}


	for (ubtemp = 0; ubtemp < PHUnitNum; ubtemp++) {
		if (ubmode) {
			if ((guwPHUnit[ubtemp].uwUnit == PH_Default) || (guwPHUnit[ubtemp].ubPlanebank == 0xff)) {
				continue;
			}
		}
		for (ulPlanePTR = guwFastPagePagesPerUnit; ulPlanePTR > 0 ; ulPlanePTR--) {

			UBYTE ubDepth, ubL4kLink;
			ubCEIndex = guwPHUnit[ubtemp].ubPlanebank / gubBurstsPerBank;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->btCheckEmpty = 1;
			FQ->btThisIsErasePage = 0;
			FQ->ulRAMAddr = BUFFER3_BASE + (gub4kEntrysPerPlane * 0x1000);
			FQ->btPreRead = 0;
			FQ->uwFUnit = guwPHUnit[ubtemp].uwUnit;

			FQ->ulFEntry = ( (guwPHUnit[ubtemp].ubPlanebank + (ulPlanePTR - 1) * gubPlanesPerPage) << gub4kEntrysPerPlaneLog);
			//UartString("\nEny:");
			//UartLongHex(FQ->ulFEntry);

			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS );
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btNoWait = 1;
			FQ->btUNServed = 1;
			FQ->btD3 = 1;

			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gulPara0x24.BitMap.ubL4KNum = gub4kEntrysPerPlane;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

			//if (ENABLE_DCACHE) {
			//		xthal_dcache_region_invalidate((void *)FQ->ulRAMAddr, gub4kEntrysPerPlane * 4096);
			//	}

			UBYTE ubL4KCount;
			for (ubL4KCount = 0; ubL4KCount < gub4kEntrysPerPlane; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = MARK_BeforeRead;
				pL4KTable.Para0x04.ulAll = 0; // for cpp check. Variable needs initial value.
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

			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR) * Spare4kSize));
			U32 ulL4K_LCA = pL4KTablePtr->ulL4K_LCA;

			if (ulL4K_LCA == MARK_PH) {
				guwPHUnit[ubtemp].uwPagePTR = (ulPlanePTR - 1);
				break;
			}


		}


	}

	// 0xff表示沒找到,所以是還沒寫所以比較小
	//排序後[0][] ptr 比較小, [1][] 比較大    ptr 0,127時除外

	if (guwPHUnit[0].uwPagePTR == PH_Default) {
		// 0xff 0xff
		//0xff 0x00

	}
	else if (guwPHUnit[1].uwPagePTR == PH_Default) {
		//0x0 0xff

		PHexchange();
	}
	else if (guwPHUnit[0].uwPagePTR > guwPHUnit[1].uwPagePTR) {

		if ((guwPHUnit[0].uwPagePTR - guwPHUnit[1].uwPagePTR) == (guwFastPagePagesPerUnit - 1)) {
			//127 0
		}
		else { //if((guwPHUnit[0][2]-guwPHUnit[1][2])==1)
			//127 126
			PHexchange();
		}

	}
	else {
		if ((guwPHUnit[1].uwPagePTR - guwPHUnit[0].uwPagePTR) == (guwFastPagePagesPerUnit - 1)) {
			//0 127
			PHexchange();
		}
		else { //if((guwPHUnit[1][2]-guwPHUnit[0][2])==1)
			//126 127
		}

	}

	return 0;
}


UBYTE FindPHBlockByScan(UBYTE ubmode)
{
	U32 ulVenderUnit;
	UBYTE ubCEIndex, ubPlaneBank, ubDepth, ubFQIndex, ubL4kLink, ubL4KNum = 1;
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;
	UBYTE ubFind = 0;


	////mode 0: ->沒有DBT時 直接SCAN
	////mode 1: ->有DBT找到時, check DBT找的真的scan的到
	////目前code裡沒有call mode 1留一條路

	for (  ulVenderUnit = PHStartUnit;  ulVenderUnit < PHEndUnit; ulVenderUnit++) {
		for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {

			if (ubmode) {
				if ((guwPHUnit[ubFind].uwUnit != ulVenderUnit) || (guwPHUnit[ubFind].ubPlanebank != ubPlaneBank)) {
					continue;
				}
			}

			ubCEIndex = ubPlaneBank / gubBurstsPerBank;
			ubFQIndex = gFQI.ubFQLinkFree;
			FlashQueue_t *FQ;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ulRAMAddr = BUFFER3_BASE + (gub4kEntrysPerPlane * 0x1000);
			FQ->btCheckEmpty = 1;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
			FQ->uwFUnit = ulVenderUnit;
			FQ->ulFEntry = (ubPlaneBank << gub4kEntrysPerPlaneLog);

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
			gMTQ->Depth[ubDepth].ulZipMap = 0x000000FF;

			UBYTE ubL4KCount = 0;
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = 0x00000000;
				pL4KTable.Para0x04.ulAll = 0;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((BUFFER3_BASE + (gub4kEntrysPerPlane * 0x1000) + (ubL4KCount * 4096)) / 512);
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;


			while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			if ((gubINTTable[FQ->ubDepth].All & 0xF0)) {
				FCTLL[FCTLL_INT_INF + (((ubCEIndex & gubPlanesPerBurstMask) * 0x200) >> 2)] = 0;
				gubINTTable[FQ->ubDepth].All &= 0x0F;
			}
			gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
			if ((pL4KTablePtr->ulL4K_LCA == MARK_PH)) {
				if (ubmode == 0) {
					guwPHUnit[ubFind].uwUnit = ulVenderUnit;
					guwPHUnit[ubFind].ubPlanebank = ubPlaneBank;
				}
				gubPHmatch[ubFind] |= Bit1_FindByScan;
				ubFind++;
				if (ubFind == PHUnitNum) {
					return 0;
				}
			}
		} //planebank

	} //unit

	return 1;

}

#if(0)
UBYTE ftlPickPHBlock_Task()
{
	UBYTE *ubDBTBuffer = (UBYTE *)(BUFFER3_BASE + 0x8000);
	U32 ulFreeUnitPTR, uli, ulj, ulFreeblock, TotalBlockNum;
	UBYTE ubbadblock;

	UBYTE ubCEIndex;

	UBYTE ubDBTtemp = 0xFF;
	U32 ulplane;
	UBYTE ubDieIndextemp, ubCEIndextemp, ubPlaneBanktemp, ubFind;
	UWORD uwUnittemp;

	UBYTE ubDBTMappingPlaneIndex;
	UBYTE ubDBTSizeInPlanes;





	if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {
		//ftlPreformatShowError(0, gubDBTUnit, 0x28);
		return 1;
	}

	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......

	//找一個可以寫DBT的BLK
	for (ulFreeUnitPTR = (gubDBTUnit + 1) ; ulFreeUnitPTR < 100; ulFreeUnitPTR++) {
		ubbadblock = 0;

		ulFreeblock = (M_Multiplier((ulFreeUnitPTR << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) ;
		uli = ulFreeblock % ulEntryPerDBTPlane;                                     //該bank放在這個dbt plane的第幾個bit
		if (uli == 0) {
			if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
				//ftlPreformatShowError(0, gubDBTUnit, 0x28);
				return 1;
			}
		}

		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) { //只找plane-a
			if (ulFreeUnitPTR == ((U32)gubDBTUnit)) {
				break;
			}
			if (ENABLE_PLANE_CH_CE) {
				if (ubDBTBuffer[uli + (ubCEIndex << gubBurstsPerBankLog)] & 0xF0) {
					ubbadblock |= 1;
					break;
				}
			}
			else {
				if (ubDBTBuffer[uli + ubCEIndex] & 0xF0) {
					ubbadblock |= 1;
					break;
				}
			}


		}

		if (ubbadblock == 0) {
			if (ulFreeUnitPTR != ((U32)gubDBTUnit)) {
#if (TLC || ENABLE_ULTRAMLC)
#if B0KB
				ftl_Direct_Erase_Unit(ulFreeUnitPTR, (BIT_FJOBI_FASTPAGE ));
#else
				ftl_Direct_Erase_Unit(ulFreeUnitPTR, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_ERASE_FAIL_RESET));
#endif
#else
				ftlA2EraseSingleUnit(ulFreeUnitPTR);
#endif
			}
			ubDBTtemp = ulFreeUnitPTR;
			break;
		}

	}

	if (ulFreeUnitPTR == 100) {
		//ftlPreformatShowError(ubCEIndex, ulFreeUnitPTR, 0x80);
		return 1;
	}

	//load DBT 起來寫到new DBT,順便將選好的BLK mark bad
	ubFind = 0;
	ulplane = 0;
	ulFreeblock = 0;
	ubDBTMappingPlaneIndex = 0;
	ubDBTSizeInPlanes = 0;
	TotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;
	while (TotalBlockNum > ulFreeblock) {
		//loadDBTtoBuffer
		if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
			//ftlPreformatShowError(0, gubDBTUnit, 0x28);
			return 1;
		}

		if ((TotalBlockNum - ulFreeblock) > ulEntryPerDBTPlane) {
			ulj = ulEntryPerDBTPlane;
		}
		else {
			ulj = (TotalBlockNum - ulFreeblock);
		}
		for (uli = ulFreeblock; uli < (ulFreeblock + ulj); uli++) {	// change later bad mark to early bad mark

			if (ubFind < PHUnitNum) {
				uwUnittemp = uli / (gubCENumber * gubBurstsPerBank * gubDieNumber);
				ubCEIndextemp = (uli / gubBurstsPerBank) % gubCENumber;
				ubPlaneBanktemp = uli % (gubCENumber * gubBurstsPerBank);
				ubDieIndextemp = (uli / (gubCENumber * gubBurstsPerBank)) % gubDieNumber;

				if ((uwUnittemp > 100) && (uwUnittemp < 150) && (ubDieIndextemp == 0)) {

					if (ubDBTBuffer[uli] == 0x00) {
						ubDBTBuffer[uli] = 0x41;
						guwPHUnit[ubFind].uwUnit = uwUnittemp;
						guwPHUnit[ubFind].ubPlanebank = ubPlaneBanktemp;
						ubFind++;
					}
				}
			}
			else {
				break;
			}
		}

		//programDBTtoFLA
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			if (ftlWriteSinglePlane(ubDBTtemp, ulplane,  ubCEIndex, ((U32)&ubDBTBuffer[0] ), 0x3FB10200, 0, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0)) {
				//ftlPreformatShowError(ubCEIndex, ubi, 0x12);
				return 1;
			}
		}
		ubDBTSizeInPlanes++;
		ubDBTMappingPlaneIndex++;
		ulplane++;//ulplane += gubPlanesPerPage;
		ulFreeblock += ulEntryPerDBTPlane;
	}

	if (ubFind < PHUnitNum) {
		return 1;
	}

	while (ulplane < guwFastPagePagesPerUnit) {
		if (ubDBTMappingPlaneIndex == ubDBTSizeInPlanes) {	// 寫完一份DBT, 需先reset再繼續寫下一份DBT
			ubDBTMappingPlaneIndex = 0;
			ulFreeblock = 0;
		}
		//programDBTtoFLA
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			if (ftlWriteSinglePlane(ubDBTtemp, ulplane,  ubCEIndex, ((U32)&ubDBTBuffer[ulFreeblock] ), 0x3FB10200, 0, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0)) {
				//ftlPreformatShowError(ubCEIndex, ubi, 0x12);
				return 1;
			}
		}
		ubDBTMappingPlaneIndex++;
		ulplane++;//ulplane += gubPlanesPerPage;
		ulFreeblock += ulEntryPerDBTPlane;
	}

	//check the new DBT
	UBYTE gubDBTUnittemp = gubDBTUnit;
	gubDBTUnit = ubDBTtemp;

	ulplane = 0;
	ulFreeblock = 0;
	while (TotalBlockNum > ulFreeblock) {
		if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
			//ftlPreformatShowError(0, gubDBTUnit, 0x28);
			return 1;
		}
		ulFreeblock += ulEntryPerDBTPlane;
	}

#if (TLC || ENABLE_ULTRAMLC)
	//free the old DBT
#if B0KB
	ftl_Direct_Erase_Unit(gubDBTUnittemp, (BIT_FJOBI_FASTPAGE));
#else
	ftl_Direct_Erase_Unit(gubDBTUnittemp, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_ERASE_FAIL_RESET));
#endif
#else
	/*
	//free the old DBT
	for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
		if (ENABLE_PLANE_CH_CE) {
			ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		}
		else {
			ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
		}
		ubFQIndex[ubPlaneBank] = gFQI.ubFQLinkFree;
		FlashQueue_t *FQ;
		UBYTE ubDepth, ubL4kLink;
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
		FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
		FQ->uwFUnit = gubDBTUnittemp;
		FQ->ulFEntry = (ubPlaneBank << gub4kEntrysPerPlaneLog);
		FQ->ubRelativeDepth = ubDepth;
		FQ->ubDepth = 0xFF;
		FQ->btUNServed = 1;
		FQ->btCheckEmpty = 0;
		gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;


		while (gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubFQPhase = BYTE_FQ_CHECKDONE;
		if ((ubPlaneBank == 0) || (ubPlaneBank == ((gubPlanesPerBurst - 1) << gubBurstsPerBankLog))) {
			if (gFQI.gFQLink[ubFQIndex[ubPlaneBank]].btPEFail) {
				//ftlPreformatShowError(ubCEIndex, gubDBTUnittemp, 0x42);
				return 1;
			}
		}
	}
	*/
	ftlA2EraseSingleUnit(gubDBTUnittemp);
#endif


	return 0;

}
#endif

#endif

#if EnableVenderCmdHistory
UBYTE Vender_SaveVenderCmdHistory()
{
	U32 ulVenderUnit, ulDBTindex;
	UBYTE *ubDBTBuffer = (UBYTE *)(BUFFER3_BASE + 0x4000);
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	UBYTE ubplanebank;
	UBYTE ubi;
	if (gubDBTSet == 0xFF) {
		//UartString("\n [NotFoundDBT]");
		return 0;
	}

	for (ubi = 0; ubi < (MaxVenderCmdLog + 1); ubi++) {
		BUFB_BASE[ubi] = gubVendeCmdHistory[ubi];
	}
	BUFB_BASE[C_VCLogPTR] = gubVendeCmdHistoryPtr;

#if BURNER
	BUFB_BASE[C_FWMode] = 0xA1;
#else
	BUFB_BASE[C_FWMode] = 0xA2;
#endif

	for (ubi = C_MARKStartOffset; ubi < C_MARKEndOffset; ubi++) {
		BUFB_BASE[ubi] = 0x5A;
	}

#if EnableVH_UARTDebug
	UartString("\n [VH_D]");
	for (ubi = 0; ubi < C_MARKEndOffset; ubi++) {
		Uart_Tx_DataHex(BUFB_BASE[ubi]);
	}
#endif

	for (ulVenderUnit = C_VHStartUnit;  ulVenderUnit < C_VHEndUnit; ulVenderUnit++) {
		ubplanebank = 0;
		ulDBTindex = ulVenderUnit * gubBurstsPerBank * gubCENumber * gubDieNumber + ubplanebank;

		if (((ulVenderUnit == C_VHStartUnit) && (ubplanebank == 0)) || ((ulDBTindex % ulEntryPerDBTPlane) == 0)) {
			if (ftlLoadDBT(ulDBTindex, (U32)&ubDBTBuffer[0])) {
				return 0;
			}
		}

		if (ubDBTBuffer[(ulDBTindex % ulEntryPerDBTPlane)] == 0) {

			ftlWriteSinglePlane(ulVenderUnit, (ubplanebank << gub4kEntrysPerPlaneLog),  (ubplanebank / gubBurstsPerBank), (U32)BUFB_BASE, MARK_VH, 0x112233, (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), (gubSectorsPerPlane / 8), 1, 0);
			return 1;
		}


	}
	return 0;

}
#endif

UBYTE RDT_FindCodeBlock_Task(UBYTE ubCEIndex)
{
	//M_CheckBurnerModeToDisableFunctionReturnValid(1);     //dummyFW need to find DBT created by RDT during preformat
	M_CheckRDTModeToDisableFunctionReturnValid(0);
	UBYTE ubSysCodeCount = 0;
	U32 ulVenderUnit;
	UBYTE ubDepth, ubFQIndex, ubL4kLink, ubL4KNum = 1, ubBurst;;
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;
	U32 ulAddressRegion = (FCONL[FCONL_FLH_FUNC] & RDM_REF_ROW_ADR_5_TO_0);
	HandleStopRW(1);
	FCONL[FCONL_FLH_FUNC] |= RDM_REF_ROW_ADR_5_TO_0;
	FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
	FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);

	gubSysCodeSet[0] = 0xFF;
	gubSysCodeSet[1] = 0xFF;
	guwSysCodeBlock[0][ubCEIndex] = 0xFFFF;
	guwSysCodeBlock[1][ubCEIndex] = 0xFFFF;

	for (ulVenderUnit = 0;  ulVenderUnit < SYSTEMAREA_DEFAULT_SCAN_RANGE ; ulVenderUnit++) {
		for (ubBurst = 0;  ubBurst < gubBurstsPerBank; ubBurst++) {
			ubFQIndex = gFQI.ubFQLinkFree;
			FlashQueue_t *FQ;

			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ulRAMAddr = BUFFER3_BASE + 0x4000;
			FQ->btCheckEmpty = 1;
			FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
			FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif
			FQ->uwFUnit = ulVenderUnit;
			if (ENABLE_PLANE_CH_CE) {
				FQ->ulFEntry = (((U32)ubCEIndex << gubBurstsPerBankLog) + ubBurst)  << gub4kEntrysPerPlaneLog;
			}
			else {
				FQ->ulFEntry =  (U32)ubCEIndex << gub4kEntrysPerPlaneLog;
			}
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

			UBYTE ubL4KCount = 0;
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = 0x00000000;
				pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
				pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((BUFFER3_BASE + 0x4000 + (ubL4KCount * 4096)) / 512);
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;


			while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			if ((gubINTTable[FQ->ubDepth].All & 0xF0)) {
				FCTLL[FCTLL_INT_INF + (((ubCEIndex & gubPlanesPerBurstMask) * 0x200) >> 2)] = 0;
				gubINTTable[FQ->ubDepth].All &= 0x0F;
				gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;
				continue;
			}
			gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
			if (pL4KTablePtr->ulL4K_LCA == HEADER_LCA) {
				guwSysCodeBlock[ubSysCodeCount][ubCEIndex] = (UWORD)((ulVenderUnit << gubBurstsPerBankLog) + ubBurst);

				ubSysCodeCount++;
				if (ubSysCodeCount == 0x2) {
					FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
					FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
					FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
					gubStopFTL = 0;
					return ubSysCodeCount;
				}
			}
#if Hynix
			else {
				FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
			}
#endif
		}
	}
	FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
	FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
	FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
	gubStopFTL = 0;
	return ubSysCodeCount;

}
/*
void ftlSet16BitMode()
{

}

void ftlSet8BitMode()
{

}
*/
void Vender_Set_FlashMode(void)
{
	M_CheckRDTModeToDisableFunction();
	UBYTE ubFailCEIndexplus;

	flaSwitchClock(FLH_CLK_40MHz);
	M_FlashModeBackToDefault();

	if (HB[HB_LBA_L]) {
		gubFlashClock = HB[HB_LBA_L];
	}
	else {
		gubFlashClock = FLH_CLK_DEFAULT;
	}

	if (HB[HB_LBA_M]) {
		gubFlashMode = HB[HB_LBA_M];
	}
	else {
		gubFlashMode = FLH_MODE_DEFAULT;
	}

	ubFailCEIndexplus = flaSetFlashMode();
	flaSet_ODT_DRIVE();

	mDMAC_SV(1, (U32)&BUFB_BASE[0], 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	gubHeaderSetFeature = FALSE;
	if (ubFailCEIndexplus) {
		gubHeaderSetFeature = FALSE;
		BUFB_BASE[4] = 0xAA;
		BUFB_BASE[5] = ubFailCEIndexplus - 1;
	}
	else {
		gubCurrentFlashClock = gubFlashClock;
		flaSwitchClock(gubFlashClock);
		gubHeaderSetFeature = TRUE;
		BUFB_BASE[4] = 0xBB;
	}
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}

	Vender_TriggerHostData();
}

void Vender_InitialPadAndDrive()
{
	//this cmd is in burner for scan DLL window initialization.
	M_CheckRDTModeToDisableFunction();

	Vender_TriggerHostData();

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)&BUFB_BASE[0], gulSectorCnt * 512);
	}

	if (BURNER) {
		if (HB[HB_LBA_H] == 0) {
			flaSwitchClock(FLH_CLK_40MHz);
			M_FlashModeBackToDefault();

			gubFlashMode = BUFB_BASE[0];
			if (gubFlashMode == 0) {
				gubFlashMode = FLH_MODE_DEFAULT;
			}

			gubFlashClock = BUFB_BASE[1];
			if (gubFlashClock == 0) {
				gubFlashClock = FLH_CLK_DEFAULT;
			}

			gubFlashODT = BUFB_BASE[2];
			if ((gubFlashODT == 1)
			        || (gubFlashODT == 2)
			        || (gubFlashODT == 3)
			        || (gubFlashODT == 4)) {

				//do nothing
			}
			else if (gubFlashODT == 0xFF) {
				gubFlashODT = 0;
			}
			else {
				gubFlashODT = FLH_ODT_VALUE;
			}

			gubFlashDrive = BUFB_BASE[3];
#if MicronFlashOnly || YMTC
			if ((gubFlashDrive == 1)
			        || (gubFlashDrive == 2)
			        || (gubFlashDrive == 3)) {

				//do nothing
			}
			else {
				gubFlashDrive = FLH_DRIVE_VALUE;
			}
#else
			if ((gubFlashDrive == 2)
			        || (gubFlashDrive == 4)
			        || (gubFlashDrive == 6)) {

				//do nothing
			}
			else {
				gubFlashDrive = FLH_DRIVE_VALUE;
			}
#endif
			gODTSetting.ubODTEnable = BUFB_BASE[4];
			gODTSetting.ubODT = BUFB_BASE[5];

			gDrivingSetting.ubDrivingEnable = BUFB_BASE[6];
			if (gDrivingSetting.ubDrivingEnable == 1) {
				gDrivingSetting.ubALEPads = BUFB_BASE[7];
				gDrivingSetting.ubCLEPads = BUFB_BASE[8];
				gDrivingSetting.ubnWEPads = BUFB_BASE[9];
				gDrivingSetting.ubnREPads = BUFB_BASE[10];
				gDrivingSetting.ubFRDYPads = BUFB_BASE[11];
				gDrivingSetting.ubnWPPads = BUFB_BASE[12];
				gDrivingSetting.ubDQSPads = BUFB_BASE[13];
				gDrivingSetting.ubDataPads = BUFB_BASE[14];
				gDrivingSetting.ubnCEPads = BUFB_BASE[15];
			}
			else {
				gDrivingSetting.ubDrivingEnable = 0;
			}

			gubScanRound = BUFB_BASE[16];
			if (gubScanRound == 0) {
				gubScanRound = 5;
			}

			gubScanInterval = BUFB_BASE[17];
			if (gubScanInterval == 0) {
				gubScanInterval = 50;
			}

			guwScanMaxLDPCErrorBit = BUFW_BASE[18 >> 1];
			if (guwScanMaxLDPCErrorBit == 0) {
				guwScanMaxLDPCErrorBit = SCAN_LDPC_THRESHOLD;
			}

			if (BUFB_BASE[21] == 1) {
				SYS1L[SYS1L_PAD_FCLE0_CFG] &= ~(BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FCLE1_CFG] &= ~(BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FALE0_CFG] &= ~(BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FALE1_CFG] &= ~(BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FWEB0_CFG] &= ~(BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FWEB1_CFG] &= ~(BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FREB0_CFG] &= ~(BIT16 | BIT17 | BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FREB1_CFG] &= ~(BIT16 | BIT17 | BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FDQS0_CFG] &= ~(BIT16 | BIT17 | BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FDQS1_CFG] &= ~(BIT16 | BIT17 | BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23);
				SYS1L[SYS1L_PAD_FDATA_DVSR0] &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT8 | BIT9 | BIT10 | BIT11 | BIT16 | BIT17 | BIT18 | BIT19 | BIT24 | BIT25 | BIT26 | BIT27);
				SYS1L[SYS1L_PAD_FDATA_DVSR1] &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT8 | BIT9 | BIT10 | BIT11 | BIT16 | BIT17 | BIT18 | BIT19 | BIT24 | BIT25 | BIT26 | BIT27);
				SYS1L[SYS1L_PAD_FDATA_DVSR2] &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT8 | BIT9 | BIT10 | BIT11 | BIT16 | BIT17 | BIT18 | BIT19 | BIT24 | BIT25 | BIT26 | BIT27);
				SYS1L[SYS1L_PAD_FDATA_DVSR3] &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT8 | BIT9 | BIT10 | BIT11 | BIT16 | BIT17 | BIT18 | BIT19 | BIT24 | BIT25 | BIT26 | BIT27);

				SYS1L[SYS1L_PAD_FCLE0_CFG] |= (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FCLE1_CFG] |= (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FALE0_CFG] |= (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FALE1_CFG] |= (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FWEB0_CFG] |= (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FWEB1_CFG] |= (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FREB0_CFG] |= (BUFB_BASE[22] << 16) | (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FREB1_CFG] |= (BUFB_BASE[22] << 16) | (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FDQS0_CFG] |= (BUFB_BASE[22] << 16) | (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FDQS1_CFG] |= (BUFB_BASE[22] << 16) | (BUFB_BASE[22] << 20);
				SYS1L[SYS1L_PAD_FDATA_DVSR0] |= (BUFB_BASE[22] << 0) | (BUFB_BASE[22] << 8) | (BUFB_BASE[22] << 16) | (BUFB_BASE[22] << 24);
				SYS1L[SYS1L_PAD_FDATA_DVSR1] |= (BUFB_BASE[22] << 0) | (BUFB_BASE[22] << 8) | (BUFB_BASE[22] << 16) | (BUFB_BASE[22] << 24);
				SYS1L[SYS1L_PAD_FDATA_DVSR2] |= (BUFB_BASE[22] << 0) | (BUFB_BASE[22] << 8) | (BUFB_BASE[22] << 16) | (BUFB_BASE[22] << 24);
				SYS1L[SYS1L_PAD_FDATA_DVSR3] |= (BUFB_BASE[22] << 0) | (BUFB_BASE[22] << 8) | (BUFB_BASE[22] << 16) | (BUFB_BASE[22] << 24);
			}

			gubEnableOverClocking = BUFB_BASE[23];
			if ((gubEnableOverClocking != 1) && (gubEnableOverClocking != 2))
			{
				gubEnableOverClocking = 0;
			}

			//mSE_COPY((U32)&BUFB_BASE[512], (U32)gubParameterPage, 512);
			memcpy((void *)&gubParameterPage, (void *)&BUFB_BASE[512], 512);

			UBYTE ubSetFlashModeResult = flaSetFlashMode();
			flaSet_ODT_DRIVE();

			if (ubSetFlashModeResult || guwErrorID) {
				UBYTE ubFailMap = 0;
				if (ubSetFlashModeResult) {
					ubFailMap |= BIT0;
				}

				if (guwErrorID) {
					ubFailMap |= BIT1;
				}

				HB[HB_LBA_L] = (0xD0 | ubFailMap);
				gubErrorCode = SET_ABRT;
			}
		}
		else {
			flaSwitchClock(gubFlashClock);
		}
	}
}


void Vender_Scan_SDLL()
{
#if 1
	M_CheckRDTModeToDisableFunction();

	if (BURNER) {
		UBYTE ubDieIndex = HB[HB_LBA_L];
		UBYTE ubChannel = HB[HB_LBA_M];
		UBYTE ubCEIndexInThatChannel = HB[HB_LBA_H];
		UBYTE ubL4KNum = gub4kEntrysPerPlane;
		UBYTE ubDepth, ubDepth2;
		UBYTE ubL4KCount = 0;
		volatile L4KTable16B *pL4KTable;
		UBYTE ubCENum = (gubExpandCE) ? gubMTQTotalPhyCE : gubCENumber;
		UBYTE ubDieNum = (gubExpandCE) ? (gubDieNumber * gubExpandCE_Ratio) : gubDieNumber;
		UBYTE ubCEPerChannel = (ubCENum >> gubPlanesPerBurstLog);
		UWORD uwWindowMax[2][MAX_CHANNEL];
		UWORD uwWindowMin[2][MAX_CHANNEL];
		UBYTE ubCmpResult[MAX_CHANNEL];
		UWORD uwInterval[MAX_CHANNEL];
		UBYTE ubStopScan[MAX_CHANNEL];
		UBYTE ubChannelHang[MAX_CHANNEL];
		UBYTE ubi, ubj;
		UWORD uwi;
		UBYTE *ubMicroCodeBuffer = (UBYTE *)gMicroCodeBuffer;
		UWORD uwHWSDLL[2][MAX_CHANNEL];
		UWORD uwCount;
		UWORD uwPollingCount = 40000;
		UWORD uwMaxNotPassWindowSize[MAX_CHANNEL];
		UWORD uwMaxNotPassWindowMin[MAX_CHANNEL];
		UWORD uwMaxNotPassWindowMax[MAX_CHANNEL];

		if (ENABLE_DEBUG_UART_SCAN_WINDOW) {
			UartString("\r\ndie: ");
			Uart_Tx_DataHex(ubDieIndex);
			UartString(" ubCH: ");
			Uart_Tx_DataHex(ubChannel);
			UartString(" ubBank: ");
			Uart_Tx_DataHex(ubCEIndexInThatChannel);
		}
		if (ENABLE_SANDISK_1ZNM_SPF) {
			if ((ubCEIndexInThatChannel >= gubHideCE) && (ubDieIndex == 2)) {
				ubDieIndex -= 2;
			}
		}
		for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
			for (ubj = 0; ubj < 2; ubj++) {
				uwWindowMin[ubj][ubi] = 0xFFFF;
				uwWindowMax[ubj][ubi] = 0xFFFF;
			}
			uwInterval[ubi] = 0;
			ubCmpResult[ubi] = 0;
			ubStopScan[ubi] = 0;
			uwMaxNotPassWindowSize[ubi] = 0;
			uwMaxNotPassWindowMin[ubi] = 0xFFFF;
			uwMaxNotPassWindowMax[ubi] = 0xFFFF;
			ubChannelHang[ubi] = 0;
		}

		if ((ubChannel != 0) && (ubChannel != 1)) {
			if (gubPlanesPerBurst < MAX_CHANNEL) {
				ubChannel = 0;
			}
			else {
				ubChannel = 2;
			}
		}

#pragma flush
		while ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0); //Philip said ASIC need not poll 8 times
		idle(0x20);

		for (uwi = 0; uwi < (gubSectorsPerPlane * 512); uwi++) {
			ubMicroCodeBuffer[uwi] = gubParameterPage[uwi % 482];
		}

		Vender_TriggerHostData();

#if ENABLE_ODD_CE
		UBYTE ubASymmetricCE = 0;
		if ((gubCENumber & BIT0) && (gubPlanesPerBurst > 1)) {
			ubCEPerChannel ++;
			ubASymmetricCE = 1;
		}

		if ( ubASymmetricCE  && (ubCEIndexInThatChannel == ubCEPerChannel - 1) ) {
			if (ubChannel & gubPlanesPerBurstMask) {
				if (ENABLE_DEBUG_UART_SCAN_WINDOW) {
					UartString("\nODD_CE no CE");
					UartString("\nABRT");
				}
				gubErrorCode = SET_ABRT;
				return;
			}
			else if (ubChannel == 2) {
				ubChannel = 0 ;
				if (ENABLE_DEBUG_UART_SCAN_WINDOW) {
					UartString("\nODD_CE ch1 no CE");            ;
				}
			}
		}

#endif

		if ((ubCEIndexInThatChannel >= ubCEPerChannel)
		        || (((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) && (ubDieIndex >= ubDieNum))) {

			gubErrorCode = SET_ABRT;
			return;
		}

		mSE_SV(4, (U32)&BUFB_BASE[0], 16384, 0xFFFFFFFF , 0);

		if (ubChannel != 2) {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
		}
		else {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + FALL);
		}

		U32 ulIFSA0_Temp = gFREG[FCTLL_IFSA0].L;
		U32 ulFSA_SEL_Temp = gFREG[FCTLL_FSA_SEL].L;
		U32 ulZIP_CFG_Temp = gFREG[FCTLL_ZIP_CFG].L;
		U32 ulRS_CFG_Temp = gFREG[FCTLL_RS_CFG].L;
		U32 ulDMA_CFG_Temp = gFREG[FCTLL_DMA_CFG].L;
		U32 ulINT_VCT_Temp = gFREG[FCTLL_INT_VCT].L;
		U32 ulCHNL_SET_Temp = gFREG[FCTLL_CHNL_SET].L;
		U32 ulL4K_SPR_ADR_Temp = gFREG[FCTLL_L4K_SPR_ADR].L;

		if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
			gFREG[FCTLL_IFSA0].L = (((ubDieIndex * guwDieBlockShift) & 0xFFFF) << 16);
			if (ENABLE_SANDISK_1ZNM_SPF) {
				if (ubCEIndexInThatChannel >= gubHideCE) {
					gFREG[FCTLL_IFSA0].L += ((24576 >> gubBurstsPerBankLog) << 16);
				}
			}
		}
		else {
			gFREG[FCTLL_IFSA0].L = 0x00000000;
		}
		gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;

		gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
		gFREG[FCTLL_RS_CFG].L &= (~RS_ON_THE_FLY_EN);

		gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
		gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubL4KNum);

		gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;

		gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);

		gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME) ;
		gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubL4KNum) << 8);

		gFREG[FCTLL_CHNL_SET].L |= (BYPASS_CONV);

		if (ubChannel != 2) {
			gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_OFFSET;
		}
		else {
			FCTLL[FCTLL_L4K_SPR_ADR] = HB_RETRY_L4K_TABLE_OFFSET;
			FCTLL[FCTLL_L4K_SPR_ADR + (0x200 >> 2)] = FSA_BASE;
		}

		UBYTE ubScanWindowType;
		UBYTE ubScanRoundIndex;
		UWORD uwSDLLvalue;
		U32 ulFDLLL_DLL_CTL_R_Temp = FDLLL[FDLLL_DLL_CTL];
		U32 ulFDLLL_DLL_CTL_W_Temp = FDLLL[FDLLL_DLL_CTL + (0x200 >> 2)];
		U32 ulFDLLL_SEL_VAL_CH0_R_Temp = FDLLL[FDLLL_SEL_VAL_0];
		U32 ulFDLLL_SEL_VAL_CH1_R_Temp = FDLLL[FDLLL_SEL_VAL_1];
		U32 ulFDLLL_SEL_VAL_CH0_W_Temp = FDLLL[FDLLL_SEL_VAL_0 + (0x200 >> 2)];
		U32 ulFDLLL_SEL_VAL_CH1_W_Temp = FDLLL[FDLLL_SEL_VAL_1 + (0x200 >> 2)];

		flaSwitchClock(FLH_CLK_40MHz);
		//---------------------  gen data  ------------------------------------//
		for (ubScanWindowType = 0; ubScanWindowType < 2; ubScanWindowType++) { //0: scan read window, 1: scan write window
			ubStopScan[0] = 0;
			ubStopScan[1] = 0;
			uwInterval[0] = 0;
			uwInterval[1] = 0;

			if (ubScanWindowType == 1) {
				flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL + ubScanWindowType * (0x200 >> 2), (FDLLL[FDLLL_DLL_CTL + ubScanWindowType * (0x200 >> 2)] & FLH_STROBE_MODE));
				flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL + ubScanWindowType * (0x200 >> 2), (FDLLL[FDLLL_DLL_CTL + ubScanWindowType * (0x200 >> 2)] | FW_CTL_SDLL_MODE));
			}

			for (uwSDLLvalue = 0; uwSDLLvalue < 512; uwSDLLvalue++) { //0~511
				//---------------------  tuning SDLL  ------------------------------------//
				if (ubChannel != 2) {
					if (ubChannel == 0) {
						flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_0 + ((ubScanWindowType * 0x200) >> 2), (ubScanWindowType ? (SDLL_VALUE_WRITE_CH0(uwSDLLvalue)) : (SDLL_VALUE_READ_CH0(uwSDLLvalue))));
					}
					else {
						flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_1 + ((ubScanWindowType * 0x200) >> 2), (ubScanWindowType ? (SDLL_VALUE_WRITE_CH0(uwSDLLvalue)) : (SDLL_VALUE_READ_CH0(uwSDLLvalue))));
					}
				}
				else {
					flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_0 + ((ubScanWindowType * 0x200) >> 2), (ubScanWindowType ? (SDLL_VALUE_WRITE_CH0(uwSDLLvalue)) : (SDLL_VALUE_READ_CH0(uwSDLLvalue))));
					flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_1 + ((ubScanWindowType * 0x200) >> 2), (ubScanWindowType ? (SDLL_VALUE_WRITE_CH0(uwSDLLvalue)) : (SDLL_VALUE_READ_CH0(uwSDLLvalue))));
				}

				ubCmpResult[0] = 0;
				ubCmpResult[1] = 0;
				for (ubScanRoundIndex = 0; ubScanRoundIndex < gubScanRound; ubScanRoundIndex++) {
					if (ENABLE_DEBUG_UART_SCAN_WINDOW) {
						UartString("\r\nrw:");
						Uart_Tx_DataHex(ubScanWindowType);
						UartString(" SDLL:");
						UartWordHex(uwSDLLvalue);
						UartString(" r:");
						Uart_Tx_DataHex(ubScanRoundIndex);
					}
					if (((ubScanWindowType == 0) && (uwSDLLvalue == 0) && (ubScanRoundIndex == 0)) || (ubScanWindowType == 1)) {
						if (ENABLE_DEBUG_UART_SCAN_WINDOW) {
							UartString(" W ");
						}
						//---------------------  write  ------------------------------------//
						M_GetMTDepth(ubDepth);
						if (ubChannel == 2) {
							M_GetMTDepth(ubDepth2);
						}

						gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
						if (ubChannel != 2) {
							gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
						}
						else {
							FCTLL[FCTLL_INT_VCT] |= IntVector(ubDepth);
							FCTLL[FCTLL_INT_VCT + (0x200 >> 2)] |= IntVector(ubDepth2);
						}

						if (ubChannel != 2) {
							for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
								pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
								//set L4K table info
								pL4KTable->ulL4K_LCA = 0x00000000;
								pL4KTable->Para0x04.ulAll = 0;
								pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
								pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
								pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)ubMicroCodeBuffer + ubL4KCount * 4096) / 512;
								pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
							}
						}
						else {
							for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
								pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
								//set L4K table info
								pL4KTable->ulL4K_LCA = 0x00000000;
								pL4KTable->Para0x04.ulAll = 0;
								pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
								pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
								pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)ubMicroCodeBuffer + ubL4KCount * 4096) / 512;
								pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
							}

							for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
								pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + FSA_BASE + (ubL4KCount * Spare4kSize));
								//set L4K table info
								pL4KTable->ulL4K_LCA = 0x00000000;
								pL4KTable->Para0x04.ulAll = 0;
								pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
								pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
								pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)ubMicroCodeBuffer + ubL4KCount * 4096) / 512;
								pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
							}
						}

						if (ubChannel != 2) {
							if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
								FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubCEIndexInThatChannel]);
							}
							else {
								FCONL[FCONL_FCE_ENB] = (BIT0 << ((ubChannel * MAX_CE_PER_CH) + ubCEIndexInThatChannel));
							}
						}
						else {
							if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
								FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubCEIndexInThatChannel]);
								FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubCEIndexInThatChannel]);
							}
							else {
								FCONL[FCONL_FCE_ENB] = ((BIT0 | BIT8) << ubCEIndexInThatChannel);
							}
						}

						gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_CA2_C80_A5_DW;
						gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;

						if (ubChannel != 2) {
							uwCount = 0;
							while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
								uwCount++;
								if (uwCount > uwPollingCount) {
									ubChannelHang[ubChannel] = 0xA0 + ubChannel;
									break;
								}
							}
						}
						else {
							uwCount = 0;
							while ((FCTLL[FCTLL_FPU_TRIG] & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
								uwCount++;
								if (uwCount > uwPollingCount) {
									ubChannelHang[0] = 0xA0;
									break;
								}
							}

							uwCount = 0;
							while ((FCTLL[FCTLL_FPU_TRIG + (0x200 >> 2)] & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
								uwCount++;
								if (uwCount > uwPollingCount) {
									ubChannelHang[1] = 0xA1;
									break;
								}
							}
						}

						if (ENABLE_DEBUG_UART_VENDER) {
							UartString("\r\nFCTLL_INT_INF: ");
							UartLongHex(gFREG[FCTLL_INT_INF].L);
						}

						// reassign CE,  MT stop後, 當MT preload + FPU read後會clear CE, 所以上面FPU trig後, CE有可能會被清掉, 這邊需再set CE一次, 之後無MT preload行為就不需要再重設
						if (ubChannel != 2) {
							if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
								FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubCEIndexInThatChannel]);
							}
							else {
								FCONL[FCONL_FCE_ENB] = (BIT0 << ((ubChannel * MAX_CE_PER_CH) + ubCEIndexInThatChannel));
							}
						}
						else {
							if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
								FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubCEIndexInThatChannel]);
								FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubCEIndexInThatChannel]);
							}
							else {
								FCONL[FCONL_FCE_ENB] = ((BIT0 | BIT8) << ubCEIndexInThatChannel);
							}
						}

#if (MicronFlashOnly || Hynix || YMTC)
						gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40;
#else	//TSB flash only
						gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
#endif
						gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;

						if (ubChannel != 2) {
							uwCount = 0;
							while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
								uwCount++;
								if (uwCount > uwPollingCount) {
									ubChannelHang[ubChannel] = 0xB0 + ubChannel;
									break;
								}
							}
						}
						else {
							uwCount = 0;
							while ((FCTLL[FCTLL_FPU_TRIG] & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
								uwCount++;
								if (uwCount > uwPollingCount) {
									ubChannelHang[0] = 0xB0;
									break;
								}
							}

							uwCount = 0;
							while ((FCTLL[FCTLL_FPU_TRIG + (0x200 >> 2)] & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
								uwCount++;
								if (uwCount > uwPollingCount) {
									ubChannelHang[1] = 0xB1;
									break;
								}
							}
						}

#if !(MicronFlashOnly || Hynix) //TSB flash only
						// Polling TRUE RDY
						gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
						gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;

						if (ubChannel != 2) {
							uwCount = 0;
							while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
								uwCount++;
								if (uwCount > uwPollingCount) {
									ubChannelHang[ubChannel] = 0xB0 + ubChannel;
									break;
								}
							}
						}
						else {
							uwCount = 0;
							while ((FCTLL[FCTLL_FPU_TRIG] & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
								uwCount++;
								if (uwCount > uwPollingCount) {
									ubChannelHang[0] = 0xB0;
									break;
								}
							}

							uwCount = 0;
							while ((FCTLL[FCTLL_FPU_TRIG + (0x200 >> 2)] & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
								uwCount++;
								if (uwCount > uwPollingCount) {
									ubChannelHang[1] = 0xB1;
									break;
								}
							}
						}

#endif

						// need clean msg dispatch
						gubINTTable[ubDepth].All = 0;
						M_AddMTDepth(ubDepth);

						if (ubChannel == 2) {
							gubINTTable[ubDepth2].All = 0;
							M_AddMTDepth(ubDepth2);
						}
					}
					//---------------------  read  ------------------------------------//
					if (ENABLE_DEBUG_UART_SCAN_WINDOW) {
						UartString(" R ");
					}
					if ((ubScanWindowType == 0) && (uwSDLLvalue == 0) && (ubScanRoundIndex == 0)) {
						flaSwitchClock(gubFlashClock);
						uwHWSDLL[0][0] = (UWORD)(FDLLL[FDLLL_SDLL_VAL_0] & 0x000001FF);
						uwHWSDLL[1][0] = (UWORD)((FDLLL[FDLLL_SDLL_VAL_0 + (0x200 >> 2)] & 0x01FF0000) >> 16);
						uwHWSDLL[0][1] = (UWORD)(FDLLL[FDLLL_SDLL_VAL_1] & 0x000001FF);
						uwHWSDLL[1][1] = (UWORD)((FDLLL[FDLLL_SDLL_VAL_1 + (0x200 >> 2)] & 0x01FF0000) >> 16);
						flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL, (FDLLL[FDLLL_DLL_CTL] & FLH_STROBE_MODE));
						flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL, (FDLLL[FDLLL_DLL_CTL] | FW_CTL_SDLL_MODE));
					}

					M_GetMTDepth(ubDepth);
					if (ubChannel == 2) {
						M_GetMTDepth(ubDepth2);
					}

					if (ubChannel != 2) {
						gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
					}
					else {
						FCTLL[FCTLL_INT_VCT] |= IntVector(ubDepth);
						FCTLL[FCTLL_INT_VCT + (0x200 >> 2)] |= IntVector(ubDepth2);
					}

					if (ubChannel != 2) {
						//mSE_SV(4, (U32)&ubMicroCodeBuffer[0x4000], gubSectorsPerPlane * 512, 0xFFFFFFFF , 0);
						for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
							pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
							//set L4K table info
							pL4KTable->ulL4K_LCA = 0x00000000;
							pL4KTable->Para0x04.ulAll = 0;
							pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
							pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
							pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)ubMicroCodeBuffer + 0x4000 + ubL4KCount * 4096) / 512;
							pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
						}
					}
					else {
						//mSE_SV(4, (U32)&ubMicroCodeBuffer[0x4000], gubSectorsPerPlane * 512, 0xFFFFFFFF , 0);
						for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
							pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
							//set L4K table info
							pL4KTable->ulL4K_LCA = 0x00000000;
							pL4KTable->Para0x04.ulAll = 0;
							pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
							pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
							pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)ubMicroCodeBuffer + 0x4000 + ubL4KCount * 4096) / 512;
							pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
						}

						//mSE_SV(4, (U32)&ubMicroCodeBuffer[0x8000], gubSectorsPerPlane * 512, 0xFFFFFFFF , 0);
						for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
							pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + FSA_BASE + (ubL4KCount * Spare4kSize));
							//set L4K table info
							pL4KTable->ulL4K_LCA = 0x00000000;
							pL4KTable->Para0x04.ulAll = 0;
							pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
							pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
							pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)ubMicroCodeBuffer + 0x8000 + ubL4KCount * 4096) / 512;
							pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
						}
					}

					if (ubChannel != 2) {
						if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
							FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubCEIndexInThatChannel]);
						}
						else {
							FCONL[FCONL_FCE_ENB] = (BIT0 << ((ubChannel * MAX_CE_PER_CH) + ubCEIndexInThatChannel));
						}
					}
					else {
						if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
							FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubCEIndexInThatChannel]);
							FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubCEIndexInThatChannel]);
						}
						else {
							FCONL[FCONL_FCE_ENB] = ((BIT0 | BIT8) << ubCEIndexInThatChannel);
						}
					}

#if (MicronFlashOnly || Hynix || YMTC)
					gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR;
#else	//TSB flash only
					gFREG[FCTLL_FPU_ENTRY].L = RAM_SEQ_PTR16(FPU_AG_C05_A5_CE0_DR);
#endif
					gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;

					if (ubChannel != 2) {
						uwCount = 0;
						while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
							uwCount++;
							if (uwCount > uwPollingCount) {
								ubChannelHang[ubChannel] = 0xC0 + ubChannel;
								break;
							}
						}
					}
					else {
						uwCount = 0;
						while ((FCTLL[FCTLL_FPU_TRIG] & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
							uwCount++;
							if (uwCount > uwPollingCount) {
								ubChannelHang[0] = 0xC0;
								break;
							}
						}

						uwCount = 0;
						while ((FCTLL[FCTLL_FPU_TRIG + (0x200 >> 2)] & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {
							uwCount++;
							if (uwCount > uwPollingCount) {
								ubChannelHang[1] = 0xC1;
								break;
							}
						}
					}

					//clear interrupt
					if (ENABLE_DEBUG_UART_VENDER) {
						UartString(" INT0:");
						UartLongHex(FCTLL[FCTLL_INT_INF]);
					}

					//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
					gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
					if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
						gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
					}

					// need clean msg dispatch
					gubINTTable[ubDepth].All = 0;
					M_AddMTDepth(ubDepth);

					if (ubChannel == 2) {
						gubINTTable[ubDepth2].All = 0;
						M_AddMTDepth(ubDepth2);
					}

					if ((ubChannelHang[0]) || (ubChannelHang[1])) {
						break;
					}
					//---------------------  compare  ------------------------------------//
					if (ENABLE_DEBUG_UART_SCAN_WINDOW) {
						UartString(" C ");
					}

					if (ubChannel != 2) {
						if (ubScanRoundIndex == 0) {
							BUFW_BASE[2 + 256 + ((MAX_CHANNEL * ubChannel + ubScanWindowType) << 9) + uwSDLLvalue] = gFREG[FCTLL_ECC_INFO].L;
						}

						if (gFREG[FCTLL_ECC_INFO].L >= guwScanMaxLDPCErrorBit) {
							ubCmpResult[ubChannel] = 1;
						}
						//ubCmpResult[0] = mDMAC_CMP((U32)&ubMicroCodeBuffer[0], (U32)&ubMicroCodeBuffer[0x4000], gubSectorsPerPlane * 512);

						if (ENABLE_DEBUG_UART_SCAN_WINDOW) {
							UartString(" C0 ");
							Uart_Tx_DataHex(ubCmpResult[ubChannel]);
							UartString(" E0 ");
							UartLongHex(gFREG[FCTLL_ECC_INFO].L);
						}

						if (ubCmpResult[ubChannel]) {
							break;//try next SDLL value.
						}
					}
					else {
						if (ubScanRoundIndex == 0) {
							BUFW_BASE[2 + 256 + ((MAX_CHANNEL * 0 + ubScanWindowType) << 9) + uwSDLLvalue] = FCTLL[FCTLL_ECC_INFO];
							BUFW_BASE[2 + 256 + ((MAX_CHANNEL * 1 + ubScanWindowType) << 9) + uwSDLLvalue] = FCTLL[FCTLL_ECC_INFO + (0x200 >> 2)];
						}

						if (FCTLL[FCTLL_ECC_INFO] >= guwScanMaxLDPCErrorBit) {
							ubCmpResult[0] += 1;
						}

						if (FCTLL[FCTLL_ECC_INFO + (0x200 >> 2)] >= guwScanMaxLDPCErrorBit) {
							ubCmpResult[1] += 1;
						}
						//ubCmpResult[0] += mDMAC_CMP((U32)&ubMicroCodeBuffer[0], (U32)&ubMicroCodeBuffer[0x4000], gubSectorsPerPlane * 512);
						//ubCmpResult[1] += mDMAC_CMP((U32)&ubMicroCodeBuffer[0], (U32)&ubMicroCodeBuffer[0x8000], gubSectorsPerPlane * 512);

						if (ENABLE_DEBUG_UART_SCAN_WINDOW) {
							UartString(" C0 ");
							Uart_Tx_DataHex(ubCmpResult[0]);
							UartString(" E0 ");
							UartLongHex(FCTLL[FCTLL_ECC_INFO]);
							UartString(" C1 ");
							Uart_Tx_DataHex(ubCmpResult[1]);
							UartString(" E1 ");
							UartLongHex(FCTLL[FCTLL_ECC_INFO + (0x200 >> 2)]);
						}

						if ((ubCmpResult[0] != 0) && (ubCmpResult[1] != 0)) {
							break;//try next SDLL value.
						}
					}

				}

				if ((ubChannelHang[0]) || (ubChannelHang[1])) {
					if (ubChannel != 2) {
						BUFW_BASE[2 + 256 + ((MAX_CHANNEL * ubChannel + ubScanWindowType) << 9) + uwSDLLvalue] = (ubChannelHang[ubChannel] != 0) ? 0x55AA : gFREG[FCTLL_ECC_INFO].L;
					}
					else {
						BUFW_BASE[2 + 256 + ((MAX_CHANNEL * 0 + ubScanWindowType) << 9) + uwSDLLvalue] = (ubChannelHang[0] != 0) ? 0x55AA : FCTLL[FCTLL_ECC_INFO];
						BUFW_BASE[2 + 256 + ((MAX_CHANNEL * 1 + ubScanWindowType) << 9) + uwSDLLvalue] = (ubChannelHang[1] != 0) ? 0x55AA : FCTLL[FCTLL_ECC_INFO + (0x200 >> 2)];
					}

					break;
				}

				if (ubChannel != 2) {
					if (ubStopScan[ubChannel] == 0) {
						if (ubCmpResult[ubChannel]) { //compare fail
							if (uwInterval[ubChannel] > gubScanInterval) {
								ubStopScan[ubChannel] = 1; //window passed
							}
							else {
								if (uwInterval[ubChannel] > uwMaxNotPassWindowSize[ubChannel]) {
									uwMaxNotPassWindowSize[ubChannel] = uwInterval[ubChannel];
									uwMaxNotPassWindowMin[ubChannel] = uwWindowMin[ubScanWindowType][ubChannel];
									uwMaxNotPassWindowMax[ubChannel] = uwWindowMax[ubScanWindowType][ubChannel];
								}
								uwInterval[ubChannel] = 0; //find next window
							}
						}
						else { //compare pass
							uwInterval[ubChannel]++;
							if (uwInterval[ubChannel] == 1) {
								uwWindowMin[ubScanWindowType][ubChannel] = uwSDLLvalue;
							}

							uwWindowMax[ubScanWindowType][ubChannel] = uwSDLLvalue;

							if (uwInterval[ubChannel] > gubScanInterval) {
								if (uwSDLLvalue == 511) {
									ubStopScan[ubChannel] = 1; //window passed
								}
							}
							else {
								if ((uwInterval[ubChannel] > uwMaxNotPassWindowSize[ubChannel]) && (uwSDLLvalue == 511)) {
									uwMaxNotPassWindowSize[ubChannel] = uwInterval[ubChannel];
									uwMaxNotPassWindowMin[ubChannel] = uwWindowMin[ubScanWindowType][ubChannel];
									uwMaxNotPassWindowMax[ubChannel] = uwWindowMax[ubScanWindowType][ubChannel];
								}
							}
						}
					}
				}
				else {
					for (ubi = 0; ubi < 2; ubi++) {
						if (ubStopScan[ubi] == 0) {
							if (ubCmpResult[ubi]) { //compare fail
								if (uwInterval[ubi] > gubScanInterval) {
									ubStopScan[ubi] = 1; //window passed, all pass case
								}
								else {
									if (uwInterval[ubi] > uwMaxNotPassWindowSize[ubi]) {
										uwMaxNotPassWindowSize[ubi] = uwInterval[ubi];
										uwMaxNotPassWindowMin[ubi] = uwWindowMin[ubScanWindowType][ubi];
										uwMaxNotPassWindowMax[ubi] = uwWindowMax[ubScanWindowType][ubi];
									}
									uwInterval[ubi] = 0; //find next window
								}
							}
							else { //compare pass
								uwInterval[ubi]++;
								if (uwInterval[ubi] == 1) {
									uwWindowMin[ubScanWindowType][ubi] = uwSDLLvalue;
								}

								uwWindowMax[ubScanWindowType][ubi] = uwSDLLvalue;

								if (uwInterval[ubi] > gubScanInterval) {
									if (uwSDLLvalue == 511) {
										ubStopScan[ubi] = 1; //window passed, all pass case
									}
								}
								else {
									if ((uwInterval[ubi] > uwMaxNotPassWindowSize[ubi]) && (uwSDLLvalue == 511)) {
										uwMaxNotPassWindowSize[ubi] = uwInterval[ubi];
										uwMaxNotPassWindowMin[ubi] = uwWindowMin[ubScanWindowType][ubi];
										uwMaxNotPassWindowMax[ubi] = uwWindowMax[ubScanWindowType][ubi];
									}
								}
							}
						}
					}
				}

				if (ubChannel != 2) {
					if (ubStopScan[ubChannel] == 1) {
						uwSDLLvalue = (uwWindowMin[ubScanWindowType][ubChannel] + uwWindowMax[ubScanWindowType][ubChannel]) / 2;
						if (ubChannel == 0) {
							flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_0 + ((ubScanWindowType * 0x200) >> 2), (ubScanWindowType ? (SDLL_VALUE_WRITE_CH0(uwSDLLvalue)) : (SDLL_VALUE_READ_CH0(uwSDLLvalue))));
						}
						else {
							flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_1 + ((ubScanWindowType * 0x200) >> 2), (ubScanWindowType ? (SDLL_VALUE_WRITE_CH0(uwSDLLvalue)) : (SDLL_VALUE_READ_CH0(uwSDLLvalue))));
						}
						break;
					}
				}
				else {
					if ((ubStopScan[0] == 1) && (ubStopScan[1] == 1)) { //both window has been found, find next scan window type.
						uwSDLLvalue = (uwWindowMin[ubScanWindowType][0] + uwWindowMax[ubScanWindowType][0]) / 2;
						flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_0 + ((ubScanWindowType * 0x200) >> 2), (ubScanWindowType ? (SDLL_VALUE_WRITE_CH0(uwSDLLvalue)) : (SDLL_VALUE_READ_CH0(uwSDLLvalue))));

						uwSDLLvalue = (uwWindowMin[ubScanWindowType][1] + uwWindowMax[ubScanWindowType][1]) / 2;
						flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_1 + ((ubScanWindowType * 0x200) >> 2), (ubScanWindowType ? (SDLL_VALUE_WRITE_CH1(uwSDLLvalue)) : (SDLL_VALUE_READ_CH1(uwSDLLvalue))));

						break;
					}
				}
			}

			if (ubChannel != 2) {
				if (ubStopScan[ubChannel] == 0) {
					if (ubChannelHang[ubChannel]) {
						uwWindowMin[ubScanWindowType][ubChannel] = 0xB000;
						uwWindowMax[ubScanWindowType][ubChannel] = ubChannelHang[ubChannel];
					}
					else {
						uwWindowMin[ubScanWindowType][ubChannel] = 0xA000 + uwMaxNotPassWindowMin[ubChannel];
						uwWindowMax[ubScanWindowType][ubChannel] = uwMaxNotPassWindowSize[ubChannel];
					}

					break;//find nobody in any window type, or trigger hang up, then will stop this scan.
				}
			}
			else {
				if ((ubStopScan[0] == 0) || (ubStopScan[1] == 0)) {
					if (ubStopScan[0] == 0) {
						if (ubChannelHang[0]) {
							uwWindowMin[ubScanWindowType][0] = 0xB000;
							uwWindowMax[ubScanWindowType][0] = ubChannelHang[0];
						}
						else {
							uwWindowMin[ubScanWindowType][0] = 0xA000 + uwMaxNotPassWindowMin[0];
							uwWindowMax[ubScanWindowType][0] = uwMaxNotPassWindowSize[0];
						}
					}

					if (ubStopScan[1] == 0) {
						if (ubChannelHang[1]) {
							uwWindowMin[ubScanWindowType][1] = 0xB000;
							uwWindowMax[ubScanWindowType][1] = ubChannelHang[1];
						}
						else {
							uwWindowMin[ubScanWindowType][1] = 0xA000 + uwMaxNotPassWindowMin[1];
							uwWindowMax[ubScanWindowType][1] = uwMaxNotPassWindowSize[1];
						}
					}

					break;//find nobody in any window type, or trigger hang up, then will stop this scan.
				}
			}
		}

		//---------------------  end  ------------------------------------//
		FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs

		gFREG[FCTLL_IFSA0].L = ulIFSA0_Temp;
		gFREG[FCTLL_FSA_SEL].L = ulFSA_SEL_Temp;
		gFREG[FCTLL_ZIP_CFG].L = ulZIP_CFG_Temp;
		gFREG[FCTLL_RS_CFG].L = ulRS_CFG_Temp;
		gFREG[FCTLL_DMA_CFG].L = ulDMA_CFG_Temp;
		gFREG[FCTLL_INT_VCT].L = ulINT_VCT_Temp;
		gFREG[FCTLL_CHNL_SET].L = ulCHNL_SET_Temp;
		gFREG[FCTLL_L4K_SPR_ADR].L = ulL4K_SPR_ADR_Temp;

		flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL, ulFDLLL_DLL_CTL_R_Temp);
		flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL + (0x200 >> 2), ulFDLLL_DLL_CTL_W_Temp);
		flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_0, ulFDLLL_SEL_VAL_CH0_R_Temp);
		flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_1, ulFDLLL_SEL_VAL_CH1_R_Temp);
		flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_0 + (0x200 >> 2), ulFDLLL_SEL_VAL_CH0_W_Temp);
		flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_1 + (0x200 >> 2), ulFDLLL_SEL_VAL_CH1_W_Temp);

#pragma flush
		while ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0); //Philip said ASIC need not poll 8 times
		idle(0x20);

		BUFL_BASE[0] = BUFFER3_BASE + 4;
		BUFW_BASE[2] = uwWindowMin[0][0];
		BUFW_BASE[3] = uwWindowMax[0][0];
		BUFW_BASE[4] = uwWindowMin[1][0];
		BUFW_BASE[5] = uwWindowMax[1][0];
		BUFW_BASE[6] = uwWindowMin[0][1];
		BUFW_BASE[7] = uwWindowMax[0][1];
		BUFW_BASE[8] = uwWindowMin[1][1];
		BUFW_BASE[9] = uwWindowMax[1][1];
		BUFW_BASE[10] = uwHWSDLL[0][0];
		BUFW_BASE[11] = uwHWSDLL[1][0];
		BUFW_BASE[12] = uwHWSDLL[0][1];
		BUFW_BASE[13] = uwHWSDLL[1][1];

		if (ENABLE_DEBUG_UART_SCAN_WINDOW || 1) {
			for (ubi = 0; ubi < 12; ubi++) {
				UartString("\r\n");
				UartWordHex(BUFW_BASE[ubi + 2]);
			}
		}
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
		}
	}
	else {
		Vender_TriggerHostData();
	}
#endif
}

void Vender_Cache_Program()
{
#if 1
	M_CheckRDTModeToDisableFunction();

	if (BURNER) {
		UBYTE ubDieIndex = HB[HB_LBA_L];
		UBYTE ubChannel = HB[HB_LBA_M];
		UBYTE ubCEIndexInThatChannel = HB[HB_LBA_H];
		UBYTE ubL4KNum = gub4kEntrysPerPlane;
		UBYTE ubDepth, ubDepth2;
		UBYTE ubL4KCount = 0;
		volatile L4KTable16B *pL4KTable;
		U32 ulECC_Temp = SYS1L[SYS1L_ECC_EN];
		UBYTE ubCENum = (gubExpandCE) ? gubMTQTotalPhyCE : gubCENumber;
		UBYTE ubDieNum = (gubExpandCE) ? (gubDieNumber * gubExpandCE_Ratio) : gubDieNumber ;

		U32 ulHL_BUF_R_EP_Temp = HL[HL_BUF_R_EP];
		U32 ulHL_BUF_W_EP_Temp = HL[HL_BUF_W_EP];

		HL[HL_BUF_R_EP] = 8;
		HL[HL_BUF_W_EP] = 8;

		if (ENABLE_DEBUG_UART_VENDER) {
			UartString("\r\nW: ");
			//Uart_Tx_DataHex(ubDieIndex);
			UartString(" ubCH: ");
			Uart_Tx_DataHex(ubChannel);
			UartString(" ubBank: ");
			Uart_Tx_DataHex(ubCEIndexInThatChannel);
			UartString(" 9A20: 0x");
			UartLongHex(FDLLL[0x220 / 4]);
			UartString(" 9A24: 0x");
			UartLongHex(FDLLL[0x224 / 4]);
		}

#pragma flush
		while ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0); //Philip said ASIC need not poll 8 times
		SYS1L[SYS1L_ECC_EN] &= ~(CR_Z1_ECC_EN | CR_Z2_ECC_EN | CR_Z3_ECC_EN);
		idle(0x20);
		mSE_SV(4, BUFFER3_BASE, gulSectorCnt * 512, 0 , 0);

		Vender_TriggerHostData();

		if (((ubChannel >= gubPlanesPerBurst) && (ubChannel != 0xFF))
		        || (ubCEIndexInThatChannel >= (ubCENum >> gubPlanesPerBurstLog))
		        || (((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) && (ubDieIndex >= ubDieNum))) {

			gubErrorCode = SET_ABRT;
			return;
		}
		if (ENABLE_SANDISK_1ZNM_SPF) {
			if ((ubCEIndexInThatChannel >= gubHideCE) && (ubDieIndex == 2)) {
				ubDieIndex -= 2;
			}
		}

		M_GetMTDepth(ubDepth);
		if (ubChannel != 0xFF) {
			M_GetMTDepth(ubDepth2);
		}

		if (ubChannel != 0xFF) {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
		}
		else {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + FALL);
		}

		U32 ulIFSA0_Temp = gFREG[FCTLL_IFSA0].L;
		U32 ulFSA_SEL_Temp = gFREG[FCTLL_FSA_SEL].L;
		U32 ulZIP_CFG_Temp = gFREG[FCTLL_ZIP_CFG].L;
		U32 ulRS_CFG_Temp = gFREG[FCTLL_RS_CFG].L;
		U32 ulDMA_CFG_Temp = gFREG[FCTLL_DMA_CFG].L;
		U32 ulINT_VCT_Temp = gFREG[FCTLL_INT_VCT].L;
		U32 ulCHNL_SET_Temp = gFREG[FCTLL_CHNL_SET].L;
		U32 ulL4K_SPR_ADR_Temp = gFREG[FCTLL_L4K_SPR_ADR].L;


		if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
			gFREG[FCTLL_IFSA0].L = (((ubDieIndex * guwDieBlockShift) & 0xFFFF) << 16);
			if (ENABLE_SANDISK_1ZNM_SPF) {
				if (ubCEIndexInThatChannel >= gubHideCE) {
					gFREG[FCTLL_IFSA0].L += ((24576 >> gubBurstsPerBankLog) << 16);
				}
			}
		}
		else {
			gFREG[FCTLL_IFSA0].L = 0x00000000;
		}
		gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;

		gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
		gFREG[FCTLL_RS_CFG].L &= (~RS_ON_THE_FLY_EN);

		gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
		gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubL4KNum);

		gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
		gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
		if (ubChannel != 0xFF) {
			gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
		}
		else {
			FCTLL[FCTLL_INT_VCT] |= IntVector(ubDepth);
			FCTLL[FCTLL_INT_VCT + (0x200 >> 2)] |= IntVector(ubDepth2);
		}

		gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);

		gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME) ;
		gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubL4KNum) << 8);

		gFREG[FCTLL_CHNL_SET].L |= (BYPASS_CONV);

		if (ubChannel != 0xFF) {
			gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_OFFSET;
		}
		else {
			FCTLL[FCTLL_L4K_SPR_ADR] = HB_RETRY_L4K_TABLE_OFFSET;
			FCTLL[FCTLL_L4K_SPR_ADR + (0x200 >> 2)] = FSA_BASE;
		}

		if (ubChannel != 0xFF) {
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0x00000000;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (BUFFER3_BASE + ubL4KCount * 4096) / 512;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
			}
		}
		else {
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0x00000000;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (BUFFER3_BASE + ubL4KCount * 4096) / 512;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
			}

			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + FSA_BASE + (ubL4KCount * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0x00000000;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (BUFFER3_BASE + 0x4000 + ubL4KCount * 4096) / 512;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
			}
		}

		if (ubChannel != 0xFF) {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubCEIndexInThatChannel]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = (BIT0 << ((ubChannel * MAX_CE_PER_CH) + ubCEIndexInThatChannel));
			}
		}
		else {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubCEIndexInThatChannel]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubCEIndexInThatChannel]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 | BIT8) << ubCEIndexInThatChannel);
			}
		}

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C80_A5_DW;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		if (ubChannel != 0xFF) {
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		}
		else {
			while ((FCTLL[FCTLL_FPU_TRIG] & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			while ((FCTLL[FCTLL_FPU_TRIG + (0x200 >> 2)] & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		}

		if (ENABLE_DEBUG_UART_VENDER) {
			UartString("\r\nFCTLL_INT_INF: ");
			UartLongHex(gFREG[FCTLL_INT_INF].L);
		}

		// reassign CE,  MT stop後, 當MT preload + FPU read後會clear CE, 所以上面FPU trig後, CE有可能會被清掉, 這邊需再set CE一次, 之後無MT preload行為就不需要再重設
		if (ubChannel != 0xFF) {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubCEIndexInThatChannel]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = (BIT0 << ((ubChannel * MAX_CE_PER_CH) + ubCEIndexInThatChannel));
			}
		}
		else {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubCEIndexInThatChannel]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubCEIndexInThatChannel]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 | BIT8) << ubCEIndexInThatChannel);
			}
		}

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		if (ubChannel != 0xFF) {
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		}
		else {
			while ((FCTLL[FCTLL_FPU_TRIG] & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			while ((FCTLL[FCTLL_FPU_TRIG + (0x200 >> 2)] & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		}

		// need clean msg dispatch
		gubINTTable[ubDepth].All = 0;
		M_AddMTDepth(ubDepth);

		if (ubChannel != 0xFF) {
			gubINTTable[ubDepth2].All = 0;
			M_AddMTDepth(ubDepth2);
		}

		FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs

		gFREG[FCTLL_IFSA0].L = ulIFSA0_Temp;
		gFREG[FCTLL_FSA_SEL].L = ulFSA_SEL_Temp;
		gFREG[FCTLL_ZIP_CFG].L = ulZIP_CFG_Temp;
		gFREG[FCTLL_RS_CFG].L = ulRS_CFG_Temp;
		gFREG[FCTLL_DMA_CFG].L = ulDMA_CFG_Temp;
		gFREG[FCTLL_INT_VCT].L = ulINT_VCT_Temp;
		gFREG[FCTLL_CHNL_SET].L = ulCHNL_SET_Temp;
		gFREG[FCTLL_L4K_SPR_ADR].L = ulL4K_SPR_ADR_Temp;

		mSE_SV(4, BUFFER3_BASE, gulSectorCnt * 512, 0xFFFFFFFF , 0);

#pragma flush
		while ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0); //Philip said ASIC need not poll 8 times
		SYS1L[SYS1L_ECC_EN] = ulECC_Temp;
		idle(0x20);
		HL[HL_BUF_R_EP] = ulHL_BUF_R_EP_Temp;
		HL[HL_BUF_W_EP] = ulHL_BUF_W_EP_Temp;
	}
	else {
		Vender_TriggerHostData();
	}
#endif
}
void Vender_Cache_Read()
{
#if 1
	M_CheckRDTModeToDisableFunction();

	if (BURNER) {
		UBYTE ubDieIndex = HB[HB_LBA_L];
		UBYTE ubChannel = HB[HB_LBA_M];
		UBYTE ubCEIndexInThatChannel = HB[HB_LBA_H];
		UBYTE ubL4KNum = gub4kEntrysPerPlane;
		UBYTE ubDepth, ubDepth2;
		UBYTE ubL4KCount = 0;
		volatile L4KTable16B *pL4KTable;
		U32 ulECC_Temp = SYS1L[SYS1L_ECC_EN];
		UBYTE ubCENum = (gubExpandCE) ? gubMTQTotalPhyCE : gubCENumber;
		UBYTE ubDieNum = (gubExpandCE) ? (gubDieNumber * gubExpandCE_Ratio) : gubDieNumber ;

		if (ENABLE_DEBUG_UART_VENDER) {
			UartString("\r\nR: ");
			//Uart_Tx_DataHex(ubDieIndex);
			UartString(" ubCH: ");
			Uart_Tx_DataHex(ubChannel);
			UartString(" ubBank: ");
			Uart_Tx_DataHex(ubCEIndexInThatChannel);
			UartString(" 9820: 0x");
			UartLongHex(FDLLL[0x20 / 4]);
			UartString(" 9824: 0x");
			UartLongHex(FDLLL[0x24 / 4]);
		}

		if (((ubChannel >= gubPlanesPerBurst) && (ubChannel != 0xFF))
		        || (ubCEIndexInThatChannel >= (ubCENum >> gubPlanesPerBurstLog))
		        || (((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) && (ubDieIndex >= ubDieNum))) {

			gubErrorCode = SET_ABRT;
			return;
		}
		if (ENABLE_SANDISK_1ZNM_SPF) {
			if ((ubCEIndexInThatChannel >= gubHideCE) && (ubDieIndex == 2)) {
				ubDieIndex -= 2;
			}
		}
		M_GetMTDepth(ubDepth);
		if (ubChannel != 0xFF) {
			M_GetMTDepth(ubDepth2);
		}

		if (ubChannel != 0xFF) {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
		}
		else {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + FALL);
		}

		U32 ulIFSA0_Temp = gFREG[FCTLL_IFSA0].L;
		U32 ulFSA_SEL_Temp = gFREG[FCTLL_FSA_SEL].L;
		U32 ulZIP_CFG_Temp = gFREG[FCTLL_ZIP_CFG].L;
		U32 ulRS_CFG_Temp = gFREG[FCTLL_RS_CFG].L;
		U32 ulDMA_CFG_Temp = gFREG[FCTLL_DMA_CFG].L;
		U32 ulINT_VCT_Temp = gFREG[FCTLL_INT_VCT].L;
		U32 ulCHNL_SET_Temp = gFREG[FCTLL_CHNL_SET].L;
		U32 ulL4K_SPR_ADR_Temp = gFREG[FCTLL_L4K_SPR_ADR].L;
		U32 ulHL_BUF_R_EP_Temp = HL[HL_BUF_R_EP];
		U32 ulHL_BUF_W_EP_Temp = HL[HL_BUF_W_EP];

		HL[HL_BUF_R_EP] = 8;
		HL[HL_BUF_W_EP] = 8;

#pragma flush
		while ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0); //Philip said ASIC need not poll 8 times
		SYS1L[SYS1L_ECC_EN] &= ~(CR_Z1_ECC_EN | CR_Z2_ECC_EN | CR_Z3_ECC_EN);
		idle(0x20);
		mSE_SV(4, BUFFER3_BASE, gulSectorCnt * 512, 0 , 0);

		if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
			gFREG[FCTLL_IFSA0].L = (U32)(((ubDieIndex * guwDieBlockShift) & 0xFFFF) << 16);
			if (ENABLE_SANDISK_1ZNM_SPF) {
				if (ubCEIndexInThatChannel >= gubHideCE) {
					gFREG[FCTLL_IFSA0].L += ((24576 >> gubBurstsPerBankLog) << 16);
				}
			}
		}
		else {
			gFREG[FCTLL_IFSA0].L = 0x00000000;
		}
		gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;
		gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
		gFREG[FCTLL_RS_CFG].L &= (~RS_ON_THE_FLY_EN);

		gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
		gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubL4KNum);

		gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;

		gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
		if (ubChannel != 0xFF) {
			gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
		}
		else {
			FCTLL[FCTLL_INT_VCT] |= IntVector(ubDepth);
			FCTLL[FCTLL_INT_VCT + (0x200 >> 2)] |= IntVector(ubDepth2);
		}

		gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);

		gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME) ;
		gFREG[FCTLL_DMA_CFG].L |= (((UWORD)ubL4KNum) << 8);

		if (ubChannel != 0xFF) {
			gFREG[FCTLL_L4K_SPR_ADR].L = HB_RETRY_L4K_TABLE_OFFSET;
		}
		else {
			FCTLL[FCTLL_L4K_SPR_ADR] = HB_RETRY_L4K_TABLE_OFFSET;
			FCTLL[FCTLL_L4K_SPR_ADR + (0x200 >> 2)] = FSA_BASE;
		}

		gFREG[FCTLL_ZIP_MAP].L = gulZipMap;
		gFREG[FCTLL_CHNL_SET].L |= (BYPASS_CONV);

		if (ubChannel != 0xFF) {
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0x00000000;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (BUFFER3_BASE + ubL4KCount * 4096) / 512;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
			}
		}
		else {
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + HB_RETRY_L4K_TABLE_OFFSET + (ubL4KCount * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0x00000000;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (BUFFER3_BASE + ubL4KCount * 4096) / 512;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
			}

			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTable = (volatile L4KTable16B*)(IRAM_BVCI_BASE + FSA_BASE + (ubL4KCount * Spare4kSize));
				//set L4K table info
				pL4KTable->ulL4K_LCA = 0x00000000;
				pL4KTable->Para0x04.ulAll = 0;
				pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (BUFFER3_BASE + 0x4000 + ubL4KCount * 4096) / 512;
				pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;	//都不讀上來, 避免蓋到原有buffer的data
			}
		}

		if (ubChannel != 0xFF) {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubCEIndexInThatChannel]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = (BIT0 << ((ubChannel * MAX_CE_PER_CH) + ubCEIndexInThatChannel));
			}
		}
		else {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubCEIndexInThatChannel]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubCEIndexInThatChannel]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 | BIT8) << ubCEIndexInThatChannel);
			}
		}

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C05_A2_CE0_DR;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;

		if (ubChannel != 0xFF) {
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		}
		else {
			while ((FCTLL[FCTLL_FPU_TRIG] & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
			while ((FCTLL[FCTLL_FPU_TRIG + (0x200 >> 2)] & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		}

		//clear interrupt
		if (ENABLE_DEBUG_UART_VENDER) {
			UartString("\r\nFCTLL_INT_INF: ");
			UartLongHex(gFREG[FCTLL_INT_INF].L);
		}

		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
			gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
		}

		// need clean msg dispatch
		gubINTTable[ubDepth].All = 0;
		M_AddMTDepth(ubDepth);

		if (ubChannel != 0xFF) {
			gubINTTable[ubDepth2].All = 0;
			M_AddMTDepth(ubDepth2);
		}

		FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs

		gFREG[FCTLL_IFSA0].L = ulIFSA0_Temp;
		gFREG[FCTLL_FSA_SEL].L = ulFSA_SEL_Temp;
		gFREG[FCTLL_ZIP_CFG].L = ulZIP_CFG_Temp;
		gFREG[FCTLL_RS_CFG].L = ulRS_CFG_Temp;
		gFREG[FCTLL_DMA_CFG].L = ulDMA_CFG_Temp;
		gFREG[FCTLL_INT_VCT].L = ulINT_VCT_Temp;
		gFREG[FCTLL_CHNL_SET].L = ulCHNL_SET_Temp;
		gFREG[FCTLL_L4K_SPR_ADR].L = ulL4K_SPR_ADR_Temp;

		Vender_TriggerHostData();

#pragma flush
		while ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0); //Philip said ASIC need not poll 8 times
		SYS1L[SYS1L_ECC_EN] = ulECC_Temp;
		idle(0x20);
		HL[HL_BUF_R_EP] = ulHL_BUF_R_EP_Temp;
		HL[HL_BUF_W_EP] = ulHL_BUF_W_EP_Temp;
	}
	else {
		Vender_TriggerHostData();
	}


#endif
}
#if MicronFlashOnly
void Vender_CheckL06BUID()
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	UBYTE ubCE = HB[HB_LBA_H];
	//	UBYTE ubDie = HB[HB_LBA_M];
	U32 ulDelayLoop;


	UWORD uwi;
	UBYTE ubChannel;

	UBYTE ubForceTwoByteRead = 0;

	U32 ulFL_FCTL_HS_MODE_All_Temp =  FALLL[FCTLL_HS_MODE];


Start:
	for ( uwi = 0; uwi < (512); uwi++) {
		BUFB_BASE[uwi] = 0x00;
	}

	{

		//ubChannel = (ubCE & gubPlanesPerBurstMask);
		ubChannel = (ubCE & 0x08) ? 1 : 0;
		ubCE = FCONB[(FCONL_FCE_SEL_0 << 2) + ubCE];


		//FCONL[FCONL_FCE_ENB] = (gubGPIO_CE_Decoder && (!MicronFlashOnly)) ? M_MapToCEDecoder(ubChannel, ubCE) : (0x00000001 << ubCE);
		FCONL[FCONL_FCE_ENB] = (0x00000001 << ubCE);

		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0xFF;

		FCTLL[FCTLL_HS_MODE + (ubChannel * ( 0x200 / 4))] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x70; //Status Read
		UBYTE ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];

		ulDelayLoop = 0;

		while (ubRet != 0xE0) {
			FCTLL[FCTLL_HS_MODE + (ubChannel * 0x200 / 4)] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];
			ulDelayLoop++;
			//Uart_Printf("Read ID Poll Status:%l\r\n", ubRet);
			if (ulDelayLoop > 40000) {
				//Uart_Printf("CE %d doesn't exist!!\r\n",ubFlashCE);
				break;
			}
		}

		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0xED;

		FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)]  = 0x00;


		FCTLL[FCTLL_HS_MODE + (ubChannel * ( 0x200 / 4))] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x70; //Status Read
		ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];

		ulDelayLoop = 0;

		while (ubRet != 0xE0) {
			FCTLL[FCTLL_HS_MODE + (ubChannel * 0x200 / 4)] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];
			ulDelayLoop++;
			//Uart_Printf("Read ID Poll Status:%l\r\n", ubRet);
			if (ulDelayLoop > 40000) {
				//Uart_Printf("CE %d doesn't exist!!\r\n",ubFlashCE);
				break;
			}
		}

		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x00;



		if (  guoFlashUseType.B.btToggle  || ubForceTwoByteRead) {

			for ( uwi = 0; uwi < 256; uwi++) {
				guwTemp = FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];
				BUFB_BASE[uwi * 2 + 0] = guwTemp;
				BUFB_BASE[uwi * 2 + 1] = guwTemp >> 8;
			}
		}
		else {

			for ( uwi = 0; uwi < 512; uwi++) {
				BUFB_BASE[uwi] = FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)] ;
			}
		}

#if 0
		UWORD uwPrintOffset;
		for (uwPrintOffset = 0; uwPrintOffset < 512; uwPrintOffset++) {
			UartString("\n [I]");
			UartWordHex(uwPrintOffset);
			UartString(" ");
			Uart_Tx_DataHex(BUFB_BASE[uwPrintOffset]);
		}


#endif

		UBYTE ubMatch = 1;

		UBYTE ubByteNo;
		for (ubByteNo = 0; ubByteNo < 16; ubByteNo++) {
			if (BUFB_BASE[ubByteNo] != BUFB_BASE[ubByteNo + 0x20]) {
				ubMatch = 0;
				break;
			}
		}

	CheckNextByte:

		if (ubMatch == 0) {
			//BUFB_BASE[256]  |= BIT0 << ubByteNo;

			if (ubForceTwoByteRead == 0) {
				// 之前版本isp jump沒切legacy... (legacy-> toggle 2.0這字包爛flash...)

				FALLL[FCTLL_HS_MODE] |= 0x51;
				ubForceTwoByteRead = 1;
				goto Start;
			}
		}

	}

	FALLL[FCTLL_HS_MODE] = ulFL_FCTL_HS_MODE_All_Temp;

END:
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}
	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

#endif
}
#endif
void Vender_CheckTSBUID()
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	UBYTE ubCE = HB[HB_LBA_H];
	UBYTE ubDie = HB[HB_LBA_M];
	U32 ulDelayLoop;


	UWORD uwi;
	UBYTE ubChannel;

	UBYTE ubForceTwoByteRead = 0;

	U32 ulFL_FCTL_HS_MODE_All_Temp =  FALLL[FCTLL_HS_MODE];


Start:
	for ( uwi = 0; uwi < (1024); uwi++) {
		BUFB_BASE[uwi] = 0x00;
	}
#if TSB_BICS4_SUPPORT
	if (  (( guoFlashUseType.B.btToshiba1Ynm) &&  ((gubDieSize == 4) || (gubDieSize == 8)) )
	        || (( guoFlashUseType.B.btToshiba1Znm && guoFlashUseType.B.btTwoPlane && (guoFlashUseType.B.btFourPlane == 0)) &&  ((gubDieSize == 8) || (gubDieSize == 16)) )
	        || ( guoFlashUseType.B.btToshibaBiCs3) || ( guoFlashUseType.B.btToshibaBiCs4)) {
#else
	if (  (( guoFlashUseType.B.btToshiba1Ynm) &&  ((gubDieSize == 4) || (gubDieSize == 8)) )
	        || (( guoFlashUseType.B.btToshiba1Znm && guoFlashUseType.B.btTwoPlane && (guoFlashUseType.B.btFourPlane == 0)) &&  ((gubDieSize == 8) || (gubDieSize == 16)) )
	        || ( guoFlashUseType.B.btToshibaBiCs3)) {
#endif
		ubChannel = (ubCE & 0x08) ? 1 : 0;
		ubCE = FCONB[(FCONL_FCE_SEL_0 << 2) + ubCE];
		//FCONL[FCONL_FCE_ENB] = (gubGPIO_CE_Decoder && (!MicronFlashOnly)) ? M_MapToCEDecoder(ubChannel, ubCE) : (0x00000001 << ubCE);
		FCONL[FCONL_FCE_ENB] = (0x00000001 << ubCE);

		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0xFF;

		FCTLL[FCTLL_HS_MODE + (ubChannel * ( 0x200 / 4))] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x70; //Status Read
		UBYTE ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];

		ulDelayLoop = 0;

		while (ubRet != 0xE0) {
			FCTLL[FCTLL_HS_MODE + (ubChannel * 0x200 / 4)] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];
			ulDelayLoop++;
			//Uart_Printf("Read ID Poll Status:%l\r\n", ubRet);
			if (ulDelayLoop > 40000) {
				//Uart_Printf("CE %d doesn't exist!!\r\n",ubFlashCE);
				break;
			}
		}

		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x5A;
		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0xB5;


		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x00;

		FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)]  = 0x00;
		FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x00;

		if (guoFlashUseType.B.btToshiba1Ynm) {
			FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x00;
		}
		else if (guoFlashUseType.B.btToshiba1Znm) {
			FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x08;
		}
		else {
			FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x28;
		}

		FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x00;

		if (ubDie == 0) {
			FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x00;

		}
		else {
#if TSB_BICS4_SUPPORT
			if (guoFlashUseType.B.btToshibaBiCs3 || guoFlashUseType.B.btToshibaBiCs4) {
#else
			if (guoFlashUseType.B.btToshibaBiCs3) {
#endif
				if (ubDie == 1) {
					FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x10;
				}
				else if (ubDie == 2) {
					FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x20;
				}
				else if (ubDie == 3) {
					FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x30;
				}
			}
			else {
				if (gubDieSize == 4) {
					FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x04;
				}
				else if (gubDieSize == 8) {
					FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x08;
				}
				else if (gubDieSize == 16) {
					FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x10;
				}
			}
			/*
			if(guoFlashUseType.B.btToshiba1Ynm){
				if (gubDieSize == 4) {
					FREGB[(F_FCTL_PIO_ADR * 4) + (ubChannel * 0x100)] = 0x04;
			}
				else if (gubDieSize == 8) {
					FREGB[(F_FCTL_PIO_ADR * 4) + (ubChannel * 0x100)] = 0x08;
				}
			}else if(guoFlashUseType.B.btToshiba1Znm){
				if (gubDieSize == 8) {
					FREGB[(F_FCTL_PIO_ADR * 4) + (ubChannel * 0x100)] = 0x08;
			}
				else if (gubDieSize == 16) {
					FREGB[(F_FCTL_PIO_ADR * 4) + (ubChannel * 0x100)] = 0x10;
				}
			}*/
		}


		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x30;

		FCTLL[FCTLL_HS_MODE + (ubChannel * ( 0x200 / 4))] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x70; //Status Read
		ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];

		ulDelayLoop = 0;

		while (ubRet != 0xE0) {
			FCTLL[FCTLL_HS_MODE + (ubChannel * 0x200 / 4)] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];
			ulDelayLoop++;
			//Uart_Printf("Read ID Poll Status:%l\r\n", ubRet);
			if (ulDelayLoop > 40000) {
				//Uart_Printf("CE %d doesn't exist!!\r\n",ubFlashCE);
				break;
			}
		}


		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x05;

		FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x00;
		FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x00;

		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0xE0;

		//for(gubTemp =0; gubTemp< 0x20; gubTemp++){}

		if (  guoFlashUseType.B.btToggle || ubForceTwoByteRead) {
			for ( uwi = 0; uwi < 512; uwi++) {
				guwTemp = FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];
				BUFB_BASE[uwi * 2 + 0] = guwTemp;
				BUFB_BASE[uwi * 2 + 1] = guwTemp >> 8;
			}
		}
		else {
			for ( uwi = 0; uwi < 1024; uwi++) {
				guwTemp = FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)] ;
				BUFB_BASE[uwi] = guwTemp;
			}
		}

#if !TLC_BICS2
		UBYTE ubMatch = 0;


		UBYTE ubOriginalSet, ubInvertSet;
		UBYTE ubByteNo;

		UWORD uwOriginalData, uwInvertData;

		for (ubByteNo = 0; ubByteNo < 8; ubByteNo++) {

			ubMatch = 0;
			for (ubOriginalSet = 0; ubOriginalSet < 8; ubOriginalSet++) {
				for ( ubInvertSet = 0; ubInvertSet < 8; ubInvertSet++) {

					uwOriginalData = BUFB_BASE[ubOriginalSet * 32 + ubByteNo];
					uwInvertData = BUFB_BASE[ubInvertSet * 32 + 16 + ubByteNo];

					if ( (uwOriginalData + uwInvertData) == 0xFF) {
						ubMatch = 1;
						goto CheckNextByte;
					}
				}
			}


		CheckNextByte:

			if (ubMatch == 0) {
				BUFB_BASE[256]  |= BIT0 << ubByteNo;

				if (ubForceTwoByteRead == 0) {
					// 之前版本isp jump沒切legacy... (legacy-> toggle 2.0這字包爛flash...)

					FALLL[FCTLL_HS_MODE] |= 0x51;
					ubForceTwoByteRead = 1;
					goto Start;
				}
			}
		}
#endif

	}

	FALLL[FCTLL_HS_MODE] = ulFL_FCTL_HS_MODE_All_Temp;

END:
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 1024);
	}
	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

#endif
}

void Vender_Detect_Thermal_Sensor()
{
	UBYTE ubExist = THEMAL_SENSOR_NOT_EXIST;

	switch (HB[HB_DEVICE] & 0x0F) {
		case THERMAL_SENSOR_TYPE_TMP101:
			ubExist = I2C_Temperature(1);
			break;
		default:
			break;
	}

	if (ubExist == THEMAL_SENSOR_EXIST) {
		HB[HB_LBA_L] = THEMAL_SENSOR_EXIST;
	}
	else {
		HB[HB_LBA_L] = THEMAL_SENSOR_NOT_EXIST;
	}

	HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
}
void Vender_ReadParameterPage()
{
	if (gulParamPageInfo.JEDEC.btNotSupport || gulParamPageInfo.ONFI.btNotSupport) {
		gubErrorCode = SET_ABRT;
		return;
	}
	memcpy(&BUFW_BASE[0], &gubParameterPage[0],  sizeof(gubParameterPage));
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)&BUFW_BASE[0], sizeof(gubParameterPage));
	}

	Vender_TriggerHostData();
}

void Vender_ReadParameterPage_CE()
{
	UBYTE ubCEIndex = HB[HB_LBA_H];
	U32 ulFCONL_FCE_ENB_Temp = FCONL[FCONL_FCE_ENB];
	flaReadParameterPage(ubCEIndex, PARAMETER_PAGE);

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)&BUFW_BASE[0], 512);
	}

	FCONL[FCONL_FCE_ENB] = ulFCONL_FCE_ENB_Temp;

	Vender_TriggerHostData();

}

void AP_Entry() //CMD 0x21 or 0x31
{
	/*
		gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
		while (gBQI.ubLinkNum||gFQI.ubFQLinkNumber) {
	                M_SwitchTask();
	        }
	*/
	///////////////////disable by Lin//////////////
	//	M_CheckRDTModeToDisableFunction();




	if (gubAP_enable) {
		HandleStopRW(0);
		UWORD uwWRBLK_temp = (HW[HW_AUTO_FIS_CTRL] & H_WR_BLK_EN);
		UBYTE  ubDDT_temp = (HB[HB_FLAG_CTRL] & H_DDT_EN);
		UWORD uwMSG_temp = (HW[HW_SYS_CTRL] & (H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));

		HB[HB_FLAG_CTRL] &= (~H_DDT_EN);
		HW[HW_SYS_CTRL] &= (~(H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
		HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);

		HL[HL_BUF_BASE] = BUFFER3_BASE / 4096;
		HL[HL_BUF_R_EP] = BUFFER3_READ_END;
		HL[HL_BUF_W_EP] = BUFFER3_WRITE_END;
		HL[HL_BUF_R_SHF] = BUFFER3_READ_START;
		HL[HL_BUF_W_SHF] = BUFFER3_WRITE_START;

		M_SetFlagSettingNone();

		if (HB[HB_COMMAND] == 0x21) {
			gubNeedSendCmdComplete = 0;
		}

		{

			{
				// 解密Command
#if EnableVenderCmdHistory
				gubVendeCmdHistory[gubVendeCmdHistoryPtr] = HB[HB_FEATURE];
#if EnableVH_UARTDebug
				UartString("\n[VC]");
				Uart_Tx_DataHex(gubVendeCmdHistoryPtr);
				Uart_Tx_DataHex(gubVendeCmdHistory[gubVendeCmdHistoryPtr]);
#endif
				gubVendeCmdHistoryPtr++;
				if (gubVendeCmdHistoryPtr == (MaxVenderCmdLog)) {
					gubVendeCmdHistoryPtr = 0;
				}
#endif
				switch (HB[HB_FEATURE]) {
					case 0x01 :
						Vender_Reset(uwWRBLK_temp, ubDDT_temp, uwMSG_temp);
						break;
					case 0x02 :
						Vender_ClearSMART();
						break;
#if(!RDT)
					case 0x05:
						Vender_DisableFreezeLock();
						break;
#endif
					case 0x0f :
						SYSL[SYS0L_SIM_CTRL0] |= SIMREG_CMD_D2H; //use as a tag for command D2H, but comreset D2H
						gubFlushMode = BYTE_STANDBY_FLUSH;
						FlushCache();
						Vender_Isp_Jump(1);
						break;
#if(!RDT)
					case 0x08 :
						Vender_Preformat();
						break;

					case 0x10 :
						Vender_Direct_Read_Flash();
						break;

					case 0x11:
						Vender_Cache_Read();
						break;

					case 0x12 :
						Vender_Read_SRAM();
						break;
#endif
					case 0x13 :
						Vender_ReadSystemInfo();
						break;

					case 0x14 :
						Vender_GetFlashID(HB[HB_LBA_H]);
						HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
						while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
						break;

					case 0x15 :
						Vender_ScanFlashID();
						break;
					case 0x16 :

						HW[HW_SECTOR_CNT] = gulSectorCnt;
						HL[HL_BUF_TRG_SEC] = gulSectorCnt;
						HB[HB_DATA_TRG] = H_DATA_TRG;
						while (HB[HB_DATA_TRG]&H_DATA_TRG);
						break;
#if(!RDT)
					case 0x20:
						Vender_Direct_Write_Page();
						break;

					case 0x21:
						Vender_Cache_Program();
						break;

					case 0x22:
						Vender_Direct_Erase_Block();
						break;

					case 0x24:
						Vender_Write_SRAM();
						break;

					case 0x25:
						Vender_Direct_Write_Info(0, DONTCARE);
						break;

					case 0x26:
#if EnableProductHistory
						if (ENABLE_DCACHE) {
							xthal_dcache_region_invalidate(BUFB_BASE, 512);
						}
						mDMAC_SV(1,  (U32)BUFB_BASE, 512, 0, 0, 0, DMA_EN_SETCQ, 1);
#endif
						HW[HW_DATA_TRG] |= H_DATA_TRG;
						while (HW[HW_DATA_TRG]&H_DATA_TRG);
#if EnableProductHistory
						if ((BUFB_BASE[0] == 0x4D) && (BUFB_BASE[1] == 0x46) && (BUFB_BASE[2] == 0x47) && (BUFB_BASE[3] == 0x49) && (BUFB_BASE[4] == 0x4E) && (BUFB_BASE[5] == 0x46) && (BUFB_BASE[6] == 0x4F)) {
							// M F G I N F O
							gubTureErase = 1;
						}
#endif
						if ((HB[HB_LBA_H]&MODE_RETAIN_RUT_EC) && (gubSSD_Init_State.B.ubState < BYTE_INIT_SCAN_DONE)) {
							if (Load_RUT_EC_Table()) {
								ftlPreformatShowError(0, guwSystemBlock[0], 0x28);
								break;
							}
						}
						Vender_Erase_All_Blocks();
#if EnableVenderCmdHistory
						Vender_SaveVenderCmdHistory();
#endif
#if EnableProductHistory
						gubTureErase = 0;
#endif
						break;
#endif
					case 0x27:
						Vender_Trim_UART_JTAG();
						break;

					case 0x28:
						Vender_Direct_Read_Info();
						break;
#if(!RDT)
					case 0x31:
#if Hynix &&(!(Hynix3DV6 || Hynix3DV5))&& BURNER
						ToggleModeEnable(0);
						Vender_Isp_Verify(0, 0xff);
						ToggleModeEnable(guoFlashUseType.B.btToggle);
#else
						Vender_Isp_Verify(0, 0xff);
#endif
						break;
#endif
					case 0x41:
#if Hynix &&(!(Hynix3DV6 || Hynix3DV5))&& BURNER
						ToggleModeEnable(0);
						Vender_Isp_Prog();
						ToggleModeEnable(guoFlashUseType.B.btToggle);
#else
						Vender_Isp_Prog();
#endif
						break;
#if (RDT || (RDT_BURNER))
					case 0x50:
						Vender_RDT_GetBadBlkLog();
						break;

					case 0x51:
						Vender_RDT_GetTimeStampLog();
						break;

					case 0x52:
						if (HB[HB_COMMAND] == 0x31) { // write command
							Vender_Detect_Thermal_Sensor();
						}
						else if (HB[HB_COMMAND] == 0x21) {   // read command
							Vender_RDT_GetSDRLog();
						}
						break;
#else
					case 0x52:
						Vender_Detect_Thermal_Sensor();
						break;
#endif

					case 0x60:
						Vender_Write124Byte_Reg();
						break;

					case 0x61:
						Vender_Read124Byte_Reg();
						break;
#if(!RDT)
					case 0x70:
					case 0x71:
					case 0x72:
					case 0x73:
					case 0x74:
					case 0x75:
					case 0x76:
					case 0x77:
					case 0x78:
					case 0x79:
					case 0x7a:
					case 0x7b:
					case 0x7c:
					case 0x7d:
					case 0x7e:
						SMART_Vender();
						break;
					case 0x80:
						Vender_Send_Seed();
						break;

					case 0x82:
						Vender_Prog_RAM_Whole();
						break;

					case 0x8C:
						Vender_SetReadSRAMParameter();
						break;

					case 0x8D:
						Vender_Trim_Set_PIO();
						break;

					case 0x8F:
						Vender_TrimAll_Test();
						break;
#if (BURNER&&ENABLE_VENDER_READ_VTH)
					case 0x95:
						Vender_FLH_Distribution_Read_Setting();
						break;

					case 0x96:
						Vender_FLH_Distribution_Read();
						break;
#endif
					case 0x97:
						Vender_BadBlockInfo();
						break;

					case 0xA1:
						Vender_HW_SDR_Read();
						break;

					case 0xA2:
						Vender_HW_SDR_Write();
						break;

					case 0xA3:
						Vender_CheckLaterBadReuse();
						break;

					case 0xA7:
						Vender_ReadParameterPage_CE();
						break;

					case 0xA8:
						Vender_MaxBad_per_Plane();
						break;

					case 0xA9:
						Vender_Set_FlashMode();
						break;

					case 0xAA:
						Vender_InitialPadAndDrive();
						break;

					case 0xAB:
						Vender_Scan_SDLL();
						break;

					case 0xAE:
#if MicronFlashOnly
						Vender_CheckL06BUID();
#else
						Vender_CheckTSBUID();
#endif
						break;

					case 0xAF:
						Vender_ReadParameterPage();
						break;
#if TLC_TSB
					case 0xB8:
						Vender_Direct_RawRead_TSB();
						break;
#endif
					case 0xB9:
						Vender_ReadVTFailLog();
						break;
#if BURNER
					case 0xBA:
						Vender_Direct_Write_DBT16KPage();
						break;

					case 0xBB:
						Vender_Direct_RawRead();
						break;
#endif
					case 0xBC:
						Vender_CheckRetryTable();
						break;

					case 0xBD:
						Vender_LoadBurnerIcode();
						break;

					case 0xBE:
						Vender_VerifyBurnerIcode();
						break;

					case 0xBF:
						Vender_GetErrorInfoBlock();
						Vender_TriggerHostData();
						break;
					
#if ENABLE_OUTPUT_TRIMMING_TABLE
					case 0xCC:
						Vender_Output_Trimming_Table();
						break;
#endif
					case 0xD6:
						Vender_I2C_Write_LedIC();
						break;

					case 0xD7:
						Vender_I2C_Read_LedIC();
						break;

					case 0xD8:
						Vender_GetBackGroundStatus();
						break;

#if EnableProductHistory
					case 0xDA:
						Vender_ReadPH();
						break;

					case 0xDB:
						Vender_WritePH();
						break;
#endif

					case 0xFB:
						Vender_ReadFLHBlockMappingInfo();
						break;

#if PH_SQL_FUNC
					case 0xF0:
						Vender_Mode_Cmd_VNandTool(3);
						break;

					case 0xF1:
						Vender_Mode_Cmd_VNandTool(4);
						break;

					case 0xFC:
						Vender_Mode_Cmd_VNandTool(0);
						break;

					case 0xFD:
						Vender_Mode_Cmd_VNandTool(1);
						break;

					case 0xFE:
						Vender_Mode_Cmd_VNandTool(2);
						break;
#endif
#endif
					default:
						NotImplement();
						break;
				}
			}
		}

		// <<< restore buffersetting...
		if (DCCMSATAB[SATAB_RD_CNT]) {
			DCCMSATAB[SATAB_RPIU] = DCCMSATAB[SATAB_RD_CNT];
			gulSATA_MSG_TYPE3[0].All = 0;
		}



		gubAP_enable = 0;

		if (gubNeedSendCmdComplete || gubErrorCode) {
			gubNeedSendCmdComplete = 0;
			if ( !(gubSoftReset | gubHostCmdIn | gubHardReset) ) {//若有任一為1,表示Host不想等結果..
				if (gubErrorCode) {
					HB[HB_STATUS] = 0x51;
					HB[HB_ERROR] = gubErrorCode;

					if (HB[HB_ERROR] != SET_ABRT) {
						if ( HB[HB_ERROR] != SET_IDNF) {
							//RecordCmdLog();
						}
					}
				}
				else {
					HB[HB_STATUS] = 0x50;
				}

				HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
				while (HB[HB_REG_REQ]&H_REG_REQ);
			}
			else if (gubErrorCode && gubHostCmdIn) {
				//有發生error, 但是下一個cmd已經進來 --> 必須把BLK_TRIG與XHOLD清掉, 才能收送FIS
				//須在H_BLK_TRG清掉的狀況下才能清H_DATA_TRG (有error發生的話, H_DATA_TRG不會自己清.. 若不清掉,會無法判斷下一個Cmd是R/W)
				HW[HW_DATA_TRG] &= ~H_DATA_TRG;
			}
		}

		if (gubErrorCode) {     //打完error D2H後, 設回原值
			HB[HB_STATUS] = 0x50;
			HB[HB_ERROR] = NO_ERROR; //這行沒設的話, CKtool or 其他會polling error reg的都會出錯 (因為像是PIO 不會打D2H的, 都要在這重設)
			gubErrorCode = 0;

			/*非NCQ的話,做完就等於flag結束*/
			if (!(gubSoftReset | gubHardReset)) {
				//M_SetFlagSettingSHF();  //在ISR清掉,但因為可能沒進Reset, 所以這邊要舉
				HandleStopRW(0);
				HW[HW_ERR_CTRL] &= ~H_REG_XHOLD; //為了PIO Read最後一筆發生CRC十擋下的XRDY, 這邊要打開
			}
		}
		while (HL[HL_AXI_FIS_STAT]&BIT23);

		HB[HB_FLAG_CTRL] |= ubDDT_temp;
		HW[HW_SYS_CTRL] |= uwMSG_temp;
		HW[HW_AUTO_FIS_CTRL] |= uwWRBLK_temp;
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
		M_SetFlagSettingSHF();

		// >>> restore buffersetting...
		if (gubErrorCode) {
			ReturnErr();
		}
	}
	else {
		if (BURNER == 0) {
			
			if (HB[HB_COMMAND] == 0x21) {
				ReadSectors();
			}
			else {
				WriteSectors();
			}			
		}
		else {
			NotImplement();
		}
	}

}


void BurnerCheckEarlyBad(UBYTE ubMode)    // ubmode BIT0 for count only, BIT1 for address
{
	M_CheckRDTModeToDisableFunction();
	U32 ulVenderUnit, ulStartUnit, ulUnitsPerGroup = (gulTotalPhyUnitNum >> gubTotalGroupsLog);
	UBYTE ubLogicCE, ubIsBad, ubTemp, ubPlaneBank, ubSystemBad, ubPlane;
	U32 ulBadCnt[4], ulBadCntAllDie = 0;
	U32 ulAddr;
	UBYTE *ubTempBuffer = (UBYTE*)(BUFFER3_BASE + 0x4000);
	UBYTE ubRealDie = gubSM_CENo;


	UBYTE *ubDBTBuffer = (UBYTE *)(BUFFER3_BASE + 0x4000);
	//	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	UBYTE ubDBTpage = 0;

	//UWORD*uwTempCounter = (WORD*)(BUFFER3_BASE+0x4000);	
	if ( ubMode == BIT1) {
		mDMAC_SV(2, (U32)guwVenderSMARTBuffer, 128 * 1024, 0, 0xffffffff, 0, DMA_EN_SETCQ, 1);
	}
	else {
		mDMAC_SV(2, (U32)guwVenderSMARTBuffer, 128 * 1024, 0, 0, 0, DMA_EN_SETCQ, 1);
	}
	ulStartUnit = (ulUnitsPerGroup * (gubSM_GroupNo + ((M_Divider(gubSM_CENo, DEF_CEPerPage_Mcl_Div)) << gubDieNumberLog))) ;
	ubLogicCE = M_Modular(gubSM_CENo, DEF_CEPerPage_Mod_Mask);
	ubRealDie = ubLogicCE / gubMTQTotalPhyCE;
	ubSystemBad = 0;
	//if ((gubSM_GroupNo == 0) && (gubSystemAreaNum > (SYSTEM_SETS - 1))) {
	//	ulStartUnit = gubSystemAreaNum;
	//	ubSystemBad = gubSystemAreaNum - SYSTEM_SETS;
	//}
	for (ubPlane = 0; ubPlane < gubBurstsPerBank; ubPlane++) {
		//ulBadCnt[ubPlane] = ubSystemBad;
		ulBadCnt[ubPlane] = 0;
	}

	if ( RDT_FindDBTBlock_Task()) {
		gubRDTDone = DBT_Checked;
	}
	else {
		gubRDTDone = (DBT_Checked | DBT_Exist);
		if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {
			ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
			gubRDTDone = DBT_Checked;
		}
	}


	for (ulVenderUnit = ulStartUnit; ulVenderUnit < (ulUnitsPerGroup + ulStartUnit); ulVenderUnit++) {
		for (ubPlane = 0; ubPlane < gubBurstsPerBank; ubPlane++) {
			if (ENABLE_PLANE_CH_CE) {
				ubPlaneBank = (ubLogicCE << gubBurstsPerBankLog) + ubPlane;
			}
			else {
				ubPlaneBank = (ubPlane << gubPlanesPerBurstLog) + (ubLogicCE & gubPlanesPerBurstMask) + ((ubLogicCE >> gubPlanesPerBurstLog) << gubPlanesPerBankLog);
			}

			if (gubRDTDone & DBT_Exist) {
				U32 offset;

				offset = ((ulVenderUnit - ulStartUnit) * (gubCENumber * gubDieNumber) << gubBurstsPerBankLog) + gubSM_GroupNo * gubCENumber * gubBurstsPerBank + ubPlaneBank;


				//ulFreeblock = ((M_Multiplier(ulVenderUnit << gubTotalGroupsLog)) << DEF_PlanesPerPage_Mcl_Div) ;
				//uli = ulFreeblock % ulEntryPerDBTPlane;                                     //該PlaneBank放在這個DBT plane的第幾個Byte
				//if (uli == 0) {
				//	if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
				//		ftlPreformatShowError(0, gubDBTUnit, 0x28);
				//		return 1;
				//	}
				//}
				if (offset >= ubDBTpage * 16384) {
					if (ftlLoadDBT(ubDBTpage * 16384, (U32)&ubDBTBuffer[0])) {
						ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
						if (ENABLE_DEBUG_RW) {
							flaLEDBlink(1);
						}
						gubStopFTL = 0;
						return;
					}
					ubDBTpage++;
				}

				if (ENABLE_PLANE_CH_CE) {

					ubIsBad = (ubDBTBuffer[offset % 16384] & 0xC0);

					//ubIsBad |= 1;
					//break;

				}
				else {
					//ubIsBad=ubDBTBuffer[uli + ubCEIndex]
					//ubIsBad |= 1;
					//break;
				}



			}
			else {
				if (ENABLE_SANDISK_CHECKEARLYBADBYPROG) {
					ubIsBad = ftlGoodBlock_SandiskCheckByProgFail(ulVenderUnit, ubPlaneBank);
				}
				else {
#if TLC
#if B0KB
					ubIsBad = ftlGoodBlock(ulVenderUnit, ubPlaneBank, (U32)&ubTempBuffer[0], 2, GOODBLK_MLC);
#else
					ubIsBad = ftlGoodBlock(ulVenderUnit, ubPlaneBank, (U32)&ubTempBuffer[0], 2);
#endif
#else
#if MicronFlashOnly
					ubIsBad = ftlGoodBlock(ulVenderUnit, ubPlaneBank, (U32)&ubTempBuffer[0], 1, GOODBLK_MLC);
#else
					ubIsBad = ftlGoodBlock(ulVenderUnit, ubPlaneBank, (U32)&ubTempBuffer[0], 1);
#endif
#endif
				}

#if EnableProductHistory
				// For PH cannot be checked by ftlGoodBlock, Turn PHUnit into EarlyBad by FindPHBlockByScan
				if ((gubPHinit & 0xF) == 0) { //沒有init過
					//no DBT
					if (FindPHBlockByScan(0)) {
						//scan fail
						gubPHinit |= Bit3_InitByScan_NotFind;
					}
					else {
						//scan pass
						gubPHinit |= Bit2_InitByScan_Find;
					}
				}
				if (((U32)guwPHUnit[0].uwUnit == ulVenderUnit) && ((U32)guwPHUnit[0].ubPlanebank == ubPlaneBank) ) {
					ubIsBad = TRUE;
				}
				if (((U32)guwPHUnit[1].uwUnit == ulVenderUnit) && ((U32)guwPHUnit[1].ubPlanebank == ubPlaneBank) ) {
					ubIsBad = TRUE;
				}
#endif


			}

			if ( ubIsBad) {
				if ( ubMode == BIT1) {
					/*
					M_Divider(ubFTask,gubPlanesPerBurst);
					uwAddr = GetPhyAddress(REMAINDER);
					*/

					ulAddr = ((ulVenderUnit - ulStartUnit) << gubBurstsPerBankLog) + ((guwDieBlockShift * (gubSM_GroupNo + ubRealDie * gubDieNumber)) << gubBurstsPerBankLog) + ubPlane;

					guwVenderSMARTBuffer[ulBadCntAllDie] = ulAddr;
					ulBadCntAllDie++;
				}
				ulBadCnt[ubPlane]++;
			}
		}
	}
	ubTemp = 0;
	if (ubMode == BIT0) {
		for (ubPlane = 0; ubPlane < gubBurstsPerBank; ubPlane++) {
			guwVenderSMARTBuffer[ubTemp] = ulBadCnt[ubPlane];
			ubTemp ++;
			guwVenderSMARTBuffer[ubTemp] = ulBadCnt[ubPlane] >> 16;
			ubTemp ++;
			//BUFL_BASE[0] = ulBadCnt;
		}
	}
}

void Vender_MaxBad_per_Plane()
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	UWORD uwFWTotalUnitNum;

#if 0
	U32 ulFreeUnitPTR;
	UBYTE ubbadblock, ubi, ubj, ubPlaneBank, ubbadblockperCE[MAX_CE], gubCE;
	UBYTE *ubTempBuffer = (UBYTE *)(BUFFER3_BASE + 0x8000);

	gubSystemUnitFreeCount = 0;
	for (ubi = 0; ubi < gubCENumber; ubi ++) {
		ubbadblockperCE[ubi] = 0;
	}
	HandleStopRW(1);
	for (ulFreeUnitPTR = 0; ulFreeUnitPTR < SYSTEMAREA_DEFAULT_SCAN_RANGE; ulFreeUnitPTR++) {
		ubbadblock = 0;
		for (ubj = 0 ; ubj < gubPlanesPerBurst ; ubj++) {

			ubPlaneBank = ubj;
#if TLC
#if B0KB
			if ( ftlGoodBlock(ulFreeUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2, GOODBLK_MLC))
#else
			if ( ftlGoodBlock(ulFreeUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2))
#endif
#else
#if MicronFlashOnly
			if ( ftlGoodBlock(ulFreeUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1, GOODBLK_MLC))
#else
			if ( ftlGoodBlock(ulFreeUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1))
#endif
#endif
			{
				gubCE = (ubi << gubPlanesPerBurstLog) + ubj;
				ubbadblockperCE[gubCE]++;
				ubbadblock |= 1;
			}
		}

		if (ubbadblock == 0) {
			// 其實應該要先read做判斷的, 現在還不想用, 先參考erase all記的ulEraseFailMap
			if (gubSystemUnitFreeCount == SYSTEM_SETS - 1) {
				gubDBTUnit = ulFreeUnitPTR;

				break;
			}
			ftlAddToSystemBlockFreeTable(ulFreeUnitPTR);
		}
	}
	gubStopFTL = 0;
	if (ulFreeUnitPTR == SYSTEMAREA_DEFAULT_SCAN_RANGE) {
		UBYTE ubCEIndex = 0;
		for (ubi = 1; ubi < gubCENumber; ubi ++) {
			if (ubbadblockperCE[ubCEIndex] < ubbadblockperCE[ubi]) {
				ubCEIndex = ubi;
			}
		}
		ftlPreformatShowError(ubCEIndex, ulFreeUnitPTR, 0x80);
		BUFB_BASE[4] = 0xAA;
		BUFB_BASE[5] = ubCEIndex;
		return;
	}
	gubSystemAreaNum = ulFreeUnitPTR;
#endif
	U32 ulMPSizeInLBA = BUFL_BASE[0];

	if (HB[HB_LBA_M]) {
		gulNativeSize = ulMPSizeInLBA;
		InitFTLVariable();
	}

	U32 ulUnitSize = (U32)(gulPlanesPerUnit - gubMaxPlanesPerP2LTable - ((gulPlanesPerUnit / guwParityPlanesNum + 1) * 2)) * gubSectorsPerPlane; // sectors
	U32 ulNativeUnit = (gulNativeSize / ulUnitSize);
	if (gulNativeSize % ulUnitSize) {
		ulNativeUnit++;
	}

#if TSB_BICS4_SUPPORT
	U32 ulNeededUnitNum = ((ulNativeUnit * 100) / 99) + SYSTEM_SETS + 7 + gubTableUnitNum + FreeBlockCountThatNeedCloseTarget + ((ReserveUnit + (guoFlashUseType.B.btToshibaBiCs4 ? gubWriteProtect_NUM : WriteProtect_NUM)) << gubTotalGroupsLog); //7=VT(4)+VTchild+RUT+Parity
#else
	U32 ulNeededUnitNum = ((ulNativeUnit * 100) / 99) + SYSTEM_SETS + 7 + gubTableUnitNum + FreeBlockCountThatNeedCloseTarget + ((ReserveUnit + WriteProtect_NUM) << gubTotalGroupsLog); //7=VT(4)+VTchild+RUT+Parity
#endif
	if ((ulNativeUnit * 100) % 99) {
		ulNeededUnitNum++;
	}
	uwFWTotalUnitNum = ulNeededUnitNum;
#if TLC

#if Hynix3DV6
	if (guoFlashUseType.B.btHynix3DV6) {

		uwFWTotalUnitNum += ((U32)MIN_4P_D1_UNIT_NUM_V6 << gubSelTotalGroupsLog);
	}
	else if (guoFlashUseType.B.btHynix3DV5) {
		if (guoFlashUseType.B.btFourPlane) {
			uwFWTotalUnitNum += ((U32)MIN_4P_D1_UNIT_NUM_V5 << gubSelTotalGroupsLog);
		}
		else {
			uwFWTotalUnitNum += ((U32)MIN_2P_D1_UNIT_NUM_V5 << gubSelTotalGroupsLog);
		}
	}

#elif B0KB //Debug
	if ((gubCENumber == 5) || (gubCENumber == 10) || (gubCENumber == 11)) {
		uwFWTotalUnitNum += ((U32)MIN_2P_D1_UNIT_NUM << gubTotalGroupsLog) * gubNum_D1_Part;
	}
	else {
		uwFWTotalUnitNum += ((U32)MIN_D1_UNIT_NUM << gubTotalGroupsLog) * gubNum_D1_Part;
	}
#else
#if TSB_BICS4_SUPPORT
	if (TLC_BICS2 && (guoFlashUseType.B.btToshibaBiCs3 || guoFlashUseType.B.btToshibaBiCs4)) {
#else
	if (TLC_BICS2 && guoFlashUseType.B.btToshibaBiCs3) {
#endif
		uwFWTotalUnitNum += ((U32)gubMIN_D1_UNIT_NUM_BiCS3 << gubTotalGroupsLog) * gubNum_D1_Part;
	}
	else {
		uwFWTotalUnitNum += ((U32)MIN_D1_UNIT_NUM << gubTotalGroupsLog) * gubNum_D1_Part;
	}
#endif


#endif
	memset(BUFB_BASE, 0, 512);
#if 0
	BUFL_BASE[0] = 77;
#else
	if (gulTotalPhyUnitNum < uwFWTotalUnitNum) {
		BUFL_BASE[0] = 0xFFFFFFFF;//Return error
	}
	else {
		BUFL_BASE[0] = (gulTotalPhyUnitNum - uwFWTotalUnitNum) >> gubTotalGroupsLog;
	}
#endif
	BUFB_BASE[4] = 0xBB;
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}
	Vender_TriggerHostData();
#endif
}

void EraseCountPerCEDie(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	UBYTE ubSectorReport = (UBYTE)((gulPhyBlocksPerCE / gubDieNumber) / 128); //block record use 4-byte
	U32 ulVenderUnit;
	U32 ulIndex;
	if (gubLockReadMode == LOCK_READ_MODE_DLMC) {
		gubErrorCode = SET_ABRT;
		return;
	}
	M_WaitFTLStop();
#if(!BURNER)
	ftlSwapL2PTable(GC_LOCK_L2P_FOR_Vender_SMART, (BYTE_Lock_Read_Mode | LOCK_READ_MODE_Vender_SMART));
#endif
	gubStopFTL = 0;
	U32 *ulBuff2 = (U32*)&gulVenderSMARTBuffer[0];
	UBYTE ubBurstBank;
	U32 ulUnitPerGroup = (gulTotalPhyUnitNum >> gubTotalGroupsLog);
	UBYTE ubchannel;
	UBYTE ubTemp;


	UBYTE ubGroup = ((M_Divider(gubSM_CENo, DEF_CEPerPage_Mcl_Div)) << gubDieNumberLog) + gubSM_GroupNo;
	UBYTE ubCE = gubSM_CENo %  gubCEsPerPage;
	if (((gulPhyBlocksPerCE >> gubDieNumberLog) % 128)) {
		ubSectorReport++;
	}
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate( (void*)ulBuff2, 512 * ubSectorReport );
	}
	memset((void*)ulBuff2, 0xFF, 512 * ubSectorReport);//先全部MarkBAD
	if (ubGroup == 0) {
		if ((gubCH_CEMappingToPCE[0][0] == ubCE) || (gubCH_CEMappingToPCE[1][0] == ubCE)) {
			if (gubCH_CEMappingToPCE[0][0] == ubCE) {
				ubchannel = 0;
			}
			else { //(gubCH_CEMappingToPCE[1][0] == ubCE)
				ubchannel = 1;
			}

			ubTemp = (UBYTE)guwDBTBlock[ubchannel];
			ulBuff2[ubTemp] = EC_For_Sys;
			ubTemp = (UBYTE)guwSystemBlock[ubchannel];
			ulBuff2[ubTemp] = EC_For_Sys;
			for (ulIndex = 0; ulIndex < 2; ulIndex++) {
				if (gubSysCodeSet[ulIndex] == 0xFF) {
					continue;
				}
				ubTemp = (UBYTE)guwSysCodeBlock[ulIndex][ubchannel];
				ulBuff2[ubTemp] = EC_For_Sys;
			}
			for (ulIndex = 0; ulIndex < gubSystemSetFreeCount; ulIndex++) {
				ubTemp = (UBYTE)guwSystemBlockRemappingTable[gubSystemSetFreeTable[ulIndex]][ubchannel];
				ulBuff2[ubTemp] = EC_For_Sys;
			}
		}
	}
	UWORD uwEndUnit, uwStratUnit = 0, uwBlock_ptr;
	U32 ulEC_Temp;
	uwEndUnit = VT->guwUnitsPerGroups[ubGroup] ;
	if (ubGroup) {
		uwStratUnit = VT->guwUnitsPerGroups[ubGroup - 1] ;
	}
	for (ulVenderUnit = uwStratUnit; ulVenderUnit < uwEndUnit; ulVenderUnit++) {
		ulEC_Temp = gulEC_D3[ulVenderUnit].B.ulEraseCount;
		for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
			if (M_RUT2DTo1D(ubCE, ulVenderUnit * gubBurstsPerBank + ubBurstBank) != RUT_MarkBad) {
				if (gubRUTMixPlaneEnable) {
					uwBlock_ptr = ((M_RUT2DTo1D(ubCE, ulVenderUnit * gubBurstsPerBank + ubBurstBank) - ((ulUnitPerGroup * ubGroup) << gubBurstsPerBankLog)));
				}
				else {
					uwBlock_ptr = ((M_RUT2DTo1D(ubCE, ulVenderUnit * gubBurstsPerBank + ubBurstBank) - ulUnitPerGroup * ubGroup) << gubBurstsPerBankLog) + ubBurstBank;
				}

				if (uwBlock_ptr < (0x20000 / 4)) {	// gulVenderSMARTBuffer
					ulBuff2[uwBlock_ptr] = ulEC_Temp;
				}
			}

		}
	}

	uwEndUnit = VT->guwDieStartSFUnitIndex[ubGroup] + VT->guwExtendUnitsIndex[ubGroup] ;
	uwStratUnit = VT->guwDieStartSFUnitIndex[ubGroup];

	for (ulVenderUnit = uwStratUnit; ulVenderUnit < uwEndUnit; ulVenderUnit++) {
		for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
			if (M_RUT2DTo1D(ubCE, ulVenderUnit * gubBurstsPerBank + ubBurstBank) != RUT_Empty) {
				if (gubRUTMixPlaneEnable) {
					uwBlock_ptr = ((M_RUT2DTo1D(ubCE, ulVenderUnit * gubBurstsPerBank + ubBurstBank) - ((ulUnitPerGroup * ubGroup) << gubBurstsPerBankLog)));
				}
				else {
					uwBlock_ptr = ((M_RUT2DTo1D(ubCE, ulVenderUnit * gubBurstsPerBank + ubBurstBank) - ulUnitPerGroup * ubGroup) << gubBurstsPerBankLog) + ubBurstBank;
				}

				if (uwBlock_ptr < (0x20000 / 4)) {	// gulVenderSMARTBuffer
					ulBuff2[uwBlock_ptr] = 0xF0F0F0F0;
				}
			}
		}
	}
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv( (void*)ulBuff2, 512 * ubSectorReport );
	}
#endif
}

void CheckBadForAllCE(UBYTE ubMode)
{
	/*scan bad block for all CE and All Die
	  burner will return bad blk by DBT or scan bad by ftlGoodBlock
	  FW will return bad by DBT
	*/
#if(!RDT)
	//M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
#if(!BURNER)
	if (gubLockReadMode == LOCK_READ_MODE_DLMC) {
		gubErrorCode = SET_ABRT;
		return;
	}
	M_WaitFTLStop();
	ftlSwapL2PTable(GC_LOCK_L2P_FOR_Vender_SMART, (BYTE_Lock_Read_Mode | LOCK_READ_MODE_Vender_SMART));
	gubStopFTL = 0;
#endif
	U32 uli;

	UWORD *uwPhysicalViewBadCntBuffer = (UWORD*)&gulVenderSMARTBuffer[0];//6187 2000
	UWORD *uwLogicalViewBadCntBuffer = (UWORD*)(&gulVenderSMARTBuffer[512]);//offset 2048byte
	//uwPhysicalViewBadCntBuffer+uwLogicalViewBadCntBuffer總共512*5byte
	UBYTE *ubDBTBuffer = (UBYTE*) &BUFB_BASE[0];
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	U32 ulTotalDBTSizeInByte = gulPhyBlocksPerCE * gubCENumber;
	UBYTE ubDetectMask = 0;
	UWORD uwLogOffset;
	UBYTE ubCE, ubDie, ubPlane;

	if (BURNER) {
		if ( RDT_FindDBTBlock_Task()) {
			gubRDTDone = DBT_Checked;//not found
		}
		else {
			gubRDTDone = (DBT_Checked | DBT_Exist); //get DBT
			if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {
				ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
				gubRDTDone = DBT_Checked;
			}
		}
	}

	if (ubMode & BIT0) {
		ubDetectMask |= (BIT6 | BIT7);//early
	}
	if (ubMode & BIT1) {
		/*if (gubSM_LaterType == 1) {
			ubDetectMask = BIT2;//erase later
		}
		else if (gubSM_LaterType == 2) {
			ubDetectMask = BIT1;//write later
		}
		else if (gubSM_LaterType == 3) {
			ubDetectMask = BIT0;//read later
		}
		else {*/
		ubDetectMask |= (BIT4 | BIT5);//all later
		//}
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate( (void*)uwPhysicalViewBadCntBuffer, 512 * 5 );
	}

	memset((void*)uwPhysicalViewBadCntBuffer, 0x00, 512 * 5);

#if BURNER
	UBYTE ubIsBad, ubGroup, ubPlaneBank , ubDieOffset;
	U32 ulUnitPTR;

	if ((gubRDTDone & DBT_Exist) == 0) {
		U32 ulUnitsPerGroup = gulTotalPhyUnitNum >> gubTotalGroupsLog;
		UBYTE ubCEGroup = gubTotalGroups >> gubDieNumberLog;
		for (uli = 0; uli < ulUnitsPerGroup; uli++) {
			for (ubDieOffset = 0; ubDieOffset < gubSelDieNumber; ubDieOffset++) {
				for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
					ulUnitPTR = (ubGroup * gubSelDieNumber + ubDieOffset) * ulUnitsPerGroup + uli;
					for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
						if (ENABLE_SANDISK_CHECKEARLYBADBYPROG) {
							ubIsBad = ftlGoodBlock_SandiskCheckByProgFail(ulUnitPTR, ubPlaneBank);
						}
						else {
#if TLC
#if B0KB
							ubIsBad = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubDBTBuffer, 2, GOODBLK_MLC);
#else
							ubIsBad = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubDBTBuffer, 2);
#endif
#else
#if MicronFlashOnly
							ubIsBad = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubDBTBuffer, 1, GOODBLK_MLC);
#else
							ubIsBad = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubDBTBuffer, 1);
#endif
#endif
						}

#if EnableProductHistory
						// For PH cannot be checked by ftlGoodBlock, Turn PHUnit into EarlyBad by FindPHBlockByScan
						if ((gubPHinit & 0xF) == 0) { //沒有init過
							//no DBT
							if (FindPHBlockByScan(0)) {
								//scan fail
								gubPHinit |= Bit3_InitByScan_NotFind;
							}
							else {
								//scan pass
								gubPHinit |= Bit2_InitByScan_Find;
							}
						}
						if (((U32)guwPHUnit[0].uwUnit == ulUnitPTR) && ((U32)guwPHUnit[0].ubPlanebank == ubPlaneBank) ) {
							ubIsBad = TRUE;
						}
						if (((U32)guwPHUnit[1].uwUnit == ulUnitPTR) && ((U32)guwPHUnit[1].ubPlanebank == ubPlaneBank) ) {
							ubIsBad = TRUE;
						}
#endif

						if (ubIsBad) {
							//先以Logical_CE的順序去放.統計完最後再轉成Physical_CE順序回傳給AP
							ubCE = (gubCEsPerPage * ubGroup) + (ubPlaneBank >> gubBurstsPerBankLog);
							ubDie = ubDieOffset;
							ubPlane = ubPlaneBank & gubBurstsPerBankMask;
							if (ENABLE_PSEUDO_CE && gubExpandCE) {
								if (ubCE >= (gubCENumber / gubExpandCE_Ratio)) {
									ubDie = gubDieNumber * (ubCE / (gubCENumber / gubExpandCE_Ratio)) + ubDie;
									M_VirtualToPhysicalCE(ubCE);//Decode Pseudo CE Mapping
								}
							}
							//以一個CE裡有4die,4plane去預留空間,一格2Byte
							uwLogOffset = ((UWORD)(ubCE << 4)) + ((UWORD)(ubDie << 2)) + (UWORD)ubPlane;
							uwLogicalViewBadCntBuffer[uwLogOffset]++;
						}
					}

				}
			}
		}
	}
	else {
#endif
		HandleStopRW(1);
		if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {
			if (BURNER) {
				ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
			}
			if (ENABLE_DEBUG_RW) {
				flaLEDBlink(1);
			}
			gubStopFTL = 0;
			return;
		}
		gubStopFTL = 0;

		//logical CE view to scan bad
		for (uli = 0; uli < ulTotalDBTSizeInByte ; uli++) {
			if (uli % ulEntryPerDBTPlane == 0) {
				if (ftlLoadDBT(uli, (U32)&ubDBTBuffer[0])) {
					if (BURNER) {
						ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
					}
					if (ENABLE_DEBUG_RW) {
						flaLEDBlink(1);
					}
					gubStopFTL = 0;
					return;
				}
			}

			if (ubDBTBuffer[uli % ulEntryPerDBTPlane] & ubDetectMask) {
				//先以Logical_CE的順序去放.統計完最後再轉成Physical_CE順序回傳給AP
				ubCE = (uli >> gubBurstsPerBankLog) % gubCENumber;
				ubDie = (uli / (gubBurstsPerBank * gubCENumber)) % gubDieNumber;
				ubPlane = uli & gubBurstsPerBankMask;
				if (ENABLE_PSEUDO_CE && gubExpandCE) {
					if (ubCE >= (gubCENumber / gubExpandCE_Ratio)) {
						ubDie = gubDieNumber * (ubCE / (gubCENumber / gubExpandCE_Ratio)) + ubDie;
						M_VirtualToPhysicalCE(ubCE);//Decode Pseudo CE Mapping
					}
				}
				//以一個CE裡有4die,4plane去預留空間,一格2Byte
				uwLogOffset = ((UWORD)(ubCE << 4)) + ((UWORD)(ubDie << 2)) + (UWORD)ubPlane;
				uwLogicalViewBadCntBuffer[uwLogOffset]++;
			}
		}
#if BURNER
	}
#endif
	//Translate Logical_CE to Physical_CE
	for (uli = 0 ; uli < gubCENumber / gubExpandCE_Ratio ; uli++) {
		UBYTE ubPhysicalCEIndex ;
		ubPhysicalCEIndex = TranslateLogicalCEtoPhysicalCE(uli);
		mDMAC_COPY((U32)&uwLogicalViewBadCntBuffer[16 * uli], (U32)&uwPhysicalViewBadCntBuffer[16 * ubPhysicalCEIndex], 32, 0, 0, 0, 0, 1);
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv( (void*)uwPhysicalViewBadCntBuffer, 512 * 5 );
	}
#endif
}

void CheckBadPerCE(UBYTE ubMode)
{
#if(!RDT)
	//M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
#if(!BURNER)
	if (gubLockReadMode == LOCK_READ_MODE_DLMC) {
		gubErrorCode = SET_ABRT;
		return;
	}
	M_WaitFTLStop();
	ftlSwapL2PTable(GC_LOCK_L2P_FOR_Vender_SMART, (BYTE_Lock_Read_Mode | LOCK_READ_MODE_Vender_SMART));
	gubStopFTL = 0;
#endif
	if (BURNER) {
		if ( RDT_FindDBTBlock_Task()) {
			gubRDTDone = DBT_Checked;	// not found
			gubErrorCode = SET_ABRT;
			return;
		}
		else {
			gubRDTDone = (DBT_Checked | DBT_Exist);	// DBT found
		}
	}
	U32 ulVenderUnit;
	UWORD *uwBuff2 = (UWORD*)&gulVenderSMARTBuffer[0];
	UBYTE *ubDBTBuffer = (UBYTE*) &BUFB_BASE[0];
	U32 ulBadCnt[4], ulBadCntAllDie = 0;

	UBYTE ubBurstBank;

	U32 ulUnitPerGroup = (gulTotalPhyUnitNum >> gubTotalGroupsLog);

	U32 ulTemp, ulTemp1 = 0, ulTemp2;
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......

	UBYTE ubGroup = (gubTotalGroups >> gubDieNumberLog) * gubSM_GroupNo + (M_Divider(gubSM_CENo, DEF_CEPerPage_Mcl_Div));
	UBYTE ubCE = gubSM_CENo %  gubCEsPerPage; //MaxIL * channel
	//UBYTE ubLogical_Group = gubSM_CENo /  gubCEsPerPage + gubSM_GroupNo;

	UBYTE ubPlaneOffset;
	UBYTE ubSectorReport;

	UBYTE ubDetectMask;
	UBYTE ubPlanePTR = 0xFF;

	ubDetectMask = 0;
	if (ubMode & BIT0) {
		ubDetectMask |= (BIT6 | BIT7);
	}

	if (ubMode & BIT1) {
		if (gubSM_LaterType == 1) {
			ubDetectMask = BIT2;
		}
		else if (gubSM_LaterType == 2) {
			ubDetectMask = BIT1;
		}
		else if (gubSM_LaterType == 3) {
			ubDetectMask = BIT0;
		}
		else if (gubSM_LaterType == 4) {
			ubDetectMask = BIT3;
		}
		else if (gubSM_LaterType == 5) {
			ubDetectMask = (BIT6 | BIT7);
		}
		else {
			ubDetectMask |= (BIT4 | BIT5);
		}
	}

	if (ubMode & BIT3) {
		ubSectorReport = (UBYTE)(gulPhyBlocksPerCE / 256); //block record use 2-byte
		if ((gulPhyBlocksPerCE % 128)) {
			ubSectorReport++;
		}
	}
	else {
		for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
			ulBadCnt[ubBurstBank] = 0;
		}
		ubSectorReport = 1;
	}


	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate( (void*)uwBuff2, 512 * ubSectorReport );
	}
	if (ubMode & BIT3) {
		memset((void*)uwBuff2, 0xFF, 512 * ubSectorReport);
	}
	else {
		memset((void*)uwBuff2, 0x00, 512 * ubSectorReport);
	}

	if (ENABLE_PLANE_CH_CE) {
		ubPlaneOffset = (M_Multiplier(ubGroup, DEF_PlanesPerPage_Mcl_Div)) + (ubCE << gubBurstsPerBankLog);
	}
	else {
		ubPlaneOffset = (M_Multiplier(ubGroup, DEF_PlanesPerPage_Mcl_Div)) + ((ubCE >> gubPlanesPerBurstLog) << gubPlanesPerBankLog) + (ubCE % gubPlanesPerBurst);
	}


	ubPlanePTR = 0;
	HandleStopRW(1);
	if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {
		if (ENABLE_DEBUG_RW) {
			flaLEDBlink(1);
		}
		gubStopFTL = 0;
		return;
	}
	gubStopFTL = 0;

	for (ulVenderUnit = 0; ulVenderUnit < ulUnitPerGroup; ulVenderUnit++) {
		ulTemp = (M_Multiplier((ulVenderUnit << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + ubPlaneOffset;
		ulTemp1 = ulTemp % ulEntryPerDBTPlane;

		/*if ((ubMode & BIT3) == 0) {
			if ((ubLogical_Group == 0) && (ulVenderUnit < gubSystemAreaNum)) {
				if ((ubDBTBuffer[ulTemp1 - ubPlaneOffset]&ubDetectMask) || (ubDBTBuffer[ulTemp1 - ubPlaneOffset + (gubBurstsPerBank * (gubPlanesPerBurst - 1))]&ubDetectMask)) {
					for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
						ulBadCnt[ubBurstBank] ++;
					}
					continue;
				}
			}
		}*/

		for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
			ulTemp1 = ulTemp % ulEntryPerDBTPlane;
			ulTemp2 = ulTemp / ulEntryPerDBTPlane;
			if (ulTemp2 != ubPlanePTR) {
				ubPlanePTR = ulTemp2;
				HandleStopRW(1);
				if (ftlLoadDBT(ulTemp, (U32)&ubDBTBuffer[0])) {
					ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
					if (ENABLE_DEBUG_RW) {
						flaLEDBlink(1);
					}
					gubStopFTL = 0;
					return;
				}
				gubStopFTL = 0;
			}

			if ((ubMode & BIT1) && (gubSM_LaterType == 5) && ((ubDBTBuffer[ulTemp1]&ubDetectMask) == ubDetectMask)) {
				if (ubMode & BIT3) {
					uwBuff2[ulBadCntAllDie] = (ulVenderUnit << gubBurstsPerBankLog) + ((guwDieBlockShift << gubBurstsPerBankLog) * gubSM_GroupNo) + ubBurstBank;
					ulBadCntAllDie ++;
				}
				else {
					ulBadCnt[ubBurstBank] ++;
				}
			}
			else if ((((ubMode & BIT1) && (ubDBTBuffer[ulTemp1] & 0x30)) || (ubMode & BIT0)) //Later || Early
			         && (ubDBTBuffer[ulTemp1]&ubDetectMask)) { // Check bit

				if (ubMode & BIT3) {
					uwBuff2[ulBadCntAllDie] = (ulVenderUnit << gubBurstsPerBankLog) + ((guwDieBlockShift << gubBurstsPerBankLog) * gubSM_GroupNo) + ubBurstBank;
					ulBadCntAllDie ++;
				}
				else {
					ulBadCnt[ubBurstBank] ++;
				}
			}

			ulTemp ++;

		}
	}


	if ((ubMode & BIT3) == 0) {
		UBYTE ubTemp = 0;
		for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
			uwBuff2[ubTemp] = ulBadCnt[ubBurstBank];
			ubTemp ++;
			uwBuff2[ubTemp] = ulBadCnt[ubBurstBank] >> 16;
			ubTemp ++;
			//BUFL_BASE[0] = ulBadCnt;
		}
	}
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv( (void*)uwBuff2, 512 * ubSectorReport );
	}
#endif
}

void SMART_Vender(void)
{
	M_CheckRDTModeToDisableFunction();
	UBYTE ubMode;
	if ((HB[HB_FEATURE] == 0x70) || (HB[HB_FEATURE] == 0xD6)) {

		HW[HW_DATA_TRG] |= H_DATA_TRG;
		while (HW[HW_DATA_TRG]&H_DATA_TRG);
		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(BUFB_BASE, 512);
		}
		gubSM_CENo = BUFB_BASE[0];
		if (ENABLE_VenderCMD_return_PhysicalCE) {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				UBYTE ubBreak = 0;
				UBYTE ubi, ubj ;
				for (ubj = 0; ubj < MAX_CHANNEL ; ubj++) {
					//ubj = (ubCE % gubPlanesPerBurst);
					for (ubi = 0; ubi < MAX_CE_PER_CH; ubi++) {
						if ((gubCH_CEMappingToPCE[ubj][ubi] + ubj * gubMAX_SCAN_CE_PER_CH) == gubSM_CENo ) {
							/*UartString("\n Physical CE:");
							Uart_Tx_DataHex(gubSM_CENo);*/
							gubSM_CENo = ubi * MAX_CHANNEL + ubj;
							/*UartString(", Mapping to Global CE:");
							Uart_Tx_DataHex(gubSM_CENo);*/
							ubBreak = 1;
							break;
						}
					}
					if (ubBreak) {
						break;
					}
				}
			}
			else {
				UBYTE ubCE_temp;
				if (FCONB[(FCONL_FCE_SEL_0 << 2) + gubSM_CENo] == 0x3F) {
					gubErrorCode = 0x11;
					return;
				}
				else {
					if (gubPlanesPerBurst == 2) {
						ubCE_temp = FCONB[(FCONL_FCE_SEL_0 << 2) + gubSM_CENo];
						gubSM_CENo = ((ubCE_temp % 8) << 1) + (ubCE_temp >> 3);
					}
					else {
						gubSM_CENo = FCONB[(FCONL_FCE_SEL_0 << 2) + gubSM_CENo];
					}
				}
			}
		}
		ubMode = BUFB_BASE[1];
		gubSM_GroupNo = BUFB_BASE[2];

		gubSM_LaterType = BUFB_BASE[3];

		if (ENABLE_PSEUDO_CE && gubExpandCE) {  //Load physical CE, translate it to logical view
			if (gubSM_GroupNo >= gubSelDieNumber) {
				gubSM_CENo += (gubSM_GroupNo / gubSelDieNumber) * gubMTQTotalPhyCE;
				gubSM_GroupNo = gubSM_GroupNo % gubSelDieNumber;
				if (ENABLE_SANDISK_1ZNM_SPF) {
					if ((gubSM_CENo >= gubCENumber)) {
						gubSM_CENo -= (gubHideCE * gubPlanesPerBurst);
					}
				}
			}
		}
		if ((gubSM_CENo >= gubCENumber) || (gubSM_GroupNo >= gubSelDieNumber)) {
			gubErrorCode = 0x11;
			return;
		}
		switch (ubMode) {
				// bit0: early
				// bit1: later
				// bit3: addr.
			case 0x73:
				CheckBadForAllCE(BIT0 | BIT1);//AllBad count
				break;
			case 0x74:
				CheckBadForAllCE(BIT0);//AllEarlyBad count
				break;
			case 0x75:
				CheckBadForAllCE(BIT1);//AllLaterBad count
				break;
			case 0x76:
				CheckBadPerCE(BIT0 | BIT1); //AllBad count
				break;
			case 0x77:
				CheckBadPerCE(BIT0 | BIT1 | BIT3); //AllBad
				break;
			case 0x78:
				CheckBadPerCE(BIT1);//LaterBad count
				break;
			case 0x79:
				CheckBadPerCE(BIT1 | BIT3); //LaterBad
				break;
			case 0x7A:
				CheckBadPerCE(BIT0);//EarlyBad count
				break;
			case 0x7B:
				CheckBadPerCE(BIT0 | BIT3); //EarlyBad
				break;
			case 0x72:
				EraseCountPerCEDie();
				break;
			case 0x7C:
				BurnerCheckEarlyBad(BIT0);// for count
				break;
			case 0x7D:
				BurnerCheckEarlyBad(BIT1);// for address
				break;
			default:
				break;

		}

		//Uart_Tx_Data(0x61);
	}
	else {

		if (!BURNER) {
			//避免 Smart & BG 搶L2P ram
			if (gubLockReadMode != LOCK_READ_MODE_Vender_SMART) {
				gubErrorCode = SET_ABRT;
				return;
			}
		}

		UWORD uwi;
		for ( uwi = 0; uwi < gulSectorCnt; uwi++) {
			//if ((HB[HB_FEATURE] == 0x7C) || (HB[HB_FEATURE] == 0x7D)) { //Because data buffer only 16k under burner mode,  use gubBuffer to store
			if (BURNER) {
				mDMAC_COPY((U32)&guwVenderSMARTBuffer[256 * uwi], (U32)&BUFB_BASE[0], 512, 0, 0, 0, 0, 1);
			}
			else {
				mDMAC_COPY((U32)&gulVenderSMARTBuffer[128 * uwi], (U32)&BUFB_BASE[0], 512, 0, 0, 0, 0, 1);
			}
			//}

			if ( gubAP_enable) {
				HL[HL_BUF_OFST] = 0;
				HL[HL_BUF_TRG_SEC] = 1;
				//HL[HL_LAST_END_LBA] =0;
				HL[HL_TQ_CONTENT_31] = 0;
				Vender_HostRemainSectorCount();
			}
			else {
				//RWBuffer_forSMARTLog(1);
			}
		}
		if (!BURNER) {
			ftlSwapL2PTable(guwLockReadNum, BYTE_Free_Read_Mode);
#if BICS2_GC_TO_D3
			ftlSwapL2PTable(guwLockWriteNum, BYTE_Free_Write_Mode);
#endif
		}
	}
}

void Vender_Send_Seed()
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	UWORD uwi;
	UWORD uwi2;

	memset(&gulSectionInformation, 0x00, (sizeof(U32) * 24));
	gulBurnerSeedOrigin = 0;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)BUFB_BASE, 512 * gulSectorCnt);
	}

	Vender_TriggerHostData();
	if (BURNER) {
		UartString("Seed Header Received\r\n");
	}
	for (uwi = 0; uwi < gulSectorCnt; uwi ++) {
		//RTT1_Stop();
		if (uwi == 0) { //// Get section information
			for (uwi2 = 0; uwi2 < 8; uwi2++) {
				gulSectionInformation[BINHEADER_WRITE_ADDR][uwi2] = BUFL_BASE[(uwi2 << 1)]; // write destination address
				gulSectionInformation[BINHEADER_READ_ADDR][uwi2]	= BUFL_BASE[16 + (uwi2 << 1)]; // header read destination address
				gulSectionInformation[BINHEADER_SECTION_SIZE][uwi2] = BUFL_BASE[1 + (uwi2 << 1)]; // each section size
			}
		}
		else {
			if (uwi > 1) {
				gulBurnerSeedOrigin ^= BUFL_BASE[(uwi - 2) + uwi * (512 / 4)];
			}
		}


	}// for loop end
	gulCurrentSeed = gulBurnerSeedOrigin;
	guloffset = 0;
	gubindex = 0;
#endif
}


void Vender_Prog_RAM_Whole()
{
	M_CheckRDTModeToDisableFunction();
#if (BURNER)
	UBYTE *ubBUF_64Kshift = (UBYTE *)(BUFFER3_BASE + 0x10000);
	U32 ulTriggerSec = gulSectorCnt;
	U32 ulBuf_ptr = 0;
	UBYTE ubi = gubindex;

	UBYTE ubDeRanCtr = 0;

	Vender_TriggerHostData();
	for (ubi = gubindex ; ubi < 8 ; ubi++) {
		U32 ulRemainSec = gulSectionInformation[2][ubi] / 0x200 - guloffset;
		U32 ulTargetAddr = gulSectionInformation[0][ubi] + guloffset * 0x200;

		if (ulRemainSec == 0) {
			continue;
		}


		if (ulRemainSec > ulTriggerSec) {
			if (ulTriggerSec) {
				mDMAC_SV(1, (U32)&ubBUF_64Kshift[0], ulTriggerSec * 0x200, gulCurrentSeed, gulCurrentSeed, 0, DMA_EN_RAND | DMA_EN_SETCQ, 1);
				mDMAC_XOR((U32)&ubBUF_64Kshift[0], (U32)&BUFB_BASE[ulBuf_ptr], ulTargetAddr, ulTriggerSec * 0x200, 1, 0);
				gulCurrentSeed += ulTriggerSec;
			}
			guloffset += ulTriggerSec;
			gubindex = ubi;
			return;
		}
		else {

			mDMAC_SV(1, (U32)&ubBUF_64Kshift[0], ulRemainSec * 0x200, gulCurrentSeed, gulCurrentSeed, 0, DMA_EN_RAND | DMA_EN_SETCQ, 1);
			mDMAC_XOR((U32)&ubBUF_64Kshift[0], (U32)&BUFB_BASE[ulBuf_ptr], ulTargetAddr, ulRemainSec * 0x200, 1, 0);

			//add DeRandomizer function here
			// for each section,DeRan each 16K to a 16K Buffer, then copy it back to original place
			if (ENABLE_BURNER_USE_RANDOMIZER_DECODE) {
				for (ubDeRanCtr = 0; ubDeRanCtr < (gulSectionInformation[2][ubi] / 0x4000); ubDeRanCtr++) {
					FlaFlashDeRandomize( (U32*)(gulSectionInformation[0][ubi] + (ubDeRanCtr * 0x4000)), (U32*)&ubBUF_64Kshift[0], 4);
					mDMAC_COPY( (U32)&ubBUF_64Kshift[0], (gulSectionInformation[0][ubi] + (ubDeRanCtr * 0x4000)), 16384, 0, 0, 0, 0, 1);
				}
			}
			// add done

			gulCurrentSeed += ulRemainSec;
			ulTriggerSec -= ulRemainSec;
			ulBuf_ptr += ulRemainSec * 0x200;
			guloffset = 0;
		}
	}

	UBYTE ubResult = 0;
	for (ubi = 0; ubi < 8; ubi++) {
		if (gulSectionInformation[2][ubi] != 0) {
			ubResult |= mDMAC_CRC(gulSectionInformation[0][ubi], gulSectionInformation[2][ubi], Burner_CRC32_Seed, 1, (DMA_CRC_CHK | DMA_CRC_LEN_8 | DMA_CRC_DCCM));
		}
	}

	if (ubResult == 0) { //CRC Pass
		HB[HB_LBA_H] = 0xBB;
	}
	else {
		HB[HB_LBA_H] = 0xAA;
	}
#endif
}

void Vender_Trim_Set_PIO(void)
{
	M_CheckRDTModeToDisableFunction();
	gubPIOTirm = 1;
	Vender_TriggerHostData();

}

void Vender_TrimAll_Test(void)
{
#if(!RDT)
	//UartString("\nTrimAll");
	TrimAll();
#endif
}

#if (BURNER&&ENABLE_VENDER_READ_VTH)
void Vender_FLH_Distribution_Read_Setting(void)
{
	M_CheckRDTModeToDisableFunction();
	UBYTE ubChannel, ubCEIndex, ubSLCMode, ubReadMode, ubDepth, ubL4KIndex, ubLMU;
	U32 ulTempFLHInterface, ulTempDMACFG, ulTempRSCFG, ulTempZIPCFG, ulTempINTCFG;
	UWORD uwPage;
	if (HB[HB_DEVICE] != 0xE0) {
		gubErrorCode = SET_ABRT;
		return;
	}
	if (gubIsPHSQLClockSet == FALSE) {
		flaSetFlashMode();
		flaSet_ODT_DRIVE();
		if (gubFlashClock > FLH_CLK_100MHz) {
			gubFlashClock = FLH_CLK_100MHz;
		}
		flaSwitchClock(gubFlashClock);
		gubIsPHSQLClockSet = TRUE;
	}
	U32 ulTOP_TGL_TIME_CFG_BACKUP = FCONL[FCONL_TGL_TIME_CFG];
	U32 ulTOP_TGL_TIME_CFG_1_BACKUP = FCONL[FCONL_TGL_TIME_CFG_1];
	if (gubFlashClock <= FLH_CLK_100MHz) { // Fix read row data bug under 100MHz
		FCONL[FCONL_TGL_TIME_CFG] &= 0x00FFFFFF;
		FCONL[FCONL_TGL_TIME_CFG] |= (TOG_DAT_PSTH_PHASE_CNT(3) | TOG_DAT_CLH_PHASE_CNT(5));

		FCONL[FCONL_TGL_TIME_CFG_1] &= 0x00FFFFFF;
		FCONL[FCONL_TGL_TIME_CFG_1] |= (TOG_DAT_PSTH_PHASE_CNT(3) | TOG_DAT_CLH_PHASE_CNT(5));
	}
	mDMAC_SV(4, (U32)&BUFB_BASE[0],  18944, 0, 0, 0, DMA_EN_SETCQ, 0);
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
	Vender_TriggerHostData_31();
	gulSectorCnt = 0;
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)BUFB_BASE, 512);
	}
	ubSLCMode = BUFB_BASE[0] & BIT2;
	ubReadMode = BUFB_BASE[0] & 3;
	ubCEIndex = BUFB_BASE[1];
	uwPage = BUFW_BASE[1];
	ubChannel = (ubCEIndex >= 8) ? 1 : 0;

	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
	M_DisableAllFlashCE();
	FCONL[FCONL_FCE_ENB] |= (1 << ubCEIndex);

	gFREG[FCTLL_FTA_COL].B.B0 = 0;
	gFREG[FCTLL_FTA_COL].B.B1 = 0;
	if (TLC) {
		if (ubReadMode == 0) {
			gFREG[FCTLL_FTA_ROW].B.B0 = uwPage;// already WL
		}
		else {
			gFREG[FCTLL_FTA_ROW].B.B0 = uwPage / 3;
		}
	}
	else {
		gFREG[FCTLL_FTA_ROW].B.B0 = uwPage;
	}
	gFREG[FCTLL_FTA_ROW].B.B1 = BUFB_BASE[4];
	gFREG[FCTLL_FTA_ROW].B.B2 = BUFB_BASE[5];
	ulTempDMACFG = gFREG[FCTLL_DMA_CFG].L;
	ulTempRSCFG = gFREG[FCTLL_RS_CFG].L;
	ulTempZIPCFG = gFREG[FCTLL_ZIP_CFG].L;
	ulTempINTCFG = gFREG[FCTLL_INT_CFG].L;
	gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME);
	gFREG[FCTLL_DMA_CFG].L |= ((UWORD)gub4kEntrysPerPlane << 8);
	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // turn off RS
	gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
	gFREG[FCTLL_ZIP_CFG].L &= (~L4K_BADR_CONTINUOUS_MODE);
	gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);
	M_GetMTDepth(ubDepth);
	gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
	gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
	gubINTTable[ubDepth].All = 0;
	gFREG[FCTLL_RAW_DMA_ADR].L = (U32)BUFFER3_BASE;
	if (ubReadMode == 0) { //Distribution analysis, dump raw data with shift read parameter.
		ulTempFLHInterface = gFREG[FCTLL_FLH_SET].L;
		gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
		gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x5C;
		gFREG[FCTLL_PIO_CMD].B.B0 = 0xC5;
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
		gFREG[FCTLL_PIO_DAT].B.B0 = 0x01; //以上為Entry code
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x04;
		gFREG[FCTLL_PIO_DAT].B.B0 = BUFB_BASE[10];// 4:LSB
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x05;
		gFREG[FCTLL_PIO_DAT].B.B0 = BUFB_BASE[11];// 5:MSB
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x06;
		gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;//always 1
		gFREG[FCTLL_PIO_CMD].B.B0 = 0xBC;

		gFREG[FCTLL_FLH_SET].L = ulTempFLHInterface;

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C00_A5_CAE;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C3D;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C05_A5_CE0_RAW;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		if (gubINTTable[ubDepth].All & 0xF0) {
			flaLEDBlink(gubINTTable[ubDepth].All);
		}
	}
	else if (ubReadMode == 1) { //Dump raw data
		if (ubSLCMode) {
			gFREG[FCTLL_PIO_CMD].B.B0 = 0xA2;
		}
		else if (TLC) {
			ubLMU = uwPage % 3;
			if (ubLMU == 0) {
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x01;
			}
			else if (ubLMU == 1) {
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x02;
			}
			else if (ubLMU == 2) {
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x03;
			}
			else {
				gubErrorCode = SET_ABRT;
				return;
			}
		}
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C00_A5_C30;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C05_A5_CE0_RAW;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		if (gubINTTable[ubDepth].All & 0xF0) {
			flaLEDBlink(gubINTTable[ubDepth].All);
		}
	}
	else {
		gubErrorCode = SET_ABRT;
		return;
	}
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	// Polling TRUE RDY
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	if (gubINTTable[ubDepth].All & 0xF0) {
		gubINTTable[ubDepth].All = 0x00;
		gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN);  // no need INT for polling
		if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
			gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
		}
	}
	M_AddMTDepth(ubDepth);
	gFREG[FCTLL_DMA_CFG].L = ulTempDMACFG;
	gFREG[FCTLL_RS_CFG].L = ulTempRSCFG;
	gFREG[FCTLL_ZIP_CFG].L = ulTempZIPCFG;
	gFREG[FCTLL_DMA_CFG].L = ulTempDMACFG;
	gFREG[FCTLL_INT_CFG].L = ulTempINTCFG;
	if (gubFlashClock <= FLH_CLK_100MHz) { // Fix read row data bug under 100MHz
		FCONL[FCONL_TGL_TIME_CFG] = ulTOP_TGL_TIME_CFG_BACKUP;
		FCONL[FCONL_TGL_TIME_CFG_1] = ulTOP_TGL_TIME_CFG_1_BACKUP;
	}
	M_DisableAllFlashCE();
	for (ubL4KIndex = 0; ubL4KIndex < gub4kEntrysPerPlane; ubL4KIndex++) {
		memcpy((void *)(&BUFB_BASE[ubL4KIndex * guwDMAFrameSize]), (void *)(&BUFB_BASE[ubL4KIndex * 5120]), guwDMAFrameSize);
	}

	memset((void *)(&BUFB_BASE[ubL4KIndex * guwDMAFrameSize]), 0, 5120 * gub4kEntrysPerPlane - guwDMAFrameSize * gub4kEntrysPerPlane);
}

void Vender_FLH_Distribution_Read(void)
{
	M_CheckRDTModeToDisableFunction();
	if (HB[HB_DEVICE] != 0xE0) {
		gubErrorCode = SET_ABRT;
		return;
	}
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, gulSectorCnt * 512);
	}
	if (gulSectorCnt != 37) {
		flaLEDBlink(0xFF);
	}
	HL[HL_BUF_OFST] = HB[HB_LBA_L] * 512;
	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	HL[HL_TQ_CONTENT_31] = 0;
	Vender_HostRemainSectorCount();
	gulSectorCnt = 0;
}
#endif

void Vender_BadBlockInfo(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	typedef struct {
		UBYTE ubValid;
		UBYTE ubCE;
		UBYTE ubDie;
		UBYTE ubBurstBank;
	} DIE_INFO;

	typedef struct {
		U32 ulValid;
		U32 ulEarlyBadCnt;
		U32 ulLaterBadCnt;
		U32 ulBadBlockPercentage;
		DIE_INFO ulDieInfo[2];
		UWORD uwSystemArea[2];
		U32 ulReserved1;
	} BADBLOCKENTRY;

	BADBLOCKENTRY *BadBlockEntry;
	UWORD uwEntryIndex = 0;
	UBYTE ubPlane;
	UBYTE ubGroup;
	UWORD uwIndex;
	UBYTE ubCE, ubDie;

	//---------------------------------------------------------//
	UWORD *uwEarlyBadCntPerPlane = (UWORD *)&BUFB_BASE[0x2200];
	UWORD *uwLaterBadCntPerPlane = (UWORD *)&BUFB_BASE[0x2400];
	if (ENABLE_RUT2DBT) {
		U32 ulEntryPerDBTPlane; // 一個plane存了幾個block state......
		U32 ulCount;
		U32 ulDBTEntryIndex;
		UBYTE *ubDBTBuffer = (UBYTE*) gubHostTable;
		U32 ulTotalBlockNum;
		U32 ulTotalBlksPerPlaneBank;

		ulEntryPerDBTPlane = (U32)(gubSectorsPerPlane * 512);
		ulTotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;
		ulTotalBlksPerPlaneBank = gubDieNumber * (gubBurstsPerBank) * (gubCEsPerPage);

		for (ulCount = 0; ulCount < 256; ulCount++) {
			uwEarlyBadCntPerPlane[ulCount] = 0;
			uwLaterBadCntPerPlane[ulCount] = 0;
		}

		HandleStopRW(1);
		for (ulCount = 0; ulCount < ulTotalBlockNum; ulCount++) {
			ulDBTEntryIndex = (ulCount % ulEntryPerDBTPlane);
			if (ulDBTEntryIndex == 0) {
				if (ftlLoadDBT(ulCount, (U32)&ubDBTBuffer[0])) { // load single plane
					ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
				}
			}

			/// EarlyBad Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & 0xC0) {
				uwEarlyBadCntPerPlane[(ulCount % ulTotalBlksPerPlaneBank)]++;
			}

			//LaterBad Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & 0x30) {
				uwLaterBadCntPerPlane[(ulCount % ulTotalBlksPerPlaneBank)]++;
			}
		}
		gubStopFTL = 0;
	}
	//---------------------------------------------------------//


	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512 * 17);
	}
	memset(&BUFB_BASE[0], 0, 512 * 17);
	BUFL_BASE[0] = gubCENumber;
	BUFL_BASE[1] = gubDieNumber;
	BUFL_BASE[2] = gubBurstsPerBank;
	BUFL_BASE[3] = guwBadBlockBoundary;


	BadBlockEntry = (BADBLOCKENTRY *)&BUFB_BASE[512];

	for ( ubGroup = 0; ubGroup < gubTotalGroups; ubGroup++) {
		for ( ubPlane = 0; ubPlane < gubPlanesPerPage; ubPlane++) {
			uwIndex = ubGroup * gubPlanesPerPage + ubPlane;

			BadBlockEntry[uwEntryIndex].ulValid = 1;

			if (ENABLE_RUT2DBT) {
				BadBlockEntry[uwEntryIndex].ulEarlyBadCnt = uwEarlyBadCntPerPlane[uwIndex];
				BadBlockEntry[uwEntryIndex].ulLaterBadCnt = uwLaterBadCntPerPlane[uwIndex ];
			}
			else {
				gubErrorCode = SET_ABRT;
				return;
			}
			guwTemp = BadBlockEntry[uwEntryIndex].ulEarlyBadCnt + BadBlockEntry[uwEntryIndex].ulLaterBadCnt;
			if (guwTemp > guwBadBlockBoundary) {
				BadBlockEntry[uwEntryIndex].ulBadBlockPercentage = 100;
			}
			else {
				BadBlockEntry[uwEntryIndex].ulBadBlockPercentage = (guwTemp * 100) / guwBadBlockBoundary;
			}

			ubCE = (M_Multiplier((ubGroup >> gubDieNumberLog), DEF_CEPerPage_Mcl_Div)) + ( ubPlane >> gubBurstsPerBankLog);
			ubDie = ubGroup & gubDieNumberMask;

			if (gubExpandCE && ENABLE_PSEUDO_CE) {
				ubDie += (ubCE / gubMTQTotalPhyCE) * gubDieNumber;
				ubCE = ubCE % gubMTQTotalPhyCE;
			}
			if (ENABLE_VenderCMD_return_PhysicalCE) {
				ubCE = TranslateLogicalCEtoPhysicalCE(ubCE);
			}
			BadBlockEntry[uwEntryIndex].ulDieInfo[0].ubValid = 1;
			BadBlockEntry[uwEntryIndex].ulDieInfo[0].ubCE = ubCE;
			BadBlockEntry[uwEntryIndex].ulDieInfo[0].ubDie = ubDie;
			BadBlockEntry[uwEntryIndex].ulDieInfo[0].ubBurstBank = (ubPlane & gubBurstsPerBankMask);

			if (ubGroup == 0) {
				BadBlockEntry[uwEntryIndex].uwSystemArea[0] = guwSystemAreaNum[0];
				BadBlockEntry[uwEntryIndex].uwSystemArea[1] = guwSystemAreaNum[1];
			}

			uwEntryIndex++;
		}
	}
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512 * 17);
	}
	Vender_TriggerHostData();
#endif
}

#if (TLC_TSB && !RDT)
void Vender_Direct_RawRead_TSB(void)
{
	M_CheckRDTModeToDisableFunction();

	U32 ulUserUnit, ulUserEntry, ulUserMode, ulUserCE, ulUserBlock, ulUserPage;
	U32 ulSramAddr = (U32)&BUFB_BASE[0];
	U32 ulPhysicalBlock;
	UWORD uwFJobInfo;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	UBYTE ubDepth, ubFQIndex, ubL4kLink;
	UBYTE ubCEIndex, ubi;
	UBYTE ubDoReadRawData = 0;

	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;

	ubFQIndex = gFQI.ubFQLinkFree;
	FlashQueue_t *FQ;

	ulUserMode =  BUFL_BASE[2]; // 0 = FW: Logical 、 1 = Burner: Physical
	if (ENABLE_DEBUG_RAWREAD_TSB) {
		UartString("\nMode: ");
		Uart_Tx_DataHex(ulUserMode);
	}

	// UserMode 不對直接 return
#if BURNER
	U32 ulA2cmd;
	if (ulUserMode == 1) {
		ulUserBlock = BUFL_BASE[0];
		ulUserPage = BUFL_BASE[1];
		ulUserCE = BUFL_BASE[3];  //Physical CE
		ulA2cmd = BUFL_BASE[4];  // 1=A2cmd
		ulUserUnit = 0;
		ulUserEntry = 0;
	}
	else {
		return;
	}
#else
	if (ulUserMode == 0) {
		ulUserUnit = BUFL_BASE[0];
		ulUserEntry = BUFL_BASE[1];
		ulUserBlock = 0;
		ulUserPage = 0;
		ulUserCE = 0;  //Physical CE
	}
	else {
		return;
	}
#endif

	mDMAC_SV(1,  (U32)BUFB_BASE, (U32)(16 * 1024), 0, 0, 0, DMA_EN_SETCQ, 0);  // Whole BUFFER3 : 16K

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, (16 * 1024));  //16K
	}

#if BURNER
	// 不透過 RUT
	ubCEIndex = 0x3F; // > gubCEsPerPage
	if (ulUserCE < MAX_CE) {
		U32 ulPhysicalCE = gubReMappingCE[ulUserCE];  // 避免不連續 Physical CE
		if (ulPhysicalCE != 0x3F) {
			ubCEIndex = (ulPhysicalCE >> 3) + ((ulPhysicalCE % 8) * gubPlanesPerBurst);  //在正確範圍內才給值
		}
	}

	if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
		ulUserUnit = ((ulUserBlock / gubBurstsPerBank) / guwDieBlockShift) * (gulTotalPhyUnitNum / gubDieNumber) + ((ulUserBlock / gubBurstsPerBank) % guwDieBlockShift);
	}
	else {
		ulUserUnit = (ulUserBlock / gubBurstsPerBank) ; //還沒處理multi group
	}

	ulUserBlock = (ulUserUnit * 2) + (ulUserBlock % gubBurstsPerBank);
	ulPhysicalBlock = ulUserBlock;
	//ulUserUnit = ulUserBlock / gubBurstsPerBank; // Phy Unit
	ulUserEntry = (ulUserPage * guw4kEntrysPerPage) + (ubCEIndex * (gub4kEntrysPerPlane * gubBurstsPerBank)) + ((ulUserBlock % gubBurstsPerBank) * gub4kEntrysPerPlane);

	if (ENABLE_DEBUG_RAWREAD_TSB) {
		UartString("\nCE/Blk/Page: ");
		UartLongHex(ubCEIndex);
		UartString(" / ");
		UartLongHex(ulUserBlock);
		UartString(" / ");
		UartLongHex(ulUserPage);
		UartString(" > ");
		UartLongHex(ulUserUnit);
		UartString(" / ");
		UartLongHex(ulUserEntry);
	}

#else
	U32 AlignmentEntry;
	UWORD uwFBlock;
	UBYTE ubBurstBank;

	if (ENABLE_PLANE_CH_CE) {
		ubCEIndex = M_Modular(((ulUserEntry >> gub4kEntrysPerPlaneLog) >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		ubBurstBank = (ulUserEntry  >> gub4kEntrysPerPlaneLog) & gubBurstsPerBankMask;
	}
	else {
		ubCEIndex = (((ulUserEntry) >> gub4kEntrysPerPlaneLog) & gubPlanesPerBurstMask) + ((M_Modular((((ulUserEntry) >> gub4kEntrysPerPlaneLog) >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
		ubBurstBank = (ulUserEntry  >> gub4kEntrysPerBurstLog) & gubBurstsPerBankMask;
	}

	uwFBlock = M_RUT2DTo1D(ubCEIndex, (ulUserUnit << gubBurstsPerBankLog) + ubBurstBank);
	if (uwFBlock != 0xFEFE) {
		if (gubRUTMixPlaneEnable) {
			ulPhysicalBlock = uwFBlock;
		}
		else {
			ulPhysicalBlock = (uwFBlock << gubBurstsPerBankLog) + ubBurstBank;
		}
	}
	else {
		ulPhysicalBlock = 0xFFFF;  // > gulPhyBlocksPerCE
	}

	if (ENABLE_DEBUG_RAWREAD_TSB) {
		UartString("\nUnit/Entry: ");
		UartLongHex(ulUserUnit);
		UartString(" / = ");
		UartLongHex(ulUserEntry);
		UartString(" > ");
		UartLongHex(ubCEIndex);
		UartString(" / ");
		UartLongHex(ulPhysicalBlock);
	}
#endif

	if ((ulUserUnit < gulTotalPhyUnitNum) && (ulUserEntry < gul4kEntrysPerUnit) && (ulUserBlock < gulPhyBlocksPerCE) && (ulUserPage < guwPagesPerUnit) && (ulUserCE < MAX_CE) \
	        && (ubCEIndex < gubCEsPerPage) && (ulPhysicalBlock < gulPhyBlocksPerCE) ) {

		ubDoReadRawData = 1;  //先預設=1 讀 Raw Data, 下面判斷是 FW Table後再設為 0

#if (!BURNER)

		//先透過VT過濾FW Table Unit
		UBYTE ubIsFWTable = 0;
		if (ulUserUnit == VT->guwVTChildTarget.B.uwTarget) {
			if (ENABLE_DEBUG_RAWREAD_TSB) {
				UartString(" [VT_CHILD] ");
			}
			memcpy(&BUFB_BASE[0], "FW TABLE : ", 11);
			BUFB_BASE[11] = (MARK_VTChild & 0x00FF0000) >> 16;  //代號為Spare Mark的 4~5 Byte
			ubIsFWTable = 1;  // 不用再 Read Flash 判斷 Spare
			ubDoReadRawData = 0;  //不讀 Raw Data
		}
		else if ((ulUserUnit == VT->guwRUTTarget.B.uwTarget) || (ulUserUnit == VT->guwOldRUTTarget.B.uwTarget)) {  //RUT
			if (ENABLE_DEBUG_RAWREAD_TSB) {
				UartString(" [RUT] ");
			}
			memcpy(&BUFB_BASE[0], "FW TABLE : ", 11);
			BUFB_BASE[11] = (MARK_RUT & 0x00FF0000) >> 16;
			ubIsFWTable = 1;
			ubDoReadRawData = 0;
		}
		else if (ulUserUnit == VT->guwInitInfoTarget.B.uwTarget) {
			if (ENABLE_DEBUG_RAWREAD_TSB) {
				UartString(" [InitInfo] ");
			}
			memcpy(&BUFB_BASE[0], "FW TABLE : ", 11);
			BUFB_BASE[11] = (MARK_TableHead & 0x00FF0000) >> 16;
			ubIsFWTable = 1;
			ubDoReadRawData = 0;
		}

		if (ubIsFWTable == 0) {
			for (ubi = 0; ubi < VTAREA_UNITS; ubi ++) {
				if (ulUserUnit == VT->guwVTAreaFreeTarget[ubi].B.uwTarget) {
					if (ENABLE_DEBUG_RAWREAD_TSB) {
						UartString(" [VT] ");
					}
					memcpy(&BUFB_BASE[0], "FW TABLE : ", 11);
					BUFB_BASE[11] = (MARK_VT & 0x00FF0000) >> 16;
					ubIsFWTable = 1;
					ubDoReadRawData = 0;
					break;
				}
			}
			for (ubi = 0; ubi < gubTableUnitNum; ubi ++) {
				if (ulUserUnit == VT->guwTableTarget[ubi].B.uwTarget) {
					if (ENABLE_DEBUG_RAWREAD_TSB) {
						UartString(" [T_HEAD] ");
					}
					memcpy(&BUFB_BASE[0], "FW TABLE : ", 11);
					BUFB_BASE[11] = (MARK_TableHead & 0x00FF0000) >> 16;
					ubIsFWTable = 1;
					ubDoReadRawData = 0;
					break;
				}
			}
		}

		// VT沒記的再進來判斷是否為 FW Table
		if ((ulUserUnit < guwFWTotalUnitNum) && (ulUserMode == 0) && (ubIsFWTable == 0)) {

			U32 Temp = FCONL[FCONL_CRC_EN];
			//避免讀到 RS Parity Fail
			FCONL[FCONL_CRC_EN] &= ~(U32)CRC_16_EN;
			FCONL[FCONL_CRC_EN] &= ~(U32)CRC_32_EN;

			AlignmentEntry = (ulUserEntry >> gub4kEntrysPerPlaneLog) << gub4kEntrysPerPlaneLog;

			//uwFJobInfo = (((gulVC[ulUserUnit].B.btFastPage || gulVC[ulUserUnit].B.btInD1Unit) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | ((gulVC[ulUserUnit].B.btFastPage && gulVC[ulUserUnit].B.ulValidCount == DefaultVC) ? BIT_FJOBI_RETRY : 0));
			uwFJobInfo = (((gulVC[ulUserUnit].B.btFastPage || gulVC[ulUserUnit].B.btInD1Unit) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY);
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ulRAMAddr = ulSramAddr;
			FQ->btCheckEmpty = 1;
			FQ->btThisIsErasePage = 0;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = uwFJobInfo;
			FQ->uwFUnit = (UWORD)ulUserUnit;
			FQ->ulFEntry = AlignmentEntry; //0;// 不管輸入哪個 Entry都固定讀 Entry 0
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btUNServed = 1;
			FQ->ubL4kNum = ubL4KNum;
#if FixShallowErase
			FQ->btD3 = (uwFJobInfo & BIT_FJOBI_D1_UNIT) ? D1 : D3;
#else
			FQ->btD3 = 0;
#endif
			FQ->FQPara.B.btConvBypass = 0;
			gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);  // 0x1200 + (ubL4kLink*16*16)
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate((void *)ulSramAddr, ubL4KNum * 4096);  //16K
			}

			UBYTE ubL4KCount = 0;
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = 0x00000000;
				pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
				pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((ulSramAddr + (ubL4KCount * 4096) ) / 512);
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0; //0xFF; // 0: Data不寫到RAM上
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;


			while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}

			gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;


			while (gFQI.ubFQLinkNumber != 0) {
				M_SwitchTask();
			}

			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));

			// 判斷是否為Table Spare Mark (只拿第一個 4k 的LCA)
			U32 ulLCAHigh4Byte = pL4KTablePtr->ulL4K_LCA & 0xFFFF0000;  //部分Spare只比前 4 Byte, 忽略 Offset
			if ( (pL4KTablePtr->ulL4K_LCA == MARK_BeforeRead) ||
			        (pL4KTablePtr->ulL4K_LCA == MARK_DPST) ||
			        (pL4KTablePtr->ulL4K_LCA == MARK_TableDummy) ||
			        (ulLCAHigh4Byte == MARK_SYS) ||
			        (ulLCAHigh4Byte == MARK_ErrorDumpUnit) ||
			        (ulLCAHigh4Byte == MARK_VT) ||
			        (ulLCAHigh4Byte == MARK_RUT) ||
			        (ulLCAHigh4Byte == MARK_VTChild) ||
			        (ulLCAHigh4Byte == MARK_TableHead) ||
			        (ulLCAHigh4Byte == MARK_TableEC) ||
			        (ulLCAHigh4Byte == MARK_TableVC) ||
			        (ulLCAHigh4Byte == MARK_TableHost) ||
			        (ulLCAHigh4Byte == MARK_TableP2L) ||
			        (ulLCAHigh4Byte == MARK_TableSeq) ||
			        (ulLCAHigh4Byte == MARK_DataP2L) ) {
				if (ENABLE_DEBUG_RAWREAD_TSB) {
					UartString(" [FW Table]: ");
					UartLongHex(pL4KTablePtr->ulL4K_LCA);
				}
				memcpy(&BUFB_BASE[0], "FW TABLE : ", 11);
				BUFB_BASE[11] = (pL4KTablePtr->ulL4K_LCA & 0x00FF0000) >> 16;  //代號為Spare Mark的 4~5 Byte
				ubDoReadRawData = 0;  //不讀 Raw Data

			}

			//讀完回復原本CRC設定
			FCONL[FCONL_CRC_EN] = Temp;

		}
#endif // !BURNER

	}
	else {  // Out of Bounds
		memcpy(&BUFB_BASE[0], "ERROR INPUT!", 13);
		ubDoReadRawData = 0;
		if (ENABLE_DEBUG_RAWREAD_TSB) {
			UartString("\tOut of Boundary!!!");
		}
	}

	// ***** Read Raw Data ***** //

	if (ubDoReadRawData) {  // 非 FW Table 才讀 Raw Data, Burner Mode不限制
		//參考 ftlGoodBlock
		U32 Temp = FALLL[FCTLL_IBF_CTL];

		FALLL[FCTLL_IBF_CTL] |= FORCE_ZIP_COR;   /* Force data to RAM even UNECC. */
		FALLL[FCTLL_CNT_ONE] |= COUNT_MODE_EN;
		FALLL[FCTLL_CNT_ONE] &= (~COUNT_ONE_MODE);	// count 0;
		//FALLL[FALLL_CHNL_SET] &= (~BYPASS_INV);     // 1b: bypass inversion

#if BURNER
		uwFJobInfo = ((ulA2cmd ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY ); //Physical
		//uwFJobInfo = ( BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY | BIT_FJOBI_FASTPAGE );  //Physical

#else
		U32 ulECCSet_Temp = 0;

		ulECCSet_Temp |= ((FALLL[FCTLL_LDPC_CFG] | LDPC_COR_EN) | (FALLL[FCTLL_OTHER_SET] | BCH_COR_EN) | (FCONL[FCONL_CRC_EN] | CRC_32_EN));
		FALLL[FCTLL_OTHER_SET] &= (~BCH_COR_EN);
		FALLL[FCTLL_LDPC_CFG] &= (~LDPC_COR_EN);
		FCONL[FCONL_CRC_EN] &= (~CRC_32_EN);

		FALLL[FALLL_CHNL_SET] |= (BYPASS_CONV);

		uwFJobInfo = (((gulVC[ulUserUnit].B.btFastPage || gulVC[ulUserUnit].B.btInD1Unit) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT );  // Logical

		//只有FW mode需要對齊4k, BURNER在上面從CE,Blk,Page換算Entry時已經對齊了
		AlignmentEntry = (ulUserEntry >> gub4kEntrysPerPlaneLog) << gub4kEntrysPerPlaneLog; // 4k對齊，因為一次讀一個 Page...不然FQ會卡

#endif

		ubFQIndex = gFQI.ubFQLinkFree;
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ulRAMAddr = ulSramAddr;
		FQ->btCheckEmpty = 1;
		FQ->btThisIsErasePage = 0;
		FQ->ubFJob = BYTE_FJOB_READ;
		FQ->uwFJobInfo = uwFJobInfo;
		FQ->uwFUnit = (UWORD)ulUserUnit;
#if BURNER
		FQ->ulFEntry = ulUserEntry;
#else
		FQ->ulFEntry = AlignmentEntry;  // Physical or Logical 由 FJob BIT_FJOBI_DIRECTACESS 控制 底層會自己轉
#endif
		FQ->ubDepth = ubDepth;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btUNServed = 1;
		FQ->ubL4kNum = ubL4KNum;
#if FixShallowErase
		FQ->btD3 = (uwFJobInfo & BIT_FJOBI_D1_UNIT) ? D1 : D3;
#else
		FQ->btD3 = 0;
#endif
		FQ->FQPara.B.btConvBypass = 1;  // Raw Data

		gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
		gulPara0x24.BitMap.btBufModeEn = 0;
		gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);  // 0x1200 + (ubL4kLink*16*16)
		gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
		gulPara0x24.BitMap.btZipBypass = 1;
		gulPara0x24.BitMap.btCmpEn = 0;
		gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
		gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate((void *)ulSramAddr, ubL4KNum * 4096);  //16K
		}

		UBYTE ubL4KCount = 0;
		for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA = 0x00000000;
			pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
			pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0;
			pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((ulSramAddr + (ubL4KCount * 4096) ) / 512);
			pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
			*pL4KTablePtr = pL4KTable;
		}
		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

		while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}

#if BURNER

		if (FQ->btReadUNC) {
			if (ENABLE_DEBUG_RAWREAD_TSB) {
				UartString(" [UNC] ");
			}
			//把Buffer清空
			mDMAC_SV(1,  (U32)BUFB_BASE, (U32)(16 * 1024), 0, 0, 0, DMA_EN_SETCQ, 0);  // Whole BUFFER3 : 16K

			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate(BUFB_BASE, (16 * 1024));  //16K
			}

			memcpy(&BUFB_BASE[0], "READ UNC!", 9);
		}
#endif

		gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

		while (gFQI.ubFQLinkNumber != 0) {
			M_SwitchTask();
		}

		//印每 4k Data的前 16 Byte
		if (ENABLE_DEBUG_RAWREAD_TSB) {
			UartString(" > ");
			UBYTE ubj;
			for (ubj = 0; ubj < ubL4KNum; ubj++) {
				for (ubi = 0; ubi < 16; ubi++) {
					if (ubi < 8) {
						Uart_Tx_DataHex(BUFB_BASE[(ubj * 4096) + ubi]);
					}
				}
				UartString(" ");
			}
		}



#if BURNER
		FALLL[FCTLL_IBF_CTL] = Temp;
#else
		FALLL[FCTLL_OTHER_SET] |= (ulECCSet_Temp & BCH_COR_EN);
		FALLL[FCTLL_LDPC_CFG] |= (ulECCSet_Temp & LDPC_COR_EN);
		FCONL[FCONL_CRC_EN] |= (ulECCSet_Temp & CRC_32_EN);
		FALLL[FALLL_CHNL_SET] &= (~BYPASS_CONV);
		FALLL[FCTLL_IBF_CTL] = Temp;
#endif


	}


	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, (16 * 1024) ); //16K = 32 Sectors
	}

	HL[HL_BUF_OFST] = 0;
	HW[HW_SECTOR_CNT] = 32;
	HL[HL_BUF_TRG_SEC] = 32;
	HL[HL_TQ_CONTENT] = 0;
	Vender_HostRemainSectorCount();

}
#endif


#if BURNER
void Vender_Direct_RawRead(void)
{
	//UWORD* uwVenderFPUEntry = (UWORD*)(0x50000000 + BURNER_FIX_FPU_LENGTH);
	UBYTE *ubDataBuffer =  (UBYTE*)(BUFFER3_BASE); //21k
	UBYTE *ubRawBuffer = (UBYTE*)((BUFFER3_BASE) + 0x5400); //25K //(Z2_DDR_BASE);
	U32 ulPhysicalCE, ulPhysicalBlock, ulPhysicalPage, ulPhysicalPageSize;
	UBYTE ubChannel, ubNotA2cmd, ubD3, ubMode;
	U32 ulTOP_TGL_TIME_CFG_BACKUP, ulTOP_TGL_TIME_CFG_1_BACKUP;
	U32 ulTemp, ulTemp_FCONL_FCE_ENB, ulTemp_CHNL_SET, ulTemp_OTHER_SET, ulTemp_LDPC_CFG, ulTemp_CRC_EN, ulTemp_CMP_CFG, ulTemp_RS_CFG;
	U32 ulTemp_ZIP_CFG, ulTemp_DMA_CFG, ulTemp_INT_INF, ulTemp_INT_CFG;
	UBYTE ubFPUOffset = 0;
	UBYTE ubL4KIndex;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	UBYTE ubDepth;

	ubNotA2cmd = (HB[HB_LBA_M] & BIT7) ? 1 : 0;
	ubD3 = (HB[HB_LBA_M] & BIT6) ? 1 : 0;
	ubMode = HB[HB_LBA_L];//0:Conversion, 1:Inversion, 2:ECC, 3:Spare,

	ulPhysicalCE = BUFL_BASE[0];//0~3
	ulPhysicalBlock = BUFL_BASE[1];//4~7
	ulPhysicalPage =  BUFL_BASE[2];//8~11
	ulPhysicalPageSize = gulNandPhysicalPageSize;

	if (ENABLE_DEBUG_SOFTWAREECC) {
		UartString("\n");
		UartLongHex(ulPhysicalCE);
		UartString(" ");
		UartLongHex(ulPhysicalBlock);
		UartString(" ");
		UartLongHex(ulPhysicalPage);
		UartString(" ");
		UartLongHex(ulPhysicalPageSize);
		UartString(" ");
		Uart_Tx_DataHex(ubNotA2cmd);
		UartString(" ");
		UartWordHex(guwDMAFrameSize);
	}

	ubChannel = ulPhysicalCE / 8; //Max Physical CE Number per channel

	mDMAC_SV(1,  (U32)BUFB_BASE, (U32)(46 * 1024), 0, 0, 0, DMA_EN_SETCQ, 0);

	gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
	//remain the original setting of Channel
	ulTemp_FCONL_FCE_ENB = FCONL[FCONL_FCE_ENB];
	ulTemp_CHNL_SET = gFREG[FCTLL_CHNL_SET].L;
	ulTemp_OTHER_SET = gFREG[FCTLL_OTHER_SET].L;
	ulTemp_LDPC_CFG = gFREG[FCTLL_LDPC_CFG].L;
	ulTemp_CRC_EN = FCONL[FCONL_CRC_EN];
	ulTemp_CMP_CFG = gFREG[FCTLL_CMP_CFG].L;
	ulTemp_RS_CFG = gFREG[FCTLL_RS_CFG].L;
	ulTemp_ZIP_CFG = gFREG[FCTLL_ZIP_CFG].L;
	ulTemp_DMA_CFG = gFREG[FCTLL_DMA_CFG].L;
	ulTemp_INT_INF = gFREG[FCTLL_INT_INF].L;
	ulTemp_INT_CFG = gFREG[FCTLL_INT_CFG].L;
	ulTOP_TGL_TIME_CFG_BACKUP = FCONL[FCONL_TGL_TIME_CFG];
	ulTOP_TGL_TIME_CFG_1_BACKUP = FCONL[FCONL_TGL_TIME_CFG_1];

	M_DisableAllFlashCE();
	FCONL[FCONL_FCE_ENB] |= BIT0 << ulPhysicalCE; // CE enable

	//Set Mode
	if (ubMode & BIT0) {
		gFREG[FCTLL_CHNL_SET].L &= ~(U32)BYPASS_CONV;
	}
	else {
		gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
	}
	if (ubMode & BIT1) {
		gFREG[FCTLL_CHNL_SET].L &= ~(U32)BYPASS_INV;
		gFREG[FCTLL_CHNL_SET].L |= BYPASS_INV;
	}
	else {
		gFREG[FCTLL_CHNL_SET].L |= BYPASS_INV;
	}
	if (ubMode & BIT2) {
		gFREG[FCTLL_OTHER_SET].L |= BCH_COR_EN;
		gFREG[FCTLL_LDPC_CFG].L |= LDPC_COR_EN;
		FCONL[FCONL_CRC_EN] |= CRC_32_EN;
	}
	else {
		gFREG[FCTLL_OTHER_SET].L &= ~(U32)BCH_COR_EN;
		gFREG[FCTLL_LDPC_CFG].L &= ~(U32)LDPC_COR_EN;
		FCONL[FCONL_CRC_EN] &= ~(U32)CRC_32_EN;
	}
	if (ubMode & BIT3) {
		gFREG[FCTLL_CMP_CFG].L |= LCA_FW_COMPARE_EN;
	}
	else {
		gFREG[FCTLL_CMP_CFG].L &= ~(U32)LCA_FW_COMPARE_EN;
	}


	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
	}

	gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // turn off RS
	gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
	gFREG[FCTLL_ZIP_CFG].L &= (~L4K_BADR_CONTINUOUS_MODE);//!!!!!!!!
	gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);//!!!!!!!!

	M_GetMTDepth(ubDepth);
	gFREG[FCTLL_INT_VCT].L &= IntVector(0x0000FF00);
	gFREG[FCTLL_INT_VCT].L |= IntVector(ubDepth);
	gFREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling
	gubINTTable[ubDepth].All = 0;

	//gFREG[FCTLL_FSA_SEL].L |= IFSA_EN; //FSA Enable
	//Fill in FTA, for number of page is smaller than 256
	if (MicronFlashOnly || YMTC) {
		if ((B0KB) || (YMTC_JGS)) { //TLC
			gFREG[FCTLL_FTA_ROW].B.B0 = (UBYTE)(ulPhysicalPage & 0x000000FF);
			gFREG[FCTLL_FTA_ROW].B.B1 = (UBYTE)(((ulPhysicalBlock & 0x0000001F) << 3) | ((ulPhysicalPage & 0x00000700) >> 8));
			gFREG[FCTLL_FTA_ROW].B.B2 = (UBYTE)((ulPhysicalBlock) >> 5);
			gFREG[FCTLL_FTA_COL].B.B0 = 0;
			gFREG[FCTLL_FTA_COL].B.B1 = 0;
		}
		else { //MLC L06B
			gFREG[FCTLL_FTA_ROW].B.B0 = (UBYTE)ulPhysicalPage;
			gFREG[FCTLL_FTA_ROW].B.B1 = (UBYTE)(((ulPhysicalBlock & 0x0000003F) << 2) | ((ulPhysicalPage & 0x00000300) >> 8));
			gFREG[FCTLL_FTA_ROW].B.B2 = (UBYTE)((ulPhysicalBlock) >> 6);
			gFREG[FCTLL_FTA_COL].B.B0 = 0;
			gFREG[FCTLL_FTA_COL].B.B1 = 0;
		}
	}
	else {//toshiba MLC and TLC
		if (guoFlashUseType.B.btToshibaBiCs4) {
			gFREG[FCTLL_FTA_ROW].B.B0 = (UBYTE)(ulPhysicalPage & 0x000000FF);
			gFREG[FCTLL_FTA_ROW].B.B1 = (UBYTE)(((ulPhysicalBlock & 0x0000007F) << 1) | ((ulPhysicalPage & 0x00000100) >> 8));
			gFREG[FCTLL_FTA_ROW].B.B2 = (UBYTE)((ulPhysicalBlock) >> 7);
		}
		else {
			gFREG[FCTLL_FTA_ROW].B.B0 = (UBYTE)ulPhysicalPage;
			gFREG[FCTLL_FTA_ROW].B.B1 = (UBYTE)ulPhysicalBlock;
			gFREG[FCTLL_FTA_ROW].B.B2 = (UBYTE)((ulPhysicalBlock & 0xFFFFFF00) >> 8);
		}
		gFREG[FCTLL_FTA_COL].B.B0 = 0;
		gFREG[FCTLL_FTA_COL].B.B1 = 0;
	}
	//Fill DMA setting
	gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME);//!!!!!!!!

	if (gubFlashClock <= FLH_CLK_100MHz) { // Fix read row data bug under 100MHz
		FCONL[FCONL_TGL_TIME_CFG] &= 0x00FFFFFF;
		FCONL[FCONL_TGL_TIME_CFG] |= (TOG_DAT_PSTH_PHASE_CNT(3) | TOG_DAT_CLH_PHASE_CNT(5));

		FCONL[FCONL_TGL_TIME_CFG_1] &= 0x00FFFFFF;
		FCONL[FCONL_TGL_TIME_CFG_1] |= (TOG_DAT_PSTH_PHASE_CNT(3) | TOG_DAT_CLH_PHASE_CNT(5));
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)ubRawBuffer, ubL4KNum * IBF_BANK_LENGTH);
	}

	gFREG[FCTLL_DMA_CFG].L = TRANSFER_ECC_FRAME(ubL4KNum); //set FCTL_DMA_CFG[10:8] to frame number
	gFREG[FCTLL_RAW_DMA_ADR].L = (U32)ubRawBuffer;

#if  MicronFlashOnly || YMTC
	//if (!ubNotA2cmd) { //switch to Normal Mode

	///gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CDF;
	//gFREG[FCTLL_FPU_TRIG].L |= FPU_TRIG;
	//while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
	gFREG[FCTLL_PIO_CMD].L = 0xDF;
	//}
#endif
	if (!ubNotA2cmd) { //switch to SLC Mode

		gFREG[FCTLL_FPU_ENTRY].L = RAM_SEQ_PTR16(FPU_CA2_C00_A5_C30_C70_POL_MK20_C05_A5_E0_DMARaw);
	}
	else {
		gFREG[FCTLL_FPU_ENTRY].L = RAM_SEQ_PTR16(FPU_C00_A5_C30_C70_POL_MK20_C05_A5_E0_DMARaw);
	}

	gFREG[FCTLL_FPU_TRIG].L |= FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	/*while (gubINTTable[ubDepth].All == 0); // use this to check
	if (gubINTTable[ubDepth].All & 0xF0) {
		flaLEDBlink(gubINTTable[ubDepth].All);
	}*/

	if (gubFlashClock <= FLH_CLK_100MHz) { // Fix read row data bug under 100MHz
		FCONL[FCONL_TGL_TIME_CFG] = ulTOP_TGL_TIME_CFG_BACKUP;
		FCONL[FCONL_TGL_TIME_CFG_1] = ulTOP_TGL_TIME_CFG_1_BACKUP;
	}

	for (ubL4KIndex = 0; ubL4KIndex < ubL4KNum; ubL4KIndex++) {
		if (guwDMAFrameSize & 31) { //not align 32 bytes
			memcpy((void *)(&ubDataBuffer[ubL4KIndex * guwDMAFrameSize]), (void *)(&ubRawBuffer[ubL4KIndex * IBF_BANK_LENGTH]), guwDMAFrameSize);
		}
		else {
			mDMAC_COPY((U32)(&ubRawBuffer[ubL4KIndex * IBF_BANK_LENGTH]), (U32)(&ubDataBuffer[ubL4KIndex * guwDMAFrameSize]), guwDMAFrameSize, 0, 0, 0, 0, 1);
		}
	}

	//if (MicronFlashOnly || Hynix) {
	if (ulPhysicalPageSize > (ubL4KIndex * guwDMAFrameSize)) {
		// phsical page size is larger than total DMA frame size, read out the remain part by PIO
		U32 ulRemainByte = ulPhysicalPageSize - (ubL4KIndex * guwDMAFrameSize);
		U32 ulTransferCnt = ulRemainByte;
		U32 ulTansferIndex;
		UBYTE* ubNandTempBuffer = (UBYTE*)(ubRawBuffer);
		UWORD* uwNandTempBuffer = (UWORD*)(ubRawBuffer);

		if (guoFlashUseType.B.btToggle || guoFlashUseType.B.btONFI) {
			ulTransferCnt >>= 1;
		}

		// change column address
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x05;
		gFREG[FCTLL_PIO_ADR].B.B0 = (ubL4KIndex * guwDMAFrameSize) & 0xFF;//byte offset in page
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
			memcpy((void *)(&ubDataBuffer[ubL4KIndex * guwDMAFrameSize]), (void*)ubRawBuffer, ulRemainByte);
		}
		else {
			mDMAC_COPY((U32)ubRawBuffer, (U32)(&ubDataBuffer[ubL4KIndex * guwDMAFrameSize]), ulRemainByte, 0, 0, 0, 0, 1);
		}

#if  MicronFlashOnly
		//if (!ubNotA2cmd) { //switch to Normal Mode

		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CDF;
		gFREG[FCTLL_FPU_TRIG].L |= FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
		//}
#endif

	}
	//}

	gubNeedSendCmdComplete = 0;

	ulTemp = HL[HL_BUF_R_EP];
	HL[HL_BUF_R_EP] = 5;//output 5*4096KB

	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

	HL[HL_BUF_R_EP] = ulTemp;
	gFREG[FCTLL_CHNL_SET].L = ulTemp_CHNL_SET;
	gFREG[FCTLL_OTHER_SET].L = ulTemp_OTHER_SET;
	gFREG[FCTLL_LDPC_CFG].L = ulTemp_LDPC_CFG;
	FCONL[FCONL_CRC_EN] = ulTemp_CRC_EN;
	gFREG[FCTLL_CMP_CFG].L = ulTemp_CMP_CFG;
	gFREG[FCTLL_RS_CFG].L = ulTemp_RS_CFG;
	gFREG[FCTLL_ZIP_CFG].L = ulTemp_ZIP_CFG;
	gFREG[FCTLL_DMA_CFG].L = ulTemp_DMA_CFG;
	gFREG[FCTLL_INT_INF].L = ulTemp_INT_INF;
	gFREG[FCTLL_INT_CFG].L = ulTemp_INT_CFG;
	FCONL[FCONL_FCE_ENB] = ulTemp_FCONL_FCE_ENB;

	M_AddMTDepth(ubDepth);
	gubStopFTL = 0;
}
#endif

void Vender_Output_Trimming_Table(void)
{
	if ((!ENABLE_BURNER_FORCEGENFAIL) && ENABLE_OUTPUT_TRIMMING_TABLE) {
		M_CheckRDTModeToDisableFunction();

		UBYTE ubDieIndex;
		UBYTE ubCEIndex;
		UBYTE ubChannelIndex;
		UBYTE ubBufferSectorCnt = gubCENumber << gubDieNumberLog;
		UBYTE ubToggleModeTemp = gubToggleMode;
		UBYTE ubUART = 0;
		UWORD uwByteIndex;
		UWORD uwBufferOffset;
		U32 ulFL_FCTL_HS_MODE_All_Temp =  FALLL[FCTLL_HS_MODE];
		U32 ulTemp[2];

		if (ubUART) {
			UartString("\n\n\nSecS:");
			UartLongHex(HL[HL_BUF_TRG_SEC]);
			UartString(" Secg:");
			UartLongHex(gulSectorCnt);
			UartString(" SecB:");
			Uart_Tx_DataHex(ubBufferSectorCnt);
			UartString("\nCH:");
			Uart_Tx_DataHex(gubPlanesPerBurst);
			UartString(" CE:");
			Uart_Tx_DataHex(gubCENumber);
			UartString(" Di:");
			Uart_Tx_DataHex(gubDieNumber);
		}

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(BUFB_BASE, ubBufferSectorCnt * 512);
		}
		mDMAC_SV(1,  (U32)BUFB_BASE, ubBufferSectorCnt * 512, 0, 0, 0, DMA_EN_SETCQ, 1);

		gubToggleMode = 0;
		for (ubChannelIndex = 0; ubChannelIndex < gubPlanesPerBurst; ubChannelIndex++) {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannelIndex & gubPlanesPerBurstMask) * 0x200));
			ulTemp[ubChannelIndex] = gFREG[FCTLL_FLH_SET].L;
			gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
			if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
				if (ASIC) {
					if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
						gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xE));
						idle(1000);
					}
				}
			}
		}

		for (ubCEIndex = 0; ubCEIndex < gubCENumber; ubCEIndex++) {
			ubChannelIndex = ubCEIndex % gubPlanesPerBurst;       //logical
			FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
			FCONL[FCONL_FCE_SET] = (ubChannelIndex * 8) + ((ubCEIndex % gubMTQTotalPhyCE) / gubPlanesPerBurst); //logical

			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannelIndex & gubPlanesPerBurstMask) * 0x200));

			for (ubDieIndex = 0; ubDieIndex < gubDieNumber; ubDieIndex++) {
				if (ubUART) {
					UartString("\nCH:");
					Uart_Tx_DataHex(ubChannelIndex);
					UartString(" CE:");
					Uart_Tx_DataHex(ubCEIndex);
					UartString(" Di:");
					Uart_Tx_DataHex(ubDieIndex);
					UartString("\n");
				}

				//Parameter Set
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x5C;
				gFREG[FCTLL_PIO_CMD].B.B0 = 0xC5;
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
				gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
				gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
				// Delay
				idle(100);

				for (uwByteIndex = 0; uwByteIndex < 0x100; uwByteIndex++) {  // Set MP's specific pattern
					switch (uwByteIndex) {
						case 0:
							BUFB_BASE[((ubCEIndex << gubDieNumberLog) + ubDieIndex) * 512 + uwByteIndex] = ubCEIndex >> gubPlanesPerBurstLog;
							break;
						case 1:
							BUFB_BASE[((ubCEIndex << gubDieNumberLog) + ubDieIndex) * 512 + uwByteIndex] = ubChannelIndex;
							break;
						case 2:
							BUFB_BASE[((ubCEIndex << gubDieNumberLog) + ubDieIndex) * 512 + uwByteIndex] = ubDieIndex;
							break;
						default:
							BUFB_BASE[((ubCEIndex << gubDieNumberLog) + ubDieIndex) * 512 + uwByteIndex] = 0xFF;
							break;
					}
				}

				for (uwByteIndex = 0; uwByteIndex < 0x100; uwByteIndex++) {
					// do PIO here
					gFREG[FCTLL_PIO_CMD].B.B0 = ((ubCEIndex / gubMTQTotalPhyCE) * gubDieNumber + ubDieIndex + 0xF1);
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x01;
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x06;
					// Delay
					idle(100);

					gFREG[FCTLL_PIO_CMD].B.B0 = 0x5F;
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;
					gFREG[FCTLL_PIO_ADR].B.B0 = (UBYTE)uwByteIndex;
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x5F;
					// Delay
					idle(100);

					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST); // set to default value
					uwBufferOffset = ((ubCEIndex << gubDieNumberLog) + ubDieIndex) * 512 + 256 + uwByteIndex;
					BUFB_BASE[uwBufferOffset] = gFREG[FCTLL_PIO_DAT].B.B0; // setDataByteHere
					if (ubUART) Uart_Tx_DataHex(BUFB_BASE[uwBufferOffset]);

#if (!TLC_BICS2)
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x5F;
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x5F;
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x5F;
#endif
					// Delay
					idle(100);
				}


				if (guoFlashUseType.B.btToshibaBiCs4) {


					for (uwByteIndex = 0; uwByteIndex < 0x2c; uwByteIndex++) {
						// do PIO here
						gFREG[FCTLL_PIO_CMD].B.B0 = ((ubCEIndex / gubMTQTotalPhyCE) * gubDieNumber + ubDieIndex + 0xF1);
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x01;
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x06;
						// Delay
						idle(100);

						gFREG[FCTLL_PIO_CMD].B.B0 = 0x56;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0xFF;
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
						// Delay
						idle(100);

						gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;
						gFREG[FCTLL_PIO_ADR].B.B0 = (UBYTE)uwByteIndex;
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x5F;
						// Delay
						idle(100);

						gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST); // set to default value
						uwBufferOffset = ((ubCEIndex << gubDieNumberLog) + ubDieIndex) * 512 + 128 + uwByteIndex;
						BUFB_BASE[uwBufferOffset] = gFREG[FCTLL_PIO_DAT].B.B0; // setDataByteHere
						if (ubUART) Uart_Tx_DataHex(BUFB_BASE[uwBufferOffset]);

						gFREG[FCTLL_PIO_CMD].B.B0 = 0x56;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0xFF;
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
						// Delay
						idle(100);
					}


				}


				gFREG[FCTLL_PIO_CMD].B.B0 = 0xFF;
				// Delay
				idle(100);
			}


		}

		FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
		FALLL[FCTLL_HS_MODE] = ulFL_FCTL_HS_MODE_All_Temp;

		gubToggleMode = ubToggleModeTemp;
		for (ubChannelIndex = 0; ubChannelIndex < gubPlanesPerBurst; ubChannelIndex++) {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannelIndex & gubPlanesPerBurstMask) * 0x200));
			gFREG[FCTLL_FLH_SET].L = ulTemp[ubChannelIndex];
			if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
				if (ASIC) {
					if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
						gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | FLH_CLK_DIV_VALUE(0xF));
						idle(1000);
						gFREG[FCTLL_FDIV_CFG].L = 0;
					}
				}
			}
		}


		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, ubBufferSectorCnt * 512);
		}

		//HL[HL_BUF_R_EP] = (ubBufferSectorCnt & 7) ? ubBufferSectorCnt >> 3 : ubBufferSectorCnt >> 3 - 1;
		HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG] & H_DATA_TRG); // Wait until Trig data to Host Finish.
	}
}

void Vender_Direct_Read_Flash(void)
{
	if (!(ENABLE_BURNER_FORCEGENFAIL)) {
#if(!RDT)
		M_CheckRDTModeToDisableFunction();
#if 1
		UBYTE ubPlaneBank, ubMode = 0, ubA2cmd = 0;

		UBYTE ubD3;//MLC no use
		UBYTE ubIsFastpage = 0;
		UBYTE ubCEIndex, ubL4KNum = (gubSectorsPerPlane >> SectorsPer4KLog);
		UBYTE ubUseRUT = 0;
		UBYTE ubPhyOrFWMode; // 1: physical addr mode, 0: FW mode
		UBYTE ubWholePage = 0;
		UBYTE ubPhysicalCE, ubLogicalCE;
		UBYTE ubDepth = 0xFF, ubL4kLink;
		UWORD uwPhysicalBlock, uwPhysicalPage, uwPhysicalSector;
		U32 ulVenderUnit;
		U32 ulVenderSector;
#if TLC_NES_TEST_SB_FLOW
		UBYTE ubBackupCRC32Enable = ((FCONL[FCONL_CRC_EN] & CRC_32_EN) ? 1 : 0);
#endif
		volatile REG_t * pFlaReg;

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(BUFB_BASE, 512);
		}

#if TLC_NES_TEST_SB_FLOW
#if MicronFlashOnly || YMTC_JGS
		if (gulParamPageInfo.ONFI.btNandSupplyByEmulator && (BUFB_BASE[30] == 0xFF))
#else
		if (gulParamPageInfo.JEDEC.btNandSupplyByEmulator && (BUFB_BASE[30] == 0xFF))
#endif
		{
			// if nes
			//UartString("\nNESVerify");
			HandleStopRW(1);

			gubNESReadIBF = 1;
			// close CRC
			FCONL[FCONL_CRC_EN] &= (~CRC_32_EN);
			guwNESDataLen = (4096 + 16); // plane size + spare size
			ubL4KNum = 1; // read 4K

			if (guoFlashUseType.B.btSandisk && (BUFB_BASE[31] == 0x00)) {
				gubSandiskRetryType = 2;
			}
			else if (guoFlashUseType.B.btSandisk && (BUFB_BASE[31] == 0x01)) {
				gubSandiskRetryType = 6;
			}
			else if (guoFlashUseType.B.btSandisk && (BUFB_BASE[31] == 0x02)) {
				gubSandiskRetryType = 10;
			}

		}
#endif

		ubA2cmd = (HB[HB_LBA_M] & BIT7) ? 1 : 0;
		ubD3 = (HB[HB_LBA_M] & BIT6) ? 1 : 0;
		ubWholePage = HB[HB_LBA_L];
		ubPhyOrFWMode = HB[HB_LBA_H];

		if (ubPhyOrFWMode) {
			ubPhysicalCE = BUFB_BASE[20];
			uwPhysicalBlock = BUFW_BASE[11];
			uwPhysicalPage =  BUFW_BASE[12];
			uwPhysicalSector = BUFW_BASE[13];

			ubLogicalCE = (ubPhysicalCE >> 3) + ((ubPhysicalCE % 8) << gubPlanesPerBurstLog); //to be change

			if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
				ulVenderUnit = ((uwPhysicalBlock >> gubBurstsPerBankLog) / guwDieBlockShift) * (gulTotalPhyUnitNum / gubDieNumber) + ((uwPhysicalBlock >> gubBurstsPerBankLog) % guwDieBlockShift);
			}
			else {
				ulVenderUnit = (uwPhysicalBlock >> gubBurstsPerBankLog) ; //還沒處理multi group
			}

			if (ENABLE_PLANE_CH_CE) {
				ulVenderSector = (U32)(uwPhysicalSector & gubSectorsPerPlaneMask) + (U32)(((M_Multiplier((U32)uwPhysicalPage, DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubLogicalCE << gubBurstsPerBankLog) + (uwPhysicalBlock & gubBurstsPerBankMask)) << gubSectorsPerPlaneLog );
			}
			else {
				ulVenderSector = (U32) (uwPhysicalSector & gubSectorsPerPlaneMask) + (U32)(((M_Multiplier((U32)uwPhysicalPage, DEF_PlanesPerPage_Mcl_Div)) + (U32)((ubLogicalCE & gubPlanesPerBurstMask) + ((uwPhysicalBlock & gubBurstsPerBankMask) << gubPlanesPerBurstLog)) + (U32)((ubLogicalCE >> gubPlanesPerBurstLog) << gubPlanesPerBankLog) ) << gubSectorsPerPlaneLog);
			}

			ubMode = 0;
			ubUseRUT = 0;
		}
		else {

			ulVenderUnit = BUFL_BASE[1];//address 4567
			ulVenderSector = BUFL_BASE[2]; //address 89ab

			ubMode = (UBYTE)BUFW_BASE[8];//byte 16
			ubUseRUT = (UBYTE)(BUFW_BASE[8] >> 8); //byte17
		}

		ubPlaneBank = M_Modular((ulVenderSector >> gubSectorsPerPlaneLog), DEF_PlanesPerPage_Mod_Mask);

		if (ENABLE_PLANE_CH_CE) {
			ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		}
		else {
			ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
		}



		if (ENABLE_DEBUG_UART_VENDER) {
			UartString("\r\n==== Vender Direct Read====   ");
			UartLongHex(gulSectorCnt);
			UartString("\r\n ubPlaneBank: 0x");
			Uart_Tx_DataHex(ubPlaneBank);
			UartString(" ubCEIndex: 0x");
			Uart_Tx_DataHex(ubCEIndex);
			UartString(" ulVenderUnit: 0x");
			UartLongHex(ulVenderUnit);
			UartString(" ulVenderSector: 0x");
			UartLongHex(ulVenderSector);
			UartString(" ubWholePage: 0x");
			Uart_Tx_DataHex(ubWholePage);
			UartString(" ubMode: 0x");
			Uart_Tx_DataHex(ubMode);
			UartString(" ubUseRUT: 0x");
			Uart_Tx_DataHex(ubUseRUT);
		}

		if (ubMode || ubA2cmd) {
			ubIsFastpage = 1;       //FastPage
		}
		else {
			ubIsFastpage = 0;       //Normal
		}

		mDMAC_SV(1,  (U32)BUFB_BASE, (gubSectorsPerPlane + 1) * 512, 0, 0, 0, DMA_EN_SETCQ, 1);
		HandleStopRW(1);
		FlashQueue_t *FQ;
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);


		FQ->ulRAMAddr = (U32)BUFB_BASE;
		FQ->ubCEIndex = ubCEIndex;
		FQ->btCheckEmpty = 1;
		FQ->btThisIsErasePage = 0;
		FQ->ubFJob = BYTE_FJOB_READ;

		if (ubUseRUT == 0) {
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
		}
		else {
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
		}

		FQ->uwFUnit = ulVenderUnit;
#if TLC_NES_TEST_SB_FLOW
		if (gubNESReadIBF) {
			FQ->ulFEntry = ((ulVenderSector >> gubSectorsPerPlaneLog) << gub4kEntrysPerPlaneLog) + ((ulVenderSector & gubSectorsPerPlaneMask) >> 3) ;
		}
		else
#endif
		{
			FQ->ulFEntry = ((ulVenderSector >> gubSectorsPerPlaneLog) << gub4kEntrysPerPlaneLog); //ulVenderSector >> (gubSectorsPer4KLog);
		}
		FQ->ubDepth = ubDepth;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btUNServed = 1;
		FQ->btD3 = ubD3;

#if TLC_NES_TEST_SB_FLOW
		if (gubNESReadIBF) {
			FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH0_EN);
			FCONL[FCONL_FLH_FUNC] &= (~IBUF_RING_CH1_EN);
			//			FCONL[FCONL_FLH_FUNC] &= (~FLH_USE_IBUF_RING);

			FQ->uwFJobInfo |= BIT_FJOBI_RETRY;
			FQ->FQPara.B.btConvBypass = 1;
			FQ->ubL4kNum = ubL4KNum;

			guwNESFQIndex = FQ->ubL4kIndex;
			// test
			//			FQ->btGenFail = 1;
			//			FQ->btCheckEmpty = 0; // Test
		}
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
			xthal_dcache_region_invalidate(BUFB_BASE, (gubSectorsPerPlane + 1) * 512);
		}


		L4KTable16B pL4KTable;
		UBYTE ubL4KCount = 0;
		L4KTable16B *pL4KTablePtr;

		for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA = 0x00000000;
			pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
			pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
			pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
			pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
			*pL4KTablePtr = pL4KTable;
		}

		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

		// wait CE read done...
		while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
		if (FQ->btThisIsErasePage) {
			if (ENABLE_DEBUG_UART_VENDER) {
				UartString("\nThis is ErasePage");
			}
		}

		UBYTE *ubBuffer1Base;

		if (ubWholePage) {
			ubBuffer1Base = &BUFB_BASE[gubSectorsPerPlane * 512];
		}
		else {
			//copy data from location
			gubTemp = ulVenderSector & gubSectorsPerPlaneMask; //ex: 4K-page, 就看ulVenderSector是sector0~7哪個sector
			if (gubTemp != 0) {     //每次都是讀one page, 將要回的sector搬到BUFB_BASE[0]
				memcpy(&BUFB_BASE[0], &BUFB_BASE[gubTemp * 512], 512);
			}
			ubBuffer1Base = &BUFB_BASE[512];
		}
		memcpy((ubBuffer1Base), (void *)&IRAMB[ L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize)], 8);

		pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[(FQ->ubCEIndex & gubPlanesPerBurstMask)];
		*((UWORD *)(&ubBuffer1Base[8])) = (UWORD)pFlaReg[FCTLL_ECC_INFO].L;
		ubBuffer1Base[9] |= (FQ->btThisIsErasePage) ? 0x80 : 0;
		ubBuffer1Base[10] = ubPlaneBank;  // plane bank
		ubBuffer1Base[11] = 0xFF;
		ubBuffer1Base[12] = (UBYTE)(ulVenderSector / guwSectorsPerPage);  // Fpage
		ubBuffer1Base[13] = 0xFF;
		ubBuffer1Base[14] = FQ->ulFEntry >> 8;
		ubBuffer1Base[15] = FQ->ulFEntry & 0xFF;

		if (ubWholePage) {
			memcpy((ubBuffer1Base + 16), (void *)&IRAMB[ L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize)], ubL4KNum * Spare4kSize);
		}

		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, gulSectorCnt * 512);
		}

		gubNeedSendCmdComplete = 0;

		U32 ulTemp;
		ulTemp = HL[HL_BUF_R_EP];

		if (ubWholePage) {
			HL[HL_BUF_R_EP] = 4; //transfer 16KB+1sector, so need set 20KB, 16K BUFFER3+4K, use gubHostTable
		}

		HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

		HL[HL_BUF_R_EP] = ulTemp;
		gubStopFTL = 0;

#if TLC_NES_TEST_SB_FLOW
		if (gubNESReadIBF) {
			gubNESReadIBF = 0;
			if (ubBackupCRC32Enable)
				FCONL[FCONL_CRC_EN] |= (CRC_32_EN);
		}
#endif
#endif
#endif
#if ENABLE_MODIFY_EC
		UWORD uwUnitIndex = 0;
		for (uwUnitIndex = VTAREA_UNITS; uwUnitIndex < guwFWTotalUnitNum; uwUnitIndex++) {
			gulEC_D3[uwUnitIndex].B.ulEraseCount = 500;
		}
#if !BURNER
		ftlSaveTable(&VT->guwTableTarget[VT->gubTableTargetIndex], (U32)gulEC_D3, 0, BYTE_Save_EC_Mode);
		VT->gubTableTargetSyncIndex = VT->gubTableTargetIndex;
		VT->guwTableTargetSyncPTR = VT->guwTableTargetPTR;
		ftlSaveVT(0, VT_FTL_TASK_0x3D);
#endif
#endif

	}
}

#if BURNER
void Vender_Direct_Write_DBT16KPage(void)
{
	UBYTE IndexOfDBT, ubDie;
	UBYTE ubCEIndex;
	UWORD uli;
	U32 ulFreeblock;
	UBYTE *ubDBT16KPageBuffer = (UBYTE*)(BUFFER3_BASE);//0x60527000
	UBYTE *ubTotalDBTBuffer = (UBYTE *)(&GCGRSearchTable[0]);

	U32 ulTotalDBTSizeInByte = gulPhyBlocksPerCE * gubCENumber;
	//UWORD uwRemainderDBTSize;
	UBYTE ubDBTUnit, ubDBTBurst;
	UBYTE ubLastDBTStartToProgram = FALSE;
	UBYTE ubLaterBadHappen;
	//ubLastDBT = (HB[HB_LBA_M] & BIT0) ? 1 : 0;

	IndexOfDBT = HB[HB_LBA_H];

	if ((ulTotalDBTSizeInByte - (gubSectorsPerPlane * 512 * IndexOfDBT)) <= (UWORD)(gubSectorsPerPlane * 512)) {
		//uwRemainderDBTSize = (UWORD)(ulTotalDBTSizeInByte - (gubSectorsPerPlane * 512 * IndexOfDBT));//the last DBT Page
		ubLastDBTStartToProgram = TRUE;
	}
	//else {
	//	uwRemainderDBTSize = (UWORD)(gubSectorsPerPlane * 512);
	//}

	mDMAC_SV(1,  (U32)ubDBT16KPageBuffer, (U32)(gubSectorsPerPlane * 512), 0, 0xFF, 0, DMA_EN_SETCQ, 1);
	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

	/*for (uwi = uwRemainderDBTSize ; uwi < (UWORD)(ubSectorCnt * 512) ; uwi ++) {
		ubDBT16KPageBuffer[uwi] = 0xFF;
	}*/

	if (IndexOfDBT == 0) {
		mDMAC_SV(1, (U32)ubTotalDBTBuffer, ((U32)gubSectorsPerPlane) * 512 * 32, 0, 0xFFFFFFFF, 0, DMA_EN_SETCQ, 1);		// 假設最大16CE, 4 plane, 8192 unit, 16*4*8192/16K = 32, 留ulEntryPerDBTPlane*32 = 512K 一定夠放DBT
	}

	//mDMAC_COPY((U32)&ubDBT16KPageBuffer, (U32)&ubTotalDBTBuffer[(gubSectorsPerPlane * 512 * IndexOfDBT)], uwRemainderDBTSize, 0, 0, 0, 0, 1);
	mDMAC_COPY((U32)ubDBT16KPageBuffer, (U32)&ubTotalDBTBuffer[(gubSectorsPerPlane * 512 * IndexOfDBT)], ((U32)gubSectorsPerPlane) * 512, 0, 0, 0, 0, 1);

	if (ubLastDBTStartToProgram == TRUE) {//MP 已經傳完完整DBT, 開始寫入flash
		UBYTE ubLastDBTIndex = IndexOfDBT;
		if (!RDT_FindDBTBlock_Task()) {	//Old DBT found
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				ubDBTUnit = guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog;
				ubDBTBurst = guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask;
				ftl_Erase_Single_Plane(ubDBTUnit, ubDBTBurst, ubCEIndex, 0);//old DBT EraseFail先不處理
			}
		}

		do {
			//1.根據傳入的DBT挑選DBT要寫入的block
			ubLaterBadHappen = FALSE;
			gubDBTSet = 0;
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				uli = ((U32)(ubCEIndex << gubBurstsPerBankLog));
				UWORD uwBlockNumber = 0;
				while (uli < ulTotalDBTSizeInByte) {
					if (ubTotalDBTBuffer[uli] == 0x00) {
						guwDBTBlock[ubCEIndex] = uwBlockNumber;
						break;
					}

					if ((uwBlockNumber & gubBurstsPerBankMask) == gubBurstsPerBank - 1) {
						uli += (((U32)(gubPlanesPerPage - (UBYTE)(M_Modular(uli, DEF_PlanesPerPage_Mod_Mask)))) + (U32)(gubPlanesPerPage * (gubDieNumber - 1)) + (U32)(ubCEIndex << gubBurstsPerBankLog));
					}
					else {
						uli++;
					}
					uwBlockNumber++;

					if ((uwBlockNumber >> gubBurstsPerBankLog) >= SYSTEMAREA_DEFAULT_SCAN_RANGE) { //超過100個Unit了
						SetFailCEtoSystemInfo(ubCEIndex);
						ftlPreformatShowError(ubCEIndex, uwBlockNumber, 0x80);
						return;
					}
				}
			}
			if (ENABLE_DEBUG_SOFTWAREECC) {
				UartString("\n DBTBlk[0]=");
				UartWordHex(guwDBTBlock[0]);
				UartString(" DBTBlk[1]=");
				UartWordHex(guwDBTBlock[1]);
				UartString(" Set:");
				Uart_Tx_DataHex(gubDBTSet);
			}

			//2.挑完了, Erase it
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				ubDBTUnit = guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog;
				ubDBTBurst = guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask;

				if (ftl_Erase_Single_Plane(ubDBTUnit, ubDBTBurst, ubCEIndex, 0)) {
					if (RDT_BURNER) {
						//mark bad
						uli = (U32)(M_Multiplier((ubDBTUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubDBTBurst;
						ubTotalDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_ERASE_FAIL);
						ubLaterBadHappen = TRUE;
						break;//跳出ubCEIndex loop
					}
					else {
						SetFailCEtoSystemInfo(ubCEIndex);
						ftlPreformatShowError(ubCEIndex , guwDBTBlock[ubCEIndex], 0xEF);
						return;
					}
				}
			}
			if (RDT_BURNER && ubLaterBadHappen == TRUE) {
				continue;//跳出IndexOfDBT loop
			}

			//3.write DBT and Read Check
			for (IndexOfDBT = 0; IndexOfDBT <= ubLastDBTIndex ; IndexOfDBT++) {
				//only for ENABLE_PLANE_CH_CE
				for (ubCEIndex  = 0; ubCEIndex  < gubPlanesPerBurst; ubCEIndex ++) {
					ubDBTUnit = guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog;
					ubDBTBurst = guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask;
					if (ftlWriteSinglePlane(ubDBTUnit, IndexOfDBT,  ubCEIndex , (U32)&ubTotalDBTBuffer[(gubSectorsPerPlane * 512 * IndexOfDBT)] , MARK_DBT, 0xFF012340, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubDBTBurst)) {
						if (RDT_BURNER) {
							//mark bad
							uli = (U32)(M_Multiplier((ubDBTUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubDBTBurst;
							ubTotalDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);
							ubLaterBadHappen = TRUE;
							break;//跳出ubCEIndex loop
						}
						else {
							SetFailCEtoSystemInfo(ubCEIndex);
							ftlPreformatShowError(ubCEIndex , guwDBTBlock[ubCEIndex], 0x12);
							return;
						}
					}
				}
				if (RDT_BURNER && ubLaterBadHappen == TRUE) {
					//把BAD BLOCK裡的DBT清掉
					for (ubCEIndex  = 0; ubCEIndex  < gubPlanesPerBurst; ubCEIndex ++) {
						ubDBTUnit = guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog;
						ubDBTBurst = guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask;
						ftl_Erase_Single_Plane(ubDBTUnit, ubDBTBurst, ubCEIndex, 0);
					}
					break;//跳出IndexOfDBT loop
				}

				//load for check
				ulFreeblock = gubSectorsPerPlane * 512 * IndexOfDBT;
				if (ftlLoadDBT(ulFreeblock, (U32)ubDBT16KPageBuffer)) {
					if (RDT_BURNER) {
						//mark bad
						for (ubCEIndex = 0; ubCEIndex  < gubPlanesPerBurst; ubCEIndex ++) {
							ubDBTUnit = guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog;
							ubDBTBurst = guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask;
							uli = (U32)(M_Multiplier((ubDBTUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubDBTBurst;
							ubTotalDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_READ_FAIL);
							ftl_Erase_Single_Plane(ubDBTUnit, ubDBTBurst, ubCEIndex, 0);
						}
						ubLaterBadHappen = TRUE;
						break;//跳出IndexOfDBT loop
					}
					else {
						for (ubCEIndex = 0; ubCEIndex  < gubPlanesPerBurst; ubCEIndex ++) {
							SetFailCEtoSystemInfo(ubCEIndex);
						}
						ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
						return;
					}
				}
			}
		}
		while (RDT_BURNER && ubLaterBadHappen);

		if (ENABLE_DEBUG_SOFTWAREECC) {
			uli = 0;
			U32 ulTotalDBTOffset;
			UBYTE ubPlaneBank;

			while (uli < ulTotalDBTSizeInByte) {
				for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
					for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
						if (ubTotalDBTBuffer[uli] != 0x00) {
							ulTotalDBTOffset = uli + (IndexOfDBT * gubSectorsPerPlane * 512);
							UartString("\n");//CE
							UartLongHex((ulTotalDBTOffset % gubPlanesPerPage) / gubBurstsPerBank);
							UartString(" ");//Die
							UartLongHex((ulTotalDBTOffset / gubPlanesPerPage) % gubDieNumber);
							UartString(" ");//BLOCK
							UartLongHex(((ulTotalDBTOffset / (gubDieNumber * gubPlanesPerPage)) * gubBurstsPerBank) + (ubPlaneBank % gubBurstsPerBank));
							UartString(" ");
							UartLongHex(ulTotalDBTOffset);
						}
						uli++;
					}
				}
			}
		}
	}
	gubNeedSendCmdComplete = 1;
}
#endif
void Vender_Direct_Write_Page(void)
{
	if (!(ENABLE_BURNER_FORCEGENFAIL)) {
#if 1
		UBYTE ubPlaneBank, ubMode, ubA2cmd = 0;
		UBYTE ubIsFastpage;
		UBYTE ubCEIndex, ubL4KNum = (gubSectorsPerPlane >> SectorsPer4KLog);
		UBYTE ubDepth = 0xFF, ubL4kLink;
		UBYTE ubPhyOrFWMode; // 1: physical addr mode, 0: FW mode
		UBYTE ubPhysicalCE, ubLogicalCE;
		UBYTE ubUseRUT = 0;
		UWORD uwPhysicalBlock, uwPhysicalPage, uwPhysicalSector;
		U32 ulVenderSector;
		U32 ulVenderUnit;

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(BUFB_BASE, 512);
		}

		ubA2cmd = (HB[HB_LBA_M] >> 7);
		ubPhyOrFWMode = HB[HB_LBA_H];

		if (ubPhyOrFWMode) {
			ubPhysicalCE = BUFB_BASE[20];
			uwPhysicalBlock = BUFW_BASE[11];
			uwPhysicalPage =  BUFW_BASE[12];
			uwPhysicalSector = BUFW_BASE[13];

			ubLogicalCE = (ubPhysicalCE >> 3) + ((ubPhysicalCE % 8) << gubPlanesPerBurstLog); //to be change

			if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
				ulVenderUnit = ((uwPhysicalBlock >> gubBurstsPerBankLog) / guwDieBlockShift) * (gulTotalPhyUnitNum / gubDieNumber) + ((uwPhysicalBlock >> gubBurstsPerBankLog) % guwDieBlockShift);
			}
			else {
				ulVenderUnit = (uwPhysicalBlock >> gubBurstsPerBankLog) ; //還沒處理multi group
			}

			if (ENABLE_PLANE_CH_CE) {
				ulVenderSector = (U32)(uwPhysicalSector & gubSectorsPerPlaneMask) + (U32)(((M_Multiplier((U32)uwPhysicalPage, DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubLogicalCE << gubBurstsPerBankLog) + (uwPhysicalBlock & gubBurstsPerBankMask)) << gubSectorsPerPlaneLog );
			}
			else {
				ulVenderSector = (U32) (uwPhysicalSector & gubSectorsPerPlaneMask) + (U32)(((M_Multiplier((U32)uwPhysicalPage, DEF_PlanesPerPage_Mcl_Div)) + (U32)((ubLogicalCE & gubPlanesPerBurstMask) + ((uwPhysicalBlock & gubBurstsPerBankMask) << gubPlanesPerBurstLog)) + (U32)((ubLogicalCE >> gubPlanesPerBurstLog) << gubPlanesPerBankLog) ) << gubSectorsPerPlaneLog);
			}
			ubMode = 0;
			ubUseRUT = 0;
		}
		else {
			ulVenderSector = BUFL_BASE[2]; //AP 3209
			ulVenderUnit = BUFL_BASE[3]; //AP 3209

			ubMode = (UBYTE)BUFW_BASE[8];//byte 16
			ubUseRUT = (UBYTE)(BUFW_BASE[8] >> 8); //byte17
		}

		if (ubMode || ubA2cmd) {
			ubIsFastpage = 1;       //FastPage
		}
		else {
			ubIsFastpage = 0;       //Normal
		}

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(BUFB_BASE, ubL4KNum * 4096);
		}

		mDMAC_SV(1,  (U32)BUFB_BASE, ubL4KNum * 4096, 0, 0, 0, DMA_EN_SETCQ, 1);
		gubNeedSendCmdComplete = 1;

		HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

		ubPlaneBank = M_Modular((ulVenderSector >> gubSectorsPerPlaneLog), DEF_PlanesPerPage_Mod_Mask);
		if (ENABLE_PLANE_CH_CE) {
			ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		}
		else {
			ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
		}

		if (ENABLE_DEBUG_UART_VENDER) {
			UartString("\r\n==== Vender Direct Write====   ");
			UartLongHex(gulSectorCnt);
			UartString("\r\n ubPlaneBank: 0x");
			Uart_Tx_DataHex(ubPlaneBank);
			UartString(" ubCEIndex: 0x");
			Uart_Tx_DataHex(ubCEIndex);
			UartString(" ulVenderSector: 0x");
			UartLongHex(ulVenderSector);
		}
		HandleStopRW(1);
		FlashQueue_t *FQ;
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

		FQ->ulRAMAddr = (U32)BUFB_BASE;
		FQ->ubFJob = BYTE_FJOB_WRITE;

		if (ubUseRUT == 0) {
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
		}
		else {
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
		}

		FQ->uwFUnit = ulVenderUnit;
#if TLC
		//ERIC
		if (TLC) {
			gubD1LinkTable[FQ->uwFUnit].B.ubD1Link = (UBYTE)ulVenderUnit;
		}
		//ERIC
#endif
		FQ->ulFEntry = ((ulVenderSector >> gubSectorsPerPlaneLog) << gub4kEntrysPerPlaneLog);
		FQ->ubRelativeDepth = ubDepth;
		FQ->ubDepth = 0xFF;
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

		L4KTable16B pL4KTable;
		UBYTE ubL4KCount = 0;
		L4KTable16B *pL4KTablePtr;

		for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA = 0xABCDFBFB;
			pL4KTable.Para0x04.BitMap.ulL4K_FW = 0x112233;
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

		// wait CE read done...
		while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}

		FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
		gubStopFTL = 0;
#endif
	}
}

void Vender_Direct_Erase_Block(void)
{
	if (!(ENABLE_BURNER_FORCEGENFAIL)) {
#if 1
		UBYTE ubPlaneBank, ubMode = 0, ubA2cmd = 0;
		UBYTE ubD3;//MLC no use
		UBYTE ubIsFastpage;
		UBYTE ubCEIndex;
		UBYTE ubDepth = 0xFF, ubL4kLink;
		UBYTE ubPhyOrFWMode; // 1: physical addr mode, 0: FW mode

		U32 ulVenderSector;
		U32 ulVenderUnit;
		UBYTE ubEraseMode; //0: erase all PlaneBank, 1: erase one PlaneBank
		UBYTE ubUseRUT;

		UBYTE ubPBStart, ubPBEnd;
		UBYTE ubPhysicalCE, ubLogicalCE;
		UWORD uwPhysicalBlock;

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(BUFB_BASE, 512);
		}

		ubA2cmd = (HB[HB_LBA_M] & BIT7) ? 1 : 0;
		ubD3 = (HB[HB_LBA_M] & BIT6) ? 1 : 0;
		ubPhyOrFWMode = HB[HB_LBA_H];

		if (ubPhyOrFWMode) {
			ubPhysicalCE = BUFB_BASE[20];
			uwPhysicalBlock = BUFW_BASE[11];
			ubEraseMode = 1;//erase one planebank

			ubLogicalCE = (ubPhysicalCE >> 3) + ((ubPhysicalCE % 8) << gubPlanesPerBurstLog); //to be change

			if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
				ulVenderUnit = ((uwPhysicalBlock >> gubBurstsPerBankLog) / guwDieBlockShift) * (gulTotalPhyUnitNum / gubDieNumber) + ((uwPhysicalBlock >> gubBurstsPerBankLog) % guwDieBlockShift);
			}
			else {
				ulVenderUnit = (uwPhysicalBlock >> gubBurstsPerBankLog) ; //還沒處理multi group
			}

			if (ENABLE_PLANE_CH_CE) {
				ulVenderSector = (U32)(((U32)(ubLogicalCE << gubBurstsPerBankLog) + (uwPhysicalBlock & gubBurstsPerBankMask)) << gubSectorsPerPlaneLog );
			}
			else {
				ulVenderSector = (U32)(((U32)((ubLogicalCE & gubPlanesPerBurstMask) + ((uwPhysicalBlock & gubBurstsPerBankMask) << gubPlanesPerBurstLog)) + (U32)((ubLogicalCE >> gubPlanesPerBurstLog) << gubPlanesPerBankLog) ) << gubSectorsPerPlaneLog);
			}


			ubPlaneBank = M_Modular((ulVenderSector >> gubSectorsPerPlaneLog), DEF_PlanesPerPage_Mod_Mask);

			ubPBStart = ubPlaneBank;
			ubPBEnd = ubPlaneBank + 1;
			ubUseRUT = 0;
		}
		else {
			ubEraseMode = BUFB_BASE[1]; //0: erase all PlaneBank, 1: erase one PlaneBank
			ulVenderSector = BUFL_BASE[2];
			ulVenderUnit = BUFL_BASE[3];
			ubUseRUT = (UBYTE)(BUFW_BASE[8] >> 8); //byte17

			if (ubEraseMode == 0) {
				ubPBStart = 0;
				ubPBEnd = gubPlanesPerPage;
			}
			else {
				ubPlaneBank = M_Modular((ulVenderSector >> gubSectorsPerPlaneLog), DEF_PlanesPerPage_Mod_Mask);
				ubPBStart = ubPlaneBank;
				ubPBEnd = ubPlaneBank + 1;
			}
		}

		if (ubMode || ubA2cmd) {
			ubIsFastpage = 1;       //FastPage
		}
		else {
			ubIsFastpage = 0;       //Normal
		}

		if (ENABLE_DEBUG_UART_VENDER) {
			UartString("\r\n==== Vender Direct Erase====");
			UartString("\r\n ubPhysicalCE: 0x");
			Uart_Tx_DataHex(ubPhysicalCE);
			UartString(" ubLogicalCE: 0x");
			Uart_Tx_DataHex(ubLogicalCE);
		}

		gubNeedSendCmdComplete = 1;
		HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

		for (ubPlaneBank = ubPBStart; ubPlaneBank < ubPBEnd; ubPlaneBank++) {
			if (ubPhyOrFWMode == 0) {
				if (ENABLE_PLANE_CH_CE) {
					ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
				}
				else {
					ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
				}
			}
			else {
				ubCEIndex = ubLogicalCE;
			}
			HandleStopRW(1);
			FlashQueue_t *FQ;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;

			if (ubUseRUT == 0) {
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
			}
			else {
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
			}


			// Must Send Reset CMD(0xFF) After Erase Fail
			FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;


			FQ->uwFUnit = ulVenderUnit;
			FQ->ulFEntry = ((U32)ubPlaneBank << gub4kEntrysPerPlaneLog);
			FQ->ubRelativeDepth = ubDepth;
			FQ->ubDepth = 0xFF;
			FQ->btUNServed = 1;
			FQ->btD3 = ubD3;
			FQ->btCheckEmpty = 0;

			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

			while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
			gubStopFTL = 0;
		}
#endif
	}
}

void Vender_BusyTime()
{
#if 0
	//do Erase 1 block, write 1 page, read 1 page
	U32 ulVenderSector = BUFL_BASE[2];
	U32 ulVenderUnit = BUFL_BASE[1];
	UBYTE ubPlaneBank;
	UBYTE ubCEIndex;
	UBYTE ubDepth;
	UBYTE ubL4kLink;
	UBYTE ubL4KNum = (gubSectorsPerPlane >> 3),  ubIsFastpage = 0;
	L4KTable16B *pL4KTable;
	UBYTE ubL4KIndex;
	UBYTE ubL4KCount;
	U32 ulCmdTime[3];

	mDMAC_SV(4, BUFB_BASE, 16 * 1024, 0, 0x11223344, 0, 0, 0, 1);

	ubPlaneBank = (ulVenderSector >> gubSectorsPerPlaneLog)&gubPlanesPerPageMask;
	ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + ((((ubPlaneBank >> gubPlanesPerBankLog)&gubBanksPerPageMask) << gubPlanesPerBurstLog));

	RTT1Clock_Init(RTT_SCALE_1US);
	RTT1_SetTime_ReStart();
	//------------------ Erase -------------------------//
	FlashQueue_t *FQ;
	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

	FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
	FQ->uwFJobInfo = ( BIT_FJOBI_WAITRESULT | BIT_FJOBI_ERASE_FAIL_RESET);
	FQ->uwFUnit = ulVenderUnit;
	FQ->ulFEntry = (ubPlaneBank << gub4kEntrysPerPlaneLog);
	FQ->ubRelativeDepth = ubDepth;
	FQ->ubDepth = 0xFF;
	FQ->btUNServed = 1;
	FQ->btCheckEmpty = 0;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
	gMTQ->Depth[ubDepth].Para0x04.BitMap.ubALUSel = 1;
	gMTQ->Depth[ubDepth].Para0x00.BitMap.btSetInternalBusy = 1;
	gMTQ->Depth[ubDepth].Para0x1C.BitMap.ubCEValue = ubCEIndex / gubPlanesPerBurst;
	gFQI.ubFQLinkNumber++;
	gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

	while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
		M_SwitchTask();
	}

	FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

	ulCmdTime[0] = RTT_Get_Value(1, 0xFF);
	RTT1_SetTime_ReStart();

	//------------------ Write -------------------------//
	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

	FQ = &gFQI.gFQLink[gFQI.ubFQLinkFree];
	FQ->ubCEIndex = ubCEIndex;
	FQ->ubFJob = BYTE_FJOB_WRITE;
	FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
	FQ->uwFUnit = ulVenderUnit;
	FQ->ulFEntry = (ulVenderSector >> gubSectorsPerPlaneLog) << gub4kEntrysPerPlaneLog;
	FQ->ubRelativeDepth = ubDepth;
	FQ->ubDepth = 0xFF;
	FQ->ubL4kIndex = ubL4kLink;
	FQ->btUNServed = 1;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.ubFrameNum = ubL4KNum;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.btBufModeEn  = 0;
	gMTQ->Depth[ubDepth].Para0x04.BitMap.ubALUSel = 1;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize);
	gMTQ->Depth[ubDepth].Para0x1C.BitMap.ubCEValue = ubCEIndex / gubPlanesPerBurst;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.btZipBypass = 1;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.ubL4KNum = ubL4KNum;

	ubL4KCount = 0;
	for (ubL4KIndex = ubL4kLink; ubL4KIndex != 0xFF; ubL4KIndex = gL4kFreeQ.L4kQueue[ubL4KIndex].ubNext ) {
		pL4KTable = (L4KTable16B*)( L4K_TABLE_ADDR + (ubL4KIndex * Spare4kSize));
		//set L4K table info
		pL4KTable->ulL4K_LCA = 0xABCDFBFB;
		pL4KTable->Para0x04.BitMap.ulL4K_FW = 0x112233;
		pL4KTable->Para0x04.BitMap.ubL4K_SPRV = 0xFF;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = L4K_TABLE_OFFSET + gL4kFreeQ.L4kQueue[ubL4KIndex].ubNext * Spare4kSize;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = 0xAB;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)BUFB_BASE + (ubL4KCount * 4096)) / 512;
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0x7;
		ubL4KCount++;
	}
	gFQI.ubFQLinkNumber++;
	gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

	// wait CE read done...
	while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
		M_SwitchTask();
	}

	FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

	ulCmdTime[1] = RTT_Get_Value(1, 0xFF);

	mDMAC_SV(4, BUFB_BASE, 16 * 1024, 0, 0xFFFFFFFF, 0, 0, 0, 1);
	RTT1_SetTime_ReStart();
	//------------------ Read -------------------------//
	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

	FQ = &gFQI.gFQLink[gFQI.ubFQLinkFree];
	FQ->ubCEIndex = ubCEIndex;
	FQ->btCheckEmpty = 1;
	FQ->btThisIsErasePage = 0;
	FQ->ubFJob = BYTE_FJOB_READ;
	FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
	FQ->uwFUnit = ulVenderUnit;
	FQ->ulFEntry = (ulVenderSector >> gubSectorsPerPlaneLog) << gub4kEntrysPerPlaneLog;
	FQ->ubDepth = ubDepth;
	FQ->ubL4kIndex = ubL4kLink;
	FQ->btUNServed = 1;
	//guwINTTable[ubCEIndex*MT_QUEUE_DEPTH+ubDepth].B.btOccupied = 1;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.ubFrameNum = ubL4KNum;//gubSectorsPerPlane / 8;
	//gMTQ->Depth[ubDepth].Para0x04.BitMap.btMultiBufPtrModeEn = 0;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.btBufModeEn = 0;
	gMTQ->Depth[ubDepth].Para0x04.BitMap.ubALUSel = 1;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize);
	gMTQ->Depth[ubDepth].Para0x1C.BitMap.ubCEValue = ubCEIndex / gubPlanesPerBurst;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.ubL4KNum = ubL4KNum;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.btZipBypass = 1;
	gMTQ->Depth[ubDepth].Para0x24.BitMap.btCmpEn = 0;
	gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;
	//gMTQ->Depth[ubDepth].Para0x04.BitMap.btIntrEnUpdate = 1;
	//gMTQ->Depth[ubDepth].Para0x1C.BitMap.uwIntrVec = ubCEIndex*MT_QUEUE_DEPTH+ubDepth;

	ubL4KCount = 0;
	for (ubL4KIndex = ubL4kLink; ubL4KIndex != 0xFF; ubL4KIndex = gL4kFreeQ.L4kQueue[ubL4KIndex].ubNext ) {
		pL4KTable = (L4KTable16B*)( L4K_TABLE_ADDR + (ubL4KIndex * Spare4kSize));
		//set L4K table info
		pL4KTable->ulL4K_LCA = 0x00000000;
		pL4KTable->Para0x04.BitMap.ulL4K_FW = 0x000000;
		pL4KTable->Para0x04.BitMap.ubL4K_SPRV = 0x00;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = L4K_TABLE_OFFSET + gL4kFreeQ.L4kQueue[ubL4KIndex].ubNext * Spare4kSize;
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = ((U32)BUFB_BASE + (ubL4KCount * 4096)) / 512;

		ubL4KCount++;
	}
	gFQI.ubFQLinkNumber++;
	gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

	// wait CE read done...
	while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
		M_SwitchTask();
	}
	FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

	ulCmdTime[2] = RTT_Get_Value(1, 0xFF);

	UartString("\nErase 1 block Time(us) :  ");
	UartLongHex(ulCmdTime[0]);
	UartString("\nWrite 1 page Time(us) :  ");
	UartLongHex(ulCmdTime[1]);
	UartString("\nRead 1 page Time(us) :  ");
	UartLongHex(ulCmdTime[2]);

	//memset(BUFB_BASE, 0, 512);
	mDMAC_SV(4, BUFB_BASE, 512, 0, 0, 0, 0, 0, 1);

	BUFL_BASE[0] = ulCmdTime[0];
	BUFL_BASE[1] = ulCmdTime[1];
	BUFL_BASE[2] = ulCmdTime[2];


	gubNeedSendCmdComplete = 0;
	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
#endif
}

void Vender_GetBackGroundStatus()
{
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	mSE_SV(4, BUFFER3_BASE, 512, 0 , 0);


	//GC
	BUFB_BASE[0] = (((VT->gulFTLState.B.btNeedCloseTarget) && (VT->gulFTLState.B.btNeedCopyData)) ? 1 : 0);
	BUFB_BASE[1] = (((VT->gulFTLState.B.btNeedCloseTarget) && (VT->gulFTLState.B.btNeedCleanGCGR)) ? 1 : 0);
	BUFB_BASE[2] = (((VT->gulFTLState.B.btNeedWearLeveling) && (VT->gulFTLState.B.btNeedCopyData)) ? 1 : 0);
	BUFB_BASE[3] = (((VT->gulFTLState.B.btNeedWearLeveling) && (VT->gulFTLState.B.btNeedCleanGCGR)) ? 1 : 0);
	//B4~B15 reserved

	BUFL_BASE[4] = (VT->gubGCGRTargetNum * gulPlanesPerUnit); //total copy
	BUFL_BASE[5] = ((VT->gubGCGRTargetIndex * gulPlanesPerUnit) + VT->gulGCGRTargetPTR);//now copy
	BUFL_BASE[6] = gulCleanP2LGroupCount;//total update
	BUFL_BASE[7] = gulCleanP2LGroupIndex;//now update
	//WL

	//SMART offline
	BUFB_BASE[32] = 	gubOfflineMode;
	BUFB_BASE[33] = AtaCfg.gSMARTSelective.ubOffLineDoing;
	BUFW_BASE[17] = AtaCfg.gSMARTSelective.uwCurrentSpan;
	BUFW_BASE[18] = AtaCfg.gSMARTSelective.uwPendingTime;
	BUFW_BASE[19] = AtaCfg.gSMARTSelective.uwFeatureFlags;
	BUFL_BASE[10] = AtaCfg.ulSMARTSelfTestStartLBA;

	BUFB_BASE[44] = AtaCfg.ubSmartState;
	BUFB_BASE[45] = AtaCfg.ubOfflineStatus;
	BUFB_BASE[46] = AtaCfg.ubSelfTestStatus;
	//B47 reserved

	BUFL_BASE[12] = AtaCfg.gSMARTSelective.ulStartLBA[0];
	BUFL_BASE[13] = AtaCfg.gSMARTSelective.ulStartLBA[1];
	BUFL_BASE[14] = AtaCfg.gSMARTSelective.ulStartLBA[2];
	BUFL_BASE[15] = AtaCfg.gSMARTSelective.ulStartLBA[3];

	BUFL_BASE[16] = AtaCfg.gSMARTSelective.ulStartLBA[4];
	BUFL_BASE[17] = AtaCfg.gSMARTSelective.ulEndLBA[0];
	BUFL_BASE[18] = AtaCfg.gSMARTSelective.ulEndLBA[1];
	BUFL_BASE[19] = AtaCfg.gSMARTSelective.ulEndLBA[2];

	BUFL_BASE[20] = AtaCfg.gSMARTSelective.ulEndLBA[3];
	BUFL_BASE[21] = AtaCfg.gSMARTSelective.ulEndLBA[4];
	BUFL_BASE[22] = gulSMARTSelfTestCurrentLBA;
	//L23~L27 reserved

	//sanitize
	BUFB_BASE[112] = AtaCfg.ubSanitizeState;
	BUFB_BASE[113] = AtaCfg.ubSanitizeValue;
	//B114,B115 reserved

	BUFB_BASE[116] = AtaCfg.gSanitizeOverwrite.ubCurrentCount;
	BUFB_BASE[117] = AtaCfg.gSanitizeOverwrite.ubEndCount;
	BUFB_BASE[118] = AtaCfg.gSanitizeOverwrite.ubNeedInvert;
	BUFB_BASE[119] = AtaCfg.gSanitizeOverwrite.ubSuspend;

	BUFL_BASE[30] = AtaCfg.gSanitizeOverwrite.ulPattern;
	BUFL_BASE[31] = gulSanitizeCurrentLBA;


	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}

	Vender_TriggerHostData();
}

void Vender_ClearSMART()
{
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

	VT->guwFWUpdateCount = 0;
	VT->guwSystemUpdateCount = 0;

	VT->guoTotalHostReadSectorCnt = 0;
	VT->guoTotalHostWriteSectorCnt = 0;
	VT->guoTotalNandReadSectorCnt = 0;
#if TLC
	VT->guoTotalNandWritePlaneCnt_D1Data = 0;
	VT->guoTotalNandWritePlaneCnt_D3TLCData = 0;
	VT->guoTotalNandWritePlaneCnt_D3SLCData = 0;
	VT->guoTotalNandWritePlaneCnt_D3SLCTable = 0;
#else
	VT->guoTotalNandWriteSectorCnt = 0;
	VT->guoTotalNandWriteData = 0;
	VT->guoTotalNandWriteTable = 0;
#endif
	VT->gulPowerOnCnt = 0;
	VT->gulPowerCyclingCnt = 0;
	VT->gulNonStandByPowerDownCnt = 0;
	VT->gulPowerOnTimeInMinutes = 0;

	VT->gulReadCRCCnt = 0;
	VT->gulWriteCRCCnt = 0;
	VT->gulHostReadECCCnt = 0;

	VT->gulTotalEraseCount = 0;

	VT->gulReadRetryCnt = 0;

#if Read_Chk_ByHostRead
	VT->guoD3UnitTotalReadCnt = 0;
	VT->guoD1UnitTotalReadCnt = 0;
#endif

	UWORD uwUnitIndex;
	U32 ulUnitEraseCnt;
	U32 ulMaxEraseCnt = 0;
	for (uwUnitIndex = 0; uwUnitIndex < guwFWTotalUnitNum; uwUnitIndex++) {
		ulUnitEraseCnt = gulEC_D3[uwUnitIndex].B.ulEraseCount;
		gulEC_D3[uwUnitIndex].B.ulEraseCount = 0;

		if (ulUnitEraseCnt > ulMaxEraseCnt) {
			ulMaxEraseCnt = ulUnitEraseCnt;
		}
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)gulEC_D3, 16384);
	}

	VT->gulPreviousMaxEraseCnt_D3 += ulMaxEraseCnt;

	VT->gulFTLState.B.btFlushCache = 0;//if want to save VT, need set to 0.
	gubFlushMode = BYTE_STANDBY_FLUSH;
	FlushCache();

	HandleStopRW(1);
	ftlSwapTable(BYTE_EC_Save_Mode, 0);
	gubStopFTL = 0;

	VT->gulFTLState.B.btFlushCache = 0;//if want to save VT, need set to 0.
	gubFlushMode = BYTE_STANDBY_FLUSH;
	FlushCache();
}

void Vender_DisableFreezeLock(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	AtaCfg.ubSecMode &= ~(SET_SEC_FROZEN | SET_SEC_LOCK);
	AtaCfg.uwDCOState &= (~SET_DCO_LOCK); //clear DCO lock
	SaveAtaConfig();
}

void Vender_ReadSystemInfo(void)
{
	//////disable by Lin/////////////////////////////
	//     M_CheckRDTModeToDisableFunction();
	UBYTE ubi;

	UBYTE *ubTemp_BUF;
	UWORD *uwTemp_BUF;
	U32 *ulTemp_BUF;
	UBYTE ubCEIndex;
	Variable_Table *VT_temp = (Variable_Table *)BUFFER3_BASE;

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

	if ((guwErrorID & BIT15) && BURNER) {
		HL[HL_BUF_BASE] = BUFFER2_BASE / 4096;
		HL[HL_BUF_R_EP] = BUFFER2_READ_END;
		HL[HL_BUF_W_EP] = BUFFER2_WRITE_END;
		HL[HL_BUF_R_SHF] = BUFFER2_READ_START;
		HL[HL_BUF_W_SHF] = BUFFER2_WRITE_START;
	}

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		if (guwErrorDumpBlock[ubCEIndex] != 0xFFFF) {
			if (ftlReadSinglePlane( (guwErrorDumpBlock[ubCEIndex] >> gubBurstsPerBankLog), 0, ubCEIndex, (U32)VT_temp, MARK_ErrorDumpUnit,  (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), gub4kEntrysPerPlane, 0, (guwErrorDumpBlock[ubCEIndex]&gubBurstsPerBankMask)) == 0) {
				if ( (VT_temp->gubSaveVTSerialNumber >= VT_ASSERT_MPRW_WRITEFAIL_0x96 ) && (VT_temp->gubSaveVTSerialNumber <= VT_ASSERT_MPRW_RSFAIL_0x99)) {
					guoCE_FailMap = VT_temp->gulMPRWCE_FailMap;
					break;
				}
			}
		}
	}

	//BufA : System Parameter
	//BufB : Infoblock content
	memset((void *)ubTemp_BUF, 0x00, 512);
	//mSE_SV(8, (U32)ubTemp_BUF, 512, 0, 0);
	ubTemp_BUF[0] = (gubExpandCE) ? gubMTQTotalPhyCE : gubCENumber; //offset 0
	ubTemp_BUF[1] = gubSystemSet; //Matt, spec not define yet.
	ubTemp_BUF[2] = gubPlanesPerBurst;
	ubTemp_BUF[3] =  flaCheckLogValue(gubSDRSizeMB) ;
	ubTemp_BUF[4] = (UBYTE)guwPagesPerUnit;
	ubTemp_BUF[5] = (UBYTE)(guwPagesPerUnit >> 8); // only zero can no fill it
	ubTemp_BUF[6] = (gubExpandCE) ? (gubDieNumber * gubExpandCE_Ratio) : gubDieNumber ;
	ubTemp_BUF[7] =  ((gubBanksPerPage == 4) ? BIT2 : 0x00) | ((gubBanksPerPage == 2) ? BIT1 : 0x00) | ((gubBurstsPerBank == 2) ? BIT0 : 0x00);
	ubTemp_BUF[8] = gubDBTSet;
	ubTemp_BUF[9] = gubErrorDumpSet;
	ubTemp_BUF[10] = gubExpandCE;
	ubTemp_BUF[11] = gubExpandCE_Ratio;
	ubTemp_BUF[12] = (gubExpandCE) ? (gubSelDieNumber * gubExpandCE_Ratio) : gubSelDieNumber ;
	ubTemp_BUF[13] = gRRTMode.All;
	ulTemp_BUF[16 >> 2] =  gulPhyBlocksPerCE / gubDieNumber;
	//BUFL_BASE[20 >> 2] = reserved;
	//BUFL_BASE[24 >> 2] = reserved;
	ulTemp_BUF[28 >> 2] =  (gubExpandCE) ? (gulPhyBlocksPerCE * gubExpandCE_Ratio) : gulPhyBlocksPerCE;
	ulTemp_BUF[32 >> 2] =  (U32)gubSysCodeSet[0];
	ulTemp_BUF[36 >> 2] =  (U32)gubSysCodeSet[1];
	ulTemp_BUF[40 >> 2] = (U32)gubSectorsPerPlane; //Sector Per Page ( if 8K, =16)
	ulTemp_BUF[44 >> 2] = 0xFFFFFFFF; //Reserved for Burn In Test use

	SMART_ERASECOUNT_INFO EraseCountInfo;
	CalculateEraseCountInfo(&EraseCountInfo);
	BUFL_BASE[48 >> 2] = EraseCountInfo.TypeD3.ulTotalEraseCnt;
	BUFL_BASE[52 >> 2] = EraseCountInfo.TypeD1.ulTotalEraseCnt;

	U32 ulTotalReadFailCnt = 0;
	U32 ulTotalProgramFailCnt = 0;
	U32 ulTotalEraseFailCnt = 0;
	U32 ulTotalRSRecoveryMarkBadCnt = 0;
	U32 ulTotalLaterBadCnt = 0;
	if (ENABLE_RUT2DBT && (!gubMPRW)) { //MPRW fail 時不做避免timeout
		U32 ulEntryPerDBTPlane; // 一個plane存了幾個block state......
		U32 ulCount;
		U32 ulDBTEntryIndex;
		UBYTE *ubDBTBuffer = (UBYTE*) gubHostTable;
		U32 ulTotalBlockNum;
		U32 ulTotalBlksPerPlaneBank;

		ulEntryPerDBTPlane = (U32)(gubSectorsPerPlane * 512);
		ulTotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;
		ulTotalBlksPerPlaneBank = gubDieNumber * (gubBurstsPerBank) * (gubCEsPerPage);

		HandleStopRW(1);

		if ((!RDT) && (!BURNER)) {//only FW run tihs.
			for (ulCount = 0; ulCount < ulTotalBlockNum; ulCount++) {
				ulDBTEntryIndex = (ulCount % ulEntryPerDBTPlane);
				if (ulDBTEntryIndex == 0) {
					if (ftlLoadDBT(ulCount, (U32)&ubDBTBuffer[0])) { // load single plane
						ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
					}
				}

				///Read Fail Cnt
				if (ubDBTBuffer[(ulDBTEntryIndex)] & DBT_MARK_READ_FAIL) {
					ulTotalReadFailCnt++;
				}

				///Prog Fail Cnt
				if (ubDBTBuffer[(ulDBTEntryIndex)] & DBT_MARK_WRITE_FAIL) {
					ulTotalProgramFailCnt++;
				}

				///Erase Fail Cnt
				if (ubDBTBuffer[(ulDBTEntryIndex)] & DBT_MARK_ERASE_FAIL) {
					ulTotalEraseFailCnt++;
				}

				///RS Recovery Mark Bad Cnt
				if (ubDBTBuffer[(ulDBTEntryIndex)] & DBT_MARK_RS_RECOVERY) {
					ulTotalRSRecoveryMarkBadCnt++;
				}

				///LaterBad Cnt
				if (ubDBTBuffer[(ulDBTEntryIndex)] & 0x30) {
					ulTotalLaterBadCnt++;
				}
			}

			if (ENABLE_DEBUG_UART_VENDER) {
				UartString("\r\nRsys LoadDBT");
			}
		}
		else {
			if (ENABLE_DEBUG_UART_VENDER) {
				UartString("\r\nRsys Not LoadDBT");
			}
		}
		gubStopFTL = 0;
	}

	// if ENABLE_RUT2DBT == 0 then ulTotalReadFailCnt/ulTotalProgramFailCnt/ulTotalEraseFailCnt = 0
	ulTemp_BUF[56 >> 2] = ulTotalRSRecoveryMarkBadCnt;//RS Recovery Mark Bad count
	ulTemp_BUF[60 >> 2] = ulTotalReadFailCnt;//Read Uncorrectable ECC count
	ulTemp_BUF[64 >> 2] = ulTotalProgramFailCnt;  //Erase Failed count
	ulTemp_BUF[68 >> 2] = ulTotalEraseFailCnt;//Program Failed count

	ubTemp_BUF[72] = gubBurstsPerPage;
	ubTemp_BUF[73] = gubBurstsPerBank;
	ubTemp_BUF[74] = gubRUTMixPlaneEnable;
	ubTemp_BUF[75] = gubFormFactor;

	if (RDT) {
		ubTemp_BUF[73] = 0;//
	}
	//BUFB_BASE[73] = reserved;
	ulTemp_BUF[76 >> 2] = VT->gulReadCRCCnt; //Reserved for Read CRC Error Count
	ulTemp_BUF[80 >> 2] = VT->gulWriteCRCCnt; //Reserved for Write CRC Error Count
	ulTemp_BUF[84 >> 2] = ulTotalLaterBadCnt;
	ulTemp_BUF[88 >> 2] = VT->gulReadRetryCnt; //Reserved for guwReadMoveCount (RD Use)

	if (RDT) {
		ulTemp_BUF[92 >> 2] = 0;
		ulTemp_BUF[96 >> 2] = 0;
		ulTemp_BUF[100 >> 2] = 0;
		ulTemp_BUF[104 >> 2] = 0;
		ulTemp_BUF[108 >> 2] = 0;
		ulTemp_BUF[112 >> 2] = 0;
		ulTemp_BUF[116 >> 2] = 0;
		ulTemp_BUF[120 >> 2] = 0;
	}

	// Avg/max/min erase count for d3/d1
	ulTemp_BUF[124 >> 2] = EraseCountInfo.TypeD3.ulAvgEraseCnt;
	ulTemp_BUF[128 >> 2] = EraseCountInfo.TypeD1.ulAvgEraseCnt;
	ulTemp_BUF[132 >> 2] = EraseCountInfo.TypeD3.ulMaxEraseCnt;
	ulTemp_BUF[136 >> 2] = EraseCountInfo.TypeD1.ulMaxEraseCnt;
	ulTemp_BUF[140 >> 2] = EraseCountInfo.TypeD3.ulMinEraseCnt;
	ulTemp_BUF[144 >> 2] = EraseCountInfo.TypeD1.ulMinEraseCnt;

	ulTemp_BUF[148 >> 2] = (U32)VT->guoTotalHostWriteSectorCnt;	// host write sector cnt
	ulTemp_BUF[152 >> 2] = (U32)(VT->guoTotalHostWriteSectorCnt >> 32);

#if TLC
	ULLONG uoNandWriteSector_D1 = (VT->guoTotalNandWritePlaneCnt_D1Data * gubSectorsPerPlane);
	ULLONG uoNandWriteSector = ((	VT->guoTotalNandWritePlaneCnt_D3SLCTable
	                                + 	VT->guoTotalNandWritePlaneCnt_D3SLCData
	                                + 	VT->guoTotalNandWritePlaneCnt_D3TLCData)
	                            * gubSectorsPerPlane);
#else
	ULLONG uoNandWriteSector_D1 = 0;
	ULLONG uoNandWriteSector = VT->guoTotalNandWriteSectorCnt;
#endif
	ulTemp_BUF[156 >> 2] = (U32)uoNandWriteSector;	// nand write sector cnt
	ulTemp_BUF[160 >> 2] = (U32)(uoNandWriteSector >> 32);
	ulTemp_BUF[164 >> 2] = (U32)uoNandWriteSector_D1;	// D1 nand write sector cnt
	ulTemp_BUF[168 >> 2] = (U32)(uoNandWriteSector_D1 >> 32);
	ulTemp_BUF[172 >> 2] = (U32)VT->guoTotalHostReadSectorCnt;	// host read sector cnt
	ulTemp_BUF[176 >> 2] = (U32)(VT->guoTotalHostReadSectorCnt >> 32);
	ulTemp_BUF[180 >> 2] = (U32)VT->guoTotalNandReadSectorCnt;	// nand read sector cnt
	ulTemp_BUF[184 >> 2] = (U32)(VT->guoTotalNandReadSectorCnt >> 32);

	// normal/abnormal power cycle cnt, power time
	U32 ulRightNowTimeInMinutes = (U32)(GetRTT_m());
	ulTemp_BUF[188 >> 2] = VT->gulPowerOnCnt;
	ulTemp_BUF[192 >> 2] = VT->gulPowerCyclingCnt;
	ulTemp_BUF[196 >> 2] = (VT->gulPowerOnTimeInMinutes + (U32)(ulRightNowTimeInMinutes - gulLastSavedTimeInMinutes)) / UPDATE_TIME_INTERVAL_MIN;	// Power On Time In Hour

	// 3105AP , command "ReadSystemInfo" used.
	ubTemp_BUF[200] = (gubExpandCE) ? ((ENABLE_SANDISK_1ZNM_SPF) ? (gubMTQTotalPhyCE * 2) : (gubPlanesPerPage / gubExpandCE_Ratio)) : gubPlanesPerPage ;
	ubTemp_BUF[201] = FW_INTERNALVERSION1;
	ubTemp_BUF[202] = FW_INTERNALVERSION2;
	uwTemp_BUF[204 >> 1] = guwDieBlockShift;      //gap per die
	uwTemp_BUF[206 >> 1] = guwFWTotalUnitNum;
	ulTemp_BUF[208 >> 2] =  (gubExpandCE) ? ((ENABLE_SANDISK_1ZNM_SPF) ? (gulTotalPhyUnitNum * gubExpandCE_Ratio - gulTotalPhyUnitNum / 2) : (gulTotalPhyUnitNum * gubExpandCE_Ratio)) : gulTotalPhyUnitNum;
	ulTemp_BUF[212 >> 2] = (gubExpandCE) ? ((ENABLE_SANDISK_1ZNM_SPF) ? (gubMTQTotalPhyCE * gubSectorsPerPlane * gubBurstsPerBank * guwPagesPerUnit) : (gulSectorsPerUnit / gubExpandCE_Ratio)) : gulSectorsPerUnit;
	ulTemp_BUF[216 >> 2] =  (gubExpandCE) ? ((ENABLE_SANDISK_1ZNM_SPF) ? (guwFWTotalUnitNum * gubExpandCE_Ratio - guwFWTotalUnitNum / 2) : (guwFWTotalUnitNum * gubExpandCE_Ratio)) : guwFWTotalUnitNum;

	ulTemp_BUF[220 >> 2] = (U32)guoFlashDefaultType.All[0];
	ulTemp_BUF[224 >> 2] = (U32)(guoFlashDefaultType.All[0] >> 32);
	ulTemp_BUF[228 >> 2] = (U32)guoFlashUseType.All[0];
	ulTemp_BUF[232 >> 2] = (U32)(guoFlashUseType.All[0] >> 32);
	ulTemp_BUF[236 >> 2] = (U32)guoCE_FailMap;
	ulTemp_BUF[240 >> 2] = (U32)((gubExpandCE) ? ((ENABLE_SANDISK_1ZNM_SPF) ? (gubMTQTotalPhyCE * 2 * gubSectorsPerPlane) : (guwSectorsPerPage / gubExpandCE_Ratio)) : guwSectorsPerPage);

	ubTemp_BUF[244] = gubFlashMode;
	ubTemp_BUF[245] = gubFlashClock;
	ubTemp_BUF[246] = 0;//SDR Read Coarse
	ubTemp_BUF[247] = gubIsWriteProtect;
	uwTemp_BUF[248 >> 1] = VT->guwFWUpdateCount;
	uwTemp_BUF[250 >> 1] = VT->guwSystemUpdateCount;

	ulTemp_BUF[252 >> 2] = gulErrorInfo; //gulErrorInfo
	uwTemp_BUF[256 >> 1] = guwErrorID; //guwErrorID


	//-------------------------------------------------//
	// 260-263
	ulTemp_BUF[260 >> 2] = AtaCfg.ubHpaState;
	// 264-267
	ulTemp_BUF[264 >> 2] = AtaCfg.ulHpaSize;
	// 268-299
	for (ubi = 0; ubi < 16; ubi++) {
		uwTemp_BUF[(268 >> 1) + ubi] = AtaCfg.uwHpaPW[ubi];
	}

	// 300-303
	ulTemp_BUF[300 >> 2] = AtaCfg.uwDCOState;
	// 304-307
	ulTemp_BUF[304 >> 2] = AtaCfg.ulDcoSize;
	// 308-311
	ulTemp_BUF[308 >> 2] = gulParamPageInfo.All;

	if (BURNER) {
		// 312-315
		U32 ulSDRSize = ((SYS1L[SYS1L_DZ2_DSA] + (SYS1L[SYS1L_DZ2_DLEN] / 36 * 32) ) >> 12 << 12)  - ((1 + ((SYS1L[SYS1L_DZ2_DSA] + 0x80000) >> 12)) << 12); //ICDOE size=0x80000
		ulTemp_BUF[312 >> 2] = ulSDRSize / 512; //for SDR_RW_DMA, sector number of Z2 when ECC on
	}

	///Lin   316~319,4byte給bad block log ,320~323  4byte給temperature log
	//BUFB_BASE[316] = badblocklog_pages;

	U32 ulCEValid_0_to_31, ulCEValid_32_to_63;
	ulCEValid_0_to_31 = 0;
	ulCEValid_32_to_63 = 0;
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
		UBYTE ubj;
		for (ubj = 0; ubj < MAX_CHANNEL ; ubj++) {
			for (ubi = 0; ubi < MAX_CE_PER_CH; ubi++) {
				if (gubCH_CEMappingToPCE[ubj][ubi] != 0xff) {
					if (ubj == 0) {
						ulCEValid_0_to_31 |= (BIT0 << (gubCH_CEMappingToPCE[ubj][ubi]));
						//UartString("\n CEValid 0~31: ");
						//UartLongHex(ulCEValid_0_to_31);
					}
					else {
						if (gubUFSRule) {
							ulCEValid_0_to_31 |= (BIT0 << ((gubCH_CEMappingToPCE[ubj][ubi]) + MAX_CE_PER_CH));
						}
						else {
							ulCEValid_32_to_63 |= (BIT0 << (gubCH_CEMappingToPCE[ubj][ubi]));
						}
						//UartString("\n CEValid 32~63: ");
						//UartLongHex(ulCEValid_32_to_63);
					}
				}
			}
		}
	}
	else {
		for (ubi = 0; ubi < (MAX_CE_PER_CH * MAX_CHANNEL); ubi++) {
			//[cppcheck][Either the condition 'ubi>31' is redundant or the array 'gubReMappingCE[16]' is accessed at index 32, which is out of bounds.]
#if 0
			if (ubi > 31) {
				if (gubReMappingCE[ubi] != 0x3F) {
					ulCEValid_32_to_63 |= (BIT0 << (ubi - 32));
				}
			}
			else
#endif
			{
				if (gubReMappingCE[ubi] != 0x3F) {
					ulCEValid_0_to_31 |= (BIT0 << ubi);
				}
			}
		}
	}

	//328-331
	ulTemp_BUF[328 >> 2] = ulCEValid_0_to_31;
	//332-335
	ulTemp_BUF[332 >> 2] = ulCEValid_32_to_63;

#if (RDT_BURNER)
	guwRDTErrorLogPageIndex = 0;

	if (gulRDTErrorLogPageCount == UNINITIALIZED_VALUE) {
		/*
		if (gulRDTSDRLogUnit == UNINITIALIZED_VALUE) {
		    FindRDTLogBlock(BUFFER3_BASE, MARK_SDR_LOG, &gulRDTSDRLogUnit, &gulRDTSDRLogPB);
		}
		if (gulRDTSDRLogUnit != UNINITIALIZED_VALUE) { // Read valid page
		    ReadRDTLogBlock(BUFFER3_BASE, gulRDTSDRLogUnit, gulRDTSDRLogPB, guwFastPagePagesPerUnit - 1);
		    gulRDTErrorLogPageCount = *(UBYTE *)BUFFER3_BASE;
		}
		*/
	}
	//gulRDTErrorLogPageCount = (gulRDTErrorLogPageCount < MAX_ERR_LOG_PAGE_COUNT) ? gulRDTErrorLogPageCount : MAX_ERR_LOG_PAGE_COUNT;

	ubTemp_BUF[316] = (gulRDTErrorLogPageCount < MAX_ERR_LOG_PAGE_COUNT) ? gulRDTErrorLogPageCount : MAX_ERR_LOG_PAGE_COUNT;
	ubTemp_BUF[320] = 0xFF;    ///temperature log 長度  暫時給為0xff 通常都是1
	ubTemp_BUF[321] = 0x1;    ///temperature log 長度  暫時給為0xff 通常都是1
	ubTemp_BUF[324] = 0xFF;
#endif

#if(RDT)
	ubTemp_BUF[316] = 0;
	ubTemp_BUF[317] = 0;
	ubTemp_BUF[318] = 0;
	ubTemp_BUF[319] = 0;
	ubTemp_BUF[320] = 0;
	ubTemp_BUF[321] = 0;
	ubTemp_BUF[322] = 0;
	ubTemp_BUF[323] = 0;
	ubTemp_BUF[324] = 0;
	ubTemp_BUF[325] = 0;
	ubTemp_BUF[326] = 0;
	ubTemp_BUF[327] = 0;
	///以上為清空/////////////
	ubTemp_BUF[316] = badblocklog_pages;
	ubTemp_BUF[320] = 0x67;    ///temperature log 長度  暫時給為0x67 通常都是1
	ubTemp_BUF[324] = SDRlog_pages;
	ubTemp_BUF[365] = gubBasiCMDLoopNumber;                 //normal loop
	ubTemp_BUF[366] = gubFlaLoopTimes - gubBasiCMDLoopNumber;                  //a2 loop
#endif
	///Lin
	if (BURNER) {
		uwTemp_BUF[344 >> 1] = (('S' << 8) | 'B');
		uwTemp_BUF[346 >> 1] = (('B' << 8) | FW_REVISION1);
		uwTemp_BUF[348 >> 1] = (FW_REVISION2 << 8) | FW_REVISION3;
		uwTemp_BUF[350 >> 1] = (FW_REVISION4 << 8) | FW_REVISION5;
	}
	else {
		if (RDT) {
			uwTemp_BUF[344 >> 1] = (('R' << 8) | 'D');
			uwTemp_BUF[346 >> 1] = (('T' << 8) | FW_REVISION1);
			uwTemp_BUF[348 >> 1] = (FW_REVISION2 << 8) | FW_REVISION3;
			uwTemp_BUF[350 >> 1] = (FW_REVISION4 << 8) | FW_REVISION5;
		}
		else {
			uwTemp_BUF[344 >> 1] = (('S' << 8) | 'B');
			uwTemp_BUF[346 >> 1] = (('F' << 8) | FW_REVISION1);
			uwTemp_BUF[348 >> 1] = (FW_REVISION2 << 8) | FW_REVISION3;
			uwTemp_BUF[350 >> 1] = (FW_REVISION4 << 8) | FW_REVISION5;
		}
	}

	ubTemp_BUF[352] = sFWVerStr[7];
	ubTemp_BUF[353] = sFWVerStr[8];
	ubTemp_BUF[354] = sFWVerStr[9];
	ubTemp_BUF[355] = sFWVerStr[10];
	ubTemp_BUF[356] = sFWVerStr[0];
	ubTemp_BUF[357] = sFWVerStr[1];
	ubTemp_BUF[358] = sFWVerStr[2];
	ubTemp_BUF[359] = sFWVerStr[4];
	ubTemp_BUF[360] = sFWVerStr[5];

	ubTemp_BUF[361] = gubDensitysize;
	ubTemp_BUF[362] = ENABLE_VenderCMD_return_PhysicalCE | BIT1;
	ubTemp_BUF[363] = gubIcdeDone;

	ubTemp_BUF[364] = gubFlashODT;
	ubTemp_BUF[365] = gubFlashDrive;
	ubTemp_BUF[366] = gODTSetting.ubODTEnable;
	ubTemp_BUF[367] = gODTSetting.ubODT;

	ubTemp_BUF[368] = gDrivingSetting.ubDrivingEnable;
	ubTemp_BUF[369] = gDrivingSetting.ubALEPads;
	ubTemp_BUF[370] = gDrivingSetting.ubCLEPads;
	ubTemp_BUF[371] = gDrivingSetting.ubnWEPads;
	ubTemp_BUF[372] = gDrivingSetting.ubnREPads;
	ubTemp_BUF[373] = gDrivingSetting.ubFRDYPads;
	ubTemp_BUF[374] = gDrivingSetting.ubnWPPads;
	ubTemp_BUF[375] = gDrivingSetting.ubDQSPads;
	ubTemp_BUF[376] = gDrivingSetting.ubDataPads;
	ubTemp_BUF[377] = gDrivingSetting.ubnCEPads;

#if(!BURNER)
	UWORD uwFailLogCount, uwEntry;
	UBYTE ubCE;
	ULLONG uoVTLogFail = 0;
	for (uwFailLogCount = 0; uwFailLogCount < VTLogEntryNum; uwFailLogCount++) {
		if (VT->guoFailLog[uwFailLogCount].FLHLog.ubLogType == FLHErrLogType) {
			if ((VT->guoFailLog[uwFailLogCount].FLHLog.ubAbnormalLoc < 10) && (VT->guoFailLog[uwFailLogCount].FLHLog.ubAbnormalLoc > 0)) {
				uwEntry = (VT->guoFailLog[uwFailLogCount].FLHLog.uwFEntry % (gubPlanesPerPage * gub4kEntrysPerPlane));
				ubCE = (UBYTE)(uwEntry >> gubBurstsPerBankLog);

				if (ENABLE_VenderCMD_return_PhysicalCE) {
					ubCE = TranslateLogicalCEtoPhysicalCE(ubCE);
				}
				uoVTLogFail |= (BIT0 << ubCE);
			}
		}
	}
	ulTemp_BUF[(380 >> 2)] = (U32)uoVTLogFail;
	ulTemp_BUF[(384 >> 2)] = (U32)(uoVTLogFail >> 32);
#endif

	ulTemp_BUF[(388 >> 2)] = gulMaxPECycle_D3;
	ulTemp_BUF[(392 >> 2)] = gulMaxPECycle_D1;

#if(!BURNER)
	//decoding flow version  http://192.168.1.213/svn/Technical_Document/hw_design_spec/DecodingFlow/Decoding_Flow/Finalized_Project/
#if MicronFlashOnly
#if B0KB
	ulTemp_BUF[404 >> 2] = 19; //Decoding flow SN number 404-407
#else
	//L06B
	ulTemp_BUF[404 >> 2] = 16; //Decoding flow SN number 404-407
#endif
#elif YMTC
	ulTemp_BUF[404 >> 2] = 20000041; //Decoding flow SN number 404-407
#elif Hynix
	if (guoFlashUseType.B.btHynix3DV5) {
		ulTemp_BUF[404 >> 2] = 20000037; //Decoding flow SN number 404-407
	}
#elif (TLC_BICS2)
	if (guoFlashUseType.B.btSandisk) {
		if (guoFlashUseType.B.btToshibaBiCs4) {
			if (gubDensitysize == FLH_Size_512Gb) {
				ulTemp_BUF[404 >> 2] = 121; //Decoding flow SN number 404-407
			}
			else {
				ulTemp_BUF[404 >> 2] = 120; //Decoding flow SN number 404-407
			}
		}
		else {
			ulTemp_BUF[404 >> 2] = 72; //Decoding flow SN number 404-407
		}
	}
	else if (guoFlashUseType.B.btToshibaBiCs4) {
		if (gubDensitysize == FLH_Size_512Gb) {
			ulTemp_BUF[404 >> 2] = 193; //Decoding flow SN number 404-407
		}
		else {
			ulTemp_BUF[404 >> 2] = ((gRRTMode.B.btNoRevision) ? 191 : ((gRRTMode.B.btRevision) ? 192 : 190)); //Decoding flow SN number 404-407
		}
	}
	else if (guoFlashUseType.B.btToshibaBiCs2) {
		ulTemp_BUF[404 >> 2] = 20000045; //Decoding flow SN number 404-407
	}
	else {
		ulTemp_BUF[404 >> 2] = 194; //Decoding flow SN number 404-407
	}
#elif (TLC)
	if (guoFlashUseType.B.btToshiba) {
		ulTemp_BUF[404 >> 2] = 18; //Decoding flow SN number 404-407
	}
#elif (ENABLE_ULTRAMLC)
	if (guoFlashUseType.B.btToshiba) {
		ulTemp_BUF[404 >> 2] = 65; //Decoding flow SN number 404-407
	}
#else
	if (guoFlashUseType.B.btSandisk) {
		ulTemp_BUF[404 >> 2] = 30000001; //Decoding flow SN number 404-407
	}
	else if (guoFlashUseType.B.btToshiba) {
		ulTemp_BUF[404 >> 2] = 30000000; //Decoding flow SN number 404-407
	}
#endif

#if TLC_BICS2
	//RRT version  http://192.168.1.213/svn/Technical_Document/fw_design_guideline/RR_Value_Library/Summary_Table/
	ubTemp_BUF[408] = (guoFlashUseType.B.btSandisk) ? 0x2 : 0x1 ; //NAND
	ubTemp_BUF[409] = (guoFlashUseType.B.btToshibaBiCs4) ? 0x4 : ((guoFlashUseType.B.btToshibaBiCs3) ? 0x3 : 0x2); //Design Rule
	ubTemp_BUF[410] = 0x2; //Mode TLC
	ubTemp_BUF[411] = gubDensitysize - 3; //Die    128Gb:1, 256Gb:2, 512Gb:3
	ubTemp_BUF[412] = (gRRTMode.B.ubType) ? gRRTMode.B.ubType : 0x1; //NAND_Type
	if (gRRTMode.B.btMPSet) {
		ubTemp_BUF[413] = gubRRTVersion;
	}
	else if (guoFlashUseType.B.btToshibaBiCs4) {
		if (guoFlashUseType.B.btSandisk) {
			ubTemp_BUF[413] = 0x41; //RRT Verison
		}
		else {
			ubTemp_BUF[413] = (gubDensitysize == FLH_Size_512Gb) ? 0x6 : ((gRRTMode.B.btNoRevision) ? 0x6 : ((gRRTMode.B.btRevision) ? 0x4 : 0x3)); //RRT Verison
		}
	}
	else if (guoFlashUseType.B.btToshibaBiCs2) {
		ubTemp_BUF[413] = 0x52; //RRT Verison
	}
	else if (guoFlashUseType.B.btSandisk && (gubDensitysize == FLH_Size_512Gb && guoFlashUseType.B.btToshibaBiCs3)) {
		ubTemp_BUF[413] = 0x51; //RRT Verison
	}
	else if (guoFlashUseType.B.btSandisk && (gubDensitysize == FLH_Size_256Gb) && guoFlashUseType.B.btToshibaBiCs3) {
		ubTemp_BUF[413] = 0x3; //RRT Verison
	}
	else {
		ubTemp_BUF[413] = ((gRRTMode.B.ubType == 3) && (gubDensitysize == FLH_Size_256Gb)) ? 0x3 : 0x2; //RRT Verison
	}
#elif (TLC)
	if (guoFlashUseType.B.btToshiba) {
		ubTemp_BUF[408] = 0x1 ; //NAND
		ubTemp_BUF[409] = 0x1; //Design Rule
		ubTemp_BUF[410] = 0x2; //Mode TLC
		ubTemp_BUF[411] = 0x1; //Die    128Gb:1, 256Gb:2, 512Gb:3
		ubTemp_BUF[412] = 0x1; //NAND_Type
		ubTemp_BUF[413] = 0x51; //RRT Verison
	}
#else
	if (guoFlashUseType.B.btSandisk) {
		ubTemp_BUF[408] = 0x2 ; //NAND
		ubTemp_BUF[409] = 0x1; //Design Rule
		ubTemp_BUF[410] = 0x1; //Mode TLC
		ubTemp_BUF[411] = 0x1; //Die    128Gb:1, 256Gb:2, 512Gb:3
		ubTemp_BUF[412] = 0x1; //NAND_Type
		ubTemp_BUF[413] = 0x51; //RRT Verison
	}
	else if (guoFlashUseType.B.btToshiba) {
		ubTemp_BUF[408] = 0x1 ; //NAND
		ubTemp_BUF[409] = 0; //Design Rule
		ubTemp_BUF[410] = 0; //Mode TLC
		ubTemp_BUF[411] = 0; //Die    128Gb:1, 256Gb:2, 512Gb:3
		ubTemp_BUF[412] = 0; //NAND_Type
		ubTemp_BUF[413] = 0x3; //RRT Verison
	}
#endif
#endif
	ubTemp_BUF[414] = gubHVFlash_FWSetting;//Bit_0=1:勾HV Flash, Bit_1=1:Translate IB to DBT
	ubTemp_BUF[415] = gubEraseMode_FWSetting;
	ubTemp_BUF[416] = gubHVFlash_RDTSetting;//Bit_0=1:勾HV Flash, Bit_1=1:Translate IB to DBT
	ubTemp_BUF[417] = gubEraseMode_RDTSetting;

	uwTemp_BUF[418 >> 1] = (UWORD)gulNandPhysicalPageSize;

	ubTemp_BUF[421] |= (gubFlashTypeFail & (0x3));
	ubTemp_BUF[422] = Scan_All_BadBlk_Command_Support;

	ubTemp_BUF[423] |= EnableProductHistory * BIT0;
	ubTemp_BUF[423] |= ENABLE_OUTPUT_TRIMMING_TABLE * BIT1;
	ubTemp_BUF[423] |= ((guoFlashUseType.B.btToshibaBiCs4 || guoFlashUseType.B.btToshibaBiCs3) ? ENABLE_MP_SET_RETRY_TABLE : 0) * BIT4;
	ubTemp_BUF[423] |= ENABLE_FAKE_TEMPTURE * BIT5;
	ubTemp_BUF[423] |= ENABLE_BICS3_SELECT_RETRY_TABLE * BIT6;
	ubTemp_BUF[423] |= ENABLE_MP_SET_RETRY_TABLE * BIT7;

	
#if (UFS && TLC_BICS2)
	ubTemp_BUF[427] = 1; //UFS Bics3 support
#endif
#if TLC_BICS2
	uwTemp_BUF[(428 >> 1)] = guwCE_FailMap2;
#endif
	uwTemp_BUF[(430 >> 1)] = guwSystemBlockRemappingTable[0][0];//BYTE 430開始
	uwTemp_BUF[(432 >> 1)] = guwSystemBlockRemappingTable[0][1];
	uwTemp_BUF[(434 >> 1)] = guwSystemBlockRemappingTable[1][0];
	uwTemp_BUF[(436 >> 1)] = guwSystemBlockRemappingTable[1][1];
	uwTemp_BUF[(438 >> 1)] = guwSystemBlockRemappingTable[2][0];
	uwTemp_BUF[(440 >> 1)] = guwSystemBlockRemappingTable[2][1];
	uwTemp_BUF[(442 >> 1)] = guwSystemBlockRemappingTable[3][0];
	uwTemp_BUF[(444 >> 1)] = guwSystemBlockRemappingTable[3][1];
	uwTemp_BUF[(446 >> 1)] = guwSystemBlockRemappingTable[4][0];
	uwTemp_BUF[(448 >> 1)] = guwSystemBlockRemappingTable[4][1];
	uwTemp_BUF[(450 >> 1)] = guwSystemBlockRemappingTable[5][0];
	uwTemp_BUF[(452 >> 1)] = guwSystemBlockRemappingTable[5][1];
	uwTemp_BUF[(454 >> 1)] = guwSystemBlockRemappingTable[6][0];
	uwTemp_BUF[(456 >> 1)] = guwSystemBlockRemappingTable[6][1];
	uwTemp_BUF[(458 >> 1)] = guwSystemBlockRemappingTable[7][0];
	uwTemp_BUF[(460 >> 1)] = guwSystemBlockRemappingTable[7][1];

	uwTemp_BUF[462 >> 1] = (UWORD)(guoCE_FailMap >> 32);
	uwTemp_BUF[464 >> 1] = (UWORD)(guoCE_FailMap >> 48);

	uwTemp_BUF[466 >> 1] = (UWORD)(guw_SDR_window_min);
	uwTemp_BUF[468 >> 1] = (UWORD)(guw_SDR_window_max);
	ulTemp_BUF[472 >> 2] = (U32)(gulMaxReadCount); //Byte 472~475 Read Count;
	//VDC_check
	/*
	BIT6  用來看是3.3V還是2.5V: 0:3.3V; 1:2.5V
	BIT22 用來power看夠不夠: 0:power不夠; 1:power夠
	如果是2.5V(BIT6 = 1)直接判FAIL
	如果是3.3V(BIT6 = 0)還要再判POWER夠(BIT22 = 1)才能PASS
	如果是3.3V(BIT6 = 0)但是POWER不夠(BIT22 = 0)還是判FAIL
	*/
	if (SYS0L[SYS0L_VDT_CTRL] & CR_VDT_FLH_CFG) { //bit6 = 1 -> 2.5 V 直接fail
		ubTemp_BUF[476] = 0xF1;
	}
	else { //bit6 = 3.3V 再判bit22
		if (SYS0L[SYS0L_VDT_CTRL] & XVDT_FLH_PG) { //bit22  = 1 ->enough ->pass
			ubTemp_BUF[476] = 0xF3;
		}
		else { //bit22 = 0 ->not enough -> fail
			ubTemp_BUF[476] = 0xF2;;
		}
	}


	ulTemp_BUF[484 >> 2] = (U32)guoFlashDefaultType.All[1];
	ulTemp_BUF[488 >> 2] = (U32)(guoFlashDefaultType.All[1] >> 32);
	ulTemp_BUF[492 >> 2] = (U32)guoFlashUseType.All[1];
	ulTemp_BUF[496 >> 2] = (U32)(guoFlashUseType.All[1] >> 32);


#if (RDT_BURNER)
	//ubTemp_BUF[367] = 0; // SDR log 8 byte

	// RDT read retry info
	//UWORD uwTempIndex = 0;
	//for (uwTempIndex = 0; uwTempIndex < 32; ++uwTempIndex) {
	//    uwTemp_BUF[(430 >> 1) + uwTempIndex] = 0;
	//}
#endif

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ubTemp_BUF, 512);
	}
	//     HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
	//     while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	Vender_TriggerHostData_31();
}

void Vender_Preformat(void)
{
	//	M_CheckRDTModeToDisableFunction();
	mDMAC_SV(1, (U32)&BUFB_BASE[0],  (U32)gubSectorsPerPlane * 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	Vender_TriggerHostData();
	//gubInfoValid = 0;
#if(!RDT)
	SYSTEM_GROUP_t *InfoArray = (SYSTEM_GROUP_t*)&BUFB_BASE[0];
#endif

	if (HB[HB_LBA_H] == 0x00) { //PreTest for RDT parameter
		//RDT_preformat_flag = 0x21;
	}
	else {
		if ((BUFB_BASE[0] == 'P') && (BUFB_BASE[1] == 'h') && (BUFB_BASE[2] == 'I') && (BUFB_BASE[3] == 's') && (BUFB_BASE[4] == 'O') && (BUFB_BASE[5] == 'n') && (HB[HB_LBA_H] == 0x02)) {
#if(!RDT)
			flaSetParameterBeforePreformat(InfoArray);
#endif
#if(!RDT)
			gubReadHBRetry_Enable = 0;
			if (InfoArray ->ubHBRetryEnable == 1) {
				gubReadHBRetry_Enable = 1;
			}
#endif
		}
		else {
			ftlPreformatShowError(0, 0, 0xFE);
			return;
		}
	}
	//	}
#if BURNER
	if (HB[HB_LBA_L]&BIT0) {
		gubPreformatunfinish = 1;
		gubPreformatTurbo = 1;
		gubNeedSendCmdComplete = 0;
		HB[HB_STATUS] = 0x50;
		HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
		while (HB[HB_REG_REQ]&H_REG_REQ);
	}
#endif
	gubPreformat = 1;
	M_SetFlagSettingNone();
#if RDT_BURNER
	if (ftlRDTPreformat() == 0)
#else
	if (ftlPreformat() == 0)
#endif
	{
		HB[HB_LBA_L] = 0;
		HB[HB_LBA_M] =  0;
		HB[HB_LBA_H] = 0;
		HB[HB_LBA_H_EXP] = 0;
		gubFlashInitialized = 1;
	}
	M_SetFlagSettingSHF();
#if BURNER
	if (gubPreformatunfinish) {
		gubPreformatunfinish = 0;
		while (gubPreformatTurbo);
	}
#endif
	gubPreformat = 0;
	//}
}

UBYTE Load_RUT_EC_Table(void)
{
	M_CheckRDTModeToDisableFunctionReturnValid(0);

	UBYTE ubCEIndex, ubPlaneBank;
	UWORD *uwCopyBuffer = (UWORD *)(&gubZ2Buffer[0]);
	UBYTE *ubCopyBuffer = (UBYTE *)(&gubZ2Buffer[0]);
	U32 *ulCopyBuffer = (U32 *)(&gubZ2Buffer[0]);
	SYSTEM_GROUP_t *InfoArray = (SYSTEM_GROUP_t *)(U32)(&gubZ2Buffer[M_SysInfo_InfoBlk]);
	UBYTE ubSystemExistFlag = 1;

	gubRUT_EC_set = 0;

	if (BURNER) {
		if (ftlCheckFlashFormat_SystemSet()) {
			ubSystemExistFlag = 0;
		}
	}

	if (ubSystemExistFlag) {
		if (ubCopyBuffer[M_SysInfo_gubTableVersion] != Table_version) {
			return 1;
		}
		memcpy((void *)&gubLastFWVer[0], (void*)&ubCopyBuffer[M_SysInfo_FW_Info1], 0x8);
		guwFWTotalUnitNum = uwCopyBuffer[(M_SysInfo_FWUnitNum / 2)];
		gulNativeSize = ulCopyBuffer[(M_SysInfo_NativeSize / 4)];

		if (BURNER) {
			flaSetParameterBeforePreformat(InfoArray);
		}

		gubFlashInitialized = 1;
		VT->guwVTAreaFreeTarget[0].B.uwTarget = uwCopyBuffer[(M_SysInfo_VTAreaTarget / 2) + 0];
		VT->guwVTAreaFreeTarget[1].B.uwTarget = uwCopyBuffer[(M_SysInfo_VTAreaTarget / 2) + 1];
		VT->guwVTAreaFreeTarget[2].B.uwTarget = uwCopyBuffer[(M_SysInfo_VTAreaTarget / 2) + 2];
		VT->guwVTAreaFreeTarget[3].B.uwTarget = uwCopyBuffer[(M_SysInfo_VTAreaTarget / 2) + 3];
		for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
				M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[0].B.uwTarget << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask))
				    = uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 0];
				M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[1].B.uwTarget << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask))
				    = uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 1];
				M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[2].B.uwTarget << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask))
				    = uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 2];
				M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[3].B.uwTarget << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask))
				    = uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 3];
			}
			else {
				ubCEIndex =  (ubPlaneBank & gubPlanesPerBurstMask) + ((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
				M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[0].B.uwTarget << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog)&gubBurstsPerBankMask))
				    = uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 0];
				M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[1].B.uwTarget << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog)&gubBurstsPerBankMask))
				    = uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 1];
				M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[2].B.uwTarget << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog)&gubBurstsPerBankMask))
				    = uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 2];
				M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[3].B.uwTarget << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog)&gubBurstsPerBankMask))
				    = uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 3];
			}
		}
	}
	else {
		return 1;
	}
	if (gubFlashInitialized) {
		FALLL[FCTLL_CNT_ONE] |= COUNT_MODE_EN;
		FALLL[FCTLL_CNT_ONE] &= (~COUNT_ONE_MODE);	// count 0;
		gubSSD_Init_State.B.ubState = BYTE_INIT_START_SCAN;
		//if (ftlInitFlash()) {
		//	gubFlashInitialized = 0;
		//}
		//FALLL[FCTLL_INT_INF] &= MAP_FIFO_POINTER_RESET;
	}
	return 0;

}

void Vender_Erase_All_Blocks(void)
{
	M_CheckRDTModeToDisableFunction();
	//HW[HW_DATA_TRG] |= H_DATA_TRG;
	//while (HW[HW_DATA_TRG]&H_DATA_TRG);
	gubPreformat = 1;
	M_SetFlagSettingNone();
	if (gubSSD_Init_State.B.ubState == BYTE_INIT_START_SCAN) {
		while (gubSSD_Init_State.B.ubState < BYTE_INIT_TABLE_DONE) {
			M_SwitchTask();
		}
	}
	if ((HB[HB_LBA_H] & MODE_RETAIN_RUT_EC) && ((gubRUT_EC_set & BIT0) == 0)) {
		ftlPreformatShowError(0, 0, gubRUT_EC_set );
		gubPreformat = 0;
		return;
	}


	if ((HB[HB_LBA_H] == 0x09) && (HB[HB_LBA_L] != 0x88)) {
		gubErrorCode = SET_ABRT;
		gubPreformat = 0;
		return;
	}

#if BURNER
	if (HB[HB_LBA_L]&BIT0) {
		gubPreformatunfinish = 1;
		gubPreformatTurbo = 1;
		gubNeedSendCmdComplete = 0;
		HB[HB_STATUS] = 0x50;
		HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
		while (HB[HB_REG_REQ]&H_REG_REQ);
	}
#endif
	HandleStopRW(1);
#if TLC
	/*
	   (!(HB[HB_LBA_H] & BIT7)) is for Debug feature.
	   Usage: Our demands is through Vender CMD feature case 0x26
	          and (HB[HB_LBA_H]&BIT4) to load RUT Table through Load_RUT_EC_Table()
	          but don't need to do ftlEraseAll(), so use BIT7 to determine.
	*/
	if (!(HB[HB_LBA_H] & MODE_DEBUG_FEATURE))
#endif
	{
		ftlEraseAll(HB[HB_LBA_H]);
	}
	gubStopFTL = 0;
	M_SetFlagSettingSHF();
#if BURNER
	if (gubPreformatunfinish) {
		gubPreformatunfinish = 0;
		while (gubPreformatTurbo);
	}
#endif
	gubPreformat = 0;
}

/*
void Vender_Reset_nonTxData(void)
{

}
*/

void Vender_Simulate_Power_Off(void)
{
	// =====gubSimulatePowerCycleMode=====
	// 1 : 在flush內做斷電
	// 2 : 不存VT 不做flush
	// 3 : 斷ftlCleanGRTable
	// 4 : 斷ftlCloseTarget
	// ==============================
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	if (!(ENABLE_POWER_CYCLE_DONT_CNT_EVENT)) {
		return;
	}

	if (gBQI.ubWLinkNum) {
		flaLEDBlink(0xEE);
	}
	gubPowerCycleSetting.B.btPowerCycleStopCmd = TRUE; //FTLTask進來這裡的都必須設這個FLAG Timer 已到並且等現有WBQ被清掉且mainTask不能繼續收Host cmd
	if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {
		UBYTE ubQindex;
		FlashQueue_t*CurrentFQ;
		BufferQueue_t* CurrentBQ;
		UartString("\n\rEnterPowerOff RstFQ:");
		Uart_Tx_DataHex(gFQI.ubFQLinkNumber);
		CurrentFQ = &gFQI.gFQLink[gFQI.ubFQLinkFirst];
		for (ubQindex = 0; ubQindex < gFQI.ubFQLinkNumber; ubQindex++) {
			if (ubQindex == 0) {
				UartString("\n\r");
			}
			Uart_Tx_DataHex(CurrentFQ->ubFJob);
			CurrentFQ = &gFQI.gFQLink[CurrentFQ->ubNext];
		}
		UartString("\n\rRstBQ:");
		Uart_Tx_DataHex(gBQI.ubLinkNum);
		CurrentBQ = &gBQI.BufferQueue[gBQI.ubLinkFirst];
		for (ubQindex = 0; ubQindex < gBQI.ubLinkNum; ubQindex++) {
			if (ubQindex == 0) {
				UartString("\n\r");
			}
			Uart_Tx_DataHex(CurrentBQ->btWrite);
			if (CurrentBQ->btWrite) {
				flaLEDBlink(0xEE);
			}
			CurrentBQ = &gBQI.BufferQueue[CurrentBQ->ubNext];
		}
	}
	if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {
		UartString("\n\rLast  W LBA:");
		UartLongHex(gulLastWCmdLBA);
		UartString(" Sector:");
		UartLongHex(guwLastWCmdSector);
		UartString("  R LBA:");
		UartLongHex(gulLastRCmdLBA);
		UartString(" Sector:");
		UartLongHex(guwLastRCmdSector);
	}
	//以下是讓HOST重新下comrest的動作
	APU_Reset();
	M_SetFlagSettingNone(); //避免timeOut, 無法從R/W回到main作fail handling
	AXIL[AXIL_SET_START_FLAG] = 0;
	AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //因為關掉autoSet/autoCheck, 可能w的FQ做完後對應的flag沒清
	AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;
	//SYSL[SYS0L_SIM_CTRL0] |= SIMREG_CMD_D2H;
	SYSL[SYS0L_SIM_CTRL0] &= ~(U32)SIMREG_CMD_D2H; //use as a tag for command D2H, but comreset D2H
	HW[HW_SYS_CTRL] &= ~H_REG_SATA_EN;
	HW[HW_SYS_CTRL] |= H_REG_SATA_EN;
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
	gubPowerCycleSetting.B.btEnableSetPowerCycle = TRUE;//進這個function代表已經要斷電，所以要把gubEnableSetPowerCycle設定為true讓下一輪起電的FW可以去設定power cycle
	PowerCycle->gulPowerCycleEventCount[PowerCycle->gubPowerCycleEvent] ++;
	PowerCycle->gubFormerPowerCycleEvent = PowerCycle->gubPowerCycleEvent;
	PowerCycle->gubFormerSimulatePowerCycleMode = gubSimulatePowerCycleMode;
	mDMAC_COPY((U32)VT, (U32)VTBackUp2, 8192, 0, 0, 0, 0, 1);
	if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {
		UartString("\n\rTimetoPC.");
		UartString("  Class:");
		UartWordHex(gubSimulatePowerCycleMode);
		if ((gubSimulatePowerCycleMode == 1) && (gubSavingEveryThingBeforeReset == 0)) {
			UartString("  All done,");
		}
		UartString("  LogPtr:");
		UartWordHex(PowerCycle->guwLogPtr);
		UartString("  Event:");
		if (PowerCycle->guwLogPtr == 0) {
			UartWordHex(PowerCycle->guwWriteFuncLog[POWER_CYCLE_LOG_ARRAY_BOUND]);
		}
		else {
			UartWordHex(PowerCycle->guwWriteFuncLog[PowerCycle->guwLogPtr - 1]);
		}
	}
	if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {//Rice
		if ((gubSimulatePowerCycleMode == 1) && (gubSavingEveryThingBeforeReset == 0)) {
			UartString("\n\rNCGRB:");
			if (VT->gulFTLState.B.btNeedCleanGR) {
				UartString("Y  C:");
			}
			else {
				UartString("N  C:");
			}
			UartLongHex(VT->gulNeedCleanGRL4KCount);
			U32 ulL2PGroupIndex;
			for (ulL2PGroupIndex = 0; ulL2PGroupIndex < gulTotalL2PGroupCnt; ulL2PGroupIndex++) {
				if (gulTable_L2P[ulL2PGroupIndex].B.ubGRSearchLinkState != GRSearchLinkLayer_Default_Value) {
					UartString("\n\rNotDefaultL2P:");
					UartLongHex(ulL2PGroupIndex);
				}
			}
			UWORD uwInRAMIndex;
			for (uwInRAMIndex = 0; uwInRAMIndex < guwWriteModeTableNum_ReadModeStartIndex; uwInRAMIndex++) {
				if (gulL2P_InRamInverse[uwInRAMIndex].B.btDirty) {
					UartString("\n\rDirtyL2P:");
					UartLongHex(gulL2P_InRamInverse[uwInRAMIndex].B.ulL2PGroupIndex);
				}
			}
		}
	}
	if ((gubSimulatePowerCycleMode > 0) && (gubSimulatePowerCycleMode != 2)) {
		if (((PowerCycle->guwFormerLogPtr + guwPowerCycleCount)&POWER_CYCLE_LOG_ARRAY_BOUND) != PowerCycle->guwLogPtr) {
			flaLEDBlink(0xEE);
		}
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)(PowerCycle) , sizeof(PowerCycleDebug_Info));
	}

	//以下是讓HOST重新下comrest的動作
	//APU_Reset();
	//M_SetFlagSettingNone(); //避免timeOut, 無法從R/W回到main作fail handling
	//AXIL[AXIL_SET_START_FLAG] = 0;
	//AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //因為關掉autoSet/autoCheck, 可能w的FQ做完後對應的flag沒清
	//AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;
	//SYSL[SYS0L_SIM_CTRL0] |= SIMREG_CMD_D2H;
	//SYSL[SYS0L_SIM_CTRL0] &= ~(U32)SIMREG_CMD_D2H; //use as a tag for command D2H, but comreset D2H
	//HW[HW_SYS_CTRL] &= ~H_REG_SATA_EN;
	//HW[HW_SYS_CTRL] |= H_REG_SATA_EN;

	flaSwitchClock(FLH_CLK_10MHz);
	if ( gubFlashMode == FLH_MODE_TOGGLE2) {
		flaSetToggleMode(INTERFACE_TOGGLE2To1, 0);
		gubFlashMode = FLH_MODE_TOGGLE1;
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
	}
	if ((gubFlashMode == FLH_MODE_TOGGLE1) && (((gubFlashID[5] & BIT7) == 0))) {
		flaSetToggleMode(INTERFACE_LEGACY, 0);
		gubFlashMode = INTERFACE_LEGACY;
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
	}

	UWORD uwWRBLK_temp = (HW[HW_AUTO_FIS_CTRL] & H_WR_BLK_EN);
	UBYTE  ubDDT_temp = (HB[HB_FLAG_CTRL] & H_DDT_EN);
	UWORD uwMSG_temp = (HW[HW_SYS_CTRL] & (H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS));
	HB[HB_FLAG_CTRL] |= ubDDT_temp;
	HW[HW_SYS_CTRL] |= uwMSG_temp;
	HW[HW_AUTO_FIS_CTRL] |= uwWRBLK_temp;
	dINTERRUPT(INT_EN1) &= (~RTT1_INT_1);   // Disenable RTT1
	dINTERRUPT(INT_STS1) |= RTT1_INT_1;
	SYS1L[SYS1L_RTT_CTRL] = 0;

	if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {
		UartString("  gubPowerCycleHandlingCmd:");
		Uart_Tx_DataHex(gubPowerCycleHandlingCmd);
		UartString("  cmdcode:");
		Uart_Tx_DataHex(gubCmdCode);
	}
	//Reset the CPU by just jumping to the _ResetVector
	_xtos_ints_off(0xFFFFFFFF);
	asm("j _ResetVector");
}

void Vender_Set_PowerCycle(UBYTE ubType) //ubType == 0 ->MainTask  ubType == 1 Vender_Reset
{
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();
	// =====gubSimulatePowerCycleMod=====
	// 1 : 在flush內做斷電
	// 2 : 不存VT 不做flush
	// 3 : 斷ftlCleanGRTable
	// 4 : 斷ftlCloseTarget
	// ==============================
	if (gubPowerCycleSetting.B.btPowerCycleCntStart == TRUE) {
		flaLEDBlink(0xEE);
	}
	gubPowerCycleSetting.B.btEnableSetPowerCycle = FALSE;
	if (getIntRandValue(10000) < 2000) {
		gubPowerCycleSetting.B.btEnableCntDuplicateEvent = 1;
	}
	else {
		gubPowerCycleSetting.B.btEnableCntDuplicateEvent = 0;
	}

	if (ubType == 0) {
		//以下隨機的方式是因為rand()好像都是太常出現偶數所以沒辦法rand()%2
		if (((VT->guwFreeBlockCount_D3 + gubMustFreeNum) <= (FreeBlockCountThatNeedCloseTarget + gubFailHandleRequestUnitNum)) || (VT->gulFTLState.B.btNeedCleanGR && ((VT->guwFreeBlockCount_D3 + gubMustFreeNum) <= (FreeBlockCountThatNeedCloseTarget + gubGC_FreeUnit_Threshold + gubFailHandleRequestUnitNum)))  || (gubPartialCloseTargetDoing && (gubCloseTargetCnt < gubPredictCloseTargetCountPerUnit)) || ((gubPartialCloseTargetDoing == 0) && VT->gulFTLState.B.btNeedCloseTarget)) {
			//if ((VT->guwFreeBlockCount_D3 + gubMustFreeNum) <= (FreeBlockCountThatNeedCloseTarget + gubGC_FreeUnit_Threshold + gubFailHandleRequestUnitNum)) { //(PowerCycle->guwMaxFreeUnit - VT->guwFreeBlockCount_D3) > 16) {//先測2跟3
			gubSimulatePowerCycleMode = (UBYTE)getIntRandValue(100);
			if (gubSimulatePowerCycleMode < 33) {
				gubSimulatePowerCycleMode = 2;
			}
			else if (gubSimulatePowerCycleMode < 66) {
				gubSimulatePowerCycleMode = 3;
			}
			else {
				gubSimulatePowerCycleMode = 4;//4;
			}
			//gubSimulatePowerCycleMode = 3;
		}
		else {
			gubSimulatePowerCycleMode = (UBYTE)getIntRandValue(100);
			if (gubSimulatePowerCycleMode < 50) {
				gubSimulatePowerCycleMode = 2;
			}
			else {
				gubSimulatePowerCycleMode = 3;
			}
			//gubSimulatePowerCycleMode = 3;
		}
	}
	else {
		gubSimulatePowerCycleMode = (UBYTE)getIntRandValue(100);
		if (gubSimulatePowerCycleMode < 50) {
			gubSimulatePowerCycleMode = 1;
		}
		else {
			gubSimulatePowerCycleMode = 2;
		}
	}

	if (VT->gulFTLState.B.btFlushCache) {
		VT->gulFTLState.B.btFlushCache = 0;
		ftlSaveVT(0, VT_SET_POWER_CYCLE_0x77);
	}


	if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {
		if (ubType == 0) {
			UartString("\n\rM_T");
		}
		else {
			UartString("\n\rV_R");
		}
		UartString("  PC Mode : ");
		Uart_Tx_DataHex(gubSimulatePowerCycleMode);

	}

	PowerCycle->gubPowerCycleEvent = 0;
	PowerCycle->guwFormerLogPtr = PowerCycle->guwLogPtr;
	if (gubSimulatePowerCycleMode == 1) { //Flush Cache
		if (gubPowerCycleSetting.B.btEnableCntDuplicateEvent) {
			guwPowerCycleCountLimit = (UWORD)getIntRandValue(0xFFFF) + 1; // (UWORD)getIntRandValue (L2P_IN_RAM_W_NUM - 1) + 1rand()
		}
		else {
			guwPowerCycleCountLimit = (UWORD)getIntRandValue(0xFF) + 1; // (UWORD)getIntRandValue (L2P_IN_RAM_W_NUM - 1) + 1rand()
		}
		guwPowerCycleCount = 0;
		if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {
			UartString("  Limit:");
			UartWordHex(guwPowerCycleCountLimit);
		}
	}
	else if (gubSimulatePowerCycleMode == 2) { //Directly Power Off
		if (gubKeepLoadAlignmentBQNum) {
			gBQI.ubLinkNum += 2;
			gBQI.ubWLinkNum++;
			gubKeepLoadAlignmentBQLink = 0xFF;
			gubKeepLoadAlignmentBQNum = 0;
		}
		while (gBQI.ubWLinkNum) { //確保現存的WBQ全部寫入Nand
			gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
			M_SwitchTask();
		}

		//gubPowerCycleCntStart不會被設定為TRUE
		PowerCycle->guwWriteFuncLog[PowerCycle->guwLogPtr] = POWER_CYCLE_LOG_DIRECT_POWER_OFF;
		if (PowerCycle->guwLogPtr != POWER_CYCLE_LOG_ARRAY_BOUND) {
			PowerCycle->guwLogPtr++;
		}
		else {
			PowerCycle->guwLogPtr = 0;
		}
		PowerCycle->gubPowerCycleEvent = POWER_CYCLE_LOG_DIRECT_POWER_OFF;
		Vender_Simulate_Power_Off();
	}
	else {
		if (gubSimulatePowerCycleMode == 3) {//GR
			if (gubPowerCycleSetting.B.btEnableCntDuplicateEvent) {
				guwPowerCycleCountLimit = (UWORD)getIntRandValue(0x3FFF) + 1; // (UWORD)getIntRandValue (L2P_IN_RAM_W_NUM - 1) + 1rand()
			}
			else {
				guwPowerCycleCountLimit = (UWORD)getIntRandValue(0x4F) + 1; // (UWORD)getIntRandValue (L2P_IN_RAM_W_NUM - 1) + 1rand()
			}
		}
		else {
			if (gubPowerCycleSetting.B.btEnableCntDuplicateEvent) {
				guwPowerCycleCountLimit = (UWORD)getIntRandValue(0x7FFF) + 1; // (UWORD)getIntRandValue (L2P_IN_RAM_W_NUM - 1) + 1rand()
			}
			else {
				guwPowerCycleCountLimit = (UWORD)getIntRandValue(0xFF) + 1; // (UWORD)getIntRandValue (L2P_IN_RAM_W_NUM - 1) + 1rand()
			}
		}
		guwPowerCycleCount = 0;
		if (ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST) {
			UartString("  Limit:");
			UartWordHex(guwPowerCycleCountLimit);
		}
	}
}


void Vender_Reset(UWORD uwWRBLK_temp, UBYTE ubDDT_temp, UWORD uwMSG_temp)
{
#if (!RDT)
#if (!BURNER)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	if (ENABLE_DEBUG_UART_VENDER) {
		UartString("\nV_Rst");
	}
	HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	gubFlushMode = BYTE_STANDBY_FLUSH;
	FlushCache();
	if (ENABLE_DEBUG_BACKUP_VT) {
		mDMAC_COPY((U32)VT, (U32)VTBackUp, 8192, 0, 0, 0, 0, 1);
	}
	SYSL[SYS0L_SIM_CTRL0] |= SIMREG_CMD_D2H; //use as a tag for command D2H, but comreset D2H
	flaSwitchClock(FLH_CLK_10MHz);
	M_FlashModeBackToDefault();


	if (ENABLE_DEBUG_UART_VENDER) {
		UartString("\nV_Rst_Done");
	}

	HB[HB_FLAG_CTRL] |= ubDDT_temp;
	HW[HW_SYS_CTRL] |= uwMSG_temp;
	HW[HW_AUTO_FIS_CTRL] |= uwWRBLK_temp;
	dINTERRUPT(INT_EN1) &= (~RTT1_INT_1);   // Disenable RTT1
	dINTERRUPT(INT_STS1) |= RTT1_INT_1;
	SYS1L[SYS1L_RTT_CTRL] = 0;
	//Reset the CPU by just jumping to the _ResetVector
	_xtos_ints_off(0xFFFFFFFF);
	asm("j _ResetVector");
#endif
#endif
}

/*
void Vender_Isp_Jump_nonTxData()
{

}
*/
void Micorn_FD_reset()
{

	UBYTE ubi;
	UBYTE ubDieIndex;
	UBYTE ubChannel;
	_UCV REG_t * pFlaReg;
	unsigned long ulFPUResult_78 = 0;
	U32 ulDelayLoop = 0;
	UBYTE ubCEPerChannel ;
#if ENABLE_ODD_CE
	/* Fix for odd CE */
	ubCEPerChannel = ((gubCENumber / gubPlanesPerBurst) + (gubCENumber % gubPlanesPerBurst));
#else
	ubCEPerChannel = (gubCENumber >> gubPlanesPerBurstLog);
#endif
	if (gubFlashID[0] == 0xb5) {
		for (ubDieIndex = 0; ubDieIndex < gubDieNumber; ubDieIndex++) {

			for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {

				pFlaReg = (volatile REG_t *)gulFlashIP_RegBase[ubChannel];

				for (ubi = 0; ubi < ubCEPerChannel; ubi++) {
					FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
					FCONL[FCONL_FCE_ENB] = (BIT0 << (ubi + ubChannel * MAX_CE_PER_CH));
					pFlaReg[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					pFlaReg[FCTLL_PIO_CMD].L = 0x78;

					pFlaReg[FCTLL_PIO_ADR].B.B0 = (UBYTE)(0x00); //R1
					pFlaReg[FCTLL_PIO_ADR].B.B0 = (UBYTE)(0x00); //R2
					pFlaReg[FCTLL_PIO_ADR].B.B0 = (UBYTE)((ubDieIndex << 6) & 0xFF); //R3

					ulFPUResult_78 = pFlaReg[FCTLL_PIO_DAT].B.B0;
					ulDelayLoop = 0;
					while ((ulFPUResult_78 & (0xE0)) != 0xE0) {
						ulFPUResult_78 = pFlaReg[FCTLL_PIO_DAT].B.B0;
						ulDelayLoop++;
						if (ulDelayLoop > 40000) {
							UartString("\r\n Timeout");
							break;
						}
					}
					pFlaReg[FCTLL_PIO_CMD].L = 0xFD;//Hard Reset
					pFlaReg[FCTLL_PIO_CMD].L = 0x70;

					//wait RB goes high, because some of CE may switch to ONFI successiful, and when busy during tITC, all command is prohibit including status command.
					while ( (FCTLL[FCTLL_RBY_INF] & FLH_CE_RBY_AND) == 0) {};
					pFlaReg[FCTLL_PIO_CMD].L = 0x00;//Reset to Noraml Data output
				}
			}
		}
		FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
		/* Turn on Micron randomizer */
		flaMicronRandomizer(1);
	}
}
void ISP_JUMP(UBYTE ubmode)
{
	if (ASIC) {
		while ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0); //Philip said ASIC need not poll 8 times
	}
	else {
		UBYTE ubCheckDDR_Idle = 8;
		while (ubCheckDDR_Idle) {
			// 元仔跟rice說 : 讀八次比較保險
			if ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0) {
				ubCheckDDR_Idle = 8;
			}
			ubCheckDDR_Idle --;
		}
	}


	dINTERRUPT(INT_EN1) &= (~RTT1_INT_1);   // Disenable RTT1
	dINTERRUPT(INT_STS1) |= RTT1_INT_1;
	SYS1L[SYS1L_RTT_CTRL] = 0;

	if (ubmode == 0) {
#if Hynix
		FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
		SYS1L[SYS1L_ECC_EN] = 0; //disable DDR ECC, E3D, shadow Z1Z2
		mDMAC_SV(1, Z1_DDR_BASE, Z1_Size, 0, 0, 0, DMA_EN_SETCQ, 0);
		mDMAC_SV(1, Z2_DDR_BASE, Z2_Size, 0, 0, 0, DMA_EN_SETCQ, 0);
		mDMAC_SV(1, Z3_DDR_BASE, Z3_Size, 0, 0, 0, DMA_EN_SETCQ, 0);
	}
	else {
	}



	if (ubmode == 0) {
		if (!BURNER) {
			HW[HW_SYS_CTRL] &= (~H_BLK_FIS_EN);
			xthal_set_region_attribute((void*)DDR_BASE, 512 * 1024 * 1024, XCHAL_CA_BYPASS, XTHAL_CAFLAG_EXACT | XTHAL_CAFLAG_NO_PARTIAL) ;
		}
	}
	else {
	}


	if (SYS1B[SYS1B_SR_HW_VER] > 1) {
	}
	else {
		if (SYS0B[0x44] == 0x2) {
			HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);//force bootcode狀況下bootcode會打錯的D2H
		}
	}
	HB[HB_STATUS] = 0x50;
	SYS0L[SYS0L_MODE_EN] ^= MD_PRAM_EN;

	while (1);
}

void Vender_Isp_Jump(UBYTE ubMode)
{
	//if (gubPreformat != 1) {
	//	gubFlushMode = BYTE_STANDBY_FLUSH;
	//	FlushCache();
	//}
	if (ubMode == 2) {

	}
	else {
		if ((ubMode != 0xFF) && (ubMode != 0xFE)) {
			HandleStopRW(0);
		}
		else if (ubMode == 0xFF) {
			SYS0L[SYS0L_SIM_CTRL1] |= SIMREG_VENDER_FORCE_BOOT_CODE;
		}
	}
	M_SetFlagSettingNone();

	if (ubMode == 2) {
	}
	else {
		flaSwitchClock(FLH_CLK_10MHz);
		M_FlashModeBackToDefault();
	}
	if (gubFlashID[0] == 0xb5) {
		M_FlashModeBackToDefault();
		Micorn_FD_reset();
	}
	SYS0B[0x200] &= ~(0x03);
#if UFS
	SYS0B[0x200] |= 0x02;
#endif
	*((volatile unsigned char*)0x04000420) = 0x0;
	if (ubMode == 2) {
		SYSL[SYS0L_RST_CTRL] &= (~(CR_RST_N_FLH));
		SYSL[SYS0L_RST_CTRL] |= (CR_RST_N_FLH);
		ISP_JUMP(1);
	}
	else {
		SYSL[SYS0L_RST_CTRL] &= (~(CR_RST_N_FLH));
		SYSL[SYS0L_RST_CTRL] |= (CR_RST_N_FLH);
		if (ubMode == 1) {
			HW[HW_SECTOR_CNT] = gulSectorCnt;
			HL[HL_BUF_TRG_SEC] = gulSectorCnt;
			//HL[HL_LAST_END_LBA] =0;
			HL[HL_TQ_CONTENT] = 0;
			Vender_HostRemainSectorCount();
		}
		while (HL[HL_AXI_FIS_STAT]&BIT23);
		HB[HB_FLAG_CTRL] |= (H_DDT_EN);
		HW[HW_SYS_CTRL] |= (H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS);

		ISP_JUMP(0); //因為bootcode要做DDR initial 需要disable ECC
	}

}

UWORD guwPageIndex;
void Vender_Isp_Verify(UBYTE ubMode, UBYTE ubCodeBlockNo)
{
	M_CheckRDTModeToDisableFunction();
	U32 ulAddressRegion = (FCONL[FCONL_FLH_FUNC] & RDM_REF_ROW_ADR_5_TO_0);
	UBYTE  ubCEIndex;
	UBYTE ubDepth, ubFQIndex, ubL4kLink, ubL4KNum = gub4kEntrysPerPlane;
	UBYTE *ubBUF_8Kshift = (UBYTE *)(BUFFER3_BASE + 0x2000);
	UBYTE *ubBUF_12Kshift = (UBYTE *)(BUFFER3_BASE + 0x3000);
	UBYTE *ubBUF_16Kshift = (UBYTE *)gulVenderSMARTBuffer;
	UWORD uwTransferCnt, uwHostTxSector, uwi, uwplane;
	UBYTE ubCodeUnit, ubCodeBurst;

	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;

#if !ENABLE_TLC_PROGRAM_CODEBLOCK2
	FCONL[FCONL_FLH_FUNC] |= RDM_REF_ROW_ADR_5_TO_0;
#endif

	//ulBurnerSeed = gulBurnerSeedOrigin;

	//if (gubSysCodeUnit[0] == 0xff) {
	//if (gubSysCodeSet[0] == 0xFF) {
	if (gubPlanesPerBurst > 1) { //2ch
		if (guwSysCodeBlock[0][0] == 0xFFFF && guwSysCodeBlock[0][1] == 0xFFFF) {
			gubErrorCode = 0x3;
			if (ENABLE_DEBUG_RW) {
				flaLEDBlink(0xcb);
			}
			ReturnErr();
			return;
		}
	}
	else {
		if (guwSysCodeBlock[0][0] == 0xFFFF) {
			gubErrorCode = 0x3;
			if (ENABLE_DEBUG_RW) {
				flaLEDBlink(0xcb);
			}
			ReturnErr();
			return;
		}
	}

#if Hynix
	FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif

	if (ubMode == 0) {              // Header , Common , Offset
		if (HB[HB_LBA_H] == 0) { // Step 1. Read Header
			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate((void *)BUFB_BASE, 512 * gulSectorCnt * 2);
			}
			guwPageIndex = 0;
			gulCurrentSeed = gulBurnerSeedOrigin;
			guloffset = 0;
			gubindex = 0;
			HandleStopRW(1);
			flaMicronRandomizer(1);
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
				FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);
				//ftlReadSinglePlane(gubSysCodeUnit[0], guwPageIndex, ubCEIndex, (U32)&BUFB_BASE[8 * 0x200 * ubCEIndex], HEADER_LCA);
				ubFQIndex = gFQI.ubFQLinkFree;
				FlashQueue_t *FQ;

				ubCodeUnit = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] >> gubBurstsPerBankLog);
				ubCodeBurst = (UBYTE)(guwSysCodeBlock[0][ubCEIndex] & gubBurstsPerBankMask);

				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ulRAMAddr = ((U32)&BUFB_BASE[8 * 0x200 * ubCEIndex]);
				FQ->btCheckEmpty = 1;
				FQ->btThisIsErasePage = 0;
				FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
				FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif
				FQ->uwFUnit = ubCodeUnit;
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((M_Multiplier((U32)guwPageIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = (((M_Multiplier((U32)guwPageIndex, DEF_PlanesPerPage_Mcl_Div))  + (ubCEIndex & gubPlanesPerBurstMask)) << gub4kEntrysPerPlaneLog);
				}
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
				UBYTE ubL4KCount;
				for (ubL4KCount = 0; ubL4KCount < 1; ubL4KCount++) {
					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
					//set L4K table info
					pL4KTable.ulL4K_LCA = 0x00000000;
					pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
					pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0;
					pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
					pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((((U32)&BUFB_BASE[8 * 0x200 * ubCEIndex]) + (ubL4KCount * 4096)) / 512);
					pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
					pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
					*pL4KTablePtr = pL4KTable;
				}
				gFQI.ubFQLinkNumber++;
				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

				// wait program done...
				while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}
				if (FQ->btThisIsErasePage) { //沒被寫過
					//ftlPreformatShowError(ubCEIndex, gubSysCodeUnit[0], 0x21);
				}
				gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * L4kQ_BIND_FACTOR * Spare4kSize));

				if ((pL4KTablePtr->ulL4K_LCA != HEADER_LCA) ) {
					//ftlPreformatShowError(ubCEIndex, gubSysCodeUnit[0], 0x24);
				}

#if (TLC || MicronFlashOnly)
				FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
#else
				FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE4);
#endif
			}
			gubStopFTL = 0;
			HB[HB_LBA_H] = 0xBB;
			if (gubPlanesPerBurst > 1) {
				for ( uwi = 0; uwi < (8 * 0x200 - 4); uwi++) {
					if (uwi == 0x28) {
						continue;
					}

					if (BUFB_BASE[uwi] != BUFB_BASE[uwi + (8 * 0x200)]) {

						if (ENABLE_DEBUG_RW) {
							flaLEDBlink(0xcd);
						}
						HB[HB_LBA_H] = 0xAA;
					}
				}
			}

			Vender_TriggerHostData();
			//gulSectorCnt -= 8;
			guwPageIndex += ENABLE_TLC_PROGRAM_CODEBLOCK2 ? 3 : 1;
			flaMicronRandomizer(0);

		}
		else {
#if(!BURNER)
			while ((VT->gulFTLState.B.btFlushCache == 0) && (VT->gulFTLState.B.btNeedCloseTarget || VT->gulFTLState.B.btNeedWearLeveling || VT->gulFTLState.B.btNeedCloseTableTarget || ((gubLockReadMode != LOCK_READ_MODE_NOBODY) && (gubLockReadMode != LOCK_READ_MODE_DLMC)))) {
				M_SwitchTask();
			}

			if (gubLockReadMode == LOCK_READ_MODE_DLMC) {
				gubErrorCode = SET_ABRT;
				return;
			}

			ftlSwapL2PTable(GC_LOCK_L2P_FOR_Vender_SMART, (BYTE_Lock_Read_Mode | LOCK_READ_MODE_Vender_SMART));
#endif
			UBYTE ub8KPtr, ub4KPtr;
			ubL4KNum = gub4kEntrysPerPlane;
			uwTransferCnt = gulSectorCnt / gubSectorsPerPlane;
			if (gulSectorCnt % gubSectorsPerPlane) {
				uwTransferCnt++;
			}
			uwHostTxSector = gulSectorCnt;
			HandleStopRW(1);
			flaMicronRandomizer(1);

			for (uwplane = 0; uwplane < uwTransferCnt; uwplane++) {
				for (ub8KPtr = 0; ub8KPtr < ubL4KNum; ub8KPtr += 2) {
					UBYTE ubstatus;
					ubCodeUnit = (UBYTE)(guwSysCodeBlock[0][0] >> gubBurstsPerBankLog);
					ubCodeBurst = (UBYTE)(guwSysCodeBlock[0][0] & gubBurstsPerBankMask);
#if ENABLE_TLC_PROGRAM_CODEBLOCK2
					ubstatus = ftlReadSinglePlane(ubCodeUnit, guwPageIndex, 0, (U32)&BUFB_BASE[0], CODE_LCA,  (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubCodeBurst);
#else
					ubstatus = ftlReadSinglePlane(ubCodeUnit, guwPageIndex, 0, (U32)&BUFB_BASE[0], CODE_LCA,  (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubCodeBurst);
#endif


					if (ubstatus == 1) {
						ftlPreformatShowError(0, ubCodeUnit, 0x21);
						SetInitialFailID(0x1, 0);
					}
					else if (ubstatus == 2) {
						ftlPreformatShowError(0, ubCodeUnit, 0x24);
						SetInitialFailID(0x5, 0);
					}

					if (ENABLE_BURNER_USE_RANDOMIZER_DECODE) {
						FlaFlashDeRandomize( (U32*)&BUFB_BASE[0], (U32*)&ubBUF_16Kshift[0], 4);
						mDMAC_COPY( (U32)&ubBUF_16Kshift[0], (U32)&BUFB_BASE[0], 16384, 0, 0, 0, 0, 1);
					}

					///================================
					if (ub8KPtr) {
						mDMAC_COPY((U32)((&BUFB_BASE[0]) + ub8KPtr * 0x1000), (U32)&BUFB_BASE[0], 0x2000, 0, 0, 0, 0, 1);
					}

					for (ub4KPtr = 0; ub4KPtr < 2; ub4KPtr ++) {
						if (uwHostTxSector > 8) {
							uwHostTxSector -= 8;
							HL[HL_BUF_TRG_SEC] = 8;
							//HW[HW_SECTOR_CNT] = gubSectorsPerPlane;
						}
						else {
							HL[HL_BUF_TRG_SEC] = uwHostTxSector;
							//HW[HW_SECTOR_CNT] = uwHostTxSector;
						}

						mDMAC_SV(1, (U32)&ubBUF_8Kshift[0], HL[HL_BUF_TRG_SEC] * 0x200, gulCurrentSeed, gulCurrentSeed, 0, DMA_EN_RAND | DMA_EN_SETCQ, 1);
						mDMAC_XOR((U32)&ubBUF_8Kshift[0], (U32)((&BUFB_BASE[0]) + ub4KPtr * 0x1000), (U32)&ubBUF_12Kshift[0], HL[HL_BUF_TRG_SEC] * 0x200, 1, 0);

						gulCurrentSeed += HL[HL_BUF_TRG_SEC];

						HL[HL_BUF_OFST] = 24;

						HL[HL_TQ_CONTENT_31] = 0;
						Vender_HostRemainSectorCount();
					}
				}
				guwPageIndex += ENABLE_TLC_PROGRAM_CODEBLOCK2 ? 3 : 1;
				if (BURNER) {
					//					UartString("	 pass  page:");
					//					UartWordHex(guwPageIndex);
					//					UartString("	   *\n");
				}
			}
			gubStopFTL = 0;
			flaMicronRandomizer(0);
			if (!BURNER) {
				ftlSwapL2PTable(guwLockReadNum, BYTE_Free_Read_Mode);
#if BICS2_GC_TO_D3
				ftlSwapL2PTable(guwLockWriteNum, BYTE_Free_Write_Mode);
#endif
			}
		}
	}
	FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
	FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;

#if Hynix
	FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif

}



void Vender_Isp_Prog()
{
	M_CheckRDTModeToDisableFunction();
	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	gubISPDoing = 1;

	if (FlaPrepareHeader_Code(1)) {
		BUFL_BASE[2] = 0xFFFFFFFF;
	}
	else {
		BUFL_BASE[2] = 0x55AA55AA;
	}

#if Hynix
	FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif

	gubISPDoing = 0;
}
void Vender_GetFlashID(unsigned char ubFlashDevice)
{
	//#if(!RDT)
	//	M_CheckRDTModeToDisableFunction();
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}

	//UBYTE ubChannal = ubFlashDevice%gubPlanesPerBurst;
	UBYTE ubChannel = ubFlashDevice / gubMAX_SCAN_CE_PER_CH;       //IL2 for different channel (S9 MaxCh: 4)
	UBYTE ubi, ubCE_temp;

	if (gubReMappingCE[ubFlashDevice] == 0x3F) {		//This ubFlashDevice was not initialized.
		mDMAC_SV(1, (U32)&BUFB_BASE[0], 512, 0, 0, 0, DMA_EN_SETCQ, 1);
		return;
	}

	if (gubUFSRule) {
		if ( (ubFlashDevice > 15) ) {
			mDMAC_SV(1, (U32)&BUFB_BASE[0], 512, 0, 0, 0, DMA_EN_SETCQ, 1);
			return;
		}
	}

	if (!gubGPIO_CE_Decoder) {
		if ( (ubFlashDevice > 15) ) {
			mDMAC_SV(1, (U32)&BUFB_BASE[0], 512, 0, 0, 0, DMA_EN_SETCQ, 1);
			return;
		}

		ubCE_temp = 0;
		for (ubi = 0; ubi < 16; ubi++) {
			if (gubReMappingCE[ubi] == ubFlashDevice) {
				FCONB[(FCONL_FCE_SEL_0 << 2) + ubi] = 0x3F;
				ubCE_temp = ubi;
				break;
			}
		}
		FCONB[(FCONL_FCE_SEL_0 << 2) + ubFlashDevice] = ubFlashDevice;
	}



	flaSwitchClock(FLH_CLK_10MHz);
#if (!(MicronFlashOnly || YMTC))
	flaSetPadIOType(2, SINGLE_END_IO_TYPE);
	FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
#endif

	if (ubFlashDevice < 32) {
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			if (gubUFSRule) {
				FCONL[FCONL_FCE_ENB]  = M_MapToCEDecoder((ubFlashDevice / MAX_CE_PER_CH) , (ubFlashDevice % MAX_CE_PER_CH));
			}
			else {
				FCONL[FCONL_FCE_ENB]  = M_MapToCEDecoder(0, ubFlashDevice);
			}
		}
		else {
			FCONL[FCONL_FCE_ENB]  = 0x00000001U << ubFlashDevice;//[cppcheck][ Shifting signed 32-bit value by 31 bits is undefined behaviour.]
			FCONL[FCONL_FCE_ENB_1]  = 0x00000000;
		}
	}
	else {
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(1, (ubFlashDevice % 32));
		}
		else {
			FCONL[FCONL_FCE_ENB]  = 0x00000000;
			FCONL[FCONL_FCE_ENB_1]  = 0x00000001U << (ubFlashDevice % 32);//[cppcheck][ Shifting signed 32-bit value by 31 bits is undefined behaviour.]
		}
	}

#if (Hynix || MicronFlashOnly || YMTC)
	if ((MicronFlashOnly || YMTC) && ( guoFlashUseType.B.btNVDDR || guoFlashUseType.B.btNVDDR2)) {
		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0xFC;
	}
	else if (Hynix && guoFlashUseType.B.btToggle) {
		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0xFC;
	}
	else {
		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0xFF;
	}
#else
	FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0xFF;
#endif
	FCTLL[FCTLL_HS_MODE + (ubChannel * 0x200 / 4)] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
	FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x70; //Status Read
	UBYTE ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];
	U32 ulDelayLoop = 0;
	while (ubRet != 0xE0) {
		FCTLL[FCTLL_HS_MODE + (ubChannel * 0x200 / 4)] |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		ubRet = (UBYTE)FCTLL[FCTLL_PIO_DAT + (ubChannel * 0x200 / 4)];
		ulDelayLoop++;
		//Uart_Printf("Read ID Poll Status:%l\r\n", ubRet);
		if (ulDelayLoop > 5000) {
			//Uart_Printf("CE %d doesn't exist!!\r\n",ubFlashCE);
			break;
		}
	}

	mDMAC_SV(1, (U32)&BUFB_BASE[0], 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
	}

	if (ubRet == 0xE0) {
		FCTLL[FCTLL_PIO_CMD + (ubChannel * 0x200 / 4)] = 0x90;
		FCTLL[FCTLL_PIO_ADR + (ubChannel * 0x200 / 4)] = 0x00;

#if Hynix_16nm
		UBYTE dat;
		for (ubi = 0; ubi < 8; ubi++) {
			for (gulTemp = 0; gulTemp < 0x100; gulTemp++);

			dat = FCTLB[(FCTLL_PIO_DAT * 4) + (ubChannel * 0x200)];

			if (((ubi == 1) || (ubi == 7)) && (dat == 0x5A)) {
				dat = 0x3A;
			}

			if ((ubi == 5) && (dat == 0x65)) {
				dat = 0x25;
			}

			BUFB_BASE[ubi] = dat;
		}
#else
		for (ubi = 0; ubi < 8; ubi++) {
			for (gulTemp = 0; gulTemp < 0x100; gulTemp++);
			BUFB_BASE[ubi] =  FCTLB[(FCTLL_PIO_DAT * 4) + (ubChannel * 0x200)];
		}
#endif
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}
	flaSwitchClock(gubCurrentFlashClock);
	if (guoFlashUseType.B.btToggle) {
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
	}
	else if (guoFlashUseType.B.btONFI) {
		FALLL[FCTLL_FLH_SET] |= FLA_ONFI_MODE;
	}
	if (gubFlashMode == FLH_MODE_TOGGLE2) {
		flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
	}
	if (!gubGPIO_CE_Decoder) {
		FCONB[(FCONL_FCE_SEL_0 << 2) + ubCE_temp] = gubReMappingCE[ubCE_temp];
		FCONB[(FCONL_FCE_SEL_0 << 2) + ubFlashDevice] = gubReMappingCE[ubFlashDevice];
	}
	//#endif
}

void Vender_ScanFlashID(void)
{
	//M_CheckRDTModeToDisableFunction();
	UBYTE ubi, ubj, ubFlashDeviceNum;
	UBYTE* BUFB_512BYTE = &BUFB_BASE[512];

	//ftlSramSet((ULONG)&BUFB_512BYTE[0],00,512*2);//first buffer
	mDMAC_SV(1, (U32)&BUFB_BASE[512], 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	ubFlashDeviceNum = (gubGPIO_CE_Decoder && (!MicronFlashOnly)) ? ( (gubUFSRule) ? 16 : 64) : 32;

	for (ubi = 0; ubi < ubFlashDeviceNum; ubi++) {
		Vender_GetFlashID(ubi);
		for (ubj = 0; ubj < 8; ubj++) {
			BUFB_512BYTE[((unsigned int)ubi * 8) + ubj] = BUFB_BASE[ubj];
		}
	}
	mDMAC_COPY((U32)&BUFB_BASE[512], (U32)&BUFB_BASE[0], 512, 0, 0, 0, 0, 1);

	if (gubAP_enable) {
		HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	}
}

void Vender_SetReadSRAMParameter(void)
{
	//  3105 是收到zero buffer ,參數抓完後  再設回0...
	//  3109沒有這東西 所以收在一般buff...
	//
	xthal_dcache_region_invalidate((void *)BUFB_BASE, 512 * gulSectorCnt);
	HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
}


void Vender_Read_SRAM(void)
{
	M_CheckRDTModeToDisableFunction();
	U32 ulAddressOffset = BUFL_BASE[0];
	U32 ulTempBufStart[2], ulTempBufEnd[2], ulTempBase, ulTemp;
	UWORD uwi;

	ulTempBase = HL[HL_BUF_BASE];
	ulTempBufStart[0] = HL[HL_BUF_R_SHF];
	ulTempBufStart[1] = HL[HL_BUF_W_SHF];
	ulTempBufEnd[0] = HL[HL_BUF_R_EP];
	ulTempBufEnd[1] = HL[HL_BUF_W_EP];

	HL[HL_BUF_BASE] = BUFFER3_BASE / 4096;
	HL[HL_BUF_R_EP] = BUFFER3_READ_END;
	HL[HL_BUF_W_EP] = BUFFER3_WRITE_END;
	HL[HL_BUF_R_SHF] = BUFFER3_READ_START;
	HL[HL_BUF_W_SHF] = BUFFER3_WRITE_START;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)ulAddressOffset, 512 * gulSectorCnt);
	}

	for (ulTemp = 0; ulTemp < gulSectorCnt; ulTemp++) {
		for (uwi = 0; uwi < 512; uwi++)
			BUFB_BASE[uwi] = RAMB_START[ulAddressOffset + uwi]; // always Buffer A Only
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv(BUFB_BASE, 512);
		}
		HL[HL_BUF_OFST] = 0;

		HL[HL_BUF_TRG_SEC] = 1;
		HB[HB_DATA_TRG] = H_DATA_TRG;
		while (HB[HB_DATA_TRG]&H_DATA_TRG);

		ulAddressOffset += 512;
	}

	HL[HL_BUF_BASE] = ulTempBase;
	HL[HL_BUF_R_SHF] = ulTempBufStart[0];
	HL[HL_BUF_W_SHF] = ulTempBufStart[1];
	HL[HL_BUF_R_EP] = ulTempBufEnd[0];
	HL[HL_BUF_W_EP] = ulTempBufEnd[1];
}
void Vender_Trim_UART_JTAG(void)
{

	UBYTE ubTrimMode;							// 0: UART, 1:JTAG
	U32 ultemp;

	//ubTrimMode = HL[HL_BUF_OFST];
	ubTrimMode = HB[HB_LBA_L];
	//
	// To read the target fuse table to check whether been programmed or not.
	//
	SYS1B[SYS1B_EFU_MODE_SEL] |= SET_BIT0;		// Set EFU_MODE: 0x0011 read mode.
	SYS1B[SYS1B_EFU_MODE_SEL] |= SET_BIT1;		// Set EFU_MODE: 0x0011 read mode.

	while (1) {									// delay 100us.
		ultemp = *((volatile unsigned long*)0x04000720);
		if ((ultemp - * ((volatile unsigned long*)0x04000720)) > 1000)
			break;
	}

	while (!(SYS1L[SYS1L_EFU_RDY] & EFU_READY));		// Polling efuse ready bit
	if ((ubTrimMode) ? (SYS1L[SYS1L_PROGRAM_WORD0] & BIT0) : (SYS1L[SYS1L_PROGRAM_WORD0] & BIT1)) {
		HB[HB_DATA_TRG] = H_DATA_TRG; 			// if the value is 1, means already programmed.
		while (HB[HB_DATA_TRG]&H_DATA_TRG); 	// return to HOST.
		return;
	}

	//
	// To program the target.
	//
	SYS1B[SYS1B_EFU_MODE_SEL] &= CLR_BIT0;		// Set EFU_MODE: 0x0010 program mode.
	SYS1B[SYS1B_EFU_MODE_SEL] |= SET_BIT1;		// Set EFU_MODE: 0x0010 program mode.

	while (1) {									// delay 100us.
		ultemp = *((volatile unsigned long*)0x04000720);
		if ((ultemp - * ((volatile unsigned long*)0x04000720)) > 1000)
			break;
	}

	//SYS1B[SYS1B_RPRAM_CFG0]	|= SET_BIT0;		// Set BIT0 to start efuse program mode.
	SYS1L[SYS1L_EFU_PROG_START] |= PROGRAM_MODE_START; // Set BIT0 to start efuse program mode.
	while (!(SYS1L[SYS1L_EFU_RDY] & EFU_READY));		// Polling efuse ready bit

	// Program trim uart disable or jtag disable.
	SYS1L[SYS1L_PROGRAM_WORD0] |= ((ubTrimMode) ? SET_BIT0 : SET_BIT1);

	while (!(SYS1L[SYS1L_EFU_RDY] & EFU_READY));		// Polling efuse ready bit

	// Program mirror trim uart disable or jtag disable.
	// SYS1B[0x480] |= ((ubTrimMode) ? SET_BIT0 : SET_BIT1);

	// while(!SYS1B[0x404])		// Polling efuse ready bit

	//SYS1B[SYS1B_RPRAM_CFG0] &= CLR_BIT0;		// Set BIT0 to start efuse program mode.
	SYS1L[SYS1L_EFU_PROG_START] &= ~(PROGRAM_MODE_START);
	while (!(SYS1L[SYS1L_EFU_RDY] & EFU_READY));		// Polling efuse ready bit

	SYS1B[SYS1B_EFU_MODE_SEL] |= SET_BIT0;		// Set EFU_MODE: 0x0011 read mode.

	HB[HB_DATA_TRG] = H_DATA_TRG; 			// if the value is 1, means already programmed.
	while (HB[HB_DATA_TRG]&H_DATA_TRG); 	// return to HOST.
	return;

}
void Vender_Write_SRAM(void)
{
	M_CheckRDTModeToDisableFunction();
	U32 ulAddressOffset;
	U32 ulTempBufStart[2], ulTempBufEnd[2], ulTempBase;

	//ulAddressOffset = (((U32)HB[HB_LBA_M]) >>1) | (((U32)(0x0F & HB[HB_LBA_H])) << 7);
	ulAddressOffset = 0;
	ulTempBase = HL[HL_BUF_BASE];
	ulTempBufStart[0] = HL[HL_BUF_R_SHF];
	ulTempBufStart[1] = HL[HL_BUF_W_SHF];
	ulTempBufEnd[0] = HL[HL_BUF_R_EP];
	ulTempBufEnd[1] = HL[HL_BUF_W_EP];

	HL[HL_BUF_BASE] = BUFFER3_BASE / 4096;
	HL[HL_BUF_R_EP] = BUFFER3_READ_END;
	HL[HL_BUF_W_EP] = BUFFER3_WRITE_END;
	HL[HL_BUF_R_SHF] = BUFFER3_READ_START;
	HL[HL_BUF_W_SHF] = BUFFER3_WRITE_START;

	HL[HL_BUF_OFST] = ulAddressOffset;
	HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

	HL[HL_BUF_BASE] = ulTempBase;
	HL[HL_BUF_R_SHF] = ulTempBufStart[0];
	HL[HL_BUF_W_SHF] = ulTempBufStart[1];
	HL[HL_BUF_R_EP] = ulTempBufEnd[0];
	HL[HL_BUF_W_EP] = ulTempBufEnd[1];
}

void Vender_Write124Byte_Reg(void)
{
	//	M_CheckRDTModeToDisableFunction();
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
	}

	HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	//RTT1_Stop();

	switch (HB[HB_LBA_M]) {
		case 1:
			*((UBYTE *)BUFL_BASE[0]) = BUFB_BASE[4];
			break;
		case 2:
			*((UWORD*)BUFL_BASE[0]) = BUFW_BASE[2];
			break;
		case 4:
			*((U32 *)BUFL_BASE[0]) = BUFL_BASE[1];
			break;
	}
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}

	if (ENABLE_DEBUG_UART_VENDER) {
		UartString("\r\nW-Reg: add:");
		UartLongHex(BUFL_BASE[0]);
		UartString("  val:");
		UartLongHex(BUFL_BASE[1]);
	}
}
//========================================================================================
//  Read Reg 4 Byte
//  Need Vender_Write_SRAM before Vender_Read124Byte_Reg
//========================================================================================
void Vender_Read124Byte_Reg(void)
{
	//	M_CheckRDTModeToDisableFunction();
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
		xthal_dcache_region_invalidate((void *)BUFL_BASE[0], 512);
	}

	switch (HB[HB_LBA_M]) {
		case 1:
			BUFB_BASE[4] = *((UBYTE *)BUFL_BASE[0] );
			break;
		case 2:
			BUFW_BASE[2] = *((UWORD*)BUFL_BASE[0]);
			break;
		case 4:
			BUFL_BASE[1] = *((U32 *)BUFL_BASE[0]);
			break;
		default:
			BUFL_BASE[1] = 0xffffffff;
			break;
	}
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
	}
	HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

	if (ENABLE_DEBUG_UART_VENDER) {
		UartString("\r\nR-Reg: add:");
		UartLongHex(BUFL_BASE[0]);
		UartString("  val:");
		UartLongHex(BUFL_BASE[1]);
	}
	//RTT1_Stop();
}
void Vender_Direct_Read_Info(void)
{
#if(!RDT)
	//	M_CheckRDTModeToDisableFunction();
	UBYTE ubCEIndex, ubMode = HB[HB_LBA_L];
	U32 ulMemShift, ulLCA_temp;

	if (ubMode == 1) {
		ulMemShift = M_SysInfo_SmartMPBlk;
	}
	else if (ubMode == 2) {
		ulMemShift = M_SysInfo_RetryTable;
	}
	else if (ubMode == 3) {
		ulMemShift = M_SysInfo_DPSTable;
	}
	else if (ubMode == 4) {
		ulMemShift = M_SysInfo_RetryTable_1k;
	}
	else {
		ulMemShift = M_SysInfo_InfoBlk;

	}
	if (RDT_BURNER) {
		ulLCA_temp = MARK_SYS_RDT;
	}
	else {
		ulLCA_temp = MARK_SYS;
	}

	//if (ENABLE_DCACHE) {
	//	xthal_dcache_region_invalidate(BUFB_BASE, 512);
	//}
	//memset(BUFB_BASE, 0, 512);
	if (gubFlashInitialized == 1) { //firmware mode
		HandleStopRW(1);
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			UBYTE ubSystemUnit = guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog;
			UBYTE ubSystemBurst = guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask;
			if (ftlReadSinglePlane(ubSystemUnit, 0, ubCEIndex, (U32)&BUFB_BASE[0], ulLCA_temp,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY), 4, 0, ubSystemBurst) == 0) {
				break;
			}
			else {
				if (ubCEIndex == gubPlanesPerBurst) {
					ftlPreformatShowError(ubCEIndex, ubSystemUnit, 0x21);
					for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
						SetFailCEtoSystemInfo(ubCEIndex);
					}
					return;
				}
			}
		}
		gubStopFTL = 0;
		//if (ubMode) {
		//	memcpy(&BUFB_BASE[0], (void*)&BUFB_BASE[M_SysInfo_SmartMPBlk], (sizeof(MP_SMART_Attribute_Entry) * AttributeTotalEntry));
		//}
		//else {
		//	memcpy(&BUFB_BASE[0], (void*)&BUFB_BASE[M_SysInfo_InfoBlk], sizeof(SYSTEM_GROUP_t));//return memory data
		//}
	}
	else if (gubFlashInitialized == 0) {} //burner Mode
	else {
		flaLEDBlink(0x21);
	}
	HL[HL_BUF_OFST] = ulMemShift / 0x200;
	HW[HW_SECTOR_CNT] = gulSectorCnt;
	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	//HL[HL_LAST_END_LBA] =0;
	HL[HL_TQ_CONTENT] = 0;
	Vender_HostRemainSectorCount();
#else
	RDT_FINDBLOCKS_TASK_without_relytest(3, 2);


	HL[HL_BUF_OFST] = BUFB_BASE[0] / 0x200;
	HW[HW_SECTOR_CNT] = gulSectorCnt;
	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	//HL[HL_LAST_END_LBA] =0;
	HL[HL_TQ_CONTENT] = 0;
	Vender_HostRemainSectorCount();

#endif
}

void Vender_Direct_Write_Info(UBYTE ubIsDLMC, U32 ulSourceAddress)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	UBYTE ubCEIndex;//, ubCodeNum;
	UBYTE ubMode = (ubIsDLMC == 0) ? (HB[HB_LBA_L]) : 0;
	U32 ulMemShift, ulLCA_temp;//, uli;
	UBYTE ubFail;

	UBYTE ubOldSystemUnit;
	UBYTE ubOldSystemBurst;
	UBYTE ubOldSystemSet = gubSystemSet;
	//UBYTE ubGetSystemSet = TRUE;
	gulSystemVersion = (gulSystemVersion & 0xFFFFFFF0) + ((gulSystemVersion + 1) & 0x0000000F);//ex: 0x112233 -> 0x112234, 0x11223F->0x112230

	HandleStopRW(1);
	if (ubMode == 1) {
		ulMemShift = M_SysInfo_SmartMPBlk;
	}
	else if (ubMode == 2) {
		ulMemShift = M_SysInfo_RetryTable;
	}
	else if (ubMode == 3) {
		ulMemShift = M_SysInfo_DPSTable;
	}
	else if (ubMode == 4) {
		ulMemShift = M_SysInfo_RetryTable_1k;
	}
	else {
		ulMemShift = M_SysInfo_InfoBlk;

	}

	if (RDT_BURNER) {
		ulLCA_temp = MARK_SYS_RDT;
	}
	else {
		ulLCA_temp = MARK_SYS;
	}
	//memset(BUFB_BASE, 0, 512);

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		ubOldSystemUnit = guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] >> gubBurstsPerBankLog;
		ubOldSystemBurst = guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] & gubBurstsPerBankMask;
		if (ftlReadSinglePlane(ubOldSystemUnit, 0, ubCEIndex, (U32)&BUFB_BASE[0], ulLCA_temp,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY), 4, 0, ubOldSystemBurst) == 0) {
			break;
		}
		else {
			if (ubCEIndex == gubPlanesPerBurst) {
				ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex], 0x21);
				for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				return;
			}
		}
	}

	if (ubMode == 3) {
		BUFB_BASE[M_SysInfo_gubDPSTableValid] = 1;
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512 * 32);
	}
	if (ubIsDLMC == 0) {
		HL[HL_BUF_OFST] = ulMemShift / 0x200;
		//HL[HL_LAST_END_LBA] =0;

		HW[HW_SECTOR_CNT] = gulSectorCnt;
		HL[HL_BUF_TRG_SEC] = gulSectorCnt;
		//HL[HL_LAST_END_LBA] =0;
		HL[HL_TQ_CONTENT] = 0;
		Vender_HostRemainSectorCount();
	}
	else {
		mSE_COPY(ulSourceAddress, (U32)&BUFB_BASE[M_SysInfo_InfoBlk], 512); //Use guwEarlyBadCntPerPlane RAM
	}
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512 * 32);
	}
	SYSTEM_GROUP_t *InfoArray = (SYSTEM_GROUP_t *)(&BUFB_BASE[M_SysInfo_InfoBlk]);

	if (((InfoArray->u8bPhIsOnInfoMark[0] == 'P') && (InfoArray->u8bPhIsOnInfoMark[1] == 'h') && (InfoArray->u8bPhIsOnInfoMark[2] == 'I')
	        && (InfoArray->u8bPhIsOnInfoMark[3] == 's') && (InfoArray->u8bPhIsOnInfoMark[4] == 'O') && (InfoArray->u8bPhIsOnInfoMark[5] == 'n')) || (ubMode)) {

		ubFail = UpdateSystemBlock(Return_FailInfo, (U32)&BUFB_BASE[0]);

		if (ubFail == NORMAL) {
			VT->guwSystemUpdateCount++;
		}
	}
	gubStopFTL = 0;
#endif
}


void Vender_HW_SDR_Read(void)
{
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
	}
	U32 ulSDR_Addr = BUFL_BASE[2];
	HL[HL_BUF_BASE] = DDR_BASE / 4096;
	HL[HL_BUF_R_EP] = DDR_SIZE << 8;
	HL[HL_BUF_W_EP] = DDR_SIZE << 8;
	HL[HL_BUF_R_SHF] = 0;
	HL[HL_BUF_W_SHF] = 0;

	HL[HL_BUF_OFST] = (ulSDR_Addr - DDR_BASE) >> 9;

	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	HB[HB_DATA_TRG] = H_DATA_TRG;
	while (HB[HB_DATA_TRG]&H_DATA_TRG);

}

void Vender_HW_SDR_Write(void)
{
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
	}
	U32 ulSDR_Addr = BUFL_BASE[2];
	HL[HL_BUF_BASE] = DDR_BASE / 4096;
	HL[HL_BUF_R_EP] = DDR_SIZE << 8;
	HL[HL_BUF_W_EP] = DDR_SIZE << 8;
	HL[HL_BUF_R_SHF] = 0;
	HL[HL_BUF_W_SHF] = 0;

	HL[HL_BUF_OFST] = (ulSDR_Addr - DDR_BASE) >> 9;

	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	HB[HB_DATA_TRG] = H_DATA_TRG;
	while (HB[HB_DATA_TRG]&H_DATA_TRG);

}

void Vender_CheckRetryTable(void)
{
	UWORD uwParamIndex;
	UWORD uwD2D3RetryTableLegth;
	mSE_SV(8, (U32)BUFB_BASE, 512 * gulSectorCnt, 0, 0);
	UBYTE ubBicsType = HB[HB_LBA_H];

	if ((ubBicsType > 1) && (!gRRTMode.B.ubType) && ENABLE_MP_SET_RETRY_TABLE && (!guoFlashUseType.B.btSandisk)) {
#if TLC_BICS2
		UBYTE ubGroupNumber_D1, ubGroupNumber_D3;
		UBYTE * ubRetryTable;


		if (guoFlashUseType.B.btToshibaBiCs3) {

			if (gubDensitysize == FLH_Size_256Gb) {
				if (ubBicsType == 2) {
					ubRetryTable = gubRetryRTable_BISC3_256Gb_Enterprise;
					ubGroupNumber_D1 = 26; // gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE]
					ubGroupNumber_D3 = 111; // gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE]
				}
				else if (ubBicsType == 3) {
					ubRetryTable = gubRetryRTable_BISC3_256Gb_Industrial;
					ubGroupNumber_D1 = 8; // gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE]
					ubGroupNumber_D3 = 56; // gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE]
					BUFB_BASE[512] = 0x26;
				}
			}
			else if (gubDensitysize == FLH_Size_512Gb) {
				if (ubBicsType == 2) {
					ubRetryTable = gubRetryRTable_BISC3_TOSHIBA_512Gb_Enterprise;
					ubGroupNumber_D1 = 8; // gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE]
					ubGroupNumber_D3 = 56; // gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE]
				}
			}

		}


		BUFW_BASE[2] = 1;  // gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE]
		BUFW_BASE[4] = 7; // gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]

		BUFW_BASE[1] = ubGroupNumber_D1;
		BUFW_BASE[3] = ubGroupNumber_D3;

		for (uwParamIndex = 0; uwParamIndex < ubGroupNumber_D3 * 7 ; uwParamIndex++) {
			BUFB_BASE[512 + 1 + uwParamIndex] = ubRetryTable[uwParamIndex + ubGroupNumber_D1];
		}
		for (uwParamIndex = 0; uwParamIndex < ubGroupNumber_D1; uwParamIndex++) {
			BUFB_BASE[512 + 1 + ubGroupNumber_D3 * 7 + uwParamIndex] = ubRetryTable[uwParamIndex];
		}


#endif

	}
	else {

		BUFW_BASE[1] = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE];
		BUFW_BASE[2] = gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];
		if (guoFlashUseType.B.btToshiba1Znm && guoFlashUseType.B.bt4LC && guoFlashUseType.B.btSandisk) {
			BUFW_BASE[3] = (UWORD)gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] * 2;
		}
		else {
			BUFW_BASE[3] = gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE];
		}
		BUFW_BASE[4] = gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE];

		uwParamIndex = 0;
		if (guoFlashUseType.B.btToshiba1Znm && guoFlashUseType.B.bt4LC && guoFlashUseType.B.btSandisk) {
			uwD2D3RetryTableLegth = (UWORD)gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] * 2 * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE];
		}
		else {
			uwD2D3RetryTableLegth = (UWORD)gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE];
		}

		BUFB_BASE[512] = (gRRTMode.B.btRelaxMode) ? 0x26 : 0x00;

		if (guoFlashUseType.B.btToshibaBiCs4 && (gubDensitysize == FLH_Size_256Gb) && (!guoFlashUseType.B.btSandisk) && (!gRRTMode.B.btNoRevision)) {
			if (gRRTMode.B.btRevision == 0) {
				BUFB_BASE[10] = 1; // early
			}
			else {
				BUFB_BASE[10] = 2; //final
			}
		}

		for (uwParamIndex = 0; uwParamIndex < uwD2D3RetryTableLegth; uwParamIndex++) {
			BUFB_BASE[512 + 1 + uwParamIndex] = gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + uwParamIndex];
		}
		for (uwParamIndex = 0; uwParamIndex < gubHardBitRetryD2D3GroupBase; uwParamIndex++) {
			BUFB_BASE[512 + 1 + uwD2D3RetryTableLegth + uwParamIndex] = gubHardBitRetryParameterTable[uwParamIndex];
		}
	}
	BUFB_BASE[0] = 'R';
	BUFB_BASE[1] = 'T';
	BUFB_BASE[12] = 1; // Bics3 RRT SLC table0

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512 * gulSectorCnt);
	}
	//	HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
	//	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	Vender_TriggerHostData_31();

}

void Vender_LoadBurnerIcode(void)
{
	UBYTE ub64Koffset = HB[HB_LBA_H];
	UBYTE ubIcodeDone = HB[HB_LBA_M];
	UBYTE *ubBUF_64Kshift = (UBYTE *)(BUFFER3_BASE + 0x10000);
	UBYTE ubi;
	Vender_TriggerHostData();
	mDMAC_SV(4, (U32)&ubBUF_64Kshift[0], gulSectorCnt * 0x200, 0, 0xFFFFFFFF, 0, DMA_EN_SETCQ, 1);
	mDMAC_XOR((U32)&ubBUF_64Kshift[0], (U32)&BUFB_BASE[0], (0x60000400 + ub64Koffset * 128 * 512), gulSectorCnt * 0x200, 1, 0);
	if (ubIcodeDone) {
		gubIcdeDone = 1;
#if TLC_BICS2
		if (guoFlashUseType.B.btToshibaBiCs2) {
			if (guoFlashDefaultType.B.btEnterprise) {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC2_Enterprise, sizeof(gubRetryRTable_BISC2_Enterprise));
			}
			else {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC2, sizeof(gubRetryRTable_BISC2));
			}
		}
		else if (guoFlashUseType.B.btToshibaBiCs3) {

			if (guoFlashUseType.B.btSandisk) {
				if (gubDensitysize == FLH_Size_512Gb) {
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_SANDISK_512Gb, sizeof(gubRetryRTable_BISC3_SANDISK_512Gb));
				}
				else if (gubDensitysize == FLH_Size_128Gb) {
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_SANDISK_128Gb, sizeof(gubRetryRTable_BISC3_SANDISK_128Gb));
				}
				else {
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_SANDISK, sizeof(gubRetryRTable_BISC3_SANDISK));
				}
			}
			else if (gubDensitysize == FLH_Size_512Gb) {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_TOSHIBA_512Gb_Normal, sizeof(gubRetryRTable_BISC3_TOSHIBA_512Gb_Normal));
			}
			else if (gubDensitysize == FLH_Size_128Gb) {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_128Gb, sizeof(gubRetryRTable_BISC3_128Gb));
			}
			else {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_256Gb_Normal, sizeof(gubRetryRTable_BISC3_256Gb_Normal));
			}
		}
#if TSB_BICS4_SUPPORT
		else if (guoFlashUseType.B.btToshibaBiCs4) {

			if (guoFlashUseType.B.btSandisk) {
				if (gubDensitysize == FLH_Size_512Gb) {
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_SANDISK_512Gb, sizeof(gubRetryRTable_BISC4_SANDISK_512Gb));
				}
				else {
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_SANDISK_256Gb, sizeof(gubRetryRTable_BISC4_SANDISK_256Gb));
				}
			}
			if (gubDensitysize == FLH_Size_512Gb) {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_512Gb, sizeof(gubRetryRTable_BISC4_512Gb));
			}
			else {
				flaBics4_Revision_Identify();
				if (gRRTMode.B.btNoRevision == 1) {
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_256Gb_HV, sizeof(gubRetryRTable_BISC4_256Gb_HV));
				}
				else if (gRRTMode.B.btRevision == 0) {
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_256Gb_Early, sizeof(gubRetryRTable_BISC4_256Gb_Early));
				}
				else {
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_256Gb_Final, sizeof(gubRetryRTable_BISC4_256Gb_Final));
				}
			}
		}
#endif
#endif
#if (TLC && (TLC_BICS2 == 0))
		if ( ( guoFlashUseType.B.btToshiba == 1 ) && ( guoFlashUseType.B.btSandisk == 0 ) ) {
			memcpy(gubHardBitRetryParameterTable, gubRetryRTable_15nmTLC, sizeof(gubRetryRTable_15nmTLC));
		}
#endif
#if EnableProductHistory
		gubPHinit = 0;
		gubTureErase = 0;
		for (ubi = 0; ubi < PHUnitNum; ubi++) {
			guwPHUnit[ubi].uwUnit = PH_Default;
			guwPHUnit[ubi].ubPlanebank = 0xff;
			guwPHUnit[ubi].uwPagePTR = PH_Default;
			gubPHmatch[ubi] = 0 ;
		}
#endif

	}
}

void Vender_VerifyBurnerIcode(void)
{
	UBYTE ub64Koffset = HB[HB_LBA_H];
	UBYTE *ubBUF_64Kshift = (UBYTE *)(BUFFER3_BASE + 0x10000);
	mDMAC_SV(4, (U32)&ubBUF_64Kshift[0], gulSectorCnt * 0x200, 0, 0xFFFFFFFF, 0, DMA_EN_SETCQ, 1);
	mDMAC_XOR((U32)&ubBUF_64Kshift[0], (0x60000400 + ub64Koffset * 128 * 512), (U32)&BUFB_BASE[0], gulSectorCnt * 0x200, 1, 0);
	Vender_TriggerHostData();
}

void Vender_CheckLaterBadReuse(void)
{
	M_CheckNonBurnerModeToDisableFunction();
	HW[HW_DATA_TRG] |= H_DATA_TRG;
	while (HW[HW_DATA_TRG]&H_DATA_TRG);
#if 0
	UWORD uwCECheck = HW[HW_LBA_L], uwDBT_Page, uwplane, uwi, uwj;
	UBYTE ubCEIndex, ubCE, ubDie = HB[HB_LBA_H], ubCE_Target, ubPlaneOffset, ubBurstBank, ubUpdateDBT = 0, ubPlaneBank, ubi, ubj, ubGoodblk;
	U32 ulTemp, ulTemp1, ulVenderUnit, ulUnitPerGroup = (gulTotalPhyUnitNum >> gubTotalGroupsLog), uli, ulStart, ulEnd;
	UBYTE *ubDBTBuffer = (UBYTE *)&gulVenderSMARTBuffer[0]; //(UBYTE*)(&gulGRTable[0]);
	UBYTE *ubCopyBuffer = (UBYTE *)&gulVenderSMARTBuffer[0] + 0x4000;
	UBYTE *Temp_Buffer;
	UBYTE *ubDataBuffer = (UBYTE *)&gubCopyBuffer[0];
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	UBYTE ubDBT_temp = 0xFF;

	//=================================find swap unit========================================

	if (gubSystemUnit == 0xFF) {
		if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {
			ftlPreformatShowError(0, gubDBTUnit, 0x28);
			return;
		}
		U32 ulFreeblock;
		for (ulVenderUnit = 0; ulVenderUnit < SYSTEMAREA_DEFAULT_SCAN_RANGE; ulVenderUnit++) {
			ulTemp = 0;
			ulFreeblock = (M_Multiplier((ulVenderUnit << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) ;
			for (ulTemp1 = 0 ; ulTemp1 < gubPlanesPerBurst ; ulTemp1++) {
				if (ENABLE_PLANE_CH_CE) {
					ubPlaneBank = ulTemp1 << gubBurstsPerBankLog;
				}
				else {
					ubPlaneBank = ulTemp1;
				}
				if (gubRDTDone & DBT_Exist) {
					if (ubDBTBuffer[ulFreeblock + ubPlaneBank] & 0xF0) {
						ulTemp |= BIT0;
					}
				}
#if MicronFlashOnly
				ulTemp |= ftlGoodBlock(ulVenderUnit, ubPlaneBank, (U32)&ubCopyBuffer[0], 1, GOODBLK_MLC);
#else
				ulTemp |= ftlGoodBlock(ulVenderUnit, ubPlaneBank, (U32)&ubCopyBuffer[0], 1);
#endif
				if (ulTemp) break;
			}

			if (ulTemp == 0) {
				ubDBT_temp = ulVenderUnit;
				break;
			}
		}
	}
	else {
		ubDBT_temp = gubSystemUnit;
	}
	//=================================scan laterbad========================================
	for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
		for (ubCE = 0; ubCE < gubMAX_SCAN_CE; ubCE++) {    //Max CE is 16 with normal mode/ 64 with decoder
			if (uwCECheck & (BIT0 << ubCE)) {
				if (ENABLE_VenderCMD_return_PhysicalCE) {
					if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
						UBYTE ubBreak = 0;
						UBYTE ubi, ubj ;
						for (ubj = 0; ubj < MAX_CHANNEL ; ubj++) {
							//ubj = (ubCE % gubPlanesPerBurst);
							for (ubi = 0; ubi < MAX_CE_PER_CH; ubi++) {
								if ((gubCH_CEMappingToPCE[ubj][ubi] + ubj * gubMAX_SCAN_CE_PER_CH) == ubCE ) {
									UartString("\n Later Bad Physical CE:");
									Uart_Tx_DataHex(ubCE);
									ubCE_Target = ubi * MAX_CHANNEL + ubj;
									UartString(", Mapping to Global CE:");
									Uart_Tx_DataHex(ubCE_Target);
									ubBreak = 1;
									break;
								}
							}
							if (ubBreak) {
								break;
							}
						}
					}
					else {
						UBYTE ubCE_temp;
						if (FCONB[(FCONL_FCE_SEL_0 << 2) + ubCE] == 0x3F) {
							continue;
						}
						else {
							ubCE_temp = FCONB[(FCONL_FCE_SEL_0 << 2) + ubCE];
							ubCE_Target = ((ubCE_temp % 8) << 1) + (ubCE_temp >> 3);
						}
					}
				}
				gubSM_GroupNo = ubDie;
				if (ENABLE_PSEUDO_CE && gubExpandCE) {  //Load physical CE, translate it to logical view
					if (ubDie >= gubDieNumber) {
						ubCE_Target += (ubDie / gubDieNumber) * gubMTQTotalPhyCE;
						gubSM_GroupNo = ubDie % gubDieNumber;
					}
				}

				if (ENABLE_PLANE_CH_CE) {
					ubPlaneOffset = (M_Multiplier(gubSM_GroupNo, DEF_PlanesPerPage_Mcl_Div)) + (ubCE_Target << gubBurstsPerBankLog);
				}
				else {
					ubPlaneOffset = (M_Multiplier(gubSM_GroupNo, DEF_PlanesPerPage_Mcl_Div)) + ((ubCE_Target >> gubPlanesPerBurstLog) << gubPlanesPerBankLog) + (ubCE_Target % gubPlanesPerBurst);
				}

				uwDBT_Page = 0;

				for (ulVenderUnit = 0; ulVenderUnit < ulUnitPerGroup; ulVenderUnit++) {
					ulTemp = M_Multiplier((ulVenderUnit << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div) ;
					ulTemp1 = ulTemp % ulEntryPerDBTPlane;
					if (ulTemp1 == 0) {
						//=================================swap&load DBT========================================
						UBYTE ubDBT_Keep = gubDBTUnit;
						if (ubUpdateDBT) {
							UWORD uwplane = 0;
							U32 ulFreeblock = 0;
							U32 TotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;

							while (TotalBlockNum > ulFreeblock) {
								//loadDBTtoBuffer
								if (uwplane != (uwDBT_Page - 1)) {
									if (ftlLoadDBT(ulFreeblock, (U32)&ubCopyBuffer[0])) {
										ftlPreformatShowError(0, gubDBTUnit, 0x28);
										ftlA2EraseSingleUnit(ubDBT_temp);
										return;
									}
								}
								//programDBTtoFLA
								for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
									if (uwplane == (uwDBT_Page - 1)) {
										Temp_Buffer = (UBYTE *)&ubDBTBuffer[0];
									}
									else {
										Temp_Buffer = (UBYTE *)&ubCopyBuffer[0];
									}
									if (ftlWriteSinglePlane(ubDBT_temp, uwplane,  ubCEIndex, ((U32)&Temp_Buffer[0] ), MARK_DBT, 0, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0)) {
										ftlPreformatShowError(ubCEIndex, ubDBT_temp, 0x12);
										ftlA2EraseSingleUnit(ubDBT_temp);
										return;
									}
								}
								gubDBTUnit = ubDBT_temp;
								if (ftlLoadDBT(ulFreeblock, (U32)&Temp_Buffer[0])) {
									ftlPreformatShowError(0, gubDBTUnit, 0x28);
									ftlA2EraseSingleUnit(ubDBT_temp);
									if (ubUpdateDBT) {
										gubDBTUnit = ubDBT_Keep;
									}
									return;
								}
								gubDBTUnit = ubDBT_Keep;
								uwplane++;//ulplane += gubPlanesPerPage;
								ulFreeblock += ulEntryPerDBTPlane;
							}
						}

						if (ubUpdateDBT) {
							gubDBTUnit = ubDBT_temp;
						}
						if (ftlLoadDBT(ulTemp, (U32)&ubDBTBuffer[0])) {
							ftlPreformatShowError(0, gubDBTUnit, 0x28);
							ftlA2EraseSingleUnit(ubDBT_temp);
							if (ubUpdateDBT) {
								gubDBTUnit = ubDBT_Keep;
							}
							return;
						}
						if (ubUpdateDBT) {
							ftlA2EraseSingleUnit(ubDBT_Keep);
							ubDBT_temp = ubDBT_Keep;
							ubUpdateDBT = 0;
						}
						uwDBT_Page ++;
					}
					if ((gubRUT_EC_set & (BIT0 | BIT1)) == (BIT0 | BIT1)) {
						if ((ulVenderUnit < gubSystemAreaNum) && (gubSM_GroupNo == 0)) {
							continue;
						}
					}
					ulTemp1 += ubPlaneOffset;
					//=================================check later bad bit========================================
					for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; ubBurstBank++) {
						if ((ubDBTBuffer[ulTemp1] & (BIT4 | BIT5))) {
							U32 ulPhyUnit = ulUnitPerGroup * gubSM_GroupNo + ulVenderUnit;
							gubReadHBRetry_Enable = 1;
							ubGoodblk = 0;
							uli = gubSM_GroupNo * ulUnitPerGroup + ulVenderUnit;
#if MicronFlashOnly
							if (ftlGoodBlock(uli, ubPlaneBank + (ubCE_Target << gubBurstsPerBankLog), (U32)ubCopyBuffer, 1, GOODBLK_MLC))
#else
							if (ftlGoodBlock(uli, ubPlaneBank + (ubCE_Target << gubBurstsPerBankLog), (U32)ubCopyBuffer, 1))
#endif
							{
								UartString("early bad:");
								UartLLongHex(ulPhyUnit);
								UartString("\n");
								UartString("ulTemp1:");
								UartLLongHex(ulTemp1);
								UartString("\n");
								UartString("ubDBTBuffer[ulTemp1]:");
								UartLLongHex(ubDBTBuffer[ulTemp1]);
								UartString("\n");
							}
							for (uwplane = 0; uwplane < 20; uwplane++) {
								for (ubi = 0; ubi < 4; ubi++) {
									mDMAC_SV(3, (U32)&ubDataBuffer[0] + 4 * 1024 * ubi, 4 * 1024, MARK_DBT, 0x55AA55AA, 0, DMA_EN_SETCQ, 1);
								}
								if (ftlWriteSinglePlane(ulPhyUnit, (((M_Multiplier(uwplane, DEF_PlanesPerPage_Mcl_Div)) + (ubCE_Target << gubBurstsPerBankLog) + ubBurstBank) << gub4kEntrysPerPlaneLog),
								                        ubCE_Target, ((U32)&ubDataBuffer[0] ), MARK_DBT, 0, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 1)) {
									ftlPreformatShowError(ubCE_Target, ulPhyUnit, 0x12);
									ubGoodblk = BIT0;
									UartString("ubCE_Target:");
									UartLLongHex(ubCE_Target);
									UartString("\n");
									UartString("ubBurstBank:");
									UartLLongHex(ubBurstBank);
									UartString("\n");
									UartString("planes:");
									UartLLongHex(uwplane);
									UartString("\n");
									UartString("W fail!!\n");
									gubErrorCode = 0;
									break;
								}
								if (ftlReadSinglePlane(ulPhyUnit, (((M_Multiplier(uwplane, DEF_PlanesPerPage_Mcl_Div)) + (ubCE_Target << gubBurstsPerBankLog) + ubBurstBank) << gub4kEntrysPerPlaneLog),
								                       ubCE_Target, ((U32)&ubDataBuffer[0] ), MARK_DBT,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY), 4, 1)) {
									ftlPreformatShowError(ubCE_Target, ulPhyUnit, 0xC);
									ubGoodblk = BIT0;
									UartString("R fail!!\n");
									gubErrorCode = 0;
									break;
								}
								for (ubi = 0; ubi < 4; ubi++) {
									mDMAC_E3D(((U32)&ubDataBuffer[0] + 4 * 1024 * ubi ), 4 * 1024, MARK_DBT, 1);
									if (gubE3DFailFlag) {
										ubGoodblk = BIT0;
										UartString("E3D fail!!\n");
										break;
									}
								}
								if (ubGoodblk) {
									break;
								}
							}
							UBYTE ubFQIndex = gFQI.ubFQLinkFree;
							FlashQueue_t *FQ;
							UBYTE ubDepth, ubL4kLink;
							M_GetFQ(FQ, ubDepth, ubL4kLink, ubCE_Target);
							FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
							FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);

							// Must Send Reset CMD(0xFF) After Erase Fail
							FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

							FQ->uwFUnit = ulPhyUnit;
							FQ->ulFEntry = (((ubCE_Target << gubBurstsPerBankLog) + ubBurstBank) << gub4kEntrysPerPlaneLog);
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
							ftlA2EraseSingleUnit(ulPhyUnit);

							gubReadHBRetry_Enable = 0;
							//===============================renew Goodblk==============================================
							if (ubGoodblk == 0) {
								//UartString("renew!!\n");
								ubUpdateDBT = 1;
								ubDBTBuffer[ulTemp1] = BIT3;  //renew mark
								//===============================set renewblk to extend========================================
								if ((gubRUT_EC_set & (BIT0 | BIT1)) == (BIT0 | BIT1)) {
									//UartString("setRUT!!\n");
									UWORD uwEndUnitofDie;
									if (gubSM_GroupNo == (gubTotalGroups - 1)) {
										uwEndUnitofDie = VT->guwDieStartSFUnitIndex[gubTotalGroups];
									}
									else {
										uwEndUnitofDie = VT->guwDieStartSFUnitIndex[gubSM_GroupNo + 1] - 1;
									}
									if ((VT->guwRUTUnitIndexOfLastBlock[ubPlaneOffset + ubBurstBank] + 1) > uwEndUnitofDie) {
										if (gubSM_GroupNo < (gubTotalGroups - 1)) {
											ulStart = (VT->guwDieStartSFUnitIndex[gubSM_GroupNo + 1] << gubBurstsPerBankLog) - 1;
											ulEnd = VT->guwDieStartSFUnitIndex[gubTotalGroups] << gubBurstsPerBankLog;
											for (uli = ulEnd; uli > ulStart; uli--) {
												for (ubj = 0; ubj < gubCENumber; ubj++) {
													M_RUT2DTo1D(ubj, uli + gubBurstsPerBank) =  M_RUT2DTo1D(ubj, uli);
												}
											}

											for (ubi = (gubTotalGroups - 1);  ubi > gubSM_GroupNo; ubi--) {
												VT->guwDieStartSFUnitIndex[ubi]++;
												//VT->guwExtendUnitsIndex[ubi]++;
												for (ubj = 0; ubj < gubPlanesPerPage; ubj++) {
													VT->guwRUTUnitIndexOfLastBlock[ubj + (M_Multiplier(ubi, DEF_PlanesPerPage_Mcl_Div))] ++;
												}
											}
										}
										VT->guwDieStartSFUnitIndex[gubTotalGroups]++;
										//VT->guwExtendUnitsIndex[ubDie_Target]++;
									}
									//	UartString("find ubPlaneOffset + ubBurstBank:\n");
									//	UartWordHex(ubPlaneOffset + ubBurstBank);
									//	UartString("\n");

									VT->guwRUTUnitIndexOfLastBlock[ubPlaneOffset + ubBurstBank]++;
									//	UartString("find VT->guwRUTUnitIndexOfLastBlock[ubPlaneOffset + ubBurstBank]:\n");
									//	UartWordHex(VT->guwRUTUnitIndexOfLastBlock[ubPlaneOffset + ubBurstBank]);
									//	UartString("\n");
									uli = (VT->guwRUTUnitIndexOfLastBlock[ubPlaneOffset + ubBurstBank] << gubBurstsPerBankLog) + ubBurstBank;
									M_RUT2DTo1D(ubCE_Target, uli) = (UWORD)ulPhyUnit;
								}
							}
						}
						ulTemp1 ++;
					}
					//===============================update last one plane DBT=====================================

				}
				if (ubUpdateDBT) {
					UBYTE ubDBT_Keep = gubDBTUnit;
					UWORD uwplane = 0;
					U32 ulFreeblock = 0;
					U32 TotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;
					while (TotalBlockNum > ulFreeblock) {
						//loadDBTtoBuffer
						if (uwplane != (uwDBT_Page - 1)) {
							if (ftlLoadDBT(ulFreeblock, (U32)&ubCopyBuffer[0])) {
								ftlPreformatShowError(0, gubDBTUnit, 0x28);
								return;
							}
						}
						//programDBTtoFLA
						for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
							if (uwplane == (uwDBT_Page - 1)) {
								Temp_Buffer = (UBYTE *)&ubDBTBuffer[0];
							}
							else {
								Temp_Buffer = (UBYTE *)&ubCopyBuffer[0];
							}
							if (ftlWriteSinglePlane(ubDBT_temp, uwplane,  ubCEIndex, ((U32)&Temp_Buffer[0] ), MARK_DBT, 0, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0)) {
								ftlPreformatShowError(ubCEIndex, ubDBT_temp, 0x12);
								return;
							}
						}
						gubDBTUnit = ubDBT_temp;
						if (ftlLoadDBT(ulFreeblock, (U32)&Temp_Buffer[0])) {
							ftlPreformatShowError(0, gubDBTUnit, 0x28);
							if (ubUpdateDBT) {
								gubDBTUnit = ubDBT_Keep;
							}
							return;
						}
						gubDBTUnit = ubDBT_Keep;
						uwplane++;//ulplane += gubPlanesPerPage;
						ulFreeblock += ulEntryPerDBTPlane;
					}
					gubDBTUnit = ubDBT_temp;
					ftlA2EraseSingleUnit(ubDBT_Keep);
					ubDBT_temp = ubDBT_Keep;
					ubUpdateDBT = 0;
				}
			}
		}
		if (gubDBTUnit == gubSystemUnit) {
			gubSystemUnit = ubDBT_temp;
		}
		//UartString("find laterbad mark:\n");
		//UartWordHex(gubSystemUnit);
		//UartString("\n");
		//UartString("find ubDBTBuffer:\n");
		//UartWordHex(gubDBTUnit);
		//UartString("\n");
		//==================================swap RUT to 0xBBBB================================================================
		UWORD guwMinRUTIndex = VT->guwRUTUnitIndexOfLastBlock[(M_Multiplier(gubSM_GroupNo, DEF_PlanesPerPage_Mcl_Div))];
		//UWORD uwRUTEndUnit = (VT->guwDieStartSFUnitIndex[0] + VT->guwExtendUnitsIndex[(gubTotalGroups - 1)]<<gubBurstsPerBankLog);
		if (gubSM_GroupNo) {
			ulStart = VT->guwUnitsPerGroups[gubSM_GroupNo - 1];
		}
		else {
			ulStart = 0;
		}
		//UartString("find gubSM_GroupNo:\n");
		//UartWordHex(gubSM_GroupNo);
		//UartString("\n");
		//UartString("find PTR 1st:\n");
		//UartWordHex(gubSM_GroupNo<<gubPlanesPerPageLog);
		//UartString("\n");
		//UartString("find guwMinRUTIndex 1st:\n");
		//UartWordHex(VT->guwRUTUnitIndexOfLastBlock[(gubSM_GroupNo<<gubPlanesPerPageLog)]);
		//UartString("\n");
		ulEnd = VT->guwUnitsPerGroups[gubSM_GroupNo];
		for (ubi = 0; ubi < gubPlanesPerPage; ubi++) {
			if (guwMinRUTIndex > VT->guwRUTUnitIndexOfLastBlock[ubi + (M_Multiplier(gubSM_GroupNo, DEF_PlanesPerPage_Mcl_Div))]) {
				//UartString("find PTR:\n");
				//UartWordHex(ubi + (gubSM_GroupNo<<gubPlanesPerPageLog));
				//UartString("\n");
				guwMinRUTIndex = VT->guwRUTUnitIndexOfLastBlock[ubi + (M_Multiplier(gubSM_GroupNo, DEF_PlanesPerPage_Mcl_Div))];
				//UartString("find guwMinRUTIndex:\n");
				//UartWordHex(guwMinRUTIndex);
				//UartString("\n");
			}

		}

		for (uwi = guwMinRUTIndex; uwi > (VT->guwDieStartSFUnitIndex[gubSM_GroupNo] + 1); uwi--) {
			if (VT->guwPutUserFreeToFailHandleAreaCnt[gubSM_GroupNo]) {
				UartString("renew RUT:\n");

				while (M_RUT2DTo1D(0, (ulStart << gubBurstsPerBankLog)) != 0xBBBB) {
					ulStart ++;
					if (ulStart == ulEnd) {
						break;
					}
				}
				if (ulStart == ulEnd) {
					break;
				}

				UartLLongHex(uwi);
				UartString("\n");
				for (ubi = 0; ubi < gubCENumber; ubi++) {
					for (ubj = 0; ubj < gubBurstsPerBank; ubj++) {
						M_RUT2DTo1D(ubi, (ulStart << gubBurstsPerBankLog) + ubj) = M_RUT2DTo1D(ubi, (uwi << gubBurstsPerBankLog) + ubj);
					}
				}
				//=================================================move RUT================================
				for (uwj = uwi; uwj < (VT->guwDieStartSFUnitIndex[gubTotalGroups] + 1); uwj++) {
					for (ubi = 0; ubi < gubCENumber; ubi++) {
						for (ubj = 0; ubj < gubBurstsPerBank; ubj++) {
							M_RUT2DTo1D(ubi, (uwj << gubBurstsPerBankLog) + ubj) = M_RUT2DTo1D(ubi, ((uwj + 1) << gubBurstsPerBankLog) + ubj);
						}
					}
				}
				for (ubi = gubSM_GroupNo;  ubi < (gubTotalGroups); ubi++) {
					if (ubi != gubSM_GroupNo) {
						VT->guwDieStartSFUnitIndex[ubi]--;
					}
					//VT->guwExtendUnitsIndex[ubi]--;
					for (ubj = 0; ubj < gubPlanesPerPage; ubj++) {
						VT->guwRUTUnitIndexOfLastBlock[ubj + (M_Multiplier(ubi, DEF_PlanesPerPage_Mcl_Div))] --;
					}
				}
				VT->guwDieStartSFUnitIndex[gubTotalGroups] --;
				VT->guwPutUserFreeToFailHandleAreaCnt[gubSM_GroupNo]--;
			}
		}
	}
#endif
}

void Vender_ReadVTFailLog(void)
{
	if (VT->gulVTLastMark != 0x55AA) {
		gubErrorCode = SET_ABRT;
	}
	else {
		memset(BUFB_BASE, 0, 512);
		UWORD uwFailLogCount, uwEntry;
		UBYTE ubCE;
		for (uwFailLogCount = 0; uwFailLogCount < VTLogEntryNum; uwFailLogCount++) {
			if (VT->guoFailLog[uwFailLogCount].FLHLog.ubLogType == FLHErrLogType) {
				if ((VT->guoFailLog[uwFailLogCount].FLHLog.ubAbnormalLoc < 10) && (VT->guoFailLog[uwFailLogCount].FLHLog.ubAbnormalLoc > 0)) {
					uwEntry = (VT->guoFailLog[uwFailLogCount].FLHLog.uwFEntry % (gubPlanesPerPage * gub4kEntrysPerPlane));
					ubCE = (UBYTE)(uwEntry >> gubBurstsPerBankLog);

					if (ENABLE_VenderCMD_return_PhysicalCE) {
						ubCE = TranslateLogicalCEtoPhysicalCE(ubCE);
					}
					BUFB_BASE[ubCE] ++;
				}
			}
		}
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
		}
		HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	}
}

void Vender_GPIO_Setting(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	SYS0L[SYS0L_GPIO_CTRL4] &= CLR_BIT10;
	if ((gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_WRITEPROTECT_ABORT)
	        || (gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_WRITEPROTECT_DUMMY)) {

		if (gubGPIOEdgeMode == GPIO_EDGE_MODE_RISING_ERASE) {
			if (SYS0L[SYS0L_GPIO_CTRL4] & BIT9) { //GPIO 13 signal
				gubIsWriteProtect |= BIT_GPIO_WRITEPROTECT;
			}
			else {
				gubIsWriteProtect &= ~BIT_GPIO_WRITEPROTECT;
			}
		}
		else {
			if (SYS0L[SYS0L_GPIO_CTRL4] & BIT9) { //GPIO 13 signal
				gubIsWriteProtect &= ~BIT_GPIO_WRITEPROTECT;
			}
			else {
				gubIsWriteProtect |= BIT_GPIO_WRITEPROTECT;
			}
		}
	}
	else if ((gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_USER)
	         || (gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_ALL)) {

		if (gubGPIOEdgeMode == GPIO_EDGE_MODE_RISING_ERASE) {
			if (SYS0L[SYS0L_GPIO_CTRL4] & BIT9) { //GPIO 13 signal
				gubCustomerGPIOEraseEnable = 2;
			}
			else {
				gubCustomerGPIOEraseEnable = 0;
			}
		}
		else {
			if (SYS0L[SYS0L_GPIO_CTRL4] & BIT9) { //GPIO 13 signal
				gubCustomerGPIOEraseEnable = 0;
			}
			else {
				gubCustomerGPIOEraseEnable = 2;
			}

		}
	}
	SYS0L[SYS0L_GPIO_CTRL1] &= CLR_BIT26;//GPIO3 clear

	if ((gubCustomerGPIO3SelectMode == CUSTOMER_GPIO3_MODE_QUICKERASE_USER)
	        || (gubCustomerGPIO3SelectMode == CUSTOMER_GPIO3_MODE_QUICKERASE_ALL)
	        || (gubCustomerGPIO3SelectMode == CUSTOMER_GPIO3_MODE_QUICKERASE_N92)) {

		if (gubGPIO3EdgeMode == GPIO_EDGE_MODE_RISING_ERASE) { 
			if (SYS0L[SYS0L_GPIO_CTRL1] & BIT25) { //GPIO 3 signal
				gubCustomerGPIO3EraseEnable = 2;
			}
			else {
				gubCustomerGPIO3EraseEnable = 0;
			}
		}
		else {
			if (SYS0L[SYS0L_GPIO_CTRL1] & BIT25) { //GPIO 3 signal
				gubCustomerGPIO3EraseEnable = 0;
			}
			else {
				gubCustomerGPIO3EraseEnable = 2;
			}

		}

	}
}

void Vender_GPIO_Erase(UBYTE ubMode)
{

	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (ubMode == 0) { //erase user
		if (ENABLE_CONTINUE_SECURITY_ERASE_AFTER_POWER_CYCLE) {
			AtaCfg.ubSecurityEraseIsDoing = 2;
			SaveAtaConfig();
		}

		TrimAll();
		gubFlushMode = BYTE_STANDBY_FLUSH;
		FlushCache();
		SecurityEraseUserData();

		if (ENABLE_CONTINUE_SECURITY_ERASE_AFTER_POWER_CYCLE) {
			AtaCfg.ubSecurityEraseIsDoing = 0;
		}
		SaveAtaConfig();
	}
	else if (ubMode == 1) { //erase all

		UBYTE ubPlaneBank;
		UBYTE ubCEIndex;
		U32 ulVenderUnit;
		U32 ulUnitIndex;
		FlashQueue_t *FQ;
		UBYTE ubDepth, ubL4kLink;
		UBYTE ubFQIndex[64];

		UBYTE ubSystemUnit;
		UBYTE ubSystemBurst;

		UBYTE ubOldSystemUnit;
		UBYTE ubOldSystemBurst;
		UBYTE ubOldSystemSet;

		UBYTE *ubUpdateBuffer = (UBYTE *)(BUFFER3_BASE);//use gubHostTable
		U32 uli;

		HandleStopRW(1);

		//save erase start tag to system unit
		ubOldSystemSet = gubSystemSet;

		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			ubOldSystemUnit = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] >> gubBurstsPerBankLog);
			ubOldSystemBurst = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] & gubBurstsPerBankMask);
			if (ftlReadSinglePlane(ubOldSystemUnit, 0, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_SYS,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY), 4, 0, ubOldSystemBurst) == 0) {
				break;
			}
		}

		ubUpdateBuffer[M_SysInfo_ubGPIOEraseDoing] = 1;

		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)ubUpdateBuffer, 512 * 32);
		}

		gubSystemSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_TAIL);
		guwSystemBlock[0] = guwSystemBlockRemappingTable[gubSystemSet][0];
		guwSystemBlock[1] = guwSystemBlockRemappingTable[gubSystemSet][1];
		gulSystemVersion = (gulSystemVersion & 0xFFFFFFF0) + ((gulSystemVersion + 1) & 0x0000000F);//ex: 0x112233 -> 0x112234, 0x11223F->0x112230

		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
			ubSystemUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
			ubSystemBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
			ftlWriteSinglePlane(ubSystemUnit, 0, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_SYS, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst);
		}

		ubOldSystemUnit = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][0] >> gubBurstsPerBankLog);
		ubOldSystemBurst = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][0] & gubBurstsPerBankMask);
		for (uli = 1; uli <  3; uli ++) {
			ftlReadSinglePlane(ubOldSystemUnit, uli, 0, (U32)&ubUpdateBuffer[0], MARK_DPST,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubOldSystemBurst);
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
				ubSystemUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
				ubSystemBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
				ftlWriteSinglePlane(ubSystemUnit, uli, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_DPST, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst);
			}
		}

		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
			ubSystemUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
			ubSystemBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
			for (uli = 3; uli <  guwFastPagePagesPerUnit; uli ++) {
				ftlWriteSinglePlane(ubSystemUnit, uli, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_SYS, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst);
			}
			UBYTE ubFQIndex = gFQI.ubFQLinkFree;
			FlashQueue_t *FQ;
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE);

			// Must Send Reset CMD(0xFF) After Erase Fail
			FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

			ubOldSystemUnit = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] >> gubBurstsPerBankLog);
			ubOldSystemBurst = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] & gubBurstsPerBankMask);
			FQ->uwFUnit = ubOldSystemUnit;
			if (ENABLE_PLANE_CH_CE) {
				FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubOldSystemBurst) << gub4kEntrysPerPlaneLog);
			}
			else {
				FQ->ulFEntry = (((ubCEIndex & gubPlanesPerBurstMask) + (ubOldSystemBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
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
		}

		ftlAddToSystemSetFreeTable(ubOldSystemSet);

		//erase all
		for (ulUnitIndex = 0; ulUnitIndex < gulTotalPhyUnitNum; ulUnitIndex++) {
			ulVenderUnit = (gulTotalPhyUnitNum - 1 - ulUnitIndex);
			for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
				if (ENABLE_PLANE_CH_CE) {
					ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
				}
				else {
					ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
				}

				if ((ubPlaneBank < (gubBurstsPerBank * gubPlanesPerBurst)) && ((guwSystemBlock[ubCEIndex] == ((UWORD)((ulVenderUnit << gubBurstsPerBankLog) + (U32)(ubPlaneBank & gubBurstsPerBankMask)))) || (guwDBTBlock[ubCEIndex] == ((UWORD)((ulVenderUnit << gubBurstsPerBankLog) + (U32)(ubPlaneBank & gubBurstsPerBankMask)))))) {
					continue;
				}

				ubFQIndex[ubPlaneBank] = gFQI.ubFQLinkFree;
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);

				// Must Send Reset CMD(0xFF) After Erase Fail
				FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

				FQ->uwFUnit = ulVenderUnit;
				FQ->ulFEntry = ((U32)ubPlaneBank << gub4kEntrysPerPlaneLog);
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
			}
			for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
				if (ENABLE_PLANE_CH_CE) {
					ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
				}
				else {
					ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
				}

				if ((ubPlaneBank < (gubBurstsPerBank * gubPlanesPerBurst)) && ((guwSystemBlock[ubCEIndex] == ((UWORD)((ulVenderUnit << gubBurstsPerBankLog) + (U32)(ubPlaneBank & gubBurstsPerBankMask)))) || (guwDBTBlock[ubCEIndex] == ((UWORD)((ulVenderUnit << gubBurstsPerBankLog) + (U32)(ubPlaneBank & gubBurstsPerBankMask)))))) {
					continue;
				}

				while (gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}
				gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubFQPhase = BYTE_FQ_CHECKDONE;
				if (gFQI.gFQLink[ubFQIndex[ubPlaneBank]].btPEFail) {
					//uoEraseFailMap[ulVenderUnit] |= (BIT0 << ubPlaneBank);
				}
			}
		}

		//save erase end tag to system unit
		ubOldSystemSet = gubSystemSet;

		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			ubOldSystemUnit = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] >> gubBurstsPerBankLog);
			ubOldSystemBurst = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] & gubBurstsPerBankMask);
			if (ftlReadSinglePlane(ubOldSystemUnit, 0, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_SYS,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY), 4, 0, ubOldSystemBurst) == 0) {
				break;
			}
		}

		ubUpdateBuffer[M_SysInfo_ubGPIOEraseDoing] = 0;

		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)ubUpdateBuffer, 512 * 32);
		}

		gubSystemSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_TAIL);
		guwSystemBlock[0] = guwSystemBlockRemappingTable[gubSystemSet][0];
		guwSystemBlock[1] = guwSystemBlockRemappingTable[gubSystemSet][1];
		gulSystemVersion = (gulSystemVersion & 0xFFFFFFF0) + ((gulSystemVersion + 1) & 0x0000000F);//ex: 0x112233 -> 0x112234, 0x11223F->0x112230

		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
			ubSystemUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
			ubSystemBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
			ftlWriteSinglePlane(ubSystemUnit, 0, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_SYS, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst);
		}

		ubOldSystemUnit = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][0] >> gubBurstsPerBankLog);
		ubOldSystemBurst = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][0] & gubBurstsPerBankMask);
		for (uli = 1; uli <  3; uli ++) {
			ftlReadSinglePlane(ubOldSystemUnit, uli, 0, (U32)&ubUpdateBuffer[0], MARK_DPST,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubOldSystemBurst);
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
				ubSystemUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
				ubSystemBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
				ftlWriteSinglePlane(ubSystemUnit, uli, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_DPST, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst);
			}
		}

		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
			ubSystemUnit = (UBYTE)(guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog);
			ubSystemBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
			for (uli = 3; uli <  guwFastPagePagesPerUnit; uli ++) {
				ftlWriteSinglePlane(ubSystemUnit, uli, ubCEIndex, (U32)&ubUpdateBuffer[0], MARK_SYS, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst);
			}
			UBYTE ubFQIndex = gFQI.ubFQLinkFree;
			FlashQueue_t *FQ;
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE);

			// Must Send Reset CMD(0xFF) After Erase Fail
			FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

			ubOldSystemUnit = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][0] >> gubBurstsPerBankLog);
			ubOldSystemBurst = (UBYTE)(guwSystemBlockRemappingTable[ubOldSystemSet][0] & gubBurstsPerBankMask);

			FQ->uwFUnit = ubOldSystemUnit;
			if (ENABLE_PLANE_CH_CE) {
				FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubOldSystemBurst) << gub4kEntrysPerPlaneLog);
			}
			else {
				FQ->ulFEntry = (((ubCEIndex & gubPlanesPerBurstMask) + (ubOldSystemBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
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
		}

		ftlAddToSystemSetFreeTable(ubOldSystemSet);
		gubStopFTL = 0;
		Vender_Isp_Jump(0);
	}
	else if (ubMode == 2) { //N92 erase
		if (ENABLE_CONTINUE_SECURITY_ERASE_AFTER_POWER_CYCLE) {
			AtaCfg.ubSecurityEraseIsDoing = 2;
			SaveAtaConfig();
		}

		TrimAll();
		gubFlushMode = BYTE_STANDBY_FLUSH;
		FlushCache();
		SecurityEraseUserData();
		FillWholeDisk(0x21212121);
		if (ENABLE_CONTINUE_SECURITY_ERASE_AFTER_POWER_CYCLE) {
			AtaCfg.ubSecurityEraseIsDoing = 0;
		}
		SaveAtaConfig();
	}
}

void Vender_GPIO_Offline()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

#if (!BURNER)
	ULLONG uoTimeInterval;
	if (gubCustomerGPIOEraseEnable == 1) {
		if (gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_USER) {
			Vender_GPIO_Erase(0);
		}
		else if (gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_ALL) {
			Vender_GPIO_Erase(1);
		}
		gubCustomerGPIOEraseEnable = 2;
	}
	else if (gubCustomerGPIO3EraseEnable == 1) {

		gubEraseLightTwinkle = 1; //閃燈
		uoTimeInterval = (GetRTT_milis() - guoCustomerGPIO3Timer);

		if (uoTimeInterval > 1000) {

			if (gubCustomerGPIO3SelectMode == CUSTOMER_GPIO3_MODE_QUICKERASE_USER) {
				Vender_GPIO_Erase(0);
			}
			else if (gubCustomerGPIO3SelectMode == CUSTOMER_GPIO3_MODE_QUICKERASE_ALL) {
				Vender_GPIO_Erase(1);
			}
			else if (gubCustomerGPIO3SelectMode == CUSTOMER_GPIO3_MODE_QUICKERASE_N92) {
				Vender_GPIO_Erase(2);
			}
			gubCustomerGPIO3EraseEnable = 2;

		}
		gubEraseLightTwinkle = 0; //停燈
	}
#endif
}
void FillWholeDisk(U32 ulPattern)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

#if(!BURNER)
	U32 ulEndLBA;
	BufferQueue_t *BQ;
	UBYTE ub4kIndex;
	UBYTE ub4kNum;
	U32 ubTotalSectors;
	UWORD uwBufferPTR;
	U32 ulBuffer;
	U32 ulDataMask;
	U32 ulCurrentLBA;

	HandleStopRW(0);
	M_SetFlagSettingNone();

	ulEndLBA = gulMediaSize.All - 1;
	uwBufferPTR = 256;
	ulBuffer = SRAM_BASE + uwBufferPTR * 512;
	ulCurrentLBA = 0;

	while (ulCurrentLBA <= ulEndLBA) {
		if ((ulCurrentLBA + gubSectorsPerPlane) <= (ulEndLBA + 1)) {
			ub4kNum = gub4kEntrysPerPlane;
			ulDataMask = 0xFFFFFFFF;
		}
		else {
			ub4kNum = (ulEndLBA  - ulCurrentLBA) / 8 + 1;
			ulDataMask = (BIT0 << (ub4kNum * 8)) - 1;
		}

		if (ENABLE_DEBUG_UART_ATA_CMD) {
			if ((ulCurrentLBA % 0x1000) == 0) {
				UartString("\r\nS LBA: ");
				UartLongHex(ulCurrentLBA);
			}
		}

		if (ulPattern == 0xFFFFFFFF) {
			mDMAC_SV(4, ulBuffer, ub4kNum * 4096, ulCurrentLBA, 0, 0, (DMA_EN_RAND | DMA_EN_SETCQ), 1);
		}
		else {
			mDMAC_SV(4, ulBuffer, ub4kNum * 4096, ulCurrentLBA, ulPattern, 0, DMA_EN_SETCQ, 1);
		}

		while (gBQI.ubLinkNum >= BQ_NUM)M_SwitchTask();
		gBQI.ubLinkActive = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
		BQ = &gBQI.BufferQueue[gBQI.ubLinkActive];
		BQ->btWrite = 1;
		BQ->ulVir4kIndex = (ulCurrentLBA >> 3);
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
		gBQI.ubWLinkNum++;//W link要同步一起加
		M_AddToBufferQueue(gBQI.ubLinkActive);
		gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
		while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}
		ulCurrentLBA += gubSectorsPerPlane;
	}

	mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 0);//強迫DMAC 全部做完
	M_SetFlagSettingSHF();
#endif
}

#if PH_SQL_FUNC
void Vender_Mode_Cmd_VNandTool(UBYTE ubMode) // 0 write ddr, 1 read ddr, 2 cmd trigger, 3 Get configure, 4 Set configure
{
	U32 ulDataTriggerOffset;
	UBYTE ubCE, ubChannel;
	if ((ubMode == 0) || (ubMode == 1)) {
		gubUsedDataSectorsInBuffer = gulSectorCnt;
		if (ubMode == 1) {
			if (HB[HB_LBA_H] == 0) {
				ulDataTriggerOffset = ((U32)gubNandVToolDataBuffer - (U32)gubNandVToolCmdBuffer) / 512;
			}
			else if (HB[HB_LBA_H] == 1) {
				ulDataTriggerOffset = 0;
			}
			else if (HB[HB_LBA_H] == 2) {
				ulDataTriggerOffset = ((U32)gubNandVToolECCBuffer - (U32)gubNandVToolCmdBuffer) / 512;
			}
			else if (HB[HB_LBA_H] == 3) {
				ulDataTriggerOffset = ((U32)gulNandVToolTimerCntBuffer - (U32)gubNandVToolCmdBuffer) / 512;
			}
			else if (HB[HB_LBA_H] == 4) {
				ulDataTriggerOffset = ((U32)gulNandVToolTimerCntBuffer - (U32)gubNandVToolCmdBuffer) / 512;
			}
		}
		else {
			ulDataTriggerOffset = ((U32)gubNandVToolDataBuffer - (U32)gubNandVToolCmdBuffer) / 512;
		}
	}
	else if (ubMode == 2) {
		gubUsedCmdSectorsInBuffer = gulSectorCnt;
		ulDataTriggerOffset = 0;
		ubCE = HB[HB_LBA_M];
		ubChannel = ubCE & gubPlanesPerBurstMask;
		gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
		M_VirtualToPhysicalCE(ubCE);
		FCONL[FCONL_FCE_ENB] = 0;
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][(ubCE / gubPlanesPerBurst)]); // Enable selected CE
		}
		else {
			FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCE / gubPlanesPerBurst); // Enable selected CE
		}
		if (ENABLE_DEBUG_UART_PH_SQL) {
			UartString("\n\rCE_Set:");
			UartLongHex(FCONL[FCONL_FCE_ENB]);
		}
	}
	else if (ubMode == 3) {
		ulDataTriggerOffset = 0;
		NandVToolGetFLHIP();
	}
	else if (ubMode == 4) {
		ulDataTriggerOffset = 0;
	}
	while (gulSectorCnt) {
		HL[HL_BUF_TRG_SEC] = gulSectorCnt;
		HL[HL_BUF_OFST] = ulDataTriggerOffset;
		HB[HB_DATA_TRG] = H_DATA_TRG; // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
		gulSectorCnt = 0;
	}
	if (ubMode == 2) {
		NandVToolPreSettingParser();
	}
	else if (ubMode == 4) {
		NandVToolSetFLHIP();
	}
	else if (ubMode == 1) {
		if (HB[HB_LBA_H] == 4) {
			gulBusyTimer = 0;
		}
	}
}
#endif

void Vender_ReadFLHBlockMappingInfo(void)
{

	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	mDMAC_SV(4, (U32)&gubflaBuffer[0],  ((4 * gulPhyBlocksPerCE + 4095) / 4096) * 4096, 0, 0, 0, DMA_EN_SETCQ, 0);
	U32 ulBufferOffset;
	UWORD uwCERUTPtr, uwCERUTBound, uwPhyBlockByRUT, uwFUnit,  uwMapBlockIndex, uwPageCount;
	UBYTE ubBlockMappingMapIndex, ubNowUseBlockMappingMapIndex, ubTargetIndex, ubTotalTargetNum, ubRBMStartCE, ubActiveUnitFlag;
	ubRBMStartCE = HB[HB_LBA_M];
	ubBlockMappingMapIndex = 0; //一個 Map 1024個 block 一個 block使用4byte
	ubNowUseBlockMappingMapIndex = 0;
	ulBufferOffset = (U32)ubNowUseBlockMappingMapIndex * 4096;
	mDMAC_COPY((U32)&gubflaBuffer[ulBufferOffset], (U32)BUFB_BASE, 4096, 0, 0, 0, 0, 1);
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 4096);
	}
	if (ubRBMStartCE < gubPlanesPerBurst) {
		uwMapBlockIndex = guwSystemBlock[ubRBMStartCE] & 1023;
		BUFL_BASE[uwMapBlockIndex] = SLC_Attribute | CODE_BLOCK_Attribute;
		uwPageCount = guwFastPagePagesPerUnit;
		BUFL_BASE[uwMapBlockIndex] |= (U32)uwPageCount << 8;
		uwMapBlockIndex = guwSysCodeBlock[0][ubRBMStartCE] & 1023;
		BUFL_BASE[uwMapBlockIndex] = SLC_Attribute | CODE_BLOCK_Attribute;
		uwPageCount = guwFastPagePagesPerUnit;
		BUFL_BASE[uwMapBlockIndex] |= (U32)uwPageCount << 8;
		uwMapBlockIndex = guwSysCodeBlock[1][ubRBMStartCE] & 1023;
		BUFL_BASE[uwMapBlockIndex] = SLC_Attribute | CODE_BLOCK_Attribute;
		uwPageCount = guwFastPagePagesPerUnit;
		BUFL_BASE[uwMapBlockIndex] |= (U32)uwPageCount << 8;

		UBYTE ub4KEntryCnt;
		uwMapBlockIndex = guwDBTBlock[ubRBMStartCE] & 1023;
		BUFL_BASE[uwMapBlockIndex] = SLC_Attribute | CODE_BLOCK_Attribute;
		ub4KEntryCnt = (gulPhyBlocksPerCE * gubCENumber + 4095) / 4096;
		uwPageCount = (ub4KEntryCnt + gub4kEntrysPerPlaneMask) / gub4kEntrysPerPlane;
		BUFL_BASE[uwMapBlockIndex] |= (U32)uwPageCount << 8;
	}
	else {
		uwMapBlockIndex = (guwSystemBlock[ubRBMStartCE] << gubBurstsPerBankLog) & 1023;
		BUFL_BASE[uwMapBlockIndex] = NOUSE_BLOCK_Attribute;
		uwMapBlockIndex = (guwSysCodeBlock[0][ubRBMStartCE] << gubBurstsPerBankLog) & 1023;
		BUFL_BASE[uwMapBlockIndex] = NOUSE_BLOCK_Attribute;
		uwMapBlockIndex = (guwSysCodeBlock[1][ubRBMStartCE] << gubBurstsPerBankLog) & 1023;
		BUFL_BASE[uwMapBlockIndex] = NOUSE_BLOCK_Attribute;
		uwMapBlockIndex = (guwDBTBlock[ubRBMStartCE] << gubBurstsPerBankLog) & 1023;
		BUFL_BASE[uwMapBlockIndex] = NOUSE_BLOCK_Attribute;
	}

	uwCERUTBound = guwFWTotalUnitNum * gubBurstsPerBank;
	for (uwCERUTPtr = 0; uwCERUTPtr < uwCERUTBound; uwCERUTPtr++) {
		if ((uwCERUTPtr & gubBurstsPerBankMask) == 0) { //When this ptr is plane A
			uwFUnit = uwCERUTPtr >> gubBurstsPerBankLog;
			ubActiveUnitFlag = 0;
		}
		if (gubRUTMixPlaneEnable) {
			uwPhyBlockByRUT = M_RUT2DTo1D(ubRBMStartCE, uwCERUTPtr);
		}
		else {
			uwPhyBlockByRUT =  (M_RUT2DTo1D(ubRBMStartCE, uwCERUTPtr) << gubBurstsPerBankLog) + (uwCERUTPtr & gubBurstsPerBankMask);
		}
		ubBlockMappingMapIndex = uwPhyBlockByRUT / 1024;
		uwMapBlockIndex =  uwPhyBlockByRUT & 1023;
		if (ubBlockMappingMapIndex != ubNowUseBlockMappingMapIndex) {
			ulBufferOffset = (U32)ubNowUseBlockMappingMapIndex * 4096;
			mDMAC_COPY((U32)BUFB_BASE , (U32)&gubflaBuffer[ulBufferOffset], 4096, 0, 0, 0, 0, 1);
			ulBufferOffset = (U32)ubBlockMappingMapIndex * 4096;
			mDMAC_COPY((U32)&gubflaBuffer[ulBufferOffset], (U32)BUFB_BASE, 4096, 0, 0, 0, 0, 1);
			ubNowUseBlockMappingMapIndex = ubBlockMappingMapIndex;
			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate(BUFB_BASE, 4096);
			}
		}

		//if ((gulVC[uwFUnit].B.ulValidCount == DefaultVC) || (gulVC[uwFUnit].B.ulValidCount == 0)) {
		if (gulVC[uwFUnit].B.ulValidCount == DefaultVC) {
			//Check guwTableTarget total MAX_TABLEUNIT
			ubTotalTargetNum = MAX_TABLEUNIT;
			for (ubTargetIndex = 0; ubTargetIndex < ubTotalTargetNum; ubTargetIndex++) {
				if (uwFUnit == VT->guwTableTarget[ubTargetIndex].B.uwTarget) {
					if (guwTable_VC[ubTargetIndex]) {
						if (ubTargetIndex == VT->gubTableTargetIndex) {
							uwPageCount = VT->guwTableTargetPTR / gubPlanesPerPage; //VT->guwTableTargetPTR / guw4kEntrysPerPage;
							if ((M_Modular(VT->guwTableTargetPTR, DEF_PlanesPerPage_Mod_Mask)) > (ubRBMStartCE * gubBurstsPerBank + (uwCERUTPtr & gubBurstsPerBankMask))) {
								uwPageCount++;
							}
						}
						else {
							uwPageCount = guwFastPagePagesPerUnit;
						}
						BUFL_BASE[uwMapBlockIndex] = SLC_Attribute | TABLE_BLOCK_Attribute | ((U32)uwPageCount << 8);
						ubActiveUnitFlag = 1;
						//UartString("\n\r\n\rValidTableUnit");
						//UartString("  Unit:0x");
						//UartWordHex(uwFUnit);
						goto Check_Table_Unit_End;
					}
				}
			}
			//Check guwRUTTarget
			if (uwFUnit == VT->guwRUTTarget.B.uwTarget) {
				uwPageCount = (RUTSize / 512 / gubSectorsPerPlane) / gubPlanesPerPage;
				if ((M_Modular((RUTSize / 512 / gubSectorsPerPlane), DEF_PlanesPerPage_Mod_Mask)) > (ubRBMStartCE * gubBurstsPerBank + (uwCERUTPtr & gubBurstsPerBankMask))) {
					uwPageCount++;
				}
				BUFL_BASE[uwMapBlockIndex] = SLC_Attribute | TABLE_BLOCK_Attribute | ((U32)uwPageCount << 8);
				ubActiveUnitFlag = 1;
				//UartString("\n\rRUTUnit");
				//UartString("  Unit:0x");
				//UartWordHex(uwFUnit);
				goto Check_Table_Unit_End;
			}
			//Check guwInitInfoTarget
			if (uwFUnit == VT->guwInitInfoTarget.B.uwTarget) {
				uwPageCount = VT->guwInitInfoTargetPTR / gubPlanesPerPage;
				if ((M_Modular(VT->guwInitInfoTargetPTR, DEF_PlanesPerPage_Mod_Mask)) > (ubRBMStartCE * gubBurstsPerBank + (uwCERUTPtr & gubBurstsPerBankMask))) {
					uwPageCount++;
				}
				BUFL_BASE[uwMapBlockIndex] = SLC_Attribute | TABLE_BLOCK_Attribute | ((U32)uwPageCount << 8);
				ubActiveUnitFlag = 1;
				//UartString("\n\rInitInfoUnit");
				//UartString("  Unit:0x");
				//UartWordHex(uwFUnit);
				goto Check_Table_Unit_End;
			}
			//Check guwVTAreaFreeTarget VTAREA_UNITS
			if (uwFUnit == VT->guwVTAreaFreeTarget[VT->gubVTAreaFreeQueueInPTR].B.uwTarget) {
				uwPageCount = VT->guwVTPTR / gubPlanesPerPage;
				if ((M_Modular(VT->guwVTPTR, DEF_PlanesPerPage_Mod_Mask)) > (ubRBMStartCE * gubBurstsPerBank + (uwCERUTPtr & gubBurstsPerBankMask))) {
					uwPageCount++;
				}
				BUFL_BASE[uwMapBlockIndex] = SLC_Attribute | TABLE_BLOCK_Attribute | ((U32)uwPageCount << 8);
				ubActiveUnitFlag = 1;
				//UartString("\n\rVTUnit");
				//UartString("  Unit:0x");
				//UartWordHex(uwFUnit);
				goto Check_Table_Unit_End;
			}
			//Check guwVTChildTarget
			if (uwFUnit == VT->guwVTChildTarget.B.uwTarget) {
				uwPageCount = VT->guwVTChildPTR / gubPlanesPerPage;
				if ((M_Modular(VT->guwVTChildPTR, DEF_PlanesPerPage_Mod_Mask)) > (ubRBMStartCE * gubBurstsPerBank + (uwCERUTPtr & gubBurstsPerBankMask))) {
					uwPageCount++;
				}
				BUFL_BASE[uwMapBlockIndex] = SLC_Attribute | TABLE_BLOCK_Attribute | ((U32)uwPageCount << 8);
				ubActiveUnitFlag = 1;
				//UartString("\n\rVTChildUnit");
				//UartString("  Unit:0x");
				//UartWordHex(uwFUnit);
				goto Check_Table_Unit_End;
			}
		Check_Table_Unit_End:
			if (ubActiveUnitFlag == 0) {
				BUFL_BASE[uwMapBlockIndex] = FREE_BLOCK_Attribute;
			}
		}
		else { //This unit is used by data
			if (gulVC[uwFUnit].B.btFastPage) {
				BUFL_BASE[uwMapBlockIndex] = SLC_Attribute;
				uwPageCount = guwFastPagePagesPerUnit;
			}
			else if (guoFlashUseType.B.bt4LC) {
				BUFL_BASE[uwMapBlockIndex] = MLC_Attribute;
				uwPageCount = guwPagesPerUnit;
			}
			else {
				BUFL_BASE[uwMapBlockIndex] = TLC_Attribute;
				uwPageCount = guwPagesPerUnit;
			}
			//Check guwGRTarget only check total (VT->gubGRTargetIndex+1)
			ubTotalTargetNum = VT->gubGRTargetIndex + 1;
			for (ubTargetIndex = 0; ubTargetIndex < ubTotalTargetNum; ubTargetIndex++) {
				if (uwFUnit == VT->guwGRTarget[(ubTotalTargetNum - 1)].B.uwTarget) {
					uwPageCount = VT->gulGRTargetPTR / gubPlanesPerPage;
					if ((M_Modular(VT->gulGRTargetPTR, DEF_PlanesPerPage_Mod_Mask)) > (ubRBMStartCE * gubBurstsPerBank + (uwCERUTPtr & gubBurstsPerBankMask))) {
						uwPageCount++;
					}
					BUFL_BASE[uwMapBlockIndex] |= CACHE_BLOCK_Attribute | ((U32)uwPageCount << 8);
					ubActiveUnitFlag = 1;
					//UartString("\n\rGRUnit");
					//UartString("  Unit:0x");
					//UartWordHex(uwFUnit);
					goto Check_Data_Unit_End;
				}
				else if (uwFUnit == VT->guwGRTarget[ubTargetIndex].B.uwTarget) {
					BUFL_BASE[uwMapBlockIndex] |= CACHE_BLOCK_Attribute | ((U32)uwPageCount << 8);
					ubActiveUnitFlag = 1;
					//UartString("\n\rGRUnit");
					//UartString("  Unit:0x");
					//UartWordHex(uwFUnit);
					goto Check_Data_Unit_End;
				}
			}
			//Check guwGCGRTarget only check total (VT->gubGCGRTargetIndex+1)
			ubTotalTargetNum = VT->gubGCGRTargetIndex + 1;
			for (ubTargetIndex = 0; ubTargetIndex < ubTotalTargetNum; ubTargetIndex++) {
				if (uwFUnit == VT->guwGCGRTarget[(ubTotalTargetNum - 1)].B.uwTarget) {
					uwPageCount = VT->gulGCGRTargetPTR / gubPlanesPerPage;
					if ((M_Modular(VT->gulGCGRTargetPTR, DEF_PlanesPerPage_Mod_Mask)) > (ubRBMStartCE * gubBurstsPerBank + (uwCERUTPtr & gubBurstsPerBankMask))) {
						uwPageCount++;
					}
					BUFL_BASE[uwMapBlockIndex] |= CACHE_BLOCK_Attribute | ((U32)uwPageCount << 8);
					ubActiveUnitFlag = 1;
					//UartString("\n\rGCGRUnit");
					//UartString("  Unit:0x");
					//UartWordHex(uwFUnit);
					goto Check_Data_Unit_End;
				}
				else if (uwFUnit == VT->guwGCGRTarget[ubTargetIndex].B.uwTarget) {
					BUFL_BASE[uwMapBlockIndex] |= CACHE_BLOCK_Attribute | ((U32)uwPageCount << 8);
					ubActiveUnitFlag = 1;
					//UartString("\n\rGCGRUnit");
					//UartString("  Unit:0x");
					//UartWordHex(uwFUnit);
					goto Check_Data_Unit_End;
				}
			}
		Check_Data_Unit_End:
			if (ubActiveUnitFlag == 0) {
				BUFL_BASE[uwMapBlockIndex] |= DATA_BLOCK_Attribute | ((U32)uwPageCount << 8);
				if (gulVC[uwFUnit].B.ulValidCount == 0) {
					BUFL_BASE[uwMapBlockIndex] = FREE_BLOCK_Attribute;
				}
				//UartString("\n\rOldDataUnit");
				//UartString("  Unit:0x");
				//UartWordHex(uwFUnit);
			}
		}
	}
	ulBufferOffset = (U32)ubNowUseBlockMappingMapIndex * 4096;
	mDMAC_COPY((U32)BUFB_BASE , (U32)&gubflaBuffer[ulBufferOffset], 4096, 0, 0, 0, 0, 1);
	ubNowUseBlockMappingMapIndex = 0;
	//Receive header from host
	while (gulSectorCnt) {
		ulBufferOffset = ubNowUseBlockMappingMapIndex * 4096;
		mDMAC_COPY( (U32)&gubflaBuffer[ulBufferOffset], (U32)BUFB_BASE , 4096, 0, 0, 0, 0, 1);
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 4096);
		}
		HL[HL_BUF_OFST] = 0;
		//HW[HW_SECTOR_CNT] = gulSectorCnt;
		HL[HL_BUF_TRG_SEC] = 8;
		//	//HL[HL_LAST_END_LBA] =0;
		HL[HL_TQ_CONTENT_31] = 0;
		Vender_HostRemainSectorCount();
		gulSectorCnt -=  8;
		ubNowUseBlockMappingMapIndex++;
	}

}

void Vender_GetErrorInfoBlock()
{
	M_CheckNonBurnerModeToDisableFunction();
	UBYTE ubSystem_DBT_Exist = 0, ubBlockPTR = 0;
	Variable_Table *VT_temp = (Variable_Table *)(&gubZ2Buffer[0]);
	RMA_Info_t  *RMA_Info = (RMA_Info_t *)BUFFER3_BASE;
	UBYTE *ubDBTBuffer = (UBYTE*) gubHostTable;
	UWORD uwUnitIndex;
	U32 ulUnitEraseCnt, ulCount, ulTotalBlockNum, ulEntryPerDBTPlane, ulDBTEntryIndex;
	guwSystemAreaNum[0] = 0xFFFF;
	guwErrorDumpBlock[0] = 0xFFFF;
	gubPreformat = 1;
	//	gubReadHBRetry_Enable = 1;
	//	if (ftlCheckFlashFormat_SystemSet() == 0) {
	//		memcpy((void *)&gubLastFWVer[0], (void*)&ubCopyBuffer[M_SysInfo_FW_Info1], 0x8);
	//		gubSystemAreaNum = ubCopyBuffer[M_SysInfo_SystemAreaNum];
	//	}
	if ( RDT_FindDBTBlock_Task()) {
		ubSystem_DBT_Exist &= ~BIT1;
	}
	else {
		ubSystem_DBT_Exist |= BIT1;
	}

	if (gubSSD_Init_State.B.ubState < BYTE_INIT_SCAN_DONE) {
		if (Load_RUT_EC_Table()) {
			ubSystem_DBT_Exist |= BIT0;
		}
	}
	if (gubSSD_Init_State.B.ubState == BYTE_INIT_START_SCAN) {
		while (gubSSD_Init_State.B.ubState < BYTE_INIT_TABLE_DONE) {
			M_SwitchTask();
		}
	}


	if (gubRUT_EC_set & BIT0) {
		ubSystem_DBT_Exist |= BIT3;
	}

	do {//原本就只讀CE0
		if (ftlReadSinglePlane(ubBlockPTR, 0, 0, (U32)VT_temp, MARK_ErrorDumpUnit,  (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), gub4kEntrysPerPlane, 1, 0) == 0) {
			guwErrorDumpBlock[0] = ubBlockPTR;

			break;
		}
		else {
			ftlPreformatShowError(0, ubBlockPTR, 0xC);
		}
		ubBlockPTR++;
	}
	while ((guwErrorDumpBlock[0] == 0xFFFF) && (ubBlockPTR < guwSystemAreaNum[0])); //amour add

	gubErrorCode = 0;
	if (guwErrorDumpBlock[0] == 0xFFFF) {
		ubSystem_DBT_Exist |= BIT4;
	}
	else {
		memcpy((void *)VT, (void*)VT_temp, 4096);
	}

	if (VT->gulVTLastMark == 0x55AA) {
		ubSystem_DBT_Exist |= BIT2;
	}
	//gubSaveVTSerialNumber = VT_temp->gubSaveVTSerialNumber;
	//	gubReadHBRetry_Enable = 0;

	mDMAC_SV(1, (U32)&BUFB_BASE[0],  (U32)gubSectorsPerPlane * 512, 0, 0, 0, DMA_EN_SETCQ, 1);

	//=====================================EC=======================================================
	if (ubSystem_DBT_Exist & BIT3) {
		U32 ulSumEC = 0;
		RMA_Info->ulMaxEC_D1 = 0;
		RMA_Info->ulMaxEC_D3 = 0;
		for (uwUnitIndex = 0; uwUnitIndex < guwFWTotalUnitNum; uwUnitIndex++) {
			ulUnitEraseCnt = gulEC_D3[uwUnitIndex].B.ulEraseCount;
			ulSumEC += gulEC_D3[uwUnitIndex].B.ulEraseCount;

			if (ulUnitEraseCnt > RMA_Info->ulMaxEC_D3) {
				RMA_Info->ulMaxEC_D3 = ulUnitEraseCnt;
			}

		}

		RMA_Info->ulTotalEC_D3 = ulSumEC;
		if (guwFWTotalUnitNum) {
			RMA_Info->ulAvgEC_D3 = ulSumEC / guwFWTotalUnitNum;
		}

		if (TLC) {
			ulSumEC = 0;
			for (uwUnitIndex = 0; uwUnitIndex < VT->gubD1UnitNum; uwUnitIndex++) {
				ulUnitEraseCnt = gulEC_D1[uwUnitIndex].B.ulEraseCount;
				ulSumEC += gulEC_D1[uwUnitIndex].B.ulEraseCount;

				if (ulUnitEraseCnt > RMA_Info->ulMaxEC_D1) {
					RMA_Info->ulMaxEC_D1 = ulUnitEraseCnt;
				}
			}
			RMA_Info->ulTotalEC_D1 = ulSumEC;
			if (VT->gubD1UnitNum) {
				RMA_Info->ulAvgEC_D1 = ulSumEC / VT->gubD1UnitNum;
			}
		}
	}

	//====================================dbt===============================================
	if (ubSystem_DBT_Exist & BIT1) {
		ulEntryPerDBTPlane = (U32)(gubSectorsPerPlane * 512);
		ulTotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;
		for (ulCount = 0; ulCount < ulTotalBlockNum; ulCount++) {
			ulDBTEntryIndex = (ulCount % ulEntryPerDBTPlane);
			if (ulDBTEntryIndex == 0) {
				if (ftlLoadDBT(ulCount, (U32)&ubDBTBuffer[0])) { // load single plane
					ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
				}
			}

			///Read Fail Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & 0x01) {
				RMA_Info->ulReadFail++;
			}

			///Prog Fail Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & 0x02) {
				RMA_Info->ulProgeramFail++;
			}

			///Erase Fail Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & 0x04) {
				RMA_Info->ulEraseFail++;
			}

			/// EarlyBad Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & 0xC0) {
				RMA_Info->ulEarlyBad++;
			}

			//LaterBad Cnt
			if (ubDBTBuffer[ulDBTEntryIndex] & 0x30) {
				RMA_Info->ulLaterBad++;
			}

		}
	}
	//===================================VT======================================
	if (ubSystem_DBT_Exist & BIT2) {
		RMA_Info->ulTotalPowerOn_Hour = VT->gulPowerOnTimeInMinutes / 60;
		RMA_Info->ulPowerOnCnt = VT->gulPowerOnCnt;
		RMA_Info->ulPowerCycleCnt = VT->gulPowerCyclingCnt;
#if(TLC)
		RMA_Info->ulTotalWrite_1G_nand = ((	VT->guoTotalNandWritePlaneCnt_D3SLCTable
		                                    + 	VT->guoTotalNandWritePlaneCnt_D3SLCData
		                                    + 	VT->guoTotalNandWritePlaneCnt_D3TLCData)
		                                  * gubSectorsPerPlane) >> 21;

#else
		RMA_Info->ulTotalWrite_1G_nand = VT->guoTotalNandWriteSectorCnt >> 21;
#endif
		RMA_Info->ulTotalRead_1G_nand = VT->guoTotalNandReadSectorCnt >> 21;
		RMA_Info->ulTotalWrite_1G = VT->guoTotalHostWriteSectorCnt >> 21;
		RMA_Info->ulTotalRead_1G = VT->guoTotalHostReadSectorCnt >> 21;
		RMA_Info->ulFWUpdate = VT->guwFWUpdateCount;
		RMA_Info->ulWriteCRC = VT->gulWriteCRCCnt;
		RMA_Info->ulReadCRC = VT->gulReadCRCCnt;
		RMA_Info->ulHostUNC = VT->gulHostReadECCCnt;
		RMA_Info->ubSaveVTSerialNumber = VT->gubSaveVTSerialNumber;

	}
	RMA_Info->ubTableCheck = ubSystem_DBT_Exist;
	gubPreformat = 0;
	//memcpy((void *)&(RMA_Info->u8bPhIsOnInfoMark[0]), "PhSiOn", 6);
	//memcpy((void *)&(RMA_Info->ubLastFWVer[0]), (void *)&gubLastFWVer[0], 0x8);
	//RMA_Info->gubSaveVTSerialNumber = gubSaveVTSerialNumber;
}

#if (MicronFlashOnly && B0KB )
void B0KB_CheckPage(CB_Info_t *FQI_CB_Info)
{
	if ( FQI_CB_Info->uwProgramOrderIndex < 16) {
		FQI_CB_Info->ubProgramOrder = 1;	//belong to 1st pass,
		//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
		//UartString(" L\r");
	}
	else if ( (FQI_CB_Info->uwProgramOrderIndex >= 16) && (FQI_CB_Info->uwProgramOrderIndex < 112)) {
		FQI_CB_Info->ubProgramOrder = 1;	//belong to 1st pass,
		//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
		//UartString(" L or M\r");
	}
	else if ( (FQI_CB_Info->uwProgramOrderIndex >= 112) && (FQI_CB_Info->uwProgramOrderIndex < 208)) {
		if ( ((FQI_CB_Info->uwProgramOrderIndex - 112) % 3) == 0) {
			FQI_CB_Info->ubProgramOrder = DIS_PreRead ? 3 : 2;	//belong to 2nd pass, need to program 3 pages.
			//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
			//UartString(" eX ");

			FQI_CB_Info->uwLPage = ((FQI_CB_Info->uwProgramOrderIndex - 112) / 3 * 2) + 48;
			FQI_CB_Info->uwUPage = FQI_CB_Info->uwLPage + 1;
			//UartWordHex(FQI_CB_Info->uwLPage);
			//UartString(" L ");
			//UartWordHex(FQI_CB_Info->uwUPage);
			//UartString(" M\r");
		}
		else if (((FQI_CB_Info->uwProgramOrderIndex - 112) % 3) == 1) {
			FQI_CB_Info->ubProgramOrder = 1;	//belong to 1st pass,
			//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
			//UartString(" L\r");
		}
		else {
			FQI_CB_Info->ubProgramOrder = 1;	//belong to 1st pass,
			//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
			//UartString(" M\r");
		}
	}
	else if ( (FQI_CB_Info->uwProgramOrderIndex >= 208) && (FQI_CB_Info->uwProgramOrderIndex < 1505)) {
		if ( ((FQI_CB_Info->uwProgramOrderIndex - 208) % 3) == 0) {
			FQI_CB_Info->ubProgramOrder = DIS_PreRead ? 3 : 2;	//belong to 2nd pass, need to program 3 pages.
			//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
			//UartString(" eX ");

			FQI_CB_Info->uwLPage = ((FQI_CB_Info->uwProgramOrderIndex - 208)) + 113;
			FQI_CB_Info->uwUPage = FQI_CB_Info->uwLPage + 1;
			//UartWordHex(FQI_CB_Info->uwLPage);
			//UartString(" L ");
			//UartWordHex(FQI_CB_Info->uwUPage);
			//UartString(" M\r");
		}
		else if (((FQI_CB_Info->uwProgramOrderIndex - 208) % 3) == 1) {
			FQI_CB_Info->ubProgramOrder = 1;	//belong to 1st pass,
			//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
			//UartString(" L\r");
		}
		else {
			FQI_CB_Info->ubProgramOrder = 1;	//belong to 1st pass,
			//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
			//UartString(" M\r");
		}
	}
	else if ( (FQI_CB_Info->uwProgramOrderIndex >= 1505) && (FQI_CB_Info->uwProgramOrderIndex < 1536)) {
		if ( ((FQI_CB_Info->uwProgramOrderIndex - 1504) % 2) == 0) {
			FQI_CB_Info->ubProgramOrder = DIS_PreRead ? 3 : 2;	//belong to 2nd pass, need to program 3 pages.
			//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
			//UartString(" eX ");

			FQI_CB_Info->uwLPage = ((FQI_CB_Info->uwProgramOrderIndex - 1504) / 2 * 3) + 1409;
			FQI_CB_Info->uwUPage = FQI_CB_Info->uwLPage + 1;
			//UartWordHex(FQI_CB_Info->uwLPage);
			//UartString(" L ");
			//UartWordHex(FQI_CB_Info->uwUPage);
			//UartString(" M\r");
		}
		else if ( ((FQI_CB_Info->uwProgramOrderIndex - 1504) % 2) == 1) {
			FQI_CB_Info->ubProgramOrder = 1;	//belong to 1st pass,
			//UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
			//UartString(" L\r");
		}
		else {
			while (1);
		}
	}
	else {
		while (1);
	}
}
#endif
#if ((!BURNER) && TLC && 0)
void Vender_CopyBack()
{
	UBYTE ubMode, ubA2cmd = 0;
	UBYTE ubIsFastpage;
	UBYTE ubL4KNum = (gubSectorsPerPlane >> SectorsPer4KLog);
	UBYTE ubPhyOrFWMode; // 1: physical addr mode, 0: FW mode
	UBYTE ubWholePage = 0;
	UBYTE ubPhysicalCE, ubLogicalCE;
	UBYTE ubUseRUT = 0;
	UWORD uwPhysicalBlock, uwPhysicalPage, uwPhysicalSector;
	U32 ulVenderSector;
	U32 ulVenderUnit;
	UWORD uwFUnit /*D3*/;
	UWORD uwFD1Unit /*D1*/;
	UWORD uwStartProgramOrderIndex ;
	UWORD uwEndProgramOrderIndex ;


	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
	}


	//UartString("\r\n==== Vender CopyBack====   ");
	UartLongHex(gulSectorCnt);

	ubA2cmd = (HB[HB_LBA_M] >> 7);
	ubPhyOrFWMode = HB[HB_LBA_H];

	if (ubPhyOrFWMode) {
		ubWholePage = HB[HB_LBA_L];
		ubPhysicalCE = BUFB_BASE[20];
		uwPhysicalBlock = BUFW_BASE[11];
		uwPhysicalPage =  BUFW_BASE[12];
		uwPhysicalSector = BUFW_BASE[13];

		ubLogicalCE = (ubPhysicalCE >> 3) + ((ubPhysicalCE % 8) << gubPlanesPerBurstLog); //to be change
		ulVenderUnit = (uwPhysicalBlock >> gubBurstsPerBankLog) ; //還沒處理multi group
		if (ENABLE_PLANE_CH_CE) {
			ulVenderSector = (U32)(uwPhysicalSector & gubSectorsPerPlaneMask) + (U32)(((M_Multiplier((U32)uwPhysicalPage, DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubLogicalCE << gubBurstsPerBankLog) + (uwPhysicalBlock & gubBurstsPerBankMask)) << gubSectorsPerPlaneLog );
		}
		else {
			ulVenderSector = (U32) (uwPhysicalSector & gubSectorsPerPlaneMask) + (U32)(((M_Multiplier((U32)uwPhysicalPage, DEF_PlanesPerPage_Mcl_Div)) + (U32)((ubLogicalCE & gubPlanesPerBurstMask) + ((uwPhysicalBlock & gubBurstsPerBankMask) << gubPlanesPerBurstLog)) + (U32)((ubLogicalCE >> gubPlanesPerBurstLog) << gubPlanesPerBankLog) ) << gubSectorsPerPlaneLog);
		}
		ubMode = 0;
		ubUseRUT = 0;
	}
	else {
		ulVenderSector = BUFL_BASE[2]; //AP 3209
		ulVenderUnit = BUFL_BASE[3]; //AP 3209

		ubMode = (UBYTE)BUFW_BASE[8];//byte 16
		ubUseRUT = (UBYTE)(BUFW_BASE[8] >> 8); //byte17
	}

	if (ubMode || ubA2cmd) {
		ubIsFastpage = 1;       //FastPage
	}
	else {
		ubIsFastpage = 0;       //Normal
	}

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, ubL4KNum * 4096);
	}

	mDMAC_SV(1,  (U32)BUFB_BASE, ubL4KNum * 4096, 0, 0, 0, DMA_EN_SETCQ, 1);
	gubNeedSendCmdComplete = 1;

	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

	if (ubUseRUT == 0) {
		//FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
	}
	else {
		//FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | (ubIsFastpage ? BIT_FJOBI_FASTPAGE : 0));
	}

	uwFUnit = BUFW_BASE[11]/*D3*/;
	uwFD1Unit = BUFW_BASE[10]/*D1*/;
	uwStartProgramOrderIndex = BUFW_BASE[12];
	uwEndProgramOrderIndex = BUFW_BASE[13];

	UartString("\r\n FUnit: ");
	UartLongHex(uwFUnit);
	UartString("\r\n FD1Unit: ");
	UartLongHex(uwFD1Unit);
	UartString("\r\n uwStartProgramOrderIndex: ");
	UartLongHex(uwStartProgramOrderIndex);
	UartString("\r\n uwEndProgramOrderIndex: ");
	UartLongHex(uwEndProgramOrderIndex);

	/*以下參考ftlFlushD1  Function 重建*/

}

#endif
#if ((!BURNER) && TLC)

#if (!TLC_BICS2)
void Get_First_Foggy_Fine(CB_Info_t *FQI_CB_Info)
{
	//取得First, Foggy, Fine,
	if (FQI_CB_Info->uwProgramOrderIndex < 2) {
		FQI_CB_Info->ubProgramOrder = FIRST_ORDER;	//First
	}
	else if ((FQI_CB_Info->uwProgramOrderIndex == 2) || (FQI_CB_Info->uwProgramOrderIndex == (guwPagesPerUnit - 3))) {
		FQI_CB_Info->ubProgramOrder = FOGGY_ORDER;	//Foggy
	}
	else if (FQI_CB_Info->uwProgramOrderIndex > (guwPagesPerUnit - 3)) {
		FQI_CB_Info->ubProgramOrder = FINE_ORDER;	//Fine
	}
	else {
		FQI_CB_Info->ubProgramOrder = ((FQI_CB_Info->uwProgramOrderIndex - 3) % 3);	//First, Foggy, Fine
	}

}

void Get_D3_WL(CB_Info_t *FQI_CB_Info)
{
	//會用到uwProgramOrder 所以上個Function需要計算正確
	//取得ubD3TargetWL
	if (FQI_CB_Info->uwProgramOrderIndex == 0 || FQI_CB_Info->uwProgramOrderIndex == 2 || FQI_CB_Info->uwProgramOrderIndex == 5) {
		FQI_CB_Info->uwD3TargetWL = 0;
	}
	else if (FQI_CB_Info->uwProgramOrderIndex == 1 || FQI_CB_Info->uwProgramOrderIndex == 4) {
		FQI_CB_Info->uwD3TargetWL = 1;
	}
	else if (FQI_CB_Info->uwProgramOrderIndex == (guwPagesPerUnit - 2)) {
		FQI_CB_Info->uwD3TargetWL = guwNum_Wordline - 2;
	}
	else if (FQI_CB_Info->uwProgramOrderIndex == (guwPagesPerUnit - 3) || FQI_CB_Info->uwProgramOrderIndex == (guwPagesPerUnit - 1) ) {
		FQI_CB_Info->uwD3TargetWL = guwNum_Wordline - 1;
	}
	else {
		FQI_CB_Info->uwD3TargetWL = (((FQI_CB_Info->uwProgramOrderIndex - (FQI_CB_Info->ubProgramOrder << 2)) / 3) + 1);
	}

}
#endif /* (!TLC_BICS2) */

#define ONE_PLANE 0

#define READ_IN 1
#define D3_READ_IN 1
#define WRITE_OUT 1

#define USE_RS_IN_OUT 1	//BYPASS ZIP

#define WRITE_NON_CHECK 1

#define NEW_DATA_BASE_EN 1

#define CPU_COPY_EN 0


#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
void Vender_SourceUint_To_TargetUint(UWORD uwFUnit_D3Target/*D3 Targe*/, UWORD uwFD1Unit/*D1 Source*/, UWORD uwFUnit_D3Source/*D3 Source*/, UWORD uwStartProgramOrderIndex, UWORD uwEndProgramOrderIndex, UBYTE ubCopyBackMode)
{
	//收到D1toD3的工作
	//1.先將FQ的事件都處理完畢(Fla做完)
	//2.確保目前沒有FQ及MT待執行(Ftl等完)
	//3.開始執行Parity SourceUint To TargetUint
	//4.直到做完Parity SourceUint To TargetUint
	volatile UBYTE ubCEIndex;
	volatile UBYTE ubFQPlaneIndex;
	volatile UWORD uw_D3TargetPageIndex;

	UBYTE ubCEIndexMAXNUM = gubCENumber;
	UBYTE ubMaxPlane = gubBurstsPerBank;



	FlashQueue_t *FQ;
	UBYTE ubDepth, ubL4kLink;
	UBYTE ubCheckDoneCnt;
	UBYTE ubPlaneIndex = 0;

	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	L4KTable16B pL4KTable;
	UBYTE ubL4KCount;
	CB_Info_t * FQI_CB_Info;

	UBYTE ubRSTag;
	UBYTE ubi;
	U32 ulSourceAddr;
	U32 ulTargetAddr;
	UBYTE ubBackUpRSDoneCount[TOTAL_PARITY_GROUP_NUM];
	UBYTE ubPlaneStep = (gubBurstsPerBank) / (((gubRUTMixPlaneEnable) && (gulEC_D3[uwFUnit_D3Target].B.btMixPlaneUnit) ) ? 1 : gubBurstsPerBank);
	UBYTE ubInitBurstBank = (ubCopyBackMode / COPYBACK_PLANE_INDEX);
	UBYTE ubGroupCENumber;//upper bound for this group ce index

	uwStartProgramOrderIndex *= 3;
	uwEndProgramOrderIndex++;
	uwEndProgramOrderIndex *= 3;
	uwEndProgramOrderIndex--;

	gubD1LinkTable[uwFUnit_D3Target].B.ubD1Link = (UBYTE)uwFD1Unit;

	// In Micron B0KB, Program Order index treat as page number.
	{
		while (gFQI.ubFQLinkNumber) { //確保目前沒有FQ及MT待執行(Ftl等完)
			M_SwitchTask();
		}
		UBYTE ubi;
		VT->gulFTLState.B.btDoingFlushD1 = 1;
		FCONL[FCONL_CRC_EN] &= ~(CRC_32_EN);

#if 0
		if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
			for ( ubi = 0; ubi < TOTAL_PARITY_GROUP_NUM; ubi ++) {
				ubBackUpRSDoneCount[ubi] = guwRSFrameDoneCount[ubi];
			}

			SaveRSparity(1);
			//Backup RS data region to copy buffer.
			mDMAC_COPY(((U32)(&guoRSFrameBits[0])), ((U32)(&gubCopyBuffer[0])), (32 * 1024 * Table_RSFrameStartIndex) , 0, 0, 0, 0, 1);

			//Backup RS spare region to end address behind RUT
			mDMAC_COPY(((U32)(&gulRSFrameSpareBits[0])), ((U32)((void *)(&guwRUTBASE[0] + (RUTSizePerCE * B0KB_MAX_CE)))), (Spare4kSize * Table_RSFrameStartIndex * 4 * 2) , 0, 0, 0, 0, 1);
		}
#endif

		// initial variables
		for (ubCEIndex = 0; ubCEIndex < ubCEIndexMAXNUM; ubCEIndex++) {
			FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];
			FQI_CB_Info->uwStartProgramOrderIndex = uwStartProgramOrderIndex;
			FQI_CB_Info->uwEndProgramOrderIndex = uwEndProgramOrderIndex;
			FQI_CB_Info->uwProgramOrderIndex = uwStartProgramOrderIndex;
			FQI_CB_Info->ubLMUIndex = 0;
		}

#if FLUSHD1_PARTIAL_CE_ADVANCE
		gubFlushD1Flag = 0xFF;//Default all CE done
#endif
		UBYTE ubPartialWork; //partial group index
		UBYTE ubActiveWorkingGroup;
		UBYTE ubCEIndexInGroup;
		UBYTE ubCurrentDataPage;
		UBYTE ubPhase = (ubCopyBackMode & COPYBACK_NON_DMA) ? 1 : 3;
		UWORD uwWLBeginWLBase;
		UBYTE ubActiveRead = 0;
		UBYTE ubActiveProgram = 0;

		if (ubCopyBackMode & COPYBACK_NON_DMA) {
			ubGroupCENumber = ubCEIndexMAXNUM;
		}
#if Hynix3DV6
		// Two Plane 占用的 RAM 比較少可以一次做比較多CE
		else if (ubCEIndexMAXNUM > (guoFlashUseType.B.btFourPlane ? FLUSHD1_PARTIAL_CENUM : (FLUSHD1_PARTIAL_CENUM * 2))) {
			ubGroupCENumber = (guoFlashUseType.B.btFourPlane ? FLUSHD1_PARTIAL_CENUM : (FLUSHD1_PARTIAL_CENUM * 2));
		}
#else
		else if (ubCEIndexMAXNUM > FLUSHD1_PARTIAL_CENUM) {
			ubGroupCENumber = FLUSHD1_PARTIAL_CENUM;
		}
#endif
		else {
			ubGroupCENumber = ubCEIndexMAXNUM;
		}

		ubPartialWork = ubCEIndexMAXNUM / ubGroupCENumber;


		UBYTE ubFQIndex_RR[ubPhase][ubGroupCENumber][ubMaxPlane];
		UBYTE ubFQIndex_WW[ubPhase][ubGroupCENumber][ubMaxPlane];


		UBYTE (*RSTagAddress)[ubGroupCENumber][ubMaxPlane] = NULL;
		UBYTE (*RSRAMAddress)[0x4000] = ((void *)(&guoRSFrameBits[0]));//3(L+M+U) * 4CE * 4Plane * 16K Page

		UBYTE (*RSSpareAddress)[0x40] = ((void *)(&gulRSFrameSpareBits[0]));

		L4KTable16B (*SpareAddress)[L4kQ_BIND_FACTOR] = (void *)L4K_TABLE_ADDR;

		memset(ubFQIndex_RR, 0xFF, sizeof(ubFQIndex_RR));
		memset(ubFQIndex_WW, 0xFF, sizeof(ubFQIndex_WW));

		for (ubCheckDoneCnt = 0; ubCheckDoneCnt < ubCEIndexMAXNUM; ) {
			for (ubActiveWorkingGroup = 0; ubActiveWorkingGroup < ubPartialWork; ubActiveWorkingGroup++) {
				//
				UBYTE ubGroupCEBeginOffset = ubActiveWorkingGroup * ubGroupCENumber;

			MARK_COPYBACK_READ:

				ubCEIndex = 0 + ubGroupCEBeginOffset;
				FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];
				uwWLBeginWLBase = FQI_CB_Info->uwProgramOrderIndex;

				for (ubCurrentDataPage = 0; ubCurrentDataPage < ubPhase; ubCurrentDataPage++) {
					for (ubCEIndexInGroup = 0; ubCEIndexInGroup < ubGroupCENumber; ubCEIndexInGroup++) {

						ubCEIndex = ubCEIndexInGroup + ubGroupCEBeginOffset;
						FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];

						if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
							continue;
						}
						// Read
						ubActiveRead = 1;

						if (ubActiveRead) {
							//Read
							for (ubPlaneIndex = ubInitBurstBank; ubPlaneIndex < ubMaxPlane ; ubPlaneIndex += ubPlaneStep) {

								ubFQIndex_RR[ubCurrentDataPage][ubCEIndexInGroup][ubPlaneIndex] = gFQI.ubFQLinkFree;

								M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
								FQ->btCheckEmpty = 0;

								UBYTE ubRSTag = ((ubCEIndexInGroup * ubPhase * ubMaxPlane) + (ubCurrentDataPage * ubMaxPlane) + ubPlaneIndex) + GC_RSFrameStartIndex * 2;
								//if( ubRSTag >= (Table_RSFrameStartIndex * 2)) {
								//ubRSTag += (GC_RSFrameStartIndex - Table_RSFrameStartIndex) * 2;
								//}


								FQ->ulRAMAddr = (U32)RSRAMAddress[ubRSTag];

								uw_D3TargetPageIndex = FQI_CB_Info->uwProgramOrderIndex;//FQI_CB_Info->uwPage;

								FQ->uwFUnit = (ubCopyBackMode & COPYBACK_D3_TO_D3) ? uwFUnit_D3Source /*D3 Source */ : uwFUnit_D3Target /*D1 Source */;
								FQ->ulFEntry = ((M_Multiplier((U32)uw_D3TargetPageIndex, DEF_4KEntryPerPage_Mcl_Div)) + (((ubCEIndex << gubBurstsPerBankLog) + ubPlaneIndex ) << gub4kEntrysPerPlaneLog)) ;

								FQ->btPreRead = 0;
								FQ->ubFJob = BYTE_FJOB_READ;
								FQ->uwFJobInfo = ( ((ubCopyBackMode & COPYBACK_NON_DMA) ? BIT_FJOBI_NON_DMA : BIT_FJOBI_WAITRESULT ) | BIT_FJOBI_FASTPAGE | BIT_FJOBI_RETRY);
								FQ->ubDepth = ubDepth;
								FQ->ubL4kIndex = ubL4kLink;
								FQ->btUNServed = 1;
								FQ->btDirectHandling = ((gubDirectHandl_RUTLog == 1) && (VT->gulFTLState.B.btNeedRUTReplace == 1)) ? 1 : 0;
								FQ->btNoWait = 1;
								FQ->btD3 = 0;
								FQ->ubD3WriteMode = (D3_SUSPEND_EN) | D3_WRITE_FINE;

								gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
								gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
								gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
								gulPara0x24.BitMap.btZipBypass = 1;
								gulPara0x24.BitMap.btCmpEn = 0;
								gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
								gulPara0x24.BitMap.btBufModeEn = 0;
								gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
								gMTQ->Depth[ubDepth].ulZipMap = 0xFFFFFFFF;

								for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
									//set L4K table info
									pL4KTable.ulL4K_LCA = 0xFFFFFFFF;
									pL4KTable.Para0x04.ulAll = 0;
									pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0;
									pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
									pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
									pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
									pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
									SpareAddress[ubL4kLink][ubL4KCount] = pL4KTable;
								}

								gFQI.ubFQLinkNumber++;
								gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
							}
						}
						else {

						}
						FQI_CB_Info->uwProgramOrderIndex++;
					}
				}
				M_SwitchTask();
				// Wait FQ & Spare
			MARK_COPYBACK_WAITREADDONE:
				while (gFQI.ubFQLinkNumber) {
					for (ubCEIndexInGroup = 0; ubCEIndexInGroup < ubGroupCENumber; ubCEIndexInGroup++) {
						for (ubCurrentDataPage = 0; ubCurrentDataPage < ubPhase; ubCurrentDataPage++) {
							for (ubPlaneIndex = ubInitBurstBank; ubPlaneIndex < ubMaxPlane ; ubPlaneIndex += ubPlaneStep) {

								if ((ubCopyBackMode & COPYBACK_NON_DMA)) {
									ubFQIndex_RR[ubCurrentDataPage][ubCEIndexInGroup][ubPlaneIndex] = 0xFF;
									continue;
								}
								else {
									UBYTE ubRFQIndex = ubFQIndex_RR[ubCurrentDataPage][ubCEIndexInGroup][ubPlaneIndex];

									if (ubRFQIndex & BIT7) {

									}
									else {
										FQ = &gFQI.gFQLink[ubRFQIndex];
										if (FQ->ubFQPhase == BYTE_FQ_WAITCHECK) {
											FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
											ubFQIndex_RR[ubCurrentDataPage][ubCEIndexInGroup][ubPlaneIndex] = 0xFF;

											UBYTE ubRSTag = ((ubCEIndexInGroup * ubPhase * ubMaxPlane) + (ubCurrentDataPage * ubMaxPlane) + ubPlaneIndex) + GC_RSFrameStartIndex * 2;
											ulSourceAddr = ((U32)SpareAddress[FQ->ubL4kIndex]);
											ulTargetAddr = ((U32)RSSpareAddress[ubRSTag]);
											mDMAC_COPY((U32)ulSourceAddr, (U32)ulTargetAddr, 16 * ubL4KNum , 0, 0, 0, 0, 1);
										}
										else {

										}
									}
								}
							}
						}
					}

					M_SwitchTask();
				}

			MARK_COPYBACK_WRITE:
				for (ubCEIndexInGroup = 0; ubCEIndexInGroup < ubGroupCENumber; ubCEIndexInGroup++) {

					ubCEIndex = ubCEIndexInGroup + ubGroupCEBeginOffset;
					FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];
					FQI_CB_Info->uwProgramOrderIndex = uwWLBeginWLBase;
				}

				for (ubCEIndexInGroup = 0; ubCEIndexInGroup < ubGroupCENumber; ubCEIndexInGroup++) {

					ubCEIndex = ubCEIndexInGroup + ubGroupCEBeginOffset;
					FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];
					if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
						continue;
					}

					// Program

					for (ubCurrentDataPage = 0; ubCurrentDataPage < ubPhase; ubCurrentDataPage++) {
						ubActiveProgram = 1;

						if (ubActiveProgram) {
							//write four plane loop
							for (ubPlaneIndex = ubInitBurstBank; ubPlaneIndex < ubMaxPlane ; ubPlaneIndex += ubPlaneStep) {

								ubFQIndex_WW[ubCurrentDataPage][ubCEIndexInGroup][ubPlaneIndex] = gFQI.ubFQLinkFree;
								M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
								FQ->btCheckEmpty = 0;

								uw_D3TargetPageIndex = FQI_CB_Info->uwProgramOrderIndex;


								UBYTE ubRSTag = ((ubCEIndexInGroup * ubPhase * ubMaxPlane) + (ubCurrentDataPage * ubMaxPlane) + ubPlaneIndex) + GC_RSFrameStartIndex * 2;

								FQ->ulRAMAddr = (U32)RSRAMAddress[ubRSTag];
								FQ->uwFUnit = uwFUnit_D3Target /*D3 Target */;
								FQ->ulFEntry = ((M_Multiplier((U32)uw_D3TargetPageIndex, DEF_4KEntryPerPage_Mcl_Div)) + (((ubCEIndex << gubBurstsPerBankLog) + ubPlaneIndex ) << gub4kEntrysPerPlaneLog)) ;


								FQ->btPreRead = 0;
								FQ->ubFJob = BYTE_FJOB_WRITE;
								FQ->ubRelativeDepth = ubDepth;
								FQ->ubDepth = 0xFF;
								FQ->ubL4kIndex = ubL4kLink;
								FQ->btUNServed = 1;
								FQ->btDirectHandling = (gubDirectHandl_RUTLog == 1) ? 1 : 0;
								FQ->btD3 = 1;
								FQ->btNoWait = 1;
								FQ->btIsMixPlaneUnit = (ubPlaneStep > 1);

								FQ->uwFJobInfo = ((ubCopyBackMode & COPYBACK_NON_DMA) ? BIT_FJOBI_NON_DMA : 0 );
								FQ->ubD3WriteMode = (D3_SUSPEND_EN) | D3_WRITE_FINE | (D3_WRITE_LOWDER << (uw_D3TargetPageIndex % 3));


								//memcpy(SpareAddress[FQ->ubL4kIndex], RSSpareAddress[ubRSTag], 0x40);

								gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (FQ->ubL4kIndex * L4kQ_BIND_FACTOR * Spare4kSize);
								gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
								gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
								gulPara0x24.BitMap.btZipBypass = 1;
								gulPara0x24.BitMap.btCmpEn = 0;
								gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
								gulPara0x24.BitMap.btBufModeEn = 0;
								gMTQ->Depth[ubDepth].Para0x04.BitMap.btRSEncodeMode = 0;
								gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
								gMTQ->Depth[ubDepth].ulZipMap = 0xFFFFFFFF;



								if ((!(ubCopyBackMode & COPYBACK_NON_DMA) ) ) {
									FQ->btParity = 1;
									M_AssertError(ubRSTag >= RS_MAX_TAG_NUM);
									FQ->ubRSFrameIndex = ubRSTag / 2;
									FQ->btRSOneParityEn = 0;
									FQ->ubRSPageNum = 0xFF;
									FQ->btRSProg2ndParity = ubPlaneIndex & 1;
									FQ->btRSLastPage = 0;


								}
								else {
									for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
										SpareAddress[FQ->ubL4kIndex][ubL4KCount].Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
										SpareAddress[FQ->ubL4kIndex][ubL4KCount].Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
									}
								}
								gFQI.ubFQLinkNumber++;
								gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
							}

						}
						FQI_CB_Info->uwProgramOrderIndex++;
						//M_SwitchTask();
					}

					if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
						ubCheckDoneCnt++;
					}


				}
				// Wait Busy
				while (gFQI.ubFQLinkNumber) {
					M_SwitchTask();
				}
			}
		}

		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}

		//UartString("\nProgram End WL ");
		//UartWordHex(uw_D3TargetPageIndex);
		/*
		 * Enable CRC check for normal FW flow at the end
		 */

		FCONL[FCONL_CRC_EN] |= (CRC_32_EN);
#if 0
		if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {

			UBYTE ubRSFastPage = VT->guwGRTarget[VT->gubGRTargetIndex].B.btFastPage;
			UBYTE ubRSTag2Group[USERDATA_RS_Group_Num + 1];
			U32 ulLastDataPlaneIndex;
			for (ubi = 0; ubi < (USERDATA_RS_Group_Num + 1) ; ubi++) {
				// here ubi use as userdata tag
				ubRSTag2Group[ubi] = 0xFF;
			}
			for (ubi = 0; ubi < USERDATA_RS_Group_Num; ubi++) {
				// here ubi use as group index
				if (ubRSFastPage) {
					Generate_RS_MAP_Index_by_Plane(RS.ulLastDataPlaneIndexFastPage[ubi]);
				}
				else {
					Generate_RS_MAP_Index_by_Plane(RS.ulLastDataPlaneIndex[ubi]);
				}
				ubRSTag2Group[RS.ubTagMAP[RS.ubGroupIndex][RS.ubTagIndex]] = ubi;
			}

			for ( ubi = USERDATA_RSFrameStartIndex; ubi < Table_RSFrameStartIndex; ubi ++) {

				if (VT->guwRSFrameCount[ubi]) {
					RSL[RSL_TAG_SEL] = ubi;
					while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubi);
					RSL[RSL_TAG_VLD_CTL] |= RS_TAG_VALID_CLR;
					while ( (RSB[RSB_TAG_CONFIG_0] & RS_TAG_VALID) == RS_TAG_VALID );
					RSB[RSB_TAG_PEC] = VT->guwRSFrameCount[ubi];

					if (ubi < (USERDATA_RS_Group_Num + 1)) {
						if (ubRSTag2Group[ubi] != 0xFF) {
							// get last data plane index of this tag
							if (ubRSFastPage) {
								ulLastDataPlaneIndex = RS.ulLastDataPlaneIndexFastPage[ubRSTag2Group[ubi]];
							}
							else {
								ulLastDataPlaneIndex = RS.ulLastDataPlaneIndex[ubRSTag2Group[ubi]];
							}
							Generate_RS_MAP_Index_by_Plane(ulLastDataPlaneIndex);
						}
						else {
							ulLastDataPlaneIndex = gulPlanesPerUnit;
						}

						// 條件1: Unit結尾未滿252, 正準備寫Parity, 條件2: 滿252,正準備寫Parity
						if ((( VT->guwRSFrameCount[ubi] >= (RS.uwPlaneOffset + 1) ) && (VT->gulGRTargetPTR > ulLastDataPlaneIndex))\
						        || (VT->guwRSFrameCount[ubi] >= (guwParityPlanesNum - 3 + 1))) {
							gubForceProgramRSParity[ubi] = 1;
						}
					}

					RSL[RSL_HANG] |= RS_TAG_VALID_SET;
				}
			}

			for ( ubi = 0; ubi < TOTAL_PARITY_GROUP_NUM; ubi ++) {
				guwRSFrameDoneCount[ubi] = ubBackUpRSDoneCount[ubi];
			}

			//Restore RS data region from copy buffer.
			mDMAC_COPY(((U32)(&gubCopyBuffer[0])), ((U32)(&guoRSFrameBits[0])), (32 * 1024 * Table_RSFrameStartIndex) , 0, 0, 0, 0, 1);

			//Backup RS spare region to address behind RUT
			mDMAC_COPY( ((U32)(&guwRUTBASE[0] + (RUTSizePerCE * B0KB_MAX_CE))), ((U32)(&gulRSFrameSpareBits[0])),	(Spare4kSize * Table_RSFrameStartIndex * 4 * 2) , 0, 0, 0, 0, 1);
		}
#endif


		M_SwitchTask();
	}
	VT->gulFTLState.B.btDoingFlushD1 = 0;

	return;

}

#elif (MicronFlashOnly && B0KB)
#define FLUSHD1_DBG 0
void Vender_SourceUint_To_TargetUint(UWORD uwFUnit_D3Target/*D3 Targe*/, UWORD uwFD1Unit/*D1 Source*/, UWORD uwFUnit_D3Source/*D3 Source*/, UWORD uwStartProgramOrderIndex, UWORD uwEndProgramOrderIndex, UBYTE ubCopyBackMode)
{
	//收到D1toD3的工作
	//1.先將FQ的事件都處理完畢(Fla做完)
	//2.確保目前沒有FQ及MT待執行(Ftl等完)
	//3.開始執行Parity SourceUint To TargetUint
	//4.直到做完Parity SourceUint To TargetUint

	UBYTE ubCEIndexMAXNUM = gubCENumber;
	volatile UBYTE ubCEIndex;
	UBYTE ubFQIndex_R[B0KB_MAX_CE * 4 * 2];
	UBYTE ubFQIndex_W[B0KB_MAX_CE * 4 * 2];
	UBYTE ubReadDone[2][B0KB_MAX_CE];
	UBYTE ubWriteDone[2][B0KB_MAX_CE];
	volatile UBYTE ubFQPlaneIndex;
	FlashQueue_t *FQ;
	UBYTE ubDepth, ubL4kLink;
	UBYTE ubCheckDoneCnt;
	UBYTE ubPlaneIndex = 0;
	volatile UWORD uw_D3TargetPageIndex;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	L4KTable16B *pL4KTablePtr;
	//L4KTable16B *pL4KTablePtrSource;
	L4KTable16B pL4KTable;
	UBYTE ubL4KCount;
	CB_Info_t *FQI_CB_Info;
	UBYTE ubMaxPlane = ( guoFlashUseType.B.btFourPlane ? 4 : (guoFlashUseType.B.btTwoPlane ? 2 : 1));
	UBYTE ubRSTag;
	U32 ulSourceAddr, ulTargetAddr;
	UBYTE ubIs2ndDataGroup;
	UBYTE ubBackUpRSDoneCount[TOTAL_PARITY_GROUP_NUM];
	UBYTE ubFirstGroupCE;
	UBYTE ubDataPageNum = (ubCopyBackMode & COPYBACK_NON_DMA) ? 1 : 2; //RW for NON DMA  RRWW for DMA
	UBYTE ub2ndPageCnt[MAX_CE] = {0};//For NON-DMA  2: Pass2 FirstPage 1: Pass2 SecondPage  0: Initial Value
	UBYTE ubPlaneStep = (gubBurstsPerBank) / (((gubRUTMixPlaneEnable) && (gulEC_D3[uwFUnit_D3Target].B.btMixPlaneUnit) ) ? 1 : gubBurstsPerBank);
	UBYTE ubInitBurstBank = (ubCopyBackMode / COPYBACK_PLANE_INDEX);


	gubD1LinkTable[uwFUnit_D3Target].B.ubD1Link = (UBYTE)uwFD1Unit;

	// In Micron B0KB, Program Order index treat as page number.
	{
		while (gFQI.ubFQLinkNumber) { //確保目前沒有FQ及MT待執行(Ftl等完)
			M_SwitchTask();
		}
		UBYTE ubi;
		VT->gulFTLState.B.btDoingFlushD1 = 1;
#if FlushD1SpeedDown
		if ( ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz)) || (gubFlashClock == FLH_CLK_266MHz) ) {
			flaSwitchClock(FLH_CLK_166MHz);
		}
#endif
		/* Disable CRC check for whole flush procedure
			 * Since parity page will hit CRC error
			 */
		FCONL[FCONL_CRC_EN] &= ~(CRC_32_EN);

		if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
			for ( ubi = 0; ubi < TOTAL_PARITY_GROUP_NUM; ubi ++) {
				ubBackUpRSDoneCount[ubi] = guwRSFrameDoneCount[ubi];
			}

			SaveRSparity(1);

			//Backup RS data region to copy buffer.
			mDMAC_COPY(((U32)(&guoRSFrameBits[0])), ((U32)(&gubCopyBuffer[0])), (32 * 1024 * Table_RSFrameStartIndex) , 0, 0, 0, 0, 1);

			//Backup RS spare region to end address behind RUT
			mDMAC_COPY(((U32)(&gulRSFrameSpareBits[0])), ((U32)(&guwRUTBASE[0] + (RUTSizePerCE * B0KB_MAX_CE))), (Spare4kSize * Table_RSFrameStartIndex * 4 * 2) , 0, 0, 0, 0, 1);
		}

		for (ubCEIndex = 0; ubCEIndex < ubCEIndexMAXNUM; ubCEIndex++) {
			FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];
			FQI_CB_Info->uwStartProgramOrderIndex = uwStartProgramOrderIndex;
			FQI_CB_Info->uwEndProgramOrderIndex = uwEndProgramOrderIndex;

			if (FQI_CB_Info->uwStartProgramOrderIndex != FQI_CB_Info->uwProgramOrderIndex) {
				//UartString("\r\n uwStartProgramOrderIndex != uwProgramOrderIndex ===========Error");
			}
			if (FQI_CB_Info->uwStartProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
				//UartString("\r\n uwStartProgramOrderIndex > uwEndProgramOrderIndex ===========Error");
			}

			FQI_CB_Info->uwProgramOrderIndex = uwStartProgramOrderIndex;
			FQI_CB_Info->ubLMUIndex = 0;

			ubReadDone[0][ubCEIndex] = 0;
			ubWriteDone[0][ubCEIndex] = 0;
			ubReadDone[1][ubCEIndex] = 0;
			ubWriteDone[1][ubCEIndex] = 0;
			gubD1D3CEPushFQ[ubCEIndex] = 0;
		}
		//memset(ubFQIndex_R, 0xFF, B0KB_MAX_CE * 4 * 2);
		//memset(ubFQIndex_W, 0xFF, B0KB_MAX_CE * 4 * 2);


#if FW_SOURCEUINT_TO_TARGETUINT_START_END_UART
		UartString("\r\n SourceUint To TargetUint START :");
#endif
		UBYTE ubPartialWork; //partial group index
		UBYTE ubGroupCENumber;//upper bound for this group ce index
		UBYTE ubGroupSel[8] = {0};//record each CE run which group, larger size but clear when check condition
		UBYTE ubCondition = 0x00;
		if ( (ubCopyBackMode & COPYBACK_NON_DMA) || (ubCEIndexMAXNUM < FLUSHD1_PARTIAL_CENUM) ) {// 1 group case
			//Internal copy back, or Re-fill copy back can be finished in on CE group
			ubGroupCENumber = ubCEIndexMAXNUM;
			ubFirstGroupCE = ubGroupCENumber;
			ubPartialWork = 1;
		}
		else {// 2 group case
			ubGroupCENumber = ubCEIndexMAXNUM / 2;
			if ( ubCEIndexMAXNUM & 1) { //odd CE case
				ubGroupCENumber ++;
			}
			ubFirstGroupCE = ubGroupCENumber;
			ubPartialWork = 2;
		}

#if FLUSHD1_PARTIAL_CE_ADVANCE
		gubFlushD1Flag = 0xFF;//Default all CE done
#endif

#if 1
		for (ubCheckDoneCnt = 0; ubCheckDoneCnt < ubCEIndexMAXNUM;) {
			for (ubi = 0; ubi < ubPartialWork ; ubi++) {
				//Check Total Done
				ubGroupCENumber = (ubi == 1) ? ubCEIndexMAXNUM : ubFirstGroupCE;
#if FLUSHD1_DBG
				UartString("\r\nG");
				Uart_Tx_DataHex(ubi);
#endif
				for (ubCEIndex = ubi * ubFirstGroupCE; ubCEIndex < ubGroupCENumber; ubCEIndex++) {
					//Get page index
					FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];

					if (ubPartialWork > 1) { //only check for two group case
						if (ubGroupSel[ubCEIndex % ubFirstGroupCE] != ubi) { //skip when not this group turn
							continue;
						}

						if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
							ubGroupSel[ubCEIndex % ubFirstGroupCE] = (ubGroupSel[ubCEIndex % ubFirstGroupCE] == 0) ? 1 : 0;//change group when done
							continue;
						}
					}
					else {
						if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
							continue;
						}
					}
					if ((ubCopyBackMode & COPYBACK_NON_DMA) || (ubCEIndexMAXNUM < FLUSHD1_PARTIAL_CENUM)) { // 1 group DMA or non DMA case
						ubCondition = (!(FQI_CB_Info->ubLMUIndex & READ_FQ_GET_DONE));
					}
					else {// 2 group DMA case
						ubCondition = (!(FQI_CB_Info->ubLMUIndex & READ_FQ_GET_DONE)) && (gubFlushD1Flag & (BIT0 << (ubCEIndex	% ubFirstGroupCE)));
					}

					if (ubCondition) {
#if 1 //Debug
						if ((FQ_NUM - gFQI.ubFQDoingNumber[ubCEIndex]) < (ubMaxPlane * ubDataPageNum)) { //confirm enough FQ can comnbine multi-plane operation
							continue;
						}
#endif



#if FLUSHD1_DBG
						UartString("\r\nR ");
						UartString("CE ");
						Uart_Tx_DataHex(ubCEIndex);
#endif
						B0KB_CheckPage(FQI_CB_Info);
						uw_D3TargetPageIndex = FQI_CB_Info->uwProgramOrderIndex;

						//for data group loop, Lower/Upper, Extra/Upper
						for (ubIs2ndDataGroup = 0; ubIs2ndDataGroup < ubDataPageNum; ubIs2ndDataGroup ++) {
							if (!(ubCopyBackMode & COPYBACK_NON_DMA)) { //DMA case
								if (ubIs2ndDataGroup) {
									if (FQI_CB_Info->ubProgramOrder == 1) { // belonging to 1st pass, and read page, lower -> upper page
										if (   guoFlashUseType.B.btEnterprise
										        && (uw_D3TargetPageIndex >= (B0KB_3SHAREPAGE_1STPASS_LOWER_START_INDEX))
										        && (uw_D3TargetPageIndex <= (B0KB_3SHAREPAGE_1STPASS_LOWER_END_INDEX))) {
											//Enterprise 8LC 1st Pass Skip Upper Page
											continue;
										}
										else if ( uw_D3TargetPageIndex >= B0KB_1STSLC_IN_END) {
											continue;// because page larger 1505 and belonging 1 pass the next page is extra page.
										}
										else {
											uw_D3TargetPageIndex ++;
										}
									}
									else if (FQI_CB_Info->ubProgramOrder == 2) {// belonging to 2nd pass, and read page, extra -> upper
										uw_D3TargetPageIndex = FQI_CB_Info->uwUPage;
									}
								}
							}
							else {//Non-DMA case update Page index at last
								if (FQI_CB_Info->ubProgramOrder == 2) {
									ub2ndPageCnt[ubCEIndex] = 2;//is first 2nd DataGroup
								}
							}

#if FLUSHD1_DBG
							if (ubIs2ndDataGroup) {
								UartString(" S ");
							}
							else {
								UartString(" F ");
							}
							UartWordHex(uw_D3TargetPageIndex);
#endif

							//Read
							for (ubPlaneIndex = ubInitBurstBank; ubPlaneIndex < ubMaxPlane ; ubPlaneIndex += ubPlaneStep) {
								ubFQPlaneIndex = ((ubCEIndex * ubMaxPlane) + ubPlaneIndex) + (ubIs2ndDataGroup * ubFirstGroupCE * ubMaxPlane);
								ubFQIndex_R[ubFQPlaneIndex] = gFQI.ubFQLinkFree;
								M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
								FQ->btCheckEmpty = 0;

								ubRSTag = USERDATA_RSFrameStartIndex + ((( (ubCEIndex % ubFirstGroupCE) * ubMaxPlane + ubPlaneIndex) + (ubIs2ndDataGroup * ubFirstGroupCE * ubMaxPlane)) >> 1);
								if ( ubRSTag >= Table_RSFrameStartIndex) {
									ubRSTag += (Table_RS_Group_Num + FailHandle_Table_RS_Group_Num);
								}
								M_AssertError(ubRSTag >= RS_MAX_TAG_NUM);
								FQ->ulRAMAddr = ((U32)(&guoRSFrameBits[(((ubRSTag * 2) + (ubPlaneIndex & 1)) * 16384) / 8]));

								FQ->uwFUnit = (ubCopyBackMode & COPYBACK_D3_TO_D3) ? uwFUnit_D3Source /*D3 Source */ : uwFUnit_D3Target /*D1 Source */;
								FQ->ulFEntry = ((M_Multiplier((U32)uw_D3TargetPageIndex, DEF_4KEntryPerPage_Mcl_Div)) + (((ubCEIndex << gubBurstsPerBankLog) + ubPlaneIndex ) << gub4kEntrysPerPlaneLog)) ;

								//debug
#if 0 //FLUSHD1_DBG
								if ( (uw_D3TargetPageIndex == 48) || (uw_D3TargetPageIndex == 49) || (uw_D3TargetPageIndex == 112) ) {
									UartString(" Page:");
									UartWordHex(uw_D3TargetPageIndex);
									UartString(" Tag:");
									UartWordHex(ubRSTag);
									UartString(" Ram:");
									UartLongHex(FQ->ulRAMAddr);
									UartString(" Entry:");
									UartLongHex(FQ->ulFEntry);
								}
#endif
								FQ->btPreRead = 0;
								FQ->ubFJob = BYTE_FJOB_READ;
								FQ->uwFJobInfo = ( ((ubCopyBackMode & COPYBACK_NON_DMA) ? BIT_FJOBI_NON_DMA : BIT_FJOBI_WAITRESULT ) | ((ubCopyBackMode & COPYBACK_D3_TO_D3) ? 0 : BIT_FJOBI_FASTPAGE) | BIT_FJOBI_RETRY);
								FQ->ubDepth = ubDepth;
								FQ->ubL4kIndex = ubL4kLink;
								FQ->btUNServed = 1;
								FQ->btD3 = (ubCopyBackMode & COPYBACK_D3_TO_D3) ? 1 : 0;
								FQ->btNoWait = 1;
								FQ->ubD3WriteMode = (ubCopyBackMode & COPYBACK_D3_TO_D3) ? 0 : D3_SUSPEND_EN;

								gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
								gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
								gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
								gulPara0x24.BitMap.btZipBypass = 1;
								gulPara0x24.BitMap.btCmpEn = 0;
								gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
								gulPara0x24.BitMap.btBufModeEn = 0;
								gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
								gMTQ->Depth[ubDepth].ulZipMap = 0xFFFFFFFF;

								for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
									pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
									//set L4K table info
									pL4KTable.ulL4K_LCA = 0xFFFFFFFF;
									pL4KTable.Para0x04.ulAll = 0;
									pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0;
									pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
									pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
									pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
									pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
									*pL4KTablePtr = pL4KTable;
								}

								gFQI.ubFQLinkNumber++;
								gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
								gubD1D3CEPushFQ[ubCEIndex]++;
							}
						}
#if FLUSHD1_PARTIAL_CE_ADVANCE
						if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
							gubFlushD1Flag &= ~(BIT0 << (ubCEIndex % ubFirstGroupCE));// all Read Plane Get FQ done, clean flush flag by CE
						}
#endif
#if FLUSHD1_DBG
						UartString(" OK");
#endif
						FQI_CB_Info->ubLMUIndex |= READ_FQ_GET_DONE;
						M_SwitchTask();
						if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
							continue; //To next ce
						}
					}

					// read is finished, copy spare
					if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
						if ( (!(FQI_CB_Info->ubLMUIndex & SPARE_COPY_DONE)) && (FQI_CB_Info->ubLMUIndex & READ_FQ_GET_DONE)) {
#if FLUSHD1_DBG
							UartString("\r\nC ");
							UartString("CE ");
							Uart_Tx_DataHex(ubCEIndex);
#endif
							for (ubIs2ndDataGroup = 0; ubIs2ndDataGroup < ubDataPageNum; ubIs2ndDataGroup ++) {
								if (ubIs2ndDataGroup && (FQI_CB_Info->ubProgramOrder == 1)) {
									if (   guoFlashUseType.B.btEnterprise
									        && (FQI_CB_Info->uwProgramOrderIndex >= (B0KB_3SHAREPAGE_1STPASS_LOWER_START_INDEX))
									        && (FQI_CB_Info->uwProgramOrderIndex <= (B0KB_3SHAREPAGE_1STPASS_LOWER_END_INDEX))) {
										// Enterprise 8LC 1st Pass Skip Upper Page
										ubReadDone[1][ubCEIndex] = ubMaxPlane;
										continue;
									}
									if ((FQI_CB_Info->uwProgramOrderIndex >= B0KB_1STSLC_IN_END)) {
										// because page larger 1505 and belonging 1 pass the next page is extra page.
										ubReadDone[1][ubCEIndex] = ubMaxPlane;
										continue;
									}
								}
#if FLUSHD1_DBG
								if (ubIs2ndDataGroup) {
									UartString(" S");
								}
								else {
									UartString(" F");
								}
#endif
								//Copy spare
								for (ubPlaneIndex = ubReadDone[ubIs2ndDataGroup][ubCEIndex] + ubInitBurstBank; ubPlaneIndex < ubMaxPlane; ubPlaneIndex += ubPlaneStep) {
									ubFQPlaneIndex = ((ubCEIndex * ubMaxPlane) + ubPlaneIndex) + (ubIs2ndDataGroup * ubFirstGroupCE * ubMaxPlane);
									//M_AssertError(ubFQIndex_R[ubFQPlaneIndex] == 0xFF);
									if (gFQI.gFQLink[ubFQIndex_R[ubFQPlaneIndex]].ubFQPhase != BYTE_FQ_WAITCHECK) {
										//M_SwitchTask();
										break;
									}
									else {
										ubRSTag = USERDATA_RSFrameStartIndex + ((( (ubCEIndex % ubFirstGroupCE) * ubMaxPlane + ubPlaneIndex) + (ubIs2ndDataGroup * ubFirstGroupCE * ubMaxPlane)) >> 1);
										if ( ubRSTag >= Table_RSFrameStartIndex) {
											ubRSTag += (Table_RS_Group_Num + FailHandle_Table_RS_Group_Num);
										}

										//Spare
										ulSourceAddr = ((U32)( L4K_TABLE_ADDR + ((gFQI.gFQLink[ubFQIndex_R[ubFQPlaneIndex]].ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize)));
										ulTargetAddr = ((U32)(&gulRSFrameSpareBits[((ubRSTag * 128) + ((ubPlaneIndex & 1) * 64) ) / 4]));

										mDMAC_COPY((U32)ulSourceAddr, (U32)ulTargetAddr, 16 * ubL4KNum , 0, 0, 0, 0, 1);

										gFQI.gFQLink[ubFQIndex_R[ubFQPlaneIndex]].ubFQPhase = BYTE_FQ_CHECKDONE;
										//ubFQIndex_R[ubFQPlaneIndex] = 0xFF;//Check done not accessed again
										ubReadDone[ubIs2ndDataGroup][ubCEIndex] += ubPlaneStep;
									}
								}
							}
							if ( (ubReadDone[0][ubCEIndex] >= ubMaxPlane) && (ubReadDone[1][ubCEIndex] >= ubMaxPlane) ) {
#if FLUSHD1_DBG
								UartString(" OK");
#endif
								FQI_CB_Info->ubLMUIndex |= SPARE_COPY_DONE;
							}
							else {
								continue;
							}
						}
					}
					else { // Internal copy back
						if ( (!(FQI_CB_Info->ubLMUIndex & SPARE_COPY_DONE)) && (FQI_CB_Info->ubLMUIndex & READ_FQ_GET_DONE)) {
							FQI_CB_Info->ubLMUIndex |= SPARE_COPY_DONE;
#if FLUSHD1_DBG
							UartString("\r\nC ");
							UartString("CE ");
							Uart_Tx_DataHex(ubCEIndex);
							UartString(" OK");
#endif

						}
					}

					if ( (!(FQI_CB_Info->ubLMUIndex & WRITE_FQ_GET_DONE)) && (FQI_CB_Info->ubLMUIndex & SPARE_COPY_DONE)) {
						if ((FQ_NUM - gFQI.ubFQDoingNumber[ubCEIndex]) < (ubMaxPlane * ubDataPageNum)) {
							//confirm all Read FQ has been removed
							//confirm enough FQ can comnbine multi-plane operation
							continue;
						}
#if FLUSHD1_DBG
						UartString("\r\nW");
						UartString(" CE ");
						Uart_Tx_DataHex(ubCEIndex);
#endif

						B0KB_CheckPage(FQI_CB_Info);
						uw_D3TargetPageIndex = FQI_CB_Info->uwProgramOrderIndex;

						//for data group loop, Lower/Upper, Extra/Upper
						for (ubIs2ndDataGroup = 0; ubIs2ndDataGroup < ubDataPageNum; ubIs2ndDataGroup ++) {
							if (!(ubCopyBackMode & COPYBACK_NON_DMA)) { //DMA case
								if (ubIs2ndDataGroup) {
									if (FQI_CB_Info->ubProgramOrder == 1) { // belonging to 1st pass, and read page, lower -> upper page
										if (   guoFlashUseType.B.btEnterprise
										        && (uw_D3TargetPageIndex >= (B0KB_3SHAREPAGE_1STPASS_LOWER_START_INDEX))
										        && (uw_D3TargetPageIndex <= (B0KB_3SHAREPAGE_1STPASS_LOWER_END_INDEX))) {
											//Enterprise 8LC 1st Pass Skip Upper Page
											continue;
										}
										else if ( uw_D3TargetPageIndex >= B0KB_1STSLC_IN_END) {
											continue;// because page larger 1505 and belonging 1 pass the next page is extra page.
										}
										else {
											uw_D3TargetPageIndex ++;
										}
									}
									else if (FQI_CB_Info->ubProgramOrder == 2) {// belonging to 2nd pass, and read page, extra -> upper
										uw_D3TargetPageIndex = FQI_CB_Info->uwUPage;
									}
								}
							}
							else {//Non-DMA case update Page index at last
								if (FQI_CB_Info->ubProgramOrder == 2) {
									ub2ndPageCnt[ubCEIndex] = 2;//is first 2nd DataGroup
								}
							}
#if FLUSHD1_DBG
							if (ubIs2ndDataGroup) {
								UartString(" S ");
							}
							else {
								UartString(" F ");
							}
							UartWordHex(uw_D3TargetPageIndex);
#endif
							//write four plane loop
							for (ubPlaneIndex = ubInitBurstBank; ubPlaneIndex < ubMaxPlane ; ubPlaneIndex += ubPlaneStep) {
								ubFQPlaneIndex = ((ubCEIndex * ubMaxPlane) + ubPlaneIndex) + (ubIs2ndDataGroup * ubFirstGroupCE * ubMaxPlane);
								ubFQIndex_W[ubFQPlaneIndex] = gFQI.ubFQLinkFree;
								M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
								FQ->btCheckEmpty = 0;

								ubRSTag = USERDATA_RSFrameStartIndex + ((( (ubCEIndex % ubFirstGroupCE) * ubMaxPlane + ubPlaneIndex) + (ubIs2ndDataGroup * ubFirstGroupCE * ubMaxPlane)) >> 1);
								if ( ubRSTag >= Table_RSFrameStartIndex) {
									ubRSTag += (Table_RS_Group_Num + FailHandle_Table_RS_Group_Num);
								}
								M_AssertError(ubRSTag >= RS_MAX_TAG_NUM);
								FQ->ulRAMAddr = ((U32)(&guoRSFrameBits[(((ubRSTag * 2) + (ubPlaneIndex & 1)) * 16384) / 8]));

								FQ->uwFUnit = uwFUnit_D3Target /*D3 Target */;
								FQ->ulFEntry = ((M_Multiplier((U32)uw_D3TargetPageIndex, DEF_4KEntryPerPage_Mcl_Div)) + (((ubCEIndex << gubBurstsPerBankLog) + ubPlaneIndex ) << gub4kEntrysPerPlaneLog)) ;

								//debug
#if 0 //FLUSHD1_DBG
								if ( (uw_D3TargetPageIndex == 48) || (uw_D3TargetPageIndex == 49) || (uw_D3TargetPageIndex == 112) ) {
									UartString(" Page:");
									UartWordHex(uw_D3TargetPageIndex);
									UartString(" Tag:");
									UartWordHex(ubRSTag);
									UartString(" Ram:");
									UartLongHex(FQ->ulRAMAddr);
									UartString(" Entry:");
									UartLongHex(FQ->ulFEntry);
								}
#endif

								FQ->btPreRead = 0;
								FQ->ubFJob = BYTE_FJOB_WRITE;
								FQ->ubRelativeDepth = ubDepth;
								FQ->ubDepth = 0xFF;
								FQ->ubL4kIndex = ubL4kLink;
								FQ->btUNServed = 1;
								FQ->btD3 = 1;
								FQ->btNoWait = 1;
								FQ->btIsMixPlaneUnit = (ubPlaneStep > 1);
#if FLUSHD1_PARTIAL_CE_ADVANCE  //Debug
								if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
									if ((ubPlaneIndex + ubPlaneStep) >= ubMaxPlane) {
										if (ubIs2ndDataGroup == 1) {
											FQ->btLastProgram = 1;
										}
										else if ( (FQI_CB_Info->ubProgramOrder == 1) && (uw_D3TargetPageIndex >= B0KB_1STSLC_IN_END) ) { //for 1st Data Group
											FQ->btLastProgram = 1;
										}
									}
								}
#endif

								//FQ->uwFJobInfo = ( ((ubCopyBackMode & COPYBACK_NON_DMA) ? BIT_FJOBI_NON_DMA : 0 ) | (((ubCopyBackMode & COPYBACK_NON_DMA) || WRITE_NON_CHECK) ? 0 : BIT_FJOBI_WAITRESULT));
#if WRITE_NON_CHECK
								FQ->uwFJobInfo = ((ubCopyBackMode & COPYBACK_NON_DMA) ? BIT_FJOBI_NON_DMA : 0 );
#else
								FQ->uwFJobInfo = ((ubCopyBackMode & COPYBACK_NON_DMA) ? BIT_FJOBI_NON_DMA : BIT_FJOBI_WAITRESULT );
#endif
								FQ->ubD3WriteMode = (D3_SUSPEND_EN);

								gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (FQ->ubL4kIndex * L4kQ_BIND_FACTOR * Spare4kSize);
								gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
								gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
								gulPara0x24.BitMap.btZipBypass = 1;
								gulPara0x24.BitMap.btCmpEn = 0;
								gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
								gulPara0x24.BitMap.btBufModeEn = 0;
								gMTQ->Depth[ubDepth].Para0x04.BitMap.btRSEncodeMode = 0;
								gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
								gMTQ->Depth[ubDepth].ulZipMap = 0xFFFFFFFF;

								if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
									FQ->btParity = 1;
									M_AssertError(ubRSTag >= RS_MAX_TAG_NUM);
									FQ->ubRSFrameIndex = ubRSTag;
									FQ->btRSOneParityEn = 0;
									FQ->ubRSPageNum = 0xFF;
									FQ->btRSProg2ndParity = ubPlaneIndex & 1;
									FQ->btRSLastPage = 0;
								}
								else {
									for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
										pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
										pL4KTable = *pL4KTablePtr;
										//set L4K table info
										pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
										pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
										*pL4KTablePtr = pL4KTable;
									}

								}
								gFQI.ubFQLinkNumber++;
								gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
								gubD1D3CEPushFQ[ubCEIndex]++;
							}
						}
						FQI_CB_Info->ubLMUIndex |= WRITE_FQ_GET_DONE;
						M_SwitchTask();
						if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
							continue; //To next ce
						}
					}

#if WRITE_NON_CHECK
					if (!(FQI_CB_Info->ubLMUIndex & WRITE_DONE) && (FQI_CB_Info->ubLMUIndex & WRITE_FQ_GET_DONE)) {
						FQI_CB_Info->ubLMUIndex |= WRITE_DONE;
#if FLUSHD1_DBG
						UartString("\r\nY ");
						UartString("CE ");
						Uart_Tx_DataHex(ubCEIndex);
						UartString(" OK");
#endif

					}
#else
					if ( !(ubCopyBackMode & COPYBACK_NON_DMA) && (FQI_CB_Info->ubLMUIndex & WRITE_FQ_GET_DONE) && !(FQI_CB_Info->ubLMUIndex & WRITE_DONE)) {
#if FLUSHD1_DBG
						UartString("\r\nY ");
						UartString("CE ");
						Uart_Tx_DataHex(ubCEIndex);
#endif
						for (ubIs2ndDataGroup = 0; ubIs2ndDataGroup < ubDataPageNum; ubIs2ndDataGroup ++) {
							if (ubIs2ndDataGroup && (FQI_CB_Info->ubProgramOrder == 1)) {
								if (   guoFlashUseType.B.btEnterprise
								        && (FQI_CB_Info->uwProgramOrderIndex >= (B0KB_3SHAREPAGE_1STPASS_LOWER_START_INDEX))
								        && (FQI_CB_Info->uwProgramOrderIndex <= (B0KB_3SHAREPAGE_1STPASS_LOWER_END_INDEX))) {
									// Enterprise 8LC 1st Pass Skip Upper Page
									ubWriteDone[1][ubCEIndex] = ubMaxPlane;
									continue;
								}
								if ((FQI_CB_Info->uwProgramOrderIndex >= B0KB_1STSLC_IN_END)) {
									// because page larger 1505 and belonging 1 pass the next page is extra page.
									ubWriteDone[1][ubCEIndex] = ubMaxPlane;
									continue;
								}
							}
#if FLUSHD1_DBG
							if (ubIs2ndDataGroup) {
								UartString(" S ");
							}
							else {
								UartString(" F ");
							}
#endif
							for (ubPlaneIndex = ubWriteDone[ubIs2ndDataGroup][ubCEIndex] + ubInitBurstBank; ubPlaneIndex < ubMaxPlane; ubPlaneIndex += ubPlaneStep) {
								ubFQPlaneIndex = ((ubCEIndex * ubMaxPlane) + ubPlaneIndex) + (ubIs2ndDataGroup * ubFirstGroupCE * ubMaxPlane);
								//M_AssertError(ubFQIndex_W[ubFQPlaneIndex] == 0xFF);
								if (gFQI.gFQLink[ubFQIndex_W[ubFQPlaneIndex]].ubFQPhase != BYTE_FQ_WAITCHECK) {
									//M_SwitchTask();
									break;
								}
								else {
#if 0 //FW_SOURCEUINT_TO_TARGETUINT_ERR_UART
									if (gFQI.gFQLink[ubFQIndex_W[ubFQPlaneIndex]].btPEFail) {
										UartString("\r\n ==CE :");
										Uart_Tx_DataHex(ubCEIndex);
										UartString("\r\n Order :");
										UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
										UartString("\r\n Page :");
										UartWordHex(uw_D3TargetPageIndex);
									}
#endif
									gFQI.gFQLink[ubFQIndex_W[ubFQPlaneIndex]].ubFQPhase = BYTE_FQ_CHECKDONE;
									//ubFQIndex_W[ubFQPlaneIndex] = 0xFF;//Check done not accessed again
									ubWriteDone[ubIs2ndDataGroup][ubCEIndex] += ubPlaneStep;
								}

							}
						}
						if ( (ubWriteDone[0][ubCEIndex] >= ubMaxPlane) && (ubWriteDone[1][ubCEIndex] >= ubMaxPlane) ) {
#if FLUSHD1_DBG
							UartString(" OK");
#endif
							FQI_CB_Info->ubLMUIndex |= WRITE_DONE;
						}
						else {
							continue;
						}
					}
					else {
						if (!(FQI_CB_Info->ubLMUIndex & WRITE_DONE) && (FQI_CB_Info->ubLMUIndex & WRITE_FQ_GET_DONE)) {
							FQI_CB_Info->ubLMUIndex |= WRITE_DONE;
						}
					}
#endif
					// switch to next page and clear some flags for read/copy/write flow
					if ((ubCopyBackMode & COPYBACK_NON_DMA) || (ubCEIndexMAXNUM < FLUSHD1_PARTIAL_CENUM)) { // 1 group  or non DMA case
						ubCondition = (FQI_CB_Info->ubLMUIndex & WRITE_DONE);
					}
					else {// 2 group DMA case
						ubCondition = ((FQI_CB_Info->ubLMUIndex & WRITE_DONE) && (gubFlushD1Flag & (BIT0 << (ubCEIndex % ubFirstGroupCE)))) ;
					}

					if (ubCondition) {
						FQI_CB_Info->ubLMUIndex = LOWER ;
						ubReadDone[0][ubCEIndex] = 0;
						ubWriteDone[0][ubCEIndex] = 0;
						ubReadDone[1][ubCEIndex] = 0;
						ubWriteDone[1][ubCEIndex] = 0;

#if FLUSHD1_DBG
						UartString("\r\nE");
						UartString(" CE ");
						Uart_Tx_DataHex(ubCEIndex);
#endif
						if (ubPartialWork > 1) { //two group case
							ubGroupSel[ubCEIndex % ubFirstGroupCE] = (ubGroupSel[ubCEIndex % ubFirstGroupCE] == 0) ? 1 : 0;//change group when done
							if (ubCEIndexMAXNUM & BIT0) { //odd CE case
								ubGroupSel[ubFirstGroupCE - 1] = 0; // first group last valid CE keep
							}
						}

#if 1 //Debug
						if ((ubCopyBackMode & COPYBACK_NON_DMA)) {
							if (FQI_CB_Info->ubProgramOrder == 1) {
								if (ub2ndPageCnt[ubCEIndex] == 1) { // 2nd Pass Second Page
									FQI_CB_Info->uwProgramOrderIndex = FQI_CB_Info->uwLPage;//use page index save in LPage as Next ProgramOrder
									ub2ndPageCnt[ubCEIndex]--;//minus cnt to 0 back to normal 1st pass state
								}
								else {
									if (   guoFlashUseType.B.btEnterprise
									        && (FQI_CB_Info->uwProgramOrderIndex >= B0KB_3SHAREPAGE_1STPASS_LOWER_START_INDEX)
									        && (FQI_CB_Info->uwProgramOrderIndex <= B0KB_3SHAREPAGE_1STPASS_LOWER_END_INDEX)) {
										FQI_CB_Info->uwProgramOrderIndex += 2; //Enterprise 8LC 1st Pass Skip Upper Page
									}
									else {
										FQI_CB_Info->uwProgramOrderIndex++; //           Normal 1st Pass  just add 1
										//Enterprise Normal 1st Pass  just add 1
									}
								}
							}
							else if (FQI_CB_Info->ubProgramOrder == 2) {
								if (ub2ndPageCnt[ubCEIndex] == 2) { //for 2nd Pass First Page
									FQI_CB_Info->uwLPage = FQI_CB_Info->uwProgramOrderIndex + 1;//record next page at LPage
									FQI_CB_Info->uwProgramOrderIndex = FQI_CB_Info->uwUPage;//and assign Upper Page to Next ProgramOrder
									ub2ndPageCnt[ubCEIndex]--;//minus cnt to 1
								}
							}
						}
						else {//DMA Case
							if ( (FQI_CB_Info->ubProgramOrder == 1) ) { // belonging to 1st pass, and read page, lower -> upper page
								// because page larger 1505 and belonging 1 pass the next page is extra page.
								FQI_CB_Info->uwProgramOrderIndex += ( FQI_CB_Info->uwProgramOrderIndex >= B0KB_1STSLC_IN_END) ? 1 : 2;
							}
							else if (FQI_CB_Info->ubProgramOrder == 2) { // belonging to 2nd pass, and read page, extra -> upper
								FQI_CB_Info->uwProgramOrderIndex++;
							}
						}
#endif
						if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
							ubCheckDoneCnt ++;
						}
					}
				}
				M_SwitchTask();
			}
		}
#endif
		if (FQI_CB_Info->uwProgramOrderIndex > ((guwNum_Wordline * 3U) - 1U)) {
			FQI_CB_Info->uwProgramOrderIndex = 0;
		}


		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}


#if FlushD1SpeedDown
		if ( ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz)) || (gubFlashClock == FLH_CLK_266MHz) ) {
			flaSwitchClock(gubFlashClock);
		}
#endif
		/*
		 * Enable CRC check for normal FW flow at the end
		 */
		FCONL[FCONL_CRC_EN] |= (CRC_32_EN);

		if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {

			UBYTE ubRSFastPage = VT->guwGRTarget[VT->gubGRTargetIndex].B.btFastPage;
			UBYTE ubRSTag2Group[USERDATA_RS_Group_Num + 1];
			U32 ulLastDataPlaneIndex;
			for (ubi = 0; ubi < (USERDATA_RS_Group_Num + 1) ; ubi++) {
				// here ubi use as userdata tag
				ubRSTag2Group[ubi] = 0xFF;
			}
			for (ubi = 0; ubi < USERDATA_RS_Group_Num; ubi++) {
				// here ubi use as group index
				if (ubRSFastPage) {
					Generate_RS_MAP_Index_by_Plane(RS.ulLastDataPlaneIndexFastPage[ubi]);
				}
				else {
					Generate_RS_MAP_Index_by_Plane(RS.ulLastDataPlaneIndex[ubi]);
				}
				ubRSTag2Group[RS.ubTagMAP[RS.ubGroupIndex][RS.ubTagIndex]] = ubi;
			}

			for ( ubi = USERDATA_RSFrameStartIndex; ubi < Table_RSFrameStartIndex; ubi ++) {

				if (VT->guwRSFrameCount[ubi]) {
					RSL[RSL_TAG_SEL] = ubi;
					while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubi);
					RSL[RSL_TAG_VLD_CTL] |= RS_TAG_VALID_CLR;
					while ( (RSB[RSB_TAG_CONFIG_0] & RS_TAG_VALID) == RS_TAG_VALID );
					RSB[RSB_TAG_PEC] = VT->guwRSFrameCount[ubi];

					if (ubi < (USERDATA_RS_Group_Num + 1)) {
						if (ubRSTag2Group[ubi] != 0xFF) {
							// get last data plane index of this tag
							if (ubRSFastPage) {
								ulLastDataPlaneIndex = RS.ulLastDataPlaneIndexFastPage[ubRSTag2Group[ubi]];
							}
							else {
								ulLastDataPlaneIndex = RS.ulLastDataPlaneIndex[ubRSTag2Group[ubi]];
							}
							Generate_RS_MAP_Index_by_Plane(ulLastDataPlaneIndex);
						}
						else {
							ulLastDataPlaneIndex = gulPlanesPerUnit;
						}

						// 條件1: Unit結尾未滿252, 正準備寫Parity, 條件2: 滿252,正準備寫Parity
						if ((( VT->guwRSFrameCount[ubi] >= (RS.uwPlaneOffset + 1) ) && (VT->gulGRTargetPTR > ulLastDataPlaneIndex))\
						        || (VT->guwRSFrameCount[ubi] >= (guwParityPlanesNum - 3 + 1))) {
							gubForceProgramRSParity[ubi] = 1;
						}
					}

					RSL[RSL_HANG] |= RS_TAG_VALID_SET;
				}
			}

			for ( ubi = 0; ubi < TOTAL_PARITY_GROUP_NUM; ubi ++) {
				guwRSFrameDoneCount[ubi] = ubBackUpRSDoneCount[ubi];
			}

			//Restore RS data region from copy buffer.
			mDMAC_COPY(((U32)(&gubCopyBuffer[0])), ((U32)(&guoRSFrameBits[0])), (32 * 1024 * Table_RSFrameStartIndex) , 0, 0, 0, 0, 1);

			//Backup RS spare region to address behind RUT
			mDMAC_COPY( ((U32)(&guwRUTBASE[0] + (RUTSizePerCE * B0KB_MAX_CE))), ((U32)(&gulRSFrameSpareBits[0])),	(Spare4kSize * Table_RSFrameStartIndex * 4 * 2) , 0, 0, 0, 0, 1);
		}

#if FW_SOURCEUINT_TO_TARGETUINT_START_END_UART
		UartString("\r\n END :");
#endif
		M_SwitchTask();
	}

	VT->gulFTLState.B.btDoingFlushD1 = 0;

	return;

}

#else
//Toshiba/Hynix/Sandisk


void Vender_SourceUint_To_TargetUint(UWORD uwFUnit_D3Target/*D3 Targe*/, UWORD uwFD1Unit/*D1 Source*/, UWORD uwFUnit_D3Source/*D3 Source*/, UWORD uwStartProgramOrderIndex, UWORD uwEndProgramOrderIndex, UBYTE ubCopyBackMode)
{
	{
		//收到D1toD3的工作
		//1.先將FQ的事件都處理完畢(Fla做完)
		//2.確保目前沒有FQ及MT待執行(Ftl等完)
		//3.開始執行Parity SourceUint To TargetUint
		//4.直到做完Parity SourceUint To TargetUint

	}
	UBYTE ubCEIndexMAXNUM = gubCENumber;
	UBYTE ubCEIndex;
	UBYTE ubBufferCEIndex;
	UBYTE ubFQIndex_R[MAX_LOGICAL_CE * 2];
	UBYTE ubFQIndex_W[MAX_LOGICAL_CE * 2];
	UBYTE ubFQPlaneIndex;
	UBYTE ubReadDone[MAX_LOGICAL_CE];
	UBYTE ubWriteDone[MAX_LOGICAL_CE];
	FlashQueue_t *FQ;
	UBYTE ubDepth, ubL4kLink;
	UBYTE ubCheckDoneCnt;
	UBYTE ubPlaneIndex = 0;
	UWORD uw_D3TargetPageIndex;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	UBYTE ubL4KCount;
	CB_Info_t *FQI_CB_Info;
	UBYTE ubMaxPlane = (guoFlashUseType.B.btTwoPlane ? 2 : 1);
	UBYTE ubRSTag;
	U32 ulSourceAddr, ulTargetAddr;
	UBYTE ubPartialWork = 1, ubi;
#if FLUSHD1_PARTIAL_CE_ADVANCE
	UBYTE ubPartialSelect[MAX_LOGICAL_CE];
#endif

	U32 ulTempReadD1UNCCnt = VT->gulReadUNCCnt_D1;

#if (ENABLE_DEBUG_D1TOD3_ERR_HANDLE)
	U32 ulTempReadD1RSCnt = VT->gulReadRSCnt_D1;
	U32 ulTempProgramD3FailCnt = VT->gulProgramFailCnt_D3;
#endif

	UBYTE ubPlaneStep = (gubBurstsPerBank) / (((gubRUTMixPlaneEnable) && (gulEC_D3[uwFUnit_D3Target].B.btMixPlaneUnit) ) ? 1 : gubBurstsPerBank);
	UBYTE ubInitBurstBank = (ubCopyBackMode / COPYBACK_PLANE_INDEX);
	VT->gulFTLState.B.btDoingFlushD1 = 1;

#if FLUSHD1_PARTIAL_CE_ADVANCE
	gubFlushD1Flag = 0xff;
#endif

	if (uwFUnit_D3Target >= 4096) {
		gubTempRUTD1LinkValue = uwFD1Unit;
	}
	////////////////////////////////////////////////////////////////////
	//else {
	//	gubD1LinkTable[uwFUnit_D3Target].B.ubD1Link = (UBYTE)uwFD1Unit;
	//}
	////////////////////////////////////////////////////////////////////

	{

		/* Disable CRC check for whole flush procedure
		     * Since parity page will hit CRC error
		     */
		while (gFQI.ubFQLinkNumber) { //確保目前沒有FQ及MT待執行(Ftl等完)
			M_SwitchTask();
		}
		FCONL[FCONL_CRC_EN] &= ~(CRC_32_EN);

		for (ubCEIndex = 0; ubCEIndex < ubCEIndexMAXNUM; ubCEIndex++) {
			FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];
			FQI_CB_Info->uwStartProgramOrderIndex = uwStartProgramOrderIndex;
			FQI_CB_Info->uwEndProgramOrderIndex = uwEndProgramOrderIndex;

			if (FQI_CB_Info->uwStartProgramOrderIndex != FQI_CB_Info->uwProgramOrderIndex) {
				//UartString("\r\n uwStartProgramOrderIndex != uwProgramOrderIndex ===========Error");
			}
			if (FQI_CB_Info->uwStartProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
				//UartString("\r\n uwStartProgramOrderIndex > uwEndProgramOrderIndex ===========Error");
			}

			FQI_CB_Info->uwProgramOrderIndex = uwStartProgramOrderIndex;
			FQI_CB_Info->ubLMUIndex = LOWER;
			ubReadDone[ubCEIndex] = 0;
			ubWriteDone[ubCEIndex] = 0;
#if FLUSHD1_PARTIAL_CE_ADVANCE
			ubPartialSelect[ubCEIndex] = 0;
#endif
		}

		ubCheckDoneCnt = 0;
		ubCEIndex = 0;
		FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];
#if FW_SOURCEUINT_TO_TARGETUINT_START_END_UART
		UartString("\r\n SourceUint To TargetUint START :");
#endif

		if ((ubCopyBackMode & COPYBACK_NON_DMA) == 0) { //Data in out
			ubPartialWork = gubExpandCE_Ratio;
		}
		UBYTE ubPartialDoingCENum = ubCEIndexMAXNUM / ubPartialWork;
#if FLUSHD1_PARTIAL_CE_ADVANCE
		if (ubPartialDoingCENum > FLUSHD1_PARTIAL_CENUM) {
			M_AssertError(ubPartialDoingCENum % FLUSHD1_PARTIAL_CENUM);
			ubPartialWork *= (ubPartialDoingCENum / FLUSHD1_PARTIAL_CENUM);
			ubPartialDoingCENum = FLUSHD1_PARTIAL_CENUM;
		}
		gubPartialDoingCENum = ubPartialDoingCENum;

		//Check Total Done
		while (ubCheckDoneCnt < ubCEIndexMAXNUM) { // Check Total Done
			for (ubi = 0; ubi < ubPartialWork; ubi++) { // CE partial work. change CE group by wordline
#else
		for (ubi = 0; ubi < ubPartialWork ; ubi++) {   //超過最大架構For 16CE 2Plane, Data in Out要分次做 ; 一個Queue只做一個CE的量 否則有可能卡住
			//Check Total Done
			while (ubCheckDoneCnt < ubPartialDoingCENum) {
#endif

				//CE
				for (ubCEIndex = ubi * ubPartialDoingCENum; ubCEIndex < (ubi + 1)*ubPartialDoingCENum; ubCEIndex++) {

					FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];

					ubBufferCEIndex = ubCEIndex - ubi * ubPartialDoingCENum;

#if FLUSHD1_PARTIAL_CE_ADVANCE
					if ((ubPartialWork >= 2) && (ubi != ubPartialSelect[ubBufferCEIndex])) {
						continue;
					}
#endif

					if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
						continue;
					}


#if TLC_BICS2
					FQI_CB_Info->ubProgramOrder = FINE_ORDER;
					FQI_CB_Info->uwD3TargetWL = FQI_CB_Info->uwProgramOrderIndex; // program order 就是wordline order
#else /* TLC_BICS2 */
					//取得FQ->uwProgramOrder , First, foggy, fine
					Get_First_Foggy_Fine(FQI_CB_Info);

					//取得FQ->uwD3TargetWL,
					Get_D3_WL(FQI_CB_Info);
#endif /* TLC_BICS2 */

					//LMU-D3 Page Index
					uw_D3TargetPageIndex = ((FQI_CB_Info->uwD3TargetWL * 3) + (FQI_CB_Info->ubLMUIndex & 0x03));

					if ((ubCopyBackMode & COPYBACK_NON_DMA) && ((gFQ_NUM - gFQI.ubFQLinkNumber) < (ubMaxPlane << 1))) {
						M_SwitchTask();
						continue;//R+W 不夠FQ
					}
					if ((ubCopyBackMode & COPYBACK_NON_DMA) && ((FQ_NUM - gFQI.ubFQDoingNumber[ubCEIndex % gubMTQTotalPhyCE]) < (ubMaxPlane << 1))) {
						M_SwitchTask();
						continue;//怕佔著茅坑不拉屎
					}

					if ((!(ubCopyBackMode & COPYBACK_NON_DMA)) && READ_IN && ((gFQ_NUM - gFQI.ubFQLinkNumber) < ubMaxPlane)) {
						continue;//R不夠FQ
					}
					if ((!(ubCopyBackMode & COPYBACK_NON_DMA)) && READ_IN && ((FQ_NUM - gFQI.ubFQDoingNumber[ubCEIndex % gubMTQTotalPhyCE]) < ubMaxPlane)) {
						continue;//怕佔著茅坑不拉屎
					}

					//Read
					if (READ_IN && (!(FQI_CB_Info->ubLMUIndex & READ_FQ_GET_DONE))) {

						for (ubPlaneIndex = ubInitBurstBank ; ubPlaneIndex < ubMaxPlane ; ubPlaneIndex += ubPlaneStep) {

							ubFQPlaneIndex = ((ubBufferCEIndex * ubMaxPlane) + ubPlaneIndex);
							ubFQIndex_R[ubFQPlaneIndex] = gFQI.ubFQLinkFree;
							M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
							FQ->btCheckEmpty = 0;

							FQ->ulRAMAddr = (U32)&gubCopyBuffer[ubFQPlaneIndex * (gubSectorsPerPlane << 9)];
							if (NEW_DATA_BASE_EN) {
#if TLC_BICS2
								ubRSTag = (GC_RSFrameStartIndex + ubBufferCEIndex);
#else /* TLC_BICS2 */
								ubRSTag = (TOTAL_PARITY_GROUP_NUM + ubBufferCEIndex);
#endif /* TLC_BICS2 */
								FQ->ulRAMAddr = ((U32)(&guoRSFrameBits[((ubRSTag * 32 * 1024) + (ubPlaneIndex * 16384)) / 8]));
							}
							FQ->uwFUnit = (ubCopyBackMode & COPYBACK_D3_TO_D3) ? uwFUnit_D3Source /*D3 Source */ : uwFUnit_D3Target /*D1 Source */;
							FQ->ulFEntry = ((M_Multiplier((U32)uw_D3TargetPageIndex, DEF_4KEntryPerPage_Mcl_Div)) + (((ubCEIndex << gubBurstsPerBankLog) + ubPlaneIndex) << gub4kEntrysPerPlaneLog)) ;
							FQ->btPreRead = 0;
							FQ->ubFJob = BYTE_FJOB_READ;
							FQ->uwFJobInfo = ( ((ubCopyBackMode & COPYBACK_NON_DMA) ? BIT_FJOBI_NON_DMA : 0 ) | ((ubCopyBackMode & COPYBACK_D3_TO_D3) ? 0 : BIT_FJOBI_FASTPAGE) | ((ubCopyBackMode & COPYBACK_NON_DMA) ? 0 : BIT_FJOBI_WAITRESULT) /* | BIT_FJOBI_DIRECTACESS*/ | BIT_FJOBI_RETRY | BIT_FJOBI_RS);
							FQ->ubDepth = ubDepth;
							FQ->ubL4kIndex = ubL4kLink;
							FQ->btUNServed = 1;
							FQ->btD3 = (ubCopyBackMode & COPYBACK_D3_TO_D3) ? 1 : 0;
							FQ->btNoWait = 1;

							FQ->ubD3WriteMode = (ubCopyBackMode & COPYBACK_D3_TO_D3) ? 0 : D3_SUSPEND_EN;

							if (uwFUnit_D3Target >= 4096) {
								FQ->uwFJobInfo |= BIT_FJOBI_RUT_D1LINK;
								FQ->ubFSAIndex = gubTempRUTD1LinkValue;
							}

							if (FQ->uwFJobInfo & BIT_FJOBI_WAITRESULT) {
								//[CC] : 有拉 Wait Result 且後面有判斷 Fail會做 ftlRecordFailLog 時, 要舉 btRecordFailLogByMyself
								// 讓底層發生 Read Retry 且 1) SB Pass or 2) RS Pass 的 Case 時, 不記 VT Fail Log
								FQ->btRecordFailLogByMyself = 1;
							}

#if (ENABLE_DEBUG_D1TOD3_ERR_HANDLE)
							{
								// Debug Version
								//if (VT->gulReadRSCnt_D1 > ulTempReadD1RSCnt) {
								// Release Version
								if (VT->gulReadUNCCnt_D1 > ulTempReadD1UNCCnt) {
									// 有造到 UNC Fail 即可
									gubGenReadFailDebug = STOP_GEN_FAIL;
								}
							}
#endif

							gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
							gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
							gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
							gulPara0x24.BitMap.btZipBypass = 1;
							gulPara0x24.BitMap.btCmpEn = 0;
							gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
							gulPara0x24.BitMap.btBufModeEn = 0;
							gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
							gMTQ->Depth[ubDepth].ulZipMap = 0xFFFFFFFF;


							for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
								pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
								//set L4K table info
								pL4KTable.ulL4K_LCA = 0xFFFFFFFF;
								pL4KTable.Para0x04.ulAll = 0;
								pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0;
								pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
								pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
								pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
								pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
								*pL4KTablePtr = pL4KTable;
							}

							gFQI.ubFQLinkNumber++;
							gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

						}

						FQI_CB_Info->ubLMUIndex |= READ_FQ_GET_DONE;

					}

					if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
						M_SwitchTask();
					}


					if (!(ubCopyBackMode & COPYBACK_NON_DMA) && READ_IN && (FQI_CB_Info->ubLMUIndex & READ_FQ_GET_DONE)) {
						//檢查Read做完沒
						for (ubPlaneIndex = ubReadDone[ubBufferCEIndex] + ubInitBurstBank; ubPlaneIndex < ubMaxPlane; ubPlaneIndex += ubPlaneStep) {
							ubFQPlaneIndex = ((ubBufferCEIndex * ubMaxPlane) + ubPlaneIndex);
							if (gFQI.gFQLink[ubFQIndex_R[ubFQPlaneIndex]].ubFQPhase != BYTE_FQ_WAITCHECK) {
								break;
							}

							//2 =============================
							if (USE_RS_IN_OUT && (!(ubCopyBackMode & COPYBACK_NON_DMA))) {
								//最大架構For 16CE 2Plane
								//超過最大架構需要改方法, 問題在於RS Tag的空間不構By CE, Plane
#if TLC_BICS2
								ubRSTag = (GC_RSFrameStartIndex + ubBufferCEIndex);
#else /* TLC_BICS2 */
								ubRSTag = (TOTAL_PARITY_GROUP_NUM + ubBufferCEIndex);
#endif /* TLC_BICS2 */
								//Spare
								ulSourceAddr = ((U32)( L4K_TABLE_ADDR + ((gFQI.gFQLink[ubFQIndex_R[ubFQPlaneIndex]].ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize)));
								ulTargetAddr = ((U32)(&gulRSFrameSpareBits[((ubRSTag * 128) + (ubPlaneIndex * 64) ) / 4]));
								if (CPU_COPY_EN) {
									U32 ulByteLength = 16 * ubL4KNum;
									// Step 1 : 先 invalidate 要使用的 memory 範圍, 使之對應 DCache 中的 Data 無效
									if (ENABLE_DCACHE) {
										if ((ulTargetAddr >= Z2_DDR_BASE) && (ulTargetAddr < Z3_DDR_BASE)) {
											xthal_dcache_region_invalidate((void *)ulTargetAddr, ulByteLength);
										}
									}
									// Disable DRAM ECC 之前先確定 DDR 沒人在操作
									//while ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0);

									// Disable ZONE2 ECC
									//SYS1L[SYS1L_ECC_EN] &= ~(CR_Z2_ECC_EN);

									//… Do Something …
									// Step 2 : Copy Data to Target Memory
									memcpy((void *)(ulTargetAddr), (void *)(ulSourceAddr), ulByteLength);

									// Step 3 : WriteBack from DCache to Memory
									if (ENABLE_DCACHE) {
										if ((ulTargetAddr >= Z2_DDR_BASE) && (ulTargetAddr < Z3_DDR_BASE)) {
											xthal_dcache_region_writeback_inv((void *)ulTargetAddr, ulByteLength);
										}
									}
									// Enable DRAM ECC 之前先確定 DDR 沒人在操作
									//while ((RAML[RAML_DDR3_PHY_CFG] & DDR_CTRL_EMPTY) == 0);

									// Enable ZONE2 ECC
									//SYS1L[SYS1L_ECC_EN] |= CR_Z2_ECC_EN;

								}
								else {
									mDMAC_COPY((U32)ulSourceAddr, (U32)ulTargetAddr, 16 * ubL4KNum , 0, 0, 0, 0, 1);
								}

								if (!NEW_DATA_BASE_EN) {
									//Data
									ulSourceAddr = ((U32)(&gubCopyBuffer[(ubFQPlaneIndex * (gubSectorsPerPlane << 9))]));
									ulTargetAddr = ((U32)(&guoRSFrameBits[((ubRSTag * 32 * 1024) + (ubPlaneIndex * 16384)) / 8]));
									mDMAC_COPY((U32)ulSourceAddr, (U32)ulTargetAddr, 4096 * ubL4KNum , 0, 0, 0, 0, 1);
								}

							}
							//2 =============================


							gFQI.gFQLink[ubFQIndex_R[ubFQPlaneIndex]].ubFQPhase = BYTE_FQ_CHECKDONE;
							ubReadDone[ubBufferCEIndex] += ubPlaneStep;
#if FLUSHD1_PARTIAL_CE_ADVANCE
							if ((ubPlaneIndex >= (ubMaxPlane - 1)) || (ubPlaneStep > 1)) {
								// Read done, clean flush flag by CE
								gubFlushD1Flag &= ~(BIT0 << ubBufferCEIndex);
							}
#endif
							M_SwitchTask();
						}
					}

					if ((gubForceFlushD1ByIgnoreFail == 0) && (!(ubCopyBackMode & COPYBACK_NON_DMA)) && (VT->gulReadUNCCnt_D1 > ulTempReadD1UNCCnt)) {
						//若 Data-In-Out Read D1 有 UNC, 檢查目前若無 FQ 在執行 即可直接跳出
						if (gFQI.ubFQLinkNumber == 0) {
							FCONL[FCONL_CRC_EN] |= (CRC_32_EN);
							VT->gulFTLState.B.btDoingFlushD1 = 0;
							return;
						}
						else {
							M_SwitchTask();
						}
					}

					if ((!(ubCopyBackMode & COPYBACK_NON_DMA)) && WRITE_OUT && ((gFQ_NUM - gFQI.ubFQLinkNumber) < ubMaxPlane)) {
						continue;//W不夠FQ
					}
					if ((!(ubCopyBackMode & COPYBACK_NON_DMA)) && WRITE_OUT && ((FQ_NUM - gFQI.ubFQDoingNumber[ubCEIndex % gubMTQTotalPhyCE]) < ubMaxPlane)) {
						continue;//怕佔著茅坑不拉屎
					}

					//Write
					if (WRITE_OUT && (!(FQI_CB_Info->ubLMUIndex & WRITE_FQ_GET_DONE))) {

						if ((gubForceFlushD1ByIgnoreFail == 0) && (VT->gulReadUNCCnt_D1 > ulTempReadD1UNCCnt)) {
							//若 Read D1 過程中有 UNC, 放棄後面所有的 Program, 等待 Read FQ 做完後即可 return
							//UartString("\n W bypass.");
							if (gFQI.ubFQLinkNumber == 0) {
								FCONL[FCONL_CRC_EN] |= (CRC_32_EN);
								VT->gulFTLState.B.btDoingFlushD1 = 0;
								return;
							}

							if ((ubCopyBackMode & COPYBACK_NON_DMA) || ubPlaneIndex >= ubMaxPlane) {
								FQI_CB_Info->ubLMUIndex |= WRITE_FQ_GET_DONE;
							}
							else {
								continue;//Read還沒做完
							}
						}
						else {
							if ((ubCopyBackMode & COPYBACK_NON_DMA) || ubPlaneIndex >= ubMaxPlane) {

								for (ubPlaneIndex = ubInitBurstBank; ubPlaneIndex < ubMaxPlane ; ubPlaneIndex += ubPlaneStep) {

									ubFQPlaneIndex = ((ubBufferCEIndex * ubMaxPlane) + ubPlaneIndex);
									ubFQIndex_W[ubFQPlaneIndex] = gFQI.ubFQLinkFree;
									M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
									FQ->btCheckEmpty = 0;
									FQ->ulRAMAddr = (U32)&gubCopyBuffer[ubFQPlaneIndex * (gubSectorsPerPlane << 9)];
									if (NEW_DATA_BASE_EN) {
#if TLC_BICS2
										ubRSTag = (GC_RSFrameStartIndex + ubBufferCEIndex);
#else /* TLC_BICS2 */
										ubRSTag = (TOTAL_PARITY_GROUP_NUM + ubBufferCEIndex);
#endif /* TLC_BICS2 */
										FQ->ulRAMAddr = ((U32)(&guoRSFrameBits[((ubRSTag * 32 * 1024) + (ubPlaneIndex * 16384)) / 8]));
									}
									FQ->uwFUnit = uwFUnit_D3Target /*D3 Target */;
									FQ->ulFEntry = ((M_Multiplier((U32)uw_D3TargetPageIndex, DEF_4KEntryPerPage_Mcl_Div)) + (((ubCEIndex << gubBurstsPerBankLog) + ubPlaneIndex) << gub4kEntrysPerPlaneLog)) ;
									FQ->btPreRead = 0;
									FQ->ubFJob = BYTE_FJOB_WRITE;
									FQ->uwFJobInfo = ( ((ubCopyBackMode & COPYBACK_NON_DMA) ? BIT_FJOBI_NON_DMA : 0 ) | (((FQI_CB_Info->ubProgramOrder == FINE_ORDER) && ((FQI_CB_Info->ubLMUIndex & 0x03) == UPPER)) ? (((ubCopyBackMode & COPYBACK_NON_DMA) || WRITE_NON_CHECK) ? 0 : BIT_FJOBI_WAITRESULT) : 0) | BIT_FJOBI_FAILHANDLE_RIGHTNOW /* | BIT_FJOBI_DIRECTACESS*/ /*| BIT_FJOBI_FASTPAGE*/);
									FQ->ubRelativeDepth = ubDepth;
									FQ->ubDepth = 0xFF;
									FQ->ubL4kIndex = ubL4kLink;
									FQ->btUNServed = 1;
									FQ->btD3 = 1;
									FQ->btNoWait = 1;
									FQ->ubD3WriteMode = (D3_SUSPEND_EN | (D3_WRITE_FIRST << FQI_CB_Info->ubProgramOrder) | (D3_WRITE_LOWDER << (FQI_CB_Info->ubLMUIndex & 0x03)));

									if ((ubPlaneIndex + ubPlaneStep) >= ubMaxPlane ) {
#if Hynix//CopyBack Disable Cache Porgram So Upper Page Wait True RDY
										FQ->ubD3WriteMode = (D3_SUSPEND_EN | (D3_WRITE_FIRST << FQI_CB_Info->ubProgramOrder) | ((((FQI_CB_Info->ubLMUIndex & 0x03) == UPPER)) ? D3_WRITE_UPPER_END : (D3_WRITE_LOWDER << (FQI_CB_Info->ubLMUIndex & 0x03))));
#else
										FQ->ubD3WriteMode = (D3_SUSPEND_EN | (D3_WRITE_FIRST << FQI_CB_Info->ubProgramOrder) | (((FQI_CB_Info->uwProgramOrderIndex == FQI_CB_Info->uwEndProgramOrderIndex) && ((FQI_CB_Info->ubLMUIndex & 0x03) == UPPER)) ? D3_WRITE_UPPER_END : (D3_WRITE_LOWDER << (FQI_CB_Info->ubLMUIndex & 0x03))));
#endif
									}
									FQ->btIsMixPlaneUnit = (ubPlaneStep > 1);
									if (uwFUnit_D3Target >= 4096) {
										FQ->uwFJobInfo |= BIT_FJOBI_RUT_D1LINK;
										FQ->ubFSAIndex = gubTempRUTD1LinkValue;
									}


#if (ENABLE_DEBUG_D1TOD3_ERR_HANDLE && ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL)
									{
										FQ->btGenFail = 1;
										if (VT->gulProgramFailCnt_D3 > ulTempProgramD3FailCnt) {
											// 有造到 Fail 即可
											gubGenProgramFailDebug = STOP_GEN_FAIL;
										}
									}
#endif

									gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (FQ->ubL4kIndex * L4kQ_BIND_FACTOR * Spare4kSize);
									gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
									gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
									gulPara0x24.BitMap.btZipBypass = 1;
									gulPara0x24.BitMap.btCmpEn = 0;
									gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
									gulPara0x24.BitMap.btBufModeEn = 0;
									gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
									gMTQ->Depth[ubDepth].ulZipMap = 0xFFFFFFFF;

									if (USE_RS_IN_OUT && (!(ubCopyBackMode & COPYBACK_NON_DMA))) {
										//最大架構For 16CE 2Plane
										//超過最大架構需要改方法, 問題在於RS Tag的空間不構By CE, Plane
										FQ->btParity = 1;
#if TLC_BICS2
										ubRSTag = (GC_RSFrameStartIndex + ubBufferCEIndex);
#else /* TLC_BICS2 */
										ubRSTag = (TOTAL_PARITY_GROUP_NUM + ubBufferCEIndex);
#endif /* TLC_BICS2 */
										FQ->ubRSFrameIndex = ubRSTag;
										FQ->btRSOneParityEn = 0;
										FQ->ubRSPageNum = 0xFF;
										FQ->btRSProg2ndParity = ubPlaneIndex;
										FQ->btRSLastPage = 0;

									}
									else {

										for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
											pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
											pL4KTable = *pL4KTablePtr;
											//set L4K table info
											pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
											pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
											*pL4KTablePtr = pL4KTable;
										}

									}

									gFQI.ubFQLinkNumber++;
									gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

								}

								FQI_CB_Info->ubLMUIndex |= WRITE_FQ_GET_DONE;
							}
							else {
								continue;//Read還沒做完
							}
						}
					}

					if (!(ubCopyBackMode & COPYBACK_NON_DMA)) {
						M_SwitchTask();
					}


					//檢查Write做完沒
					if (!((ubCopyBackMode & COPYBACK_NON_DMA) || WRITE_NON_CHECK) && WRITE_OUT && (FQI_CB_Info->ubProgramOrder == FINE_ORDER) && ((FQI_CB_Info->ubLMUIndex & 0x03) == UPPER)) {
						for (ubPlaneIndex = ubWriteDone[ubBufferCEIndex] + ubInitBurstBank ; ubPlaneIndex < ubMaxPlane; ubPlaneIndex += ubPlaneStep) {
							ubFQPlaneIndex = ((ubBufferCEIndex * ubMaxPlane) + ubPlaneIndex);

							if (gFQI.gFQLink[ubFQIndex_W[ubFQPlaneIndex]].ubFQPhase != BYTE_FQ_WAITCHECK) {
								break;
							}
							gFQI.gFQLink[ubFQIndex_W[ubFQPlaneIndex]].ubFQPhase = BYTE_FQ_CHECKDONE;
							ubWriteDone[ubBufferCEIndex]++;
							if (gFQI.gFQLink[ubFQIndex_W[ubFQPlaneIndex]].btPEFail) {
#if FW_SOURCEUINT_TO_TARGETUINT_ERR_UART
								UartString("\r\n ==CE :");
								Uart_Tx_DataHex(ubCEIndex);
								UartString("\r\n Order :");
								UartWordHex(FQI_CB_Info->uwProgramOrderIndex);
#endif
							}
							M_SwitchTask();
						}
					}
					else {
						ubPlaneIndex = ubMaxPlane;
					}

#if FLUSHD1_PARTIAL_CE_ADVANCE
					if ((ubPlaneIndex >= ubMaxPlane)
					        && ((ubPartialWork < 2) || (( (FQI_CB_Info->ubLMUIndex & 0x03) != UPPER) && ( (FQI_CB_Info->ubLMUIndex & 0x03) != UPPER_END)) || (gubFlushD1Flag & (BIT0 << ubBufferCEIndex)))) {
#else
					if (ubPlaneIndex >= ubMaxPlane) {
#endif
						//Next Order, LMU
						FQI_CB_Info->ubLMUIndex &= (~(WRITE_FQ_GET_DONE | READ_FQ_GET_DONE )) ;
						FQI_CB_Info->ubLMUIndex ++;

						if ((FQI_CB_Info->ubLMUIndex & 0x03) == 0x03) {
							FQI_CB_Info->uwProgramOrderIndex++;
							if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
								ubCheckDoneCnt ++;
							}
							FQI_CB_Info->ubLMUIndex = LOWER ;
						}
						ubReadDone[ubBufferCEIndex] = 0;
						ubWriteDone[ubBufferCEIndex] = 0;
#if FLUSHD1_PARTIAL_CE_ADVANCE
						if (FQI_CB_Info->ubLMUIndex == LOWER) {
							ubPartialSelect[ubBufferCEIndex] = ((ubi + 1) & (ubPartialWork - 1));
						}
#endif
					}

				}

			}
#if (!FLUSHD1_PARTIAL_CE_ADVANCE)
			ubCheckDoneCnt = 0;
#endif
		}
		for (ubCEIndex = 0; ubCEIndex < ubCEIndexMAXNUM; ubCEIndex++) {
			FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];

			if (FQI_CB_Info->uwProgramOrderIndex > ((guwNum_Wordline * TLC_PROGRAM_SEQ_CNT) - 1U)) {
				FQI_CB_Info->uwProgramOrderIndex = 0;
			}
		}
		/*
		     * Enable CRC check for normal FW flow at the end
		     */
		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}
		FCONL[FCONL_CRC_EN] |= (CRC_32_EN);
#if FW_SOURCEUINT_TO_TARGETUINT_START_END_UART
		UartString("\r\n END :");
#endif
		M_SwitchTask();
	}

	VT->gulFTLState.B.btDoingFlushD1 = 0;
	return;
}
#endif

void Vender_D1D3_VerifyECC_FirstFrame(UWORD uwFUnit/*D3*/, UWORD uwFD1Unit/*D1*/, UBYTE ubVerifyMode)
{
	// support first frame of all page.
	UBYTE ubCEIndex;
	UBYTE ubFQPlaneIndex;
	FlashQueue_t *FQ;
	UBYTE ubDepth, ubL4kLink;
	UBYTE ubL4KNum = 1;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	UBYTE ubL4KCount;
	UBYTE ubD3 = 1;

	U32 ulEntry;
#if (ENABLE_DEBUG_D1TOD3_ERR_HANDLE)
	U32 ulTempVerifyD3RSorUNCCnt = VT->gulReadRSCnt_D3 + VT->gulReadUNCCnt_D3;
#endif

	if (uwFUnit >= 4096) {
		gubTempRUTD1LinkValue = uwFD1Unit;
	}

	if (!ubVerifyMode) {
		return;
	}

	VT->gulFTLState.B.btDoingFlushD1 = 1;

	{
		/* Disable CRC check for whole flush procedure
		     * Since parity page will hit CRC error
		     */
		while (gFQI.ubFQLinkNumber) { //確保目前沒有FQ及MT待執行(Ftl等完)
			M_SwitchTask();
		}

		FCONL[FCONL_CRC_EN] &= ~(CRC_32_EN);

		for (ulEntry = 0; ulEntry < gul4kEntrysPerUnit ; ulEntry += gub4kEntrysPerPlane) {
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular(((ulEntry >> gub4kEntrysPerPlaneLog) >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (((ulEntry) >> gub4kEntrysPerPlaneLog)&gubPlanesPerBurstMask) + ((M_Modular((((ulEntry) >> gub4kEntrysPerPlaneLog) >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}

			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->btCheckEmpty = 0;
			FQ->ulRAMAddr = (U32)&gubCopyBuffer[ubFQPlaneIndex * (gubSectorsPerPlane << 9)];
			FQ->uwFUnit = uwFUnit;
			FQ->ulFEntry = ulEntry;
			FQ->btPreRead = 0;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = ( (ubD3 ? 0 : BIT_FJOBI_FASTPAGE) | BIT_FJOBI_RETRY | (ENABLE_RUT_SWAP ? BIT_FJOBI_FAILHANDLE_RIGHTNOW : 0));
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->ubL4kNum = ubL4KNum;
			FQ->btUNServed = 1;
			FQ->btDirectHandling = (gubDirectHandl_RUTLog == 1) ? 1 : 0;
			FQ->btD3 = ubD3;
			FQ->btNoWait = 0;
			FQ->ubD3WriteMode = 0;

			if (uwFUnit >= 4096) {
				FQ->uwFJobInfo |= BIT_FJOBI_RUT_D1LINK;
				FQ->ubFSAIndex = gubTempRUTD1LinkValue;
			}

			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = 0xFFFFFFFF;

			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = 0xFFFFFFFF;
				pL4KTable.Para0x04.ulAll = 0;
				pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0x00;//DATA不進 SDR RAM
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}

			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;


		}

		/*
		     * Enable CRC check for normal FW flow at the end
		     */
		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}

		FCONL[FCONL_CRC_EN] |= (CRC_32_EN);
#if ECC_VERIFY_START_END_UART
		UartString("\r\n VerifyECC END :");
#endif
		M_SwitchTask();
	}

	VT->gulFTLState.B.btDoingFlushD1 = 0;
	return;



}

void Vender_D1D3_VerifyECC(UWORD uwFUnit/*D3*/, UWORD uwFD1Unit/*D1*/, UWORD uwStartPageIndex, UWORD uwEndPageIndex, UBYTE ubCopyBackMode, UBYTE ubVerifyMode)
{
	UBYTE ubCEIndexMAXNUM = gubCENumber;
	UBYTE ubCEIndex;
	UBYTE ubFQPlaneIndex;
	FlashQueue_t *FQ;
	UBYTE ubDepth, ubL4kLink;
	UBYTE ubCheckDoneCnt;
	UBYTE ubi = 0;
	UBYTE ubPlaneIndex = 0;
	UWORD uw_D3TargetPageIndex;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	UBYTE ubL4KCount;
	//CB_Info_t FQI_CB_Info_tmp;
	CB_Info_t *FQI_CB_Info;
	UBYTE ubMaxPlane = ( guoFlashUseType.B.btFourPlane ? 4 : (guoFlashUseType.B.btTwoPlane ? 2 : 1));
	UBYTE ubD3 = 0;

	UBYTE ubPartialWork = 1, ubPartialIndex;
	UBYTE ubBufferCEIndex = 0;
	UBYTE ubCELimit = FQ_NUM;
#if Hynix3DV6
	if (gubCENumber > 8) {
		ubCELimit = 8;
	}
#endif


#if B0KB
	UBYTE ubBypassCheck;
	UWORD uwVerifyPageCnt = 0;
#endif

#if (ENABLE_DEBUG_D1TOD3_ERR_HANDLE)
	U32 ulTempVerifyD3RSorUNCCnt = VT->gulReadRSCnt_D3 + VT->gulReadUNCCnt_D3;
#endif


	if (uwFUnit >= 4096) {
		gubTempRUTD1LinkValue = uwFD1Unit;
	}

	if (!ubVerifyMode) {
		return;
	}

	VT->gulFTLState.B.btDoingFlushD1 = 1;

	if (ENABLE_DEBUG_UART_CC && 0) {
		UartString("\n\r FUnit : 0x");
		UartWordHex(uwFUnit);
		UartString("\n\r FD1Unit : 0x");
		UartWordHex(uwFD1Unit);
		UartString("\n\r D1LinkTable : 0x");
		Uart_Tx_DataHex(gubD1LinkTable[uwFUnit].B.ubD1Link);
	}

	{
		/* Disable CRC check for whole flush procedure
		     * Since parity page will hit CRC error
		     */
		while (gFQI.ubFQLinkNumber) { //確保目前沒有FQ及MT待執行(Ftl等完)
			M_SwitchTask();
		}

		FCONL[FCONL_CRC_EN] &= ~(CRC_32_EN);

#if(!(TLC_BICS2 || B0KB || Hynix3DV6 || Hynix3DV5 || YMTC_JGS))
		if (ubVerifyMode & D3_ECC_VERIFY_CHECK) {

			//取得最後一個可以Read的Middle Page
			//Order index -> Middle Page Index
			UBYTE ubProgramOrder = 0;	//First, Foggy, Fine
			UWORD uwD3TargetWL = 0;

		MARK_ReCheckVaildEndPageIndex:
			//取得First, Foggy, Fine,
			if (uwEndPageIndex < 2) {
				ubProgramOrder = 0;//First
			}
			else if ((uwEndPageIndex == 2) || (uwEndPageIndex == (guwPagesPerUnit - 3))) {
				ubProgramOrder = 1;//Foggy
			}
			else if (uwEndPageIndex > (guwPagesPerUnit - 3)) {
				ubProgramOrder = 2;//Fine
			}
			else {
				if (((uwEndPageIndex - 3) % 3) == 0) {
					ubProgramOrder = 0;//First
				}
				else if (((uwEndPageIndex - 3) % 3) == 1) {
					ubProgramOrder = 1;//Foggy
				}
				else if (((uwEndPageIndex - 3) % 3) == 2) {
					ubProgramOrder = 2;//Fine
				}
			}

			//取得ubD3TargetWL
			if (uwEndPageIndex == 0 || uwEndPageIndex == 2 || uwEndPageIndex == 5) {
				uwD3TargetWL = 0;
			}
			else if (uwEndPageIndex == 1 || uwEndPageIndex == 4) {
				uwD3TargetWL = 1;
			}
			else if (uwEndPageIndex == (guwPagesPerUnit - 2)) {
				uwD3TargetWL = guwNum_Wordline - 2;
			}
			else if (uwEndPageIndex == (guwPagesPerUnit - 3) || uwEndPageIndex == (guwPagesPerUnit - 1) ) {
				uwD3TargetWL = guwNum_Wordline - 1;
			}
			else {
				uwD3TargetWL = (((uwEndPageIndex - (ubProgramOrder << 2)) / 3) + 1);
			}

			if (ubProgramOrder == 2) {
				//do nothing
			}
			else {
				if (uwEndPageIndex) {
					uwEndPageIndex--;
					goto MARK_ReCheckVaildEndPageIndex;
				}

				VT->gulFTLState.B.btDoingFlushD1 = 0;
				return;
			}

		}
#endif

		//memcpy((void*)&FQI_CB_Info_tmp, &gFQI.gCB_Info_t[0], sizeof(FQI_CB_Info_tmp));

		for (ubCEIndex = 0; ubCEIndex < ubCEIndexMAXNUM; ubCEIndex++) {
			FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];

			FQI_CB_Info->ubLMUIndex = LOWER;

#if (!B0KB)
			if (ubVerifyMode & D3_ECC_VERIFY_CHECK) {
				if ((uwStartPageIndex % 3) == LOWER) {
					//is LOW
					if (!(ubVerifyMode & D3_ECC_VERIFY_L)) {
						if (ubVerifyMode & D3_ECC_VERIFY_M) {
							uwStartPageIndex = uwStartPageIndex + 1;//調成Middle Index
						}
						else {
							uwStartPageIndex = uwStartPageIndex + 2;//調成Upper Index
						}
					}
				}
				else if ((uwStartPageIndex % 3) == MIDDLE) {
					//is Middle
					if (!(ubVerifyMode & D3_ECC_VERIFY_M)) {
						if (ubVerifyMode & D3_ECC_VERIFY_U) {
							uwStartPageIndex = uwStartPageIndex + 1;//調成Upper Index
						}
						else {
							uwStartPageIndex = uwStartPageIndex + 2;//調成Lower Index
						}
					}
				}
				else if ((uwStartPageIndex % 3) == UPPER) {
					//is Upper
					if (!(ubVerifyMode & D3_ECC_VERIFY_U)) {
						if (ubVerifyMode & D3_ECC_VERIFY_L) {
							uwStartPageIndex = uwStartPageIndex + 1;//調成Lower Index
						}
						else {
							uwStartPageIndex = uwStartPageIndex + 2;//調成Middle Index
						}
					}
				}
				if (uwStartPageIndex > uwEndPageIndex) {
					return ;
				}
			}
#else
			if (uwStartPageIndex > uwEndPageIndex) {
				return ;
			}
#endif

			//管理D1 D3Read
			FQI_CB_Info->uwStartProgramOrderIndex = uwStartPageIndex;
#if B0KB
			// Last lower/middle/upper page index: 1535 / 1503 / 1534
			if ( ubVerifyMode & D3_ECC_VERIFY_L ) {
				uwEndPageIndex = 1535;
			}
			else if ( ubVerifyMode & D3_ECC_VERIFY_U ) {
				uwEndPageIndex = 1534;
			}
			else {
				uwEndPageIndex = 1503;
			}
#endif
			FQI_CB_Info->uwEndProgramOrderIndex = uwEndPageIndex;
			FQI_CB_Info->uwProgramOrderIndex = uwStartPageIndex;

			//管理FQ回收機制
			FQI_CB_Info->ubQueueIndex 	= 0;
			FQI_CB_Info->ubQueueFirst		= 0;
			FQI_CB_Info->ubQueuePushCnt	= 0;
		}

		ubCheckDoneCnt = 0;
		ubCEIndex = 0;
		FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];

		if (ubVerifyMode & D3_ECC_VERIFY) {
			ubD3 = 1;
#if ECC_VERIFY_START_END_UART
			UartString("\r\n D3VerifyECC START :");
#endif
		}
		else if (ubVerifyMode & D1_ECC_VERIFY) {
			ubD3 = 0;
#if ECC_VERIFY_START_END_UART
			UartString("\r\n D1VerifyECC START :");
#endif
		}

		if ((ubCopyBackMode & COPYBACK_NON_DMA) == 0) { //Data in out
			ubPartialWork = gubExpandCE_Ratio;
		}

		UBYTE ubPartialDoingCENum = ubCEIndexMAXNUM / ubPartialWork;

#if B0KB
		if ( (ubVerifyMode & D3_ECC_VERIFY) && ((ubVerifyMode & D3_ECC_VERIFY_CHECK) == 0) ) {	// Do not check any page
			ubPartialWork = 0;
		}
#endif

		for (ubPartialIndex = 0; ubPartialIndex < ubPartialWork ; ubPartialIndex++) {   //超過最大架構For 16CE 2Plane, Data in Out要分次做 ; 一個Queue只做一個CE的量 否則有可能卡住
			//Check Total Done
			while (ubCheckDoneCnt < ubPartialDoingCENum) {
				//CE
				for (ubCEIndex = ubPartialIndex * ubPartialDoingCENum; ubCEIndex < (ubPartialIndex + 1)*ubPartialDoingCENum; ubCEIndex++) {

					FQI_CB_Info = &gFQI.gCB_Info_t[ubCEIndex];

					ubBufferCEIndex = ubCEIndex - ubPartialIndex * ubPartialDoingCENum;

					//檢查FQ回收
					for (ubi = FQI_CB_Info->ubQueueFirst; FQI_CB_Info->ubQueuePushCnt/*有push才檢查*/; ((ubi + 1) >= sizeof(FQI_CB_Info->ubQueue)) ? ubi = 0 : ubi++) {
						if (gFQI.gFQLink[FQI_CB_Info->ubQueue[ubi]].ubFQPhase != BYTE_FQ_WAITCHECK) {
							break;
						}

						gFQI.gFQLink[FQI_CB_Info->ubQueue[ubi]].ubFQPhase = BYTE_FQ_CHECKDONE;
						M_AssertError(FQI_CB_Info->ubQueuePushCnt == 0);
						FQI_CB_Info->ubQueuePushCnt--;

						if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
							if (FQI_CB_Info->ubQueuePushCnt == 0) {
								ubCheckDoneCnt ++;
							}
						}
					}
					FQI_CB_Info->ubQueueFirst = ubi;

					//
					M_SwitchTask();

					if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
						continue;
					}

					if ((ubCELimit - gFQI.ubFQDoingNumber[ubCEIndex % gubMTQTotalPhyCE]) < ubMaxPlane) {
						continue;
					}

					//塞D3 or D1 Read FQ
					while ((ubCELimit - FQI_CB_Info->ubQueuePushCnt) >= ubMaxPlane) {

						//LMU-D3 Page Index
						uw_D3TargetPageIndex = FQI_CB_Info->uwProgramOrderIndex;

#if B0KB

						// terminal condition
						if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
							break;
						}

						// BYPASS page do not need check
						ubBypassCheck = 0;

						if (ubVerifyMode & D3_ECC_VERIFY_CHECK) {
							if ( uw_D3TargetPageIndex < 16 ) {
								if ( !(ubVerifyMode & D3_ECC_VERIFY_L) ) {
									ubBypassCheck = 1;
								}
							}
							else if ( uw_D3TargetPageIndex < 112 ) {
								if ( uw_D3TargetPageIndex & 1 ) {	// MIDDLE PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_M) ) {
										ubBypassCheck = 1;
									}
								}
								else {	// LOWER PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_L) ) {
										ubBypassCheck = 1;
									}
								}
							}
							else if ( uw_D3TargetPageIndex < 208 ) {
								if ( ((uw_D3TargetPageIndex - 112) % 3) == 0 ) {	// UPPER PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_U) ) {
										ubBypassCheck = 1;
									}
								}
								else if ( ((uw_D3TargetPageIndex - 112) % 3) == 1 )	 {	// LOWER PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_L) ) {
										ubBypassCheck = 1;
									}
								}
								else {	// MIDDLE PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_M) ) {
										ubBypassCheck = 1;
									}
								}
							}
							else if ( uw_D3TargetPageIndex < 1505 ) {
								if ( ((uw_D3TargetPageIndex - 208) % 3) == 0 ) {	// UPPER PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_U) ) {
										ubBypassCheck = 1;
									}
								}
								else if ( ((uw_D3TargetPageIndex - 208) % 3) == 1 )	 {	// LOWER PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_L) ) {
										ubBypassCheck = 1;
									}
								}
								else {	// MIDDLE PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_M) ) {
										ubBypassCheck = 1;
									}
								}
							}
							else if ( uw_D3TargetPageIndex < 1536 ) {
								if ( ((uw_D3TargetPageIndex - 1504) % 2) == 0 ) {	// UPPER PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_U) ) {
										ubBypassCheck = 1;
									}
								}
								else if ( ((uw_D3TargetPageIndex - 1504) % 2) == 1 ) {	// LOWER PAGE
									if ( !(ubVerifyMode & D3_ECC_VERIFY_L) ) {
										ubBypassCheck = 1;
									}
								}
							}
							// NAND team suggested
							if ( uw_D3TargetPageIndex == 725 || uw_D3TargetPageIndex == 726 ||
							        uw_D3TargetPageIndex == 728 || uw_D3TargetPageIndex == 729 ||
							        uw_D3TargetPageIndex == 731 || uw_D3TargetPageIndex == 732 ||
							        uw_D3TargetPageIndex == 734 || uw_D3TargetPageIndex == 735 ||
							        uw_D3TargetPageIndex == 820 || uw_D3TargetPageIndex == 823 ||
							        uw_D3TargetPageIndex == 826 || uw_D3TargetPageIndex == 829) {
								ubBypassCheck = 0;
							}

							// Bypass Check
							if ( ubBypassCheck ) {
								FQI_CB_Info->uwProgramOrderIndex++;
								continue;
							}

							// Update Counter
							if ( ubCEIndex == 0 ) {
								uwVerifyPageCnt++;
							}

						}

#endif

						for (ubPlaneIndex = 0 ; ubPlaneIndex < ubMaxPlane ; ubPlaneIndex++) {

							FQI_CB_Info->ubQueue[FQI_CB_Info->ubQueueIndex] = gFQI.ubFQLinkFree;
							FQI_CB_Info->ubQueuePushCnt++;
							FQI_CB_Info->ubQueueIndex++;
							if (FQI_CB_Info->ubQueueIndex >= sizeof(FQI_CB_Info->ubQueue) ) {
								FQI_CB_Info->ubQueueIndex = 0x00;
							}

							//ubFQPlaneIndex = gFQI.ubFQLinkFree;//16K * 128 FQ = 2MB
							ubFQPlaneIndex = ((ubBufferCEIndex * ubMaxPlane) + ubPlaneIndex);

							M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
							FQ->btCheckEmpty = 0;
							FQ->ulRAMAddr = (U32)&gubCopyBuffer[ubFQPlaneIndex * (gubSectorsPerPlane << 9)];
							FQ->uwFUnit = uwFUnit;
							FQ->ulFEntry = ((M_Multiplier((U32)uw_D3TargetPageIndex, DEF_4KEntryPerPage_Mcl_Div)) + (((ubCEIndex << gubBurstsPerBankLog) + ubPlaneIndex) << gub4kEntrysPerPlaneLog)) ;
							FQ->btPreRead = 0;
							FQ->ubFJob = BYTE_FJOB_READ;
							FQ->uwFJobInfo = ( (ubD3 ? 0 : BIT_FJOBI_FASTPAGE) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | (ENABLE_RUT_SWAP ? BIT_FJOBI_FAILHANDLE_RIGHTNOW : 0));
							FQ->ubDepth = ubDepth;
							FQ->ubL4kIndex = ubL4kLink;
							FQ->btUNServed = 1;
							FQ->btDirectHandling = (gubDirectHandl_RUTLog == 1) ? 1 : 0;
							FQ->btD3 = ubD3;
							FQ->btNoWait = 0;
							FQ->ubD3WriteMode = 0;

							if (uwFUnit >= 4096) {
								FQ->uwFJobInfo |= BIT_FJOBI_RUT_D1LINK;
								FQ->ubFSAIndex = gubTempRUTD1LinkValue;
							}

							gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
							gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
							gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
							gulPara0x24.BitMap.btZipBypass = 1;
							gulPara0x24.BitMap.btCmpEn = 0;
							gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
							gulPara0x24.BitMap.btBufModeEn = 0;
							gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
							gMTQ->Depth[ubDepth].ulZipMap = 0xFFFFFFFF;

							for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
								pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
								//set L4K table info
								pL4KTable.ulL4K_LCA = 0xFFFFFFFF;
								pL4KTable.Para0x04.ulAll = 0;
								pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0;
								pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
								pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0x00;//DATA不進 SDR RAM
								pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
								pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
								*pL4KTablePtr = pL4KTable;
							}

							gFQI.ubFQLinkNumber++;
							gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

						}

						FQI_CB_Info->uwProgramOrderIndex++;
#if (!B0KB)
						if (ubVerifyMode & D3_ECC_VERIFY_CHECK) {
							if ((FQI_CB_Info->uwProgramOrderIndex % 3) == LOWER) {
								//is LOW
								if (!(ubVerifyMode & D3_ECC_VERIFY_L)) {
									if (ubVerifyMode & D3_ECC_VERIFY_M) {
										FQI_CB_Info->uwProgramOrderIndex = FQI_CB_Info->uwProgramOrderIndex + 1;//調成Middle Index
									}
									else {
										FQI_CB_Info->uwProgramOrderIndex = FQI_CB_Info->uwProgramOrderIndex + 2;//調成Upper Index
									}
								}
							}
							else if ((FQI_CB_Info->uwProgramOrderIndex % 3) == MIDDLE) {
								//is Middle
								if (!(ubVerifyMode & D3_ECC_VERIFY_M)) {
									if (ubVerifyMode & D3_ECC_VERIFY_U) {
										FQI_CB_Info->uwProgramOrderIndex = FQI_CB_Info->uwProgramOrderIndex + 1;//調成Upper Index
									}
									else {
										FQI_CB_Info->uwProgramOrderIndex = FQI_CB_Info->uwProgramOrderIndex + 2;//調成Lower Index
									}
								}
							}
							else if ((FQI_CB_Info->uwProgramOrderIndex % 3) == UPPER) {
								//is Upper
								if (!(ubVerifyMode & D3_ECC_VERIFY_U)) {
									if (ubVerifyMode & D3_ECC_VERIFY_L) {
										FQI_CB_Info->uwProgramOrderIndex = FQI_CB_Info->uwProgramOrderIndex + 1;//調成Lower Index
									}
									else {
										FQI_CB_Info->uwProgramOrderIndex = FQI_CB_Info->uwProgramOrderIndex + 2;//調成Middle Index
									}
								}
							}
						}
#endif
						if (FQI_CB_Info->uwProgramOrderIndex > FQI_CB_Info->uwEndProgramOrderIndex) {
							break;
						}

					}

					M_SwitchTask();

				}
			}
		}

		//memcpy(&gFQI.gCB_Info_t[ubCEIndex], (void*)&FQI_CB_Info_tmp, sizeof(FQI_CB_Info_tmp));

		/*
		     * Enable CRC check for normal FW flow at the end
		     */
		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}

		FCONL[FCONL_CRC_EN] |= (CRC_32_EN);
#if ECC_VERIFY_START_END_UART
		UartString("\r\n VerifyECC END :");
#endif
		M_SwitchTask();
	}

	VT->gulFTLState.B.btDoingFlushD1 = 0;
	return;
}
#endif

/**
 * @name    Vender_UnlockCmd
 * @brief   See if Host send the specific unlock vender command. If host send
 *          the unlock command 3 times, then unlock the device
 * @return  gubRealLock
 *
 * When gubRealLock is 0, the device is unlocked.
 */
void Vender_UnlockCmd()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	RWBuffer(1);
	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
	}
	if (UnlockCmdPass1Time()) {
		gubCheckLockCnt++;
	}
	else {
		/*
		 * The host does not issue the correct vender command to
		 * unlock the device, just lock the device by hanging up.
		 */
		flaLEDBlink(0);
	}

	if (gubCheckLockCnt >= 3) {
		gubRealLock = 0;
	}
}

/**
 * @name    CheckVenderLock
 * @brief   The Lock Function of the Vender Lock feature
 */
void CheckVenderLock()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (!gubVenderLock) {
		return;
	}

	if (((gubCmdCode == 0xEF) && (HB[HB_FEATURE] == 0x02))
	        || ((gubCmdCode == 0xEF) && (HB[HB_FEATURE] == 0x10) && (HB[HB_SECTOR_CNT] == 0x06))
	        || ((gubCmdCode == 0xEF) && (HB[HB_FEATURE] == 0x66))) {
		/*
		 * When the Vender Platform wakeup from S3, the Host may send these
		 * command. Also need to unlock the device.
		 */
		gubRealLock = 0;
	}
	else if (gubCmdCode != 0x31 && gubRealLock) {
		/*
		 * This branch means the host does not issue the vender command to
		 * unlock the device, just lock the device by hanging up.
		 */

		flaLEDBlink(0);
	}
}

/**
 * @brief   To see if the data of vender command meet the specific unlock data.
 * @retval  1  the vender command meet requirement
 * @retval  0  the vender command does not meet requirement
 */
UBYTE UnlockCmdPass1Time()
{
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);

	if (((UBYTE *)BUFFER3_BASE)[0] == 0x6 && ((UBYTE *)BUFFER3_BASE)[1] == 0x5
	        && ((UBYTE *)BUFFER3_BASE)[2] == 0x4 && ((UBYTE *)BUFFER3_BASE)[3] == 0x3
	        && ((UBYTE *)BUFFER3_BASE)[4] == 0x2 && ((UBYTE *)BUFFER3_BASE)[5] == 0x1) {
		return 1;
	}

	return 0;
}

#if EnableProductHistory

void Vender_ReadPH()
{
	U32 ulTemp;
	//分辨出 current & swap & 備份
	if ((gubPHinit & Bit4_InitDone) != BIT4) {
		//check producthistory
		//先不考慮在開rdt時寫生產履歷,因為RDT有一段沒DBT
		if ((gubPHinit & 0xF) == 0) {
			if (RDT_FindDBTBlock_Task()) {
				//no DBT
				if (FindPHBlockByScan(0)) {
					//scan fail
					gubPHinit |= Bit3_InitByScan_NotFind;
				}
				else {
					//scan pass
					gubPHinit |= Bit2_InitByScan_Find;
				}
			}
			else {
				if (FindPHBlockByDBT()) {
					//DBT沒找到PH mark ->fail
					if (!BURNER) {
						//FW 但沒PH
						//cmd abort
						gubErrorCode = SET_ABRT;
						return;
					}
					else {
						//1.RDT要掃PH unit並且mark DBT,有可能會有只有一個PH的狀態
						//2.若是空卡做完RDT 也會盡這裡 目前還沒有任何一個PH unit

					}
					gubPHinit |= Bit1_InitByDBT_NotFind;
					if (FindPHBlockByScan(0)) {
						//scan fail
						gubPHinit |= Bit3_InitByScan_NotFind;
					}
					else {
						//scan pass
						gubPHinit |= Bit2_InitByScan_Find;
					}
				}
				else {
					//DBT找到PH mark->pass
					gubPHinit |= Bit0_InitByDBT_Find;
				}
			}
		}

		if (ScanPHBlock_Task(1)) {
			//cmd abort
			gubErrorCode = SET_ABRT;
			return;
		}
		else {
			gubPHinit |= Bit4_InitDone;
		}
	}


	//Direct_Read
	if (guwPHUnit[1].uwPagePTR == PH_Default) {
		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(BUFB_BASE, (gubSectorsPerPlane + 1) * 512);
		}
		mDMAC_SV(1,  (U32)BUFB_BASE, (gubSectorsPerPlane + 1) * 512, 0, 0xFFFFFFFF, 0, DMA_EN_SETCQ, 1);
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
		}


		ulTemp = HL[HL_BUF_R_EP];
		HL[HL_BUF_R_EP] = 1;
		HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

		HL[HL_BUF_R_EP] = ulTemp;

	}
	else {

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(BUFB_BASE, 512);
		}

		ftlReadSinglePlane(guwPHUnit[1].uwUnit, ((guwPHUnit[1].ubPlanebank + guwPHUnit[1].uwPagePTR * gubPlanesPerPage) << gub4kEntrysPerPlaneLog), (guwPHUnit[1].ubPlanebank / gubBurstsPerBank), (U32)(BUFB_BASE), MARK_PH, (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), (gubSectorsPerPlane / 8), 1, 0);
		mDMAC_COPY((U32)&BUFB_BASE[((gub4kEntrysPerPlane - 1) * 0x1000)], (U32)&BUFB_BASE[0], 0x1000, MARK_PH, 0, 0, 0, 1); // Last Frame
		ulTemp = HL[HL_BUF_R_EP];
		HL[HL_BUF_R_EP] = 1;
		HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

		HL[HL_BUF_R_EP] = ulTemp;
	}

}


void Vender_WritePH()
{

	UWORD uwtemp;

	//分辨出 current & swap & 備份
	if ((gubPHinit & Bit4_InitDone) != BIT4) {
		//check producthistory
		//先不考慮在開rdt時寫生產履歷,因為RDT有一段沒DBT
		if ((gubPHinit & 0xF) == 0) {
			if (RDT_FindDBTBlock_Task()) {
				//no DBT
				if (FindPHBlockByScan(0)) {
					//scan fail
					gubPHinit |= Bit3_InitByScan_NotFind;
				}
				else {
					//scan pass
					gubPHinit |= Bit2_InitByScan_Find;
				}
			}
			else {
				if (FindPHBlockByDBT()) {
					//DBT沒找到PH mark ->fail
					if (!BURNER) {
						//FW 但沒PH
						//cmd abort -> not support PH
						gubErrorCode = SET_ABRT;
						return;
					}
					else {
						//1.RDT要掃PH unit並且mark DBT,有可能會有只有一個PH的狀態
						//2.若是空卡做完RDT 也會盡這裡 目前還沒有任何一個PH unit
					}
					gubPHinit |= Bit1_InitByDBT_NotFind;
					if (FindPHBlockByScan(0)) {
						//scan fail
						gubPHinit |= Bit3_InitByScan_NotFind;
					}
					else {
						//scan pass
						gubPHinit |= Bit2_InitByScan_Find;
					}

				}
				else {
					//DBT找到PH mark->pass
					gubPHinit |= Bit0_InitByDBT_Find;
				}
			}
		}

		if (ScanPHBlock_Task(0)) {
			//cmd abort
			gubErrorCode = SET_ABRT;
			return;
		}
		else {
			gubPHinit |= Bit4_InitDone;
		}
	}

	if (((gubPHinit & Bit0_InitByDBT_Find) != BIT0)) {
		gubErrorCode = SET_ABRT;
		return;
	}
	ftl_Erase_Single_Plane(guwPHUnit[0].uwUnit, guwPHUnit[0].ubPlanebank % gubBurstsPerBank, M_Modular((guwPHUnit[0].ubPlanebank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask), BIT_FJOBI_FASTPAGE);


	if ((guwPHUnit[0].uwPagePTR == PH_Default) && (guwPHUnit[1].uwPagePTR == PH_Default)) {
		//沒有任何一筆PH
		mDMAC_SV(4,  (U32)BUFB_BASE + (gub4kEntrysPerPlane * 0x1000), gubSectorsPerPlane * 512, MARK_PH, 0x33CC33CC, 0, DMA_EN_SETCQ, 1);

	}
	else if (guwPHUnit[1].uwPagePTR == (guwFastPagePagesPerUnit - 1)) {
		//洗一輪了,只read
		ftlReadSinglePlane(guwPHUnit[1].uwUnit, ((guwPHUnit[1].ubPlanebank + guwPHUnit[1].uwPagePTR * gubPlanesPerPage) << gub4kEntrysPerPlaneLog), (guwPHUnit[1].ubPlanebank / gubBurstsPerBank), (U32)(BUFB_BASE + (gub4kEntrysPerPlane * 0x1000)), MARK_PH, (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), (gubSectorsPerPlane / 8), 1, 0);
	}
	else {
		//把前面的page copy過來
		for (uwtemp = 0; uwtemp <= guwPHUnit[1].uwPagePTR; uwtemp++) {
			//read
			ftlReadSinglePlane(guwPHUnit[1].uwUnit, ((guwPHUnit[1].ubPlanebank + uwtemp * gubPlanesPerPage) << gub4kEntrysPerPlaneLog), (guwPHUnit[1].ubPlanebank / gubBurstsPerBank), (U32)(BUFB_BASE + (gub4kEntrysPerPlane * 0x1000)), MARK_PH, (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), (gubSectorsPerPlane / 8), 1, 0);

			//Write
			ftlWriteSinglePlane(guwPHUnit[0].uwUnit, ((guwPHUnit[0].ubPlanebank + uwtemp * gubPlanesPerPage) << gub4kEntrysPerPlaneLog),  (guwPHUnit[0].ubPlanebank / gubBurstsPerBank), (U32)(BUFB_BASE + (gub4kEntrysPerPlane * 0x1000)), MARK_PH, 0x112233, (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), (gubSectorsPerPlane / 8), 1, 0);

		}

	}
	//讀前一個page,放到ram上,拿fram 2,3,4當這個要寫的fram1,2,3
	//前面有下 Read, 應該已經讀上ram了

	//if (ENABLE_DCACHE) {
	//            xthal_dcache_region_invalidate(BUFB_BASE, ubL4KNum * 4096);
	//}

	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	mDMAC_COPY((U32)&BUFB_BASE[0], (U32)&BUFB_BASE[(gub4kEntrysPerPlane - 1) * 0x1000], 0x1000, MARK_PH, 0, 0, 0, 1); // 擺在plane的最後一個位置
	mDMAC_COPY((U32)&BUFB_BASE[(gub4kEntrysPerPlane * 0x1000) + 0x1000], (U32)&BUFB_BASE[0], ((gub4kEntrysPerPlane - 1) * 0x1000), MARK_PH, 0, 0, 0, 1); //淘汰一個frame 並抓這個Plane剩下的 Frame 餵過來

	//Write
	if ((guwPHUnit[0].uwPagePTR == PH_Default) && (guwPHUnit[1].uwPagePTR == PH_Default)) { // avoid overflow
		ftlWriteSinglePlane(guwPHUnit[0].uwUnit, ((guwPHUnit[0].ubPlanebank + ((0) % guwFastPagePagesPerUnit)*gubPlanesPerPage) << gub4kEntrysPerPlaneLog),  (guwPHUnit[0].ubPlanebank / gubBurstsPerBank), (U32)BUFB_BASE, MARK_PH, 0x112233, (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), (gubSectorsPerPlane / 8), 1, 0);
	}
	else	{
		ftlWriteSinglePlane(guwPHUnit[0].uwUnit, ((guwPHUnit[0].ubPlanebank + ((guwPHUnit[1].uwPagePTR + 1) % guwFastPagePagesPerUnit)*gubPlanesPerPage) << gub4kEntrysPerPlaneLog),  (guwPHUnit[0].ubPlanebank / gubBurstsPerBank), (U32)BUFB_BASE, MARK_PH, 0x112233, (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), (gubSectorsPerPlane / 8), 1, 0);
	}



	//program 完 ptr +1, 然後交換

	if ((guwPHUnit[0].uwPagePTR == PH_Default) && (guwPHUnit[1].uwPagePTR == PH_Default)) {
		guwPHUnit[0].uwPagePTR = 0;
	}
	else {
		guwPHUnit[0].uwPagePTR = (guwPHUnit[1].uwPagePTR + 1) % guwFastPagePagesPerUnit;
	}
	PHexchange();


	//交換完後再補dummy補滿一個blk

	mDMAC_SV(4,  (U32)BUFB_BASE + (gub4kEntrysPerPlane * 0x1000), gubSectorsPerPlane * 512, MARK_PH_Dummy, 0x33CC33CC, 0, DMA_EN_SETCQ, 1);

	for (uwtemp = (guwPHUnit[1].uwPagePTR + 1); uwtemp < guwFastPagePagesPerUnit; uwtemp++) {
		ftlWriteSinglePlane(guwPHUnit[1].uwUnit, ((guwPHUnit[1].ubPlanebank + uwtemp * gubPlanesPerPage) << gub4kEntrysPerPlaneLog),  (guwPHUnit[1].ubPlanebank / gubBurstsPerBank), (U32)(BUFB_BASE + (gub4kEntrysPerPlane * 0x1000)), MARK_PH_Dummy, 0x112233, (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_FASTPAGE), (gubSectorsPerPlane / 8), 1, 0);
	}

}

#endif

UBYTE I2C_WriteRead_LedIC(UBYTE ubMode, UBYTE ubSubAddr, UBYTE *ubValue)
{
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);

	UBYTE ubAddrSelect = 0x50;
	U32   ulTimeOut = GetRTT_milis();

	SYS1L[SYS1L_I2C_BUF0_REG] = 0; //clear buffer 0

	SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] &= 0xFFFFFF00;
	SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] |= (U32)(ubAddrSelect);
	SYS1L[SYS1L_I2C_GP1_OP_REG] = ubSubAddr | (*ubValue << 8);

	//0: read, 1: write
	if (ubMode) {
		SYS1L[SYS1L_I2C_GP1_CTRL] = ((2 << 16) | I2C_WRITE);  //OP_Cnt = 2

		SYS1L[SYS1L_I2C_GP1_CTRL] |= I2C_START; //Trigger

		while (((SYS1L[SYS1L_I2C_GP1_CTRL] & I2C_START) == 1) && ((GetRTT_milis() - ulTimeOut) < 5));
		if ((SYS1L[SYS1L_I2C_GP1_NACK] & GP1_ANACK) == 1) {
			return 0x01;   // not exist
		}
		else if ((SYS1L[SYS1L_I2C_GP1_CTRL] & I2C_START) == 1) {
			return 0x02;   // Salve Address not support
		}
	}
	else {
		//write slave address
		SYS1L[SYS1L_I2C_GP1_CTRL] = ((1 << 16) | I2C_WRITE);  //OP_Cnt = 1

		SYS1L[SYS1L_I2C_GP1_CTRL] |= I2C_START; //Trigger

		while (((SYS1L[SYS1L_I2C_GP1_CTRL] & I2C_START) == 1) && ((GetRTT_milis() - ulTimeOut) < 5));
		if ((SYS1L[SYS1L_I2C_GP1_NACK] & GP1_ANACK) == 1) {
			return 0x01;   // not exist
		}
		else if ((SYS1L[SYS1L_I2C_GP1_CTRL] & I2C_START) == 1) {
			return 0x02;   // Salve Address not support
		}

		//read data
		SYS1L[SYS1L_I2C_GP1_CTRL] = ((0 << 16) | (1 << 8) | (0 << 4) | I2C_READ); // OP_CNT=1  DATA_CNT=1 BUFF=0 DIR=read

		SYS1L[SYS1L_I2C_GP1_CTRL] |= I2C_START; //Trigger

		while (((SYS1L[SYS1L_I2C_GP1_CTRL] & I2C_START) == 1) && ((GetRTT_milis() - ulTimeOut) < 5));
		if (((SYS1L[SYS1L_I2C_GP1_NACK] & GP1_ANACK) == 1) || ((SYS1L[SYS1L_I2C_GP1_NACK] & GP1_DNACK) != 0)) {
			return 0x01;   // not exist
		}
		else if ((SYS1L[SYS1L_I2C_GP1_CTRL] & I2C_START) == 1) {
			return 0x02;   // Salve Address not support
		}
		else {
			*ubValue = SYS1B[SYS1L_I2C_BUF0_REG << 2];
		}
	}

	return 0x00;
}

/**
 * @name    Vender_I2C_Write_LedIC
 * @brief   This function is designed for set special LED IC
**/
void Vender_I2C_Write_LedIC(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubRtn = 0x00;
	UBYTE ubSubAddr, ubValue;
	UWORD uwi = 0x00;
	U32   ulMuxBackup;
	U32	  ulClkDivCntBackup;

	ulMuxBackup = SYS0L[SYS0L_SYS_MUX_CTRL0];
	ulClkDivCntBackup = SYS1L[SYS1L_I2C_GP1_CLK_DIV_CNT];

	SYS1L[SYS1L_I2C_GP1_CLK_DIV_CNT] &= 0xFF00FFFF;
	SYS1L[SYS1L_I2C_GP1_CLK_DIV_CNT] |= 0x00C80000;

	SYS0L[SYS0L_SYS_MUX_CTRL0] &= ~(BIT27);
	SYS0L[SYS0L_SYS_MUX_CTRL0] |= BIT26;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
	}

	//Get Op code from buffer
	for (uwi = 0; uwi < 256; uwi += 2) {

		ubSubAddr = BUFB_BASE[uwi];
		ubValue = BUFB_BASE[uwi + 1];

		if (ubSubAddr != 0xFF) {
			ubRtn = I2C_WriteRead_LedIC(0x01, ubSubAddr, &ubValue);
			if (ubRtn != 0) {
				gubErrorCode = SET_ABRT;
				break;
			}
		}
	}

	HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
	while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.

	SYS0L[SYS0L_SYS_MUX_CTRL0] = ulMuxBackup;

	SYS1L[SYS1L_I2C_GP1_CLK_DIV_CNT] = ulClkDivCntBackup;
}

/**
 * @name    Vender_I2C_Read_LedIC
 * @brief   This function is designed for get special LED IC
**/
void Vender_I2C_Read_LedIC(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubRtn = 0x00;
	UBYTE ubSubAddr, ubValue;
	UWORD uwi = 0x00;
	U32   ulMuxBackup;
	U32	  ulClkDivCntBackup;

	ulMuxBackup = SYS0L[SYS0L_SYS_MUX_CTRL0];
	ulClkDivCntBackup = SYS1L[SYS1L_I2C_GP1_CLK_DIV_CNT];

	SYS1L[SYS1L_I2C_GP1_CLK_DIV_CNT] &= 0xFF00FFFF;
	SYS1L[SYS1L_I2C_GP1_CLK_DIV_CNT] |= 0x00C80000;

	SYS0L[SYS0L_SYS_MUX_CTRL0] &= ~(BIT27);
	SYS0L[SYS0L_SYS_MUX_CTRL0] |= BIT26;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(BUFB_BASE, 512);
	}

	//Get Op code from buffer
	for (uwi = 0; uwi < 256; uwi += 2) {

		ubSubAddr = BUFB_BASE[uwi];
		ubValue = 0x00;

		if (ubSubAddr != 0xFF) {
			ubRtn = I2C_WriteRead_LedIC(0x00, ubSubAddr, &ubValue);
			if (ubRtn != 0) {
				gubErrorCode = SET_ABRT;
				break;
			}
			else {
				BUFB_BASE[uwi + 1] = ubValue;
			}

			//U32 tmp = GetRTT_micros();
			//while ((GetRTT_micros() - tmp) < 200);
		}
	}

	if (gubErrorCode == NO_ERROR) {
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv(BUFB_BASE, 512);
		}

		HB[HB_DATA_TRG] = H_DATA_TRG;      // Bit0: Data Transfer Trigger
		while (HB[HB_DATA_TRG]&H_DATA_TRG); // Wait until Trig data to Host Finish.
	}

	SYS0L[SYS0L_SYS_MUX_CTRL0] = ulMuxBackup;

	SYS1L[SYS1L_I2C_GP1_CLK_DIV_CNT] = ulClkDivCntBackup;
}
void Vender_HostRemainSectorCount(void)
{
	while (HL[HL_CMD_REMAIN_SEC ]) {
		if (gubHardReset) {
			return;
		}
	}
}

void Vender_TriggerHostData(void)
{

	HL[HL_BUF_OFST] = 0;
	HW[HW_SECTOR_CNT] = gulSectorCnt;
	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	HL[HL_TQ_CONTENT] = 0;
	Vender_HostRemainSectorCount();
}

void Vender_TriggerHostData_31(void)
{

	HL[HL_BUF_OFST] = 0;
	HW[HW_SECTOR_CNT] = gulSectorCnt;
	HL[HL_BUF_TRG_SEC] = gulSectorCnt;
	HL[HL_TQ_CONTENT_31] = 0;
	Vender_HostRemainSectorCount();
}
