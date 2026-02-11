#include "Setup.h"
#include "DDR.h"
#include "Vardef.h"
#include "IO.h"
#include "global/_types.h"
#include "global/_global.h"
#include "FTL.h"

void idle (volatile unsigned long len)
{
	volatile unsigned long i, j;

	for (i = 0; i < len; i = i + 1 ) {
		j = i;
	}
}
unsigned char ddr_phy_init(unsigned char ddr_type)
{

	M_CheckRDTModeToDisableFunctionReturnValid(0);
#if(!RDT)
	if (ddr_type == SDR)
		*((volatile unsigned char*)(0x04000319)) = 0x80;//select mode=SDR
	else
		*((volatile unsigned char*)(0x04000319)) = 0x40; //set ddr type = DDR3/MRAM

	dDDR(0x468) = 0x80c34ac8;
	idle(10);
	dDDR(0x478) = 0x0ccfc000;
	idle(10);
	dDDR(0x468) = 0x80c36ac8;
	idle(10);
	dDDR(0x468) = 0x80c3eac8;
	idle(10);

	while ( (dDDR(0x46c) & 0x00000004) != 0x00000004) { //wait pll lock
		if (RTT1_GetValue() > TO_DDR) {
			return RET_TIMEOUT;
		}
	}
	dDDR(0x468) = 0xa0c3eac8;   //DLL_RSTB              =CFG46[29];

	if (ddr_type == SDR)
		idle(100);
	else
		while ( (dDDR(0x46c) & 0x00000002) != 0x00000002) { //wait dll lock
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
	dDDR(0x468) = 0xe0c3eac8;
	idle(100);
	dDDR(0x468) = 0x60c3eac8;
	idle(100);

	while ( (dDDR(0x200) & 0x00000040) != 0x00000040) { //wait CR_DFIINITCOMPLETE
		if (RTT1_GetValue() > TO_DDR) {
			return RET_TIMEOUT;
		}
	}

	//	dDDR(0x45C) = 0x00000ff8;
	dDDR(0x45C) = 0x000383e0;
	idle(10);
	return RET_OK;
#endif
}

unsigned char ddr_controller_init(unsigned char ddr_type, unsigned char ddr_size, unsigned char ddr_bank, unsigned char ubSetAgain)
{

	M_CheckRDTModeToDisableFunctionReturnValid(0);
#if(!RDT)
	if (ubSetAgain) {

		goto SET_AGAIN_REGION;
	}

	//Controller initial=========================================================
	while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
		if (RTT1_GetValue() > TO_DDR) {
			return RET_TIMEOUT;
		}
	}
	dDDR(0x010) = 0x00000200; //set RESETB=1
	idle(200);
	while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
		if (RTT1_GetValue() > TO_DDR) {
			return RET_TIMEOUT;
		}
	}
	dDDR(0x010) = 0x00000020; //set CKE=1
	//note: So far, not yet to add size select for SDRAM

SET_AGAIN_REGION:
	if (ddr_type == MRAM)
		dDDR(0x018) = 0x05240000; // set ROW=14bit COL=5bit
	else if (ddr_type == SDR) {
		if (ddr_size == SIZE_32MB)
			//dDDR(0x018)=0x45110000; // set ROW=13bit COL=9bit
			dDDR(0x018) = 0x44910000; // set ROW=13bit COL=9bit   //{COARSE[1:0],fine}=[25:23]=3'b001
		else if (ddr_size == SIZE_16MB)
			//dDDR(0x018)=0x45010000; // set ROW=12bit COL=9bit
			dDDR(0x018) = 0x44810000; // set ROW=12bit COL=9bit   //{COARSE[1:0],fine}=[25:23]=3'b001
		else //(ddr_size==SIZE_8MB)
			//dDDR(0x018)=0x45000000; // set ROW=12bit COL=8bit
			dDDR(0x018) = 0x44800000; // set ROW=12bit COL=8bit   //{COARSE[1:0],fine}=[25:23]=3'b001
	}
	else if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		if (ddr_bank == 1) { //4 bank
			if (ddr_size == SIZE_512MB)
				dDDR(0x018) = 0x45420000; // set ROW=16bit COL=10bit
			else if (ddr_size == SIZE_256MB)
				dDDR(0x018) = 0x45320000; // set ROW=15bit COL=10bit
			else //(ddr_size=SIZE_128MB)
				dDDR(0x018) = 0x45220000; // set ROW=14bit COL=10bit
		}
		else { // 8 bank
			if (ddr_size == SIZE_512MB)
				dDDR(0x018) = 0x05320000; // set ROW=15bit COL=10bit
			else if (ddr_size == SIZE_256MB)
				dDDR(0x018) = 0x05220000; // set ROW=14bit COL=10bit
			else //(ddr_size=SIZE_128MB)
				dDDR(0x018) = 0x05120000; // set ROW=13bit COL=10bit
		}
	}
	if (ubSetAgain) {

		goto SET_AGAIN_REGION_2;
	}
	idle(50);
	dDDR(0x028) = 0x01490000; // set BL8 16bit
	idle(50);
	dDDR(0x00c) = 0x0000b144; // R2W_DIFRK=11,TWTR_AL_DIFRK=20,TCCD_DIFRK=4
	idle(50);
SET_AGAIN_REGION_2:
	if (ddr_type == SDR)
		dDDR(0x014) = 0x09102233;
	else if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		if (ddr_bank == 1) { //4 bank
			if (ddr_size == SIZE_512MB)
				dDDR(0x014) = 0x1bffff88; // TRTP=4,TRAS=1b,TRFC=ff TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else if (ddr_size == SIZE_256MB)
				dDDR(0x014) = 0x1b8cff88; // TRTP=4,TRAS=1b,TRFC=8c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else
				dDDR(0x014) = 0x1b57ff88; // TRTP=4,TRAS=1b,TRFC=57 TWTR_AL=f,TWR=f,TRP=8,TRCD=8
		}
		else {
			if (ddr_size == SIZE_512MB)
				dDDR(0x014) = 0x1b8cff88; // TRTP=4,TRAS=1b,TRFC=8c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else if (ddr_size == SIZE_256MB)
				dDDR(0x014) = 0x1b57ff88; // TRTP=4,TRAS=1b,TRFC=57 TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else
				dDDR(0x014) = 0x1b3cff88; // TRTP=4,TRAS=1b,TRFC=3c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
		}
	}

	if (ubSetAgain) {
		goto SET_AGAIN_REGION_3;
	}
	idle(50);
	dDDR(0x01c) = 0x00007080; //TFAW=40, R2W=8

SET_AGAIN_REGION_3:
	return RET_OK;
#endif
}

void Set_DDR3_DQS_G(unsigned char Index_Gate_DQS0, unsigned char Index_Gate_DQS1)
{
	bDDR(0x47c) &= (0x88);
	bDDR(0x47c) |= 0x01 * (Index_Gate_DQS0);
	bDDR(0x47c) |= 0x10 * (Index_Gate_DQS1);
}

