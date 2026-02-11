
#include "setup.h"
#include "Vardef.h"
#include "ftl.h"
#include "CMDTBL.H"

#include <string.h>
#include <stdlib.h>
#include <IO.h>

//========================================================
//
//#pragma code(".ICode")  // Declare FW Code in DDR
//
//========================================================

void InitDCO(unsigned char ub_Reset)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	if (ub_Reset == 0) {//ATA8 rev2 chapter 4.10 only power cycle clear lock state.
		//HW reset or SW reset not affect lock state.
		AtaCfg.uwDCOState &= (~SET_DCO_LOCK); //clear DCO lock
	}
}


/***************************************************************************
* NAME:         void DeviceConfig(void) - 0xB1
* DESCRIPTION:
*       This routine is for Device Configuration Overlay feature set.
***************************************************************************/
void DeviceConfig(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	HandleStopRW(0);

	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || ((IDT.W83 & BIT11) == 0)
	        || (AtaCfg.uwDCOState & SET_DCO_LOCK)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect) {
			//c0 Restore
			//c3 c5 SET
			if ( (HB[HB_FEATURE] == 0xC0) || (HB[HB_FEATURE] == 0xC3) || (HB[HB_FEATURE] == 0xC5)) {
				gubErrorCode = SET_ABRT;
				return;
			}

		}
	}

	gubErrorCode = NO_ERROR;

	switch (HB[HB_FEATURE]) {
		case 0xC0:  // Restore
			DCO_Restore();
			break;

		case 0xC1:  // Freeze Lock
			DCO_Lock();
			break;

		case 0xC2:  // Identify
			DCO_Identify();
			if (ENABLE_DCACHE) {
				xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
			}
			gubNeedSendCmdComplete = 0;
			RWBuffer(1);
			break;

		case 0xC3:  // SET
			HB[HB_CMD_INFO_LOWBYTE] |= H_WR_CMD; //write command is received
			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate((void *)BUFB_BASE, 512);
			}
			RWBuffer(1);
			DCO_Set();
			break;

		case 0xC4:  // Identify DMA
			if (IDT.W49 & SET_BIT8) {
				HB[HB_CMD_INFO_LOWBYTE] |= H_PIO_DMA_CMD; //dma command is received
				DCO_Identify();
				if (ENABLE_DCACHE) {
					xthal_dcache_region_writeback_inv((void *)BUFB_BASE, 512);
				}
				RWBuffer(1); //get buffer would clear gubNAtaCmdCpl
			}
			else {
				gubErrorCode = SET_ABRT;
			}
			break;

		case 0xC5:  // SET DMA
			HB[HB_CMD_INFO_LOWBYTE] |= (H_PIO_DMA_CMD | H_WR_CMD);
			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate((void *)BUFB_BASE, 512);
			}
			RWBuffer(1);
			DCO_Set();
			break;
		default:
			gubErrorCode = SET_ABRT;
			break;
	}
#endif
}

void DCO_Restore(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (AtaCfg.uwDCOState & SET_DCO_SET) {
		if (AtaCfg.ubHpaState & (SET_HPASET | SET_HPALOCK)) {//if HPA enable, not allow media size change
			//SET_HPASET, SET_HPALOCK這兩個state表示nativ max已被改過...
			gubErrorCode = SET_ABRT;
			return;
		}
		else {
			gulMediaSize.All = gulNativeSize;
			ChangeMediaSize();
		}

		if (AtaCfg.uwDCOState & SET_DCO_IPM) {
			HW[HW_PM_CTRL] = AtaCfg.uwPM_CTRL_Temp;
			gubDevIPMON = AtaCfg.ubDevIPMON_Temp;
		}

		AtaCfg.uwDCOState = 0;
		SaveAtaConfig();

		//if (gubHIPMSupport || gubDIPMSupport) {
		//	HW[HW_PM_CTRL] |= H_IPM_EN;// Bit7 : Enable IPM (interface power management)
		//}

	}
	else {
		// DCO0:DCO0
		gubErrorCode = SET_ABRT;
	}
#endif
}

