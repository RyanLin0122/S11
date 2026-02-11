#ifndef _RDT_H_
#define _RDT_H_


extern void RTT_ISR(void);

#define DBUF_Byte   ((volatile unsigned char *) 0x2c000000)
#define DBUF_2Byte   ((volatile unsigned short *) 0x2c000000)
#define DBUF_4Byte   ((volatile unsigned long *) 0x2c000000)


///////////////////////// RTT interrupt
#define dINTERRUPT(index)         * ( (ADDR32_t) 0x04003100+(  index>>2           ))
#define INT_STS1                       0x18
#define INT_EN1                        0x48
#define RTT1_INT_1   BIT8
#define RTT1_INT_2   BIT9
#define RTT0_INT     BIT0
#if(RDT)

///Define RDT renewed function(such as DDR)

#define ENABLE_USE_DBT_CHECK_RETEST                        1
#if  ENABLE_USE_DBT_CHECK_RETEST == 0
	#define ENABLE_ENDLOOP_IF_OVER_1024BLKS_FAIL           ENABLE_USE_DBT_CHECK_RETEST
//    #define END_LOOP_BAD_BLK_THRESHOLD 						1024   ´«¨ìRDT.c «Å§i
#else
/////  User-decided
	#define ENABLE_ENDLOOP_IF_OVER_1024BLKS_FAIL            0
//    #define END_LOOP_BAD_BLK_THRESHOLD 						 1024
#endif

////For Debug Usage
#define WRITE_SDR_LOG_TO_FLASH                   1
#define WRITE_FLASH_START_END_MARK_TO_FLASH      1
#define LOG_BLOCK_SKIP_FIRST_TWO_BLOCKS              0
#define CODEBLOCKOFFSET                                    1   /// if no offset, default value should be 1.

#define ENABLE_DEBUG_MESSAGE                                1


///DBT start address
#define DBT_START_ADDR                     0x60000000
#define MAXBADBLKS_PER_DIE_START_ADDR      0x2c03f000

////////////////////////////////////////////
///  Log debug usage
///
///  Notice : if ENABLE_ONLY_DBT_AND_ERROR_LOG set 1
///           user should only let ENABLE_ERROR_LOG = 1
///			  Other Log definitions should be set 0
////////////////////////////////////////////
#define ENABLE_ONLY_DBT_AND_ERROR_LOG                      1
#if (ENABLE_ONLY_DBT_AND_ERROR_LOG==1)
	#define SetDBT   SetDBT_BIT
	#define GetDBT   GetDBT_BIT
#else
	#define SetDBT   SetDBT_BYTE
	#define GetDBT   GetDBT_BYTE
#endif

#define ENABLE_MAX_ERR_BIT_LOG                             0
#define ENABLE_HANDLE_TEMP_TIME_LOG                        1
#define ENABLE_ERROR_LOG                                   1
#define ENABLE_DBT                                         1


////////////////////////////////////////////
///  FLASH TEST Type define
///
////////////////////////////////////////////
#define E_ALL_W_ALL_R_ALL             1
#define E_R_W_BLOCKA_E_R_W_BLOCKB     0
#define TEST_FLASH_TYPE         E_R_W_BLOCKA_E_R_W_BLOCKB


#define	Dump(x) {\
				if (ENABLE_DEBUG_MESSAGE) {x;}\
				else {}\
}


//// In Read Function, Used only in FIND_BLOCK_TASK, in order to check the block in both non-A2 and A2 cmd
extern volatile unsigned char gubNotPrintReadError;

extern volatile unsigned long long gullDDRSize;
extern volatile unsigned char  gubCEIndex;
extern volatile unsigned short  guwEntry;
extern volatile unsigned char gubPattern;   /// 0: each block E/W/R    1: all E/W/R
extern volatile unsigned char gubInVerse;   /// 0: DISABLE inverse    1: Enable
extern volatile unsigned long gulLoopTimes;
extern volatile unsigned char gubEnableA2Test; /// 0: DISABLE  1:ENABLE

