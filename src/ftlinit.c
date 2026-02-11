#include "setup.h"
#include "Vardef.h"
#include "ftl.h"
#include <string.h>

void ftlUpdateMixPlaneStatus()
{
	if (gubRUTMixPlaneEnable) {
		UWORD uwCurrentUnit;
		UBYTE ubCurrentPlane;
		UBYTE ubCurrentCE;
		UWORD uwTotalUnit = guwFWTotalUnitNum;

		for (uwCurrentUnit = 0; uwCurrentUnit < 8; uwCurrentUnit++) {
			uwTotalUnit = (VT->guwDieStartSFUnitIndex[uwCurrentUnit] > uwTotalUnit) ? VT->guwDieStartSFUnitIndex[uwCurrentUnit] : uwTotalUnit;
		}


		for ( uwCurrentUnit = 0; uwCurrentUnit < uwTotalUnit; uwCurrentUnit++) {
			gulEC_D3[uwCurrentUnit].B.btMixPlaneUnit = 0;
			for (ubCurrentCE = 0; ubCurrentCE < gubCENumber; ubCurrentCE++) {
				for (ubCurrentPlane = 0; ubCurrentPlane < gubBurstsPerBank; ubCurrentPlane++) {
					if (ubCurrentPlane != (M_RUT2DTo1D(ubCurrentCE, (((uwCurrentUnit) << gubBurstsPerBankLog) + ubCurrentPlane))&gubBurstsPerBankMask )) {
						gulEC_D3[uwCurrentUnit].B.btMixPlaneUnit = 1;
						ubCurrentCE    = gubCENumber;
						ubCurrentPlane = gubBurstsPerBank;
					}
				}
			}
		}
	}
}
void CopyVTAreaRUTToRAM(void * buffer)
{
	M_CheckBurnerModeToDisableFunction();

	UBYTE ubPlaneBank;
	UBYTE ubCEIndex;
	UBYTE ubUnit;
	UWORD * uwBuffer = (UWORD *)buffer;

	for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
		ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		for (ubUnit = 0; ubUnit < 4; ubUnit++) {
			uwBuffer[(ubPlaneBank * 4) + ubUnit] = M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[ubUnit].B.uwTarget << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask));
		}

	}
}

void ProgramVTPowerCyclingCheck(UWORD * buffer_src, UWORD * buffer_dest)
{
	M_CheckBurnerModeToDisableFunction();

	UWORD uwIndex;
	for (uwIndex = 0; uwIndex < gubPlanesPerPage * 4; uwIndex++) {
		M_AssertCriticalError((buffer_src[uwIndex] != buffer_dest[uwIndex]), VT_ASSERT_RUTADDRESS_NOTMATCH_0x9B);
	}
}