void Set_DDR3_DQS_Fine(unsigned short DQS0_fine, unsigned short DQS1_fine)
{
	M_CheckRDTModeToDisableFunction();
	unsigned short uwtemp1 = 0;
	unsigned short uwtemp2 = 0;
	unsigned long ultemp = 0;

	//	unsigned char CheckDQS0Fine0 = 0;
	unsigned char CheckDQS0Fine1 = 0;
	//	unsigned char CheckDQS1Fine0 = 0;
	//	unsigned char CheckDQS1Fine1 = 0;

	//// clean DQSx_finex
	dDDR(0x44C) &= 0xF03FFFFF;
	dDDR(0x42C) &= 0xF00F3FFF;

	////////// Fill in DQS0_FINE1
	if ((DQS0_fine) < 8) {
		////DOS0_FINE1
	}
	else if (((DQS0_fine) > 7) && ((DQS0_fine) < 21)) {
		////DOS0_FINE1		////Change DQS0_fine1 = 7
		uwtemp2 |= 0x0540;
	}
	else {
		////DOS0_FINE1     		 ////Change DQS0_fine1 = 13
		uwtemp2 |= 0x0440;
		dDDR(0x42c) |= 0x00004000;
	}

	/////////Fill in DQS1_FINE1
	if ((DQS1_fine) < 8) {
		////DOS1_FINE1
	}
	else if (((DQS1_fine) > 7) && ((DQS1_fine) < 21)) {
		////DOS1_FINE1 		////Change DQS1_fine1 = 7
		uwtemp2 |= 0x0A80;
		//dDDR(0x42C) &= 0xFFFF7FFF;
	}
	else {
		////DOS1_FINE1            	////change DQS1_fine1 = 13   1101
		uwtemp2 |= 0x0880;
		dDDR(0x42c) |= 0x00008000;
	}

	ultemp = (U32)(uwtemp2 << 16) ;
	dDDR(0x44c) |= ultemp;
	gul_temp1 = ultemp;

	//////////clean DQS1_FINE0
	dDDR(0x42C) &= 0xF00FFFFF;

	if ((DQS1_fine) < 8) {
		////DOS1_FINE0
		uwtemp1  |= (((DQS1_fine) & 0x1) << 5) | (((DQS1_fine) & 0x2) << 6) | (((DQS1_fine) & 0x4) << 7) | (((DQS1_fine) & 0x8) << 8);
	}
	else if (((DQS1_fine) > 7) && ((DQS1_fine) < 21)) {
		////DOS1_FINE0
		uwtemp1 |= ((((DQS1_fine) - 7) & 0x1) << 5) | ((((DQS1_fine) - 7) & 0x2) << 6) | ((((DQS1_fine) - 7) & 0x4) << 7) | ((((DQS1_fine) - 7) & 0x8) << 8);
	}
	else if (((DQS1_fine) > 26)) {
		////DOS1_FINE0
		uwtemp1 |= (((13) & 0x1) << 5) | (((13) & 0x2) << 6) | (((13) & 0x4) << 7) | (((13) & 0x8) << 8);
	}
	else {
		////DOS1_FINE0
		uwtemp1 |= ((((DQS1_fine) - 13) & 0x1) << 5) | ((((DQS1_fine) - 13) & 0x2) << 6) | ((((DQS1_fine) - 13) & 0x4) << 7) | ((((DQS1_fine) - 13) & 0x8) << 8);
	}

	//////////Fill in DQS0_FINE0

	if ((DQS0_fine) < 8) {
		////DOS0_FINE0
		uwtemp1  |= (((DQS0_fine) & 0x1) << 4) | (((DQS0_fine) & 0x2) << 5) | (((DQS0_fine) & 0x4) << 6) | (((DQS0_fine) & 0x8) << 7);
	}
	else if (((DQS0_fine) > 7) && ((DQS0_fine) < 21)) {
		////DOS0_FINE0
		uwtemp1  |= ((((DQS0_fine) - 7) & 0x1) << 4) | ((((DQS0_fine) - 7) & 0x2) << 5) | ((((DQS0_fine) - 7) & 0x4) << 6) | ((((DQS0_fine) - 7) & 0x8) << 7);
	}
	else if ((DQS0_fine) > 26) {
		////DOS0_FINE0
		uwtemp1 |= (((13) & 0x1) << 4) | (((13) & 0x2) << 5) | (((13) & 0x4) << 6) | (((13) & 0x8) << 7);
	}
	else {
		////DOS0_FINE0
		uwtemp1 |= ((((DQS0_fine) - 13) & 0x1) << 4) | ((((DQS0_fine) - 13) & 0x2) << 5) | ((((DQS0_fine) - 13) & 0x4) << 6) | ((((DQS0_fine) - 13) & 0x8) << 7);
	}
	///// Fill in DQS1_FINE0 & DQS0_FINE0
	dDDR(0x42C) |= ((uwtemp1 << 16) & 0xFFFF0000);
	gul_temp2 = ((uwtemp1 << 16) & 0xFFFF0000);

	//CheckDQS0Fine1 = (unsigned char)(((*((unsigned long*)(0x0400542c))&_bit14) >> (14-3) )  |  ((*((unsigned long*)(0x0400544c))&_bit26) >> (26-2) ) | ((*((unsigned long*)(0x0400544c))&_bit24) >> (24-1) ) | (((*((volatile unsigned long*)(0x0400544c))&_bit22))>>22) )  ;
	//CheckDQS0Fine0 = (unsigned char)(((*((unsigned long*)(0x0400542c))&_bit26) >> (26-3) )  |  ((*((unsigned long*)(0x0400542c))&_bit24) >> (24-2) ) | ((*((unsigned long*)(0x0400542c))&_bit22) >> (22-1) ) | (((*((volatile unsigned long*)(0x0400542c))&_bit20))>>20) )  ;
	//CheckDQS1Fine1 = (unsigned char)(((*((unsigned long*)(0x0400542c))&_bit15) >> (15-3) )  |  ((*((unsigned long*)(0x0400544c))&_bit27) >> (27-2) ) | ((*((unsigned long*)(0x0400544c))&_bit25) >> (25-1) ) | (((*((volatile unsigned long*)(0x0400544c))&_bit23))>>23) )  ;
	//CheckDQS1Fine0 = (unsigned char)(((*((unsigned long*)(0x0400542c))&_bit27) >> (27-3) )  |  ((*((unsigned long*)(0x0400542c))&_bit25) >> (25-2) ) | ((*((unsigned long*)(0x0400542c))&_bit23) >> (23-1)) | (((*((volatile unsigned long*)(0x0400542c))&_bit21))>>21) )  ;


	if (CheckDQS0Fine1);
	//	Uart_PutByteVal(CheckDQS0Fine1);
	//	Uart_PutString("   ");
	//	Uart_PutByteVal(CheckDQS0Fine0);
	//	Uart_PutString("   ");
	//	Uart_PutByteVal(CheckDQS1Fine1);
	//	Uart_PutString("   ");
	//	Uart_PutByteVal(CheckDQS1Fine0);
	//	Uart_PutString("   \r\n");

}

void Auto_Refresh(unsigned char ddr_type)
{

	dDDR(0x004) = 0x00090018;   // =24*16*5ns =1920ns
	dDDR(0x000) = 0x00000001;

}


unsigned char MRS_Setting(unsigned char ddr_type, unsigned char ddr_size)
{

	M_CheckRDTModeToDisableFunctionReturnValid(0);
#if(!RDT)
	if (ddr_type == SDR) {
		while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
		dDDR(0x010) = 0x00000004; //PRA
		while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
		dDDR(0x010) = 0x00000001; //AutoRefresh
		while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
		dDDR(0x010) = 0x00000001; //AutoRefresh
		while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
		dDDR(0x010) = 0x00330002; //MR0 CAS latency=3
	}
	else {
		while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
		//dDDR(0x010)=0x00002002; //MR2,CWL=5
		if (ddr_type == DDR3_400)
			dDDR(0x010) = 0x00002002; //MR2,CWL=5
		else
			dDDR(0x010) = 0x00082002; //MR2,CWL=6
		//dDDR(0x010)=0x00102002; //MR2,CWL=7

		while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
		dDDR(0x010) = 0x00003002; //MR3
		while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
		//dDDR(0x010)=0x00041002; //MR1
		dDDR(0x010) = 0x00041002; //MR1

		while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
		if (ddr_type == DDR3_400)
			dDDR(0x010) = 0x19200002; //MR0 CAS latency=6
		else
			dDDR(0x010) = 0x19400002; //MR0 CAS latency=8
		//			dDDR(0x010)=0x19300002; //MR0 CAS latency=7
		//dDDR(0x010)=0x1b500002; //MR0 CAS latency=9 WR=10

		while ( (dDDR(0x010) & 0x000003ff) != 0x00000000) { //wait ddrcmdfree
			if (RTT1_GetValue() > TO_DDR) {
				return RET_TIMEOUT;
			}
		}
		dDDR(0x010) = 0x00000040; //MR0
		idle(200);
	}

	return RET_OK;
#endif
}

