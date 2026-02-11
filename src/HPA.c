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

void InitHPA(unsigned char ub_Reset)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	AtaCfg.ubHpaState = AtaCfg.ubHpaState & (~SET_HPAFNV);
	if (ub_Reset) { //Hardware reset
		//first set max address (nv) should be clear after power-on or hardware reset(ATA8 rev2, 7.51.2.2)

		if (AtaCfg.ubHpaState & SET_HPALOCK) {//
			if ((AtaCfg.ubHpaState & 0x07) == 0x01) { //HL1:H2, HEL1:H2
				AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPANVLBA) | (0x02);
			}
		}
		else if (AtaCfg.ubHpaState & SET_HPASET) {
			if ((AtaCfg.ubHpaState & 0x07) == 0x01) { //HS1:H0, HES1:H0
				AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPANVLBA);
			}
			else if ((AtaCfg.ubHpaState & 0x07) == 0x02) { //HS2:HS3, HES2:HES3
				AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPAALL) | (0x03);
			}
			else if ((AtaCfg.ubHpaState & 0x07) == 0x04) { //HS4:H1, HES4:H1
				AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPANVLBA) | (0x01);
			}
		}
	}
	else { //power cycle
		if (AtaCfg.ubHpaState & (SET_HPASET | SET_HPALOCK)) {
			if (((AtaCfg.ubHpaState & 0x07) == 0x01) || ((AtaCfg.ubHpaState & 0x07) == 0x04)) {//HS1:H0, HS4:H0, HL1:H0, HL4:H0
				AtaCfg.ubHpaState = AtaCfg.ubHpaState & SET_HPANVLBA;
			}
			else { //HS2:HS3, HS3b:HS3, HS5:HS3, HS6:HS3
				//HL2:HS3, HL3:HS3, HL5:HS3, HL6:HS3
				AtaCfg.ubHpaState = (AtaCfg.ubHpaState & (SET_HPANVLBA | SET_HPAEXT)) | SET_HPASET | (0x03); //clear lock& set HPASet
			}
		}
		else { //H[0:3]:H0
			AtaCfg.ubHpaState = AtaCfg.ubHpaState & SET_HPANVLBA;
		}
	}
}