U32 ftlInitFlash(void)
{
#if (!RDT)
	M_CheckRDTModeToDisableFunctionReturnValid(1);

	UBYTE ubCEIndex, ubPlaneIndex, ubi, ubj, ubL4KNum = gub4kEntrysPerPlane;
	UBYTE ubScanTableDoneIndex = 0;
	UBYTE ubPlaneNum;
#if B0KB
	UBYTE ubFQIndex[MAX_PLANE_PER_P2L];
#else
	UBYTE ubFQIndex[MAX_PLANE_NUM];
#endif
	UBYTE ubNeedCheckFQIndex;
	UBYTE ubNeedCheckFQNum;
	UBYTE ubMax_buffer_Plane_Num;
	UBYTE ubRenew_VT_flag = 0;
#if (!BURNER)
	UBYTE ubPC_para = 0;
#endif
	UWORD uwInRAMIndex;
	UWORD uwP2LInfo;
	UWORD uwUnit;
	UWORD uwFJobInfo;
	U32 ulPlane_PTR;
	U32 ulPlaneIndexofTable[8];
	U32 ulScanRegion = 0;
	U32 ulSpare_Mark;
	U32 ulDest_Address;
	U32 ulTableIndex;
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	U32 ulTime[10] = {0};
#endif
	U32 ulScanNum[7] = {0}; // VT, VTChild
	FlashQueue_t *FQ;
	L4KTable16B *pL4KTablePtr;
	P4KTable16B *pP4KTablePtr;
	L4KTable16B pL4KTable;

#if (!BURNER)
	UBYTE ubScanGRTargetIndex;
	U32 ulGRTableSize;
#endif

Mark_InitialTable: // 先塞DMAC SQ, 才掃VT, 同步做可以節省時間
	// for table & GR
	// Saving the GRTable and the tmp GR table to flash seems to read overflow due to the 16K per plane.
	mDMAC_SV(4, (U32)gulGRTable, GRTableSize + TempGRTableSize, 0, gulTable_Empty, 0, 0, 0);
	mDMAC_SV(4, (U32)guwCleanGRGroups, 128 * 1024, 0, 0, 0, 0, 0);
	/*mDMAC_SV(4, (U32)gulGRTable, GRTableSize + TempGRTableSize, 0, Table_Empty, 0, 0, 0);
	mDMAC_SV(4, (U32)GRSearchTable, 256 * 1024, 0, GRSearchTable_Default_Value, 0, 0, 0);
	mDMAC_SV(4, (U32)guwCleanGRGroups, 128 * 1024, 0, 0, 0, 0, 0);
	mDMAC_SV(4, (U32)gulGRSearchLinkTail, (256 + 1024 + 1024) * 1024, 0, 0, 0, 0, 0); // Tail Physical Next*/
	mDMAC_SV(4, (U32)gulTable_L2P, (240 + 32 + 32) * 1024, 0, 0, 0, 0, 0); // Table_L2P,  gubTable_P2L_BitMap, guwActiveTable_P2L
#if MicronFlashOnly
	mDMAC_SV(4, (U32)guwActiveTable_P2L, 64 * 1024, 0, 0, 0, 0, 0); // Table_L2P,  gubTable_P2L_BitMap, guwActiveTable_P2L
#endif
	mDMAC_SV(2, (U32)guwL2P_InRamIndex, 120 * 1024, 0, 0x7FFF, 0, 0, 0);
	mDMAC_SV(4, (U32)gulL2P_InRamInverse, L2PInRamInverseSize, 0, NoL2PTableHere, 0, 0, 0);
#if ENABLE_READ_CHECK_FUNCTION
	mDMAC_SV(2, (U32)guwUnitReadCnt, 0x2000, 0, 0, 0, 0, 0);
	mDMAC_SV(2, (U32)guwUnitReadCheckInfo, 0x2000, 0, 0xFFFF, 0, 0, 0);
	mDMAC_SV(2, (U32)guwForceRead_Info, 0x2000, 0, 0xFFFF, 0, DMA_EN_SETCQ, 0);
	guwForceRead_Info->uwQueuePushCnt = 0;
	guwForceRead_Info->uwQueueQuota = 0;
#endif
#if (Read_Chk_ByHostRead || Read_Chk_ByReadScan)
	mDMAC_SV(4, (U32)guwUnitRRCnt, 2 * 4 * 1024, 0, 0, 0, 0, 0);
#endif

	mDMAC_SV(4, (U32)gubTrimTable, TRIMSize + DataInGRSize, 0, 0, 0, 0, 0); // gubTrimTable and gubDataInGR
	if (ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING) {
		mDMAC_SV(4, (U32)gubCopyBuffer, 1024 * 1024, 0, 0, 0, 0, 0);
		mDMAC_SV(4, (U32)gubCopyBuffer2, 1024 * 1024, 0, 0, 0, 0, 0);
	}
	else {
		mDMAC_SV(4, (U32)gubCopyBuffer, 2 * 1024 * 1024, 0, 0, 0, 0, 0);
	}
	/// added by Victor
#if((!ENABLE_OLD_TRIM)&&(!BURNER))
#if MicronFlashOnly
	gulTrimEnd4kIndex = (U32 *)(gubTrimTable);
	gulTrimStart4kIndex = (U32 *)(gubTrimTable + (D_TRIM_TABLE_NUMBER * 4 ));
	gulInRamInverseBackUp = (L2P_InRamInverse_t *)(gubTrimTable + 0x400);
#else
	gulTrimEnd4kIndex = (U32 *)(TableZoneBase + 0x1828000);
	gulTrimStart4kIndex = (U32 *)(TableZoneBase + 0x1828000 + (D_TRIM_TABLE_NUMBER * 4 ));
	gulInRamInverseBackUp = (L2P_InRamInverse_t *)(TableZoneBase + 0x1828400);
	//	gulInRamInverseBackUp = (L2P_InRamInverse_t *)(TableZoneBase + 0x1828000);
#endif
#endif

#if FixShallowErase
	guwVTChildPTRShallowErase = guwFastPagePlanesPerUnit;
	guwVTPTRShallowErase = guwFastPagePlanesPerUnit;
#endif
	// for GCGR  可以等到要走power cycle flow再清掉, 但記得做GC前也必須清掉
	/*mDMAC_SV(4, (U32)P2LSearchTableNext, 2048 * 1024, 0, 0, 0, 0, 0);
	mDMAC_SV(4, (U32)guwCleanGCGRGroups, 128 * 1024, 0, 0, 0, 0, 0);
	mDMAC_SV(4, (U32)P2LSearchTableNext, 2048 * 1024, 0, 0, 0, 0, 0);
	mDMAC_SV(4, (U32)GCGRSearchTablePhysical, (1024 + 1024 + 256 + 512) * 1024, 0, 0, 0, 0, 0); // Phy Next TRIM DIG*/
	// Zone 3
	mDMAC_SV(4, (U32)guoRSFrameBits, 1024 * 1024, 0, 0, 0, 0, 0);
	gubDoubleVT = 0;

Mark_ScanVT:
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	ulTime[0] = gulOperationTime;
#endif
	if (ENABLE_DEBUG_UART) {
		UartString("\nVT");
	}

	if (SYSB[0x44] == SIMREG_DEVSLP_LOAD_CODE) {
		ulPlane_PTR = gDEVSLP_Table->uwVT_PTR | (((U32)VT->guwVTAreaFreeTarget[gDEVSLP_Table->ubVTIndex].B.uwTarget) << 16);
		ulScanRegion = gDEVSLP_Table->uwVTChild_PTR;
	}
	else {
		ulPlane_PTR = 0xFFFFFFFF;
	}
	memset(gulVC, 0x00, 1024);
	CopyVTAreaRUTToRAM(&gulVC[0]);
	ftlScanVT_Binary(1, ulPlane_PTR);
	if (gubSSD_Init_State.B.btVTLastPlaneState) {
		ubRenew_VT_flag |= BIT_RENEW_VT;
	}
	if (VT->gubSaveVTSerialNumber == VT_INIT_FLASH_VTDUMMY_0x8B) {
		ubRenew_VT_flag |= BIT_SKIP_VTPTR_CHECK;
	}
	if (BURNER) {
		if (VT->gulVTLastMark != 0x55AA)
			return 1;
	}
	ulScanNum[0] = VT->guwVTPTR;
	if (ENABLE_DEBUG_UART) {
		UartString(" ");
		Uart_Tx_DataHex(VT->gubVTAreaFreeQueueInPTR);
		UartString(" ");
		UartWordHex(VT->guwVTPTR);
		UartString(" ");
		UartWordHex(VT->guwVTChildTarget.B.uwTarget);
	}

Mark_LoadRUT:
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	ulTime[1] = gulOperationTime;
#endif
	if (ENABLE_DEBUG_UART) {
		UartString(" RUT");
	}
	for (ubPlaneIndex = 0; ubPlaneIndex < gubPlanesPerPage; ubPlaneIndex++) {
		if (ENABLE_PLANE_CH_CE) {
			ubCEIndex = M_Modular((ubPlaneIndex >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			M_RUT2DTo1D(ubCEIndex, (VT->guwRUTTarget.B.uwTarget << gubBurstsPerBankLog) + (ubPlaneIndex & gubBurstsPerBankMask)) = VT->guwRUTFBlock[ubPlaneIndex];
		}
		else {
			ubCEIndex = (ubPlaneIndex & gubPlanesPerBurstMask) + ((M_Modular((ubPlaneIndex >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			M_RUT2DTo1D(ubCEIndex, (VT->guwRUTTarget.B.uwTarget << gubBurstsPerBankLog) + ((ubPlaneIndex >> gubPlanesPerBurstLog)&gubBurstsPerBankMask)) = VT->guwRUTFBlock[ubPlaneIndex];
		}
	}

	ubPlaneIndex = 0;
	ULLONG uoRUTPlaneNeedCheckMap = 0;
	ULLONG uoRUTPlaneCheckPassMap = 0;
	UWORD uwRUTPlanePtr = 0;
	UWORD uwScanDonePlanePtr = 0;
	UBYTE ubRUTSizeinPlane = RUTSize / 512 / gubSectorsPerPlane;
	UBYTE ubFailCEIndex;
	if ((RUTSize / 512 ) % gubSectorsPerPlane) {
		ubRUTSizeinPlane++;
	}
	//ulDMAAddr = (U32)gubZ2Buffer;
	U32 ulDMAAddr = (U32)&M_RUT2DTo1D(0, 0);
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ulDMAAddr, RUTSize);
	}

	for (ubi = 0; ubi < ubRUTSizeinPlane; ubi++) {
		uoRUTPlaneNeedCheckMap |= ((ULLONG)BIT0 << ubi);
	}

	while (uwScanDonePlanePtr < guwFastPagePlanesPerUnit) {
		ubi = 0;
		while (ubi < ubRUTSizeinPlane) { // total RUT is 128KB

			if (uoRUTPlaneCheckPassMap & ((ULLONG)BIT0 << ubi)) {	// 還沒check pass的才要讀
				ubi++;
				continue;
			}

			uwRUTPlanePtr = uwScanDonePlanePtr + ubi;
			if (uwRUTPlanePtr >= guwFastPagePlanesPerUnit) {
				break;
			}
			ubFQIndex[ubi] = gFQI.ubFQLinkFree;

			FlashQueue_t *FQ;
			UBYTE ubDepth, ubL4kLink;
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((uwRUTPlanePtr >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (uwRUTPlanePtr & gubPlanesPerBurstMask) + (((M_Modular((uwRUTPlanePtr >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
			}
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ulRAMAddr = (ulDMAAddr + (ubi * gub4kEntrysPerPlane * 4096));
			FQ->btCheckEmpty = 0;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY);
			FQ->uwFUnit = VT->guwRUTTarget.B.uwTarget;
			FQ->ulFEntry = (U32)uwRUTPlanePtr * gub4kEntrysPerPlane;
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btUNServed = 1;
#if TLC
			FQ->btD3 = 1;
#endif
			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

			UBYTE ubL4KCount;
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = 0x00000000;
				pL4KTable.Para0x04.ulAll = 0;
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
#if(!BURNER)
			ubi++;
		}
		UBYTE ubNeedCheckPlanes;
		if (ubi < ubRUTSizeinPlane) {
			ubNeedCheckPlanes = ubi;	// unit最後可能因空間不足無法再寫滿整個RUT mapping, 所以只check寫入的部份mapping
		}
		else {
			ubNeedCheckPlanes = ubRUTSizeinPlane;	// check一整組backup
		}
		ubi = 0;
		while (ubi < ubNeedCheckPlanes) { // total RUT is 128KB

			if (uoRUTPlaneCheckPassMap & ((ULLONG)BIT0 << ubi)) {
				ubi++;
				continue;
			}
#endif
			while (gFQI.gFQLink[ubFQIndex[ubi]].ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			gFQI.gFQLink[ubFQIndex[ubi]].ubFQPhase = BYTE_FQ_CHECKDONE;
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex[ubi]].ubL4kIndex * L4kQ_BIND_FACTOR * Spare4kSize));

			if ( (pL4KTablePtr->ulL4K_LCA != (MARK_RUT | ubi)) || gFQI.gFQLink[ubFQIndex[ubi]].btReadUNC) {
				uwRUTPlanePtr = uwScanDonePlanePtr + ubi;

				if (ENABLE_PLANE_CH_CE) {
					ubCEIndex = M_Modular((uwRUTPlanePtr >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
				}
				else {
					ubCEIndex = (uwRUTPlanePtr & gubPlanesPerBurstMask) + (((M_Modular((uwRUTPlanePtr >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
				}
				ubFailCEIndex = ubCEIndex;
			}
			else {
				uoRUTPlaneCheckPassMap |= ((ULLONG)BIT0 << ubi);
			}
			ubi++;
		}
		if (uoRUTPlaneCheckPassMap == uoRUTPlaneNeedCheckMap) {
			break;
		}
		else {
			uwScanDonePlanePtr += ubRUTSizeinPlane;
		}
	}
	if (uoRUTPlaneCheckPassMap != uoRUTPlaneNeedCheckMap) {	//湊不到完整的RUT mapping
		if (BURNER) {
			ftlPreformatShowError(ubFailCEIndex, VT->guwRUTTarget.B.uwTarget, 0x24);
		}
		else {
			M_AssertCriticalError(1, VT_ASSERT_INIT_FLASH_0x2F);
		}
		return 1;
	}

	if (BURNER || (gubPreformat == 1)) {
		gubRUT_EC_set |= BIT1;
	}
	if (ENABLE_DCACHE) {
		ulDMAAddr = (U32)&M_RUT2DTo1D(0, 0);
		xthal_dcache_region_invalidate((void *)ulDMAAddr, RUTSize);
	}
	// Load RUT Done Check it Same with Infoblock Record
	CopyVTAreaRUTToRAM(&gulVC[512 / sizeof(ValidCount_t)]);
	ProgramVTPowerCyclingCheck((void *)&gulVC[0], (void *)&gulVC[512 / sizeof(ValidCount_t)]);
Mark_ScanVTChild:
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	ulTime[2] = gulOperationTime;
#endif
	if (ENABLE_DEBUG_UART) {
		UartString(" VTChild");
	}
	gubSSD_Init_State.B.btVTLastPlaneState = 0;

	if (SYSB[0x44] == SIMREG_DEVSLP_LOAD_CODE) {
		if (ulScanRegion != 0xFFFF) {
			ulPlane_PTR = ulScanRegion | (((U32)VT->guwVTChildTarget.B.uwTarget) << 16);
			ftlScanVT_Binary(0, ulPlane_PTR);
		}
	}
	else {
		ulPlane_PTR = 0xFFFFFFFF;
		ftlScanVT_Binary(0, ulPlane_PTR);
	}
	if (gubSSD_Init_State.B.btVTLastPlaneState) {
		ubRenew_VT_flag |= BIT_RENEW_VTCHILD; /* renew VTC. */
	}
	gubSSD_Init_State.B.btVTLastPlaneState = (ubRenew_VT_flag & (BIT_RENEW_VT | BIT_RENEW_VTCHILD)) ? 1 : 0;
	if (ENABLE_DEBUG_UART) {
		UartString(" ");
		UartWordHex(VT->guwVTChildPTR);
		UartString(" ");
		Uart_Tx_DataHex(VT->gubSaveVTSerialNumber);
	}

	if (!BURNER) {
		VT->gulFTLState.B.btNeedRUTReplace = 0;
		VT->gulFTLState.B.btDoingPreLoadL2P = 0; //Avoid PowerCycle When ftlPreLoadL2P() switchTask TrimAll()
	}

	if (BURNER) {
		if ((ulScanNum[0] != VT->guwVTPTR) && !(ubRenew_VT_flag & BIT_SKIP_VTPTR_CHECK)) {
			return 1;
		}
	}
	else {
		M_AssertCriticalError(((ulScanNum[0] != VT->guwVTPTR) && !(ubRenew_VT_flag & BIT_SKIP_VTPTR_CHECK)), VT_ASSERT_INIT_FLASH_0x30);
	}
	ulScanNum[1] = VT->guwVTChildPTR;

	if (ENABLE_DEBUG_FORCE_POWERCYCLE_FLOW) {
		VT->gulFTLState.B.btFlushCache = FALSE;
	}
MARK_Check_Write_Protect:
	if (ENABLE_WRITEPROTECT) {
		if (VT->gubWriteProtectState) {
			gubIsWriteProtect |= BIT_TRULY_WRITEPROTECT;
		}
	}

	mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 1);//強迫DMAC 全部做完
	gubSSD_Init_State.B.ubState = BYTE_INIT_VT_RUT_VTC_DONE;

Mark_Get_Table_L2P:
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	ulTime[3] = gulOperationTime;
#endif
	if (ENABLE_DEBUG_UART) {
		UartString(" Table");
	}
	if (VT->gubTableTargetIndex != VT->gubTableTargetSyncIndex) {
		if (ENABLE_DEBUG_UART) {
			UartString("\nT_S!=T_A");
		}
		//VT->gulNoUse4B[VT_NoUse4Byte_DebugIndex - 1] ++; // record sync table and active table are not match when power cycle happen
		VT->gubTableTargetIndex = VT->gubTableTargetSyncIndex;
	}
Mark_Set_Unit_Get_Table_L2P:
	if (VT->gulFTLState.B.btInitLoadTableHeader_Directly && VT->gulFTLState.B.btFlushCache) {
		uwUnit = VT->guwInitInfoTarget.B.uwTarget;
		ulPlane_PTR = VT->guwTableInitInfoPTR;
		ulScanRegion = gubTableHeadInfoPlaneCnt + gubPlanesPerTableP2LTable + gubPlanesPerSeqTable;
		uwFJobInfo = (ENABLE_RUT_SWAP ? BIT_FJOBI_FAILHANDLE_RIGHTNOW : 0);
	}
	else {
		uwUnit = VT->guwTableTarget[VT->gubTableTargetIndex].B.uwTarget;
		ulPlane_PTR = 0;
		ulScanRegion = gubTableHeadInfoPlaneCnt;
		uwFJobInfo = BIT_FJOBI_RS;
	}

	ubPlaneIndex = 0;
	while (ubPlaneIndex < ulScanRegion) {
		if (ENABLE_PLANE_CH_CE) {
			ubCEIndex = M_Modular((ulPlane_PTR >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		}
		else {
			ubCEIndex = (ulPlane_PTR & gubPlanesPerBurstMask) + ((M_Modular((ulPlane_PTR >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
		}

		UBYTE ubDepth, ubL4kLink;
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->btCheckEmpty = 0;
		if (ubPlaneIndex < gubTableHeadInfoPlaneCnt) {
			FQ->ulRAMAddr = ((U32)gulTable_L2P) + ((512 << gubSectorsPerPlaneLog) * ubPlaneIndex);
			ulSpare_Mark = MARK_TableHead | ubPlaneIndex;
		}
		else if (ubPlaneIndex < (gubTableHeadInfoPlaneCnt + gubPlanesPerTableP2LTable)) {
			FQ->ulRAMAddr = ((U32)guwActiveTable_P2L) + ((512 << gubSectorsPerPlaneLog) * (ubPlaneIndex - gubTableHeadInfoPlaneCnt));
			ulSpare_Mark = MARK_TableP2L | (ubPlaneIndex - gubTableHeadInfoPlaneCnt);
		}
		else {
			FQ->ulRAMAddr = ((U32)guoSequential_PhyHead) + ((512 << gubSectorsPerPlaneLog) * (ubPlaneIndex - gubTableHeadInfoPlaneCnt - gubPlanesPerTableP2LTable));
			ulSpare_Mark = MARK_TableSeq | (ubPlaneIndex - gubTableHeadInfoPlaneCnt - gubPlanesPerTableP2LTable);
		}
		FQ->btPreRead = 0;
		FQ->uwFUnit = uwUnit;
		FQ->ulFEntry = ulPlane_PTR << gub4kEntrysPerPlaneLog;
		FQ->ubFJob = BYTE_FJOB_READ;
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | uwFJobInfo);
		FQ->ubDepth = ubDepth;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btNoWait = 1;
		FQ->btUNServed = 1;
		FQ->btD3 = 1;
		if (FQ->uwFUnit == VT->guwTableTarget[VT->gubTableTargetIndex].B.uwTarget) {
			FQ->FQPara.B.btRWTableUnit = 1;
		}

		gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
		gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
		gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
		gulPara0x24.BitMap.btZipBypass = 1;
		gulPara0x24.BitMap.btCmpEn = 0;
		gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
		gulPara0x24.BitMap.btBufModeEn = 0;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
		gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void *)FQ->ulRAMAddr, ubL4KNum * 4096);
		}
		UBYTE ubL4KCount;
		for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA = 0x00000000;
			pL4KTable.Para0x04.BitMap.ulL4K_FW = 0x000000;
			pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0x00;
			pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0x00;
			pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
			pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
			*pL4KTablePtr = pL4KTable;
		}
		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
		while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
		for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));

			if ((pL4KTablePtr->ulL4K_LCA != ulSpare_Mark) || FQ->btReadUNC) {
				if (BURNER) {
					return 1;
				}

				if (VT->gulFTLState.B.btInitLoadTableHeader_Directly) {
					VT->gulFTLState.B.btInitLoadTableHeader_Directly = 0;//把正常斷電流程存放initinfo的data標示無效
					gubSpecialUnitReadMoveMap |= INITINFO_READMOVE;
					goto Mark_Set_Unit_Get_Table_L2P;//進到abnormal power off的flow
				}
				else {
					M_AssertCriticalError(TRUE, VT_ASSERT_INIT_FLASH_0x31);
				}
			}
		}

		if (ENABLE_RUT_SWAP && (uwUnit == VT->guwInitInfoTarget.B.uwTarget)) {
			if (FQ->btReadMove) {
				gubSpecialUnitReadMoveMap |= INITINFO_READMOVE;

			}
		}

		ubPlaneIndex++;
		ulPlane_PTR ++;
	}

#if(!BURNER)
	if (ENABLE_DEBUG_TABLE_VC_CHECK) {
		ftlCheckTableVC();
	}
#endif
	if (VT->gulFTLState.B.btInitLoadTableHeader_Directly && VT->gulFTLState.B.btFlushCache) {
		mSE_MSV(4, (U32)gulTable_L2P, gulTotalL2PGroupCnt * 4, SearchLinkDefault, 0, (SearchLinkDefault | TableTrimBit | 0x800000), 0, 1);
		if ((gubPlanesPerSeqTable * 4096 * gub4kEntrysPerPlane) < (256 * 1024)) {
			mSE_SV(8, ((U32)guoSequential_PhyHead) + (gubPlanesPerSeqTable * 4096 * gub4kEntrysPerPlane), (256 * 1024) - (gubPlanesPerSeqTable * 4096 * gub4kEntrysPerPlane), 0, 0);
		}
		if (BURNER || (gubPreformat == 1))
			goto Mark_ScanActiveTableUnit;
		else
			goto Mark_Load_Necessary_Table;
	}
	else {
		if (VT->gulFTLState.B.btFlushCache) {
			//VT->gulNoUse4B[VT_NoUse4Byte_DebugIndex] ++; // flush cache state without info block
		}
		mSE_MSV(4, (U32)gulTable_L2P, gulTotalL2PGroupCnt * 4, SearchLinkDefault, 0, (SearchLinkDefault | TableTrimBit | TableSeqBit | 0x800000), 0, 1); //不能往前挪, 要在這裡設值
		mSE_SV(8, (U32)guoSequential_PhyHead, 256 * 1024, 0, 0); //不需往前挪, 沒有從flash讀才需設值
	}

Mark_ScanActiveTableUnit:
	//Clear active table unit bit map and VC
	mSE_SV(1, (U32)&gubTable_P2L_BitMap[(VT->gubTableTargetIndex * guwTableP2LBitMapSize) / 8],  guwTableP2LBitMapSize / 8, 0, 0);
	// Clear guwActiveTable_P2L
#if MicronFlashOnly
	mSE_SV(1, (U32)guwActiveTable_P2L,  guwTableP2LBitMapSize * 2, 0, 0); //不需往前挪, 沒有從flash讀才需設值
#else
	mSE_SV(1, (U32)guwActiveTable_P2L,  32 * 1024, 0, 0); //不需往前挪, 沒有從flash讀才需設值
#endif
	guwTable_VC[VT->gubTableTargetIndex] = 0;

	U32 ulSearchBase = (U32)gulTable_L2P;
	U32 ulByteCount = (gulTotalL2PGroupCnt + TableTypeNum_ExceptL2P + gubHostTable_GroupNum) * 4; // 4 Byte per entry
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ulSearchBase, ulByteCount);
	}
	while (1) {
		while (SEB[SEB_SQ_WR_CNT] < 1);
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 0] = SE0_SEQU | SE0_CMDUNIT_1 | SE0_SIZE_4BYTE | SE0_MATCH_NUMBER_6 | SE0_ENABLE_MASK | SE0_TARGET(0);
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 1] = ulSearchBase;
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 2] = ulByteCount; // 4 Byte per entry
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 3] = TableIndex_Mask;
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 4] = TableIndex_Mask & (U32)(VT->gubTableTargetIndex << TableIndex_StartBit);
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 5] = 0;
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 6] = 0;
		SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 7] = 0;
		SEB[SEB_SQ_WPIU] = 1;
		while (SEB[SEB_CQ_RD_CNT] < 1);
		SEB[SEB_CQ_RPIU] = 1;

		UBYTE ubMatchCount = 0;
		while (ubMatchCount < (SEL_CQBaseAddr[guwCQCnt_SE0 * 8] >> 16) ) {
			gulTable_L2P[SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + ubMatchCount + 1] + ((ulSearchBase - (U32)gulTable_L2P) / 4)].All |= SearchLinkDefault | TableNotInFlash;
			ubMatchCount ++;
		}
		if ((SEL_CQBaseAddr[guwCQCnt_SE0 * 8] & SE0_CQ_SH_STOP) == 0) { //表示找完沒有了
			guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
			guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;
			break;
		}

		ulByteCount -= (SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + 7] - ulSearchBase);
		ulSearchBase = SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + 7];
		guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
		guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;
	}

	// Update Table_L2P by current Table Unit
	ulPlane_PTR = gubTableHeadInfoPlaneCnt;

	if (VT->guwTableTargetSyncPTR < guwTableP2LPlanePtr) {
		ulScanRegion = VT->guwTableTargetSyncPTR;
		guwTable_LastEncodePlane =  M_Multiplier((M_Divider(VT->guwTableTargetSyncPTR, DEF_TableParityPlanes_Mcl_Div)), DEF_TableParityPlanes_Mcl_Div);
	}
	else {
		ulScanRegion = guwTableP2LPlanePtr;
		guwTable_LastEncodePlane = guwFastPagePlanesPerUnit;
	}
	ubMax_buffer_Plane_Num = ENABLE_SCAN_TABLE_LOAD_DATA  ? (BUFFER2_NUM / gubSectorsPerPlane) : gubPlanesPerPage;
	UBYTE ubBufferValid = ENABLE_SCAN_TABLE_LOAD_DATA ? 0xFF : 0; // 順便抓一些L2P Table到Active L2P Table(不分 write read mode)
	Table_L2P_t ulLocalTable_L2P;

	while (ulPlane_PTR < ulScanRegion) {
		for (ubNeedCheckFQIndex = 0; ubNeedCheckFQIndex < ubMax_buffer_Plane_Num; ubNeedCheckFQIndex ++) {
			// Skip read table parity
			if ((M_Modular(ulPlane_PTR, DEF_TableParityPlanes_Mod_Mask)) == guwTableParityPlanesNumMax) {
				ulPlane_PTR ++;
			}
			if (ulPlane_PTR >= ulScanRegion) {
				break;
			}

			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ulPlane_PTR >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (ulPlane_PTR & gubPlanesPerBurstMask) + ((M_Modular((ulPlane_PTR >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->btCheckEmpty = 0;
			FQ->btPreRead = 0;
			FQ->ulRAMAddr = BUFFER2_START + ((U32)4096 << gub4kEntrysPerPlaneLog) * ubNeedCheckFQIndex;
			FQ->uwFUnit = VT->guwTableTarget[VT->gubTableTargetIndex].B.uwTarget;
			FQ->ulFEntry = ulPlane_PTR << gub4kEntrysPerPlaneLog;
			FQ->ubFJob = BYTE_FJOB_READ;
			if (ulPlane_PTR > guwTable_LastEncodePlane) {
				FQ->uwFJobInfo = ( BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY);
			}
			else {
				FQ->uwFJobInfo = ( BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | BIT_FJOBI_RS);
			}
			FQ->btBunchRSParity = 0;
			FQ->FQPara.B.btRWTableUnit = 1;
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btNoWait = 1;
			FQ->btUNServed = 1;
			FQ->btD3 = 1;

			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

			UBYTE ubL4KCount;
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = MARK_BeforeRead;
				pL4KTable.Para0x04.ulAll = 0;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = ubBufferValid;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}
			ubFQIndex[ubNeedCheckFQIndex] = gFQI.ubFQLinkFree;
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
			ulPlane_PTR ++;
		}

		ubNeedCheckFQNum = ubNeedCheckFQIndex;
		for (ubNeedCheckFQIndex = 0; ubNeedCheckFQIndex < ubNeedCheckFQNum; ubNeedCheckFQIndex ++) {
			FQ = &gFQI.gFQLink[ubFQIndex[ubNeedCheckFQIndex]];
			while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

			pP4KTablePtr = (P4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize));
			ulSpare_Mark = pP4KTablePtr->ulL4K_LCA;
			U32 ulFQ_Plane_PTR = (FQ->ulFEntry >> gub4kEntrysPerPlaneLog);
#if(BURNER)
			if (0) {
#else
			if (FQ->btReadUNC) {
#endif
			Mark_GetUNC:
				if (ulFQ_Plane_PTR < ulScanRegion) {
					M_AssertCriticalError(1, VT_ASSERT_INIT_FLASH_0x32);
				}
				else {
					break;
				}
			}
			else if (ulSpare_Mark == MARK_TableDummy) {
			MarK_GetDummy:
				guwActiveTable_P2L[ulFQ_Plane_PTR] = 0xffff;
			}

			else if ((ulSpare_Mark & MARK_TableHost) == MARK_TableHost) {
			Mark_Get_Host:
				ulTableIndex = (ulSpare_Mark & 0xFF) + gulTableIndex_HOST;
				ulLocalTable_L2P.All = gulTable_L2P[ulTableIndex].All;
				if ((ulLocalTable_L2P.All & TableNotInFlash) != TableNotInFlash) {
					gulTemp = (ulLocalTable_L2P.B.uwFEntry >> gub4kEntrysPerPlaneLog) + ((U32)ulLocalTable_L2P.B.ubFUnitIndex * guwTableP2LBitMapSize);
					gubTable_P2L_BitMap[gulTemp / 8] &= ~(BIT0 << (gulTemp & 7));
					guwTable_VC[ulLocalTable_L2P.B.ubFUnitIndex] --;
				}
				gulTable_L2P[ulTableIndex].All = FQ->ulFEntry + (VT->gubTableTargetIndex << TableIndex_StartBit);
				gulTemp = ulFQ_Plane_PTR + (VT->gubTableTargetIndex * guwTableP2LBitMapSize);
				gubTable_P2L_BitMap[gulTemp / 8] |= BIT0 << (gulTemp & 7);
				guwActiveTable_P2L[ulFQ_Plane_PTR] = 0xff00 | (ulSpare_Mark & 0xFF);
				guwTable_VC[VT->gubTableTargetIndex] ++;
			}

			else if (((ulSpare_Mark & MARK_TableVC) == MARK_TableVC) || ((ulSpare_Mark & MARK_TableEC) == MARK_TableEC)) {
			Mark_GetTable_ExceptL2P:
				if (ulSpare_Mark == MARK_TableEC) {
					uwP2LInfo = 0xffc3;
					ubPlaneNum = gubPlanesPerECTable;
					ulDest_Address = (U32)gulEC_D3;
					ulTableIndex = gulTableIndex_EC;
				}
				else if (ulSpare_Mark == MARK_TableVC) {
					uwP2LInfo = 0xffc4;
					ubPlaneNum = gubPlanesPerVCTable;
					ulDest_Address = (U32)gulVC;
					ulTableIndex = gulTableIndex_VC;
				}

				ulPlaneIndexofTable[ubScanTableDoneIndex] = ulFQ_Plane_PTR;
				ubScanTableDoneIndex ++;

				if (ubScanTableDoneIndex < ubPlaneNum) {
					continue;
				}
				//Update Table_P2L_BitMap, Vald Count
				ulLocalTable_L2P.All = gulTable_L2P[ulTableIndex].All;
				if ((ulLocalTable_L2P.All & TableNotInFlash) != TableNotInFlash) {
					U32 ulStartPlaneIndex = (ulLocalTable_L2P.B.uwFEntry >> gub4kEntrysPerPlaneLog) + ((U32)ulLocalTable_L2P.B.ubFUnitIndex * guwTableP2LBitMapSize);
					ubi = 0;
					ubj = 0;
					while (ubi < ubPlaneNum) {
						if ((M_Modular((ulStartPlaneIndex + ubj), DEF_TableParityPlanes_Mod_Mask)) == guwTableParityPlanesNumMax) {
							ubj ++;
						}
						gulTemp = ulStartPlaneIndex + ubj;
						gubTable_P2L_BitMap[gulTemp / 8] &= ~(BIT0 << (gulTemp & 7));
						ubi ++;
						ubj ++;
					}
					guwTable_VC[ulLocalTable_L2P.B.ubFUnitIndex] -= ubPlaneNum;
				}

				for (ubi = 0; ubi < ubPlaneNum; ubi ++) {
					gulTemp = ulPlaneIndexofTable[ubi] + (VT->gubTableTargetIndex * guwTableP2LBitMapSize);
					gubTable_P2L_BitMap[gulTemp / 8] |= BIT0 << (gulTemp & 7);
					guwActiveTable_P2L[ulPlaneIndexofTable[ubi]] = uwP2LInfo;
				}
				guwTable_VC[VT->gubTableTargetIndex] += ubPlaneNum;
				gulTable_L2P[ulTableIndex].All = (ulPlaneIndexofTable[0] << gub4kEntrysPerPlaneLog) + (VT->gubTableTargetIndex << TableIndex_StartBit);
				ubScanTableDoneIndex = 0;
			}

			else {
				UWORD uwL2P_Group = ulSpare_Mark;
			Mark_Get_L2P:
				//Update Table_L2P, Table_P2L_BitMap, Vald Count
				if ((gulTable_L2P[uwL2P_Group].All & TableNotInFlash) != TableNotInFlash) {
					gulTemp = (gulTable_L2P[uwL2P_Group].B.uwFEntry >> gub4kEntrysPerPlaneLog) + ((U32)gulTable_L2P[uwL2P_Group].B.ubFUnitIndex * guwTableP2LBitMapSize);
					gubTable_P2L_BitMap[gulTemp / 8] &= ~(BIT0 << (gulTemp & 7));
					guwTable_VC[gulTable_L2P[uwL2P_Group].B.ubFUnitIndex] --;
				}

				gulTemp = ulFQ_Plane_PTR + (VT->gubTableTargetIndex * guwTableP2LBitMapSize);
				gubTable_P2L_BitMap[gulTemp / 8] |= BIT0 << (gulTemp & 7);
				guwActiveTable_P2L[ulFQ_Plane_PTR] = uwL2P_Group;
				guwTable_VC[VT->gubTableTargetIndex] ++;
				gulTable_L2P[uwL2P_Group].All = FQ->ulFEntry + (VT->gubTableTargetIndex << TableIndex_StartBit) + SearchLinkDefault + (pP4KTablePtr->Para0x04.BitMap.ulL4K_FW << TableAllZero_StartBit);

				if (ENABLE_SCAN_TABLE_LOAD_DATA) {
					//Update L2P_InRAMIndex and L2P_InRAMInverse
					uwInRAMIndex = guwActiveL2PTableReadPTR;
					UWORD uwPreviousInRAMIndex = (guwL2P_InRamIndex[uwL2P_Group] & TableInTempMask);
					if (uwPreviousInRAMIndex !=  TableNotInRAM) {
						gulL2P_InRamInverse[uwPreviousInRAMIndex].All = NoL2PTableHere;
					}
					if (gulL2P_InRamInverse[uwInRAMIndex].B.ulL2PGroupIndex != NoL2PTableHere) {
						guwL2P_InRamIndex[gulL2P_InRamInverse[uwInRAMIndex].B.ulL2PGroupIndex] = TableNotInRAM; // Remove mapping
					}

					guwL2P_InRamIndex[uwL2P_Group] = uwInRAMIndex;
					gulL2P_InRamInverse[uwInRAMIndex].All = (uwL2P_Group | BIT_VALID_L2P | BIT_FLAG_L2P);

					mDMAC_COPY(FQ->ulRAMAddr, ((U32)&gulL2PBase[guwActiveL2PTableReadPTR * guwL2PEntrysPerL2PGroup]), 512 * gubSectorsPerPlane, 0, 0, 0, 0, 1);

					guwActiveL2PTableReadPTR ++;
					if (guwActiveL2PTableReadPTR >= guwActiveTableNum) {
						guwActiveL2PTableReadPTR = 0;
					}
				}
			}
		}
	}
	if (guwActiveL2PTableReadPTR >= guwWriteModeTableNum_ReadModeStartIndex) {
		guwActiveL2PTableReadPTR -= guwWriteModeTableNum_ReadModeStartIndex;
	}
	else {
		guwActiveL2PTableReadPTR = 0;
	}

	if (BURNER || (gubPreformat == 1)) {
		ftlLoadTable(BYTE_Load_EC_Mode, (U32)gulEC_D3, 0, 0, 0);
		gubRUT_EC_set |= BIT0;
		ftlUpdateMixPlaneStatus();
		return 0;
	}
#if(!BURNER)
	if (ENABLE_DEBUG_TABLE_VC_CHECK) {
		ftlCheckTableVC();
	}
#endif

Mark_Load_Necessary_Table:
	// initial完就update TableTargetProgramDonePTR, 這樣table unit 發生program fail才知道guwTableTargetProgramDonePTR前的plane要從flh讀
	guwTableTargetProgramDonePTR = VT->guwTableTargetPTR - 1;

#if(!BURNER)
	ftlSwapTable(BYTE_EC_Load_Mode, 0);
	ftlSwapTable(BYTE_VC_Load_Mode, 0);
	ftlUpdateMixPlaneStatus();
	gulSLCPoolSize_Sync = gulSLCPoolSize_LastLoad;
	if (!(gubPreformat == 1 || BURNER)) {
		ftlSwapTable(BYTE_HOST_Load_Mode, HOSTTABLE_ATACFG_ADR);
		memcpy(&AtaCfg, &BUFB_BASE[0], sizeof(ATACONFIG));
	}
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	ulTime[4] = gulOperationTime;
#endif
#if TLC
	if (TLC) {
		// Mark_Rebuild_FreeBlockCountTable_D1:
		UWORD uwi;
		UWORD uwFreeCountD1 = VT->guwFreeBlockCount_D1;
		FTarget_t *uwTempTargetPtr;
		mSE_SV(2, (U32)gulGRTable, 512, 0xFFFF, 0);
		uwTempTargetPtr = (FTarget_t *) &gulGRTable[0];

		gubRebuildFreeQueueDoing = 1;

		for (uwi = 0; uwi < uwFreeCountD1; uwi++) {
			ftlGetUserFreeBlock(&uwTempTargetPtr[uwi], 0, D1);
		}

		for (uwi = 0; uwi < uwFreeCountD1; uwi++) {
			ftlAddToUserFreeTable(&uwTempTargetPtr[uwi], D1);
		}

		gubRebuildFreeQueueDoing = 0;

		//mDMAC_SV(2, (U32)gulGRTable, Default_D1_Unit, 0, 0xFF, 0, 0, 0);

	Mark_Rebuild_btInD1Unit:
		for (uwUnit = 0; uwUnit < MAX_USABLEUNIT; uwUnit++) {
			if (gubD1LinkTable[uwUnit].B.ubD1Link != Default_D1_Unit) {
				gulVC[uwUnit].B.btInD1Unit = 1;
			}
			else {
				gulVC[uwUnit].B.btInD1Unit = 0;
			}
		}
	}
#endif
Mark_Check_Old_RUT:
	if (VT->guwOldRUTTarget.All != Default_Unit) {
		M_AssertError(VT->guwVTChildPTR != 0);
		VT->gulDebugInfo[0] = 29; //AddFreeQ Caller Num
		ftlAddToUserFreeTable(&VT->guwOldRUTTarget, D3);
		VT->guwOldRUTTarget.All = Default_Unit;
	}

Mark_Check_Old_VTChild:
	// Check VT Child first for save VT
	if (VT->guwOldVTChildTarget.B.uwTarget != Default_Unit) {		// old VT Child hasn't been added to user free yet.
#if FixShallowErase
		U32 ulRET = 0;
		// Check old VT Child for shallow erase
		ulRET = ftlScanUnit_Binary(BIT_D3_TARGET, VT->guwOldVTChildTarget.B.uwTarget);
		if (ENABLE_D3_SHALLOWERASE_SAVE_DUMMY && (ulRET < (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div) - 1))) {
			ftlForceSaveDummy(ulRET, (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div)), VT->guwOldVTChildTarget.B.uwTarget, FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE, D3_AREA, VT_FORCE_DUMMY_0xD4);
		}
#endif
		VT->gulDebugInfo[0] = 30; //AddFreeQ Caller Num
		ftlAddToUserFreeTable(&VT->guwOldVTChildTarget , D3);
		VT->guwOldVTChildTarget.B.uwTarget = Default_Unit;
	}

#if DEBUG_FTLINIT_STATE==1
	UartString("\n[FTLINIT] FTLState: ");
	UartLongHex( VT->gulFTLState.All );
	if ( VT->gulFTLState.B.btFlushCache ) {
		UartString("\n[Warning] FlushCache = 1");
	}
#endif

Mark_Check_Power_Cycle_And_VT_State:
	if (VT->gulFTLState.B.btFlushCache && (gubSSD_Init_State.B.btVTLastPlaneState == 0)) {
		// 希望提早建Link與回D2H的時間
		//gubSSD_Init_State.B.ubState = BYTE_INIT_TABLE_DONE;
		if (VT->gulSLCPoolSize != gulSLCPoolSize_Sync) {
			VT->gulSLCPoolSize = gulSLCPoolSize_Sync;
		}
		M_AssertError( VT->gubGRTargetIndex);
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
		ulTime[5] = gulOperationTime;
#endif
	}
	else {
		UBYTE ubDummyPlaneNum = gubPlanesPerPage + (gubPlanesPerVT << ENABLE_VT_BACKUP);
		UBYTE ubDummyVTNum = ubDummyPlaneNum >> ENABLE_VT_BACKUP;
		UBYTE ubProgramIndex;
		UWORD uwOldVTPtr;
		UWORD uwOldVTCPtr;
	Mark_Erase_Another_VT:
		if (gubDoubleVT) {
			UBYTE ubVTIndex;
			if (gubDoubleVT == 1) { // there are two VTUnits and we took the latter one
				ubVTIndex = (VT->gubVTAreaFreeQueueInPTR == 0) ? (VTAREA_UNITS - 1) : (VT->gubVTAreaFreeQueueInPTR - 1);
			}
			else { // there are two VTUnits and due to latter one readFail, we took the previous one instead
				ubVTIndex = (VT->gubVTAreaFreeQueueInPTR == (VTAREA_UNITS - 1)) ? 0 : (VT->gubVTAreaFreeQueueInPTR + 1);
			}
			if (ftlEraseTarget(&VT->guwVTAreaFreeTarget[ubVTIndex], D3, FALSE)) {  // erase fail先不處理, 統一到下次SaveVT要用這個VT時的erase fail再處理
				//M_AssertCriticalError(1, VT_ASSERT_INIT_FLASH_0x33);			// 先卡下來
				gubVTAreaUnitEraseFailCnt++;	// record when ENABLE_DEBUG_RW = 0

				UWORD uwRUTSummaryLogIndex;
				for (uwRUTSummaryLogIndex = 0; uwRUTSummaryLogIndex < guwRUTSummaryLogNum; uwRUTSummaryLogIndex++) {
					if (VT->guwVTAreaFreeTarget[ubVTIndex].B.uwTarget == guwRUTSummaryLog[uwRUTSummaryLogIndex].B.uwUnit) {
						break;	// found
					}
				}

				if (uwRUTSummaryLogIndex == guwRUTSummaryLogNum) {
					M_AssertError(1);  // not found
				}
				else {
					// 因為沒有要馬上換, 所以這邊把多記的log清掉, 避免之後SwapRUT mode 1時做到這筆log而偷換VT的RUT mapping
					guwRUTSummaryLog[uwRUTSummaryLogIndex].B.btHandleLogDone = 1;
					guwRUTSummaryLog[uwRUTSummaryLogIndex].B.uwUnit = 0xFFFF;
					ftlMakeRUTLogCloser(VT->guwVTAreaFreeTarget[ubVTIndex].B.uwTarget, BIT_REARRANGE_RUTLOG | BIT_REARRANGE_RUTSUMMARYLOG);
				}
			}
			//gulEC_D3[VT->guwVTAreaFreeTarget[ubVTIndex].B.uwTarget].B.ulEraseCount --;
			//VT->gulTotalEraseCount --;
			gubDoubleVT = 0;
		}

		if ((ubRenew_VT_flag & BIT_RENEW_VT) || (VT->guwVTChildPTR == 0) || (VT->guwVTChildPTR == guwFastPagePlanesPerUnit)) { /* Renew VT but also need renew VT Child*/
#if FixShallowErase
			guwVTPTRShallowErase = VT->guwVTPTR;
			guwVTChildPTRShallowErase = VT->guwVTChildPTR;
#endif
			VT->guwVTPTR = guwFastPagePlanesPerUnit;
			VT->guwVTChildPTR = guwFastPagePlanesPerUnit;

		}
		else if ((ubRenew_VT_flag & BIT_RENEW_VTCHILD) || (!ENABLE_VT_DUMMY)) { /* Renew VT Child*/
#if FixShallowErase
			if (VT->guwVTChildPTR != guwFastPagePlanesPerUnit) {
				guwVTChildPTRShallowErase = VT->guwVTChildPTR;
			}
#endif
			VT->guwVTChildPTR = guwFastPagePlanesPerUnit;
		}


	Mark_Power_Cycle_Handle_VT_Table:
		if (VT->gulFTLState.B.btFlushCache == 0) {
			if (VT->gulFTLState.B.btDoingAbnormalInitial == 0) {
				VT->gulFTLState.B.btDoingAbnormalInitial = 1;
				VT->gulFTLState.B.btAbnormalInitialCopyDone = 0;
			}
			if (ENABLE_VT_DUMMY) {
			Mark_Save_VT_Dummy:
				if (ENABLE_DEBUG_UART_VT_INIT) {
					uwOldVTPtr = VT->guwVTPTR;
					uwOldVTCPtr = VT->guwVTChildPTR;
				}
				if (VT->guwVTPTR >= guwFastPagePlanesPerUnit - ubDummyPlaneNum) { // 因無法辨認VTMother是否正在被寫，故都補
#if FixShallowErase
					if (VT->guwVTPTR != guwFastPagePlanesPerUnit) {
						guwVTPTRShallowErase = VT->guwVTPTR;
					}
					if (VT->guwVTChildPTR != guwFastPagePlanesPerUnit) {
						guwVTChildPTRShallowErase = VT->guwVTChildPTR;
					}
#endif
					VT->guwVTChildPTR = guwFastPagePlanesPerUnit;
					VT->guwVTPTR = guwFastPagePlanesPerUnit;
				}
				else if (gubSpecialUnitReadMoveMap & VT_READMOVE) {
					// 會在 ftlSaveVT補滿, 故不用補 Dummy
				}
				else {
					if (ENABLE_DEBUG_UART_VT_INIT) {
						UartString("\nVTD");
					}
					for (ubProgramIndex = 0; ubProgramIndex < ubDummyVTNum; ubProgramIndex++) {
						ftlSaveVT(0, VT_INIT_FLASH_VTDUMMY_0x8B);
					}
				}
				if (VT->guwVTChildPTR >= guwFastPagePlanesPerUnit - ubDummyPlaneNum) {
#if FixShallowErase
					if (VT->guwVTChildPTR != guwFastPagePlanesPerUnit) {
						guwVTChildPTRShallowErase = VT->guwVTChildPTR;
					}
#endif
					VT->guwVTChildPTR = guwFastPagePlanesPerUnit;
				}
				else if ((gubUpdateRUTBeforeSaveVT && (VT->gulFTLState.B.btNeedRUTReplace == 0)) || (gubSpecialUnitReadMoveMap & (VTCHILD_READMOVE | VT_READMOVE))) {
					// 會在 ftlSaveVT補滿, 故不用補 Dummy
				}
				else {
					if (ENABLE_DEBUG_UART_VT_INIT) {
						UartString("\nVTCD");
					}
					for (ubProgramIndex = 0; ubProgramIndex < ubDummyVTNum; ubProgramIndex++) {
						ftlSaveVT(0, VT_INIT_FLASH_VTCHILDDUMMY_0x8C);
					}
				}

				if (ENABLE_DEBUG_UART_VT_INIT) {
					M_AssertError(((VT->guwVTPTR != uwOldVTPtr + ubDummyPlaneNum) && (VT->guwVTPTR != guwFastPagePlanesPerUnit))
					              || ((VT->guwVTChildPTR != uwOldVTCPtr + ubDummyPlaneNum) && (VT->guwVTChildPTR != guwFastPagePlanesPerUnit)));
				}
			}

			ftlSaveVT(0, VT_INIT_FLASH_0x15);

		Mark_Update_AllZero_Map:
			ulSearchBase = (U32)gulTable_L2P;
			ulByteCount = gulTotalL2PGroupCnt * 4; // 4 Byte per entry
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)ulSearchBase, ulByteCount);
			}
			while (1) {
				while (SEB[SEB_SQ_WR_CNT] < 1);
				SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 0] = SE0_SEQU | SE0_CMDUNIT_1 | SE0_SIZE_4BYTE | SE0_MATCH_NUMBER_6 | SE0_ENABLE_MASK | SE0_TARGET(0);
				SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 1] = ulSearchBase;
				SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 2] = ulByteCount; // 4 Byte per entry
				SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 3] = TableCheckAllZero;
				SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 4] = TableCheckAllZero;
				SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 5] = 0;
				SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 6] = 0;
				SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 7] = 0;
				SEB[SEB_SQ_WPIU] = 1;
				while (SEB[SEB_CQ_RD_CNT] < 1);
				SEB[SEB_CQ_RPIU] = 1;

				UBYTE ubMatchCount = 0;
				while (ubMatchCount < (SEL_CQBaseAddr[guwCQCnt_SE0 * 8] >> 16) ) {
					UWORD uwL2PGroupIndex = SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + ubMatchCount + 1] + ((ulSearchBase - (U32)gulTable_L2P) / 4);
					Table_L2P_t ulTable_L2P;
				Mark_Read_One_4KFrame_Spare_Only_Update_AllZeroMap:
					ulTable_L2P.All = gulTable_L2P[uwL2PGroupIndex].All;
					// Don't load from flah before this L2P plane is saved
					if ((ulTable_L2P.All & TableNotInFlash) == TableNotInFlash) {
						gulTable_L2P[uwL2PGroupIndex].B.ubAllZero_BM = 0xF;
					}
					else {
						if (ENABLE_PLANE_CH_CE) {
							ubCEIndex = M_Modular(((ulTable_L2P.B.uwFEntry >> gub4kEntrysPerPlaneLog) >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
						}
						else {
							ubCEIndex = ((ulTable_L2P.B.uwFEntry >> gub4kEntrysPerPlaneLog)&gubPlanesPerBurstMask) + ((M_Modular(((ulTable_L2P.B.uwFEntry >> gub4kEntrysPerPlaneLog) >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
						}
						UBYTE ubDepth, ubL4kLink;
						M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
						if ((ulTable_L2P.B.ubFUnitIndex == VT->gubTableTargetIndex) && ((ulTable_L2P.B.uwFEntry) > guwTable_LastEncodePlane)) {
							FQ->uwFJobInfo = ( BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY);
						}
						else {
							FQ->uwFJobInfo = ( BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | BIT_FJOBI_RS);
						}
						FQ->btBunchRSParity = 0;
						FQ->uwFUnit = VT->guwTableTarget[ulTable_L2P.B.ubFUnitIndex].B.uwTarget;
						FQ->ulFEntry = ulTable_L2P.B.uwFEntry;
						FQ->ubFJob = BYTE_FJOB_READ;
						FQ->ubCEIndex = ubCEIndex;
						FQ->btCheckEmpty = 0;
						FQ->btPreRead = 0;
						FQ->FQPara.B.btRWTableUnit = 1;
						FQ->ubDepth = ubDepth;
						FQ->ubL4kIndex = ubL4kLink;
						FQ->btUNServed = 1;
						FQ->btD3 = 1;
						FQ->ulRAMAddr = 0;
						FQ->ulSpare = uwL2PGroupIndex;
						gulPara0x24.BitMap.ubFrameNum = 1;
						gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
						gulPara0x24.BitMap.ubL4KNum = 1;
						gulPara0x24.BitMap.btZipBypass = 1;
						gulPara0x24.BitMap.btCmpEn = 1;
						gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
						gulPara0x24.BitMap.btBufModeEn = 0;
						gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
						gMTQ->Depth[ubDepth].ulZipMap = 0xFF;
						pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR) * Spare4kSize));
						//set L4K table info
						pL4KTable.ulL4K_LCA = uwL2PGroupIndex;
						pL4KTable.Para0x04.ulAll = 0; // for cpp check. Variable needs initial value.
						pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
						pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0;
						pL4KTable.Para0x0C.BitMap.ulL4K_BADR = 0;
						pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
						*pL4KTablePtr = pL4KTable;
						gFQI.ubFQLinkNumber++;
						gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
						while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
							M_SwitchTask();
						}
						FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
						gulTable_L2P[uwL2PGroupIndex].B.ubAllZero_BM = pL4KTablePtr->Para0x04.BitMap.ulL4K_FW;
					}
					gulTable_L2P[uwL2PGroupIndex].B.btNeedCheckAllZero = 0;
					ubMatchCount ++;
				}
				if ((SEL_CQBaseAddr[guwCQCnt_SE0 * 8] & SE0_CQ_SH_STOP) == 0) { //表示找完沒有了
					guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
					guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;
					break;
				}
				ulByteCount -= (SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + 7] - ulSearchBase);
				ulSearchBase = SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + 7];
				guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;
				guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;
			}

		Mark_Move_Table:
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
			ulTime[5] = gulOperationTime;