#define SEL_ALL_CE()                FCONL[FCONL_FCE_ENB] = 0xFFFF;
#define DESEL_ALL_CE()              (FCONL[FCONL_FCE_ENB] = 0LL)
#define SEL_CE_BIT(ce) {\
	FCONL[FCONL_FCE_ENB] |= 1LL<<(ce);\
}
#define gubCE_Per_CH_Log         3  //without decoder


#define ENABLE 				1
#define DISABLE 				0
#define RET_ERR				1
#define RET_TIMEOUT			0x80
#define RET_OK				0
#define RET_UNC             2
#define RET_UNC_OVER_THRESHOLD             4
#define RET_PROG_FAIL             8
#define RET_CMP_FAIL              16
#define RET_OTHERFAIL             32

#define ENABLE_A2            1
#define DISABLE_A2           0
#define ENABLE_CMPCHK        1
#define DISABLE_CMPCHK        0
#define ENABLE_MAXECCBIT      1
#define DISABLE_MAXECCBIT     0
#define ENABLE_CHKEarlyBad     1
#define DISABLE_CHKEarlyBad    0


#define   LOG_EARLY_BAD_BADMARK            0x01
#define   LOG_EARLY_BAD_CNT_ONE_ERR        0x02
#define   LOG_ERASE_FAIL                   0x04    ////(including Timeout/Not returning 0xE0 During Eraseing)
#define   LOG_PROG_FAIL                    0x08    ////(including Timeout/Not returning 0xE0 During Programing)
#define   LOG_UNC_ERROR                    0x10
#define   LOG_UNC_OVER_THRESHOLD_ERROR     0x20
#define   LOG_CMP_FAIL_ERROR               0x40
#define   LOG_OTHER_FAIL_REASON            0x80    ////(including Timeout/Not returning 0xE0 During Reading)

/*
 *	In Error Logs,
 *
 *  EARLY_BAD                  01
 *  ERASE_FAIL                 02  (including Timeout/Not returning 0xE0 During Eraseing)
 *  PROG_FAIL                  03  (including Timeout/Not returning 0xE0 During Programing)
 *  UNC_ERROR                  04
 *  UNC_OVER_THRESHOLD_ERROR   05
 *  CMP_FAIL_ERROR             06
 *  OTHER_FAIL_REASOM          07  (including Timeout/Not returning 0xE0 During Reading)
 *
 *
 *  PS. IF Directly return 0x04009_64 Error status back from E/R/W function, Timeout status is at BIT15.
 *
 */


////temperature uusage
//#define bANGC(index)           * ( (ADDR8_t ) 0x04000300+(  index              ))
//#define wANGC(index)           * ( (ADDR16_t) 0x04000300+(  index>>1           ))
//#define dANGC(index)           * ( (ADDR32_t) 0x04000300+(  index>>2           ))
//#define qANGC(index)           * ( (ADDR64_t) 0x04000300+(  index>>3           ))

////Timer usage
#define bRTT(index)           * ( (volatile unsigned char* ) 0x04002600+(  index              ))
#define RTT1_CTRL                 (0x00)
#define RTT2_CTRL                 (0x01)

#define RTT1_Stop()			(bRTT(RTT1_CTRL) = (bRTT(RTT1_CTRL) & (~SYS_RTTEN)) | SYS_CNT_CLEAR)
#define RTT1_Start()			(bRTT(RTT1_CTRL) |= SYS_RTTEN)
#define RTT2_Stop()			(bRTT(RTT2_CTRL) = (bRTT(RTT2_CTRL) & (~SYS_RTTEN)) | SYS_CNT_CLEAR)
#define RTT2_Start()			(bRTT(RTT2_CTRL) |= SYS_RTTEN)

#define RTT1_Get_Value()				(SYS1L[SYS1L_RT1_CNT])
#define RTT2_Get_Value()				(SYS1L[SYS1L_RT2_CNT])
#define RTT1_GetValue()      * ( (volatile unsigned long*) 0x04002620)

