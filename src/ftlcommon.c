
#include "setup.h"
#include "Vardef.h"
#include "FTL.h"

#include <string.h>
#include <stdlib.h>

#if FixShallowErase
UWORD ftlScanSystemUnit_Binary(UWORD uwUnit, UBYTE ubBurst, UBYTE ubCEindex)
{
	//Scan System unit
	UBYTE ubReadStatus;
	UWORD uwHigh, uwLow, uwMid;
	uwHigh = guwFastPagePagesPerUnit - 1;
	uwLow = 0;
	ubReadStatus = 0;
	while (1) {
		uwMid = (uwHigh + uwLow) / 2;
		if (uwLow >= uwHigh) {
			break;
		}
		ubReadStatus = 0;
		if (uwMid > guwFastPagePagesPerUnit) {
			M_AssertError(1);
		}
		ubReadStatus = ftlReadSinglePlane(uwUnit, uwMid, ubCEindex, BUFFER2_START, MARK_SYS, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_D1_UNIT), 4, 0, ubBurst);
		if ( ubReadStatus == Read_Erase_Page) {
			uwHigh = uwMid;
		}
		else {
			uwLow = uwMid + 1;
		}
	}
	return uwLow;
}
#endif

UWORD ftlGetAnotherSystemBlockInSet(UBYTE ubMode, U32 ulDBTAddr, UBYTE ubChannel, UWORD uwBlock)
{
	UBYTE ubSetNumber;
	UWORD uwAnotherBlockInSet;

	if (ubMode == Mode_ByRemappingTable) {
		for (ubSetNumber = 0; ubSetNumber < SYSTEM_SETS; ubSetNumber++) {
			if (guwSystemBlockRemappingTable[ubSetNumber][ubChannel] == uwBlock) {
				uwAnotherBlockInSet = guwSystemBlockRemappingTable[ubSetNumber][!ubChannel];
				break;
			}
		}
		if (ubSetNumber == SYSTEM_SETS) {
			uwAnotherBlockInSet = 0xFFFF;
		}
	}
	else if (ubMode == Mode_ByDBTinFlash || ubMode == Mode_ByDBTinRAM) {//Mode_ByDBTinFlash:一次從flash拉一個page DBT起來看, Mode_ByDBTinRAM:整張DBT都在RAM上了
		U32 ulOffset, ulEntryInDBT;
		U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512;
		UBYTE *ubDBTBuffer = (UBYTE *)ulDBTAddr;

		if (gubSystemArea_RemappingRule == Rule_UnitPlaneA) { //old plane A rule, 檢查CE0,CE1兩個block是否都是good block就好
			U32 ulUnit = (U32)(uwBlock >> gubBurstsPerBankLog);
			U32 ulBurst = (U32)(uwBlock & gubBurstsPerBankMask);

			ulEntryInDBT = ((ulUnit * gubDieNumber) * gubPlanesPerPage) + ulBurst + (ubChannel << gubBurstsPerBankLog);
			if (ubMode == Mode_ByDBTinFlash) {
				if (ftlLoadDBT(ulEntryInDBT, (U32)&ubDBTBuffer[0])) {
					return 0xFFFF;
				}
				ulOffset = ulEntryInDBT % ulEntryPerDBTPlane;
			}
			else if (ubMode == Mode_ByDBTinRAM) {
				ulOffset = ulEntryInDBT;
			}
			//當gubPreformat = 1時要多判斷erasefailmap是否有記此block發生過erasefail
			if ((ubDBTBuffer[ulOffset] == 0x00) && ((gubPreformat == 0) || ((gubPreformat == 1) && ((gubEraseFailBitMap[ulOffset >> 3] & (BIT0 << (ulOffset & 7))) == 0)))) { //CE0
				if (gubPlanesPerBurst == MAX_CHANNEL) { // 2 channel
					ulOffset = ((ulUnit * gubDieNumber) * gubPlanesPerPage) + ulBurst + (!ubChannel << gubBurstsPerBankLog);
					if ((ubDBTBuffer[ulOffset] == 0x00) && ((gubPreformat == 0) || ((gubPreformat == 1) && ((gubEraseFailBitMap[ulOffset >> 3] & (BIT0 << (ulOffset & 7))) == 0)))) { //CE1
						uwAnotherBlockInSet = uwBlock;
					}
					else {
						uwAnotherBlockInSet = 0xFFFF;
					}
				}
				else {//因為只有1 channel, 另外一個一定是0xFFFF
					uwAnotherBlockInSet = 0xFFFF;
				}
			}
			else {
				uwAnotherBlockInSet = 0xFFFF;
			}
		}
		else if (gubSystemArea_RemappingRule == Rule_Set) {
			U32 ulTotalDBTSizeInByte = (U32)gulTotalPhyUnitNum * gubPlanesPerPage;
			UWORD uwBlockNumber;
			UBYTE ubGoodBlkCounter;
			UBYTE ubAnotherChannel = !ubChannel;

			ulEntryInDBT = ((U32)(ubChannel << gubBurstsPerBankLog));
			uwBlockNumber = 0;
			ubGoodBlkCounter = 0;

			while (ulEntryInDBT < ulTotalDBTSizeInByte) {

				if (ubMode == Mode_ByDBTinFlash) {
					ulOffset = ulEntryInDBT % ulEntryPerDBTPlane;
					if (ulOffset == (U32)(ubChannel << gubBurstsPerBankLog)) {
						if (ftlLoadDBT(ulEntryInDBT, (U32)&ubDBTBuffer[0])) {
							return 0xFFFF;
						}
					}
				}
				else if (ubMode == Mode_ByDBTinRAM) {
					ulOffset = ulEntryInDBT;
				}
				//當gubPreformat = 1時要多判斷erasefailmap是否有記此block發生過erasefail
				if ((ubDBTBuffer[ulOffset] == 0x00) && ((gubPreformat == 0) || ((gubPreformat == 1) && ((gubEraseFailBitMap[ulOffset >> 3] & (BIT0 << (ulOffset & 7))) == 0)))) {
					if (uwBlockNumber == uwBlock) {
						ubSetNumber = ubGoodBlkCounter;
						break;
					}
					else if (uwBlockNumber > uwBlock) { //超過了
						ubSetNumber = 0xFF;
						break;
					}
					else {
						ubGoodBlkCounter++;
					}
				}
				if ((uwBlockNumber & gubBurstsPerBankMask) == gubBurstsPerBank - 1) {
					ulEntryInDBT += ((U32)(gubPlanesPerPage - (UBYTE)(M_Modular(ulEntryInDBT, DEF_PlanesPerPage_Mod_Mask))) + (U32)(gubPlanesPerPage * (gubDieNumber - 1)) + (U32)(ubChannel << gubBurstsPerBankLog));
				}
				else {
					ulEntryInDBT++;
				}
				uwBlockNumber++;
			}
			if (ubSetNumber == 0xFF) {
				uwAnotherBlockInSet = 0xFFFF;
			}
			else { //找到另外一個ch的第ubSetNumber個goodblk
				ulEntryInDBT = ((U32)(ubAnotherChannel << gubBurstsPerBankLog));
				uwBlockNumber = 0;
				ubGoodBlkCounter = 0;
				while (ulEntryInDBT < ulTotalDBTSizeInByte) {
					if (ubMode == Mode_ByDBTinFlash) {
						ulOffset = ulEntryInDBT % ulEntryPerDBTPlane;
						if (ulOffset == (U32)(ubAnotherChannel << gubBurstsPerBankLog)) {
							if (ftlLoadDBT(ulEntryInDBT, (U32)&ubDBTBuffer[0])) {
								return 0xFFFF;
							}
						}
					}
					else if (ubMode == Mode_ByDBTinRAM) {
						ulOffset = ulEntryInDBT;
					}
					//當gubPreformat = 1時要多判斷erasefailmap是否有記此block發生過erasefail
					if ((ubDBTBuffer[ulOffset] == 0x00) && ((gubPreformat == 0) || ((gubPreformat == 1) && ((gubEraseFailBitMap[ulOffset >> 3] & (BIT0 << (ulOffset & 7))) == 0)))) {
						if (ubGoodBlkCounter == ubSetNumber) {
							uwAnotherBlockInSet = uwBlockNumber;
							break;
						}
						ubGoodBlkCounter++;
					}
					if ((uwBlockNumber & gubBurstsPerBankMask) == gubBurstsPerBank - 1) {
						ulEntryInDBT += ((U32)(gubPlanesPerPage - (UBYTE)(M_Modular(ulEntryInDBT, DEF_PlanesPerPage_Mod_Mask))) + (U32)(gubPlanesPerPage * (gubDieNumber - 1)) + (U32)(ubAnotherChannel << gubBurstsPerBankLog));
					}
					else {
						ulEntryInDBT++;
					}
					uwBlockNumber++;
				}
			}
		}
	}

	return uwAnotherBlockInSet;
}

UBYTE ftlGetSystemSetNumber(UBYTE ubMode, U32 ulDBTAddr, UBYTE ubChannel, UWORD uwBlock)
{
	UBYTE ubSetNumber = 0;
	if (ubMode == Mode_ByRemappingTable) {
		for (ubSetNumber = 0; ubSetNumber < SYSTEM_SETS; ubSetNumber++) {
			if (guwSystemBlockRemappingTable[ubSetNumber][ubChannel] == uwBlock) {
				break;
			}
		}
		if (ubSetNumber == SYSTEM_SETS) {
			ubSetNumber = 0xFF;
		}
	}
	else if (ubMode == Mode_ByDBTinFlash || ubMode == Mode_ByDBTinRAM) {//Mode_ByDBTinFlash:一次從flash拉一個page DBT起來看, Mode_ByDBTinRAM:整張DBT都在RAM上了
		UBYTE *ubDBTBuffer = (UBYTE *)ulDBTAddr;
		U32 ulTotalDBTSizeInByte = (U32)gulTotalPhyUnitNum * gubPlanesPerPage;
		U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512;
		U32 ulOffset, ulEntryInDBT;
		UWORD uwBlockNumber = 0;
		UBYTE ubGoodBlkCounter = 0;
		UBYTE ubCEIndex;
		UBYTE ubGoodUnit;

		if (gubSystemArea_RemappingRule == Rule_UnitPlaneA) { //old planeA rule兩個ch挑的block一樣
			ulEntryInDBT = 0;//從block 0開始檢查
			while (ulEntryInDBT < ulTotalDBTSizeInByte) {
				ubGoodUnit = TRUE;
				if (ubMode == Mode_ByDBTinFlash) {
					ulOffset = ulEntryInDBT % ulEntryPerDBTPlane;
					if (ulOffset == 0) {
						if (ftlLoadDBT(ulEntryInDBT, (U32)&ubDBTBuffer[0])) {
							return 0xFF;
						}
					}
				}
				else if (ubMode == Mode_ByDBTinRAM) {
					ulOffset = ulEntryInDBT;
				}

				//當gubPreformat = 1時要多判斷erasefailmap是否有記此block發生過erasefail
				for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++ ) {
					U32 ulj = ulOffset + (gubBurstsPerBank * ubCEIndex);
					if ((ubDBTBuffer[ulj] != 0x00) || ((gubPreformat == 1) && ((gubEraseFailBitMap[ulj >> 3] & (BIT0 << (ulj & 7))) != 0))) {
						ubGoodUnit = FALSE;
					}
				}
				if (ubGoodUnit == TRUE) {
					if (uwBlockNumber == uwBlock) {
						ubSetNumber = ubGoodBlkCounter;
						break;
					}
					else if (uwBlockNumber > uwBlock) { //block編號超過了卻還沒找到
						ubSetNumber = 0xFF;
						break;
					}
					else {
						ubGoodBlkCounter++;
					}
				}

				ulEntryInDBT += (U32)(gubPlanesPerPage * gubDieNumber);//下一個CE0 PlaneA Block
				uwBlockNumber += gubBurstsPerBank;
			}
		}
		else if (gubSystemArea_RemappingRule == Rule_Set) { //計算這個block在此channel中是第幾個goodblk,setNumber等於goodblk編號
			ulEntryInDBT = ((U32)(ubChannel << gubBurstsPerBankLog));
			while (ulEntryInDBT < ulTotalDBTSizeInByte) {
				if (ubMode == Mode_ByDBTinFlash) {
					ulOffset = ulEntryInDBT % ulEntryPerDBTPlane;
					if (ulOffset == (U32)(ubChannel << gubBurstsPerBankLog)) {
						if (ftlLoadDBT(ulEntryInDBT, (U32)&ubDBTBuffer[0])) {
							return 0xFF;
						}
					}
				}
				else if (ubMode == Mode_ByDBTinRAM) {
					ulOffset = ulEntryInDBT;
				}

				if ((ubDBTBuffer[ulOffset] == 0x00) && ((gubPreformat == 0) || ((gubPreformat == 1) && ((gubEraseFailBitMap[ulOffset >> 3] & (BIT0 << (ulOffset & 7))) == 0)))) {
					if (uwBlockNumber == uwBlock) { //找到了
						ubSetNumber = ubGoodBlkCounter;
						break;
					}
					else if (uwBlockNumber > uwBlock) { //block編號超過了卻還沒找到
						ubSetNumber = 0xFF;
						break;
					}
					else {
						ubGoodBlkCounter++;
					}
				}
				if ((uwBlockNumber & gubBurstsPerBankMask) == gubBurstsPerBank - 1)
					ulEntryInDBT += ((U32)(gubPlanesPerPage - (UBYTE)(M_Modular(ulEntryInDBT, DEF_PlanesPerPage_Mod_Mask))) + (U32)(gubPlanesPerPage * (gubDieNumber - 1)) + (U32)(ubChannel << gubBurstsPerBankLog));
				else
					ulEntryInDBT++;

				uwBlockNumber++;
			}
		}
	}
	return ubSetNumber;
}

UBYTE ftlGetSystemSetFreeSet(UBYTE ubMode)
{
#if (!RDT)
	UBYTE ubGetFreeSet;
Mark_GetSystemBlockFreeSet:

	if (gubSystemSetFreeCount == 0) {
		flaLEDBlink(1);
	}

	gubSystemSetFreeCount--;

	if (ENABLE_SYSTEM_FREE_BLOCK_SORTING && (!gubDontUseSortingSystemSet)) {

		if (ubMode & GET_SYSTEM_SET_FROM_TAIL) { //從尾拿
			ubGetFreeSet = gubSystemSetFreeTable[gubSystemSetFreeCount];
		}
		else { //從頭拿
			ubGetFreeSet = gubSystemSetFreeTable[0];

			UBYTE ubi;
			for (ubi = 0; ubi < gubSystemSetFreeCount; ubi++) {
				gubSystemSetFreeTable[ubi] = gubSystemSetFreeTable[ubi + 1];//往前補
			}
		}
		gubSystemSetFreeTable[gubSystemSetFreeCount] = 0xFF;
	}
	else {
		ubGetFreeSet = gubSystemSetFreeTable[gubSystemSetFreeQueueOutPTR];
		gubSystemSetFreeTable[gubSystemSetFreeQueueOutPTR] = 0xFF;
		gubSystemSetFreeQueueOutPTR++;
		if (gubSystemSetFreeQueueOutPTR >= SYSTEM_SETS) {
			gubSystemSetFreeQueueOutPTR = 0;
		}
	}

#if FixShallowErase //Erase Plane-A for CE0 & CE1
	if ((ubMode & GET_SYSTEM_SET_SKIP_SHALLOW_ERASE_CHECK) == 0) {
		UBYTE ubCEindex, ubEraseFail;
		UWORD uwPageIndex = 0;
		UWORD uwBlock;
		UBYTE ubGetFreeUnit, ubGetFreeBurst;
		for (ubCEindex = 0; ubCEindex < gubPlanesPerBurst; ubCEindex++) {
			uwBlock = guwSystemBlockRemappingTable[ubGetFreeSet][ubCEindex];
			ubGetFreeUnit = uwBlock >> gubBurstsPerBankLog;
			ubGetFreeBurst = uwBlock & gubBurstsPerBankMask;
			uwPageIndex = ftlScanSystemUnit_Binary(ubGetFreeUnit, ubGetFreeBurst, ubCEindex);
			if (uwPageIndex != 0) {
				ftlForceSaveDummy((ubCEindex << gubBurstsPerBankLog) + (gubPlanesPerPage * uwPageIndex) + ubGetFreeBurst, (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div)), ubGetFreeUnit, (FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE | FORCE_SAVE_DUMMY_DIRECT_ACCESS  | FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE), D3_AREA, VT_FORCE_DUMMY_0xD3);
				ubEraseFail = ftl_Erase_Single_Plane(ubGetFreeUnit, ubGetFreeBurst, ubCEindex, BIT_FJOBI_FASTPAGE);
				if (ubEraseFail) {
					goto Mark_GetSystemBlockFreeSet; //erase fail get another GetFreeUnit
				}
			}
		}
	}
#endif
	return ubGetFreeSet;
#endif
#if(RDT)
	return 0;
#endif
}