void ReadNativeMax(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((gubLBA48BitCmdSupport == 0 && HB[HB_COMMAND] == 0x27)
	        || ((IDT.W82 & BIT10) == 0)
	        || (AtaCfg.uwDCOState & SET_DCO_HPA)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	HandleStopRW(0);

	if (AtaCfg.uwDCOState & SET_DCO_SIZE) {
		gulLBA.All = AtaCfg.ulDcoSize - 1;
	}
	else {
		gulLBA.All = gulNativeSize - 1;
	}

	if (HB[HB_CMD_INFO_LOWBYTE] & CHK_BIT3) {//LBA 48 command is received
		HL[HL_LBA_L] = gulLBA.All;
		AtaCfg.ulReadNativeMaxLBA = gulLBA.All;
	}
	else {
		if (gulLBA.All >= 0x0FFFFFFE) {
			HL[HL_LBA_L] = 0x00FFFFFE;
			HB[HB_DEVICE] = 0xEF;
			AtaCfg.ulReadNativeMaxLBA = 0x0FFFFFFE;
		}
		else {
			HL[HL_LBA_L] = gulLBA.All & 0x00ffffff;
			HB[HB_DEVICE] = 0xE0 | gulLBA.BYTEMODE.BYTE_4;
			AtaCfg.ulReadNativeMaxLBA = gulLBA.All;
		}
	}

	HB[HB_LBA_M_EXP] = HB[HB_LBA_H_EXP] = 0;


}

void SetMax(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((AtaCfg.ubSecMode & SET_SEC_LOCK)
	        || ((IDT.W82 & BIT10) == 0)
	        || (AtaCfg.uwDCOState & SET_DCO_HPA)
	        || ((gubLBA48BitCmdSupport == 0) && (HB[HB_COMMAND] == 0x37))
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

	UBYTE ubOldHpaState;
	ubOldHpaState = AtaCfg.ubHpaState;

	if (HB[HB_CMD_INFO_LOWBYTE] & CHK_BIT3) { // 0x37 (48 bit), SET MAX ADDRESS EXT
		if (AtaCfg.ubLastCmd == 0x27) {//READ NATIVE MAX ADDRESS EXT
			SetMaxAddress(SET_HPAEXT);
		}
		else {
			gubErrorCode = SET_ABRT;
		}
	}
	else { // 0xF9 (28 bit) SET MAX ADDRESS
		if (AtaCfg.ubLastCmd == 0xF8) {//若前一個command為READ NATIVE MAX ADDRESS，則接下來的command都當做set max address
			SetMaxAddress(0);
		}
		else {
			switch ( HB[HB_FEATURE] ) {
				case 0x01:  // set max password
					HB[HB_CMD_INFO_LOWBYTE] |= H_WR_CMD; //set write direction
					if (ENABLE_DCACHE) {
						xthal_dcache_region_invalidate(BUFB_BASE, 512);
					}
					RWBuffer(1);
					SetMaxSetPass();
					break;

				case 0x02:  // set max lock
					SetMaxLock();
					break;

				case 0x03:  // set max unlock
					HB[HB_CMD_INFO_LOWBYTE] |= H_WR_CMD;
					if (ENABLE_DCACHE) {
						xthal_dcache_region_invalidate(BUFB_BASE, 512);
					}
					RWBuffer(1);
					SetMaxUnlock();
					break;

				case 0x04:  // set max freeze lock
					SetMaxFreeze();
					break;

				case 0x05:  // set max password DMA
					if (IDT.W69 & BIT9) {
						HB[HB_CMD_INFO_LOWBYTE] |= (H_PIO_DMA_CMD | H_WR_CMD); //set write direction
						if (ENABLE_DCACHE) {
							xthal_dcache_region_invalidate(BUFB_BASE, 512);
						}
						RWBuffer(1);
						SetMaxSetPass();
					}
					else {
						gubErrorCode = SET_ABRT;
					}

					break;
				case 0x06:  // set max unlock DMA
					if (IDT.W69 & BIT9) {
						HB[HB_CMD_INFO_LOWBYTE] |= (H_PIO_DMA_CMD | H_WR_CMD);
						if (ENABLE_DCACHE) {
							xthal_dcache_region_invalidate(BUFB_BASE, 512);
						}
						RWBuffer(1);
						SetMaxUnlock();
					}
					else {
						gubErrorCode = SET_ABRT;
					}
					break;

				default:
					gubErrorCode = SET_ABRT;
					break;
			}
		}
	}

	if (ubOldHpaState != AtaCfg.ubHpaState) {
		SaveAtaConfig();
	}

#endif
}



void SetMaxAddress(unsigned char ub_ExtCmd)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ub_NonVolatile, ub_NativeMax;
	U32 ulMaxLBA;
	ULLONG uoLBA;

	if ((IDT.W82 & BIT10) == 0) {
		gubErrorCode = SET_ABRT;
		return;
	}

	gubErrorCode = 0;
	if (ub_ExtCmd) {
		uoLBA = *((ULLONG*)(U32)(&HL[HL_LBA_L]));
		uoLBA &= 0xFFFFFFFFFFFFULL;
	}
	else {
		uoLBA = HL[HL_CMD_S_LBA];
	}

	ub_NonVolatile = ub_NativeMax = 0;

	ulMaxLBA = AtaCfg.ulReadNativeMaxLBA;

	if (uoLBA  > ulMaxLBA) {
		gubErrorCode = SET_ABRT;
		return;
	}
	else if (uoLBA == ulMaxLBA) { //Native Max
		ub_NativeMax = 1;
	}

	if (HB[HB_SECTOR_CNT] & CHK_BIT0) { //Volatile_value(V_V)
		ub_NonVolatile = 1;
	}

	if (AtaCfg.ubHpaState & SET_HPASET) {
		if ((AtaCfg.ubHpaState & SET_HPAEXT) ^ ub_ExtCmd) {//若不同，則回 ABRT
			gubErrorCode = SET_ABRT;
		}
		else if (((AtaCfg.ubHpaState & 0x07) == 0x01) || ((AtaCfg.ubHpaState & 0x07) == 0x04)) {
			if (ub_NativeMax) {// 因為是native max,所以hpasize是無效的,因是現在size直接抓native size就可以...
				if (AtaCfg.ubHpaState & 0x01) { //HS1:H0, HES1:H0
					AtaCfg.ubHpaState = AtaCfg.ubHpaState & SET_HPAFNV;
				}
				else { //HS4:H1, HES4:H1
					AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPAFNV) | (0x01);
				}
			}
			else if (ub_NonVolatile) {
				if (AtaCfg.ubHpaState & 0x01) { //HS1:HS2
					AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPAALL) | (0x02);
				}
				else { //HS4:HS5
					AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPAALL) | (0x05);
				}
				AtaCfg.ubHpaState |= (SET_HPAFNV | SET_HPANVLBA); // first & valid.
			}
			else { //Volatile
				//HS1b:HS1, HS4b:HS4
			}
		}
		else { //HS2,HS3,HS5,HS6
			if (ub_NonVolatile == 0) { //Volatile
				//HS2c:HS2, HS3c:HS3, HS5e:HS5, HS6a:HS6
			}
			else if (ub_NativeMax) {// 因為是native max,所以hpasize是無效的,因是現在size直接抓native size就可以...
				if (AtaCfg.ubHpaState & 0x04) { //HS5:H1, HS6:H1, HES5:H1, HES6:H1
					AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPAFNV) | (0x01);
				}
				else { //HS2:H0, HS3:H0, HES2:H0, HES3:H0
					AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPAFNV);
				}
			}
			else {
				if (((AtaCfg.ubHpaState & 0x07) == 0x03) || ((AtaCfg.ubHpaState & 0x07) == 0x06)) {//HS3:HS2, HS6:HS5
					AtaCfg.ubHpaState--;
				}
				else { //HS2b:HS2, HS5c:HS5 // ID not found
					gubErrorCode = SET_IDNF;
				}
			}
		}
	}
	else if (AtaCfg.ubHpaState & SET_HPALOCK) { // State HL[1:6]
		gubErrorCode = SET_ABRT;
	}
	else {
		if (AtaCfg.ubHpaState & 0x02) { // State H2 or H3
			gubErrorCode = SET_ABRT;
		}
		else { // State H0 or H1
			if (ub_NonVolatile) { // Non Volatile
				if (ub_NativeMax) { //H0b:H0 or H1b:H1
					//clear non volatile LBA mark, media size = native size at next HW reset or Power cycle
					AtaCfg.ubHpaState &= (~SET_HPANVLBA);
				}
				else {
					if (AtaCfg.ubHpaState & SET_HPAFNV) { //H0a:H0 or H1e:H1
						//A host should not issue more than one non-volatile SET MAX ADDRESS or
						//SET MAX ADDRESS EXT command after a power-on or hardware reset.
						gubErrorCode = SET_ABRT;
						return;
					}
					AtaCfg.ubHpaState |= (SET_HPAFNV | SET_HPANVLBA);

					if (AtaCfg.ubHpaState & 0x01) { //H1:HS5
						AtaCfg.ubHpaState |= (SET_HPASET | 0x05);
					}
					else { //H0:HS2
						AtaCfg.ubHpaState |= (SET_HPASET | 0x02);
					}
					if (ub_ExtCmd) {
						AtaCfg.ubHpaState |= SET_HPAEXT;
					}
				}
			}
			else {//volatile
				if (ub_NativeMax) { //H0b:H0 or H1b:H1
				}
				else {
					if (AtaCfg.ubHpaState & 0x01) { //H1:HS4
						AtaCfg.ubHpaState = (AtaCfg.ubHpaState & SET_HPAALL) | (SET_HPASET | 0x04);
					}
					else { //H0:HS1
						AtaCfg.ubHpaState |= (SET_HPASET | 0x01);
					}
					if (ub_ExtCmd) {
						AtaCfg.ubHpaState |= SET_HPAEXT;
					}
				}
			}
		}
	}

	if (gubErrorCode == 0) {
		//When the device successfully processes a SET MAX ADDRESS command with the value returned by the READ
		//NATIVE MAX ADDRESS command, the device shall:
		//e) If the value returned by the READ NATIVE MAX EXT command is greater than the value returned by the
		//READ NATIVE MAX command, then IDENTIFY DEVICE data words 100..103 shall indicate the value
		//returned by the READ NATIVE MAX EXT command and IDENTIFY DEVICE data words 60..61 shall
		//comply with 4.12.4; or
		//f) Otherwise IDENTIFY DEVICE data words 60..61 shall indicate the value returned by the READ NATIVE
		//MAX command and IDENTIFY DEVICE data words 100..103 shall comply with 4.12.4.
		if (ub_NativeMax) {
			if (AtaCfg.uwDCOState & SET_DCO_SIZE) {
				gulMediaSize.All = AtaCfg.ulDcoSize;
			}
			else {
				gulMediaSize.All = gulNativeSize;
			}
		}
		else {
			gulMediaSize.All = uoLBA + 1;
		}
		ChangeMediaSize();
		if (ub_NonVolatile) {
			AtaCfg.ulHpaSize = gulMediaSize.All;
		}
	}


}