#endif
			// Scan GR前要先做table error handle
			ftlClearRAMForGR((U32)gulGRTable, 0);
		Mark_Sync_VC_EC:
			for (ulPlane_PTR = VTAREA_UNITS; ulPlane_PTR < guwFWTotalUnitNum; ulPlane_PTR ++) {
				if ((gulVC[ulPlane_PTR].B.ulValidCount != DefaultVC) && (ulPlane_PTR != VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget)) {
					gulEC_D3[ulPlane_PTR].B.btUsedDataUnit = 1;
				}
			}
			ftlCopyTable(VT->gubTableTargetIndex);
			if ((VT->guwOpenGRTarget.B.uwTarget < guwFWTotalUnitNum) && (VT->guwInitInfoOpenGRP2LPtr < guwFastPagePlanesPerUnit)) {
				ftlCopyInitInfo();
			}
			if (ENABLE_POWER_CYCLE_DONT_CNT_EVENT) {
			}
			mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 1);//強迫DMAC 全部做完
			gubSSD_Init_State.B.btCleanRAMForWriteDone = 1;
			gubSSD_Init_State.B.ubState = BYTE_INIT_TABLE_COPY_DONE;
		}
		else {
			if (VT->gulSLCPoolSize != gulSLCPoolSize_Sync) {
				VT->gulSLCPoolSize = gulSLCPoolSize_Sync;
			}
			ftlSaveVT(0, VT_INIT_FLASH_0x16);
			if (ENABLE_POWER_CYCLE_DONT_CNT_EVENT) {
			}
		}
	}
	ulScanNum[2] = VT->guwTableTargetPTR;
	ulScanNum[3] = 0;
	ulScanNum[4] = VT->gubGRTargetIndex;

Mark_SetGRVC_FromVT:
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	ulTime[6] = gulOperationTime;
#endif
	if (ENABLE_DEBUG_UART) {
		UartString(" GR");
	}

	UBYTE ubSaveVC_Prevent_PCPC_LostUnit = 0;
	for (ubi = 0; ubi < (VT->gubGRTargetIndex + 1); ubi ++) {
		if (gulVC[VT->guwGRTarget[ubi].B.uwTarget].B.ulValidCount == DefaultVC) {
			gulVC[VT->guwGRTarget[ubi].B.uwTarget].B.ulValidCount = 0;
			gulVC[VT->guwGRTarget[ubi].B.uwTarget].B.btFastPage = VT->guwGRTarget[ubi].B.btFastPage;
			ubSaveVC_Prevent_PCPC_LostUnit = 1;
		}
	}
	if (ubSaveVC_Prevent_PCPC_LostUnit) {
		ftlFlushL2PTable(1);
		ftlSaveVT(0, VT_INIT_FLASH_0x3F);
		if (ENABLE_POWER_CYCLE_DONT_CNT_EVENT) {
		}
	}
	if ((VT->gubGRTargetIndex == 0) && (VT->gulGRTableTempStartIndex > GRTable_Max_Collect_Num)) {
		ubPC_para |= PC_ACTIVE_GR_CIM;
	}

	U32 ulGRL4kNumInThisUnit;
	U32 ulGRL4KNum;
