#define ASIC				1
#define ASIC_BB_FW			1 // 0: AA  1: BB

#define ENABLE_HIGH_SPEED		1
//#define _BURNER_MODE_                            //BURNER
//#define _RDT_MODE_
//#define _ICE_MODE_
//#define TLC                    	 		0
#define TLCRUT                    	 	TLC
#ifndef TLC_TSB
#define TLC_TSB 0
#endif
#ifndef ENABLE_PFAIL
#define ENABLE_PFAIL 0
#endif
#define ENABLE_OPENGR 0
#define ENABLE_VT_MIDDLE_STOP_CHECK 1
#define ENABLE_VT_DUMMY 0
#define ENABLE_D3_SHALLOWERASE_SAVE_DUMMY 0
#define ENABLE_OUTPUT_TRIMMING_TABLE (1 && TLC_BICS2)
#define ENABLE_FAKE_TEMPTURE 1
#define ENABLE_BICS3_SELECT_RETRY_TABLE (1 && TLC_BICS2)
#define ENABLE_MP_SET_RETRY_TABLE (1)

#define ENABLE_TLC_PROGRAM_CODEBLOCK (1 && (YMTC))
#define ENABLE_TLC_PROGRAM_CODEBLOCK2 (1 && (YMTC))
#define ENABLE_TLC_SPI_BURNER (0 && (YMTC))

#ifndef ENABLE_ULTRAMLC
#define ENABLE_ULTRAMLC        0
#endif
//#define Hynix							1
//#define Hynix_16nm					       ( 1 &  Hynix )
#define Enable_Sandisk_force_toggle1    1		//for sandisk force toggle 1
#define ENABLE_SANDISK_CHECKEARLYBADBYPROG		(1 && (BURNER) && (!MicronFlashOnly))
//========Linda=========
#define EnableProductHistory            1
#if EnableProductHistory
#define PHUnitNum                       2
#define PHStartUnit                     100
#define PHEndUnit                       150
#define PHMarkBad                       0x41
//gubPHinit
#define Bit0_InitByDBT_Find             BIT0
#define Bit1_InitByDBT_NotFind          BIT1
#define Bit2_InitByScan_Find            BIT2
#define Bit3_InitByScan_NotFind         BIT3
#define Bit4_InitDone                   BIT4
//gubPHmatch[]
#define Bit0_FindByDBT                  BIT0
#define Bit1_FindByScan                 BIT1

#define PH_Default                       0xffff
#endif
//========Linda=========

#define ENABLE_YMTC_1V2					(0&&YMTC)

#define EnableVenderCmdHistory			1
#if EnableVenderCmdHistory
#define EnableVH_UARTDebug				0
#define MaxVenderCmdLog 				16
#define C_VCLogPTR						MaxVenderCmdLog+1
#define C_FWMode						C_VCLogPTR+1
#define C_MARKStartOffset				C_FWMode+1
#define C_MARKEndOffset					C_MARKStartOffset+16
#define C_VHStartUnit                     150
#define C_VHEndUnit                       200
#endif
// for Retry FPU timeout error handle
#define	CHECK_FPU_POL_TIMEOUT		1
#define	CHECK_FPU_DMA_R_TIMEOUT		2
#define	CHECK_FPU_DMA_W_TIMEOUT		3
#define	CHECK_FQ_TIMEOUT		4

//#define		MicronFlashOnly				1
//#define 	B0KB						( 1 &&  MicronFlashOnly && TLC)

//==============================================================================
//Add for BiCs2
//#define TLC_BICS2						0
#define ENABLE_RUNTIME_SEEDINIT			(1 && (TLC_BICS2 || B0KB || Hynix3DV6 || Hynix3DV5 || YMTC_JGS))		// Adjust seed_init value by page index group
#define FLUSHD1_FORCE_DATAINOUT			(1 && (TLC_BICS2 || B0KB))
#define FLUSHD1_D3_VERIFY			(0 && (TLC_BICS2 || B0KB))
#define FLUSHD1_PARTIAL_CE_ADVANCE		(1 && (TLC_BICS2 || B0KB))
#define TSB_BICS3_COARSE_TUNING_TEST	0     //for test 先分成2條路
#define TSB_BICS4_SUPPORT				(1 && TLC_BICS2)
#define BICS2_GC_TO_D3					(1 && (TLC_BICS2))

#if B0KB
#define FLUSHD1_PARTIAL_CENUM			8	//when used CE < this value, do one group flush D1 (EX: 6ce set 8 do one group, set 6 do two group)
#elif (Hynix3DV6 || Hynix3DV5)
#define FLUSHD1_PARTIAL_CENUM			2
#elif YMTC_JGS
#define FLUSHD1_PARTIAL_CENUM			4
#else
#define FLUSHD1_PARTIAL_CENUM			8
#endif

#define BICS2_PAGES_PER_WORDLINE		3
#define BICS2_GC_TO_D3_L2P_LOCK_NUM		((1 * 1024 * 1024) / (16 *1024)) // 64 = (1MB / 16KB)

//Add for Hynix
#define Hynix_100Mhz_T1					0
#define Hynix_256G_LIMIT				1



// Add for Micron L06B
#if MicronFlashOnly
#define		MicronRandomizer			1
#define		Enable_MicronCalibration		1
#define		DIS_PreRead					0
#define		FlushD1SpeedDown			0
#define 	ENABLE_ONLY_SLC_ERASE_FOR_D1	1		// Do SLC mode erase on D1, and do TLC mode erase on D3
#endif
#if (MicronFlashOnly || TLC_BICS2 || (Hynix3DV6 || Hynix3DV5 || YMTC_JGS))
#define		SupportLargeUnitSize		1
#else
#define		SupportLargeUnitSize		0
#endif
#define		MicronUseSBSBR				(0 & MicronFlashOnly)
#if B0KB
#define		FixShallowErase				(1 & MicronFlashOnly)
#else
#define		FixShallowErase				(1 & MicronFlashOnly)
#endif
#define B0KB_PROGRAM_STEP 				12				// Flush D1 page number adjustment minumn unit is 12 pages.
#define B0KB_PROGRAM_MILESTONE_1 		48				// 1st step copy pages in flush D1
#define B0KB_PROGRAM_MILESTONE_2 		112				// 2nd step copy pages in flush D1
#define B0KB_PROGRAM_MILESTONE_3 		1503			// Final step copy pages, when over 1503 pages, finish flush D1 in one time.
#define B0KB_1STSLC_IN_END				1505			// 1st slc block in the end of B0KB, eg, 1505 slc( 1 pass programming and only 1 pages, 1506 2nd pass), 1st pass rule
														// different from other 1st 1 pass.
#define B0KB_3SHAREPAGE_1STPASS_LOWER_START_INDEX   48
#define B0KB_3SHAREPAGE_1STPASS_LOWER_END_INDEX   1454

#define GR_1152KB				(1 & B0KB)

#define	 		FORCE_SAVE_DUMMY_UNIT_MODE				BIT0	// force save dummy into unit
#define 		FORCE_SAVE_DUMMY_SINGLE_PLANE_MODE		BIT1	// force save dummy into single FLH plane
#define			FORCE_SAVE_DUMMY_DIRECT_ACCESS			BIT2
#define			FORCE_SAVE_DUMMY_ERASEALL_SLC_MODE		BIT3	// force use slc mode to save dummy when eraseall
#define 	Force_MLC_Erase				(1 & MicronFlashOnly & (!B0KB))	// force switch to MLC mode before erasure
#define		D3_AREA						1
#define		D1_AREA						0
#define		ENABLE_AUTOPOL_TIMEOUT_HANDLING			(1 & (((!Hynix) & (!ENABLE_ULTRAMLC)) | (MicronFlashOnly)) )			// auto pol timeout handling
#define 	ENABLE_DO_AUTOPOL_TIMEOUT_HANDLING (1 & ENABLE_AUTOPOL_TIMEOUT_HANDLING)
#define		UART_AUTOPOL_TIMEOUT_HANDLING			(0 & ENABLE_AUTOPOL_TIMEOUT_HANDLING)
#define		FORCE_AUTOPOL_TIMEOUT					(0 & ENABLE_AUTOPOL_TIMEOUT_HANDLING & MicronFlashOnly)	// debug option for autopol timeout
#define		FORCE_AUTOPOL_TIMEOUT_NUMERATOR			1		// Numerator for trigger autopol timeout event
#define		FORCE_AUTOPOL_TIMEOUT_DENOMINATOR		1000	// Denominator for trigger autopol timeout event
#define		AUTOPOL_TIMEOUT_MILLISEC				10000	// autopol timeout threshold time (mSec)
#define		MICRON_NEW_BOOTCODE			0
#define		MLCHBPassCntThreshold 			10

#define 		ENABLE_TABLE_VERIFY			(1 & ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING)
#define 		ENABLE_RESET_RETRY_LEVEL			(1 & (TLC_BICS2 || ((!TLC) & (!MicronFlashOnly) & (!Hynix) & (!ENABLE_ULTRAMLC) & (!YMTC))) & (!BURNER) & (!UFS))

#define		ENABLE_DEBUG_FREE_BLOCK_TABLE		(0 & (B0KB))
#define		ENABLE_RECORD_RETRY_INFO			0				// HB Retry組數若超過68組將造成VT蓋RAM問題, default關閉Record功能
#define 	ENABLE_RECORD_D1TOD3_READRETRY		(1 & (B0KB))	// Micron only support 16 HB retry sets
#define 		RECORD_D1TOD3_INFO_BASE				18			// gulRetryPassCount[18~20] use to record Flush D1 Unit info
#define 		RECORD_D1TOD3_READRETRY_BASE		21			//gulRetryPassCount [21~37] use to record flush D1 HB Pass cnt
#define 	ENABLE_RECORD_SLC_RETRY_INFO		(1 & (B0KB))	// Micron only support 16 HB retry sets
#define 		RECORD_SLC_READRETRY_BASE			38			// gulRetryPassCount[38~54] use to record SLC page HB Pass Cnt

#if (B0KB || TLC_BICS2)
#define ENABLE_ODD_CE		1	// yishion
#else
#define ENABLE_ODD_CE		0
#endif

#define ENABLE_FQ_ALIGNMENT	(1 & MicronFlashOnly)
#define		DOING_FQ_ALIGNMENT			BIT31
#define 	CHECK_FQ_ALIGNMENT			BIT30

// Add for Protect Data Consistency of Share Page
#define DEBUG_INSTEAD_STANDBY_BY_NORMAL_FLUSH		0	// for debug replace standby flush with normal flush
#define ENABLE_GEN_FAIL								0	// for generating read fail
#define ENABLE_GEN_FAIL_WHEN_SCAN_ACTIVE_GR			0	// for generating read fail when scan active GR in ftlinit.c
#define ENABLE_GEN_FAIL_WHEN_COPY_UNIT				0	// for generating read fail when copy unit in ftlPowerCycle
#define ENABLE_GEN_2ND_DPAGE_FAIL					0	// for generating 2nd Dpage fail when Bunch_RS_Recover
#define DISABLE_FLAMARKFAIL							0	// return in the beginning of flaMarkFail() function
#define DEBUG_BUNCH_SPARE_REGION					0
#define DEBUG_FTLINIT_STATE							0	// for dump FTLState after Scan VT finished
#define ENABLE_DEBUG_BUNCH_DUMMY_FLOW				1	// prevent fill up GR in ftlSaveBunchParityToInitInfo()
#define DEBUG_FIX_NCQT_TIMEOUT						1	// prevent clean GR in middle when GC is processing

#if (MicronFlashOnly && (B0KB==0))
#define BACKUP_RS_REGISTER_IN_BUNCH_RS_RECOVER		1	// store/restore rs registers in Bunch_RS_Recover()
#define SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND			1	// save a bunch of RS parity pages when received flush command for protect share page
#define USE_PBUF1_WHEN_SAVE_BUNCH_PARITY			1	// this should be removed... to be continue
#define ONLY_USE_SINGLE_TAG_FOR_BUNCH_PARITY		1	// this should be removed... to be continue
#define DIRECT_LOAD_TO_RSFRAMEBITS					1	// load Dpage to RSFrameBits[] rather to SRAM
#define USE_NEW_PARITY_PAGE_CHECKING                1   // for use new method for checking parity page or not
#define BYPASS_NONUNC_FPU_CHECKING					1	// bypass non-uncorrectable FPU operation in Bunch_RS_Recover()
#define GEN_FAIL_PER_MILLE							16	// define the gen fail parts of thousand, ex: 10 meas 10/1000 for fail rate, 0 means disabled
#define FORCE_GEN_FAIL_TO_RS_RECOVER				1	// force fail level to RS
#define DUMP_READ_FAIL_RETRY_TIME					0	// dump time period of flaReadFailRetry() when entered Bunch_RS_Recover()
#else
#define BACKUP_RS_REGISTER_IN_BUNCH_RS_RECOVER		0	// store/restore rs registers in Bunch_RS_Recover()
#define SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND			0	// save a bunch of RS parity pages when received flush command for protect share page
#define USE_PBUF1_WHEN_SAVE_BUNCH_PARITY			0	// this should be removed... to be continue
#define ONLY_USE_SINGLE_TAG_FOR_BUNCH_PARITY		0	// this should be removed... to be continue
#define DIRECT_LOAD_TO_RSFRAMEBITS					0	// load Dpage to RSFrameBits[] rather to SRAM
#define USE_NEW_PARITY_PAGE_CHECKING                0   // for use new method for checking parity page or not
#define BYPASS_NONUNC_FPU_CHECKING					0	// bypass non-uncorrectable FPU operation in Bunch_RS_Recover()
#define GEN_FAIL_PER_MILLE							0	// define the gen fail parts of thousand, ex: 10 meas 10/1000 for fail rate, 0 means disabled
#define FORCE_GEN_FAIL_TO_RS_RECOVER				0	// force fail level to RS
#define DUMP_READ_FAIL_RETRY_TIME					0	// dump time period of flaReadFailRetry() when entered Bunch_RS_Recover()
#endif
#define MAX_SHARE_PAGE_GAP							63	// 3 for Toshiba MLC flash, 63 for Micron L06B
#define GOODBLK_MLC									0
#define GOODBLK_SLC									1
#define DANCING_BIT_THRESHOLD						0x2000
#if B0KB
#define SHALLOW_ENDPAGE								guwPagesPerUnit
#define SHALLOW_ENDFASTPAGE							guwFastPagePagesPerUnit
#else
#define SHALLOW_ENDPAGE								20
#define SHALLOW_ENDFASTPAGE							20
#endif

//==============================================================================

#ifdef _BURNER_MODE_
        #define BURNER          1
        #define PH_SQL_FUNC  1
        #define ENABLE_PH_SQL_DMA_HIGH_SPEED	1
        #define ENABLE_VENDER_READ_VTH	0

#if TLC
#if Hynix
		#define BURNER_FIX_FPU_LENGTH  0x548
#elif B0KB
		#define BURNER_FIX_FPU_LENGTH  0x5D2
#else
        #define BURNER_FIX_FPU_LENGTH  0x540
#endif
#else
#if MicronFlashOnly
	#if FORCE_AUTOPOL_TIMEOUT
		#define BURNER_FIX_FPU_LENGTH  0x55E
	#else
		#define BURNER_FIX_FPU_LENGTH  0x540
	#endif
#else
        #define BURNER_FIX_FPU_LENGTH  0x51E
#endif
#endif
#else
        #define BURNER          0
        #define PH_SQL_FUNC  0
#endif
#ifdef _ICE_MODE_
        #define ICE          1
#else
        #define ICE          0
#endif

#ifdef _RDT_MODE_
        #define RDT     1
#else
        #define RDT     0
#endif

#if TLC
#if Hynix
#define FPU_Length		0x562+104
#elif B0KB
	#if FORCE_AUTOPOL_TIMEOUT
	#define FPU_Length		0x620 //0x608
	#else
	#define FPU_Length		0x5FE //0x5E6
#endif
#else
#define FPU_Length		0x540
#endif
#else
//MLC
#if MicronFlashOnly
	#if FORCE_AUTOPOL_TIMEOUT
	#define FPU_Length		0x55E
	#else
	#define FPU_Length		0x540
#endif
#else
#define FPU_Length		0x51E
#endif
#endif

#define ComresetRisingEdge 		1
#define Enable_Parity_Error 		1 // Close BRAM parity error interrupt enable
#define RDT_BURNER	0
#define RDT_BURNER_MARKBAD	0xC0

#if TLC_TSB//TSB
        #define FW_REVISION1			'A'
        #define FW_REVISION2			'7'
        #if TLC_BICS2
        #define FW_REVISION3            '2'
        #else
        #define FW_REVISION3			'0'
        #endif
        #define FW_REVISION4			'.'     // "W" means Write Protect, "T" means TEST, "." means normal release
        #define FW_REVISION5			'H'
        #define FW_INTERNALVERSION1 	'0'
        #define FW_INTERNALVERSION2		'0'
#else

        #define FW_REVISION1			'M'
#if TLC
    #if Hynix

	#if (Hynix3DV6 || Hynix3DV5)
		#define FW_REVISION2			'X'
	#elif Hynix_16nm
		#define FW_REVISION2			'5'
	#else
		#define FW_REVISION2			'3'
	#endif
    #elif MicronFlashOnly
            #if ENABLE_FORCE_L06B_TO_B0KB
                #define FW_REVISION2			'A'
            #else
                #define FW_REVISION2			'7'
            #endif
	#elif UFS
			#define FW_REVISION2				'M'
	#elif YMTC_JGS
			#define FW_REVISION2                'P'
    #else   //TSB
            #if TLC_BICS2
                #define FW_REVISION2			'6'
            #else
                #define FW_REVISION2			'1'
            #endif
    #endif
#else	//NOT TLC
    #if MicronFlashOnly
        #define FW_REVISION2			'2'
	#else
        #if UFS
			#define FW_REVISION2			'8'
        #elif HV
			#define FW_REVISION2			'9'
		#elif (ENABLE_ULTRAMLC && ENABLE_PFAIL)
			#define FW_REVISION2            'V'
        #elif ENABLE_ULTRAMLC
			#define FW_REVISION2			'U'
		#elif ENABLE_SANDISK_1ZNM_SPF
			#define FW_REVISION2			'D'
        #else
			#define FW_REVISION2			'0'
        #endif
    #endif
#endif
	#define FW_REVISION3			'1'
	#define FW_REVISION4			'.'     // "W" means Write Protect, "T" means TEST, "." means normal release
	#define FW_REVISION5			'3'
	#define FW_INTERNALVERSION1		'7'
	#define FW_INTERNALVERSION2		'2'
#endif

#define Table_version			0
#define VT_VERSION			1
//#define Nand_Emulator_Switch    0 //for TLC


#define        ENABLE_HYNIX_SLC	1
#define        ENABLE_MICRON_SLC     	1
#define 	FLASH19NM			7
#define 	FLASH24NM			6
#define 	FLASH32NM			5
#define 	FLASH43NM			4
#define 	FLASH56NM			3
#define 	FLASH70NM			2
#define 	FLASH1ZNM			1    // 90nm沒了...
#define 	FLASH1YNM 			0

//#define	ENABLE_SANDISK_1ZNM_SPF		0//gubSandiskSpecialFlash

//************************************************  Debug Setting ***************************************************************
#if BURNER
#define			ENABLE_DEBUG_WAF_KAO 							0
#define        ENABLE_DEBUG_RW                 					0
#else
#define			ENABLE_DEBUG_WAF_KAO 							0
#define        ENABLE_DEBUG_RW                 					0
#endif
#define         ENABLE_WL_NEW_CONDITION                         1
#define ENABLE_READ_CHECK_FUNCTION_UART 	0
#define ENABLE_SWAP_ACTIVEGR 1

//Read Disturb
#define         ENABLE_READ_DISTURB_LIMITED_HOST_READ           (1 && (0==BURNER))
#define         ENABLE_LIMITED_HOST_READ_UART                   (0 && (0==BURNER))
#define         ENABLE_CHECK_MT_DEPTH                           (0 && (0==BURNER))
#define         ENABLE_READ_DISTURB_PREREAD                     (1 && (0==BURNER))
#define         ENABLE_DEBUG_LOAD_TABLE_FLOW                    (0 && (0==BURNER))
#define         ENABLE_FORCE_READ_KAO_UART                      (0 && (0==BURNER))
#define         ENABLE_FORCE_TABLE_READ_CNT						(0 && (0==BURNER))
#define         ENABLE_DEBUG_READ_FLOW                          (0 && (0==BURNER))
#define         ENABLE_READ_DISTURB_CHECK_WITH_CRC              (1 && (0==BURNER))
#define         ENABLE_READ_DISTURB_TQ_ORDER                    (1 && (0==BURNER))
#define         ENABLE_SEQ_READ_CHECK_KAO_UART                   0
#define         ENABLE_MODIFY_EC                                 0
//