void SetMaxSetPass(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((IDT.W82 & BIT10) == 0) {
		gubErrorCode = SET_ABRT;
		return;
	}

	if (AtaCfg.ubHpaState & SET_HPASET) {
		if ((AtaCfg.ubHpaState & 0x04) == 0x00) {//HS1:HS4, HS2:HS5, HS3:HS6
			AtaCfg.ubHpaState = AtaCfg.ubHpaState + 0x03;
		}
		//else HS4c:HS4, HS5d:HS5, HS6c:HS6
	}
	else if (AtaCfg.ubHpaState & SET_HPALOCK) { // State HL[1:6]
		gubErrorCode = SET_ABRT;
	}
	else {
		if (AtaCfg.ubHpaState & 0x02) { //H2a:H2, H3a:H3
			gubErrorCode = SET_ABRT;
		}
		else { //H0:H1, H1c:H1
			AtaCfg.ubHpaState = (AtaCfg.ubHpaState & (SET_HPAFNV | SET_HPANVLBA)) | (0x01);
		}
	}

	if (gubErrorCode == 0) { //store password word[1:16]
		memcpy(AtaCfg.uwHpaPW, (unsigned char *)(BUFFER3_BASE + 2) , 32);
		IDT.W86 |= SET_BIT8;
	}
}

