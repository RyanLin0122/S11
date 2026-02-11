#include "setup.h"
#include "Vardef.h"
#include "ftl.h"
#include <string.h>
#include <stdlib.h>

extern UWORD SerialTable[10];
extern UWORD VerTable[4];
extern UWORD ModelTable[20];
///ftlEraseAll ; ubMode==1,EraseBadMark==0xFF ; ubMode==2,EraseBadMark!=0x00 ; ubMode==3,ForceEraseAll
#if MicronFlashOnly
U32 ftlGoodBlock(U32 ulUnitIndex, UBYTE ubPlaneBank, U32 ulSramAddr, U32 ulErasePage, UBYTE ubMode)
#else
U32 ftlGoodBlock(U32 ulUnitIndex, UBYTE ubPlaneBank, U32 ulSramAddr, U32 ulErasePage)               //max is 8 bursts
#endif
{
#if(!RDT)
	M_CheckRDTModeToDisableFunctionReturnValid(1);

	U32 ulPlaneStatus = 0, Temp, ulECCSet_Temp;
	UBYTE ubCEIndex, ubCheckTimes = 0, FastPage, ubCounter, ubCounter0, ubCounter1;
	UBYTE ubDepth, ubFQIndex, ubL4kLink, ubL4KNum = gub4kEntrysPerPlane;
	L4KTable16B pL4KTable;
	UBYTE *ubBuffer = (UBYTE *)ulSramAddr;
	UWORD uwByteOffset, uwLDPCSize;
#if !TLC
	UWORD uwPlaneBank;
#endif
	L4KTable16B *pL4KTablePtr;
#if TLC
	UWORD uwPlaneBankThis;
#endif
	//gubPreformat_Temp=gubPreformat;
	//gubPreformat=1;
	switch (gubLDPC_Mode_Test) {
		case 0:
			uwLDPCSize = 496;
			break;
		case 1:
			uwLDPCSize = 440;
			break;
		case 2:
			uwLDPCSize = 420;
			break;
		case 3:
			uwLDPCSize = 360;
			break;
		case 4:
			uwLDPCSize = 272;
			break;
	}

	if ((gubFlashID[3] & 0x03) == 0x03) {
		//       uwByteOffset = 15272;   //16k page 1024*14+936
		uwByteOffset = (((16384 / (4096 + 16 + 32 + uwLDPCSize)) * 4096) + (16384 % (4096 + 16 + 32 + uwLDPCSize)));

	}
	else if ((gubFlashID[3] & 0x03) == 0x02) {
		//       uwByteOffset = 7910;   //8k page 2048*3+1766
		uwByteOffset = (((8192 / (4096 + 16 + 32 + uwLDPCSize)) * 4096) + (8192 % (4096 + 16 + 32 + uwLDPCSize)));
	}
	else if ((gubFlashID[3] & 0x03) == 0x01) { //4k
		if ( gubFlashID[0] == 0x98 ) {
			uwByteOffset = 3998;
		}
		else if (gubFlashID[0] == 0xEC) { //samsung  // unfinished
			uwByteOffset = 4028;
		}
		else {
			uwByteOffset = 0;
		}
	}

	Temp = FALLL[FCTLL_IBF_CTL];
#if TLC
	FALLL[FCTLL_IBF_CTL] |= FORCE_ZIP_COR;   /* Force data to RAM even UNECC. */
#else
	//FALLL[FCTLL_IBF_CTL] |= FORCE_ZIP_COR;
#endif

	FALLL[FCTLL_CNT_ONE] |= COUNT_MODE_EN;
	FALLL[FCTLL_CNT_ONE] &= (~COUNT_ONE_MODE);	// count 0;
	//FALLL[FALLL_CHNL_SET] &= (~BYPASS_INV);     // 1b: bypass inversion
	ulECCSet_Temp = 0;
	ulECCSet_Temp |= ((FALLL[FCTLL_LDPC_CFG] | LDPC_COR_EN) | (FALLL[FCTLL_OTHER_SET] | BCH_COR_EN) | (FCONL[FCONL_CRC_EN] | CRC_32_EN));
	FALLL[FCTLL_OTHER_SET] &= (~BCH_COR_EN);
	FALLL[FCTLL_LDPC_CFG] &= (~LDPC_COR_EN);
	FCONL[FCONL_CRC_EN] &= (~CRC_32_EN);
	if (ENABLE_PLANE_CH_CE) {
		ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
	}
	else {
		ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
	}
#if TLC
#if UFS
	FastPage = guoFlashUseType.B.btA2Cmd;	 //UFS early bad can be found only in A2 mode
#else
	FastPage = (TLC && (ulErasePage == 1)) ? 1 : 0;
#endif
	uwPlaneBankThis = (UWORD)ubPlaneBank;
#else
	FastPage = guoFlashUseType.B.btA2Cmd;
	uwPlaneBank = (UWORD)ubPlaneBank;
#endif
	UBYTE ubStatus[2] = {0, 0};
	for (ubCheckTimes = 0 ; ubCheckTimes < ((Hynix3DV6) ? 1 : 2) ; ubCheckTimes++) {
		ubFQIndex = gFQI.ubFQLinkFree;
		FlashQueue_t *FQ;

		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ulRAMAddr = (U32)ubBuffer;
#if TLC
		FQ->btCheckEmpty = ((ulErasePage == 1) || (ulErasePage == 2)) ? 1 : 0;
#else
		FQ->btCheckEmpty = ulErasePage;
#endif
		FQ->btThisIsErasePage = 0;
		FQ->ubFJob = BYTE_FJOB_READ;
#if TLC
		if (FastPage) {
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
		}
		else {
			FQ->uwFJobInfo = ( BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
		}
#else
		//if (FastPage) {
		//	FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
		//}
		//else {
		//FQ->uwFJobInfo = ( BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
		//}
#if MicronFlashOnly
		if (ubMode == GOODBLK_SLC) {
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
		}
		else
#endif
		{
			FQ->uwFJobInfo = ( BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
		}
#endif
		FQ->uwFUnit = ulUnitIndex;
#if TLC
		FQ->ulFEntry =  (U32)uwPlaneBankThis << gub4kEntrysPerPlaneLog;
#else
		FQ->ulFEntry =  (U32)uwPlaneBank << gub4kEntrysPerPlaneLog;
#endif
		FQ->ubDepth = ubDepth;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btUNServed = 1;
		FQ->FQPara.B.btConvBypass = 1;
		gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
		gulPara0x24.BitMap.btBufModeEn = 0;
		gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
		gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
		gulPara0x24.BitMap.btZipBypass = 1;
		gulPara0x24.BitMap.btCmpEn = 0;
		gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
		gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(ubBuffer, ubL4KNum * 4096);
		}

		UBYTE ubL4KCount;
		for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA = 0x00000000;
			pL4KTable.Para0x04.ulAll = 0;
			pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (((U32)&ubBuffer[0]) + (ubL4KCount * 4096)) / 512;
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
		if (FQ->btThisIsErasePage || (ulErasePage == 0)) { //沒被寫過
			//ulPlaneStatus = 0;
		}
		else {
			ulPlaneStatus = BIT7;
			M_SwitchTask();
			break;
		}


		M_SwitchTask();
#if TLC
		if (TLC) { // return 0 is good, return BIT7 is bad
			UBYTE ubi;
			ulPlaneStatus = BIT7;
			if (guoFlashDefaultType.B.btSandisk) {
				for (ubi = 0; ubi < 6; ubi++) { // check early bad block, (page0/1 first 6 bytes are 0x00 == early bad block)
					if (ubBuffer[0 + ubi] != 0) {
						ulPlaneStatus = 0;
						break;
					}
				}
			}
#if YMTC
			else if (guoFlashDefaultType.B.btYMTC) {

				if (ubBuffer[0] != 0) {
					ubStatus[ubCheckTimes] = 1;
				}
				if (ubStatus[0]&ubStatus[1]) {
					ulPlaneStatus = 0;
					break;
				}

			}
#endif
#if 0 //Hynix only check ubBuffer[uwByteOffset] in spec but we sued hard condition as toshiba now
			else if (guoFlashDefaultType.B.btHynix) {
				ubi = 0;
				if ((ubBuffer[uwByteOffset + (UWORD)ubi] != 0)) {
					ulPlaneStatus = 0;
					break;
				}
			}
#endif
			else {
				//Toshiba
				for (ubi = 0; ubi < 8; ubi++) {
					if ((ubBuffer[0 + ubi] != 0) || (ubBuffer[uwByteOffset + (UWORD)ubi] != 0)) {
						ulPlaneStatus = 0;
						break;
					}
				}
			}
		}
		else
#endif
		{
			ubCounter0 = 0;
			ubCounter1 = 0;
			for (ubCounter = 0; ubCounter < 8; ubCounter++) {

				if ((ubBuffer[0] & (BIT0 << ubCounter)) == 0) {
					ubCounter0 ++;
				}
				if ((ubBuffer[uwByteOffset] & (BIT0 << ubCounter)) == 0) {
					ubCounter1 ++;
				}
				if ((ubCounter0 > 3) || (ubCounter1 > 3)) {
					ulPlaneStatus = BIT7;
					break;
				}
			}
		}
#if TLC
		if (guoFlashDefaultType.B.btSandisk) {
			uwPlaneBankThis += (M_Multiplier(1, DEF_PlanesPerPage_Mcl_Div));  //sandisk check page0/1
		}
		else {
			//Toshiba
			if (FastPage)
				uwPlaneBankThis += (M_Multiplier((guwFastPagePagesPerUnit - 1), DEF_PlanesPerPage_Mcl_Div));
			else
				uwPlaneBankThis += (M_Multiplier((guwPagesPerUnit - 1), DEF_PlanesPerPage_Mcl_Div));
		}
#else
		uwPlaneBank = uwPlaneBank + (M_Multiplier((guwPagesPerUnit - 1), DEF_PlanesPerPage_Mcl_Div));
#endif
	}
	FALLL[FCTLL_OTHER_SET] |= (ulECCSet_Temp & BCH_COR_EN);
	FALLL[FCTLL_LDPC_CFG] |= (ulECCSet_Temp & LDPC_COR_EN);
	FCONL[FCONL_CRC_EN] |= (ulECCSet_Temp & CRC_32_EN);
	//FALLL[FALLL_CHNL_SET] |= (BYPASS_INV);     // 1b: bypass inversion
	FALLL[FALLL_CHNL_SET] &= (~BYPASS_CONV);
#if TLC
	FALLL[FCTLL_IBF_CTL] = Temp;
#else
	//FALLL[FCTLL_IBF_CTL] = Temp;
#endif
	//gubPreformat=gubPreformat_Temp;
	return ulPlaneStatus;
#endif
#if(RDT)
	return 0;
#endif
}

#if ENABLE_SANDISK_CHECKEARLYBADBYPROG
UBYTE ftlGoodBlock_SandiskCheckByProgFail(U32 ulUnitIndex, UBYTE ubPlaneBank)
{
	UBYTE ubCEIndex, ubChannel;
	UWORD pFSATarget_W0;
	U32 ulTemp_FCONL_FCE_ENB;
	UBYTE ubPhyPlane = (ubPlaneBank & gubBurstsPerBankMask);
	UBYTE ubStatus = 0;
	UBYTE ubPlaneStatus = 0;
	UWORD uwCount = 0;
	UWORD uwPollingCount = 4000;
	UWORD uwBlock = (UWORD)ulUnitIndex;
	FSA_t * pFSA = (FSA_t *)gulFSABase[0];

	//Wait All FQ done
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}

	// ==== Setting Register ======= //
	if (ENABLE_PLANE_CH_CE) {
		ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
	}
	else {
		ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
	}
	ubChannel = ubCEIndex & gubPlanesPerBurstMask;

	volatile REG_t *FREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
	ulTemp_FCONL_FCE_ENB = FCONL[FCONL_FCE_ENB];	//temp CE
	FREG[FCTLL_INT_CFG].L &= (~INTR_VEC_EN); // no need INT for polling

	if ((guoFlashUseType.B.btMultiDie) && (ENABLE_MULTI_DIE) && gubParaInitialized) {
		if ((uwBlock > ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) ) || (uwBlock == ((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog)  )) {
			uwBlock = (  ((uwBlock / (UWORD)(((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )) * (guwDieBlockShift)) + (uwBlock % (((gulPhyBlocksPerCE >> gubDieNumberLog) >> gubBurstsPerBankLog) )));
		}
	}
	M_Check_PseudoCE(ubCEIndex, uwBlock);

	M_VirtualToPhysicalCE(ubCEIndex);

#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][(ubCEIndex / gubPlanesPerBurst)]);
	}
	else {
		FCONL[FCONL_FCE_ENB] = (BIT0 << ((ubChannel * 8) + (ubCEIndex / gubPlanesPerBurst))); // Enable selected CE
	}

	// ==== Setting FSA and ALU ======= //
	pFSATarget_W0 = (ubPhyPlane << (guoFlashUseType.B.btFourPlane ? 14 : 15));

	if (ENABLE_iFSA) {
		FREG[FCTLL_FSA_SEL].L |= IFSA_EN;
		FREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
		FREG[FCTLL_FSA_SEL].B.B0  = 0;
		FREG[FCTLL_IFSA0].L = ( ((uwBlock & 0xFFFF) << 16) | pFSATarget_W0 );
	}
	else {
		pFSA[gubFSAIndex[ubChannel]].uword.W0 = pFSATarget_W0;
		pFSA[gubFSAIndex[ubChannel]].uword.W1 = uwBlock & 0xFFFF;
		pFSA[gubFSAIndex[ubChannel]].B.B4 = (uwBlock & 0xFF0000) >> 16;
		FREG[FCTLL_DMA_CFG].L &= SEL_OTHER_FSA;
		FREG[FCTLL_FSA_SEL].L  = gubFSAIndex[ubChannel];
	}

	FREG[FCTLL_DMA_CFG].L &= ALU0_GROUP_SEL;
	if (guoFlashUseType.B.btA2Cmd) {
		FREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(ALU_A2);//new add
	}
	else {
		FREG[FCTLL_DMA_CFG].L |= ALU_GROUP_SEL(ALU_NORMAL);//new add
	}


	// ==== Program Command (Mark bad) ======= //
#if(Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
	FREG[FCTLL_FPU_ENTRY].L = FPU_SEQ_PTR16(PTR_AG_CA2_C80_A5_C10);
#else
	FREG[FCTLL_FPU_ENTRY].L = RAM_SEQ_PTR16(FPU_AG_CA2_C80_A5_C10);
#endif

	FREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((FREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	FREG[FCTLL_PIO_CMD].B.B0 = 0x70;	//Send 0x70 to check status
	do {
		FREG[FCTLL_IO_SET].L |= FLH_PHY_IF_RESET;
		ubStatus = (UBYTE) FREG[FCTLL_PIO_DAT].L;
		uwCount++;
		if (uwCount > uwPollingCount) {
			break;
		}
	}
	while ((ubStatus & 0xE0) != 0xE0);	//spec Bit 5 is NU for page program

#if (TLC  && (!YMTC) && (!Hynix3DV5) && (!Hynix3DV6))
	if (ubStatus & BIT2)	//SLC program fail if Bit2 is 1
#else
	if (ubStatus & BIT0)	//SLC program fail if Bit0 is 1
#endif
	{
		ubPlaneStatus = BIT7;
	}

	// ==== Recovery ======= //
	FCONL[FCONL_FCE_ENB] = ulTemp_FCONL_FCE_ENB;
	FREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;

	return ubPlaneStatus;
}
#endif

U32 ftlLaterBadBlock(U32 ulUnitIndex, UBYTE ubPlaneBank, U32 ulSramAddr)               //max is 8 bursts
{
#if(!RDT)
	M_CheckRDTModeToDisableFunctionReturnValid(1);

	UBYTE ubCEIndex,  FastPage;
	UBYTE ubDepth, ubFQIndex, ubL4kLink, ubL4KNum = gub4kEntrysPerPlane;
	L4KTable16B pL4KTable;
	UBYTE *ubBuffer = (UBYTE *)ulSramAddr;
	_UCV L4KTable16B *pL4KTablePtr;

	if (ENABLE_PLANE_CH_CE) {
		ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
	}
	else {
		ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
	}
#if TLC
	FastPage = TLC; /* TLC use fastpage to mark bad. */
#else
	FastPage = 0;
#endif
ReadforCheck:
	ubFQIndex = gFQI.ubFQLinkFree;
	FlashQueue_t *FQ;

	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
	FQ->ulRAMAddr = (U32)ubBuffer;
	FQ->btCheckEmpty = 1;
	FQ->btThisIsErasePage = 0;
	FQ->ubFJob = BYTE_FJOB_READ;
	if (FastPage) {
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
	}
	else {
		FQ->uwFJobInfo = ( BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
	}
	FQ->uwFUnit = ulUnitIndex;
	FQ->ulFEntry =  (U32)ubPlaneBank << gub4kEntrysPerPlaneLog;
	FQ->ubDepth = ubDepth;
	FQ->ubL4kIndex = ubL4kLink;
	FQ->btUNServed = 1;
	FQ->FQPara.B.btConvBypass = 1;
	gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
	gulPara0x24.BitMap.btBufModeEn = 0;
	gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
	gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
	gulPara0x24.BitMap.btZipBypass = 1;
	gulPara0x24.BitMap.btCmpEn = 0;
	gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
	gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
	gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate(ubBuffer, ubL4KNum * 4096);
	}

	UBYTE ubL4KCount;
	for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
		pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
		//set L4K table info
		pL4KTable.ulL4K_LCA = 0x00000000;
		pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
		pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0;
		pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (((U32)&ubBuffer[0]) + (ubL4KCount * 4096)) / 512;
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

	pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
	if ((pL4KTablePtr->Para0x08.BitMap.ubL4K_FW2 == 0x0C) ) {
		return 1;
	}

	return 0;
#endif
#if(RDT)
	return 0;
#endif
}

#if FixShallowErase
void ftlForceSaveDummy(U32 ulCurrentPTR, U32 ulEndPtr, UWORD uwFUnit, UBYTE ubMode, UBYTE ubD3Mode, UBYTE ubMark)
{
	M_CheckRDTModeToDisableFunction();

	M_AssertError(uwFUnit == Default_Unit);
	M_AssertError(ulCurrentPTR > ulEndPtr);

	UBYTE ubFastPage = 0;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	FlashQueue_t *FQ;
	UBYTE ubCEIndex, ubL4KNum, ubDepth, ubL4kLink, ubL4KCount;
#if B0KB
	CB_Info_t uFQI_CB_Info;
#endif
	U32 ulProgramPage;
	UBYTE ubPageLoop;
	UBYTE ubBlockShift = 0;
	VT->gubSaveVTSerialNumber = ubMark;
	//UBYTE ubPlaneBankIndex;	//only used for debug

	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
	gubSkipProgramFail = 1;

	//if (ubMode & FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE) {
	// only force save dummy into ubPlaneBankIndex, and this plane bank calculate from ulCurrentPTR
	//	ubPlaneBankIndex = (ulCurrentPTR & gubPlanesPerPageMask);
	//}

	if (ubMode & FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE) {
		ubFastPage = 1;
#if B0KB
		ubBlockShift = ulCurrentPTR / guwFastPagePlanesPerUnit;
#endif
	}
	else {
#if B0KB
		if ( ((( ubMode & FORCE_SAVE_DUMMY_DIRECT_ACCESS) == 0) && (ubD3Mode == 0)
		        && (gubD1LinkTable[uwFUnit].B.ubD1Link != Default_D1_Unit)) || (gulVC[uwFUnit].B.btFastPage == 1)) {
			ubFastPage = 1;
			ubBlockShift = ulCurrentPTR / guwFastPagePlanesPerUnit;
		}
#else
		ubFastPage = gulVC[uwFUnit].B.btFastPage;
#endif
	}

	if (ubFastPage) {
		// Force SLC unit dummy pages program
		ulEndPtr = (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div)) + (ubBlockShift * guwFastPagePlanesPerUnit);
	}
	else {
		// Force MLC unit dummy pages program
		ulEndPtr = (M_Multiplier((U32)SHALLOW_ENDPAGE, DEF_PlanesPerPage_Mcl_Div));
#if B0KB
		VT->gulWrongUnit = uwFUnit;
		M_AssertCriticalError(1, VT_ASSERT_SAVE_DUMMY_WRONG_MODE_0x76);
#endif
	}

	if (ENABLE_DEBUG_UART) {
		UartString("\nFSaveDummy: ");
		UartWordHex(uwFUnit);
		UartString(" ");
		UartLongHex(ulCurrentPTR);
		UartString(" ");
		UartLongHex(ulEndPtr);
		if (ubMode == FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE) {
			UartString(" PB ");
			UartWordHex(M_Modular(ulCurrentPTR, DEF_PlanesPerPage_Mod_Mask));
		}
	}

	while (ulCurrentPTR < ulEndPtr) {

		ubL4KNum = gub4kEntrysPerPlane;

		if (ENABLE_PLANE_CH_CE) {
			ubCEIndex = M_Modular((ulCurrentPTR >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		}
		else {
			ubCEIndex = (ulCurrentPTR & gubPlanesPerBurstMask) + ((M_Modular((ulCurrentPTR >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
		}
#if B0KB
		if (ubFastPage) {
			ulProgramPage = ulCurrentPTR;
			ubPageLoop = 1;
			uFQI_CB_Info.ubProgramOrder = 1;
		}
		else {
			uFQI_CB_Info.uwProgramOrderIndex = M_Divider(ulCurrentPTR, DEF_PlanesPerPage_Mcl_Div);
			B0KB_CheckPage(&uFQI_CB_Info);
			ubPageLoop = uFQI_CB_Info.ubProgramOrder;
			ulProgramPage = M_Multiplier(uFQI_CB_Info.uwProgramOrderIndex, DEF_PlanesPerPage_Mcl_Div) + M_Modular(ulCurrentPTR, gubPlanesPerPage);
		}
#else
		ulProgramPage = ulCurrentPTR;
		ubPageLoop = 1;
#endif
		do {
#if B0KB
			if ( (uFQI_CB_Info.ubProgramOrder != 1) && (ubPageLoop == 1 ) ) {
				ulProgramPage = M_Multiplier(uFQI_CB_Info.uwUPage, DEF_PlanesPerPage_Mcl_Div) + M_Modular(ulCurrentPTR, gubPlanesPerPage);
			}
#endif
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

			FQ->btCheckEmpty = 0;
			FQ->uw4KInFQMap = 0;
			FQ->uwFUnit = uwFUnit;
			FQ->ulFEntry = ulProgramPage << gub4kEntrysPerPlaneLog;
			FQ->ubFJob = BYTE_FJOB_WRITE;
			FQ->uwFJobInfo = (ubFastPage ? BIT_FJOBI_FASTPAGE : 0) | ((ubMode & FORCE_SAVE_DUMMY_DIRECT_ACCESS) ? BIT_FJOBI_DIRECTACESS : 0) ;
			FQ->btNeedToClearWrite4kBufferFlag = 1;
			FQ->FQPara.B.btUserData = 1;
			FQ->ubL4kNum = ubL4KNum;
			//gulProgramUserDataCount ++;
			FQ->ubRSFrameIndex = gubRSCurrentTag;	// 這裡亂塞應該沒差
			FQ->ubRSFrameReuseCount = gubftlRSFrameReuseCount[gubRSCurrentTag];	// 這裡亂塞應該沒差
			FQ->ubRelativeDepth = ubDepth;
			FQ->ubDepth = 0xFF;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btD3 = ubD3Mode;
			FQ->btUNServed = 1;

			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane >> 3;
			gulPara0x24.BitMap.btZipBypass = 0;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btForceFixData = 0;
			gulPara0x24.BitMap.btBufModeEn  = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				if (ubL4KCount) { // 第一~ N個都填High 3 Byte, 因為scan時是bypass zip read, 確保可以用P4K分析
					pL4KTable.Para0x04.BitMap.ulL4K_FW = (VT->guoUnitTimeLine >> 24) & 0xFFFFFF; // High 3Byte
				}
				else { // 第零個填 Low 3Byte
					pL4KTable.Para0x04.BitMap.ulL4K_FW = VT->guoUnitTimeLine & 0xFFFFFF; // Low 3Byte
				}
				pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x08.BitMap.ubL4K_FW2 = Program_Shallow;
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (((((U32)VT) + 0) + (ubL4KCount * 4096)) / 512);//主要是存VT, 暫時沒考慮SectorPerPage小於16K的
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = MAX_ZCODE;
				pL4KTable.ulL4K_LCA = gulTable_Dummy << SectorsPer4KLog;
				*pL4KTablePtr = pL4KTable;
			}
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
			ubPageLoop --;
		}
		while (ubPageLoop);

		if (ubMode & FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE) {// go to next page of same single plane
#if (ENABLE_DEBUG_UART) //Debug
			UartString("\nPB ");
			UartWordHex(M_Modular(ulCurrentPTR, DEF_PlanesPerPage_Mod_Mask));
			UartString(" Page ");
			UartWordHex(M_Divider(ulCurrentPTR , DEF_PlanesPerPage_Mcl_Div));
#endif

			ulCurrentPTR += gubPlanesPerPage;

		}
		else {
			ulCurrentPTR++;
		}
	}
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
	gubSkipProgramFail = 0;
}
#endif

void ftlEraseAll(UBYTE ubMode)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	UBYTE ubPlaneBank, ubi;
	UBYTE ubCEIndex, ubDie;
	U32 ulVenderUnit, ulVenderUnitDieOffset;
	ULLONG uoGoodMap;
	U32 ulFreeblock;
	U32 ulj;
	UBYTE ubCodeCount[2] = {0, 0};

	UBYTE *ubDBTBuffer = (UBYTE *)(BUFFER3_BASE + 0x8000); //ubBUF_32Kshift
	UBYTE *ubReadTemp = (UBYTE *)(BUFFER3_BASE + 0x4000); //ubBUF_16Kshift
	mDMAC_SV(1, (U32)gubEraseFailBitMap, (U32)(64 * 1024), 0, 0, 0, DMA_EN_SETCQ, 1); // 假設最大16CE, 4 plane, 8192 unit, 16*4*8192bit = 64KB
	gubEraseFailBitMapInit = 1;

	//mDMAC_SV(1, (U32)gubZ2Buffer, (U32)gubSectorsPerBurst * 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	U32 ulPhyUnitNumPerDie = gulTotalPhyUnitNum >> gubDieNumberLog;

#if BURNER
	U32	   ulAutoPolFailCeMap = 0;
	U32    ulSkipAutoPolFailCeMap = 0;
	U32    ulAutoPolTimeoutCnt = VT->gulAutoPolTimeoutCnt;
	U32    ulLastAutoPolTimeoutUint;
	UBYTE  ubLastAutoPolTimeoutCEIndex;
#endif

#if EnableProductHistory

	//check producthistory
	//先不考慮在開rdt時寫生產履歷,因為RDT有一段沒DBT
	if (((gubPHinit & 0xF) == 0) && (gubTureErase == 0)) { //沒有init過
		//要erase DBT 所以強制用SCAN
		if (RDT_FindDBTBlock_Task() || (ubMode & MODE_ERASE_DBT)) {
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
	/*
	UartString("\nphunit1:");
	UartLongHex(guwPHUnit[0][0]);
	UartString(" ");
	UartLongHex(guwPHUnit[0][1]);

	UartString("\nphunit2:");
	UartLongHex(guwPHUnit[1][0]);
	UartString(" ");
	UartLongHex(guwPHUnit[1][1]);
	*/

#endif

	if ((ubMode & MODE_DO_ERASEALL) == 0 ) {
		return;
	}

	if (((ubMode & MODE_ERASE_DBT) == 0) ) {
		// bit5 if 1 NOT erase code block
		// bit 3 if 1 "Erase old bad block table(dbt table)" if 0 "reference old bad block table"
		//for (ubCEIndex = 0; ubCEIndex < gubCENumber; ubCEIndex++) {

		if (RDT_FindDBTBlock_Task()) {//get DBT, return 0
			gubRDTDone = DBT_Checked;// no DBT
		}
		else {
			gubRDTDone = (DBT_Checked | DBT_Exist); //get DBT
		}
	}

	if ((ubMode & MODE_RETAIN_CODEBLOCK)) {
#if Hynix
		FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
		ubCodeCount[0] = RDT_FindCodeBlock_Task(0);//找ch0的code blk
		if (gubPlanesPerBurst == 2) {
			ubCodeCount[1] = RDT_FindCodeBlock_Task(1);//找ch1的code blk
			if (ubCodeCount[0] != ubCodeCount[1]) {
				//error?
			}
		}

		if (!BURNER) { //DLMC+EraseAll+Preformat
			gubSysCodeSet[0] = 0xFF;
			gubSysCodeSet[1] = 0xFF;

			gubSysCodeSet[0] = ftlGetSystemSetNumber(Mode_ByRemappingTable, 0, 0, guwSysCodeBlock[0][0]);
			gubSysCodeSet[1] = ftlGetSystemSetNumber(Mode_ByRemappingTable, 0, 0, guwSysCodeBlock[1][0]);
			//檢查兩個CodeBlk是否在同一個Set?
		}
#if Hynix
		FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
	}
	//如果是eraseall+preformat這種case無法保證在這邊可以判讀gubSysCodeSet(因為沒有table也不一定有DBT)
	//但目前在一般情況下不會保留codeBlk去erase
	//但DLMC+Preformat gubSysCodeSet可以在FW起來時的HandleCodeUnit中得到
	//所以這邊先不設定gubSysCodeSet

	U32 ulStartUnit;
#if BURNER
	ulStartUnit = 0;
#else
	if (ICE)
		ulStartUnit = 10;
	else
		ulStartUnit = 0;
#endif
	UBYTE ubFLHPlaneMask = (gubBurstsPerBank << gubBurstsPerBankLog) - 1;
	for (ulVenderUnit = ulStartUnit; ulVenderUnit < gulTotalPhyUnitNum; ulVenderUnit++) {

#if FixShallowErase
		UBYTE ubCanMultiPlaneSaveDummy = 1;
#endif
		uoGoodMap = 0;
		if (((ubMode & MODE_ERASE_DBT) == 0) && (gubRDTDone & DBT_Exist)) {
			ubDie = ulVenderUnit / ulPhyUnitNumPerDie;
			ulVenderUnitDieOffset = ulVenderUnit % ulPhyUnitNumPerDie;
			ulFreeblock = (M_Multiplier((ulVenderUnitDieOffset << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + (M_Multiplier(ubDie, DEF_PlanesPerPage_Mcl_Div));
			ulj = ulFreeblock % ulEntryPerDBTPlane;

			if (ulj < M_Multiplier(gubDieNumber, DEF_PlanesPerPage_Mcl_Div)) {
				if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
					ftlPreformatShowError(0, guwDBTBlock[0], 0xC);
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						SetFailCEtoSystemInfo(ubCEIndex);
					}
					return;
				}
			}
		}

		if (((ubMode & MODE_ERASE_DBT) == 0) && (gubRDTDone & DBT_Exist)) {
			//bit 3 if 1 "Erase old bad block table(dbt table)" if 0 "reference old bad block table"
			for (ubi = 0; ubi < gubPlanesPerBurst; ubi++) {
				if (((U32)guwDBTBlock[ubi] >> gubBurstsPerBankLog) == ulVenderUnit) {
					uoGoodMap |= (((ULLONG)BIT0) << ((ubi << gubBurstsPerBankLog) + (guwDBTBlock[ubi] & gubBurstsPerBankMask)));
				}
			}
		}

		if ((ubMode & MODE_RETAIN_CODEBLOCK) && (ubCodeCount[0] > 0 || ubCodeCount[1] > 0)) {
			// bit5 if 1 NOT erase code block
			for (ubi = 0; ubi < gubPlanesPerBurst; ubi++) {
				if ((guwSysCodeBlock[0][ubi] >> gubBurstsPerBankLog) == ulVenderUnit) {
					uoGoodMap |= (((ULLONG)BIT0) << ((ubi << gubBurstsPerBankLog) + (guwSysCodeBlock[0][ubi] & gubBurstsPerBankMask)));
				}
				if (((guwSysCodeBlock[1][ubi] >> gubBurstsPerBankLog) == ulVenderUnit) && (ubCodeCount[ubi] == 2)) {
					uoGoodMap |= (((ULLONG)BIT0) << ((ubi << gubBurstsPerBankLog) + (guwSysCodeBlock[1][ubi] & gubBurstsPerBankMask)));
				}
			}
		}

		for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
			if (((ubMode & MODE_ERASE_DBT) == 0) && (gubRDTDone & DBT_Exist)) { //By DBT
				if (ubDBTBuffer[ulj]) {
					ulj++;
					uoGoodMap |= (((ULLONG)BIT0) << ubPlaneBank);
					continue;
				}
				else {
					ulj++;
				}
			}
#if BURNER
			if ((ubMode & MODE_CHECK_EARLYBAD) && (!ulAutoPolFailCeMap)) {
#else
			if ((ubMode & MODE_CHECK_EARLYBAD)) {
#endif
#if MicronFlashOnly
				if ((ftlGoodBlock(ulVenderUnit, ubPlaneBank, (U32)&ubReadTemp[0], 0, GOODBLK_MLC)))
#elif ENABLE_SANDISK_CHECKEARLYBADBYPROG
				if (ftlGoodBlock_SandiskCheckByProgFail(ulVenderUnit, ubPlaneBank))
#else
				if ((ftlGoodBlock(ulVenderUnit, ubPlaneBank, (U32)&ubReadTemp[0], 0)))
#endif
				{
					uoGoodMap |= (((ULLONG)BIT0) << ubPlaneBank);
					//continue;
				}
#if (BURNER)
				if (VT->gulAutoPolTimeoutCnt > ulAutoPolTimeoutCnt) {
					if (ENABLE_PLANE_CH_CE) {
						ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
					}
					else {
						ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
					}
					ulAutoPolFailCeMap |= (((ULLONG)BIT0) << ubCEIndex);
					ulSkipAutoPolFailCeMap |= (((ULLONG)BIT0) << ubCEIndex);
					ulAutoPolTimeoutCnt = VT->gulAutoPolTimeoutCnt;
					ulLastAutoPolTimeoutUint = ulVenderUnit;
					ubLastAutoPolTimeoutCEIndex = ubCEIndex;
				}
#endif

				if (uoGoodMap & (((ULLONG)BIT0) << ubPlaneBank)) {
					continue;
				}

			}
#if EnableProductHistory
			if (((U32)guwPHUnit[0].uwUnit == ulVenderUnit) && ((U32)guwPHUnit[0].ubPlanebank == ubPlaneBank) && (gubTureErase == 0)) {
				uoGoodMap |= (((ULLONG)BIT0) << ubPlaneBank);
				continue;
			}
			if (((U32)guwPHUnit[1].uwUnit == ulVenderUnit) && ((U32)guwPHUnit[1].ubPlanebank == ubPlaneBank) && (gubTureErase == 0)) {
				uoGoodMap |= (((ULLONG)BIT0) << ubPlaneBank);
				continue;
			}
#endif

			//#if (!BURNER)
#if 1
			if ((ubMode & MODE_CHECK_LATERBAD)) {
				if (ftlLaterBadBlock(ulVenderUnit, ubPlaneBank, (U32)&ubReadTemp[0])) {
					uoGoodMap |= (((ULLONG)BIT0) << ubPlaneBank);
#if FixShallowErase
					ubCanMultiPlaneSaveDummy = 0;
#endif
					continue;
				}
			}
#endif
		}

		if (ENABLE_DEBUG_SOFTWAREECC) {
			for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
				if (uoGoodMap & (((ULLONG)BIT0) << ubPlaneBank)) {
					UartString("\n Not Erase CE=");//CE
					UartLongHex(ubPlaneBank / gubBurstsPerBank);
					UartString(" Die=");//Die
					U32 ulBlockNumber = (ulVenderUnit * gubBurstsPerBank) + (ubPlaneBank % gubBurstsPerBank);
					UartLongHex(ulBlockNumber / (gulPhyBlocksPerCE / gubDieNumber));
					UartString(" Blk=");//BLOCK
					UartLongHex(ulBlockNumber % (gulPhyBlocksPerCE / gubDieNumber));
				}
			}
		}

		// do erase by ulNeedDoEraseMap
		UBYTE ubFQIndex[64];
		for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
			UBYTE ubEraseUnit_flag = 0;
			//check only planeA and all plane can be erased => Can do multi-plane Erase
			if ( ((ubPlaneBank & gubBurstsPerBankMask) == 0) && ((uoGoodMap & (((ULLONG)(ubFLHPlaneMask)) << ubPlaneBank)) == 0)) {
				ubEraseUnit_flag = 1;
			}

			if (uoGoodMap & (((ULLONG)BIT0) << ubPlaneBank)) {
				continue;
			}
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
			}

#if BURNER
			if (((ulAutoPolFailCeMap >> ubCEIndex) & BIT0) == 1) {
				continue;
			}
#endif
			ubFQIndex[ubPlaneBank] = gFQI.ubFQLinkFree;
			FlashQueue_t *FQ;
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			if (ubEraseUnit_flag) {
				FQ->ubFJob = BYTE_FJOB_ERASE_UNIT;
			}
			else {
				FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
			}
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_ERASE_FAIL_RESET);

			// Must Send Reset CMD(0xFF) After Erase Fail
			FQ->uwFUnit = ulVenderUnit;
			FQ->ulFEntry = ((U32)ubPlaneBank << gub4kEntrysPerPlaneLog);
			FQ->ubRelativeDepth = ubDepth;
			FQ->ubDepth = 0xFF;
			FQ->btUNServed = 1;
			FQ->btCheckEmpty = 0;
			FQ->ubL4kIndex = ubL4kLink;
#if TLC
			FQ->btD3 = 1;
#endif
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
			if (ubEraseUnit_flag) {
				ubPlaneBank += gubBurstsPerBankMask;
			}
		}
		for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
			UBYTE ubEraseUnit_flag = 0;
			//check only planeA and all plane can be erased => Can do multi-plane Erase
			if ( ((ubPlaneBank & gubBurstsPerBankMask) == 0) && ((uoGoodMap & (((ULLONG)(ubFLHPlaneMask)) << ubPlaneBank)) == 0)) {
				ubEraseUnit_flag = 1;
			}

			if (uoGoodMap & (((ULLONG)BIT0) << ubPlaneBank)) {
				continue;
			}

#if BURNER
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
			}

			if (((ulAutoPolFailCeMap >> ubCEIndex) & BIT0) == 1) {
				if (((ulSkipAutoPolFailCeMap >> ubCEIndex) & BIT0) == 1) {
					continue;
				}
				else {
					gubINTTable[gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubDepth].All = 0x01;
				}
			}
#endif

			while (gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubFQPhase = BYTE_FQ_CHECKDONE;
			if (gFQI.gFQLink[ubFQIndex[ubPlaneBank]].btPEFail) {
#if (BURNER)
				if (VT->gulAutoPolTimeoutCnt > ulAutoPolTimeoutCnt) {
					ulAutoPolFailCeMap |= (((ULLONG)BIT0) << ubCEIndex);
					ulAutoPolTimeoutCnt = VT->gulAutoPolTimeoutCnt;
					ulLastAutoPolTimeoutUint = ulVenderUnit;
					ubLastAutoPolTimeoutCEIndex = ubCEIndex;
				}
#endif
				if (((ubMode & MODE_ERASE_DBT) == 0) && (gubRDTDone & DBT_Exist) && (ubMode & MODE_CHECK_ERASEFAIL)) {
					ubDie = ulVenderUnit / ulPhyUnitNumPerDie;
					ulVenderUnitDieOffset = ulVenderUnit % ulPhyUnitNumPerDie;
					ulFreeblock = (M_Multiplier((ulVenderUnitDieOffset << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + (M_Multiplier(ubDie, DEF_PlanesPerPage_Mcl_Div)) + ubPlaneBank;
					if (ubEraseUnit_flag) { //Need to check which plane fail and update DBT when Erase Unit
						UBYTE ubEraseFail_Count = 0;
						FlashQueue_t *FQ;
						FQ = &gFQI.gFQLink[ubFQIndex[ubPlaneBank]];
						for (ubEraseFail_Count = 0; ubEraseFail_Count < gubBurstsPerBank; ubEraseFail_Count++) {
							if ((FQ->ubCheckStatusPhase & (BIT0 << ubEraseFail_Count))) { //mark fail plane
								gubEraseFailBitMap[(ulFreeblock >> 3)] |= BIT0 << (ulFreeblock & 7); //record erase fail and mark bad in preformat
								/*
								UartString("\n Erase Fail, PlaneBank: ");
								UBYTE ubCEIndexTemp = gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubCEIndex;
								UWORD uwFUnitTemp = gFQI.gFQLink[ubFQIndex[ubPlaneBank]].uwFUnit;
								ubCEIndexTemp = ubCEIndexTemp % gubMTQTotalPhyCE;
								Uart_Tx_DataHex(ubPlaneBank+ubEraseFail_Count);
								UartString(" ");
								UartWordHex(uwFUnitTemp);
								*/
							}
							ulFreeblock++;
						}
					}
					else {
						gubEraseFailBitMap[(ulFreeblock >> 3)] |= BIT0 << (ulFreeblock & 7); //record erase fail and mark bad in preformat
						/*
						UartString("\n Erase Fail, PlaneBank: ");
						UBYTE ubCEIndexTemp = gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubCEIndex;
						UWORD uwFUnitTemp = gFQI.gFQLink[ubFQIndex[ubPlaneBank]].uwFUnit;
						ubCEIndexTemp = ubCEIndexTemp % gubMTQTotalPhyCE;
						ubFailCount++;
						Uart_Tx_DataHex(ubPlaneBank);
						UartString(" ");
						UartWordHex(uwFUnitTemp);
						*/
					}
				}
			}
			if (ubEraseUnit_flag) {
				ubPlaneBank += gubBurstsPerBankMask;
			}
		}

#if BURNER
		for (ubCEIndex = 0; ubCEIndex < MAX_CE; ubCEIndex++) {
			if ((((ulAutoPolFailCeMap >> ubCEIndex) & BIT0) == 1) && (((ulSkipAutoPolFailCeMap >> ubCEIndex) & BIT0) == 0)) {
				ulSkipAutoPolFailCeMap |= (((ULLONG)BIT0) << ubCEIndex);
			}
		}
#endif
		//#if FixShallowErase
#if 0
		for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
			if ((ftlGoodBlock(ulVenderUnit, ubPlaneBank, (U32)&ubReadTemp[0], 0, GOODBLK_MLC))) {
				uoGoodMap |= (BIT0 << ubPlaneBank);
				ubCanMultiPlaneSaveDummy = 0;
				continue;
			}
		}
		// 先補完DUMMY再推ERASE FQ, 不然ERASE WAIT CHECK會卡住
		if (ubCanMultiPlaneSaveDummy) {
			// using FORCE_SAVE_DUMMY_UNIT_MODE to combine multi plane program
			UartString(" Multi: ");
			ftlForceSaveDummy((U32)00 << gubPlanesPerPageLog, (U32)SHALLOW_ENDPAGE << gubPlanesPerPageLog, ulVenderUnit, (FORCE_SAVE_DUMMY_UNIT_MODE | FORCE_SAVE_DUMMY_DIRECT_ACCESS ), D3_AREA);
		}
		else {
			// using FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE
			UartString(" Single: ");
			for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
				if (uoGoodMap & (BIT0 << ubPlaneBank)) {
					continue;
				}
				ftlForceSaveDummy(((U32)00 << gubPlanesPerPageLog) + ubPlaneBank, ((U32)SHALLOW_ENDPAGE << gubPlanesPerPageLog) + ubPlaneBank, ulVenderUnit, (FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE | FORCE_SAVE_DUMMY_DIRECT_ACCESS), D3_AREA);
			}
		}
#endif
	}
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
	gubEraseAllDone_Flag = 1;
#if BURNER
	if (ulAutoPolFailCeMap != 0) {
		ftlPreformatShowError(ubLastAutoPolTimeoutCEIndex, ulLastAutoPolTimeoutUint, 0xEF);

		for (ubCEIndex = 0; ubCEIndex < MAX_CE; ubCEIndex++) {
			if (((ulAutoPolFailCeMap >> ubCEIndex) & BIT0) == 1) {
				SetFailCEtoSystemInfo(ubCEIndex);
			}
		}
	}
#endif

#endif
}

void ftlNandEmulatorVenderErase(UWORD uwVnederUnit, UBYTE ubFeature)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubPlaneBank;
	UBYTE ubCEIndex;

	if (ubFeature == 0) {
		flaLEDBlink(0xFFE);
	}

	if ((gFQI.ubFQLinkNumber + gubPlanesPerPage) > gFQ_NUM) {
		M_SwitchTask();
	}

	// do erase
	UBYTE ubFQIndex[64];
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
		FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_ERASE_FAIL_RESET);

		// Must Send Reset CMD(0xFF) After Erase Fail


		FQ->uwFUnit = uwVnederUnit;
#if TLC_NES_TEST
		switch (uwVnederUnit) {
			case 0x11: // D1 A
				//ftlNandEmulatorVenderErase(0x11, 0x11);
				FQ->uwFUnit = M_RUT2DTo1D(ubCEIndex, ((guwFWTotalUnitNum + (gubD1LinkTable[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.ubD1Link * D1_UNIT_NUM_PER_LINK) + 0) << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask) );
				break;
			case 0x12: // D1 B
				//ftlNandEmulatorVenderErase(0x12, 0x12);
				FQ->uwFUnit = M_RUT2DTo1D(ubCEIndex, ((guwFWTotalUnitNum + (gubD1LinkTable[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.ubD1Link * D1_UNIT_NUM_PER_LINK) + 1) << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask) );
				break;
			case 0x13: // D1 C
				//ftlNandEmulatorVenderErase(0x13, 0x13);
				FQ->uwFUnit = M_RUT2DTo1D(ubCEIndex, ((guwFWTotalUnitNum + (gubD1LinkTable[VT->guwGRTarget[VT->gubGRTargetIndex].B.uwTarget].B.ubD1Link * D1_UNIT_NUM_PER_LINK) + 2) << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask) );
				break;
			default:
				break;
		}
#endif
		FQ->ulFEntry = (M_Multiplier((U32)ubFeature, DEF_4KEntryPerPage_Mcl_Div)) + (ubPlaneBank << gub4kEntrysPerPlaneLog);
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
		while (gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		gFQI.gFQLink[ubFQIndex[ubPlaneBank]].ubFQPhase = BYTE_FQ_CHECKDONE;
		if (gFQI.gFQLink[ubFQIndex[ubPlaneBank]].btPEFail) {
			flaLEDBlink(0xFE);
		}
	}

#endif
}

UBYTE TranslateLogicalCEtoPhysicalCE(UBYTE ubLogical_CEIndex)
{
	UBYTE ubi, ubPhysical_CEIndex;
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
		UBYTE ubChannel;
		ubChannel = (ubLogical_CEIndex % gubPlanesPerBurst);
		for (ubi = 0; ubi < gubMAX_SCAN_CE_PER_CH; ubi++) {
			if (gubCH_CEMappingToPCE[ubChannel][(ubLogical_CEIndex / gubPlanesPerBurst)] == ubi) {
				ubPhysical_CEIndex = ubi + (ubChannel * gubMAX_SCAN_CE_PER_CH);
				break;
			}
		}
	}
	else {
		ubLogical_CEIndex = ((ubLogical_CEIndex % gubPlanesPerBurst) << 3) + (ubLogical_CEIndex / gubPlanesPerBurst);//LogicalCE to F_CE
		for (ubi = 0; ubi < 16; ubi++) {
			if (gubReMappingCE[ubi] == ubLogical_CEIndex) {
				ubPhysical_CEIndex = ubi;
				break;
			}
		}
	}
	return ubPhysical_CEIndex;
}

void ftlPreformatShowError(UBYTE ubCEIndex, UWORD uwUnitIndex, UBYTE ubFailStatus)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();

	if (ENABLE_DEBUG_RW) {
		flaLEDBlink(ubFailStatus);
	}
	gubErrorCode = SET_ABRT;
	gubTest = ubFailStatus;
	HB[HB_LBA_L] = ubFailStatus;
	HB[HB_LBA_M] = ubCEIndex;
	HB[HB_LBA_H] = uwUnitIndex & 0xFF;
	HB[HB_LBA_H_EXP] = (uwUnitIndex >> 0x8) & 0xFF;
#endif
}

UBYTE ftlLoadDBT( U32 ulTarget, U32 ulSramAddr)      // load 1 plane
{
#if(!RDT)
	M_CheckRDTModeToDisableFunctionReturnValid(1);

	UBYTE ubCEIndex, ubDBTBurst, ubStartCEIndex, ubNextCE, ubTwoChannelRead;
	UBYTE ubDepth, ubFQIndex, ubL4kLink, ubL4KNum = gub4kEntrysPerPlane;
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;
	U32 ubPlaneBank;
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	U32 ulTotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;
	UBYTE ubDBTSizeInPlanes = (ulTotalBlockNum + (ulEntryPerDBTPlane - 1)) / ulEntryPerDBTPlane;	// 取餘數進位
	UWORD uwNextBackupCnt = 0;
	U32 ulDiffEntrysPerBackup = M_Multiplier(ubDBTSizeInPlanes, DEF_PlanesPerPage_Mcl_Div) << gub4kEntrysPerPlaneLog;

	ubPlaneBank = M_Multiplier(((ulTarget / 512) >> gubSectorsPerPlaneLog), DEF_PlanesPerPage_Mcl_Div);
	//ubCEIndex = (ubPlaneBank&gubPlanesPerBurstMask) + ((((ubPlaneBank>>gubPlanesPerBankLog)&gubBanksPerPageMask)<<gubPlanesPerBurstLog));

	if (guwDBTBlock[0] == 0xFFFF && guwDBTBlock[1] == 0xFFFF) { //兩個CE都沒有DBT, 就不用Read
		return 1;
	}
	else if (guwDBTBlock[0] == 0xFFFF) { //CE_1 有DBT, 只read CE_1
		ubTwoChannelRead = FALSE;
		ubStartCEIndex = 1;
		ubNextCE = 2;
	}
	else if (guwDBTBlock[1] == 0xFFFF) { //CE_0 有DBT, 只read CE_0
		ubTwoChannelRead = FALSE;
		ubStartCEIndex = 0;
		ubNextCE = 2;
	}
	else {//兩個CE都有DBT, 兩邊都可以Read
		ubTwoChannelRead = TRUE;
		ubStartCEIndex = 0;
		ubNextCE = 1;
	}

	for (ubCEIndex = ubStartCEIndex; (ubCEIndex %= gubPlanesPerBurst) < gubPlanesPerBurst; ubCEIndex += ubNextCE) {
		ubFQIndex = gFQI.ubFQLinkFree;
		FlashQueue_t *FQ;
		ubDBTBurst = guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask;

		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ulRAMAddr = ulSramAddr;
		FQ->btCheckEmpty = 1;
		FQ->btThisIsErasePage = 0;
		FQ->ubFJob = BYTE_FJOB_READ;
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
		FQ->uwFUnit = (guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog);
		FQ->ulFEntry = ((ubPlaneBank + (ubCEIndex << gubBurstsPerBankLog) + ubDBTBurst) << gub4kEntrysPerPlaneLog) + (uwNextBackupCnt * ulDiffEntrysPerBackup);
		FQ->ubDepth = ubDepth;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btUNServed = 1;
		FQ->btD3 = 1;
		FQ->FQPara.B.btConvBypass = 0;
		gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
		gulPara0x24.BitMap.btBufModeEn = 0;
		gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
		gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
		gulPara0x24.BitMap.btZipBypass = 1;
		gulPara0x24.BitMap.btCmpEn = 0;
		gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
		gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;


		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate((void *)ulSramAddr, ubL4KNum * 4096);
		}

		UBYTE ubL4KCount = 0;
		for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA = 0x00000000;
			//pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
			pL4KTable.Para0x04.BitMap.ulL4K_FW = 0xFF;
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
		gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

		if (FQ->btThisIsErasePage) { //沒被寫過
			if ((ubTwoChannelRead && (ubCEIndex == (gubPlanesPerBurst - 1))) || (!ubTwoChannelRead)) {
				if (((FQ->ulFEntry + ulDiffEntrysPerBackup) >= (guwFastPagePlanesPerUnit << gub4kEntrysPerPlaneLog)) ) {
					return 1;
				}
				uwNextBackupCnt++;
			}
			continue;
		}

		//pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
		pL4KTablePtr = (volatile L4KTable16B*)(IRAM_BVCI_BASE + SPARE_START_OFFSET + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
#pragma flush
#if(BURNER)
		if ((pL4KTablePtr->ulL4K_LCA != MARK_DBT) || FQ->btReadUNC ) {
#else
		if ((pL4KTablePtr->ulL4K_LCA != MARK_DBT) ) {
#endif
			if ((ubTwoChannelRead && (ubCEIndex == (gubPlanesPerBurst - 1))) || (!ubTwoChannelRead)) {
				if (((FQ->ulFEntry + ulDiffEntrysPerBackup) >= (guwFastPagePlanesPerUnit << gub4kEntrysPerPlaneLog)) ) {
					if (ENABLE_DEBUG_UART_CC) {
						UartString("\n\r LDBT Fail");
					}
					return 1;
				}
				uwNextBackupCnt++;
			}
		}
		else {
			break;
		}
	}
	return 0;

#endif
#if(RDT)
	return 0;
#endif
}


//only for CE0 CE1 case
UBYTE ftlReadSinglePlane(U32 ulUnitIndex, U32 ulPlane,  UBYTE ubCEIndex, U32 ulSramAddr, U32 LCA, UWORD uwFJobInfo, UBYTE ubL4KNum, UBYTE ubSetEntry, UBYTE ubBurstIndex)
{
#if(!RDT)
	M_CheckRDTModeToDisableFunctionReturnValid(1);

	UBYTE ubDepth, ubFQIndex, ubL4kLink, ubi;
	UBYTE ubBank = (ubCEIndex >> gubPlanesPerBurstLog);
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;
	U32 ulZip = 0;

	ubFQIndex = gFQI.ubFQLinkFree;
	FlashQueue_t *FQ;

	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
	FQ->ulRAMAddr = ulSramAddr;
	FQ->btCheckEmpty = 1;
	FQ->btThisIsErasePage = 0;
	FQ->ubFJob = BYTE_FJOB_READ;
	FQ->uwFJobInfo = uwFJobInfo;
	FQ->uwFUnit = (UWORD)ulUnitIndex;
	if (ubSetEntry) {
		FQ->ulFEntry = ulPlane;
	}
	else {
		if (ENABLE_PLANE_CH_CE) {
			FQ->ulFEntry = (((M_Multiplier((U32)ulPlane, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubBurstIndex) << gub4kEntrysPerPlaneLog);
		}
		else {
			FQ->ulFEntry = (((M_Multiplier((U32)ulPlane, DEF_PlanesPerPage_Mcl_Div)) + (ubBank << gubPlanesPerBankLog) + (ubCEIndex & gubPlanesPerBurstMask) + (ubBurstIndex << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
		}
	}
	FQ->ubDepth = ubDepth;
	FQ->ubL4kIndex = ubL4kLink;
	FQ->btUNServed = 1;
#if FixShallowErase
	FQ->btD3 = (uwFJobInfo & BIT_FJOBI_D1_UNIT) ? D1 : D3;
	//if(uwFJobInfo & BIT_FJOBI_D1_UNIT){
	//	UartString("\nD1_R");
	//}
#else
	FQ->btD3 = 1;
#endif
	FQ->FQPara.B.btConvBypass = 0;
	gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
	gulPara0x24.BitMap.btBufModeEn = 0;
	gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
	gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
	gulPara0x24.BitMap.btZipBypass = 1;
	gulPara0x24.BitMap.btCmpEn = 0;
	gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
	gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
	for (ubi = 0; ubi < ubL4KNum; ubi++) {
		ulZip |= (0xFF << (ubi * 8));
	}
	gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;


	if (ENABLE_DCACHE) {
		xthal_dcache_region_invalidate((void *)ulSramAddr, ubL4KNum * 4096);
	}

	UBYTE ubL4KCount = 0;
	for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
		pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
		//set L4K table info
		pL4KTable.ulL4K_LCA = 0x00000000;
		//pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
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

	gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;
	//M_SwitchTask();

	if (FQ->btThisIsErasePage) { //沒被寫過
		return Read_Erase_Page;
		//gubTemp = 1;
		//while (gubTemp);
	}

	pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
#if(BURNER)
	if ((pL4KTablePtr->ulL4K_LCA != LCA) || FQ->btReadUNC ) {
#else
	if ((pL4KTablePtr->ulL4K_LCA != LCA) ) {
#endif
		return Read_Spear_Mismatch;
		//gubTemp = 1;
		//while (gubTemp);
	}
	return 0;
#endif
#if(RDT)
	return 0;
#endif
}
#if ENABLE_TLC_PROGRAM_CODEBLOCK2
UBYTE ftlWriteSinglePlaneTLC(U32 ulUnitIndex, U32 ulPlane,  UBYTE ubCEIndex, U32 ulSramAddr, U32 LCA, U32 FW, UWORD uwFJobInfo, UBYTE ubL4KNum, UBYTE ubSetEntry, UBYTE ubBurstIndex)
{


	UBYTE ubDepth, ubFQIndex, ubL4kLink;
	UBYTE ubBank = (ubCEIndex >> gubPlanesPerBurstLog);
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;

	FlashQueue_t *FQ;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ulSramAddr, ubL4KNum * 4096);
	}


	UBYTE ubLMU = 0;
	UBYTE ubL4KCount = 0;

	for (ubLMU = 0; ubLMU < 3; ubLMU++) {
		ubFQIndex = gFQI.ubFQLinkFree;
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ulRAMAddr = ulSramAddr;
		FQ->ubFJob = BYTE_FJOB_WRITE;
		FQ->uwFJobInfo = uwFJobInfo;
		FQ->uwFUnit = ulUnitIndex;
		if (ubSetEntry) {
			FQ->ulFEntry = ulPlane + ubLMU;
		}
		else {
			FQ->ulFEntry = (((M_Multiplier((U32)(ulPlane + ubLMU), DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubBurstIndex) << gub4kEntrysPerPlaneLog);
		}

		FQ->ubRelativeDepth = ubDepth;
		FQ->ubDepth = 0xFF;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btUNServed = 1;
		FQ->btD3 = 1;
		FQ->btGenFail = 0;

		FQ->ubD3WriteMode = (D3_SUSPEND_EN) | D3_WRITE_FINE | (D3_WRITE_LOWDER << ubLMU);
		FQ->btTLCProgramCodeBlock = 1;

		gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
		gulPara0x24.BitMap.btBufModeEn = 0;
		gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
		gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
		gulPara0x24.BitMap.btZipBypass = 1;
		gulPara0x24.BitMap.btCmpEn = 0;
		gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;



		for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA = LCA;
			pL4KTable.Para0x04.BitMap.ulL4K_FW = FW;
			pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
			pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = MAX_ZCODE;
			pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0xAB;
			pL4KTable.Para0x0C.BitMap.ulL4K_BADR =  ((ulSramAddr + (ubL4KCount * 4096) ) / 512);
			*pL4KTablePtr = pL4KTable;
		}

		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

		// wait program done...

		if (FQ->uwFJobInfo & BIT_FJOBI_WAITRESULT) {
			while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

			if (gFQI.gFQLink[ubFQIndex].btPEFail) {
				return Write_Fail;
			}
			M_SwitchTask();
		}
	}
	return 0;
}
#endif


UBYTE ftlWriteSinglePlane(U32 ulUnitIndex, U32 ulPlane,  UBYTE ubCEIndex, U32 ulSramAddr, U32 LCA, U32 FW, UWORD uwFJobInfo, UBYTE ubL4KNum, UBYTE ubSetEntry, UBYTE ubBurstIndex)
{
#if(!RDT)


	UBYTE ubDepth, ubFQIndex, ubL4kLink;
	UBYTE ubBank = (ubCEIndex >> gubPlanesPerBurstLog);
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;
	ubFQIndex = gFQI.ubFQLinkFree;
	FlashQueue_t *FQ;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ulSramAddr, ubL4KNum * 4096);
	}

	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
	FQ->ulRAMAddr = ulSramAddr;
	FQ->ubFJob = BYTE_FJOB_WRITE;
	FQ->uwFJobInfo = uwFJobInfo;
	FQ->uwFUnit = ulUnitIndex;
	if (ubSetEntry) {
		FQ->ulFEntry = ulPlane;
	}
	else {
		if (ENABLE_PLANE_CH_CE) {
			FQ->ulFEntry = (((M_Multiplier((U32)ulPlane, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubBurstIndex) << gub4kEntrysPerPlaneLog);
		}
		else {
			FQ->ulFEntry = (((M_Multiplier((U32)ulPlane, DEF_PlanesPerPage_Mcl_Div)) + (ubBank << gubPlanesPerBankLog) + (ubCEIndex & gubPlanesPerBurstMask) + (ubBurstIndex << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
		}
	}
	FQ->ubRelativeDepth = ubDepth;
	FQ->ubDepth = 0xFF;
	FQ->ubL4kIndex = ubL4kLink;
	FQ->btUNServed = 1;
	FQ->btD3 = 1;
	FQ->btGenFail = 0;

#if (ENABLE_DEBUG_SYSTEM_UNIT_FAIL && ENABLE_RUT2DBT)
	FQ->btGenFail = gubGenProgramFailDebug;	// 造program fail
#endif

	gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
	gulPara0x24.BitMap.btBufModeEn = 0;
	gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
	gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
	gulPara0x24.BitMap.btZipBypass = 1;
	gulPara0x24.BitMap.btCmpEn = 0;
	gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
	gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

	UBYTE ubL4KCount = 0;

	for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
		pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
		//set L4K table info
		pL4KTable.ulL4K_LCA = LCA;
		pL4KTable.Para0x04.BitMap.ulL4K_FW = FW;
		pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
		pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = MAX_ZCODE;
		pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
		pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0xAB;
		pL4KTable.Para0x0C.BitMap.ulL4K_BADR =  ((ulSramAddr + (ubL4KCount * 4096) ) / 512);
		*pL4KTablePtr = pL4KTable;
	}

	gFQI.ubFQLinkNumber++;
	gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

	// wait program done...

	if (FQ->uwFJobInfo & BIT_FJOBI_WAITRESULT) {
		while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
		if (gFQI.gFQLink[ubFQIndex].btPEFail) {
			return Write_Fail;
		}
		M_SwitchTask();
	}

#endif
	return 0;
}

void ftlInitAtaCfg()
{
#if(!RDT)
	M_CheckRDTModeToDisableFunction();

	UBYTE ubi;
	memset(&AtaCfg, 0x00, sizeof(ATACONFIG));
	//AtaCfg.ubPreformatCheckEarlyBadDoing = gubActiveTaskMap;

	if (gubSecuritySupport) {
		// for the initial value not zero
		for (ubi = 0; ubi < 16; ubi++) {
			// default MasterPassword
			AtaCfg.uwSecMasterPW[ubi] = 0x2020; //for TSB spec
		}
		AtaCfg.uwSecMasterID = 0xFFFE;
		// bit 0 Security supported ,W128 Bit0
		// bit 5 Enhanced Security erase support
		AtaCfg.ubSecMode = (SET_SEC_SUPPORT | SET_SEC_ENHANCE_ERASE);

		AtaCfg.ubLastSecurityEraseMode = 0; // Set Default Value

	}

	if (SMART_Support) {
		// default smart enable for DM2008
		AtaCfg.ubSmartState = 1;
	}

	AtaCfg.ubTemperatureMax = AtaCfg.ubTemperatureMin = I2C_Temperature(0);
#endif
}

#if Hynix
void flaDisableSecretMode()
{
#if (Hynix3DV6 || Hynix3DV5)
	//Secret Mode Workaround by Failure Prevention Note_V5 512Gb TLC_R1_20190509_nodrm.pdf
	//V5 512Gb Only
	if (guoFlashUseType.B.btHynix3DV5 && (gubDensitysize == FLH_Size_512Gb)) {

		UBYTE ubCEIndex, ubChannel, ubPhyCEIndex;
		UBYTE ubDieOffset = 0x20;	//V5 LUN[A36:A38]
		UBYTE ubDie;

		UBYTE ubPara;

		for (ubCEIndex = 0; ubCEIndex < gubCENumber; ubCEIndex++) {

			ubPhyCEIndex = ubCEIndex / MAX_CHANNEL;
			ubChannel = ubCEIndex % MAX_CHANNEL;
			if (gubGPIO_CE_Decoder) {
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubPhyCEIndex]); // Enable selected CE
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((UWORD)BIT0 << (ubPhyCEIndex + (ubChannel * MAX_CE_PER_CH) ));
			}

			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannel & gubPlanesPerBurstMask) * 0x200));

			gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);

			for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
				PollingStatusByDie((ubDie * ubDieOffset));
				//Get Parameter
				Hynix3D_GetParameter(0x4C, &ubPara);

				PollingStatusByDie((ubDie * ubDieOffset));
				//Set Parameter
				Hynix3D_SetParameter(0x4C, (ubPara & 0x7F));
			}

		}
		M_DisableAllFlashCE();
	}
	else {
		//V6 not yet
		//flaLEDBlink(1);
	}
#endif
}


void FlaIssueHynixSDPCmd(U32 ch, U32 ce, U32 cmd)
{
	if (guoFlashDefaultType.B.bt8LC && guoFlashUseType.B.btHynix) {
		volatile REG_t *FREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ch);
		FCONL[FCONL_FCE_ENB] |= ce;     //Need Fix: if CE Decoder Mode is enable
		if (cmd == 0x38) {
			FREG[FCTLL_PIO_CMD].L = cmd;
			FREG[FCTLL_PIO_CMD].L = 0x70;			//Send 0x70 to check status
			FREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
			while (FREG[FCTLL_PIO_DAT].B.B0 != 0xE0);
			FlaIssueHynixDaCmd(ch, ce, 0xBE);
		}
		else {
			FREG[FCTLL_PIO_CMD].L = 0x36;
#if Hynix_16nm
			FREG[FCTLL_PIO_ADR].L = 0x73;
			PIO_WDATA(0x01, FREG);
			FREG[FCTLL_PIO_ADR].L = 0x9A;
			PIO_WDATA(0x04, FREG);
			FREG[FCTLL_PIO_ADR].L = 0xA2;
			PIO_WDATA(0x0D, FREG);
			FREG[FCTLL_PIO_ADR].L = 0xC7;
			PIO_WDATA(0x55, FREG);
#else
			FREG[FCTLL_PIO_ADR].L = 0x96;
			PIO_WDATA(0x04, FREG);
			FREG[FCTLL_PIO_ADR].L = 0x7D;
			PIO_WDATA(0x04, FREG);
			FREG[FCTLL_PIO_ADR].L = 0x96;
			PIO_WDATA(0x00, FREG);
			FREG[FCTLL_PIO_ADR].L = 0x80;
			PIO_WDATA(0x08, FREG);
			FREG[FCTLL_PIO_ADR].L = 0x88;
			PIO_WDATA(0x10, FREG);
			FREG[FCTLL_PIO_ADR].L = 0xB3;
			PIO_WDATA(0x05, FREG);
#endif
			FREG[FCTLL_PIO_CMD].L = 0x16;
		}
		FCONL[FCONL_FCE_ENB] &= (~ce);
	}
}
#endif

UBYTE ftl_Erase_Single_Plane(U32 ulUnitIndex, UBYTE ubPlane, UBYTE ubCEIndex, UWORD uwFJobInfo)
{
	M_CheckRDTModeToDisableFunctionReturnValid(0);
#if(!RDT)
	FlashQueue_t *FQ;
	UBYTE ubDepth, ubFQIndex, ubL4kLink;
	UBYTE ubBank = (ubCEIndex >> gubPlanesPerBurstLog);
#if Hynix
	UBYTE ubChannel = (ubCEIndex & gubPlanesPerBurstMask);
#endif
	ubFQIndex = gFQI.ubFQLinkFree;
	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
	FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
	FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | uwFJobInfo);

	// Must Send Reset CMD(0xFF) After Erase Fail
	FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

	FQ->uwFUnit = ulUnitIndex;
	if (ENABLE_PLANE_CH_CE) {
		FQ->ulFEntry = (((ubPlane & gubBurstsPerBankMask) + (ubCEIndex << gubBurstsPerBankLog)) << gub4kEntrysPerPlaneLog);
	}
	else {
		FQ->ulFEntry = (((ubPlane & gubBurstsPerBankMask) + (ubBank << gubPlanesPerBankLog) + (ubCEIndex & gubPlanesPerBurstMask)) << gub4kEntrysPerPlaneLog);
	}
	FQ->ubRelativeDepth = ubDepth;
	FQ->ubDepth = 0xFF;
	FQ->btUNServed = 1;
	FQ->btCheckEmpty = 0;
	FQ->btD3 = 1;

	if (ENABLE_RUT_SWAP && ENABLE_DEBUG_SYSTEM_UNIT_FAIL) {
		if (gubGenEraseFailDebug) {
			FQ->btGenFail = 1;	// 造erase fail
		}
	}

	gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
	gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
	gFQI.ubFQLinkNumber++;
	gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
#if Hynix
	if (uwFJobInfo == BIT_FJOBI_TLC_TREATMENT) {
		FlaIssueHynixSDPCmd((ubChannel * 0x200), ((U32)BIT0 << (ubChannel * MAX_CE_PER_CH + ubBank)), 0x00);
	}
#endif
	while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
		M_SwitchTask();
	}
#if Hynix
	if (uwFJobInfo == BIT_FJOBI_TLC_TREATMENT) {
		FlaIssueHynixSDPCmd((ubChannel * 0x200), ((U32)BIT0 << (ubChannel * MAX_CE_PER_CH + ubBank)), 0x38);
	}
#endif
	FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
	//gMTQ->Depth[ubDepth].Para0x24.BitMap.btBufModeEn = 1;
	if (gFQI.gFQLink[ubFQIndex].btPEFail) {
		return 1;
	}

	return 0;
#endif
}
#if 0
#if (TLC || ENABLE_ULTRAMLC)
UBYTE ftl_Direct_Erase_Unit(U32 ulUnitIndex, UWORD uwFJobInfo) /* For A2 or C6(TLC treatment) system reserved unit. */
{
	M_CheckRDTModeToDisableFunctionReturnValid(0);

#if(!RDT)
	UBYTE ubi, ubDSelect, ubErase_fail;
	UBYTE ubCEIndex;
	UBYTE ubUnitCount;
	UBYTE ubFQIndex[MAX_LOGICAL_CE][3];

	while (gubNum_D1_Part > 3);

	ubErase_fail = 0;
	gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
	ubDSelect = D3;
	ubUnitCount = (ubDSelect) ? 1 : gubNum_D1_Part;

	for (ubCEIndex = 0; ubCEIndex < gubCENumber; ubCEIndex++) {
		for (ubi = 0; ubi < ubUnitCount ; ubi++) {
			FlashQueue_t *FQ;
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			ubFQIndex[ubCEIndex][ubi] = gFQI.ubFQLinkFree;
			FQ->ubFJob = BYTE_FJOB_ERASE_UNIT;
			FQ->uwFJobInfo = BIT_FJOBI_DIRECTACESS | BIT_FJOBI_WAITRESULT | uwFJobInfo;

			// Must Send Reset CMD(0xFF) After Erase Fail
			FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

			FQ->uwFUnit = ulUnitIndex;
			FQ->ulFEntry = (UWORD)ubi * gulFastPage4kEntrysPerUnit;
			FQ->ubRelativeDepth = ubDepth;
			FQ->ubDepth = 0xFF;
			FQ->btD3 = ubDSelect;
			FQ->btUNServed = 1;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
		}
	}

	for (ubCEIndex = 0; ubCEIndex < gubCENumber; ubCEIndex++) {
		for (ubi = 0; ubi < ubUnitCount ; ubi++) {
			while (gFQI.gFQLink[ubFQIndex[ubCEIndex][ubi]].ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			gFQI.gFQLink[ubFQIndex[ubCEIndex][ubi]].ubFQPhase = BYTE_FQ_CHECKDONE;
			if (gFQI.gFQLink[ubFQIndex[ubCEIndex][ubi]].btPEFail) {
				ubErase_fail = 1;
			}
		}
	}

	return ubErase_fail;
#endif
}
#else
void ftlA2EraseSingleUnit(U32 ulUnitIndex)
{
	UBYTE ubDepth, ubFQIndex, ubL4kLink, ubCEIndex, ubPlaneBank;
	FlashQueue_t *FQ;

	for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
		if (ENABLE_PLANE_CH_CE) {
			ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		}
		else {
			ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + (((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
		}
		ubFQIndex = gFQI.ubFQLinkFree;

		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_ERASE_FAIL_RESET);
		FQ->uwFUnit = ulUnitIndex;
		FQ->ulFEntry = (ubPlaneBank << gub4kEntrysPerPlaneLog);
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
}
#endif
#endif

UBYTE CheckPWICExist()
{
	UBYTE ubAddrSelect = 0x1B, ubReturn;
	U32 ulBackup1;
	ulBackup1 = SYS0L[SYS0L_SYS_MUX_CTRL0];

	SYS0L[SYS0L_SYS_MUX_CTRL0] &= ~(BIT27);
	SYS0L[SYS0L_SYS_MUX_CTRL0] |= BIT26;

	SYS1L[SYS1L_I2C_BUF0_REG] = 0; //clear buffer 0

	SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] &= 0xFFFFFF00;
	SYS1L[SYS1L_I2C_GP1_SLAVE_ADDR] |= (U32)(ubAddrSelect);
	SYS1L[SYS1L_I2C_GP1_OP_REG] = 20;//read only

	////the master device must issue an Hs-mode master
	////code (00001XXX) as the first byte after a START condition
	////to switch the bus to high-speed operation

	SYS1L[SYS1L_I2C_GP1_CTRL] = ((1 << 16) | (1 << 8) | (0 << 4) | I2C_READ); // OP_CNT=1  DATA_CNT=2 BUFF=0 DIR=read | SPEED;

	SYS1L[SYS1L_I2C_GP1_CTRL] |= I2C_START; //Trigger

	while ((SYS1L[SYS1L_I2C_GP1_CTRL] & I2C_START) == 1); //Victor: if there is no I2C slave, maybe it will stuck at here.
	idle(10);
	if ((SYS1L[SYS1L_I2C_GP1_NACK] & GP1_ANACK) == 0) {

		ubReturn = 0;   /// exist
	}
	else {
		ubReturn = 1;   /// not exist
	}

	SYS0L[SYS0L_SYS_MUX_CTRL0] = ulBackup1;

	return ubReturn;
}

#if ENABLE_CODEBLOCKREPLACEMENT
void ftlCheckCodeBlockECC(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	volatile REG_t * pFlaReg;
	FlashQueue_t * FQ;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	U32 ulLDPC_CFG_Temp, ulFCONL_FLH_FUNC_Temp;
	UWORD uwPageIndex, uwECCBitInfo, uwEndPageIndex, uwExamineMinECC;
	UBYTE ubi, ubj, ubCEIndex, ubDepth, ubL4kLink, ubL4KCount , ubNeedChangeCodeBlock , ubReplaceCodeBlockStatus, ubCheckTimes, ubL4kNum, ubCheckECCCnt, ubMaxCheckECCCnt, ubAllSystemBlkCheckedECC;
	UBYTE ubDidReplaceCodeBlock = FALSE, ubFailCnt;
	UBYTE ubWorstCaseSet = 0xFF;
	UBYTE ubSysCodeUnit, ubSysCodeBurst;

	//HandleStopRW(1);
	if (gubSystemSetFreeCount == 0) {
		return;
	}
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
	gubDontUseSortingSystemSet = TRUE;
	gubSystemSetFreeQueueInPTR = gubSystemSetFreeQueueOutPTR + gubSystemSetFreeCount;
	ubCheckTimes = 2;// Kelvin said that FW should check page 0 and  end page
	if (ENABLE_DEBUG_CODEREPLACEMENT_POWERCYCLE) {
		ubCheckTimes = 20;
	}
	ubMaxCheckECCCnt = 1 + gubSystemSetFreeCount + 1;//每一個code block最多換的次數(1 自己)
	ulFCONL_FLH_FUNC_Temp = FCONL[FCONL_FLH_FUNC];
	FCONL[FCONL_FLH_FUNC] |= RDM_REF_ROW_ADR_5_TO_0;
	if (ENABLE_DEBUG_UART_HC) {
		UartString("\n\r===============================");
	}
	//Read Header First
	for (ubi = 0; ubi < 2; ubi++) {
		if (ENABLE_DEBUG_UART_HC) {
			UartString("\n\rCodeBlock ");
			Uart_Tx_DataHex(ubi);
			UartString(":[0]");
			UartWordHex(guwSysCodeBlock[ubi][0]);
			UartString(" :[1]");
			UartWordHex(guwSysCodeBlock[ubi][1]);
		}
		ubCheckECCCnt = 0;
		uwExamineMinECC = 0xFFFF;
		ubAllSystemBlkCheckedECC = FALSE;
		ubFailCnt = 0;
	Mark_Start_Checking_Code_Block_ECC:
		ubNeedChangeCodeBlock = 0;
		ubReplaceCodeBlockStatus = 0;
		for (ubj = 0; ubj < ubCheckTimes; ubj++) {
			if (ubj == 0) {
				ubL4kNum = gub4kEntrysPerPlane - 1;
				uwPageIndex = 0;
				ulLDPC_CFG_Temp = FCONL[FCONL_LDPC_CFG];
				FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
				FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);
			}
			else {
				if (ubj == (ubCheckTimes - 1)) {
					uwPageIndex = uwEndPageIndex - 1;
				}
				else {
					uwPageIndex = ubj;
				}
				ubL4kNum = gub4kEntrysPerPlane;
			}
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				pFlaReg = (volatile REG_t*)gulFlashIP_RegBase[ubCEIndex];
				while (gFQI.ubFQLinkNumber) {
					M_SwitchTask();
				}
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ulRAMAddr = ((U32)&BUFB_BASE[0]);
				FQ->btCheckEmpty = 1;
				FQ->btThisIsErasePage = 0;
				FQ->ubFJob = BYTE_FJOB_READ;

#if ENABLE_TLC_PROGRAM_CODEBLOCK
				FQ->uwFJobInfo =  (((ubj == 0) ? 0 : BIT_FJOBI_FASTPAGE) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#else
				FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#endif

				ubSysCodeUnit = (UBYTE)(guwSysCodeBlock[ubi][ubCEIndex] >> gubBurstsPerBankLog);
				ubSysCodeBurst = (UBYTE)(guwSysCodeBlock[ubi][ubCEIndex] >> gubBurstsPerBankLog);

				FQ->uwFUnit = ubSysCodeUnit;
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((M_Multiplier((U32)uwPageIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubSysCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = (((M_Multiplier((U32)uwPageIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex) << gub4kEntrysPerPlaneLog);
				}
				FQ->ubDepth = ubDepth;
				FQ->ubL4kIndex = ubL4kLink;
				FQ->btUNServed = 1;
				FQ->FQPara.B.btConvBypass = 0;
#if TLC
				FQ->btD3 = 1;
#endif
				gulPara0x24.BitMap.ubFrameNum = ubL4kNum;
				gulPara0x24.BitMap.btBufModeEn = 0;
				//gMTQ->Depth[ubDepth].Para0x04.BitMap.ubALUSel = 1;
				gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
				//gMTQ->Depth[ubDepth].Para0x1C.BitMap.ubCEValue = ubCEIndex / gubPlanesPerBurst;
				gulPara0x24.BitMap.ubL4KNum = ubL4kNum;
				gulPara0x24.BitMap.btZipBypass = 1;
				gulPara0x24.BitMap.btCmpEn = 0;
				gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gMTQ->Depth[ubDepth].Para0x04.BitMap.btRSEncodeMode = 0;
				gMTQ->Depth[ubDepth].ulZipMap = 0;
				for (ubL4KCount = 0; ubL4KCount < ubL4kNum; ubL4KCount++) {
					gMTQ->Depth[ubDepth].ulZipMap |= (0xFF << (8 * ubL4KCount));
				}
				if (ENABLE_DCACHE) {
					xthal_dcache_region_invalidate((void*)FQ->ulRAMAddr, 4096 * ubL4kNum);
				}
				for (ubL4KCount = 0; ubL4KCount < ubL4kNum; ubL4KCount++) {
					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
					//set L4K table info
					pL4KTable.ulL4K_LCA = 0x00000000;
					pL4KTable.Para0x04.ulAll = 0;
					pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
					pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((FQ->ulRAMAddr  + (ubL4KCount * 4096) ) / 512);
					pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
					pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
					*pL4KTablePtr = pL4KTable;
				}

				gFQI.ubFQLinkNumber++;
				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
				while (gFQI.gFQLink[ubL4kLink].ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}
				gFQI.gFQLink[ubL4kLink].ubFQPhase = BYTE_FQ_CHECKDONE;
				if (uwPageIndex == 0) {
					uwEndPageIndex = (UWORD)(1 + BUFW_BASE[HEADERW_S0_PAGES] + BUFW_BASE[HEADERW_S1_PAGES] + BUFW_BASE[HEADERW_S2_PAGES] + BUFW_BASE[HEADERW_S3_PAGES] + BUFW_BASE[HEADERW_S4_PAGES] + BUFW_BASE[HEADERW_S5_PAGES] + BUFW_BASE[HEADERW_S6_PAGES] + BUFW_BASE[HEADERW_S7_PAGES]);
				}
				uwECCBitInfo = pFlaReg[FCTLL_ECC_INFO].L & 0x3FF;
				if (ENABLE_DEBUG_UART_HC) {
					UartString("\n\rCE:");
					Uart_Tx_DataHex(ubCEIndex);
					UartString("  Page:");
					Uart_Tx_DataHex(uwPageIndex);
					UartString("   ECC:");
					UartLongHex(uwECCBitInfo);
				}
				ubCheckECCCnt++;
				if (ubCheckECCCnt == ubMaxCheckECCCnt) {
					ubAllSystemBlkCheckedECC = TRUE;
				}

				if (uwECCBitInfo > 49) { // Kelvin said that ECC bound should be 50
					if (ubAllSystemBlkCheckedECC == 0) {
						if (uwECCBitInfo < uwExamineMinECC) {
							uwExamineMinECC = uwECCBitInfo;
							ubWorstCaseSet = gubSysCodeSet[ubi];
							if (ENABLE_DEBUG_UART_HC) {
								UartString("\n\rWorstCaseSet:");
								Uart_Tx_DataHex(ubWorstCaseSet);
								UartString(" WorstCaseBlock[0]:");
								UartWordHex(guwSysCodeBlock[ubWorstCaseSet][0]);
								UartString(" WorstCaseBlock[1]:");
								UartWordHex(guwSysCodeBlock[ubWorstCaseSet][1]);
							}
						}
						ubNeedChangeCodeBlock = TRUE;
						break;
					}
					else {
						if (gubSysCodeSet[ubi] == ubWorstCaseSet) { //都換滿了直接換成比較好的unit
							if (ENABLE_DEBUG_UART_HC) {
								UartString("\n\rDon't need change.");
							}
						}
						else {
							ubNeedChangeCodeBlock = TRUE;
							break;
						}
					}

				}
				if (ENABLE_DEBUG_CODEREPLACEMENT_POWERCYCLE) {
					if (uwECCBitInfo) {// Kelvin said that ECC bound should be 50
						if (ubAllSystemBlkCheckedECC == 0) {
							if (uwECCBitInfo < uwExamineMinECC) {
								uwExamineMinECC = uwECCBitInfo;
								ubWorstCaseSet = gubSysCodeSet[ubi];
								if (ENABLE_DEBUG_UART_HC) {
									UartString("\n\rNot bad Set:");
									Uart_Tx_DataHex(ubWorstCaseSet);
									UartString(" Block[0]:");
									UartWordHex(guwSysCodeBlock[ubWorstCaseSet][0]);
									UartString(" Block[1]:");
									UartWordHex(guwSysCodeBlock[ubWorstCaseSet][1]);
								}
							}
							ubNeedChangeCodeBlock = TRUE;
							break;
						}
						else {
							if (gubSysCodeSet[ubi] == ubWorstCaseSet) { //都換滿了直接換成比較好的unit
								if (ENABLE_DEBUG_UART_HC) {
									UartString("\n\rDon't need change.");
								}
							}
							else {
								ubNeedChangeCodeBlock = TRUE;
								break;
							}
						}
					}
				}
			}
			if (ubj == 0) {
				FCONL[FCONL_LDPC_CFG] = ulLDPC_CFG_Temp;
			}
			if (ubNeedChangeCodeBlock) {
				break;
			}
		}
		if (ubNeedChangeCodeBlock) {
			if (ENABLE_DEBUG_UART_HC) {
				UartString("\n\rNeed Chcange");
				UartString("   Status:");
			}
			while (1) {
				ubDidReplaceCodeBlock = TRUE;
				ubReplaceCodeBlockStatus = ftlReplaceCodeBlock(gubSysCodeSet[ubi]);

				if (ENABLE_DEBUG_UART_HC) {
					Uart_Tx_DataHex(ubReplaceCodeBlockStatus);
				}

				if (ubReplaceCodeBlockStatus == 0) {
					VT->gulCodeBlkReplaceCnt++;
					goto Mark_Start_Checking_Code_Block_ECC;
				}
				else {
					VT->gulCodeBlkReplaceFailCnt++;
					ubFailCnt++;
					ubCheckECCCnt++;
					if (ubFailCnt == (ubMaxCheckECCCnt - 1)) {
						M_AssertError(0xCB);
					}
				}
			}
		}
	}
	if (ENABLE_DEBUG_UART_HC) {
		for (ubi = 0; ubi < 2; ubi++) {
			UartString("\n\rFinishCodeSet ");
			Uart_Tx_DataHex(ubi);
			UartString(":");
			Uart_Tx_DataHex(gubSysCodeSet[ubi]);
		}
		UartString("\n\r\n\r");
	}
	FCONL[FCONL_FLH_FUNC] = ulFCONL_FLH_FUNC_Temp;
	gubDontUseSortingSystemSet = FALSE;
	if (ubDidReplaceCodeBlock) {
		for (ubi = 0; ubi < SYSTEM_SETS; ubi++) {
			if (ENABLE_DEBUG_UART_HC) {
				if (ubi == 0) {
					UartString("\n\r");
				}
				Uart_Tx_DataHex(gubSystemSetFreeTable[ubi]);
				UartString("  ");
			}
			gubSystemSetFreeTable[ubi] = 0xFF;
		}
		ScanSystemFreeSet(0);
	}
}

UBYTE ftlReplaceCodeBlock(UBYTE ubCodeSet)
{
	M_CheckBurnerModeToDisableFunctionReturnValid(1);
	M_CheckRDTModeToDisableFunctionReturnValid(1);
	FlashQueue_t * FQ;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	U32 ulFCONL_FLH_FUNC_Temp, ulLDPC_CFG_Temp, ulLCAMark;
	UWORD uwPlaneNum, uwPlaneIndex, uwFWValue, ubReplaceCodeSet;
	UBYTE ubL4kLink, ubDepth, ubL4KCount, ubL4KNum;
	UBYTE ubi, ubj, ubCEIndex, ubReadCE;
	UBYTE ubReplaceCodeUnit = 0xFF, ubReplaceCodeBurst = 0xFF;
	UBYTE ubReadFail = 0, ubWriteFail = 0, ubFailMaxCnt;
	UBYTE ubReturnStatus, ubCodeVersion;
	UBYTE ubCodeUnit, ubCodeBurst;
	//HandleStopRW(1);
	ubReturnStatus = 0;
	//  1 沒有辦法讀到欲替換的code block header
	//  2 無法erase要當code block的unit
	//  3 沒辦法讀取code block的內容
	//  4 讀不到欲copy的data
	//  5 copy過程發生fail
	//  6 傳錯誤的code unit進來
	ubReplaceCodeSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_HEAD);

	for (ubi = 0; ubi < 2; ubi++) {
		if (ubCodeSet == gubSysCodeSet[ubi]) {
			break;
		}
	}
	if (ubi == 2) {
		ubReturnStatus = 6;
		return ubReturnStatus;
	}

	ubFailMaxCnt = gubPlanesPerBurst;

	ulFCONL_FLH_FUNC_Temp = FCONL[FCONL_FLH_FUNC];
	FCONL[FCONL_FLH_FUNC] |= RDM_REF_ROW_ADR_5_TO_0;
	ulLDPC_CFG_Temp = FCONL[FCONL_LDPC_CFG];
	FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
	FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);

	//Read Header First
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ulRAMAddr = ((U32)&BUFB_BASE[0]);
		FQ->btCheckEmpty = 1;
		FQ->btThisIsErasePage = 0;
		FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
		FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#else
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#endif

		ubCodeUnit = (UBYTE)(guwSysCodeBlock[ubi][ubCEIndex] >> gubBurstsPerBankLog);
		ubCodeBurst = (UBYTE)(guwSysCodeBlock[ubi][ubCEIndex] & gubBurstsPerBankMask);

		FQ->uwFUnit = ubCodeUnit;
		if (ENABLE_PLANE_CH_CE) {
			FQ->ulFEntry = ((M_Multiplier(0, DEF_PlanesPerPage_Mcl_Div) + (ubCEIndex << gubBurstsPerBankLog) + ubCodeBurst) << gub4kEntrysPerPlaneLog);
		}
		else {
			FQ->ulFEntry = ((M_Multiplier(0, DEF_PlanesPerPage_Mcl_Div) + ubCEIndex) << gub4kEntrysPerPlaneLog);
		}
		FQ->ubDepth = ubDepth;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btUNServed = 1;
		FQ->FQPara.B.btConvBypass = 0;
#if TLC
		FQ->btD3 = 1;
#endif
		gulPara0x24.BitMap.ubFrameNum = 1;
		gulPara0x24.BitMap.btBufModeEn = 0;
		//gMTQ->Depth[ubDepth].Para0x04.BitMap.ubALUSel = 1;
		gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
		//gMTQ->Depth[ubDepth].Para0x1C.BitMap.ubCEValue = ubCEIndex / gubPlanesPerBurst;
		gulPara0x24.BitMap.ubL4KNum = 1;
		gulPara0x24.BitMap.btZipBypass = 1;
		gulPara0x24.BitMap.btCmpEn = 0;
		gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
		gMTQ->Depth[ubDepth].Para0x04.BitMap.btRSEncodeMode = 0;
		gMTQ->Depth[ubDepth].ulZipMap = 0x000000FF;
		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate((void*)FQ->ulRAMAddr, 4096);
		}
		for (ubL4KCount = 0; ubL4KCount < 1; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA = 0x00000000;
			pL4KTable.Para0x04.ulAll = 0;
			pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((FQ->ulRAMAddr  + (ubL4KCount * 4096) ) / 512);
			pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
			*pL4KTablePtr = pL4KTable;
		}

		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
		while (gFQI.gFQLink[ubL4kLink].ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		if (gFQI.gFQLink[ubL4kLink].btReadUNC) {
			ubReadFail++;
			continue;
		}
		if (gFQI.gFQLink[ubL4kLink].btThisIsErasePage) {
			ubReadFail++;
			continue;
		}
		gFQI.gFQLink[ubL4kLink].ubFQPhase = BYTE_FQ_CHECKDONE;
		if (ENABLE_DCACHE) {
			xthal_dcache_region_writeback_inv((void*)FQ->ulRAMAddr, 4096 * gub4kEntrysPerPlane);
		}
		if ((pL4KTablePtr->ulL4K_LCA == HEADER_LCA)) {
			uwPlaneNum = (UWORD)(1 + BUFW_BASE[HEADERW_S0_PAGES] + BUFW_BASE[HEADERW_S1_PAGES] + BUFW_BASE[HEADERW_S2_PAGES] + BUFW_BASE[HEADERW_S3_PAGES] + BUFW_BASE[HEADERW_S4_PAGES] + BUFW_BASE[HEADERW_S5_PAGES] + BUFW_BASE[HEADERW_S6_PAGES] + BUFW_BASE[HEADERW_S7_PAGES]);
			ubCodeVersion = (UBYTE)(pL4KTablePtr->Para0x04.BitMap.ulL4K_FW >> 8);
			ubReadCE = ubCEIndex;
			break;
		}
	}

	if (ubReadFail == ubFailMaxCnt) {
		ubReturnStatus = 1;// 1 沒有辦法讀到欲替換的code block header
		goto Mark_Replace_Fail;
	}

MarK_Erase_Replace_Code_Unit:
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		//erase
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif

		// Must Send Reset CMD(0xFF) After Erase Fail
		FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

		ubReplaceCodeUnit = (UBYTE)(guwSystemBlockRemappingTable[ubReplaceCodeSet][ubCEIndex] >> gubBurstsPerBankLog);
		ubReplaceCodeBurst = (UBYTE)(guwSystemBlockRemappingTable[ubReplaceCodeSet][ubCEIndex] & gubBurstsPerBankMask);

		FQ->uwFUnit = ubReplaceCodeUnit;
		if (ENABLE_PLANE_CH_CE) {
			FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubReplaceCodeBurst) << gub4kEntrysPerPlaneLog);
		}
		else {
			FQ->ulFEntry = (ubCEIndex << gub4kEntrysPerPlaneLog);
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
		if (FQ->btPEFail) {
			ubWriteFail++;
			break;
		}
	}

	if (ubWriteFail) {
		ubReturnStatus = 2;	//  2 無法erase要當code block的unit
		goto Mark_Replace_Fail;
	}

#if Hynix
	FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
Mark_Copy_Code_Unit:
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		for (uwPlaneIndex = 0; uwPlaneIndex < uwPlaneNum; uwPlaneIndex++) {
			ubReadFail = 0;
			ubWriteFail = 0;
		Mark_Read_Code_Unit:
			if (uwPlaneIndex != 0) {
				ulLCAMark = CODE_LCA;
				ubL4KNum = gub4kEntrysPerPlane;
				FCONL[FCONL_LDPC_CFG] = ulLDPC_CFG_Temp;
			}
			else {
				mDMAC_SV(4, (U32)BUFB_BASE,  gub4kEntrysPerPlane * 4096, 0, 0, 0, DMA_EN_SETCQ, 0);
				ulLCAMark = HEADER_LCA;
				ubL4KNum = 1;
				FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
				FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);
			}
			//Read
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubReadCE);
			FQ->ulRAMAddr = ((U32)&BUFB_BASE[0]);
			FQ->btCheckEmpty = 1;
			FQ->btThisIsErasePage = 0;
			FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
			FQ->uwFJobInfo = ( ((uwPlaneIndex != 0) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);

#else
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);

#endif




			ubCodeUnit = (UBYTE)(guwSysCodeBlock[ubi][ubCEIndex] >> gubBurstsPerBankLog);
			ubCodeBurst = (UBYTE)(guwSysCodeBlock[ubi][ubCEIndex] & gubBurstsPerBankMask);

			FQ->uwFUnit = ubCodeUnit;
			if (ENABLE_PLANE_CH_CE) {
				FQ->ulFEntry = (((M_Multiplier((U32)uwPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubCodeBurst) << gub4kEntrysPerPlaneLog);
			}
			else {
				FQ->ulFEntry = (((M_Multiplier((U32)uwPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex) << gub4kEntrysPerPlaneLog);
			}
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btUNServed = 1;
			FQ->FQPara.B.btConvBypass = 0;
#if TLC
			FQ->btD3 = 1;
#endif
			gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
			gulPara0x24.BitMap.btBufModeEn = 0;
			//gMTQ->Depth[ubDepth].Para0x04.BitMap.ubALUSel = 1;
			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
			//gMTQ->Depth[ubDepth].Para0x1C.BitMap.ubCEValue = ubCEIndex / gubPlanesPerBurst;
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].Para0x04.BitMap.btRSEncodeMode = 0;
			gMTQ->Depth[ubDepth].ulZipMap = 0;
			for (ubj = 0; ubj < ubL4KNum; ubj++) {
				gMTQ->Depth[ubDepth].ulZipMap |= (0xFF << (8 * ubj));
			}
			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate((void*)FQ->ulRAMAddr, 4096 * gub4kEntrysPerPlane);
			}
			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = 0x00000000;
				pL4KTable.Para0x04.ulAll = 0;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((FQ->ulRAMAddr  + (ubL4KCount * 4096) ) / 512);
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				*pL4KTablePtr = pL4KTable;
			}

			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
			while (gFQI.gFQLink[ubL4kLink].ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			if (FQ->btReadUNC) {
				ubReadFail++;
				if (ubReadFail == ubFailMaxCnt) {
					ubReturnStatus = 3;//  3 沒辦法讀取code block的內容
					goto Mark_Replace_Fail;
				}
				else {
					ubReadCE++;
					ubReadCE &= gubPlanesPerBurstMask;
					goto Mark_Read_Code_Unit;
				}
			}
			if (FQ->btThisIsErasePage) {
				ubReadFail++;
				if (ubReadFail == ubFailMaxCnt) {
					ubReturnStatus = 3; //  3 沒辦法讀取code block的內容
					goto Mark_Replace_Fail;
				}
				else {
					ubReadCE++;
					ubReadCE &= gubPlanesPerBurstMask;
					goto Mark_Read_Code_Unit;
				}
			}
			FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
			if ((pL4KTablePtr->ulL4K_LCA != ulLCAMark)) {
				ubReadFail++;
				if (ubReadFail == ubFailMaxCnt) {
					ubReturnStatus = 4; //  4 讀不到欲copy的data
					goto Mark_Replace_Fail;
				}
				else {
					ubReadCE++;
					ubReadCE &= gubPlanesPerBurstMask;
					goto Mark_Read_Code_Unit;
				}
			}
			else {
				uwFWValue =  ((UWORD)ubCodeVersion << 8) + (pL4KTablePtr->Para0x04.BitMap.ulL4K_FW & 0xFF); ;
				if (uwPlaneIndex == 0) {
					UBYTE ubCodeIndexFW;
					ubL4KNum = gub4kEntrysPerPlane;
					ubCodeIndexFW = (pL4KTablePtr->Para0x04.BitMap.ulL4K_FW & 0xFF) - ubReadCE * 2;
					uwFWValue = ((UWORD)ubCodeVersion << 8) + (ubCodeIndexFW + ubCEIndex * 2) ;
					BUFB_BASE[HEADERB_FW_REVISION_ID] = ubCodeVersion;
					BUFW_BASE[ HEADERW_CH0_CODE0_Block] = guwSystemBlockRemappingTable[ubReplaceCodeSet][0]; //uwStartBlock + (ubVersion * 2 * ubNext);
					BUFW_BASE[ HEADERW_CH0_CODE1_Block] = guwSystemBlockRemappingTable[ubReplaceCodeSet][0]; //uwStartBlock + ubNext + (ubVersion * 2 * ubNext);
					BUFW_BASE[ HEADERW_CH1_CODE0_Block] = guwSystemBlockRemappingTable[ubReplaceCodeSet][1]; //uwStartBlock + (ubVersion * 2 * ubNext);
					BUFW_BASE[ HEADERW_CH1_CODE1_Block] = guwSystemBlockRemappingTable[ubReplaceCodeSet][1]; //uwStartBlock + ubNext + (ubVersion * 2 * ubNext);
					BUFL_BASE[(4096 - 4) / 4] = mDMAC_CRC((U32)BUFW_BASE, 1 * 4096, HEADER_CRC32_SEED, 1, (DMA_CRC_LEN_8 | DMA_CRC_DCCM));
				}
			}

		Mark_Write_Replace_Code_Unit:
			//Write
			UBYTE ubLMU;
			UBYTE btIsTLCProgram = (ENABLE_TLC_PROGRAM_CODEBLOCK && (uwPlaneIndex == 0));
			for (ubLMU = 0; ubLMU < (btIsTLCProgram ? 3 : 1); ubLMU++) {
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ulRAMAddr = (U32)&BUFB_BASE[0];
				FQ->ubFJob = BYTE_FJOB_WRITE;
				FQ->uwFJobInfo = ((btIsTLCProgram ? 0 : BIT_FJOBI_FASTPAGE) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);

				ubReplaceCodeUnit = (UBYTE)(guwSystemBlockRemappingTable[ubReplaceCodeSet][ubCEIndex] >> gubBurstsPerBankLog);
				ubReplaceCodeBurst = (UBYTE)(guwSystemBlockRemappingTable[ubReplaceCodeSet][ubCEIndex] & gubBurstsPerBankMask);
				FQ->uwFUnit = ubReplaceCodeUnit;
				if (ENABLE_PLANE_CH_CE) {
					FQ->ulFEntry = (((M_Multiplier((U32)uwPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubReplaceCodeBurst) << gub4kEntrysPerPlaneLog);
				}
				else {
					FQ->ulFEntry = (((M_Multiplier((U32)uwPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + ubCEIndex) << gub4kEntrysPerPlaneLog);
				}
				FQ->ubRelativeDepth = ubDepth;
				FQ->ubDepth = 0xFF;
				FQ->ubL4kIndex = ubL4kLink;
				FQ->btUNServed = 1;
#if TLC
				FQ->btD3 = 1;
#endif
				if (btIsTLCProgram) {
					FQ->btTLCProgramCodeBlock = 1;
					FQ->ubD3WriteMode = (D3_SUSPEND_EN) | D3_WRITE_FINE | (D3_WRITE_LOWDER << ubLMU);
				}

				gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
				gulPara0x24.BitMap.btBufModeEn = 0;
				gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
				gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
				gulPara0x24.BitMap.btZipBypass = 1;
				gulPara0x24.BitMap.btCmpEn = 0;
				gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gMTQ->Depth[ubDepth].Para0x04.BitMap.btRSEncodeMode = 0;
				if (ENABLE_DCACHE) {
					xthal_dcache_region_writeback_inv((void*)FQ->ulRAMAddr, 4096 * gub4kEntrysPerPlane);
				}
				for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
					//set L4K table info
					pL4KTable.ulL4K_LCA = ulLCAMark;
					pL4KTable.Para0x04.BitMap.ulL4K_FW = uwFWValue;//((1 + ubCEIndex * 2) + (ubCodeVersion << 8));
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
				if (FQ->btPEFail) {
					ubReturnStatus = 5; //  5 copy過程發生fail
					goto Mark_Replace_Fail;
				}
			}


		}
	}


Mark_Erase_Old_System_Block:

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		//erase
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);

		// Must Send Reset CMD(0xFF) After Erase Fail
		FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

		ubCodeUnit = (UBYTE)(guwSysCodeBlock[ubi][ubCEIndex] >> gubBurstsPerBankLog);
		ubCodeBurst = (UBYTE)(guwSysCodeBlock[ubi][ubCEIndex] & gubBurstsPerBankMask);

		FQ->uwFUnit = ubCodeUnit;
		if (ENABLE_PLANE_CH_CE) {
			FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubCodeBurst) << gub4kEntrysPerPlaneLog);
		}
		else {
			FQ->ulFEntry = (ubCEIndex << gub4kEntrysPerPlaneLog);
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

	}
	ftlAddToSystemSetFreeTable(ubCodeSet);

	gubSysCodeSet[ubi] = ubReplaceCodeSet;
	guwSysCodeBlock[ubi][0] = guwSystemBlockRemappingTable[ubReplaceCodeSet][0];
	guwSysCodeBlock[ubi][1] = guwSystemBlockRemappingTable[ubReplaceCodeSet][1];


Mark_Replace_Success:
	FCONL[FCONL_FLH_FUNC] = ulFCONL_FLH_FUNC_Temp;
	FCONL[FCONL_LDPC_CFG] = ulLDPC_CFG_Temp;
	return ubReturnStatus;

Mark_Replace_Fail:
#if Hynix
	FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
Mark_Erase_Choose_Replace_Block:
	ubWriteFail = 0;
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		ubWriteFail = 0;
		//erase
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);

		// Must Send Reset CMD(0xFF) After Erase Fail
		FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

		ubReplaceCodeUnit = (UBYTE)(guwSystemBlockRemappingTable[ubReplaceCodeSet][ubCEIndex] >> gubBurstsPerBankLog);
		ubReplaceCodeBurst = (UBYTE)(guwSystemBlockRemappingTable[ubReplaceCodeSet][ubCEIndex] & gubBurstsPerBankMask);
		FQ->uwFUnit = ubReplaceCodeUnit;
		if (ENABLE_PLANE_CH_CE) {
			FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubReplaceCodeBurst) << gub4kEntrysPerPlaneLog);
		}
		else {
			FQ->ulFEntry = (ubCEIndex << gub4kEntrysPerPlaneLog);
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
		if (FQ->btPEFail) {
			ubWriteFail++;
			break;
		}
	}
	if (ubWriteFail == 0) {
		ftlAddToSystemSetFreeTable(ubReplaceCodeSet);
	}

	FCONL[FCONL_FLH_FUNC] = ulFCONL_FLH_FUNC_Temp;
	FCONL[FCONL_LDPC_CFG] = ulLDPC_CFG_Temp;
	return ubReturnStatus;
}
#endif

UBYTE HandleDBTSetLocation(U32 ulDBTAddr, UBYTE ubMode)
{
	UBYTE *ubDBTBuffer = (UBYTE *)ulDBTAddr;
	//FW預期要用的SystemRule有可能會被MP強迫改變,所以這邊SetNumber要重算
	UBYTE ubCEIndex = (guwDBTBlock[0] != 0xFFFF) ? 0 : 1; //決定用那一個CE為主去算Set Number
	gubDBTSet = ftlGetSystemSetNumber(ubMode, ((U32)&ubDBTBuffer[0]), ubCEIndex, guwDBTBlock[ubCEIndex]);

	if (gubDBTSet == 0xFF)
		return 1;

	//因為在erase 0x01後在這個時間點無法確定DBT原本的排列方法是Old planeA還是New Remapping,
	//所以在這邊要強迫把DBTblock改在預期SystemRule擺放的位子
	if (gubPlanesPerBurst == MAX_CHANNEL) {
		UWORD uwAnotherBlock;
		//用DBT推出另一個是誰
		uwAnotherBlock = ftlGetAnotherSystemBlockInSet(ubMode, ((U32)&ubDBTBuffer[0]), ubCEIndex, guwDBTBlock[ubCEIndex]);

		if (uwAnotherBlock == 0xFFFF)
			return 1;

		if ((guwDBTBlock[!ubCEIndex] != 0xFFFF) && (guwDBTBlock[!ubCEIndex] != uwAnotherBlock)) {
			//原本mapping的DBT block在eraseAll時沒被erase到,這邊補erase
			ftl_Erase_Single_Plane((guwDBTBlock[!ubCEIndex] >> gubBurstsPerBankLog) , (guwDBTBlock[!ubCEIndex] & gubBurstsPerBankMask), !ubCEIndex, BIT_FJOBI_FASTPAGE);
			if (ENABLE_DEBUG_UART_PREFORMAT) {
				UartString("\nErase CE:");
				Uart_Tx_DataHex(!ubCEIndex);
				UartString("\nBlock:");
				UartWordHex(guwDBTBlock[!ubCEIndex]);
			}
		}
		else {
			if (ENABLE_DEBUG_UART_PREFORMAT) {
				UartString("\nNot Erase");
			}
		}
		guwDBTBlock[!ubCEIndex] = uwAnotherBlock;
	}

	return 0;
}

UBYTE ftlIsMixPlaneUnit(UWORD uwTarget)
{

	UBYTE ubBurstBank, ubCEIndex;
	UWORD uwFBlock;

	//How to Check MixPlane Unit >>> At One Unit / Any CE / Have the Same PlaneBank
	for (ubCEIndex = 0; ubCEIndex < gubCENumber; ubCEIndex++) { //Any CE
		UBYTE ubPhyBurst = 0;

		for (ubBurstBank = 0; ubBurstBank < gubBurstsPerBank; (ubBurstBank ++)) {
			uwFBlock = M_RUT2DTo1D(ubCEIndex , (uwTarget << gubBurstsPerBankLog) + ubBurstBank);

			if (ubBurstBank && (ubPhyBurst & (BIT0 << (uwFBlock & gubBurstsPerBankMask)))) {
				return 1; //Same PlaneBank. It's a MixPlane Unit.
			}
			ubPhyBurst |= (BIT0 << (uwFBlock & gubBurstsPerBankMask));

		}
	}
	return 0;
}

void ftlSearchMixPlaneInFreeBlockTable()
{

	if (!gubRUTMixPlaneEnable) {
		return;
	}

	//Avoiding Using Mixplane Unit (MLC Normal & TLC SLC Pool)

	FTarget_t *uwFreeBlockTable;
	EraseCount_t *ulEC;
	UWORD uwFreeBlockCount;

	uwFreeBlockTable = guwFreeBlockTable_D3;
	ulEC = (EraseCount_t*)gulEC_D3;
	uwFreeBlockCount = VT->guwFreeBlockCount_D3;
	UWORD uwi;

	//Checking All FreeBlockTable_D3 are MixPlane Unit or Not
	//Only Download Mirco Code will Do it.

	for (uwi = 2; uwi < uwFreeBlockCount + 2; uwi++) {
		ulEC[uwFreeBlockTable[uwi].B.uwTarget].B.btMixPlaneUnit = ftlIsMixPlaneUnit(uwFreeBlockTable[uwi].B.uwTarget);
	}

}


void ftlSwapD1MixPlaneUnit(U32 *ulPlaneBankCounterPtr, UBYTE ubGroupIndex, U32 ulD1UnitNum)
{

	if (!gubRUTMixPlaneEnable) {
		return;
	}

	UBYTE ubPlaneBank = 0;
	UWORD uwNonMixPlaneUnitBoundary = ulPlaneBankCounterPtr[0];
	for (ubPlaneBank = 1; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
		if (uwNonMixPlaneUnitBoundary > ulPlaneBankCounterPtr[ubPlaneBank]) {
			uwNonMixPlaneUnitBoundary = ulPlaneBankCounterPtr[ubPlaneBank];
		}
	}

	//決定三個變數 1.從哪裡開始搬(uwD3SwapStartIndex) 2.要搬到哪裡(固定D1底) 3.要搬多少個(uwSwapNum)

	//根據 Non MixPlane Unit 的數量, 有 4 種可能狀況

	//Case A:
	//	Non MixPlane Unit 大於 D3 + D1  >>>  不用做, return
	//Case B:
	//	Non MixPlane Unit 大於D3 小於D3 + D1  >>>  換D3 + D1 - bulNonMixPlaneUnitCounter
	//	Swap數量 : D3 + D1 - bulNonMixPlaneUnitCounter
	//	Swap範圍 : [D3 ~ D3 - (D3 + D1 - bulNonMixPlaneUnitCounter)] <--> [D3 + D1 ~ D3 + D1 - (D3 + D1 - bulNonMixPlaneUnitCounter)]
	//Case C:
	//	Non MixPlane Unit 小於D3 && D1 < Non MixPlane Unit - VTArea  >>>  數量夠換整個D1區, D1全換
	//	Swap數量 : D1
	//	Swap範圍 : [uwNonMixPlaneUnitBoundary ~ uwNonMixPlaneUnitBoundary - D1] <--> [D3 + D1 ~ D3]
	//Case D:
	//	Non MixPlane Unit 小於D3 && D1 > Non MixPlane Unit - VTArea  >>>  數量不夠換整個D1區, 只換部分
	//	Swap數量 : bulNonMixPlaneUnitCounter - VTArea
	//	Swap範圍 : [uwNonMixPlaneUnitBoundary ~ uwNonMixPlaneUnitBoundary-VTArea] <--> [D3 + D1 ~ D3 + D1 - (bulNonMixPlaneUnitCounter - VTArea)]

	//Case A:
	//VT->guwUnitsPerGroups[ubGroupIndex] = Avg - ReserveUnit, maybe uwNonMixPlaneUnitBoundary is more.
	if ((uwNonMixPlaneUnitBoundary >= VT->guwUnitsPerGroups[ubGroupIndex] - (ubGroupIndex ? VT->guwUnitsPerGroups[ubGroupIndex - 1] : 0)) || uwNonMixPlaneUnitBoundary <= VTAREA_UNITS) {
		//D3 & D1 don't have any Mixplane-Unit.
		return;
	}


	//Case B:
	UWORD uwSwapNum = VT->guwUnitsPerGroups[ubGroupIndex] - (ubGroupIndex ? VT->guwUnitsPerGroups[ubGroupIndex - 1] : 0) - uwNonMixPlaneUnitBoundary;
	UWORD uwD3SwapStartIndex = VT->guwUnitsPerGroups[ubGroupIndex] - ulD1UnitNum * D1_UNIT_NUM_PER_LINK;

	//MixPlane的數量已淹到D3區? (if true -> Case C & Case D)
	if (uwSwapNum > ulD1UnitNum * D1_UNIT_NUM_PER_LINK) {
		//D3是否有足夠數量可換?
		if (uwNonMixPlaneUnitBoundary - VTAREA_UNITS > ulD1UnitNum * D1_UNIT_NUM_PER_LINK) {	//顧慮Debug 不想換VT
			//Case C:
			//Maximum D1 Count
			uwSwapNum = ulD1UnitNum * D1_UNIT_NUM_PER_LINK;
		}
		else {
			//Case D:
			//Maximum NonMixPlaneUnit Count
			uwSwapNum = uwNonMixPlaneUnitBoundary - VTAREA_UNITS;
		}
		uwD3SwapStartIndex = uwNonMixPlaneUnitBoundary + (ubGroupIndex ? VT->guwUnitsPerGroups[ubGroupIndex - 1] : 0);
	}

	// 1 Unit Size = Number of Plane(gubBurstsPerBank) * 2Bytes
	UBYTE ubRUTUnitSizePerCE = 2 << gubBurstsPerBankLog;
	//降低計算, 原本 [Index - 1] 的 '1' 轉嫁給uwi (uwi = 0 => uwi = 1, while(uwi < uwSwapNum) => while(uwi <= uwSwapNum))
	UBYTE ubCEIndex = 0;
	for (ubCEIndex = 0; ubCEIndex < gubCENumber; ubCEIndex++) {
		memcpy(&gubCopyBuffer[0], &M_RUT2DTo1D(ubCEIndex, ((uwD3SwapStartIndex - uwSwapNum) << (gubBurstsPerBankLog))), ubRUTUnitSizePerCE * uwSwapNum);
		memcpy(&M_RUT2DTo1D(ubCEIndex, ((uwD3SwapStartIndex - uwSwapNum) << (gubBurstsPerBankLog))), &M_RUT2DTo1D(ubCEIndex, ((VT->guwUnitsPerGroups[ubGroupIndex] - uwSwapNum) << (gubBurstsPerBankLog))), ubRUTUnitSizePerCE * uwSwapNum);
		memcpy(&M_RUT2DTo1D(ubCEIndex, ((VT->guwUnitsPerGroups[ubGroupIndex] - uwSwapNum) << (gubBurstsPerBankLog))), &gubCopyBuffer[0], ubRUTUnitSizePerCE * uwSwapNum);
	}

}

UBYTE ftlPreformat(void)
{
	//#if(!RDT)
	M_CheckRDTModeToDisableFunctionReturnValid(1);
	UWORD uwTotalReduceUnit;
	UBYTE ubi, ubj, ubPlaneBank, ubCEIndex, ubGroup, ubCEGroup, ubDie, ubCodeNum, ubMPSetUnit = 0, ubBurst;
	U32 ulplane, ulGroupStartUnit, ulL4K_FW, ulUnitPTR;
	U32 ulFreeUnitPTR, ulFreeSystemUnitPTR, uli, ulj, ulFreeblock, TotalBlockNum, ulLogicUnitPTR, ulCheckEraseFailBlkIndex;
	U32  ulUnitsPerGroup, ulDMAAddr;
	U32 *ulPlaneBank_ptr = (U32 *)((&gulVenderSMARTBuffer[0]) + 0x10000);
	U32 *ulPlaneBank_Counter = (U32 *)((&gulVenderSMARTBuffer[0]) + 0x10000 + 256 * 4);
	U32 *ulPlaneBank_ptr_D1 = (U32 *)((&gulVenderSMARTBuffer[0]) + 0x10000  +  256 * 4 * 2 + 64 * 4 * 2);
#if TLC
	U32 *ulPlaneBank_Counter_D1 = (U32 *)((&gulVenderSMARTBuffer[0]) + 0x10000 +  256 * 4 * 2 + 64 * 4 * 2);
#endif
	UBYTE *ubDBTBuffer = (UBYTE *)(&GCGRSearchTable[0]);
	UBYTE *ubTempBuffer = (UBYTE *)((&gulVenderSMARTBuffer[0]) + 0x8000);
	UBYTE ubDBTMappingPlaneIndex;
	UBYTE ubDBTSizeInPlanes;
	UBYTE ubbadblock;
	UBYTE ubCodeBlkNum = 0;
	U32 ulStartUnit[MAX_CHANNEL];
	UBYTE ubStartBurst[MAX_CHANNEL];
	UBYTE ubGetFreeBlock;
	UBYTE ubCodeSetCheck = 0; //Ch0:BIT0, Ch1,BIT1
	UBYTE ubSYSRemappingSetPtr = 0;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	UBYTE ubParityPlaneIndex = Table_RSFrameStartIndex;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	SYSTEM_GROUP_t *InfoArray = (SYSTEM_GROUP_t *)&BUFB_BASE[0];
	UBYTE ubKeepSMARTwhenKeepEC;
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	ulUnitsPerGroup = gulTotalPhyUnitNum >> gubTotalGroupsLog;
	ubCEGroup = gubTotalGroups >> gubDieNumberLog;  //gubDieNumberLog原來沒算 要記得加 Sosa

#if EnableProductHistory
	UBYTE ubFind = 0;
	UWORD uwUnittemp;
	UBYTE ubCEIndextemp;
	UBYTE ubPlaneBanktemp;
	UBYTE ubDieIndextemp;

	if ((gubPHinit & 0xF) == 0) { //沒有init過
		if (RDT_FindDBTBlock_Task()) {
			//no DBT
			if (FindPHBlockByScan(0)) {
				gubPHinit |= Bit3_InitByScan_NotFind;//scan fail
			}
			else {
				gubPHinit |= Bit2_InitByScan_Find;//scan pass
			}

		}
		else {
			if (FindPHBlockByDBT()) {
				//DBT沒找到PH mark ->fail
				gubPHinit |= Bit1_InitByDBT_NotFind;
				if (FindPHBlockByScan(0)) {
					gubPHinit |= Bit3_InitByScan_NotFind;//scan fail
				}
				else {
					gubPHinit |= Bit2_InitByScan_Find;//scan pass
				}
			}
			else {
				gubPHinit |= Bit0_InitByDBT_Find;//DBT找到PH mark->pass
			}
		}
	}
	//DBT已經有標 ubfind++
	//或Scan已經有標 ubfind++
	if (((gubPHmatch[0]&Bit0_FindByDBT) == BIT0) && (guwPHUnit[0].uwUnit != PH_Default) && (guwPHUnit[0].ubPlanebank != 0xFF)) {
		ubFind++;
	}
	else if (((gubPHmatch[0]&Bit1_FindByScan) == BIT1) && (guwPHUnit[0].uwUnit != PH_Default) && (guwPHUnit[0].ubPlanebank != 0xFF)) {
		ubFind++;
	}

	if (((gubPHmatch[1]&Bit0_FindByDBT) == BIT0) && (guwPHUnit[1].uwUnit != PH_Default) && (guwPHUnit[1].ubPlanebank != 0xFF)) {
		ubFind++;
	}
	else if (((gubPHmatch[1]&Bit1_FindByScan) == BIT1) && (guwPHUnit[1].uwUnit != PH_Default) && (guwPHUnit[1].ubPlanebank != 0xFF)) {
		ubFind++;
	}

	//UartString("\nmatch0:");
	//UartLongHex(gubPHmatch[0]);
	//UartString("\nmatch1:");
	//UartLongHex(gubPHmatch[1]);

#endif


	if (InfoArray->ubKeepSMARTwhenKeepEC == 0) {
		ubKeepSMARTwhenKeepEC = ENABLE_KEEP_SMART_WHEN_KEEP_EC;
	}
	else if (InfoArray->ubKeepSMARTwhenKeepEC == 1) {
		ubKeepSMARTwhenKeepEC = 1;
	}
	else {
		ubKeepSMARTwhenKeepEC = 0;
	}

	UWORD uwPlaneBankIndex;
	//計算所需要的Unit數量
#if (ENABLE_ULTRAMLC)
	U32 ulUnitSize = (U32)(gulPlanesPerUnit - gubMaxPlanesPerP2LTable - (guwParityMappingCount * RS_PARITY_PLANES)) * gubSectorsPerPlane; // sectors
#else
	U32 ulUnitSize = (U32)(gulPlanesPerUnit - gubMaxPlanesPerP2LTable - ((gulPlanesPerUnit / guwParityPlanesNum + 1) * RS_PARITY_PLANES)) * gubSectorsPerPlane; // sectors
#endif

	U32 ulNativeUnit = (gulNativeSize / ulUnitSize);
	if (gulNativeSize % ulUnitSize) {
		ulNativeUnit++;
	}
#if TSB_BICS4_SUPPORT
	U32 ulNeededUnitNum = ((ulNativeUnit * 100) / 99) + 7 + gubTableUnitNum + FreeBlockCountThatNeedCloseTarget + ((guoFlashUseType.B.btToshibaBiCs4 ? gubWriteProtect_NUM : WriteProtect_NUM) << gubSelTotalGroupsLog); //7=VT(4)+VTchild+RUT+Parity
#else
	U32 ulNeededUnitNum = ((ulNativeUnit * 100) / 99) + 7 + gubTableUnitNum + FreeBlockCountThatNeedCloseTarget + (WriteProtect_NUM << gubSelTotalGroupsLog); //7=VT(4)+VTchild+RUT+Parity
#endif
	if ((ulNativeUnit * 100) % 99) {
		ulNeededUnitNum++;
	}
	if (ulNeededUnitNum & gubSelTotalGroupsMask) {
		ulNeededUnitNum = ((ulNeededUnitNum >> gubSelTotalGroupsLog) + 1) << gubSelTotalGroupsLog;
	}


Mark_Build_SystemArea:
	//initial SystemArea相關變數
	gubSystemSetFreeCount = 0;
	gubSystemSetFreeQueueInPTR = 0;
	gubSystemSetFreeQueueOutPTR = 0;
	for (ubi = 0; ubi < SYSTEM_SETS; ubi++) {
		for (ubj = 0; ubj < MAX_CHANNEL; ubj++) {
			guwSystemBlockRemappingTable[ubi][ubj] = 0xFFFF;
		}
		gubSystemSetFreeTable[ubi] = 0xFF;
	}

	//確認DBT存在與否, 以決定下面要進那一條路
	if (RDT_FindDBTBlock_Task()) {
		gubRDTDone = DBT_Checked;	// not found
	}
	else {
		gubRDTDone = (DBT_Checked | DBT_Exist);	// DBT found
		if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {//先load第一段DBT
			ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				SetFailCEtoSystemInfo(ubCEIndex);
			}
			return 1;
		}
	}

	//在沒有保留RUT和SystemArea界線的情況下, MP可以由infoBlk指定SystemRule
	if (!(gubRUT_EC_set & BIT1)) {
		if (InfoArray->ubSystemAreaRule_ByAP != 0) { //MP由infoblk指定SystemRule
			if (InfoArray->ubSystemAreaRule_ByAP == 1) {
				gubSystemArea_RemappingRule = Rule_UnitPlaneA;
			}
			else if (InfoArray->ubSystemAreaRule_ByAP == 2) {
				gubSystemArea_RemappingRule = Rule_Set;
			}
			else {//參數錯誤
				ftlPreformatShowError(0, 0, 0x11);
				return 1;
			}
		}
		else {//Burner,FW自己判斷
		}
	}

	if (gubRDTDone & DBT_Exist) {		// DBT found
		ubCodeNum = 0;
		UBYTE ubSystemUnitLimit[MAX_CHANNEL];
		UBYTE ubDBTSetCheck = 0;
		UBYTE ubSystemSetCheck = 0;
		UWORD uwDBTNewBlock[MAX_CHANNEL];
		UBYTE ubDBTNewSet;

		if (HandleDBTSetLocation((U32)&ubDBTBuffer[0], Mode_ByDBTinFlash)) {
			ftlPreformatShowError(SystemArea_Error_DBTSet, gubDBTSet, 0x84);
			return 1;
		}

		for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
#if BURNER
			ulStartUnit[ubCEIndex] = 0;
#else
			if (ICE)
				ulStartUnit[ubCEIndex] = 10;
			else
				ulStartUnit[ubCEIndex] = 0;
#endif

			ubStartBurst[ubCEIndex] = 0;

			if (gubRUT_EC_set & BIT1)
				ubSystemUnitLimit[ubCEIndex] = ((guwSystemAreaNum[ubCEIndex] - 1) >> gubBurstsPerBankLog) + 1;
			else
				ubSystemUnitLimit[ubCEIndex] = SYSTEMAREA_DEFAULT_SCAN_RANGE;
		}


		if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {//先load第一段DBT
			ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				SetFailCEtoSystemInfo(ubCEIndex);
			}
			return 1;
		}

		while (1) {
			if (ENABLE_DEBUG_UART_PREFORMAT) {
				UartString("\n Set:");
				Uart_Tx_DataHex(ubSYSRemappingSetPtr);
			}
			if (gubSystemArea_RemappingRule == Rule_UnitPlaneA) { //依照舊rule只挑planeA的systemUnit
				ubBurst = 0;//只挑planeA,兩個channel的起點終點相同
				for (ulFreeSystemUnitPTR = ulStartUnit[0] ; ulFreeSystemUnitPTR < ubSystemUnitLimit[0]; ulFreeSystemUnitPTR++) {
					ubbadblock = 0;
					ulFreeblock = (M_Multiplier((ulFreeSystemUnitPTR << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + ubBurst;
					uli = ulFreeblock % ulEntryPerDBTPlane;//該PlaneBank放在這個DBT plane的第幾個Byte
					if (uli == 0) {
						if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
							ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
							for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
								SetFailCEtoSystemInfo(ubCEIndex);
							}
							return 1;
						}
					}

					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						if (ENABLE_PLANE_CH_CE) {
							if (ubDBTBuffer[uli + (ubCEIndex << gubBurstsPerBankLog)] & 0xF0) {
								ubbadblock |= 1;
							}
							if (gubEraseFailBitMap[(ulFreeblock + (ubCEIndex << gubBurstsPerBankLog)) >> 3] & (BIT0 << ((ulFreeblock + (ubCEIndex << gubBurstsPerBankLog)) & 7)))  { //Skip erase fail block in erase all
								ubbadblock |= 1;
							}
						}
						else {
							if (ubDBTBuffer[uli + ubCEIndex] & 0xF0) {
								ubbadblock |= 1;
							}
							if (gubEraseFailBitMap[(ulFreeblock + ubCEIndex) >> 3] & (BIT0 << ((ulFreeblock + ubCEIndex) & 7)))  { //Skip erase fail block in erase all
								ubbadblock |= 1;
							}
						}
					}

					if (ubbadblock == 0) {//挑到了兩個planeA同時為goodblk的Unit,放入remapping table
						if (ENABLE_DEBUG_UART_PREFORMAT) {
							UartString("\nBlock:");
							UartLongHex((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst);
						}

						for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
							guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][ubCEIndex] = (ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst;
						}

						for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
							if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) == ((U32)guwSysCodeBlock[ubCodeNum][ubCEIndex])) {
								ubCodeSetCheck |= (BIT0 << ubCEIndex);//兩個channel會同時找到
							}
						}
						if (ubCodeSetCheck != 0) {
							ulStartUnit[0] = ulFreeSystemUnitPTR + 1;
							break;
						}

						for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
							if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) != ((U32)guwDBTBlock[ubCEIndex])) {//不是DBT(system unit也會進這個case去erase掉)
								ftl_Erase_Single_Plane(ulFreeSystemUnitPTR, ubBurst, ubCEIndex, BIT_FJOBI_FASTPAGE);

								if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) == ((U32)guwSystemBlock[ubCEIndex])) {
									ubSystemSetCheck |= (BIT0 << ubCEIndex);
								}
							}
							else { //It is DBT, not erase it
								ubDBTSetCheck |= (BIT0 << ubCEIndex);
							}
						}
						//把目前掃到的進度保留下來
						ulStartUnit[0] = ulFreeSystemUnitPTR + 1;
						break;
					}
				}
				if (ulFreeSystemUnitPTR == ubSystemUnitLimit[0]) {
					ftlPreformatShowError(0, ulFreeSystemUnitPTR << gubBurstsPerBankLog, 0x80);
					//因為是Old PlaneA Rule 兩個CE都同時到達界線
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						SetFailCEtoSystemInfo(ubCEIndex);
					}
					return 1;
				}
			}
			else if (gubSystemArea_RemappingRule == Rule_Set) { //plane A,B,C,D都可以挑
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					ubGetFreeBlock = 0;
					for (ulFreeSystemUnitPTR = ulStartUnit[ubCEIndex] ; ulFreeSystemUnitPTR < ubSystemUnitLimit[ubCEIndex]; ulFreeSystemUnitPTR++) {
						for (ubBurst = ubStartBurst[ubCEIndex]; ubBurst < gubBurstsPerBank; ubBurst++) {
							if (ENABLE_DEBUG_UART_PREFORMAT) {
								UartString("\n CE:");
								Uart_Tx_DataHex(ubCEIndex);
								UartString(" Block:");
								UartLongHex((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst);
							}
							if (gubRUT_EC_set & BIT1) {//因為guwSystemAreaNum界線不一定切齊unit(不在planeA), 所以這邊要先用block number擋
								if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) == guwSystemAreaNum[ubCEIndex]) {
									ftlPreformatShowError(ubCEIndex, ((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst), 0x80);
									SetFailCEtoSystemInfo(ubCEIndex);
									return 1;
								}
							}
							ubbadblock = 0;
							ulFreeblock = (M_Multiplier((ulFreeSystemUnitPTR << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + ubBurst;
							uli = ulFreeblock % ulEntryPerDBTPlane;//該PlaneBank放在這個DBT plane的第幾個Byte
							if (uli == 0) {
								if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
									ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
									for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
										SetFailCEtoSystemInfo(ubCEIndex);
									}
									return 1;
								}
							}

							if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) == ((U32)guwSysCodeBlock[ubCodeNum][ubCEIndex])) {
								ubCodeSetCheck |= (BIT0 << ubCEIndex);

								guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][ubCEIndex] = (ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst;

								if (ubBurst == gubBurstsPerBank - 1) { //把這個CE目前掃到的進度保留下來
									ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR + 1;
									ubStartBurst[ubCEIndex] = 0;
								}
								else {
									ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR;
									ubStartBurst[ubCEIndex] = ubBurst + 1;
								}
								ubGetFreeBlock = 1;//換下一個CE挑
								break;
							}

							if (ENABLE_PLANE_CH_CE) {
								if (ubDBTBuffer[uli + (ubCEIndex << gubBurstsPerBankLog)] & 0xF0) {
									ubbadblock |= 1;
								}
								if (gubEraseFailBitMap[(ulFreeblock + (ubCEIndex << gubBurstsPerBankLog)) >> 3] & (BIT0 << ((ulFreeblock + (ubCEIndex << gubBurstsPerBankLog)) & 7)))  { //Skip erase fail block in erase all
									ubbadblock |= 1;
								}
							}
							else {
								if (ubDBTBuffer[uli + ubCEIndex] & 0xF0) {
									ubbadblock |= 1;
								}
								if (gubEraseFailBitMap[(ulFreeblock + ubCEIndex) >> 3] & (BIT0 << ((ulFreeblock + ubCEIndex) & 7)))  { //Skip erase fail block in erase all
									ubbadblock |= 1;
								}
							}

							if (ubbadblock == 0) {//挑到了,放入remapping table
								guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][ubCEIndex] = (ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst;

								if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) != ((U32)guwDBTBlock[ubCEIndex])) {//不是DBT(system unit也會進這個case去erase掉)
									ftl_Erase_Single_Plane(ulFreeSystemUnitPTR, ubBurst, ubCEIndex, BIT_FJOBI_FASTPAGE);

									if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) == ((U32)guwSystemBlock[ubCEIndex])) {
										ubSystemSetCheck |= (BIT0 << ubCEIndex);
									}
								}
								else { //It is DBT, not erase it
									ubDBTSetCheck |= (BIT0 << ubCEIndex);
								}

								//把這個CE目前掃到的進度保留下來
								if (ubBurst == gubBurstsPerBank - 1) {
									ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR + 1;
									ubStartBurst[ubCEIndex] = 0;
								}
								else {
									ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR;
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
					if (ulFreeSystemUnitPTR == ubSystemUnitLimit[ubCEIndex]) {
						ftlPreformatShowError(ubCEIndex, ulFreeSystemUnitPTR << gubBurstsPerBankLog, 0x80);
						SetFailCEtoSystemInfo(ubCEIndex);
						return 1;
					}
				}
			}
			//兩個CE都挑到了,湊成一組Set

			if (ubCodeSetCheck != 0) { //這組是CodeBlk
				//在同一codeset中兩個codeblk都存在的情況下,上面的迴圈應該要讓兩個CE同時挑到code block(沒辦法用set判斷)
				if ((gubPlanesPerBurst == MAX_CHANNEL) && (ubCodeSetCheck != (BIT0 | BIT1))) {// 2 channel才卡兩個CE同時挑到CodeBlock
					if (ubCodeSetCheck == BIT0) { //CE0存在CodeBlock, CE1不存在CodeBlock
						ubCEIndex = 1;
					}
					else if (ubCodeSetCheck == BIT1) { //CE1存在CodeBlock, CE0不存在CodeBlock
						ubCEIndex = 0;
					}
					SetFailCEtoSystemInfo(ubCEIndex);
					ftlPreformatShowError(SystemArea_Error_CodeSet, ubSYSRemappingSetPtr, 0x85);
					return 1;
				}
				else {
					ubCodeBlkNum++;
					if (ubCodeNum == 0) {
						ubCodeNum = 1;
					}
					if (ubSYSRemappingSetPtr == (SYSTEM_SETS - 1)) {
						break;//挑完了, 跳出while(1)
					}
					ubSYSRemappingSetPtr++;
					ubCodeSetCheck = 0;
					continue;
				}
			}

			if (gubRUT_EC_set & BIT1) {//erase 0x11
				if (ubSystemSetCheck != 0) { //如果是system block就留著吧
					if (ubSYSRemappingSetPtr != gubSystemSet) { //SystemSet編號不對(會進這個case多半是preformat bug)
						ftlPreformatShowError(SystemArea_Error_SystemSet, ubSYSRemappingSetPtr, 0x85);
						return 1;
					}
					//set符合,此時CE0是systemBlk or CE1是SystemBlk都算是SystemSet
					ubSystemSetCheck = 0;
				}
				else {//這組可能是DBTset
					if (ubDBTSetCheck != 0) {
						if (ubSYSRemappingSetPtr != gubDBTSet) { //DBTSet編號不對(會進這個case多半是preformat bug)
							ftlPreformatShowError(SystemArea_Error_DBTSet, ubSYSRemappingSetPtr, 0x85);
							return 1;
						}
						ubDBTSetCheck = 0;
					}
					else {//這組不是DBT
						ftlAddToSystemSetFreeTable(ubSYSRemappingSetPtr);
					}
				}

				if ((guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][0] == (guwSystemAreaNum[0] - 1)) ||
				        (guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][1] == (guwSystemAreaNum[1] - 1)) ||
				        (ubSYSRemappingSetPtr == (SYSTEM_SETS - 1))) {//systemSet有可能在set_7

					ubDBTNewSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_TAIL);
					uwDBTNewBlock[0] = guwSystemBlockRemappingTable[ubDBTNewSet][0];
					uwDBTNewBlock[1] = guwSystemBlockRemappingTable[ubDBTNewSet][1];
					if (ubDBTNewSet == gubDBTSet) {
						UBYTE ubNewSet;
						ubNewSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_TAIL);
						uwDBTNewBlock[0] = guwSystemBlockRemappingTable[ubNewSet][0];
						uwDBTNewBlock[1] = guwSystemBlockRemappingTable[ubNewSet][1];
						ubDBTNewSet = ubNewSet;
					}
					break;
				}
			}
			else {//erase 0x01
				//sys必寫在set_4, DBT預計寫在Set_7, 如果已經在set_7就重新從Queue拿, 如果DBT在set_4會先把它搬到set_7再寫sys到set_4
				if (ubSYSRemappingSetPtr == (SYSTEM_SETS - 1)) {
					uwDBTNewBlock[0] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][0];
					uwDBTNewBlock[1] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][1];
					ubDBTNewSet = ubSYSRemappingSetPtr;

					if (ubDBTNewSet == gubDBTSet) {
						UBYTE ubNewSet;
						ubNewSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_TAIL);
						uwDBTNewBlock[0] = guwSystemBlockRemappingTable[ubNewSet][0];
						uwDBTNewBlock[1] = guwSystemBlockRemappingTable[ubNewSet][1];
						ubDBTNewSet = ubNewSet;
					}
					break;//跳出while(1)
				}
				else {
					if ((ubSYSRemappingSetPtr == (SYSTEM_SETS - 4))) {//ubSYSRemappingSetPtr = 4固定放systemSet
						if (ubDBTSetCheck != 0) {//如果這個SET已經放DBT了,會先把DBT搬走,再寫入SystemSet,進這個if只是為了把ubDBTSetCheck清成0
							if (ubSYSRemappingSetPtr != gubDBTSet) { //DBTSet編號不對(會進這個case多半是preformat bug)
								ftlPreformatShowError(SystemArea_Error_DBTSet, ubSYSRemappingSetPtr, 0x85);
								return 1;
							}
							ubDBTSetCheck = 0;
						}
						guwSystemBlock[0] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][0];
						guwSystemBlock[1] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][1];
						gubSystemSet = ubSYSRemappingSetPtr;
					}
					else {
						if (ubDBTSetCheck != 0) {
							if (ubSYSRemappingSetPtr != gubDBTSet) { //DBTSet編號不對(會進這個case多半是preformat bug)
								ftlPreformatShowError(SystemArea_Error_DBTSet, ubSYSRemappingSetPtr, 0x85);
								return 1;
							}
							ubDBTSetCheck = 0;
						}
						else {
							ftlAddToSystemSetFreeTable(ubSYSRemappingSetPtr);
						}
					}
				}
			}
			ubSYSRemappingSetPtr++;

#if (MicronFlashOnly && (MICRON_NEW_BOOTCODE == 0))
#if (B0KB)   //這裡在卡code block的數量
			if (ubSYSRemappingSetPtr == 2) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if ((gubCENumber == 5) || (gubCENumber == 10) || (gubCENumber == 11)) {//B0KB Force to 2 plane
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (4 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
					else {
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (2 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
#elif (!B0KB)//此判斷給L06B //B16 define出來後需歸類跟B0KB一樣
			if (ubSYSRemappingSetPtr == 2) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (4 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return 1;
					}
				}
			}
#endif
#elif Hynix  //Hynix//如果有特殊case以後再額外寫
			if (guwPagesPerUnit == 128) {
				if (ubSYSRemappingSetPtr == 4) {
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (63 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
			else if (guwPagesPerUnit == 256) {
				if (ubSYSRemappingSetPtr == 4) {
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (32 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
#else  //for Toshiba
			if (gubBurstsPerBank == 2) {
				if (ubSYSRemappingSetPtr == 4) {
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
#if TSB_BICS4_SUPPORT
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= ((guoFlashUseType.B.btToshibaBiCs4 ? 16 : 32) << gubBurstsPerBankLog)) {
#else
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (32 << gubBurstsPerBankLog)) {
#endif
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
			else if (gubBurstsPerBank == 4) {
				if (ubSYSRemappingSetPtr == 4) {
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (16 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
#endif
		}

		ulplane = 0;
		ulFreeblock = 0;
		ubDBTMappingPlaneIndex = 0;
		ubDBTSizeInPlanes = 0;
		TotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;
		while (TotalBlockNum > ulFreeblock) {
			//loadDBTtoBuffer
			if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[ulFreeblock])) {
				ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				return 1;
			}
			if ((TotalBlockNum - ulFreeblock) > ulEntryPerDBTPlane) {
				ulj = ulEntryPerDBTPlane;
			}
			else {
				ulj = (TotalBlockNum - ulFreeblock);
			}
			for (uli = ulFreeblock; uli < (ulFreeblock + ulj); uli++) {	// change later bad mark to early bad mark
#if EnableProductHistory
				if (((gubPHmatch[0]&Bit1_FindByScan) == BIT1) || ((gubPHmatch[1]&Bit1_FindByScan) == BIT1)) {
					uwUnittemp = uli / (gubCENumber * gubBurstsPerBank * gubDieNumber);
					ubCEIndextemp = (uli / gubBurstsPerBank) % gubCENumber;
					ubPlaneBanktemp = uli % (gubCENumber * gubBurstsPerBank);
					ubDieIndextemp = (uli / (gubCENumber * gubBurstsPerBank)) % gubDieNumber;
					if ((uwUnittemp >= PHStartUnit) && (uwUnittemp < PHEndUnit) && (ubDieIndextemp == 0)) {
						//find by scan和 find by DBT只會相信一個
						//這邊有DBT,卻FindByScan,所以相信SCAN

						if (((gubPHmatch[0]&Bit1_FindByScan) == BIT1) && (guwPHUnit[0].uwUnit == uwUnittemp) && (guwPHUnit[0].ubPlanebank == ubPlaneBanktemp)) {
							ubDBTBuffer[uli] = PHMarkBad;
						}
						if (((gubPHmatch[1]&Bit1_FindByScan) == BIT1) && (guwPHUnit[1].uwUnit == uwUnittemp) && (guwPHUnit[1].ubPlanebank == ubPlaneBanktemp)) {
							ubDBTBuffer[uli] = PHMarkBad;
						}
					}
				}
#endif

				if (ubDBTBuffer[uli] & 0x30) {
					if (InfoArray->ubPreformatNoLaterBad == 1) {
						ubDBTBuffer[uli] = 0;
					}
					else {
						ubDBTBuffer[uli] = BIT7 | (ubDBTBuffer[uli] & 0x0F);
					}
				}
				else {	//Mark EraseFail accoding to gubEraseFailBitMap
					if (gubEraseFailBitMap[uli >> 3] & (BIT0 << (uli & 7))) {
						if (InfoArray->ubRecordEraseFail_EarlyOrLater == 1) {
							ubDBTBuffer[uli] = 0x84;
						}
						else {
							ubDBTBuffer[uli] = 0x34;
						}
					}
#if EnableProductHistory
					if (ubFind < PHUnitNum) {

						uwUnittemp = uli / (gubCENumber * gubBurstsPerBank * gubDieNumber);
						ubCEIndextemp = (uli / gubBurstsPerBank) % gubCENumber;
						ubPlaneBanktemp = uli % (gubCENumber * gubBurstsPerBank);
						ubDieIndextemp = (uli / (gubCENumber * gubBurstsPerBank)) % gubDieNumber;

						if ((uwUnittemp >= PHStartUnit) && (uwUnittemp < PHEndUnit) && (ubDieIndextemp == 0)) {
							//find by scan和 find by DBT只會做一個
							//這邊有DBT不用 Handle find by scan 沒mark DBT的狀況

							if (ubDBTBuffer[uli] == 0x00) {
								ubDBTBuffer[uli] = PHMarkBad;
								guwPHUnit[ubFind].uwUnit = uwUnittemp;
								guwPHUnit[ubFind].ubPlanebank = ubPlaneBanktemp;
								ubFind++;
							}
						}
					}
#endif
				}
			}
			//programDBTtoFLA
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				UWORD uwDBTUnit = uwDBTNewBlock[ubCEIndex] >> gubBurstsPerBankLog;
				UBYTE ubDBTBurst = (UBYTE)(uwDBTNewBlock[ubCEIndex] & gubBurstsPerBankMask);
				if (ftlWriteSinglePlane(uwDBTUnit, ulplane,  ubCEIndex, ((U32)&ubDBTBuffer[ulFreeblock] ), MARK_DBT, 0xFF012345, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubDBTBurst)) {
					ftlPreformatShowError(ubCEIndex, uwDBTNewBlock[ubCEIndex], 0x12);
					SetFailCEtoSystemInfo(ubCEIndex);
					return 1;
				}
			}
			ubDBTSizeInPlanes++;
			ubDBTMappingPlaneIndex++;
			ulplane++;//ulplane += gubPlanesPerPage;
			ulFreeblock += ulEntryPerDBTPlane;
		}

		while (ulplane < guwFastPagePagesPerUnit) {
			if (ubDBTMappingPlaneIndex == ubDBTSizeInPlanes) {	// 寫完一份DBT, 需先reset再繼續寫下一份DBT
				ubDBTMappingPlaneIndex = 0;
				ulFreeblock = 0;
			}
			//programDBTtoFLA
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				UWORD uwDBTUnit = uwDBTNewBlock[ubCEIndex] >> gubBurstsPerBankLog;
				UBYTE ubDBTBurst = (UBYTE)(uwDBTNewBlock[ubCEIndex] & gubBurstsPerBankMask);
				if (ftlWriteSinglePlane(uwDBTUnit, ulplane,  ubCEIndex, ((U32)&ubDBTBuffer[ulFreeblock] ), MARK_DBT, 0xFF012345, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubDBTBurst)) {
					ftlPreformatShowError(ubCEIndex, uwDBTNewBlock[ubCEIndex], 0x12);
					SetFailCEtoSystemInfo(ubCEIndex);
					return 1;
				}
			}
			ubDBTMappingPlaneIndex++;
			ulplane++;//ulplane += gubPlanesPerPage;
			ulFreeblock += ulEntryPerDBTPlane;
		}

		//check the new DBT
		UWORD uwDBTBlockTemp[2];
		UBYTE uwDBTSetTemp;
		uwDBTBlockTemp[0] = guwDBTBlock[0];
		uwDBTBlockTemp[1] = guwDBTBlock[1];
		uwDBTSetTemp = gubDBTSet;

		guwDBTBlock[0] = uwDBTNewBlock[0];
		guwDBTBlock[1] = uwDBTNewBlock[1];
		gubDBTSet = ubDBTNewSet;

		ulplane = 0;
		ulFreeblock = 0;
		while (TotalBlockNum > ulFreeblock) {
			if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
				ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				return 1;
			}
			ulFreeblock += ulEntryPerDBTPlane;
		}

		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			UWORD uwDBTUnit = uwDBTBlockTemp[ubCEIndex] >> gubBurstsPerBankLog;
			UBYTE ubDBTBurst = (UBYTE)(uwDBTBlockTemp[ubCEIndex] & gubBurstsPerBankMask);

			ftl_Erase_Single_Plane(uwDBTUnit, ubDBTBurst, ubCEIndex, 0);
			ftl_Erase_Single_Plane(uwDBTUnit, ubDBTBurst, ubCEIndex, BIT_FJOBI_FASTPAGE);
		}

		if (uwDBTSetTemp < SYSTEM_SETS) {
			ftlAddToSystemSetFreeTable(uwDBTSetTemp);//ADD old DBT unit in to free System unit
		}

		if (gubPlanesPerBurst == MAX_CHANNEL) {
			//check DBT and System 挑得是否成對
			UWORD uwOtherBlockInSet;
			uwOtherBlockInSet = ftlGetAnotherSystemBlockInSet(Mode_ByRemappingTable, 0, 0, guwDBTBlock[0]);
			if (guwDBTBlock[1] != uwOtherBlockInSet) {
				ftlPreformatShowError(SystemArea_Error_DBTSet, guwDBTBlock[1], 0x86);
				return 1;
			}
			uwOtherBlockInSet = ftlGetAnotherSystemBlockInSet(Mode_ByRemappingTable, 0, 0, guwSystemBlock[0]);
			if (guwSystemBlock[1] != uwOtherBlockInSet) {
				ftlPreformatShowError(SystemArea_Error_SystemSet, guwSystemBlock[1], 0x86);
				return 1;
			}
		}

		if ((gubRUT_EC_set & BIT1) == 0) {
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				guwSystemAreaNum[ubCEIndex] = guwSystemBlockRemappingTable[SYSTEM_SETS - 1][ubCEIndex] + 1;
			}
		}
		goto Mark_BuildRUTInRAM;
	}
	else {
		//Lin_RDT_Preformat1:
		ubCodeNum = 0;

		for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
#if BURNER
			ulStartUnit[ubCEIndex] = 0;
#else
			if (ICE)
				ulStartUnit[ubCEIndex] = 10;
			else
				ulStartUnit[ubCEIndex] = 0;
#endif
			ubStartBurst[ubCEIndex] = 0;
		}

		while (1) {
			if (ENABLE_DEBUG_UART_PREFORMAT) {
				UartString("\n Set:");
				Uart_Tx_DataHex(ubSYSRemappingSetPtr);
			}
			if (gubSystemArea_RemappingRule == Rule_UnitPlaneA) { //依照舊rule只挑planeA的systemUnit
				ubBurst = 0;
				for (ulFreeSystemUnitPTR = ulStartUnit[0]; ulFreeSystemUnitPTR < SYSTEMAREA_DEFAULT_SCAN_RANGE; ulFreeSystemUnitPTR++) {
					ubbadblock = 0;

					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						if (ENABLE_PLANE_CH_CE) {
							ubPlaneBank = (ubCEIndex << gubBurstsPerBankLog) + ubBurst;
						}
						else {
							ubPlaneBank = ubCEIndex ;
						}

						if (ENABLE_SANDISK_CHECKEARLYBADBYPROG) {
							ubbadblock |= ftlGoodBlock_SandiskCheckByProgFail(ulFreeSystemUnitPTR, ubPlaneBank);
						}
						else {
#if TLC
#if B0KB
							ubbadblock |= ftlGoodBlock(ulFreeSystemUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2, GOODBLK_MLC);
#else
							ubbadblock |= ftlGoodBlock(ulFreeSystemUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2);
#endif
#else
#if MicronFlashOnly
							ubbadblock |= ftlGoodBlock(ulFreeSystemUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1, GOODBLK_MLC);
#else
							ubbadblock |= ftlGoodBlock(ulFreeSystemUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1);
#endif
#endif
						}
						ulCheckEraseFailBlkIndex = (M_Multiplier((ulFreeSystemUnitPTR << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + ubPlaneBank;
						if (gubEraseFailBitMap[ulCheckEraseFailBlkIndex >> 3] & (BIT0 << (ulCheckEraseFailBlkIndex & 7))) { //Skip erase fail block in erase all
							ubbadblock |= 1;
						}
					}

					if (ubbadblock == 0) {
						if (ENABLE_DEBUG_UART_PREFORMAT) {
							UartString("\nBlock:");
							UartLongHex((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst);
						}

						for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
							guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][ubCEIndex] = (ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst;
						}

						for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
							if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) == ((U32)guwSysCodeBlock[ubCodeNum][ubCEIndex])) {
								ubCodeSetCheck |= (BIT0 << ubCEIndex);//兩個channel會同時找到
							}
						}
						if (ubCodeSetCheck != 0) {
							ulStartUnit[0] = ulFreeSystemUnitPTR + 1;
							break;
						}

						for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
							if (!gubEraseAllDone_Flag) { //以這個Flag避免Double Erase
								//ftl_Erase_Single_Plane(ulFreeSystemUnitPTR, ubBurst, ubCEIndex, BIT_FJOBI_FASTPAGE);
								ftl_Erase_Single_Plane(ulFreeSystemUnitPTR, ubBurst, ubCEIndex, 0);//以NonA2方式Erase，避免某些Flash在NonA2 Erase後，ftlGoodBlock會誤判成EarlyBad
							}
						}
						//把目前掃到的進度保留下來
						ulStartUnit[0] = ulFreeSystemUnitPTR + 1;
						break;
					}
				}
				if (ulFreeSystemUnitPTR == SYSTEMAREA_DEFAULT_SCAN_RANGE) {
					ftlPreformatShowError(0, ulFreeSystemUnitPTR << gubBurstsPerBankLog, 0x80);
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						SetFailCEtoSystemInfo(ubCEIndex);
					}
					return 1;
				}
			}
			else if ((gubSystemArea_RemappingRule == Rule_Set)) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					ubGetFreeBlock = 0;
					for (ulFreeSystemUnitPTR = ulStartUnit[ubCEIndex]; ulFreeSystemUnitPTR < SYSTEMAREA_DEFAULT_SCAN_RANGE; ulFreeSystemUnitPTR++) {
						for (ubBurst = ubStartBurst[ubCEIndex]; ubBurst < gubBurstsPerBank; ubBurst++) {
							if (ENABLE_DEBUG_UART_PREFORMAT) {
								UartString("\n CE:");
								Uart_Tx_DataHex(ubCEIndex);
								UartString(" Block:");
								UartLongHex((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst);
							}
							ubbadblock = 0;
							if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) == ((U32)guwSysCodeBlock[ubCodeNum][ubCEIndex])) {
								ubCodeSetCheck |= (BIT0 << ubCEIndex);

								guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][ubCEIndex] = (ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst;
								//把這個CE目前掃到的進度保留下來
								if (ubBurst == gubBurstsPerBank - 1) {
									ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR + 1;
									ubStartBurst[ubCEIndex] = 0;
								}
								else {
									ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR;
									ubStartBurst[ubCEIndex] = ubBurst + 1;
								}
								ubGetFreeBlock = 1;//換下一個CE挑
								break;
							}

							if (ENABLE_PLANE_CH_CE) {
								ubPlaneBank = (ubCEIndex << gubBurstsPerBankLog) + ubBurst;
							}
							else {
								ubPlaneBank = ubCEIndex ;
							}

							if (ENABLE_SANDISK_CHECKEARLYBADBYPROG) {
								ubbadblock |= ftlGoodBlock_SandiskCheckByProgFail(ulFreeSystemUnitPTR, ubPlaneBank);
							}
							else {
#if TLC
#if B0KB
								ubbadblock |= ftlGoodBlock(ulFreeSystemUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2, GOODBLK_MLC);
#else
								ubbadblock |= ftlGoodBlock(ulFreeSystemUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2);
#endif
#else
#if MicronFlashOnly
								ubbadblock |= ftlGoodBlock(ulFreeSystemUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1, GOODBLK_MLC);
#else
								ubbadblock |= ftlGoodBlock(ulFreeSystemUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1);
#endif
#endif
							}
							ulCheckEraseFailBlkIndex = (M_Multiplier((ulFreeSystemUnitPTR << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + ubPlaneBank;
							if (gubEraseFailBitMap[ulCheckEraseFailBlkIndex >> 3] & (BIT0 << (ulCheckEraseFailBlkIndex & 7))) { //Skip erase fail block in erase all
								ubbadblock |= 1;
							}


							if (ubbadblock == 0) {
								if (!gubEraseAllDone_Flag) { //以這個Flag避免Double Erase
									//ftl_Erase_Single_Plane(ulFreeSystemUnitPTR, ubBurst, ubCEIndex, BIT_FJOBI_FASTPAGE);
									ftl_Erase_Single_Plane(ulFreeSystemUnitPTR, ubBurst, ubCEIndex, 0);//以NonA2方式Erase，避免某些Flash在NonA2 Erase後，ftlGoodBlock會誤判成EarlyBad
								}
								guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][ubCEIndex] = (ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst;
								//把這個CE目前掃到的進度保留下來
								if (ubBurst == gubBurstsPerBank - 1) {
									ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR + 1;
									ubStartBurst[ubCEIndex] = 0;
								}
								else {
									ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR;
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
					if (ulFreeSystemUnitPTR == SYSTEMAREA_DEFAULT_SCAN_RANGE) {
						ftlPreformatShowError(ubCEIndex, ulFreeSystemUnitPTR, 0x80);
						SetFailCEtoSystemInfo(ubCEIndex);
						return 1;
					}
				}
			}
			//兩個CE都挑到了,湊成一組Set

			if (ubCodeSetCheck != 0) { //這組可能是CodeBlk
				//在同一codeset中兩個codeblk都存在的情況下,上面的迴圈應該要讓兩個CE同時挑到code block(沒辦法用set判斷)
				if ((gubPlanesPerBurst == MAX_CHANNEL) && (ubCodeSetCheck != (BIT0 | BIT1))) {// 2 channel才卡這個條件
					if (ubCodeSetCheck == BIT0) { //CE0存在CodeBlock, CE1不存在CodeBlock
						ubCEIndex = 1;
					}
					else if (ubCodeSetCheck == BIT1) { //CE1存在CodeBlock, CE0不存在CodeBlock
						ubCEIndex = 0;
					}
					SetFailCEtoSystemInfo(ubCEIndex);
					ftlPreformatShowError(SystemArea_Error_CodeSet, ubSYSRemappingSetPtr, 0x85);
					return 1;
				}
				else {
					ubCodeBlkNum++;
					if (ubCodeNum == 0) {
						ubCodeNum = 1;
					}
					if (ubSYSRemappingSetPtr == (SYSTEM_SETS - 1)) {
						break;//跳出while(1)
					}
					ubSYSRemappingSetPtr++;
					ubCodeSetCheck = 0;
					continue;
				}
			}

			if (ubSYSRemappingSetPtr == (SYSTEM_SETS - 1)) {
				guwDBTBlock[0] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][0];
				guwDBTBlock[1] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][1];
				gubDBTSet = ubSYSRemappingSetPtr;
				break;
			}
			if (ubSYSRemappingSetPtr == (SYSTEM_SETS - 4)) {
				guwSystemBlock[0] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][0];
				guwSystemBlock[1] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][1];
				gubSystemSet = ubSYSRemappingSetPtr;
			}
			else {
				ftlAddToSystemSetFreeTable(ubSYSRemappingSetPtr);
			}

			ubSYSRemappingSetPtr++;

			//各家flash放code block 位置的限制
#if (MicronFlashOnly && (MICRON_NEW_BOOTCODE == 0))
#if (B0KB)   //這裡在卡code block的數量
			if (ubSYSRemappingSetPtr == 2) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if ((gubCENumber == 5) || (gubCENumber == 10) || (gubCENumber == 11)) {//B0KB Force to 2 plane
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (4 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
					else {
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (2 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
#elif (!B0KB)//此判斷給L06B //B16 define出來後需歸類跟B0KB一樣
			if (ubSYSRemappingSetPtr == 2) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (4 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return 1;
					}
				}
			}
#endif
#elif Hynix  //Hynix//如果有特殊case以後再額外寫
			if (guwPagesPerUnit == 128) {
				if (ubSYSRemappingSetPtr == 4) {
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (63 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
			else if (guwPagesPerUnit == 256) {
				if (ubSYSRemappingSetPtr == 4) {
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (32 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
#else  //for Toshiba
			if (gubBurstsPerBank == 2) {
				if (ubSYSRemappingSetPtr == 4) {
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
#if TSB_BICS4_SUPPORT
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= ((guoFlashUseType.B.btToshibaBiCs4 ? 16 : 32) << gubBurstsPerBankLog)) {
#else
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (32 << gubBurstsPerBankLog)) {
#endif
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
			else if (gubBurstsPerBank == 4) {
				if (ubSYSRemappingSetPtr == 4) {
					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (16 << gubBurstsPerBankLog)) {
							ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
							SetFailCEtoSystemInfo(ubCEIndex);
							return 1;
						}
					}
				}
			}
#endif
		}

		if (gubPlanesPerBurst > 1) {
			//check DBT and System 挑得是否成對
			UWORD uwOtherBlockInSet;
			uwOtherBlockInSet = ftlGetAnotherSystemBlockInSet(Mode_ByRemappingTable, 0, 0, guwDBTBlock[0]);
			if (guwDBTBlock[1] != uwOtherBlockInSet) {
				ftlPreformatShowError(SystemArea_Error_DBTSet, guwDBTBlock[1], 0x86);
				return 1;
			}
			uwOtherBlockInSet = ftlGetAnotherSystemBlockInSet(Mode_ByRemappingTable, 0, 0, guwSystemBlock[0]);
			if (guwSystemBlock[1] != uwOtherBlockInSet) {
				ftlPreformatShowError(SystemArea_Error_SystemSet, guwSystemBlock[1], 0x86);
				return 1;
			}
		}

		for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
			guwSystemAreaNum[ubCEIndex] = guwSystemBlockRemappingTable[SYSTEM_SETS - 1][ubCEIndex] + 1;
		}
	}

Mark_BuildDBT:
	ulj = 0;
	ulplane = 0;
	ulFreeblock = 0;
	ubDBTMappingPlaneIndex = 0;
	ubDBTSizeInPlanes = 0;
	// build DBT by copybuffer
	mDMAC_SV(1, (U32)ubDBTBuffer, ulEntryPerDBTPlane * 32, 0, 0xFFFFFFFF, 0, DMA_EN_SETCQ, 1);		// 假設最大16CE, 4 plane, 8192 unit, 16*4*8192/16K = 32, 留ulEntryPerDBTPlane*32 = 512K 一定夠放DBT
	gulTemp = 0;
	for (uli = 0; uli < ulUnitsPerGroup; uli++) {
		for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
			for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
				ulUnitPTR = (ubGroup * gubDieNumber + ubDie) * ulUnitsPerGroup + uli;
				for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
					if (ENABLE_SANDISK_CHECKEARLYBADBYPROG) {
						ubDBTBuffer[ulj] = ftlGoodBlock_SandiskCheckByProgFail(ulUnitPTR, ubPlaneBank);
					}
					else {
#if TLC
#if B0KB
						ubDBTBuffer[ulj] = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2, GOODBLK_MLC);
#else
						ubDBTBuffer[ulj] = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2);
#endif
#else
#if MicronFlashOnly
						ubDBTBuffer[ulj] = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1, GOODBLK_MLC);
#else
						ubDBTBuffer[ulj] = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1);
#endif
#endif
					}
#if EnableProductHistory
					if (((gubPHmatch[0]&Bit1_FindByScan) == BIT1) || ((gubPHmatch[1]&Bit1_FindByScan) == BIT1)) {
						uwUnittemp = ulj / (gubCENumber * gubBurstsPerBank * gubDieNumber);
						ubCEIndextemp = (ulj / gubBurstsPerBank) % gubCENumber;
						ubPlaneBanktemp = ulj % (gubCENumber * gubBurstsPerBank);
						ubDieIndextemp = (ulj / (gubCENumber * gubBurstsPerBank)) % gubDieNumber;
						if ((uwUnittemp >= PHStartUnit) && (uwUnittemp < PHEndUnit) && (ubDieIndextemp == 0)) {
							//find by scan和 find by DBT只會相信一個
							//這邊沒有DBT要 Handle find by scan 沒mark DBT的狀況
							//若存在之前的PH unit,會被ftlGoodBlock判成0x80 在這裡改回0x41
							if (((gubPHmatch[0]&Bit1_FindByScan) == BIT1) && (guwPHUnit[0].uwUnit == uwUnittemp) && (guwPHUnit[0].ubPlanebank == ubPlaneBanktemp)) {
								ubDBTBuffer[ulj] = PHMarkBad;
							}
							if (((gubPHmatch[1]&Bit1_FindByScan) == BIT1) && (guwPHUnit[1].uwUnit == uwUnittemp) && (guwPHUnit[1].ubPlanebank == ubPlaneBanktemp)) {
								ubDBTBuffer[ulj] = PHMarkBad;
							}
						}
					}
#endif
					if (ubDBTBuffer[ulj] & 0xF0) {
						gulTemp++;
					}
					else {//Mark EraseFail accoding to gubEraseFailBitMap
						if (gubEraseFailBitMap[ulj >> 3] & (BIT0 << (ulj & 7))) {
							//UartString("\nMark Erase Fail in Built DBT");
							if (InfoArray->ubRecordEraseFail_EarlyOrLater == 1) {
								ubDBTBuffer[ulj] = 0x84;
							}
							else {
								ubDBTBuffer[ulj] = 0x34;
							}
						}

#if EnableProductHistory
						if (ubFind < PHUnitNum) {
							uwUnittemp = ulj / (gubCENumber * gubBurstsPerBank * gubDieNumber);
							ubCEIndextemp = (ulj / gubBurstsPerBank) % gubCENumber;
							ubPlaneBanktemp = ulj % (gubCENumber * gubBurstsPerBank);
							ubDieIndextemp = (ulj / (gubCENumber * gubBurstsPerBank)) % gubDieNumber;
							if ((uwUnittemp >= PHStartUnit) && (uwUnittemp < PHEndUnit) && (ubDieIndextemp == 0)) {
								if (ubDBTBuffer[ulj] == 0x00) {
									ubDBTBuffer[ulj] = PHMarkBad;
									guwPHUnit[ubFind].uwUnit = uwUnittemp;
									guwPHUnit[ubFind].ubPlanebank = ubPlaneBanktemp;
									ubFind++;
								}
							}
						}
#endif

					}
					ulj++;
				};
				//save last DBT
			}
		}
#if ENABLE_ODD_CE
		if ((ulj >= (ulEntryPerDBTPlane * (ubDBTMappingPlaneIndex + 1))) || (uli == (ulUnitsPerGroup - 1))) 	// Odd CE
#else
		if ((ulj == (ulEntryPerDBTPlane * (ubDBTMappingPlaneIndex + 1))) || (uli == (ulUnitsPerGroup - 1)))
#endif
		{
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
				UBYTE ubDBTUnit = (UBYTE)(guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog);
				UBYTE ubDBTBurst = (UBYTE)(guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask);

				//Sandisk Check Early Bad By Prog Case need to erase before program DBT plane 0
				if ((BURNER) && guoFlashUseType.B.btSandisk  && (ulplane == 0)) {
					ftl_Erase_Single_Plane(ubDBTUnit, ubDBTBurst, ubCEIndex, BIT_FJOBI_FASTPAGE);
				}

				if (ftlWriteSinglePlane(ubDBTUnit, ulplane,  ubCEIndex, ((U32)&ubDBTBuffer[ulFreeblock]), MARK_DBT, 0xFF012345, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubDBTBurst)) {
					ftlPreformatShowError(ubCEIndex, guwDBTBlock[ubCEIndex], 0x12);
					SetFailCEtoSystemInfo(ubCEIndex);
					return 1;
				}
			}

			if (ftlLoadDBT(ulFreeblock, (U32)ubTempBuffer)) {
				ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				return 1;
			}
			ubDBTMappingPlaneIndex++;
			ubDBTSizeInPlanes++;
			//mDMAC_SV(1, (U32)ubDBTBuffer, (U32)gubSectorsPerPlane * 512, 0, 0xFF, 0, DMA_EN_SETCQ, 1);
			ulplane++;
			ulFreeblock += ulEntryPerDBTPlane;
		}
	}

	while (ulplane < guwFastPagePagesPerUnit) {
		if (ubDBTMappingPlaneIndex == ubDBTSizeInPlanes) {	// 寫完一份DBT, 需先reset再繼續寫下一份DBT
			ubDBTMappingPlaneIndex = 0;
			ulFreeblock = 0;
		}
		//programDBTtoFLA
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			UBYTE ubDBTUnit = guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog;
			UBYTE ubDBTBurst = guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask;
			if (ftlWriteSinglePlane(ubDBTUnit, ulplane,  ubCEIndex, ((U32)&ubDBTBuffer[ulFreeblock] ), MARK_DBT, 0xFF012345, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubDBTBurst)) {
				ftlPreformatShowError(ubCEIndex, guwDBTBlock[ubCEIndex], 0x12);
				SetFailCEtoSystemInfo(ubCEIndex);
				return 1;
			}
		}
		ubDBTMappingPlaneIndex++;
		ulplane++;//ulplane += gubPlanesPerPage;
		ulFreeblock += ulEntryPerDBTPlane;

	}

	//Sandisk Check Early Bad By Prog Case need to erase System Area
	if ((BURNER) && guoFlashUseType.B.btSandisk ) {
		UBYTE ubSetPtr = 0;
		for (; ubSetPtr < SYSTEM_SETS; ubSetPtr++) {
			if (ubSetPtr == gubDBTSet) {		//Do not erase DBT
				continue;
			}
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				ftl_Erase_Single_Plane((guwSystemBlockRemappingTable[ubSetPtr][ubCEIndex] >> gubBurstsPerBankLog), (guwSystemBlockRemappingTable[ubSetPtr][ubCEIndex] & gubBurstsPerBankMask), ubCEIndex, BIT_FJOBI_FASTPAGE);
			}
		}
	}


	//	for (uli = ulplane; uli <  guwFastPagePagesPerUnit; uli ++) {
	//		ftlWriteSinglePlane(gubDBTUnit, uli,  0, ((U32)&ubDBTBuffer[0] ), MARK_DBT, 0xFF012345, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4);
	//	}

Mark_BuildRUTInRAM:

	mDMAC_SV(1, (U32)VT_Preformat_Temp, 16 * 1024, 0, 0, 0, DMA_EN_SETCQ, 1);
	uwTotalReduceUnit = 0;

	if (gubRUT_EC_set & BIT1) {

		for (ubGroup = 0; ubGroup < 16; ubGroup++) {
			gubReserveUnitsPerGroup[ubGroup] = VT->gubReserveUnitsPerGroup[ubGroup];
			guwUnitsPerGroups[ubGroup] = VT->guwUnitsPerGroups[ubGroup];
#if TLC
			VT_Preformat_Temp->guwD1UnitsPerGroups[ubGroup] = VT->guwD1UnitsPerGroups[ubGroup];
#endif
			guwExtendUnitsIndex[ubGroup] = VT->guwExtendUnitsIndex[ubGroup];
			uwTotalReduceUnit += VT->guwPutUserFreeToFailHandleAreaCnt[ubGroup];
			VT_Preformat_Temp->guwPutUserFreeToFailHandleAreaCnt[ubGroup] = VT->guwPutUserFreeToFailHandleAreaCnt[ubGroup];
		}
		for (uwPlaneBankIndex = 0; uwPlaneBankIndex < 256; uwPlaneBankIndex++) {
			guwEarlyBadCntPerPlane[uwPlaneBankIndex] = VT->guwEarlyBadCntPerPlane[uwPlaneBankIndex];
			guwRUTUnitIndexOfLastBlock[uwPlaneBankIndex] = VT->guwRUTUnitIndexOfLastBlock[uwPlaneBankIndex];
		}
		for (ubGroup = 0; ubGroup < 8; ubGroup++) {

			VT_Preformat_Temp->guwDieStartSFUnitIndex[ubGroup] = VT->guwDieStartSFUnitIndex[ubGroup];
		}

		VT_Preformat_Temp->gulPreviousMaxEraseCnt_D3 = VT->gulPreviousMaxEraseCnt_D3;
		VT_Preformat_Temp->gulTotalEraseCount = VT->gulTotalEraseCount;
		VT_Preformat_Temp->gulTotal_Erase_Count_D1 = VT->gulTotal_Erase_Count_D1;
#if TLC
		VT_Preformat_Temp->gubD1UnitNum = VT->gubD1UnitNum;
#endif
		if (ubKeepSMARTwhenKeepEC == 1) {
			VT_Preformat_Temp->guwFWUpdateCount = VT->guwFWUpdateCount;
			VT_Preformat_Temp->guwSystemUpdateCount = VT->guwSystemUpdateCount;
#if TLC
			VT_Preformat_Temp->guoTotalNandWritePlaneCnt_D1Data = VT->guoTotalNandWritePlaneCnt_D1Data;
			VT_Preformat_Temp->guoTotalNandWritePlaneCnt_D3TLCData = VT->guoTotalNandWritePlaneCnt_D3TLCData;
			VT_Preformat_Temp->guoTotalNandWritePlaneCnt_D3SLCData = VT->guoTotalNandWritePlaneCnt_D3SLCData;
			VT_Preformat_Temp->guoTotalNandWritePlaneCnt_D3SLCTable = VT->guoTotalNandWritePlaneCnt_D3SLCTable;
#else
			VT_Preformat_Temp->guoTotalNandWriteSectorCnt = VT->guoTotalNandWriteSectorCnt;
			VT_Preformat_Temp->guoTotalNandWriteData = VT->guoTotalNandWriteData;
			VT_Preformat_Temp->guoTotalNandWriteTable = VT->guoTotalNandWriteTable;
#endif
			VT_Preformat_Temp->guoTotalHostReadSectorCnt = VT->guoTotalHostReadSectorCnt;
			VT_Preformat_Temp->guoTotalHostWriteSectorCnt = VT->guoTotalHostWriteSectorCnt;
			VT_Preformat_Temp->guoTotalNandReadSectorCnt = VT->guoTotalNandReadSectorCnt;

			VT_Preformat_Temp->gulPowerOnCnt = VT->gulPowerOnCnt;
			VT_Preformat_Temp->gulPowerCyclingCnt = VT->gulPowerCyclingCnt;
			VT_Preformat_Temp->gulNonStandByPowerDownCnt = VT->gulNonStandByPowerDownCnt;
			VT_Preformat_Temp->gulPowerOnTimeInMinutes = VT->gulPowerOnTimeInMinutes;

			VT_Preformat_Temp->gulReadCRCCnt = VT->gulReadCRCCnt;
			VT_Preformat_Temp->gulWriteCRCCnt = VT->gulWriteCRCCnt;
			VT_Preformat_Temp->gulHostReadECCCnt = VT->gulHostReadECCCnt;
		}
	}

	mDMAC_SV(1, (U32)VT, 16 * 1024, 0, 0, 0, DMA_EN_SETCQ, 1);
	if (gubRUT_EC_set & BIT1) {
#if TLC
		gulMaxUserUnitNum = VT_Preformat_Temp->guwD1UnitsPerGroups[gubTotalGroups - 1];
#else
		gulMaxUserUnitNum = guwUnitsPerGroups[gubTotalGroups - 1];
#endif
		for (ubGroup = 0; ubGroup < 16; ubGroup++) {
			VT->gubReserveUnitsPerGroup[ubGroup] = gubReserveUnitsPerGroup[ubGroup];
			VT->guwUnitsPerGroups[ubGroup] = guwUnitsPerGroups[ubGroup];
#if TLC
			VT->guwD1UnitsPerGroups[ubGroup] = VT_Preformat_Temp->guwD1UnitsPerGroups[ubGroup];
#endif
			VT->guwExtendUnitsIndex[ubGroup] = guwExtendUnitsIndex[ubGroup];
			VT->guwPutUserFreeToFailHandleAreaCnt[ubGroup] = VT_Preformat_Temp->guwPutUserFreeToFailHandleAreaCnt[ubGroup];
		}
		for (uwPlaneBankIndex = 0; uwPlaneBankIndex < 256; uwPlaneBankIndex++) {
			VT->guwEarlyBadCntPerPlane[uwPlaneBankIndex] = guwEarlyBadCntPerPlane[uwPlaneBankIndex];
			VT->guwRUTUnitIndexOfLastBlock[uwPlaneBankIndex] = guwRUTUnitIndexOfLastBlock[uwPlaneBankIndex];
		}
		for (ubGroup = 0; ubGroup < 8; ubGroup++) {
			VT->guwDieStartSFUnitIndex[ubGroup] = VT_Preformat_Temp->guwDieStartSFUnitIndex[ubGroup];
		}

		VT->gulPreviousMaxEraseCnt_D3 = VT_Preformat_Temp->gulPreviousMaxEraseCnt_D3;
		VT->gulTotalEraseCount = VT_Preformat_Temp->gulTotalEraseCount;
		VT->gulTotal_Erase_Count_D1 = VT_Preformat_Temp->gulTotal_Erase_Count_D1;
#if TLC
		VT->gubD1UnitNum = VT_Preformat_Temp->gubD1UnitNum;
#endif
		if (ubKeepSMARTwhenKeepEC == 1) {
#if TLC
			VT->guoTotalNandWritePlaneCnt_D1Data = VT_Preformat_Temp->guoTotalNandWritePlaneCnt_D1Data;
			VT->guoTotalNandWritePlaneCnt_D3TLCData = VT_Preformat_Temp->guoTotalNandWritePlaneCnt_D3TLCData;
			VT->guoTotalNandWritePlaneCnt_D3SLCData = VT_Preformat_Temp->guoTotalNandWritePlaneCnt_D3SLCData;
			VT->guoTotalNandWritePlaneCnt_D3SLCTable = VT_Preformat_Temp->guoTotalNandWritePlaneCnt_D3SLCTable;
#else
			VT->guoTotalNandWriteSectorCnt = VT_Preformat_Temp->guoTotalNandWriteSectorCnt;
			VT->guoTotalNandWriteData = VT_Preformat_Temp->guoTotalNandWriteData;
			VT->guoTotalNandWriteTable = VT_Preformat_Temp->guoTotalNandWriteTable;
#endif
			VT->guwFWUpdateCount = VT_Preformat_Temp->guwFWUpdateCount;
			VT->guwSystemUpdateCount = VT_Preformat_Temp->guwSystemUpdateCount;

			VT->guoTotalHostReadSectorCnt = VT_Preformat_Temp->guoTotalHostReadSectorCnt;
			VT->guoTotalHostWriteSectorCnt = VT_Preformat_Temp->guoTotalHostWriteSectorCnt;
			VT->guoTotalNandReadSectorCnt = VT_Preformat_Temp->guoTotalNandReadSectorCnt;

			VT->gulPowerOnCnt = VT_Preformat_Temp->gulPowerOnCnt;
			VT->gulPowerCyclingCnt = VT_Preformat_Temp->gulPowerCyclingCnt;
			VT->gulNonStandByPowerDownCnt = VT_Preformat_Temp->gulNonStandByPowerDownCnt;
			VT->gulPowerOnTimeInMinutes = VT_Preformat_Temp->gulPowerOnTimeInMinutes;

			VT->gulReadCRCCnt = VT_Preformat_Temp->gulReadCRCCnt;
			VT->gulWriteCRCCnt = VT_Preformat_Temp->gulWriteCRCCnt;
			VT->gulHostReadECCCnt = VT_Preformat_Temp->gulHostReadECCCnt;
		}
		goto Mark_SetUnitToZero;
	}

#if TLC
	U32 ulD1UnitNum = 0U;
	U32 ulD3UnitNum;
	UWORD uwRemainder;
	U32 ulOPUnitNum;
	UWORD uwMaxUnit;
	U32 ulD1UnitNumPerDie;
	if (gubRMApreformat) {
		ulOPUnitNum = ((ulNativeUnit * 100) / 99);
		if ((ulNativeUnit * 100) % 99) {
			ulOPUnitNum++;
		}
		ulOPUnitNum = ulOPUnitNum - ulNativeUnit;

		ulD3UnitNum = ulNeededUnitNum;

#if Hynix3DV6
		if (guoFlashUseType.B.btHynix3DV6) {

			ulD1UnitNum = ((U32)MIN_4P_D1_UNIT_NUM_V6 << gubSelTotalGroupsLog);
		}
		else if (guoFlashUseType.B.btHynix3DV5) {

			if (guoFlashUseType.B.btFourPlane) {
				ulD1UnitNum = ((U32)MIN_4P_D1_UNIT_NUM_V5 << gubSelTotalGroupsLog);
			}
			else {
				ulD1UnitNum = ((U32)MIN_2P_D1_UNIT_NUM_V5 << gubSelTotalGroupsLog);
			}
		}

#elif B0KB //Debug
		if ((gubCENumber == 5) || (gubCENumber == 10) || (gubCENumber == 11)) {
			ulD1UnitNum = ((U32)MIN_2P_D1_UNIT_NUM << gubSelTotalGroupsLog);
		}
		else {
			ulD1UnitNum = ((U32)MIN_D1_UNIT_NUM << gubSelTotalGroupsLog);
		}
#else
#if TSB_BICS4_SUPPORT
		if (TLC_BICS2 && (guoFlashUseType.B.btToshibaBiCs3 || guoFlashUseType.B.btToshibaBiCs4)) {
#else
		if (TLC_BICS2 && guoFlashUseType.B.btToshibaBiCs3) {
#endif
			ulD1UnitNum = ((U32)gubMIN_D1_UNIT_NUM_BiCS3 << gubSelTotalGroupsLog);
		}
		else {
			ulD1UnitNum = ((U32)MIN_D1_UNIT_NUM << gubSelTotalGroupsLog);
		}
#endif

		uwRemainder = gulTotalPhyUnitNum - SYSTEM_SETS - ReserveUnit - ulD3UnitNum - (ulD1UnitNum * gubNum_D1_Part);

		//uwRemainder = gulMaxUserUnitNum - ulD3UnitNum - (ulD1UnitNum * gubNum_D1_Part);
		while (uwRemainder) {
			if (uwRemainder >= ((ulOPUnitNum * 2) + (((U32)gubNum_D1_Part * 1) << gubSelTotalGroupsLog))) { // 2: 2% OP, 1: 1 D1 Unit
				//ulD3UnitNum += (ulOPUnitNum * 2);
				ulD1UnitNum += ((U32)1 << gubSelTotalGroupsLog);
				uwRemainder -= ((ulOPUnitNum * 2) + (((U32)gubNum_D1_Part * 1) << gubSelTotalGroupsLog));
			}
			else {
				//ulD3UnitNum += uwRemainder;
				uwRemainder = 0;
			}

			if (ulD1UnitNum > MAX_D1_UNIT_NUM) {
				ulD1UnitNum = MAX_D1_UNIT_NUM;
				//ulD3UnitNum = gulMaxUserUnitNum - (ulD1UnitNum * gubNum_D1_Part);
				break;
			}
		}
		ulD1UnitNumPerDie = ulD1UnitNum / gubDieNumber;
		if (ulD1UnitNum % gubDieNumber) {
			ulD1UnitNumPerDie ++;
		}
	}
#endif

	for (uwPlaneBankIndex = 0; uwPlaneBankIndex < (gubTotalGroups * gubPlanesPerPage ); uwPlaneBankIndex++) {
		ulPlaneBank_Counter[uwPlaneBankIndex] = 0;
#if TLC
		if (gubRMApreformat) {
			ulPlaneBank_Counter_D1[uwPlaneBankIndex] = 0;
		}
#endif
	}
	if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {
		ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
		for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
			SetFailCEtoSystemInfo(ubCEIndex);
		}
		return 1;
	}
	for (uli = 0; uli < ulUnitsPerGroup; uli++) {
		for (ubGroup = 0; ubGroup < gubTotalGroups; ubGroup++) {
			for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
				//if ((ubGroup == 0) && (uli < ulFreeUnitPTR) && (ubPlaneBank < (gubPlanesPerBurst << gubBurstsPerBankLog))) {
				if ((ubGroup == 0) && (ubPlaneBank < (gubPlanesPerBurst << gubBurstsPerBankLog))) { //CE0, CE1
					ubCEIndex = ubPlaneBank >> gubBurstsPerBankLog;
					if (((uli << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask)) < guwSystemAreaNum[ubCEIndex]) {
						continue;
					}
				}
				ulFreeblock = (M_Multiplier((uli << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + (M_Multiplier(ubGroup, DEF_PlanesPerPage_Mcl_Div)) + ubPlaneBank;
				ulj = ulFreeblock % ulEntryPerDBTPlane;                                    //該PlaneBank放在這個DBT plane的第幾個Byte
				if (ulj == 0) {
					if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
						ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
						for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
							SetFailCEtoSystemInfo(ubCEIndex);
						}
						return 1;
					}
				}
				if ((ubDBTBuffer[ulj] & 0xF0) || (((ubGroup / ubCEGroup) % gubDieNumber) >= (gubSelDieNumber))) {
					//			VT->guwEarlyBadperPlane[ubPlaneBank + ubGroup * gubPlanesPerPage]++;
					// bad block
					continue;
				}
				else {
#if TLC
					if (gubRMApreformat) {
						if (ubDBTBuffer[ulj] == 0x08) {
							ulPlaneBank_Counter_D1[ubPlaneBank + (M_Multiplier(ubGroup, DEF_PlanesPerPage_Mcl_Div))] ++;
						}
						else {
							ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier(ubGroup, DEF_PlanesPerPage_Mcl_Div))] ++;
						}

					}
					else
#endif
					{
						ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier(ubGroup, DEF_PlanesPerPage_Mcl_Div))] ++;
					}
				}
			}
		}
	}
	UWORD uwUnitsPerGroups[16];
	UWORD uwCEBlockCounter = 0;
	gulMaxUserUnitNum = 0;
	for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
		for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
			for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
				VT->guwEarlyBadCntPerPlane[ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie), DEF_PlanesPerPage_Mcl_Div))]
				    = ulUnitsPerGroup - ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
#if TLC
				if (gubRMApreformat) {
					if (ulPlaneBank_Counter_D1[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))] < (ulD1UnitNumPerDie * gubNum_D1_Part)) {
						ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))] -=
						    ulD1UnitNumPerDie * gubNum_D1_Part - ulPlaneBank_Counter_D1[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
					}
				}
#endif
				if (gubRUTMixPlaneEnable == 0) {
					if (ubPlaneBank == 0) {
						VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] = ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
						VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] = ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
					}
					else {
						if (VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] > ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))]) {
							VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] = ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
						}
						if (VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] < ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))]) {
							VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] = ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
						}
					}
				}
				else {
					if ((ubPlaneBank & gubBurstsPerBankMask) == 0) {
						uwCEBlockCounter = 0;
					}
					uwCEBlockCounter += ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
					if ((ubPlaneBank & gubBurstsPerBankMask) == gubBurstsPerBankMask) {
						guwDieBlockAvg[(ubGroup * gubSelDieNumber + ubDie) * (gubPlanesPerPage >> gubBurstsPerBankLog) + (ubPlaneBank >> gubBurstsPerBankLog)] = (uwCEBlockCounter >> gubBurstsPerBankLog);
						if ((ubPlaneBank >> gubBurstsPerBankLog) == 0) {
							VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] = uwCEBlockCounter;
							VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] = uwCEBlockCounter;
						}
						else {
							if (VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] > uwCEBlockCounter) {
								VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] = uwCEBlockCounter;
							}
							if (VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] < uwCEBlockCounter) {
								VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] = uwCEBlockCounter;
							}
						}
					}
				}
			}
			if (gubRUTMixPlaneEnable) {
				VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] = VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] >> gubBurstsPerBankLog;
			}
			if (VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] > ReserveUnit) {
				VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] -= ReserveUnit;
				VT->gubReserveUnitsPerGroup[ubGroup * gubSelDieNumber + ubDie] = ReserveUnit;
			}
			uwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] = VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie];
			gulMaxUserUnitNum += VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie];  //reserve define units to Extend unit for  fail handling

			if (gubRUTMixPlaneEnable == 0) {
				VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] -= VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie];
			}
			else {
				VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] -= (VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] << gubBurstsPerBankLog);
				UWORD uwExtendTemp = (VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] >> gubBurstsPerBankLog);
				if (VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] & gubBurstsPerBankMask) {
					VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] = uwExtendTemp + gubBurstsPerBankMask;
				}
				else {
					VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] = uwExtendTemp + gubBurstsPerBankMask - 1;
				}
			}

			if ((ubGroup * gubSelDieNumber + ubDie)) {
				VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie] += VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie - 1];
				VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] += VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie - 1];
			}
		}
	}
#if (TLC)
	if (gubRMApreformat) {
		if (ulNeededUnitNum > gulMaxUserUnitNum) {
			U32 ulMaxUnitNumPerPlaneBank;
			for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
				for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
					for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
						if (ubDie == 0) {
							ulMaxUnitNumPerPlaneBank = ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
						}
						else {
							ulMaxUnitNumPerPlaneBank += ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
						}
					}

					if (ulNeededUnitNum > (ulMaxUnitNumPerPlaneBank - (ReserveUnit * gubSelDieNumber))) {
						ubCEIndex = ubPlaneBank >> gubBurstsPerBankLog;
						SetFailCEtoSystemInfo(ubCEIndex);
					}
				}
			}
			ftlPreformatShowError(0, 0, 0xEE);
			return 1;
		}

		ulD3UnitNum = gulMaxUserUnitNum;

		if (gubL2P_FORMAT_MODE) {
			uwMaxUnit = (1024 * 1024 * 256) / gul4kEntrysPerUnit; // 限制會隨著SSD, L2P struct 大小變化
		}
		else {
			uwMaxUnit = (256 * 1024 * 256) / gul4kEntrysPerUnit; // 限制會隨著SSD, L2P struct 大小變化
		}

		if ((BIT0 << 12) < uwMaxUnit) { // Max Unit: 4096
			uwMaxUnit = BIT0 << 12;
		}

		if (uwMaxUnit < ulD3UnitNum) { // Depends On L2P struct, D3太多全丟給D1
			U32 ulIncreaseD1;
			if ((ulD3UnitNum - uwMaxUnit) % (gubNum_D1_Part * gubSelDieNumber)) {
				ulIncreaseD1 = ((ulD3UnitNum - uwMaxUnit) / (gubNum_D1_Part * gubSelDieNumber)) + 1;
				ulD1UnitNumPerDie += ulIncreaseD1;
				ulD1UnitNum = ulD1UnitNumPerDie * gubSelDieNumber;
				if (ulD1UnitNum > MAX_D1_UNIT_NUM) {
					ftlPreformatShowError(0, 2, 0xEE);//Unit太多把D1塞炸了 無法判斷FailCE
					return 1;
				}
				ulD3UnitNum = gulMaxUserUnitNum - (ulIncreaseD1 * gubSelDieNumber * gubNum_D1_Part);
			}
			else {
				ulIncreaseD1 = ((ulD3UnitNum - uwMaxUnit) / (gubNum_D1_Part * gubSelDieNumber));
				ulD1UnitNumPerDie += ulIncreaseD1;
				ulD1UnitNum = ulD1UnitNumPerDie * gubSelDieNumber;
				if (ulD1UnitNum > MAX_D1_UNIT_NUM) {
					ftlPreformatShowError(0, 3, 0xEE);//Unit太多把D1塞炸了 無法判斷FailCE
					return 1;
				}
				ulD3UnitNum = uwMaxUnit;
			}

			if (uwMaxUnit < ulD3UnitNum) {//丟完了以後還是太多
				ftlPreformatShowError(0, 4, 0xEE);
				return 1;
			}
			for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
				for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
					VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] -=  (ulIncreaseD1 * gubNum_D1_Part * ((ubGroup * gubSelDieNumber + ubDie) + 1));
				}
			}
			gulMaxUserUnitNum = ulD3UnitNum;
		}

		if ((ulD3UnitNum / ulD1UnitNum) > D1_D3_PE_MIN_RATIO) { /* Should consider D1/D3 PE ratio for copyback quality. */
			ftlPreformatShowError(0, 6, 0xEE);//無法判斷FailCE
			return 1;
		}

		for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
			VT->guwD1UnitsPerGroups[ubDie] = gulMaxUserUnitNum + (ulD1UnitNumPerDie * gubNum_D1_Part * (ubDie + 1));
		}

		guwFWTotalUnitNum = ulD3UnitNum;
		gulMaxUserUnitNum += (ulD1UnitNumPerDie * gubNum_D1_Part * gubSelDieNumber);
	}
#endif


#if (!TLC)
	U32 uwMaxUnit; // 限制會隨著SSD, L2P struct 大小變化
	UWORD ubRUTReset = 0; // 限制會隨著SSD, L2P struct 大小變化
	UWORD uwReducePerGroups[16];
	ubMPSetUnit = 0;

	if (gubL2P_FORMAT_MODE) {
		uwMaxUnit = (1024 * 1024 * 256) / gul4kEntrysPerUnit; // 512G
	}
	else {
		uwMaxUnit = (256 * 1024 * 256) / gul4kEntrysPerUnit; // undre 256G
	}
	if ((BIT0 << 12) < uwMaxUnit) {
		uwMaxUnit = BIT0 << 12;
	}
	if (uwMaxUnit < gulMaxUserUnitNum) {
		ubRUTReset = 1;
	}
	uwMaxUnit += SYSTEM_SETS;
	if (InfoArray->ulTotalUserUnitNumber == 1) {
		uwMaxUnit = (ulNeededUnitNum + SYSTEM_SETS);
		ubRUTReset = 1;
		ubMPSetUnit = 1;
	}
	else if (InfoArray->ulTotalUserUnitNumber == 2) {
		uwMaxUnit = (ulNeededUnitNum + SYSTEM_SETS) - WriteProtect_NUM * gubSelTotalGroups;
		ubRUTReset = 1;
		ubMPSetUnit = 2;
	}
	else if (InfoArray->ulTotalUserUnitNumber > 2) {
		if ( ((InfoArray->ulTotalUserUnitNumber - ReserveUnit * gubSelTotalGroups - SYSTEM_SETS) < ulNeededUnitNum) || (InfoArray->ulTotalUserUnitNumber > gulMaxUserUnitNum)) {
			ftlPreformatShowError(0, 0, 0xFF);//Setting Error
			return 1;
		}
		uwMaxUnit = (InfoArray->ulTotalUserUnitNumber - ReserveUnit * gubSelTotalGroups);
		ubRUTReset = 1;
		ubMPSetUnit = 1;
	}

	if (ubRUTReset) {
		U32 uwAvgUnit = uwMaxUnit >> gubSelTotalGroupsLog;
		U32 uwRemainUnit = uwMaxUnit - (uwAvgUnit << gubSelTotalGroupsLog);
		ubj = 0;
		uwAvgUnit -= SYSTEM_SETS;
		for (ubi = 0; ubi < gubSelTotalGroups; ubi++) {
			if (uwUnitsPerGroups[ubi] > uwAvgUnit) {
				uwReducePerGroups[ubi] = uwUnitsPerGroups[ubi] - uwAvgUnit;
				uwUnitsPerGroups[ubi] = uwAvgUnit;
			}
			else {
				uwReducePerGroups[ubi] = 0;
				uwRemainUnit += (uwAvgUnit - uwUnitsPerGroups[ubi]);
			}
			if (ubi == 0) {
				uwAvgUnit += SYSTEM_SETS;
			}
		}

		uwAvgUnit -= SYSTEM_SETS;
		for (ubi = 0; ubi < gubSelTotalGroups; ubi++) {
			if (uwReducePerGroups[ubi] < uwRemainUnit) {
				uwUnitsPerGroups[ubi] = uwUnitsPerGroups[ubi] + uwReducePerGroups[ubi];
				uwRemainUnit -= uwReducePerGroups[ubi];
				uwReducePerGroups[ubi] = 0;
			}
			else {
				uwUnitsPerGroups[ubi] = uwUnitsPerGroups[ubi] + uwRemainUnit;
				uwRemainUnit = 0;
				uwReducePerGroups[ubi] = uwReducePerGroups[ubi] - uwRemainUnit;
			}
			if (ubi == 0) {
				uwAvgUnit += SYSTEM_SETS;
			}
		}

		gulMaxUserUnitNum = 0;
		for (ubi = 0; ubi < gubSelTotalGroups; ubi++) {
			VT->guwUnitsPerGroups[ubi] = uwUnitsPerGroups[ubi] + gulMaxUserUnitNum;
			gulMaxUserUnitNum += uwUnitsPerGroups[ubi];
		}

		if (ubMPSetUnit) {
			for (ubi = 0; ubi < gubSelTotalGroups; ubi++) {
				VT->guwExtendUnitsIndex[ubi] = ReserveUnit * (ubi + 1);
			}
		}
		else {
			for (ubi = gubSelTotalGroups; ubi > 0; ubi--) {
				if (uwReducePerGroups[ubi - 1]) {
					for (ubj = ubi; ubj < (gubSelTotalGroups + 1); ubj++) {
						VT->guwExtendUnitsIndex[ubj - 1] += uwReducePerGroups[ubi - 1];
					}
				}
			}
		}
	}
#endif

	mDMAC_SV(2, (U32)&guwRUTBASE[0], RUTSize, 0, RUT_Empty, 0, DMA_EN_SETCQ, 1);
	ulGroupStartUnit = 0;
	for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
		for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
			for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
				if (ENABLE_PLANE_CH_CE) {
					ulPlaneBank_ptr[ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie ), DEF_PlanesPerPage_Mcl_Div))] =  (ubPlaneBank  & gubBurstsPerBankMask);
					if (gubRMApreformat) {
						ulPlaneBank_ptr_D1[ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie ), DEF_PlanesPerPage_Mcl_Div))] = (ubPlaneBank  & gubBurstsPerBankMask);
					}
				}
				else {
					ulPlaneBank_ptr[ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie ), DEF_PlanesPerPage_Mcl_Div))] =  ((ubPlaneBank >> gubPlanesPerBurstLog) &gubBurstsPerBankMask);
					if (gubRMApreformat) {
						ulPlaneBank_ptr_D1[ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie ), DEF_PlanesPerPage_Mcl_Div))] = ((ubPlaneBank >> gubPlanesPerBurstLog) &gubBurstsPerBankMask);
					}
				}
				if ((ubGroup * gubSelDieNumber + ubDie )) {
					ulPlaneBank_ptr[ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie ), DEF_PlanesPerPage_Mcl_Div))] += (VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie - 1] << gubBurstsPerBankLog);
					if (gubRMApreformat) {
						ulPlaneBank_ptr_D1[ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie ), DEF_PlanesPerPage_Mcl_Div))] += (VT->guwD1UnitsPerGroups[ubGroup * gubSelDieNumber + ubDie - 1] << gubBurstsPerBankLog);
					}
				}
				else {
					if (gubRMApreformat) {
						ulPlaneBank_ptr_D1[ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie ), DEF_PlanesPerPage_Mcl_Div))] += (VT->guwUnitsPerGroups[ubCEGroup * gubSelDieNumber - 1] << gubBurstsPerBankLog);
					}
				}
			}
		}
	}

	if (gubRUTMixPlaneEnable) {
		for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
			for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
				for (ubCEIndex = 0; ubCEIndex < (gubPlanesPerPage >> gubBurstsPerBankLog); ubCEIndex++) {
					UBYTE ubBlockAvgPTR = (ubGroup * gubSelDieNumber + ubDie ) * (gubPlanesPerPage >> gubBurstsPerBankLog) + ubCEIndex;
					for (ubPlaneBank = 0; ubPlaneBank < gubBurstsPerBank; ubPlaneBank++) {
						UWORD uwCounterPTR = (ubCEIndex << gubBurstsPerBankLog) + ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div));
						if (ulPlaneBank_Counter[uwCounterPTR] < (U32)guwDieBlockAvg[ubBlockAvgPTR]) {
							UWORD uwUnitInGroup = VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie];
							if (ubGroup * gubSelDieNumber + ubDie) {
								uwUnitInGroup -= VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie - 1];
							}

							U32 ulShiftUnit;
							if (((U32)uwUnitInGroup) > ulPlaneBank_Counter[uwCounterPTR]) {
								if (ubGroup * gubSelDieNumber + ubDie) {
									ulShiftUnit = ulPlaneBank_Counter[uwCounterPTR] + VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie - 1];
								}
								else {
									ulShiftUnit = ulPlaneBank_Counter[uwCounterPTR];
								}
							}
							else {
								ulShiftUnit = ulPlaneBank_Counter[uwCounterPTR] - uwUnitInGroup + gulMaxUserUnitNum;
								if (ubGroup * gubSelDieNumber + ubDie) {
									ulShiftUnit += VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie - 1];
								}
							}
							gulRUTDieCompensatePTR[ubBlockAvgPTR] = (ulShiftUnit << gubBurstsPerBankLog) + ubPlaneBank;
							guwDieBlockAvg[ubBlockAvgPTR] = guwDieBlockAvg[ubBlockAvgPTR] - uwUnitInGroup + gulMaxUserUnitNum;
							if (ubGroup * gubSelDieNumber + ubDie) {
								guwDieBlockAvg[ubBlockAvgPTR] += VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie - 1];
							}
							break;
						}
						if (ubPlaneBank == (gubBurstsPerBank - 1)) {
							UWORD uwUnitInGroup = VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie];
							if (ubGroup * gubSelDieNumber + ubDie) {
								uwUnitInGroup -= VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie - 1];
							}
							gulRUTDieCompensatePTR[ubBlockAvgPTR] = 0xFFFFFFFF;
							guwDieBlockAvg[ubBlockAvgPTR] = guwDieBlockAvg[ubBlockAvgPTR] - uwUnitInGroup + gulMaxUserUnitNum;
							if (ubGroup * gubSelDieNumber + ubDie) {
								guwDieBlockAvg[ubBlockAvgPTR] += VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie - 1];
							}
						}
					}
				}
			}
		}
	}
#if TLC
	if (gubRMApreformat) {
		UBYTE ubD3_flag;
		for (ubD3_flag = 0; ubD3_flag < 2; ubD3_flag++) {

			if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {
				ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				return 1;
			}

			for (uli = 0; uli < ulUnitsPerGroup; uli++) {
				for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
					for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
						for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
							if ((ubGroup == 0) && (ubDie == 0) && (ubPlaneBank < (gubPlanesPerBurst << gubBurstsPerBankLog))) { //CE0, CE1
								ubCEIndex = ubPlaneBank >> gubBurstsPerBankLog;
								if (((uli << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask)) < guwSystemAreaNum[ubCEIndex]) {
									continue;
								}
							}

							if (ENABLE_PLANE_CH_CE) {
								ubCEIndex = (ubPlaneBank >> gubBurstsPerBankLog);
							}
							else {
								ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + ((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
							}
							ulFreeblock = (M_Multiplier((uli << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + (M_Multiplier((ubGroup + ubDie * ubCEGroup ), DEF_PlanesPerPage_Mcl_Div)) + ubPlaneBank;
							ulj = ulFreeblock % ulEntryPerDBTPlane;                                    //該PlaneBank放在這個DBT plane的第幾個Byte
							if (ulj == 0) {
								if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
									ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
									for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
										SetFailCEtoSystemInfo(ubCEIndex);
									}
									return 1;
								}
							}
							if ((ubDBTBuffer[ulj] & 0xF0) || (ubDie >= gubSelDieNumber)) {
								// bad block
								continue;
							}
							else {
								uwPlaneBankIndex = ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie ), DEF_PlanesPerPage_Mcl_Div));

								if (ENABLE_RUT_SWAP) {
									if (ubDBTBuffer[ulj] == 0x08) {
										if (ubD3_flag) {
											continue;
										}
									}
									else {
										if (ubD3_flag == 0) {
											continue;
										}
									}
									//3 ============ 記下各個Die含各個PB ' 目前的 ' 指標值 ======================
									//註一 : 這一大段做完，就會從 ' 目前的 ' 變成 ' 最後的 '.
									//註二 : 這裡還沒從指標值轉換成Unit編號.
									//註三 : 不要跟著 ' ulUnitsPerGroup-1 ' 跑，因為它不一定是最後一層.

									if (ubD3_flag == 0) {
										ulLogicUnitPTR = ulPlaneBank_ptr_D1[uwPlaneBankIndex];
										//3 ============ 轉成各個Die含各個PB ' 目前的 ' single free Unit編號 ======================
										if (ulLogicUnitPTR < (ubPlaneBank & gubBurstsPerBankMask)) {
											M_AssertError(1);   //CORRECT_Z  //目前僅做這個檢查.
										}
										else {
											ulLogicUnitPTR -= (ubPlaneBank & gubBurstsPerBankMask); //Sosa加在指標值上的，為了照Plane編號；現在減回來.
										}
										ulLogicUnitPTR = (ulLogicUnitPTR >> gubBurstsPerBankLog);

										if (ulLogicUnitPTR == VT->guwD1UnitsPerGroups[ubGroup * gubSelDieNumber + ubDie] ) {
											continue;
										}

									}
									else {

										ulLogicUnitPTR = ulPlaneBank_ptr[uwPlaneBankIndex];

										//3 ============ 轉成各個Die含各個PB ' 目前的 ' single free Unit編號 ======================
										if (ulLogicUnitPTR < (ubPlaneBank & gubBurstsPerBankMask)) {
											M_AssertError(1);	//CORRECT_Z  //目前僅做這個檢查.
										}
										else {
											ulLogicUnitPTR -= (ubPlaneBank & gubBurstsPerBankMask);	//Sosa加在指標值上的，為了照Plane編號；現在減回來.
										}
										ulLogicUnitPTR = (ulLogicUnitPTR >> gubBurstsPerBankLog);
										if (ubMPSetUnit) {
											if (ulLogicUnitPTR == (gulMaxUserUnitNum + VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie])) {
												continue;
											}
										}

										VT->guwRUTUnitIndexOfLastBlock[uwPlaneBankIndex] = ulLogicUnitPTR;
									}
								}



								ulUnitPTR = (ubDie + ubGroup * gubSelDieNumber) * ulUnitsPerGroup + uli;    //transfer totallyUnit format

								UWORD uwEraseMode = 0;
#if TLC
								uwEraseMode |= BIT_FJOBI_TLC_TREATMENT;
#elif ENABLE_ULTRAMLC
								uwEraseMode |= BIT_FJOBI_FASTPAGE;
#endif
								if ((TLC || ENABLE_ULTRAMLC) && (guoFlashUseType.B.btIntelMicronB0KB == 0) && (!TLC_BICS2) && (!(Hynix3DV6 || Hynix3DV5 || YMTC_JGS))) {
									if (ftl_Erase_Single_Plane(ulUnitPTR, ubPlaneBank, ubCEIndex, uwEraseMode)) {

										SetFailCEtoSystemInfo(ubCEIndex);
										if (ENABLE_VenderCMD_return_PhysicalCE) {
											ubCEIndex = TranslateLogicalCEtoPhysicalCE(ubCEIndex);
										}
										ftlPreformatShowError((ubCEIndex << 4) | ((ulUnitPTR & 0xFFFFFF00) >> 8), ulUnitPTR, 0xC6);
										return 1;
									}
								}

								if (ubD3_flag == 0) {
									M_RUT2DTo1D(ubCEIndex, ulPlaneBank_ptr_D1[uwPlaneBankIndex]) = ulUnitPTR;
									ulPlaneBank_ptr_D1[uwPlaneBankIndex] += gubBurstsPerBank;
								}
								else {
									M_RUT2DTo1D(ubCEIndex, ulPlaneBank_ptr[uwPlaneBankIndex]) = ulUnitPTR;
									ulPlaneBank_ptr[uwPlaneBankIndex] += gubBurstsPerBank;
									UWORD uwUnitIndex = (ulPlaneBank_ptr[uwPlaneBankIndex] >> gubBurstsPerBankLog);

									if (uwUnitIndex == VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie]) {
										ulPlaneBank_ptr[uwPlaneBankIndex] = ulPlaneBank_ptr_D1[uwPlaneBankIndex];
										uwUnitIndex = (ulPlaneBank_ptr[uwPlaneBankIndex] >> gubBurstsPerBankLog);
									}
									if (uwUnitIndex == VT->guwD1UnitsPerGroups[ubGroup * gubSelDieNumber + ubDie]) {
										if (ENABLE_PLANE_CH_CE) {
											ulPlaneBank_ptr[uwPlaneBankIndex] = (gulMaxUserUnitNum << gubBurstsPerBankLog) + (ubPlaneBank  & gubBurstsPerBankMask);
										}
										else {
											ulPlaneBank_ptr[uwPlaneBankIndex] = (gulMaxUserUnitNum << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog) &gubBurstsPerBankMask);
										}
										if ((ubGroup * gubSelDieNumber + ubDie )) {
											ulPlaneBank_ptr[uwPlaneBankIndex] += (VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie - 1] << gubBurstsPerBankLog);
										}
									}
								}

							}
						}
					}
				}
			}
		}



	}
	else
#endif
	{
		if (ftlLoadDBT(0, (U32)&ubDBTBuffer[0])) {
			ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				SetFailCEtoSystemInfo(ubCEIndex);
			}
			return 1;
		}
		for (uli = 0; uli < ulUnitsPerGroup; uli++) {
			for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
				for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
					for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
						if ((ubGroup == 0) && (ubDie == 0) && (ubPlaneBank < (gubPlanesPerBurst << gubBurstsPerBankLog))) { //CE0, CE1
							ubCEIndex = ubPlaneBank >> gubBurstsPerBankLog;
							if (((uli << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask)) < guwSystemAreaNum[ubCEIndex]) {
								continue;
							}
						}
						if (ENABLE_PLANE_CH_CE) {
							ubCEIndex = (ubPlaneBank >> gubBurstsPerBankLog);
						}
						else {
							ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + ((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
						}
						ulFreeblock = (M_Multiplier((uli << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + (M_Multiplier((ubGroup + ubDie * ubCEGroup ), DEF_PlanesPerPage_Mcl_Div)) + ubPlaneBank;
						ulj = ulFreeblock % ulEntryPerDBTPlane;                                    //該PlaneBank放在這個DBT plane的第幾個Byte
						if (ulj == 0) {
							if (ftlLoadDBT(ulFreeblock, (U32)&ubDBTBuffer[0])) {
								ftlPreformatShowError(0, guwDBTBlock[0], 0x28);
								for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
									SetFailCEtoSystemInfo(ubCEIndex);
								}
								return 1;
							}
						}
						if ((ubDBTBuffer[ulj] & 0xF0) || (ubDie >= gubSelDieNumber)) {
							// bad block
							continue;
						}
						else {
							uwPlaneBankIndex = ubPlaneBank + (M_Multiplier((ubGroup * gubSelDieNumber + ubDie ), DEF_PlanesPerPage_Mcl_Div));

							if (ENABLE_RUT_SWAP) {
								//3 ============ 記下各個Die含各個PB ' 目前的 ' 指標值 ======================
								//註一 : 這一大段做完，就會從 ' 目前的 ' 變成 ' 最後的 '.
								//註二 : 這裡還沒從指標值轉換成Unit編號.
								//註三 : 不要跟著 ' ulUnitsPerGroup-1 ' 跑，因為它不一定是最後一層.
								ulLogicUnitPTR = ulPlaneBank_ptr[uwPlaneBankIndex];

								//3 ============ 轉成各個Die含各個PB ' 目前的 ' single free Unit編號 ======================
								if (ulLogicUnitPTR < (ubPlaneBank & gubBurstsPerBankMask)) {
									M_AssertError(1);	//CORRECT_Z  //目前僅做這個檢查.
								}
								else {
									ulLogicUnitPTR -= (ubPlaneBank & gubBurstsPerBankMask);	//Sosa加在指標值上的，為了照Plane編號；現在減回來.
								}
								ulLogicUnitPTR = (ulLogicUnitPTR >> gubBurstsPerBankLog);
								if (ubMPSetUnit) {
									if (ulLogicUnitPTR == (gulMaxUserUnitNum + VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie])) {
										continue;
									}
								}

							}



							ulUnitPTR = (ubDie + ubGroup * gubSelDieNumber) * ulUnitsPerGroup + uli;    //transfer totallyUnit format

							UWORD uwEraseMode = 0;
#if TLC
							uwEraseMode |= BIT_FJOBI_TLC_TREATMENT;
#elif ENABLE_ULTRAMLC
							uwEraseMode |= BIT_FJOBI_FASTPAGE;
#endif
							if ((TLC || ENABLE_ULTRAMLC) && (guoFlashUseType.B.btIntelMicronB0KB == 0) && (!TLC_BICS2) && (!(Hynix3DV6 || Hynix3DV5 || YMTC_JGS))) {
								if (ftl_Erase_Single_Plane(ulUnitPTR, ubPlaneBank, ubCEIndex, uwEraseMode)) {

									SetFailCEtoSystemInfo(ubCEIndex);
									if (ENABLE_VenderCMD_return_PhysicalCE) {
										ubCEIndex = TranslateLogicalCEtoPhysicalCE(ubCEIndex);
									}

									ftlPreformatShowError((ubCEIndex << 4) | ((ulUnitPTR & 0xFFFFFF00) >> 8), ulUnitPTR, 0xC6);
									return 1;
								}
							}
							if (gubRUTMixPlaneEnable) {
								UBYTE ubBlockAvgPTR = (ubGroup * gubSelDieNumber + ubDie ) * (gubPlanesPerPage >> gubBurstsPerBankLog) + (ubPlaneBank >> gubBurstsPerBankLog);
								if (gulRUTDieCompensatePTR[ubBlockAvgPTR] != 0xFFFFFFFF) {
									U32 ulPtrTemp = (ulPlaneBank_ptr[uwPlaneBankIndex] >> gubBurstsPerBankLog);
									if (ulPtrTemp == guwDieBlockAvg[ubBlockAvgPTR]) {
										VT->guwRUTUnitIndexOfLastBlock[((uwPlaneBankIndex >> gubBurstsPerBankLog) << gubBurstsPerBankLog) + (gulRUTDieCompensatePTR[ubBlockAvgPTR] & gubBurstsPerBankMask)] = gulRUTDieCompensatePTR[ubBlockAvgPTR] >> gubBurstsPerBankLog;
										M_RUT2DTo1D(ubCEIndex, gulRUTDieCompensatePTR[ubBlockAvgPTR]) = (ulUnitPTR << gubBurstsPerBankLog) | (ubPlaneBank & gubBurstsPerBankMask);
										gulRUTDieCompensatePTR[ubBlockAvgPTR] += gubBurstsPerBank;
										if ((gulRUTDieCompensatePTR[ubBlockAvgPTR] >> gubBurstsPerBankLog) == VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie]) {
											gulRUTDieCompensatePTR[ubBlockAvgPTR] = (gulMaxUserUnitNum << gubBurstsPerBankLog) + (gulRUTDieCompensatePTR[ubBlockAvgPTR]   & gubBurstsPerBankMask);
											if ((ubGroup * gubSelDieNumber + ubDie )) {
												gulRUTDieCompensatePTR[ubBlockAvgPTR] += (VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie - 1] << gubBurstsPerBankLog);
											}
										}
										if ((gulRUTDieCompensatePTR[ubBlockAvgPTR] >> gubBurstsPerBankLog) == guwDieBlockAvg[ubBlockAvgPTR]) {
											UBYTE ubPlane = (gulRUTDieCompensatePTR[ubBlockAvgPTR] & gubBurstsPerBankMask);
											if (ubPlane == gubBurstsPerBankMask) {
												gulRUTDieCompensatePTR[ubBlockAvgPTR] = 0xFFFFFFFF;
											}
											else {
												UWORD uwCounterPTR = (ubCEIndex << gubBurstsPerBankLog) + ubPlane + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div));
												UWORD uwDieBlockAvgTemp = guwDieBlockAvg[ubBlockAvgPTR] - gulMaxUserUnitNum + VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie];
												if (ubGroup * gubSelDieNumber + ubDie) {
													uwDieBlockAvgTemp -= (VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie - 1] + VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie - 1]);
												}

												for (ubi = (ubPlane + 1); ubi < gubBurstsPerBank; ubi++) {
													uwCounterPTR++;

													if (ulPlaneBank_Counter[uwCounterPTR] < (U32)uwDieBlockAvgTemp) {
														UWORD uwUnitInGroup = VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie];
														if (ubGroup * gubSelDieNumber + ubDie) {
															uwUnitInGroup -= VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie - 1];
														}

														U32 ulShiftUnit;
														if (((U32)uwUnitInGroup) > ulPlaneBank_Counter[uwCounterPTR]) {
															if (ubGroup * gubSelDieNumber + ubDie) {
																ulShiftUnit = ulPlaneBank_Counter[uwCounterPTR] + VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie - 1];
															}
															else {
																ulShiftUnit = ulPlaneBank_Counter[uwCounterPTR];
															}
														}
														else {
															ulShiftUnit = ulPlaneBank_Counter[uwCounterPTR] - uwUnitInGroup + gulMaxUserUnitNum;
															if (ubGroup * gubSelDieNumber + ubDie) {
																ulShiftUnit += VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie - 1];
															}
														}
														gulRUTDieCompensatePTR[ubBlockAvgPTR] = (ulShiftUnit << gubBurstsPerBankLog) + (uwCounterPTR & gubBurstsPerBankMask);
														break;
													}
													if (ubi == (gubBurstsPerBank - 1)) {
														gulRUTDieCompensatePTR[ubBlockAvgPTR] = 0xFFFFFFFF;
													}
												}
											}
										}
										continue;
									}
								}

								M_RUT2DTo1D(ubCEIndex, ulPlaneBank_ptr[uwPlaneBankIndex]) = (ulUnitPTR << gubBurstsPerBankLog) | (ubPlaneBank & gubBurstsPerBankMask);
							}
							else {

								M_RUT2DTo1D(ubCEIndex, ulPlaneBank_ptr[uwPlaneBankIndex]) = ulUnitPTR;
							}
							if (ulLogicUnitPTR > VT->guwRUTUnitIndexOfLastBlock[uwPlaneBankIndex]) {
								VT->guwRUTUnitIndexOfLastBlock[uwPlaneBankIndex] = ulPlaneBank_ptr[uwPlaneBankIndex] >> gubBurstsPerBankLog;
							}
							ulPlaneBank_ptr[uwPlaneBankIndex] += gubBurstsPerBank;
							UWORD uwUnitIndex = (ulPlaneBank_ptr[uwPlaneBankIndex] >> gubBurstsPerBankLog);
							if (uwUnitIndex == VT->guwUnitsPerGroups[ubGroup * gubSelDieNumber + ubDie]) {
								if (ENABLE_PLANE_CH_CE) {
									ulPlaneBank_ptr[uwPlaneBankIndex] = (gulMaxUserUnitNum << gubBurstsPerBankLog) + (ubPlaneBank  & gubBurstsPerBankMask);
								}
								else {
									ulPlaneBank_ptr[uwPlaneBankIndex] = (gulMaxUserUnitNum << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog) &gubBurstsPerBankMask);
								}
								if ((ubGroup * gubSelDieNumber + ubDie )) {
									ulPlaneBank_ptr[uwPlaneBankIndex] += (VT->guwExtendUnitsIndex[ubGroup * gubSelDieNumber + ubDie - 1] << gubBurstsPerBankLog);
								}
							}
						}
					}
				}
			}
		}
	}

Mark_SetUnitToZero:
	mSE_SV(4, (U32)gulVC, (gubPlanesPerVCTable << gubSectorsPerPlaneLog) * 512, DefaultVC, 0);
	if ((gubRUT_EC_set & BIT0) == 0) {
		mSE_SV(4, (U32)gulEC_D3, (gubPlanesPerECTable << gubSectorsPerPlaneLog) * 512, 0, 0);
	}

	ulFreeUnitPTR = 0;
Mark_BuildVTArea:
	// uli is unit count
	for (uli = 0; uli < VTAREA_UNITS; uli++) {
		while (M_RUT2DTo1D(0, ulFreeUnitPTR * gubBurstsPerBank) == 0xBBBB) {
			ulFreeUnitPTR++;
		}
		guwVTTarget.All = ulFreeUnitPTR;
		gulVC[ulFreeUnitPTR].B.btFastPage = 1; //跟318寫法不同,但應該沒差
		ftlEraseTarget(&guwVTTarget, D3, FALSE);
		ftlAddToVTAreaFreeTable(&guwVTTarget);
		ulFreeUnitPTR++;
	}

Mark_GetAndProgramRUTUnit:
	while (M_RUT2DTo1D(0, ulFreeUnitPTR * gubBurstsPerBank) == 0xBBBB) {
		ulFreeUnitPTR++;
	}
	VT->guwRUTTarget.All = ulFreeUnitPTR;
#if TLC
	gulVC[ulFreeUnitPTR].B.btFastPage = 1;
	ftlEraseTarget(&(VT->guwRUTTarget), D3, FALSE);	// 還沒考慮TLC case, 都先不換RUT
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
#else
	gulVC[VT->guwRUTTarget.B.uwTarget].B.btFastPage = 1;
	ftlEraseTarget(&VT->guwRUTTarget, D3, FALSE);
#endif
	ulFreeUnitPTR++;
#if (!TLC)
	//1 Program to Flash
	// ubi is how many planes programed
	// uli is FEntryIndex
	ulDMAAddr = (U32)&M_RUT2DTo1D(0, 0);
	UWORD uwRUTPlanePtr;
	UBYTE ubRUTSizeinPlane = RUTSize / 512 / gubSectorsPerPlane;
	if ((RUTSize / 512 ) % gubSectorsPerPlane) {
		ubRUTSizeinPlane++;
	}
	if (ENABLE_DCACHE) {
		U32 ulRUTSize = RUTSize;	// total RUT is 256KB
		xthal_dcache_region_writeback_inv((void *)ulDMAAddr, ulRUTSize);
	}
	uwRUTPlanePtr = 0;
	while (uwRUTPlanePtr < guwFastPagePlanesPerUnit) {
		ubi = 0;
		uli = 0;
		while ((ubi < ubRUTSizeinPlane) && (uwRUTPlanePtr < guwFastPagePlanesPerUnit)) {

			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((uwRUTPlanePtr >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (uwRUTPlanePtr & gubPlanesPerBurstMask) + (((M_Modular((uwRUTPlanePtr >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
			}
			ftlWriteSinglePlane(VT->guwRUTTarget.B.uwTarget, uwRUTPlanePtr * gub4kEntrysPerPlane,  ubCEIndex, (ulDMAAddr + (uli * 4096)), (MARK_RUT | (uli >> gub4kEntrysPerPlaneLog)), 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT), ubL4KNum, 1, 0);

			uli += gub4kEntrysPerPlane;
			ubi++;
			uwRUTPlanePtr++;
		}
	}

	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}

	//1 read for verification...
	// ubi is how many planes programed
	// uli is FEntryIndex
	UBYTE ubCheckPassCnt;
	//ulDMAAddr = (U32)gubZ2Buffer;
	ulDMAAddr = (U32)&M_RUT2DTo1D(0, 0) ;
	if (ENABLE_DCACHE) {
		U32 ulRUTSize = RUTSize;	// total RUT is 256KB
		xthal_dcache_region_invalidate((void *)ulDMAAddr, ulRUTSize);
	}

	ubi = 0;
	uli = 0;
	while (ubi < ubRUTSizeinPlane) { // total RUT is 128KB
		uwRUTPlanePtr = ubi;
		ubCheckPassCnt = 0;
		while ((uwRUTPlanePtr < guwFastPagePlanesPerUnit) && (ubCheckPassCnt != RUTVeirfyCnt)) {
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((uwRUTPlanePtr >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (uwRUTPlanePtr & gubPlanesPerBurstMask) + (((M_Modular((uwRUTPlanePtr >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
			}
			if (ftlReadSinglePlane(VT->guwRUTTarget.B.uwTarget, uwRUTPlanePtr * gub4kEntrysPerPlane,  ubCEIndex, (ulDMAAddr + (uli * 4096)), (MARK_RUT | (uli >> gub4kEntrysPerPlaneLog)),  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY), ubL4KNum, 1, 0)) {
			}
			else {
				ubCheckPassCnt++;
			}

			uwRUTPlanePtr += ubRUTSizeinPlane;	// 再讀next backup

			//ENABLE_DEBUG_READ_RUTBACKUP
		}
		if ((uwRUTPlanePtr >= guwFastPagePlanesPerUnit) && (ubCheckPassCnt != RUTVeirfyCnt)) {
			ftlPreformatShowError(ubCEIndex, VT->guwRUTTarget.B.uwTarget, 0x24);//在所有CE中無法湊出完整的RUT
			return 1;
		}
		if (ubCheckPassCnt == RUTVeirfyCnt) {	// check next RUT plane
			ubi++;
			uli += gub4kEntrysPerPlane;
		}
	}
#endif

Mark_GetParityUnit:
	while (M_RUT2DTo1D(0, ulFreeUnitPTR * gubBurstsPerBank) == 0xBBBB) {
		ulFreeUnitPTR++;
	}
	VT->gubRSCurrentTag = USERDATA_RSFrameStartIndex;
	VT->gubRSIdleTag = USERDATA_RSFrameStartIndex + USERDATA_RS_Group_Num;
	VT->gubRSShiftTagPhase = 0;
	VT->guwInitInfoTarget.All = ulFreeUnitPTR;
	gulVC[ulFreeUnitPTR].B.btFastPage = 1;
	gulVC[ulFreeUnitPTR].B.ulValidCount = DefaultVC;
	ftlEraseTarget(&(VT->guwInitInfoTarget), D3, FALSE);
	ulFreeUnitPTR++;

Mark_GetVTUnit:
	ftlGetVTAreaFreeBlock(&guwVTTarget);
	VT->gulVTLastMark = 0x55AA;
	for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
		UBYTE ubBurstBankInBank;
		if (ENABLE_PLANE_CH_CE) {
			ubBurstBankInBank = ubPlaneBank & gubBurstsPerBankMask;
			ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		}
		else {
			ubBurstBankInBank = ((ubPlaneBank >> gubPlanesPerBurstLog)&gubBurstsPerBankMask);
			ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + ((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
		}

		VT->guwRUTFBlock[ubPlaneBank] = M_RUT2DTo1D(ubCEIndex, (VT->guwRUTTarget.B.uwTarget << gubBurstsPerBankLog) + ubBurstBankInBank);
	}

Mark_GetVTChildUnit:
	while (M_RUT2DTo1D(0, ulFreeUnitPTR * gubBurstsPerBank) == 0xBBBB) {
		ulFreeUnitPTR++;
	}
	VT->guwVTChildTarget.All = ulFreeUnitPTR;
#if TLC
	gulVC[ulFreeUnitPTR].B.btFastPage = 1;
	gulVC[ulFreeUnitPTR].B.ulValidCount = DefaultVC;
	ftlEraseTarget(&(VT->guwVTChildTarget), D3, FALSE);
#else
	gulVC[VT->guwVTChildTarget.B.uwTarget].B.ulValidCount = DefaultVC;
	gulVC[VT->guwVTChildTarget.B.uwTarget].B.btFastPage = 1;
	ftlEraseTarget(&VT->guwVTChildTarget, D3, FALSE);
#endif
	ulFreeUnitPTR++;

Mark_GeTableUnit:
#if TLC
	for (uli = 0; uli < gubTableUnitNum; uli ++) {
		while (M_RUT2DTo1D(0, ulFreeUnitPTR * gubBurstsPerBank) == 0xBBBB) {
			ulFreeUnitPTR++;
		}
		VT->guwTableTarget[uli].All = ulFreeUnitPTR;
		gulVC[ulFreeUnitPTR].B.ulValidCount = DefaultVC; //table target 的valid count另外記
		gulVC[ulFreeUnitPTR].B.btFastPage = 1;
		if (0 == uli) {
			ftlEraseTarget(&VT->guwTableTarget[uli], D3, FALSE);
		}
		ulFreeUnitPTR++;
	}

	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
#else
	for (uli = 0; uli < gubTableUnitNum; uli ++) {
		while (M_RUT2DTo1D(0, ulFreeUnitPTR * gubBurstsPerBank) == 0xBBBB) {
			ulFreeUnitPTR++;
		}
		VT->guwTableTarget[uli].All = ulFreeUnitPTR;
		gulVC[VT->guwTableTarget[uli].B.uwTarget].B.ulValidCount = DefaultVC; //table target 的valid count另外記
		gulVC[VT->guwTableTarget[uli].B.uwTarget].B.btFastPage = 1;
		ftlEraseTarget(&VT->guwTableTarget[uli], D3, FALSE);
		ulFreeUnitPTR++;
	}
#endif
Mark_BuildUserFreeBlockTable:
	mSE_SV(1, (U32)gubD1LinkTable, 4096, 0xFF, 0);
	mSE_SV(2, (U32)guwFreeBlockTable_D3, 8192, 0xFFFF, 0);

#if (TLC)
	if (TLC && ((gubRUT_EC_set & BIT0) == 0)) {
		UWORD uwD1UnitIndex;
		for (uwD1UnitIndex = 0; uwD1UnitIndex < MAX_D1_UNIT_NUM; uwD1UnitIndex++) {
			gulEC_D1[uwD1UnitIndex].B.ulEraseCount = 0;//大小有可能不對齊32 Byte
		}
	}
#endif

	{
#if TLC
		if (!gubRMApreformat) {
			//U32 ulD1UnitNum = 0U;
			//U32 ulD3UnitNum;
			//UWORD uwRemainder;
			//U32 ulOPUnitNum;
			//UWORD uwMaxUnit;

			ulOPUnitNum = ((ulNativeUnit * 100) / 99);
			if ((ulNativeUnit * 100) % 99) {
				ulOPUnitNum++;
			}
			ulOPUnitNum = ulOPUnitNum - ulNativeUnit;

			if (ulNeededUnitNum > gulMaxUserUnitNum) {
				U32 ulMaxUnitNumPerPlaneBank;
				for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
					for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
						for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
							if (ubDie == 0) {
								ulMaxUnitNumPerPlaneBank = ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
							}
							else {
								ulMaxUnitNumPerPlaneBank += ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
							}
						}
						if (ulNeededUnitNum > ulMaxUnitNumPerPlaneBank) {
							ubCEIndex = ubPlaneBank >> gubBurstsPerBankLog;
							SetFailCEtoSystemInfo(ubCEIndex);
						}
					}
				}
				ftlPreformatShowError(0, 0, 0xEE);//James_Li
				return 1;
			}
			if (gubRUT_EC_set & BIT1) {
				ulD1UnitNum = (VT->guwD1UnitsPerGroups[gubSelDieNumber - 1] - VT->guwUnitsPerGroups[gubSelDieNumber - 1]) / gubNum_D1_Part;
				ulD3UnitNum = VT->guwUnitsPerGroups[gubSelDieNumber - 1];
				if (ulD1UnitNum != VT->gubD1UnitNum) {
					ftlPreformatShowError(0, 9, 0xEE);
					return 1;
				}
			}
			else {
				ulD3UnitNum = ulNeededUnitNum;

#if Hynix3DV6
				if (guoFlashUseType.B.btHynix3DV6) {

					ulD1UnitNum = ((U32)MIN_4P_D1_UNIT_NUM_V6 << gubSelTotalGroupsLog);
				}
				else if (guoFlashUseType.B.btHynix3DV5) {
					if (guoFlashUseType.B.btFourPlane) {
						ulD1UnitNum = ((U32)MIN_4P_D1_UNIT_NUM_V5 << gubSelTotalGroupsLog);
					}
					else {
						ulD1UnitNum = ((U32)MIN_2P_D1_UNIT_NUM_V5 << gubSelTotalGroupsLog);
					}
				}

#elif B0KB //Debug
				if ((gubCENumber == 5) || (gubCENumber == 10) || (gubCENumber == 11)) {
					ulD1UnitNum = ((U32)MIN_2P_D1_UNIT_NUM << gubSelTotalGroupsLog);
				}
				else {
					ulD1UnitNum = ((U32)MIN_D1_UNIT_NUM << gubSelTotalGroupsLog);
				}
#else
#if TSB_BICS4_SUPPORT
				if (TLC_BICS2 && (guoFlashUseType.B.btToshibaBiCs3 || guoFlashUseType.B.btToshibaBiCs4)) {
#else
				if (TLC_BICS2 && guoFlashUseType.B.btToshibaBiCs3) {
#endif
					ulD1UnitNum = ((U32)gubMIN_D1_UNIT_NUM_BiCS3 << gubSelTotalGroupsLog);
				}
				else {
					ulD1UnitNum = ((U32)MIN_D1_UNIT_NUM << gubSelTotalGroupsLog);
				}
#endif
				if ((ulNeededUnitNum + (ulD1UnitNum * gubNum_D1_Part)) > gulMaxUserUnitNum) {
					U32 ulMaxUnitNumPerPlaneBank;
					for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
						for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
							for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
								if (ubDie == 0) {
									ulMaxUnitNumPerPlaneBank = ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
								}
								else {
									ulMaxUnitNumPerPlaneBank += ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
								}
							}

							if ((ulNeededUnitNum + (ulD1UnitNum * gubNum_D1_Part)) > ulMaxUnitNumPerPlaneBank) {
								ubCEIndex = ubPlaneBank >> gubBurstsPerBankLog;
								SetFailCEtoSystemInfo(ubCEIndex);
							}
						}
					}
					ftlPreformatShowError(0, 1, 0xEE);//James_Li
					return 1;
				}

				uwRemainder = gulMaxUserUnitNum - ulD3UnitNum - (ulD1UnitNum * gubNum_D1_Part);
				while (uwRemainder) {
					if (uwRemainder >= ((ulOPUnitNum * 2) + (((U32)gubNum_D1_Part * 1) << gubSelTotalGroupsLog))) { // 2: 2% OP, 1: 1 D1 Unit
						ulD3UnitNum += (ulOPUnitNum * 2);
						ulD1UnitNum += ((U32)1 << gubSelTotalGroupsLog);
						uwRemainder -= ((ulOPUnitNum * 2) + (((U32)gubNum_D1_Part * 1) << gubSelTotalGroupsLog));
					}
					else {
						ulD3UnitNum += uwRemainder;
						uwRemainder = 0;
					}

					if (ulD1UnitNum > MAX_D1_UNIT_NUM) {
						ulD1UnitNum = MAX_D1_UNIT_NUM;
						ulD3UnitNum = gulMaxUserUnitNum - (ulD1UnitNum * gubNum_D1_Part);
						break;
					}
				}

				if (gubL2P_FORMAT_MODE) {
					uwMaxUnit = (1024 * 1024 * 256) / gul4kEntrysPerUnit; // 限制會隨著SSD, L2P struct 大小變化
				}
				else {
					uwMaxUnit = (256 * 1024 * 256) / gul4kEntrysPerUnit; // 限制會隨著SSD, L2P struct 大小變化
				}

				if ((BIT0 << 12) < ulD3UnitNum) { // Max Unit: 4096
					ulD3UnitNum = BIT0 << 12;
				}

				if (uwMaxUnit < ulD3UnitNum) { // Depends On L2P struct
					if ((ulD3UnitNum - uwMaxUnit) % gubNum_D1_Part) {
						ulD1UnitNum += ((ulD3UnitNum - uwMaxUnit) / gubNum_D1_Part);
						ulD1UnitNum++;
						if (ulD1UnitNum > MAX_D1_UNIT_NUM) {
							ftlPreformatShowError(0, 2, 0xEE);//Unit太多把D1塞炸了 無法判斷FailCE
							return 1;
						}
						ulD3UnitNum = gulMaxUserUnitNum - (ulD1UnitNum * gubNum_D1_Part);
					}
					else {
						ulD1UnitNum += ((ulD3UnitNum - uwMaxUnit) / gubNum_D1_Part);
						if (ulD1UnitNum > MAX_D1_UNIT_NUM) {
							ftlPreformatShowError(0, 3, 0xEE);//Unit太多把D1塞炸了 無法判斷FailCE
							return 1;
						}
						ulD3UnitNum = uwMaxUnit;
					}

					if (uwMaxUnit < ulD3UnitNum) {
						ftlPreformatShowError(0, 4, 0xEE);//分配完後D3還是太多
						return 1;
					}
				}

				if (gulMaxUserUnitNum - ulD3UnitNum - (ulD1UnitNum * gubNum_D1_Part)) {//如果分配正確就會gulMaxUserUnitNum = ulD3UnitNum + (ulD1UnitNum * gubNum_D1_Part)
					ftlPreformatShowError(0, 5, 0xEE);
					return 1;
				}
#if TSB_BICS4_SUPPORT
				if ((ulD3UnitNum / ulD1UnitNum) > (guoFlashUseType.B.btToshibaBiCs4 ? D1_D3_PE_MIN_RATIO_BiCS4 : D1_D3_PE_MIN_RATIO)) { /* Should consider D1/D3 PE ratio for copyback quality. */
#else
				if ((ulD3UnitNum / ulD1UnitNum) > D1_D3_PE_MIN_RATIO) { /* Should consider D1/D3 PE ratio for copyback quality. */
#endif
					ftlPreformatShowError(0, 6, 0xEE);
					return 1;
				}
			}
		}
#else

		if (ubMPSetUnit < 2) {
			if (ulNeededUnitNum > gulMaxUserUnitNum) {
				U32 ulMaxUnitNumPerPlaneBank;
				for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
					for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
						for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
							if (ubDie == 0) {
								ulMaxUnitNumPerPlaneBank = ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
							}
							else {
								ulMaxUnitNumPerPlaneBank += ulPlaneBank_Counter[ubPlaneBank + (M_Multiplier((ubGroup + ubDie * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))];
							}
						}

						if (ulNeededUnitNum > ulMaxUnitNumPerPlaneBank) {
							ubCEIndex = ubPlaneBank >> gubBurstsPerBankLog;
							SetFailCEtoSystemInfo(ubCEIndex);
						}
					}
				}
				ftlPreformatShowError(0, 0, 0xFF);//James_Li
				return 1;
			}
		}
		//UWORD uwMaxUnit; // 限制會隨著SSD, L2P struct 大小變化
		if (gubL2P_FORMAT_MODE) {
			uwMaxUnit = (1024 * 1024 * 256) / gul4kEntrysPerUnit; // 512G
		}
		else {
			uwMaxUnit = (256 * 1024 * 256) / gul4kEntrysPerUnit; // undre 256G
		}
		U32 ulD3UnitNum = gulMaxUserUnitNum;
		if ((BIT0 << 12) < ulD3UnitNum) {
			ulD3UnitNum = BIT0 << 12;
		}
		if (uwMaxUnit < ulD3UnitNum) {
			ulD3UnitNum = uwMaxUnit;
		}
#endif

#if TLC
		guwFWTotalUnitNum = ulD3UnitNum;

		if (!gubRMApreformat) {
			if ((gubRUT_EC_set & BIT1) == 0) {
				if ((ENABLE_MULTI_DIE && guoFlashUseType.B.btMultiDie && gubParaInitialized) && (gubSelDieNumber > 1)) {
					//Multi-die
					UBYTE ubDieIndex;
					UBYTE ubShiftUnitNum;
					UWORD uwSwapIndex;
					UBYTE ubBurstBank;
					UWORD uwFBlock;
					UBYTE ubD1NumByDie[4];
					//cal shiftNum
					for (ubDieIndex = 0; ubDieIndex < gubSelDieNumber; ubDieIndex++) {
						if (ubDieIndex == (gubSelDieNumber - 1)) {
							ubD1NumByDie[ubDieIndex] = ulD1UnitNum;
							UBYTE ubDieIndex_2;
							for (ubDieIndex_2 = 0; ubDieIndex_2 < (gubSelDieNumber - 1); ubDieIndex_2++) {
								ubD1NumByDie[ubDieIndex] -= ubD1NumByDie[ubDieIndex_2];
							}
						}
						else {
							//每個DIE的D1數量以這個DIE的TotalUnit乘上整體D1Unit/AllUnit比例
							ubD1NumByDie[ubDieIndex] = (uwUnitsPerGroups[ubDieIndex] * ulD1UnitNum) / gulMaxUserUnitNum;
#if TSB_BICS4_SUPPORT
							if (((uwUnitsPerGroups[ubDieIndex] - ((UWORD)ubD1NumByDie[ubDieIndex] * gubNum_D1_Part)) / ubD1NumByDie[ubDieIndex]) > (guoFlashUseType.B.btToshibaBiCs4 ? D1_D3_PE_MIN_RATIO_BiCS4 : D1_D3_PE_MIN_RATIO)) {
#else
							if (((uwUnitsPerGroups[ubDieIndex] - ((UWORD)ubD1NumByDie[ubDieIndex] * gubNum_D1_Part)) / ubD1NumByDie[ubDieIndex]) > D1_D3_PE_MIN_RATIO) {
#endif
								if (((uwUnitsPerGroups[ubDieIndex]) * ulD1UnitNum) % gulMaxUserUnitNum) {
									ubD1NumByDie[ubDieIndex]++;
									// If "D3/D1 > D1_D3_PE_MIN_RATIO" still happens, which will be checked by Error case 8.
								}
								else {
									ftlPreformatShowError(ubDieIndex, 7, 0xEE);
									return 1;
								}
							}
						}

						if (ENABLE_DEBUG_UART_YT) {
							UartString("\n D1 Die");
							UartWordHex((UWORD)ubDieIndex);
							UartString(":");
							UartWordHex((UWORD)ubD1NumByDie[ubDieIndex]);
							UartString(", uwUnitsPerGroups:");
							UartWordHex((UWORD)uwUnitsPerGroups[ubDieIndex]);
						}
						//把D1 MixPlane Unit換掉
						ftlSwapD1MixPlaneUnit(&ulPlaneBank_Counter[(M_Multiplier((ubDieIndex * ubCEGroup), DEF_PlanesPerPage_Mcl_Div))], ubDieIndex, ubD1NumByDie[ubDieIndex]);
					}

					for (ubDieIndex = gubSelDieNumber; ubDieIndex != 1; ubDieIndex--) {
						UBYTE ubDieIndex_2;
						ubShiftUnitNum = 0;
						for (ubDieIndex_2 = 0; ubDieIndex_2 < (ubDieIndex - 1); ubDieIndex_2++) {
							ubShiftUnitNum += ubD1NumByDie[ubDieIndex_2];
						}
						if (ubDieIndex == gubSelDieNumber) {
							VT->guwD1UnitsPerGroups[ubDieIndex - 1] = VT->guwUnitsPerGroups[ubDieIndex - 1]; //D1 底線 = 原本D3底線
							VT->guwUnitsPerGroups[ubDieIndex - 1] = guwFWTotalUnitNum;                      //D3 底線 = FW Total Unit Number
						}
						VT->guwD1UnitsPerGroups[ubDieIndex - 2] = guwFWTotalUnitNum + (ubShiftUnitNum * D1_UNIT_NUM_PER_LINK);
						VT->guwUnitsPerGroups[ubDieIndex - 2] = VT->guwUnitsPerGroups[ubDieIndex - 2] - (ubShiftUnitNum * D1_UNIT_NUM_PER_LINK);
						//Swap RUT
						for (uwSwapIndex = 0; uwSwapIndex < (ubShiftUnitNum * D1_UNIT_NUM_PER_LINK); uwSwapIndex++) {
							for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
								if (ENABLE_PLANE_CH_CE) {
									ubBurstBank = ubPlaneBank & gubBurstsPerBankMask;
									ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
								}
								else {
									ubBurstBank = (ubPlaneBank >> gubPlanesPerBurstLog) & gubBurstsPerBankMask;
									ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + ((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
								}

								uwFBlock = M_RUT2DTo1D(ubCEIndex, ((guwFWTotalUnitNum  + uwSwapIndex) << gubBurstsPerBankLog) + ubBurstBank);
								M_RUT2DTo1D(ubCEIndex, ((guwFWTotalUnitNum  + uwSwapIndex) << gubBurstsPerBankLog) + ubBurstBank) = M_RUT2DTo1D(ubCEIndex, ((VT->guwUnitsPerGroups[ubDieIndex - 2] + uwSwapIndex) << gubBurstsPerBankLog) + ubBurstBank);
								M_RUT2DTo1D(ubCEIndex, ((VT->guwUnitsPerGroups[ubDieIndex - 2] + uwSwapIndex) << gubBurstsPerBankLog) + ubBurstBank) = uwFBlock;
							}
						}
					}
				}
				else {
					//把D1 MixPlane Unit換掉
					ftlSwapD1MixPlaneUnit(&ulPlaneBank_Counter[0], 0, ulD1UnitNum);
					//Single Die
					VT->guwD1UnitsPerGroups[0] = VT->guwUnitsPerGroups[0];
					VT->guwUnitsPerGroups[0] = guwFWTotalUnitNum;
				}
			}
			else {
				if ((ulD1UnitNum != (VT->guwD1UnitsPerGroups[gubSelDieNumber - 1] - VT->guwUnitsPerGroups[gubSelDieNumber - 1]) / gubNum_D1_Part)
				        || (ulD3UnitNum != VT->guwUnitsPerGroups[gubSelDieNumber - 1])
				        || (ulD1UnitNum != VT->gubD1UnitNum)) {
					ftlPreformatShowError(0, 10, 0xEE);
					return 1;
				}
			}
		}

		//*******************************************************************************************************************
		//                      Program RUT Table
		//*******************************************************************************************************************
		//1 Program to Flash
		// ubi is how many planes programed
		// uli is FEntryIndex
		ulDMAAddr = (U32)&M_RUT2DTo1D(0, 0) ;
		UWORD uwRUTPlanePtr;
		UBYTE ubRUTSizeinPlane = RUTSize / 512 / gubSectorsPerPlane;
		if ((RUTSize / 512 ) % gubSectorsPerPlane) {
			ubRUTSizeinPlane++;
		}
		if (ENABLE_DCACHE) {
			U32 ulRUTSize = RUTSize;	// total RUT is 256KB
			xthal_dcache_region_writeback_inv((void *)ulDMAAddr, ulRUTSize);
		}
		uwRUTPlanePtr = 0;
		while (uwRUTPlanePtr < guwFastPagePlanesPerUnit) {
			ubi = 0;
			uli = 0;
			while ((ubi < ubRUTSizeinPlane) && (uwRUTPlanePtr < guwFastPagePlanesPerUnit)) {

				//FlashQueue_t *FQ;
				//UBYTE ubDepth, ubL4kLink;
				if (ENABLE_PLANE_CH_CE) {
					ubCEIndex = M_Modular((uwRUTPlanePtr >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
				}
				else {
					ubCEIndex = (uwRUTPlanePtr & gubPlanesPerBurstMask) + (((M_Modular((uwRUTPlanePtr >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
				}

				ftlWriteSinglePlane(VT->guwRUTTarget.B.uwTarget, uwRUTPlanePtr * gub4kEntrysPerPlane,  ubCEIndex, (ulDMAAddr + (uli * 4096)), (MARK_RUT | (uli >> gub4kEntrysPerPlaneLog)), 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT), ubL4KNum, 1, 0);
				uli += gub4kEntrysPerPlane;
				ubi++;
				uwRUTPlanePtr++;
			}
		}

		while (gFQI.ubFQLinkNumber) {
			M_SwitchTask();
		}

		//1 read for verification...
		// ubi is how many planes programed
		// uli is FEntryIndex

		UBYTE ubCheckPassCnt;
		//ulDMAAddr = (U32)gubZ2Buffer;
		ulDMAAddr = (U32)&M_RUT2DTo1D(0, 0);
		if (ENABLE_DCACHE) {
			U32 ulRUTSize = RUTSize;	// total RUT is 256KB
			xthal_dcache_region_invalidate((void *)ulDMAAddr, ulRUTSize);
		}

		ubi = 0;
		uli = 0;
		while (ubi < ubRUTSizeinPlane) { // total RUT is 128KB
			uwRUTPlanePtr = ubi;
			ubCheckPassCnt = 0;

			//ubFQIndex[ubi] = gFQI.ubFQLinkFree;
			//FlashQueue_t *FQ;
			//UBYTE ubDepth, ubL4kLink;
			while ((uwRUTPlanePtr < guwFastPagePlanesPerUnit) && (ubCheckPassCnt != RUTVeirfyCnt)) {
				if (ENABLE_PLANE_CH_CE) {
					ubCEIndex = M_Modular((uwRUTPlanePtr >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
				}
				else {
					ubCEIndex = (uwRUTPlanePtr & gubPlanesPerBurstMask) + (((M_Modular((uwRUTPlanePtr >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog));
				}
				if (ftlReadSinglePlane(VT->guwRUTTarget.B.uwTarget, uwRUTPlanePtr * gub4kEntrysPerPlane,  ubCEIndex, (ulDMAAddr + (uli * 4096)), (MARK_RUT | (uli >> gub4kEntrysPerPlaneLog)),  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT), ubL4KNum, 1, 0)) {
				}
				else {
					ubCheckPassCnt++;
				}

				uwRUTPlanePtr += ubRUTSizeinPlane;	// 再讀next backup

				//ENABLE_DEBUG_READ_RUTBACKUP
			}
			if ((uwRUTPlanePtr >= guwFastPagePlanesPerUnit) && (ubCheckPassCnt != RUTVeirfyCnt)) {
				ftlPreformatShowError(ubCEIndex, VT->guwRUTTarget.B.uwTarget, 0x24);//全部CE中湊不出完整RUT
				return 1;
			}
			if (ubCheckPassCnt == RUTVeirfyCnt) {	// check next RUT plane
				ubi++;
				uli += gub4kEntrysPerPlane;
			}
		}


		//==============================================================================================================================
#endif
#if TLC
		//**************************************************************************************
		//      Add User Unit
		//**************************************************************************************
		U32 ulAddFreeTableUnitNum = ulD3UnitNum - ulFreeUnitPTR;
		U32 ulAddFreeTableUnitCount = 0;
		U32 ulAddFreeTableUnitIndex = 0;
		U32 ulDieStartIndex;
		UBYTE ubDieIndex;
		UWORD uwFreeBlock_D3ByDie[4];
		UWORD uwFreeBlock_D1ByDie[4];
		UWORD uwRUTBadCNT = 0;
		for (ubDieIndex = 0; ubDieIndex < gubSelDieNumber; ubDieIndex++) {
			uwFreeBlock_D3ByDie[ubDieIndex] = 0;
			uwFreeBlock_D1ByDie[ubDieIndex] = 0;
		}
		while (ulAddFreeTableUnitCount < ulAddFreeTableUnitNum) {
			FTarget_t uwTarget;
			UWORD uwUnit;
			for (ubDieIndex = 0; ubDieIndex < gubSelDieNumber; ubDieIndex++) {
				ulDieStartIndex = (ubDieIndex == 0) ? ulFreeUnitPTR : VT->guwUnitsPerGroups[ubDieIndex - 1];
				uwUnit = ulDieStartIndex + ulAddFreeTableUnitIndex;
				if (uwUnit < VT->guwUnitsPerGroups[ubDieIndex]) {
					uwTarget.B.uwTarget = uwUnit;
					if (M_RUT2DTo1D(0, uwUnit * gubBurstsPerBank) != 0xBBBB) {
						VT->gulDebugInfo[0] = 31; //AddFreeQ Caller Num
						ftlAddToUserFreeTable(&uwTarget, D3);
#if Hynix3DV6
						if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
							ftlEraseTarget(&uwTarget, UnitTLCTreatment | D3, 0);
						}
#endif
						M_GetDieNumber(ubDie, uwTarget.B.uwTarget, D3);
						uwFreeBlock_D3ByDie[ubDie]++;
					}
					else {
						uwRUTBadCNT++;
					}
					ulAddFreeTableUnitCount ++;

					if (ENABLE_DEBUG_UART_YT) {
						//UBYTE ubDie;

						UartString("\nD3Target:");
						UartLongHex((U32)uwTarget.B.uwTarget);
						UartString(" Die:");
						UartLongHex((U32)ubDie);
					}

				}
			}
			ulAddFreeTableUnitIndex++;
		}
		if (gulMaxUserUnitNum - ulNeededUnitNum < uwRUTBadCNT) {
			ftlPreformatShowError(0, 0, 0x90);
		}

		ulFreeUnitPTR = ulD3UnitNum;

		if (TLC) {
			uli = 0;
			while (uli < ulD1UnitNum) {
				FTarget_t uwTarget;
				uwTarget.B.uwTarget = uli;
				ftlAddToUserFreeTable(&uwTarget, D1);
#if Hynix3DV6
				if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
					ftlEraseTarget(&uwTarget, UnitTLCTreatment | D1, 0);
				}
#endif
				M_GetDieNumber(ubDie, uwTarget.B.uwTarget, D1);
				uwFreeBlock_D1ByDie[ubDie]++;
				uli++;
			}
			uli = 0;
			while (guwFreeBlockTable_D1[uli].B.uwTarget != Default_Unit) {
				uli ++;
			}
			guwFreeBlockTable_D1[uli].All = 0;
			guwFreeBlockTable_D1[uli + 1].All = 0;
			VT->gubD1UnitNum = (UBYTE)ulD1UnitNum;
		}

		if (ENABLE_DEBUG_UART_YT) {
			for (ubDieIndex = 0; ubDieIndex < gubSelDieNumber; ubDieIndex++) {
				UartString("\nDie");
				UartWordHex(ubDieIndex);
				UartString(" D3:");
				UartWordHex(uwFreeBlock_D3ByDie[ubDieIndex]);
				UartString(" D1:");
				UartWordHex(uwFreeBlock_D1ByDie[ubDieIndex]);
			}
		}

		if ((ENABLE_MULTI_DIE && guoFlashUseType.B.btMultiDie && gubParaInitialized) && (gubSelDieNumber > 1))  {
			UBYTE ubDieIndex;
			for (ubDieIndex = 0; ubDieIndex < gubSelDieNumber; ubDieIndex++) {
#if TSB_BICS4_SUPPORT
				if ( ((uwFreeBlock_D3ByDie[ubDieIndex] / uwFreeBlock_D1ByDie[ubDieIndex]) > (guoFlashUseType.B.btToshibaBiCs4 ? D1_D3_PE_MIN_RATIO_BiCS4 : D1_D3_PE_MIN_RATIO))) { /* Should consider D1/D3 PE ratio for copyback quality. */
#else
				if ( ((uwFreeBlock_D3ByDie[ubDieIndex] / uwFreeBlock_D1ByDie[ubDieIndex]) > D1_D3_PE_MIN_RATIO)) { /* Should consider D1/D3 PE ratio for copyback quality. */
#endif
					ftlPreformatShowError(ubDieIndex, 8, 0xEE);
					return 1;
				}
			}
		}
#else
		UWORD uwRUTBadCNT = 0;
		while (ulFreeUnitPTR < ulD3UnitNum) {
			FTarget_t uwTarget;
			uwTarget.B.uwTarget = ulFreeUnitPTR;
			if (M_RUT2DTo1D(0, ulFreeUnitPTR * gubBurstsPerBank) != 0xBBBB) {
				ftlAddToUserFreeTable(&uwTarget, D3);
			}
			else {
				uwRUTBadCNT++;
			}
			ulFreeUnitPTR++;
		}
		if (gulMaxUserUnitNum - ulNeededUnitNum < uwRUTBadCNT) {
			ftlPreformatShowError(0, 0, 0x90);
		}
		guwFWTotalUnitNum = ulFreeUnitPTR;

#endif
		ftlSearchMixPlaneInFreeBlockTable();//Search and mark MixPlane Unit

		if (ubMPSetUnit < 2) {
			if (gulMaxUserUnitNum < (guwFWTotalUnitNum - uwTotalReduceUnit)) {//Unit太多...
				ftlPreformatShowError(0, 0, 0xFF);
				return 1;
			}
		}
	}

	if (ENABLE_RUT_SWAP) {	//在guwFWTotalUnitNum確定後，算RUT剩下要的東西.
		for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {
			//3 ============ 各個Die起始的single free Unit編號 ======================
			VT->guwDieStartSFUnitIndex[ubDie] = 0;
			if (ubDie == 0) {
				//				VT->guwDieStartSFUnitIndex[ubDie] = VT->guwUnitsPerGroups[gubDieNumber - 1];
				VT->guwDieStartSFUnitIndex[ubDie] = guwFWTotalUnitNum + (VT->gubD1UnitNum * D1_UNIT_NUM_PER_LINK)/* + ReserveUnit*/;
			}
			else {
				//				VT->guwDieStartSFUnitIndex[ubDie] = VT->guwUnitsPerGroups[gubDieNumber - 1] + VT->guwExtendUnitsIndex[ubDie - 1];
				VT->guwDieStartSFUnitIndex[ubDie] = VT->guwDieStartSFUnitIndex[0] + VT->guwExtendUnitsIndex[ubDie - 1];
			}
		}
		//3 ============ 最後一個Die的最大single free Unit編號 ======================
		//		VT->guwDieStartSFUnitIndex[ubDie] = VT->guwUnitsPerGroups[gubDieNumber - 1] + VT->guwExtendUnitsIndex[ubDie - 1] - 1;
		VT->guwDieStartSFUnitIndex[gubSelDieNumber] = VT->guwDieStartSFUnitIndex[0] + VT->guwExtendUnitsIndex[ubDie - 1] - 1;

#if TLCRUT
		//3 ============ 各個Die在D1-Only區域的起始single free Unit編號 ( 底線，從此線往上放 ) ======================
		if ((gubRUT_EC_set & BIT1) == 0) {
			UBYTE ubBytesPerUnitNumLog = (gubBurstsPerBankLog + 1);	//+1 : 以2B表示一個unit；如果是以4B來表示，要+2.
			UWORD uwPartTemp = ((guwRUTSizePerCE >> ubBytesPerUnitNumLog) - 1) - VT->guwDieStartSFUnitIndex[gubSelDieNumber];
			uwPartTemp = uwPartTemp / gubSelDieNumber;
			for (ubDie = 1; ubDie < gubSelDieNumber; ubDie++) {
				VT->guwDieStartSFUnitIndex[gubSelDieNumber + ubDie] = VT->guwDieStartSFUnitIndex[gubSelDieNumber + ubDie - 1] + uwPartTemp;
			}
		}
#endif
	}

Mark_Set_SLCPool_Size:
	// SLCPool 先暫定10G大小
	if (ENABLE_FORCE_MLC) {
		VT->gulSLCPoolSize = 0;
	}
	else if (ENABLE_FORCE_SLC) {
		VT->gulSLCPoolSize = gulDiskSizeInMB * 1024 * 1024 / 4096;
	}
	else if (ENABLE_DEBUG_FORCE_SLC_POOL_SIZE) {
		VT->gulSLCPoolSize = FORCE_SLC_SIZE;
	}
	else {
		if (InfoArray->ubSLCPoolEnable) {
			VT->gulSLCPoolSize = InfoArray->ulSLCPoolSizeIn4KEntry ;
		}
#if TLC
		else {
			//U32 D1SizeInMB = (((VT->gubD1UnitNum - FreeD1CountThatNeedWriteData) * gulDataPlanesPerUnit) * 16) / 1024;
			//暫定 1.5G for Test
			VT->gulSLCPoolSize = ForceSLCPoolSizeInMB * 1024 / 4;
		}
#else
		else
#if MicronFlashOnly
			VT->gulSLCPoolSize = 2048 * (1024 * 1024 / 4096);
#else
			VT->gulSLCPoolSize = ForceSLCPoolSizeInMB * (1024 * 1024 / 4096);
#endif
#endif
	}
	if ( (VT->gulSLCPoolSize > (1536 * 1024 / 4)) && (gulDiskSizeInMB <  (16 * 1024 ))) {
		VT->gulSLCPoolSize = 1536 * 1024 / 4;
	}
	//Fla_Set_Offset_DQS_Delay(0);
#if MicronFlashOnly || YMTC_JGS
	if (gulParamPageInfo.ONFI.btNandEmulatorNoSLCPool && gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
	if (gulParamPageInfo.JEDEC.btNandEmulatorNoSLCPool && gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
	{
		VT->gulSLCPoolSize = 0;
	}
	gulSLCPoolSize_Sync = VT->gulSLCPoolSize;
	VT->guwOldRUTTarget.All = Default_Unit;
	VT->guwOldVTChildTarget.B.uwTarget = Default_Unit;
	VT->gulSLCPoolSizeMax = VT->gulSLCPoolSize;
	VT->guwFreeBlockCountMax = VT->guwFreeBlockCount_D3;
Mark_GetGRUnit:
	ftlGetUserFreeBlock(&VT->guwGRTarget[0], 0, D3);
	gulVC[VT->guwGRTarget[0].B.uwTarget].B.ulValidCount = 0;


	VT->guwMinimumEC_UsedUnit = VT->guwGRTarget[0].B.uwTarget;
	VT->guwOpenGRTarget.B.uwTarget = Default_Unit;
	VT->guwInitInfoOpenGRP2LPtr = 0xFFFF;
#if (TLC)
#if 0
	if (ENABLE_SLCPOOL && \
	        (VT->guwFreeBlockCount_D1 <= (FreeD1CountThatNeedWriteData + 1)) && \
	        (VT->guwFreeBlockCount_D3 > SLCPOOL_FREEBLOCK_COUNT_THRESHOLD) && \
	        (gulLastCleanGRGroupCount < ((GRTableSize / (4 * guwL2PEntrysPerL2PGroup)) + 1)) && \
	        (gulEC_D3[guwFreeBlockTable_D3[3].B.uwTarget].B.uwEraseCount < SLCPOOL_ERASE_COUNT_THRESHOLD)    && \
	        (VT->gulSLCPoolSize >= (gubMaxZipRate * gulFastPage4kEntrysPerUnit))) { //not enough SLCPoolSize, which is (max L4k in a SLC unit)
		VT->guwGRTarget[0].B.btFastPage = 1;
		gulVC[VT->guwGRTarget[0].B.uwTarget].B.btFastPage = 1;
		gulVC[VT->guwGRTarget[0].B.uwTarget].B.btInD1Unit = 0;
		ftlEraseTarget(&VT->guwGRTarget[0], D3); /* Erase if in SLC_POOL */
	}
	else
#endif
	{
		FTarget_t uwD1Target;
		if ((ENABLE_MULTI_DIE && guoFlashUseType.B.btMultiDie && gubParaInitialized) && (gubSelDieNumber > 1)) {
			UBYTE ubDieByD3;
			M_GetDieNumber(ubDieByD3, VT->guwGRTarget[0].B.uwTarget, D3);
			ftlGetUserFreeBlockByDie(&uwD1Target, ubDieByD3, D1);
			if (ENABLE_DEBUG_UART_YT) {
				UBYTE ubDieByD1;
				M_GetDieNumber(ubDieByD1, uwD1Target.B.uwTarget, D1);
				M_AssertError(ubDieByD3 != ubDieByD1);
			}
		}
		else {
			ftlGetUserFreeBlock(&uwD1Target, 0, D1);
		}
		ftlEraseTarget(&uwD1Target, D1, FALSE);
		gubD1LinkTable[VT->guwGRTarget[0].B.uwTarget].B.ubD1Link = (UBYTE)uwD1Target.B.uwTarget;
		VT->guwGRTarget[0].B.btFastPage = 0;
		gulVC[VT->guwGRTarget[0].B.uwTarget].B.btFastPage = 0;
#if TLC
		gulVC[VT->guwGRTarget[0].B.uwTarget].B.btInD1Unit = 1;
#endif
	}

	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}

#endif
	if ((TLC == 0) && (VT->guwFreeBlockCount_D3 > GRUseMLCBlockThreshold)) {
		if (ENABLE_SLCPOOL) {
			if (VT->gulSLCPoolSize < (gubMaxZipRate * gulFastPage4kEntrysPerUnit)) { //not enough SLCPoolSize, which is (max L4k in a SLC unit)
				gulVC[VT->guwGRTarget[0].B.uwTarget].B.btFastPage = 0;
				VT->guwGRTarget[0].B.btFastPage = 0;
			}
			else {
				gulVC[VT->guwGRTarget[0].B.uwTarget].B.btFastPage = 1;
				VT->guwGRTarget[0].B.btFastPage = 1;
			}
		}
		else {
			gulVC[VT->guwGRTarget[0].B.uwTarget].B.btFastPage = 1;
			VT->guwGRTarget[0].B.btFastPage = 1;
		}
	}
	else {
		gulVC[VT->guwGRTarget[0].B.uwTarget].B.btFastPage = gubUltraMLCEnable;
		VT->guwGRTarget[0].B.btFastPage = gubUltraMLCEnable; // gulVC的fast page在get free block裡就清掉了
#if TLC
		gulVC[VT->guwGRTarget[0].B.uwTarget].B.btInD1Unit = 1;
#endif
	}

	if (BUFB_BASE[400] == 1) { // //不論'上面判斷選什麼,這裡都強制覆蓋為只使用SLC mode
		gulVC[VT->guwGRTarget[0].B.uwTarget].B.btFastPage = 1;
		VT->guwGRTarget[0].B.btFastPage = 1;
	}

	ftlEraseTarget(&VT->guwGRTarget[0], D3, FALSE);

#if MicronFlashOnly
	VT->gulGRTableStartIndex[0] = GRTable_Max_Collect_Num;
	VT->gulGRTableTempStartIndex = GRTable_Max_Collect_Num;
#else
	VT->gulGRTableStartIndex[0] = 4096;
	VT->gulGRTableTempStartIndex = 4096;
#endif

Mark_BuildTableUnitHeader:
	mDMAC_SV(1, (U32)gubTable_P2L_BitMap,  ((U32)gubTableUnitNum * guwTableP2LBitMapSize) / 8, 0, 0, 0, DMA_EN_SETCQ, 1);
	mSE_MSV(4, (U32)gulTable_L2P, (gulTotalL2PGroupCnt + TableTypeNum_ExceptL2P + gubHostTable_GroupNum) * 4, (TableNotInFlash | TableAllZero | SearchLinkDefault), 0, 0xFFFFFFFF, 0, 1);
	mSE_SV(2, (U32)guwTable_VC, MAX_TABLEUNIT * 2, 0, 0);
	mDMAC_SV(1,  (U32)&guoRSFrameBits[Table_RSFrameStartIndex * gub4kEntrysPerPlane * W_4KB_8BNUM],  (4096 << gub4kEntrysPerPlaneLog << 2), 0, 0, 0, DMA_EN_SETCQ, 1);
	UBYTE ubLength = (4 << gub4kEntrysPerPlaneLog);
	for (ubi = 0; ubi < (ubLength * 4); ubi++) {
		gulRSFrameSpareBits[Table_RSFrameStartIndex * gub4kEntrysPerPlane + ubi] = 0;
	}

	VT->gubTableTargetIndex = 0;
	VT->gubTableTargetSyncIndex = VT->gubTableTargetIndex;
	UBYTE ubHeaderFinishPlaneNum = 0;
	UBYTE ubECFinishPlane = 0;
	UBYTE ubVCFinishPlane = 0;
	UBYTE ubHostFinishPlane = 0;
	U32 ulSpareMark;
	// uli is PlanePtr
	uli = 0;
	ubParityPlaneIndex = Table_RSFrameStartIndex;
	while ((ubHeaderFinishPlaneNum < gubTableHeadInfoPlaneCnt) || (ubVCFinishPlane < gubPlanesPerVCTable) || (ubECFinishPlane < gubPlanesPerECTable) || (ubHostFinishPlane < gubHostTable_GroupNum) || VT->guwRSFrameCount[ubParityPlaneIndex]) {
		if (ENABLE_PLANE_CH_CE) {
			ubCEIndex = M_Modular((uli >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
		}
		else {
			ubCEIndex = (uli & gubPlanesPerBurstMask) + ((M_Modular((uli >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
		}
		FlashQueue_t *FQ;
		UBYTE ubDepth, ubL4kLink;
		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		ubParityPlaneIndex = Table_RSFrameStartIndex + ((M_Divider(uli, DEF_TableParityPlanes_Mcl_Div)) & Table_RS_Group_Mask);
		if (VT->guwRSFrameCount[ubParityPlaneIndex] == 0) {
			// avoid same channel double encode before prog parity
			// Preformat's FQ will wait result. Actually, we don't need to check done count?
			if (ENABLE_DEBUG_NOT_ENCODE_PARITY) {
				RSL[RSL_TAG_SEL] = ubParityPlaneIndex;
				while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubParityPlaneIndex);
				if (RSB[RSB_TAG_PEC]) {
					while ((guwRSFrameDoneCount[ubParityPlaneIndex] < 1) && ((RSB[RSB_TAG_CONFIG_0]&RS_TAG_PRF) == 0)) {
						M_SwitchTask();
					}
					RSL[RSL_TAG_VLD_CTL] |= RS_TAG_VALID_CLR;
				}
				guwRSFrameDoneCount[ubParityPlaneIndex] = 0;
			}
			else {
				while (guwRSFrameDoneCount[ubParityPlaneIndex] < 1) {
					M_SwitchTask();
				}
				guwRSFrameDoneCount[ubParityPlaneIndex] = 0;
			}
		}
		FQ->btCheckEmpty = 0;
		FQ->FQPara.B.btRWTableUnit = 1;
		FQ->uwFUnit = VT->guwTableTarget[VT->gubTableTargetIndex].B.uwTarget;
		FQ->ulFEntry = (uli << gub4kEntrysPerPlaneLog);
		FQ->ubFJob = BYTE_FJOB_WRITE;
		FQ->uwFJobInfo = ( BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_PARITY);
		FQ->ubRSFrameIndex = ubParityPlaneIndex;
		FQ->ubRSFrameReuseCount = gubftlRSFrameReuseCount[ubParityPlaneIndex];
		FQ->ubRelativeDepth = ubDepth;
		FQ->ubDepth = 0xFF;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btUNServed = 1;
		FQ->btD3 = 1;
		//RS MT setting
		FQ->btRSOneParityEn = 0;
		FQ->ubRSPageNum = VT->guwRSFrameCount[ubParityPlaneIndex];
		M_AssertError((VT->guwRSFrameCount[ubParityPlaneIndex] == guwTableParityPlanesNumMax));
		if (VT->guwRSFrameCount[ubParityPlaneIndex] == (guwTableParityPlanesNum - 2)) {
			FQ->btRSLastPage = 1;
		}
		else {
			FQ->btRSLastPage = 0;
		}

		if (ubHeaderFinishPlaneNum < gubTableHeadInfoPlaneCnt) {
		Mark_SaveHeader:
			FQ->ulRAMAddr = ((U32)gulTable_L2P) + ((512 << gubSectorsPerPlaneLog) * ubHeaderFinishPlaneNum);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)FQ->ulRAMAddr, ubL4KNum * 4096);
			}
			ulSpareMark = (MARK_TableHead | ubHeaderFinishPlaneNum);
			ulL4K_FW = 0;
			ubHeaderFinishPlaneNum ++;
		}
		else if (ubECFinishPlane < gubPlanesPerECTable) {
		Mark_SaveEC:
			if (((gubRUT_EC_set & BIT0) == 0) || (((gubRUT_EC_set & BIT0) == 1) && (ubKeepSMARTwhenKeepEC == 0))) {
				if (((gubRUT_EC_set & BIT0) == 1) && (ubKeepSMARTwhenKeepEC == 0)) {
					UWORD uwUnitIndex;
					U32 ulUnitEraseCnt;
					U32 ulMaxEraseCnt = 0;
					for (uwUnitIndex = 0; uwUnitIndex < guwFWTotalUnitNum; uwUnitIndex++) {
						ulUnitEraseCnt = gulEC_D3[uwUnitIndex].B.ulEraseCount;

						if (ulUnitEraseCnt > ulMaxEraseCnt) {
							ulMaxEraseCnt = ulUnitEraseCnt;
						}
					}
					VT->gulPreviousMaxEraseCnt_D3 += ulMaxEraseCnt;
				}
				mSE_SV(4, (U32)gulEC_D3, (gubPlanesPerECTable << gubSectorsPerPlaneLog) * 512, 0, 0);
				ftlSearchMixPlaneInFreeBlockTable(); //We search MixPlane Unit again when EC_D3 has earsed
			}
			FQ->ulRAMAddr = ((U32)gulEC_D3) + ((512 << gubSectorsPerPlaneLog) * ubECFinishPlane);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)FQ->ulRAMAddr, ubL4KNum * 4096);
			}
			ulSpareMark = (MARK_TableEC | ubECFinishPlane);
			ulL4K_FW = 0x112233;
			ubECFinishPlane ++;
		}
		else if (ubVCFinishPlane < gubPlanesPerVCTable) {
		Mark_SaveVC:
			FQ->ulRAMAddr = ((U32)gulVC) + ((512 << gubSectorsPerPlaneLog) * ubVCFinishPlane);
			ulSpareMark = (MARK_TableVC | ubVCFinishPlane);
			ulL4K_FW = gulSLCPoolSize_Sync;
			ubVCFinishPlane ++;
		}
		else if (ubHostFinishPlane < gubHostTable_GroupNum) {
		Mark_SaveHost:
			mDMAC_SV(4, (U32)gubL2PBase, 512 << gubSectorsPerPlaneLog, 0, 0, 0, DMA_EN_SETCQ, 1);

			if (ubHostFinishPlane == (HOSTTABLE_ATACFG_ADR >> gubSectorsPerPlaneLog)) {
				ftlInitAtaCfg();
				memcpy(&gubL2PBase[(HOSTTABLE_ATACFG_ADR % gubSectorsPerPlane) * 512], &AtaCfg, 512);
			}
			FQ->ulRAMAddr = (U32)gubL2PBase;
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)FQ->ulRAMAddr, ubL4KNum * 4096);
			}
			ulSpareMark = (MARK_TableHost | ubHostFinishPlane);
			ubHostFinishPlane ++;
		}
		else {
		Mark_SaveDummyForParity:
			mDMAC_SV(4, (U32)gubL2PBase, 512 << gubSectorsPerPlaneLog, 0, gulTable_Invalid, 0, DMA_EN_SETCQ, 1);
			FQ->ulRAMAddr = (U32)gubL2PBase;
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)FQ->ulRAMAddr, ubL4KNum * 4096);
			}
			ulSpareMark = MARK_TableDummy;
		}
		UBYTE ubL4KCount;
		for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
			//set L4K table info
			pL4KTable.ulL4K_LCA  = ulSpareMark;
			pL4KTable.Para0x04.BitMap.ulL4K_FW = ulL4K_FW;
			pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
			pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = MAX_ZCODE;
			pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			if ((ulSpareMark & MARK_TableVC) == MARK_TableVC) {
				pL4KTable.Para0x08.BitMap.ubL4K_FW2 = (ulL4K_FW >> 24);
			}
			else {
				pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0xAB;
			}
			pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512);
			pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			*pL4KTablePtr = pL4KTable;
		}
		gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
		gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
		gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
		gulPara0x24.BitMap.btZipBypass = 1;
		gulPara0x24.BitMap.btCmpEn = 0;
		gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
		gulPara0x24.BitMap.btBufModeEn = 0;

		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
		VT->guwRSFrameCount[ubParityPlaneIndex] ++;
		while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

		uli++;

		if (VT->guwRSFrameCount[ubParityPlaneIndex] == guwTableParityPlanesNumMax) {
		Mark_SaveTableParity:
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((uli >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (uli & gubPlanesPerBurstMask) + ((M_Modular((uli >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}
			FlashQueue_t *FQ;
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->btCheckEmpty = 0;
			FQ->FQPara.B.btRWTableUnit = 1;
			FQ->btParity = 1;
			FQ->uwFUnit = VT->guwTableTarget[VT->gubTableTargetIndex].B.uwTarget;
			FQ->ulFEntry = (uli << gub4kEntrysPerPlaneLog);
			FQ->ubFJob = BYTE_FJOB_WRITE;
			FQ->uwFJobInfo = ( BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT);
			FQ->ubRSFrameIndex = ubParityPlaneIndex;
			FQ->ubRelativeDepth = ubDepth;
			FQ->ubDepth = 0xFF;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btUNServed = 1;
			FQ->btD3 = 1;
			//RS MT setting
			FQ->btRSOneParityEn = 1;
			FQ->ubRSPageNum = 0;
			FQ->btRSLastPage = 0;

			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize);
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btBufModeEn = 0;

			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			if (ENABLE_DEBUG_NOT_ENCODE_PARITY) {
				UBYTE ubL4KCount;
				L4KTable16B *pL4KTablePtr;
				L4KTable16B pL4KTable;
				for (ubL4KCount = 0; ubL4KCount < gub4kEntrysPerPlane; ubL4KCount++) {
					pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
					//set L4K table info
					pL4KTable.Para0x04.BitMap.ulL4K_FW = 0;
					pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
					pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
					pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0;
					pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0x0;
					pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = MAX_ZCODE;
					pL4KTable.Para0x0C.BitMap.ulL4K_BADR =  ((U32)((&guoRSFrameBits[(ubParityPlaneIndex * 32 * 1024) / 8]))) / 512;
					pL4KTable.ulL4K_LCA = gulTable_Parity << SectorsPer4KLog;
					*pL4KTablePtr = pL4KTable;
				}
			}

			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
			while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}
			FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

			uli++;
			VT->guwRSFrameCount[ubParityPlaneIndex] = 0;
			gubftlRSFrameReuseCount[ubParityPlaneIndex]++;
		}
	}
	VT->guwTableTargetPTR = uli;
	VT->guwTableTargetSyncPTR = VT->guwTableTargetPTR;

Mark_ProgramVTUnit:
	if (((gubRUT_EC_set & BIT0) == 0) || (ubKeepSMARTwhenKeepEC == 0)) {
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

#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
		VT->guwRemainVCOldGRTargetCnt = 0;
		VT->gulBunchRSRecoverEnterCnt = 0;
		VT->gulBunchRSRecoverSuccessCnt = 0;
		VT->gulBunchRSRecoverFailErrorPageOver2Cnt = 0;
#if ENABLE_DEBUG_BUNCH_DUMMY_FLOW==1
		VT->gulBunchDummyFlowCnt = 0;
		VT->gulCleanGRInMiddleWhenGCDoingCnt = 0;
#endif
#endif

		VT->gulReadCRCCnt = 0;
		VT->gulWriteCRCCnt = 0;
		VT->gulHostReadECCCnt = 0;

#if Read_Chk_ByHostRead
		VT->guoD3UnitTotalReadCnt = 0;
		VT->guoD1UnitTotalReadCnt = 0;
#endif
	}

	if ((gubRUT_EC_set & BIT0) == 0) {
		VT->gulTotalEraseCount = 0;
	}

	for (ubi = 0; ubi < 4; ubi++) {
		VT->guwGCGRTarget[ubi].B.uwTarget = Default_Unit;
	}
	VT->guwCopyTarget.B.uwTarget = Default_Unit;
	VT->gulFTLState.B.btFlushCache = 1;
	VT->guwVTPTR += (gubPlanesPerVT << ENABLE_VT_BACKUP);
	VT->guwReadMoveD1Target.B.uwTarget = Default_D1_Unit;

	if (BURNER) {
		VT->gubDramType = gubDram_Type;
		VT->gubDramSize = gubDram_Size;
		VT->guwDDR_DQS_FINE = guwDQS0_Fine;
		VT->gubDDR_DQS_G = gubDQS0_G;
		VT->gubDram_Bank = gubDram_Bank;
		VT->gubSDR_Corase = gub_SDR_Corse;
		VT->guwSDR_Fine = guw_SDR_Fine;
	}
	else {	//// For FW-doing Preformat, SDR as default
		VT->gubDramType = 3;/// SDR
		VT->gubDramSize = 3; /// 32MB
		VT->gubDram_Bank = 0;
		VT->guwDDR_DQS_FINE = 0;
		VT->gubDDR_DQS_G = 0;
		VT->gubSDR_Corase = 2; ///For EV-Board
		VT->guwSDR_Fine = 224; ///For EV-Board
	}
	VT->guoUnitTimeLineGCGR = 1;

	//1 Program to flash
	ubPlaneBank = 0;
	for (ubj = 0; ubj < (1 << ENABLE_VT_BACKUP); ubj ++) {
		ubi = 0;
		while (ubi < gubPlanesPerVT) { // VT+V2P is 16KB
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + ((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}
			if (ftlWriteSinglePlane(guwVTTarget.B.uwTarget, (ubj << gub4kEntrysPerPlaneLog),  ubCEIndex, ((U32)VT + ((512 << gubSectorsPerPlaneLog) * ubi)), MARK_VT, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT), ubL4KNum, 1, 0)) {
				SetFailCEtoSystemInfo(ubCEIndex);
				ftlPreformatShowError(ubCEIndex, guwVTTarget.B.uwTarget, 0x12);
				return 1;
			}
			ubi++;
			ubPlaneBank = M_Modular((ubPlaneBank + 1), DEF_PlanesPerPage_Mod_Mask);
		}
	}
	//1 Read for verification...
	// ubi is how many planes read
	ubPlaneBank = 0;
	for (ubj = 0; ubj < (1 << ENABLE_VT_BACKUP); ubj ++) {
		ubi = 0;
		while (ubi < gubPlanesPerVT) { // VT+V2P is 16KB
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
			}
			else {
				ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + ((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
			}
			FlashQueue_t *FQ;
			UBYTE ubDepth, ubL4kLink;
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->btCheckEmpty = 0;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_RETRY);
			FQ->ulRAMAddr = ((U32)&gubCopyBuffer[0]);
			FQ->uwFUnit = guwVTTarget.B.uwTarget;
			FQ->ulFEntry = (ubj << gub4kEntrysPerPlaneLog);
			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
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
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0x00;
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
			if ((pL4KTablePtr->ulL4K_LCA != MARK_VT) || FQ->btReadUNC ) {
				SetFailCEtoSystemInfo(ubCEIndex);
				ftlPreformatShowError(ubCEIndex, guwVTTarget.B.uwTarget, 0x24);
				return 1;
			}

			ubi++;
			ubPlaneBank = M_Modular((ubPlaneBank + 1), DEF_PlanesPerPage_Mod_Mask);
		}
	}

Mark_BuildSystemUnit:
	mDMAC_SV(1, (U32)gubL2PBase,  (U32)gubSectorsPerPlane * 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	memcpy(&gubL2PBase[M_SysInfo_HW_Info], "PS3111", 6);
	gubL2PBase[M_SysInfo_Separation] = 'I';
	gubL2PBase[M_SysInfo_Year1] = sFWVerStr[7];
	gubL2PBase[M_SysInfo_Year2] = sFWVerStr[8];
	gubL2PBase[M_SysInfo_Year3] = sFWVerStr[9];
	gubL2PBase[M_SysInfo_Year4] = sFWVerStr[10];
	gubL2PBase[M_SysInfo_Month1] = sFWVerStr[0];
	gubL2PBase[M_SysInfo_Month2] = sFWVerStr[1];
	gubL2PBase[M_SysInfo_Month3] = sFWVerStr[2];
	gubL2PBase[M_SysInfo_Day1] = sFWVerStr[4];
	gubL2PBase[M_SysInfo_Day2] = sFWVerStr[5];
	gubL2PBase[M_SysInfo_FW_Info1] = FW_REVISION1;
	gubL2PBase[M_SysInfo_FW_Info2] = FW_REVISION2;
	gubL2PBase[M_SysInfo_FW_Info3] = FW_REVISION3;
	gubL2PBase[M_SysInfo_FW_Info4] = FW_REVISION4;
	gubL2PBase[M_SysInfo_FW_Info5] = FW_REVISION5;
	gubL2PBase[M_SysInfo_FW_Info6] = '-';
	gubL2PBase[M_SysInfo_FW_Info7] = FW_INTERNALVERSION1;
	gubL2PBase[M_SysInfo_FW_Info8] = FW_INTERNALVERSION2;

	memcpy((void *)&gubL2PBase[M_SysInfo_LastCodeVersion], (void *)&gubLastFWVer[0], 8);

	gubL2PBase[M_SysInfo_gubSaveVTSerialNumber] = gubSaveVTSerialNumber;

	gubL2PBase[M_SysInfo_PreformatCENum] = gubCENumber;
	gubL2PBase[M_SysInfo_gubDram_Size] = gubDram_Size;
	gubL2PBase[M_SysInfo_gubNoPWIC] = CheckPWICExist();
	gubL2PBase[M_SysInfo_gubTableVersion] = Table_version;
	gubL2PBase[M_SysInfo_DBT_SET] = gubDBTSet;
	gubL2PBase[M_SysInfo_gubDPSTableValid] = gubDPSTableValid;

	if (ENABLE_DEBUG_UART) {
		UartString("PWIC");
		Uart_Tx_DataHex(gubL2PBase[M_SysInfo_gubNoPWIC]);
	}

	if (1) {
		UWORD *uwCopyBuffer = (UWORD *)(&gubL2PBase[0]);
		U32 *ulCopyBuffer = (U32 *)(&gubL2PBase[0]);
		uwCopyBuffer[(M_SysInfo_SystemAreaNumBlock_CH0 / 2)] = guwSystemAreaNum[0];
		uwCopyBuffer[(M_SysInfo_SystemAreaNumBlock_CH1 / 2)] = guwSystemAreaNum[1];

		if (gubTransformSystemUnitToSystemSet) {
			gubL2PBase[M_SysInfo_SystemArea_RemappingRule] = Rule_UnitPlaneA;//一定走舊rule
		}
		else {
			gubL2PBase[M_SysInfo_SystemArea_RemappingRule] = gubSystemArea_RemappingRule;//原值填回去
		}

		for (ubi = 0; ubi < SYSTEM_SETS; ubi++) { //RemappingTable
			for (ubj = 0; ubj < MAX_CHANNEL; ubj++) {
				uwCopyBuffer[(M_SysInfo_SystemBlockRemapping_Set0_CH0 / 2) + (ubi * MAX_CHANNEL) + ubj] = guwSystemBlockRemappingTable[ubi][ubj];
			}
		}

		uwCopyBuffer[(M_SysInfo_FWUnitNum / 2)] = guwFWTotalUnitNum;
		uwCopyBuffer[(M_SysInfo_uwSDR_window_min / 2)] = guw_SDR_window_min;
		uwCopyBuffer[(M_SysInfo_uwSDR_window_max / 2)] = guw_SDR_window_max;
		ulCopyBuffer[(M_SysInfo_NativeSize / 4)] = gulNativeSize;
		uwCopyBuffer[(M_SysInfo_VTAreaTarget / 2) + 0] = VT->guwVTAreaFreeTarget[0].B.uwTarget;
		uwCopyBuffer[(M_SysInfo_VTAreaTarget / 2) + 1] = VT->guwVTAreaFreeTarget[1].B.uwTarget;
		uwCopyBuffer[(M_SysInfo_VTAreaTarget / 2) + 2] = VT->guwVTAreaFreeTarget[2].B.uwTarget;
		uwCopyBuffer[(M_SysInfo_VTAreaTarget / 2) + 3] = VT->guwVTAreaFreeTarget[3].B.uwTarget;
		gulNeededUnitNum = ulNeededUnitNum + SYSTEM_SETS + (ReserveUnit << gubSelTotalGroupsLog);
		ulCopyBuffer[(M_SysInfo_ulNeededUnitNum / 4)] = gulNeededUnitNum;
		uwCopyBuffer[(M_SysInfo_uwInitialSpareBlocks / 2)] = GetMinRemainingSpareBlocksPerPlane();
		for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
			if (ENABLE_PLANE_CH_CE) {
				ubCEIndex = M_Modular((ubPlaneBank >> gubBurstsPerBankLog), DEF_CEPerPage_Mod_Mask);
				uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 0] =
				    M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[0].B.uwTarget << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask));
				uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 1] =
				    M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[1].B.uwTarget << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask));
				uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 2] =
				    M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[2].B.uwTarget << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask));
				uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 3] =
				    M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[3].B.uwTarget << gubBurstsPerBankLog) + (ubPlaneBank & gubBurstsPerBankMask));
			}
			else {
				ubCEIndex = (ubPlaneBank & gubPlanesPerBurstMask) + ((M_Modular((ubPlaneBank >> gubPlanesPerBankLog), DEF_BankPerPage_Mod_Mask)) << gubPlanesPerBurstLog);
				uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 0] =
				    M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[0].B.uwTarget << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog)&gubBurstsPerBankMask));
				uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 1] =
				    M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[1].B.uwTarget << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog)&gubBurstsPerBankMask));
				uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 2] =
				    M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[2].B.uwTarget << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog)&gubBurstsPerBankMask));
				uwCopyBuffer[(M_SysInfo_VTArea / 2) + (ubPlaneBank * 4) + 3] =
				    M_RUT2DTo1D(ubCEIndex, (VT->guwVTAreaFreeTarget[3].B.uwTarget << gubBurstsPerBankLog) + ((ubPlaneBank >> gubPlanesPerBurstLog)&gubBurstsPerBankMask));
			}
		}
		//		memcpy((void*)&gubL2PBase[M_SMART_setting], &BUFB_BASE[512], (sizeof(MP_SMART_Attribute_Entry) * AttributeTotalEntry));

		//		memcpy((void*)&gubL2PBase[M_SysInfo_InfoBlk], &BUFB_BASE[0], sizeof(SYSTEM_GROUP_t));

		if ((gubParameterPage[0] == 0x4A) && (gubParameterPage[1] == 0x45) && (gubParameterPage[2] == 0x53) && (gubParameterPage[3] == 0x44)) {
			uwCopyBuffer[M_SysInfo_ParameterPageVaild >> 1] = 1;
			memcpy( &uwCopyBuffer[M_SysInfo_ParameterPage >> 1], &gubParameterPage[0], sizeof(gubParameterPage));
		}
#if TLC
		else if ((gubParameterPage[0] == 0x54) && (gubParameterPage[1] == 0x53) && (gubParameterPage[2] == 0x42) && (gubParameterPage[3] == 0x4E) ) { // for Emulator
			uwCopyBuffer[M_SysInfo_ParameterPageVaild >> 1] = 1;
			memcpy( &uwCopyBuffer[M_SysInfo_ParameterPage >> 1], &gubParameterPage[0], sizeof(gubParameterPage));
		}
#endif
		memcpy(&gubL2PBase[M_SysInfo_InfoBlk], &BUFB_BASE[0], 512);
		memcpy(&gubL2PBase[M_SysInfo_SmartMPBlk], &BUFB_BASE[512], 512);
		memcpy(&gubL2PBase[M_SysInfo_RetryTable], &BUFB_BASE[1024], 512);
		memcpy(&gubL2PBase[M_SysInfo_DPSTable], &BUFB_BASE[1536], 2048);
	}


	//memcpy(&gubL2PBase[M_SysInfo_InfoBlk_from_host], &BUFB_BASE[0], 1024);

	ulDMAAddr = (U32)gubL2PBase;
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ulDMAAddr, ubL4KNum * 4096);
	}

	U32 ulLCA_Temp;
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		ulLCA_Temp = MARK_SYS;

		if (ftlWriteSinglePlane((guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), 0, ubCEIndex, ulDMAAddr, ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ((UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask)))) {
			SetFailCEtoSystemInfo(ubCEIndex);
			ftlPreformatShowError(ubCEIndex, (guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), 0x12);				//return 1;
			return 1;
		}
	}

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		mDMAC_SV(1, (U32)gubL2PBase,  (U32)gubSectorsPerPlane * 512, 0, 0, 0, DMA_EN_SETCQ, 1);

		for (uli = 1; uli < 3 ; uli ++) {

			ulLCA_Temp = MARK_DPST;
			if (ftlWriteSinglePlane((guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), uli, ubCEIndex, ulDMAAddr, ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ((UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask)))) {
				SetFailCEtoSystemInfo(ubCEIndex);
				ftlPreformatShowError(ubCEIndex, (guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), 0x12);				//return 1;
				return 1;
			}

		}
#if Hynix
		//mDMAC_SV(1, (U32)gubL2PBase,  (U32)gubSectorsPerPlane * 512, 0, 0, 0, DMA_EN_SETCQ, 1);
		mDMAC_SV(1, (U32)gubL2PBase,  (U32)32 * 1024, 0, 0, 0, DMA_EN_SETCQ, 1);//2plane Save RR Table
		gpsHynixRetryMeta = (HynixRetryMeta *) (gubL2PBase + 0x0100);
		gpubSlcRetryTable = (UBYTE *) (gubL2PBase + 0x0400);
#if (Hynix3DV6 || Hynix3DV5)
		gpubXlcRetryTable = (UBYTE *) (gubL2PBase + 0x1007); // RRT table 0
#else
		gpubXlcRetryTable = (UBYTE *) (gubL2PBase + 0x1000);
#endif
		gsHynixRetryInfo.ulSlcRetryTblBase = (U32) gpubSlcRetryTable;
		gsHynixRetryInfo.ulXlcRetryTblBase = (U32) gpubXlcRetryTable;

		if (flaRetrieveRetryTable(gulCeEnBitMap)) {
			ftlPreformatShowError(0x99, (guwSystemBlock[0] >> gubBurstsPerBankLog), 0x12);
			return 1;
		}

		memcpy((void *)gubL2PBase, &gsHynixRetryInfo, sizeof(HynixRetryInfo));//32B
#if (ENABLE_DCACHE)
		xthal_dcache_region_writeback_inv((void *)ulDMAAddr, ubL4KNum * 4096);
#endif
		ulLCA_Temp = MARK_SYS_HYNIX_RRT;
		for (uli = 3; uli < 5 ; uli ++) {
			if (ftlWriteSinglePlane((guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), uli, ubCEIndex, (ulDMAAddr + ((uli - 3) * 16384)), ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ((UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask)))) {
				SetFailCEtoSystemInfo(ubCEIndex);
				ftlPreformatShowError(ubCEIndex, (guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), 0x12);				//return 1;
				return 1;
			}
		}
		for (uli = 5 ; uli <  guwFastPagePagesPerUnit; uli ++) {
			ulLCA_Temp = MARK_SYS;
			if (ftlWriteSinglePlane((guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), uli, ubCEIndex, ulDMAAddr, ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ((UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask)))) {
				SetFailCEtoSystemInfo(ubCEIndex);
				ftlPreformatShowError(ubCEIndex, (guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), 0x12); 			 //return 1;
				return 1;
			}
		}
#else

		for (uli = 3 ; uli <  guwFastPagePagesPerUnit; uli ++) {
			ulLCA_Temp = MARK_SYS;
			if (ftlWriteSinglePlane((guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), uli, ubCEIndex, ulDMAAddr, ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ((UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask)))) {
				SetFailCEtoSystemInfo(ubCEIndex);
				ftlPreformatShowError(ubCEIndex, (guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), 0x12);				//return 1;
				return 1;
			}

		}
#endif
	}
	//1 Read for verification...
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		for (uli = 0; uli <  guwFastPagePagesPerUnit; uli ++) {
			if ((uli == 1) || (uli == 2)) {
				ulLCA_Temp = MARK_DPST;
			}
#if Hynix
			else if ((uli == 3) || (uli == 4)) {
				ulLCA_Temp = MARK_SYS_HYNIX_RRT;
			}
#endif
			else {
				ulLCA_Temp = MARK_SYS;
			}
			if (ftlReadSinglePlane((guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), uli, ubCEIndex, (U32)&gubCopyBuffer[0], ulLCA_Temp,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY), 4, 0, ((UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask)))) {
				SetFailCEtoSystemInfo(ubCEIndex);
				ftlPreformatShowError(ubCEIndex, (guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog), 0x24);				//return 1;
				return 1;
			}
		}
	}
	//#endif
	return 0;
}

#if RDT_BURNER
UBYTE BuildDBTinRAM(U32 ulSramAddr)
{
	UBYTE *ubDBTBuffer = (UBYTE *)ulSramAddr;
	UBYTE *ubTempBuffer = (UBYTE *)((&gulVenderSMARTBuffer[0]) + 0x8000);
	SYSTEM_GROUP_t *InfoArray = (SYSTEM_GROUP_t *)&BUFB_BASE[0];

	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	U32 ulFreeblock, ulTotalBlockNum, uli, ulj, ulUnitPTR;
	U32 ulUnitsPerGroup = gulTotalPhyUnitNum >> gubTotalGroupsLog;
	UBYTE ubCEGroup = gubTotalGroups >> gubDieNumberLog;

	UBYTE ubDie, ubGroup, ubPlaneBank, ubCEIndex;

	if (RDT_FindDBTBlock_Task()) {
		gubRDTDone = DBT_Checked;	// not found
	}
	else {
		gubRDTDone = (DBT_Checked | DBT_Exist);	// DBT found
	}

	mDMAC_SV(1, (U32)ubDBTBuffer, ulEntryPerDBTPlane * 32, 0, 0xFFFFFFFF, 0, DMA_EN_SETCQ, 1);		// 假設最大16CE, 4 plane, 8192 unit, 16*4*8192/16K = 32, 留ulEntryPerDBTPlane*32 = 512K 一定夠放DBT

	if (gubRDTDone & DBT_Exist) {//把DBT拉上RAM
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
			if ((ulTotalBlockNum - ulFreeblock) > ulEntryPerDBTPlane) {
				ulj = ulEntryPerDBTPlane;
			}
			else {
				ulj = (ulTotalBlockNum - ulFreeblock);
			}
			for (uli = ulFreeblock; uli < (ulFreeblock + ulj); uli++) {	// change later bad mark to early bad mark
				if (ubDBTBuffer[uli] & 0x30) {
					ubDBTBuffer[uli] = BIT7 | (ubDBTBuffer[uli] & 0x0F);
				}
				else {	//Mark EraseFail accoding to gubEraseFailBitMap
					if (gubEraseFailBitMap[uli >> 3] & (BIT0 << (uli & 7))) {
						if (InfoArray->ubRecordEraseFail_EarlyOrLater == 1) {
							ubDBTBuffer[uli] = 0x84;
						}
						else {
							ubDBTBuffer[uli] = 0x34;
						}
					}
				}
			}
			ulFreeblock += ulEntryPerDBTPlane;
		}
	}
	else {//DBT不在就把它建起來
		ulj = 0;
		// build DBT by copybuffer
		for (uli = 0; uli < ulUnitsPerGroup; uli++) {
			for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
				for (ubGroup = 0; ubGroup < ubCEGroup; ubGroup++) {
					ulUnitPTR = (ubGroup * gubDieNumber + ubDie) * ulUnitsPerGroup + uli;
					for (ubPlaneBank = 0; ubPlaneBank < gubPlanesPerPage; ubPlaneBank++) {
						if (ENABLE_SANDISK_CHECKEARLYBADBYPROG) {
							ubDBTBuffer[ulj] = ftlGoodBlock_SandiskCheckByProgFail(ulUnitPTR, ubPlaneBank);
						}
						else {
#if TLC
#if B0KB
							ubDBTBuffer[ulj] = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2, GOODBLK_MLC);
#else
							ubDBTBuffer[ulj] = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 2);
#endif
#else
#if MicronFlashOnly
							ubDBTBuffer[ulj] = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1, GOODBLK_MLC);
#else
							ubDBTBuffer[ulj] = ftlGoodBlock(ulUnitPTR, ubPlaneBank, (U32)ubTempBuffer, 1);
#endif
#endif
						}
						//Mark EraseFail accoding to gubEraseFailBitMap
						if (gubEraseFailBitMap[ulj >> 3] & (BIT0 << (ulj & 7))) {
							//UartString("\nMark Erase Fail in Built DBT");
							if (InfoArray->ubRecordEraseFail_EarlyOrLater == 1) {
								ubDBTBuffer[ulj] = 0x84;
							}
							else {
								ubDBTBuffer[ulj] = 0x34;
							}
						}
						ulj++;
					}
				}
			}
		}
	}
	return NORMAL;
}

/*UBYTE RDTBurner_A2_EWR_Test(U32 ulUnit, UBYTE ubBurst, UBYTE ubCEIndex)
{
	UBYTE *ubTempBuffer = (UBYTE *)((&gulVenderSMARTBuffer[0]) + 0x8000);
	U32 ulForValue = (((ulUnit & 0x0000FFFF) << 16 ) | ((UWORD)(ubBurst & 0xFF) << 8));
	UWORD uwi;

	if (ftl_Erase_Single_Plane(ulUnit, ubBurst, ubCEIndex, BIT_FJOBI_FASTPAGE)) {
		return DBT_MARK_ERASE_FAIL;
	}
	for (uwi = 0; uwi < guwFastPagePagesPerUnit; uwi++) {
		mDMAC_SV( 4, (U32)ubTempBuffer, gub4kEntrysPerPlane * 0x1000, ulForValue, 0, 0, (DMA_EN_RAND | DMA_EN_SETCQ), 1);
		if (ftlWriteSinglePlane(ulUnit, uwi, ubCEIndex, (U32)&ubTempBuffer[0], 0x12345678, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubBurst)) {
			return DBT_MARK_WRITE_FAIL;
		}
		if (ftlReadSinglePlane(ulUnit, uwi, ubCEIndex, (U32)&ubTempBuffer[0], 0x12345678,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS ), 4, 0, ubBurst)) {
			return DBT_MARK_READ_FAIL;
		}
	}
	return NORMAL;
}*/

UBYTE ScanRDTSystemArea(U32 ulSramAddr)
{
	U32 ulFreeSystemUnitPTR, ulFreeblock, ulStartUnit[MAX_CHANNEL];
	UBYTE ubSYSRemappingSetPtr = 0;
	UBYTE ubBurst;
	UBYTE *ubDBTBuffer = (UBYTE *)ulSramAddr;
	UBYTE ubSystemUnitLimit[MAX_CHANNEL];
	UBYTE ubDBTSetCheck = 0;
	UBYTE ubbadblock = 0;
	SYSTEM_GROUP_t *InfoArray = (SYSTEM_GROUP_t *)&BUFB_BASE[0];

	UBYTE ubi, ubj, ubCEIndex, ubGetFreeBlock, ubStartBurst[MAX_CHANNEL];

	//initial SystemArea相關變數
	gubSystemSetFreeCount = 0;
	gubSystemSetFreeQueueInPTR = 0;
	gubSystemSetFreeQueueOutPTR = 0;
	for (ubi = 0; ubi < SYSTEM_SETS; ubi++) {
		for (ubj = 0; ubj < MAX_CHANNEL; ubj++) {
			guwSystemBlockRemappingTable[ubi][ubj] = 0xFFFF;
		}
		gubSystemSetFreeTable[ubi] = 0xFF;
	}

	//在沒有保留RUT和SystemArea界線的情況下, MP可以由infoBlk指定SystemRule
	if (InfoArray->ubSystemAreaRule_ByAP != 0) { //MP由infoblk指定SystemRule
		if (InfoArray->ubSystemAreaRule_ByAP == 1) {
			gubSystemArea_RemappingRule = Rule_UnitPlaneA;
		}
		else if (InfoArray->ubSystemAreaRule_ByAP == 2) {
			gubSystemArea_RemappingRule = Rule_Set;
		}
		else {//參數錯誤
			ftlPreformatShowError(0, 0, 0x11);
			return TERMINAL_FAIL;
		}
	}
	else {//Burner自己判斷
	}

	if (RDT_FindDBTBlock_Task()) {
		gubRDTDone = DBT_Checked;	// not found
		//UartString("\nNoDBTinPreformat");
	}
	else {
		gubRDTDone = (DBT_Checked | DBT_Exist);	// DBT found
		//UartString("\nDBTinPreformat");
	}

	if (gubRDTDone & DBT_Exist) {
		if (HandleDBTSetLocation((U32)&ubDBTBuffer[0], Mode_ByDBTinRAM)) {
			ftlPreformatShowError(SystemArea_Error_DBTSet, gubDBTSet, 0x84);
			return TERMINAL_FAIL;
		}
	}

	for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
		ulStartUnit[ubCEIndex] = 0;
		ubStartBurst[ubCEIndex] = 0;
		ubSystemUnitLimit[ubCEIndex] = SYSTEMAREA_DEFAULT_SCAN_RANGE;
	}

	while (1) {
		if (ENABLE_DEBUG_UART_PREFORMAT) {
			UartString("\nSet:");
			Uart_Tx_DataHex(ubSYSRemappingSetPtr);
		}
		if (gubSystemArea_RemappingRule == Rule_UnitPlaneA) { //依照舊rule只挑planeA的systemUnit
			ubBurst = 0;//只挑planeA,兩個channel的起點終點相同
			for (ulFreeSystemUnitPTR = ulStartUnit[0] ; ulFreeSystemUnitPTR < ubSystemUnitLimit[0]; ulFreeSystemUnitPTR++) {
				ubbadblock = 0;
				ulFreeblock = (M_Multiplier((ulFreeSystemUnitPTR << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + ubBurst;

				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (ENABLE_PLANE_CH_CE) {
						if (ubDBTBuffer[ulFreeblock + (ubCEIndex << gubBurstsPerBankLog)] & 0xF0) {
							ubbadblock |= 1;
						}
					}
					else {
						if (ubDBTBuffer[ulFreeblock + ubCEIndex] & 0xF0) {
							ubbadblock |= 1;
						}
					}
					/*if (ubbadblock == 0) {
						if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) != ((U32)guwDBTBlock[ubCEIndex])) {//不是DBT才去做測試
							ubbadblock = RDTBurner_A2_EWR_Test(ulFreeSystemUnitPTR, ubBurst, ubCEIndex);
							if (ubbadblock != 0) {
								if (ENABLE_PLANE_CH_CE) {
									ubDBTBuffer[ulFreeblock + (ubCEIndex << gubBurstsPerBankLog)] = (RDT_BURNER_MARKBAD | ubbadblock);
								}
								else {
									ubDBTBuffer[ulFreeblock + ubCEIndex] = (RDT_BURNER_MARKBAD | ubbadblock);
								}
							}
						}
					}*/
				}

				if (ubbadblock == 0) {//挑到了兩個planeA同時為goodblk的Unit,放入remapping table
					if (ENABLE_DEBUG_UART_PREFORMAT) {
						UartString("\nBlock:");
						UartLongHex((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst);
					}

					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][ubCEIndex] = (ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst;
					}

					for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
						if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) != ((U32)guwDBTBlock[ubCEIndex])) {//不是DBT(system unit也會進這個case去erase掉)
							ftl_Erase_Single_Plane(ulFreeSystemUnitPTR, ubBurst, ubCEIndex, BIT_FJOBI_FASTPAGE);
						}
						else { //It is DBT, not erase it
							ubDBTSetCheck |= (BIT0 << ubCEIndex);
						}
					}
					//把目前掃到的進度保留下來
					ulStartUnit[0] = ulFreeSystemUnitPTR + 1;
					break;
				}
			}
			if (ulFreeSystemUnitPTR == ubSystemUnitLimit[0]) {
				ftlPreformatShowError(0, ulFreeSystemUnitPTR << gubBurstsPerBankLog, 0x80);
				//因為是Old PlaneA Rule 兩個CE都同時到達界線
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				return TERMINAL_FAIL;
			}
		}
		else if (gubSystemArea_RemappingRule == Rule_Set) { //plane A,B,C,D都可以挑
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				ubGetFreeBlock = 0;
				for (ulFreeSystemUnitPTR = ulStartUnit[ubCEIndex] ; ulFreeSystemUnitPTR < ubSystemUnitLimit[ubCEIndex]; ulFreeSystemUnitPTR++) {
					for (ubBurst = ubStartBurst[ubCEIndex]; ubBurst < gubBurstsPerBank; ubBurst++) {
						ubbadblock = 0;
						ulFreeblock = (M_Multiplier((ulFreeSystemUnitPTR << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div)) + ubBurst;

						if (ENABLE_PLANE_CH_CE) {
							if (ubDBTBuffer[ulFreeblock + (ubCEIndex << gubBurstsPerBankLog)] & 0xF0) {
								ubbadblock |= 1;
							}
						}
						else {
							if (ubDBTBuffer[ulFreeblock + ubCEIndex] & 0xF0) {
								ubbadblock |= 1;
							}
						}

						/*if (ubbadblock == 0) {
							if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) != (U32)guwDBTBlock[ubCEIndex]) {//不是DBT才去做測試
								ubbadblock = RDTBurner_A2_EWR_Test(ulFreeSystemUnitPTR, ubBurst, ubCEIndex);
								if (ubbadblock != 0) {
									if (ENABLE_PLANE_CH_CE) {
										ubDBTBuffer[ulFreeblock + (ubCEIndex << gubBurstsPerBankLog)] = (RDT_BURNER_MARKBAD | ubbadblock);
									}
									else {
										ubDBTBuffer[ulFreeblock + ubCEIndex] = (RDT_BURNER_MARKBAD | ubbadblock);
									}
								}
							}
						}*/

						if (ubbadblock == 0) {//挑到了,放入remapping table
							if (ENABLE_DEBUG_UART_PREFORMAT) {
								if (ubCEIndex == 0)
									UartString("\n");
								UartString(" CE:");
								Uart_Tx_DataHex(ubCEIndex);
								UartString(" Block:");
								UartLongHex((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst);
							}
							guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][ubCEIndex] = (ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst;

							if (((ulFreeSystemUnitPTR << gubBurstsPerBankLog) + ubBurst) != ((U32)guwDBTBlock[ubCEIndex])) {//不是DBT(system unit也會進這個case去erase掉)
								ftl_Erase_Single_Plane(ulFreeSystemUnitPTR, ubBurst, ubCEIndex, BIT_FJOBI_FASTPAGE);
							}
							else { //It is DBT, not erase it
								ubDBTSetCheck |= (BIT0 << ubCEIndex);
							}

							//把這個CE目前掃到的進度保留下來
							if (ubBurst == gubBurstsPerBank - 1) {
								ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR + 1;
								ubStartBurst[ubCEIndex] = 0;
							}
							else {
								ulStartUnit[ubCEIndex] = ulFreeSystemUnitPTR;
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
				if (ulFreeSystemUnitPTR == ubSystemUnitLimit[ubCEIndex]) {
					ftlPreformatShowError(ubCEIndex, ulFreeSystemUnitPTR << gubBurstsPerBankLog, 0x80);
					SetFailCEtoSystemInfo(ubCEIndex);
					return TERMINAL_FAIL;
				}
			}
		}
		//兩個CE都挑到了,湊成一組Set
		//sys必寫在set_4, DBT預計寫在Set_7, 如果已經在set_7就重新從Queue拿, 如果DBT在set_4會先把它搬到set_7再寫sys到set_4
		if (ubSYSRemappingSetPtr == (SYSTEM_SETS - 1)) {
			if (ubSYSRemappingSetPtr == gubDBTSet) {
			}
			else {
				ftlAddToSystemSetFreeTable(ubSYSRemappingSetPtr);
			}
			break;//跳出while(1)
		}
		else {
			if ((ubSYSRemappingSetPtr == (SYSTEM_SETS - 4))) {//挑它為systemSet
				if (ubDBTSetCheck != 0) {//如果這個SET已經放DBT了,會先把DBT搬走,再寫入SystemSet,進這個if只是為了把ubDBTSetCheck清成0
					if (ubSYSRemappingSetPtr != gubDBTSet) { //DBTSet編號不對(會進這個case多半是preformat bug)
						ftlPreformatShowError(SystemArea_Error_DBTSet, ubSYSRemappingSetPtr, 0x85);
						return 1;
					}
					ubDBTSetCheck = 0;
				}
				guwSystemBlock[0] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][0];
				guwSystemBlock[1] = guwSystemBlockRemappingTable[ubSYSRemappingSetPtr][1];
				gubSystemSet = ubSYSRemappingSetPtr;
			}
			else {
				if (ubDBTSetCheck != 0) {
					if (ubSYSRemappingSetPtr != gubDBTSet) { ///DBTSet編號不對(會進這個case多半是preformat bug)

						ftlPreformatShowError(SystemArea_Error_DBTSet, ubSYSRemappingSetPtr, 0x85);
						return 1;
					}
					ubDBTSetCheck = 0;
				}
				else {
					ftlAddToSystemSetFreeTable(ubSYSRemappingSetPtr);
				}
			}
		}
		ubSYSRemappingSetPtr++;

#if (MicronFlashOnly && (MICRON_NEW_BOOTCODE == 0))
#if (B0KB)   //這裡在卡code block的數量
		if (ubSYSRemappingSetPtr == 2) {
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				if ((gubCENumber == 5) || (gubCENumber == 10) || (gubCENumber == 11)) {//B0KB Force to 2 plane
					if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (4 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
				else {
					if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (2 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
#elif (!B0KB)//此判斷給L06B //B16 define出來後需歸類跟B0KB一樣
		if (ubSYSRemappingSetPtr == 2) {
			for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (4 << gubBurstsPerBankLog)) {
					ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
					SetFailCEtoSystemInfo(ubCEIndex);
					return TERMINAL_FAIL;
				}
			}
		}
#endif
#elif Hynix  //Hynix//如果有特殊case以後再額外寫
		if (guwPagesPerUnit == 128) {
			if (ubSYSRemappingSetPtr == 4) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (63 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
		else if (guwPagesPerUnit == 256) {
			if (ubSYSRemappingSetPtr == 4) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (32 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
#else  //for Toshiba
		if (gubBurstsPerBank == 2) {
			if (ubSYSRemappingSetPtr == 4) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (32 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
		else if (gubBurstsPerBank == 4) {
			if (ubSYSRemappingSetPtr == 4) {
				for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
					if (guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex] >= (16 << gubBurstsPerBankLog)) {
						ftlPreformatShowError(ubCEIndex, guwSystemBlockRemappingTable[(ubSYSRemappingSetPtr - 1)][ubCEIndex], 0x87);
						SetFailCEtoSystemInfo(ubCEIndex);
						return TERMINAL_FAIL;
					}
				}
			}
		}
#endif
	}

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		guwSystemAreaNum[ubCEIndex] = guwSystemBlockRemappingTable[SYSTEM_SETS - 1][ubCEIndex] + 1;
	}

	return NORMAL;
}

UBYTE SaveDBTSet(U32 ulSramAddr)
{
	UBYTE ubDBTNewSet;
	UWORD uwDBTNewBlock[2] = {0};
	UWORD uwDBTBlockTemp[2];
	UBYTE uwDBTSetTemp;
	UBYTE *ubTempBuffer = (UBYTE *)((&gulVenderSMARTBuffer[0]) + 0x8000);
	UBYTE *ubDBTBuffer = (UBYTE *)ulSramAddr;
	UBYTE ubGetDBTSet;

	U32 ulplane = 0;
	U32 ulFreeblock = 0;
	UBYTE ubDBTMappingPlaneIndex = 0;
	UBYTE ubDBTSizeInPlanes = 0;
	U32 ulTotalBlockNum = gulTotalPhyUnitNum * gubPlanesPerPage;
	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......
	UBYTE ubCEIndex, ubCodeNum;

	do {
		ubGetDBTSet = TRUE;
		if (gubSystemSetFreeCount > 0) {
			ubDBTNewSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_TAIL);

			//generateHeader是自己去scan block去找可以寫的地方,不從free Q拿, 合理來說會scan到Systemfree Q位子比較前面的free set
			//所以這邊要檢查freeSet是否已經被拿去寫Code Block
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				for (ubCodeNum = 0; ubCodeNum < 2 ; ubCodeNum++) {
					if (guwSysCodeBlock[ubCodeNum][ubCEIndex] != 0xFFFF) {
						if (guwSysCodeBlock[ubCodeNum][ubCEIndex] == guwSystemBlockRemappingTable[ubDBTNewSet][ubCEIndex]) {
							ubGetDBTSet = FALSE;
						}
					}
				}
			}
			if (ubGetDBTSet == TRUE) {
				uwDBTNewBlock[0] = guwSystemBlockRemappingTable[ubDBTNewSet][0];
				uwDBTNewBlock[1] = guwSystemBlockRemappingTable[ubDBTNewSet][1];
			}
		}
		else {//preformat不會進這個case(會進scanSystemArea去卡SystemUnit界線), generateHeader會進這邊
			ftlPreformatShowError(SystemArea_Error_DBTSet, 0, 0x41);
			//看那一個CE的Burner_later bad多就回傳誰(1 Channel時不存在的CE的gubLaterBadCount_ForDBT=0)
			if (gubLaterBadCount_ForDBT[0] > gubLaterBadCount_ForDBT[1])
				SetFailCEtoSystemInfo(0);
			else if (gubLaterBadCount_ForDBT[0] < gubLaterBadCount_ForDBT[1])
				SetFailCEtoSystemInfo(1);
			else {
				SetFailCEtoSystemInfo(0);
				SetFailCEtoSystemInfo(1);
			}
			return TERMINAL_FAIL;
		}
	}
	while (!ubGetDBTSet);

	if (gubPlanesPerBurst == MAX_CHANNEL) {
		//check DBT挑得是否成對
		UWORD uwOtherBlockInSet;
		uwOtherBlockInSet = ftlGetAnotherSystemBlockInSet(Mode_ByRemappingTable, 0, 0, uwDBTNewBlock[0]);
		if (uwDBTNewBlock[1] != uwOtherBlockInSet) {
			ftlPreformatShowError(SystemArea_Error_DBTSet, uwDBTNewBlock[1], 0x86);
			return TERMINAL_FAIL;
		}
	}

	while (ulTotalBlockNum > ulFreeblock) {
		//programDBTtoFLA
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			UWORD uwDBTUnit = uwDBTNewBlock[ubCEIndex] >> gubBurstsPerBankLog;
			UBYTE ubDBTBurst = (UBYTE)(uwDBTNewBlock[ubCEIndex] & gubBurstsPerBankMask);
			if (ftlWriteSinglePlane(uwDBTUnit, ulplane,  ubCEIndex, (U32)&ubDBTBuffer[ulFreeblock], MARK_DBT, 0xFF012345, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubDBTBurst)) {
				U32 uli = (U32)(M_Multiplier((uwDBTUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubDBTBurst;
				ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);
				gubLaterBadCount_ForDBT[ubCEIndex]++;
				return LATERBAD_FAIL;
			}
		}

		ubDBTSizeInPlanes++;
		ubDBTMappingPlaneIndex++;
		ulplane++;//ulplane += gubPlanesPerPage;
		ulFreeblock += ulEntryPerDBTPlane;
	}

	while (ulplane < guwFastPagePagesPerUnit) {
		if (ubDBTMappingPlaneIndex == ubDBTSizeInPlanes) {	// 寫完一份DBT, 需先reset再繼續寫下一份DBT
			ubDBTMappingPlaneIndex = 0;
			ulFreeblock = 0;
		}
		//programDBTtoFLA
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			UWORD uwDBTUnit = uwDBTNewBlock[ubCEIndex] >> gubBurstsPerBankLog;
			UBYTE ubDBTBurst = (UBYTE)(uwDBTNewBlock[ubCEIndex] & gubBurstsPerBankMask);
			if (ftlWriteSinglePlane(uwDBTUnit, ulplane,  ubCEIndex, (U32)&ubDBTBuffer[ulFreeblock], MARK_DBT, 0xFF012345, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubDBTBurst)) {
				U32 uli = (U32)(M_Multiplier((uwDBTUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubDBTBurst;
				ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);
				gubLaterBadCount_ForDBT[ubCEIndex]++;
				return LATERBAD_FAIL;
			}
		}
		ubDBTMappingPlaneIndex++;
		ulplane++;//ulplane += gubPlanesPerPage;
		ulFreeblock += ulEntryPerDBTPlane;
	}

	if (gubDBTSet != 0xFF) { //原本DBT存在
		//check the new DBT
		uwDBTBlockTemp[0] = guwDBTBlock[0];
		uwDBTBlockTemp[1] = guwDBTBlock[1];
		uwDBTSetTemp = gubDBTSet;

		guwDBTBlock[0] = uwDBTNewBlock[0];
		guwDBTBlock[1] = uwDBTNewBlock[1];
		gubDBTSet = ubDBTNewSet;

		ulplane = 0;
		ulFreeblock = 0;
		while (ulTotalBlockNum > ulFreeblock) {
			if (ftlLoadDBT(ulFreeblock, (U32)&ubTempBuffer[0])) {//read fail
				for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
					UWORD uwDBTUnit = guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog;
					UBYTE ubDBTBurst = (UBYTE)(guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask);

					U32 uli = (U32)(M_Multiplier((uwDBTUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubDBTBurst;
					ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_READ_FAIL);
					gubLaterBadCount_ForDBT[ubCEIndex]++;
				}
				//return fail前記得把原本的DBT填回去
				guwDBTBlock[0] = uwDBTBlockTemp[0];
				guwDBTBlock[1] = uwDBTBlockTemp[1];
				gubDBTSet = uwDBTSetTemp;
				return LATERBAD_FAIL;
			}
			ulFreeblock += ulEntryPerDBTPlane;
		}
		//Erase原本的DBT並且放回FreeQ
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			UWORD uwDBTUnit = uwDBTBlockTemp[ubCEIndex] >> gubBurstsPerBankLog;
			UBYTE ubDBTBurst = (UBYTE)(uwDBTBlockTemp[ubCEIndex] & gubBurstsPerBankMask);

			ftl_Erase_Single_Plane(uwDBTUnit, ubDBTBurst, ubCEIndex, 0);
			ftl_Erase_Single_Plane(uwDBTUnit, ubDBTBurst, ubCEIndex, BIT_FJOBI_FASTPAGE);
		}

		if (uwDBTSetTemp < SYSTEM_SETS) {
			ftlAddToSystemSetFreeTable(uwDBTSetTemp);//ADD old DBT unit in to free System unit
		}
	}
	else {
		guwDBTBlock[0] = uwDBTNewBlock[0];
		guwDBTBlock[1] = uwDBTNewBlock[1];
		gubDBTSet = ubDBTNewSet;

		ulplane = 0;
		ulFreeblock = 0;
		while (ulTotalBlockNum > ulFreeblock) {
			if (ftlLoadDBT(ulFreeblock, (U32)&ubTempBuffer[0])) {//read fail
				for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
					UWORD uwDBTUnit = guwDBTBlock[ubCEIndex] >> gubBurstsPerBankLog;
					UBYTE ubDBTBurst = (UBYTE)(guwDBTBlock[ubCEIndex] & gubBurstsPerBankMask);

					U32 uli = (U32)(M_Multiplier((uwDBTUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubDBTBurst;
					ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_READ_FAIL);
					gubLaterBadCount_ForDBT[ubCEIndex]++;
				}
				//return fail前記得把原本的DBT填回去
				guwDBTBlock[0] = 0xFFFF;
				guwDBTBlock[1] = 0xFFFF;
				gubDBTSet = 0xFF;
				return LATERBAD_FAIL;
			}
			ulFreeblock += ulEntryPerDBTPlane;
		}
	}

	return NORMAL;
}

UBYTE SaveSystemSet(U32 ulSramAddr)
{
	//因為systemSet在ScanSystemArea就已經決定了,所以這邊不用再挑了
	U32 ulDMAAddr;
	U32 ulLCA_Temp, uli;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	UBYTE ubCEIndex;
	UBYTE *ubDBTBuffer = (UBYTE *)ulSramAddr;

	if (gubPlanesPerBurst == MAX_CHANNEL) {
		//check System 挑得是否成對
		UWORD uwOtherBlockInSet;
		uwOtherBlockInSet = ftlGetAnotherSystemBlockInSet(Mode_ByRemappingTable, 0, 0, guwSystemBlock[0]);
		if (guwSystemBlock[1] != uwOtherBlockInSet) {
			ftlPreformatShowError(SystemArea_Error_DBTSet, guwSystemBlock[1], 0x86);
			return TERMINAL_FAIL;
		}
	}

	memcpy(&gubL2PBase[M_SysInfo_InfoBlk], &BUFB_BASE[0], 512);//infor blk
	gubL2PBase[M_SysInfo_gubDram_Size] = gubDram_Size;
	if (gubDram_Size < 4) {
		gubSDRSizeMB = (BIT0 << (gubDram_Size + 2));
	}
	else {
		gubSDRSizeMB = (BIT0 << (gubDram_Size + 3));
	}
	gubL2PBase[M_SysInfo_gubDram_Size] = gubSDRSizeMB;
	ulDMAAddr = (U32)gubL2PBase;
	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)ulDMAAddr, ubL4KNum * 4096);
	}

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		ulLCA_Temp = MARK_SYS_RDT;
		UWORD uwSystemUnit = guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog;
		UBYTE ubSystemBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
		if (ftlWriteSinglePlane(uwSystemUnit, 0, ubCEIndex, ulDMAAddr, ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
			U32 uli = (U32)(M_Multiplier((uwSystemUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSystemBurst;
			ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);
			return LATERBAD_FAIL;
		}
	}

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		UWORD uwSystemUnit = guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog;
		UBYTE ubSystemBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);
		mDMAC_SV(1, (U32)gubL2PBase,  (U32)gubSectorsPerPlane * 512, 0, 0, 0, DMA_EN_SETCQ, 1);

		for (uli = 1; uli < 3 ; uli ++) {

			ulLCA_Temp = MARK_DPST;
			if (ftlWriteSinglePlane(uwSystemUnit, uli, ubCEIndex, ulDMAAddr, ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
				U32 uli = (U32)(M_Multiplier((uwSystemUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSystemBurst;
				ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);
				return LATERBAD_FAIL;
			}
		}
#if Hynix
		//mDMAC_SV(1, (U32)gubL2PBase,  (U32)gubSectorsPerPlane * 512, 0, 0, 0, DMA_EN_SETCQ, 1);
		mDMAC_SV(1, (U32)gubL2PBase,  (U32)32 * 1024, 0, 0, 0, DMA_EN_SETCQ, 1);//2plane Save RR Table
		gpsHynixRetryMeta = (HynixRetryMeta *) (gubL2PBase + 0x0100);
		gpubSlcRetryTable = (UBYTE *) (gubL2PBase + 0x0400);
#if (Hynix3DV6 || Hynix3DV5)
		gpubXlcRetryTable = (UBYTE *) (gubL2PBase + 0x1007); // RRT table 0
#else
		gpubXlcRetryTable = (UBYTE *) (gubL2PBase + 0x1000);
#endif
		gsHynixRetryInfo.ulSlcRetryTblBase = (U32) gpubSlcRetryTable;
		gsHynixRetryInfo.ulXlcRetryTblBase = (U32) gpubXlcRetryTable;

		if (flaRetrieveRetryTable(gulCeEnBitMap)) {
			ftlPreformatShowError(0x99, (guwSystemBlock[0] >> gubBurstsPerBankLog), 0x12);
			return TERMINAL_FAIL;
		}

		memcpy((void *)gubL2PBase, &gsHynixRetryInfo, sizeof(HynixRetryInfo));//32B
#if (ENABLE_DCACHE)
		xthal_dcache_region_writeback_inv((void *)ulDMAAddr, ubL4KNum * 4096);
#endif
		ulLCA_Temp = MARK_SYS_HYNIX_RRT;
		for (uli = 3; uli < 5 ; uli ++) {
			if (ftlWriteSinglePlane(uwSystemUnit, uli, ubCEIndex, (ulDMAAddr + ((uli - 3) * 16384)), ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ((UBYTE)ubSystemBurst))) {
				U32 uli = (U32)(M_Multiplier((uwSystemUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSystemBurst;
				ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);
				return LATERBAD_FAIL;
			}
		}


		for (uli = 5 ; uli <  guwFastPagePagesPerUnit; uli ++) {
			ulLCA_Temp = MARK_SYS_RDT;
			if (ftlWriteSinglePlane(uwSystemUnit, uli, ubCEIndex, ulDMAAddr, ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
				U32 uli = (U32)(M_Multiplier((uwSystemUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSystemBurst;
				ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);
				return LATERBAD_FAIL;
			}
		}

#else

		for (uli = 3 ; uli <  guwFastPagePagesPerUnit; uli ++) {
			ulLCA_Temp = MARK_SYS_RDT;
			if (ftlWriteSinglePlane(uwSystemUnit, uli, ubCEIndex, ulDMAAddr, ulLCA_Temp, 0x112233, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
				U32 uli = (U32)(M_Multiplier((uwSystemUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSystemBurst;
				ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_WRITE_FAIL);
				return LATERBAD_FAIL;
			}
		}
#endif
	}

	//1 Read for verification...
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		UWORD uwSystemUnit = guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog;
		UBYTE ubSystemBurst = (UBYTE)(guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask);

		for (uli = 0; uli <  guwFastPagePagesPerUnit; uli ++) {
			if ((uli == 1) || (uli == 2)) {
				ulLCA_Temp = MARK_DPST;
			}
#if Hynix
			else if ((uli == 3) || (uli == 4)) {
				ulLCA_Temp = MARK_SYS_HYNIX_RRT;
			}
#endif
			else {
				ulLCA_Temp = MARK_SYS_RDT;
			}

			if (ftlReadSinglePlane(uwSystemUnit, uli, ubCEIndex, (U32)&gubCopyBuffer[0], ulLCA_Temp,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY), 4, 0, ubSystemBurst)) {
				U32 uli = (U32)(M_Multiplier((uwSystemUnit << gubDieNumberLog), DEF_PlanesPerPage_Mcl_Div)) + (U32)(ubCEIndex << gubBurstsPerBankLog) + (U32)ubSystemBurst;
				ubDBTBuffer[uli] = (RDT_BURNER_MARKBAD | DBT_MARK_READ_FAIL);
				return LATERBAD_FAIL;
			}
		}
	}

	return NORMAL;
}

UBYTE ftlRDTPreformat(void)
{
	M_CheckRDTModeToDisableFunctionReturnValid(1);
	UBYTE *ubDBTBuffer = (UBYTE *)(&GCGRSearchTable[0]);
	UBYTE ubPreformatState = PREFORMAT_BUILD_DBT;
	UBYTE ubFail, ubi, ubj;

	if (gubEraseFailBitMapInit == 0) {
		mDMAC_SV(1, (U32)gubEraseFailBitMap, (U32)(64 * 1024), 0, 0, 0, DMA_EN_SETCQ, 0); // 假設最大16CE, 4 plane, 8192 unit, 16*4*8192bit = 64KB
	}

	for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
		gubLaterBadCount_ForDBT[ubi] = 0;
	}

	/*
	Step 1-> PREFORMAT_BUILD_DBT
	Step 2-> REFORMAT_SCAN_SYSTEMAREA
	Step 3-> REFORMAT_SAVE_DBT
	Step 4-> REFORMAT_SAVE_SYSTEM
	Step 5-> PREFORMAT_DONE
	*/
	do {
		switch (ubPreformatState) {
			case PREFORMAT_BUILD_DBT:
				ubFail = BuildDBTinRAM((U32)&ubDBTBuffer[0]);
				if (ubFail == TERMINAL_FAIL) {
					ubPreformatState = PREFORMAT_ERROR;
				}
				else {
					ubPreformatState = PREFORMAT_SCAN_SYSTEMAREA;
				}
				break;

			case PREFORMAT_SCAN_SYSTEMAREA:
				ubFail = ScanRDTSystemArea((U32)&ubDBTBuffer[0]);
				if (ubFail == TERMINAL_FAIL) {
					ubPreformatState = PREFORMAT_ERROR;
				}
				else {
					ubPreformatState = PREFORMAT_SAVE_DBT;
				}
				break;

			case PREFORMAT_SAVE_DBT:
				ubFail = SaveDBTSet((U32)&ubDBTBuffer[0]);
				if (ubFail == TERMINAL_FAIL) {
					ubPreformatState = PREFORMAT_ERROR;
				}
				else if (ubFail == LATERBAD_FAIL) {
					ubPreformatState = PREFORMAT_ERASE_SYSTEMAREA;
				}
				else {
					ubPreformatState = PREFORMAT_SAVE_SYSTEM;
				}
				break;

			case PREFORMAT_SAVE_SYSTEM:
				ubFail = SaveSystemSet((U32)&ubDBTBuffer[0]);
				if (ubFail == TERMINAL_FAIL) {
					ubPreformatState = PREFORMAT_ERROR;
				}
				else if (ubFail == LATERBAD_FAIL) {
					ubPreformatState = PREFORMAT_ERASE_SYSTEMAREA;
				}
				else {
					ubPreformatState = PREFORMAT_DONE;
				}
				break;

			case PREFORMAT_ERASE_SYSTEMAREA:
				for (ubi = 0; ubi < SYSTEM_SETS; ubi++) {
					for (ubj = 0; ubj < MAX_CHANNEL; ubj++) {
						if (guwSystemBlockRemappingTable[ubi][ubj] != guwDBTBlock[ubj]) { //別把原本的DBT幹掉了
							UWORD uwUnit = guwSystemBlockRemappingTable[ubi][ubj] >> gubBurstsPerBankLog;
							UBYTE ubBurst = (UBYTE)(guwSystemBlockRemappingTable[ubi][ubj] & gubBurstsPerBankMask);
							ftl_Erase_Single_Plane(uwUnit, ubBurst, ubj, 0);//Normal Erase
						}
					}
				}
				ubPreformatState = PREFORMAT_SCAN_SYSTEMAREA;
				break;

			case PREFORMAT_ERROR:
				return 1;
				break;

			default:
				break;

		}

	}
	while (ubPreformatState < PREFORMAT_DONE);
	//再看看要不要考慮size的問題

	return 0;
}
#endif