void ftlAddToSystemSetFreeTable(UBYTE ubSet)
{
#if (!RDT)
	if (gubSystemSetFreeCount >= SYSTEM_SETS) {
		gubSystemSetFreeCount = SYSTEM_SETS;
		return;
	}

	if (ENABLE_SYSTEM_FREE_BLOCK_SORTING && (!gubDontUseSortingSystemSet)) {
		UBYTE ubi, ubInsertIndex;
		ubInsertIndex = gubSystemSetFreeCount;
		for (ubi = 0; ubi < gubSystemSetFreeCount; ubi++) {
			if (ubSet < gubSystemSetFreeTable[ubi]) {//依照block順序找到對的位子
				ubInsertIndex = ubi;
				break;
			}
		}
		for (ubi = gubSystemSetFreeCount; ubi > ubInsertIndex; ubi--) {
			gubSystemSetFreeTable[ubi] = gubSystemSetFreeTable[ubi - 1];//往後丟,騰出空間
		}

		gubSystemSetFreeTable[ubInsertIndex] = ubSet;
	}
	else {//根據in,out ptr
		gubSystemSetFreeTable[gubSystemSetFreeQueueInPTR] = ubSet;
		gubSystemSetFreeQueueInPTR++;
		if (gubSystemSetFreeQueueInPTR >= SYSTEM_SETS) {
			gubSystemSetFreeQueueInPTR = 0;
		}
	}
	gubSystemSetFreeCount++;
#endif
}

void ftlAddToVTAreaFreeTable(FTarget_t *uwTarget)
{
#if(!RDT)
	VT->gubVTAreaFreeCount++;
	VT->guwVTAreaFreeTarget[VT->gubVTAreaFreeQueueInPTR].All = uwTarget->All;
	VT->gubVTAreaFreeQueueInPTR++;
	VT->gubVTAreaFreeQueueInPTR &= (VTAREA_UNITS - 1);
#endif
}

void ftlGetVTAreaFreeBlock(FTarget_t *uwTarget)
{
#if(!RDT)
	VT->gubVTAreaFreeCount--;
	uwTarget->All = VT->guwVTAreaFreeTarget[VT->gubVTAreaFreeQueueOutPTR].All;
	VT->gubVTAreaFreeQueueOutPTR++;
	VT->gubVTAreaFreeQueueOutPTR &= (VTAREA_UNITS - 1);
#endif
}

void ftlAddToUserFreeTable(FTarget_t *uwTarget, UBYTE ubTableSelect)
{
#if(!RDT)
	UWORD uwIndex;
#if(!BURNER)
	if (ENABLE_RUT_SWAP && guwRUTSummaryLogNum) {
		UWORD uwRUTSummaryLogIndex;

		for (uwRUTSummaryLogIndex = 0; uwRUTSummaryLogIndex < guwRUTSummaryLogNum; uwRUTSummaryLogIndex++) {
#if TLCRUT
			if ((ubTableSelect == D1) && (guwRUTSummaryLog[uwRUTSummaryLogIndex].B.btIsD1Unit != 0) && (guwRUTSummaryLog[uwRUTSummaryLogIndex].B.btHandleLogDone == 0)) {
				if (uwTarget->B.uwTarget == guwRUTSummaryLog[uwRUTSummaryLogIndex].B.ubUnitD1Link) {
					guwRUTSummaryLog[uwRUTSummaryLogIndex].B.btNeedCopyData = 0;	// data已經在GC時搬走, 之後只要swap RUT就好, 跳過CopyUnit
					if (ENABLE_DEBUG_UART) {
						UartString("\nSRUTthenAddF(D1): ");
						UartWordHex(uwTarget->B.uwTarget);
					}
					//gubUpdateRUTBeforeSaveVT = 1;	// SwapRUT mode 2 裡面ftlChangeRUTMappingAndEraseCount會拉gubUpdateRUTBeforeSaveVT
					if (VT->gulFTLState.B.btNeedRUTReplace) {
						ftlRemoveRUTSummaryLog(guwRUTSummaryLog[uwRUTSummaryLogIndex].B.uwUnit, TRUE, TRUE, uwTarget->B.uwTarget);
					}
					else {
						ftlSwapRUT(2, uwTarget->B.uwTarget, TRUE);
					}
					break;
				}
			}
#endif
			if ((ubTableSelect == D3) && (guwRUTSummaryLog[uwRUTSummaryLogIndex].B.btIsD1Unit == 0) && (guwRUTSummaryLog[uwRUTSummaryLogIndex].B.btHandleLogDone == 0)) {
				if (uwTarget->B.uwTarget == guwRUTSummaryLog[uwRUTSummaryLogIndex].B.uwUnit) {
					guwRUTSummaryLog[uwRUTSummaryLogIndex].B.btNeedCopyData = 0;	// data已經在GC時搬走, 之後只要swap RUT就好, 跳過CopyUnit
					if (ENABLE_DEBUG_UART) {
						UartString("\nSRUTthenAddF(D3): ");
						UartWordHex(uwTarget->B.uwTarget);
					}
					//gubUpdateRUTBeforeSaveVT = 1;	// SwapRUT mode 2 裡面ftlChangeRUTMappingAndEraseCount會拉gubUpdateRUTBeforeSaveVT
					ftlSwapRUT(2, uwTarget->B.uwTarget, FALSE);
					break;
				}
			}
		}
	}
#endif

	FTarget_t *uwFreeBlockTable;
	EraseCount_t *ulEC;
	UWORD uwFreeBlockCount;
	if (ubTableSelect == D3) {
		uwFreeBlockTable = guwFreeBlockTable_D3;
		ulEC = (EraseCount_t*)gulEC_D3;
		uwFreeBlockCount = VT->guwFreeBlockCount_D3;
		if (uwTarget->B.uwTarget < VTAREA_UNITS) {
			//VT->gulDebugInfo[0] for caller
			VT->gulDebugInfo[1] = uwTarget->All;
			M_AssertCriticalError(1, VT_ASSERT_ADD_FREEQ_UNIT0_0xA8);
		}
	}
	else {
		uwFreeBlockTable = guwFreeBlockTable_D1;
		ulEC = (EraseCount_t*)gulEC_D1;
		uwFreeBlockCount = VT->guwFreeBlockCount_D1;
	}
	if (ENABLE_DEBUG_RW) {
		M_AssertError(uwTarget->B.uwTarget >= gulTotalPhyUnitNum);
		if (VT->gulVTLastMark == 0x55AA) {
			for (uwIndex = 2; uwIndex < uwFreeBlockCount + 2; uwIndex++) {
				M_AssertError(uwFreeBlockTable[uwIndex].B.uwTarget == uwTarget->B.uwTarget);
			}
		}
	}
	U32 ulTargetEC = M_UserFreeTableEC(ulEC, uwTarget, 0); //Add a MixPlane Weights.
	uwIndex = uwFreeBlockCount + 2; //因為FreeBlockTable前面兩個位置0,1沒有填值
	U32 ulLastFreeEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex - 1);
	if ((uwIndex % 2) && (ulTargetEC < ulLastFreeEC)) {
		uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwIndex - 1].B.uwTarget;
		uwIndex--;
	}
	while (uwIndex >= 4) {
		UWORD parent = uwIndex / 4;
		UWORD children = 2 * parent;
		U32 ulLeftChildrenEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, children);
		U32 ulRightChildrenEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, children + 1);
		if (ulTargetEC < ulLeftChildrenEC) {
			uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[children].B.uwTarget;
			uwIndex = children;
		}
		else if (ulTargetEC > ulRightChildrenEC) {
			uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[children + 1].B.uwTarget;
			uwIndex = children + 1;
		}
		else
			break;
	}
	if (ubTableSelect == D3) {
		if (ENABLE_READ_CHECK_FUNCTION) {
			if (ENABLE_FORCE_READ_KAO_UART) {
				UartString("\nAdd:");
				UartWordHex(uwTarget->B.uwTarget);
			}
			ftlCleanReadCheckInfo(uwTarget->B.uwTarget);
			ftlRemoveForceReadFIFO(uwTarget->B.uwTarget);
		}
#if TLC
		gulVC[uwTarget->B.uwTarget].B.btInD1Unit = 0;
#endif
		gulEC_D3[uwTarget->B.uwTarget].B.btUsedDataUnit = 0;
		M_AssertError(gulVC[uwTarget->B.uwTarget].B.ulValidCount != DefaultVC);
		if (uwTarget->B.uwTarget == VT->guwMinimumEC_UsedUnit) {
			M_Get_Minimum_UsedUnit_EC();
		}
		if (uwTarget->B.uwTarget == VT->guwOpenGRTarget.B.uwTarget) {
#if ENABLE_DEBUG_UART_INIT_TIME_II
			UartString("FQ: ");
			UartWordHex(VT->guwOpenGRTarget.B.uwTarget);
			UartString("\n");
#endif
			VT->guwOpenGRTarget.B.uwTarget = Default_Unit;
			VT->guwInitInfoOpenGRP2LPtr = 0xFFFF;
		}
	}
	uwFreeBlockTable[uwIndex].B.uwTarget = uwTarget->B.uwTarget;
	uwFreeBlockTable[0].B.uwTarget = uwFreeBlockTable[3].B.uwTarget;
	uwFreeBlockTable[1].B.uwTarget = uwFreeBlockTable[2].B.uwTarget;
	uwFreeBlockCount++;
#if TLC
	if (ENABLE_DEBUG_RW && (0 == gubRebuildFreeQueueDoing)) {
#else
	if (ENABLE_DEBUG_RW) {
#endif
		UWORD uwi;

		for (uwi = 2; uwi < uwFreeBlockCount + 2; uwi = uwi + 2) {
			M_AssertError((M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi + 1)) && ((uwi + 1) < (uwFreeBlockCount + 2)));
		}

		for (uwi = 3; uwi < uwFreeBlockCount + 2; uwi++) {
			M_AssertError(M_UserFreeTableEC(ulEC, uwFreeBlockTable, 2) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi));
		}

		if (uwFreeBlockCount > 2) {
			for (uwi = 4; uwi < uwFreeBlockCount + 2; uwi++) {
				M_AssertError(M_UserFreeTableEC(ulEC, uwFreeBlockTable, 3) < M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi));
			}
		}
	}
	if (ubTableSelect == D3) {
		VT->guwFreeBlockCount_D3 = uwFreeBlockCount;
	}
	else {
#if TLC
		UBYTE ubDie;
		VT->guwFreeBlockCount_D1 = uwFreeBlockCount;
		M_GetDieNumber(ubDie, uwTarget->B.uwTarget, D1);
		VT->gubD1DieCount[ubDie]++;
#endif
	}
	return;
#endif
}
void ftlGetUserFreeBlock(FTarget_t *uwTarget, UBYTE ubPopAllFreeBlock, UBYTE ubTableSelect)
{
#if(!RDT)
	UWORD uwIndex;
	UWORD LastEC;
	UWORD childEC;
	FTarget_t *uwFreeBlockTable;
	EraseCount_t *ulEC;
	UWORD uwFreeBlockCount;

	if (ubTableSelect == D3) {
		uwFreeBlockTable = guwFreeBlockTable_D3;
		ulEC = (EraseCount_t*)gulEC_D3;
		uwFreeBlockCount = VT->guwFreeBlockCount_D3;
		if (ubPopAllFreeBlock == 0) {
			M_AssertCriticalError(uwFreeBlockCount < 1, VT_ASSERT_GET_USER_FREE_BLOCK_0x4C);
		}
	}
	else {
		uwFreeBlockTable = guwFreeBlockTable_D1;
		ulEC = (EraseCount_t*)gulEC_D1;
		uwFreeBlockCount = VT->guwFreeBlockCount_D1;
		M_AssertCriticalError(uwFreeBlockCount == 0, VT_ASSERT_GET_USER_FREE_BLOCK_0x4D);
	}
#if TLC
	if (ENABLE_DEBUG_RW && (0 == gubRebuildFreeQueueDoing)) {
#else
	if (ENABLE_DEBUG_RW) {
#endif
		UWORD uwi;
		for (uwi = 3; uwi < uwFreeBlockCount + 2; uwi++) {
			M_AssertError(M_UserFreeTableEC(ulEC, uwFreeBlockTable, 2) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi));
		}

		if (uwFreeBlockCount > 2) {
			for (uwi = 4; uwi < uwFreeBlockCount + 2; uwi++) {
				M_AssertError(M_UserFreeTableEC(ulEC, uwFreeBlockTable, 3) < M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi));
			}
		}
	}
	uwTarget->B.uwTarget = uwFreeBlockTable[2].B.uwTarget;
#if TLC
	if (D3 == ubTableSelect) {
		gulVC[uwTarget->B.uwTarget].B.btFastPage = 0;
	}
#else
	gulVC[uwTarget->B.uwTarget].B.btFastPage = gubUltraMLCEnable;
	uwTarget->B.btFastPage = gubUltraMLCEnable;
#endif
	uwFreeBlockCount--;
	uwIndex = 2;
	UWORD uwLast = uwFreeBlockCount + 2;
	LastEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwLast);//Add a MixPlane Weights.

	while (1) {
		UWORD leftminchild = 2 * uwIndex;
		UWORD rightminchild = 2 * (uwIndex + 1);
		UWORD children;
		U32 ulCheckEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex + 1);
		if ((ulCheckEC < LastEC) && ((uwIndex + 1) < uwLast)) {
			uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwIndex + 1].B.uwTarget;
			uwFreeBlockTable[uwIndex + 1].B.uwTarget = uwFreeBlockTable[uwLast].B.uwTarget;
			uwFreeBlockTable[uwLast].B.uwTarget = uwFreeBlockTable[uwIndex].B.uwTarget;
		}
		if (leftminchild >= uwLast) {

			break;
		}

		if (rightminchild > uwLast) {
			children = leftminchild;
		}
		else {
			U32 ulLeftChildEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, leftminchild);
			U32 ulRightChildEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, rightminchild);
			if ((ulLeftChildEC > ulRightChildEC) && (rightminchild < uwLast)) {
				children = rightminchild;
			}
			else {
				children = leftminchild;
			}
		}


		childEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, children);
		if (LastEC < childEC) {
			break;
		}
		else {
			uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[children].B.uwTarget;
		}
		uwIndex = children;
	}
	uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwLast].B.uwTarget;
	uwFreeBlockTable[1].B.uwTarget = uwFreeBlockTable[2].B.uwTarget;
#if TLC
	if (ENABLE_DEBUG_RW && (0 == gubRebuildFreeQueueDoing)) {
#else
	if (ENABLE_DEBUG_RW) {
#endif
		UWORD uwi;
		for (uwi = 2; uwi < uwFreeBlockCount + 2; uwi = uwi + 2) {
			M_AssertError((M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi + 1)) && ((uwi + 1) < uwLast));
		}

		for (uwi = 3; uwi < uwFreeBlockCount + 2; uwi++) {
			M_AssertError(M_UserFreeTableEC(ulEC, uwFreeBlockTable, 2) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi));
		}

		if (uwFreeBlockCount > 2) {
			for (uwi = 4; uwi < uwFreeBlockCount + 2; uwi++) {
				M_AssertError(M_UserFreeTableEC(ulEC, uwFreeBlockTable, 3) < M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi));
			}
		}
	}
	if (ubTableSelect == D3) {
		VT->guwFreeBlockCount_D3 = uwFreeBlockCount;
	}
	else {
#if TLC
		UBYTE ubDie;
		VT->guwFreeBlockCount_D1 = uwFreeBlockCount;
		M_GetDieNumber(ubDie, uwTarget->B.uwTarget, D1);
		VT->gubD1DieCount[ubDie]--;
#endif
	}
	return;
#endif
}
#if TLC
UBYTE ftlGetUserFreeBlockByDie (FTarget_t * uwTarget, UBYTE ubDie, UBYTE ubTableSelect)
{

	UWORD uwUnit;
	UWORD uwFreeBlockTablePTR;
	UWORD uwFreeBlockTableCount = 0;
	UWORD uwEndOfMinNode;
	UWORD uwStartOfMaxNode;
	UBYTE ubCompareDie;
	UBYTE ubisFind = 0;
	FTarget_t* uwFreeBlockTable;

	if (ubTableSelect == D3) {
		uwFreeBlockTable = guwFreeBlockTable_D3;
		uwFreeBlockTableCount = VT->guwFreeBlockCount_D3;
	}
	else if (ubTableSelect == D1) {
		uwFreeBlockTable = guwFreeBlockTable_D1;
		uwFreeBlockTableCount = VT->guwFreeBlockCount_D1;
	}

	//Search Die

	//=======================================================================================================
	// !!!!! 可以想如何找Die
	//=======================================================================================================
	if (uwFreeBlockTableCount != 0) { //確保uwFreeBlockTablePTR不會減超過
		if (uwFreeBlockTableCount & 1) {
			uwEndOfMinNode = uwFreeBlockTableCount + 1;
			uwStartOfMaxNode = uwFreeBlockTableCount;
		}
		else {
			uwEndOfMinNode = uwFreeBlockTableCount;
			uwStartOfMaxNode = uwFreeBlockTableCount + 1;
		}
		for (uwFreeBlockTablePTR = 2; uwFreeBlockTablePTR <= uwEndOfMinNode; uwFreeBlockTablePTR += 2) {
			M_GetDieNumber(ubCompareDie, uwFreeBlockTable[uwFreeBlockTablePTR].B.uwTarget, ubTableSelect);
			if (ubDie == ubCompareDie) {
				uwUnit = uwFreeBlockTable[uwFreeBlockTablePTR].B.uwTarget;
				ubisFind = 1;
				break;
			}
		}

		if (!ubisFind) {
			for (uwFreeBlockTablePTR = uwStartOfMaxNode; uwFreeBlockTablePTR >= 3; uwFreeBlockTablePTR -= 2) {
				M_GetDieNumber(ubCompareDie, uwFreeBlockTable[uwFreeBlockTablePTR].B.uwTarget, ubTableSelect);
				if (ubDie == ubCompareDie) {
					uwUnit = uwFreeBlockTable[uwFreeBlockTablePTR].B.uwTarget;
					ubisFind = 1;
					break;
				}
			}
		}
	}
	if (ubisFind == 0) {
		return 0;
	}
	//M_AssertError(ubisFind == 0);
	ftlGetRandomFreeBlock(uwTarget, uwUnit, ubTableSelect, BIT_SEARCH_BY_UNIT, 0);
#if TLC
	if (D3 == ubTableSelect) {
		gulVC[uwTarget->B.uwTarget].B.btFastPage = 0;
	}
#else
	gulVC[uwTarget->B.uwTarget].B.btFastPage = gubUltraMLCEnable;
	uwTarget->B.btFastPage = gubUltraMLCEnable;
#endif
	return 1;
}
#endif