Mark_Load_GRTable:
	ulGRL4KNum = 0;
	ulGRTableSize = VT->gulGRTableStartIndex[0];
	//Get P2L to build GR Table
	for (ubScanGRTargetIndex = 0; ubScanGRTargetIndex < VT->gubGRTargetIndex; ubScanGRTargetIndex ++) {
		if (ubScanGRTargetIndex == 0) {
			VT->gulGRTableTempStartIndex -= VT->gulGRTableStartIndex[0];
			VT->gulGRTableStartIndex[0] = 0;
			ulGRTableSize = VT->gulGRTableStartIndex[0];
		}
		U32 ulPlanesOfThisUnit = gulVC[VT->guwGRTarget[ubScanGRTargetIndex].B.uwTarget].B.btFastPage ? guwFastPagePlanesPerUnit : gulPlanesPerUnit;
		U32 ulP2LPlanePtr = ulPlanesOfThisUnit - 2 - 1;
		UBYTE ubPlanesPerP2LTable = gubMaxPlanesPerP2LTable;
		U32 ulDMAAddr = (U32)gulGRTable + (ulGRTableSize * 4);
		UWORD uwUnit;
		UBYTE ubLoadP2LFromInitInfo = FALSE;
		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate((void *)ulDMAAddr, ubPlanesPerP2LTable * gub4kEntrysPerPlane * 4096);
		}
		uwUnit = VT->guwGRTarget[ubScanGRTargetIndex].B.uwTarget;
		if (uwUnit == VT->guwOpenGRTarget.B.uwTarget) {
			M_AssertError(VT->guwInitInfoOpenGRP2LPtr >= guwFastPagePlanesPerUnit);
			uwUnit = VT->guwInitInfoTarget.B.uwTarget;
			ulP2LPlanePtr = VT->guwInitInfoOpenGRP2LPtr;
			ubLoadP2LFromInitInfo = TRUE;
			uwFJobInfo = BIT_FJOBI_FASTPAGE;
			if (ENABLE_RUT_SWAP) {
				uwFJobInfo |= BIT_FJOBI_FAILHANDLE_RIGHTNOW;
			}
		}
		else {
			uwFJobInfo = (gulVC[uwUnit].B.btFastPage ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_RS;
		}

		for (ubPlaneIndex = 0; ubPlaneIndex < ubPlanesPerP2LTable; ubPlaneIndex++) {
			UBYTE ubL4KNum = gub4kEntrysPerPlane;
			UBYTE ubCEIndex;
			if (ubLoadP2LFromInitInfo == FALSE) {
				if (gulVC[VT->guwGRTarget[ubScanGRTargetIndex].B.uwTarget].B.btFastPage) {
					U32 ulP2LPlanePtrTemp = ulP2LPlanePtr;
					UWORD uwParityIndex = guwParityMappingCountSLC - 1;
					while (ulP2LPlanePtrTemp <= (guwParityMappingTableSLC[uwParityIndex] + 1)) {
						if (ulP2LPlanePtrTemp == (guwParityMappingTableSLC[uwParityIndex] + 1)) {
							ulP2LPlanePtrTemp -= 2;
						}

						if (uwParityIndex == 0) {
							break;
						}

						uwParityIndex--;
					}
					ulP2LPlanePtr = ulP2LPlanePtrTemp;
				}
				else {
					U32 ulP2LPlanePtrTemp = ulP2LPlanePtr;
					UWORD uwParityIndex = guwParityMappingCount - 1;
					while (ulP2LPlanePtrTemp <= (guwParityMappingTable[uwParityIndex] + 1)) {
						if (ulP2LPlanePtrTemp == (guwParityMappingTable[uwParityIndex] + 1)) {
							ulP2LPlanePtrTemp -= 2;
						}

						if (uwParityIndex == 0) {
							break;
						}

						uwParityIndex--;
					}
					ulP2LPlanePtr = ulP2LPlanePtrTemp;
				}
			}

			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ulP2LPlanePtr >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (ulP2LPlanePtr & gubPlanesPerBurstMask) + ((M_Modular((ulP2LPlanePtr >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			ubFQIndex[ubPlaneIndex] = gFQI.ubFQLinkFree;
			FQ->btCheckEmpty = 0;
			FQ->ulRAMAddr = ulDMAAddr + ((ubPlaneIndex << gubSectorsPerPlaneLog) * 512);
			FQ->btPreRead = 0;
			FQ->uwFUnit = uwUnit;
			FQ->ulFEntry = ulP2LPlanePtr << gub4kEntrysPerPlaneLog;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = uwFJobInfo | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY;//(gulVC[FQ->uwFUnit].B.btFastPage ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | BIT_FJOBI_RS;
			FQ->btBunchRSParity = 0;
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btNoWait = 1;
			FQ->btUNServed = 1;
			if (ubLoadP2LFromInitInfo) {
				FQ->btD3 = 1;
			}
			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + ((ubL4kLink * L4kQ_BIND_FACTOR) * Spare4kSize);
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

			UBYTE ubL4KCount;
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = 0;
				pL4KTable.Para0x04.ulAll = 0;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

			if (ubPlaneIndex == 0) {
				while (gFQI.gFQLink[ubFQIndex[ubPlaneIndex]].ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}
				gFQI.gFQLink[ubFQIndex[ubPlaneIndex]].ubFQPhase = BYTE_FQ_CHECKDONE;
				M_AssertCriticalError(FQ->btReadUNC, VT_ASSERT_INIT_FLASH_0x34);
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize));
				M_AssertCriticalError(pL4KTablePtr->ulL4K_LCA != (MARK_DataP2L | ubPlaneIndex), VT_ASSERT_INIT_FLASH_0x35);
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + 1) * Spare4kSize));
				ulGRL4kNumInThisUnit = pL4KTablePtr->ulL4K_LCA;
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + 2) * Spare4kSize));
				ubPlanesPerP2LTable = pL4KTablePtr->ulL4K_LCA;
			Mark_Check_GRTable_Remain_Size:
				if (((ubPlanesPerP2LTable << gubSectorsPerPlaneLog) * 512) == GRTableSize) {

				}
				else if (((ulGRTableSize * 4) + ((ubPlanesPerP2LTable << gubSectorsPerPlaneLog) * 512)) > (GRTableSize - (512 << gubSectorsPerPlaneLog))) {
					if (ENABLE_DEBUG_UART) {
						UartString("\nGRFull");
						Uart_Tx_DataHex(ubScanGRTargetIndex);
					}
					gubCleanGREndIndex = ubScanGRTargetIndex;
					ftlHandleGRTable(BIT_Initial_GR | BIT_Handle_GR_Full);
					if (VT->gulGRTargetMiddleP4kPTR) {
						//ftlDiscardGRearchLink(VT->gulGRTargetMiddleP4kPTR);
					}
					ulScanNum[3] += gulCleanGRGroupCount;
					ftlCleanGRTable(BIT_CleanGR | BIT_Finish_In_One_Time | BIT_CleanGR_Initial_Full);
					goto Mark_Load_GRTable;
				}
			}

			ulP2LPlanePtr--;

			if (ubLoadP2LFromInitInfo == FALSE) {
				Generate_RS_MAP_Index_by_Plane(ulP2LPlanePtr);
				if ((RS.uwPlaneOffset == (PARITY_DATA_PAGE_NUM - 1)) || (ulP2LPlanePtr == ((gulVC[FQ->uwFUnit].B.btFastPage ? RS.ulLastPlaneIndexFastPage[RS.ubGroupIndex] : RS.ulLastPlaneIndex[RS.ubGroupIndex]) - 1))) {
					ulP2LPlanePtr -= 2;
				}
			}
		}

		for (ubPlaneIndex = 1; ubPlaneIndex < ubPlanesPerP2LTable; ubPlaneIndex++) {
			while (gFQI.gFQLink[ubFQIndex[ubPlaneIndex]].ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			gFQI.gFQLink[ubFQIndex[ubPlaneIndex]].ubFQPhase = BYTE_FQ_CHECKDONE;
			M_AssertCriticalError(FQ->btReadUNC, VT_ASSERT_INIT_FLASH_0x36);
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex[ubPlaneIndex]].ubL4kIndex * L4kQ_BIND_FACTOR * Spare4kSize));
			M_AssertCriticalError(pL4KTablePtr->ulL4K_LCA != (MARK_DataP2L | ubPlaneIndex), VT_ASSERT_INIT_FLASH_0x37);
		}

	Mark_Calculate_Next_GRStartIndex:
		ulGRL4KNum += ulGRL4kNumInThisUnit;
		UBYTE ubGREntrysPer512B = 512 / 4;
		U32 ulIncNum = 0;
		if ((ubScanGRTargetIndex + 1) == VT->gubGRTargetIndex) {
			ulIncNum += GRTable_Max_Collect_Num; // 可以考慮一下有沒有需要預留...會做scan應該都是發生power cycle, 後面會做cleanGR
		}
		if ((ulGRTableSize + ulGRL4kNumInThisUnit) & (ubGREntrysPer512B - 1)) {
			// align 512 Byte
			ulIncNum += (ubGREntrysPer512B - ((ulGRTableSize + ulGRL4kNumInThisUnit) & (ubGREntrysPer512B - 1)));
		}
		VT->gulGRTableStartIndex[ubScanGRTargetIndex + 1] = (ulGRTableSize + ulGRL4kNumInThisUnit) + ulIncNum;
		ulGRTableSize = VT->gulGRTableStartIndex[ubScanGRTargetIndex + 1];
	}
	// ulGRL4KNum不一定會等於VT->gulGRTableL4kNum......因為只會收集需要 update L2P的
	// M_AssertError(VT->gulGRTableL4kNum != ulGRL4KNum);
	VT->gulGRTableL4kNum = ulGRL4KNum;

Mark_Check_GRTable_Remain_Size_Before_ScanActiveGR:
	if (VT->gubGRTargetIndex) {
		ulScanRegion = gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.btFastPage ? ((U32)gulFastPage4kEntrysPerUnit * gubMaxZipRate) : (gul4kEntrysPerUnit * gubMaxZipRate);
		if (((VT->gulGRTableStartIndex[VT->gubGRTargetIndex]  + ulScanRegion) * 4) > GRTableSize) {
			if (ENABLE_DEBUG_UART) {
				UartString("\nGRFull");
				Uart_Tx_DataHex(VT->gubGRTargetIndex);
			}
			gubCleanGREndIndex = VT->gubGRTargetIndex;
			ftlHandleGRTable(BIT_Initial_GR | BIT_Handle_GR_Full);
			if (VT->gulGRTargetMiddleP4kPTR) {
				//ftlDiscardGRearchLink(VT->gulGRTargetMiddleP4kPTR);
			}
			ulScanNum[3] += gulCleanGRGroupCount;
			ftlCleanGRTable(BIT_CleanGR | BIT_Finish_In_One_Time | BIT_CleanGR_Initial_Full);
			ulGRTableSize = VT->gulGRTableStartIndex[VT->gubGRTargetIndex];
		}
	}
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	ulTime[7] = gulOperationTime;
#endif

