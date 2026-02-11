#define _HOST_DECL_

#include <xtensa/tie/PS3111_Xtensa.h>
#include <xtensa/xtruntime.h>
#include <xtensa/hal.h>
#include <xtensa/config/core.h>

#include "global/_global.h"
#include "global/_types.h"
#include "Setup.h"
#include "Vardef.h"
#include "IO.h"
#define TASK_DS_INSTANCE
#include "FTL.h"
#undef TASK_DS_INSTANCE
#include "cmdtbl.h"
#include "RDT.h"
#include "Ddr.h"

#include <stdio.h>
#include <stdlib.h>

#if MicronFlashOnly
extern UBYTE gubIRAMContent[FPU_Length];
#else
#if TLC

#if Hynix3DV6 || YMTC_JGS
extern FPUSEQs_T gubIRAMContent;
#else
extern UBYTE gubIRAMContent[FPU_Length];
#endif
extern RamSEQs_T guwFpuEntry;



#else
extern UBYTE gubIRAMContent[FPU_Length];
#endif
#endif


TX_THREAD	Mainthread;
TX_THREAD               thread_0;
TX_THREAD               thread_1;
TX_THREAD               thread_2;

__attribute__((section(".ICODE.data"))) UBYTE gubAonBufOffset[0x50];
__attribute__((section(".ICODE.data")))	U32 LPM7RRAMConstant1[64] = {

	0x00000000,	0x04000414,	0x0c400000,	0x04000534,
	0x04000414,	0x00000000,	0x04000410,	0x70700000,
	0x04000324,	0xe0e00000,	0x04000324,	0x00000000,
	0x04000030,	0x04050180,	0x00004000,	0x04000624,
	0x0000e0fc,	0x04000520,	0x0000c700,	0x04000300,
	0x00000200,	0x04000534,	0x00008000,	0x04000524,
	0x00000001,	0x04000030,	0x04000180,	0x00000002,
	0x04000030,	0x04010180,	0x01000000,	0x04000538,
	0x00000002,	0x04000530,	0x04000530,	0x1f008022,
	0x00000000,	0x04000538,	0x00020000,	0x04000534,
	0x04000524,	0x0f008028,	0x0000802b,	0x00000082,
	0x04000524,	0x04000524,	0x0700802d,	0x00000080,
	0x04000410,	0x00000100,	0x04000538,	0x80000000,
	0x04000350,	0x00000300,	0x04000534,	0x00030000,
	0x04000534,	0x00000000,	0x04000500,	0x00000000,
	0x04000538,	0x00000000,	0x00000000,	0x00000000

};

__attribute__((section(".ICODE.data")))	UBYTE LPM7RRAMConstant2[64] = {

	0x5f,	0x29,	0x5c,	0x28,
	0x24,	0x54,	0x24,	0x5c,
	0x2c,	0x5c,	0x2c,	0x51,
	0x21,	0x3f,	0x52,	0x22,
	0x53,	0x23,	0x52,	0x22,
	0x52,	0x22,	0x52,	0x22,
	0x051,	0x21,	0x3f,	0x51,
	0x21,	0x3f,	0x5c,	0x28,
	0x5f,	0x21,	0x18,	0x8f,
	0x54,	0x24,	0x54,	0x24,
	0x12,	0x8f,	0x80,	0x51,
	0x21,	0x11,	0x8f,	0x51,
	0x21,	0x52,	0x22,	0x58,
	0x28,	0x52,	0x22,	0x54,
	0x24,	0x52,	0x22,	0x51,
	0x21,	0xf0,	0x00,	0x00

};
#if TLC
__attribute__((section(".ICODE.data"))) UBYTE toshiba_sb_tlc_default_llr_table_1[] = {
	0x07, 0x00, 0x07, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x0B, 0x00, 0x0B, 0x00, 0x0F, 0x00, 0x0F, 0x00,
	0x19, 0x00, 0x19, 0x00, 0x1D, 0x00, 0x1D, 0x00,
	0x15, 0x00, 0x15, 0x00, 0x11, 0x00, 0x11, 0x00,

	0x07, 0x07, 0x07, 0x00, 0x03, 0x03, 0x03, 0x00,
	0x0B, 0x0B, 0x0B, 0x00, 0x0F, 0x0F, 0x0F, 0x0F,
	0x19, 0x19, 0x19, 0x00, 0x1D, 0x1D, 0x1D, 0x00,
	0x15, 0x15, 0x15, 0x00, 0x11, 0x11, 0x11, 0x11,

	0x07, 0x00, 0x07, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x0B, 0x00, 0x0B, 0x00, 0x0F, 0x00, 0x0F, 0x00,
	0x19, 0x00, 0x19, 0x00, 0x1D, 0x00, 0x1D, 0x00,
	0x15, 0x00, 0x15, 0x00, 0x11, 0x00, 0x11, 0x00
};

__attribute__((section(".ICODE.data"))) UBYTE toshiba_sb_tlc_default_llr_table_2[] = {
	0x03, 0x00, 0x09, 0x00, 0x1D, 0x00, 0x06, 0x00,
	0x09, 0x00, 0x0C, 0x00, 0x0F, 0x00, 0x0F, 0x00,
	0x17, 0x00, 0x1D, 0x00, 0x1A, 0x00, 0x03, 0x00,
	0x14, 0x00, 0x17, 0x00, 0x11, 0x00, 0x11, 0x00,

	0x03, 0x09, 0x09, 0x00, 0x1D, 0x06, 0x06, 0x00,
	0x09, 0x0C, 0x0C, 0x00, 0x0F, 0x0F, 0x0F, 0x0F,
	0x17, 0x1D, 0x1D, 0x00, 0x1A, 0x03, 0x03, 0x00,
	0x14, 0x17, 0x17, 0x00, 0x11, 0x11, 0x11, 0x11,

	0x03, 0x00, 0x09, 0x00, 0x1D, 0x00, 0x06, 0x00,
	0x09, 0x00, 0x0C, 0x00, 0x0F, 0x00, 0x0F, 0x00,
	0x17, 0x00, 0x1D, 0x00, 0x1A, 0x00, 0x03, 0x00,
	0x14, 0x00, 0x17, 0x00, 0x11, 0x00, 0x11, 0x00
};

__attribute__((section(".ICODE.data"))) UBYTE SanDisk_sb_tlc_default_llr_table_1[] = {
	0x04, 0x00, 0x07, 0x00, 0x01, 0x00, 0x03, 0x00,
	0x08, 0x00, 0x0A, 0x00, 0x0C, 0x00, 0x0D, 0x00,
	0x1C, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x01, 0x00,
	0x18, 0x00, 0x1A, 0x00, 0x14, 0x00, 0x11, 0x00,

	0x04, 0x08, 0x07, 0x00, 0x01, 0x03, 0x03, 0x00,
	0x08, 0x0A, 0x0A, 0x00, 0x0F, 0x0F, 0x0E, 0x0F,
	0x1C, 0x1F, 0x1F, 0x00, 0x1F, 0x01, 0x01, 0x00,
	0x18, 0x1A, 0x1A, 0x00, 0x11, 0x11, 0x11, 0x11,

	0x04, 0x00, 0x07, 0x00, 0x01, 0x00, 0x03, 0x00,
	0x08, 0x00, 0x0A, 0x00, 0x0C, 0x00, 0x0D, 0x00,
	0x1C, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x01, 0x00,
	0x18, 0x00, 0x1A, 0x00, 0x11, 0x00, 0x14, 0x00
};
#if 0
__attribute__((section(".ICODE.data"))) UBYTE SanDisk_sb_tlc_default_llr_table_2[] = {
	0x04, 0x00, 0x04, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x08, 0x00, 0x08, 0x00, 0x0D, 0x00, 0x0D, 0x00,
	0x1C, 0x00, 0x1C, 0x00, 0x1F, 0x00, 0x1F, 0x00,
	0x18, 0x00, 0x18, 0x00, 0x14, 0x00, 0x11, 0x00,

	0x04, 0x04, 0x04, 0x00, 0x01, 0x01, 0x01, 0x00,
	0x08, 0x08, 0x08, 0x00, 0x0F, 0x0F, 0x0F, 0x0F,
	0x1C, 0x1C, 0x1C, 0x00, 0x1F, 0x1F, 0x1F, 0x00,
	0x18, 0x18, 0x18, 0x00, 0x11, 0x11, 0x11, 0x11,

	0x04, 0x00, 0x04, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x08, 0x00, 0x08, 0x00, 0x0D, 0x00, 0x0D, 0x00,
	0x1C, 0x00, 0x1C, 0x00, 0x1F, 0x00, 0x1F, 0x00,
	0x18, 0x00, 0x18, 0x00, 0x11, 0x00, 0x14, 0x00
};
#endif

#endif
void M_SwitchTask()
{
#if ENABLE_PFAIL
	SYS1L[SYS1L_INT_EN4] &= ~(GPIO_INT_SEL0_FALLING_EDGE); // workaround for thread_x + gpio isr
#endif
	M_DebugGPIO3_High();
	tx_thread_relinquish();
	if (ENABLE_DEBUG_DDRINTERRUPT) {
		SYS1L[SYS1L_ECC_EN] &= (~(CR_Z1_ECC_EN | CR_Z2_ECC_EN | CR_Z3_ECC_EN));
		UBYTE *ubZ1 = (UBYTE*)(0x60000000);
		ubZ1[0] = (~ubZ1[0]);
		UBYTE *ubZ2 = (UBYTE*)(0x60100000);
		ubZ2[0] = (~ubZ2[0]);
		UBYTE *ubZ3 = (UBYTE*)(0x64000000);
		ubZ3[0] = (~ubZ3[0]);
		SYS1L[SYS1L_ECC_EN] |= (CR_Z1_ECC_EN | CR_Z2_ECC_EN | CR_Z3_ECC_EN);
		UBYTE ubTemp = ubZ1[0];
		ubTemp = ubZ2[0];
		ubTemp = ubZ3[0];
		idle(10);
	}
	M_DebugGPIO3_Low();
#if ENABLE_PFAIL
	SYS1L[SYS1L_INT_EN4] |= (GPIO_INT_SEL0_FALLING_EDGE); // workaround for thread_x + gpio isr
#endif
}
__attribute__((section (".iram0.text"))) void cfg_clk()
{

#ifndef FPGA
	//	int i;

	SYS0B[0x420] |= 0x0f;//set cpu division = 0xf

#if 0
	SYS0B[0x380] &= 0x7f; //release PLL0_DIS

	for (i = 0; i < 100; i++); //wait at least 20us

	SYS0B[0x380] &= 0xf; //release PLL_RESET & PLL_CHPMP_DIS & EN_VCMID

	while ( ( SYS0B[0x400] & 0x01 ) != 0x01 ); //wait PLL0 LOCK

	//config cpu & ddr clk
	SYS0B[0x428]  = 0x90;//set CPU RX_P0_SEL = 1 & P0_SEL = 2'b01
	SYS0B[0x42c]  = 0x90;//set DDR RX_P0_SEL = 1 & P0_SEL = 2'b01
#endif

	SYS0B[0x416] |= 0x80;//ddr clk gate enable

#endif

}

void SYS_Interrupt_Init()
{
	M_CheckBurnerModeToDisableFunction();

	/// clear WDT timeout bit
	SYS1L[SYS1L_WDT_CTRL] &= 0xFFFF00FF;

	_xtos_set_interrupt_handler(21, GPIO_DEVSLP_ISR);
	_xtos_set_intlevel(0);
	_xtos_ints_on( 1L << (21) );

	bSYS0(0x102) &= 0x8F;		//20150708  //Reset  CR_DEVSLP_PD, CR_DEVSLP_PU[1:0]
	/// bSYS0(0x102) |= 0x10;		//20150708  //CR_DEVSLP_PU[1:0] = 75K Pull UP  (will cause Samsung NB detect DEVSLP pin fail, so back to default setting and no pull up.

	//bSYS0(0x3154) |= (BIT5);    //Rising of DEVSLP, for LPM2 wakeup usage.

	/*
		///Debug usage of using GPIO rising, using GPIO3.
		/// 1. Set MUX.
		* ( (volatile unsigned short*) 0x04000202) =0x0000;

		/// 2. Set GPIO DET SEL
		* ( (volatile unsigned char*) (0x04002204)) = 3;
		* ( (volatile unsigned char*) (0x04002200)) = 1;

		/// 3. Enable interrupt
		* ( (volatile unsigned char*) 0x04003155) = 0x03;
	*/
	SYS0W[SYS0W_CR_MUX_GPIO] &= CLR_BIT9; //MUX_GPIO[13], 0: for GPIO
	SET_CR_GPIO_DET_EN(BIT2); //cr_gpio_det_sel_2 enable
	SYS1B[SYS1B_CR_GPIO_DET_SEL_2] = 13;
	//SYS1L[SYS1L_INT_EN4] |= (BIT12|BIT13); //rising/falling edge selected by cr_gpio_det_sel_2
#if ENABLE_PFAIL
	/* GPIO Initial
	 * Set GPIO 3 and 13 to input mode and pull high
	 * GPIO3 = Pfail signal, low = Power drop
	 * GPIO13 = CAP signal, high = Cap fully charged
	 */
	SYS0W[SYS0W_CR_MUX_GPIO] = 0x0009U;
	//SYS0W[SYS0W_CR_MUX_GPIO] &= CLR_BIT9; // GPIO13 for GPIO, instead of Debug Port

	SYS0B[SYS0B_GPIO_CTRL_GPIO3] = SYS0B_CR_GPIO_PU_X_0;  // set GPIO to input mode, and pull up 75K
	SYS0B[SYS0B_GPIO_CTRL_GPIO13] = SYS0B_CR_GPIO_PU_X_0;  // set GPIO to input mode, and pull up 75K

	SYS1L[SYS1L_INT_EN4] &= ~(GPIO_INT_SEL0_RISING_EDGE | GPIO_INT_SEL0_FALLING_EDGE | GPIO_INT_SEL1_RISING_EDGE | GPIO_INT_SEL1_FALLING_EDGE);
	SYS1L[SYS1L_INT_STS4] |= (GPIO_INT_SEL0_RISING_EDGE | GPIO_INT_SEL0_FALLING_EDGE | GPIO_INT_SEL1_RISING_EDGE | GPIO_INT_SEL1_FALLING_EDGE);

	// ENABLE_GPIO_3 INTERRUPT
	SYS1B[SYS1B_CR_GPIO_DET_SEL_0] = 3;
	SYS1L[SYS1L_EDGE_DET_EN] |= BIT0; // enable cr_gpio_det_sel_0

	/* // ENABLE_GPIO_13 INTERRUPT
	SYS1B[SYS1B_CR_GPIO_DET_SEL_1] = 13;
	SYS1L[SYS1L_EDGE_DET_EN] |= BIT1; // enable cr_gpio_det_sel_1 */

	SYS1L[SYS1L_INT_EN4] |= (GPIO_INT_SEL0_FALLING_EDGE /* | GPIO_INT_SEL1_RISING_EDGE*/); // Power Down Only
#endif
}


UBYTE Sata_Isr_Handle_Error(UBYTE ubMode)
{
	return 1;
}

void Sata_InterfaceReg_Init()
{
	HB[HB_DEVICE] = 0x00;  // After add DRV_HD & ERR , Can Configure already.
	HB[HB_ERROR] = 0x01;
	HB[HB_SECTOR_CNT] = 0x01;
	HB[HB_LBA_L] = 0x01;
	HB[HB_LBA_H] = 0x00;
	HB[HB_LBA_M] = 0x00;
}

void APU_Reset()
{
	HB[HB_DATA_TRG] = H_APU_RST;
	while ((HW[HW_SATA_INFO]&H_APU_RST_STATUS) != 0); //等穩態
	HW[HW_MZ_CTRL] |= BIT11; // 清FIFO
	HW[HW_MZ_CTRL] &= ~BIT11; // 清FIFO

	gubLBAOverlap = 0;
	gubNeedWaitTQDone = 0;
}

//return 讓ISR一次做一件事,比較單純
/* Read Flow
從dbuf讀走data (unc就讀不走)
-> 清flag
-> 傳給host
-> 最後一筆data檢查E3D(有E3D err還是會送data)
-> 等Host reply R_OK或R_ERR: R_OK就舉 done tag(E3D error時Host還是會回R_OK),
                                          R_ERR就舉error intr(R_ERR大部分是CRC error)
*/
void SATA_ISR()
{
	//	M_CheckRDTModeToDisableFunction();
	volatile U32 ul_Temp_IntFlag = HL[HL_INT_FLAG];

	if (ul_Temp_IntFlag & H_LNK_REQPRCV_INT) {      // Partial cmd
		HL[HL_INT_FLAG] = (U32)H_LNK_REQPRCV_INT;	//W1C
		if (HW[HW_SATA_INFO]&H_SLUMBER) {
			if ((HB[HB_REMAIN_VLD_NCQ_NUM] != 0) || (HL[HL_AXI_FIS_STAT] & BIT23)) {
				HW[HW_PM_CTRL] |= H_REG_PMCANCEL;
			}
		}
		if (!BURNER) {
			if (  ((HW[HW_PM_CTRL] & BIT7) != 0) && ((HW[HW_PM_CTRL] & BIT8) != 0)  ) {
				gubHIPMEnable = 1;
			}
			else {
				//			gubHIPMEnable = 0;
				HW[HW_PM_CTRL] |= H_REG_PMNAK_P;
			}
		}
	}
	else {
		//		gubHIPMEnable = 0;
	}

	if (ul_Temp_IntFlag & H_LNK_REQSRCV_INT) {      // Slumber cmd
		HL[HL_INT_FLAG] = (U32)H_LNK_REQSRCV_INT;	//W1C
		if (HW[HW_SATA_INFO]&H_PARTIAL) {
			if ((HB[HB_REMAIN_VLD_NCQ_NUM] != 0) || (HL[HL_AXI_FIS_STAT] & BIT23)) {
				HW[HW_PM_CTRL] |= H_REG_PMCANCEL;
			}
		}
		if (!BURNER) {
			if (  ((HW[HW_PM_CTRL] & BIT7) != 0) && ((HW[HW_PM_CTRL] & BIT8) != 0)  ) {
				gubHIPMEnable = 1;
			}
			else {
				//			gubHIPMEnable = 0;
				HW[HW_PM_CTRL] |= H_REG_PMNAK_S;
			}
		}
	}
	else {
		//		gubHIPMEnable = 0;
	}

	//softReset :  block Host Trig & stop auto FIS & Stop auto trigger & APU Reset
	//1. H_REG_XHOLD: 不打XRDY, Host便不會再下任何FIS............  H_BLK_TRG:停下準備打出的D2H/SDB , 兩者一起開會把D2H H2D都停下
	//2. 把FlagAutoSet/Chk都關掉, 避免有些要傳的資料被flag卡住
	//3. 清掉buffer flag
	//4. APU reset
	// ps. APU reset會把HAT (HW auto trigger關掉, 如果要用的話要在reset cmdin裡打開)
	if (ul_Temp_IntFlag & H_SRST_CLR_INT) {
		if (ENABLE_KT_DEBUG_UART)
			UartString("\n\rSRST");
		//                UartLongHex(ul_Temp_IntFlag);
#if(!BURNER)
		if (gubIgnoreFlag > 0) {
			HW[HW_AUTO_FIS_CTRL] |= gubAutoD2HBackup;
			gubAutoD2HBackup = 0;
			gubIgnoreFlag = 0;
		}
#endif

		gubNeedChkDoneTag = 0;// cmd timeout有可能會進來

		HL[HL_INT_FLAG] = H_SRST_CLR_INT;

		HW[HW_ERR_CTRL] |= H_REG_XHOLD | H_BLK_TRG;

		APU_Reset(); // Reset Application layer Unit



		/* 有些driver遇到fail會下softReset*/
		M_SetFlagSettingNone(); //避免timeOut, 無法從R/W回到main作fail handling
		AXIL[AXIL_SET_START_FLAG] = 0;
		AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //因為關掉autoSet/autoCheck, 可能w的FQ做完後對應的flag沒清
		AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;
		gubFirstCMDPending = 0;

		gubSoftReset = 1;
		if (ENABLE_RESET_CNT) {
			if (gubResetFlag == 1) {
				gubResetCountDuringCmd ++;
			}
		}
		return ;
	}

	if (ul_Temp_IntFlag & H_LNK_COMRI_INT) {
		if (ENABLE_DEBUG_UART)
			UartString("\nHRST");

		//阻止清flag
		/*
		if (ENABLE_KT_DEBUG && gubTest) {
			while (gubTest);
			HL[HL_ERR_INT_FLAG] = H_LNK_NOCOMERR_INT; //comreset會先斷Link
			HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;
			HL[HL_INT_FLAG] = (U32)H_LNK_COMRI_INT;
			return;
		}
		*/
	Mark_COMRESET:
#if(!RDT)
		gubNeedChkDoneTag = 0;// cmd timeout有可能會進來
#endif
#if(!BURNER)
		if (gubIgnoreFlag > 0) {
			HW[HW_AUTO_FIS_CTRL] |= gubAutoD2HBackup;
			gubAutoD2HBackup = 0;
			gubIgnoreFlag = 0;
		}
#endif
		//如果0x44尚有造成INT的error Bit未清, 就算清掉0x0的errorEvent, 出ISR後還是會再舉一次, 所以可以放心清
		//(eg. 若CRC後接一個Link lost, 則就算清掉 0x44 CRC error 與 0x0 errEvent, 出ISR後 0x0還是會因為Link lost而把errEvent舉起)
		HL[HL_ERR_INT_FLAG] = H_LNK_NOCOMERR_INT; //comreset會先斷Link
		HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;

		if (!ComresetRisingEdge) {
			//這個原因是因為COMRESET是一個level的訊號,
			//因為CRC先發生  -> 收到Comreset /"""""""""""\ (level)
			//                                                               ^(1)      ^(2) 清兩次comreset bit, 剛好都在comreset level裡
			//所以這時候FW清comreset bit跟comreset發生的時間剛好一直重疊,
			//導致comreset bit一清掉又被HW設起來,
			//(但是因為comreset level大約持續1~200ns, 最後還是會被清掉)
			//可以把apu reg 0x048h[5:4] 改成0x01b就可以保證comreset只會發一次isr
			while (HL[HL_INT_FLAG]&H_LNK_COMRI_INT) {
				HL[HL_INT_FLAG] = (U32)H_LNK_COMRI_INT;
			}
		}
		else {
			HL[HL_INT_FLAG] = (U32)H_LNK_COMRI_INT;
		}

		HW[HW_ERR_CTRL] |= H_REG_XHOLD | H_BLK_TRG;

		APU_Reset(); // Reset Application layer Unit

		M_SetFlagSettingNone(); //避免timeOut, 無法從R/W回到main作fail handling
		AXIL[AXIL_SET_START_FLAG] = 0;
		AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //因為關掉autoSet/autoCheck, 可能w的FQ做完後對應的flag沒清
		AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;
		gubFirstCMDPending = 0;

		gubHardReset = 1;

		if (AutoActivate_Support) {
			HB[HB_NCQ_SET] = H_NCQ_SET_SEL;//Bit7舉起表示之後寫的值只對bit5bit6作用,comreset要清掉
		}

		if (ENABLE_RESET_CNT) {
			if (gubResetFlag == 1) {
				gubResetCountDuringCmd ++;
			}
		}
		return;
	}

	if (ul_Temp_IntFlag & H_LNK_ESTB_CHG_INT) {
		if (HW[HW_SATA_INFO] &H_LNK_ESTABLISH) {
			HL[HL_INT_EN] &= ~H_LNK_ESTB_CHG_INT_EN; //不然會一直進來, 很煩
		}
		HL[HL_INT_FLAG] = (U32)H_LNK_ESTB_CHG_INT;	//W1C
		return;
	}

	if (ul_Temp_IntFlag & H_PHYRDY_CHG_INT) {
		HL[HL_INT_FLAG] = (U32)H_PHYRDY_CHG_INT;	//W1C

		return;
	}

	if (ul_Temp_IntFlag & H_ERR_EVT_INT) {

		volatile U32 ulErrorIntStatus = HL[HL_ERR_INT_FLAG];
		if (!BURNER) {
			//UartString("\n\rError  ,flag = 0x");
			// UartLongHex(HL[HL_ERR_INT_FLAG]);

			if ((ulErrorIntStatus & (H_D2H_D_RERR_INT | H_H2D_D_RERR_INT))) { //Data CRC
#if(!RDT)
				gubNeedChkDoneTag = 0;
#endif
				if (ulErrorIntStatus & H_D2H_D_RERR_INT) {
					/*
					/ 與Read有關 : (D2H指的是方向)
					/ BIT25 H2D/D2H(雙向) Data FIS R_ERR
					/ BIT5 有D2H R_ERR (不管是DMA or data or PIO setup...etc FIS)
					/ BIT9 D2H Data R_ERR
					*/
					if (ENABLE_KT_DEBUG_UART)
						UartString("\n\rCRC_R");
					//UartLLongHex(guoRCRCCount);

					VT->gulReadCRCCnt++;

					if (Failed_Test_CRC_RD)
						HW[HW_ERR_CTRL] &= ~H_REG_CRC_DATAERR_EN;

					HL[HL_ERR_INT_FLAG] = H_D2H_D_RERR_INT | H_LNK_DH_ERR_INT | H_TRS_DRERREND_INT; //用or會清掉其他bit~
					HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;
				}
				else if (ulErrorIntStatus & H_H2D_D_RERR_INT) {
					/*
					/ 與Write有關 :
					/ BIT25 H2D/D2H(雙向) Data FIS R_ERR
					/ BIT10 H2D Data FIS R_ERR (只要有發R_ERR,不一定是CRC)
					/ BIT14 H2D CRC Err (CRC error)
					/ BIT16 H2D Data FIS CRC R_ERR (因為CRC error而發R_ERR)
					*/
					if (ENABLE_KT_DEBUG_UART)
						UartString("\n\rCRC_W");
					//UartLLongHex(guoWCRCCount);

					VT->gulWriteCRCCnt++;
					gubForceStop |= BIT1;
					gubMultiTrigTag = 0xff;
					if (Failed_Test_CRC_WR)
						HW[HW_ERR_CTRL] &= ~(H_REG_CRC_DATAERR_EN | H_REG_CRC_DIR_RECV);

					HL[HL_ERR_INT_FLAG] = H_H2D_D_RERR_INT | H_H2D_RERR_CRC_INT | H_H2D_D_RERR_CRC_INT | H_TRS_DRERREND_INT;
					HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;
				}
				//下0x4,表示後面可能會接softReset...
				// ex  Read Data H2D  -->
				//                           <-- data
				//                           <-- data CRC
				//     SoftReset step 1, h2d ctl = 0x4 -->
				//                           此行ISR
				//     SoftReset step 2, h2d ctl = 0x0, real reset -->
				//     這種flow下 ctrl = 0x4時不做事
				// ex  Read Data H2D  -->
				//                           <-- data
				//                           <-- data CRC
				//                            此行ISR
				//     (可能有)(SoftReset step 1, h2d ctl = 0x4 -->)
				//     (可能有)(SoftReset step 2, h2d ctl = 0x0, real reset -->)
				//     這種flow下, 當成softreset處理
				if ((HB[HB_CONTROL] & 0x04) != 0x04) {
					/*
					                ****** CRC:
					* 1. 先卡住XRDY, 避免"判斷"下筆Cmd是否進來"後", 才收到CmdISR, 可能會造成fail D2H回給下一筆cmd.
					* 2. 關掉autocheck/set, 避免清掉flag後又被FQ重新設起來
					                * 3. 清掉buffer flag, 避免之後的Read/Write cmd被殘留的flag卡住
					                      Read : host尚未讀完data便進入此ISR, 執行完2 , 會導致殘留flag
					                      write :因為Write要收滿一個plane才會填BQ, 如果沒收滿就遇到WCRC, 會殘留flag
					* 4. 等沒有正在傳輸的FIS後(CRC 會使host發R_ERR, 而此FIS會使state machine 進入Idle, 即不再傳下筆FIS,  所以等到idle便可以傳, 不會有FIS傳到一半被reset的問題)
					* 5-1 下筆cmd已經進來(進IISR前Host就猴急先傳, 一般是PIO, 因為CRC發生就不會打D2H),已經不能回fail D2H, 所以打開(A)buffer flag as/ac (B)Blk FIS傳輸, 繼續下一筆cmd  (用APU_rst.. 會把下一筆cmd的info也清掉)
					* 5-2 若是PIO Read且已是最後一筆, 不打D2H (因為timing關係,有可能影響到下筆cmd.. 測試方式詳看下方註解)
					* 5-3 反之, APU reset, 不再傳data出去
					* 6. 設定error code
					*/
					HW[HW_ERR_CTRL] |= H_REG_XHOLD; //避免(PIO Read時)在ISR處理時尚未收到cmd isr, 一處理完就收到ISR, 造成判斷錯誤,要擋住host

					//if (ulErrorIntStatus & H_D2H_D_RERR_INT) {
					M_SetFlagSettingNone();
					AXIL[AXIL_SET_START_FLAG] = 0;
					AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //因為關掉autoSet/autoCheck, 可能w的FQ做完後對應的flag沒清
					AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;
					gubFirstCMDPending = 0;
					//}

					while ((HW[HW_SATA_INFO] & (H_TRS_IDLEST | H_LNK_IDLEST)) != (H_TRS_IDLEST | H_LNK_IDLEST));
					if (HL[HL_INT_FLAG]&H_RCV_CMD_INT) {
						//if (KT_Debug)
						//UartString("\n\r    NextCmd");

						M_SetFlagSettingSHF(); //已經開始收data, 所以要先把flag打開
						HW[HW_SYS_CTRL] &= ~H_BLK_FIS_EN; // 把發生error便擋下所有fis的開關關掉, 開始收/傳下筆data
						HW[HW_SYS_CTRL] |= H_BLK_FIS_EN;
					}
					else if (((HW[HW_CMD_INFO] & (H_NCQ_CMD | H_PIO_DMA_CMD)) == 0) && (HL[HL_CMD_REMAIN_SEC] == 0)) {
						//UartString("  ccc");  //若是在此加入小delay(Uart), 且以APU_reset處理 (即5-3,一般處理方式,會打fail D2H), test pattern用PIO seccnt = 1, 會發現打出的D2H影響到下次的cmd,造成error
						APU_Reset();
						M_SetFlagSettingSHF(); //已經開始收data, 所以要先把flag打開
						HW[HW_ERR_CTRL] &= H_REG_XHOLD_CLR;
						gubErrorCode = 0;
						return;
					}
					else {
						APU_Reset(); // Reset Application layer Unit, LastDoneTag由Error handle flow處理
					}

					//再判斷一次, 避免timing有誤 --> softreset進來後不須發error
					if ((HB[HB_CONTROL] & 0x04) != 0x04) {
						gubErrorCode = CRC_ERR;
					}
					else {
						HW[HW_ERR_CTRL] &= H_REG_XHOLD_CLR;
					}
				}
				else {
					if (ENABLE_KT_DEBUG)
						while (1);
				}

				if (HW[HW_CMD_INFO] & H_NCQ_CMD) {
					gubNCQINTRError = 1;
					gubErrorTag = (HB[HB_NCQ_SET] & H_CUR_TAG);

					memset(NCQ_READ_LOG_EXT_ERR, 0x0, 16);
					NCQ_READ_LOG_EXT_ERR[0]  = gubErrorTag;
					NCQ_READ_LOG_EXT_ERR[2]  = 0x41;
					NCQ_READ_LOG_EXT_ERR[3]  =  HB[HB_ERROR];
					NCQ_READ_LOG_EXT_ERR[4]  = (UBYTE)NLL[gubErrorTag];
					NCQ_READ_LOG_EXT_ERR[5]  = (UBYTE)(NLL[gubErrorTag] >> 8);
					NCQ_READ_LOG_EXT_ERR[6]  = (UBYTE)(NLL[gubErrorTag] >> 16);
					NCQ_READ_LOG_EXT_ERR[7]  = (((UBYTE)((NLL[gubErrorTag] >> 16)&BIT0)) << 7);
					NCQ_READ_LOG_EXT_ERR[8]  = (UBYTE)(NLL[gubErrorTag] >> 24);
					NCQ_READ_LOG_EXT_ERR[12] = (UBYTE)NLL[gubErrorTag];
					NCQ_READ_LOG_EXT_ERR[13] = (UBYTE)(NLL[gubErrorTag] >> 8);
				}
			}

			else if (ulErrorIntStatus & H_H2D_ND_RERR_NCRC_INT) { //H2D CRC
				if (ENABLE_KT_DEBUG_UART) {
					UartString(" H_RD_CRC_INT2");
				}
				VT->gulNonDataCRCErrCnt++;
				//while(1);
				HL[HL_ERR_INT_FLAG] = H_H2D_ND_RERR_NCRC_INT;
				HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;

				while ((HW[HW_SATA_INFO] & (H_TRS_IDLEST | H_LNK_IDLEST)) != (H_TRS_IDLEST | H_LNK_IDLEST));    //S9 小明說要等
				APU_Reset(); // Reset Application layer Unit, LastDoneTag直接清掉
				gubLastDoneTag = 0x3F;
				gulSATA_MSG_TYPE3[0].All = 0;
			}

			/* Link Lost*/
			else if (ulErrorIntStatus & H_LNK_NOCOMERR_INT) {
				//if (KT_Debug)
				//UartString("\r\nNCERR");
				if (Enable_LLAP) {
					idle(40000000); //vic實驗一個nop約75ns, 這邊先等三秒. 之後要改成可以MP設定開關以及秒數
				}

				VT->gulNoComeERR++;
				//當斷Link發生但nocomErr還沒來的話,HW有可能會收垃圾資料進來,所以一定要擋.
				if (gulNCQWRCmdTriggerCnt != 0) {
					//DCCMSATAB[SATAB_RD_CNT] represent number of valid tags. Dont drop.
					gubNCQWRCmdTriggerPassCnt = DCCMSATAB[SATAB_RD_CNT];
					gubForceStop |= BIT1;
				}
				HL[HL_ERR_INT_FLAG] = H_LNK_NOCOMERR_INT;
				HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;
				if ((ul_Temp_IntFlag & H_LNK_COMRI_INT) || (gubHardReset)) {
				}
				else {
					HL[HL_INT_FLAG] = H_LNK_ESTB_CHG_INT;
					if (HW[HW_SATA_INFO] & H_LNK_ESTABLISH) {}
					else {
						gubLinkLost = 1;
						gubNeedChkDoneTag = 0;
						gubNeedSendCmdComplete = 0;
						gubHostCmdIn = 0;
						gubFirstCMDPending = 0;
						HW[HW_ERR_CTRL] |= (H_REG_XHOLD | H_BLK_TRG);

						//while ((HW[HW_SATA_INFO] & (H_TRS_IDLEST | H_LNK_IDLEST)) != (H_TRS_IDLEST | H_LNK_IDLEST));    //都斷Link了
						APU_Reset(); // Reset Application layer Unit
						gubLastDoneTag = 0x3F;
						gulSATA_MSG_TYPE3[0].All = 0;

						M_SetFlagSettingNone();
						AXIL[AXIL_SET_START_FLAG] = 0;
						AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM;
						AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
						HL[HL_INT_EN] |= H_LNK_ESTB_CHG_INT_EN;

						//gubErrorCode = CMD_ABORT; Link沒建也送不出去
					}
				}
			}
			/*
			****** UNC:
			* 1. 等沒有正在傳輸的FIS後 (Flag卡), APU reset, 不再傳data出去
			* 2. 關掉autocheck/set, 避免清掉flag後又被FQ重新設起來
			* 3. 清掉Read buffer flag, 避免之後的Read cmd被殘留的flag卡住
			* 4. 設定error code
			* ps. 若是1要拉到2,3後, 記得0x04007022[5]要舉起, 阻擋發生Unc後的所有FIS
			*/
			else  if (ulErrorIntStatus & H_RD_UNC_INT) {
				if (ENABLE_KT_DEBUG_UART)
					UartString("\r\nUNC");
				//UartLLongHex(guoUncCount);
#if(!RDT)
				gubNeedChkDoneTag = 0;
#endif
				VT->gulHostReadECCCnt++;


				U32 uxx;
				for (uxx = 0; uxx < FLA_READUNC_MAXLOG_CNT; uxx++) {
					if ( gulSATAUncAddr[uxx] == HL[HL_RD_UNC_ADDR]) {
						break;
					}
				}
				if (uxx == 32) {
					M_AssertError(0x5566);
					uxx = 0;
				}

				//依Sepc說法, 應該先清INT_EN (0x40), 再清ERR_INT_FLAG(0x44), 再清INT_FLAG
				M_Clear_UNC_Addr(); //因為打開H_RD_UNC_INT_EN後,會去比對axi flag跟H_RD_UNC_ADDR, 若不先歸0, 打開後又會觸發一次Unc ISR
				HL[HL_ERR_INT_EN] &= (~H_RD_UNC_INT_EN);
				HL[HL_ERR_INT_FLAG] = H_RD_UNC_INT;
				HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;
				HL[HL_ERR_INT_EN] |= H_RD_UNC_INT_EN;

				gubErrorCode = DATA_ERR;
				gulUNCLBA = gulFailLBAforLog[uxx];

				if (HW[HW_CMD_INFO]&H_NCQ_CMD) {
					//H_REG_XHOLD: don't send R_RDY when Host X_RDY, 擋下一個Cmd
					//H_BLK_TRG: eg.若是在ReadSector中的檢查NeedChkDoneTag = 0到Trigger TQ前進中斷, 就會繼續trigger data, 所以要block
					HW[HW_ERR_CTRL] |= (H_REG_XHOLD);
					gubNCQINTRError = 1;
					//gubD2HMask = 1;
					gubErrorTag = (HB[HB_NCQ_SET] & H_CUR_TAG); //現在做到的
					//因為APU在看flag時就可能進中斷(data尚未傳), 所以直接記
					//另外如果有多的錯誤LBA, 反正這些LBA的data都沒傳完, log裡任意記一組正確的就好
					memset(NCQ_READ_LOG_EXT_ERR, 0x0, 16);
					NCQ_READ_LOG_EXT_ERR[0]  = gubErrorTag;
					NCQ_READ_LOG_EXT_ERR[2]  = 0x41;
					NCQ_READ_LOG_EXT_ERR[3]  = gubErrorCode;
					NCQ_READ_LOG_EXT_ERR[4]  = (UBYTE)gulFailLBAforLog[uxx];
					NCQ_READ_LOG_EXT_ERR[5]  = (UBYTE)(gulFailLBAforLog[uxx] >> 8);
					NCQ_READ_LOG_EXT_ERR[6]  = (UBYTE)(gulFailLBAforLog[uxx] >> 16);
					NCQ_READ_LOG_EXT_ERR[7]  = (((UBYTE)((NSL[gubErrorTag] >> 16)&BIT0)) << 7); //H_NCQN_FUA
					NCQ_READ_LOG_EXT_ERR[8]  = (UBYTE)(gulFailLBAforLog[uxx] >> 24);
					//NCQ_READ_LOG_EXT_ERR[12] = (UBYTE)NCQ_TAG_INFO_LONG[gubErrorTag].All;
					//NCQ_READ_LOG_EXT_ERR[13] = (UBYTE)(NCQ_TAG_INFO_LONG[gubErrorTag].All>>8);
					
					NCQ_READ_LOG_EXT_ERR[12] = (UBYTE)HL[HL_CMD_REMAIN_SEC];
					NCQ_READ_LOG_EXT_ERR[13] = (UBYTE)(HL[HL_CMD_REMAIN_SEC] >> 8);					
				}
				while ((HW[HW_SATA_INFO] & (H_TRS_IDLEST | H_LNK_IDLEST)) != (H_TRS_IDLEST | H_LNK_IDLEST));    //S9 小明說要等

				SYSW[SYS0W_XDB_PSEL] &= 0xf000;
				HB[0x271] = 0x1;

				if ((HL[HL_DBG_PORT]&BIT13) && HL[HL_NCQ_VLD_BIT]) {
					if (HL[HL_INT_FLAG]&H_RCV_CMD_INT) { //cmd isr
						if (HW[HW_CMD_INFO] & (H_LBA_ERR | H_DUP_NCQ_TAG)) { // error
							APU_Reset();
							HB[HB_ERROR] = ID_ERR;
							HB[HB_STATUS] = 0X41;
							HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
							while (HB[HB_REG_REQ]&H_REG_REQ);
						}
						else {
							APU_Reset();
							HB[HB_ERROR] = 0X0;
							HB[HB_STATUS] = 0X40;
							HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
							while (HB[HB_REG_REQ]&H_REG_REQ);
						}
						HL[HL_INT_FLAG] = H_RCV_CMD_INT; //W1C
					}
					else {
						APU_Reset();
						HB[HB_ERROR] = 0X0;
						HB[HB_STATUS] = 0X40;
						HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
						while (HB[HB_REG_REQ]&H_REG_REQ);
					}
				}
				else {
					APU_Reset(); // Reset Application layer Unit
				}

				M_SetFlagSettingNone();
				AXIL[AXIL_SET_START_FLAG] = 0;
				AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM;
				AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;
				gubFirstCMDPending = 0;

				//H_CONT_LBA_CHK_CMD不僅指沒有check是否continue,也包含HW不Decode的cmd. 此類cmd (ex. stanby)我們預設要打D2H
				//I.
				//09時, DMA 可能有host已經指到到Unc位置, 但之前的data FIS尚未傳完的情形.
				//       因此若直接下APU Reset, 會有"Data FIS"傳到一半(eg. 只傳408 byte)就終止的問題
				//       (DDT是湊到8k再傳, 但依然會有傳到一半時, host便指到Unc Adr的問題)
				//       所以S9讓整個Xfer完成再打Error D2H
				//       PIO則是指到哪讀到哪, 不會有問題
				//11的話, 因為進Unc ISR後, TQ會自動Idle, 所以等H_TRS_IDLEST/H_LNK_IDLEST舉起(1 表示idle), 便代表此次Data FIS (8k)傳輸已經結束,
				//      正在等待buffer flag(需用data trig)或是reset
				//      此時下reset便沒有問題
				//II.
				//apu reset後autoD2h不會為此次cmd送D2h, 所以也不需另外disable
				//III.
				//因為data會被flag卡住, 所以不會有處理最後一筆SecCnt時已經有下筆Cmd進來的問題

			}
			/*
			****** E3D:
			* 1. 先卡住XRDY, 避免"判斷"下筆Cmd是否進來"後", 才收到CmdISR, 可能會造成fail D2H回給下一筆cmd.
			* 2. 關掉autocheck/set, 避免清掉flag後又被FQ重新設起來
			* 3. 清掉Read buffer flag, 避免之後的Read cmd被殘留的flag卡住
			* 4. 等沒有正在傳輸的FIS後 (發生E3D後, HW預設會停掉cmd的傳輸)
			* 5-1 下筆cmd已經進來(進IISR前Host就猴急先傳, 一般是PIO, 因為CRC發生就不會打D2H),已經不能回fail D2H, 所以打開(A)buffer flag as/ac (B)Blk FIS傳輸, 繼續下一筆cmd  (用APU_rst.. 會把下一筆cmd的info也清掉)
			* 5-2 若是PIO Read且已是最後一筆, 不打D2H (因為timing關係,有可能影響到下筆cmd.. 測試方式詳看下方註解)
			* 5-3 反之, APU reset, 不再傳data出去
			* 6. 設定error code
			*/
			else  if (ulErrorIntStatus & H_E3D_INT) {
				if (ENABLE_KT_DEBUG_UART) {
					UartString("\r\nE3D");
					//UartLLongHex(guoE3DCount);
				}
#if(!RDT)
				gubNeedChkDoneTag = 0;
#endif
				VT->gulHostReadE3DCnt++;

				if (Failed_Test_E3D)
					HW[HW_MZ_CTRL] &= H_E3D_DIS;

				HL[HL_ERR_INT_FLAG] = H_E3D_INT;//再清ERR_INT_FLAG (W1C) flag不先清的話, APU_RST似乎會無效?
				HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT; //最後清INT_FLAG

				HW[HW_ERR_CTRL] |= H_REG_XHOLD;

				M_SetFlagSettingNone();
				AXIL[AXIL_SET_START_FLAG] = 0; //The register stands for the flag start index
				AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //因為關掉autoSet/autoCheck, 可能w的FQ做完後對應的flag沒清
				AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
				gubFirstCMDPending = 0;

				while ((HW[HW_SATA_INFO] & (H_TRS_IDLEST | H_LNK_IDLEST)) != (H_TRS_IDLEST | H_LNK_IDLEST));
				if (HL[HL_INT_FLAG]&H_RCV_CMD_INT) {
					HW[HW_MZ_CTRL] |= H_E3D_ERR;
					/*
					* E3D如果是最後一筆, 會因為APU判定已經做完而有DoneTag
					* Unc會卡flag, 沒有DoneTag
					* CRC因為有R_ERR, 會認定是Error finish而沒有DoneTag (0x040070fc與MsgDispatch功能相同, 但是互相獨立, 是作為backup用, 如果要用0x040070fc,須手動清掉其done tag bit)
					*/

					//while (DCCMSATAB[SATAB_RD_CNT]) { 搬到R/W flow一起做
					//DCCMSATAB[SATAB_RPIU] = 1;
					//}

					//後面的cmd已經下來,不能用APU_rst.. 會把下一筆cmd的info也清掉
					//把發生error便擋下所有fis的開關關掉, 讓sata可以收下筆cmd
					M_SetFlagSettingSHF(); //已經開始收data, 所以要先把flag打開
					HW[HW_SYS_CTRL] &= ~H_BLK_FIS_EN;
					HW[HW_SYS_CTRL] |= H_BLK_FIS_EN;
				}
				else if (((HW[HW_CMD_INFO] & (H_NCQ_CMD | H_PIO_DMA_CMD)) == 0) && (HL[HL_CMD_REMAIN_SEC] == 0)) {
					if (ENABLE_KT_DEBUG_UART) {
						//UartString(" ,bbb");  //若是在此加入小delay(Uart), 且以APU_reset處理 (即5-3,一般處理方式,會打fail D2H), test pattern用PIO seccnt = 1, 會發現打出的D2H影響到下次的cmd,造成error
					}

					//while (DCCMSATAB[SATAB_RD_CNT]) { 搬到R/W flow一起做
					//DCCMSATAB[SATAB_RPIU] = 1; // E3D如果是最後一筆, 會因為APU判定已經做完而有DoneTag
					//}

					APU_Reset();
					M_SetFlagSettingSHF();
					HW[HW_ERR_CTRL] &= H_REG_XHOLD_CLR;
					gubErrorCode = 0;
					return;
				}
				else {
					APU_Reset();  // Reset Application layer Unit
				}

				gubErrorCode = CMD_ABORT;

				if (HW[HW_CMD_INFO]&H_NCQ_CMD) {
					gubNCQINTRError = 1;
					gubErrorTag = (HB[HB_NCQ_SET] & H_CUR_TAG); //現在做到的
					U32 ulLBA = (NLL[gubErrorTag] + HL[HL_BUF_TRG_SEC] - HL[HL_CMD_REMAIN_SEC]);

					memset(NCQ_READ_LOG_EXT_ERR, 0x0, 16);
					NCQ_READ_LOG_EXT_ERR[0]  = gubErrorTag;
					NCQ_READ_LOG_EXT_ERR[2]  = 0x41;
					NCQ_READ_LOG_EXT_ERR[3]  = HB[HB_ERROR];
					NCQ_READ_LOG_EXT_ERR[4]  = (UBYTE)ulLBA;
					NCQ_READ_LOG_EXT_ERR[5]  = (UBYTE)(ulLBA >> 8);
					NCQ_READ_LOG_EXT_ERR[6]  = (UBYTE)(ulLBA >> 16);
					NCQ_READ_LOG_EXT_ERR[7]  = (((UBYTE)((NSL[gubErrorTag] >> 16)&BIT0)) << 7); //H_NCQN_FUA
					NCQ_READ_LOG_EXT_ERR[8]  = (UBYTE)(ulLBA >> 24);
					//NCQ_READ_LOG_EXT_ERR[12] = (UBYTE)NCQ_TAG_INFO_LONG[gubErrorTag].All;
					//NCQ_READ_LOG_EXT_ERR[13] = (UBYTE)(NCQ_TAG_INFO_LONG[gubErrorTag].All>>8);
					NCQ_READ_LOG_EXT_ERR[12] = (UBYTE)HL[HL_CMD_REMAIN_SEC];
					NCQ_READ_LOG_EXT_ERR[13] = (UBYTE)(HL[HL_BUF_TRG_SEC] >> 8);
				}
			}
			/*處理方式與CRC同, H_TRS_SYNCRCV_INT:SYNC Escape in Data FIS Interrupt, H_TRS_FISACK_SYNC_INT:SYNC Escape in non-Data FIS Interrupt*/
			else if (ulErrorIntStatus & (H_TRS_SYNCRCV_INT | H_TRS_FISACK_SYNC_INT)) {
				//if (KT_Debug)
				//UartString("\r\nSync Escape");

				VT->gulSynEscape++;
				gubForceStop |= BIT0;
				if (ulErrorIntStatus & H_TRS_SYNCRCV_INT)
					HL[HL_ERR_INT_FLAG] = H_TRS_SYNCRCV_INT;
				if (ulErrorIntStatus & H_TRS_FISACK_SYNC_INT)
					HL[HL_ERR_INT_FLAG] = H_TRS_FISACK_SYNC_INT;
				HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT; //最後清INT_FLAG


				if (gubLinkLost == 0) {
					while ((HW[HW_SATA_INFO] & (H_TRS_IDLEST | H_LNK_IDLEST)) != (H_TRS_IDLEST | H_LNK_IDLEST));    //S9 小明說要等

					gubLinkLost = 1;
					gubNeedChkDoneTag = 0;
					gubNeedSendCmdComplete = 0;
					gubHostCmdIn = 0;
					HW[HW_ERR_CTRL] |= (H_REG_XHOLD | H_BLK_TRG);

					APU_Reset();
					gubLastDoneTag = 0x3F;
					gulSATA_MSG_TYPE3[0].All = 0;

					M_SetFlagSettingNone(); //把flag setting關掉, 之後的FQ才不會被卡住
					AXIL[AXIL_SET_START_FLAG] = 0; //The register stands for the flag start index
					AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //The register stands for the flag length that you want to set/clear.
					AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag

					gubFirstCMDPending = 0;
					gubErrorCode = CMD_ABORT;
				}
			}
			else if (ulErrorIntStatus & (H_LNK_DB_ERR_INT | H_LNK_DD_ERR_INT)) {//8b10b發生後CRC可能會發生
				if (ulErrorIntStatus & (H_LNK_DB_ERR_INT)) {
					HL[HL_ERR_INT_EN] &= (~H_LNK_DB_ERR_INT_EN);
					HL[HL_ERR_INT_FLAG] = H_LNK_DB_ERR_INT;
				}
				if (ulErrorIntStatus & (H_LNK_DD_ERR_INT)) {
					HL[HL_ERR_INT_EN] &= (~H_LNK_DD_ERR_INT_EN);
					HL[HL_ERR_INT_FLAG] = H_LNK_DD_ERR_INT;
				}
				idle(150);
				gubForceStop |= BIT0;
				HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;
				HL[HL_ERR_INT_EN] |= H_LNK_DD_ERR_INT_EN;
				HL[HL_ERR_INT_EN] |= H_LNK_DB_ERR_INT_EN;
			}
			/*之後處理*/
			else {
				if (ENABLE_KT_DEBUG_UART) {
					//UartString("\r\nno sup,  0x");
					//UartLongHex(ulErrorIntStatus);
					//flaLEDBlink(0x4);
				}
				//其他的依照建輝的說法不避處理, S9也沒處理
				//HL[HL_ERR_INT_FLAG] = ulErrorIntStatus;
				//HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;
				/* S9也沒處理
				flaLEDBlink(0x4);
				*/
				U32 ulTemp2;
				ulTemp2  = HL[HL_ERR_INT_FLAG];
				HL[HL_ERR_INT_FLAG] = ulTemp2;
				HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;
			}
		}
		else {
			U32 ulTemp2;
			ulTemp2  = HL[HL_ERR_INT_FLAG];
			HL[HL_ERR_INT_FLAG] = ulTemp2;
			HL[HL_INT_FLAG] = (U32)H_ERR_EVT_INT;
			if (BURNER) {
				if (HW[HW_MZ_CTRL]&H_E3D_ERR)
					HW[HW_MZ_CTRL]  |= H_E3D_ERR;
			}

		}
		return ;
	}

	if (gubEnable_CTRL_INT_For_CBitZeroCase) {
		if ((ul_Temp_IntFlag & H_RCV_CTRL_INT) && (HB[HB_COMMAND]) && (!HB[HB_CONTROL])) { //因為特殊老舊平台會有 C bit = 0 但是又有 cmd op code的錯誤 Case →此筆不做
			HL[HL_INT_FLAG] = H_RCV_CTRL_INT; //W1C
			HW[HW_SYS_CTRL] &= ~(H_BLK_FIS_EN); //先clear再set回來, 用來清掉 HW 內部舉的 BLK_FIS bit
			HW[HW_SYS_CTRL] |= H_BLK_FIS_EN;

			HW[HW_DATA_TRG] |= H_HOLD_NCQ_DIS; //先set再clear掉 (0 = Enable)
			HW[HW_DATA_TRG] &= H_HOLD_NCQ_EN;

			return ;
		}
		//某些特殊平台，會在剛開始下Identify之前先打一筆C bit = 0之 dummy control fis，此筆並非做soft reset的fis，故需要多判H_SRST_CLR_INT和HB_CONTROL的BIT2區隔 ->此種dummy control fis 不做
		if ((ul_Temp_IntFlag & H_RCV_CTRL_INT) && ((ul_Temp_IntFlag & H_SRST_CLR_INT) != H_SRST_CLR_INT) && ( (HB[HB_CONTROL] & BIT2) != BIT2)) {
			HL[HL_INT_FLAG] = H_RCV_CTRL_INT; //W1C
			HW[HW_SYS_CTRL] &= ~(H_BLK_FIS_EN); //先clear再set回來, 用來清掉 HW 內部舉的 BLK_FIS bit
			HW[HW_SYS_CTRL] |= H_BLK_FIS_EN;
			return ;
		}
	}

	if (ul_Temp_IntFlag & H_RCV_CMD_INT) {
#if BURNER
		if (gubPreformatTurbo) {
			HL[HL_INT_FLAG] = H_RCV_CMD_INT; //W1C
			if (gubPreformatunfinish || (HB[HB_COMMAND] != 0x31) || (HB[HB_FEATURE] != 0xCC) ) {

				if (HW[HW_CMD_INFO]&H_NCQ_CMD) {
					HB[HB_STATUS] = 0x41;
				}
				else {
					HB[HB_STATUS] = 0x51;
				}
				HB[HB_ERROR] = CMD_ABORT;
				HB[HB_LBA_L] = 0;
				HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
				while (HB[HB_REG_REQ]&H_REG_REQ);
				return ;
			}
			else {
				gubPreformatTurbo = 0;
				HL[HL_BUF_OFST] = 0;
				HL[HL_TQ_CONTENT] = 0;
				while (HL[HL_CMD_REMAIN_SEC ]);
				while (HL[HL_AXI_FIS_STAT]&BIT23);
				HB[HB_STATUS] = 0x50;
				if (gubErrorCode == 0) {
					HB[HB_LBA_L] = 0;
				}
				HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
				while (HB[HB_REG_REQ]&H_REG_REQ);
				if (gubErrorCode) {     //打完error D2H後, 設回原值
					HB[HB_STATUS] = 0x50;
					HB[HB_ERROR] = NO_ERROR; //這行沒設的話, CKtool or 其他會polling error reg的都會出錯 (因為像是PIO 不會打D2H的, 都要在這重設)
					gubErrorCode = 0;

					/*非NCQ的話,做完就等於flag結束*/
					if (!(gubSoftReset | gubHardReset)) {
						//M_SetFlagSettingSHF();  //在ISR清掉,但因為可能沒進Reset, 所以這邊要舉
						HW[HW_ERR_CTRL] &= ~H_REG_XHOLD; //為了PIO Read最後一筆發生CRC十擋下的XRDY, 這邊要打開
					}
				}
				return ;
			}
		}
#endif
		if (ENABLE_KT_DEBUG_UART) {
			UartString("\n\rCMD isr, 0x");
			Uart_Tx_DataHex(HB[HB_COMMAND]);
		}

		gubCmdCode = HB[HB_COMMAND];
		HL[HL_INT_FLAG] = H_RCV_CMD_INT; //W1C
#if(!BURNER)
		if (gubIgnoreFlag == 1) {
			return;
		}
#endif
		/*
		if(HB[HB_NCQ_OVLP_NUM] == 0){
		gubHostCmdIn = 1;
		}
		*/
		if (!BURNER) {
			gubLastPowerMode = gubPowerMode;
			if (gubPowerMode == PWR_SLEEP) {
				HB[HB_STATUS] = 0x51;
				HB[HB_ERROR] = CMD_ABORT;
				HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
				while (HB[HB_REG_REQ]&H_REG_REQ);
				return;
			}

			if (gubNCQINTRError) { //處理NCQ error中,除了ReadLog與ComReset外,皆視為無效
				UBYTE ubBLKTRGTemp;
				ubBLKTRGTemp = (HW[HW_ERR_CTRL] & H_BLK_TRG) ? 1 : 0;

				if (((gubCmdCode == 0x2f) || ((gubCmdCode == 0x47) && (IDT.W119 & BIT3))) && (HB[HB_LBA_L] == 0x10)) { //LBA low 是 feature...
					// read log ext...
					gubHostCmdIn = 1;
				}
				else {
					if (HW[HW_CMD_INFO]&H_NCQ_CMD) {
						HB[HB_STATUS] = 0x41;
					}
					else {
						HB[HB_STATUS] = 0x51;
					}
					HB[HB_ERROR] = CMD_ABORT;

					if (ubBLKTRGTemp) {
						HW[HW_ERR_CTRL] &= (~H_BLK_TRG);
					}
					/*
					有可能comreset已經下來, 如果回abort D2H會變成回給Comreset.. 所以這邊要回去做comreset flow
					*/
					if (HL[HL_INT_FLAG]&H_LNK_COMRI_INT) {
						//if (KT_Debug)
						//UartString("\n\rgoto COMRI_INT 2");
						goto Mark_COMRESET;
					}

					HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
					while (HB[HB_REG_REQ]&H_REG_REQ) {
						/*
						@ IRST 13
						有可能在回fail SDB後, host還是下NCQ cmd.
						這邊照理來說我們要回abort d2h. 但因為host覺得他其實應該下comreset
						所以對我們的abort D2H會不回R_OK, 而直接下comreset, 此舉 導致sata ip不清H_REG_REQ. (須收到R_OK或是R_ERR才會清)
						*/
						if (HL[HL_INT_FLAG]&H_LNK_COMRI_INT) {
							//UartString("\n\rgoto COMRI_INT 1");
							goto Mark_COMRESET;
						}
					}

					if (HW[HW_CMD_INFO]&H_NCQ_CMD) {
						while ((HW[HW_SATA_INFO] & (H_TRS_IDLEST | H_LNK_IDLEST)) != (H_TRS_IDLEST | H_LNK_IDLEST));
						APU_Reset(); // Reset Application layer Unit, 怕後面有一串cmd要收
						gubLastDoneTag = 0x3F; //其實前面有errir應該就清過一次了
						gulSATA_MSG_TYPE3[0].All = 0;
					}
					HB[HB_ERROR] = NO_ERROR;
				}

				if (ubBLKTRGTemp) {
					HW[HW_ERR_CTRL] |= H_BLK_TRG;	// until READ_LOG_EXT or COMRESET handler to release
				}

				return;
			}
		}
		if (!(HW[HW_CMD_INFO]&H_NCQ_CMD)) {
			gubHostCmdIn = 1;
		}
		else if (HW[HW_CMD_INFO]&H_NCQ_LBA_OVLP) {
			if (!BURNER) {
				VT->gulTestCount[8] ++; // NCQ overlap
				gubLBAOverlap = 1;
				gubNeedWaitTQDone = 1;
			}
		}
		if (!BURNER) {
			//if (HB[HB_NCQ_ORDER_VLD_SUM]) {  //valid sum是表示還沒Trig的有幾筆, 所以若是已被trig, 看這個會不準
			if (HL[HL_NCQ_VLD_BIT]) {  //還有幾筆pending
				if (ENABLE_KT_DEBUG_UART) {
					UartString("\r\nIns ");
					if (HW[HW_CMD_INFO]&H_NCQ_CMD) {
						UartString(" NCQCMDINFO = 0x");
						UartWordHex(HW[HW_CMD_INFO]);
					}
					else {
						UartString(" CMD = 0x");
						Uart_Tx_DataHex(HB[HB_COMMAND]);
					}
				}
				// NCQ doing, recv non-NCQ cmd.
				if (!(HW[HW_CMD_INFO]&H_NCQ_CMD)) {
					M_SetFlagSettingNone();
					AXIL[AXIL_SET_START_FLAG] = 0;
					AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //如果是要寫的, data已收完才會有BQ -> FQ, 所以清掉也可
					AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
					gubFirstCMDPending = 0;

					while ((HW[HW_SATA_INFO] & (H_TRS_IDLEST | H_LNK_IDLEST)) != (H_TRS_IDLEST | H_LNK_IDLEST));
					APU_Reset();

					HB[HB_STATUS] = 0x51;
					HB[HB_ERROR] = SET_ABRT;

					gubHostCmdIn = 0;

					gubNCQINTRError = 1;

					gubNeedChkDoneTag = 0;
					gubLastDoneTag = 0x3F;
					gulSATA_MSG_TYPE3[0].All = 0;

					gubErrorTag = (HB[HB_CMD_INFO_HIGHBYTE] & H_CUR_TAG);
					memset(NCQ_READ_LOG_EXT_ERR, 0x0, 16);
					NCQ_READ_LOG_EXT_ERR[0]  = 0x80;
					NCQ_READ_LOG_EXT_ERR[1]  = 0x1;
					NCQ_READ_LOG_EXT_ERR[2]  = 0x51;
					NCQ_READ_LOG_EXT_ERR[3]  = SET_ABRT;
					NCQ_READ_LOG_EXT_ERR[4]  = HB[HB_LBA_L];
					NCQ_READ_LOG_EXT_ERR[5]  = HB[HB_LBA_M];
					NCQ_READ_LOG_EXT_ERR[6]  = HB[HB_LBA_H];
					NCQ_READ_LOG_EXT_ERR[7]  = HB[HB_DEVICE];
					NCQ_READ_LOG_EXT_ERR[8]  = HB[HB_LBA_L_EXP];
					NCQ_READ_LOG_EXT_ERR[12] = HB[HB_SECTOR_CNT];
					NCQ_READ_LOG_EXT_ERR[13] = HB[HB_SECTOR_CNT_EXP];

					HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
					while (HB[HB_REG_REQ]&H_REG_REQ);
					HW[HW_ERR_CTRL] |= H_BLK_TRG;
				}

				else {      // duplicate Tag or out of range.
					if (( (HW[HW_CMD_INFO] & (H_LBA_ERR | H_DUP_NCQ_TAG)) != 0)
					        || (AtaCfg.ubSecMode & SET_SEC_LOCK)
					        || ((AtaCfg.ubSanitizeState != SD0_SANITIZE_IDLE) && (AtaCfg.ubSanitizeState != SD1_SANITIZE_FROZEN))
					        || (gubNCQSupport == 0)) {

						if (gubFirstCMDIn && (HW[HW_CMD_INFO] & H_LBA_ERR)) {
							U32 ulsec;
							ulsec = HW[HW_FEATURE];
							if (ulsec == 0) {
								ulsec = 65536;
							}
							if ((HL[HL_CMD_S_LBA] < gulMediaSize.All) && ((HL[HL_CMD_S_LBA] + ulsec - 1) < gulMediaSize.All)) {
								HB[HB_ERROR] = NO_ERROR;
								HB[HB_STATUS] = 0x40;
								HB[HB_REG_REQ] = H_REG_REQ; // cheney
								while (HB[HB_REG_REQ]&H_REG_REQ) {
									//有可能送之前, APU指到Unc, 先離開 ,等進ffail handle flow再處理
									if (HL[HL_INT_FLAG]&H_ERR_EVT_INT) {
										break;
									}
								}
							}
							else {
								//真的有out of range
								goto Mark_LBAErr;
							}
						}
						else {
						Mark_LBAErr:
							M_SetFlagSettingNone();
							AXIL[AXIL_SET_START_FLAG] = 0;
							AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM; //如果是要寫的, data已收完才會有BQ -> FQ, 所以清掉也可
							AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
							gubFirstCMDPending = 0;

							while ((HW[HW_SATA_INFO] & (H_TRS_IDLEST | H_LNK_IDLEST)) != (H_TRS_IDLEST | H_LNK_IDLEST));
							APU_Reset();

							//HB[HB_ERROR] = SET_ABRT;

							if (HW[HW_CMD_INFO] & H_LBA_ERR) {
								HB[HB_ERROR]  = ID_ERR;
							}
							else {
								HB[HB_ERROR] = SET_ABRT;
							}

							HB[HB_STATUS] = 0x41;

							gubHostCmdIn = 0;

							gubNCQINTRError = 1;

							gubNeedChkDoneTag = 0;
							gubLastDoneTag = 0x3F;
							gulSATA_MSG_TYPE3[0].All = 0;

							gubErrorTag = (HB[HB_CMD_INFO_HIGHBYTE] & H_CUR_TAG);
							memset(NCQ_READ_LOG_EXT_ERR, 0x0, 16);
							NCQ_READ_LOG_EXT_ERR[0]  = (gubErrorTag & 0x1f);
							NCQ_READ_LOG_EXT_ERR[2]  = 0x41;
							NCQ_READ_LOG_EXT_ERR[3]  = HB[HB_ERROR];
							NCQ_READ_LOG_EXT_ERR[4]  = (UBYTE)NLL[gubErrorTag];
							NCQ_READ_LOG_EXT_ERR[5]  = (UBYTE)(NLL[gubErrorTag] >> 8);
							NCQ_READ_LOG_EXT_ERR[6]  = (UBYTE)(NLL[gubErrorTag] >> 16);
							NCQ_READ_LOG_EXT_ERR[7]  = (((UBYTE)((NSL[gubErrorTag] >> 16)&BIT0)) << 7);
							NCQ_READ_LOG_EXT_ERR[8]  = (UBYTE)(NLL[gubErrorTag] >> 24);
							NCQ_READ_LOG_EXT_ERR[12] = (UBYTE)NSL[gubErrorTag];
							NCQ_READ_LOG_EXT_ERR[13] = (UBYTE)(NSL[gubErrorTag] >> 8);

							HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
							while (HB[HB_REG_REQ]&H_REG_REQ);
							HW[HW_ERR_CTRL] |= H_BLK_TRG;
						}
					}

					else {
						HB[HB_ERROR] = NO_ERROR;
						HB[HB_STATUS] = 0x40;
						HB[HB_REG_REQ] = H_REG_REQ; // cheney
						while (HB[HB_REG_REQ]&H_REG_REQ) {
							//有可能送之前, APU指到Unc, 先離開 ,等進ffail handle flow再處理
							if (HL[HL_INT_FLAG]&H_ERR_EVT_INT) {
								break;
							}
						}
					}

					//while (HB[HB_REG_REQ]&H_REG_REQ);
					HB[HB_ERROR] = NO_ERROR;
					gubFirstCMDIn = 0;
				}
			}
		}
	}


#if ENABLE_D2HSDBByHW
	if ( (!gubNCQINTRError) && ((AtaCfg.ubSecMode & SET_SEC_LOCK) == 0) && (gubPowerMode != PWR_SLEEP) && (gubIsWriteProtect == FALSE) && ((AtaCfg.ubSanitizeState == SD0_SANITIZE_IDLE) || (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN)) && (gubNCQSupport)) {
		M_EnableHWD2H_NCQ(); //NCQ不會進ISR... 進ISR表示沒有自動打SDB, 所以在這邊補開
	}
#endif
}

void DDR_ISR()
{
#if(!RDT)
	UBYTE ubi;

	gulCountDDRINT++;
	/*if (ENABLE_DEBUG_UART) {
	        UartString("\nDDR ISR  ");
	        ubError = RAML[RAML_DDR3_ECC_CTRL] & 0xFF;
	        if (ubError & Z1_UN_CORRECT_INT) {
	                UartString("\nZ1 : ");
	                UartLongHex(RAML[RAML_DDR3_Z1_ECC_UN_CORRECT_ADDR]);
	        }
	        if (ubError & Z2_UN_CORRECT_INT) {
	                UartString("\nZ2 : ");
	                UartLongHex(RAML[RAML_DDR3_Z2_ECC_UN_CORRECT_ADDR]);
	        }
	        if (ubError & Z3_UN_CORRECT_INT) {
	                UartString("\nZ3 : ");
	                UartLongHex(RAML[RAML_DDR3_Z3_ECC_UN_CORRECT_ADDR]);
	        }
	        }*/

	ftlRecordFailLog(DDRErrLogType, 0x1, DONTCARE, DONTCARE, DONTCARE, DONTCARE);
	if ((RAML[RAML_DDR3_ECC_CTRL] & (Z2_ECC_ERROR_INT | Z3_ECC_ERROR_INT)) && ((RAML[RAML_DDR3_ECC_CTRL] & (Z2_CRC_ERROR_INT | Z3_CRC_ERROR_INT)) == 0)) {
		RAML[RAML_DDR3_ECC_CTRL] &= ~(Z2_ECC_ERROR_INT_EN | Z3_ECC_ERROR_INT_EN);// 1 bit error, 只記一次fail log且不處理
		return;
	}

	if (!((guwErrorID & BIT15) && BURNER)) {
		M_AssertError(RAML[RAML_DDR3_ECC_CTRL] & (Z1_UN_CORRECT_INT | Z2_UN_CORRECT_INT | Z2_CRC_ERROR_INT | Z3_UN_CORRECT_INT | Z3_CRC_ERROR_INT));
		M_AssertResetError(RAML[RAML_DDR3_ECC_CTRL] & (Z1_UN_CORRECT_INT | Z2_UN_CORRECT_INT | Z3_UN_CORRECT_INT | Z2_CRC_ERROR_INT | Z3_CRC_ERROR_INT), VT_ASSERT_DDR_UNC_0x4F);
	}
	for (ubi = 0; ubi < 8; ubi++) {
		if (RAML[RAML_DDR3_ECC_CTRL] & (BIT0 << ubi)) {
			//有哪種int就清哪種int
			RAML[RAML_DDR3_ECC_CTRL] |= (BIT0 << (ubi + 16));
			RAML[RAML_DDR3_ECC_CTRL] &= (~(BIT0 << (ubi + 16)));
		}
	}
#endif
}
void FLH_ISR()
{

}
void STACK_MONITOR_ISR()
{
	//UBYTE ubi;
	M_AssertResetError(1, VT_ASSERT_STACK_OVERFLOW_0x50);
	/*都critical...了
		for (ubi = 0; ubi < 4; ubi++) {
			if (SYS1L[SYS1L_INT_STS1] & (BIT0 << (ubi + 24))) {
				//有哪種int就清哪種int
				SYS1L[SYS1L_INT_STS1] = (BIT0 << (ubi + 24));
			}
		}
	*/
}

void PARITY_ERROR_ISR()
{
	M_CheckBurnerModeToDisableFunction();

	VT->gulPECount++;
	VT->gulPESYSINTSTS3 = SYS1L[SYS1L_INT_STS3];
	U32 ulPEWillBlink = (DMAC_PE_INT | CPU_PE_INT | DDR_PE_INT | FLH_PE_INT_TRAM | FLH_PE_INT_IRAM | FLH_PE_INT_BPTRAM);
	/*
		U32 ulPEWillCount=(RRAM_PE_INT | SATA_PE_INT | DBUF0_PE_INT | DBUF1_PE_INT | DBUF2_PE_INT | DBUF3_PE_INT | FLH_PE_INT_NMBUF|
			FLH_PE_INT_PBUF0 | FLH_PE_INT_PBUF1 | FLH_PE_INT_PBUF2 | FLH_PE_INT_PBUF3 | FLH_PE_INT_BRAM_1 | FLH_PE_INT_BRAM_2 |
			FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2 | FLH_PE_INT_ENCSRAM_1 | FLH_PE_INT_ENCSRAM_2 | FLH_PE_INT_ZIP );
	*/

	ftlRecordFailLog(ParityErrLogType, 0x1, DONTCARE, DONTCARE, DONTCARE, DONTCARE);

	if (SYS1L[SYS1L_INT_STS3] & RRAM_PE_INT) {
		VT->gulRRAMPECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & SATA_PE_INT) {
		VT->gulSATAPECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & DBUF0_PE_INT) {
		VT->gulDBUF0PECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & DBUF1_PE_INT) {
		VT->gulDBUF1PECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & DBUF2_PE_INT) {
		VT->gulDBUF2PECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & DBUF3_PE_INT) {
		VT->gulDBUF3PECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & FLH_PE_INT_NMBUF) {
		VT->gulNMBUFPECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & FLH_PE_INT_PBUF0) {
		VT->gulPBUF0PECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & FLH_PE_INT_PBUF1) {
		VT->gulPBUF1PECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & FLH_PE_INT_PBUF2) {
		VT->gulPBUF2PECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & FLH_PE_INT_PBUF3) {
		VT->gulPBUF3PECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_IBF_1 | FLH_PE_INT_IBF_2)) {
		VT->gulIBFPECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_BRAM_1 | FLH_PE_INT_BRAM_2)) {
		VT->gulBRAMPECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & (FLH_PE_INT_ENCSRAM_1 | FLH_PE_INT_ENCSRAM_2)) {
		VT->gulENCSRAMPECount++;
	}
	if (SYS1L[SYS1L_INT_STS3] & FLH_PE_INT_ZIP) {
		VT->gulZIPPECount++;
	}

	if (SYS1L[SYS1L_INT_STS3] & ulPEWillBlink)  {
		M_AssertResetError(0xEE, VT_ASSERT_PARITY_ERROR_0x51);
	}
	else {
		M_AssertError(0xEE);
	}

	SYS1L[SYS1L_INT_STS3] = 0xffffffff; //W1C
}

void Enable_ParityErr_Interrupt()
{

	SYS1L[SYS1L_INT_STS3] = 0xffffffff; //W1C

	SYS1L[SYS1L_INT_EN2] |= (BIT24 | BIT25 | BIT26 | BIT27); //Security module parity error interrupt enable
	SYS1L[SYS1L_INT_EN3] |= 0x7Cff0000; //Flash parity error interrupt enable, but close BRAM's PE
	SYS1L[SYS1L_INT_EN3] |= 0x00003C00; //DBUF parity error interrupt enable
	SYS1L[SYS1L_INT_EN3] |= 0x00000100; //DDR parity error interrupt enable
	SYS1L[SYS1L_INT_EN3] |= 0x00000080; //CPU parity error interrupt enable
	SYS1L[SYS1L_INT_EN3] |= 0x00000040; //SATA parity error interrupt enable
	SYS1L[SYS1L_INT_EN3] |= 0x00000020; //DMAC parity error interrupt enable
	SYS1L[SYS1L_INT_EN3] |= 0x00000001; //RRAM parity error interrupt enable

	_xtos_set_interrupt_handler(20, PARITY_ERROR_ISR);
	_xtos_ints_on( 1L << (20) );
	_xtos_set_intlevel(0);
}

void NMI_HANDLER() //VDT_ERROR_ISR(){
{
	VT->gubInto_NMI_ISR_Count++;
	while (1) {
		UartString("NMI_ISR\r\n");
	}
}

void Enable_VDT_Interrupt()
{
	gubGatingVDTEnable = 1;

	SYS0L[SYS0L_VDT_CTRL] &= 0xffff00ff;
	SYS0L[SYS0L_PG_CTRL] |= 0x0000ffff;

	// Enable edge_detect
	SYS1L[SYS1L_EDGE_DET_EN] |= 0x0C000000;

	SYS1L[SYS1L_INT_EN4] |= 0x03ff0000; //RRAM parity error interrupt enable
	//_xtos_set_interrupt_handler(26, VDT_ERROR_ISR);

	/*
		///Debug using GPIO 3
		*((volatile unsigned char*)(0x0400315c)) = 3;
		/// Debug section end
	*/

	/// This is for NMI Interrupt. Now use Masked Int only(BIT21), which is opened in SYS_Interrupt_Init(), ISR is GPIO_DEVSLP_ISR
	//	_xtos_ints_on( 1L << (26) );
	//	_xtos_set_intlevel(0);
}

void Enable_Stack_Monitor_Interrupt()
{
	UBYTE ubi;
	for (ubi = 0; ubi < 4; ubi++) {
		if (SYS1L[SYS1L_INT_STS1] & (BIT0 << (ubi + 24))) {
			//有哪種int就清哪種int
			SYS1L[SYS1L_INT_STS1] = (BIT0 << (ubi + 24));
		}
	}

	DCCM0_BANK0_BASE_LONG[0x220 >> 2] = TxThreadStackArea0;
	DCCM0_BANK0_BASE_LONG[0x224 >> 2] = TxThreadStackArea1;
	DCCM0_BANK0_BASE_LONG[0x228 >> 2] = TxThreadStackArea2;
	DCCM0_BANK0_BASE_LONG[0x230 >> 2] |= (BIT0 | BIT1 | BIT2);
	SYS1L[SYS1L_INT_EN1] |= (STACK_MONITOR_0 | STACK_MONITOR_1 | STACK_MONITOR_2);
	DCCM0_BANK0_BASE_LONG[0x230 >> 2] &= CLR_BIT3;
	SYS1L[SYS1L_INT_EN1] &= ~STACK_MONITOR_3;
	_xtos_set_interrupt_handler(19, STACK_MONITOR_ISR);
	_xtos_ints_on( 1L << (19) );
	_xtos_set_intlevel(0);
}
void Enable_SATA_Interrupt()
{
	SYS1L[SYS1L_INT_EN0] |= SATA_INT;	// enable sata int
	_xtos_set_interrupt_handler(5, SATA_ISR);
	_xtos_ints_on( 1L << (5) );
	_xtos_set_intlevel(0);

}
void Enable_DDR_Interrupt()
{
#if(!BURNER)
	UBYTE ubi = 0;
	if (SYSB[0x44] == SIMREG_DEVSLP_LOAD_CODE) {
		///Clean all interrupt
		SYS1L[SYS1L_INT_STS0] |= DDR_INT;
		for (ubi = 0; ubi < 8; ubi++) {
			if (RAML[RAML_DDR3_ECC_CTRL] & (BIT0 << ubi)) {
				//有哪種int就清哪種int
				RAML[RAML_DDR3_ECC_CTRL] |= (BIT0 << (ubi + 16));
				RAML[RAML_DDR3_ECC_CTRL] &= (~(BIT0 << (ubi + 16)));
			}
		}
	}

#endif
	SYS1L[SYS1L_INT_EN0] |= DDR_INT;	// enable DDR int
	RAML[RAML_DDR3_ECC_CTRL] |= (Z1_UN_CORRECT_INT_EN | Z2_CRC_ERROR_INT_EN | Z2_ECC_ERROR_INT_EN | Z2_UN_CORRECT_INT_EN | Z3_CRC_ERROR_INT_EN | Z3_ECC_ERROR_INT_EN | Z3_UN_CORRECT_INT_EN);
	_xtos_set_interrupt_handler(22, DDR_ISR);
	_xtos_ints_on( 1L << (22) );
	_xtos_set_intlevel(0);
}
void ResetCmdIn()
{
	UBYTE ubHardReset = 0;

	if (gubSoftReset) {
		if (ENABLE_KT_DEBUG_UART)
			UartString("\r\n  SR!");

		Init_Reset();
		gubSoftReset = 0;
		HW[HW_ERR_CTRL] &= (~H_BLK_TRG);     //  off Discard trigger bit function, maybe in softreset no need.
		//HB_Data_Transfer_Trigger_and_Control_B0=SATA_Application_Layer_Unit_Reset;// Reset Application layer Unit
		//M_EnableHWD2H_NCQ();
		HB[HB_STATUS] = 0x50;
		Sata_InterfaceReg_Init();
		HB[HB_REG_REQ] = H_REG_REQ;
		while (HB[HB_REG_REQ]&H_REG_REQ);
		if ( ((AtaCfg.ubSecMode & SET_SEC_LOCK) == 0) && (gubPowerMode != PWR_SLEEP) && (gubIsWriteProtect == FALSE) && ((AtaCfg.ubSanitizeState == SD0_SANITIZE_IDLE) || (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN)) && (gubNCQSupport)) {
			M_EnableHWD2H_NCQ();
		}
		HW[HW_ERR_CTRL] &= ~H_REG_XHOLD;
		if (ENABLE_COMMANDQUEUE == 1) {
			ftlAddToDebugCommanddQueue(0xAAAA, 0xAAAA, 0xAAAA, 0);
		}
		gubForceExit = 0;
		gubNCQINTRError = 0;
		//mSE_SV(4, (U32)gulErrorPhyEven, sizeof(gulErrorPhyEven), 0, 0);
		M_SetFlagSettingSHF();
		if ((gubENABLE_HW_DIPM && gubDIPMSupport) && (gubDevIPMON)) {
			HW[HW_PM_CTRL]  |= H_HW_AUTO_DIPM_EN;
		}
	}

	if (gubHardReset) {
		if (ENABLE_DEBUG_UART)
			UartString("\nHR!");

		Init_Reset();
		VT->gulCOMResetCnt++;
		ubHardReset = 1;
		gubHardReset = 0;
		gubHostCmdIn = 0;
		gubNCQINTRError = 0;
		HW[HW_ERR_CTRL] &= (~H_BLK_TRG);     //  off Discard trigger bit function
		HB[HB_STATUS] = 0X50;
		Sata_InterfaceReg_Init();
		if (ENABLE_DEBUG_COMRESET_NEEDJUMP == 0) {
			while (!(HW[HW_SATA_INFO]&H_LNK_PHYRDY));
			HB[HB_REG_REQ] = H_REG_REQ;
			while (HB[HB_REG_REQ]&H_REG_REQ);
			gubCMDFinish = 0;//comrest回D2H後不做BG
		}
		if ( ((AtaCfg.ubSecMode & SET_SEC_LOCK) == 0) && (gubPowerMode != PWR_SLEEP) && (gubIsWriteProtect == FALSE) && ((AtaCfg.ubSanitizeState == SD0_SANITIZE_IDLE) || (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN)) && (gubNCQSupport)) {
			M_EnableHWD2H_NCQ();
		}
		HW[HW_ERR_CTRL] &= ~H_REG_XHOLD;
		if (ENABLE_COMMANDQUEUE == 1) {
			ftlAddToDebugCommanddQueue(0xBBBB, 0xBBBB, 0xBBBB, 0);
		}
		gubForceExit = 0;
		//HB_Data_Transfer_Trigger_and_Control_B0=SATA_Application_Layer_Unit_Reset;// Reset Application layer Unit
		//M_EnableHWD2H_NCQ();
		//mSE_SV(4, (U32)gulErrorPhyEven, sizeof(gulErrorPhyEven), 0, 0);
		M_SetFlagSettingSHF();
	}
	gubErrorCode = 0;

	gubLastDoneTag = 0x3F;
	gulSATA_MSG_TYPE3[0].All = 0;
	HB[HB_ERROR] = NO_ERROR;

	if (ENABLE_DEBUG_COMRESET_NEEDJUMP & ubHardReset) {
		gubFlushMode = BYTE_STANDBY_FLUSH;
		FlushCache();
		SYSL[SYS0L_SIM_CTRL0] |= SIMREG_COMRESET_JUMP;
		SYSL[SYS0L_SIM_CTRL0] |= SIMREG_CMD_D2H; //use as a tag for command D2H, but comreset D2H
		Vender_Isp_Jump(0);
	}
}

void Uart_InitBaudRate(unsigned int ulPClk, unsigned int ulBaudRate)
{
	//	baud rate register values are computed as follows:
	//	baud_reg=(clock freq/(baud rate*4))-1. Ex: If clk=12Mhz and
	//	you want 19.2k baud, (12e6/(19.2e3*4))-1=0x009b. So
	//	baudh=0x00 and baudl=0x9b.

	unsigned int ulDivisor;
	ulDivisor = (ulPClk / (ulBaudRate * 4)); //ulDivisor = (ulPClk/(ulBaudRate*4))-1;

	UARTL[UARTL_UART_CTRL] = (UARTL[UARTL_UART_CTRL] & 0xFF000000) | (ulDivisor & 0x00FFFFFF);

	UARTL[UARTL_OOB_UR_INT_EN] |= (RX_NOT_EMPTY_INT_EN | RX_OVFLW_INT_EN | RX_FRAME_ERR_INT_EN);
	UARTL[UARTL_S_FIFO_CTLST] |= TX_SERIAL_FIFO_EN;
}

void Uart_Tx_Data(unsigned char ubData)
{

	while ( !(UARTL[UARTL_S_FIFO_CTLST] & S_FIFO_EMPTY) );
	UARTB[UARTB_SERIAL_DATA] = ubData;
	while ( !(UARTL[UARTL_S_FIFO_CTLST] & S_FIFO_EMPTY) );
}
UBYTE  HexTable[] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};
void Uart_Tx_DataHex(UBYTE ubData)
{
	Uart_Tx_Data(HexTable[ubData / 16]);
	Uart_Tx_Data(HexTable[ubData % 16]);
}
void UartWordHex(UWORD uwData)
{
	Uart_Tx_DataHex((UBYTE)(uwData >> (8)));
	Uart_Tx_DataHex((UBYTE)(uwData >> (0)));
}
void UartLongHex(U32 ulData)
{
	Uart_Tx_DataHex((UBYTE)(ulData >> (8 * 3)));
	Uart_Tx_DataHex((UBYTE)(ulData >> (8 * 2)));
	Uart_Tx_DataHex((UBYTE)(ulData >> (8)));
	Uart_Tx_DataHex((UBYTE)(ulData >> (0)));
}
void UartLLongHex(ULLONG uoData)
{
	Uart_Tx_DataHex((UBYTE)(uoData >> (8 * 7)));
	Uart_Tx_DataHex((UBYTE)(uoData >> (8 * 6)));
	Uart_Tx_DataHex((UBYTE)(uoData >> (8 * 5)));
	Uart_Tx_DataHex((UBYTE)(uoData >> (8 * 4)));
	Uart_Tx_DataHex((UBYTE)(uoData >> (8 * 3)));
	Uart_Tx_DataHex((UBYTE)(uoData >> (8 * 2)));
	Uart_Tx_DataHex((UBYTE)(uoData >> (8)));
	Uart_Tx_DataHex((UBYTE)(uoData >> (0)));
}
void UartString(char *ubptr)
{
	// transmit 1 byte data until end of string
	while (*ubptr != 0x00) {
		Uart_Tx_Data(*ubptr);
		if (*ubptr == 0x0A) {
			Uart_Tx_Data(0x0D);  // ASCII: return
		}
		ubptr++;
	}
}
void InitMTSetTemplate(UBYTE ubLength, UBYTE ubClear, U32 ulArg0x00, U32 ulArg0x04, U32 ulArg0x1C, U32 ulZipMap, U32 ulArg0x24, U32 ulArg0x28)
{
	//UBYTE ubDepth= 0;
	UBYTE ubQueue;

	gMTQ = (MTQ *)(IRAM_BASE + MT_ADR_BASE);
	if (ubClear) {
		//Init an "all-0" template to be the copy source
		gMTQ->Depth[0].Para0x00.ulAll = 0x00000000;
		gMTQ->Depth[0].Para0x04.ulAll = 0x00000000;
		gMTQ->Depth[0].Para0x08.ulAll = 0x00000000;
		gMTQ->Depth[0].ulL4KBADR = 0x00000000;
		gMTQ->Depth[0].Para0x10.ulAll = 0x00000000;
		gMTQ->Depth[0].ulSeedInit = 0x00000000;
		gMTQ->Depth[0].ulZipMap = 0x00000000;
		gMTQ->Depth[0].Para0x1C.ulAll = 0x00000000;
		gMTQ->Depth[0].Para0x20.ulAll = 0x00000000;
		gMTQ->Depth[0].Para0x24.ulAll = 0x00000000;
		gMTQ->Depth[0].Para0x28.ulAll = 0x00000000;
		gMTQ->Depth[0].Para0x2C.ulAll = 0x00000000;
		gMTQ->Depth[0].uliFSA1 = 0x00000000;
		gMTQ->Depth[0].uliFSA2 = 0x00000000;
		gMTQ->Depth[0].uliFSA3 = 0x00000000;
		gMTQ->Depth[0].uliFSA0_2 = 0x00000000;
	}
	else {
		//Init a template to be the copy source
		gMTQ->Depth[0].Para0x00.ulAll = ulArg0x00;
		gMTQ->Depth[0].Para0x04.ulAll = ulArg0x04;
		gMTQ->Depth[0].Para0x08.ulAll = 0x00000040;
		gMTQ->Depth[0].ulL4KBADR = 0x00000000;
		gMTQ->Depth[0].Para0x10.ulAll = 0x00000000;
		gMTQ->Depth[0].ulSeedInit = 0x00000000;
		gMTQ->Depth[0].ulZipMap = ulZipMap;
		gMTQ->Depth[0].Para0x1C.ulAll = ulArg0x1C;
		gMTQ->Depth[0].Para0x20.ulAll = 0x00000000;
		gMTQ->Depth[0].Para0x24.ulAll = ulArg0x24;
		gMTQ->Depth[0].Para0x28.ulAll = ulArg0x28;
		gMTQ->Depth[0].Para0x2C.ulAll = 0x00000000;
		gMTQ->Depth[0].uliFSA1 = 0x00000000;
		gMTQ->Depth[0].uliFSA2 = 0x00000000;
		gMTQ->Depth[0].uliFSA3 = 0x00000000;
		gMTQ->Depth[0].uliFSA0_2 = 0x00000000;
		gulPara0x00.ulAll = ulArg0x00;
		gulPara0x04.ulAll = ulArg0x04;
		gulPara0x10.ulAll = 0x00000000;
		gulPara0x1C.ulAll = ulArg0x1C;
		gulPara0x20.ulAll = 0x00000000;
		gulPara0x24.ulAll = ulArg0x24;
	}

	//Copy the template to other events
	FCONW[FCONW_IRC_SRC_ADR] = MT_QUEUE_ADR_0;
	FCONW[FCONW_IRC_DST_ADR] = MT_QUEUE_ADR_0 + 64;
	FCONL[FCONL_IRC_CTL] = IRAM_COPY_LEN((ubLength - 1));
	FCONL[FCONL_IRC_CTL] |= IRAM_COPY_TRIG;
	while (FCONL[FCONL_IRC_CTL] & IRAM_COPY_TRIG) {}

	for (ubQueue = 1; ubQueue < MAX_CE_PER_CH; ubQueue++) {
		FCONW[FCONW_IRC_DST_ADR] = gulMTQAdr[ubQueue];
		FCONL[FCONL_IRC_CTL] = IRAM_COPY_LEN(ubLength);
		FCONL[FCONL_IRC_CTL] |= IRAM_COPY_TRIG;
		while (FCONL[FCONL_IRC_CTL] & IRAM_COPY_TRIG) {}
	}
}

void FLH_LLR_Table_Init(void)
{
	U32* ulLLRPtr;
#if TLC
	if (TLC) {
#if MicronFlashOnly
		ulLLRPtr = (U32*)TSB_A19_LLR1_BASE;
		//LLR table 1 of lower page
		ulLLRPtr[0] = (15 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[1] = (8 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[2] = (0 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[3] = (1 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[4] = (17 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[5] = (24 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[6] = (0 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[7] = (31 << 24) | (0 << 16) | (0 << 8) | (0);
		//LLR table 1 of upper page
		ulLLRPtr[8] = (15 << 24) | (0 << 16) | (0 << 8) | (15);
		ulLLRPtr[9] = (8 << 24) | (0 << 16) | (0 << 8) | (8);
		ulLLRPtr[10] = (0 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[11] = (1 << 24) | (0 << 16) | (0 << 8) | (1);
		ulLLRPtr[12] = (17 << 24) | (0 << 16) | (0 << 8) | (17);
		ulLLRPtr[13] = (24 << 24) | (0 << 16) | (0 << 8) | (24);
		ulLLRPtr[14] = (0 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[15] = (31 << 24) | (0 << 16) | (0 << 8) | (31);
		//LLR table 1 of extra page
		ulLLRPtr[16] = (15 << 24) | (15 << 16) | (15 << 8) | (15);
		ulLLRPtr[17] = (8 << 24) | (8 << 16) | (8 << 8) | (8);
		ulLLRPtr[18] = (0 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[19] = (1 << 24) | (1 << 16) | (1 << 8) | (1);
		ulLLRPtr[20] = (17 << 24) | (17 << 16) | (17 << 8) | (17);
		ulLLRPtr[21] = (24 << 24) | (24 << 16) | (24 << 8) | (24);
		ulLLRPtr[22] = (0 << 24) | (0 << 16) | (0 << 8) | (0);
		ulLLRPtr[23] = (31 << 24) | (31 << 16) | (31 << 8) | (31);
	}
	else {
#else
		if (guoFlashUseType.B.btToshiba) {   //		(btSandisk) || (btToshibaBiCs2) || (btToshibaBiCs3) || (btToshibaBiCs4) || (TSB 15nm TLC)
			memcpy((void *)(TSB_TLC_1Z_LLR1_BASE), SanDisk_sb_tlc_default_llr_table_1, sizeof(SanDisk_sb_tlc_default_llr_table_1));

			memcpy((void *)(TSB_TLC_1Z_LLR2_BASE), SanDisk_sb_tlc_default_llr_table_2, sizeof(SanDisk_sb_tlc_default_llr_table_2));
		}
		else {
			memcpy((void *)(TSB_TLC_1Z_LLR1_BASE), toshiba_sb_tlc_default_llr_table_1, sizeof(toshiba_sb_tlc_default_llr_table_1));

			memcpy((void *)(TSB_TLC_1Z_LLR2_BASE), toshiba_sb_tlc_default_llr_table_2, sizeof(toshiba_sb_tlc_default_llr_table_2));
		}
	}
	else {
#endif
#else
	{
#endif
		//MLC LLR Table Parameter is for 1Znm
		ulLLRPtr = (U32*)TSB_A19_LLR1_BASE;
#if MicronFlashOnly
		//L06B   LDPC-PS3111 SB Flow for Micron L06B_180423.pptx
		//LLR table 1 of lower page
		ulLLRPtr[0] = (0x07 << 24) | (0x09 << 16) | (0x0A << 8) | (0x0F);
		ulLLRPtr[1] = (0x04 << 24) | (0x05 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[2] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[3] = (0x01 << 24) | (0x02 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[4] = (0x19 << 24) | (0x18 << 16) | (0x16 << 8) | (0x11);
		ulLLRPtr[5] = (0x1C << 24) | (0x1D << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[6] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[7] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		//LLR table 1 of middle page
		ulLLRPtr[8] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[9] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[10] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[11] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[12] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[13] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[14] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[15] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		//LLR table 1 of upper page
		ulLLRPtr[16] = (0x06 << 24) | (0x08 << 16) | (0x0A << 8) | (0x0E);
		ulLLRPtr[17] = (0x02 << 24) | (0x04 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[18] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[19] = (0x1F << 24) | (0x02 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[20] = (0x19 << 24) | (0x19 << 16) | (0x11 << 8) | (0x14);
		ulLLRPtr[21] = (0x1C << 24) | (0x1D << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[22] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[23] = (0x1D << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
#else
		//LLR table 1 of lower page
		ulLLRPtr[0] = (0x08 << 24) | (0x08 << 16) | (0x0F << 8) | (0x0F);
		ulLLRPtr[1] = (0x04 << 24) | (0x04 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[2] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[3] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[4] = (0x17 << 24) | (0x17 << 16) | (0x11 << 8) | (0x11);
		ulLLRPtr[5] = (0x1B << 24) | (0x1B << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[6] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[7] = (0x1E << 24) | (0x1E << 16) | (0x1F << 8) | (0x1F);
		//LLR table 1 of middle page
		ulLLRPtr[8] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[9] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[10] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[11] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[12] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[13] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[14] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[15] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		//LLR table 1 of upper page
		ulLLRPtr[16] = (0x05 << 24) | (0x08 << 16) | (0x0A << 8) | (0x0E);
		ulLLRPtr[17] = (0x01 << 24) | (0x04 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[18] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[19] = (0x1E << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[20] = (0x18 << 24) | (0x18 << 16) | (0x11 << 8) | (0x14);
		ulLLRPtr[21] = (0x1B << 24) | (0x1C << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[22] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[23] = (0x1C << 24) | (0x1E << 16) | (0x1F << 8) | (0x1F);
#endif
		ulLLRPtr = (U32*)TSB_A19_LLR2_BASE;
		//LLR table 2 of lower page
		ulLLRPtr[0] = (0x08 << 24) | (0x08 << 16) | (0x0F << 8) | (0x0F);
		ulLLRPtr[1] = (0x04 << 24) | (0x04 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[2] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[3] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[4] = (0x17 << 24) | (0x17 << 16) | (0x11 << 8) | (0x11);
		ulLLRPtr[5] = (0x1B << 24) | (0x1B << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[6] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[7] = (0x1E << 24) | (0x1E << 16) | (0x1F << 8) | (0x1F);

		//LLR table 2 of middle page
		ulLLRPtr[8] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[9] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[10] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[11] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[12] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[13] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[14] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[15] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);

		//LLR table 2 of upper page
		ulLLRPtr[16] = (0x05 << 24) | (0x08 << 16) | (0x0A << 8) | (0x0E);
		ulLLRPtr[17] = (0x01 << 24) | (0x04 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[18] = (0x01 << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[19] = (0x1E << 24) | (0x01 << 16) | (0x01 << 8) | (0x01);
		ulLLRPtr[20] = (0x18 << 24) | (0x18 << 16) | (0x11 << 8) | (0x14);
		ulLLRPtr[21] = (0x1B << 24) | (0x1C << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[22] = (0x1F << 24) | (0x1F << 16) | (0x1F << 8) | (0x1F);
		ulLLRPtr[23] = (0x1C << 24) | (0x1E << 16) | (0x1F << 8) | (0x1F);
	}

}
#if(!RDT)
void Tune_PHYPMA_GEN3(void) {
	SYS0B[0x3A6] = 0xFF;
	if (guwPMASetting) {
		SYS0B[0x3A7] = (UBYTE)(guwPMASetting & 0xFF);
		SYS0B[0x3A8] = (UBYTE)((guwPMASetting >> 8) & 0xFF);
	}
	else {
		SYS0B[0x3A7] = 0x33;
		SYS0B[0x3A8] = 0xA4;
	}
	SYS0B[0x3AA] = 0x07;
	SYS0B[0x3AD] = 0x4D;
	SYS0B[0x3AF] = 0x20;
	SYS0B[0x3B0] = 0x20;
	SYS0B[0x3B2] = 0x7F;
	SYS0B[0x3B3] = 0x04;
	SYS0B[0x3B4] = 0xA3;
	SYS0B[0x3B7] = 0x39;

	if (gubSSC_Enable == 1) {
		SYS0B[0x3A1] |= BIT5;
	}
	else if (gubSSC_Enable == 2) {
		SYS0B[0x3A1] &= ~BIT5;
	}

}

void Tune_PHYPMA_GEN2(void) {
	SYS0B[0x386] = 0x66;
	SYS0B[0x387] = 0x02;
	SYS0B[0x388] = 0xA2;
	SYS0B[0x38A] = 0x07;
	SYS0B[0x38D] = 0x59; // analog部門 更改KP (抗雜訊能力)
	SYS0B[0x394] = 0xA3;
	SYS0B[0x397] = 0x39;

	if (gubSSC_Enable == 1) {
		SYS0B[0x381] |= BIT5;
	}
	else if (gubSSC_Enable == 2) {
		SYS0B[0x381] &= ~BIT5;
	}

}

void Tune_PHYPMA_GEN1(void) {
	SYS0B[0x367] = 0x02;
	SYS0B[0x368] = 0xA2;
	SYS0B[0x36D] = 0x59;  // analog部門 更改KP (抗雜訊能力)
	SYS0B[0x374] = 0xA3;
	SYS0B[0x377] = 0x39;

	if (gubSSC_Enable == 1) {
		SYS0B[0x361] |= BIT5;
	}
	else if (gubSSC_Enable == 2) {
		SYS0B[0x361] &= ~BIT5;
	}

}
#endif
void InitSATARegister(void) {
	/*
	重設sata phy --
	Vender Reset不作(因為沒必要,且會斷link), 但是萬一做了其實也沒差, 就多打一次Cominit, 導致host會多下一次comreset而已
	*/
	wSYS0(0x51E) |= 0x200;          //Set WU_EN = SATA

	HB[HB_FLAG_CTRL] |= (H_DDT_EN);
	HW[HW_SYS_CTRL] |= (H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_WR_MSGQ_EN | H_LZSS);
	HW[HW_SYS_CTRL] |= (H_WR_MSGQ_EN );
#if(!BURNER)
	if ((SYSB[0x44] != SIMREG_DEVSLP_LOAD_CODE) || (HW[HW_SATA_INFO]&BIT13)) {
#endif
		HL[HL_INT_FLAG] = 0xFFFFFFFF; //w1c --clear all interrupt status;
#if(!BURNER)
	}
#endif
	HL[HL_ERR_INT_EN] = (DATAFIS_CRC | FORCE_ESCAPE | H_RD_UNC_INT_EN | H_E3D_INT_EN | H_LNK_NOCOMERR_INT_EN | H_LNK_DB_ERR_INT_EN | H_LNK_DD_ERR_INT_EN);
	HW[HW_MZ_CTRL]  |= H_E3D_ERR;
	Enable_SATA_Interrupt();
	if (ASIC == 0) {
		HW[HW_SYS_CTRL] |= H_REG_GENMAX_GEN1;	 // force Gen1
	}

	while (!(HW[HW_SATA_INFO] & H_LNK_ESTABLISH)) { // every 1 millisecond to see if link establish or not
#if(!RDT)
		M_SwitchTask();
#endif
#if(RDT)

		guwsatalink_retry++;
		if (guwsatalink_retry == 0x9999) {
			guwsatalink_retry = 0xFFFF;
			break;
		}

#endif
	}
#if(!BURNER)
	if ((SYSB[0x44] != SIMREG_DEVSLP_LOAD_CODE) || (HW[HW_SATA_INFO]&BIT13)) {
#endif
		HL[HL_INT_FLAG] = 0xFFFF;
#if(!BURNER)
	}
#endif
#if(!BURNER)
	if (((SYSB[0x44] == SIMREG_DEVSLP_LOAD_CODE) && (HW[HW_SATA_INFO]&BIT13)) || gubSATAPhyBeforeDevslp) {
		gubHardReset = 1;
		Init_Reset();
		gubHardReset = 0;
	}
#endif
	//HB[HB_REG_REQ] = H_REG_REQ;
	//SW_Interrupt_Event |=(BIT5);    // clear DCCM ISR EVENT
	//SW_DCCM_Bus_Monitor_Status|=BIT0;     // clear DCCM ISR EVENT
	//enable_interrupts();
	//DCCM_INTR_ENABLE();
	HL[HL_INT_EN] |= ( H_LNK_COMRI_INT_EN  | H_SRST_CLR_INT_EN  |  H_RCV_CMD_INT_EN | H_LNK_ESTB_CHG_INT_EN | H_ERR_EVT_INT_EN | H_PHYRDY_CHG_INT_EN); //??? why comwake_ien need not enable??
	if (gubEnable_CTRL_INT_For_CBitZeroCase) {
		HL[HL_INT_EN] |= H_RCV_CTRL_INT_EN; // Enable CTRL_INT
	}
	if (!BURNER) {
		if (((SYSL[SYS0L_SIM_CTRL0] & SIMREG_CMD_D2H) == 0) || (SYSL[SYS0L_SIM_CTRL0] & SIMREG_COMRESET_JUMP)) {
			if ((SYSB[0x44] != SIMREG_DEVSLP_LOAD_CODE) || (HW[HW_SATA_INFO]&BIT13 || gubSATAPhyBeforeDevslp)) {
				if (ENABLE_DEBUG_UART_ATA_CMD) {
					UartString("\r\nsend comreset D2H\r\n");
				}
				Sata_InterfaceReg_Init();
				HW[HW_ERR_CTRL] &= ~(H_REG_XHOLD | H_BLK_TRG);
				HW[HW_SYS_CTRL] &= ~H_BLK_FIS_EN;
				APU_Reset();
				HB[HB_REG_REQ] = H_REG_REQ;
				while (HB[HB_REG_REQ]&H_REG_REQ) {
#if(!RDT)
					M_SwitchTask();
#endif
#if(RDT)
					if (guwsatalink_retry == 0xFFFF)
						break;
#endif
				}
				gubCMDFinish = 0; //comrest回D2H後不做BG(1st D2H)
			}
		}
		else {
			if (ENABLE_DEBUG_UART_ATA_CMD) {
				UartString("\r\nsend cmd D2H\r\n");
			}
			HB[HB_STATUS] = 0x50;
			HB[HB_SECTOR_CNT] = 0x02; //ATA device has applied the new microcode
			HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
			while (HB[HB_REG_REQ]&H_REG_REQ);
		}
	}
	else {
		if (ENABLE_DEBUG_UART_ATA_CMD) {
			UartString("\r\nsend ISP Jump cmd D2H\r\n");
		}

		HB[HB_STATUS] = 0x50;

		HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
		while (HB[HB_REG_REQ]&H_REG_REQ);
	}

	if (gubHIPMSupport && !BURNER) {	//because burner don't need HIPM
		HW[HW_PM_CTRL] |= ( H_IPM_EN | H_AUTO_ACK_HIPM );
		HL[HL_INT_EN] |= (H_LNK_REQPRCV_INT_EN | H_LNK_REQSRCV_INT_EN); //|H_BUF_DONE_INT_EN);
		gubHIPMEnable = 0;    /// initial
	}
	else {
		HW[HW_PM_CTRL] |= H_IPM_EN ;
		HL[HL_INT_EN] |= (H_LNK_REQPRCV_INT_EN | H_LNK_REQSRCV_INT_EN); //|H_BUF_DONE_INT_EN);
	}

	//	if (gubDIPMSupport) {
	//HW[HW_DIPM_TIMER_THR] |= ((5 << 8) | (20)); ///20us for partical, 5ms for slumber
	//  gubDIPM_THRESHOLD = DIPM_THRESHOLD;
	//  gubDIPM_P2S_THRESHOLD = DIPM_P2S_THRESHOLD;
	//  guwDIPM_AUTOP2S_THRESHOLD = DIPM_AUTOP2S_THRESHOLD;
	//	}

	//HL[HL_INT_EN] |= ( H_LNK_COMRI_INT_EN  | H_SRST_CLR_INT_EN  |  H_RCV_CMD_INT_EN | H_LNK_ESTB_CHG_INT_EN | H_ERR_EVT_INT_EN | H_PHYRDY_CHG_INT_EN); //??? why comwake_ien need not enable??
	HB[HB_ERROR] = NO_ERROR;

	HL[HL_HAT_MISC] |= H_PHYRDYN_CLR_HAT_N;

	SYSL[SYS0L_SIM_CTRL0] &= (~(SIMREG_CMD_D2H | SIMREG_COMRESET_JUMP));
}
void InitFlashRegister(UBYTE ubInitIRAM) {

	UBYTE ubONFIMode = 0;
#if(ENABLE_TLC_PROGRAM_CODEBLOCK ||RDT || (!BURNER))
	if (ubInitIRAM) {
		InitFPUEntry();
	}
#endif

	FALLL[FALLL_POL_SEQ_0] = FPU_PTR_C70_DAT_MK40;
	FALLL[FALLL_POL_SEQ_1] = FPU_PTR_C71_DAT_MK40;
	FALLL[FALLL_POL_SEQ_2] = FPU_PTR_C70_DAT_MK20;
#if FORCE_AUTOPOL_TIMEOUT
	FALLL[FALLL_POL_SEQ_3] = FPU_PTR_C70_DAT_MK00;
#else
	FALLL[FALLL_POL_SEQ_3] = FPU_PTR_C70_DAT_MK40;
#endif
	FALLL[FALLL_POL_SEQ_4] = FPU_PTR_C71_DAT_MK40;
	FALLL[FALLL_POL_SEQ_5] = FPU_PTR_C70_DAT_MK20;
	FALLL[FALLL_POL_SEQ_6] = FPU_PTR_C70_DAT_MK40;
	FALLL[FALLL_POL_SEQ_7] = FPU_PTR_C71_DAT_MK40;

	FALLL[FALLL_CHNL_SET] |= MT_AUTO_CE_CLR;
	FCONL[FCONL_MT_ADR_BASE] = (MT_GLOBAL_TRIG_EN | MT_TABLE_SIZE(64) | INT_INDEX_MODE | MT_BASE_ADDR(MT_ADR_BASE));

	//======== Flash  Setting =======
	FCONL[FCONL_ADR_GEN] |= FRAME_PTR_UPDATE_EN | COL_ADR_AUTO_GEN;
	if (ENABLE_iFSA) {
		FCONL[FCONL_ADR_GEN] &= 0xFFFFFFC0;
		FCONL[FCONL_ADR_GEN] |= FRAME_PTR_CATCH_COL_ADR(8);	// 因為use iFSA, HW會自動將col low byte addr補0
	}
	FCONL[FCONL_ALU_BAS] = ALU_BASE_0; //ALU start offset at IRAM
	FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
	FCONB[FCONB_SPECIAL] =  MCU_WAIT_COUNTER_VALUE(0x3F);
	//set ECC threshold, LDPC protect 200 bit, BCH protect 16 bit
	if (ENABLE_DEBUG_FIX_MT_EOT_BUG) {
		FCONL[FCONL_SPECIAL] |= SET_ECC_THRESHOLD_NUM(200);
	}
	else {
		FCONL[FCONL_SPECIAL] |= SET_ECC_THRESHOLD_NUM(1023); // no EOT interrupt to fix Ping-Pong FIFO frame map mismatch bug
	}

	FCONL[FCONL_LGC_TIME_CFG_1] = 0xFFFFFFFF;
	FCONL[FCONL_TGL_TIME_CFG_1] = 0xFFFFFFFF;
	FCONL[FCONL_ONF_TIME_CFG_1] = 0xFFFFFFFF;
	FCONL[FCONL_ONF_TIME_CFG_1] &= CLR_BIT19;

	FCONL[FCONL_SYS_INTQ_BASE] = Quue_Addr_FLH; //FLH Qbase address;
	if (ENABLE_DEBUG_OVER_ECC_STOP) {
		if (ENABLE_DEBUG_PROGRAM_E3D_FAIL_FIP_CHECK) {
			FALLL[FALLL_INT_CFG] = 0x3F7FF;		// close PFA interrupt
		}
		else {
			FALLL[FALLL_INT_CFG] = 0x1F7FF;		// close PFA interrupt, close Program E3D fail interrupt
		}
	}
	else {
		if (!ENABLE_DEBUG_FIX_MT_EOT_BUG) {
			if (ENABLE_DEBUG_PROGRAM_E3D_FAIL_FIP_CHECK) {
				FALLL[FALLL_INT_CFG] = 0x203F7DF;		// close PFA interrupt, EOT not stop MT for MLC & TLC, disable EOT Interrupt to fix Ping-Pong FIFO frame map mismatch bug
			}
			else {
				FALLL[FALLL_INT_CFG] = 0x201F7DF;		// close PFA interrupt, EOT not stop MT for MLC & TLC, disable EOT Interrupt to fix Ping-Pong FIFO frame map mismatch bug, close Program E3D fail interrupt
			}
		}
		else {
			if (ENABLE_DEBUG_PROGRAM_E3D_FAIL_FIP_CHECK) {
				FALLL[FALLL_INT_CFG] = 0x203F7FF;		// close PFA interrupt, EOT not stop MT for MLC & TLC
			}
			else {
				FALLL[FALLL_INT_CFG] = 0x201F7FF;		// close PFA interrupt, EOT not stop MT for MLC & TLC, close Program E3D fail interrupt
			}
		}
	}
	FALLL[FALLL_MAP_CFG] |= 0x3FFF00;	// show map all turn on
	FALLL[FALLL_CNT_ONE] |= COUNT_MODE_EN;
	FCONL[FCONL_ERASE_PAGE_TH] = 255;
	FALLL[FALLL_CHNL_SET] |= (BYPASS_INV | FLA_EDO_MODE);   // 1b: bypass inversion

#if(BURNER)					//close write E3D
	FALLL[FCTLL_MAP_CFG] &= (~BIT18);
	FALLL[FCTLL_INT_CFG] &= (~BIT17);
#endif

	FCONL[FCONL_SGN_MSK] |= SIGNOFF_MASK_EN;
	//FCONL[FCONL_FLH_FUNC] |= DATA_TO_RAM_WHEN_UECC;
	//FCONL[FCONL_FLH_FUNC] |= DATA_TO_RAM_WHEN_CRC_FAIL;
	if (ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE) {
#if MicronFlashOnly
		FCONL[FCONL_FLH_FUNC] |= ( IBUF_RING_CH0_EN | IBUF_RING_CH1_EN | FLH_USE_IBUF_RING | RDM_REF_ROW_ADR_8_TO_0 | MT_TABLE_CE_NUMBER_MODE);
#else
		FCONL[FCONL_FLH_FUNC] |= ( IBUF_RING_CH0_EN | IBUF_RING_CH1_EN | FLH_USE_IBUF_RING | RDM_REF_ROW_ADR_7_TO_0 | MT_TABLE_CE_NUMBER_MODE);
#endif
	}
	else {
#if MicronFlashOnly
		FCONL[FCONL_FLH_FUNC] |= ( IBUF_RING_CH0_EN | IBUF_RING_CH1_EN | FLH_USE_IBUF_RING | RDM_REF_ROW_ADR_8_TO_0 | MT_TABLE_CE_NUMBER_MODE);
#else
		FCONL[FCONL_FLH_FUNC] |= ( IBUF_RING_CH0_EN | IBUF_RING_CH1_EN | FLH_USE_IBUF_RING | RDM_REF_ROW_ADR_7_TO_0 | MT_TABLE_CE_NUMBER_MODE);
#endif
	}

	FALLL[FALLL_ZIP_CFG] |= RING_MODE;

	FALLL[FCTLL_DMA_CFG] |= ULTRA_DMA_EN;
	SYS1W[SYS1W_PAD_FCEB_OE] = 0xFFFF;	//enable flash CE output
	SYS1B[0x11B]  |= SET_BIT0;	//disable flash write protect

#if Hynix || YMTC_JGS
	//	SYS1L[SYS1L_PAD_FLH_CTRL]  |= CR_FLH_RSEL;
	FALLL[FCTLL_CHNL_SET] |= DUMMY_DATA_LEN(96);//24 Byte * 4 Frame
#endif

#if MicronFlashOnly
	// Micron L06B page size : 16,384 + 2208 bytes
	FALLL[FCTLL_CHNL_SET] |= DUMMY_DATA_LEN(32);//24 Byte * 4 Frame
	gubLDPC_Mode_Test = LDPC_4K_MODE0;
	FCONL[FCONL_LDPC_CFG] &= CLR_LDPC_MODE;
#else
#if TLC
	if (TLC) {
		gubLDPC_Mode_Test = LDPC_4K_MODE1;
	}
	else {
		gubLDPC_Mode_Test = LDPC_4K_MODE4;
	}
#else
	gubLDPC_Mode_Test = LDPC_4K_MODE4;
#endif
#endif

	FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
	UWORD uwLDPCParitySize = 272; //[cppcheck][Uninitialized variable: uwLDPCParitySize]


	if (gubLDPC_Mode_Test == LDPC_4K_MODE0) {
		uwLDPCParitySize = 496;
	}
	else if (gubLDPC_Mode_Test == LDPC_4K_MODE1) {
		uwLDPCParitySize = 440;
	}
	else if (gubLDPC_Mode_Test == LDPC_4K_MODE2) {
		uwLDPCParitySize = 420;
	}
	else if (gubLDPC_Mode_Test == LDPC_4K_MODE3) {
		uwLDPCParitySize = 360;
	}
	else if (gubLDPC_Mode_Test == LDPC_4K_MODE4) {
		uwLDPCParitySize = 272;
	}
	else {
		M_AssertError(1);
	}

	guwDMAFrameSize = 4096 + 16 + 32 + uwLDPCParitySize;

	FCONL[FCONL_TMR_CTL] = TIMER_INTR_EN | TIMER_CNT_LEN(0xFFFFFF) ;	// fix DQS/RE counter mismatch

#if MicronFlashOnly || YMTC_JGS
	if (gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
	if (gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
	{
		FCONL[FCONL_TMR_CTL] &= ~(TIMER_CPU_INTR_EN | TIMER_INTR_EN);
	}

	flaSetPadIOType(2, SINGLE_END_IO_TYPE);
	if (gubNormalFWMode) {
		if ((gubFlashMode_CE[0] & BIT0) && ((gubFlashONFI_CE[0] & BIT0) == 0)) {	//Ting 20170223
			gubToggleMode = 1;
		}
		else if ((gubFlashMode_CE[0] & BIT0) && (gubFlashONFI_CE[0] & BIT0)) {
			ubONFIMode = 1;
		}

		if (gubIOMode_CE[0] & BIT0) {
			flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
		}

		if (ENABLE_DEBUG_UART_FLH_INITIAL) {
			UartString("\r\ngubFlashMode_CE[0]: ");
			Uart_Tx_DataHex(gubFlashMode_CE[0]);
			UartString("\r\ngubIOMode_CE[0]: ");
			Uart_Tx_DataHex(gubIOMode_CE[0]);
		}
	}
	else {
		gubToggleMode = (guoFlashUseType.B.btToggle ? 1 : 0) ;
	}

	if (gubToggleMode) {
		if (ubInitIRAM) {
			// Set MTP 0x00, 0x04, 0x1C, ZipMap, 0x24, 0x28
			InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000,  0x0F000000, gulZipMap, 0x0C000000, 0x001A0001);
		}
		if (ASIC == 0) {
			flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL, (FDLLL[FDLLL_DLL_CTL] & FLH_STROBE_MODE));
			flaDLLCDCIssueWorkaround(FDLLL_DLL_CTL, (FDLLL[FDLLL_DLL_CTL] | FW_CTL_SDLL_MODE));
			UBYTE ubChannel;
			for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
				flaDLLCDCIssueWorkaround((FDLLL_SEL_VAL_0 + ubChannel), (FDLLL[(FDLLL_SEL_VAL_0 + ubChannel)] & 0xFFFFFE00));
				flaDLLCDCIssueWorkaround((FDLLL_SEL_VAL_0 + ubChannel), (FDLLL[(FDLLL_SEL_VAL_0 + ubChannel)] | SDLL_VALUE_READ_CH0(0x060)));	//[7:5] = 3
			}
		}
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
	}
	else if (ubONFIMode) {	//Ting 20170223
		if (ubInitIRAM) {
			// Set MTP 0x00, 0x04, 0x1C, ZipMap, 0x24, 0x28
			InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000,  0x0F000000, gulZipMap, 0x0C000000, 0x001B0001);
		}	//暫時複製 gating clock 的 delay
		FALLL[FCTLL_FLH_SET] |= FLA_ONFI_MODE;
	}
	else {		//Legacy Mode
		if (ubInitIRAM) {
			// Set MTP 0x00, 0x04, 0x1C, ZipMap, 0x24, 0x28
			InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000,  0x0F000000, gulZipMap, 0x0C000000, 0x00180001);
		}
	}

	UBYTE ubChannel;
	for (ubChannel = 0; ubChannel < gubPlanesPerBurst; ubChannel++) {
		_UCV REG_t *pFlaReg  = (_UCV REG_t *)gulFlashIP_RegBase[ubChannel];
		pFlaReg[FCTLL_FSA_ADR].L = gulFSABase[ubChannel]; //FSA start offset at IRAM
	}

	if (ubInitIRAM) {
		RSL[RSL_DBASE0] = (U32)(&guoRSFrameBits[0]);
		RSL[RSL_SBASE0] = (U32)(&gulRSFrameSpareBits[0]);
		RSL[RSL_MODE_0] &= RS_CLR_PAGE;
		RSL[RSL_MODE_0] |= RS_16K_PAGE;
		RSB[RSB_PAGE_CNT] = 1; // always 1
		RSL[RSL_IP_CTL] |= RS_INTR_EN;
		if (!ENABLE_DEBUG_RS_PRE_SL) {
			RSL[RSL_PBUF_CTL] &= (~RS_PRE_LOAD_EN); // close pre load function
			RSL[RSL_PBUF_CTL] |= RS_RRE_SAVE_LOAD_EN;
		}
		else {
			RSL[RSL_PBUF_CTL] &= (~(RS_PRE_LOAD_EN | RS_PRE_SAVE_EN));
		}

		FLH_LLR_Table_Init();
	}

	/*
	雖然Power On時Init Flow在InitFlashRegister()後會Call LoadSystemBlock()裡面會Call到flaSetParameter()裡面會設定PAGE_CFG，
	但是如果是Slumber之類的醒來後只會Call InitFlashRegister()，會導致PAGE_CFG變成預設的 PHYSICAL_16KPAGE，這邊如果是8K or 4K Flash會有問題，
	所以再多加這段重新設定PAGE_CFG, by Brian
	*/
	if (guoFlashUseType.B.btFlash16k) {
		FCONL[FCONL_PAGE_CFG] = PHYSICAL_16KPAGE;
	}
	else if (guoFlashUseType.B.btFlash8k) {
		FCONL[FCONL_PAGE_CFG] = PHYSICAL_8KPAGE;
	}
	else if (guoFlashUseType.B.btFlash4k) {
		FCONL[FCONL_PAGE_CFG] = PHYSICAL_4KPAGE;
	}

}
void InitCommonRegister(void) {
	if (ASIC) {
		SYS0B[SYS0B_CR_DIV_0] = 0xF;
	}
	// set DDR cache
	UWORD uwTemp;
	if (ENABLE_DCACHE && (!BURNER)) {
		xthal_dcache_all_unlock();
		xthal_dcache_all_invalidate();
		xthal_icache_all_unlock();
		xthal_icache_all_invalidate();
		XT_DSYNC();
		uwTemp = xthal_set_region_attribute((void*)DDR_BASE, 512 * 1024 * 1024, XCHAL_CA_WRITEBACK, XTHAL_CAFLAG_EXACT | XTHAL_CAFLAG_NO_PARTIAL) ;
		//xthal_dcache_region_invalidate((void *)DDR_BASE, 512 * 1024 * 1024);
		//xthal_set_cache_prefetch(XTHAL_PREFETCH_ENABLE | XTHAL_DCACHE_PREFETCH_LOW);
	}
#if(!BURNER)
	gubIgnoreFlag = 0;
	gubAutoD2HBackup = 0;
#endif
	SYS1W[SYS1W_PAD_FCEB_OE] = 0xFFFF;	//enable flash CE output

	SYS0L[SYS0L_FLL_CNY_VAL] &= (~CR_OP_FLL); //\\power2\dep\研發處\研發二部\s3\_PS3111_Verification\_PS3111_HWBUG\HBR_PS3111_20150126_AsyncReset_Timing_Bug

	///////////////////////////////////////////////////////////////////////////////////////////
	//                      Buffer Flag Reg
	///////////////////////////////////////////////////////////////////////////////////////////
	AXIL[AXIL_SET_START_FLAG] = 0;
	AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM;
	AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;     //clear all buffer flag
	M_SetFlagSettingSHF();

	//======== SATA  Setting =======
	HL[HL_BUF_BASE] = BUFFER2_BASE / 4096;
	HL[HL_BUF_BASE2] = 0x20000000 / 4096;
	HL[HL_BUF_R_SHF] = BUFFER2_READ_START;
	//HW[HW_AUTO_FIS_CTRL] |= H_WR_BLK_EN;
	//HW[HW_SYS_CTRL] |= (H_EC_CNTQ_EN | H_DONE_TAGQ_EN | H_LZSS);
	if (HW[HW_SATA_INFO]&BIT13) {
		Sata_InterfaceReg_Init();
	}
	HL[HL_DONE_TAG_BASE] = Quue_Addr_SATA_DoneTag;
	HL[HL_ZINFO_BASE] = Quue_Addr_SATA_Zinfo;
	HL[HL_HAT_MISC] |= H_TQ_BY_ORDER;
	//HL[HL_TQ_CTRL] &= (~H_TQ_FORCE_CHK_SEC); //check all buffer flag valid
	//HW[HW_AUTO_FIS_CTRL] &= (~H_WR_BLK_EN);
	//HB[HB_FLAG_CTRL] &= (~H_DDT_EN); //DDT off
	//HW[HW_SYS_CTRL]&=~(H_EC_CNTQ_EN|H_DONE_TAGQ_EN);//read done tag off

#if (BURNER)
	U32 ulIRAMAddress;
	for (ulIRAMAddress = ((FPU_ADR + FPU_SIZE) / 4); ulIRAMAddress < (FPU_ADR2 / 4); ulIRAMAddress ++) {
		IRAM4B[ulIRAMAddress] = 0;
	}
	for (ulIRAMAddress = ((FPU_ADR2 + FPU_SIZE2) / 4); ulIRAMAddress < (IRAM_SIZE / 4); ulIRAMAddress ++) {
		IRAM4B[ulIRAMAddress] = 0;
	}
#else
	FCONL[FCONL_IRC_CTL] |= IRAM_INITIAL_TRIG; // Clear Flash IRAM
	while (FCONL[FCONL_IRC_CTL] & IRAM_INITIAL_TRIG);
#endif


	if (ENABLE_DDR_MODE) {
		// DDR MODE
		HL[HL_BUF_R_EP] = BUFFER2_WRITE_END;
		HL[HL_BUF_W_SHF] = 0;
		HL[HL_BUF_W_EP] = SDRAM_CACHE_4KNUM - 1;
		AXIL[AXIL_FLAG_SET] &= Flag_SW_256K;
		AXIL[AXIL_FIRST_START_ADDRESS] = BUFFER2_BASE;
		AXIL[AXIL_FIRST_END_ADDRESS] = BUFFER2_BASE + (BUFFER2_NUM - 1) * 512;
	}
	else {
		HL[HL_BUF_R_EP] = BUFFER2_READ_END;
		HL[HL_BUF_W_SHF] = BUFFER2_WRITE_START;
		HL[HL_BUF_W_EP] = BUFFER2_WRITE_END;
		AXIL[AXIL_FLAG_SET] &= Flag_SW_256K;
		AXIL[AXIL_FLAG_SET] |= Flag_SW_128K_128K;
		//AXIL[AXIL_FLAG_SET] |= Flag_SW_192K_64K;
		AXIL[AXIL_FIRST_START_ADDRESS] = BUFFER2_BASE;
		AXIL[AXIL_FIRST_END_ADDRESS] = BUFFER2_BASE + (BUFFER2_Read_NUM - 1) * 512;
	}
	AXIL[AXIL_SECOND_START_ADDRESS] = BUFFER2_WRITE_BASE;
	AXIL[AXIL_SECOND_END_ADDRESS] = BUFFER2_WRITE_BASE + (BUFFER2_WRITE_NUM_AXIL - 1) * 512;

	//test
	SYS0B[SYS0B_CR_LZSS_CFG] = LZSS_RESOLUTION;

	//GPIO
	SYS0W[SYS0W_CR_MUX_GPIO] = 0x09;
	//HB[HB_FLAG_CTRL] |= H_RD_TRG_DONE_SEL;

	//Disable PD1/PD2 RAM deep sleep to avoid PBUF parity error
	SYSL[SYS0L_MEMPD_CTRL] &= (~CR_OP_PD1_RAM_DS);
	SYSL[SYS0L_MEMPD_CTRL] &= (~CR_OP_PD2_RAM_DS);

	flaSetFlashPadControl_Default();
}

void LoadSystemBlock() {

	if ((gubPreformat == 0) && (!BURNER)) {
		if (ftlCheckFlashFormat_SystemSet() == 0) {
			//system block found
			HandleSystemDBTSet();

			if ((gubCENumber != gubCENumber_Preformat)) {
				flaLEDBlink(1);
				if (ENABLE_DEBUG_UART_FLH_INITIAL) {
					UartString("\r\ngubCENumber != gubCENumber_Preformat");
				}
			}
			gubFlashInitialized = 1;
		}
		else {
			//system block not found
			//flaLEDBlink(1);
			UartString("LSF ");
			Vender_Isp_Jump(0xFF);
		}
	}
	else {
		flaSetParameter(0);
		flaSetParamPageInfo(1);
		if (!BURNER) {
			flaSet_ODT_DRIVE();
		}
		gubFlashInitialized = 0;
	}
#if(!BURNER)&&(ENABLE_DPS)
	UWORD uwcount = 0;
	/// Add to Decide whether to rewrite HardBitRetryTable or Not
	if ((gub_ENABLE_DPS) && (gubDPSTableValid) && (TLC)) {
		if (gubDPSTable[2047] != 0) {
			for (uwcount = 0; uwcount < (gubDPSTable[2047] * 7); uwcount++) {
				gubHardBitRetryParameterTable[uwcount + gubHardBitRetryD2D3GroupBase] = gubDPSTable[uwcount + 1101];
			}
			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = gubDPSTable[2047]; // uwGroupNumber[1] Non-Fast
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
		}
		else {
			if (gubDensitysize == FLH_Size_128Gb) {
				for (uwcount = 0; uwcount < 315; uwcount++) { /// 45 commands * 7 addrs
					gubHardBitRetryParameterTable[uwcount + gubHardBitRetryD2D3GroupBase] = gubDPSTable[uwcount + 1101];
				}
				gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 45; // uwGroupNumber[1] Non-Fast
				gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
			}
			else if (gubDensitysize == FLH_Size_64Gb) {
				for (uwcount = 0; uwcount < 301; uwcount++) { /// 43 commands * 7 addrs
					gubHardBitRetryParameterTable[uwcount + gubHardBitRetryD2D3GroupBase] = gubDPSTable[uwcount + 1101];
				}
				gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 43; // uwGroupNumber[1] Non-Fast
				gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
			}
		}
	}
#endif
}

void InitVariable() {
#if(!RDT)
	UBYTE ubi, ubj;

	for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
		gubFSAIndex[ubi] = 0;
	}

	M_CheckRDTModeToDisableFunction();
	gulUNCLBA = 0;
	gubErrorCode = 0;
#if (ENABLE_CY_DEBUG)
	for (ubi = 0; ubi < TOTAL_CE_PHASE_NUM; ubi++) {
		guwDebug[ubi] = 0;
	}
#endif
	if (ENABLE_CH_DEBUG) {
		for (ubi = 0; ubi < 12; ubi++) {
			guwCHDebug[ubi] = 0;
		}
	}
	gulCountDDRINT = 0;
	if (BURNER) {
		if (gubDram_Size < 4) {
			gubSDRSizeMB = (BIT0 << (gubDram_Size + 2));
		}
		else {
			gubSDRSizeMB = (BIT0 << (gubDram_Size + 3));
		}
	}
	else {
		gubSDRSizeMB =  0;
	}
#if UFS
	guwNotSetMap = 0;
	guwDefaultToggleCEMap = 0;
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
#if EnableVenderCmdHistory
	for (ubi = 0; ubi < MaxVenderCmdLog; ubi++) {
		gubVendeCmdHistory[ubi] = 0xFF;
	}
	gubVendeCmdHistoryPtr = 0;
	//gulVendeCmdPagePtr=0xFFFF;
#endif
	gubEraseAllDone_Flag = 0;
	gubRUTMixPlaneEnable = ENABLE_RUT_MIX_PLANE;
	gubRDTDone = 0;
	//gubUFSRule = 0;
	gubExpandCE_initial = 0;
	gubPreformatTurbo = 0;
	gubPreformatunfinish = 0;
	gubFlashDrive = FLH_DRIVE_VALUE;
	gubFlashODT = FLH_ODT_VALUE;
	gubSaveVTSerialNumber = 0;
	memset((void *)&gubLastFWVer[0], 0x0, 8);
	gubFirstCMDIn = 1;
	gubIcdeDone = 0;
	gubDPSTableValid = 0;
	gulNeededUnitNum = 0;
	gubFlashODT = 0;
	gubRUT_EC_set  = 0;
	gulDiskSizeInMB = 0;
	gulNativeSize = 0;
	guwOldRUTTargetTemp.All = Default_Unit;
	gulAPKeyPassword = 0;
	gubWUNCDoing = 0;
	gulSmartOfflineUNCCount = 0;
	gubWriteProtectWriteDummy = 0;
	gulSmartNeedScanTimeInterval = 0;
	gubCMDFinish = 0;
	enumWriteWaitCntMode = WaitCnt_Default;

	gBQI.ubLinkNum = 0;
	gBQI.ubWLinkNum = 0;
	gBQI.ubRLinkNum = 0;
	gBQI.BufferQueue[0].ubNext = 1;
	gBQI.BufferQueue[0].ubPrevious = BQ_NUM - 1;
	UBYTE ubBQIndex;
	for (ubBQIndex = 1; ubBQIndex < (BQ_NUM - 1); ubBQIndex++) {
		gBQI.BufferQueue[ubBQIndex].ubNext = ubBQIndex + 1;
		gBQI.BufferQueue[ubBQIndex].ubPrevious = ubBQIndex - 1;
	}
	gBQI.BufferQueue[BQ_NUM - 1].ubNext = 0;
	gBQI.BufferQueue[BQ_NUM - 1].ubPrevious = BQ_NUM - 2;
	gBQI.ubLinkLast = BQ_NUM - 1;
	gBQI.ubLinkLastUse = gBQI.ubLinkLast;
	gBQI.ubLinkFTLLast = gBQI.ubLinkLast;
	gBQI.ubLinkFirst = gBQI.BufferQueue[gBQI.ubLinkLastUse].ubNext;
#if ENABLE_PFAIL
	gBQI.uwWrite_buffer_window = gBQI.uwWrite_buffer_remain = gBQI.uwWrite_tq_2_bq = gBQI.uwWrite_buffer_tq = gBQI.ubWrite_TQ_lock = 0;
	gBQI.ubForce_clean_wbq_threshold = 0;
	gBQI.ulHostWriteWaitBufferFlag = 0x0UL;
#endif
#if DEBUG_ULTRAMLC_POWER_FAIL
	trace_arr_index = 0;
#endif

	gRRTMode.All = 0;

	gMTDepth.ubLinkNum = G_MT_QUEUE_DEPTH - 1;
	gMTDepth.ubLinkFirst = 0;
	UWORD uwMTDepthIndex;
	for (uwMTDepthIndex = 0; uwMTDepthIndex < (G_MT_QUEUE_DEPTH - 1); uwMTDepthIndex++) {
		gMTDepth.DepthQueue[uwMTDepthIndex] = uwMTDepthIndex;
	}

	//memset(gubNCQ,0xFF,sizeof(gubNCQ));
	gubReadHBRetry_Enable = 0;
	guwPMASetting = 0;
	gDrivingSetting.ubDrivingEnable = 0;		//for BURNER default
	gODTSetting.ubODTEnable = 0;
	gSDLL_OFFSET_Setting.uwReadSDLLOffsetCH0 = 0;
	gSDLL_OFFSET_Setting.uwReadSDLLOffsetCH1 = 0;
	gSDLL_OFFSET_Setting.uwProgramSDLLOffsetCH0 = 0;
	gSDLL_OFFSET_Setting.uwProgramSDLLOffsetCH1 = 0;
	gubSSC_Enable = 0;
	gubHostCmdIn = 0;
	gubNCQDoing = 0;
	gubSoftReset = 0;
	gubHardReset = 0;
	gubLinkLost = 0;
	gubLBAOverlap = 0;
	gubNeedWaitTQDone = 0;
	//gubDMACmd=0;
	gubNeedSendCmdComplete = 1;
	gubHIPMEnable = 0;
	gubDevIPMON = 0;
	gubDEVSLPEnable = 0;
	gubDIPMP2SEnable = 0;
	//gubNCQ_ValidCount = 0;
	//gubNCQTagNum = 0;
	//gubNCQOutPTR = 0;
	//gubNCQInPTR = 0;
	gubNCQINTRError = 0;
	gubD2HMask = 0;
	gubJTagWakeUp = 1;
	gubQE_FromSlumber = 0;

	gubAP_enable = 0;
	guwErrorDumpBlock[0] = 0xFFFF;
	guwErrorDumpBlock[1] = 0xFFFF;
	gubErrorDumpSet = 0xFF;
	gubEraseFailBitMapInit = 0;
#if(!BURNER)
	//========= Error Dump =======

	gubErrorDumpFreeSetGet = 0xFF;
	guwErrorDumpLastPlaneWrite = 0;
	gubAlreadyCopyFLHRegister = 0;
#endif
	gubPowerCycling = 0;
	gubPowerCycleNeedGenSearchLink = 0;
	if (ENABLE_FORCE_L06B_TO_B0KB || ENABLE_SANDISK_1ZNM_SPF) {
		if (!(gubIsWriteProtect & BIT_FLASH_INIT_FAIL_WRITEPROTECT)) {
			gubIsWriteProtect = FALSE;
		}
	}
	else {
		gubIsWriteProtect = FALSE;
	}
	gSwapHostTable.All = 0;
	gubFlashInitialized = 0;
	gubParaInitialized = 0;
	gubSSD_Init_State.All = 0;
	gulParamPageInfo.All = 0;
	//For Pseudo CE
	gubExpandCE_Ratio = 1;
	gubExpandCE = 0;

#if MicronFlashOnly
	gubMTEVENT_CNT_LIMIT = 13;     // 因為像flaFlashToSram_0030會一次推兩個MT, or 前面cmd sequence的last DMA + 1P rand cache會做00 30/00 31兩個MT加切mode MT, 一共4個MT, 執行前是13個以上就要跳走, 12個以下才能做
#else

#if Hynix

#if Hynix_256G_LIMIT
	gubMTEVENT_CNT_LIMIT = 8;
#else
	gubMTEVENT_CNT_LIMIT = 14; 	// 因為像flaFlashToSram_0030會一次推兩個MT, or 前面cmd sequence的last DMA + 1P rand cache會做00 30/00 31兩個MT, 一共三個MT, 執行前是14個以上就要跳走, 13個以下才能做
#endif

#else   //NOT Micron, NOT Hynix
	gubMTEVENT_CNT_LIMIT = 14;     // 因為像flaFlashToSram_0030會一次推兩個MT, or 前面cmd sequence的last DMA + 1P rand cache會做00 30/00 31兩個MT, 一共三個MT, 執行前是14個以上就要跳走, 13個以下才能做
#endif

#endif
	if (SYSB[0x44] == SIMREG_DEVSLP_LOAD_CODE) {
		if ((gDEVSLP_Table->ulFirstMark == 0x31113111) && (gDEVSLP_Table->ulLastMark == 0xDDBADBAD)) {
			for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
				guwSystemBlock[ubi] = gDEVSLP_Table->uwSystemBlock[ubi];
			}
			gubSystemSet = gDEVSLP_Table->ubSystemSet;

			gubSATAPhyBeforeDevslp = 	gDEVSLP_Table->ubSATAPhyStatus;
			gubHostCmdIn = gDEVSLP_Table->ubHostCmdIn;
			if (gubHostCmdIn)
				gubCmdCode = gDEVSLP_Table->ubSATACmdCode;
		}
		else {
			SYSB[0x44] = SIMREG_NORMAL_LOAD_CODE;
			for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
				guwSystemBlock[ubi] = 0xFFFF;
			}
			gubSystemSet = 0xFF;
			gubSATAPhyBeforeDevslp = 0;
		}
	}
	else {
		gubSATAPhyBeforeDevslp = 0;
		for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
			guwSystemBlock[ubi] = 0xFFFF;
		}
		gubSystemSet = 0xFF;
	}

	gulSystemVersion = 0x112233;
	gulDBTVersion = 0;
	gubDLMCStep = 0;
	gubLastDLMCMode = 0;
	gubMicroCodeTmpSet = 0xFF;
	gubHeaderTransferSectorCnt = 0;
	gulBurnerTransferSectorCnt = 0;
	gulMicroCodeTransferSectorCnt = 0;
	gubDoPreformatAfterDLMC = 0;
	gulUpdateSystemInfoMap = 0;
	gubUpdateSysInfoTransferSectorCnt = 0;
	memset(&AtaCfg, 0, sizeof(AtaCfg));

	gubFlashMode = FLH_MODE_DEFAULT;
	gubFlashClock = FLH_CLK_DEFAULT;
	gubCurrentFlashClock = FLH_CLK_40MHz;
	gubClockDivValue = 0x0F;
	gubTransformSystemUnitToSystemSet = FALSE;
	gubTranslateMixPlaneMark = FALSE;
	gubSystemArea_RemappingRule = Rule_Set;
	guwSystemAreaNum[0] = 0;
	guwSystemAreaNum[1] = 0;
	guwDBTBlock[0] = 0xFFFF;
	guwDBTBlock[1] = 0xFFFF;
	guwSysCodeBlock[0][0] = 0xFFFF;
	guwSysCodeBlock[0][1] = 0xFFFF;
	guwSysCodeBlock[1][0] = 0xFFFF;
	guwSysCodeBlock[1][1] = 0xFFFF;
	gubSysCodeSet[0] = 0xFF;
	gubSysCodeSet[1] = 0xFF;
	gubDBTSet = 0xFF;

	gubExpectErasePage = 1;
	gubExpectBadkBlock = 1;
	if (ENABLE_RESET_CNT) {
		gubResetCountDuringCmd = 0;
		gubResetFlag = 0;
	}
	for (ubi = 0; ubi < SYSTEM_SETS; ubi++) {
		for (ubj = 0; ubj < MAX_CHANNEL; ubj++) {
			guwSystemBlockRemappingTable[ubi][ubj] = 0xFFFF;
		}
		gubSystemSetFreeTable[ubi] = 0xFF;
	}

	if (ENABLE_COMMANDQUEUE) {
#if (!BURNER)
		gubDebugCommandQueueIndex = 0;
		ftlAddToDebugCommanddQueue(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		ftlAddToDebugCommanddQueue(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		ftlAddToDebugCommanddQueue(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		ftlAddToDebugCommanddQueue(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		gubDebugCommandQueueIndex = 0;
#endif
	}
#if B0KB
	mDMAC_SV(1, (U32)&gubINTTable[0], G_MT_QUEUE_DEPTH * 2, 0, 0, 0, DMA_EN_SETCQ, 1);
#else
	mDMAC_SV(1, (U32)&gubINTTable[0], MAX_CE * MT_QUEUE_DEPTH * 2, 0, 0, 0, DMA_EN_SETCQ, 1);
#endif

	if (!(BURNER)) {
		mDMAC_SV(1, (U32)&WR_NCQ_CMD_INFO[0], 32 * 16, 0, 0, 0, DMA_EN_SETCQ, 1);
	}
#if PH_SQL_FUNC
	gulBusyTimer = 0;
	gubIsPHSQLClockSet = FALSE;
	gubFLHIPNoBusyPin = FALSE;
	gubFlashClockMax = 0xFF;
#endif
	gubDontUseSortingSystemSet = FALSE;
#if TLC_BICS2
	flaSetParamPageInfo(1);
#endif
#if TLC
	if (0) {
#else
	if (guoFlashUseType.B.btToshiba19nm) {
#endif
		gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 6; // uwGroupNumber[0] Fast
		gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

		gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

		gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 6; // uwGroupNumber[1] Non-Fast
		gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 4; // uwBytePerGroup[1] Non-Fast

		// RETRY_D1_BLOCK_TYPE
		// 19nm A2, ref: MFOPR-2313_Read Retry Sequence of SLC mode for 19nm MLC NAND_20130115.pdf
		gubHardBitRetryParameterTable[0] = 0x04;
		gubHardBitRetryParameterTable[1] = 0x70;
		gubHardBitRetryParameterTable[2] = 0x10;
		gubHardBitRetryParameterTable[3] = 0x18;
		gubHardBitRetryParameterTable[4] = 0x20;
		gubHardBitRetryParameterTable[5] = 0x6C;

		// RETRY_D2D3_BLOCK_TYPE
		// 19nm, ref: MFOPR-1739 Toshiba NAND Read Retry Sequence for 19nm MLC_20120601.pdf
		gubHardBitRetryParameterTable[6] = 0x00;
		gubHardBitRetryParameterTable[7] = 0x00;
		gubHardBitRetryParameterTable[8] = 0x00;
		gubHardBitRetryParameterTable[9] = 0x00;

		gubHardBitRetryParameterTable[10] = 0x04;
		gubHardBitRetryParameterTable[11] = 0x04;
		gubHardBitRetryParameterTable[12] = 0x04;
		gubHardBitRetryParameterTable[13] = 0x04;

		gubHardBitRetryParameterTable[14] = 0x7C;
		gubHardBitRetryParameterTable[15] = 0x7C;
		gubHardBitRetryParameterTable[16] = 0x7C;
		gubHardBitRetryParameterTable[17] = 0x7C;

		gubHardBitRetryParameterTable[18] = 0x78;
		gubHardBitRetryParameterTable[19] = 0x78;
		gubHardBitRetryParameterTable[20] = 0x78;
		gubHardBitRetryParameterTable[21] = 0x78;

		gubHardBitRetryParameterTable[22] = 0x74;
		gubHardBitRetryParameterTable[23] = 0x74;
		gubHardBitRetryParameterTable[24] = 0x74;
		gubHardBitRetryParameterTable[25] = 0x74;

		gubHardBitRetryParameterTable[26] = 0x08;
		gubHardBitRetryParameterTable[27] = 0x08;
		gubHardBitRetryParameterTable[28] = 0x08;
		gubHardBitRetryParameterTable[29] = 0x08;

	}
#if TLC
	else if (0) {
#else
	else if (guoFlashUseType.B.btToshiba1Ynm || guoFlashUseType.B.btToshiba1Znm) {
#endif
		if (guoFlashUseType.B.btSandisk && guoFlashUseType.B.btToshiba1Znm) {
			// s15-rr-v1.3-appnote 1Znm 002 - 1Znm eX2 Dynamic Read Sequence with SET FEATURES - Rev1.7
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 25; // uwGroupNumber[0] Fast
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 33; // uwGroupNumber[1] Non-Fast
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 2; // uwBytePerGroup[1] Non-Fast

			//SLC
			gubHardBitRetryParameterTable[0] = 0x00;  // Reset
			gubHardBitRetryParameterTable[1] = 0x04;
			gubHardBitRetryParameterTable[2] = 0x7C;
			gubHardBitRetryParameterTable[3] = 0x08;
			gubHardBitRetryParameterTable[4] = 0x78;
			gubHardBitRetryParameterTable[5] = 0x0C;
			gubHardBitRetryParameterTable[6] = 0x74;
			gubHardBitRetryParameterTable[7] = 0x10;
			gubHardBitRetryParameterTable[8] = 0x70;
			gubHardBitRetryParameterTable[9] = 0x14;
			gubHardBitRetryParameterTable[10] = 0x6C;
			gubHardBitRetryParameterTable[11] = 0x18;
			gubHardBitRetryParameterTable[12] = 0x68;
			gubHardBitRetryParameterTable[13] = 0x1C;
			gubHardBitRetryParameterTable[14] = 0x64;
			gubHardBitRetryParameterTable[15] = 0x20;
			gubHardBitRetryParameterTable[16] = 0x60;
			gubHardBitRetryParameterTable[17] = 0x24;
			gubHardBitRetryParameterTable[18] = 0x5C;
			gubHardBitRetryParameterTable[19] = 0x28;
			gubHardBitRetryParameterTable[20] = 0x58;
			gubHardBitRetryParameterTable[21] = 0x2C;
			gubHardBitRetryParameterTable[22] = 0x30;
			gubHardBitRetryParameterTable[23] = 0x34;
			gubHardBitRetryParameterTable[24] = 0x38;

			// MLC
			//Lower Page
			//Case 0: P0 0   P2 0
			gubHardBitRetryParameterTable[25] = 0x00;
			gubHardBitRetryParameterTable[26] = 0x00;
			//Case 1: P0 4   P2 0
			gubHardBitRetryParameterTable[27] = 0x04;
			gubHardBitRetryParameterTable[28] = 0x00;
			//Case 2: P0 7C   P2 0
			gubHardBitRetryParameterTable[29] = 0x7C;
			gubHardBitRetryParameterTable[30] = 0x00;
			//Case 3: P0 4   P2 7C
			gubHardBitRetryParameterTable[31] = 0x04;
			gubHardBitRetryParameterTable[32] = 0x7C;
			//Case 4: P0 8   P2 0
			gubHardBitRetryParameterTable[33] = 0x08;
			gubHardBitRetryParameterTable[34] = 0x00;
			//Case 5: P0 0   P2 7C
			gubHardBitRetryParameterTable[35] = 0x00;
			gubHardBitRetryParameterTable[36] = 0x7C;
			//Case 6: P0 8   P2 7C
			gubHardBitRetryParameterTable[37] = 0x08;
			gubHardBitRetryParameterTable[38] = 0x7C;
			//Case 7: P0 4   P2 8
			gubHardBitRetryParameterTable[39] = 0x04;
			gubHardBitRetryParameterTable[40] = 0x08;
			//Case 8: P0 0   P2 4
			gubHardBitRetryParameterTable[41] = 0x00;
			gubHardBitRetryParameterTable[42] = 0x04;
			//Case 9: P0 7C   P2 0
			gubHardBitRetryParameterTable[43] = 0x7C;
			gubHardBitRetryParameterTable[44] = 0x00;
			//Case 10: P0 8   P2 8
			gubHardBitRetryParameterTable[45] = 0x08;
			gubHardBitRetryParameterTable[46] = 0x08;
			//Case 11: P0 C   P2 7C
			gubHardBitRetryParameterTable[47] = 0x0C;
			gubHardBitRetryParameterTable[48] = 0x7C;
			//Case 12: P0 0   P2 78
			gubHardBitRetryParameterTable[49] = 0x00;
			gubHardBitRetryParameterTable[50] = 0x78;
			//Case 13: P0 C   P2 0
			gubHardBitRetryParameterTable[51] = 0x0C;
			gubHardBitRetryParameterTable[52] = 0x00;
			//Case 14: P0 78   P2 0
			gubHardBitRetryParameterTable[53] = 0x78;
			gubHardBitRetryParameterTable[54] = 0x00;
			//Case 15: P0 7C   P2 0
			gubHardBitRetryParameterTable[55] = 0x7C;
			gubHardBitRetryParameterTable[56] = 0x00;
			//Case 16: P0 78   P2 74
			gubHardBitRetryParameterTable[57] = 0x78;
			gubHardBitRetryParameterTable[58] = 0x74;
			//Case 17: P0 7C   P2 78
			gubHardBitRetryParameterTable[59] = 0x7C;
			gubHardBitRetryParameterTable[60] = 0x78;
			//Case 18: P0 4   P2 4
			gubHardBitRetryParameterTable[61] = 0x04;
			gubHardBitRetryParameterTable[62] = 0x04;
			//Case 19: P0 8   P2 8
			gubHardBitRetryParameterTable[63] = 0x08;
			gubHardBitRetryParameterTable[64] = 0x08;
			//Case 20: P0 8   P2 8
			gubHardBitRetryParameterTable[65] = 0x08;
			gubHardBitRetryParameterTable[66] = 0x08;
			//Case 21: P0 78   P2 78
			gubHardBitRetryParameterTable[67] = 0x78;
			gubHardBitRetryParameterTable[68] = 0x78;
			//Case 22: P0 7C   P2 7C
			gubHardBitRetryParameterTable[69] = 0x7C;
			gubHardBitRetryParameterTable[70] = 0x7C;
			//Case 23: P0 8   P2 4
			gubHardBitRetryParameterTable[71] = 0x08;
			gubHardBitRetryParameterTable[72] = 0x04;
			//Case 24: P0 0   P2 8
			gubHardBitRetryParameterTable[73] = 0x00;
			gubHardBitRetryParameterTable[74] = 0x08;
			//Case 25: P0 4   P2 78
			gubHardBitRetryParameterTable[75] = 0x04;
			gubHardBitRetryParameterTable[76] = 0x78;
			//Case 26: P0 8   P2 78
			gubHardBitRetryParameterTable[77] = 0x08;
			gubHardBitRetryParameterTable[78] = 0x78;
			//Case 27: P0 7C   P2 74
			gubHardBitRetryParameterTable[79] = 0x7C;
			gubHardBitRetryParameterTable[80] = 0x74;
			//Case 28: P0 7C   P2 78
			gubHardBitRetryParameterTable[81] = 0x7C;
			gubHardBitRetryParameterTable[82] = 0x78;
			//Case 29: P0 0   P2 74
			gubHardBitRetryParameterTable[83] = 0x00;
			gubHardBitRetryParameterTable[84] = 0x74;
			//Case 30: P0 0   P2 0
			gubHardBitRetryParameterTable[85] = 0x00;
			gubHardBitRetryParameterTable[86] = 0x00;
			//Case 31: P0 78   P2 7C
			gubHardBitRetryParameterTable[87] = 0x78;
			gubHardBitRetryParameterTable[88] = 0x7C;
			//Case 32: P0 78   P2 70
			gubHardBitRetryParameterTable[89] = 0x78;
			gubHardBitRetryParameterTable[90] = 0x70;
			//Lower Page End
			//Upper Page
			//Case 0: P1 0   P3 0
			gubHardBitRetryParameterTable[91] = 0x00;
			gubHardBitRetryParameterTable[92] = 0x00;
			//Case 1: P1 4   P3 0
			gubHardBitRetryParameterTable[93] = 0x04;
			gubHardBitRetryParameterTable[94] = 0x00;
			//Case 2: P1 7C   P3 0
			gubHardBitRetryParameterTable[95] = 0x7C;
			gubHardBitRetryParameterTable[96] = 0x00;
			//Case 3: P1 4   P3 7C
			gubHardBitRetryParameterTable[97] = 0x04;
			gubHardBitRetryParameterTable[98] = 0x7C;
			//Case 4: P1 8   P3 0
			gubHardBitRetryParameterTable[99] = 0x08;
			gubHardBitRetryParameterTable[100] = 0x00;
			//Case 5: P1 0   P3 7C
			gubHardBitRetryParameterTable[101] = 0x00;
			gubHardBitRetryParameterTable[102] = 0x7C;
			//Case 6: P1 8   P3 7C
			gubHardBitRetryParameterTable[103] = 0x08;
			gubHardBitRetryParameterTable[104] = 0x7C;
			//Case 7: P1 4   P3 4
			gubHardBitRetryParameterTable[105] = 0x04;
			gubHardBitRetryParameterTable[106] = 0x04;
			//Case 8: P1 0   P3 4
			gubHardBitRetryParameterTable[107] = 0x00;
			gubHardBitRetryParameterTable[108] = 0x04;
			//Case 9: P1 7C   P3 7C
			gubHardBitRetryParameterTable[109] = 0x7C;
			gubHardBitRetryParameterTable[110] = 0x7C;
			//Case 10: P1 8   P3 4
			gubHardBitRetryParameterTable[111] = 0x08;
			gubHardBitRetryParameterTable[112] = 0x04;
			//Case 11: P1 4   P3 78
			gubHardBitRetryParameterTable[113] = 0x04;
			gubHardBitRetryParameterTable[114] = 0x78;
			//Case 12: P1 0   P3 78
			gubHardBitRetryParameterTable[115] = 0x00;
			gubHardBitRetryParameterTable[116] = 0x78;
			//Case 13: P1 C   P3 0
			gubHardBitRetryParameterTable[117] = 0x0C;
			gubHardBitRetryParameterTable[118] = 0x00;
			//Case 14: P1 78   P3 0
			gubHardBitRetryParameterTable[119] = 0x78;
			gubHardBitRetryParameterTable[120] = 0x00;
			//Case 15: P1 7C   P3 4
			gubHardBitRetryParameterTable[121] = 0x7C;
			gubHardBitRetryParameterTable[122] = 0x04;
			//Case 16: P1 78   P3 7C
			gubHardBitRetryParameterTable[123] = 0x78;
			gubHardBitRetryParameterTable[124] = 0x7C;
			//Case 17: P1 7C   P3 78
			gubHardBitRetryParameterTable[125] = 0x7C;
			gubHardBitRetryParameterTable[126] = 0x78;
			//Case 18: P1 4   P3 8
			gubHardBitRetryParameterTable[127] = 0x04;
			gubHardBitRetryParameterTable[128] = 0x08;
			//Case 19: P1 C   P3 4
			gubHardBitRetryParameterTable[129] = 0x0C;
			gubHardBitRetryParameterTable[130] = 0x04;
			//Case 20: P1 8   P3 8
			gubHardBitRetryParameterTable[131] = 0x08;
			gubHardBitRetryParameterTable[132] = 0x08;
			//Case 21: P1 78   P3 78
			gubHardBitRetryParameterTable[133] = 0x78;
			gubHardBitRetryParameterTable[134] = 0x78;
			//Case 22: P1 7C   P3 0
			gubHardBitRetryParameterTable[135] = 0x7C;
			gubHardBitRetryParameterTable[136] = 0x00;
			//Case 23: P1 4   P3 0
			gubHardBitRetryParameterTable[137] = 0x04;
			gubHardBitRetryParameterTable[138] = 0x00;
			//Case 24: P1 0   P3 0
			gubHardBitRetryParameterTable[139] = 0x00;
			gubHardBitRetryParameterTable[140] = 0x00;
			//Case 25: P1 8   P3 0
			gubHardBitRetryParameterTable[141] = 0x08;
			gubHardBitRetryParameterTable[142] = 0x00;
			//Case 26: P1 C   P3 7C
			gubHardBitRetryParameterTable[143] = 0x0C;
			gubHardBitRetryParameterTable[144] = 0x7C;
			//Case 27: P1 8   P3 78
			gubHardBitRetryParameterTable[145] = 0x08;
			gubHardBitRetryParameterTable[146] = 0x78;
			//Case 28: P1 7C   P3 74
			gubHardBitRetryParameterTable[147] = 0x7C;
			gubHardBitRetryParameterTable[148] = 0x74;
			//Case 29: P1 0   P3 74
			gubHardBitRetryParameterTable[149] = 0x00;
			gubHardBitRetryParameterTable[150] = 0x74;
			//Case 30: P1 0   P3 8
			gubHardBitRetryParameterTable[151] = 0x00;
			gubHardBitRetryParameterTable[152] = 0x08;
			//Case 31: P1 78   P3 74
			gubHardBitRetryParameterTable[153] = 0x78;
			gubHardBitRetryParameterTable[154] = 0x74;
			//Case 32: P1 4   P3 74
			gubHardBitRetryParameterTable[155] = 0x04;
			gubHardBitRetryParameterTable[156] = 0x74;
		}
		else {
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 10; // uwGroupNumber[0] Fast
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 2; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 10; // uwGroupNumber[1] Non-Fast
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 5; // uwBytePerGroup[1] Non-Fast
			if (guoFlashUseType.B.btToshiba1Ynm) {
				// RETRY_D1_BLOCK_TYPE
				// A19nm A2, ref: tA19-rr-v1.1-M2CMP01-060 A19nm pSLC Read Retry Sequence.pdf
				gubHardBitRetryParameterTable[0] = 0x0C;
				gubHardBitRetryParameterTable[1] = 0x00;
				gubHardBitRetryParameterTable[2] = 0x78;
				gubHardBitRetryParameterTable[3] = 0x00;
				gubHardBitRetryParameterTable[4] = 0x70;
				gubHardBitRetryParameterTable[5] = 0x00;
				gubHardBitRetryParameterTable[6] = 0x18;
				gubHardBitRetryParameterTable[7] = 0x00;
				gubHardBitRetryParameterTable[8] = 0x74;
				gubHardBitRetryParameterTable[9] = 0x00;
				gubHardBitRetryParameterTable[10] = 0x6C;
				gubHardBitRetryParameterTable[11] = 0x00;
				gubHardBitRetryParameterTable[12] = 0x1C;
				gubHardBitRetryParameterTable[13] = 0x00;
				gubHardBitRetryParameterTable[14] = 0x7C;
				gubHardBitRetryParameterTable[15] = 0x00;
				gubHardBitRetryParameterTable[16] = 0x68;
				gubHardBitRetryParameterTable[17] = 0x00;
				gubHardBitRetryParameterTable[18] = 0x14;
				gubHardBitRetryParameterTable[19] = 0x00;

				// RETRY_D2D3_BLOCK_TYPE
				// A19nm, ref: tA19-rr-v1.1-M2CMP01-060 A19nm MLC 128Gb Read Retry Sequence.pdf
				gubHardBitRetryParameterTable[20] = 0x04;
				gubHardBitRetryParameterTable[21] = 0x04;
				gubHardBitRetryParameterTable[22] = 0x7C;
				gubHardBitRetryParameterTable[23] = 0x00;
				gubHardBitRetryParameterTable[24] = 0x00;

				gubHardBitRetryParameterTable[25] = 0x0C;
				gubHardBitRetryParameterTable[26] = 0x06;
				gubHardBitRetryParameterTable[27] = 0x78;
				gubHardBitRetryParameterTable[28] = 0x78;
				gubHardBitRetryParameterTable[29] = 0x00;

				gubHardBitRetryParameterTable[30] = 0x7C;
				gubHardBitRetryParameterTable[31] = 0x00;
				gubHardBitRetryParameterTable[32] = 0x74;
				gubHardBitRetryParameterTable[33] = 0x76;
				gubHardBitRetryParameterTable[34] = 0x00;

				gubHardBitRetryParameterTable[35] = 0x14;
				gubHardBitRetryParameterTable[36] = 0x08;
				gubHardBitRetryParameterTable[37] = 0x00;
				gubHardBitRetryParameterTable[38] = 0x00;
				gubHardBitRetryParameterTable[39] = 0x00;

				gubHardBitRetryParameterTable[40] = 0x1C;
				gubHardBitRetryParameterTable[41] = 0x04;
				gubHardBitRetryParameterTable[42] = 0x7A;
				gubHardBitRetryParameterTable[43] = 0x78;
				gubHardBitRetryParameterTable[44] = 0x00;

				gubHardBitRetryParameterTable[45] = 0x02;
				gubHardBitRetryParameterTable[46] = 0x78;
				gubHardBitRetryParameterTable[47] = 0x76;
				gubHardBitRetryParameterTable[48] = 0x74;
				gubHardBitRetryParameterTable[49] = 0x00;

				gubHardBitRetryParameterTable[50] = 0x08;
				gubHardBitRetryParameterTable[51] = 0x76;
				gubHardBitRetryParameterTable[52] = 0x72;
				gubHardBitRetryParameterTable[53] = 0x72;
				gubHardBitRetryParameterTable[54] = 0x00;

				gubHardBitRetryParameterTable[55] = 0x10;
				gubHardBitRetryParameterTable[56] = 0x7C;
				gubHardBitRetryParameterTable[57] = 0x70;
				gubHardBitRetryParameterTable[58] = 0x76;
				gubHardBitRetryParameterTable[59] = 0x00;

				gubHardBitRetryParameterTable[60] = 0x18;
				gubHardBitRetryParameterTable[61] = 0x06;
				gubHardBitRetryParameterTable[62] = 0x08;
				gubHardBitRetryParameterTable[63] = 0x76;
				gubHardBitRetryParameterTable[64] = 0x00;

				gubHardBitRetryParameterTable[65] = 0x78;
				gubHardBitRetryParameterTable[66] = 0x0C;
				gubHardBitRetryParameterTable[67] = 0x04;
				gubHardBitRetryParameterTable[68] = 0x00;
				gubHardBitRetryParameterTable[69] = 0x00;
			}
			else {
				// RETRY_D1_BLOCK_TYPE
				// 15nm A2, ref: t15-rr-v0.3-M2CMP01-186_15nm MLC (pSLC mode) Read Retry Sequence Rev0.3.pdf
				gubHardBitRetryParameterTable[0] = 0x78;
				gubHardBitRetryParameterTable[1] = 0x00;
				gubHardBitRetryParameterTable[2] = 0x08;
				gubHardBitRetryParameterTable[3] = 0x00;
				gubHardBitRetryParameterTable[4] = 0x10;
				gubHardBitRetryParameterTable[5] = 0x00;
				gubHardBitRetryParameterTable[6] = 0x18;
				gubHardBitRetryParameterTable[7] = 0x00;
				gubHardBitRetryParameterTable[8] = 0x1C;
				gubHardBitRetryParameterTable[9] = 0x00;
				gubHardBitRetryParameterTable[10] = 0x70;
				gubHardBitRetryParameterTable[11] = 0x00;
				gubHardBitRetryParameterTable[12] = 0x64;
				gubHardBitRetryParameterTable[13] = 0x00;
				gubHardBitRetryParameterTable[14] = 0x24;
				gubHardBitRetryParameterTable[15] = 0x00;
				gubHardBitRetryParameterTable[16] = 0x5C;
				gubHardBitRetryParameterTable[17] = 0x00;
				gubHardBitRetryParameterTable[18] = 0x54;
				gubHardBitRetryParameterTable[19] = 0x00;

				// RETRY_D2D3_BLOCK_TYPE
				// 15nm, ref: t15-rr-v0.3-M2CMP01-138_15nm MLC Read Retry Sequence Rev0.3.pdf
				gubHardBitRetryParameterTable[20] = 0x00;
				gubHardBitRetryParameterTable[21] = 0x00;
				gubHardBitRetryParameterTable[22] = 0x00;
				gubHardBitRetryParameterTable[23] = 0x00;
				gubHardBitRetryParameterTable[24] = 0x00;

				gubHardBitRetryParameterTable[25] = 0x02;
				gubHardBitRetryParameterTable[26] = 0x04;
				gubHardBitRetryParameterTable[27] = 0x02;
				gubHardBitRetryParameterTable[28] = 0x00;
				gubHardBitRetryParameterTable[29] = 0x00;

				gubHardBitRetryParameterTable[30] = 0x7C;
				gubHardBitRetryParameterTable[31] = 0x00;
				gubHardBitRetryParameterTable[32] = 0x7C;
				gubHardBitRetryParameterTable[33] = 0x7C;
				gubHardBitRetryParameterTable[34] = 0x00;

				gubHardBitRetryParameterTable[35] = 0x7A;
				gubHardBitRetryParameterTable[36] = 0x7C;
				gubHardBitRetryParameterTable[37] = 0x7A;
				gubHardBitRetryParameterTable[38] = 0x7C;
				gubHardBitRetryParameterTable[39] = 0x00;

				gubHardBitRetryParameterTable[40] = 0x78;
				gubHardBitRetryParameterTable[41] = 0x00;
				gubHardBitRetryParameterTable[42] = 0x78;
				gubHardBitRetryParameterTable[43] = 0x7A;
				gubHardBitRetryParameterTable[44] = 0x00;

				gubHardBitRetryParameterTable[45] = 0x7E;
				gubHardBitRetryParameterTable[46] = 0x02;
				gubHardBitRetryParameterTable[47] = 0x7E;
				gubHardBitRetryParameterTable[48] = 0x7A;
				gubHardBitRetryParameterTable[49] = 0x00;

				gubHardBitRetryParameterTable[50] = 0x76;
				gubHardBitRetryParameterTable[51] = 0x04;
				gubHardBitRetryParameterTable[52] = 0x76;
				gubHardBitRetryParameterTable[53] = 0x02;
				gubHardBitRetryParameterTable[54] = 0x00;

				gubHardBitRetryParameterTable[55] = 0x04;
				gubHardBitRetryParameterTable[56] = 0x00;
				gubHardBitRetryParameterTable[57] = 0x04;
				gubHardBitRetryParameterTable[58] = 0x78;
				gubHardBitRetryParameterTable[59] = 0x00;

				gubHardBitRetryParameterTable[60] = 0x06;
				gubHardBitRetryParameterTable[61] = 0x00;
				gubHardBitRetryParameterTable[62] = 0x06;
				gubHardBitRetryParameterTable[63] = 0x76;
				gubHardBitRetryParameterTable[64] = 0x00;

				gubHardBitRetryParameterTable[65] = 0x74;
				gubHardBitRetryParameterTable[66] = 0x7C;
				gubHardBitRetryParameterTable[67] = 0x74;
				gubHardBitRetryParameterTable[68] = 0x76;
				gubHardBitRetryParameterTable[69] = 0x00;
			}
		}
	}
	// ==========================ERIC_ADD==============================
#if TLC
	else if (guoFlashUseType.B.btToshibaBiCs2 && TLC_BICS2) {
#if TLC_BICS2
		if (guoFlashDefaultType.B.btEnterprise) {
			// ref: M2CMP01-293 [BiCS FLASH Gen2 eTLC 256Gb]Read Retry with Set Feature 20161122.pdf
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 8; // uwGroupNumber[0] Fast    1+7
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 21; // uwGroupNumber[1] Non-Fast  1+20
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
		}
		else {
			// ref: M2CMP01-280 [BF2 TLC 256Gb]Read Retry with Set Feature_0.2 for SSD.pdf
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 8; // uwGroupNumber[0] Fast    1+7
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 31; // uwGroupNumber[1] Non-Fast  1+30
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
		}
#if(!BURNER)
		if (guoFlashDefaultType.B.btEnterprise) {
			memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC2_Enterprise, sizeof(gubRetryRTable_BISC2_Enterprise));
		}
		else {
			memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC2, sizeof(gubRetryRTable_BISC2));
		}
#endif
#endif
	}
	else if (guoFlashUseType.B.btToshibaBiCs3 && TLC_BICS2) {
#if TLC_BICS2
		if (guoFlashUseType.B.btSandisk) {
			if (gubDensitysize == FLH_Size_512Gb) {
				// ref: sBiCS3-rr-v1.0-appnote010 - BiCS3 512Gb X3_Dynamic_Read_Sequence_with_SET_FEATURES_ver1.0.pdf
				gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 21; // uwGroupNumber[0] Fast
				gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

				gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

				gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 43; // uwGroupNumber[1] Non-Fast  41+2
				gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
			}
			else if (gubDensitysize == FLH_Size_128Gb) {
				// ref: sBiCS3-rr-v1.0-appnote008 - BiCS3 128Gb X3_Dynamic_Read_Sequence_with_SET_FEATURES_ver1.1.pdf
				gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 28; // uwGroupNumber[0] Fast
				gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

				gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

				gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 35; // uwGroupNumber[1] Non-Fast 35
				gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
			}
			else {
				// ref: sBiCS3-rr-v1.1-appnote005 - BiCS3 256Gb X3_Dynamic_Read_Sequence_with_SET_FEATURES_ver1.4(SDWFR-256GB3ED3D-A) + 6 industrial
				gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 11; // uwGroupNumber[0] Fast
				gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

				gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

				gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 47; // uwGroupNumber[1] Non-Fast  41 + 6
				gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
			}
		}
		else if (gubDensitysize == FLH_Size_512Gb) {
			// ref: M2CMP01-457 BiCS FLASH Gen3 512Gb TLC Read Retry with Set Feature Rev1.0 - Relax Read.pdf
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 8; // uwGroupNumber[0] Fast
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];
			gRRTMode.B.btRelaxMode = 1;// 2合1 for PPS

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 56; // uwGroupNumber[1] Non-Fast  1+55
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
		}
		else if (gubDensitysize == FLH_Size_128Gb) {
			//M2CMP01-465 BiCS FLASH Gen3 128Gb TLC Read Retry with Set Feature Rev1.1(Relax Read).pdf
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 8; // uwGroupNumber[0] Fast
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];
			gRRTMode.B.btRelaxMode = 1;

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 56; // uwGroupNumber[1] Non-Fast  1+55
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
		}
		else {
			// ref: M2CMP01-339_BiCS FLASH Gen3 TLC Read Retry with Set Feature Rev0.6.pdf
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 8; // uwGroupNumber[0] Fast
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];
			gRRTMode.B.btRelaxMode = 1;

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 56; // uwGroupNumber[1] Non-Fast  55 + 1
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
		}

#if(!BURNER)
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
#endif
#endif
	}
#if TSB_BICS4_SUPPORT
	else if (guoFlashUseType.B.btToshibaBiCs4 && TLC_BICS2) {
#if TLC_BICS2
		if (guoFlashUseType.B.btSandisk) {
			if (gubDensitysize == FLH_Size_512Gb) {
				// ref: sBiCS4-rr-V1.0-3D-Gen4-512Gb-2P-Dynamic_Read_Sequence_with_SET_FEATURES_r1.0(CS2).pdf
				gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 17; // uwGroupNumber[0] Fast    16+1
				gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

				gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

				gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 33; // uwGroupNumber[1] Non-Fast   32+1
				gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
			}
			else {
				// ref: sBiCS4-rr-V1.0-3D-Gen4-256Gb-2P-Dynamic_Read_Sequence_with_SET_FEATURES_r1.0(CS2)
				gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 22; // uwGroupNumber[0] Fast    22+1
				gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

				gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

				gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 65; // uwGroupNumber[1] Non-Fast   64+1
				gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
			}
		}
		else {
			if (gubDensitysize == FLH_Size_512Gb) {
				// ref: M2CMP01-597 BiCS FLASH Gen4 512Gb TLC Read Retry with Set Feature rev0.1 - for CM Wafer.pdf
				// 620 +2
				gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 24; // uwGroupNumber[0] Fast    23+1
				gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

				gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

				gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 60; // uwGroupNumber[1] Non-Fast   55+3+2
				gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast
			}
			else {
				// check early final 搬到後面做  flaBics4_Revision_Identify

				// ref: M2CMP01-483 Early BiCS4 256Gb TLC Read Retry with Set Feature Rev1.0 - Retrieval Read.pdf
				gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 24; // uwGroupNumber[0] Fast   23+1
				gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

				gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];
				gRRTMode.B.btRelaxMode = 1;

				gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 59; // uwGroupNumber[1] Non-Fast  55+4
				gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast

			}
		}
#if(!BURNER)
		if (guoFlashUseType.B.btSandisk) {
			if (gubDensitysize == FLH_Size_512Gb) {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_SANDISK_512Gb, sizeof(gubRetryRTable_BISC4_SANDISK_512Gb));
			}
			else {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_SANDISK_256Gb, sizeof(gubRetryRTable_BISC4_SANDISK_256Gb));
			}
		}
		else {
			if (gubDensitysize == FLH_Size_512Gb) {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_512Gb, sizeof(gubRetryRTable_BISC4_512Gb));
			}
			else {
				memcpy(gubHardBitRetryParameterTable, gubRetryRTable_BISC4_256Gb_Early, sizeof(gubRetryRTable_BISC4_256Gb_Early));
			}
		}
#endif
#endif
	}
#endif
	else if (TLC && (!TLC_BICS2)) {
		if (guoFlashUseType.B.btSandisk) {
			//UartString("\r\nTLC");
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 25; // uwGroupNumber[0] Fast
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 47; // uwGroupNumber[1] Non-Fast
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast

			// RETRY_D1_BLOCK_TYPE
			// s15-rr-v1.2-appnote04 - 1Znm 128Gb eX3 2-plane ABL Dynamic Read Sequence - generic 1.2
			gubHardBitRetryParameterTable[0] = 0x00;  // Reset
			gubHardBitRetryParameterTable[1] = 0x08;
			gubHardBitRetryParameterTable[2] = 0xF8;
			gubHardBitRetryParameterTable[3] = 0x10;
			gubHardBitRetryParameterTable[4] = 0xF0;
			gubHardBitRetryParameterTable[5] = 0x18;
			gubHardBitRetryParameterTable[6] = 0xE8;
			gubHardBitRetryParameterTable[7] = 0x20;
			gubHardBitRetryParameterTable[8] = 0xE0;
			gubHardBitRetryParameterTable[9] = 0x28;
			gubHardBitRetryParameterTable[10] = 0xD8;
			gubHardBitRetryParameterTable[11] = 0x30;
			gubHardBitRetryParameterTable[12] = 0xD0;
			gubHardBitRetryParameterTable[13] = 0x38;
			gubHardBitRetryParameterTable[14] = 0xC8;
			gubHardBitRetryParameterTable[15] = 0x40;
			gubHardBitRetryParameterTable[16] = 0xC0;
			gubHardBitRetryParameterTable[17] = 0x48;
			gubHardBitRetryParameterTable[18] = 0xB8;
			gubHardBitRetryParameterTable[19] = 0x50;
			gubHardBitRetryParameterTable[20] = 0xB0;
			gubHardBitRetryParameterTable[21] = 0x58;
			gubHardBitRetryParameterTable[22] = 0x60;
			gubHardBitRetryParameterTable[23] = 0x68;
			gubHardBitRetryParameterTable[24] = 0x70;

			// RETRY_D2D3_BLOCK_TYPE
			// s15-rr-v1.2-appnote04 - 1Znm 128Gb eX3 2-plane ABL Dynamic Read Sequence - generic 1.2
			//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Case0
			gubHardBitRetryParameterTable[25] = 0x00;
			gubHardBitRetryParameterTable[26] = 0x00;
			gubHardBitRetryParameterTable[27] = 0x00;
			gubHardBitRetryParameterTable[28] = 0x00;
			gubHardBitRetryParameterTable[29] = 0x00;
			gubHardBitRetryParameterTable[30] = 0x00;
			gubHardBitRetryParameterTable[31] = 0x00;
			// 0xF8 0x00 0x10 0x08 0x08 0x00 0xF8, // Case1
			gubHardBitRetryParameterTable[32] = 0xF8;
			gubHardBitRetryParameterTable[33] = 0x00;
			gubHardBitRetryParameterTable[34] = 0x10;
			gubHardBitRetryParameterTable[35] = 0x08;
			gubHardBitRetryParameterTable[36] = 0x08;
			gubHardBitRetryParameterTable[37] = 0x00;
			gubHardBitRetryParameterTable[38] = 0xF8;
			// 0xF8 0xF8 0xF0 0xF8 0xF8 0xF8 0xF8, // Case2
			gubHardBitRetryParameterTable[39] = 0xF8;
			gubHardBitRetryParameterTable[40] = 0xF8;
			gubHardBitRetryParameterTable[41] = 0xF0;
			gubHardBitRetryParameterTable[42] = 0xF8;
			gubHardBitRetryParameterTable[43] = 0xF8;
			gubHardBitRetryParameterTable[44] = 0xF8;
			gubHardBitRetryParameterTable[45] = 0xF8;
			// 0xE8 0x00 0x00 0x00 0xF8 0xF0 0xE0, // Case3
			gubHardBitRetryParameterTable[46] = 0xE8;
			gubHardBitRetryParameterTable[47] = 0x00;
			gubHardBitRetryParameterTable[48] = 0x00;
			gubHardBitRetryParameterTable[49] = 0x00;
			gubHardBitRetryParameterTable[50] = 0xF8;
			gubHardBitRetryParameterTable[51] = 0xF0;
			gubHardBitRetryParameterTable[52] = 0xE0;
			// 0x00 0x08 0xF8 0x08 0x08 0x00 0x00, // Case4
			gubHardBitRetryParameterTable[53] = 0x00;
			gubHardBitRetryParameterTable[54] = 0x08;
			gubHardBitRetryParameterTable[55] = 0xF8;
			gubHardBitRetryParameterTable[56] = 0x08;
			gubHardBitRetryParameterTable[57] = 0x08;
			gubHardBitRetryParameterTable[58] = 0x00;
			gubHardBitRetryParameterTable[59] = 0x00;
			// 0xF0 0xF8 0xF8 0xF8 0x00 0xF0 0xE8, // Case5
			gubHardBitRetryParameterTable[60] = 0xF0;
			gubHardBitRetryParameterTable[61] = 0xF8;
			gubHardBitRetryParameterTable[62] = 0xF8;
			gubHardBitRetryParameterTable[63] = 0xF8;
			gubHardBitRetryParameterTable[64] = 0x00;
			gubHardBitRetryParameterTable[65] = 0xF0;
			gubHardBitRetryParameterTable[66] = 0xE8;
			// 0xF0 0x08 0x08 0xF8 0xF0 0xF0 0x00, // Case6
			gubHardBitRetryParameterTable[67] = 0xF0;
			gubHardBitRetryParameterTable[68] = 0x08;
			gubHardBitRetryParameterTable[69] = 0x08;
			gubHardBitRetryParameterTable[70] = 0xF8;
			gubHardBitRetryParameterTable[71] = 0xF0;
			gubHardBitRetryParameterTable[72] = 0xF0;
			gubHardBitRetryParameterTable[73] = 0x00;
			// 0x08 0x08 0x08 0x08 0xF8 0xF8 0xE8, // Case7
			gubHardBitRetryParameterTable[74] = 0x08;
			gubHardBitRetryParameterTable[75] = 0x08;
			gubHardBitRetryParameterTable[76] = 0x08;
			gubHardBitRetryParameterTable[77] = 0x08;
			gubHardBitRetryParameterTable[78] = 0xF8;
			gubHardBitRetryParameterTable[79] = 0xF8;
			gubHardBitRetryParameterTable[80] = 0xE8;
			// 0x00 0xF0 0x00 0xF8 0xF0 0xF8 0xF8, // Case8
			gubHardBitRetryParameterTable[81] = 0x00;
			gubHardBitRetryParameterTable[82] = 0xF0;
			gubHardBitRetryParameterTable[83] = 0x00;
			gubHardBitRetryParameterTable[84] = 0xF8;
			gubHardBitRetryParameterTable[85] = 0xF0;
			gubHardBitRetryParameterTable[86] = 0xF8;
			gubHardBitRetryParameterTable[87] = 0xF8;
			// 0x00 0x08 0x00 0x00 0xF8 0x00 0xE8, // Case9
			gubHardBitRetryParameterTable[88] = 0x00;
			gubHardBitRetryParameterTable[89] = 0x08;
			gubHardBitRetryParameterTable[90] = 0x00;
			gubHardBitRetryParameterTable[91] = 0x00;
			gubHardBitRetryParameterTable[92] = 0xF8;
			gubHardBitRetryParameterTable[93] = 0x00;
			gubHardBitRetryParameterTable[94] = 0xE8;
			// 0xF8 0x00 0x00 0xF8 0xE8 0xF8 0x08, // Case10
			gubHardBitRetryParameterTable[95] = 0xF8;
			gubHardBitRetryParameterTable[96] = 0x00;
			gubHardBitRetryParameterTable[97] = 0x00;
			gubHardBitRetryParameterTable[98] = 0xF8;
			gubHardBitRetryParameterTable[99] = 0xE8;
			gubHardBitRetryParameterTable[100] = 0xF8;
			gubHardBitRetryParameterTable[101] = 0x08;
			// 0xF8 0x00 0xF8 0x08 0xF0 0xF8 0xF8, // Case11
			gubHardBitRetryParameterTable[102] = 0xF8;
			gubHardBitRetryParameterTable[103] = 0x00;
			gubHardBitRetryParameterTable[104] = 0xF8;
			gubHardBitRetryParameterTable[105] = 0x08;
			gubHardBitRetryParameterTable[106] = 0xF0;
			gubHardBitRetryParameterTable[107] = 0xF8;
			gubHardBitRetryParameterTable[108] = 0xF8;
			// 0xF8 0x08 0x08 0x00 0x00 0xF8 0xF8, // Case12
			gubHardBitRetryParameterTable[109] = 0xF8;
			gubHardBitRetryParameterTable[110] = 0x08;
			gubHardBitRetryParameterTable[111] = 0x08;
			gubHardBitRetryParameterTable[112] = 0x00;
			gubHardBitRetryParameterTable[113] = 0x00;
			gubHardBitRetryParameterTable[114] = 0xF8;
			gubHardBitRetryParameterTable[115] = 0xF8;
			// 0x08 0xF8 0xF8 0x00 0x00 0xF8 0xF0, // Case13
			gubHardBitRetryParameterTable[116] = 0x08;
			gubHardBitRetryParameterTable[117] = 0xF8;
			gubHardBitRetryParameterTable[118] = 0xF8;
			gubHardBitRetryParameterTable[119] = 0x00;
			gubHardBitRetryParameterTable[120] = 0x00;
			gubHardBitRetryParameterTable[121] = 0xF8;
			gubHardBitRetryParameterTable[122] = 0xF0;
			// 0xF0 0xF8 0x00 0x00 0xF8 0x00 0xF0, // Case14
			gubHardBitRetryParameterTable[123] = 0xF0;
			gubHardBitRetryParameterTable[124] = 0xF8;
			gubHardBitRetryParameterTable[125] = 0x00;
			gubHardBitRetryParameterTable[126] = 0x00;
			gubHardBitRetryParameterTable[127] = 0xF8;
			gubHardBitRetryParameterTable[128] = 0x00;
			gubHardBitRetryParameterTable[129] = 0xF0;
			// 0x08 0x00 0x08 0x00 0xF0 0xF8 0xF0, // Case15
			gubHardBitRetryParameterTable[130] = 0x08;
			gubHardBitRetryParameterTable[131] = 0x00;
			gubHardBitRetryParameterTable[132] = 0x08;
			gubHardBitRetryParameterTable[133] = 0x00;
			gubHardBitRetryParameterTable[134] = 0xF0;
			gubHardBitRetryParameterTable[135] = 0xF8;
			gubHardBitRetryParameterTable[136] = 0xF0;
			// 0x10 0x00 0x10 0x00 0xF8 0x08 0x00, // Case16
			gubHardBitRetryParameterTable[137] = 0x10;
			gubHardBitRetryParameterTable[138] = 0x00;
			gubHardBitRetryParameterTable[139] = 0x10;
			gubHardBitRetryParameterTable[140] = 0x00;
			gubHardBitRetryParameterTable[141] = 0xF8;
			gubHardBitRetryParameterTable[142] = 0x08;
			gubHardBitRetryParameterTable[143] = 0x00;
			// 0xE8 0x00 0xF0 0xF8 0xF0 0xF0 0xF0, // Case17
			gubHardBitRetryParameterTable[144] = 0xE8;
			gubHardBitRetryParameterTable[145] = 0x00;
			gubHardBitRetryParameterTable[146] = 0xF0;
			gubHardBitRetryParameterTable[147] = 0xF8;
			gubHardBitRetryParameterTable[148] = 0xF0;
			gubHardBitRetryParameterTable[149] = 0xF0;
			gubHardBitRetryParameterTable[150] = 0xF0;
			// 0xF0 0x00 0x08 0xF8 0xE8 0x00 0x08, // Case18
			gubHardBitRetryParameterTable[151] = 0xF0;
			gubHardBitRetryParameterTable[152] = 0x00;
			gubHardBitRetryParameterTable[153] = 0x08;
			gubHardBitRetryParameterTable[154] = 0xF8;
			gubHardBitRetryParameterTable[155] = 0xE8;
			gubHardBitRetryParameterTable[156] = 0x00;
			gubHardBitRetryParameterTable[157] = 0x08;
			// 0x08 0x10 0x10 0x00 0x08 0xF8 0x08, // Case19
			gubHardBitRetryParameterTable[158] = 0x08;
			gubHardBitRetryParameterTable[159] = 0x10;
			gubHardBitRetryParameterTable[160] = 0x10;
			gubHardBitRetryParameterTable[161] = 0x00;
			gubHardBitRetryParameterTable[162] = 0x08;
			gubHardBitRetryParameterTable[163] = 0xF8;
			gubHardBitRetryParameterTable[164] = 0x08;
			// 0x10 0xF8 0x18 0x00 0x08 0xFC 0x00, // Case20
			gubHardBitRetryParameterTable[165] = 0x10;
			gubHardBitRetryParameterTable[166] = 0xF8;
			gubHardBitRetryParameterTable[167] = 0x18;
			gubHardBitRetryParameterTable[168] = 0x00;
			gubHardBitRetryParameterTable[169] = 0x08;
			gubHardBitRetryParameterTable[170] = 0xFC;
			gubHardBitRetryParameterTable[171] = 0x00;
			// 0x10 0x08 0x18 0x08 0x00 0xFC 0xF8, // Case21
			gubHardBitRetryParameterTable[172] = 0x10;
			gubHardBitRetryParameterTable[173] = 0x08;
			gubHardBitRetryParameterTable[174] = 0x18;
			gubHardBitRetryParameterTable[175] = 0x08;
			gubHardBitRetryParameterTable[176] = 0x00;
			gubHardBitRetryParameterTable[177] = 0xFC;
			gubHardBitRetryParameterTable[178] = 0xF8;
			// 0x18 0x10 0x10 0x00 0x00 0x00 0xF0, // Case22
			gubHardBitRetryParameterTable[179] = 0x18;
			gubHardBitRetryParameterTable[180] = 0x10;
			gubHardBitRetryParameterTable[181] = 0x10;
			gubHardBitRetryParameterTable[182] = 0x00;
			gubHardBitRetryParameterTable[183] = 0x00;
			gubHardBitRetryParameterTable[184] = 0x00;
			gubHardBitRetryParameterTable[185] = 0xF0;
			// 0x00 0x00 0x04 0x00 0xFC 0xFC 0xFC, // Case23
			gubHardBitRetryParameterTable[186] = 0x00;
			gubHardBitRetryParameterTable[187] = 0x00;
			gubHardBitRetryParameterTable[188] = 0x04;
			gubHardBitRetryParameterTable[189] = 0x00;
			gubHardBitRetryParameterTable[190] = 0xFC;
			gubHardBitRetryParameterTable[191] = 0xFC;
			gubHardBitRetryParameterTable[192] = 0xFC;
			// 0x08 0x08 0x10 0x00 0x10 0xFC 0x10, // Case24
			gubHardBitRetryParameterTable[193] = 0x08;
			gubHardBitRetryParameterTable[194] = 0x08;
			gubHardBitRetryParameterTable[195] = 0x10;
			gubHardBitRetryParameterTable[196] = 0x00;
			gubHardBitRetryParameterTable[197] = 0x10;
			gubHardBitRetryParameterTable[198] = 0xFC;
			gubHardBitRetryParameterTable[199] = 0x10;
			// 0x00 0x10 0x08 0x00 0x10 0xFC 0x10, // Case25
			gubHardBitRetryParameterTable[200] = 0x00;
			gubHardBitRetryParameterTable[201] = 0x10;
			gubHardBitRetryParameterTable[202] = 0x08;
			gubHardBitRetryParameterTable[203] = 0x00;
			gubHardBitRetryParameterTable[204] = 0x10;
			gubHardBitRetryParameterTable[205] = 0xFC;
			gubHardBitRetryParameterTable[206] = 0x10;
			// 0xF8 0x00 0x00 0xF8 0x10 0xFC 0x10, // Case26
			gubHardBitRetryParameterTable[207] = 0xF8;
			gubHardBitRetryParameterTable[208] = 0x00;
			gubHardBitRetryParameterTable[209] = 0x00;
			gubHardBitRetryParameterTable[210] = 0xF8;
			gubHardBitRetryParameterTable[211] = 0x10;
			gubHardBitRetryParameterTable[212] = 0xFC;
			gubHardBitRetryParameterTable[213] = 0x10;
			// 0xF0 0xF8 0xF8 0xF8 0x08 0xFC 0x08, // Case27
			gubHardBitRetryParameterTable[214] = 0xF0;
			gubHardBitRetryParameterTable[215] = 0xF8;
			gubHardBitRetryParameterTable[216] = 0xF8;
			gubHardBitRetryParameterTable[217] = 0xF8;
			gubHardBitRetryParameterTable[218] = 0x08;
			gubHardBitRetryParameterTable[219] = 0xFC;
			gubHardBitRetryParameterTable[220] = 0x08;
			// 0xE8 0x04 0xF0 0x00 0x08 0x00 0x00, // Case28
			gubHardBitRetryParameterTable[221] = 0xE8;
			gubHardBitRetryParameterTable[222] = 0x04;
			gubHardBitRetryParameterTable[223] = 0xF0;
			gubHardBitRetryParameterTable[224] = 0x00;
			gubHardBitRetryParameterTable[225] = 0x08;
			gubHardBitRetryParameterTable[226] = 0x00;
			gubHardBitRetryParameterTable[227] = 0x00;
			// 0xE8 0x08 0xF0 0x08 0x00 0x08 0xE8, // Case29
			gubHardBitRetryParameterTable[228] = 0xE8;
			gubHardBitRetryParameterTable[229] = 0x08;
			gubHardBitRetryParameterTable[230] = 0xF0;
			gubHardBitRetryParameterTable[231] = 0x08;
			gubHardBitRetryParameterTable[232] = 0x00;
			gubHardBitRetryParameterTable[233] = 0x08;
			gubHardBitRetryParameterTable[234] = 0xE8;
			// 0xE0 0x10 0xE8 0x08 0xF8 0x08 0xE8, // Case30
			gubHardBitRetryParameterTable[235] = 0xE0;
			gubHardBitRetryParameterTable[236] = 0x10;
			gubHardBitRetryParameterTable[237] = 0xE8;
			gubHardBitRetryParameterTable[238] = 0x08;
			gubHardBitRetryParameterTable[239] = 0xF8;
			gubHardBitRetryParameterTable[240] = 0x08;
			gubHardBitRetryParameterTable[241] = 0xE8;
			// 0xE0 0x08 0x00 0x10 0x00 0x10 0x00  // Case31
			gubHardBitRetryParameterTable[242] = 0xE0;
			gubHardBitRetryParameterTable[243] = 0x08;
			gubHardBitRetryParameterTable[244] = 0x00;
			gubHardBitRetryParameterTable[245] = 0x10;
			gubHardBitRetryParameterTable[246] = 0x00;
			gubHardBitRetryParameterTable[247] = 0x10;
			gubHardBitRetryParameterTable[248] = 0x00;
			// 0xE0 0x04 0x00 0xF8 0xF0 0xF8 0x00  // Case32
			gubHardBitRetryParameterTable[249] = 0xE0;
			gubHardBitRetryParameterTable[250] = 0x04;
			gubHardBitRetryParameterTable[251] = 0x00;
			gubHardBitRetryParameterTable[252] = 0xF8;
			gubHardBitRetryParameterTable[253] = 0xF0;
			gubHardBitRetryParameterTable[254] = 0xF8;
			gubHardBitRetryParameterTable[255] = 0x00;
			// 0xE0 0x04 0x00 0x08 0xE8 0x08 0x00  // Case33
			gubHardBitRetryParameterTable[256] = 0xE0;
			gubHardBitRetryParameterTable[257] = 0x04;
			gubHardBitRetryParameterTable[258] = 0x00;
			gubHardBitRetryParameterTable[259] = 0x08;
			gubHardBitRetryParameterTable[260] = 0xE8;
			gubHardBitRetryParameterTable[261] = 0x08;
			gubHardBitRetryParameterTable[262] = 0x00;
			// 0xE0 0x04 0x00 0x00 0xE0 0xFC 0x00  // Case34
			gubHardBitRetryParameterTable[263] = 0xE0;
			gubHardBitRetryParameterTable[264] = 0x04;
			gubHardBitRetryParameterTable[265] = 0x00;
			gubHardBitRetryParameterTable[266] = 0x00;
			gubHardBitRetryParameterTable[267] = 0xE0;
			gubHardBitRetryParameterTable[268] = 0xFC;
			gubHardBitRetryParameterTable[269] = 0x00;
			// 0xE8 0x04 0x00 0x10 0xE8 0x10 0x00  // Case35
			gubHardBitRetryParameterTable[270] = 0xE8;
			gubHardBitRetryParameterTable[271] = 0x04;
			gubHardBitRetryParameterTable[272] = 0x00;
			gubHardBitRetryParameterTable[273] = 0x10;
			gubHardBitRetryParameterTable[274] = 0xE8;
			gubHardBitRetryParameterTable[275] = 0x10;
			gubHardBitRetryParameterTable[276] = 0x00;
			// 0xE8 0xFC 0x00 0x08 0xE0 0x08 0x00  // Case36
			gubHardBitRetryParameterTable[277] = 0xE8;
			gubHardBitRetryParameterTable[278] = 0xFC;
			gubHardBitRetryParameterTable[279] = 0x00;
			gubHardBitRetryParameterTable[280] = 0x08;
			gubHardBitRetryParameterTable[281] = 0xE0;
			gubHardBitRetryParameterTable[282] = 0x08;
			gubHardBitRetryParameterTable[283] = 0x00;
			// 0x00 0xFC 0x00 0x00 0x00 0x00 0x00  // Case37
			gubHardBitRetryParameterTable[284] = 0x00;
			gubHardBitRetryParameterTable[285] = 0xFC;
			gubHardBitRetryParameterTable[286] = 0x00;
			gubHardBitRetryParameterTable[287] = 0x00;
			gubHardBitRetryParameterTable[288] = 0x00;
			gubHardBitRetryParameterTable[289] = 0x00;
			gubHardBitRetryParameterTable[290] = 0x00;
			// 0x00 0xFC 0x00 0x00 0x00 0xFC 0x00  // Case38
			gubHardBitRetryParameterTable[291] = 0x00;
			gubHardBitRetryParameterTable[292] = 0xFC;
			gubHardBitRetryParameterTable[293] = 0x00;
			gubHardBitRetryParameterTable[294] = 0x00;
			gubHardBitRetryParameterTable[295] = 0x00;
			gubHardBitRetryParameterTable[296] = 0xFC;
			gubHardBitRetryParameterTable[297] = 0x00;
			// 0x00 0xFC 0x00 0xF8 0x00 0xF8 0x00  // Case39
			gubHardBitRetryParameterTable[298] = 0x00;
			gubHardBitRetryParameterTable[299] = 0xFC;
			gubHardBitRetryParameterTable[300] = 0x00;
			gubHardBitRetryParameterTable[301] = 0xF8;
			gubHardBitRetryParameterTable[302] = 0x00;
			gubHardBitRetryParameterTable[303] = 0xF8;
			gubHardBitRetryParameterTable[304] = 0x00;
			// 0x00 0xFC 0x00 0xF0 0x00 0xF0 0x00  // Case40
			gubHardBitRetryParameterTable[305] = 0x00;
			gubHardBitRetryParameterTable[306] = 0xFC;
			gubHardBitRetryParameterTable[307] = 0x00;
			gubHardBitRetryParameterTable[308] = 0xF0;
			gubHardBitRetryParameterTable[309] = 0x00;
			gubHardBitRetryParameterTable[310] = 0xF0;
			gubHardBitRetryParameterTable[311] = 0x00;
			// 0x00 0xF0 0x00 0xF0 0x00 0xF0 0x00  // Case41
			gubHardBitRetryParameterTable[312] = 0x00;
			gubHardBitRetryParameterTable[313] = 0xF0;
			gubHardBitRetryParameterTable[314] = 0x00;
			gubHardBitRetryParameterTable[315] = 0xF0;
			gubHardBitRetryParameterTable[316] = 0x00;
			gubHardBitRetryParameterTable[317] = 0xF0;
			gubHardBitRetryParameterTable[318] = 0x00;
			// 0x00 0xF8 0x00 0xF0 0x00 0xF0 0x00  // Case42
			gubHardBitRetryParameterTable[319] = 0x00;
			gubHardBitRetryParameterTable[320] = 0xF8;
			gubHardBitRetryParameterTable[321] = 0x00;
			gubHardBitRetryParameterTable[322] = 0xF0;
			gubHardBitRetryParameterTable[323] = 0x00;
			gubHardBitRetryParameterTable[324] = 0xF0;
			gubHardBitRetryParameterTable[325] = 0x00;
			// 0x00 0xF0 0x00 0xE8 0x00 0xE8 0x00  // Case43
			gubHardBitRetryParameterTable[326] = 0x00;
			gubHardBitRetryParameterTable[327] = 0xF0;
			gubHardBitRetryParameterTable[328] = 0x00;
			gubHardBitRetryParameterTable[329] = 0xE8;
			gubHardBitRetryParameterTable[330] = 0x00;
			gubHardBitRetryParameterTable[331] = 0xE8;
			gubHardBitRetryParameterTable[332] = 0x00;
			// 0x00 0xE8 0x00 0xE8 0x00 0xE8 0x00  // Case44
			gubHardBitRetryParameterTable[333] = 0x00;
			gubHardBitRetryParameterTable[334] = 0xE8;
			gubHardBitRetryParameterTable[335] = 0x00;
			gubHardBitRetryParameterTable[336] = 0xE8;
			gubHardBitRetryParameterTable[337] = 0x00;
			gubHardBitRetryParameterTable[338] = 0xE8;
			gubHardBitRetryParameterTable[339] = 0x00;
			// 0x00 0xE8 0x00 0xF0 0x00 0xF0 0x00  // Case45
			gubHardBitRetryParameterTable[340] = 0x00;
			gubHardBitRetryParameterTable[341] = 0xE8;
			gubHardBitRetryParameterTable[342] = 0x00;
			gubHardBitRetryParameterTable[343] = 0XF0;
			gubHardBitRetryParameterTable[344] = 0x00;
			gubHardBitRetryParameterTable[345] = 0xF0;
			gubHardBitRetryParameterTable[346] = 0x00;
			// 0x00 0xE8 0x00 0xE0 0x00 0xE0 0x00  // Case46
			gubHardBitRetryParameterTable[347] = 0x00;
			gubHardBitRetryParameterTable[348] = 0xE8;
			gubHardBitRetryParameterTable[349] = 0x00;
			gubHardBitRetryParameterTable[350] = 0xE0;
			gubHardBitRetryParameterTable[351] = 0x00;
			gubHardBitRetryParameterTable[352] = 0xE0;
			gubHardBitRetryParameterTable[353] = 0x00;
		}
		else {
			//Toshiba
			//UartString("\r\nTLC");
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 28; // uwGroupNumber[0] Fast
			gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 2; // uwBytePerGroup[0] Fast

			gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

#if(!TLC_BICS2)
			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = RETRY_SEQUENCE_MAX_COUNT - 1; // 36   uwGroupNumber[1] Non-Fast
#endif
			gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 7; // uwBytePerGroup[1] Non-Fast

#if(!BURNER)
			memcpy(gubHardBitRetryParameterTable, gubRetryRTable_15nmTLC, sizeof(gubRetryRTable_15nmTLC));
#endif
		}
	}
#endif
#if YMTC

	gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = 10; // uwGroupNumber[0] Fast
	gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE] = 1; // uwBytePerGroup[0] Fast

	gubHardBitRetryD2D3GroupBase = gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] * gubHardBitRetryBytePerGroup[RETRY_D1_BLOCK_TYPE];

	gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = 87; // uwGroupNumber[1] Non-Fast
	gubHardBitRetryBytePerGroup[RETRY_D2D3_BLOCK_TYPE] = 8; // uwBytePerGroup[1] Non-Fast
	memcpy(gubHardBitRetryParameterTable, gubRetryRTable_YMTC, sizeof(gubRetryRTable_YMTC));
#elif MicronFlashOnly

	// 先給CheckFlashType給的default gubParameterPage值，之後LoadSystemBlock又會再更新gubParameterPage，最後在HB Retry給到gubHardBitRetryGroupNumber
	if (guoFlashUseType.B.btIntelMicron) {
		// Number of supported retry options was defined in byte 180 of parameter page , minus 1 to skip default option 0
		gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = gubParameterPage[180] - 1;
		gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = gubParameterPage[180] - 1;
	}
#endif
#if (Hynix)
	// 先給一次值，之後再看要不要統一由這邊給值還是在HB Retry給值
	if (guoFlashDefaultType.B.btHynix) {
		if (guoFlashUseType.B.bt8LC) {
			gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] = gsHynixRetryInfo.ubSlcRRstepCnt;
			gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] = gsHynixRetryInfo.ubXlcRRstepCnt;
		}
	}
#endif
	// ================================================================

	for ( ubi = 0 ; ubi < HB_RETRY_Q_NUM ; ubi++ ) {
		gHBRetryQI[ubi].ubLinkFirst = 0;
		gHBRetryQI[ubi].ubLinkLast = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE] - 1) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE] - 1);
		gHBRetryQI[ubi].ubLinkNum = (ubi == HB_RETRY_Q_NUM - 1) ? (gubHardBitRetryGroupNumber[RETRY_D1_BLOCK_TYPE]) : (gubHardBitRetryGroupNumber[RETRY_D2D3_BLOCK_TYPE]);

		//gHBRetryQI.
		//UBYTE ubBQIndex;
		for (ubBQIndex = 0; ubBQIndex < (RETRY_QUEUE_MAX_COUNT - 1); ubBQIndex++) {
			if ( ubBQIndex != 0 ) {
				gHBRetryQI[ubi].HBRetryQueue[ubBQIndex].ubPrevious = (ubBQIndex - 1);
			}
			gHBRetryQI[ubi].HBRetryQueue[ubBQIndex].ubNext = (ubBQIndex + 1);
			gHBRetryQI[ubi].HBRetryQueue[ubBQIndex].ubRetryIndex = ubBQIndex;
			//gHBRetryQI[ubi].HBRetryQueue[ubBQIndex].uwRetryCount = 0x00;
		}
		gHBRetryQI[ubi].HBRetryQueue[0].ubPrevious = 0xFF;
		gHBRetryQI[ubi].HBRetryQueue[(RETRY_QUEUE_MAX_COUNT - 2)].ubNext = 0xFF;
	}

	///Enable Thermosensor
	*((volatile unsigned char*)0x04000310) |= 0x80;
	*((volatile unsigned char*)0x04000310) &= 0xBF; /// enable thermometer

#endif
#if ENABLE_RESET_RETRY_LEVEL
	gulKeepRetryLevelCE_BITMAP = 0x0;
#if (!TLC)
	gul26CMDStartTime = 0;
	gub26CMDflag = 0;
	gulMLCHBPassCnt = 0;
#endif

#endif
}

#if (!BURNER)
void CalculateLastDataPlaneIndex(UBYTE btFastPage) {
	UBYTE ubi = 0;
	U32 ulj = 0;
	U32 ulLastPlaneIndexTmp = 0;

	//***************************************************
	//	Set Last Data Plane Index
	//***************************************************
	for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
		UWORD uwPlaneOffset, uwPageIndex, uwPageIndex2;
		UBYTE ubGroupIndex;
		UBYTE ubHit = 0;

		//for 126 + 2, empty's group
		ulj = btFastPage ? (RS.ulLastPlaneIndexFastPage[ubi]) : (RS.ulLastPlaneIndex[ubi]);
		if (ulj == RS_DEFAULT_PLANE_IDX) {
			ulLastPlaneIndexTmp = RS_DEFAULT_PLANE_IDX;
			goto Mark_Set_Value;
		}
		// generate planeoffset and groupidx of first RS plane
		ulj = btFastPage ? (RS.ulLastPlaneIndexFastPage[ubi] - 2) : (RS.ulLastPlaneIndex[ubi] - 2);
		Generate_RS_MAP_Index_by_Plane(ulj);
		uwPlaneOffset = RS.uwPlaneOffset;
		ubGroupIndex = RS.ubGroupIndex;
		//uwPageIndex = RS.uwPageIndex;
		uwPageIndex = ulj / (RS.uwPlanesPerGroup * RS_GROUP_NUM);

		// Dummy in the end OR Nature RS encode in the end
		// Set an extremly large value so that RSlastpage won't be set in ftl level
		if (uwPlaneOffset == 0) {
			ulLastPlaneIndexTmp = RS_DEFAULT_PLANE_IDX;
			goto Mark_Set_Value;
		}
		else if (uwPlaneOffset == (PARITY_DATA_PAGE_NUM - 2)) {
			ulLastPlaneIndexTmp = RS_DEFAULT_PLANE_IDX;
			goto Mark_Set_Value;
		}
		else {
			ulLastPlaneIndexTmp = 0;
		}

		// set ulj as default last data plane idx
		ulj--;

		// loop until real last data plane idx found
		while (ulj > 0) {

			Generate_RS_MAP_Index_by_Plane(ulj);
			uwPageIndex2 = ulj / (RS.uwPlanesPerGroup * RS_GROUP_NUM);
			if ((RS.ubGroupIndex == ubGroupIndex) && (RS.uwPlaneOffset == (uwPlaneOffset - 1))) {
				ulLastPlaneIndexTmp = ulj;
				ubHit = 1;
				break;
			}

			ulj--;
		}
		M_AssertCriticalError((ubHit == 0), VT_ULTRAMLC_BUILD_LASTDATAPLANE_00);

	Mark_Set_Value:
		if (btFastPage) {
			RS.ulLastDataPlaneIndexFastPage[ubi] = ulLastPlaneIndexTmp;
		}
		else {
			RS.ulLastDataPlaneIndex[ubi] = ulLastPlaneIndexTmp;
		}

		M_AssertCriticalError((RS.ulLastDataPlaneIndex[ubi] == 0), VT_ULTRAMLC_BUILD_LASTDATAPLANE_01);
	}
}
#endif

void InitFTLVariable(void) {
#if(!RDT)  // Mask the function inside

	M_CheckRDTModeToDisableFunction();
	U32 ulAlignmentIndex;
	U32 ulTotalTableNum;
	U32 ulTest, ulTemp;
	U32 ulMaxTRIMSize;

	if (BURNER && (gubCENumber == 0)) {
		return;
	}

	if (gulDiskSizeInMB == 0) {
		if (TLC) {
#if ENABLE_ODD_CE
			ulTest = (gulTotalPhyUnitNum * guwPagesPerUnit * (guwSectorsPerPage / 2)) / 1024;
			gulDiskSizeInMB = ulTest / (gubCENumber * gubDieNumber * 1024) / 8 * 8;
			gulDiskSizeInMB = gulDiskSizeInMB * gubCENumber * gubDieNumber * 1024;
#else
			ulTest = flaCheckLogValue(gulTotalPhyUnitNum * guwPagesPerUnit); /* Total disk's page */
			ulTemp = ulTest + (U32)gubSectorsPerPageLog - 11U;		/* (ulTest * sectors/page) / SECTORS_PER_1MB */
			gulDiskSizeInMB = ((U32)(BIT0 << ulTemp));
#endif

#if B0KB
			gulDiskSizeInMB = 48 * gubCENumber * gubDieNumber * 1024;

#endif
		}
		else {
			if (ENABLE_SANDISK_1ZNM_SPF) {
				gulDiskSizeInMB = (gulTotalPhyUnitNum / 1024) * 1024 * guwPagesPerUnit * (guwSectorsPerPage / 2) / 1024;
			}
			else {
				ulTest = flaCheckLogValue(gulTotalPhyUnitNum);
				ulTemp = ulTest + gubSectorsPerPageLog  - 10;
				ulTemp = ((U32)(BIT0 << ulTemp));
				ulTemp = ulTemp * guwPagesPerUnit;
				gulDiskSizeInMB = ulTemp / 2;
				gulDiskSizeInMB = (gulDiskSizeInMB / 1024) * 1024;
			}
		}
		//gulDiskSizeInMB = gulDiskSizeInMB - 8 * 1024; //大部分不可以開全容 先降容
		if (ENABLE_DEBUG_SET_SSD_SIZE) {
			M_AssertError(gulDiskSizeInMB < SET_SSD_SIZE);
			gulDiskSizeInMB = SET_SSD_SIZE;
		}
	}
	if (gulNativeSize == 0) {
		gulNativeSize = ((21168 * 1024) + ((ULLONG)(1953504) * gulDiskSizeInMB)) / 1024;        // set capacity
	}

	UBYTE ubPercentage;
	ubPercentage = (ULLONG)(gulNativeSize) * 100 / (((21168 * 1024) + ((ULLONG)(1953504) * gulDiskSizeInMB)) / 1024);

	if (ubPercentage < 27) {  // 1/4   25+1%
		gubSelDieNumber = gubDieNumber / 4;
		gubSelTotalGroups = gubTotalGroups / 4;
	}
	else if (ubPercentage < 52) { // 1/2   50+1%
		gubSelDieNumber = gubDieNumber / 2;
		gubSelTotalGroups = gubTotalGroups / 2;
	}
	if (gubSelDieNumber == 0) {
		gubSelDieNumber = 1;
	}
	if (gubSelTotalGroups == 0) {
		gubSelTotalGroups = 1;
	}

	gubSelTotalGroupsMask = gubSelTotalGroups - 1;
	gubSelTotalGroupsLog = flaCheckLogValue((U32)gubSelTotalGroups);


	//----->  adjust for write protect
	//ULONG ulDiskSizeInMBbeforeReduce;
	//ULONG ulTotalUserUnitNumbeforeReduce;
	ulTest = gulDiskSizeInMB;      //DiskSizeInMB before reduce
	ulTemp = ((21168 * 1024) + ((ULLONG)(1953504) * ulTest)) / 1024;

	ulTest = (ulTemp / gulSectorsPerUnit);     //TotalUserUnitNum before reduce
	if (ulTemp % gulSectorsPerUnit) {
		ulTest++;
	}
	//<-----

#if 0
	if (gulReducedDiskSizeInLBA) {  // adjust SSD Size through MP
		/*long long SSDsize_LBA = gulReducedDiskSizeInLBA;
		short GB = ((SSDsize_LBA - 97696368 ) / 1953504 ) + 50;
		gulDiskSizeInMB = (ULONG)GB * 1024;*/

		gulDiskSizeInMB = gulReducedDiskSizeInLBA / (2 * 1024);

		//gulDiskSizeInMB = DISKSIZEINMB;
	}
#endif

#if (!(Hynix3DV6 || Hynix3DV5 || YMTC_JGS ||MicronFlashOnly || TLC_BICS2 || ENABLE_PFAIL))
	gubLZSS_RESOLUTION = LZSS_RESOLUTION;
#endif
#if FixShallowErase
	gubSkipProgramFail = 0;
#endif
	if (gulDiskSizeInMB > (256 * 1024)) {
		gubL2P_FORMAT_MODE = 1;
	}
	else {
		gubL2P_FORMAT_MODE = 0;
	}
	if (gubL2P_FORMAT_MODE) {
		gubZByteNumBitAddr = 28;
		gubZBytePer4kEntry = 3;
		gulTable_Invalid = 0xfffffff;
		gulTable_Parity = 0xffffffe;
		gulTable_Dummy = 0xffffffd;
		gulMask_NoValidData = 0xffffff0;  // 表示Invalid, Dummy or Parity
		gulTable_Empty = 0xffffff0;// 此entry無有效資料, 純粹為了align 32B, 512B
		if (gubLZSS_RESOLUTION < 1) {
			gubLZSS_RESOLUTION = 1;
			SYS0B[SYS0B_CR_LZSS_CFG] = gubLZSS_RESOLUTION;
		}
#if TLC
		if ((gubLZSS_RESOLUTION < 2) && (gubPlanesPerPage >= 64)) { // TLC為384 page, 必須再提高壓縮單位
			gubLZSS_RESOLUTION = 2;
			gubMaxZipRate = 2;
			SYS0B[SYS0B_CR_LZSS_CFG] = gubLZSS_RESOLUTION;
			gulMaxL4kEntrysPerUnit = gul4kEntrysPerUnit * gubMaxZipRate;
			gulMaxL4kEntrysPerUnitMask = gulMaxL4kEntrysPerUnit - 1;
			gubMaxL4kEntrysPerUnitLog = flaCheckLogValue((U32)gulMaxL4kEntrysPerUnit);
		}
#endif
	}
	else {
		gubZByteNumBitAddr = 27;
		gubZBytePer4kEntry = 7;
		gulTable_Invalid = 0x3ffffff;
		gulTable_Parity = 0x3fffffe;
		gulTable_Dummy = 0x3fffffd;
		gulMask_NoValidData = 0x3fffff0;  // 表示Invalid, Dummy or Parity
		gulTable_Empty = 0x3fffff0;// 此entry無有效資料, 純粹為了align 32B, 512B
	}
	ACCB_BaseAddr[ACC_MODE] = gubL2P_FORMAT_MODE;
	ACCB_BaseAddr[ACC_MASK] = gub4kEntrysPerPlaneMask;

	//guwTotalFineUnit = 0;
	gubFlushMode = 0;
	// BG
	//gulBGSLC_StartThreshold = BGSLCPoolCopyThreshold;
	//gulBGSLC_EndThreshold = BGSLCPoolReleaseThreshold;
	//gubBGMLC_StartThreshold = BGMLCCopyThreshold;
	//gubBGMLC_EndThreshold = BGMLCCopyReleaseThreshold;
	//gulBGSLC_Threshold = gulBGSLC_StartThreshold;
	//gubBGMLC_Threshold = gubBGMLC_StartThreshold;
	//
	gubStopFTL = 0;
	gubFTL_Only_HandleBQ = 0;
	gubPartialCleanGRDoing = 0;
	gubPartialCleanP2LDoing = 0;
	gubPartialCloseTargetDoing = 0;
	gubWearLevelingCount = MAX_WearLeveling_Num;
	gubStartWL = 0;
	gubFailHandleRequestUnitNum = 0;
	gubGC_FreeUnit_Threshold = CLOSETARGET_FREEUNIT_THRESHELD;
	gubFlushCacheDoing = 0;
	gubFlushCache_StopBG = 0;
	gulFlushCachePrevGRTablePTR = 0;
	gubSavingEveryThingBeforeReset = 0;
	//For RUT Macro
	guwRUTSizePerCE = (ENABLE_SANDISK_1ZNM_SPF || ((gubPlanesPerPage == 64) && (gubBurstsPerBank == 2))) ? (RUTSizePerCE / 2) : RUTSizePerCE;

#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
	gubReadBunchParityDoing = 0;
	gubBunchRSCorrectFlag = 0;
#if USE_PBUF1_WHEN_SAVE_BUNCH_PARITY==1
	gubSaveBunchParityDoing = 0;
#endif
#endif
	gubForceReadMoveDoing = 0;
	gubReadDisturbNeedWaitTQDone = 0;
	gubReadDisturbStartPollingCMD = 0;

	gubHandleBQStart = 0;
	gubNeedWaitCount = 0;
	gubWaitCountLastBQ = 0xFF;

	gubFQOutOfOrder = 0;
	gubFQOutOfOrderCnt = 0;

	gubBQSerialNum = 0;

	gubPlanesPerVT = (16384 / 512) >> gubSectorsPerPlaneLog;
	gubEnable_VT_Backup = (gubPlanesPerBurst > 1) ? 1 : 0;

	// Disk size相關variable
	// 1 L2P group size = 1 plane
	gulTotalL2PGroupCnt = (gulNativeSize / 2 / 1024) / (gub4kEntrysPerPlane * 4);
	if ((gulNativeSize) % (gub4kEntrysPerPlane * 4 * 2 * 1024)) {
		gulTotalL2PGroupCnt++;
	}
	guwL2PEntrysPerL2PGroup = 1024 << gub4kEntrysPerPlaneLog;
	guwL2PEntrysPerL2PGroupQuarter = guwL2PEntrysPerL2PGroup >> 2;
	gulTableIndex_EC = gulTotalL2PGroupCnt;
	gulTableIndex_VC = gulTotalL2PGroupCnt + 1;
	gulTableIndex_HOST = gulTotalL2PGroupCnt + 2;
	gubHostTable_GroupNum = HOST_TABLE_NUM >> gubSectorsPerPlaneLog;

	gulTotalL2PGroupCntPowerOf2 = gulTotalL2PGroupCnt;
	ulTemp = flaCheckLogValue(gulTotalL2PGroupCntPowerOf2);
	if ((BIT0 << ulTemp) != gulTotalL2PGroupCntPowerOf2) {
		gulTotalL2PGroupCntPowerOf2 = (BIT0 << (ulTemp + 1));
	}

	gubL2PEntrysPerL2PGroupLog = flaCheckLogValue((U32)guwL2PEntrysPerL2PGroup);
	gubL2PEntrysPerL2PGroupQuarterLog = flaCheckLogValue((U32)(guwL2PEntrysPerL2PGroup >> 2));
	guwL2PEntrysPerL2PGroupMask = guwL2PEntrysPerL2PGroup - 1;

	gubTableUnitNum = ((gulTotalL2PGroupCntPowerOf2 * 3) / guwFastPagePlanesPerUnit); // *3: Table usage 33%
	if ((gulTotalL2PGroupCntPowerOf2 * 3) % (guwFastPagePlanesPerUnit)) {
		gubTableUnitNum ++;
	}

	if (gubTableUnitNum < 2) {
		gubTableUnitNum = 2;
	}


	// L2P read write mode相關variable
	gubGetWriteModeDirectly = FALSE;
	gubLockReadMode = LOCK_READ_MODE_NOBODY;
	guwActiveTableNum = L2P_IN_RAM_NUM;
	guwWriteModeTableNum_ReadModeStartIndex = L2P_IN_RAM_W_NUM;
	guwReplaceableWriteModeTableCount = L2P_IN_RAM_W_NUM;
	guwWriteModeTableNum_Max = guwWriteModeTableNum_ReadModeStartIndex;
	guwReadModeTableNum = guwActiveTableNum - guwWriteModeTableNum_ReadModeStartIndex;
	guwLockReadNum = 0;
	gubVCZeroTableIndexListNum = 0;
#if BICS2_GC_TO_D3
	guwLockWriteNum = 0;
	gulGCTableRecordStartIndex = 0;
	gulGCGRSetL2PIgnoreCount = 0;
	gulGCGRSetL2PIndex = 0;
	gubForceD1GC = 0;
#endif

	UBYTE ubi;
	gTempTable.ubLinkNum = 0;
	gTempTable.ubLinkFirst = 0;
	gTempTable.ubLinkLast = (TempModeNum - 1);
	gTempTable.ubLinkLastUse = (TempModeNum - 1);
	gTempTable.ubLinkPtr = 0;
	for (ubi = 0; ubi < TempModeNum; ubi++) {
		gTempTable.gTempTableQueue[ubi].ubNext = ((ubi + 1) == TempModeNum) ? 0 : (ubi + 1);
		gTempTable.gTempTableQueue[ubi].ubPrevious = ((ubi - 1) < 0) ? (TempModeNum - 1) : (ubi - 1);
		gTempTable.gTempTableQueue[ubi].uwTableInRamIndex = guwWaitRamToLoadTable;
	}
	guwL2PEntrysPerL2PTempGroup = guwL2PEntrysPerL2PGroup >> 2;
	gubL2PEntrysPerL2PTempGroupLog = flaCheckLogValue((U32)guwL2PEntrysPerL2PTempGroup);
	guwL2PEntrysPerL2PTempGroupMask = guwL2PEntrysPerL2PTempGroup - 1;
	guwL2PEntrysPerL2PTemp512BGroup = guwL2PEntrysPerL2PGroup >> 5;
	gubL2PEntrysPerL2PTemp512BGroupLog = flaCheckLogValue((U32)guwL2PEntrysPerL2PTemp512BGroup);
	guwL2PEntrysPerL2PTemp512BGroupMask = guwL2PEntrysPerL2PTemp512BGroup - 1;

	// clean GR相關variable
	gubCleanGRinMiddle = 0;
	gulGRTemp4KPTR = 0;
	gulGRMiddleCleanGRL4KPTR = 0;
	gulActiveGRValid4KCount = 0;
	gulMin4kEntrysReserved = gul4kEntrysPerUnit  / 2; //留1.5倍
	gubMaxPlanesPerP2LTable = ((gulMaxL4kEntrysPerUnit * 4) / 512) >> gubSectorsPerPlaneLog;
	if (((gulMaxL4kEntrysPerUnit * 4) / 512)&gubSectorsPerPlaneMask) {
		gubMaxPlanesPerP2LTable++;
	}
	gubPlanesPerTableP2LTable = ((guwFastPagePlanesPerUnit * 2) / 512) >> gubSectorsPerPlaneLog;

	if (((guwFastPagePlanesPerUnit * 2) / 512)&gubSectorsPerPlaneMask) {
		gubPlanesPerTableP2LTable++;
	}
	if (gubPlanesPerTableP2LTable == 0) {
		gubPlanesPerTableP2LTable++;
	}
	U32 ulTable_Header_P2L_Byte = (gulTotalL2PGroupCnt + TableTypeNum_ExceptL2P + gubHostTable_GroupNum) * 4 + ((U32)gubTableUnitNum * guwTableP2LBitMapSize) / 8 + (MAX_TABLEUNIT * 2);

	gubTableHeadInfoPlaneCnt = ulTable_Header_P2L_Byte / (gubSectorsPerPlane * 512);
	if (ulTable_Header_P2L_Byte & ((gubSectorsPerPlane * 512) - 1)) {
		gubTableHeadInfoPlaneCnt++;
	}

	// Trim相關variable
	gubCleanGRInTrimAll = 0;
	gulRemainTrimbit = 0;
#if ENABLE_NEW_TRIMALL
	gubTrimAll = 0;
#endif
#if (!BURNER)
	gulTrimRemainderStartLCA = 0;
	guwTrimRemainder4KNum = 0;
	guwTrimRemainderSectorCnt = 0;
#endif
	ulMaxTRIMSize = (guwL2PEntrysPerL2PGroup / 8) * gulTotalL2PGroupCntPowerOf2;	// 2G: 512bytes per L2PGroup, (4096/8) * 128 = 64k
	if (ulMaxTRIMSize > TRIMSize) {
		ulMaxTRIMSize = TRIMSize;
	}
	guwTrimBytesPerL2PGroup = ulMaxTRIMSize / gulTotalL2PGroupCntPowerOf2;	// 32G: 128Bytes per L2PGroup
	ulTemp = flaCheckLogValue(guwTrimBytesPerL2PGroup);
	if (ENABLE_FORCE_TRIMSIZE_POWEROF2 && ((BIT0 << ulTemp) != guwTrimBytesPerL2PGroup)) {
		guwTrimBytesPerL2PGroup = (BIT0 << ulTemp);
	}
	else {
#if TLC
		guwTrimBytesPerL2PGroup = 1U << ulTemp;
#else
		if (TRIMSize % gulTotalL2PGroupCnt) {
			if (guwTrimBytesPerL2PGroup % 4) {
				guwTrimBytesPerL2PGroup = (guwTrimBytesPerL2PGroup / 4) * 4;
			}
			//guwTrimBytesPerL2PGroup+1;
		}
#endif

		gulTrimSizenoAlign = guwTrimBytesPerL2PGroup * gulTotalL2PGroupCnt;

		//guw4KEntrysPerTrimbit = ((ULLONG)gulDiskSizeInMB * 1024 * 1024 / 8) / (gulTrimSizenoAlign * 8 * 512);	// 32G = 32 SectorsPerbit
		//if (((ULLONG)gulDiskSizeInMB * 1024 * 1024 / 8) % (gulTrimSizenoAlign * 8 * 512)) {
		//	guw4KEntrysPerTrimbit += 1;
		//}
	}

	guw4KEntrysPerTrimbit = guwL2PEntrysPerL2PGroup / (guwTrimBytesPerL2PGroup * 8);
	guw4KEntrysPerTrimbitMask = guw4KEntrysPerTrimbit - 1;
	gub4KEntrysPerTrimbitLog = flaCheckLogValue(guw4KEntrysPerTrimbit);

	gubTrimTableNotCleaned = 0;
	gubPIOTirm = 0;


	// RS相關variable
#if ENABLE_ULTRAMLC
	if (gulPlanesPerUnit <= 256) {
		gubParityDataPageNum = 126 + 2;
		gubRSGroupNum = 1;
	}
	else {
		gubParityDataPageNum = 250 + 2;
		gubRSGroupNum = 3;
	}
#endif

	guwParityPlanesNum = PARITY_DATA_PAGE_NUM; // 250 Dpage + 2 Parity
	if ((B0KB && (gubPlanesPerPage > 12)) || ((!B0KB) && (gubPlanesPerPage > 2))) { // 2: avoid 1 Table + 1 RS, 12: ignore 3CE B0KB
		guwTableParityPlanesNum = gubPlanesPerPage;
	}
	else {
		guwTableParityPlanesNum = PARITY_TABLE_PAGE_NUM_1CH;
	}
	gubTableParityPlanesNumLog = flaCheckLogValue((U32)guwTableParityPlanesNum);
	guwTableParityPlanesNumMax = guwTableParityPlanesNum - 1;
	guwTableRSParityNumPerUnit = ( guwFastPagePlanesPerUnit / guwTableParityPlanesNum );

	gubRSCurrentTag = USERDATA_RSFrameStartIndex;
	gubRSIdleTag = USERDATA_RSFrameStartIndex + USERDATA_RS_Group_Num;
	gubRSShiftTagPhase = 0;
	gubRSCurrentTagCopy = GC_RSFrameStartIndex;
	gubRSIdleTagCopy = GC_RSFrameStartIndex + GC_RS_Group_Num;
	gubRSShiftTagPhaseCopy = 0;
	gubRSCurrentTagCopyUnit = GC_RSFrameStartIndex;
	gubRSIdleTagCopyUnit = GC_RSFrameStartIndex + GC_RS_Group_Num;
	gubRSShiftTagPhaseCopyUnit = 0;

	guwRSPlanesPerPage = gubPlanesPerPage * RS_PAGES_PER_GROUP;
	gubRSPlanesPerPageLog = flaCheckLogValue((U32)guwRSPlanesPerPage);
	guwRSPlanesPerPageMask = guwRSPlanesPerPage - 1;

	for (ubi = 0; ubi < (TOTAL_PARITY_GROUP_NUM); ubi++) {
		guwRSFrameDoneCount[ubi] = 0xFF;
		gubftlRSFrameReuseCount[ubi] = 0;
		gubflaRSFrameReuseCount[ubi] = 0;
		guwParityDepth[ubi] = 0xFFFF;
	}

	for (ubi = 0; ubi < (USERDATA_RS_Group_Num + 1); ubi++) {
		gubForceProgramRSParity[ubi] = 0;
	}

#if (!BURNER)
	/* RS Related, by new calculation of RS tag, Yishion */
	// init guwRSParityCntAdjust[][].
	// guwRSParityCntAdjust is for adjust calculation of RS parity count,
	// when the position of additional 6 RS parity at the last 3 RS group
	// is the same as original position of RS group.
	U32 ulPlaneIndex;
	U32 ulRSGroupNum;
	U32 ulPlaneOffsetInRSFrame;
	U32 ulPlanesPerUnit = gulPlanesPerUnit;
	UBYTE ubFastPage;
	for (ubFastPage = 0; ubFastPage < 2; ++ubFastPage) {
		guwRSParityCntAdjust[ubFastPage][0] = 0;
		guwRSParityCntAdjust[ubFastPage][1] = 0;
		ulPlaneIndex = ulPlanesPerUnit - (guwRSPlanesPerPage * (USERDATA_RS_Group_Num - 1)) - 1;
		ulRSGroupNum =  ulPlaneIndex / (guwRSPlanesPerPage * USERDATA_RS_Group_Num);
		ulPlaneOffsetInRSFrame = ((ulPlaneIndex % guwRSPlanesPerPage) + (ulRSGroupNum * guwRSPlanesPerPage)) % PARITY_DATA_PAGE_NUM;
		if (ulPlaneOffsetInRSFrame != PARITY_DATA_PAGE_NUM - 1) { // match at second last group
			guwRSParityCntAdjust[ubFastPage][0] += RS_PARITY_PLANES;
			guwRSParityCntAdjust[ubFastPage][1] += RS_PARITY_PLANES;
		}
		ulPlaneIndex = ulPlanesPerUnit - guwRSPlanesPerPage - 1;
		ulRSGroupNum =  ulPlaneIndex / (guwRSPlanesPerPage * USERDATA_RS_Group_Num);
		ulPlaneOffsetInRSFrame = ((ulPlaneIndex % guwRSPlanesPerPage) + (ulRSGroupNum * guwRSPlanesPerPage)) % PARITY_DATA_PAGE_NUM;
		if (ulPlaneOffsetInRSFrame != PARITY_DATA_PAGE_NUM - 1) { // match at last group
			guwRSParityCntAdjust[ubFastPage][1] += RS_PARITY_PLANES;
		}
		ulPlanesPerUnit = guwFastPagePlanesPerUnit;
	}


	UBYTE ubRSTag[RS_GROUP_NUM];
	UBYTE ubRSTagIdle;
	U32 ulj, ulLinkIndex;
	UBYTE ubtmp;

	RS.ubPlanesPerP2LRS = 0;
	RS.ubDummyPlanes = 0;
	memset(RS.ubTagMAP, 0, RS_GROUP_NUM * RS_CHANGE_TAG_COUNT_PER_GROUP);

	for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
		ubRSTag[ubi] = ubi + 1; // for first round
		for (ulj = 0; ulj < RS_MAX_LINK_COUNT_PER_GROUP; ulj++) {
			RS_FLAFAIL.ulPageLinkList[ubi][ulj] = 0xFFFFFFFF;
		}
	}

	ubRSTagIdle = 0; // for first round

#if ENABLE_ULTRAMLC
	if (gulPlanesPerUnit <= 256) {
		RS.ubPagesPerGroup = PARITY_DATA_PAGE_NUM / gubPlanesPerPage;

		if (PARITY_DATA_PAGE_NUM % gubPlanesPerPage) {
			RS.ubPagesPerGroup++;
		}
	}
	else {
		RS.ubPagesPerGroup = RS_PAGES_PER_GROUP;
	}
#else
	RS.ubPagesPerGroup = RS_PAGES_PER_GROUP;
#endif

	RS.uwPlanesPerGroup = gubPlanesPerPage * RS.ubPagesPerGroup;

	if (RS.uwPlanesPerGroup > PARITY_DATA_PAGE_NUM) {
		RS.uwPlanesPerGroup = PARITY_DATA_PAGE_NUM;
	}
	RS.ubIsParity = 0;

	//*******************
	//	Set RS Tag Map
	//*******************
	for (ulj = 0; ulj < gulPlanesPerUnit; ulj++) {
		Generate_RS_MAP_Index_by_Plane(ulj);
		if (RS.uwPlaneOffset == 0) {
			ubtmp = ubRSTag[RS.ubGroupIndex];
			ubRSTag[RS.ubGroupIndex] = ubRSTagIdle;
			ubRSTagIdle = ubtmp;
			M_AssertError(RS.ubTagIndex >= RS_CHANGE_TAG_COUNT_PER_GROUP);
			RS.ubTagMAP[RS.ubGroupIndex][RS.ubTagIndex] = ubRSTag[RS.ubGroupIndex];
			if (ENABLE_DEBUG_RSMAP) {
				UartString("\nSwap Tag from ");
				Uart_Tx_DataHex(ubRSTagIdle);
				UartString(" to ");
				Uart_Tx_DataHex(ubRSTag[RS.ubGroupIndex]);
				UartString(" at ");
				UartWordHex(ulj);
			}
		}
	}

	//***************************************************
	//	Set Last Plane Index for Non-FastPage Group
	//***************************************************
	for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
		RS.ulLastPlaneIndex[ubi] = RS_DEFAULT_PLANE_IDX;
		ubRSTag[ubi] = 0;
	}
	for (ulj = gulPlanesPerUnit; ulj > 0; ulj -= RS.uwPlanesPerGroup) {
		Generate_RS_MAP_Index_by_Plane(ulj - 1);

		// here RS tag use as bitmap for each group
		if (ubRSTag[RS.ubGroupIndex] == 0) {
			// found last plane index of this group
			RS.ulLastPlaneIndex[RS.ubGroupIndex] = ulj;
			RS.ub1stEndLastGroup = RS.ubGroupIndex;
			ubRSTag[RS.ubGroupIndex] = 1;

			// check that all groups found their last plane index
			for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
				if (ubRSTag[ubi] == 0) {
					break;
				}
			}
			if (ubi == RS_GROUP_NUM) {
				// check done
				break;
			}
		}
	}
	//M_AssertError(ulj <= 0);

	CalculateLastDataPlaneIndex(0);

	//**********************************************
	//	Set Last Plane Index for FastPage Groups
	//**********************************************
	for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
		RS.ulLastPlaneIndexFastPage[ubi] = RS_DEFAULT_PLANE_IDX;
		ubRSTag[ubi] = 0;
	}
	for (ulj = guwFastPagePlanesPerUnit; ulj > 0; ulj -= RS.uwPlanesPerGroup) {
		Generate_RS_MAP_Index_by_Plane(ulj - 1);

		// here RS tag use as bitmap for each group
		if (ubRSTag[RS.ubGroupIndex] == 0) {
			// found last plane index of this group
			RS.ulLastPlaneIndexFastPage[RS.ubGroupIndex] = ulj;
			RS.ub1stEndLastGroupFastPage = RS.ubGroupIndex;
			ubRSTag[RS.ubGroupIndex] = 1;

			// check that all groups found their last plane index
			for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
				if (ubRSTag[ubi] == 0) {
					break;
				}
			}
			if (ubi == RS_GROUP_NUM) {
				// check done
				break;
			}
		}
	}
	//M_AssertError(ulj <= 0);

	CalculateLastDataPlaneIndex(1);

	//**********************************
	//	Set RS Group Page Link List
	//**********************************
	for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
		ubRSTag[ubi] = 0;
	}
	for (ulj = 0; ulj < gulPlanesPerUnit; ulj += RS.uwPlanesPerGroup) {
		Generate_RS_MAP_Index_by_Plane(ulj);
		//M_AssertError(ubRSTag[RS.ubGroupIndex] >= RS_MAX_LINK_COUNT_PER_GROUP);
		// here ulPageLinkList use as "link list of used RS page" for each group
		RS_FLAFAIL.ulPageLinkList[RS.ubGroupIndex][ubRSTag[RS.ubGroupIndex]] = RS.uwPageIndex;
		// here ubRSTag use as "number of used RS page" for each group
		ubRSTag[RS.ubGroupIndex]++;
		M_AssertError(ubRSTag[RS.ubGroupIndex] == 0xFF);
	}

	//***********************************************************
	//	Set Last Plane Index for FastPage Groups
	//	 Encode planes number of each group might not equal,
	//	 Find 1stLastGroup by total Encode count & Link list
	//***********************************************************
	ulj = 0xFFFFFFFF;
	ubtmp = 0xFF;
	for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
		// get tag index
		Generate_RS_MAP_Index_by_Plane(RS.ulLastPlaneIndex[ubi] - 1);
		// total encode count / uwPlanesPerGroup = link index
		ulLinkIndex = (RS.ubTagIndex * PARITY_DATA_PAGE_NUM) / RS.uwPlanesPerGroup;
		// link index is start page index of last plane, find the minimun one
		if (ulj > RS_FLAFAIL.ulPageLinkList[ubi][ulLinkIndex]) {
			ulj = RS_FLAFAIL.ulPageLinkList[ubi][ulLinkIndex];
			ubtmp = ubi;
		}
	}
	M_AssertError(ubtmp == 0xFF);
	RS.ub1stStartLastGroup = ubtmp;

	ulj = 0xFFFFFFFF;
	ubtmp = 0xFF;
	for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
		// get tag index
		Generate_RS_MAP_Index_by_Plane(RS.ulLastPlaneIndexFastPage[ubi] - 1);
		// total encode count / uwPlanesPerGroup = link index
		ulLinkIndex = (RS.ubTagIndex * PARITY_DATA_PAGE_NUM) / RS.uwPlanesPerGroup;
		// link index is start page index of last plane, find the minimun one
		if (ulj > RS_FLAFAIL.ulPageLinkList[ubi][ulLinkIndex]) {
			ulj = RS_FLAFAIL.ulPageLinkList[ubi][ulLinkIndex];
			ubtmp = ubi;
		}
	}
	M_AssertError(ubtmp == 0xFF);
	RS.ub1stStartLastGroupFastPage = ubtmp;


	if (ENABLE_DEBUG_RSMAP) {
		UartString("\n1stStartLastGroup: ");
		Uart_Tx_DataHex(RS.ub1stStartLastGroup);
		UartString("\n1stEndLastGroup: ");
		Uart_Tx_DataHex(RS.ub1stEndLastGroup);
		for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
			UartString("\nLastPlane[");
			Uart_Tx_DataHex(ubi);
			UartString("]: ");
			UartLongHex(RS.ulLastPlaneIndex[ubi]);
		}
		UartString("\n1stStartLastGroupFP: ");
		Uart_Tx_DataHex(RS.ub1stStartLastGroupFastPage);
		UartString("\n1stEndLastGroupFP: ");
		Uart_Tx_DataHex(RS.ub1stEndLastGroupFastPage);
		for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
			UartString("\nLastPlaneFP[");
			Uart_Tx_DataHex(ubi);
			UartString("]: ");
			UartLongHex(RS.ulLastPlaneIndexFastPage[ubi]);
		}
		for (ubi = 0; ubi < RS_GROUP_NUM; ubi++) {
			UartString("\nLink[");
			Uart_Tx_DataHex(ubi);
			UartString("]:");
			for (ulj = 0; ulj < ubRSTag[ubi]; ulj++) {
				if ((ulj & 7) == 0) {
					UartString("\n");
				}
				UartLongHex(RS_FLAFAIL.ulPageLinkList[ubi][ulj]);
				UartString(", ");

			}
		}
	}
#endif


#if (BURNER && ENABLE_ULTRAMLC)
	UBYTE ubPagesPerSubGroup, ubGroupIndex;
	UBYTE ubSubGroupsPerGroup; /* How many Sub-Groups per Group */
	UWORD uwPlanesPerGroup; /* How manys Planes per Group */
	guwParityMappingCount = 0;
	guwParityMappingCountSLC = 0;

	if (gulPlanesPerUnit <= 256) {
		ubPagesPerSubGroup = PARITY_DATA_PAGE_NUM / gubPlanesPerPage;

		if (PARITY_DATA_PAGE_NUM % gubPlanesPerPage) {
			ubPagesPerSubGroup++;
		}
	}
	else {
		ubPagesPerSubGroup = RS_PAGES_PER_GROUP;
	}

	for (ubGroupIndex = 0; ubGroupIndex < VAR_RS_GROUP_NUM; ubGroupIndex++) {
		ubSubGroupsPerGroup = (guwPagesPerUnit / ((UWORD)RS_GROUP_NUM * ubPagesPerSubGroup));
		/* Count for remainder of RS Encoded times */
		if (ubGroupIndex < ((guwPagesPerUnit % ((UWORD)RS_GROUP_NUM * ubPagesPerSubGroup)) / ubPagesPerSubGroup)) {
			ubSubGroupsPerGroup++;
		}

		if (ENABLE_DEBUG_RSMAP) {
			UartString("\nGroupIndex: ");
			Uart_Tx_DataHex(ubGroupIndex);
			UartString(", SubGroupsPerGroup: ");
			Uart_Tx_DataHex(ubSubGroupsPerGroup);
		}

		uwPlanesPerGroup = (UWORD)gubPlanesPerPage * ubPagesPerSubGroup * ubSubGroupsPerGroup;
		if (ENABLE_DEBUG_RSMAP) {
			UartString(", uwPlanesPerGroup: ");
			UartWordHex(uwPlanesPerGroup);
		}
		/* How many RS Parity Encoded times per Group */
		guwParityMappingCount = uwPlanesPerGroup / PARITY_DATA_PAGE_NUM;
		if (uwPlanesPerGroup % PARITY_DATA_PAGE_NUM) {
			guwParityMappingCount++;
		}
		if (ENABLE_DEBUG_RSMAP) {
			UartString(", ParityMappingCount: ");
			UartWordHex(guwParityMappingCount);
		}
	}

	guwParityMappingCountSLC = guwParityMappingCount;
#endif

#if (!BURNER)
	guwParityMappingCount = 0;
	guwParityMappingCountSLC = 0;

	for (ulj = 0; ulj < RS_CHANGE_TAG_COUNT; ulj++) {
		if (ulj < (RS_CHANGE_TAG_COUNT / 2)) {
			guwParityMappingTableSLC[ulj] = 0;
		}
		guwParityMappingTable[ulj] = 0;
	}

	for (ulPlaneIndex = 0; ulPlaneIndex < gulPlanesPerUnit; ulPlaneIndex++) {
		Generate_RS_MAP_Index_by_Plane(ulPlaneIndex);
		if ((RS.uwPlaneOffset == (PARITY_DATA_PAGE_NUM - 2)) || (ulPlaneIndex == (RS.ulLastPlaneIndex[RS.ubGroupIndex] - 2))) {
			M_AssertError(guwParityMappingCount >= RS_CHANGE_TAG_COUNT);	// if error, check pre-define MAX_PAGE_PER_BLOCK
			guwParityMappingTable[guwParityMappingCount] = ulPlaneIndex;
			guwParityMappingCount++;
			ulPlaneIndex++;
		}
	}
	for (ulPlaneIndex = 0; ulPlaneIndex < guwFastPagePlanesPerUnit; ulPlaneIndex++) {
		Generate_RS_MAP_Index_by_Plane(ulPlaneIndex);
		if ((RS.uwPlaneOffset == (PARITY_DATA_PAGE_NUM - 2)) || (ulPlaneIndex == (RS.ulLastPlaneIndexFastPage[RS.ubGroupIndex] - 2))) {
			M_AssertError(guwParityMappingCountSLC >= (RS_CHANGE_TAG_COUNT / 2));	// if error, check pre-define MAX_PAGE_PER_BLOCK
			guwParityMappingTableSLC[guwParityMappingCountSLC] = ulPlaneIndex;
			guwParityMappingCountSLC++;
			ulPlaneIndex++;
		}
	}

#if MicronFlashOnly || (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
	gulConsiderP2L = (SupportLargeUnitSize || guoFlashUseType.B.btIntelMicron) ? ((gubMaxPlanesPerP2LTable + gubPlanesPerPage) * 4) : (gubPlanesPerPage * 2);
#else
	// calculate consider p2l (expected max pl2PlanePtr)
	// not consider fastpage
	UWORD uwParityIndex = guwParityMappingCount - 1;
	U32 ulP2LPlanePtrTemp = gulPlanesPerUnit - gubMaxPlanesPerP2LTable;
	while (ulP2LPlanePtrTemp <= (guwParityMappingTable[uwParityIndex] + 1)) {
		ulP2LPlanePtrTemp -= 2;
		if (uwParityIndex == 0) {
			break;
		}
		uwParityIndex--;
	}
	/*************************
	 Checking For below case,
	    DAT DAT DAT DAT
	    DAT DAT RS  RS
	    P2L P2L P2L P2L
	    P2L P2L RS  RS
	*************************/
	if (ulP2LPlanePtrTemp > (guwParityMappingTable[uwParityIndex] + 1)) {
		if ((ulP2LPlanePtrTemp - 1) <= (guwParityMappingTable[uwParityIndex] + 1)) {
			ulP2LPlanePtrTemp -= 2;
		}
	}
	gulConsiderP2L = gulPlanesPerUnit - ulP2LPlanePtrTemp;
	gulConsiderP2L += MAX_ENDING_DUMMY_PER_UNIT;
#endif


#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
	// calculate guwMaxPlanesIncludeParityPerP2LTable
	U32 ulP2LPlanePtr = gulPlanesPerUnit - gubMaxPlanesPerP2LTable;
	UWORD uwParityIndex = guwParityMappingCount - 1;

	guwMaxPlanesIncludeParityPerP2LTable = gubMaxPlanesPerP2LTable;
	while (ulP2LPlanePtr <= (guwParityMappingTable[uwParityIndex] + 1)) {
		ulP2LPlanePtr -= 2;
		guwMaxPlanesIncludeParityPerP2LTable += 2;
		if (uwParityIndex == 0) {
			break;
		}
		uwParityIndex--;
	}
#endif

#endif
	// table not use 255 dpage
	if ((guwTableParityPlanesNum - 1) >= gubPlanesPerTableP2LTable) {
		guwTableP2LPlanePtr = guwFastPagePlanesPerUnit - (gubPlanesPerTableP2LTable + 1) ;
	}
	else {
		guwTableP2LPlanePtr = guwFastPagePlanesPerUnit - (gubPlanesPerTableP2LTable + 2) ;
	}

	//guwTablePlanesPerUnit = guwFastPagePagesPerUnit * (gubPlanesPerPage - 1) - gubTableHeadInfoPlaneCnt - gubPlanesPerTableP2LTable;
	guwTablePlanesPerUnit = guwFastPagePlanesPerUnit - guwTableRSParityNumPerUnit  - gubTableHeadInfoPlaneCnt - gubPlanesPerTableP2LTable;
#if ENABLE_ULTRAMLC
	UBYTE ubMinPlanesPerP2LTable = 0;

	ubMinPlanesPerP2LTable = ((gul4kEntrysPerUnit * 4) / 512) >> gubSectorsPerPlaneLog;
	if (((gul4kEntrysPerUnit * 4) / 512) & gubSectorsPerPlaneMask) {
		ubMinPlanesPerP2LTable++;
	}

	gulDataPlanesPerUnit = gulPlanesPerUnit - ubMinPlanesPerP2LTable - (guwParityMappingCount * RS_PARITY_PLANES);
#else
	gulDataPlanesPerUnit = gulPlanesPerUnit - (gubMaxPlanesPerP2LTable / 4) - ((gulPlanesPerUnit / guwParityPlanesNum + 1) * RS_PARITY_PLANES);
#endif
	gulData4kEntrysPerUnit = (U32)gulDataPlanesPerUnit << gub4kEntrysPerPlaneLog;
	gulDataSectorsPerUnit = gulData4kEntrysPerUnit << SectorsPer4KLog;
	guwPlanesPerGCDisrict = gulPlanesPerUnit / 32;
	guwPlanesPerGCDisrictMask = guwPlanesPerGCDisrict - 1;

	if (guwPlanesPerGCDisrict >= GC_CleanGR_Partial_Doing_Planes_Num) {
		guwSeparatePartsNum = guwPlanesPerGCDisrict / GC_CleanGR_Partial_Doing_Planes_Num;
	}
	else {
		guwSeparatePartsNum = 1;
	}

	gubSeparatePartsNumLog = flaCheckLogValue((U32)guwSeparatePartsNum);
#if TLC
	guwFlushD1SeparatePartsNum = guwSeparatePartsNum;
	guwNormalFlushD1PartialPlaneNum = ((gulDataPlanesPerUnit - guwPlanesPerGCDisrict) / ((guwNum_Wordline * TLC_PROGRAM_SEQ_CNT) / TLC_FLUSH_ORDER_NUM));
	gulLastCleanGRGroupCount = 0xFFFFFFFF;      /* Make sure first GR won't be SLC Pool */
#endif

	if ((gubPlanesPerPage * 2) < GC_CleanGR_InOneTime_Planes_Boundary_Min) {
		// Noted that if using micron L06B with 8 RS Group  for small geometry
		// Last 16 Planes might all be RS planes, magic 16 is not enough then
		guwInOneTimePlanesBoundary = GC_CleanGR_InOneTime_Planes_Boundary_Min;
	}
	else {
		guwInOneTimePlanesBoundary = gubPlanesPerPage * 2;
	}

	gulCleanGRGroupCount = 0;
	gulCleanGCGRGroupCount = 0;
	gulCleanP2LGroupCount = 0;
	ActiveGRTable = gulGRTable;
	guwGRTargetNum = 15;
	guwPartialCleanGRMaxL2PPlaneNum = (gulTotalL2PGroupCnt / guwSeparatePartsNum);//gulTotalL2PGroupCnt >> gubSeparatePartsNumLog;
	if (guwPartialCleanGRMaxL2PPlaneNum < guwWriteModeTableNum_ReadModeStartIndex) {
		guwPartialCleanGRMaxL2PPlaneNum = guwWriteModeTableNum_ReadModeStartIndex;
	}
	guwWaitRamToLoadTable = guwActiveTableNum;

	// Read flow相關variable
	gubWaitingTableBQCnt = 0;
	guwReplaceableTableCount = guwReadModeTableNum;
	gubRBQIIndex = 0;
	gubRBQINumber = 0;
	gubRBQILast = 31;


	for (ubi = 0; ubi < 32; ubi++) {
		gRBQI[ubi].ubNext = ((ubi + 1) == 32) ? 0 : (ubi + 1);
		gRBQI[ubi].ubPrevious = ((ubi - 1) < 0) ? 31 : (ubi - 1);
		gRBQI[ubi].uwTableInRamIndex = guwWaitRamToLoadTable;
		gBQI.BufferQueue[ubi].ubRBQIIndex = 0xFF;
		gBQI.BufferQueue[ubi].ubThisBQisBG = 0;
		gubNowItsYourTernToLoadDataToThis4kBuffer[ubi] = 0;
		gubUseEach4kBufferInThisOrder[ubi] = 0;
	}
#if ENABLE_PFAIL
	gBQI.ubForce_clean_wbq_threshold = gub4kEntrysPerPlane * gubMaxZipRate;
	gBQI.ubLBAOverlap = 0;
#endif
#if (!B0KB)
	/*for (ubi = 0; ubi < 64; ubi++) {
		gubNowItsYourTernToLoadDataToThis4kBuffer[ubi] = 0;
		gubUseEach4kBufferInThisOrder[ubi] = 0;
	}*/
#endif
	// SDR Cache相關variable
	if (ENABLE_DDR_MODE) {
		UWORD uwi;
		for (uwi = 0; uwi < SDRAM_CACHE_4KNUM; uwi++) {
			guwFreeSDRL2PTableLink[uwi] = (uwi + 1) & (SDRAM_CACHE_4KNUM - 1);
		}
		mSE_SV(1, (U32)guwDataCacheL2PTable_L1,  1024 * 1024, 0xff, 0);
		mSE_SV(1, (U32)guwDataCacheL2PTable_L2,  1024 * 1024, 0xff, 0);
		mSE_SV(1, (U32)SDR_DataEntryTable,  8 * SDRAM_CACHE_4KNUM, 0, 0);
		mSE_SV(1, (U32)gubSDRL2PEntryCount,  1024, 0, 0);
	}
	guwSDRFreeLinkHead = 0;
	guwSDRFreeLinkTail = SDRAM_CACHE_4KNUM - 1;
	guwSDRFreeTableBlockNum = SDRAM_CACHE_4KNUM;
	guwSDRBufferPTR = 0;
	guwSRamBufferPTR = BUFFER2_WRITE_START_INDEX;
	guwReadBufferPTR = 0;
	guoFWRead4kBufferFlag = 0;
	gubPreRead = 0;
	gulPreReadTriggerSectorCnt = 0;
	gubClearPreRead = 0;
	gubSeqR = 0;
	gubRBQFull = 0;
	gubPreReadCheckCount = 0;
	gubPreReadPlaneNum = 4;
	gulPreReadSectors = ((U32)gubSectorsPerPlane << flaCheckLogValue((U32)gubPreReadPlaneNum));
	gubLimitPrereadCount = (gubPlanesPerBank < 4) ? 1 : 0; /* If there are 4 planes to distribute read FQ will not halt write BQ. */
#if ENABLE_READ_DISTURB_PREREAD
	gubLimitPrereadCount = 0;
#endif
	guwDataInSDR4kNum = 0;
	guwSDRFlushEnd4kIndex = 0;
	gubDataCacheL2PTablePerBlockEntryNumLog = flaCheckLogValue((U32)BYTE_DATA_CACHE_L2P_TABLE_ENTRY_NUM);
	gulDataCacheL2PTableEntryL2Mask = BYTE_DATA_CACHE_L2P_TABLE_ENTRY_NUM - 1;
	guwDataCacheL2PL1TablePerEntry4kNum = BYTE_DATA_CACHE_L2P_TABLE_ENTRY_NUM;
	gubDataCacheL2PL1TablePerEntry4kNumLog = flaCheckLogValue((U32)guwDataCacheL2PL1TablePerEntry4kNum);
	gubSDRFlushDoing = 0;
	gulWriteCmdsCount = 0;
	gubDDRMode = ENABLE_DDR_MODE;
	gulBuffer2ReadNum = gubDDRMode ? BUFFER2_NUM : BUFFER2_Read_NUM;
	gulBufferr2ReadMask = gulBuffer2ReadNum - 1;
	gulBuffer2Read4KNum = gulBuffer2ReadNum >> SectorsPer4KLog;
	gulBuffer2Read4KMask = gulBuffer2Read4KNum - 1;
	guwFlush4kCount = 0;

	gubLoadAlignDoing = 0;

	gulProgramUserDataCount = 0;
	gubProgramVCTable = 0;
	gubProgramTable_P2L = 0;


	// table memory address相關variable
	gubPlanesPerVCTable = ((MAX_USABLEUNIT * 4) / 512) >> gubSectorsPerPlaneLog;
	if (((MAX_USABLEUNIT * 4) / 512) & gubSectorsPerPlaneMask) {
		gubPlanesPerVCTable++;
	}
	gubPlanesPerECTable = gubPlanesPerVCTable;
	if (gulDiskSizeInMB > (512 * 1024)) {
		gubPlanesPerSeqTable = (gulTotalL2PGroupCnt * 4) / (4096 * gub4kEntrysPerPlane);
		if ((gulTotalL2PGroupCnt * 4) % (4096 * gub4kEntrysPerPlane)) {
			gubPlanesPerSeqTable ++;
		}
	}
	else {
		gubPlanesPerSeqTable = (gulTotalL2PGroupCnt * 8) / (4096 * gub4kEntrysPerPlane);
		if ((gulTotalL2PGroupCnt * 8) % (4096 * gub4kEntrysPerPlane)) {
			gubPlanesPerSeqTable ++;
		}
	}
	ulTotalTableNum = gulTotalL2PGroupCnt + TableTypeNum_ExceptL2P + gubHostTable_GroupNum;
	ulAlignmentIndex = ulTotalTableNum % 8 ? ((ulTotalTableNum / 8 + 1) * 8) : ulTotalTableNum;
	guwTable_VC = (UWORD*)&gulTable_L2P[ulAlignmentIndex];
	gubTable_P2L_BitMap = (UBYTE*)&guwTable_VC[MAX_TABLEUNIT];
	guwActiveL2PTableReadPTR = 0;
	guwActiveL2PTableWritePTR = 0;
	guwWriteModeLastFree = 0;
	guwTable_LastEncodePlane = 0;
	gubReferenceBit = 1;

	gulEntrysInDataInGRTable = (DataInGRSize * 8);
	gubEntrysInDataInGRTableLog = flaCheckLogValue(gulEntrysInDataInGRTable);
	gulEntrysInDataInGRTableMask = (gulEntrysInDataInGRTable - 1);

	gubSkipDataInGRCheck = ENABLE_Skip_DIG_Check; // for read performance

	gulNCQCMDCnt = 0;
	gulNCQWRCmdTriggerCnt = 0;
	gulNCQRDCmdTriggerCnt = 0;
	gubNCQWRCmdTriggerPassCnt = 0;
	gulWRWaitCnt = 0;
	guwNonAlignBufferPTR = 0xFFFF;
	gubMultiTrigTag = 0xFF;
	gubLastDoneTag = 0x3F;
	gubPreReadStartTag = 0xFF;
	gubPreReadTQListFirst = 0;
	gubPreReadTQListLast = 0;

	gubFreeUnitListNum = 0;
	gubMustFreeNum = 0;

	gulNCQCMDDebug = 0;
	gulNCQCMDDebugCount = 0;

	gSwapHostTable.All = 0;

	gubFTLNoWait = 0;

	gul4KRandomCacheRead = 0;
	guw4KRandomCacheReadCount = 0;

	gubKeepRFQ = 0;
	gubKeepRFQL4KNum = 0;
	gulKeepRFQZipMap = 0;


	gubKeepLoadAlignmentBQLink = 0xFF;
	gubKeepLoadAlignmentBQNum = 0;

	gubKeepWBQNum = 0;
	gubKeepWBQSectorCnt = 0;
	gubKeepWBQ4KCnt = 0;
	gulWriteBufferBitMap = 0;
	for (ubi = 0; ubi < 32; ubi++) {
		gulWriteBufferVir4KTable[ubi] = 0xFFFFFFFF;
		gubWriteBufferDataMaskTable[ubi] = 0;
		gubWriteBufferTableHead[ubi] = Default_WB_Index;
		gubWriteBufferTableNext[ubi] = Default_WB_Index;
	}

	gubTimeToDoBG = 0;
#if TLC
	gubStopD1BGVictimSearch = 0;
#endif
	gubEnableBG_Idle = ENABLE_BACKGROUND_IDLE;
	gubBG_Copy_State = 0;
	gubFTL_Stop_BGCopy = 0;

	//gubEnableSpecialData = ENABLE_SPECIALDATA;
	gulReadZeroDataCnt = 0;

	for (ubi = 0; ubi < TOTAL_PARITY_GROUP_NUM; ubi++) {
		gulRSEncodeStartPlaneNo[ubi] = 0xFFFFFFFF;
	}
	for (ubi = 0; ubi < MAX_RS_GROUP_TAG_FAIL_MAP_LENGTH; ubi++) {
		gubRSGroupTagFailMap[ubi] = 0;
	}
	if (ENABLE_RS_NORMAL_MODE && ENABLE_RS_NORMAL_MODE_FPUCOR_BY_FRAME) {
		for (ubi = 0; ubi < RS_MAX_ERROR_COUNT; ubi++) {
			gubRSErrorPlaneUNCFrameMap[ubi] = 0;
		}
	}
#if ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR
	for (ubi = 0; ubi < RS_MAX_ERROR_COUNT; ubi++) {
		guwDebugRSNormalModeErrorPlaneNo[ubi] = 0;
	}
#endif
#if (!BURNER)
	if (ENABLE_RS_NORMAL_MODE) {
		gsRSNormalModeCorBetterMap.uwFUnit = 0xFFFF;
		gsRSNormalModeCorBetterMap.uwRSGroupTag = 0xFFFF;
		for (ubi = 0; ubi < RS_NORMAL_MODE_MAX_FPUCOR_SIZE; ubi++) {
			gsRSNormalModeCorBetterMap.uwRSNormalModeBetterMap[ubi] = 0x0FFF;
		}
	}
#endif
	gubAbortDoneDepth = 0xFF;
	gubAbortDone_Case = 0x5A;
	gubWP_DEBUG = 0;
	gubTimeoutEnterRebuildFQ = 0;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
	gulNotPolTimeoutCnt = 0;
#if FORCE_AUTOPOL_TIMEOUT
	gubDebugAutoPolFPUSeqInUse = 0;
#endif
#endif
	gulReadFail_4KMAP = 0;
	gulReadUNCWaitCount = 0;
	gubUncPtr = 0;
	gulWP_DEBUG_HWgenFail = 0;
	gubRSReadingParityPage = 0;
	gubEnterftlCopyUnit = 0;
	gubRS_FPU_ReadGetLDPC_BCH = 0;
#if ENABLE_DEBUG_RS_VERIFY_DATA
	gulRS_SpareFail_LDPC_TrappingSet_DebugCnt = 0;
#endif
	gubDebugMaxRSErrorNum = 0;
#if (!BURNER)
	guwRUTLogNum = 0;
	guwRUTLogIndex = 0;
	guwRUTSummaryLogNum = 0;
	guwRUTSummaryLogIndex = 0;
	for (ubi = 0; ubi < RUT_LOGINFO_NUM; ubi++) {
		guwRUTSummaryLog[ubi].All = 0xFFFF;
		guwRUTSummaryLog[ubi].B.ubUnitD1Link = Default_D1_Unit;
		guwRUTSummaryLog[ubi].B.btIsD1Unit = 0;	//Default=0 ( 當成D3 )，uwUnit=0xFFFF也當成D3.
		guwRUTSummaryLog[ubi].B.btNeedCopyActiveTableUnit = 0;
	}
	gubDirectHandl_RUTLog = 0;


	if (ENABLE_DEBUG_HW_FORCEREADFAIL || ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_HW_FORCEERASEFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_ERASE_FAIL || ENABLE_DEBUG_D1TOD3_ERR_HANDLE) {
		gubGenReadFailDebug = 0;
		gubGenProgramFailDebug = 0;
		gubGenEraseFailDebug = 0;
		gubContinueFailCnt = 0;
		gulPutUserFreeToFailHandleAreaCnt = 0;
		gulRandomValue = 0;
	}

#if TLC_NES_TEST_SB_FLOW
	gubNESReadIBF = 0;
	guwNESDataLen = 0;
	guwNESFQIndex = 0;
#endif
	gubSpecialUnitReadMoveMap = 0;
	gulDQSREMismatchMap = 0;
	guoCE_FailMap = 0;
#if TLC_BICS2
	guwCE_FailMap2 = 0;
#endif
	gubMPRW = 0;
	gubFlashTypeFail = 0;

	guoTableProgramFail_BM = 0;
	guwTableTargetProgramDonePTR = 0;
	gubSaveParityCntInSaveHeaderInfo = 0;

	if (ENABLE_RUT2DBT) {
		guwModifyListIndex = 0;
	}

	gubIsRebuildP2LTable = 0;
	//gubEnterSwapRUTCnt = 0;
	gubUpdateRUTBeforeSaveVT = 0;
	gubNeedSaveVTMother = 0;

	guwLastPFTableUnit = Default_Unit;
	guwLastPFTableTargetPtr = 0xFFFF;

	gubVTAreaUnitEraseFailCnt = 0;
	gubVTUnitEraseFailCnt = 0;
	gubSystemUnitEraseFailCnt = 0;
	gubFreeUnitNotEnoughCnt = 0;

	gubNeedCheckSeq = 0;

	gubPredictGC_Done = 0;
	gubCloseTargetCnt = 0;
	gubPredictWLCountPerUnit = 0;
	gubPredictCloseTargetCountPerUnit = 0;

	gulFlushD1_NeedToDoMore_Cnt = 0;
	guwPartialBuildGCTableCnt = 0;
	gulP2LGroupCount_Backup = 0;
#endif
	//	if(gulNeededUnitNum==0){
	//		U32 ulUnitSize = (U32)(guwPlanesPerUnit - gubMaxPlanesPerP2LTable - ((guwPlanesPerUnit / guwParityPlanesNum + 1) * 2)) * gubSectorsPerPlane; // sectors
	//		gulNeededUnitNum = ((gulNativeSize * 100) / (ulUnitSize * 99))  + SYSTEM_SETS + 8 + gubTableUnitNum + FreeBlockCountThatNeedCloseTarget; //8=VT(4)+VTchild+RUT+Parity+GR
	//	}
#if TSB_BICS4_SUPPORT
	guwBadBlockBoundary = ((gulTotalPhyUnitNum - gulNeededUnitNum) >> gubSelTotalGroupsLog) + (guoFlashUseType.B.btToshibaBiCs4 ? gubWriteProtect_NUM : WriteProtect_NUM);
#else
	guwBadBlockBoundary = ((gulTotalPhyUnitNum - gulNeededUnitNum) >> gubSelTotalGroupsLog) + WriteProtect_NUM;
#endif
	if (ENABLE_DEBUG_UART_WRITEPROTECT) {
		UartString("\n\rBadBlkBound");
		UartWordHex(guwBadBlockBoundary);
	}
	gubPartialFlushD1Doing = 0;
	gubForceFlushD1ByIgnoreFail = 0;

	guwFlushD1Fun_D3Unit = Default_Unit;

	gubGetTempFailFlag = 0;
#if(!BURNER)
	/// add for Trim CMD use Zero CMD usage, tried by Vic
	gubCalledWithHostCMD = 1;
	/// Trim CMD usage
	gubWaitFTLtoSwapL2P = 0;

	gubSwapRUTState = BYTE_SWAPRUT_NONE;
	gubSwapRUTForceDone = 0;
	guwSwapRUTRecordQueueNum = 0;
	gulSwapRUTPartialPlaneNumber = gubPlanesPerPage;

	guwHotDataSamplingUnit = Default_Unit;
	guwHotDataSamplingRate = HotDataSamplingRate_DefaultValue;
	guwHotDataSamplingCount = 0;
	gubHotDataLevel = HotDataMinLevel;
#endif
	gubWSeq = 0;
	gubLargeSizeCmdCount = 0;
	gubRFQCnt = 0;
#if TLC
	gubRebuildFreeQueueDoing = 0;
#endif

	guwReadCheckCnt = 0;
	gulMaxReadCount = 0;
	gubBusyNow = 0;
	gubReadDisturbMoveLevel = RDML_RESET_READ;//Read Retry 就搬
	gubReadDisturbCheck = 0;
	gubForceReadMove = 0;
	gubRetryCount_RD = 0xFF; //RD
	gulGRTargetPTR_RD = 0;
	gubSwapRUT_RD = 0;
	gubRetryOK_RD = 0;
	gulLastWriteTime = 0;
	gulLastEnterBGTime = 0;
#endif  ///end of RDT mask function
	gubFirstCMDPending = 0;

	//Uart_Tx_Data(((volatile SecureKeyInfo_t *)SRAM_BASE)->Key[0]);
	//Uart_Tx_Data(((volatile SecureKeyInfo_t *)SRAM_BASE)->Key[1]);
	memset(gubBicsRetryBuffer, 0x00, sizeof(gubBicsRetryBuffer));
	btIsVerifyFlushD1Data = 0;
}

void ChangeMediaSize(void) {
	UWORD uwxTmpI;
	U32 ulxTmpCyl;

	gubMaxHead   = 16;
	gubMaxSec  = 63;

	uwxTmpI = gubMaxHead * gubMaxSec;

	ulxTmpCyl = gulMediaSize.All / uwxTmpI;
	guwMaxCyl = (UWORD) ulxTmpCyl;

	gulCurrentSize.All = ulxTmpCyl * gubMaxHead * gubMaxSec;

	/*
	- * very large drives (8GB+) may lie about the number of cylinders
	- * This is a split test for drives 8 Gig and Bigger only.
	+ * The ATA spec tells large drives to return
	+ * C/H/S = 16383/16/63 independent of their size.
	+ * Some drives can be jumpered to use 15 heads instead of 16.
	*/

	// No Use now
	if ( (gubMaxHead == 16) && (gubMaxSec == 63) ) {
		if (ulxTmpCyl > 0x3fff) {
			guwMaxCyl = 0x3FFF;
			gulCurrentSize.All = 0xFBFC10;  // 0x3fff * 16 * 63
		}
	}

	guwCurCyl = guwMaxCyl;
	gubCurHead  = gubMaxHead;
	gubCurSec = gubMaxSec;

	HB[HB_MAX_HEAD] = gubMaxHead;
	HB[HB_MAX_SECTOR] = gubMaxSec;
	HW[HW_MAX_CYLINDER] = guwMaxCyl;
	HL[HL_MAX_LBA] = gulMediaSize.All;
	if (gulMediaSize.All > 0xFBFC10) {
		HL[HL_MAX_LBA_CHS] = 0xFBFC10;
	}
	else {
		HL[HL_MAX_LBA_CHS] = gulMediaSize.All;
	}
}

void Init_Reset() {
	//	M_CheckRDTModeToDisableFunction();
	unsigned char ub_Mode = 0;
	UBYTE ubi;
	UBYTE ubSataGenSupport;
	if (gubHardReset) {

		AtaCfg.ubLastCmd = 0xFF;

		if (gubPowerMode == PWR_SLEEP) { // PM3:PM2
			U32 ulRightNowTimeInMinutes = (U32)GetRTT_m();
			gulSleepTimeInMinutes = ulRightNowTimeInMinutes - gulSleepTimeInMinutes;  //現在時間扣掉開始 Sleep 的時間
			VT->gulPowerOnTimeInMinutes = (VT->gulPowerOnTimeInMinutes + (ulRightNowTimeInMinutes - gulLastSavedTimeInMinutes)) - gulSleepTimeInMinutes;
			gulLastSavedTimeInMinutes = ulRightNowTimeInMinutes;

			gubPowerMode = PWR_STANBY;
		}
		if (IDT.W79 & CHK_BIT6) { // SSP support & enable
			ub_Mode = RESET_HARD_SSP_ENABLE;
		}
		else {
			ub_Mode = RESET_HARD_SSP_DISABLE; //SSP disable
		}
	}
	else if (gubSoftReset) {

		AtaCfg.ubLastCmd = 0xFF;

		if (gubPowerMode == PWR_SLEEP) { // PM3:PM2
			U32 ulRightNowTimeInMinutes = (U32)GetRTT_m();
			gulSleepTimeInMinutes = ulRightNowTimeInMinutes - gulSleepTimeInMinutes;  //現在時間扣掉開始 Sleep 的時間
			VT->gulPowerOnTimeInMinutes = (VT->gulPowerOnTimeInMinutes + (ulRightNowTimeInMinutes - gulLastSavedTimeInMinutes)) - gulSleepTimeInMinutes;
			gulLastSavedTimeInMinutes = ulRightNowTimeInMinutes;

			gubPowerMode = PWR_STANBY;
		}
		if (gubRTD_enable) { //Reverting to Power on default is enable
			ub_Mode = RESET_SOFT_RTD_ENABLE;
		}
		else {
			ub_Mode = RESET_SOFT_RTD_DISABLE;
		}

	}
	else { //Power on reset
		gubPowerMode = PWR_ACTIVE;
		gulSMARTSelfTestCurrentLBA = AtaCfg.ulSMARTSelfTestStartLBA;
		gulSanitizeCurrentLBA = 0;
		AtaCfg.ubLastCmd = 0;

		ub_Mode = RESET_POWER_ON;

		memset(&IDT, 0, sizeof(IDT));

		IDT.W49 = SET_BIT13 | SET_BIT11 | SET_BIT10 | SET_BIT9;

#if !ENABLE_FORCE_L06B_TO_B0KB
		if (gubFormFactor == FORM_FACTOR_CFAST) {
			IDT.W69 |= SET_BIT15;
			IDT.W161 |= SET_BIT15; // W161 is valid
			IDT.W161 |= SET_BIT1; // Support ACTPM
			IDT.W161 |= SET_BIT9; //CFast2.0
			if ( (gubCFastHighTemprature >= 60) /*&& (gubCFastLowTemprature >= 0)*/ ) {
				IDT.W165 |= SET_BIT15; // w165 is valid
				IDT.W165 |= ((((gubCFastHighTemprature - 60) / 5) << 4) & 0xF0); // Bit 7-4  High Temp
				IDT.W165 |= (((gubCFastLowTemprature) / 5) & 0x0F); // BIT 3-0 Low Temp
			}
		}
#endif

		//Use GPIO 6 & 7 to detect SATA Gen Spport
		ubSataGenSupport = ((SYS0L[SYS0L_GPIO_LH] & (BIT6 | BIT7)) >> 6);
		if (ubSataGenSupport == 1) {
			IDT.W76 = SET_BIT10 | SET_BIT1;
		}
		else if (ubSataGenSupport == 2) {
			IDT.W76 = SET_BIT10 | SET_BIT2 | SET_BIT1;//supports Gen2
		}
		else {
			IDT.W76 = SET_BIT10 | SET_BIT3 | SET_BIT2 | SET_BIT1;//supports Gen3
		}

		IDT.W79 = 0;

		IDT.W82 = SET_BIT14 | SET_BIT13 | SET_BIT12 | SET_BIT6 | SET_BIT3;
		IDT.W83 = SET_BIT14 | SET_BIT13 | SET_BIT12;
		IDT.W84 = SET_BIT14 | SET_BIT5;

		if (IdleUnload_Support) {
			IDT.W84 |= SET_BIT13;
		}

		//Bit 14,13,12,10,4,3 of word 85 is a copy of bit of word 82.
		IDT.W85 = AtaCfg.ubSmartState; //smart enable state not change
		//Bit 13,12,11,10,6,2,0 of word 86 is a copy of bit of word 83.
		IDT.W86 = SET_BIT15;
		//Bit 13,8,6,5,1,0      of word 87 is a copy of bit of word 84.
		IDT.W87 = SET_BIT14;

		IDT.W119 = SET_BIT14 ;
		IDT.W120 = SET_BIT14 ;

		if (gubWWNSupport) {
			IDT.W84 |= BIT8; //The 64-bit World wide name is supported
		}

		if (SMART_Support) {
			IDT.W82 |= SET_BIT0;
			{
				IDT.W84 |= SET_BIT0;
				if (gubSMARTExecuteOfflineEnable) {
					IDT.W84 |= SET_BIT1;
				}
			}
		}

		if (DMA_Support) {
			IDT.W49 |= SET_BIT8;

			IDT.W119 |= SET_BIT3;//Support read log DMA ext and write log DMA ext
			IDT.W120 |= SET_BIT3;//Support read log DMA ext and write log DMA ext
			IDT.W69 |= (SET_BIT10 | SET_BIT11); //READ/WRITE BUFFER DMA is supported
		}

		if (gubNCQSupport) {
			IDT.W76 |= SET_BIT8;
			IDT.W76 |= SET_BIT15;//If the NCQ FEATURE SET SUPPORTED bit is cleared to zero, then the READ LOG DMA EXT AS EQUIVALENT TO READ LOG EXT SUPPORTED bitshall be cleared to zero

			if (AutoActivate_Support) {
				IDT.W78 |= SET_BIT2;
			}
		}

		if (gubSecuritySupport) {
			IDT.W82 |= SET_BIT1;
		}

		if (gubDLMCSupport) {
			IDT.W83 |= SET_BIT0;
			if (DLMC_Feature0x03_Support) {
				IDT.W119 |= SET_BIT4;
				IDT.W120 |= SET_BIT4;
			}			
			IDT.W69 |= SET_BIT8;			
		}

		if (DDT_Support) {
			HB[HB_FLAG_CTRL] |= H_DDT_EN;
		}

		if (gubWUNCSupport) {
			IDT.W119 |= SET_BIT2;
			IDT.W120 |= SET_BIT2;
		}

		if (APM_Support) {
			IDT.W83 |= SET_BIT3;
		}
		if (gubLBA48BitCmdSupport) {
			IDT.W83 |= SET_BIT10;
		}

		if (gubDCOSupport) {
			IDT.W83 |= SET_BIT11;
			IDT.W69 |= SET_BIT12;
		}


		if (FUA_Support && DMA_Support) {
			IDT.W84 |= SET_BIT6;
		}

		if (gubHIPMSupport )
		{
			IDT.W76 |= (SET_BIT9 | SET_BIT13);
		}

		if (gubDIPMSupport ) {
			IDT.W78 |= SET_BIT3; // Device supports initiating interface power management
		}
		if (gubAPSSupport ) {
			IDT.W76 |= SET_BIT14;//supports host automatic partial to slumber transitions
		}

		if (PowerDisable_Support) {
			IDT.W78 |= SET_BIT12;
			IDT.W79 &= CLR_BIT10;

			if (POWER_DISABLE_ALWAYS_ENABLE) {
				IDT.W77 |= SET_BIT8;//power disable always enabled
				gubDEVSLPSupport = 0; //DEVICE SLEEP SUPPORTED bit shall be cleared to zero
				IDT.W79 |= SET_BIT10;//power disable enabled
			}
		}

		if (gubDEVSLPSupport) {
			IDT.W77 |= SET_BIT7;//devslp to reduced pwrstate	support
			IDT.W78 |= SET_BIT8;//devslp support
			IDT.W79 &= CLR_BIT8;//As a result of processing a power on reset, the device shall clear the DEVICE SLEEP ENABLED bit to zero
		}

		if (gubAMAXSupport) {
			IDT.W119 |= SET_BIT8;
		}

		if (gubSanitizeSupport) {
			IDT.W59 |= (SET_BIT11 | SET_BIT12);

			if (SANITIZE_ANTIFREEZE_SUPPORT) {
				IDT.W59 |= SET_BIT10;
			}

			if (SANITIZE_CRYPTO_SCRAMBLE_SUPPORT) {
				IDT.W59 |= SET_BIT13;
			}

			if (SANITIZE_OVERWRITE_SUPPORT) {
				IDT.W59 |= SET_BIT14;
			}

			if (SANITIZE_BLOCK_ERASE_SUPPORT) {
				IDT.W59 |= SET_BIT15;
			}

			if (RESTRICTED_SANITIZE_OVERRIDES_SECURITY_ENABLE) {
				AtaCfg.ubSanitizeValue |= SET_RESTRICTED_SANITIZE_OVERRIDES_SECURITY;
			}
		}

		IDT.W168 = (gubFormFactor & 0x7F);
	}

	//------------------------------------------------------------------------------------//


	if (ub_Mode & (RESET_HARD_SSP_DISABLE | RESET_HARD_SSP_ENABLE | RESET_SOFT_RTD_ENABLE | RESET_SOFT_RTD_DISABLE)) {//soft reset/hard reset
		if (gubOfflineMode & SMART_M_OFFLINEROUTINE) { // off-line routine
			AtaCfg.ubOfflineStatus = SMART_STATUS_OFFLINE_ABORTED; //Abort by HardReset/SoftReset
			gubOfflineMode = SMART_M_SAVEATACONFIG;
		}
		else if (gubOfflineMode & (SMART_M_SHORTSELFTEST | SMART_M_EXTENDEDSELFTEST | SMART_M_CONVEYANCESELFTEST | SMART_M_SELECTIVESELFTEST)) { // self-test
			// When bit 3 in the Selective self-test feature flags field is set to one (see A.15), a device shall continue processing
			// the Selective self-test after processing a hardware reset or a software reset .  When bit 3 in the Selective self-test
			// feature flags field is cleared to zero, a device shall abort the Selective self-test during processing a hardware
			// reset or a software reset.

			if ((gubOfflineMode & SMART_M_SELECTIVESELFTEST) && (AtaCfg.gSMARTSelective.uwFeatureFlags & SMART_FeatureFlag_OfflineScanPending)) {
				// But it seems that it's impossible to have a condition that SMART_FeatureFlag_OfflineScanPending = 1 and Selective self-test is processing?
			}
			else {
				AtaCfg.ubSelfTestStatus = SMART_STATUS_SELFTEST_ABORTEDBYRESET; //Abort by HardReset/SoftReset
				SetSmartLog_CmdDone();
				gubOfflineMode = SMART_M_SAVEATACONFIG;
			}

		}
		else if (AtaCfg.gSMARTSelective.ubOffLineDoing) {
			if ((AtaCfg.gSMARTSelective.uwFeatureFlags & SMART_FeatureFlag_OfflineScanPending) == 0) {
				//But it seems that it's impossible to have a condition that SMART_FeatureFlag_OfflineScanPending = 0 and Selective offline scan is processing?

				AtaCfg.gSMARTSelective.ubOffLineDoing = 0;
				SaveAtaConfig();
			}
		}

		//if the device processes a hardware reset or a software reset, then the device shall:
		//1.discard all updated microcode data that has not been saved;
		//2.retain all deferred microcode data;and
		//3.transition to DL1:No DL In Process state.
		DLMC_InitialSetting();
	}

	if (ub_Mode & (RESET_POWER_ON | RESET_HARD_SSP_DISABLE | RESET_HARD_SSP_ENABLE)) {//power-on reset/comreset
		if (ub_Mode & RESET_HARD_SSP_ENABLE) { //ssp enable
			if (gubSanitizeSupport) {
				InitSanitize(0);
			}
		}
		else { //power-on reset/ssp disable
			IDT.W79 &= CLR_BIT8;
			gubDEVSLPEnable = 0;
			*((volatile unsigned short*)0x0400051C) &= 0xFEFF;

			gubRTD_enable = 0;
			if (gubDCOSupport) {
				if (ub_Mode & RESET_POWER_ON) {
					InitDCO(0);
				}
				else {
					InitDCO(1);
				}
			}
			if (gubHPASupport) {
				IDT.W82 |= SET_BIT10;
				if (ub_Mode & RESET_POWER_ON) {
					InitHPA(0);
				}
				else {
					InitHPA(1);
				}
			}
			if (gubSecuritySupport) {
				// initial Security
				InitSec( (ub_Mode & RESET_POWER_ON) && (gubDLMCReboot == 0x01) );
			}
			if (gubSanitizeSupport) {
				InitSanitize(1);
			}
		}

		IDT.W79 &= CLR_BIT2;//The enable/disable state for the auto-activate optimization shall be reset to its default state upon COMRESET
	}

	if (ub_Mode & (RESET_POWER_ON | RESET_HARD_SSP_DISABLE | RESET_SOFT_RTD_ENABLE)) {//Power on reset/COMRESET SSP disable/SRST Revert to power on setting
		gubStandyByTimerState = 0;

		//default set multiple 16 sector
		IDT.W59 &= 0xFE00;
		IDT.W59 |= SET_BIT8;
		IDT.W59 |= 0x0010;
		
		HW[HW_SYS_CTRL] &= 0x8FFF; //clear bit12,13,14
		HW[HW_SYS_CTRL] |= H_MUL_PIO_DRQ_SIZE_8KB;

		if (DMA_Support) {
			//back to default.
			gXferMode.ubDefaultDMAType = gubDefaultDMAType;
			gXferMode.ubDMASelectMode[SEL_MDMA] = gubDefaultDMASelectMode[SEL_MDMA];
			gXferMode.ubDMASelectMode[SEL_UDMA] = gubDefaultDMASelectMode[SEL_UDMA];

			IDT.W63 = gXferMode.ubDMASupportMAP[SEL_MDMA];
			IDT.W88 = gXferMode.ubDMASupportMAP[SEL_UDMA];

			IDT.W63 &= 0x00ff;// clear Multiword DMA modes selected flag
			IDT.W88 &= 0x00ff;// clear Ultra DMA modes selected flag

			if (gXferMode.ubDefaultDMAType == SEL_MDMA) {
				if (AtaCfg.uwDCOState & SET_DCO_MDMA) {
					// support還是全舉起來...
					// identify 那邊看到DCO set會自己&掉support ...
					// 所以這邊設定enable ,至於support還是全開...
					UBYTE ubCheckBit = (BIT0 << gXferMode.ubDMASupportMode[SEL_MDMA]);
					for ( ubi = 0; ubi <= gXferMode.ubDMASupportMode[SEL_MDMA]; ubi++) {
						if ( AtaCfg.IDM_W63 & ubCheckBit) {
							IDT.W63 |= (ubCheckBit << 8);
							break;
						}
						ubCheckBit >>= 1;
					}
				}
				else {
					IDT.W63 |= (BIT8 << gXferMode.ubDMASelectMode[SEL_MDMA]);
				}
			}
			else {
				if (AtaCfg.uwDCOState & SET_DCO_UDMA) {
					UBYTE ubCheckBit = (BIT0 << gXferMode.ubDMASupportMode[SEL_UDMA]);
					for ( ubi = 0; ubi <= gXferMode.ubDMASupportMode[SEL_UDMA]; ubi++) {
						if ( AtaCfg.IDM_W88 & ubCheckBit) {
							IDT.W88 |= (ubCheckBit << 8);
							break;
						}
						ubCheckBit >>= 1;
					}
				}
				else {
					IDT.W88 |= (BIT8 << gXferMode.ubDMASelectMode[SEL_UDMA]);
				}
			}

		}


		if (gubWriteCacheSupport) {
			IDT.W82 |= SET_BIT5;
			IDT.W85 |= SET_BIT5;//enable WR Cache
			gubWriteCacheEnable = 1;
		}

		IDT.W85 |= SET_BIT6;//enable look ahead

		IDT.W86 &= CLR_BIT3;//disable APM
		IDT.W91 = 0;

		if (gubAMAXSupport) {
			if (ub_Mode & RESET_POWER_ON) {
				AtaCfg.ubAmaxState &= ~(SET_AMAX_SET | SET_AMAX_FREEZE);
			}

			if (AtaCfg.ubAmaxState & SET_AMAX_FOREVER) {
				gulMediaSize.All = AtaCfg.ulAmaxSize;
			}
			else {
				gulMediaSize.All = gulNativeSize;
			}
			ChangeMediaSize();
		}
		else {
			//must do InitHPA first, because power cycle may clear HPA state.
			if ((AtaCfg.ubHpaState & (SET_HPASET | SET_HPALOCK)) && (ub_Mode & RESET_SOFT_RTD_ENABLE)) {
				// softreset : no change
			}
			else if ((AtaCfg.ubHpaState & 0x17) == 0x14) { //HL4 : HL4 (Hardware Reset)
				//HL4/HEL4 state, keep volatile Mediasize
				//because when HL4 power cycle, it will transfer to H0 (cannot not enters here)
				//and when HL4 softreset, it will enters upper condition, but not here.
				//so if it enters here, it should be HardReset
			}
			else if (AtaCfg.ubHpaState & SET_HPANVLBA) {//NV LBA valid or
				if (ub_Mode & (RESET_POWER_ON | RESET_HARD_SSP_DISABLE )) { // softreset : no change
					gulMediaSize.All = AtaCfg.ulHpaSize;
				}
			}
			else if (AtaCfg.uwDCOState & SET_DCO_SIZE) {
				gulMediaSize.All = AtaCfg.ulDcoSize;
			}
			else {
				gulMediaSize.All = gulNativeSize;
			}
			ChangeMediaSize();
		}
	}

	if (ub_Mode & (RESET_POWER_ON | RESET_HARD_SSP_DISABLE | RESET_HARD_SSP_ENABLE | RESET_SOFT_RTD_ENABLE)) {//power-on reset/comreset/SRST Revert to power on setting
		if (SSP_Support) {
			//By default, if the device supports software settings preservation the
			//feature is enabled when it processes a power-on reset. The enable/disable state for software settings
			//preservation shall persist across software reset. The enable/disable state for software settings preservation shall
			//be reset to its default state upon COMRESET.
			IDT.W78 |= SET_BIT6; //SSP Support
			IDT.W79 |= SET_BIT6;// If a device supports the SSP feature set, then the feature shall be enabled by default .
		}

		IDT.W79 &= CLR_BIT3;//disable DIPM
		gubDevIPMON = 0;//reset gubDevIPMON with IDT.W79 BIT3 cleared  byHC

		IDT.W79 &= CLR_BIT7;
		gubDIPMP2SEnable = 0;
		HW[HW_PM_CTRL] &=  ~(H_HW_AUTO_DIPM_EN);
		HW[HW_PM_CTRL] &= ~H_REG_APSREQ;

	}

	if (gubOfflineMode & SMART_M_SAVEATACONFIG) {
		SaveAtaConfig();              // power reset???
	}
	gubOfflineMode = 0;
}

int main(void) {
	///W1C wU flag of DEVSLP
Mark_BackUp_LoadCode_Info:
#if TLC
	memcpy(gubAonBufOffset, (void *)0x5c0D5000, sizeof(gubAonBufOffset));
#else
	memcpy(gubAonBufOffset, (void *)0x5c0D5000, 0x50);
#endif

	if ((bSYS0(0x504) & 0x01) == 0x1) {
		bSYS0(0x504) |= 0x01;
	}
Mark_For_Saving_Power:
	//==========1.  Pull GPIO#6 AND #7 down==========
	bSYS0(0x116) &= 0xCF;             //CR_GPIO_PU_X[13:12] = 0
	bSYS0(0x116) |= BIT6;              //CR_GPIO_PD[6] = 1
	bSYS0(0x117) &= 0xCF;             //CR_GPIO_PU_X[15:14] = 0
	bSYS0(0x117) |= BIT6;              //CR_GPIO_PD[7] = 1

	unsigned char ddr_type = (* ( (volatile unsigned char*) 0x04000004) & (_bit5 | _bit4)) >> 4;
	//==========3.  With DDR, pull GPIO#4 OR #5 down==========
	if ((ddr_type == DDR3_400) || (ddr_type == DDR3_533)) {
		if ( (bSYS0(0x114)&BIT1) == 0 ) { //If XGPIO_I[4] is 0, pull down
			bSYS0(0x114) &= 0xCF;             //CR_GPIO_PU_X[9:8] = 0
			bSYS0(0x114) |= BIT6;              //CR_GPIO_PD[4] = 1
		}

		if ( (bSYS0(0x115)&BIT1) == 0 ) { //If XGPIO_I[4] is 0, pull down
			bSYS0(0x115) &= 0xCF;             //CR_GPIO_PU_X[11:10] = 0
			bSYS0(0x115) |= BIT6;              //CR_GPIO_PD[5] = 1
		}
	}
	if (ENABLE_NEW_RTT_INT_INIT) {
		if (!BURNER) {
			dINTERRUPT(INT_EN1) &= ~RTT1_INT_1;   // Disenable RTT1
			dINTERRUPT(INT_STS1) |= RTT1_INT_1;
		}
	}
	else {
	Mark_Initial_RTT1_ISR:
		if (!BURNER) {
			dINTERRUPT(INT_EN1) &= ~RTT1_INT_1;   // Disenable RTT1
			dINTERRUPT(INT_STS1) |= RTT1_INT_1;
			RTT_Set_Limit(1, 1000); // 1ms
			SYS1L[SYS1L_RTT_CTRL] = (SYS_RTTEN | SYS_CNT_CLEAR | RTT_SCALE_1US); //Enable RTT1, Repeat Mode, Scale 1us
			dINTERRUPT(INT_EN1) |= RTT1_INT_1;   // Enable RTT1

#if ENABLE_ULTRAMLC_PFAIL_MEASURE
			RTT_Set_Limit(2, 0xFFFFFFFF);
			SYS1B[SYS1B_RTT2_CTRL] = (SYS_RTTEN | SYS_RTTAURST | SYS_CNT_CLEAR | RTT_SCALE_1US); //Enable RTT2, Repeat Mode, Scale 1us
#endif
			_xtos_set_interrupt_handler(6, RTT_ISR);
			_xtos_ints_on( 1L << (6) );
			_xtos_set_intlevel(0);
			gulOperationTime = 0;
#if ENABLE_PFAIL
			gulPfail_time = 0;
			gubPfail_state = 0;
			gubAbort_CMD = 0;
#endif
#if ENABLE_ULTRAMLC_PFAIL_SIM
			gulLastWriteCmdsCount = gulWriteCmdsCount;
#endif

			gulFW_START_Time = gulOperationTime;
#if ENABLE_DEBUG_WAF_KAO
			memset(&gulZipCount, 0x00, sizeof(gulZipCount));
			gulGetD3SLCPoolUnitCnt = 0;
			gulGetD3SLCTableUnitCnt = 0;
			gulGetD3TLCUnitCnt = 0;
			gulGetD1SLCUnitCnt = 0;
#endif
		}
	}

	//#if(RDT)
#if TLC
	//	memcpy((void*)0x50000000, gubIRAMContent, 0x528);
#else
	//	memcpy((void*)0x50000000, gubIRAMContent, 0x508);
#endif
	//#endif
	tx_kernel_enter();
	return 0;
}
void StackCheck() {
	flaLEDBlink(0);
}
void tx_application_define(void * first_unused_memory) {
	//tx_thread_stack_error_notify(StackCheck);
	// Create the main thread.
#if(RDT)
	tx_thread_create(&Mainthread, "RDT_MainTask", RDT_MainTask, 0, (void*)TxThreadStackArea0, TxMainStackSize, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
#else
	tx_thread_create(&Mainthread, "mainTask", mainTask, 0, (void*)TxThreadStackArea0, TxMainStackSize, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

	tx_thread_create(&thread_1, "ftlTask", ftlTask, 0, (void*)TxThreadStackArea1, TxFTLStackSize, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

	tx_thread_create(&thread_2, "flaTask", flaTask, 0, (void*)TxThreadStackArea2, TxFLAStackSize, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
#endif
}

void InitFPUEntry() {
#if(ENABLE_TLC_PROGRAM_CODEBLOCK || RDT || (!BURNER))

#if (Hynix3DV6 || YMTC_JGS)
	memcpy((void *)(0x50000000 + FPU_ADR), &gubIRAMContent, sizeof(gubIRAMContent));
#else
	memcpy((void *)(0x50000000 + FPU_ADR), gubIRAMContent, sizeof(gubIRAMContent));
#endif


	memcpy((void *)(0x50000000 + FPU_ADR2), &guwFpuEntry, sizeof(guwFpuEntry));
#if TLC

#if (Hynix3DV6 || Hynix3DV5)
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANEA][LOWER]  = RAM_SEQ_PTR16(FPU_C01_AG_C80_A5_DW);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANEB][LOWER]  = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C81_A5_DW);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANEC][LOWER]  = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C81_A5_DW);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANED][LOWER]  = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C81_A5_DW_C22);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANEA][MIDDLE] = RAM_SEQ_PTR16(FPU_C02_AG_C80_A5_DW);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANEB][MIDDLE] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C81_A5_DW);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANEC][MIDDLE] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C81_A5_DW);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANED][MIDDLE] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C81_A5_DW_C22);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANEA][UPPER]  = RAM_SEQ_PTR16(FPU_C03_AG_C80_A5_DW);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANEB][UPPER]  = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C81_A5_DW);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANEC][UPPER]  = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C81_A5_DW);
	guwFourPlaneD3WriteFPUArray[DMA_EN][PLANED][UPPER]  = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C81_A5_DW_C23);

	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANEA][LOWER] = RAM_SEQ_PTR16(FPU_C01_AG_C85_A5_NOP);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANEB][LOWER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C85_A5_NOP);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANEC][LOWER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C85_A5_NOP);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANED][LOWER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C85_A5_NOP_C22);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANEA][MIDDLE] = RAM_SEQ_PTR16(FPU_C02_AG_C85_A5_NOP);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANEB][MIDDLE] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C85_A5_NOP);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANEC][MIDDLE] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C85_A5_NOP);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANED][MIDDLE] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C85_A5_NOP_C22);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANEA][UPPER] = RAM_SEQ_PTR16(FPU_C03_AG_C85_A5_NOP);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANEB][UPPER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C85_A5_NOP);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANEC][UPPER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C85_A5_NOP);
	guwFourPlaneD3WriteFPUArray[DMA_DIS][PLANED][UPPER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C85_A5_NOP_C23);




	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEA][LOWER] = RAM_SEQ_PTR16(FPU_C01_AG_C80_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEB][LOWER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C81_A5_DW_C22);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEA][MIDDLE] = RAM_SEQ_PTR16(FPU_C02_AG_C80_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEB][MIDDLE] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C81_A5_DW_C22);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEA][UPPER] = RAM_SEQ_PTR16(FPU_C03_AG_C80_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEB][UPPER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C81_A5_DW_C23);

	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEA][LOWER] = RAM_SEQ_PTR16(FPU_C01_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEB][LOWER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C01_AG_C85_A5_NOP_C22);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEA][MIDDLE] = RAM_SEQ_PTR16(FPU_C02_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEB][MIDDLE] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C02_AG_C85_A5_NOP_C22);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEA][UPPER] = RAM_SEQ_PTR16(FPU_C03_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEB][UPPER] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_C03_AG_C85_A5_NOP_C23);

	guwOnePlaneD3WriteFPUArray[LOWER]  = RAM_SEQ_PTR16(FPU_C01_AG_C80_A5_DW_C22);
	guwOnePlaneD3WriteFPUArray[MIDDLE] = RAM_SEQ_PTR16(FPU_C02_AG_C80_A5_DW_C22);
	guwOnePlaneD3WriteFPUArray[UPPER]  = RAM_SEQ_PTR16(FPU_C03_AG_C80_A5_DW_C23);

#elif YMTC_JGS

	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEA][LOWER]	= RAM_SEQ_PTR16(FPU_CDF_AG_C80_A6_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEB][LOWER]	= RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C80_A6_DW_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEA][MIDDLE]	= RAM_SEQ_PTR16(FPU_CDF_AG_C80_A6_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEB][MIDDLE]	= RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C80_A6_DW_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEA][UPPER]	= RAM_SEQ_PTR16(FPU_CDF_AG_C80_A6_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][PLANEB][UPPER]	= RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C80_A6_DW_C10);

	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEA][LOWER]	= RAM_SEQ_PTR16(FPU_CDF_AG_C85_A6_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEB][LOWER]	= RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C85_A6_NOP_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEA][MIDDLE] = RAM_SEQ_PTR16(FPU_CDF_AG_C85_A6_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEB][MIDDLE] = RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C85_A6_NOP_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEA][UPPER]	= RAM_SEQ_PTR16(FPU_CDF_AG_C85_A6_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][PLANEB][UPPER]	= RAM_SEQ_PTR16(FPU_C11_POL_MK40_AG_C85_A6_NOP_C10);

	guwOnePlaneD3WriteFPUArray[LOWER]					= RAM_SEQ_PTR16(FPU_CDF_AG_C80_A5_DW_C1A);
	guwOnePlaneD3WriteFPUArray[MIDDLE]					= RAM_SEQ_PTR16(FPU_CDF_AG_C80_A5_DW_C1A);
	guwOnePlaneD3WriteFPUArray[UPPER]					= RAM_SEQ_PTR16(FPU_CDF_AG_C80_A5_DW_C10);


#else
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FIRST_ORDER][PLANEA][LOWER]			= RAM_SEQ_PTR16(FPU_C09_C01_AG_C85_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FIRST_ORDER][PLANEB][LOWER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C01_AG_C85_A5_DW_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FIRST_ORDER][PLANEA][MIDDLE]			= RAM_SEQ_PTR16(FPU_C09_C02_AG_C85_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FIRST_ORDER][PLANEB][MIDDLE]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C02_AG_C85_A5_DW_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FIRST_ORDER][PLANEA][UPPER]			= RAM_SEQ_PTR16(FPU_C09_C03_AG_C85_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FIRST_ORDER][PLANEB][UPPER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C03_AG_C85_A5_DW_C15);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FIRST_ORDER][PLANEB][UPPER_END]		= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C03_AG_C85_A5_DW_C10);

	guwTwoPlaneD3WriteFPUArray[DMA_EN][FOGGY_ORDER][PLANEA][LOWER]			= RAM_SEQ_PTR16(FPU_C0D_C01_AG_C85_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FOGGY_ORDER][PLANEB][LOWER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C01_AG_C85_A5_DW_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FOGGY_ORDER][PLANEA][MIDDLE]			= RAM_SEQ_PTR16(FPU_C0D_C02_AG_C85_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FOGGY_ORDER][PLANEB][MIDDLE]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C02_AG_C85_A5_DW_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FOGGY_ORDER][PLANEA][UPPER]			= RAM_SEQ_PTR16(FPU_C0D_C03_AG_C85_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FOGGY_ORDER][PLANEB][UPPER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_DW_C15);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FOGGY_ORDER][PLANEB][UPPER_END]		= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_DW_C10);

	guwTwoPlaneD3WriteFPUArray[DMA_EN][FINE_ORDER][PLANEA][LOWER]			= RAM_SEQ_PTR16(FPU_NOP_C01_AG_C85_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FINE_ORDER][PLANEB][LOWER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C01_AG_C85_A5_DW_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FINE_ORDER][PLANEA][MIDDLE]			= RAM_SEQ_PTR16(FPU_NOP_C02_AG_C85_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FINE_ORDER][PLANEB][MIDDLE]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C02_AG_C85_A5_DW_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FINE_ORDER][PLANEA][UPPER]			= RAM_SEQ_PTR16(FPU_NOP_C03_AG_C85_A5_DW);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FINE_ORDER][PLANEB][UPPER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_DW_C15);
	guwTwoPlaneD3WriteFPUArray[DMA_EN][FINE_ORDER][PLANEB][UPPER_END]		= RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_DW_C10);

	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FIRST_ORDER][PLANEA][LOWER]			= RAM_SEQ_PTR16(FPU_C09_C01_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FIRST_ORDER][PLANEB][LOWER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C01_AG_C85_A5_NOP_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FIRST_ORDER][PLANEA][MIDDLE]		= RAM_SEQ_PTR16(FPU_C09_C02_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FIRST_ORDER][PLANEB][MIDDLE]		= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C02_AG_C85_A5_NOP_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FIRST_ORDER][PLANEA][UPPER]			= RAM_SEQ_PTR16(FPU_C09_C03_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FIRST_ORDER][PLANEB][UPPER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C03_AG_C85_A5_NOP_C15);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FIRST_ORDER][PLANEB][UPPER_END]		= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C09_C03_AG_C85_A5_NOP_C10);

	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FOGGY_ORDER][PLANEA][LOWER]			= RAM_SEQ_PTR16(FPU_C0D_C01_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FOGGY_ORDER][PLANEB][LOWER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C01_AG_C85_A5_NOP_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FOGGY_ORDER][PLANEA][MIDDLE]		= RAM_SEQ_PTR16(FPU_C0D_C02_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FOGGY_ORDER][PLANEB][MIDDLE]		= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C02_AG_C85_A5_NOP_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FOGGY_ORDER][PLANEA][UPPER]			= RAM_SEQ_PTR16(FPU_C0D_C03_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FOGGY_ORDER][PLANEB][UPPER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_NOP_C15);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FOGGY_ORDER][PLANEB][UPPER_END]		= RAM_SEQ_PTR16(FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_NOP_C10);

	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FINE_ORDER][PLANEA][LOWER]			= RAM_SEQ_PTR16(FPU_NOP_C01_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FINE_ORDER][PLANEB][LOWER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C01_AG_C85_A5_NOP_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FINE_ORDER][PLANEA][MIDDLE]			= RAM_SEQ_PTR16(FPU_NOP_C02_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FINE_ORDER][PLANEB][MIDDLE]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C02_AG_C85_A5_NOP_C1A);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FINE_ORDER][PLANEA][UPPER]			= RAM_SEQ_PTR16(FPU_NOP_C03_AG_C85_A5_NOP);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FINE_ORDER][PLANEB][UPPER]			= RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_NOP_C15);
	guwTwoPlaneD3WriteFPUArray[DMA_DIS][FINE_ORDER][PLANEB][UPPER_END]		= RAM_SEQ_PTR16(FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_NOP_C10);

	guwOnePlaneD3WriteFPUArray[FIRST_ORDER][LOWER]			= RAM_SEQ_PTR16(FPU_C09_C01_AG_C85_A5_DW_C1A);
	guwOnePlaneD3WriteFPUArray[FIRST_ORDER][MIDDLE]			= RAM_SEQ_PTR16(FPU_C09_C02_AG_C85_A5_DW_C1A);
	guwOnePlaneD3WriteFPUArray[FIRST_ORDER][UPPER]			= RAM_SEQ_PTR16(FPU_C09_C03_AG_C85_A5_DW_C15);
	guwOnePlaneD3WriteFPUArray[FIRST_ORDER][UPPER_END]	= RAM_SEQ_PTR16(FPU_C09_C03_AG_C85_A5_DW_C10);

	guwOnePlaneD3WriteFPUArray[FOGGY_ORDER][LOWER]			= RAM_SEQ_PTR16(FPU_C0D_C01_AG_C85_A5_DW_C1A);
	guwOnePlaneD3WriteFPUArray[FOGGY_ORDER][MIDDLE]			= RAM_SEQ_PTR16(FPU_C0D_C02_AG_C85_A5_DW_C1A);
	guwOnePlaneD3WriteFPUArray[FOGGY_ORDER][UPPER]			= RAM_SEQ_PTR16(FPU_C0D_C03_AG_C85_A5_DW_C15);
	guwOnePlaneD3WriteFPUArray[FOGGY_ORDER][UPPER_END]	= RAM_SEQ_PTR16(FPU_C0D_C03_AG_C85_A5_DW_C10);

	guwOnePlaneD3WriteFPUArray[FINE_ORDER][LOWER]			  = RAM_SEQ_PTR16(FPU_NOP_C01_AG_C85_A5_DW_C1A);
	guwOnePlaneD3WriteFPUArray[FINE_ORDER][MIDDLE]			  = RAM_SEQ_PTR16(FPU_NOP_C02_AG_C85_A5_DW_C1A);
	guwOnePlaneD3WriteFPUArray[FINE_ORDER][UPPER]			  = RAM_SEQ_PTR16(FPU_NOP_C03_AG_C85_A5_DW_C15);
	guwOnePlaneD3WriteFPUArray[FINE_ORDER][UPPER_END]	      = RAM_SEQ_PTR16(FPU_NOP_C03_AG_C85_A5_DW_C10);

#endif

#endif
#endif
}


void SetInitialFailID(UWORD uwErrorID, U32 ulErrorInfo) {
	guwErrorID = uwErrorID;
	gulErrorInfo = ulErrorInfo;
}

void SetFailCEtoSystemInfo(UBYTE ubLogical_CEIndex) {
	if (ENABLE_VenderCMD_return_PhysicalCE) {
		UBYTE ubPhysical_CEIndex;
		M_VirtualToPhysicalCE(ubLogical_CEIndex);
		ubPhysical_CEIndex = TranslateLogicalCEtoPhysicalCE(ubLogical_CEIndex);
		/*UartString("\nL:");
		Uart_Tx_DataHex(ubLogical_CEIndex);
		UartString(" P:");
		Uart_Tx_DataHex(ubPhysical_CEIndex);*/
		guoCE_FailMap |= (((ULLONG)BIT0) << ubPhysical_CEIndex);
	}
	else {
		guoCE_FailMap |= (((ULLONG)BIT0) << ubLogical_CEIndex);
	}
}

void mainTask() {
	if (ENABLE_NEW_RTT_INT_INIT) {
		if (!BURNER) {
			RTT_Set_Limit(1, 1000); // 1ms
			SYS1L[SYS1L_RTT_CTRL] = (SYS_RTTEN | SYS_CNT_CLEAR | RTT_SCALE_1US); //Enable RTT1, Repeat Mode, Scale 1us
			dINTERRUPT(INT_EN1) |= RTT1_INT_1;   // Enable RTT1
#if ENABLE_ULTRAMLC_PFAIL_MEASURE
			RTT_Set_Limit(2, 0xFFFFFFFF);
			SYS1B[SYS1B_RTT2_CTRL] = (SYS_RTTEN | SYS_RTTAURST | SYS_CNT_CLEAR | RTT_SCALE_1US); //Enable RTT2, Repeat Mode, Scale 1us
#endif
			_xtos_set_interrupt_handler(6, RTT_ISR);
			_xtos_ints_on( 1L << (6) );
			_xtos_set_intlevel(0);
			gulOperationTime = 0;
			gulFW_START_Time = gulOperationTime;
#if ENABLE_ULTRAMLC_PFAIL_SIM
			gulLastWriteCmdsCount = gulWriteCmdsCount;
#endif
		}
	}
	else {

	}
#if(!RDT)
	U32 ulTime[5] = {0};
	ulTime[0] = gulOperationTime;
#endif

	guwErrorID = 0;
	gulErrorInfo = 0;
#if Hynix
	gubDoFlushD1 = 0;
	gubDaCmdEnable = 0;
	gubDaCmdEraseEnable = 0;
#endif

	SYS0B[0x200] |= 0x3;
	SetQInfo();

	UBYTE ubi;

	if (BURNER || RDT || (ASIC == 0)) {
		gubNormalFWMode = 0;
		for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
			gubFlashMode_CE[ubi] = 0;
			gubIOMode_CE[ubi] = 0;
			gubFlashONFI_CE[ubi] = 0;
		}
	}
	else {
		UWORD uwLoadFWTag = (*((UWORD *)DCCM1_BANK1_STARADDRESS));
		if (uwLoadFWTag == 0x4449) {	//FW都會拉起來
			gubNormalFWMode = 1;
		}
		else {
			gubNormalFWMode = 0;
		}

		if (gubNormalFWMode) {
			for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
				gubFlashMode_CE[ubi] = 0;
				gubIOMode_CE[ubi] = 0;
				gubFlashONFI_CE[ubi] = 0;

				if ((FCTLL[FCTLL_FLH_SET] & FLA_ONFI_MODE) == FLA_ONFI_MODE) {
					gubFlashMode_CE[ubi] |= BIT0;
					gubFlashONFI_CE[ubi] |= BIT0;
				}
				else if (FCTLL[FCTLL_FLH_SET] & FLA_TOGGLE_MODE) {
					gubFlashMode_CE[ubi] |= BIT0;
				}

				if (FCTLL[FCTLL_IO_SET] & DIFFERENTIAL_IO_TYPE) {
					gubIOMode_CE[ubi] |= BIT0;
				}
			}
		}
		else {
			for (ubi = 0; ubi < MAX_CHANNEL; ubi++) {
				gubFlashMode_CE[ubi] = 0;
				gubIOMode_CE[ubi] = 0;
				gubFlashONFI_CE[ubi] = 0;
			}
		}
	}
#if !BURNER
	// power cycling detect
	if (RET_OK == mDMAC_CRC((U32)ReservedData, 512, ReservedData_SEED, 1, (DMA_CRC_CHK | DMA_CRC_LEN_8 | DMA_CRC_DCCM)) ) {

		if (ReservedData->ubScanVTReboot) {
			gubScanVTReboot = ReservedData->ubScanVTReboot;
			gubDLMCReboot = 0x00;
		}
		else {
			gubScanVTReboot = 0x00;
			gubDLMCReboot = 0x01;
			gubSecurityMode = ReservedData->ubSecMode;
		}
		// destory power on data crc
		ReservedData->CRC = 0xAAAAAAAA;
	}
	else {
		gubDLMCReboot = 0x00;
		gubScanVTReboot = 0x00;
		memset(ReservedData, 0x00, sizeof(ReservedData));
	}
#endif
	if (ENABLE_DEBUG_UART_FLH_INITIAL) {
		UartString("\r\nFLH_SET: 0x");
		UartLongHex(FCTLL[FCTLL_FLH_SET]);
		UartString("\r\nIO_SET: 0x");
		UartLongHex(FCTLL[FCTLL_IO_SET]);
	}
#if (!RDT)

#if BURNER
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
	SYS1L[SYS1L_ECC_EN] = 0;
	DRAM_init();
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
	SYS1L[SYS1L_ECC_EN] = 0;

	//U32 counter1;
	//for(counter1=0; counter1<0xFFFFFF; counter1++);
	//mDMAC_SV(1, DDR_BASE , 32*1024*1024, 0, 0, 0, 0, 0, 1);
	if (1) { //gubDram_Type==SDR){
		RAM_Set_ZONE_Region(1, DDR_BASE, 1024);
		RAM_Set_ZONE_Region(2, Z2_DDR_BASE, Z2_Size);
		RAM_Set_ZONE_Region(3, Z3_DDR_BASE, Z3_Size);
	}
	else {
		RAM_Set_ZONE_Region(1, DDR_BASE, 0x100000);
		RAM_Set_ZONE_Region(2, DDR_BASE + 0x100000, 0x5000000 - 0x100000);
		RAM_Set_ZONE_Region(3, DDR_BASE + 0x5000000, 0xb000000);
	}
	RAM_Set_ZONE_Region(4, SZ1_DDR_BASE , 0);
	RAM_Set_ZONE_Region(5, SZ2_DDR_BASE , 0);

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
	memset((void *)DDR_BASE, 0x00, 32 * 1024 * 1024);
	//mSE_SV(4, DDR_BASE, 32*1024*1024, 0, 0);
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
	//SYS1L[SYS1L_ECC_EN] = 0; //disable DDR ECC, E3D, shadow Z1Z2
	SYS1L[SYS1L_ECC_EN] |= (CR_E3D_EN | CR_Z1_ECC_EN | CR_Z2_ECC_EN | CR_Z3_ECC_EN); //enable DDR ECC, E3D, shadow Z1Z2
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

	RTT_Set_Limit(1, 1000); // 1ms
	SYS1L[SYS1L_RTT_CTRL] = (SYS_RTTEN | SYS_CNT_CLEAR | RTT_SCALE_1US); //Enable RTT1, Repeat Mode, Scale 1us
	dINTERRUPT(INT_EN1) |= RTT1_INT_1;   // Enable RTT1

	_xtos_set_interrupt_handler(6, RTT_ISR);
	_xtos_ints_on( 1L << (6) );
	_xtos_set_intlevel(0);
	gulOperationTime = 0;
	gulFW_START_Time = gulOperationTime;

	ulTime[0] = gulOperationTime;
#endif
	U32 ulIdle_Counter;
	U32 ulIdle_Counter2 = 0;
	UBYTE ubIdle2Enable = 0;
Mark_Manual_DRAM_Zone_Setting:
	gubTemp = 0;
	if (gubTemp) {
		//避免之前被改掉... 反正下面會重設
		RAM_Set_ZONE_Region(1, DDR_BASE, DDR_SIZE * 1024 * 1024);
		RAM_Set_ZONE_Region(2, 0x80000000, Z2_Size);
		RAM_Set_ZONE_Region(3, 0xA0000000, Z3_Size);
		mDMAC_SV(1, DDR_BASE, DDR_SIZE * 1024 * 1024, 0, 0, 0, DMA_EN_SETCQ, 0);
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
		SYS1L[SYS1L_ECC_EN] = 0; //disable DDR ECC, E3D, shadow Z1Z2
		RAM_Set_ZONE_Region(1, Z1_DDR_BASE, Z1_Size);
		RAM_Set_ZONE_Region(2, Z2_DDR_BASE, Z2_Size);
		RAM_Set_ZONE_Region(3, Z3_DDR_BASE, Z3_Size);
		SYS1L[SYS1L_ECC_EN] |= (CR_E3D_EN | CR_Z1_ECC_EN | CR_Z2_ECC_EN | CR_Z3_ECC_EN); //enable DDR ECC, E3D
		while (1);
	}
	else {
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
		if (!((guwErrorID & BIT15) && BURNER)) {
			for (ubi = 0; ubi < 8; ubi++) {
				if (RAML[RAML_DDR3_ECC_CTRL] & (BIT0 << ubi)) {
					//有哪種int就清哪種int
					RAML[RAML_DDR3_ECC_CTRL] |= (BIT0 << (ubi + 16));
					RAML[RAML_DDR3_ECC_CTRL] &= (~(BIT0 << (ubi + 16)));
				}
			}

			Enable_DDR_Interrupt();
		}
		// FLH reset
		SYSL[SYS0L_RST_CTRL] &= (~CR_RST_N_FLH);
		SYSL[SYS0L_RST_CTRL] |= (CR_RST_N_FLH);
		/*
		SATA reset --
		Burner 不做
		        Vender Reset不作(因為沒必要,且會斷link), 但是萬一做了其實也沒差, 就多打一次Cominit, 導致host會多下一次comreset而已
		        */
		if ((!BURNER) && (!((gubCmdCode == 0x31) && (HB[HB_FEATURE] == 0x1))) && ((SYSL[SYS0L_SIM_CTRL0] & SIMREG_CMD_D2H) == 0) && (SYSB[0x44] != SIMREG_DEVSLP_LOAD_CODE) ) {
			SYSL[SYS0L_RST_CTRL] &= (~CR_RST_N_SATA);
			SYSL[SYS0L_RST_CTRL] |= (CR_RST_N_SATA);
		}
	}


Mark_Init_DRAM: // 先推, 不等CQ


#if UFS || (YMTC && (!ENABLE_TLC_PROGRAM_CODEBLOCK))
	if (!BURNER) {
		mDMAC_SV(1, BUFFER2_START,  (128 * 1024), 0, 0, 0, 0, 0);				// For SATA send zero bug ??
	}
#else
	mDMAC_SV(1, BUFFER2_START,  (128 * 1024), 0, 0, 0, 0, 0);				// For SATA send zero bug ??
#endif
#if (!BURNER)
#if MicronFlashOnly
	mDMAC_SV(4, (U32)&guwRUTBASE[0], RUTSize , 0, 0, 0, 0, 0);
	//mDMAC_SV(4, (U32)gubZ2Buffer, 64 * 1024, 0, 0, 0, 0, 0); 				// Z2  buffer 64KB

	mDMAC_SV(4, (U32)BUFFER3_BASE, (32 + 4 + 16) * 1024, 0, 0, 0, 0, 0); 	// Host Spare (+16k for cache)
	//test
	mDMAC_SV(4, (U32)(TableZoneBase + 0x4AF000 + 32 * 1024), (32 + 4 + 16) * 1024, 0, 0, 0, 0, 0);
	mDMAC_SV(4, (U32)guwTableP2L_GC, 64 * 1024, 0, 0, 0, 0, 0);         // Table_P2L_GC  Host Spare (+16k for cache)
	mDMAC_SV(4, (U32)gubHostTable, (32 + 4 + 16) * 1024, 0, 0, 0, 0, 0);      // Table_P2L_GC  Host Spare (+16k for cache)
	mDMAC_SV(4, (U32)gubZ2Buffer, (16 + 2) * 1024, 0, 0, 0, 0, 0);                                 // z2 RS decode
#if B0KB
	mDMAC_SV(4, (U32)CorrectRSSpareStart, 2 * 1024, 0, 0, 0, 0, 0);
#endif
#else

	mDMAC_SV(4, (U32)&guwRUTBASE[0], 544 * 1024, 0, 0, 0, 0, 0);    //RUT已轉成1維, 這邊要記得取址
	mDMAC_SV(4, (U32)guwTableP2L_GC, (32 + 32 + 4 + 16) * 1024, 0, 0, 0, 0, 0); 	// Table_P2L_GC  Host Spare (+16k for cache)
	mDMAC_SV(4, (U32)gubZ2Buffer, (16 + 2) * 1024, 0, 0, 0, 0, 0);					// z2 RS decode
	mDMAC_SV(4, (U32)gubCopyBuffer2, 1024 * 1024, 0, 0, 0, 0, 0);					// Copy Buffer
#endif
#else
	mDMAC_SV(4, TableZoneBase, TableZoneSize, 0, 0, 0, 0, 0);
	mDMAC_SV(1, Z3_DDR_BASE,  ((Z3_Size / 520) * 512), 0, 0, 0, DMA_EN_SETCQ, 0);
	memcpy((void *)&gulVenderSMARTBuffer[0], (void*)0x50000000, FPU_Length);
#endif
	ulTime[1] = gulOperationTime;
	gFQI.ubFQLinkNumber = 0;
	gBQI.ubLinkNum = 0;

	Enable_Stack_Monitor_Interrupt();
	Uart_InitBaudRate(30000000, 115200);

	if (Enable_Parity_Error) {
		if (!((guwErrorID & BIT15) && BURNER)) {
			Enable_ParityErr_Interrupt();
		}
	}
	Enable_VDT_Interrupt();

	gubPreformat = 0;
	SYS_Interrupt_Init();
	InitCommonRegister();
	CheckFlashType();
#if(BURNER)
#if((!(MicronFlashOnly || TLC || Hynix || Sandisk))&&BURNER)
	if (guoFlashDefaultType.B.btNoSupport == TRUE) {
		InitVariable(); // for 0MB test
		guoFlashUseType = guoFlashDefaultType;
		goto Mark_SATA_Link;
	}
	if (!(guoFlashDefaultType.B.btToshiba )) {
		InitVariable();
		flaSetFlashUseType(1);
		goto Mark_SATA_Link;
	}
#else
	if (guoFlashDefaultType.B.btNoSupport == TRUE) {

		guoFlashUseType = guoFlashDefaultType;
		goto Mark_SATA_Link;
	}
#endif
#endif
#if Hynix
	FlaIssueHynixDaCmd(FALL, gulCeEnBitMap, 0xBE);
#endif
	InitVariable();
	InitFlashRegister(1); // Set MTP and LLR table in flash IRAM
	mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 1);//強迫DMAC 全部做完, Must switch task to initial fla before setting FQ.
#if TLC
	if (TLC) {
		FCTLL[FCTLL_CHNL_SET] |= FLA_EDO_MODE;
		FCTLL[FCTLL_CHNL_SET + FIP1] |= FLA_EDO_MODE;
	}
#endif
	LoadSystemBlock();
	InitFTLVariable();
#if TLC_BICS2
#if (!BURNER)
	flaBics4_Revision_Identify();
#endif
	flaDisable_IPR();  //CM part 512Gb flash burner & FW disable IPR
#endif
#if (TSB_BICS4_SUPPORT && (!BURNER))
	if (guoFlashUseType.B.btToshiba && guoFlashUseType.B.btToshibaBiCs4) {

		UBYTE ubCEIndex, ubChannelIndex, ubDie;

		for (ubCEIndex = 0; ubCEIndex < gubMTQTotalPhyCE; ubCEIndex++) {
			ubChannelIndex = ubCEIndex % gubPlanesPerBurst;       //logical
			FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
			FCONL[FCONL_FCE_SET] = (ubChannelIndex * 8) + (ubCEIndex / gubPlanesPerBurst); //logical
			gFREG = (volatile REG_t*)(U32)(Flash_Reg_Base + ((ubChannelIndex & gubPlanesPerBurstMask) * 0x200));

			for (ubDie = 0; ubDie < gubDieNumber * gubExpandCE_Ratio; ubDie++) {

				memcpy((void *)&gubBicsRetryBuffer[(ubCEIndex * gubDieNumber + ubDie) * 8 + 0], (void*)&gubHardBitRetryParameterTable[gubHardBitRetryD2D3GroupBase + 0], 7);

				guliFSA[0] = (ubDie << (gubDieBlockShiftLog + 16));
				flaReadFailRetryToshibaBiCs2Flow(0, 0, NULL);
			}
		}
		guliFSA[0] = 0;
		FCONL[FCONL_FCE_ENB] = 0x00000000;  // Disable   0~31  CEs
	}
#endif

#if TLC_NES_TEST
#if MicronFlashOnly || YMTC_JGS
	if (gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
	if (gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
	{
		FCONL[FCONL_TMR_CTL] &= ~(TIMER_CPU_INTR_EN | TIMER_INTR_EN);
	}
#endif
#if Hynix
#if (!BURNER)
	if (gubPreformat == 0x00) {
		UBYTE ubSystemUnit, ubSystemBurst;
		ubSystemUnit = guwSystemBlock[0] >> gubBurstsPerBankLog;
		ubSystemBurst = guwSystemBlock[0] & gubBurstsPerBankMask;

		if (RET_OK == ftlReadSinglePlane(ubSystemUnit, 3, CE0, (U32)BUFB_BASE, MARK_SYS_HYNIX_RRT, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
			//Uart_Printf("check %l\r\n",BUFB_BASE);
			memcpy(&gsHynixRetryInfo, BUFB_BASE, sizeof(HynixRetryInfo));//32B
			gsHynixRetryInfo.ulSlcRetryTblBase = (U32) &gubSlcRetryTable;
			gsHynixRetryInfo.ulXlcRetryTblBase = (U32) &gubXlcRetryTable;
			memcpy(&gsHynixRetryMeta, BUFB_BASE + 256, sizeof(gsHynixRetryMeta));//512B
			memcpy(&gubSlcRetryTable, BUFB_BASE + 1024, sizeof(gubSlcRetryTable));//2144B
			memcpy(&gubXlcRetryTable, BUFB_BASE + 4096, sizeof(gubXlcRetryTable));//17152B
			if (RET_OK == ftlReadSinglePlane(ubSystemUnit, 4, CE0, (U32)BUFB_BASE, MARK_SYS_HYNIX_RRT, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
				gsHynixRetryInfo.ulXlcRetryTblBase = (U32) &gubXlcRetryTable;
				memcpy(((&gubXlcRetryTable) + 12288), BUFB_BASE, sizeof(gubXlcRetryTable)); //17152B
			}
		}
		else {
			//Uart_Printf("RR Page not found\r\n");
		}
	}
#endif
	FlaIssueHynixDaCmd(FALL, gulCeEnBitMap, 0xBE);
#endif

	//==========4.  Turn off CH2==========
	if (!gubNoPWICEnable) {
		SYS0L[SYS0L_SYS_MUX_CTRL0] &= ~(BIT27);
		SYS0L[SYS0L_SYS_MUX_CTRL0] |= BIT26;

		I2C_Write_ConfigNOtrig(0x1b, 1, 0 , 0x11 , 0xBC);  //Group1  //Buffer0  //CH2 off
		*((volatile unsigned char*)0x0400250c) |= 0x01;
	}


	if (SYSL[SYS0L_SIM_CTRL0] & SIMREG_DLMC_PREFORMAT) {
		gubPreformat = 1;
		SYSL[SYS0L_SIM_CTRL0] &= (~SIMREG_DLMC_PREFORMAT);
	}

Mark_Manual_Preformat:
	if (!BURNER) {
		if (gubPreformat) {
			U32 ulE3Derror_temp;
			SYSTEM_GROUP_t *InfoArray;
			while (1) {
				switch (gubPreformat) {
					case 5:         //preformat
						ulE3Derror_temp = (FALLL[FCTLL_INT_CFG] & BIT17);
						FALLL[FCTLL_INT_CFG] &= (~BIT17);
						M_SetFlagSettingNone();
						ftlEraseAll(1);

						InfoArray = (SYSTEM_GROUP_t *)(&BUFB_BASE[0]);
						MP_SMART_Attribute_Entry * MP_Parameter = (MP_SMART_Attribute_Entry *)(&BUFB_BASE[512]);
						flaSetFakeInfoBlk(InfoArray, MP_Parameter);
						flaSetParameterBeforePreformat(InfoArray);

						ftlPreformat();
						FALLL[FCTLL_INT_CFG] |= ulE3Derror_temp;
						M_SetFlagSettingSHF();
						M_DisableAllFlashCE();
						gubPreformat = 0;
						while (1);
						break;

					case 1:
						ulE3Derror_temp = (FALLL[FCTLL_INT_CFG] & BIT17);
						FALLL[FCTLL_INT_CFG] &= (~BIT17);
						M_SetFlagSettingNone();

						UBYTE ubPreformatAfterDLMCEraseType = SYSB[SYS0B_SIM_CTRL0_B2];
						SYSB[SYS0B_SIM_CTRL0_B2] = 0xFF;//default value of this byte is 0xFF

						if ((ubPreformatAfterDLMCEraseType == 0xFF) || (ubPreformatAfterDLMCEraseType == 0x00)) {
							ubPreformatAfterDLMCEraseType = (BIT0 | BIT4 | BIT5);
						}

						if (ubPreformatAfterDLMCEraseType & BIT4) {
							Load_RUT_EC_Table();
							if (gubSSD_Init_State.B.ubState == BYTE_INIT_START_SCAN) {
								M_SetFlagSettingNone();
								while (gubSSD_Init_State.B.ubState < BYTE_INIT_TABLE_DONE) {
									M_SwitchTask();
								}
								M_SetFlagSettingSHF();
							}
						}
						mSE_COPY((U32)(&gubZ2Buffer[M_SysInfo_InfoBlk]), (U32)(&BUFB_BASE[0]), 0x200);
						mSE_COPY((U32)(&gubZ2Buffer[M_SysInfo_SmartMPBlk]), (U32)(&BUFB_BASE[512]), 0x200);
						mSE_COPY((U32)(&gubZ2Buffer[M_SysInfo_RetryTable]), (U32)(&BUFB_BASE[1024]), 0x200);
						mSE_COPY((U32)(&gubZ2Buffer[M_SysInfo_DPSTable]), (U32)(&BUFB_BASE[1536]), 0x800);
						memcpy((void *)&gubLastFWVer[0], (void*)&gubZ2Buffer[M_SysInfo_FW_Info1], 0x8);
						/// This line is for burner or manaul-Preformat usage, so need not use ENABLE_DPS to check.
						gubDPSTableValid = gubZ2Buffer[M_SysInfo_gubDPSTableValid];

						ftlEraseAll(ubPreformatAfterDLMCEraseType);//0x31,retain code block and keep RUT,DBT
						ftlPreformat();
						FALLL[FCTLL_INT_CFG] |= ulE3Derror_temp;
						M_SetFlagSettingSHF();
						M_DisableAllFlashCE();
						mSE_SV(4, (U32)PowerCycle, 96, 0, 0);
						PowerCycle->guwMaxFreeUnit = VT->guwFreeBlockCount_D3;


						if (ENABLE_ISPJUMP_AFTER_DLMC_PREFORMAT) {
							Vender_Isp_Jump(0);
						}
						else {
							UBYTE ubi, ubj;
							gubTransformSystemUnitToSystemSet = FALSE;
							gubSystemArea_RemappingRule = Rule_Set;
							gubPreformat = 0;
							gubSystemSet = 0xFF;
							guwSystemBlock[0] = 0xFFFF;
							guwSystemBlock[1] = 0xFFFF;
							gubDBTSet = 0xFF;
							guwDBTBlock[0] = 0xFFFF;
							guwDBTBlock[1] = 0xFFFF;
							guwSystemAreaNum[0] = 0;
							guwSystemAreaNum[1] = 0;
							for (ubi = 0; ubi < SYSTEM_SETS; ubi++) {
								for (ubj = 0; ubj < MAX_CHANNEL; ubj++) {
									guwSystemBlockRemappingTable[ubi][ubj] = 0xFFFF;
								}
								gubSystemSetFreeTable[ubi] = 0xFF;
							}
							LoadSystemBlock();
							InitFTLVariable();
						}
						break;
					default:
						gubPreformat = 0;
						break;
				}
				break;
			}
		}
	}

Mark_Get_Info:
	ulTime[2] = gulOperationTime;
	if (gubFlashInitialized && (!BURNER)) {
		UWORD *uwCopyBuffer = (UWORD *)(&gubZ2Buffer[0]);
		U32 *ulCopyBuffer = (U32 *)(&gubZ2Buffer[0]);
		UBYTE ubCEIndex;
		UBYTE ubPlaneBank;
		guwFWTotalUnitNum = uwCopyBuffer[(M_SysInfo_FWUnitNum / 2)];
		gulNativeSize = ulCopyBuffer[(M_SysInfo_NativeSize / 4)];
		guw_SDR_window_min = uwCopyBuffer[(M_SysInfo_uwSDR_window_min / 2)];
		guw_SDR_window_max = uwCopyBuffer[(M_SysInfo_uwSDR_window_max / 2)];
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
		// count 0
		FALLL[FCTLL_CNT_ONE] |= COUNT_MODE_EN;
		FALLL[FCTLL_CNT_ONE] &= (~COUNT_ONE_MODE);

		// Scan System Area
		if (SYSB[0x44] == SIMREG_DEVSLP_LOAD_CODE) {
			M_Reload_DEVDLP_Table();
		}
		else {
			ScanSystemFreeSet(0);
			flaMicronRandomizer(1);
#if Hynix
			FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif
			HandleCodeSet();

#if Hynix
			FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
			ScanSystemFreeSet(1);
#if Hynix
			FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xDA);
#endif

			flaMicronRandomizer(0);
#if (B0KB && !BURNER && DIS_PreRead)
			flaMicronPreRead(1);	// 1==> Disable Pre read
#endif
#if ENABLE_CODEBLOCKREPLACEMENT
			ftlCheckCodeBlockECC();
#endif
#if Hynix
			FlaIssueHynixDaCmd(FALL, 0xFFFFFFFF, 0xBE);
#endif
			if ((gubTransformSystemUnitToSystemSet == TRUE) || (gubTranslateMixPlaneMark == TRUE)) {
				TransformSystemBlock((U32)&gubZ2Buffer[0]);
				gubTransformSystemUnitToSystemSet = FALSE;//因為已經把SystemBlock轉換完並且存下去了, 所以就不用再轉換了
				gubTranslateMixPlaneMark = FALSE;
			}
		}
		gubExpectErasePage = 0;
		gubExpectBadkBlock = 0;

		// Switch to FTL to Do ftlInit
		gubSSD_Init_State.B.ubState = BYTE_INIT_START_SCAN;
		if (ENABLE_WRITEPROTECT) {
			if (gubIsWriteProtect & (BIT_FLASH_INIT_FAIL_WRITEPROTECT | BIT_ERR_UNIT_WRITEPROTECT)) { // If Err Unit Found, Not Init FTL
				gubSSD_Init_State.B.ubState = FALSE;
				mSE_SV(4, (U32)gulEC_D3, (gubPlanesPerECTable << gubSectorsPerPlaneLog) * 512, (U32)MARK_ErrorDumpUnit, 0);
			}
		}
		//FALLB[(FCTLL_INT_INF << 2) + 1] &= MAP_FIFO_POINTER_RESET_BYTE1;
	}

	if (gubGPIOEraseDoing) {
		Vender_GPIO_Erase(1);
	}

	if (BURNER) {
#if (!ENABLE_ODD_CE)
		UBYTE ubCECH0 = 0, ubCECH1 = 0, ubCount;
		for (ubCount = 0; ubCount < MAX_CE_PER_CH; ubCount ++) {
			if (gubReMappingCE[ubCount] != 0x3F) {
				ubCECH0 ++;
			}
			if (gubReMappingCE[MAX_CE_PER_CH + ubCount] != 0x3F) {
				ubCECH1 ++;
			}
		}
		if (ubCECH0 != ubCECH1) {
			goto Mark_SATA_Link;
		}
#endif
#if ENABLE_BURNER_SWITCH_TO_TOGGLE1
		gubForceExit = 0;
		gubFlashClock = FLH_CLK_40MHz;
#if Hynix
		gubInterface = INTERFACE_TOGGLE1;
		gubFlashMode = FLH_MODE_TOGGLE1;
#endif
		flaSetFlashMode();
#else
		gubForceExit = 1;
		flaSetFlashMode();
		flaSet_ODT_DRIVE();
		gubForceExit = 0;
#endif
	}
#if Hynix
	flaDisableSecretMode();
#endif
Mark_SATA_Link:
	if ((SYSL[SYS0L_SIM_CTRL0] & SIMREG_CMD_D2H) == 0) {
		if (ASIC && (!((gubCmdCode == 0x31) && (HB[HB_FEATURE] == 0x1)))) {

			if (gubSSC_Enable == 1) {
				SYS0B[0x335] |= BIT4;
			}
			else if (gubSSC_Enable == 2) {
				SYS0B[0x335] &= ~BIT4;
			}
			Tune_PHYPMA_GEN3();
			Tune_PHYPMA_GEN2();
			Tune_PHYPMA_GEN1();
			if (gubHighTempSetting == 1) {
				*((volatile unsigned long*)0x04000374) |= (BIT3); //gen1 config
				*((volatile unsigned long*)0x04000374) |= (BIT6 | BIT7);
				*((volatile unsigned long*)0x04000374) &= (CLR_BIT9 & CLR_BIT10);
				*((volatile unsigned long*)0x04000394) |= (BIT3); //gen2 config
				*((volatile unsigned long*)0x04000394) |= (BIT6 | BIT7);
				*((volatile unsigned long*)0x04000394) &= (CLR_BIT9 & CLR_BIT10);
				*((volatile unsigned long*)0x040003B4) |= (BIT3); //gen3 config
				*((volatile unsigned long*)0x040003B4) |= (BIT6 | BIT7);
				*((volatile unsigned long*)0x040003B4) &= (CLR_BIT9 & CLR_BIT10);
			}
			if ((HW[HW_SATA_INFO] & H_LNK_ESTABLISH) == 0) {
				if (!BURNER) {
					if (SYSB[0x44] == SIMREG_DEVSLP_LOAD_CODE) {
						HL[HL_PHY_MISC1] &= ~H_OOB_STOP;
					}
					else {
						HW[HW_SYS_CTRL] &= ~H_REG_SATA_EN;
						HW[HW_SYS_CTRL] |= H_REG_SATA_EN;
						HL[HL_PHY_MISC1] &= ~H_OOB_STOP;
					}
				}
			}
		}
	}
	HL[HL_PHY_MISC1] &= ~H_OOB_STOP;
Mark_FTL_Init:
	ulTime[3] = gulOperationTime;
	// Wait Scan Table Done
	if (!BURNER) {
		M_DebugGPIO8_ON_LED();
		M_SetFlagSettingNone();
		while (gubSSD_Init_State.B.ubState < BYTE_INIT_TABLE_DONE) {
			if (ENABLE_WRITEPROTECT && (gubIsWriteProtect & (BIT_ERR_UNIT_WRITEPROTECT | BIT_FLASH_INIT_FAIL_WRITEPROTECT))) {
				break; // If Err Unit Found, Not Init FTL
			}
			M_SwitchTask();
		}
	}

	if (ENABLE_CONTINUE_SECURITY_ERASE_AFTER_POWER_CYCLE) {
		if (AtaCfg.ubSecurityEraseIsDoing) {
			if (ENABLE_DEBUG_UART_ATA_CMD) {
				UartString("\r\nerase-power cycle start");
			}

			TrimAll();
			gubFlushMode = BYTE_STANDBY_FLUSH;
			FlushCache();
			if (AtaCfg.ubLastSecurityEraseMode & SECURITY_ERASE_ENHANCE_MODE) {
				SecurityEraseUserData();
			}
			AtaCfg.ubLastSecurityEraseMode = 0;

			if (AtaCfg.ubSecurityEraseIsDoing == 1) {
				AtaCfg.ubSecMode &= ~(SET_SEC_EN | SET_SEC_LOCK | SET_SEC_MAX);
			}

			if (ENABLE_CONTINUE_SECURITY_ERASE_AFTER_POWER_CYCLE) {
				AtaCfg.ubSecurityEraseIsDoing = 0;
			}
			SaveAtaConfig();
			if (ENABLE_DEBUG_UART_ATA_CMD) {
				UartString("\r\nerase-power cycle end");
			}
		}
	}

Mark_1ST_D2H:
	ulTime[4] = gulOperationTime;
	gulLastSavedTimeInMinutes = (U32)GetRTT_m();

	if (ENABLE_DEBUG_UART_INIT_TIME) {
		UBYTE ubTimeIndex;
		UartString("\nmain_t ");
		for (ubTimeIndex = 0; ubTimeIndex < 4; ubTimeIndex ++) {
			UartLongHex(ulTime[ubTimeIndex + 1] - ulTime[ubTimeIndex]); // SDR_IRAM_Init   variable_reg_init   scan_sys_set_feature
			UartString("  ");
		}
		UartString(" Total_T");
		UartLongHex(ulTime[4] - ulTime[0]);
	}
	if (SYSB[0x44] == SIMREG_DEVSLP_LOAD_CODE) {
		ResetRegFromIDT();	// AtaCfg會由flash load, IDT會由AON抓
		//if (HL[HL_INT_FLAG] & H_LNK_COMRI_INT) {
		//gubHardReset = 1;
		//Init_Reset();
		//   gubHardReset = 0;
		//}
#if(!BURNER)
#else
		SYSB[0x44] = SIMREG_NORMAL_LOAD_CODE;
#endif
		if (ENABLE_DEBUG_UART) {
			UartString("\nDEVSLP_wake");
		}
#if(!BURNER)
		if (SYS1L[SYS1L_INT_STS4]& PMU_INT_PD0GPIO_FALLING_EDGE) {
			if (gubJTagWakeUp == 0) {
				gubCustomerGPIOEraseEnable = 1;
				goto Mark_QuickErase;
			}
		}
#endif
	}
	else {
		Init_Reset();
	}

	InitSATARegister();
#if(!BURNER)
	if (SYSB[0x44] == SIMREG_DEVSLP_LOAD_CODE) {
		SYSB[0x44] = SIMREG_NORMAL_LOAD_CODE;
	}
#endif

	if ( (!gubNCQINTRError) && ((AtaCfg.ubSecMode & SET_SEC_LOCK) == 0) && (gubPowerMode != PWR_SLEEP) && (gubIsWriteProtect == FALSE) && ((AtaCfg.ubSanitizeState == SD0_SANITIZE_IDLE) || (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN)) && (gubNCQSupport) ) {
		M_EnableHWD2H_NCQ();
	}

	if (ENABLE_DEBUG_FORCE_BG_TEST) {
		UBYTE ubClearIndex;
		for (ubClearIndex = 0; ubClearIndex < 10; ubClearIndex ++) {
			gulPseudo_BG_Stop_Case_Num[ubClearIndex] = 0;
		}
	}
	/*
	  發生error後 擋住所有FIS
	  E3D HW預設幫檔, 但是如果清掉error flag, 就會續傳, 所以需要
	  CRC 有R_ERR讓State machine回到idle, 不需要
	  Unc可以flag確保APU Reset不會切掉正在傳輸的FIS, 也不需要 (先清掉flag在作apuReset就需要)
	  */
	if (!BURNER) {
		HW[HW_SYS_CTRL] |= H_BLK_FIS_EN;
	}
	//參考Comreset isr說明
	if (ComresetRisingEdge) {
		HL[HL_HAT_MISC] &= CLR_BIT4 | CLR_BIT5;
		HL[HL_HAT_MISC] |= BIT4;
	}

	M_SetFlagSettingSHF();

	if (ENABLE_DEBUG_HW_FORCEREADFAIL || ENABLE_DEBUG_HW_FORCEPROGRAMFAIL || ENABLE_DEBUG_HW_FORCEERASEFAIL || ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL) {
		gubGenReadFailDebug = ENABLE_DEBUG_HW_FORCEREADFAIL;
		gubGenProgramFailDebug = STOP_GEN_FAIL;	// P_FAIL_TEMP
		//gubGenProgramFailDebug = ENABLE_DEBUG_HW_FORCEPROGRAMFAIL;
		gubGenEraseFailDebug = ENABLE_DEBUG_HW_FORCEERASEFAIL;
	}

Mark_Main_Task_Loop:
	gulBackGround_Counter = gulOperationTime;
	gulEnterBG_Time_1 = gulEnterBG_Time_D2H_RST; // For 1st D2H
	gulEnterBG_Time_2 = gulEnterBG_Time_D2H_RST; // For 1st D2H

	if (ENABLE_DEBUG_UART_WRITEPROTECT) {
		if (gubIsWriteProtect) {
			UartString("\n\rMain btFlush:");
			Uart_Tx_DataHex(VT->gulFTLState.B.btFlushCache);
		}
	}

#if (!BURNER)
	Vender_GPIO_Setting();
#endif
	while (1) {
		M_SwitchTask();
		M_DebugGPIO8_OFF_LED();
	Mark_Reset:
		if (gubENABLE_FW_DIPM && gubDIPMSupport) {	/// if AUTO_DIPM is on at where FW-DIPM, it's the place to shut it down
			HW[HW_PM_CTRL] &= ~(H_HW_AUTO_DIPM_EN);
		}
#if !BURNER
		if ((gubJTagWakeUp == 0) && (gubQE_FromSlumber == 1) && (((gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_USER)
		        || (gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_ALL)) && (gubGPIOEdgeMode == GPIO_EDGE_MODE_FALLING_ERASE))) {
			goto Mark_QuickErase;
		}
#endif
		if (gubSoftReset || gubHardReset || gubLinkLost) {
			/*可能比較晚才下reset, 或是突然拔插, 所以reset前還是要加等FQ*/
			/*
			gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
			while (gBQI.ubLinkNum|| gFQI.ubFQLinkNumber) {
			        M_SwitchTask(); //等之前的FQ與BQ做完, 否則把flag清掉會做不完, 哭哭
			}
			*/
			M_DebugGPIO8_ON_LED();
			if (gubTimeToDoBG) {
				if (ENABLE_DEBUG_UART_BG) {
					UartString("\nRstIn");
					UartString(" ");
					Uart_Tx_DataHex(gubBG_Copy_State);
				}
				M_StopBG_Copy();
			}
			else {
				HandleStopRW(0);
			}
			if (gubSoftReset || gubHardReset) {
				AtaCfg.gSanitizeOverwrite.ubSuspend = 0;
				ResetCmdIn();

				if (gubLinkLost) {
					gubLinkLost = 0;
				}
			}
			else if (gubLinkLost) {

				HW[HW_ERR_CTRL] &= ~(H_BLK_TRG | H_REG_XHOLD);
				gubLinkLost = 0;

			}
			gulBackGround_Counter = gulOperationTime;
			gulEnterBG_Time_1 = gulEnterBG_Time_D2H_RST; // For hard reset
			gulEnterBG_Time_2 = gulEnterBG_Time_D2H_RST; // For hard reset
			gubForceStop &= ~BIT1;
		}
#if ENABLE_PFAIL
		if (gubPfail_state) {
			switch (gubPfail_state) {
				case PFAIL_STATE_PRE_LOCK: {
#if ENABLE_ULTRAMLC_PFAIL_MEASURE
					UBYTE timed = (gBQI.ubWLinkNum + gFQI.ubFQLinkNumber_W + gubKeepLoadAlignmentBQNum);

					//_ASSERT(0x80, (SYS1L[SYS1L_RT2_CNT] > 10000) && timed);
					if (0) { //(timed>0) && (SYS1L[SYS1L_RT2_CNT] > 10000))
						UBYTE ubFQCount;
						FlashQueue_t *FQ;

						UartLongHex(SYS1L[SYS1L_RT2_CNT]);
						UartString("\ntr:\n");
#endif

#if DEBUG_ULTRAMLC_POWER_FAIL
						UBYTE ubgFQLinkIndex = 0;
						guwTemp = (trace_arr_index - 32) & 0x3FFU;
						do {
							UartLongHex(trace_arr[guwTemp]);
							UartString(" ");
							guwTemp++;
							guwTemp &= 0x3FFU;
							ubgFQLinkIndex++;
							if (0 == (ubgFQLinkIndex & 0x07)) {
								UartString("\n");
							}
						}
						while (ubgFQLinkIndex < 32);
#endif

#if ENABLE_ULTRAMLC_PFAIL_MEASURE
						UartLongHex(VT->gulFTLState.All);

						UartString("\nFQ:\n");
						for (ubFQCount = 0, ubgFQLinkIndex = gFQI.ubFQLinkFirst; ubFQCount < gFQI.ubFQLinkNumber; (ubgFQLinkIndex = FQ->ubNext), ubFQCount++) {
							FQ = &gFQI.gFQLink[ubgFQLinkIndex];
							Uart_Tx_DataHex(FQ->ubFJob);
							UartString(",");
							UartWordHex(FQ->uwFJobInfo);
							UartString(",");
							Uart_Tx_DataHex(FQ->FQPara.All);
							UartString(",");
							UartLongHex(FQ->ulSpare);
							UartString("\n");
						}

					}

					if (timed) {
						ulTime[0] = SYS1L[SYS1L_RT2_CNT];
						ulTime[1] = (gubKeepLoadAlignmentBQNum << 24) | (gBQI.ubWLinkNum << 16) | (gBQI.ubRLinkNum << 8) | (gBQI.ubLinkNum << 0);
						ulTime[2] = (gubTimeToDoBG << 16) | (gFQI.ubFQLinkNumber_W << 8) | (gFQI.ubFQLinkNumber << 0);
						ulTime[3] = (gubAbort_CMD << 24) | VT->gulGRTargetPTR;
						ulTime[4] = VT->gulFTLState.All;
					}
					//_ASSERT(0x80, (gulPfail_time>0) && (timed));
#endif
					//_DEBUG_TRACE((0xE1<<24) | ((gFQI.ubFQLinkNumber<<16)) | ((gBQI.ubLinkNum<<8)) | (gulLastWriteCmdsCount&0xFF));
					if (gubTimeToDoBG != 0) {
#if (DEBUG_ULTRAMLC_POWER_FAIL_BG_COPY && DEBUG_ULTRAMLC_POWER_FAIL)
						_DEBUG_TRACE((0xE1 << 24) | ((gFQI.ubFQLinkNumber << 16)) | ((gBQI.ubLinkNum << 8)) | (gulLastWriteCmdsCount & 0xFF));
#endif
						M_StopBG_Copy();
					}
#if (DEBUG_ULTRAMLC_POWER_FAIL_BG_COPY && DEBUG_ULTRAMLC_POWER_FAIL)
					_DEBUG_TRACE((0xE2 << 24) | ((gFQI.ubFQLinkNumber << 16)) | ((gBQI.ubLinkNum << 8)) | (gulLastWriteCmdsCount & 0xFF));
#endif
					HandleStopRW(1);
					gubStopFTL = 0;
					gubPfail_state = PFAIL_STATE_LOCKED;
#if ENABLE_ULTRAMLC_PFAIL_MEASURE
					if (timed) {
						//UartString(", ");
						if (1) { //(SYS1L[SYS1L_RT2_CNT]>(2000UL * 8 / 10))
							UartLongHex((SYS1L[SYS1L_RT2_CNT]));
							UartString(", ");
							UartLongHex(ulTime[0]);
							UartString(", ");
							UartLongHex(ulTime[1]);
							UartString(", ");
							UartLongHex(ulTime[2]);
							UartString(", ");
							UartLongHex(ulTime[3]);
							UartString(", ");
							UartLongHex(ulTime[4]);
						}
#endif

#if DEBUG_ULTRAMLC_POWER_FAIL
						if (1) { //(gubAbort_CMD)
							UBYTE ubgFQLinkIndex;
							ubgFQLinkIndex = 0;
							UartString("\n");
							guwTemp = (trace_arr_index - 32) & 0x3FFU;
							do {
								UartLongHex(trace_arr[guwTemp]);
								UartString(" ");
								guwTemp++;
								guwTemp &= 0x3FFU;
								ubgFQLinkIndex++;
								if (0 == (ubgFQLinkIndex & 0x07)) {
									UartString("\n");
								}
							}
							while (ubgFQLinkIndex < 32);
						}
#endif

#if ENABLE_ULTRAMLC_PFAIL_MEASURE
					}
					UartString("\n");
#endif
				}
				break;
				case PFAIL_STATE_PRE_RELEASE:
					gubPfail_state = 0;
					//gubStopFTL = 0;
#if ENABLE_ULTRAMLC_PFAIL_MEASURE
					UartString("  U:");
					//UartLongHex(gulOperationTime);
					//UartString(", ");
					UartLongHex(gulPfail_time);
					UartString("\n");
#endif
					break;
			}
		}
#endif
	Mark_NCQ_CMD:
#if(!BURNER)
		if ((!BURNER) && (gubNCQSupport && (HB[HB_NCQ_ORDER_VLD_SUM] || gulNCQWRCmdTriggerCnt) && (!gubNCQINTRError)) && (gubIgnoreFlag == 0) ) {
#else
		if ((!BURNER) && (gubNCQSupport && (HB[HB_NCQ_ORDER_VLD_SUM] || gulNCQWRCmdTriggerCnt) && (!gubNCQINTRError))) {
#endif
			M_DebugGPIO8_ON_LED();
			if (gubTimeToDoBG) {
				if (ENABLE_DEBUG_UART_BG) {
					UartString("\nNCQ");
					if (HW[HW_NCQ_ORDER_INFO]&H_NCQN_WRITE) {
						UartString("W");
					}
					else {
						UartString("R");
					}
					UartString(" ");
					Uart_Tx_DataHex(gubBG_Copy_State);
				}
				M_StopBG_Copy();
			}
			M_Check_LastCMD_StandBy(VT_MAIN_CLEAR_LASTCMD_STANDBY_NCQ); //TSB 要求......
			if (ENABLE_D2HSDBByHW) {
				M_EnableHWD2HSDB_WR();
				gubNeedSendCmdComplete = 0;
			}
			else {
				gubNeedSendCmdComplete = 1;
			}

			gubNCQDoing = 1;
			CheckVenderLock();
			/*
			RD_NCQ_INFO ulNCQ_INFO;
						ulNCQ_INFO.All = HL[HL_RD_NCQ_INFO];
						if (ulNCQ_INFO.B.ubNCQ_CMD_VLD_SUM && (ulNCQ_INFO.B.ubNCQ_NEXT_RD != 0x3F) && (ulNCQ_INFO.B.ubNCQ_NEXT == ulNCQ_INFO.B.ubNCQ_NEXT_RD)) {
							ReadSectors();
						}
						else {
							WriteSectors();
						}
			                      */

			M_Refresh_PowerMode_And_SMART_Status(1, 0, 0);
			if (ENABLE_RESET_CNT) {
				gubResetFlag = 1;
				gubResetCountDuringCmd = 0;
			}
			if (HW[HW_NCQ_ORDER_INFO]&H_NCQN_WRITE) {
				if (gubIsWriteProtect && ENABLE_DEBUG_UART_WRITEPROTECT) {
					UartString("\r\nNCQW");
				}
				//UartString("\r\nNCQW");

				// If FUA is support and FUA bit is set, disable WriteCache
				if (FUA_Support && (HB[HB_DEVICE] & CHK_FUA)) {

					UBYTE ubWriteCacheEnable_Temp = gubWriteCacheEnable;
					gubWriteCacheEnable = 0;

					WriteSectors();

					gubWriteCacheEnable = ubWriteCacheEnable_Temp;
				}
				else {
					WriteSectors();
				}
			}
			else {
				if (gubIsWriteProtect && ENABLE_DEBUG_UART_WRITEPROTECT) {
					UartString("\r\nNCQR");
				}
				//UartString("\r\nNCQR");
				ReadSectors();
			}

			//AtaCfg.ubLastCmd = gubCmdCode;
			AtaCfg.ubLastCmd = HB[HB_COMMAND];  //因為 NCQ R/W 不會進 SATA_ISR, 所以 ubLastCmd 不會被改到 -> 會造成連續性 CMD 如 Security Erase 和 SetMax CMD 出問題

			if (ENABLE_RESET_CNT) {
				gubResetFlag = 0;
			}

			if (gubNeedSendCmdComplete || gubErrorCode) {
				if ( !(gubSoftReset | gubHardReset) ) {
					if (gubErrorCode) {
						HB[HB_STATUS] = 0x41;    //used status 0x40 for NCQ

						if (gubIsWriteProtect) {
							HB[HB_STATUS] |= SET_MC;
						}
						if (gubErrorCode & SET_UNC) {
							HL[HL_LBA_L] = gulUNCLBA;
						}
						HB[HB_ERROR] = gubErrorCode;
						HL[HL_SACTIVE] = 0;
						//HW[HW_ERR_CTRL] &= ~ H_BLK_TRG;   //  off Discard trigger bit function

						//RMA_FLOW
						if (gubIsWriteProtect & (BIT_ERR_UNIT_WRITEPROTECT | BIT_FLASH_INIT_FAIL_WRITEPROTECT)) {
							HB[HB_ERROR] = 0;
							HB[HB_STATUS] = 0x40;    //used status 0x40 for NCQ
							HL[HL_SACTIVE] = (U32)BIT0 << gubNCQDoingTag; //set complition tag to host, SetDevBits
						}

						if ((gubLinkLost != 0) && ((HB[HB_CONTROL] & 0x04) != 0x04)) { //有Link Lost且沒有softreset就打SDB/D2H
							HW[HW_ERR_CTRL] &= ~(H_BLK_TRG);
						}

					}
					else {
						HB[HB_STATUS] = 0x40;    //used status 0x40 for NCQ
						HL[HL_SACTIVE] = (U32)BIT0 << gubNCQDoingTag; //set complition tag to host, SetDevBits
					}
					/*
					if (gubForceExit && (!(gubHardReset | gubSoftReset))) {
					//gubForceExit = 0;
					HW[HW_ERR_CTRL] &= (~H_BLK_TRG);     //  off Discard trigger bit function
					}
					HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
					while (HB[HB_REG_REQ]&H_REG_REQ);
					HB[HB_ERROR] = NO_ERROR;
					*/

					HB[HB_SDB_REQ] = H_SDB_REQ | H_SDB_IBIT;
					while (HB[HB_SDB_REQ]&H_SDB_REQ);	//Wait D2H Finished
					gubCMDFinish = 1; //CMD 結束可以啟動BG(NCQ)

					//if ((!gubNCQINTRError)&&(gubWriteProtectMap==0)) {
					/* 若NCQ發生fail, 接下來的CMD需是ReadLog或是Reset, 故不能autoD2H (APU reset會清掉autoD2H)
					if ( (!gubNCQINTRError) && ((AtaCfg.ubSecMode & SET_SEC_LOCK) == 0) && (gubPowerMode != PWR_SLEEP) && (gubWriteProtectMap == 0) && ((AtaCfg.ubSanitizeState == SD0_SANITIZE_IDLE) || (AtaCfg.ubSanitizeState == SD1_SANITIZE_FROZEN))) {
						M_EnableHWD2H_NCQ();
					}
					*/
					HW[HW_ERR_CTRL] &= (~H_REG_XHOLD);
					if (gubForceExit) {}
				}

				if (gubErrorCode) {     //打完error D2H後, 設回原值
					HB[HB_STATUS] = 0x40;
					HB[HB_ERROR] = NO_ERROR; //這行沒設的話, CKtool or 其他會polling error reg的都會出錯 (因為像是PIO 不會打D2H的, 都要在這重設)
					gubErrorCode = 0;
					gulUNCLBA = 0;
					//未必會下reset, 所以在這邊把buffer flag都清掉
					if (!(gubSoftReset | gubHardReset)) {
						/*
						gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
						while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber) {
						        M_SwitchTask(); //等之前的FQ與BQ做完
						}

						M_SetFlagSettingNone(); // 在resetCmdIn中會設回來
						AXIL[AXIL_SET_START_FLAG] = 0;
						AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM;
						AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
						M_SetFlagSettingSHF();  //在ISR清掉,但因為可能沒進Reset, 所以這邊要舉
						*/
#if ENABLE_PFAIL
						if (0U == gubAbort_CMD)
#endif
						{
							HandleStopRW(0);
						}
						HW[HW_ERR_CTRL] &= ~H_REG_XHOLD; //Error會擋XRDY
					}
				}
			}

			M_Refresh_CMDInStartTime_For_PwrMode(1, 0);
			gulBackGround_Counter = gulOperationTime;
			gulEnterBG_Time_1 = gulEnterBG_Time_Flush;
			gulEnterBG_Time_2 = gulEnterBG_Time_COPY;
		}
		if (BURNER) {
			if (HB[HB_NCQ_ORDER_VLD_SUM] ) {

				if (ENABLE_D2HSDBByHW) {
					M_EnableHWD2HSDB_WR();
					gubNeedSendCmdComplete = 0;
				}
				else {
					gubNeedSendCmdComplete = 1;
				}
				gubNCQDoing = 1;
				SDR_RW_DMA();

				if (gubNeedSendCmdComplete || gubErrorCode) {
					if ( !(gubSoftReset | gubHardReset) ) {

						HB[HB_STATUS] = 0x40;    //used status 0x40 for NCQ
						HL[HL_SACTIVE] = (U32)BIT0 << gubNCQDoingTag; //set complition tag to host, SetDevBits

						HB[HB_SDB_REQ] = H_SDB_REQ | H_SDB_IBIT;
						while (HB[HB_SDB_REQ]&H_SDB_REQ);	//Wait D2H Finished

						HW[HW_ERR_CTRL] &= (~H_REG_XHOLD);
						if (gubForceExit) {}
					}

				}
			}
		}
	Mark_Host_CMD:
		if (gubHostCmdIn) {
			UBYTE ubCmdCode;
			M_DebugGPIO8_ON_LED();
			if (gubDevIPMON) {
				HW[HW_PM_CTRL] &= H_HW_AUTO_DIPM_DIS;
			}
			if (gubIsWriteProtect && ENABLE_DEBUG_UART_WRITEPROTECT) {
				UartString("\n\rCmd In:");
				Uart_Tx_DataHex(gubCmdCode);
				UartString("  ErrorCode:");
				Uart_Tx_DataHex(gubErrorCode);
			}
			//gubErrorCode = 0;
			gubHostCmdIn = 0; //處理這次的cmd, 若是後來舉1表示又收到Cmd
			gubNCQDoing = 0;
			if (gubTimeToDoBG) { // S9在此還有分是否為RW command, 是RW才清成零
				if (ENABLE_DEBUG_UART_BG) {
					UartString("\nCMD");
					Uart_Tx_DataHex(gubCmdCode);
					UartString(" ");
					Uart_Tx_DataHex(gubBG_Copy_State);
				}
				M_StopBG_Copy();
			}
			if ((gubCmdCode != 0xE0) && (gubCmdCode != 0xE2) && (gubCmdCode != 0xE6)) { // 非standby cmd, 存VT
				M_Check_LastCMD_StandBy(VT_MAIN_CLEAR_LASTCMD_STANDBY_DOSTCMD); //TSB 要求......
			}
			//PIO Read不須打D2H
			if ((ENABLE_D2HSDBByHW == 0) && ((HW[HW_CMD_INFO] & (H_PIO_DMA_CMD | H_NCQ_CMD | H_WR_CMD | H_CONT_LBA_CHK_CMD)) == 0)) {
				// (not NCQ)&&(not DMA)&&(not Write)&&(not notRW), means PIO read when send D2H by FW.
				gubNeedSendCmdComplete = 0;
			}
			else {
				gubNeedSendCmdComplete = 1; //原本code是在cmd後給1當預設, 但是這邊比較直覺?
			}

			if (ENABLE_COMMANDQUEUE) {
				while (HW[HW_CMD_LBA_INFO]&H_C2L_ACT);
				ftlAddToDebugCommanddQueue(gubCmdCode, HL[HL_CMD_S_LBA], HL[HL_BUF_TRG_SEC], gulOperationTime);
			}
			/*
			        disable all hw (做完的)D2H or SDB, 只有R/W需要回
			        (這邊mark掉的話,可能造成FW還沒做完, HW先回,導致host判定Dev已經做完而下下一道cmd
			*/
			M_DisableHWD2HSDB();

			gulSectorCnt = HL[HL_BUF_TRG_SEC];
			ubCmdCode = gubCmdCode;// use local variable to save, or global variable my be changed when new command in.
			if (ENABLE_KT_DEBUG_UART) {
				UartString("\r\nCmd = 0x");
				Uart_Tx_DataHex(gubCmdCode);
				UartString(" ,Feature= 0x");
				Uart_Tx_DataHex(HB[HB_FEATURE]);
				UartString(" ,LBA = 0x");
				UartLongHex(HL[HL_LBA_L]);
			}

			AtaCfg.gSanitizeOverwrite.ubSuspend = 0;
			M_Refresh_PowerMode_And_SMART_Status(0, ubCmdCode, HB[HB_FEATURE]);

			if (ENABLE_RESET_CNT) {
				if (((ubCmdCode == 0x2f) || ((ubCmdCode == 0x47) && (IDT.W119 & BIT3))) && (HB[HB_LBA_L] == 0x04)) {
					gubResetFlag = 0;
				}
				else {

					gubResetFlag = 1;
					gubResetCountDuringCmd = 0;
				}
			}

			if ( (ubCmdCode == 0x25 || ubCmdCode == 0x35) && gubAP_enable && (!gubMPRW) ) {
				gubMPRW = 1;
			}
			if (gubMPRW) {
				gulMPRWCMDInStartTime = GetRTT_milis();
			}

			CheckVenderLock();
			gulHostCMDTriggerTime = gulOperationTime;
			gulFTLStateForTimeout = VT->gulFTLState.All;
			SataCmdTable[ubCmdCode]();//command dispatch

			if ((!gubAP_enable) && gubMPRW ) {
				gubMPRW = 0;
			}


			if ((ubCmdCode == 0xB0) && (HB[HB_FEATURE] == 0xD0)) {
				if (gubOfflineMode & SMART_M_TIMERENABLE) {
					if (gubOfflineMode & SMART_M_SUSPEND) {
						gubOfflineMode &= (~SMART_M_SUSPEND);
						gulSmartScanStartTime = (U32)(GetRTT_s() - gulSmartCurrentTime);//current time here equals to suspend time
					}
				}
				else if (AtaCfg.gSMARTSelective.ubOffLineDoing & SELECTIVE_OFFLINESUSPEND ) {
					AtaCfg.gSMARTSelective.ubOffLineDoing &= (~SELECTIVE_OFFLINESUSPEND);
				}
			}

			AtaCfg.ubLastCmd = ubCmdCode;
			if (gubIsWriteProtect && ENABLE_DEBUG_UART_WRITEPROTECT) {
				UartString("\n\rCmdOut:");
				Uart_Tx_DataHex(ubCmdCode);
				UartString("  ErrorCode:");
				Uart_Tx_DataHex(gubErrorCode);
			}
			if (ENABLE_RESET_CNT) {
				gubResetFlag = 0;
			}

			if (ENABLE_KT_DEBUG_UART) {
				//UartString(" ..done");
			}
			if ((M_CommandOperationTime(gulHostCMDTriggerTime)) > guwATATimeoutThreshold) {
				gulFTLStateForTimeout |= VT->gulFTLState.All;
				ftlRecordFailLog(ATATimeoutLogType, 0x1, ubCmdCode, gubErrorCode, DONTCARE, DONTCARE);
			}
			if (gubNeedSendCmdComplete || gubErrorCode) {
				//gubNeedSendCmdComplete = 1; // YK:拿掉測 smart log會fail...
				if ( !(gubSoftReset | gubHostCmdIn | gubHardReset) ) {//若有任一為1,表示Host不想等結果..
					if (gubErrorCode) {
						if (gubErrorCode & SET_UNC) {
							HL[HL_LBA_L] = gulUNCLBA;
						}
						HB[HB_STATUS] = 0x51;
						if (gubIsWriteProtect) {
							HB[HB_STATUS] |= SET_MC;
						}
						HB[HB_ERROR] = gubErrorCode;

						if (HB[HB_ERROR] != SET_ABRT) {
							if ( HB[HB_ERROR] != SET_IDNF) {
								RecordCmdLog();
							}
						}

						if ((gubLinkLost != 0) && ((HB[HB_CONTROL] & 0x04) != 0x04)) { //有Link Lost且沒有softreset就打SDB/D2H
							HW[HW_ERR_CTRL] &= ~(H_BLK_TRG);
						}

					}
					else {
						HB[HB_STATUS] = 0x50;
					}

					HB[HB_REG_REQ] = (H_REG_REQ | H_REG_IBIT);
					while (HB[HB_REG_REQ]&H_REG_REQ);
					gubCMDFinish = 1;//CMD 結束可以啟動BG(Host)
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
				gulUNCLBA = 0;
				/*非NCQ的話,做完就等於flag結束*/
				if (!(gubSoftReset | gubHardReset)) {
					//M_SetFlagSettingSHF();  //在ISR清掉,但因為可能沒進Reset, 所以這邊要舉
#if ENABLE_PFAIL
					if (0U == gubAbort_CMD)
#endif
					{
						HandleStopRW(0);
					}
					HW[HW_ERR_CTRL] &= ~H_REG_XHOLD; //為了PIO Read最後一筆發生CRC十擋下的XRDY, 這邊要打開
				}
			}

			M_Refresh_CMDInStartTime_For_PwrMode(0, ubCmdCode);

			if (gubENABLE_HW_DIPM && gubDevIPMON && (!gubHostCmdIn)) {
				HW[HW_PM_CTRL] |= H_HW_AUTO_DIPM_EN;
			}
			gulBackGround_Counter = gulOperationTime;
			gulEnterBG_Time_1 = gulEnterBG_Time_Flush;
			gulEnterBG_Time_2 = gulEnterBG_Time_COPY;
		}

		if (ENABLE_WRITEPROTECT) {
			//Avoid entering powercycle handling when power is on.
			if ((gubIsWriteProtect & BIT_TRULY_WRITEPROTECT) && (VT->gubWriteProtectState == BYTE_STATE_WAIT_LAST_FLUSH)) {
				if (gubTimeToDoBG) { // S9在此還有分是否為RW command, 是RW才清成零
					if (ENABLE_DEBUG_UART_BG) {
						UartString("\nWP");
						Uart_Tx_DataHex(gubCmdCode);
						UartString(" ");
						Uart_Tx_DataHex(gubBG_Copy_State);
					}
					M_StopBG_Copy();
				}
				gubFlushMode = BYTE_STANDBY_FLUSH;
				VT->gubWriteProtectState = BYTE_STATE_WRITEPROTECT;
				if (ENABLE_DEBUG_UART_WRITEPROTECT) {
					UartString("\n\rEnter write protect flush.");
				}
				FlushCache();
			}
		}

		/// Get Temperature & change auto refresh value
#if(!BURNER)
		//////////////////////////
		if ((gubCustomerGPIO3SelectMode == CUSTOMER_GPIO3_MODE_QUICKERASE_USER)
		        || (gubCustomerGPIO3SelectMode == CUSTOMER_GPIO3_MODE_QUICKERASE_ALL)
		        || (gubCustomerGPIO3SelectMode == CUSTOMER_GPIO3_MODE_QUICKERASE_N92)) {
			if (gubGPIO3EdgeMode == GPIO_EDGE_MODE_RISING_ERASE) {
				if ((gubCustomerGPIO3EraseEnable == 0) && ((SYS0L[SYS0L_GPIO_CTRL1] & BIT25) == BIT25)) { //rising
					guoCustomerGPIO3Timer = GetRTT_milis();
					gubCustomerGPIO3EraseEnable = 1;
				}
				else if ((gubCustomerGPIO3EraseEnable != 0) && ((SYS0L[SYS0L_GPIO_CTRL1] & BIT25) == 0)) { //falling
					gubCustomerGPIO3EraseEnable = 0;
				}
			}
			else {
				if ((gubCustomerGPIO3EraseEnable == 0) && ((SYS0L[SYS0L_GPIO_CTRL1] & BIT25) == 0)) { //falling
					guoCustomerGPIO3Timer = GetRTT_milis();
					gubCustomerGPIO3EraseEnable = 1;
				}
				else if ((gubCustomerGPIO3EraseEnable != 0) && ((SYS0L[SYS0L_GPIO_CTRL1] & BIT25) == BIT25)) { //rising
					gubCustomerGPIO3EraseEnable = 0;
				}
			}
		}
		///////////////////////
		if ((gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_WRITEPROTECT_ABORT)
		        || (gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_WRITEPROTECT_DUMMY)) {

			if (gubGPIOEdgeMode == GPIO_EDGE_MODE_RISING_ERASE) {
				if (((gubIsWriteProtect & BIT_GPIO_WRITEPROTECT) == 0) && ((SYS0L[SYS0L_GPIO_CTRL4] & BIT9) == BIT9)) { //rising
					gubIsWriteProtect |= BIT_GPIO_WRITEPROTECT;
				}
				else if (((gubIsWriteProtect & BIT_GPIO_WRITEPROTECT) == BIT_GPIO_WRITEPROTECT) && ((SYS0L[SYS0L_GPIO_CTRL4] & BIT9) == 0)) { //falling
					gubIsWriteProtect &= ~BIT_GPIO_WRITEPROTECT;
				}
			}
			else {
				if (((gubIsWriteProtect & BIT_GPIO_WRITEPROTECT) == 0) && ((SYS0L[SYS0L_GPIO_CTRL4] & BIT9) == 0)) { //falling
					gubIsWriteProtect |= BIT_GPIO_WRITEPROTECT;
				}
				else if (((gubIsWriteProtect & BIT_GPIO_WRITEPROTECT) == BIT_GPIO_WRITEPROTECT) && ((SYS0L[SYS0L_GPIO_CTRL4] & BIT9) == BIT9)) { //rising
					gubIsWriteProtect &= ~BIT_GPIO_WRITEPROTECT;
				}
			}
		}
		else if ((gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_USER)
		         || (gubCustomerGPIOSelectMode == CUSTOMER_GPIO_MODE_ERASE_ALL)) {
			if (gubGPIOEdgeMode == GPIO_EDGE_MODE_FALLING_ERASE) {
				if ((gubCustomerGPIOEraseEnable == 0) && ((SYS0L[SYS0L_GPIO_CTRL4] & BIT9) == 0)) { //falling
					UBYTE ubflag = 0;
					if (gubQuickEraseWaitTime) {
						U32 ulBefore = GetRTT_milis();
						U32 ulQuickEraseWaitTime = gubQuickEraseWaitTime * 1000;
						while ((SYS0L[SYS0L_GPIO_CTRL4] & BIT9) == 0) {
							U32 ulOperationTime = GetRTT_milis();
							if (ulOperationTime >= ulBefore) {
								if ((ulOperationTime - ulBefore) >= ulQuickEraseWaitTime) {
									ubflag = 1;
									break;
								}
							}
							else {
								if ((0xFFFFFFFF - ulBefore + ulOperationTime) >= ulQuickEraseWaitTime) {
									ubflag = 1;
									break;
								}
							}
						}
					}
					else {
						idle(100);
						if (((SYS0L[SYS0L_GPIO_CTRL4] & BIT9) == 0)) {
							ubflag = 1;
						}
					}
					if (ubflag) { // 因為沒有interrupt 這邊用一小段防呆 避免資料被亂清
					Mark_QuickErase:
						ftlSaveVT(0, VT_QUICK_ERASE_0x94);
						gubCustomerGPIOEraseEnable = 1;
						HandleStopRW(1);
						gubStopFTL = 0;
						HandleSMARTSelfTest();
						Sanitize_Overwrite_Offline();
						gubTimeToDoBG = BYTE_BG_LEVEL_IDLE;
						M_DebugGPIO8_OFF_LED();
						Vender_GPIO_Offline();
						while (1);
					}
				}
				else if ((gubCustomerGPIOEraseEnable != 0) && ((SYS0L[SYS0L_GPIO_CTRL4] & BIT9) == BIT9)) { //rising
					gubCustomerGPIOEraseEnable = 0;
				}
			}
		}
#endif
		/// end of Get Temperature & change auto refresh value

		if (!BURNER) {
		Mark_BG:
			if (gubBackGroundClean_Enable) {
				UBYTE ubExtremeMode = 0;
				if (IDT.W86 & CHK_BIT3) {
					if ((IDT.W91 >= 0x01) && (IDT.W91 <= 0x80)) {
						gulEnterBG_Time_1 = 300; //300ms
						gulEnterBG_Time_2 = 500; //500ms
					}
					else if ((IDT.W91 >= 0x81) && (IDT.W91 <= 0xA0)) {
						gulEnterBG_Time_1 = 500; //500ms
						gulEnterBG_Time_2 = 1000; //1s
					}
					else if ((IDT.W91 >= 0xA1) && (IDT.W91 <= 0xFE)) {
						gulEnterBG_Time_1 = 500; //500ms
						gulEnterBG_Time_2 = 1000; //1s
						ubExtremeMode = 1;
					}
				}
				ulIdle_Counter = gulOperationTime;
				if (ulIdle_Counter < gulBackGround_Counter) {
					ulIdle_Counter += (0xFFFFFFFF - gulBackGround_Counter);
				}
				else {
					ulIdle_Counter -= gulBackGround_Counter;
				}

				if (ENABLE_DEBUG_FORCE_BG_TEST && ENABLE_BACKGROUND_COPY) {
					UBYTE ubRandSeed = (VT->gulGRTableL4kPTR & 3);
					UBYTE ubBGTestGRNum = ubRandSeed;
					if ((VT->gubGRTargetIndex > ubBGTestGRNum) && (VT->gulGRTableL4kPTR > (gul4kEntrysPerUnit >> ubRandSeed)) && (VT->gulSLCPoolSize < BGSLCPoolCopyThreshold)) {
						ulIdle_Counter = gulEnterBG_Time_2 + 1;
					}
				}

				if (gubSSD_Init_State.B.btSetDPSDone == 0) {
#if (ENABLE_DPS)
					DPS_Command(gubZ2Buffer, BIT_DPS_Initial | BIT_DPS_PowerOn_Reset); // function內自己會判斷erase count
#endif
				}

				if ((gubTimeToDoBG == 0) && (ulIdle_Counter > gulEnterBG_Time_1)) {
					ulIdle_Counter2 = 0;
					ubIdle2Enable = 0;
					///after 1SEC wakeup, manual send a DIPM request.
					//HW[HW_PM_CTRL]|=H_REG_PMREQ_P;
					if ((gubDevIPMON == 1) && gubENABLE_FW_DIPM)
					{
						//// HW-DIPM
						//HW[HW_PM_CTRL] |= (H_HW_AUTO_DIPM_EN);
						if ((gubSoftReset == 0) && (gubHardReset == 0) && (gubHostCmdIn == 0) && (HB[HB_REMAIN_VLD_NCQ_NUM] == 0) && ((HL[HL_INT_FLAG] & (H_RCV_CMD_INT | H_SRST_CLR_INT | H_LNK_COMRI_INT)) == 0)) {
							if (gubDIPMP2SEnable) {
								///if ( guwFW_DIPM_DIVIDER == 1) {

								HW[HW_PM_CTRL] |= H_REG_PMREQ_P;

								///	guwFW_DIPM_DIVIDER = 0;
								///}
								///else {
								///	guwFW_DIPM_DIVIDER++;
								///}
							}
							else {
								HW[HW_PM_CTRL] |= H_REG_PMREQ_S;
							}
						}
					}
					M_DebugGPIO8_ON_LED();
					HandleStopRW(0);
					if (ENABLE_DEBUG_FORCE_BG_TEST && ENABLE_BACKGROUND_COPY && (!gubIsWriteProtect)) {
						M_StartBG_Copy();
						gubPseudo_BG_Doing = 1;
						gubPseudo_BG_Stop_Case = VT->gulGRTableL4kPTR & 7; // 8種停掉的狀況
						if (ENABLE_DEBUG_UART_BG) {
							UartString("\nBGStopCase:");
							Uart_Tx_DataHex(gubPseudo_BG_Stop_Case);
						}
						while (gubPseudo_BG_Doing) {
							M_SwitchTask();
						}
						if (ENABLE_DEBUG_UART_BG) {
							UartString("\nBGStopCaseReal:");
							Uart_Tx_DataHex(gubPseudo_BG_Stop_Case);
						}
						gulPseudo_BG_Stop_Case_Num[gubPseudo_BG_Stop_Case] ++;
					}
					else if (gubFTL_Idle && (~(gubDEVSLPEnable && ((HW[HW_SATA_INFO]&H_DEVSLP))))) {
						ulIdle_Counter = gulOperationTime;
						if (ulIdle_Counter < gulBackGround_Counter) {
							ulIdle_Counter += (0xFFFFFFFF - gulBackGround_Counter);
						}
						else {
							ulIdle_Counter -= gulBackGround_Counter;
						}
						if ((ulIdle_Counter < gulEnterBG_Time_2) && gubEnableBG_Idle) {
							//進去 idle 1sec, 有可能被RTT叫醒或是command in
							if (ENABLE_DEBUG_UART_BG) {
								UartString("\nBG1SecIdle");
								idle(20000);
							}
							bSYS0(0x504) = 0x01;       //WIC WU flag before power down
							M_DebugGPIO8_OFF_LED();
							gating_clock(BYTE_GATING_CLOCK_BG_1SEC, (gulEnterBG_Time_2 - ulIdle_Counter));
							gulBackGround_Counter = gulOperationTime;
							if (ENABLE_DEBUG_UART_BG) {
								UartString("  BG1SecWake");
							}
							M_StartBG_Copy();
						}
						else if (ulIdle_Counter > gulEnterBG_Time_2) {
							M_StartBG_Copy();
						}
					}
					if (gubSoftReset | gubHardReset | gubHostCmdIn | HB[HB_REMAIN_VLD_NCQ_NUM] | (HL[HL_INT_FLAG] & (H_RCV_CMD_INT | H_SRST_CLR_INT | H_LNK_COMRI_INT))) {
						M_StopBG_Copy();
						if (HW[HW_SATA_INFO] & (H_SLUMBER | H_PARTIAL)) {
							HW[HW_PM_CTRL] |= H_REG_PMCANCEL;
						}
						goto Mark_Reset;
					}
					else if (gubDEVSLPEnable && (HW[HW_SATA_INFO]&H_DEVSLP)) {
						M_StopBG_Copy();
						gating_clock(BYTE_GATING_CLOCK_BG, 0);
						/* if enable return from DEVSLP mean HOST already send wakeup interrupt while FW doing Flush on DEVSLP Flow,
						   and next line goto Mark_Reset is to reset gulEnterBG_Time_1 to prevent too fast send Partial/Slumber REQ
						*/
						goto Mark_Reset;
					}

				}
				else if (gubEnableBG_Idle && (gubTimeToDoBG == BYTE_BG_LEVEL_IDLE)) {
					if (gubDevIPMON && gubDIPMP2SEnable) {
						if (ulIdle_Counter2 == 0 ) {
							ulIdle_Counter2 = gulOperationTime;
						}
						///=========================================================
						if ((0xFFFFFFFF - ulIdle_Counter2) < gubAP2SWaitIdle2Time) {
							if (((0xFFFFFFFF - ulIdle_Counter2 + gulOperationTime) > gubAP2SWaitIdle2Time) && ( ulIdle_Counter2 > gulOperationTime )) {
								if (ENABLE_DEBUG_UART) {
									UartString("\r\nIdle2 wait over 100ms");
								}
								ubIdle2Enable = 1;
							}
							if ((HW[HW_SATA_INFO] & (H_SLUMBER)) ) {
								ubIdle2Enable = 1;
							}
						}
						else {
							if ((gulOperationTime - ulIdle_Counter2) > gubAP2SWaitIdle2Time) {
								if (ENABLE_DEBUG_UART) {
									UartString("\r\nIdle2 wait over 100ms");
								}
								ubIdle2Enable = 1;
							}
							if ((HW[HW_SATA_INFO] & (H_SLUMBER)) ) {
								ubIdle2Enable = 1;
							}

						}
					}
					else {
						ubIdle2Enable = 1;
					}

					if (ubIdle2Enable) {
						// 進去 idle,  Command in & ?? 叫醒
						ulIdle_Counter2 = 0;
						ubIdle2Enable = 0;
						bSYS0(0x504) = 0x01;       //WIC WU flag before power down
						if (ENABLE_DEBUG_UART_BG) {
							UartString("\nBGIdle SLCPool:");
							UartLongHex(VT->gulSLCPoolSize);
							UartString(" FreeUnit:");
							UartWordHex(VT->guwFreeBlockCount_D3);
							idle(80000);
						}
						M_StopBG_Copy();

						if (((gubOfflineMode & SMART_M_TIMERENABLE) && ((gubOfflineMode & SMART_M_SUSPEND) == 0))
						        || (AtaCfg.gSMARTSelective.ubOffLineDoing && ((AtaCfg.gSMARTSelective.ubOffLineDoing & SELECTIVE_OFFLINESUSPEND) == 0))
						        || (AtaCfg.ubSanitizeState == SD2_SANITIZE_IN_PROGRESS)
						        || (gubCustomerGPIO3EraseEnable == 1)) {

							HandleSMARTSelfTest();
							Sanitize_Overwrite_Offline();
							Vender_GPIO_Offline();

						}
						else {
							M_DebugGPIO8_OFF_LED(); // 睡覺前記得關燈喔
							gulReadZeroDataCnt = 0;
							gating_clock(BYTE_GATING_CLOCK_BG, 0);
							gulBackGround_Counter = gulOperationTime;
							if (ENABLE_DEBUG_UART_BG) {
								UartString("  BGIdleWake");
							}
							goto Mark_Reset;
						}
					}
					else {
						goto Mark_Reset;
					}
				}
				else if (gubTimeToDoBG) { // 只是為了亮燈
					M_DebugGPIO8_ON_LED();
				}
			}
			else {
				HandleSMARTSelfTest();
				Sanitize_Overwrite_Offline();
				Vender_GPIO_Offline();
			}
		}
	}
#endif
}

void HandleStopRW(UBYTE ubStopMode) { // 0 : Flush Data into flash 1: FTL idle
	if (gubPreformat || (BURNER) || (gubSSD_Init_State.B.ubState < BYTE_INIT_SCAN_DONE)) {
		return;
	}

#if(!RDT)
	if (ENABLE_KEEP_WBQ) {
		if (gubKeepWBQNum || gubKeepLoadAlignmentBQNum) {
			gulWriteBufferBitMap = 0;
			UBYTE ubi;
			for (ubi = 0; ubi < 32; ubi++) {
				gubWriteBufferTableHead[ubi] = Default_WB_Index;
				gubWriteBufferTableNext[ubi] = Default_WB_Index;
			}
		}
		if (gubKeepWBQNum) {
			gBQI.ubLinkNum += gubKeepWBQNum;
			gBQI.ubWLinkNum += gubKeepWBQNum;
			gubKeepWBQNum = 0;
			gubKeepWBQSectorCnt = 0;
			gubKeepWBQ4KCnt = 0;
			if (gubKeepLoadAlignmentBQNum) {
				gubFirstKeepBQLink = gubKeepLoadAlignmentBQLink;
				gBQI.ubLinkFTLLast = gBQI.BufferQueue[gubFirstKeepBQLink].ubPrevious;
			}
			else {
				gBQI.ubLinkFTLLast = gBQI.ubLinkLastUse;
			}
		}
	}
	if (gubKeepLoadAlignmentBQNum) {
		gBQI.ubLinkNum += 2;
		gBQI.ubWLinkNum++;
		gBQI.ubLinkFTLLast = gBQI.ubLinkLastUse;
		gubKeepLoadAlignmentBQLink = 0xFF;
		gubKeepLoadAlignmentBQNum = 0;
	}
	//UWORD uwFlagSetTemp = AXIW[AXIW_FLAG_SET];
	if (ENABLE_DDR_MODE && guwDataInSDR4kNum) {
		FlushSDR();
	}
	else if (gBQI.ubLinkNum) {
		gBQI.ulHostWriteWaitBufferFlag = 0xFFFFFFFF;
	}
	gubSeqR = 0;
	if (gubPreRead) {
		gubPreReadCheckCount = 0;
		while (gBQI.ubWLinkNum || gFQI.ubFQLinkNumber_W || gubLoadAlignDoing) {
#if(!RDT)
			M_SwitchTask();
#endif
		}
		gubClearPreRead = 1;
		M_SetFlagSettingNone();
		AXIL[AXIL_SET_START_FLAG] = 0;
		AXIL[AXIL_SET_FLAG_LENGTH] = gulBuffer2ReadNum;
		AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;      //clear all buffer flag
		guoFWRead4kBufferFlag = 0;
	}
	_DEBUG_TRACE((0xFE << 24) | (0x8 << 16) | gBQI.uwWrite_tq_2_bq);
#if ENABLE_PFAIL
	gubFTLNoWait = (gBQI.ubWrite_TQ_lock & POWER_FAIL_TQ_LOCK) ? 1 : 0; /* When power fail, no need to wait for cache read/write to save time. */
#endif
	while (gBQI.ubLinkNum || gFQI.ubFQLinkNumber || ((VT->gulFTLState.B.btBG_Copy || VT->gulFTLState.B.btBG_RetentionRead) && (!BURNER))) {
#if(!RDT)
		M_SwitchTask();
#endif
	}
#if ENABLE_PFAIL
	gubFTLNoWait = 0;
#endif
	gBQI.ulHostWriteWaitBufferFlag = 0x0UL;

	gubPreRead = 0;
	gulPreReadTriggerSectorCnt = 0;
	M_Clear_UNC_Addr();
	if (ubStopMode && (!BURNER) && (gubSSD_Init_State.B.ubState >= BYTE_INIT_SCAN_DONE)) {
		gubWearLevelingCount = MAX_WearLeveling_Num;
		if (gubStopFTL != BYTE_STOP_FTL_DONE) {
			gubStopFTL = BYTE_STOP_FTL_REQUEST;
		}
		while ((gubStopFTL != BYTE_STOP_FTL_DONE) || gFQI.ubFQLinkNumber) {
#if(!RDT)
			M_SwitchTask();
#endif
		}
	}
	mDMAC_SV(4, (U32)gubBuffer2, 0, 0, 0, 0, DMA_EN_SETCQ, 0);//強迫DMAC 全部做完
	//AXIW[AXIW_FLAG_SET] = uwFlagSetTemp;
	M_SetFlagSettingSHF();
	gubClearPreRead = 0;
#endif
}
void SaveRSparity(UBYTE ubReEnableRSPreSaveLoad) {
	M_CheckBurnerModeToDisableFunction();
	while (RSL[RSL_TRIG] & (RS_ENC_BSY | RS_TRIG_BSY)); // check encode done, and all MT should be abort
	// no anyone will access PBUF
	while (RSL[RSL_TAG_VLD_CTL] & RS_PRE_SAVE_LOAD_BUSY);
	RSL[RSL_PBUF_CTL] &= (~RS_RRE_SAVE_LOAD_EN);

	// set BPT
	RSL[RSL_MODE_0] &= (~RS_BPT_SEL);
	RSL[RSL_BP_CONF] |= RS_ALL_CONT;
	// Force save RS parity from PBUF to DRAM
	volatile U32 * pBPT = RSL_BPT_BASE_0;
	UBYTE ub4K_IDX = 0;
	for (ub4K_IDX = 0; ub4K_IDX < gub4kEntrysPerPlane; ub4K_IDX++) {
		pBPT[((RSB[RSB_ROW_IDX]) * 4 + ub4K_IDX)] = 0; // fix BPTRAM parity error
		pBPT[((RSB[RSB_ROW_IDX] + 1) * 4 + ub4K_IDX)] = 0; // fix BPTRAM parity error
		pBPT[((RSB[RSB_DPAGE_NUM]) * 4 + ub4K_IDX)] = (ub4K_IDX) * 4096;
		pBPT[((RSB[RSB_DPAGE_NUM] + 1) * 4 + ub4K_IDX)] = (ub4K_IDX + 4) * 4096;
	}

	U32 ulSpareAddr = 0;
	if (RSL[RSL_PBUF_CTL] & RS_PBUF0_Status) {
		UBYTE ubtag = (RSL[RSL_PBUF_CTL] & RS_PBUF0_RS_TAG_NUM);
		ulSpareAddr = (U32)(&gulRSFrameSpareBits[(ubtag * 128) / 4]);
		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate((void *)ulSpareAddr, 128);
		}
		RSL[RSL_DBASE0] = (U32)(&guoRSFrameBits[(ubtag * 32 * 1024) / 8]);
		RSL[RSL_SBASE0] = ulSpareAddr;
		RSL[RSL_PBUF_CTL] |= RS_FORCE_SAVE_RS_PBUF0;
		while (RSL[RSL_PBUF_CTL] & RS_FORCE_SAVE_RS_PBUF0);
		while (RSL[RSL_PBUF_CTL] & RS_PBUF0_Status);
	}
	if (RSL[RSL_PBUF_CTL] & RS_PBUF1_Status) {
		UBYTE ubtag = (RSL[RSL_PBUF_CTL] & RS_PBUF1_RS_TAG_NUM) >> 8;
		ulSpareAddr = (U32)(&gulRSFrameSpareBits[(ubtag * 128) / 4]);
		if (ENABLE_DCACHE) {
			xthal_dcache_region_invalidate((void *)ulSpareAddr, 128);
		}
		RSL[RSL_DBASE0] = (U32)(&guoRSFrameBits[(ubtag * 32 * 1024) / 8]);
		RSL[RSL_SBASE0] = ulSpareAddr;
		RSL[RSL_PBUF_CTL] |= RS_FORCE_SAVE_RS_PBUF1;
		while (RSL[RSL_PBUF_CTL] & RS_FORCE_SAVE_RS_PBUF1);
		while (RSL[RSL_PBUF_CTL] & RS_PBUF1_Status);
	}
	if (ubReEnableRSPreSaveLoad) {
		RSL[RSL_PBUF_CTL] |= RS_RRE_SAVE_LOAD_EN;
	}

	//switch back for RS parity program
	RSL[RSL_DBASE0] = (U32)(&guoRSFrameBits[0]);
	RSL[RSL_SBASE0] = (U32)(&gulRSFrameSpareBits[0]);
}

void HandleSMARTSelfTest() {
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	if (((gubOfflineMode & SMART_M_TIMERENABLE) && ((gubOfflineMode & SMART_M_SUSPEND) == 0))
	        || (AtaCfg.gSMARTSelective.ubOffLineDoing && ((AtaCfg.gSMARTSelective.ubOffLineDoing & SELECTIVE_OFFLINESUSPEND) == 0))) {

		SMART_RealScanLBA();
	}

#endif
}

UBYTE UpdateSystemBlock(UBYTE ubReturnFailInfo, U32 ulSystemBlockAddr) {
	UBYTE *SystemBlockBuffer = (UBYTE *)ulSystemBlockAddr;
	UBYTE ubCEIndex, ubCodeNum, ubGetSystemSet;
	U32 uli, ulLCA_temp;
	UBYTE ubj;
	UBYTE ubOldSystemUnit, ubOldSystemBurst,  ubSystemUnit, ubSystemBurst;
	UBYTE ubOldSystemSet = gubSystemSet;
	UBYTE ubSystemNewSet;
	UBYTE ubEraseFail = FALSE;
	UBYTE ubFailType = NORMAL;

	if (RDT_BURNER) {
		ulLCA_temp = MARK_SYS_RDT;
	}
	else {
		ulLCA_temp = MARK_SYS;
	}

	do {
		ubGetSystemSet = TRUE;
		if (gubSystemSetFreeCount > 0) {
			ubSystemNewSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_TAIL);
			//generateHeader是自己去scan block去找可以寫的地方,不從free Q拿, 合理來說會scan到Systemfree Q位子比較前面的free set
			//所以這邊要檢查freeSet是否已經被拿去寫Code Block
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
				for (ubCodeNum = 0; ubCodeNum < 2 ; ubCodeNum++) {
					if (guwSysCodeBlock[ubCodeNum][ubCEIndex] != 0xFFFF) {
						if (guwSysCodeBlock[ubCodeNum][ubCEIndex] == guwSystemBlockRemappingTable[ubSystemNewSet][ubCEIndex])
							ubGetSystemSet = FALSE;//這組Set是CodeBlock, 再找下一個
					}
				}
			}
		}
		else {
			if (ubReturnFailInfo == Return_FailInfo) {
				ftlPreformatShowError(SystemArea_Error_SystemSet, 0, 0x41);//拿光了
			}
			return TERMINAL_FAIL;
		}
	}
	while (!ubGetSystemSet);

	//拿到freeSet了 先用ubSystemNewSet記下來,確定完成後再放進gubSystemSet
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		ubSystemUnit = guwSystemBlockRemappingTable[ubSystemNewSet][ubCEIndex] >> gubBurstsPerBankLog;
		ubSystemBurst = guwSystemBlockRemappingTable[ubSystemNewSet][ubCEIndex] & gubBurstsPerBankMask;
		if (ftlWriteSinglePlane(ubSystemUnit, 0, ubCEIndex, (U32)&SystemBlockBuffer[0], ulLCA_temp, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
			if (ubReturnFailInfo == Return_FailInfo) {
				ftlPreformatShowError(ubCEIndex, ubSystemUnit, 0x12);
				SetFailCEtoSystemInfo(ubCEIndex);
			}
			ubFailType = LATERBAD_FAIL;
			goto MARK_RETURN_FAIL;
		}
	}

	for (uli = 1; uli <  3; uli ++) {
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
			ubOldSystemUnit = guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] >> gubBurstsPerBankLog;
			ubOldSystemBurst = guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] & gubBurstsPerBankMask;
			if (ftlReadSinglePlane(ubOldSystemUnit, uli, ubCEIndex, (U32)&SystemBlockBuffer[0], MARK_DPST,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubOldSystemBurst)) {
				if (ubCEIndex == gubPlanesPerBurst) {
					if (ubReturnFailInfo == Return_FailInfo) {
						ftlPreformatShowError(SystemArea_Error_SystemSet, ubOldSystemSet, 0xC);
						for (ubj = 0; ubj < gubPlanesPerBurst; ubj ++) {
							SetFailCEtoSystemInfo(ubj);
						}
					}
					ubFailType = TERMINAL_FAIL;
					goto MARK_RETURN_FAIL;
				}
			}
		}
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
			ubSystemUnit = guwSystemBlockRemappingTable[ubSystemNewSet][ubCEIndex] >> gubBurstsPerBankLog;
			ubSystemBurst = guwSystemBlockRemappingTable[ubSystemNewSet][ubCEIndex] & gubBurstsPerBankMask;
			if (ftlWriteSinglePlane(ubSystemUnit, uli, ubCEIndex, (U32)&SystemBlockBuffer[0], MARK_DPST, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
				if (ubReturnFailInfo == Return_FailInfo) {
					ftlPreformatShowError(ubCEIndex, ubSystemUnit, 0x12);
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				ubFailType = LATERBAD_FAIL;
				goto MARK_RETURN_FAIL;
			}
		}
	}

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		ubSystemUnit = guwSystemBlockRemappingTable[ubSystemNewSet][ubCEIndex] >> gubBurstsPerBankLog;
		ubSystemBurst = guwSystemBlockRemappingTable[ubSystemNewSet][ubCEIndex] & gubBurstsPerBankMask;

#if Hynix
		ubOldSystemUnit = guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] >> gubBurstsPerBankLog;
		ubOldSystemBurst = guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] & gubBurstsPerBankMask;

		for (uli = 3; uli <  5; uli ++) {
			if (ftlReadSinglePlane(ubOldSystemUnit, uli, ubCEIndex, (U32)&SystemBlockBuffer[0], MARK_SYS_HYNIX_RRT, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubOldSystemBurst)) {
				if (ubReturnFailInfo == Return_FailInfo) {
					ftlPreformatShowError(ubCEIndex, ubOldSystemUnit, 0xC);
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				ubFailType = TERMINAL_FAIL;
				goto MARK_RETURN_FAIL;
			}
			if (ftlWriteSinglePlane(ubSystemUnit, uli, ubCEIndex, (U32)&SystemBlockBuffer[0], MARK_SYS_HYNIX_RRT, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
				if (ubReturnFailInfo == Return_FailInfo) {
					ftlPreformatShowError(ubCEIndex, ubSystemUnit, 0x12);
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				ubFailType = LATERBAD_FAIL;
				goto MARK_RETURN_FAIL;
			}
		}
		for (uli = 5; uli <  guwFastPagePagesPerUnit; uli ++) {//write dummy
			if (ftlWriteSinglePlane(ubSystemUnit, uli, ubCEIndex, (U32)&SystemBlockBuffer[0], ulLCA_temp, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
				if (ubReturnFailInfo == Return_FailInfo) {
					ftlPreformatShowError(ubCEIndex, ubSystemUnit, 0x12);
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				ubFailType = LATERBAD_FAIL;
				goto MARK_RETURN_FAIL;
			}
		}
#else
		for (uli = 3; uli <  guwFastPagePagesPerUnit; uli ++) {//write dummy
			if (ftlWriteSinglePlane(ubSystemUnit, uli, ubCEIndex, (U32)&SystemBlockBuffer[0], ulLCA_temp, gulSystemVersion, (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS), 4, 0, ubSystemBurst)) {
				if (ubReturnFailInfo == Return_FailInfo) {
					ftlPreformatShowError(ubCEIndex, ubSystemUnit, 0x12);
					SetFailCEtoSystemInfo(ubCEIndex);
				}
				ubFailType = LATERBAD_FAIL;
				goto MARK_RETURN_FAIL;
			}
		}
#endif
	}

	//新的SystemSet寫進去了, 把舊的幹掉
	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
		ubOldSystemUnit = guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] >> gubBurstsPerBankLog;
		ubOldSystemBurst = guwSystemBlockRemappingTable[ubOldSystemSet][ubCEIndex] & gubBurstsPerBankMask;
		if (ftl_Erase_Single_Plane((U32)ubOldSystemUnit, ubOldSystemBurst, ubCEIndex, BIT_FJOBI_FASTPAGE)) {
			ubEraseFail = TRUE;
		}
	}
	if (ubEraseFail == FALSE) { //沒有erase fail才放回free Q
		ftlAddToSystemSetFreeTable(ubOldSystemSet);
	}

	gubSystemSet = ubSystemNewSet;
	guwSystemBlock[0] = guwSystemBlockRemappingTable[ubSystemNewSet][0];
	guwSystemBlock[1] = guwSystemBlockRemappingTable[ubSystemNewSet][1];
	return NORMAL;

MARK_RETURN_FAIL:
	for (ubj = 0; ubj < gubPlanesPerBurst; ubj ++) {
		ubSystemUnit = guwSystemBlockRemappingTable[ubSystemNewSet][ubj] >> gubBurstsPerBankLog;
		ubSystemBurst = guwSystemBlockRemappingTable[ubSystemNewSet][ubj] & gubBurstsPerBankMask;
		ftl_Erase_Single_Plane((U32)ubSystemUnit, ubSystemBurst, ubj, BIT_FJOBI_FASTPAGE);
	}
	return ubFailType;
}

void TransformSystemBlock(U32 ulAddr) {
	UBYTE ubUpdateFail, ubCEIndex, ubi, ubj;
	UWORD *uwSysBlockBuffer = (UWORD *)(ulAddr);
	UBYTE *ubSysBlockBuffer = (UBYTE*)(ulAddr);
	gulSystemVersion = (gulSystemVersion & 0xFFFFFFF0) + ((gulSystemVersion + 1) & 0x0000000F);//ex: 0x112233 -> 0x112234, 0x11223F->0x112230

	for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
		UBYTE ubOldSystemUnit = guwSystemBlock[ubCEIndex] >> gubBurstsPerBankLog;
		UBYTE ubOldSystemBurst = guwSystemBlock[ubCEIndex] & gubBurstsPerBankMask;
		if (ftlReadSinglePlane(ubOldSystemUnit, 0, ubCEIndex, (U32)&uwSysBlockBuffer[0], MARK_SYS,  (BIT_FJOBI_FASTPAGE | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY), 4, 0, ubOldSystemBurst) == 0) {
			break;//success
		}
		else {
			if (ENABLE_WRITEPROTECT) {
				gubIsWriteProtect |= BIT_FLASH_INIT_FAIL_WRITEPROTECT;
				return;
			}
			else {
				flaLEDBlink(0);
			}//SystemBlock拉不起來...
		}
	}

	if (gubTranslateMixPlaneMark == TRUE) {
		SYSTEM_GROUP_t *InfoArray = (SYSTEM_GROUP_t *)(&ubSysBlockBuffer[M_SysInfo_InfoBlk]);
		if (InfoArray->ubRUTMixPlaneEnable == 0x01) {
			gubRUTMixPlaneEnable = 1;
			InfoArray->ubRUTMixPlaneEnable = 0x50;
			//為mixplane Mark定義從0x01改成0x50,在DLMC時,為了要相容舊版, 0x01轉換成0x50
		}
	}

	if (gubTransformSystemUnitToSystemSet == TRUE) {
		uwSysBlockBuffer[(M_SysInfo_SystemAreaNumBlock_CH0 / 2)] = guwSystemAreaNum[0];
		uwSysBlockBuffer[(M_SysInfo_SystemAreaNumBlock_CH1 / 2)] = guwSystemAreaNum[1];
		ubSysBlockBuffer[M_SysInfo_SystemArea_RemappingRule] = Rule_UnitPlaneA;//一定走舊rule
		ubSysBlockBuffer[M_SysInfo_DBT_SET] = gubDBTSet;//DBT Unit轉DBT Set

		for (ubi = 0; ubi < SYSTEM_SETS; ubi++) { //RemappingTable
			for (ubj = 0; ubj < MAX_CHANNEL; ubj++) {
				uwSysBlockBuffer[(M_SysInfo_SystemBlockRemapping_Set0_CH0 / 2) + (ubi * MAX_CHANNEL) + ubj] = guwSystemBlockRemappingTable[ubi][ubj];
			}
		}
	}

	do {
		ubUpdateFail = UpdateSystemBlock(Not_Return_FailInfo, (U32)&uwSysBlockBuffer[0]);
	}
	while (ubUpdateFail == LATERBAD_FAIL);

	if (ubUpdateFail == TERMINAL_FAIL) {
		if (ENABLE_WRITEPROTECT) {
			gubIsWriteProtect |= BIT_FLASH_INIT_FAIL_WRITEPROTECT;
		}
		else {
			flaLEDBlink(0);
		}
	}
}

void ScanSystemFreeSet(UBYTE ubMode) {
#if(!RDT)
	M_CheckRDTModeToDisableFunction();
	M_CheckBurnerModeToDisableFunction();

	U32 ulFreeblock, uli;
	UBYTE ubSystemSetPTR;
	UBYTE ubPlaneBank;
	UBYTE ubCEIndex;
	UBYTE ubBadBlock;
	UBYTE ubi;
	UBYTE ubDoContinue;

	U32 ulEntryPerDBTPlane = (U32)gubSectorsPerPlane * 512; // 一個plane存了幾個block state......

	//reference by DBT
	UBYTE *ubDBTBuffer = (UBYTE *)(gubZ2Buffer + 0x4000);

	if (ubMode == 0) {
		gubSystemSetFreeCount = 0;
		gubSystemSetFreeQueueInPTR = 0;
		gubSystemSetFreeQueueOutPTR = 0;
	}

	for (ubSystemSetPTR = 0; ubSystemSetPTR < SYSTEM_SETS; ubSystemSetPTR++) {//根據remapping table去掃
		ubBadBlock = 0;

		if (gubSystemSetFreeCount == SYSTEM_FREE_SETS) {
			//DLMC to new code with planeB system, guwSystemAreaNum定義變成掃block不是掃unit， 變大後造成後面會save dummy跟erase，power on 可能會timeout.
			//這邊卡system free ount到4個後就break不需再掃，後面也就不會再save dummy跟erase。
			//其實這邊是可以掃超過4個再break，但8個Terry說俊華會有從第9個遞補進來的問題，所以還是照一般case，即維持4個free。
			//此外若是system區的power cycle處理完也會重新ISP jump，因此卡4個是ok的。

			break;
		}

		if ((gubSystemSet != 0xFF) && (ubSystemSetPTR == gubSystemSet)) {
			continue;
		}

		if ((gubDBTSet != 0xFF) && (ubSystemSetPTR == gubDBTSet)) {
			continue;
		}

		if ((gubErrorDumpSet != 0xFF) && (ubSystemSetPTR == gubErrorDumpSet)) {
			continue;
		}

		for (ubCEIndex = 0 ; ubCEIndex < gubPlanesPerBurst ; ubCEIndex++) {
			if (guwSystemBlockRemappingTable[ubSystemSetPTR][ubCEIndex] >= guwSystemAreaNum[ubCEIndex]) {
				continue;
			}
		}

		if (ubMode) {
			if (((gubSysCodeSet[0] != 0xFF) && (ubSystemSetPTR == gubSysCodeSet[0])) || ((gubSysCodeSet[1] != 0xFF) && (ubSystemSetPTR == gubSysCodeSet[1]))) {
				continue;
			}

			ubDoContinue = 0;//確認此free block是否已經加入systemtable了
			for (ubi = 0; ubi < SYSTEM_SETS; ubi++) {
				if ((gubSystemSetFreeTable[ubi] != 0xFF) && (ubSystemSetPTR == gubSystemSetFreeTable[ubi])) {
					ubDoContinue = 1;
					break;
				}
			}

			if (ubDoContinue) {
				continue;
			}
		}

		for (ubi = 0 ; ubi < gubPlanesPerBurst ; ubi++) {
			//ubPlaneBank = (ubi << gubBurstsPerBankLog) + (uwBlockPTR & gubBurstsPerBankMask);
			UBYTE uwBlock = guwSystemBlockRemappingTable[ubSystemSetPTR][ubi];
			ubPlaneBank = (ubi << gubBurstsPerBankLog) + (uwBlock & gubBurstsPerBankMask);
			ubCEIndex = (ubPlaneBank >> gubBurstsPerBankLog) & gubCEsPerPageMask;

			if (ubMode && gubErrorDumpSet == 0xFF) { //有Error Block時不做Erase, 不然有可能將兩個Error Block其中之一給Erase掉
#if FixShallowErase //Erase Plane-A for CE0 & CE1
				ftlForceSaveDummy(ubPlaneBank, (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div)), (uwBlock >> gubBurstsPerBankLog), (FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE | FORCE_SAVE_DUMMY_DIRECT_ACCESS | FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE), D3_AREA, VT_FORCE_DUMMY_0xD7);
#endif
				if (ftl_Erase_Single_Plane((uwBlock >> gubBurstsPerBankLog), ubPlaneBank, ubCEIndex, BIT_FJOBI_FASTPAGE))  { // Erase fail will return 1
					ubBadBlock |= BIT7;
					break;
				}
			}

			ulFreeblock = (M_Multiplier(((uwBlock >> gubBurstsPerBankLog) << gubTotalGroupsLog), DEF_PlanesPerPage_Mcl_Div));
			uli = ulFreeblock % ulEntryPerDBTPlane;                                     //該PlaneBank放在這個DBT plane的第幾個Byte
			ubBadBlock |= ubDBTBuffer[uli + ubPlaneBank];

#if MicronFlashOnly
			//如果這個Block是CodeBlock就會判成bad block,並且不會放進freeQ
			ubBadBlock |= ftlGoodBlock((uwBlock >> gubBurstsPerBankLog), ubPlaneBank, (U32)gubZ2Buffer, 1, GOODBLK_SLC);
			ubBadBlock |= ftlGoodBlock((uwBlock >> gubBurstsPerBankLog), ubPlaneBank, (U32)gubZ2Buffer, 1, GOODBLK_MLC);
#elif ENABLE_TLC_PROGRAM_CODEBLOCK
			ubBadBlock |= ftlGoodBlock((uwBlock >> gubBurstsPerBankLog), ubPlaneBank, (U32)gubZ2Buffer, 1);
			ubBadBlock |= ftlGoodBlock((uwBlock >> gubBurstsPerBankLog), ubPlaneBank, (U32)gubZ2Buffer, 2);
#else
			ubBadBlock |= ftlGoodBlock((uwBlock >> gubBurstsPerBankLog), ubPlaneBank, (U32)gubZ2Buffer, 1);
#endif
			if (ENABLE_DEBUG_UART_FLH_INITIAL) {
				UartString("\r\nGoodBlock: ");
				Uart_Tx_DataHex(ubBadBlock);
			}
		}

		if (ubBadBlock == 0) {
			ftlAddToSystemSetFreeTable(ubSystemSetPTR);
		}
	}
#endif
}

void HandleCodeSet() {
#if(!RDT)
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();

	UWORD uwBlock;
	UBYTE ubSystemSetPTR;
	UBYTE ubCEIndex, ubL4KNum;
	L4KTable16B *pL4KTablePtr;
	L4KTable16B pL4KTable;
	UWORD ubCodeSet_FirstScan[MAX_CODEUNIT_NUMBER];
	UBYTE ubCodeVersion[MAX_CODEUNIT_NUMBER];
	UWORD uwPlaneNum[MAX_CODEUNIT_NUMBER];
	UBYTE ubTotalCodeSetNumber = 0;
	FlashQueue_t *FQ;
	UBYTE ubDepth, ubL4kLink;
	UBYTE ubi;
	U32 ulPlaneIndex;
	UBYTE ubSecondCodeSet = 0xFF;
	UBYTE ubL4KCount;
	U32 ulLDPC_CFG_Temp;
	UBYTE ubEraseCodeSetNumber;
	U32 ulFCONL_FLH_FUNC_Temp;
	UBYTE ubNeedScan;
	UBYTE ubNewCodeVersion = 0;
	UBYTE ubOldCodeVersion = 0;
	UBYTE ubNewCodeIndex = 0xFF;
	UBYTE ubOldCodeIndex = 0xFF;
	UBYTE ubUsedCodeIndex = 0xFF;
	UBYTE ubLastNewCodeIndex;
	UWORD uwCodeVersionMap = 0;
	UBYTE ubNeedISPJump = 0;
	U32 ulL4K_LCA_Clone;
	U32 ulL4K_FW_Clone;
	//UBYTE ubUsingCodeVersionCnt = 0;

	for (ubi = 0; ubi < MAX_CODEUNIT_NUMBER; ubi++) {
		ubCodeSet_FirstScan[ubi] = 0xFF;
		ubCodeVersion[ubi] = 0xFF;
		uwPlaneNum[ubi] = 0;
	}

	ulFCONL_FLH_FUNC_Temp = FCONL[FCONL_FLH_FUNC];
#if !ENABLE_TLC_PROGRAM_CODEBLOCK2
	FCONL[FCONL_FLH_FUNC] |= RDM_REF_ROW_ADR_5_TO_0;
#endif
	ubL4KNum = 1;
	ubCEIndex = 0;
	ulLDPC_CFG_Temp = FCONL[FCONL_LDPC_CFG];

	FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
	FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);


	//for (uwBlockPTR = 0; uwBlockPTR < guwSystemAreaNum; uwBlockPTR++) {
	for (ubSystemSetPTR = 0; ubSystemSetPTR < SYSTEM_SETS; ubSystemSetPTR++) {
		ubNeedScan = 1;

		if ((gubSystemSet != 0xFF) && (ubSystemSetPTR == gubSystemSet)) {
			ubNeedScan = 0;
		}

		if ((gubDBTSet != 0xFF) && (ubSystemSetPTR == gubDBTSet)) {
			ubNeedScan = 0;
		}

		if ((gubErrorDumpSet != 0xFF) && (ubSystemSetPTR == gubErrorDumpSet)) {
			ubNeedScan = 0;
		}

		for (ubi = 0; ubi < SYSTEM_SETS; ubi++) {//在freetable中就不用掃了
			if ((gubSystemSetFreeTable[ubi] != 0xFF) && (ubSystemSetPTR == gubSystemSetFreeTable[ubi])) {
				ubNeedScan = 0;
				break;
			}
		}

		if (ubNeedScan) {
			for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex ++) {
				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ulRAMAddr = (U32)&gubZ2Buffer[0];
				FQ->btCheckEmpty = 1;
				FQ->btThisIsErasePage = 0;
				FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
				FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#endif
				uwBlock = guwSystemBlockRemappingTable[ubSystemSetPTR][ubCEIndex];
				FQ->uwFUnit = (uwBlock >> gubBurstsPerBankLog);
				FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + (uwBlock & gubBurstsPerBankMask)) << gub4kEntrysPerPlaneLog);
				FQ->ubDepth = ubDepth;
				FQ->ubL4kIndex = ubL4kLink;
				FQ->btUNServed = 1;
#if TLC
				FQ->btD3 = 1;
#endif
				gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + ((ubL4kLink * L4kQ_BIND_FACTOR) * Spare4kSize);
				gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
				gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
				gulPara0x24.BitMap.btZipBypass = 1;
				gulPara0x24.BitMap.btCmpEn = 0;
				gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
				gulPara0x24.BitMap.btBufModeEn = 0;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;
				gMTQ->Depth[ubDepth].ulZipMap = 0xFF;

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
				if (pL4KTablePtr->ulL4K_LCA == HEADER_LCA) {
					ubCodeSet_FirstScan[ubTotalCodeSetNumber] = ubSystemSetPTR;
					ubCodeVersion[ubTotalCodeSetNumber] = (UBYTE)(pL4KTablePtr->Para0x04.BitMap.ulL4K_FW >> 8);
					uwCodeVersionMap |= (BIT0 << (ubCodeVersion[ubTotalCodeSetNumber] & 0x0F));

					//get how many page FW code use
					uwPlaneNum[ubTotalCodeSetNumber] = (UWORD)(1 + gubZ2Buffer[HEADERW_S0_PAGES << 1] + gubZ2Buffer[HEADERW_S1_PAGES << 1] + gubZ2Buffer[HEADERW_S2_PAGES << 1] + gubZ2Buffer[HEADERW_S3_PAGES << 1] + gubZ2Buffer[HEADERW_S4_PAGES << 1] + gubZ2Buffer[HEADERW_S5_PAGES << 1] + gubZ2Buffer[HEADERW_S6_PAGES << 1] + gubZ2Buffer[HEADERW_S7_PAGES << 1]);

					ubTotalCodeSetNumber++;
					break;//換下一個set
				}
			}
		}
	}

	if (ubTotalCodeSetNumber == 0) { //use ICE to initial card has no code unit
		FCONL[FCONL_FLH_FUNC] = ulFCONL_FLH_FUNC_Temp;
		FCONL[FCONL_LDPC_CFG] = ulLDPC_CFG_Temp;
		return;
	}
	//***************************************************************************************************************
	// find the newest code version
	//***************************************************************************************************************
	for (ubi = 0; ubi < MAX_CODEVERSION_NUM; ubi++) {
		if ((((uwCodeVersionMap >> ubi) & BIT0) == BIT0) && (((uwCodeVersionMap >> ((ubi + 1) & 0x0F)) & BIT0) == 0)) {
			ubNewCodeVersion = 0xA0 + ubi;
			break;
		}
	}

	for (ubi = 0; ubi < MAX_CODEVERSION_NUM; ubi++) {
		if ((((uwCodeVersionMap >> ubi) & BIT0) == 0) && (((uwCodeVersionMap >> ((ubi + 1) & 0x0F)) & BIT0) == BIT0)) {
			ubOldCodeVersion = 0xA0 + ((ubi + 1) & 0x0F);
			break;
		}
	}

	for (ubi = 0; ubi < ubTotalCodeSetNumber; ubi++) {
		if (ubCodeVersion[ubi] == ubNewCodeVersion) {
			ubLastNewCodeIndex = ubi; //ubLastNewCodeIndex is the last scanned new code unit.

			if (ubNewCodeIndex == 0xFF) {
				ubNewCodeIndex = ubi; //ubNewCodeIndex is the first scanned new code unit
			}
		}

		if (ubCodeVersion[ubi] == ubOldCodeVersion) {
			if (ubOldCodeIndex == 0xFF) {
				ubOldCodeIndex = ubi;
			}
		}
	}

	for (ubi = 0; ubi < MAX_CODEVERSION_NUM; ubi++) {
		if (ubi < (MAX_CODEVERSION_NUM - 1)) {
			if ( (uwCodeVersionMap & ((BIT0 | BIT1) << ubi)) == ((BIT0 | BIT1) << ubi) ) {
				ubNeedISPJump = 1;
				break;
			}
		}
		else {
			if ( (uwCodeVersionMap & (BIT0 | BIT15)) == (BIT0 | BIT15) ) {
				ubNeedISPJump = 1;
				break;
			}
		}
	}
	//***************************************************************************************************************
	// check the last page to confirm the newest code block has been successfully programed or not.
	//***************************************************************************************************************
#if MicronFlashOnly
	FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
	FCONL[FCONL_LDPC_CFG] = ulLDPC_CFG_Temp;
#else
	FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
#endif
	ubL4KNum = (gubSectorsPerPlane >> 3);
	ulPlaneIndex = uwPlaneNum[ubLastNewCodeIndex] - 1;
	ubCEIndex = gubPlanesPerBurst - 1; //CE=1 in 2CH.

	M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
	FQ->ulRAMAddr = (U32)&gubZ2Buffer[0];
	FQ->btCheckEmpty = 1;
	FQ->btThisIsErasePage = 0;
	FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
	FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
	FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#endif

	uwBlock = guwSystemBlockRemappingTable[ubCodeSet_FirstScan[ubLastNewCodeIndex]][ubCEIndex];
	FQ->uwFUnit = (uwBlock >> gubBurstsPerBankLog);
	FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex, DEF_PlanesPerPage_Mcl_Div)) + (ubCEIndex << gubBurstsPerBankLog) + (uwBlock & gubBurstsPerBankMask)) << gub4kEntrysPerPlaneLog);
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
	if (pL4KTablePtr->ulL4K_LCA != CODE_LCA) {
		ubUsedCodeIndex = ubOldCodeIndex;
	}
	else {
		ubUsedCodeIndex = ubNewCodeIndex;
	}

	//***************************************************************************************************************
	// erase code block that version not the same as the used code version
	//***************************************************************************************************************
	if (ENABLE_DEBUG_UART_ATA_CMD) {
		UartString("\r\nThis is codeversion: ");
		Uart_Tx_DataHex(ubCodeVersion[ubUsedCodeIndex]);
	}

#if MicronFlashOnly
	U32 ulRET, ulRET2;
#endif
	gubSysCodeSet[0] = ubCodeSet_FirstScan[ubUsedCodeIndex];
	guwSysCodeBlock[0][0] = guwSystemBlockRemappingTable[gubSysCodeSet[0]][0];
	guwSysCodeBlock[0][1] = guwSystemBlockRemappingTable[gubSysCodeSet[0]][1];

	ubEraseCodeSetNumber = 0;
	if (ubTotalCodeSetNumber >= 2) {
		for (ubi = 0; ubi < ubTotalCodeSetNumber; ubi++) {
			if (ubCodeVersion[ubUsedCodeIndex] != ubCodeVersion[ubi]) {
				for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
					uwBlock = guwSystemBlockRemappingTable[ubCodeSet_FirstScan[ubi]][ubCEIndex];
#if MicronFlashOnly
					ulRET = ftlScanUnit_Binary((BIT_D3_TARGET | BIT_DIRECT_ACCESS), (uwBlock >> gubBurstsPerBankLog));
					ulRET2 = ((ulRET / gubPlanesPerPage + 1) * gubPlanesPerPage) + (ubCEIndex << gubBurstsPerBankLog) + (uwBlock & gubBurstsPerBankMask);
					if ((ulRET2 < (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div) - 1)) && (ulRET2 != 0)) {
						ftlForceSaveDummy(ulRET2, (M_Multiplier((U32)SHALLOW_ENDFASTPAGE, DEF_PlanesPerPage_Mcl_Div)), (uwBlock >> gubBurstsPerBankLog), (FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE | FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE | FORCE_SAVE_DUMMY_DIRECT_ACCESS), D3_AREA, VT_FORCE_DUMMY_0xD8);
					}
#endif
					//erase
					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
					FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
					FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif

					// Must Send Reset CMD(0xFF) After Erase Fail
					FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

					FQ->uwFUnit = (uwBlock >> gubBurstsPerBankLog);
					FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + (uwBlock & gubBurstsPerBankMask)) << gub4kEntrysPerPlaneLog);
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

				ftlAddToSystemSetFreeTable(ubCodeSet_FirstScan[ubi]);
				ubEraseCodeSetNumber ++;
			}
			else {
				if (ubi != ubUsedCodeIndex) {
					gubSysCodeSet[1] = ubCodeSet_FirstScan[ubi];
					guwSysCodeBlock[1][0] = guwSystemBlockRemappingTable[gubSysCodeSet[1]][0];
					guwSysCodeBlock[1][1] = guwSystemBlockRemappingTable[gubSysCodeSet[1]][1];
				}
			}
		}
	}


	//******************************************
	//find another goot unit to make code unit number to 2
	//******************************************
	ubTotalCodeSetNumber -= ubEraseCodeSetNumber;
	if (ubTotalCodeSetNumber == 1) {
#if ((MicronFlashOnly || YMTC) && (MICRON_NEW_BOOTCODE == 0))

		UBYTE ubCodeLimit, ubSmallestSet;
		if (B0KB || YMTC_JGS) {
			ubCodeLimit = 2 << gubBurstsPerBankLog;
		}
		else {
			ubCodeLimit = 4 << gubBurstsPerBankLog;
		}

		ubSmallestSet = gubSystemSetFreeTable[0];
		if ((guwSystemBlockRemappingTable[ubSmallestSet][0] < ubCodeLimit) && (guwSystemBlockRemappingTable[ubSmallestSet][1] < ubCodeLimit) && (ubSmallestSet != 0xFF)) {
			ubSecondCodeSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_HEAD);
		}
		else {
			M_AssertError(1);
		}
#else
		ubSecondCodeSet = ftlGetSystemSetFreeSet(GET_SYSTEM_SET_FROM_HEAD);
#endif
		for (ubCEIndex = 0; ubCEIndex < gubPlanesPerBurst; ubCEIndex++) {
			//erase
			M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
			FQ->ubFJob = BYTE_FJOB_ERASE_BLOCK;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
			FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
			FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif

			// Must Send Reset CMD(0xFF) After Erase Fail
			FQ->uwFJobInfo |= BIT_FJOBI_ERASE_FAIL_RESET;

			uwBlock = guwSystemBlockRemappingTable[ubSecondCodeSet][ubCEIndex];
			FQ->uwFUnit = (uwBlock >> gubBurstsPerBankLog);
			FQ->ulFEntry = (((ubCEIndex << gubBurstsPerBankLog) + (uwBlock & gubBurstsPerBankMask)) << gub4kEntrysPerPlaneLog);
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

			//read
			if (gubFlashID[0] == 0xb5) { // if ID start with 0xb5,special L06B, change page count to full block
				uwPlaneNum[ubUsedCodeIndex] = guwPagesPerUnit;
			}
#if ENABLE_TLC_PROGRAM_CODEBLOCK
			uwPlaneNum[ubUsedCodeIndex] = (guwPagesPerUnit / 3);
#endif

			for (ulPlaneIndex = 0; ulPlaneIndex < uwPlaneNum[ubUsedCodeIndex]; ulPlaneIndex++) {
				if (ulPlaneIndex == 0) {
					FCONL[FCONL_LDPC_CFG] &= ~(LDPC_MODE(0x7));
					FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(LDPC_4K_MODE0);
					ubL4KNum = 1;
				}
				else {
					FCONL[FCONL_LDPC_CFG] |= LDPC_MODE(gubLDPC_Mode_Test);
					ubL4KNum = gub4kEntrysPerPlane;
				}

				M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
				FQ->ulRAMAddr = (U32)&gubZ2Buffer[0];
				FQ->btCheckEmpty = 1;
				FQ->btThisIsErasePage = 0;
				FQ->ubFJob = BYTE_FJOB_READ;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
				FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#else
				FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS | BIT_FJOBI_RETRY);
#endif
				uwBlock = guwSystemBlockRemappingTable[ubCodeSet_FirstScan[ubUsedCodeIndex]][ubCEIndex];
				FQ->uwFUnit = (uwBlock >> gubBurstsPerBankLog);
				FQ->ulFEntry = (((M_Multiplier(ulPlaneIndex * (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1), DEF_PlanesPerPage_Mcl_Div)) + ((ubCEIndex << gubBurstsPerBankLog) + (uwBlock & gubBurstsPerBankMask))) << gub4kEntrysPerPlaneLog);
				FQ->ubDepth = ubDepth;
				FQ->ubL4kIndex = ubL4kLink;
				FQ->btUNServed = 1;
#if TLC
				FQ->btD3 = 1;
#endif
				gulPara0x24.BitMap.ubFrameNum = ubL4KNum;
				gulPara0x24.BitMap.btBufModeEn = 0;
				gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
				gulPara0x24.BitMap.ubL4KNum = ubL4KNum;
				gulPara0x24.BitMap.btZipBypass = 1;
				gulPara0x24.BitMap.btCmpEn = 0;
				gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
				gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

				if (ubL4KNum == 1) {
					gMTQ->Depth[ubDepth].ulZipMap = 0xFF;
				}
				else {
					gMTQ->Depth[ubDepth].ulZipMap = gulZipMap;
				}

				if (ENABLE_DCACHE) {
					xthal_dcache_region_invalidate((void*)FQ->ulRAMAddr, ubL4KNum * 4096);
				}

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
				pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + (FQ->ubL4kIndex * Spare4kSize * L4kQ_BIND_FACTOR));

				ulL4K_LCA_Clone = pL4KTablePtr->ulL4K_LCA;
				ulL4K_FW_Clone = pL4KTablePtr->Para0x04.BitMap.ulL4K_FW;
				//program
				UBYTE ubLMU = 0;
				for (ubLMU = 0; ubLMU < (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1) ; ubLMU++) {
					M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex);
					FQ->ulRAMAddr = (U32)&gubZ2Buffer[0];
					FQ->ubFJob = BYTE_FJOB_WRITE;
#if ENABLE_TLC_PROGRAM_CODEBLOCK
					FQ->uwFJobInfo = (BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#else
					FQ->uwFJobInfo = (((gubFlashID[0] != 0xb5) ? BIT_FJOBI_FASTPAGE : 0) | BIT_FJOBI_WAITRESULT | BIT_FJOBI_DIRECTACESS);
#endif
					uwBlock = guwSystemBlockRemappingTable[ubSecondCodeSet][ubCEIndex];
					FQ->uwFUnit = (uwBlock >> gubBurstsPerBankLog);
					FQ->ulFEntry = (((M_Multiplier((ulPlaneIndex * (ENABLE_TLC_PROGRAM_CODEBLOCK ? 3 : 1)) + ubLMU, DEF_PlanesPerPage_Mcl_Div)) + ((ubCEIndex << gubBurstsPerBankLog) + (uwBlock & gubBurstsPerBankMask))) << gub4kEntrysPerPlaneLog);
					FQ->ubRelativeDepth = ubDepth;
					FQ->ubDepth = 0xFF;
					FQ->ubL4kIndex = ubL4kLink;
					FQ->btUNServed = 1;
#if TLC
					FQ->btD3 = 1;
#endif
#if ENABLE_TLC_PROGRAM_CODEBLOCK
					FQ->ubD3WriteMode = (D3_SUSPEND_EN) | D3_WRITE_FINE | (D3_WRITE_LOWDER << ubLMU);
					FQ->btTLCProgramCodeBlock = 1;
#endif
					gulPara0x24.BitMap.ubFrameNum = gub4kEntrysPerPlane;
					gulPara0x24.BitMap.btBufModeEn = 0;
					gulPara0x24.BitMap.uwL4KSparePtr = L4K_TABLE_OFFSET + (ubL4kLink * Spare4kSize * L4kQ_BIND_FACTOR);
					gulPara0x24.BitMap.ubL4KNum = gub4kEntrysPerPlane;
					gulPara0x24.BitMap.btZipBypass = 1;
					gulPara0x24.BitMap.btCmpEn = 0;
					gulPara0x24.BitMap.btL4kBADRContModeEn = 0;
					gMTQ->Depth[ubDepth].Para0x24.ulAll = gulPara0x24.ulAll;

					for (ubL4KCount = 0; ubL4KCount < gub4kEntrysPerPlane; ubL4KCount++) {
						pL4KTablePtr = (L4KTable16B*)( L4K_TABLE_ADDR + ((ubL4kLink * L4kQ_BIND_FACTOR + ubL4KCount) * Spare4kSize));
						//set L4K table info

						pL4KTable.ulL4K_LCA = ulL4K_LCA_Clone;
						if (ulPlaneIndex == 0) {
							// should modify the header spare info, to indicate the new codeindex.
							pL4KTable.Para0x04.BitMap.ulL4K_FW = ((ulL4K_FW_Clone & 0xFF00) | ((1 - (ulL4K_LCA_Clone % 2)) + ubCEIndex * 2));
						}
						else {
							pL4KTable.Para0x04.BitMap.ulL4K_FW = ulL4K_FW_Clone;
						}
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
				}

			}
		}

		gubSysCodeSet[1] = ubSecondCodeSet;
		guwSysCodeBlock[1][0] = guwSystemBlockRemappingTable[gubSysCodeSet[1]][0];
		guwSysCodeBlock[1][1] = guwSystemBlockRemappingTable[gubSysCodeSet[1]][1];

	}

	FCONL[FCONL_FLH_FUNC] = ulFCONL_FLH_FUNC_Temp;
	FCONL[FCONL_LDPC_CFG] = ulLDPC_CFG_Temp;

	if (ubNeedISPJump) {
		Vender_Isp_Jump(0);
	}
	else {
		gubDeferredCodeSet[0] = 0xFF;
		gubDeferredCodeSet[1] = 0xFF;
	}
#endif
}

//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#if(!RDT)
void gating_clock(UBYTE ubMode, U32 ulIdleCount) {
	M_CheckBurnerModeToDisableFunction();
	volatile UBYTE ubDiePerChannel = gubDieNumber * (gubCENumber / gubPlanesPerBurst);
	gubGatingVDTEnable = 0;
	HandleStopRW(1);
	gubStopFTL = 0;
#pragma flush
	//    //==========2.  CR_MUX_GPIO[8:5] =0==========
	//    bSYS(0x202)&=0x1F;               	//CR_MUX_GPIO[7:5] = 0
	//    bSYS(0x203)&=0xFE;               	//CR_MUX_GPIO[8] = 0
	//
	//    //==========3.  With DDR, pull GPIO#4 OR #5 down==========
	//    if( (bSYS(0x114)&BIT1) == 0 ) //If XGPIO_I[4] is 0, pull down
	//    {
	//            bSYS(0x114)&=0xCF;               //CR_GPIO_PU_X[9:8] = 0
	//            bSYS(0x114)|=BIT6;                //CR_GPIO_PD[4] = 1
	//    }
	//
	//    if( (bSYS(0x115)&BIT1) == 0 ) //If XGPIO_I[4] is 0, pull down
	//    {
	//            bSYS(0x115)&=0xCF;               //CR_GPIO_PU_X[11:10] = 0
	//            bSYS(0x115)|=BIT6;                //CR_GPIO_PD[5] = 1
	//    }
	///optional:
	//==========2.  CR_MUX_GPIO[8:5] =0==========
	bSYS0(0x202) &= 0x1F;            		//CR_MUX_GPIO[7:5] = 0
	bSYS0(0x203) &= 0xFE;       		//CR_MUX_GPIO[8] = 0

	if (ubMode == BYTE_GATING_CLOCK_BG_1SEC) {
	Mark_IDLE_LPM2:
		if ((bSYS0(0x504) & 0x01) == 0x1) {
			bSYS0(0x504) |= 0x01;
		}
		//_xtos_ints_off( 1L << (5) );
		//ADDRESS
		wSYS0(0x50C) = 0x1100;          //LPM 2
		wSYS0(0x50E) = 0x2100;
		/***  NOTE#2a :    Normal Host may draw down DEVSLP pin directly, so set cr_wake_en[3] ONLY if DEVSLP has been enabled.
		                ( PPS sends Slumber INT before drawing down DEVSLP pin. )

		     After WU from LPM 2 ( DEVSLP pin L --> H ),
		     With DEVSLP enabled, CHECK if Device is ready for DEVSLP (APU 03Eh).
		     With DEVSLP not enabled, CHECK if Device idles again.

		     Since DEVSLP pin has serious glitch on EVBoard, verify this with care!  ***/
		if (gubDEVSLPEnable == 0) {
			wSYS0(0x51E) |= 0x600;          //Set WU_EN = SATA & ITC
		}
		else {
			wSYS0(0x51C) |= 0x100;          //Set MASK = cr_devslp_det_en
			wSYS0(0x51E) |= 0x208;          //Set WU_EN = SATA & (DEVSLP rising edge)
			bSYS0(0x3154) |= (BIT5);        //Rising of DEVSLP, for LPM2 wakeup usage.
		}

		//	SYS1L[SYS1L_INT_EN4] |= (BIT12 | BIT13 | BIT14 | BIT15); //rising/falling edge selected by cr_gpio_det_sel_2

		//		bSYS0(0x51C) &= 0xFD;          //Set MASK = cr_devslp_det_en
		//		bSYS0(0x51C) |= 0x0D;          //Set MASK = cr_devslp_det_en
		//
		//		wSYS0(0x51E) |= 0x0C0;          //Set WU_EN = SATA & (DEVSLP rising edge)
		//		bSYS0(0x51D) |= 0x02;          //Set WU_EN = SATA & (DEVSLP rising edge)

		/// add RTT0 wakeup if for idle1
		if (ubMode == BYTE_GATING_CLOCK_BG_1SEC) {
			wSYS0(0x51E) |= 0x100;
		}
		JTagSetting();
		if (!gubJTagWakeUp) {
			SYS0L[SYS0L_WU_SEL] |= BIT23; // 避免因為假睡導致切gpio 卻沒有醒過來把 gubCustomerGPIOEraseEnable切成0 會有user 明明切回high 再切low卻沒做 QE的情況
		}
		/***  NOTE#1 :     To avoid getting stuck in LPM 2, skip it by 3 conditions.
		                ( SRST, can be reproduced at longest 18 hours ),
		              Main point is turning off interrupts (ISR) before MASK is set.
		                ( HW register still rises the interrupt flags (ITC). )

		     HostVAR.gubHostSRST :       If there was sata_isr() before ' _xtos_ints_off() '
		     (bSYS0(0x3110)&0x20)==0x20 :    HW ITC received interrupt after MASK is set
		     ((bSYS0(0x504)&0x01)==0x1) :    WU source by SATA, ITC, or any other unexpected conditions  ***/
		if (gubSoftReset || ((bSYS0(0x3110) & 0x20) == 0x20) || ((bSYS0(0x504) & 0x01) == 0x1)) {
			//                                       UartString("\r\n  Skip LPM 2.1");
		}
		else {
			//                                       UartString("\r\n  Ready LPM 2.1");
			entry_selfrefresh();

			bSYS0(0x500) = 0x0F;       //Clear I2C groups
			bSYS0(0x502) = 0x0F;       //Clear I2C groups

			if (ubMode == BYTE_GATING_CLOCK_BG_1SEC) {
				SYS1L[SYS1L_INT_EN1] |= RTT0_INT;
				RTT_Set_Offset_48Bit(ulIdleCount * 1000);  //Set 1us time scale to 1 sec.
			}

			if (gubSoftReset | gubHardReset | gubHostCmdIn | HB[HB_REMAIN_VLD_NCQ_NUM]) {

			}
			else {
				*((volatile unsigned char*)0x04000500) &= 0xFB;
				if (ubMode == BYTE_GATING_CLOCK_BG_1SEC) {
					bSYS0(0x501) = 0x80;       //POWER DOWN
				}
				else {
					//					while(gubPMUTest);
					bSYS0(0x501) = 0x80;       //POWER DOWN
				}

				if (ubMode == BYTE_GATING_CLOCK_BG_1SEC) {
					if (SYS1L[SYS1L_INT_STS1] & RTT0_INT) {
						SYS1L[SYS1L_INT_STS1] |= RTT0_INT;
					}
				}

				while ((bSYS0(0x504) & 0x01) == 0) {
					if ((bSYS0(0x501) & 0x80) == 0) {
						break;            //Normal wakeup would go here ONCE.
					}
				}
			}
			exit_selfrefresh();
			/// wait for pll0/1 lock
			while (( ( *(volatile unsigned char*)0x04000400 ) & 0x3) != 0x3);
		}

		wSYS0(0x51E) = 0;               	//Clear all WU_EN
		bSYS0(0x504) = 0x01;           	 //W1C wakeup_f
		wSYS0(0x51E) |= 0x200;          //Set WU_EN = SATA
		bSYS0(0x3154) &= ~(BIT5);        //Rising of DEVSLP, for LPM2 wakeup usage.

		//	SYS1L[SYS1L_INT_EN4] &= (~(BIT12 | BIT13 | BIT14 | BIT15)); //rising/falling edge selected by cr_gpio_det_sel_2//14 and 15 for GPIO3
		//	SYS1L[SYS1L_INT_STS4] |= (BIT12 | BIT13 | BIT14 | BIT15); //rising/falling edge selected by cr_gpio_det_sel_2

		dINTERRUPT(INT_EN1) &= ~RTT1_INT_1;   // Disenable RTT1
		dINTERRUPT(INT_STS1) |= RTT1_INT_1;
		RTT_Set_Limit(1, 1000); // 1ms
		SYS1L[SYS1L_RTT_CTRL] = (SYS_RTTEN | SYS_CNT_CLEAR | RTT_SCALE_1US); //Enable RTT1, Repeat Mode, Scale 1us
		dINTERRUPT(INT_EN1) |= RTT1_INT_1;   // Enable RTT1
		//_xtos_ints_on( 1L << (5) );
	}
	else {
		if (gubDEVSLPEnable && (HW[HW_SATA_INFO]&H_DEVSLP)) {
			if (ENABLE_DEBUG_UART) {
				UartString("\nDEVSLP");
			}
		Mark_DEVSLP_LPM7:
			SaveRSparity(0);

			if ((bSYS0(0x504) & 0x01) == 0x1) {
				bSYS0(0x504) |= 0x01;
			}

			///Flush
			SaveAtaConfig();
			gubFlushMode = BYTE_STANDBY_FLUSH;
			FlushCache();
			HandleStopRW(1);
			if (0U == (HW[HW_SATA_INFO]&H_DEVSLP)) { /*match this case mean host need wakeup in this moment, direct return*/
				gubStopFTL = 0;
				if (HW[HW_SATA_INFO] & (H_SLUMBER | H_PARTIAL)) {
					HW[HW_PM_CTRL] |= H_REG_PMCANCEL;
				}
				return;
			}
			M_Save_DEVDLP_Table();
		Mark_Set_AON_To_Enhance_LoadCode_Speed:

			flaReloadAonHeader();
			//mDMAC_COPY( (U32)&gubCopyBuffer[0], (U32)(0x5c0d4000),  0x1000, 0, 0, 0, 0, 1);
#if TLC
			memcpy((void *)0x5c0D5000, gubAonBufOffset, sizeof(gubAonBufOffset));
#else
			memcpy((void *)0x5c0D5000, gubAonBufOffset, 0x50);
#endif

			_xtos_ints_off( 1L << (5) );         //Both turn ON in HostPowerOnInit() AFTER WU from DEVSLP
			_xtos_ints_off( 1L << (21) );

			//                         wSYS0(0x50C) = 0x1600; //LPM 7 new
			wSYS0(0x50C) = 0x8000; //LPM 7 new
			wSYS0(0x50E) = 0x2600;

			/// Below undone, need to comfirm with Zega
			wSYS0(0x51E) |= 0x1;   //Set WU_EN = (DEVSLP=0)

			JTagSetting();

			// Set Feature to Flash default interface
			flaSwitchClock(FLH_CLK_10MHz);
			M_FlashModeBackToDefault();

			*((ADDR8_t)0x04000044) = SIMREG_DEVSLP_LOAD_CODE; 	// For reminding Bootcode it's DEVSLP state
			*((ADDR8_t)0x04000046) = 0xFF; 					// Default Interface
			*((ADDR32_t)0x5C0D7FFC) = mDMAC_CRC(AON_BASE, AON_SIZE, AON_CRC32_SEED, 1 , (DMA_CRC_LEN_8 | DMA_CRC_DCCM));
			//HL[HL_PHY_MISC1] |= H_OOB_STOP;

			LPM_SaveMore();
			bSYS0(0x420) = 0; /// Set sysclk to Bootcode usage (100MHz

			entry_retention();  /// consider LPM7 as DDR/SDR power off as default

			bSYS0(0x500) = 0x0F;                 //Clear I2C groups
			bSYS0(0x502) = 0x0F;                 //Clear I2C groups
			//UartString("\r\n  Clear I2C cmd (DEVSLP)");
			I2C_beforePD();
			if (gubNoPWICEnable) { /// activate GPIO 11/12 output low
				//                                   *((volatile unsigned short*)0x04000202) &= 0xFE1F;  //mux to gpio
				//                                   *((volatile unsigned char*)0x0400011B) = 0x14;  //GPIO 11 as low
				//                                   *((volatile unsigned char*)0x0400011C) = 0x14;  //GPIO 12 as low

				*((volatile unsigned short*)0x04000202) &= 0xFE1F;  //mux to gpio
				*((volatile unsigned short*)0x04000202) |= 0x00A0;  //mux to gpio
			}

			while (1) {
				if (((HW[HW_SATA_INFO]&H_LNK_IDLEST) || (HW[HW_SATA_INFO]&H_DEVSLP)) && (HW[HW_SATA_INFO]&H_TRS_IDLEST)) {
					break;
				}
			}

			/// add at 20160307 detect GPIO8 to save power.
			*((volatile unsigned char*)0x04000118) &= 0x8B; /// set GPIO8 no pull up or down, and close output enable
			idle(10);
			if (*((volatile unsigned char*)0x04000118) & BIT1) {	/// is not grounded, so set pullup
				*((volatile unsigned char*)0x04000118) |= 0x10;
			}
			else { /// is not grounded, so set pull down
				*((volatile unsigned char*)0x04000118) |= 0x40;
			}
#if UFS
			SYS0B[0x200] &= ~(0x03);
			SYS0B[0x200] |= 0x02;
#endif
			//                      UartString("\r\n  Ready LPM 7.x");
			bSYS0(0x504) |= 0x1;                   	// W1C before DEVSLP
			bSYS0(0x501) = 0x80;                 		//POWER DOWN......起來就在 Boot Code了
			idle(50000);

			while (HW[HW_SATA_INFO]&H_DEVSLP);
			SYS0L[SYS0L_CLKSW_CTRL0] &= ~(CR_SEL_3_3 | CR_SEL_2_3);
			HW[HW_SYS_CTRL] &= ~H_BLK_FIS_EN;
			//SYSL[SYS0L_SIM_CTRL0] |= SIMREG_CMD_D2H;
			//SYSL[SYS0L_RST_CTRL] &= (~CR_RST_N_SATA);
			//SYSL[SYS0L_RST_CTRL] |= (CR_RST_N_SATA);
			//HL[HL_PHY_MISC1] &= ~H_OOB_STOP;

			gDEVSLP_Table->ubSATAPhyStatus = 0;
			gDEVSLP_Table->ubHostCmdIn = gubHostCmdIn;
			gDEVSLP_Table->ubSATACmdCode = gubCmdCode;
			*((ADDR32_t)0x5C0D7FFC) = mDMAC_CRC(AON_BASE, AON_SIZE, AON_CRC32_SEED, 1 , (DMA_CRC_LEN_8 | DMA_CRC_DCCM));

			Vender_Isp_Jump(2);

		}//// End of if(  gubDEVSLPEnable  &&  (HW[HW_SATA_INFO]&H_DEVSLP)  )
		else {
		Mark_Slumber_LPM6:
#if (ENABLE_NO_PARTIAL_ONLY_SLUMBER)
			if ((HW[HW_SATA_INFO] & (H_SLUMBER | H_PARTIAL)) && ((gubHIPMEnable) || (gubDevIPMON)))  {
#else
			if ((HW[HW_SATA_INFO] & (H_SLUMBER)) && ((gubHIPMEnable) || (gubDevIPMON)))  {
#endif
#if Hynix
				flaDisableSecretMode();
#endif
				SaveRSparity(0);
				//       UartString("2 S\r\n");
				//ADDRESS
				wSYS0(0x50C) = 0x1500;          //LPM 6.0
				wSYS0(0x50E) = 0x2500;

				//       wSYS0(0x51E) |= 0x600;          //Set WU_EN = SATA & ITC
				if (gubDEVSLPEnable == 0) {
					wSYS0(0x51E) |= 0x600;          //Set WU_EN = SATA & ITC
				}
				else {
					wSYS0(0x51C) |= 0x100;          //Set MASK = cr_devslp_det_en
					wSYS0(0x51E) |= 0x208;          //Set WU_EN = SATA & (DEVSLP rising edge)
					bSYS0(0x3154) |= (BIT5);        //Rising of DEVSLP, for LPM2 wakeup usage.
				}

				bSYS0(0x51E) |= 0xC0;          //Set WU_EN = Rising & falling of GPIO
				bSYS0(0x51C) |= 0x02;          //Set WU_EN = Rising & falling of GPIO
				bSYS0(0x3155) |= (0x30);        //Rising of DEVSLP, for LPM2 wakeup usage.



				JTagSetting();

				//delay_use(4096);
				entry_retention();              //Chk the comment in ' Read_beforeExitRetention() '

				bSYS0(0x500) = 0x0F;            //Clear I2C groups
				bSYS0(0x502) = 0x0F;            //Clear I2C groups
				I2C_beforePD();

				while (1) {
					//if((HW[HW_SATA_INFO]&H_LNK_IDLEST)&&(HW[HW_SATA_INFO]&H_TRS_IDLEST))
					if (HW[HW_SATA_INFO]&H_TRS_IDLEST) { //Link Layer won't idle when P\S.
						break;
					}
				}

				if (gubSoftReset | gubHardReset | gubHostCmdIn | HB[HB_REMAIN_VLD_NCQ_NUM]) {
				}
				else {
					bSYS0(0x501) = 0x80;            //POWER DOWN

					while ((bSYS0(0x504) & 0x01) == 0) {
						if ((bSYS0(0x501) & 0x80) == 0) {
							break;
						}
					}
				}
				if (SYS1L[SYS1L_INT_STS4]& PMU_INT_PD0GPIO_FALLING_EDGE) {
					gubQE_FromSlumber = 1;
#if 0
					gubCustomerGPIOEraseEnable = 1;
					HandleStopRW(1);
					gubStopFTL = 0;
					HandleSMARTSelfTest();
					Sanitize_Overwrite_Offline();
					gubTimeToDoBG = BYTE_BG_LEVEL_IDLE;
					M_DebugGPIO8_OFF_LED();
					Vender_GPIO_Offline();
					while (1);
#endif
				}
				wSYS0(0x51E) = 0;               //Clear all WU_EN
				bSYS0(0x504) = 0x01;            //W1C wakeup_f
				bSYS0(0x3154) &= ~(BIT5);        //Close Rising of DEVSLP interrupt
				bSYS0(0x3155) &= (0xCF);        //Rising & faliing of GPIO.
				bSYS0(0x51C) &= 0xFB;          //Set WU_EN = Rising & falling of GPIO
				SYS0L[SYS0L_CLKSW_CTRL0] &= ~(CR_SEL_3_3 | CR_SEL_2_3);

				//Read_beforeExitRetention();
				//This function is used to read DDR/SDR informantion from AON-Buffer, need to be replaced by "getting informaiton from VT" function, which is undone.
				gubDram_Type = VT->gubDramType;
				gubDram_Size = VT->gubDramSize;
				guwDQS0_Fine = VT->guwDDR_DQS_FINE;
				gubDQS0_G =  VT->gubDDR_DQS_G ;
				gubDram_Bank = VT->gubDram_Bank;
				gub_SDR_Corse = VT->gubSDR_Corase;
				guw_SDR_Fine = VT->guwSDR_Fine;
				exit_retention(gubDram_Type, gubDram_Size, gubDram_Bank);
				while (( (*(volatile unsigned char*)(0x04000400)) & 0x3) != 0x3); //Check if PLL0&1 lock
				//        UartString("Leaving S\r\n");
				/// Reset RTT for idle count
				//       gulRTT0StartCountTime = *((volatile unsigned long*)0x04000720);
				//UartString("\r\n    Left Slumber");
			}
#if (ENABLE_NO_PARTIAL_ONLY_SLUMBER)
#else
			else if ((HW[HW_SATA_INFO]&H_PARTIAL) && ((gubHIPMEnable) || (gubDevIPMON)))  {
			Mark_Partial_LPM5:
#if Hynix
				flaDisableSecretMode();
#endif
				SaveRSparity(0);
				//                UartString("2 P\r\n");
				//ADDRESS
				wSYS0(0x50C) = 0x1400;          //LPM 5
				wSYS0(0x50E) = 0x2400;

				wSYS0(0x51E) |= 0x600;          //Set WU_EN = SATA & ITC, original value is wSYS0(0x51E) |= 0x600


				bSYS0(0x51E) |= 0xC0;          //Set WU_EN = Rising & falling of GPIO
				bSYS0(0x51C) |= 0x02;          //Set WU_EN = Rising & falling of GPIO
				bSYS0(0x3155) |= (0x30);        //Rising of DEVSLP, for LPM2 wakeup usage.




				JTagSetting();

				entry_retention();              //Chk the comment in ' Read_beforeExitRetention() '

				bSYS0(0x500) = 0x0F;            //Clear I2C groups
				bSYS0(0x502) = 0x0F;            //Clear I2C groups

				// UartString("\r\n  Clear I2C cmd (P)");

				while (1) {
					//if((HW[HW_SATA_INFO]&H_LNK_IDLEST)&&(HW[HW_SATA_INFO]&H_TRS_IDLEST))
					if (HW[HW_SATA_INFO]&H_TRS_IDLEST) { //Link Layer won't idle when P\S.
						break;
					}
				}
				/// FOR AVOIDING Double D2H problem
				HW[HW_ERR_CTRL] |= H_REG_XHOLD;
				HB[HB_FLAG_CTRL] &= H_TSP_IDLE_SEL_8_clk_delay;
				HB[HB_FLAG_CTRL] |= H_TSP_IDLE_SEL_64_clk_delay;
				HW[HW_ERR_CTRL] &= H_REG_XHOLD_CLR;
#pragma flush
				if (gubSoftReset | gubHardReset | gubHostCmdIn | HB[HB_REMAIN_VLD_NCQ_NUM]) {
				}
				else {
					bSYS0(0x501) = 0x80;            //POWER DOWN

					while ((bSYS0(0x504) & 0x01) == 0) {
						if ((bSYS0(0x501) & 0x80) == 0) {
							break;
						}
					}
				}
				wSYS0(0x51E) = 0;               //Clear all WU_EN
				bSYS0(0x504) = 0x01;            //W1C wakeup_f
				wSYS0(0x51E) |= 0x200;          //Set WU_EN = SATA

				bSYS0(0x3155) &= (0xCF);        //Rising & faliing of GPIO.
				bSYS0(0x51C) &= 0xFB;          //Set WU_EN = Rising & falling of GPIO
				SYS0L[SYS0L_CLKSW_CTRL0] &= ~(CR_SEL_3_3 | CR_SEL_2_3);

				//Read_beforeExitRetention();
				//This function is used to read DDR/SDR informantion from AON-Buffer, need to be replaced by "getting informaiton from VT" function, which is undone.
				gubDram_Type = VT->gubDramType;
				gubDram_Size = VT->gubDramSize;
				guwDQS0_Fine = VT->guwDDR_DQS_FINE;
				gubDQS0_G =  VT->gubDDR_DQS_G ;
				gubDram_Bank = VT->gubDram_Bank;
				gub_SDR_Corse = VT->gubSDR_Corase;
				guw_SDR_Fine = VT->guwSDR_Fine;
				exit_retention(gubDram_Type, gubDram_Size, gubDram_Bank);
				while (( (*(volatile unsigned char*)(0x04000400)) & 0x3) != 0x3); //Check if PLL0&1 lock

				/// FOR AVOIDING Double D2H problem
				HW[HW_ERR_CTRL] |= H_REG_XHOLD;
				HB[HB_FLAG_CTRL] &= H_TSP_IDLE_SEL_8_clk_delay;
				HB[HB_FLAG_CTRL] |= H_TSP_IDLE_SEL_16_clk_delay;
				HW[HW_ERR_CTRL] &= H_REG_XHOLD_CLR;

				//UartString("\r\n    Left Partial");

				//       UartString("Leaving P\r\n");
				/// Reset RTT for idle count
				//       gulRTT0StartCountTime = *((volatile unsigned long*)0x04000720);
			}/// end of partial
#endif
			else {
			Mark_BG_IDLE2:
				goto Mark_IDLE_LPM2;
			}

		Mark_Set_Register_IRAM:
			//   UartString("ICODE OK\r\n");
			//   *((volatile unsigned char*)0x04000516)&=0xFE;
			idle(100);
			dINTERRUPT(INT_EN1) &= ~RTT1_INT_1;   // Disenable RTT1
			dINTERRUPT(INT_STS1) |= RTT1_INT_1;
			RTT_Set_Limit(1, 1000); // 1ms
			SYS1L[SYS1L_RTT_CTRL] = (SYS_RTTEN | SYS_CNT_CLEAR | RTT_SCALE_1US); //Enable RTT1, Repeat Mode, Scale 1us
			dINTERRUPT(INT_EN1) |= RTT1_INT_1;   // Enable RTT1
			////// Reset IRAM
			FCONL[FCONL_IRC_CTL] |= IRAM_INITIAL_TRIG; // Clear Flash IRAM
			while (FCONL[FCONL_IRC_CTL] & IRAM_INITIAL_TRIG);

			///0. Flash registers
			InitFlashRegister(1); // Set MTP and LLR table in flash IRAM


			if (gubPlanesPerBurst == 1) {
				FCONL[FCONL_FLH_FUNC] &= MT_TABLE_CE_NORMAL_MODE;
			}

#if MicronFlashOnly || YMTC_JGS
			if (gulParamPageInfo.ONFI.btNandSupplyByEmulator)
#else
			if (gulParamPageInfo.JEDEC.btNandSupplyByEmulator)
#endif
			{
				FCONL[FCONL_TMR_CTL] &= ~(TIMER_CPU_INTR_EN | TIMER_INTR_EN);
			}

#if(!BURNER)
			///1. FPU cmd
#if (Hynix3DV6 || YMTC_JGS)
			memcpy((void *)(0x50000000 + FPU_ADR), &gubIRAMContent, sizeof(gubIRAMContent));
#else
			memcpy((void *)(0x50000000 + FPU_ADR), gubIRAMContent, sizeof(gubIRAMContent));
#endif



			memcpy((void *)(0x50000000 + FPU_ADR2), &guwFpuEntry, sizeof(guwFpuEntry));
#endif

			///2. ALU
			flaSetFlashAddressType();
			//flaScanCE_And_Remapping();

			if (!gubGPIO_CE_Decoder) {
				UBYTE ubi;
				for (ubi = 0; ubi < (MAX_CE_PER_CH * MAX_CHANNEL); ubi++) {
					FCONB_REMAPPING_CE[ubi] = gubReMappingCE[ubi];
				}
			}

			FALLL[FCTLL_CNT_ONE] |= COUNT_MODE_EN;
			FALLL[FCTLL_CNT_ONE] &= (~COUNT_ONE_MODE);

			///3. MTP
			//   if (gubToggleMode) {
			//       // Set MTP 0x00, 0x04, 0x1C, ZipMap, 0x24, 0x28
			//      InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000,  0x0F000000, gulZipMap, 0x0C000000, 0x001A0001);
			//   }
			//   else{
			//       // Set MTP 0x00, 0x04, 0x1C, ZipMap, 0x24, 0x28
			//      InitMTSetTemplate((MAX_CHANNEL * MT_QUEUE_DEPTH), 0x00, 0x00000001, 0x00000000,  0x0F000000, gulZipMap, 0x0C000000, 0x00180001);
			//   }

			//   /// Flash dll tracking
			//   flaDLLTracking(0, 1, 0); //read
			//   flaDLLTracking(1, 1, 0); //write

			//   while(ubTest);

			//gubFlashMode = FLH_MODE_DEFAULT;
			//gubFlashClock = FLH_CLK_DEFAULT;

			if (gubFlashMode == FLH_MODE_TOGGLE2) {
				FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
				flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
				FALLL[FCTLL_RESERVE] |= DETECT_DQSB_ENABLE;
				//       pFlaReg[FCTLL_IO_SET].L |= DIFFERENTIAL_IO_TYPE;
			}
			else if (gubFlashMode == FLH_MODE_TOGGLE1) {
				FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
				flaSetPadIOType(2, SINGLE_END_IO_TYPE);
				//       pFlaReg[FCTLL_IO_SET].L &= SINGLE_END_IO_TYPE;
			}
			else if (gubFlashMode == FLH_MODE_ONFI_NVDDR2) {
				FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;
				flaSetPadIOType(2, DIFFERENTIAL_IO_TYPE);
				FALLL[FCTLL_RESERVE] |= DETECT_DQSB_ENABLE;
			}
			else if (gubFlashMode == FLH_MODE_ONFI_NVDDR) {
				FALLL[FCTLL_FLH_SET] |= FLA_ONFI_MODE;
				flaSetPadIOType(2, SINGLE_END_IO_TYPE);
			}
			else {
				FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;
				flaSetPadIOType(2, SINGLE_END_IO_TYPE);
				//       pFlaReg[FCTLL_IO_SET].L &= SINGLE_END_IO_TYPE;
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
				default:
					while (1);
					break;
			}

			//   UartString("1C:");
			//   UartLongHex( gulPara0x1C.ulAll );
			//   UartString("\r\n");

			//   flaSet_ODT_DRIVE();

			//   flaSetFlashPadControl(0);
			//   flaSetOverDrive();

			if (((ubDiePerChannel >= 16) && ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz)))
			        || ((ubDiePerChannel >= 8) && (gubFlashClock == FLH_CLK_266MHz))
			        || (guoFlashDefaultType.B.bt8LC && ((ubDiePerChannel >= 8) && ((gubFlashClock == FLH_CLK_200MHz ) || (gubFlashClock == FLH_CLK_180MHz))))
			        || (gODTSetting.ubODTEnable)) {
				FALLL[FCTLL_DYNAMIC_ODT] |= DYNAMIC_ODT_EN; //SET BIT0 of FCTLL_DYNAMIC_ODT 時才會開啟ODT(FlashIP自動決定什麼時候開啟 例如ToggleRead時)
			}

			/// Flash dll tracking
			flaDLLTracking(0, 1, 0); //read
			flaDLLTracking(1, 1, 0); //write
			if (ASIC && ENABLE_HIGH_SPEED) {
				fla_Set_Offset_DQS_Delay(gubFlashClock);        //Only Works with MP info setting
			}
#if (ENABLE_FORCE_L06B_TO_B0KB)
			ForceL06BToB0KB(0);
#endif
			///Restore RS Encode Plane Count
			UBYTE ubRSFrameIndex;
#if (!BURNER)
			UBYTE ubRSFastPage = VT->guwGRTarget[VT->gubGRTargetIndex].B.btFastPage;
			UBYTE ubRSTag2Group[USERDATA_RS_Group_Num + 1];
			U32 ulLastDataPlaneIndex;
			for (ubRSFrameIndex = 0; ubRSFrameIndex < (USERDATA_RS_Group_Num + 1) ; ubRSFrameIndex++) {
				ubRSTag2Group[ubRSFrameIndex] = 0xFF;
			}
			for (ubRSFrameIndex = 0; ubRSFrameIndex < USERDATA_RS_Group_Num; ubRSFrameIndex++) {
				// here ubRSFrameIndex use as group index
				if (ubRSFastPage) {
					Generate_RS_MAP_Index_by_Plane(RS.ulLastDataPlaneIndexFastPage[ubRSFrameIndex]);
				}
				else {
					Generate_RS_MAP_Index_by_Plane(RS.ulLastDataPlaneIndex[ubRSFrameIndex]);
				}
				ubRSTag2Group[RS.ubTagMAP[RS.ubGroupIndex][RS.ubTagIndex]] = ubRSFrameIndex;
			}
#endif
			for (ubRSFrameIndex = 0; ubRSFrameIndex < TOTAL_PARITY_GROUP_NUM; ubRSFrameIndex ++) {
				if (VT->guwRSFrameCount[ubRSFrameIndex] > 0) {
					RSL[RSL_TAG_SEL] = ubRSFrameIndex;
					while ((RSB[RSB_TAG_CONFIG_0] & RS_TAG_SEL_READONLY) != ubRSFrameIndex);
					RSB[RSB_TAG_PEC] = VT->guwRSFrameCount[ubRSFrameIndex];
#if (!BURNER)
					if (ubRSFrameIndex < (USERDATA_RS_Group_Num + 1)) {
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

						// 條件1: Unit結尾未滿252, 正準備寫Parity, 條件2: 滿252,正準備寫Parity
						if ((( VT->guwRSFrameCount[ubRSFrameIndex] >= (RS.uwPlaneOffset + 1) ) && (VT->gulGRTargetPTR > ulLastDataPlaneIndex))\
						        || (VT->guwRSFrameCount[ubRSFrameIndex] >= (guwParityPlanesNum - 3 + 1))) {
							gubForceProgramRSParity[ubRSFrameIndex] = 1;
						}
					}
#endif
					RSL[RSL_HANG] |= RS_TAG_VALID_SET;
				}
			}

			///Reset BufferFlag
			AXIL[AXIL_SET_START_FLAG] = 0;
			AXIL[AXIL_SET_FLAG_LENGTH] = BUFFER2_NUM;
			AXIL[AXIL_CLEAR_ARRANGEMENT_FLAG] = (U32)1;     //clear all buffer flag
			M_SetFlagSettingSHF();

			if (ENABLE_DDR_MODE) {
				// DDR MODE
				HL[HL_BUF_R_EP] = BUFFER2_WRITE_END;
				HL[HL_BUF_W_SHF] = 0;
				HL[HL_BUF_W_EP] = SDRAM_CACHE_4KNUM - 1;
				AXIL[AXIL_FLAG_SET] &= Flag_SW_256K;
				AXIL[AXIL_FIRST_START_ADDRESS] = BUFFER2_BASE;
				AXIL[AXIL_FIRST_END_ADDRESS] = BUFFER2_BASE + (BUFFER2_NUM - 1) * 512;
			}
			else {
				HL[HL_BUF_R_EP] = BUFFER2_READ_END;
				HL[HL_BUF_W_SHF] = BUFFER2_WRITE_START;
				HL[HL_BUF_W_EP] = BUFFER2_WRITE_END;
				AXIL[AXIL_FLAG_SET] &= Flag_SW_256K;
				AXIL[AXIL_FLAG_SET] |= Flag_SW_128K_128K;
				//AXIL[AXIL_FLAG_SET] |= Flag_SW_192K_64K;
				AXIL[AXIL_FIRST_START_ADDRESS] = BUFFER2_BASE;
				AXIL[AXIL_FIRST_END_ADDRESS] = BUFFER2_BASE + (BUFFER2_Read_NUM - 1) * 512;
			}
			AXIL[AXIL_SECOND_START_ADDRESS] = BUFFER2_WRITE_BASE;
			AXIL[AXIL_SECOND_END_ADDRESS] = BUFFER2_WRITE_BASE + (BUFFER2_WRITE_NUM_AXIL - 1) * 512;
			//   flaSetFlashMode();
			//   flaSwitchClock(gubFlashClock);
			//   InitSATARegister();

			//UartLongHex( *((volatile unsigned long*)0x0400941c) );
			//UartString("\r\n");
			//#pragma flush
		}
	}

	if (gubJTagWakeUp && (bSYS0(0x3124) & 0xC8)) {        //Wake up by JTAG......Disable back ground idle.
		gubEnableBG_Idle = 0;
		gubTimeToDoBG = 0;
		if (ENABLE_DEBUG_UART_BG) {
			UartString("\nJTAG!");
		}
	}
	gubGatingVDTEnable = 1;
}
#endif
///////////////////////////////////////
void I2C_beforePD_Test() {
	M_CheckBurnerModeToDisableFunction();
	*((volatile unsigned char*)0x04000203) &= 0xFD;
	*((volatile unsigned char*)0x04000203) |= 0x04;

	I2C_Write_ConfigNOtrig(0x1B, 1, 0 , 0x11 , 0x0C);		//Group1  //CH1, 2, 3, 4 off
	bSYS0(0x250C) |= BIT5;

	I2C_Write_ConfigNOtrig(0x1B, 4, 1 , 0x11 , 0xFC);		//Group4  //CH1, 2, 3, 4 on
	bSYS0(0x256C) |= BIT5;

	/*
	UartString("ADDR 1:");
	UartLongHex( *((volatile unsigned long*)0x04002500) );
	UartString("\r\n");

	UartString("ADDR 4:");
	UartLongHex( *((volatile unsigned long*)0x04002560) );
	UartString("\r\n");
	 */

	/*  PMU  */
	bSYS0(0x500) = 0x1F;						//I2C group1 needs to be power down
	bSYS0(0x502) = 0x8F;						//I2C group4 needs to be wake up


}
////////////////////////////////////////
void I2C_beforePD() {
	M_CheckBurnerModeToDisableFunction();

	if (gubNoPWICEnable)
		return;

	UBYTE flhType = ((bSYS0(0x4) & (BIT5 | BIT4)) >> 4);

	SYS0L[SYS0L_SYS_MUX_CTRL0] &= ~(BIT27);
	SYS0L[SYS0L_SYS_MUX_CTRL0] |= BIT26;


	*((volatile unsigned char*)0x04000203) &= 0xF5;
	*((volatile unsigned char*)0x04000203) |= 0x04;

	/*  0x11 is SLEEP REG on I2C Spec, p.27  */
	//BIT 0: SLEEP,		0: exit sleep,		1: enter Sleep
	//BIT 1: Reserved
	//BIT 4: CH4, DRAM,		0: turn off,		1: LPM (Slumber)
	//BIT 5: CH3, NAND IO,	0: turn off,		1: LPM (Slumber)
	//BIT 7: CH1, NAND,		0: turn off,		1: LPM (Slumber)

	//====================LPM 7.2====================
	if ( (HW[HW_SATA_INFO]&H_DEVSLP) && !(HW[HW_SATA_INFO]&H_SLUMBER) && (wSYS0(0x50E) == 0x2600) ) { /// For DEVSLP
		/*  I2C  */
		I2C_Write_ConfigNOtrig(0x1B, 1, 0 , 0x11 , 0x0C);		//Group1  //CH1, 2, 3, 4 off
		bSYS0(0x250C) |= BIT5;

		SYS1L[I2C_Slave_Address_Register(2)] |= (unsigned long)0x1B;
		SYS1L[I2C_OP_Register(2)] = 0x400c300c;
		/*
				bI2C(I2C_OP_Register(2)) = 0x0C;				//Group2  //CH6 volt down
				bI2C(I2C_OP_Register(2) + 1) = 0x30;
				bI2C(I2C_OP_Register(2) + 2) = 0x0C;			//Group2  //CH6 volt up
				bI2C(I2C_OP_Register(2) + 3) = 0x40;
				*/
		SYS1L[I2C_Ctrl_Register(2)]  = (0x00020000 | ( I2C_BUF_SELECT & (3 << 4)) );
		bSYS0(0x252C) |= BIT5;

		//I2C_Write_ConfigNOtrig(0x1b, 3, 0 ,0x11 ,0xDC);	//Group3  //CH1, 4 on
		SYS1L[I2C_Slave_Address_Register(3)] |= (unsigned long)0x1B;
		SYS1L[I2C_OP_Register(3)] = (unsigned long)0xDC11;
		/*
				bI2C(I2C_OP_Register(3)) = 0x11;
				bI2C(I2C_OP_Register(3) + 1) = 0xDC;
			*/
		SYS1L[I2C_Ctrl_Register(3)] = (0x00020000 | ( I2C_BUF_SELECT & (3 << 4)) );
		//dI2C(I2C_Ctrl_Register(3))  = (0x00020000 | ( I2C_BUF_SELECT &(3 << 4)) );
		bSYS0(0x254C) |= BIT5;

		//FAE suggests NAND IO exit sleep latest
		I2C_Write_ConfigNOtrig(0x1B, 4, 1 , 0x11 , 0xFC);		//Group4  //CH3 on
		bSYS0(0x256C) |= BIT5;


		/*  PMU  */
		bSYS0(0x500) = 0x3F;						//I2C group1, 2 needs to be power down
		bSYS0(0x502) = 0xEF;						//I2C group2, 3, 4 needs to be wake up
	}

	//====================All Slumber====================
	else if ( (HW[HW_SATA_INFO]&H_SLUMBER) && ( (flhType == DDR3_400) || (flhType == DDR3_533) ) ) { /// only when DDR need to use i2c to lower the power
		I2C_Write_ConfigNOtrig(0x1B, 1, 0 , 0x08 , 0x00);		//Group1  //CH4 volt down
		bSYS0(0x250C) |= BIT5;
		I2C_Write_ConfigNOtrig(0x1B, 4, 1 , 0x08 , 0x04);		//Group4  //CH4 volt up
		bSYS0(0x256C) |= BIT5;
		bSYS0(0x254C) |= BIT5;

		/*  PMU  */
		bSYS0(0x500) = 0x1F;						//I2C group1 needs to be power down
		bSYS0(0x502) = 0x8F;						//I2C group4 needs to be wake up
	}
}
////////////////////////////
void I2C_Write_ConfigNOtrig(unsigned char ubAddrSelect, unsigned char ubGroupsel, unsigned char ubBufSel, unsigned char ubOPcode , unsigned char ubValue) {
	M_CheckBurnerModeToDisableFunction();
	SYS1L[I2C_Slave_Address_Register(ubGroupsel)] &= 0xFFFFFF00;
	SYS1L[I2C_Slave_Address_Register(ubGroupsel)] |= (unsigned long)ubAddrSelect; //set address
	SYS1L[I2C_OP_Register(ubGroupsel)] = (0x000000FF & ubOPcode);
	if (ubBufSel == 0) {
		SYS1L[SYS1L_I2C_BUF0_REG] = 0;
		SYS1L[SYS1L_I2C_BUF0_REG] = (unsigned long)ubValue;
	}
	else {
		SYS1L[SYS1L_I2C_BUF1_REG] = 0;
		SYS1L[SYS1L_I2C_BUF1_REG] = (unsigned long)ubValue;
	}

	SYS1L[I2C_Ctrl_Register(ubGroupsel)] = (0x00010100 | ( I2C_BUF_SELECT & (ubBufSel << 4)) ); // OP_CNT=1  DATA_CNT=1  DIR=write

}
/////////////////////////////////
void Idle2LPM2() {
	M_CheckBurnerModeToDisableFunction();
	FlushCache();

	_xtos_ints_off( 1L << (5) );
	//ADDRESS
	wSYS0(0x50C) = 0x1100;			//LPM 2
	wSYS0(0x50E) = 0x2100;

	//MASK, WU_EN

	/***  NOTE#2a :	Normal Host may draw down DEVSLP pin directly, so set cr_wake_en[3] ONLY if DEVSLP has been enabled.
				  ( PPS sends Slumber INT before drawing down DEVSLP pin. )

		After WU from LPM 2 ( DEVSLP pin L --> H ),
		With DEVSLP enabled, CHECK if Device is ready for DEVSLP (APU 03Eh).
		With DEVSLP not enabled, CHECK if Device idles again.

		Since DEVSLP pin has serious glitch on EVBoard, verify this with care!  ***/

	///// Need to replace the variable with a different one. by Victor
	if (gubDEVSLPEnable == 0) {
		wSYS0(0x51E) |= 0x600;			//Set WU_EN = SATA & ITC
	}
	else {
		wSYS0(0x51C) |= 0x100;			//Set MASK = cr_devslp_det_en
		wSYS0(0x51E) |= 0x208;			//Set WU_EN = SATA & (DEVSLP rising edge)
	}


	/***  NOTE#1 :	To avoid getting stuck in LPM 2, skip it by 3 conditions.
				  ( SRST, can be reproduced at longest 18 hours ),
				Main point is turning off interrupts (ISR) before MASK is set.
				  ( HW register still rises the interrupt flags (ITC). )

		HostVAR.gubHostSRST : 		If there was sata_isr() before ' _xtos_ints_off() '
		(bSYS0(0x3110)&0x20)==0x20 :	HW ITC received interrupt after MASK is set
		((bSYS0(0x504)&0x01)==0x1) :	WU source by SATA, ITC, or any other unexpected conditions  ***/
	if (  gubSoftReset  ||  ((bSYS0(0x3110) & 0x20) == 0x20)  ||  ((bSYS0(0x504) & 0x01) == 0x1)  ) {
		//									UartString("\r\n  Skip LPM 2.1");
	}
	else {
		//									UartString("\r\n  Ready LPM 2.1");

		//bSYS0(0x504) = 0x01;		//Before WU_EN is set, shouldn't be 1

		entry_selfrefresh();

		bSYS0(0x500) = 0x0F;		//Clear I2C groups
		bSYS0(0x502) = 0x0F;		//Clear I2C groups


		bSYS0(0x501) = 0x80;		//POWER DOWN

		while ((bSYS0(0x504) & 0x01) == 0) {
			if ((bSYS0(0x501) & 0x80) == 0) {
				break;			//Normal wakeup would go here ONCE.
			}
		}


		///Uart0_Printf("\r\n  pmu_int (main)=");
		///Uart0_PutByteVal(bSYS0(0x3124));

		exit_selfrefresh();
		//									UartString("\r\n  Exit LPM 2.1");

		while (( ( *(volatile unsigned char*)0x04000400 ) & 0x3) != 0x3);

		//UartString("\r\n    Left LPM 2.1");
	}

	wSYS0(0x51E) = 0;				//Clear all WU_EN
	bSYS0(0x504) = 0x01;			//W1C wakeup_f

	_xtos_ints_on( 1L << (5) );
}
////////////////////////////////////////////
void LPM_SaveMore() {
	M_CheckBurnerModeToDisableFunction();
	bSYS0(0x30) = 0x4;	//CR_RRAM_IDX=4

	unsigned char ubCount = 0;

	for (ubCount = 0; ubCount < 64; ubCount++) {
		dSYS0((0x600 + (ubCount << 2) )) = LPM7RRAMConstant1[ubCount];
	}

	bSYS0(0x30) = 0x8;	//CR_RRAM_IDX=8

	for (ubCount = 0; ubCount < 64; ubCount++) {
		bSYS0((0x600 + (ubCount << 2) )) = LPM7RRAMConstant2[ubCount];
	}

	bSYS0(0x30) = 0x0;	//CR_RRAM_IDX=0
}


////////////////////////////=======================================================
void GPIO_DEVSLP_ISR() {
	M_CheckBurnerModeToDisableFunction();
#if ENABLE_PFAIL

	volatile unsigned long ulInt_Event;
	ulInt_Event = SYS1L[SYS1L_INT_STS4];

	/* 0xF30 = GPIO_SEL_0/1 Rising/Falling, DEVSLP Rising/Falling */
	if ((ulInt_Event & 0x0000F30)) {
		SYS1L[SYS1L_INT_STS4] = (ulInt_Event & 0x0000F30);

		if (ulInt_Event & GPIO_INT_SEL0_FALLING_EDGE) {

			gulPfail_time = 0;
			gubPfail_state = PFAIL_STATE_PRE_LOCK;
			gBQI.ubWrite_TQ_lock |= POWER_FAIL_TQ_LOCK;
#if ENABLE_ULTRAMLC_PFAIL_MEASURE
			SYS1B[SYS1B_RTT2_CTRL] |= SYS_CNT_CLEAR;
#endif
		}

		if (ulInt_Event & GPIO_INT_SEL1_RISING_EDGE) {
			//Cap full, do nothing now
		}
	}
	else {
		SYS1L[SYS1L_INT_STS4] = ulInt_Event;

		if ((ENABLE_VDT_RESET) && (gubGatingVDTEnable) && (ulInt_Event & 0x00A00000)) {
			// vdt power drop detected, set WDT to reset cpu
			WDT_Set_Limit(1);
			SYS1L[SYS1L_WDT_CTRL] |= 0x00000081;
		}
	}

#else
	U32 ulInt_Even;
	UBYTE ubReset = 0;
	ulInt_Even = *((volatile unsigned long*)0x04003124);
	if ( (ulInt_Even &  0x00000030)) {
		*((volatile unsigned long*)0x04003124) = ulInt_Even;	//W1C
	}
	else {
		if ((ulInt_Even &  0x00A00000)) {
			ubReset = 1;
		}

		*((volatile unsigned long*)0x04003124) = ulInt_Even;	//W1C

		if ((ENABLE_VDT_RESET) && (gubGatingVDTEnable)) {
			if (ubReset) {
				///WDT
				WDT_Set_Limit(1);
				SYS1L[SYS1L_WDT_CTRL] |= 0x00000081;
			}
		}
	}
#endif
}
////================================================================================
void ResetRegFromIDT() {
	M_CheckBurnerModeToDisableFunction();
	M_CheckRDTModeToDisableFunction();
	UBYTE ubTempCounter = 0;

	if (IDT.W59 & SET_BIT8) {
		for (; ubTempCounter < 5; ubTempCounter++) {
			if (IDT.W59 & (BIT0 << ubTempCounter)) {
				switch (ubTempCounter) {
					case 0:
						HW[HW_SYS_CTRL] &= 0x8FFF;
						break;
					case 1:
						HW[HW_SYS_CTRL] &= 0x8FFF;
						HW[HW_SYS_CTRL] |= H_MUL_PIO_DRQ_SIZE_1KB;
						break;

					case 2:
						HW[HW_SYS_CTRL] &= 0x8FFF;
						HW[HW_SYS_CTRL] |= H_MUL_PIO_DRQ_SIZE_2KB;
						break;

					case 3:
						HW[HW_SYS_CTRL] &= 0x8FFF;
						HW[HW_SYS_CTRL] |= H_MUL_PIO_DRQ_SIZE_4KB;
						break;
					case 4:
						HW[HW_SYS_CTRL] &= 0x8FFF;
						HW[HW_SYS_CTRL] |= H_MUL_PIO_DRQ_SIZE_8KB;
						break;
				}
				break;
			}
		}
	} /// end of if(IDT.W59 & SET_BIT8)

	/// DIPM
	if (((IDT.W79 & SET_BIT3) == 0) || (AtaCfg.uwDCOState & SET_DCO_IPM)) {
		gubDevIPMON = 0;
		gubDIPMP2SEnable = 0;
		HW[HW_PM_CTRL] &=  ~(H_HW_AUTO_DIPM_EN);

		if ((IDT.W79 & SET_BIT7) == 0) {
			HW[HW_PM_CTRL] &= ~H_REG_APSREQ;
		}
	}
	else {
		if (IDT.W79 & SET_BIT7) {	// APS enabled
			gubDevIPMON = 1;
			gubDIPMP2SEnable = 1;

			/*======= TC7_ALL, Phase 1 : Counts to H_DIPM_AUTO_TMR_THR and enter P or S =======*/
			HW[HW_PM_CTRL] &= 0xFF00;					//1)  Clear all ACK & NAK,
			HW[HW_PM_CTRL] |= H_IPM_EN;			//	only set IPM
			HW[HW_PM_CTRL] &= ~(H_FORCE_IPM_NAK);	//2)  Clear IPM_NAK  ...sometimes it's set unexpectedly
			HW[HW_DIPM_TIMER_THR] = (0x80 | (gubDIPM_THRESHOLD & 0x7F));			//3)  Set DIPM threshold,
			HB[HB_DIPM_AUTO_P2S_THR] = 0;		//                                20 ms  (partial wakeup, ask for wake up and slumber
			HW[HW_AUTOP2S_THR] = guwDIPM_AUTOP2S_THRESHOLD;			//                10 ms    ( S9 is 10 ms )

			HW[HW_SATA_INFO] |= SET_BIT12;			//5)  Clear DIPM-denied state  //W1C
			HW[HW_PM_CTRL] |= H_HW_AUTO_DIPM_P;		//6)  Make HW send P request
			if (gubENABLE_HW_DIPM && gubDIPMSupport) {
				HW[HW_PM_CTRL] |=  (H_HW_AUTO_DIPM_EN);
			}
		}
		else {
			gubDevIPMON = 1;
			gubDIPMP2SEnable = 0;
			/*======= TC7_ALL, Phase 1 : Counts to H_DIPM_AUTO_TMR_THR and enter P or S =======*/
			HW[HW_PM_CTRL] &= 0xFF00;					//1)  Clear all ACK & NAK,
			HW[HW_PM_CTRL] |= H_IPM_EN;			//	only set IPM
			HW[HW_PM_CTRL] &= ~(H_FORCE_IPM_NAK);	//2)  Clear IPM_NAK  ...sometimes it's set unexpectedly
			HW[HW_DIPM_TIMER_THR] = (0x80 | (gubDIPM_THRESHOLD & 0x7F));				//	 0x4E:                                  3 ms    ( S9 is 2 ms :0x56 )
			HB[HB_DIPM_AUTO_P2S_THR] = gubDIPM_P2S_THRESHOLD;		//                                20 ms  (partial wakeup, ask for wake up and slumber
			if (gubDEVSLPEnable == 1) {
				HB[HB_DIPM_AUTO_P2S_THR] = 0;
			}
			HW[HW_AUTOP2S_THR] = 0;				//4)  Clear 052h

			HW[HW_SATA_INFO] |= SET_BIT12;			//5)  Clear DIPM-denied state  //W1C
			HW[HW_PM_CTRL] |= H_HW_AUTO_DIPM_P;		//6)  Make HW send P request
			if (gubENABLE_HW_DIPM && gubDIPMSupport) {
				HW[HW_PM_CTRL] |=  (H_HW_AUTO_DIPM_EN);
			}
		}
	}

	/// DEVSLP
	if ((IDT.W79 & SET_BIT8) == 0) {
		gubDEVSLPEnable = 0;
		*((volatile unsigned short*)0x0400051C) &= 0xFEFF;
	}
	else {
		gubDEVSLPEnable = 1;
		*((volatile unsigned short*)0x0400051C) |= 0x100;
	}

	gulSMARTSelfTestCurrentLBA = AtaCfg.ulSMARTSelfTestStartLBA;
	gulSanitizeCurrentLBA = 0;
	gubStandyByTimerState = 0;
	//HB[HB_SECTOR_CNT] = MAXMULTICNT;  // Cheney110106

	if (IDT.W85 & BIT5) {
		gubWriteCacheEnable = 1;
	}

	if (gubAMAXSupport) {
		if (AtaCfg.ubAmaxState & SET_AMAX_FOREVER) {
			gulMediaSize.All = AtaCfg.ulAmaxSize;
		}
		else {
			gulMediaSize.All = gulNativeSize;
		}
		ChangeMediaSize();
	}
	else {
		if ((AtaCfg.ubHpaState & 0x17) == 0x14) { //HL4 : HL4 (Hardware Reset)
			//HL4/HEL4 state, keep volatile Mediasize
		}
		else if (AtaCfg.ubHpaState & SET_HPANVLBA) {//NV LBA valid or
			gulMediaSize.All = AtaCfg.ulHpaSize;
		}
		else if (AtaCfg.uwDCOState & SET_DCO_SIZE) {
			gulMediaSize.All = AtaCfg.ulDcoSize;
		}
		else {
			gulMediaSize.All = gulNativeSize;
		}
		ChangeMediaSize();
	}
}
void JTagSetting() {
	if (gubJTagWakeUp) {
		//Also use Jtag as GPIO. add at 10/19
		wSYS0(0x51C) |= 0x200;          	//Set MASK = cr_gpio_det_en
		wSYS0(0x51C) |= 0x10;           	//Use Jtag as GPIO
		wSYS0(0x51E) |= 0x80;           	//Set WU_EN = GPIO rising edge
		bSYS0(0x208) = 0x1;              	//Jtag auto detect
		bSYS0(0x3124) |= 0xC8;          	// W1C devslp & JTAG GPIO interrupt
		//////
	}
	else { // Open Quick Erase Function. Cant use Jtag wake up.
		SYS0B[SYS0B_CR_GPIO_DET_SEL] = 0xD;
		SYS0L[SYS0L_WU_SEL] |= (BIT9 | BIT22);
	}
}