#if (BURNER||RDT)
//////////////////////////////////////////////////////
unsigned char ScanDelayLine_DDR3(void)
{
	unsigned short Matrix[8][4];
	unsigned char Matrix_detail[8][2][27];
	unsigned short ultemp1, counterDQSG0, counterDQSG1;
	unsigned char ubDQS_G;
	unsigned short uwDQS_Fine;
	unsigned short Max_Counter_DQS0_G, Max_Counter_DQS1_G, Max_Index_DQS0_G, Max_Index_DQS1_G, Counter_DQS0, Counter_DQS1, Index_DQS0, Index_DQS1, Max_Counter_DQS0,
	         Max_Counter_DQS1, Max_Index_DQS0, Max_Index_DQS1, Index_Gate_DQS0, Index_Gate_DQS1;


	unsigned char ubtemp, ubBISTResult;

	dDDR(0x47c) &= (0xffffff88);      //Clean DQS_G0 & DQS_G1

	Max_Counter_DQS0_G = 0;
	Max_Index_DQS0_G = 0;

	Max_Counter_DQS1_G = 0;
	Max_Index_DQS1_G = 0;

	for (ubDQS_G = 0; ubDQS_G < 8; ubDQS_G++) {
		//wDDR(0x1A) &= ~(0x0380);
		//wDDR(0x1A) |= ((ubCorase<<7) & 0x0380);
		dDDR(0x47c) &= (0xffffff88);      //Clean DQS_G0 & DQS_G1
		dDDR(0x47c) |= (0x00000011 * ubDQS_G); // add DQS_G0 & G1


		dDDR(0x42C) &= 0xF00F3FFF;   // Clean all DQS0_Fine
		dDDR(0x44c) &= 0xF03FFFFF;       // Clean all DQS0_Fine

		//DQS0_fine1 : 0x42c[14] 0x44c[26]  0x44c[24] 0x44c[22]  0.7.13
		//DQS0_fine0 :  0x42c[26]	0x42c[24]	0x42c[22]	0x42c[20]
		//DQS1_fine1 : 0x42c[15] 0x44c[27]  0x44c[25] 0x44c[23]   0,7,13
		//DQS1_fine0 :  0x42c[27]	0x42c[25]	0x42c[23]	0x42c[21]

		Counter_DQS0 = 0;
		Counter_DQS1 = 0;
		Index_DQS0 = 0;
		Index_DQS1 = 0;
		Max_Counter_DQS0 = 0;
		Max_Counter_DQS1 = 0;
		Max_Index_DQS0 = 0;
		Max_Index_DQS1 = 0;
		Index_Gate_DQS0 = 0;
		Index_Gate_DQS1 = 0;

		for (uwDQS_Fine = 0; uwDQS_Fine < 27; uwDQS_Fine++) {

			///For Debug
			Matrix_detail[ubDQS_G][0][uwDQS_Fine] = 0;
			Matrix_detail[ubDQS_G][1][uwDQS_Fine] = 0;

			//////////////////////////////////// change DQS0_fine0&1
			Set_DDR3_DQS_Fine(uwDQS_Fine, uwDQS_Fine);

			//////////// Following is data training with BIST& DDR W/R
			ubBISTResult = 0;
			for (ubtemp = 0; ubtemp < 3; ubtemp++) {
				if (ubtemp < 2) {
					ubBISTResult = BIST_ForScanDelayLine((0x60000000 + ubtemp * 0x400), 0x400, ubtemp);
					//					if(ubTest==1)
					//						{ubBISTResult = BIST_ForScanDelayLine((0x60000000 + ubtemp * 0x400), 0x400, ubtemp);}
					//					while(( *((volatile unsigned long*)0x04005028) & 0x200  )==0);

					if (ubBISTResult == 1)
						goto Result;
				}
				else {
					ubBISTResult = CheckData_DDR3((U32*)(0x60000000 + ubtemp * 0x400), 0x400);
					//					ubBISTResult = BIST_ForScanDelayLine((0x60000000 + ubtemp * 0x400), 0x400, ubtemp);
					//					ubBISTResult = BIST_ForScanDelayLine((0x60000000), 0x400, ubtemp);
					if (ubBISTResult == 1)
						goto Result;
				}
			}

		Result:
			switch (ubBISTResult) {
				case 0x1:  //all fail
					////For Debug
					Matrix_detail[ubDQS_G][0][uwDQS_Fine] = 1;
					Matrix_detail[ubDQS_G][1][uwDQS_Fine] = 1;

					if (Counter_DQS0 > Max_Counter_DQS0) {
						Max_Counter_DQS0 = Counter_DQS0;
						Max_Index_DQS0 = Index_DQS0;
						Index_Gate_DQS0 = ubDQS_G;
					}
					Counter_DQS0 = 0;
					Index_DQS0 = 0;


					if (Counter_DQS1 > Max_Counter_DQS1) {
						Max_Counter_DQS1 = Counter_DQS1;
						Max_Index_DQS1 = Index_DQS1;
						Index_Gate_DQS1 = ubDQS_G;
					}
					Counter_DQS1 = 0;
					Index_DQS1 = 0;

					break;
				case 0x00:     //Success
					////For Debug
					Matrix_detail[ubDQS_G][0][uwDQS_Fine] = 0;
					Matrix_detail[ubDQS_G][1][uwDQS_Fine] = 0;

					Counter_DQS0++;
					if (Counter_DQS0 == 1) {
						Index_DQS0 = uwDQS_Fine;
					}

					if (uwDQS_Fine == 26) {
						if (Counter_DQS0 > Max_Counter_DQS0) {
							Max_Counter_DQS0 = Counter_DQS0;
							Max_Index_DQS0 = Index_DQS0;
							Counter_DQS0 = 0;
							Index_DQS0 = 0;
							Index_Gate_DQS0 = ubDQS_G;
						}
					}

					Counter_DQS1++;

					if (Counter_DQS1 == 1) {
						Index_DQS1 = uwDQS_Fine;
					}

					if (uwDQS_Fine == 26) {
						if (Counter_DQS1 > Max_Counter_DQS1) {
							Max_Counter_DQS1 = Counter_DQS1;
							Max_Index_DQS1 = Index_DQS1;
							Counter_DQS1 = 0;
							Index_DQS1 = 0;
							Index_Gate_DQS1 = ubDQS_G;
						}
					}
					break;
				default:
					break;
			}
		}//Second loop end
		/////////////////////////// Changed here
		//
		//			switch (CheckData_DDR3(0x60000000, 0x10)) {
		//				case 0x00:  //all fail
		//					////For Debug
		//					Matrix_detail[ubDQS_G][0][uwDQS_Fine] = 1;
		//					Matrix_detail[ubDQS_G][1][uwDQS_Fine] = 1;
		//
		//					if (Counter_DQS0 > Max_Counter_DQS0) {
		//						Max_Counter_DQS0 = Counter_DQS0;
		//						Max_Index_DQS0 = Index_DQS0;
		//						Index_Gate_DQS0 = ubDQS_G;
		//					}
		//					Counter_DQS0 = 0;
		//					Index_DQS0 = 0;
		//
		//
		//					if (Counter_DQS1 > Max_Counter_DQS1) {
		//						Max_Counter_DQS1 = Counter_DQS1;
		//						Max_Index_DQS1 = Index_DQS1;
		//						Index_Gate_DQS1 = ubDQS_G;
		//					}
		//					Counter_DQS1 = 0;
		//					Index_DQS1 = 0;
		//
		//					break;
		//				case 0x1:   //only DQS1 fail
		//					////For Debug
		//					Matrix_detail[ubDQS_G][0][uwDQS_Fine] = 0;
		//					Matrix_detail[ubDQS_G][1][uwDQS_Fine] = 1;
		//
		//					Counter_DQS0++;
		//					if (Counter_DQS0 == 1) {
		//						Index_DQS0 = uwDQS_Fine;
		//					}
		//
		//					if (uwDQS_Fine == 26) {
		//						if (Counter_DQS0 > Max_Counter_DQS0) {
		//							Max_Counter_DQS0 = Counter_DQS0;
		//							Max_Index_DQS0 = Index_DQS0;
		//							Index_Gate_DQS0 = ubDQS_G;
		//							Counter_DQS0 = 0;
		//							Index_DQS0 = 0;
		//						}
		//					}
		//
		//					if (Counter_DQS1 > Max_Counter_DQS1) {
		//						Max_Counter_DQS1 = Counter_DQS1;
		//						Max_Index_DQS1 = Index_DQS1;
		//						Index_Gate_DQS1 = ubDQS_G;
		//					}
		//					Counter_DQS1 = 0;
		//					Index_DQS1 = 0;
		//					break;
		//				case 0x2:    //only DQS0 fail
		//					////For Debug
		//					Matrix_detail[ubDQS_G][0][uwDQS_Fine] = 1;
		//					Matrix_detail[ubDQS_G][1][uwDQS_Fine] = 0;
		//
		//					if (Counter_DQS0 > Max_Counter_DQS0) {
		//						Max_Counter_DQS0 = Counter_DQS0;
		//						Max_Index_DQS0 = Index_DQS0;
		//						Index_Gate_DQS0 = ubDQS_G;
		//					}
		//					Counter_DQS0 = 0;
		//					Index_DQS0 = 0;
		//
		//					Counter_DQS1++;
		//					if (Counter_DQS1 == 1) {
		//						Index_DQS1 = uwDQS_Fine;
		//					}
		//
		//					if (uwDQS_Fine == 26) {
		//						if (Counter_DQS1 > Max_Counter_DQS1) {
		//							Max_Counter_DQS1 = Counter_DQS1;
		//							Max_Index_DQS1 = Index_DQS1;
		//							Index_Gate_DQS1 = ubDQS_G;
		//							Counter_DQS1 = 0;
		//							Index_DQS1 = 0;
		//						}
		//					}
		//					break;
		//				case 0x03:
		//					////For Debug
		//					Matrix_detail[ubDQS_G][0][uwDQS_Fine] = 0;
		//					Matrix_detail[ubDQS_G][1][uwDQS_Fine] = 0;
		//
		//					Counter_DQS0++;
		//					if (Counter_DQS0 == 1) {
		//						Index_DQS0 = uwDQS_Fine;
		//					}
		//
		//					if (uwDQS_Fine == 26) {
		//						if (Counter_DQS0 > Max_Counter_DQS0) {
		//							Max_Counter_DQS0 = Counter_DQS0;
		//							Max_Index_DQS0 = Index_DQS0;
		//							Counter_DQS0 = 0;
		//							Index_DQS0 = 0;
		//							Index_Gate_DQS0 = ubDQS_G;
		//						}
		//					}
		//
		//					Counter_DQS1++;
		//
		//					if (Counter_DQS1 == 1) {
		//						Index_DQS1 = uwDQS_Fine;
		//					}
		//
		//					if (uwDQS_Fine == 26) {
		//						if (Counter_DQS1 > Max_Counter_DQS1) {
		//							Max_Counter_DQS1 = Counter_DQS1;
		//							Max_Index_DQS1 = Index_DQS1;
		//							Counter_DQS1 = 0;
		//							Index_DQS1 = 0;
		//							Index_Gate_DQS1 = ubDQS_G;
		//						}
		//					}
		//					break;
		//				default:
		//					break;
		//			}
		//		}//Second loop end, uwfine set

		//////////////////// Changed end here

		Matrix[ubDQS_G][0] = Max_Index_DQS0;
		Matrix[ubDQS_G][1] = Max_Counter_DQS0;
		Matrix[ubDQS_G][2] = Max_Index_DQS1;
		Matrix[ubDQS_G][3] = Max_Counter_DQS1;
	}//First loop end

	counterDQSG0 = 0;
	counterDQSG1 = 0;
	for (ultemp1 = 0; ultemp1 < 8; ultemp1++) {

		if (ultemp1 == 0) {
			Max_Counter_DQS0 = Matrix[ultemp1][1];
			Max_Index_DQS0 = Matrix[ultemp1][0];
			Max_Counter_DQS1 = Matrix[ultemp1][3];
			Max_Index_DQS1 = Matrix[ultemp1][2];
			Index_Gate_DQS0 = 0;
			Index_Gate_DQS1 = 0;
		}
		else {
			if (Max_Counter_DQS0 == Matrix[ultemp1][1]) {
				counterDQSG0++;
				if (counterDQSG0 == 1) {
					Max_Index_DQS0 = Matrix[ultemp1][0];
					Index_Gate_DQS0 = ultemp1;
				}
			}
			if (Max_Counter_DQS0 < Matrix[ultemp1][1]) {
				Max_Counter_DQS0 = Matrix[ultemp1][1];
				Max_Index_DQS0 = Matrix[ultemp1][0];
				counterDQSG0 = 0;
				Index_Gate_DQS0 = ultemp1;
			}


			if (Max_Counter_DQS1 == Matrix[ultemp1][3]) {
				counterDQSG1++;
				if (counterDQSG1 == 1) {
					Max_Index_DQS1 = Matrix[ultemp1][2];
					Index_Gate_DQS1 = ultemp1;
				}
			}
			if (Max_Counter_DQS1 < Matrix[ultemp1][3]) {
				Max_Counter_DQS1 = Matrix[ultemp1][3];
				Max_Index_DQS1 = Matrix[ultemp1][2];
				counterDQSG1 = 0;
				Index_Gate_DQS1 = ultemp1;
			}
		}
	}

	if ((Max_Counter_DQS1 == 0) || (Max_Counter_DQS0 == 0)) {
		SetInitialFailID(BIT15, 0x02);
		return RET_ERR;
	}

	////insert MDQS0_G, DQS1_G ,DQS0_Fine, DQS1_Fine
	Set_DDR3_DQS_G(Index_Gate_DQS0, Index_Gate_DQS1);

	//// SET DQS_FINE0&1 (include clean DQSx_finex)
	Set_DDR3_DQS_Fine((Max_Index_DQS0 + (Max_Counter_DQS0 / 2)), (Max_Index_DQS1 + (Max_Counter_DQS1 / 2)));

	gubDQS0_G = Index_Gate_DQS0;
	gubDQS1_G = Index_Gate_DQS1;
	guwDQS0_Fine = (Max_Index_DQS0 + (Max_Counter_DQS0 / 2));
	guwDQS1_Fine = (Max_Index_DQS1 + (Max_Counter_DQS1 / 2));

	return RET_OK;
}