void ftlGetMinMaxECFreeBlock(FTarget_t *uwTarget, UBYTE ubTableSelect, UBYTE ubMode)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	UWORD uwIndex;
	FTarget_t *uwFreeBlockTable;
	EraseCount_t *ulEC;
	UWORD uwFreeBlockCount;
	if (ubTableSelect == D3) {
		uwFreeBlockTable = guwFreeBlockTable_D3;
		ulEC = (EraseCount_t*)gulEC_D3;
		uwFreeBlockCount = VT->guwFreeBlockCount_D3;
	}
	else {
		uwFreeBlockTable = guwFreeBlockTable_D1;
		ulEC = (EraseCount_t*)gulEC_D1;
		uwFreeBlockCount = VT->guwFreeBlockCount_D1;
	}
	M_AssertCriticalError(uwFreeBlockCount < 1, VT_ASSERT_GET_MAX_EC_FREE_BLOCK_0x4E);
	if (ENABLE_DEBUG_RW) {
		UWORD uwi;
		for (uwi = 3; uwi < uwFreeBlockCount + 2; uwi++) {
			M_AssertError(M_UserFreeTableEC(ulEC, uwFreeBlockTable, 2) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi));
		}
	}
	if (ENABLE_DEBUG_RW && (uwFreeBlockCount > 2)) {
		UWORD uwi;
		for (uwi = 4; uwi < uwFreeBlockCount + 2; uwi++) {
			M_AssertError(M_UserFreeTableEC(ulEC, uwFreeBlockTable, 3) < M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi));
		}
	}

	if (ubMode == GET_2ND_MAX_EC_FREE) {
		if (uwFreeBlockCount == 2) {
			uwIndex = 3;
		}
		else {
			uwIndex = ftlGet2ndMaxECFreeIndex(ubTableSelect);
		}
	}
	else if (ubMode == GET_2ND_MIN_EC_FREE) {
		uwIndex = ftlGet2ndMinECFreeIndex(ubTableSelect);
	}
	else if (ubMode == GET_MAX_EC_FREE) {
		uwIndex = (uwFreeBlockCount == 1) ? 2 : 3;
	}
	else {
		uwIndex = 2;
	}
	uwTarget->B.uwTarget = uwFreeBlockTable[uwIndex].B.uwTarget;
	uwFreeBlockCount--;
	UWORD uwLast = uwFreeBlockCount + 2;
#if TLC
	if (D3 == ubTableSelect) {
		gulVC[uwTarget->B.uwTarget].B.btFastPage = 0;
	}
#else
	gulVC[uwTarget->B.uwTarget].B.btFastPage = gubUltraMLCEnable;
	uwTarget->B.btFastPage = gubUltraMLCEnable;
#endif
	U32 ulLastEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwLast);//Add a MixPlane Weights.
	if (uwIndex & 1) {
		while (1) {
			UWORD leftmaxchild = 2 * (uwIndex - 1) + 1;
			UWORD rightmaxchild = 2 * uwIndex + 1;
			UWORD children;
			U32 ulCheckEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex - 1);
			if (ulCheckEC > ulLastEC) {
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwIndex - 1].B.uwTarget;
				uwFreeBlockTable[uwIndex - 1].B.uwTarget = uwFreeBlockTable[uwLast].B.uwTarget;
				uwFreeBlockTable[uwLast].B.uwTarget = uwFreeBlockTable[uwIndex].B.uwTarget;
			}
			if (leftmaxchild >= uwLast) {

				break;
			}

			if (rightmaxchild > uwLast) {
				children = leftmaxchild;
			}
			else {
				U32 ulLeftMaxChildEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, leftmaxchild);
				U32 ulRightMaxChildEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, rightmaxchild);
				if ((ulLeftMaxChildEC < ulRightMaxChildEC) && (rightmaxchild < uwLast)) {
					children = rightmaxchild;
				}
				else {
					children = leftmaxchild;
				}
			}
			U32 ulChildrenEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, children);
			if (ulLastEC < ulChildrenEC) {
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[children].B.uwTarget;
			}
			else {
				break;
			}
			uwIndex = children;
		}
		uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwLast].B.uwTarget;
		uwFreeBlockTable[0].B.uwTarget = uwFreeBlockTable[3].B.uwTarget;
	}
	else {
		while (1) {
			UWORD leftminchild = 2 * uwIndex;
			UWORD rightminchild = 2 * (uwIndex + 1);
			UWORD children;
			U32 ulCheckEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex + 1);
			if ((ulCheckEC < ulLastEC) && ((uwIndex + 1) < uwLast)) {
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwIndex + 1].B.uwTarget;
				uwFreeBlockTable[uwIndex + 1].B.uwTarget = uwFreeBlockTable[uwLast].B.uwTarget;
				uwFreeBlockTable[uwLast].B.uwTarget = uwFreeBlockTable[uwIndex].B.uwTarget;
			}
			if (leftminchild >= uwLast) {

				break;
			}

			if (rightminchild > uwLast) {
				children = leftminchild;
			}
			else {
				U32 ulLeftChildEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, leftminchild);
				U32 ulRightChildEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, rightminchild);
				if ((ulLeftChildEC > ulRightChildEC) && (rightminchild < uwLast)) {
					children = rightminchild;
				}
				else {
					children = leftminchild;
				}
			}

			U32 ulChildrenEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, children);
			if (ulLastEC < ulChildrenEC) {
				break;
			}
			else {
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[children].B.uwTarget;
			}
			uwIndex = children;
		}
		uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwLast].B.uwTarget;
		uwFreeBlockTable[1].B.uwTarget = uwFreeBlockTable[2].B.uwTarget;
	}
	if (ubTableSelect == D3) {
		VT->guwFreeBlockCount_D3 = uwFreeBlockCount;
	}
	else {
#if TLC
		VT->guwFreeBlockCount_D1 = uwFreeBlockCount;
		UBYTE ubDie;
		M_GetDieNumber(ubDie, uwTarget->B.uwTarget, D1);
		VT->gubD1DieCount[ubDie]--;
#endif
	}
	return;
#endif
}

UWORD ftlGet2ndMinECFreeIndex(UBYTE ubTableSelect)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	UWORD uwFreeQIndex;
	//FTarget_t uwTarget;
	FTarget_t *uwFreeBlockTable;
	EraseCount_t *ulEC;
	UWORD uwFreeBlockCount;
	UWORD uwi;
	U32 ulMinEC;

	if (ubTableSelect == D3) {
		uwFreeBlockTable = guwFreeBlockTable_D3;
		ulEC = (EraseCount_t*)gulEC_D3;
		uwFreeBlockCount = VT->guwFreeBlockCount_D3;
	}
	else {
		uwFreeBlockTable = guwFreeBlockTable_D1;
		ulEC = (EraseCount_t*)gulEC_D1;
		uwFreeBlockCount = VT->guwFreeBlockCount_D1;
	}

	if (uwFreeBlockCount <= 3) {
		uwFreeQIndex = uwFreeBlockCount + 1;
	}
	else {
		uwFreeQIndex = 4;
		ulMinEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwFreeQIndex);
		for (uwi = uwFreeQIndex + 1; uwi < uwFreeBlockCount + 2 && uwi < 8; uwi++) {
			if (M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi) < ulMinEC) {
				uwFreeQIndex = uwi;
				ulMinEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwFreeQIndex);
			}
		}
	}

	return uwFreeQIndex;
#endif
}

UWORD ftlGet2ndMaxECFreeIndex(UBYTE ubTableSelect)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	UWORD uwFreeQIndex;
	//FTarget_t uwTarget;
	FTarget_t *uwFreeBlockTable;
	EraseCount_t *ulEC;
	UWORD uwFreeBlockCount;
	UWORD uwi;
	U32 ulMaxEC;

	if (ubTableSelect == D3) {
		uwFreeBlockTable = guwFreeBlockTable_D3;
		ulEC = (EraseCount_t*)gulEC_D3;
		uwFreeBlockCount = VT->guwFreeBlockCount_D3;
	}
	else {
		uwFreeBlockTable = guwFreeBlockTable_D1;
		ulEC = (EraseCount_t*)gulEC_D1;
		uwFreeBlockCount = VT->guwFreeBlockCount_D1;
	}

	if (uwFreeBlockCount <= 2) {
		uwFreeQIndex = 2;
	}
	else {
		uwFreeQIndex = 4;
		ulMaxEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwFreeQIndex);
		for (uwi = uwFreeQIndex + 1; uwi < uwFreeBlockCount + 2 && uwi < 8; uwi++) {
			if (M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwi) > ulMaxEC) {
				uwFreeQIndex = uwi;
				ulMaxEC = M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwFreeQIndex);
			}
		}
	}

	return uwFreeQIndex;
#endif
}


void ftlGetRandomFreeBlock(FTarget_t *uwTarget, UWORD uwFreeUnitPtr, UBYTE ubTableSelect, UBYTE ubMode, UWORD uwUnitIndex)
{
	UWORD uwIndex = 0;
	FTarget_t *uwFreeBlockTable;
	UWORD uwFreeBlockCount;
	EraseCount_t *ulEC;
	if (ubTableSelect == D3) {
		uwFreeBlockTable = guwFreeBlockTable_D3;
		uwFreeBlockCount = VT->guwFreeBlockCount_D3;
		ulEC = (EraseCount_t*)gulEC_D3;
	}
	else { // ubTableSelect == D1
		uwFreeBlockTable = guwFreeBlockTable_D1;
		uwFreeBlockCount = VT->guwFreeBlockCount_D1;
		ulEC = (EraseCount_t*)gulEC_D1;
	}

	if (ubMode & BIT_SEARCH_BY_UNIT) {
		while (SEB[SEB_SQ_WR_CNT] < 1);
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_CMD] =  SE0_SEQU | SE0_CMDUNIT_1 | SE0_SIZE_2BYTE | SE0_ENABLE_MASK | SE0_FINDMORE | SE0_TARGET(1);
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_SADDR0] = (U32) (uwFreeBlockTable + 2);
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_LEN] =  (uwFreeBlockCount) * 2;
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_bMASK0] = 0x7FFF;
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_PAT0] = uwFreeUnitPtr;
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_PAT1] = 0;
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_PAT2] = 0;
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_PAT3] = 0;
		SEB[SEB_SQ_WPIU] = 1;
		while (SEB[SEB_CQ_RD_CNT] < 1);
		SEB[SEB_CQ_RPIU] = 1;
		if (SEL_CQBaseAddr[guwCQCnt_SE0 * 8] & BIT8) {
			uwIndex = SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + SE0_SH_RESULT_0];
			uwIndex = uwIndex + 2  ;
			M_AssertError(uwFreeBlockTable[uwIndex].B.uwTarget  != uwFreeUnitPtr);
		}
		else {
			M_AssertError(1);
		}
		guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
		guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;
	}
	else if (ubMode & BIT_SEARCH_BY_INDEX) {
		uwIndex = uwUnitIndex;
	}
	else if ((ubMode & BIT_SEARCH_BY_AVOID_MIXPLANE) || (ubMode & BIT_SEARCH_BY_FORCE_MIXPLANE)) {
		UBYTE ubFindUnit = 0;
		UBYTE ubUnitType = (ubMode & BIT_SEARCH_BY_AVOID_MIXPLANE) ? FALSE : TRUE;
		//盡量不讓SLC Pool取得MixPlane Unit
		//Round 1 : 找EC相對小
		for (uwIndex = 2; uwIndex < uwFreeBlockCount + 2; uwIndex += 2) {
			if (ulEC[uwFreeBlockTable[uwIndex].B.uwTarget].B.btMixPlaneUnit == ubUnitType) {
				ubFindUnit = 1;
				break;
			}
		}
		//Round 2 : 找到EC相對大
		if (!ubFindUnit) {
			//(uwFreeBlockCount + 1) = 最後一個Index; 找最後一個奇數Index
			for (uwIndex = (((uwFreeBlockCount) % 2 == 0) ? (uwFreeBlockCount + 1) : uwFreeBlockCount); uwIndex >= 3; uwIndex -= 2) { //最後一個不要抓 符合演算法
				if (ulEC[uwFreeBlockTable[uwIndex].B.uwTarget].B.btMixPlaneUnit == ubUnitType) {
					ubFindUnit = 1;
					break;
				}
			}
		}

		if (!ubFindUnit) {
			uwIndex = 2;
		}
	}

	// FreeUnitPtr → uwIndex
	uwTarget->B.uwTarget = uwFreeBlockTable[uwIndex].B.uwTarget;
	uwFreeBlockCount--;
	uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwFreeBlockCount + 2].B.uwTarget;
#if TLC
	if (D3 == ubTableSelect) {
		gulVC[uwTarget->B.uwTarget].B.btFastPage = 0;
	}
#else
	gulVC[uwTarget->B.uwTarget].B.btFastPage = gubUltraMLCEnable;
	uwTarget->B.btFastPage = gubUltraMLCEnable;
#endif
	// (1) 向下排
	if (uwIndex % 2 == 1) {
		// Max
		while (1) {
			UWORD leftmaxchild, rightmaxchild, children;

			// (a) 往下比
			leftmaxchild = 2 * (uwIndex - 1) + 1;
			rightmaxchild = 2 * uwIndex + 1;
			if ((rightmaxchild <= (uwFreeBlockCount + 2)) && (M_UserFreeTableEC(ulEC, uwFreeBlockTable, leftmaxchild) < M_UserFreeTableEC(ulEC, uwFreeBlockTable, rightmaxchild))) {
				children = rightmaxchild;
			}
			else {
				children = leftmaxchild;
			}
			if ((leftmaxchild <= (uwFreeBlockCount + 2)) && (M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex) < M_UserFreeTableEC(ulEC, uwFreeBlockTable, children))) {
				UWORD uwSwap_Temp = uwFreeBlockTable[uwIndex].B.uwTarget;
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[children].B.uwTarget;
				uwFreeBlockTable[children].B.uwTarget = uwSwap_Temp;
				uwIndex = children;
				continue;
			}

			// (b) 左右比
			if  (M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex - 1) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex)) {
				UWORD uwSwap_Temp = uwFreeBlockTable[uwIndex].B.uwTarget;
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwIndex - 1].B.uwTarget;
				uwFreeBlockTable[uwIndex - 1].B.uwTarget = uwSwap_Temp;
				uwIndex = uwIndex - 1;
			}
			break;
		}
		uwFreeBlockTable[0].B.uwTarget = uwFreeBlockTable[3].B.uwTarget;
	}
	else {
		// Min
		while (1) {
			UWORD leftminchild, rightminchild, children;

			// (a) 往下比
			leftminchild = 2 * uwIndex;
			rightminchild = 2 * (uwIndex + 1);
			if ((rightminchild <= (uwFreeBlockCount + 2)) && (M_UserFreeTableEC(ulEC, uwFreeBlockTable, leftminchild) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, rightminchild))) {
				children = rightminchild;
			}
			else {
				children = leftminchild;
			}

			if ((leftminchild <= (uwFreeBlockCount + 2)) && (M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, children))) {
				UWORD uwSwap_Temp = uwFreeBlockTable[uwIndex].B.uwTarget;
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[children].B.uwTarget;
				uwFreeBlockTable[children].B.uwTarget = uwSwap_Temp;
				uwIndex = children;
				continue;
			}

			// (b) 左右比
			if (((uwIndex + 1) <= (uwFreeBlockCount + 2)) && (M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex + 1) < M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex))) {
				UWORD uwSwap_Temp = uwFreeBlockTable[uwIndex].B.uwTarget;
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[uwIndex + 1].B.uwTarget;
				uwFreeBlockTable[uwIndex + 1].B.uwTarget = uwSwap_Temp;
				uwIndex = uwIndex + 1;
			}
			break;
		}
		uwFreeBlockTable[1].B.uwTarget = uwFreeBlockTable[2].B.uwTarget;
	}


	// (2)向上排
	if (uwIndex != (uwFreeBlockCount + 2)) {

		while (uwIndex >= 4) {
			UWORD gp = uwIndex / 4;
			UWORD L_Children = (2 * gp);
			UWORD R_Children = (2 * gp) + 1;

			if (M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex) < M_UserFreeTableEC(ulEC, uwFreeBlockTable, L_Children)) {
				UWORD uwSwap_Temp = uwFreeBlockTable[uwIndex].B.uwTarget;
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[L_Children].B.uwTarget;
				uwFreeBlockTable[L_Children].B.uwTarget = uwSwap_Temp;
				uwIndex = L_Children;
				continue;
			}
			else if (M_UserFreeTableEC(ulEC, uwFreeBlockTable, uwIndex) > M_UserFreeTableEC(ulEC, uwFreeBlockTable, R_Children)) {
				UWORD uwSwap_Temp = uwFreeBlockTable[uwIndex].B.uwTarget;
				uwFreeBlockTable[uwIndex].B.uwTarget = uwFreeBlockTable[R_Children].B.uwTarget;
				uwFreeBlockTable[R_Children].B.uwTarget = uwSwap_Temp;
				uwIndex = R_Children;
				continue;
			}
			else
				break;
		}
		uwFreeBlockTable[0].B.uwTarget = uwFreeBlockTable[3].B.uwTarget;
		uwFreeBlockTable[1].B.uwTarget = uwFreeBlockTable[2].B.uwTarget;
	}

	if (ubTableSelect == D3) {
		VT->guwFreeBlockCount_D3 = uwFreeBlockCount;
	}
	else {
#if TLC
		UBYTE ubDie;
		VT->guwFreeBlockCount_D1 = uwFreeBlockCount;
		M_GetDieNumber(ubDie, uwTarget->B.uwTarget, D1);
		VT->gubD1DieCount[ubDie]--;
#endif
	}



	return;
}

