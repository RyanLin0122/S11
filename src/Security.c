
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

void InitSec(UBYTE IsDLMCPowerOnReset)
{
	//UartString("Initial Security!!  \r\n");
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	if (IsDLMCPowerOnReset) {
		//這條路打假球, 只有在DLMC的Mode 3 Isp Jump之前存下ubSecMode然後 Restore回來供檢查用 , Sec Counter還是讓它回復到5次
		AtaCfg.ubSecMode = gubSecurityMode;
	}
	else {
		// clear frozen, locked and counter exceeded flag

		AtaCfg.ubSecMode &= ~(SET_SEC_FROZEN | SET_SEC_LOCK | SET_SEC_PWEXCE);

		if (AtaCfg.ubSecMode & SET_SEC_EN) {
			// User PW is Valid, lock device
			AtaCfg.ubSecMode |= SET_SEC_LOCK;
			M_DisableHWD2H_NCQ();
		}
	}
	// max Password Attempt Counter default is 5
	AtaCfg.ubSecCounter = 4;

	AtaCfg.ubLastSecurityEraseMode = 0; // Reset Last Security Erase Mode to Default
}


// if Locked mode, 0xF1/0xF5/0xF6 already Aborted in interrupt
void Security(void)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (((IDT.W82 & BIT1) == 0)
	        || (AtaCfg.uwDCOState & SET_DCO_SEC)
	        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN) && (gubCmdCode != 0xF2))) {//shall abort other than (g)SECURITY UNLOCK

		gubErrorCode = SET_ABRT;
		return;
	}

	if (ENABLE_WRITEPROTECT) {
		if (gubIsWriteProtect) {//因為bad block 太多，不夠用了，writeprotectmap會舉起來，之後某些write command下來會改成回abort
			// F1 security set password
			// F4 security Erase
			// F6 security disable password
			if ( (gubCmdCode == 0xF1) || (gubCmdCode == 0xF4) || (gubCmdCode == 0xF6)) {
				gubErrorCode = SET_ABRT;
				return;
			}

		}
	}

	HandleStopRW(0);

	if (gubCmdCode == 0xF5) { //SecurityFreezeLock
		if (AtaCfg.ubSecMode & SET_SEC_LOCK) {
			gubErrorCode = SET_ABRT;
			return;
		}

		if ( AtaCfg.ubSecMode & SET_SEC_LOCK) {
			gubErrorCode = SET_ABRT; // The Abort bit shall be set to one if the device is in locked mode
		}
		else {
			// Frozen device
			//If a SECURITY FREEZE LOCK command is issued when the device is in Frozen mode, the command executes and the device shall remain in Frozen mode.
			AtaCfg.ubSecMode |= SET_SEC_FROZEN;
		}
	}
	else if (AtaCfg.ubSecMode & SET_SEC_FROZEN) {
		gubErrorCode = SET_ABRT;
	}
	else if (gubCmdCode == 0xF3) { //SecurityErasePre
		// do nothing
	}
	else { //0xF1, 0xF2, 0xF4, 0xF6
		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate(BUFB_BASE, 512);
		}
		RWBuffer(1);
		if (gubCmdCode == 0xF1) {
			// security set password
			if ( AtaCfg.ubSecMode & SET_SEC_LOCK ) {
				// If the device is locked (see 4.22.2.2) or in Frozen mode (see 4.22.4), then the device shall return command aborted .
				gubErrorCode = SET_ABRT;
			}
			else {
				SecuritySetPass();
			}
			return;
		}

		gubPasswordHit = 0;
		if (BUFW_BASE[0] & CHK_SEC_MPW) { //Compare Master Password
			if (memcmp(AtaCfg.uwSecMasterPW, (unsigned char *)(BUFFER3_BASE + 2), 32)) {
				// mis-match
				gubErrorCode = SET_ABRT;
			}
			else {
				// match
				gubPasswordHit = MASTER_PW_CORRECT;//Master PW OK.
			}
		}
		else { //Compare User Password
			if ((AtaCfg.ubSecMode & SET_SEC_EN) == 0) { //User PW invalid
				// User PW invalid, Security is invalid
				gubErrorCode = SET_ABRT;
				return;
			}
			else if (memcmp(AtaCfg.uwSecUserPW, (unsigned char *)(BUFFER3_BASE + 2), 32)) {
				// mis-match
				gubErrorCode = SET_ABRT;
			}
			else {
				// match
				gubPasswordHit = USER_PW_CORRECT; //User PW OK.
			}
		}

		if (gubCmdCode == 0xF2) {//unlock
			SecurityUnlock();
		}
		else if (gubCmdCode == 0xF4) {//erase unit
			SecurityEraseUnit();
		}
		else { //0xF6, disable
			if ( AtaCfg.ubSecMode & SET_SEC_LOCK ) {
				//The device shall return command aborted if:
				//   a) the Security feature set is not supported;
				//   b) security is Locked (i.e., the device is in SEC4 state (see figure 16));
				//   c) security is Frozen (i.e., the device is in SEC2 state or SEC6 state (see figure 16)); or
				//   d) the password received in the data for the command does not match the password previously saved by the device.
				gubErrorCode = SET_ABRT;
			}
			else {
				// security disable password
				SecurityDisPass();
			}
		}
	}