unsigned char CheckData_DDR3(unsigned long* uladdress, unsigned long ulZ1size)
{
	unsigned long ulTestPattern[8] = {0x55555555, 0x0000ffff, 0xC36969C3, 0xAAAA5555, 0xf0f0f0f0, 0x5a5aa5a5, 0x69c3c369, 0xffff0000};
	unsigned long uwFillNumber;
	unsigned char FailDQS0, FailDQS1 = 0;

	FailDQS0 = 0x0;
	FailDQS1 = 0x0;

	//	for (ubTestCase = 0; ubTestCase < 8; ubTestCase++) {
	//fill
	for (uwFillNumber = 0; uwFillNumber < (ulZ1size >> 2); uwFillNumber++) {
		//			uladdress[uwFillNumber + ((ulZ1size >> 2))] = ulTestPattern[(uwFillNumber%8)];
		uladdress[uwFillNumber] = ulTestPattern[(uwFillNumber % 8)];
	}
	//check
	//#pragma flush
	for (uwFillNumber = 0; uwFillNumber < (ulZ1size >> 2); uwFillNumber++) {
		//			if ((uladdress[uwFillNumber + ((ulZ1size >> 2))] & 0xFF00FF00) != (ulTestPattern[(uwFillNumber%8)] & 0xFF00FF00)) {
		if ((uladdress[uwFillNumber] & 0xFF00FF00) != (ulTestPattern[(uwFillNumber % 8)] & 0xFF00FF00)) {
			FailDQS1 = 1;
			//
			//				Uart_PutString("1    ");
			//				Uart_PutWordVal(uwFillNumber);
			//				Uart_PutString("   ");
			//				Uart_PutLongVal((uladdress[uwFillNumber + ((ulZ1size >> 2)*ubTestCase)] & 0xFF00FF00));
			//				Uart_PutString("   ");
			//				Uart_PutLongVal((ulTestPattern[ubTestCase] & 0xFF00FF00));
			//				Uart_PutString("   \r\n");

		}
		//			if ((uladdress[uwFillNumber + ((ulZ1size >> 2))] & 0x00FF00FF) != (ulTestPattern[(uwFillNumber%8)] & 0x00FF00FF)) {
		if ((uladdress[uwFillNumber] & 0x00FF00FF) != (ulTestPattern[(uwFillNumber % 8)] & 0x00FF00FF)) {
			FailDQS0 = 1;
			//
			//				Uart_PutString("0    ");
			//				Uart_PutWordVal(uwFillNumber);
			//				Uart_PutString("   ");
			//				Uart_PutLongVal((uladdress[uwFillNumber + ((ulZ1size >> 2)*ubTestCase)] & 0x00FF00FF));
			//				Uart_PutString("   ");
			//				Uart_PutLongVal((ulTestPattern[ubTestCase] & 0x00FF00FF));
			//				Uart_PutString("   \r\n");
		}
	}
	//	} // end of ubTestCase loop



	if ((FailDQS1 == 1) || (FailDQS0 == 1)) {
		return 0x1;
	}
	else {
		return 0x0;
	}

	//	if (FailDQS1 == 1) {
	//		if (FailDQS0 == 1) {
	//			return 0x0;   //all fail
	//		}
	//		else {
	//			return 0x1;
	//		}
	//	}
	//	else {
	//		if (FailDQS0 == 1) {
	//			return 0x2;
	//		}
	//		else {
	//			return 0x3;
	//		}   //ALL OK
	//	}

}