UBYTE ftlGetMaxECUnitIndex()
{
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);
	if (VT->guwFreeBlockCount_D3 >= 6) {
		return 7;
	}
	else if ((VT->guwFreeBlockCount_D3 >= 4) && (VT->guwFreeBlockCount_D3 <= 5)) {
		return 5;
	}
	else  if (VT->guwFreeBlockCount_D3 <= 3) {
		return 3;
	}
	else {
		return 2;
	}
}

void ftlCheckUserFreeBlockTable()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	M_AssertError(VT->guwFreeBlockCount_D3 < 2);

	if (ENABLE_DEBUG_RW) {
		UWORD uwi;
		for (uwi = 2; uwi < VT->guwFreeBlockCount_D3 + 2; uwi = uwi + 2) {
			M_AssertError((M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, uwi) > M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, uwi + 1)) && ((uwi + 1) < (VT->guwFreeBlockCount_D3 + 2)));
		}
	}
	if (ENABLE_DEBUG_RW) {
		UWORD uwi;
		for (uwi = 3; uwi < VT->guwFreeBlockCount_D3 + 2; uwi++) {
			M_AssertError(M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, 2) > M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, uwi));
		}
	}
	if (ENABLE_DEBUG_RW && (VT->guwFreeBlockCount_D3 > 2)) {
		UWORD uwi;
		for (uwi = 4; uwi < VT->guwFreeBlockCount_D3 + 2; uwi++) {
			M_AssertError(M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, 3) < M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, uwi));
		}
	}

	if (ENABLE_DEBUG_RW) {	// check left child
		UBYTE ubIsMax = FALSE, ubIsMin = FALSE;
		UWORD uwi;
		UWORD uwCheck;
		UWORD uwParent;
		UWORD uwLeftRange, uwRightRange;
		for (uwCheck = 4; uwCheck < VT->guwFreeBlockCount_D3 + 2; uwCheck++) {
			uwParent = uwCheck / 2;
			if (uwCheck % 2) {
				ubIsMax = TRUE;
				ubIsMin = FALSE;
			}
			else {
				ubIsMax = FALSE;
				ubIsMin = TRUE;
			}


			uwLeftRange = uwParent * 2;
			uwRightRange = (uwParent * 2) + 1;


			uwi = uwLeftRange;

			while (uwi < VT->guwFreeBlockCount_D3 + 2) {

				if ( (uwi >= uwLeftRange) && (uwi <= uwRightRange)) {
					if (ubIsMin) { // left
						M_AssertError(M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, uwCheck) > M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, uwi));
					}
					else {	// right
						M_AssertError(M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, uwCheck) < M_UserFreeTableEC(gulEC_D3, guwFreeBlockTable_D3, uwi));
					}
				}
				uwi++;

				if (uwi == (uwLeftRange * 2)) {	// 到下一層heap了, 重新計算LeftRange跟RightRange
					uwLeftRange = uwLeftRange * 2;
					uwRightRange = (uwRightRange * 2) + 1;
				}

			}
		}
	}

}

UBYTE ftlGenZCodeData(UBYTE ubZCode, U32 ulTargetAddr, U32 ulL4K_LCA, UBYTE ubFlagIndex)
{
#if(!RDT)
	/*	產生下列zip size的 4k data 所需RandomSectors數量分別為
		1KB		:	0 sectors (random data) + 8 sectors (sequential or repeaten byte)
		1.5KB	:	1 sectors (random data) + 7 sectors (sequential or repeaten byte)
		2KB		:	2 sectors (random data) + 6 sectors (sequential or repeaten byte)
		2.5KB	:	3 sectors (random data) + 5 sectors (sequential or repeaten byte)
		3KB		:	5 sectors (random data) + 3 sectors (sequential or repeaten byte)
		3.5K	B	:	6 sectors (random data) + 2 sectors (sequential or repeaten byte)
		4KB		:	8 sectors (random data) + 0 sectors (sequential or repeaten byte)
	*/

	M_CheckBurnerModeToDisableFunctionReturnValid(0);

	UBYTE ubRandomDataSectors;
	UBYTE ubGenZCode;
	UBYTE ubGenZCodeMap;
	U32 ulCQAddress;

	if (ubZCode == MAX_ZCODE) {
		// Zcode為7, 表示不壓縮, 需要8個RandomSectors來產生data
		ubRandomDataSectors = 8;
	}
	else {
		if (gubLZSS_RESOLUTION == 1) {
			// 此處Resolution 1 的Zcode為 0, 1, 2, 分別表示1KB, 2KB, 3KB, 分別需要0, 2, 5個RandomSectors來產生data
			ubRandomDataSectors = (ubZCode << 1) + (ubZCode >> 1);
		}
		else if (gubLZSS_RESOLUTION == 2) {
			// 此處Resolution 2 的Zcode為 0, 表示2KB, 需要2個RandomSectors來產生data
			ubRandomDataSectors = 2;
		}
		else {
			// 此處Resolution 0 的Zcode為 1~6, 分別表示1KB~3.5KB, 分別需要0, 1, 2, 3, 5, 6個RandomSectors來產生data
			ubRandomDataSectors = ubZCode - ((ubZCode > 4) ? 0 : 1);
		}
	}

	if (ubRandomDataSectors > 8) {
		M_AssertError(1);
		ubRandomDataSectors = 8;
	}

	ubGenZCodeMap = 0;

Mark_ReGenZCodeData:
	if (ubRandomDataSectors) {	// random data sectors
#if BICS2_GC_TO_D3
		//避免D3GC在FQ塞滿WL前提前進去flaTask下Cmd且當下WaitCnt=0導致Program Order被打斷
		mDMAC_SV(4, ulTargetAddr,  ubRandomDataSectors * 512, ulL4K_LCA, 0, 0, DMA_EN_RAND | DMA_EN_SETCQ, (enumWriteWaitCntMode == WaitCnt_GC ? 0 : 1));
#else
		mDMAC_SV(4, ulTargetAddr,  ubRandomDataSectors * 512, ulL4K_LCA, 0, 0, DMA_EN_RAND | DMA_EN_SETCQ, 1);
#endif
	}

	if (ubRandomDataSectors != 8) {	// fixed data sectors
#if BICS2_GC_TO_D3
		//避免D3GC在FQ塞滿WL前提前進去flaTask下Cmd且當下WaitCnt=0導致Program Order被打斷
		mDMAC_SV(4, ulTargetAddr + (ubRandomDataSectors * 512), (8 - ubRandomDataSectors) * 512, ulL4K_LCA + ubRandomDataSectors, ZCodeData, 0, DMA_EN_SETCQ, (enumWriteWaitCntMode == WaitCnt_GC ? 0 : 1));
#else
		mDMAC_SV(4, ulTargetAddr + (ubRandomDataSectors * 512), (8 - ubRandomDataSectors) * 512, ulL4K_LCA + ubRandomDataSectors, ZCodeData, 0, DMA_EN_SETCQ, 1);
#endif
	}

	ulCQAddress = DCCM0_BANK0_BASE_LONG[0x28 >> 2];
	mDMAC_PZIP(ulTargetAddr, 0, 1);
	ubGenZCode = ((unsigned char*)(ulCQAddress))[0];

	// If hardware set no zip, ubGenZcode should be "0".
	if (gubLZSS_RESOLUTION == 3) {
		if ( (ENABLE_DEBUG_RW) && (ubGenZCode != 0)) {
			flaLEDBlink(1);
		}
		else {
			ubGenZCode = MAX_ZCODE;
		}
	}

	if (ubGenZCodeMap == (BIT2 | BIT1 | BIT0)) {	// random sectors變多變少 gen 完都失敗後, 就直接gen zcode 1, 然後用force zcode方式program
		return 1;
	}

	if (ubGenZCode != ubZCode) {
		if (ubGenZCodeMap == (BIT1 | BIT0)) {
			ubRandomDataSectors = 0;
			ubGenZCodeMap |= BIT2;
		}
		if (ubGenZCode < ubZCode) {	// less random sectors
			ubRandomDataSectors++;
			ubGenZCodeMap |= BIT1;
			//UartString("+random\n");
		}
		else {	// too many random sectors
			ubRandomDataSectors--;
			ubGenZCodeMap |= BIT0;
			//UartString("-random\n");
		}

		if (ubFlagIndex != 0xFF) {
			AXIB[ubFlagIndex] = 0;
		}

		goto Mark_ReGenZCodeData;
	}
#endif
	return 0;

}
void ftlAddToDebugCommanddQueue(U32 ulCmd, U32 ulLBA, U32 ulSectorCnt, U32 ulOther)
{
#if(!(RDT||BURNER))
	gDebugCommandQueue[gubDebugCommandQueueIndex].ulCmd = ulCmd;
	gDebugCommandQueue[gubDebugCommandQueueIndex].ulLBA = ulLBA;
	gDebugCommandQueue[gubDebugCommandQueueIndex].ulSectorCnt = ulSectorCnt;
	gDebugCommandQueue[gubDebugCommandQueueIndex].ulOther = ulOther;
	gubDebugCommandQueueIndex++;
	if (gubDebugCommandQueueIndex == (BYTE_COMMAND_QUEUE / 16)) {
		gubDebugCommandQueueIndex = 0;
	}
	gDebugCommandQueue[gubDebugCommandQueueIndex].ulCmd = 0xFFFFFFFF;
	gDebugCommandQueue[gubDebugCommandQueueIndex].ulLBA = 0xFFFFFFFF;
	gDebugCommandQueue[gubDebugCommandQueueIndex].ulSectorCnt = 0xFFFFFFFF;
	gDebugCommandQueue[gubDebugCommandQueueIndex].ulOther = 0xFFFFFFFF;
#endif
}



void SetQInfo()
{
	//DMAC
	// Reset
	DCCM0_BANK0_BASE_BYTE[0x17] &= ~BIT0; //invalid SQ
	DCCM0_BANK0_BASE_BYTE[0x37] &= ~BIT0; //invalid CQ

	DCCM0_BANK0_BASE_LONG[0] = Quue_Addr_DMAC_SQ; //SQ base address
	DCCM0_BANK0_BASE_BYTE[0x14] = MaxSQCnt_DMAC; //Buffer Length=Data Size*MaxSQCnt_DMAC
	DCCM0_BANK0_BASE_BYTE[0x16] = 5; //Data Size=32 Byte
	DCCM0_BANK0_BASE_BYTE[0x17] |= BIT1; //Reset
	DCCM0_BANK0_BASE_BYTE[0x17] &= (~BIT1); //Reset
	DCCM0_BANK0_BASE_BYTE[0x17] |= BIT0; //valid Queue

	DCCM0_BANK0_BASE_LONG[8] = Quue_Addr_DMAC_CQ; //CQ base address
	DCCM0_BANK0_BASE_BYTE[0x34] = MaxCQCnt_DMAC; //Buffer Length=Data Size*MaxCQCnt_DMAC
	DCCM0_BANK0_BASE_BYTE[0x36] = 3; //Data Size=8Byte
	DCCM0_BANK0_BASE_BYTE[0x37] |= BIT1; //Reset
	DCCM0_BANK0_BASE_BYTE[0x37] &= (~BIT1);  //Reset
	DCCM0_BANK0_BASE_BYTE[0x37] |= BIT0; //valid Queue
	guwSQCnt_DMAC = 0;
	guwCQCnt_DMAC = 0;

	//SE
	//Reset
	DCCM0_BANK0_BASE_BYTE[0x57] &= ~BIT0; //invalid SQ
	DCCM0_BANK0_BASE_BYTE[0x77] &= ~BIT0; //invalid CQ

	DCCM0_BANK0_BASE_LONG[16] = Quue_Addr_SS_SQ; //SQ base address
	DCCM0_BANK0_BASE_BYTE[0x54] = MaxSQCnt_SE; //Buffer Length=Data Size*MaxSQCnt_SE
	DCCM0_BANK0_BASE_BYTE[0x56] = 5; //Data Size=32 Byte
	DCCM0_BANK0_BASE_BYTE[0x57] |= BIT1; //Reset
	DCCM0_BANK0_BASE_BYTE[0x57] &= (~BIT1); //Reset
	DCCM0_BANK0_BASE_BYTE[0x57] |= BIT0; //valid Queue

	DCCM0_BANK0_BASE_LONG[24] = Quue_Addr_SS_CQ; //CQ base address
	DCCM0_BANK0_BASE_BYTE[0x74] = MaxCQCnt_SE; //Buffer Length=Data Size*MaxCQCnt_SE
	DCCM0_BANK0_BASE_BYTE[0x76] = 5; //Data Size=32Byte
	DCCM0_BANK0_BASE_BYTE[0x77] |= BIT1; //Reset
	DCCM0_BANK0_BASE_BYTE[0x77] &= (~BIT1); //Reset
	DCCM0_BANK0_BASE_BYTE[0x77] |= BIT0; //valid Queue
	guwSQCnt_SE0 = 0;
	guwCQCnt_SE0 = 0;

	//SATA
	DCCM0_BANK0_BASE_BYTE[0xD7] &= ~BIT0; //invalid Queue
	DCCM0_BANK0_BASE_LONG[0xC0 >> 2] = Quue_Addr_SATA_DoneTag; //SATA Qbase address
	DCCM0_BANK0_BASE_BYTE[0xD4] = MaxQLength_SATA; //Buffer Length=Data Size*MaxQLength_SATA
	DCCM0_BANK0_BASE_BYTE[0xD6] = 0x0; //Data Size=1 Byte
	DCCM0_BANK0_BASE_BYTE[0xD7] |= BIT1; //Reset
	DCCM0_BANK0_BASE_BYTE[0xD7] &= (~BIT1); //Reset
	DCCM0_BANK0_BASE_BYTE[0xD7] |= BIT0; //valid Queue

	// set doorbell for FLH msg dispatch
	DCCM0_BANK0_BASE_BYTE[(0x97)] |= BIT0;
}
void mSE_SV(UBYTE ubOperationSize, U32 ulSourceAddr, U32 ulByteLength, U32 ulPattern_L, U32 ulPattern_H)
{

	//M_AssertError(ulByteLength > Z2_Size);
	UBYTE ubSizeUnit;
	UBYTE ubCmdUnit = 1;
	UWORD uwSQCnt_SE0 = guwSQCnt_SE0;
	if (ENABLE_DCACHE) {
		if ((ulSourceAddr >= Z2_DDR_BASE) && (ulSourceAddr < Z3_DDR_BASE)) {
			xthal_dcache_region_invalidate((void *)ulSourceAddr, ulByteLength);
		}
	}
	M_AssertError(ulSourceAddr > Z3_DDR_BASE);

	if (ubOperationSize == 1)
		ubSizeUnit = 0; //1B
	else if (ubOperationSize == 2)
		ubSizeUnit = 1; //2B
	else if (ubOperationSize == 4)
		ubSizeUnit = 2; //4B
	else
		ubSizeUnit = 3; //8B

	while (SEB[SEB_SQ_WR_CNT] < 1);
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 0] = SE0_SV | ubCmdUnit << 6 | ubSizeUnit << 8;
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 1] = ulSourceAddr; //source address
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 2] = ulByteLength; //byte count
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 3] = 0;
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 4] = ulPattern_L; //pattern_L
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 5] = ulPattern_H; //pattern_H
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 6] = 0; //bMSK0_L
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 7] = 0; //bMSK0_H
	SEB[SEB_SQ_WPIU] = 1;
	while (SEB[SEB_CQ_RD_CNT] < 1);
	SEB[SEB_CQ_RPIU] = 1;

	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;


}
void mSE_MSV(UBYTE ubOperationSize, U32 ulSourceAddr, U32 ulByteLength, U32 ulPattern_L, U32 ulPattern_H, U32 ulMask_L, U32 ulMask_H, UBYTE ubEnableMask)
{
#if(!RDT)
	UBYTE ubSizeUnit;
	UBYTE ubCmdUnit = 1;
	UWORD uwSQCnt_SE0 = guwSQCnt_SE0;
	if (ENABLE_DCACHE) {
		if ((ulSourceAddr >= Z2_DDR_BASE) && (ulSourceAddr < Z3_DDR_BASE)) {
			//xthal_dcache_region_invalidate((void *)ulSourceAddr, ulByteLength);
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr, ulByteLength);
		}
	}
	M_AssertError(ulSourceAddr > Z3_DDR_BASE);

	if (ubOperationSize == 1)
		ubSizeUnit = 0; //1B
	else if (ubOperationSize == 2)
		ubSizeUnit = 1; //2B
	else if (ubOperationSize == 4)
		ubSizeUnit = 2; //4B
	else
		ubSizeUnit = 3; //8B

	while (SEB[SEB_SQ_WR_CNT] < 1);
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 0] = SE0_MSV | ubCmdUnit << 6 | ubSizeUnit << 8 | ubEnableMask << 10;
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 1] = ulSourceAddr; //source address
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 2] = ulByteLength; //byte count
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 3] = 0;
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 4] = ulPattern_L; //pattern_L
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 5] = ulPattern_H; //pattern_H
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 6] = ulMask_L; //bMSK0_L
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 7] = ulMask_H; //bMSK0_H
	SEB[SEB_SQ_WPIU] = 1;
	while (SEB[SEB_CQ_RD_CNT] < 1);
	SEB[SEB_CQ_RPIU] = 1;


	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;