#define			ENABLE_TEMP_MICROCODE_FORCE_TO_FLASH			1
#define			ENABLE_DUMP_ERROR_SKIP_CHECK_SHALLOW_ERASE	1
#define			ENABLE_DEBUG_UART_TOSHIBA_ENTERPRISE			0
#define     	ENABLE_DEBUG_UART             					0
#define 	ENABLE_DEBUG_UART_SWAPTABLE				0
#define     	ENABLE_DEBUG_UART_VENDER             			0
#define     	ENABLE_DEBUG_UART_SCAN_WINDOW          			0
#define     	ENABLE_DEBUG_UART_RDT             				0
#define     	ENABLE_DEBUG_UART_FAIL        				0
#define     	ENABLE_DEBUG_UART_BG            				0
#define        ENABLE_DEBUG_UART_YH                             0
#define         ENABLE_DEBUG_UART_YT                            0
#define        ENABLE_DEBUG_UART_KJ                             0
#define 	ENABLE_DEBUG_UART_CC					0
#define 	ENABLE_DEBUG_UART_HC					0
#define	ENABLE_DEBUG_UART_INIT_TIME				0
#define	ENABLE_DEBUG_UART_INIT_TIME_II				0
#define        ENABLE_DEBUG_UART_FLH_INITIAL             			0
#define		ENABLE_FORCE_NVDDR_TEST								0
#define        ENABLE_DEBUG_UART_ERASE_COUNT    			0
#define 	ENABLE_DEBUG_UART_ATA_CMD					0
#define 	ENABLE_DEBUG_UART_DLMC					0
#define	ENABLE_DEBUG_UART_RS						0
#define        ENABLE_DEBUG_UART_TRIM	    				0
#define        ENABLE_DEBUG_UART_REPLACE_RUT   				0
#define        ENABLE_DEBUG_UART_4kSUSTAIENED  				0
#define 	ENABLE_DEBUG_UART_WRITEPROTECT				0
#define	ENABLE_DEBUG_UART_DPS					0
#define	ENABLE_DEBUG_RS_CalculateTagAndIndex			0
#define	ENABLE_DEBUG_BACKUP_VT					0
#define	ENABLE_DEBUG_VC_CHECK					0
#define	ENABLE_DEBUG_VC_CHECK_IN_CLEANGR			0
#define	ENABLE_DEBUG_TABLE_VC_CHECK				0
#define	ENABLE_DEBUG_UART_PH_SQL					0
#define	ENABLE_DEBUG_SOFTWAREECC					0
#define ENABLE_DEBUG_UART_VT_INIT				0
#define	ENABLE_DEBUG_RAWREAD_TSB					0
#define	ENABLE_DEBUG_UART_PREFORMAT					0
#if BURNER
#define	ENABLE_POWER_CYCLE_DONT_CNT_EVENT			0
#define	ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST			0
#else
#define	ENABLE_POWER_CYCLE_DONT_CNT_EVENT			0
#define	ENABLE_DEBUG_UART_POWER_CYCLERAND_TEST			0
#endif
#define 	ENABLE_DEBUG_FORCE_BG_TEST				0
#define 	ENABLE_DEBUG_RSDECODE					0
#define 	ENABLE_DEBUG_NCQ_UART_W					0
#define	ENABLE_DEBUG_NCQ_UART_R					0
#define	ENABLE_DEBUG_CHECK_LOADALIGN_E3D			0
#define	ENABLE_DEBUG_DDRINTERRUPT					0
#define 	ENABLE_DEBUG_ZByteNumSumTable 				0	//尚未排ZByteNumSumTable 的位子, 接SDR不能開
#define        ENABLE_DEBUG_GPIO               					0
#define 	ENABLE_DEBUG_NO_HANG					0

#define        ENABLE_DEBUG_COMRESET_NEEDJUMP      			0         // comreset will do standby flush and ISP Jump.
#define	ENABLE_DEBUG_FORCE_FASTPAGE_NONA2			0
#define        ENABLE_DEBUG_FORCE_POWERCYCLE_FLOW			0
#define	ENABLE_DEBUG_SET_SSD_SIZE					0
#define 	ENABLE_DEBUG_CODEREPLACEMENT_POWERCYCLE		0
#define 	ENABLE_DEBUG_UART_SANDISK_1ZNM_SPF		0
// PFAIL Debug Swtich Define
#define DEBUG_ULTRAMLC_POWER_FAIL 					0
#define ENABLE_UART_PFAIL_SAME_LBA                  0
#define DEBUG_ULTRAMLC_POWER_FAIL_BG_COPY           0
#define ENABLE_ULTRAMLC_PFAIL_SIM 					0
#define ENABLE_ULTRAMLC_PFAIL_MEASURE				0
#define ENABLE_ULTRAMLC_FORCE_1P 					0
//----------------------------------------------------------------------------------
#define 	ENABLE_DEBUG_RUT_BAD_REUSE				0
//----------------------------------------------------------------------------------
#define 	ENABLE_DEBUG_UART_CHECK_CHANGE_ID				0  //For Checking L06B change ID to B0KB
#define	ENABLE_WP_DEBUG						0
#define 	ENABLE_CY_DEBUG							0
#define 	ENABLE_CY_DEBUG_UART						0
#define	ENABLE_CH_DEBUG						0
#define 	ENABLE_KT_DEBUG							0
#define 	ENABLE_KT_DEBUG_UART						0
#define 	ENABLE_KT_DEBUG_SIZE						0
#define 	ENABLE_KT_DEBUG_MOREFAIL					0
#define	ENABLE_DEBUG_FORCE_SLC_POOL_SIZE			0
#define	FORCE_SLC_SIZE							0x80000
#define 	Failed_Test_UNC 							0
#define 	Failed_Test_CRC_WR						0
#define 	Failed_Test_CRC_RD						0
#define 	Failed_Test_E3D							0
#define 	Enable_LLAP							0
//************************************************************************************************

#if ASIC_BB_FW
	#define 	ENABLE_DEBUG_NOT_ENCODE_PARITY			0
	#define	ENABLE_DEBUG_BB_ITEM_CROSS_FRAME_ISSUE	0 // 0 for BB
	#define	ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE		0 // 0 for BB
#else
#define 	ENABLE_DEBUG_NOT_ENCODE_PARITY				1
	#define	ENABLE_DEBUG_BB_ITEM_CROSS_FRAME_ISSUE	1
	#define	ENABLE_DEBUG_BB_ITEM_IBUF_DIS_ISSUE		1
#endif
#define	ENABLE_DEBUG_REBUILD_P2L					0
#define 	ENABLE_DEBUG_READ_RUTBACKUP				0	//
#define	ENABLE_DEBUG_FORCE_FLH_CLOCK				FLH_CLK_DEFAULT
#define	ENABLE_DEBUG_FORCE_TABLE_FAIL_HANDLE			0

#define        ENABLE_EDO                    					0
#if MicronFlashOnly
#define        ENABLE_RANDCACHEREAD        					1
#else
#define        ENABLE_RANDCACHEREAD        					0
#endif
#define	ENABLE_ADJUST_L2P_WriteMode         				0
#define        ENABLE_DDR_MODE 						0	//尚未排DRAM Cache Table的位子, 接SDR不能開
#define	ENABLE_2ND_CHANCE_SWAPL2P				1
#define	ENABLE_SPECIALDATA						0

#define 	PARAMETER_PAGE							BIT0
#define 	JEDEC_PAGE								BIT1
#define		SET_PARAMETER_PAGE						BIT2

// FLA Retry Parameter
#define	RETRY_D1_BLOCK_TYPE						0
#define	RETRY_D2D3_BLOCK_TYPE					1

// FLA Error Handle Setting
#define	ENABLE_DEBUG_FIP_RESET_UART				0
#define	ENABLE_DEBUG_CHECK_CHANNEL_FPU_TIMEOUT_UART	0
#define	ENABLE_CHECK_CHANNEL_FPU_TIMEOUT			(1 && (BURNER == 0))
#define	ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN		0
#define	ENABLE_DEBUG_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN_UART		0
#define	ENABLE_RS_RECOVER_FAIL_DO_FPUCOR_AGAIN		1
#define	ENABLE_PROGRAM_FAIL_CHECK_UART				0
#define	ENABLE_READ_AUTO_POL_TIMEOUT_HANDLING		0
#define	ENABLE_DEBUG_UART_FIP_RESET_IN_PROGRAM_TIMEOUT	0
#define	ENABLE_FIP_RESET_IN_PROGRAM_TIMEOUT			(1 & ENABLE_AUTOPOL_TIMEOUT_HANDLING) // A2 sample 80h dma write timeout (lack 8 bytes)
#define	ENABLE_DEBUG_UART_FIP_RESET_IN_REBUILD_FQ		0
#define	ENABLE_FIP_RESET_IN_REBUILD_FQ				(1 & ENABLE_DO_AUTOPOL_TIMEOUT_HANDLING) // auto poll timeout error handle
#define	ENABLE_DEBUG_UART_DQS_MISSMATCH			0
#define	ENABLE_HBRETRY_LAST_NORMAL_READ_SPEED_DOWN	1 // 避免訊號太差，在HB retry最後一次normal read降速
#define	ENALBE_DEBUG_NES_CHANGE_ERROR_SETTING		0
#define	ENABLE_DEBUG_NES_ITERATION_UART				0
#define	ENABLE_DEBUG_RS_OVERLAP					0
#define	ENABLE_DEBUG_SAVETIME_RS_NORMAL_MODE		0
#define	ENABLE_SAVETIME_RS_NORMAL_MODE				1
#define	ENABLE_DEBUG_REDUCE_VARIABLE_STACK			0
#define	ENABLE_DEBUG_NES_READ_FAIL				0
#define	ENABLE_DEBUG_UART_NES_RS					0
#define	ENABLE_RS_NORMAL_MODE_FPUCOR_BY_FRAME		1
#define	ENABLE_DEBUG_RS_NORMAL_MODE_UART_PARITY_DUMP		0
#define	ENABLE_DEBUG_RS_NORMAL_MODE_UART_PARITY		0
#define	ENABLE_DEBUG_RS_NORMAL_MODE_UART			0
#define	ENABLE_DEBUG_RS_NORMAL_MODE_LARGE_ERROR_PAGE	0 // 0: 4 error;  1: 13 errors
#define	ENABLE_DEBUG_RS_NORMAL_MODE_COR_PARITY_PAGE	0 // to do
#define	ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR_ITERATION	0
#define	ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR 		0
#define	BIT_RS_CANREUSE							BIT15
#define	BIT_RS_LDPCCONVERGE						BIT14
#define	BIT_RS_INVALID_ERROR						BIT15
#define	ENABLE_DEBUG_RS_NORMAL_MODE_FAIL_BLOCK		0
#define	ENABLE_DEBUG_RS_NORMAL_MODE				0
#if ((ENABLE_GEN_FAIL==1) || ENABLE_ULTRAMLC)
#define	ENABLE_SAVE_RS_RECOVER_TIME_DURING_FTLCOPYUNIT	0
#else
#define	ENABLE_SAVE_RS_RECOVER_TIME_DURING_FTLCOPYUNIT	1
#endif
#define	ENABLE_DEBUG_RS_READ_SOFTBITCORRECT			0
#define	ENABLE_DEBUG_FIP_NO_HANG_BY_GEN_COMPRESSION_ERROR	0
#define	ENABLE_DEBUG_FIP_NO_HANG_BY_GEN_E3D_ERROR	0 // close DBUF parity error check to avoid exception...
#define	ENABLE_DEBUG_PROGRAM_E3D_FAIL_FIP_CHECK		0
#define	ENABLE_FIP_NO_HANG						1
#define	FLA_READUNC_MAXLOG_CNT					32
#define	ENABLE_DEBUG_READ_ERASEPAGE_ERROR			0
#define	ENABLE_DEBUG_FPUCOR_GEN_CRC32_ERROR			0
#define	ENABLE_RS_NORMAL_MODE					0
#define	ENABLE_DEBUG_BYPASS_GEN_ZIPHEADER			0
#define	ENABLE_DEBUG_RETRY_PARAMETER				0
#define	ENABLE_DEBUG_RESUME_STA_ERROR_BUG			0
//-------------------------------------------------------------------------------
#if ENABLE_GEN_FAIL==1
#define	ENABLE_DEBUG_ALL_RETRY					1
#else
#define	ENABLE_DEBUG_ALL_RETRY					0
#endif
#define	ENABLE_DEBUG_ALL_RETRY_EXCEPT_UNC			0
#define	ENABLE_DEBUG_ALL_UNC						0
//-------------------------------------------------------------------------------
#define	ENABLE_DEBUG_PBUF_TEMP_CLEAR				0
#define	ENABLE_DEBUG_SBC_HANG_ON					0
#define	ENABLE_DEBUG_RETRY_NOT_SWITCH_LEGACY 		0
#define	ENABLE_DEBUG_FIX_MT_EOT_BUG				0
#define	ENABLE_DEBUG_NO_HB						0
#define	ENABLE_DEBUG_RS_PRE_SL					0
#define	ENABLE_DEBUG_OVER_ECC_STOP				0
#define 	ENABLE_SOFTBITCORRECTION					1
#define 	ENABLE_DEBUG_SOFTBITCORRECTION				0
#define 	ENABLE_DEBUG_SOFTBITCORRECTION_READ_RAW_BUG	0
#define	ENABLE_DEBUG_OLD_USERDATA_GC_RS_FORMAT		0
#define	ENABLE_DEBUG_EMULATOR					0
//-------------------------------------------------------------------------------
#if ENABLE_GEN_FAIL==1
#define	ENABLE_DEBUG_HW_FORCEREADFAIL				1
#else
#define	ENABLE_DEBUG_HW_FORCEREADFAIL				0
#endif
#define	ENABLE_DEBUG_HW_FORCEREADFAIL_MT_PERCENTAGE	100
#define	ENABLE_DEBUG_HW_FORCEREADFAIL_ONLY_ONE_FAIL	0
#define	ENABLE_DEBUG_HW_FORCEREADFAIL_DATA_REGION	4	//[CC]: Read Fail 造D1D3 Fail Case, 0: Default, 1: D1 Only, 2: D3 Only, 3: D3+Table, 4: D1 + D3 without Table
#define	ENABLE_DEBUG_FLA_RANDOM_READFAIL			0
//-------------------------------------------------------------------------------
#define 	ENABLE_DEBUG_HW_FORCEERASEFAIL				0
#define 	ENABLE_DEBUG_HW_FORCEERASEFAIL_DATA_REGION	0	//[CC]: Erase Fail 造D1D3 Fail Case, 0: Default, 1: D1 Only, 2: D3 Only
#define 	ENABLE_DEBUG_HW_FORCEERASEFAIL_RATIO	20
//-------------------------------------------------------------------------------
#define 	ENABLE_DEBUG_HW_FORCEPROGRAMFAIL			0
#define 	ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_RATIO	5
#define 	ENABLE_DEBUG_HW_FORCEPROGRAMFAIL_SPECAILCASE_RATIO	20
#define 	STOP_GEN_FAIL								0
//-------------------------------------------------------------------------------
#if TLCRUT
#define 	ENABLE_DEBUG_MARKFAIL_CATEGORY				0	//[CC]: 限制 D1 or D3 MarkFail 的個數 以便穿插足夠數量的 D1 D3 交錯 SwapRUT
#define 	MAX_MARKFAIL_NUM_D1 					28
#define 	MAX_MARKFAIL_NUM_D3 					28
#endif


#define	ENABLE_DEBUG_SWAPRUT_D1TOD3_PROGRAM_FAIL	0
#define	ENABLE_DEBUG_SWAPRUT_D1TOD3_ERASE_FAIL		0
#define 	ENABLE_DEBUG_D1TOD3_ERR_HANDLE		0

#if TLC_BICS2 || YMTC
#define   RETRY_QUEUE_MAX_COUNT						120	// 512Gb  110
#else
#define   RETRY_QUEUE_MAX_COUNT						56
#endif

#if (TLC || MicronFlashOnly)
#if Hynix
#define	RETRY_SEQUENCE_MAX_COUNT					33
#elif (TLC_BICS2)
#define	RETRY_SEQUENCE_MAX_COUNT_BICS2					31
#define	RETRY_SEQUENCE_MAX_COUNT_BICS3					111 // 512Gb  110
#else
#define	RETRY_SEQUENCE_MAX_COUNT					37
#endif
#else
#define	RETRY_SEQUENCE_MAX_COUNT					12 // 0 for normal read again, 1~10 for retry parameter, 11 for TSB 15nm special flow
#endif
#define 	ENABLE_DEBUG_READ_VT_SHIFT				0 // 0: use default read voltage  1: read voltage shift (fake data retention / disturbance)
#define	ENABLE_DEBUG_RS_DMAC					0
#if FORCE_GEN_FAIL_TO_RS_RECOVER==1
#define	ENABLE_DEBUG_RS							1 // 0: normal  1: verify always UNC then enter RS recover
#else
#define	ENABLE_DEBUG_RS							0 // 0: normal  1: verify always UNC then enter RS recover
#endif
#define	ENABLE_DEBUG_RS_VERIFY_DATA				0 // 0: normal  1: verify golden data after RS correct
#if ENABLE_DEBUG_RS_VERIFY_DATA
#define	RS_NORMAL_MODE_MAX_FPUCOR_SIZE			5 // one for verify
#else
#define	RS_NORMAL_MODE_MAX_FPUCOR_SIZE			6
#endif
#define	ENABLE_DEBUG_RS_MAX_ERROR					0 // 0: normal  1: verify UNC flow during RS FPU read
#define	ENABLE_RS_RECOVER						1
#define	RS_TABLE_MAX_ERROR_COUNT					2
#if ENABLE_RS_NORMAL_MODE
#define	RS_MAX_ERROR_COUNT						25 // normal mode
#else
#define	RS_MAX_ERROR_COUNT						2 // erasure mode
#endif
#define	ENABLE_DEBUG_RETRY_SEQUENCE				0 // 0: enter retry sequence  1: not enter and normal read
#define	ENABLE_DEBUG_HB_RETRY					0 // 0: HB retry  1: normal read again
#define	ENABLE_HB_RETRY_ALL						0 // 0: only retry fail p4k  1: retry all
#define	ENABLE_DEBUG_HB_RETRY_ALL					0 // 0: normal  1: 根據gubHardBitRetryGroupNumber，可以任意設定在哪一次HB retry解回來
#define 	ENABLE_DEBUG_MTREMAINCNT_REG				0
#define	ENABLE_FLA_ERROR_LOG_COUNT				1
#define	ENABLE_FORCE_TWOPLANE					0
#define	ENABLE_SLCPOOL							(!ENABLE_ULTRAMLC)

#define	ENABLE_COMWAKEWORDAROUND				1
//-------------------------------------------------------------------------------
//#if (MicronFlashOnly || TLC_BICS2)
//#define ENABLE_USE_RSMAP					 1
//#else
//#define ENABLE_USE_RSMAP					 0
//#endif

//#define RSMAP_NEW_FLOW						(0 & ENABLE_USE_RSMAP & (!B0KB) )
#if MicronFlashOnly
#define L06B_8GROUP_RSMAP					(1 & (!B0KB))
#define L06B_12GROUP_RSMAP					(0 & (!L06B_8GROUP_RSMAP))
#define B0KB_12GROUP_RSMAP					(1 & B0KB )
#elif TLC_BICS2
#define BICS2_12GROUP_RSMAP					(1)
#elif (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
#define Hynix3DV6_12GROUP_RSMAP				(1)
#else
#define	RS_3GROUP_RSMAP						(1)
#endif
#define ENABLE_DEBUG_RSMAP					 0
//-------------------------------------------------------------------------------
#define 	ENABLE_NEW_RTT_INT_INIT					1
#define 	ENABLE_RUT_SWAP							1
#define	ENABLE_KEEP_LOADALIGN_BQ					1
#define 	ENABLE_FORCE_TRIMSIZE_POWEROF2				1
#define 	ENABLE_DEBUG_TRIM_BIT						0
#define 	ENABLE_BURNER_FORCEGENFAIL				0
#if (BURNER)
#define 	ENABLE_WRITEPROTECT						0
#else
#define 	ENABLE_WRITEPROTECT						1
#endif
#define	ENABLE_CODEBLOCKREPLACEMENT				0
#define        ENABLE_COMMANDQUEUE             		1
#if (BURNER||RDT)
#define	ENABLE_DCACHE					0
#else
#define	ENABLE_DCACHE					1
#endif

#define NotNessary 1