unsigned char CheckData_SDR(unsigned long* uladdress, unsigned long ulZ1size)
{
	volatile unsigned long ulTestPattern[8] = {0x55555555, 0x0000ffff, 0xC36969C3, 0xAAAA5555, 0xf0f0f0f0, 0x5a5aa5a5, 0x69c3c369, 0xffff0000};
	volatile unsigned long ulFillNumber;

	//	for (ubTestCase = 0; ubTestCase < 8; ubTestCase++) {
	//fill
	for (ulFillNumber = 0; ulFillNumber < (ulZ1size >> 2); ulFillNumber++) {
		uladdress[ulFillNumber + ((ulZ1size >> 2))] = ulTestPattern[(ulFillNumber % 8)];
	}
	//check
	//#pragma flush
	for (ulFillNumber = 0; ulFillNumber < (ulZ1size >> 2); ulFillNumber++) {
		if (uladdress[ulFillNumber + ((ulZ1size >> 2))] != ulTestPattern[(ulFillNumber % 8)]) {
			return 1;
		}
	}
	//	}
	return 0;
}

unsigned char ScanDelayLine_SDR(void)
{
	UBYTE ubCorase;
	UWORD uwFine;
	UWORD uwSuccess;
	UWORD uwSuccessCount;
	UBYTE ubHadSucceed;
	ULONG ultemp1, ultemp2;
	UBYTE ubBISTResult;
	UBYTE ubtemp;

	///New (20151124)
	UWORD uwCoraseRecord[3]; /// [0]=> coarse   [1]=> head fine value  [2]=>  count
	UBYTE ubRecordCoraseRecord = 0;  /// 0:havn't been written  1:has been written
	uwCoraseRecord[0] = 0xFFFF;
	uwCoraseRecord[1] = 0xFFFF;
	uwCoraseRecord[2] = 0xFFFF;


	* ( (volatile unsigned char*) 0x04005003) &= 0xEF;   // clean GPIO

	wDDR(0x1A) &= ~(0x0380);        // clean Corase
	for (ubCorase = 0; ubCorase < 8; ubCorase++) {

		//wDDR(0x1A) &= ~(0x0380);
		//wDDR(0x1A) |= ((ubCorase<<7) & 0x0380);
		if (ubCorase > 0) {
			wDDR(0x1A) += 0x80;
		}

		uwSuccessCount = 0;
		ubHadSucceed = 0;
		uwSuccess = 0;

		dDDR(0x7C) &= 0xFF800000;   //set 0 at first    //Clean fine
		for (uwFine = 0; uwFine < 256; uwFine++) {
			//dDDR(0x7C) &= 0xFF800000;
			//dDDR(0x7C) |= (((uwFine>>2)<<16) | ((uwFine & 0x2)<<7) | (uwFine & 0x1));
			if (uwFine > 0) {
				ultemp1 = (((uwFine >> 2) << 16) | ((uwFine & 0x2) << 7) | (uwFine & 0x1));
				ultemp2 = (ULONG)(uwFine - 1);
				ultemp2 = (((ultemp2 >> 2) << 16) | ((ultemp2 & 0x2) << 7) | (ultemp2 & 0x1));
				dDDR(0x7C) += (ultemp1 - ultemp2);   // add fine tune
			}

			//////////// Following is data training with BIST& DDR W/R
			ubBISTResult = 0;
			for (ubtemp = 0; ubtemp < 3; ubtemp++) {
				if (ubtemp < 2) {
					ubBISTResult = BIST_ForScanDelayLine((0x60000000 + ubtemp * 0x400), 0x400, ubtemp);
					if (ubBISTResult == 1)
						goto RESULT_SDR;
				}
				else {
					ubBISTResult = CheckData_DDR3((U32*)(0x60000000 + ubtemp * 0x400), 0x400);
					if (ubBISTResult == 1)
						goto RESULT_SDR;
				}
			}

		RESULT_SDR:
			if ( ubBISTResult == 0) { //succeed
				* ( (volatile UBYTE*) 0x04005003) |= 0x10;
				uwSuccessCount++;
				if (ubHadSucceed == 0x0) {
					ubHadSucceed = 0x1;
					uwSuccess = uwFine;
				}
				if (uwFine == 255) {
					ubHadSucceed = 0x0;
					* ( (volatile UBYTE*) 0x04005003) &= 0xEF;

					/// Check if fit the condition and the range is larger than now
					if ((uwSuccessCount > 5) && (uwSuccess < 50)) {

						/// Check if havn't  been recorded
						if (ubRecordCoraseRecord == 0) {
							uwCoraseRecord[0] = ubCorase;
							uwCoraseRecord[1] = uwSuccess;
							uwCoraseRecord[2] = uwSuccessCount;

							ubRecordCoraseRecord = 1;
						}
						else {
							if (uwCoraseRecord[2] < uwSuccessCount) {
								uwCoraseRecord[0] = ubCorase;
								uwCoraseRecord[1] = uwSuccess;
								uwCoraseRecord[2] = uwSuccessCount;
							}
						}
						uwFine = 256;

					}
				}

			}
			else { //fail
				* ( (volatile UBYTE*) 0x04005003) &= 0xEF;
				if (ubHadSucceed == 0x1) {
					ubHadSucceed = 0x0;
					/// Check if fit the condition and the range is larger than now
					if ((uwSuccessCount > 5) && (uwSuccess < 50)) {
						/// Check if havn't  been recorded
						if (ubRecordCoraseRecord == 0) {
							uwCoraseRecord[0] = ubCorase;
							uwCoraseRecord[1] = uwSuccess;
							uwCoraseRecord[2] = uwSuccessCount;

							ubRecordCoraseRecord = 1;
						}
						else {
							if (uwCoraseRecord[2] < uwSuccessCount) {
								uwCoraseRecord[0] = ubCorase;
								uwCoraseRecord[1] = uwSuccess;
								uwCoraseRecord[2] = uwSuccessCount;
							}
						}
						uwFine = 256;

					}
					/// End of Check if fit the condition and the range is larger than now
					uwSuccessCount = 0;
				}
			}

		}
	}

	guw_SDR_window_min = uwCoraseRecord[1];
	guw_SDR_window_max = uwCoraseRecord[1] + uwCoraseRecord[2];

	if (uwCoraseRecord[0] == 0xFFFF) {
		SetInitialFailID(BIT15, 0x02);
		return RET_ERR;
	}
	/*   telling which entry represent what value
	     ubMaxSuccessCorase = uwCoraseRecord[0];
	     uwMaxSuccessCounter = uwCoraseRecord[2];
	     uwMaxSuccessFine = uwCoraseRecord[1];
	*/
	wDDR(0x1A) &= ~(0x0380);
	wDDR(0x1A) |= ((uwCoraseRecord[0] << 7) & 0x0380);

	gub_SDR_Corse = (UBYTE)uwCoraseRecord[0];

	if (uwCoraseRecord[1] == 0) {
		if (uwCoraseRecord[2] > 22) { /// 加這個條件是因為當region < 23 時   算出來的中央fine值會變成負值   這種情況下就直接取中央值就好
			uwCoraseRecord[1]  = (((uwCoraseRecord[2] + 23) >> 1) + 1 - 23);
		}
		else {
			uwCoraseRecord[1]  = 0;
		}
	}
	else {
		uwCoraseRecord[1]  = uwCoraseRecord[1]   + (uwCoraseRecord[2]  >> 1) + 1;
	}

	guw_SDR_Fine = uwCoraseRecord[1];

	dDDR(0x7C) &= 0xFF800000;
	dDDR(0x7C) |= (((uwCoraseRecord[1] >> 2) << 16) | ((uwCoraseRecord[1] & 0x2) << 7) | (uwCoraseRecord[1] & 0x1));
	gul_temp3 = (((uwCoraseRecord[1] >> 2) << 16) | ((uwCoraseRecord[1] & 0x2) << 7) | (uwCoraseRecord[1] & 0x1));

	//       UartString("\r\nCorase:");
	//       UartWordHex(uwMaxSuccessFine);
	//       UartString("\r\nFine:");
	//       UartWordHex((uwCoraseRecord[1]));


	return RET_OK;
}