void SetMaxLock(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((IDT.W82 & BIT10) == 0) {
		gubErrorCode = SET_ABRT;
		return;
	}

	if (AtaCfg.ubHpaState & SET_HPASET) {
		if (AtaCfg.ubHpaState & 0x04) {//HS4:HL1, HS5:HL2, HS6:HL3
			AtaCfg.ubHpaState = ((AtaCfg.ubHpaState & (~SET_HPASET)) | SET_HPALOCK) - 0x03;
			AtaCfg.ubHpaSECounter = 5;
		}
		else {//HS1a:HS1, HS2a:HS2, HS3a:HS3
			gubErrorCode = SET_ABRT;
		}
	}
	else if (AtaCfg.ubHpaState & SET_HPALOCK) { // State HL[1:6]
		//HL1c:HL1, HL2D:HL2, HL3d:HL3, HL4a:HL4, HL5a:HL5, HL6a:HL6
		gubErrorCode = SET_ABRT;
	}
	else {
		if ((AtaCfg.ubHpaState & 0x07) == 0x01) { //H1:H2
			AtaCfg.ubHpaState = (AtaCfg.ubHpaState & (SET_HPAFNV | SET_HPANVLBA)) | (0x02);
			AtaCfg.ubHpaSECounter = 5;
		}
		else {//H0a:H0, H2c:H2, H3a:H3
			gubErrorCode = SET_ABRT;
		}
	}

}

void SetMaxUnlock(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((IDT.W82 & BIT10) == 0) {
		gubErrorCode = SET_ABRT;
		return;
	}

	if (AtaCfg.ubHpaState & SET_HPASET) {//HS[1:6]a:HS[1:6]
		gubErrorCode = SET_ABRT;
	}
	else if (AtaCfg.ubHpaState & SET_HPALOCK) {
		if (AtaCfg.ubHpaState & 0x04) {//HL[4:6]a:HL[4:6]
			gubErrorCode = SET_ABRT;
		}
		else {
			if (memcmp(AtaCfg.uwHpaPW, (unsigned char *)(BUFFER3_BASE + 2), 32)) { //password error
				//HL[1:3]a:HL[1:3]
				if (AtaCfg.ubHpaSECounter) {
					AtaCfg.ubHpaSECounter--;
				}
				gubErrorCode = SET_ABRT;
			}
			else if (AtaCfg.ubHpaSECounter == 0) { //password correct, count zero
				//HL3c:HL3, HL2c:HL2, HL1b:HL1
				gubErrorCode = SET_ABRT;
			}
			else {//HL1:HS4, HL2:HS5, HL3:HS6
				AtaCfg.ubHpaState = ((AtaCfg.ubHpaState & (~SET_HPALOCK)) | SET_HPASET) + 0x03;
			}
		}
	}
	else {
		if ((AtaCfg.ubHpaState & 0x07) == 0x02) { //H2:H1
			if (memcmp(AtaCfg.uwHpaPW, (unsigned char *)(BUFFER3_BASE + 2), 32)) { //password error
				if (AtaCfg.ubHpaSECounter) {
					AtaCfg.ubHpaSECounter--;
				}
				gubErrorCode = SET_ABRT;
			}
			else if (AtaCfg.ubHpaSECounter == 0) { //password correct, count zero
				gubErrorCode = SET_ABRT;
			}
			else {
				AtaCfg.ubHpaState = (AtaCfg.ubHpaState & (SET_HPAFNV | SET_HPANVLBA)) | (0x01);
			}
		}
		else {//H0a:H0, H1a:H1, H3a:H3
			gubErrorCode = SET_ABRT;
		}
	}

}