#endif
}
void mSE_COPY(U32 ulSourceAddr,  U32 ulTargetAddr, U32 ulByteLength)
{
#if(!RDT)
	if (ENABLE_DCACHE) {
		// Z2 need flush and use original DDR addr
		if (ulSourceAddr >= Z2_DDR_BASE) {
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr, ulByteLength);
		}
		if (ulTargetAddr >= Z2_DDR_BASE) {
			xthal_dcache_region_invalidate((void *)ulTargetAddr, ulByteLength);
		}
	}
	M_AssertError((ulSourceAddr > Z3_DDR_BASE) || (ulTargetAddr > Z3_DDR_BASE));
	while (SEB[SEB_SQ_WR_CNT] < 1);
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 0] = SE0_COPY | 1 << 6; // CmdUnit = 1
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 1] = ulSourceAddr; //source address
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 2] = ulByteLength; //byte count
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 3] = ulTargetAddr; //target address
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 4] = 0; //pattern_L
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 5] = 0; //pattern_H
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 6] = 0; //bMSK0_L
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 7] = 0; //bMSK0_H
	SEB[SEB_SQ_WPIU] = 1;
	while (SEB[SEB_CQ_RD_CNT] < 1);
	SEB[SEB_CQ_RPIU] = 1;
	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;
#endif
}
U32 mSE_Count1(U32 ulSourceAddr, U32 ulByteLength)
{
#if(!RDT)
	UBYTE ubCmdUnit = 1;
	UWORD uwSQCnt_SE0 = guwSQCnt_SE0;
	UWORD uwCQCnt_SE0 = guwCQCnt_SE0;
	U32 ulResult;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ulSourceAddr, ulByteLength);
	}
	M_AssertError(ulSourceAddr > Z3_DDR_BASE);
	while (SEB[SEB_SQ_WR_CNT] < 1);
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 0] = SE0_COneCZero | ubCmdUnit << 6;
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 1] = ulSourceAddr; //source address
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 2] = ulByteLength; //byte count
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 3] = 0;
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 4] = 0; //pattern_L
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 5] = 0; //pattern_H
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 6] = 0; //bMSK0_L
	SEL_SQBaseAddr[uwSQCnt_SE0 * 8 + 7] = 0; //bMSK0_H
	SEB[SEB_SQ_WPIU] = 1;
	while (SEB[SEB_CQ_RD_CNT] < 1);
	SEB[SEB_CQ_RPIU] = 1;
	ulResult = SEL_CQBaseAddr[uwCQCnt_SE0 * 8 + 1];
	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;
	return ulResult;
#endif
#if(RDT)
	return 0;
#endif
}
void mDMAC_SV(UBYTE ubOperationSize,  U32 ulDestinateAddr, U32 ulByteLength, U32 ulLBA, U32 ulRefValue, U32 ulMask, U32 ulMode, UBYTE ubEnableSwitchTask)
{
	UBYTE ubSizeUnit;
	UBYTE ubOPLength = 8;

	if (ubOperationSize == 1) {
		ubSizeUnit = 0; //1B
	}
	else if (ubOperationSize == 2) {
		ubSizeUnit = 1; //2B
	}
	else {
		ubSizeUnit = 2; //4B
	}

	while (DMACB[DMACB_SQ_WR_CNT] < 1) {
		if (ubEnableSwitchTask) {
			M_SwitchTask();
		}
	}
	M_AssertError((U32)&DMACL_SQBaseAddr[guwSQCnt_DMAC * 8] != DCCM0_BANK0_BASE_LONG[1]);
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 0] = DMA_SV | ubSizeUnit << 8 | ulMode | guwSQCnt_DMAC << 16 | ubOPLength << 24; //cmd
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 1] = 0; //source address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 2] = ulDestinateAddr; //target address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 3] = ulRefValue; //set value
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 4] = ulByteLength; //byte count
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 5] = ulMask; //mask bit map
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 6] = ulLBA; //LBA
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 7] = 0; //reserved
	DMACB[DMACB_SQ_WPIU] = 1;
	guwSQCnt_DMAC ++;
	guwSQCnt_DMAC %= MaxSQCnt_DMAC;

	if (ulMode & DMA_EN_SETCQ) {
		guwCQCnt_DMAC ++;
		while (DMACB[DMACB_CQ_RD_CNT] < guwCQCnt_DMAC) {
			if (ubEnableSwitchTask) {
				M_SwitchTask();
			}
		}
		DMACB[DMACB_CQ_RPIU] = guwCQCnt_DMAC;
		guwCQCnt_DMAC = 0;
	}
	if (ENABLE_DCACHE) {
		if ((ulDestinateAddr >= Z2_DDR_BASE) && (ulDestinateAddr < Z3_DDR_BASE)) {
			xthal_dcache_region_invalidate((void *)ulDestinateAddr, ulByteLength);
		}
	}
}
void mDMAC_COPY( U32 ulSourceAddr, U32 ulTargetAddr, U32 ulByteLength, U32 ulLBA, U32 ulMask, UBYTE ubEnableMask, UBYTE ubEnableErrIns, UBYTE ubSetCQ)
{
	UBYTE ubSizeUnit = 0;
	UBYTE ubOPLength = 8;

	while (DMACB[DMACB_SQ_WR_CNT] < 1);
	if (ENABLE_DCACHE) {
		// Z2 need flush and use original DDR addr
		if ((ulSourceAddr >= Z2_DDR_BASE) && (ulSourceAddr < Z3_DDR_BASE)) {
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr, ulByteLength);
		}
		if ((ulTargetAddr >= Z2_DDR_BASE) && (ulTargetAddr < Z3_DDR_BASE)) {
			xthal_dcache_region_invalidate((void *)ulTargetAddr, ulByteLength);
		}
	}
	M_AssertError((U32)&DMACL_SQBaseAddr[guwSQCnt_DMAC * 8] != DCCM0_BANK0_BASE_LONG[1]);
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 0] = DMA_COPY | ubSizeUnit << 8 | ubEnableMask << 11 | ubEnableErrIns << 14  | ubSetCQ << 15 | guwSQCnt_DMAC << 16 | ubOPLength << 24; //cmd
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 1] = ulSourceAddr; //source address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 2] = ulTargetAddr; //target address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 3] = 0; //set value
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 4] = ulByteLength; //byte count
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 5] = ulMask; //mask bit map
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 6] = ulLBA; //LBA
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 7] = 0; //reserved
	DMACB[DMACB_SQ_WPIU] = 1;
	guwSQCnt_DMAC ++;
	guwSQCnt_DMAC %= MaxSQCnt_DMAC;

	if (ubSetCQ) {
		guwCQCnt_DMAC ++;
		while (DMACB[DMACB_CQ_RD_CNT] < guwCQCnt_DMAC);
		DMACB[DMACB_CQ_RPIU] = guwCQCnt_DMAC;
		guwCQCnt_DMAC = 0;
	}
}
void mDMAC_PZIP(U32 ulSourceAddr, UBYTE ubEnableErrIns, UBYTE ubSetCQ)
{
	UBYTE ubSizeUnit = 0;
	UBYTE ubOPLength = 8;

	while (DMACB[DMACB_SQ_WR_CNT] < 1);
	if (ENABLE_DCACHE) {
		if ((ulSourceAddr >= Z2_DDR_BASE) && (ulSourceAddr < Z3_DDR_BASE)) {
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr, 4096);
		}
	}
	M_AssertError((U32)&DMACL_SQBaseAddr[guwSQCnt_DMAC * 8] != DCCM0_BANK0_BASE_LONG[1]);
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 0] = DMA_PZIP | ubSizeUnit << 8 | ubEnableErrIns << 14  | ubSetCQ << 15 | guwSQCnt_DMAC << 16 | ubOPLength << 24; //cmd
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 1] = ulSourceAddr; //source address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 2] = 0; //target address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 3] = 0; //set value
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 4] = 4096; //byte count
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 5] = 0; //mask bit map
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 6] = 0; //LBA
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 7] = 0; //reserved
	DMACB[DMACB_SQ_WPIU] = 1;
	guwSQCnt_DMAC ++;
	guwSQCnt_DMAC %= MaxSQCnt_DMAC;
	if (ubSetCQ) {
		guwCQCnt_DMAC ++;
		while (DMACB[DMACB_CQ_RD_CNT] < guwCQCnt_DMAC);
		DMACB[DMACB_CQ_RPIU] = guwCQCnt_DMAC;
		guwCQCnt_DMAC = 0;
	}
}
void mDMAC_E3D(U32 ulSourceAddr, U32 ulByteLength, U32 ulLBA, UBYTE ubSetCQ)
{
	UBYTE ubSizeUnit = 1;//OPSize2
	UBYTE ubOPLength = 8;

	while (DMACB[DMACB_SQ_WR_CNT] < 1);
	/*if (ENABLE_DCACHE) {//應該根本不用判斷，check E3D一定不在Z2
		if ((ulSourceAddr >= Z2_DDR_BASE) && (ulSourceAddr < Z3_DDR_BASE)) {
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr, ulByteLength);
		}
	}*/
	M_AssertError((U32)&DMACL_SQBaseAddr[guwSQCnt_DMAC * 8] != DCCM0_BANK0_BASE_LONG[1]);
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 0] = DMA_E3D | ubSizeUnit << 8 | ubSetCQ << 15 | guwSQCnt_DMAC << 16 | ubOPLength << 24; //cmd
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 1] = ulSourceAddr; //source address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 2] = 0; //target address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 3] = 0; //set value
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 4] = ulByteLength; //byte count
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 5] = 0; //mask bit map
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 6] = ulLBA; //LBA
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 7] = 0; //reserved
	DMACB[DMACB_SQ_WPIU] = 1;
	guwSQCnt_DMAC++;
	guwSQCnt_DMAC %= MaxSQCnt_DMAC;
	if (ubSetCQ) {
		guwCQCnt_DMAC ++;
		while (DMACB[DMACB_CQ_RD_CNT] < guwCQCnt_DMAC);
		U32 ulCQAddress = DCCM0_BANK0_BASE_LONG[0x28 >> 2];
		gubE3DFailFlag = ((unsigned char*)(ulCQAddress))[0];
		M_AssertError(gubE3DFailFlag);
		DMACB[DMACB_CQ_RPIU] = guwCQCnt_DMAC;
		guwCQCnt_DMAC = 0;
	}
}

void mDMAC_XOR(U32 ulSourceAddr1, U32 ulSourceAddr2, U32 ulDestinateAddr, U32 ulByteLength, UBYTE ubSetCQ, UBYTE ubXNOR)
{
	UBYTE ubOPLength = 8;

	while (DMACB[DMACB_SQ_WR_CNT] < 1);
	if (ENABLE_DCACHE) {
		if ((ulSourceAddr1 >= Z2_DDR_BASE) && (ulSourceAddr1 < Z3_DDR_BASE)) {
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr1, ulByteLength);
		}
		if ((ulSourceAddr2 >= Z2_DDR_BASE) && (ulSourceAddr2 < Z3_DDR_BASE)) {
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr2, ulByteLength);
		}
	}
	M_AssertError((U32)&DMACL_SQBaseAddr[guwSQCnt_DMAC * 8] != DCCM0_BANK0_BASE_LONG[1]);
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 0] = DMA_XOR | (ubXNOR << 10) | ubSetCQ << 15 | guwSQCnt_DMAC << 16 | ubOPLength << 24; //cmd
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 1] = ulSourceAddr1; //source1 address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 2] = ulSourceAddr2; //source2 address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 3] = ulDestinateAddr; //target address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 4] = ulByteLength; //byte count
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 5] = 0; //mask bit map
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 6] = 0; //LBA
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 7] = 0; //reserved
	DMACB[DMACB_SQ_WPIU] = 1;
	guwSQCnt_DMAC++;
	guwSQCnt_DMAC %= MaxSQCnt_DMAC;

	if (ubSetCQ) {
		guwCQCnt_DMAC ++;
		while (DMACB[DMACB_CQ_RD_CNT] < guwCQCnt_DMAC);
		DMACB[DMACB_CQ_RPIU] = guwCQCnt_DMAC;
		guwCQCnt_DMAC = 0;
	}
	if (ENABLE_DCACHE) {
		if ((ulDestinateAddr >= Z2_DDR_BASE) && (ulDestinateAddr < Z3_DDR_BASE)) {
			xthal_dcache_region_invalidate((void *)ulDestinateAddr, ulByteLength);
		}
	}
}

U32 mDMAC_CRC(U32 ulSourceAddr, U32 ulByteLength, U32 ulLBA, UBYTE ubSetCQ , UWORD uwMode)
{
	UBYTE ubSizeUnit = 0;//OPSize2
	UBYTE ubOPLength = 8;
	U32 ulCRCFailFlag = 0;

	while (DMACB[DMACB_SQ_WR_CNT] < 1);
	if (ENABLE_DCACHE) {
		if ((ulSourceAddr >= Z2_DDR_BASE) && (ulSourceAddr < Z3_DDR_BASE)) {
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr, ulByteLength);
		}
	}
	M_AssertError((U32)&DMACL_SQBaseAddr[guwSQCnt_DMAC * 8] != DCCM0_BANK0_BASE_LONG[1]);
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 0] = DMA_CRC | ubSizeUnit << 8 | ubSetCQ << 15 | guwSQCnt_DMAC << 16 | ubOPLength << 24 | uwMode; //cmd
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 1] = ulSourceAddr; //source address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 2] = 0; //target address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 3] = ulLBA; //set value
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 4] = ulByteLength; //byte count
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 5] = 0; //mask bit map
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 6] = 0; //LBA
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 7] = 0; //reserved
	DMACB[DMACB_SQ_WPIU] = 1;
	guwSQCnt_DMAC++;
	guwSQCnt_DMAC %= MaxSQCnt_DMAC;

	if (ubSetCQ) {
		guwCQCnt_DMAC ++;
		while (DMACB[DMACB_CQ_RD_CNT] < guwCQCnt_DMAC);
		U32 ulCQAddress = DCCM0_BANK0_BASE_LONG[0x28 >> 2];
		ulCRCFailFlag = ((U32*)(ulCQAddress))[0];
		//M_AssertError(ubCRCFailFlag);
		if (uwMode & DMA_CRC_CHK) {
			ulCRCFailFlag &= 0xFF;
			if (ulCRCFailFlag) {
				ulCRCFailFlag = RET_OK;
			}
			else {
				ulCRCFailFlag = RET_ERR;
			}
		}
		else {
			ulCQAddress += 4;
			ulCRCFailFlag = ((U32*)(ulCQAddress))[0];
		}
		DMACB[DMACB_CQ_RPIU] = guwCQCnt_DMAC;
		guwCQCnt_DMAC = 0;
	}

	return ulCRCFailFlag;
}