#if(!RDT)
unsigned char Scan_size(unsigned char ddr_type)
{
	unsigned char ddr_size = 0;

	if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		* ( (volatile unsigned long*) 0x60000000) = 0;
		* ( (volatile unsigned long*) 0x68000000) = 0; //128MB
		* ( (volatile unsigned long*) 0x70000000) = 0; //256MB
		* ( (volatile unsigned long*) 0x60000000) = 0xAABBCCDD;

		if (* ( (volatile unsigned long*) 0x70000000) == 0xAABBCCDD) {
			if (* ( (volatile unsigned long*) 0x68000000) == 0xAABBCCDD) {
				ddr_size = SIZE_128MB;
			}
			else {
				ddr_size = SIZE_256MB;
			}
		}
		else {
			ddr_size = SIZE_512MB;
		}
	}
	else if (ddr_type == SDR) {
#if 1 //cppcheck if condition leads to a dead code block
		volatile unsigned long * zero  = (volatile unsigned long *)0x60000000;
		volatile unsigned long * _8MB  = (volatile unsigned long *)0x60800000;
		volatile unsigned long * _16MB = (volatile unsigned long *)0x61000000;

		zero[0] = 0x11111111;
		_8MB[0] = 0x00000000;
		if (zero[0] != 0) {
			_16MB[0] = 0x00000000;
			if (zero[0] != 0) {
				ddr_size = SIZE_32MB;
			}
			else {
				ddr_size = SIZE_16MB;
			}
		}
		else {
			ddr_size = SIZE_8MB;
		}

#else
		* ( (volatile unsigned long*) 0x60000000) = 0x11111111;
		* ( (volatile unsigned long*) 0x60000200) = 0; //8MB
		//		* ( (volatile unsigned long*) 0x61000000) = 0; //16MB
		//		* ( (volatile unsigned long*) 0x60000000) = 0xAABBCCDD;

		// cppcheck-suppress oppositeInnerCondition
		if (* ( (volatile unsigned long*) 0x60000000) != 0) {
			* ( (volatile unsigned long*) 0x61000000) = 0;
			// cppcheck-suppress oppositeInnerCondition
			if (* ( (volatile unsigned long*) 0x60000000) == 0) {
				ddr_size = SIZE_16MB;
			}
			else {
				ddr_size = SIZE_32MB;
			}
		}
		else {
			ddr_size = SIZE_8MB;
		}
#endif
	}

	return ddr_size;
}
#endif
unsigned char DRAM_init(void)
{
	M_CheckRDTModeToDisableFunctionReturnValid(0);
#if(!RDT)
	RTT1_Stop();


	// GPIO[5:4]=2'b11 : SDRAM-200 3.3V
	// GPIO[5:4]=2'b10 : Reseved
	// GPIO[5:4]=2'b01 : DDR3-1066 (266Mhz)
	// GPIO[5:4]=2'b00 : DDR3-800 (200Mhz)
	// DDR Size (0/1/2/3/4/5/6 : 2/8/16/32/128/256/512M)

	unsigned char ddr_type = 0, ddr_size = 0, ddr_bank = 0;
	unsigned char ubResult = 0;
	////Disable ECC and Enable E3D
	dMISCH(ECC_EN) = CR_E3D_ENABLE;
	////DDR _PHY_CLKEN
	dCLK(CLK_EN_CTRL) |= CR_DDRP_CKEN;
	ddr_bank = 0;

	///Simple initial RTT
	bRTT(RTT1_CTRL) = (SYS_RTTAURST | SYS_CNT_CLEAR | RTT_SCALE_1MS);

	//		ddr_type = DDR3_533;
	ddr_type = (* ( (volatile unsigned char*) 0x04000004) & (_bit5 | _bit4)) >> 4;  ////GPIO_Latch

	//// first suppose size to Maximum
	if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		ddr_size = SIZE_512MB;
		ddr_bank = 0;  //0:8 bank, 1:4 bank
	}
	else if (ddr_type == SDR) {
		ddr_size = SIZE_8MB;
	}


	RTT1_Start();
	ubResult |= ddr_phy_init(ddr_type);              			// Phy init
	ubResult |= ddr_controller_init(ddr_type, ddr_size, ddr_bank, 0);      	// Controller init
	ubResult |= MRS_Setting(ddr_type, ddr_size);               	// MRS setting
	Auto_Refresh(ddr_type);                                   		// Auto Refresh
	RTT1_Stop();

	//		while(ubread);
	if (ubResult != 0) {
		return RET_ERR;
	}


	// Scan Delay Line
	if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		//For DDR
		//wire   [15:0]     MCTDQPIX2             =CFG35[15:0 ];
		//wire   [1:0]      MCTDQSQPB10           =CFG35[23:22];
		//wire   [1:0]      MCTDQSQPB11           =CFG35[25:24];
		//wire   [1:0]      MCTDQSQPB12           =CFG35[27:26];
		//wire   [1:0]      MCTDQSQPBX2           =CFG35[29:28];
		dDDR(0x43c) = 0x3000ffff; //default is 0x0fc0_0000,to adjust read mode DQ and adjust write mode DQ
		//wire   [15:0]     MCTDQPI11             =CFG34[15:0];
		//wire   [15:0]     MCTDQPI12             =CFG34[31:16];
		dDDR(0x438) = 0x00000000; //default is 0xffff_ffff , to adjust write mode DQ

		/// driving & ODT

		*((volatile unsigned long*)0x04005478) = 0x1ecfc002;
		*((volatile unsigned long*)0x04005458) = 0x11b6db00;


		RTT1_Start();
		while ((*((volatile unsigned short*)0x04005028) & 0x0200) != 0x0200) { //check ddr idle
			if (RTT1_GetValue() > TO_DDR) {
				ubResult |= 0x1;
			}
		}
		while ((*((volatile unsigned short*)0x04005010) & 0x03ff) != 0) { //check command free
			if (RTT1_GetValue() > TO_DDR) {
				ubResult |= 0x1;
			}
		}
		RTT1_Stop();


		//				*((volatile unsigned long*)0x04005010)=0x00061002; // default, 34ohm
		*((volatile unsigned long*)0x04005010) = 0x00041002; // 40   ,fine-tune

		//7.CLK add 100ps delay from ideal 1/4T location DDR3 only
		dDDR(0x410) = 0x00000000;
		dDDR(0x414) = 0xffff0000;
		dDDR(0x418) = 0x038e0000;
		dDDR(0x450) = 0x040e38e0;
		dDDR(0x474) = 0x00ff0000;

		//////////////////////////////////////////////////////////
		//
		///////Test added done.
		//				*((volatile unsigned long*)0x0400543c) = 0x30000000;

		ubResult = ScanDelayLine_DDR3();
		ddr_bank = Scan_bank();
		*((volatile unsigned long*)0x04005018) |= (ddr_bank << 30);

		ddr_size = Scan_size(ddr_type);

		if (ddr_bank == 1) { //4 bank
			if (ddr_size == SIZE_512MB)
				dDDR(0x018) = 0x45420000; // set ROW=16bit COL=10bit
			else if (ddr_size == SIZE_256MB)
				dDDR(0x018) = 0x45320000; // set ROW=15bit COL=10bit
			else //(ddr_size=SIZE_128MB)
				dDDR(0x018) = 0x45220000; // set ROW=14bit COL=10bit
		}
		else { // 8 bank
			if (ddr_size == SIZE_512MB)
				dDDR(0x018) = 0x05320000; // set ROW=15bit COL=10bit
			else if (ddr_size == SIZE_256MB)
				dDDR(0x018) = 0x05220000; // set ROW=14bit COL=10bit
			else //(ddr_size=SIZE_128MB)
				dDDR(0x018) = 0x05120000; // set ROW=13bit COL=10bit
		}

		if (ddr_bank == 1) { //4 bank
			if (ddr_size == SIZE_512MB)
				dDDR(0x014) = 0x1bffff88; // TRTP=4,TRAS=1b,TRFC=ff TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else if (ddr_size == SIZE_256MB)
				dDDR(0x014) = 0x1b8cff88; // TRTP=4,TRAS=1b,TRFC=8c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else
				dDDR(0x014) = 0x1b57ff88; // TRTP=4,TRAS=1b,TRFC=57 TWTR_AL=f,TWR=f,TRP=8,TRCD=8
		}
		else {
			if (ddr_size == SIZE_512MB)
				dDDR(0x014) = 0x1b8cff88; // TRTP=4,TRAS=1b,TRFC=8c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else if (ddr_size == SIZE_256MB)
				dDDR(0x014) = 0x1b57ff88; // TRTP=4,TRAS=1b,TRFC=57 TWTR_AL=f,TWR=f,TRP=8,TRCD=8
			else
				dDDR(0x014) = 0x1b3cff88; // TRTP=4,TRAS=1b,TRFC=3c TWTR_AL=f,TWR=f,TRP=8,TRCD=8
		}

	}

	if (ddr_type == SDR) {
		//[2:0]     TMSHCKN               =CFG45[15:13];
		dDDR(0x464) = 0x00004010; //default is 0x00002010
		ubResult = ScanDelayLine_SDR();



		//Set SIZE as 32MB
		bDDR(0x01A) &= 0x80;          /// default value 0 : 8MB
		bDDR(0x01A) |= 0x11;          /// 32MB

		ddr_size = Scan_size(ddr_type);

		bDDR(0x01A) &= 0x80;           /// default value 0 : 8MB
		if (ddr_size == SIZE_32MB) {
			bDDR(0x01A) |= 0x11;
		} // set ROW=13bit COL=9bit   //{COARSE[1:0],fine}=[25:23]=3'b001
		else if (ddr_size == SIZE_16MB) {
			bDDR(0x01A) |= 0x01;
		} // set ROW=12bit COL=9bit   //{COARSE[1:0],fine}=[25:23]=3'b001

	}

	gubDram_Size = ddr_size;
	gubDram_Type = ddr_type;
	gubDram_Bank = ddr_bank;



	if (ubResult == 0x0) {
		return RET_OK;
	}
	else {
		return RET_ERR;
	}