#define ENABLE_BURNER_SWITCH_TO_TOGGLE1  (!(B0KB || MicronFlashOnly)) //Only TLC MLC and BICS2
#define ENABLE_PSEUDO_CE             			1	//pseudo CE for FTL
#define ENABLE_FORCE_PSEUDO_CE             		0	//Force Pseudo CE
#define        ENABLE_D2HSDBByHW             			1
#define	ENABLE_READRETRY				1
#define        ENABLE_UPDATE_L2P_IN_XFERDATAIN 		1
#define        ENABLE_L2P_READ_TO_WRITE 			1
#define	ENABLE_SLCPOOL_SYNC_TO_L2P		1
#define        ENABLE_UPDATE_L2P_IN_CopyData		0
#define        ENABLE_Skip_DIG_Check 				1
#define	ENABLE_READ_DONETAG 				1
#define   ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING	1
#define 	ENABLE_CLEANGR_PRE_RW_L2P			(!ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING)
#define	ENABLE_READ_CACHE				1	//TQ R/W 交錯的時候要關掉，否則R BQ 塞完之後W會把DATA蓋掉
#define 	ENABLE_TRIM					1
#define 	ENABLE_NEW_TRIMALL 1
#define	ENABLE_WEARLEVELING				1
#define	ENABLE_INITIAL_READ_L2P_TO_W		1	// Load L2P to W in ftlInit
#define 	ENABLE_PRESERVE_L2P_GROUP_NUM		128	// Preserve 2GB
#define 	ENABLE_PRELOAD_L2P_GROUP_NUM		16	// PreLoad 256M
#define	ENABLE_SCAN_TABLE_LOAD_DATA		0	// buffer valid = 1 or 0 when scanning table unit
#define	ENABLE_DBUF_COPY_GR_PC			0	// Use data buffer or copy buffer do copy active GR
#define	ENABLE_PLANE_CH_CE				1
#define 	ENABLE_PREFETCH_FQ				1	// for cache read
#define        ENABLE_TWOPLANE          			1
#define        ENABLE_TWOPLANEREAD   			1
#define	ENABLE_FOURPLANE				1
#if BURNER
#define        ENABLE_MULTIPLANE_PROGRAM 		0
#define        ENABLE_MULTIPLANE_READ             	0
#define        ENABLE_CACHEPROGRAM     			0
#define        ENABLE_CACHEREAD             			0
#define	  ENABLE_ONEPLANE_CACHEREAD   	0

#else
#define        ENABLE_MULTIPLANE_PROGRAM  		1
#define        ENABLE_MULTIPLANE_READ             	1

#if Hynix
#if (Hynix3DV6 || Hynix3DV5)
#define        ENABLE_CACHEPROGRAM     			1
#define        ENABLE_CACHEREAD             	1
#else
#define        ENABLE_CACHEPROGRAM     			0
#define        ENABLE_CACHEREAD             		Hynix_16nm
#endif

#elif YMTC_JGS
#define        ENABLE_CACHEPROGRAM     			1
#define        ENABLE_CACHEREAD             	1
#else
#define        ENABLE_CACHEPROGRAM     			1
#define        ENABLE_CACHEREAD             			1
#endif
#define	  ENABLE_ONEPLANE_CACHEREAD		(1 && ((!B0KB) && (!Hynix)))

#endif

#define YMTC_JGS_DEBUG 1

#define	ENABLE_iFSA                       			1
#define	ENABLE_BACKGROUND_CLEAN			1
#define	ENABLE_BACKGROUND_COPY			1
#define	ENABLE_BACKGROUND_READ			1
#define	ENABLE_BACKGROUND_IDLE			1
#define	ENABLE_VT_BACKUP				(gubEnable_VT_Backup)
#define   RUTVeirfyCnt					3
#define	ENABLE_COMBINE_BQ				1
#define 	ENABLE_FIND_ZERO_VC_IN_CLEANGR		1
#define	ENABLE_FORCE_MLC				0
#define	ENABLE_FORCE_SLC				0
#define	ENABLE_4KSUSTAINED				1
#define	ENABLE_SWIFT_FLUSH				0
#define	ENABLE_GUARANTEE_FLUSH			0
#define 	ENABLE_SYSTEM_FREE_BLOCK_SORTING	             1
#define	ENABLE_STANDBY_STOP_GC			1
#define	ENABLE_DPS				             0
#define ENABLE_DPS_USE_TIMEOUT               0
#define DPS_WAIT_TIMEOUT_TIME_MILISEC        2
#define	ENABLE_HANDLE_BG_READ_FAIL		             0
#define	ENABLE_PARTIALBUILDGCTABLE			1
#define	ENABLE_GC_BUFFER				0
#define	ENABLE_FORCE_INIT_FINISH_WL			0
#if (BURNER)
#define 	ENABLE_ERROR_FORCE_BOOT	             	0
#define	ENABLE_ERROR_JUMP_RESET			0
#else
#define 	ENABLE_ERROR_FORCE_BOOT	             	0
#define	ENABLE_ERROR_JUMP_RESET			1
#endif
#define 	ENABLE_ERROR_DUMP_UNIT_CHECK             	1
#define   ENABLE_INITIAL_COPY_SKIP_DUMMY		0
#define	ENABLE_VenderCMD_return_PhysicalCE	             1
#define	ENABLE_NEW_SEQ_CALCULATION		             1
#define	ENABLE_KEEP_WBQ				0
#define	ENABLE_KEEP_WBQ_WITHOUT_LOADALIGN_COLLECT	0
#define	ENABLE_GC_GET_MAXEC_UNIT			1//To avoid keep choosing the high EC target
//-------------------------------------------------------------------------------
#define 	ENABLE_IOMETER_RAND			1
//-------------------------------------------------------------------------------
#define 	ENABLE_DEBUG_SYSTEM_UNIT_FAIL		0
//-------------------------------------------------------------------------------
#define	MAX_WRITE_TRIG_4K_NUM 			64   //64: zinfo queue
#define 	CLEANGR_PRELOAD_LINK_THRESHOLD		32
#define   MAX_ZIP_RATE 					4
#define	LZSS_RESOLUTION				0	//0: 0.5K   1: 1K    2: 2K   3: 4K
#define 	ENABLE_FIX_2PARITIES_CROSS_CH_BUG                 0

#define 	ENABLE_RESET_CNT                                                0
//For TLC
#if !(B0KB)
#define ENABLE_READ_CHECK_FUNCTION  1
#else
#define ENABLE_READ_CHECK_FUNCTION  0
#endif
#define ENABLE_FIND_MINVC_IN_D1                                     0

#if ENABLE_READ_CHECK_FUNCTION
  #if TLC
    #define Read_Chk_ByHostRead 0
	#define Read_Chk_ByReadScan 1
  #else
    #define Read_Chk_ByHostRead 0
	#define Read_Chk_ByReadScan 1 // 0 for MLC read disturb force move
  #endif
#endif

//#define MAX_PUSHCNT 4096-3 // -3 是為了對齊8192Bytes
#define MAX_PUSHCNT 4096-2
#define MAX_CHECKCNT 1 // ReadDisturb, 每次最多執行ForceReadMove的數量

#if ENABLE_READ_CHECK_FUNCTION
 // for Bics3
#define ReadDisturb_CheckECC_TH1 75
#define ReadDisturb_CheckECC_TH2 60
#define ReadDisturb_CheckECC_TH3 55
#define ReadDisturb_CheckECC_TH4 35
#define ReadDisturb_CheckECC_TH5 30

#define ReadDisturb_ForceReadMove_TH1 150
#define ReadDisturb_ForceReadMove_TH2 120
#define ReadDisturb_ForceReadMove_TH3 110
#define ReadDisturb_ForceReadMove_TH4 70
#define ReadDisturb_ForceReadMove_TH5 60

#define ReadDisturb_ForceReadMove_Buffer 1
#endif

//Partial SwapRUT
#define SWAPRUT_FORCEREADMOVE_MODE 	5

#define BYTE_SWAPRUT_NONE			0
#define BYTE_SWAPRUT_INIT			1
#define BYTE_SWAPRUT_PICK			2
#define BYTE_SWAPRUT_PREPARECOPY	3
#define BYTE_SWAPRUT_COPY			4
#define BYTE_SWAPRUT_FlushD1		5
#define BYTE_SWAPRUT_STOPCOPY		6
#define BYTE_SWAPRUT_UpdateRUTInRAM	7
#define BYTE_SWAPRUT_REMOVELOG		8
#define BYTE_SWAPRUT_DONE			9

//WL Issue - Hot Data Sampling Rate
#define HotDataSamplingRate_DefaultValue 200
#define HotDataSamplingRate_Min 50
#define HotDataMaxLevel	8
#define HotDataMinLevel	1
//TT Timeout Value
#define TEMPERATURE_NACK_TIMEOUT	2000//ms
#define TEMPERATURE_NACK_FAIL_RETURN_VALUE	(128) //-128 dgree

#if Read_Chk_ByHostRead
#define     RDRRCNTSTH                              50
#define     RDECCBITTH                             150
#endif

#if TLC
	#if Hynix
		#if Hynix_16nm
			#define RDECCBITTH 150// 暫定 H16
			#define ECCA2Threshold 150// 暫定 H16
		#else
			#define RDECCBITTH 150// 暫定 H14
			#define ECCA2Threshold 150// 暫定 H14
		#endif
  #elif MicronFlashOnly
  	#if ENABLE_FORCE_L06B_TO_B0KB
    	#define RDECCBITTH 150 // 暫定 L06 to B0K
			#define ECCA2Threshold 150// 暫定 L06 to B0K
    #else
      #define RDECCBITTH 188 // B0KB
			#define ECCA2Threshold 188 //B0KB
    #endif
  #else   //TSB
  	#if TLC_BICS2
  	  #define RDECCBITTH 150  // Bics3 / Bics2
  	  #define ECCA2Threshold 150 //暫定
    #else
  	  #define RDECCBITTH 150 // 15nm TLC
  	  #define ECCA2Threshold 150
    #endif
  #endif
#else	//NOT TLC
 	#if MicronFlashOnly
  	#define RDECCBITTH 150 // 暫定 L06
		#define ECCA2Threshold 150 // 暫定 L06
	#else
    #if UFS
			#define RDECCBITTH 150 // 暫定 UFS
			#define ECCA2Threshold 150
    #elif HV
			#define RDECCBITTH 150 //  HV
			#define ECCA2Threshold 68
		#elif (ENABLE_ULTRAMLC && ENABLE_PFAIL)
			#define RDECCBITTH 150 // 暫定
			#define ECCA2Threshold 150
    #elif ENABLE_ULTRAMLC
			#define RDECCBITTH 150 // 暫定 pSLC
			#define ECCA2Threshold 150
		#elif ENABLE_SANDISK_1ZNM_SPF
			#define RDECCBITTH 68 //  SND
			#define ECCA2Threshold 68
    #else
			#define RDECCBITTH 150 //  15nm MLC
			#define ECCA2Threshold 68
  	#endif
  #endif
#endif

#define     RDCHECKF                               256

#define TLC_NES_TEST_LEGACY_MODE	1
#define TLC_NES_TEST_SB_FLOW		(1 && (BURNER == 0))
#define TLC_NES_TEST        (0 && (BURNER == 0))


#define RetetionRead_EntryPerPlan   1
#define RetetionRead_EntrysOneTime     4096    //16M


#define MAXMULTICNT 16

#define MAXDSMCNT 			8

#define DPS_TRIMLEN_TLC 28
#define DPS_FIXLEN_TLC 2
#define DPS_STAGE_TLC 5
#define DPS_TRIMLEN_MLC 4
#define DPS_FIXLEN_MLC 1
#define DPS_STAGE_MLC 1

#ifdef DDR_Buffer
	#define BufferRingSize 1024 //512 KBytes
	#define WriteBufferSize 0x10000 //32MB
#endif
#ifdef ICCM_Buffer
	#define BufferRingSize (ICCM_TOTAL_SIZE-ICCM_CODE_SIZE)*2
	#define WriteBufferSize 2
#endif
#ifdef DCCM_Buffer
	#define BufferRingSize (DCCM_TOTAL_SIZE-DCCM_DATA_SIZE)*2
	#define WriteBufferSize 2
#endif

#define MICRON_SLC						BIT_FJOBI_FASTPAGE
#define MICRON_MLC						0

#define MAX_LOGICAL_CE                                                      	32
#define MAX_EXPAND_CE   ((ENABLE_SANDISK_1ZNM_SPF||((gubCENumber >= 16) &&(gubDieNumber>=4)))? MAX_LOGICAL_CE : MAX_CE )

#define MAX_SCAN_CE                                                      	64
#define MAX_SCAN_CE_PER_CH                                              32

#define MAX_PLANE_NUM                                   		64      // 2ch il8 4plane
#define MAX_CHANNEL                                             		2


#if GR_1152KB
#define MAX_CE                                                      		12
#elif (Hynix3DV6 || Hynix3DV5)
#define MAX_CE                                                               16
#else
#define MAX_CE                                                      		16
#endif
#if B0KB
#if GR_1152KB
#define	MAX_PLANE_PER_P2L			(1152 / 16)	//gubMaxPlanesPerP2LTable is max L4K entry in GRTable divide by page size
#else
#define MAX_PLANE_PER_P2L			(1056 / 16)	//gubMaxPlanesPerP2LTable is max L4K entry in GRTable divide by page size
#endif
#endif

#define B0KB_MAX_CE							12//max CE B0KB Support now

#define MAX_CE_PER_CH                                               	8
#define MAX_GROUP                                                       	4
#define MAX_TOTAL_PLANE_NUM                        		(MAX_PLANE_NUM*MAX_GROUP)
#define CHANNEL_NUM						2
#define CE_NUM_PER_CH						4
#define MT_QUEUE_NUM						8
#define MT_QUEUE_DEPTH					16
#define MT_QUEUE_DEPTH_MASK				15
#define G_MT_QUEUE_DEPTH					256
#define G_MT_QUEUE_DEPTH_MASK				255
#define MAX_EVENTS_PER_CH					(MT_QUEUE_DEPTH*CE_NUM_PER_CH)
#define FSA_PER_CH						128
#define ALU_GROUP_NUM						4

#define BRAM_SIZE		0x8000
#define BRAM_BASE	0x04010000

#define IRAM_SIZE		0x10000
#define IRAM_BASE		0x50000000

#define IRAMB   ((volatile unsigned char *)0x50000000)
#define IRAM2B   ((volatile unsigned short *)0x50000000)
#define IRAM4B   ((volatile unsigned long *)0x50000000)
#define IRAM8B   ((volatile unsigned long long *)0x50000000)

#define IRAM_BVCI_BASE		0x04020000

#define FPU_ADR		0x0000
#define FPU_SIZE		0x0700

#define DUMMY_L4K_TABLE_OFFSET	(FPU_ADR+FPU_SIZE) //0x0500_0700
#define DUMMY_L4K_TABLE_SIZE	256

#define FPU_ADR2	0xB000 		/* TLC's FPU */
#define FPU_SIZE2	0x1000

#define FSA_BASE	(DUMMY_L4K_TABLE_OFFSET+DUMMY_L4K_TABLE_SIZE) //0x0500_0800
#define FSA_BASE_CH0_0	(FSA_BASE)
#define FSA_BASE_CH1_0	(FSA_BASE+(0x400*1))
#define FSA_SIZE 	2048  // 64 FSA * 2CH * 8B

#define ALU_BASE_0	(FSA_BASE+FSA_SIZE)	// 0x04021000
#define ALU_BASE_1	(ALU_BASE_0+(16*1))
#define ALU_BASE_2	(ALU_BASE_0+(16*2))
#define ALU_BASE_3	(ALU_BASE_0+(16*3))

#define ALU_NORMAL	0
#define ALU_A2		1

#define ALU_SIZE 	64
#define TEMP_L4K_TABLE_OFFSET  (ALU_BASE_0+ALU_SIZE) // 0x5000_1040
#define TEMP_L4K_TABLE_SIZE  64

#define HB_RETRY_L4K_TABLE_ADDR  (IRAM_BASE+TEMP_L4K_TABLE_OFFSET+TEMP_L4K_TABLE_SIZE) // 0x5000_1080
#define HB_RETRY_L4K_TABLE_OFFSET  (TEMP_L4K_TABLE_OFFSET+TEMP_L4K_TABLE_SIZE)
#define HB_RETRY_L4K_TABLE_SIZE  64

#define TSB_A19_LLR1_BASE	(HB_RETRY_L4K_TABLE_ADDR+HB_RETRY_L4K_TABLE_SIZE) // 0x5000_10C0
#define LLR_TABLE_LOWER_PAGE_OFFSET	0
#define LLR_TABLE_MIDDLE_PAGE_OFFSET	(8*4)
#define LLR_TABLE_UPPER_PAGE_OFFSET	(8*4*2)
#define LLR_TABLE_SIZE		(32*4)// 128Bytes
#define TSB_A19_LLR2_BASE	(TSB_A19_LLR1_BASE+LLR_TABLE_SIZE) //0x5000_1140

#define TSB_TLC_1Z_LLR1_BASE	(HB_RETRY_L4K_TABLE_ADDR+HB_RETRY_L4K_TABLE_SIZE) // 0x5000_10C0
#define TSB_TLC_1Z_LLR2_BASE	(TSB_A19_LLR1_BASE+LLR_TABLE_SIZE) //0x5000_1140

#define SPARE_START_OFFSET	0x1200
#define SPARE_SIZE	0x8000		//128 L4kNum* 16B* 16 L4k bind together

#define SPARE_Base (IRAM_BASE+SPARE_START_OFFSET) //0x5000_1000
#define SPAREB	((volatile unsigned char  *)SPARE_Base)
#define SPARE2B	((volatile unsigned short *)SPARE_Base)
#define SPARE4B	((volatile unsigned long  *)SPARE_Base)

#define MAX_L4K_NUM_PER_PHYSICAL_PAGE   16      // 4k data  compress to 1k data,  so 64K data -> 16K page
#define L4K_TABLE_SIZE_PER_CE   (16*MAX_L4K_NUM_PER_PHYSICAL_PAGE)
#define L4K_TABLE_SIZE_PER_CH   (L4K_TABLE_SIZE_PER_CE * MAX_CE_PER_CH)
#define L4K_TABLE_ADDR  IRAM_BASE+SPARE_START_OFFSET
#define L4K_TABLE_OFFSET SPARE_START_OFFSET
#define L4K_TABLE_ADDR_CH0      L4K_TABLE_OFFSET
#define L4K_TABLE_ADDR_CH1      (L4K_TABLE_OFFSET+L4K_TABLE_SIZE_PER_CH)
#define L4K_TABLE_WRITE_ADDR L4K_TABLE_OFFSET

#define L4K_TABLE_READ_ADDR (L4K_TABLE_OFFSET+L4K_TABLE_SIZE_PER_CH*MAX_CHANNEL)
	#define L4K_LCA			0x00
	#define L4K_FW			0x04
	#define L4K_SPRV		0x07
	#define L4K_ZCODE		0x08
	#define L4K_NEXT_L4K_PTR	0x09
	#define L4K_FW2		0x0B
	#define L4K_BADR		0x0C
	#define L4K_BUF_MASK		0x0F

#define MT_EVENT_SIZE	64

#define MT_ADR_BASE		0xC000
#define MT_SIZE_PER_CH	(MT_EVENT_SIZE*MT_QUEUE_DEPTH*CE_NUM_PER_CH)
#define MT_SIZE_PER_QUEUE	(MT_EVENT_SIZE*MT_QUEUE_DEPTH)

//#define MT_SIZE_PER_DEPTH	(MT_EVENT_SIZE*CHANNEL_NUM)
#define MT_SIZE_PER_DEPTH	(MT_EVENT_SIZE*MAX_CHANNEL)
#define MT_QUEUE_OFFSET	(MT_SIZE_PER_DEPTH*MT_QUEUE_DEPTH)
#define MT_QUEUE_ADR_0	(MT_ADR_BASE)
#define MT_QUEUE_ADR_1	(MT_ADR_BASE+(MT_QUEUE_OFFSET*1))
#define MT_QUEUE_ADR_2	(MT_ADR_BASE+(MT_QUEUE_OFFSET*2))
#define MT_QUEUE_ADR_3	(MT_ADR_BASE+(MT_QUEUE_OFFSET*3))
#define MT_QUEUE_ADR_4	(MT_ADR_BASE+(MT_QUEUE_OFFSET*4))
#define MT_QUEUE_ADR_5	(MT_ADR_BASE+(MT_QUEUE_OFFSET*5))
#define MT_QUEUE_ADR_6	(MT_ADR_BASE+(MT_QUEUE_OFFSET*6))
#define MT_QUEUE_ADR_7	(MT_ADR_BASE+(MT_QUEUE_OFFSET*7))

// UART
#define     UART_FIFO_FULL  BIT0         // 1: fifo is full   0: fifo is not full
#define     UART_FIFO_EMPTY BIT1         // 1: fifo all empty 0: fifo has valid data


#define W_4KB_8BNUM 512

#define RS_PARITY_PER_FRAME		2
#if ENABLE_ULTRAMLC
#define PARITY_DATA_PAGE_NUM	gubParityDataPageNum	// (126 + 2)
#define VAR_PARITY_DATA_PAGE_NUM	252
#else
#define PARITY_DATA_PAGE_NUM	252
#define VAR_PARITY_DATA_PAGE_NUM	PARITY_DATA_PAGE_NUM
#endif