UBYTE mDMAC_CMP(U32 ulSourceAddr, U32 ulSourceAddr2, U32 ulByteLength)
{
	UBYTE ubEnableErrIns = 0;
	UBYTE ubSizeUnit = 0;
	UBYTE ubSQTag = guwSQCnt_DMAC;
	UBYTE ubOPLength = 8;
	UBYTE ubCmpResult = 0;
	UBYTE ubSetCQ = 1;
	if (ubSetCQ && guwCQCnt_DMAC) {
		while ((DMACB[DMACB_CQ_RD_CNT] < guwCQCnt_DMAC)) {};
		DMACB[DMACB_CQ_RPIU] = guwCQCnt_DMAC;
		guwCQCnt_DMAC = 0;
	}

	while (DMACB[DMACB_SQ_WR_CNT] < 1);
	if (ENABLE_DCACHE) {
		if ((ulSourceAddr >= Z2_DDR_BASE) && (ulSourceAddr < Z3_DDR_BASE)) {
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr, ulByteLength);
		}
		if ((ulSourceAddr2 >= Z2_DDR_BASE) && (ulSourceAddr2 < Z3_DDR_BASE)) {
			xthal_dcache_region_writeback_inv((void *)ulSourceAddr2, ulByteLength);
		}
	}

	//step2
	M_AssertError((U32)&DMACL_SQBaseAddr[guwSQCnt_DMAC * 8] != DCCM0_BANK0_BASE_LONG[1]);
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 0] = DMA_CMP | ubSizeUnit << 8 | ubEnableErrIns << 14 | ubSetCQ << 15 | ubSQTag << 16 | ubOPLength << 24; //cmd
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 1] = ulSourceAddr; //source 1 address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 2] = ulSourceAddr2; //source 2 address
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 3] = 0;
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 4] = ulByteLength; //byte count
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 5] = 0; //mask bit map
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 6] = 0; //LBA
	DMACL_SQBaseAddr[guwSQCnt_DMAC * 8 + 7] = 0; //reserved
	DMACB[DMACB_SQ_WPIU] = 1;
	guwSQCnt_DMAC++;
	guwSQCnt_DMAC %= MaxSQCnt_DMAC;

	if (ubSetCQ) {
		guwCQCnt_DMAC ++;
		while (DMACB[DMACB_CQ_RD_CNT] < guwCQCnt_DMAC);
		//U32 ulCQAddress = DCCM0_BANK0_BASE_LONG[0x28 >> 2];
		//ubCmpResult = (((U32*)(ulCQAddress))[0] & 0xFF);
		ubCmpResult = *(UBYTE *)(DCCM0_BANK0_BASE_LONG[0x28 >> 2]);

		DMACB[DMACB_CQ_RPIU] = guwCQCnt_DMAC;
		guwCQCnt_DMAC = 0;
	}

	return ubCmpResult;
}
#if 1
void mForceSramToFlash(U32 ulRAMAddr, U32 ulSizeInByte, U32 ulHeader)
{
#if (!RDT)
#if(!BURNER)
	UBYTE ubEraseStatus = 0x00;
	UBYTE ubProgramStatus = 0x00;
	UWORD uwTotalDumpPlaneNum = ulSizeInByte / 16384; //guwFastPagePlanesPerUnit;//16;//RUTsize  //guwPlanesPerUnit;
	UWORD uwDumpSectorNum = ulSizeInByte / 512;

	if ((ulSizeInByte % 16384) > 0) {
		uwTotalDumpPlaneNum += 1;
	}

	UBYTE ubFrameCnt = 4;
	UBYTE ubL4KNum = ubFrameCnt;
	U32 ulCopiedRegisterAddr = (U32)&gubBuffer2[0];
	U32 ulCopyAddr = (U32)&gubZ2Buffer[0];
	U32 ulSourceAddr = ulRAMAddr ;

	UBYTE ubResetFail = 0;
	UBYTE ubEraseFail = 0;
	UBYTE ubProgramFail = 0;

	UBYTE ubBurstBank = 0;
	UWORD uwFBlock;
	U32 ulFEntry = 0;
	UWORD pFSATarget_W0;

	UBYTE ubChannel = 0;
	UBYTE ubCEIndex = 0;
	UBYTE  ubFreeSet;
	UWORD uwDumpPlaneCount = 0;

	volatile L4KTable16B *pL4KTable = (L4KTable16B*)(L4K_TABLE_ADDR);
	FSA_t * pFSA = (FSA_t *)gulFSABase[ubChannel];

	UBYTE ubi = 0;

	//copy FLH register first anyway, before FW_RESET reset the register
	if (gubAlreadyCopyFLHRegister == 0) {
		memcpy((void *)ulCopiedRegisterAddr, (void *)Flash_Reg_Base, 4096);
		gubAlreadyCopyFLHRegister = 1;
	}

Mark_Begin:

	for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
		gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
		// force empty to avoid program parity clear interrupt vector enable bit bug
		gFREG[FCTLL_MTQ_CFG].L = (FORCE_EMPTY_ALL);
	}


	for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
		gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		if ((gFREG[FCTLL_INT_INF].L & INTR_BUSY)) {
			gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
		}

		// Clear all MTQ in channel
		gFREG[FCTLL_MT_TRIG].B.B1 = 0xFF;
		gFREG[FCTLL_MT_TRIG].B.B1 = 0;

		gFREG[FCTLL_RS_CFG].L &= (~(RS_ON_THE_FLY_EN | RS_PROGRAM_PARITY_EN | RS_ONE_PARITY_PAGE_EN)); // fix 2nd program parity hang on bug
	}

	for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
		gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
		// force empty to avoid program parity clear interrupt vector enable bit bug
		gFREG[FCTLL_MT_TRIG].L |= MTP_RELOAD;
		gFREG[FCTLL_MTQ_CFG].L &= (CLEAR_FORCE_EMPTY_ALL);
	}


	//=================== FW RESET  ====================
	FCONL[FCONL_FW_RESET] = 0;
	FCONL[FCONL_FW_RESET] = 0xFFFFFFFF;

	//=================== RESET EACH CHANNEL ====================
	for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
		ubCEIndex = ubChannel;
		gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));

		//Enable CE
		FCONL[FCONL_FCE_ENB] = 0x00000000;
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]);
		}
		else {
			FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
		}

		//reset
#if MicronFlashOnly || YMTC
		if ((guoFlashUseType.B.btNVDDR2 || guoFlashUseType.B.btNVDDR)) {
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFC;
		}
		else {
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
		}
#elif Hynix
		if (guoFlashUseType.B.btToggle) {
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFC;
		}
		else {
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
		}
#else
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
#endif

		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		// MT preload + FPU trig = Auto clear CE, so it need to set CE again
		//FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE

		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		//Get Status
		gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		gFREG[FCTLL_PIO_CMD].L = 0x70;
		ubEraseStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;

		if (ubEraseStatus != 0xE0) {
			ubResetFail = 1;
		}

	}
	//}
	if (ENABLE_DEBUG_FIP_RESET_UART) {
		if (ubResetFail == 0) {
			UartString("\nDump_R_O");
		}
		else {
			UartString("\nDump_R_X");
		}
	}

Mark_GetFreeUnit:
	if (gubErrorDumpFreeSetGet == 0xFF) {
		ubFreeSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_HEAD | (ENABLE_DUMP_ERROR_SKIP_CHECK_SHALLOW_ERASE ? GET_SYSTEM_SET_SKIP_SHALLOW_ERASE_CHECK : 0)); //not sure the mode is right or not.....
		gubErrorDumpFreeSetGet = ubFreeSet;


		//============================== ERASE EACH BLOCK OF 1 UNIT==========================
		ubChannel = 0;
		ubCEIndex = 0;
		ubBurstBank = 0;
		pFSATarget_W0 = 0;

		UBYTE ubCECount = 0;
		//UBYTE ubPCount = 0;

		for (ubCECount = 0; ubCECount < gubPlanesPerBurst; ubCECount++) {
			ubChannel = (ubCECount & gubPlanesPerBurstMask);
			ubCEIndex = ubCECount;

			//pFSATarget_W0 = (ubPCount << (guoFlashUseType.B.btFourPlane ? 14 : 15));

			gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));

			//set FSA
			//uwFBlock = ubFreeTargetUnit;
			uwFBlock = guwSystemBlockRemappingTable[ubFreeSet][ubCEIndex] >> gubBurstsPerBankLog;
			ubBurstBank = guwSystemBlockRemappingTable[ubFreeSet][ubCEIndex] & gubBurstsPerBankMask;
			pFSATarget_W0 = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15));

			gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
			gFREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(ALU_NORMAL);


			if (ENABLE_iFSA) {
				gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;
				gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
				gFREG[FCTLL_FSA_SEL].B.B0  = 0;
				gFREG[FCTLL_IFSA0].L = ( ((uwFBlock & 0xFFFF) << 16) | pFSATarget_W0 );
			}
			else {
				pFSA[gubFSAIndex[ubChannel]].uword.W0 = pFSATarget_W0;
				pFSA[gubFSAIndex[ubChannel]].uword.W1 = uwFBlock & 0xFFFF;
				pFSA[gubFSAIndex[ubChannel]].B.B4 = (uwFBlock & 0xFF0000) >> 16;
				gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
				gFREG[FCTLL_FSA_SEL].L  = gubFSAIndex[ubChannel];
				//gubFSAIndex[ubChannel] = (gubFSAIndex[ubChannel] + 1) & 63;

			}

			//gFREG[FCTLL_INT_INF].B.B0 |= FREE_INTERRUPT;

			FCONL[FCONL_FCE_ENB] = 0x00000000;
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]);
			}
			else {
				FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
			}

			//Erase
#if MicronFlashOnly
			M_AssertError(!ENABLE_MICRON_SLC);
#if (Force_MLC_Erase||ENABLE_DUMP_ERROR_SKIP_CHECK_SHALLOW_ERASE)
			//switch to MLC
			gubMicronMode[ubCEIndex] = MICRON_MLC;
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CDF;
#else
			//switch to SLC
			gubMicronMode[ubCEIndex] = MICRON_SLC;
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CDA;
#endif
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C60_A3_CD0;
#else
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_CA2_C60_A3_CD0; //0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0
#endif
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

			//Polling
			gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40; // 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0xF0,
			gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

			//Get Status
			gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			gFREG[FCTLL_PIO_CMD].L = 0x70;
			ubEraseStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;

			if (ubEraseStatus != 0xE0) {
				ubEraseFail = 1;
			}

		}

		if (ENABLE_DEBUG_FIP_RESET_UART) {
			if (ubEraseFail == 0) {
				UartString("\nDump_E_O");
			}
			else {
				UartString("\nDump_E_X");
			}
		}
	}
	else {
		ubFreeSet = gubErrorDumpFreeSetGet;
	}

	//============================== PROGRAM ==========================
	//copy source to buffer and then write from buffer to flash, if source is not in DRAM

	UWORD uwPageCount = 0;

	if (ubEraseStatus == 0xE0) {
		for (uwPageCount = (guwErrorDumpLastPlaneWrite / gubPlanesPerBurst) * gubPlanesPerPage; uwPageCount < guwFastPagePlanesPerUnit; uwPageCount += gubPlanesPerPage) {
			if (uwDumpPlaneCount >= uwTotalDumpPlaneNum) {
				break;
			}
			for (ubCEIndex = (guwErrorDumpLastPlaneWrite & (gubPlanesPerBurst - 1)); ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				if (uwDumpPlaneCount >= uwTotalDumpPlaneNum) {
					break;
				}

				if ((ulRAMAddr + (uwDumpPlaneCount * 16384)) < DDR_BASE) {
					memcpy((void *)ulCopyAddr, (void *)(ulRAMAddr + (uwDumpPlaneCount * 16384)), 16384);
					ulSourceAddr = ulCopyAddr;
				}
				else {
					ulSourceAddr = ulRAMAddr + (uwDumpPlaneCount * 16384);
				}

				ulFEntry = ((U32)uwPageCount + (ubCEIndex * gubBurstsPerBank)) * ubFrameCnt;
				ubChannel = (ubCEIndex & gubPlanesPerBurstMask);
				uwFBlock = guwSystemBlockRemappingTable[ubFreeSet][ubCEIndex] >> gubBurstsPerBankLog;

				gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));

				if (ENABLE_iFSA) {
					gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;
					gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
					gFREG[FCTLL_FSA_SEL].B.B0  = 0;

					gFREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;//new add
					if (guoFlashUseType.B.btA2Cmd) {
						gFREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(ALU_A2);//new add
					}
					else {
						gFREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(ALU_NORMAL);//new add
					}
					ubBurstBank = guwSystemBlockRemappingTable[ubFreeSet][ubCEIndex] & gubBurstsPerBankMask;
					pFSATarget_W0 = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.btIntelMicronB0KB ? 2 : (guoFlashUseType.B.bt1024Page ? 1 : 0))) + (ulFEntry & gub4kEntrysPerPlaneMask));
					gFREG[FCTLL_IFSA0].L = ( ((uwFBlock & 0xFFFF) << 16) | pFSATarget_W0 );
				}
				else {
					pFSA[gubFSAIndex[ubChannel]].uword.W0 = pFSATarget_W0;
					pFSA[gubFSAIndex[ubChannel]].uword.W1 = uwFBlock & 0xFFFF;
					pFSA[gubFSAIndex[ubChannel]].B.B4 = (uwFBlock & 0xFF0000) >> 16;
					gFREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
					gFREG[FCTLL_FSA_SEL].L  = gubFSAIndex[ubChannel];
					//gubFSAIndex[ubChannel] = (gubFSAIndex[ubChannel] + 1) & 63;
				}
#if ENABLE_RUNTIME_SEEDINIT
				// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
				gFREG[FCTLL_SEED_INIT].L = M_RandomSeed((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)), ((guoFlashUseType.B.btHynix3DV6) ? 512 : 384));
#elif TSB_BICS4_SUPPORT
				gFREG[FCTLL_SEED_INIT].L = flaGenRandSeed(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#else
				if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 512) {
					gFREG[FCTLL_SEED_INIT].L = 0x0;
				}
				else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1024) {
					gFREG[FCTLL_SEED_INIT].L = 0xFFFFFF;
				}
#if TLC_BICS2
#else /* TLC_BICS2 */
				else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1536) {
					gFREG[FCTLL_SEED_INIT].L = 0x555555;
				}
#if B0KB
#else
				else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2048) {
					gFREG[FCTLL_SEED_INIT].L = 0xAAAAAA;
				}
				else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2560) {
					gFREG[FCTLL_SEED_INIT].L = 0xCCCCCC;
				}
				else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3072) {
					gFREG[FCTLL_SEED_INIT].L = 0x333333;
				}
				else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3584) {
					gFREG[FCTLL_SEED_INIT].L = 0x999999;
				}
				else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 4096) {
					gFREG[FCTLL_SEED_INIT].L = 0x666666;
				}
#endif
#endif /* TLC_BICS2 */
#endif

				// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#endif /* ENABLE_RUNTIME_SEEDINIT */
#if TLC
				//Enable REF_SEED_FROM_OTHER
				if (TLC) {
					gFREG[FCTLL_OTHER_SET].L &= REF_SEED_VALUE(0);
#if TSB_BICS4_SUPPORT || (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
					gFREG[FCTLL_OTHER_SET].L |= REF_SEED_VALUE(M_RefSeedPage(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));
#else
					gFREG[FCTLL_OTHER_SET].L |= REF_SEED_VALUE((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));
#endif
					gFREG[FCTLL_OTHER_SET].L |= (REF_SEED_FROM_OTHER);
				}
#endif
				//Enable CE
				FCONL[FCONL_FCE_ENB] = 0x00000000;
#if(UFS)
				if (1)