Mark_Load_ActiveGR_Parity:
	if ((VT->gulFTLState.B.btFlushCache && VT->gulGRTargetPTR) || (VT->gulFTLState.B.btDoingAbnormalInitial && VT->gulFTLState.B.btAbnormalInitialCopyDone)) {
		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}
		FCONL[FCONL_CRC_EN] &= ~CRC_32_EN;
		FCONL[FCONL_SGN_MSK] &= ~SIGNOFF_MASK_EN;
		UBYTE ubScannedParityNum = 0;
		UBYTE ubRSFrameIndex;
		UBYTE ubCheckPowerCycle = 0;
		for (ubRSFrameIndex = USERDATA_RS_Group_Num; ubRSFrameIndex != 0xFF ; ubRSFrameIndex--) {
			if (VT->guwRSFrameCount[ubRSFrameIndex]) {
				ubScannedParityNum += 2;
				for (ubPlaneIndex = 0; ubPlaneIndex < 2; ubPlaneIndex++) {
					UBYTE ubCEIndex;
					ulPlane_PTR = VT->guwInitInfoTargetPTR - ubScannedParityNum + ubPlaneIndex;
					if (ENABLE_PLANE_CH_CE) {
						ubCEIndex = M_Modular((ulPlane_PTR >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
					}
					else {
						ubCEIndex = (ulPlane_PTR & gubPlanesPerBurstMask) + ((M_Modular((ulPlane_PTR >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
					}
					UBYTE ubDepth, ubL4kLink;
					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->btCheckEmpty = 0;
					FQ->ulRAMAddr = ((U32)&guoRSFrameBits[((ubRSFrameIndex * 2 + ubPlaneIndex) * 4096 * (U32)gub4kEntrysPerPlane) / 8]);
					FQ->btPreRead = 0;
					FQ->uwFUnit = VT->guwInitInfoTarget.B.uwTarget;
					FQ->ulFEntry = ulPlane_PTR << gub4kEntrysPerPlaneLog;
					FQ->ubFJob = BYTE_FJOB_READ;
					FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | (ENABLE_RUT_SWAP ? BIT_FJOBI_FAILHANDLE_RIGHTNOW : 0));
					FQ->ubDepth = ubDepth;
					FQ->ubL4kIndex = ubL4kLink;
					FQ->btNoWait = 1;
					FQ->btUNServed = 1;
					FQ->btD3 = 1;

					gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
					gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
					gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
					gulPara0x24.BitMap.btZipBypass = 1;
					gulPara0x24.BitMap.btCmpEn = 0;
					gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
					gulPara0x24.BitMap.btBufModeEn = 0;
					gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
					gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

					UBYTE ubL4KCount;
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
					ubFQIndex[(ubRSFrameIndex * 2) + ubPlaneIndex] = gFQI.ubFQLinkFree;
					gFQI.ubFQLinkNumber++;
					gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
				}
			}
		}

		// 1CH1CE1P
		UBYTE ubRSFastPage = VT->guwGRTarget[VT->gubGRTargetIndex].B.btFastPage;
		UBYTE ubRSTag2Group[USERDATA_RS_Group_Num + 1];
		for (ubRSFrameIndex = 0; ubRSFrameIndex < (USERDATA_RS_Group_Num + 1) ; ubRSFrameIndex++) {
			ubRSTag2Group[ubRSFrameIndex] = 0xFF;
		}
		for (ubi = 0; ubi < USERDATA_RS_Group_Num ; ubi++) {
			if (ubRSFastPage) {
				Generate_RS_MAP_Index_by_Plane(RS.ulLastDataPlaneIndexFastPage[ubi]);
			}
			else {
				Generate_RS_MAP_Index_by_Plane(RS.ulLastDataPlaneIndex[ubi]);
			}
			ubRSTag2Group[RS.ubTagMAP[RS.ubGroupIndex][RS.ubTagIndex]] = ubi;
		}

		// here polling should keep the order of push FQ, or the MTQ might be full
		for (ubRSFrameIndex = USERDATA_RS_Group_Num; ubRSFrameIndex != 0xFF ; ubRSFrameIndex--) {
			if (VT->guwRSFrameCount[ubRSFrameIndex]) {
				for (ubPlaneIndex = 0; ubPlaneIndex < 2; ubPlaneIndex++) {
					FQ = &gFQI.gFQLink[ubFQIndex[(ubRSFrameIndex * 2) + ubPlaneIndex]];
					while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
						M_SwitchTask();
					}
					M_AssertError(FQ->btReadUNC);
					U32 ulSpareAddr = (U32)&gulRSFrameSpareBits[(ubRSFrameIndex * 2 + ubPlaneIndex) * (gub4kEntrysPerPlane * 4)];
					U32 ulIRAMAddr = L4K_TABLE_ADDR + (FQ->ubL4kIndex * L4kQ_BIND_FACTOR * Spare4kSize);
					if (ENABLE_DCACHE) {
						xthal_dcache_region_invalidate((void *)ulSpareAddr, Spare16kSize);
					}
					memcpy((void *)(ulSpareAddr), (void *)(ulIRAMAddr), Spare16kSize);
					if (ENABLE_DCACHE) {
						xthal_dcache_region_writeback_inv((void *)ulSpareAddr, Spare16kSize);
					}
					FQ->ubFQPhase = BYTE_FQ_CHECKDONE;


					if (ENABLE_RUT_SWAP && (uwUnit == VT->guwInitInfoTarget.B.uwTarget)) {
						if (FQ->btReadMove) {
							gubSpecialUnitReadMoveMap |= INITINFO_READMOVE;
						}
						if (FQ->btReadUNC) {
							gubSpecialUnitReadMoveMap |= INITINFO_READMOVE;
							VT->gulFTLState.B.btFlushCache = 0;
							ubCheckPowerCycle = 1;
							//goto Mark_Check_Power_Cycle_And_VT_State;
						}
					}

				}
				guwRSFrameDoneCount[ubRSFrameIndex] = 0;
				RSL[RSL_TAG_SEL] = ubRSFrameIndex;
				while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubRSFrameIndex);
				RSB[RSB_TAG_PEC] = VT->guwRSFrameCount[ubRSFrameIndex];

				// for 1CH 1CE 1Plane Case(RS Lastpage與RS Parity沒有連續)
				// 問題: 斷電於RS Lastpage寫完後, 重新給電時HW RS資訊(0x60~0x63 PEC.PRC.VALID)已不在,此時要寫RS Parity就會卡住
				// 解法: 設定RS_TAG_VALID_CLR 且寫parity時要將RSPageNum設 0xFF
				//M_AssertError(ubRSTag2Group[ubRSFrameIndex] >= USERDATA_RS_Group_Num);
				U32 ulLastDataPlaneIndex;
				if (ubRSTag2Group[ubRSFrameIndex] != 0xFF) {
					// get last data plane index of this tag
					if (ubRSFastPage) {
						ulLastDataPlaneIndex = RS.ulLastDataPlaneIndexFastPage[ubRSTag2Group[ubRSFrameIndex]];
					}
					else {
						ulLastDataPlaneIndex = RS.ulLastDataPlaneIndex[ubRSTag2Group[ubRSFrameIndex]];
					}
					Generate_RS_MAP_Index_by_Plane(ulLastDataPlaneIndex);
				}
				else {
					ulLastDataPlaneIndex = gulPlanesPerUnit;
				}


				gubForceProgramRSParity[ubRSFrameIndex] = 0;
				// 條件1: Unit結尾未滿252, 正準備寫Parity, 條件2: 滿252,正準備寫Parity
				if ((( VT->guwRSFrameCount[ubRSFrameIndex] >= (RS.uwPlaneOffset + 1) ) && (VT->gulGRTargetPTR > ulLastDataPlaneIndex))\
				        || (VT->guwRSFrameCount[ubRSFrameIndex] >= (guwParityPlanesNum - 3 + 1))) {
					gubForceProgramRSParity[ubRSFrameIndex] = 1;
				}


				RSL[RSL_HANG] |= RS_TAG_VALID_SET;
			}
		}
		FCONL[FCONL_CRC_EN] |= CRC_32_EN;
		FCONL[FCONL_SGN_MSK] |= SIGNOFF_MASK_EN;
		if (ubCheckPowerCycle) {
			goto Mark_Check_Power_Cycle_And_VT_State;
		}
	}
	else if ((VT->gulFTLState.B.btFlushCache == 0) && VT->gulGRTargetPTR) {
		UBYTE ubParityGroupIndex;
		for (ubParityGroupIndex = 0; ubParityGroupIndex < (USERDATA_RS_Group_Num + 1); ubParityGroupIndex++) {
			guwRSFrameDoneCount[ubParityGroupIndex] = 0xFF;
			VT->guwRSFrameCount[ubParityGroupIndex] = 0;
			RSL[RSL_TAG_SEL] = ubParityGroupIndex;
			while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubParityGroupIndex);
			RSL[RSL_TAG_VLD_CTL] |= RS_TAG_VALID_CLR;
			RSB[RSB_TAG_PEC] = VT->guwRSFrameCount[ubParityGroupIndex];
		}
	}

Mark_ScanActiveGR:
	if (VT->gulFTLState.B.btFlushCache) {
		ulScanRegion = VT->gulGRTargetPTR;
		if (VT->guwGRLastP2LPTR) {
			if (ftlLoadP2LTable(BYTE_P2L_No_Limit, VT->guwInitInfoTarget.B.uwTarget, VT->guwGRLastP2LPTR, (U32)&gulGRTable[ulGRTableSize]) != LONG_P2L_Load_Fail) {
				gulStartClearGRAddress = (U32)&gulGRTable[ulGRTableSize] + gubPlanesPerP2LTable * 4096 * gub4kEntrysPerPlane;
				gubRSCurrentTag = VT->gubRSCurrentTag;
				gubRSIdleTag = VT->gubRSIdleTag;
				gubRSShiftTagPhase = VT->gubRSShiftTagPhase;
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
				ulTime[8] = gulOperationTime;
#endif
				goto Mark_Initial_Variable_For_Zone_A_Random_B;
			}
			if (VT->gulFTLState.B.btFlushCache == 0) {
				goto Mark_Check_Power_Cycle_And_VT_State;
			}
		}
		gulStartClearGRAddress = (U32)gulGRTable;
		M_AssertError(ulScanRegion);
	}
	else { //Didn't save VT properly, need to scan whole last GR
		ulScanRegion = gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.btFastPage ? guwFastPagePlanesPerUnit : gulPlanesPerUnit;
		if (ENABLE_DEBUG_VC_CHECK) {
			ftlCheckVC(BIT_Scan_L2P_Check_VC);
		}
	}

#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
	// calculate gulBunchRSStartPlanePTR
	if ( VT->gulFTLState.B.btSATAFlushCommand == 1 && VT->gulBunchInitInfoTarget == VT->guwInitInfoTarget.B.uwTarget ) {
		if ( VT->gulGRTargetPTRBackup >= MAX_SHARE_PAGE_GAP * (U32)gubPlanesPerPage ) {
			gulBunchRSStartPlanePTR = VT->gulGRTargetPTRBackup - MAX_SHARE_PAGE_GAP * (U32)gubPlanesPerPage;
		}
		else {
			gulBunchRSStartPlanePTR = 0;
		}
	}
	else {
		gulBunchRSStartPlanePTR = 0xFFFFFFFF;
	}

	// set gubBunchRSCorrectFlag
#if ENABLE_GEN_FAIL==1

#if ENABLE_GEN_FAIL_WHEN_SCAN_ACTIVE_GR==1
	gubBunchRSCorrectFlag = 1;
	gubGenReadFailDebug = 1;
#else
	gubBunchRSCorrectFlag = 0;
#endif
#else
	gubBunchRSCorrectFlag = 1;
#endif

#endif

	ulPlane_PTR = 0;
	ubMax_buffer_Plane_Num = BUFFER2_NUM / gubSectorsPerPlane;
	UBYTE ubUNCBreakScanGR = 0;
	UBYTE ubReleaseRemianFQ = 0;
	UWORD uwGRParityCount[USERDATA_RS_Group_Num + 1] = {0};
	U32 ulActiveGRL4kPTR = 0;
	ULLONG uoSetRSMark_BM;

	while (ulPlane_PTR < ulScanRegion) {
		uoSetRSMark_BM = 0;
		for (ubNeedCheckFQIndex = 0; ubNeedCheckFQIndex < ubMax_buffer_Plane_Num; ubNeedCheckFQIndex ++) {
			if (ulPlane_PTR >= ulScanRegion) {
				break;
			}
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ulPlane_PTR >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (ulPlane_PTR & gubPlanesPerBurstMask) + ((M_Modular((ulPlane_PTR >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->btCheckEmpty = 1;
			FQ->btThisIsErasePage = 0;
			FQ->ulRAMAddr = BUFFER2_START + (512 << gubSectorsPerPlaneLog) * ubNeedCheckFQIndex;
			FQ->btPreRead = 0;
			FQ->uwFUnit = VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget;
			FQ->ulFEntry = ulPlane_PTR << gub4kEntrysPerPlaneLog;
			FQ->ubFJob = BYTE_FJOB_READ;
#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
			if ( ulPlane_PTR >= gulBunchRSStartPlanePTR && ulPlane_PTR < VT->gulGRTargetPTRBackup ) {
				FQ->uwFJobInfo = (gulVC[FQ->uwFUnit].B.btFastPage ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | BIT_FJOBI_RS;
				FQ->btBunchRSParity = 1;
			}
			else
#endif
			{
				FQ->uwFJobInfo = (gulVC[FQ->uwFUnit].B.btFastPage ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | BIT_FJOBI_RS;
				FQ->btBunchRSParity = 0;
			}
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btNoWait = 1;
			FQ->btUNServed = 1;

			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

			UBYTE ubL4KCount;
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = MARK_BeforeRead;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}
			ubFQIndex[ubNeedCheckFQIndex] = gFQI.ubFQLinkFree;
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
			ulPlane_PTR ++;
			// Calculate RS
			uwGRParityCount[gubRSCurrentTag]++;

			Generate_RS_MAP_Index_by_Plane(ulPlane_PTR);
			gubRSCurrentTag = RS.ubTagMAP[RS.ubGroupIndex][RS.ubTagIndex] + USERDATA_RSFrameStartIndex;
			if ((RS.uwPlaneOffset == (PARITY_DATA_PAGE_NUM - 2)) || (ulPlane_PTR == ((gulVC[FQ->uwFUnit].B.btFastPage ? RS.ulLastPlaneIndexFastPage[RS.ubGroupIndex] : RS.ulLastPlaneIndex[RS.ubGroupIndex]) - 2))) {
				uoSetRSMark_BM |= BIT0 << ubNeedCheckFQIndex;
				ulPlane_PTR += 2;
			}
		}

		ubNeedCheckFQNum = ubNeedCheckFQIndex;
		for (ubNeedCheckFQIndex = 0; ubNeedCheckFQIndex < ubNeedCheckFQNum; ubNeedCheckFQIndex ++) {
			UBYTE ubValidL4K_ThisPlane = 0;
			U32 ulStart_ActiveGRL4kPTR_ThisPlane = ulActiveGRL4kPTR;
			FQ = &gFQI.gFQLink[ubFQIndex[ubNeedCheckFQIndex]];
			while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
			if (ubReleaseRemianFQ) {
				continue;
			}
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize));
			if (FQ->btThisIsErasePage || ((pL4KTablePtr->ulL4K_LCA & MARK_DataP2L) == MARK_DataP2L)) {
				ubUNCBreakScanGR = 1;
			}
			else {
				UBYTE ubZByteNum = 0;
				UBYTE ubPreviousP4KUNC = 0;
				UBYTE ubZCodeInGRTable = 0;
				for (ubi = 0; ubi < gub4kEntrysPerPlane; ubi++) {
					P4KTable16B P4KTable = *(P4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR + ubi) * Spare4kSize));
					if ((P4KTable.ulL4K_LCA != MARK_BeforeRead) && (ubPreviousP4KUNC == 0)) {
						UBYTE ubSectorIndex = 0;
						U32 ulVir4kIndex = P4KTable.ulL4K_LCA >> SectorsPer4KLog;

						for (; ubSectorIndex < SectorsPer4K; ubSectorIndex++) {
							ubZByteNum++;
							// 3種條件表示已經組成一個L4K, 功德圓滿
							if ((ubZByteNum == SectorsPer4K)
							        || ((ubSectorIndex != (SectorsPer4K - 1)) && ((((UWORD)P4KTable.Para0x08.BitMap.uwP4k_Head >> ubSectorIndex)&BIT0) != (((UWORD)P4KTable.Para0x08.BitMap.uwP4k_Head >> (ubSectorIndex + 1))&BIT0)))
							        || ((ubSectorIndex == (SectorsPer4K - 1)) && (((P4KTable.Para0x08.BitMap.uwP4k_Head >> 8) & 7) == 0))) {

								if (ubZByteNum != SectorsPer4K) {
									U32 *ulBypassReadData = (U32*)(FQ->ulRAMAddr + (ubi * 4096) + (ubSectorIndex * 512) + 504);
									ulVir4kIndex = ulBypassReadData[0] >> SectorsPer4KLog;
								}
								if (gubL2P_FORMAT_MODE) {
									ubZCodeInGRTable = (ubZByteNum / 2) - 1;
								}
								else {
									ubZCodeInGRTable = ubZByteNum - 1;
								}

								if ((ulVir4kIndex & gulMask_NoValidData) != gulMask_NoValidData) {
									gulGRTable[ulActiveGRL4kPTR + ulGRTableSize].All = ulVir4kIndex | (ubZCodeInGRTable << gubZByteNumBitAddr) | NEED_UPDATE_L2P;
								}
								else {
									gulGRTable[ulActiveGRL4kPTR + ulGRTableSize].All = ulVir4kIndex | (ubZCodeInGRTable << gubZByteNumBitAddr);
								}
								ubZByteNum = 0;
								ulActiveGRL4kPTR ++;
								ubValidL4K_ThisPlane ++;
							}
						}
					}
					else { // UNC
						if (ubZByteNum) { // 前一個P4K無UNC, 但還有L4K需與目前的P4K湊, 不過目前的P4K UNC, 需幫前一個補zcode
							if (gubL2P_FORMAT_MODE) {
								ubZCodeInGRTable = (ubZByteNum / 2) - 1;
							}
							else {
								ubZCodeInGRTable = ubZByteNum - 1;
							}
							gulGRTable[ulActiveGRL4kPTR + ulGRTableSize].All = gulTable_Dummy | (ubZCodeInGRTable << gubZByteNumBitAddr);
							ubZByteNum = 0;
							ulActiveGRL4kPTR ++;
						}
						gulGRTable[ulActiveGRL4kPTR + ulGRTableSize].All = gulTable_Dummy | (gubZBytePer4kEntry << gubZByteNumBitAddr);
						ulActiveGRL4kPTR ++;
						ubPreviousP4KUNC = 1;
						ubUNCBreakScanGR = 1;
						if (ulPlane_PTR < VT->gulGRTargetPTR) {
							if (VT->guwDebugActiveGR < 0xFFFF) {
								VT->guwDebugActiveGR++;
							}
						}
					}
				}
			}

			if (ubUNCBreakScanGR) {
				M_AssertError(VT->gulFTLState.B.btAbnormalInitialCopyDone && (VT->gulGRTargetPTR != (FQ->ulFEntry >> gub4kEntrysPerPlaneLog)) && (VT->gulGRTargetPTR != gulPlanesPerUnit));
				if (VT->gulFTLState.B.btAbnormalInitialCopyDone == 0) {
					VT->gulGRTargetPTR = (FQ->ulFEntry >> gub4kEntrysPerPlaneLog);
				}
				if (ubValidL4K_ThisPlane) { //此plane有部分valid L4K, 應該把last plane ++
					VT->gulGRTargetPTR ++;
				}
				else { //此plane 全部 UNC, 應該把 ulActiveGRL4kPTR 往回扣
					//ulActiveGRL4kPTR = ulStart_ActiveGRL4kPTR_ThisPlane;
					while (ulActiveGRL4kPTR > ulStart_ActiveGRL4kPTR_ThisPlane) {
						ulActiveGRL4kPTR --;
						gulGRTable[ulActiveGRL4kPTR + ulGRTableSize].All = gulTable_Empty;
					}

					if (ENABLE_DEBUG_RW) {
						for (ubi = 0; ubi < gub4kEntrysPerPlane; ubi ++) {
							M_AssertError(gulGRTable[ulActiveGRL4kPTR + ulGRTableSize + ubi].All != gulTable_Empty);
						}
					}
				}
				if (ENABLE_DEBUG_UART) {
					UartString(" LastPlane");
					UartLongHex(VT->gulGRTargetPTR);
					UartString("LastL4K");
					UartLongHex(ulActiveGRL4kPTR);
				}
				ubReleaseRemianFQ = 1;
			}
			else {
				if (uoSetRSMark_BM & (BIT0 << ubNeedCheckFQIndex)) {
					for (ubi = 0; ubi < (gub4kEntrysPerPlane * 2); ubi++) {
						gulGRTable[ulActiveGRL4kPTR + ulGRTableSize].All = gulTable_Parity | (gubZBytePer4kEntry << gubZByteNumBitAddr);
						ulActiveGRL4kPTR ++;
					}
				}
			}
		}
		if (ubUNCBreakScanGR) {
			break;
		}
	}

#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
	// clear gubBunchRSCorrectFlag
	gubBunchRSCorrectFlag = 0;
	gubGenReadFailDebug = 0;
#endif

	// 因為middle cleanGR, 之後要把tempGR搬回來, 受到 SS 32Byte align限制, 會多補dummy
	// 所以initial scan時有可能 ulActiveGRL4kPTR < VT->gulGRTableL4kPTR
	if ((VT->gubGRTargetIndex == 0) && VT->gulGRTableTempL4KPTRIncNum) {
		VT->gulGRTableTempStartIndex -= VT->gulGRTableTempL4KPTRIncNum;
		//M_AssertError((ulActiveGRL4kPTR + VT->gulGRTableTempL4KPTRIncNum) < VT->gulGRTableL4kPTR);
		// 有可能save VT後發生pwercycle打死share page, 所以不能加上列的debug code, 因為每次cleanGR前都有補share page, 應該不用擔心資料不見
		VT->gulGRTableTempL4KPTRIncNum = 0;
	}
	VT->gulGRTableL4kPTR = ulActiveGRL4kPTR;
	//掃出來沒有資料, 但VC不等於零, 無法處理......已經有補share page不應該發生
	M_AssertCriticalError((VT->gulGRTableL4kPTR == 0) && gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.ulValidCount, VT_ASSERT_INIT_FLASH_0x38);
	if (VT->gulFTLState.B.btDoingAbnormalInitial && VT->gulFTLState.B.btAbnormalInitialCopyDone) {
		gubRSCurrentTag = VT->gubRSCurrentTag;
		gubRSIdleTag = VT->gubRSIdleTag;
		gubRSShiftTagPhase = VT->gubRSShiftTagPhase;
	}
	ulScanNum[5] = VT->gulGRTargetPTR;
	ulScanNum[6] = VT->gulGRTableStartIndex[VT->gubGRTargetIndex] + VT->gulGRTableL4kPTR;
	//ulScanNum[5] = VT->guwGRTarget[VT->gubGRTargetIndex].B.btFastPage;
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	ulTime[8] = gulOperationTime;
#endif
	if (ENABLE_DEBUG_UART) {
		UartString("\nRS");
		UBYTE ubParityGroupIndex;
		for (ubParityGroupIndex = 0; ubParityGroupIndex < (USERDATA_RS_Group_Num + 1); ubParityGroupIndex++) {
			UartString(" ");
			Uart_Tx_DataHex(VT->guwRSFrameCount[ubParityGroupIndex]);
		}
		UartString(" Tag");
		Uart_Tx_DataHex(gubRSCurrentTag);
		UartString(" ");
		Uart_Tx_DataHex(gubRSIdleTag);
		UartString(" ");
		Uart_Tx_DataHex(gubRSShiftTagPhase);
	}

	if ((VT->gulFTLState.B.btFlushCache == FALSE) && (VT->gubWriteProtectState != BYTE_STATE_WRITEPROTECT)) {
	Mark_HandlePowerCycle:
		gubPowerCycling = 1;
		ftlPowerCycle(ubPC_para);
		if (ENABLE_POWER_CYCLE_DONT_CNT_EVENT) {
		}
	}
	else {
	Mark_Build_GRSerachLink_and_UpdateL2P:
		ftlHandleGRTable(BIT_Initial_GR);
	}

Mark_Initial_Variable_For_Zone_A_Random_B:
	while (AtaCfg.ubSecurityEraseIsDoing && VT->gulFTLState.B.btNeedCloseTarget) {	//For Security Erase APC 0x6e Issue
		ftlCloseTarget(BIT_Finish_In_One_Time);
	}
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	ulTime[9] = gulOperationTime;
#endif
	/*================================Initial起來全部當Random區, XferDataIn後才開始建B區======================================*/
	gubZcodeA =  8;
	gulRandomZcodeZoneBegin =  0; // Random區開始位置
	gulRandomZcodeZoneBegin_PhyEntry = (VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget * gulSectorsPerUnit); // Phy < gulRandomZcodeZoneBegin_PhyEntry的為Zcode A的區域
	gubZcodeB =  8;
	gulZoneBBegin =  gulRandomZcodeZoneBegin;

	if ((((SYSL[SYS0L_SIM_CTRL0] & SIMREG_CMD_D2H) == 0) || (SYSL[SYS0L_SIM_CTRL0] & SIMREG_COMRESET_JUMP)) && (SYSB[0x44] != SIMREG_DEVSLP_LOAD_CODE)) {
		if (!gubPowerCycling) {
			VT->gulPowerOnCnt ++;
		}

		if (VT->gulFTLState.B.btLast_CMD_Not_StandBy) {
			VT->gulNonStandByPowerDownCnt ++;
		}
		else {
			VT->gulFTLState.B.btLast_CMD_Not_StandBy = 1;
		}
	}

	if (gubPowerCycling) {
		gubPowerCycling = 0;
		gubSSD_Init_State.B.ubState = BYTE_INIT_SCAN_DONE_PC;
	}
	else {
		if (gubAdjust_SLCPool_Enable) {
			if (gulAdjust_SLCPool_Size > VT->gulSLCPoolSizeMax) {
				VT->gulSLCPoolSize += (gulAdjust_SLCPool_Size - VT->gulSLCPoolSizeMax);
				VT->gulSLCPoolSizeMax = gulAdjust_SLCPool_Size;
			}
			else if ((gulAdjust_SLCPool_Size < VT->gulSLCPoolSizeMax) && (VT->gulSLCPoolSize == VT->gulSLCPoolSizeMax)) {
				VT->gulSLCPoolSize = gulAdjust_SLCPool_Size;
				VT->gulSLCPoolSizeMax = gulAdjust_SLCPool_Size;
			}
		}
		gubSSD_Init_State.B.ubState = BYTE_INIT_SCAN_DONE;
	}

	// Variable for guarantee flush
	gulFlushCachePrevGRTablePTR = VT->gulGRTargetPTR;
#if (ENABLE_DEBUG_UART_INIT_TIME || ENABLE_DEBUG_UART_INIT_TIME_II)
	if (VT->gulMaxInitTime < (ulTime[9] - ulTime[0])) {
		VT->gulMaxInitTime = (ulTime[9] - ulTime[0]);
	}
#endif
	if (ENABLE_DEBUG_UART) {
		UartString(" InitDone");
		UartLongHex(VT->gulPowerOnCnt);
	}
#if (ENABLE_DEBUG_UART_INIT_TIME)
	UBYTE ubTimeIndex;
	UartString("\nftlInit_t ");
	for (ubTimeIndex = 0; ubTimeIndex < 7; ubTimeIndex ++) {
		UartLongHex(ulTime[ubTimeIndex + 1] - ulTime[ubTimeIndex]); // VT RUT VTChild Table Table_GC GR PowerCycle
		UartString(" ");
	}
	UartString("\nPTR ");
	for (ubTimeIndex = 0; ubTimeIndex < 5; ubTimeIndex ++) {
		UartLongHex(ulScanNum[ubTimeIndex]); // VT VTChild Table GR_PTR GR_L4k_PTR
		UartString(" ");
	}
	UartString("SLC");
	Uart_Tx_DataHex(ulScanNum[5]); //active GR SLC or MLC
	UartString("\nftlInitTotal_T");
	UartLongHex((ulTime[7] - ulTime[0]));
	UartString(" Max");
	UartLongHex(VT->gulMaxInitTime);
#elif (ENABLE_DEBUG_UART_INIT_TIME_II)
	UartString("\n");
	UBYTE ubTimeIndex;
	for (ubTimeIndex = 0; ubTimeIndex < 9; ubTimeIndex ++) {
		//Uart_Tx_DataHex(ubTimeIndex);
		//UartString(".");
		UartLongHex(ulTime[ubTimeIndex + 1] - ulTime[ubTimeIndex]); // VT RUT VTChild Table Table_GC GR PowerCycle
		UartString(",");
	}
	UartLongHex(VT->gulMaxInitTime);
	UartString(",");
	for (ubTimeIndex = 0; ubTimeIndex < 7; ubTimeIndex ++) {
		//Uart_Tx_DataHex(ubTimeIndex);
		//UartString(".");
		UartLongHex(ulScanNum[ubTimeIndex]); // VT RUT VTChild Table Table_GC GR PowerCycle
		UartString(",");
	}
#endif

	if (ENABLE_DEBUG_RW) {
		// 檢查有沒有unit經過power cycle後便失蹤兒童
		UBYTE ubi, ubSkip;
		UBYTE ubLostHappen = 0;
		UWORD uwScanUnit;
		UWORD uwScanUnitIndex;
		UWORD uwGetCount = 0;

		for (uwScanUnitIndex = VTAREA_UNITS; uwScanUnitIndex < guwFWTotalUnitNum; uwScanUnitIndex ++) {
			if (gulVC[uwScanUnitIndex].B.ulValidCount != DefaultVC) {
				uwGetCount ++;
			}
		}
		if (uwGetCount != (VT->guwFreeBlockCountMax - VT->guwFreeBlockCount_D3)) {
			UartString("\nFree");
			UartWordHex(uwGetCount);
			UartString(" VT_Free");
			UartWordHex((VT->guwFreeBlockCountMax - VT->guwFreeBlockCount_D3));
			ubLostHappen = 1;
		}

		for (uwScanUnitIndex = 2; uwScanUnitIndex < (VT->guwFreeBlockCount_D3 + 2); uwScanUnitIndex++) {
			guwTemp = uwScanUnitIndex;
			if (gulVC[guwFreeBlockTable_D3[uwScanUnitIndex].B.uwTarget].B.ulValidCount != DefaultVC) {
				UartString("\nWrongVCUnit");
				UartWordHex(uwScanUnitIndex);
				ubLostHappen = 1;
			}
		}

		for (uwScanUnit = VTAREA_UNITS; uwScanUnit < guwFWTotalUnitNum; uwScanUnit ++) {
			guwTemp = uwScanUnit;
			ubSkip = 0;
			for (ubi = 0; ubi < gubTableUnitNum; ubi ++) {
				if (VT->guwTableTarget[ubi].B.uwTarget == uwScanUnit) {
					ubSkip = 1;
					break;
				}
			}
			if (ubSkip ||
			        (VT->guwRUTTarget.B.uwTarget == uwScanUnit) ||
			        (VT->guwInitInfoTarget.B.uwTarget == uwScanUnit) ||
			        (VT->guwVTChildTarget.B.uwTarget == uwScanUnit)) {
				continue;
			}

			if ((gulVC[uwScanUnit].B.ulValidCount == DefaultVC) && (M_RUT2DTo1D(0, uwScanUnit << gubBurstsPerBankLog) != 0xBBBB)) {
				ubi = 0;
				for (uwScanUnitIndex = 2; uwScanUnitIndex < (VT->guwFreeBlockCount_D3 + 2); uwScanUnitIndex++) {
					if (guwFreeBlockTable_D3[uwScanUnitIndex].B.uwTarget == uwScanUnit) {
						ubi = 1;
						break;
					}
				}
				if (ubi == 0) {
					UartString("\nLostUnit");
					UartWordHex(uwScanUnit);
					ubLostHappen = 1;
				}
			}
		}
		M_AssertError(ubLostHappen);
	}

	VT->gubFlashTypeFail |= (gubFlashTypeFail & 0x3);
	VT->gubFlashTypeFail |= (gubScanVTReboot) ? BIT2 : 0;
	if (ENABLE_WRITEPROTECT) {
		if (VT->gubWriteProtectState != BYTE_STATE_WRITEPROTECT) {
			ftlSaveVT(0, VT_INIT_FLASH_0x40);
			if (ENABLE_POWER_CYCLE_DONT_CNT_EVENT) {
			}
		}
	}
	else {
		ftlSaveVT(0, VT_INIT_FLASH_0x17);
		if (ENABLE_POWER_CYCLE_DONT_CNT_EVENT) {
		}
	}
#endif

#if TLC
	{
		/* Gwyn+ Temp solution */
		UBYTE ubNeedSaveVTForD1 = 0;
		if ((ENABLE_MULTI_DIE && guoFlashUseType.B.btMultiDie && gubParaInitialized) && (gubSelDieNumber > 1)) {
			UBYTE ubDieIndex;
			for (ubDieIndex = 0; ubDieIndex < gubSelDieNumber; ubDieIndex++) {
				while (VT->gubD1DieCount[ubDieIndex] <= FreeD1CountThatNeedWriteData) {
					ftlFlushD1(BIT_Finish_In_One_Time | BIT_Search_By_Die, ubDieIndex, BYTE_NO_NEED_SAVE_VT);
					ubNeedSaveVTForD1 = 1;
				}
			}
		}
		else {
			while (VT->guwFreeBlockCount_D1 <= FreeD1CountThatNeedWriteData) {
				ftlFlushD1(BIT_Finish_In_One_Time, 0, BYTE_NO_NEED_SAVE_VT);
				ubNeedSaveVTForD1 = 1;
			}
		}
		if (ubNeedSaveVTForD1) {
			ftlSaveVT(0, VT_INIT_FLASH_0x21);
			if (ENABLE_POWER_CYCLE_DONT_CNT_EVENT) {
			}
		}
	}
#endif
	if (0) {
		gubTemp = BIT_Load_P2L_Check_VC | BIT_Scan_L2P_Check_VC;
		ftlCheckVC(gubTemp);
	}
#endif

#if (!BURNER)
	if (VT->gulGRTargetPTR < (gulVC[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.btFastPage ? guwFastPagePlanesPerUnit : gulPlanesPerUnit)) {

		if (VT->gulGRTargetPTR >= (guwRSPlanesPerPage * USERDATA_RS_Group_Num)) {
			UBYTE ubRSTag;
			UBYTE ubGroupBitmap[((VAR_RS_GROUP_NUM >> 3) + 1) << 3] = {0};
			UWORD uwListIndex;
			U32 ulTmp;
			U32 ulPlanePTR = VT->gulGRTargetPTR;
			ubi = 0;
			while (1) {
				Generate_RS_MAP_Index_by_Plane_flafail(ulPlanePTR);
				if (RS_FLAFAIL.ubTagIndex == 0) {
					break;
				}
				if ((ubGroupBitmap[RS_FLAFAIL.ubGroupIndex >> 3] & (BIT0 << (RS_FLAFAIL.ubGroupIndex & 7))) == 0) {
					ubGroupBitmap[RS_FLAFAIL.ubGroupIndex >> 3] |= (BIT0 << (RS_FLAFAIL.ubGroupIndex & 7));
					// find index of page link list
					uwListIndex = (RS_FLAFAIL.ubTagIndex * PARITY_DATA_PAGE_NUM) / RS.uwPlanesPerGroup;
					// get RS tag
					ubRSTag = RS.ubTagMAP[RS_FLAFAIL.ubGroupIndex][RS_FLAFAIL.ubTagIndex];
					// get start plane index
					ulTmp = RS_FLAFAIL.ulPageLinkList[RS_FLAFAIL.ubGroupIndex][uwListIndex] * RS.uwPlanesPerGroup;
					// adjust to real plane offset
					gulRSEncodeStartPlaneNo[ubRSTag] = ulTmp + ((RS_FLAFAIL.ubTagIndex * PARITY_DATA_PAGE_NUM) % RS.uwPlanesPerGroup);

					ubi++;
					if (ubi == RS_GROUP_NUM) {
						// all group done
						break;
					}
				}
				M_AssertError(ulPlanePTR < RS.uwPlanesPerGroup);
				ulPlanePTR = ((ulPlanePTR / RS.uwPlanesPerGroup) * RS.uwPlanesPerGroup) - 1;
			}
		}
	}
#endif
	return 0;
}

void ftlScanVT(UBYTE ubMode, UBYTE ubStartVT) // 1 : VT, 0 : VT Child
{
	M_CheckRDTModeToDisableFunction();
	UBYTE ubCEIndex;
	UBYTE ubUnitIndex;
	UBYTE ubVTUnitIndex = ubStartVT;
	UBYTE ubUnitRegion = ubMode ? VTAREA_UNITS : 1;
	UBYTE ubPlaneIndex;
	UBYTE ubGetLast = 0;
	UBYTE ubCheckTime;
	UBYTE ubIsErase;
	UBYTE ubErasedPlaneNum = 0;
	UBYTE ubErasedPlaneCheckNum = (ENABLE_VT_MIDDLE_STOP_CHECK) ? ((3 * gubPlanesPerVT) << ENABLE_VT_BACKUP) : 1;
	U32 ulPlanePTR;
	U32 ulSpareMark = ubMode ? MARK_VT : MARK_VTChild;
	FlashQueue_t *FQ;
	U32 ulValidPTR = 0, ulInValidPTR = 0, ulInValidCnt = 0; //超過兩組UNC or UNC之後有Valid 都卡下來
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	Variable_Table *SavedVT = (Variable_Table *)(BUFFER2_START + (512 << gubSectorsPerPlaneLog) * gubPlanesPerVT);
	mDMAC_SV(1, (U32)SavedVT, (512 << gubSectorsPerPlaneLog) * gubPlanesPerVT, 0, 0, 0, DMA_EN_SETCQ, 1);

	for (ubUnitIndex = 0; ubUnitIndex < ubUnitRegion; ubUnitIndex ++, ubVTUnitIndex ++) {
		if (ubVTUnitIndex >= VTAREA_UNITS) {
			ubVTUnitIndex = 0;
		}
		ulPlanePTR = 0;
		ubErasedPlaneNum = 0;
		ubIsErase = FALSE;
		if (ENABLE_DEBUG_UART_VT_INIT) {
			UartString("\nSVTL,U:");
			if (ubMode) UartWordHex(VT->guwVTAreaFreeTarget[ubVTUnitIndex].B.uwTarget);
			else UartWordHex(VT->guwVTChildTarget.B.uwTarget);
		}
		while (ulPlanePTR < guwFastPagePlanesPerUnit) {
			ubCheckTime = 1 << ENABLE_VT_BACKUP;
			while (ubCheckTime) {
				ubPlaneIndex = 0;
				while (ubPlaneIndex < gubPlanesPerVT) {
					if (ENABLE_PLANE_CH_CE) {
						ubCEIndex = M_Modular((ulPlanePTR >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
					}
					else {
						ubCEIndex = (ulPlanePTR & gubPlanesPerBurstMask) + ((M_Modular((ulPlanePTR >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
					}
					UBYTE ubDepth, ubL4kLink;
					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->btCheckEmpty = 1;
					FQ->btThisIsErasePage = 0;
					FQ->ulRAMAddr = BUFFER2_START + (512 << gubSectorsPerPlaneLog) * ubPlaneIndex;
					FQ->btPreRead = 0;
					FQ->uwFUnit = ubMode ? VT->guwVTAreaFreeTarget[ubVTUnitIndex].B.uwTarget : VT->guwVTChildTarget.B.uwTarget;
					FQ->ulFEntry = ulPlanePTR << gub4kEntrysPerPlaneLog;
					FQ->ubFJob = BYTE_FJOB_READ;
					FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | (ENABLE_RUT_SWAP ? BIT_FJOBI_FAILHANDLE_RIGHTNOW : 0));
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

					if (ENABLE_DCACHE) {
						xthal_dcache_region_invalidate((void *)FQ->ulRAMAddr, gub4kEntrysPerPlane * 4096);
					}
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

					if (ENABLE_RUT_SWAP) {
						if (FQ->btReadMove) {
							gubSpecialUnitReadMoveMap |= (ubMode ? VT_READMOVE : VTCHILD_READMOVE);
						}
					}

					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR) * Spare4kSize));
					U32 ulL4K_LCA = pL4KTablePtr->ulL4K_LCA;
#if FixShallowErase
					if (FQ->btThisIsErasePage || FQ->btReadUNC || (ulL4K_LCA == (gulTable_Dummy << SectorsPer4KLog)))
#else
					if (FQ->btThisIsErasePage || FQ->btReadUNC || (ulL4K_LCA == 0x69B68392)) //沒被寫過 or UNC
#endif
					{
						if (ENABLE_DEBUG_UART_VT_INIT) {
							UartString(" ");
							UartWordHex(ulPlanePTR);
							UartString("/");
							if (FQ->btThisIsErasePage) Uart_Tx_DataHex(ubErasedPlaneNum);
							else if (FQ->btReadUNC) UartString("u");
							else if (ulL4K_LCA == (gulTable_Dummy << SectorsPer4KLog)) UartString("t");
							else UartLongHex(ulL4K_LCA);
						}
						if (FQ->btThisIsErasePage) {
							ubErasedPlaneNum++;
							if (ubErasedPlaneNum == ubErasedPlaneCheckNum) {
								ubIsErase = TRUE;
							}
						}
						else {
							ubErasedPlaneNum = 0;
							ubIsErase = FALSE;
							if (ubCheckTime == 1) { //第二次Check
								ulInValidPTR = ulPlanePTR;
								ulInValidCnt++;
								M_AssertError(ulInValidCnt > 1); //超過兩組 UNC
							}
						}
						ulPlanePTR += (gubPlanesPerVT - ubPlaneIndex);
						ubCheckTime --; // 再檢查一次
						break;
					}
					else if (ulL4K_LCA == ulSpareMark) {
						ulPlanePTR ++;
						ubPlaneIndex++;
						if (ubPlaneIndex == gubPlanesPerVT) {
							ubErasedPlaneNum = 0;
							ubIsErase = FALSE;
							mDMAC_COPY(BUFFER2_START, (U32)SavedVT, (512 << gubSectorsPerPlaneLog) * gubPlanesPerVT, 0, 0, 0, 0, 1);
							M_AssertError(SavedVT->gulVTLastMark != 0x55AA);
							ulValidPTR = ulPlanePTR;
							M_AssertError(ulInValidPTR);//UNC之後,有Valid的
							if (ubCheckTime == 2) { // 第一次就讀到了
								ulPlanePTR += gubPlanesPerVT;
							}
							ubCheckTime = 0;
						}
					}
					else {
#if (!BURNER)
						if (gubScanVTReboot == 0) {
							ReservedData->ubSecMode = 0;
							ReservedData->ubScanVTReboot = 1;
							ReservedData->CRC = mDMAC_CRC((U32)ReservedData, 512, ReservedData_SEED, 1, (DMA_CRC_LEN_8 | DMA_CRC_DCCM));
							Vender_Isp_Jump(0);
						}
						else if (gubScanVTReboot == 1)
#endif
						{
							M_AssertCriticalError(1, VT_ASSERT_SCAN_VT_0x70);
						}
					}
				}
			}
			if (ubIsErase) { // the conditions of skip scanning this unit: 1. found erase page 2. scan over whole unit
				break;
			}
		}
		if (ubGetLast) { // 前一個Unit已經找到過，這次不管找不找到都不必再找
			break;
		}
		else if (SavedVT->gulVTLastMark == 0x55AA) {
			ubGetLast = TRUE;
		}
	}

	if (ubGetLast) {
		mDMAC_COPY((U32)SavedVT, (U32)VT, (512 << gubSectorsPerPlaneLog) * gubPlanesPerVT, 0, 0, 0, 0, 1);
		guwVTTarget.B.uwTarget = VT->guwVTAreaFreeTarget[VT->gubVTAreaFreeQueueInPTR].B.uwTarget;
	}
	else {
		M_AssertError(ubMode == 1);
	}

	if (!BURNER) {
		if (VT->gulVTLastMark != 0x55AA) {

			if (gubScanVTReboot == 0) {
				ReservedData->ubSecMode = 0;
				ReservedData->ubScanVTReboot = 1;
				ReservedData->CRC = mDMAC_CRC((U32)ReservedData, 512, ReservedData_SEED, 1, (DMA_CRC_LEN_8 | DMA_CRC_DCCM));
				Vender_Isp_Jump(0);
			}
			else if (gubScanVTReboot == 1) {
				M_AssertCriticalError(VT->gulVTLastMark != 0x55AA, VT_ASSERT_SCAN_VT_0x39);
			}
		}
	}
}



#if FixShallowErase
UWORD ftlScanUnit_Binary(UBYTE ubMode, UWORD uwUnit) // [Mode] BIT2: FORCE DIRECT ACCESS, BIT1 : D3,  BIT0: D1
{

	UBYTE ubCEIndex;
	UBYTE ubPlaneIndex;
	UBYTE ubFind;
	UBYTE ubScanTime = 0;
	//Because scantime index is count up from 0 at below, so we don't need to consider the odd CE conditon.
	UBYTE ubScanTimeMax = (ubMode & BIT_D1_TARGET) ? gubPlanesPerUnitLog : gubFastPagePlanesPerUnitLog;
	U32 ulPlanePTR;
	U32 ulHeadGroup;
	U32 ulMidGroup;
	U32 ulTailGroup;
	U32 ulLastMatchGroup;
	UWORD uwLastMatchUnit = 0xFFFF;
	U32 ulLastMatchPlane = 0xFFFF;
	FlashQueue_t *FQ;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;

	// Wait all previous FQ done
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
Mark_Scan:

	FCONL[FCONL_CRC_EN] &= ~CRC_32_EN;
	// Scan D1 should set index within 1536, because we need to scan 3 D1 unit which scan 512 pages, the index will pass to FLA and it will transfered.
	// Scan D3 should only scan 512 pages, because shallow erase only happened on SLC mode D3 unit.
	if (ubMode & BIT_D1_TARGET) {
		ulTailGroup = gulPlanesPerUnit;
	}
	else {
		ulTailGroup = guwFastPagePlanesPerUnit;
	}
	ulHeadGroup = 0;
	ulLastMatchGroup = 0xFFFFFFFF;
	ubScanTime = 0;
	while (1) {
		ulMidGroup = (ulTailGroup + ulHeadGroup) >> 1; // Mid = (tail + head) / 2, binary search
		if ((ulLastMatchGroup == ulMidGroup) || (ubScanTime > ubScanTimeMax)) {
			break;
		}
		ulPlanePTR = ulMidGroup;
	Mark_Read:
		ubPlaneIndex = 0;
		ubFind = 0;
		M_DebugGPIO4_High();
		while ((ubFind == 0) && (ubPlaneIndex < 1)) {
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ulPlanePTR >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (ulPlanePTR & gubPlanesPerBurstMask) + ((M_Modular((ulPlanePTR >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->btCheckEmpty = 1;
			FQ->btThisIsErasePage = 0;
			FQ->ulRAMAddr = BUFFER2_START;
			FQ->btPreRead = 0;
			//FQ->uwFUnit = (ubMode & D3)? uwUnit : M_RUT2DTo1D(ubCEIndex, ((guwFWTotalUnitNum + (uwUnit * D1_UNIT_NUM_PER_LINK) + ubIndex) << gubBurstsPerBankLog));
			FQ->uwFUnit = uwUnit;
			FQ->ulFEntry = (ulPlanePTR + ubPlaneIndex) << gub4kEntrysPerPlaneLog;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | ((ubMode & BIT_D1_TARGET) ? 0 : BIT_FJOBI_FASTPAGE) | ((ubMode & BIT_DIRECT_ACCESS) ? BIT_FJOBI_DIRECTACESS : 0));
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btNoWait = 1;
			FQ->btUNServed = 1;
			FQ->btD3 = (ubMode & BIT_D3_TARGET) ? D3 : D1;

			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gulPara0x24.BitMap.ubL4KNum = gub4kEntrysPerPlane;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

			UBYTE ubL4KCount;
			for (ubL4KCount = 0; ubL4KCount < gub4kEntrysPerPlane; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = MARK_BeforeRead;
				pL4KTable.Para0x04.ulAll = 0; // for cpp check. Variable needs initial value.
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
			while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize));
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((FQ->ubCEIndex & gubPlanesPerBurstMask) * 0x200));

			if (FQ->btThisIsErasePage) {
				gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
			}
			else {				// Found not empty.
				if (ubFind == 0) {
					ubFind = 1;
					ulLastMatchGroup = ulMidGroup;
					uwLastMatchUnit = FQ->uwFUnit;
					ulLastMatchPlane = (FQ->ulFEntry >> gub4kEntrysPerPlaneLog);
				}
			}
			ubPlaneIndex++;
		}
		ubScanTime ++;
		M_DebugGPIO4_Low();
		if (ubFind) {
			ulHeadGroup = ulMidGroup;
		}
		else {
			ulTailGroup = ulMidGroup;
		}
	}

	FCONL[FCONL_CRC_EN] |= CRC_32_EN;

	return ulLastMatchPlane;



}
#endif
void ftlScanVT_Binary(UBYTE ubMode, U32 ulUnit_PlanePTR) // 1 : VT, 0 : VT Child
{
	M_CheckRDTModeToDisableFunction();
	//M_CheckBurnerModeToDisableFunction();
	UBYTE ubCEIndex;
	UBYTE ubUnitIndex;
	UBYTE ubUnitRegion = ubMode ? VTAREA_UNITS : 1;
	UBYTE ubPlaneIndex;
	UBYTE ubCheckTime = 1 << ENABLE_VT_BACKUP;
	UBYTE ubFind;
	UBYTE ubScanTime = 0;
	UBYTE ubLastMatchVTIndex = 0xFF;
	//Because scantime index is count up from 0 at below, so we don't need to consider gubFastPagePalnesPerUnitLog about the odd CE conditon.
	UBYTE ubScanTimeMax = gubFastPagePlanesPerUnitLog - ENABLE_VT_BACKUP;
	U32 ulPlanePTR;
	UWORD uwHeadGroup;
	UWORD uwMidGroup;
	UWORD uwTailGroup;
	UWORD uwLastMatchGroup;
	UWORD uwLastMatchUnit = 0xFFFF;
	UWORD uwLastMatchPlane = 0xFFFF;
	U32 ulSpareMark = ubMode ? MARK_VT : MARK_VTChild;
	FlashQueue_t *FQ;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	U32 ulVTPTRShallowerase[2] = {0};
	UWORD uwVTShallowerase[2] = {0xFFFF, 0xFFFF};
	UBYTE ubVTFoundCnt = 0;
	// Wait all previous FQ done
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
	if (ENABLE_DEBUG_HW_FORCEREADFAIL) {
		gubGenReadFailDebug = STOP_GEN_FAIL; //CY_TEMP
	}
Mark_Scan:
	if (ulUnit_PlanePTR == 0xFFFFFFFF) {
#if !MicronFlashOnly
		// Micron SLC block need retry
		FALLL[FCTLL_OTHER_SET] &= ~BCH_COR_EN;
		FALLL[FCTLL_LDPC_CFG] &= ~LDPC_COR_EN;
		FCONL[FCONL_CRC_EN] &= ~CRC_32_EN;
#endif
		for (ubUnitIndex = 0; ubUnitIndex < ubUnitRegion; ubUnitIndex ++) {
			uwHeadGroup = 0;
			uwTailGroup = (guwFastPagePlanesPerUnit >> ENABLE_VT_BACKUP) / gubPlanesPerVT; // 總共幾份
			uwLastMatchGroup = 0xFFFF;
			ubScanTime = 0;
			while (1) {
				uwMidGroup = (uwTailGroup + uwHeadGroup) >> 1; // Mid = (tail + head) / 2, binary search
				if ((uwLastMatchGroup == uwMidGroup) || (ubScanTime > ubScanTimeMax)) {
					if (uwLastMatchGroup == uwMidGroup) {
						ubVTFoundCnt++;
					}
					break;
				}
				ulPlanePTR = (uwMidGroup << ENABLE_VT_BACKUP) * gubPlanesPerVT;
			Mark_Read:
				ubPlaneIndex = 0;
				ubFind = 0;
				UBYTE ubCorrectNum[2] = {0};
				M_DebugGPIO4_High();
				while ((ubFind == 0) && (ubPlaneIndex < (gubPlanesPerVT * ubCheckTime))) {
					if (ENABLE_PLANE_CH_CE) {
						ubCEIndex = M_Modular((ulPlanePTR >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
					}
					else {
						ubCEIndex = (ulPlanePTR & gubPlanesPerBurstMask) + ((M_Modular((ulPlanePTR >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
					}
					UBYTE ubDepth, ubL4kLink;
					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->btCheckEmpty = 1;
					FQ->btThisIsErasePage = 0;
					FQ->ulRAMAddr = BUFFER2_START;
					FQ->btPreRead = 0;
					FQ->uwFUnit = ubMode ? VT->guwVTAreaFreeTarget[ubUnitIndex].B.uwTarget : VT->guwVTChildTarget.B.uwTarget;
					FQ->ulFEntry = (ulPlanePTR + ubPlaneIndex) << gub4kEntrysPerPlaneLog;
					FQ->ubFJob = BYTE_FJOB_READ;
#if MicronFlashOnly
					FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY);
#else
					FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT);
#endif
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

					UBYTE ubL4KCount;
					for (ubL4KCount = 0; ubL4KCount < gub4kEntrysPerPlane; ubL4KCount++) {
						pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
						//set L4K table info
						pL4KTable.ulL4K_LCA = MARK_BeforeRead;
						pL4KTable.Para0x04.ulAll = 0; // for cpp check. Variable needs initial value.
						pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
						pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
						pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0;
						pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
						*pL4KTablePtr = pL4KTable;
					}
					gFQI.ubFQLinkNumber++;
					gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
					while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
						M_SwitchTask();
					}
					FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((FQ->ubL4kIndex * L4kQ_BIND_FACTOR) * Spare4kSize));
					gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((FQ->ubCEIndex & gubPlanesPerBurstMask) * 0x200));
#if((!BURNER) && (!MicronFlashOnly))
					if (((gFREG[FCTLL_MAP_CFG].L & CURRENT_FIFO_HAS_NEW) && (gFREG[FCTLL_INT_INF].L & ERASE_PAGE)) || (gubPreformat && FQ->btThisIsErasePage))  //沒被寫過
#else
					if (FQ->btThisIsErasePage)
#endif
					{
						gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
					}
					else {//if (pL4KTablePtr->ulL4K_LCA == ulSpareMark) {
#if MicronFlashOnly
						if (pL4KTablePtr->ulL4K_LCA == ulSpareMark)
#endif
						{
							if (ubPlaneIndex < gubPlanesPerVT) {
								ubCorrectNum[0] ++;
							}
							else {
								ubCorrectNum[1] ++;
							}
							if ((ubFind == 0) && ((ubCorrectNum[0] == gubPlanesPerVT) || (ubCorrectNum[1] == gubPlanesPerVT))) {
								ubFind = 1;
								uwLastMatchGroup = uwMidGroup;
								uwLastMatchUnit = FQ->uwFUnit;
								uwLastMatchPlane = (FQ->ulFEntry >> gub4kEntrysPerPlaneLog) + 1 - gubPlanesPerVT;
								ubLastMatchVTIndex = ubUnitIndex;
							}
						}
					}
					ubPlaneIndex++;
				}
				ubScanTime ++;
				M_DebugGPIO4_Low();
				if (ubFind) {
					uwHeadGroup = uwMidGroup;
				}
				else {
					uwTailGroup = uwMidGroup;
				}
			}
			if (ubVTFoundCnt == 1) {
				uwVTShallowerase[0] = uwLastMatchUnit;
				ulVTPTRShallowerase[0] = uwLastMatchPlane;                                  // first found unit.
			}
			else if ((ubVTFoundCnt == 2) && (uwVTShallowerase[0] != uwLastMatchUnit)) {
				gubDoubleVT = 1;                                                            // second found unit.
				if ((uwLastMatchUnit == (VTAREA_UNITS - 1)) && (uwVTShallowerase[0] == 0)) {
					// need swap
					uwVTShallowerase[1] = uwVTShallowerase[0];
					ulVTPTRShallowerase[1] = ulVTPTRShallowerase[0];
					uwVTShallowerase[0] = uwLastMatchUnit;
					ulVTPTRShallowerase[0] = uwLastMatchPlane;
					uwLastMatchUnit = uwVTShallowerase[1];
					uwLastMatchPlane = ulVTPTRShallowerase[1];
				}
				else {
					uwVTShallowerase[1] = uwLastMatchUnit;
					ulVTPTRShallowerase[1] = uwLastMatchPlane;
				}
			}
			else if (ubVTFoundCnt > 2) {
				M_AssertError(1);                                                           // suppose 2 VT exissts at most.
			}
		}
		FALLL[FCTLL_OTHER_SET] |= BCH_COR_EN;
		FALLL[FCTLL_LDPC_CFG] |= LDPC_COR_EN;
		FCONL[FCONL_CRC_EN] |= CRC_32_EN;
	}
	else {
		uwLastMatchUnit = (ulUnit_PlanePTR & 0xFFFF0000) >> 16;
		uwLastMatchPlane = ulUnit_PlanePTR & 0xFFFF;
	}

	if (uwLastMatchUnit == 0xFFFF) {
		// 只有VT 一定要 scan 到, VT Child可以是空的
		M_AssertCriticalError(ubMode, VT_ASSERT_SCAN_VT_BINARY_0x3A);
	}
	else {
		if (ENABLE_VT_MIDDLE_STOP_CHECK) {
			UBYTE ubScanGroupRegion, ubScanPlaneRegion;
			UWORD uwCurrentGroup, uwPlanePtr;

			ubScanGroupRegion = 3;
			uwCurrentGroup = uwLastMatchPlane / (gubPlanesPerVT << ENABLE_VT_BACKUP);
			uwPlanePtr = (uwCurrentGroup + 1) * (gubPlanesPerVT << ENABLE_VT_BACKUP);
			if (guwFastPagePlanesPerUnit - uwPlanePtr > ubScanGroupRegion * (gubPlanesPerVT << ENABLE_VT_BACKUP)) {
				ubScanPlaneRegion = ubScanGroupRegion * (gubPlanesPerVT << ENABLE_VT_BACKUP);
			}
			else {
				ubScanPlaneRegion = guwFastPagePlanesPerUnit - uwPlanePtr;
			}
			if (ENABLE_DEBUG_UART_VT_INIT) {
				UartString("\nMid:");
				Uart_Tx_DataHex(ubMode);
				Uart_Tx_DataHex(FixShallowErase);
				UartString(" ");
				UartWordHex(uwLastMatchUnit);
				UartString(" ");
				UartWordHex(uwPlanePtr);
				UartString("~");
				UartWordHex(uwPlanePtr + ubScanPlaneRegion);
			}

			for (ubPlaneIndex = 0; ubPlaneIndex < ubScanPlaneRegion; ubPlaneIndex++) {
				if (ENABLE_PLANE_CH_CE) {
					ubCEIndex = M_Modular(((uwPlanePtr + ubPlaneIndex) >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
				}
				else {
					ubCEIndex = ((uwPlanePtr + ubPlaneIndex) & gubPlanesPerBurstMask) + ((M_Modular(((uwPlanePtr + ubPlaneIndex) >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
				}
				UBYTE ubDepth, ubL4kLink;
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->btCheckEmpty = 1;
				FQ->btThisIsErasePage = 0;
				FQ->ulRAMAddr = BUFFER2_START;
				FQ->btPreRead = 0;
				FQ->uwFUnit = uwLastMatchUnit;
				FQ->ulFEntry = ((U32)uwPlanePtr + ubPlaneIndex) << gub4kEntrysPerPlaneLog;
				FQ->ubFJob = BYTE_FJOB_READ;
				FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY);
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

				if ((FQ->btThisIsErasePage == 0) && ((ulL4K_LCA == ulSpareMark) || (FQ->btReadUNC))) {
					// there is a VT after the uwLastMatchPlane
					if (ENABLE_DEBUG_UART_VT_INIT) {
						UartString(" FindXVT:");
						UartWordHex(uwLastMatchUnit);
						UartString("/");
						UartWordHex(uwPlanePtr + ubPlaneIndex);
					}
					ftlScanVT(ubMode, ubLastMatchVTIndex);
					gubSSD_Init_State.B.btVTLastPlaneState = 1; // UNC, VT/VTChild要換新unit
					goto Mark_End_Of_Scanning;
				}
				else {
					if (ENABLE_DEBUG_UART_VT_INIT) {
						UartString(" ");
						Uart_Tx_DataHex(ubPlaneIndex);
						if (FQ->btThisIsErasePage) UartString("e");
						else if (FQ->btReadUNC) UartString("u");
						else if (ulL4K_LCA != ulSpareMark) UartLongHex(ulL4K_LCA);
					}
				}
			}
		}
		if (ubMode == 1) {
			guwVTTarget.B.uwTarget = uwLastMatchUnit;
		}
	Mark_Read_From_Flash:
		ubPlaneIndex = 0;
		while (ubPlaneIndex < gubPlanesPerVT) {
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((uwLastMatchPlane >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (uwLastMatchPlane & gubPlanesPerBurstMask) + ((M_Modular((uwLastMatchPlane >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->btCheckEmpty = 0;
			FQ->ulRAMAddr = BUFFER2_START + ((512 << gubSectorsPerPlaneLog) * ubPlaneIndex);
			FQ->btPreRead = 0;
			FQ->uwFUnit = uwLastMatchUnit;
			FQ->ulFEntry = ((U32)uwLastMatchPlane + ubPlaneIndex) << gub4kEntrysPerPlaneLog;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY | (ENABLE_RUT_SWAP ? BIT_FJOBI_FAILHANDLE_RIGHTNOW : 0));
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
			if ((ulL4K_LCA == ulSpareMark) && (FQ->btReadUNC == 0)) {
				ubPlaneIndex++;
				if (ubPlaneIndex == gubPlanesPerVT) {
					mDMAC_COPY(BUFFER2_START, (U32)VT, (512 << gubSectorsPerPlaneLog) * gubPlanesPerVT, 0, 0, 0, 0, 1);
				}
				if (ENABLE_RUT_SWAP) {
					if (FQ->btReadMove) {
						gubSpecialUnitReadMoveMap |= (ubMode ? VT_READMOVE : VTCHILD_READMOVE);
					}
				}
			}
			else {
				if (ENABLE_DEBUG_UART || ENABLE_DEBUG_UART_VT_INIT) {
					UartString(" Again");
					UartWordHex(uwLastMatchUnit);
					UartString(" ");
					UartWordHex(uwLastMatchPlane);
				}
				ftlScanVT(ubMode, ubLastMatchVTIndex);
				gubSSD_Init_State.B.btVTLastPlaneState = 1; // UNC, VT/VTChild要換新unit
				break;
			}
		}
	}

Mark_End_Of_Scanning:
	if (ENABLE_DEBUG_HW_FORCEREADFAIL) {
		gubGenReadFailDebug = 0;
	}
	// 20161227 turn off temp, when check empty -> slc erase -> slc varify is good, then remove the outer save dummy(ftlInit), save here.
	if (ubMode && gubDoubleVT) {
		ulVTPTRShallowerase[0] += 2;
		ulVTPTRShallowerase[1] += 2;
		if (guwVTTarget.B.uwTarget == uwVTShallowerase[0]) {
			gubDoubleVT = 2;
#if FixShallowErase
			if (ulVTPTRShallowerase[1] < (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div))) {
				ftlForceSaveDummy(ulVTPTRShallowerase[1], (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div)), uwVTShallowerase[1],
				                  FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE, D3_AREA, VT_FORCE_DUMMY_0xD5);
			}
#endif
		}
		else if (guwVTTarget.B.uwTarget == uwVTShallowerase[1]) {
#if FixShallowErase
			if (ulVTPTRShallowerase[0] < (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div))) {
				ftlForceSaveDummy(ulVTPTRShallowerase[0], (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div)), uwVTShallowerase[0],
				                  FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE, D3_AREA, VT_FORCE_DUMMY_0xD6);
			}
#endif
		}
		else {
			M_AssertError(1);
		}
	}

	if (VT->guwOpenGRTarget.B.uwTarget == 0) { // DLMC from the old version
		M_AssertError(VT->guwInitInfoOpenGRP2LPtr != 0);
		VT->guwOpenGRTarget.B.uwTarget = Default_Unit;
		VT->guwInitInfoOpenGRP2LPtr = 0xFFFF;
	}
	if (VT->guwOldVTChildTarget.B.uwTarget == 0) { // DLMC from the old version
		VT->guwOldVTChildTarget.B.uwTarget = Default_Unit;
	}
	if (!BURNER) {
		M_AssertCriticalError(VT->gulVTLastMark != 0x55AA, VT_ASSERT_SCAN_VT_BINARY_0x3B);
	}
}


#if (!BURNER)
#if (ENABLE_DPS)
void DPS_Command(UBYTE *ubData, UBYTE ubMode)
{
	if (gubDPSTableValid) {
		UBYTE ubCMDLength;
		UBYTE ubDPS_Stage;
		UWORD uwDataOffset;
#if TLC
		UBYTE ubDieNumber;
		for (ubDieNumber = 1; ubDieNumber < ((gubDieNumber * gubExpandCE_Ratio) + 1); ubDieNumber++) {
			///Check Stage and give offset
			if ((ubMode & BIT_DPS_Initial) && VT->gulGRTargetPTR) {
				ubDPS_Stage = VT->gubDPS_Stage;
			}
			else {
				U32 ulAverageEraseCount = VT->gulTotalEraseCount / guwFWTotalUnitNum;
				if (ulAverageEraseCount <= (U32)(gubDPSTable[4] << 8 | gubDPSTable[3]) ) {
					ubDPS_Stage = 0;
				}
				else if (ulAverageEraseCount <= (U32)(gubDPSTable[6] << 8 | gubDPSTable[5]) ) {
					ubDPS_Stage = 1;
				}
				else if (ulAverageEraseCount < (U32)(gubDPSTable[8] << 8 | gubDPSTable[7]) ) {
					ubDPS_Stage = 2;
				}
				else if (ulAverageEraseCount < (U32)(gubDPSTable[10] << 8 | gubDPSTable[9]) ) {
					ubDPS_Stage = 3;
				}
				else {
					ubDPS_Stage = 4;
				}

				if ((ubMode & BIT_DPS_Check_Level) && (VT->gubDPS_Stage == ubDPS_Stage)) {
					return;
				}
				else if (VT->gubDPS_Stage != ubDPS_Stage) {
					VT->gubDPS_Stage = ubDPS_Stage;
				}
			}

			while (gFQI.ubFQLinkNumber) {
				M_SwitchTask();
			}
			UBYTE ubFlashClock = gubFlashClock;
			UBYTE ubFlashMode = gubFlashMode;
			DPS_SpeedDown();
			//if (ubMode & BIT_DPS_PowerOn_Reset) {
			DPS_TLC_Power_On_Reset(ubDieNumber);
			//}
			ubCMDLength = gubDPSTable[11 + ubDPS_Stage];
			uwDataOffset = 141 + (120 * ubDPS_Stage);
			DPS_TLC_Parameter_out(21, ubData, ubDieNumber , ubCMDLength);
			DPS_TLC_Parameter_set(21, uwDataOffset, ubData, ubDieNumber , ubCMDLength);
			DPS_TLC_Parameter_out(21, ubData, ubDieNumber , ubCMDLength);
			DPS_SpeedBack(ubFlashClock, ubFlashMode);
		}
#if TLC_BICS2
		flaDisable_IPR();
#endif

#else
		///Check Stage and give offset
		if ((ubMode & BIT_DPS_Initial) && VT->gulGRTargetPTR) {
			ubDPS_Stage = VT->gubDPS_Stage;
			if (ENABLE_DEBUG_UART_DPS) {
				UartString("\nDPSInit1 ");
				Uart_Tx_DataHex(ubDPS_Stage);
			}
		}
		else {
			U32 ulAverageEraseCount = VT->gulTotalEraseCount / guwFWTotalUnitNum;
			if (ulAverageEraseCount <= (U32)(gubDPSTable[4] << 8 | gubDPSTable[3]) ) {
				ubDPS_Stage = 0;
			}
			else {
				ubDPS_Stage = 1;
			}
			if ((ubMode & BIT_DPS_Check_Level) && (VT->gubDPS_Stage == ubDPS_Stage)) {
				if (ENABLE_DEBUG_UART_DPS) {
					UartString("\nSameDPS");
				}
				return;
			}
			else if (VT->gubDPS_Stage != ubDPS_Stage) {
				VT->gubDPS_Stage = ubDPS_Stage;
			}
			if (ENABLE_DEBUG_UART_DPS) {
				UartString("\nDPSInit2 ");
				Uart_Tx_DataHex(ubDPS_Stage);
			}
		}

		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}
		UBYTE ubFlashClock = gubFlashClock;
		UBYTE ubFlashMode = gubFlashMode;
		DPS_SpeedDown();
		//if (ubMode & BIT_DPS_PowerOn_Reset) {
		DPS_MLC_Power_On_Reset();
		//}
		ubCMDLength = gubDPSTable[11 + ubDPS_Stage];
		uwDataOffset = 141 + (120 * ubDPS_Stage);
		DPS_MLC_Parameter_out(21, ubData , ubCMDLength);
		DPS_MLC_Parameter_set(21, uwDataOffset, ubData , ubCMDLength);
		DPS_MLC_Parameter_out(21, ubData , ubCMDLength);
		DPS_SpeedBack(ubFlashClock, ubFlashMode);
#endif
	}
	gubSSD_Init_State.B.btSetDPSDone = 1;
}

void DPS_TLC_Power_On_Reset(UBYTE ubDieNumber)
{
	UBYTE ubj;
	UBYTE ubCounter = 1;
	U32  ulOperationTime;
	UBYTE ubCEPerChannel = (gubMTQTotalPhyCE >> gubPlanesPerBurstLog);
	_UCV REG_t * pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];

	/// Enable All CE
	FCONL[FCONL_FCE_ENB] = 0;
	for (ubj = 0; ubj < ubCEPerChannel; ubj++) {
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubj]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubj]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = ((BIT0 << ubj) | (BIT8 << ubj));
		}
		//}

		//	for (ubM = 0; ubM < gubDieNumber; ubM++) {
		//Test Mode Entry
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5C;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xC5;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
		pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x00;

		pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
		pFlaReg[FCTLL_PIO_DAT].L = 0x01;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {

				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
		//Reset
		pFlaReg[FCTLL_PIO_CMD].B.B0 = (0xF0 | ubDieNumber);
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x00;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFD;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {

				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}

		//Exit
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFF;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}
}

void DPS_TLC_Parameter_set(UWORD uwAddrOffset, UWORD uwDataOffset , UBYTE* ubData , UBYTE ubDieNumber , UBYTE ubDPS_TRIMLEN_TLC)
{
	UBYTE ubj;
	UBYTE ubL;
	UBYTE ubCounter = 1;
	U32 ulOperationTime;
	UBYTE ubCEPerChannel = (gubMTQTotalPhyCE >> gubPlanesPerBurstLog);
	UWORD* uwDPSTableAddr = (UWORD*)(gubDPSTable + uwAddrOffset);
	UWORD* uwDPSTableData = (UWORD*)(gubDPSTable + uwDataOffset);
	_UCV REG_t * pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];

	FCONL[FCONL_FCE_ENB] = 0;
	for (ubj = 0; ubj < ubCEPerChannel; ubj++) {
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubj]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubj]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = ((BIT0 << ubj) | (BIT8 << ubj));
		}
		//}
		//-----------------------parameter set------------------------//
		//Test Mode Entry
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5c;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xc5;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
		pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x00;

		pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
		pFlaReg[FCTLL_PIO_DAT].L = 0x01;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}
	//Address Set/Loop
	for (ubj = 0; ubj < (ubDPS_TRIMLEN_TLC); ubj++) {
		if (uwDPSTableData[ubj] == 0xffff) {
			continue;
		}

		//		for (ubM = 0; ubM < ubDieNumber; ubM++) {
		for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				//CE Decode can not trigger more than one CE in each channel.
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
			}

			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
			pFlaReg[FCTLL_PIO_CMD].B.B0 = (0xF0 | ubDieNumber);
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x56;
			pFlaReg[FCTLL_PIO_ADR].B.B0 = uwDPSTableAddr[ubj];//addr

			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[0];
			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			if ((uwDPSTableData[ubj] & 0x7000) == 0x0) {   /// not constant
				if (uwDPSTableData[ubj] & 0x8000) /// minus
					pFlaReg[FCTLL_PIO_DAT].L = ubData[ubL * ubDPS_TRIMLEN_TLC * 2 * gubDieNumber + ubDieNumber * ubDPS_TRIMLEN_TLC * 2 + ubj] - (uwDPSTableData[ubj] & 0xFF);
				else
					pFlaReg[FCTLL_PIO_DAT].L = ubData[ubL * ubDPS_TRIMLEN_TLC * 2 * gubDieNumber + ubDieNumber * ubDPS_TRIMLEN_TLC * 2 + ubj] + (uwDPSTableData[ubj] & 0xFF);
			}
			else {
				pFlaReg[FCTLL_PIO_DAT].L = (uwDPSTableData[ubj] & 0xFF);
			}

			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[1];
			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			if ((uwDPSTableData[ubj] & 0x7000) == 0x0) {   /// not constant
				if (uwDPSTableData[ubj] & 0x8000) /// minus
					pFlaReg[FCTLL_PIO_DAT].L = ubData[ubL * ubDPS_TRIMLEN_TLC * 2 * gubDieNumber + ubDieNumber * ubDPS_TRIMLEN_TLC * 2 + ubDPS_TRIMLEN_TLC + ubj] -  (uwDPSTableData[ubj] & 0xFF);
				else
					pFlaReg[FCTLL_PIO_DAT].L = ubData[ubL * ubDPS_TRIMLEN_TLC * 2 * gubDieNumber + ubDieNumber * ubDPS_TRIMLEN_TLC * 2 + ubDPS_TRIMLEN_TLC + ubj] +  (uwDPSTableData[ubj] & 0xFF);
			}
			else {
				pFlaReg[FCTLL_PIO_DAT].L =  (uwDPSTableData[ubj] & 0xFF);
			}
			if (gubDPSEnableUseTimeOut) {
				ulOperationTime = gulOperationTime;
				while (ubCounter) {
					if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
						ubCounter = 0;
					}
					else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
						ubCounter = 0;
					}
				}
				ubCounter = 1;
			}
			else {
				while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
			}
		}
		//		}
		//FCONL[FCONL_FCE_ENB] = 0xFFFF;
		//pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
	}

	//Exit
	for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
		}

		pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFF;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}
}