void DCO_Lock(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	AtaCfg.uwDCOState |= SET_DCO_LOCK;
}

void DCO_Identify(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubCheckSum;
	UWORD uwi;

	mSE_SV(8, (U32)BUFB_BASE, 512, 0, 0);

	// =====================================
	BUFW_BASE[0] = 0x0002; //Revision
	BUFW_BASE[1] = gXferMode.ubDMASupportMAP[SEL_MDMA]; //MDMA
	BUFW_BASE[2] = gXferMode.ubDMASupportMAP[SEL_UDMA]; //UDMA
	gulLBA.All = gulNativeSize - 1;
	BUFW_BASE[3] = gulLBA.INTMODE.INT1;
	BUFW_BASE[4] = gulLBA.INTMODE.INT2;
	/*
	15 Reserved
	14 1 = Reporting support for the Write-Read-Verify feature set is changeable
	13 1 = Reporting support for the SMART Conveyance self-test is changeable
	12 1 = Reporting support for the SMART Selective self-test is changeable
	11 1 = Reporting support for the Forced Unit Access is changeable
	10 Reserved for TLC
	9 1 = Reporting support for the Streaming feature set is changeable
	8 1 = Reporting support for the 48-bit Addressing feature set is changeable
	7 1 = Reporting support for the HPA feature set is changeable
	6 1 = Reporting support for the AAM feature set is changeable
	5 This field is obsolete.
	4 1 = Reporting support for the PUIS feature set is changeable
	3 1 = Reporting support for the Security feature set is changeable
	2 1 = Reporting support for the SMART error log is changeable
	1 1 = Reporting support for the SMART self-test is changeable
	0 1 = Reporting support for the SMART feature set is changeable
	*/
	BUFW_BASE[7] = 0x008F;
	/*
	4 1 = Reporting support for the SSP feature set is changeable
	3 1 = Reporting support for asynchronous notification is changeable
	2 1 = Reporting support for interface power management is changeable
	1 1 = Reporting support for non-zero buffer offsets is changeable
	0 1 = Reporting support for the NCQ feature set is changeable
	*/
	BUFW_BASE[8] = 0x0014;
	/*
	15 1 = Reporting support for the NV Cache feature set is changeable
	14 1 = Reporting support for the NV Cache Power Management feature set is changeable
	13 1 = Reporting support for WRITE UNCORRECTABLE EXT is changeable
	12 1 = Reporting of support for the Trusted Computing feature set is changeable
	11 1 = Reporting support for the Free-fall Control feature set is changeable
	10 1 = Reporting support for the DATA SET MANAGEMENT command is changeable
	9 1 = Reporting support for Extended Power Conditions is changeable
	*/
	BUFW_BASE[21] = 0x0400;
	BUFW_BASE[255] = 0x00A5; //Signature
	//check sum should be 0xAFA5



	ubCheckSum = 0;
	for (uwi = 0; uwi < 511; uwi++) {
		ubCheckSum += BUFB_BASE[uwi];
	}

	// 2's Complement of the Sum of the 1st 511 Bytes.
	// eg. ubCheckSum=0xf0, ~ubCheckSum=0x0f,
	//     so,  2's Complement of ubCheckSum=0x0f+0x01=0x10
	ubCheckSum = ~ubCheckSum;
	ubCheckSum++;
	BUFB_BASE[511] = ubCheckSum;
}