#else
				if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
				{
					FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]);
				}
				else {
					FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE
				}

				//Set Source Info
				gFREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);
				gFREG[FCTLL_DMA_CFG].L &= TRANSFER_0_ECC_FRAME;
				gFREG[FCTLL_DMA_CFG].B.B1 |= ubFrameCnt;

				//gFREG[FCTLL_DMA_ADR_0].L = ulSourceAddr+((uwDumpPlaneCount*ubFrameCnt)*4096);//ulSourceAddr
				gFREG[FCTLL_DMA_ADR_0].L = ulSourceAddr;

				gFREG[FCTLL_ZIP_CFG].L |= L4K_BADR_CONTINUOUS_MODE;
				gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
				gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
				gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(4);//bypass zip need to set L4KNUM as 4(fixed,full lengh)

				gFREG[FCTLL_CHNL_SET].L &= (~BYPASS_CONV);////////////////////

				if (ENABLE_DCACHE) {
					xthal_dcache_region_writeback_inv((void *)ulSourceAddr, ubL4KNum * 4096);
				}

				//Set L4K Table
				gFREG[FCTLL_L4K_SPR_ADR].L = L4K_TABLE_ADDR + (L4kQ_BIND_FACTOR * Spare4kSize);
				pL4KTable = (L4KTable16B*)(L4K_TABLE_ADDR + (L4kQ_BIND_FACTOR * Spare4kSize));

				UBYTE ubL4KIndex;

				for (ubL4KIndex = 0; ubL4KIndex < ubL4KNum; ubL4KIndex++) {
					if (ulRAMAddr + (((uwDumpPlaneCount * ubL4KNum) + ubL4KIndex) * 4096) == Flash_Reg_Base) {
						memcpy((void *)(ulSourceAddr + (ubL4KIndex * 4096)), (void *)(ulCopiedRegisterAddr), 4096);
						if (ENABLE_DCACHE) {
							xthal_dcache_region_writeback_inv((void *)(ulSourceAddr + (ubL4KIndex * 4096)), 4096);
						}
					}

					if (ubL4KIndex == 0) {
						pL4KTable->ulL4K_LCA = MARK_ErrorDumpUnit | uwDumpPlaneCount;
					}
					else {
						pL4KTable->ulL4K_LCA = uwTotalDumpPlaneNum;
					}
					pL4KTable->Para0x04.BitMap.ulL4K_FW =  ulHeader & 0xFFFFFF;//low 3 byte

					if (uwDumpSectorNum <= 0) {
						pL4KTable->Para0x04.BitMap.ubL4K_SPRV = 0x00;
					}
					else if (uwDumpSectorNum < 8) {
						pL4KTable->Para0x04.BitMap.ubL4K_SPRV = 0x00;
						for (ubi = 0; ubi < uwDumpSectorNum; ubi++) {
							pL4KTable->Para0x04.BitMap.ubL4K_SPRV |= (BIT0 << ubi);
						}
						uwDumpSectorNum -= uwDumpSectorNum;
					}
					else {
						pL4KTable->Para0x04.BitMap.ubL4K_SPRV = 0xFF;
						uwDumpSectorNum -= 8;
					}
					pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
					pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
					pL4KTable->Para0x08.BitMap.ubL4K_FW2 = (ulHeader >> 24) & 0xFF; //high 1 byte
					pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (ulSourceAddr + ubL4KIndex * 0x1000) / 512;
					pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0x07;
					pL4KTable++;

				}

				//Write
#if MicronFlashOnly
				M_AssertError(!ENABLE_MICRON_SLC);
				// switch to SLC mode for SLC programming
				gubMicronMode[ubCEIndex] = MICRON_SLC;
				gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CDA;
				gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);
				gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C80_A5_DW_C10;
#else
				gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_CA2_C80_A5_DW_C10;//FPU_PTR_AG_C80_A5_DW_C10;
#endif
				gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {}

				//Polling
				gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40;
				gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
				while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0) {}

				//Get Status
				gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
				gFREG[FCTLL_PIO_CMD].L = 0x70;
				ubProgramStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;

				if (ubProgramStatus != 0xE0) {
					ubProgramFail = 1;
				}

				guwErrorDumpLastPlaneWrite++;
				uwDumpPlaneCount++;
			}
		}

		if (uwDumpPlaneCount < uwTotalDumpPlaneNum) {
			gubErrorDumpFreeSetGet = 0xFF;
			guwErrorDumpLastPlaneWrite = 0;
			goto Mark_GetFreeUnit;
		}

		if (ENABLE_DEBUG_FIP_RESET_UART) {
			if (ubProgramFail == 0) {
				UartString("\nDump_P_O");
			}
			else {
				UartString("\nDump_P_X");
			}
		}
	}
#endif
#endif
}
#endif
#if 0
void mForceSramToFlash(U32 ulRAMAddr, U32 ulSizeInByte, U32 ulHeader)
{
#if (!RDT)
#if(!BURNER)
	UBYTE ubEraseStatus = 0;
	UBYTE ubProgramStatus = 0;
	UBYTE ubResetFail = 0;
	UBYTE ubEraseFail = 0;
	UBYTE ubProgramFail = 0;
	UBYTE ubFrameCnt = 4;
	UBYTE ubL4KNum = ubFrameCnt;
	UBYTE ubChannel = 0;
	UBYTE ubCEIndex = 0;
	UWORD uwFEntry = 0;
	UWORD uwDumpPlaneCount = 0;
	UWORD uwTotalDumpPlaneNum = ulSizeInByte / 16384; //guwFastPagePlanesPerUnit;//16;//RUTsize  //guwPlanesPerUnit;
	if ((ulSizeInByte % 16384) > 0) {
		uwTotalDumpPlaneNum += 1;
	}
	U32 ulCopiedRegisterAddr = (U32)&gubBuffer2[0];
	U32 ulCopyAddr = (U32)&gubZ2Buffer[0];
	U32 ulSourceAddr;

	//copy FLH register first anyway, before FW_RESET reset the register
	if (gubAlreadyCopyFLHRegister == 0) {
		memcpy((void *)ulCopiedRegisterAddr, (void *)Flash_Reg_Base, 4096);
		gubAlreadyCopyFLHRegister = 1;
	}

Mark_Begin:
	for (ubChannel = 0; ubChannel < gubBurstsPerBank; ubChannel++) {
		gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));
		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		gFREG[FCTLL_INT_INF].L &= FREE_INTERRUPT;
	}
	//=================== FW RESET  ====================
	FCONL[FCONL_FW_RESET] = 0;
	FCONL[FCONL_FW_RESET] = 0xFFFFFFFF;
	//=================== RESET EACH CHANNEL ====================
	for (ubChannel = 0; ubChannel < gubBurstsPerBank; ubChannel++) {
		ubCEIndex = ubChannel;
		gFREG  = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));

		//Enable CE
		FCONL[FCONL_FCE_ENB] = 0x00000000;
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE

		//reset
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CFF;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		// MT preload + FPU trig = Auto clear CE, so it need to set CE again
		//FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst); // Enable selected CE

		//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
		gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
		gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK20;
		gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		//Get Status
		gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
		gFREG[FCTLL_PIO_CMD].L = 0x70;
		ubEraseStatus = (UBYTE)gFREG[FCTLL_PIO_DAT].L;

		if (ubEraseStatus != 0xE0) {
			ubResetFail = 1;
		}

	}
	if (ubResetFail == 0) {
		UartString("\nDump_R_O");
	}
	else {
		UartString("\nDump_R_X");
	}

Mark_GetFreeUnit:
	if (gubErrorDumpFreeUnitGet == 0xFF) {
		gubErrorDumpFreeUnitGet = ftlGetSystemBlockFreeUnit(0);//not sure the mode is right or not.....
		ubEraseFail = FPU_A2_Erase(0, gubErrorDumpFreeUnitGet);
		if (ubEraseFail == 0) {
			UartString("\nDump_E_O");
		}
		else {
			UartString("\nDump_E_X");
		}
	}

	UWORD uwPageCount = 0;
Mark_Program:
	//============================== PROGRAM ==========================
	//copy source to buffer and then write from buffer to flash, if source is not in DRAM

	if ((ubEraseFail == 0) && (ubResetFail == 0)) {
		for (uwPageCount = (guwErrorDumpLastPlaneWrite / MAX_CHANNEL) * gubPlanesPerPage; uwPageCount < guwFastPagePlanesPerUnit; uwPageCount += gubPlanesPerPage) {
			if (uwDumpPlaneCount >= uwTotalDumpPlaneNum) {
				break;
			}
			ubCEIndex = (guwErrorDumpLastPlaneWrite & (MAX_CHANNEL - 1));

			if ((ulRAMAddr + (uwDumpPlaneCount * 16384)) < DDR_BASE) {
				memcpy((void *)ulCopyAddr, (void *)(ulRAMAddr + (uwDumpPlaneCount * 16384)), 16384);
				ulSourceAddr = ulCopyAddr;
			}
			else {
				ulSourceAddr = ulRAMAddr + (uwDumpPlaneCount * 16384);
			}
			uwFEntry = (uwPageCount + (ubCEIndex * gubBurstsPerBank)) * ubFrameCnt;


			if ((ulRAMAddr + (uwDumpPlaneCount * ubL4KNum * 4096)) == Flash_Reg_Base) {
				memcpy((void *)(ulSourceAddr), (void *)(ulCopiedRegisterAddr), 4096);
				if (ENABLE_DCACHE) {
					xthal_dcache_region_writeback_inv((void *)ulSourceAddr, 4096);
				}
			}

			ubProgramStatus = FPU_A2_Program(0, ubCEIndex, gubErrorDumpFreeUnitGet, ulFEntry, ulSourceAddr, MARK_ErrorDumpUnit | uwDumpPlaneCount, uwTotalDumpPlaneNum, ulHeader);
			if (ubProgramStatus) {
				ubProgramFail = 1;
			}
			guwErrorDumpLastPlaneWrite++;
			uwDumpPlaneCount++;
		}

		if (uwDumpPlaneCount < uwTotalDumpPlaneNum) {
			gubErrorDumpFreeUnitGet = 0xFF;
			guwErrorDumpLastPlaneWrite = 0;
			goto Mark_GetFreeUnit;
		}

		if (ubProgramFail == 0) {
			UartString("\nDump_P_O");
		}
		else {
			UartString("\nDump_P_X");
		}
	}
#endif
#endif
}
#endif
UBYTE FPU_A2_Program(UBYTE ubUseRUT, UBYTE ubCE, UWORD uwFUnit, U32 ulFEntry, U32 ulBaseAddr, U32 ulLCA1, U32 ulLCA2, U32 ulFW)
{
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);
#if (!RDT)
#if(!BURNER)
	UBYTE ubChannel = (ubCE & gubPlanesPerBurstMask);
	UBYTE ubProgramStatus;
	UBYTE ubFrameCnt = gub4kEntrysPerPlane;
	UBYTE ubL4KNum = ubFrameCnt;
	UBYTE ubBurstBank, ubPhyPlane;
	UBYTE ubL4KIndex;
	UWORD uwFBlock;
	UWORD pFSATarget_W0;
	volatile L4KTable16B *pL4KTable = (L4KTable16B*)(L4K_TABLE_ADDR);
	FSA_t * pFSA = (FSA_t *)gulFSABase[0];
	volatile REG_t *FREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));

	//SetFSA
	if (ENABLE_PLANE_CH_CE) {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerPlaneLog)&gubBurstsPerBankMask;
	}
	else {
		ubBurstBank = (ulFEntry >> gub4kEntrysPerBurstLog)&gubBurstsPerBankMask;
	}
	if (ubUseRUT == 0) {
		uwFBlock = uwFUnit;
		ubPhyPlane = ubBurstBank;
	}
	else {
		uwFBlock = M_RUT2DTo1D(ubCE , (uwFUnit << gubBurstsPerBankLog) + ubBurstBank);
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
	M_Check_PseudoCE(ubCE, uwFBlock);
	pFSATarget_W0 = (ubBurstBank << (guoFlashUseType.B.btFourPlane ? 14 : 15))  | ((((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) << gubSectorsPerPlaneLog) >> (guoFlashUseType.B.btIntelMicronB0KB ? 2 : (guoFlashUseType.B.bt1024Page ? 1 : 0))) + (ulFEntry & gub4kEntrysPerPlaneMask));
	if (ENABLE_DEBUG_UART) {
		UartString("\nFPU_P ");
		Uart_Tx_DataHex(ubChannel);
		UartString(" ");
		Uart_Tx_DataHex(ubCE);
		UartString(" ");
		UartWordHex(uwFUnit);
		UartString(" ");
		UartWordHex(ulFEntry);
		UartString(" ");
		Uart_Tx_DataHex(ubBurstBank);
	}
	if (ENABLE_iFSA) {
		FREG[FCTLL_FSA_SEL].L |= IFSA_EN;
		FREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
		FREG[FCTLL_FSA_SEL].B.B0  = 0;
		FREG[FCTLL_IFSA0].L = ( ((uwFBlock & 0xFFFF) << 16) | pFSATarget_W0 );
	}
	else {
		pFSA[gubFSAIndex[ubChannel]].uword.W0 = pFSATarget_W0;
		pFSA[gubFSAIndex[ubChannel]].uword.W1 = uwFBlock & 0xFFFF;
		pFSA[gubFSAIndex[ubChannel]].B.B4 = (uwFBlock & 0xFF0000) >> 16;
		FREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
		FREG[FCTLL_FSA_SEL].L  = gubFSAIndex[ubChannel];
		//gubFSAIndex[ubChannel] = (gubFSAIndex[ubChannel] + 1) & 63;
	}
#if ENABLE_RUNTIME_SEEDINIT
	// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
	gFREG[FCTLL_SEED_INIT].L = M_RandomSeed((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)), ((guoFlashUseType.B.btHynix3DV6) ? 512 : 384));
#elif TSB_BICS4_SUPPORT
	FREG[FCTLL_SEED_INIT].L = flaGenRandSeed(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div));
#else
	if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 512) {
		FREG[FCTLL_SEED_INIT].L = 0x0;
	}
	else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1024) {
		FREG[FCTLL_SEED_INIT].L = 0xFFFFFF;
	}
#if TLC_BICS2
#else /* TLC_BICS2 */
	else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 1536) {
		FREG[FCTLL_SEED_INIT].L = 0x555555;
	}
#if B0KB
#else
	else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2048) {
		FREG[FCTLL_SEED_INIT].L = 0xAAAAAA;
	}
	else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 2560) {
		FREG[FCTLL_SEED_INIT].L = 0xCCCCCC;
	}
	else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3072) {
		FREG[FCTLL_SEED_INIT].L = 0x333333;
	}
	else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 3584) {
		FREG[FCTLL_SEED_INIT].L = 0x999999;
	}
	else if ((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)) < 4096) {
		FREG[FCTLL_SEED_INIT].L = 0x666666;
	}
#endif
#endif /* TLC_BICS2 */
#endif

	// 201607 SEED_INIT: randomize block seed----------------------------------------------------------
#endif /* ENABLE_RUNTIME_SEEDINIT */
#if TLC
	//Enable REF_SEED_FROM_OTHER
	if (TLC) {
		FREG[FCTLL_OTHER_SET].L &= REF_SEED_VALUE(0);
#if TSB_BICS4_SUPPORT || (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
		FREG[FCTLL_OTHER_SET].L |= REF_SEED_VALUE(M_RefSeedPage(M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));
#else
		FREG[FCTLL_OTHER_SET].L |= REF_SEED_VALUE((M_Divider(ulFEntry, DEF_4KEntryPerPage_Mcl_Div)));
#endif
		FREG[FCTLL_OTHER_SET].L |= (REF_SEED_FROM_OTHER);
	}
#endif
	UBYTE ubTrueCE = ubCE;
	M_VirtualToPhysicalCE(ubTrueCE);
	//Enable CE
	FCONL[FCONL_FCE_ENB] = 0x00000000;