#endif
}


unsigned char Scan_bank(void)
{

	*((volatile unsigned long*)0x60000000) = 0x11111111;
	*((volatile unsigned long*)0x60002000) = 0x00000000;
	if (*((volatile unsigned long*)0x60000000) == 0x11111111) {
		return 0;   // 8 banks
	}
	else {
		return 1;   // 4 banks
	}
}

unsigned char BIST_ForScanDelayLine(unsigned long uladdress, unsigned long ulZ1size, unsigned char ubtemp)
{
	unsigned char ubError = 0x0;
	unsigned long ulTestPattern[8] = {0x55555555, 0xC36969C3, 0x0000ffff, 0xAAAA5555, 0xf0f0f0f0, 0x5a5aa5a5, 0x69c3c369, 0xffff0000};

	////step 3. set address data
	////RAM_BIST_ADDRESS_DATA_SET(uladdress,uladdress + ulZ1size, 0x60000100, 0, 0);
	//    DDRREGL[RAML_DDR3_BIST_START_ADDR] = uladdress;
	//    DDRREGL[RAML_DDR3_BIST_END_ADDR] = uladdress + ulZ1size;
	//    DDRREGL[RAML_DDR3_BIST_RD_START_TIME] = uladdress + 0X100;
	//    DDRREGL[RAML_DDR3_BIST_START_DATA] = 0;

	dDDR(0x58) = uladdress;
	dDDR(0x5C) = uladdress + ulZ1size;
	dDDR(0x60) = uladdress + 0x100;
	dDDR(0x64) = ulTestPattern[ubtemp % 8];
	//    dDDR(0x64) = 0;
	//    dDDR(0x64) = ((ubtemp > 3)? ulTestPattern[ubtemp] : 0 );



	////Step 4. set BIST CFG
	////cycle=0 DATA Sequential MODE ,LEN=0,3,15,63			32'h340_00F1
	////RAM_BIST_CFG_SET( BIST_Data_Sequential, 0x04, 0, BIST_INCR, BIST_Len_0_3_15_63, 0xf, 0, 0);
	////DDRREGL[RAML_DDR3_BIST_CFG] = ((ubWRAP<<28) + (ubLenSlection<<25) + RW_PERIOD_ADJUST + (ubNxtAddr<<20) + (1<<12) + (ubCycle<<8) + (ubLen<<4) + (ubDataMode<<1) + ubEnableFixLen);
	////dDDR(0x6C) = ((BIST_INCR<<28) | (BIST_Len_Asign<<25) | RW_PERIOD_ADJUST | (0x04<<20) | (1<<12) | (0x0<<8) | (0x0<<4) | (BIST_Data_Sequential<<1) | 0x0);
	dDDR(0x6C) = ((0 << 28) | (0 << 25) | (0x01000000) | (0x04 << 20) | (1 << 12) | (0x0 << 8) | (0xF << 4) | (  ((ubtemp > 0) ? 1 : 2 )   << 1) | 0x1);
	//    	dDDR(0x6C) = ((0<<28) | (0<<25) | (0x01000000) | (0x04<<20) | (1<<12) | (0x0<<8) | (0xF<<4) | (  1   <<1) | 0x1);
#pragma flush
	////Step 5. set BIST trigger
	////RAM_BIST_TRIGGER_SET(BIST_Address_Sequential, 0, 0, 0);
	////dDDR(0x24)|=0x01000000;  // clear BIST
	////dDDR(0x24) = ((ubAddressMode<<16) + (ubShift4MB<<1) + BIST_TRIGGER);
	if (ubtemp == 2) {
		idle(0x1000);
	}
	dDDR(0x24) = ((0x1 << 16) + (0x0 << 1) + 0x1);
	if (dDDR(0x24) & 0x100000) { // BIST set error
		dDDR(0x24) |= 0x01000000; // clear BIST
		while ((dDDR(0x24) & 0x01000000) == 0x01000000 );

		return 1;
	}

	////RAM_BIST_WAIT_TRIGGER();
	////ubError = RAM_BIST_ERROR_REPORT();

	while (dDDR(0x24) & 0x01);
	ubError = ((dDDR(0x24) & 0x0100) >> 8);


	dDDR(0x24) |= 0x01000000; // clear BIST
	while ((dDDR(0x24) & 0x01000000) == 0x01000000);
	if (ubError)
		return 1;
	else
		return 0;
}

#endif