#endif
}

void SecuritySetPass(void)  //0xF1
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if (BUFW_BASE[0] & CHK_SEC_MPW) {
		// Set Master Password
		memcpy(&AtaCfg.uwSecMasterPW, (unsigned char *)(BUFFER3_BASE + 2), 32);
		if ((BUFW_BASE[17] == 0x0000) || (BUFW_BASE[17] == 0xFFFF)) {//Master Password Identifier keep current value
		}
		else {
			// valid Master Password Identifier
			AtaCfg.uwSecMasterID = BUFW_BASE[17];
		}
	}
	else {
		// Set User Password, Security Enable
		AtaCfg.ubSecMode |= SET_SEC_EN;
		memcpy(&AtaCfg.uwSecUserPW, (unsigned char *)(BUFFER3_BASE + 2), 32);
		if (BUFW_BASE[0] & CHK_SEC_CAP_MAX) {
			// Master PW Capability : Maximum
			AtaCfg.ubSecMode |= SET_SEC_MAX;
		}
		else {
			// Master PW Capability : High
			AtaCfg.ubSecMode &= ~SET_SEC_MAX;
		}
	}
	// max Password Attempt Counter default is 5
	SaveAtaConfig();
#endif
}


void SecurityUnlock(void)  //0xF2
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	if (AtaCfg.ubSecMode & (SET_SEC_PWEXCE)) {

		// gubSecCounter is 0, Security count exceeded
		gubErrorCode = SET_ABRT;
		return;
	}

	if (gubPasswordHit == MASTER_PW_CORRECT) { //Master PW OK
		// security enable
		if (AtaCfg.ubSecMode & SET_SEC_EN) {
			if (AtaCfg.ubSecMode & SET_SEC_MAX) {
				//master password capability is maximum, command abort.

				if (AtaCfg.ubSecCounter) { //元：看不懂為何couter也要-- (答：覺得不該收到maximum的，故被當作收到錯的密碼，spec各自解讀)
					AtaCfg.ubSecCounter--;
				}
				else {
					AtaCfg.ubSecMode |= SET_SEC_PWEXCE;
				}
				gubErrorCode = SET_ABRT;
				return;
			}
			else {//master password capability is high, unlock device
			}
		}
		else { //Ref ATA8-ACS.pdf (T13/1699-D Revision 4b, 4.20.3 Table 8)
			// security disable, Master Password correct. Do nothing and return success.
			// security disable , so no lock.
			return;
		}
	}
	else if (gubPasswordHit == USER_PW_CORRECT) {//User Password OK & Security enable, master password capability is high or maximum, unlock device
	}
	else { //PW incorrect
		if (AtaCfg.ubSecMode & SET_SEC_LOCK) {//Lock mode(SEC4), counter increase
			if (AtaCfg.ubSecCounter) {
				AtaCfg.ubSecCounter--;
			}
			else {
				AtaCfg.ubSecMode |= SET_SEC_PWEXCE;
			}
		}
		else {// if unlock state, counter not affect
			// SECURITY UNLOCK commands issued when the device is unlocked have no effect on the unlock counter.
		}
		return;
	}
	// password correct, set drive unlocked
	AtaCfg.ubSecMode &= ~SET_SEC_LOCK;
	//gubSecCounter = 4;  // spec 只有說power-up, hardware reset 才設回來,這裡設4,之後下hardware reset( SSP=1),則有問題