#define ENABLE_USE_255RS_DPAGE	1
#define RS_MAX_TAG_NUM			32
#define MAX_ENDING_DUMMY_PER_UNIT   2
#define PARITY_TABLE_PAGE_NUM_1CH 16

#if MicronFlashOnly
#define Fill_SharePage_Reserved	70
#elif (0==ENABLE_ULTRAMLC)
#define Fill_SharePage_Reserved	11 /* 1ch-MLC last 6 planes are RS, should consider last 11 plane. */
#else
#define Fill_SharePage_Reserved	6
#endif

/* RS Related, by new calculation of RS tag */
#define RS_TAG_PAGES_PER_GROUP	2
#define RS_PARITY_PLANES		(RS_PARITY_PER_FRAME)

#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
#define TOTAL_RS_TAG_NUM		32
#define BUNCH_RSFRAMEStartIndex 16
#define BUNCH_RS_TAG_NUM		16
#endif
/*#if ENABLE_FORCE_1CE
#define TOTAL_PARITY_GROUP_NUM 	16
#define USERDATA_RSFrameStartIndex 0
#define USERDATA_RS_Group_Num	4
#define USERDATA_RS_Group_Mask	(USERDATA_RS_Group_Num-1)
#define GC_RSFrameStartIndex 	4
#define GC_RS_Group_Num		4
#define GC_RS_Group_Mask	(GC_RS_Group_Num-1)
#define Table_RSFrameStartIndex 8
#define Table_RS_Group_Num	8
#define Table_RS_Group_Mask	(Table_RS_Group_Num-1)
#else*/
#if L06B_8GROUP_RSMAP
#define RS_GROUP_NUM			8
#define VAR_RS_GROUP_NUM			RS_GROUP_NUM
#define TABLE_RS_GROUP_NUM		4
#elif (L06B_12GROUP_RSMAP | BICS2_12GROUP_RSMAP | B0KB_12GROUP_RSMAP|Hynix3DV6_12GROUP_RSMAP)
#define RS_GROUP_NUM			12
#define VAR_RS_GROUP_NUM			RS_GROUP_NUM
#define TABLE_RS_GROUP_NUM		2
#elif ENABLE_ULTRAMLC
#define RS_GROUP_NUM			gubRSGroupNum
#define VAR_RS_GROUP_NUM		3
#define TABLE_RS_GROUP_NUM		4
#else
#define RS_GROUP_NUM			3
#define VAR_RS_GROUP_NUM			RS_GROUP_NUM
#define TABLE_RS_GROUP_NUM		4
#endif

#if B0KB_12GROUP_RSMAP
#define RS_PAGES_PER_GROUP			(1)
#else
#define RS_PAGES_PER_GROUP			(2)
#endif

#define USERDATA_RSFrameStartIndex 	(0)
#define USERDATA_RS_Group_Num		(VAR_RS_GROUP_NUM)
#define USERDATA_RS_Group_Mask		(USERDATA_RS_Group_Num-1)
#define Table_RSFrameStartIndex 	(USERDATA_RSFrameStartIndex+USERDATA_RS_Group_Num+1)
#define Table_RS_Group_Num			(TABLE_RS_GROUP_NUM)
#define Table_RS_Group_Mask			(Table_RS_Group_Num-1)
#define FailHandle_Table_RSFrameStartIndex (Table_RSFrameStartIndex+Table_RS_Group_Num)
#define FailHandle_Table_RS_Group_Num	(TABLE_RS_GROUP_NUM)
#define FailHandle_Table_RS_Group_Mask	(FailHandle_Table_RS_Group_Num-1)
#define GC_RSFrameStartIndex 		(FailHandle_Table_RSFrameStartIndex+FailHandle_Table_RS_Group_Num)
#define GC_RS_Group_Num				(VAR_RS_GROUP_NUM)
#define GC_RS_Group_Mask			(GC_RS_Group_Num-1)
#define TOTAL_PARITY_GROUP_NUM 		(USERDATA_RS_Group_Num+1+GC_RS_Group_Num+1+Table_RS_Group_Num+FailHandle_Table_RS_Group_Num)
#define RS_DEFAULT_PLANE_IDX        (0xFFFFFFFF)

#if MicronFlashOnly
    #if B0KB
        #define MAX_PAGE_PER_BLOCK 	1536
        #define MAX_FLH_PLANE_NUM	4
    #else
        #define MAX_PAGE_PER_BLOCK	1024
        #define MAX_FLH_PLANE_NUM	4
    #endif
#elif Hynix3DV6
    #define MAX_PAGE_PER_BLOCK 	1536
    #define MAX_FLH_PLANE_NUM	4
#elif Hynix3DV5
#define MAX_PAGE_PER_BLOCK	1152
#define MAX_FLH_PLANE_NUM	4
#elif YMTC_JGS
#define MAX_PAGE_PER_BLOCK	1152
#define MAX_FLH_PLANE_NUM	2
#else
    #if TLC_BICS2
		#if TSB_BICS4_SUPPORT
			#define MAX_PAGE_PER_BLOCK	1152	//BiCS4 1152Page
		#else
        	#define MAX_PAGE_PER_BLOCK	768
		#endif
        #define MAX_FLH_PLANE_NUM	2
    #elif TLC
        #define MAX_PAGE_PER_BLOCK	384
        #define MAX_FLH_PLANE_NUM	2
    #else
        #define MAX_PAGE_PER_BLOCK  256
        #define MAX_FLH_PLANE_NUM	4
    #endif
#endif

#define MAX_PLANE_PER_UNIT	(MAX_CE*MAX_FLH_PLANE_NUM*MAX_PAGE_PER_BLOCK)

#if B0KB
// Max page link length : roughly ((PagePerBlock / RS_PAGES_PER_GROUP) / RS group num)+1		ps: B0KB need more...because group is not to be used regularly
#define RS_MAX_LINK_COUNT_PER_GROUP		(((MAX_PAGE_PER_BLOCK/RS_PAGES_PER_GROUP)/VAR_RS_GROUP_NUM)+10)
// Roughly : MaxPlanePerUnit/252 + 2*RS_GROUP_NUM => make it align to RS_GROUP_NUM
#define RS_CHANGE_TAG_COUNT				(((MAX_PLANE_PER_UNIT/VAR_PARITY_DATA_PAGE_NUM + 2*VAR_RS_GROUP_NUM)/VAR_RS_GROUP_NUM)*VAR_RS_GROUP_NUM)
#define RS_CHANGE_TAG_COUNT_PER_GROUP	(RS_CHANGE_TAG_COUNT / VAR_RS_GROUP_NUM)
#else
// Max page link length : roughly ((PagePerBlock / RS_PAGES_PER_GROUP) / RS group num)+1
#define RS_MAX_LINK_COUNT_PER_GROUP		(((MAX_PAGE_PER_BLOCK/RS_PAGES_PER_GROUP)/VAR_RS_GROUP_NUM)+1)
// Roughly : MaxPlanePerUnit/252 + RS_GROUP_NUM
#define RS_CHANGE_TAG_COUNT				(MAX_PLANE_PER_UNIT / VAR_PARITY_DATA_PAGE_NUM + VAR_RS_GROUP_NUM)
#define RS_CHANGE_TAG_COUNT_PER_GROUP	(RS_CHANGE_TAG_COUNT / VAR_RS_GROUP_NUM + 1)
#endif


// plus 7 & div 8 & mul 8: make alignment, 2nd div 8 : bit to byte
#define	MAX_RS_GROUP_TAG_FAIL_MAP_LENGTH			((((VAR_RS_GROUP_NUM * RS_CHANGE_TAG_COUNT_PER_GROUP + 7) / 8) * 8) / 8)

#define MODE_FOWARD_FOR_DATA_PLANE	(2)
#define MODE_BACKWARD_FOR_P2L_PLANE	(1)
#define MODE_FAIL_RETRY_SET_FEATURE	(0)
#define MODE_FAIL_RETRY_GET_FEATURE	(1)
#define BYTES_PER_P2L_ENTRY			(4)

//#endif

#define FromXferDataIn		0
#define FromCopyData		1
#define FromCopyUnit		2
#define FromCopyRUT		3

#define BYTE_DATA_CACHE_L2P_TABLE_ENTRY_NUM 512
#define BYTE_MAX_SEARCH_GRLINK_NUM 128

#define BYTE_COMMAND_QUEUE      1024

//==========define for ftl=================
#define GR_HIT 0
#define L2P_TABLE_HIT 1
#define DDR_HIT 2
#define SET_VALUE 3
#define TABLE_SEQ 4
#define COPY_WB 5

#define BIT_POWER_CYCLE_COPY_UNIT BIT0
#define BIT_POWER_CYCLE_CLEAN_GRT  BIT1
#define BIT_POWER_CYCLE_SAVE_P2L_INIT_INFO BIT2

#define BIT_Handle_PowerCycle BIT1
#define BIT_CleanGR BIT2
#define BIT_CleanGCGR BIT3
#define BIT_Build_GCTable BIT4
#define BIT_CleanGR_Initial_Full BIT5
#define BIT_CRITICAL_GC BIT6
#define BIT_STANDBY_STOP_GC		BIT7

#define BIT_Finish_In_One_Time BIT0
#define BIT_GC_WearLeveling BIT1
#define BIT_GC_OPEN_P2L_STATIC BIT2

#define BIT_BuildGC_SetDummy		BIT0
#define BIT_BuildGC_P2LSearchTable	BIT1

#define LINK_TAIL_DEFAULT_VALUE 0xFFFFFFFF
#define MAX_ZCODE 			(7)
#define ZIP_FORCE_ZCODE		(BIT6)
#define ZIP_FORCE_DUMMYDATA  	(BIT7)
#define CrossFrame_Mark		(BIT0)
#define Program_BG				(0xB0)
#define Program_Shallow			(0xC0)
#define Program_DataIn			(0xD0)
#define Program_CopyData		(0xE0)
#define Program_CopyUnit		(0xF0)
#define GC_TABLE_TOP 0
#define GC_TABLE_BOTTOM 1

#define BIT_Initial_GR 			BIT0
#define BIT_Handle_GR_Full		BIT1

#define BIT_NORMAL_MODE         BIT1
#define BIT_CLOSE_TARGET_MODE   BIT2
#define BIT_Search_By_Die       BIT3
#define BYTE_NO_NEED_SAVE_VT    0U
#define BYTE_NEED_SAVE_VT       1U

#define BYTE_Classify_P2L_Zcode 0
#define BYTE_Classify_P2L 1
#define BYTE_Classify_GR 2

#define BYTE_Find_In_Write_Mode 1
#define BYTE_Find_In_Read_Mode 0
#define Cannot_Save_And_Load 0xffff

#define BYTE_CloseTableTarget_Normal 0
#define BYTE_CloseTableTarget_NotFullProgram 1

#define Load_Mode_Mask 0x3
#define BYTE_Load_L2P_Mode 0
#define BYTE_Load_VC_Mode 1
#define BYTE_Load_EC_Mode 2
#define BYTE_Load_HOST_Mode 			3
#define BIT_WaitLoadDone 				BIT3
#define BIT_UseCopyBuffer 				BIT4
#define BIT_CleanTableInRAMFlag 			BIT5
#define BIT_LoadInTempMode				BIT6
#define BIT_DoingTableGC 				BIT7

#define BYTE_Load_Normal_Mode 0x00
#define BYTE_Load_Write_Mode 0x01
#define BYTE_Lock_Read_Mode 		0x02
#define BYTE_Free_Read_Mode 		0x03
#define BYTE_Lock_Write_Mode		0x04
#define BYTE_Free_Write_Mode		0x05

#define LOCK_READ_MODE_NOBODY 0x00
#define LOCK_READ_MODE_R_NUM 0x10
#define LOCK_READ_MODE_P2L 0x20
#define LOCK_READ_MODE_GCTable 0x30
#define LOCK_READ_MODE_GCGR 0x40
#define LOCK_READ_MODE_BG_COPY 0x50
#define LOCK_READ_MODE_DLMC 0x60
#define LOCK_READ_MODE_Vender_SMART 0x70

#define Save_Mode_Mask 				0x7
#define BYTE_Save_L2P_Mode 0
#define BYTE_Save_VC_Mode 1
#define BYTE_Save_EC_Mode 2
#define BYTE_Save_HOST_Mode 			3
#define BYTE_Save_Dumy_Mode 			4
#define BIT_SAVE_NO_Verify				BIT3
#define FW3Byte_StartBit				8
#define U32_FW3Byte_Mask 				0xFFFFFF00

#define BYTE_VC_Load_Mode 0
#define BYTE_EC_Load_Mode 	2
#define BYTE_EC_Save_Mode 	3
#define BYTE_HOST_Load_Mode 	4
#define BYTE_HOST_Save_Mode 	5

#define LONG_P2L_Over_Limit		0xFFFFFFFF
#define LONG_P2L_Load_Fail		0xFFFFFFEE
#define BYTE_P2L_No_Limit		0xFF

#if ENABLE_TABLE_VERIFY
#define Verify_Mode_Mask 					0x3
#define BYTE_TABLE_VERIFY_VT				0
#define BYTE_TABLE_VERIFY_SAVE_TABLE		1
#define BYTE_TABLE_VERIFY_RESAVE_TABLE	2
#define BYTE_TABLE_VERIFY_COPY_TABLE		3
#define BIT_VERIFY_NO_WAIT					BIT3
#endif

#define BYTE_BG_FIND_SLC_MIN_VC	1
#define BYTE_BG_FIND_MIN_VC		2
#define BYTE_BG_FIND_MIN_EC		3
#if TLC
#define BYTE_BG_FIND_D1_MIN_VC  4
#define BYTE_BG_FIND_ALL_MODE   (BYTE_BG_FIND_SLC_MIN_VC | BYTE_BG_FIND_MIN_VC | BYTE_BG_FIND_D1_MIN_VC)
#endif

#define BYTE_BG_COPY_FIND_VICTIM	1
#define BYTE_BG_COPY_LOAD_P2L	2
#define BYTE_BG_COPY_SET_GC	3
#define BYTE_BG_COPY_READ		4
#define BYTE_BG_COPY_PROGRAM	5

#define BYTE_BG_WBQ_DONE		1
#define BYTE_BG_WBQ_UNC_STOP	2

#define BYTE_BG_LEVEL_COPY		1
#define BYTE_BG_LEVEL_IDLE		2

#define BYTE_STOP_FTL_REQUEST	1
#define BYTE_STOP_FTL_DONE		2

#define BYTE_GATING_CLOCK_HOST		0
#define BYTE_GATING_CLOCK_BG_1SEC	1
#define BYTE_GATING_CLOCK_BG		2

#define BIT_Only_Check_ListUnit	BIT0
#define BIT_Scan_L2P_Check_VC	BIT1
#define BIT_Load_P2L_Check_VC	BIT2

#define BYTE_SATACMD_FLUSH 		1
#define BYTE_STANDBY_FLUSH 		2
#define BYTE_SATASTANDBY_FLUSH 	3 // 經standby or sleep command，只會存第一次VT

#if ENABLE_ULTRAMLC
#define GC_CleanGR_Partial_Doing_Planes_Num 4
#else
#define GC_CleanGR_Partial_Doing_Planes_Num 8
#endif

#define GC_CleanGR_Partial_Doing_Planes_Mask (GC_CleanGR_Partial_Doing_Planes_Num - 1)
#define GC_CleanGR_InOneTime_Planes_Boundary_Min   16

#define ULONG_FNV_PRIME ((U32)0x01000193)
#define ULONG_FNV_Hash ((U32)0x811c9dc5)
#define BF_K 4

#if Hnyix3DV6
#define GRTable_Max_Collect_Num 0
#elif MicronFlashOnly
#define GRTable_Max_Collect_Num 0
#else
#define GRTable_Max_Collect_Num 4096
#endif

#define ulEachBuildGCTableCount 0x600

//4k Random TempMode
#define TableInTemp                                       0x8000//bit15
#define TableInTempMask                                0x7fff//bit15
#define TempModeNum				   32



#define BIT_SEARCH_BY_UNIT            BIT0
#define BIT_SEARCH_BY_INDEX           BIT1
#define BIT_SEARCH_BY_AVOID_MIXPLANE		  BIT2
#define BIT_SEARCH_BY_FORCE_MIXPLANE		  BIT3
//==============end define for ftl=================

//==========define for ftlInit======================
#define BYTE_ScanFullGR			0
#define BYTE_ScanActiveGR		1

#define BYTE_INIT_START_SCAN		1
#define BYTE_INIT_VT_RUT_VTC_DONE	2
#define BYTE_INIT_TABLE_COPY_DONE	3
#define BYTE_INIT_TABLE_DONE		4
#define BYTE_INIT_SCAN_DONE			5
#define BYTE_INIT_SCAN_DONE_PC		6
#define BYTE_INIT_PRELOAD_L2P		7
//==============end define for ftlInit===============
#define Default_Unit		0x7fff
#define Default_D1_Unit		0xFF
#define D1_UNIT_NUM_PER_LINK (gubNum_D1_Part)
#define ForceSLCPoolSizeInMB (3072) // 3G, TLC

#define NEED_UPDATE_L2P BIT31
//L2P_InRAM_Inverse
#define NoL2PTableHere 		0xffff
#define BIT_DIRTY_L2P 			BIT16
#define BIT_VALID_L2P 			BIT17
#define BIT_REFERENCE 			BIT18
#define BIT_WAITING_L2P 		BIT19
//#define FIND_WRITEMODE_MASK	(BIT_VALID_L2P | BIT_REFERENCE | BIT_WAITING_L2P)
#define FIND_WRITEMODE_MASK	(BIT_DIRTY_L2P | BIT_REFERENCE | BIT_WAITING_L2P)
#define BIT_FLAG_L2P 			BIT20
#define BIT_SAVING_L2P 			BIT21
#define WORD_WAIT_COUNT		0xFF000000
//L2P_InRAMIndex
#define TableNotInRAM  0x7fff
//Table_L2P
#define TableNotInFlash 			0x3fffff
#define TableIndex_Mask			0x3e0000
#define TableIndex_StartBit		17
#define TableTrimBit			BIT22
#define TableAllZero_StartBit		24
#define TableAllZero			0xf000000
#define TableCheckAllZero		BIT28
#define TableSeqBit			BIT29
#define SearchLinkDefault		0xC0000000
#define TableTypeNum_ExceptL2P	2

// VT ftl state
#define FTL_STATE_CleanGR_GC_WL_MiddleClean	(BIT0 | BIT1 | BIT4 | BIT7 | BIT15)
#define FTL_STATE_CleanGR_GC_WL			(BIT0 | BIT1 | BIT4)

//RUT
#define RUT_Empty				0xfefe
#define RUT_MarkBad			0xbbbb
#define ZCodeData				0x56565656
#define SpareInvalidData			0xA55A5AA5
#define AUTOPOL_TIMEOUT_FAIL	0xA

//DBT
#define DBT_MARK_READ_FAIL				0x01
#define DBT_MARK_WRITE_FAIL			0x02
#define DBT_MARK_ERASE_FAIL				0x04
#define DBT_MARK_RS_RECOVERY			0x08

// SaveInitInfo
#define BYTE_SaveInit_GRParity		BIT0
#define BYTE_SaveInit_Open_GR_P2L	BIT1
#define BYTE_SaveInit_StandBy		BIT2

// DPS
#define BIT_DPS_PowerOn_Reset	BIT0
#define BIT_DPS_Check_Level		BIT1
#define BIT_DPS_Initial			BIT2

#if TLCRUT
#define ReserveUnit			(2 * D1_UNIT_NUM_PER_LINK)
#else
#define ReserveUnit			2
#endif
#define GRSearchLinkLayer_Default_Value 0x3
#define GRSearchTable_Default_Value (0x7FFFF) // 19 bit
#define GCSearchTable_Default_Value (0xFFFFF) // 20 bit
#define BIT_CLEANGR_FIND_TABLE 0
#define BIT_CLEANGR_UPDATE_TABLE 1

#define BYTE_BQR_Check_TableInRam (1)
#define BYTE_BQR_Check_Table_Seq	(2)
#define BYTE_BQR_Search_GR            	(3)
#define BYTE_BQR_Load_Table           	(4)
#define BYTE_BQR_Load_Data           	 (5)
#define BYTE_BQR_BQ_Done              	(6)

#define BYTE_FQR_ADDR                    (1)
#define BYTE_FQR_DMA                     (2)
#define BYTE_CMD_ADDR_DONE		 (3)
#define BYTE_WAIT_DMA			(4)
#define BYTE_TRIG_DMA_DONE		 (5)
#define BYTE_FQR_READFAIL		(6)
#define BYTE_FQR_ADDRCMD		(7)
#define BYTE_WAIT_MTQFREE		(8)