#define bANGC(index)           * ( (ADDR8_t ) 0x04000300+(  index              ))


#define FLA_TIMEOUT_ERASE       5000
#define FLA_TIMEOUT_PROG        5000
#define FLA_TIMEOUT_READ        5000
#define FCTLL_DMA_ADR_0		(0x0C>>2)

///Flash setting ALU
#define ALU_SourceBit(X)	((X) << 10)
#define ALU_TargetBit(X)	((X) << 4)
#define ALU_MapBitLen(X)	(X)


/////DDR
//unsigned char entry_selfrefresh (void);
//unsigned char exit_selfrefresh (void);
unsigned char  Scan_size(UBYTE ddr_type);

////////////////////// ECC and E3D Control Rgister
#define dMISCH(index)         * ( (ADDR32_t) 0x04003000+(  index>>2           ))
#define ECC_EN			0x30
#define CR_E3D_ENABLE		(_bit7)
#define CR_E3D_DISABLE		(~_bit7)
#define TO_DDR			2000000	// 2s



extern void RTT_Interrupt_Init(void);


extern unsigned char Get_Temperature(void);

extern UBYTE SetDBT_BYTE_ForLatterHalf(U32 ulStartAddress, UWORD uwUnitIndex, U32 ulPlaneBankIndex, UBYTE ubDieNumber, UBYTE ubDBTFirstHalfPageNumber, UBYTE SetValue);
extern UBYTE SetDBT_BYTE(U32 ulStartAddress, UWORD uwUnitIndex, U32 ulPlaneBankIndex, UBYTE ubDieNumber, UBYTE SetValue);
extern UBYTE GetDBT_BYTE(U32 ulStartAddress, UWORD uwUnitIndex,U32 ulPlaneBankIndex, UBYTE ubDieNumber);
extern UBYTE SetDBT_BIT(U32 ulStartAddress, UWORD uwUnitIndex, U32 ulPlaneBankIndex, UBYTE ubDieNumber, UBYTE SetValue);
extern UBYTE GetDBT_BIT(U32 ulStartAddress, UWORD uwUnitIndex,U32 ulPlaneBankIndex, UBYTE ubDieNumber);
//extern UBYTE (*SetDBT)(U32 , UWORD , U32 , UBYTE , UBYTE );
//extern UBYTE (*GetDBT)(U32 , UWORD ,U32 , UBYTE);


extern UWORD SetDeadBLKCounterPerDie(U32 ulStartAddress, UBYTE ubChannel,UBYTE ubCEIndex , UBYTE ubDieNumber, UWORD SetValue);
extern UWORD GetDeadBLKCounterPerDie(U32 ulStartAddress, UBYTE ubChannel,UBYTE ubCEIndex , UBYTE ubDieNumber);


extern void RDT_MainTask();
extern UWORD FlaSetFastPageRule(UWORD uwPageIndex);
extern UBYTE RDT_BUILD_DBT_TASK(U32 * ulLogVariables, U32 ulDBTSize);

extern UWORD FlaEraseSingleBlock_FPU(UWORD uwUnit, UBYTE ubPlaneBank, UBYTE ubDieIndex, UBYTE ubEnableA2);
extern UWORD FlaProgramSinglePage_FPU(UWORD uwUnit, UBYTE ubPlaneBank, UBYTE ubDieIndex, UWORD uwPageIndex, UBYTE ubFrames, U32 ulSourceAddress, U32 ulLCA, UBYTE ubEnableA2, UBYTE ubEnableALLCE, UBYTE ubEnableRWDBT);
extern UWORD FlaReadSinglePage_FPU(UWORD uwUnit, UBYTE ubPlaneBank, UBYTE ubDieIndex, UWORD uwPageIndex, UBYTE ubFrames, U32 ulTargetAddress, U32 ulLCA, UBYTE ubEnableA2, UBYTE ubEnableCMPCheck,  UBYTE ubEnableMaxECCBit, UBYTE ubEnableCheckEarlyBad ,UBYTE ubEnableRWDBT);
extern UWORD FlaReadSinglePage_FPU_sortingflash(UWORD uwUnit, UBYTE ubPlaneBank, UBYTE ubDieIndex, UWORD uwPageIndex, UBYTE ubFrames, U32 ulTargetAddress, U32 ulLCA, UBYTE ubEnableA2, UBYTE ubEnableCMPCheck,  UBYTE ubEnableMaxECCBit, UBYTE ubEnableCheckEarlyBad ,UBYTE ubEnableRWDBT);