void SetMaxFreeze(void)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((IDT.W82 & BIT10) == 0) {
		gubErrorCode = SET_ABRT;
		return;
	}
	if (AtaCfg.ubHpaState & SET_HPASET) {
		if (AtaCfg.ubHpaState & 0x04) { //HS4:HL4, HS5:HL5, HS6:HL6
			AtaCfg.ubHpaState = (AtaCfg.ubHpaState & (~SET_HPASET)) | (SET_HPALOCK);
		}
		else {//HS1a:HS1, HS2a:HS2, HS3a:HS3
			gubErrorCode = SET_ABRT;
		}
	}
	else if (AtaCfg.ubHpaState & SET_HPALOCK) {
		if (AtaCfg.ubHpaState & 0x04) {//HL4a:HL4, HL5a:HL5, HL6a:HL6
			gubErrorCode = SET_ABRT;
		}
		else { //HL1:HL4, HL2:HL5, HL3:HL6
			AtaCfg.ubHpaState = AtaCfg.ubHpaState + 0x03;
		}
	}
	else {
		if (((AtaCfg.ubHpaState & 0x07) == 0x01) || ((AtaCfg.ubHpaState & 0x07) == 0x02)) {//H1:H3, H2:H3
			AtaCfg.ubHpaState = (AtaCfg.ubHpaState & (SET_HPAFNV | SET_HPANVLBA)) | (0x03);
		}
		else {//H0a:H0, H3a:H3
			gubErrorCode = SET_ABRT;
		}
	}

}

void AccessibleMaxAddress()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	//defined command after ACS3, and DCO/HPA becomes obsolete command after ACS3
	if ((gubAMAXSupport == 0)
	        || (gubLBA48BitCmdSupport == 0)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))
	        || ((AtaCfg.ubSecMode & SET_SEC_LOCK) && ((HW[HW_FEATURE] == 0x0001) || (HW[HW_FEATURE] == 0x0002)))) {

		gubErrorCode = SET_ABRT;
		return;
	}

	if (ENABLE_WRITEPROTECT) {
		if (HW[HW_FEATURE] == 0x0001) {
			if (gubIsWriteProtect) {
				gubErrorCode = SET_ABRT;
				return;
			}
		}
	}

	switch ( HW[HW_FEATURE] ) {
		case 0x0000:  //get native max address ext
			HL[HL_LBA_L] = gulNativeSize - 1;
			break;

		case 0x0001:  // set accessible max address ext
			SetAccessibleMaxAddress();
			break;

		case 0x0002:  // freeze accessible max address ext
			AtaCfg.ubAmaxState |= SET_AMAX_FREEZE;
			break;

		default:
			gubErrorCode = SET_ABRT;
			break;
	}
}

void SetAccessibleMaxAddress()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	//The results of a SET ACCESSIBLE MAX ADDRESS EXT
	//command that returns command completion without an error shall persist across all resets (e.g., power-on
	//resets). The accessible max address is the native max address unless a SET ACCESSIBLE MAX ADDRESS
	//EXT command has been completed without error.
	if (AtaCfg.ubAmaxState & (SET_AMAX_FREEZE | SET_AMAX_SET)) {
		gubErrorCode = SET_ABRT;
	}
	else {
		gulLBA.All = HL[HL_LBA_L];
		if ((gulLBA.All + 1) > gulNativeSize) {
			//If the value in the LBA field is greater than the native max address, the device shall return an ID Not Found error.
			gubErrorCode = SET_IDNF;
		}
		else {
			AtaCfg.ubAmaxState |= (SET_AMAX_SET | SET_AMAX_FOREVER);
			AtaCfg.ulAmaxSize = gulLBA.All + 1;
			gulMediaSize.All = gulLBA.All + 1;
			ChangeMediaSize();
			SaveAtaConfig();
		}
	}
}

//========================================================
//
//#pragma code()  // Declare FW Code in ICCM
//
//========================================================