#define BYTE_FQW_ADDRDMA     (0x81)
#define BYTE_FQW_BUSY     (0x82)

#define BYTE_FQ_WAITCHECK       (0xC0)
#define BYTE_FQ_CHECKDONE       (0xC1)

#define BYTE_FQ_D1TOD3DOING		(0xDD)

#define WAIT_CACHE_READY     	   0
#define WAIT_CACHE_READY_2P     1
#define WAIT_TRUE_READY	  2
#define WAIT_AUTOPOL_TIMEOUT	3

#define WAIT_CACHE_READY_POLL_FAIL	  6 	//FPU_C70_DAT_MK0F
#define WAIT_TRUE_READY_POLL_FAIL	  7 	//FPU_C70_DAT_MK0F

#define BYTE_CEW_1PLANE_CACHE_START             		1       	// wait FQ, 80h xxxxx DMA 15h WaitCacheReady
#define BYTE_CEW_1PLANE_CACHE_CONTINUOUS		2       	// wait FQ, 80h xxxxx DMA 15h WaitCacheReady // 80h xxxxx DMA 15h WaitCacheReady CheckW ...  //
#define BYTE_CEW_MULTIPLANE_DMA_START			3	// wait FQ, 80h xxxxx DMA
#define BYTE_CEW_2PLANE_CACHE_START			4	// wait FQ, 80h xxxxx DMA // 11h WaitDummyReady 81h xxxxx DMA 15h WaitCacheReady //
#define BYTE_CEW_2PLANE_DMA_CONTINUOUS 		5	// wait FQ, 80h xxxxx DMA or  81h xxxxx DMA
#define BYTE_CEW_2PLANE_CACHE_CONTINUOUS		6       	// wait FQ, 80h xxxxx DMA // 11h WaitDummyReady 81h xxxxx DMA 15h WaitCacheReady // CheckW // ...
#define BYTE_CEW_4PLANE_DMA2_CONTINUOUS		7	//
#define BYTE_CEW_4PLANE_DMA3_CONTINUOUS		8	//
#define BYTE_CEW_4PLANE_CACHE_START			9	//
#define BYTE_CEW_4PLANE_DMA5_CONTINUOUS		10	//
#define BYTE_CEW_4PLANE_DMA6_CONTINUOUS		11	//
#define BYTE_CEW_4PLANE_DMA7_CONTINUOUS		12	//
#define BYTE_CEW_4PLANE_CACHE_CONTINUOUS		13	//
#define BYTE_CEW_MULTIPLANE_CACHE_STOP		14	//

#define BYTE_CER_1PLANE_BUSY_START			21
#define BYTE_CER_1PLANE_CACHE_DMA_CONTINUOUS	22
#define BYTE_CER_1PLANE_RAND_CACHE_CONTINUOUS	23
#define BYTE_CER_2PLANE_BUSY_START			24
#define BYTE_CER_2PLANE_CACHE_DMA_CONTINUOUS	25
#define BYTE_CER_4PLANE_BUSY_START			26
#define BYTE_CER_4PLANE_CACHE_DMA_CONTINUOUS	27
#define BYTE_CER_0x3F_CMD_DONE				28
#define BYTE_CER_CACHE_REMAINDER_DMA			29

#define TOTAL_CE_PHASE_NUM					30

#define PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS			1
#define PHASE_CONTINUE_ONEPLANE_DMA_CONTINUOUS				2
#define PHASE_CONTINUE_ONEPLANE_DMA_CONTINUOUS2				3
#define PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS_BREAK    		4
#define PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS_BREAK2		5
#define PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS_BREAK3		6
#define PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS_BREAK4		7
#define PHASE_CONTINUE_ONEPLANE_DMA_CONTINUOUS_BREAK			8
#define PHASE_CONTINUE_ONEPLANE_DMA_CONTINUOUS_BREAK2		9
#define PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS_NOTSEQ 		10
#define PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS_NOTSEQ2 		11
#define PHASE_CONTINUE_MULTIPLANE_DMA_CONTINUOUS_NOTSEQ3 		12
#define PHASE_CONTINUE_ONEPLANE_DMA_CONTINUOUS_NOTSEQ 		13
#define PHASE_CONTINUE_NORMAL_READ_DMA_CONTINUOUS 			14
#define PHASE_CONTINUE_DISABLE_RANDOM_CACHE_READ_DMA_CONTINUOUS	15
#define PHASE_CONTINUE_DISABLE_CACHE_READ_DMA_CONTINUOUS	16

#define PHASE_CONTINUE_ONEPLANE_DMA_A				17
#define PHASE_CONTINUE_ONEPLANE_DMA_B				18
#define PHASE_CONTINUE_ONEPLANE_DMA_B2				19


#define STATUS_CHECK_ONE_FQ		0						// (4P/2P) 下完80h xxxxx DMA
#define STATUS_CHECK_ON_CEW_4PLANE_DMA2_CONTINUOUS		1	// (4P)      下完80h xxxxx DMA, 11h 81h xxxxx DMA
#define STATUS_CHECK_ON_CEW_4PLANE_DMA3_CONTINUOUS		2	// (4P)      下完80h xxxxx DMA, (11h 81h xxxxx DMA)*2
#define STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_START		3	// (4P/2P) 下完80h xxxxx DMA, (11h 81h xxxxx DMA)*n, 11h 81h xxxxx DMA 15;  n = 2 for 4P, n = 0 for 2P
#define STATUS_CHECK_ON_CEW_2PLANE_DMA_CONTINUOUS		4	// (2P) 	下完80h xxxxx DMA, 11h 81h xxxxx DMA 15, 80h xxxxx DMA
#define STATUS_CHECK_ON_CEW_4PLANE_DMA5_CONTINUOUS		5	// (4P)      下完80h xxxxx DMA, (11h 81h xxxxx DMA)*2, 11h 81h xxxxx DMA 15, 80h xxxxx DMA
#define STATUS_CHECK_ON_CEW_4PLANE_DMA6_CONTINUOUS		6	// (4P)      下完80h xxxxx DMA, (11h 81h xxxxx DMA)*2, 11h 81h xxxxx DMA 15, 80h xxxxx DMA, 11h 81h xxxxx DMA
#define STATUS_CHECK_ON_CEW_4PLANE_DMA7_CONTINUOUS		7	// (4P)      下完80h xxxxx DMA, (11h 81h xxxxx DMA)*2, 11h 81h xxxxx DMA 15, 80h xxxxx DMA, (11h 81h xxxxx DMA)*2
#define STATUS_CHECK_ON_CEW_MULTIPLANE_CACHE_CONTINUOUS	8	// (4P/2P) 下完80h xxxxx DMA, (11h 81h xxxxx DMA)*2, 11h 81h xxxxx DMA 15, 80h xxxxx DMA, (11h 81h xxxxx DMA)*n, 11h 81h xxxxx DMA 15


#define USE_SINGLE_MT					0
#define USE_REUSE_MT					SET_BIT0
#define GET_EXTRA_DEPTH				SET_BIT1

#define KEEP_P2LTABLE_SIZE			SET_BIT0
#define REBUILDED_P2LTABLE			SET_BIT1
#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
#define INIT_FLASH_COPY				SET_BIT2
#endif
#define RS_WRITE_BACK_TO_SOURCE_UNIT			SET_BIT3 //Copy Unit 遇到Active GR or Table 需要回填RS資訊到Active RS
#define PARTIAL_COPYUNIT			SET_BIT4 //Partial CopyUnit for ForceReadMove

#define INITINFO_READMOVE			(BIT0)
#define VTCHILD_READMOVE			(BIT1)
#define VT_READMOVE				(BIT2)

#define BYTE_NORMAL_READ				1
#define BYTE_1PLANE_CACHE_READ			2
#define BYTE_RANDOM_CACHE_READ			3
#define BYTE_2PLANE_READ_START			4
#define BYTE_2PLANE_READ				5
#define BYTE_2PLANE_CACHE_READ			6
#define BYTE_2PLANE_CACHE_READ_END		7

#define BYTE_NORMAL_WRITE				8
#define BYTE_NORMAL_WRITE_START 			9
#define BYTE_2PLANE_CACHE_WRITE			10
#define BYTE_2PLANE_WRITE_START			11

#define WAIT_RFQ_THRESHOLD	0

#define BYTE_FJOB_READ          			(1)
#define BYTE_FJOB_WRITE         			(2)
#define BYTE_FJOB_ERASE_BLOCK      			(3)
#define BYTE_FJOB_ERASE_UNIT         			(4)
#define BYTE_FJOB_SETVALUE    			(5)
#define BYTE_FJOB_COPYFROMDRAM    		(6)


#define BIT_FJOBI_FAILHANDLE      		(BIT0)
#define BIT_FJOBI_RETRY             		(BIT1)
#define BIT_FJOBI_WAITRESULT     		(BIT2)
#define BIT_FJOBI_FASTPAGE             		(BIT3)
#define BIT_FJOBI_BUFFERMODE             	(BIT4)
#define BIT_FJOBI_CACHEREADY            	(BIT5)
#define BIT_FJOBI_DIRECTACESS           	(BIT6)
#define BIT_FJOBI_PARITY            		(BIT7)
#define BIT_FJOBI_RS      			(BIT8)
#define BIT_FJOBI_CHECKINFAILQUEUE       	(BIT9)
#define BIT_FJOBI_NON_DMA       	(BIT10)
#if (TLC || ENABLE_ULTRAMLC)
#define BIT_FJOBI_TLC_TREATMENT		(BIT11)
#endif
#define BIT_FJOBI_ERASE_FAIL_RESET		(BIT12)

#define BIT_FJOBI_FAILHANDLE_RIGHTNOW		(BIT13)

#define BIT_FJOBI_RUT_D1LINK		(BIT14)
#define BIT_FJOBI_D1_UNIT			(BIT15)

#define BIT_FJOBI_HR_SETINITIAL  (BIT5) // for guwFJobInfo_HR
#define BIT_FJOBI_HR_FIRSTRETRY  (BIT9) // for guwFJobInfo_HR
#define BIT_FJOBI_HR_SOFTBITRESET  (BIT5) // for guwFJobInfo_HR


#define D1		0
#define D3		1
#if Hynix3DV6
#define UnitTLCTreatment  0x80
#else
#define UnitTLCTreatment  0x00
#endif
#define GET_MIN_EC_FREE     0
#define GET_MAX_EC_FREE     1
#define GET_2ND_MIN_EC_FREE 2
#define GET_2ND_MAX_EC_FREE 3
#define BIT_D1_TARGET		(BIT0)
#define BIT_D3_TARGET		(BIT1)
#define BIT_DIRECT_ACCESS	(BIT2)

//=======================================Write Protect State==========================================================
#define BYTE_STATE_NO_WRITEPROTECT	0
#define BYTE_STATE_WAIT_LAST_FLUSH		1
#define BYTE_STATE_WRITEPROTECT   		2

#define BIT_TRULY_WRITEPROTECT BIT0
#define BIT_GPIO_WRITEPROTECT BIT1
#define BIT_ERR_UNIT_WRITEPROTECT BIT2
#define BIT_FLASH_INIT_FAIL_WRITEPROTECT BIT3
#define BIT_TRULY_WRITEDUMMY BIT0
#define BIT_GPIO_WRITEDUMMY BIT1
//==========================================Spare Mark=============================================================
#define MARK_BeforeRead		0xDDBADBAD
#define MARK_SYS				0xFFA00000
#define MARK_SYS_RDT				0xFFA00001
#define MARK_SYS_HYNIX_RRT	0xFFA00002
#define MARK_ErrorDumpUnit             	0xFFD00000
#define MARK_DPST			0x44505354
#define MARK_VT				0xFFB00000
#define MARK_DBT            0x3FB10200
#define MARK_RUT				0xFFC00000
#define MARK_VTChild			0xFFC10000
#if EnableProductHistory
//PH_LCA
//PH_Dummy
#define MARK_PH 0x3FB10400
#define MARK_VH 0x3FB10500
#define MARK_PH_Dummy 0x3FB10600
#endif

#define MARK_TableHead			0xFFC20000
#define MARK_TableEC			0xFFC30000
#define MARK_TableVC			0xFFC40000
#define MARK_TableHost			0xFFC50000
#define MARK_TableDummy		0xFFC6FFFF
#define MARK_TableP2L			0xFFC70000
#define MARK_TableSeq			0xFFC80000

#define MARK_DataP2L			0xFFC90000
//===============================================================================================================
#define MAX_USABLEUNIT    		4096
#define MAX_TABLEUNIT			32
#define MAX_GCGRUNIT            4

#if RDT_BURNER
#define SYSTEM_FREE_SETS		(4)
#define SYSTEMAREA_DEFAULT_SCAN_RANGE	64	//因為RDT BURNER會往後找goodBlock,有可能會找到timeout,
#else
#define SYSTEM_FREE_SETS		(4)
#define SYSTEMAREA_DEFAULT_SCAN_RANGE	100	//100 Unit
#endif
#define SYSTEM_SETS                    	(2+1+1+SYSTEM_FREE_SETS)               // 2 code + dbt + system + 4
#define VTAREA_UNITS			(4)
#define MAX_CODEUNIT_NUMBER	(SYSTEM_SETS - 2)  //minus system block and DBT block
#define MAX_SYSDBT_NUMBER	(SYSTEM_SETS - 2)  //minus 2 code block
#define SYSDBT_SYS 0
#define SYSDBT_DBT 1

#define PREFORMAT_BUILD_DBT	0
#define PREFORMAT_SCAN_SYSTEMAREA	1
#define PREFORMAT_SAVE_DBT	2
#define PREFORMAT_SAVE_SYSTEM	3
#define PREFORMAT_ERASE_SYSTEMAREA 4
#define PREFORMAT_ERROR	5
#define PREFORMAT_DONE	6

#define NORMAL	0
#define TERMINAL_FAIL	1
#define LATERBAD_FAIL	2

#define GET_SYSTEM_SET_FROM_HEAD 0
#define GET_SYSTEM_SET_FROM_TAIL BIT0
#define GET_SYSTEM_SET_SKIP_SHALLOW_ERASE_CHECK BIT1

//#define VT_NoUse4Byte_Num 			0
//#define VT_NoUse4Byte_DebugIndex 		(VT_NoUse4Byte_Num - 1)

#define SRAM_BASE                0x2C000000

#define BUFFER2_START                            (0x2C000000)
#define BUFFER2_NUM                              512             // 256KB
#define BUFFER2_BASE                  	(SRAM_BASE+0x00000000)
#define BUFFER2_READ_START                            0
#define BUFFER2_READ_END                          (BUFFER2_READ_START+(BUFFER2_Read_NUM/8)-1)
#define BUFFER2_WRITE_START                            BUFFER2_READ_END+1
#define BUFFER2_WRITE_END                         (BUFFER2_WRITE_START+(BUFFER2_WRITE_NUM/8)-1)

#if BURNER
#define BUFFER3_NUM                              256          // 8KB
#if MicronFlashOnly
#define BUFFER3_BASE                           (TableZoneBase+0x4C7000)//(SDRAM_BASE+0x03000000)
#else
#define BUFFER3_BASE                           (TableZoneBase+0x4B7000)//(SDRAM_BASE+0x03000000)
#endif
#define BUFFER3_READ_START                            0
#define BUFFER3_READ_END                          (BUFFER3_READ_START+(BUFFER3_NUM/8)-1)
#define BUFFER3_WRITE_START                            0
#define BUFFER3_WRITE_END                         (BUFFER3_WRITE_START+(BUFFER3_NUM/8)-1)
#define HOST_TABLE_NUM			1024 // sector number
#elif (RDT)
/////Lin   32變大成128//////////
#define BUFFER3_NUM                              	128            // 16KB
#if MicronFlashOnly
#define BUFFER3_BASE                            	(TableZoneBase+0x4C7000)
#else
#define BUFFER3_BASE                            	(TableZoneBase+0x4B7000)
#endif
#define BUFFER3_READ_START                            0
#define BUFFER3_READ_END                          (BUFFER3_READ_START+(BUFFER3_NUM/8)-1)
#define BUFFER3_WRITE_START                            0
#define BUFFER3_WRITE_END                         (BUFFER3_WRITE_START+(BUFFER3_NUM/8)-1)
#define HOST_TABLE_NUM			1024 // sector number
#else
#define BUFFER3_NUM                              	32            // 16KB
#if MicronFlashOnly
#if B0KB
#if GR_1152KB
#define BUFFER3_BASE                            	(TableZoneBase+0x4B1000)
#else
#define BUFFER3_BASE                            	(TableZoneBase+0x489000)
#endif
#else
#define BUFFER3_BASE                            	(TableZoneBase+0x4C7000)
#endif
#else
#define BUFFER3_BASE                            	(TableZoneBase+0x4B7000)
#endif
#define BUFFER3_READ_START                            0
#define BUFFER3_READ_END                          (BUFFER3_READ_START+(BUFFER3_NUM/8)-1)
#define BUFFER3_WRITE_START                            0
#define BUFFER3_WRITE_END                         (BUFFER3_WRITE_START+(BUFFER3_NUM/8)-1)
#define HOST_TABLE_NUM			1024 // sector number
#endif

#define SDRAM_CACHE_NUM                              8192           // 4MB
#define SDRAM_CACHE_MASK                             (SDRAM_CACHE_NUM-1)
#define SDRAM_CACHE_BASE                            (Z3_DDR_BASE)
#define SDRAM_CACHE_4KNUM                              1024           // 4MB
#define SDRAM_CACHE_4KNUM_MASK                (SDRAM_CACHE_4KNUM-1)

#define BUFFER2_Read_NUM                              256             // 128KB
#define BUFFER2_Read_MASK			(BUFFER2_Read_NUM-1)
#define BUFFER2_Read_4kNUM                          32             // 128KB
#define BUFFER2_Read_4kNUM_Mask                  (BUFFER2_Read_4kNUM-1)             // 128KB

#define BUFFER2_WRITE_START_INDEX		BUFFER2_Read_NUM
#define BUFFER2_WRITE_BASE			(BUFFER2_BASE + (BUFFER2_Read_NUM*512))

#define BUFFER2_WRITE_NUM_AXIL          (BUFFER2_NUM-BUFFER2_Read_NUM) /* (512-256) = 256(128k) */
#if ENABLE_PFAIL
#define BUFFER2_WRITE_NUM               ((BUFFER2_NUM-BUFFER2_Read_NUM)/8) /* (512-256)/2 = 128(64k), (512-256)/4 = 64(32k), (512-256)/8 = 32(16K)limited to window size. */
#else
#define BUFFER2_WRITE_NUM               (BUFFER2_NUM-BUFFER2_Read_NUM) /* 512-256 = 256 */
#endif
#define BUFFER2_WRITE_MASK			(BUFFER2_WRITE_NUM-1)
#define BUFFER2_WRITE_4KNUM             (BUFFER2_WRITE_NUM/SectorsPer4K) /* 256/8 = 32EC; ultramlc(64k):16; ultramlc(32k):8 */
#define BUFFER2_WRITE_4KNUM_MASK                  (BUFFER2_WRITE_4KNUM-1)             /// 128KB
#define BUFFER2_WRITE_4K_START_INDEX		BUFFER2_Read_4kNUM

#define BUFFER2_WRITE_WINDOW_4KNUM		BUFFER2_WRITE_4KNUM /* Maximum write EC within electric capacity w/o data lost. */

/*#define BUFFER2_Read_NUM                              384             // 192KB
#define BUFFER2_Read_MASK			(BUFFER2_Read_NUM-1)
#define BUFFER2_Read_4kNUM                          48             // 192KB
#define BUFFER2_Read_4kNUM_Mask                  (BUFFER2_Read_4kNUM-1)             // 128KB

#define BUFFER2_WRITE_START_INDEX		BUFFER2_Read_NUM
#define BUFFER2_WRITE_BASE			(BUFFER2_BASE + (BUFFER2_Read_NUM*512))
#define BUFFER2_WRITE_NUM                           (BUFFER2_NUM-BUFFER2_Read_NUM)            // 128KB
#define BUFFER2_WRITE_MASK			(BUFFER2_WRITE_NUM-1)
#define BUFFER2_WRITE_4KNUM                           16            // 64KB
#define BUFFER2_WRITE_4KNUM_MASK                  (BUFFER2_WRITE_4KNUM-1)             /// 128KB
#define BUFFER2_WRITE_4K_START_INDEX		BUFFER2_Read_4kNUM
*/