void entry_selfrefresh ()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	unsigned long memrd_rdata;

	RTT1_Start();
	while ((dDDR(0x28) & 0x00000200) == 0x00000000) {
		if (RTT1_GetValue() > TO_DDR) {
			RTT1_Stop();
			//			return RET_TIMEOUT;
		}
	}
	while ((dDDR(0x28) & 0x00000200) == 0x00000000) {
		if (RTT1_GetValue() > TO_DDR) {
			RTT1_Stop();
			//			return RET_TIMEOUT;
		}
	}
	idle(100);
	//1)entry SF
	while ((dDDR(0x10) & 0x000003ff) != 0x00000000) {
		if (RTT1_GetValue() > TO_DDR) {
			RTT1_Stop();
			//			return RET_TIMEOUT;
		}
	}
	dDDR(0x010) = 0x00000008;
	while ((dDDR(0x10) & 0x000003ff) != 0x00000000) {
		if (RTT1_GetValue() > TO_DDR) {
			RTT1_Stop();
			//			return RET_TIMEOUT;
		}
	}
	idle(16);
	//2)DRAM Clock GATING
	memrd_rdata = dDDR(0x468);
	dDDR(0x468) = memrd_rdata | 0x80000000;

	RTT1_Stop();
#endif
}

void exit_selfrefresh ()
{
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	U32_t memrd_rdata;
	RTT1_Start();
	//1)DRAM Clock UN-GATING
	memrd_rdata = dDDR(0x468);
	dDDR(0x468) = memrd_rdata & 0x7fffffff;
	idle(512);
	//2)exit SF
	while ((dDDR(0x10) & 0x000003ff) != 0x00000000) {
		if (RTT1_GetValue() > TO_DDR) {
			RTT1_Stop();
			//			return RET_TIMEOUT;
		}
	}
	dDDR(0x010) = 0x00000020;
	while ((dDDR(0x10) & 0x000003ff) != 0x00000000) {
		if (RTT1_GetValue() > TO_DDR) {
			RTT1_Stop();
			//			return RET_TIMEOUT;
		}
	}
	idle(1024);

	RTT1_Stop();

	//For Etron DDR Self-Refresh MR(Burst Length form 8 to 4) Issue
	while ( (dDDR(0x010) & 0x000003ff) != 0x00000000); //wait ddrcmdfree
	dDDR(0x010) = 0x00330002; //MR0 CAS latency=3
	while ( (dDDR(0x010) & 0x000003ff) != 0x00000000); //wait ddrcmdfree

#endif
}

void RAM_Set_ZONE_Region(UBYTE ubZoneSelect, U32 ulAddress, U32 ulBytelength)
{
	if (ubZoneSelect == 1) {
		SYS1L[SYS1L_DZ1_DSA] = ulAddress;
		SYS1L[SYS1L_DZ1_DLEN] = ulBytelength;
	}
	else if (ubZoneSelect == 2) {
		SYS1L[SYS1L_DZ2_DSA] = ulAddress;
		SYS1L[SYS1L_DZ2_DLEN] = ulBytelength;
	}
	else if (ubZoneSelect == 3) {
		SYS1L[SYS1L_DZ3_DSA] = ulAddress;
		SYS1L[SYS1L_DZ3_DLEN] = ulBytelength;
	}
	else if (ubZoneSelect == 4) { //shadow Z1
		SYS1L[SYS1L_SZ1_ADDRESS] = ulAddress;
	}
	else if (ubZoneSelect == 5) { //shadow Z2
		SYS1L[SYS1L_SZ2_ADDRESS] = ulAddress;
	}
}

void entry_retention(void)
{
	//---
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	while ( (*((volatile unsigned short*)0x04005028) & 0x0200) == 0 );
	entry_selfrefresh();

	//	*((volatile unsigned long*)0x04005468) |= 0x80000000;
	bSYS0(0x500) = (bSYS0(0x500) & 0xF7);         // cr_pmu_vdt_pd=0
	/*
	Hardware:
	若cr_op_vdt_wu為0,pmu wake up flow不會等PMU_RST_PD2_ENB為1, 就會解開cpu stall,
	若cpu開始動作後, 也沒有確認PMU_RST_PD2_ENB為1, 就對PD2的人(maybe flash…) access,
	有可能因為PD2還沒完全wake up導致fail.
	*/
	//bSYS0(0x52F) = (bSYS0(0x52F) & 0xBF);         // cr_op_vdt_wu=0

	bSYS0(0x318) = _bit7;                         // CR_DDR_DPD=1


}


unsigned char exit_retention(unsigned char ddr_type, unsigned char ddr_size, unsigned char ddr_bank)
{
	M_CheckRDTModeToDisableFunctionReturnValid(0);
#if(!RDT)
	M_CheckBurnerModeToDisableFunctionReturnValid(0);
	////////////////Simple read-me
	unsigned char ubResult = 0;

	ddr_controller_init(ddr_type, ddr_size, ddr_bank , 0);
	MRS_Setting(ddr_type, ddr_size);               	// MRS setting

	entry_selfrefresh();                     			// into self refresh

	bSYS0(0x318) = 0x00;                          		// CR_DDR_DPD=0
	bSYS0(0x530) = 0xC8;                          		// sr_sm_cnt=0xC8 (200us)

	RTT1_Start();
	while (bSYS0(0x533) != 0x00) {
		if (RTT1_GetValue() > TO_DDR) {
			return RET_TIMEOUT;
		}
	}
	RTT1_Stop();

	ddr_phy_init(ddr_type);
	Auto_Refresh(ddr_type);
	exit_selfrefresh();
	///Set driving & ODT
	if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		//For DDR
		//wire   [15:0]     MCTDQPIX2             =CFG35[15:0 ];
		//wire   [1:0]      MCTDQSQPB10           =CFG35[23:22];
		//wire   [1:0]      MCTDQSQPB11           =CFG35[25:24];
		//wire   [1:0]      MCTDQSQPB12           =CFG35[27:26];
		//wire   [1:0]      MCTDQSQPBX2           =CFG35[29:28];
		dDDR(0x43c) = 0x3000ffff; //default is 0x0fc0_0000,to adjust read mode DQ and adjust write mode DQ
		//wire   [15:0]     MCTDQPI11             =CFG34[15:0];
		//wire   [15:0]     MCTDQPI12             =CFG34[31:16];
		dDDR(0x438) = 0x00000000; //default is 0xffff_ffff , to adjust write mode DQ

		/// driving & ODT

		*((volatile unsigned long*)0x04005478) = 0x1ecfc002;
		*((volatile unsigned long*)0x04005458) = 0x11b6db00;


		RTT1_Start();
		while ((*((volatile unsigned short*)0x04005028) & 0x0200) != 0x0200) { //check ddr idle
			if (RTT1_GetValue() > TO_DDR) {
				ubResult |= 0x1;
			}
		}
		while ((*((volatile unsigned short*)0x04005010) & 0x03ff) != 0) { //check command free
			if (RTT1_GetValue() > TO_DDR) {
				ubResult |= 0x1;
			}
		}
		RTT1_Stop();


		*((volatile unsigned long*)0x04005010) = 0x00061002; //34ohm`


		//7.CLK add 100ps delay from ideal 1/4T location DDR3 only
		dDDR(0x410) = 0x00000000;
		dDDR(0x414) = 0xffff0000;
		dDDR(0x418) = 0x038e0000;
		dDDR(0x450) = 0x040e38e0;
		dDDR(0x474) = 0x00ff0000;
	}
	if (ddr_type == SDR) {
		//[2:0]     TMSHCKN               =CFG45[15:13];
		dDDR(0x464) = 0x00004010; //default is 0x00002010
	}



	// set delay line value
	if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		Set_DDR3_DQS_G(gubDQS0_G, gubDQS1_G);
		Set_DDR3_DQS_Fine(guwDQS0_Fine, guwDQS1_Fine);
	}
	if (ddr_type == SDR) {
		wDDR(0x1A) &= ~(0x0380);
		wDDR(0x1A) |= ((gub_SDR_Corse << 7) & 0x0380);

		dDDR(0x7C) &= 0xFF800000;
		dDDR(0x7C) |= (((guw_SDR_Fine >> 2) << 16) | ((guw_SDR_Fine & 0x2) << 7) | (guw_SDR_Fine & 0x1));
	}

	return ubResult;
#endif
}
