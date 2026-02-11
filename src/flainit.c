#include "setup.h"
#include "Vardef.h"
#include "ftl.h"
#include <string.h>



// for CheckFlashFormat.
extern UWORD ModelTable[20];
extern UWORD SerialTable[10];
extern UWORD VerTable[4];
extern UWORD WPModelTable[10];
extern UWORD WPVerTable[4];
#if Hynix
extern UWORD garEraseFPU[3][3];
#endif

//------------------------------//
//  ubMode=0 , legacy
//  ubMode=1 , toggle


//UBYTE flaCheckFlashClock()
//{
//	return 1;
//}

//void flaInfoBlockSetting(SYSTEM_GROUP_t *InfoArray ) {}

//void  flaHynixVccQSetting_33vto18v() {}

void flaMicronRandomizer(UBYTE ubMode)	// mode 0 : off .    mode 1 : on.
{
#if MicronRandomizer
	UBYTE ubFeature[4] = {0};
	UBYTE ubFeatureCheck[MAX_CHANNEL][4] = {{0}};
	UBYTE ubChannel , ubCE;
	UBYTE ubAddr ;
	UBYTE ubCEPerChannel;

	ubAddr = 0x92;
	UBYTE ubCnt = 0;
	M_AssertError(gubPlanesPerBurst == 0);
	ubCEPerChannel = gubCENumber / gubPlanesPerBurst;
#if ENABLE_ODD_CE
	UBYTE ubASymmetricCE = 0;
	if ((gubCENumber & BIT0) && (gubPlanesPerBurst > 1)) {
		ubCEPerChannel ++;
		ubASymmetricCE = 1;
	}
#endif

	//Get feature to check randomizer status
	for (ubCE = 0; ubCE < ubCEPerChannel; ubCE++) {
		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
#if B0KB
			if (ubASymmetricCE && (ubCE == ubCEPerChannel - 1) && (ubChannel == 1)) {
				break;
			}
#endif
		RANDOMIZER_START:

#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				//FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][ubCE]);
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubCE]); // Enable selected CE
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((UWORD)BIT0 << (ubCE + (ubChannel * MAX_CE_PER_CH) ));
			}

			//----- Get Feature -----//
			flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);
			//UartString("Get:");
			//UartWordHex(ubFeatureCheck[ubChannel][0]);
			//UartString("\r\n");
			if (ubFeatureCheck[ubChannel][0] != ubMode) {
				ubFeature[0] = ubMode;
				flaSetFeature(ubChannel, ubAddr, &ubFeature[0]);
				flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);
				UBYTE ubj;
				//UartString("\nGetR: ");
				//UartWordHex(ubCE);
				for (ubj = 0; ubj < 4; ubj ++) {
					//UartWordHex(ubFeatureCheck[ubChannel][ubj]);
					if (ubFeatureCheck[ubChannel][ubj] != ubFeature[ubj]) {
						//UartString("R");
						ubCnt ++;
						if ( ubCnt > 10) {
							guoFlashUseType.B.btNoSupport = 1;
							guoFlashDefaultType.B.btNoSupport = 1;
						}
						else {
							goto RANDOMIZER_START;
						}
					}
				}
			}
		}
	}

#endif
}

#if (B0KB && !BURNER && DIS_PreRead)
void flaMicronPreRead(UBYTE ubMode)	// mode 1 : Disable .    mode 0 : Enable.
{
	UBYTE ubFeature[4] = {0};
	UBYTE ubFeatureCheck[MAX_CHANNEL][4] = {{0}};
	UBYTE ubChannel;
	UBYTE ubCE;
	UBYTE ubAddr ;
	UBYTE ubCEPerChannel;

	ubAddr = 0xDF;
	UBYTE ubCnt = 0;

	ubCEPerChannel = gubCENumber / MAX_CHANNEL;
#if ENABLE_ODD_CE
	UBYTE ubASymmetricCE = 0;
	if ((gubCENumber & BIT0) && (gubPlanesPerBurst > 1)) {
		ubCEPerChannel ++;
		ubASymmetricCE = 1;
	}
#endif

	//Get feature to check Pre read status
	for (ubCE = 0; ubCE < ubCEPerChannel; ubCE++) {
		for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
#if B0KB
			if (ubASymmetricCE && (ubCE == ubCEPerChannel - 1) && (ubChannel == 1)) {
				break;
			}
#endif
		DISPREREAD_START:

#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				//FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][ubCE]);
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubCE]); // Enable selected CE
			}
			else {
				FCONL[FCONL_FCE_ENB] = ((UWORD)BIT0 << (ubCE + (ubChannel * MAX_CE_PER_CH) ));
			}

			//----- Get Feature -----//
			flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);
			//UartString("\nGet:");
			//UartWordHex(ubFeatureCheck[ubChannel][0]);

			if ( (ubFeatureCheck[ubChannel][0] & (BIT5)) != (( ubMode << 5) )) {
				ubFeature[0] = ( ubMode << 5);
				flaSetFeature(ubChannel, ubAddr, &ubFeature[0]);
				flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);

				//UartString(" GetR: ");
				//UartWordHex(ubCE);
				//UartString(", ");
				//UartWordHex(ubChannel);
				//UartString(", ");
				//UartWordHex(ubFeatureCheck[ubChannel][0]);
				if ( (ubFeatureCheck[ubChannel][0] & (BIT5 )) != (ubFeature[0] & (BIT5 )) ) {
					//UartString("R");
					ubCnt ++;
					if ( ubCnt > 10) {
						guoFlashUseType.B.btNoSupport = 1;
						guoFlashDefaultType.B.btNoSupport = 1;
					}
					else {
						goto DISPREREAD_START;
					}
				}

			}
		}
	}
}
#endif

UBYTE  flaCheckMicronSLCIMode_CE(void)
{
#if ((!BURNER) && MicronFlashOnly)
	UBYTE ubFeature[4] = {0};
	UBYTE ubFeatureCheck[MAX_CHANNEL][4] = {{0}};
	UBYTE ubChannel, ubChCnt = 0, ubStatus;
	U32 ulChannelMap = 0, ulDelayLoop;
	_UCV REG_t * pFlaReg;
	UBYTE ubAddr ;

	// Check Channel numbers first
	for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
		// Only have to check CE0 and CE8.
		FCONL[FCONL_FCE_ENB] = (BIT0 << (ubChannel * 8));

		pFlaReg = (volatile REG_t *)gulFlashIP_RegBase[ubChannel];
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFF;		//Send 0xff
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;		//Send 0x70 to check status

		ulDelayLoop = 0;
		do {
			ubStatus = 0;
			pFlaReg[FCTLL_IO_SET].L |= FLH_PHY_IF_RESET;
			ubStatus = (UBYTE) pFlaReg[FCTLL_PIO_DAT].L;
			ulDelayLoop++;
			if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator) {
				if (ulDelayLoop > 10) {
					break;
				}
			}
			else {
				if (ulDelayLoop > 10000) {
					break;
				}
			}
		}
		while ( !(ubStatus & BIT6));
		if (ubStatus & BIT6) {
			ubChCnt++;
		}
	}

	for ( ubChannel = 0; ubChannel < ubChCnt ; ubChannel++) {
		//for ( ubChannel = 0; ubChannel < 2 ; ubChannel++) {
		ulChannelMap |= (BIT0 << (ubChannel * 8));
	}

	UBYTE ubCnt = 0;

	//Switch to SLC mode from MLC.
#if B0KB
	ubFeature[0] = 0x04;	// 00 SLC, 02 MLC, 04 TLC.
#else
	ubFeature[0] = 0x02;	// 00 SLC, 02 MLC, 04 TLC.
#endif
	ubFeature[1] = 0x01;	// Default value.
	ubFeature[2] = 0x00;	// Default value.
	ubFeature[3] = 0x00;	// Default value.

	ubAddr = 0x91;

	//Get feature to check SLC mode
	//UartString("Before set feature\r\n");
#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][0]);
		FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][0]);
	}
	else {
		FCONL[FCONL_FCE_ENB] = ulChannelMap;
	}

	for (ubChannel = 0; ubChannel < ubChCnt; ubChannel++) {
		//for ( ubChannel = 0; ubChannel < 2 ; ubChannel++) {
	SET_MLC_START:
		//----- Get Feature -----//
		flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);
		//UartString("Get:");
		//UartWordHex(ubFeatureCheck[ubChannel][0]);
		//UartString("\r\n");
		if (ubFeatureCheck[ubChannel][0] == 0x0) {
			flaSetFeature(ubChannel, ubAddr, &ubFeature[0]);
			flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);
			UBYTE ubj;
			//UartString("\nGetM:");
			for (ubj = 0; ubj < 4; ubj ++) {
				//UartWordHex(ubFeatureCheck[ubChannel][ubj]);
				if (ubFeatureCheck[ubChannel][ubj] != ubFeature[ubj]) {
					//UartString("M");
					ubCnt ++;
					if (ubCnt > 10) {
						guoFlashUseType.B.btNoSupport = 1;
						guoFlashDefaultType.B.btNoSupport = 1;
					}
					else {
						goto SET_MLC_START;
					}
				}
			}
		}
	}

#endif
	return 1;
}


UBYTE  flaSetToggleMode_CE(UBYTE ubMode , UBYTE ubForceSetFeature , UBYTE ubCE)
{
	return 1;
}



void InitUart(void)
{

}
void flaCEControl(UBYTE ubChannel, UBYTE ubFlashCE, UBYTE ubEnable)
{
	if (ubEnable) {
		FCONL[FCONL_FCE_ENB]  = 0;
		FCONL[FCONL_FCE_ENB]  |= BIT0 << (ubChannel * 8 + ubFlashCE);
	}
	else {
		FCONL[FCONL_FCE_ENB]  = 0;
	}
}




void flaSetFlashAddressType(void)
{
	UBYTE ubSpareDummyLength = 0;

	//Address Rule
	//FL_FCTL_ALU_SEL_All = 2;

#define ALU_RULE(source, target, length) (source << 10 | target << 4 | length )
	if (guoFlashUseType.B.btFlash16k) {
		if (guoFlashUseType.B.bt8LC) {
			if ((guoFlashUseType.B.btToshiba) || (guoFlashUseType.B.btHynix) /* TODO: eric, confirm... */) {

				if (guoFlashUseType.B.btFourPlane) {
					if (ENABLE_iFSA) {
#if Hynix3DV6 || Hynix3DV5
						if (guoFlashUseType.B.btHynix3DV6) {
							// Source = 13 Target = 16 Length = 9
							IRAM2B[(ALU_BASE_0 >> 1) + 0] = ALU_RULE(13, 16, 9);
							// Source = 22 Target = 25 Length = 2
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = ALU_RULE(22, 25, 2);
							// Source = 24 Target = 26 Length = 11
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = ALU_RULE(24, 27, 11);
							// Source = 33 Target = 38 Length = 2
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = ALU_RULE(35, 38, 2);
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {		// A2 fast page array
								// Source = 13 Target = 16 Length = 9
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = ALU_RULE(13, 16, 9);
								// Source = 22 Target = 25 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = ALU_RULE(22, 25, 2);
								// Source = 24 Target = 26 Length = 11
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = ALU_RULE(24, 27, 11);
								// Source = 33 Target = 38 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = ALU_RULE(35, 38, 2);
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;

							}

						}
						else if (guoFlashUseType.B.btHynix3DV5) {

							// Source = 13 Target = 16 Length = 9
							IRAM2B[(ALU_BASE_0 >> 1) + 0] = ALU_RULE(13, 16, 9);
							// Source = 22 Target = 25 Length = 2
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = ALU_RULE(22, 25, 2);
							// Source = 24 Target = 26 Length = 11
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = ALU_RULE(24, 27, 10);
							// Source = 33 Target = 38 Length = 2
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = ALU_RULE(34, 37, 2);
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {		// A2 fast page array
								// Source = 13 Target = 16 Length = 9
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = ALU_RULE(13, 16, 9);
								// Source = 22 Target = 25 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = ALU_RULE(22, 25, 2);
								// Source = 24 Target = 26 Length = 11
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = ALU_RULE(24, 27, 10);
								// Source = 33 Target = 38 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = ALU_RULE(34, 37, 2);
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						else {
							flaLEDBlink(1);
						}
#endif
					}
					else {
						flaLEDBlink(1);
					}



				}
				else if (guoFlashUseType.B.btTwoPlane) {
					if (ENABLE_iFSA) {	// iFSA

#if Hynix3DV6 || Hynix3DV5
						if (guoFlashUseType.B.btHynix3DV6) {
							// Source = 13 Target = 16 Length = 9
							IRAM2B[(ALU_BASE_0 >> 1) + 0] = ALU_RULE(13, 16, 9);
							// Source = 22 Target = 25 Length = 2
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = ALU_RULE(23, 25, 1);
							// Source = 24 Target = 26 Length = 11
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = ALU_RULE(24, 26, 12);
							// Source = 33 Target = 38 Length = 2
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = ALU_RULE(36, 38, 2);
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {		// A2 fast page array
								// Source = 13 Target = 16 Length = 9
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = ALU_RULE(13, 16, 9);
								// Source = 22 Target = 25 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = ALU_RULE(23, 25, 1);
								// Source = 24 Target = 26 Length = 11
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = ALU_RULE(24, 26, 12);
								// Source = 33 Target = 38 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = ALU_RULE(36, 38, 2);
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;

							}

						}

						else if (guoFlashUseType.B.btHynix3DV5) {

							// Source = 13 Target = 16 Length = 9
							IRAM2B[(ALU_BASE_0 >> 1) + 0] = ALU_RULE(13, 16, 9);
							// Source = 22 Target = 25 Length = 2
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = ALU_RULE(23, 25, 1);
							// Source = 24 Target = 26 Length = 11
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = ALU_RULE(24, 26, 11);
							// Source = 33 Target = 38 Length = 2
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = ALU_RULE(35, 37, 2);
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {		// A2 fast page array
								// Source = 13 Target = 16 Length = 9
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = ALU_RULE(13, 16, 9);
								// Source = 22 Target = 25 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = ALU_RULE(23, 25, 1);
								// Source = 24 Target = 26 Length = 11
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = ALU_RULE(24, 26, 11);
								// Source = 33 Target = 38 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = ALU_RULE(35, 37, 2);
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;

							}
						}
						else {
							flaLEDBlink(1);
						}
#else
#if TSB_BICS4_SUPPORT
						if (guoFlashUseType.B.btToshibaBiCs4) {	//WL 383
							// Source = 13 Target = 16 Length = 9
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x3509;
							// Source = 23 Target = 25 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5D91;
							// Source = 24 Target = 26 Length = 10
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x61AA;
							// Source = 34 Target = 36 Length = 4
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8A44;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 13 Target = 16 Length = 9
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3509;
								// Source = 23 Target = 25 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5D91;
								// Source = 24 Target = 26 Length = 10
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x61AA;
								// Source = 34 Target = 36 Length = 4
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8A44;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						else
#endif
						{
							// Source = 13 Target = 16 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x3508;
							// Source = 23 Target = 24 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5D81;
							// Source = 24 Target = 25 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x6198;
							// Source = 32 Target = 33 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8217;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 13 Target = 16 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3508;
								// Source = 23 Target = 24 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5D81;
								// Source = 24 Target = 25 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x6198;
								// Source = 32 Target = 33 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8217;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
#endif
					}
					else {  //eFSA
#if TSB_BICS4_SUPPORT
						if (guoFlashUseType.B.btToshibaBiCs4) {	//WL 383
							// Source = 5 Target = 16 Length = 9
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x1509;
							// Source = 15 Target = 25 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3D91;
							// Source = 16 Target = 26 Length = 10
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x41AA;
							// Source = 24 Target = 36 Length = 4
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6A44;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 5 Target = 16 Length = 9
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x1509;
								// Source = 15 Target = 25 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x3D91;
								// Source = 16 Target = 26 Length = 10
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x41AA;
								// Source = 24 Target = 36 Length = 4
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x6A44;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						else
#endif
						{
							// Source = 5 Target = 16 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x1508;
							// Source = 15 Target = 24 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3D81;
							// Source = 16 Target = 25 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x4198;
							// Source = 24 Target = 33 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6217;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 5 Target = 16 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x1508;
								// Source = 15 Target = 24 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x3D81;
								// Source = 16 Target = 25 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x4198;
								// Source = 24 Target = 33 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x6217;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
					}
#if TSB_BICS4_SUPPORT
					if (guoFlashUseType.B.btToshibaBiCs4) {
						gubTwoPlaneBit = 25;
					}
					else
#endif
					{
						gubTwoPlaneBit = 24;
					}
				}
			}
#if MicronFlashOnly
			else if (guoFlashUseType.B.btIntelMicron) {
				// Micron TLC
				//1 not finished yet
				if (guoFlashUseType.B.btTwoPlane) {
					if (ENABLE_iFSA) {	// iFSA
						// Source = 11 Target = 16 Length = 11 //flash page size, N flash page
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x2D0B;
						// Source = 23 Target = 27 Length = 1
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5DB1;
						// Source =24 Target = 28 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x61C8;
						// Source = 32 Target = 36 Length = 4
						IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8244;
						FCONL[FCONL_ADR_GEN_STOP] = 3;
						//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
						if (guoFlashUseType.B.btA2Cmd) {        // SLC page array
							// Source = 11 Target = 16 Length = 11 //flash page size, N flash page
							IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x2D0B;
							// Source = 23 Target = 27 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5DB1;
							// Source =24 Target = 28 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x61C8;
							// Source = 32 Target = 36 Length = 4
							IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8244;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;
						}
					}
					else {
						//not modified
						// Source = 3 Target = 16 Length = 9 //flash page size, N flash page
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x0D09;
						// Source = 12 Target = 24 Length = 4
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3184;
						// Source = 16 Target = 28 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x41C8;
						// Source = 24 Target = 36 Length = 2
						IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6242;
						FCONL[FCONL_ADR_GEN_STOP] = 3;
						//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
					}
				}
				else if (guoFlashUseType.B.btFourPlane) {
					if (ENABLE_iFSA) {	// iFSA
						// Source = 11 Target = 16 Length = 9 //flash page size, N flash page
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x2D09;
						// Source = 20 Target = 25 Length = 4
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5194;
						// Source =24 Target = 29 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x61D8;
						// Source = 32 Target = 37 Length = 3
						IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8253;
						FCONL[FCONL_ADR_GEN_STOP] = 3;
						//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
						if (guoFlashUseType.B.btA2Cmd) {        // SLC page array
							// Source = 11 Target = 16 Length = 9 //flash page size, N flash page
							IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x2D09;
							// Source = 20 Target = 25 Length = 4
							IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5194;
							// Source =24 Target = 29 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x61D8;
							// Source = 32 Target = 37 Length = 3
							IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8253;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;
						}
					}
					else {
						// Source = 3 Target = 16 Length = 9 //flash page size, N flash page
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x0D09;
						// Source = 12 Target = 24 Length = 4
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3184;
						// Source = 16 Target = 28 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x41C8;
						// Source = 24 Target = 36 Length = 2
						IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6242;
						FCONL[FCONL_ADR_GEN_STOP] = 3;
						//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
					}
				}
			}
#endif
#if YMTC
			else if (guoFlashUseType.B.btYMTC) {
				if (guoFlashUseType.B.btTwoPlane) {
					if (ENABLE_iFSA) {	// iFSA
						// Source = 11 Target = 16 Length = 11 //flash page size, N flash page
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x2D0B;
						// Source = 23 Target = 27 Length = 1
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5DB1;
						// Source =24 Target = 28 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x61C8;
						// Source = 32 Target = 36 Length = 4
						IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8244;
						FCONL[FCONL_ADR_GEN_STOP] = 3;
						//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
						if (guoFlashUseType.B.btA2Cmd) {        // SLC page array
							// Source = 11 Target = 16 Length = 11 //flash page size, N flash page
							IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x2D0B;
							// Source = 23 Target = 27 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5DB1;
							// Source =24 Target = 28 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x61C8;
							// Source = 32 Target = 36 Length = 4
							IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8244;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;
						}
					}
					else {
						flaLEDBlink(1);
					}
				}

			}
#endif
			else {

			}
		}
		else if (guoFlashUseType.B.bt4LC) {
			if (guoFlashUseType.B.btToshiba) {
				if (guoFlashUseType.B.btFourPlane) {
					if (guoFlashUseType.B.bt256Page) {
						if (ENABLE_iFSA) {	// iFSA
							// Source = 13 Target = 16 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x3508;
							// Source = 22 Target = 24 Length = 2
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5982;
							// Source = 24 Target = 26 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x61A8;
							// Source = 32 Target = 34 Length = 6
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8226;
							FCONL[FCONL_ADR_GEN_STOP] = 3;
							//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
							// set Addr gen pointer
							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 13 Target = 17 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3517;
								// Source = 22 Target = 24 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5982;
								// Source = 24 Target = 26 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x61A8;
								// Source = 32 Target = 34 Length = 6
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8226;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						else {	// eFSA
							// Source = 5 Target = 16 Length = 8 //flash page size, N flash page, A2
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x1508;
							// Source = 14 Target = 24 Length = 2  //X-plane
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3982;
							// Source = 16 Target = 26 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x41A8;
							// Source = 24 Target = 34 Length = 6
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6226;
							FCONL[FCONL_ADR_GEN_STOP] = 3;
							//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
							// set Addr gen pointer
							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 5 Target = 17 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x1517;
								// Source = 14 Target = 24 Length = 2
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x3982;
								// Source = 16 Target = 26 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x41A8;
								// Source = 24 Target = 34 Length = 6
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x6226;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						//gubTwoPlaneBit = 24;
					}
				}
				else if (guoFlashUseType.B.btTwoPlane) {
					if (guoFlashUseType.B.bt256Page) {
						if (ENABLE_iFSA) {	// iFSA
							// Source = 13 Target = 16 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x3508;
							// Source = 23 Target = 24 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5D81;
							// Source = 24 Target = 25 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x6198;
							// Source = 32 Target = 33 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8217;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 13 Target = 17 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3517;
								// Source = 23 Target = 24 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5D81;
								// Source = 24 Target = 25 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x6198;
								// Source = 32 Target = 33 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8217;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						else {  //eFSA
							// Source = 5 Target = 16 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x1508;
							// Source = 15 Target = 24 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3D81;
							// Source = 16 Target = 25 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x4198;
							// Source = 24 Target = 33 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6217;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 5 Target = 17 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x1517;
								// Source = 15 Target = 24 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x3D81;
								// Source = 16 Target = 25 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x4198;
								// Source = 24 Target = 33 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x6217;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						gubTwoPlaneBit = 24;
					}
					else { // 128 page
						if (ENABLE_iFSA) {	// iFSA
							// Source = 13 Target = 16 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x3507;
							// Source = 23 Target = 23 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5D71;
							// Source = 24 Target = 24 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x6188;
							// Source = 32 Target = 32 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8208;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 13 Target = 17 Length = 6
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3516;
								// Source = 23 Target = 23 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5D71;
								// Source = 24 Target = 24 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x6188;
								// Source = 32 Target = 32 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8208;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						else {  //eFSA
							// Source = 5 Target = 16 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x1507;
							// Source = 15 Target = 23 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3D71;
							// Source = 16 Target = 24 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x4188;
							// Source = 24 Target = 32 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6208;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 5 Target = 17 Length = 6
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x1516;
								// Source = 15 Target = 23 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x3D71;
								// Source = 16 Target = 24 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x4188;
								// Source = 24 Target = 32 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x6208;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
					}
				}
				else {//one plane
					if (guoFlashUseType.B.bt256Page) {
						if (ENABLE_iFSA) {	// iFSA
							// Source = 13 Target = 16 Length = 8 //flash page size, N flash page, A2
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x3508;
							// Source = 24 Target = 24 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x6188;
							// Source = 32 Target = 32 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x8208;
							// set Addr gen pointe
							FCONL[FCONL_ADR_GEN_STOP] = 2;
							//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
							// set Addr gen pointer
							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 5 Target = 17 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3517;
								// Source = 16 Target = 24 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x6188;
								// Source = 24 Target = 32 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x8208;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 2;
							}
						}
						else {
							// Source = 5 Target = 16 Length = 8 //flash page size, N flash page, A2
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x1508;
							// Source = 16 Target = 24 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x4188;
							// Source = 24 Target = 32 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x6208;
							// set Addr gen pointe
							FCONL[FCONL_ADR_GEN_STOP] = 2;
							//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
							// set Addr gen pointer
							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 5 Target = 17 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x1517;
								// Source = 16 Target = 24 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x4188;
								// Source = 24 Target = 32 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x6208;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 2;
							}
						}
					}
				}
			}
			else if (guoFlashUseType.B.btHynix) {
				if (guoFlashUseType.B.btTwoPlane) {
				}
			}
#if MicronFlashOnly
			else if (guoFlashUseType.B.btIntelMicron) {
				if (guoFlashUseType.B.btFourPlane && guoFlashUseType.B.bt1024Page) {
					if (ENABLE_iFSA) {	// iFSA
						// Source = 12 Target = 16 Length = 8 //flash page size, N flash page
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x3108;
						// Source = 20 Target = 24 Length = 4
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5184;
						// Source =24 Target = 28 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x61C8;
						// Source = 32 Target = 36 Length = 4
						IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8244;
						FCONL[FCONL_ADR_GEN_STOP] = 3;
						//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
						if (guoFlashUseType.B.btA2Cmd) {        // SLC page array
							// Source = 12 Target = 16 Length = 8 //flash page size, N flash page
							IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3108;
							// Source = 20 Target = 24 Length = 4
							IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5184;
							// Source =24 Target = 28 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x61C8;
							// Source = 32 Target = 36 Length = 4
							IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8244;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;
						}
					}
					else {
						// Source = 4 Target = 16 Length = 8 //flash page size, N flash page
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x1108;
						// Source = 12 Target = 24 Length = 4
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3184;
						// Source = 16 Target = 28 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x41C8;
						// Source = 24 Target = 36 Length = 4
						IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6244;
						FCONL[FCONL_ADR_GEN_STOP] = 3;
						//FCONL_ADR_GEN |= FTA_EQUAL_FSA;
					}
				}
				else {
					M_AssertError(1);
				}
			}
#endif
			else {
				M_AssertError(1);
			}
		}
	}
	else if (guoFlashUseType.B.btFlash8k) {
		if (guoFlashUseType.B.bt8LC) {}
		else if (guoFlashUseType.B.bt4LC) {
			if (guoFlashUseType.B.btToshiba) {
				if (guoFlashUseType.B.bt512Page) {
					// 以後有再加...
					M_AssertError(1);
				}
				else if (guoFlashUseType.B.bt256Page) {
					if (guoFlashUseType.B.btFourPlane) {

					}
					else if (guoFlashUseType.B.btTwoPlane) {
						if (ENABLE_iFSA) {	// iFSA
							// Source = 12 Target = 16 Length = 8 //flash page size, N flash page, A2
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x3108;
							// Source = 23 Target = 24 Length = 1  //X-plane
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5D81;
							// Source = 24 Target = 25 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x6198;
							// Source = 32 Target = 33 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8217;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 12 Target = 17 Length = 7 //flash page size, N flash page, A2
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3117;
								// Source = 23 Target = 24 Length = 1  //X-plane
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5D81;
								// Source = 24 Target = 25 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x6198;
								// Source = 32 Target = 33 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8217;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						else {
							// Source = 4 Target = 16 Length = 8 //flash page size, N flash page, A2
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x1108;
							// Source = 15 Target = 24 Length = 1  //X-plane
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3D81;
							// Source = 16 Target = 25 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x4198;
							// Source = 24 Target = 33 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6217;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 4 Target = 17 Length = 7 //flash page size, N flash page, A2
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x1117;
								// Source = 15 Target = 24 Length = 1  //X-plane
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x3D81;
								// Source = 16 Target = 25 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x4198;
								// Source = 24 Target = 33 Length = 7
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x6217;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
					}
					else { //one plane
						if (ENABLE_iFSA) {	// iFSA
							// Source = 12 Target = 16 Length = 8 //flash page size, N flash page, A2
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x3108;
							// Source = 24 Target = 24 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x6188;
							// Source = 32 Target = 32 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x8208;

							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 2;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 12 Target = 17 Length = 7 //flash page size, N flash page, A2
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3117;
								// Source = 24 Target = 24 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x6188;
								// Source = 32 Target = 32 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x8208;

								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 2;
							}
						}
						else {
							// Source = 4 Target = 16 Length = 8 //flash page size, N flash page, A2
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x1108;
							// Source = 16 Target = 24 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x4188;
							// Source = 24 Target = 32 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x6208;

							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 2;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 4 Target = 17 Length = 7 //flash page size, N flash page, A2
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x1117;
								// Source = 16 Target = 24 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x4188;
								// Source = 24 Target = 32 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x6208;

								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 2;
							}
						}
					}
				}
				else {// 128 page
					if (guoFlashUseType.B.btTwoPlane) {
						if (ENABLE_iFSA) {	// iFSA
							// Source = 12 Target = 16 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x3107;
							// Source = 23 Target = 23 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5D71;
							// Source = 24 Target = 24 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x6188;
							// Source = 32 Target = 32 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8208;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 12 Target = 17 Length = 6
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x3116;
								// Source = 23 Target = 23 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x5D71;
								// Source = 24 Target = 24 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x6188;
								// Source = 32 Target = 32 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x8208;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
						else {  //eFSA
							// Source = 4 Target = 16 Length = 7
							IRAM2B[(ALU_BASE_0 >> 1)] = 0x1107;
							// Source = 15 Target = 23 Length = 1
							IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3D71;
							// Source = 16 Target = 24 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x4188;
							// Source = 24 Target = 32 Length = 8
							IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6208;
							// set Addr gen pointer
							FCONL[FCONL_ADR_GEN_STOP] = 3;

							if (guoFlashUseType.B.btA2Cmd) {        // A2 fast page array
								// Source = 4 Target = 17 Length = 6
								IRAM2B[(ALU_BASE_0 >> 1) + 8] = 0x1116;
								// Source = 15 Target = 23 Length = 1
								IRAM2B[(ALU_BASE_0 >> 1) + 9] = 0x3D71;
								// Source = 16 Target = 24 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 10] = 0x4188;
								// Source = 24 Target = 32 Length = 8
								IRAM2B[(ALU_BASE_0 >> 1) + 11] = 0x6208;
								// set Addr gen pointer
								FCONL[FCONL_ADR_GEN_STOP] = 3;
							}
						}
					}
					else {}
				}
			}
			else if (guoFlashUseType.B.btHynix || guoFlashUseType.B.btIntelMicron) {
				// Hynix A-die    or Micron L84A L74A
				if (guoFlashUseType.B.btTwoPlane) {
					if (guoFlashUseType.B.bt512Page) {
						M_AssertError(1);
					}
					else if (guoFlashUseType.B.bt256Page) {
						// set Addr gen pointer
					}
					else {
						M_AssertError(1);
					}
				}
				else {}
			}
		}
		else if (guoFlashUseType.B.btSLC) {
			// Source = 4 Target = 16 Length = 7
			// Source = 15 Target = 23 Length = 1
			// Source = 16 Target = 24 Length = 8
			// Source = 24 Target = 32 Length = 4
			// set Addr gen pointer
			// Source = 5 Target = 16 Length = 8
			if (guoFlashUseType.B.btFourPlane) {
			}
			else if (guoFlashUseType.B.btTwoPlane) {
				if (guoFlashUseType.B.bt128Page) {
					if (ENABLE_iFSA) {
						// Source = 12 Target = 16 Length = 7
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x3107;
						// Source = 23 Target = 23 Length = 1
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x5D71;
						// Source = 24 Target = 24 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x6188;
						// Source = 32 Target = 32 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x8208;
						// set Addr gen pointer
						FCONL[FCONL_ADR_GEN_STOP] = 3;
					}
					else {
						// Source = 4 Target = 16 Length = 7 //flash page size, N flash page, A2
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x1107;
						// Source = 15 Target = 23 Length = 1  //X-plane
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x3D71;
						// Source = 16 Target = 24 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x4188;
						// Source = 24 Target = 32 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 3] = 0x6208;
						FCONL[FCONL_ADR_GEN_STOP] = 3;
						gubTwoPlaneBit = 23;
					}
				}
			}
			else { // one plane
				if (guoFlashUseType.B.bt128Page) {
					if (ENABLE_iFSA) {
						// Source = 12 Target = 16 Length = 7 //flash page size, N flash page, A2
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x3107;
						// Source = 24 Target = 23 Length = 8  //X-plane
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x6178;
						// Source = 32 Target = 31 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x81F8;

						FCONL[FCONL_ADR_GEN_STOP] = 2;
					}
					else {
						// Source = 4 Target = 16 Length = 7 //flash page size, N flash page, A2
						IRAM2B[(ALU_BASE_0 >> 1)] = 0x1107;
						// Source = 16 Target = 23 Length = 8  //X-plane
						IRAM2B[(ALU_BASE_0 >> 1) + 1] = 0x4178;
						// Source = 24 Target = 31 Length = 8
						IRAM2B[(ALU_BASE_0 >> 1) + 2] = 0x61F8;

						FCONL[FCONL_ADR_GEN_STOP] = 2;
					}
				}
			}

		}
	}
	else if (guoFlashUseType.B.btFlash4k) {
		if (guoFlashUseType.B.btSLC) {
			if (guoFlashUseType.B.btTwoPlane) {
				if (gulPhyBlocksPerCE == 4096) {
				}
			}
			else {
			}
		}
	}
	else if ( guoFlashUseType.B.btFlash2k ) {}
}

UBYTE ftlCheckFlashFormat_SystemSet(void)
{
#if(!RDT_BURNER)
	UBYTE ubCEIndex;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	//UBYTE ubPlanePtr = 0;
	U32 ulTotalPhyUnitNum;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;

	UWORD uwBlockPTR[MAX_CHANNEL];
	UWORD uwScanCount[MAX_CHANNEL];
	UBYTE ubScanMode;
	UBYTE ubStopScan = 0;
	UBYTE ubGetSysBlk_CE = 0xFF;
	UBYTE ubi, ubj, ubk;
	UBYTE ubTotalSystemDBTBlockNumber[MAX_CHANNEL][2] = {{0, 0}, {0, 0}};
	UWORD uwSystemDBTBlock_FirstScan_PerCh[MAX_CHANNEL][2][MAX_SYSDBT_NUMBER];
	U32 ulSystemDBTVersion_PerCh[MAX_CHANNEL][2][MAX_SYSDBT_NUMBER];
	UBYTE ubDBTOffsetInSystem_PerCh[MAX_CHANNEL][MAX_SYSDBT_NUMBER];

	if ((gubExpandCE_initial == 0) || (!BURNER)) {
		if (ENABLE_PSEUDO_CE && guoFlashUseType.B.btMultiDie) {
			gubILMode = INTERLEAVE_8 ;//IL8
			ulTotalPhyUnitNum = gulTotalPhyUnitNum;
			gulPhyBlocksPerCE = ulTotalPhyUnitNum;

			flaScanCE_And_Remapping();
			M_DisableAllFlashCE();

			if (guoFlashUseType.B.btToggle == 1) {
				gubFlashMode_CE[0] = 0xFF;
				gubFlashMode_CE[1] = 0xFF;
			}

			ulTotalPhyUnitNum =  (ulTotalPhyUnitNum * gubTotalGroups); //one group的TotalPhyUnitNum * TotalGroups
			if (ENABLE_SANDISK_1ZNM_SPF) {
				ulTotalPhyUnitNum -= gubHideCE * (gulTotalPhyUnitNum / gubDieNumber);
			}
			// gulTotalPhyUnitNum&gulPhyBlocksPerCE use 2k/128 to calculate before
			if (guoFlashUseType.B.btFlash16k) {
				FCONL[FCONL_PAGE_CFG] = PHYSICAL_16KPAGE;

				if (guoFlashUseType.B.bt4LC) {
					if (guoFlashUseType.B.bt1024Page) {
						ulTotalPhyUnitNum /= 64; // 64 = (16K/2K)*(1024pages/128pages) = 8*8
						gulPhyBlocksPerCE /= 64;
					}
					else if (guoFlashUseType.B.bt512Page) {

						ulTotalPhyUnitNum /= 32; // 32 = (16K/2K)*(512pages/128pages) = 8*4
						gulPhyBlocksPerCE /= 32;
					}
					else if (guoFlashUseType.B.bt256Page) {
						// must 256page
						ulTotalPhyUnitNum /= 16;
						gulPhyBlocksPerCE /= 16;
					}
					else {
						M_AssertError(1);
					}
				}
				else if (guoFlashUseType.B.bt8LC) {
					if (guoFlashUseType.B.bt258Page) { /* A19nm TLC */
#if Hynix
						ulTotalPhyUnitNum >>= 4;
						gulPhyBlocksPerCE >>= 4;
#else
						ulTotalPhyUnitNum >>= 3;
						gulPhyBlocksPerCE >>= 3;
#endif
					}
					else if (guoFlashUseType.B.bt384Page) { /* 1Znm TLC */
						ulTotalPhyUnitNum >>= 5;
						gulPhyBlocksPerCE >>= 5;
					}
					else if (guoFlashUseType.B.bt576Page || guoFlashUseType.B.bt768Page) { /* BiCs2 TLC and BiCs3 TLC*/
						ulTotalPhyUnitNum >>= 6;
						gulPhyBlocksPerCE >>= 6;
					}
#if TSB_BICS4_SUPPORT
					else if (guoFlashUseType.B.btToshibaBiCs4) { /* BiCs4 TLC*/
						ulTotalPhyUnitNum >>= 7;	// 72 = (16K/2K)*(1152pages/128pages) = 8*9
						gulPhyBlocksPerCE >>= 7;
					}
#endif
					else if (guoFlashUseType.B.btIntelMicronB0KB) { /* B0KB */
						ulTotalPhyUnitNum >>= 7;	// (16K/2K)*(1536 pages/128 pages) = 8*12 =96 make it to 128.
						gulPhyBlocksPerCE >>= 7;
					}
					else {
						M_AssertError(1);
					}
				}
			}
			else if (guoFlashUseType.B.btFlash8k) {
				FCONL[FCONL_PAGE_CFG] = PHYSICAL_8KPAGE;
				if (guoFlashUseType.B.bt512Page) {
					M_AssertError(1);
				}
				else if (guoFlashUseType.B.bt256Page) {
					ulTotalPhyUnitNum /= 8; // 8 = (8K/2K)*(256pages/128pages) = 4*2
					gulPhyBlocksPerCE /= 8;
				}
				else {
					ulTotalPhyUnitNum /= 4;
					gulPhyBlocksPerCE /= 4;
				}
			}
			else if (guoFlashUseType.B.btFlash4k) {
				FCONL[FCONL_PAGE_CFG] = PHYSICAL_4KPAGE;

				if (guwPagesPerUnit == 64) {
					// 1 = (4K/2K)*(64pages/128pages) = 2*0.5 = 1
				}
				else { //128 pages
					ulTotalPhyUnitNum /= 2; // 2 = (4K/2K)*(128pages/128pages) = 2*1
					gulPhyBlocksPerCE /= 2;
				}
			}
			else {
				//fail
			}

			guwDieBlockShift = 0;
			if (guoFlashUseType.B.btMultiDie) {
				if (ENABLE_MULTI_DIE) {
					if ((guoFlashUseType.B.btToshiba) || (guoFlashUseType.B.btHynix) || (guoFlashUseType.B.btIntelMicron) || (guoFlashUseType.B.btYMTC)) {
						guwDieBlockShift = (gulPhyBlocksPerCE / gubDieNumber) << 1;
#if YMTC_JGS
						guwDieBlockShift = 4096;
#elif Hynix3DV6
						if (guoFlashDefaultType.B.btHynix3DV5) {
							guwDieBlockShift = 4096;
						}
						else {
							guwDieBlockShift = 8192;
						}
#endif
					}
					//else if (guoFlashUseType.B.btIntelMicron) {
					//	guwDieBlockShift = 4096;
					//}
					else {
						M_AssertError(1);
					}
				}
				else {
					ulTotalPhyUnitNum /= 2;
				}
			}


			gubTotalGroups *= gubDieNumber; //前面得CH下的group數, CE內multiDie為不同group, 所以再乘DieNumber
			if (ENABLE_SANDISK_1ZNM_SPF) {
				gubTotalGroups -= gubHideCE;
			}
			ulTotalPhyUnitNum += (U32)(guwExtendBlocks * gubTotalGroups);
			gulPhyBlocksPerCE += (guwExtendBlocks * gubDieNumber );
			//Check FourPlane/TwoPlane Status
			if (guoFlashUseType.B.btFourPlane) {
				ulTotalPhyUnitNum /= 4;
				guwDieBlockShift /= 4;
			}
			else if (guoFlashUseType.B.btTwoPlane) {
				ulTotalPhyUnitNum /= 2;
				guwDieBlockShift /= 2;
			}
			else {
			}
			gubDieBlockShiftLog = flaCheckLogValue((U32)guwDieBlockShift);

			//Check Interleave Status

			if (gubILMode == INTERLEAVE_8 ) {//IL8
				if ((gubTotalGroups >= (8 * gubDieNumber))) { //IL4 && unit must large than 2048 for GRTarget
					ulTotalPhyUnitNum /= 8;
				}
				//if ((gubTotalGroups>=(4*gubDieNumber))&&((gulTotalPhyUnitNum/4)>=2048)) {     //IL4 && unit must large than 2048 for GRTarget
				else if ((gubTotalGroups >= (4 * gubDieNumber))) { //IL4 && unit must large than 2048 for GRTarget
					ulTotalPhyUnitNum /= 4;
				}
				//else if ((gubTotalGroups>=(2*gubDieNumber))&&((gulTotalPhyUnitNum/2)>=2048)) { //IL2
				else if ((gubTotalGroups >= (2 * gubDieNumber))) { //IL2
					ulTotalPhyUnitNum /= 2;
				}
				else { //noIL
					//gubPlanesPerPage *=1;
				}
			}
			else if (gubILMode == INTERLEAVE_4 ) {//IL4
				//if ((gubTotalGroups>=(4*gubDieNumber))&&((gulTotalPhyUnitNum/4)>=2048)) {     //IL4 && unit must large than 2048 for GRTarget
				if ((gubTotalGroups >= (4 * gubDieNumber))) { //IL4 && unit must large than 2048 for GRTarget
					ulTotalPhyUnitNum /= 4;
				}
				//else if ((gubTotalGroups>=(2*gubDieNumber))&&((gulTotalPhyUnitNum/2)>=2048)) { //IL2
				else if ((gubTotalGroups >= (2 * gubDieNumber))) { //IL2
					ulTotalPhyUnitNum /= 2;
				}
				else { //noIL
					//gubPlanesPerPage *=1;
				}
			}
			else if (gubILMode == INTERLEAVE_2) {//ENABLE_INTERLEAVE2
				if ((gubTotalGroups >= (2 * gubDieNumber))) { //IL2
					ulTotalPhyUnitNum /= 2;
				}
				else {
				}
			}
			else {}

			if (((ENABLE_FORCE_PSEUDO_CE || (guoFlashDefaultType.B.btHynix3DV5 && (!guoFlashUseType.B.btFourPlane))) && guoFlashUseType.B.btMultiDie) ||  (ENABLE_PSEUDO_CE && (((ulTotalPhyUnitNum / 1024) * 1024) > 4096) )) {
				//if(gubExpandCE == 0){
				gubExpandCE_Ratio = (gubDieNumber < (MAX_EXPAND_CE / gubCENumber)) ? (gubDieNumber) : (MAX_EXPAND_CE / gubCENumber) ;
				gubDieNumber /= gubExpandCE_Ratio;
				gubSelDieNumber = gubDieNumber;
				gubCENumber *= gubExpandCE_Ratio;
				gubMTQTotalPhyCE = (gubCENumber / gubExpandCE_Ratio);
				if (ENABLE_SANDISK_1ZNM_SPF) {
					gubCENumber -= gubPlanesPerBurst * 1 * gubHideCE; //Total Ch : 1 = 3Die -2Die : HideCE is 2die
				}
				//}
				//gulTotalPhyUnitNum /= gubExpandCE_Ratio; //After expand CE, re-calculate Physical Unit number
				if (gubExpandCE_Ratio > 1) {
					gubExpandCE = 1;
				}
			}
			else {
				gubMTQTotalPhyCE = (gubCENumber / gubExpandCE_Ratio);
			}
			gubExpandCE_initial = 1;
		}
		else {
			gubMTQTotalPhyCE = (gubCENumber / gubExpandCE_Ratio);
		}
	}

Mark_ScanSystem:
	if (gubSystemSet == 0xFF) {
		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\nSCAN_ALL");
		}
		ubScanMode = SCAN_SYSTEM_ALL;
		for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
			uwBlockPTR[ubi] = 0;
		}
	}
	else {
		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\nSCAN_SYS");
		}
		ubScanMode = SCAN_SYSTEM_SYS;
		for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
			uwBlockPTR[ubi] = guwSystemBlock[ubi];//直接去找systemBlk
		}
		gubSystemSet = 0xFF;
		gubDBTSet = 0xFF;
		for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
			guwSystemBlock[ubi] = 0xFFFF;
			guwDBTBlock[ubi] = 0xFFFF;
		}
	}

	for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
		for (ubk = 0; ubk < MAX_SYSDBT_NUMBER; ubk++) {
			for (ubj = 0; ubj < 2; ubj++) {
				gubSystemDBTSet_FirstScan[ubj][ubk] = 0xFF;
				gulSystemDBTVersion[ubj][ubk] = 0xFFFFFFFF;
				uwSystemDBTBlock_FirstScan_PerCh[ubi][ubj][ubk] = 0xFFFF;
				ulSystemDBTVersion_PerCh[ubi][ubj][ubk] = 0xFFFFFFFF;
			}
			ubDBTOffsetInSystem_PerCh[ubi][ubk] = 0xFF;
			gubDBTOffsetInSystem[ubk] = 0xFF;
		}
		uwScanCount[ubi] = 0;
	}

	guwSystemAreaNum[0] = SYSTEMAREA_DEFAULT_SCAN_RANGE << gubBurstsPerBankLog;
	guwSystemAreaNum[1] = SYSTEMAREA_DEFAULT_SCAN_RANGE << gubBurstsPerBankLog;

	do {
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
			if ((gulCeEnBitMap & (BIT0 << (ubCEIndex << 3))) && uwBlockPTR[ubCEIndex] < guwSystemAreaNum[ubCEIndex]) {
				if (ENABLE_DEBUG_UART_FLH_INITIAL) {
					UartString("\nCE:");
					Uart_Tx_DataHex(ubCEIndex);
					UartString(" block:");
					UartWordHex(uwBlockPTR[ubCEIndex]);
					UartString(" Scan:");
					UartWordHex(uwScanCount[ubCEIndex]);
				}
				FlashQueue_t *FQ;
				UBYTE ubDepth, ubL4kLink;
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ulRAMAddr = (U32)&BUFB_BASE[0];
				FQ->ubCEIndex = ubCEIndex;
				FQ->btCheckEmpty = 1;
				FQ->btThisIsErasePage = 0;
				FQ->ubFJob = BYTE_FJOB_READ;
				FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
				FQ->uwFUnit = (uwBlockPTR[ubCEIndex] >> gubBurstsPerBankLog);
				FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + (uwBlockPTR[ubCEIndex] & gubBurstsPerBankMask)) << gub4kEntrysPerPlaneLog);
				FQ->ubDepth = ubDepth;
				FQ->ubL4kIndex = ubL4kLink;
				FQ->btD3 = 1;
				FQ->btUNServed = 1;

				gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + ((ubL4kLink * L4kQ_BIND_FACTOR) * Spare4kSize);
				gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
				gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
				gulPara0x24.BitMap.btZipBypass = 1;
				gulPara0x24.BitMap.btCmpEn = 0;
				gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
				gulPara0x24.BitMap.btBufModeEn = 0;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

				if (ENABLE_DCACHE) {
					xthal_dcache_region_invalidate((void *)FQ->ulRAMAddr, ubL4KNum * 4096);
				}
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
				gFQI.ubFQLinkNumber++;
				gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;


				while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
					M_SwitchTask();
				}
				FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize));
				if ((!BURNER) && ENABLE_ERROR_DUMP_UNIT_CHECK && ((pL4KTablePtr->ulL4K_LCA & MARK_ErrorDumpUnit) == MARK_ErrorDumpUnit)) { //amour add
					guwErrorDumpBlock[ubCEIndex] = uwBlockPTR[ubCEIndex];
					if (ENABLE_ERROR_FORCE_BOOT) {
						Vender_Isp_Jump(0xFF);
					}
					else {
						if (ENABLE_WRITEPROTECT) {
							gubIsWriteProtect |= BIT_ERR_UNIT_WRITEPROTECT;
						}
						else {
							flaLEDBlink(0);
						}
					}
				}
				else if (pL4KTablePtr->ulL4K_LCA == MARK_SYS) {
					guwSystemBlock[ubCEIndex] = uwBlockPTR[ubCEIndex];
					gulSystemVersion = (pL4KTablePtr->Para0x04.BitMap.ulL4K_FW);
					ubDBTOffsetInSystem_PerCh[ubCEIndex][ubTotalSystemDBTBlockNumber[ubCEIndex][SYSDBT_SYS]] = BUFB_BASE[M_SysInfo_DBT_SET];//舊Code M_SysInfo_DBTUnit 0x80等於M_SysInfo_DBT_SET 0x80
					uwSystemDBTBlock_FirstScan_PerCh[ubCEIndex][SYSDBT_SYS][ubTotalSystemDBTBlockNumber[ubCEIndex][SYSDBT_SYS]] = uwBlockPTR[ubCEIndex];
					ulSystemDBTVersion_PerCh[ubCEIndex][SYSDBT_SYS][ubTotalSystemDBTBlockNumber[ubCEIndex][SYSDBT_SYS]] = gulSystemVersion;

					mSE_COPY((U32)(&BUFB_BASE[0]), (U32)(&gubZ2Buffer[0]), 0x4000);

					InitialSystemRemappingTable((U32)&gubZ2Buffer[0]);//這邊要先initial RemappingTable和設定flag
					if (gubTransformSystemUnitToSystemSet == TRUE) {
						guwSystemAreaNum[0] = ((UWORD)BUFB_BASE[0x18]) << gubBurstsPerBankLog;//舊code M_SysInfo_SystemAreaNum 0x18
						guwSystemAreaNum[1] = ((UWORD)BUFB_BASE[0x18]) << gubBurstsPerBankLog;
					}
					else {
						guwSystemAreaNum[0] = BUFW_BASE[(M_SysInfo_SystemAreaNumBlock_CH0) / 2];
						guwSystemAreaNum[1] = BUFW_BASE[(M_SysInfo_SystemAreaNumBlock_CH1) / 2];
					}

					if ((ubTotalSystemDBTBlockNumber[0][SYSDBT_SYS] == 0) && (ubTotalSystemDBTBlockNumber[1][SYSDBT_SYS] == 0)) {
						SetInfoBySystemBlock();
					}

					ubTotalSystemDBTBlockNumber[ubCEIndex][SYSDBT_SYS]++;
					ubGetSysBlk_CE = ubCEIndex;
				}
				else if (pL4KTablePtr->ulL4K_LCA == MARK_DBT) {//DBT
					guwDBTBlock[ubCEIndex] = uwBlockPTR[ubCEIndex];
					gulDBTVersion = (pL4KTablePtr->Para0x04.BitMap.ulL4K_FW);

					uwSystemDBTBlock_FirstScan_PerCh[ubCEIndex][SYSDBT_DBT][ubTotalSystemDBTBlockNumber[ubCEIndex][SYSDBT_DBT]] = uwBlockPTR[ubCEIndex];
					ulSystemDBTVersion_PerCh[ubCEIndex][SYSDBT_DBT][ubTotalSystemDBTBlockNumber[ubCEIndex][SYSDBT_DBT]] = gulDBTVersion;
					ubTotalSystemDBTBlockNumber[ubCEIndex][SYSDBT_DBT]++;

					mSE_COPY((U32)(&BUFB_BASE[0]), (U32)(&gubZ2Buffer[0x4000]), 0x4000);
					if (ubScanMode == SCAN_SYSTEM_DBT) {
						ubStopScan = 1;
					}
				}
			}
		}

		if (ubScanMode == SCAN_SYSTEM_ALL) {
			for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
				uwBlockPTR[ubi]++;
				uwScanCount[ubi]++;
			}
		}
		else if (ubScanMode == SCAN_SYSTEM_SYS) {
			if (guwSystemBlock[0] == 0xFFFF && guwSystemBlock[1] == 0xFFFF) {//兩個ch都沒有找到sysblock
				M_AssertError(1);
				for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
					uwBlockPTR[ubi] = 0;
					uwScanCount[ubi] = 0;
				}
				ubScanMode = SCAN_SYSTEM_ALL;

				if (ENABLE_DEBUG_UART_FLH_INITIAL) {
					UartString("\nFAIL_SCAN_SYS");
				}
			}
			else {//system block在任一個CH中被找到
				for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
					uwScanCount[ubi] = 0;
					uwBlockPTR[ubi] = guwSystemBlockRemappingTable[ubDBTOffsetInSystem_PerCh[ubGetSysBlk_CE][0]][ubi];
				}
				ubScanMode = SCAN_SYSTEM_DBT;

				if (ENABLE_DEBUG_UART_FLH_INITIAL) {
					UartString("\nSCAN_DBT");
				}
			}
		}
		else if (ubScanMode == SCAN_SYSTEM_DBT) {
			for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
				uwBlockPTR[ubi]++;
				uwScanCount[ubi]++;
			}

			for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
				if (uwBlockPTR[ubi] >= guwSystemAreaNum[ubi]) {
					uwBlockPTR[ubi] = 0;
				}
			}
		}
	}
	while ((ubStopScan == 0) && ((uwBlockPTR[0] < guwSystemAreaNum[0]) || (uwBlockPTR[1] < guwSystemAreaNum[1])));


	if (((guwDBTBlock[0] == 0xFFFF) && (guwDBTBlock[1] == 0xFFFF)) || ((guwSystemBlock[0] == 0xFFFF) && (guwSystemBlock[1] == 0xFFFF))) {
		return 1;//如果都沒有掃到也不用湊set了,直接回fail吧
	}
	else {
		if (gubTransformSystemUnitToSystemSet == TRUE) {
			if (ENABLE_DEBUG_UART_FLH_INITIAL) {
				UartString("\n Rebuild remapping");
			}
			ftlRebuildRemappingTableByOldRule_AccordingToDBT((U32)&gubZ2Buffer[0x4000]);
		}
		else {
#if ENABLE_DEBUG_UART_FLH_INITIAL
			UartString("\n Check remapping");
			ftlCheckRemappingTable_AccordingToDBT((U32)&gubZ2Buffer[0x4000]);
#endif
		}

		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) { //沒辦法確定是那一個CE有掃到,但兩個CE算出來的結果應該都要一樣
			if (guwSystemBlock[ubCEIndex] != 0xFFFF) {
				gubSystemSet = ftlGetSystemSetNumber(Mode_ByRemappingTable, 0, ubCEIndex, guwSystemBlock[ubCEIndex]);
				if (gubPlanesPerBurst == 2) { //2 channel架構
					guwSystemBlock[!ubCEIndex] = ftlGetAnotherSystemBlockInSet(Mode_ByRemappingTable, 0, ubCEIndex, guwSystemBlock[ubCEIndex]);
				}
			}
			if (guwDBTBlock[ubCEIndex] != 0xFFFF) {
				gubDBTSet = ftlGetSystemSetNumber(Mode_ByRemappingTable, 0, ubCEIndex, guwDBTBlock[ubCEIndex]);
				if (gubPlanesPerBurst == 2) { //2 channel架構
					guwDBTBlock[!ubCEIndex] = ftlGetAnotherSystemBlockInSet(Mode_ByRemappingTable, 0, ubCEIndex, guwDBTBlock[ubCEIndex]);
				}
			}
			if (guwErrorDumpBlock[ubCEIndex] != 0xFFFF) {
				gubErrorDumpSet = ftlGetSystemSetNumber(Mode_ByRemappingTable, 0, ubCEIndex, guwErrorDumpBlock[ubCEIndex]);
				if (gubPlanesPerBurst == 2) { //2 channel架構
					guwErrorDumpBlock[!ubCEIndex] = ftlGetAnotherSystemBlockInSet(Mode_ByRemappingTable, 0, ubCEIndex, guwErrorDumpBlock[ubCEIndex]);
				}
			}
		}

		//分開掃完後 , 湊SystemSet , 讓HandleSystemDBTUnit可以用set觀點下去整理
		UBYTE ubBlock_Scaned_InSet, ubBlockNumber[2], ubSYS_or_DBT, ubSystemSetPTR;
		UBYTE ubTotalSystemDBTSetNumber[2] = {0, 0}; //0:SYS 1:DBT

		for (ubSystemSetPTR = 0; ubSystemSetPTR < SYSTEM_SETS; ubSystemSetPTR++) {
			for (ubSYS_or_DBT = 0; ubSYS_or_DBT < 2; ubSYS_or_DBT++) {
				ubBlock_Scaned_InSet = 0;
				ubBlockNumber[0] = 0;
				ubBlockNumber[1] = 0;
				for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
					for (ubi = 0; ubi < ubTotalSystemDBTBlockNumber[ubCEIndex][ubSYS_or_DBT]; ubi++) {
						if ((guwSystemBlockRemappingTable[ubSystemSetPTR][ubCEIndex] != 0xFFFF) && (guwSystemBlockRemappingTable[ubSystemSetPTR][ubCEIndex] == uwSystemDBTBlock_FirstScan_PerCh[ubCEIndex][ubSYS_or_DBT][ubi])) {
							//這個set中的block在上面有被掃到,先記下來
							ubBlock_Scaned_InSet |= (BIT0 << ubCEIndex);
							ubBlockNumber[ubCEIndex] = ubi;
							break;//換另一個CE
						}
					}
				}

				if (ubBlock_Scaned_InSet == (BIT0 | BIT1)) { //這個set中的兩個block都有被掃到
					if (ulSystemDBTVersion_PerCh[0][ubSYS_or_DBT][ubBlockNumber[0]] != ulSystemDBTVersion_PerCh[1][ubSYS_or_DBT][ubBlockNumber[1]]) {
						if (ENABLE_DEBUG_UART_FLH_INITIAL) {
							flaLEDBlink(1);//SET裡面兩個Block不相同
						}
					}
					else {
						//因為兩個Channel的內容相同, 把Channel 0的填進去就好
						gubSystemDBTSet_FirstScan[ubSYS_or_DBT][ubTotalSystemDBTSetNumber[ubSYS_or_DBT]] = ubSystemSetPTR;
						gulSystemDBTVersion[ubSYS_or_DBT][ubTotalSystemDBTSetNumber[ubSYS_or_DBT]] = ulSystemDBTVersion_PerCh[0][ubSYS_or_DBT][ubBlockNumber[0]];
						if (ubSYS_or_DBT == SYSDBT_SYS) {
							if (gubTransformSystemUnitToSystemSet == TRUE) { //Unit Number轉成Set Number
								gubDBTOffsetInSystem[ubTotalSystemDBTSetNumber[ubSYS_or_DBT]] = ftlGetSystemSetNumber(Mode_ByRemappingTable, 0, 0, (ubDBTOffsetInSystem_PerCh[0][ubBlockNumber[0]] << gubBurstsPerBankLog));
							}
							else {
								gubDBTOffsetInSystem[ubTotalSystemDBTSetNumber[ubSYS_or_DBT]] = ubDBTOffsetInSystem_PerCh[0][ubBlockNumber[0]];
							}
						}

						ubTotalSystemDBTSetNumber[ubSYS_or_DBT]++;
					}
					break;//這個set已經被確定角色不用再繼續檢查
				}
				else if ((ubBlock_Scaned_InSet == BIT0) || (ubBlock_Scaned_InSet == BIT1)) { //這個set中只有一個block被掃到
					ubCEIndex = (ubBlock_Scaned_InSet == BIT0) ? 0 : 1; //那一個CE有找到Block

					gubSystemDBTSet_FirstScan[ubSYS_or_DBT][ubTotalSystemDBTSetNumber[ubSYS_or_DBT]] = ubSystemSetPTR;
					gulSystemDBTVersion[ubSYS_or_DBT][ubTotalSystemDBTSetNumber[ubSYS_or_DBT]] = ulSystemDBTVersion_PerCh[ubCEIndex][ubSYS_or_DBT][ubBlockNumber[ubCEIndex]];
					if (ubSYS_or_DBT == SYSDBT_SYS) {
						if (gubTransformSystemUnitToSystemSet == TRUE) { //Unit Number轉成Set Number
							gubDBTOffsetInSystem[ubTotalSystemDBTSetNumber[ubSYS_or_DBT]] = ftlGetSystemSetNumber(Mode_ByRemappingTable, 0, ubCEIndex, (ubDBTOffsetInSystem_PerCh[ubCEIndex][ubBlockNumber[ubCEIndex]] << gubBurstsPerBankLog));
						}
						else {
							gubDBTOffsetInSystem[ubTotalSystemDBTSetNumber[ubSYS_or_DBT]] = ubDBTOffsetInSystem_PerCh[ubCEIndex][ubBlockNumber[ubCEIndex]];
						}
					}

					ubTotalSystemDBTSetNumber[ubSYS_or_DBT]++;
					break;//這個set已經被確定角色不用再繼續檢查
				}
				else { }
				//先檢查SYS再檢查DBT
			}
		}
		return 0;
	}
#else
	return 0;
#endif
}

void HandleSystemDBTSet()
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubCEIndex;
	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	UBYTE ubi;
	UBYTE ubIndex;
	U32 ulNewSystemDBTVersion[2] = {0xFFFFFFFF, 0xFFFFFFFF};
	U32 ulOldSystemDBTVersion[2] = {0xFFFFFFFF, 0xFFFFFFFF};
	UBYTE ubNewSystemDBTIndex[2] = {0xFF, 0xFF};//0:SYS, 1:DBT
	UBYTE ubOldSystemDBTIndex[2] = {0xFF, 0xFF};//0:SYS, 1:DBT
	UBYTE ubUsedSystemDBTIndex[2] = {0xFF, 0xFF};//0:SYS, 1:DBT
	UWORD uwSystemDBTVersionMap[2] = {0, 0};
	UBYTE ubTotalSystemDBTSetNumber[2] = {0, 0};
	FlashQueue_t *FQ;
	UBYTE ubDepth, ubL4kLink;
	UBYTE ubL4KCount;
	U32 ulPlaneIndex;
	UBYTE ubNeedCheckLastPage = 0;

	for (ubIndex = 0; ubIndex < 2; ubIndex++) {
		for (ubi = 0; ubi < MAX_SYSDBT_NUMBER; ubi++) {
			if (gulSystemDBTVersion[ubIndex][ubi] != 0xFFFFFFFF) {
				uwSystemDBTVersionMap[ubIndex] |= (BIT0 << (gulSystemDBTVersion[ubIndex][ubi] & 0x0F));
				ubTotalSystemDBTSetNumber[ubIndex]++;
			}
		}
	}

	if ((ubTotalSystemDBTSetNumber[SYSDBT_SYS] != 1) || (ubTotalSystemDBTSetNumber[SYSDBT_DBT] != 1)) { //SA DA, or SB DB, normal case
		//***************************************************************************************************************
		// find the newest version
		//***************************************************************************************************************
		for (ubIndex = 0; ubIndex < 2; ubIndex++) {
			for (ubi = 0; ubi < MAX_CODEVERSION_NUM; ubi++) {
				if ((((uwSystemDBTVersionMap[ubIndex] >> ubi) & BIT0) == BIT0) && (((uwSystemDBTVersionMap[ubIndex] >> ((ubi + 1) & 0x0F)) & BIT0) == 0)) {
					ulNewSystemDBTVersion[ubIndex] = (gulSystemDBTVersion[ubIndex][0] & 0xFFFFFFF0) + ubi;
					break;
				}
			}

			for (ubi = 0; ubi < MAX_CODEVERSION_NUM; ubi++) {
				if ((((uwSystemDBTVersionMap[ubIndex] >> ubi) & BIT0) == 0) && (((uwSystemDBTVersionMap[ubIndex] >> ((ubi + 1) & 0x0F)) & BIT0) == BIT0)) {
					ulOldSystemDBTVersion[ubIndex] = (gulSystemDBTVersion[ubIndex][0] & 0xFFFFFFF0) + ((ubi + 1) & 0x0F);
					break;
				}
			}

			for (ubi = 0; ubi < ubTotalSystemDBTSetNumber[ubIndex]; ubi++) {//挑最老跟最新的
				if (gulSystemDBTVersion[ubIndex][ubi] == ulNewSystemDBTVersion[ubIndex]) {
					ubNewSystemDBTIndex[ubIndex] = ubi;
				}

				if (gulSystemDBTVersion[ubIndex][ubi] == ulOldSystemDBTVersion[ubIndex]) {
					ubOldSystemDBTIndex[ubIndex] = ubi;
				}
			}
		}

		if ((ubTotalSystemDBTSetNumber[SYSDBT_SYS] == 1) && (ubTotalSystemDBTSetNumber[SYSDBT_DBT] >= 2)) { //SA DA DB, leave A (delete DB)
			//SYS有一份,DBT有兩份,先用舊的DBT
			ubUsedSystemDBTIndex[SYSDBT_SYS] = ubOldSystemDBTIndex[SYSDBT_SYS];
			ubUsedSystemDBTIndex[SYSDBT_DBT] = ubOldSystemDBTIndex[SYSDBT_DBT];
		}
		else if ((ubTotalSystemDBTSetNumber[SYSDBT_SYS] >= 2) && (ubTotalSystemDBTSetNumber[SYSDBT_DBT] >= 2)) { //SA DA DB SB, leave A or B (delete SA DA or delete SB DB, need check)
			ubNeedCheckLastPage = 1;
		}
		else if ((ubTotalSystemDBTSetNumber[SYSDBT_SYS] >= 2) && (ubTotalSystemDBTSetNumber[SYSDBT_DBT] == 1)) { //SA SB DB, leave B (delete SA), or SA SB DA (when only update system block, delete SB, so need check)
			ubNeedCheckLastPage = 1;
		}

		//***************************************************************************************************************
		// check the last page to confirm the newest System block has been successfully programed or not.
		// 檢查新的system block的最後一個page是否可以讀,如果可以就用新的SystemBlock和新的DBT(因為他們兩個一起更新, VERSION會同時加1)
		// 如果不行就用舊的SystemBlock和舊的DBT
		//***************************************************************************************************************
		if (ubNeedCheckLastPage) {
			UWORD uwBlock;
			ubL4KNum = (gubSectorsPerPlane >> 3);
			ulPlaneIndex = guwFastPagePagesPerUnit - 1;
			ubCEIndex = gubPlanesPerBurst - 1; //CE=1 in 2CH.

			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ulRAMAddr = (U32)&gubZ2Buffer[0];
			FQ->btCheckEmpty = 1;
			FQ->btThisIsErasePage = 0;
			FQ->ubFJob = BYTE_FJOB_READ;
			FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);

			uwBlock = guwSystemBlockRemappingTable[gubSystemDBTSet_FirstScan[SYSDBT_SYS][ubNewSystemDBTIndex[SYSDBT_SYS]]][ubCEIndex];//拿ch1的block
			FQ->uwFUnit = (uwBlock >> gubBurstsPerBankLog);
			FQ->ulFEntry = (((ulPlaneIndex << gubPlanesPerPageLog) + ((ubCEIndex << gubBurstsPerBankLog) + (uwBlock & gubBurstsPerBankMask))) << gub4kEntrysPerPlaneLog);

			FQ->ubDepth = ubDepth;
			FQ->ubL4kIndex = ubL4kLink;
			FQ->btUNServed = 1;
#if TLC
			FQ->btD3 = 1;
#endif
			gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + ((ubL4kLink * L4kQ_BIND_FACTOR) * Spare4kSize);
			gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
			gulPara0x24.BitMap.ubFrameNum = gubSectorsPerPlane / 8;
			gulPara0x24.BitMap.btZipBypass = 1;
			gulPara0x24.BitMap.btCmpEn = 0;
			gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
			gulPara0x24.BitMap.btBufModeEn = 0;
			gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
			gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

			if (ENABLE_DCACHE) {
				xthal_dcache_region_invalidate((void *)FQ->ulRAMAddr, ubL4KNum * 4096);
			}

			for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
				//set L4K table info
				pL4KTable.ulL4K_LCA = 0x00000000;
				pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
				pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
				pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
				pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
				pL4KTable.Para0x04.ulAll = 0;
				*pL4KTablePtr = pL4KTable;
			}
			gFQI.ubFQLinkNumber++;
			gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;
			while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
				M_SwitchTask();
			}

			FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

			pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (FQ->ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));

			if (pL4KTablePtr->ulL4K_LCA != MARK_SYS) {
				ubUsedSystemDBTIndex[SYSDBT_SYS] = ubOldSystemDBTIndex[SYSDBT_SYS];
				ubUsedSystemDBTIndex[SYSDBT_DBT] = ubOldSystemDBTIndex[SYSDBT_DBT];
			}
			else {
				ubUsedSystemDBTIndex[SYSDBT_SYS] = ubNewSystemDBTIndex[SYSDBT_SYS];
				ubUsedSystemDBTIndex[SYSDBT_DBT] = ubNewSystemDBTIndex[SYSDBT_DBT];
			}
		}
		//選出來的system裡記的選出來的DBT預期是要一樣。但萬一不幸不一樣，會優先用system裡記的DBT位置，
		//若很不幸該位置又沒DBT，則會使用原本預計要用的DBT
		if (gubSystemDBTSet_FirstScan[SYSDBT_DBT][ubUsedSystemDBTIndex[SYSDBT_DBT]] != gubDBTOffsetInSystem[ubUsedSystemDBTIndex[SYSDBT_SYS]]) {
			if (ENABLE_DEBUG_RW) {
				flaLEDBlink(1);
			}

			for (ubi = 0; ubi < ubTotalSystemDBTSetNumber[SYSDBT_DBT]; ubi++) {
				if (gubSystemDBTSet_FirstScan[SYSDBT_DBT][ubi] == gubDBTOffsetInSystem[ubUsedSystemDBTIndex[SYSDBT_SYS]]) {
					//選出來的system裡記的DBT與掃出來的DBT一樣時
					ubUsedSystemDBTIndex[SYSDBT_DBT] = ubi;
					break;
				}
			}
		}

		//***************************************************************************************************************
		// erase system/DBT block that would not be used
		//***************************************************************************************************************
		UWORD uwSystemDBTBlock;
		UBYTE ubSystemDBTUnit;
		UBYTE ubSystemDBTBurst;
		for (ubIndex = 0; ubIndex < 2; ubIndex++) {
			for (ubi = 0; ubi < ubTotalSystemDBTSetNumber[ubIndex]; ubi++) {
				if (ubi != ubUsedSystemDBTIndex[ubIndex]) {
					for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
						uwSystemDBTBlock = guwSystemBlockRemappingTable[gubSystemDBTSet_FirstScan[ubIndex][ubi]][ubCEIndex];
						ubSystemDBTUnit = (UBYTE)(uwSystemDBTBlock >> gubBurstsPerBankLog);
						ubSystemDBTBurst = (UBYTE)(uwSystemDBTBlock & gubBurstsPerBankMask);
#if FixShallowErase //Erase Plane-A for CE0 & CE1
						ftlForceSaveDummy(((ubCEIndex << gubBurstsPerBankLog) + ubSystemDBTBurst), (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div)), ubSystemDBTUnit, (FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE | FORCE_SAVE_DUMMY_DIRECT_ACCESS | FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE), D3_AREA, VT_FORCE_DUMMY_0xC0);
#endif
						//erase
						M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
						FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
						FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);

						// Must Send Reset CMD(0xFF) After Erase Fail
						FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

						FQ->uwFUnit = ubSystemDBTUnit;
						FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + ubSystemDBTBurst) << gub4kEntrysPerPlaneLog);

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
				}
			}
		}
		Vender_Isp_Jump(0);
	}
}

UBYTE flaLoadSystemAreaSet(UBYTE ubCEIndex, UBYTE ubSet, U32 ulSramAddr, U32 ulL4K)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(1);
	M_CheckRDTModeToDisableFunctionReturnValid(1);

	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;

	FlashQueue_t *FQ;
	UBYTE ubDepth, ubL4kLink, ubi, ubUnit, ubBurst;
	UBYTE ubReturn = 0;	// 0 success, 1 fail

	for (ubi = ubCEIndex; ubi < gubPlanesPerBurst; ubi++) {

		ubUnit = (UBYTE)(guwSystemBlockRemappingTable[ubSet][ubi] >> gubBurstsPerBankLog);
		ubBurst = (UBYTE)(guwSystemBlockRemappingTable[ubSet][ubi] & gubBurstsPerBankMask);

		M_GetFQ(FQ, ubDepth, ubL4kLink, ubi);
		FQ->ulRAMAddr = ulSramAddr;
		FQ->ubCEIndex = ubi;
		FQ->btCheckEmpty = 0;
		FQ->btThisIsErasePage = 0;
		FQ->ubFJob = BYTE_FJOB_READ;
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
		FQ->uwFUnit = ubUnit;

		if (ENABLE_PLANE_CH_CE) {
			FQ->ulFEntry = (((ubi << gubBurstsPerBankLog) + ubBurst) << gub4kEntrysPerPlaneLog);
		}
		else {
			FQ->ulFEntry = (((ubi & gubPlanesPerBurstMask) + (ubBurst << gubPlanesPerBurstLog)) << gub4kEntrysPerPlaneLog);
		}
		FQ->ubDepth = ubDepth;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btD3 = 1;
		FQ->btUNServed = 1;

		gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + ((ubL4kLink * L4kQ_BIND_FACTOR) * Spare4kSize);
		gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
		gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
		gulPara0x24.BitMap.btZipBypass = 1;
		gulPara0x24.BitMap.btCmpEn = 0;
		gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
		gulPara0x24.BitMap.btBufModeEn = 0;
		gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
		gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;

		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate((void *)FQ->ulRAMAddr, ubL4KNum * 4096);
		}
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
		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;


		while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		FQ->ubFQPhase = BYTE_FQ_CHECKDONE;
		pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (ubL4kLink * L4kQ_BIND_FACTOR * Spare4kSize));

		if (pL4KTablePtr->ulL4K_LCA != ulL4K) {
			UBYTE ubTableSel = 0;
			if (ulL4K == MARK_SYS) {
				ubTableSel = BYTE_SYSTEMUNIT;
			}
			else if (ulL4K == MARK_DBT) {
				ubTableSel = BYTE_DBT;
			}
			ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, BYTE_UNC_READTABLE, ubTableSel, BIT_NormalErr);
			if (ubi == 0) {
				continue;
			}
			else {
				if (ulL4K == MARK_SYS) { //SYSTEM
					M_AssertError(1);
				}
				else if (ulL4K == MARK_DBT) { //DBT
					gubDBTSet = 0xFF;
					guwDBTBlock[0] = 0xFFFF;
					guwDBTBlock[1] = 0xFFFF;
				}
				ubReturn = 1;
			}
		}
		else {
			break;
		}
	}
	return ubReturn;
#endif
}
/*
void flaSaveSystemAreaUnit(UBYTE ubCEIndex, UBYTE ubUnit, U32 ulPlanePtr, U32 ulSramAddr, U32 ulL4K)
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UBYTE ubL4KNum = gub4kEntrysPerPlane;
	FlashQueue_t *FQ;
	UBYTE ubDepth, ubL4kLink;
	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);

	FQ->ulRAMAddr = ulSramAddr;
	FQ->ubFJob = BYTE_FJOB_WRITE;
	FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);

	FQ->uwFUnit = ubUnit;
	FQ->ulFEntry = (ulPlanePtr << gub4kEntrysPerPlaneLog);
	FQ->ubRelativeDepth = ubDepth;
	FQ->ubDepth = 0xFF;
	FQ->ubL4kIndex = ubL4kLink;
	FQ->btD3 = 1;
	FQ->btUNServed = 1;

	gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
	gulPara0x24.BitMap.btBufModeEn = 0;
	gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
	gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
	gulPara0x24.BitMap.btZipBypass = 1;
	gulPara0x24.BitMap.btCmpEn = 0;
	gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
	gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

	if (ENABLE_DCACHE) {
		xthal_dcache_region_writeback_inv((void *)FQ->ulRAMAddr, ubL4KNum * 4096);
	}

	L4KTable16B pL4KTable;
	UBYTE ubL4KCount = 0;
	L4KTable16B *pL4KTablePtr;

	for (ubL4KCount = 0; ubL4KCount < ubL4KNum; ubL4KCount++) {
		pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
		//set L4K table info
		pL4KTable.ulL4K_LCA = ulL4K;
		pL4KTable.Para0x04.BitMap.ulL4K_FW = 0x112233;
		pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
		pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
		pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
		pL4KTable.Para0x08.BitMap.ubL4K_FW2 = 0xAB;
		pL4KTable.Para0x0C.BitMap.ulL4K_BADR = (FQ->ulRAMAddr + (ubL4KCount * 4096)) / 512;
		pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = MAX_ZCODE;
		*pL4KTablePtr = pL4KTable;
	}

	gFQI.ubFQLinkNumber++;
	gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

	// wait CE read done...
	while (FQ->ubFQPhase != BYTE_FQ_WAITCHECK) {
		M_SwitchTask();
	}

	FQ->ubFQPhase = BYTE_FQ_CHECKDONE;

	if (FQ->btPEFail) {
		UBYTE ubTableSel = 0;
		if (ulL4K == MARK_SYS) {
			ubTableSel = BYTE_SYSTEMUNIT;
		}
		else if (ulL4K == MARK_DBT) {
			ubTableSel = BYTE_DBT;
		}
		ftlRecordFailLog(FLHErrLogType, 0x1, FQ->ubL4kIndex, BYTE_PF_WRITETABLE, ubTableSel, BIT_NormalErr);
	}

#endif
}
*/

void flaCheckFirstID(void)
{
	UBYTE ubi;
	FCONL[FCONL_FCE_ENB] = (gubGPIO_CE_Decoder && (!MicronFlashOnly)) ? M_MapToCEDecoder(0, 0) : BIT0; //CE0 enable
	FCTLL[(FCTLL_PIO_CMD)] = 0x90;
	FCTLL[(FCTLL_PIO_ADR)] = 0x00;
	FCTLL[FCTLL_HS_MODE] &= (~PIO_DAT_LAST); //First

	if ((FCTLL[FCTLL_FLH_SET] & FLA_ONFI_MODE) == FLA_ONFI_MODE) {
		FCTLL[FCTLL_HS_MODE] |= ((PIO_DAT_FIRST | PIO_DAT_LAST));
		for (ubi = 0; ubi < 8; ubi++) {
			gubFlashID[ubi] = FCTLL[(FCTLL_PIO_DAT)];
		}
	}
	else {
		for (ubi = 0; ubi < 8; ubi++) {
			if (ubi == 7) {
				FCTLL[FCTLL_HS_MODE] |= PIO_DAT_LAST;
			}

			gubFlashID[ubi] = FCTLL[(FCTLL_PIO_DAT)];

			if (ubi == 0) {
				FCTLL[FCTLL_HS_MODE] &= (~(PIO_DAT_FIRST | PIO_DAT_LAST)); //First
			}
		}
		FCTLL[FCTLL_HS_MODE] |= ((PIO_DAT_FIRST | PIO_DAT_LAST));
	}

	//check bus
	//if ((gubFlashID[3]&BIT6)==0) {
	guoFlashDefaultType.B.bt8Bit = TRUE;
	//FL_FCON_MODE_SET_TOP&=(~(BIT28|BIT27));         // set 8 channel mode??????????????????????
	//}

	//check level per cell
	switch (gubFlashID[2] & (BIT3 | BIT2)) {
		case 0x00:	// 2 level cell, SLC
			guoFlashDefaultType.B.btSLC = 1;
			break;
		case 0x04:	// 4 level cell
			guoFlashDefaultType.B.bt4LC = 1;
			break;
		case 0x08:	// 8 level cell
			guoFlashDefaultType.B.bt8LC = 1;
			break;
		default:
			break;

	}

	/*gubFlashID[1] = 0xD7;
	gubFlashID[2] = 0x94;*/



	switch (gubFlashID[0]) {
		case 0x98:              //toshiba
			guoFlashDefaultType.B.btToshiba = TRUE;
			//TC58NVG3S0FTA00, TH58NVG4S0FTA20 =>  98 d3 90 26 76 15
			//TC58NVG3S0FTAI0 =>
			//TH58NVG4S0FTAK0 =>
			if (gubFlashID[1] == 0xd3 && gubFlashID[2] == 0x90 && gubFlashID[3] == 0x26 && gubFlashID[4] == 0x76 &&  gubFlashID[5] == 0x15) {
				gub4KSLC = 1;
			}
			else {
				gub4KSLC = 0;
			}
			flaToshibaIDCheck();
			break;
#if !NotNessary
		case 0xEC:              //samsung
			guoFlashDefaultType.B.btSamsung = TRUE;
			//flaSamsungIDCheck();
			break;
		case 0x45:              //sandisk
			guoFlashDefaultType.B.btSandisk = TRUE;
			guoFlashDefaultType.B.btToshiba = TRUE;
			flaToshibaIDCheck();
			break;
#if (Hynix||((!(MicronFlashOnly || TLC || Hynix || Sandisk))&&BURNER))
		case 0x20:              //ST
		case 0xAD:              //hynix
			guoFlashDefaultType.B.btHynix = TRUE;
			flaHynixIDCheck();
			break;
#endif
		case 0x89:              //Intel
		case 0xB5:              //SpecTech
		case 0x2C:              //Micron
			//Address refers to samsung
			guoFlashDefaultType.B.btIntelMicron = TRUE;
			flaIntelMicronIDCheck();
			break;
		case 0x9B:
			flaYMTCIDCheck();
			break;
#endif
		default:
			guoFlashDefaultType.B.btNoSupport = TRUE;
			break;
	}

	/*if (guoFlashDefaultType.B.btToshiba&&guoFlashDefaultType.B.btToggle) {
	        // need check 90-40 id
	        FL_FCTL_PIO_CMD_IP0 = 0x90;
	        FL_FCTL_PIO_ADR_IP0 = 0x40;
	        for (ubi = 0;ubi<20;ubi++) {};
	        for (gubTemp = 0;gubTemp<20;gubTemp++) {};
	        gubFlashID2 = FL_FCTL_PIO_DAT_IP0;      // "J"
	        for (gubTemp = 0;gubTemp<20;gubTemp++) {};
	        gubFlashID2 = FL_FCTL_PIO_DAT_IP0;      // "E"
	        for (gubTemp = 0;gubTemp<20;gubTemp++) {};
	        gubFlashID2 = FL_FCTL_PIO_DAT_IP0;      // "D"
	        for (gubTemp = 0;gubTemp<20;gubTemp++) {};
	        gubFlashID2 = FL_FCTL_PIO_DAT_IP0;      // "E"
	        for (gubTemp = 0;gubTemp<20;gubTemp++) {};
	        gubFlashID2 = FL_FCTL_PIO_DAT_IP0;      // "C"
	        for (gubTemp = 0;gubTemp<20;gubTemp++) {};
	        gubFlashID2 = FL_FCTL_PIO_DAT_IP0;      // ToggleMode
	        if ((gubFlashID2&0x07)==TSB_ID40_LEGACY) {
	                guoFlashDefaultType.B.btToggle = 0;
	        }
	}*/
#if ENABLE_SANDISK_1ZNM_SPF
	CheckSandisk160G();
#endif
	M_DisableAllFlashCE();
}
#if Hynix
UBYTE flaRetrieveRetryTable(U32 ulCEBitMap)
{
	UBYTE ubCH, ubCE, ubDie, ubIndex, ubSLCMode, ubRet = 0;
	U32 ulBitMap;

	ToggleModeEnable(0);

	for (ubCH = 0; ubCH < MAX_CHANNEL; ubCH++) {
		for (ubCE = 0; ubCE < MAX_CE_PER_CH; ubCE++) {
			ulBitMap = (1uL << (ubCE + (ubCH * MAX_CE_PER_CH)));
			if (ulCEBitMap & ulBitMap) { //Get RR Table when CE exists
				for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
					for (ubSLCMode = 0; ubSLCMode < 2; ubSLCMode++) {
						if (flaGetHynixReadRetryTable(ubCH, ubCE, ubDie, ubSLCMode)) {
							ubRet = 0x01;
						}
						else {
							ubIndex = ((ubCE * MAX_CHANNEL) + ubCH);
							gpsHynixRetryMeta[ubIndex].ubDie[ubDie].ubIsValid = ENABLE;
						}
					}
				}
			}
		}
	}

	ToggleModeEnable(guoFlashUseType.B.btToggle);

	return (ubRet) ? RET_ERR : RET_OK;
}

#endif
#if UFS
U32 M_MapToCEDecoder(UBYTE ubChannel, UBYTE ubCE)
{

	return ((U32)(gubUFSRule ? (U32)(gubUFSRuleMap[ubChannel][ubCE] << (8 * ubChannel)) : (U32)((BIT7 | (ubCE << 1)) << (8 * ubChannel))));
}

U32 M_MapToCEDecoderMT(UBYTE ubCE)
{

	return ((U32)(gubUFSRule ? (U32)gubUFSRuleMap[0][ubCE] : (U32)(BIT7 | (ubCE << 1))));
}

UBYTE UFSToggleCheck(U32 ulCEEnable, UBYTE ubCH)
{
	FCONL[FCONL_FCE_ENB] = ulCEEnable; //CE0 enable
	FCTLL[(FCTLL_PIO_CMD) + ((ubCH * 0x200) >> 2)] = 0x90;
	FCTLL[(FCTLL_PIO_ADR) + ((ubCH * 0x200) >> 2)] = 0x00;
	FCTLL[FCTLL_HS_MODE + ((ubCH * 0x200) >> 2)] &= (~PIO_DAT_LAST); //First

	UBYTE ubi, ubFlashID[8];
	for (ubi = 0; ubi < 8; ubi++) {
		if (ubi == 7) {
			FCTLL[FCTLL_HS_MODE + ((ubCH * 0x200) >> 2)] |= PIO_DAT_LAST;
		}

		ubFlashID[ubi] = FCTLL[(FCTLL_PIO_DAT) + ((ubCH * 0x200) >> 2)];

		if (ubi == 0) {
			FCTLL[FCTLL_HS_MODE + ((ubCH * 0x200) >> 2)] &= (~(PIO_DAT_FIRST | PIO_DAT_LAST)); //First
		}
	}
	FCTLL[FCTLL_HS_MODE + ((ubCH * 0x200) >> 2)] |= ((PIO_DAT_FIRST | PIO_DAT_LAST));
	FCONL[FCONL_FCE_ENB] = 0;
	if ((ubFlashID[5] & 0x80)) {
		return 1;
	}
	else {
		return 0;
	}
}

void UFSByPass(void)
{

	UBYTE ubChannel ;
	volatile REG_t * pFlaReg ;
	FCONL[FCONL_FCE_ENB] = 0x00;
	FALLL[FCTLL_IO_SET] &= SINGLE_END_IO_TYPE;
	for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel ++) {

		pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[ubChannel];

		pFlaReg[FCTLL_IO_SET].L |= F_IO_L_GPO_MODE_EN ;
		idle(3) ;
		pFlaReg[FCTLL_IO_SET].L |=  (FORCE_FLA_ALE_HIGH  | FORCE_FLA_CLE_HIGH);
		idle(3) ;
		//WEZ
		//DQS
		////////////////////
		////Write
		////////////////////
		pFlaReg[FCTLL_HS_MODE].L |= DQS_GPO_MODE ;
		idle(3) ;
		pFlaReg[FCTLL_HS_MODE].L |= DQS_GPO_VALUE ;
		idle(3) ;
		pFlaReg[FCTLL_IO_SET].B.B0 = 0xA0;
		idle(3) ;
		pFlaReg[FCTLL_IO_SET].L &= (FORCE_FLA_REN_LOW);
		idle(3) ;
		pFlaReg[FCTLL_HS_MODE].L &= (~DQS_GPO_VALUE ) ;

		idle(3) ;
		pFlaReg[FCTLL_IO_SET].L |= (~FORCE_FLA_REN_LOW);
		////////////////////
		////Read
		////////////////////
		idle(3) ;
		pFlaReg[FCTLL_IO_SET].L &= FORCE_FLA_WEN_LOW ;
		idle(3) ;
		pFlaReg[FCTLL_IO_SET].L &= (~F_IO_L_GPO_MODE_EN);


		idle(3) ;
		pFlaReg[FCTLL_IO_SET].L |= (~FORCE_FLA_WEN_LOW);
		idle(3) ;

		////////////////////
		////End
		////////////////////
		pFlaReg[FCTLL_HS_MODE].L &= (~DQS_GPO_MODE );
		idle(3) ;
		pFlaReg[FCTLL_IO_SET].L &= (~ (FORCE_FLA_ALE_HIGH  | FORCE_FLA_CLE_HIGH));
		idle(3) ;

	}
}
#endif

void CheckFlashType(void)
{
	/* Adjust FLH Clock Frequency to Lower Setting */
	if (ASIC) {
		gubCurrentFlashClock = FLH_CLK_10MHz;
		flaSwitchClock(FLH_CLK_10MHz);//adjust clock to 10Mbps
	}

	U32 ulDelayLoop;
	UBYTE ubStatus;
	UBYTE ubi;


	gulCeEnBitMap = 0;
#if UFS
	gubUFSDefaultMode_CE = 0;
#endif
	if (((SYS0W[SYS0W_GPIO_LEFT] & BIT9) == 0) && (SYS0W[SYS0W_GPIO_LEFT] & BIT3)) { //CE Decoder Mode
		gubUFSRule = 0;
		gubGPIO_CE_Decoder = 1;
		gubMAX_SCAN_CE = 64;
		gubMAX_SCAN_CE_PER_CH = 32;
	}
	else {
		gubUFSRule = 0;
		gubGPIO_CE_Decoder = 0;
		gubMAX_SCAN_CE = 16;
		gubMAX_SCAN_CE_PER_CH = 8;
	}
	//reset ce0,ce8


	for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {

		FCONL[FCONL_FCE_ENB] = (gubGPIO_CE_Decoder && (!MicronFlashOnly)) ? M_MapToCEDecoder(ubi, 0) : BIT0 << (ubi << 3);

		FCTLL[FCTLL_PIO_CMD + (ubi * 0x200 >> 2)] = 0xFF;			//Send 0xff
		FCTLL[FCTLL_PIO_CMD + (ubi * 0x200 >> 2)] = 0x70;			//Send 0x70 to check status

		ulDelayLoop = 0;
		ubStatus = 0;
		do {
			ubStatus = FCTLL[FCTLL_PIO_DAT + (ubi * 0x200 >> 2)];
			ulDelayLoop++;
			if (ulDelayLoop > 40000) {
				break;
			}
		}
		while (ubStatus != 0xE0);

		if (ulDelayLoop > 40000) {
			break;
		}
#if UFS
		gubUFSDefaultMode_CE |= UFS_Sample_Exist << ubi;
#endif
		gulCeEnBitMap |= (BIT0 << (ubi << 3));

	}
#if	ENABLE_YMTC_1V2
	gubIOMode_CE[0] = 0xFF;
	gubIOMode_CE[1] = 0xFF;

	//for set Toggle mode
	gubFlashMode_CE[0] = 0xFF;
	gubFlashMode_CE[1] = 0xFF;

	gubFlashONFI_CE[0] = 0x00;
	gubFlashONFI_CE[1] = 0x00;
#endif

	if ((gubFlashMode_CE[0] & BIT0) && ((gubFlashONFI_CE[0] & BIT0) == 0)) {
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
	}
	else if ((gubFlashMode_CE[0] & BIT0) && (gubFlashONFI_CE[0] & BIT0)) {	//Ting 20170223
		FALLL[FCTLL_FLH_SET] |= FLA_ONFI_MODE;
	}
	else {
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
	}

	if (gubIOMode_CE[0] & BIT0) {
		flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
	}
	else {
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);
	}


	if (ENABLE_SANDISK_1ZNM_SPF) {
		gubHideCE = 0;
	}
	//--------------------------------------------------//
	if (BURNER) {
		for (ubi = 0; ubi < 20; ubi++) {
			guoFlashDefaultType.All[0] = 0; //clear all Flash Info
			guoFlashDefaultType.All[1] = 0; //clear all Flash Info
			gubDieNumber = 1;
			gubSelDieNumber = 1;

			flaCheckFirstID();
		}
	}
	else {
		do {
			guoFlashDefaultType.All[0] = 0; //clear all Flash Info
			guoFlashDefaultType.All[1] = 0; //clear all Flash Info
			gubDieNumber = 1;
			gubSelDieNumber = 1;

			flaCheckFirstID();
		}
		while (guoFlashDefaultType.B.btNoSupport == TRUE);
	}
	if (BURNER && (guoFlashDefaultType.B.btNoSupport == TRUE))
		return;
#if UFS
	UBYTE ubj;

	gubGPIO_CE_Decoder = 1;
	gubUFSRule = 1;
	//UBYTE ubUFSRuleMap[MAX_CHANNEL][MAX_CE_PER_CH] = {{0x1, 0x2, 0x3, 0x4, 0x9, 0xA, 0xB, 0xC}, {0x1, 0x2, 0x3, 0x4, 0x9, 0xA, 0xB, 0xC}};
	for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
		for (ubj = 0; ubj < MAX_CE_PER_CH; ubj++) {
			gubUFSRuleMap[ubi][ubj] = 0;
		}
		if (gulCeEnBitMap & (BIT0 << (ubi * 8))) {
			gubUFSRuleMap[ubi][0] = 0x1;
		}
	}


	if (gubUFSRule) {
		UFSByPass();
	}

	if ((guoFlashDefaultType.B.btToggle == 1) && UFS) {
		gubUFSDefaultMode_CE |= UFS_Sample_Default_Mode_Toggle;
		gubFlashMode_CE[0] |= BIT0;
	}

	if (gubUFSDefaultMode_CE & (UFS_Sample_Exist << 1)) { //check ch1 first sample exist
		if (UFSToggleCheck((BIT0 << 8), 1)) {
			gubUFSDefaultMode_CE |= (UFS_Sample_Default_Mode_Toggle << 1);
			gubFlashMode_CE[1] |= BIT0;
		}
	}

#endif
#if((!(MicronFlashOnly || TLC || Hynix || Sandisk))&&BURNER)
	if (!(guoFlashDefaultType.B.btToshiba)) {
		return;
	}
#endif

#if MicronFlashOnly
#if (ENABLE_FORCE_L06B_TO_B0KB)
	ForceL06BToB0KB(1);
	if (!guoFlashDefaultType.B.btNoSupport) {
		guoFlashDefaultType.All = 0;
		flaCheckFirstID();
	}
#endif
	flaScanCE_And_Remapping();
	flaMicronRandomizer(0);
	M_DisableAllFlashCE();
#endif

	flaSetParameterByFlashDefaultType();
	flaSetFlashUseType(0);
	flaSetFlashAddressType();
	InitVaribleBeforeLoadSystemBlock();
#if UFS

#else
	if ((guoFlashUseType.B.btToggle == 1) && ((guoFlashDefaultType.B.btHynix == 0) && (guoFlashUseType.B.bt8LC == 0))) {
		gubFlashMode_CE[0] |= BIT0;
		gubFlashMode_CE[1] |= BIT0;
	}
#endif
	if (ASIC) {
		gubCurrentFlashClock = FLH_CLK_40MHz;
		flaSwitchClock(FLH_CLK_40MHz);//reset clock to default value
	}
}

#if ENABLE_SANDISK_1ZNM_SPF
void CheckSandisk160G(void)
{
	if (gubHideCE == 0) {
		UBYTE ubChannel, ubCE , ubStatus, ubID, ubHideCEFind, ubi;
		U32 ulDelay, ulAdr;
		if (gubFlashMode_CE[0] & BIT0) {// CE0 & CE1 are in toggle mode, if fw want to check other ce's id, flh ip should be legacy mode
			FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
		}
		for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
			UBYTE ubCheckCEPerCh = 1;
			gFREG = (volatile REG_t*)gulFlashIP_RegBase[ubChannel];
			if (ubChannel == 0) {
				ubCheckCEPerCh = 2;
			}
			for (ubCE = 0; ubCE < ubCheckCEPerCh; ubCE++) { //掃前面兩個CE 確認3個die的CE有幾個 幾個3die就配幾個2die
#if(UFS)
				if (1)
#else
				if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
				{
					FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, ubCE);//((BIT7 |(ubCE << 1)) << (ubChannel * 8));
				}
				else {
					FCONL[FCONL_FCE_ENB] = (BIT0 << (ubCE + ubChannel * 8));
				}
				gFREG[(FCTLL_PIO_CMD)].L = 0xFF;
				gFREG[FCTLL_PIO_CMD].L = 0x70;
				ulDelay = 0;
				do {
					ubStatus = 0;
					gFREG[FCTLL_IO_SET].L |= FLH_PHY_IF_RESET;
					ubStatus = gFREG[FCTLL_PIO_DAT].B.B0;
					ulDelay++;
					if (ulDelay > 4000) {
						break;
					}
				}
				while ( !(ubStatus & BIT7));
				if (ubStatus & BIT7) {
					//power on reset
					ulDelay = 0;
					do {
						ubStatus = 0;
						gFREG[FCTLL_IO_SET].L |= FLH_PHY_IF_RESET;
						ubStatus = gFREG[FCTLL_PIO_DAT].B.B0;
						ulDelay++;
						if (ulDelay > 4000) {
							break;
						}
					}
					while (ubStatus != 0xE0);
					if ((ubStatus == 0xE0) && (ubChannel == 0)) {
						gubHideCE++;
					}
				}
			}
			if ((ubStatus != 0xE0) && ubChannel) { // No Channel 1
				continue;
			}
			if ( ENABLE_DEBUG_UART_SANDISK_1ZNM_SPF) {
				if (ubChannel == 0) {
					UartString("\n\rPhyCE:");
					Uart_Tx_DataHex(gubHideCE);
				}
			}
			ubHideCEFind = 0;
			for (ubCE = 2; ubCE < 4; ubCE++) {// 2個die的CE必須放在index2
				//M_DisableAllFlashCE();
#if(UFS)
				if (1)
#else
				if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
				{
					FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, ubCE);//((BIT7 |(ubCE << 1)) << (ubChannel * 8));
				}
				else {
					FCONL[FCONL_FCE_ENB] = (BIT0 << (ubCE + ubChannel * 8));
				}
				ulAdr = 24576 << 8;
				//1  First_Step_Read_ID
				gFREG[(FCTLL_PIO_CMD)].L = 0xFF;
				gFREG[FCTLL_PIO_CMD].L = 0x70;
				ulDelay = 0;
				do {
					ubStatus = 0;
					ubStatus = gFREG[FCTLL_PIO_DAT].B.B0;
					ulDelay++;
					if (ulDelay > 4000) {
						break;
					}
				}
				while (ubStatus != 0xE0);
				gFREG[(FCTLL_PIO_CMD)].L = 0x90;
				gFREG[(FCTLL_PIO_ADR)].B.B0  = 0x00;
				gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First
				if ( ENABLE_DEBUG_UART_SANDISK_1ZNM_SPF) {
					UartString("\n\rCh:");
					Uart_Tx_DataHex(ubChannel);
					UartString("  CE:");
					Uart_Tx_DataHex(ubCE);
					UartString("\n\rID1:");
				}
				for (ubi = 0; ubi < 8; ubi++) {
					if (ubi == 7) {
						gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;
					}

					ubID = gFREG[FCTLL_PIO_DAT].B.B0;
					if (ENABLE_DEBUG_UART_SANDISK_1ZNM_SPF) {
						Uart_Tx_DataHex(ubID);
					}
					if (ubi == 0) {
						gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST)); //First
					}
				}
				gFREG[FCTLL_HS_MODE].L |= ((PIO_DAT_FIRST | PIO_DAT_LAST));
				//2 Second_Step_Dummy_Read
				gFREG[(FCTLL_PIO_CMD)].L = 0x00;
				gFREG[(FCTLL_PIO_ADR)].B.B0 = 0x00;
				gFREG[(FCTLL_PIO_ADR)].B.B0 = 0x00;
				gFREG[(FCTLL_PIO_ADR)].B.B0 = ulAdr & 0xFF;
				gFREG[(FCTLL_PIO_ADR)].B.B0 = (ulAdr >> 8) & 0xFF;
				gFREG[(FCTLL_PIO_ADR)].B.B0 = (ulAdr >> 16) & 0xFF;
				gFREG[(FCTLL_PIO_CMD)].L = 0x30;
				gFREG[FCTLL_PIO_CMD].L = 0x70;			//Send 0x70 to check status
				ulDelay = 0;
				do {
					ubStatus = 0;
					ubStatus = gFREG[FCTLL_PIO_DAT].B.B0;
					ulDelay++;
					if (ulDelay > 4000) {
						break;
					}
				}
				while (ubStatus != 0xE0);
				if (ubStatus != 0xE0) {
					continue;
				}
				else {
					ubHideCEFind++;
				}
				//3 Third_Step_Read_ID
				gFREG[(FCTLL_PIO_CMD)].L = 0xFF;
				gFREG[FCTLL_PIO_CMD].L = 0x70;
				ulDelay = 0;
				do {
					ubStatus = 0;
					ubStatus = gFREG[FCTLL_PIO_DAT].B.B0;
					ulDelay++;
					if (ulDelay > 4000) {
						break;
					}
				}
				while (ubStatus != 0xE0);
				gFREG[(FCTLL_PIO_CMD)].L = 0x90;
				gFREG[(FCTLL_PIO_ADR)].B.B0  = 0x00;
				gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First
				if (ENABLE_DEBUG_UART_SANDISK_1ZNM_SPF) {
					UartString("\n\rID2:");
				}
				for (ubi = 0; ubi < 8; ubi++) {
					if (ubi == 7) {
						gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;
					}

					ubID = gFREG[FCTLL_PIO_DAT].B.B0;
					if (ENABLE_DEBUG_UART_SANDISK_1ZNM_SPF) {
						Uart_Tx_DataHex(ubID);
					}
					if (ubID != gubFlashID[ubi]) {
						if (!BURNER) {
							gubIsWriteProtect = BIT_FLASH_INIT_FAIL_WRITEPROTECT;
						}
						else {
							guoFlashDefaultType.B.btNoSupport = TRUE;
							SetInitialFailID(BIT10, 0);
						}
					}
					if (ubi == 0) {
						gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST)); //First
					}
				}
				gFREG[FCTLL_HS_MODE].L |= ((PIO_DAT_FIRST | PIO_DAT_LAST));
				if (ubHideCEFind == gubHideCE) {
					break;
				}
			}
			if (ubHideCEFind != gubHideCE) {
				if (!BURNER) {
					gubIsWriteProtect = BIT_FLASH_INIT_FAIL_WRITEPROTECT;
				}
				else {
					guoFlashDefaultType.B.btNoSupport = TRUE;
					SetInitialFailID(BIT10, 0);
				}
			}
		}
		if (gubFlashMode_CE[0] & BIT0) {
			FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
		}
	}
	guoFlashDefaultType.B.btSNDK160G = TRUE;

}
#endif

#if ENABLE_FORCE_L06B_TO_B0KB
void ForceL06BToB0KB(UBYTE ubFirstSetB0KB)
{
	if (guoFlashDefaultType.B.btIntelMicron == TRUE) {
		UBYTE ubChannel, ubCE, ubStatus, ubi, ubFailCnt;
		UBYTE ubFeatureCheck[4] = {0};
		UBYTE ubFeature[4] = {0};
		U32 ulDelayLoop;
		for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + (ubChannel * 0x200));
			for (ubCE = 0; ubCE < MAX_CE_PER_CH; ubCE++) {
#if(UFS)
				if (1)
#else
				if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
				{
					//FCONB[FCONB_FCE_ENB_CH0 + ubChannel] = M_MapToCEDecoderMT(gubCH_CEMappingToPCE[ubChannel][ubCE]);
					FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, gubCH_CEMappingToPCE[ubChannel][ubCE]); // Enable selected CE
				}
				else {
					FCONL[FCONL_FCE_ENB] = ((UWORD)BIT0 << (ubCE + (ubChannel * MAX_CE_PER_CH) ));
				}
				if (guoFlashUseType.B.btNVDDR || guoFlashUseType.B.btNVDDR2) { //MicronFlashOnly && ( guoFlashUseType.B.btNVDDR || guoFlashUseType.B.btNVDDR2)) {
					gFREG[FCTLL_PIO_CMD].L = 0xFC;
				}
				else {
					gFREG[FCTLL_PIO_CMD].L = 0xFF;
				}
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;		//Send 0x70 to check status

				ulDelayLoop = 0;
				do {
					ubStatus = 0;
					gFREG[FCTLL_IO_SET].L |= FLH_PHY_IF_RESET;
					ubStatus = (UBYTE) gFREG[FCTLL_PIO_DAT].L;
					ulDelayLoop++;

					if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator) {
						if (ulDelayLoop > 10) {
							break;
						}
					}
					else {
						if (ulDelayLoop > 10000) {
							break;
						}
					}
				}
				while ( !(ubStatus & BIT7));

				if (ubStatus & BIT7) {
					//power on reset
					ulDelayLoop = 0;
					do {
						ubStatus = 0;
						gFREG[FCTLL_IO_SET].L |= FLH_PHY_IF_RESET;
						ubStatus = (UBYTE) gFREG[FCTLL_PIO_DAT].L;
						ulDelayLoop++;
						if (ulDelayLoop > 4000) {
							break;
						}
					}
					while (ubStatus != 0xE0);
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x90;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
					gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First
					if (ENABLE_DEBUG_UART_CHECK_CHANGE_ID) {
						UartString("\n\rFlashIDS:");
					}
					if ((gFREG[FCTLL_FLH_SET].B.B0 & FLA_ONFI_MODE) == FLA_ONFI_MODE) {
						gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
						for (ubi = 0; ubi < 2; ubi++) {
							gubFlashID[ubi] = (UBYTE)gFREG[FCTLL_PIO_DAT].L;
						}
					}
					else {
						for (ubi = 0; ubi < 2; ubi++) {
							if (ubi == 1) {
								gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;
							}

							gubFlashID[ubi] = (UBYTE)gFREG[FCTLL_PIO_DAT].L;

							if (ubi == 0) {
								gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST)); //First
							}
							if (ENABLE_DEBUG_UART_CHECK_CHANGE_ID) {
								Uart_Tx_DataHex(gubFlashID[ubi]);
							}
						}
						gFREG[FCTLL_HS_MODE].L |= ((PIO_DAT_FIRST | PIO_DAT_LAST));
					}

					if ( (gubFlashID[0] != 0x89) || ((gubFlashID[1] != 0xa4 ) && ( gubFlashID[1] != 0xc4))) { //如果不是intel或者不屬於L06B的就不做切換
						continue;
					}

					ubFailCnt = 0;
					if (ENABLE_DEBUG_UART_CHECK_CHANGE_ID) {
						flaGetFeature(ubChannel, 0x85, &ubFeatureCheck[0]);
						UartString("\n\rCh:");
						Uart_Tx_DataHex(ubChannel);
						UartString("  CE:");
						Uart_Tx_DataHex(ubCE);
						UartString("\n\rCheck1:");
						for (ubi = 0; ubi < 4; ubi++) {
							Uart_Tx_DataHex(ubFeatureCheck[ubi]);
						}
					}
				SET_L06B_TO_B0KB:
					ubFeature[0] = (BIT0 | BIT1);
					if (ubFirstSetB0KB) { //(ubFeatureCheck[0] & 3) == 0){
						flaSetFeature(ubChannel, 0x85, &ubFeature[0]);
						if (ENABLE_DEBUG_UART_CHECK_CHANGE_ID) {
							UartString("\n\rSet:");
							for (ubi = 0; ubi < 4; ubi++) {
								Uart_Tx_DataHex(ubFeature[ubi]);
							}
						}
					}

					if (guoFlashUseType.B.btNVDDR || guoFlashUseType.B.btNVDDR2) { //MicronFlashOnly && ( guoFlashUseType.B.btNVDDR || guoFlashUseType.B.btNVDDR2)) {
						gFREG[FCTLL_PIO_CMD].L = 0xFC;
					}
					else {
						gFREG[FCTLL_PIO_CMD].L = 0xFF;
					}

					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					gFREG[FCTLL_PIO_CMD].L = 0x70; //Status Read
					UBYTE ubRet = (UBYTE)gFREG[FCTLL_PIO_DAT].B.B0;
					while (ubRet != 0xE0) {
						gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
						ubRet = (UBYTE)gFREG[FCTLL_PIO_DAT].B.B0;
						ulDelayLoop++;
						//Uart_Printf("Read ID Poll Status:%l\r\n", ubRet);
						if (ulDelayLoop > 40000) {
							//Uart_Printf("CE %d doesn't exist!!\r\n",ubFlashCE);
							break;
						}
					}
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x90;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
					gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First
					if (ENABLE_DEBUG_UART_CHECK_CHANGE_ID) {
						UartString("\n\rFlashIDS:");
					}

					if ((gFREG[FCTLL_FLH_SET].B.B0 & FLA_ONFI_MODE) == FLA_ONFI_MODE) {
						gFREG[FCTLL_HS_MODE].L |= ((PIO_DAT_FIRST | PIO_DAT_LAST));
						for (ubi = 0; ubi < 8; ubi++) {
							gubFlashID[ubi] = (UBYTE)gFREG[FCTLL_PIO_DAT].L;
						}
					}
					else {
						for (ubi = 0; ubi < 8; ubi++) {
							if (ubi == 7) {
								gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;
							}

							gubFlashID[ubi] = (UBYTE)gFREG[FCTLL_PIO_DAT].L;

							if (ubi == 0) {
								gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST)); //First
							}
							if (ENABLE_DEBUG_UART_CHECK_CHANGE_ID) {
								Uart_Tx_DataHex(gubFlashID[ubi]);
							}
						}
						gFREG[FCTLL_HS_MODE].L |= ((PIO_DAT_FIRST | PIO_DAT_LAST));
					}

					if ((gubFlashID[1] != 0xB4) && (gubFlashID[1] != 0xCC)) {
						if (ubFirstSetB0KB) {
							ubFailCnt++;
							if (ubFailCnt < 5) {
								goto SET_L06B_TO_B0KB;
							}
							else {
								guoFlashDefaultType.B.btNoSupport = TRUE;
								SetInitialFailID(BIT10, 0);
								if (!BURNER) {
									gubIsWriteProtect = BIT_FLASH_INIT_FAIL_WRITEPROTECT;
									return;
								}
							}
						}
						else {
							guoFlashDefaultType.B.btNoSupport = TRUE;
							SetInitialFailID(BIT10, 0);
							if (!BURNER) {
								gubIsWriteProtect = BIT_FLASH_INIT_FAIL_WRITEPROTECT;
								return;
							}
						}
					}
					flaGetFeature(ubChannel, 0x85, &ubFeatureCheck[0]);
					if (ENABLE_DEBUG_UART_CHECK_CHANGE_ID) {
						UartString("\n\rCheck2:");
					}
					for (ubi = 0; ubi < 4; ubi++) {
						if (ENABLE_DEBUG_UART_CHECK_CHANGE_ID) {
							Uart_Tx_DataHex(ubFeatureCheck[ubi]);
						}
						if (ubFeatureCheck[ubi] != ubFeature[ubi]) {
							ubFailCnt++;
							if (ubFailCnt < 5) {
								goto SET_L06B_TO_B0KB;
							}
							else {
								guoFlashDefaultType.B.btNoSupport = TRUE;
								SetInitialFailID(BIT10, 0);
								if (!BURNER) {
									gubIsWriteProtect = BIT_FLASH_INIT_FAIL_WRITEPROTECT;
									return;
								}
							}
						}
					}
					//Set feature for forcing L06B to B0KB
				}
			}
		}
	}
}
#endif

void InitVaribleBeforeLoadSystemBlock()
{
	gubSectorsPerPlane = guoFlashUseType.B.btFlash16k ? 32 : guoFlashUseType.B.btFlash8k ? 16 : guoFlashUseType.B.btFlash4k ? 8 : 4;
	gubSectorsPerPlaneMask = gubSectorsPerPlane - 1;
	gubSectorsPerPlaneLog = flaCheckLogValue((U32)gubSectorsPerPlane);

	gulZipMap = guoFlashUseType.B.btFlash16k ? 0xFFFFFFFF : guoFlashUseType.B.btFlash8k ? 0x0000FFFF : guoFlashUseType.B.btFlash4k ? 0x000000FF : 0;

	gubBurstsPerBank = ((guoFlashUseType.B.btFourPlane) ? 4 : (guoFlashUseType.B.btTwoPlane) ? 2 : 1);
	gubBurstsPerBankMask = gubBurstsPerBank - 1;
	gubBurstsPerBankLog = flaCheckLogValue((U32)gubBurstsPerBank);

	gub4kEntrysPerPlane = gubSectorsPerPlane / 8;
	gub4kEntrysPerPlaneMask = gub4kEntrysPerPlane - 1;
	gub4kEntrysPerPlaneLog = flaCheckLogValue((U32)gub4kEntrysPerPlane);

	gubPlanesPerBurst = MAX_CHANNEL;
	gubPlanesPerBurstMask = gubPlanesPerBurst - 1;
	gubPlanesPerBurstLog = flaCheckLogValue((U32)gubPlanesPerBurst);
#if (!B0KB)
	gubCENumber = MAX_CE; //B0KB has scaned CE number first 
#endif
	gulPlanesPerUnit = 0xFFFFFFFF;
	gub4kEntrysPerPageLog = 0xFF;//for read retry of system unit
	guw4kEntrysPerPage = 0xFFFF;//for read retry of system unit
}

UBYTE flaCheckLogValue(U32 ulDataIn)
{
	if (ulDataIn)
		return (31 - __builtin_clz(ulDataIn));
	return 0;
}


void flaCheckAllFlashDefaultMode(void)
{

}

void flaSetFlashUseType(UBYTE ubMode)
{

#if Hynix
	UBYTE ubTempFUT = guoFlashUseType.B.btToggle;
	guoFlashUseType = guoFlashDefaultType;
	guoFlashUseType.B.btToggle = ubTempFUT;
#else
	guoFlashUseType = guoFlashDefaultType;
#endif


	if (ubMode == 0) {
		guoFlashUseType.B.btTwoPlane &= ENABLE_TWOPLANE;
		guoFlashUseType.B.btTwoPlaneRead &= ENABLE_TWOPLANEREAD;
		guoFlashUseType.B.btFourPlane &= ENABLE_FOURPLANE;
	}
	else {
		if (gubMultiPlaneNum == MULTIPLANE_ONE) { //one plane
			guoFlashUseType.B.btTwoPlane &= 0;
			guoFlashUseType.B.btTwoPlaneRead &= 0;
			guoFlashUseType.B.btFourPlane &= 0;
		}
		else if (gubMultiPlaneNum == MULTIPLANE_TWO) { //two plane
			guoFlashUseType.B.btTwoPlane &= 1;
			guoFlashUseType.B.btTwoPlaneRead &= 1;
			guoFlashUseType.B.btFourPlane &= 0;
		}
		else if (gubMultiPlaneNum == MULTIPLANE_FOUR) { //four plane
			guoFlashUseType.B.btTwoPlane &= 1;
			guoFlashUseType.B.btTwoPlaneRead &= 1;
			guoFlashUseType.B.btFourPlane &= 1;
		}
		else {
			guoFlashUseType.B.btTwoPlane &= ENABLE_TWOPLANE;
			guoFlashUseType.B.btTwoPlaneRead &= ENABLE_TWOPLANEREAD;
			guoFlashUseType.B.btFourPlane &= ENABLE_FOURPLANE;
		}

		guoFlashUseType.B.btA2Cmd &= gubA2CmdEnable;
	}

#if B0KB //force two plane base on ce number
	if ((gubCENumber == 5) || (gubCENumber == 10) || (gubCENumber == 11)) {
		guoFlashUseType.B.btTwoPlane = 1;
		guoFlashUseType.B.btTwoPlaneRead = 1;
		guoFlashUseType.B.btFourPlane = 0;
		//UartString("\r\nF2 ");
		//Uart_Tx_DataHex(gubCENumber);
	}
#endif

	if (ENABLE_FORCE_TWOPLANE) {
		guoFlashUseType.B.btTwoPlane = 1;
		guoFlashUseType.B.btTwoPlaneRead = 1;
		guoFlashUseType.B.btFourPlane = 0;
	}

	guoFlashUseType.B.btEDO &= ENABLE_EDO;
	guoFlashUseType.B.btCacheProgram &= ENABLE_CACHEPROGRAM;
	guoFlashUseType.B.btCacheRead &= ENABLE_CACHEREAD;
	guoFlashUseType.B.btRandCacheRead &= ENABLE_RANDCACHEREAD;
#if (!MicronFlashOnly)
	if (ENABLE_RANDCACHEREAD && (gubCENumber == 16)) {
		guoFlashUseType.B.btRandCacheRead = FALSE;
	}
#endif

	if (ENABLE_RANDCACHEREAD && ((guoFlashUseType.B.btMultiDie))) {
		guoFlashUseType.B.btRandCacheRead = FALSE;
	}

}

void flaSetParameterByFlashDefaultType()
{
	if ((guoFlashDefaultType.B.btToshiba1Ynm | guoFlashDefaultType.B.btToshiba1Znm | guoFlashDefaultType.B.btToshiba32nm | guoFlashDefaultType.B.btToshiba24nm | guoFlashDefaultType.B.btToshiba19nm) && guoFlashDefaultType.B.bt4LC) {
		guoFlashDefaultType.B.btA2Cmd = 1;
	}
#if TSB_BICS4_SUPPORT
	else if ((guoFlashDefaultType.B.btToshiba1Znm | guoFlashDefaultType.B.btToshibaBiCs2 | guoFlashDefaultType.B.btToshibaBiCs3 | guoFlashDefaultType.B.btToshibaBiCs4) && ( guoFlashDefaultType.B.bt8LC)) {
#else
	else if ((guoFlashDefaultType.B.btToshiba1Znm | guoFlashDefaultType.B.btToshibaBiCs2 | guoFlashDefaultType.B.btToshibaBiCs3) && ( guoFlashDefaultType.B.bt8LC)) {
#endif
		guoFlashDefaultType.B.btA2Cmd = 1;
	}
	else if (ENABLE_HYNIX_SLC && guoFlashDefaultType.B.btHynix && guoFlashDefaultType.B.bt4LC && guoFlashDefaultType.B.btHynixF20Bdie ) {
		guoFlashDefaultType.B.btA2Cmd = 1;
	}
	else if ( guoFlashDefaultType.B.btIntelMicron && ENABLE_MICRON_SLC ) {
		if ( flaCheckMicronSLCIMode_CE() == 0 ) {
			guoFlashDefaultType.B.btA2Cmd = 1;
			guoFlashDefaultType.B.btRandCacheRead = 0;
		}
	}
#if Hynix
	else if (ENABLE_HYNIX_SLC && guoFlashDefaultType.B.btHynix && guoFlashDefaultType.B.bt8LC) {
		guoFlashDefaultType.B.btA2Cmd = 1;
	}
#endif
	if (guoFlashDefaultType.B.bt8LC && guoFlashDefaultType.B.btToshiba && guoFlashDefaultType.B.btFlash8k) {
		guoFlashDefaultType.B.btPseudoPlaneEn = 1;
	}
	else {
		guoFlashDefaultType.B.btPseudoPlaneEn = 0;
	}

	gulTotalPhyUnitNum = 0;
	switch (gubFlashID[1]) {       //gulTotalPhyUnitNum base on 2k/128pages flash
		case 0xA3 :
		case 0xD3 : //1GB
			gubDensitysize = FLH_Size_8Gb;
			gulTotalPhyUnitNum = ((U32)BIT0 << 12);
			gubDieSize = 1 / gubDieNumber;
			break;
		case 0xA5 :
		case 0xD5 : //2GB
			gubDensitysize = FLH_Size_16Gb;
			gulTotalPhyUnitNum = ((U32)BIT0 << 13);
			if (guoFlashDefaultType.B.bt4LC) {
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash8k) {
						if (guoFlashDefaultType.B.btToshiba32nm) {
							guwExtendBlocks = 2;// 14 * 2
						}
					}
					else {
						guwExtendBlocks = 128; //??
					}
				}
			}

			gubDieSize = 2 / gubDieNumber;
			break;
		case 0x44: // micron L83A  4G
		case 0x68:      // intel 4G
		case 0xD7 : //4GB
			gubDensitysize = FLH_Size_32Gb;
			gulTotalPhyUnitNum = ((U32)BIT0 << 14);
			if (guoFlashDefaultType.B.bt4LC) {
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash16k) {
						if (guoFlashDefaultType.B.btToshiba19nm) {
							guwExtendBlocks = 36;
						}
						else if (guoFlashDefaultType.B.btToshiba1Znm) {
							guwExtendBlocks = 1056U - 1024U; //TC58TEG5DCLTA00
						}
					}
					else if (guoFlashDefaultType.B.btFlash8k) {
						if (guoFlashDefaultType.B.btToshiba32nm) {
							if (guoFlashDefaultType.B.btTwoPlane) {
								guwExtendBlocks = 2 * 2;
							}
						}
					}
					else {
						guwExtendBlocks = 256; //??
					}
				}
			}
			else if (guoFlashDefaultType.B.btSLC) {
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash8k) {
						if (guoFlashDefaultType.B.btToshiba32nm) {
							///
						}
						else {
							// TC58NVG6E2HTA00   -YK -
							guwExtendBlocks = 30 * 2; //?
						}
					}
				}
			}

			gubDieSize = 4 / gubDieNumber;
			break;
		case 0x64:      // L84A
		case 0x88: // intel 8G  L74A
		case 0xDE :     //8GB
			gubDensitysize = FLH_Size_64Gb;
			gulTotalPhyUnitNum = ((U32)BIT0 << 15);
			if (guoFlashDefaultType.B.bt4LC) {
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash16k) {
						if (guoFlashDefaultType.B.btToshiba19nm) {
							if (guoFlashDefaultType.B.btTwoPlane) {     // type C
								guwExtendBlocks = 34 * 2;
							}
							else {  // type B
								guwExtendBlocks = 44;
							}
						}
						else if (guoFlashDefaultType.B.btToshiba1Ynm ) {
							if (guoFlashDefaultType.B.btTwoPlane) {
								guwExtendBlocks = 42 * 2;
							}
							else {
								guwExtendBlocks = 36;
							}
						}
						else if (guoFlashDefaultType.B.btToshiba1Znm ) {
							if (guoFlashDefaultType.B.btTwoPlane) {
								guwExtendBlocks = 50 * 2;
							}
						}
						else if (guoFlashDefaultType.B.btHynix ) {
							if (guoFlashDefaultType.B.btTwoPlane) {
								// F20 B-Die
								guwExtendBlocks = 42 * 2;
							}
						}
						else if (guoFlashDefaultType.B.btIntelMicron ) {
							// 16k 512page  L84C
							guwExtendBlocks = 12 * 2;
						}
					}
					else if (guoFlashDefaultType.B.btFlash8k) {
						if (guoFlashDefaultType.B.btToshiba24nm) {
							if (guoFlashDefaultType.B.btToshiba24nmABL) {
								guwExtendBlocks = 34 * 2;
							}
							else {  // 24nm HBL
								guwExtendBlocks = 14 * 2;
							}
						}
					}
				}
			}
			else if (guoFlashDefaultType.B.btSLC) {
				if (guoFlashDefaultType.B.btToshiba24nm) {
					if (guoFlashDefaultType.B.btToshiba24nmABL) {
						if (guoFlashDefaultType.B.btTwoPlane) {
							guwExtendBlocks = 30 * 2; // TH58NVG7H2HTA20 -CY-
						}
					}
				}
			}

			gubDieSize = 8 / gubDieNumber;
			break;
		case 0x84: //
		case 0x4C: // 0x4C: include Sandisk TLC
		case 0x3A :     //16GB
#if Hynix
		case 0x5A: // 0x5A: include Hynix 1.8v TLC, 0x3A: include Hynix 3.3v TLC
#endif
			gubDensitysize = FLH_Size_128Gb;
			gulTotalPhyUnitNum = ((U32)BIT0 << 16);
			if (guoFlashDefaultType.B.bt4LC) {
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash16k) {
						if (guoFlashDefaultType.B.btToshiba19nm) {
							if (guoFlashDefaultType.B.btTwoPlane) {
								guwExtendBlocks = 34 * 2;
							}
							else {
								guwExtendBlocks = 44;
							}
						}
						else if (guoFlashDefaultType.B.btToshiba1Ynm) {
							if (guoFlashDefaultType.B.btFourPlane) {
								guwExtendBlocks = 45 * 4;
							}
							else if (guoFlashDefaultType.B.btTwoPlane) {
								guwExtendBlocks = 42 * 2;
							}
						}
						else if (guoFlashDefaultType.B.btToshiba1Znm) {
							if (guoFlashDefaultType.B.btMultiDie) {
								if (guoFlashDefaultType.B.btTwoPlane) {
									guwExtendBlocks = 50 * 2 ;
								}
							}
							else {
								if (guoFlashDefaultType.B.btFourPlane) {
									guwExtendBlocks = 39 * 4 ;
								}
								else if (guoFlashDefaultType.B.btTwoPlane) {
									guwExtendBlocks = 58 * 2 ;
								}
							}
						}
						else if (guoFlashDefaultType.B.btIntelMicron ) {
							// 16k 512page  L84C
							guwExtendBlocks = 12 * 2;
						}
					}
				}
				else if (guoFlashDefaultType.B.btFlash8k) {
					if (guoFlashDefaultType.B.btToshiba24nm) {
						if (guoFlashDefaultType.B.btTwoPlane) {     // TH58TEG9E2HBA89 (8K eMLC) -CY-
							guwExtendBlocks = 34 * 2;
						}
					}
				}
			}
			else if (guoFlashDefaultType.B.bt8LC) {

				if (guoFlashDefaultType.B.btSandisk) { //GARY
					if (guoFlashDefaultType.B.btToshibaBiCs3) {
						guwExtendBlocks = ((684 * 2) + (74 * 2)) - 1024U; // 1516 - 1024, 配合計算架構
						gubMIN_D1_UNIT_NUM_BiCS3 = MIN_D1_UNIT_NUM_BICS3_128Gb;
					}
					else if (guoFlashDefaultType.B.btTwoPlane) {
						guwExtendBlocks = (1446U - 1024U) * 2;
					}
				}
				else if (guoFlashDefaultType.B.btHynix) {
					// in order to normalize the formular of gulTotalPhyUnitNum, make the phys. block 2^n + x - 2^n belong to ext. block
					if ((gubFlashID[5] & 0xF0) == 0x50) {
						guwExtendBlocks = 62 * 2 + (2732 - 2048);   // TODO: confirm 60 x 2 or 62 x 2
					}
					else if ((gubFlashID[5] & 0xF0) == 0x20) {
						guwExtendBlocks = 60 * 2;
					}
					gulTotalPhyUnitNum *= gubDieNumber;
				}
				else {
					//Toshiba
					if (guoFlashDefaultType.B.btToshibaBiCs3) {
						guwExtendBlocks = ((684 * 2) + (74 * 2)) - 1024U; // 1516 - 1024, 配合計算架構
						gubMIN_D1_UNIT_NUM_BiCS3 = MIN_D1_UNIT_NUM_BICS3_128Gb;
					}
					else {
						guwExtendBlocks = (2892U - 2048U);
					}
				}

			}
			if (guoFlashDefaultType.B.btHynix) {
				gubDieSize = 16;
			}
			else {
				gubDieSize = 16 / gubDieNumber;
			}
			break;
		case 0xA4://L06B, 32GB
			if (guoFlashDefaultType.B.btIntelMicron) {
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash16k) {
						if (guoFlashDefaultType.B.btFourPlane)
							guwExtendBlocks = 36 * 4;			// (548 - 512)*4, refer to spec.
					}
				}
			}
			// gulTotalPhyUnitNum is calculated base on 2KB/Page and 128 Pages, exactly unit number will be altered after.
			gulTotalPhyUnitNum = ((U32)BIT0 << 17);
			break;
		case 0xB4: //B0KB, 48GB
			gubDensitysize = FLH_Size_384Gb;
			if (guoFlashDefaultType.B.btIntelMicron) {
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash16k) {
						if (guoFlashDefaultType.B.btFourPlane)
							guwExtendBlocks = 36 * 4;			// (548 - 512)*4, refer to spec.
					}
				}
			}
			// gulTotalPhyUnitNum is calculated base on 2KB/Page and 128 Pages, exactly unit number will be altered after.
			//gulTotalPhyUnitNum = 196608;
			gulTotalPhyUnitNum = ((U32)BIT0 << 18);
			break;
		case 0xC4://L06B, 64GB
			if (guoFlashDefaultType.B.btIntelMicron) {
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash16k) {
						if (guoFlashDefaultType.B.btFourPlane)
							guwExtendBlocks = 36 * 4;			// (548 - 512)*4, refer to spec.
					}
				}
			}
			// gulTotalPhyUnitNum is calculated base on 2KB/Page and 128 Pages, exactly unit number will be altered after.
			gulTotalPhyUnitNum = ((U32)BIT0 << 18);
			gubDensitysize = FLH_Size_512Gb;
			gubDieNumber = 2;
			gubSelDieNumber = 2;
			guoFlashDefaultType.B.btMultiDie = TRUE;
			break;
		case 0xCC://B0KB, 96GB
			gubDensitysize = FLH_Size_384Gb;
			if (guoFlashDefaultType.B.btIntelMicron) {
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash16k) {
						if (guoFlashDefaultType.B.btFourPlane)
							guwExtendBlocks = 36 * 4;			// (548 - 512)*4, refer to spec.
					}
				}
			}
			// gulTotalPhyUnitNum is calculated base on 2KB/Page and 128 Pages, exactly unit number will be altered after.
			gulTotalPhyUnitNum = ((U32)BIT0 << 19);
			gubDieNumber = 2;
			gubSelDieNumber = 2;
			guoFlashDefaultType.B.btMultiDie = TRUE;
			break;
		case 0x3C: //32GB
			gubDensitysize = FLH_Size_256Gb;
			// gulTotalPhyUnitNum is calculated base on 2KB/Page and 128 Pages, exactly unit number will be altered after.
#if TLC_BICS2
			gubDensitysize -= (gubFlashID[2] & 3); //要除Die Number => Memory Density / (flashID[2] & (BIT0|BIT1))
#endif
			if (guoFlashDefaultType.B.bt8LC) {
				gulTotalPhyUnitNum = ((U32)BIT0 << 17);
				if (guoFlashDefaultType.B.btToshibaBiCs2) {
					if (guoFlashDefaultType.B.btTwoPlane) {
						guwExtendBlocks = ((1821 * 2) + (151 * 2)) - 2048U; // 3944 - 2048, 配合計算架構
					}
				}
				else if (guoFlashDefaultType.B.btToshibaBiCs3) {
					if (guoFlashDefaultType.B.btTwoPlane) {
						if ((gubFlashID[2] & 3) == 1) {
							guwExtendBlocks = ((684 * 2) + (74 * 2)) - 1024U; // 1516 - 1024, 配合計算架構
							gubMIN_D1_UNIT_NUM_BiCS3 = MIN_D1_UNIT_NUM_BICS3_128Gb;
						}
						else {
							guwExtendBlocks = ((1366 * 2) + (112 * 2)) - 2048U; // 2956 - 2048, 配合計算架構
						}
					}
				}
#if TSB_BICS4_SUPPORT
				else if (guoFlashDefaultType.B.btToshibaBiCs4) {
					if (guoFlashDefaultType.B.btTwoPlane) {
						guwExtendBlocks = ((910 * 2) + (80 * 2)) - 1024U; // 1980 - 1024, 配合計算架構
					}
				}
#endif
				else {
					guwExtendBlocks = (2892U - 2048U);
				}
			}
			else if (guoFlashDefaultType.B.bt4LC) {
				gulTotalPhyUnitNum = ((U32)BIT0 << 17);
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash16k) {
						if (guoFlashDefaultType.B.btToshiba1Ynm) {
							if (guoFlashDefaultType.B.btFourPlane) {
								guwExtendBlocks = 45 * 4;// 45 * 2 * 2;   ///modified by Victor
							}
							else if (guoFlashDefaultType.B.btTwoPlane) {
								guwExtendBlocks = 42 * 2;// 42 * 2* 2;
							}
						}
						else if (guoFlashDefaultType.B.btToshiba1Znm) {
							if (guoFlashDefaultType.B.btFourPlane) {
								guwExtendBlocks = 39 * 4;// 39 * 4* 2;
							}
							else if (guoFlashDefaultType.B.btTwoPlane) {
								guwExtendBlocks = 58 * 2;// 58 * 2* 2;
							}
						}
					}
				}
			}
			else if (guoFlashDefaultType.B.btSLC) {
			}
			gubDieSize = 32 / gubDieNumber;
			break;
		case 0x3e ://64GB
			gubDensitysize = FLH_Size_512Gb;
#if TLC_BICS2
			gubDensitysize -= (gubFlashID[2] & 3); //要除Die Number => Memory Density / (flashID[2] & (BIT0|BIT1))
#endif
			if (guoFlashDefaultType.B.bt8LC) {
				gulTotalPhyUnitNum = ((U32)BIT0 << 18);
				if (guoFlashDefaultType.B.btToshibaBiCs2) {
					if (guoFlashDefaultType.B.btTwoPlane) {
						guwExtendBlocks = ((1821 * 2) + (151 * 2)) - 2048U; // 3944 - 2048, 配合計算架構
					}
				}
				else if (guoFlashDefaultType.B.btToshibaBiCs3) {
					if (guoFlashDefaultType.B.btTwoPlane) {
						if (guoFlashDefaultType.B.btMultiDie) {	//256Gb
							guwExtendBlocks = ((1366 * 2) + (112 * 2)) - 2048U; // 2956 - 2048, 配合計算架構
							gubMIN_D1_UNIT_NUM_BiCS3 = MIN_D1_UNIT_NUM;
						}
						else {	//512Gb
							guwExtendBlocks = ((2732 * 2) + (226 * 2)) - 4096U; // 5916 - 4096 配合計算架構
							gubMIN_D1_UNIT_NUM_BiCS3 = MIN_D1_UNIT_NUM_BICS3_512Gb;
						}
					}

				}
#if TSB_BICS4_SUPPORT
				else if (guoFlashDefaultType.B.btToshibaBiCs4) {
					if (guoFlashDefaultType.B.btTwoPlane) {
						if (guoFlashDefaultType.B.btMultiDie) {	//256Gb
							guwExtendBlocks = ((910 * 2) + (80 * 2)) - 1024U; // 1980 - 1024, 配合計算架構
						}
						else {	//512Gb
							guwExtendBlocks = ((1822 * 2) + (136 * 2)) - 2048U; // 3916 - 2048 配合計算架構
							gubMIN_D1_UNIT_NUM_BiCS3 = MIN_D1_UNIT_NUM_BICS4_512Gb;
							gubWriteProtect_NUM = WriteProtect_NUM_Bics4_512Gb;
						}
					}
				}
#endif
				else {
					guwExtendBlocks = (2892U - 2048U);
				}
			}
			else if (guoFlashDefaultType.B.bt4LC) {
				gulTotalPhyUnitNum = ((U32)BIT0 << 18);
				if (guoFlashDefaultType.B.btExtendBlock) {
					if (guoFlashDefaultType.B.btFlash16k) {
						if (guoFlashDefaultType.B.btToshiba1Ynm) {
							if (guoFlashDefaultType.B.btFourPlane) {
								guwExtendBlocks = 45 * 4;//90 * 2 * 4;
							}
							else if (guoFlashDefaultType.B.btTwoPlane) {
								guwExtendBlocks = 42 * 2;//42 * 2 * 4;
							}
						}
						else if (guoFlashDefaultType.B.btToshiba1Znm) {
							if (guoFlashDefaultType.B.btFourPlane) {
								guwExtendBlocks = 39 * 4;//78 * 2 * 4;
							}
							else if (guoFlashDefaultType.B.btTwoPlane) {
								guwExtendBlocks = 58 * 2;//58 * 2 * 4;
							}
						}
					}
				}
			}
			else if (guoFlashDefaultType.B.btSLC) {
			}
			gubDieSize = 64 / gubDieNumber;
			break;

		case 0x48 ://128GB
			gubDensitysize = FLH_Size_1Tb;
#if TLC_BICS2
			gubDensitysize -= (gubFlashID[2] & 3); //要除Die Number => Memory Density / (flashID[2] & (BIT0|BIT1))
#endif
			if (guoFlashDefaultType.B.bt8LC) {
				gulTotalPhyUnitNum = ((U32)BIT0 << 19);
				if (guoFlashDefaultType.B.btToshibaBiCs2) {
					if (guoFlashDefaultType.B.btTwoPlane) {
						guwExtendBlocks = ((1821 * 2) + (151 * 2)) - 2048U; // 3944 - 2048, 配合計算架構
					}
				}
				else if (guoFlashDefaultType.B.btToshibaBiCs3) {
					if (guoFlashDefaultType.B.btTwoPlane) {
						if (guoFlashDefaultType.B.btMultiDie) {
							if ((gubFlashID[2] & 3) == 1) {	// 2Die (512Gb/Die)
								guwExtendBlocks = ((2732 * 2) + (226 * 2)) - 4096U; // 5916 - 4096 配合計算架構
								gubMIN_D1_UNIT_NUM_BiCS3 = MIN_D1_UNIT_NUM_BICS3_512Gb;
							}
							else if ((gubFlashID[2] & 3) == 2) { //256Gb 4Die
								guwExtendBlocks = ((1366 * 2) + (112 * 2)) - 2048U; // 2956 - 2048, 配合計算架構
								gubMIN_D1_UNIT_NUM_BiCS3 = MIN_D1_UNIT_NUM;
							}
						}
					}
				}
			}
			else {
				guoFlashDefaultType.B.btNoSupport = TRUE;
			}
			gubDieSize = 128 / gubDieNumber;
			break;


		case 0x58 : //Sandisk 4CE/10Die CE0,1 3Die, CE2,3 2Die //48GB
			if (!guoFlashDefaultType.B.btSandisk) {
				guoFlashDefaultType.B.btNoSupport = TRUE;
				SetInitialFailID(BIT10, 0);
			}
			else {
				gubDensitysize = FLH_Size_384Gb;
				if (guoFlashDefaultType.B.bt4LC) {
					gulTotalPhyUnitNum = 196608;//Max Block of CE0 & CE1
					if (guoFlashDefaultType.B.btExtendBlock) {
						if (guoFlashDefaultType.B.btFlash16k) {
							if (guoFlashDefaultType.B.btToshiba1Znm) {
								if (guoFlashDefaultType.B.btTwoPlane) {
									guwExtendBlocks = 58 * 2;//58 * 2 * 4;
								}
							}
						}
					}
				}
				gubDieSize = 16;// Each Die is 16G
			}
			break;
#if Hynix3DV6
		case 0x5E:
		case 0x7E:

			if (gubFlashID[2] == 0x28) {
				gubDensitysize = FLH_Size_512Gb;
				gulTotalPhyUnitNum = 911 * 4;
				guwExtendBlocks = 43 * 4;
			}
			else if (gubFlashID[2] == 0x29) {
				gubDensitysize = FLH_Size_512Gb;
				gulTotalPhyUnitNum = 911 * 4 * 2;
				guwExtendBlocks = 43 * 4 * 2;

			}
			else if (gubFlashID[2] == 0x2A) {
				//too big
				flaLEDBlink(1);

			}
			else if (gubFlashID[2] == 0x2B) {
				//too big
				flaLEDBlink(1);
			}
			else {
				flaLEDBlink(1); // no reconize
			}


			break;
		case 0x89:

			if (gubFlashID[2] == 0x28) {
				gubDensitysize = FLH_Size_1Tb;
				gulTotalPhyUnitNum = 1366 * 4;
				guwExtendBlocks = 72 * 4;
			}
			else if (gubFlashID[2] == 0x29) {
				gubDensitysize = FLH_Size_1Tb;
				gulTotalPhyUnitNum = 1366 * 4 * 2;
				guwExtendBlocks = 72 * 4 * 2;
			}
			else if (gubFlashID[2] == 0x2A) {
				//too big
				flaLEDBlink(1);

			}
			else if (gubFlashID[2] == 0x2B) {
				//too big
				flaLEDBlink(1);
			}
			else {
				flaLEDBlink(1); // no reconize
			}

			break;
#endif

#if YMTC
		case 0xC3:
			gubDensitysize = FLH_Size_256Gb;
			gulTotalPhyUnitNum = 1006 * 2;
			guwExtendBlocks = 0 * 2;
			break;
#endif



		default:
			guoFlashDefaultType.B.btNoSupport = TRUE;
			break;
	}

	if (gub4KSLC) {
		guwExtendBlocks = 0;
	}

	if (ENABLE_KT_DEBUG_SIZE) {
		gulTotalPhyUnitNum = 4096;
	}

	// set flash fastpage type
	if (guoFlashDefaultType.B.bt8LC) {
		gubFastPageType = 0;
	}
	else if (guoFlashDefaultType.B.bt4LC) {
		if (guoFlashDefaultType.B.btToshiba || guoFlashDefaultType.B.btHynix) {
			if (guoFlashDefaultType.B.btFlash16k) {
				if (guoFlashDefaultType.B.btHynix) {
					gubFastPageType = 0x4A;
				}
				else {
					gubFastPageType = 0x21;
				}
			}
			else if (guoFlashDefaultType.B.btFlash8k) {
				if (guoFlashDefaultType.B.bt512Page) {
					M_AssertError(1);
				}
				else if (guoFlashDefaultType.B.bt256Page) {
					gubFastPageType = 0x4A;
				}
				else {
					gubFastPageType = 0x21;
				}
			}
			else if (guoFlashDefaultType.B.btFlash4k) {
				gubFastPageType = 0x4A;
			}
		}
		else if (guoFlashDefaultType.B.btIntelMicron) {
			if ((guoFlashDefaultType.B.btFlash8k || guoFlashDefaultType.B.btFlash4k) && guoFlashUseType.B.bt256Page) {
				if ( guoFlashDefaultType.B.btIntelMicronL74A) {
					gubFastPageType = 0x4A;
				}
				else {
					gubFastPageType = 0x4B; //0x4B 無特別意義, 其他type有意義
				}
			}
			else if ((guoFlashDefaultType.B.btFlash16k) && guoFlashDefaultType.B.bt512Page) {
				gubFastPageType = 0x4B; //0x4B 無特別意義, 其他type有意義
			}
			else if ((guoFlashDefaultType.B.btFlash16k) && guoFlashDefaultType.B.bt1024Page) { //L06B
				gubFastPageType = 0x4C;
			}
			else {
				M_AssertError(1);
			}
		}
	}
	else if (guoFlashDefaultType.B.btSLC) {
		gubFastPageType = 0x00;
	}
}

void flaSetParameter(UBYTE ubMode)
{
	//ubMode == 0 : default setting for preformat
	//ubMode == 1 : gain MP setting from system unit
	if (ubMode == 0) {
		gubILMode = INTERLEAVE_8 ;//IL8
		gubUltraMLCEnable = ENABLE_ULTRAMLC;
		gubScanPreformatCENumber = 0;
	}
	else {
		if (gubCENumber_MP == 0) {
			gubScanPreformatCENumber = 0;
		}
		else {
			gubScanPreformatCENumber = 1;
		}
	}

	gulPhyBlocksPerCE = gulTotalPhyUnitNum;

	if (gubExpandCE) {
		gubDieNumber *= gubExpandCE_Ratio;      //restore original die number
	}
	flaScanCE_And_Remapping();
	M_DisableAllFlashCE();
	/* Turn off all CE Micron randomizer */
	flaMicronRandomizer(0);
#if UFS
	UBYTE ubUFSCheckCH;
	for (ubUFSCheckCH = 0; ubUFSCheckCH < 4; ubUFSCheckCH++) {
		if (gubUFSDefaultMode_CE & (UFS_Sample_Exist << ubUFSCheckCH)) {
			if (gubUFSDefaultMode_CE & (UFS_Sample_Default_Mode_Toggle << ubUFSCheckCH)) {
				gubFlashMode_CE[ubUFSCheckCH % 2] |= 0xF << (4 * (ubUFSCheckCH / 2));
				guwDefaultToggleCEMap |= 0xF << (4 * (ubUFSCheckCH / 2) + 8 * (ubUFSCheckCH % 2));
				if (ubUFSCheckCH < 2) {
					if ((gubUFSDefaultMode_CE & (UFS_Sample_Exist << (ubUFSCheckCH + 2))) == 0) {
						gubFlashMode_CE[ubUFSCheckCH % 2] |= ( 0xF << 4);
						guwDefaultToggleCEMap |= 0xF << (4  + 8 * (ubUFSCheckCH % 2));
					}
				}
			}
		}
	}
#else
	if (guoFlashUseType.B.btToggle == 1) {
		gubFlashMode_CE[0] = 0xFF;
		gubFlashMode_CE[1] = 0xFF;
	}
#endif
	gulTotalPhyUnitNum =  (gulTotalPhyUnitNum * gubTotalGroups); //one group的TotalPhyUnitNum * TotalGroups

	//Check Flash Page
	if (guoFlashUseType.B.bt8LC) {
#if Hynix3DV6
		gubNum_D1_Part = guoFlashUseType.B.btWordline_Skip ? 6 : 3;
		if (guoFlashDefaultType.B.btHynix3DV5) {
			guwPagesPerUnit = gubUltraMLCEnable ? 384 : 1152;	//
		}
		else {
			guwPagesPerUnit = gubUltraMLCEnable ? 512 : 1536;	//
		}
		guwFastPagePagesPerUnit = guwPagesPerUnit / gubNum_D1_Part;
		guwNum_Wordline = guwPagesPerUnit / 3;
#elif Hynix3DV5 || YMTC_JGS
		gubNum_D1_Part = guoFlashUseType.B.btWordline_Skip ? 6 : 3;
		guwPagesPerUnit = gubUltraMLCEnable ? 384 : 1152;	//
		guwFastPagePagesPerUnit = guwPagesPerUnit / gubNum_D1_Part;
		guwNum_Wordline = guwPagesPerUnit / 3;
#elif B0KB
		gubNum_D1_Part = guoFlashUseType.B.btWordline_Skip ? 6 : 3;
		guwPagesPerUnit = gubUltraMLCEnable ? 512 : 1536;	//
		guwFastPagePagesPerUnit = guwPagesPerUnit / gubNum_D1_Part;
		guwNum_Wordline = guwPagesPerUnit / 3;
#else
		//guwPagesPerUnit = guoFlashUseType.B.bt576Page ? 576 : (guoFlashUseType.B.bt384Page ? 384 : 258);
#if TSB_BICS4_SUPPORT
		guwPagesPerUnit = guoFlashUseType.B.btToshibaBiCs4 ? 1152 : (guoFlashUseType.B.bt768Page ? 768 : (guoFlashUseType.B.bt576Page ? 576 : (guoFlashUseType.B.bt384Page ? 384 : 258)));
#else
		guwPagesPerUnit = guoFlashUseType.B.bt768Page ? 768 : (guoFlashUseType.B.bt576Page ? 576 : (guoFlashUseType.B.bt384Page ? 384 : 258));
#endif
		gubNum_D1_Part = guoFlashUseType.B.btWordline_Skip ? 6 : 3;
		guwFastPagePagesPerUnit =  guwPagesPerUnit / gubNum_D1_Part;
		guwNum_Wordline = guwPagesPerUnit / 3;
#endif
	}
	else if (guoFlashUseType.B.bt4LC) {
		// need add 256 pages judgement
		if (guoFlashUseType.B.bt1024Page) {
			//FL_FCON_FLH_FUNC_TOP|=(BIT11);       // 1x
			guwPagesPerUnit = gubUltraMLCEnable ? 512 : 1024;
			guwFastPagePagesPerUnit = 512;
		}
		else if (guoFlashUseType.B.bt512Page) {
			//FL_FCON_FLH_FUNC_TOP|=(BIT11);       // 1x
			guwPagesPerUnit = gubUltraMLCEnable ? 256 : 512;
			guwFastPagePagesPerUnit = 256;
		}
		else if (guoFlashUseType.B.bt256Page) {
			//FL_FCON_FLH_FUNC_TOP|=(BIT11);       // 1x
			guwPagesPerUnit = gubUltraMLCEnable ? 128 : 256;
			guwFastPagePagesPerUnit = 128;
		}
		else {
			//FL_FCON_FLH_FUNC_TOP&=(~(BIT10|BIT11)); // 00
			guwPagesPerUnit = gubUltraMLCEnable ? 64 : 128;
			guwFastPagePagesPerUnit = 64;
		}
		gubNum_D1_Part = gubUltraMLCEnable ? 1 : 2;
		guwNum_Wordline = guwFastPagePagesPerUnit;
	}
	else if (guoFlashUseType.B.btSLC) {
		// TC58NVG6E2HTA00   -YK -
		if (gub4KSLC == 0) {
			//FL_FCON_FLH_FUNC_TOP&=(~(BIT10|BIT11)); // 00
			guwPagesPerUnit = 128;
			guwFastPagePagesPerUnit = 128;
		}
		else { //TC58NVG3S0FTA00, TH58NVG4S0FTA20  -CY-
			//FL_FCON_FLH_FUNC_TOP|=BIT10;
			//FL_FCON_FLH_FUNC_TOP&=(~BIT11);       // 01
			guwPagesPerUnit = 64;
			guwFastPagePagesPerUnit = 64;
		}
		gubNum_D1_Part = 1;
		guwNum_Wordline = guwFastPagePagesPerUnit;
	}

	//Check Total Unit Number without Interleave&TwoPlane

	gubPlanesPerPage = gubPlanesPerBurst;

	// gulTotalPhyUnitNum&gulPhyBlocksPerCE use 2k/128 to calculate before
	if (guoFlashUseType.B.btFlash16k) {
		FCONL[FCONL_PAGE_CFG] = PHYSICAL_16KPAGE;

		if (guoFlashUseType.B.bt4LC) {
			if (guoFlashUseType.B.bt1024Page) {
				gulTotalPhyUnitNum /= 64; // 64 = (16K/2K)*(1024pages/128pages) = 8*8
				gulPhyBlocksPerCE /= 64;
			}
			else if (guoFlashUseType.B.bt512Page) {

				gulTotalPhyUnitNum /= 32; // 32 = (16K/2K)*(512pages/128pages) = 8*4
				gulPhyBlocksPerCE /= 32;
			}
			else if (guoFlashUseType.B.bt256Page) {
				// must 256page

				gulTotalPhyUnitNum /= 16;
				gulPhyBlocksPerCE /= 16;
			}
			else {
				M_AssertError(1);
			}
		}
		else if (guoFlashUseType.B.bt8LC) {
			if (guoFlashUseType.B.bt258Page) { /* A19nm TLC */
#if Hynix
				gulTotalPhyUnitNum >>= 4;
				gulPhyBlocksPerCE >>= 4;
#else
				gulTotalPhyUnitNum >>= 3;
				gulPhyBlocksPerCE >>= 3;
#endif
			}
			else if (guoFlashUseType.B.bt384Page) { /* 1Znm TLC */
				gulTotalPhyUnitNum >>= 5;
				gulPhyBlocksPerCE >>= 5;
			}
			else if (guoFlashDefaultType.B.btIntelMicronB0KB) {
				gulTotalPhyUnitNum >>= 7; // 96 = (16K/2K)*(1536pages/128pages) = 8*12
				gulPhyBlocksPerCE >>= 7;
			}
			else if (guoFlashUseType.B.bt576Page || guoFlashUseType.B.bt768Page) { /* BiCs2 TLC and BICs3 TLC*/
				gulTotalPhyUnitNum >>= 6;
				gulPhyBlocksPerCE >>= 6;
			}
#if TSB_BICS4_SUPPORT
			else if (guoFlashUseType.B.btToshibaBiCs4) { /* BiCs4 TLC*/
				gulTotalPhyUnitNum >>= 7;	// 72 = (16K/2K)*(1152pages/128pages) = 8*9
				gulPhyBlocksPerCE >>= 7;
			}
#endif
			else {
				M_AssertError(1);
			}
		}
	}
	else if (guoFlashUseType.B.btFlash8k) {
		FCONL[FCONL_PAGE_CFG] = PHYSICAL_8KPAGE;

		if (guoFlashUseType.B.bt512Page) {
			M_AssertError(1);
		}
		else if (guoFlashUseType.B.bt256Page) {

			gulTotalPhyUnitNum /= 8; // 8 = (8K/2K)*(256pages/128pages) = 4*2
			gulPhyBlocksPerCE /= 8;
		}
		else {

			gulTotalPhyUnitNum /= 4;
			gulPhyBlocksPerCE /= 4;
		}
	}
	else if (guoFlashUseType.B.btFlash4k) {
		FCONL[FCONL_PAGE_CFG] = PHYSICAL_4KPAGE;


		if (guwPagesPerUnit == 64) {
			// 1 = (4K/2K)*(64pages/128pages) = 2*0.5 = 1
		}
		else { //128 pages
			gulTotalPhyUnitNum /= 2; // 2 = (4K/2K)*(128pages/128pages) = 2*1
			gulPhyBlocksPerCE /= 2;
		}
	}
	else {
		//fail
	}

	guwDieBlockShift = 0;
	if (guoFlashUseType.B.btMultiDie) {
		if (ENABLE_MULTI_DIE) {
			if ((guoFlashUseType.B.btToshiba) || (guoFlashUseType.B.btHynix) || (guoFlashUseType.B.btIntelMicron) || (guoFlashUseType.B.btYMTC)) {
				guwDieBlockShift = (gulPhyBlocksPerCE / gubDieNumber) << 1;
#if YMTC_JGS
				guwDieBlockShift = 4096;
#elif Hynix3DV6
				if (guoFlashDefaultType.B.btHynix3DV5) {
					guwDieBlockShift = 4096;
				}
				else {
					guwDieBlockShift = 8192;
				}
#endif
			}
			//else if (guoFlashUseType.B.btIntelMicron) {
			//	guwDieBlockShift = 4096;
			//}
			else {
				M_AssertError(1);
			}
		}
		else {
			gulTotalPhyUnitNum /= 2;
		}
	}

	gubTotalGroups *= gubDieNumber; //前面得CH下的group數, CE內multiDie為不同group, 所以再乘DieNumber

	gulTotalPhyUnitNum += (U32)(guwExtendBlocks * gubTotalGroups);
	gulPhyBlocksPerCE += (guwExtendBlocks * gubDieNumber);

	//Check FourPlane/TwoPlane Status
	U32	ulPlanes;	// Odd CE
	if (guoFlashUseType.B.btFourPlane) {
		gubPlanesPerPage *= 4;
		//gubBurstsPerPage*=4;
		gulTotalPhyUnitNum /= 4;
		guwDieBlockShift /= 4;
		ulPlanes = 4;
	}
	else if (guoFlashUseType.B.btTwoPlane) {
		gubPlanesPerPage *= 2;
		//gubBurstsPerPage*=2;
		gulTotalPhyUnitNum /= 2;
		guwDieBlockShift /= 2;
		ulPlanes = 2;
	}
	else {
	}
	gubDieBlockShiftLog = flaCheckLogValue((U32)guwDieBlockShift);

	//Check Interleave Status
#if	ENABLE_ODD_CE
	U32 ulbase = gubTotalGroups / gubDieNumber;
	gulTotalPhyUnitNum /= ulbase;
	gubTotalGroups /= ulbase;
	gubPlanesPerPage *= ulbase;
	gubInterleave = ulbase;

	if (ENABLE_ODD_CE && (gubCENumber & BIT0) && (gubPlanesPerBurst > 1)) { // Odd CE
		gubPlanesPerPage -= ulPlanes;
	}
#else
	if (gubILMode == INTERLEAVE_8 ) {//IL8
		if ((gubTotalGroups >= (8 * gubDieNumber))) { //IL4 && unit must large than 2048 for GRTarget
			gulTotalPhyUnitNum /= 8;
			gubTotalGroups /= 8;
			gubPlanesPerPage *= 8;
			gubInterleave = 8;
			//gubBurstsPerPage*=4;
		}
		//if ((gubTotalGroups>=(4*gubDieNumber))&&((gulTotalPhyUnitNum/4)>=2048)) {     //IL4 && unit must large than 2048 for GRTarget
		else if ((gubTotalGroups >= (4 * gubDieNumber))) { //IL4 && unit must large than 2048 for GRTarget
			gulTotalPhyUnitNum /= 4;
			gubTotalGroups /= 4;
			gubPlanesPerPage *= 4;
			gubInterleave = 4;
			//gubBurstsPerPage*=4;
		}
		//else if ((gubTotalGroups>=(2*gubDieNumber))&&((gulTotalPhyUnitNum/2)>=2048)) { //IL2
		else if ((gubTotalGroups >= (2 * gubDieNumber))) { //IL2
			gulTotalPhyUnitNum /= 2;
			gubTotalGroups /= 2;
			gubPlanesPerPage *= 2;
			gubInterleave = 2;
			//gubBurstsPerPage*=2;
		}
		else { //noIL
			//gubPlanesPerPage *=1;
		}
	}
	else if (gubILMode == INTERLEAVE_4 ) {//IL4
		//if ((gubTotalGroups>=(4*gubDieNumber))&&((gulTotalPhyUnitNum/4)>=2048)) {     //IL4 && unit must large than 2048 for GRTarget
		if ((gubTotalGroups >= (4 * gubDieNumber))) { //IL4 && unit must large than 2048 for GRTarget
			gulTotalPhyUnitNum /= 4;
			gubTotalGroups /= 4;
			gubPlanesPerPage *= 4;
			gubInterleave = 4;
			//gubBurstsPerPage*=4;
		}
		//else if ((gubTotalGroups>=(2*gubDieNumber))&&((gulTotalPhyUnitNum/2)>=2048)) { //IL2
		else if ((gubTotalGroups >= (2 * gubDieNumber))) { //IL2
			gulTotalPhyUnitNum /= 2;
			gubTotalGroups /= 2;
			gubPlanesPerPage *= 2;
			gubInterleave = 2;
			//gubBurstsPerPage*=2;
		}
		else { //noIL
			//gubPlanesPerPage *=1;
		}
	}
	else if (gubILMode == INTERLEAVE_2) {//ENABLE_INTERLEAVE2
		if ((gubTotalGroups >= (2 * gubDieNumber))) { //IL2
			gulTotalPhyUnitNum /= 2;
			gubTotalGroups /= 2;
			gubPlanesPerPage *= 2;
			gubInterleave = 2;
		}
		else {
		}
	}
	else {}
#endif

	if (((ENABLE_FORCE_PSEUDO_CE || (guoFlashDefaultType.B.btHynix3DV5 && (!guoFlashUseType.B.btFourPlane))) && guoFlashUseType.B.btMultiDie) ||  (ENABLE_PSEUDO_CE && (((gulTotalPhyUnitNum / 1024) * 1024) > 4096) )) {
		//if(gubExpandCE == 0){
		gubExpandCE_Ratio = (gubDieNumber < (MAX_EXPAND_CE / gubCENumber)) ? (gubDieNumber) : (MAX_EXPAND_CE / gubCENumber) ;
		gubDieNumber /= gubExpandCE_Ratio;
		gubSelDieNumber = gubDieNumber;
		//}
		gubTotalGroups /= gubExpandCE_Ratio;
		gubCENumber *= gubExpandCE_Ratio;
		gulTotalPhyUnitNum /= gubExpandCE_Ratio; //After expand CE, re-calculate Physical Unit number
		gulPhyBlocksPerCE /= gubExpandCE_Ratio; //After expand CE, re-calculate Physical Unit number
		gubPlanesPerPage *= gubExpandCE_Ratio;
		gubInterleave *= gubExpandCE_Ratio;
		if (ENABLE_SANDISK_1ZNM_SPF) {
			gubCENumber -= gubPlanesPerBurst * 1 * gubHideCE; //2Ch 1 = 3Die -2Die HideCE is 2die
			gubPlanesPerPage -= gubPlanesPerBurst * 2 * gubHideCE; // 2CH*2Planes*Hide CE
			gubInterleave -= gubHideCE;// 2CE only 2Die, so total is 3+3+2+2
		}
		if (gubExpandCE_Ratio > 1)	 {
			gubExpandCE = 1;
		}
	}
	else {
		gubExpandCE_Ratio = 1;
	}

	M_AssertError(((gulTotalPhyUnitNum / 1024) * 1024) > 4096);


	if ((gubPlanesPerPage == 64) && (gubExpandCE)) { //PlanesPerPage =64 and FW go through Pseudo CE flow, MTdepth will run out (255) because 2FQs use 4 depths;
		gubMTEVENT_CNT_LIMIT = 8;       //Limit MT Event under 8 in each queue to avoid MT depth run out.
	}
	//due to MP can control multiplane number, so gubBurstsPerBank need to be set again.
	gubBurstsPerBank = ((guoFlashUseType.B.btFourPlane) ? 4 : (guoFlashUseType.B.btTwoPlane) ? 2 : 1);
	gubBurstsPerBankMask = gubBurstsPerBank - 1;
	gubBurstsPerBankLog = flaCheckLogValue((U32)gubBurstsPerBank);

	gubSectorsPerBurst = gubSectorsPerPlane * gubPlanesPerBurst;
	gubSectorsPerBurstMask = gubSectorsPerBurst - 1;
	gubSectorsPerBurstLog = flaCheckLogValue((U32)gubSectorsPerBurst);

	guwSectorsPerPage = gubSectorsPerPlane * gubPlanesPerPage;
	guwSectorsPerPageMask = guwSectorsPerPage - 1;
	gubSectorsPerPageLog = flaCheckLogValue((U32)guwSectorsPerPage);

	gulSectorsPerUnit = (U32)gubSectorsPerPlane * gubPlanesPerPage * guwPagesPerUnit;
	gulSectorsPerUnitMask = gulSectorsPerUnit - 1;
	gubSectorsPerUnitLog = flaCheckLogValue((U32)gulSectorsPerUnit);

#if TLC
	gulFastPageSectorsPerUnit = (U32)gubSectorsPerPlane * gubPlanesPerPage * guwFastPagePagesPerUnit;
#endif

	gub4kEntrysPerBurst = gubSectorsPerBurst / 8;
	gub4kEntrysPerBurstMask = gub4kEntrysPerBurst - 1;
	gub4kEntrysPerBurstLog = flaCheckLogValue((U32)gub4kEntrysPerBurst);

	guw4kEntrysPerPage = guwSectorsPerPage / 8;
	guw4kEntrysPerPageMask = guw4kEntrysPerPage - 1;
	gub4kEntrysPerPageLog = flaCheckLogValue((U32)guw4kEntrysPerPage);

	gul4kEntrysPerUnit = gulSectorsPerUnit / 8;
	gul4kEntrysPerUnitMask = gul4kEntrysPerUnit - 1;
	gub4kEntrysPerUnitLog = flaCheckLogValue((U32)gul4kEntrysPerUnit);

#if MicronFlashOnly

	UWORD uwZipRate;
#if B0KB
	// If GR_1152KB is enabled, zip rate calculation still using 1056KB, due to zip need to compatible with Old FW(download microcode)
	uwZipRate = ( 0x108000 / (gul4kEntrysPerUnit * 4) );
#else
	uwZipRate =  (((U32)gulGRTableTemp - (U32)gulGRTable) / (gul4kEntrysPerUnit * 4) );
#endif

#if B0KB
	if ( (uwZipRate == 1) || (uwZipRate == 0) ) {
		gubMaxZipRate = 1;	// no zip
		gubLZSS_RESOLUTION = 3;	// no zip
	}
	else if ( (uwZipRate == 2) || ( uwZipRate == 3) ) {
		gubMaxZipRate = 2;	// 2K zip
		gubLZSS_RESOLUTION = 2;	// 2K zip
	}
	else {
		gubMaxZipRate = MAX_ZIP_RATE;
		gubLZSS_RESOLUTION = LZSS_RESOLUTION;
	}
#else


	if ((gulSectorsPerUnit >> gubSectorsPerPlaneLog) >= 0x8000) {
		gubMaxZipRate = 1;	// no zip
		gubLZSS_RESOLUTION = 3;	// no zip
	}
	//else if ((gulSectorsPerUnit >> gubSectorsPerPlaneLog) == 0x8000) {
	//	gubMaxZipRate = 2;	// 2K zip
	//	gubLZSS_RESOLUTION = 2;	// 2K zip
	//}
	else {
		gubMaxZipRate = MAX_ZIP_RATE;
		gubLZSS_RESOLUTION = LZSS_RESOLUTION;
	}
#endif

	SYS0B[SYS0B_CR_LZSS_CFG] = gubLZSS_RESOLUTION;
#elif TLC_BICS2 || Hynix3DV6 || Hynix3DV5 || YMTC_JGS
	if ((gulSectorsPerUnit >> gubSectorsPerPlaneLog) >= 0x8000) {
		gubMaxZipRate = 1;	// no zip
		gubLZSS_RESOLUTION = 3;	// no zip
	}
	else if ((gulSectorsPerUnit >> gubSectorsPerPlaneLog) > 0x4000) { // 16-CE, 2-Plane
		gubMaxZipRate = 2;	// 2K zip
		gubLZSS_RESOLUTION = 2;	// 2K zip
	}
	else {
		gubMaxZipRate = MAX_ZIP_RATE;
		gubLZSS_RESOLUTION = LZSS_RESOLUTION;
	}
	SYS0B[SYS0B_CR_LZSS_CFG] = gubLZSS_RESOLUTION;

#elif (ENABLE_PFAIL && (ENABLE_ULTRAMLC_PFAIL_MEASURE || (BUFFER2_WRITE_WINDOW_4KNUM==4)))
	gubMaxZipRate = 1;	/* for pfail, no zip to test worst case. */
	gubLZSS_RESOLUTION = 3;	// no zip
	SYS0B[SYS0B_CR_LZSS_CFG] = gubLZSS_RESOLUTION;
#else
	gubMaxZipRate = MAX_ZIP_RATE;
	gubLZSS_RESOLUTION = LZSS_RESOLUTION;
#endif

	gulMaxL4kEntrysPerUnit = gul4kEntrysPerUnit * gubMaxZipRate;
	gulMaxL4kEntrysPerUnitMask = gulMaxL4kEntrysPerUnit - 1;
	gubMaxL4kEntrysPerUnitLog = flaCheckLogValue((U32)gulMaxL4kEntrysPerUnit);


	gubPlanesPerBurstMask = gubPlanesPerBurst - 1;
	gubPlanesPerBurstLog = flaCheckLogValue((U32)gubPlanesPerBurst);



	gubPlanesPerBank = gubPlanesPerBurst << gubBurstsPerBankLog;
	gubPlanesPerBankMask = gubPlanesPerBank - 1;
	gubPlanesPerBankLog = flaCheckLogValue((U32)gubPlanesPerBank);


	gubPlanesPerPageMask = gubPlanesPerPage - 1;
	gubPlanesPerPageLog = flaCheckLogValue((U32)gubPlanesPerPage);

	gulPlanesPerUnit = gulSectorsPerUnit >> gubSectorsPerPlaneLog;
	gulPlanesPerUnitMask = gulPlanesPerUnit - 1;
	gubPlanesPerUnitLog = flaCheckLogValue((U32)gulPlanesPerUnit);


	gubBurstsPerPage = gubPlanesPerPage >> gubPlanesPerBurstLog;
	gubBurstsPerPageMask = gubBurstsPerPage - 1;
	gubBurstsPerPageLog = flaCheckLogValue((U32)gubBurstsPerPage);


	guwBurstsPerUnit = (UWORD)gubBurstsPerPage * guwPagesPerUnit;
	guwBurstsPerUnitMask = guwBurstsPerUnit - 1;
	gubBurstsPerUnitLog = flaCheckLogValue((U32)guwBurstsPerUnit);

	guwFastPagePlanesPerUnit = (UWORD)gubPlanesPerPage * guwFastPagePagesPerUnit;
	gubFastPagePlanesPerUnitLog = flaCheckLogValue((UWORD)guwFastPagePlanesPerUnit);
#if TLC_BICS2
#if TSB_BICS4_SUPPORT
	if (guoFlashUseType.B.btToshibaBiCs2 || guoFlashUseType.B.btToshibaBiCs4) {
#else
	if (guoFlashUseType.B.btToshibaBiCs2) {
#endif
		gubFastPagePlanesPerUnitLog += 1; // guwFastPagePlanesPerUnit is not power of 2
	}
#endif /* TLC_BICS2 */

	guwTableP2LBitMapSize = ((UWORD)SV_ALIGNED_32(guwFastPagePlanesPerUnit / 8) * 8); //Alignment 32 Byte For Hard Ware memory Set

	gulFastPage4kEntrysPerUnit = guwFastPagePlanesPerUnit * gub4kEntrysPerPlane;
	gulFastPage4kEntrysPerUnitMask = (gulFastPage4kEntrysPerUnit - 1);
	gubFastPage4kEntrysPerUnitLog = flaCheckLogValue((UWORD)gulFastPage4kEntrysPerUnit);

	guwFastPageBurstsPerUnit = (UWORD)gubBurstsPerPage * guwFastPagePagesPerUnit;
	guwFastPageBurstsPerUnitMask = guwFastPageBurstsPerUnit - 1;

	gubBanksPerPage = gubBurstsPerPage >> gubBurstsPerBankLog;
	if (ENABLE_ODD_CE && (gubPlanesPerBurst > 1)) { // Odd CE, do not support CE-Groups yet
		gubBanksPerPage += gubCENumber & BIT0;
	}
	gubBanksPerPageMask = gubBanksPerPage - 1;
	gubBanksPerPageLog = flaCheckLogValue((U32)gubBanksPerPage);

	guwPagesPerUnitMask = guwPagesPerUnit - 1;
	gubPagesPerUnitLog = flaCheckLogValue((U32)guwPagesPerUnit);

	gubCEsPerPage = gubPlanesPerBurst * (gubBanksPerPage);
	if (ENABLE_ODD_CE && (gubPlanesPerBurst > 1)) { // Odd CE, do not support CE-Groups yet
		gubCEsPerPage -= (gubCENumber & BIT0);
	}
	gubCEsPerPageMask = gubCEsPerPage - 1;
	gubCEsPerPageLog = flaCheckLogValue((U32)gubCEsPerPage);

	gubTotalGroupsMask = gubTotalGroups - 1;
	gubTotalGroupsLog = flaCheckLogValue((U32)gubTotalGroups);
	gubSelTotalGroups = gubTotalGroups;
	gubSelTotalGroupsMask = gubTotalGroupsMask;
	gubSelTotalGroupsLog = gubTotalGroupsLog;

	gubDieNumberMask = gubDieNumber - 1;
	gubDieNumberLog = flaCheckLogValue((U32)gubDieNumber);

	gubParaInitialized = 1;
	gubExpandCE_initial = 1;
	gubMTQTotalPhyCE = (gubCENumber / gubExpandCE_Ratio);
	if (ENABLE_SANDISK_1ZNM_SPF) {
		gubMTQTotalPhyCE = ((gubCENumber + gubHideCE * gubPlanesPerBurst) / gubExpandCE_Ratio);
	}
#if FLUSHD1_PARTIAL_CE_ADVANCE
	gubPartialDoingCENum = FLUSHD1_PARTIAL_CENUM;
#endif
}
void flaToshibaIDCheck(void)
{
	guoFlashDefaultType.B.btCacheProgram = TRUE;
	guoFlashDefaultType.B.btCacheRead = TRUE;
	switch (gubFlashID[1]) {
		case 0xA0 :
		case 0xF0 :      //64
		case 0xA1 :
		case 0xF1 :      //128
		case 0xBA :      //
		case 0xCA :      // SanDisk 256MB
		case 0xAA :
		case 0xDA :      //256
		case 0xBC :      // SanDisk 512MB
		case 0xCC :      // SanDisk 512MB
		case 0xAC :
		case 0xDC :      //512
		case 0xA3 :
		case 0xB3 :
		case 0xC3 :
		case 0xD3 :      //1G
		case 0xA5 :
		case 0xB5 :
		case 0xC5 :
		case 0xD5 :      //2G
		case 0xA7 :
		case 0xB7 :
		case 0xC7 :
		case 0xD7 :      //4G
		case 0xAE :
		case 0xBE :
		case 0xCE :
		case 0xDE : //8G byte
		case 0xE7:      // 4G
		case 0xEE:      // 8G
		case 0x3A:      // 16G
		case 0x4C:      // 16G SanDisk  //GARY
		case 0x3C:      // 32G
		case 0x3E:      // 64G
		case 0x48:		// 128G
		case 0x58:      // 48G
			if (gubFlashID[2] & (BIT1 | BIT0)) { //Check MultiDie for Toshiba
				guoFlashDefaultType.B.btMultiDie = TRUE;//die number
				if (gub4KSLC) {
					//gulFlashDefaultType.B.btMultiDie = FALSE; // test CH
				}
				if ((gubFlashID[2]&BIT1) && (gubFlashID[2]&BIT0)) {
					gubDieNumber = 8;
				}
				else if ((gubFlashID[2]&BIT1) && (!(gubFlashID[2]&BIT0))) {
					gubDieNumber = 4;
				}
				else if ((!(gubFlashID[2]&BIT1)) && (gubFlashID[2]&BIT0)) {
					gubDieNumber = 2;
				}
				if (ENABLE_SANDISK_1ZNM_SPF) {
					gubDieNumber = 3;
				}
				gubSelDieNumber = gubDieNumber;
			}

			if (gubFlashID[4] & (BIT3 | BIT2)) { //Check Two Plane
				UBYTE ubPlanesPerCE = BIT0 << ((gubFlashID[4] & (BIT3 | BIT2)) >> 2);
				UBYTE ubPlanesPerDie = ubPlanesPerCE / gubDieNumber;
				if (ENABLE_SANDISK_1ZNM_SPF) {
					ubPlanesPerDie = 2;
				}
				if ((gubFlashID[0] == 0x98) && (gubFlashID[1] == 0x3E) && (gubFlashID[2] == 0xA6) && (gubFlashID[3] == 0x93) && (gubFlashID[4] == 0x7E) && ( (gubFlashID[5] & (BIT2 | BIT1 | BIT0)) == BIT0)) {
					//The 5th FlashID(gubFlashID[4] = 0x7E) is not right, this Flash is total 16 planes per CE.(4 planes per die, and four die)
					ubPlanesPerDie = 4;
				}
				if (ubPlanesPerDie == 4) {
					guoFlashDefaultType.B.btFourPlane = TRUE;
					guoFlashDefaultType.B.btTwoPlane = TRUE;
					guoFlashDefaultType.B.btTwoPlaneRead = TRUE;
				}
				else if (ubPlanesPerDie == 2) {
					guoFlashDefaultType.B.btTwoPlane = TRUE;
					guoFlashDefaultType.B.btTwoPlaneRead = TRUE;
				}
			}
			// Toshiba_43nm_4LC, Toshiba_56nm_8LC
			//if ( (gubFlashID[5]&0x07)>=0x04) {   //need to confirm
#if !NotNessary
			if (gubFlashID[5] & BIT5) { //3D NAND
				UBYTE ubGeneration = (gubFlashID[5] & (BIT0 | BIT1));
				if (ubGeneration == 0x01) { // BiCs2
					guoFlashDefaultType.B.btToshibaBiCs2 = 1;
					guoFlashDefaultType.B.bt576Page = 1;
				}
				else if (ubGeneration == 0x02) { // BiCs3
					guoFlashDefaultType.B.btToshibaBiCs3 = 1;
					guoFlashDefaultType.B.bt768Page = 1;
				}
#if TSB_BICS4_SUPPORT
				else if (ubGeneration == 0x03) {	// BiCs4
					guoFlashDefaultType.B.btToshibaBiCs4 = 1;
					gubWriteProtect_NUM = WriteProtect_NUM_Bics4_256Gb;
				}
#endif
				else {//以後再補
				}
				gubMIN_D1_UNIT_NUM_BiCS3 = MIN_D1_UNIT_NUM;
			}
			else if ((gubFlashID[5] & 0x07) == FLASH43NM) {
				guoFlashDefaultType.B.btToshiba43nm = 1;
			}
			else if ( (gubFlashID[5] & 0x07) == FLASH32NM) {
				guoFlashDefaultType.B.btToshiba32nm = 1;
				//FL_PollingTimeCTL_All = 0x47474747;
			}
			else if ( (gubFlashID[5] & 0x07) == FLASH24NM) {
				if ((gubFlashID[3] & 0xF0) == 0x80) {   // 8G
					guoFlashDefaultType.B.bt256Page = TRUE;
					if ((gubFlashID[2] & 0x30) == 0x20) { //4
						guoFlashDefaultType.B.btToshiba24nmABL = TRUE;
					}
				}
				else if ((gubFlashID[3] & 0xF0) == 0x30) {
					guoFlashDefaultType.B.btToshiba24nmABL = TRUE;
				}

				guoFlashDefaultType.B.btToshiba24nm = TRUE;

				if (guoFlashDefaultType.B.btSLC) {
					guoFlashDefaultType.B.btRandCacheRead = TRUE;
				}
				//FL_PollingTimeCTL_All = 0x47474747;
			}
			else if ( (gubFlashID[5] & 0x07) == FLASH19NM) {
				guoFlashDefaultType.B.bt256Page = TRUE;
				guoFlashDefaultType.B.btToshiba19nm = TRUE;
				guoFlashDefaultType.B.btRandCacheRead = TRUE;
			}
			else if ( (gubFlashID[5] & 0x07) == FLASH1YNM) {
				if (guoFlashDefaultType.B.bt4LC) {
					guoFlashDefaultType.B.bt256Page = TRUE;
				}
				else {
					guoFlashDefaultType.B.bt258Page = TRUE;
				}
				guoFlashDefaultType.B.btToshiba1Ynm = TRUE;
				guoFlashDefaultType.B.btRandCacheRead = TRUE;
			}
			else if ( (gubFlashID[5] & 0x07) == FLASH1ZNM) {
				if (guoFlashDefaultType.B.bt4LC) {
					guoFlashDefaultType.B.bt256Page = TRUE;
				}
				else {
					guoFlashDefaultType.B.bt384Page = TRUE;
				}
				guoFlashDefaultType.B.btToshiba1Znm = TRUE;
				guoFlashDefaultType.B.btRandCacheRead = TRUE;
			}
#else
			if ( (gubFlashID[5] & 0x07) == FLASH1ZNM) {
				if (guoFlashDefaultType.B.bt4LC) {
					guoFlashDefaultType.B.bt256Page = TRUE;
				}
				else {
					guoFlashDefaultType.B.bt384Page = TRUE;
				}
				guoFlashDefaultType.B.btToshiba1Znm = TRUE;
				guoFlashDefaultType.B.btRandCacheRead = TRUE;
			}
			else if (gubFlashID[5] & BIT5) {
				guoFlashDefaultType.B.btNoSupport = TRUE;

			}
#endif
			// check toggle
			if ((gubFlashID[5] & 0x80)) {
				guoFlashDefaultType.B.btToggle = TRUE;
			}
			if (gub4KSLC == 0 ) {
				if ((gubFlashID[3] & 0x03) == 0x00) guoFlashDefaultType.B.btFlash2k = TRUE;
				else if ((gubFlashID[3] & 0x03) == 0x01) guoFlashDefaultType.B.btFlash4k = TRUE;
				else if ((gubFlashID[3] & 0x03) == 0x02)guoFlashDefaultType.B.btFlash8k = TRUE;
				else if ((gubFlashID[3] & 0x03) == 0x03)guoFlashDefaultType.B.btFlash16k = TRUE;

			}
			else {
				if ((gubFlashID[3] & 0x03) == 0x01) guoFlashDefaultType.B.btFlash2k = TRUE;
				else if ((gubFlashID[3] & 0x03) == 0x02) guoFlashDefaultType.B.btFlash4k = TRUE;
				else if ((gubFlashID[3] & 0x03) == 0x03) guoFlashDefaultType.B.btFlash8k = TRUE;
			}


			if (gubFlashID[4] & 0x02) {
				guoFlashDefaultType.B.btExtendBlock = TRUE;
			}

			if (gubFlashID[6]&BIT3) {
				if (!guoFlashDefaultType.B.btFlash8k) {
					guoFlashDefaultType.B.btABL = TRUE;
				}
			}
			if ((!guoFlashDefaultType.B.btSandisk) && (gubFlashID[1] == 0x58)) {
				guoFlashDefaultType.B.btNoSupport = TRUE;
				SetInitialFailID(BIT10, 0);
			}
			break;
		default:
			guoFlashDefaultType.B.btNoSupport = TRUE;
			SetInitialFailID(BIT10, 0);
			break;
	}
}
//void flaSamsungIDCheck(void) {}

void flaIntelMicronIDCheck(void)
{
#if B0KB
	guoFlashDefaultType.B.btFlash16k = TRUE;
	guoFlashDefaultType.B.btIntelMicronB0KB  = TRUE;
	guoFlashDefaultType.B.bt8LC = 1;
	// Normal     2C B4 78 32 AA 04
	// Enterprise 2C B4 78 32 AA 05
	if (gubFlashID[5] == 0x05) {
		guoFlashDefaultType.B.btEnterprise = TRUE;
	}
#else
	guoFlashDefaultType.B.btFlash16k = TRUE;
	guoFlashDefaultType.B.bt1024Page = TRUE;
	guoFlashDefaultType.B.btIntelMicronL06B  = TRUE;
#endif

	guoFlashDefaultType.B.btFourPlane = TRUE;
	guoFlashDefaultType.B.btExtendBlock = TRUE;

	guoFlashDefaultType.B.btCacheProgram = TRUE;
	guoFlashDefaultType.B.btCacheRead = TRUE;
	guoFlashDefaultType.B.btRandCacheRead = TRUE;

#if ENABLE_MICRON_SLC
	guoFlashDefaultType.B.btA2Cmd = TRUE;
#else
	guoFlashDefaultType.B.btA2Cmd = FALSE;
#endif

	// need to discuss to read parameterpage first here.
	//flaReadParameterPage();

}

void flaYMTCIDCheck(void)
{
#if YMTC
	guoFlashDefaultType.B.btFlash16k = TRUE;
	guoFlashDefaultType.B.btYMTC  = TRUE;
#if YMTC_JGS
	guoFlashDefaultType.B.btYMTCJGS  = TRUE;
#endif
	guoFlashDefaultType.B.btTwoPlane = TRUE;
	guoFlashDefaultType.B.btExtendBlock = TRUE;
	guoFlashDefaultType.B.btCacheProgram = TRUE;
	guoFlashDefaultType.B.btCacheRead = TRUE;
	guoFlashDefaultType.B.btRandCacheRead = TRUE;
	guoFlashDefaultType.B.btA2Cmd = TRUE;
#endif
}


#if (Hynix||((!(MicronFlashOnly || TLC || Hynix || Sandisk))&&BURNER))
void flaHynixIDCheck()
{
	UBYTE ubDat;

	guoFlashDefaultType.B.btA2Cmd = ENABLE;

	ubDat = (gubFlashID[2] & (BIT6 | BIT1 | BIT0));
	guoFlashDefaultType.B.btMultiDie = (ubDat) ? ENABLE : DISABLE;
	switch (ubDat) {
		case BIT0:
			gubDieNumber = 2;
			break;
		case BIT1:
			gubDieNumber = 4;
			break;
		case (BIT1|BIT0):
			gubDieNumber = 8;
			break;
		case BIT6:
			gubDieNumber = 16;
			break;
		case 0:
		default:
			gubDieNumber = 1;
			break;
	}
	gubSelDieNumber = gubDieNumber;

	ubDat = (gubFlashID[2] & (BIT3 | BIT2));
	guoFlashDefaultType.B.btSLC = DISABLE;
	guoFlashDefaultType.B.bt4LC = DISABLE;
	guoFlashDefaultType.B.bt8LC = DISABLE;
	switch (ubDat) {
		case BIT2:
			guoFlashDefaultType.B.bt4LC = ENABLE;
			break;
		case BIT3:
			guoFlashDefaultType.B.bt8LC = ENABLE;
			break;
		case (BIT3|BIT2):
			//16LC
			break;
		case 0:
		default:
			guoFlashDefaultType.B.btSLC = ENABLE;
			break;
	}

	ubDat = (gubFlashID[2] & (BIT5 | BIT4));
	guoFlashDefaultType.B.btTwoPlane = DISABLE;
	guoFlashDefaultType.B.btTwoPlaneRead = DISABLE;
	guoFlashDefaultType.B.btFourPlane = DISABLE;
	switch (ubDat) {
		case BIT4:
			guoFlashDefaultType.B.btTwoPlane = ENABLE;
			guoFlashDefaultType.B.btTwoPlaneRead = ENABLE;
			break;
		case BIT5:
			guoFlashDefaultType.B.btTwoPlane = ENABLE;
			guoFlashDefaultType.B.btFourPlane = ENABLE;
			guoFlashDefaultType.B.btTwoPlaneRead = ENABLE;
			break;
		case (BIT5|BIT4):
			//8 plane
			break;
		case 0:
		default:
			break;
	}


	/*if you want to know Generation info, parse 5th ID*/

	ubDat = (gubFlashID[5] & (BIT7 | BIT6 | BIT5 | BIT4));

	switch (ubDat) {
#if (!Hynix)
		case (BIT5)://16nm
			guoFlashDefaultType.B.btHynix16nm  = TRUE;
			break;
		case (BIT6|BIT4)://1ynm
			guoFlashDefaultType.B.btHynix14nm  = TRUE;
			break;
#else
#if (Hynix3DV6 || Hynix3DV5)
		case (BIT5|BIT7):
			guoFlashDefaultType.B.btHynix3DV5 = TRUE;
			guoFlashDefaultType.B.btFourPlane = DISABLE; // Hynix V5 Set 2 Plane
		case (BIT4|BIT5|BIT7):
			if (guoFlashDefaultType.B.btHynix3DV5) {
				//Nothing
			}
			else {
				guoFlashDefaultType.B.btHynix3DV6 = TRUE;
			}
			//guoFlashDefaultType.B.btNoSupport = TRUE;
			guoFlashDefaultType.B.btFlash16k = TRUE;
			guoFlashDefaultType.B.btExtendBlock   = TRUE;
			guoFlashDefaultType.B.btCacheRead = TRUE;
			guoFlashDefaultType.B.btRandCacheRead = TRUE;
			guoFlashDefaultType.B.btCacheProgram  = TRUE;

			gsHynixRetryInfo.ubXlcRegAdr[0] = 0x29;
			gsHynixRetryInfo.ubXlcRegAdr[1] = 0x2A;
			gsHynixRetryInfo.ubXlcRegAdr[2] = 0x2B;
			gsHynixRetryInfo.ubXlcRegAdr[3] = 0x2C;
			gsHynixRetryInfo.ubXlcRegAdr[4] = 0x2D;
			gsHynixRetryInfo.ubXlcRegAdr[5] = 0x2E;
			gsHynixRetryInfo.ubXlcRegAdr[6] = 0x2F;
			gsHynixRetryInfo.ubXlcRegAdr[7] = 0x29;

			gsHynixRetryInfo.ubSlcRegAdr[0] = 0x25;

#if BURNER   // TODO: support burner scan OPT table, need to provide an available buffer (not in dcode)
			gsHynixRetryInfo.ulSlcRetryTblBase = 0;
			gsHynixRetryInfo.ulXlcRetryTblBase = 0;
#else //FW use Default RR Info first
			gsHynixRetryInfo.ubSlcRRstepCnt = 27;
			gsHynixRetryInfo.ubSlcRRregCnt	= 1;
			gsHynixRetryInfo.ubXlcRRstepCnt = 27;
			gsHynixRetryInfo.ubXlcRRregCnt	= 8;
			gsHynixRetryInfo.uwD1RetryTblSizePerDie = 27;
			gsHynixRetryInfo.uwD3RetryTblSizePerDie = 216;
			gsHynixRetryInfo.ulSlcRetryTblBase = (U32) &gubSlcRetryTable;
#if 0
			gubSlcRetryTable[0] = 0x00;
			gubSlcRetryTable[1] = 0x02;
			gubSlcRetryTable[2] = 0x04;
			gubSlcRetryTable[3] = 0x06;
			gubSlcRetryTable[4] = 0x08;
			gubSlcRetryTable[5] = 0x0A;
			gubSlcRetryTable[6] = 0x0C;
			gubSlcRetryTable[7] = 0x0E;
			gubSlcRetryTable[8] = 0x10;
			gubSlcRetryTable[9] = 0x12;
			gubSlcRetryTable[10] = 0x14;
			gubSlcRetryTable[11] = 0x16;
			gubSlcRetryTable[12] = 0x18;
			gubSlcRetryTable[13] = 0x1A;
			gubSlcRetryTable[14] = 0x1C;
			gubSlcRetryTable[15] = 0x1E;
			gubSlcRetryTable[16] = 0x20;
			gubSlcRetryTable[17] = 0x22;
			gubSlcRetryTable[18] = 0xFE;
			gubSlcRetryTable[19] = 0xFC;
			gubSlcRetryTable[20] = 0xFA;
			gubSlcRetryTable[21] = 0xF8;
			gubSlcRetryTable[22] = 0xF6;
			gubSlcRetryTable[23] = 0xF4;
			gubSlcRetryTable[24] = 0xF2;
			gubSlcRetryTable[25] = 0xF0;
			gubSlcRetryTable[26] = 0xEF;
#endif
			gsHynixRetryInfo.ulXlcRetryTblBase = (U32) &gubXlcRetryTable;
#endif


			break;
#elif Hynix_16nm
		case (BIT5)://16nm
			guoFlashDefaultType.B.btHynix16nm  = TRUE;
			if (guoFlashDefaultType.B.bt8LC) {
				guoFlashDefaultType.B.bt258Page  = TRUE;
				guoFlashDefaultType.B.btFlash16k = TRUE;
				guoFlashDefaultType.B.btExtendBlock   = TRUE;
				guoFlashDefaultType.B.btCacheRead = TRUE;
				guoFlashDefaultType.B.btRandCacheRead = TRUE;
				guoFlashDefaultType.B.btCacheProgram  = TRUE;
#if 1 //Retry Reg Adr and Table Base  Init
				gsHynixRetryInfo.ubXlcRegAdr[0] = 0x39;
				gsHynixRetryInfo.ubXlcRegAdr[1] = 0x3A;
				gsHynixRetryInfo.ubXlcRegAdr[2] = 0x3B;
				gsHynixRetryInfo.ubXlcRegAdr[3] = 0x3C;
				gsHynixRetryInfo.ubXlcRegAdr[4] = 0x3D;
				gsHynixRetryInfo.ubXlcRegAdr[5] = 0x3E;
				gsHynixRetryInfo.ubXlcRegAdr[6] = 0x3F;
				gsHynixRetryInfo.ubXlcRegAdr[7] = 0x29;

				gsHynixRetryInfo.ubSlcRegAdr[0] = 0x28;

#if BURNER   // TODO: support burner scan OPT table, need to provide an available buffer (not in dcode)
				gsHynixRetryInfo.ulSlcRetryTblBase = 0;
				gsHynixRetryInfo.ulXlcRetryTblBase = 0;
#else //FW use Default RR Info first
				gsHynixRetryInfo.ubSlcRRstepCnt = 27;
				gsHynixRetryInfo.ubSlcRRregCnt  = 1;
				gsHynixRetryInfo.ubXlcRRstepCnt = 27;
				gsHynixRetryInfo.ubXlcRRregCnt  = 8;
				gsHynixRetryInfo.uwD1RetryTblSizePerDie = 27;
				gsHynixRetryInfo.uwD3RetryTblSizePerDie = 216;
				gsHynixRetryInfo.ulSlcRetryTblBase = (U32) &gubSlcRetryTable;

				gubSlcRetryTable[0] = 0x00;
				gubSlcRetryTable[1] = 0x02;
				gubSlcRetryTable[2] = 0x04;
				gubSlcRetryTable[3] = 0x06;
				gubSlcRetryTable[4] = 0x08;
				gubSlcRetryTable[5] = 0x0A;
				gubSlcRetryTable[6] = 0x0C;
				gubSlcRetryTable[7] = 0x0E;
				gubSlcRetryTable[8] = 0x10;
				gubSlcRetryTable[9] = 0x12;
				gubSlcRetryTable[10] = 0x14;
				gubSlcRetryTable[11] = 0x16;
				gubSlcRetryTable[12] = 0x18;
				gubSlcRetryTable[13] = 0x1A;
				gubSlcRetryTable[14] = 0x1C;
				gubSlcRetryTable[15] = 0x1E;
				gubSlcRetryTable[16] = 0x20;
				gubSlcRetryTable[17] = 0x22;
				gubSlcRetryTable[18] = 0xFE;
				gubSlcRetryTable[19] = 0xFC;
				gubSlcRetryTable[20] = 0xFA;
				gubSlcRetryTable[21] = 0xF8;
				gubSlcRetryTable[22] = 0xF6;
				gubSlcRetryTable[23] = 0xF4;
				gubSlcRetryTable[24] = 0xF2;
				gubSlcRetryTable[25] = 0xF0;
				gubSlcRetryTable[26] = 0xEF;

				gsHynixRetryInfo.ulXlcRetryTblBase = (U32) &gubXlcRetryTable;
#endif
#endif
			}
			else {
				guoFlashDefaultType.B.btNoSupport = TRUE;
			}
			break;
#else
		case (BIT6|BIT4)://1ynm
			guoFlashDefaultType.B.btHynix14nm  = TRUE;
			if (guoFlashDefaultType.B.bt8LC) {
				guoFlashDefaultType.B.bt384Page  = TRUE;
				guoFlashDefaultType.B.btFlash16k = TRUE;
				guoFlashDefaultType.B.btExtendBlock   = TRUE;
				guoFlashDefaultType.B.btCacheRead = TRUE;
				guoFlashDefaultType.B.btRandCacheRead = TRUE;
				guoFlashDefaultType.B.btCacheProgram  = TRUE;
#if 1 //Retry Reg Adr and Table Base  Init
				gsHynixRetryInfo.ubXlcRegAdr[0] = 0xB0;
				gsHynixRetryInfo.ubXlcRegAdr[1] = 0xB1;
				gsHynixRetryInfo.ubXlcRegAdr[2] = 0xB2;
				gsHynixRetryInfo.ubXlcRegAdr[3] = 0xB3;
				gsHynixRetryInfo.ubXlcRegAdr[4] = 0xB4;
				gsHynixRetryInfo.ubXlcRegAdr[5] = 0xB5;
				gsHynixRetryInfo.ubXlcRegAdr[6] = 0xB6;
				gsHynixRetryInfo.ubXlcRegAdr[7] = 0x4F;

				gsHynixRetryInfo.ubSlcRegAdr[0] = 0x4E;

#if BURNER   // TODO: support burner scan OPT table, need to provide an available buffer (not in dcode)
				gsHynixRetryInfo.ulSlcRetryTblBase = 0;
				gsHynixRetryInfo.ulXlcRetryTblBase = 0;
#else //FW use Default RR Info first
				gsHynixRetryInfo.ubSlcRRstepCnt = 32;
				gsHynixRetryInfo.ubSlcRRregCnt  = 1;
				gsHynixRetryInfo.ubXlcRRstepCnt = 32;
				gsHynixRetryInfo.ubXlcRRregCnt  = 8;
				gsHynixRetryInfo.uwD1RetryTblSizePerDie = 32;
				gsHynixRetryInfo.uwD3RetryTblSizePerDie = 256;
				gsHynixRetryInfo.ulSlcRetryTblBase = (U32) &gubSlcRetryTable;

				gubSlcRetryTable[0] = 0x00;
				gubSlcRetryTable[1] = 0x02;
				gubSlcRetryTable[2] = 0x04;
				gubSlcRetryTable[3] = 0x06;
				gubSlcRetryTable[4] = 0x08;
				gubSlcRetryTable[5] = 0x0A;
				gubSlcRetryTable[6] = 0x0C;
				gubSlcRetryTable[7] = 0x0E;
				gubSlcRetryTable[8] = 0x10;
				gubSlcRetryTable[9] = 0x12;
				gubSlcRetryTable[10] = 0x14;
				gubSlcRetryTable[11] = 0x16;
				gubSlcRetryTable[12] = 0x18;
				gubSlcRetryTable[13] = 0x1A;
				gubSlcRetryTable[14] = 0x1C;
				gubSlcRetryTable[15] = 0x1E;
				gubSlcRetryTable[16] = 0x20;
				gubSlcRetryTable[17] = 0x22;
				gubSlcRetryTable[18] = 0x05;
				gubSlcRetryTable[19] = 0x03;
				gubSlcRetryTable[20] = 0xFE;
				gubSlcRetryTable[21] = 0xFD;
				gubSlcRetryTable[22] = 0xFC;
				gubSlcRetryTable[23] = 0xFB;
				gubSlcRetryTable[24] = 0xFA;
				gubSlcRetryTable[25] = 0xF9;
				gubSlcRetryTable[26] = 0xF8;
				gubSlcRetryTable[27] = 0xF6;
				gubSlcRetryTable[28] = 0xF4;
				gubSlcRetryTable[29] = 0xF2;
				gubSlcRetryTable[30] = 0xF0;
				gubSlcRetryTable[31] = 0xEF;
				gubSlcRetryTable[16] = 0x20;

				gsHynixRetryInfo.ulXlcRetryTblBase = (U32) &gubXlcRetryTable;
#endif
#endif
			}
			else {
				guoFlashDefaultType.B.btNoSupport = TRUE;
			}
			break;
#endif
		case BIT4://2xnm
		case (BIT5|BIT4)://2ynm
		case BIT6://1xnm
		case (BIT6|BIT5)://3D-V1
		case (BIT6|BIT5|BIT4)://3D-V2
		case BIT7://3D-V3
		case 0://3ynm
#endif
		default://Only focus hynix 1ynm now
			guoFlashDefaultType.B.btNoSupport = TRUE;
			break;
	}

	ubDat = (gubFlashID[5] & (BIT1 | BIT0));
	// debug, eric, force lagacy
	//gubFlashID[5] &= ~0x03;
	//gubFlashID[5] |= BIT0;
	// -------------------------
	switch (ubDat) {
		case BIT0:// SDR only
			guoFlashDefaultType.B.btToggle = FALSE;
			break;// DDR only
		case BIT1:
			guoFlashDefaultType.B.btToggle = TRUE;
			break;
		case 0://SDR and DDR
		default:
			guoFlashDefaultType.B.btToggle = FALSE;
			break;
	}
#if Hynix3DV6
#elif Hynix
	// init erase command
	garEraseFPU[0][0] = FPU_PTR_AG_C60_A3_CD0;
	garEraseFPU[0][1] = FPU_PTR_AG_CA2_C60_A3_CD0;
	garEraseFPU[0][2] = FPU_PTR_AG_C60_A3_CD0;
	garEraseFPU[1][0] = FPU_PTR_AG_C60_A3_CD1_AG_C60_A3_CD0;
	garEraseFPU[1][1] = FPU_PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0;
	garEraseFPU[1][2] = FPU_PTR_AG_C60_A3_CD1_AG_C60_A3_CD0;
#endif
}
#endif




void flaInfoBlockSetting(SYSTEM_GROUP_t *InfoArray)
{

	if ((InfoArray->u8bPhIsOnInfoMark[0] == 'P') && (InfoArray->u8bPhIsOnInfoMark[1] == 'h') && (InfoArray->u8bPhIsOnInfoMark[2] == 'I')
	        && (InfoArray->u8bPhIsOnInfoMark[3] == 's') && (InfoArray->u8bPhIsOnInfoMark[4] == 'O') && (InfoArray->u8bPhIsOnInfoMark[5] == 'n')) {

		memcpy(ModelTable, (void*)InfoArray->ubModelNumber, sizeof(ModelTable));
		memcpy(SerialTable, (void*)InfoArray->ubSerialNumber, sizeof(SerialTable));

		//#if (!BURNER)
		// Serial Num Customer ID Byte270~289
#if(!BURNER)
		memcpy(TSBSerialTable, (void*)InfoArray->ubTSB_SerialNumber_Customer, sizeof(TSBSerialTable));
		//#endif
#endif
		if (InfoArray->ubFWVersionEnable == 0x33) {
			memcpy(VerTable, (void*)InfoArray->ubFWVersion, sizeof(VerTable));
		}

		if (InfoArray->ubILMode == 0) { //default
			gubILMode = INTERLEAVE_DEFAULT;
		}
		else {
			gubILMode = InfoArray->ubILMode;
		}

		if (InfoArray->ubUltraMLCEnable == 0) { //default:0 enable:1 disable:2
			gubUltraMLCEnable = ENABLE_ULTRAMLC;
		}
		else if (InfoArray->ubUltraMLCEnable == 1) {
			gubUltraMLCEnable = 1;
		}
		else {
			gubUltraMLCEnable = 0;
		}

		//gulDiskSizeInMB = InfoArray->ulDiskSizeInMB;
		if (InfoArray->ulSSDSize_LBA) {
			gulNativeSize = InfoArray->ulSSDSize_LBA;
		}
		if (!BURNER) {
			if (ENABLE_FORCE_SET_FLH_MODE) {
				gubFlashMode = FORCE_FLH_MODE;
			}
			else {
				if (InfoArray->ubFlashInterface == 0) {
					gubFlashMode = FLH_MODE_DEFAULT;
				}
				else {
					gubFlashMode = InfoArray->ubFlashInterface;
				}
			}

			if (ENABLE_FORCE_SET_FLH_CLK) {
				gubFlashClock = FORCE_FLH_CLK;
			}
			else {
				if (InfoArray->ubFlashClk == 0) {
					gubFlashClock = FLH_CLK_DEFAULT;
				}
				else {
					gubFlashClock = InfoArray->ubFlashClk;
				}
			}
		}

		//SMART Customer
		gubCustomer = InfoArray->ubCustomer;
		gubCustomersCustomerID = InfoArray->ubCustomersCustomerID;

		if (!BURNER) {
			// DEVSLEEP support
			if (InfoArray->ubDEVSLPSupport == 0) { //default:0 enable:1 disable:2
				gubDEVSLPSupport = DEVSLP_Support;
			}
			else if (InfoArray->ubDEVSLPSupport == 1) {
				gubDEVSLPSupport = 1;
			}
			else {
				gubDEVSLPSupport = 0;
			}

#if TLC_TSB
			gubDEVSLPSupport = 1;
#endif

			//World Wide Name (WWN)
			/*	if (InfoArray->ubWWNSupport == 0) { //default:0 enable:1 disable:2
					gubWWNSupport = WWN_Support;
				}
				else if (InfoArray->ubWWNSupport == 1) {
					gubWWNSupport = 1;
				}
				else {
					gubWWNSupport = 0;
				}*/

			memcpy(guwWWN, (void*)InfoArray->uwWWN, sizeof(guwWWN));
			UBYTE ubi = 0;
			gubWWNSupport = 0;
			for (; ubi < 4; ubi++) { // Byte 90 to 97 有值才要開這個flag
				if (guwWWN[ubi] != 0) {
					gubWWNSupport = 1;
					break;
				}
			}


			//NCQ
			if (InfoArray->ubNCQSupport == 0) { //default:0 enable:1 disable:2
				gubNCQSupport = NCQ_Support; //MP default, run FW setting
			}
			else if (InfoArray->ubNCQSupport == 1) {
				gubNCQSupport = 1;
			}
			else {
				gubNCQSupport = 0;
			}

			//48bit command
			if (InfoArray->ub48BitCmdSupport == 0) { //default:0 enable:1 disable:2
				gubLBA48BitCmdSupport = LBA48_Support;//MP default, run FW setting
			}
			else if (InfoArray->ub48BitCmdSupport == 1) {
				gubLBA48BitCmdSupport = 1;
			}
			else {
				gubLBA48BitCmdSupport = 0;
			}

			//write cache
			if (InfoArray->ubWriteCacheSupport == 0) { //default:0 enable:1 disable:2
				gubWriteCacheSupport = WrCache_Support;//MP default, run FW setting
			}
			else if (InfoArray->ubWriteCacheSupport == 1) {
				gubWriteCacheSupport = 1;
			}
			else {
				gubWriteCacheSupport = 0;
			}


			if (InfoArray->ubDCOSupport == 0) { //default:0 enable:1 disable:2
				gubDCOSupport = DCO_Support;//MP default, run FW setting
			}
			else if (InfoArray->ubDCOSupport == 1) {
				gubDCOSupport = 1;
			}
			else {
				gubDCOSupport = 0;
			}

			if (InfoArray->ubHPASupport == 0) { //default:0 enable:1 disable:2
				gubHPASupport = HPA_Support;//MP default, run FW setting
			}
			else if (InfoArray->ubHPASupport == 1) {
				gubHPASupport = 1;
			}
			else {
				gubHPASupport = 0;
			}

			if (InfoArray->ubAMAXSupport == 0) { //default:0 enable:1 disable:2
				gubAMAXSupport = AMAX_Support;//MP default, run FW setting
			}
			else if (InfoArray->ubAMAXSupport == 1) {
				gubAMAXSupport = 1;
			}
			else {
				gubAMAXSupport = 0;
			}

			if (InfoArray->ubSanitizeSupport == 0) { //default:0 enable:1 disable:2
				gubSanitizeSupport = Sanitize_Support;//MP default, run FW setting
			}
			else if (InfoArray->ubSanitizeSupport == 1) {
				gubSanitizeSupport = 1;
			}
			else {
				gubSanitizeSupport = 0;
			}

			if (InfoArray->ubWUNCSupport == 0) { //default:0 enable:1 disable:2
				gubWUNCSupport = WUNC_Support;//MP default, run FW setting
			}
			else if (InfoArray->ubWUNCSupport == 1) {
				gubWUNCSupport = 1;
			}
			else {
				gubWUNCSupport = 0;
			}

			if (InfoArray->ubZeroSupport == 0) { //default:0 enable:1 disable:2
				gubZeroSupport = ZERO_Support;//MP default, run FW setting
			}
			else if (InfoArray->ubZeroSupport == 1) {
				gubZeroSupport = 1;
			}
			else {
				gubZeroSupport = 0;
			}

			gubFormFactor = InfoArray->ubFormFactor;


			UBYTE ubXferModeEnable;
			if (InfoArray->ubXferModeEnable == 0) { //default:0 enable:1 disable:2
				ubXferModeEnable = ENABLE_XFERMODE;
			}
			else if (InfoArray->ubXferModeEnable == 1) {
				ubXferModeEnable = 1;
			}
			else {
				ubXferModeEnable = 0;
			}

			if ((ubXferModeEnable == 0) || ((InfoArray->ubMultiDMAMode[1] == 0) && (InfoArray->ubUltraDMAMode[1] == 0))) {
				gXferMode.ubDefaultDMAType = SEL_MDMA;
				gXferMode.ubDMASelectMode[SEL_MDMA] = 2;
				gXferMode.ubDMASupportMode[SEL_MDMA] = 2;
				gXferMode.ubDMASupportMAP[SEL_MDMA] = (BIT0 | BIT1 | BIT2);
				gXferMode.ubDMASelectMode[SEL_UDMA] = 0;
				gXferMode.ubDMASupportMode[SEL_UDMA] = 6;
				gXferMode.ubDMASupportMAP[SEL_UDMA] = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6);
			}
			else {
				if (InfoArray->ubMultiDMAMode[1]) {
					gXferMode.ubDefaultDMAType = SEL_MDMA;
					gXferMode.ubDMASelectMode[SEL_MDMA] = flaCheckLogValue(InfoArray->ubMultiDMAMode[1]);
					gXferMode.ubDMASelectMode[SEL_UDMA] = 0;
				}
				else {
					gXferMode.ubDefaultDMAType = SEL_UDMA;
					gXferMode.ubDMASelectMode[SEL_UDMA] = flaCheckLogValue(InfoArray->ubUltraDMAMode[1]);
					gXferMode.ubDMASelectMode[SEL_MDMA] = 0;
				}

				gXferMode.ubDMASupportMode[SEL_MDMA] = flaCheckLogValue(InfoArray->ubMultiDMAMode[0]);
				gXferMode.ubDMASupportMAP[SEL_MDMA] = InfoArray->ubMultiDMAMode[0];

				gXferMode.ubDMASupportMode[SEL_UDMA] = flaCheckLogValue(InfoArray->ubUltraDMAMode[0]);
				gXferMode.ubDMASupportMAP[SEL_UDMA] = InfoArray->ubUltraDMAMode[0];
			}
			gubDefaultDMAType = gXferMode.ubDefaultDMAType;
			gubDefaultDMASelectMode[SEL_MDMA] = gXferMode.ubDMASelectMode[SEL_MDMA];
			gubDefaultDMASelectMode[SEL_UDMA] = gXferMode.ubDMASelectMode[SEL_UDMA];

			//Identify Word 129~159
			if (InfoArray->ubIDW129_159Enable == 0) {
				gubIDW129_159Enable = ENABLE_IDW129_159;
			}
			else if (InfoArray->ubIDW129_159Enable == 1) {
				gubIDW129_159Enable = 1;
			}
			else {
				gubIDW129_159Enable = 0;
			}

			if (gubIDW129_159Enable) {
				memcpy(guwIDW129_159, (void*)(InfoArray->uwIDW129_159), sizeof(guwIDW129_159));
			}
			else {
				memset(guwIDW129_159, 0, sizeof(guwIDW129_159));
			}

			// DataInOut Threshold Byte 268~269
			if (InfoArray->uwDataInOutThreshold == 0) {
				guwDataInOutThreshold = DATAINOUT_THRESHOLD;
			}
			else {
				//memcpy(guwDataInOutThreshold, (void*)(InfoArray->uwDataInOutThreshold), sizeof(guwDataInOutThreshold));
				guwDataInOutThreshold = InfoArray->uwDataInOutThreshold;
			}
			// DataVerify Byte 388
#if TLC
			if (InfoArray->ubFlushD1VerifyMode == 0) { //default:0 enable:1 disable:2
#if Hynix
#if Hynix3DV6
				gubFlushD1VerifyMode = (D3_ECC_VERIFY | D3_ECC_VERIFY_M);
#else
				gubFlushD1VerifyMode = (D3_ECC_VERIFY | D3_ECC_VERIFY_M | D3_ECC_VERIFY_U);
#endif
#elif YMTC
				gubFlushD1VerifyMode = (D3_ECC_VERIFY | D3_ECC_VERIFY_L | D3_ECC_VERIFY_M | D3_ECC_VERIFY_U);
#else
				gubFlushD1VerifyMode = (D3_ECC_VERIFY | D3_ECC_VERIFY_M);
#endif

			}
			else if (InfoArray->ubFlushD1VerifyMode & BIT1) {
				gubFlushD1VerifyMode = 0;
			}
			else {
				gubFlushD1VerifyMode = InfoArray->ubFlushD1VerifyMode;
			}
#endif

			if (InfoArray->ubUnitReadCheckThreshold == 0) {
				gubUnitReadCheckThreshold = 1; //   default Open ReadDisturb
			}
			else if (InfoArray->ubUnitReadCheckThreshold == 1) {
				gubUnitReadCheckThreshold = 1; // 防呆 , 避免別人亂填
			}
			else {
				gubUnitReadCheckThreshold = 0xFF;	//  填0, 1  以外的值 , 關掉 . In Code , 0xFF means dont open ReadDisturb
			}

		}

		gubCENumber_MP = InfoArray->ubCENumber;

		if (InfoArray->ubSecuritySupport == 0) { //default:0 enable:1 disable:2
			gubSecuritySupport = Security_Support;//MP default, run FW setting
		}
		else if (InfoArray->ubSecuritySupport == 1) {
			gubSecuritySupport = 1;
		}
		else {
			gubSecuritySupport = 0;
		}

		if (InfoArray->ubSecurityEraseNormalNeedDoRealErase == 0) { //default:0 enable:1 disable:2
			gubSecurityEraseNormalNeedDoRealErase = SecurityEraseNormalNeedDoRealErase; // default enable
		}
		else if (InfoArray->ubSecurityEraseNormalNeedDoRealErase == 1) {
			gubSecurityEraseNormalNeedDoRealErase = 1;
		}
		else {
			gubSecurityEraseNormalNeedDoRealErase = 0;
		}

		if (InfoArray->ubDLMCSupport == 0) { //default:0 enable:1 disable:2
			gubDLMCSupport = DLMC_Support;//MP default, run FW setting
		}
		else if (InfoArray->ubDLMCSupport == 1) {
			gubDLMCSupport = 1;
		}
		else {
			gubDLMCSupport = 0;
		}

		if (InfoArray->ubTrimSupport == 0) { //default:0 enable:1 disable:2
			gubTrimSupport = Trim_Support;//MP default, run FW setting
		}
		else if (InfoArray->ubTrimSupport == 1) {
			gubTrimSupport = 1;
		}
		else {
			gubTrimSupport = 0;
		}

		if (InfoArray->ubDLMCReturnDummyAbort == 0) { //default:0 enable:1 disable:2
			gubDLMCReturnDummyAbort = ENABLE_DLMC_RETURN_DUMMY_ABORT;//MP default, run FW setting
		}
		else if (InfoArray->ubDLMCReturnDummyAbort == 1) {
			gubDLMCReturnDummyAbort = 1;
		}
		else {
			gubDLMCReturnDummyAbort = 0;
		}

#if Hynix
		if (InfoArray->ubRSEL_Enable == 0) { //default:0 enable:1 disable:2
			SYS1L[SYS1L_PAD_FLH_CTRL]  &= (~CR_FLH_RSEL);		//MP default, run FW setting Disable
		}
		else if (InfoArray->ubRSEL_Enable == 1) {
			SYS1L[SYS1L_PAD_FLH_CTRL]  |= CR_FLH_RSEL;
		}
		else {
			SYS1L[SYS1L_PAD_FLH_CTRL]  &= (~CR_FLH_RSEL);
		}
#endif

		if (InfoArray->ubSMARTExecuteOfflineEnable == 0) { //default:0 enable:1 disable:2
			gubSMARTExecuteOfflineEnable = ENABLE_SMART_SELFTEST_EXECUTE_OFFLINE_IMMEDIATE;//MP default, run FW setting
		}
		else if (InfoArray->ubSMARTExecuteOfflineEnable == 1) {
			gubSMARTExecuteOfflineEnable = 1;
		}
		else {
			gubSMARTExecuteOfflineEnable = 0;
		}


		if (InfoArray->uwSLCPool_PE_Threshold == 0) {
			guwSLCPool_PE_Threshold = SLCPOOL_PE_THRESHOLD;
		}
		else {
			guwSLCPool_PE_Threshold = InfoArray->uwSLCPool_PE_Threshold;
		}

		if (InfoArray->ulMaxPECycle_D3 == 0) {
			if (gubUltraMLCEnable && guoFlashUseType.B.btA2Cmd) {
				gulMaxPECycle_D3 = PECYCLE_ULTRA;
			}
			else {
				gulMaxPECycle_D3 = (guoFlashUseType.B.btSLC ? PECYCLE_SLC : (guoFlashUseType.B.bt4LC ? PECYCLE_MLC : PECYCLE_TLC));
			}
		}
		else {
			gulMaxPECycle_D3 = InfoArray->ulMaxPECycle_D3;
		}

		if (InfoArray->ulMaxPECycle_D1 == 0) {
			if (gubUltraMLCEnable && guoFlashUseType.B.btA2Cmd) {
				gulMaxPECycle_D1 = PECYCLE_ULTRA;
			}
			else {
				gulMaxPECycle_D1 = PECYCLE_SLC;
			}
		}
		else {
			gulMaxPECycle_D1 = InfoArray->ulMaxPECycle_D1;
		}

		if (InfoArray->ubDisableEnableMultiPlaneMode == 0) {
			gubMultiPlaneNum = MULTIPLANE_DEFAULT;
		}
		else {
			gubMultiPlaneNum = InfoArray->ubDisableEnableMultiPlaneMode;
		}

		if (InfoArray->ubDisableEnableA2Mode == 0) {
			gubA2CmdEnable = ENABLE_A2_CMD;
		}
		else {
			gubA2CmdEnable = InfoArray->ubDisableEnableA2Mode;
		}

		gulSmartStatusRuleMap = InfoArray->ulSmartStatusRuleMap;
		//--------------------------------added by Sosa , edited Varible name by OTHER----------------------------

		if (InfoArray->ubWLCheckRate == 0) {
			gubMaxSkipCheckWLNum = WLCheckRate;
		}
		else {
			gubMaxSkipCheckWLNum = InfoArray->ubWLCheckRate;
		}
		gubSkipCheckWLNum = WLCheckRate_Minimum;

		if (InfoArray->ubSwiftFlushEnable == 0) {
			gubSWIFTFLUSH = ENABLE_SWIFT_FLUSH;
		}
		else if (InfoArray->ubSwiftFlushEnable == 1) {
			gubSWIFTFLUSH = 1;
		}
		else {
			gubSWIFTFLUSH = 0;
		}

		if (InfoArray->ubGuaranteeFlushEnable == 0) {
			gubGUARANTEE_FLUSH = ENABLE_GUARANTEE_FLUSH;
		}
		else if (InfoArray->ubGuaranteeFlushEnable == 1) {
			gubGUARANTEE_FLUSH = 1;
		}
		else {
			gubGUARANTEE_FLUSH = 0;
		}

#if (0==ENABLE_PFAIL)
		if (InfoArray->ubSpecialDataEnable == 2) {
			gubEnableSpecialData = 0;
		}
		else {
			if (SYS1B[SYS1B_SR_HW_VER] > 1) {
				gubEnableSpecialData = 1;
			}
			else {
				gubEnableSpecialData = 0;
			}
		}
#else
		gubEnableSpecialData = 0;
#endif

		if (InfoArray->ubSetBGThreshouldEnable == 1) {
			gulBGSLC_StartThreshold = InfoArray->ulBGSLCPoolCopyThreshold;
			gulBGSLC_EndThreshold = InfoArray->ulBGSLCPoolReleaseThreshold;
			gubBGMLC_StartThreshold = InfoArray->ubBGMLCCopyThreshold;
			gubBGMLC_EndThreshold = InfoArray->ubBGMLCCopyReleaseThreshold;
		}
		else {
			gulBGSLC_StartThreshold = BGSLCPoolCopyThreshold;
			gulBGSLC_EndThreshold = BGSLCPoolReleaseThreshold;
			gubBGMLC_StartThreshold = BGMLCCopyThreshold;
			gubBGMLC_EndThreshold = BGMLCCopyReleaseThreshold;
		}

		gulBGSLC_Threshold = gulBGSLC_StartThreshold;
		gubBGMLC_Threshold = gubBGMLC_StartThreshold;

		if (InfoArray->ubSLCPoolEnable) {
			gubAdjust_SLCPool_Enable = 1;
			gulAdjust_SLCPool_Size = InfoArray->ulSLCPoolSizeIn4KEntry ;
		}
		else {
			gubAdjust_SLCPool_Enable = 0;
		}

		if (InfoArray->ubforceactiveGRSLC == 1) { //byte 400//如果是1,蓋掉上面那段
			gub_forceactiveGRalwaysSLC = 1;
			gubAdjust_SLCPool_Enable = 1;
			gulAdjust_SLCPool_Size = (gulNativeSize >> 3) * 4; //gulNativeSize 是SSD size以512byte單位
		}
		else {
			gub_forceactiveGRalwaysSLC = 0;
		}

		if (InfoArray->ubNoPWICEnable == 2) {
			gubNoPWICEnable = 0;
		}
		else if (InfoArray->ubNoPWICEnable == 1) {
			gubNoPWICEnable = 1;
		}

		if (InfoArray->ubTimeToDoBGEnable == 0) { //default:0 enable:1 disable:2
			gubBackGroundClean_Enable = ENABLE_BACKGROUND_CLEAN;
		}
		else if (InfoArray->ubTimeToDoBGEnable == 1) {
			gubBackGroundClean_Enable = 1;
		}
		else {
			gubBackGroundClean_Enable = 0;
		}

		if (InfoArray->ulTimeToDoBG_Clean == 0) { //default:0 enable:1 disable:2
			gulEnterBG_Time_Flush = BG_Clean_T_Threshold;
		}
		else {
			gulEnterBG_Time_Flush = InfoArray->ulTimeToDoBG_Clean;
		}


		if (InfoArray->ulTimeToDoBG_Copy == 0) { //default:0 enable:1 disable:2
			gulEnterBG_Time_COPY = BG_Copy_T_Threshold;
		}
		else {
			gulEnterBG_Time_COPY = InfoArray->ulTimeToDoBG_Copy;
		}

		gulEnterBG_Time_D2H_RST = BG_RST_T_Threshold;
		if (InfoArray->ubSetEnterBG_Time_Enable == 1) {
			gubDIPM_THRESHOLD = InfoArray->ubDIPM_THRESHOLD;
			gubDIPM_P2S_THRESHOLD = InfoArray->ubDIPM_P2S_THRESHOLD;
			guwDIPM_AUTOP2S_THRESHOLD = InfoArray->uwDIPM_AUTOP2S_THRESHOLD;
		}
		else {
			gubDIPM_THRESHOLD = DIPM_THRESHOLD;
			gubDIPM_P2S_THRESHOLD = DIPM_P2S_THRESHOLD;
			guwDIPM_AUTOP2S_THRESHOLD = DIPM_AUTOP2S_THRESHOLD;

		}
		// HIPM Support
		if (InfoArray->ubHIPMSupport == 0) { //default:0 enable:1 disable:2
			gubHIPMSupport = HIPM_Support;
		}
		else if (InfoArray->ubHIPMSupport == 1) {
			gubHIPMSupport = 1;
		}
		else {
			gubHIPMSupport = 0;
		}
		// DIPM Support
		if (InfoArray->ubDIPMSupport == 0) { //default:0 enable:1 disable:2
			gubDIPMSupport = DIPM_Support;
		}
		else if (InfoArray->ubDIPMSupport == 1) {
			gubDIPMSupport = 1;
		}
		else {
			gubDIPMSupport = 0;
		}

#if TLC_TSB
		gubHIPMSupport = 0;
		gubDIPMSupport = 1;
#endif

		// FW-DIPM ENABLE
		if (InfoArray->ubENABLE_FW_DIPM == 0) { //default:0 enable:1 disable:2
			gubENABLE_FW_DIPM = ENABLE_FW_DIPM;
		}
		else if (InfoArray->ubENABLE_FW_DIPM == 1) {
			gubENABLE_FW_DIPM = 1;
		}
		else {
			gubENABLE_FW_DIPM = 0;
		}

		// HW-DIPM ENABLE
		if (InfoArray->ubENABLE_HW_DIPM == 0) { //default:0 enable:1 disable:2
			gubENABLE_HW_DIPM = ENABLE_HW_DIPM;
		}
		else if (InfoArray->ubENABLE_HW_DIPM == 1) {
			gubENABLE_HW_DIPM = 1;
		}
		else {
			gubENABLE_HW_DIPM = 0;
		}

		if ((gubDIPMSupport == 1) && (gubENABLE_HW_DIPM == 0) && (gubENABLE_FW_DIPM == 0)) {
			gubENABLE_FW_DIPM = 1;
		}

		if (InfoArray->ubAP2SWaitIdle2Time == 0) { //default:0
			gubAP2SWaitIdle2Time = AP2S_WAIT_IDLE2_TIME;
		}
		else {
			gubAP2SWaitIdle2Time = InfoArray->ubAP2SWaitIdle2Time;
		}


		// APS Support
		if (InfoArray->ubAPSSupport == 0) { //default:0 enable:1 disable:2
			gubAPSSupport = DIPM_APS;
		}
		else if (InfoArray->ubAPSSupport == 1) {
			gubAPSSupport = 1;
		}
		else {
			gubAPSSupport = 0;
		}

#if TLC_TSB
		gubAPSSupport = 0;
#endif

		if (InfoArray->ubThermalSensorMode == 0) { //default:0 enable:1 disable:2
			gubThermalSensorMode = THERMAL_SENSOR_MODE_EXTERNAL_33;
		}
		else if ((InfoArray->ubThermalSensorMode == THERMAL_SENSOR_MODE_EXTERNAL_33)
		         || (InfoArray->ubThermalSensorMode == THERMAL_SENSOR_MODE_INNER)
		         || (InfoArray->ubThermalSensorMode == THERMAL_SENSOR_MODE_33)) {

			gubThermalSensorMode = InfoArray->ubThermalSensorMode;
		}
		else if (InfoArray->ubThermalSensorMode == THERMAL_SENSOR_MODE_MANUAL) {
			gubThermalSensorMode = InfoArray->ubThermalSensorMode;
			gubFAKETemperatureValue = InfoArray->ubFakeTemperatureValue;
		}
		else {
			gubThermalSensorMode = THERMAL_SENSOR_MODE_EXTERNAL_33;
		}

		if (InfoArray->ub_ENABLE_DPS == 0) { //default:0 enable:1 disable:2
			gub_ENABLE_DPS = ENABLE_DPS;
		}
		else if (InfoArray->ub_ENABLE_DPS == 1) {
			gub_ENABLE_DPS = 1;
		}
		else {
			gub_ENABLE_DPS = 0;
		}


		if (InfoArray->ubACTimingTableEnable == 0) { //default:0 enable:1 disable:2
			gubACTimingTableEnable = ENABLE_AC_TIMING_TABLE;
		}
		else if (InfoArray->ubACTimingTableEnable == 1) {
			gubACTimingTableEnable = 1;
		}
		else {
			gubACTimingTableEnable = 0;
		}


		if (gubACTimingTableEnable) {
			gubACTimingClock = InfoArray->ubACTimingClock;
			memcpy((void *)gubACTimingTable, (void*)InfoArray->ubACTimingTable, sizeof(gubACTimingTable));
		}


		if (InfoArray->ubDPSEnableUseTimeOut == 0) { //default:0 enable:1 disable:2
			gubDPSEnableUseTimeOut = ENABLE_DPS_USE_TIMEOUT ;
		}
		else if (InfoArray->ubDPSEnableUseTimeOut == 1) {
			gubDPSEnableUseTimeOut = 1;
		}
		else {
			gubDPSEnableUseTimeOut = 0;
		}

		if (InfoArray->uwDPSWaitTimeOutMiliSec == 0) { //default:0
			guwDPSWaitTimeOutMiliSec = DPS_WAIT_TIMEOUT_TIME_MILISEC;
		}
		else {
			guwDPSWaitTimeOutMiliSec = InfoArray->uwDPSWaitTimeOutMiliSec;
		}

		UBYTE ubCounter;
		U32 ulOldOperationTime;
		gubSSC_Enable = InfoArray->ubSSC_Enable;
		if (InfoArray->ubDrivingEnable == 1) {
			gDrivingSetting.ubDrivingEnable = InfoArray->ubDrivingEnable;
			gDrivingSetting.ubALEPads = InfoArray->ubALEPads;
			gDrivingSetting.ubCLEPads = InfoArray->ubCLEPads;
			gDrivingSetting.ubnWEPads = InfoArray->ubnWEPads;
			gDrivingSetting.ubnREPads = InfoArray->ubnREPads;
			gDrivingSetting.ubFRDYPads = InfoArray->ubFRDYPads;
			gDrivingSetting.ubnWPPads = InfoArray->ubnWPPads;
			gDrivingSetting.ubDQSPads = InfoArray->ubDQSPads;
			gDrivingSetting.ubDataPads = InfoArray->ubDataPads;
			gDrivingSetting.ubnCEPads = InfoArray->ubnCEPads;
		}
		else {
			gDrivingSetting.ubDrivingEnable = 0;
		}
		gODTSetting.ubODTEnable = InfoArray->ubPadODTEnable;
		gODTSetting.ubODT = InfoArray->ubPadODT;

		if ((InfoArray->ubSetFlashODT == 1)
		        || (InfoArray->ubSetFlashODT == 2)
		        || (InfoArray->ubSetFlashODT == 3)
		        || (InfoArray->ubSetFlashODT == 4)) {

			gubFlashODT = InfoArray->ubSetFlashODT;
		}
		else if (InfoArray->ubSetFlashODT == 0xFF) {
			gubFlashODT = 0;
		}
		else {
			gubFlashODT = FLH_ODT_VALUE;
		}

#if MicronFlashOnly
		if ((InfoArray->ubSetFlashDrive == 1)
		        || (InfoArray->ubSetFlashDrive == 2)
		        || (InfoArray->ubSetFlashDrive == 3)) {

			gubFlashDrive = InfoArray->ubSetFlashDrive;
		}
		else {
			gubFlashDrive = FLH_DRIVE_VALUE;
		}
#else
		if ((InfoArray->ubSetFlashDrive == 2)
		        || (InfoArray->ubSetFlashDrive == 4)
		        || (InfoArray->ubSetFlashDrive == 6)) {

			gubFlashDrive = InfoArray->ubSetFlashDrive;
		}
		else {
			gubFlashDrive = FLH_DRIVE_VALUE;
		}
#endif
		if (InfoArray->ubSlewRateEnable == 1) {
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


			SYS1L[SYS1L_PAD_FCLE0_CFG] |= (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FCLE1_CFG] |= (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FALE0_CFG] |= (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FALE1_CFG] |= (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FWEB0_CFG] |= (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FWEB1_CFG] |= (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FREB0_CFG] |= (InfoArray->ubSlewRate << 16) | (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FREB1_CFG] |= (InfoArray->ubSlewRate << 16) | (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FDQS0_CFG] |= (InfoArray->ubSlewRate << 16) | (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FDQS1_CFG] |= (InfoArray->ubSlewRate << 16) | (InfoArray->ubSlewRate << 20);
			SYS1L[SYS1L_PAD_FDATA_DVSR0] |= (InfoArray->ubSlewRate << 0) | (InfoArray->ubSlewRate << 8) | (InfoArray->ubSlewRate << 16) | (InfoArray->ubSlewRate << 24);
			SYS1L[SYS1L_PAD_FDATA_DVSR1] |= (InfoArray->ubSlewRate << 0) | (InfoArray->ubSlewRate << 8) | (InfoArray->ubSlewRate << 16) | (InfoArray->ubSlewRate << 24);
			SYS1L[SYS1L_PAD_FDATA_DVSR2] |= (InfoArray->ubSlewRate << 0) | (InfoArray->ubSlewRate << 8) | (InfoArray->ubSlewRate << 16) | (InfoArray->ubSlewRate << 24);
			SYS1L[SYS1L_PAD_FDATA_DVSR3] |= (InfoArray->ubSlewRate << 0) | (InfoArray->ubSlewRate << 8) | (InfoArray->ubSlewRate << 16) | (InfoArray->ubSlewRate << 24);
		}

		guwPMASetting = InfoArray->uwPMASetting;

#if TLC
		if (InfoArray->uwWearlevelingRange_D1 == 0) {
			guwWearLevelingRange_D1 = WearLeveling_Range_D1;
		}
		else {
			guwWearLevelingRange_D1 = InfoArray->uwWearlevelingRange_D1;
		}
#endif

		if (InfoArray->uwWearlevelingRange_D3 == 0) {
			guwWearLevelingRange_D3 = WearLeveling_Range;
			guwWearLevelingRange_D3_High = WearLeveling_Range + (WearLeveling_Range / 2);
		}
		else {
			guwWearLevelingRange_D3 = InfoArray->uwWearlevelingRange_D3;
			guwWearLevelingRange_D3_High = InfoArray->uwWearlevelingRange_D3 + (InfoArray->uwWearlevelingRange_D3 / 2);
		}

		if (InfoArray->ubSMARTExtendScanAllEnable == 0) {
			gubSMARTExtendScanAllEnable = ENABLE_SMART_EXTEND_SCAN_ALL;
		}
		else if (InfoArray->ubSMARTExtendScanAllEnable == 1) {
			gubSMARTExtendScanAllEnable = 1;
		}
		else {
			gubSMARTExtendScanAllEnable = 0;
		}

		if (InfoArray->ubWriteProtectWriteSupport == 0) {
			gubWriteProtectWriteDummy |= BIT_TRULY_WRITEDUMMY;
		}
		else if (InfoArray->ubWriteProtectWriteSupport == 1) {
			gubWriteProtectWriteDummy |= BIT_TRULY_WRITEDUMMY;
		}
		else {
			gubWriteProtectWriteDummy = 0;
		}
		gubJTagWakeUp = 1; // GPIO and JTag Wake Up cant exist in same time.
		if (InfoArray->ubCustomerGPIOSelectMode == 0) {
			gubCustomerGPIOSelectMode = CUSTOMER_GPIO_MODE_DISABLE;
		}
		else {
			gubCustomerGPIOSelectMode = InfoArray->ubCustomerGPIOSelectMode;
			if ((gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_USER) || (gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_ALL)) {
				gubJTagWakeUp = 0; //if open Quick Erase , then Jtag Wake up need close.
			}
		}
		if ((InfoArray->ubQuickEraseWaitTime == 0) || (gubJTagWakeUp)) {
			gubQuickEraseWaitTime = 0;
		}
		else {
			gubQuickEraseWaitTime = InfoArray->ubQuickEraseWaitTime; // 以秒為單位
		}
		if (gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_WRITEPROTECT_DUMMY) {
			gubWriteProtectWriteDummy |= BIT_GPIO_WRITEDUMMY;
		}

		if (InfoArray->ubCustomerGPIO3SelectMode == 0) {
			gubCustomerGPIO3SelectMode = CUSTOMER_GPIO3_MODE_DISABLE;
		}
		else {
			gubCustomerGPIO3SelectMode = InfoArray->ubCustomerGPIO3SelectMode;
			gubEraseLightTwinkle = 0; //初始化
			M_DebugGPIO8_Low();
			gublighton = 0;
			SYS0W[SYS0W_CR_MUX_GPIO] &= CLR_BIT0; //指定gpio3
			SYS0W[SYS0W_CR_MUX_GPIO] &= CLR_BIT1; //MUX_GPIO[3], 00: for GPIO
			SET_CR_GPIO_DET_EN(BIT3); //cr_gpio_det_sel_2 enable
			SYS1B[SYS1B_CR_GPIO_DET_SEL_3] = 3;
		}
		if (InfoArray->ubGPIO3EdgeMode == 0) {
			gubGPIO3EdgeMode = GPIO_EDGE_MODE_RISING_ERASE;
		}
		else {
			gubGPIO3EdgeMode = InfoArray->ubGPIO3EdgeMode;
		}

		if (InfoArray->ubGPIOEdgeMode == 0) {
			gubGPIOEdgeMode = GPIO_EDGE_MODE_RISING_ERASE;
		}
		else {
			gubGPIOEdgeMode = InfoArray->ubGPIOEdgeMode;
		}

		gubVenderLock = 0;
		gubRealLock = 0;
		gubCheckLockCnt = 0;

		if (InfoArray->ubRMApreformat) {
			gubRMApreformat = 1;
		}
		else {
			gubRMApreformat = 0;
		}

		if (InfoArray->ubSMARTBufferTimeSecond == 0) {
			gubSMARTBufferTimeSecond = SMART_TIME_BUFFER_TIME_SECOND;
		}
		else {
			gubSMARTBufferTimeSecond = InfoArray->ubSMARTBufferTimeSecond;
		}

		if (InfoArray->uwSMARTOfflineTimeSecond == 0) {
			guwSMARTOfflineTimeSecond = SMART_TIME_OFFLINE_SECOND;
		}
		else {
			guwSMARTOfflineTimeSecond = InfoArray->uwSMARTOfflineTimeSecond;
		}

		if (InfoArray->ubSMARTShortSelfTestTimeMin == 0) {
			gubSMARTShortSelfTestTimeMin = SMART_TIME_SHORTSELFTEST_MINUTE;
		}
		else {
			gubSMARTShortSelfTestTimeMin = InfoArray->ubSMARTShortSelfTestTimeMin;
		}

		if (InfoArray->ubSMARTConveyanceSelfTestTimeMin == 0) {
			gubSMARTConveyanceSelfTestTimeMin = SMART_TIME_CONVEYANCESELFTEST_MINUTE;
		}
		else {
			gubSMARTConveyanceSelfTestTimeMin = InfoArray->ubSMARTConveyanceSelfTestTimeMin;
		}

		if (InfoArray->uwSMARTExtendedSelfTestTimeMin == 0) {
			guwSMARTExtendedSelfTestTimeMin = SMART_TIME_EXTENDEDSELFTEST_MINUTE;
		}
		else {
			guwSMARTExtendedSelfTestTimeMin = InfoArray->uwSMARTExtendedSelfTestTimeMin;
		}

		if (InfoArray->ubRUTMixPlaneEnable == 0) {
			gubRUTMixPlaneEnable = ENABLE_RUT_MIX_PLANE;
		}
		else if (InfoArray->ubRUTMixPlaneEnable == 0x50) {
			gubRUTMixPlaneEnable = 1;
		}
		else if (InfoArray->ubRUTMixPlaneEnable == 0x01) {
			gubTranslateMixPlaneMark = TRUE;
		}
		else {
			gubRUTMixPlaneEnable = 0;
		}

		if (InfoArray->ubHighTempConfigDefault == 0) { //設定高溫參數
			gubHighTempSetting = 1;
		}
		else {
			gubHighTempSetting = 0;
		}
		if (InfoArray->guwNCQTimeoutThreshold == 0) {
			guwNCQTimeoutThreshold = NCQ_TIMEOUT_THRESHOLD;
		}
		else {
			guwNCQTimeoutThreshold = InfoArray->guwNCQTimeoutThreshold;
		}

		if (InfoArray->guwATATimeoutThreshold == 0) {
			guwATATimeoutThreshold = ATA_TIMEOUT_THRESHOLD;
		}
		else {
			guwATATimeoutThreshold = InfoArray->guwATATimeoutThreshold;
		}
		if (InfoArray->ubEnableMilitaryErase == 1) { //設定military erase enable
			gubEnableMilitaryErase = 1;
		}
		else {
			gubEnableMilitaryErase = 0;
		}

		if (InfoArray->ubEnable_CTRL_INT_For_CBitZeroCase == 0) { //設定 CTRL_INT_EN, For C-bit Zero Case
			gubEnable_CTRL_INT_For_CBitZeroCase = 0;
		}
		else {
			gubEnable_CTRL_INT_For_CBitZeroCase = 1;
		}
		if ((InfoArray->ubEnable_Special_Data_Workaround == 0) || (InfoArray->ubEnable_Special_Data_Workaround == 0xF7)) { //default:0 enable:1 disable:2
			gubEnable_Special_Data_Workaround = 0xF7;
		}
		else if (InfoArray->ubEnable_Special_Data_Workaround == 1) {
			gubEnable_Special_Data_Workaround = 1;
		}
		else {
			gubEnable_Special_Data_Workaround = 0;
		}

		if (InfoArray->ubEnableTemperatureControl == 0) { //設定temperature control enable
			gubEnableTemperatureControl = 0;
			guwTemperatureDetectCnt = 0;
			gubTemperatureCooling = 0;
		}
		else {
			gubEnableTemperatureControl = 1;
			guwTemperatureDetectCnt = 0;
			gubTemperatureCooling = 0;
			if ((InfoArray->ubDropSpeedTemperature[0] == 0) || (InfoArray->ubDropSpeedTemperature[0] == 0xFF)
			        || (InfoArray->ubDropSpeedTemperature[1] == 0) || (InfoArray->ubDropSpeedTemperature[1] == 0xFF)
			        || (InfoArray->ubDropSpeedTemperature[0] >= InfoArray->ubDropSpeedTemperature[1])) {

				gubEnableTemperatureControl = 0;
			}
			else {
				gubDropSpeedTemperature[0] = InfoArray->ubDropSpeedTemperature[0];
				gubDropSpeedTemperature[1] = InfoArray->ubDropSpeedTemperature[1];
			}
			if ((InfoArray->ubDelayCnt[0] == 0) || (InfoArray->ubDelayCnt[0] == 0xFF)
			        || (InfoArray->ubDelayCnt[1] == 0) || (InfoArray->ubDelayCnt[1] == 0xFF)) {
				gulDelayCnt[0] = 15 * 1000;
				gulDelayCnt[1] = 60 * 1000;
			}
			else {
				gulDelayCnt[0] = InfoArray->ubDelayCnt[0] * 1000;
				gulDelayCnt[1] = InfoArray->ubDelayCnt[1] * 1000;
			}
		}
		if (InfoArray->ubEnableCFastIdentifyDeviceWord0Request == 0)	{ // 設定 Identify Device word 0 要不要回848A
			gubEnableCFastIdentifyDeviceWord0Request = 0;
		}
		else {
			gubEnableCFastIdentifyDeviceWord0Request = 1;
		}

		if (InfoArray->ubEnableOverClocking == 0)	{ //0:disable(default), 1:enable (R/W : 200/100), 2:enable (R/W : 150/150)
			gubEnableOverClocking = 0;
		}
		else {
			gubEnableOverClocking = InfoArray->ubEnableOverClocking;
		}
		//Support SATA, BIT0:SATA1.0 BIT1:SATAII BIT2:SATA2.5 BIT3:SATA2.6 BIT4:SATA3.0 BIT5:SATA3.1 BIT6:SATA3.2 BIT7:SATA3.3
		gubSupportSATA = InfoArray->ubSupportSATA;

		if (InfoArray->ubTemperatureFailValue == 0) {
			gubGetTempFailValue = TEMPERATURE_NACK_FAIL_RETURN_VALUE;
		}
		else {
			gubGetTempFailValue = InfoArray->ubTemperatureFailValue;
		}
#if !ENABLE_FORCE_L06B_TO_B0KB
		if ((gubFormFactor == FORM_FACTOR_CFAST)) { // 只要是CFast 就吃InfoArray的值
			gubCFastHighTemprature = InfoArray->ubCFastHighTemprature;
			gubCFastLowTemprature = InfoArray->ubCFastLowTemprature; // 填的時候填正的值即可  ex 最低溫可以到-5 , 此值填5即可
		}
		else { // 不是CFast  都走default
			gubCFastHighTemprature = 0;
			gubCFastLowTemprature = 0;
		}
#endif

		gSDLL_OFFSET_Setting.uwReadSDLLOffsetCH0 = InfoArray->uwReadSDLLOffsetCH0;
		gSDLL_OFFSET_Setting.uwReadSDLLOffsetCH1 = InfoArray->uwReadSDLLOffsetCH1;
		gSDLL_OFFSET_Setting.uwProgramSDLLOffsetCH0 = InfoArray->uwProgramSDLLOffsetCH0;
		gSDLL_OFFSET_Setting.uwProgramSDLLOffsetCH1 = InfoArray->uwProgramSDLLOffsetCH1;

		gubFlashMode_Scan = InfoArray->ubFlashInterface_Scan;
		gubFlashClock_Scan = InfoArray->ubFlashClock_Scan;

		SYS1L[SYS1L_PAD_FREB0_CFG] &= 0xFFFFF0FF;//clear bit8~11
		SYS1L[SYS1L_PAD_FREB1_CFG] &= 0xFFFFF0FF;//clear bit8~11
		SYS1L[SYS1L_PAD_FREB0_CFG] |= ((InfoArray->ubREBDuty[0] & 0x0F) << 8);
		SYS1L[SYS1L_PAD_FREB1_CFG] |= ((InfoArray->ubREBDuty[1] & 0x0F) << 8);

		if (((InfoArray->ubComWakeWorkaround == 0) && ENABLE_COMWAKEWORDAROUND) || (InfoArray->ubComWakeWorkaround == 1)) {
			SYS0L[SYS0L_PHY_PCS_SATA2] &= (~BIT6);
		}
		else {
			SYS0L[SYS0L_PHY_PCS_SATA2] |= BIT6; //其實default就會舉bit6..
		}

		for (ubCounter = 0; ubCounter < 6; ubCounter++) {
			if (!gubNoPWICEnable) {
				if (InfoArray->ubPMICSel[ubCounter << 1]) {
					ulOldOperationTime = gulOperationTime;
					while (ulOldOperationTime == gulOperationTime);
					I2C_Write_ConfigNOtrig(0x1b, 1, 0 , InfoArray->ubPMICSel[ubCounter << 1] , InfoArray->ubPMICSel[(ubCounter << 1) + 1]);
					*((volatile unsigned char*)0x0400250c) |= 0x01;
				}
				else {
					break;
				}
			}
		}
#if (!BURNER)
		if (InfoArray->ubWPVersionEnable == 0x33) {
			memcpy(WPVerTable, (void*)InfoArray->uwWPVerTable, sizeof(WPVerTable));
			memcpy(WPModelTable, (void*)InfoArray->uwWPModelTable, sizeof(WPModelTable));
		}
#endif
		gubHVFlash_RDTSetting = InfoArray->ubHVFlash_RDTSetting;
		gubEraseMode_RDTSetting = InfoArray->ubEraseMode_RDTSetting;
		gubHVFlash_FWSetting = InfoArray->ubHVFlash_FWSetting;
		gubEraseMode_FWSetting = InfoArray->ubEraseMode_FWSetting;
		//----------------------------------------------------------------------------------------------------------------
		//MP開卡工規會在Info Block 帶入下面的資訊,可以用來判定高低溫的卡控
#if(!BURNER)
		if (InfoArray->ubIndustrialSpec == 0x2) { // Industrial Specification
			gubIndustrial_Specification = 1;
			//工規溫度範圍 -40~85度
			gubTemperatureMax = 85;
			gubTemperatureMin = 216; //-40度

		}
		else {
			gubIndustrial_Specification = 0;
			//商規溫度範圍 0~70度
			gubTemperatureMax = 70;


		}
#endif
	}
	else {
		flaLEDBlink(0);
	}

}

#if ENABLE_DEBUG_UART_FLH_INITIAL
void ftlCheckRemappingTable_AccordingToDBT(U32 ulDBTAddr)
{
	UBYTE ubSystemSet, ubCEIndex, ubCheckSet;
	UWORD uwCheckBlock;
	UBYTE ubRemappingTable_Error = 0;

	//根據DBT檢查Remapping Table
	for (ubSystemSet = 0; ubSystemSet < SYSTEM_SETS; ubSystemSet++) {
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			ubCheckSet = ftlGetSystemSetNumber(Mode_ByDBTinFlash, ulDBTAddr, ubCEIndex, guwSystemBlockRemappingTable[ubSystemSet][ubCEIndex]);
			if (ubCheckSet != ubSystemSet) {
				ubRemappingTable_Error = 1;
				break;
			}
			uwCheckBlock = ftlGetAnotherSystemBlockInSet(Mode_ByDBTinFlash, ulDBTAddr, ubCEIndex, guwSystemBlockRemappingTable[ubSystemSet][ubCEIndex]);
			if (uwCheckBlock != guwSystemBlockRemappingTable[ubSystemSet][!ubCEIndex]) {
				ubRemappingTable_Error = 1;
				break;
			}
		}
		if (ubRemappingTable_Error == 1) {
			break;
		}
	}

	if (ubRemappingTable_Error == 1) {
		UartString("\n ubRemappingTable_Error");
		flaLEDBlink(1);
	}
}
#endif

void ftlRebuildRemappingTableByOldRule_AccordingToDBT(U32 ulDBTAddr)
{
	UBYTE *ubDBTBuffer = (UBYTE *)ulDBTAddr;
	//UBYTE ubChannel;
	UBYTE ubSystemSet;
	U32 ulTotalDBTSizeInByte = 4 * 1024;
	U32 uli;
	UWORD uwBlockNumber = 0;
	uli = 0;

	//if(gubSystemArea_RemappingRule == Rule_UnitPlaneA){//只挑plane A
	uli = 0;
	ubSystemSet = 0;
	//因為在Rule_UnitPlaneA的情況下guwSystemAreaNum[0]必等於guwSystemAreaNum[1], 所以看[0]就好
	while ((uli < ulTotalDBTSizeInByte) && (ubSystemSet < SYSTEM_SETS) && (uwBlockNumber < guwSystemAreaNum[0])) {
		if (ubDBTBuffer[uli] == 0x00) {
			if (gubPlanesPerBurst == 2) { //CE0 and CE1的PlaneA都是good blk
				if (ubDBTBuffer[uli + (gubBurstsPerBank * (gubPlanesPerBurst - 1))] == 0x00) {
					guwSystemBlockRemappingTable[ubSystemSet][0] = uwBlockNumber;
					guwSystemBlockRemappingTable[ubSystemSet][1] = uwBlockNumber;
					ubSystemSet++;
					//UartString("\n Block:");
					//UartWordHex(uwBlockNumber);
				}
				else {
				}
			}
			else {//1 channel
				guwSystemBlockRemappingTable[ubSystemSet][0] = uwBlockNumber;
				guwSystemBlockRemappingTable[ubSystemSet][1] = 0xFFFF;
				ubSystemSet++;
			}
		}
		uli += (U32)(gubPlanesPerPage * gubDieNumber);//下一個PlaneA Block
		uwBlockNumber += gubBurstsPerBank;
	}

	//}
	/*else if(gubSystemArea_RemappingRule == Rule_Set){
		for(ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++){
			uli = ((U32)(ubChannel << gubBurstsPerBankLog));
			ubSystemSet = 0;
			uwBlockNumber = 0;
			while ((uli < ulTotalDBTSizeInByte) && ubSystemSet < SYSTEM_SETS) {
				if(ubDBTBuffer[uli] == 0x00){
					guwSystemBlockRemappingTable[ubSystemSet][ubChannel] = uwBlockNumber;
					ubSystemSet++;
				}

				if((uwBlockNumber & gubBurstsPerBankMask) == gubBurstsPerBank - 1){
					uli += ((U32)(gubPlanesPerPage - (UBYTE)(uli & gubPlanesPerPageMask)) + (U32)(gubPlanesPerPage * (gubDieNumber - 1)) + (U32)(ubChannel << gubBurstsPerBankLog));
				}
				else{
					uli++;
				}
				uwBlockNumber++;
			}
		}
	}*/
}

void InitialSystemRemappingTable(U32 ulRAMAddr)
{
	UBYTE ubSystemSet, ubCEIndex;
	UWORD *uwSysBlockBuffer = (UWORD *)(ulRAMAddr);
	UBYTE *ubSysBlockBuffer = (UBYTE *)(ulRAMAddr);
	UBYTE ubZeroCounter[MAX_CHANNEL] = {0, 0};

	if (ubSysBlockBuffer[M_SysInfo_SystemArea_RemappingRule] == Rule_UnitPlaneA) {
		gubSystemArea_RemappingRule = Rule_UnitPlaneA;
	}
	//read SystemRemappingTable
	for (ubSystemSet = 0; ubSystemSet < SYSTEM_SETS; ubSystemSet++) {
		for (ubCEIndex = 0; ubCEIndex < MAX_CHANNEL; ubCEIndex++) {
			guwSystemBlockRemappingTable[ubSystemSet][ubCEIndex] = uwSysBlockBuffer[(M_SysInfo_SystemBlockRemapping_Set0_CH0 / 2) + (ubSystemSet * MAX_CHANNEL) + ubCEIndex];
			if (guwSystemBlockRemappingTable[ubSystemSet][ubCEIndex] == 0) {
				ubZeroCounter[ubCEIndex]++;
			}
		}
	}
	//check SystemRemappingTable rule
	for (ubCEIndex = 0; ubCEIndex < MAX_CHANNEL; ubCEIndex++) {
		if (ubZeroCounter[ubCEIndex] >= 2) { //block0不可能同時存在2個set, 所以此systemblock沒有存RemappingTable
			gubTransformSystemUnitToSystemSet = TRUE;
			gubSystemArea_RemappingRule = Rule_UnitPlaneA;
			break;
		}
	}
}


void SetInfoBySystemBlock()
{
#if(!BURNER)
	if ( gubZ2Buffer[M_SysInfo_ParameterPageVaild] ) {
		memcpy(&gubParameterPage[0], &gubZ2Buffer[M_SysInfo_ParameterPage], sizeof(gubParameterPage));
		flaSetParamPageInfo(0);
	}
	else {
		// old vrsion,
		flaSetParamPageInfo(1);
	}

	flaInfoBlockSetting((SYSTEM_GROUP_t *)(U32)(&gubZ2Buffer[M_SysInfo_InfoBlk]));
	flaSetFlashUseType(1);
	flaSetFlashAddressType();
	flaSetParameter(1);

	if (guoFlashUseType.B.btToshibaBiCs2 && TLC_BICS2) {
		if (gubZ2Buffer[M_SysInfo_RetryTable_eable]) {
			memcpy((void *)&gubHardBitRetryGroupNumber[0], (void*)&gubZ2Buffer[M_SysInfo_RetryTable], sizeof(gubHardBitRetryGroupNumber));
			memcpy((void *)&gubHardBitRetryBytePerGroup[0], (void*)&gubZ2Buffer[M_SysInfo_RetryTable + sizeof(gubHardBitRetryGroupNumber) ], sizeof(gubHardBitRetryBytePerGroup));


			// table 0
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE]++;
			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE]++;

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];
			UWORD uwHardBitRetryD2D3GroupSize = gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE];
			memset(gubHardBitRetryParameterTable, 0, gubHardBitRetryD2D3GroupBase + uwHardBitRetryD2D3GroupSize);

			memcpy((void *)&gubHardBitRetryParameterTable[1], (void*)&gubZ2Buffer[M_SysInfo_RetryTable + sizeof(gubHardBitRetryGroupNumber) + sizeof(gubHardBitRetryBytePerGroup)], gubHardBitRetryD2D3GroupBase - 1);

			memcpy((void *)&gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE]],
			       (void*)&gubZ2Buffer[M_SysInfo_RetryTable + sizeof(gubHardBitRetryGroupNumber) + sizeof(gubHardBitRetryBytePerGroup) + gubHardBitRetryD2D3GroupBase - 1], uwHardBitRetryD2D3GroupSize);

			gRRTMode.B.btMPSet = 1; // for MP 帶 RRT


			if ((uwHardBitRetryD2D3GroupSize + gubHardBitRetryD2D3GroupBase) > sizeof(gubHardBitRetryParameterTable)) {
				flaLEDBlink(1);
			}
			if ((gubHardBitRetryD2D3GroupBase == 0) || (uwHardBitRetryD2D3GroupSize == 0)) {
				flaLEDBlink(1);
			}

			UBYTE ubi;
			for ( ubi = 0 ; ubi < HB_RETRY_Q_NUM ; ubi++ ) {
				gHBRetryQI[ubi].ubLinkLast = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] - 1) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] - 1);
				gHBRetryQI[ubi].ubLinkNum = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE]) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE]);
			}

		}
	}

#if TLC_BICS2
	if (ENABLE_MP_SET_RETRY_TABLE) {
		if ((gubZ2Buffer[M_SysInfo_ubNandTypeBiCS3] & BIT7) && (guoFlashUseType.B.btToshibaBiCs4 || guoFlashUseType.B.btToshibaBiCs3)) {

			memcpy((void *)&gubHardBitRetryGroupNumber[0], (void*)&gubZ2Buffer[M_SysInfo_RetryTable], sizeof(gubHardBitRetryGroupNumber));
			memcpy((void *)&gubHardBitRetryBytePerGroup[0], (void*)&gubZ2Buffer[M_SysInfo_RetryTable + sizeof(gubHardBitRetryGroupNumber) ], sizeof(gubHardBitRetryBytePerGroup));

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];
			UWORD uwHardBitRetryD2D3GroupSize = gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE];
			gRRTMode.B.btRelaxMode = (gubZ2Buffer[M_SysInfo_RetryTable_1k] == 0x26) ? 1 : 0;
			gRRTMode.B.btMPSet = 1; // for MP 帶 RRT
			gRRTMode.B.ubType = (gubZ2Buffer[M_SysInfo_ubNandTypeBiCS3] & 0xF );
			gubRRTVersion = gubZ2Buffer[M_SysInfo_RetryTable + 4];

			memcpy((void *)&gubHardBitRetryParameterTable[0], (void*)&gubZ2Buffer[M_SysInfo_RetryTable_1k + 1 + uwHardBitRetryD2D3GroupSize], gubHardBitRetryD2D3GroupBase);
			memcpy((void *)&gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase], (void*)&gubZ2Buffer[M_SysInfo_RetryTable_1k + 1], uwHardBitRetryD2D3GroupSize);

			if ((uwHardBitRetryD2D3GroupSize + gubHardBitRetryD2D3GroupBase) > sizeof(gubHardBitRetryParameterTable)) {
				flaLEDBlink(1);
			}
			if ((gubHardBitRetryD2D3GroupBase == 0) || (uwHardBitRetryD2D3GroupSize == 0)) {
				flaLEDBlink(1);
			}

			UBYTE ubi;
			for ( ubi = 0 ; ubi < HB_RETRY_Q_NUM ; ubi++ ) {
				gHBRetryQI[ubi].ubLinkLast = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] - 1) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] - 1);
				gHBRetryQI[ubi].ubLinkNum = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE]) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE]);
			}

		}
		else if (gubZ2Buffer[M_SysInfo_ubNandTypeBiCS3] && guoFlashUseType.B.btToshibaBiCs3 && ((gubDensitysize == FLH_Size_256Gb) || (gubDensitysize == FLH_Size_512Gb)) && (!guoFlashUseType.B.btSandisk)) {

			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
			gRRTMode.B.ubType = (gubZ2Buffer[M_SysInfo_ubNandTypeBiCS3] & 0xF );


			if (gubDensitysize == FLH_Size_256Gb) {
				if (gRRTMode.B.ubType == 1) {
					// ref: M2CMP01-339_BiCS FLASH Gen3 TLC Read Retry with Set Feature Rev0.6.pdf
					gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 8; // uwGroupNumber[0] Fast
					gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 56; // uwGroupNumber[1] Non-Fast
					gRRTMode.B.btRelaxMode = 1;
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_256Gb_Normal, sizeof(gubRetryRTable_BISC3_256Gb_Normal));
				}
				else if (gRRTMode.B.ubType == 2) {
					// ref: M2CMP01-460 BiCS FLASH Gen3 256Gb eTLC Read Retry with Set Feature Rev0.1_for CM.pdf
					gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 26; // uwGroupNumber[0] Fast
					gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 111; // uwGroupNumber[1] Non-Fast
					gRRTMode.B.btRelaxMode = 0;
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_256Gb_Enterprise, sizeof(gubRetryRTable_BISC3_256Gb_Enterprise));
				}
				else if (gRRTMode.B.ubType == 3) {
					// ref: M2CMP01-476_BiCS3 Industrial 256Gb TLC_pSLC Read Retry Rev0.1_20180629.pdf
					gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 8; // uwGroupNumber[0] Fast
					gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 56; // uwGroupNumber[1] Non-Fast
					gRRTMode.B.btRelaxMode = 1;
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_256Gb_Industrial, sizeof(gubRetryRTable_BISC3_256Gb_Industrial));
				}
			}
			else {
				if (gRRTMode.B.ubType == 1) {
					// ref: M2CMP01-457 BiCS FLASH Gen3 512Gb TLC Read Retry with Set Feature Rev1.0 - Relax Read.pdf
					gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 8; // uwGroupNumber[0] Fast
					gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 56; // uwGroupNumber[1] Non-Fast
					gRRTMode.B.btRelaxMode = 1;
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_TOSHIBA_512Gb_Normal, sizeof(gubRetryRTable_BISC3_TOSHIBA_512Gb_Normal));
				}
				else if (gRRTMode.B.ubType == 2) {
					// ref: M2CMP01-441 BiCS FLASH Gen3 512Gb eTLC Read Retry with Set Feature Rev0.1_for CM.pdf
					gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 8; // uwGroupNumber[0] Fast
					gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 56; // uwGroupNumber[1] Non-Fast
					gRRTMode.B.btRelaxMode = 0;
					memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC3_TOSHIBA_512Gb_Enterprise, sizeof(gubRetryRTable_BISC3_TOSHIBA_512Gb_Enterprise));
				}
			}
			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

			UBYTE ubi;
			for ( ubi = 0 ; ubi < HB_RETRY_Q_NUM ; ubi++ ) {
				gHBRetryQI[ubi].ubLinkLast = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] - 1) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] - 1);
				gHBRetryQI[ubi].ubLinkNum = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE]) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE]);
			}

		}
	}
#endif
#if ENABLE_DPS
	if (gubZ2Buffer[M_SysInfo_gubDPSTableValid]) {
		memcpy((void *)&gubDPSTable[0], (void*)&gubZ2Buffer[M_SysInfo_DPSTable], sizeof(gubDPSTable));
		if (ENABLE_DPS) {
			gubDPSTableValid = 1;
		}
	}
#endif
	if (gubZ2Buffer[M_SysInfo_ubGPIOEraseDoing]) {
		gubGPIOEraseDoing = 1;
	}
	else {
		gubGPIOEraseDoing = 0;
	}


	U32 *ulCopyBuffer = (U32 *)(&gubZ2Buffer[0]);
	//UWORD *uwCopyBuffer = (UWORD *)(&gubZ2Buffer[0]);
	gulNeededUnitNum = ulCopyBuffer[(M_SysInfo_ulNeededUnitNum / 4)];
	gubNoPWICEnable = gubZ2Buffer[M_SysInfo_gubNoPWIC];
	gubCENumber_Preformat = gubZ2Buffer[M_SysInfo_PreformatCENum];
	//guwSystemAreaNum[0] = uwCopyBuffer[(M_SysInfo_SystemAreaNumBlock_CH0 / 2)];
	//guwSystemAreaNum[1] = uwCopyBuffer[(M_SysInfo_SystemAreaNumBlock_CH1 / 2)];

	if (ICE) {
		gubDram_Size = SIZE_32MB;
	}
	else {
		gubDram_Size = gubZ2Buffer[M_SysInfo_gubDram_Size];
	}
	if (gubDram_Size < 4) {
		gubSDRSizeMB = (BIT0 << (gubDram_Size + 2));
	}
	else {
		gubSDRSizeMB = (BIT0 << (gubDram_Size + 3));
	}

	if (ASIC && ENABLE_HIGH_SPEED) {
		flaSetFlashMode();
	}

	flaSet_ODT_DRIVE();

#if MicronFlashOnly
	if (guoFlashUseType.B.btIntelMicron) {
		gubReadVTShift = 0;
		if (ENABLE_DEBUG_READ_VT_SHIFT && gubReadVTShift) {
			// shift offset :  + (gubTemp * 10 mV)
			UBYTE ubPIOData[] = {gubReadVTShift, 0, 0, 0};
			// shift r01 += shift_offset
			flaSetFeature(0, 0xA0, ubPIOData);
			flaSetFeature(1, 0xA0, ubPIOData);
			// shift r00 += shift_offset
			flaSetFeature(0, 0xA1, ubPIOData);
			flaSetFeature(1, 0xA1, ubPIOData);
			// shift r10 += shift_offset
			flaSetFeature(0, 0xA2, ubPIOData);
			flaSetFeature(1, 0xA2, ubPIOData);
			// shift rLP += shift_offset
			flaSetFeature(0, 0xA3, ubPIOData);
			flaSetFeature(1, 0xA3, ubPIOData);
			// shift rSLC += shift_offset
			flaSetFeature(0, 0xA4, ubPIOData);
			flaSetFeature(1, 0xA4, ubPIOData);
		}

		//read out OTP page & init LLR Table
		//flaReadOTPPage(0);
	}
#endif

	if (ASIC && ENABLE_HIGH_SPEED) {
		gubCurrentFlashClock = gubFlashClock;
		flaSwitchClock(gubFlashClock);
		fla_Set_Offset_DQS_Delay(gubFlashClock);        //Only Works with MP info setting
	}
#endif
}

#if Hynix
void flaSetNandParameter(UBYTE ch, UBYTE addr, UBYTE data)
{
	if (guoFlashDefaultType.B.btHynix) {
#if Hynix3DV6 || Hynix3DV5
		return;
#endif
		gFREG = (volatile REG_t *)gulFlashIP_RegBase[ch];
		//----- Set parameter -----//
		//UBYTE ubDieOffset = (flaCheckLogValue((U32)(guwDieBlockShift >> 8))+(gubPlanesPerPageLog - gubPlanesPerBurstLog));
		UBYTE ubDieOffset = ((Hynix_16nm) ? 5 : 4);
		UBYTE ubDie = 0;
		for (ubDie = 0; ubDie < gubDieNumber; ubDie++) {
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x78;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
			gFREG[FCTLL_PIO_ADR].B.B0 = (ubDie << ubDieOffset);

			gFREG[FCTLL_PIO_CMD].B.B0 = 0x36;
#if !Hynix_16nm
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x96;
			PIO_WDATA(0x04, gFREG);
#endif
			gFREG[FCTLL_PIO_ADR].B.B0 = addr;
			PIO_WDATA(data, gFREG);
#if !Hynix_16nm
			gFREG[FCTLL_PIO_ADR].B.B0 = 0x96;
			PIO_WDATA(0x00, gFREG);
#endif
			gFREG[FCTLL_PIO_CMD].B.B0 = 0x16;
			idle(10);
		}
	}
}
#endif
UBYTE  flaSetToggleMode(UBYTE ubMode , UBYTE ubForceSetFeature)
{
#if (!MicronFlashOnly)
#if ((TLC_NES_TEST_LEGACY_MODE)||(!TLC))
	//Fla_Set_Offset_DQS_Delay(0);
	if (gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
		if (!guoFlashDefaultType.B.btToggle) { //Legacy
			if (ubMode != INTERFACE_LEGACY) {
				return 1;
			}
			return 0;
		}
		else { //Toggle
			if (ubMode == INTERFACE_TOGGLE2) {
				return 1;
			}
			return 0;
		}
	}
#endif

	UBYTE ubFeature[MAX_CHANNEL][4] = {{0}}, ubFeatureCheck[MAX_CHANNEL][4] = {{0}};
	UBYTE ubGetFeatureCount, ubCount;
	UBYTE ubi, ubj, ubk;
	UBYTE ubChannel, ubCEIndex;
	UBYTE ubAddr = 0;
	U32 ulChannelMap = 0;
	U32 ulTemp[MAX_CHANNEL], ulTemp2[MAX_CHANNEL];

	UBYTE ubCEPerChannel ;

	UBYTE ubWriteData = 0;
	ULLONG uoCE_FailMap = 0;
#if UFS
	UBYTE ubStartCh = 0, ubEndCh = gubPlanesPerBurst;
#else
	UBYTE ubNeedSetFeature;
#endif
#if Hynix
	UBYTE reset_cmd;
	UBYTE ubParam = 0;
#endif
	for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
		ulTemp[ubChannel] = FCTLL[FCTLL_FLH_SET + (ubChannel * 0x200 >> 2)];
		ulTemp2[ubChannel] = FCTLL[FCTLL_IO_SET + (ubChannel * 0x200 >> 2)];
	}

	ubCEPerChannel = (gubCENumber >> gubPlanesPerBurstLog);
	if (ENABLE_SANDISK_1ZNM_SPF) {
		ubCEPerChannel = ((gubCENumber + gubHideCE * gubPlanesPerBurst) >> gubPlanesPerBurstLog);
	}
#if ENABLE_ODD_CE
	UBYTE ubASymmetricCE = 0;
	if ((gubCENumber & BIT0) && (gubPlanesPerBurst > 1)) {
		ubCEPerChannel ++;
		ubASymmetricCE = 1;
	}
#endif
#if Hynix

	if (ENABLE_PSEUDO_CE) {
		ubCEPerChannel /= gubExpandCE_Ratio;
	}

	for ( ubi = 0; ubi < gubPlanesPerBurst ; ubi++) {
		ulChannelMap |= (BIT0 << (ubi * 8));
	}
	if (guoFlashDefaultType.B.btHynix) {
		reset_cmd = 0xFC;
	}
	else {
		reset_cmd = 0xFF;
	}

	if (guoFlashDefaultType.B.btHynix && guoFlashUseType.B.bt8LC) {
		if (ubMode == INTERFACE_TOGGLE2) {
			ubAddr = 0x02;
			ubWriteData = (BIT2 | BIT1 | BIT0);
		}
		else if (ubMode == INTERFACE_TOGGLE1) {
			ubAddr = 0x01;
			ubWriteData = 0x20;
		}
		else if (ubMode == INTERFACE_LEGACY) {
			ubAddr = 0x01;
			ubWriteData = 0x00;
		}
		else if (ubMode == INTERFACE_TOGGLE2To1) {
			ubAddr = 0x02;
			ubWriteData = 0;
		}
		else { // odt drive strength
			ubAddr = 0x10;
			ubWriteData = 0x04; // default, should replace by gubFlashODT

			//ubWriteData = gubFlashODT;

		}
	}
	else
#endif
	{
		if (ENABLE_PSEUDO_CE) {
			ubCEPerChannel /= gubExpandCE_Ratio;
		}

		for ( ubi = 0; ubi < gubPlanesPerBurst ; ubi++) {
			ulChannelMap |= (BIT0 << (ubi * 8));
		}

		if (ubMode == INTERFACE_TOGGLE2 || ubMode == SET_FLH_ODT || ubMode == INTERFACE_TOGGLE2To1) {
			ubAddr = 0x02;
		}
		else if (ubMode == INTERFACE_TOGGLE1 || ubMode == INTERFACE_LEGACY) {
			ubAddr = 0x80;
		}

		if (ubMode == INTERFACE_TOGGLE2) {
			ubWriteData = (gubFlashODT << 4) | (BIT2 | BIT1 | BIT0); //BIT0~BIT3 for turn on DQS_Bar and RE, BIT4~BIT7 for Flash ODT setting //
		}
		else if (ubMode == INTERFACE_TOGGLE2To1) {
			ubWriteData = 0;
		}
		else if (ubMode == SET_FLH_ODT ) {
			ubWriteData = (FLH_ODT_VALUE << 4);
		}
		else if (ubMode == INTERFACE_TOGGLE1 ) {
			ubWriteData = 0;
		}
		else if (ubMode == INTERFACE_LEGACY) {
			ubWriteData = 1;
		}
	}
	for (ubi = 0; ubi < ubCEPerChannel; ubi++) {
		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nChannel Map ");
			UartLongHex(ulChannelMap << ubi);
		}
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
#if UFS
			if ((guwNotSetMap & (BIT0 << ubi)) == 0) {
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubi]);
				ubStartCh = 0;
			}
			else {
				FCONL[FCONL_FCE_ENB] = 0;
				ubStartCh = 1;
			}
			if ((guwNotSetMap & (BIT0 << (ubi + 8))) == 0) {
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubi]);
				ubEndCh = gubPlanesPerBurst;
			}
			else {
				ubEndCh = 1;
			}
#else
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubi]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubi]);
#endif
		}
		else {
			FCONL[FCONL_FCE_ENB] = (ulChannelMap << ubi);
		}

		//----- Get Feature -----//
		//ToriTani said: relevant to flash interface, cannot not set 2 channel one time, so need to set channel separately.
#if Hynix
		if (guoFlashDefaultType.B.btHynix && guoFlashUseType.B.bt8LC) {
			memset((void *)&ubFeature[0][0], 0, sizeof(ubFeature));
		}
		else {
			for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
#if ENABLE_ODD_CE
				if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
					break;
				}
#endif
				flaReset(ubChannel, reset_cmd);
				flaSetFlashInterface(ubChannel, ubi);
				flaGetFeature(ubChannel, ubAddr, &ubFeature[0][0]);
				flaReset(ubChannel, reset_cmd);
			}
		}
#else

#if UFS
		for (ubChannel = ubStartCh; ubChannel < ubEndCh; ubChannel++)
#else
		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++)
#endif
		{
#if ENABLE_ODD_CE
			if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
				break;
			}
#endif
			flaReset(ubChannel, 0xFF);
			flaSetFlashInterface(ubChannel, ubi);
			flaGetFeature(ubChannel, ubAddr, &ubFeature[0][0]);
			flaReset(ubChannel, 0xFF);
		}
#endif
		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nG1");
			for (ubj = 0; ubj < gubPlanesPerBurst; ubj++) {
				UartString("\r\n");
				for (ubk = 0; ubk < 4; ubk++) {
					UartString(" ");
					Uart_Tx_DataHex(ubFeature[ubj][ubk]);
				}
			}
		}
#if !UFS
		if (ubForceSetFeature == 0) {
			ubNeedSetFeature = 0;

#if UFS
			for (ubChannel = ubStartCh; ubChannel < ubEndCh; ubChannel++)
#else
			for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++)
#endif
			{
#if ENABLE_ODD_CE
				if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
					break;
				}
#endif
				if ( (ubWriteData != ubFeature[ubChannel][0]) ||
				        ubFeature[ubChannel][1] != 0 ||
				        ubFeature[ubChannel][2] != 0 ||
				        ubFeature[ubChannel][3] != 0) {

					ubNeedSetFeature  = 1;
				}
			}
			if (ubNeedSetFeature == 0) {
				continue;
			}

		}
#endif
		//----- Set feature -----//
#if UFS
		for (ubChannel = ubStartCh; ubChannel < ubEndCh; ubChannel++)
#else
		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++)
#endif
		{
#if ENABLE_ODD_CE
			if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
				break;
			}
#endif
			if (ubMode == INTERFACE_TOGGLE2) {
				ubFeature[ubChannel][0] |= ubWriteData;
#if Hynix
				ubParam = 0x01;
#endif
			}
			else if (ubMode == INTERFACE_TOGGLE2To1) {
				ubFeature[ubChannel][0] = ubWriteData;
#if Hynix
				ubParam = 0x01;
#endif
			}
			else if (ubMode == SET_FLH_ODT ) {
				ubFeature[ubChannel][0] |= ubWriteData;
			}
			else if (ubMode == INTERFACE_TOGGLE1 ) {
				ubFeature[ubChannel][0] = ubWriteData;
				ubFeature[ubChannel][1] = 0;
#if Hynix
				ubParam = 0x01;
#endif
			}
			else if (ubMode == INTERFACE_LEGACY) {
				ubFeature[ubChannel][0] = ubWriteData;
				ubFeature[ubChannel][1] = 0;
			}

			ubFeature[ubChannel][2] = 0;
			ubFeature[ubChannel][3] = 0;
#if Hynix3DV6
			if (guoFlashDefaultType.B.btHynix3DV5) {
				if (ubAddr == 0x01) { // set feature vccq 1.2v
					ubFeature[ubChannel][2] |= BIT0;
				}
			}
#endif


#if Hynix
			//UBYTE drv_strength[] = {0x06, 0x00, 0x00, 0x00};
			flaSetNandParameter(ubChannel, 0x70, ubParam);
			flaSetFeature(ubChannel, ubAddr, &ubFeature[ubChannel][0]);
			//flaSetFeature(ubChannel, 0x10, drv_strength);
			flaReset(ubChannel, reset_cmd);
#else
			flaSetFeature(ubChannel, ubAddr, &ubFeature[ubChannel][0]);
			flaReset(ubChannel, 0xFF);
#endif

		}

		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nS");
			for (ubj = 0; ubj < gubPlanesPerBurst; ubj++) {
				UartString("\r\n");
				for (ubk = 0; ubk < 4; ubk++) {
					UartString(" ");
					Uart_Tx_DataHex(ubFeature[ubj][ubk]);
				}
			}
		}



#if UFS
		for (ubChannel = ubStartCh; ubChannel < ubEndCh; ubChannel++) {
			FCTLL[FCTLL_FLH_SET + (0x200 >> 2) * ubChannel] &= FLA_LEGACY_MODE;
			if (ubMode == INTERFACE_TOGGLE2) {
				FCTLL[FCTLL_FLH_SET + (0x200 >> 2) * ubChannel] |= FLA_TOGGLE_MODE;
				flaSetPadIOType(ubChannel, DIFFERENTIAL_IO_TYPE);
			}
			else if (ubMode == INTERFACE_TOGGLE1 || ubMode == INTERFACE_TOGGLE2To1) {
				FCTLL[FCTLL_FLH_SET + (0x200 >> 2) * ubChannel] |= FLA_TOGGLE_MODE;
				flaSetPadIOType(ubChannel, SINGLE_END_IO_TYPE);
			}
			else {
				FCTLL[FCTLL_FLH_SET + (0x200 >> 2) * ubChannel] &= FLA_LEGACY_MODE;
				flaSetPadIOType(ubChannel, SINGLE_END_IO_TYPE);
			}
		}
#else
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
		if (ubMode == INTERFACE_TOGGLE2) {
			FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
			flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
#if Hynix
			guoFlashUseType.B.btToggle = TRUE;
#endif
		}
		else if (ubMode == INTERFACE_TOGGLE1 || ubMode == INTERFACE_TOGGLE2To1) {
			FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
			flaSetPadIOType(2, SINGLE_END_IO_TYPE);
#if Hynix
			guoFlashUseType.B.btToggle = TRUE;
#endif
		}
		else {
			FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
			flaSetPadIOType(2, SINGLE_END_IO_TYPE);
#if Hynix
			guoFlashUseType.B.btToggle = 0;
#endif
		}
#endif

		//----- Get Feature Check -----//
		if (BURNER) {
			ubGetFeatureCount = 10;
		}
		else {
			ubGetFeatureCount = 1;
		}

		for (ubCount = 0; ubCount < ubGetFeatureCount; ubCount++) {

			for (ubj = 0; ubj < gubPlanesPerBurst; ubj++) {//先清成0
				for (ubk = 0; ubk < 4; ubk++) {
					ubFeatureCheck[ubj][ubk] = 0;
				}
			}
#if UFS
			for (ubChannel = ubStartCh; ubChannel < ubEndCh; ubChannel++)
#else
			for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++)
#endif
			{
#if ENABLE_ODD_CE
				if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
					break;
				}
#endif
				ubCEIndex = (ubChannel * gubMAX_SCAN_CE_PER_CH + ubi); //F_CE
				ubCEIndex = (ubCEIndex / gubMAX_SCAN_CE_PER_CH) + (ubCEIndex % gubMAX_SCAN_CE_PER_CH) * gubPlanesPerBurst; //F_CE轉成Logical CE
				ubCEIndex = TranslateLogicalCEtoPhysicalCE(ubCEIndex);//最後轉成physical CE

				if ((uoCE_FailMap & (((ULLONG)BIT0) << ubCEIndex)) == 0) {
					flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);
#if Hynix
					flaReset(ubChannel, reset_cmd);
#else
					flaReset(ubChannel, 0xFF);
#endif
				}
			}

			if (ENABLE_DEBUG_UART_FLH_INITIAL) {
				UartString("\r\nG2");
				for (ubj = 0; ubj < gubPlanesPerBurst; ubj++) {
					UartString("\r\n");
					for (ubk = 0; ubk < 4; ubk++) {
						UartString(" ");
						Uart_Tx_DataHex(ubFeatureCheck[ubj][ubk]);
					}
				}
			}

#if UFS
			for (ubChannel = ubStartCh; ubChannel < ubEndCh; ubChannel++)
#else
			for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++)
#endif
			{
				if (ubCount == (ubGetFeatureCount - 1)) {
					FCTLL[FCTLL_FLH_SET + (ubChannel * 0x200 >> 2)] = ulTemp[ubChannel];
					FCTLL[FCTLL_IO_SET + (ubChannel * 0x200 >> 2)] = ulTemp2[ubChannel];
				}
#if ENABLE_ODD_CE
				if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
					break;
				}
#endif
				ubCEIndex = (ubChannel * gubMAX_SCAN_CE_PER_CH + ubi); //F_CE
				ubCEIndex = (ubCEIndex / gubMAX_SCAN_CE_PER_CH) + (ubCEIndex % gubMAX_SCAN_CE_PER_CH) * gubPlanesPerBurst; //F_CE轉成Logical CE
				ubCEIndex = TranslateLogicalCEtoPhysicalCE(ubCEIndex);//最後轉成physical CE

				if ((uoCE_FailMap & (((ULLONG)BIT0) << ubCEIndex)) == 0) {
					for (ubj = 0; ubj < 4; ubj ++) {
						if (ubFeatureCheck[ubChannel][ubj] != ubFeature[ubChannel][ubj]) {
#if Hynix
							// for hynix, reset back to legacy mode, hw mode will rollback by the caller
							flaReset(ubChannel, 0xFF);
#endif
							uoCE_FailMap |= (((ULLONG)BIT0) << ubCEIndex);
						}
					}
				}
			}
		}
	}

	guoCE_FailMap = uoCE_FailMap;

	if (uoCE_FailMap) {
		return 1;
	}
	else {
		return 0;
	}
#else
	return 1;
#endif
}


UBYTE flaSetONFIMode(UBYTE ubMode)
{
#if MicronFlashOnly || YMTC

	// ubInterface = 0, NV-DDR
	// ubInterface = 1, NV-DDR2
	// ubInterface = 2, NV-DDR2 without toggle2 signals


	_UCV REG_t * pFlaReg;
	UBYTE ubFeature[4] = {0};
	UBYTE ubFeatureCheck[MAX_CHANNEL][4] = {{0}};
	UBYTE ubGetFeatureCount, ubCount;
	UBYTE ubChannel;
	U32 ulChannelMap = 0;
	ULLONG uoCE_FailMap = 0;

	UBYTE ubCEPerChannel ;
	UBYTE ubAddr ;
	UBYTE ubi, ubj;
	UBYTE ubForceToLegacy = 0;

	ubCEPerChannel = (gubCENumber >> gubPlanesPerBurstLog);
#if ENABLE_ODD_CE
	UBYTE ubASymmetricCE = 0;
	if ((gubCENumber & BIT0) && (gubPlanesPerBurst > 1)) {
		ubCEPerChannel ++;
		ubASymmetricCE = 1;
	}
#endif
	if (BURNER) {
		ubGetFeatureCount = 10;
	}
	else {
		ubGetFeatureCount = 1;
	}

	for ( ubChannel = 0; ubChannel < gubPlanesPerBurst ; ubChannel++) {
		ulChannelMap |= (BIT0 << (ubChannel * 8));
	}

	if ( ubMode == INTERFACE_NVDDR2 || ubMode == INTERFACE_NVDDR2_WITHOUTTOGGLE2) {
		// NVDDR2

		//	   AC timing spec.		value of P0,		FLH IP setting,
		// mode 0: 33Mhz/66Mbps, 	0					No ONFI, stay in Legacy mode
		// mode 1: 40Mhz/80Mbps, 	1					40Mhz/80Mbps
		// mode 2: 66Mhz/133Mbps, 	2					50Mhz/100Mbps
		// mode 3: 83Mhz/166Mbps, 	3					83Mhz/166Mbps
		// mode 4: 100Mhz/200Mbps, 	4					100Mhz/200Mbps
		// mode 5: 133Mhz/266Mbps, 	5					100Mhz/200Mbps
		// mode 6: 166Mhz/333Mbps,	6					166Mhz/333Mbps
		// mode 7: 200Mhz/400Mbps,	7					200Mhz/400Mbps
		// mode 8: 266Mhz/533Mbps,	8					266Mhz/533Mbps

		if ( gubFlashClock == FLH_CLK_266MHz) {
			ubFeature[0] = 8 | BIT5;
		}
		else if ( gubFlashClock == FLH_CLK_200MHz) {
			ubFeature[0] = 7 | BIT5;
		}
		else if ( gubFlashClock == FLH_CLK_180MHz) {
			ubFeature[0] = 7 | BIT5;
		}
		else if ( gubFlashClock == FLH_CLK_166MHz) {
			ubFeature[0] = 6 | BIT5;
		}
		else if ( gubFlashClock == FLH_CLK_100MHz) {
			ubFeature[0] = 4 | BIT5;
		}
		else if ( gubFlashClock == FLH_CLK_83MHz) {
			ubFeature[0] = 3 | BIT5;
		}
		else if ( gubFlashClock == FLH_CLK_50MHz) {
			ubFeature[0] = 2 | BIT5;
		}
		else if ( gubFlashClock == FLH_CLK_40MHz) {
			ubFeature[0] = 1 | BIT5;
		}
		else {
			// use a not defined clock
			//flaLEDBlink(0x11);
			return 1;
		}
	}
	else {
		//NVDDR

		//	   AC timing spec.		value of P0,		FLH IP setting,
		// mode 0: 20Mhz/40Mbps, 	0					No ONFI, stay in Legacy mode
		// mode 1: 33Mhz/66Mbps, 	1					No ONFI, stay in Legacy mode
		// mode 2: 50Mhz/100Mbps, 	2					50Mhz/100Mbps
		// mode 3: 67Mhz/134Mbps, 	3					50Mhz/100Mbps
		// mode 4: 83Mhz/166Mbps, 	4					83Mhz/166Mbps
		// mode 5: 100Mhz/200Mbps, 	5					100Mhz/200Mbps

		if ( gubFlashClock == FLH_CLK_100MHz) {
			ubFeature[0] = 5 | BIT4;
		}
		else if ( gubFlashClock == FLH_CLK_83MHz) {
			ubFeature[0] = 4 | BIT4;
		}
		else if ( gubFlashClock == FLH_CLK_50MHz) {
			ubFeature[0] = 2 | BIT4;
		}
		else {
			// use a not defined clock
			//flaLEDBlink(0x11);
			return 1;
		}
	}

	ubAddr = 0x01;

	//Switch to ONFI/NVDDR/NVDDR2 mode from Legacy.
	for (ubi = 0; ubi < ubCEPerChannel; ubi++) {
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubi]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubi]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = ulChannelMap << ubi;
		}
		flaSetFlashInterface(0, ubi);
		flaSetFlashInterface(1, ubi);

		//send set feature with FA=01 for switching to ONFI/NVDDR/NVDDR2.
		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
#if ENABLE_ODD_CE
			if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
				break;
			}
#endif
			pFlaReg = (volatile REG_t*)gulFlashIP_RegBase[ubChannel];
			pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xEF;
			pFlaReg[FCTLL_PIO_ADR].B.B0 = ubAddr;
			pFlaReg[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First
			if ((pFlaReg[FCTLL_FLH_SET].B.B0 & FLA_ONFI_MODE) == FLA_ONFI_MODE) {
				pFlaReg[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
				for (ubj = 0; ubj < 4; ubj++) {
					pFlaReg[FCTLL_PIO_DAT].B.B0 = ubFeature[ ubj];
				}
			}
			else {
				for (ubj = 0; ubj < 4; ubj++) {
					if (ubj == 3) {
						pFlaReg[FCTLL_HS_MODE].L |= PIO_DAT_LAST;    //Last
					}

					pFlaReg[FCTLL_PIO_DAT].B.B0 = ubFeature[ ubj];

					if (ubj == 0) {
						pFlaReg[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST)); //First
					}
				}
				pFlaReg[FCTLL_HS_MODE].L |= PIO_DAT_FIRST | PIO_DAT_LAST; // set to default value
			}
			// Polling RB for waiting tITC, when busy during tITC, all command is prohibit including status command.
			while ( (pFlaReg[FCTLL_RBY_INF].L & FLH_CE_RBY_AND) == 0) {};
		}
	}

	//set controller into ONFI mode
	if (ubMode == INTERFACE_NVDDR2) {
		gubSupportToggle2 = 1;
		guoFlashUseType.B.btToggle = TRUE;
		flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
		FALLL[FCTLL_RESERVE] |= DETECT_DQSB_ENABLE;
	}
	else {
		FALLL[FCTLL_FLH_SET] |= FLA_ONFI_MODE;
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);
	}

	//Read timing mode by get feature command.
	for (ubi = 0; ubi < ubCEPerChannel; ubi++) {
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubi]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubi]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = ulChannelMap << ubi;
		}

		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
			UBYTE ubCEIndex = (ubChannel * gubMAX_SCAN_CE_PER_CH + ubi); //F_CE
			ubCEIndex = (ubCEIndex / gubMAX_SCAN_CE_PER_CH) + (ubCEIndex % gubMAX_SCAN_CE_PER_CH) * gubPlanesPerBurst; //F_CE轉成Logical CE
			ubCEIndex = TranslateLogicalCEtoPhysicalCE(ubCEIndex);//最後轉成physical CE
#if ENABLE_ODD_CE
			if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
				break;
			}
#endif
			//If one of all CEs timing mode difference from others. force to switch to legacy

			//----- Get Feature -----//
			for (ubCount = 0; ubCount < ubGetFeatureCount; ubCount++) {
				UBYTE ubk;
				for (ubj = 0; ubj < gubPlanesPerBurst; ubj++) {//先清成0
					for (ubk = 0; ubk < 4; ubk++) {
						ubFeatureCheck[ubj][ubk] = 0;
					}
				}
				flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);

				for (ubj = 0; ubj < 4; ubj ++) {
					if (ubFeatureCheck[ubChannel][ubj] != ubFeature[ubj]) {
						ubForceToLegacy = 1;

						uoCE_FailMap |= (((ULLONG)BIT0) << ubCEIndex);
					}
				}
				if (uoCE_FailMap & (((ULLONG)BIT0) << ubCEIndex)) {
					break;
				}
			}
		}
	}

#if ENABLE_NVDDR2_DIFFERENTIAL
	if (ubMode == INTERFACE_NVDDR2) {
		ubAddr = 0x02;
		ubFeature[0] = 0x07;
		ubFeature[1] = 0x00;
		ubFeature[2] = 0x00;
		ubFeature[3] = 0x00;
		//Switch to ONFI/NVDDR/NVDDR2 mode from Legacy.
		for (ubi = 0; ubi < ubCEPerChannel; ubi++) {
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubi]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubi]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ulChannelMap << ubi;
			}

			//send set feature with FA=01 for switching to ONFI/NVDDR/NVDDR2.
			for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
#if ENABLE_ODD_CE
				if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
					break;
				}
#endif
				pFlaReg = (volatile REG_t*)gulFlashIP_RegBase[ubChannel];
				pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xEF;
				pFlaReg[FCTLL_PIO_ADR].B.B0 = ubAddr;
				pFlaReg[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First
				for (ubj = 0; ubj < 4; ubj++) {
					if (ubj == 3) {
						pFlaReg[FCTLL_HS_MODE].L |= PIO_DAT_LAST;	 //Last
					}

					pFlaReg[FCTLL_PIO_DAT].B.B0 = ubFeature[ ubj];

					if (ubj == 0) {
						pFlaReg[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST));
					}
				}
				pFlaReg[FCTLL_HS_MODE].L |= PIO_DAT_FIRST | PIO_DAT_LAST; // set to default value
				// Polling RB for waiting tITC, when busy during tITC, all command is prohibit including status command.
				while ( (pFlaReg[FCTLL_RBY_INF].L & FLH_CE_RBY_AND) == 0) {};
			}
		}

		//Read timing mode by get feature command.
		for (ubi = 0; ubi < ubCEPerChannel; ubi++) {
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubi]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubi]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ulChannelMap << ubi;
			}

			for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
				UBYTE ubCEIndex = (ubChannel * gubMAX_SCAN_CE_PER_CH + ubi); //F_CE
				ubCEIndex = (ubCEIndex / gubMAX_SCAN_CE_PER_CH) + (ubCEIndex % gubMAX_SCAN_CE_PER_CH) * gubPlanesPerBurst; //F_CE轉成Logical CE
				ubCEIndex = TranslateLogicalCEtoPhysicalCE(ubCEIndex);//最後轉成physical CE
#if ENABLE_ODD_CE
				if (ubASymmetricCE && (ubi == ubCEPerChannel - 1) && (ubChannel == 1)) {
					break;
				}
#endif
				//If one of all CEs timing mode difference from others. force to switch to legacy

				//----- Get Feature -----//
				for (ubCount = 0; ubCount < ubGetFeatureCount; ubCount++) {
					UBYTE ubk;
					for (ubj = 0; ubj < gubPlanesPerBurst; ubj++) {//先清成0
						for (ubk = 0; ubk < 4; ubk++) {
							ubFeatureCheck[ubj][ubk] = 0;
						}
					}

					flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);

					for (ubj = 0; ubj < 4; ubj ++) {
						if (ubFeatureCheck[ubChannel][ubj] != ubFeature[ubj]) {

							ubForceToLegacy = 1;
							uoCE_FailMap |= (((ULLONG)BIT0) << ubCEIndex);

						}
					}
					if (uoCE_FailMap & (((ULLONG)BIT0) << ubCEIndex)) {
						break;
					}
				}
			}
		}

	}
#endif

	guoCE_FailMap = uoCE_FailMap;

	//Force to legacy Mode
	if (ubForceToLegacy) {
		//send reset command "FF"

		for (ubi = 0; ubi < ubCEPerChannel; ubi++) {
#if(UFS)
			if (1)
#else
			if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
			{
				FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubi]);
				FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubi]);
			}
			else {
				FCONL[FCONL_FCE_ENB] = ulChannelMap << ubi;
			}
			//----- Reset -----//
			FALLL[FCTLL_PIO_CMD] = 0xFF;
			FALLL[FCTLL_PIO_CMD] = 0x70;			//Send 0x70 to check status

			//wait RB goes high, because some of CE may switch to ONFI successiful, and when busy during tITC, all command is prohibit including status command.
			while ( (FCTLL[FCTLL_RBY_INF] & FLH_CE_RBY_AND) == 0) {};
		}
		guoFlashUseType.B.btONFI = 0;
		guoFlashUseType.B.btNVDDR = 0;
		guoFlashUseType.B.btNVDDR2 = 0;
		gubFlashMode = FLH_MODE_LEGACY;
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);
		return 1;
	}
	if (ubMode == INTERFACE_NVDDR2) {
		guoFlashUseType.B.btNVDDR2 = 1;
	}
	else {
		guoFlashUseType.B.btNVDDR = 1;
	}

#endif
	return 0;
}
UBYTE flaSetFlashMode(void)
{
	UBYTE ubSetToggleFail = 0;

	if (BURNER && (gubCENumber == 0)) {
		return 0;
	}

	gubSupportToggle2 = 0;
	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}

	if (ENABLE_DEBUG_FORCE_FLH_CLOCK != FLH_CLK_DEFAULT) {
		gubFlashClock = 	ENABLE_DEBUG_FORCE_FLH_CLOCK;
	}

	if (BURNER && ENABLE_FORCE_SET_FLH_MODE) {
		gubFlashMode = FORCE_FLH_MODE;
		gubInterface = INTERFACE_TOGGLE2;
	}

	if (BURNER && ENABLE_FORCE_SET_FLH_CLK) {
		gubFlashClock = FORCE_FLH_CLK;
	}

#if ENABLE_FORCE_NVDDR_TEST

	gubFlashMode = FLH_MODE_ONFI_NVDDR;
	gubFlashClock = FLH_CLK_100MHz;
#endif
	if ((guoFlashUseType.B.btToshiba) || (guoFlashDefaultType.B.btHynix && guoFlashUseType.B.bt8LC)) {
		if (gubFlashClock == FLH_CLK_DEFAULT) {
			switch (gulParamPageInfo.JEDEC.uwToggleDDRSpeedGrade) {
				case 0x00FF:    // 200
					gubSupportToggle2 = 1;
					if ((gubDieNumber * (gubCENumber >> gubPlanesPerBurstLog)) >= 32) {
						if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
							gubFlashClock = FLH_CLK_200MHz;
						}
						else {
							gubFlashClock = FLH_CLK_166MHz;
						}
					}
					else {
						gubFlashClock = FLH_CLK_200MHz;
					}
					gubInterface = INTERFACE_TOGGLE2;
#if Enable_Sandisk_force_toggle1
					if (guoFlashUseType.B.btSandisk) {
						gubSupportToggle2 = 0;
						gubFlashClock = FLH_CLK_100MHz;
						gubInterface = INTERFACE_TOGGLE1;
					}
#endif
					break;
				case 0x007F:    // 166
					gubSupportToggle2 = 1;
					gubFlashClock = FLH_CLK_166MHz;
					gubInterface = INTERFACE_TOGGLE2;
					break;
				case 0x003F:   //133
					gubSupportToggle2 = 1;
					gubFlashClock = FLH_CLK_100MHz;
					gubInterface = INTERFACE_TOGGLE2;
					break;
				case 0x001F:    // 100 Mhz   假設flash只support到100mhz，則不會試切到toggle2.0，因為有些只support到toggle1.0
					gubFlashClock = FLH_CLK_100MHz;
					gubInterface = INTERFACE_TOGGLE1;
					break;
				case 0x000F:    // 83
					gubFlashClock = FLH_CLK_83MHz;
					gubInterface = INTERFACE_TOGGLE1;
					break;
				case 0x0007:    // 66
					gubFlashClock = FLH_CLK_50MHz;
					gubInterface = INTERFACE_LEGACY;
					break;
				case 0x0003:    // 40
					gubFlashClock = FLH_CLK_40MHz;
					gubInterface = INTERFACE_LEGACY;
					break;
				case 0x0001:    // 33
					gubFlashClock = FLH_CLK_10MHz;
					gubInterface = INTERFACE_LEGACY;
					break;
				case 0x0000:
					gubFlashClock = FLH_CLK_DEFAULT;
					gubInterface = INTERFACE_TOGGLE1;
					break;
				case 0xEEEE:
					gubFlashClock = FLH_CLK_40MHz;
					gubInterface = (guoFlashUseType.B.btToggle ? INTERFACE_TOGGLE1 : INTERFACE_LEGACY);
					break;
				default:
					gubFlashClock = FLH_CLK_40MHz;
					gubInterface = INTERFACE_LEGACY;
					break;
			}
		}
		else if ((gubFlashClock >= FLH_CLK_100MHz) && (gubEnableOverClocking == 0)) {
			gubSupportToggle2 = 1;
		}
		else {
			gubSupportToggle2 = 0;
		}
	}
#if ENABLE_YMTC_1V2
	else if (guoFlashUseType.B.btYMTC) {
		gubFlashClock = FLH_CLK_200MHz;
		gubInterface = INTERFACE_NVDDR2;
		gubFlashMode = FLH_MODE_ONFI_NVDDR2;
	}
#endif
	else if (guoFlashUseType.B.btIntelMicron || guoFlashUseType.B.btYMTC) {
		if (gubFlashClock == FLH_CLK_DEFAULT) {
			if (gulParamPageInfo.ONFI.btNVDDR2) {
				gubInterface = INTERFACE_NVDDR2;
				gulParamPageInfo.ONFI.btNVDDR2TimingMode8 = 0;
				if ( gulParamPageInfo.ONFI.btNVDDR2TimingMode8 && (ENABLE_ONFI_200M_NVDDR2 && ENABLE_MICRON_TOGGLE2_SIGNAL) ) {
					// Mode 8 support 533Mbps
					gubFlashClock = FLH_CLK_266MHz;
				}
				else if (gulParamPageInfo.ONFI.ubNVDDR2TimingMode) {
					if ( (gulParamPageInfo.ONFI.ubNVDDR2TimingMode & BIT7) && (ENABLE_ONFI_200M_NVDDR2 && ENABLE_MICRON_TOGGLE2_SIGNAL) ) {
						// Mode 7 support 400Mbps
						gubFlashClock = FLH_CLK_200MHz;
					}
					else if (gulParamPageInfo.ONFI.ubNVDDR2TimingMode & BIT6) {
						// Mode 6 support 333Mbps
						gubFlashClock = FLH_CLK_166MHz;
					}
					else if (gulParamPageInfo.ONFI.ubNVDDR2TimingMode & BIT5) {
						// Mode 5 support 266Mbps, but set to 200Mbps
						gubFlashClock = FLH_CLK_100MHz;
					}
					else if (gulParamPageInfo.ONFI.ubNVDDR2TimingMode & BIT4) {
						// Mode 4 support 200Mbps
						gubFlashClock = FLH_CLK_100MHz;
					}
					else if (gulParamPageInfo.ONFI.ubNVDDR2TimingMode & BIT3) {
						// Mode 3 support 166Mbps
						gubFlashClock = FLH_CLK_83MHz;
					}
					else if (gulParamPageInfo.ONFI.ubNVDDR2TimingMode & BIT2) {
						// Mode 2 support 133Mbps, but set to 100Mbps
						gubFlashClock = FLH_CLK_50MHz;
					}
					else if (gulParamPageInfo.ONFI.ubNVDDR2TimingMode & BIT1) {
						// Mode 1 support 80Mbps
						gubFlashClock = FLH_CLK_40MHz;
					}
					else {
						// Mode 0 support 66Mbps, but set 40Mhz and Legacy mode.
						gubFlashClock = FLH_CLK_40MHz;
						gubInterface = INTERFACE_LEGACY;
					}

				}
				else {
					// parameter is wierd, use 40Mhz and legacy mode.
					gubFlashClock = FLH_CLK_40MHz;
					gubInterface = INTERFACE_LEGACY;
				}
			}
			else if (gulParamPageInfo.ONFI.btNVDDR) {
				gubInterface = INTERFACE_NVDDR;
				if (gulParamPageInfo.ONFI.ubNVDDRTimingMode & BIT5) {
					// Mode 5 support 200Mbps
					gubFlashClock = FLH_CLK_100MHz;
				}
				else if (gulParamPageInfo.ONFI.ubNVDDRTimingMode & BIT4) {
					// Mode 4 support 166Mbps
					gubFlashClock = FLH_CLK_83MHz;
				}
				else if (gulParamPageInfo.ONFI.ubNVDDRTimingMode & BIT3) {
					// Mode 3 support 134Mbps, but set to 100Mbps
					gubFlashClock = FLH_CLK_50MHz;
				}
				else if (gulParamPageInfo.ONFI.ubNVDDRTimingMode & BIT2) {
					// Mode 2 support 100Mbps
					gubFlashClock = FLH_CLK_50MHz;
				}
				//else if(gulParamPageInfo.ONFI.ubNVDDRTimingMode & BIT1){
				// Mode 1 support 66Mbps
				//}
				//else if(gulParamPageInfo.ONFI.ubNVDDRTimingMode & BIT0){
				// Mode 0 support 40Mbps
				//}
				else {
					//Parameter is wierd or Mode 0/1, use 40Mhz and Legacy mode.
					gubFlashClock = FLH_CLK_40MHz;
					gubInterface = INTERFACE_LEGACY;
				}
			}
			else {
				//default use LEGACY & 40Mhz
				gubFlashClock = FLH_CLK_40MHz;
				gubInterface = INTERFACE_LEGACY;
			}
		}
		else {
			if (gubFlashClock >= FLH_CLK_50MHz) {
#if ENABLE_FORCE_NVDDR_TEST
				gubInterface = INTERFACE_NVDDR;
#else
				gubInterface = INTERFACE_NVDDR2;
#endif
			}
			else {
				gubInterface = INTERFACE_LEGACY;
			}
		}
	}
	else {

	}


	if (BURNER && gubForceExit) {
		return 1;
	}

	if ( gubFlashMode == FLH_MODE_DEFAULT) {
		if ((guoFlashUseType.B.btToshiba) || (guoFlashDefaultType.B.btHynix && guoFlashUseType.B.bt8LC)) {
#if Hynix&&(!Hynix3DV6)&&(!Hynix3DV5)&&(!Hynix_16nm)
			if ((gubFlashID[5] & 0x03) == 0x01) {
				ubSetToggleFail = TRUE;
			}
			else
#endif
			{
				ubSetToggleFail = flaSetToggleMode(INTERFACE_TOGGLE1, 1);
			}

			if (ubSetToggleFail) {
				// LEGACY
				FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
				guoFlashUseType.B.btToggle = 0;
				gubFlashMode = FLH_MODE_LEGACY;

			}
			else {
				FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
#if Hynix
				flaSetPadIOType(2, SINGLE_END_IO_TYPE);
#endif
				gubFlashMode_CE[0] = 0xFF;
				gubFlashMode_CE[1] = 0xFF;

				// clk = 200 or  ParameterPage 空的...才去試toggle 2...
				if ((gubSupportToggle2) || ((gubFlashClock == FLH_CLK_DEFAULT) && (ENABLE_FORCETOGGLE1 == 0))) {
#if Enable_Sandisk_force_toggle1
					if (guoFlashUseType.B.btSandisk) {
						ubSetToggleFail = 1;
					}
					else
#endif
					{
						ubSetToggleFail = flaSetToggleMode(INTERFACE_TOGGLE2, 1);
					}

					if (ubSetToggleFail) {
						// Toggle 1.0
						guoFlashUseType.B.btToggle = TRUE;
						gubFlashMode = FLH_MODE_TOGGLE1;
						if (ENABLE_FORCETOGGLE1TO166MB) {
							gubFlashClock = FLH_CLK_83MHz;
						}
					}
					else {
						// Toggle 2.0
						guoFlashUseType.B.btToggle = TRUE;
						gubFlashMode = FLH_MODE_TOGGLE2;
						gubIOMode_CE[0] = 0xFF;
						gubIOMode_CE[1] = 0xFF;

						flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
						/*
							if (fla_Detect_DQSB()) {
								if (ENABLE_DEBUG_UART_FLH_INITIAL) {
									UartString(" \r\nDetect DQSB !!");
								}
							}
							else {
								if (ENABLE_DEBUG_UART_FLH_INITIAL) {
									UartString(" \r\n---No DQSB, switch back to Toggle1---");
								}

								if (flaSetToggleMode(INTERFACE_TOGGLE2To1, 1)) {
									flaLEDBlink(1);
								}
								else {
									// Toggle 1.0
									flaSetPadIOType(2, SINGLE_END_IO_TYPE);
									guoFlashUseType.B.btToggle = TRUE;
									gubFlashMode = FLH_MODE_TOGGLE1;
									if (ENABLE_FORCETOGGLE1TO166MB) {
										gubFlashClock = FLH_CLK_83MHz;
									}
								}
							}
						*/
					}
				}
				else {
					// Toggle 1.0
					guoFlashUseType.B.btToggle = TRUE;
					gubFlashMode = FLH_MODE_TOGGLE1;
				}
			}
		}
		else {
			gubFlashMode = FLH_MODE_LEGACY;
			guoFlashUseType.B.btONFI = 0;
			guoFlashUseType.B.btNVDDR = 0;
			guoFlashUseType.B.btNVDDR2 = 0;
			if (gubInterface == INTERFACE_LEGACY) {
				//NAND interface stay in Legacy mode.
			}
			else {
				UBYTE ubCnt = 0;
				do {
					ubSetToggleFail = flaSetONFIMode(gubInterface);
					if (ubSetToggleFail) {
						//UartString("I");
						ubCnt ++;
						if (ubCnt > 10) {
							gubFlashClock = FLH_CLK_40MHz;//flaSetONFIMode fail 10次之後會切成legacy
							guoFlashUseType.B.btNoSupport = 1;
							guoFlashDefaultType.B.btNoSupport = 1;
							break;
						}
					}

				}
				while (ubSetToggleFail);

				if (ubSetToggleFail == 0) {
					//Switch to NVDDR/NVDDR2 success
					if (gubInterface == INTERFACE_NVDDR2) {
						gubFlashMode = FLH_MODE_ONFI_NVDDR2;

						//for set differential signal
						gubIOMode_CE[0] = 0xFF;
						gubIOMode_CE[1] = 0xFF;

						//for set Toggle mode
						gubFlashMode_CE[0] = 0xFF;
						gubFlashMode_CE[1] = 0xFF;

						gubFlashONFI_CE[0] = 0x00;
						gubFlashONFI_CE[1] = 0x00;
					}
					else if (gubInterface == INTERFACE_NVDDR) {
						gubFlashMode = FLH_MODE_ONFI_NVDDR;

						//for set Single IO signal
						gubIOMode_CE[0] = 0x00;
						gubIOMode_CE[1] = 0x00;

						//for set ONFI mode
						gubFlashMode_CE[0] = 0xFF;
						gubFlashMode_CE[1] = 0xFF;

						gubFlashONFI_CE[0] = 0xFF;
						gubFlashONFI_CE[1] = 0xFF;
					}
				}
			}
		}
	}
	else {
		if ( gubFlashMode == FLH_MODE_TOGGLE2) {
			// set toggle1 -> set toggle 2
#if Hynix
			ubSetToggleFail = flaSetToggleMode(INTERFACE_TOGGLE1, 1);
#else
			ubSetToggleFail = flaSetToggleMode(INTERFACE_TOGGLE1, 0);
#endif
			if (ubSetToggleFail) {
				// LEGACY
				if (BURNER) {
					FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
					guoFlashUseType.B.btToggle = 0;
					gubFlashMode = FLH_MODE_LEGACY;
				}
				else {
					flaLEDBlink(0x11);
				}
			}
			else {
				FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
#if Hynix
				flaSetPadIOType(2, SINGLE_END_IO_TYPE);
#endif
				gubFlashMode_CE[0] = 0xFF;
				gubFlashMode_CE[1] = 0xFF;
#if Hynix
				ubSetToggleFail = flaSetToggleMode(INTERFACE_TOGGLE2, 1);
#else
				ubSetToggleFail = flaSetToggleMode(INTERFACE_TOGGLE2, 0);
#endif
				if (ubSetToggleFail) {
					// Toggle 1.0
					if (BURNER) {
						guoFlashUseType.B.btToggle = TRUE;
						gubFlashMode = FLH_MODE_TOGGLE1;

						if (ENABLE_FORCETOGGLE1TO166MB) {
							gubFlashClock = FLH_CLK_83MHz;
						}
					}
					else {
						flaLEDBlink(0x12);
					}
				}
				else {
					// Toggle 2.0
					guoFlashUseType.B.btToggle = TRUE;
					gubIOMode_CE[0] = 0xFF;
					gubIOMode_CE[1] = 0xFF;
					flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
					/*
						if (fla_Detect_DQSB()) {
							if (ENABLE_DEBUG_UART_FLH_INITIAL) {
								UartString(" \r\nDetect DQSB !!");
							}
						}
						else {
							if (ENABLE_DEBUG_UART_FLH_INITIAL) {
								UartString(" \r\n---No DQSB, switch back to Toggle1---");
							}
						}
					*/
				}
			}
		}
		else if (gubFlashMode == FLH_MODE_TOGGLE1) {
#if Hynix
			ubSetToggleFail = flaSetToggleMode(INTERFACE_TOGGLE1, 1);
#else
			ubSetToggleFail = flaSetToggleMode(INTERFACE_TOGGLE1, 0);
#endif

			if (ubSetToggleFail) {
				// LEGACY
				if (BURNER) {
					FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
					guoFlashUseType.B.btToggle = 0;
					gubFlashMode = FLH_MODE_LEGACY;
				}
				else {
					flaLEDBlink(0x13);
				}
			}
			else {
				FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
#if Hynix
				flaSetPadIOType(2, SINGLE_END_IO_TYPE);
#endif

				gubFlashMode_CE[0] = 0xFF;
				gubFlashMode_CE[1] = 0xFF;
				// Toggle 1.0
				guoFlashUseType.B.btToggle = TRUE;
			}
		}
		else if (gubFlashMode == FLH_MODE_ONFI_NVDDR) {
			UBYTE ubCnt = 0;
			do {
				ubSetToggleFail = flaSetONFIMode(INTERFACE_NVDDR);
				if (ubSetToggleFail) {
					//UartString("I");
					ubCnt ++;
					if (ubCnt > 10) {
						gubFlashClock = FLH_CLK_40MHz;//flaSetONFIMode fail 10次之後會切成legacy
						guoFlashUseType.B.btNoSupport = 1;
						guoFlashDefaultType.B.btNoSupport = 1;
						break;
					}
				}
			}
			while (ubSetToggleFail);

			if (ubSetToggleFail == 0) {
				gubFlashMode = FLH_MODE_ONFI_NVDDR;//有可能會被前一次SetONFIMode Fail清掉,要重新設回來
				//for set Single IO signal
				gubIOMode_CE[0] = 0x00;
				gubIOMode_CE[1] = 0x00;

				//for set ONFI mode
				gubFlashMode_CE[0] = 0xFF;
				gubFlashMode_CE[1] = 0xFF;

				gubFlashONFI_CE[0] = 0xFF;
				gubFlashONFI_CE[1] = 0xFF;
			}
		}
		else if (gubFlashMode == FLH_MODE_ONFI_NVDDR2) {
			UBYTE ubCnt = 0;
			do {
				ubSetToggleFail = flaSetONFIMode(INTERFACE_NVDDR2);
				if (ubSetToggleFail) {
					//UartString("I");
					ubCnt ++;
					if (ubCnt > 10) {
						gubFlashClock = FLH_CLK_40MHz;//flaSetONFIMode fail 10次之後會切成legacy
						guoFlashUseType.B.btNoSupport = 1;
						guoFlashDefaultType.B.btNoSupport = 1;
						break;
					}
				}
			}
			while (ubSetToggleFail);

			if (ubSetToggleFail == 0) {
				gubFlashMode = FLH_MODE_ONFI_NVDDR2;//有可能會被前一次SetONFIMode Fail清掉,要重新設回來
				//for set differential signal
				gubIOMode_CE[0] = 0xFF;
				gubIOMode_CE[1] = 0xFF;

				//for set Toggle mode
				gubFlashMode_CE[0] = 0xFF;
				gubFlashMode_CE[1] = 0xFF;

				gubFlashONFI_CE[0] = 0x00;
				gubFlashONFI_CE[1] = 0x00;
			}
		}
		/*else if (gubFlashMode == FLH_MODE_ONFI_NVDDR2_WITHOUT_TOGGLE2SIGNAL) {
			ubSetToggleFail = flaSetONFIMode(INTERFACE_NVDDR2_WITHOUTTOGGLE2, 1);
			if (ubSetToggleFail) {
				flaLEDBlink(0x14);
			}
			else {
				FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
				guoFlashUseType.B.btONFI = TRUE;
			}
		}*/
		else if (gubFlashMode == FLH_MODE_LEGACY) {
			FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
			guoFlashUseType.B.btONFI = 0;
			guoFlashUseType.B.btToggle = 0;
		}
		else if (gubFlashMode == FLH_MODE_IGNORE) {
		}
		else {
			// info block 帶錯??
		}
	}

	if ( gubFlashClock == FLH_CLK_DEFAULT) {
		//  設成default ,但是 ParameterPage 空的...所以根據flash mode來判斷...
		if ( gubFlashMode == FLH_MODE_TOGGLE2) {
			gubFlashClock = FLH_CLK_100MHz;
		}
		else if ( gubFlashMode == FLH_MODE_TOGGLE1) {
			gubFlashClock = FLH_CLK_83MHz;
		}
		else if ( gubFlashMode == FLH_MODE_ONFI_NVDDR) {
			gubFlashClock = FLH_CLK_83MHz;
		}
		else if (gubFlashMode == FLH_MODE_ONFI_NVDDR2) {
			if ( ENABLE_ONFI_200M_NVDDR2) {
				gubFlashClock = FLH_CLK_100MHz;
			}
			else {
				gubFlashClock = FLH_CLK_83MHz;
			}
		}
		else if (gubFlashMode == FLH_MODE_ONFI_NVDDR2_WITHOUT_TOGGLE2SIGNAL) {
			gubFlashClock = FLH_CLK_83MHz;
		}
		else {
			gubFlashClock = FLH_CLK_40MHz;
		}
	}


	if (ENABLE_AC_TIMING_SETTING) {
		ACTiming_Setting(gubFlashClock, 1);
	}

	switch (gubFlashMode) {
		case FLH_MODE_LEGACY:
			InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000, gulPara0x1C.ulAll, gulZipMap, 0x0C000000, 0x00180001);
			break;
		case FLH_MODE_TOGGLE1:
			InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000, gulPara0x1C.ulAll, gulZipMap, 0x0C000000, 0x001A0001);
			break;
		case FLH_MODE_TOGGLE2:
			InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000, gulPara0x1C.ulAll, gulZipMap, 0x0C000000, 0x001A0101);
			break;
		case FLH_MODE_ONFI_NVDDR:
			InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000, gulPara0x1C.ulAll, gulZipMap, 0x0C000000, 0x001B0001);
			break;
		case FLH_MODE_ONFI_NVDDR2:
			InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000, gulPara0x1C.ulAll, gulZipMap, 0x0C000000, 0x001A0101);
			break;
		case FLH_MODE_DEFAULT:
			if (ENABLE_HIGH_SPEED) {
				flaLEDBlink(0);
			}
			break;
		default:
			flaLEDBlink(0);
			break;
	}

	return ubSetToggleFail;

}


#if Hynix
void ToggleModeEnable(UBYTE Mode) // 1 : Toggle 1.0 Mode , 0 : Legacy Mode
{
	UBYTE TempMode;

	TempMode = guoFlashUseType.B.btToggle;

	gubForceExit = 0;
	gubFlashClock = FLH_CLK_40MHz;

	if (Mode) {
		gubFlashMode = FLH_MODE_TOGGLE1;
		gubInterface = INTERFACE_TOGGLE1;
		flaSetFlashMode();
	}
	else {
		M_FlashModeBackToDefault();
		gubFlashMode = FLH_MODE_LEGACY;
		gubInterface = INTERFACE_LEGACY;
		flaSetFlashMode();
	}
	guoFlashUseType.B.btToggle = TempMode;

}
#endif


void flaReadParameterPage(UBYTE ubCE, UBYTE ubMode)
{
	//#if(!RDT)
	// TSB, 512 bytes.
	// 19/24nm, no CRC, 3 copies
	// A19,  32 copies
	// 1Z(MLC), 32 copies
	// 1Z(TLC), 16 copies

	// Micron, 256 bytes.
	// L73A/ L74A/ L74A+, 3 copies
	// L83A/ L84A, 29 copies
	// L84C/ L84C+/ L84D/ L85A/ L85C/ L85C+, 57 copies
	// L94C, 45 copies
	// L95B, 60 copies
	// L06B, 61 copies

	// Hynix...讀到怪的值  不理了...等哪天確定要support再說...
	UWORD uwi;

	UBYTE ubTotalPageCount;
	UBYTE ubParamIndex;
	UWORD uwReadBytes ;

	UBYTE ubChannel = ubCE >> 3;
	_UCV REG_t * pFlaReg;

	UBYTE ubCRCFail = 1;
	U32 ulTemp;


	pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[ubChannel];


	if ( guoFlashUseType.B.btToshiba1Ynm) {
		ubTotalPageCount = 32;
	}
	else if ( guoFlashUseType.B.btToshiba1Znm) {
		ubTotalPageCount = 32;
	}
	else if ( guoFlashUseType.B.btToshibaBiCs2) {
		ubTotalPageCount = 0x10;
	}
	else if ( guoFlashUseType.B.btHynix) { //H14
		ubTotalPageCount = 32;
	}
	else if ( guoFlashUseType.B.btIntelMicronL06B) {
		ubTotalPageCount = 0x3D;
	}
	else if ( guoFlashUseType.B.btIntelMicronB0KB) {
		ubTotalPageCount = 0x39;
	}
	/*
		else if ( guoFlashUseType.B.btIntelMicron && guwFlashType.Micron.btL74A) {
			ubTotalPageCount = 3;
		}
		else if ( guoFlashUseType.B.btIntelMicron && guwFlashType.Micron.btL84A) {
			ubTotalPageCount = 29;
		}
		else if ( guoFlashUseType.B.btIntelMicron && guwFlashType.Micron.btL84C) {
			ubTotalPageCount = 57;
		}
		else if ( guoFlashUseType.B.btIntelMicron && guwFlashType.Micron.btL85A) {
			ubTotalPageCount = 57;
		}
		else if ( guoFlashUseType.B.btIntelMicron && guwFlashType.Micron.btL85C) {
			ubTotalPageCount = 57;
		}
		else if ( guoFlashUseType.B.btIntelMicron && guwFlashType.Micron.btL95B) {
			ubTotalPageCount = 60;
		}
	*/
	else {
		ubTotalPageCount = 1;
	}

#if Hynix
	// don't care whether it is 0xFF or 0xFC here, because it still keeps legacy mode by default
	// For hynix tlc, default is legacy mode, it is not set to toggle before read parameter page

	// The better way to modify this is making the gubFlashMode sync. with the hardware setting whenever hardware is set to
	// toggle or onfi mode, and then use gubFlashMode to check the condition instead of using guoFlashUseType.B.btToggle (btToggle only means
	// that nand can support toggle or onfi mode, but not the current working mode).
	UBYTE ubResetCmd = 0xFF;
	if (guoFlashUseType.B.btToggle) {
		ubResetCmd = 0xFC;
	}
	UBYTE force_legacy_mode = TRUE;
#endif

#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, ubCE);
	}
	else {
		FCONL[FCONL_FCE_ENB] = (BIT0 << ubCE);        // CE0 enable
	}
#if Hynix
	flaReset(0, ubResetCmd);
#else
	pFlaReg[FCTLL_PIO_CMD].B.B0 = ( guoFlashUseType.B.btNVDDR || guoFlashUseType.B.btNVDDR2) ? 0xFC : 0xFF;

#if MicronFlashOnly || YMTC_JGS
	do {
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
	}
#else
	pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
#endif
	while (pFlaReg[FCTLL_PIO_DAT].B.B0 != 0xE0);
#endif

	mSE_SV(8, (U32)BUFB_BASE, 512, 0, 0);

	pFlaReg[FCTLL_PIO_CMD].B.B0  = 0xEC;
	if ( guoFlashUseType.B.btToshiba || guoFlashUseType.B.btHynix) {
		// Toshiba or Hynix
		pFlaReg[FCTLL_PIO_ADR].B.B0  = 0x40;
		uwReadBytes = 512;
	}
	else {
		// intel micron, Hynix
		if (ubMode & PARAMETER_PAGE) {
			pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x00;	// Read ONFI's parameter
		}
		else if (ubMode & JEDEC_PAGE) {
			pFlaReg[FCTLL_PIO_ADR].B.B0 = 0x40;	// Read JEDEC's parameter
		}
		uwReadBytes = 256;
	}

#if MicronFlashOnly || YMTC_JGS
	do {
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
	}
#else
	pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
#endif
	while (pFlaReg[FCTLL_PIO_DAT].B.B0 != 0xE0);
	pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x00;


	ulTemp = pFlaReg[FCTLL_FLH_SET].L;
	for ( ubParamIndex = 0 ; (ubParamIndex < ubTotalPageCount) && (ubCRCFail == 1) ; ubParamIndex++) {
#if Hynix
		if (( guoFlashUseType.B.btToggle || ((gubFlashMode != FLH_MODE_LEGACY) && (gubFlashMode != FLH_MODE_DEFAULT))) && (force_legacy_mode == FALSE))
#else
		if ( guoFlashUseType.B.btToggle || ((gubFlashMode != FLH_MODE_LEGACY) && (gubFlashMode != FLH_MODE_DEFAULT)) )
#endif
		{
			pFlaReg[FCTLL_FLH_SET].L |= FLA_TOGGLE_MODE;//要加，否則會讀不到parameter page
			pFlaReg[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //FIRST
			for (uwi = 0; uwi < (uwReadBytes / 2); uwi++) {

				if (uwi == (uwReadBytes / 2 - 1)) {
					pFlaReg[FCTLL_HS_MODE].L |= PIO_DAT_LAST;    //Last
				}

				guwTemp = pFlaReg[FCTLL_PIO_DAT].W.W0;
				BUFB_BASE[uwi * 2 + 0] = (UBYTE)guwTemp;
				BUFB_BASE[uwi * 2 + 1] = (UBYTE)(guwTemp >> 8);

				if (uwi == 0) {
					pFlaReg[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST));
				}
			}
		}
		else {
			if ((pFlaReg[FCTLL_FLH_SET].B.B0 & FLA_ONFI_MODE) == FLA_ONFI_MODE) {
				pFlaReg[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST); // set to default value
				for (uwi = 0; uwi < uwReadBytes; uwi++) {
					BUFB_BASE[uwi] = pFlaReg[FCTLL_PIO_DAT].B.B0;
				}
			}
			else {
				pFlaReg[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //FIRST
				for (uwi = 0; uwi < uwReadBytes; uwi++) {

					if (uwi == (uwReadBytes - 1)) {
						pFlaReg[FCTLL_HS_MODE].L |= PIO_DAT_LAST;    //Last
					}

					BUFB_BASE[uwi] = pFlaReg[FCTLL_PIO_DAT].B.B0;

					if (uwi == 0) {
						pFlaReg[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST));
					}
				}
			}
		}
		pFlaReg[FCTLL_HS_MODE].L |= PIO_DAT_FIRST | PIO_DAT_LAST; // set to default value


		UBYTE ubSignature = 0;
		if ( guoFlashUseType.B.btToshiba || (guoFlashDefaultType.B.btHynix && guoFlashUseType.B.bt8LC)) {
			if ((BUFB_BASE[0] == 0x4A) && (BUFB_BASE[1] == 0x45) && (BUFB_BASE[2] == 0x53) && (BUFB_BASE[3] == 0x44)) {
				ubSignature = 1;
			}
			else if ((BUFB_BASE[0] == 0x54) && (BUFB_BASE[1] == 0x53) && (BUFB_BASE[2] == 0x42) && (BUFB_BASE[3] == 0x4E) ) { // 0~3 TSBN
				//&&(BUFB_BASE[172] == 0x4E)&&(BUFB_BASE[173] == 0x61)&&(BUFB_BASE[174] == 0x6E)&&(BUFB_BASE[175] == 0x64)
				//&&(BUFB_BASE[176] == 0x45)&&(BUFB_BASE[177] == 0x6D)&&(BUFB_BASE[178] == 0x75)){// 0~3 TSBN  172~175  Nand  176~178 Emu
				ubSignature = 1;
			}
			else if ((BUFB_BASE[0] == 0x48) && (BUFB_BASE[1] == 0x59) && (BUFB_BASE[2] == 0x4E) && (BUFB_BASE[3] == 0x58) ) { // 0~3 HYNX
				//&&(BUFB_BASE[172] == 0x4E)&&(BUFB_BASE[173] == 0x61)&&(BUFB_BASE[174] == 0x6E)&&(BUFB_BASE[175] == 0x64)
				//&&(BUFB_BASE[176] == 0x45)&&(BUFB_BASE[177] == 0x6D)&&(BUFB_BASE[178] == 0x75)){// 0~3 TSBN  172~175  Nand  176~178 Emu
				ubSignature = 1;
			}
		}
		else {
			// ONFI : Micron
			if (ubMode & JEDEC_PAGE) {
				if ((BUFB_BASE[0] == 0x4A) && (BUFB_BASE[1] == 0x45) && (BUFB_BASE[2] == 0x53) && (BUFB_BASE[3] == 0x44)) {
					ubSignature = 1;
				}
			}
			else if (ubMode & PARAMETER_PAGE) {
				if ((BUFB_BASE[0] == 0x4F) && (BUFB_BASE[1] == 0x4E) && (BUFB_BASE[2] == 0x46) && (BUFB_BASE[3] == 0x49)) {
					ubSignature = 1;
				}
				// need to open CRC when NES.
				else if ((BUFB_BASE[0] == 0x49) && (BUFB_BASE[1] == 0x4D) && (BUFB_BASE[2] == 0x4E) && (BUFB_BASE[3] == 0x45)) {
					ubSignature = 1;
				}
			}
		}

		if (ubSignature) {
			if (ubTotalPageCount == 1) {
				ubCRCFail = 0;
			}
			else {
				guwTemp = flaCalculateCRC((U32)&BUFB_BASE[0], (uwReadBytes - 2));

				if ( BUFW_BASE[(uwReadBytes - 2) / 2] == guwTemp) {
					ubCRCFail = 0;
				}
			}
		}
	}

#if Hynix
	flaReset(0, ubResetCmd);
#else
	pFlaReg[FCTLL_PIO_CMD].B.B0 = ( guoFlashUseType.B.btNVDDR || guoFlashUseType.B.btNVDDR2) ? 0xFC : 0xFF;
#if MicronFlashOnly || YMTC_JGS
	do {
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
	}
#else
	pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
#endif
	while (pFlaReg[FCTLL_PIO_DAT].B.B0 != 0xE0);

#endif


	pFlaReg[FCTLL_FLH_SET].L = ulTemp;

	if (ubCRCFail) {
		mSE_SV(8, (U32)BUFB_BASE, 512, 0, 0);
	}

	if (ubMode & SET_PARAMETER_PAGE) {
		memcpy(&gubParameterPage[0], &BUFW_BASE[0], sizeof(gubParameterPage));
#if MicronFlashOnly
		gubParameterPage[180] = 0x10;
		gubParameterPage[181] = 0xFF;
		gubParameterPage[182] = 0xFF;
		gubParameterPage[183] = 0x00;
		gubParameterPage[184] = 0x00;
#endif
	}

#if 0//Hynix //to make following FW flow work when no parameter Page
	if ((BUFL_BASE[0] == 0x00000000)) {
		BUFB_BASE[0]    = 'J';
		BUFB_BASE[1]    = 'E';
		BUFB_BASE[2]    = 'S';
		BUFB_BASE[3]    = 'D';
		BUFB_BASE[146]  = (Hynix_100Mhz_T1) ? 0x1F :  0x7F;
		BUFB_BASE[147]  = 0x00;
		BUFW_BASE[(uwReadBytes - 2) / 2] = flaCalculateCRC((U32)BUFB_BASE, (uwReadBytes - 2));
	}
#endif
}
#if MicronFlashOnly
void flaReadOTPPage(UBYTE ubCE)
{
#if 0
	UWORD uwi;
	UBYTE ubi, ubTblLen, ubGapLen, ubHeaderLen;
	M_AssertError(!guoFlashUseType.B.btIntelMicron);

	gFREG = (volatile REG_t*)gulFlashIP_RegBase[ubCE & gubPlanesPerBurstMask];

	gubZ2Buffer[0] = 0x01;
	gubZ2Buffer[1] = 0;
	gubZ2Buffer[2] = 0;
	gubZ2Buffer[3] = 0;

	// Array Operation mode feature address
	// enable OTP Block Mode

	flafailFeatureOperation(MODE_FAIL_RETRY_SET_FEATURE , ubCE & gubPlanesPerBurstMask , gubParameterPage[179] , &gubZ2Buffer[0]);

	gulTemp = gFREG[FCTLL_FLH_SET].L;

	gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;

	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
	gFREG[FCTLL_PIO_ADR].B.B0 = gubParameterPage[176];
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;

	gFREG[FCTLL_PIO_CMD].B.B0 = 0x30;

	gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;
	while (gFREG[FCTLL_PIO_DAT].B.B0 != 0xE0);

	gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;

	if ( guoFlashUseType.B.btToggle || ((gubFlashMode != FLH_MODE_LEGACY) && (gubFlashMode != FLH_MODE_DEFAULT)) ) {
		gFREG[FCTLL_FLH_SET].L |= FLA_TOGGLE_MODE;//要加，否則會讀不到parameter page
		gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //FIRST
		for (uwi = 0; uwi < (8192 / 2); uwi++) {

			if (uwi == (8192 / 2 - 1)) {
				gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;    //Last
			}

			guwTemp = gFREG[FCTLL_PIO_DAT].W.W0;
			gubZ2Buffer[uwi * 2 + 0] = (UBYTE)guwTemp;
			gubZ2Buffer[uwi * 2 + 1] = (UBYTE)(guwTemp >> 8);

			if (uwi == 0) {
				gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST));
			}
		}
	}
	else {
		gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //FIRST
		for (uwi = 0; uwi < 8192; uwi++) {

			if (uwi == (8192 - 1)) {
				gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;    //Last
			}

			gubZ2Buffer[uwi] = gFREG[FCTLL_PIO_DAT].B.B0;

			if (uwi == 0) {
				gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST));
			}
		}
	}
	gFREG[FCTLL_HS_MODE].L |= PIO_DAT_FIRST | PIO_DAT_LAST; // set to default value

	//gFREG[FCTLL_PIO_CMD].B.B0 = 0xFC;
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
	while (gFREG[FCTLL_PIO_DAT].B.B0 != 0xE0);

	gFREG[FCTLL_FLH_SET].L = gulTemp;


	// LP Bin Num + MP Bin Num + UP Bin Num
	ubTblLen = gubZ2Buffer[4] + gubZ2Buffer[5] + gubZ2Buffer[6];
	ubGapLen = 14;
	ubHeaderLen = 63;

	// compare table group & table group copy
	for (uwi = 0; uwi < gubZ2Buffer[3]; uwi += 2) {
		for (ubi = 0; ubi < gubZ2Buffer[1]; ubi++) {
			//if(gubZ2Buffer[63+ubi*(44+14)] != gubZ2Buffer[uwi*63+ubi*(44+14)])
		}
	}

	/*
	// backup original setting & enable iFSA
	gulTemp = gFREG[FCTLL_FSA_SEL].L;
	gFREG[FCTLL_FSA_SEL].L |= IFSA_EN;
	guliFSA[0] = gubParameterPage[176];
	guliFSA[0] <<= ((guoFlashUseType.B.bt1024Page) ? (gubSectorsPerPlaneLog - 1) : gubSectorsPerPlaneLog);
	gFREG[FCTLL_IFSA0].L = guliFSA[0];
	gFREG[FCTLL_CHNL_SET].L |= BYPASS_CONV;
	gFREG[FCTLL_ZIP_CFG].L |= BYPASS_ZIP;
	gFREG[FCTLL_RAW_DMA_ADR].L = ((U32)(&gubZ2Buffer[0]));
	gFREG[FCTLL_ZIP_CFG].L &= (~0x3F);
	gFREG[FCTLL_ZIP_CFG].L |= L4K_NUM(1);
	gFREG[FCTLL_DMA_CFG].L &= (TRANSFER_0_ECC_FRAME);
	gFREG[FCTLL_DMA_CFG].L |= (((UWORD)1) << 8);

	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_C00_A5_C30;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_C70_POL_MK40_C00;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	//gFREG[FCTLL_INT_INF].L &= MAP_FIFO_POINTER_RESET;
	gFREG[FCTLL_INT_INF].B.B1 &= (MAP_FIFO_POINTER_RESET_BYTE1);
	gFREG[FCTLL_INT_CFG].L |= INTR_VEC_EN;
	gFREG[FCTLL_FPU_ENTRY].L = FPU_PTR_AG_RAW;
	gFREG[FCTLL_FPU_TRIG].L = FPU_TRIG;
	while ((gFREG[FCTLL_FPU_TRIG].L & (SIGNOFF_BUSY | FPU_TRIG)) != 0);

	// restore setting
	gFREG[FCTLL_FSA_SEL].L = gulTemp;
	*/
	// Array Operation mode feature address
	// disable OTP Mode
	gubZ2Buffer[0] = 0;
	gubZ2Buffer[1] = 0;
	gubZ2Buffer[2] = 0;
	gubZ2Buffer[3] = 0;
	flafailFeatureOperation(MODE_FAIL_RETRY_SET_FEATURE, ubCE & gubPlanesPerBurstMask , gubParameterPage[179] , &gubZ2Buffer[0]);
#endif

}
#endif
void flaSetParamPageInfo(UBYTE ubMode)
{
	//#if(!RDT)
	if ((gulParamPageInfo.All != 0) || (BURNER && (gubCENumber == 0) )) {
		return ;
	}

	gubTemp = 0;

	if (ubMode) {
		// Read ONFI parameter page first
		flaReadParameterPage(0, (PARAMETER_PAGE | SET_PARAMETER_PAGE));
		// Then read JEDEC parameter page
		//flaReadParameterPage(0, JEDEC_PAGE);
		//memcpy(&gubParameterPage2[0], &BUFW_BASE[0], sizeof(gubParameterPage));
	}

	gulParamPageInfo.All = 0;
	if ( guoFlashUseType.B.btToshiba || (guoFlashDefaultType.B.btHynix && guoFlashUseType.B.bt8LC)) {
		if ((gubParameterPage[0] == 0x4A) && (gubParameterPage[1] == 0x45) && (gubParameterPage[2] == 0x53) && (gubParameterPage[3] == 0x44)) {
			gulParamPageInfo.JEDEC.uwToggleDDRSpeedGrade = gubParameterPage[146] | ( gubParameterPage[147] << 8);
		}
		else if ((gubParameterPage[0] == 0x54) && (gubParameterPage[1] == 0x53) && (gubParameterPage[2] == 0x42) && (gubParameterPage[3] == 0x4E) ) {
			//&&(gubParameterPage[172] == 0x4E)&&(gubParameterPage[173] == 0x61)&&(gubParameterPage[174] == 0x6E)&&(gubParameterPage[175] == 0x64)
			//&&(gubParameterPage[176] == 0x45)&&(gubParameterPage[177] == 0x6D)&&(gubParameterPage[178] == 0x75)){// 0~3 TSBN  172~175  Nand  176~178 Emu
			gulParamPageInfo.JEDEC.uwToggleDDRSpeedGrade = 0xEEEE;
			gulParamPageInfo.JEDEC.btNandSupplyByEmulator = 1;
			FCONL[FCONL_TMR_CTL] &= ~(TIMER_CPU_INTR_EN | TIMER_INTR_EN);
			if (gubParameterPage[179] == 1) {// 179 Enale No SLC Pool
				gulParamPageInfo.JEDEC.btNandEmulatorNoSLCPool = 1;
			}
		}
		else if ((gubParameterPage[0] == 0x48) && (gubParameterPage[1] == 0x59) && (gubParameterPage[2] == 0x4E) && (gubParameterPage[3] == 0x58) ) {
			//&&(gubParameterPage[172] == 0x4E)&&(gubParameterPage[173] == 0x61)&&(gubParameterPage[174] == 0x6E)&&(gubParameterPage[175] == 0x64)
			//&&(gubParameterPage[176] == 0x45)&&(gubParameterPage[177] == 0x6D)&&(gubParameterPage[178] == 0x75)){// 0~3 TSBN  172~175  Nand  176~178 Emu
			gulParamPageInfo.JEDEC.uwToggleDDRSpeedGrade = 0xEEEE;
			gulParamPageInfo.JEDEC.btNandSupplyByEmulator = 1;

			//if(gulParamPageInfo.JEDEC.btNandSupplyByEmulator){
			//	UartString("\nOK");
			//}

			FCONL[FCONL_TMR_CTL] &= ~(TIMER_CPU_INTR_EN | TIMER_INTR_EN);
			if (gubParameterPage[179] == 1) {// 179 Enale No SLC Pool
				gulParamPageInfo.JEDEC.btNandEmulatorNoSLCPool = 1;
			}
		}
		else {
			gulParamPageInfo.JEDEC.btNotSupport = 1;
		}
		if (guoFlashUseType.B.btToshibaBiCs2) {
			if ((gubParameterPage[44] == 0x54)/*T*/ && (gubParameterPage[45] == 0x48)/*H*/ && (gubParameterPage[46] == 0x35)/*5*/ && (gubParameterPage[47] == 0x38)/*8*/ && (gubParameterPage[48] == 0x54/*T*/)\
			        && (gubParameterPage[49] == 0x46)/*F*/ && (gubParameterPage[50] == 0x54)/*T*/ && ((gubParameterPage[51] == 0x31)/*1*/ || (gubParameterPage[51] == 0x30)/*0*/) && (gubParameterPage[52] == 0x56)/*V*/ && (gubParameterPage[53] == 0x32)/*2*/\
			        && (gubParameterPage[54] == 0x32)/*2*/ && (gubParameterPage[55] == 0x42)/*B*/ && (gubParameterPage[56] == 0x41)/*A*/ && (gubParameterPage[57] == 0x38)/*8*/ && ((gubParameterPage[58] == 0x48)/*H*/ || (gubParameterPage[58] == 0x43)/*C*/)) {
				guoFlashDefaultType.B.btEnterprise = TRUE;
				if (ENABLE_DEBUG_UART_TOSHIBA_ENTERPRISE) {
					UartString("\nToshibaBiCs2_btEnterprise");
				}
			}
		}
	}
	else {
		if ((gubParameterPage[0] == 0x4F) && (gubParameterPage[1] == 0x4E) && (gubParameterPage[2] == 0x46) && (gubParameterPage[3] == 0x49)) {
			// "ONFI"
			if ( gubParameterPage[6] & BIT5) {
				gulParamPageInfo.ONFI.btNVDDR = 1;
			}

			if ( gubParameterPage[7] & BIT2) {
				gulParamPageInfo.ONFI.btNVDDR2 = 1;
			}
			// removed get die number value from parameter page, due to the value may not be as same as spec described.
			// gubDieNumber = gubParameterPage[100];
			gulParamPageInfo.ONFI.ubAsynchronousTimingMode = gubParameterPage[129];
			gulParamPageInfo.ONFI.ubNVDDRTimingMode = gubParameterPage[141];
			gulParamPageInfo.ONFI.ubNVDDR2TimingMode = gubParameterPage[142];
			gulParamPageInfo.ONFI.btNVDDR2TimingMode8 = (gubParameterPage[162] & BIT0);
			/*
			switch (gubParameterPage[49]) {
				case 0x32:
					gubDensitysize = FLH_Size_256Gb;
					break;
				case 0x35:
					gubDensitysize = FLH_Size_512Gb;
					break;
				case 0x31:
					gubDensitysize = FLH_Size_1Tb;
					break;
				case 0x33:
					gubDensitysize = FLH_Size_384Gb;
					break;
				default:
					M_AssertError(1);
					break;
			}
			*/
			// Set Device Density Info for MP

			// In L95B-Plus ONFi Param Page, Byte250 (page-98, Table-15) has the "R" designator,
			// which corresponds to the MLC+ Part Numbering's "R" as shown in page-2 of the datasheet.
			// All MLC+ packaged parts have this designator.
		}
		else if ((gubParameterPage[0] == 0x4A) && (gubParameterPage[1] == 0x45) && (gubParameterPage[2] == 0x53) && (gubParameterPage[3] == 0x44)) {
			//gulParamPageInfo.JEDEC.btRandCacheRead = (gubParameterPage2[8] & BIT6);   // need to check
		}
#if MicronFlashOnly || YMTC_JGS
		else if ((gubParameterPage[0] == 0x49) && (gubParameterPage[1] == 0x4D) && (gubParameterPage[2] == 0x4E) && (gubParameterPage[3] == 0x45)) {

			//UartString("\n\r IMNE");
			FCONL[FCONL_TMR_CTL] &= ~(TIMER_CPU_INTR_EN | TIMER_INTR_EN);
			if (gubParameterPage[179] == 1) {// 179 Enale No SLC Pool
				gulParamPageInfo.ONFI.btNandEmulatorNoSLCPool = 1;
			}
			gulParamPageInfo.ONFI.btNandSupplyByEmulator = 1;
			// removed get die number value from parameter page, due to the value may not be as same as spec described.
			// gubDieNumber = gubParameterPage[100];
			gulParamPageInfo.ONFI.ubAsynchronousTimingMode = gubParameterPage[129];
			gulParamPageInfo.ONFI.ubNVDDRTimingMode = gubParameterPage[141];
			gulParamPageInfo.ONFI.ubNVDDR2TimingMode = gubParameterPage[142];
			gulParamPageInfo.ONFI.btNVDDR2TimingMode8 = (gubParameterPage[162] & BIT0);
			/*
			switch(gubParameterPage[49]){
				case 0x32:
					gubDensitysize = FLH_Size_256Gb;
					break;
				case 0x35:
					gubDensitysize = FLH_Size_512Gb;
					break;
				case 0x31:
					gubDensitysize = FLH_Size_1Tb;
					break;
				case 0x33:
					gubDensitysize = FLH_Size_384Gb;
				default:
					M_AssertError(1);
					break;
			}
			*/
		}
#endif
		else {
			gulParamPageInfo.ONFI.btNotSupport = 1;
		}
	}
	if (Hynix) {
		gulNandPhysicalPageSize = gub4kEntrysPerPlane * 4096 + 2048;
	}
	else {
		// data bytes per page
		gulNandPhysicalPageSize = gubParameterPage[80];
		gulNandPhysicalPageSize += gubParameterPage[81] << 8;
		gulNandPhysicalPageSize += gubParameterPage[82] << 16;
		gulNandPhysicalPageSize += gubParameterPage[83] << 24;

		// spare bytes per page
		gulNandPhysicalPageSize += gubParameterPage[84];
		gulNandPhysicalPageSize += gubParameterPage[85] << 8;

		if (gulNandPhysicalPageSize == 0) { //if gubParameterPage doesn't contain the PhysicalPageSize, set the fix value
			if (MicronFlashOnly) {
				if (B0KB) {
					gulNandPhysicalPageSize = 18592;
				}
				else {
					gulNandPhysicalPageSize = 18592;
				}
			}
			else {
				if (TLC || TLC_BICS2) {
					gulNandPhysicalPageSize = 18336;
				}
				else {
					if (guoFlashDefaultType.B.btToshiba1Znm) {
						if (guoFlashDefaultType.B.btFourPlane) {
							gulNandPhysicalPageSize = 17760; //Toshiba MLC 4plane 15nm
						}
						else {
							gulNandPhysicalPageSize = 17664; //Toshiba MLC 2plane 15nm
						}
					}
					else {
						gulNandPhysicalPageSize = 17664;
					}
				}
			}
		}
	}
	//#endif
}

UWORD flaCalculateCRC (U32 ulSrcAdr, U32 ulLen)
{

	UWORD uwCRC = 0x4F4E;
	UWORD uwValue, uwj, uwBit;
	U32 ulIndex;
	for (ulIndex = 0; ulIndex < ulLen; ulIndex++) {
		uwValue = ((UBYTE*)ulSrcAdr)[ulIndex];
		for (uwj = 0x80; uwj; uwj >>= 1) {
			uwBit = uwCRC & BIT15;

			uwCRC <<= 1;

			if (uwValue & uwj) {
				uwBit ^= BIT15;
			}

			if (uwBit) {
				uwCRC ^= 0x8005;
			}
		}
	}
	return uwCRC;
}


void flaGetFeature(UBYTE ubChannel, UBYTE ubAddr , UBYTE *ubGetFeatureData)
{
	UBYTE ubi;
	UWORD uwCount = 0;
	UWORD uwPollingCount = 40000;
	UBYTE ubPollingFail = 0;

	gFREG = (volatile REG_t*)gulFlashIP_RegBase[ubChannel];
#if Hynix
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x78;
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
#endif
	//----- Get feature -----//
	gFREG[FCTLL_PIO_CMD].B.B0 = 0xEE;
	gFREG[FCTLL_PIO_ADR].B.B0 = ubAddr;

#if (!(MicronFlashOnly || YMTC_JGS))
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
#endif

	do {
#if MicronFlashOnly || YMTC_JGS
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
#endif
		uwCount++;
		if (uwCount > uwPollingCount) {
			break;
		}
	}
	while (gFREG[FCTLL_PIO_DAT].B.B0 != 0xE0);


	if (ubPollingFail == 0) {
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;
		if (!Hynix) {
			gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST); // set to default value
			for (ubi = 0; ubi < 4; ubi++) {
				ubGetFeatureData[ubChannel * 4 + ubi] = gFREG[FCTLL_PIO_DAT].B.B0;
			}
		}
		else {
			gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First
			for (ubi = 0; ubi < 4; ubi++) {
				if (ubi == 3) {
					gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;    //Last
				}

				ubGetFeatureData[ubChannel * 4 + ubi] = gFREG[FCTLL_PIO_DAT].B.B0;

				if (ubi == 0) {
					gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST));
				}
			}
			gFREG[FCTLL_HS_MODE].L |= PIO_DAT_FIRST | PIO_DAT_LAST; // set to default value
		}
	}
	else {
		for (ubi = 0; ubi < 4; ubi++) {
			ubGetFeatureData[ubChannel * 4 + ubi] = 0xAA;
		}
	}
}

void flaSetFeature(UBYTE ubChannel , UBYTE ubAddr , UBYTE *ubSetFeatureData)
{
	UBYTE ubi;
	UWORD uwCount = 0;
	UWORD uwPollingCount = 40000;

	gFREG = (volatile REG_t*)gulFlashIP_RegBase[ubChannel];
#if Hynix
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x78;
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
	gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
#endif
	//----- Set feature -----//
	gFREG[FCTLL_PIO_CMD].B.B0 = 0xEF;
	gFREG[FCTLL_PIO_ADR].B.B0 = ubAddr;

	gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First

	if ((gFREG[FCTLL_FLH_SET].B.B0 & FLA_ONFI_MODE) == FLA_ONFI_MODE) {
		gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST); // set to default value
		for (ubi = 0; ubi < 4; ubi++) {
			gFREG[FCTLL_PIO_DAT].B.B0 = ubSetFeatureData[ubi];
		}
	}
	else {
		gFREG[FCTLL_HS_MODE].L &= (~PIO_DAT_LAST); //First
		for (ubi = 0; ubi < 4; ubi++) {
			if (ubi == 3) {
				gFREG[FCTLL_HS_MODE].L |= PIO_DAT_LAST;    //Last
			}

			gFREG[FCTLL_PIO_DAT].B.B0 = ubSetFeatureData[ubi];

			if (ubi == 0) {
				gFREG[FCTLL_HS_MODE].L &= (~(PIO_DAT_FIRST | PIO_DAT_LAST));
			}
		}
		gFREG[FCTLL_HS_MODE].L |= PIO_DAT_FIRST | PIO_DAT_LAST; // set to default value
	}

#if (!(MicronFlashOnly || YMTC_JGS))
	gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
#endif

	do {
#if MicronFlashOnly || YMTC_JGS
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
#endif
		uwCount++;
		if (uwCount > uwPollingCount) {
			break;
		}
	}
	while (gFREG[FCTLL_PIO_DAT].B.B0 != 0xE0);
}

void flaReset(UBYTE ubChannel, UBYTE ubResetCmd)
{
	_UCV REG_t * pFlaReg;
	UWORD uwCount = 0;
	UWORD uwPollingCount = 40000;

	pFlaReg = (_UCV REG_t *)gulFlashIP_RegBase[ubChannel];
	//----- Reset -----//
	pFlaReg[FCTLL_PIO_CMD].B.B0 = ubResetCmd;			//Send 0xff

#if (!(MicronFlashOnly || YMTC_JGS))
	pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
#endif

	do {
#if MicronFlashOnly || YMTC_JGS
		pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;			//Send 0x70 to check status
#endif
		uwCount++;
		if (uwCount > uwPollingCount) {
			break;
		}
	}
	while (pFlaReg[FCTLL_PIO_DAT].B.B0 != 0xE0);

}

void flaSetOverDrive(void)
{
	UBYTE ubFeature[MAX_CHANNEL][4] = {{0}}, ubFeatureCheck[MAX_CHANNEL][4] = {{0}};
	UBYTE ubi, ubj, ubk;
	UBYTE ubChannel;
	UBYTE ubAddr;
	U32 ulChannelMap = 0;
	UBYTE ubCEPerChannel;
	U32 ulFlashMode_FailMap = 0;
#if YMTC
	UBYTE reset_cmd;
	if (guoFlashUseType.B.btNVDDR2 || guoFlashUseType.B.btNVDDR) {
		reset_cmd = 0xFC;
	}
	else {
		reset_cmd = 0xFF;
	}
#elif (Hynix || MicronFlashOnly)
	UBYTE reset_cmd;


	if (Hynix && guoFlashUseType.B.btToggle) {
		reset_cmd = 0xFC;
	}
	else if (MicronFlashOnly && (guoFlashUseType.B.btNVDDR2 || guoFlashUseType.B.btNVDDR)) {
		reset_cmd = 0xFC;
	}
	else {
		reset_cmd = 0xFF;
	}
#endif


	ubCEPerChannel = (gubCENumber >> gubPlanesPerBurstLog);
	if (ENABLE_SANDISK_1ZNM_SPF) {
		ubCEPerChannel = ((gubCENumber + gubHideCE * gubPlanesPerBurst) >> gubPlanesPerBurstLog);
	}

	if (ENABLE_PSEUDO_CE) {
		ubCEPerChannel /= gubExpandCE_Ratio;
	}

	for ( ubi = 0; ubi < gubPlanesPerBurst ; ubi++) {
		ulChannelMap |= (BIT0 << (ubi * 8));
	}

	ubAddr = 0x10;

	for (ubi = 0; ubi < ubCEPerChannel; ubi++) {
		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nChannel Map ");
			UartLongHex(ulChannelMap << ubi);
		}
#if(UFS)
		if (1)
#else
		if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
		{
			FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(0, gubCH_CEMappingToPCE[0][ubi]);
			FCONL[FCONL_FCE_ENB] |= M_MapToCEDecoder(1, gubCH_CEMappingToPCE[1][ubi]);
		}
		else {
			FCONL[FCONL_FCE_ENB] = (ulChannelMap << ubi);
		}

		//----- Get Feature -----//
		//ToriTani said: relevant to flash interface, cannot not set 2 channel one time, so need to set channel separately.
		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
			flaSetFlashInterface(ubChannel, ubi);
			flaGetFeature(ubChannel, ubAddr, &ubFeature[0][0]);
#if (Hynix || MicronFlashOnly || YMTC)
			flaReset(ubChannel, reset_cmd);
#else
			flaReset(ubChannel, 0xFF);
#endif
		}

		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nOGF");
			for (ubj = 0; ubj < gubPlanesPerBurst; ubj++) {
				UartString("\r\n");
				for (ubk = 0; ubk < 4; ubk++) {
					UartString(" ");
					Uart_Tx_DataHex(ubFeature[ubj][ubk]);
				}
			}
		}
		//----- Set feature -----//
		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
			ubFeature[ubChannel][0] = gubFlashDrive;
#if (Hynix || MicronFlashOnly || YMTC)
			flaSetFeature(ubChannel, ubAddr, &ubFeature[ubChannel][0]);
			flaReset(ubChannel, reset_cmd);
#else
			flaSetFeature(ubChannel, ubAddr, &ubFeature[ubChannel][0]);
			flaReset(ubChannel, 0xFF);
#endif
		}

		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nOSF");
			for (ubj = 0; ubj < gubPlanesPerBurst; ubj++) {
				UartString("\r\n");
				for (ubk = 0; ubk < 4; ubk++) {
					UartString(" ");
					Uart_Tx_DataHex(ubFeature[ubj][ubk]);
				}
			}
		}
		//----- Get Feature Check -----//
		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
			flaGetFeature(ubChannel, ubAddr, &ubFeatureCheck[0][0]);
#if (Hynix || MicronFlashOnly || YMTC)
			flaReset(ubChannel, reset_cmd);
#else
			flaReset(ubChannel, 0xFF);
#endif
		}

		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nOGF2");
			for (ubj = 0; ubj < gubPlanesPerBurst; ubj++) {
				UartString("\r\n");
				for (ubk = 0; ubk < 4; ubk++) {
					UartString(" ");
					Uart_Tx_DataHex(ubFeatureCheck[ubj][ubk]);
				}
			}
		}

		for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
			UBYTE ubCEIndex = (ubChannel * gubMAX_SCAN_CE_PER_CH + ubi); //F_CE
			ubCEIndex = (ubCEIndex / gubMAX_SCAN_CE_PER_CH) + (ubCEIndex % gubMAX_SCAN_CE_PER_CH) * gubPlanesPerBurst; //F_CE轉成Logical CE
			ubCEIndex = TranslateLogicalCEtoPhysicalCE(ubCEIndex);//最後轉成physical CE
			if (ubFeatureCheck[ubChannel][0] != gubFlashDrive) {
				ulFlashMode_FailMap |= (BIT0 << ubCEIndex);
			}
		}
	}

	if (ulFlashMode_FailMap) {
		SetInitialFailID(BIT2, ulFlashMode_FailMap);
	}
#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		M_DisableAllFlashCE();
	}

}


void flaSetFlashPadControl(UBYTE ubODT_Value)
{
	UBYTE ubDiePerChannel = gubDieNumber * (gubCENumber / gubPlanesPerBurst);
	if (gDrivingSetting.ubDrivingEnable) {
		SYS1L[SYS1L_PAD_FCLE0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FCLE1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FALE0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FALE1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FWEB0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FWEB1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FRDY_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FCEB_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FWPB_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FREB0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FREB1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FDQS0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FDQS1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FDATA_DVSR0] &= ~(BIT6 | BIT7 | BIT14 | BIT15 | BIT22 | BIT23 | BIT30 | BIT31);
		SYS1L[SYS1L_PAD_FDATA_DVSR1] &= ~(BIT6 | BIT7 | BIT14 | BIT15 | BIT22 | BIT23 | BIT30 | BIT31);
		SYS1L[SYS1L_PAD_FDATA_DVSR2] &= ~(BIT6 | BIT7 | BIT14 | BIT15 | BIT22 | BIT23 | BIT30 | BIT31);
		SYS1L[SYS1L_PAD_FDATA_DVSR3] &= ~(BIT6 | BIT7 | BIT14 | BIT15 | BIT22 | BIT23 | BIT30 | BIT31);

		SYS1L[SYS1L_PAD_FCLE0_CFG] |= (gDrivingSetting.ubCLEPads << 14);
		SYS1L[SYS1L_PAD_FCLE1_CFG] |= (gDrivingSetting.ubCLEPads << 14);
		SYS1L[SYS1L_PAD_FALE0_CFG] |= (gDrivingSetting.ubALEPads << 14);
		SYS1L[SYS1L_PAD_FALE1_CFG] |= (gDrivingSetting.ubALEPads << 14);
		SYS1L[SYS1L_PAD_FWEB0_CFG] |= (gDrivingSetting.ubnWEPads << 14);
		SYS1L[SYS1L_PAD_FWEB1_CFG] |= (gDrivingSetting.ubnWEPads << 14);
		SYS1L[SYS1L_PAD_FRDY_CFG] |= (gDrivingSetting.ubFRDYPads << 14);
		SYS1L[SYS1L_PAD_FCEB_CFG] |= (gDrivingSetting.ubnCEPads << 14);
		SYS1L[SYS1L_PAD_FWPB_CFG] |= (gDrivingSetting.ubnWPPads << 14);
		SYS1L[SYS1L_PAD_FREB0_CFG] |= (gDrivingSetting.ubnREPads << 14);
		SYS1L[SYS1L_PAD_FREB1_CFG] |= (gDrivingSetting.ubnREPads << 14);
		SYS1L[SYS1L_PAD_FDQS0_CFG] |= (gDrivingSetting.ubDQSPads << 14);
		SYS1L[SYS1L_PAD_FDQS1_CFG] |= (gDrivingSetting.ubDQSPads << 14);
		SYS1L[SYS1L_PAD_FDATA_DVSR0] |= (gDrivingSetting.ubDataPads << 6) | (gDrivingSetting.ubDataPads << 14) | (gDrivingSetting.ubDataPads << 22) | (gDrivingSetting.ubDataPads << 30);
		SYS1L[SYS1L_PAD_FDATA_DVSR1] |= (gDrivingSetting.ubDataPads << 6) | (gDrivingSetting.ubDataPads << 14) | (gDrivingSetting.ubDataPads << 22) | (gDrivingSetting.ubDataPads << 30);
		SYS1L[SYS1L_PAD_FDATA_DVSR2] |= (gDrivingSetting.ubDataPads << 6) | (gDrivingSetting.ubDataPads << 14) | (gDrivingSetting.ubDataPads << 22) | (gDrivingSetting.ubDataPads << 30);
		SYS1L[SYS1L_PAD_FDATA_DVSR3] |= (gDrivingSetting.ubDataPads << 6) | (gDrivingSetting.ubDataPads << 14) | (gDrivingSetting.ubDataPads << 22) | (gDrivingSetting.ubDataPads << 30);

	}
	else {

		UBYTE ubPadDriveValue;
		/***********************/
		/*Fla_PAD_Drive_Setting*/
		/***********************/
		if ((ubDiePerChannel >= 8) ||	 //Zout == 18ohm
		        ((ubDiePerChannel >= 4) && (gubEnableOverClocking == 2))) {
			ubPadDriveValue = 3;
		}
		else if (ubDiePerChannel >= 4) { //Zout == 25ohm
			ubPadDriveValue = 2;
		}
		else {  //Zout == 35ohm
			ubPadDriveValue = 1;
		}

		SYS1L[SYS1L_PAD_FCLE0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FCLE1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FALE0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FALE1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FWEB0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FWEB1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FREB0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FREB1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FDQS0_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FDQS1_CFG] &= ~(BIT14 | BIT15);
		SYS1L[SYS1L_PAD_FDATA_DVSR0] &= ~(BIT6 | BIT7 | BIT14 | BIT15 | BIT22 | BIT23 | BIT30 | BIT31);
		SYS1L[SYS1L_PAD_FDATA_DVSR1] &= ~(BIT6 | BIT7 | BIT14 | BIT15 | BIT22 | BIT23 | BIT30 | BIT31);
		SYS1L[SYS1L_PAD_FDATA_DVSR2] &= ~(BIT6 | BIT7 | BIT14 | BIT15 | BIT22 | BIT23 | BIT30 | BIT31);
		SYS1L[SYS1L_PAD_FDATA_DVSR3] &= ~(BIT6 | BIT7 | BIT14 | BIT15 | BIT22 | BIT23 | BIT30 | BIT31);

		SYS1L[SYS1L_PAD_FCLE0_CFG] |= (ubPadDriveValue << 14);
		SYS1L[SYS1L_PAD_FCLE1_CFG] |= (ubPadDriveValue << 14);
		SYS1L[SYS1L_PAD_FALE0_CFG] |= (ubPadDriveValue << 14);
		SYS1L[SYS1L_PAD_FALE1_CFG] |= (ubPadDriveValue << 14);

#if HV
		UBYTE ubNEWPadWEBDriveValue = ubPadDriveValue;
		// 35 ohm:
		//	1. TSB - 15nm MLC / TLC / BiCs2
		//	2. Hynix - H14 / H16
		// 25 ohm:
		//	1. Sandisk - 15nm TLC
		//	2. Micron - L06 / B0K

		if (guoFlashUseType.B.btToshiba || guoFlashUseType.B.btHynix) {
			ubNEWPadWEBDriveValue = 1; //Zout == 35ohm
		}
		else if (guoFlashUseType.B.btSandisk || guoFlashUseType.B.btIntelMicron) {
			ubNEWPadWEBDriveValue = 2; //Zout == 25ohm
		}
		SYS1L[SYS1L_PAD_FWEB0_CFG] |= (ubNEWPadWEBDriveValue << 14);
		SYS1L[SYS1L_PAD_FWEB1_CFG] |= (ubNEWPadWEBDriveValue << 14);
#else
		SYS1L[SYS1L_PAD_FWEB0_CFG] |= (ubPadDriveValue << 14);
		SYS1L[SYS1L_PAD_FWEB1_CFG] |= (ubPadDriveValue << 14);
#endif
		SYS1L[SYS1L_PAD_FREB0_CFG] |= (ubPadDriveValue << 14);
		SYS1L[SYS1L_PAD_FREB1_CFG] |= (ubPadDriveValue << 14);
		SYS1L[SYS1L_PAD_FDQS0_CFG] |= (ubPadDriveValue << 14);
		SYS1L[SYS1L_PAD_FDQS1_CFG] |= (ubPadDriveValue << 14);
		SYS1L[SYS1L_PAD_FDATA_DVSR0] |= (ubPadDriveValue << 6) | (ubPadDriveValue << 14) | (ubPadDriveValue << 22) | (ubPadDriveValue << 30);
		SYS1L[SYS1L_PAD_FDATA_DVSR1] |= (ubPadDriveValue << 6) | (ubPadDriveValue << 14) | (ubPadDriveValue << 22) | (ubPadDriveValue << 30);
		SYS1L[SYS1L_PAD_FDATA_DVSR2] |= (ubPadDriveValue << 6) | (ubPadDriveValue << 14) | (ubPadDriveValue << 22) | (ubPadDriveValue << 30);
		SYS1L[SYS1L_PAD_FDATA_DVSR3] |= (ubPadDriveValue << 6) | (ubPadDriveValue << 14) | (ubPadDriveValue << 22) | (ubPadDriveValue << 30);

		gDrivingSetting.ubALEPads = ubPadDriveValue;
		gDrivingSetting.ubCLEPads = ubPadDriveValue;
#if HV
		gDrivingSetting.ubnWEPads = ubNEWPadWEBDriveValue;
#else
		gDrivingSetting.ubnWEPads = ubPadDriveValue;
#endif
		gDrivingSetting.ubnREPads = ubPadDriveValue;
		gDrivingSetting.ubFRDYPads = ubPadDriveValue;
		gDrivingSetting.ubnWPPads = ubPadDriveValue;
		gDrivingSetting.ubDQSPads = ubPadDriveValue;
		gDrivingSetting.ubDataPads = ubPadDriveValue;
		gDrivingSetting.ubnCEPads = ubPadDriveValue;
	}
	/*********************/
	/*Fla_PAD_ODT_Setting*/
	/*********************/
	//00 : 150 ohm
	//01 : 100 ohm
	//10 : 75 ohm
	//11 : 50 ohm

	SYS1L[SYS1L_PAD_FDQS0_CFG] &= ~(BIT28 | BIT29);
	SYS1L[SYS1L_PAD_FDQS1_CFG] &= ~(BIT28 | BIT29);
	SYS1L[SYS1L_PAD_FDAT0_CFG] &= ~(BIT28 | BIT29);
	SYS1L[SYS1L_PAD_FDAT1_CFG] &= ~(BIT28 | BIT29);
	FALLL[FCTLL_DYNAMIC_ODT] &= ~DYNAMIC_ODT_EN;

	if (((ubDiePerChannel >= 16) && ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz)))
	        || ((ubDiePerChannel >= 8) && (gubFlashClock == FLH_CLK_266MHz))
	        || (guoFlashDefaultType.B.bt8LC && ((ubDiePerChannel >= 8) && ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz))))
	        || gODTSetting.ubODTEnable) {

		if (gODTSetting.ubODTEnable == 2) {
			gODTSetting.ubODT = 0xFF;
			return;
		}
		FALLL[FCTLL_DYNAMIC_ODT] |= DYNAMIC_ODT_EN; //SET BIT0 of FCTLL_DYNAMIC_ODT 時才會開啟ODT(FlashIP自動決定什麼時候開啟 例如ToggleRead時)
		if (gODTSetting.ubODTEnable) {
			SYS1L[SYS1L_PAD_FDQS0_CFG] |= SET_ODT_VALUE(gODTSetting.ubODT);
			SYS1L[SYS1L_PAD_FDQS1_CFG] |= SET_ODT_VALUE(gODTSetting.ubODT);
			SYS1L[SYS1L_PAD_FDAT0_CFG] |= SET_ODT_VALUE(gODTSetting.ubODT);
			SYS1L[SYS1L_PAD_FDAT1_CFG] |= SET_ODT_VALUE(gODTSetting.ubODT);
		}
		else {
			SYS1L[SYS1L_PAD_FDQS0_CFG] |= SET_ODT_VALUE(ubODT_Value);
			SYS1L[SYS1L_PAD_FDQS1_CFG] |= SET_ODT_VALUE(ubODT_Value);
			SYS1L[SYS1L_PAD_FDAT0_CFG] |= SET_ODT_VALUE(ubODT_Value);
			SYS1L[SYS1L_PAD_FDAT1_CFG] |= SET_ODT_VALUE(ubODT_Value);
			gODTSetting.ubODT = ubODT_Value;
		}
	}
	else if (ubDiePerChannel >= 32) {
		FALLL[FCTLL_DYNAMIC_ODT] |= DYNAMIC_ODT_EN; //SET BIT0 of FCTLL_DYNAMIC_ODT 時才會開啟ODT(FlashIP自動決定什麼時候開啟 例如ToggleRead時)
		SYS1L[SYS1L_PAD_FDQS0_CFG] |= SET_ODT_VALUE(SET_ODT_50ohm);
		SYS1L[SYS1L_PAD_FDQS1_CFG] |= SET_ODT_VALUE(SET_ODT_50ohm);
		SYS1L[SYS1L_PAD_FDAT0_CFG] |= SET_ODT_VALUE(SET_ODT_50ohm);
		SYS1L[SYS1L_PAD_FDAT1_CFG] |= SET_ODT_VALUE(SET_ODT_50ohm);
		gODTSetting.ubODT = SET_ODT_50ohm;
	}
	else {
		gODTSetting.ubODT = 0xFF;
	}
}

void flaSetFlashPadControl_Default()
{
	SYS1L[SYS1L_PAD_FRDY_CFG] = 0x02804020;
	SYS1L[SYS1L_PAD_FWPB_CFG] = 0x01804041;
	SYS1L[SYS1L_PAD_FCEB_CFG] = 0x00804011;//Thomas said bit6 should not be set.
#if MicronFlashOnly  //Pull-Up FDAT to reduce leakage
	SYS1L[SYS1L_PAD_FDAT0_CFG] = 0x00000011;
	SYS1L[SYS1L_PAD_FDAT1_CFG] = 0x00000011;
#else
	SYS1L[SYS1L_PAD_FDAT0_CFG] = 0x00000041;
	SYS1L[SYS1L_PAD_FDAT1_CFG] = 0x00000041;
#endif

	//Zout == 25ohm                                                 //Ray said 25ohm is ok.
	SYS1L[SYS1L_PAD_FCLE0_CFG] = 0x00F08041;
	SYS1L[SYS1L_PAD_FCLE1_CFG] = 0x00F08041;
	SYS1L[SYS1L_PAD_FALE0_CFG] = 0x00F08041;
	SYS1L[SYS1L_PAD_FALE1_CFG] = 0x00F08041;

#if HV
	SYS1L[SYS1L_PAD_FWEB0_CFG] = 0x02F04011; // 35ohm
	SYS1L[SYS1L_PAD_FWEB1_CFG] = 0x02F04011; // 35ohm
#else
	SYS1L[SYS1L_PAD_FWEB0_CFG] = 0x02F08011;
	SYS1L[SYS1L_PAD_FWEB1_CFG] = 0x02F08011;
#endif
	SYS1L[SYS1L_PAD_FREB0_CFG] = 0x02F08011;
	SYS1L[SYS1L_PAD_FREB1_CFG] = 0x02F08011;
	SYS1L[SYS1L_PAD_FDQS0_CFG] = 0x00FF8041;
	SYS1L[SYS1L_PAD_FDQS1_CFG] = 0x00FF8041;
	SYS1L[SYS1L_PAD_FDATA_DVSR0] = 0x8F8F8F8F;
	SYS1L[SYS1L_PAD_FDATA_DVSR1] = 0x8F8F8F8F;
	SYS1L[SYS1L_PAD_FDATA_DVSR2] = 0x8F8F8F8F;
	SYS1L[SYS1L_PAD_FDATA_DVSR3] = 0x8F8F8F8F;
}

void fla_Set_Offset_DQS_Delay(UBYTE ubFlashClock)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	if ((gubFlashClock == gubFlashClock_Scan) && (gubFlashMode == gubFlashMode_Scan)) {
		//tuning offset
		//1st DLL Contorl
		UWORD uwOffset;
		UWORD uwHWTrackingResult;
		UWORD uwSafeArea = 25;

		uwHWTrackingResult = (UWORD)((FDLLL[FDLLL_SDLL_VAL_0] & 0x000001FF));
		uwOffset = gSDLL_OFFSET_Setting.uwReadSDLLOffsetCH0;
		if (uwOffset & BIT15) { //minus
			if (uwHWTrackingResult > ((uwOffset & 0x01FF) + uwSafeArea)) {
				uwOffset = 0x200 - (uwOffset & 0x01FF);
				flaDLLCDCIssueWorkaround(FDLLL_SDLL_OFFSET_0, SDLL_DLY_OFFSET_RD_CH0(uwOffset));
			}
		}
		else { //plus
			if ((uwHWTrackingResult + (uwOffset & 0x01FF)) < (512 - uwSafeArea)) {
				uwOffset = (uwOffset & 0x01FF);
				flaDLLCDCIssueWorkaround(FDLLL_SDLL_OFFSET_0, SDLL_DLY_OFFSET_RD_CH0(uwOffset));
			}
		}

		uwHWTrackingResult = (UWORD)((FDLLL[FDLLL_SDLL_VAL_1] & 0x000001FF));
		uwOffset = gSDLL_OFFSET_Setting.uwReadSDLLOffsetCH1;
		if (uwOffset & BIT15) {
			if (uwHWTrackingResult > ((uwOffset & 0x01FF) + uwSafeArea)) {
				uwOffset = 0x200 - (uwOffset & 0x01FF);
				flaDLLCDCIssueWorkaround(FDLLL_SDLL_OFFSET_1, SDLL_DLY_OFFSET_RD_CH1(uwOffset));
			}
		}
		else {
			if ((uwHWTrackingResult + (uwOffset & 0x01FF)) < (512 - uwSafeArea)) {
				uwOffset = (uwOffset & 0x01FF);
				flaDLLCDCIssueWorkaround(FDLLL_SDLL_OFFSET_1, SDLL_DLY_OFFSET_RD_CH1(uwOffset));
			}
		}
		flaDLLCDCIssueWorkaround(FDLLL_LOAD_CTL, (FDLLL[FDLLL_LOAD_CTL] | (0xFF << 16)));	// FW load MDLL value
		//2nd DLL Contorl

		uwHWTrackingResult = (UWORD)((FDLLL[FDLLL_SDLL_VAL_0 + (0x200 >> 2)] & 0x01FF0000) >> 16);
		uwOffset = gSDLL_OFFSET_Setting.uwProgramSDLLOffsetCH0;
		if (uwOffset & BIT15) {
			if (uwHWTrackingResult > ((uwOffset & 0x01FF) + uwSafeArea)) {
				uwOffset = 0x200 - (uwOffset & 0x01FF);
				flaDLLCDCIssueWorkaround(FDLLL_SDLL_OFFSET_0 + (0x200 >> 2), SDLL_DLY_OFFSET_WR_CH0(uwOffset));
			}
		}
		else {
			if ((uwHWTrackingResult + (uwOffset & 0x01FF)) < (512 - uwSafeArea)) {
				uwOffset = (uwOffset & 0x01FF);
				flaDLLCDCIssueWorkaround(FDLLL_SDLL_OFFSET_0 + (0x200 >> 2), SDLL_DLY_OFFSET_WR_CH0(uwOffset));
			}
		}

		uwHWTrackingResult = (UWORD)((FDLLL[FDLLL_SDLL_VAL_1 + (0x200 >> 2)] & 0x01FF0000) >> 16);
		uwOffset = gSDLL_OFFSET_Setting.uwProgramSDLLOffsetCH1;
		if (uwOffset & BIT15) {
			if (uwHWTrackingResult > ((uwOffset & 0x01FF) + uwSafeArea)) {
				uwOffset = 0x200 - (uwOffset & 0x01FF);
				flaDLLCDCIssueWorkaround(FDLLL_SDLL_OFFSET_1 + (0x200 >> 2), SDLL_DLY_OFFSET_WR_CH1(uwOffset));
			}
		}
		else {
			if ((uwHWTrackingResult + (uwOffset & 0x01FF)) < (512 - uwSafeArea)) {
				uwOffset = (uwOffset & 0x01FF);
				flaDLLCDCIssueWorkaround(FDLLL_SDLL_OFFSET_1 + (0x200 >> 2), SDLL_DLY_OFFSET_WR_CH1(uwOffset));
			}
		}
		flaDLLCDCIssueWorkaround(FDLLL_LOAD_CTL + (0x200 >> 2), (FDLLL[FDLLL_LOAD_CTL + (0x200 >> 2)] | (0xFF << 16))); // FW load MDLL value
	}

}

void flaDLLTracking(UBYTE ubProgramTracking, UBYTE ubAutoMode, U32 ulSelVal)//void FlaDLLTracking(UBYTE ubChannel)
{
	U32 ulOffset;//DLL_Controller_Index_Offset;
	ulOffset = (ubProgramTracking ? 0x0200 : 0x0000);

	//Every time when Flash Clock is changed, this function should be called once.
	//This function adopts "Load with DM Trigger"
	// Enable DLL auto load
	flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL + (ulOffset >> 2), (FDLLL[FDLLL_DLL_CTL + (ulOffset >> 2)] & (~MDLL_RSTJ_NORMAL))); // reset MDLL
	flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL + (ulOffset >> 2), (FDLLL[FDLLL_DLL_CTL + (ulOffset >> 2)] | (MDLL_RSTJ_NORMAL | MDLL_EN)));
	flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL + (ulOffset >> 2), (FDLLL[FDLLL_DLL_CTL + (ulOffset >> 2)] & FLH_STROBE_MODE));
	flaDLLCDCIssueWorkaround(FDLLL_LOAD_CTL + (ulOffset >> 2), (FDLLL[FDLLL_LOAD_CTL + (ulOffset >> 2)] | MDLL_AVG_REG_LOAD_MDLL)); // stop average function

	while (!(FDLLL[FDLLL_MDLL_STA + (ulOffset >> 2)] & MDLL_LOCK)) {}

	if (FDLLL[FDLLL_MDLL_STA + (ulOffset >> 2)] & MDLL_FAIL) {
		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nMDLL_FAIL");
		}

		if (ubAutoMode) {
			if (FDLLL[FDLLL_MDLL_STA + (ulOffset >> 2)] & MDLL_UNDERFLOW) {
				flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_0 + (ulOffset >> 2), (ubProgramTracking ? (SDLL_VALUE_WRITE_CH0(1)) : (SDLL_VALUE_READ_CH0(1)))); // Set minimum
				flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_1 + (ulOffset >> 2), (ubProgramTracking ? (SDLL_VALUE_WRITE_CH1(1)) : (SDLL_VALUE_READ_CH1(1))));
			}
			else { //Overflow
				flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_0 + (ulOffset >> 2), (FDLLL[FDLLL_SEL_VAL_0 + (ulOffset >> 2)] | (ubProgramTracking ? (SDLL_VALUE_WRITE_CH0(0X1FF)) : (SDLL_VALUE_READ_CH0(0X1FF))))); // Set maximum
				flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_1 + (ulOffset >> 2), (FDLLL[FDLLL_SEL_VAL_1 + (ulOffset >> 2)] | (ubProgramTracking ? (SDLL_VALUE_WRITE_CH1(0X1FF)) : (SDLL_VALUE_READ_CH1(0X1FF)))));
			}
		}
		else {
			flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_0 + (ulOffset >> 2), (ubProgramTracking ? (SDLL_VALUE_WRITE_CH0(ulSelVal)) : (SDLL_VALUE_READ_CH0(ulSelVal)))); // Set minimum
			flaDLLCDCIssueWorkaround(FDLLL_SEL_VAL_1 + (ulOffset >> 2), (ubProgramTracking ? (SDLL_VALUE_WRITE_CH1(ulSelVal)) : (SDLL_VALUE_READ_CH1(ulSelVal))));
		}

		flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL + (ulOffset >> 2), (FDLLL[FDLLL_DLL_CTL + (ulOffset >> 2)] | FW_CTL_SDLL_MODE));
		flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL + (ulOffset >> 2), (FDLLL[FDLLL_DLL_CTL + (ulOffset >> 2)] & (~MDLL_EN)));
		//Dump(Uart_Printf("f_MDLL \r\n"));

	}
	else {

		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nMDLL_OK");
		}

		flaDLLCDCIssueWorkaround(FDLLL_LOAD_CTL + (ulOffset >> 2), (FDLLL[FDLLL_LOAD_CTL + (ulOffset >> 2)] | (0xFF << 16))); // FW load MDLL value
		flaDLLCDCIssueWorkaround(FDLLL_LOAD_CTL + (ulOffset >> 2), (FDLLL[FDLLL_LOAD_CTL + (ulOffset >> 2)] & (~MDLL_AVG_REG_LOAD_MDLL)));	// start average function
		flaDLLCDCIssueWorkaround(FDLLL_LOAD_CTL + (ulOffset >> 2), (FDLLL[FDLLL_LOAD_CTL + (ulOffset >> 2)] | LOAD_WITH_DMA_TRIG_EN));	// start load with DMA function
	}
}
void flaSwitchClock(UBYTE ubFlashClock)
{
	UWORD uwCR_FW;
	UBYTE ubFlhClkDiv = 0xF;
	UBYTE ubFlhClkDivEDO = 0xF;//Bics and Sandisk support the Legacy mode speed up to 50MHz
	UBYTE ubNoFWRoutine = 0;
	gubDLLFlashClock = ubFlashClock; //record switch clock for dll workaround

	switch (ubFlashClock) {
		case FLH_CLK_10MHz:
			uwCR_FW = 0x3000;
			ubFlhClkDiv = 0x02; //should div 4, but advise div 8
			break;

		case FLH_CLK_40MHz:
			uwCR_FW = 0x3000;
			ubFlhClkDivEDO = 0x0F;
			break;

		case FLH_CLK_50MHz:
			ubNoFWRoutine = 1;
			ubFlhClkDivEDO = 0x0F;
			break;
		case FLH_CLK_83MHz:
			ubNoFWRoutine = 1;
			ubFlhClkDivEDO = 0;
			break;

		case FLH_CLK_100MHz:
			uwCR_FW = 0x3300;	//Toggle-200
			ubFlhClkDivEDO = 0;
			break;

		case FLH_CLK_150MHz:
		case FLH_CLK_166MHz:
		case FLH_CLK_180MHz:
			ubNoFWRoutine = 1;
			ubFlhClkDivEDO = 0x02;
			break;

		case FLH_CLK_200MHz:
			uwCR_FW = 0x3200;//Toggle2-400
			ubFlhClkDivEDO = 0x02;
			break;

		case FLH_CLK_266MHz:
			uwCR_FW = 0x3100; //Toggle2-533
			ubFlhClkDivEDO = 0x06;
			break;

		default:
			uwCR_FW = 0x3000;
			ubFlhClkDivEDO = 0x0F;
			break;

	}

	if (ubNoFWRoutine) {
		fla_PMU_Setting(ubFlashClock);
	}
	else {
		/* Adjust PMU Coprocessor Toggle Mode */
		// Step 1) Set Toggle Mode Clk
		SYSW[SYS0W_CR_ADR_FW] = uwCR_FW;

		// Step 2) Set this field to 1, PMU will execute firmware routine
		SYSL[SYS0L_WAKEUP_FLAG] |= CR_FW_T_F;

		// Step 3) Polling Bit 31 until to 0
		while ((SYSL[SYS0L_WAKEUP_FLAG] & CR_FW_T_F));
	}

	if (gubEnableOverClocking == 1) {
		//FlashClock >= FLH_CLK_200MHz (include 150/180/200/266)
		if ((ubFlashClock >= FLH_CLK_200MHz) && (gubFlashMode == FLH_MODE_TOGGLE1)) {
			//SEL CLK SW R/W
			FCONL[FCON_FLHCLK_SPLITE_EN] |= SPLITE_WR_CLK_EN;
			SYS0B[SYS0B_CLKSW_SEL7] = 0x80;
		}
		else {
			FCONL[FCON_FLHCLK_SPLITE_EN] &= ~SPLITE_WR_CLK_EN;
		}
	}

	/* Adjust FLH Clock Frequency to Lower Setting */
	SYSB[(SYS0L_CLK_DIV_VAL1 << 2) + 2] = ubFlhClkDiv; //IP6
	SYSB[(SYS0L_CLK_DIV_VAL1 << 2) + 3] = ubFlhClkDiv; //IP7
	SYSB[(SYS0L_CLK_DIV_VAL2 << 2) + 0] = ubFlhClkDiv; //IP8
	SYSB[(SYS0L_CLK_DIV_VAL2 << 2) + 1] = ubFlhClkDiv; //IP9

	// wait PLL lock
	while ((SYSL[SYS0L_CLK_CFG0 ] & SR_PLL_LOCK_PLL1) == 0);

	/*Wait DLL Tracking Done, Lock*/

	flaDLLTracking(0, 1, 0); //read
	flaDLLTracking(1, 1, 0); //write

	if (ENABLE_DEBUG_UART_FLH_INITIAL) {
		UartString("\r\nflash clock: ");
		Uart_Tx_DataHex(ubFlashClock);
	}

	if (ubFlashClock == FLH_CLK_10MHz) {
		FALLL[FCTLL_CHNL_SET] &= ~FLA_EDO_MODE;
		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nEDO off");
		}
	}
	else {
		gubClockDivValue = FLH_CLK_DIV_VALUE(ubFlhClkDivEDO);
		FALLL[FCTLL_CHNL_SET] |= FLA_EDO_MODE;
		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\nEDO on");
		}
	}

}


void fla_PMU_Setting(UBYTE ubFlashClock)
{
#if(!RDT)
	UBYTE ubCr_div_4 = 0xF;
	//Switch PLL1 CLK to REF CLK,   Set cr_sel_4 and 6~9 to 0;
	SYS0L[SYS0L_CLKSW_CTRL1] &= 0x0000FF00;
	SYS0L[SYS0L_CLKSW_CTRL2] &= 0xFFFF0000;

	//Set CR_PLL1_DIS to 1, //enable PLL1_RESET & PLL1_CHPMP_DIS & PLL1_EN_VCMID & PLL1_DIS
	SYS0L[SYS0L_PLL_CFG2] |= (CR_PLL_EN_VCMID_1 | CR_PLL_CHPMP_DIS_1 | CR_PLL_RST_1 | CR_PLL_DIS_1);

	switch (ubFlashClock) {
		case FLH_CLK_50MHz:
			SYS0L[SYS0L_PLL1_DIV] = 0x28010C6C; //DIVS=1, DIVM=40   //PLL1 DIVN1,2,3 = 12, 6, 12 (no FW-routine)
			break;

		case FLH_CLK_83MHz:
			SYS0L[SYS0L_PLL1_DIV] = 0x64030646; //DIVS=3, DIVM=100   //PLL1 DIVN1,2,3 = 6, 4, 6 (no FW-routine)
			break;

		case FLH_CLK_150MHz:
			SYS0L[SYS0L_PLL1_DIV] = 0x96030565; //DIVS=3, DIVM=96   //PLL1 DIVN1,2,3 = 3, 3, 3 (no FW-routine)
			break;

		case FLH_CLK_166MHz:
			SYS0L[SYS0L_PLL1_DIV] = 0x64030343; //DIVS=3, DIVM=100   //PLL1 DIVN1,2,3 = 3, 4, 3 (no FW-routine)
			break;

		case FLH_CLK_180MHz:
			SYS0L[SYS0L_PLL1_DIV] = 0x30010564; //DIVS=1, DIVM=48   //PLL1 DIVN1,2,3 = 4, 6, 5 (no FW-routine)
			break;

		default://40MHz
			SYS0L[SYS0L_PLL1_DIV] = 0x28010F6F; //DIVS=1, DIVM=40   //PLL1 DIVN1,2,3 = 15, 6, 15
			break;


	}

	//Set CR_PLL1_DIS to 0
	SYS0L[SYS0L_PLL_CFG2] &= ~(CR_PLL_DIS_1);

	//wait at least 20us.
	gulTemp = 0;
	while (gulTemp < 500) {
		gulTemp++;
	}

	//set  PLL1_RESET & PLL1_CHPMP_DIS & PLL1_EN_VCMID to 0
	SYS0L[SYS0L_PLL_CFG2] &= ~(CR_PLL_EN_VCMID_1 | CR_PLL_CHPMP_DIS_1 | CR_PLL_RST_1);

	//wait PLL lock
	while ((SYS0L[SYS0L_CLK_CFG0 ] & SR_PLL_LOCK_PLL1) == 0);

	//switch REF CLK to PLL_1 CLK
	SYS0L[SYS0L_CLKSW_CTRL1] |= 0xC0C000D0;
	SYS0L[SYS0L_CLKSW_CTRL2] |= 0x0000C0C0;

	// set cr_div_4
	SYS0B[SYS0B_CLK_DIV_4] = ubCr_div_4;

	// set cr_fw_t_f=0
	SYS0L[SYS0L_WAKEUP_FLAG] &= ~CR_FW_T_F;
#endif
}

void fla_Set_FPU_DLY_Cycle(UWORD uwFPUStartPtr, UBYTE ubDelayCycleValue)
{
	UWORD uwFPUWordAdr;
	uwFPUWordAdr = FPU_ADR + uwFPUStartPtr;
	uwFPUWordAdr >>= 1;
	while (IRAM2B[uwFPUWordAdr] != 0xF000) {
		if ((IRAM2B[uwFPUWordAdr] & 0xFF00) == 0x1000) {
			IRAM2B[uwFPUWordAdr] = 0x1000 | (UWORD)ubDelayCycleValue;
		}
		uwFPUWordAdr++;
	}

}

UBYTE fla_Detect_DQSB()
{
	FCONL[FCONL_FCE_ENB] = 0x01;
	FALLL[FCTLL_RESERVE] |= DETECT_DQSB_ENABLE;
	flaReset(0, 0xFF);

	if (FALLL[FCTLL_RESERVE] & DQSB_IS_TOGGLED) {
		return 1;
	}
	else {
		return 0;
	}
}

void flaScanCE_And_Remapping()
{
	UBYTE ubChannel;
	UBYTE ubStatus;
	U32 ulDelayLoop;
	UBYTE ubi;
	UBYTE ubCE, ubCEOffset;
	UBYTE pubCEMapCH[MAX_SCAN_CE_PER_CH * MAX_CHANNEL] = {0}, pubIndex[MAX_CHANNEL] = {0};
	UBYTE pubCHAndCEMap[MAX_CHANNEL][MAX_SCAN_CE_PER_CH];
	UBYTE ubNoScanCE;
	UBYTE ubUseCENumPerCH;
	UBYTE ubScanCENum[MAX_CHANNEL] = {0};
#if UFS
	UBYTE ubUFSCECount;
#endif

	U32 ulTemp[MAX_CHANNEL], ulTemp2[MAX_CHANNEL];
#if UFS
	UBYTE ubCEPTRinCH;
	UBYTE ubUFSRuleMap[16] = {0x1, 0x2, 0x3, 0x4, 0x9, 0xA, 0xB, 0xC, 0x10, 0x20, 0x30, 0x40, 0x90, 0xA0, 0xB0, 0xC0};
#endif

#if Hynix
	//UBYTE ubResetCmd;

	if (guoFlashDefaultType.B.btHynix && guoFlashUseType.B.bt8LC) {
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);
	}
#endif


	for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
		ulTemp[ubChannel] = FCTLL[FCTLL_FLH_SET + (ubChannel * 0x200 >> 2)];
		ulTemp2[ubChannel] = FCTLL[FCTLL_IO_SET + (ubChannel * 0x200 >> 2)];
	}

	_UCV REG_t * pFlaReg;

	//Check Flash Number
	gubCENumber = 0;
	gubTotalGroups = 0;
	gubPlanesPerBurst = 0;

	if (0) {
		UartString(" \r\nScan CE Number");
	}
	for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
		for (ubCE = 0; ubCE < gubMAX_SCAN_CE_PER_CH; ubCE++) {
			pubCHAndCEMap[ubChannel][ubCE] = 0xFF;
			pubCEMapCH[((gubMAX_SCAN_CE_PER_CH * ubChannel) + ubCE)] = 0xFF;
			FCONB_REMAPPING_CE[ubCE + ubChannel * MAX_CE_PER_CH] = ubCE + ubChannel * MAX_CE_PER_CH;
		}
	}

#if YMTC_JGS
	FCONL[FCONL_FCE_ENB]  = 0xFFFFFFFF;
	FALLL[FCTLL_PIO_CMD] = 0xFF;
	while ( (FALLL[FCTLL_RBY_INF] & FLH_CE_RBY_AND) == 0);

#endif

	for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
		ubScanCENum[ubChannel] = 0;
#if UFS
		ubUFSCECount = 0;
		for (ubCE = 0; ubCE < 16; ubCE++)      //Scan Max =16, CE Decoder
#else
		for (ubCE = 0; ubCE < gubMAX_SCAN_CE_PER_CH; ubCE++)      //Scan Max =16, CE Decoder
#endif
		{
			ubNoScanCE = 0;

			if (ENABLE_FORCE_CE_NUM) {
				ubUseCENumPerCH = FORCE_CE_NUMBER / 2;
			}
			else {
				if (gubScanPreformatCENumber) {
					ubUseCENumPerCH = gubCENumber_MP / 2;
				}
				else {
					ubUseCENumPerCH = MAX_CE_PER_CH; //Use Max 8
				}
			}

			if ( ubScanCENum[ubChannel] < ubUseCENumPerCH ) { //ex: 4CE => CE0 CE1 CE8 CE9
#if(UFS)
				if (1)
#else
				if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
				{
#if UFS
					FCONL[FCONL_FCE_ENB] = (U32)(ubUFSRuleMap[ubCE] << (8 * ubChannel));
#else
					FCONL[FCONL_FCE_ENB] = M_MapToCEDecoder(ubChannel, ubCE);//((BIT7 |(ubCE << 1)) << (ubChannel * 8));
#endif
				}
				else {
					FCONL[FCONL_FCE_ENB] = (BIT0 << (ubCE + ubChannel * 8));
				}
			}
			else {
				ubNoScanCE = 1;
			}

			if (ubNoScanCE == 0) {
#if Hynix
#else
				flaSetFlashInterface(ubChannel, ubCE);
#endif

				pFlaReg = (volatile REG_t *)gulFlashIP_RegBase[ubChannel];
				if (Hynix && guoFlashUseType.B.btToggle)
					pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFC;		//Send 0xfc
				else
					pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xFF;		//Send 0xff

#if (!(MicronFlashOnly || YMTC_JGS))
				pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70;		//Send 0x70 to check status
#endif


				ulDelayLoop = 0;
				do {
					ubStatus = 0;
					pFlaReg[FCTLL_IO_SET].L |= FLH_PHY_IF_RESET;
#if MicronFlashOnly || YMTC_JGS
					pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70; 	//Send 0x70 to check status
#endif
					ubStatus = (UBYTE) pFlaReg[FCTLL_PIO_DAT].L;
					ulDelayLoop++;
#if MicronFlashOnly || YMTC_JGS
					if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
					if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
					{
						if (ulDelayLoop > 10) {
							break;
						}
					}
					else {
						if (ulDelayLoop > 10000) {
							break;
						}
					}
				}
				while ( !(ubStatus & BIT7));

				if (0 || ENABLE_DEBUG_UART_SANDISK_1ZNM_SPF) {
					UartString(" \r\nCEIndex: ");
					Uart_Tx_DataHex((ubCE + ubChannel * gubMAX_SCAN_CE_PER_CH));
					UartString(" Channel: ");
					Uart_Tx_DataHex(ubChannel);
					UartString(" Status1: ");
					Uart_Tx_DataHex(ubStatus);
				}

				if (ubStatus & BIT7) {
					//power on reset
					ulDelayLoop = 0;
					do {
						ubStatus = 0;
						pFlaReg[FCTLL_IO_SET].L |= FLH_PHY_IF_RESET;
#if MicronFlashOnly || YMTC_JGS
						pFlaReg[FCTLL_PIO_CMD].B.B0 = 0x70; 	//Send 0x70 to check status
#endif
						ubStatus = (UBYTE) pFlaReg[FCTLL_PIO_DAT].L;
						ulDelayLoop++;
						if (ulDelayLoop > 4000) {
							break;
						}
					}
					while (ubStatus != 0xE0);

					if (0 || ENABLE_DEBUG_UART_SANDISK_1ZNM_SPF) {
						UartString(" Status2: ");
						Uart_Tx_DataHex(ubStatus);
					}

					if (ubStatus == 0xE0) {
#if MicronFlashOnly
						// Send DA cmd to switch back to MLC mode if the selected CE is ready.
						if (ubStatus & BIT6) {
							pFlaReg[FCTLL_PIO_CMD].B.B0 = 0xDF; 	  // Send 0xdf
						}
#endif
#if Hynix
						flaSetNandParameter(ubChannel, 0x70, 0x01);
#endif
						ubScanCENum[ubChannel]++;
#if UFS
						if ((ubCE > 7) && (ubUFSCECount < 5)) {
							ubUFSCECount = 5;
							gubUFSDefaultMode_CE |= UFS_Sample_Exist << (2 + ubChannel);
							if (UFSToggleCheck((BIT0 << (8 * ubChannel + 4)), ubChannel)) {
								gubUFSDefaultMode_CE |= UFS_Sample_Default_Mode_Toggle << (2 + ubChannel);
							}
						}
						else {
							ubUFSCECount ++;
						}
#endif
#if(UFS)
						if (1)
#else
						if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
						{
#if UFS
							ubCEPTRinCH = ubUFSCECount - 1;
							pubCEMapCH[ubCEPTRinCH + ubChannel * gubMAX_SCAN_CE_PER_CH] = ubChannel;
							gubUFSRuleMap[ubChannel][ubCEPTRinCH] = ubUFSRuleMap[ubCE];
#else
							pubCEMapCH[ubCE + ubChannel * gubMAX_SCAN_CE_PER_CH] = ubChannel;
#endif
						}
						else {
							pubCEMapCH[ubCE + ubChannel * 8] = ubChannel;
						}
						gubCENumber++;
						if ((ubChannel + 1) > gubPlanesPerBurst) {
							gubPlanesPerBurst = ubChannel + 1;
						}
						if (ubChannel == 0) {
							gubTotalGroups++;
						}
					}
				}
			}
		}
	}

	if (gubPlanesPerBurst == 1) {
		FCONL[FCONL_FLH_FUNC] &= MT_TABLE_CE_NORMAL_MODE;
	}

	for (ubi = 0; ubi < (gubMAX_SCAN_CE_PER_CH * MAX_CHANNEL); ubi++) {
		if (pubCEMapCH[ubi] != 0xFF) {
			pubCHAndCEMap[pubCEMapCH[ubi]][pubIndex[pubCEMapCH[ubi]]] = ubi;        //Logical CH,CE mapping to Physical CE
			if (pubIndex[pubCEMapCH[ubi]] == MAX_CE_PER_CH) {
				while (1) {}
			}
			else {
				pubIndex[pubCEMapCH[ubi]]++;
			}
		}
#if MicronFlashOnly
		gubMicronMode[ubi] = MICRON_MLC;
#endif
	}

#if(UFS)
	if (1)
#else
	if (gubGPIO_CE_Decoder && (!MicronFlashOnly))
#endif
	{
		//CE Decoder need channel's "physical CEIndex" information
		for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
			for (ubCE = 0; ubCE < MAX_CE_PER_CH; ubCE++) {
				if (pubCHAndCEMap[ubChannel][ubCE] < MAX_CHANNEL * gubMAX_SCAN_CE_PER_CH) {
					gubCH_CEMappingToPCE[ubChannel][ubCE] = pubCHAndCEMap[ubChannel][ubCE] % gubMAX_SCAN_CE_PER_CH;
				}
				else { //FF
					gubCH_CEMappingToPCE[ubChannel][ubCE] = 0xFF;
				}
			}
		}
	}


	for (ubi = 0; ubi < MAX_CE_PER_CH * MAX_CHANNEL; ubi++) {
		pubCEMapCH[ubi] = 0x3F;
	}

	for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
		ubCEOffset = 0;
		for (ubCE = 0; ubCE < MAX_CE_PER_CH; ubCE++) {
			if (pubCHAndCEMap[ubChannel][ubCE] != 0xFF) {
				pubCEMapCH[pubCHAndCEMap[ubChannel][ubCE]] = (8 * ubChannel) + ubCEOffset;
				ubCEOffset++;
			}
		}
	}

	for (ubi = 0; ubi < (MAX_CE_PER_CH * MAX_CHANNEL); ubi++) {
#if(UFS)
		if (0)
#else
		if (!gubGPIO_CE_Decoder)
#endif
		{
			FCONB_REMAPPING_CE[ubi] = pubCEMapCH[ubi];
		}
		gubReMappingCE[ubi] = pubCEMapCH[ubi];
#if Hynix
		if (pubCEMapCH[ubi] != 0x3F) {
			gulCeEnBitMap |= 1uL << pubCEMapCH[ubi];
		}
#endif
	}

	for (ubChannel = 0; ubChannel < MAX_CHANNEL; ubChannel++) {
		FCTLL[FCTLL_FLH_SET + (ubChannel * 0x200 >> 2)] = ulTemp[ubChannel];
		FCTLL[FCTLL_IO_SET + (ubChannel * 0x200 >> 2)] = ulTemp2[ubChannel];
	}
}

void flaSetFlashInterface(UBYTE ubChannel, UBYTE ubCE)
{
	if (gubIOMode_CE[ubChannel] & (BIT0 << ubCE)) {
		flaSetPadIOType(ubChannel, DIFFERENTIAL_IO_TYPE);
	}
	else {
		flaSetPadIOType(ubChannel, SINGLE_END_IO_TYPE);
	}
	/*	FlashMode_CE :0; FlashONFI_CE:0 => Legacy
	 *	FlashMode_CE :1; FlashONFI_CE:0 => Toggle(NVDDR2)
	 *	FlashMode_CE :1; FlashONFI_CE:1 => NVDDR1
	*/
	if (gubFlashMode_CE[ubChannel] & (BIT0 << ubCE)) {
		if (gubFlashONFI_CE[ubChannel] & (BIT0 << ubCE)) {
			FCTLL[FCTLL_FLH_SET + (ubChannel * 0x200 >> 2)] |= FLA_ONFI_MODE;
		}
		else {
			FCTLL[FCTLL_FLH_SET + (ubChannel * 0x200 >> 2)] |= FLA_TOGGLE_MODE;
		}
	}
	else {
		FCTLL[FCTLL_FLH_SET + (ubChannel * 0x200 >> 2)] &= FLA_LEGACY_MODE;
	}
}

void flaSet_ODT_DRIVE()
{
	if (BURNER && (gubCENumber == 0)) {
		return;
	}

	while (gFQI.ubFQLinkNumber) {
		M_SwitchTask();
	}

	flaSetFlashPadControl(SET_ODT_100ohm); // =1, 在IO.h宣告


#if MicronFlashOnly || YMTC_JGS
	if (!gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
	if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
	{
		flaSetOverDrive();
	}

}

void flaSetPadIOType(UBYTE ubChannel, U32 ulPadIOType)
{
	_UCV REG_t * pFlaReg;
	pFlaReg  = (_UCV REG_t *)gulFlashIP_RegBase[ubChannel];

	if (ulPadIOType == SINGLE_END_IO_TYPE) {
		pFlaReg[FCTLL_IO_SET].L &= SINGLE_END_IO_TYPE;
#if 0//Hynix
		SYS1L[ SYS1L_PAD_FDAT0_CFG] &= ~(CR_FDAT0_CO | CR_FDAT0_ODT_EN | BIT28 | BIT29);
		SYS1L[ SYS1L_PAD_FDAT1_CFG] &= ~(CR_FDAT1_CO | CR_FDAT1_ODT_EN | BIT28 | BIT29);
#else
		SYS1L[ SYS1L_PAD_FDAT0_CFG] &= ~(CR_FDAT0_CO);
		SYS1L[ SYS1L_PAD_FDAT1_CFG] &= ~(CR_FDAT1_CO);
#endif
	}
	else {
		pFlaReg[FCTLL_IO_SET].L |= DIFFERENTIAL_IO_TYPE;  // for DQSB
#if 0//Hynix
		SYS1L[ SYS1L_PAD_FDAT0_CFG] |= (CR_FDAT0_CO | CR_FDAT0_ODT_EN | BIT28 | BIT29);
		SYS1L[ SYS1L_PAD_FDAT1_CFG] |= (CR_FDAT1_CO | CR_FDAT1_ODT_EN | BIT28 | BIT29);
#else
		SYS1L[ SYS1L_PAD_FDAT0_CFG] |= (CR_FDAT0_CO);
		SYS1L[ SYS1L_PAD_FDAT1_CFG] |= (CR_FDAT1_CO);
#endif
	}

}

void flaSetParameterBeforePreformat(SYSTEM_GROUP_t *InfoArray)
{
	gulDiskSizeInMB = 0;
	gulNativeSize = 0;
	flaInfoBlockSetting(InfoArray);
	flaSetParameterByFlashDefaultType();
	flaSetFlashUseType(1);
	flaSetFlashAddressType();
	flaSetParameter(1);
	InitFTLVariable();
}

void flaSetFakeInfoBlk(SYSTEM_GROUP_t *InfoArray, MP_SMART_Attribute_Entry * MP_Parameter)
{
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	/////////////// pretend creating MP parameter -- MATT ///////////////
	char *s = "PhIsOn";
	//memset(InfoArray, 0, sizeof(InfoArray));
	mDMAC_SV(4, (U32)InfoArray, 512, 0, 0, 0, DMA_EN_SETCQ, 1);
	memcpy(InfoArray->u8bPhIsOnInfoMark, s, 6);
	memcpy((void*)InfoArray->ubModelNumber, ModelTable, sizeof(ModelTable));
	memcpy((void*)InfoArray->ubSerialNumber, SerialTable, sizeof(SerialTable));
	memcpy((void*)InfoArray->ubFWVersion, VerTable, sizeof(VerTable));
	InfoArray->ubFWVersionEnable = 0x33;
	InfoArray->ubILMode = 0;//default
	InfoArray->ubUltraMLCEnable = 0;//default:0 enable:1 disable:2
	InfoArray->ubFlashInterface = 0;
	InfoArray->ubFlashClk = 0;

	InfoArray->ubCustomer = 0;
	InfoArray->ubDEVSLPSupport = 0;//default:0 enable:1 disable:2
	//InfoArray->ubWWNSupport = 0; //default:0 enable:1 disable:2
	InfoArray->uwWWN[0] = 0;
	InfoArray->uwWWN[1] = 0;
	InfoArray->uwWWN[2] = 0;
	InfoArray->uwWWN[3] = 0;

	InfoArray->ubNCQSupport = 0; //default:0 enable:1 disable:2
	InfoArray->ub48BitCmdSupport = 0; //default:0 enable:1 disable:2
	InfoArray->ubFormFactor = 0;
	InfoArray->ubWriteCacheSupport = 0;//default:0 enable:1 disable:2

	InfoArray->ubXferModeEnable = 0;//default:0 enable:1 disable:2
	InfoArray->ubMultiDMAMode[0] = 0;
	InfoArray->ubMultiDMAMode[1] = 0;
	InfoArray->ubUltraDMAMode[0] = 0;
	InfoArray->ubUltraDMAMode[1] = 0;
	InfoArray->ubCENumber = 0;

	InfoArray->ubIDW129_159Enable = 0;
	memset(InfoArray->uwIDW129_159, 0, sizeof(InfoArray->uwIDW129_159));

	InfoArray->ulMaxPECycle_D3 = 0;
	InfoArray->ulMaxPECycle_D1 = 0;

	InfoArray->ubDisableEnableMultiPlaneMode = 0;//default:0 enable:1 disable:2
	InfoArray->ubDisableEnableA2Mode = 0;//default:0 enable:1 disable:2

	UBYTE ubIndex;
	for (ubIndex = 0; ubIndex < AttributeTotalEntry; ubIndex++) {
		if (ubIndex >= 3) {
			MP_Parameter[ubIndex].ubID = 0x01 + ubIndex;
			MP_Parameter[ubIndex].ubFlag[0] = ubIndex;
			MP_Parameter[ubIndex].ubFlag[1] = ubIndex;
			MP_Parameter[ubIndex].uwValue = 0x0000;
			MP_Parameter[ubIndex].uwWorst = 0x0000;
			MP_Parameter[ubIndex].uwData = 0x0002;
			MP_Parameter[ubIndex].ubThreshold = 0x00;
		}
		else {
			//temperature
			MP_Parameter[0].ubID = 0xC2;
			MP_Parameter[0].ubFlag[0] = 0;
			MP_Parameter[0].ubFlag[1] = 0;
			MP_Parameter[0].uwValue = 0x0000;
			MP_Parameter[0].uwWorst = 0x0000;
			MP_Parameter[0].uwData = 0x0801;
			MP_Parameter[0].ubThreshold = 0x00;

			//Avg erase count
			MP_Parameter[1].ubID = 0xF4;
			MP_Parameter[1].ubFlag[0] = 0;
			MP_Parameter[1].ubFlag[1] = 0;
			MP_Parameter[1].uwValue = 0x0000;
			MP_Parameter[1].uwWorst = 0x0000;
			MP_Parameter[1].uwData = 0x0202;
			MP_Parameter[1].ubThreshold = 0x00;

			//Max erase count
			MP_Parameter[2].ubID = 0xF5;
			MP_Parameter[2].ubFlag[0] = 0;
			MP_Parameter[2].ubFlag[1] = 0;
			MP_Parameter[2].uwValue = 0x0000;
			MP_Parameter[2].uwWorst = 0x0000;
			MP_Parameter[2].uwData = 0x0201;
			MP_Parameter[2].ubThreshold = 0x00;
		}
	}

}

void ACTiming_Setting_Register(UBYTE ubFlashClock)
{
	if ((gubFlashMode == FLH_MODE_TOGGLE1) || (gubFlashMode == FLH_MODE_TOGGLE2) || (gubFlashMode == FLH_MODE_ONFI_NVDDR2) || (gubFlashMode == FLH_MODE_ONFI_NVDDR) ) {
		UBYTE ubTime[TIME_NUM_REGISTER_TOGGLE];
		//UBYTE ubTime[TIME_NUM_REGISTER_TOGGLE] = {1, 1, 2, 2, 2, 2, 7, 1, 1, 3, 2, 1, 1, 7, 3, 5, 23, 6, 3, 3, 2, 4, 2};//TOGGLE-400, byte
		if ( ((gubFlashMode == FLH_MODE_TOGGLE2) || (gubFlashMode == FLH_MODE_ONFI_NVDDR2)) && (gubFlashClock == FLH_CLK_200MHz)) {
			ubTime[TIME_TGL_3_0] = 3;
			ubTime[TIME_TGL_6_4] = 1;
			ubTime[TIME_TGL_11_8] = 2;
			ubTime[TIME_TGL_14_12] = 1;

			ubTime[TIME_TGL_19_16] = 11;  // delay 55 ns

			ubTime[TIME_TGL_23_20] = 7;
			ubTime[TIME_TGL_27_24] = 6;
			ubTime[TIME_ONFI_26_24] = 3;
			ubTime[TIME_ONFI_30_28] = 3;
			ubTime[TIME_RTY_16] = 0;
			ubTime[TIME_RTY_18_17] = 0;
			ubTime[TIME_RTY_20_19] = 0;
			ubTime[TIME_RTY_21] = 0;
			ubTime[TIME_RTY_22] = 1;
		}
		else if (gubFlashMode == FLH_MODE_TOGGLE2 && gubFlashClock == FLH_CLK_180MHz) {
			ubTime[TIME_TGL_3_0] = 2;
			ubTime[TIME_TGL_6_4] = 1;
			ubTime[TIME_TGL_11_8] = 2;  //Terry 0413
			ubTime[TIME_TGL_14_12] = 1;
			ubTime[TIME_TGL_19_16] = 11;  // delay 55 ns
			ubTime[TIME_TGL_23_20] = 6;
			ubTime[TIME_TGL_27_24] = 5;
			ubTime[TIME_ONFI_26_24] = 3;
			ubTime[TIME_ONFI_30_28] = 3;
			ubTime[TIME_RTY_16] = 0;
			ubTime[TIME_RTY_18_17] = 0;
			ubTime[TIME_RTY_20_19] = 0;
			ubTime[TIME_RTY_21] = 0;
			ubTime[TIME_RTY_22] = 1;
		}
		else if (gubFlashMode == FLH_MODE_TOGGLE2 && gubFlashClock == FLH_CLK_166MHz) {
			ubTime[TIME_TGL_3_0] = 2;
			ubTime[TIME_TGL_6_4] = 1;
			ubTime[TIME_TGL_11_8] = 1;
			ubTime[TIME_TGL_14_12] = 1;
			ubTime[TIME_TGL_19_16] = 11;
			ubTime[TIME_TGL_23_20] = 6;
			ubTime[TIME_TGL_27_24] = 5;
			ubTime[TIME_ONFI_26_24] = 3;
			ubTime[TIME_ONFI_30_28] = 3;
			ubTime[TIME_RTY_16] = 0;
			ubTime[TIME_RTY_18_17] = 0;
			ubTime[TIME_RTY_20_19] = 0;
			ubTime[TIME_RTY_21] = 0;
			ubTime[TIME_RTY_22] = 1;
		}
		else { //default toggle-400
			ubTime[TIME_TGL_3_0] = 3;
			ubTime[TIME_TGL_6_4] = 1;
			ubTime[TIME_TGL_11_8] = 2;
			ubTime[TIME_TGL_14_12] = 1;
			ubTime[TIME_TGL_19_16] = 11;  // delay 55 ns
			ubTime[TIME_TGL_23_20] = 7;
			ubTime[TIME_TGL_27_24] = 6;
			ubTime[TIME_ONFI_26_24] = 3;
			ubTime[TIME_ONFI_30_28] = 3;
			ubTime[TIME_RTY_16] = 0;
			ubTime[TIME_RTY_18_17] = 0;
			ubTime[TIME_RTY_20_19] = 0;
			ubTime[TIME_RTY_21] = 0;
			ubTime[TIME_RTY_22] = 1;
		}

		if (gubACTimingTableEnable) {
			UBYTE ubi;
			for (ubi = 0; ubi < TIME_NUM_REGISTER_TOGGLE; ubi++) {
				ubTime[ubi] = gubACTimingTable[ubi];
			}
		}

		//TGL_TIME_CFG 3:0
		FCONL[FCONL_TGL_TIME_CFG] &= ~(0x0F << 0);
		FCONL[FCONL_TGL_TIME_CFG] |= (ubTime[TIME_TGL_3_0] << 0);

		//TGL_TIME_CFG 6:4
		FCONL[FCONL_TGL_TIME_CFG] &= ~(0x07 << 4);
		FCONL[FCONL_TGL_TIME_CFG] |= (ubTime[TIME_TGL_6_4] << 4);

		//TGL_TIME_CFG 11:8
		FCONL[FCONL_TGL_TIME_CFG] &= ~(0x0F << 8);
		FCONL[FCONL_TGL_TIME_CFG] |= (ubTime[TIME_TGL_11_8] << 8);

		//TGL_TIME_CFG 14:12
		FCONL[FCONL_TGL_TIME_CFG] &= ~(0x07 << 12);
		FCONL[FCONL_TGL_TIME_CFG] |= (ubTime[TIME_TGL_14_12] << 12);

		//TGL_TIME_CFG 19:16
		FCONL[FCONL_TGL_TIME_CFG] &= ~(0x0F << 16);
		FCONL[FCONL_TGL_TIME_CFG] |= (ubTime[TIME_TGL_19_16] << 16);

		//TGL_TIME_CFG 23:20
		FCONL[FCONL_TGL_TIME_CFG] &= ~(0x0F << 20);
		FCONL[FCONL_TGL_TIME_CFG] |= (ubTime[TIME_TGL_23_20] << 20);

		//TGL_TIME_CFG 27:24
		FCONL[FCONL_TGL_TIME_CFG] &= ~(0x0F << 24);
		FCONL[FCONL_TGL_TIME_CFG] |= (ubTime[TIME_TGL_27_24] << 24);


		//ONF_TIME_CFG 26:24
		FCONL[FCONL_ONF_TIME_CFG] &= ~(0x07 << 24);
		FCONL[FCONL_ONF_TIME_CFG] |= (ubTime[TIME_ONFI_26_24] << 24);

		//ONF_TIME_CFG 30:28
		FCONL[FCONL_ONF_TIME_CFG] &= ~(0x07 << 28);
		FCONL[FCONL_ONF_TIME_CFG] |= (ubTime[TIME_ONFI_30_28] << 28);

		//RTY_CFG 16
		FCONL[FCONL_RTY_CFG] &= ~(0x01 << 16);
		FCONL[FCONL_RTY_CFG] |= (ubTime[TIME_RTY_16] << 16);

		//RTY_CFG 18:17
		FCONL[FCONL_RTY_CFG] &= ~(0x03 << 17);
		FCONL[FCONL_RTY_CFG] |= (ubTime[TIME_RTY_18_17] << 17);

		//RTY_CFG 20:19
		FCONL[FCONL_RTY_CFG] &= ~(0x03 << 19);
		FCONL[FCONL_RTY_CFG] |= (ubTime[TIME_RTY_20_19] << 19);

		//RTY_CFG 21
		FCONL[FCONL_RTY_CFG] &= ~(0x01 << 21);
		FCONL[FCONL_RTY_CFG] |= (ubTime[TIME_RTY_21] << 21);

		//RTY_CFG 22
		FCONL[FCONL_RTY_CFG] &= ~(0x01 << 22);
		FCONL[FCONL_RTY_CFG] |= (ubTime[TIME_RTY_22] << 22);
	}

	if (ENABLE_DEBUG_UART_FLH_INITIAL) {
		UartString("\r\nAC toggle:");
		UartLongHex(FCONL[FCONL_TGL_TIME_CFG]);
	}
}

void ACTiming_Setting_FPU(UBYTE ubFlashClock)
{
	if ((gubFlashMode == FLH_MODE_TOGGLE1) || (gubFlashMode == FLH_MODE_TOGGLE2) || (gubFlashMode == FLH_MODE_ONFI_NVDDR2) || (gubFlashMode == FLH_MODE_ONFI_NVDDR) ) {
		//UBYTE ubTime[TIME_NUM_FPU_TOGGLE] = {0, 3, 2, 3, 61, 2, 24, 60, 20, 0xFF, 0xFF};//Toggle-400 {61, 3, 2, 3, 61, 2, 24, 60, 20},
		UBYTE ubTime[TIME_NUM_FPU_TOGGLE];
		//toritani：only tAGL can set 0
		if (gubFlashMode == FLH_MODE_TOGGLE2 && gubFlashClock == FLH_CLK_200MHz) {
			ubTime[TIME_ADL_TGL] = 0;
			ubTime[TIME_AR_TGL] = 3;
			ubTime[TIME_CH_TGL] = 2;
			ubTime[TIME_CLR_TGL] = 3;
			ubTime[TIME_CWAW_TGL] = 61;
			ubTime[TIME_RR_TGL] = 2;
			if (BURNER) {
				ubTime[TIME_WHR_TGL] = 24;
			}
#if !UFS
			else if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				ubTime[TIME_WHR_TGL] = 120;
			}
#endif
			else {
				ubTime[TIME_WHR_TGL] = 24;
			}
			ubTime[TIME_WHR2_TGL] = 60;
			ubTime[TIME_WW_TGL] = 20;
		}
		else if (gubFlashMode == FLH_MODE_TOGGLE2 && gubFlashClock == FLH_CLK_180MHz) {
			ubTime[TIME_ADL_TGL] = 0;
			ubTime[TIME_AR_TGL] = 3;
			ubTime[TIME_CH_TGL] = 2;
			ubTime[TIME_CLR_TGL] = 3;
			ubTime[TIME_CWAW_TGL] = 61;
			ubTime[TIME_RR_TGL] = 2;
			if (BURNER) {
				ubTime[TIME_WHR_TGL] = 22;
			}
#if !UFS
			else if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				ubTime[TIME_WHR_TGL] = 110;
			}
#endif
			else {
				ubTime[TIME_WHR_TGL] = 22;
			}
			ubTime[TIME_WHR2_TGL] = 54;
			ubTime[TIME_WW_TGL] = 20;
		}
		else if (gubFlashMode == FLH_MODE_TOGGLE2 && gubFlashClock == FLH_CLK_166MHz) {
			ubTime[TIME_ADL_TGL] = 0;
			ubTime[TIME_AR_TGL] = 2;
			ubTime[TIME_CH_TGL] = 1;
			ubTime[TIME_CLR_TGL] = 2;
			ubTime[TIME_CWAW_TGL] = 50;
			ubTime[TIME_RR_TGL] = 1;
			if (BURNER) {
				ubTime[TIME_WHR_TGL] = 20;
			}
#if !UFS
			else if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				ubTime[TIME_WHR_TGL] = 100;
			}
#endif
			else {
				ubTime[TIME_WHR_TGL] = 20;
			}
			ubTime[TIME_WHR2_TGL] = 50;
			ubTime[TIME_WW_TGL] = 17;
		}
		else { //default toggle-400
			ubTime[TIME_ADL_TGL] = 0;
			ubTime[TIME_AR_TGL] = 3;
			ubTime[TIME_CH_TGL] = 2;
			ubTime[TIME_CLR_TGL] = 3;
			ubTime[TIME_CWAW_TGL] = 61;
			ubTime[TIME_RR_TGL] = 2;
			if (BURNER) {
				ubTime[TIME_WHR_TGL] = 24;
			}
#if !UFS
			else if (gubGPIO_CE_Decoder && (!MicronFlashOnly)) {
				ubTime[TIME_WHR_TGL] = 120;
			}
#endif
			else {
				ubTime[TIME_WHR_TGL] = 24;
			}
			ubTime[TIME_WHR2_TGL] = 60;
			ubTime[TIME_WW_TGL] = 20;
		}

		if (gubACTimingTableEnable) {
			UBYTE ubi;
			for (ubi = 0; ubi < TIME_NUM_FPU_TOGGLE; ubi++) {
				ubTime[ubi] = gubACTimingTable[TIME_NUM_REGISTER_TOGGLE + ubi];
			}
		}

		fla_Set_FPU_DLY_Cycle(FPU_PTR_C70_POL_MK40, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C70_POL_MK40_C00, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C70_POL_MK20, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C70_POL_MK20_C00, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C70_DAT_MK40, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C70_DAT_MK20, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C71_DAT_MK40, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C71_DAT_MK20, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C73_DAT_MK40, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C73_DAT_MK20, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C70_DAT_MK0F, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C71_DAT_MK1F, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_C11_C70_POL_MK40_C81_A5_DW_C15, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C11_C70_POL_MK40_C81_A5_DW, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, ubTime[TIME_WHR_TGL]);

#if Hynix3DV6 || YMTC_JGS

#else
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, ubTime[TIME_WHR_TGL]);
#endif




		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31, ubTime[TIME_WHR_TGL]);



#if Hynix3DV6 || YMTC_JGS

#else
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31, ubTime[TIME_WHR_TGL]);

#endif
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31, ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31, ubTime[TIME_WHR_TGL]);


		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C00_DR, ubTime[TIME_WHR2_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C00_A5_C05_A2_CE0_DR, ubTime[TIME_WHR2_TGL]);

		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C80_A5_DW, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C80_A5_DW, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C80_A5_DW_C11, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C80_A5_DW_C11, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C80_A5_DW_C15, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C80_A5_DW_C15, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C80_A5_DW_C10, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C80_A5_DW_C10, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C81_A5_DW_C11, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C81_A5_DW_C11, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C81_A5_DW_C15, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C81_A5_DW_C15, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_CA2_C81_A5_DW_C10, ubTime[TIME_ADL_TGL]);
		fla_Set_FPU_DLY_Cycle(FPU_PTR_AG_C81_A5_DW_C10, ubTime[TIME_ADL_TGL]);

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C70_DAT_MK03), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C70_DAT_MK0C), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C70_DAT_MK04), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C70_DAT_MK01), ubTime[TIME_WHR_TGL]);
#if TLC
#if Hynix3DV6
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C01_AG_C80_A5_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C81_A5_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C81_A5_DW_C22), ubTime[TIME_WHR_TGL]);

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C02_AG_C80_A5_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C81_A5_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C81_A5_DW_C22), ubTime[TIME_WHR_TGL]);

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C03_AG_C80_A5_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C81_A5_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C81_A5_DW_C23), ubTime[TIME_WHR_TGL]);



		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C01_AG_C85_A5_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C85_A5_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C85_A5_NOP_C22), ubTime[TIME_WHR_TGL]);

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C02_AG_C85_A5_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C85_A5_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C85_A5_NOP_C22), ubTime[TIME_WHR_TGL]);

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C03_AG_C85_A5_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C85_A5_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C85_A5_NOP_C23), ubTime[TIME_WHR_TGL]);



		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C01_AG_C80_A5_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C81_A5_DW_C22), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C02_AG_C80_A5_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C81_A5_DW_C22), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C03_AG_C80_A5_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C81_A5_DW_C23), ubTime[TIME_WHR_TGL]);

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C01_AG_C85_A5_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C85_A5_NOP_C22), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C02_AG_C85_A5_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C85_A5_NOP_C22), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C03_AG_C85_A5_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C85_A5_NOP_C23), ubTime[TIME_WHR_TGL]);

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C01_AG_C80_A5_DW_C22), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C02_AG_C80_A5_DW_C22), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C03_AG_C80_A5_DW_C23), ubTime[TIME_WHR_TGL]);


#elif YMTC

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_CDF_AG_C80_A6_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C80_A6_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_CDF_AG_C80_A6_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C80_A6_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_CDF_AG_C80_A6_DW), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C80_A6_DW_C10), ubTime[TIME_WHR_TGL]);

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_CDF_AG_C85_A6_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C85_A6_NOP_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_CDF_AG_C85_A6_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C85_A6_NOP_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_CDF_AG_C85_A6_NOP), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C85_A6_NOP_C10), ubTime[TIME_WHR_TGL]);

		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_CDF_AG_C80_A5_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_CDF_AG_C80_A5_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_CDF_AG_C80_A5_DW_C10), ubTime[TIME_WHR_TGL]);
#else

		// =====D3 Write (2P)=====
		// ===== First =====
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C01_AG_C85_A5_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C02_AG_C85_A5_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C03_AG_C85_A5_DW_C15), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C03_AG_C85_A5_DW_C10), ubTime[TIME_WHR_TGL]);
		// ===== Foggy =====
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C01_AG_C85_A5_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C02_AG_C85_A5_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_DW_C15), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_DW_C10), ubTime[TIME_WHR_TGL]);
		// ===== Fine =====
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C01_AG_C85_A5_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C02_AG_C85_A5_DW_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_DW_C15), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_DW_C10), ubTime[TIME_WHR_TGL]);

		//Non-DMA
		// ===== First =====
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C01_AG_C85_A5_NOP_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C02_AG_C85_A5_NOP_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C03_AG_C85_A5_NOP_C15), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C03_AG_C85_A5_NOP_C10), ubTime[TIME_WHR_TGL]);
		// ===== Foggy =====
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C01_AG_C85_A5_NOP_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C02_AG_C85_A5_NOP_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_NOP_C15), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_NOP_C10), ubTime[TIME_WHR_TGL]);
		// ===== Fine =====
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C01_AG_C85_A5_NOP_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C02_AG_C85_A5_NOP_C1A), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_NOP_C15), ubTime[TIME_WHR_TGL]);
		fla_Set_FPU_DLY_Cycle(RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_NOP_C10), ubTime[TIME_WHR_TGL]);

#endif
#endif
	}

}

void ACTiming_Setting_MTP(UBYTE ubFlashClock, UBYTE ubWaitFQ)
{
	if ((gubFlashMode == FLH_MODE_TOGGLE1) || (gubFlashMode == FLH_MODE_TOGGLE2)) {
		UBYTE ubValue;
		//UBYTE ubTime[TIME_NUM_MTP_CE_DELAY_TOGGLE] = 	{0, 0, 0, 0xFF, 0xFF};//Toggle-400{5, 9, 0},

		UBYTE ubTime[TIME_NUM_MTP_CE_DELAY_TOGGLE];
		if (gubFlashMode == FLH_MODE_TOGGLE2 && gubFlashClock == FLH_CLK_200MHz) {
			ubTime[TIME_CR_TGL] = 0;
			ubTime[TIME_CS_TGL] = 0;
			ubTime[TIME_CS2_0_TGL] = 0;
		}
		else if (gubFlashMode == FLH_MODE_TOGGLE2 && gubFlashClock == FLH_CLK_180MHz) {//Terry from 200MHz
			ubTime[TIME_CR_TGL] = 0;
			ubTime[TIME_CS_TGL] = 0;
			ubTime[TIME_CS2_0_TGL] = 0;
		}
		else if (gubFlashMode == FLH_MODE_TOGGLE2 && gubFlashClock == FLH_CLK_166MHz) {
			ubTime[TIME_CR_TGL] = 0;
			ubTime[TIME_CS_TGL] = 0;
			ubTime[TIME_CS2_0_TGL] = 0;
		}
		else { //default toggle-400
			ubTime[TIME_CR_TGL] = 0;
			ubTime[TIME_CS_TGL] = 0;
			ubTime[TIME_CS2_0_TGL] = 0;
		}

		if (gubACTimingTableEnable) {
			UBYTE ubi;
			for (ubi = 0; ubi < TIME_NUM_MTP_CE_DELAY_TOGGLE; ubi++) {
				ubTime[ubi] = gubACTimingTable[TIME_NUM_REGISTER_TOGGLE + TIME_NUM_FPU_TOGGLE + ubi];
			}
		}

		ubValue = 0;
		ubValue = GetLargerValue(ubValue, ubTime[TIME_CR_TGL], 0xF);
		ubValue = GetLargerValue(ubValue, ubTime[TIME_CS_TGL], 0xF);
		ubValue = GetLargerValue(ubValue, ubTime[TIME_CS2_0_TGL], 0xF);

		if (ubWaitFQ) {
			while (gFQI.ubFQLinkNumber) {
				M_SwitchTask();
			}
		}

		gulPara0x1C.BitMap.ubCEDelay = ubValue;
	}
}

void ACTiming_Setting(UBYTE ubFlashClock, UBYTE ubWaitFQ)
{
#if (!BURNER)
	if (ubFlashClock != gubACTimingClock) {
		gubACTimingTableEnable = 0;
	}

#else
	gubACTimingTableEnable = 0;
#endif

	ACTiming_Setting_Register(ubFlashClock);
	ACTiming_Setting_FPU(ubFlashClock);
	ACTiming_Setting_MTP(ubFlashClock, ubWaitFQ);
}

UBYTE GetLargerValue(UBYTE ubNum1, UBYTE ubNum2, UBYTE ubMask)
{
	if ((ubNum1 & ubMask) > (ubNum2 & ubMask)) {
		return (ubNum1 & ubMask);
	}
	else {
		return (ubNum2 & ubMask);
	}
}

void flaReloadAonHeader(void)
{
	M_CheckRDTModeToDisableFunction();
	U32 ulAddressRegion = (FCONL[FCONL_FLH_FUNC] & RDM_REF_ROW_ADR_5_TO_0);
	UBYTE  ubCEIndex;
	UBYTE ubDepth, ubFQIndex, ubL4kLink;
	UWORD uwPageIndex = 0;
	UBYTE ubCodeUnit, ubCodeBurst;
	_UCV L4KTable16B pL4KTable;
	_UCV L4KTable16B *pL4KTablePtr;

	FCONL[FCONL_FLH_FUNC] |= RDM_REF_ROW_ADR_5_TO_0;
	//ulBurnerSeed = gulBurnerSeedOrigin;

	if (gubSysCodeSet[0] == 0xFF) {
		gubErrorCode = 0x3;
		if (ENABLE_DEBUG_RW) {
			flaLEDBlink(0xcb);
		}
		ReturnErr();
		return;
	}

#if Hynix
	FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
		FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);

		ubFQIndex = gFQI.ubFQLinkFree;
		FlashQueue_t *FQ;
		ubCodeUnit = guwSysCodeBlock[0][ubCEIndex] >> gubBurstsPerBankLog;
		ubCodeBurst = guwSysCodeBlock[0][ubCEIndex] & gubBurstsPerBankMask;

		M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
		FQ->ulRAMAddr = AON_BASE;
		FQ->btCheckEmpty = 1;
		FQ->btThisIsErasePage = 0;
		FQ->ubFJob = BYTE_FJOB_READ;

#if ENABLE_TLC_PROGRAM_CODEBLOCK
		FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#else
		FQ->uwFJobInfo = (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#endif

		FQ->uwFUnit = ubCodeUnit;
		if (ENABLE_PLANE_CH_CE) {
			FQ->ulFEntry = (((M_Multiplier((U32)uwPageIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + ubCodeBurst) << gub4kEntrysPerPlaneLog);
		}
		else {
			FQ->ulFEntry = (((M_Multiplier((U32)uwPageIndex, DEF_PlanesPerPage_Mcl_Div))  + (ubCEIndex & gubPlanesPerBurstMask)) << gub4kEntrysPerPlaneLog);
		}
		FQ->ubDepth = ubDepth;
		FQ->ubL4kIndex = ubL4kLink;
		FQ->btD3 = 1;
		FQ->btUNServed = 1;
		FQ->btRecordFailLogByMyself = 1;

		gulPara0x24.BitMap.ubFrameNum = 1;
		gulPara0x24.BitMap.btBufModeEn = 0;
		//gMTQ->Depth[ubDepth].Para0x04.BitMap.ubALUSel = 1;
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
			pL4KTable.Para0x04.BitMap.ubL4K_SPRV = 0xFF;
			pL4KTable.Para0x08.BitMap.uwNEXT_L4K_PTR = 0xFFFF;
			pL4KTable.Para0x0C.BitMap.ulL4K_BADR = ((AON_BASE + (ubL4KCount * 4096)) / 512);
			pL4KTable.Para0x08.BitMap.ubL4K_BUF_MASK = 0xFF;
			pL4KTable.Para0x0C.BitMap.ubL4K_ZCODE = 0;
			*pL4KTablePtr = pL4KTable;
		}
		gFQI.ubFQLinkNumber++;
		gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;

		while (gFQI.gFQLink[ubFQIndex].ubFQPhase != BYTE_FQ_WAITCHECK) {
			M_SwitchTask();
		}
		//if (FQ->btThisIsErasePage) { //沒被寫過
		//ftlPreformatShowError(ubCEIndex, gubSysCodeUnit[0], 0x21);
		//}
		gFQI.gFQLink[ubFQIndex].ubFQPhase = BYTE_FQ_CHECKDONE;

		pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (gFQI.gFQLink[ubFQIndex].ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));
		if (  ((pL4KTablePtr->ulL4K_LCA == HEADER_LCA) ) && (*((volatile unsigned char*)AON_BASE) == 'I')  && (*((volatile unsigned char*)(AON_BASE + 1)) == 'D')  ) {
			FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
			FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
			FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test); //InitFlashRegister給
			break;
		}
		//else { No VT in ram at this moment
		//	ftlRecordFailLog(FLHErrLogType, 0x2, FQ->ubL4kIndex, BYTE_UNC_READTABLE, BYTE_CODEUNIT, BIT_NormalErr);
		//}
		FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);//InitFlashRegister給
	}
#if Hynix
	FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
	FCONL[FCONL_FLH_FUNC] &= (~RDM_REF_ROW_ADR_5_TO_0);
	FCONL[FCONL_FLH_FUNC] |= ulAddressRegion;
}

#if TLC_BICS2
void flaBics4_Revision_Identify(void)
{
	M_CheckRDTModeToDisableFunction();

	if (!((guoFlashUseType.B.btToshibaBiCs4) && (gubDensitysize == FLH_Size_256Gb) && (!guoFlashUseType.B.btSandisk))) {
		return;
	}


	UWORD uwi;
	UBYTE ubEarlyCnt, ubFinalCnt, ubCEIndex, ubChannelIndex, ubFirstRevision = 0xFF;
	ULLONG uoStartStamp;

	for (ubCEIndex = 0; ubCEIndex < gubMTQTotalPhyCE; ubCEIndex++) {

		ubEarlyCnt = 0;
		ubFinalCnt = 0;

		ubChannelIndex = ubCEIndex % gubPlanesPerBurst;       //logical
		FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
		FCONL[FCONL_FCE_SET] = (ubChannelIndex * 8) + (ubCEIndex / gubPlanesPerBurst); //logical
		gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannelIndex & gubPlanesPerBurstMask) * 0x200));

		memset(BUFB_BASE, 0, 4096);

		gFREG[FCTLL_PIO_CMD].B.B0 = 0xFF;

		//Wait FF ready
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;
		uoStartStamp = GetRTT_micros();
		do {
			if (((GetRTT_micros() - uoStartStamp) > 1000) && (!(gulParamPageInfo.JEDEC.btNandSupplyByEmulator))) {
				guoCE_FailMap |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(ubCEIndex)));
				break;
			}
		}
		while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);

		gFREG[FCTLL_PIO_CMD].B.B0 = 0x5A;
		gFREG[FCTLL_PIO_CMD].B.B0 = 0xB5;
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;

		gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x38;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;

		gFREG[FCTLL_PIO_CMD].B.B0 = 0x30;

		gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;
		uoStartStamp = GetRTT_micros();
		do {
			if (((GetRTT_micros() - uoStartStamp) > 1000) && (!(gulParamPageInfo.JEDEC.btNandSupplyByEmulator))) {
				guoCE_FailMap |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(ubCEIndex)));
				break;
			}
		}
		while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);


		gFREG[FCTLL_PIO_CMD].B.B0 = 0x05;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
		gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
		gFREG[FCTLL_PIO_CMD].B.B0 = 0xE0;

		if (guoFlashUseType.B.btToggle) {
			for ( uwi = 0; uwi < 2048; uwi++) {
				guwTemp = gFREG[FCTLL_PIO_DAT].W.W0;
				BUFB_BASE[uwi * 2 + 0] = guwTemp;
				BUFB_BASE[uwi * 2 + 1] = guwTemp >> 8;
			}
		}
		else {
			for ( uwi = 0; uwi < 4096; uwi++) {
				guwTemp = gFREG[FCTLL_PIO_DAT].B.B0 ;
				BUFB_BASE[uwi] = guwTemp;
			}
		}

		gFREG[FCTLL_PIO_CMD].B.B0 = 0xFF;

		//Wait FF ready
		gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;
		uoStartStamp = GetRTT_micros();
		do {
			if (((GetRTT_micros() - uoStartStamp) > 1000) && (!(gulParamPageInfo.JEDEC.btNandSupplyByEmulator))) {
				guoCE_FailMap |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(ubCEIndex)));
				break;
			}
		}
		while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);

		for (uwi = 0; uwi < 8; uwi++) {
			if ((BUFB_BASE[0x4 + uwi * 0x200] == 0x43) && (BUFB_BASE[0x104 + uwi * 0x200] == 0xBC)) {
				ubEarlyCnt++; //early
			}
			else if ((BUFB_BASE[0x4 + uwi * 0x200] == 0x4E) && (BUFB_BASE[0x104 + uwi * 0x200] == 0xB1)) {
				ubFinalCnt++; //Final
			}
		}

		if (ubFirstRevision == 0xFF) {
			if (ubEarlyCnt > 5) {
				ubFirstRevision = 1; //early
			}
			else if (ubFinalCnt > 5) {
				ubFirstRevision = 2; //final
			}
			else {
				gubFlashTypeFail |= BIT0; //  有認不出TYPE的CE
				guwCE_FailMap2 |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(ubCEIndex)));
			}
		}
		else {
			if (((ubEarlyCnt > 5) && (ubFirstRevision == 2)) || ((ubFinalCnt > 5) && (ubFirstRevision == 1))) {
				gubFlashTypeFail |= BIT1; //  有混early 跟 final
				guwCE_FailMap2 |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(ubCEIndex)));
			}
			else if ((ubEarlyCnt <= 5) && (ubFinalCnt <= 5)) {
				gubFlashTypeFail |= BIT0;
				guwCE_FailMap2 |= (BIT0 << (TranslateLogicalCEtoPhysicalCE(ubCEIndex)));
			}

		}
	}

	if (gubFlashTypeFail & 0x3) {
		gRRTMode.B.btNoRevision = 1; //HV 有分不出

		if (!gRRTMode.B.btMPSet) {
			// ref: TSB BiCS4 256Gb cTLC RRT for Spec 636 Reduction (Non-early Final).xlsx
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 24; // uwGroupNumber[0] Fast   23+1
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];
			gRRTMode.B.btRelaxMode = 0;

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 97; // uwGroupNumber[1] Non-Fast  93+4
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
#if !BURNER
			memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_256Gb_HV, sizeof(gubRetryRTable_BISC4_256Gb_HV));
#endif
		}
	}
	else if (ubFirstRevision == 1) {
		gRRTMode.B.btRevision = 0; //early
	}
	else if (ubFirstRevision == 2) {
		gRRTMode.B.btRevision = 1; //Final

		if (!gRRTMode.B.btMPSet) {
			// ref: M2CMP01-599 Final BiCS4 256Gb TLC Read Retry with Set Feature rev1.0.pdf
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 24; // uwGroupNumber[0] Fast  23+1
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];
			gRRTMode.B.btRelaxMode = 0;

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 34; // uwGroupNumber[1] Non-Fast  30+4
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
#if !BURNER
			memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_256Gb_Final, sizeof(gubRetryRTable_BISC4_256Gb_Final));
#endif
		}
	}

	UBYTE ubi;
	for ( ubi = 0 ; ubi < HB_RETRY_Q_NUM ; ubi++ ) {
		gHBRetryQI[ubi].ubLinkLast = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] - 1) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] - 1);
		gHBRetryQI[ubi].ubLinkNum = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE]) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE]);
	}

}


void flaDisable_IPR(void)
{
	M_CheckRDTModeToDisableFunction();

	UBYTE ubCEIndex;
	UBYTE ubChannelIndex;
	UBYTE ubDie;
	U32 ulFL_FCTL_HS_MODE_All_Temp = FALLL[FCTLL_HS_MODE];
	U32 ulTemp[2];

	if ((guoFlashUseType.B.btToshibaBiCs3) && ((gubDensitysize == FLH_Size_512Gb) || ((guoFlashUseType.B.btSandisk) && (gubDensitysize == FLH_Size_256Gb)))) {

		for (ubChannelIndex = 0; ubChannelIndex < gubPlanesPerBurst; ubChannelIndex++) {
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannelIndex & gubPlanesPerBurstMask) * 0x200));
			ulTemp[ubChannelIndex] = gFREG[FCTLL_FLH_SET].L;
			gFREG[FCTLL_FLH_SET].L &= FLA_LEGACY_MODE;
			if (!ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG) {
				if (ASIC) {
					if (!gulParamPageInfo.JEDEC.btNandSupplyByEmulator) {
						gFREG[FCTLL_FDIV_CFG].L = (FLH_CLK_DIV_EN | gubClockDivValue);
						idle(1000);
					}
				}
			}
		}

		if (gubDensitysize == FLH_Size_512Gb) {
			for (ubCEIndex = 0; ubCEIndex < gubMTQTotalPhyCE; ubCEIndex++) {
				ubChannelIndex = ubCEIndex % gubPlanesPerBurst;       //logical
				FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
				FCONL[FCONL_FCE_SET] = (ubChannelIndex * 8) + (ubCEIndex / gubPlanesPerBurst); //logical

				gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannelIndex & gubPlanesPerBurstMask) * 0x200));

				// ====setting mode entry==== //
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x5C;
				gFREG[FCTLL_PIO_CMD].B.B0 = 0xC5;

				if (guoFlashUseType.B.btSandisk) {
					for (ubDie = 0; ubDie < (gubDieNumber * gubExpandCE_Ratio); ubDie++) {
						// ====parameter set to disable IPR==== //
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
						gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
						gFREG[FCTLL_PIO_CMD].B.B0 = (0xF1 + ubDie);
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x03;
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x40;
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x01;
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x06;
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x8E;
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x5F;
						// Delay
						idle(100);
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x57;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x8E;
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x00;
						// Delay
						idle(100);
					}
				}
				else {
					// ====parameter set to disable IPR==== //
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x8E;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					gFREG[FCTLL_PIO_DAT].B.B0 = 0xA4;

					// Delay
					idle(100);
				}
				// ====setting mode exit==== //
				gFREG[FCTLL_PIO_CMD].B.B0 = 0xFF;

				//Wait FF ready
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;
				while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);
			}

#if (!BURNER)
			if (guoFlashUseType.B.btSandisk) { // enable SB
				for (ubCEIndex = 0; ubCEIndex < gubMTQTotalPhyCE; ubCEIndex++) {
					ubChannelIndex = ubCEIndex % gubPlanesPerBurst;       //logical
					FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
					FCONL[FCONL_FCE_SET] = (ubChannelIndex * 8) + (ubCEIndex / gubPlanesPerBurst); //logical

					gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannelIndex & gubPlanesPerBurstMask) * 0x200));

					// ====setting mode entry==== //
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x5C;
					gFREG[FCTLL_PIO_CMD].B.B0 = 0xC5;

					for (ubDie = 0; ubDie < (gubDieNumber * gubExpandCE_Ratio); ubDie++) {
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
						gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
						gFREG[FCTLL_PIO_CMD].B.B0 = (0xF1 + ubDie);
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x03;
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x01;
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x06;
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x76;
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x5F;
						// Delay
						idle(100);
						gFREG[FCTLL_PIO_CMD].B.B0 = 0x57;
						gFREG[FCTLL_PIO_ADR].B.B0 = 0x76;
						gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
						// Delay
						idle(100);
					}

					// ====setting mode exit==== //
					gFREG[FCTLL_PIO_CMD].B.B0 = 0xFF;

					//Wait FF RBY
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;
					while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);
				}
			}
#endif
		}
		else if ((guoFlashUseType.B.btSandisk) && (gubDensitysize == FLH_Size_256Gb)) {
#if (!BURNER)
			for (ubCEIndex = 0; ubCEIndex < gubMTQTotalPhyCE; ubCEIndex++) {
				ubChannelIndex = ubCEIndex % gubPlanesPerBurst;       //logical
				FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
				FCONL[FCONL_FCE_SET] = (ubChannelIndex * 8) + (ubCEIndex / gubPlanesPerBurst); //logical

				gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannelIndex & gubPlanesPerBurstMask) * 0x200));

				// ====setting mode entry==== //
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x5C;
				gFREG[FCTLL_PIO_CMD].B.B0 = 0xC5;

				for (ubDie = 0; ubDie < (gubDieNumber * gubExpandCE_Ratio); ubDie++) {
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x00;
					gFREG[FCTLL_HS_MODE].L |= (PIO_DAT_FIRST | PIO_DAT_LAST);
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
					gFREG[FCTLL_PIO_CMD].B.B0 = (0xF1 + ubDie);
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x03;
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x55;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x01;
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x06;
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x00;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x73;
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x5F;
					// Delay
					idle(100);
					gFREG[FCTLL_PIO_CMD].B.B0 = 0x57;
					gFREG[FCTLL_PIO_ADR].B.B0 = 0x73;
					gFREG[FCTLL_PIO_DAT].B.B0 = 0x01;
					// Delay
					idle(100);
				}

				// ====setting mode exit==== //
				gFREG[FCTLL_PIO_CMD].B.B0 = 0xFF;

				//Wait FF ready
				gFREG[FCTLL_PIO_CMD].B.B0 = 0x70;
				while ( (gFREG[FCTLL_PIO_DAT].B.B0 & 0xF0 ) != 0xE0);
			}
#endif
		}

		FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
		FALLL[FCTLL_HS_MODE] = ulFL_FCTL_HS_MODE_All_Temp;

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

	}
}

#endif

void flaDLLCDCIssueWorkaround(U32 ulDLLRegOffset, U32 ulVal)
{
	//2018 PS8225_20180623_DLL_REG_CDC_BUG workaround
	FDLLL[ulDLLRegOffset] = ulVal;
	/*
	200/200
	DLL_CLK = 300以下 ; Flash Clk = 150以下
	ceil((floor(SYS_CLK*2/DLL_CLK))/(SYS_CLK/CPU_CLK))
	*/
	//FLH_CLK_100MHz -> 4 Nop
	//FLH_CLK_83MHz -> 4 Nop
	//FLH_CLK_50MHz -> 8 Nop
	//FLH_CLK_40MHz -> 10 Nop
	//FLH_CLK_10MHz -> 40 Nop
	//40個nop太浪費code size, 用loop的delay又會大於nop.. code size比較重要, 加上nop時間本來就是可多不可少, 故改用loop處理

	volatile U32 ubNopLoop = 0;
	//volatile UBYTE ubi = 0;
	if (gubDLLFlashClock == FLH_CLK_100MHz || gubDLLFlashClock == FLH_CLK_83MHz) {
		ubNopLoop = 4;
	}
	else if (gubDLLFlashClock == FLH_CLK_50MHz) {
		ubNopLoop = 8;
	}
	else if (gubDLLFlashClock == FLH_CLK_40MHz) {
		ubNopLoop = 10;
	}
	else if (gubDLLFlashClock == FLH_CLK_10MHz) {
		ubNopLoop = 40;
	}

	for (; ubNopLoop > 0; ubNopLoop--) {
	}

}