////////DDR Define//////////
#define DDR_BASE 0x60000000u
#define DDR_SIZE 	32

	#define Z1_DDR_BASE 	DDR_BASE
	#define 	Z1_Size		(1024)
	#define Z2_DDR_BASE 	(Z1_DDR_BASE+Z1_Size)
	#define 	Z2_Size		(29515*1024)
	#define Z3_DDR_BASE 	(Z2_DDR_BASE+Z2_Size)	//0x61CD3000
	#define	Z3_Size		(3250*1024)
	#define TableZoneBase	(Z2_DDR_BASE+((384+63)*1024)) // Icode:384KB, Dcode:63KB	//0x60070000
	#define TableZoneSize	(((Z2_Size)/36)*32-((384+63)*1024))
	#define SZ1_DDR_BASE 0xA0000000
	#define SZ2_DDR_BASE 0x70000000
#define DataZoneBase	Z3_DDR_BASE
#define Z2_TO_SZ2_OFFSET	(SZ2_DDR_BASE-Z2_DDR_BASE)
//=============================
// DDR Phy cfg
//=============================
#define DDR3_400   0x00
#define DDR3_533   0x01
#define MRAM       0x02
#define SDR        0x03

#define SIZE_2MB   0x00
#define SIZE_8MB   0x01
#define SIZE_16MB  0x02
#define SIZE_32MB  0x03
#define SIZE_128MB 0x04
#define SIZE_256MB 0x05
#define SIZE_512MB 0x06


#define         BQ_NUM          32
#define         BQ_NUM_MASK          (BQ_NUM-1)
#define         gFQ_NUM          128
#define         gFQ_NUM_MASK        (gFQ_NUM-1)
#if (Hynix3DV6 || Hynix3DV5)
#define         FQ_NUM          16
#else
#define         FQ_NUM          8
#endif
#define         FQ_NUM_MASK          (FQ_NUM-1)
#define		L4kQ_NUM	128	// if need extend Queue num, remember to modify variable type(UBYTE->UWORD)
#define		L4kQ_NUM_MASK		(L4kQ_NUM-1)
#define		L4kQ_BIND_FACTOR	16

#define 	MaxQLength_SATA 128
#define		Spare4kSize	16
#define		Spare4kSizeLog	4
#define		Spare16kSize	(Spare4kSize*4)

#define 	Default_WB_Index        0xff


#define         SET_SSD_SIZE                 		245760		//122880//245760//131072//262144//MB
#if GR_1152KB
#define 	L2P_IN_RAM_NUM					1242	// ((8MB + (11MB + 384KB) + 32KB) / 16KB) = 1242
#define	 	GC_LOCK_L2P_FOR_DLMC		    64		// (1MB / 16KB) = 64
#define	 	GC_LOCK_L2P_FOR_Vender_SMART    8		// (128KB / 16KB) = 8
#define	 	L2P_IN_RAM_W_NUM				384		// (6MB / 16KB) = 512
#define	 	GC_LOCK_L2P_R_NUM				800		// ((12MB +512KB) / 16KB) =800
#define	 	GC_LOCK_L2P_FOR_GCGR			240		// ((3MB + 768KB) / 16KB) = 240
#define	 	GC_LOCK_L2P_FOR_BG_COPY			144		// ((1MB +128KB) / 16KB) = 72, 為了預留空間給 rebuild P2L Table
#else
#define 	 L2P_IN_RAM_NUM				1242		// ((8MB + (11MB + 384KB) + 32KB) / 16KB) = 1242
#define	 L2P_IN_RAM_W_NUM			512		// (8MB / 16KB) = 512
#define	 GC_LOCK_L2P_R_NUM			728		// ((11MB + 384KB) / 16KB) = 728
#define	 GC_LOCK_L2P_FOR_P2L			384		// (6MB / 16KB) = 384
#define	 GC_LOCK_L2P_FOR_GCTable		128		// (2MB / 16KB) = 128
#define	 GC_LOCK_L2P_FOR_GCGR		216		// ((3MB + 384KB) / 16KB) = 728
#define	 GC_LOCK_L2P_FOR_BG_COPY		128		// (1MB / 16KB) = 64, 為了預留空間給 rebuild P2L Table
#define	 GC_LOCK_L2P_FOR_DLMC		64		// (1MB / 16KB) = 64
#define	 GC_LOCK_L2P_FOR_Vender_SMART     8		// (128KB / 16KB) = 8
#endif

#define         FreeBlockCountThatNeedCloseTarget 	5
#define         FreeBlockCountThatCouldFreeToFailHandle 	(FreeBlockCountThatNeedCloseTarget + 2)
#define			FREE_BLOCK_COUNT_FOR_CRITICAL_GC 	4
#if Hynix
#define         FreeD1CountThatNeedWriteData         4
#else
#define         FreeD1CountThatNeedWriteData         2
#endif
#if TLC
    
      #define  WriteProtect_NUM               11
		#if TSB_BICS4_SUPPORT
			#define  WriteProtect_NUM_Bics4_256Gb			8	//follow toshiba
			#define  WriteProtect_NUM_Bics4_512Gb			10	//follow toshiba
		#endif    
#else
#define	 WriteProtect_NUM				5
#endif
#if ENABLE_4KSUSTAINED

#define	CLOSETARGET_FREEUNIT_THRESHELD   0

#else
#define	CLOSETARGET_FREEUNIT_THRESHELD   0
#endif
#define         BGMLCCopyThreshold 			4
#define         BGMLCCopyReleaseThreshold 		8
#define         BGSLCPoolCopyThreshold 		((1024 * 1024 / 4))// 0xb0000 (3G) //0x50000 (1.5G) // 剩下1G再啟動
#define         BGSLCPoolReleaseThreshold 		(ForceSLCPoolSizeInMB * 1024 / 4)	// 0xc0000 (3G) //0x60000 (1.5G)
#define	 BGCopyUnitThreshold			1
#if TLC
#define  BGD1ValidCntThrsHold_Div       4
#endif
#if ENABLE_DEBUG_SOFTBITCORRECTION
#define         GRUseMLCBlockThreshold		 	50000
#else
#define         GRUseMLCBlockThreshold		 	(FreeBlockCountThatNeedCloseTarget+CLOSETARGET_FREEUNIT_THRESHELD)
#endif
#define  SLCPOOL_ERASE_COUNT_THRESHOLD      (20) //TLC
#define  SLCPOOL_FREEBLOCK_COUNT_THRESHOLD      (guwFWTotalUnitNum / 4) //TLC
#if TLC
#if Hynix
#if Hynix_16nm
#define MIN_D1_UNIT_NUM 		21 /* Default_D1_Unit=0xFF */ //TLC
#elif Hynix3DV6

//V6 4Plane
//#define MIN_D1_UNIT_NUM 		27 /* Default_D1_Unit=0xFF */ //TLC
//V5 2Plane
#define MIN_D1_UNIT_NUM 		36 /* Default_D1_Unit=0xFF */ //TLC

#define MIN_2P_D1_UNIT_NUM_V5 33
#define MIN_4P_D1_UNIT_NUM_V5 17
#define MIN_4P_D1_UNIT_NUM_V6 25

#elif Hynix3DV5
#define MIN_D1_UNIT_NUM 		0xFF /* Default_D1_Unit=0xFF */ //TLC
#else
#define MIN_D1_UNIT_NUM 		26 /* Default_D1_Unit=0xFF */ //TLC
#endif
#elif TLC_BICS2
#define MIN_D1_UNIT_NUM 		19 /* Default_D1_Unit=0xFF */ //TLC_BICS2
#elif B0KB
#define MIN_D1_UNIT_NUM 		7
#define MIN_2P_D1_UNIT_NUM		11
#elif YMTC_JGS
#define MIN_D1_UNIT_NUM 		19 /* Default_D1_Unit=0xFF */ //YMTC_JGS
#else
#define MIN_D1_UNIT_NUM 		14 /* Default_D1_Unit=0xFF */ //TLC
#endif
#define MAX_D1_UNIT_NUM 		(256U-4U) /* Default_D1_Unit=0xFF */ //TLC
#if Hynix&&(!Hynix_16nm)
#if Hynix3DV6
#define D1_D3_PE_MIN_RATIO 		55U //TLC
#else
#define D1_D3_PE_MIN_RATIO 		50U //TLC
#endif
#else
#define D1_D3_PE_MIN_RATIO 		100U //TLC
#endif
#if TSB_BICS4_SUPPORT
#define D1_D3_PE_MIN_RATIO_BiCS4		70U //TLC
#endif
#else
#define MAX_D1_UNIT_NUM                        0 //TLC
#define MIN_D1_UNIT_NUM                        0
#endif
#define MIN_D1_UNIT_NUM_BICS3_128Gb 		10
#define MIN_D1_UNIT_NUM_BICS3_512Gb 		38
#define MIN_D1_UNIT_NUM_BICS4_512Gb 		34

#define         MAX_WearLeveling_Num			3
#if TLC
#define         WearLeveling_Range			50
#define         WearLeveling_Range_D1		(WearLeveling_Range>>1)
#define         WLCheckRate				32
#else
#define         WearLeveling_Range			100
#define         WLCheckRate				100
#endif
#define         COMPARE_THRESHOLD                       0
#define    WLCheckRate_Minimum				8
#if MicronFlashOnly
#if B0KB
#if GR_1152KB
#define	 GRTableSize                                   	(1152*1024)
#define	 RUTSizePerCE				0x2800 //0x2800	//	Micron : 548 block * 4 Plane * 2B = 4384B => use 8KB
#define	 RUTSize					0x20000//(RUTSizePerCE * MAX_CE)	// Maximum RUT size 8KB * 16 = 128KB
#define	 TempGRTableSize                            	(GRTableSize/32)
#define	 DataInGRSize               (512*1024)
#define	 GCP2LBufferSize                                	(2*1024*1024 + 256*1024)
#define	 GCTableSize                                    	(2*1024*1024 + 256*1024)
#define	 L2PInRamInverseSize                               8192
#else
#define	 GRTableSize                                   	(1056*1024)
#define	 RUTSizePerCE				0x2800	//	Micron : 548 block * 4 Plane * 2B = 4384B => use 8KB
#define  RUTSize                                 0x20000 //(RUTSizePerCE * MAX_CE)   // Maximum RUT size 8KB * 16 = 128KB
#define	 TempGRTableSize                            	((GRTableSize/32) + 4096 + 65536)
#define	 DataInGRSize                                    	(256*1024)
#define	 GCP2LBufferSize                                	(2*1024*1024 + 64*1024)
#define	 GCTableSize                                    	(2*1024*1024 + 64*1024)
#define	 L2PInRamInverseSize                               65536
#endif
#define	 TRIMSize					(256*1024)
#else
//L06B
#define	 RUTSizePerCE				0x3000 //12KB	//	Micron : support maxumun 6*1024 blocks per CE
#define	 RUTSize					(RUTSizePerCE * MAX_CE)	// Maximum RUT size 12KB * 16 = 192KB
#define	 TempGRTableSize                            	((GRTableSize/32) + 4096 + 65536)
#define	 TRIMSize					(192*1024)
#define	 DataInGRSize                                   (512*1024)
#define	 GRTableSize                                   	(1024*1024)
#define	 GCTableSize                                    (2*1024*1024)
#define	 GCP2LBufferSize                                (2*1024*1024)
#define	 L2PInRamInverseSize                               65536
#endif

#else
//TSB MLC/TLC/BiCS2/Hynix/Sandisk
#define	 TRIMSize					(256*1024)
#define	 GRTableSize                                   	(1024*1024)
#define 	 RUTSize					(544*1024)
#define	 RUTSizePerCE				34816 // (4096+256)Blocks*2Plane*2Byte OR  (4096+256)Blocks*4Plane*2Byte , need replaced by guwRUTSizePerCE
#define	 TempGRTableSize                            	((GRTableSize/32) + 4096)
#define	 DataInGRSize                                    	(512*1024)
#define	 GCP2LBufferSize                                	(2*1024*1024)
#define	 GCTableSize                                    	(2*1024*1024)
#define	 L2PInRamInverseSize                               65536
#if BICS2_GC_TO_D3
#define CopyBufferExtSize                                 	(1*1024*1024) // for BICS2 in L2P
#if ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING
#define	SpareValidTableSize									((CopyBufferSize + CopyBuffer2Size + CopyBufferExtSize) / 512 / 8) // for 3M copybuffer, 1 bit for 1 sector
#else
#define	SpareValidTableSize									((CopyBufferSize + CopyBufferExtSize) / 512 / 8) // for 3M copybuffer, 1 bit for 1 sector
#endif
#else
#define	SpareValidTableSize									(CopyBufferSize / 512 / 8) // for 2M copybuffer, 1 bit for 1 sector
#endif
#endif

#define	 BG_CleanGR_Threshold                      	(GRTableSize / 32 / 4)

#define	 GCGRTableSize                                   (GCTableSize/2)
#if ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING
	#define	 CopyBuffer2Size                                 	(1024*1024)	// 原本2M的CopyBufferSize, 切1M作為table unit發生program fail時拿來搬table data的buffer
	#define	 COPYBUFFER2_4KNUM			(CopyBuffer2Size/4096)
	#define	 CopyBufferSize                                 	(1024*1024)	// 1M
	#define	 COPYBUFFER_16KNUM    				(CopyBufferSize/16384)
#else
	#define	 CopyBufferSize                                 	(2*1024*1024)	// 2M
#endif
#define	 COPYBUFFER_4KNUM			(CopyBufferSize/4096)

#if (BICS2_GC_TO_D3 && ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING)
#define	 GC_COPYBUFFER_4KNUM			((CopyBufferSize + CopyBuffer2Size)/4096)	//512
#else
#define	 GC_COPYBUFFER_4KNUM			(CopyBufferSize/4096)
#endif


#define 	 BG_Clean_T_Threshold			50 		// 20 msec
#define 	 BG_Copy_T_Threshold			1000 		// 1 sec
#define 	 BG_RST_T_Threshold			2000		// 2 sec
#define 	RUT_LOG_NUM	512
#define     RUT_LOG_NUM_MASK (RUT_LOG_NUM-1)
#define 	RUT_LOGINFO_NUM	64
#define 	RUT_LOGINFO_NUM_MASK	(RUT_LOGINFO_NUM-1)
#define     ScanRUTLogToDBTMatrix			(0)
#define 	RefreshRUTLogToDBT				(BIT0)
#define 	InitialDBTMatrix					(BIT1)

#define 	BIT_NEED_UPDATE_RUT			(BIT0)
#define 	BIT_NEED_UPDATE_SYSTEMUNIT		(BIT1)
#define 	BIT_NEED_FINDDIE_SWAPRUT		(BIT2)	//要不要重新找die、要不要換RUT mapping.
									//(目前D1不用找die時，一定也不用換RUT；其它情況找die這個動作可以是DONTCARE).
#define 	BIT_NEED_PUTTO_D1ONLY		(BIT3)	//有沒有D1需要放到D1-Only.
#define 	BIT_NEED_FOCUSDIE0_GETD1ONLY	(BIT4)	//兩種意義 :
									//1.  只針對die#0，即 "(20160414)" 那種情況. (像是copy D3用到的D1 erase、prog fail，都不用舉!)
									//2.  代表補充SF時，以D1為優先；沒舉它的情況，不能從D1-Only補上來，代表該區SF必須維持是D3!
#define 	BIT_NEED_USE2ND_ERRHANDLER	(BIT5)	//使用第二個errhandler!
#define		BIT_NEED_NOT_INITDBT			(BIT6)	//在SwapRUT內發生，不能拉init DBT.

#define 	BIT_REARRANGE_RUTLOG			(BIT0)
#define 	BIT_REARRANGE_RUTSUMMARYLOG	(BIT1)

#define	PENDING_CMD_THRESHOLD		500
#define THRESHOLD_FORCE_READ        1

//=============================
// VT RMA log
//=============================
#define 	VTLogEntryNum					(1024/16)
#define 	VTTimeoutLogEntryNum			4


#define         FLHErrLogType			0
#define         DDRErrLogType			1
#define         ParityErrLogType			2
#define         StackMonitorErrLogType		3
#define         FTLErrLogType		        4
#define     	NCQTimeoutLogType		    5
#define     	ATATimeoutLogType		    6

#define 	DONTCARE					0

//FLH Err Fail Type
#define	BYTE_PF_XFERDATA        				1
#define	BYTE_INTERR_ABNORMAL_ABORTDONE        	2
#define     BYTE_PF_WRITETABLE      				3
#define     BYTE_EF_GR                             			4
#define     BYTE_EF_TABLE                   			5
#define	BYTE_DQS_MISSMATCH				6
#define     BYTE_UNC_READTABLE              			8
#define     BYTE_UNC_LOADALIGN              			9
#define     BYTE_SIGNFAIL_LOADALIGN       	      10
#define     BYTE_UNC_FAILHANDLING               	      11
#define 	BYTE_FPU_TIMEOUT				      12
#define 	BYTE_TIMEOUT_AUTOPOL				      13
#define 	BYTE_UNC_FLUSHD1				      14
#define 	BYTE_PF_FLUSHD1				      15
#define 	BYTE_PF_GCTOD3				      16
#define 	BYTE_UNC_GCTOD3				      17

//NCQ Timeout Fail Type
#define 	IsReadNCQ					0
#define 	IsWriteNCQ					1

/*
#define     BYTE_PF_COPYDATA        2
#define     BYTE_UNC_XFERDATA       6
#define     BYTE_UNC_COPYDATA       7
#define     BYTE_READMOVE_FAILHANDLING               11
#define     BYTE_MARKBAD                            12      // maybe no need...
#define     BYTE_POWERCYCLE_TABLE_REBUILD           13
#define     BYTE_POWERCYCLE_DEAD_LOOP_REBUILD       14
*/

//FTL Err Fail Type
#define	    BYTE_ON_PROBATION        				1

#define 	BYTE_P2LTable           1
#define	BYTE_RUT				0xA
#define 	BYTE_INITINFO			0xB
#define	BYTE_VT				0xC
#define	BYTE_DBT				0xD
#define 	BYTE_SYSTEMUNIT		0xE
#define 	BYTE_CODEUNIT		0xF
/*
#define 	BYTE_L2PTable           2
#define 	BYTE_FreeBlockTable     3
#define 	BYTE_HostTable          4
#define 	BYTE_FBlockInverseTable 5
#define 	BYTE_P2LTempTable       6
#define 	BYTE_DUMMY_DATA         7
*/

#define 	BIT_NormalErr				0
#define 	BIT_Program_FWSettingErr	BIT0
#define	BIT_Program_CompressionErr	BIT1
#define	BIT_Program_Lack8ByteTimeoutErr (BIT3 | BIT1)
#define	BIT_Program_E3DErr 			(BIT1 | BIT2)
#define 	BIT_Read_FWSettingErr		BIT2
#define 	BIT_Read_DecompressionErr	BIT3
#define 	BIT_Read_ErasePageErr		BIT4
#define	BIT_Read_LCAErr			BIT5
#define 	BIT_Read_CRC32Err			BIT6
#define 	BIT_Read_CRC16Err			BIT7


#define PWR_SLEEP   0xFE // spec not define sleep state, in sleep mode, device ignore or abort any command
#define PWR_IDLE    0X80 // spec is 0xff or 0x80 is ok but the drive master must use 0xff, T ask for 0x80
#define PWR_ACTIVE  0XFF
#define PWR_STANBY  0X00

#define RESET_POWER_ON	BIT0
#define RESET_HARD_SSP_ENABLE	BIT1
#define RESET_HARD_SSP_DISABLE	BIT2
#define RESET_SOFT_RTD_ENABLE	BIT3
#define RESET_SOFT_RTD_DISABLE	BIT4

//-----EraseMode-----
#define MODE_DO_ERASEALL	BIT0
#define MODE_CHECK_LATERBAD	BIT1
#define MODE_CHECK_EARLYBAD	BIT2
#define MODE_ERASE_DBT	BIT3
#define MODE_RETAIN_RUT_EC	BIT4
#define MODE_RETAIN_CODEBLOCK	BIT5
#define MODE_CHECK_ERASEFAIL	BIT6
#define MODE_DEBUG_FEATURE	BIT7
//-------------------

#define Mode_ByRemappingTable	BIT0
#define Mode_ByDBTinFlash	BIT1//Mode_ByDBTinFlash:一次從flash拉一個page DBT起來看, Mode_ByDBTinRAM:整張DBT都在RAM上了
#define Mode_ByDBTinRAM	BIT2

#define Rule_UnitPlaneA BIT0
#define Rule_Set BIT1

#define Return_FailInfo TRUE
#define Not_Return_FailInfo FALSE

#define DBT_Checked BIT0
#define DBT_Exist BIT1

#define SystemArea_Error_CodeSet 0
#define SystemArea_Error_SystemSet 1
#define SystemArea_Error_DBTSet 2