void DPS_TLC_Parameter_out(UWORD uwAddrOffset, UBYTE* ubData, UBYTE ubDieNumber , UBYTE ubDPS_TRIMLEN_TLC)
{
	UBYTE ubCounter = 1;
	U32 ulOperationTime;
	UBYTE ubj;
	UBYTE ubL;
	UBYTE ubCEPerChannel = (gubMTQTotalPhyCE >> gubPlanesPerBurstLog);
	UWORD* uwDPSTableAddr = (UWORD*)(gubDPSTable + uwAddrOffset);
	_UCV REG_t * pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];

	FCONL[FCONL_FCE_ENB] = 0;
	for (ubj = 0; ubj < ubCEPerChannel; ubj++) {
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubj]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubj]);
		}
		else {
			FCONL[FCONL_FCE_ENB] |= ((BIT0 << ubj) | (BIT8 << ubj));
		}
		//}
		if (ENABLE_DEBUG_UART_DPS) {
			UartString("\r\n");
			UartString("ModifiedPara:");
		}
		//Test Mode Entry
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5C;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xC5;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
		pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x00;
		pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
		pFlaReg[FCTLL_PIO_DAT].L = 0x01;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}
	//Address Set/Loop
	for (ubj = 0; ubj < ubDPS_TRIMLEN_TLC; ubj++) {

		//		for (ubM = 0; ubM < ubDieNumber; ubM++) {
		if (ENABLE_DEBUG_UART_DPS) {
			UartString("\r\nAddr");
			Uart_Tx_DataHex(uwDPSTableAddr[ubj]);
			UartString(" ");
		}
		for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				//CE Decode can not trigger more than one CE in each channel.
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
			}
			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
			pFlaReg[FCTLL_PIO_CMD].B.B0 = (0xF0 | ubDieNumber);

			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
			pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x01;

			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			pFlaReg[FCTLL_PIO_DAT].L = 0x06;
			if (gubDPSEnableUseTimeOut) {
				ulOperationTime = gulOperationTime;
				while (ubCounter) {
					if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
						ubCounter = 0;
					}
					else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
						ubCounter = 0;
					}
				}
				ubCounter = 1;
			}
			else {
				while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
			}

			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5F;
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x00;
			pFlaReg[FCTLL_PIO_ADR].B.B0 = uwDPSTableAddr[ubj];//addr
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5F;
			if (gubDPSEnableUseTimeOut) {
				ulOperationTime = gulOperationTime;
				while (ubCounter) {
					if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
						ubCounter = 0;
					}
					else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
						ubCounter = 0;
					}
				}
				ubCounter = 1;
			}
			else {
				while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
			}
		}
		for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				//CE Decode can not trigger more than one CE in each channel.
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
			}
			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[0];
			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			ubData[ubL * ubDPS_TRIMLEN_TLC * 2 * gubDieNumber + ubDieNumber * ubDPS_TRIMLEN_TLC * 2 + ubj] = (pFlaReg[FCTLL_PIO_DAT].L & 0xFF);
			if (ENABLE_DEBUG_UART_DPS) {
				Uart_Tx_DataHex(ubData[ubL * ubDPS_TRIMLEN_TLC * 2 * gubDieNumber + ubDieNumber * ubDPS_TRIMLEN_TLC * 2 + ubj]);
				UartString(" ");
			}
			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[1];
			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			ubData[ubL * ubDPS_TRIMLEN_TLC * 2 * gubDieNumber + ubDieNumber * ubDPS_TRIMLEN_TLC * 2 + ubDPS_TRIMLEN_TLC + ubj] = (pFlaReg[FCTLL_PIO_DAT].L & 0xFF);
			if (ENABLE_DEBUG_UART_DPS) {
				Uart_Tx_DataHex(ubData[ubL * ubDPS_TRIMLEN_TLC * 2 * gubDieNumber + ubDieNumber * ubDPS_TRIMLEN_TLC * 2 + ubDPS_TRIMLEN_TLC + ubj]);
				UartString(" ");
			}
		}


		for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				//CE Decode can not trigger more than one CE in each channel.
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
			}
			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];

			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5F;
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x00;
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5F;
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5F;
			if (gubDPSEnableUseTimeOut) {
				ulOperationTime = gulOperationTime;
				while (ubCounter) {
					if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
						ubCounter = 0;
					}
					else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
						ubCounter = 0;
					}
				}
				ubCounter = 1;
			}
			else {
				while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
			}
		}
		//		}
	}

	//Exit

	for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
		}
		pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFF;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}
}