#endif
}

void SecurityEraseUnit(void)  //0xF4
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	// 不管 Master Password Capability = high or max , master password都能用...
	if ((AtaCfg.ubSecMode & SET_SEC_PWEXCE) || (AtaCfg.ubLastCmd != 0xF3)) { // previous cmd not 0xF3
		gubErrorCode = SET_ABRT;
		return;
	}
	if ( ((AtaCfg.ubSecMode & SET_SEC_ENHANCE_ERASE) == 0) && (BUFW_BASE[0]&BIT1) ) { //問：bit5在哪裡? (答：因為沒支援enhanced mode，所以如果host下了的話就要回abort)
		// (Enhanced security erase supported = 0) &&  (ERASE MODE=Enhanced)
		gubErrorCode = SET_ABRT;
		return;
	}



	if (gubSecurityEraseNormalNeedDoRealErase) {

		AtaCfg.ubLastSecurityEraseMode = SECURITY_ERASE_ENHANCE_MODE;
	}
	else {
		if ((AtaCfg.ubSecMode & SET_SEC_ENHANCE_ERASE) && (BUFW_BASE[0]&BIT1)) {
			AtaCfg.ubLastSecurityEraseMode = SECURITY_ERASE_ENHANCE_MODE;
		}
		else { // Normal Mode
			AtaCfg.ubLastSecurityEraseMode = SECURITY_ERASE_NORMAL_MODE;
		}
	}
	// Upon successful completion, the fields in the IDENTIFY DEVICE data (see table 54) or the IDENTIFY PACKET
	// DEVICE data (see table 65) shall be updated as follows:
	//        a) word 85, bit 1 shall be cleared to zero (i.e., there is no active User password);
	//        b) word 128, bit 1 shall be cleared to zero (i.e., there is no active User password); and
	//        c) word 128, bit 8 shall be cleared to zero (i.e., the Master Password Capability is set to High).
	if (gubPasswordHit) { //PW OK
		//Security Disable, clear lock mode.
		if (ENABLE_CONTINUE_SECURITY_ERASE_AFTER_POWER_CYCLE) {
			AtaCfg.ubSecurityEraseIsDoing = 1;
			SaveAtaConfig();
		}

		TrimAll();
		gubFlushMode = BYTE_STANDBY_FLUSH;
		FlushCache();
		if (AtaCfg.ubLastSecurityEraseMode & SECURITY_ERASE_ENHANCE_MODE) {
			SecurityEraseUserData();
		}
		AtaCfg.ubLastSecurityEraseMode = 0;

		AtaCfg.ubSecMode &= ~(SET_SEC_EN | SET_SEC_LOCK | SET_SEC_MAX);
		if (ENABLE_CONTINUE_SECURITY_ERASE_AFTER_POWER_CYCLE) {
			AtaCfg.ubSecurityEraseIsDoing = 0;
		}
		SaveAtaConfig();
	}
#endif
}