void DCO_Set(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubCheckZero;
	UBYTE ubCheckBit;
	UBYTE ubDMASelectModeMap;
	UWORD uwConfigSet;
	UWORD uwi;
	UBYTE ubSum;

	uwConfigSet = 0;
	HW[HW_LBA_L] = 0;
	HB[HB_LBA_H] = 0;
	gubErrorCode = SET_ABRT;//default error

	if (AtaCfg.uwDCOState & SET_DCO_SET) {
		return;
	}

	//verify check sum
	if (BUFB_BASE[510] != 0xA5) {
		HB[HB_LBA_H] = 255;
		return;
	}

	ubSum = 0;
	for (uwi = 0; uwi < 512; uwi++) {
		ubSum += BUFB_BASE[uwi];
	}
	if (ubSum != 0) {
		HB[HB_LBA_H] = 255;
		return;
	}

	//verify revision
	if (BUFW_BASE[0] != 0x0002) {
		HB[HB_LBA_H] = 0;
		return;
	}

	//change MDMA
	BUFW_BASE[1] &= gXferMode.ubDMASupportMAP[SEL_MDMA];

	if (gXferMode.ubDefaultDMAType == SEL_MDMA) { // if W63 set MDMA2(100b), ubDMASelectModeMap would be 111b
		ubDMASelectModeMap = ((BIT0 << gXferMode.ubDMASelectMode[SEL_MDMA]) << 1) - 1;
	}
	else {//MDMA disable
		ubDMASelectModeMap = 0;
	}

	ubCheckZero = 0;
	ubCheckBit = (BIT0 << gXferMode.ubDMASupportMode[SEL_MDMA]);

	//ex: if select mode2, now only support to mode1, then bit2 will check fail
	do {
		if (ubCheckZero) {
			// ub_CheckZero=1 means all the following bit should be set to 1
			if ((BUFW_BASE[1] & ubCheckBit) == 0) {
				HW[HW_LBA_L] |= ubCheckBit;
			}
		}
		else {
			if ((ubDMASelectModeMap & ubCheckBit) || (BUFW_BASE[1] & ubCheckBit)) {
				ubCheckZero = 1;
				continue;
			}
		}

		ubCheckBit >>= 1;
	}
	while (ubCheckBit);

	if (HW[HW_LBA_L]) {
		HB[HB_LBA_H] = 1;
		return;
	}
	uwConfigSet |= SET_DCO_MDMA;


	//change UDMA

	BUFW_BASE[2] &= gXferMode.ubDMASupportMAP[SEL_UDMA];

	if (gXferMode.ubDefaultDMAType == SEL_UDMA) { // if W88 set UDMA5(100000b), ubDMASelectModeMap would be 111111b
		ubDMASelectModeMap = ((BIT0 << gXferMode.ubDMASelectMode[SEL_UDMA]) << 1) - 1;
	}
	else {
		ubDMASelectModeMap = 0;
	}

	ubCheckZero = 0;
	ubCheckBit = (BIT0 << gXferMode.ubDMASupportMode[SEL_UDMA]);

	do {
		if (ubCheckZero) {
			if ((BUFW_BASE[2] & ubCheckBit) == 0) {
				HW[HW_LBA_L] |= ubCheckBit;
			}
		}
		else {
			if ((ubDMASelectModeMap & ubCheckBit) || (BUFW_BASE[2] & ubCheckBit)) {
				ubCheckZero = 1;
				continue;
			}
		}
		ubCheckBit >>= 1;
	}
	while (ubCheckBit);

	if (HW[HW_LBA_L]) {
		HB[HB_LBA_H] = 2;
		return;
	}
	uwConfigSet |= SET_DCO_UDMA;


	//change media size
	HB[HB_LBA_H] = 3;
	if (BUFW_BASE[5] || BUFW_BASE[6]) {
		return;
	}

	if (AtaCfg.ubHpaState & (SET_HPASET | SET_HPALOCK)) {
		return;
	}
	//LBA.All = gulNativeSize - 1;
	gulLBA.INTMODE.INT1 = BUFW_BASE[3];
	gulLBA.INTMODE.INT2 = BUFW_BASE[4];
	if ((gulLBA.All + 1) > gulNativeSize) {
		return;
	}
	else if ((gulLBA.All + 1) != gulNativeSize) {
		uwConfigSet |= SET_DCO_SIZE;
	}
	/*
	7 1 = Reporting support for the HPA feature set is changeable
	6 1 = Reporting support for the AAM feature set is changeable
	5 This field is obsolete.
	4 1 = Reporting support for the PUIS feature set is changeable
	3 1 = Reporting support for the Security feature set is changeable
	2 1 = Reporting support for the SMART error log is changeable
	1 1 = Reporting support for the SMART self-test is changeable
	0 1 = Reporting support for the SMART feature set is changeable
	*/
	//BUF0W_BASE[7] = 0x008F;
	HB[HB_LBA_H] = 7; //word offset

	if (BUFW_BASE[7] != 0x008F) {
		if ((BUFW_BASE[7] & CHK_BIT0) == 0) {
			if ((BUFW_BASE[7] & (BIT1 | BIT2)) || (IDT.W85 & BIT0)) { //smart enable
				HW[HW_LBA_L] |= ((~BUFW_BASE[7]) & (BIT0 | BIT1 | BIT2));
			}

			uwConfigSet |= SET_DCO_SMART;
		}

		if ((BUFW_BASE[7] & CHK_BIT1) == 0) {
			uwConfigSet |= SET_DCO_SELFTEST;
		}

		if ((BUFW_BASE[7] & CHK_BIT2) == 0) {
			uwConfigSet |= SET_DCO_LOG;
		}

		if ((BUFW_BASE[7] & CHK_BIT3) == 0) {
			if (AtaCfg.ubSecMode & 0x1E) {
				//If bit 3 of word 7 is cleared to zero, and security is enabled, then the device shall return command aborted,
				//*return abort when security frozen, because spec said after DCO restore, need to become SEC1.
				HW[HW_LBA_L] |= SET_BIT3;// bit offset
			}
			uwConfigSet |= SET_DCO_SEC;
		}
		if ((BUFW_BASE[7] & CHK_BIT7) == 0) {
			if (AtaCfg.ubHpaState) {
				HW[HW_LBA_L] |= SET_BIT7;// bit offset
			}
			uwConfigSet |= SET_DCO_HPA;
		}

		if (HW[HW_LBA_L]) {
			return;
		}
	}
	/*
	4 1 = Reporting support for the SSP feature set is changeable
	3 1 = Reporting support for asynchronous notification is changeable
	2 1 = Reporting support for interface power management is changeable
	1 1 = Reporting support for non-zero buffer offsets is changeable
	0 1 = Reporting support for the NCQ feature set is changeable
	*/
	// BUF0W_BASE[8] = 0x0014;
	if (BUFW_BASE[8] != 0x0014) {
		if ( (BUFW_BASE[8]&BIT2) == 0) {
			if (IDT.W79 & CHK_BIT3) { //  3  Device initiated power management enabled
				HB[HB_LBA_H] = 8; //word offset
				HW[HW_LBA_L] = 0x0004;// bit offset
				return;
			}
			uwConfigSet |= SET_DCO_IPM;
		}


		if ( (BUFW_BASE[8]&BIT4) == 0) {
			uwConfigSet |= SET_DCO_SSP;
		}
	}
	/*
	15 1 = Reporting support for the NV Cache feature set is changeable
	14 1 = Reporting support for the NV Cache Power Management feature set is changeable
	13 1 = Reporting support for WRITE UNCORRECTABLE EXT is changeable
	12 1 = Reporting of support for the Trusted Computing feature set is changeable
	11 1 = Reporting support for the Free-fall Control feature set is changeable
	10 1 = Reporting support for the DATA SET MANAGEMENT command is changeable
	9 1 = Reporting support for Extended Power Conditions is changeable
	*/
	if (BUFW_BASE[21] != 0x0400) {
		if ((BUFW_BASE[21]& BIT10) == 0) {
			uwConfigSet |= SET_DCO_TRIM;
		}
	}

	gubErrorCode = 0;

	//因為之後可能會下DCO_RESTORE，所以不能直接用IDT來存

	memset(&AtaCfg.IDM_W63, 0xff, 22); // set IDM_Wxx to 0xFF




	if (uwConfigSet & SET_DCO_MDMA) {
		AtaCfg.IDM_W63 = 0xff00 | BUFW_BASE[1]; //modify support part, not modify select part
	}
	if (uwConfigSet & SET_DCO_UDMA) {
		AtaCfg.IDM_W88 = 0xff00 | BUFW_BASE[2];
	}
	//config media size
	if (uwConfigSet & SET_DCO_SIZE) {
		AtaCfg.ulDcoSize = (gulLBA.All + 1);
		gulMediaSize.All = (gulLBA.All + 1);
		ChangeMediaSize();
	}

	if (uwConfigSet & SET_DCO_HPA) { //HPA
		AtaCfg.IDM_W82_85 &= CLR_BIT10;//85 bit 10
		AtaCfg.IDM_W83_86 &= CLR_BIT8;//86 bit 8
		AtaCfg.IDM_W69 &= CLR_BIT9;//69 bit 9
	}

	if (uwConfigSet & SET_DCO_SEC) {//3 1 = Reporting support for the Security feature set is changeable
		AtaCfg.IDM_W82_85 &= CLR_BIT1;//85 bit 1
		AtaCfg.IDM_W_Security = 0; //89,90,92,128
	}
	if (uwConfigSet & SET_DCO_LOG) {//2 1 = Reporting support for the SMART error log is changeable
		AtaCfg.IDM_W84_87 &= CLR_BIT0;//87 bit 0
	}
	if (uwConfigSet & SET_DCO_SELFTEST) {//1 1 = Reporting support for the SMART self-test is changeable
		AtaCfg.IDM_W84_87 &= CLR_BIT1;//87 bit 1
	}
	if (uwConfigSet & SET_DCO_SMART) {//0 1 = Reporting support for the SMART feature set is changeable
		AtaCfg.IDM_W82_85 &= CLR_BIT0;//85 bit 0
	}
	if (uwConfigSet & SET_DCO_IPM) {//2 1 = Reporting support for interface power management is changeable
		AtaCfg.IDM_W76 &= CLR_BIT13;
		AtaCfg.IDM_W76 &= CLR_BIT14;

		AtaCfg.IDM_W76 &= CLR_BIT9;
		AtaCfg.IDM_W78 &= CLR_BIT3;
		AtaCfg.IDM_W79 &= CLR_BIT3;
		AtaCfg.IDM_W79 &= CLR_BIT7;

		AtaCfg.uwPM_CTRL_Temp = HW[HW_PM_CTRL];
		AtaCfg.ubDevIPMON_Temp = gubDevIPMON;

		gubDevIPMON = 0;
		HW[HW_PM_CTRL] &= ( ~(H_HW_AUTO_DIPM_EN | H_REG_APSREQ | H_IPM_EN));

	}
	if (uwConfigSet & SET_DCO_SSP) {
		AtaCfg.IDM_W78 &= CLR_BIT6;
		AtaCfg.IDM_W79 &= CLR_BIT6;
	}
	if (uwConfigSet & SET_DCO_TRIM) {// 10 1 = Reporting support for the DATA SET MANAGEMENT command is changeable
		AtaCfg.IDM_W_Trim = 0; //105,169
		AtaCfg.IDM_W69 &= (CLR_BIT5 & CLR_BIT14);
	}

	AtaCfg.uwDCOState = SET_DCO_SET | uwConfigSet; //記哪些被改成不support的
	SaveAtaConfig();
}

//========================================================
//
//#pragma code()  // Declare FW Code in ICCM
//
//========================================================