#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][(ubTrueCE / gubPlanesPerBurst)]);
	}
	else {
		FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubTrueCE / gubPlanesPerBurst); // Enable selected CE
	}

	//Set Source Info
	FREG[FCTLL_DMA_CFG].L &= (~FLH_BUF_MODE);
	FREG[FCTLL_DMA_CFG].L &= TRANSFER_0_ECC_FRAME;
	FREG[FCTLL_DMA_CFG].B.B1 |= ubFrameCnt;
	FREG[FCTLL_DMA_ADR_0].L = ulBaseAddr;
	FREG[FCTLL_ZIP_CFG].L |= L4K_BADR_CONTINUOUS_MODE;
	FREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
	FREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	FREG[FCTLL_ZIP_CFG].L |= L4K_NUM(ubL4KNum);//bypass zip need to set L4KNUM as 4(fixed,full lengh)
	FREG[FCTLL_CHNL_SET].L &= (~BYPASS_CONV);////////////////////

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ulBaseAddr, ubL4KNum * 4096);
	}

	//Set L4K Table
	FREG[FCTLL_L4K_SPR_ADR].L = L4K_TABLE_ADDR + (L4kQ_BIND_FACTOR * Spare4kSize);
	pL4KTable = (L4KTable16B*)(L4K_TABLE_ADDR + (L4kQ_BIND_FACTOR * Spare4kSize));

	for (ubL4KIndex = 0; ubL4KIndex < ubL4KNum; ubL4KIndex++) {
		if ((ubL4KIndex == 0) || (ulLCA2 == 0)) {
			pL4KTable->ulL4K_LCA = ulLCA1;
		}
		else if (ulLCA2 != 0) {
			pL4KTable->ulL4K_LCA = ulLCA2;
		}
		pL4KTable->Para0x04.BitMap.ulL4K_FW =  ulFW & 0xFFFFFF;//low 3 byte
		pL4KTable->Para0x04.BitMap.ubL4K_SPRV = 0xFF;
		pL4KTable->Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
		pL4KTable->Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable->Para0x08.BitMap.ubL4K_FW2 = (ulFW >> 24) & 0xFF; //high 1 byte
		pL4KTable->Para0x0C.BitMap.ulL4K_BADR = (ulBaseAddr + ubL4KIndex * 0x1000) / 512;
		pL4KTable->Para0x0C.BitMap.ubL4K_ZCODE = 0x07;
		pL4KTable++;
	}

	//Write
	FREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;//new add
	if (guoFlashUseType.B.btA2Cmd) {
		FREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(ALU_A2);//new add
#if MicronFlashOnly
		M_AssertError(!ENABLE_MICRON_SLC);
		gubMicronMode[ubCE] = MICRON_SLC;
		//switch to SLC
		FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CDA;
		FREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
		while ((FREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

		FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C80_A5_DW_C10;
#else
		FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_CA2_C80_A5_DW_C10;//FPU_PTR_AG_C80_A5_DW_C10;
#endif
	}
	else {
		FREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(ALU_NORMAL);//new add
		FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C80_A5_DW_C10;//FPU_PTR_AG_C80_A5_DW_C10;
	}
	FREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while (FREG[FCTLL_FPU_TRIG].L != SRQ_AVL) {}

	//Polling
	FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40;
	FREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while (FREG[FCTLL_FPU_TRIG].L != SRQ_AVL) {}

	//Get Status
	FREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
	FREG[FCTLL_PIO_CMD].L = 0x70;
	ubProgramStatus = (UBYTE)FREG[FCTLL_PIO_DAT].L;

	if (ubProgramStatus != 0xE0) {
		ubProgramStatus = 1;
	}
	else {
		ubProgramStatus = 0;
	}

	FCONL[FCONL_FCE_ENB] = 0x00000000;
	return ubProgramStatus;
#endif
#endif
}

UBYTE FPU_A2_Erase(UBYTE ubUseRUT, UWORD uwFUnit)
{
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	M_CheckRDTModeToDisableFunctionReturnValid(0);
#if (!RDT)
#if(!BURNER)
	UBYTE ubEraseFail = 0;
	//UBYTE ubPCount = 0;
	UBYTE ubBurstBank, ubPhyPlane;
	UBYTE ubEraseStatus;
	UBYTE ubChannel;
	UBYTE ubCEIndex;
	UBYTE ubCENum = ubUseRUT ? gubCENumber : MAX_CHANNEL;
	UBYTE BurstsPerBankNum =  ubUseRUT ? gubBurstsPerBank : 1;
	UWORD uwFBlock;
	UWORD pFSATarget_W0;
	FSA_t * pFSA = (FSA_t *)gulFSABase[0];
	volatile REG_t *FREG;

	if (ENABLE_DEBUG_UART) {
		UartString("\nFPU_E");
		UartWordHex(uwFUnit);
	}

	for (ubCEIndex = 0; ubCEIndex < ubCENum; ubCEIndex++) {
		ubChannel = (ubCEIndex & gubPlanesPerBurstMask);
		FREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
		for (ubBurstBank = 0; ubBurstBank < BurstsPerBankNum; ubBurstBank ++) {
			//set FSA
			if (ubUseRUT == 0) {
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
			pFSATarget_W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15));
			if (ENABLE_iFSA) {
				FREG[FCTLL_FSA_SEL].L |= IFSA_EN;
				FREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
				FREG[FCTLL_FSA_SEL].B.B0  = 0;
				FREG[FCTLL_IFSA0].L = ( ((uwFBlock & 0xFFFF) << 16) | pFSATarget_W0 );
			}
			else {
				pFSA[gubFSAIndex[ubChannel]].uword.W0 = pFSATarget_W0;
				pFSA[gubFSAIndex[ubChannel]].uword.W1 = uwFBlock & 0xFFFF;
				pFSA[gubFSAIndex[ubChannel]].B.B4 = (uwFBlock & 0xFF0000) >> 16;
				FREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
				FREG[FCTLL_FSA_SEL].L  = gubFSAIndex[ubChannel];
			}

			//gFREG[FCTLL_INT_INF].B.B0 |= FREE_INTERRUPT;
			UBYTE ubTrueCE = ubCEIndex;
			M_VirtualToPhysicalCE(ubTrueCE);
			// Select CE
			FCONL[FCONL_FCE_ENB] = 0x00000000;
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][(ubTrueCE / gubPlanesPerBurst)]);
			}
			else {
				FCONL[FCONL_FCE_SET] = (ubChannel * 8) + (ubTrueCE / gubPlanesPerBurst); // Enable selected CE
			}

			//Erase
			FREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
			if (guoFlashUseType.B.btA2Cmd) {
				FREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(ALU_A2);//new add
#if MicronFlashOnly
#if Force_MLC_Erase
				//switch to MLC
				gubMicronMode[ubCEIndex] = MICRON_MLC;
				FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CDF;
#else
				//switch to SLC
				gubMicronMode[ubCEIndex] = MICRON_SLC;
				FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_CDA;
#endif
				FREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
				while ((FREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

				FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C60_A3_CD0;
#else
				FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_CA2_C60_A3_CD0; //0xA2 0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0
#endif
			}
			else {
				FREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(ALU_NORMAL);//new add
				FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C60_A3_CD0; //0x60, 0x20, 0x00, 0x33, 0xD0, 0x20, 0x00, 0xF0
			}
			FREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while (FREG[FCTLL_FPU_TRIG].L != SRQ_AVL);

			//Polling
			FREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40; // 0x70, 0x20, 0x18, 0x10, 0x40, 0x51, 0x00, 0xF0,
			FREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
			while (FREG[FCTLL_FPU_TRIG].L != SRQ_AVL);

			//Get Status
			FREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			FREG[FCTLL_PIO_CMD].L = 0x70;
			ubEraseStatus = (UBYTE)FREG[FCTLL_PIO_DAT].L;

			if (ubEraseStatus != 0xE0) {
				ubEraseFail = 1;
			}
		}
	}
	FCONL[FCONL_FCE_ENB] = 0x00000000;
	return ubEraseFail;
#endif
#endif
}

U32 ftlEraseTarget(FTarget_t *PhyTarget, UBYTE ubDSelect, UBYTE ubDoSwapRUT)
{
	M_CheckRDTModeToDisableFunctionReturnValid(0);
#if(!RDT)
	UBYTE ubCEIndex;
	gubFQLinkIndex[0] = 0xFF;

	UBYTE ubContinueFailCnt = 0, ubBurstBank, ubBurstShift, ubBurstShift1, ubEraseTypeUnit = 0;
	U32 ulEraseUnitFailCEMap = 0;
	FlashQueue_t *FQ;
	if (gubRUTMixPlaneEnable == 0) {
		ubBurstShift1 = gubBurstsPerBank;
		ubBurstShift = gubBurstsPerBank;
	}
	else {
		ubBurstShift1 = 1;
	}

#if Hynix3DV6
	UBYTE ubTLCTreatment = (ubDSelect & UnitTLCTreatment) ? 1 : 0;
	ubDSelect &= ~UnitTLCTreatment;
#endif

MARK_ReDoFtlEraseTarget:

#if TLC
	while (gubNum_D1_Part > 3);
#endif
	for (ubCEIndex = 0; ubCEIndex < gubCENumber; ubCEIndex++) {
		UBYTE ubi;
		UBYTE ubUnitCount = (ubDSelect) ? 1 : gubNum_D1_Part;
		for (ubi = 0; ubi < ubUnitCount ; ubi++) {
			if (gubRUTMixPlaneEnable) {
				UBYTE ubPhyBurst = 0;
				UWORD uwFBlock;
				ubBurstShift = gubBurstsPerBank;
				ubEraseTypeUnit = 1;
				for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; (ubBurstBank ++)) {
					if ((TLC == 0) || ubDSelect) {
						uwFBlock = M_RUT2DTo1D(ubCEIndex , (PhyTarget->B.uwTarget << gubBurstsPerBankLog) + ubBurstBank);

					}
					else {
						uwFBlock = M_RUT2DTo1D(ubCEIndex, ((guwFWTotalUnitNum + (PhyTarget->B.uwTarget * D1_UNIT_NUM_PER_LINK) + ubi) << gubBurstsPerBankLog) + ubBurstBank);
					}
					if (ubBurstBank) {
						if (ubPhyBurst & (BIT0 << (uwFBlock & gubBurstsPerBankMask))) {
							ubEraseTypeUnit = 0;
							ubBurstShift = 1;
							break;
						}
						else {
							ubPhyBurst |= (BIT0 << (uwFBlock & gubBurstsPerBankMask));
						}
					}
					else {
						ubPhyBurst |= (BIT0 << (uwFBlock & gubBurstsPerBankMask));
					}
					if (ubBurstBank == (gubBurstsPerBank - 1)) {
						UBYTE ubBurstTemp;
						for (ubBurstTemp = 1; ubBurstTemp < gubBurstsPerBank; ubBurstTemp++) {
							gubFQLinkIndex[((ubCEIndex * ubUnitCount) + ubi) * gubBurstsPerBank + ubBurstTemp] = 0xFF;
						}
					}
				}
			}
			UBYTE ubDepth, ubL4kLink;
			//M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; (ubBurstBank = ubBurstBank + ubBurstShift)) {


				while ((gFQI.ubFQLinkNumber >= gFQ_NUM) || (gMTDepth.ubLinkNum == 0)) {
					if (ENABLE_RUT_SWAP) {
						UBYTE ubCEIndexCheck;
						UBYTE ubiCheck;
						UBYTE ubBurstBankcheck;
						for (ubCEIndexCheck = 0; ubCEIndexCheck <= ubCEIndex; ubCEIndexCheck++) {        // 檢查之前的CE
							for (ubiCheck = 0; ubiCheck < ((ubCEIndexCheck == ubCEIndex) ? ubi : ubUnitCount); ubiCheck++) {
								for (ubBurstBankcheck = 0; ubBurstBankcheck < (((ubCEIndexCheck == ubCEIndex) && (ubiCheck == ubi)) ? ubBurstBank : gubBurstsPerBank); (ubBurstBankcheck = ubBurstBankcheck + ubBurstShift1)) {
									if (gubFQLinkIndex[((ubCEIndexCheck * ubUnitCount) + ubiCheck) * gubBurstsPerBank + ubBurstBankcheck] != 0xFF) {   // 還沒做完的才需要檢查
										FQ = &gFQI.gFQLink[gubFQLinkIndex[((ubCEIndexCheck * ubUnitCount) + ubiCheck) * gubBurstsPerBank + ubBurstBankcheck]];
										if (FQ->ubFQPhase == BYTE_FQ_WAITCHECK) {
											FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
											gubFQLinkIndex[((ubCEIndexCheck * ubUnitCount) + ubiCheck) * gubBurstsPerBank + ubBurstBankcheck] = 0xFF;
											if (FQ->btPEFail) {
												ulEraseUnitFailCEMap |= (BIT0 << ubCEIndexCheck);
											}
										}
									}
								}
							}
						}
					}
					M_SwitchTask();
				}
				FQ = &gFQI.gFQLink[gFQI.ubFQLinkFree];
				FQ->ubCEIndex = ubCEIndex;
				ubL4kLink = gFQI.ubFQLinkFree;
				M_GetMTDepth(ubDepth);
				if (gubRUTMixPlaneEnable == 0) {
					FQ->ubFJob = BYTE_FJOB_ERASE_UNIT;
				}
				else {
					if (ubEraseTypeUnit) {
						FQ->ubFJob = BYTE_FJOB_ERASE_UNIT;
					}
					else {
						FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
					}
				}

				if (PhyTarget->B.uwTarget >= 4096) {
					FQ->uwFJobInfo = ((gubFastPageForCopyFromRUT ? BIT_FJOBI_FASTPAGE : 0) | (ENABLE_RUT_SWAP ? (BIT_FJOBI_WAITRESULT | BIT_FJOBI_FAILHANDLE_RIGHTNOW) : 0));
				}
				else {
					FQ->uwFJobInfo = ((gulVC[PhyTarget->B.uwTarget].B.btFastPage ? BIT_FJOBI_FASTPAGE : 0) | (ENABLE_RUT_SWAP ? (BIT_FJOBI_WAITRESULT | BIT_FJOBI_FAILHANDLE_RIGHTNOW) : 0));
				}
#if ENABLE_ONLY_SLC_ERASE_FOR_D1==1
				if ( ubDSelect == D3 && (FQ->uwFUnit > 3/*排除VT Mother*/)) {
					FQ->uwFJobInfo &= (~BIT_FJOBI_FASTPAGE);
				}
#endif


				// Must Send Reset CMD(0xFF) After Erase Fail
				FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

#if Hynix3DV6
				if (ubTLCTreatment) {
					FQ->uwFJobInfo |= BIT_FJOBI_TLC_TREATMENT;
					FQ->uwFJobInfo &= (~BIT_FJOBI_FASTPAGE);
				}
#endif




				FQ->uwFUnit = PhyTarget->B.uwTarget;
				FQ->ulFEntry = ubi * gulFastPage4kEntrysPerUnit + (((ubCEIndex << gubBurstsPerBankLog) + ubBurstBank) << gub4kEntrysPerPlaneLog);
				FQ->ubRelativeDepth = ubDepth;
				FQ->ubDepth = 0xFF;
				FQ->btD3 = ubDSelect;
				FQ->btUNServed = 1;
				FQ->btDirectHandling = (gubDirectHandl_RUTLog == 1) ? 1 : 0;

				if (ENABLE_RUT_SWAP && ENABLE_DEBUG_HW_FORCEERASEFAIL) {
					/*
					if ((ubDoSwapRUT == TRUE) && (ubContinueFailCnt == 0)) {
						FQ->btGenFail = gubGenEraseFailDebug;	// 造erase fail
					}
					else {
						FQ->btGenFail = STOP_GEN_FAIL;	//已經是在UpdateRUT or SaveVT的一些狀況下, 不連續造fail
					}
					*/
					if (gubPreformat || ubContinueFailCnt) {
						FQ->btGenFail = STOP_GEN_FAIL;		// preformat階段 or 已經造過一次erase fail後就不造fail了
					}
					else {
						FQ->btGenFail = gubGenEraseFailDebug;	// 造erase fail
					}
				}
				gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gFQI.ubFQLinkNumber++;
				if (ENABLE_RUT_SWAP) {
					gubFQLinkIndex[((ubCEIndex * ubUnitCount) + ubi) * gubBurstsPerBank + ubBurstBank] = gFQI.ubFQLinkFree;
				}

				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
			}
		}
	}

	if (ubDSelect == D3) {
		if (PhyTarget->B.uwTarget < 4096) {
			gulEC_D3[PhyTarget->B.uwTarget].B.ulEraseCount++;
		}
		VT->gulTotalEraseCount++;
	}
	else {
		gulEC_D1[PhyTarget->B.uwTarget].B.ulEraseCount++;
#if TLC
		VT->gulTotal_Erase_Count_D1++;
#endif
	}

	if (ENABLE_DEBUG_UART_ERASE_COUNT) {
		Uart_Tx_Data(PhyTarget->B.uwTarget >> 8);
		Uart_Tx_Data(PhyTarget->B.uwTarget);
	}

	if (ENABLE_RUT_SWAP) {
		for (ubCEIndex = 0; ubCEIndex < gubCENumber; ubCEIndex++) {
			UBYTE ubUnitCount = (ubDSelect) ? 1 : gubNum_D1_Part;
			UBYTE ubi;
			for (ubi = 0; ubi < ubUnitCount ; ubi++) {
				for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; (ubBurstBank = ubBurstBank + ubBurstShift1)) {
					if (gubFQLinkIndex[((ubCEIndex * ubUnitCount) + ubi) * gubBurstsPerBank + ubBurstBank] != 0xFF) {
						FQ = &gFQI.gFQLink[gubFQLinkIndex[((ubCEIndex * ubUnitCount) + ubi) * gubBurstsPerBank + ubBurstBank]];
						while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
							M_SwitchTask();
						}
						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
						if (FQ->btPEFail) {
							ulEraseUnitFailCEMap |= (BIT0 << ubCEIndex);
						}
					}
				}
			}
		}
#if (!RDT)
#if(!BURNER)
		if (ubDoSwapRUT) {
			if (ulEraseUnitFailCEMap) {
				if (ENABLE_DEBUG_UART_CC) {
					if (ubDSelect == D3) {
						UartString("\n\r EF D3");
					}
					else {
						UartString("\n\r EF D1");
					}
				}

				if (ENABLE_DEBUG_UART) {
					UartString("Erase Fail Unit: ");
					UartWordHex(PhyTarget->B.uwTarget);
					UartString(" Map: ");
					UartLongHex(ulEraseUnitFailCEMap);
					UartString("\n");
				}

				if (ENABLE_DEBUG_HW_FORCEERASEFAIL) {
					ubContinueFailCnt++;	// 避免一直erase fail而一直卡在裡面處理swapRUT
					M_AssertError(ubContinueFailCnt >= 2);
				}

				ftlSwapRUT(1, Default_Unit, DONTCARE);	// erase fail需要馬上換掉RUT mapping

				if (ENABLE_CY_DEBUG) {
					ftlCheckUserFreeBlockTable();
				}

				ulEraseUnitFailCEMap = 0;	// clear before redo erase
				goto MARK_ReDoFtlEraseTarget;
			}
		}
#endif
#endif
	}
	if (ENABLE_DEBUG_HW_FORCEERASEFAIL) {
		gubGenEraseFailDebug = 1;	// default都會造erase fail, 只有還沒有fail handling flow的才在ftl layer用 gubGenEraseFailDebug = STOP_GEN_FAIL
	}

	return ulEraseUnitFailCEMap;
#endif
}


U32 getIntRandValue(U32 limit)
{
#if ENABLE_IOMETER_RAND
	gullRandomVector = ((ULLONG)RAND_VEC_A * gullRandomVector) + ((ULLONG)RAND_VEC_B);
#else
	gullRandomVector = rand();
#endif
	return (U32)(gullRandomVector % limit);
}