///===================below are DPS setting==================
void DPS_MLC_Power_On_Reset(void)
{
	UBYTE ubCounter = 1;
	U32 ulOperationTime;
	UBYTE ubj;
	UBYTE ubCEPerChannel = (gubMTQTotalPhyCE >> gubPlanesPerBurstLog);
	_UCV REG_t * pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];

	/// Enable All CE
	FCONL[FCONL_FCE_ENB] = 0;
	for (ubj = 0; ubj < ubCEPerChannel; ubj++) {
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubj]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubj]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = ((BIT0 << ubj) | (BIT8 << ubj));
		}
		//}

		//Test Mode Entry
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x19;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x96;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x17;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
		pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x00;

		pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
		pFlaReg[FCTLL_PIO_DAT].L = 0x01;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}

		//Reset
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x00;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFD;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}

		//			for(ubj=0;ubj<ubCEPerChannel;ubj++)
		//			{
		//
		//				FCONL[FCONL_FCE_ENB] = ((BIT0 << ubj)|(BIT8 << ubj));
		//
		//				pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[0];
		//				while(!(pFlaReg[FCTLL_RBY_INF].B.B0 & (BIT0 << ubj))){}
		//
		//				pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[1];
		//				while(!(pFlaReg[FCTLL_RBY_INF].B.B0 & (BIT0 << ubj))){}
		//
		//			}
		//			FCONL[FCONL_FCE_ENB] = 0xFFFF;
		//			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];

		//Exit
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFF;
		if (gubDPSEnableUseTimeOut) {

			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}
}