#define M_SysInfo_HW_Info		                0               // 6B
#define M_SysInfo_Separation                                    0x06
#define M_SysInfo_Month1                                        0x07
#define M_SysInfo_Month2                                        0x08
#define M_SysInfo_Month3                                        0x09
#define M_SysInfo_Day1                                          0x0A
#define M_SysInfo_Day2                                          0x0B
#define M_SysInfo_Year1                                         0x0C
#define M_SysInfo_Year2                                         0x0D
#define M_SysInfo_Year3                                         0x0E
#define M_SysInfo_Year4                                         0x0F
#define M_SysInfo_FW_Info1                                      0x10
#define M_SysInfo_FW_Info2                                      0x11
#define M_SysInfo_FW_Info3                                      0x12
#define M_SysInfo_FW_Info4                                      0x13
#define M_SysInfo_FW_Info5                                      0x14
#define M_SysInfo_FW_Info6                                      0x15
#define M_SysInfo_FW_Info7                                      0x16
#define M_SysInfo_FW_Info8                                      0x17


#define M_SysInfo_SystemAreaNumBlock_CH0    0x18	//2Bytes
#define M_SysInfo_SystemAreaNumBlock_CH1    0x1A	//2Bytes

#define M_SysInfo_FWUnitNum                 0x20	//4Bytes
#define M_SysInfo_NativeSize                0x24	//4Bytes
#define M_SysInfo_LastCodeVersion           0x28	//8Bytes
#define M_SysInfo_gubSaveVTSerialNumber     0x30	//4Bytes
#define M_SysInfo_PreformatCENum            0x34	//1Bytes
#define M_SysInfo_gubDram_Size              0x35	//1Bytes
#define M_SysInfo_gubNoPWIC                 0x36	//1Bytes
#define M_SysInfo_gubTableVersion           0x37	//1Bytes
#define M_SysInfo_gubDPSTableValid          0x38	//2Bytes
#define M_SysInfo_ulNeededUnitNum           0x40	//4Bytes
#define M_SysInfo_ubGPIOEraseDoing			0x44	//1Bytes
#define M_SysInfo_uwInitialSpareBlocks      0x46	//2Bytes
#define M_SysInfo_DBT_SET              		0x80	//1byte

#define M_SysInfo_SystemArea_RemappingRule				0x83	//1byte
#define M_SysInfo_SystemBlockRemapping_Set0_CH0		0x84	//2byte
#define M_SysInfo_SystemBlockRemapping_Set0_CH1		0x86	//2byte
#define M_SysInfo_SystemBlockRemapping_Set1_CH0		0x88	//2byte
#define M_SysInfo_SystemBlockRemapping_Set1_CH1		0x8A	//2byte
#define M_SysInfo_SystemBlockRemapping_Set2_CH0		0x8C	//2byte
#define M_SysInfo_SystemBlockRemapping_Set2_CH1		0x8E	//2byte
#define M_SysInfo_SystemBlockRemapping_Set3_CH0		0x90	//2byte
#define M_SysInfo_SystemBlockRemapping_Set3_CH1		0x92	//2byte
#define M_SysInfo_SystemBlockRemapping_Set4_CH0		0x94	//2byte
#define M_SysInfo_SystemBlockRemapping_Set4_CH1		0x96	//2byte
#define M_SysInfo_SystemBlockRemapping_Set5_CH0		0x98	//2byte
#define M_SysInfo_SystemBlockRemapping_Set5_CH1		0x9A	//2byte
#define M_SysInfo_SystemBlockRemapping_Set6_CH0		0x9C	//2byte
#define M_SysInfo_SystemBlockRemapping_Set6_CH1		0x9E	//2byte
#define M_SysInfo_SystemBlockRemapping_Set7_CH0		0xA0	//2byte
#define M_SysInfo_SystemBlockRemapping_Set7_CH1		0xA2	//2byte
#define M_SysInfo_uwSDR_window_min			0xA4	//2bytes
#define M_SysInfo_uwSDR_window_max			0xA6	//2bytes

#define M_SysInfo_ParameterPageVaild		0xF0
#define M_SysInfo_VTAreaTarget                          0xF8            // 2bytes*4Blocks
#define M_SysInfo_VTArea                                0x100           // 2bytes*4CH*IL4*2plane*4Blocks=256B

//0x400 - 0x600
#define M_SysInfo_InfoBlk                               0x400
#define M_SysInfo_ubNandTypeBiCS3			0x5AF

//0x600 - 0x800
#define M_SysInfo_SmartMPBlk 				0x600

// 0x800-0xA00
#define M_SysInfo_ParameterPage				0x800

//0xA00 - 0xC00
#define M_SysInfo_RetryTable                               0xA00
#define M_SysInfo_RetryTable_eable			0xBFF

//0xC00 - 0x1400
#define M_SysInfo_DPSTable                               0xC00

//0x1400 - 0x1900
#define M_SysInfo_RetryTable_1k                               0x1400
////////////////////////////////////////////////////////
#define Scan_All_BadBlk_Command_Support 1
#define IPM_AUTO 1              // always on
#define Security_Support 1
#define SecurityEraseNormalNeedDoRealErase 1
#define SMART_Support 1
#define Trim_Support 1
#define APM_Support 0
#define DDT_Support 1 //disalbe data transfer
#define SSP_Support 1
#define FUA_Support 1
#define DLMC_Support 1 //Download Microcode
#define DMA_Support 1
#define AutoActivate_Support 1
#define IdleUnload_Support 0
#define PowerDisable_Support 0
	#define POWER_DISABLE_ALWAYS_ENABLE 1
#define NO_PWIC      1
#define ENABLE_MULTI_DIE 1
#if (((ENABLE_SANDISK_1ZNM_SPF||HV||ENABLE_ULTRAMLC||ENABLE_FORCE_L06B_TO_B0KB||UFS||MicronFlashOnly || Hynix ||TLC_BICS2 || YMTC)) && (!RDT_BURNER))
#define ENABLE_BURNER_USE_RANDOMIZER_DECODE    0
#else
#define ENABLE_BURNER_USE_RANDOMIZER_DECODE    0
#endif

#define ENABLE_RUT2DBT     1
#define ENABLE_VDT_RESET   1

#define ENABLE_FW_DIPM     1
#define ENABLE_HW_DIPM     0
#define AP2S_WAIT_IDLE2_TIME  10;  //unit:ms

#define ENABLE_OLD_TRIM    1
#if (ENABLE_OLD_TRIM==0)
	#define D_TRIM_TABLE_NUMBER  10
#else
	#define D_TRIM_TABLE_NUMBER  0
#endif
#define ENABLE_CONTINUE_SECURITY_ERASE_AFTER_POWER_CYCLE 1

//MP default setting
#define DCO_Support 1
#define HPA_Support 1
#define AMAX_Support 0
#define Sanitize_Support 0

#define ZERO_Support 1
#define WrCache_Support 1
#define WWN_Support 1
#define LBA48_Support 1
#if (ENABLE_DEBUG_FORCE_BG_TEST || BURNER)
#define NCQ_Support 0
#else
#define NCQ_Support 1
#endif

#define WUNC_Support 1
#define DEVSLP_Support 0
#define HIPM_Support 0
#define DIPM_Support 1
#define DIPM_APS 0//0=>DIPM send slumber ; 1=>DIPM send partial & enable APS
#define  DIPM_THRESHOLD   3    /// 3ms
#define  DIPM_P2S_THRESHOLD   20    /// 20ms
#define  DIPM_AUTOP2S_THRESHOLD   1000    /// unit is 10us , so here's 10ms
#define ENABLE_XFERMODE 1
#define ENABLE_IDW129_159       1
#define ENABLE_A2_CMD 1
#define ENABLE_DLMC_RETURN_DUMMY_ABORT 1
#define ENABLE_KEEP_SMART_WHEN_KEEP_EC 1
#define ENABLE_NO_PARTIAL_ONLY_SLUMBER  0
#define ENABLE_AC_TIMING_TABLE 0
#define WRITE_PROTECT_WRITE_DUMMY_SUPPORT 1
#define ENABLE_SMART_EXTEND_SCAN_ALL 0
#define ENABLE_RUT_MIX_PLANE 0

#define THERMAL_SENSOR_MODE_EXTERNAL_33 1
#define THERMAL_SENSOR_MODE_INNER 2
#define THERMAL_SENSOR_MODE_33 3
#define THERMAL_SENSOR_MODE_MANUAL 4

#define MULTIPLANE_ONE 1
#define MULTIPLANE_TWO 2
#define MULTIPLANE_FOUR 3
#define MULTIPLANE_DEFAULT 0 //0: run ENABLE_TWOPLANE/ENABLE_FOURPLANE

#define FORM_FACTOR_2D5_INCH 3
#define FORM_FACTOR_1D8_INCH 4
#define FORM_FACTOR_LESS1D8_INCH 5
#define FORM_FACTOR_MSATA 6
#define FORM_FACTOR_MD2 7
#define FORM_FACTOR_MICROSSD 8
#define FORM_FACTOR_CFAST 9

#define SCAN_SYSTEM_ALL 0
#define SCAN_SYSTEM_SYS 1
#define SCAN_SYSTEM_DBT 2
////////////////////////////////////////////////////////
#define CUSTOMER_PHISON 0


////////////////////////////////////////////////////////

#define ENABLE_SS 1
#define ENABLE_DMAC 1

#define Enable_Test 1
#define Enable_BurnIn 0
#define Enable_Test_Function 0

#define ENABLE_FLAG_AUTOCHK 1
#define ENABLE_FLAG_AUTOSET 1


// Define For DMAC and SS
#define DMA_COPY 0
#define DMA_SV 1
#define DMA_E3D 2
#define DMA_PZIP 3
#define DMA_XOR 4
#define DMA_CMP 5
#define DMA_CRC 6

#define DMA_EN_MASK 	BIT11
#define DMA_EN_RAND	BIT12
#define DMA_EN_SETCQ	BIT15

#define SE0_MSV 0
#define SE0_INC 1
#define SE0_COPY 2
#define SE0_SEQU 3
#define SE0_SNEQ 4
#define SE0_SMORE 5
#define SE0_SLESS 6
#define SE0_SRANG 7
#define SE0_SMaxSMin 8
#define SE0_ChkSum 9
#define SE0_CRC 10
#define SE0_COneCZero 11
#define SE0_CEQU 12
#define SE0_CNEQ 13
#define SE0_CMORE 14
#define SE0_CLESS 15
#define SE0_CRANG 16
#define SE0_SV  17
#define SE0_SHSUM 18
#define SE0_RDATA 19
#define SE0_NMIN 20
#define SE0_BF 21

#define SE0_MINMAX_IGNORE BIT11
#define SE0_MINMAX_MIN BIT12
#define SE0_MINMAX_NEQ BIT13


#define SE0_SIZE_1BYTE 0<<8
#define SE0_SIZE_2BYTE 1<<8
#define SE0_SIZE_4BYTE 2<<8
#define SE0_SIZE_8BYTE 3<<8
#define SE0_ENABLE_MASK BIT10
#define SE0_ENABLE_SEARCH_BACK BIT11
#define SE0_FINDMORE	BIT12
#define SE0_TARGET(X)	((X)<<13)
#define SE0_MATCH_NUMBER_6 0<<16
#define SE0_MATCH_NUMBER_14 1<<16
#define SE0_CMDUNIT_1	1<<6
#define SE0_CMDUNIT_2	2<<6
#define SE0_CMDUNIT_3	3<<6
#define SE0_THRESHOLD(X)	(X<<24)

#define SE0_CQ_SH_STOP  BIT9

#define SE0_SHIFT0(X)	(X<<16)


#define SE0_CMD 0
#define SE0_SADDR0 1
#define SE0_LEN 2
#define SE0_bMASK0 3
#define SE0_PAT0 4
#define SE0_PAT1 5
#define SE0_PAT2 6
#define SE0_PAT3 7
#define SE0_bMASK1_L 12

#define SE0_DEP 3
#define SE0_PAT0_L 4
#define SE0_PAT0_H 5
#define SE0_bMASK0_L 6
#define SE0_bMASK0_H 7

#define SE0_INDEX0 2
#define SE0_VAL0 3

#define SE0_SH_RESULT_0 1
#define SE0_SH_RESULT_1 2
#define SE0_SH_RESULT_2 3
#define SE0_SH_RESULT_3 4

#define SectorsPer4K	8
#define SectorsPer4KMask		7
#define SectorsPer4KLog	3

#define AttributeTotalEntry 30

#if(TLC)
	#define PECYCLE_SLC	(30000)
#else
#define PECYCLE_SLC	(60000)
#endif
#define PECYCLE_ULTRA	(20000)
#define PECYCLE_MLC	(3000)
#define PECYCLE_TLC	(1000) //TSB requests TLC PE default 1000 for HP flash

#define SLCPOOL_PE_THRESHOLD (300)
#if B0KB
#define DATAINOUT_THRESHOLD (1000)
#else
#define DATAINOUT_THRESHOLD (300)
#endif

#define THEMAL_SENSOR_EXIST         0x01
#define THEMAL_SENSOR_NOT_EXIST     0x00
//////////////////////////////
#define THERMAL_SENSOR_TYPE_TMP101  0x00

//////////////////////////////
#define SEL_UDMA 0
#define SEL_MDMA 1

//////////////////////////////
#define  DMA_CRC_CAL 0
#define  DMA_CRC_CHK BIT10
#define  DMA_CRC_LEN_8 BIT11
#define  DMA_CRC_DCCM BIT12

#define HEADER_LCA 				0X31113111
#define HEADER_CRC32_SEED 			0x31113111
#define CODE_LCA 					0X55AA55AA
#define Burner_CRC32_Seed			0x55AA55AA
#define CODE_LCA_TEMP 				0X11131113
#define ReservedData_SEED    	0x82998299

#define Read_Erase_Page 0x1
#define Read_Spear_Mismatch 0x2
#define Write_Fail 0x3

#define ENABLE 				1
#define DISABLE 				0
#define RET_ERR				1
#if(!RDT)
#define RET_TIMEOUT			0xff
#endif
#define RET_OK				0

/// included by Burner


//============Burner Option=============
#define SLC_SAMPLE  	0
#define MLC_SAMPLE  	1
#define TLC_SAMPLE  	2

#define ENABLE 				1
#define DISABLE 				0
#define RET_ERR				1
#if(!RDT)
#define RET_TIMEOUT			0xff
#endif
#define RET_OK				0

//#define FOR_BOOT_IC
#define SELF_PREPARE_CODE  1


#define Enable_Code_Max_LDPC 	0
#define Enable_Only_Header		0
#define Enable_Set_Feature		1
#define Enable_Force_NVDDR		0
#define Enable_Force_Toggle1		0
#define Initial_DDR				1
#define Enable_Default_DDR_Init 0
#define Enable_InitialDDR_JustBeforeLoad	1

#define UFS_Sample_Exist			BIT4
#define UFS_Sample_Exist_Mask		0xF0
#define UFS_Sample_Default_Mode_Toggle	BIT0
#define UFS_Sample_Default_Mode_Mask	0x0F

//==============================================================================
// Header Content Ofset
//==============================================================================
#define AON_BASE 				DCCM1_BANK1_STARADDRESS
#define AON_SIZE					16384
#define HEADER_SIZE 				4096
#define AON_LOAD_CODE_RAM_SIZE		(4096 + 128)
#define AON_CRC32_SEED 			0X55AA55AA



#if (Enable_InitialDDR_JustBeforeLoad)
	#define Enable_FullInitDDR_JustBeforeLoad   1
#endif

#define Enable_DRAM_ECC		       1
#define Enable_SecondHeader_Scan	0
#define Enable_Skip_CRC_Check	0
#define Enable_Function_Code		0
#define START_CODE_BLOCK		0
#define Z1_size				(0x100000)

//============Boot Debug Option=============
#define ENABLE_FORCE_UNC		0
#define UNC_Probability			0x40
#define Enable_FORCE_CRC		0	// Code CRC Fail
#define CRC_Probability			0x50
#define Enable_Debug_Info		1
#ifdef BOOT_SIM
#define Scan_Header_Block_Num	2
#else
#define Scan_Header_Block_Num	250
#endif

#define AttributeTotalEntry 30

#define HEADERB                                  	 ((volatile UBYTE *)AON_BASE)
#define HEADERW                                  	 ((volatile UWORD *)AON_BASE)
#define HEADERL                                  	 ((volatile U32 *)AON_BASE)


#define HEADERW_HEADER                              	(0x00 >> 1)		//The ID Page Header mark to indicate this page is the ID page
	#define HEADERB_HEADER_0                        (0x00 >> 0)		//The first ID Page Header mark, must be 'I'
	#define HEADERB_HEADER_1                        (0x01 >> 0)		//The second ID PAge Header mark, must be 'D'

#define HEADERB_FW_REVISION_ID						(0x02 >> 0)		//The revision identifier of the Firmware Code, this is used to differentiate between 2 sets of Firmware Code

// LDPC_Mode
#define HEADERB_LDPC_MODE				(0x03 >> 0)	 // Load Code LDPC Mode

//Code Block Index
#define HEADERW_CH0_CODE0_Block            				(0x04 >>1)
#define HEADERW_CH0_CODE1_Block           	 				(0x06 >>1)
#define HEADERW_CH1_CODE0_Block            				(0x08 >>1)
#define HEADERW_CH1_CODE1_Block            				(0x0A >>1)

//Flash Size
#define HEADERW_PAGE_PER_BLOCK			(0x0C >>1)
#define HEADERB_PAGE_PER_BLOCK_LOG		(0x0E >>0)
#define HEADERB_SECTOR_PER_PAGE_LOG		(0x0F >>0)

//ALU
#define HEADER_ALU_RULE							0x10
#define HEADERW_ALU_RULE_W0						(0x10 >> 1)		//The ALU Rule that is to be used when the Boot Code is loading the FW Code, 16 bytes
#define HEADERW_ALU_RULE_W1						(0x12 >> 1)
#define HEADERW_ALU_RULE_W2						(0x14 >> 1)
#define HEADERW_ALU_RULE_W3						(0x16 >> 1)
#define HEADERW_ALU_RULE_W4						(0x18 >> 1)
#define HEADERW_ALU_RULE_W5						(0x1A >> 1)
#define HEADERW_ALU_RULE_W6						(0x1C >> 1)
#define HEADERW_ALU_RULE_W7						(0x1E >> 1)

// Register Setting
#define HEADERL_ADR_GEN		                    (0x20 >> 2)
#define HEADERL_ADR_GEN_STOP	                    (0x24 >> 2)
#define HEADERL_FCTL_CHNL_SET	                   	 (0x28 >> 2)	// Radomize rule, EDO
#define HEADERL_FCON_FLH_FUNC                    	 (0x2C >> 2)	// Radomize rule

//Load Code CRC 32 Check
#define HEADERB_LOAD_CODE_CRC32_CHK					(0x30 >> 0)		// 0x55 == Skip CRC32 Check
	#define CODE_CRC32_SKIP 						0x55

//Load Code Frame Per Page
#define HEADERB_LOAD_CODE_FRAME_PER_PAGE	(0x31 >> 0)		// Related to flash page size and LDPC mode

// A2
#define HEADERB_A2_SUPPORT							(0x32 >> 0)		// 0:No, 1:Support

//Default SDR DDR Initial Enable
#define HEADERB_DEFAULT_DRAM_INIT_ENABLE	(0x33 >> 0)		// 0:No, 1:Enable
#define HEADERL_DRAM_DELAYLINE			(0x34 >> 2)
#define HEADERB_DRAM_TYPE				(0x38 >> 0)
#define HEADERB_DRAM_SIZE				(0x39 >> 0)

//Load Function Code Enable
#define HEADERB_LOAD_FUNCTION_ENABLE					(0x3A >> 0)		// 0:No, 1:Enable

//Set and Get Feature Setting
#define HEADERB_INTERFACE_MODE					(0x3B >> 0)
#define HEADERB_FLASH_CLOCK					(0x3C >> 0)
#define HEADERB_FEATURE_GCMD					(0x3D >> 0)
	#define NOT_CHECK_FEATURE			0xFF
#define HEADERB_FEATURE_ADR					(0x3E >> 0)
#define HEADERB_FEATURE_B0						(0x3F >> 0)
#define HEADERB_FEATURE_B1						(0x40 >> 0)
#define HEADERB_FEATURE_B2						(0x41 >> 0)
#define HEADERB_FEATURE_B3						(0x42 >> 0)
#define HEADERB_FEATURE_POLLING_WAY				(0x43 >> 0)
	#define POLLING_70				1
	#define POLLING_RDY				2
#define HEADERL_FEATURE_DELAY					(0x44 >> 2)  //預防RDY沒有得看

// Pre and Post OP
#define HEADERL_PRE_OP_START_ADDRESS		(0x48 >> 2)
#define HEADERL_POST_OP_START_ADDRESS		(0x4C >> 2)
#define HEADERW_PRE_OP_NUM				(0x50 >> 1)
#define HEADERW_POST_OP_NUM			(0x52 >> 1)

// MDLL auto or manual
#define HEADERB_MDLL_AUTO						(0x54 >> 0)
#define HEADERB_MDLL_77_143						(0x55 >> 0)  // 0 : > 77MHz,  1 : <143 MHz
	#define EXTRA_EN_77				0
	#define EXTRA_EN_143				1