void SecurityDisPass(void)  //0xF6
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	if (gubPasswordHit == MASTER_PW_CORRECT) { //Master PW OK
		// security enable
		if (AtaCfg.ubSecMode & SET_SEC_EN) {
			if (AtaCfg.ubSecMode & SET_SEC_MAX) {//master password capability is maximum, command abort.
				gubErrorCode = SET_ABRT;
				return;
			}
			else {//master password capability is high, disable user password
			}
		}
		else { //Ref ATA8-ACS.pdf (T13/1699-D Revision 4b, 4.20.3 Table 8)
			// User PW invalid, Master PW correct. Do nothing and return success.
			return;
		}
	}
	else if (gubPasswordHit == USER_PW_CORRECT) {//User PW compare OK & User PW valid, Master PW capability is high or maximum, disable user password
	}
	else { //PW incorrect
		return;
	}
	//Security Set disable
	AtaCfg.ubSecMode &= ~(SET_SEC_EN | SET_SEC_MAX);
	SaveAtaConfig();
#endif
}

void SecurityEraseUserData()
{
#if(!BURNER)
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UWORD uwUnitIndex;
	UBYTE ubSkip;
	UWORD uwScanUnit;
	UWORD uwi;
	UBYTE ubPhyPlane;
	FTarget_t uwD3Target;
	UBYTE ubPlane, ubDie, ubCEIndex;

	HandleStopRW(1);
	if (guwRUTSummaryLogNum)
		ftlRecordFailLog(FTLErrLogType, VT_SECURITY_ERASE_USERDATA_0x88, DONTCARE, DONTCARE, DONTCARE, DONTCARE);




	for (uwUnitIndex = 0; uwUnitIndex < guwFWTotalUnitNum; uwUnitIndex++) {
		if (gulVC[uwUnitIndex].B.ulValidCount == DefaultVC) {
			continue;
		}
		else {
			M_AssertError(gulVC[uwUnitIndex].B.ulValidCount != 0);
			ubSkip = 0;
			for (uwi = 0; uwi < gubTableUnitNum; uwi ++) {
				if (VT->guwTableTarget[uwi].B.uwTarget == uwUnitIndex) {
					ubSkip = 1;
					break;
				}
			}
			if (ubSkip ||
			        (VT->guwRUTTarget.B.uwTarget == uwUnitIndex) ||
			        (VT->guwInitInfoTarget.B.uwTarget == uwUnitIndex) ||
			        (VT->guwVTChildTarget.B.uwTarget == uwUnitIndex)) {

				M_AssertCriticalError(1, VT_SECURITY_ERASE_USERDATA_0x65);
			}
		}

		uwD3Target.B.uwTarget = uwUnitIndex;
		ftlEraseTarget(&uwD3Target, UnitTLCTreatment | D3, FALSE);
	}

	//erase free user unit
	for (uwUnitIndex = 2; uwUnitIndex < VT->guwFreeBlockCount_D3 + 2; uwUnitIndex++) {
		ubSkip = 0;
		uwScanUnit = guwFreeBlockTable_D3[uwUnitIndex].B.uwTarget;
		for (uwi = 0; uwi < gubTableUnitNum; uwi ++) {
			if (VT->guwTableTarget[uwi].B.uwTarget == uwScanUnit) {
				ubSkip = 1;
				break;
			}
		}
		if (ubSkip ||
		        (VT->guwRUTTarget.B.uwTarget == uwScanUnit) ||
		        (VT->guwInitInfoTarget.B.uwTarget == uwScanUnit) ||
		        (VT->guwVTChildTarget.B.uwTarget == uwScanUnit)) {

			M_AssertCriticalError(1, VT_SECURITY_ERASE_USERDATA_0x66);
		}

		uwD3Target.B.uwTarget = uwScanUnit;
		ftlEraseTarget(&uwD3Target, UnitTLCTreatment |  D3, FALSE);
	}
#if TLC
	FTarget_t uwD1Target;
	//Erase D1 Unit
	for (uwUnitIndex = 0; uwUnitIndex < VT->gubD1UnitNum; uwUnitIndex++) {
		uwD1Target.B.uwTarget = uwUnitIndex;
		ftlEraseTarget(&uwD1Target, D1, FALSE);
	}
#endif

#if Hynix
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
	flaSetToggleMode(INTERFACE_LEGACY, 0);
	flaSwitchClock(FLH_CLK_40MHz);
#endif



	/// erase all extend blks
	for (ubPlane = 0; ubPlane < gubPlanesPerPage; ubPlane++) {
		for (ubDie = 0; ubDie < gubSelDieNumber; ubDie++) {

			for (uwUnitIndex = VT->guwDieStartSFUnitIndex[ubDie]; uwUnitIndex <= VT->guwRUTUnitIndexOfLastBlock[(ubDie * gubPlanesPerPage) + ubPlane]; uwUnitIndex++) { // extend blocks range
				if (gubRUTMixPlaneEnable) {
					ubCEIndex = ubPlane >> gubBurstsPerBankLog;

					UWORD uwFBlock = M_RUT2DTo1D(ubCEIndex, uwUnitIndex * gubBurstsPerBank + (ubPlane & gubBurstsPerBankMask));
					ubPhyPlane = uwFBlock & gubBurstsPerBankMask; // physical plane

#if(TLC && !MicronFlashOnly && !TLC_BICS2&&(!(Hynix3DV6 || Hynix3DV5 || YMTC_JGS)))
					ftl_Erase_Single_Plane((uwFBlock >> gubBurstsPerBankLog), ubPhyPlane, ubCEIndex, BIT_FJOBI_TLC_TREATMENT);
#else
					ftl_Erase_Single_Plane((uwFBlock >> gubBurstsPerBankLog), ubPhyPlane, ubCEIndex, 0);
#endif
				}
				else {
					ubCEIndex = ubPlane >> gubBurstsPerBankLog;
					ubPhyPlane = ubPlane & gubBurstsPerBankMask; // physical plane

#if(TLC && !MicronFlashOnly && !TLC_BICS2&&(!(Hynix3DV6 || Hynix3DV5 || YMTC_JGS)))
					ftl_Erase_Single_Plane(M_RUT2DTo1D(ubCEIndex, uwUnitIndex * gubBurstsPerBank + ubPhyPlane), ubPhyPlane, ubCEIndex, BIT_FJOBI_TLC_TREATMENT);
#else
					ftl_Erase_Single_Plane(M_RUT2DTo1D(ubCEIndex, uwUnitIndex * gubBurstsPerBank + ubPhyPlane), ubPhyPlane, ubCEIndex, 0);
#endif
				}

				//UartString("\n Erase plane ");
				//Uart_Tx_DataHex(ubPlane);
				//UartString(" Unit ");
				//UartWordHex(M_RUT2DTo1D(ubCEIndex,uwUnitIndex*gubBurstsPerBank+uwi));

			}
		}
	}
#if Hynix
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}
	switch (gubFlashMode) {
		case FLH_MODE_TOGGLE1:
			flaSetToggleMode(INTERFACE_TOGGLE1, 0);
			break;
		case FLH_MODE_TOGGLE2:
			flaSetToggleMode(INTERFACE_TOGGLE2, 0);
			break;
	}
	flaSwitchClock(gubFlashClock);
#endif

	gubStopFTL = 0;

	//clear all RUT fail log
	if (guwRUTSummaryLogNum) {
		guwRUTLogNum = 0;
		guwRUTLogIndex = 0;
		guwRUTSummaryLogNum = 0;
		guwRUTSummaryLogIndex = 0;
		for (uwi = 0; uwi < RUT_LOGINFO_NUM; uwi++) {
			guwRUTSummaryLog[uwi].All = 0xFFFF;
			guwRUTSummaryLog[uwi].B.ubUnitD1Link = Default_D1_Unit;
			guwRUTSummaryLog[uwi].B.btIsD1Unit = 0;	//Default=0 ( 當成D3 )，uwUnit=0xFFFF也當成D3.
		}

		for (uwi = 0; uwi < RUT_LOG_NUM; uwi++) {
			gulRUTLog[uwi].All = 0x0000;
		}


	}


#endif
#endif
}

//========================================================
//
//#pragma code()  // Declare FW Code in ICCM
//
//========================================================