void DPS_MLC_Parameter_set(UWORD uwAddrOffset, UWORD uwDataOffset , UBYTE* ubData , UBYTE ubDPS_TRIMLEN_MLC)
{
	UBYTE ubj;
	UBYTE ubL;
	UBYTE ubCounter = 1;
	U32 ulOperationTime;
	UBYTE ubCEPerChannel = (gubMTQTotalPhyCE >> gubPlanesPerBurstLog);
	_UCV REG_t * pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
	UWORD* uwDPSTableAddr = (UWORD*)(gubDPSTable + uwAddrOffset);
	UWORD* uwDPSTableData = (UWORD*)(gubDPSTable + uwDataOffset);

	FCONL[FCONL_FCE_ENB] = 0;
	for (ubj = 0; ubj < ubCEPerChannel; ubj++) {
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubj]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubj]);
		}
		else {
			FCONL[FCONL_FCE_ENB] |= ((BIT0 << ubj) | (BIT8 << ubj));
		}
		//}
		//-----------------------parameter set------------------------//
		//Test Mode Entry
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x19;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x96;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x17;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
		pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x00;

		pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
		pFlaReg[FCTLL_PIO_DAT].L = 0x01;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}

	//Address Set/Loop
	for (ubj = 0; ubj < (ubDPS_TRIMLEN_MLC); ubj++) {
		if (uwDPSTableData[ubj] == 0xffff) {
			continue;
		}
		for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				//CE Decode can not trigger more than one CE in each channel.
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
			}
			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5F;
			pFlaReg[FCTLL_PIO_ADR].B.B0 = 0xFF;

			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			if (uwDPSTableAddr[ubj] < 256) {
				pFlaReg[FCTLL_PIO_DAT].L = 0x00;
			}
			else {
				pFlaReg[FCTLL_PIO_DAT].L = 0x01;
			}

			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
			pFlaReg[FCTLL_PIO_ADR].B.B0 = uwDPSTableAddr[ubj];//addr
			if (ENABLE_DEBUG_UART_DPS) {
				UartString("\r\nAddr");
				Uart_Tx_DataHex(uwDPSTableAddr[ubj]);
				UartString(" ");
			}
		}
		for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				//CE Decode can not trigger more than one CE in each channel.
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
			}

			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[0];
			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			if ((uwDPSTableData[ubj] & 0x7000) == 0x0) {   /// not constant
				if (uwDPSTableData[ubj] & 0x8000) { /// minus
					pFlaReg[FCTLL_PIO_DAT].L = ubData[ubL * ubDPS_TRIMLEN_MLC * 2 + ubj] - (uwDPSTableData[ubj] & 0xFF);
					if (ENABLE_DEBUG_UART_DPS) {
						Uart_Tx_DataHex(ubData[ubL * ubDPS_TRIMLEN_MLC * 2 + ubj]);
						UartString("-");
						Uart_Tx_DataHex((uwDPSTableData[ubj] & 0xFF));
						UartString(" ");
					}
				}
				else {
					pFlaReg[FCTLL_PIO_DAT].L = ubData[ubL * ubDPS_TRIMLEN_MLC * 2 + ubj] + (uwDPSTableData[ubj] & 0xFF);
					if (ENABLE_DEBUG_UART_DPS) {
						Uart_Tx_DataHex(ubData[ubL * ubDPS_TRIMLEN_MLC * 2 + ubj]);
						UartString("+");
						Uart_Tx_DataHex((uwDPSTableData[ubj] & 0xFF));
						UartString(" ");
					}
				}
			}
			else {
				pFlaReg[FCTLL_PIO_DAT].L = (uwDPSTableData[ubj] & 0xFF);
				if (ENABLE_DEBUG_UART_DPS) {
					Uart_Tx_DataHex(uwDPSTableData[ubj] & 0xFF);
					UartString(" ");
				}
			}

			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[1];
			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			if ((uwDPSTableData[ubj] & 0x7000) == 0x0) {   /// not constant
				if (uwDPSTableData[ubj] & 0x8000) { /// minus
					pFlaReg[FCTLL_PIO_DAT].L = ubData[(ubL * 2 + 1) * ubDPS_TRIMLEN_MLC + ubj] - (uwDPSTableData[ubj] & 0xFF);
					if (ENABLE_DEBUG_UART_DPS) {
						Uart_Tx_DataHex(ubData[(ubL * 2 + 1)*ubDPS_TRIMLEN_MLC + ubj]);
						UartString("-");
						Uart_Tx_DataHex((uwDPSTableData[ubj] & 0xFF));
						UartString(" ");
					}
				}
				else {
					pFlaReg[FCTLL_PIO_DAT].L = ubData[(ubL * 2 + 1) * ubDPS_TRIMLEN_MLC + ubj] + (uwDPSTableData[ubj] & 0xFF);
					if (ENABLE_DEBUG_UART_DPS) {
						Uart_Tx_DataHex(ubData[(ubL * 2 + 1)*ubDPS_TRIMLEN_MLC + ubj]);
						UartString("+");
						Uart_Tx_DataHex((uwDPSTableData[ubj] & 0xFF));
						UartString(" ");
					}
				}
			}
			else {
				pFlaReg[FCTLL_PIO_DAT].L = (uwDPSTableData[ubj] & 0xFF);
				if (ENABLE_DEBUG_UART_DPS) {
					Uart_Tx_DataHex((uwDPSTableData[ubj] & 0xFF));
					UartString(" ");
				}
			}
			//					while(!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)){}
		}
		//FCONL[FCONL_FCE_ENB] = 0xFFFF;
		//pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
	}

	//Exit

	for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
		}
		pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
		pFlaReg[FCTLL_PIO_ADR].B.B0 = 0xFF;

		pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
		pFlaReg[FCTLL_PIO_DAT].L = 0x00;

		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFF;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}
}

void DPS_MLC_Parameter_out(UWORD uwAddrOffset, UBYTE* ubData , UBYTE ubDPS_TRIMLEN_MLC)
{
	UBYTE ubj;
	UBYTE ubL;
	UBYTE ubCounter = 1;
	U32 ulOperationTime;
	UBYTE ubCEPerChannel = (gubMTQTotalPhyCE >> gubPlanesPerBurstLog);
	UWORD* uwDPSTableAddr = (UWORD*)(gubDPSTable + uwAddrOffset);

	_UCV REG_t * pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];

	FCONL[FCONL_FCE_ENB] = 0;
	for (ubj = 0; ubj < ubCEPerChannel; ubj++) {
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubj]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubj]);
		}
		else {
			FCONL[FCONL_FCE_ENB] |= ((BIT0 << ubj) | (BIT8 << ubj));
		}
		//}
		if (ENABLE_DEBUG_UART_DPS) {
			UartString("\r\n");
			UartString("ModifiedPara:");
		}
		//Test Mode Entry
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x19;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x96;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x17;
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
		pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x00;
		pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
		pFlaReg[FCTLL_PIO_DAT].L = 0x01;
		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}
	//Address Set/Loop
	for (ubj = 0; ubj < ubDPS_TRIMLEN_MLC; ubj++) {
		if (ENABLE_DEBUG_UART_DPS) {
			UartString("\r\nAddr");
			Uart_Tx_DataHex(uwDPSTableAddr[ubj ]);
			UartString(" ");
		}

		for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				//CE Decode can not trigger more than one CE in each channel.
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
			}
			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
			pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x01;

			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			pFlaReg[FCTLL_PIO_DAT].L = 0x06;

			if (gubDPSEnableUseTimeOut) {
				ulOperationTime = gulOperationTime;
				while (ubCounter) {
					if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
						ubCounter = 0;
					}
					else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
						ubCounter = 0;
					}
				}
				ubCounter = 1;
			}
			else {
				while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
			}

			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5F;
			pFlaReg[FCTLL_PIO_ADR].B.B0 = 0xFF;
			if (uwDPSTableAddr[ubj ] < 256) {
				pFlaReg[FCTLL_PIO_DAT].L = 0x00;
			}
			else {
				pFlaReg[FCTLL_PIO_DAT].L = 0x01;
			}

			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x00;
			pFlaReg[FCTLL_PIO_ADR].B.B0 = uwDPSTableAddr[ubj ];//addr
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x5F;

			if (gubDPSEnableUseTimeOut) {
				ulOperationTime = gulOperationTime;
				while (ubCounter) {
					if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
						ubCounter = 0;
					}
					else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
						ubCounter = 0;
					}
				}
				ubCounter = 1;
			}
			else {
				while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
			}
		}
		for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				//CE Decode can not trigger more than one CE in each channel.
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
			}

			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[0];
			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			ubData[ubL * ubDPS_TRIMLEN_MLC * 2 + ubj] = (pFlaReg[FCTLL_PIO_DAT].L & 0xFF);
			if (ENABLE_DEBUG_UART_DPS) {
				Uart_Tx_DataHex(ubData[ubL * ubDPS_TRIMLEN_MLC * 2 + ubj]);
				UartString(" ");
			}
			pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[1];
			pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
			ubData[(ubL * 2 + 1)*ubDPS_TRIMLEN_MLC + +ubj] = (pFlaReg[FCTLL_PIO_DAT].L & 0xFF);
			if (ENABLE_DEBUG_UART_DPS) {
				Uart_Tx_DataHex(ubData[(ubL * 2 + 1)*ubDPS_TRIMLEN_MLC + +ubj]);
				UartString(" ");
			}
		}
		//FCONL[FCONL_FCE_ENB] = 0xFFFF;
		//pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[2];

	}

	//Exit

	for (ubL = 0; ubL < ubCEPerChannel; ubL++) {
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
			//CE Decode can not trigger more than one CE in each channel.
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubL]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubL]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = ((BIT0 << ubL) | (BIT8 << ubL));
		}
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x55;
		pFlaReg[FCTLL_PIO_ADR].B.B0 = 0xFF;

		pFlaReg[FCTLL_HS_MODE].L |= ((PIO_DAT_LAST) | (PIO_DAT_FIRST));
		pFlaReg[FCTLL_PIO_DAT].L = 0x00;

		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFF;

		if (gubDPSEnableUseTimeOut) {
			ulOperationTime = gulOperationTime;
			while (ubCounter) {
				if (  (gulOperationTime >=  ulOperationTime) && ((gulOperationTime -  ulOperationTime) > guwDPSWaitTimeOutMiliSec) ) {
					ubCounter = 0;
				}
				else if ((0xFFFFFFFF - ulOperationTime + gulOperationTime) > guwDPSWaitTimeOutMiliSec) {
					ubCounter = 0;
				}
			}
			ubCounter = 1;
		}
		else {
			while (!(pFlaReg[FCTLL_RBY_INF].B.B1 & BIT0)) {}
		}
	}
}

void DPS_SpeedDown(void)
{
	gubFlashClock = FLH_CLK_10MHz;
	flaSwitchClock(gubFlashClock);
	*((volatile unsigned char*)0x04000420) = 0x0;

	if ( gubFlashMode == FLH_MODE_TOGGLE2) {
		flaSetToggleMode(INTERFACE_TOGGLE2To1, 0);
		gubFlashMode = FLH_MODE_TOGGLE1;
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
		gubIOMode_CE[0] = 0;
		gubIOMode_CE[1] = 0;
	}

	/// All need to switch to LEGACY
	flaSetToggleMode(INTERFACE_LEGACY, 0);
	gubFlashMode = FLH_MODE_LEGACY;
	FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
	gubFlashMode_CE[0] = 0;
	gubFlashMode_CE[1] = 0;
}

void DPS_SpeedBack(UBYTE ubFlashClock, UBYTE ubFlashMode)
{
	gubFlashClock = ubFlashClock;
	gubFlashMode = ubFlashMode;

	flaSetFlashMode();
	flaSetFlashPadControl(SET_ODT_100ohm);
	if (guoFlashUseType.B.btToshiba) {
		if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
			flaSetOverDrive();
		}
	}
	flaSwitchClock(gubFlashClock);

	*((volatile unsigned char*)0x04000420) = 0xF;
	/// ================Return CLK and Interface Done==================
}
#endif
#if ENABLE_DEBUG_FREE_BLOCK_TABLE==1
#define MAX_BACKUP_SIZE 160
void ftlCheckFreeBlockTable (UBYTE ubBackup)
{
	// 檢查有沒有unit變失蹤兒童
	volatile UBYTE ubi, ubSkip;
	volatile UBYTE ubLostHappen = 0;
	volatile UWORD uwScanUnit;
	volatile UWORD uwScanUnitIndex;
	volatile UWORD uwGetCount = 0;
	volatile FTarget_t *guwFreeBlockTable_D3_Backup = (FTarget_t *)(&VT->gulNoUseMiddle4B_2[0]);

	for (uwScanUnitIndex = VTAREA_UNITS; uwScanUnitIndex < guwFWTotalUnitNum; uwScanUnitIndex ++) {
		if (gulVC[uwScanUnitIndex].B.ulValidCount != DefaultVC) {
			uwGetCount ++;
		}
	}
	if (uwGetCount != (VT->guwFreeBlockCountMax - VT->guwFreeBlockCount_D3)) {
		UartString("\nFree");
		UartWordHex(uwGetCount);
		UartString(" VT_Free");
		UartWordHex((VT->guwFreeBlockCountMax - VT->guwFreeBlockCount_D3));
		ubLostHappen = 1;

		M_AssertCriticalError(1, VT_ASSERT_UNIT_LOST_0xFB);
	}

	// Backup FreeBlockCount
	VT->gulNoUseMiddle4B_2[0] = VT->guwFreeBlockCount_D3;
	for (uwScanUnitIndex = 2; uwScanUnitIndex < (VT->guwFreeBlockCount_D3 + 2); uwScanUnitIndex++) {
		guwTemp = uwScanUnitIndex;
		// Backup FreeBlockTable
		if ( ubBackup && uwScanUnitIndex < MAX_BACKUP_SIZE ) {
			guwFreeBlockTable_D3_Backup[uwScanUnitIndex] = guwFreeBlockTable_D3[uwScanUnitIndex];
		}
		if (gulVC[guwFreeBlockTable_D3[uwScanUnitIndex].B.uwTarget].B.ulValidCount != DefaultVC) {
			UartString("\nWrongVCUnit");
			UartWordHex(uwScanUnitIndex);
			ubLostHappen = 1;
			VT->gulNoUse4B[0] = uwScanUnitIndex;
			M_AssertCriticalError(1, VT_ASSERT_UNIT_LOST_0xFC);
		}
	}

	for (uwScanUnit = VTAREA_UNITS; uwScanUnit < guwFWTotalUnitNum; uwScanUnit ++) {
		guwTemp = uwScanUnit;
		ubSkip = 0;
		for (ubi = 0; ubi < gubTableUnitNum; ubi ++) {
			if (VT->guwTableTarget[ubi].B.uwTarget == uwScanUnit) {
				ubSkip = 1;
				break;
			}
		}
		if (ubSkip ||
		        (VT->guwRUTTarget.B.uwTarget == uwScanUnit) ||
		        (VT->guwInitInfoTarget.B.uwTarget == uwScanUnit) ||
		        (VT->guwVTChildTarget.B.uwTarget == uwScanUnit)) {
			continue;
		}

		if ((gulVC[uwScanUnit].B.ulValidCount == DefaultVC) && (M_RUT2DTo1D(0, uwScanUnit << gubBurstsPerBankLog) != 0xBBBB)) {
			ubi = 0;
			for (uwScanUnitIndex = 2; uwScanUnitIndex < (VT->guwFreeBlockCount_D3 + 2); uwScanUnitIndex++) {
				if (guwFreeBlockTable_D3[uwScanUnitIndex].B.uwTarget == uwScanUnit) {
					ubi = 1;
					break;
				}
			}
			if (ubi == 0) {
				UartString("\nLostUnit");
				UartWordHex(uwScanUnit);
				ubLostHappen = 1;
				VT->gulNoUse4B[0] = uwScanUnitIndex;
				M_AssertCriticalError(1, VT_ASSERT_UNIT_LOST_0xFD);
			}
		}
	}
	M_AssertCriticalError(ubLostHappen, VT_ASSERT_UNIT_LOST_0xFE);

}

#endif
#endif