#define HEADERB_SLC_COMMAND (0x56>>0)

// SDLL Direct Value
#define HEADERL_SDLL_CH0				(0x58 >> 2)
#define HEADERL_SDLL_CH1				(0x5C >> 2)

// Clock Divide Enable
#define HEADERB_SYSDIV_ENABLE					(0x60 >> 0)
#define HEADERB_FCORDIV_ENABLE					(0x61 >> 0)
#define HEADERB_FECCDIV_ENABLE					(0x62 >> 0)
#define HEADERB_FWRITEDIV_ENABLE					(0x63 >> 0)
#define HEADERB_FREADDIV_ENABLE					(0x64 >> 0)
#define HEADERB_FCH0DIV_ENABLE					(0x65 >> 0)
#define HEADERB_FCH1DIV_ENABLE					(0x66 >> 0)

// Clock Divide Value
#define HEADERB_SYSDIV_VAL				(0x68 >> 0)
#define HEADERB_FCORDIV_VAL			(0x69 >> 0)
#define HEADERB_FECCDIV_VAL				(0x6A >> 0)
#define HEADERB_FWRITEDIV_VAL			(0x6B >> 0)
#define HEADERB_FREADDIV_VAL			(0x6C >> 0)
#define HEADERB_FCH0DIV_VAL			(0x6D >> 0)
#define HEADERB_FCH1DIV_VAL			(0x6E >> 0)

// Change PLL?
#define HEADERB_ENABLE_SET_PLL0					(0x70 >> 0)
#define HEADERB_ENABLE_SET_PLL1					(0x71 >> 0)
#define HEADERB_PLL0_BAND						(0x72 >> 0)
#define HEADERB_PLL1_BAND						(0x73 >> 0)
#define HEADERL_PLL0_DIV						(0x74 >> 2)
#define HEADERL_PLL1_DIV						(0x78 >> 2)
// PLL Source
#define HEADERL_CLKSW_CTRL0						(0x7C >> 2)
#define HEADERL_CLKSW_CTRL1						(0x80 >> 2)
#define HEADERL_CLKSW_CTRL2						(0x84 >> 2)

// DDR DQS
#define HEADERW_DQS0_FINE				(0x88 >> 1)
#define HEADERW_DQS1_FINE				(0x8A >> 1)
#define HEADERB_DQS0_G				(0x8C >> 0)
#define HEADERB_DQS1_G				(0x8D >> 0)

#define HEADERB_DA_CH0				(0x8E >> 0)
#define HEADERB_DA_CH1				(0x8F >> 0)

// Set Pad ?
#define HEADERB_ENABLE_SET_PAD					(0x90 >> 0)
// Pad value
#define HEADERL_PAD_FRDY_CFG					(0x94 >> 2)
#define HEADERL_PAD_FWPB_CFG					(0x98 >> 2)
#define HEADERL_PAD_FCEB_CFG					(0x9C >> 2)

// Change Voltage?
#define HEADERB_ENABLE_SET_VOLTAGE		(0xA0 >> 0)
#define HEADERB_VOLTAGE_VALUE			(0xA1 >> 0)
	#define FLASH_VOLTAGE_18	1
	#define FLASH_VOLTAGE_12	2

// FlaSecondScanHeader do scan or Not
#define HEADERB_ENABLE_SCAN_HEADER				(0xA4 >> 0)
#define HEADERB_TIMES_SCAN_HEADER				(0xA5 >> 0)
#define HEADERB_LIMIT_BLOCK_MASK					(0xA6 >> 0)

#define HEADERB_SDR_SCAN_CORASE						(0x1A0 >> 0)
#define HEADERW_SDR_SCAN_FINE					(0x1A2 >> 1)

// DDR Zone Setting
#define HEADERB_ECC_ENABLE				(0xB0 >> 0)
#define HEADERB_DDR_Bank				(0xB2 >> 0)
#define HEADERL_Z1_BASE				(0xB4 >> 2)
#define HEADERL_Z2_BASE				(0xB8 >> 2)
#define HEADERL_Z3_BASE				(0xBC >> 2)
#define HEADERL_Z1_SIZE				(0xC0 >> 2)
#define HEADERL_Z2_SIZE				(0xC4 >> 2)
#define HEADERL_Z3_SIZE				(0xC8 >> 2)
#define HEADERL_Z1_LIMIT				(0xCC >> 2)
#define HEADERL_Z2_LIMIT				(0xD0 >> 2)
#define HEADERL_Z3_LIMIT				(0xD4 >> 2)

// Auto DBF -> DCCM after ISP Jump
#define HEADERB_DBUF_TO_DCCM_ENABLE1				(0xE0 >> 0)
#define HEADERL_SOURCE_ADDR1					(0xE4 >> 2)
#define HEADERL_COPY_LEN1						(0xE8 >> 2)

// Function Code Auto DBF -> DCCM after ISP Jump
#define HEADERB_DBUF_TO_DCCM_ENABLE2		(0xF0 >> 0)
#define HEADERL_SOURCE_ADDR2			(0xF4 >> 2)
#define HEADERL_COPY_LEN2				(0xF8 >> 2)

// FW and Function Code Start Section, Section Num
#define HEADERB_FW_START_SECTION						(0xFC >> 0)
#define HEADERB_FW_SECTION_NUM						(0xFD >> 0)
#define HEADERB_FUNCTION_START_SECTION				(0xFE >> 0)
#define HEADERB_FUNCTION_SECTION_NUM					(0xFF >> 0)

// Each section occupy pages
#define HEADERW_S0_PAGES							(0x100 >> 1)
#define HEADERW_S1_PAGES							(0x102 >> 1)
#define HEADERW_S2_PAGES							(0x104 >> 1)
#define HEADERW_S3_PAGES							(0x106 >> 1)
#define HEADERW_S4_PAGES							(0x108 >> 1)
#define HEADERW_S5_PAGES							(0x10A >> 1)
#define HEADERW_S6_PAGES							(0x10C >> 1)
#define HEADERW_S7_PAGES							(0x10E >> 1)

// Each section start page
#define HEADERW_S0_START_PAGE			(0x110 >> 1)
#define HEADERW_S1_START_PAGE			(0x112 >> 1)
#define HEADERW_S2_START_PAGE			(0x114 >> 1)
#define HEADERW_S3_START_PAGE			(0x116 >> 1)
#define HEADERW_S4_START_PAGE			(0x118 >> 1)
#define HEADERW_S5_START_PAGE			(0x11A >> 1)
#define HEADERW_S6_START_PAGE			(0x11C >> 1)
#define HEADERW_S7_START_PAGE			(0x11E >> 1)

// Each section Destination
#define HEADERL_S0_DESTINATION						(0x120 >> 2)
#define HEADERL_S1_DESTINATION						(0x124 >> 2)
#define HEADERL_S2_DESTINATION						(0x128 >> 2)
#define HEADERL_S3_DESTINATION						(0x12C >> 2)
#define HEADERL_S4_DESTINATION						(0x130 >> 2)
#define HEADERL_S5_DESTINATION						(0x134 >> 2)
#define HEADERL_S6_DESTINATION						(0x138 >> 2)
#define HEADERL_S7_DESTINATION						(0x13C >> 2)

// Each section Size
#define HEADERL_S0_SIZE				(0x140 >> 2)
#define HEADERL_S1_SIZE				(0x144 >> 2)
#define HEADERL_S2_SIZE				(0x148 >> 2)
#define HEADERL_S3_SIZE				(0x14C >> 2)
#define HEADERL_S4_SIZE				(0x150 >> 2)
#define HEADERL_S5_SIZE				(0x154 >> 2)
#define HEADERL_S6_SIZE				(0x158 >> 2)
#define HEADERL_S7_SIZE				(0x15C >> 2)

// PAD Setting
#define HEADERL_PAD_FCLE0_CFG						(0x160 >> 2)
#define HEADERL_PAD_FCLE1_CFG						(0x164 >> 2)
#define HEADERL_PAD_FALE0_CFG						(0x168 >> 2)
#define HEADERL_PAD_FALE1_CFG						(0x16C >> 2)
#define HEADERL_PAD_FWEB0_CFG						(0x170 >> 2)
#define HEADERL_PAD_FWEB1_CFG						(0x174 >> 2)
#define HEADERL_PAD_FREB0_CFG						(0x178 >> 2)
#define HEADERL_PAD_FREB1_CFG						(0x17C >> 2)
#define HEADERL_PAD_FDQS0_CFG						(0x180 >> 2)
#define HEADERL_PAD_FDQS1_CFG						(0x184 >> 2)
#define HEADERL_PAD_FDAT0_CFG						(0x188 >> 2)
#define HEADERL_PAD_FDAT1_CFG						(0x18C >> 2)
#define HEADERL_PAD_FDATA_DVSR0						(0x190 >> 2)
#define HEADERL_PAD_FDATA_DVSR1						(0x194 >> 2)
#define HEADERL_PAD_FDATA_DVSR2						(0x198 >> 2)
#define HEADERL_PAD_FDATA_DVSR3						(0x19C >> 2)



// Initial DDR After Load ICCM and DCCM
#define HEADERB_INITIAL_DDR_JUSTBEFORE_LOAD				(0x1A4 >> 0)
#define HEADERB_NEED_INITIAL_DDR_SECTION				(0x1A5 >> 0)
#define HEADERB_DEFAULT_DRAM_INIT2_ENABLE				(0x1A6 >> 0)		// 0:No, 1:Enable
#define HEADERL_DDR_OP_START_ADDRESS					(0x1A8 >> 2)
#define HEADERW_DDR_OP_NUM						(0x1AC >> 1)
// Fast Page Rule
//Supports up to MAX_FAST_PG(256) Wordlines/Fast Pages as Code Pages
#define HEADER_MAX_FAST_PG							256
#define HEADERW_FAST_PAGE_RULE						(0x200 >> 1)		//The table that translates the logical code pages into the actual physical code pages
#define HEADERW_FAST_PAGE(X)						(HEADERW_FAST_PAGE_RULE + (X))

// OP Code
// 2048 / 16 = 128 op code
#define HEADER_OP_START							0x400		//16B Per OP,  Total 3KB
#define HEADER_OP_END								0x1000
//#include "vardef.h"

#define OP_4B	0
#define OP_2B	1
#define OP_1B	2

#define OP_SET	0x01//              0x01 Addr = value
#define OP_OR	0x02//              0x02 Addr = value | Value
#define OP_AND	0x03//              0x03 Addr = value & Value
#define OP_SET1	0x11//              0x11 Addr = Addr
#define OP_OR1	0x12//              0x12 Addr = Addr | Value
#define OP_AND1	0x13//              0x13 Addr = Addr & Value
#define OP_OR2     0x32//              0x32 Addr = Addr | Addr
#define OP_AND2	0x33//              0x33 Addr = Addr & Addr

#define OP_WHILE_OR_E 		0x04//              0x04 whie ((Addr | value) == value)
#define OP_WHILE_OR_NE 	0x05//              0x05 whie ((Addr | value) != value)
#define OP_WHILE_AND_E	0x06//              0x06 whie ((Addr & value) == value)
#define OP_WHILE_AND_NE	0x07//              0x07 whie ((Addr & value) != value)
#define OP_DELAY 			0x08//              0x08 delay

#define EC_Restore_EN		1
#define RUT_Restore_EN		1

#define EC_For_Bad          0xFFFFFFFF
#define EC_For_Sys          0xFFFF0000

//Define value for vender command of read block mapping
#define Bad_Block_Attribute		0
#define SLC_Attribute			1
#define MLC_Attribute			2
#define TLC_Attribute			3
#define FREE_BLOCK_Attribute		BIT2
#define CACHE_BLOCK_Attribute	BIT3
#define DATA_BLOCK_Attribute		BIT4
#define TABLE_BLOCK_Attribute		BIT5
#define CODE_BLOCK_Attribute		BIT6
#define NOUSE_BLOCK_Attribute	BIT7


#define ENABLE_USE_RBUFFFER_AS_COPYBUFFER  1

//=================================================================================================
//Power Cycle Test Log Define
//=================================================================================================
#define POWER_CYCLE_LOG_ARRAY_BOUND									127
//Naming rule:POWER_CYCLE_LOG_FIRST TIER FUNC_SEC. TIER FUNC_THIRD TIER FUNC.....
#define POWER_CYCLE_LOG_DIRECT_POWER_OFF									0
//ftlGuarantee_Flush
#define POWER_CYCLE_LOG_GUARANTEE_FLUSH_SAVEVT1_OUT							1
#define POWER_CYCLE_LOG_GUARANTEE_FLUSH_SAVEVT2_OUT							2
#define POWER_CYCLE_LOG_GUARANTEE_FLUSH_XFERDATAIN1_OUT							3
#define POWER_CYCLE_LOG_GUARANTEE_FLUSH_XFERDATAIN2_OUT							4 //NonDuplicate
#define POWER_CYCLE_LOG_GUARANTEE_FLUSH_XFERDATAIN3_OUT							5
#define POWER_CYCLE_LOG_GUARANTEE_FLUSH_XFERDATAIN4_OUT							6 //NonDuplicate
#define POWER_CYCLE_LOG_GUARANTEE_FLUSH_SAVEDUMMY_OUT							7
//ftlBG_Flow
#define POWER_CYCLE_LOG_BG_XFERDATAIN1_OUT									8
#define POWER_CYCLE_LOG_BG_XFERDATAIN2_OUT									9 //NonDuplicate
#define POWER_CYCLE_LOG_BG_XFERDATAIN3_OUT									10
#define POWER_CYCLE_LOG_BG_XFERDATAIN4_OUT									11 //NonDuplicate
#define POWER_CYCLE_LOG_BG_XFERDATAIN5_OUT									12
#define POWER_CYCLE_LOG_BG_XFERDATAIN6_OUT									13 //NonDuplicate
#define POWER_CYCLE_LOG_BG_SAVEDUMMY_OUT									14
#define POWER_CYCLE_LOG_BG_SAVEVT1_OUT									15
#define POWER_CYCLE_LOG_BG_SAVEVT2_OUT									16
#define POWER_CYCLE_LOG_BG_SAVEVT3_OUT									17
//ftlXferDataIn
#define POWER_CYCLE_LOG_XFDI_SAVEDUMMY1_OUT								18
#define POWER_CYCLE_LOG_XFDI_SAVEDUMMY2_OUT								19
#define POWER_CYCLE_LOG_XFDI_SAVEVT1_OUT									20
#define POWER_CYCLE_LOG_XFDI_SAVEVT2_OUT									21
//ftlGCLoadP2LTable
#define POWER_CYCLE_LOG_GCLOADP2LTABLE_SAVEVT1_OUT							22
#define POWER_CYCLE_LOG_GCLOADP2LTABLE_SAVEVT2_OUT							23
//ftlFlushD1
#define POWER_CYCLE_LOG_FLUSHD1_SAVEVT_OUT									24
//ftlCloseTarget
#define POWER_CYCLE_LOG_CLOSETARGET_SAVEVT1_OUT								25
#define POWER_CYCLE_LOG_CLOSETARGET_SAVEVT2_OUT								26
#define POWER_CYCLE_LOG_CLOSETARGET_COPYDATA_OUT								27
//ftlForceSaveDummy
#define POWER_CYCLE_LOG_FORCESAVEDUMMY_DUMMY1_OUT							28
#define POWER_CYCLE_LOG_FORCESAVEDUMMY_DUMMY2_OUT 							29 //NonDuplicate
//ftCloseTarget_Stop
#define POWER_CYCLE_LOG_CLOSETARGET_STOP_SAVEVT_OUT							30
//ftlCleanGRTable
#define POWER_CYCLE_LOG_CLEANGR_SAVETABLE1_OUT								31
#define POWER_CYCLE_LOG_CLEANGR_SAVETABLE2_OUT								32 //NonDuplicate
#define POWER_CYCLE_LOG_CLEANGR_SAVEVT1_OUT								33
#define POWER_CYCLE_LOG_CLEANGR_SAVEVT2_OUT								34
#define POWER_CYCLE_LOG_CLEANGR_SAVEVT3_OUT								35
#define POWER_CYCLE_LOG_CLEANGR_SAVEVT4_OUT								36
//ftlCloseTableTarget
#define POWER_CYCLE_LOG_CLOSETABLETARGERT_SAVEVT1_OUT							37
#define POWER_CYCLE_LOG_CLOSETABLETARGERT_SAVEVT2_OUT							38
#define POWER_CYCLE_LOG_CLOSETABLETARGERT_SAVEVT3_OUT							39
#define POWER_CYCLE_LOG_CLOSETABLETARGERT_WRITEHEADER_OUT						40
#define POWER_CYCLE_LOG_CLOSETABLETARGERT_SAVETABLE_NONL2P_OUT						41
#define POWER_CYCLE_LOG_CLOSETABLETARGERT_SAVETABLE_L2P1_OUT						42
#define POWER_CYCLE_LOG_CLOSETABLETARGERT_SAVETABLE_L2P2_OUT						43
#define POWER_CYCLE_LOG_CLOSETABLETARGERT_SAVETABLE_NEEDCNT_OUT					44 //NonDuplicate
//ftlFillTable
#define POWER_CYCLE_LOG_FILLTABLE_SAVETABLE1_OUT								45
#define POWER_CYCLE_LOG_FILLTABLE_SAVETABLE2_OUT								46
#define POWER_CYCLE_LOG_FILLTABLE_SAVETABLE3_OUT								47 //NonDuplicate
//ftlFlushL2PTable
#define POWER_CYCLE_LOG_FLUSHL2PTABLE_SAVETABLE1_OUT							48
#define POWER_CYCLE_LOG_FLUSHL2PTABLE_SAVETABLE2_OUT							49 //NonDuplicate
#define POWER_CYCLE_LOG_FLUSHL2PTABLE_SAVETABLE3_OUT							50
//ftlSwapTable
#define POWER_CYCLE_LOG_SWAPTABLE_EC_SAVETABLE_OUT							51
#define POWER_CYCLE_LOG_SWAPTABLE_HOST_SAVETABLE_OUT							52
#define POWER_CYCLE_LOG_SWAPTABLE_HOST_SAVEVT_OUT							53
//ftlSaveBunchParityToInitInfo
#define POWER_CYCLE_LOG_SAVEBUNCHPARITYINITINFO_SAVEDUMMY_OUT						54
#define POWER_CYCLE_LOG_SAVEBUNCHPARITYINITINFO_XFDI1_OUT						55
#define POWER_CYCLE_LOG_SAVEBUNCHPARITYINITINFO_XFDI2_OUT						56
#define POWER_CYCLE_LOG_SAVEBUNCHPARITYINITINFO_SAVERS_OUT						57
//ftlSaveInitInfo
#define POWER_CYCLE_LOG_SAVEINITINFO_SAVETABLEINFO1_OUT							58
#define POWER_CYCLE_LOG_SAVEINITINFO_SAVETABLEINFO2_OUT							59 //NonDuplicate
#define POWER_CYCLE_LOG_SAVEINITINFO_SAVEDATAP2L1_OUT							60
#define POWER_CYCLE_LOG_SAVEINITINFO_SAVEDATAP2L2_OUT							61 //NonDuplicate
#define POWER_CYCLE_LOG_SAVEINITINFO_SAVERSRESTPARITY1_OUT						62
#define POWER_CYCLE_LOG_SAVEINITINFO_SAVERSRESTPARITY2_OUT						63
//ftlSwapRUT
#define POWER_CYCLE_LOG_SWAPRUT_SAVEVT1_OUT								64
#define POWER_CYCLE_LOG_SWAPRUT_SAVEVT2_OUT								65
//#define POWER_CYCLE_LOG_SWAPRUT_SAVEVT3_OUT								66		// 原本SwapRUT裡面會Call兩次SaveVT, 改成Call一次SaveVT做兩次Save, 所以拿掉
#define POWER_CYCLE_LOG_SWAPRUT_COPYTABLEUNIT_OUT							67
#define POWER_CYCLE_LOG_SWAPRUT_COPYUNIT_OUT								68
#define POWER_CYCLE_LOG_SWAPRUT_UPDATERUTUNIT_OUT							69
//ftlRefreshDBTAccordingToRUT
#define POWER_CYCLE_LOG_RFDBT_WRITEDBT_OUT								70
#define POWER_CYCLE_LOG_RFDBT_WRITESYS_OUT								71
#define POWER_CYCLE_LOG_RFDBT_WRITEDPST_OUT								72
#define POWER_CYCLE_LOG_RFDBT_WRITESYSDUMMY_OUT								73
//ftlTask
#define POWER_CYCLE_LOG_FTLTASK_FTLINIT_OUT									74
#define POWER_CYCLE_LOG_FTLTASK_XFERDATAIN_OUT								75
#define POWER_CYCLE_LOG_FTLTASK_SAVEVT_OUT									76

#include "IO.h"
#include "tx_api.h"