extern void RDT_FLASH_SET_REVERSE(UBYTE ubSetEnable);
extern UBYTE FlacheckEarlyBad(UWORD uwUnit, U32 ulPlaneBank, UBYTE ulDieIndex);
extern UBYTE FlaSingleBlockReliableTest(UWORD uwUnit, U32 ulPlaneBank, U32 ulLoopTimes, U32 ulBasiCMDLoopNumber);
extern UBYTE RDT_FINDBLOCKS_TASK(U32 ulFlaLoopTimes, U32 ulBasiCMDLoopNumber);
extern UBYTE RDT_FINDBLOCKS_TASK_without_relytest(U32 ulFlaLoopTimes, U32 ulBasiCMDLoopNumber);
extern UBYTE mDMAC_E3D_CMP_USAGE(U32 ulSourceAddr, U32 ulByteLength, U32 ulLBA, UBYTE ubSetCQ);
extern UBYTE FlaReadID(UBYTE ubCE, UBYTE* ubFlashID, UBYTE ubRST);
extern UBYTE RDT_SDR_TASK(U32 ulSDRLoopTimes);
extern UBYTE RDT_FLASH_TASK(UBYTE ubFlaTestType, UBYTE ubEnableA2, U32 ulLoopCount, U32 ulFlaLoopTimes, U32 * ulLogVariables);
extern UBYTE RDT_FLASH_TASK_multiplane(UBYTE ubFlaTestType, UBYTE ubEnableA2, U32 ulLoopCount, U32 ulFlaLoopTimes, U32 * ulLogVariables);


extern UBYTE FlaWriteErrorLog(U32 ulOffset, UWORD uwUnit, UBYTE ulPlaneBank, UBYTE ubFailReason, U32 ulLoopCounter, U32 ulLoopTimes, UBYTE ulCycleCounter, UBYTE ubStepTime,U32* ulBuffStartAddr);
extern UBYTE FlaHandleMaxErrorBitLog(U32 ulStartAddress, UWORD uwUnitIndex, U32 ulPlaneBankIndex, UBYTE ubDieNumber, UBYTE ubFlashCE ,UWORD SetValue, UWORD BlkAddress);

extern UWORD FlaErasePlaneBankBlock_MTQ(UWORD uwUnit, UBYTE ubDieIndex, UBYTE ubEnableRepeatWrite, UBYTE ubEnableSingleBlockTest, UBYTE ubPlaneBankIndex);
extern UWORD FlaEraseSingleBlock_MTQ(UWORD uwUnit, UBYTE ubPlaneBank, UBYTE ubDieIndex, UBYTE ubEnableA2, UBYTE ubEnableFailReasonBITMap, UBYTE ubEnableBurnerALU);
extern UWORD FlaProgramSingleBlock_MTQ(UWORD uwUnit, UBYTE ubPlaneBank, UBYTE ubDieIndex, U32 ulSourceAddress, UWORD uwPagesPerBlock,U32 ulLCA, UBYTE ubEnableA2, UBYTE ubEnableALLCE, UBYTE ubEnableRWDBT,UBYTE ubEnableProgSinglePage, UBYTE uwSinglePageIndex);
extern UWORD FlaReadSingleBlock_MTQ(UWORD uwUnit, UBYTE ubPlaneBank, UBYTE ubDieIndex, U32 ulTargetAddress, UWORD uwPagesPerBlock,U32 ulLCA, UBYTE ubEnableA2,UBYTE ubEnableCMPCheck, UBYTE ubEnableMaxECCBit, UBYTE ubEnableCheckEarlyBad, UBYTE ubEnableRWDBT, UBYTE ubEnableSinglePageRead, UBYTE uwReadPageIndex, UBYTE ubframes);
extern void PrintDBTIndex(void);

