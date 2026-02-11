#include "Setup.h"
#include "parasoft.h"

#if (BURNER||(RDT))
#define ENABLE_ICODE 0
#else
#define ENABLE_ICODE 1
#endif


#ifdef TASK_DS_INSTANCE
#define EXTERN
#else
#define EXTERN extern
#endif


#define TASK_IDX_MAIN 5
#define TASK_IDX_FTL  6
#define TASK_IDX_FLA  7

EXTERN void mainTask();
EXTERN void ftlTask();
EXTERN void flaTask();

#ifndef FTL_VAR_DECLS
#define _FTL_DECL		extern
#define _FTL_BURNER_ICODE __attribute__((section(".ICODE.text"))) extern
#define _FTL_BURNER_DCODE __attribute__((section(".ICODE.data"))) extern
#if ENABLE_ICODE
#define _FTL_DECL_ICODE __attribute__((section(".ICODE.text"))) extern
#define _FTL_DECL_DCODE __attribute__((section(".ICODE.data"))) extern
#else
#define _FTL_DECL_ICODE		extern
#define _FTL_DECL_DCODE 		extern
#endif

#define _FTL_DECL_RTB_TEXT __attribute__((section(".RTB.text"))) extern
#define _FTL_DECL_RTB_DATA __attribute__((section(".RTB.data")))extern
extern UBYTE *gubZ1Cache;
extern const UBYTE sFWVerStr[];
extern const U32 gulMaskTable[32];
extern const UBYTE gub4kMaskTable[8];
extern const UBYTE gubZipTable[8];
#if YMTC
_FTL_BURNER_DCODE UBYTE gubRetryRTable_YMTC[(10 * 1) + (87 * 8)];
#endif
#if TLC_BICS2
#if TSB_BICS4_SUPPORT
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_256Gb_HV[703];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_256Gb_Early[437];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_256Gb_Final[262];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_512Gb[444];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_SANDISK_256Gb[477];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_SANDISK_512Gb[248];
#endif
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_256Gb_Normal[400];
_FTL_BURNER_DCODE UBYTE gubRetryRTable_BISC3_256Gb_Enterprise[803];
_FTL_BURNER_DCODE UBYTE gubRetryRTable_BISC3_256Gb_Industrial[400];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_128Gb[400];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC2[225];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC2_Enterprise[155];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_SANDISK[340];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_SANDISK_128Gb[273];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_SANDISK_512Gb[322];
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_TOSHIBA_512Gb_Normal[400];
_FTL_BURNER_DCODE UBYTE gubRetryRTable_BISC3_TOSHIBA_512Gb_Enterprise[400];

#endif
#if TLC
_FTL_BURNER_DCODE UBYTE gubRetryRTable_15nmTLC[308];
_FTL_BURNER_DCODE UBYTE SanDisk_sb_tlc_default_llr_table_2[96];
#endif
extern Variable_Table *VT;
extern Variable_Table *VT_Preformat_Temp;
extern D1LinkTable_t *gubD1LinkTable;
extern ValidCount_t *gulVC;
extern AON_SAVED_DATA * ReservedData;
#if(!RDT)
extern DEVSLP_Table *gDEVSLP_Table;
#endif
extern Identify_Table *IDT_temp;
#if (DEBUG_ULTRAMLC_POWER_FAIL && ENABLE_PFAIL)
extern UWORD * plane_window;
extern U32 * trace_arr;
#endif
extern EraseCount_t *gulEC_D3;
extern EraseCount_t *gulEC_D1;
extern FTarget_t *guwFreeBlockTable_D3;
extern FTarget_t *guwFreeBlockTable_D1;

extern UBYTE *gubHostTable;
extern UWORD *guwTableP2L_GC;
extern UBYTE *gubBuffer2;
extern UWORD *guwRUTBASE;
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
extern const U32 gulSeed[8];
#endif
extern UBYTE *gubCopyBuffer;
extern UBYTE *gubCopyBuffer2;
extern UBYTE *gubZ2Buffer;
extern UBYTE *gubZ2Buffer_NCS;
extern L2PTable_t *gulL2PBase;
extern UBYTE *gubL2PBase;
extern L2PTable_t *L2PTable;
extern P2LTable_t *gulGRTable;
extern P2LTable_t *gulGRTableTemp;
extern P2LTable_t *gulGCGRTable;
extern UBYTE *gubDataInGR;
extern GCTable_t *guoGCTable;
extern P2LTable_t *gulBG_GCTable;
extern P2LTable_t *gulBG_P2LTable;
extern UBYTE *gubEraseFailBitMap;

extern UWORD *guwCleanGCGRGroups;
extern GRSearchTable_t *GCGRSearchTable;
extern GRSearchTablePhysical_t *GCGRSearchTablePhysical;
extern U32 *GCGRSearchTableNext;

extern GCSearchTable_t *P2LSearchTable;
extern U32 *P2LSearchTableNext;

extern Table_L2P_t *gulTable_L2P;
extern UWORD *guwTable_VC;
extern UWORD *guwL2P_InRamIndex;
extern L2P_InRamInverse_t *gulL2P_InRamInverse;
#if ENABLE_READ_CHECK_FUNCTION
extern UWORD *guwUnitReadCnt;
extern RC_Info_t *guwUnitReadCheckInfo;
#if (Read_Chk_ByHostRead || Read_Chk_ByReadScan)
extern UWORD *guwUnitRRCnt; //byHostRead: unit read retry cnt //byReadScan: unit read scan index 0~255
#endif
extern ForceRead_Info_t *guwForceRead_Info;
#endif
extern UWORD *guwActiveTable_P2L;
extern UBYTE *gubTable_P2L_BitMap;
extern PowerCycleDebug_Info*PowerCycle;
extern U32 *GRSearchTablePhysicalPrevious;
extern UWORD *guwCleanGRGroups;
extern GRSearchTablePhysical_t *GRSearchTablePhysical;
extern U32 *GRSearchTableNext;
extern  GRSearchTable_t *GRSearchTable;
extern UBYTE *gubflaBuffer;
extern ULLONG *guoRSFrameBits;
extern U32 *gulRSFrameSpareBits;
extern UBYTE *gubSDRCache;
extern UWORD *guwDataCacheL2PTable_L1;
extern UWORD *guwDataCacheL2PTable_L2;
extern UWORD *guwFreeSDRL2PTableLink;
extern SDR_DataEntryTable_t *SDR_DataEntryTable;
extern UBYTE *gubSDRL2PEntryCount;

extern U32 *gulZByteNumSumTable;

extern UBYTE *gubGCP2LBuffer;
extern U32 *gulGRSearchLinkTail;
extern UBYTE *gubTrimTable;
extern void *gMicroCodeBuffer;
extern UWORD *guwVenderSMARTBuffer;
extern U32 *gulVenderSMARTBuffer;
extern UWORD *guwEarlyBadCntPerPlane;//MaxDie*MaxPlane
extern UBYTE *gubReserveUnitsPerGroup;//MaxDie * MaxInterleave
extern UWORD *guwUnitsPerGroups;
extern UWORD *guwExtendUnitsIndex;
extern UWORD *guwRUTUnitIndexOfLastBlock;
extern U32 *gulRUTDieCompensatePTR;// 4Die 16CE=64
extern UWORD *guwDieBlockAvg;
extern Seq_Table_t *guoSequential_PhyHead;
extern Seq_Table_1T_t *guoSequential_PhyHead_1T;

extern U32 const gulFlashIP_RegBase[5];
extern U32 const gulFSABase[MAX_CHANNEL];
extern U32 const gulMTChAdr[MAX_CHANNEL];
extern U32 const gulMTQAdr[MAX_CE_PER_CH];
extern volatile INT_t *gubINTTable;
extern volatile ZINFO_t *gubZINFO;
extern volatile SATA_MSG_TYPE3 *gulSATA_MSG_TYPE3;
extern MTQ*       gMTQ;

extern Variable_Table *VTBackUp; //debug
extern Variable_Table *VTBackUp2; //debug
#if PH_SQL_FUNC
extern UBYTE *gubNandVToolCmdBuffer;
extern UBYTE *gubNandVToolDataBuffer;
extern UWORD *guwNandVToolDataBuffer;
extern UBYTE *gubNandVToolECCBuffer;
extern U32 *gulNandVToolTimerCntBuffer;
extern UBYTE *gubNandVToolRawBuffer;
extern UBYTE *gubNandVToolSequencialRawBuffer;
#endif
extern U32 gulNandPhysicalPageSize;

#else
#define _FTL_DECL
#define _FTL_BURNER_ICODE __attribute__((section(".ICODE.text")))
#define _FTL_BURNER_DCODE __attribute__((section(".ICODE.data")))
#define _FTL_DECL_RTB_TEXT __attribute__((section(".RTB.text")))
#define _FTL_DECL_RTB_DATA __attribute__((section(".RTB.data")))
#if ENABLE_ICODE
#define _FTL_DECL_ICODE __attribute__((section(".ICODE.text")))
#define _FTL_DECL_DCODE __attribute__((section(".ICODE.data")))
#else
#define _FTL_DECL_ICODE
#define _FTL_DECL_DCODE
#endif

#if TLC_BICS2
//M2CMP01-460 BiCS FLASH Gen3 256Gb eTLC Read Retry with Set Feature Rev0.1_for CM.pdf
_FTL_BURNER_DCODE UBYTE gubRetryRTable_BISC3_256Gb_Enterprise[803] = {

0x00,

0xF8,0x08,0xF0,0x10,0xE0,0xD0,0x20,0xE8,0x18,
0xD8,0x28,0xC8,0x30,0xC0,0x38,0xB8,0x40,0xB0,
0x48,0x50,0xA8,0x58,0xA0,0x68,0x78,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // table 0

0xF9, 0xF7, 0xFA, 0xFB, 0xF7, 0xF8, 0xFB, //	1
0x07, 0x01, 0x02, 0x02, 0x01, 0x02, 0x08, //	2
0xFC, 0xF4, 0xF4, 0x00, 0xF8, 0xF4, 0xFC, //	3
0x00, 0xF0, 0xF0, 0xFC, 0x00, 0xF8, 0xF4, //	4
0x00, 0xF8, 0xEC, 0xE0, 0x00, 0xF0, 0xEC, //	5
0x00, 0xFC, 0xE4, 0xE4, 0xF8, 0xF0, 0xF0, //	6
0x04, 0xF4, 0xF0, 0xDE, 0xF8, 0xE8, 0xE0, //	7
0xFC, 0xF4, 0xF8, 0x04, 0xF8, 0xFC, 0x00, //	8
0x04, 0x00, 0xFC, 0xF8, 0x00, 0xFC, 0xFC, //	9
0xFC, 0xF8, 0xE8, 0xDC, 0xF4, 0xF8, 0x00, //	10
0x04, 0xFC, 0xE4, 0xEC, 0x00, 0xF4, 0xEC, //	11
0xFB, 0xE4, 0xEC, 0xEC, 0xE8, 0xE8, 0xE8, //	12
0x04, 0xFC, 0xF8, 0xF4, 0xFC, 0xF8, 0xF8, //	13
0xF8, 0xE8, 0xEA, 0xEC, 0xF0, 0xE8, 0xEC, //	14
0xFB, 0xEC, 0xE6, 0xE0, 0xEC, 0xE4, 0xEC, //	15
0xF8, 0xF8, 0xE6, 0xF0, 0xF8, 0xEC, 0xEC, //	16
0x08, 0x04, 0x00, 0xFC, 0x04, 0x00, 0x00, //	17
0x0C, 0x00, 0xFC, 0xFC, 0x08, 0x00, 0x00, //	18
0xF8, 0xEC, 0xDC, 0xEC, 0xF1, 0xEC, 0xF0, //	19
0xF4, 0xEC, 0xE6, 0xE4, 0xF5, 0xE6, 0xE4, //	20
0xFB, 0xE8, 0xE0, 0xE8, 0xF5, 0xEC, 0xE8, //	21
0x08, 0xF8, 0x08, 0x00, 0xF4, 0xF4, 0x00, //	22
0x00, 0xF4, 0xF4, 0xEE, 0xFC, 0xF4, 0xF0, //	23
0x08, 0x08, 0xF4, 0x00, 0xEC, 0xE8, 0xEC, //	24
0x0C, 0xF4, 0x00, 0xE4, 0xFC, 0xEC, 0xE4, //	25
0xF8, 0xE8, 0xF0, 0xE4, 0xF8, 0xF4, 0xF8, //	26
0x10, 0xF0, 0x00, 0xE4, 0x08, 0x08, 0x08, //	27
0x04, 0x04, 0x00, 0xF8, 0xFC, 0x00, 0xFC, //	28
0x00, 0xF4, 0xF8, 0xE8, 0xFC, 0xF0, 0xE4, //	29
0xFC, 0xF0, 0x04, 0xD4, 0x04, 0x04, 0x04, //	30
0xF4, 0xF1, 0xEF, 0xEA, 0xF2, 0xF2, 0xF0, //	31
0xF4, 0xF5, 0xED, 0xEE, 0xF0, 0xF0, 0xEE, //	32
0xF6, 0xEB, 0xE7, 0xE4, 0xEC, 0xE8, 0xE6, //	33
0xF4, 0xE9, 0xE5, 0xE0, 0xEC, 0xEC, 0xEA, //	34
0xF4, 0xF6, 0xF0, 0xEE, 0xF6, 0xF2, 0xF0, //	35
0xF4, 0xF4, 0xE8, 0xEC, 0xF0, 0xEC, 0xEC, //	36
0xF4, 0xEE, 0xE4, 0xE6, 0xEC, 0xEA, 0xEA, //	37
0x16, 0x13, 0x09, 0x0C, 0x14, 0x10, 0x0E, //	38
0x12, 0x13, 0x0B, 0x0A, 0x10, 0x10, 0x0E, //	39
0x14, 0x15, 0x0D, 0x0A, 0x12, 0x12, 0x10, //	40
0x16, 0x15, 0x0B, 0x08, 0x12, 0x12, 0x0C, //	41
0x0C, 0x10, 0x0C, 0x08, 0x0C, 0x10, 0x0C, //	42
0x02, 0x00, 0xFA, 0xF4, 0x00, 0x00, 0xFA, //	43
0xFC, 0x04, 0x00, 0xEC, 0xFE, 0xFA, 0xF2, //	44
0xFE, 0x00, 0xFE, 0xFA, 0x02, 0x01, 0xFC, //	45
0x0A, 0x08, 0xFE, 0xF0, 0x02, 0x00, 0xFA, //	46
0xFE, 0x02, 0xFA, 0xF2, 0x02, 0xFE, 0xF6, //	47
0x00, 0xFE, 0xFC, 0xEE, 0x00, 0xFE, 0xF6, //	48
0x02, 0xFC, 0xF6, 0xE8, 0x02, 0xFE, 0xEE, //	49
0xFE, 0xFE, 0xF6, 0xEA, 0x00, 0xFC, 0xEE, //	50
0xFE, 0xFE, 0xFC, 0xE6, 0x00, 0xFA, 0xEE, //	51
0x00, 0xF9, 0xF3, 0xF5, 0xFE, 0xFD, 0xF5, //	52
0x10, 0x09, 0x07, 0x07, 0x0E, 0x09, 0x07, //	53
0xFE, 0x01, 0x01, 0xFB, 0x02, 0x03, 0xFD, //	54
0x02, 0xFF, 0xFD, 0xEF, 0x00, 0xFF, 0xF7, //	55
0xF0, 0xF8, 0xFA, 0xFA, 0xF8, 0xF6, 0xFA, //	56
0x00, 0x08, 0x08, 0x0C, 0x04, 0x08, 0x14, //	57
0xF8, 0xFC, 0x00, 0x00, 0xFC, 0xFE, 0x00, //	58
0xFC, 0xFC, 0xFE, 0xF6, 0xFE, 0xFC, 0xFA, //	59
0xFE, 0xFE, 0xFE, 0xFA, 0x02, 0xFE, 0xFE, //	60
0xFC, 0xFC, 0xFC, 0xF4, 0x00, 0xFC, 0xFA, //	61
0xFE, 0xFA, 0xF8, 0xF2, 0xFE, 0xFA, 0xF6, //	62
0x02, 0xFE, 0x04, 0xF6, 0x00, 0xFC, 0xF6, //	63
0xFE, 0xFA, 0x04, 0xF0, 0xFE, 0xF8, 0xF4, //	64
0x06, 0x02, 0x04, 0xF2, 0x04, 0xFE, 0xFA, //	65
0x02, 0xFE, 0x00, 0xF2, 0x04, 0xFA, 0xF8, //	66
0xFE, 0x02, 0x00, 0xEE, 0x06, 0xFE, 0xF6, //	67
0x06, 0xFE, 0x00, 0xEE, 0x04, 0xFE, 0xF2, //	68
0x06, 0xFE, 0xFC, 0xEA, 0x02, 0xFC, 0xF2, //	69
0x02, 0x06, 0xFC, 0xEA, 0x06, 0x00, 0xF4, //	70
0x00, 0x02, 0xFE, 0xEA, 0x04, 0x00, 0xEE, //	71
0xF6, 0x04, 0xF4, 0xE6, 0x02, 0xFE, 0xEE, //	72
0x10, 0xFC, 0x10, 0xF0, 0xFE, 0xFA, 0xF8, //	73
0x0E, 0xF0, 0x0C, 0xE6, 0xF4, 0xEE, 0xEE, //	74
0xF6, 0x0E, 0xF8, 0x06, 0x0E, 0x0C, 0x0A, //	75
0x00, 0x0A, 0x04, 0x02, 0x0A, 0x06, 0x02, //	76
0x02, 0xF6, 0x06, 0xEE, 0xFA, 0xF6, 0xF4, //	77
0x00, 0x0C, 0x06, 0x06, 0x00, 0xFC, 0xFE, //	78
0xFE, 0x00, 0x08, 0xFC, 0x0C, 0x08, 0x08, //	79
0x04, 0x04, 0x08, 0xFE, 0x04, 0x01, 0x00, //	80
0x0A, 0x02, 0x08, 0xFC, 0x02, 0x00, 0xFE, //	81
0x02, 0x06, 0xF4, 0xFE, 0x04, 0x00, 0xFE, //	82
0x00, 0x06, 0xF8, 0xFA, 0x02, 0x02, 0x00, //	83
0x0A, 0x02, 0xFC, 0xFE, 0x04, 0x00, 0x00, //	84
0x08, 0x02, 0x00, 0x00, 0x06, 0x02, 0x00, //	85
0xFA, 0xF6, 0xEC, 0xE4, 0xFE, 0xF0, 0xEA, //	86
0xF8, 0xF8, 0xF0, 0xEA, 0xFE, 0xF2, 0xEE, //	87
0xFC, 0xFE, 0xFA, 0xEC, 0x06, 0xF8, 0xF2, //	88
0x02, 0x00, 0x02, 0xF2, 0x06, 0xFA, 0xF6, //	89
0x04, 0xEE, 0x02, 0xDC, 0xF6, 0xE8, 0xE2, //	90
0xFE, 0xF0, 0xFC, 0xE2, 0xF6, 0xEA, 0xE6, //	91
0x0A, 0xFC, 0xFC, 0xE6, 0xFC, 0xF4, 0xF0, //	92
0x06, 0x04, 0xF8, 0xF4, 0x02, 0xFE, 0xFA, //	93
0x00, 0x00, 0x02, 0xF0, 0x00, 0xFA, 0xF6, //	94
0x0E, 0x00, 0x04, 0xEC, 0x02, 0xFA, 0xF2, //	95
0x04, 0xFA, 0xEC, 0xEA, 0x00, 0xF6, 0xEE, //	96
0x0E, 0xF8, 0xFC, 0xDE, 0xFE, 0xF2, 0xEA, //	97
0x0B, 0x00, 0xF2, 0xF6, 0x02, 0xFC, 0xFA, //	98
0x02, 0x04, 0xF0, 0xFA, 0x06, 0x00, 0xFE, //	99
0x04, 0xF2, 0xF2, 0xD4, 0xFC, 0xEC, 0xE0, //	100
0x06, 0x00, 0xF4, 0xE8, 0x07, 0xFA, 0xEE, //	101
0xFC, 0xFA, 0xF2, 0xDE, 0x00, 0xF3, 0xE8, //	102
0xFA, 0xF6, 0xF6, 0xD2, 0xFC, 0xEE, 0xDE, //	103
0x08, 0xF6, 0x0E, 0xD6, 0xFE, 0xF2, 0xE6, //	104
0x0A, 0xFE, 0xF6, 0xE2, 0x02, 0xF4, 0xEA, //	105
0x0C, 0xF4, 0x0A, 0xDE, 0xF8, 0xF0, 0xEC, //	106
0xFC, 0xF2, 0x04, 0xEA, 0xF6, 0xF4, 0xF4, //	107
0xF8, 0x06, 0xFA, 0x0A, 0x06, 0x08, 0x0C, //	108
0xF4, 0x0A, 0xFC, 0xF2, 0x0A, 0xFC, 0xF4, //	109
0xF4, 0x0C, 0x04, 0xF6, 0x0C, 0x04, 0x00  //	110
};

//M2CMP01-516_BiCS3 Industrial 256Gb TLC_pSLC Read Retry Rev1.0.pdf
_FTL_BURNER_DCODE UBYTE gubRetryRTable_BISC3_256Gb_Industrial[400] =
{
0x00,
// Parameter setting table for pSLC 7Cycle, each 1 Byte
0xF8,0x08,0xF0,0xE8,0x18,0xD8,0x28,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // table 0

0xF1, 0xF7, 0xF6, 0xF5, 0xF8, 0xF7, 0xF4, //	1
0xED, 0xF3, 0xF4, 0xF7, 0xF2, 0xF1, 0xF4, //	2
0xEB, 0xF1, 0xFA, 0x03, 0xF0, 0xF5, 0x02, //	3
0xFD, 0xFF, 0x04, 0x05, 0x00, 0x03, 0x04, //	4
0xE7, 0xEB, 0xF8, 0xFB, 0xFA, 0xFB, 0x00, //	5
0xF5, 0xF9, 0xFE, 0xF7, 0xEE, 0xF3, 0x04, //	6
0xFF, 0xF5, 0xFA, 0xF3, 0xEC, 0xED, 0xFE, //	7
0xF7, 0xED, 0xFA, 0xFF, 0xFA, 0xF9, 0xFE, //	8
0xE9, 0xF9, 0xF6, 0xEF, 0x04, 0x04, 0x08, //	9
0xF5, 0xEF, 0xF2, 0xFB, 0xF8, 0xF9, 0xFA, //	10
0xE5, 0xF1, 0xFE, 0xEF, 0xF6, 0xF5, 0xFA, //	11
0xE9, 0xE9, 0xFC, 0xF7, 0xEC, 0xF1, 0xFE, //	12
0xF5, 0xF3, 0x02, 0x07, 0xF4, 0xF9, 0x04, //	13
0xF9, 0xF5, 0xFE, 0x03, 0xFE, 0xF9, 0xFA, //	14
0x01, 0x04, 0x02, 0x0C, 0xFC, 0xF9, 0xFA, //	15
0xFB, 0x01, 0xFC, 0x03, 0x00, 0xFF, 0xFE, //	16
0x01, 0xFF, 0x06, 0xF9, 0xEA, 0xED, 0xFA, //	17
0x04, 0xED, 0x04, 0xF7, 0xFC, 0xFB, 0xFA, //	18
0xF3, 0x01, 0xF8, 0x07, 0x06, 0x06, 0x0C, //	19
0x00, 0xF9, 0x0C, 0x03, 0xF4, 0xF9, 0x00, //	20
0xE5, 0xF9, 0xFA, 0x07, 0xFA, 0xFB, 0xFC, //	21
0xFC, 0xED, 0x08, 0x03, 0xF2, 0xF5, 0xFE, //	22
0xF9, 0x04, 0xF6, 0xFC, 0xF6, 0xF5, 0xF6, //	23
0xEF, 0xF9, 0xF8, 0xFF, 0xF0, 0xED, 0xEE, //	24
0xF9, 0xF9, 0x06, 0xF3, 0xF0, 0xF5, 0xEE, //	25
0xF9, 0xE5, 0xEE, 0xE3, 0xFA, 0xFD, 0xF8, //	26
0xFB, 0xF5, 0xF8, 0xEB, 0xE8, 0xED, 0xF8, //	27
0xED, 0xED, 0xFC, 0xE7, 0xEC, 0xE9, 0xE8, //	28
0xEF, 0x01, 0xEC, 0xFB, 0x04, 0x01, 0x06, //	29
0xE9, 0xE9, 0x02, 0xEB, 0xE4, 0xDD, 0xE0, //	30
0xF3, 0xED, 0xFC, 0xF3, 0xFA, 0xED, 0xF6, //	31
0xEF, 0xE9, 0xFE, 0xE3, 0xEA, 0xE5, 0xEE, //	32
0xF9, 0xE5, 0x02, 0xE7, 0x00, 0x01, 0x02, //	33
0x04, 0xFB, 0xFC, 0xF9, 0xFE, 0xFB, 0xFC, //	34
0xE9, 0xE1, 0xE6, 0xEF, 0xF4, 0xF5, 0xEA, //	35
0xE5, 0x06, 0xF4, 0x10, 0xF8, 0xF5, 0xF8, //	36
0xF3, 0xF9, 0xEC, 0xFB, 0xFA, 0xF5, 0xF6, //	37
0xED, 0xF1, 0xF0, 0xF9, 0xF4, 0xF3, 0xF4, //	38
0xF1, 0xE5, 0xF2, 0xEF, 0xF8, 0xED, 0xEE, //	39
0xF5, 0xF5, 0xEE, 0xFD, 0xF8, 0xF1, 0xF2, //	40
0xF9, 0x04, 0xF2, 0x08, 0xF6, 0xED, 0xF2, //	41
0xE9, 0xF1, 0xF0, 0xF3, 0xF2, 0xF1, 0xEE, //	42
0xED, 0xE9, 0xE2, 0xF3, 0xF4, 0xED, 0xEA, //	43
0xF1, 0xE9, 0xEE, 0xEF, 0xE8, 0xE5, 0xEA, //	44
0xED, 0xDF, 0xEA, 0xE9, 0x02, 0x02, 0x06, //	45
0xF5, 0xF5, 0xF6, 0xE7, 0xF8, 0xF5, 0xF4, //	46
0xE5, 0xED, 0xEA, 0xEB, 0xEC, 0xE5, 0xE6, //	47
0xE5, 0xF1, 0xDE, 0xEB, 0xE0, 0xDD, 0xE6, //	48
0xE9, 0xF1, 0xEA, 0xE7, 0xE4, 0xDF, 0xEA, //	49
0xFD, 0xFB, 0xF6, 0xF5, 0xFC, 0xF7, 0xF6, //	50
0xE7, 0xE9, 0xE4, 0xDB, 0xFA, 0xF9, 0xF8, //	51
0xEF, 0xED, 0xE4, 0xE3, 0xF2, 0xE9, 0xE6, //	52
0xF1, 0xE3, 0xE8, 0xEB, 0xE6, 0xE1, 0xEA, //	53
0xE5, 0xEB, 0xE2, 0xDF, 0xF2, 0xE7, 0xE2, //	54
0xED, 0xE1, 0xE6, 0xE7, 0xF0, 0xE5, 0xDE  //	55
};

// ref: M2CMP01-441 BiCS FLASH Gen3 512Gb eTLC Read Retry with Set Feature Rev0.1_for CM.pdf
_FTL_BURNER_DCODE UBYTE gubRetryRTable_BISC3_TOSHIBA_512Gb_Enterprise[400] =
{
0x00,
0xF8 ,0x08 ,0xF0 ,0xE8 ,0x18 ,0xD8 ,0x28,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // table 0

//M2CMP01-441 BiCS FLASH Gen3 512Gb eTLC Read Retry with Set Feature Rev0.1_for CM.pdf
0xFC	,	0x02	,	0x00	,	0xF6	,	0x02	,	0x00	,	0xFE	,	// 1
0xFA	,	0xFE	,	0xFC	,	0xF6	,	0x00	,	0xFE	,	0xFA	,	// 2
0xF8	,	0x00	,	0xFA	,	0xF2	,	0xFE	,	0xFC	,	0xF8	,	// 3
0xF8	,	0xFC	,	0xF8	,	0xF0	,	0xFE	,	0xFA	,	0xF6	,	// 4
0xFC	,	0xFA	,	0xFC	,	0xF6	,	0x00	,	0xFC	,	0xFA	,	// 5
0x00	,	0x00	,	0xFC	,	0xEE	,	0x00	,	0xFE	,	0xF6	,	// 6
0x00	,	0xFC	,	0xF8	,	0xEC	,	0xFE	,	0xF8	,	0xF4	,	// 7
0x02	,	0xFC	,	0xF6	,	0xEA	,	0xFE	,	0xF6	,	0xF4	,	// 8
0x00	,	0x00	,	0xFE	,	0xF6	,	0x00	,	0xFC	,	0xFC	,	// 9
0x00	,	0xFE	,	0xFA	,	0xEE	,	0xFE	,	0xFA	,	0xF6	,	// 10
0x02	,	0x00	,	0xF2	,	0xEA	,	0xFE	,	0xF8	,	0xF2	,	// 11
0x04	,	0xFA	,	0xF4	,	0xE8	,	0xFC	,	0xF6	,	0xF0	,	// 12
0x00	,	0x06	,	0x02	,	0xFE	,	0x06	,	0x03	,	0x00	,	// 13
0x04	,	0x00	,	0xFE	,	0xFE	,	0x00	,	0xFE	,	0xFE	,	// 14
0x04	,	0x04	,	0x04	,	0x04	,	0x04	,	0x02	,	0x04	,	// 15
0x02	,	0x00	,	0x02	,	0x04	,	0x00	,	0xFE	,	0x00	,	// 16
0xFA	,	0x06	,	0xFE	,	0xFC	,	0x04	,	0x00	,	0xFC	,	// 17
0xF6	,	0xFE	,	0xFC	,	0xF8	,	0xFC	,	0xFC	,	0xFC	,	// 18
0xF4	,	0xFC	,	0xFA	,	0xF4	,	0xFA	,	0xFA	,	0xF8	,	// 19
0xFC	,	0xFA	,	0xF6	,	0xF2	,	0xFA	,	0xF6	,	0xF4	,	// 20
0xFA	,	0xF8	,	0xF4	,	0xF0	,	0xFA	,	0xF4	,	0xF2	,	// 21
0xFA	,	0xF6	,	0xF2	,	0xEC	,	0xF8	,	0xF2	,	0xEE	,	// 22
0xF0	,	0xF8	,	0xF6	,	0xF8	,	0xF8	,	0xF4	,	0xF8	,	// 23
0xEC	,	0xFA	,	0xFC	,	0x00	,	0xF8	,	0xFA	,	0xFC	,	// 24
0xF0	,	0xFC	,	0xFC	,	0x02	,	0xFA	,	0xFC	,	0xFE	,	// 25
0xF6	,	0xFC	,	0xF8	,	0xF8	,	0xFC	,	0xFA	,	0xFC	,	// 26
0xFE	,	0x02	,	0x00	,	0x00	,	0x00	,	0x00	,	0x00	,	// 27
0xF8	,	0x00	,	0xFE	,	0xFC	,	0xFE	,	0xFE	,	0xFE	,	// 28
0xF0	,	0xFA	,	0xF8	,	0xFA	,	0xF6	,	0xF6	,	0xFA	,	// 29
0xFC	,	0x06	,	0x0A	,	0x0A	,	0x02	,	0x06	,	0x0C	,	// 30
0xFE	,	0x02	,	0xFC	,	0xFC	,	0xFE	,	0xFC	,	0xFA	,	// 31
0x04	,	0xFE	,	0xFC	,	0xFC	,	0xFC	,	0xFC	,	0xFA	,	// 32
0xFA	,	0xF4	,	0xF8	,	0xF4	,	0xFE	,	0xFA	,	0xF8	,	// 33
0xFC	,	0xF8	,	0xFA	,	0xF4	,	0xFE	,	0xFA	,	0xF8	,	// 34
0xF8	,	0xFC	,	0xF6	,	0xF2	,	0xFC	,	0xF8	,	0xF4	,	// 35
0xFE	,	0xF4	,	0xF4	,	0xF0	,	0xFC	,	0xF6	,	0xF2	,	// 36
0xF8	,	0xFE	,	0xFC	,	0xFA	,	0xFC	,	0xFA	,	0xFA	,	// 37
0xFC	,	0xF8	,	0xF2	,	0xEC	,	0xF8	,	0xF2	,	0xF0	,	// 38
0xEC	,	0xFA	,	0xFA	,	0xFC	,	0xFA	,	0xF8	,	0xFE	,	// 39
0xF0	,	0xFE	,	0xFE	,	0x00	,	0xFE	,	0xFC	,	0x02	,	// 40
0xE8	,	0xF6	,	0xF6	,	0xF8	,	0xF6	,	0xF4	,	0xFA	,	// 41
0xFC	,	0xFE	,	0xFE	,	0xF4	,	0x00	,	0xFA	,	0xFC	,	// 42
0x00	,	0x02	,	0x04	,	0xF8	,	0x04	,	0xFE	,	0x00	,	// 43
0xF6	,	0xFA	,	0xF4	,	0xF0	,	0xFC	,	0xF6	,	0xF8	,	// 44
0xE0	,	0xEC	,	0xF4	,	0x02	,	0xEC	,	0xEE	,	0xFC	,	// 45
0xE4	,	0xF0	,	0xF8	,	0xFE	,	0xF0	,	0xF2	,	0xFC	,	// 46
0xE8	,	0xF4	,	0xF4	,	0xFC	,	0xF4	,	0xF6	,	0xFC	,	// 47
0xEC	,	0xF8	,	0xF8	,	0x00	,	0xF8	,	0xFA	,	0x00	,	// 48
0xE0	,	0xE8	,	0xF0	,	0xFE	,	0xE8	,	0xEA	,	0xF8	,	// 49
0xE8	,	0xE4	,	0xE8	,	0xF2	,	0xE8	,	0xE4	,	0xEE	,	// 50
0xEC	,	0xE8	,	0xEC	,	0xEE	,	0xEC	,	0xE8	,	0xEE	,	// 51
0xF0	,	0xEC	,	0xEC	,	0xEC	,	0xF0	,	0xEC	,	0xEE	,	// 52
0xE4	,	0xE0	,	0xE4	,	0xEE	,	0xE4	,	0xE0	,	0xEA	,	// 53
0xF4	,	0xF0	,	0xF0	,	0xF0	,	0xF4	,	0xF0	,	0xF2	,	// 54
0xF8	,	0xF4	,	0xF0	,	0xEA	,	0xF8	,	0xF0	,	0xEE	 	// 55
};
#endif

#if TLC_BICS2
#if TSB_BICS4_SUPPORT
// ref: TSB BiCS4 256Gb cTLC RRT for Spec 636 Reduction (Non-early Final).xlsx
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_256Gb_HV[703] =
{
0x00, // table 0
// Parameter setting table for pSLC 23 Cycle, each 1 BYTE0
0xF8,0x08,0xF0,0x10,0xE0,0xD0,0x20,0xE8,0x18,
0xD8,0x28,0xC8,0x30,0xC0,0x38,0xB8,0x40,0xB0,
0x48,0x50,0xA8,0x58,0xA0,

0xFE, 0xFE, 0xF8, 0xF9, 0xFF, 0xFD, 0xFA,    // scratch table
0x00, 0xFC, 0xF8, 0xF2, 0x00, 0xF9, 0xF4,    // scratch table
0xFC, 0xFC, 0xF8, 0xF2, 0x00, 0xF9, 0xF4,    // scratch table
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // table 0


0xF8, 0xF9, 0xF9, 0xFA, 0xF8, 0xFB, 0xFB,  // 1
0x00, 0x06, 0x0A, 0x06, 0x05, 0x0A, 0x0C,  // 2
0x00, 0x03, 0x0E, 0x08, 0x00, 0x06, 0x0E,  // 3
0x00, 0x07, 0xFE, 0xFE, 0x06, 0x02, 0x03,  // 4
0x04, 0x03, 0x00, 0xF8, 0x03, 0x02, 0xFB,  // 5
0xFC, 0x00, 0xF8, 0xEF, 0x02, 0xFA, 0xF8,  // 6
0x03, 0x06, 0x03, 0xFA, 0x08, 0x05, 0x02,  // 7
0xF4, 0xFE, 0x02, 0x0A, 0x04, 0xFE, 0xFA,  // 8
0xF9, 0xF9, 0xF7, 0x02, 0xFF, 0xFE, 0x0C,  // 9
0xF9, 0xFF, 0xF1, 0x02, 0xFA, 0xF9, 0xFD,  // 10
0xFC, 0xFC, 0xFC, 0xF4, 0xFF, 0xFA, 0xF6,  // 11
0xF2, 0xF6, 0xF6, 0xF2, 0x00, 0x00, 0xFC,  // 12
0x05, 0x04, 0x01, 0xF6, 0xFC, 0xF9, 0xF1,  // 13
0x02, 0xFE, 0xFB, 0xF8, 0x00, 0xFA, 0xFD,  // 14
0xF6, 0xFD, 0xFC, 0x06, 0xF6, 0xF6, 0xFA,  // 15
0xFA, 0xF4, 0x01, 0xEC, 0x07, 0x04, 0xFA,  // 16
0xFE, 0x02, 0xF7, 0xF9, 0x01, 0xFB, 0xF9,  // 17
0xFA, 0xFF, 0xFA, 0xF3, 0xF8, 0xF3, 0xEE,  // 18
0xF5, 0xFC, 0xF3, 0xEF, 0xF0, 0xF0, 0xF0,  // 19
0xEB, 0xF6, 0xF7, 0xE8, 0xFB, 0xFB, 0xFB,  // 20
0xE9, 0xF8, 0xEE, 0xF6, 0xF6, 0xF1, 0xF1,  // 21
0xEE, 0xEE, 0xF6, 0xE8, 0xFA, 0xF5, 0xF3,  // 22
0xF2, 0xF3, 0xFA, 0xF5, 0x08, 0x06, 0x07,  // 23
0xFC, 0x0B, 0x07, 0x03, 0x0C, 0x09, 0x08,  // 24
0xF4, 0x09, 0x04, 0xFE, 0x08, 0x02, 0x02,  // 25
0xF0, 0x04, 0x02, 0x0C, 0x04, 0x07, 0x11,  // 26
0xF4, 0x05, 0x08, 0x05, 0x01, 0x04, 0x0C,  // 27
0xF8, 0xF4, 0x00, 0xFC, 0xFD, 0xFF, 0x02,  // 28
0x00, 0xFA, 0x04, 0x00, 0xF9, 0xF6, 0x00,  // 29
0xEE, 0xF8, 0xF1, 0xEE, 0xF5, 0xF4, 0xFC,  // 30
0x04, 0x02, 0xF9, 0xED, 0x05, 0x01, 0xFD,  // 31
0xF6, 0x00, 0xF8, 0xE8, 0x02, 0xFC, 0xF5,  // 32
0xFC, 0xFD, 0xEF, 0xEC, 0x01, 0xF8, 0xEE,  // 33
0x06, 0xFE, 0xF6, 0xE7, 0x00, 0xF5, 0xF1,  // 34
0xF2, 0xFC, 0xF2, 0xE1, 0x05, 0xFE, 0xF5,  // 35
0x00, 0xF4, 0xEF, 0xE2, 0xFA, 0xF3, 0xEA,  // 36
0xFD, 0xF6, 0xF3, 0xE5, 0xFC, 0xF5, 0xED,  // 37
0x03, 0xFE, 0xF2, 0xE2, 0x02, 0xFC, 0xEE,  // 38
0xF6, 0xFD, 0xEE, 0xDE, 0x01, 0xF9, 0xEB,  // 39
0xFB, 0xF7, 0xF0, 0xE1, 0xFC, 0xF5, 0xEC,  // 40
0xFF, 0xF6, 0xF5, 0xDE, 0xFB, 0xF3, 0xE8,  // 41
0xFF, 0xF4, 0xF2, 0xDA, 0xF9, 0xF0, 0xE6,  // 42
0xF9, 0xEE, 0xF3, 0xE0, 0xF2, 0xE7, 0xE7,  // 43
0xFA, 0xF4, 0xED, 0xDC, 0xFA, 0xEE, 0xE6,  // 44
0xF9, 0xFE, 0xFD, 0xF1, 0xFC, 0xF8, 0xF8,  // 45
0xF0, 0xF6, 0xF4, 0xEE, 0xFA, 0xF2, 0xF4,  // 46
0xEC, 0xF1, 0xF1, 0xE6, 0xF3, 0xED, 0xEE,  // 47
0xEB, 0xEB, 0xE9, 0xE3, 0xFA, 0xF8, 0xF6,  // 48
0xEF, 0xF0, 0xEE, 0xE8, 0x04, 0xFA, 0xF3,  // 49
0xF3, 0xF4, 0xEC, 0xE8, 0xED, 0xE6, 0xE8,  // 50
0xF7, 0xE9, 0xF4, 0xDD, 0xF0, 0xEC, 0xEE,  // 51
0xED, 0xF1, 0xEB, 0xDF, 0xF4, 0xE7, 0xEB,  // 52
0xEC, 0xF5, 0xEE, 0xE0, 0xF9, 0xF0, 0xF1,  // 53
0xEA, 0xEF, 0xE4, 0xE3, 0xEC, 0xE2, 0xE4,  // 54
0xE9, 0xED, 0xEA, 0xE4, 0xF0, 0xEE, 0xEE,  // 55
0x00, 0x03, 0x0C, 0x07, 0xFF, 0x04, 0x0C,  // 56
0xF4, 0x00, 0xF6, 0xFA, 0x00, 0xFD, 0xF6,  // 57
0xFC, 0x08, 0x02, 0x00, 0x06, 0x06, 0x03,  // 58
0xF8, 0xFA, 0xFA, 0xF4, 0xFA, 0xFA, 0xF8,  // 59
0xFC, 0xFD, 0x04, 0x01, 0xFE, 0x00, 0x04,  // 60
0xF6, 0xFE, 0xF3, 0xFB, 0xF6, 0xF4, 0xF0,  // 61
0xF3, 0xF7, 0xFA, 0xF6, 0xF4, 0xEE, 0xF2,  // 62
0xED, 0x03, 0xF7, 0xFD, 0xEE, 0xE8, 0xEC,  // 63
0xEE, 0xF6, 0xF2, 0xEB, 0x02, 0x02, 0xFE,  // 64
0x08, 0xFA, 0xF8, 0xEE, 0xFC, 0xF6, 0xF4,  // 65
0xF4, 0xF9, 0xFE, 0xFA, 0x00, 0x03, 0x05,  // 66
0xF1, 0xF4, 0x02, 0xEE, 0xFA, 0xF4, 0xEE,  // 67
0xF4, 0x01, 0xEE, 0x00, 0x02, 0xFC, 0xFC,  // 68
0xF0, 0xED, 0xF6, 0xEB, 0xF4, 0xF2, 0xEE,  // 69
0x00, 0x05, 0x06, 0xFE, 0x07, 0x02, 0x07,  // 70
0xEE, 0xF0, 0xEE, 0xED, 0xFC, 0xFC, 0xFE,  // 71
0xF1, 0xF2, 0xF3, 0xEC, 0xF2, 0xEC, 0xF0,  // 72
0xEF, 0xF9, 0xF7, 0xEE, 0xEE, 0xEA, 0xEE,  // 73
0xF8, 0x07, 0xFE, 0xFC, 0x03, 0xFE, 0xFF,  // 74
0xF0, 0xEE, 0xFC, 0xE7, 0xF8, 0xF6, 0xF0,  // 75
0xF2, 0xEC, 0x06, 0xE7, 0xF6, 0xF2, 0xEE,  // 76
0x00, 0xF6, 0xF0, 0xE9, 0xF8, 0xF4, 0xEC,  // 77
0xF2, 0xF0, 0xFF, 0xE9, 0xF8, 0xF8, 0xF8,  // 78
0xF3, 0xF5, 0x03, 0xEA, 0xF2, 0xEE, 0xF2,  // 79
0x0C, 0xF1, 0xF8, 0xE2, 0xF6, 0xF0, 0xEA,  // 80
0xF8, 0xFE, 0xEC, 0xF4, 0xFE, 0xFC, 0xF6,  // 81
0xF6, 0xF1, 0x06, 0xF1, 0xF8, 0xF4, 0xF0,  // 82
0xFC, 0xFF, 0xFE, 0xF6, 0xFF, 0xFE, 0xFB,  // 83
0xF2, 0xF2, 0xFB, 0xE7, 0xEE, 0xE8, 0xEE,  // 84
0xF3, 0xEE, 0xFD, 0xEF, 0xF8, 0xF6, 0xF2,  // 85
0xEC, 0xF6, 0xF0, 0xF3, 0xF4, 0xF4, 0xF6,  // 86
0xEC, 0xF8, 0xF8, 0xF0, 0xF6, 0xF2, 0xEC,  // 87
0xF3, 0xFB, 0xF5, 0xF6, 0xF0, 0xEC, 0xEE,  // 88
0x05, 0x04, 0x02, 0xF6, 0xFC, 0xF9, 0xF1,  // 89
0x02, 0x02, 0x03, 0xFC, 0x02, 0x00, 0xF8,  // 90
0xF8, 0xF8, 0xF5, 0xF7, 0xF8, 0xFB, 0xFB,  // 91
0xFE, 0xFE, 0xFB, 0xF2, 0xFE, 0xFC, 0xF4,  // 92
0xFA, 0x0B, 0x07, 0x03, 0x0C, 0x09, 0x08   // 93

};



// ref: M2CMP01-483 Early BiCS4 256Gb TLC Read Retry with Set Feature Rev1.0 - Retrieval Read.pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_256Gb_Early[437] =
{

0x00, // table 0
// Parameter setting table for pSLC 23 Cycle, each 1 BYTE0
0xF8,0x08,0xF0,0x10,0xE0,0xD0,0x20,0xE8,0x18,
0xD8,0x28,0xC8,0x30,0xC0,0x38,0xB8,0x40,0xB0,
0x48,0x50,0xA8,0x58,0xA0,

0xFE, 0xFE, 0xF8, 0xF9, 0xFF, 0xFD, 0xFA,    // scratch table
0x00, 0xFC, 0xF8, 0xF2, 0x00, 0xF9, 0xF4,    // scratch table
0xFC, 0xFC, 0xF8, 0xF2, 0x00, 0xF9, 0xF4,    // scratch table
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // table 0

0xF6, 0xF6, 0xF6, 0xF8, 0xF4, 0xF7, 0xF9,	// 1
0xFE, 0x03, 0x07, 0x04, 0x01, 0x06, 0x0A,	// 2
0x00, 0x03, 0x0E, 0x08, 0x00, 0x06, 0x0E,	// 3
0xFC, 0x08, 0x02, 0x00, 0x06, 0x06, 0x03,	// 4
0x00, 0x05, 0x06, 0xFE, 0x07, 0x02, 0x07,	// 5
0xF4, 0xFE, 0x02, 0x0A, 0x04, 0xFE, 0xFA,	// 6
0xFC, 0xFC, 0xFC, 0xF4, 0xFF, 0xFA, 0xF6,	// 7
0x05, 0x04, 0x01, 0xF6, 0xFC, 0xF9, 0xF1,	// 8
0x02, 0xFE, 0xFB, 0xF8, 0x00, 0xFA, 0xFD,	// 9
0xF6, 0xFD, 0xFC, 0x06, 0xF6, 0xF6, 0xFA,	// 10
0xFA, 0xF4, 0x01, 0xEC, 0x07, 0x04, 0xFA,	// 11
0xFE, 0x02, 0xF7, 0xF9, 0x01, 0xFB, 0xF9,	// 12
0xFA, 0xFF, 0xFA, 0xF3, 0xF8, 0xF3, 0xEE,	// 13
0xF5, 0xFC, 0xF3, 0xEF, 0xF0, 0xF0, 0xF0,	// 14
0xEB, 0xF6, 0xF7, 0xE8, 0xFB, 0xFB, 0xFB,	// 15
0xE9, 0xF8, 0xEE, 0xF6, 0xF6, 0xF1, 0xF1,	// 16
0xEE, 0xEE, 0xF6, 0xE8, 0xFA, 0xF5, 0xF3,	// 17
0xF2, 0xF3, 0xFA, 0xF5, 0x08, 0x06, 0x07,	// 18
0xFC, 0x0B, 0x07, 0x03, 0x0C, 0x09, 0x08,	// 19
0xF4, 0x09, 0x04, 0xFE, 0x08, 0x02, 0x02,	// 20
0xF0, 0x04, 0x02, 0x0C, 0x04, 0x07, 0x11,	// 21
0xF4, 0x05, 0x08, 0x05, 0x01, 0x04, 0x0C,	// 22
0xF8, 0xF4, 0x00, 0xFC, 0xFD, 0xFF, 0x02,	// 23
0x00, 0xFA, 0x04, 0x00, 0xF9, 0xF6, 0x00,	// 24
0xEE, 0xF8, 0xF1, 0xEE, 0xF5, 0xF4, 0xFC,	// 25
0x04, 0x02, 0xF9, 0xED, 0x05, 0x01, 0xFD,	// 26
0xF6, 0x00, 0xF8, 0xE8, 0x02, 0xFC, 0xF5,	// 27
0xFC, 0xFD, 0xEF, 0xEC, 0x01, 0xF8, 0xEE,	// 28
0x06, 0xFE, 0xF6, 0xE7, 0x00, 0xF5, 0xF1,	// 29
0xF2, 0xFC, 0xF2, 0xE1, 0x05, 0xFE, 0xF5,	// 30
0xFD, 0xF6, 0xF3, 0xE5, 0xFC, 0xF5, 0xED,	// 31
0x03, 0xFE, 0xF2, 0xE2, 0x02, 0xFC, 0xEE,	// 32
0xF6, 0xFD, 0xEE, 0xDE, 0x01, 0xF9, 0xEB,	// 33
0xFB, 0xF7, 0xF0, 0xE1, 0xFC, 0xF5, 0xEC,	// 34
0xFA, 0xF4, 0xED, 0xDC, 0xFA, 0xEE, 0xE6,	// 35
0xF9, 0xFE, 0xFD, 0xF1, 0xFC, 0xF8, 0xF8,	// 36
0xF0, 0xF6, 0xF4, 0xEE, 0xFA, 0xF2, 0xF4,	// 37
0xEC, 0xF1, 0xF1, 0xE6, 0xF3, 0xED, 0xEE,	// 38
0xEB, 0xEB, 0xE9, 0xE3, 0xFA, 0xF8, 0xF6,	// 39
0xEF, 0xF0, 0xEE, 0xE8, 0x04, 0xFA, 0xF3,	// 40
0xF3, 0xF4, 0xEC, 0xE8, 0xED, 0xE6, 0xE8,	// 41
0xF7, 0xE9, 0xF4, 0xDD, 0xF0, 0xEC, 0xEE,	// 42
0xED, 0xF1, 0xEB, 0xDF, 0xF4, 0xE7, 0xEB,	// 43
0xEC, 0xF5, 0xEE, 0xE0, 0xF9, 0xF0, 0xF1,	// 44
0xE9, 0xED, 0xEA, 0xE4, 0xF0, 0xEE, 0xEE,	// 45
0xEC, 0xF1, 0xFE, 0xF9, 0xEC, 0xF4, 0xF6,	// 46
0xEE, 0xE9, 0xED, 0xE7, 0xE8, 0xE8, 0xE8,	// 47
0xF6, 0xEB, 0xE9, 0xDF, 0xEE, 0xE8, 0xE2,	// 48
0xF4, 0xF4, 0xEE, 0xE0, 0xF4, 0xF0, 0xE6,	// 49
0xF3, 0xF2, 0x00, 0xFC, 0xF2, 0xF4, 0xFA,	// 50
0xEE, 0xF6, 0x02, 0x06, 0xF0, 0xF2, 0xFC,	// 51
0xFC, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0xFA,	// 52
0xF6, 0xF8, 0xFB, 0xFB, 0xFA, 0xFD, 0xFA,	// 53
0xF6, 0xF7, 0xFC, 0xF1, 0xF4, 0xF8, 0xF6,	// 54
0xF7, 0xFC, 0x02, 0xFC, 0xFA, 0x02, 0x04	// 55


};

// ref: M2CMP01-599 Final BiCS4 256Gb TLC Read Retry with Set Feature rev1.0.pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_256Gb_Final[262] =
{
0x00, // table 0

// Parameter setting table for pSLC 23 Cycle, each 1 BYTE0
0xF8,0x08,0xF0,0x10,0xE0,0xD0,0x20,0xE8,0x18,
0xD8,0x28,0xC8,0x30,0xC0,0x38,0xB8,0x40,0xB0,
0x48,0x50,0xA8,0x58,0xA0,

0xFE, 0xFE, 0xF8, 0xF9, 0xFF, 0xFD, 0xFA,     // scratch table
0x00, 0xFC, 0xF8, 0xF2, 0x00, 0xF9, 0xF4,     // scratch table
0xFC, 0xFC, 0xF8, 0xF2, 0x00, 0xF9, 0xF4,     // scratch table
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // table 0

0x00, 0x06, 0x0A, 0x06, 0x05, 0x0A, 0x0C,	// 1
0xFC, 0xFD, 0x04, 0x01, 0xFE, 0x00, 0x04,	// 2
0xF8, 0xFE, 0xFA, 0xFB, 0xFA, 0xFA, 0xF8,	// 3
0xF6, 0xFA, 0xF3, 0xF4, 0xF6, 0xF4, 0xF0,	// 4
0x00, 0x03, 0x0E, 0x08, 0x00, 0x06, 0x0E,	// 5
0x00, 0x07, 0xFE, 0xFE, 0x06, 0x02, 0x03,	// 6
0x04, 0x03, 0x00, 0xF8, 0x03, 0x02, 0xFB,	// 7
0xFC, 0xFC, 0xFC, 0xF4, 0xFF, 0xFE, 0x09,	// 8
0xFC, 0x00, 0xF8, 0xEF, 0x00, 0x00, 0xFC,	// 9
0xF9, 0xF9, 0xF7, 0x02, 0x02, 0xFA, 0xF8,	// 10
0xF4, 0xF6, 0xF6, 0xF2, 0xFF, 0xFA, 0xF6,	// 11
0xEE, 0xF6, 0xF2, 0xEB, 0x02, 0x02, 0xFE,	// 12
0xF4, 0xF9, 0xFE, 0xFA, 0x00, 0x06, 0x0E,	// 13
0xEE, 0xFB, 0xF5, 0xFC, 0xFC, 0xFC, 0xF6,	// 14
0xF1, 0xF3, 0xFF, 0xF4, 0xF4, 0xF4, 0xF4,	// 15
0xEE, 0xF5, 0xF9, 0xEC, 0xFA, 0xF8, 0xF2,	// 16
0xF2, 0xF7, 0xFF, 0xED, 0xFC, 0xFC, 0xFE,	// 17
0xEF, 0xF0, 0xFE, 0xED, 0xF8, 0xF8, 0xF8,	// 18
0xEE, 0xF0, 0xEE, 0xE9, 0xF6, 0xF6, 0xF6,	// 19
0xF3, 0xF9, 0x03, 0xEE, 0xF8, 0xF6, 0xF0,	// 20
0xF2, 0xF5, 0x06, 0xEA, 0xF2, 0xEE, 0xF2,	// 21
0xF0, 0xEE, 0xFC, 0xE7, 0xF6, 0xF2, 0xEE,	// 22
0xEF, 0xEC, 0xF7, 0xE7, 0xEE, 0xEA, 0xEE,	// 23
0xF1, 0xF4, 0x02, 0xEE, 0xFA, 0xF6, 0xF0,	// 24
0xF0, 0xF7, 0xF8, 0xF7, 0xF7, 0xF1, 0xF3,	// 25
0xF2, 0xF2, 0xF6, 0xE6, 0xFA, 0xF4, 0xEE,	// 26
0xF4, 0xF1, 0xF2, 0xE4, 0xF6, 0xEF, 0xEC,	// 27
0xEF, 0xEE, 0xF1, 0xE4, 0xF4, 0xEC, 0xEC,	// 28
0xF0, 0xEC, 0xF0, 0xE4, 0xF4, 0xF0, 0xEA,	// 29
0xF1, 0xF0, 0xEE, 0xE2, 0xF8, 0xEE, 0xEA	// 30

};


// ref: M2CMP01-597 BiCS FLASH Gen4 512Gb TLC Read Retry with Set Feature rev0.1 - for CM Wafer.pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_512Gb[444] =
{
0x00, // table 0
0xF8,0x08,0xF0,0x10,0xE0,0xD0,0x20,0xE8,
0x18,0xD8,0x28,0xC8,0x30,0xC0,0x38,0xB8,
0x40,0xB0,0x48,0x50,0xA8,0x58,0xA0,

0x00, 0xFC, 0xF9, 0xF2, 0xFC, 0xF9, 0xF4,	// scratch table 0
0xF4, 0x00, 0xFC, 0xF8, 0xFC, 0xFC, 0xFC,	// scratch table 1
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// table 0

0xF7, 0xF9, 0xF6, 0xF5, 0xF8, 0xF6, 0xF6,	// 1
0xFF, 0xF3, 0xFD, 0xE4, 0xFE, 0xFE, 0x02,	// 2
0x05, 0xFE, 0xF8, 0xFB, 0xF6, 0xF0, 0xEC,	// 3
0x03, 0xF2, 0xF8, 0xEE, 0xF2, 0xF2, 0xF2,	// 4
0xF3, 0xEF, 0xF4, 0xE6, 0xFC, 0xF8, 0xF0,	//5
0xF3, 0xEC, 0xED, 0xE8, 0xF0, 0xEC, 0xEE,	//6
0xFC, 0x04, 0xFB, 0xFA, 0x02, 0x02, 0x04,	//7
0xFD, 0xF7, 0xF4, 0x02, 0xFA, 0xFA, 0xFC,	//8
0xF2, 0xF5, 0xF3, 0xE6, 0xF4, 0xEE, 0xEA,	//9
0xF1, 0xEA, 0xEE, 0xE2, 0xEE, 0xE6, 0xEA,	//10
0xF3, 0xF4, 0xF2, 0xF1, 0xF4, 0xF0, 0xF2,	//11
0xFA, 0xEE, 0xFD, 0xE7, 0xF4, 0xF4, 0xF6,	//12
0xEF, 0xF1, 0xEB, 0xED, 0xF0, 0xE8, 0xEC,	//13
0xF6, 0xF5, 0xEF, 0xEC, 0xF4, 0xF0, 0xF0,	//14
0xFE, 0xF0, 0xF8, 0xE9, 0xF6, 0xF0, 0xEA,	//15
0xF2, 0xF2, 0xEC, 0xF0, 0xF2, 0xEA, 0xEE,	//16
0x04, 0xEB, 0xF9, 0xE5, 0xEC, 0xE6, 0xEA,	//17
0x01, 0xF6, 0xF7, 0xEF, 0xFE, 0x00, 0x0A,	//18
0xFE, 0xF2, 0xF6, 0xE9, 0xF8, 0xF4, 0xF2,	//19
0xFD, 0xF3, 0xF7, 0xE7, 0xF0, 0xEA, 0xEE,	//20
0xF1, 0xF2, 0xF2, 0xE5, 0xF4, 0xF2, 0xF4,	//21
0xF5, 0xF4, 0xF3, 0xEE, 0xF2, 0xE8, 0xEE,	//22
0xEF, 0xF4, 0xF0, 0xE8, 0xF2, 0xEE, 0xF0,	//23
0xF3, 0xEE, 0xF1, 0xEA, 0xFC, 0xFA, 0xF8,	//24
0xFF, 0xF6, 0xF9, 0xF3, 0xFA, 0xF2, 0xEE,	//25
0xF5, 0xEC, 0xEE, 0xEA, 0xEE, 0xE6, 0xE6,	//26
0xEF, 0xF3, 0xED, 0xF3, 0xEC, 0xE4, 0xE8,	//27
0xFC, 0xF7, 0xF6, 0xE9, 0xEE, 0xE8, 0xEC,	//28
0xFD, 0xF8, 0xF6, 0xF0, 0xF2, 0xF0, 0xF0,	//29
0x05, 0xFB, 0xFA, 0xF6, 0xF8, 0xF8, 0xFA,	//30
0xF3, 0xFA, 0xEB, 0xF2, 0xEE, 0xEA, 0xEE,	//31
0xFE, 0xFB, 0xFE, 0xFA, 0xF2, 0xF0, 0xF2,	//32
0xF7, 0xEC, 0xF2, 0xE3, 0xF6, 0xF4, 0xF4,	//33
0xEB, 0xFD, 0xEA, 0xF7, 0xF4, 0xF2, 0xF2,	//34
0xFC, 0xFC, 0xEF, 0xF3, 0xF6, 0xF2, 0xEE,	//35
0xEC, 0xE9, 0xEC, 0xDF, 0xEC, 0xE2, 0xE4,	//36
0xF8, 0xEB, 0xF4, 0xDE, 0xEA, 0xE4, 0xE6,	//37
0xFA, 0xE8, 0xF2, 0xE3, 0xEA, 0xE2, 0xE8,	//38
0x01, 0xE7, 0xFB, 0xDC, 0xE8, 0xE0, 0xE2,	//39
0x00, 0xFC, 0xF9, 0xF2, 0xFC, 0xF9, 0xF4,	//40
0x00, 0xFC, 0xF7, 0xF0, 0xFC, 0xF8, 0xF3,	//41
0xF2, 0xEE, 0xF0, 0xE4, 0xF0, 0xEE, 0xE9,	//42
0xF3, 0xEE, 0xEE, 0xED, 0xF2, 0xEC, 0xEB,	//43
0xF5, 0xF0, 0xF0, 0xE5, 0xF1, 0xF4, 0xE7,	//44
0xF9, 0xF0, 0xF1, 0xEE, 0xF6, 0xEE, 0xF1,	//45
0xFA, 0xF2, 0xF5, 0xEC, 0xF8, 0xF1, 0xEF,	//46
0xFC, 0xF5, 0xF3, 0xE9, 0xF9, 0xF0, 0xF4,	//47
0xFC, 0xF7, 0xF9, 0xEC, 0xFB, 0xF6, 0xF6,	//48
0xFA, 0xF7, 0xF0, 0xF6, 0xFF, 0xFB, 0xFC,	//49
0xF5, 0xF8, 0xF5, 0xEA, 0xFE, 0xFA, 0xFA,	//50
0xF9, 0xF9, 0xF7, 0xEE, 0xFE, 0xF8, 0xFE,	//51
0xF7, 0xFA, 0xF5, 0xF0, 0xFF, 0xFD, 0xF8,	//52
0xF9, 0xFA, 0xF9, 0xEC, 0xFA, 0xF8, 0xF9,	//53
0xFE, 0xEE, 0xFB, 0xE1, 0xFA, 0xF4, 0xF1,	//54
0x02, 0xF0, 0xF9, 0xE2, 0xFC, 0xF4, 0xFC, 	//55

0x07, 0xFA, 0xF8, 0xF0, 0xFC, 0xF8, 0xF2,    // 31  620_V0.3  20190516
0xF6, 0xF2, 0xEE, 0xE4, 0xF6, 0xF0, 0xE8     // 32
};

// ref: sBiCS4-rr-V1.0-3D-Gen4-256Gb-2P-Dynamic_Read_Sequence_with_SET_FEATURES_r1.0(CS2).pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_SANDISK_256Gb[477] =
{

0x00,	// table 0
// Parameter setting table for pSLC 21 Cycle, each 1 BYTE0
0xB0,0xB8,0xC0,0xC8,0xD0,0xD8,0xE0,0xE8,0xF0,
0xF8,0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
0x40,0x48,0x50,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // table 0

0xFC, 0xE8, 0xF0, 0xEC, 0xF4, 0xEC, 0xE4,  // 1
0xE4, 0xEC, 0xF0, 0xEC, 0xF4, 0xE8, 0xEC,  // 2
0x00, 0xF0, 0xF0, 0xEC, 0xF0, 0xF4, 0xF0,  // 3
0xF0, 0xF4, 0xF0, 0xEC, 0xF0, 0xEC, 0xE8,  // 4
0xEC, 0xF8, 0xF0, 0xEC, 0xF0, 0xE8, 0xE4,  // 5
0xF8, 0xE8, 0xF0, 0xE8, 0xEC, 0xF0, 0xE8,  // 6
0xEC, 0xEC, 0xEC, 0xE8, 0xF4, 0xF4, 0xF0,  // 7
0xF0, 0xF0, 0xEC, 0xE8, 0xF4, 0xF0, 0xFC,  // 8
0xE4, 0xF4, 0xF4, 0xE8, 0xEC, 0xE8, 0xE4,  // 9
0xE0, 0xF8, 0xF4, 0xE8, 0xEC, 0xE8, 0xE0,  //10
0xE8, 0xE8, 0xF4, 0xE4, 0xE8, 0xEC, 0xE8,  //11
0xE8, 0xEC, 0xF0, 0xE4, 0xE8, 0xE8, 0xE4,  //12
0xF4, 0xF0, 0xF0, 0xE4, 0xE4, 0xE4, 0xE0,  //13
0xF4, 0xF4, 0xEC, 0xE4, 0x04, 0xFC, 0xF4,  //14
0xF8, 0xE8, 0xEC, 0xE0, 0xF8, 0xF4, 0xF4,  //15
0xF4, 0xEC, 0xE8, 0xE0, 0xF8, 0xF4, 0xF0,  //16
0xF0, 0xF4, 0xE8, 0x00, 0xF8, 0xF4, 0xEC,  //17
0xEC, 0xF4, 0xE8, 0xFC, 0xF8, 0xF0, 0xEC,  //18
0xE8, 0xF4, 0xE8, 0xF8, 0xF4, 0xFC, 0xF4,  //19
0xEC, 0xF0, 0xE4, 0xF4, 0x04, 0xF8, 0xF0,  //20
0xE4, 0xF4, 0xE4, 0xF4, 0x00, 0xF8, 0xF4,  //21
0xF4, 0xEC, 0x00, 0xF0, 0xFC, 0xFC, 0xF8,  //22
0xF4, 0xF0, 0xFC, 0xF0, 0xFC, 0xFC, 0xF0,  //23
0xF4, 0xF4, 0xF8, 0xF0, 0xFC, 0xF4, 0xF0,  //24
0xF8, 0xF8, 0xF8, 0xF4, 0xF8, 0xFC, 0xFC,  //25
0xEC, 0xF8, 0xF8, 0xF0, 0xF8, 0xFC, 0xF8,  //26
0xF4, 0x00, 0xF4, 0xF0, 0xFC, 0xF8, 0xFC,  //27
0xF8, 0xFC, 0xF4, 0xEC, 0xF8, 0xF8, 0xF8,  //28
0x00, 0x00, 0xF4, 0xEC, 0xF8, 0xF8, 0xEC,  //29
0xFC, 0x00, 0x08, 0x10, 0x08, 0x08, 0x00,  //30
0x00, 0x04, 0x08, 0x10, 0x08, 0x04, 0x04,  //31
0x04, 0xFC, 0x08, 0x0C, 0x08, 0x04, 0x00,  //32
0x08, 0x00, 0x08, 0x0C, 0x08, 0x04, 0xFC,  //33
0xF8, 0x04, 0x04, 0x0C, 0x08, 0x00, 0x00,  //34
0x04, 0x08, 0x04, 0x0C, 0x08, 0x00, 0xFC,  //35
0xFC, 0xF8, 0x04, 0x08, 0x08, 0x00, 0xF8,  //36
0x00, 0xFC, 0x04, 0x08, 0x04, 0x08, 0x08,  //37
0x08, 0x00, 0x04, 0x08, 0x04, 0x08, 0x00,  //38
0xF4, 0x04, 0x04, 0x08, 0x04, 0x04, 0x04,  //39
0x04, 0x08, 0x00, 0x08, 0x04, 0x04, 0xFC,  //40
0x08, 0xF8, 0xF8, 0x04, 0x04, 0x00, 0x00,  //41
0x08, 0xFC, 0x00, 0x04, 0x04, 0x00, 0xFC,  //42
0xEC, 0x00, 0x00, 0x04, 0x04, 0x00, 0xF8,  //43
0xFC, 0x04, 0x00, 0x04, 0x04, 0x00, 0xF4,  //44
0xF8, 0x08, 0x00, 0x04, 0x04, 0xFC, 0xF8,  //45
0x0C, 0xF8, 0x00, 0x00, 0x00, 0x08, 0x0C,  //46
0xF0, 0xFC, 0x00, 0x00, 0x00, 0x08, 0x08,  //47
0x08, 0x04, 0xFC, 0xF0, 0x00, 0x04, 0x04,  //48
0xFC, 0x04, 0xFC, 0x00, 0x00, 0x04, 0x00,  //49
0x04, 0x08, 0xFC, 0x00, 0x00, 0x04, 0xF8,  //50
0x00, 0xF8, 0xFC, 0xFC, 0x00, 0x00, 0xFC,  //51
0xF8, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0xF4,  //52
0xF0, 0x00, 0xFC, 0xFC, 0x00, 0xFC, 0xFC,  //53
0xE8, 0x04, 0xFC, 0xFC, 0x00, 0xFC, 0xF8,  //54
0xEC, 0x08, 0xFC, 0xFC, 0x00, 0xFC, 0xF0,  //55
0xF0, 0xF8, 0xF8, 0xF8, 0x00, 0xF8, 0xF0,  //56
0x04, 0xFC, 0xF8, 0xF8, 0xFC, 0x04, 0x00,  //57
0xFC, 0x00, 0xF8, 0xF8, 0xFC, 0x00, 0xFC,  //58
0x00, 0x04, 0xF8, 0xF8, 0xFC, 0x00, 0xF8,  //59
0xE8, 0x08, 0xF8, 0xF8, 0xFC, 0xFC, 0x00,  //60
0x04, 0xFC, 0xF4, 0xF4, 0xFC, 0xF8, 0xF4,  //61
0xEC, 0x00, 0xF4, 0xF4, 0xFC, 0xF8, 0xF0,  //62
0xF0, 0x04, 0xF4, 0xF4, 0xFC, 0xF4, 0xF4,  //63
0xFC, 0xFC, 0xF4, 0xF0, 0xF8, 0xF8, 0xF4   //64
};


// ref:sBiCS4-rr-V1.0-3D-Gen4-512Gb-2P-Dynamic_Read_Sequence_with_SET_FEATURES_r1.0(CS2).pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC4_SANDISK_512Gb[248] =
{
0x00, // table 0

0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0x08,
0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40,


0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// table 0

0xE1, 0x03, 0xFA, 0x06, 0xFD, 0xF9, 0xF8,	// 1
0xF9, 0x04, 0x06, 0x0A, 0x02, 0x00, 0x00,	// 2
0xF7, 0xFB, 0xF8, 0x0B, 0xFE, 0xFD, 0x02,	// 3
0xF5, 0xF2, 0xF3, 0xFB, 0x01, 0x04, 0x09,	// 4
0xFC, 0xEC, 0x02, 0xEB, 0xF3, 0xF1, 0xF1,	// 5
0xED, 0xF0, 0x00, 0xF2, 0xF3, 0xF4, 0xF0,	// 6
0xF4, 0x03, 0x01, 0x0F, 0x01, 0x05, 0x02,	// 7
0xE6, 0xF9, 0xFE, 0x01, 0x05, 0x06, 0x07,	// 8
0xE8, 0xFA, 0xF6, 0x07, 0xF8, 0xFB, 0x00,	// 9
0xE3, 0x05, 0x04, 0x13, 0xF6, 0xF8, 0xFE,	// 10
0xF4, 0x00, 0x06, 0x13, 0x02, 0x01, 0x05,	// 11
0x09, 0xF1, 0x06, 0xEE, 0xF4, 0xF4, 0xF5,	// 12
0xE1, 0xFC, 0xF2, 0xFC, 0xF8, 0xF4, 0xF2,	// 13
0xF2, 0xF7, 0xEE, 0xFD, 0xFB, 0xF7, 0xF4,	// 14
0xFE, 0xFF, 0xFC, 0x08, 0xFE, 0x00, 0x07,	// 15
0xF9, 0x03, 0xFE, 0x02, 0x05, 0x04, 0x03,	// 16
0xEF, 0xEC, 0xF6, 0xF0, 0xFB, 0xFF, 0xFE,	// 17
0x04, 0xF6, 0x07, 0x0A, 0xF9, 0xFA, 0xF6,	// 18
0xE8, 0xFE, 0x04, 0x04, 0xF7, 0xF7, 0xF2,	// 19
0x02, 0xF5, 0x01, 0xF2, 0xF0, 0xED, 0xEA,	// 20
0xF7, 0xF7, 0xEE, 0xFA, 0xFF, 0x01, 0xFE,	// 21
0x02, 0x00, 0xF4, 0x0D, 0xF7, 0xF3, 0xEE,	// 22
0xFC, 0x07, 0xF2, 0x0D, 0xF4, 0xF1, 0xEC,	// 23
0xE0, 0xEF, 0xFF, 0xF6, 0xF1, 0xEE, 0xEE,	// 24
0xF2, 0xF9, 0xFC, 0x0E, 0xF8, 0xFA, 0xFB,	// 25
0xEF, 0xF9, 0x05, 0xF6, 0xFD, 0xFD, 0xF9,	// 26
0x03, 0xF4, 0xFB, 0xF6, 0xF8, 0xF6, 0xF7,	// 27
0xEB, 0xFF, 0xEF, 0xFF, 0xFC, 0xFB, 0xFD,	// 28
0x07, 0xFD, 0x01, 0x10, 0xFE, 0x01, 0x03,	// 29
0xFF, 0xF4, 0x06, 0x00, 0xF4, 0xF7, 0xFA,	// 30
0xFE, 0xF6, 0xF7, 0x05, 0x00, 0xFD, 0xFC,	// 31
0xEB, 0xF2, 0xFB, 0x05, 0xFA, 0xFE, 0x04	// 32

};


#endif

_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_256Gb_Normal[400] =
{
0x00,
// Parameter setting table for pSLC 7Cycle, each 1 Byte
0xF8,0x08,0xF0,0xE8,0x18,0xD8,0x28,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // table 0

// 1st
0xF7, 0xF4, 0xF7, 0xF9, 0xF3, 0xF4, 0xF9,
// 2nd
0x05, 0xFE, 0xFF, 0x00, 0xFD, 0xFE, 0x06,
// 3rd
0xFA, 0xF1, 0xF1, 0xFE, 0xF4, 0xF0, 0xFA,
// 4th
0xFE, 0xED, 0xED, 0xFA, 0xFC, 0xF4, 0xF2,
// 5th
0xFE, 0xF5, 0xE9, 0xDE, 0xFC, 0xEC, 0xEA,
// 6th
0xFE, 0xF9, 0xE1, 0xE2, 0xF4, 0xEC, 0xEE,
// 7th
0x02, 0xF1, 0xED, 0xDC, 0xF4, 0xE4, 0xDE,
// 8th
0xFA, 0xF1, 0xF5, 0x02, 0xF4, 0xF8, 0xFE,
// 9th
0x02, 0xFD, 0xF9, 0xF6, 0xFC, 0xF8, 0xFA,
// 10th
0xFA, 0xF5, 0xE5, 0xDA, 0xF0, 0xF4, 0xFE,
// 11th
0x02, 0xF9, 0xE1, 0xEA, 0xFC, 0xF0, 0xEA,
// 12th
0xF9, 0xE1, 0xE9, 0xEA, 0xE4, 0xE4, 0xE6,
// 13th
0x02, 0xF9, 0xF5, 0xF2, 0xF8, 0xF4, 0xF6,
// 14th
0xF6, 0xE5, 0xE7, 0xEA, 0xEC, 0xE4, 0xEA,
// 15th
0xF9, 0xE9, 0xE3, 0xDE, 0xE8, 0xE0, 0xEA,
// 16th
0xF6, 0xF5, 0xE3, 0xEE, 0xF4, 0xE8, 0xEA,
// 17th
0x06, 0x01, 0xFD, 0xFA, 0x00, 0xFC, 0xFE,
// 18th
0x0A, 0xFD, 0xF9, 0xFA, 0x04, 0xFC, 0xFE,
// 19th
0xF6, 0xE9, 0xD9, 0xEA, 0xED, 0xE8, 0xEE,
// 20th
0xF2, 0xE9, 0xE3, 0xE2, 0xF1, 0xE2, 0xE2,
// 21st
0xF9, 0xE5, 0xDD, 0xE6, 0xF1, 0xE8, 0xE6,
// 22nd
0x06, 0xF5, 0x05, 0xFE, 0xF0, 0xF0, 0xFE,
// 23rd
0xFE, 0xF1, 0xF1, 0xEC, 0xF8, 0xF0, 0xEE,
// 24th
0x06, 0x05, 0xF1, 0xFE, 0xE8, 0xE4, 0xEA,
// 25th
0x0A, 0xF1, 0xFD, 0xE2, 0xF8, 0xE8, 0xE2,
// 26th
0xF6, 0xE5, 0xED, 0xE2, 0xF4, 0xF0, 0xF6,
// 27th
0x0E, 0xED, 0xFD, 0xE2, 0x04, 0x04, 0x06,
// 28th
0x02, 0x01, 0xFD, 0xF6, 0xF8, 0xFC, 0xFA,
// 29th
0xFE, 0xF1, 0xF5, 0xE6, 0xF8, 0xEC, 0xE2,
// 30th
0x06, 0xED, 0xFD, 0xD2, 0x00, 0x00, 0x02,
// 31st
0x06, 0x09, 0x05, 0x02, 0x04, 0x08, 0x06,
// 32nd
0x08, 0x09, 0x09, 0x06, 0x08, 0x08, 0x06,
// 33rd
0x0A, 0x0D, 0x09, 0x06, 0x08, 0x0C, 0x0A,
// 34th
0x02, 0x05, 0x01, 0xFE, 0x00, 0x00, 0x06,
// 35th
0x0C, 0x01, 0x0D, 0x02, 0x0C, 0x0C, 0x0E,
// 36th
0xF4, 0xF2, 0xEC, 0xEE, 0xF0, 0xF0, 0xF2,
// 37th
0xF4, 0xF6, 0xEB, 0xF2, 0xF4, 0xF4, 0xF6,
// 38th
0xF4, 0xEE, 0xEA, 0xE8, 0xEE, 0xEE, 0xEE,
// 39th
0xF4, 0xF2, 0xE9, 0xEC, 0xEC, 0xEC, 0xEC,
// 40th
0xF4, 0xE8, 0xE8, 0xE2, 0xE8, 0xE4, 0xE4,
// 41st
0xF4, 0xE6, 0xE7, 0xDE, 0xE8, 0xE8, 0xE8,
// 42nd
0xF4, 0xF3, 0xE6, 0xEC, 0xF2, 0xEE, 0xEE,
// 43rd
0xF4, 0xF1, 0xE5, 0xEA, 0xEC, 0xE8, 0xEA,
// 44th
0xF4, 0xEB, 0xE4, 0xE4, 0xE8, 0xE6, 0xE8,
// 45th
0x14, 0x10, 0x06, 0x0A, 0x10, 0x0C, 0x0C,
// 46th
0x10, 0x10, 0x08, 0x08, 0x0C, 0x0C, 0x0C,
// 47th
0x12, 0x12, 0x0A, 0x08, 0x0E, 0x0E, 0x0E,
// 48th
0x10, 0x0E, 0x06, 0x06, 0x0C, 0x0E, 0x08,
// 49th
0x14, 0x12, 0x08, 0x06, 0x0E, 0x0E, 0x0A,
// 50th
0x14, 0x14, 0x06, 0x0A, 0x0A, 0x0C, 0x0C,
// 51st
0x0A, 0x0D, 0x05, 0x02, 0x0E, 0x08, 0x06,
// 52nd
0x06, 0x0D, 0x09, 0x04, 0x0E, 0x0A, 0x0A,
// 53rd
0x0A, 0x0D, 0x09, 0x06, 0x08, 0x0C, 0x0A,
// 54th
0x02, 0x05, 0x01, 0xFE, 0x00, 0x00, 0x06,
// 55th
0x0C, 0x01, 0x0D, 0x02, 0x0C, 0x0C, 0x0E
};


//M2CMP01-465 BiCS FLASH Gen3 128Gb TLC Read Retry with Set Feature Rev1.1(Relax Read).pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_128Gb[400] =
{
0x00,
// Parameter setting table for pSLC 7Cycle, each 1 Byte
0xF8,0x08,0xF0,0xE8,0x18,0xD8,0x28,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // table 0

0xF5,0xF7,0xF4,0xF6,0xF5,0xF4,0xF6, // 1
0x02,0xFF,0x03,0x06,0x01,0x01,0x08, // 2
0xF4,0xF5,0xF9,0xFE,0xF5,0xF5,0x00, // 3
0xFE,0xFA,0xF9,0xF6,0xFF,0xFD,0xFA, // 4
0xF8,0xF3,0xEB,0xE8,0xF5,0xEF,0xEA, // 5
0xFC,0xF2,0xEF,0xEC,0xF9,0xF3,0xEE, // 6
0x00,0xFB,0xF3,0xF0,0xFD,0xF7,0xF2, // 7
0xF0,0xF3,0xF7,0xFA,0xF1,0xF3,0xFC, // 8
0xFA,0xF8,0xF5,0xFA,0xFD,0xFB,0xF6, // 9
0xFA,0xF6,0xF3,0xEA,0xFB,0xF5,0xF0, //10
0xFA,0xF3,0xED,0xE4,0xF7,0xEF,0xEC, //11
0x04,0x06,0xFB,0x06,0xF8,0xF4,0xF2, //12
0xF6,0xEB,0xEB,0xEA,0xEF,0xE9,0xEE, //13
0xEC,0xED,0xEF,0xEE,0xEF,0xEB,0xF0, //14
0xFE,0xF9,0xF1,0xEA,0xFD,0xF5,0xF0, //15
0xFC,0xF9,0xF9,0xF0,0xF9,0xF9,0xF8, //16
0x07,0xFD,0x06,0xF6,0xFD,0xFB,0xFA, //17
0x02,0xFD,0xF7,0xEA,0xFF,0xFB,0xF4, //18
0x00,0xF7,0xEF,0xE4,0xFB,0xF5,0xEC, //19
0xFC,0xF6,0xED,0xF2,0xF7,0xF1,0xEC, //20
0xF0,0xE7,0xE9,0xE4,0xEB,0xE5,0xEA, //21
0xFC,0xF3,0xEB,0xE0,0xF7,0xEF,0xE8, //22
0xF4,0xF3,0xEF,0xF0,0xF3,0xF1,0xF0, //23
0x00,0x03,0xFF,0xFA,0x01,0x01,0xFE, //24
0xFC,0x06,0xFB,0x0A,0xFD,0xFD,0xFA, //25
0x02,0x04,0xFD,0x02,0x01,0xFF,0xFC, //26
0x04,0x07,0x07,0x02,0x07,0x07,0x08, //27
0x07,0x09,0x0A,0x08,0x0B,0x09,0x0C, //28
0x03,0x03,0x0A,0x06,0x03,0x05,0x04, //29
0x01,0x02,0xFC,0xFA,0x04,0x02,0xFC, //30
0xF7,0xFE,0xFA,0xFA,0xFE,0x00,0xFA, //31
0xF9,0xFA,0xFC,0x02,0xFC,0xFC,0x02, //32
0xF5,0xF8,0xFA,0xFE,0xF8,0xFA,0xFE, //33
0xEF,0xF6,0xFA,0xFE,0xF6,0xFA,0x00, //34
0xEB,0xF4,0xF8,0xFA,0xF2,0xF8,0xFC, //35
0xF9,0x00,0xFA,0xF6,0x00,0x02,0xFA, //36
0xF7,0xFE,0xF8,0xF4,0xFE,0x00,0xF6, //37
0xFD,0xFC,0xF8,0xF2,0x00,0xFC,0xF6, //38
0x03,0xFA,0xF2,0xEC,0xFC,0xF8,0xF0, //39
0xFB,0xF6,0xF6,0xEE,0xFE,0xFA,0xF2, //40
0xF7,0xF8,0xF4,0xEC,0xFC,0xFA,0xF2, //41
0xF7,0xF6,0xF0,0xE6,0xFA,0xF4,0xEC, //42
0xF9,0x00,0xF2,0x02,0xFC,0xF6,0xEE, //43
0xF5,0xF4,0xEC,0xE2,0xF8,0xF2,0xE8, //44
0xF3,0xF2,0xE8,0xDE,0xF6,0xF0,0xE4, //45
0xF5,0xFA,0xF8,0xFA,0xF8,0xF6,0xFC, //46
0x03,0x01,0xFB,0xF8,0x00,0x00,0xFC, //47
0x00,0xEE,0xFD,0xE6,0x02,0x02,0xFC, //48
0xFB,0x00,0xFC,0xFE,0x00,0x00,0xFF, //49
0xF3,0xF8,0xF8,0xF4,0xFA,0xFA,0xF8, //50
0xF1,0xF2,0xF6,0xF6,0xF4,0xF4,0xF8, //51
0xF3,0xF0,0xF2,0xF0,0xF0,0xF0,0xF2, //52
0xFB,0xF4,0xF2,0xF4,0xF6,0xF6,0xF2, //53
0xEF,0xF0,0xF0,0xEA,0xF2,0xEE,0xF2, //54
0xEB,0xEC,0xEE,0xEA,0xEE,0xEC,0xEE  //55
};


// ref: M2CMP01-280 [BF2 TLC 256Gb]Read Retry with Set Feature_0.2 for SSD.pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC2[225] =
{
0x00, // table 0
0x10,0x20,0xF0,0xE0,0x30,0xD0,0xC0,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // table 0
// 1st
0xF7, 0xFA, 0x04, 0x0A, 0xF9, 0xFD, 0x08,
// 2nd
0xFD, 0xFD, 0x0C, 0x12, 0xFD, 0x03, 0x10,
// 3rd
0xFD, 0x00, 0x06, 0x06, 0xFF, 0x01, 0x08,
// 4th
0xF9, 0xFA, 0x06, 0x04, 0xFE, 0x00, 0x06,
// 5th
0xFD, 0x01, 0x09, 0x0D, 0x02, 0x03, 0x0D,
// 6th
0x06, 0x03, 0x08, 0x08, 0x04, 0x06, 0x08,
// 7th
0x06, 0x06, 0x02, 0xFC, 0x06, 0x04, 0x00,
// 8th
0x02, 0x00, 0x02, 0xFA, 0x05, 0x03, 0xFE,
// 9th
0xFB, 0xF8, 0x01, 0x00, 0xF7, 0xFB, 0x03,
// 10th
0xF7, 0xFD, 0xFD, 0xFD, 0xFD, 0xFB, 0xFE,
// 11th
0xF7, 0xF9, 0xF9, 0xF9, 0xF9, 0xF7, 0xFA,
// 12th
0xF7, 0xF5, 0xF5, 0xF5, 0xF5, 0xF3, 0xF6,
// 13th
0xFB, 0xF3, 0xF2, 0xF1, 0xF3, 0xF1, 0xF3,
// 14th
0xFB, 0xF1, 0xEF, 0xED, 0xF1, 0xEF, 0xF0,
// 15th
0xFB, 0xEF, 0xEC, 0xE9, 0xEF, 0xED, 0xED,
// 16th
0x01, 0x03, 0x04, 0xF7, 0x04, 0x03, 0xFB,
// 17th
0x01, 0x01, 0x00, 0xF3, 0x02, 0x01, 0xF7,
// 18th
0x04, 0xFF, 0xFD, 0xEF, 0x00, 0xFE, 0xF4,
// 19th
0x04, 0xFB, 0xF9, 0xEB, 0xFE, 0xFA, 0xF0,
// 20th
0x08, 0xF7, 0xF5, 0xE7, 0xFA, 0xF6, 0xEC,
// 21st
0x08, 0xF3, 0xF1, 0xE3, 0xF8, 0xF2, 0xE8,
// 22nd
0x08, 0xEF, 0xED, 0xDF, 0xF6, 0xEE, 0xE4,
// 23th
0x08, 0xEE, 0xE9, 0xDB, 0xF4, 0xEA, 0xE0,
// 24th
0x07, 0x05, 0xFE, 0xED, 0x07, 0x03, 0xF3,
// 25th
0x0A, 0x03, 0xFB, 0xE9, 0x05, 0x00, 0xF0,
// 26th
0x0A, 0xFF, 0xF7, 0xE5, 0x03, 0xFC, 0xEC,
// 27th
0x0E, 0xFB, 0xF3, 0xE1, 0xFF, 0xF8, 0xE8,
// 28th
0x0E, 0xF7, 0xEF, 0xDD, 0xFD, 0xF4, 0xE4,
// 29th
0x0E, 0xF3, 0xEB, 0xD9, 0xFB, 0xF0, 0xE0,
// 30th
0x0E, 0xEF, 0xE7, 0xD5, 0xF9, 0xEC, 0xDC
};

// ref: M2CMP01-293 [BiCS FLASH Gen2 eTLC 256Gb]Read Retry with Set Feature 20161122.pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC2_Enterprise[155] =
{
0x00, // table 0
0x10,0x20,0xF0,0xE0,0x30,0xD0,0xC0,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // table 0
// 1st
0x06, 0x05, 0x03, 0xFA, 0x05, 0x03, 0x00,
// 2nd
0x07, 0x02, 0x04, 0xF6, 0x04, 0x00, 0xFC,
// 3rd
0x06, 0x07, 0x05, 0xFE, 0x08, 0x04, 0xFC,
// 4th
0xFE, 0xFE, 0xFD, 0xFA, 0x06, 0x06, 0x03,
// 5th
0x08, 0x05, 0x07, 0xFF, 0x0A, 0x07, 0x01,
// 6th
0x09, 0x06, 0x07, 0xFB, 0x05, 0x05, 0x02,
// 7th
0x05, 0x05, 0x02, 0xFD, 0x04, 0x06, 0x00,
// 8th
0x07, 0x08, 0x05, 0xFF, 0x0C, 0x09, 0x07,
// 9th
0x08, 0x09, 0x08, 0xFF, 0x0A, 0x0A, 0x04,
// 10th
0x09, 0x0A, 0x06, 0xFE, 0x0B, 0x09, 0x05,
// 11th
0x05, 0x0D, 0xFF, 0x02, 0x0D, 0x0B, 0x07,
// 12th
0x04, 0x0A, 0xFF, 0xFF, 0x01, 0x06, 0x00,
// 13th
0x04, 0x0B, 0x01, 0x00, 0x05, 0x02, 0xF9,
// 14th
0x05, 0x05, 0x00, 0xF3, 0x04, 0x00, 0xF8,
// 15th
0x04, 0x04, 0x00, 0xF4, 0x03, 0x03, 0xFA,
// 16th
0x05, 0x06, 0x01, 0xF6, 0x08, 0x04, 0xFB,
// 17th
0x04, 0x05, 0xFE, 0xF5, 0x07, 0x03, 0xFA,
// 18th
0x06, 0x01, 0x01, 0xF5, 0xFD, 0xFD, 0xFA,
// 19th
0xFE, 0x03, 0xFF, 0xF2, 0x04, 0x01, 0xF9,
// 20th
0x05, 0x02, 0xFD, 0xF2, 0x05, 0x03, 0xFD
};

// ref: sBiCS3-rr-v1.1-appnote005 - BiCS3 256Gb X3_Dynamic_Read_Sequence_with_SET_FEATURES_ver1.4(SDWFR-256GB3ED3D-A) + 6 industrial
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_SANDISK[340] =
{
0x00 ,0xB0, 0xC0, 0xD0, 0xE0, 0xF0, 0x10, 0x20, 0x30, 0x40, 0x50,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// 0
0x06, 0xF0, 0xFC, 0xF9, 0xFC, 0xF9, 0xF4, 	// 1
0xFA, 0xFF, 0xF2, 0x0F, 0xF5, 0xEF, 0xE9, 	// 2
0xF5, 0xF2, 0xEF, 0xF0, 0xF9, 0xFC, 0xFB, 	// 3
0x05, 0xF4, 0x01, 0x01, 0x04, 0x02, 0x03, 	// 4
0xF0, 0x04, 0x03, 0x03, 0x0B, 0x0B, 0x0C, 	// 5
0xF5, 0xF7, 0xEA, 0xF0, 0xFA, 0xFE, 0x04, 	// 6
0x01, 0x01, 0xFE, 0xFF, 0xFF, 0xF6, 0xF1, 	// 7
0x09, 0x06, 0x03, 0x08, 0xFC, 0x00, 0x00, 	// 8
0xF0, 0xFC, 0xEC, 0x0D, 0xFD, 0xFD, 0xF8, 	// 9
0x08, 0xFC, 0xF5, 0xFF, 0x01, 0xFC, 0xFA, 	// 10
0xFD, 0xF4, 0xFA, 0xFC, 0xFC, 0xFB, 0xFE, 	// 11
0xF0, 0xEE, 0xFE, 0xF2, 0xFE, 0x00, 0xFC, 	// 12
0x02, 0xF8, 0xF9, 0xF9, 0x08, 0x05, 0x01, 	// 13
0x01, 0xEB, 0xEE, 0xEF, 0x02, 0x04, 0xFF, 	// 14
0xFD, 0xFA, 0x0A, 0x03, 0x01, 0x04, 0x08, 	// 15
0xFD, 0xFF, 0x06, 0x03, 0xF5, 0xF8, 0xFA, 	// 16
0xFD, 0xF7, 0x01, 0x05, 0x06, 0x01, 0xFE, 	// 17
0x0B, 0xFB, 0xFA, 0xF9, 0xF7, 0xFB, 0xFF, 	// 18
0xFB, 0xFE, 0xED, 0xFB, 0xF4, 0xF4, 0xF2, 	// 19
0xF5, 0xF9, 0xF4, 0xFC, 0xFC, 0xF3, 0xEE, 	// 20
0xF5, 0x04, 0x02, 0x11, 0xF8, 0xF4, 0xF5, 	// 21
0x0C, 0xF4, 0xFF, 0xF7, 0xF7, 0xF2, 0xEE, 	// 22
0xF0, 0xF7, 0xF1, 0xFF, 0xFF, 0x04, 0x03, 	// 23
0x06, 0xF6, 0x07, 0xF4, 0xEA, 0xEC, 0xEB, 	// 24
0xF5, 0xFD, 0xFC, 0xF6, 0xF2, 0xF1, 0xEF, 	// 25
0x0D, 0x03, 0x06, 0x0C, 0xED, 0xEF, 0xEE, 	// 26
0x03, 0xEC, 0xF2, 0xF7, 0xF7, 0xF9, 0xF5, 	// 27
0xF9, 0xFD, 0xFE, 0x06, 0xF9, 0xF6, 0xF1, 	// 28
0xFE, 0xF1, 0xF5, 0xF4, 0xFD, 0x01, 0x08, 	// 29
0x15, 0xEA, 0x00, 0xF3, 0xFF, 0xFF, 0x04, 	// 30
0xF8, 0x02, 0xF8, 0x07, 0x02, 0x01, 0xFB, 	// 31
0x02, 0xFE, 0x0B, 0x0A, 0xF0, 0xF2, 0xF3, 	// 32
0xF8, 0x0A, 0x09, 0x12, 0x00, 0xFA, 0xF6, 	// 33
0x11, 0xF9, 0x02, 0x09, 0xFC, 0xF8, 0xF9, 	// 34
0xF0, 0xF0, 0xF8, 0xFC, 0x05, 0x08, 0x0A, 	// 35
0x02, 0xFA, 0x04, 0xF4, 0xF3, 0xF6, 0xF6, 	// 36
0x11, 0xE7, 0xFB, 0xEE, 0x04, 0x06, 0x05, 	// 37
0xF9, 0x08, 0x04, 0x0D, 0xFA, 0xF1, 0xEA, 	// 38
0xF2, 0xF3, 0x08, 0xEC, 0x01, 0xFE, 0xFF, 	// 39
0x09, 0xF0, 0x0C, 0xEA, 0x09, 0x08, 0x06, 	// 40

0x14, 0x0C, 0x0C, 0x0C, 0x10, 0x08, 0x08, 	// 1
0x14, 0x0C, 0x0C, 0x0C, 0x0C, 0x08, 0x08, 	// 2
0x14, 0x0C, 0x0C, 0x0C, 0x10, 0x10, 0x08, 	// 3
0x14, 0x0C, 0x0C, 0x0C, 0x0C, 0x10, 0x08, 	// 4
0x14, 0x0C, 0x0C, 0x0C, 0x10, 0x0C, 0x08, 	// 5
0x14, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x08 	// 6
};

//sBiCS3-rr-v1.0-appnote008 - BiCS3 128Gb X3_Dynamic_Read_Sequence_with_SET_FEATURES_ver1.1.pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_SANDISK_128Gb[273] =
{
0x00 ,0xF8 ,0x08 ,0xF0 ,0x10 ,0xE8 ,0x18,
0xE0 ,0x20 ,0xD8 ,0x28 ,0xD0 ,0x30 ,0xC8,
0x38 ,0xC0 ,0x40 ,0xB8 ,0x48 ,0xB0 ,0x50,
0xA8, 0x58 ,0xA0 ,0x98 ,0x90 ,0x88 ,0x80,
// 0
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x04,	0x00,	0x04,	0xFC,	0x00,	0x00,	0xFC,// 1
0x00,	0xFC,	0x04,	0xFC,	0x00,	0xFC,	0xFC,// 2
0x08,	0x04,	0x00,	0xF8,	0xFC,	0xFC,	0xFC,// 3
0x04,	0x00,	0x00,	0xF8,	0x00,	0x00,	0xF8,// 4
0x08,	0xFC,	0xFC,	0xF8,	0x00,	0xFC,	0xF8,// 5
0x04,	0x00,	0xFC,	0xF4,	0xFC,	0xFC,	0xF8,// 6
0x00,	0xFC,	0xFC,	0xF4,	0xFC,	0xF8,	0xF8,// 7
0xFC,	0xF8,	0xFC,	0xF4,	0x00,	0xFC,	0xF4,// 8
0x08,	0xFC,	0xF8,	0xF0,	0xFC,	0xFC,	0xF4,// 9
0x04,	0xF8,	0xF8,	0xF0,	0x00,	0xF8,	0xF4,// 10
0x00,	0xF4,	0xF8,	0xF0,	0xFC,	0xF8,	0xF4,// 11
0xFC,	0xFC,	0xF8,	0xEC,	0x00,	0xFC,	0xF0,// 12
0x04,	0xF8,	0xF4,	0xEC,	0xFC,	0xF8,	0xF0,// 13
0x00,	0xF4,	0xF4,	0xEC,	0xF8,	0xF8,	0xF0,// 14
0xFC,	0xFC,	0xF4,	0xE8,	0xF8,	0xF4,	0xF0,// 15
0xF8,	0xF8,	0xF4,	0xE8,	0xFC,	0xF8,	0xEC,// 16
0x00,	0xF4,	0xF0,	0xE8,	0xF8,	0xF4,	0xEC,// 17
0xFC,	0xF8,	0xF0,	0xE4,	0xF4,	0xF4,	0xEC,// 18
0xF8,	0xF4,	0xF0,	0xE4,	0xF8,	0xF0,	0xEC,// 19
0xF4,	0xF0,	0xF0,	0xE4,	0xF4,	0xF0,	0xEC,// 20
0x00,	0xF8,	0xEC,	0xE0,	0xFC,	0xF8,	0xE8,// 21
0xFC,	0xF4,	0xEC,	0xE0,	0xF8,	0xF4,	0xE8,// 22
0xF8,	0xF0,	0xEC,	0xE0,	0xF4,	0xF4,	0xE8,// 23
0xF4,	0xF4,	0xEC,	0xDC,	0xF8,	0xF0,	0xE8,// 24
0xFC,	0x04,	0x04,	0x04,	0x04,	0x08,	0x08,// 25
0xF8,	0x00,	0x04,	0x04,	0x04,	0x04,	0x04,// 26
0xF4,	0xF8,	0x04,	0x04,	0x04,	0x00,	0x00,// 27
0xF0,	0x04,	0x04,	0x08,	0x04,	0xFC,	0xFC,// 28
0xFC,	0x00,	0x00,	0x08,	0x04,	0xF8,	0xF8,// 29
0xF8,	0xF8,	0x00,	0x08,	0x08,	0x08,	0x08,// 30
0xF4,	0x04,	0x00,	0x0C,	0x08,	0x04,	0x04,// 31
0xF0,	0x00,	0x00,	0x0C,	0x08,	0x00,	0x00,// 32
0xF4,	0xF8,	0xF4,	0x0C,	0x08,	0xFC,	0xFC,// 33
0xF0,	0xF4,	0xF4,	0x0C,	0x08,	0xF8,	0xF8// 34
};

//sBiCS3-rr-v1.1-appnote010 - BiCS3 512Gb X3_Dynamic_Read_Sequence_with_SET_FEATURES_ver2.0.pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_SANDISK_512Gb[322] =
{
0x00 ,0x08 ,0xF8 ,0x10 ,0xF0 ,0x18 ,0xE8,
0x20 ,0xE0 ,0x28 ,0xD8 ,0x30 ,0xD0 ,0x38,
0xC8 ,0x40 ,0xC0 ,0x48 ,0xB8 ,0x50 ,0xB0,
//
0x00	,	0x00	,	0x00	,	0x00	,	0x00	,	0x00	,	0x00	,	// 0
0xD2	,	0xC8	,	0xF5	,	0xC8	,	0xF9	,	0xF9	,	0xF2	,	// 1
0x0C	,	0x01	,	0xFB	,	0xEB	,	0x00	,	0xFA	,	0xFE	,	// 2
0x02	,	0xFA	,	0xFF	,	0x01	,	0xFC	,	0xF5	,	0xF0	,	// 3
0xF3	,	0xF7	,	0xF1	,	0xF2	,	0x0A	,	0x08	,	0xFC	,	// 4
0x02	,	0xC8	,	0xF5	,	0x1A	,	0xF5	,	0xF3	,	0xF2	,	// 5
0x05	,	0xFA	,	0x04	,	0xEF	,	0x07	,	0x00	,	0xF5	,	// 6
0xF0	,	0x03	,	0xF8	,	0xEF	,	0xC8	,	0xC8	,	0x22	,	// 7
0x1C	,	0x02	,	0xF9	,	0xF7	,	0x04	,	0xF7	,	0xFA	,	// 8
0xF6	,	0xF4	,	0xF8	,	0xF6	,	0xF8	,	0xF8	,	0xF8	,	// 9
0xFF	,	0xF9	,	0xFA	,	0xFE	,	0xC8	,	0xC8	,	0xC8	,	// 10
0x08	,	0x05	,	0x00	,	0xF2	,	0x02	,	0xF6	,	0xEE	,	// 11
0xEB	,	0xFD	,	0xFD	,	0xED	,	0x06	,	0x02	,	0xF1	,	// 12
0xE0	,	0x07	,	0xF5	,	0xF7	,	0x02	,	0xFE	,	0xFF	,	// 13
0x16	,	0xFE	,	0x2E	,	0xFB	,	0xFE	,	0xF6	,	0xF5	,	// 14
0x10	,	0xFE	,	0xFE	,	0xF3	,	0x08	,	0x05	,	0xF7	,	// 15
0xF4	,	0xFA	,	0xFF	,	0xF6	,	0xFE	,	0xF6	,	0xFA	,	// 16
0x03	,	0x03	,	0xFA	,	0x01	,	0x06	,	0x06	,	0xFC	,	// 17
0x15	,	0xF8	,	0xFE	,	0xED	,	0xFC	,	0xFF	,	0xFF	,	// 18
0xFA	,	0x03	,	0xFA	,	0xFA	,	0x03	,	0xF6	,	0xF4	,	// 19
0x10	,	0xF3	,	0xF8	,	0xED	,	0x05	,	0x00	,	0xFA	,	// 20
0x0D	,	0xFD	,	0x04	,	0x06	,	0x00	,	0xF9	,	0xF2	,	// 21
0x0C	,	0x09	,	0x30	,	0xF2	,	0x09	,	0x00	,	0xFA	,	// 22
0x06	,	0xF7	,	0xF1	,	0xF9	,	0x00	,	0x05	,	0xFD	,	// 23
0xFE	,	0x08	,	0x03	,	0x06	,	0xF9	,	0xFA	,	0xFE	,	// 24
0x16	,	0xF3	,	0xF8	,	0xF2	,	0x04	,	0x02	,	0xFF	,	// 25
0x1A	,	0xFF	,	0xFF	,	0xEF	,	0x08	,	0xFF	,	0xFF	,	// 26
0xFB	,	0xFF	,	0xFF	,	0xF7	,	0xFF	,	0xFF	,	0xF7	,	// 27
0xE3	,	0x07	,	0xFB	,	0xFE	,	0x02	,	0x04	,	0xF6	,	// 28
0x14	,	0x02	,	0x03	,	0x05	,	0x03	,	0xFE	,	0xF2	,	// 29
0xFA	,	0xFB	,	0xF2	,	0xF2	,	0x00	,	0xFB	,	0xEE	,	// 30
0xF8	,	0x08	,	0x03	,	0x01	,	0x01	,	0x01	,	0xFA	,	// 31
0x07	,	0xFB	,	0xFA	,	0xF9	,	0x06	,	0x07	,	0x02	,	// 32
0xC8	,	0x01	,	0xC8	,	0xF3	,	0x09	,	0x03	,	0xFF	,	// 33
0x08	,	0x05	,	0xF5	,	0xF6	,	0x06	,	0xFB	,	0xF6	,	// 34
0x00	,	0xFC	,	0xF1	,	0xE9	,	0x06	,	0xFA	,	0xF0	,	// 35
0xEA	,	0x08	,	0xF5	,	0xFB	,	0x0B	,	0x08	,	0x02	,	// 36
0x11	,	0x00	,	0xF3	,	0xFD	,	0x06	,	0xFA	,	0xFC	,	// 37
0x0C	,	0x05	,	0xF2	,	0xF4	,	0x00	,	0xFB	,	0xF8	,	// 38
0xFC	,	0x0A	,	0xF6	,	0xF7	,	0xFE	,	0xFF	,	0xF2	,	// 39
0x1F	,	0xFF	,	0xFF	,	0x01	,	0xFA	,	0xFE	,	0xF9	,	// 40

0xFC	,	0xF8	,	0xEB	,	0xE4	,	0xFC	,	0xEC	,	0xE3	,	// add1   2019/01/23
0xFC	,	0xFA	,	0xF0	,	0xE8	,	0xFC	,	0xF4	,	0xE8		// add2

};




// ref: M2CMP01-457 BiCS FLASH Gen3 512Gb TLC Read Retry with Set Feature Rev1.0 - Relax Read.pdf
_FTL_DECL_RTB_DATA UBYTE gubRetryRTable_BISC3_TOSHIBA_512Gb_Normal[400] =
{
0x00,
0xF8 ,0x08 ,0xF0 ,0xE8 ,0x18 ,0xD8 ,0x28,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // table 0
// 1st
0xEC, 0xF8, 0xF7, 0xF6, 0xF8, 0xF4, 0xF6,
// 2nd
0x06, 0x06, 0x0A, 0x08, 0x04, 0x04, 0x0A,
// 3rd
0x00, 0xFF, 0x07, 0xF4, 0x00, 0xFC, 0xF9,
// 4th
0x08, 0xFA, 0x04, 0xE8, 0x02, 0x00, 0xFE,
// 5th
0x04, 0x04, 0x04, 0x00, 0x06, 0x04, 0x04,
// 6th
0x00, 0x02, 0x02, 0xF8, 0x04, 0x00, 0xFE,
// 7th
0x00, 0xFC, 0xF6, 0xF4, 0x00, 0xF6, 0xF8,
// 8th
0xFC, 0xF6, 0xF0, 0xE4, 0xFC, 0xF0, 0xE8,
// 9th
0x04, 0x02, 0x00, 0xF4, 0x02, 0x00, 0xFA,
// 10th
0x04, 0x02, 0xFC, 0xE8, 0x06, 0x00, 0xF4,
// 11th
0xF0, 0xFF, 0x00, 0xF8, 0xFC, 0xF4, 0xE8,
// 12th
0x04, 0x08, 0x0C, 0xFC, 0x06, 0x04, 0x02,
// 13th
0x00, 0xFB, 0xF2, 0xEE, 0x00, 0xF2, 0xF2,
// 14th
0xFC, 0xF5, 0xEB, 0xDE, 0xFC, 0xEC, 0xE3,
// 15th
0x00, 0x01, 0xFD, 0xEE, 0x02, 0xFC, 0xF4,
// 16th
0x04, 0x01, 0xF8, 0xE2, 0x06, 0xFC, 0xEF,
// 17th
0xFC, 0xFC, 0xFC, 0xF8, 0xFE, 0xFA, 0xFC,
// 18th
0xF8, 0xF8, 0xFA, 0xF0, 0xFC, 0xF8, 0xF6,
// 19th
0xF8, 0xF4, 0xEE, 0xEC, 0xF8, 0xEE, 0xF0,
// 20th
0xF4, 0xEE, 0xE6, 0xDC, 0xF4, 0xE6, 0xE0,
// 21th
0xFC, 0xFA, 0xF4, 0xE0, 0xFE, 0xF6, 0xEC,
// 22th
0x0C, 0x0C, 0x0C, 0x08, 0x0E, 0x0C, 0x0C,
// 23th
0x08, 0x0A, 0x08, 0x02, 0x0C, 0x08, 0x06,
// 24th
0x08, 0x04, 0xFE, 0xFC, 0x08, 0xFE, 0x00,
// 25th
0x04, 0xFE, 0xF4, 0xEC, 0x04, 0xF6, 0xF0,
// 26th
0x0C, 0xFC, 0x06, 0xFC, 0x08, 0x08, 0x10,
// 27th
0xEC, 0xEC, 0xDE, 0xD4, 0xF0, 0xE0, 0xD8,
// 28th
0xF8, 0xF2, 0xE8, 0xD8, 0xF6, 0xEE, 0xE4,
// 29th
0xFC, 0x00, 0x04, 0xDC, 0x0C, 0xFE, 0xF8,
// 30th
0x00, 0x0E, 0x0C, 0xF8, 0x16, 0x0E, 0x04,
// 31th
0xF8, 0xFC, 0xE0, 0xEA, 0x00, 0xF6, 0xF0,
// 32th
0x00, 0xF0, 0xEC, 0xF0, 0xFE, 0xEC, 0xE6,
// 33th
0x00, 0xF4, 0xE8, 0xE0, 0xF6, 0xEA, 0xE6,
// 34th
0xFC, 0xEC, 0xE4, 0xE2, 0xF6, 0xE4, 0xD6,
// 35th
0xF2, 0xFC, 0xF0, 0xE8, 0xFC, 0xF3, 0xEC,
// 36th
0xF4, 0xF8, 0xF0, 0xE4, 0x00, 0xF7, 0XF0,
// 37th
0xF2, 0xFC, 0xEC, 0xE4, 0x00, 0xF7, 0xEC,
// 38th
0xFF, 0x08, 0x04, 0xF1, 0x06, 0x07, 0xF5,
// 39th
0xF6, 0xF4, 0xF4, 0xE0, 0xF8, 0xEF, 0xE8,

// M2CMP01-457 BiCS FLASH Gen3 512Gb TLC Read Retry with Set Feature Rev1.0 - Relax Read.pdf
0xF8	,	0x00	,	0x04	,	0x06	,	0x00	,	0xFC	,	0xFC	,	// 40
0xFC	,	0x04	,	0x08	,	0x0A	,	0x04	,	0x00	,	0x00	,	// 41
0xF4	,	0xFC	,	0x00	,	0x02	,	0xFC	,	0xF8	,	0xF8	,	// 42
0x00	,	0x00	,	0xFE	,	0xFC	,	0x00	,	0xFC	,	0xFE	,	// 43
0x04	,	0x04	,	0x02	,	0x00	,	0xFE	,	0xFE	,	0xFE	,	// 44
0xFC	,	0xFC	,	0xFA	,	0xF8	,	0x02	,	0xFE	,	0x00	,	// 45
0xF4	,	0xFC	,	0xF8	,	0x08	,	0x02	,	0x02	,	0xFE	,	// 46
0xEC	,	0xF8	,	0x00	,	0x00	,	0xFE	,	0xFE	,	0xFC	,	// 47
0xF0	,	0xFE	,	0x08	,	0x04	,	0x00	,	0x04	,	0x04	,	// 48
0xF0	,	0x00	,	0xF8	,	0x0A	,	0xFE	,	0x00	,	0xFE	,	// 49
0xFA	,	0xF8	,	0xF8	,	0x08	,	0xFE	,	0xFE	,	0x02	,	// 50
0xFA	,	0xFA	,	0x00	,	0x04	,	0xFC	,	0x00	,	0x04	,	// 51
0xF6	,	0x02	,	0x08	,	0x02	,	0xFE	,	0x00	,	0x02	,	// 52
0xF2	,	0xFD	,	0x04	,	0x00	,	0xFE	,	0x04	,	0x04	,	// 53
0xF2	,	0xFE	,	0xFC	,	0x0A	,	0xFC	,	0xFE	,	0xFE	,	// 54
0xEC	,	0xFC	,	0x08	,	0x0C	,	0xFC	,	0x00	,	0x00	,	// 55

};

#endif


#if( TLC && (!TLC_BICS2))
_FTL_BURNER_DCODE UBYTE gubRetryRTable_15nmTLC[308] =
{
// RETRY_D1_BLOCK_TYPE
// 15nm A2, ref: t15-rr-v0.6-M2CMP01-179_15nm TLC (pSLC mode) Retry Read Sequence Rev0.6.pdf
// ref: 1Z TLC Retry Table Reduction with High Performance(元政).xlsx
0xF0,0x00, // SLC 1st
0xE0,0x00, // SLC 2nd
0xD0,0x00, // SLC 3rd
0xC0,0x00, // SLC 4th
0x20,0x00, // SLC 5th
0x30,0x00, // SLC 6th
0x40,0x00, // SLC 7th
0x08,0x00, // SLC 8th
0xF8,0x00, // SLC 9th
0x10,0x00, // SLC 10th
0x18,0x00, // SLC 11th
0xE8,0x00, // SLC 12th
0x28,0x00, // SLC 13th
0xD8,0x00, // SLC 14th
0x38,0x00, // SLC 15th
0xC8,0x00, // SLC 16th
0x48,0x00, // SLC 17th
0xB8,0x00,// SLC 18th
0x50,0x00,// SLC 19th
0xB0,0x00,// SLC 20th
0x58,0x00,// SLC 21th
0x60,0x00,// SLC 22th
0x68,0x00,// SLC 23th
0xBC,0x00,// SLC 24th
0x70,0x00,// SLC 25th
0xA0,0x00,// SLC 26th
0x90,0x00,// SLC 27th
0x80,0x00,// SLC 28th
//161220 nick mail
        // 1st
        0xF4,        0xFF,        0xFF,        0x01,       0x02,       0x06,       0x09,
        // 2st
        0xF2,        0xFA,       0xFB,       0xFC,       0xFF,        0x02,       0x05,
        // 3st
        0xF3,        0xFD,       0xFC,       0xFC,       0xFC,       0xFA,       0xFF,
        // 4st
        0xFD,       0xFF,        0xFE,        0xFE,        0xFA,       0xF6,        0xF7,
        // 5st
        0xFD,       0xFE,        0xFD,       0xFC,       0xF8,        0xF4,        0xF5,
        // 6st
        0xFD,       0xFD,       0xFC,       0xFB,       0xF6,        0xF2,        0xF2,
        // 7st
        0xFD,       0xFB,       0xFB,       0xF9,        0xF5,        0xF0,        0xF0,
        // 8st
        0xFD,       0xFA,       0xF9,        0xF8,        0xF3,        0xEE,       0xEE,
        // 9st
        0xFD,       0xF9,        0xF8,        0xF6,        0xF1,        0xEC,       0xEC,
        // 10st
        0xFD,       0xF8,        0xF7,        0xF5,        0xEF,        0xEA,       0xE9,
        // 11st
        0xFA,       0xFA,       0xFB,       0xFA,       0xFB,       0xFA,       0xFA,
        // 12st
        0xFA,       0xFA,       0xFA,       0xF9,        0xFA,       0xF8,        0xF8,
        // 13st
        0xFA,       0xFA,       0xFA,       0xF8,        0xF9,        0xF6,        0xF5,
        // 14st
        0xFB,       0xFA,       0xF9,        0xF7,        0xF7,        0xF4,        0xF3,
        // 15st
        0xFB,       0xFB,       0xF9,        0xF6,        0xF6,        0xF2,        0xF0,
        // 16st
        0xFB,       0xFB,       0xF8,        0xF5,        0xF5,        0xF0,        0xEE,
        // 17st
        0xFB,       0xFB,       0xF8,        0xF5,        0xF4,        0xEE,       0xEB,
        // 18st
        0xFC,       0xFB,       0xFC,        0xF4,        0xF2,        0xEC,       0xEE,
        // 19st
        0xF5,        0xFD,       0xFD,       0xFF,        0xFC,       0xFA,       0xFC,
        // 20st
        0xF5,        0xFB,       0xFB,       0xFC,       0xF9,        0xF6,        0xF9,
        // 21st
        0xF5,        0xFA,       0xFA,       0xFA,       0xF8,        0xF4,        0xF7,
        // 22st
        0xFE,        0x03,       0x02,       0x02,       0xFF,        0xFC,       0xFD,
        // 23st
        0xFE,        0x02,       0x01,       0x01,       0xFE,        0xFA,       0xFB,
        // 24st
        0xFE,        0x00,       0x00,       0xFF,        0xFC,       0xF8,        0xF9,
        // 25st
        0xFE,        0x03,       0x03,       0x04,       0x01,       0xFF,        0x01,
        // 26st
        0xFC,       0x00,       0x00,       0x01,       0xFE,        0xFC,       0xFE,
        // 27st
        0xFA,       0xFA,       0xFC,       0xFC,       0x0C,       0xF7,        0xFA,
        // 28st
        0x00,       0x03,       0x02,       0x03,       0xFF,        0xFC,       0xFE,
        // 29st
        0x04,       0xFF,       0x03,       0xF5,       0x00,       0xF0,       0xFD,
        // 30st
        0x08,       0x04,       0x03,       0x04,       0x00,       0xFC,       0xFC,
        // 31st
        0xF7,        0x03,       0x03,       0x06,       0x04,       0x04,       0x08,
        // 32st
        0xFA,       0x04,       0x06,       0x09,       0x09,       0x08,       0x0C,
        // 33st
        0xFC,       0x06,       0x09,       0x0C,       0x0D,       0x0C,       0x10,
        // 34st
        0xFA,       0x04,       0x05,       0x06,       0x06,       0x08,       0x0C,
        // 35st
        //0xFA,       0x06,       0x09,       0x0C,       0x0C,       0x0C,       0x10,  // delete 20180328 joe
        // 35st
        0xF4,        0x03,       0x03,       0x06,       0x08,       0x0C,       0x0F,
        //36st
        0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

//BICS4 LLR  ref : PS3111 Decoding_FLow_and_Error_Handling_Flow for BiCS4 20180807_(Ver Beta).pdf
#endif

#if YMTC
_FTL_BURNER_DCODE UBYTE gubRetryRTable_YMTC[(10 * 1) + (87 * 8)] =
{
	// Parameter setting table for pSLC 24Cycle, each 1 Byte
	0x00,
	0xF8,
	0xF0,
	0xF4,
	0xFC,
	0x08,
	0xE8,
	0x18,
	0xD8,
	0x28,

	// Parameter setting table for TLC 87 Cycle, each 8 Byte
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xFA,0xFA,0xFC,0xFC,0xFC,0xFA,0xF8,0x00,
	0xFA,0xFA,0xFA,0xFA,0xFA,0xFA,0xF5,0x00,
	0xFA,0xFA,0xFA,0xFA,0xFA,0xF8,0xF2,0x00,
	0xFE,0xFE,0xFE,0xFC,0xFC,0xFA,0xFA,0x00,
	0xFE,0x02,0x04,0x04,0x06,0x06,0x06,0x00,
	0xFA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xF8,0xF8,0xF8,0xF8,0xF8,0xF9,0xF9,0x00,
	0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
	0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,
	0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x00,
	0x00,0x00,0xFC,0x04,0x00,0x00,0x04,0x00,
	0x00,0x00,0x00,0x04,0xFC,0x00,0xFC,0x00,
	0x80,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,
	0xB0,0xFC,0x00,0xFC,0xFC,0x00,0xFC,0x00,
	0xF8,0xFC,0x00,0x00,0x00,0x04,0x00,0x00,
	0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x04,0x08,0x08,0x00,0x00,0x04,0x04,0x00,
	0x00,0x04,0x04,0x00,0x04,0x04,0x00,0x00,
	0x04,0x04,0x04,0x00,0x00,0x00,0xFC,0x00,
	0x00,0x08,0x08,0x04,0x00,0x04,0x00,0x00,
	0x00,0x04,0x04,0x04,0x00,0x00,0x00,0x00,
	0x00,0x04,0x08,0x04,0x00,0x04,0x00,0x00,
	0x00,0x04,0x08,0x04,0x00,0x04,0x00,0x00,
	0x00,0x08,0x04,0x04,0x04,0x08,0x00,0x00,
	0x8C,0xFC,0x64,0xF8,0xF8,0x00,0xF8,0x00,
	0xBC,0x00,0xFC,0xFC,0xFC,0xFC,0xF8,0x00,
	0xF8,0x00,0x00,0xFC,0xF8,0x00,0xF8,0x00,
	0xF8,0xFC,0x00,0xFC,0xFC,0x00,0xF8,0x00,
	0x00,0x00,0xF8,0x00,0xFC,0xF8,0xFC,0x00,
	0xB0,0xFC,0x00,0xF8,0xF8,0xFC,0xFC,0x00,
	0x8C,0xFC,0xF8,0xFC,0xF8,0xFC,0xFC,0x00,
	0xAC,0x00,0x00,0xFC,0xFC,0x00,0x00,0x00,
	0x04,0xFC,0xF4,0xF8,0xF4,0xF0,0xEC,0x00,
	0x04,0xFC,0xF8,0xF4,0xF0,0xF0,0xF0,0x00,
	0xFC,0xFC,0xF8,0xF4,0xF4,0xF0,0xF0,0x00,
	0x00,0xFC,0xF8,0xF4,0xF0,0xF0,0xF0,0x00,
	0xFC,0xFC,0xF8,0xF4,0xF0,0xF0,0xF0,0x00,
	0xFC,0x00,0xF8,0xF4,0xF4,0xF0,0xEC,0x00,
	0xFC,0xFC,0xF8,0xF0,0xF0,0xF0,0xF0,0x00,
	0x04,0xFC,0xF4,0xF4,0xF0,0xF0,0xEC,0x00,
	0xF8,0xFC,0xF8,0x00,0xF8,0x00,0xF8,0x00,
	0xAC,0x00,0xFC,0xFC,0xFC,0xFC,0xF8,0x00,
	0x00,0x00,0xF8,0xF8,0xF8,0xFC,0xF8,0x00,
	0xF4,0x04,0xFC,0xFC,0xFC,0xF8,0xF8,0x00,
	0xF4,0x00,0xFC,0xFC,0xF8,0xFC,0xFC,0x00,
	0x00,0x00,0xF8,0xFC,0xF8,0x00,0xF4,0x00,
	0x90,0xFC,0xFC,0x00,0xFC,0xFC,0xFC,0x00,
	0x84,0x00,0xF8,0xFC,0xF8,0xF8,0xF8,0x00,
	0xF8,0xFC,0xF4,0xEC,0xEC,0xEC,0xE4,0x00,
	0x00,0xFC,0xF4,0xF4,0xEC,0xEC,0xE4,0x00,
	0x00,0xFC,0xF4,0xF0,0xEC,0xEC,0xE8,0x00,
	0x00,0xFC,0xF0,0xF0,0xEC,0xEC,0xE8,0x00,
	0x00,0xFC,0xF0,0xEC,0xF0,0xEC,0xE8,0x00,
	0x00,0xFC,0xF4,0xEC,0xEC,0xEC,0xE8,0x00,
	0x00,0xFC,0xF0,0xF0,0xF0,0xEC,0xE8,0x00,
	0x00,0xFC,0xF0,0xF4,0xEC,0xEC,0xE8,0x00,
	0x04,0x00,0xFC,0xF8,0xFC,0xF8,0xEC,0x00,
	0x04,0xF8,0xF4,0xFC,0xF8,0xF4,0xEC,0x00,
	0x04,0xF8,0xF8,0xFC,0xF8,0xF4,0xEC,0x00,
	0x08,0x00,0xFC,0xFC,0xF8,0xF4,0xF0,0x00,
	0x08,0xF8,0xFC,0xF8,0xF8,0xF4,0xEC,0x00,
	0x08,0xF8,0xF4,0xF4,0xF8,0xF4,0xEC,0x00,
	0x08,0xF8,0xF8,0xF8,0xFC,0xF4,0xEC,0x00,
	0x08,0xFC,0xF8,0xF4,0xFC,0xF4,0xEC,0x00,
	0x08,0x00,0x00,0xF8,0xF8,0xF4,0xEC,0x00,
	0x04,0xFC,0xFC,0xF4,0xFC,0xF4,0xEC,0x00,
	0x08,0x00,0x00,0xF4,0xFC,0xF4,0xEC,0x00,
	0x08,0xF8,0xFC,0xF4,0xFC,0xF4,0xEC,0x00,
	0x04,0xF8,0xFC,0xFC,0xF8,0xF8,0xF0,0x00,
	0x08,0xF8,0xF8,0xFC,0xF8,0xF4,0xEC,0x00,
	0x08,0xFC,0xF4,0xF0,0xF4,0xF4,0xEC,0x00,
	0x08,0xFC,0xF8,0xF8,0xF8,0xF4,0xF0,0x00,
	0x0C,0x04,0x04,0x04,0xFC,0x00,0xF4,0x00,
	0x10,0x00,0x04,0x08,0x00,0x00,0x00,0x00,
	0x0C,0x04,0x04,0x08,0x00,0x00,0xF8,0x00,
	0x10,0x04,0x08,0x08,0x00,0x00,0xFC,0x00,
	0x14,0x04,0x08,0x08,0x04,0x00,0x00,0x00,
	0x10,0x04,0x04,0x0C,0x08,0x04,0x00,0x00,
	0x00,0x0C,0x14,0x0C,0x0C,0x10,0x0C,0x00,
	0x00,0x14,0x10,0x14,0x0C,0x14,0x08,0x00,
	0x10,0x0C,0x0C,0x0C,0x10,0x10,0x08,0x00,
	0x10,0x0C,0x14,0x18,0x10,0x10,0x10,0x00,
	0xA4,0xE4,0xF0,0xE0,0xE8,0xEC,0xEC,0x00,
	0x88,0xE4,0xE4,0xE0,0xEC,0xE8,0xE8,0x00,
	0xA4,0xE8,0xE0,0xE8,0xE8,0xE8,0xEC,0x00,
	0x9C,0xE0,0xE8,0xE8,0xE4,0xEC,0xEC,0x00

};

#endif


#if TLC
_FTL_BURNER_DCODE UBYTE SanDisk_sb_tlc_default_llr_table_2[96] = {
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

const UBYTE sFWVerStr[]  = __DATE__  ;          //12 bytes in ARC
const U32 gulMaskTable[32] = {0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFC,0xFFFFFFF8,0xFFFFFFF0,0xFFFFFFE0,0xFFFFFFC0,0xFFFFFF80
                                                ,0xFFFFFF00,0xFFFFFE00,0xFFFFFC00,0xFFFFF800,0xFFFFF000,0xFFFFE000,0xFFFFC000,0xFFFF8000
                                                ,0xFFFF0000,0xFFFE0000,0xFFFC0000,0xFFF80000,0xFFF00000,0xFFE00000,0xFFC00000,0xFF800000
                                                ,0xFF000000,0xFE000000,0xFC000000,0xF8000000,0xF0000000,0xE0000000,0xC0000000,0x80000000};
const UBYTE gub4kMaskTable[8] = {0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80};
const UBYTE gubZipTable[8] = {0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF};
const U32 gulSeed[8] = {0x000000,0xFFFFFF,0x555555,0xAAAAAA,0xCCCCCC,0x333333,0x999999,0x666666};
//UBYTE *gubRedundant = (UBYTE *)(SPARE_Base);           // 1k

// =========================================================table in DCCM=====================================================
ValidCount_t *gulVC = (ValidCount_t*)DCCM1_BANK0_STARADDRESS;
Variable_Table *VT = (Variable_Table *)DCCM1_BANK1_STARADDRESS;
D1LinkTable_t *gubD1LinkTable = (D1LinkTable_t *)(DCCM1_BANK1_STARADDRESS + 4096);
DEVSLP_Table *gDEVSLP_Table = (DEVSLP_Table *)(DCCM1_BANK1_STARADDRESS + 4096 + 128);
Identify_Table *IDT_temp = (Identify_Table *)(DCCM1_BANK1_STARADDRESS + 4096 + 512);
AON_SAVED_DATA * ReservedData = (AON_SAVED_DATA *)(DCCM1_BANK1_STARADDRESS + 4096 + 1024); // align for gen crc
FTarget_t *guwFreeBlockTable_D3 = (FTarget_t *)(DCCM1_BANK1_STARADDRESS + 8192);
FTarget_t *guwFreeBlockTable_D1 = (FTarget_t *)(DCCM1_BANK1_STARADDRESS + 8192 + 8192 - ((MAX_D1_UNIT_NUM + 4)*2));

// =========================================================table zone========================================================
P2LTable_t *gulBG_GCTable = (P2LTable_t*)(TableZoneBase - 512);							// 512B
UBYTE *gubEraseFailBitMap = (UBYTE *)(TableZoneBase);
Variable_Table *VT_Preformat_Temp = (Variable_Table *)(TableZoneBase);

#if MicronFlashOnly
#if B0KB
#if GR_1152KB
P2LTable_t *gulGRTable = (P2LTable_t *)(TableZoneBase); 									// 1M + 128K = 0x120000
P2LTable_t *gulGRTableTemp = (P2LTable_t *)(TableZoneBase + 0x120000);      					// GRTable / 32 = 36KB = 0x9000
UWORD *guwCleanGRGroups =  (UWORD*)(TableZoneBase + 0x129000); 						// 64K Group(for 256G) * 2B  = 128K = 0x20000
GRSearchTable_t *GRSearchTable = (GRSearchTable_t*)(TableZoneBase + 0x149000); 				// 64K Group(for 256G) * 4B  = 256K = 0x40000
U32 *gulGRSearchLinkTail =  (U32*)(TableZoneBase + 0x189000);								// 64K Group(for 256G) * 4B  = 256K
GRSearchTablePhysical_t *GRSearchTablePhysical = (GRSearchTablePhysical_t*)(TableZoneBase+ 0x1C9000); 	// 1M + 128K = 0x120000
U32 *GRSearchTableNext = (U32*)(TableZoneBase + 0x2E9000); 								// 1M +128K = 0x120000

// table header start
Table_L2P_t *gulTable_L2P = (Table_L2P_t *)(TableZoneBase + 0x409000);						// 240KB = 0x3C000
UWORD *guwTable_VC;
UBYTE *gubTable_P2L_BitMap = (UBYTE *)(TableZoneBase + 0x445000);							// 32KB = 0x8000
// table header end
UWORD *guwActiveTable_P2L = (UWORD*)(TableZoneBase + 0x44D000);			// for Micron 16CE*4P*512page*2B = 64KB = 0x10000

UWORD *guwL2P_InRamIndex = (UWORD*)(TableZoneBase + 0x45D000);						// 120KB = 0x1E000
L2P_InRamInverse_t *gulL2P_InRamInverse = (L2P_InRamInverse_t*)(TableZoneBase + 0x47B000);		// 8KB = 0x2000

// RAM Size is not engough for ReadDisturb infomation [Sam]

// RUT = (2B*(543)Blocks*4plane)*2CH*IL8 => 128KB = 0x20000, but use 12CE in GR-2MB,
UWORD *guwRUTBASE = (UWORD *)(TableZoneBase+0x47D000);
UBYTE *gubZ2Buffer_NCS = (UBYTE*)(TableZoneBase + 0x49B000);									// 8KB = 0x2000
//UWORD *guwRUT[MAX_CE] = {(UWORD *)(TableZoneBase+0x47D000),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 1),(UWORD *)(TableZoneBase+0x47D000+ RUTSizePerCE * 2),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 3),
//                                                (UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 4),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 5),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 6),(UWORD *)(TableZoneBase+0x47D000+ RUTSizePerCE * 7),
//                                                (UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 8),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 9),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 10),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 11)
//                                                //(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 12),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 13),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 14),(UWORD *)(TableZoneBase+0x47D000 + RUTSizePerCE * 15)
//                                     			};

EraseCount_t *gulEC_D3 = (EraseCount_t*)(TableZoneBase + 0x49D000);											// 16KB = 0x4000
#if TLC
EraseCount_t *gulEC_D1 = (EraseCount_t*)(TableZoneBase + 0x4A1000 - (((MAX_D1_UNIT_NUM+7)&(~7))*4));			//  D1_UNIT_NUM Unit*4B
#else
EraseCount_t *gulEC_D1 = (EraseCount_t*)(TableZoneBase + 0x4A1000 - (D1_UNIT_NUM*4));									//  D1_UNIT_NUM Unit*4B
#endif

UWORD *guwTableP2L_GC = (UWORD*)(TableZoneBase + 0x4A1000);		// 64KB = 0x10000
// BUFFER3_BASE = TableZoneBase + 0x4B1000                                                      // 32KB = 0x8000
UBYTE *gubHostTable = (UBYTE*)(BUFFER3_BASE + 0x4000);									// 16KB, BUFFER3_BASE = (TableZoneBase+0x487000), Must 4KB align for sata......
U32 *gulRSFrameSpareBits = (U32*)(TableZoneBase + 0x4B9000);  								// 64B * 2 * 32 maxtags = 4KB

L2PTable_t *gulL2PBase =  (L2PTable_t *)(TableZoneBase +0x4BA000);								// Write : 6MB, Read : 13MB+416KB
UBYTE *gubL2PBase =  (UBYTE *)(TableZoneBase + 0x4BA000);
L2PTable_t *L2PTable;
// Table For CloseTarget, get from L2P Read Mode, 11MB + 384KB
// (W 384 + R 58) * 16KB = 0x6E8000
//============================
//                       GC
//============================
UBYTE *gubGCP2LBuffer =  (UBYTE*)(TableZoneBase + 0xBA2000); 								// 2M + 256K = 0x240000
GCSearchTable_t *P2LSearchTable = (GCSearchTable_t*)(TableZoneBase + 0xDE2000); 					// (64K * 4) Group(for 1T) * 8B = 2M = 0x200000
U32 *P2LSearchTableNext = (U32*)(TableZoneBase + 0xFE2000); 								// (GCP2LBufferSize / 4) * 4B = 2M +256K = 0x240000
GCTable_t *guoGCTable = (GCTable_t *)( TableZoneBase + 0x1222000); 								// 2M + 256K = 0x240000

P2LTable_t *gulGCGRTable = (P2LTable_t *)(TableZoneBase + 0x1462000); 							// 1M + 128K = 0x120000
UWORD *guwCleanGCGRGroups =  (UWORD*)(TableZoneBase + 0x1582000); 							// 64K Group(for 256G) * 2B  = 128K = 0x20000
GRSearchTable_t *GCGRSearchTable = (GRSearchTable_t*)(TableZoneBase + 0x15A2000); 					// 64K Group(for 256G) * 4B  = 256K = 0x40000
GRSearchTablePhysical_t *GCGRSearchTablePhysical = (GRSearchTablePhysical_t*)(TableZoneBase + 0x15E2000);  	// (GCGRTableSize / 4) * 4B = 1M + 128K = 0x120000
U32 *GCGRSearchTableNext = (U32*)(TableZoneBase + 0x1702000); 								// (GCGRTableSize / 4) * 4B = 1M + 128K = 0x120000
//============================
//                       BG
//============================
P2LTable_t *gulBG_P2LTable = (P2LTable_t *)(TableZoneBase + 0x15E2000);//(1MB +128KB) *2 = 0x240000
//============================
//                       DLMC
//============================
void *gMicroCodeBuffer = (void *)(TableZoneBase + 0x1722000); // 1MB = 0x100000
//============================
//                      Vender
//============================
UWORD *guwVenderSMARTBuffer = (UWORD*)(TableZoneBase + 0x1802000); // 128KB = 0x20000
U32 *gulVenderSMARTBuffer = (U32*)(TableZoneBase + 0x1802000);
//============================
//                       Preformat
//============================
UWORD *guwEarlyBadCntPerPlane = (UWORD*)(TableZoneBase + 0x1802000);//MaxDie*MaxPlane
UBYTE *gubReserveUnitsPerGroup = (UBYTE*)(TableZoneBase + 0x1802000) + 256 * 2;//MaxDie * MaxInterleave
UWORD *guwUnitsPerGroups = (UWORD*)(TableZoneBase + 0x1802000) + 256 * 2 + 16;
UWORD *guwExtendUnitsIndex = (UWORD*)(TableZoneBase + 0x1802000)+ 256 * 2 + 16 + 16*2;
UWORD *guwRUTUnitIndexOfLastBlock = (UWORD*)(TableZoneBase + 0x1802000)+ 256 * 2 + 16 + 16*2 + 16*2;
U32 *gulRUTDieCompensatePTR = (U32 *)((TableZoneBase + 0x1802000) + 0x10000 + 256 * 4 * 2);// 4Die 16CE=64
UWORD *guwDieBlockAvg = (UWORD *)((TableZoneBase + 0x1802000) + 0x10000 + 256 * 4 * 2 + 64 * 4);

UBYTE *gubTrimTable = (UBYTE*)(TableZoneBase + 0x1822000);									// 256KB = 0x40000
UBYTE *gubDataInGR = (UBYTE*)(TableZoneBase + 0x1862000); 									// 512KB = 0x80000
Seq_Table_t *guoSequential_PhyHead = (Seq_Table_t*)(TableZoneBase + 0x18E2000);						// 256KB  1T以下才處理跨Unit
Seq_Table_1T_t *guoSequential_PhyHead_1T = (Seq_Table_1T_t*)(TableZoneBase + 0x18E2000);			// 256KB  1T case
UBYTE *gubZ2Buffer = (UBYTE*)(TableZoneBase + 0x1922000);									// 16KB = 0x4000

#else
P2LTable_t *gulGRTable = (P2LTable_t *)(TableZoneBase); 									// 1M + 32K = 0x108000
P2LTable_t *gulGRTableTemp = (P2LTable_t *)(TableZoneBase + 0x109000);      					// 104KB, GRTable / 32 + (64 plane * 4 page * 4 4kentry * 4B)
UWORD *guwCleanGRGroups =  (UWORD*)(TableZoneBase + 0x123000); 						// 64K Group(for 256G) * 2B  = 128K = 0x20000
GRSearchTable_t *GRSearchTable = (GRSearchTable_t*)(TableZoneBase + 0x143000); 				// 64K Group(for 256G) * 4B  = 256K = 0x40000
U32 *gulGRSearchLinkTail =  (U32*)(TableZoneBase + 0x183000);								// 64K Group(for 256G) * 4B  = 256K
GRSearchTablePhysical_t *GRSearchTablePhysical = (GRSearchTablePhysical_t*)(TableZoneBase+ 0x1C3000); 	// 1M + 32K
U32 *GRSearchTableNext = (U32*)(TableZoneBase + 0x2CB000); 								// 1M + 32K

// table header start
Table_L2P_t *gulTable_L2P = (Table_L2P_t *)(TableZoneBase + 0x3D3000);						// 240KB = 0x3C000
UWORD *guwTable_VC;
UBYTE *gubTable_P2L_BitMap = (UBYTE *)(TableZoneBase + 0x40F000);                                                  // 32KB = 0x8000
// table header end

UWORD *guwL2P_InRamIndex = (UWORD*)(TableZoneBase + 0x417000);						// 120KB = 0x1E000
L2P_InRamInverse_t *gulL2P_InRamInverse = (L2P_InRamInverse_t*)(TableZoneBase + 0x435000);		// 64KB = 0x10000

UWORD *guwUnitReadCnt = (UWORD*) (TableZoneBase + 0x435000 + 0x10000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB
RC_Info_t *guwUnitReadCheckInfo = (RC_Info_t*) (TableZoneBase + 0x435000 + 0x10000 - 0x2000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB
#if (Read_Chk_ByHostRead || Read_Chk_ByReadScan)
UWORD *guwUnitRRCnt = (UWORD*)(TableZoneBase + 0x435000 + 0x10000 - 0x2000 - 0x2000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB
#endif
ForceRead_Info_t *guwForceRead_Info = (ForceRead_Info_t*)(TableZoneBase + 0x435000 + 0x10000 - 0x2000 - 0x2000 - 0x2000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB

// RUT = (2B* 548Blocks*4plane)*2CH*IL8 = 4384B*16 => RUTSizePerCE(8KB)*16 or RUTSizePerPlane*64 = 128KB = 0x20000
// RUT = 0x2800 * 12 = 0x1E000, 120KB, but still occupite 128KB=0x20000
UWORD *guwRUTBASE = (UWORD *)(TableZoneBase+0x445000);
UBYTE *gubZ2Buffer_NCS = (UBYTE*)(TableZoneBase + 0x463000);									// 8KB = 0x2000
//UWORD *guwRUT[MAX_CE] = {(UWORD *)(TableZoneBase+0x445000),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 1),(UWORD *)(TableZoneBase+0x445000+ RUTSizePerCE * 2),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 3),
//                                                (UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 4),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 5),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 6),(UWORD *)(TableZoneBase+0x445000+ RUTSizePerCE * 7),
//                                                (UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 8),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 9),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 10),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 11),
//                                                (UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 12),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 13),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 14),(UWORD *)(TableZoneBase+0x445000 + RUTSizePerCE * 15)};

UWORD *guwActiveTable_P2L = (UWORD*)(TableZoneBase + 0x465000);                 // for Micron 16CE*4P*512page*2B = 64KB
UWORD *guwTableP2L_GC = (UWORD*)(TableZoneBase + 0x475000 );             // 64KB
EraseCount_t *gulEC_D3 = (EraseCount_t*)(TableZoneBase + 0x485000);                                                                                  // 16KB
EraseCount_t *gulEC_D1 = (EraseCount_t*)(TableZoneBase + 0x489000 - (((MAX_D1_UNIT_NUM+7)&(~7))*4));                   //  D1_UNIT_NUM Unit*4B

UBYTE *gubHostTable = (UBYTE*)(BUFFER3_BASE + 0x4000);									// 16KB, BUFFER3_BASE = (TableZoneBase+0x487000), Must 4KB align for sata......
U32 *gulRSFrameSpareBits = (U32*)(TableZoneBase + 0x491000);  								// 64B * 2 * 32 maxtags = 4KB

L2PTable_t *gulL2PBase =  (L2PTable_t *)(TableZoneBase +0x492000);								// Write : 8MB, Read : 11.5MB
UBYTE *gubL2PBase =  (UBYTE *)(TableZoneBase + 0x492000);
L2PTable_t *L2PTable;
// Table For CloseTarget, get from L2P Read Mode, 11MB + 384KB
// (X) Start = 8MB + 128KB = 0x820000
// (O) Start = 8MB + 32KB = 0x808000
UBYTE *gubGCP2LBuffer =  (UBYTE*)(TableZoneBase + 0xC9A000); 								// 2M + 64K

GCSearchTable_t *P2LSearchTable = (GCSearchTable_t*)(TableZoneBase + 0xEAA000); 					// (64K * 4) Group(for 1T) * 8B = 2M
U32 *P2LSearchTableNext = (U32*)(TableZoneBase + 0x10AA000); 								// (GCP2LBufferSize / 4) * 4B = 2M + 64K

GCTable_t *guoGCTable = (GCTable_t *)( TableZoneBase + 0x12BA000); 								// 2M + 64K
P2LTable_t *gulGCGRTable = (P2LTable_t *)(TableZoneBase + 0x14CA000); 							// 1M + 32K
UWORD *guwCleanGCGRGroups =  (UWORD*)(TableZoneBase + 0x15D2000); 							// 64K Group(for 256G) * 2B  = 128K
GRSearchTable_t *GCGRSearchTable = (GRSearchTable_t*)(TableZoneBase + 0x15F2000); 					// 64K Group(for 256G) * 4B  = 256K
GRSearchTablePhysical_t *GCGRSearchTablePhysical = (GRSearchTablePhysical_t*)(TableZoneBase + 0x1632000);  	// (GCGRTableSize / 4) * 4B = 1M + 32K
U32 *GCGRSearchTableNext = (U32*)(TableZoneBase + 0x173A000); 								// (GCGRTableSize / 4) * 4B = 1M + 32K

P2LTable_t *gulBG_P2LTable = (P2LTable_t *)(TableZoneBase + 0x1632000);
void *gMicroCodeBuffer = (void *)(TableZoneBase + 0x173A000);
UWORD *guwVenderSMARTBuffer = (UWORD*)(TableZoneBase + 0x181A000);
U32 *gulVenderSMARTBuffer = (U32*)(TableZoneBase + 0x181A000);
UWORD *guwEarlyBadCntPerPlane = (UWORD*)(TableZoneBase + 0x181A000);//MaxDie*MaxPlane
UBYTE *gubReserveUnitsPerGroup = (UBYTE*)(TableZoneBase + 0x181A000) + 256 * 2;//MaxDie * MaxInterleave
UWORD *guwUnitsPerGroups = (UWORD*)(TableZoneBase + 0x181A000) + 256 * 2 + 16;
UWORD *guwExtendUnitsIndex = (UWORD*)(TableZoneBase + 0x181A000)+ 256 * 2 + 16 + 16*2;
UWORD *guwRUTUnitIndexOfLastBlock = (UWORD*)(TableZoneBase + 0x181A000)+ 256 * 2 + 16 + 16*2 + 16*2;

U32 *gulRUTDieCompensatePTR = (U32 *)((TableZoneBase + 0x181A000) + 0x10000 + 256 * 4 * 2);// 4Die 16CE=64
UWORD *guwDieBlockAvg = (UWORD *)((TableZoneBase + 0x181A000) + 0x10000 +  256 * 4 * 2 + 64 * 4);

UBYTE *gubTrimTable = (UBYTE*)(TableZoneBase + 0x1842000);									// 256KB
UBYTE *gubDataInGR = (UBYTE*)(TableZoneBase + 0x1882000); 									// 256KB

Seq_Table_t *guoSequential_PhyHead = (Seq_Table_t*)(TableZoneBase + 0x18C2000);						// 256KB  1T以下才處理跨Unit
Seq_Table_1T_t *guoSequential_PhyHead_1T = (Seq_Table_1T_t*)(TableZoneBase + 0x18C2000);			// 256KB  1T case
UBYTE *gubZ2Buffer = (UBYTE*)(TableZoneBase + 0x1902000);									// 16KB
#endif

#else //L06B
P2LTable_t *gulGRTable = (P2LTable_t *)(TableZoneBase); 									// 1M = 0x100000
P2LTable_t *gulGRTableTemp = (P2LTable_t *)(TableZoneBase + 0x100000);      					// 100KB = 0x19000, GRTable / 32 + (64 plane * 4 page * 4 4kentry * 4B)
UWORD *guwCleanGRGroups =  (UWORD*)(TableZoneBase + 0x119000); 						// 64K Group(for 256G) * 2B  = 128K = 0x20000
GRSearchTable_t *GRSearchTable = (GRSearchTable_t*)(TableZoneBase + 0x139000); 				// 64K Group(for 256G) * 4B  = 256K = 0x40000
U32 *gulGRSearchLinkTail =  (U32*)(TableZoneBase + 0x179000);								// 64K Group(for 256G) * 4B  = 256K
GRSearchTablePhysical_t *GRSearchTablePhysical = (GRSearchTablePhysical_t*)(TableZoneBase+ 0x1B9000); 	// 256k *4B = 1M
U32 *GRSearchTableNext = (U32*)(TableZoneBase + 0x2B9000); 								// 256k *4B = 1M

// table header start
Table_L2P_t *gulTable_L2P = (Table_L2P_t *)(TableZoneBase + 0x3B9000);						// 240KB = 0x3C000
UWORD *guwTable_VC;
UBYTE *gubTable_P2L_BitMap = (UBYTE *)(TableZoneBase + 0x3F5000);							// 32KB = 0x8000
// table header end
//UWORD *guwActiveTable_P2L = (UWORD*)(TableZoneBase + 0x3FD000);						// 32KB
UWORD *guwL2P_InRamIndex = (UWORD*)(TableZoneBase + 0x405000);						// 120KB = 0x1E000
L2P_InRamInverse_t *gulL2P_InRamInverse = (L2P_InRamInverse_t*)(TableZoneBase + 0x423000);		// 64KB = 0x10000

UWORD *guwUnitReadCnt = (UWORD*) (TableZoneBase + 0x423000 + 0x10000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB
RC_Info_t *guwUnitReadCheckInfo = (RC_Info_t*) (TableZoneBase + 0x423000 + 0x10000 - 0x2000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB
#if (Read_Chk_ByHostRead || Read_Chk_ByReadScan)
UWORD *guwUnitRRCnt = (UWORD*)(TableZoneBase + 0x423000 + 0x10000 - 0x2000 - 0x2000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB
#endif
ForceRead_Info_t *guwForceRead_Info = (ForceRead_Info_t*)(TableZoneBase + 0x423000 + 0x10000 - 0x2000 - 0x2000 - 0x2000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB


// RUT = (2B* 548Blocks*4plane)*2CH*IL8 = 4384B*16 => RUTSizePerCE(8KB)*16 or RUTSizePerPlane*64 = 128KB = 0x20000
UWORD *guwRUTBASE = (UWORD *)(TableZoneBase+0x433000);
UBYTE *gubZ2Buffer_NCS = (UBYTE*)(TableZoneBase + 0x461000);									// 8KB = 0x2000

// RUT = (2B*(4096+256)Blocks*4plane)*2CH*IL8 = 544KB = 0x88000
//UWORD *guwRUT[MAX_CE] = {(UWORD *)(TableZoneBase+0x433000),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 1),(UWORD *)(TableZoneBase+0x433000+ RUTSizePerCE * 2),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 3),
//                                                (UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 4),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 5),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 6),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 7),
//                                                (UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 8),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 9),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 10),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 11),
//                                                (UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 12),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 13),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 14),(UWORD *)(TableZoneBase+0x433000 + RUTSizePerCE * 15)};
EraseCount_t *gulEC_D3 = (EraseCount_t*)(TableZoneBase + 0x4BB000);											// 16KB
EraseCount_t *gulEC_D1 = (EraseCount_t*)(TableZoneBase + 0x4BF000 - (((MAX_D1_UNIT_NUM+7)&(~7))*4));			//  D1_UNIT_NUM Unit*4B
UWORD *guwActiveTable_P2L = (UWORD*)(TableZoneBase + 0x433000 + (RUTSize));			// for Micron 16CE*4P*512page*2B = 64KB
UWORD *guwTableP2L_GC = (UWORD*)(TableZoneBase + 0x433000 + (RUTSize) + 64*1024);										// 64KB
UBYTE *gubHostTable = (UBYTE*)(BUFFER3_BASE + 0x4000);									// 16KB, BUFFER3_BASE = (TableZoneBase+0x4B7000), Must 4KB align for sata......
U32 *gulRSFrameSpareBits = (U32*)(TableZoneBase + 0x4CF000);  								// 64B * 2 * 32 maxtags = 4KB

L2PTable_t *gulL2PBase =  (L2PTable_t *)(TableZoneBase +0x4D0000);								// Write : 8MB, Read : 11.5MB
UBYTE *gubL2PBase =  (UBYTE *)(TableZoneBase + 0x4D0000);
L2PTable_t *L2PTable;
// Table For CloseTarget, get from L2P Read Mode, 11MB + 384KB
// (X) Start = 8MB + 128KB = 0x820000
// (O) Start = 8MB + 32KB = 0x808000
UBYTE *gubGCP2LBuffer =  (UBYTE*)(TableZoneBase + 0xCD8000); 								// 2M

GCSearchTable_t *P2LSearchTable = (GCSearchTable_t*)(TableZoneBase + 0xED8000); 					// (64K * 4) Group(for 1T) * 8B = 1M
U32 *P2LSearchTableNext = (U32*)(TableZoneBase + 0x10D8000); 								// (GCP2LBufferSize / 4) * 4B = 2M

GCTable_t *guoGCTable = (GCTable_t *)( TableZoneBase + 0x12D8000); 								// 2M

P2LTable_t *gulGCGRTable = (P2LTable_t *)(TableZoneBase + 0x14D8000); 							// 1M
UWORD *guwCleanGCGRGroups =  (UWORD*)(TableZoneBase + 0x15D8000); 							// 64K Group(for 256G) * 2B  = 128K
GRSearchTable_t *GCGRSearchTable = (GRSearchTable_t*)(TableZoneBase + 0x15F8000); 					// 64K Group(for 256G) * 4B  = 256K
GRSearchTablePhysical_t *GCGRSearchTablePhysical = (GRSearchTablePhysical_t*)(TableZoneBase + 0x1638000);  	// (GCGRTableSize / 4) * 4B = 1M
U32 *GCGRSearchTableNext = (U32*)(TableZoneBase + 0x1738000); 								// (GCGRTableSize / 4) * 4B = 1M

P2LTable_t *gulBG_P2LTable = (P2LTable_t *)(TableZoneBase + 0x1638000);
void *gMicroCodeBuffer = (void *)(TableZoneBase + 0x1738000);
UWORD *guwVenderSMARTBuffer = (UWORD*)(TableZoneBase + 0x1818000);
U32 *gulVenderSMARTBuffer = (U32*)(TableZoneBase + 0x1818000);
UWORD *guwEarlyBadCntPerPlane = (UWORD*)(TableZoneBase + 0x1818000);//MaxDie*MaxPlane
UBYTE *gubReserveUnitsPerGroup = (UBYTE*)(TableZoneBase + 0x1818000) + 256 * 2;//MaxDie * MaxInterleave
UWORD *guwUnitsPerGroups = (UWORD*)(TableZoneBase + 0x1818000) + 256 * 2 + 16;
UWORD *guwExtendUnitsIndex = (UWORD*)(TableZoneBase + 0x1818000)+ 256 * 2 + 16 + 16*2;
UWORD *guwRUTUnitIndexOfLastBlock = (UWORD*)(TableZoneBase + 0x1818000)+ 256 * 2 + 16 + 16*2 + 16*2;
U32 *gulRUTDieCompensatePTR = (U32 *)((TableZoneBase + 0x1818000) + 0x10000 + 256 * 4 * 2);// 4Die 16CE=64
UWORD *guwDieBlockAvg = (UWORD *)((TableZoneBase + 0x1818000) + 0x10000 + 256 * 4 * 2 + 64 * 4);

UBYTE *gubTrimTable = (UBYTE*)(TableZoneBase + 0x1838000);									// 192KB
UBYTE *gubDataInGR = (UBYTE*)(TableZoneBase + 0x1868000); 									// 512KB

Seq_Table_t *guoSequential_PhyHead = (Seq_Table_t*)(TableZoneBase + 0x18E8000);						// 256KB  1T以下才處理跨Unit
Seq_Table_1T_t *guoSequential_PhyHead_1T = (Seq_Table_1T_t*)(TableZoneBase + 0x18E8000);			// 256KB  1T case
UBYTE *gubZ2Buffer = (UBYTE*)(TableZoneBase + 0x1928000);									// 16KB
#endif
#else
P2LTable_t *gulGRTable = (P2LTable_t *)(TableZoneBase); 									// 1M = 0x100000
P2LTable_t *gulGRTableTemp = (P2LTable_t *)(TableZoneBase + 0x100000);      					// 36KB = 0x9000, GRTable / 32 + (64 plane * 4 page * 4 4kentry * 4B)
UWORD *guwCleanGRGroups =  (UWORD*)(TableZoneBase + 0x109000); 						// 64K Group(for 256G) * 2B  = 128K = 0x20000
GRSearchTable_t *GRSearchTable = (GRSearchTable_t*)(TableZoneBase + 0x129000); 				// 64K Group(for 256G) * 4B  = 256K = 0x40000
U32 *gulGRSearchLinkTail =  (U32*)(TableZoneBase + 0x169000);								// 64K Group(for 256G) * 4B  = 256K
GRSearchTablePhysical_t *GRSearchTablePhysical = (GRSearchTablePhysical_t*)(TableZoneBase+ 0x1A9000); 	// 256k *4B = 1M
U32 *GRSearchTableNext = (U32*)(TableZoneBase + 0x2A9000); 								// 256k *4B = 1M

// table header start
Table_L2P_t *gulTable_L2P = (Table_L2P_t *)(TableZoneBase + 0x3A9000);						// 240KB = 0x3C000
UWORD *guwTable_VC;
UBYTE *gubTable_P2L_BitMap = (UBYTE *)(TableZoneBase + 0x3E5000);							// 32KB = 0x8000
// table header end
UWORD *guwActiveTable_P2L = (UWORD*)(TableZoneBase + 0x3ED000);						// 32KB
UWORD *guwL2P_InRamIndex = (UWORD*)(TableZoneBase + 0x3F5000);						// 120KB = 0x1E000
L2P_InRamInverse_t *gulL2P_InRamInverse = (L2P_InRamInverse_t*)(TableZoneBase + 0x413000);		// 64KB = 0x10000

UWORD *guwUnitReadCnt = (UWORD*) (TableZoneBase + 0x413000 + 0x10000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB
RC_Info_t *guwUnitReadCheckInfo = (RC_Info_t*) (TableZoneBase + 0x413000 + 0x10000 - 0x2000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB
#if (Read_Chk_ByHostRead || Read_Chk_ByReadScan)
UWORD *guwUnitRRCnt = (UWORD*)(TableZoneBase + 0x413000 + 0x10000 - 0x2000 - 0x2000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB
#endif
ForceRead_Info_t *guwForceRead_Info = (ForceRead_Info_t*)(TableZoneBase + 0x413000 + 0x10000 - 0x2000 - 0x2000 - 0x2000 - 0x2000);//偷用gulL2P_InRamInverse後面8KB


UWORD *guwRUTBASE = (UWORD *)(TableZoneBase+0x423000);
UBYTE *gubZ2Buffer_NCS = (UBYTE*)(TableZoneBase + 0x4A9000);									// 8KB = 0x2000
#if DEBUG_ULTRAMLC_POWER_FAIL && ENABLE_PFAIL
UWORD * plane_window = (UWORD *)(TableZoneBase + 0x423000 + RUTSizePerCE * 14);
U32 * trace_arr = (U32 *)(TableZoneBase + 0x423000 + RUTSizePerCE * 15);
#endif
EraseCount_t *gulEC_D3 = (EraseCount_t*)(TableZoneBase + 0x4AB000);											// 16KB
EraseCount_t *gulEC_D1 = (EraseCount_t*)(TableZoneBase + 0x4AF000 - (((MAX_D1_UNIT_NUM+7)&(~7))*4));			//  D1_UNIT_NUM Unit*4B
UWORD *guwTableP2L_GC = (UWORD*)(TableZoneBase + 0x4AF000);										// 32KB
UBYTE *gubHostTable = (UBYTE*)(BUFFER3_BASE + 0x4000);									// 16KB, BUFFER3_BASE = (TableZoneBase+0x4B7000), Must 4KB align for sata......
U32 *gulRSFrameSpareBits = (U32*)(TableZoneBase + 0x4BF000);  								// 64B * 2 * 32 maxtags = 4KB

L2PTable_t *gulL2PBase =  (L2PTable_t *)(TableZoneBase +0x4C0000);								// Write : 8MB, Read : 11.5MB
UBYTE *gubL2PBase =  (UBYTE *)(TableZoneBase + 0x4C0000);
L2PTable_t *L2PTable;
// Table For CloseTarget, get from L2P Read Mode, 11MB + 384KB
// (X) Start = 8MB + 128KB = 0x820000
// (O) Start = 8MB + 32KB = 0x808000
#if BICS2_GC_TO_D3
// (O) Start = 7MB + 32KB = 0x708000, 1MB L2P space for copy data
UBYTE *gubCopyBufferExt = (UBYTE*)(TableZoneBase + 0xBC8000);								// 1M
#endif
UBYTE *gubGCP2LBuffer =  (UBYTE*)(TableZoneBase + 0xCC8000); 								// 2M

GCSearchTable_t *P2LSearchTable = (GCSearchTable_t*)(TableZoneBase + 0xEC8000); 					// (64K * 4) Group(for 1T) * 8B = 1M
U32 *P2LSearchTableNext = (U32*)(TableZoneBase + 0x10C8000); 								// (GCP2LBufferSize / 4) * 4B = 2M

GCTable_t *guoGCTable = (GCTable_t *)( TableZoneBase + 0x12C8000); 								// 2M

P2LTable_t *gulGCGRTable = (P2LTable_t *)(TableZoneBase + 0x14C8000); 							// 1M
UWORD *guwCleanGCGRGroups =  (UWORD*)(TableZoneBase + 0x15C8000); 							// 64K Group(for 256G) * 2B  = 128K
GRSearchTable_t *GCGRSearchTable = (GRSearchTable_t*)(TableZoneBase + 0x15E8000); 					// 64K Group(for 256G) * 4B  = 256K
GRSearchTablePhysical_t *GCGRSearchTablePhysical = (GRSearchTablePhysical_t*)(TableZoneBase + 0x1628000);  	// (GCGRTableSize / 4) * 4B = 1M
U32 *GCGRSearchTableNext = (U32*)(TableZoneBase + 0x1728000); 								// (GCGRTableSize / 4) * 4B = 1M

P2LTable_t *gulBG_P2LTable = (P2LTable_t *)(TableZoneBase + 0x1628000);
void *gMicroCodeBuffer = (void *)(TableZoneBase + 0x1728000);
UWORD *guwVenderSMARTBuffer = (UWORD*)(TableZoneBase + 0x1808000);
U32 *gulVenderSMARTBuffer = (U32*)(TableZoneBase + 0x1808000);
UWORD *guwEarlyBadCntPerPlane = (UWORD*)(TableZoneBase + 0x1808000);//MaxDie*MaxPlane
UBYTE *gubReserveUnitsPerGroup = (UBYTE*)(TableZoneBase + 0x1808000) + 256 * 2;//MaxDie * MaxInterleave
UWORD *guwUnitsPerGroups = (UWORD*)(TableZoneBase + 0x1808000) + 256 * 2 + 16;
UWORD *guwExtendUnitsIndex = (UWORD*)(TableZoneBase + 0x1808000)+ 256 * 2 + 16 + 16*2;
UWORD *guwRUTUnitIndexOfLastBlock = (UWORD*)(TableZoneBase + 0x1808000)+ 256 * 2 + 16 + 16*2 + 16*2;
U32 *gulRUTDieCompensatePTR = (U32 *)((TableZoneBase + 0x1808000) + 0x10000 + 256 * 4 * 2);// 4Die 16CE=64
UWORD *guwDieBlockAvg = (UWORD *)((TableZoneBase + 0x1808000) + 0x10000 + 256 * 4 * 2 + 64 * 4);

UBYTE *gubTrimTable = (UBYTE*)(TableZoneBase + 0x1828000);									// 256KB
UBYTE *gubDataInGR = (UBYTE*)(TableZoneBase + 0x1868000); 									// 512KB

Seq_Table_t *guoSequential_PhyHead = (Seq_Table_t*)(TableZoneBase + 0x18E8000);						// 256KB  1T以下才處理跨Unit
Seq_Table_1T_t *guoSequential_PhyHead_1T = (Seq_Table_1T_t*)(TableZoneBase + 0x18E8000);			// 256KB  1T case
UBYTE *gubZ2Buffer = (UBYTE*)(TableZoneBase + 0x1928000);									// 16KB
#endif

// Table For DDR Cache, SDR FW dosen't have these tables.
UWORD *guwDataCacheL2PTable_L2 =  (UWORD*)(TableZoneBase + 0x18A0000);	 					//1024*512*2B = 1M
UWORD *guwDataCacheL2PTable_L1 = (UWORD *)(TableZoneBase + 0x19A0000);						//1M
UWORD *guwFreeSDRL2PTableLink = (UWORD *)(TableZoneBase + 0x1AA0000);						//1024*2=2K
SDR_DataEntryTable_t *SDR_DataEntryTable = (SDR_DataEntryTable_t*)(TableZoneBase + 0x1AA0800); 		//8B*1024 = 8K
UBYTE *gubSDRL2PEntryCount = (UBYTE *)(TableZoneBase + 0x1AA2800);							//1024
// End of Table For DDR Cache

#if (ENABLE_DDR_MODE)
// Debug Table
Variable_Table *VTBackUp = (Variable_Table *)(TableZoneBase+0x1AA2C00); 							// debug 16KB
Variable_Table *VTBackUp2 = (Variable_Table *)(TableZoneBase+0x1AA6C00); 							// debug 16KB
PowerCycleDebug_Info*PowerCycle = (PowerCycleDebug_Info*)(TableZoneBase+0x1AAAC00); 				// debug 74B
#if(ENABLE_DEBUG_ZByteNumSumTable)
U32 *gulZByteNumSumTable =  (U32*)(TableZoneBase+0x1B27080); 								// (GCP2LBufferSize / 4) * 4B = 2M
#endif
#else
// Debug Table
Variable_Table *VTBackUp = (Variable_Table *)(TableZoneBase + 0x1928000); 							// debug 8KB
Variable_Table *VTBackUp2 = (Variable_Table *)(TableZoneBase + 0x192A000); 							// debug 8KB
PowerCycleDebug_Info*PowerCycle = (PowerCycleDebug_Info*)(TableZoneBase + 0x192C800); 				// debug
#endif

#if PH_SQL_FUNC
UBYTE *gubNandVToolCmdBuffer =  (UBYTE*)(BUFFER3_BASE); //  4K
UBYTE *gubNandVToolDataBuffer =  (UBYTE*)((BUFFER3_BASE)+0x1000); // 20K
UWORD *guwNandVToolDataBuffer = (UWORD*)((BUFFER3_BASE)+0x1000);//20K
UBYTE *gubNandVToolECCBuffer =  (UBYTE*)((BUFFER3_BASE)+0x6000); // 1K
UBYTE *gubNandVToolRawBuffer = (UBYTE*)((BUFFER3_BASE)+0x6400);//25K //(Z2_DDR_BASE);
U32 *gulNandVToolTimerCntBuffer = (U32*)((BUFFER3_BASE)+0xC800);
#endif
U32 gulNandPhysicalPageSize;

// ===================================================data zone====================================================//0x61CD3000
// Total 3MB + 128KB
UBYTE *gubCopyBuffer = (UBYTE *)(DataZoneBase+0x000000);      		// 2M,  if (ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING) 2M -> 1M, 後面1M給program table unit fail使用
UBYTE *gubCopyBuffer2 = (UBYTE *)(DataZoneBase+0x100000); 		// 1M for ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING
ULLONG *guoRSFrameBits = (ULLONG*)(DataZoneBase+0x200000) ; 	// 1MB, Must align 4KB
UBYTE *gubflaBuffer = (UBYTE*)(DataZoneBase+0x300000);			// 16KB * 2ch * 4plane = 128K
// ======================================================data zone & DDR=======================================================
UBYTE *gubSDRCache = (UBYTE *)(DataZoneBase+0x320000);	//4MB
// ======================================================SRAM & DCCM=========================================================
UBYTE *gubBuffer2 = (UBYTE *)(0x2C000000);   //256KB
volatile INT_t *gubINTTable = (volatile INT_t*)Quue_Addr_FLH;
volatile ZINFO_t *gubZINFO = (volatile ZINFO_t*)Quue_Addr_SATA_Zinfo;
volatile SATA_MSG_TYPE3 *gulSATA_MSG_TYPE3 = (volatile SATA_MSG_TYPE3*)(Quue_Addr_SATA_Zinfo + 64);

U32 const gulFlashIP_RegBase[5] =	{	(Flash_Reg_Base), 			//IP0
							(Flash_Reg_Base+512),
							(Flash_Reg_Base+(512*2)), 	//All IP
							(Flash_Reg_Base+(512*3)),	//TOP
							(Flash_Reg_Base+(512*4))		//DLL
						};

U32 const gulFSABase[MAX_CHANNEL] = { (IRAM_BASE+FSA_BASE_CH0_0), (IRAM_BASE+FSA_BASE_CH1_0) };
U32 const gulMTQAdr[MT_QUEUE_NUM] = {MT_QUEUE_ADR_0, MT_QUEUE_ADR_1, MT_QUEUE_ADR_2, MT_QUEUE_ADR_3, MT_QUEUE_ADR_4, MT_QUEUE_ADR_5, MT_QUEUE_ADR_6, MT_QUEUE_ADR_7};
// ======================================================MTQ================================================================
MTQ * gMTQ = (MTQ *)(IRAM_BASE+MT_ADR_BASE);
#endif

#if ENABLE_RS_NORMAL_MODE
// For RS Spare Correct Total 2KB
#define CorrectRSSpareStart (TableZoneBase+0x192C000) // 0x6199C000
#define CorrectRSSpareSize (64 * RS_NORMAL_MODE_MAX_FPUCOR_SIZE) //64byte * 6
#define RSFrameSpareDecodeStart (CorrectRSSpareStart+CorrectRSSpareSize)
#define RSFrameSpareDecodeSize 128 // 64byte * 2 for ByPass Read
#define VerifyDecodeRSSpareStart (RSFrameSpareDecodeStart+RSFrameSpareDecodeSize)
#define VerifyDecodeRSSpareSize 64

#define RSFrameSpareDebugStart (VerifyDecodeRSSpareStart+VerifyDecodeRSSpareSize+192)
#define RSFrameSpareDebugStart2 (RSFrameSpareDebugStart+512)

// For RS Data Correct
#define CorrectRSDataStart (DataZoneBase+0x300000)
#define CorrectRSDataSize (16384 * RS_NORMAL_MODE_MAX_FPUCOR_SIZE) //16KB * 6
#define RSFrameDataDecodeStart (CorrectRSDataStart+CorrectRSDataSize)
#define RSFrameDataDecodeSize 32768 // 16KB * 2 for ByPass Read
#define VerifyDecodeRSDataStart (RSFrameDataDecodeStart+RSFrameDataDecodeSize)
#define VerifyDecodeRSDataSize 16384
#else
// For RS Spare Correct Total 2KB
#define CorrectRSSpareStart (TableZoneBase+0x192C000) // 0x6199C000
#define CorrectRSSpareSize (64 * 2) //64byte * 8
#define VerifyDecodeRSSpareStart (CorrectRSSpareStart+CorrectRSSpareSize)
#define VerifyDecodeRSSpareSize 64
#define RSFrameSpareDecodeStart (VerifyDecodeRSSpareStart+VerifyDecodeRSSpareSize)
#define RSFrameSpareDecodeSize 128 // 64byte * 2 for ByPass Read
#define RSFrameSpareDebugStart (RSFrameSpareDecodeStart+RSFrameSpareDecodeSize+192)
#define RSFrameSpareDebugStart2 (RSFrameSpareDebugStart+512)
// For RS Data Correct
#define CorrectRSDataStart (DataZoneBase+0x300000)
#define CorrectRSDataSize (16384 * 2) //16KB * 8
#define VerifyDecodeRSDataStart (CorrectRSDataStart+CorrectRSDataSize)
#define VerifyDecodeRSDataSize 16384
#define RSFrameDataDecodeStart (VerifyDecodeRSDataStart+VerifyDecodeRSDataSize)
#define RSFrameDataDecodeSize 32768 // 16KB * 2 for ByPass Read
#endif

////////////Lin//////
//_FTL_DECL_DCODE UBYTE RDT_preformat_flag;
//_FTL_DECL_DCODE UBYTE start_end_mark;
/*
_FTL_DECL_DCODE unsigned char gubENABLE_FLASH_TASK ;
_FTL_DECL_DCODE UBYTE gubFlashModeSet;
_FTL_DECL_DCODE UBYTE gubFlashClockSet;
_FTL_DECL_DCODE U32 gulSDRLoopTimes;
_FTL_DECL_DCODE U32 gulFlaLoopTimes;
_FTL_DECL_DCODE U32 gulBasiCMDLoopNumber;
_FTL_DECL_DCODE U32 gulFlaWaitTime;
_FTL_DECL_DCODE UBYTE gubFlaTestType;
_FTL_DECL_DCODE U32 guwECCthreshold;
_FTL_DECL_DCODE unsigned short guwENABLE_ONLY_TEST_FIRST_LAST_BLKS_OF_CE;
_FTL_DECL_DCODE unsigned char gubENABLE_RDT_REVERSE_ONOFFTEST;
_FTL_DECL_DCODE UBYTE gubFlaTestType;
_FTL_DECL_DCODE unsigned char gubENABLE_SDR_TASK;
*/
//extern U32 const gulFlashIP_RegBase[5];
////Lin////////////

#if(RDT)
_FTL_DECL_DCODE UWORD guwsatalink_retry;
extern UBYTE badblocklog_pages;
extern UBYTE SDRlog_pages;
extern UBYTE gubstart_end_mark;
extern UBYTE gubBasiCMDLoopNumber;
extern UBYTE gubFlaLoopTimes;
extern U32 gulstart_wait_time;
//////preformat///////////////
#endif

///////////Pseudo CE///////////////
_FTL_DECL UBYTE gubExpandCE;
_FTL_DECL UBYTE gubExpandCE_Ratio;
_FTL_DECL UBYTE gubExpandCE_initial;
_FTL_DECL UWORD guwRUTSizePerCE;

// CE Deocder
_FTL_DECL UBYTE gubCH_CEMappingToPCE[MAX_CHANNEL][MAX_CE_PER_CH];
_FTL_DECL UBYTE gubUFSRuleMap[MAX_CHANNEL][MAX_CE_PER_CH];
_FTL_DECL UBYTE gubGPIO_CE_Decoder;
_FTL_DECL UBYTE gubUFSRule;
_FTL_DECL UBYTE gubMAX_SCAN_CE;
_FTL_DECL UBYTE gubMAX_SCAN_CE_PER_CH;

// Fail Handle Variable==========================================================================
_FTL_BURNER_DCODE UBYTE gubAbortDoneDepth;
_FTL_BURNER_DCODE UBYTE gubAbortDone_Case;
_FTL_BURNER_DCODE UBYTE gubFPUTimeout;
#if ENABLE_DEBUG_RS_VERIFY_DATA
_FTL_BURNER_DCODE U32 gulRS_SpareFail_LDPC_TrappingSet_DebugCnt;
_FTL_BURNER_DCODE UBYTE gubRSRecoverSpareFail;
#endif
_FTL_BURNER_DCODE UBYTE gubSandiskRetryType;//for sandisk
_FTL_BURNER_DCODE UBYTE gubIsHBRetryLastNormalRead;
_FTL_BURNER_DCODE UBYTE gubDebugHBRetryForcePassPhase; // Valid when open ENABLE_DEBUG_HB_RETRY_ALL
_FTL_DECL_DCODE UBYTE gubNotEncodeDone;
_FTL_DECL_DCODE UWORD guwRSPlaneIndex;
_FTL_DECL_DCODE UBYTE gubRSCorrectPageIndex;
_FTL_DECL_DCODE UBYTE gubRSNormalModeCorrectDataBefore;
_FTL_DECL_DCODE UBYTE gubEnterftlCopyUnit;
_FTL_DECL_DCODE UBYTE gubRSGroupTagFailMap[MAX_RS_GROUP_TAG_FAIL_MAP_LENGTH];
_FTL_DECL_DCODE UBYTE gubEnterRS;
_FTL_DECL_DCODE UBYTE gubEnterSB;
_FTL_DECL_DCODE UBYTE gubEnterHB;
_FTL_DECL_DCODE UBYTE gubEnterRSNormalMode;
_FTL_DECL_DCODE UBYTE gubRS_FPU_ReadGetLDPC_BCH;
_FTL_DECL_DCODE UBYTE gubRSReadingParityPage;
_FTL_DECL_DCODE UBYTE gubFPUCORClearBufMask;
_FTL_BURNER_DCODE UBYTE gubRSDebug_NoGoldenToVerifyDATA;
_FTL_BURNER_DCODE U32 gulRSDebug_NoGoldenToVerifyDATA_DebugCount;
_FTL_BURNER_DCODE U32 gulRSEncodeStartPlaneNo[TOTAL_PARITY_GROUP_NUM];
_FTL_DECL_DCODE RS_FPU_FAIL_CONDITION_t gsTableRSErrorFrameBitMap;
_FTL_DECL_DCODE UBYTE gubRSErrorNum;
_FTL_DECL_DCODE UBYTE gubOriRSErrorNum;
_FTL_DECL_DCODE U32 gulRSDMAAddr;
_FTL_DECL_DCODE UWORD guwRSErrorPlaneIndex[RS_MAX_ERROR_COUNT];
_FTL_DECL_DCODE UBYTE gubRSErrorPlaneUNCFrameMap[RS_MAX_ERROR_COUNT];
_FTL_DECL_DCODE UBYTE gubEncodedDPageMap[((VAR_PARITY_DATA_PAGE_NUM + 7) >> 3)];
#if ENABLE_DEBUG_RS_NROMAL_MODE_MULTI_ERROR
_FTL_DECL_DCODE UWORD guwDebugRSNormalModeErrorPlaneNo[RS_MAX_ERROR_COUNT];
#endif
_FTL_DECL_DCODE U32 gulGetUNCFrameMapInMT;
_FTL_DECL_DCODE U32 gulReadFail_4KMAP;
_FTL_DECL_DCODE U32 gulRSReadFail_4KMAP;
_FTL_DECL U32 gulReadUNCWaitCount;
_FTL_DECL UBYTE gubUncPtr;

#if MicronFlashOnly
_FTL_BURNER_DCODE U32 gulSATAUncAddr[FLA_READUNC_MAXLOG_CNT];
#else
_FTL_DECL_DCODE U32 gulSATAUncAddr[FLA_READUNC_MAXLOG_CNT];
#endif

#if (!BURNER)
_FTL_DECL_DCODE RS_PARAM RS;
_FTL_DECL_DCODE RS_FLAFAIL_PARAM RS_FLAFAIL;
_FTL_DECL_DCODE RS_NORMAL_MODE_COR_BETTER_MAP_t gsRSNormalModeCorBetterMap;
#endif


_FTL_DECL_DCODE UBYTE gubWP_DEBUG;
_FTL_DECL_DCODE UBYTE gubRetryGenFailPhase;
_FTL_DECL_DCODE UBYTE gubForceGenCRC32;

_FTL_DECL_DCODE U32 gulWP_DEBUG_HWgenFail;
_FTL_DECL_DCODE U32 gulWP_DEBUG_sb_TARPINGset;

// debug
_FTL_BURNER_DCODE UBYTE gubDebugMaxRSErrorNum;
//

_FTL_DECL_DCODE UBYTE gubUNCandLCAfailCondition; // temp solution
_FTL_DECL_DCODE UBYTE gubEnterReadFailRetryCondition; // 1: UNC;  4: CRC32;  5: LCA;  12: FWsetting;  100: DQS mismatch, 101: erase page
_FTL_DECL_DCODE UBYTE gubEnterReadFailRetryCon_Debug;
_FTL_DECL_DCODE UBYTE gubRSclearMTQRebuildFQ;

_FTL_DECL_DCODE UBYTE gubHardBitRetryGroupNumber[2]; // [0] put FastPage data, [1] put Non-FastPage data
_FTL_DECL_DCODE UBYTE gubHardBitRetryBytePerGroup[2]; // [0] put FastPage data, [1] put Non-FastPage data
#if TLC_BICS2 || YMTC
#if BURNER
_FTL_BURNER_DCODE UBYTE gubHardBitRetryParameterTable[850]; // BICs4 256G  : 822B
#else
_FTL_DECL_DCODE UBYTE gubHardBitRetryParameterTable[850]; // BICs4 256G  : 822B
#endif
#else
_FTL_DECL_DCODE UBYTE gubHardBitRetryParameterTable[467]; // MLC: 70B,   TLC: 317B,   BiCs: 255B,   NewFlash: 420B , BICs3: 467B
#endif
_FTL_DECL_DCODE UBYTE gubHardBitRetryD2D3GroupBase;
#if TLC
_FTL_DECL_DCODE UWORD guwRSDataPlaneNum;
#endif

#if B0KB
#define HB_RETRY_Q_NUM		7
#else
#define HB_RETRY_Q_NUM		4
#endif
_FTL_BURNER_DCODE HBRetryQueueInfo_t gHBRetryQI[HB_RETRY_Q_NUM];
_FTL_BURNER_DCODE UWORD guwSoftBitDistributeData[24];

#if TLC_NES_TEST_SB_FLOW
_FTL_BURNER_DCODE UBYTE gubNES_C9_Data[68];
_FTL_BURNER_DCODE UBYTE gubNESSBFrameIndex;
_FTL_DECL UBYTE gubNESReadIBF; // for Nes read IBF
_FTL_BURNER_DCODE UWORD guwNESDataLen; // for Nes read IBF
_FTL_BURNER_DCODE UWORD guwNESFQIndex; // for Nes read IBF
#endif
#if TLC_BICS2
_FTL_BURNER_DCODE UBYTE gubSBHighTemperCondition;
_FTL_BURNER_DCODE UBYTE gubSBHighTShiftValue[7];
#endif

#if TLC
#if (!RDT)
_FTL_DECL_DCODE U32 gulIRAM_L4K_TABLE_OFFSET_HR;
_FTL_DECL_DCODE UBYTE gubStartFrameNo_HR;
_FTL_DECL_DCODE UBYTE gubFrameNum_HR;
_FTL_DECL_DCODE UBYTE gubIsFastPage_HR;
_FTL_DECL_DCODE UBYTE gubDepth_HR;
_FTL_DECL_DCODE UBYTE gubChannel_HR;
_FTL_DECL_DCODE UBYTE gubALUIndex_HR;
_FTL_DECL_DCODE UBYTE gubD3LMU_HR;
_FTL_DECL_DCODE UBYTE gubD3WriteMode_HR;
_FTL_DECL_DCODE U32 gulReadFail_4KMAP_HR;
_FTL_DECL_DCODE UWORD guwFJobInfo_HR;
_FTL_DECL_DCODE UBYTE gubFQLinkIndex_HR;
_FTL_DECL_DCODE UBYTE gubCEIndex_HR;
#endif
#else
_FTL_DECL_DCODE U32 gulFQEntry_HR;
#endif
_FTL_DECL_DCODE UBYTE btIsVerifyFlushD1Data;
_FTL_DECL_DCODE UBYTE gubBicsRetryBuffer[512];
_FTL_BURNER_DCODE UBYTE gubD3RetryCount_HR;

#if ENABLE_RESET_RETRY_LEVEL
_FTL_DECL U32 gulKeepRetryLevelCE_BITMAP;
#if (!TLC)
_FTL_DECL_DCODE UBYTE gub26CMDflag;
_FTL_DECL_DCODE U32 gulMLCHBPassCnt;
_FTL_DECL_DCODE U32 gul26CMDStartTime;
#endif
#endif
// End of Fail Handle Variable==========================================================================

// 不常用放DRAM==================================================================================
_FTL_DECL_DCODE UBYTE gubSaveVTSerialNumber;
_FTL_DECL_DCODE UBYTE gubLastFWVer[8];
_FTL_DECL_DCODE UBYTE gubE3DFailFlag;
_FTL_DECL_DCODE UBYTE gubIcdeDone;
_FTL_DECL_DCODE UBYTE gubDPSTableValid;
_FTL_DECL_DCODE UBYTE gubGPIOEraseDoing;
_FTL_DECL_DCODE UBYTE gubDPSEnableUseTimeOut;
_FTL_DECL_DCODE UWORD guwDPSWaitTimeOutMiliSec;
_FTL_DECL_DCODE UBYTE gub_ENABLE_DPS;
_FTL_DECL_DCODE UBYTE gubACTimingTableEnable;
_FTL_DECL_DCODE UBYTE gubDoubleVT;
#if(!BURNER)&&(ENABLE_DPS)
_FTL_DECL_DCODE UBYTE gubDPSTable[2048];
#endif
_FTL_DECL_DCODE UBYTE gubACTimingTable[32];
_FTL_DECL_DCODE UBYTE gubACTimingClock;
_FTL_DECL_DCODE volatile UBYTE gubNormalFWMode;
_FTL_DECL_DCODE UBYTE gubFlashODT;
_FTL_DECL_DCODE UBYTE gubFlashDrive;
_FTL_DECL_DCODE UBYTE gubScanRound;
_FTL_DECL_DCODE UBYTE gubScanInterval;
_FTL_DECL_DCODE UWORD guwScanMaxLDPCErrorBit;
_FTL_DECL_DCODE DRIVING_SETTING gDrivingSetting;
_FTL_DECL_DCODE ODT_SETTING gODTSetting;
_FTL_DECL_DCODE SDLL_DELAY_SETTING gSDLL_OFFSET_Setting;
_FTL_DECL_DCODE UBYTE gubMaxECCError;
_FTL_DECL_DCODE UWORD guwDieBlockShift;
_FTL_DECL_DCODE UBYTE gubDieBlockShiftLog;
_FTL_DECL_DCODE UBYTE gubDieNumber;
_FTL_DECL_DCODE UBYTE gubDieNumberMask;
_FTL_DECL_DCODE UBYTE gubDieNumberLog;
_FTL_DECL_DCODE UBYTE gubSelDieNumber;
_FTL_DECL_DCODE UBYTE gubSDRSizeMB;   // BIT0:2MB, BIT1:4MB, BIT2:8MB, BIT3:16MB, BIT4:32MB, BIT5:64MB, BIT6:32MBF, BIT7:32MBFT
_FTL_DECL_DCODE UBYTE gubFlashInitialized;
_FTL_DECL_DCODE UBYTE gubParaInitialized;
_FTL_DECL_DCODE UBYTE gubPowerCycling;
_FTL_DECL_DCODE UBYTE gubPowerCycleNeedGenSearchLink;
_FTL_DECL_DCODE volatile UBYTE gubTest;
_FTL_DECL_DCODE volatile UBYTE gubPreformat;
#if EnableVenderCmdHistory
_FTL_DECL_DCODE volatile UBYTE gubVendeCmdHistory[MaxVenderCmdLog];
_FTL_DECL_DCODE volatile UBYTE gubVendeCmdHistoryPtr;
#endif
_FTL_DECL_DCODE volatile UBYTE gubPreformatTurbo;
_FTL_DECL_DCODE volatile UBYTE gubPreformatunfinish;
_FTL_DECL_DCODE UBYTE gubTwoPlaneBit;
_FTL_DECL_DCODE UBYTE gub4KSLC;
_FTL_DECL_DCODE UBYTE gubChannel;
_FTL_DECL_DCODE UWORD guwDMAFrameSize;

_FTL_DECL_DCODE UBYTE gubPasswordHit;
_FTL_DECL_DCODE UBYTE gubLBA48BitCmdSupport;
_FTL_DECL_DCODE UBYTE gubWriteCacheSupport;
_FTL_DECL_DCODE UBYTE gubWriteCacheEnable;
_FTL_DECL_DCODE UBYTE gubACTPMEnable;
_FTL_DECL_DCODE UBYTE gubACTPMValue;
_FTL_DECL_DCODE UBYTE gubStandyByTimerState;
_FTL_DECL_DCODE U32 gulSMARTSelfTestCurrentLBA;
_FTL_DECL_DCODE UBYTE gubSMARTSelfTestRealScan;
_FTL_DECL_DCODE U32 gulStandByJudgeInSecond;
_FTL_DECL_DCODE ULLONG guoCMDInStartTime;
_FTL_DECL_DCODE U32 gulMPRWCMDInStartTime;
_FTL_DECL_DCODE U32 gulSanitizeCurrentLBA;
_FTL_DECL_DCODE U32 gulSmartNeedScanTimeInterval;
_FTL_DECL U32 gulSmartScanStartTime;
_FTL_DECL U32 gulSmartCurrentTime;
_FTL_DECL_DCODE U32 gulSmartOfflineUNCCount;

_FTL_DECL_DCODE UWORD guwWearLevelingRange_D1;
_FTL_DECL_DCODE UWORD guwWearLevelingRange_D3;
_FTL_DECL_DCODE UWORD guwWearLevelingRange_D3_High;
_FTL_DECL_DCODE UBYTE gubFlashClock_Scan;
_FTL_DECL_DCODE UBYTE gubFlashMode_Scan;
_FTL_DECL_DCODE UBYTE gubThermalSensorMode;
_FTL_DECL_DCODE UBYTE gubFAKETemperatureValue;
_FTL_DECL_DCODE UBYTE gubILMode;
_FTL_DECL_DCODE UBYTE gubUltraMLCEnable;
_FTL_DECL_DCODE UBYTE gubDEVSLPSupport;
_FTL_DECL_DCODE UBYTE gubHIPMSupport;
_FTL_DECL_DCODE UBYTE gubDIPMSupport;
_FTL_DECL_DCODE UBYTE gubAPSSupport;
_FTL_DECL_DCODE UWORD guwPMASetting;
_FTL_DECL_DCODE UBYTE gubWWNSupport;
_FTL_DECL_DCODE UBYTE gubDCOSupport;
_FTL_DECL_DCODE UBYTE gubHPASupport;
_FTL_DECL_DCODE UBYTE gubAMAXSupport;
_FTL_DECL_DCODE UBYTE gubSanitizeSupport;
_FTL_DECL_DCODE UBYTE gubWUNCSupport;
_FTL_DECL_DCODE UBYTE gubZeroSupport;
_FTL_DECL_DCODE UBYTE gubSecuritySupport;
_FTL_DECL_DCODE UBYTE gubSecurityEraseNormalNeedDoRealErase;
_FTL_DECL_DCODE UBYTE gubDLMCSupport;
_FTL_DECL_DCODE UBYTE gubTrimSupport;
_FTL_DECL_DCODE UWORD guwWWN[4]; //WWN (World Wide Name)
_FTL_DECL_DCODE UBYTE gubFormFactor;
_FTL_DECL UBYTE gubCustomer;
_FTL_DECL_DCODE UBYTE gubCustomersCustomerID;
_FTL_DECL_DCODE U32 gulAPKeyPassword;
_FTL_DECL_DCODE UBYTE gubRTD_enable;
_FTL_DECL_DCODE UBYTE gubPlanesPerPageDefault;
_FTL_DECL_DCODE XFERMODE  gXferMode;
_FTL_DECL_DCODE FLASH_PARAMETER_PAGE gulParamPageInfo;
#if (MicronFlashOnly && BURNER)
_FTL_BURNER_DCODE UBYTE gubParameterPage[512];
#else
_FTL_DECL_DCODE UBYTE gubParameterPage[512];
#endif
_FTL_DECL_DCODE UBYTE gubDefaultDMAType;
_FTL_DECL_DCODE UBYTE gubDefaultDMASelectMode[2];
_FTL_DECL_DCODE UBYTE gubDLMCReturnDummyAbort;
_FTL_DECL_DCODE UBYTE gubSMARTExecuteOfflineEnable;
_FTL_DECL_DCODE UBYTE gubWriteProtectWriteDummy;
_FTL_DECL_DCODE UBYTE gubSMARTExtendScanAllEnable;
_FTL_DECL_DCODE UBYTE gubCustomerGPIOSelectMode;
_FTL_DECL_DCODE UBYTE gubJTagWakeUp;
_FTL_DECL_DCODE UBYTE gubQE_FromSlumber;
_FTL_DECL_DCODE UBYTE gubQuickEraseWaitTime;
_FTL_DECL_DCODE UBYTE gubCustomerGPIO3SelectMode;
_FTL_DECL_DCODE UBYTE gubCustomerGPIOEraseEnable;
//_FTL_DECL_DCODE ULLONG guoTimeInterval;
_FTL_DECL_DCODE UBYTE gubCustomerGPIO3EraseEnable;
_FTL_DECL UBYTE gubEraseLightTwinkle;
_FTL_DECL_DCODE UBYTE gublighton;
_FTL_DECL_DCODE UBYTE gubGPIOEdgeMode;
_FTL_DECL_DCODE UBYTE gubGPIO3EdgeMode;
_FTL_DECL_DCODE UBYTE gubVenderLock;
_FTL_DECL_DCODE UBYTE gubRealLock; /// flag for Vender_UnlockCmd()
_FTL_DECL_DCODE UBYTE gubCheckLockCnt;
_FTL_DECL_DCODE UBYTE gubHVFlash_RDTSetting; //Bit_0=1:勾HV Flash, Bit_1=1:Translate IB to DBT, Bit_2=1:On Board Sorting Before RDT
_FTL_DECL_DCODE UBYTE gubEraseMode_RDTSetting;
_FTL_DECL_DCODE UBYTE gubHVFlash_FWSetting; //Bit_0=1:勾HV Flash, Bit_1=1:Translate IB to DBT, Bit_2=1:On Board Sorting Before RDT
_FTL_DECL_DCODE UBYTE gubEraseMode_FWSetting; //
_FTL_DECL_DCODE UBYTE gubRMApreformat;
_FTL_DECL_DCODE UBYTE gubIndustrial_Specification;//工規sample
_FTL_DECL_DCODE UBYTE gubTemperatureMin;//spec溫度下限
_FTL_DECL_DCODE UBYTE gubTemperatureMax;//spec溫度上限

#if(!BURNER)
_FTL_DECL_DCODE UWORD guwSMARTOfflineTimeSecond;
_FTL_DECL_DCODE UBYTE gubSMARTShortSelfTestTimeMin;
_FTL_DECL_DCODE UWORD guwSMARTExtendedSelfTestTimeMin;
_FTL_DECL_DCODE UBYTE gubSMARTConveyanceSelfTestTimeMin;
_FTL_DECL_DCODE UBYTE gubSMARTBufferTimeSecond;
#else
_FTL_BURNER_DCODE UWORD guwSMARTOfflineTimeSecond;
_FTL_BURNER_DCODE UBYTE gubSMARTShortSelfTestTimeMin;
_FTL_BURNER_DCODE UWORD guwSMARTExtendedSelfTestTimeMin;
_FTL_BURNER_DCODE UBYTE gubSMARTConveyanceSelfTestTimeMin;
_FTL_BURNER_DCODE UBYTE gubSMARTBufferTimeSecond;
#endif

#if(!BURNER)
//_FTL_DECL_DCODE ULLONG guoCustomerGPIOTimer;
_FTL_DECL_DCODE ULLONG guoCustomerGPIO3Timer;
#if (BICS2_GC_TO_D3)
_FTL_DECL_DCODE UBYTE gubSpareValidTable[SpareValidTableSize];//BiCs CopyData() using 3M copybuffer, 1 bit for 1 sector,  1: sparevalid, 0: spareinvalid
#else
_FTL_DECL_DCODE UBYTE gubSpareValidTable[512];	// for 2M copybuffer, 1 bit for 1 sector,  1: sparevalid, 0: spareinvalid
#endif

#if (ENABLE_TABLEUNIT_PROGRAMFAIL_HANDLING)
_FTL_DECL_DCODE	 U32 gul3ByteFW[64];	// 1M copybuffer, for each read FQ, 1M/16K = 64
#else
_FTL_DECL_DCODE U32 gul3ByteFW[128];	// 2M copybuffer, for each read FQ, 2M/16K = 128
#endif
#endif
_FTL_DECL_DCODE U32 gulNeededUnitNum;
_FTL_DECL_DCODE UWORD guwExtendBlocks;
_FTL_DECL_DCODE UWORD guwBadBlockBoundary;
_FTL_BURNER_DCODE UWORD guwInitialSpareBlocks;
_FTL_DECL_DCODE U32 gulMaxPECycle_D1;
_FTL_DECL_DCODE U32 gulMaxPECycle_D3;
_FTL_DECL_DCODE UBYTE gubDieSize;
_FTL_DECL_DCODE U32 gulFW_START_Time;
_FTL_DECL_DCODE UBYTE gubIDW129_159Enable;
_FTL_DECL_DCODE UWORD guwIDW129_159[31];
_FTL_DECL_DCODE U32 gulSmartStatusRuleMap;
_FTL_DECL_DCODE UBYTE gubMultiPlaneNum;
_FTL_DECL_DCODE UBYTE gubA2CmdEnable;
_FTL_DECL UBYTE gubNoPWICEnable;  // Notice:this variable has to do with DDR/SDR retention, so must put in DCCM
_FTL_DECL_DCODE UBYTE gubSSC_Enable;
_FTL_DECL_DCODE UBYTE gubCENumber_Preformat;
_FTL_DECL_DCODE UBYTE gubCENumber_MP;
_FTL_DECL_DCODE UBYTE gubScanPreformatCENumber;
_FTL_DECL UBYTE gubResetCountDuringCmd;
_FTL_DECL UBYTE gubResetFlag;
// BG
_FTL_DECL_DCODE UBYTE gubBG_Copy_State;
_FTL_DECL_DCODE UBYTE gubFTL_Stop_BGCopy;
_FTL_DECL_DCODE UBYTE gubBG_BuildGC_Index;
_FTL_DECL_DCODE UBYTE gubBG_GC_R_4kIndex;
_FTL_DECL_DCODE UBYTE gubBG_GC_W_4kIndex;
_FTL_DECL_DCODE UBYTE gubBG_Victim_Index;
_FTL_DECL_DCODE UWORD guwBG_R_BufferIndex;
_FTL_DECL_DCODE UWORD guwBG_W_BufferIndex;
_FTL_DECL_DCODE UWORD guwBG_Victim_Unit;
_FTL_DECL_DCODE U32 gulBG_Scan_P2L_Index;
_FTL_DECL_DCODE U32 gulBG_P2L_L4K_NUM;
_FTL_DECL_DCODE U32 gulBG_Debug_VC;
_FTL_DECL_DCODE U32 gulBG_BackUp_VC;
_FTL_DECL_DCODE U32 gulBG_ZByteNumSumInThisUnit;
_FTL_DECL_DCODE UWORD guwBG_RetentionRead_L2PGroup;
_FTL_DECL_DCODE UWORD guwBG_RetentionRead_4kOffset;
_FTL_BURNER_DCODE U32 gulBG_RetentionRead_TotalEntry;
#if MicronFlashOnly
_FTL_BURNER_DCODE U32 gulFailLBAforLog[FLA_READUNC_MAXLOG_CNT];
#else
_FTL_DECL_DCODE U32 gulFailLBAforLog[FLA_READUNC_MAXLOG_CNT];
#endif

#if(!RDT)
_FTL_DECL_DCODE UBYTE gubBG_BuildGC_Not_Full_Index[8];
_FTL_DECL_DCODE UBYTE gubBG_Copy_SectorPerPlane[8];
_FTL_DECL_DCODE UBYTE gubPseudo_BG_Doing;
_FTL_DECL_DCODE UBYTE gubPseudo_BG_Stop_Case;
_FTL_DECL_DCODE U32 gulBGSLC_Threshold;
_FTL_DECL_DCODE UBYTE gubBGMLC_Threshold;
_FTL_DECL_DCODE U32 gulBGSLC_StartThreshold;
_FTL_DECL_DCODE U32 gulBGSLC_EndThreshold;
_FTL_DECL_DCODE UBYTE gubBGMLC_StartThreshold;
_FTL_DECL_DCODE UBYTE gubBGMLC_EndThreshold;
_FTL_DECL_DCODE UBYTE gubAdjust_SLCPool_Enable;
_FTL_DECL_DCODE U32 gulAdjust_SLCPool_Size;
//For flush L2P
_FTL_DECL_DCODE UBYTE gubVCZeroTableIndexList[MAX_TABLEUNIT];
_FTL_DECL_DCODE UBYTE gubVCZeroTableIndexListNum;
_FTL_DECL_DCODE UBYTE gubBG_Read_UNC;
_FTL_DECL_DCODE U32 gulBG_ReadFail4k_BitMap; // Total W Buffer 128KB = 32 * 4k
_FTL_DECL_DCODE U32 gulPseudo_BG_Stop_Case_Num[10];

#endif
_FTL_DECL_DCODE U32 gulSLCPoolSize_LastLoad;
_FTL_DECL_DCODE UBYTE gubReMappingCE[MAX_CHANNEL*MAX_CE_PER_CH];
_FTL_DECL_DCODE U32 gulMainTaskSetPowerCycleTimer;

_FTL_DECL_DCODE UBYTE gubSecurityMode;
_FTL_DECL_DCODE UBYTE gubDLMCReboot;
_FTL_DECL_DCODE UBYTE gubScanVTReboot;

//===============================Burner=================================================
_FTL_DECL_DCODE UBYTE gubSupportToggle2; //Burner
_FTL_DECL_DCODE UBYTE gubForceExit;  //Burner
_FTL_DECL_DCODE UBYTE gubRUT_EC_set;		//bit0:EC bit1:RUT
_FTL_DECL_DCODE UBYTE gubInterface;
_FTL_DECL_DCODE UBYTE gubInterleave;

_FTL_DECL_DCODE U32 gulReducedDiskSizeInLBA;
_FTL_DECL_DCODE U32 gulDiskSizeInMB;
_FTL_DECL_DCODE UWORD guw_SDR_window_min;
_FTL_DECL_DCODE UWORD guw_SDR_window_max;

_FTL_DECL unsigned char gubDram_Type;
_FTL_DECL unsigned char gubDram_Size;
//_FTL_DECL unsigned long gulDram_Size;
_FTL_DECL unsigned char gubDram_Bank;
_FTL_DECL unsigned char gub_SDR_Corse;
_FTL_DECL unsigned short guw_SDR_Fine;
_FTL_DECL unsigned long gul_temp3;
_FTL_DECL unsigned long gul_temp1;
_FTL_DECL unsigned long gul_temp2;
_FTL_DECL unsigned char gubHeaderSetFeature;

_FTL_DECL unsigned short guwDQS0_Fine;
_FTL_DECL unsigned short guwDQS1_Fine;
_FTL_DECL unsigned char gubDQS0_G;
_FTL_DECL unsigned char gubDQS1_G;

_FTL_DECL_DCODE U32 gulSectionInformation[3][8];
_FTL_DECL_DCODE U32 gulBurnerSeedOrigin;
_FTL_DECL_DCODE U32 gulCurrentSeed;
_FTL_DECL_DCODE U32 guloffset;
_FTL_DECL_DCODE UBYTE gubSM_CENo;
_FTL_DECL_DCODE UBYTE gubSM_GroupNo;
_FTL_DECL_DCODE UBYTE gubSM_LaterType;

_FTL_DECL_DCODE UWORD guwCodeSectionStart[8];
_FTL_DECL_DCODE UBYTE gubindex;
_FTL_DECL_DCODE UBYTE gubRDTDone;          //Bit0:checked Bit1:found

_FTL_DECL_DCODE U32 gulErrorInfo;
_FTL_DECL_DCODE UWORD guwErrorID;
//_FTL_DECL_DCODE U32 gulFlashMode_FailMap;
_FTL_DECL_DCODE ULLONG guoCE_FailMap;
#if TLC_BICS2
_FTL_DECL_DCODE UWORD guwCE_FailMap2;
#endif

_FTL_DECL_DCODE U32 gulMaxUserUnitNum;
//_FTL_DECL_DCODE UWORD guwUnitsPerGroups[15];	//MaxInterleave*
//_FTL_DECL_DCODE UWORD guwExtendUnitsIndex[15];	//MaxInterleave*

_FTL_DECL_DCODE volatile UBYTE gubReadHBRetry_Enable;

_FTL_DECL_DCODE UBYTE gubFeature1_B0;
_FTL_DECL_DCODE UBYTE gubFeature1_ADR;
_FTL_DECL_DCODE UBYTE gubFeature_B0;
_FTL_DECL_DCODE UBYTE gubFeature_ADR;
_FTL_DECL_DCODE UBYTE gubLDPC_Mode_Test;
_FTL_DECL_DCODE UBYTE gubISPDoing;

_FTL_DECL_DCODE FLASH_PARAMETER_PAGE gulParamPageInfo;

#if PH_SQL_FUNC
_FTL_BURNER_DCODE UBYTE gubIsPHSQLClockSet;
_FTL_BURNER_DCODE UBYTE gubFLHIPNoBusyPin;
_FTL_BURNER_DCODE SQLHeadPara gOperationSetting;
_FTL_BURNER_DCODE UBYTE gubFlashClockMax;
_FTL_BURNER_DCODE UBYTE gubUsedDataSectorsInBuffer;
_FTL_BURNER_DCODE UBYTE gubUsedCmdSectorsInBuffer;
_FTL_BURNER_DCODE UBYTE gubNandVToolAdr[6];
_FTL_BURNER_DCODE U32 gulBusyTimer;
#endif

//DLMC
_FTL_DECL_DCODE UBYTE gubDLMCStep;
_FTL_DECL_DCODE UBYTE gubLastDLMCMode;
_FTL_DECL_DCODE UBYTE gubHeaderTransferSectorCnt;
_FTL_DECL_DCODE U32 gulMicroCodeTransferSectorCnt;
_FTL_DECL_DCODE UBYTE gubInfoBlockTransferSectorCnt;
_FTL_DECL_DCODE UBYTE gubUpdateSysInfoTransferSectorCnt;
_FTL_DECL_DCODE U32 gulMicroCodeBinFileSectors;
_FTL_DECL_DCODE U32 gulBurnerTransferSectorCnt;
_FTL_DECL_DCODE U32 gulBurnerBinFileSectors;
_FTL_DECL_DCODE UBYTE gubDeferredCodeSet[2];
_FTL_DECL_DCODE UBYTE gubDoPreformatAfterDLMC;
_FTL_DECL_DCODE U32 gulUpdateSystemInfoMap;
_FTL_DECL_DCODE UBYTE gubUpdateInfoBlock;
//
_FTL_DECL_DCODE UBYTE gubExpectErasePage;
_FTL_DECL_DCODE UBYTE gubExpectBadkBlock;
_FTL_DECL_DCODE U32 gulDiskSizeInMB;

//RUT
_FTL_DECL_DCODE UBYTE gubFastPageForCopyFromRUT;
_FTL_DECL_DCODE UBYTE gubSpecialUnitReadMoveMap;
_FTL_DECL_DCODE UBYTE gubGenReadFailDebug;
_FTL_DECL_DCODE UBYTE gubGenProgramFailDebug;
_FTL_DECL_DCODE UBYTE gubGenEraseFailDebug;
_FTL_DECL_DCODE UBYTE gubPlaneABDebug;
_FTL_DECL_DCODE UBYTE gubContinueFailCnt;
_FTL_DECL_DCODE UWORD guwRUTLogNum;
_FTL_DECL_DCODE U32 gulPutUserFreeToFailHandleAreaCnt;
_FTL_DECL_DCODE U32 gulRandomValue;
_FTL_DECL_DCODE FTarget_t guwOldRUTTargetTemp;
_FTL_DECL_DCODE UWORD guwModifyListIndex;
_FTL_BURNER_DCODE UWORD guwRefreshDBTPageIndex[32];  /// because DBT has 32 physical page at most

_FTL_BURNER_DCODE UBYTE gubSaveParityCntInSaveHeaderInfo;
_FTL_BURNER_DCODE UWORD  guwTableTargetProgramDonePTR;
_FTL_DECL_DCODE U32 gulDQSREMismatchMap;
_FTL_BURNER_DCODE U32 gulTableProgramFail_BaseAddr[64];	// DCODE 會不會太慢
_FTL_BURNER_DCODE U32 gulTableProgramFail_FW3[64];	// DCODE 會不會太慢
#if(! BURNER)
_FTL_DECL ULLONG guoTableProgramFail_BM;
#else
_FTL_BURNER_DCODE ULLONG guoTableProgramFail_BM;
#endif
_FTL_DECL_DCODE enum enumWriteWaitCnt enumWriteWaitCntMode;

//stack
#if ENABLE_TABLE_VERIFY
_FTL_DECL_DCODE	UWORD guwVerifyInRAMIndex;
_FTL_DECL_DCODE	UWORD guwInRAMIndexVerifyDone;
_FTL_DECL_DCODE	UBYTE gubdoTableGC;
_FTL_DECL_DCODE	UWORD guwVerifyFailCnt;
_FTL_DECL_DCODE	UWORD guwTableProgramE3DCnt;
_FTL_DECL_DCODE	UWORD guwVerifyTableTargetPTR;
_FTL_DECL_DCODE	UWORD guwNeedSavePlaneDoingNum;
#endif

_FTL_DECL_DCODE	UWORD guwP2L_Mark;
_FTL_DECL_DCODE	U32 gulBufferPlaneNumForCopyTable;
_FTL_DECL_DCODE	U32 gulTable_P2LOffset;

_FTL_DECL_DCODE	U32 gulUnitL4KNum;
_FTL_DECL_DCODE	UWORD guwTempRUTGCGRTarget;
_FTL_DECL_DCODE	UWORD guwOldGCGRTarget;
_FTL_DECL_DCODE	UBYTE gubOldGCGRTargetIndex;

_FTL_DECL_DCODE	FTarget_t * guwTarget;
_FTL_DECL_DCODE	UWORD guwNeedToProgramTableTargetPTR;

#if(!BURNER)
#if(!RDT)
_FTL_DECL_DCODE UBYTE gubSwapRUTMode;
_FTL_DECL_DCODE UWORD guwRUTLogIndex;
_FTL_DECL_DCODE UWORD guwRUTSummaryLogNum;
_FTL_DECL_DCODE UWORD guwRUTSummaryLogIndex;
_FTL_DECL_DCODE RUTLog_t gulRUTLog[RUT_LOG_NUM];
_FTL_DECL_DCODE RUTSummaryLog_t guwRUTSummaryLog[RUT_LOGINFO_NUM];
_FTL_DECL_DCODE UBYTE gubVTAreaUnitEraseFailCnt;
_FTL_DECL_DCODE UBYTE gubVTUnitEraseFailCnt;
_FTL_DECL_DCODE UBYTE gubSystemUnitEraseFailCnt;
_FTL_DECL_DCODE UBYTE gubFreeUnitNotEnoughCnt;
_FTL_DECL_DCODE UBYTE gubIsRebuildP2LTable;
//_FTL_DECL_DCODE UBYTE gubEnterSwapRUTCnt;	// 用來控制何時才要調整RUTSummaryLog, 因為可能會進SwapRUT又進SwapRUT, 當為0時才一併調整
_FTL_DECL_DCODE UBYTE gubProgFailAtCopyUnit;
_FTL_DECL_DCODE UBYTE gubUpdateRUTBeforeSaveVT;
_FTL_DECL_DCODE UBYTE gubNeedSaveVTMother;


_FTL_DECL_DCODE UBYTE gubTempRUTD1LinkValue;
_FTL_DECL UWORD guwLastPFTableUnit;
_FTL_DECL UWORD guwLastPFTableTargetPtr;

#if TLCRUT
#if(ENABLE_DEBUG_MARKFAIL_CATEGORY)
_FTL_DECL_DCODE UBYTE gub_Debug_MarkFail_D1Cnt;
_FTL_DECL_DCODE UBYTE gub_Debug_MarkFail_D3Cnt;
#endif
#endif

#endif
#endif
// End of  不常用放DRAM=============================================================================
#if BURNER
_FTL_BURNER_ICODE BufferQueueInfo_t gBQI;
#else
_FTL_DECL BufferQueueInfo_t gBQI;
#endif
#if (DEBUG_ULTRAMLC_POWER_FAIL && ENABLE_PFAIL)
 _FTL_DECL UWORD trace_arr_index;
#endif
_FTL_DECL MTDepth_t gMTDepth;
_FTL_DECL volatile REG_t *gFREG;
_FTL_DECL HostTable_t gSwapHostTable;

_FTL_DECL UBYTE gubFillMLCGRTarget;
_FTL_DECL UBYTE gubGetWriteModeDirectly;
_FTL_DECL UBYTE gubLockReadMode;
_FTL_DECL UBYTE gubMicroCodeTmpSet;
_FTL_DECL UBYTE gubReferenceBit;
_FTL_DECL UWORD guwLockReadNum;
_FTL_DECL UWORD guwLockWriteNum;
_FTL_DECL UWORD guwActiveL2PTableReadPTR;
_FTL_DECL UWORD guwActiveL2PTableWritePTR;
_FTL_DECL UWORD guwWriteModeLastFree;
_FTL_DECL UWORD guwTable_LastEncodePlane;
_FTL_DECL UBYTE gubTableHeadInfoPlaneCnt;
_FTL_DECL UBYTE gubSavingEveryThingBeforeReset;
_FTL_DECL InitState_t gubSSD_Init_State;
_FTL_DECL U32 gulStartClearGRAddress;
_FTL_DECL UBYTE gubFlushMode;
_FTL_DECL UBYTE gubFTL_Idle;
_FTL_DECL U32 gulEnterBG_Time_1;
_FTL_DECL U32 gulEnterBG_Time_2;
_FTL_DECL U32 gulEnterBG_Time_Flush;
_FTL_DECL U32 gulEnterBG_Time_COPY;
_FTL_DECL U32 gulEnterBG_Time_D2H_RST;
_FTL_DECL UBYTE gubBackGroundClean_Enable;
_FTL_DECL UBYTE gubSWIFTFLUSH;
_FTL_DECL UBYTE gubGUARANTEE_FLUSH;
_FTL_DECL UBYTE gubFlushCache_StopBG;
_FTL_DECL UBYTE gubFlushCacheDoing;
_FTL_DECL U32 gulFlushCachePrevGRTablePTR;
#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
_FTL_DECL UBYTE gubReadBunchParityDoing;
_FTL_DECL UBYTE gubBunchRSCorrectFlag;		// 1: invoke Bunch_RS_Recover, 0: invoke RS_Recover
_FTL_DECL U32	gulBunchRSStartPlanePTR;
	#if USE_PBUF1_WHEN_SAVE_BUNCH_PARITY==1
		_FTL_DECL UBYTE gubSaveBunchParityDoing;
	#endif
#if USE_NEW_PARITY_PAGE_CHECKING==1
_FTL_DECL UWORD guwBunchCheckParityIndex;
#endif
#endif
_FTL_DECL UBYTE gubReadFailRetryIsSLC;
_FTL_DECL UBYTE gubStopFTL;
_FTL_DECL UBYTE gubFTL_Only_HandleBQ;
_FTL_DECL UWORD guwDebugXferDataInHitL2P;
_FTL_DECL U32 gulCleanP2LBoundary;
_FTL_DECL U32 gulCleanP2LGroupCount;
_FTL_DECL PowerCycleSetting gubPowerCycleSetting;
_FTL_DECL UBYTE gubSimulatePowerCycleMode;
_FTL_DECL UBYTE gubPowerCycleHandlingCmd;
_FTL_DECL UWORD guwPowerCycleCountLimit;
_FTL_DECL UWORD guwPowerCycleCount;
_FTL_DECL_DCODE U32 gulLastRCmdLBA;
_FTL_DECL_DCODE U32 gulLastWCmdLBA;
_FTL_DECL_DCODE UWORD guwLastRCmdSector;
_FTL_DECL_DCODE UWORD guwLastWCmdSector;
_FTL_DECL FlashType_t guoFlashDefaultType;
_FTL_DECL FlashType_t guoFlashUseType;

_FTL_DECL UBYTE gubSectorsPerPlane;
_FTL_DECL UBYTE gubSectorsPerPlaneMask;
_FTL_DECL UBYTE gubSectorsPerPlaneLog;
_FTL_DECL UBYTE gubSectorsPerBurst;
_FTL_DECL UBYTE gubSectorsPerBurstMask;
_FTL_DECL UBYTE gubSectorsPerBurstLog;
_FTL_DECL UWORD guwSectorsPerPage;
_FTL_DECL UWORD guwSectorsPerPageMask;
_FTL_DECL UBYTE gubSectorsPerPageLog;
_FTL_DECL U32 gulSectorsPerUnit;
_FTL_DECL U32 gulSectorsPerUnitMask;
_FTL_DECL UBYTE gubSectorsPerUnitLog;
_FTL_DECL UBYTE gub4kEntrysPerPlane;
_FTL_DECL UBYTE gub4kEntrysPerPlaneMask;
_FTL_DECL UBYTE gub4kEntrysPerPlaneLog;
_FTL_DECL UBYTE gub4kEntrysPerBurst;
_FTL_DECL UBYTE gub4kEntrysPerBurstMask;
_FTL_DECL UBYTE gub4kEntrysPerBurstLog;
_FTL_DECL UWORD guw4kEntrysPerPage;
_FTL_DECL UWORD guw4kEntrysPerPageMask;
_FTL_DECL UBYTE gub4kEntrysPerPageLog;
_FTL_DECL U32 gul4kEntrysPerUnit;
_FTL_DECL U32 gul4kEntrysPerUnitMask;
_FTL_DECL UBYTE gub4kEntrysPerUnitLog;
_FTL_DECL U32 gulMaxL4kEntrysPerUnit;
_FTL_DECL U32 gulMaxL4kEntrysPerUnitMask;
_FTL_DECL UBYTE gubMaxL4kEntrysPerUnitLog;
_FTL_DECL UBYTE gubPlanesPerBurst;
_FTL_DECL UBYTE gubPlanesPerBurstLog;
_FTL_DECL UBYTE gubPlanesPerBurstMask;
_FTL_DECL UBYTE gubPlanesPerBank;
_FTL_DECL UBYTE gubPlanesPerBankLog;
_FTL_DECL UBYTE gubPlanesPerBankMask;
_FTL_DECL UBYTE gubPlanesPerPage;
_FTL_DECL UBYTE gubPlanesPerPageMask;
_FTL_DECL UBYTE gubPlanesPerPageLog;
_FTL_DECL U32 gulPlanesPerUnit;
_FTL_DECL U32 gulPlanesPerUnitMask;
_FTL_DECL UBYTE gubPlanesPerUnitLog;
_FTL_DECL UBYTE gubBurstsPerBank;
_FTL_DECL UBYTE gubBurstsPerBankMask;
_FTL_DECL UBYTE gubBurstsPerBankLog;
_FTL_DECL UBYTE gubBurstsPerPage;
_FTL_DECL UBYTE gubBurstsPerPageMask;
_FTL_DECL UBYTE gubBurstsPerPageLog;
_FTL_DECL UWORD guwBurstsPerUnit;
_FTL_DECL UWORD guwBurstsPerUnitMask;
_FTL_DECL UBYTE gubBurstsPerUnitLog;
_FTL_DECL UBYTE gubBanksPerPage;
_FTL_DECL UBYTE gubBanksPerPageMask;
_FTL_DECL UBYTE gubBanksPerPageLog;
_FTL_DECL UWORD guwPagesPerUnit;
_FTL_DECL UWORD guwPagesPerUnitMask;
_FTL_DECL UBYTE gubPagesPerUnitLog;

_FTL_DECL UWORD guwFastPagePlanesPerUnit;
_FTL_DECL UBYTE gubFastPagePlanesPerUnitLog;
_FTL_DECL UWORD guwTableP2LBitMapSize;//Alignment 32 Byte For Hard Ware memory Set
_FTL_DECL U32 gulFastPage4kEntrysPerUnit;
_FTL_DECL U32 gulFastPage4kEntrysPerUnitMask;
_FTL_DECL UBYTE gubFastPage4kEntrysPerUnitLog;
_FTL_DECL UWORD guwFastPageBurstsPerUnit;
_FTL_DECL UWORD guwFastPageBurstsPerUnitMask;
_FTL_DECL UWORD guwFastPagePagesPerUnit;
#if TLC
_FTL_DECL U32 gulFastPageSectorsPerUnit;
#endif
_FTL_DECL UBYTE gubMIN_D1_UNIT_NUM_BiCS3;
_FTL_DECL UWORD guwNum_Wordline;
_FTL_DECL UBYTE gubNum_D1_Part;
_FTL_DECL UBYTE gubCEsPerPage;
_FTL_DECL UBYTE gubCEsPerPageMask;
_FTL_DECL UBYTE gubCEsPerPageLog;
_FTL_DECL UBYTE gubTotalGroups;
_FTL_DECL UBYTE gubTotalGroupsMask;
_FTL_DECL UBYTE gubTotalGroupsLog;
_FTL_DECL UBYTE gubSelTotalGroups;
_FTL_DECL UBYTE gubSelTotalGroupsMask;
_FTL_DECL UBYTE gubSelTotalGroupsLog;
_FTL_DECL UBYTE gubFastPageType;
_FTL_DECL UWORD guwTablePlanesPerUnit;
_FTL_DECL U32 gulDataPlanesPerUnit;
_FTL_DECL U32 gulData4kEntrysPerUnit;
_FTL_DECL U32 gulDataSectorsPerUnit;
_FTL_DECL UBYTE gubEntrysInDataInGRTableLog;
_FTL_DECL U32 gulEntrysInDataInGRTable;
_FTL_DECL U32 gulEntrysInDataInGRTableMask;
_FTL_DECL U32 gulMin4kEntrysReserved;
_FTL_DECL U32 gulTrimSizenoAlign;
_FTL_DECL UWORD guw4KEntrysPerTrimbit;
_FTL_DECL UWORD guw4KEntrysPerTrimbitMask;
_FTL_DECL UBYTE gub4KEntrysPerTrimbitLog;
_FTL_DECL UWORD guwTrimBytesPerL2PGroup;
_FTL_DECL UWORD guwTrimBytesPerL2PGroupMask;
_FTL_DECL UBYTE gubTrimBytesPerL2PGroupLog;
_FTL_DECL UBYTE gubTrimTableNotCleaned;
_FTL_DECL U32 gulExcecute4kTrimNum;
_FTL_DECL U32 gulRemainTrimbit;
_FTL_DECL UBYTE gubPIOTirm;
_FTL_DECL UBYTE gubPlanesPerP2LTable;

_FTL_DECL UWORD guwTemperatureDetectCnt;
_FTL_DECL UBYTE gubTemperatureCooling;
_FTL_DECL UBYTE gubDropSpeedTemperature[2];
_FTL_DECL UWORD gulDelayCnt[2];
#if TSB_BICS4_SUPPORT
_FTL_DECL UBYTE gubWriteProtect_NUM;
#endif
#if ENABLE_DEBUG_WAF_KAO
_FTL_DECL U32       gulZipCount[MAX_ZCODE+1];
_FTL_DECL U32		gulGetD3SLCPoolUnitCnt;
_FTL_DECL U32		gulGetD3SLCTableUnitCnt;
_FTL_DECL U32		gulGetD3TLCUnitCnt;
_FTL_DECL U32		gulGetD1SLCUnitCnt;
#endif

_FTL_DECL_DCODE UWORD guwDataInOutThreshold;

//for Read Disturb
_FTL_DECL_DCODE UBYTE gubUnitReadCheckThreshold;//啟動 Unit Read Check的Threshold
_FTL_DECL_DCODE UBYTE gubUnitForceReadMoveThreshold;//啟動 Unit Force Read Move的Threshold
_FTL_DECL_DCODE UWORD guwReadCheckCnt;//有幾個Unit需要執行Read Count
_FTL_DECL_DCODE UBYTE gubReadDisturbMoveLevel;//0:Normal, 1:Reset Read , 2:HB[0], 3:HB[1], N:HB[N-2]
_FTL_DECL_DCODE UBYTE gubReadDisturbCheck;//執行 Read Disturb Check
_FTL_DECL_DCODE UBYTE gubForceReadMove;//一定要在 SB前搬走, 其他狀況Data預設會在SB之後搬走
_FTL_DECL_DCODE UWORD guwReadRetryUnit;
_FTL_DECL_DCODE UBYTE gubReadCheckPageCnt;
_FTL_DECL_DCODE UBYTE gubRetryCount_RD;
_FTL_DECL_DCODE UBYTE gubRetryLimit_RD;
_FTL_DECL_DCODE UBYTE gubRetryFrameNum_RD;
_FTL_DECL_DCODE UBYTE gubRRDIRECTACESS;
_FTL_DECL_DCODE U32   gulGRTargetPTR_RD;
_FTL_DECL_DCODE UWORD guwTableTargetPTR_RD;
_FTL_DECL_DCODE UBYTE gubSwapRUT_RD;
_FTL_DECL_DCODE UBYTE gubRetryOK_RD;
_FTL_DECL_DCODE UBYTE gubRetryAgain_RD;
_FTL_DECL_DCODE U32 gulMaxReadCount;
_FTL_DECL UBYTE gubBusyNow;
#if Read_Chk_ByHostRead
_FTL_DECL_DCODE U32   gulHostReadTH;
#endif


#if(!BURNER)
_FTL_DECL_DCODE UBYTE TSBSerialTable[20];
#endif
_FTL_DECL_DCODE UWORD guwSLCPool_PE_Threshold;

_FTL_DECL U32 gulOperationTime; // ms
_FTL_DECL U32 gulLastWriteTime;
_FTL_DECL U32 gulLastEnterBGTime;
_FTL_DECL WORD guwNCQTimeoutThreshold;
_FTL_DECL WORD guwATATimeoutThreshold;
_FTL_DECL U32 gulFTLStateForTimeout;
_FTL_DECL U32 gulHostCMDTriggerTime;

#if ENABLE_PFAIL
 _FTL_DECL U32 gulPfail_time; /* 10s: false pfail; 100ms: to unlock. */
 _FTL_DECL UBYTE gubPfail_state;
 _FTL_DECL UBYTE gubAbort_CMD;
 #endif
#if ENABLE_ULTRAMLC_PFAIL_SIM
 _FTL_DECL U32 gulLastWriteCmdsCount;
#endif
// PFAIL Lock State
 #define PFAIL_STATE_INIT 0
 #define PFAIL_STATE_PRE_LOCK 1
 #define PFAIL_STATE_LOCKED 2
 #define PFAIL_STATE_PRE_RELEASE 3

// PFAIL Lock Type
#define POWER_FAIL_TQ_LOCK BIT7
#define BG_TQ_LOCK BIT0

// L2P manage
_FTL_DECL UBYTE gubHostTable_GroupNum;
_FTL_DECL U32 gulSLCPoolSize_Sync;
_FTL_DECL UBYTE gubTableUnitNum;
_FTL_DECL U32 gulTotalL2PGroupCnt;
_FTL_DECL U32 gulTotalL2PGroupCntPowerOf2;
_FTL_DECL U32 gulLastSavedTimeInMinutes;
_FTL_DECL U32 gulSleepTimeInMinutes;
_FTL_DECL UWORD  guwReplaceableWriteModeTableCount;
_FTL_DECL UWORD guwL2PEntrysPerL2PGroup;
#if(!RDT)
_FTL_DECL UBYTE gubPartialCleanGRDoing;
_FTL_DECL UBYTE gubPartialCloseTargetDoing;
_FTL_DECL UBYTE gubGC_FreeUnit_Threshold;
_FTL_DECL U32 gulCleanGCGRGroupCount;
_FTL_DECL U32 gulCopyBoundary;
_FTL_DECL UWORD guwPlanesPerGCDisrict;
_FTL_DECL UWORD guwPlanesPerGCDisrictMask;
_FTL_DECL UWORD guwSeparatePartsNum;
_FTL_DECL UBYTE gubSeparatePartsNumLog;
_FTL_DECL UBYTE gubForceReadMoveDoing;
_FTL_DECL UBYTE gubReadDisturbNeedWaitTQDone;
_FTL_DECL UBYTE gubReadDisturbStartPollingCMD;
#if TLC
_FTL_DECL UWORD guwFlushD1SeparatePartsNum;
_FTL_DECL UWORD guwNormalFlushD1PartialPlaneNum;
_FTL_DECL_DCODE UBYTE gubFlushD1VerifyMode;
#endif
_FTL_DECL UWORD guwPartialCleanGRMaxL2PPlaneNum;
_FTL_DECL U32 gulGCTableCount;
#if BICS2_GC_TO_D3
_FTL_DECL U32 gulGCTableRecordStartIndex;
_FTL_DECL U32 gulGCGRSetL2PIgnoreCount;
_FTL_DECL U32 gulGCGRSetL2PIndex;
_FTL_DECL_DCODE UBYTE gubForceD1GC;
#endif
_FTL_DECL UBYTE gubBuildGCTable;
_FTL_DECL UBYTE gubFinishBuildGCTable;
_FTL_DECL UBYTE gubCleanGRinMiddle;
_FTL_DECL UBYTE gubCleanGRInTrimAll;
#if ENABLE_NEW_TRIMALL
_FTL_DECL UBYTE gubTrimAll;
#endif
//For Last Trim Cmd Resolution Issue
_FTL_DECL U32 gulTrimRemainderStartLCA;
_FTL_DECL UWORD guwTrimRemainder4KNum;
_FTL_DECL UWORD guwTrimRemainderSectorCnt;
_FTL_DECL U32 gulGRTemp4KPTR;
_FTL_DECL U32 gulGRMiddleCleanGRL4KPTR;
_FTL_DECL U32 gulActiveGRValid4KCount;
_FTL_DECL UBYTE gubFTLNoWait;

_FTL_DECL U32 gulRandomZcodeZoneBegin;
_FTL_DECL U32 gulRandomZcodeZoneBegin_PhyEntry;
_FTL_DECL U32 gulZoneBBegin;
_FTL_DECL U32 gulZoneBBegin_PhyEntry;
_FTL_DECL UBYTE gubZcodeA;
_FTL_DECL UBYTE gubZcodeB;


_FTL_DECL UWORD guwL2PTableInRAMNumberWhenClassify;
_FTL_DECL U32 gulCleanGRGroupCount;
#if TLC
_FTL_DECL U32 gulLastCleanGRGroupCount;
#endif
_FTL_DECL U32 gulP2LGroupCount;
_FTL_DECL U32 gulGC_SaveL2PGroupNum;
_FTL_DECL U32 gulGC_L2P_512B_BM;
_FTL_DECL U32 gulCleanP2LGroupIndex;
_FTL_DECL UWORD guwGRTargetNum;
_FTL_DECL P2LTable_t *ActiveGRTable;
_FTL_DECL UBYTE gubPartialCleanP2LDoing;
_FTL_DECL UWORD guwCleanGRDebugCount;
_FTL_DECL UBYTE gubCleanGREndIndex;

// L2P manage
_FTL_DECL UBYTE gubGCTableIndexList[MAX_TABLEUNIT/4];
_FTL_DECL UBYTE gubGCTableIndexListNum;

_FTL_DECL U32 gulTableIndex_EC;
_FTL_DECL U32 gulTableIndex_VC;
_FTL_DECL U32 gulTableIndex_HOST;


_FTL_DECL UBYTE gubL2PEntrysPerL2PGroupLog;
_FTL_DECL UBYTE gubL2PEntrysPerL2PGroupQuarterLog;
_FTL_DECL UWORD guwL2PEntrysPerL2PGroupMask;
_FTL_DECL UWORD guwWaitRamToLoadTable;
_FTL_DECL UWORD  guwReplaceableTableCount;

_FTL_DECL ReadBQInformation_t gRBQI[32];
_FTL_DECL UBYTE gubRBQIIndex;
_FTL_DECL UBYTE gubRBQINumber;
_FTL_DECL UBYTE gubRBQILast;


_FTL_DECL UBYTE  gubProgramTable_P2L;
_FTL_DECL UBYTE  gubProgramVCTable;
_FTL_DECL UWORD guwL2PEntrysPerL2PGroupQuarter;
_FTL_DECL U32  	gulProgramUserDataCount;
#if B0KB
_FTL_DECL UBYTE gubNowItsYourTernToLoadDataToThis4kBuffer[32];
_FTL_DECL UBYTE gubUseEach4kBufferInThisOrder[32];
#else
_FTL_DECL UBYTE gubNowItsYourTernToLoadDataToThis4kBuffer[32];
_FTL_DECL UBYTE gubUseEach4kBufferInThisOrder[32];
#endif
#endif
_FTL_DECL UWORD guwSDRBufferPTR;
_FTL_DECL UWORD guwSRamBufferPTR;
_FTL_DECL UWORD guwPreReadStartBufferPTR;
_FTL_DECL UWORD guwPreReadEndBufferPTR;
_FTL_DECL ULLONG guoFWRead4kBufferFlag;//Sign: fla    clear: main
_FTL_DECL UBYTE gubNeedWaitCount;
_FTL_DECL UBYTE gubWaitCountLastBQ;
_FTL_DECL UWORD guwReadBufferPTR;
_FTL_DECL UBYTE gubHandleBQStart;
_FTL_DECL UBYTE gubFQOutOfOrder;
_FTL_DECL UBYTE gubOutOfOrderLastFQ;
_FTL_DECL UBYTE gubWaitingTableBQCnt;


_FTL_DECL UWORD guwActiveTableNum;
_FTL_DECL UWORD guwWriteModeTableNum_Max;
_FTL_DECL UWORD guwWriteModeTableNum_ReadModeStartIndex;
_FTL_DECL UWORD guwReadModeTableNum;
_FTL_DECL UWORD guwWriteModeTableNeedSavePlaneNum;


_FTL_DECL UWORD guwDataInSDR4kNum;
_FTL_DECL UWORD guwSDRFlushStart4kIndex;
_FTL_DECL UWORD guwSDRFlushEnd4kIndex;
_FTL_DECL UBYTE gubDataCacheL2PTablePerBlockEntryNumLog;
_FTL_DECL U32 gulDataCacheL2PTableEntryL2Mask;
_FTL_DECL UWORD guwDataCacheL2PL1TablePerEntry4kNum;
_FTL_DECL UBYTE gubDataCacheL2PL1TablePerEntry4kNumLog;
_FTL_DECL UWORD guwSDRFreeLinkHead;
_FTL_DECL UWORD guwSDRFreeLinkTail;
_FTL_DECL UWORD guwSDRFreeTableBlockNum;
_FTL_DECL UBYTE gubSDRFlushDoing;
_FTL_DECL UWORD guwFlush4kCount;
_FTL_DECL UWORD guwSDRTotalFlush4kNum;

_FTL_DECL UBYTE gubKeepLoadAlignmentBQLink;
_FTL_DECL U32 gulKeepLoadAlignmentBQLBA;
_FTL_DECL U32 gulKeepLoadAlignmentBQBufferIndex;
_FTL_DECL UBYTE gubKeepLoadAlignmentBQNum;

_FTL_DECL UBYTE gubKeepWBQNum;
_FTL_DECL UBYTE gubKeepWBQSectorCnt;
_FTL_DECL UBYTE gubKeepWBQ4KCnt;
_FTL_DECL UBYTE gubFirstKeepBQLink;
_FTL_DECL UBYTE gubFirstLoadAlignmentBQLink;

_FTL_BURNER_DCODE UBYTE gubFQLinkIndex[MAX_LOGICAL_CE * 3 * 4]; //for solving mix-plane overflow in tlc, only using in ftlEraseTarget

//For Write Buffer Cache
_FTL_DECL_DCODE U32 gulWriteBufferVir4KTable[32];
#if B0KB
_FTL_DECL_DCODE UBYTE gubWriteBufferDataMaskTable[32];
_FTL_DECL_DCODE UBYTE gubWriteBufferTableHead[32];
_FTL_DECL_DCODE UBYTE gubWriteBufferTableNext[32];
_FTL_DECL_DCODE U32 gulWriteBufferBitMap;
#else
_FTL_DECL UBYTE gubWriteBufferDataMaskTable[32];
_FTL_DECL UBYTE gubWriteBufferTableHead[32];
_FTL_DECL UBYTE gubWriteBufferTableNext[32];
_FTL_DECL U32 gulWriteBufferBitMap;
#endif

_FTL_DECL UBYTE  gubLZSS_RESOLUTION;
_FTL_DECL UBYTE gubL2P_FORMAT_MODE;
_FTL_DECL UBYTE gubZByteNumBitAddr;
_FTL_DECL UBYTE gubZBytePer4kEntry;
_FTL_DECL U32 gulTable_Invalid;
_FTL_DECL U32 gulTable_Parity;
_FTL_DECL U32 gulTable_Dummy;
_FTL_DECL U32 gulMask_NoValidData;
_FTL_DECL U32 gulTable_Empty;

#if B0KB
_FTL_DECL_DCODE UBYTE gubftlRSFrameReuseCount[TOTAL_PARITY_GROUP_NUM];
_FTL_DECL_DCODE UWORD guwRSFrameDoneCount[TOTAL_PARITY_GROUP_NUM];
#else
_FTL_DECL UBYTE gubftlRSFrameReuseCount[TOTAL_PARITY_GROUP_NUM];
_FTL_DECL UWORD guwRSFrameDoneCount[TOTAL_PARITY_GROUP_NUM];
#endif
_FTL_DECL UWORD guwTableParityPlanesNum;
_FTL_DECL UWORD guwParityPlanesNum;
_FTL_DECL UWORD guwTableParityPlanesNumMax;
_FTL_DECL UWORD guwTableRSParityNumPerUnit;
#if(!RDT)
_FTL_DECL UBYTE gubflaRSFrameReuseCount[TOTAL_PARITY_GROUP_NUM];
_FTL_DECL UBYTE gubTableParityPlanesNumLog;
_FTL_DECL UWORD guwTableP2LPlanePtr;
_FTL_DECL UBYTE gubRSCurrentTag;
_FTL_DECL UBYTE gubRSIdleTag;
_FTL_DECL UBYTE gubRSShiftTagPhase;
_FTL_DECL UBYTE gubRSCurrentTagCopy;
_FTL_DECL UBYTE gubRSIdleTagCopy;
_FTL_DECL UBYTE gubRSShiftTagPhaseCopy;
_FTL_DECL UBYTE gubRSCurrentTagCopyUnit;
_FTL_DECL UBYTE gubRSIdleTagCopyUnit;
_FTL_DECL UBYTE gubRSShiftTagPhaseCopyUnit;
_FTL_DECL UWORD guwRSPlanesPerPage;
_FTL_DECL UBYTE gubRSPlanesPerPageLog;
_FTL_DECL UWORD guwRSPlanesPerPageMask;
_FTL_DECL UWORD guwParityDepth[TOTAL_PARITY_GROUP_NUM] ;

_FTL_DECL UBYTE gubForceProgramRSParity[USERDATA_RS_Group_Num + 1];

#if ENABLE_ULTRAMLC
_FTL_DECL UBYTE gubParityDataPageNum;
_FTL_DECL UBYTE gubRSGroupNum;
#endif
#endif
_FTL_DECL UBYTE gubSkipDataInGRCheck;

_FTL_DECL FlashQueueInfo_t gFQI;       // 以後再橋位置~
_FTL_DECL UBYTE gubFSAIndex[MAX_CHANNEL];

_FTL_DECL ATACONFIG AtaCfg;
_FTL_DECL UBYTE gubOfflineMode;
#if(!(RDT || BURNER))
// #if(MicronFlashOnly && ENABLE_DEBUG_UART && ENABLE_DEBUG_RW)
#if(MicronFlashOnly || TLC_BICS2 || ENABLE_PFAIL)
_FTL_DECL_DCODE DebugCommandLog_t gDebugCommandQueue[BYTE_COMMAND_QUEUE/16];     // 1k
#else
#if ENABLE_DEBUG_REDUCE_VARIABLE_STACK
_FTL_DECL_DCODE DebugCommandLog_t gDebugCommandQueue[BYTE_COMMAND_QUEUE/16];     // 1k or 512b
#else
_FTL_DECL_DCODE DebugCommandLog_t gDebugCommandQueue[BYTE_COMMAND_QUEUE/16];     // 1k or 512b
#endif
#endif
_FTL_DECL UBYTE gubDebugCommandQueueIndex;
#endif
_FTL_DECL UBYTE gubFlashID[8];
_FTL_DECL UBYTE gubCENumber;
_FTL_DECL UBYTE gubMTQTotalPhyCE;
_FTL_DECL UBYTE gubHideCE;
_FTL_DECL U32 gulTotalPhyUnitNum;
_FTL_DECL U32 gulPhyBlocksPerCE;
_FTL_DECL U32 gulTotalUserUnitNum;
_FTL_DECL UBYTE gubDensitysize;
_FTL_DECL UBYTE gubRUTMixPlaneEnable;

_FTL_DECL UBYTE gubMaxPlanesPerP2LTable;
_FTL_DECL UBYTE gubPlanesPerTableP2LTable;
_FTL_DECL UBYTE gubPlanesPerVCTable;
_FTL_DECL UBYTE gubPlanesPerECTable;
_FTL_DECL UBYTE gubPlanesPerSeqTable;
_FTL_DECL UBYTE gubPlanesPerVT;
_FTL_DECL UBYTE gubEnable_VT_Backup;
#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
_FTL_DECL UWORD guwMaxPlanesIncludeParityPerP2LTable;
#endif
_FTL_DECL U32 gulConsiderP2L;
_FTL_DECL U32 guwInOneTimePlanesBoundary;

_FTL_DECL UBYTE gubSystemSetFreeCount;
_FTL_DECL UBYTE gubSystemSetFreeQueueInPTR;
_FTL_DECL UBYTE gubSystemSetFreeQueueOutPTR;
_FTL_DECL UBYTE gubTransformSystemUnitToSystemSet;//1.舊版planeA FW DLMC+Preformat到新版remappingsystem, 2.舊版planeA FW以erase 0x11重開卡至新版remappingsystem會舉這個flag
_FTL_DECL UBYTE gubSystemArea_RemappingRule;		//當此byte = 1 代表remappingtable是按照原本planeA rule建的
_FTL_DECL UBYTE gubTranslateMixPlaneMark;	//因為mixplane Mark定義從0x01改成0x50,在DLMC時, 為了要相容舊版, 這個flag舉起來代表要轉換
/*_FTL_DECL UBYTE gubDBTUnit;
_FTL_DECL UBYTE gubSysCodeUnit[2];
_FTL_DECL UBYTE gubSystemUnit;
*/
#if EnableProductHistory
_FTL_BURNER_DCODE UBYTE gubPHinit; //bit 0:DBT init pass  bit 1:DBT init fail  bit2:有scan init pass  bit3:連can init fail  bit 4:有scan到page
_FTL_BURNER_DCODE PHstruct guwPHUnit[PHUnitNum]; // [][0]:unit [][1]:planebank [][2]:pageptr
_FTL_BURNER_DCODE UBYTE gubPHmatch[PHUnitNum];   // Bit 0:find by DBT , Bit 1:find by scan
_FTL_DECL UBYTE gubTureErase;
#endif
_FTL_DECL_DCODE UBYTE gubEraseAllDone_Flag;


_FTL_DECL UWORD guwSysCodeBlock[2][MAX_CHANNEL];
_FTL_DECL UWORD guwDBTBlock[MAX_CHANNEL];
_FTL_DECL UWORD guwSystemBlock[MAX_CHANNEL];
_FTL_DECL UBYTE gubSysCodeSet[2];
_FTL_DECL UBYTE gubDBTSet;
_FTL_DECL UBYTE gubSystemSet;
#if RDT_BURNER
_FTL_DECL UBYTE gubLaterBadCount_ForDBT[MAX_CHANNEL];//CE0, CE1在Read/Write/Erase DBT時遇到Later Bad的次數
_FTL_DECL UBYTE gubDebugRDT;
#endif

_FTL_DECL UBYTE gubSATAPhyBeforeDevslp;
//_FTL_DECL UWORD guwSystemAreaNum;
_FTL_DECL UWORD guwSystemAreaNum[MAX_CHANNEL];
_FTL_DECL UWORD guwFWTotalUnitNum;
_FTL_DECL FTarget_t guwVTTarget;
_FTL_DECL UBYTE gubWearLevelingCount;
_FTL_DECL UBYTE gubMaxSkipCheckWLNum;
_FTL_DECL UBYTE gubSkipCheckWLNum;
_FTL_DECL UBYTE gubStartWL;
_FTL_DECL UBYTE gubDontUseSortingSystemSet;
_FTL_DECL_DCODE U32 gulDBTVersion;
_FTL_DECL_DCODE U32 gulSystemVersion;
_FTL_DECL_DCODE U32 gulSystemDBTVersion[2][MAX_SYSDBT_NUMBER];
_FTL_DECL_DCODE UBYTE gubSystemDBTSet_FirstScan[2][MAX_SYSDBT_NUMBER];//SET NUMBER
_FTL_DECL_DCODE UBYTE gubDBTOffsetInSystem[MAX_SYSDBT_NUMBER];//DBT SET NUMBER IN SYSTEMBLOCK
_FTL_DECL UBYTE gub_forceactiveGRalwaysSLC;
 #if(!RDT)

            //要放在info block內
_FTL_DECL UBYTE gubSystemSetFreeTable[SYSTEM_SETS];
_FTL_DECL UWORD guwSystemBlockRemappingTable[SYSTEM_SETS][MAX_CHANNEL];

#if (!BURNER)
_FTL_DECL_DCODE UWORD guwGCTargetList[32];    // 32 enough?以後再說...
_FTL_DECL_DCODE UWORD guwGCTargetFreeList[32];    // 32 enough?以後再說...
_FTL_DECL UWORD guwFreeUnitList[32];    			// 32 enough?以後再說...
_FTL_DECL_DCODE U32 gulGCTargetListVCSave[32];    // 32 enough?以後再說...
_FTL_DECL_DCODE U32 gulGCTargetListECSave[32];    // 32 enough?以後再說...
_FTL_DECL_DCODE U32 gulGCTargetListVCDebug[32];    // 32 enough?以後再說...
_FTL_DECL_DCODE U32 gulP2LTableStartIndex[33];			// 32 + 1
_FTL_DECL_DCODE U32 gulGCGRTableStartIndex[5];	//4+1
_FTL_DECL_DCODE U32 gulGCTableEraseCountBackup[MAX_TABLEUNIT];
_FTL_DECL_DCODE U32 gulPrevGCTargetVCBackup[32];
_FTL_DECL_DCODE U32 gulPrevGCTargetECBackup[32];
_FTL_DECL UBYTE gubGCTargetListNum;


_FTL_DECL_DCODE UWORD guwGCTargetList_Backup[32];
_FTL_DECL_DCODE UBYTE gubGCTargetListNum_BackupCnt;
_FTL_DECL_DCODE U32 gulGCTargetListVCDebug_Backup[32];
_FTL_DECL_DCODE U32 gulVC_Backup[32];

_FTL_DECL_DCODE U32 gulCopyDataPartialNum_NeedToDoMore_Cnt;
_FTL_DECL_DCODE U32 gulFlushD1_NeedToDoMore_Cnt;
#endif
_FTL_DECL_DCODE UWORD guwPartialBuildGCTableCnt;
_FTL_DECL_DCODE U32 gulP2LGroupCount_Backup;
_FTL_DECL UBYTE gubFailHandleRequestUnitNum; // Fail Handle不夠用時令GC條件變嚴苛
_FTL_DECL UBYTE gubFreeUnitListNum;
_FTL_DECL UWORD guwVCCheckList[16];
_FTL_DECL UBYTE gubVCCheckListNum;
_FTL_DECL U32 gulGCTableL4kPTR_Forward;
_FTL_DECL U32 gulGCTableL4kPTR_Backward;
_FTL_DECL U32 gulGCTargetVC;
_FTL_DECL UWORD guwGCTableTargetVC;
_FTL_DECL UWORD guwTempTableTargetVC;
_FTL_DECL UWORD guwTableGC_FQIndex;

_FTL_DECL U32 gulTopVC;
_FTL_DECL U32 gulTopVCSum;
_FTL_DECL U32 gulGCZByteNumSum;
_FTL_DECL UBYTE gubPlaneZByteSum[2];
_FTL_DECL U32 gulGCCheckL2PDoneCount;
_FTL_DECL UBYTE gubCloseTargetMode;
_FTL_DECL UBYTE gubMustFreeNum;
_FTL_DECL UBYTE gubStopBottom;
_FTL_DECL UBYTE gubWLDirectCopy;
_FTL_DECL UBYTE gubMaxZipRate;
#if MicronFlashOnly
_FTL_DECL_DCODE volatile UBYTE gubReadVTShift;
_FTL_DECL_DCODE volatile UBYTE gubMicronSBCoarseTuning;
#endif
_FTL_DECL UBYTE gubMicronMode[MAX_CE];
_FTL_DECL UBYTE gubMicronModeSet;
#if B0KB
_FTL_DECL_DCODE UBYTE gubD1D3CEPushFQ[MAX_CE];
#endif

_FTL_DECL volatile U32 gulLEDBlinkConditon;
_FTL_DECL volatile UWORD guwDebug[TOTAL_CE_PHASE_NUM];
_FTL_DECL volatile UWORD guwCHDebug[12];
_FTL_DECL volatile UWORD guwDebugP2LPtr;
_FTL_DECL volatile UWORD guwDebugP2LPtrCopy;
_FTL_DECL volatile UBYTE gubDebugFQLinkIndex;
#endif

// For shallow erase
#if FixShallowErase
	_FTL_DECL UWORD guwVTChildPTRShallowErase;
	_FTL_DECL UWORD guwVTPTRShallowErase;
#endif

_FTL_DECL UBYTE gubFirstCMDIn;
_FTL_DECL UBYTE gubIsWriteProtect;
/*
 *	FlashMode_CE :0; FlashONFI_CE:0 => Legacy
 *	FlashMode_CE :1; FlashONFI_CE:0 => Toggle(NVDDR2)
 *	FlashMode_CE :1; FlashONFI_CE:1 => NVDDR1
 */
 #if UFS
    _FTL_DECL UBYTE gubUFSDefaultMode_CE;
    _FTL_DECL UWORD guwNotSetMap;
    _FTL_DECL UWORD guwDefaultToggleCEMap;
#endif
_FTL_DECL UBYTE gubFlashMode_CE[MAX_CHANNEL];
_FTL_DECL UBYTE gubFlashONFI_CE[MAX_CHANNEL];
_FTL_DECL UBYTE gubIOMode_CE[MAX_CHANNEL];
_FTL_DECL UBYTE gubFlashMode;
_FTL_DECL UBYTE gubFlashClock;
_FTL_DECL UBYTE gubCurrentFlashClock;
_FTL_DECL UBYTE gubDLLFlashClock;	//for DLl workaround
_FTL_DECL UBYTE gubClockDivValue;   //clock div for EDO mode


_FTL_DECL WR_NCQ_CMD_INFO_t WR_NCQ_CMD_INFO[32];
_FTL_DECL UBYTE gubPreReadTQList[32];
_FTL_DECL UBYTE gubPreReadTQListFirst;
_FTL_DECL UBYTE gubPreReadTQListLast;
_FTL_DECL U32 gulNCQCMDDebug;
_FTL_DECL U32 gulNCQCMDDebugCount;
_FTL_DECL U32 gulNCQCMDCnt;
_FTL_DECL U32 gulNCQWRCmdTriggerCnt;
_FTL_DECL U32 gulNCQRDCmdTriggerCnt;
_FTL_DECL UBYTE gubNCQWRCmdTriggerPassCnt;
_FTL_DECL U32 gulWRWaitCnt;
_FTL_DECL UWORD guwNonAlignBufferPTR;
_FTL_DECL UBYTE gubMultiTrigTag;
_FTL_DECL UBYTE gubLastDoneTag;
_FTL_DECL UBYTE gubCmdDoneCount;

_FTL_DECL U32 gulZipMap;

_FTL_DECL U32 gulBuffer2ReadNum;
_FTL_DECL U32 gulBufferr2ReadMask;
_FTL_DECL U32 gulBuffer2Read4KNum;
_FTL_DECL U32 gulBuffer2Read4KMask;

_FTL_DECL UBYTE gubCheckBufferFlag;
_FTL_DECL UBYTE gubErrorTag;
_FTL_DECL UBYTE NCQ_READ_LOG_EXT_ERR[16];
_FTL_DECL volatile U32 gulTemp;
_FTL_DECL volatile UBYTE gubTemp;
_FTL_DECL volatile UWORD guwTemp;
_FTL_DECL U32 gul4KRandomCacheRead;//32 bit for performance
_FTL_DECL UBYTE gubDirectHandl_RUTLog;
#if Hynix3DV6
_FTL_DECL UBYTE gubSBDataA;
_FTL_DECL UBYTE gubSBDataB;
#endif
#if(!RDT)

_FTL_DECL UWORD guw4KRandomCacheReadCount;
_FTL_DECL U32 gulPreviousLBA;
_FTL_DECL U32 gulLastFQEntry;

_FTL_DECL UBYTE gubKeepRFQ;
_FTL_DECL UBYTE gubKeepRFQLink;
_FTL_DECL UBYTE gubKeepRFQL4KNum;
_FTL_DECL U32 gulKeepRFQZipMap;
_FTL_DECL U32 gulKeepRFQLastFEntry;
_FTL_DECL U32 gulKeepRFQNextOffset;
_FTL_DECL UBYTE gubTimeoutEnterRebuildFQ;
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
_FTL_DECL UBYTE gubTimeoutFQIndex;
_FTL_DECL UBYTE gubDebugSendCmdFQIndex;
_FTL_DECL U32 gulNotPolTimeoutCnt;
#endif
#if FORCE_AUTOPOL_TIMEOUT
_FTL_DECL UBYTE gubDebugAutoPolFPUSeqInUse;
_FTL_DECL UBYTE gubDebugAutoPolMTDepth;
_FTL_DECL UBYTE gubDebugAutoPolFQIndex;
_FTL_DECL UBYTE gubDebugAutoPolDirectRemoveFQ;
#endif
#if (!BURNER)
_FTL_DECL U32 gulSeqVir4kIndex;
_FTL_DECL UWORD guwCheckSeqL2PGroup;
_FTL_DECL UWORD guwSeqWCount;
#endif

#if (ENABLE_DEBUG_RW || ENABLE_AUTOPOL_TIMEOUT_HANDLING || B0KB || Hynix3DV6)
	typedef U32	DEF_PARITY_TYPE;
#if BURNER
		_FTL_BURNER_DCODE DEF_PARITY_TYPE guwParityMappingTable[RS_CHANGE_TAG_COUNT];
	 	_FTL_BURNER_DCODE DEF_PARITY_TYPE guwParityMappingTableSLC[(RS_CHANGE_TAG_COUNT/2)];
#else
		_FTL_DECL_DCODE DEF_PARITY_TYPE guwParityMappingTable[RS_CHANGE_TAG_COUNT];
		_FTL_DECL_DCODE DEF_PARITY_TYPE guwParityMappingTableSLC[(RS_CHANGE_TAG_COUNT/2)];
#endif
#else
	typedef UWORD DEF_PARITY_TYPE;
    #if (!BURNER)
        _FTL_DECL DEF_PARITY_TYPE guwParityMappingTable[RS_CHANGE_TAG_COUNT];
        _FTL_DECL DEF_PARITY_TYPE guwParityMappingTableSLC[(RS_CHANGE_TAG_COUNT/2)];
    #endif
#endif


_FTL_DECL UWORD guwParityMappingCount;
_FTL_DECL UWORD guwParityMappingCountSLC;

_FTL_DECL UBYTE gubNeedCheckSeq;
/* RS Related, by new calculation of RS tag */
/* guwRSParityCntAdjust is for adjust calculation of RS parity count,
 * when the position of additional 6 RS parity at the last 3 RS group
 * is the same as original position of RS group.
 */
_FTL_DECL_DCODE UWORD guwRSParityCntAdjust[2][2];

_FTL_DECL UBYTE gubDoFlushCache;
_FTL_DECL U32 gulCountDDRINT;

#endif
// sata isp / read/ write
_FTL_DECL UBYTE gubCmdCode;
_FTL_DECL UBYTE gubHostCmdIn;
_FTL_DECL U32 gulSectorCnt;
_FTL_DECL UBYTE gubNeedSendCmdComplete;
_FTL_DECL UBYTE gubWriteMultiple;
_FTL_DECL union intlba gulLBA,gulMediaSize;
_FTL_DECL UBYTE gubErrorCode;
_FTL_DECL U32 gulUNCLBA;
_FTL_DECL UBYTE gubPowerMode;
_FTL_DECL UBYTE gubLastPowerMode;
_FTL_DECL UBYTE gubNCQDoingTag;
_FTL_DECL UBYTE gubNCQDoing;
_FTL_DECL UBYTE gubNCQINTRError;
_FTL_DECL UBYTE gubD2HMask;
_FTL_DECL UBYTE gubPreRead;
_FTL_DECL U32 gulPreReadTriggerSectorCnt;
_FTL_DECL UBYTE gubPreReadCheckCount;
_FTL_DECL U32 gulPreReadStartLBA;
_FTL_DECL U32 gulPreReadEndLBA;
_FTL_DECL UBYTE gubPreReadPlaneNum;
_FTL_DECL UBYTE gubLimitPrereadCount; /* Limit preread count to 256 to avoid buffer flag deadlock in 1 ch 1 pl case. */
_FTL_DECL U32 gulPreReadSectors;
_FTL_DECL U32 gulWriteCmdsCount;
_FTL_DECL UBYTE gubPreviousWriteCmdMode; //0: SRAM  1:SDR
_FTL_DECL UBYTE gubDDRMode;//0: SRAM 1:DDR
_FTL_DECL UBYTE gubPreReadStartTag;
_FTL_DECL UBYTE gubLBAOverlap;
_FTL_DECL UBYTE gubNeedWaitTQDone;
_FTL_DECL UBYTE gubWUNCDoing;
_FTL_DECL UBYTE gubClearPreRead;
_FTL_DECL UBYTE gubLoadAlignDoing;
_FTL_DECL UBYTE gubBQSerialNum;
_FTL_DECL UBYTE gubForceStop;//BIT0:可能要擋, BIT1:一定要擋

_FTL_DECL UBYTE gubSeqR;
_FTL_DECL U32 gulPreviousEndLBA;
_FTL_DECL UBYTE gubNonSeqCount;
_FTL_DECL UBYTE gubRBQFull;

_FTL_DECL UBYTE gubToggleMode;
_FTL_DECL  UBYTE gubSoftReset;
_FTL_DECL UBYTE gubHardReset;
_FTL_DECL UBYTE gubLinkLost;
_FTL_DECL UBYTE gubForceExit;
_FTL_DECL UBYTE gubAP_enable;
_FTL_DECL UBYTE gubMPRW;
_FTL_DECL RRT_SETTING gRRTMode;
_FTL_DECL UBYTE gubRRTVersion;
_FTL_DECL UBYTE gubFlashTypeFail;
_FTL_DECL UBYTE gubEraseFailBitMapInit;
_FTL_DECL union intlba gulCurrentSize;
_FTL_DECL U32 gulNativeSize;
_FTL_DECL UWORD guwMaxCyl,guwCurCyl;
_FTL_DECL UBYTE gubMaxHead,gubCurHead;
_FTL_DECL UBYTE gubMaxSec,gubCurSec;
_FTL_DECL Identify_Table  IDT;
_FTL_DECL UBYTE gubDevIPMON;//S5
_FTL_DECL UBYTE gubCMDFinish;
//_FTL_DECL UBYTE gubQuickEraseFlag;
/// Added by Victor
_FTL_DECL UBYTE gubHIPMEnable;
_FTL_DECL UBYTE gubDIPMP2SEnable;
_FTL_DECL UBYTE gubDEVSLPEnable;
_FTL_DECL UBYTE gubAP2SWaitIdle2Time;

#if(!BURNER)
_FTL_DECL UBYTE gubAutoD2HBackup;
_FTL_DECL UBYTE gubIgnoreFlag;
#endif

_FTL_DECL UBYTE  gubDIPM_THRESHOLD;
_FTL_DECL UBYTE  gubDIPM_P2S_THRESHOLD;
_FTL_DECL UWORD  guwDIPM_AUTOP2S_THRESHOLD;
_FTL_DECL UBYTE  gubGatingVDTEnable;
_FTL_DECL UBYTE  gubENABLE_HW_DIPM;
_FTL_DECL UBYTE  gubENABLE_FW_DIPM;
#if Hynix
_FTL_DECL_DCODE UBYTE  gubDoFlushD1;
_FTL_DECL UBYTE  gubDaCmdEnable;
_FTL_DECL UBYTE  gubDaCmdEraseEnable;
#endif

_FTL_DECL UBYTE gubL2PSWAPorMoveDone;
#if(!BURNER)
#if Hynix
_FTL_DECL U32 gulD1RetryTableSizePerDie;
_FTL_DECL U32 gulD3RetryTableSizePerDie;
#endif

_FTL_DECL UBYTE  gubWaitFTLtoSwapL2P;
//_FTL_DECL U32 gulTrimEnd4kIndex[D_TRIM_TABLE_NUMBER];   // U32*16
//_FTL_DECL U32 gulTrimStart4kIndex[D_TRIM_TABLE_NUMBER];  // U32*16
_FTL_DECL U32 *gulTrimEnd4kIndex;
_FTL_DECL U32 *gulTrimStart4kIndex;
_FTL_DECL UBYTE gubNeedFlushL2PInTrim;
_FTL_DECL U32 gulTotalL2PGroupTrimed;


_FTL_DECL UBYTE gubL2PSWAPorMoveDone;

_FTL_DECL UWORD guwDebugOldUnit;

/// Added by Victor
_FTL_DECL UBYTE gubTest2;
_FTL_DECL UBYTE gubPrevDPSStage;

_FTL_DECL UWORD  guwL2PGroupIndexForTrim;
_FTL_DECL L2P_InRamInverse_t *gulInRamInverseBackUp;

_FTL_DECL UBYTE  gubCalledWithHostCMD;
_FTL_DECL UBYTE  gubNoAlignSecCntForTrim;
_FTL_DECL U32    gulNoAlignStartLBAForTrim;
#endif

/*_FTL_DECL UBYTE gubErrorDumpUnit;
_FTL_DECL UBYTE gubErrorDumpBurst;*/
_FTL_DECL UWORD guwErrorDumpBlock[MAX_CHANNEL];
_FTL_DECL UBYTE gubErrorDumpSet;

#if(!BURNER)
#if(!RDT)
_FTL_DECL_DCODE DBTModifyList_t gullDBTModifyList[RUT_LOG_NUM];
// ============ Error Dump ===========
//_FTL_DECL UBYTE gubErrorDumpFreeUnitGet;
_FTL_DECL UBYTE gubErrorDumpFreeSetGet;
_FTL_DECL UWORD guwErrorDumpLastPlaneWrite;
_FTL_DECL UBYTE gubAlreadyCopyFLHRegister;
#endif
#endif


_FTL_DECL UBYTE gubNCQSupport;
_FTL_DECL UBYTE gubFQNeedExecuteRightNow[MAX_CE];
_FTL_DECL UBYTE gubFQNotSequential[MAX_CE]; // for read
_FTL_DECL UBYTE gubToCheckNextCmdSequence[MAX_CE]; // for read
_FTL_DECL void *gulNotFinishPhase[MAX_CE]; // for read
_FTL_DECL UBYTE gubNoFQIn[MAX_CE]; // for read
_FTL_DECL UBYTE gubMTDepthPushNumber[MAX_CE];
_FTL_DECL UBYTE gubMTEVENT_CNT_LIMIT;

_FTL_DECL UWORD guwSQCnt_DMAC;
_FTL_DECL UWORD guwCQCnt_DMAC;
_FTL_DECL UWORD guwSQCnt_SE0;
_FTL_DECL UWORD guwCQCnt_SE0;

_FTL_DECL ParaOffset0x00 gulPara0x00;
_FTL_DECL ParaOffset0x04 gulPara0x04;
_FTL_DECL ParaOffset0x10 gulPara0x10;
_FTL_DECL ParaOffset0x1C gulPara0x1C;
_FTL_DECL ParaOffset0x20 gulPara0x20;
_FTL_DECL ParaOffset0x24 gulPara0x24;
_FTL_DECL U32 guliFSA[4];
_FTL_DECL U32 guliFSA0_2;

_FTL_DECL U32 gulBackGround_Counter;
_FTL_DECL UBYTE gubTimeToDoBG;
_FTL_DECL UBYTE gubEnableBG_Idle;
_FTL_DECL volatile UBYTE gubNeedChkDoneTag;
_FTL_DECL UBYTE gubSeqCmdCnt;
#if TLC
_FTL_DECL_DCODE UBYTE gubStopD1BGVictimSearch;
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
_FTL_DECL_DCODE UWORD guwFourPlaneD3WriteFPUArray[2][4][3];// Data IO : PLANE : LMU
_FTL_DECL_DCODE UWORD guwTwoPlaneD3WriteFPUArray[2][2][3];
_FTL_DECL_DCODE UWORD guwOnePlaneD3WriteFPUArray[3];
#else
_FTL_DECL UWORD guwTwoPlaneD3WriteFPUArray[2][3][2][4];
_FTL_DECL UWORD guwOnePlaneD3WriteFPUArray[3][4];
#endif
#endif
#if FLUSHD1_PARTIAL_CE_ADVANCE
_FTL_DECL UBYTE gubFlushD1Flag; //D1 to D3 Program DMA done flag for CE in group
_FTL_DECL UBYTE gubPartialDoingCENum;
#endif


#if(!RDT)

//_FTL_DECL ULLONG guoRCnt;
//_FTL_DECL ULLONG guoWCnt;
//_FTL_DECL UBYTE gubRUncFailGen;


//For
//_FTL_DECL UWORD guwFlushD1Fun_D1Unit;
_FTL_DECL_DCODE UWORD guwFlushD1Fun_D3Unit;
//_FTL_DECL volatile UBYTE gubFlushD1Fun_LockL2P;
//_FTL_DECL volatile UWORD guwFlushD1Fun_PlanePTR;
//_FTL_DECL U32 gulFlushD1Fun_GCGRL4KNum;
//_FTL_DECL U32 gulFlushD1Fun_L4kPTR;
_FTL_DECL_DCODE volatile UBYTE gubPartialFlushD1Doing;
_FTL_DECL_DCODE volatile UBYTE gubPartialFlushD1toD3Mode;
_FTL_DECL_DCODE volatile UBYTE gubForceFlushD1ByIgnoreFail;
_FTL_DECL_DCODE volatile UWORD guwCopyboundary;
_FTL_DECL_DCODE volatile UWORD guwStartProgramOrderIndex;
_FTL_DECL_DCODE volatile UWORD guwEndProgramOrderIndex;

//_FTL_DECL volatile UBYTE gubGCTargetInD1Num;
#endif

_FTL_DECL UBYTE gubEnableSpecialData;

_FTL_DECL U32 gulReadZeroDataCnt;
_FTL_DECL_DCODE UBYTE gubHighTempSetting;
_FTL_DECL_DCODE UBYTE gubEnable_Special_Data_Workaround;
_FTL_DECL_DCODE UBYTE gubEnableMilitaryErase;
_FTL_DECL_DCODE UBYTE gubEnableTemperatureControl;
_FTL_DECL_DCODE UBYTE gubEnableCFastIdentifyDeviceWord0Request;
_FTL_DECL_DCODE UBYTE gubEnableOverClocking;
_FTL_DECL_DCODE UBYTE gubCFastHighTemprature;
_FTL_DECL_DCODE UBYTE gubCFastLowTemprature;
_FTL_DECL UBYTE gubEnable_CTRL_INT_For_CBitZeroCase;
//4k sustined
_FTL_DECL UWORD guwCopyDataPartialNum;
_FTL_DECL UBYTE gubPredictGC_Done;
_FTL_DECL UBYTE gubPredictCloseTargetCountPerUnit;
_FTL_DECL UBYTE gubPredictWLCountPerUnit;
_FTL_DECL UBYTE gubCloseTargetCnt;
_FTL_DECL UBYTE gubLastCloseTargetDone;
#if(!RDT)
_FTL_DECL UWORD guwL2PEntrysPerL2PTempGroup;
_FTL_DECL UBYTE gubL2PEntrysPerL2PTempGroupLog;
_FTL_DECL UWORD guwL2PEntrysPerL2PTempGroupMask;
_FTL_DECL UWORD guwL2PEntrysPerL2PTemp512BGroup;
_FTL_DECL UBYTE gubL2PEntrysPerL2PTemp512BGroupLog;
_FTL_DECL UWORD guwL2PEntrysPerL2PTemp512BGroupMask;
_FTL_DECL TempTableInformation gTempTable;

_FTL_DECL UBYTE gubFirstCMDPending;
_FTL_DECL U32 gulFirstCMDTimeSave;

_FTL_DECL UBYTE gubFQOutOfOrderCnt;
_FTL_DECL UBYTE gubWSeq;

_FTL_DECL UBYTE gubLargeSizeCmdCount;

_FTL_DECL UBYTE gubRFQCnt;

_FTL_DECL UBYTE gubIsDoingRUT2DBT;

#endif
#if FixShallowErase
_FTL_DECL UBYTE gubSkipProgramFail;
#endif


//ReadDisturb Partial
#if (!BURNER)
_FTL_DECL UBYTE gubSwapRUTState;
_FTL_DECL UBYTE gubSwapRUTDie;
_FTL_DECL UBYTE gubSwapRUTCopyDone;
_FTL_DECL UBYTE gubSwapRUTForceDone;
_FTL_DECL UWORD guwSwapRUTLogIndex;
_FTL_DECL UWORD guwSwapRUTRecordQueueNum;
_FTL_DECL U32 	gulSwapRUTPartialPlaneNumber;
_FTL_DECL U32	gulCopyUnitLoadL4KIndex;
_FTL_DECL U32	gulCopyUnitLoadPlanePTR;
_FTL_DECL U32	gulCopyUnitSaveL4KIndex;
#endif

_FTL_DECL_DCODE UBYTE gubSupportSATA;
_FTL_DECL UBYTE gubGetTempFailFlag;
_FTL_DECL UBYTE gubGetTempFailValue;

//WL Issue
#if (!BURNER)
_FTL_DECL UWORD guwHotDataSamplingUnit;
_FTL_DECL UWORD guwHotDataSamplingRate;
_FTL_DECL UWORD guwHotDataSamplingCount;
_FTL_DECL UBYTE gubHotDataLevel;
#endif


#if Hynix
typedef struct
{
    UBYTE ubSlcRRstepCnt; //for hynix 1ynm TLC == 32
    UBYTE ubSlcRRregCnt;  //for hynix 1ynm TLC == 1
    UBYTE ubSlcRegAdr[4]; //for hynix 1ynm TLC 4 is enough
    UBYTE ubXlcRRstepCnt; //for hynix 1ynm TLC == 32
    UBYTE ubXlcRRregCnt;  //for hynix 1ynm TLC == 8
    UBYTE ubXlcRegAdr[8]; //for hynix 1ynm TLC 8 is enough

    UWORD uwD1RetryTblSizePerDie;//for hynix 1ynm TLC == 1 * 32 = 32
    UWORD uwD3RetryTblSizePerDie;//for hynix 1ynm TLC == 8 * 32 = 256
    U32   ulSlcRetryTblBase;
    U32   ulXlcRetryTblBase;
    UBYTE ubInitDone;  //BIT0: XLC  BIT1: SLC
    UBYTE ubOpCH;
    UBYTE ubOpCE;
    UBYTE ubOpDie;
} HynixRetryInfo; //32Byte

typedef struct
{
    UWORD uwOpPage;
    UBYTE ubIsValid;
    UBYTE ubRsv;
    UBYTE ubSlcCurRRstep;
    UBYTE ubXlcCurRRstep;
    UBYTE ubSlcLastRRstep;
    UBYTE ubXlcLastRRstep;
    U32   ulSlcRetryTblOffset;
    U32   ulXlcRetryTblOffset;
} HynixRetryPerDieInfo; //16Byte

typedef struct
{
    HynixRetryPerDieInfo ubDie[2]; //for hynix 1ynm TLC 1 is enough  Array should be cover max die support
} HynixRetryMeta; //32Byte

_FTL_DECL_DCODE HynixRetryInfo gsHynixRetryInfo; //32Byte
_FTL_DECL_DCODE HynixRetryMeta *gpsHynixRetryMeta;  //4Byte   |CE0CH0|CE0CH1|CE1CH0|CE1CH1|
_FTL_DECL_DCODE UBYTE *gpubXlcRetryTable; 			//4Byte   |CE0CH0DIE0|CE0CH0DIE1|CE0CH1DIE0|CE0CH1DIE1|CE1CH0DIE0|CE1CH0DIE1|CE1CH1DIE0|CE1CH1DIE1|
_FTL_DECL_DCODE UBYTE *gpubSlcRetryTable; 			//4Byte   |CE0CH0DIE0|CE0CH0DIE1|CE0CH1DIE0|CE0CH1DIE1|CE1CH0DIE0|CE1CH0DIE1|CE1CH1DIE0|CE1CH1DIE1|

#if (!BURNER)
_FTL_DECL_DCODE HynixRetryMeta gsHynixRetryMeta[MAX_CE]; //16*32 = 512Byte  //|CE0CH0|CE0CH1|CE1CH0|CE1CH1|

#if (Hynix3DV6 || Hynix3DV5)
_FTL_DECL_DCODE UBYTE gubXlcRetryTable[357 * MAX_CE]; //|CE0CH0DIE0|CE0CH0DIE1|CE0CH1DIE0|CE0CH1DIE1|CE1CH0DIE0|CE1CH0DIE1|CE1CH1DIE0|CE1CH1DIE1|
_FTL_DECL_DCODE UBYTE gubSlcRetryTable[50 * MAX_CE];  //|CE0CH0DIE0|CE0CH0DIE1|CE0CH1DIE0|CE0CH1DIE1|CE1CH0DIE0|CE1CH0DIE1|CE1CH1DIE0|CE1CH1DIE1|
#else
_FTL_DECL_DCODE UBYTE gubXlcRetryTable[17152]; //|CE0CH0DIE0|CE0CH0DIE1|CE0CH1DIE0|CE0CH1DIE1|CE1CH0DIE0|CE1CH0DIE1|CE1CH1DIE0|CE1CH1DIE1|
_FTL_DECL_DCODE UBYTE gubSlcRetryTable[2144];  //|CE0CH0DIE0|CE0CH0DIE1|CE0CH1DIE0|CE0CH1DIE1|CE1CH0DIE0|CE1CH0DIE1|CE1CH1DIE0|CE1CH1DIE1|
#endif

#endif

#endif

_FTL_DECL U32 gulCeEnBitMap;

#if TLC
// disable freequeue check when add/get free queue in initial state
_FTL_DECL UBYTE gubRebuildFreeQueueDoing;
#endif

//_FTL_DECL  MP_SMART_Attribute_Entry  gMPAttributeArray[AttributeTotalEntry];
//DDR.c
_FTL_DECL void idle(U32 ulLen);
_FTL_DECL void DDR_Initial();
_FTL_DECL void SetDQS(U32 ulSetValue);
_FTL_DECL UBYTE CountDQS(UBYTE* ubSuccess);
_FTL_DECL UBYTE CheckData(U32* uladdress,U32 ulZ1size);
_FTL_DECL void RAM_Set();
_FTL_DECL void RAM_Set_ZONE_Region(UBYTE ubZoneSelect, U32 ulAddress, U32 ulBytelength);
_FTL_DECL void entry_selfrefresh ();
_FTL_DECL void exit_selfrefresh ();
_FTL_DECL void ScanDelayLine_BISTver();
_FTL_DECL unsigned char BIST_ForScanDelayLine(unsigned long uladdress,unsigned long ulZ1size, unsigned char ubtemp);

//flainit.c
#if Hynix
_FTL_DECL_ICODE  UBYTE flaRetrieveRetryTable(U32 ulCEBitMap);
_FTL_DECL_ICODE  UBYTE flaGetHynixReadRetryTable(UBYTE ubCH, UBYTE ubCE, UBYTE ubDie, UBYTE ubSLCMode);
_FTL_DECL_ICODE  void flaChkStatus(UBYTE ubCH, UBYTE ubDie, UBYTE ubMask);
_FTL_DECL_ICODE  void  flaSetNandParameter(UBYTE ch, UBYTE addr, UBYTE data);
#endif
_FTL_DECL_ICODE  void flaSetFlashAddressType(void);
_FTL_DECL_ICODE  UBYTE ftlCheckFlashFormat_SystemSet(void);
_FTL_DECL_ICODE  void flaCheckFirstID(void);
_FTL_DECL_ICODE  void CheckFlashType(void);
#if UFS
_FTL_DECL_ICODE U32 M_MapToCEDecoder(UBYTE ubChannel, UBYTE ubCE);
_FTL_DECL_ICODE U32 M_MapToCEDecoderMT(UBYTE ubCE);
_FTL_DECL_ICODE UBYTE UFSToggleCheck(U32 ulCEEnable, UBYTE ubCH);
_FTL_DECL_ICODE  void UFSByPass(void);
#endif
#if ENABLE_SANDISK_1ZNM_SPF
_FTL_DECL_ICODE void CheckSandisk160G(void);
#endif
#if ENABLE_FORCE_L06B_TO_B0KB
_FTL_DECL_ICODE void ForceL06BToB0KB(UBYTE ubFirstSetB0KB);
#endif
_FTL_DECL_ICODE  UBYTE flaCheckLogValue(U32 uwDataIn);
_FTL_DECL_ICODE  UBYTE  flaSetToggleMode(UBYTE ubMode, UBYTE ubForceSetFeature);
_FTL_DECL_ICODE UBYTE  flaSetToggleMode_CE(UBYTE ubMode , UBYTE ubForceSetFeature , UBYTE ubCE);
_FTL_DECL_ICODE  UBYTE flaSetFlashMode(void);
#if Hynix
_FTL_DECL_ICODE void ToggleModeEnable(UBYTE Mode);
#endif
_FTL_DECL_ICODE void flaSetFlashPadControl(UBYTE ubODT_Value);
_FTL_DECL_ICODE void flaSetFlashPadControl_Default(void);
_FTL_DECL_ICODE void InitUart(void);
_FTL_DECL_ICODE void flaSetOverDrive();
#if MicronFlashOnly
_FTL_BURNER_ICODE  void flaToshibaIDCheck(void);
#else
_FTL_DECL_ICODE  void flaToshibaIDCheck(void);
#endif
//_FTL_DECL_ICODE  void flaSamsungIDCheck(void);
_FTL_DECL_ICODE  void flaIntelMicronIDCheck(void);
_FTL_DECL_ICODE  void flaYMTCIDCheck(void);
_FTL_DECL_ICODE void flaHynixIDCheck(void);
_FTL_DECL_ICODE void flaSetParameter(UBYTE ubMode);
_FTL_DECL_ICODE void flaSetFlashUseType(UBYTE ubMode);
_FTL_DECL_ICODE UBYTE flaSetONFIMode(UBYTE ubMode);
_FTL_DECL_ICODE void flaMicronRandomizer(UBYTE ubMode);
_FTL_DECL_ICODE void flaMicronPreRead(UBYTE ubMode );
_FTL_DECL_ICODE void flaReadParameterPage(UBYTE ubCE, UBYTE ubMode);
#if MicronFlashOnly
_FTL_DECL_ICODE void flaReadOTPPage(UBYTE ubCE);
#endif
_FTL_DECL_ICODE UWORD flaCalculateCRC (U32 ulSrcAdr, U32 ulLen);
_FTL_DECL_ICODE void flaInfoBlockSetting(SYSTEM_GROUP_t *InfoArray );
_FTL_DECL_ICODE void flaSetParamPageInfo(UBYTE ubMode);
_FTL_DECL_ICODE void flaGetFeature(UBYTE ubChannel, UBYTE ubAddr , UBYTE *ubGetFeatureData);
_FTL_DECL_ICODE void flaSetFeature(UBYTE ubChannel, UBYTE ubAddr , UBYTE *ubSetFeatureData);
_FTL_DECL_ICODE void flaReset(UBYTE ubChannel, UBYTE ubResetCmd);
//_FTL_DECL_ICODE  void flaToshibaIDCheck(void);
//_FTL_DECL_ICODE  void flaSamsungIDCheck(void);
//_FTL_DECL_ICODE  void flaIntelMicronIDCheck(void);
//_FTL_DECL_ICODE void flaHynixIDCheck(void);
#if ENABLE_DEBUG_UART_FLH_INITIAL
_FTL_DECL_ICODE void ftlCheckRemappingTable_AccordingToDBT(U32 ulDBTAddr);
#endif
_FTL_DECL_ICODE void ftlRebuildRemappingTableByOldRule_AccordingToDBT(U32 ulDBTAddr);
_FTL_DECL_ICODE void InitialSystemRemappingTable(U32 ulRAMAddr);
_FTL_DECL_ICODE void SetInfoBySystemBlock(void);
_FTL_DECL void flaSwitchClock(UBYTE ubFlashClock);
_FTL_DECL_ICODE void fla_PMU_Setting(UBYTE ubFlashClock);
_FTL_DECL void flaDLLTracking(UBYTE ubProgramTracking, UBYTE ubAutoMode, U32 ulSelVal);
_FTL_DECL_ICODE void fla_Set_Offset_DQS_Delay(UBYTE ubFlashClock);
_FTL_DECL_ICODE void fla_Set_FPU_DLY_Cycle(UWORD uwFPUStartPtr, UBYTE ubDelayCycleValue);
_FTL_DECL_ICODE UBYTE fla_Detect_DQSB(void);
_FTL_DECL_ICODE void flaScanCE_And_Remapping(void);
_FTL_DECL_ICODE void flaSetFlashInterface(UBYTE ubChannel, UBYTE ubCE);
_FTL_DECL_ICODE void flaSet_ODT_DRIVE(void);
_FTL_DECL_ICODE void flaSetPadIOType(UBYTE ubChannel, U32 ulPadIOType);
_FTL_DECL_ICODE UBYTE flaLoadSystemAreaSet(UBYTE ubCEIndex, UBYTE ubSet, U32 ulSramAddr, U32 ulL4K);
//_FTL_DECL_ICODE void flaSaveSystemAreaUnit(UBYTE ubCEIndex, UBYTE ubUnit, U32 ulPlanePtr, U32 ulSramAddr, U32 ulL4K);
_FTL_DECL_ICODE void flaSetParameterBeforePreformat(SYSTEM_GROUP_t *InfoArray);
_FTL_DECL_ICODE void flaPowerOnReset(void);
_FTL_DECL_ICODE void flaSetFakeInfoBlk(SYSTEM_GROUP_t *InfoArray, MP_SMART_Attribute_Entry * MP_Parameter);
_FTL_DECL_ICODE void flaSetParameterByFlashDefaultType(void );
_FTL_DECL_ICODE void ACTiming_Setting_Register(UBYTE ubFlashClock);
_FTL_DECL_ICODE void ACTiming_Setting_FPU(UBYTE ubFlashClock);
_FTL_DECL_ICODE void ACTiming_Setting_MTP(UBYTE ubFlashClock, UBYTE ubWaitFQ);
_FTL_DECL_ICODE void ACTiming_Setting(UBYTE ubFlashClock, UBYTE ubWaitFQ);
_FTL_DECL_ICODE UBYTE GetLargerValue(UBYTE ubNum1, UBYTE ubNum2, UBYTE ubMask);
_FTL_DECL_ICODE void flaReloadAonHeader(void);
#if TLC_BICS2
_FTL_DECL void flaBics4_Revision_Identify(void);
_FTL_DECL_ICODE void flaDisable_IPR(void);
#endif
_FTL_DECL_ICODE void flaDLLCDCIssueWorkaround(U32 ulDLLRegOffset, U32 ulVal);

_FTL_BURNER_ICODE void ftlGetUserFreeBlock(FTarget_t *uwTarget, UBYTE ubPopAllFreeBlock, UBYTE ubTableSelect);
_FTL_BURNER_ICODE UBYTE ftlGetUserFreeBlockByDie (FTarget_t * uwTarget, UBYTE ubDie, UBYTE ubTableSelect);
_FTL_BURNER_ICODE void ftlAddToUserFreeTable(FTarget_t *uwTarget, UBYTE ubTableSelect);
_FTL_DECL_ICODE void ftlAddToSystemSetFreeTable(UBYTE ubSet);
_FTL_DECL_ICODE void ftlAddToVTAreaFreeTable(FTarget_t *uwTarget);
_FTL_DECL_ICODE void ftlGetVTAreaFreeBlock(FTarget_t *uwTarget);
#if FixShallowErase
_FTL_DECL_ICODE  UWORD ftlScanSystemUnit_Binary(UWORD uwUnit, UBYTE ubBurst, UBYTE ubCEindex);
#endif
//_FTL_BURNER_ICODE U32 ftlGetSystemBlockFreeUnit(UBYTE ubMode) ;
_FTL_DECL_ICODE UBYTE ftlGetSystemSetNumber(UBYTE ubMode, U32 ulDBTAddr, UBYTE ubChannel, UWORD uwBlock);
_FTL_DECL_ICODE UWORD ftlGetAnotherSystemBlockInSet(UBYTE ubMode, U32 ulDBTAddr, UBYTE ubChannel, UWORD uwBlock);
_FTL_DECL_ICODE UBYTE ftlGetSystemSetFreeSet(UBYTE ubMode);
_FTL_DECL U32 mDMAC_CRC(U32 ulSourceAddr, U32 ulByteLength, U32 ulLBA, UBYTE ubSetCQ , UWORD uwMode);
_FTL_DECL_ICODE void ftlAddToDebugCommanddQueue(U32 ulCmd,U32 ulLBA,U32 ulSectorCnt,U32 ulOther);
#if (!RDT)
//ftlcommon.c
_FTL_BURNER_ICODE void ftlGetMaxECFreeBlock(FTarget_t *uwTarget, UBYTE ubTableSelect);
_FTL_DECL_ICODE void ftlGetMinMaxECFreeBlock(FTarget_t *uwTarget, UBYTE ubTableSelect, UBYTE ubMode);
_FTL_DECL_ICODE UWORD ftlGet2ndMinECFreeIndex(UBYTE ubTableSelect);
_FTL_DECL_ICODE UWORD ftlGet2ndMaxECFreeIndex(UBYTE ubTableSelect);
_FTL_BURNER_ICODE void ftlGetRandomFreeBlock(FTarget_t *uwTarget, UWORD uwFreeUnitPtr, UBYTE ubTableSelect, UBYTE ubMode, UWORD uwUnitIndex);
_FTL_BURNER_ICODE UBYTE ftlGetMaxECUnitIndex();
_FTL_BURNER_ICODE void ftlCheckUserFreeBlockTable(void);
_FTL_DECL_ICODE UBYTE ftlGenZCodeData(UBYTE ubZCode, U32 ulTargetAddr, U32 ulL4K_LCA, UBYTE ubFlagIndex);
#endif

_FTL_DECL_ICODE void SetQInfo();
_FTL_DECL_ICODE void mSE_SV(UBYTE ubOperationSize, U32 ulSourceAddr, U32 ulByteLength, U32 ulPattern_L, U32 ulPattern_H);
_FTL_DECL_ICODE void mSE_MSV(UBYTE ubOperationSize, U32 ulSourceAddr, U32 ulByteLength, U32 ulPattern_L, U32 ulPattern_H, U32 ulMask_L, U32 ulMask_H, UBYTE ubEnableMask);
_FTL_DECL_ICODE void mSE_COPY(U32 ulSourceAddr,  U32 ulTargetAddr, U32 ulByteLength);
_FTL_BURNER_ICODE U32 mSE_Count1(U32 ulSourceAddr, U32 ulByteLength);
_FTL_DECL void mDMAC_SV(UBYTE ubOperationSize,  U32 ulDestinateAddr, U32 ulByteLength, U32 ulLBA, U32 ulRefValue, U32 ulMask, U32 ulMode, UBYTE ubEnableSwitchTask);
_FTL_DECL_ICODE void mDMAC_COPY( U32 ulSourceAddr, U32 ulTargetAddr, U32 ulByteLength, U32 ulLBA, U32 ulMask, UBYTE ubEnableMask, UBYTE ubEnableErrIns, UBYTE ubSetCQ);
_FTL_BURNER_ICODE void mDMAC_PZIP(U32 ulSourceAddr, UBYTE ubEnableErrIns, UBYTE ubSetCQ);
_FTL_BURNER_ICODE void mDMAC_E3D(U32 ulSourceAddr, U32 ulByteLength, U32 ulLBA, UBYTE ubSetCQ);
_FTL_DECL_ICODE void mDMAC_XOR(U32 ulSourceAddr1, U32 ulSourceAddr2, U32 ulDestinateAddr, U32 ulByteLength, UBYTE ubSetCQ, UBYTE ubXNOR);
_FTL_DECL_ICODE UBYTE mDMAC_CMP(U32 ulSourceAddr, U32 ulSourceAddr2, U32 ulByteLength);
#if (B0KB && BURNER)
_FTL_BURNER_ICODE U32 ftlEraseTarget(FTarget_t *PhyTarget, UBYTE ubDSelect, UBYTE ubDoSwapRUT);
#else
_FTL_DECL_ICODE U32 ftlEraseTarget(FTarget_t *PhyTarget, UBYTE ubDSelect, UBYTE ubDoSwapRUT);
#endif
_FTL_DECL_ICODE void mForceSramToFlash(U32 ulRAMAddr,U32 ulSizeInByte, U32 ulHeader);
_FTL_DECL_ICODE UBYTE FPU_A2_Program(UBYTE ubUseRUT, UBYTE ubCE, UWORD uwFUnit, U32 ulFEntry, U32 ulBaseAddr, U32 ulLCA1, U32 ulLCA2,U32 ulFW);
_FTL_DECL_ICODE UBYTE FPU_A2_Erase(UBYTE ubUseRUT, UWORD uwFUnit);

//flash.c
_FTL_DECL_ICODE void flaDumpError(UBYTE ubForceBootCode, UBYTE ubSaveVTSerialNumber);
_FTL_DECL_ICODE UWORD flaReadFailStatusFPUPointer(UBYTE ubgFQLinkIndex);
_FTL_DECL void flaLEDBlink(U32 ulFailCondition);
_FTL_BURNER_ICODE void flaSetFlashSourceAddrCE_CH(UBYTE ubgFQLinkIndex);
_FTL_DECL UBYTE flaCheckFQContinueNum(UBYTE ubCEIndex, UBYTE ubCFQIndex, UBYTE ubUnServedCount);

_FTL_DECL UBYTE flaSendCmd_CH(UBYTE ubgFQLinkIndex, UBYTE ubCEIndex, UWORD uwFPU, UBYTE ubAutoPoll, UBYTE ubSendINT, UBYTE ubPolSeqSel);

_FTL_DECL UBYTE flaFlashToSram_CH(UBYTE ubPhase, UBYTE ubgFQLinkIndex, UWORD uwFPU, UBYTE ubMode, UBYTE ubPolSeqSel);
_FTL_DECL UBYTE flaSramToFlash_CH(UBYTE ubPhase,UBYTE ubgFQLinkIndex, UWORD uwFPU,  UBYTE ubAutoPol, UBYTE ubPolSeqSel);
#if TLC
_FTL_DECL UBYTE flaEraseTarget_CH(UBYTE ubPhase,UBYTE ubgFQLinkIndex, UBYTE ubPlaneMode);
#else
_FTL_DECL UBYTE flaEraseTarget_CH(UBYTE ubPhase,UBYTE ubgFQLinkIndex, UWORD uwFPU);
#endif
_FTL_DECL UBYTE flaFlashToSramRandCache_CH(UBYTE ubFQLinkIndexForCmd, UBYTE ubFQLinkIndexForDMA);
_FTL_DECL U32 flaFastPage(U32 ulFSector);
_FTL_DECL UBYTE flaFlashToSram_0030(UBYTE ubPhase, UBYTE ubgFQLinkIndex, UWORD uwFPU, UBYTE ubMode);
#if MicronFlashOnly
_FTL_DECL void flaSwitchMicronSLCMode(UBYTE  ubgFQLinkIndex ,UBYTE ubCEIndex);
#endif
#if ENABLE_RESET_RETRY_LEVEL
_FTL_DECL void flaSwitchDefaultRetryLevel(UBYTE ubgFQLinkIndex , UBYTE ubCEIndex);
#endif
_FTL_DECL_ICODE void flaGetUnitReadCheckInfo(UBYTE ubgFQLinkIndex, UBYTE ubMaxPlaneOffset ,UBYTE ubA2Mode);
_FTL_BURNER_ICODE void flaNES_SB_TESTFlow( UBYTE ubgFQLinkIndex );
#if TLC
#if ((!BURNER) && ENABLE_READ_CHECK_FUNCTION && 0)
_FTL_BURNER_ICODE void flaGetUnitReadCheckInfo_DynamicD1SLC(UWORD uwFUnit, U32 ulFEntry, U32 ulCurrentPlaneIndex);
_FTL_BURNER_ICODE void flaGetUnitReadCheckInfo_DynamicD3SLC(UWORD uwFUnit, U32 ulFEntry, U32 ulCurrentPlaneIndex);
_FTL_BURNER_ICODE void flaGetUnitReadCheckInfo_StaticD1SLC(UWORD uwFUnit, U32 ulFEntry);
_FTL_BURNER_ICODE void flaGetUnitReadCheckInfo_StaticD3SLC(UWORD uwFUnit, U32 ulFEntry);
_FTL_BURNER_ICODE void flaGetUnitReadCheckInfo_StaticD3TLC(UWORD uwFUnit, U32 ulFEntry);
_FTL_BURNER_ICODE void flaGetUnitReadCheckInfo_TLCRule(UBYTE ubgFQLinkIndex, UBYTE ubMaxPlaneOffset);
#endif
_FTL_BURNER_ICODE UWORD flaFourPlaneD3WriteChangFPU(UBYTE ubDMA_Mode, UBYTE ubD3WriteMode, UBYTE ubPlane );
_FTL_BURNER_ICODE UWORD flaTwoPlaneD3WriteChangFPU(UBYTE ubDMA_Mode, UBYTE ubD3WriteMode, UBYTE ubPlane );
_FTL_BURNER_ICODE UWORD flaOnePlaneD3WriteChangFPU(UBYTE ubD3WriteMode);
_FTL_BURNER_ICODE UBYTE flaCheckD3CopyBackMTDone(UBYTE ubgFQLinkIndex);
#endif
#if ((!BURNER) && ENABLE_READ_CHECK_FUNCTION)
_FTL_DECL_ICODE void flaGetUnitReadCheckInfo_Rule(UBYTE ubgFQLinkIndex, UBYTE ubMaxPlaneOffset, UBYTE ubA2Mode);
_FTL_DECL_ICODE UBYTE flaCheckFQSeq(UBYTE ubgFQLinkIndex, U32 ulCurrentFEntry);
_FTL_DECL_ICODE UBYTE flaNeedReadChk(UWORD uwFUnit, UBYTE ubgFQLinkIndex, UBYTE ubA2Mode);
_FTL_DECL_ICODE UWORD flaGetForceReadThreshold(UBYTE ubA2Mode , UWORD uwUnit);
#endif


#if Hynix
#if (!RDT)
_FTL_DECL_ICODE UBYTE FlaHynixCheckStatusPerPlane(UBYTE ubgFQLinkIndex, UBYTE ubMode);
_FTL_DECL_ICODE void flaReadFailRetryHynixTerminateFlow(UWORD uwFJobInfo);
#if Hynix3DV6
_FTL_DECL_ICODE void flaReadFailRetryHynix3DTerminateFlow(UBYTE ubPageSel);
#endif
_FTL_DECL_ICODE void flaReadFailRetryHynixTlcFlow(UBYTE ubRetryCount, UWORD uwFJobInfo, UBYTE *ubExternalBuffer);
#endif
#endif
#if YMTC
 _FTL_BURNER_ICODE UBYTE FlaYMTCCheckStatusPerPlane(UBYTE ubgFQLinkIndex, UBYTE ubMode);
#endif


_FTL_BURNER_ICODE void flaCheckWriteStatus (UBYTE ubgFQLinkIndex);

#if TSB_BICS4_SUPPORT
_FTL_DECL U32 flaGenRandSeed(UWORD ubPage);
#endif

//flafail.c
_FTL_BURNER_ICODE void flaCheckChannelFPUBusy(UBYTE ubgFQLinkIndex, UBYTE ubEnterSerialNumber, volatile REG_t *CheckChannelFREG, UBYTE ubFPUType);
_FTL_BURNER_ICODE void flaChangeWEDrivingSetting(UBYTE ubTimeoutChannel);
_FTL_BURNER_ICODE void flaFIPTimeoutErrorHandle(UBYTE ubCEIndex);
_FTL_BURNER_ICODE UBYTE flaCheckMTFirstReadFailL4KDuetoFakeSignoffMaskFailorNot(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaRebuildFQPhase(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaMarkFail(UBYTE ubgFQLinkIndex, UBYTE ubFailPhase, UBYTE ubDiffBurstBank);	// for program/erase FQ, ubDiffBurstBank代表實際fail的FQ跟傳入的ubgFQLinkIndex差幾個burstbank
_FTL_BURNER_ICODE void flaReadFailFixDQSREMismatch(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadDQSREMismatchHandling(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadFailRetryReFillL4KTable(UBYTE ubgFQLinkIndex);

_FTL_BURNER_ICODE UBYTE flaMicronCheckWriteStatus( UBYTE ubgFQLinkIndex, UBYTE ubStartPlane);
_FTL_BURNER_ICODE void flaReadFailRetryToshiba1ZnmSpecialFlow(void);
_FTL_BURNER_ICODE void flaReadFailRetryToshiba1ZnmSpecialTerminateFlow(void);
_FTL_BURNER_ICODE void flaReadFailRetrySandiskFlow(UBYTE ubRetryCount, UWORD uwFJobInfo, UBYTE * ubExternalBuffer);
_FTL_BURNER_ICODE void flaSandiskSet_LMFLGFIX_NEXT(UBYTE ubLMFLGFIX);

_FTL_BURNER_ICODE void flaReadFailRetrySandiskTerminateFlow(UWORD uwFJobInfo);
_FTL_BURNER_ICODE void flaReadFailRetryToshibaFlow(UBYTE ubRetryCount, UWORD uwFJobInfo);
_FTL_BURNER_ICODE void flaReadFailRetryToshibaTerminateFlow(UWORD uwFJobInfo);
_FTL_BURNER_ICODE void flaReadFailRetryToshibaBiCs2Flow(UBYTE ubRetryCount, UWORD uwFJobInfo, UBYTE * ubExternalBuffer);
_FTL_BURNER_ICODE void flaReadFailRetryToshibaBiCs2TerminateFlow(UWORD uwFJobInfo);
_FTL_BURNER_ICODE void flaModifyHBRetryQueue(  UBYTE ubRetryBlockType , UBYTE  ubRetryQindex);
_FTL_BURNER_ICODE void flaModifyHBRetryQueue_Last(  UBYTE ubRetryBlockType , UBYTE  ubRetryQindex );


_FTL_BURNER_ICODE void flafailFeatureOperation(UBYTE ubMode, UBYTE ubChannel, UBYTE ubFeatureAddr , UBYTE *ubData, UBYTE ubcheck);
_FTL_BURNER_ICODE void flaFailFeatureOpeartionYMTC(UBYTE ubChannel, UBYTE ubLUN ,UBYTE ubFeature, UBYTE * ubData);

_FTL_BURNER_ICODE UBYTE flaReadFailRetryCheckReadStatusInFail(UBYTE ubDepth, UBYTE ubALUIndex);
#if TLC
#if (BURNER && ENABLE_FORCE_L06B_TO_B0KB)
_FTL_BURNER_ICODE void flaReadFailRetryTriggerFPU(UBYTE ubD3LMU, UBYTE ubALUIndex);
#else
_FTL_BURNER_ICODE void flaReadFailRetryTriggerFPU(UBYTE ubD3LMU, UBYTE ubALUIndex);
#endif
#else
_FTL_BURNER_ICODE void flaReadFailRetryTriggerFPU(UBYTE ubALUIndex);
#endif
#if (BURNER && ENABLE_FORCE_L06B_TO_B0KB)
_FTL_BURNER_ICODE void flaReadFailRetryCheckSendCmdStatus(UBYTE ubDepth, UBYTE ubRecycleDepth, UBYTE ubClearINT, UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadFailRetryRebuildFQPhase(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadFailRetryResetIBFPTR(UBYTE ubChannel, UBYTE ubDepth);
_FTL_BURNER_ICODE UBYTE flaReadFailRetryGetL4KFailNo(UBYTE ubgFQLinkIndex,  UBYTE ubFailFrameNoInMT);
_FTL_BURNER_ICODE void flaReadSignoffmaskFail (UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadFailRetryForceIRDYtoReady(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE UBYTE flaReadFailRetryFPUCORRecoverData(UBYTE ubgFQLinkIndex, UBYTE ubDepth, UBYTE ubFailFrameNoInMT);
#else
_FTL_BURNER_ICODE void flaReadFailRetryCheckSendCmdStatus(UBYTE ubDepth, UBYTE ubRecycleDepth, UBYTE ubClearINT, UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadFailRetryRebuildFQPhase(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadFailRetryResetIBFPTR(UBYTE ubChannel, UBYTE ubDepth);
_FTL_BURNER_ICODE UBYTE flaReadFailRetryGetL4KFailNo(UBYTE ubgFQLinkIndex,  UBYTE ubFailFrameNoInMT);
_FTL_BURNER_ICODE void flaReadSignoffmaskFail (UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadFailRetryForceIRDYtoReady(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE UBYTE flaReadFailRetryFPUCORRecoverData(UBYTE ubgFQLinkIndex, UBYTE ubDepth, UBYTE ubFailFrameNoInMT);
#endif
#if Hynix
_FTL_BURNER_ICODE UBYTE Hynix_Calculate(UBYTE * ubHynix_RR_Table, UBYTE * ubHynix_OTP_Table, U32 ubOffset, U32 ubGroupCnt);
_FTL_BURNER_ICODE UBYTE flaGetHynixReadRetryTable(UBYTE ubCH, UBYTE ubCE, UBYTE ubDie, UBYTE ubSLCMode);
#endif
_FTL_BURNER_ICODE void NESGetIBFData(UBYTE ubIBFPtr, UBYTE ubDepth);
_FTL_BURNER_ICODE void NESSendC9Data(UBYTE ubNES_C9_Address, UBYTE mode, UWORD num_byte);
_FTL_BURNER_ICODE void NESSendC9Data_CMD10(void);
_FTL_BURNER_ICODE void NESSendC9Data_CMD20(void);
#if TLC
_FTL_BURNER_ICODE UBYTE flaReadFailHBRetry_SB(UBYTE ubRetry4K_IDX , UBYTE ubStartFrame ,UBYTE ubDepth , UBYTE ubPageSel , UBYTE ubCEIndex );
_FTL_BURNER_ICODE UBYTE flaReadFailHBRetry(void);
#else
_FTL_BURNER_ICODE UBYTE flaReadFailHBRetry(U32 ulIRAM_L4K_TABLE_OFFSET, UBYTE ubStartFrameNo, UBYTE ubFrameNum, UBYTE ubIsFastPage, UBYTE ubDepth, UBYTE ubChannel, UBYTE ubALUIndex);
#endif
_FTL_BURNER_ICODE void flaReadFailRetrySendResetCommand(UBYTE ubDepth);
_FTL_BURNER_ICODE U32 crc32_bitwise_f(U32 init, const void* data, U32 length);
_FTL_BURNER_ICODE void flaReadFailRetryReGenP4KCRC32(UBYTE ubgFQLinkIndex, UBYTE ubDepth);
_FTL_BURNER_ICODE UBYTE flaReadFailRetryRSFailFPUCORagain(UBYTE ubgFQLinkIndex, UBYTE ubDepth);
_FTL_BURNER_ICODE UBYTE flaReadFailRetryDummyFPUCOR(UBYTE ubgFQLinkIndex, UBYTE ubDepth, UBYTE ubFailFrameNoInMT);
_FTL_BURNER_ICODE void flaReadFailRetryReFillLCA(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadFailRetrySetUNCAddr(U32 ulAddr, U32 ulFailLBA);
_FTL_BURNER_ICODE UBYTE flaReadFailRetryCheckFirstReadFailL4KDuetoUNCorSignoffMaskFail(UBYTE ubgFQLinkIndex, UBYTE ubFailFrameNoInMT);
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
_FTL_BURNER_ICODE UBYTE flaMTCheckIsAutoPollTimeout(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaMTAutoPollTimeoutHandling(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void flaReadFailRetryAutoPollTimeoutHandling(UBYTE ubgFQLinkIndex, U32 ulFCE_ENB, UBYTE ubAbnormalLoc);
#endif

#if  (BURNER && ENABLE_FORCE_L06B_TO_B0KB)
_FTL_BURNER_ICODE void flaReadFailRetry(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void SoftBitCorrectSetDSPParam(UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectSetLLRTable(U32 LLRTablePtr, UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectToshibaFlow(UBYTE ubSoftBitReadData, UBYTE ubPageSel, UBYTE ubPreCondition) ;
#if MicronFlashOnly
_FTL_BURNER_ICODE void SoftBitCorrectMicronCoarseTuning(UBYTE ubCEIndex, UWORD uwPageIndex, UBYTE ubPageSel, UBYTE  ubFrameIndex, UWORD uwFUnit, U32 ulFEntry, UWORD uwFJobInfo, UBYTE ubDepth);
#endif
#else
_FTL_BURNER_ICODE void flaReadFailRetry(UBYTE ubgFQLinkIndex);
_FTL_BURNER_ICODE void SoftBitCorrectSetDSPParam(UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectSetLLRTable(U32 LLRTablePtr, UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectToshibaFlow(UBYTE ubSoftBitReadData, UBYTE ubPageSel, UBYTE ubPreCondition) ;
#if MicronFlashOnly
_FTL_BURNER_ICODE void SoftBitCorrectMicronCoarseTuning(UBYTE ubCEIndex, UWORD uwPageIndex, UBYTE ubPageSel, UBYTE  ubFrameIndex, UWORD uwFUnit, U32 ulFEntry, UWORD uwFJobInfo, UBYTE ubDepth);
#endif
#endif

#if TLC
#if Hynix
_FTL_BURNER_ICODE void SoftBitDataOut(void);
_FTL_DECL_ICODE void PIO_WDATA(UBYTE PIOData,volatile REG_t *BufP);
_FTL_BURNER_ICODE void SoftBitCorrectTLCFlow(UBYTE ubSoftBitReadData, UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectTLCSendReadCom(UBYTE CommandSet , UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectTLCSendXorCom( UBYTE DataOut );
_FTL_BURNER_ICODE void SoftBitCorrectTLCReadIndicator(UBYTE ubPageSel, UBYTE ubCmd_5D);
#elif YMTC
_FTL_BURNER_ICODE void SoftBitCorrectYMTCTLCFlow(UBYTE ubSoftBitReadData, UBYTE ubPageSel);
#else
_FTL_BURNER_ICODE void SoftBitCorrectToshibaTLCFlow(UBYTE ubSoftBitReadData, UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectTLCPreconditionFlow(void);
_FTL_BURNER_ICODE void SoftBitCorrectTLCSendReadCom(UBYTE CommandSet , UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectSandiskBics2SReadCom(UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectTLCSendXorCom( UBYTE SoftBitDataOut );
_FTL_BURNER_ICODE void SoftBitCorrectSandiskBics2SendXorCom( UBYTE SoftBitDataOut );
_FTL_BURNER_ICODE void SoftBitCorrectTLCReadIndicator(UBYTE ubPageSel, UBYTE ubCmd_5D);
_FTL_BURNER_ICODE void SoftBitCorrectTLCReadIndicator_Bics3(UBYTE ubPageSel , UBYTE ubMode);
_FTL_BURNER_ICODE void SoftBitCorrectSanDiskTLCFlow(UBYTE ubSoftBitReadData, UBYTE ubPageSel, UBYTE ubold_method);
_FTL_BURNER_ICODE void SoftBitCorrectSanDiskBics4Flow(UBYTE ubSoftBitReadData, UBYTE ubPageSel);
_FTL_BURNER_ICODE void SoftBitCorrectSanDiskBics2Flow(UBYTE ubSoftBitReadData, UBYTE ubPageSel, UBYTE ubReadData, UBYTE ubReadHB);
_FTL_BURNER_ICODE void SoftBitCorrectSanDiskBics2AdjustRetryData(UBYTE ii, UBYTE ubPageSel, UBYTE *ubRetryData, UBYTE ubReadData);
_FTL_BURNER_ICODE void SoftBitCorrectBiCS2TLCFlow(UBYTE ubSoftBitReadData, UBYTE ubPageSel, UBYTE ubold_method);
_FTL_BURNER_ICODE void flaReadFailRetryBiCS2SBTerminateFlow(UBYTE ubLen);
#endif
#endif
_FTL_BURNER_ICODE UBYTE SoftBitCorrectFLHShiftReadDataFPU(UBYTE ubCEIndex, UWORD uwPageIndex, UBYTE ubPageSel, UBYTE ubReadFrameIndex, UBYTE ubStartFrame, UWORD uwFUnit, U32 ulFEntry, UWORD uwFJobInfo, UBYTE ubDepth);
_FTL_BURNER_ICODE void GenSoftBit(void);
_FTL_BURNER_ICODE UBYTE SmoothRankFW( UBYTE RankSum );
_FTL_BURNER_ICODE UBYTE FLHSoftBitDecode(UBYTE ubPageSel, UBYTE ubDepth);
_FTL_BURNER_ICODE UBYTE SoftBitCorrect(U32 ulDMAAddr, UBYTE ubCEIndex, UWORD uwFUnit, U32 ulFEntry, UWORD uwFJobInfo, U32 ulReadFail_4KMAP_SBused, UBYTE ubDepth);
_FTL_BURNER_ICODE UBYTE RS_FPU_Read(U32 ulDMAAddr, UBYTE ubgFQLinkIndex, U32 ulPlaneIndex, UWORD uwRSPlaneIndex, UBYTE ubUncCheckInRSTwoCh, UBYTE ubDepth);
_FTL_BURNER_ICODE U32 RS_Get_FrameStartPhyPlaneIndex(U32 ulTargetPlaneIndex, UWORD uwRSGroupMaxEncodePlaneNum, UBYTE ubIdleTagNum);
_FTL_BURNER_ICODE UWORD RS_Get_RSGroupTagFailMapIndex(UBYTE ubgFQLinkIndex);

#if (SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1 && !BURNER)
_FTL_DECL_ICODE UBYTE RS_FPU_Read_InitInfo_Parity(U32 ulDMAAddr, U32 ulSpareAddr, U32 ulTargetPlaneIndex, UWORD uwRSPlaneIndex, UBYTE ubMTerrorCheckInRSTwoCh, UBYTE ubDepth);
#endif
#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
#if USE_NEW_PARITY_PAGE_CHECKING==1
_FTL_DECL_ICODE UBYTE ftlCheckPlaneParity( U32 ulPlaneIndex );
#else
_FTL_DECL_ICODE UBYTE ftlPlaneIsParity(U32 ulPlaneIndex);
#endif
_FTL_DECL_ICODE void Bunch_RS_Recover(UBYTE ubgFQLinkIndex, UBYTE ubDepth);
#endif
#if ENABLE_DEBUG_FREE_BLOCK_TABLE==1
_FTL_DECL_ICODE void ftlCheckFreeBlockTable(UBYTE ubBackup);
#endif
_FTL_BURNER_ICODE void RS_Flush_MultiPlaneProgram(UBYTE ubDepth);
_FTL_BURNER_ICODE void RS_CheckEncodeDPageMap(UBYTE FQRWTableUnit, UBYTE ubRSFrameIndex, UBYTE ubIdleTagNum, UWORD uwRSGroupMaxEncodePlaneNum, UWORD uwDataParityDummyPlanesNum, UBYTE ubPARITY_GROUP_NUM);
_FTL_BURNER_ICODE void RS_Get_RSPlaneIndex(UBYTE ubgFQLinkIndex);
#if ENABLE_DEBUG_RS_VERIFY_DATA
_FTL_BURNER_ICODE void Debug_RS_VERIFY_DATA(UBYTE ubgFQLinkIndex, UBYTE ubMTerrorCheckInRSTwoCh, UBYTE ubSerialNumber, UBYTE ubDepth);
#endif
_FTL_BURNER_ICODE void RS_Recover(UBYTE ubgFQLinkIndex, UBYTE ubDepth);
#if (BURNER)
_FTL_DECL void FlaFlashDeRandomize(U32 * ulSourceBuferPtr, U32 * ulResultBuferPtr, UBYTE ubFrame );
#endif

_FTL_DECL_ICODE void FlaFlashDeRandomize(U32 * ulSourceBuferPtr, U32 * ulResultBuferPtr, UBYTE ubFrame );
#if ENALBE_DEBUG_NES_CHANGE_ERROR_SETTING
_FTL_DECL_ICODE void flaNandEmulatorVenderFPUErase(UWORD uwVenderUnit, UBYTE ubFeature, UBYTE ubDepth);
#endif
_FTL_BURNER_ICODE void flaEraseFailHandle(UBYTE ubgFQLinkIndex);
#if MicronFlashOnly
_FTL_BURNER_ICODE UBYTE Micron_CheckPage(UWORD uwPageIndex);
#endif

//============================================================================================================
//RDT.c   Lin
//============================================================================================================
_FTL_DECL_ICODE void Vender_RDT_GetSDRLog(void);
_FTL_DECL_ICODE void Vender_RDT_GetTimeStampLog();
_FTL_DECL_ICODE void Vender_RDT_GetBadBlkLog();
//============================================================================================================
//ata cmd
//============================================================================================================
_FTL_DECL_ICODE  void NoOpCmd(void);
_FTL_DECL_ICODE  void AtaIdentifyDrive(void);
_FTL_DECL_ICODE  void NotImplement(void);
_FTL_DECL_ICODE  void ReCalibrate(void);
_FTL_DECL void WriteSectors(void);
_FTL_DECL void ReadSectors(void);
_FTL_DECL void DoneEC_Check(UBYTE ubEnableSwitchTask);
_FTL_DECL void MoveLink(UBYTE ubMoveLink);
_FTL_DECL void CheckID(void);
_FTL_DECL_ICODE void FlushSDR(void);
_FTL_DECL_ICODE  void ReadBuffer(void);
_FTL_DECL_ICODE  void WriteBuffer(void);
_FTL_DECL_ICODE  void SetFeature(void);
_FTL_DECL_ICODE void RWBuffer(U32 ulXferCnt);
_FTL_DECL_ICODE void WriteSectorFUA(void);
#if (DEBUG_ULTRAMLC_POWER_FAIL && ENABLE_PFAIL)
_FTL_DECL_ICODE  void Trim(void);
#else
_FTL_DECL void Trim(void);
#endif
_FTL_DECL_ICODE  void ReadVerify(void);
_FTL_DECL_ICODE  void Seek(void);
_FTL_DECL_ICODE  void Diagnostics(void);
_FTL_DECL_ICODE  void InitDrvPara(void);
_FTL_DECL_ICODE  void SetStandbyTimer (void);
_FTL_DECL_ICODE  void Standby(void);
_FTL_DECL_ICODE  void Idle(void);
_FTL_DECL_ICODE  void ChkPwrMode(void);
_FTL_DECL_ICODE  void Sleep(void);

_FTL_DECL_ICODE  void SetMultiple(void);
_FTL_DECL_ICODE  void ReadMultiple(void);
_FTL_DECL_ICODE  void WriteMultiple(void);
#if (DEBUG_ULTRAMLC_POWER_FAIL && ENABLE_PFAIL)
_FTL_DECL_ICODE void FlushCache(void);
#else
_FTL_DECL void FlushCache(void);
#endif
_FTL_DECL_ICODE void FlushCacheCmd(void);
_FTL_DECL_ICODE void WriteUncorrectable(void);
_FTL_DECL_ICODE void Zero(void);
_FTL_BURNER_ICODE void DLMC_ErrorDetect(UBYTE ubErrorCode);
_FTL_DECL_ICODE void Download_MicroCode(void);
_FTL_DECL_ICODE void DLMC_InitialSetting();
_FTL_DECL_ICODE void DLMC_UpdateSystemInfo(U32 ulUpdateSystemInfoMap);
_FTL_DECL_ICODE void SDR_RW_DMA(void);      //C.Y.
_FTL_DECL_ICODE void RWBuffer_forSMARTLog(U32 ulXferCnt, U32 ulOffset);

_FTL_DECL_ICODE void Uart_Tx_Data(unsigned char ubData);
_FTL_DECL_ICODE void Uart_Tx_DataHex(UBYTE ubData);
_FTL_DECL_ICODE void UartWordHex(UWORD uwData);
_FTL_DECL_ICODE void UartLongHex(U32 ulData);
#if (DEBUG_ULTRAMLC_POWER_FAIL && ENABLE_PFAIL)
_FTL_DECL_ICODE void UartLLongHex(ULLONG uoData);
#else
_FTL_DECL_ICODE void UartLLongHex(ULLONG uoData);
#endif
_FTL_DECL_ICODE void UartString(char *ubptr);
_FTL_DECL void WakeUpTask(void);
_FTL_DECL void IdleState(void);

_FTL_DECL_ICODE void TrimAll(void);
#if (DEBUG_ULTRAMLC_POWER_FAIL && ENABLE_PFAIL)
_FTL_DECL_ICODE void SaveAtaConfig(void);
#else
_FTL_DECL void SaveAtaConfig(void);
#endif

//============================================================================================================
//vender cmd
//============================================================================================================
#if (RDT_BURNER)
_FTL_BURNER_ICODE void FindRDTLogBlock(U32 ulSramAddr, U32 ulTargetLCA, U32 *ulFoundUnit, U32 *ulFoundPB);
_FTL_BURNER_ICODE UBYTE ReadRDTLogBlock(U32 ulSramAddr, U32 ulUnit, U32 ulPB, U32 ulPage);
_FTL_BURNER_ICODE void Vender_RDT_GetBadBlkLog();
_FTL_BURNER_ICODE void Vender_RDT_GetTimeStampLog();
_FTL_BURNER_ICODE void Vender_RDT_GetSDRLog();
#endif
#if Hynix
	_FTL_DECL void FlaIssueHynixDaCmd(U32 ch, U32 ce, U32 cmd);
#endif
_FTL_DECL_ICODE void Vender_Isp_Prog();
_FTL_BURNER_ICODE void Vender_Write124Byte_Reg(void);
_FTL_BURNER_ICODE void Vender_Read124Byte_Reg(void);
#if(BURNER)
_FTL_DECL_ICODE  void Vender_Isp_Jump(UBYTE ubMode);
#else
_FTL_DECL void Vender_Isp_Jump(UBYTE ubMode);
#endif
_FTL_BURNER_ICODE void Vender_GetFlashID(unsigned char ubFlashDevice);
_FTL_DECL_ICODE  void Vender_ReadSystemInfo(void);
_FTL_DECL void Vender_Simulate_Power_Off(void);
_FTL_DECL_ICODE void Vender_Set_PowerCycle(UBYTE ubType);
_FTL_DECL  void Vender_Reset(UWORD uwWRBLK_temp,UBYTE ubDDT_temp, UWORD uwMSG_temp);
//_FTL_DECL_ICODE  void Vender_Reset_nonTxData(void);
_FTL_BURNER_ICODE void Vender_ScanFlashID(void);
_FTL_BURNER_ICODE void Vender_Direct_Read_Info(void);
_FTL_DECL_ICODE void Vender_ClearSMART(void);
_FTL_DECL_ICODE void Vender_GetBackGroundStatus(void);
_FTL_DECL_ICODE void Vender_Trim_UART_JTAG(void);
_FTL_DECL_ICODE void Vender_Detect_Thermal_Sensor(void);
#if(!RDT)
_FTL_DECL void ISP_JUMP(UBYTE ubMode);
_FTL_DECL_ICODE  UBYTE ISP_Prog_DummyData(U32 ulUnitIndex, UWORD uwPlane,  UBYTE ubCEIndex, UBYTE ubL4KNum);
_FTL_DECL_ICODE  void M_SETOP(UBYTE ubOP, U32 ulValue, U32 ulValue2, U32 ulDes, UBYTE ubType, U32 ulOPAddr);
_FTL_DECL_ICODE  UWORD FlaGenerateSetFeatureOP_Code(U32 ulStartAddr);
_FTL_DECL_ICODE  UWORD FlaGenerateInitialDRAM_OP_Code(U32 ulStartAddr);
_FTL_DECL_ICODE  UBYTE FlaPrepareHeader_Code(UBYTE ubProgramAgain);
/*
_FTL_DECL_ICODE  void ftlSet16BitMode();
_FTL_DECL_ICODE  void ftlSet8BitMode();
*/
_FTL_BURNER_ICODE  void Vender_Set_FlashMode(void);
_FTL_BURNER_ICODE  void Vender_CheckLaterBadReuse(void);
_FTL_BURNER_ICODE  void Vender_GPIO_Setting(void);
_FTL_BURNER_ICODE void Vender_GPIO_Erase(UBYTE ubMode);
_FTL_BURNER_ICODE void Vender_GPIO_Offline(void);
_FTL_BURNER_ICODE void Vender_InitialPadAndDrive(void);
_FTL_BURNER_ICODE void Vender_Scan_SDLL(void);
_FTL_BURNER_ICODE  void Vender_Cache_Program(void);
_FTL_BURNER_ICODE  void Vender_Cache_Read(void);
#if (MicronFlashOnly || Hynix || (UFS && TLC_BICS2))
_FTL_BURNER_ICODE  void Vender_CheckTSBUID();
#else
_FTL_DECL_ICODE  void Vender_CheckTSBUID();
#endif
_FTL_BURNER_ICODE void Vender_MaxBad_per_Plane();
_FTL_BURNER_ICODE void Vender_ReadParameterPage(void);
_FTL_BURNER_ICODE void Vender_ReadParameterPage_CE(void);
_FTL_BURNER_ICODE void Vender_ReadVTFailLog(void);
_FTL_BURNER_ICODE void EraseCountPerCEDie(void);
_FTL_BURNER_ICODE void CheckBadPerCE(UBYTE ubMode);
_FTL_BURNER_ICODE void CheckBadForAllCE(UBYTE ubMode);
_FTL_BURNER_ICODE  void BurnerCheckEarlyBad(UBYTE ubMode);
_FTL_BURNER_ICODE void SMART_Vender(void);

_FTL_DECL_ICODE  void Vender_Send_Seed();
_FTL_DECL_ICODE  void Vender_Prog_RAM_Whole();
_FTL_BURNER_ICODE UBYTE Load_RUT_EC_Table(void);
_FTL_DECL_ICODE void Vender_Trim_Set_PIO(void);
_FTL_DECL_ICODE void Vender_TrimAll_Test(void);
_FTL_BURNER_ICODE  void Vender_Output_Trimming_Table(void);
_FTL_BURNER_ICODE  void Vender_Direct_Read_Flash(void);
_FTL_BURNER_ICODE  void Vender_Preformat(void);
_FTL_BURNER_ICODE  void Vender_Erase_All_Blocks(void);
_FTL_BURNER_ICODE  UBYTE Vender_SaveVenderCmdHistory(void);
_FTL_BURNER_ICODE  void Vender_Direct_Erase_Block(void);
_FTL_BURNER_ICODE  void Vender_Direct_Write_Page(void);
_FTL_BURNER_ICODE  void Vender_Direct_RawRead_TSB (void);

#if BURNER
_FTL_BURNER_ICODE  void Vender_Direct_RawRead (void);
_FTL_BURNER_ICODE  void Vender_Direct_Write_DBT16KPage (void);
#endif
//_FTL_DECL_ICODE void Vender_Isp_Jump_nonTxData(void );
_FTL_DECL_ICODE  void Vender_ReadSystemInfo_Detail(void); // yk.
_FTL_DECL_ICODE void Vender_Isp_Verify(UBYTE ubMode, UBYTE ubCodeBlockNo);
_FTL_DECL_ICODE void Vender_SetReadSRAMParameter(void);
_FTL_DECL_ICODE void Vender_Read_SRAM(void);
_FTL_DECL_ICODE void Vender_Write_SRAM(void);

_FTL_DECL_ICODE void Vender_Direct_Write_Info(UBYTE ubIsDLMC,U32 ulSourceAddress);
_FTL_DECL_ICODE void Vender_BusyTime(void);
_FTL_DECL_ICODE void Vender_DisableFreezeLock(void);
_FTL_DECL_ICODE void Vender_HW_SDR_Read(void);
_FTL_DECL_ICODE void Vender_HW_SDR_Write(void);

_FTL_BURNER_ICODE void Vender_CheckRetryTable(void);
_FTL_DECL_ICODE void Vender_LoadBurnerIcode(void);
_FTL_DECL_ICODE void Vender_VerifyBurnerIcode(void);


_FTL_BURNER_ICODE void Vender_Send_Handshake_Request();
_FTL_BURNER_ICODE void Vender_Receive_Encryption_Data();
_FTL_BURNER_ICODE void Vender_Send_Encryption();


#if (BURNER&&ENABLE_VENDER_READ_VTH)
_FTL_BURNER_ICODE void Vender_FLH_Distribution_Read_Setting(void);
_FTL_BURNER_ICODE void Vender_FLH_Distribution_Read(void);
#endif
_FTL_BURNER_ICODE void Vender_BadBlockInfo(void);

_FTL_DECL_ICODE void FillWholeDisk(U32 ulPattern);

#if PH_SQL_FUNC
_FTL_BURNER_ICODE void Vender_Mode_Cmd_VNandTool(UBYTE ubMode);
#endif
_FTL_BURNER_ICODE void Vender_ReadFLHBlockMappingInfo(void);
_FTL_BURNER_ICODE void Vender_GetErrorInfoBlock(void);
#if 0
_FTL_DECL_ICODE void Vender_CopyBack(void);
#endif
_FTL_DECL_ICODE void Vender_D1D3_VerifyECC(UWORD uwFUnit/*D3*/, UWORD uwFD1Unit/*D1*/, UWORD uwStartProgramOrderIndex, UWORD uwEndProgramOrderIndex, UBYTE ubCopyBackMode, UBYTE ubVerifyMode);
_FTL_BURNER_ICODE void Vender_D1D3_VerifyECC_FirstFrame(UWORD uwFUnit/*D3*/, UWORD uwFD1Unit/*D1*/, UBYTE ubVerifyMode);

_FTL_DECL_ICODE void Vender_UnlockCmd();
_FTL_DECL_ICODE void CheckVenderLock();
_FTL_DECL_ICODE UBYTE UnlockCmdPass1Time();
#if EnableProductHistory
_FTL_BURNER_ICODE void Vender_ReadPH();
_FTL_BURNER_ICODE void Vender_WritePH();
#endif
_FTL_BURNER_ICODE void Vender_SourceUint_To_TargetUint(UWORD uwFUnit_D3Target/*D3 Targe*/, UWORD uwFD1Unit/*D1 Source*/, UWORD uwFUnit_D3Source/*D3 Source*/, UWORD uwStartProgramOrderIndex, UWORD uwEndProgramOrderIndex, UBYTE ubCopyBackMode);
#if (MicronFlashOnly && B0KB)
#if (BURNER)
_FTL_BURNER_ICODE void B0KB_CheckPage(CB_Info_t *FQI_CB_Info);
#else
_FTL_DECL_ICODE void B0KB_CheckPage(CB_Info_t *FQI_CB_Info);
#endif
#endif

#endif
#if ((!BURNER) && TLC)
_FTL_DECL_ICODE void Get_D3_WL(CB_Info_t *FQI_CB_Info);
_FTL_DECL_ICODE void Get_First_Foggy_Fine(CB_Info_t *FQI_CB_Info);
#endif

_FTL_DECL_ICODE UBYTE I2C_WriteRead_LedIC(UBYTE ubMode, UBYTE ubSubAddr, UBYTE *ubValue);
_FTL_DECL_ICODE void Vender_I2C_Write_LedIC();
_FTL_DECL void Vender_I2C_Read_LedIC();
_FTL_DECL_ICODE void Vender_HostRemainSectorCount();
_FTL_DECL void Vender_TriggerHostData();
_FTL_DECL void Vender_TriggerHostData_31();

//============================================================================================================
//main.c
//============================================================================================================
_FTL_DECL void M_SwitchTask(void);
_FTL_DECL void HandleStopRW(UBYTE ubStopMode);
_FTL_DECL void SaveRSparity(UBYTE ubReEnableRSPreSaveLoad);
_FTL_DECL_ICODE void InitFPUEntry(void);
_FTL_DECL_ICODE void FPU_tableset(void);
_FTL_DECL_ICODE void SetInitialFailID(UWORD uwErrorID, U32 ulErrorInfo);
_FTL_DECL_ICODE void SetFailCEtoSystemInfo(UBYTE ubCEIndex);
_FTL_DECL_ICODE void ChangeMediaSize(void);
_FTL_DECL_ICODE void InitMTSetTemplate(UBYTE ubLength, UBYTE ubClear, U32 ulArg0x00, U32 ulArg0x04, U32 ulArg0x1C, U32 ulZipMap, U32 ulArg0x24, U32 ulArg0x28);
_FTL_DECL_ICODE void FLH_LLR_Table_Init(void);
_FTL_DECL_ICODE void HandleSMARTSelfTest();
_FTL_DECL_ICODE void InitFlashRegister(UBYTE ubInitIRAM);
_FTL_DECL_ICODE void Sata_InterfaceReg_Init(void);
_FTL_DECL_ICODE  void InitVariable(void);
_FTL_DECL_ICODE void CalculateLastDataPlaneIndex(UBYTE btFastPage);
_FTL_DECL_ICODE  void InitFTLVariable(void);
_FTL_DECL_ICODE  void InitCommonRegister(void);
_FTL_DECL_ICODE void Init_Reset();
_FTL_DECL_ICODE void InitVaribleBeforeLoadSystemBlock(void);
_FTL_DECL_ICODE void HandleCodeSet();
_FTL_DECL_ICODE void HandleSystemDBTSet(void);
_FTL_DECL_ICODE void TransformSystemBlock(U32 ulAddr);
_FTL_DECL_ICODE UBYTE UpdateSystemBlock(UBYTE ubReturnFailInfo, U32 ulSystemBlockAddr);
_FTL_DECL_ICODE	 void ScanSystemFreeSet(UBYTE ubMode);
_FTL_DECL_ICODE	 void LoadSystemBlock();
_FTL_DECL void APU_Reset(void);
_FTL_DECL	 void SYS_Interrupt_Init();
_FTL_DECL	void gating_clock(UBYTE ubMode, U32 ulIdleCount);
_FTL_DECL void JTagSetting();
_FTL_DECL	void I2C_beforePD();
_FTL_DECL	void I2C_Write_ConfigNOtrig(unsigned char ubAddrSelect, unsigned char ubGroupsel, unsigned char ubBufSel,unsigned char ubOPcode ,unsigned char ubValue);
_FTL_DECL	void Idle2LPM2();
_FTL_DECL	void LPM_SaveMore();
_FTL_DECL	void GPIO_DEVSLP_ISR();
_FTL_DECL_ICODE   void ResetCmdIn();
_FTL_DECL   void Enable_SATA_Interrupt();
_FTL_DECL   void InitSATARegister();
_FTL_DECL   void Uart_InitBaudRate(unsigned int ulPClk, unsigned int ulBaudRate);

///added by Victor
_FTL_DECL_ICODE	 void ResetRegFromIDT();

//============================================================================================================
//Smart cmd
//============================================================================================================
_FTL_DECL_ICODE void SetSmartLog_CmdDone(void);
_FTL_DECL_ICODE ULLONG GetRTT_s(void);
_FTL_DECL_ICODE void IdentifyDeviceDataLog_Page0_LogPage(void);
_FTL_DECL_ICODE void IdentifyDeviceDataLog_Page2_Capacity(void);
_FTL_DECL_ICODE void IdentifyDeviceDataLog_Page3_SupportedCapabilities(void);
_FTL_DECL_ICODE void IdentifyDeviceDataLog_Page4_CurrentSettings(void);
_FTL_DECL_ICODE void IdentifyDeviceDataLog_Page5_ATA_Strings(void);
_FTL_DECL_ICODE void IdentifyDeviceDataLog_Page6_Security(void);
_FTL_DECL_ICODE void IdentifyDeviceDataLog_Page8_Serial_ATA(void);
#if(!RDT)
_FTL_DECL_ICODE void GetEraseCount(FTarget_t *TargetMax,FTarget_t *TargetMin, UBYTE* ubTaskMax,UBYTE* ubTaskMin);
_FTL_DECL_ICODE  void SMART(void);
_FTL_DECL_ICODE  void SMART_Read_Data(void);
_FTL_DECL_ICODE  void SMART_Read_Threshold(void);
_FTL_DECL_ICODE UBYTE Get_SMART_Attribute_Value_Worst(UWORD  uwValueWorst_MP, SMART_BADBLOCK_INFO BadBlockInfo, SMART_ERASECOUNT_INFO EraseCountInfo, SMART_FAILCOUNT_INFO FailCountInfo, UBYTE ubTemperature);
_FTL_DECL_ICODE ULLONG Get_SMART_Attribute_Data(UWORD  uwData_MP, SMART_BADBLOCK_INFO BadBlockInfo, SMART_ERASECOUNT_INFO EraseCountInfo, SMART_FAILCOUNT_INFO FailCountInfo, UBYTE ubTemperature);
_FTL_DECL_ICODE void Set_SMART_Attribute_Entry(SMART_Attribute_Entry* AttributeArray, SMART_BADBLOCK_INFO* BadBlockInfo);
_FTL_DECL_ICODE void CalculateFailAndBadCountInfo(SMART_FAILCOUNT_INFO *FailCountInfo, SMART_BADBLOCK_INFO *BadBlockInfo);
_FTL_DECL_ICODE void CalculateEraseCountInfo(SMART_ERASECOUNT_INFO *EraseCountInfo);
_FTL_DECL_ICODE UBYTE SMART_Return_Status(void);
_FTL_DECL_ICODE  void SMART_Read_Log(void);
_FTL_DECL_ICODE  void Read_LOG_EXT(void);
_FTL_DECL_ICODE  void Write_LOG_EXT(void);
_FTL_DECL_ICODE  void GetLogSector(unsigned short uw_LogAddr);
_FTL_DECL_ICODE  void SMART_Write_Log(void);
_FTL_DECL_ICODE  void Load_LogSector(UWORD uw_LogSec);
_FTL_DECL_ICODE  void Save_LogSector(UWORD uw_LogSec );
_FTL_DECL_ICODE  void RecordCmdLog(void);
_FTL_DECL_ICODE  void SummaryErrLog(void);
_FTL_DECL_ICODE void SMART_SuspendOffLineDoing();
_FTL_DECL_ICODE void SMART_StopOffLineDoing();
_FTL_DECL_ICODE  void SMART_Off_Line(void);
_FTL_DECL_ICODE  void SMART_RealScanLBA();
_FTL_DECL_ICODE UBYTE WriteLogCheckSum(UWORD uw_LogSec);
_FTL_DECL_ICODE  void ReadHostLog(UWORD uw_LogSec);
_FTL_DECL_ICODE  void WriteHostLog(UWORD uw_LogSec);
_FTL_DECL_ICODE ULLONG GetRTT_micros(void);
_FTL_DECL_ICODE ULLONG GetRTT_milis(void);
_FTL_DECL_ICODE ULLONG GetRTT_m(void);
_FTL_DECL_ICODE ULLONG GetRTT_h(void);
_FTL_DECL_ICODE UBYTE I2C_Temperature(UBYTE ubMode);
_FTL_DECL_ICODE void SetSmartLog_CmdRecv(UBYTE );
_FTL_DECL_ICODE void PhyEventCountersLog(void);
_FTL_DECL_ICODE void CFALog(UWORD uwLogPage);
_FTL_DECL_ICODE void CFALog_Page0_LogPage(void);
_FTL_DECL_ICODE void CFALog_Page1_PerformanceControl(void);
_FTL_DECL_ICODE void CFALog_Page2_PowerDescription(void);
_FTL_DECL_ICODE void CFALog_Page3_PowerProfile(void);
_FTL_DECL_ICODE void DeviceStatisticsLog(UWORD uwLogPage);
_FTL_DECL_ICODE void DeviceStatisticsLog_Page0_LogPage(void);
_FTL_DECL_ICODE void DeviceStatisticsLog_Page1_General(void);
_FTL_DECL_ICODE void DeviceStatisticsLog_Page4_GeneralErrors(void);
_FTL_DECL_ICODE void DeviceStatisticsLog_Page5_Temperature(void);
_FTL_DECL_ICODE void DeviceStatisticsLog_Page6_Transport(void);
_FTL_DECL_ICODE void DeviceStatisticsLog_Page7_SSD(void);
_FTL_DECL_ICODE void IdentifyDeviceDataLog(UWORD uwLogPage);
_FTL_DECL_ICODE void SMART_Self_Merge();
_FTL_DECL_ICODE void RTT1_SetTime_ReStart();
_FTL_DECL_ICODE void RTT1Clock_Init(UBYTE ubTimeScale);
#if (Hynix3DV6 || Hynix3DV5)
_FTL_DECL_ICODE UBYTE Hynix3D_SetParameter(UBYTE ubAddr, UBYTE ubVal);
_FTL_DECL_ICODE void Hynix3D_GetParameter(UBYTE ubAddr, UBYTE *ubData);
_FTL_DECL_ICODE UBYTE PollingStatusByDie(UBYTE ubDiePara);
#endif

#endif
//============================================================================================================
//Security cmd
//============================================================================================================
_FTL_DECL_ICODE void InitSec(UBYTE IsDLMCPowerOnReset);


#if(!RDT)
_FTL_DECL_ICODE void Security(void);
_FTL_DECL_ICODE void SecuritySetPass(void);
_FTL_DECL_ICODE void SecurityUnlock(void);
//_FTL_DECL void SecurityFreezeLock(void);
//_FTL_DECL void SecurityErasePre(void);
_FTL_DECL_ICODE void SecurityDisPass(void);
_FTL_DECL_ICODE void SecurityEraseUnit(void);
_FTL_DECL_ICODE void SecurityEraseUserData(void);
#endif

//============================================================================================================
//HPA cmd host protect area command
//============================================================================================================
_FTL_DECL_ICODE void InitHPA(unsigned char ub_Reset);
#if(!RDT)
_FTL_DECL_ICODE void SetMax(void);
_FTL_DECL_ICODE void ReadNativeMax(void);
_FTL_DECL_ICODE void SetMaxAddress(unsigned char ub_ExtCmd);
_FTL_DECL_ICODE void SetMaxSetPass(void);
_FTL_DECL_ICODE void SetMaxLock(void);
_FTL_DECL_ICODE void SetMaxUnlock(void);
_FTL_DECL_ICODE void SetMaxFreeze(void);
#endif
//============================================================================================================
//AMAX command
//============================================================================================================

_FTL_DECL_ICODE void SetAccessibleMaxAddress(void);
#if(!RDT)
_FTL_DECL_ICODE void AccessibleMaxAddress(void);
#endif

//============================================================================================================
//DCO cmd
//============================================================================================================
_FTL_DECL_ICODE void InitDCO(unsigned char ub_Reset);
#if(!RDT)
_FTL_DECL_ICODE void DeviceConfig(void);
_FTL_DECL_ICODE void DCO_Restore(void);
_FTL_DECL_ICODE void DCO_Lock(void);
_FTL_DECL_ICODE void DCO_Identify(void);
_FTL_DECL_ICODE void DCO_Set(void);
#endif
//============================================================================================================
//Sanitize cmd
//============================================================================================================
_FTL_DECL_ICODE void InitSanitize(UBYTE ubMode);
_FTL_DECL_ICODE void Sanitize_NormalOutput(void);
#if(!RDT)
_FTL_DECL_ICODE void Sanitize(void);
_FTL_DECL_ICODE void Sanitize_Freeze(void);
_FTL_DECL_ICODE void Sanitize_AntiFreeze(void);
_FTL_DECL_ICODE void Sanitize_Status(void);
_FTL_DECL_ICODE void Sanitize_Overwrite(void);
_FTL_DECL_ICODE void Sanitize_Overwrite_Offline(void);
#endif
//============================================================================================================
//other function
//============================================================================================================
_FTL_DECL_ICODE void AP_Entry(void);
_FTL_DECL_ICODE void ReturnErr(void);
_FTL_DECL_ICODE void CalCheckSum(void);
_FTL_DECL_ICODE void Load_AtaIdenDrv(void);
#if(!RDT)
_FTL_DECL_ICODE UBYTE CheckCheckSum(void);
#endif

//==================================BURNER==========================================================================


#if Hynix
_FTL_DECL_ICODE void flaDisableSecretMode();
_FTL_DECL_ICODE void FlaIssueHynixSDPCmd(U32 ch, U32 ce, U32 cmd);
#endif
_FTL_DECL_ICODE UBYTE ftl_Erase_Single_Plane(U32 ulUnitIndex, UBYTE ubPlane, UBYTE ubCEIndex, UWORD uwFJobInfo);
#if 0
#if (TLC||ENABLE_ULTRAMLC)
_FTL_DECL_ICODE UBYTE ftl_Direct_Erase_Unit(U32 ulUnitIndex, UBYTE ubBurst, UWORD uwFJobInfo);
 #if ENABLE_ULTRAMLC
 #define ftlA2EraseSingleUnit(unit, ubBurst) ftl_Direct_Erase_Unit(unit, ubBurst, BIT_FJOBI_FASTPAGE | BIT_FJOBI_ERASE_FAIL_RESET)
 #endif
#else
_FTL_DECL_ICODE void ftlA2EraseSingleUnit(U32 ulUnitIndex, UBYTE ubBurst);
#endif
#endif
_FTL_DECL_ICODE void ftlInitAtaCfg(void);
_FTL_DECL_ICODE UBYTE CheckPWICExist(void);
_FTL_DECL_ICODE UBYTE ftlReadSinglePlane(U32 ulUnitIndex, U32 ulPlane,  UBYTE ubCEIndex, U32 ulSramAddr, U32 LCA,   UWORD uwFJobInfo,UBYTE ubL4KNum, UBYTE ubSetEntry, UBYTE ubBurstIndex);
_FTL_DECL_ICODE UBYTE ftlWriteSinglePlane(U32 ulUnitIndex, U32 ulPlane,  UBYTE ubCEIndex, U32 ulSramAddr, U32 LCA, U32 FW, UWORD uwFJobInfo,UBYTE ubL4KNum, UBYTE ubSetEntry, UBYTE ubBurstIndex);
_FTL_BURNER_ICODE UBYTE ftlWriteSinglePlaneTLC(U32 ulUnitIndex, U32 ulPlane,  UBYTE ubCEIndex, U32 ulSramAddr, U32 LCA, U32 FW, UWORD uwFJobInfo,UBYTE ubL4KNum, UBYTE ubSetEntry, UBYTE ubBurstIndex);

#if MicronFlashOnly
_FTL_BURNER_ICODE U32 ftlGoodBlock(U32 ulUnitIndex, UBYTE ubPlaneBank, U32 ulSramAddr, U32 ulErasePage, UBYTE ubMode);
#else
_FTL_BURNER_ICODE U32 ftlGoodBlock(U32 ulUnitIndex, UBYTE ubPlaneBank, U32 ulSramAddr, U32 ulErasePage);
#endif
#if ENABLE_SANDISK_CHECKEARLYBADBYPROG
_FTL_BURNER_ICODE UBYTE ftlGoodBlock_SandiskCheckByProgFail(U32 ulUnitIndex, UBYTE ubPlaneBank);
#else
#define ftlGoodBlock_SandiskCheckByProgFail(x, y)	(0)
#endif
_FTL_DECL_ICODE UBYTE ftlLoadDBT( U32 ulTarget, U32 ulSramAddr);
_FTL_DECL_ICODE UBYTE RDT_FindDBTBlock_Task(void);
#if EnableProductHistory
_FTL_BURNER_ICODE void ftlPHexchange(void);
_FTL_BURNER_ICODE UBYTE FindPHBlockByDBT(void);
_FTL_BURNER_ICODE UBYTE ScanPHBlock_Task(UBYTE ubmode);
_FTL_BURNER_ICODE UBYTE FindPHBlockByScan(UBYTE ubmode);
#endif
#if(!RDT)
_FTL_BURNER_ICODE UBYTE RDT_FindCodeBlock_Task(UBYTE ubCEIndex);
_FTL_DECL_ICODE	UBYTE FlaGenerateHeader(UBYTE ubProgramAgain, U32 ulDBTAddr);
_FTL_BURNER_ICODE U32 ftlLaterBadBlock(U32 ulUnitIndex, UBYTE ubPlaneBank, U32 ulSramAddr);

#if ENABLE_CODEBLOCKREPLACEMENT
_FTL_DECL_ICODE void ftlCheckCodeBlockECC(void);
_FTL_DECL_ICODE UBYTE ftlReplaceCodeBlock(UBYTE ubCodeSet);
#endif
//===============================================================================================================

#endif
//===================RDT=======================================================
_FTL_DECL_ICODE UBYTE RDT_FINDBLOCKS_TASK_without_relytest(U32 ulFlaLoopTimes, U32 ulBasiCMDLoopNumber);
//_FTL_DECL_ICODE UBYTE RDT_FINDBLOCKS_TASK(U32 ulFlaLoopTimes, U32 ulBasiCMDLoopNumber);

//ftltools.c
#if FixShallowErase
#if BURNER
_FTL_BURNER_ICODE void ftlForceSaveDummy(U32 ulCurrentPTR, U32 ulEndPtr, UWORD uwFUnit, UBYTE ubMode, UBYTE ubD3Mode, UBYTE ubMark);
#else
_FTL_DECL_ICODE void ftlForceSaveDummy(U32 ulCurrentPTR, U32 ulEndPtr, UWORD uwFUnit, UBYTE ubMode, UBYTE ubD3Mode, UBYTE ubMark);
#endif
#endif
_FTL_BURNER_ICODE void ftlEraseAll(UBYTE ubMode);
_FTL_DECL_ICODE void ftlNandEmulatorVenderErase(UWORD uwVnederUnit, UBYTE ubFeature);
_FTL_DECL_ICODE void ftlPreformatShowError(UBYTE ubCEIndex, UWORD uwUnitIndex, UBYTE ubFailStatus);
_FTL_DECL_ICODE UBYTE TranslateLogicalCEtoPhysicalCE(UBYTE ubLogical_CE);
_FTL_BURNER_ICODE UBYTE ftlIsMixPlaneUnit(UWORD uwUnit);
_FTL_BURNER_ICODE void ftlSearchMixPlaneInFreeBlockTable();
_FTL_BURNER_ICODE void ftlSwapD1MixPlaneUnit(U32 *ulPlaneBankCounterPtr, UBYTE ubGroupIndex, U32 ulD1UnitNum);
_FTL_BURNER_ICODE UBYTE ftlPreformat(void);
_FTL_BURNER_ICODE UBYTE HandleDBTSetLocation(U32 ulDBTAddr, UBYTE ubMode);

#if RDT_BURNER
//_FTL_BURNER_ICODE RDTBurner_A2_EWR_Test(U32 ulUnit, UBYTE ubBurst, UBYTE ubCEIndex);
_FTL_BURNER_ICODE UBYTE ftlRDTPreformat(void);
_FTL_BURNER_ICODE UBYTE BuildDBTinRAM(U32 ulSramAddr);
_FTL_BURNER_ICODE UBYTE ScanRDTSystemArea(U32 ulSramAddr);
_FTL_DECL_ICODE UBYTE SaveDBTSet(U32 ulSramAddr);
_FTL_BURNER_ICODE UBYTE SaveSystemSet(U32 ulSramAddr);
#endif

//ftlinit.c
_FTL_BURNER_ICODE U32 ftlInitFlash(void);
_FTL_BURNER_ICODE void ProgramVTPowerCyclingCheck(UWORD * buffer_src, UWORD * buffer_dest);
_FTL_BURNER_ICODE void CopyVTAreaRUTToRAM(void * buffer);
_FTL_BURNER_ICODE void ftlScanVT(UBYTE ubMode, UBYTE ubStartVT);
_FTL_BURNER_ICODE void ftlScanVT_Binary(UBYTE ubMode, U32 ulUnit_PlanePTR);
#if FixShallowErase
_FTL_DECL_ICODE UWORD ftlScanUnit_Binary(UBYTE ubMode,UWORD uwUnit);
#endif
_FTL_DECL_ICODE void DPS_SpeedBack(UBYTE ubFlashClock, UBYTE ubFlashMode);
_FTL_DECL_ICODE void DPS_SpeedDown(void);
_FTL_DECL_ICODE void DPS_MLC_Parameter_out(UWORD uwAddrOffset, UBYTE* ubData , UBYTE ubDPS_TRIMLEN_MLC);
_FTL_DECL_ICODE void DPS_MLC_Parameter_set(UWORD uwAddrOffset, UWORD uwDataOffset ,UBYTE* ubData , UBYTE ubDPS_TRIMLEN_MLC);
_FTL_DECL_ICODE void DPS_MLC_Power_On_Reset(void);
_FTL_DECL_ICODE void DPS_TLC_Parameter_out(UWORD uwAddrOffset, UBYTE* ubData , UBYTE ubDieNumber , UBYTE ubDPS_TRIMLEN_MLC);
_FTL_DECL_ICODE void DPS_TLC_Parameter_set(UWORD uwAddrOffset, UWORD uwDataOffset ,UBYTE* ubData , UBYTE ubDieNumber ,UBYTE ubDPS_TRIMLEN_MLC);
_FTL_DECL_ICODE void DPS_TLC_Power_On_Reset(UBYTE ubDieNumber);
#if (ENABLE_DPS)
_FTL_DECL_ICODE void DPS_Command(UBYTE *ubData, UBYTE ubMode);
#endif

//ftl.c
_FTL_DECL_ICODE void ftlSwapL2PTable(UWORD uwL2PGroupIndex, UBYTE ubMode);
_FTL_DECL_ICODE void ftlMoveL2PTable(UWORD uwL2PGroupIndex);
_FTL_DECL_ICODE void ftlDebugSwapTable();
#if(!RDT)

_FTL_DECL_ICODE	void Generate_RS_MAP_Index_by_Plane(U32 ulPlaneIndex);
_FTL_DECL_ICODE void Generate_RS_MAP_Index_by_Plane_flafail(U32 ulPlaneIndex);

_FTL_DECL_ICODE void ftlBG_Flow();
_FTL_DECL void ftl_cal_write_buffer_window(void);
_FTL_BURNER_ICODE void ftlUpdateMixPlaneStatus(void);
_FTL_DECL_ICODE void ftlGuarantee_Flush();
_FTL_DECL UBYTE ftlHandleSpecialData(UBYTE ubLink);
_FTL_DECL void ftlXferDataOut(UBYTE ubLink);
_FTL_DECL void ftlXferDataIn(XferDataIn_t *XferDataIn, UBYTE ub4kNum) ;
_FTL_DECL_ICODE void ftlMoveSearchLink(UWORD uwL2PEntryIndex, UBYTE ubSearchLinkLayer, U32 ulTableIndex);
_FTL_DECL_ICODE void ftlSaveDummy(U32 ulNextPageAPtr, UBYTE ubSource, UBYTE ubEncode);
_FTL_DECL void ftlCheckP2L_Valid(UBYTE ubMiddleClean) ;
_FTL_DECL_ICODE void ftlSaveP2LTable(UBYTE ubPlanesPerP2LTable);
_FTL_DECL_ICODE U32 RS_ParityCount(U32 *pulPlaneIndex, UBYTE ubFastPage);
_FTL_DECL_ICODE U32 SEQ_Sector_Calculation(U32 ulVir4kIndexInL2POffset, UWORD uwL2PGroupIndex, U32 ulSeqHeadFEntry, UBYTE ubSeqHeadOffset, UBYTE ubSeqHeadZByteNum);
_FTL_DECL_ICODE UBYTE ftlLoadAlignment(UBYTE ubLink);
_FTL_DECL_ICODE UBYTE ftlGCLoadP2LTable(UBYTE ubCloseTargetMode);
_FTL_DECL_ICODE UBYTE ftlFlushD1(UBYTE ubMode, UBYTE ubDie, UBYTE ubNeedSaveVT);
_FTL_DECL_ICODE void ftlBuildGCTable(U32 ulGroupTable_Addr, U32 ulGroupNum, UBYTE ubMode);
_FTL_DECL_ICODE UWORD ftl_get_flush_d3(UBYTE ubMode, UBYTE ubDie);
_FTL_DECL_ICODE void ftlWLReBuildGCTable();
//#if ((TLC || MicronFlashOnly) && (ENABLE_DEBUG_RW || FORCE_AUTOPOL_TIMEOUT))
#if   1 //(TLC || MicronFlashOnly)	// for avoiding binary code overflowed
_FTL_DECL_ICODE void ftlCleanGRTable(UBYTE ubMode);
#else
_FTL_DECL void ftlCleanGRTable(UBYTE ubMode);
#endif

_FTL_DECL_ICODE void ftlHandleGRTable(UBYTE ubMode);

_FTL_DECL_ICODE UBYTE ftlFindVictim_BG();
_FTL_DECL_ICODE void ftlGCLoadP2LTable_BG();
_FTL_DECL_ICODE void ftlBuildGCTable_BG();
_FTL_DECL_ICODE UBYTE ftlRetentionRead_BG();
_FTL_DECL_ICODE UBYTE ftlRetentionRead_BG_Bics3();
_FTL_DECL_ICODE UBYTE ftlRead4k_BG();
_FTL_DECL_ICODE UBYTE ftlWrite4k_BG();
_FTL_DECL_ICODE void ftlStop_BG_Copy();
_FTL_DECL_ICODE void ftlCloseTarget(UBYTE ubMode);
_FTL_DECL_ICODE void ftCloseTarget_Stop();
_FTL_DECL_ICODE void ftlCopyData(void);
_FTL_DECL_ICODE void ftlClassifyTableEntry(U32 ulTableSize, U32*ulTableaddress,U32 *ulTempTableAddress, UBYTE ubTableType);
_FTL_DECL_ICODE UWORD ftlFindRAMToLoadL2P(UBYTE ubMode);
_FTL_DECL_ICODE void ftlCloseTableTarget(UBYTE ubVictimTableIndex, UBYTE ubMode);
#if ENABLE_TABLE_VERIFY
_FTL_DECL_ICODE UBYTE ftlVerifyTable(UWORD uwFUnit, U32 ulFEntry, U32 ulSpareMark, UBYTE ubVerifyMode);
_FTL_DECL_ICODE UWORD ftlCheckParityP2LPTR(UWORD uwVerifyTableTargetPTR);
#endif
_FTL_DECL_ICODE void ftlLoadTable(UBYTE ubMode, U32 ulInRAMAddress, UWORD uwInRamIndex, UWORD uwL2PGroupIndex, UBYTE ubFrameIndex);
_FTL_DECL_ICODE void ftlFlushL2PTable(UBYTE ubSaveVC);
_FTL_DECL_ICODE void ftlGetVictimTableUnit(UBYTE *ubMinTableVCUnitIndex);
_FTL_DECL_ICODE void ftlSaveTable(FTarget_t *uwTarget, U32 ulInRAMAddress, UWORD uwL2PGroupIndex, U32 ulMode);
_FTL_DECL_ICODE UBYTE ftlReSaveTable(FTarget_t *uwTarget, UWORD uwNeedToProgramTableTargetPTR);
_FTL_DECL_ICODE void ftlFillTable(UWORD uwFillPlaneNum);
_FTL_DECL_ICODE void ftlSwapTable(UBYTE ubMode, UWORD uwHostTableIndex);
_FTL_DECL_ICODE void ftlLoadTableP2L(UWORD uwUnit, U32 ulInRAMAddress);
_FTL_DECL_ICODE U32 ftlLoadP2LTable(UBYTE ubPlaneLimit, UWORD uwUnit, U32 ulStartPlanePTR, U32 ulInRAMAddress);
_FTL_DECL_ICODE U32 ftlRebuildP2L(UWORD uwUnit, U32 ulTargetAddress);
#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
_FTL_DECL_ICODE void ftlSaveBunchParityToInitInfo();
#endif
_FTL_DECL_ICODE void ftlSaveInitInfo(UBYTE ubMode);
_FTL_DECL_ICODE void ftlSaveVT(UBYTE ubUseFPU, UBYTE ubSaveVTSerialNumber);
_FTL_DECL_ICODE void ftlCopyInitInfo(void);
_FTL_DECL_ICODE void ftlCopyTable(UBYTE ubMovedTableUnitIndex);
_FTL_DECL_ICODE void ftlCopyTableUnit(UWORD uwSourceUnit, UWORD* uwTableP2L, UBYTE ubCopyActiveTableUnit);
_FTL_DECL_ICODE void ftlCopyUnit(P2LTable_t* ulSourceP2LTable, P2LTable_t* ulTargetP2LTable, U32 ulP2L_L4KNum, UWORD uwSourceUnit, UBYTE ubMode);
_FTL_DECL_ICODE void ftlCheckCopyUnit(P2LTable_t* ulSourceP2LTable, U32 ulP2L_L4KNum, UWORD uwSourceUnit);
_FTL_DECL_ICODE void ftlCheckCopyTableUnit(UWORD uwSourceUnit, UWORD* uwTableP2L, UBYTE ubCheckActiveTableUnit);
_FTL_DECL_ICODE void ftlCheckVC(UBYTE ubMode);
_FTL_DECL_ICODE void ftlSetRSParityFQ(UBYTE ubSource, UBYTE ubRSTag);
_FTL_DECL_ICODE void ftlCheckTableVC();
_FTL_DECL_ICODE void ftlPowerCycle(UBYTE ubPC_para);
#define PC_ACTIVE_GR_CIM BIT0
_FTL_DECL_ICODE void ftlCheckRUTLines (void);
_FTL_DECL_ICODE void ftlCheckExtend (void);
_FTL_DECL_ICODE UBYTE ftlCheckFailHandleFreeUnits(UBYTE ubDie);
_FTL_DECL_ICODE UBYTE ftlGetFreeUnitToFailHandle (UBYTE ubDie, UBYTE ubTableSelect);
_FTL_DECL_ICODE UBYTE ftlFindFailHandleUnit(UWORD uwFailUnit);
_FTL_DECL_ICODE void ftlChangeRUTMappingAndEraseCount (UWORD uwFailUnit, UBYTE ubD1Link, UBYTE ubUse2ndErrHandler);
_FTL_DECL_ICODE void ftlSwapRUTMapping3Times(UWORD uwAUnit, UWORD uwBUnit);
_FTL_DECL_ICODE void ftlPutBlocksToRUTD1Only(UBYTE ubDie, UWORD uwFailUnit);
_FTL_DECL_ICODE UBYTE ftlCheckUnitsAtRUTD1Only(UBYTE ubDie, UBYTE ubMode, UWORD uwUnit);
_FTL_DECL_ICODE void ftlGetUnitsFromRUTD1Only(UBYTE ubDie, UWORD uwFailUnit);
_FTL_DECL_ICODE void ftlMakeRUTCloser(UBYTE ubDie, UBYTE ubPlaneBank, UBYTE ubMode, UWORD uwUnitStart, UWORD uwUnitStop);
_FTL_DECL_ICODE void ftlHandleSingleFree(UWORD uwFailUnit, UWORD uwTargetUnit, UWORD uwRUTSummaryLogIndex, UBYTE ubDie);
_FTL_DECL_ICODE UWORD CalculateRemainingSpareBlocksPerPlane(UBYTE ubGroupIndex, UBYTE ubPlaneBank);
_FTL_DECL_ICODE UWORD GetMinRemainingSpareBlocksPerPlane(void);
_FTL_DECL_ICODE void ftlUpdateRUT();
_FTL_DECL_ICODE void ftlMakeRUTLogCloser(UWORD uwRUTUnit, UBYTE ubMode);	//跟guwRUT沒有關係.
_FTL_DECL_ICODE void ftlSwapRUT(UBYTE ubSwapMode, UWORD uwFailUnit, UBYTE ubIsD1Unit);
_FTL_DECL_ICODE void ftlDirectHandleFailUnit (UBYTE ubMode, UWORD uwFailUnit, UBYTE ubFailD1Link, UBYTE ubDie);
_FTL_DECL_ICODE void ftlRecordFailLog(UBYTE ubLogType, UBYTE ubAbnormalLoc, UBYTE ubgFQLinkIndex, UBYTE ubFailType, UBYTE ubTableSel, UBYTE ubFLHErrorMap);
_FTL_DECL_ICODE void ftlClearRAMForGR(U32 ulClearGRStartAddress, UBYTE ubWaitCQ);
_FTL_DECL_ICODE void ftlClearRAMForGC();
_FTL_DECL_ICODE void ftlPreLoadL2P(UWORD uwStartL2PGroup, U32 ulGroupNum);
_FTL_DECL_ICODE void ftlResetRSGroup(UBYTE ubRSGroupNum, UBYTE ubRSStartIndex);
_FTL_DECL_ICODE void ftlCheckRSTagDone(UBYTE ubRSTag, UBYTE ubIsTable, U32 ulTargetPTR);
_FTL_DECL_ICODE void ftlRemoveForceReadFIFO(UWORD uwFUnit);
_FTL_DECL_ICODE UBYTE ftlCheckForceReadFIFO(UWORD uwFUnit);
_FTL_DECL_ICODE UWORD ftlPopFirstForceReadInfo(UBYTE ubRemoveFirst);
_FTL_DECL_ICODE void ftlPushForceReadInfo(UWORD uwFUnit);
_FTL_DECL_ICODE UBYTE ftlCheckForceReadInfo(UWORD uwFUnit);
_FTL_DECL_ICODE void ftlCleanReadCheckInfo(UWORD uwFUnit);
_FTL_DECL_ICODE void ftlForceReadMove(void);
_FTL_DECL_ICODE void ftlCalPartialForceReadMoveRatio(void);
_FTL_DECL_ICODE void ftlReadDisturbCheck(void);
_FTL_DECL_ICODE void TrimInFtl();
_FTL_DECL_ICODE void ftlPowerCycleRandTest(UBYTE ubPowerCycleType);
_FTL_DECL_ICODE void ftlRemoveRUTSummaryLog(UWORD uwTarget, UBYTE ubCheckD1, UBYTE ubIsD1Unit, UBYTE ubD1Link);
#if(ENABLE_OLD_TRIM==0)
_FTL_DECL_ICODE void ftlSwapL2PTableForTrim(UWORD uwL2PGroupIndex, UWORD uwRAMIndexForLoad, UWORD uwL2PGroupCount);
#endif
#if(!BURNER)
_FTL_DECL_ICODE UBYTE ftlRefreshDBTAccordingToRUT(UBYTE ubMode, UBYTE ubRUTSummaryLogIndex);
#endif
#if ((!TLC) && (!MicronFlashOnly))
_FTL_DECL_ICODE void ftlFillMLCGRTarget();
#endif
#endif


//======================================================================================================================
//PH_SQL
//======================================================================================================================
#if PH_SQL_FUNC
_FTL_BURNER_ICODE void NandVToolOperationSetting(void);
_FTL_BURNER_ICODE void NandVToolCmdParser(void);
_FTL_BURNER_ICODE void NandVToolSetFLHIP(void);
_FTL_BURNER_ICODE void NandVToolPreSettingParser(void);
_FTL_BURNER_ICODE void Set_FlashIP_InterFace(UBYTE ubMode);
_FTL_BURNER_ICODE void NandVToolGetFLHIP(void);
#endif


//----------------------
// Random Function
//----------------------
_FTL_DECL_DCODE ULLONG gullRandomVector;
_FTL_DECL_ICODE U32 getIntRandValue(U32 limit);