extern UWORD FlaEraseSingleBlock_FPU_2CH(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, UBYTE ubEnableA2, UBYTE ubEnableLog, UWORD uwReturnMatrix);  ////ubPlane started with 0
extern UWORD FlaProgramSingleBlock_FPU_2CH(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, U32 ulSourceAddress, UBYTE ubEnableA2, UWORD uwReturnMatrix);
extern UWORD FlaReadSingleBlock_FPU_2CH(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, U32 ulSourceAddress_00, U32 ulSourceAddress_01,UBYTE ubEnableA2, UWORD uwReturnMatrix);
extern UWORD flaReadFail_retry();
extern UWORD FlaReadSingleBlock_FPU_2CH_interleave(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, U32 ulSourceAddress_00, U32 ulSourceAddress_01,UBYTE ubEnableA2, UWORD uwReturnMatrix);
extern UWORD FlaReadSingleBlock_FPU_2CH_interleave_4plane(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, U32 ulSourceAddress_00, U32 ulSourceAddress_01, UBYTE ubEnableA2, UWORD uwReturnMatrix);

extern UWORD FlaReadSingleBlock_FPU_2CH_CheckEarlyBad(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, UWORD uwPageIndex,UWORD uwByteOffset, U32 ulSourceAddress_00, U32 ulSourceAddress_01,UBYTE ubEnableA2,UBYTE ubEnableLog,UWORD uwReturnMatrix);
extern UBYTE FlacheckEarlyBad_2CH(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex);
extern UWORD FlaEraseSingleBlock_FPU_2CH_Test2(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, UBYTE ubEnableA2, UBYTE ubEnableLog ,UWORD uwReturnMatrix);  ////ubPlane started with 0
extern UWORD FlaProgramSingleBlock_FPU_2CH_Test2(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, U32 ulSourceAddress, UBYTE ubEnableA2, UWORD uwReturnMatrix);
extern UWORD FlaProgramSingleBlock_FPU_2CH_4plane(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, U32 ulSourceAddress, UBYTE ubEnableA2, UWORD uwReturnMatrix);

extern UWORD FlaProgramSingleBlock_FPU_2CH_Test2_ALLCE(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, U32 ulSourceAddress, UBYTE ubEnableA2, UWORD uwReturnMatrix);
extern unsigned char CH_CE_PLANE__OrderTo__CH_PLANE_CE(unsigned char ubCH_CE_PLANE_Order);
extern unsigned char CH_CE_PLANE__OrderTo__PLANE_CH_CE(unsigned char ubCH_CE_PLANE_Order);

extern volatile UBYTE gubDepth;
extern void Uart_DebugDetail(UWORD uwUnit, UBYTE ubPlane, UBYTE ubDieIndex, UWORD uwPageIndex, UBYTE ubtype);
extern UBYTE FlaResetSingleCE(UBYTE ubPlaneBank);
extern void EraseByDBT(void);
extern void FlaHandleTempLog(U32 ulStartAddress, U32 ulOffset , UBYTE ubLoopNumber, UBYTE ubRDTStep);
extern void FlaAnalylizeID_FPU(void);
extern void RTT1_SetTime_ReStart();
extern void RTT1Clock_Init(UBYTE ubTimeScale);
extern void RTT2_SetTime_ReStart();
extern void RTT2Clock_Init(UBYTE ubTimeScale);
//extern unsigned char FlaResetAndSetToggleMode_ASIC(void);

#endif

#endif
