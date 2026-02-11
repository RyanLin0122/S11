/*=======================================================================================
Define const
-----------------------------------------------------------------------------------------


=======================================================================================*/
#include "S11_FPU.h"
#include <stddef.h>
#include "Setup.h"
#define	_Uncached
#define _UCV _Uncached volatile
#define	_V	volatile
#define KB		<< 10

#define TRUE		1
#define FALSE	0

#define EVEN		0
#define ODD		1

//H -> L
//#define HIGHBYTE(x)			(((unsigned char *)(&x))[0])
//#define LOWBYTE(x)			(((unsigned char *)(&x))[1])
#define HIGHWORD(x)			(((unsigned short *)(&x))[0])
#define LOWWORD(x)			(((unsigned short *)(&x))[1])

#define LONGBYTE1(x)		(((unsigned char *)(&x))[3])
#define LONGBYTE2(x)		(((unsigned char *)(&x))[2])
#define LONGBYTE3(x)		(((unsigned char *)(&x))[1])
#define LONGBYTE4(x)		(((unsigned char *)(&x))[0])
#define A_LONGBYTE1(x)		((((x))>>24)&0xFF)
#define A_LONGBYTE2(x)		((((x))>>16)&0xFF)
#define A_LONGBYTE3(x)		((((x))>>8)&0xFF)
#define A_LONGBYTE4(x)		((((x))>>0)&0xFF)


#define HIGHBYTE(x)			(((unsigned short)(x)>>8))
#define LOWBYTE(x)			(((unsigned char )(x)))


#define _setareg(reg, value)  (_sr(_lr(reg)|(value), reg))
#define _clrareg(reg, value)  (_sr(_lr(reg)&(~(value)), reg))

typedef signed char SBYTE;
typedef unsigned char UBYTE;
typedef short WORD;
typedef unsigned short UWORD;
typedef unsigned long U32;
typedef unsigned long long ULLONG;

#define M_RUT2DTo1D(ubCEIndex,uwBlock)        guwRUTBASE[(ubCEIndex)*(guwRUTSizePerCE/2)+(uwBlock)]
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
#define M_RandomSeed(uwPage, uwBase)  	gulSeed[(uwPage)/(uwBase)];
#endif
#define M_DebugToggleGPIO1()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO1]=SYS0B_CR_GPIO_OE|1;\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO1]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugToggleGPIO2()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO2]=SYS0B_CR_GPIO_OE|1;\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO2]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugToggleGPIO3()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO3]=SYS0B_CR_GPIO_OE|1;\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO3]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugToggleGPIO4()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO4]=SYS0B_CR_GPIO_OE|1;\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO4]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugToggleGPIO5()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO5]=SYS0B_CR_GPIO_OE|1;\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO5]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugToggleGPIO6()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO6]=SYS0B_CR_GPIO_OE|1;\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO6]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugToggleGPIO7()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO7]=SYS0B_CR_GPIO_OE|1;\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO7]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DisableAllFlashCE()   {\
				                FCONL[FCONL_FCE_ENB]   = 0x00000000;\
						FCONL[FCONL_FCE_ENB_1] = 0x00000000;\
                                                }
#define M_DebugGPIO2_High()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO2]=SYS0B_CR_GPIO_OE|1;\
                                                }\
                                        }
#define M_DebugGPIO2_Low()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO2]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugGPIO3_High()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO3]=SYS0B_CR_GPIO_OE|1;\
                                                }\
                                        }
#define M_DebugGPIO3_Low()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO3]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugGPIO4_High()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO4]=SYS0B_CR_GPIO_OE|1;\
                                                }\
                                        }
#define M_DebugGPIO4_Low()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO4]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugGPIO5_High()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO5]=SYS0B_CR_GPIO_OE|1;\
                                                }\
                                        }
#define M_DebugGPIO5_Low()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO5]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugGPIO6_High()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO6]=SYS0B_CR_GPIO_OE|1;\
                                                }\
                                        }
#define M_DebugGPIO6_Low()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO6]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugGPIO7_High()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO7]=SYS0B_CR_GPIO_OE|1;\
                                                }\
                                        }
#define M_DebugGPIO7_Low()    {\
                                                if(ENABLE_DEBUG_GPIO==1){\
                                                        SYS0B[SYS0B_GPIO_CTRL_GPIO7]=SYS0B_CR_GPIO_OE|0;\
                                                }\
                                        }
#define M_DebugGPIO8_High()    {\
                                                  SYS0B[SYS0B_GPIO_CTRL_GPIO8]=(SYS0B_CR_GPIO_PU_X_1|SYS0B_CR_GPIO_PU_X_0|SYS0B_CR_GPIO_OE|1);\
                                        }
#define M_DebugGPIO8_Low()    {\
                                                  SYS0B[SYS0B_GPIO_CTRL_GPIO8]=(SYS0B_CR_GPIO_PU_X_1|SYS0B_CR_GPIO_PU_X_0|SYS0B_CR_GPIO_OE|0);\
                                        }
#define M_DebugGPIO8_ON_LED()    {\
                                                  SYS0B[SYS0B_GPIO_CTRL_GPIO8]=(SYS0B_CR_GPIO_PU_X_1|SYS0B_CR_GPIO_PU_X_0|SYS0B_CR_GPIO_OE|0);\
                                        }
#define M_DebugGPIO8_OFF_LED()    {\
                                                  SYS0B[SYS0B_GPIO_CTRL_GPIO8]=(SYS0B_CR_GPIO_PU_X_1|SYS0B_CR_GPIO_PU_X_0|0);\
                                        }

#define M_EnableAllFlashCE()   {\
                                                        FCONL[FCONL_FCE_ENB]   = 0xFFFFFFFF;\
							FCONL[FCONL_FCE_ENB_1] = 0xFFFFFFFF;\
                                                }
// BIT0:HostAS, BIT1:FlashAS, BIT3:SS2_AS, BIT8:HostCHK, BIT10:FlashCHK, BIT11:SS2_CHK
#define M_SetFlagSettingNone(){\
					AXIW[AXIW_FLAG_SET] = 0;\
					gubCheckBufferFlag = 0;\
				}
#define M_SetFlagSettingH()          (AXIW[AXIW_FLAG_SET] = (HOST1_AS|HOST1_CHK))
#define M_SetFlagSettingF()          (AXIW[AXIW_FLAG_SET] = (FLH_QRYRND_CHK|FLH_AS|FLH_CHK))
#define M_SetFlagSettingHF()          (AXIW[AXIW_FLAG_SET] = (HOST1_AS|FLH_AS|HOST1_CHK|FLH_CHK|FLH_QRYRND_CHK))
#define M_SetFlagSettingHS()          (AXIW[AXIW_FLAG_SET] = (HOST1_AS|TDMAC0_AS|HOST1_CHK|TDMAC0_CHK))
#define M_SetFlagSettingSHF(){\
					AXIW[AXIW_FLAG_SET] = (HOST1_AS|FLH_AS|TDMAC0_AS|HOST1_CHK|FLH_QRYRND_CHK|FLH_CHK|TDMAC0_CHK);\
					gubCheckBufferFlag = 1;\
				}


#define M_DisableHWD2H_NCQ()             (HW[HW_AUTO_FIS_CTRL] &= (~H_NCQ_AUTO_REG_EN))
#define M_EnableHWD2H_NCQ()             (HW[HW_AUTO_FIS_CTRL] |= H_NCQ_AUTO_REG_EN)
#define M_DisableHWD2HSDB()                (HW[HW_AUTO_FIS_CTRL] &= (~(H_NCQ_AUTO_SDB_W_EN|H_NCQ_AUTO_SDB_R_EN|H_DMA_AUTO_REG_W_EN|H_DMA_AUTO_REG_R_EN|H_PIO_AUTO_REG_W_EN)))
#define M_EnableHWD2H_DMAR()            (HW[HW_AUTO_FIS_CTRL] |= H_DMA_AUTO_REG_R_EN)
#define M_EnableHWD2H_DMAW()            (HW[HW_AUTO_FIS_CTRL] |= H_DMA_AUTO_REG_W_EN)
#define M_EnableHWD2H_PIOW()            (HW[HW_AUTO_FIS_CTRL] |= H_PIO_AUTO_REG_W_EN)
#define M_EnableHWSDB_NCQW()            (HW[HW_AUTO_FIS_CTRL] |= H_NCQ_AUTO_SDB_W_EN)
#define M_DisableHWSDB_NCQW()            (HW[HW_AUTO_FIS_CTRL] &= (~H_NCQ_AUTO_SDB_W_EN))
#define M_EnableHWSDB_NCQR()            (HW[HW_AUTO_FIS_CTRL] |= H_NCQ_AUTO_SDB_R_EN)
#define M_EnableHWD2HSDB_W()         (HW[HW_AUTO_FIS_CTRL] |= (H_PIO_AUTO_REG_W_EN|H_DMA_AUTO_REG_W_EN|H_NCQ_AUTO_SDB_W_EN))
#define M_EnableHWD2HSDB_R()         (HW[HW_AUTO_FIS_CTRL] |= (H_DMA_AUTO_REG_R_EN|H_NCQ_AUTO_SDB_R_EN))
#define M_EnableHWD2HSDB_WR()           (HW[HW_AUTO_FIS_CTRL] |= ((H_PIO_AUTO_REG_W_EN|H_DMA_AUTO_REG_W_EN|H_NCQ_AUTO_SDB_W_EN)|(H_DMA_AUTO_REG_R_EN|H_NCQ_AUTO_SDB_R_EN)))


#if UFS
#define M_FlashModeBackToDefault(){\
	if ( gubFlashMode == FLH_MODE_TOGGLE2) {\
		flaSetToggleMode(INTERFACE_TOGGLE2To1, 1);\
		gubFlashMode = FLH_MODE_TOGGLE1;\
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);\
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;\
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;\
		gubIOMode_CE[0] = 0;\
		gubIOMode_CE[1] = 0;\
	}\
	if ((gubFlashMode == FLH_MODE_TOGGLE1) && (((gubUFSDefaultMode_CE & UFS_Sample_Exist_Mask) >> 4) != ((gubUFSDefaultMode_CE & UFS_Sample_Default_Mode_Mask)))) {\
		guwNotSetMap = guwDefaultToggleCEMap;\
		flaSetToggleMode(INTERFACE_LEGACY, 1);\
		guwNotSetMap = 0;\
		gubFlashMode = FLH_MODE_LEGACY;\
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;\
		gubFlashMode_CE[0] = 0;\
		gubFlashMode_CE[1] = 0;\
	}\
	if ( (gubFlashMode == FLH_MODE_ONFI_NVDDR) || (gubFlashMode == FLH_MODE_ONFI_NVDDR2) ) {\
		UBYTE ubi, ubj;\
		for(ubi = 0; ubi <= (gubCENumber >> gubPlanesPerBurstLog); ubi++) {\
			for(ubj = 0; ubj < gubPlanesPerBurst; ubj++) {\
				FCONL[FCONL_FCE_ENB] = (BIT0 << (ubj*MAX_CE_PER_CH)) << ubi;\
				flaReset(ubj, 0xFF);\
			}\
			FCONL[FCONL_FCE_ENB] = 0x00000000;\
		}\
		gubFlashMode = FLH_MODE_LEGACY;\
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);\
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;\
		gubFlashMode_CE[0] = 0;\
		gubFlashMode_CE[1] = 0;\
		gubFlashONFI_CE[0] = 0;\
		gubFlashONFI_CE[1] = 0;\
	}\
}
#else
#define M_FlashModeBackToDefault(){\
	if ( gubFlashMode == FLH_MODE_TOGGLE2) {\
		flaSetToggleMode(INTERFACE_TOGGLE2To1, 1);\
		gubFlashMode = FLH_MODE_TOGGLE1;\
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);\
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;\
		FALLL[FCTLL_FLH_SET] |= FLA_TOGGLE_MODE;\
		gubIOMode_CE[0] = 0;\
		gubIOMode_CE[1] = 0;\
	}\
	if ((gubFlashMode == FLH_MODE_TOGGLE1) && (((gubFlashID[5] & BIT7) == 0))) {\
		flaSetToggleMode(INTERFACE_LEGACY, 1);\
		gubFlashMode = FLH_MODE_LEGACY;\
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;\
		gubFlashMode_CE[0] = 0;\
		gubFlashMode_CE[1] = 0;\
	}\
	if ( (gubFlashMode == FLH_MODE_ONFI_NVDDR) || (gubFlashMode == FLH_MODE_ONFI_NVDDR2) ) {\
		UBYTE ubi, ubj;\
		for(ubi = 0; ubi <= (gubCENumber >> gubPlanesPerBurstLog); ubi++) {\
			for(ubj = 0; ubj < gubPlanesPerBurst; ubj++) {\
				FCONL[FCONL_FCE_ENB] = (BIT0 << (ubj*MAX_CE_PER_CH)) << ubi;\
				flaReset(ubj, 0xFF);\
			}\
			FCONL[FCONL_FCE_ENB] = 0x00000000;\
		}\
		gubFlashMode = FLH_MODE_LEGACY;\
		flaSetPadIOType(2, SINGLE_END_IO_TYPE);\
		FALLL[FCTLL_FLH_SET] &= FLA_LEGACY_MODE;\
		gubFlashMode_CE[0] = 0;\
		gubFlashMode_CE[1] = 0;\
		gubFlashONFI_CE[0] = 0;\
		gubFlashONFI_CE[1] = 0;\
	}\
}
#endif

#define M_CommandOperationTime(ulTriggerTime) 		(gulOperationTime >= ulTriggerTime) ? (gulOperationTime - ulTriggerTime) : (0xFFFFFFFF - ulTriggerTime + gulOperationTime)

#define M_AddToBufferQueue(ubLink){\
                                                        gBQI.ubLinkLastUse = ubLink;\
                                                        gBQI.ubLinkFTLLast = ubLink;\
                                                        M_AssertError(ubLink==0xFF);\
                                                        gBQI.ubLinkNum++;\
                                                        gBQI.ubLinkActive=0xFF;\
                                                        M_AssertError(gBQI.ubLinkNum>BQ_NUM);\
                                                }
#define M_RemoveWriteBufferQueue(ubLink){\
                                                gBQI.ubLinkNum--;\
                                                gBQI.BufferQueue[ubLink].ubThisBQisBG = 0;\
                                                if((ubLink)==gBQI.ubLinkFTLLast){\
						gBQI.ubLinkFTLLast = gBQI.BufferQueue[ubLink].ubPrevious;\
					}\
                                                if ((ubLink)!=gBQI.ubLinkLast){\
                                                        if ((ubLink)==gBQI.ubLinkFirst) {\
                                                                gBQI.ubLinkFirst = gBQI.BufferQueue[ubLink].ubNext;\
                                                                gBQI.BufferQueue[gBQI.ubLinkFirst].ubPrevious = ubLink;\
                                                                gBQI.BufferQueue[ubLink].ubPrevious = gBQI.ubLinkLast;\
                                                                gBQI.BufferQueue[ubLink].ubNext = gBQI.ubLinkFirst;\
                                                        }\
                                                        else {\
                                                                UBYTE ubPrevious = gBQI.BufferQueue[ubLink].ubPrevious;\
                                                                UBYTE ubNext = gBQI.BufferQueue[ubLink].ubNext;\
                                                                if((ubLink)==gBQI.ubLinkLastUse){\
                                                                        gBQI.ubLinkLastUse = gBQI.BufferQueue[ubLink].ubPrevious;\
                                                                }\
							if((ubLink)==gBQI.ubLinkFTLLast){\
								gBQI.ubLinkFTLLast = gBQI.BufferQueue[ubLink].ubPrevious;\
							}\
                                                                gBQI.BufferQueue[ubPrevious].ubNext = ubNext;\
                                                                gBQI.BufferQueue[ubNext].ubPrevious = ubPrevious;\
                                                                gBQI.BufferQueue[ubLink].ubPrevious = gBQI.ubLinkLast;\
                                                                gBQI.BufferQueue[ubLink].ubNext = gBQI.ubLinkFirst;\
                                                                gBQI.BufferQueue[gBQI.ubLinkFirst].ubPrevious=ubLink;\
                                                        }\
                                                        gBQI.BufferQueue[gBQI.ubLinkLast].ubNext = ubLink;\
                                                        gBQI.ubLinkLast = ubLink;\
                                                }\
                                                else{\
                                                        gBQI.ubLinkLastUse = gBQI.BufferQueue[ubLink].ubPrevious;\
                                                        gBQI.ubLinkFTLLast = gBQI.BufferQueue[ubLink].ubPrevious;\
                                                }\
}
#define M_RemoveReadBufferQueue(ubLink){\
                                                UBYTE ubRBQIIndex=gBQI.BufferQueue[ubLink].ubRBQIIndex;\
                                                gubRBQINumber--;\
					M_AssertError((gubRBQINumber>32));\
                                                if(ubRBQIIndex!=gubRBQILast){\
                                                        gRBQI[gRBQI[ubRBQIIndex].ubPrevious].ubNext=gRBQI[ubRBQIIndex].ubNext;\
                                                        gRBQI[gRBQI[ubRBQIIndex].ubNext].ubPrevious=gRBQI[ubRBQIIndex].ubPrevious;\
                                                        gRBQI[ubRBQIIndex].ubNext=gRBQI[gubRBQILast].ubNext;\
                                                        gRBQI[ubRBQIIndex].ubPrevious=gubRBQILast;\
                                                        gRBQI[ gRBQI[gubRBQILast].ubNext].ubPrevious=ubRBQIIndex;\
                                                        gRBQI[gubRBQILast].ubNext=ubRBQIIndex;\
                                                        gubRBQILast=ubRBQIIndex;\
                                                }\
                                                if(gubRBQINumber==31){\
                                                        gubRBQIIndex=gubRBQILast;\
                                                }\
                                                gBQI.ubLinkNum--;\
                                                gBQI.BufferQueue[ubLink].ubThisBQisBG = 0;\
                                                if (gubKeepRFQ && ((ubLink == gBQI.ubLinkLastUse) || gBQI.BufferQueue[gBQI.BufferQueue[ubLink].ubNext].btWrite)) {\
	                                                if (gubNeedWaitCount) {\
															gFQI.gFQLink[gFQI.ubFQLinkFree].btOutOfOrder = 1;\
	                                                        gubOutOfOrderLastFQ = gFQI.ubFQLinkFree;\
															gubFQOutOfOrderCnt++;\
	                                                }\
	                                                if (gFQI.gFQLink[gFQI.ubFQLinkFree].FQPara.B.btUserData) {\
							gubRFQCnt ++;\
						}\
	                                                gFQI.ubFQLinkNumber++;\
	                                                gFQI.ubFQLinkFree = gFQI.gFQLink[gFQI.ubFQLinkFree].ubNext;\
	                                                gubKeepRFQ = 0;\
	                                                gubKeepRFQL4KNum = 0;\
	                                                gulKeepRFQZipMap = 0;\
	                                        }\
                                                if ((ubLink)!=gBQI.ubLinkLast){\
                                                        if (ubLink==gBQI.ubLinkFirst) {\
                                                                gBQI.ubLinkFirst = gBQI.BufferQueue[ubLink].ubNext;\
                                                                gBQI.BufferQueue[gBQI.ubLinkFirst].ubPrevious = ubLink;\
                                                                gBQI.BufferQueue[ubLink].ubPrevious = gBQI.ubLinkLast;\
                                                                gBQI.BufferQueue[ubLink].ubNext = gBQI.ubLinkFirst;\
                                                        }\
                                                        else {\
                                                                UBYTE ubPrevious = gBQI.BufferQueue[ubLink].ubPrevious;\
                                                                UBYTE ubNext = gBQI.BufferQueue[ubLink].ubNext;\
                                                                if(ubLink==gBQI.ubLinkLastUse){\
                                                                        gBQI.ubLinkLastUse = gBQI.BufferQueue[ubLink].ubPrevious;\
                                                                }\
                                                                if((ubLink)==gBQI.ubLinkFTLLast){\
								gBQI.ubLinkFTLLast = gBQI.BufferQueue[ubLink].ubPrevious;\
							}\
                                                                gBQI.BufferQueue[ubPrevious].ubNext = ubNext;\
                                                                gBQI.BufferQueue[ubNext].ubPrevious = ubPrevious;\
                                                                gBQI.BufferQueue[ubLink].ubPrevious = gBQI.ubLinkLast;\
                                                                gBQI.BufferQueue[ubLink].ubNext = gBQI.ubLinkFirst;\
                                                                gBQI.BufferQueue[gBQI.ubLinkFirst].ubPrevious=ubLink;\
                                                        }\
                                                        gBQI.BufferQueue[gBQI.ubLinkLast].ubNext = ubLink;\
                                                        gBQI.ubLinkLast = ubLink;\
                                                }\
                                                else{\
                                                        gBQI.ubLinkLastUse = gBQI.BufferQueue[ubLink].ubPrevious;\
                                                        gBQI.ubLinkFTLLast = gBQI.BufferQueue[ubLink].ubPrevious;\
                                                }\
}
#define M_GetNewRBQI(ubLink){\
                                                gBQI.BufferQueue[ubLink].ubRBQIIndex=gubRBQIIndex;\
                                                gubRBQIIndex=gRBQI[gubRBQIIndex].ubNext;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].btSeq = 0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].uwTableInRamIndex=guwWaitRamToLoadTable;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].ubGRHitMap=0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].ub4kInBQMap=0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].ubSpecialDataMap=0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].btIsTempHit=0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].ubPhase= BYTE_BQR_Check_TableInRam;\
												if (gBQI.BufferQueue[ubLink].ubThisBQisBG == 0) {\
													UBYTE ub4kInBQIndex;\
													for(ub4kInBQIndex=0;ub4kInBQIndex<gBQI.BufferQueue[ubLink].ub4kNum;ub4kInBQIndex++){\
														if((gBQI.BufferQueue[ubLink].ubDataInDRamMap==0)||(gBQI.BufferQueue[ubLink].ubDataInDRamMap&(BIT0<<ub4kInBQIndex))){\
															UBYTE ubBQ4kBufferIndex= ((gBQI.BufferQueue[ubLink].uwBufferIndex/8)+ub4kInBQIndex) & gulBuffer2Read4KMask;\
															gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].ubUseBufferOrder[ub4kInBQIndex]=gubUseEach4kBufferInThisOrder[ubBQ4kBufferIndex];\
															gubUseEach4kBufferInThisOrder[ubBQ4kBufferIndex]++;\
														}\
													}\
												}\
                                                gubRBQINumber++;\
}
#define M_LoadAlignmentGetNewRBQI(ubLink){\
                                                gBQI.BufferQueue[ubLink].ubRBQIIndex=gubRBQIIndex;\
                                                gubRBQIIndex=gRBQI[gubRBQIIndex].ubNext;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].btSeq = 0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].uwTableInRamIndex=guwWaitRamToLoadTable;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].ubGRHitMap=0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].ub4kInBQMap=0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].ubSpecialDataMap=0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].btIsTempHit=0;\
                                                gRBQI[gBQI.BufferQueue[ubLink].ubRBQIIndex].ubPhase= BYTE_BQR_Search_GR;\
                                                gubRBQINumber++;\
}

#define M_AddTempTable(uwIndex){\
	UBYTE ubptr;\
	if(gTempTable.ubLinkNum==0){\
		ubptr = gTempTable.ubLinkFirst;\
		gTempTable.ubLinkLastUse = ubptr;\
	}\
	else{\
		ubptr = gTempTable.gTempTableQueue[gTempTable.ubLinkLastUse].ubNext;\
		gTempTable.ubLinkLastUse=gTempTable.gTempTableQueue[gTempTable.ubLinkLastUse].ubNext;\
	}\
	gTempTable.gTempTableQueue[ubptr].uwTableInRamIndex = uwIndex;\
	gTempTable.ubLinkNum++;\
	M_AssertError(( gTempTable.ubLinkNum>TempModeNum));\
}

// 如果把所有Temp找了一圈都沒有可以用的, pointer指向first
#define M_GetTempTable(uwIndex){\
	M_AssertError(gTempTable.ubLinkNum == 0);\
	UBYTE ubCheckNum = 0;\
	do {\
		uwIndex =  gTempTable.gTempTableQueue[gTempTable.ubLinkPtr].uwTableInRamIndex;\
		if (gTempTable.ubLinkPtr == gTempTable.ubLinkLastUse) {\
			gTempTable.ubLinkPtr = gTempTable.ubLinkFirst;\
		}\
		else {\
			gTempTable.ubLinkPtr =  gTempTable.gTempTableQueue[gTempTable.ubLinkPtr].ubNext;\
		}\
		ubCheckNum ++;\
	} while((gulL2P_InRamInverse[uwIndex].B.uwWaitingTableCount) && (ubCheckNum < gTempTable.ubLinkNum));\
	if (gulL2P_InRamInverse[uwIndex].B.uwWaitingTableCount) {\
		uwIndex = guwWaitRamToLoadTable;\
	}\
}

#define M_RemoveTempTable(uwInRamIndex){\
	UBYTE ubIndex = 0xFF;\
	UBYTE  ubi;\
	UBYTE ubptr;\
	UBYTE ubNext;\
	if(gTempTable.ubLinkNum>0){\
	        ubptr= gTempTable.ubLinkFirst;\
	        for(ubi=0; ubi< gTempTable.ubLinkNum;ubi++){\
	                if(gTempTable.gTempTableQueue[ubptr].uwTableInRamIndex == uwInRamIndex){\
	                        ubIndex = ubptr;\
	                        break;\
	                }\
	                else{\
	                        ubNext =  gTempTable.gTempTableQueue[ubptr].ubNext;\
	                        ubptr = ubNext;\
	                }\
	        }\
	        M_AssertError(gTempTable.gTempTableQueue[ubIndex].uwTableInRamIndex != uwInRamIndex);\
	        if(ubIndex != 0xFF){\
	                U32 ulGroupIndex;\
	                ulGroupIndex = gulL2P_InRamInverse[uwInRamIndex].B.ulL2PGroupIndex;\
	                if((ulGroupIndex != NoL2PTableHere)&&(uwInRamIndex  < L2P_IN_RAM_NUM)){\
				guwL2P_InRamIndex[ulGroupIndex] = TableNotInRAM;\
				gulL2P_InRamInverse[uwInRamIndex].B.btTableInRAMFlag = 0;\
				gulL2P_InRamInverse[uwInRamIndex].B.ulL2PGroupIndex = NoL2PTableHere;\
	                }\
	                else{\
			    	if(ENABLE_DEBUG_UART_YH){\
					UartWordHex(uwInRamIndex);\
					M_AssertError(1);\
			    	}\
	                }\
	                if ((ubIndex)!=gTempTable.ubLinkLast){\
	                        if (ubIndex==gTempTable.ubLinkFirst) {\
	                                gTempTable.ubLinkFirst = gTempTable.gTempTableQueue[ubIndex].ubNext;\
	                                gTempTable.gTempTableQueue[gTempTable.ubLinkFirst].ubPrevious = ubIndex;\
	                                gTempTable.gTempTableQueue[ubIndex].ubPrevious = gTempTable.ubLinkLast;\
			             if(ubIndex == gTempTable.ubLinkPtr){\
			     	             gTempTable.ubLinkPtr = gTempTable.ubLinkFirst;\
			             }\
	                        }\
	                        else {\
	                                UBYTE ubPrevious = gTempTable.gTempTableQueue[ubIndex].ubPrevious;\
	                                UBYTE ubNext = gTempTable.gTempTableQueue[ubIndex].ubNext;\
	                                if(ubIndex==gTempTable.ubLinkLastUse){\
	                                        gTempTable.ubLinkLastUse = gTempTable.gTempTableQueue[ubIndex].ubPrevious;\
	                                        if(ubIndex == gTempTable.ubLinkPtr){\
			     	                     gTempTable.ubLinkPtr = gTempTable.ubLinkLastUse;\
			                     }\
	                                }\
	                                else {\
					     if(ubIndex == gTempTable.ubLinkPtr){\
					     	     gTempTable.ubLinkPtr = gTempTable.gTempTableQueue[ubIndex].ubNext;\
					     }\
	                                }\
	                                gTempTable.gTempTableQueue[ubPrevious].ubNext = ubNext;\
	                                gTempTable.gTempTableQueue[ubNext].ubPrevious = ubPrevious;\
	                                gTempTable.gTempTableQueue[ubIndex].ubPrevious = gTempTable.ubLinkLast;\
	                                gTempTable.gTempTableQueue[ubIndex].ubNext = gTempTable.ubLinkFirst;\
	                                gTempTable.gTempTableQueue[gTempTable.ubLinkFirst].ubPrevious=ubIndex;\
	                        }\
	                        gTempTable.gTempTableQueue[gTempTable.ubLinkLast].ubNext = ubIndex;\
	                        gTempTable.ubLinkLast = ubIndex;\
	                 }\
	                else{\
			     if(ubIndex == gTempTable.ubLinkPtr){\
			     	     gTempTable.ubLinkPtr = gTempTable.gTempTableQueue[ubIndex].ubPrevious;\
			     }\
	                        gTempTable.ubLinkLastUse = gTempTable.gTempTableQueue[ubIndex].ubPrevious;\
	                }\
	                gTempTable.ubLinkNum--;\
	        }\
	}\
}

#define M_RemoveAllTempTable(){\
	UBYTE ubptr;\
	U32 ulGroupIndex;\
	ubptr =  gTempTable.ubLinkFirst;\
	while(gTempTable.ubLinkNum){\
		ulGroupIndex = gulL2P_InRamInverse[(gTempTable.gTempTableQueue[ubptr].uwTableInRamIndex&TableInTempMask)].B.ulL2PGroupIndex;\
		if(ulGroupIndex != NoL2PTableHere){\
			guwL2P_InRamIndex[ulGroupIndex] = TableNotInRAM;\
			if(gTempTable.gTempTableQueue[ubptr].uwTableInRamIndex < L2P_IN_RAM_NUM){\
				gulL2P_InRamInverse[(gTempTable.gTempTableQueue[ubptr].uwTableInRamIndex&TableInTempMask)].B.ulL2PGroupIndex = NoL2PTableHere;\
			}\
			else{\
				if(ENABLE_DEBUG_UART_YH){\
					Uart_Tx_DataHex(ubptr);\
					M_AssertError(1);\
			    	}\
			}\
		}\
		M_AssertError((gTempTable.gTempTableQueue[ubptr].uwTableInRamIndex&TableInTempMask)>=L2P_IN_RAM_NUM);\
		ubptr = gTempTable.gTempTableQueue[ubptr].ubNext;\
		gTempTable.ubLinkFirst = ubptr;\
		gTempTable.ubLinkNum--;\
	}\
	gTempTable.ubLinkPtr = gTempTable.ubLinkFirst;\
	gTempTable.ubLinkLast = gTempTable.gTempTableQueue[gTempTable.ubLinkFirst].ubPrevious;\
	gTempTable.ubLinkLastUse = gTempTable.ubLinkLast;\
}

#define M_GetMTDepth(ubDepth){\
	M_AssertError(gMTDepth.ubLinkNum== 0);\
	ubDepth = gMTDepth.DepthQueue[gMTDepth.ubLinkFirst];\
	gMTDepth.ubLinkNum--;\
	gMTDepth.ubLinkFirst = (gMTDepth.ubLinkFirst+1) % G_MT_QUEUE_DEPTH_MASK;\
}
// if num == 256 will overflow, so check if num == 0
#define M_AddMTDepth(ubDepth){\
	gMTDepth.DepthQueue[(gMTDepth.ubLinkFirst + gMTDepth.ubLinkNum) % G_MT_QUEUE_DEPTH_MASK] = ubDepth;\
	gMTDepth.ubLinkNum++;\
	M_AssertError(gMTDepth.ubLinkNum == 0);\
}

#define M_GetL2PTable(uwL2PGroupIndex){\
	if (guwL2P_InRamIndex[uwL2PGroupIndex] == TableNotInRAM) {\
		ftlSwapL2PTable(uwL2PGroupIndex, BYTE_Load_Normal_Mode);\
	}\
	else if((guwL2P_InRamIndex[uwL2PGroupIndex] & TableInTemp)){\
		M_AssertError((guwL2P_InRamIndex[uwL2PGroupIndex] & TableInTempMask)>=L2P_IN_RAM_NUM);\
		UWORD uwInRamIndex = (guwL2P_InRamIndex[uwL2PGroupIndex] & TableInTempMask);\
		M_RemoveTempTable(uwInRamIndex);\
		ftlSwapL2PTable(uwL2PGroupIndex, BYTE_Load_Normal_Mode);\
          }\
	else {\
		L2PTable = (L2PTable_t*)&gulL2PBase[(guwL2P_InRamIndex[uwL2PGroupIndex]&TableInTempMask) * guwL2PEntrysPerL2PGroup];\
	}\
}

// 原本就在RAM裡面的才去舉reference bit
#define M_TryToGetL2PTableFromWriteMode(uwL2PGroupIndex){\
	uwInRAMIndex = guwL2P_InRamIndex[uwL2PGroupIndex];\
	if (uwInRAMIndex == TableNotInRAM) {\
		ftlSwapL2PTable(uwL2PGroupIndex, BYTE_Load_Write_Mode);\
	}\
	else if((uwInRAMIndex& TableInTemp)){\
		M_RemoveTempTable((uwInRAMIndex & TableInTempMask));\
		ftlSwapL2PTable(uwL2PGroupIndex, BYTE_Load_Write_Mode);\
	}\
	else if (uwInRAMIndex >= guwWriteModeTableNum_ReadModeStartIndex) {\
		ftlMoveL2PTable(uwL2PGroupIndex);\
	}\
	else {\
		gulL2P_InRamInverse[uwInRAMIndex].B.btWaiting_Update = 1;\
		gubL2PSWAPorMoveDone = 0;\
	}\
}

#define M_ReadModeToWriteMode(uwL2PGroupIndex, uwL2P_ReadModeIndex){\
	UWORD uwWriteModeIndexForLoad;\
	U32 ulBeRemovedL2PGroup;\
	uwWriteModeIndexForLoad = ftlFindRAMToLoadL2P(BYTE_Find_In_Write_Mode);\
	if ((uwWriteModeIndexForLoad != Cannot_Save_And_Load) && (gulL2P_InRamInverse[uwWriteModeIndexForLoad].B.btDirty == 0))  {\
		/*if (ENABLE_DEBUG_UART) {\
			UartString("\nTarget");\
			UartWordHex(uwWriteModeIndexForLoad);\
			UartString(" ");\
			UartLongHex(gulL2P_InRamInverse[uwWriteModeIndexForLoad].All);\
			UartString(" Source");\
			UartWordHex(uwL2P_ReadModeIndex);\
			UartString(" ");\
			UartLongHex(gulL2P_InRamInverse[uwL2P_ReadModeIndex].All);\
		}*/\
		ulBeRemovedL2PGroup = gulL2P_InRamInverse[uwWriteModeIndexForLoad].B.ulL2PGroupIndex;\
		if (ulBeRemovedL2PGroup != NoL2PTableHere) {\
			guwL2P_InRamIndex[ulBeRemovedL2PGroup] = TableNotInRAM;\
		}\
		mSE_COPY((U32)&gulL2PBase[guwL2PEntrysPerL2PGroup * uwL2P_ReadModeIndex], (U32)&gulL2PBase[guwL2PEntrysPerL2PGroup * uwWriteModeIndexForLoad], 4096 << gub4kEntrysPerPlaneLog);\
		guwL2P_InRamIndex[uwL2PGroupIndex] = uwWriteModeIndexForLoad;\
		gulL2P_InRamInverse[uwWriteModeIndexForLoad].All = (BIT_FLAG_L2P | BIT_REFERENCE | BIT_VALID_L2P | uwL2PGroupIndex);\
		gulL2P_InRamInverse[uwL2P_ReadModeIndex].All = NoL2PTableHere;\
		uwL2P_ReadModeIndex = uwWriteModeIndexForLoad;\
		if (ENABLE_DEBUG_RW) {\
			ftlDebugSwapTable();\
		}\
	}\
}

#if (ENABLE_ODD_CE||ENABLE_SANDISK_1ZNM_SPF)
#define	DEF_PlanesPerPage_Mcl_Div		gubPlanesPerPage
#define DEF_CEPerPage_Mod_Mask			gubCEsPerPage
#define	DEF_PlanesPerPage_Mod_Mask		gubPlanesPerPage
#define DEF_BankPerPage_Mod_Mask		gubBanksPerPage
#define DEF_4KEntryPerPage_Mcl_Div		guw4kEntrysPerPage
#define DEF_CEPerPage_Mcl_Div			gubCEsPerPage
#define DEF_RSPlanesPerPage_Mcl_Div		guwRSPlanesPerPage
#define DEF_RSPlanesPerPage_Mod_Mask	guwRSPlanesPerPage
#define	DEF_TableParityPlanes_Mcl_Div		guwTableParityPlanesNum
#define DEF_TableParityPlanes_Mod_Mask	guwTableParityPlanesNum
#define DEF_4kEntrysPerUnit_Mcl_Div     gul4kEntrysPerUnit
#else
#define	DEF_PlanesPerPage_Mcl_Div		gubPlanesPerPageLog
#define DEF_CEPerPage_Mod_Mask			gubCEsPerPageMask
#define	DEF_PlanesPerPage_Mod_Mask		gubPlanesPerPageMask
#define DEF_BankPerPage_Mod_Mask	   	gubBanksPerPageMask
#define DEF_4KEntryPerPage_Mcl_Div		gub4kEntrysPerPageLog
#define DEF_CEPerPage_Mcl_Div			gubCEsPerPageLog
#define DEF_RSPlanesPerPage_Mcl_Div 	gubRSPlanesPerPageLog
#define DEF_RSPlanesPerPage_Mod_Mask   	guwRSPlanesPerPageMask
#define	DEF_TableParityPlanes_Mcl_Div		gubTableParityPlanesNumLog
#define DEF_TableParityPlanes_Mod_Mask  guwTableParityPlanesNumMax
#define DEF_4kEntrysPerUnit_Mcl_Div     gub4kEntrysPerUnitLog
#endif

#if (ENABLE_ODD_CE||ENABLE_SANDISK_1ZNM_SPF)
#define M_Divider(ulNumber, ulDivider) 			((ulNumber)/(ulDivider))

#define M_Multiplier(ulNumber,ulMultiplier) ((ulNumber) * (ulMultiplier))

#define M_Modular(ulNumber,ulModular)		((ulNumber) % (ulModular))
#else
#define M_Divider(ulNumber, ulDivider)			((ulNumber)>>(ulDivider))

#define M_Multiplier(ulNumber,ulMultiplier)	((ulNumber) << (ulMultiplier))

#define M_Modular(ulNumber,ulModular)		((ulNumber) & (ulModular))

#endif

#define M_Calculate_TableUnit_FreePlane(ubBeforeFlush){\
	UBYTE ubTableUnitIndex;\
	ubFreeTableUnitNum = 0;\
	if (ubBeforeFlush) {\
		gubVCZeroTableIndexListNum = 0;\
	}\
	for (ubTableUnitIndex = 0; ubTableUnitIndex < gubTableUnitNum; ubTableUnitIndex ++) {\
		if ((ubTableUnitIndex != VT->gubTableTargetIndex) && (guwTable_VC[ubTableUnitIndex] == 0)) {\
			ubFreeTableUnitNum ++;\
			if (ubBeforeFlush) {\
				gubVCZeroTableIndexList[gubVCZeroTableIndexListNum] = ubTableUnitIndex;\
				gubVCZeroTableIndexListNum ++;\
			}\
		}\
	}\
	if (VT->guwTableTargetPTR < guwFastPagePlanesPerUnit) {\
		ulActiveTableUnitRemainFreeNum = (guwTableRSParityNumPerUnit - (M_Divider(VT->guwTableTargetPTR,DEF_TableParityPlanes_Mcl_Div))) * (guwTableParityPlanesNum - 1) - (M_Modular(VT->guwTableTargetPTR, DEF_TableParityPlanes_Mod_Mask)) - gubPlanesPerTableP2LTable;\
	}\
	else {\
		ulActiveTableUnitRemainFreeNum = 0;\
	}\
	ulTableRemainFreeNum = ulActiveTableUnitRemainFreeNum + (guwTablePlanesPerUnit * ubFreeTableUnitNum);\
}

#define M_Check_Do_We_Need_Table_GC(ubTablePlanesNum){\
	if ((VT->guwTableTargetPTR + ubTablePlanesNum) > guwTableP2LPlanePtr) {\
		if (VT->guwTableTargetPTR < guwFastPagePlanesPerUnit) {\
			UBYTE ubActuralAddParityPlane;\
			if ((guwTableP2LPlanePtr - VT->guwTableTargetPTR) > guwTableParityPlanesNumMax) {\
				ubActuralAddParityPlane = (guwTableP2LPlanePtr - VT->guwTableTargetPTR) / gubPlanesPerPage;\
			}\
			else {\
				ubActuralAddParityPlane = 0;\
			}\
			ftlFillTable(guwTableP2LPlanePtr - ubActuralAddParityPlane - VT->guwTableTargetPTR);\
		}\
		gubGCTableIndexListNum = 0;\
		ftlGetVictimTableUnit(&VT->gubTableTargetIndex);\
		VT->gulFTLState.B.btNeedCloseTableTarget = 1;\
		M_AssertError(ubError && guwGCTableTargetVC);\
		ftlCloseTableTarget(VT->gubTableTargetIndex, BYTE_CloseTableTarget_Normal);\
	}\
}

#define M_DataInGR_Add(ulVir4kIndex){\
	UBYTE ubCmdUnit = 1;\
	while (SEB[SEB_SQ_WR_CNT] < 1);\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 0] = SE0_BF | (ubCmdUnit << 6) |(BF_K << 12) | BIT11 ;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 1] = (U32)gubDataInGR;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 2] = DataInGRSize*8;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 3] = ulVir4kIndex;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 4] = ULONG_FNV_Hash;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 5] = ULONG_FNV_PRIME;\
	SEB[SEB_SQ_WPIU] = 1;\
	while (SEB[SEB_CQ_RD_CNT] < 1);\
	SEB[SEB_CQ_RPIU] = 1;\
	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;\
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;\
	while (SEB[SEB_SQ_WR_CNT] < 1);\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 0] = SE0_SV | ubCmdUnit << 6;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 1] = (U32)gubZ2Buffer;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 2] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 3] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 4] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 5] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 6] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 7] = 0;\
	SEB[SEB_SQ_WPIU] = 1;\
	while (SEB[SEB_CQ_RD_CNT] < 1);\
	SEB[SEB_CQ_RPIU] = 1;\
	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;\
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;\
	gubSkipDataInGRCheck = 0;\
}
#define M_DataInGR_Check(ulVir4kIndex, ubHit){\
	UBYTE ubCmdUnit = 1;\
	while (SEB[SEB_SQ_WR_CNT] < 1);\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 0] = SE0_BF | (ubCmdUnit << 6) |(BF_K << 12) ;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 1] = (U32)gubDataInGR;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 2] = DataInGRSize*8;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 3] = ulVir4kIndex;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 4] = ULONG_FNV_Hash;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 5] = ULONG_FNV_PRIME;\
	SEB[SEB_SQ_WPIU] = 1;\
	while (SEB[SEB_CQ_RD_CNT] < 1);\
	SEB[SEB_CQ_RPIU] = 1;\
	UBYTE ubCQIndex = guwCQCnt_SE0;\
	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;\
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;\
	while (SEB[SEB_SQ_WR_CNT] < 1);\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 0] = SE0_SV | ubCmdUnit << 6;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 1] = (U32)gubZ2Buffer;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 2] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 3] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 4] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 5] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 6] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + 7] = 0;\
	SEB[SEB_SQ_WPIU] = 1;\
	while (SEB[SEB_CQ_RD_CNT] < 1);\
	SEB[SEB_CQ_RPIU] = 1;\
	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;\
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;\
	ubHit = (~SEL_CQBaseAddr[ubCQIndex * 8]) & BIT0;\
}

#define M_Check_Active_SLCPool() {\
	if (ENABLE_SLCPOOL_SYNC_TO_L2P && (VT->gulNeedCleanGRL4KCount == 0)) {\
		if ((gulSLCPoolSize_Sync == VT->gulSLCPoolSizeMax) && (VT->gulSLCPoolSize != gulSLCPoolSize_Sync)) {\
			UWORD uwUnitIndex;\
			U32 ulLostSLC = 0;\
			if (ENABLE_DEBUG_RW) {\
				for (uwUnitIndex = VTAREA_UNITS;uwUnitIndex < guwFWTotalUnitNum;uwUnitIndex ++) {\
					if (gulVC[uwUnitIndex].B.btFastPage && (gulVC[uwUnitIndex].B.ulValidCount != DefaultVC)) {\
						ulLostSLC += gulVC[uwUnitIndex].B.ulValidCount;\
					}\
				}\
				UartString("\nCan'tRelease...");\
				UartLongHex(VT->gulSLCPoolSize);\
				UartString("\nLost...");\
				UartLongHex(ulLostSLC);\
			}\
			VT->gulSLCPoolSize = gulSLCPoolSize_Sync;\
		}\
	}\
}

#define M_DoneEC_Check(ubEnableSwitchTask){\
        UWORD uw4kBufferIndex;\
	if(gulNCQRDCmdTriggerCnt){\
		UWORD uwECDone;\
		if(DCCMSATAB[SATAB_RD_CNT]){\
			while(DCCMSATAB[SATAB_RD_CNT]){\
				M_AssertError(gulNCQRDCmdTriggerCnt == 0);\
				UBYTE ubDoneTag=*DONETAG;\
				if(gubNCQDoing == 0){\
					ubDoneTag = 0;\
				}\
				UWORD uw4kIndex;\
				uwECDone = WR_NCQ_CMD_INFO[ubDoneTag].uwEC-WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone;\
				for(uw4kIndex = 0;uw4kIndex<uwECDone;uw4kIndex++){\
                                        uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubDoneTag].uwBufferPTR+ WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone + uw4kIndex)&gulBuffer2Read4KMask);\
					guoFWRead4kBufferFlag &= (~((ULLONG)BIT0<<uw4kBufferIndex));\
				}\
				if (ENABLE_DEBUG_NCQ_UART_R) {\
					UartString("  -");\
					Uart_Tx_DataHex(ubDoneTag);\
					UartString("  ");\
					UartLLongHex(guoFWRead4kBufferFlag);\
				}\
				WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone = WR_NCQ_CMD_INFO[ubDoneTag].uwEC;\
                           if(uwECDone && gubPreRead && ((gubPreReadStartTag == 0xFF) || (ubDoneTag == gubPreReadStartTag))){\
                                if((WR_NCQ_CMD_INFO[ubDoneTag].ulLBA + WR_NCQ_CMD_INFO[ubDoneTag].ulSectorCnt)&SectorsPer4KMask){\
                                                uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubDoneTag].uwBufferPTR+ WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone - 1)&gulBuffer2Read4KMask);\
                                                guoFWRead4kBufferFlag |= ((ULLONG)BIT0<<uw4kBufferIndex);\
                                                if (ENABLE_DEBUG_NCQ_UART_R) {\
	                                              UartString("  A:");\
                                                        UartWordHex(uw4kBufferIndex);\
                                                        UartString("  ");\
                                                        UartWordHex(WR_NCQ_CMD_INFO[ubDoneTag].uwBufferPTR);\
                                                        UartString("  ");\
                                                        UartWordHex(WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone);\
                                                        UartString("  ");\
                                                        UartWordHex(gulBuffer2Read4KMask);\
                                                        UartString("  ");\
                                                        UartWordHex(((WR_NCQ_CMD_INFO[ubDoneTag].uwBufferPTR+ WR_NCQ_CMD_INFO[ubDoneTag].uwEC_BQDone - 1)&gulBuffer2Read4KMask));\
                                                }\
	                                }\
                                        gubPreReadStartTag = 0xFF;\
                                }\
				if(gubPreRead && ((gubPreReadTQListFirst != gubPreReadTQListLast) || (gulNCQRDCmdTriggerCnt == 32))){\
					M_AssertError(ubDoneTag != gubPreReadTQList[gubPreReadTQListFirst]);\
					gubPreReadTQListFirst = (gubPreReadTQListFirst + 1) & 31;\
				}\
				if(gubNCQDoing){\
				gubLastDoneTag = ubDoneTag;\
				}\
				else{\
					gubLastDoneTag = 0x3F;\
				}\
				DCCMSATAB[SATAB_RPIU] = 1;\
				gulNCQRDCmdTriggerCnt--;\
				gubCmdDoneCount++;\
				gulNCQCMDDebug &= (~((U32)BIT0<<ubDoneTag));\
				if(gubSeqCmdCnt){\
				    gubSeqCmdCnt--;\
				}\
				if(gubLBAOverlap && (gubNeedWaitTQDone == 0)){\
					if(HB[HB_NCQ_OVLP_NUM]==0){\
						if((gubPreRead == 0) && (gubSeqR == 0)&& (gubSeqCmdCnt == 0)){\
							HL[HL_HAT_MISC]&= (~H_TQ_BY_ORDER);\
						}\
						gubLBAOverlap = 0;\
						gubNeedWaitTQDone = 0;\
						if (ENABLE_D2HSDBByHW == 1) {\
							HW[HW_AUTO_FIS_CTRL] |= (H_DMA_AUTO_REG_R_EN|H_NCQ_AUTO_SDB_R_EN);\
						}\
						if(HB[HB_NCQ_OVLP_NUM]){\
							gubLBAOverlap = 1;\
							gubNeedWaitTQDone = 1;\
						}\
					}\
				}\
			}\
		}\
		else{\
			if(gul4KRandomCacheRead == 0){\
				SATA_MSG_TYPE3 ulSATA_MSG_TYPE3;\
				ulSATA_MSG_TYPE3.All = gulSATA_MSG_TYPE3[0].All;\
				UBYTE ubCurrentTag;\
				if(gubNCQDoing){\
					ubCurrentTag = ulSATA_MSG_TYPE3.B.ubCurrentTag;\
				}\
				else{\
					ubCurrentTag = 0;\
				}\
				if(WR_NCQ_CMD_INFO[ubCurrentTag].uwEC != WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone){\
					if((gubNCQDoing==0) || ((gubPreRead == 0) || (ubCurrentTag == gubPreReadTQList[gubPreReadTQListFirst]))){\
						if(gubNCQDoing && (ubCurrentTag != gubLastDoneTag) && (gubLastDoneTag != 0x3F)){\
					if(ulSATA_MSG_TYPE3.B.uwReceived_EC>WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone){\
						uwECDone = ulSATA_MSG_TYPE3.B.uwReceived_EC-WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone;\
						UWORD uw4kIndex;\
						for(uw4kIndex = 0;uw4kIndex<uwECDone; uw4kIndex++){\
                                                        uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubCurrentTag].uwBufferPTR + WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone+ uw4kIndex)&gulBuffer2Read4KMask);\
							guoFWRead4kBufferFlag &= (~((ULLONG)BIT0<<uw4kBufferIndex));\
						}\
						WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone = ulSATA_MSG_TYPE3.B.uwReceived_EC;\
                                                 if((WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone == WR_NCQ_CMD_INFO[ubCurrentTag].uwEC) && gubPreRead&& ((gubPreReadStartTag == 0xFF) || (ubCurrentTag == gubPreReadStartTag))){\
                                                        if((WR_NCQ_CMD_INFO[ubCurrentTag].ulLBA + WR_NCQ_CMD_INFO[ubCurrentTag].ulSectorCnt)&SectorsPer4KMask){\
                                                        uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubCurrentTag].uwBufferPTR+ WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone - 1)&gulBuffer2Read4KMask);\
                                                                guoFWRead4kBufferFlag |= ((ULLONG)BIT0<<uw4kBufferIndex);\
                                                                 if (ENABLE_DEBUG_NCQ_UART_R) {\
                                                                UartString("  B:");\
                                                                        UartWordHex(uw4kBufferIndex);\
                                                                }\
                                                        }\
                                                         gubPreReadStartTag = 0xFF;\
                                                }\
					}\
				}\
				else{\
					if((gubNCQDoing == 0)||((HB[HB_TQ_ACT]&H_TQ_ACT) && ((HB[HB_NCQ_SET]&0x1F) == ubCurrentTag))){\
						U32 ulRemainSectorCnt = HL[HL_CMD_REMAIN_SEC];\
						U32 ulSectorCntDone = (WR_NCQ_CMD_INFO[ubCurrentTag].ulSectorCnt -ulRemainSectorCnt);\
						uwECDone = ((ulSectorCntDone + (WR_NCQ_CMD_INFO[ubCurrentTag].ulLBA & SectorsPer4KMask))/8);\
						if((ulRemainSectorCnt < WR_NCQ_CMD_INFO[ubCurrentTag].ulSectorCnt)&&(uwECDone>WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone)){\
							uwECDone = uwECDone-WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone;\
							UWORD uw4kIndex;\
							for(uw4kIndex = 0;uw4kIndex<uwECDone; uw4kIndex++){\
                                                                uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubCurrentTag].uwBufferPTR + WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone+ uw4kIndex)&gulBuffer2Read4KMask);\
								guoFWRead4kBufferFlag &= (~((ULLONG)BIT0<<uw4kBufferIndex));\
							}\
							WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone += uwECDone;\
                                                          if((WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone == WR_NCQ_CMD_INFO[ubCurrentTag].uwEC) && gubPreRead&& ((gubPreReadStartTag == 0xFF) || (ubCurrentTag == gubPreReadStartTag))){\
                                                                if((WR_NCQ_CMD_INFO[ubCurrentTag].ulLBA + WR_NCQ_CMD_INFO[ubCurrentTag].ulSectorCnt)&SectorsPer4KMask){\
                                                                uw4kBufferIndex = ((WR_NCQ_CMD_INFO[ubCurrentTag].uwBufferPTR+ WR_NCQ_CMD_INFO[ubCurrentTag].uwEC_BQDone - 1)&gulBuffer2Read4KMask);\
                                                                        guoFWRead4kBufferFlag |= ((ULLONG)BIT0<<uw4kBufferIndex);\
                                                                         if (ENABLE_DEBUG_NCQ_UART_R) {\
                                                                        UartString("  C:");\
                                                                                UartWordHex(uw4kBufferIndex);\
						}\
					}\
                                                                 gubPreReadStartTag = 0xFF;\
                                                        }\
				}\
	                                        }\
	                                }\
			}\
			}\
			}\
			if(ubEnableSwitchTask){\
			M_SwitchTask();\
		}\
	}\
	}\
	else if(ubEnableSwitchTask){\
		M_SwitchTask();\
	}\
}

#define M_FastPage(ulFastPageSector, ulFSector){\
	UWORD uwPageIndex = (ulFSector >> gubSectorsPerPlaneLog);\
	UWORD uwFastPageIndex = uwPageIndex;\
	switch (gubFastPageType) {\
		case 0x21:\
			if (uwPageIndex > 1) {\
				uwFastPageIndex = (uwPageIndex << 1) - 1;\
			}\
			break;\
		case 0x4A:\
			if (uwPageIndex > 1) {\
				uwFastPageIndex = (uwPageIndex << 1) - 2 - (uwPageIndex & BIT0);\
			}\
			break;\
		case 0x4B:\
			if (uwPageIndex > 5) {\
				uwFastPageIndex = (uwPageIndex << 1) - 4 - (uwPageIndex & BIT0);\
			}\
			break;\
		case 0x4C:\
			if (uwPageIndex > 63) {\
				uwFastPageIndex = (uwPageIndex << 1) - 63;\
			}\
			break;\
		case 0x00:\
			uwFastPageIndex = uwPageIndex;\
			break;\
		case 0x2C:\
			uwFastPageIndex = ((uwPageIndex >> 1) << 2) + (uwPageIndex & BIT0);\
			break;\
		case 0x2A:\
			uwFastPageIndex = (uwPageIndex << 1);\
			break;\
		default:\
			break;\
	}\
	ulFastPageSector =  ((uwFastPageIndex << gubSectorsPerPlaneLog) + (ulFSector & gubSectorsPerPlaneMask));\
}\
 
#define M_GetFQ(FQ, ubDepth, ubL4kLink, ubCEIndex2){\
	while ((gFQI.ubFQLinkNumber >= gFQ_NUM) || (gMTDepth.ubLinkNum == 0)) M_SwitchTask();\
	FQ = &gFQI.gFQLink[gFQI.ubFQLinkFree];\
	FQ->ubCEIndex = ubCEIndex2;\
	ubL4kLink = gFQI.ubFQLinkFree;\
	M_GetMTDepth(ubDepth);\
}\
 
#define M_CheckZcodeZone(ubZByteNum, ulPhyEntry){\
	if (gubCleanGRinMiddle == 0) {\
			if (VT->gulGRTableL4kPTR == 0) {\
				gubZcodeA =  ubZByteNum;\
				gulRandomZcodeZoneBegin = 0;\
				gulZoneBBegin = 0;\
				gubZcodeB = 0xFF;\
			}\
			else if (gulRandomZcodeZoneBegin == gulZoneBBegin) {\
				if (gubZcodeB == 0xFF) {\
					if (ubZByteNum != gubZcodeA) {\
						gulRandomZcodeZoneBegin =  VT->gulGRTableL4kPTR;\
						gulZoneBBegin =  gulRandomZcodeZoneBegin;\
						gubZcodeB =  ubZByteNum;\
					}\
					gulRandomZcodeZoneBegin_PhyEntry = ulPhyEntry;\
				}\
				else {\
					if (ubZByteNum == gubZcodeB) {\
						gulZoneBBegin = VT->gulGRTableL4kPTR;\
					}\
					else {\
						gubZcodeB =  ubZByteNum;\
					}\
					gulZoneBBegin_PhyEntry =ulPhyEntry;\
				}\
			}\
			else if (ubZByteNum != gubZcodeB) {\
				gulZoneBBegin = gulRandomZcodeZoneBegin;\
				gubZcodeB =  ubZByteNum;\
				gulZoneBBegin_PhyEntry = ulPhyEntry;\
			}\
	}\
}\
//It is pseudo CE, need re-calculate Fblock with multi-die
#define M_Check_PseudoCE(ubFQCEIndex, uwFBlock){\
	if((ENABLE_PSEUDO_CE)&&(ubFQCEIndex>=gubMTQTotalPhyCE)){\
		uwFBlock = uwFBlock + (UWORD)(gubDieNumber*guwDieBlockShift*(ubFQCEIndex/gubMTQTotalPhyCE));\
	}\
	if(ENABLE_SANDISK_1ZNM_SPF){\
		if((ubFQCEIndex%gubMTQTotalPhyCE)>=(gubHideCE*gubPlanesPerBurst)){\
			uwFBlock += (24576/gubBurstsPerBank);\
		}\
	}\
}\
 
#define M_VirtualToPhysicalCE(ubCEIndex){\
	if( ENABLE_PSEUDO_CE && gubExpandCE){\
		ubCEIndex = (ubCEIndex == gubCENumber)? gubMTQTotalPhyCE : ( ubCEIndex%gubMTQTotalPhyCE);\
	}\
}\
 
//CE Decoder
#if !UFS
#define M_MapToCEDecoder(ubChannel, ubCE)	((BIT7 | (ubCE << 1)) << (8 * ubChannel))

#define M_MapToCEDecoderMT(ubCE)	(BIT7 | (ubCE << 1))
#endif
#define M_Get_Minimum_UsedUnit_EC(){\
        if ((gulVC[VT->guwMinimumEC_UsedUnit].B.ulValidCount == DefaultVC) || (gulEC_D3[VT->guwMinimumEC_UsedUnit].B.ulEraseCount == DefaultEC)) {\
                UBYTE ubDontStopSearch = 1;\
                while (ubDontStopSearch) {\
                        while (SEB[SEB_SQ_WR_CNT] < 1);\
                        if (ENABLE_DCACHE) {\
                                xthal_dcache_region_writeback_inv((void *)( (U32)gulEC_D3), (guwFWTotalUnitNum * 4));\
                        }\
                        SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_CMD] = SE0_SMaxSMin | SE0_CMDUNIT_1 | SE0_SIZE_4BYTE | SE0_ENABLE_MASK | SE0_MINMAX_MIN | SE0_MINMAX_NEQ;\
                        SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_bMASK0_L] = BIT_UsedDataUnit;\
                        SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_bMASK0_H] = DefaultEC;\
                        SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_SADDR0] = (U32)gulEC_D3;\
                        SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_PAT0_L] = 0;\
                        SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_PAT0_H] = 0;\
                        SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_LEN] =   (guwFWTotalUnitNum * 4);\
                        SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_DEP] = 0;\
                        SEB[SEB_SQ_WPIU] = 1;\
                        while (SEB[SEB_CQ_RD_CNT] < 1);\
                        SEB[SEB_CQ_RPIU] = 1;\
                        UWORD uwMinECTarget = SEL_CQBaseAddr[guwCQCnt_SE0 * 8 + 1];\
                        guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;\
                        guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;\
                        if (gulEC_D3[uwMinECTarget].B.btUsedDataUnit == 0) {\
                                VT->guwMinimumEC_UsedUnit = uwMinECTarget;\
                                break;\
                        }\
                        if (gulVC[uwMinECTarget].B.ulValidCount == DefaultVC) {\
                                gulEC_D3[uwMinECTarget].B.btUsedDataUnit = 0;\
                        }\
                        else {\
                                ubDontStopSearch = 0;\
                                VT->guwMinimumEC_UsedUnit = uwMinECTarget;\
                        }\
                }\
        }\
}\
 
#define M_Check_LastCMD_StandBy(ubSaveVT_Location){\
        if ((!BURNER) && (VT->gubWriteProtectState != BYTE_STATE_WRITEPROTECT) && (VT->gulFTLState.B.btLast_CMD_Not_StandBy == 0)) {\
                VT->gulFTLState.B.btLast_CMD_Not_StandBy = 1;\
                HandleStopRW(1);\
                ftlSaveVT(0, ubSaveVT_Location);\
                if (ENABLE_POWER_CYCLE_DONT_CNT_EVENT) {\
	}\
                gubStopFTL = 0;\
        }\
}

//#if (ENABLE_ULTRAMLC && DEBUG_ULTRAMLC_POWER_FAIL)
#if (ENABLE_PFAIL && DEBUG_ULTRAMLC_POWER_FAIL)
#if 0
#define _ASSERT(loc, condition) do { \
			if (condition) { \
				flaLEDBlink(1); \
			} \
		} while(0)
#else
#define _ASSERT(loc, condition) M_AssertCriticalError(condition, (0x80 | loc))
#endif

#define _DEBUG_TRACE(x) do { \
				trace_arr[trace_arr_index] = (x); \
				trace_arr_index ++; \
				trace_arr_index &= 0x3FF; \
		}while(0)
#else
#define _ASSERT(loc, condition)
#define _DEBUG_TRACE(x)
#endif

#define M_AssertError(ubFailCondition){\
	if(ENABLE_DEBUG_RW && (ubFailCondition)){\
		flaLEDBlink(0);\
	}\
}\
 
#define M_AssertCriticalError(ubFailCondition, ubSaveVTSerialNumber){\
        if(ubFailCondition){\
                if (ENABLE_DEBUG_RW == 0) {\
                        flaDumpError(1, ubSaveVTSerialNumber);\
                }\
                else{\
                        flaLEDBlink(0);\
                }\
        }\
}\
 
#define M_AssertResetError(ubFailCondition, ubSaveVTSerialNumber){\
        if(ubFailCondition){\
                if (ENABLE_ERROR_JUMP_RESET && (ENABLE_DEBUG_RW == 0)) {\
                        flaDumpError(0, ubSaveVTSerialNumber);\
                }\
                else{\
                	flaLEDBlink(0);\
                }\
        }\
}\
 
#define M_CheckBurnerModeToDisableFunction(){\
        if(BURNER){\
                return;\
        }\
}

#define M_CheckNonRDTModeToDisableFunction(){\
        if(!RDT){\
                return;\
        }\
}
#define M_CheckNonRDTModeToDisableFunctionReturnValid(X){\
        if(!RDT){\
                return X;\
        }\
}

#define M_CheckRDTModeToDisableFunction(){\
        if(RDT){\
                return;\
        }\
}

#define M_CheckRDTModeToDisableFunctionReturnValid(X){\
        if(RDT){\
                return X;\
        }\
}

#define M_CheckBurnerModeToDisableFunctionReturnValid(X){\
        if(BURNER){\
                return X;\
        }\
}

#define M_CheckNonBurnerModeToDisableFunction(){\
        if(!BURNER){\
                return;\
        }\
}\
 
#define M_CheckNonBurnerModeToDisableFunctionReturnValid(X){\
        if(!BURNER){\
                return X;\
        }\
}

#define M_SEQ_Sector_Calculation(ulExpectedPhySector, ulVir4kIndexInL2POffset, uwL2PGroupIndex, ulSeqHeadFEntry, ubSeqHeadOffset, ubSeqHeadZByteNum){\
	Seq_Table_t ulSeqHead;\
	UWORD uwHeadParityIndex;\
	UWORD uwTargetParityIndex;\
	UBYTE ubFastPage;\
	U32 ulSeqHeadPlanePTR;\
	U32 ulTargetPlanePTR;\
	if(gulDiskSizeInMB <= (512*1024)){\
		ulSeqHead = guoSequential_PhyHead[uwL2PGroupIndex];\
	}\
	if((gulDiskSizeInMB > (512*1024)) || (ulSeqHead.B.uwNextUnit == 0xFFFF) || (ulVir4kIndexInL2POffset < ulSeqHead.B.uwNewUnitStartOffset)){\
		ulSeqHeadPlanePTR = (ulSeqHeadFEntry % gul4kEntrysPerUnit)>>gub4kEntrysPerPlaneLog;\
		ulExpectedPhySector = (ulSeqHeadFEntry<<SectorsPer4KLog) + (ulVir4kIndexInL2POffset * ubSeqHeadZByteNum) + ubSeqHeadOffset;\
		ubFastPage = gulVC[ulSeqHeadFEntry/ gul4kEntrysPerUnit].B.btFastPage;\
	}\
	else{\
		ulSeqHeadPlanePTR= 0;\
		ulExpectedPhySector = (ulSeqHead.B.uwNextUnit * gulSectorsPerUnit)  + ((ulVir4kIndexInL2POffset - ulSeqHead.B.uwNewUnitStartOffset) * ubSeqHeadZByteNum);\
		ubFastPage = gulVC[ulSeqHead.B.uwNextUnit].B.btFastPage;\
	}\
	ulTargetPlanePTR = (ulExpectedPhySector>>gubSectorsPerPlaneLog) % gulPlanesPerUnit;\
	if(ubFastPage){\
		for(uwHeadParityIndex = 0; guwParityMappingTableSLC[uwHeadParityIndex]<ulSeqHeadPlanePTR; uwHeadParityIndex++);\
		for(uwTargetParityIndex = uwHeadParityIndex; guwParityMappingTableSLC[uwTargetParityIndex]<=ulTargetPlanePTR; uwTargetParityIndex++){\
			ulTargetPlanePTR += 2;\
			ulExpectedPhySector += (gubSectorsPerPlane*2);\
		}\
	}\
	else{\
		for(uwHeadParityIndex = 0; guwParityMappingTable[uwHeadParityIndex]<ulSeqHeadPlanePTR; uwHeadParityIndex++);\
		for(uwTargetParityIndex = uwHeadParityIndex; guwParityMappingTable[uwTargetParityIndex]<=ulTargetPlanePTR; uwTargetParityIndex++){\
			ulTargetPlanePTR += 2;\
			ulExpectedPhySector += (gubSectorsPerPlane*2);\
		}\
	}\
}\
 
//清空gulSATA_MSG_TYPE3與gubLastDoneTag交給resetCmdIn去做
#define M_CheckDoneTag(X){\
	while (ENABLE_READ_DONETAG && (DCCMSATAB[SATAB_RD_CNT] == 0)){\
		if(gubNeedChkDoneTag == 0){\
			VT->gulDoneTagMiss++;\
			break;\
		}\
	}\
	DCCMSATAB[SATAB_RPIU] = DCCMSATAB[SATAB_RD_CNT];\
	gubLastDoneTag = 0x3F;\
}\
 
#define M_ACC_PARSE_L2P(ulL2P, ulCurrentFEntry, ubCurrentFEntryOffset, ubCurrentFEntryZByteNum){\
	ACCL_BaseAddr[ACC_L2P] = ulL2P;\
	ulCurrentFEntry = ACCL_BaseAddr[ACC_PCA];\
	ubCurrentFEntryOffset = ACCB_BaseAddr[ACC_OFFSET];\
	ubCurrentFEntryZByteNum = ACCB_BaseAddr[ACC_LENGTH];\
	if(gubL2P_FORMAT_MODE == 1){\
		ubCurrentFEntryZByteNum *= 2;\
		ubCurrentFEntryOffset *= 2;\
	}\
}\
 
#define M_ACC_GEN_L2P(ubOffset, ubZbyteNum, ulFEntry, ulL2P){\
	if(gubL2P_FORMAT_MODE == 1){\
		ACCB_BaseAddr[ACC_OFFSET] = (ubOffset >> 1);\
		ACCB_BaseAddr[ACC_LENGTH] = (ubZbyteNum >> 1) + 1;\
	}\
	else{\
		ACCB_BaseAddr[ACC_OFFSET] = ubOffset;\
		ACCB_BaseAddr[ACC_LENGTH] = ubZbyteNum + 1;\
	}\
	ACCL_BaseAddr[ACC_PCA] = ulFEntry;\
	ulL2P = ACCL_BaseAddr[ACC_L2P];\
}\
 
#define M_Check_TRIM(ubNeedToDoTRIM){\
	if (ENABLE_DCACHE) {\
		xthal_dcache_region_writeback_inv((void *)((U32)gubTrimTable), TRIMSize);\
	}\
	while (SEB[SEB_SQ_WR_CNT] < 1);\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_CMD] = SE0_SNEQ | SE0_CMDUNIT_1 | SE0_SIZE_4BYTE | SE0_MATCH_NUMBER_6 | SE0_ENABLE_MASK;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_SADDR0] = (U32)gubTrimTable;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_LEN] = TRIMSize;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_DEP] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_PAT0_L] =  0x00000000;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_PAT0_H] = 0;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_bMASK0_L] = 0xFFFFFFFF;\
	SEL_SQBaseAddr[guwSQCnt_SE0 * 8 + SE0_bMASK0_H] = 0;\
	SEB[SEB_SQ_WPIU] = 1;\
	while (SEB[SEB_CQ_RD_CNT] < 1);\
	SEB[SEB_CQ_RPIU] = 1;\
	if (SEL_CQBaseAddr[guwCQCnt_SE0 * 8] >> 16) {\
		ubNeedToDoTRIM = 1;\
	}\
	guwSQCnt_SE0 = (guwSQCnt_SE0 + 1) % MaxSQCnt_SE;\
	guwCQCnt_SE0 = (guwCQCnt_SE0 + 1) % MaxCQCnt_SE;\
}

#define M_Reload_DEVDLP_Table(){\
	UBYTE ubi,ubj;\
	for (ubi = 0;ubi < 2;ubi ++) {\
		for(ubj = 0; ubj < MAX_CHANNEL; ubj++) {\
			guwSysCodeBlock[ubi][ubj] = gDEVSLP_Table->uwSysCodeBlock[ubi][ubj];\
		}\
		gubSysCodeSet[ubi] = gDEVSLP_Table->ubSysCodeSet[ubi];\
		gubDeferredCodeSet[ubi] = gDEVSLP_Table->ubDeferredCodeSet[ubi];\
	}\
	for(ubi = 0; ubi < MAX_CHANNEL; ubi++) {\
		guwDBTBlock[ubi] = gDEVSLP_Table->uwDBTBlock[ubi];\
	}\
	gubDBTSet = gDEVSLP_Table->ubDBTSet;\
	gubDEVSLPSupport = gDEVSLP_Table->ubDEVSLPSupport;\
	gubSystemSetFreeCount = gDEVSLP_Table->ubSystemSetFreeCount;\
	gubSystemSetFreeQueueOutPTR = gDEVSLP_Table->ubSystemSetFreeQueueOutPTR;\
	gubSystemSetFreeQueueInPTR = gDEVSLP_Table->ubSystemSetFreeQueueInPTR;\
	gubPowerMode = gDEVSLP_Table->ubPowerMode;\
	gubRTD_enable = gDEVSLP_Table->ubRTD_Enable;\
	gubDLMCStep = gDEVSLP_Table->ubDLMCStep;\
	gubLastDLMCMode = gDEVSLP_Table->ubLastDLMCMode;\
	gubHeaderTransferSectorCnt = gDEVSLP_Table->ubHeaderTransferSectorCnt;\
	gubDoPreformatAfterDLMC = gDEVSLP_Table->ubDoPreformatAfterDLMC;\
	gulBurnerTransferSectorCnt = gDEVSLP_Table->ulBurnerTransferSectorCnt;\
	gulMicroCodeTransferSectorCnt = gDEVSLP_Table->ulMicroCodeTransferSectorCnt;\
	for (ubi = 0;ubi < SYSTEM_SETS;ubi ++) {\
		gubSystemSetFreeTable[ubi] = gDEVSLP_Table->ubSystemSetFreeTable[ubi];\
	}\
	memcpy(&gXferMode, &gDEVSLP_Table->uoXferMode, sizeof(gXferMode));\
	gubACTPMEnable= gDEVSLP_Table->ubACTPMEnable;\
	gubACTPMValue = gDEVSLP_Table->ubACTPMValue;\
	IDT = *IDT_temp;\
}

#define M_Save_DEVDLP_Table(){\
	UBYTE ubi,ubj;\
	for (ubi = 0;ubi < 2;ubi ++) {\
		for(ubj = 0; ubj < MAX_CHANNEL; ubj++) {\
			gDEVSLP_Table->uwSysCodeBlock[ubi][ubj] = guwSysCodeBlock[ubi][ubj];\
		}\
		gDEVSLP_Table->ubSysCodeSet[ubi] = gubSysCodeSet[ubi];\
		gDEVSLP_Table->ubDeferredCodeSet[ubi] = gubDeferredCodeSet[ubi];\
	}\
	if(HW[HW_SATA_INFO]&(H_SLUMBER|H_PARTIAL)){\
		gDEVSLP_Table->ubSATAPhyStatus = 0;\
	}\
	else{\
		gDEVSLP_Table->ubSATAPhyStatus = 1;\
	}\
	gDEVSLP_Table->ubHostCmdIn = 0;\
 	gDEVSLP_Table->ubSATACmdCode = 0;\
	gDEVSLP_Table->ulFirstMark = 0x31113111;\
	for(ubi = 0; ubi < MAX_CHANNEL; ubi++) {\
		gDEVSLP_Table->uwSystemBlock[ubi] = guwSystemBlock[ubi];\
		gDEVSLP_Table->uwDBTBlock[ubi] = guwDBTBlock[ubi];\
	}\
	gDEVSLP_Table->ubSystemSet = gubSystemSet;\
	gDEVSLP_Table->ubDBTSet = gubDBTSet;\
	gDEVSLP_Table->ubDEVSLPSupport = gubDEVSLPSupport;\
	gDEVSLP_Table->ubSystemSetFreeCount = gubSystemSetFreeCount;\
	gDEVSLP_Table->ubSystemSetFreeQueueOutPTR = gubSystemSetFreeQueueOutPTR;\
	gDEVSLP_Table->ubSystemSetFreeQueueInPTR = gubSystemSetFreeQueueInPTR;\
	gDEVSLP_Table->ubVTIndex = VT->gubVTAreaFreeQueueInPTR;\
	gDEVSLP_Table->uwVT_PTR = VT->guwVTPTR - (gubPlanesPerVT << ENABLE_VT_BACKUP);\
	if (VT->guwVTChildPTR) {\
	gDEVSLP_Table->uwVTChild_PTR = VT->guwVTChildPTR - (gubPlanesPerVT << ENABLE_VT_BACKUP);\
	}\
	else {\
		gDEVSLP_Table->uwVTChild_PTR = 0xFFFF;\
	}\
	gDEVSLP_Table->ubPowerMode = gubPowerMode;\
	gDEVSLP_Table->ubRTD_Enable = gubRTD_enable;\
	gDEVSLP_Table->ubDLMCStep = gubDLMCStep;\
	gDEVSLP_Table->ubLastDLMCMode = gubLastDLMCMode;\
	gDEVSLP_Table->ubHeaderTransferSectorCnt = gubHeaderTransferSectorCnt;\
	gDEVSLP_Table->ubDoPreformatAfterDLMC = gubDoPreformatAfterDLMC;\
	gDEVSLP_Table->ulBurnerTransferSectorCnt = gulBurnerTransferSectorCnt;\
	gDEVSLP_Table->ulMicroCodeTransferSectorCnt = gulMicroCodeTransferSectorCnt;\
	for (ubi = 0;ubi < SYSTEM_SETS;ubi ++) {\
		gDEVSLP_Table->ubSystemSetFreeTable[ubi] = gubSystemSetFreeTable[ubi];\
	}\
	memcpy(&gDEVSLP_Table->uoXferMode, &gXferMode, sizeof(gXferMode));\
	gDEVSLP_Table->ubACTPMEnable= gubACTPMEnable;\
	gDEVSLP_Table->ubACTPMValue = gubACTPMValue;\
	gDEVSLP_Table->ulLastMark = 0xDDBADBAD;\
	*IDT_temp = IDT;\
}

// BG時不要管flag, write FQ用 copy mode, read FQ用 buffer mode(為了發signoff fail)
// 塞write FQ前必須等read FQ做完, 而且要判斷有無發生UNC, 有的話就停掉BG, signoff用FW flag 去記
#define M_StartBG_Copy() {\
	if ((gubFlushCache_StopBG && (VT->guwGRTarget[VT->gubGRTargetIndex].B.btFastPage == 0) && (!TLC)) || VT->gulFTLState.B.btFlushCache) {\
		gubTimeToDoBG = BYTE_BG_LEVEL_IDLE;\
	}\
	else if ((gubCMDFinish == 1) && ENABLE_BACKGROUND_COPY && (!(gubIsWriteProtect & BIT_TRULY_WRITEPROTECT))) {\
	gubTimeToDoBG = BYTE_BG_LEVEL_COPY;\
	M_SetFlagSettingNone();\
	}\
	else {\
		gubTimeToDoBG = BYTE_BG_LEVEL_IDLE;\
	}\
}
// 停掉BG時, 會在HandleStopRW裡等到 ftl 停掉 BG, 再把 flag auto check / set 打開
#define M_StopBG_Copy() {\
	gubTimeToDoBG = 0;\
	HandleStopRW(0);\
}
// Start CleanGR
// gubPartialCleanP2LDoing = 0;是因為 partial build GCTable會令gubPartialCleanP2LDoing = 1
#if BICS2_GC_TO_D3
#define M_Set_NeedCleanGR() {\
		if (VT->gulFTLState.B.btNeedCleanGR == 0) {\
				VT->gulFTLState.B.btNeedCleanGR = 1;\
				VT->gulNeedCleanGRL4KCount = 0;\
				if (gubBuildGCTable) {\
						guwPartialBuildGCTableCnt = 0;\
	                    gulP2LGroupCount_Backup = 0;\
						gubBuildGCTable = 0;\
						gubPartialCleanP2LDoing = 0;\
						if (gubLockReadMode == TRUE) {\
								ftlSwapL2PTable(guwLockReadNum, BYTE_Free_Read_Mode);\
								ftlSwapL2PTable(guwLockWriteNum, BYTE_Free_Write_Mode);\
						}\
				}\
		}\
}
#else
#define M_Set_NeedCleanGR() {\
        if (VT->gulFTLState.B.btNeedCleanGR == 0) {\
                VT->gulFTLState.B.btNeedCleanGR = 1;\
                VT->gulNeedCleanGRL4KCount = 0;\
                if (gubBuildGCTable) {\
                        guwPartialBuildGCTableCnt = 0;\
                        gulP2LGroupCount_Backup = 0;\
                        gubBuildGCTable = 0;\
                        gubPartialCleanP2LDoing = 0;\
                        ftlSwapL2PTable(guwLockReadNum, BYTE_Free_Read_Mode);\
                }\
        }\
}
#endif


#define M_Clear_UNC_Addr() {\
	gubUncPtr = 0;\
	gulReadUNCWaitCount = 0;\
	HL[HL_RD_UNC_ADDR] = 0;\
}

#define M_Get_Error_4K_Index(ubErrorIndex, ubMaxErrorIndex, ulErrorMap) {\
	while (ubErrorIndex < ubMaxErrorIndex) {\
		if ((ulErrorMap >> ubErrorIndex) & BIT0) {\
			break;\
		}\
		ubErrorIndex++;\
	}\
}
#define SV_ALIGNED_32(x) (((x)+31UL)&(~31UL))
#if TLC
// For Multi-Die
#define M_GetDieNumber(ubDie, uwUnit, ubSelect){\
        UWORD uwPhyUnitIndexOfThisDie;\
        UWORD uwUnitInRUT;\
        for(ubDie = 0; ubDie < gubSelDieNumber; ubDie++){\
		if(gubRUTMixPlaneEnable){\
			uwPhyUnitIndexOfThisDie = (((ubDie + 1) * (gulTotalPhyUnitNum >> gubDieNumberLog)) << gubBurstsPerBankLog) - 1;\
		}\
		else{\
                uwPhyUnitIndexOfThisDie = ((ubDie + 1) * (gulTotalPhyUnitNum >> gubDieNumberLog)) - 1;\
		}\
                if(ubSelect == D3){\
                        uwUnitInRUT = uwUnit;\
                }\
                else if(ubSelect == D1){\
                        uwUnitInRUT= guwFWTotalUnitNum + uwUnit * D1_UNIT_NUM_PER_LINK;\
                }\
		if(M_RUT2DTo1D(0, (uwUnitInRUT << gubBurstsPerBankLog)) <= uwPhyUnitIndexOfThisDie){\
                        break;\
                }\
        }\
}
#endif

#define M_MicroCodeTrigger(){\
	if (uwTransferSectorCnt == 0) {\
		if (uwDivide16K_Quotient) {\
			uwTransferSectorCnt = 32;\
			uwDivide16K_Quotient --;\
		}\
		else {\
			uwTransferSectorCnt = uwDivide16K_Remainder;\
		}\
		RWBuffer_forSMARTLog((U32)uwTransferSectorCnt, (U32)ubOffsetInSector);\
		if (ENABLE_DCACHE) {\
			xthal_dcache_region_invalidate((void *)&BUFB_BASE[0], 16*1024);\
		}\
		uwSectorCnt -= uwTransferSectorCnt;\
		uwSectorPTR = (UWORD)(ubOffsetInSector*512);\
		ubOffsetInSector = ((ubOffsetInSector+uwTransferSectorCnt) & 0x07);\
	}\
	uwTransferSectorCnt --;\
	/* break while ((uwSectorCnt != 0) || (uwTransferSectorCnt != 0)) loop in DLMC */\
	if (gubHardReset || gubSoftReset) {\
		gubDLMCStep = ERROR_DETECT;\
		break;\
	}\
}

#define M_SortForAddSysUnit(ubScanSysSetIndex, ubReplaceSysSetIndex, ubTempSystemSetFreeTable, ubSet){\
	ubReplaceSysSetIndex = (gubSystemSetFreeCount - 1);\
	/*找到符合順序的地方*/\
	for (ubScanSysSetIndex = 0; ubScanSysSetIndex < gubSystemSetFreeCount; ubScanSysSetIndex++) {\
		if((ubTempSystemSetFreeTable[ubScanSysSetIndex] != 0xFF)  && (ubSet < ubTempSystemSetFreeTable[ubScanSysSetIndex])){\
			ubReplaceSysSetIndex = ubScanSysSetIndex;\
			break;\
		}\
	}\
	/*往後放,騰出空間插進去*/\
	for (ubScanSysSetIndex = gubSystemSetFreeCount; ubScanSysSetIndex > ubReplaceSysSetIndex; ubScanSysSetIndex--) {\
		ubTempSystemSetFreeTable[ubScanSysSetIndex] = ubTempSystemSetFreeTable[ubScanSysSetIndex-1];\
	}\
	ubTempSystemSetFreeTable[ubReplaceSysSetIndex] = ubSet;\
}

#define M_SortForGetSysUnit(ubScanSysSetIndex, ubReplaceSysSetIndex, ubTempSystemSetFreeTable, ubSet){\
	for (ubScanSysSetIndex = 0; ubScanSysSetIndex < SYSTEM_SETS; ubScanSysSetIndex++) {\
		if(ubTempSystemSetFreeTable[ubScanSysSetIndex] == ubSet){\
			/*它的後面沒有了*/\
			if(ubTempSystemSetFreeTable[ubScanSysSetIndex+1] == 0xFF) {\
				ubTempSystemSetFreeTable[ubScanSysSetIndex] = 0xFF;\
			}else {\
				ubReplaceSysSetIndex = ubScanSysSetIndex;\
				do{\
					/*往前補*/\
					ubTempSystemSetFreeTable[ubReplaceSysSetIndex] = ubTempSystemSetFreeTable[ubReplaceSysSetIndex + 1];\
					ubReplaceSysSetIndex++;\
				}while(ubTempSystemSetFreeTable[ubReplaceSysSetIndex+1] != 0xFF);\
				ubTempSystemSetFreeTable[ubReplaceSysSetIndex] = 0xFF;\
			}\
			break;\
		}\
	}\
}

#define M_UserFreeTableEC(EC, FreeBlockTable, index) (EC[FreeBlockTable[index].B.uwTarget].B.ulEraseCount)
#define M_AllowGCtoD3(uwTarget) (((!gubRUTMixPlaneEnable) || (!gulEC_D3[uwTarget].B.btMixPlaneUnit)) && (!gubForceD1GC))
#if TSB_BICS4_SUPPORT
#define M_RefSeedPage(uwPage) ((guoFlashUseType.B.btToshibaBiCs4) ? (uwPage % 384) : uwPage)
#elif  (Hynix3DV6 || Hynix3DV5)
#define M_RefSeedPage(uwPage) ((guoFlashUseType.B.btHynix3DV6) ? (uwPage % 512) : (uwPage % 384))
#elif YMTC_JGS
#define M_RefSeedPage(uwPage) (uwPage % 384)
#endif

#define M_Refresh_CMDInStartTime_For_PwrMode(NoCheck, Cmd){\
	if (NoCheck || ((Cmd != 0x98) && (Cmd != 0xe5) && (Cmd != 0xec) && (Cmd != 0xef))) {\
		guoCMDInStartTime = GetRTT_milis();\
	}\
}

#define M_Refresh_PowerMode_And_SMART_Status(NoCheck, Cmd, Fea){\
	if (gubPowerMode != PWR_ACTIVE) {\
		if (NoCheck || ((Cmd != 0x98) && (Cmd != 0xe5) && (Cmd != 0xec) && (Cmd != 0xef))) {\
			if (NoCheck || !((Cmd == 0xB0) && (Fea == 0xD0))) {\
				if (gubOfflineMode & SMART_M_TIMERENABLE) {\
					if (gubOfflineMode & SMART_M_SUSPEND) {\
						gubOfflineMode &= (~SMART_M_SUSPEND);\
						gulSmartScanStartTime = (U32)(GetRTT_s() - gulSmartCurrentTime);\
					}\
				}\
				else if (AtaCfg.gSMARTSelective.ubOffLineDoing & SELECTIVE_OFFLINESUSPEND ) {\
					AtaCfg.gSMARTSelective.ubOffLineDoing &= (~SELECTIVE_OFFLINESUSPEND);\
				}\
			}\
			gubPowerMode = PWR_ACTIVE;\
		}\
	}\
}


#define M_PowerCycleRandTest(ubPowerCycleType)


#define M_CheckPartialSwapRUTDone(){\
	while (VT->gubSwapRUTMode == SWAPRUT_FORCEREADMOVE_MODE && gubSwapRUTState == BYTE_SWAPRUT_COPY) {\
		gubSwapRUTForceDone = 1;\
		ftlSwapRUT(SWAPRUT_FORCEREADMOVE_MODE, Default_Unit, DONTCARE);\
	}\
}
#define M_WaitFTLStop(){\
	HandleStopRW(0);\
	while ((VT->gulFTLState.B.btFlushCache == 0) && (VT->gulFTLState.B.btNeedCloseTarget || VT->gulFTLState.B.btNeedWearLeveling || VT->gulFTLState.B.btNeedCloseTableTarget || ((gubLockReadMode != LOCK_READ_MODE_NOBODY) && (gubLockReadMode != LOCK_READ_MODE_DLMC)))) {\
		M_SwitchTask();\
	}\
	HandleStopRW(1);\
}

/*=======================================================================================
Define
-----------------------------------------------------------------------------------------


=======================================================================================*/
#define    	BUF2_BYTE_BASE		((unsigned char *)BUFFER2_BASE)
#define    	BUFB_BASE		((unsigned char *)BUFFER3_BASE)
#define    	BUFW_BASE      ((unsigned short *)BUFFER3_BASE)
#define    	BUFL_BASE      ((unsigned long  *)BUFFER3_BASE)

//[cppcheck][Shifting signed 32-bit value by 31 bits is undefined behaviour(satacmd.h line 4534)(flainit.c line 2994)]
#define BIT0        0x01U
#define BIT1        0x02U
#define BIT2        0x04U
#define BIT3        0x08U
#define BIT4        0x10U
#define BIT5        0x20U
#define BIT6        0x40U
#define BIT7        0x80U
#define BYTE0	0xFF
#define BIT8        0x0100U
#define BIT9        0x0200U
#define BIT10      0x0400U
#define BIT11      0x0800U
#define BIT12      0x1000U
#define BIT13      0x2000U
#define BIT14      0x4000U
#define BIT15      0x8000U
#define BYTE1	0xFF00
#define BIT16      0x010000U
#define BIT17      0x020000U
#define BIT18      0x040000U
#define BIT19      0x080000U
#define BIT20      0x100000U
#define BIT21      0x200000U
#define BIT22      0x400000U
#define BIT23      0x800000U
#define BYTE2	0xFF0000
#define BIT24      0x01000000U
#define BIT25      0x02000000U
#define BIT26      0x04000000U
#define BIT27      0x08000000U
#define BIT28      0x10000000U
#define BIT29      0x20000000U
#define BIT30      0x40000000U
#define BIT31      0x80000000U
#define BIT32 (((ULLONG)0x01)<<32)
#define BIT33 (((ULLONG)0x01)<<33)
#define BIT34 (((ULLONG)0x01)<<34)
#define BIT35 (((ULLONG)0x01)<<35)
#define BIT36 (((ULLONG)0x01)<<36)
#define BIT37 (((ULLONG)0x01)<<37)
#define BIT38 (((ULLONG)0x01)<<38)
#define BIT39 (((ULLONG)0x01)<<39)
#define BIT40 (((ULLONG)0x01)<<40)
#define BIT41 (((ULLONG)0x01)<<41)
#define BIT42 (((ULLONG)0x01)<<42)
#define BIT43 (((ULLONG)0x01)<<43)
#define BIT44 (((ULLONG)0x01)<<44)
#define BIT45 (((ULLONG)0x01)<<45)
#define BIT46 (((ULLONG)0x01)<<46)
#define BIT47 (((ULLONG)0x01)<<47)
#define BIT48 (((ULLONG)0x01)<<48)
#define BIT49 (((ULLONG)0x01)<<49)
#define BIT50 (((ULLONG)0x01)<<50)
#define BIT51 (((ULLONG)0x01)<<51)
#define BIT52 (((ULLONG)0x01)<<52)
#define BIT53 (((ULLONG)0x01)<<53)
#define BIT54 (((ULLONG)0x01)<<54)
#define BIT55 (((ULLONG)0x01)<<55)
#define BIT56 (((ULLONG)0x01)<<56)
#define BIT57 (((ULLONG)0x01)<<57)
#define BIT58 (((ULLONG)0x01)<<58)
#define BIT59 (((ULLONG)0x01)<<59)
#define BIT60 (((ULLONG)0x01)<<60)
#define BIT61 (((ULLONG)0x01)<<61)
#define BIT62 (((ULLONG)0x01)<<62)
#define BIT63 (((ULLONG)0x01)<<63)
#define BYTE3	0xFF000000



#define SET_BIT0        0x01
#define SET_BIT1        0x02
#define SET_BIT2        0x04
#define SET_BIT3        0x08
#define SET_BIT4        0x10
#define SET_BIT5        0x20
#define SET_BIT6        0x40
#define SET_BIT7        0x80

#define SET_BIT8        0x0100
#define SET_BIT9        0x0200
#define SET_BIT10       0x0400
#define SET_BIT11       0x0800
#define SET_BIT12       0x1000
#define SET_BIT13       0x2000
#define SET_BIT14       0x4000
#define SET_BIT15       0x8000

#define SET_BIT16       0x010000
#define SET_BIT17       0x020000
#define SET_BIT18       0x040000
#define SET_BIT19       0x080000
#define SET_BIT20       0x100000
#define SET_BIT21       0x200000
#define SET_BIT22       0x400000
#define SET_BIT23       0x800000

#define SET_BIT24       0x01000000
#define SET_BIT25       0x02000000
#define SET_BIT26       0x04000000
#define SET_BIT27       0x08000000
#define SET_BIT28       0x10000000
#define SET_BIT29       0x20000000
#define SET_BIT30       0x40000000
#define SET_BIT31       0x80000000

#define BIT0x4        0x01010101
#define BIT1x4        0x02020202
#define BIT2x4        0x04040404
#define BIT3x4        0x08080808
#define BIT4x4        0x10101010
#define BIT5x4        0x20202020
#define BIT6x4        0x40404040
#define BIT7x4        0x80808080

#define CHK_BIT0        0x01
#define CHK_BIT1        0x02
#define CHK_BIT2        0x04
#define CHK_BIT3        0x08
#define CHK_BIT4        0x10
#define CHK_BIT5        0x20
#define CHK_BIT6        0x40
#define CHK_BIT7        0x80

#define 	CHK_BIT8        	0x0100
#define 	CHK_BIT9        	0x0200
#define 	CHK_BIT10       	0x0400
#define 	CHK_BIT11       	0x0800
#define 	CHK_BIT12       	0x1000
#define 	CHK_BIT13       	0x2000
#define 	CHK_BIT14       	0x4000
#define 	CHK_BIT15       	0x8000

#define CLR_BIT0        0xFFFFFFFE
#define CLR_BIT1        0xFFFFFFFD
#define CLR_BIT2        0xFFFFFFFB
#define CLR_BIT3        0xFFFFFFF7
#define CLR_BIT4        0xFFFFFFEF
#define CLR_BIT5        0xFFFFFFDF
#define CLR_BIT6        0xFFFFFFBF
#define CLR_BIT7        0xFFFFFF7F

#define CLR_BIT8        0xFFFFFEFF
#define CLR_BIT9        0xFFFFFDFF
#define CLR_BIT10       0xFFFFFBFF
#define CLR_BIT11       0xFFFFF7FF
#define CLR_BIT12       0xFFFFEFFF
#define CLR_BIT13       0xFFFFDFFF
#define CLR_BIT14       0xFFFFBFFF
#define CLR_BIT15       0xFFFF7FFF

#define CLR_BIT16       0xFFFEFFFF
#define CLR_BIT17       0xFFFDFFFF
#define CLR_BIT18	   0xFFFBFFFF
#define CLR_BIT19       0xFFF7FFFF
#define CLR_BIT20       0xFFEFFFFF
#define CLR_BIT21       0xFFDFFFFF
#define CLR_BIT22       0xFFBFFFFF
#define CLR_BIT23       0xFF7FFFFF

#define CLR_BIT24       0xFEFFFFFF
#define CLR_BIT25       0xFDFFFFFF
#define CLR_BIT26		0xFBFFFFFF
#define CLR_BIT27       0xF7FFFFFF
#define CLR_BIT28       0xEFFFFFFF
#define CLR_BIT29       0xDFFFFFFF
#define CLR_BIT30       0xBFFFFFFF
#define CLR_BIT31       0x7FFFFFFF

////////////     ATA use     //////////////////
//SetFeatures
#define EN8BITXFER     0x01    // Enable 8-bit data transfer
#define ENACTPM				 0x09    // Enable ACTPM
#define DISRDLAHD      0x55    // Disable Read Look Ahead
#define DISPOR         0x66    // Disable power on  reset establishment
#define NOPACP1        0x69    // NOP - Accepted
#define DIS8BITXFER    0x81    // Disable 8-bit data transfer
#define DISACTPM			 0x89		 // Disable ACTPM
#define NOPACP2        0x96    // NOP - Accepted
#define ACPBACK        0x97    // Accepted for backword compatibility.
#define HSTCRTCAP      0x9A    // Set host current source capability.
#define FOURBYTERWCMD    0xBB    // 4 bytes of data apply on read/write
#define ENPOR          0xCC    // Enable Power on Reset (POR) establishment
#define SetXferMode    0x03    // Set transfer mode based on value in
#define EnableSATAFeature    0x10    // Enable SATA Feature
#define DisableSATAFeature    0x90    // Enable SATA Feature
#define PIODefault     0x00    // PIO default transfer mode
#define MultiDMA       0x20    // Multiword DMA mode x ([2..0] for the
#define SingleDMA      0x10    // Obsolete in ATA-3
#define PIOMode        0x08    // PIO flow control transfer mode x
#define DisRevert      0x66    // Disable reverting to power on default
// until hard reset or power off).
#define EnRevert       0xCC    // Enable reverting to power on default
#define Reserved       0x18

#define Ena_Use_SATA_Feature       0x10  //  Enable Use of SATA Feature
#define Dis_Use_SATA_Feature       0x90  // Disable Use of SATA Feature
#define APM_EN         0x05    // Set Advance Power Managment
#define APM_DIS        0x85    // DIS advance power mangement
#define DDT_EN         0x5F
#define DDT_DIS        0xDF

#define ENWRCACHE      0x02    // Enable Write Cache
#define DISWRCACHE     0x82    // Disable Write Cache
#define ENRDLAHD       0xAA    // Enable Read Look Ahead


// Security
#define HYNIX_RETRY_TABLE_0808  0
#define HYNIX_RETRY_TABLE_0C0A  1
#define HYNIX_RETRY_TABLE_0804  2

#define BIT_RENEW_VT 			BIT0
#define BIT_RENEW_VTCHILD		BIT1
#define BIT_SKIP_VTPTR_CHECK 	BIT2

#define	    CHK_SEC_MPW     CHK_BIT0	//
#define	    CHK_SEC_CAP_MAX     CHK_BIT8	//

#define	    SET_SEC_SUPPORT      CHK_BIT0
#define	    SET_SEC_EN      CHK_BIT1
#define	    SET_SEC_LOCK    CHK_BIT2
#define	    SET_SEC_FROZEN      CHK_BIT3
#define	    SET_SEC_PWEXCE      SET_BIT4     // Password Attempt Counter Exceeded
#define            SET_SEC_ENHANCE_ERASE SET_BIT5
#define	    SET_SEC_MAX     SET_BIT7    // Master Password Capability

#define USER_PW_CORRECT 1
#define MASTER_PW_CORRECT 2
#define ENHANCED_SECURITY_TIME 30
#define NORMAL_SECURITY_TIME 10

#define SECURITY_ERASE_NORMAL_MODE  BIT0
#define SECURITY_ERASE_ENHANCE_MODE BIT5

// HPA
#define	    SET_HPANVLBA    SET_BIT3 //non volatile size valid	,  issue set max addr( non-volatile,max) shall ckear. if valid,comreset/power cycle 則使用hpasize,反之native...
#define	    SET_HPALOCK     SET_BIT4 //HPA Lock
#define	    SET_HPASET      SET_BIT5 //HPA Set
#define	    SET_HPAEXT      SET_BIT6 //HPA Ext
#define	    SET_HPAFNV      SET_BIT7 //first set max address (non volatile)
#define       SET_HPAALL      (SET_HPANVLBA|SET_HPALOCK|SET_HPASET|SET_HPAEXT|SET_HPAFNV)



// DCO
#define     SET_DCO_MDMA    SET_BIT0
#define     SET_DCO_UDMA    SET_BIT1
#define     SET_DCO_SIZE    SET_BIT2
#define     SET_DCO_HPA     SET_BIT3
#define     SET_DCO_SEC     SET_BIT4
#define     SET_DCO_LOG     SET_BIT5
#define     SET_DCO_SELFTEST    SET_BIT6
#define     SET_DCO_SMART   SET_BIT7
#define     SET_DCO_IPM     SET_BIT8
#define     SET_DCO_TRIM    SET_BIT9
#define     SET_DCO_SSP    SET_BIT10

#define	    SET_DCO_LOCK    SET_BIT14 //DCO Lock
#define	    SET_DCO_SET     SET_BIT15 //DCO reduced config

//AMAX
#define	    SET_AMAX_SET   SET_BIT0
#define	    SET_AMAX_FREEZE  SET_BIT1
#define 	    SET_AMAX_FOREVER  SET_BIT2

//SANITIZE
#define	SD0_SANITIZE_IDLE  0
#define	SD1_SANITIZE_FROZEN  1
#define	SD2_SANITIZE_IN_PROGRESS   2
#define	SD3_SANITIZE_OPERATION_FAIL 3
#define	SD4_SANITIZE_OPERATION_SUCCESS 4

#define	SET_SANITIZE_OPERATION_COMPLETED_WITHOUT_ERROR_VALUE SET_BIT0
#define	SET_SANITIZE_FAILURE_MODE_POLICY_VALUE SET_BIT1
#define	SET_SANITIZE_ANTIFREEZE_VALUE SET_BIT2
#define	SET_RESTRICTED_SANITIZE_OVERRIDES_SECURITY SET_BIT3

#define	SANITIZE_ANTIFREEZE_SUPPORT 1
#define	SANITIZE_BLOCK_ERASE_SUPPORT 0
#define	SANITIZE_CRYPTO_SCRAMBLE_SUPPORT 0
#define	SANITIZE_OVERWRITE_SUPPORT 1
#define	RESTRICTED_SANITIZE_OVERRIDES_SECURITY_ENABLE 0

#define	ENABLE_FAST_SANITIZE_TEST 0
#define	FAST_SANITIZE_SIZE 0x4008
// DLMC
#define 	ENABLE_ISPJUMP_AFTER_DLMC_PREFORMAT 1

//Bin Header index Define
#define DLMC_FW_Version_2 6
#define DLMC_FW_Version_4 28
#define DLMC_With_Infoblock 36
#define DLMC_Infoblock_Checksum_High_Byte 37
#define DLMC_Infoblock_Checksum_Low_Byte 38
#define DLMC_InfoblockWithDummy_Checksum_High_Byte 39
#define DLMC_InfoblockWithDummy_Checksum_Low_Byte 40


#define BINFILE_HEADER_SECTOR 1  //header 512B
#define MICROCODE_HEADER_SECTOR 32  //header 16KB
#define BINFILE_SYSINFO_SECTOR 32  //16KB
#define BINFILE_INFOBLOCK_SECTOR 2  //infoblock + dummy 1KB

#define HEADER_TRANSFERDONE 1
#define BURNER_TRANSFERDONE 2
#define MICROCODE_16K_HEADER_TRANSFERDONE 3
#define MICROCODE_TRANSFERDONE 4
#define INFOBLOCK_TRANSFERDONE 5
#define SYSINFO_TRANSFERDONE 6
#define ALL_TRANSFERDONE 7
#define SAVE_MICROCODE_DONE 8
#define SAVE_INFOBLOCK_DONE 9
#define GOING_TO_ACTIVATE_MICROCODE 10
#define ERROR_DETECT 0xFF

#define BINHEADER_WRITE_ADDR 0
#define BINHEADER_READ_ADDR 1
#define BINHEADER_SECTION_SIZE 2

#define DLMC_MINSECTORCNT	(0x0001)
#define DLMC_MAXSECTORCNT	(0xFFFE)
#define DLMC_Feature0x03_Support	(1)
#define DLMC_Feature0x07_Support	(1)
#define DLMC_Feature0x0E0x0F_Support (1)
#define MAX_CODEVERSION_NUM 16

// Smart
#define	ENABLE_SMART_SELFMERGE  1
#define	ENABLE_SMART_SELFMERGE_DUMMY  1

#define	ENABLE_SMART_OFFLINE_READ	1
#define	ENABLE_SMART_SELFTEST_SHORT_EXTEND_SELFTEST	1
#define	ENABLE_SMART_SELFTEST_CONVEYANCESELFTEST	1
#define	ENABLE_SMART_SELFTEST_SELECTIVESELFTEST	1


#define	ENABLE_SMART_SELFTEST_EXECUTE_OFFLINE_IMMEDIATE 1

#define	SMART_SELFTEST_REALSCANMODE  1
#define 	UPDATE_TIME_INTERVAL_MIN 60
#define	DEVSLP_EXIT_TIMEOUT 100

// Host Table total 1024 sector (Table TOTAL_HOST_TABLE_NUM )
#define     HOSTTABLE_SMARTLOG2_ADR      0   //51
#define     HOSTTABLE_SMARTLOG3_ADR      51  //64
#define     HOSTTABLE_SMARTLOG6_ADR      115 //01
#define     HOSTTABLE_SMARTLOG7_ADR      116 //01
#define     HOSTTABLE_SMARTLOGB_ADR      117 //   B ?? 這三小... 0Ah-0Ch :Reserved
#define     HOSTTABLE_SMARTLOG80_ADR     118 // 512, 0x80-0x9F, each contain sixteen log pages
#define     HOSTTABLE_ATACFG_ADR    630 // 1
#define     HOSTTABLE_SECURITYID2_ADR  631 // 1
#define     HOSTTABLE_SMARTLOG9_ADR  632 // HC-20131115

// 改其他數值,read/write log function 要重寫, smart table也要從分配...
#define     HOSTTABLE_SMARTLOG0_PAGESIZE 1
#define     HOSTTABLE_SMARTLOG1_PAGESIZE 1
#define     HOSTTABLE_SMARTLOG2_PAGESIZE 51  // 在增加ubSmart02hIndex會爆...
#define     HOSTTABLE_SMARTLOG3_PAGESIZE 64
#define     HOSTTABLE_SMARTLOG4_PAGESIZE 8
#define     HOSTTABLE_SMARTLOG6_PAGESIZE 1
#define     HOSTTABLE_SMARTLOG7_PAGESIZE 1
#define     HOSTTABLE_SMARTLOG9_PAGESIZE 1// HC-20131115
#define     HOSTTABLE_SMARTLOG10_PAGESIZE 1
#define     HOSTTABLE_SMARTLOG11_PAGESIZE 1
#define     HOSTTABLE_SMARTLOG30_PAGESIZE 9
#define     HOSTTABLE_SMARTLOG80to9F_PAGESIZE 16
#define     HOSTTABLE_SMARTLOG31_PAGESIZE 4

#define     SMART_TIME_OFFLINE_SECOND (TLC_TSB)?(0x1E):(0xFFFF)
#define     SMART_TIME_SHORTSELFTEST_MINUTE 2// Max waitting time (minute)
#define     SMART_TIME_EXTENDEDSELFTEST_MINUTE 30    // Max waitting time (minute)
#define     SMART_TIME_CONVEYANCESELFTEST_MINUTE 6    // Max waitting time (minute)
#define     SMART_TIME_BUFFER_TIME_SECOND 2 //num of buffer time after done to return to host.(ex. return 30sec to host, self test will test only (30-x)second)

//  M: for gubOfflineMode use.
#define     SMART_M_OFFLINEROUTINE BIT0
#define     SMART_M_SHORTSELFTEST BIT1
#define     SMART_M_EXTENDEDSELFTEST BIT2
#define     SMART_M_CONVEYANCESELFTEST BIT3
#define     SMART_M_SELECTIVESELFTEST BIT4
#define     SMART_M_SUSPEND BIT5
#define     SMART_M_SAVEATACONFIG BIT6
#define     SMART_M_TIMERENABLE BIT7

#define SMART_SCAN_STATUS_PROGRESS 0
#define SMART_SCAN_STATUS_DONE 1
#define SMART_SCAN_STATUS_FAIL 2
#define SMART_SCAN_STATUS_DEVSLP 3

#define     SELECTIVE_OFFLINEDOING BIT0
#define     SELECTIVE_OFFLINESUSPEND BIT1

#define     SMART_STATUS_OFFLINE_COMPLETED  0x02
#define     SMART_STATUS_OFFLINE_PROGRESS  0x03
#define     SMART_STATUS_OFFLINE_SUSPEND  0x04
#define     SMART_STATUS_OFFLINE_ABORTED 0x05
#define     SMART_STATUS_OFFLINE_ABORTEDBYUNC 0x06
#define     SMART_STATUS_SELFTEST_COMPLETED 0x00
#define     SMART_STATUS_SELFTEST_ABORTEDBYHOST 0x10
#define     SMART_STATUS_SELFTEST_ABORTEDBYRESET 0x20
#define     SMART_STATUS_SELFTEST_ABORTEDBYUNC 0x70
#define     SMART_STATUS_SELFTEST_PROGRESS 0xF0

#define     SMART_FeatureFlag_PerformOfflineScan	BIT1
#define     SMART_FeatureFlag_OfflineScanPending	BIT3
#define     SMART_FeatureFlag_OfflineScanActive	BIT4

// Command Timeout Threshold
#define NCQ_TIMEOUT_THRESHOLD	8000 //ms
#define ATA_TIMEOUT_THRESHOLD	8000 //ms

///////////////////////////////////////////
// Flash Clock Setting
#define FLH_CLK_DEFAULT 0xFF

#define FLH_CLK_150MHz 10 //toggle-300
#define FLH_CLK_180MHz 9 //toggle-360
#define FLH_CLK_266MHz 8 //toggle-533
#define FLH_CLK_200MHz 7 //toggle-400
#define FLH_CLK_166MHz 6 //toggle-333
#define FLH_CLK_100MHz 5 //toggle-200
#define FLH_CLK_83MHz 4 //toggle-166
#define FLH_CLK_50MHz 3
#define FLH_CLK_40MHz 2
#define FLH_CLK_10MHz 1


//Flash Mode Setting
#define FLH_MODE_DEFAULT 0xFF
#define FLH_MODE_IGNORE  7
#define FLH_MODE_ONFI_NVDDR2_WITHOUT_TOGGLE2SIGNAL  6
#define FLH_MODE_ONFI_NVDDR2 5
#define FLH_MODE_ONFI_NVDDR 4
#define FLH_MODE_TOGGLE2 3
#define FLH_MODE_TOGGLE1 2
#define FLH_MODE_LEGACY 1

//Flash Densitysize
#define FLH_Size_8Gb 0
#define FLH_Size_16Gb 1
#define FLH_Size_32Gb 2
#define FLH_Size_64Gb 3
#define FLH_Size_128Gb 4
#define FLH_Size_256Gb 5
#define FLH_Size_512Gb 6
#define FLH_Size_1Tb 7
#define FLH_Size_384Gb 8
#define FLH_Size_2Tb 9


#define INTERFACE_LEGACY	0
#define INTERFACE_TOGGLE1	1
#define INTERFACE_TOGGLE2	2
#define INTERFACE_NVDDR	3
#define INTERFACE_NVDDR2	4
#define INTERFACE_NVDDR2_WITHOUTTOGGLE2	5
#define SET_FLH_ODT  6
#define INTERFACE_TOGGLE2To1 7

#define ENABLE_ONFI_NVDDR2  	1
#define ENABLE_ONFI_200M_NVDDR2  	1
#define ENABLE_MICRON_TOGGLE2_SIGNAL 	1
#define ENABLE_FORCETOGGLE1     1
#define ENABLE_FORCETOGGLE1TO166MB      1
#define ENABLE_FLH_OVERDRIVE 	1  // 0: defalut
#define ENABLE_NVDDR2_DIFFERENTIAL	1

#define FLH_ODT_VALUE 0
#if MicronFlashOnly
#define FLH_DRIVE_VALUE 2
#else
#define FLH_DRIVE_VALUE 6
#endif

#define SCAN_LDPC_THRESHOLD 3

#define CUSTOMER_GPIO_MODE_DISABLE 0
#define CUSTOMER_GPIO_MODE_WRITEPROTECT_ABORT 1
#define CUSTOMER_GPIO_MODE_WRITEPROTECT_DUMMY 2
#define CUSTOMER_GPIO_MODE_ERASE_USER 3
#define CUSTOMER_GPIO_MODE_ERASE_ALL 4

#define CUSTOMER_GPIO3_MODE_DISABLE 0
#define CUSTOMER_GPIO3_MODE_QUICKERASE_USER 1
#define CUSTOMER_GPIO3_MODE_QUICKERASE_ALL 2
#define CUSTOMER_GPIO3_MODE_QUICKERASE_N92 3



#define GPIO_EDGE_MODE_RISING_ERASE 1 //rising erase
#define GPIO_EDGE_MODE_FALLING_ERASE 2 //falling erase

//Force Flash Setting
#define ENABLE_FORCE_CE_NUM    0
#define FORCE_CE_NUMBER	8

#define ENABLE_FORCE_SET_FLH_CLK 		0 // 1:force set clk   0:MP set clk
#define ENABLE_FORCE_SET_FLH_MODE	0 // 1:force set mode   0:MP set mode
#define FORCE_FLH_CLK 		FLH_CLK_100MHz
#define FORCE_FLH_MODE 	FLH_MODE_TOGGLE2

#define INTERLEAVE_8 	4
#define INTERLEAVE_4 	3
#define INTERLEAVE_2 	2
#define NO_INTERLEAVE 	1
#define INTERLEAVE_DEFAULT INTERLEAVE_8


/* AC timing */
//FPU - TOGGLE
#define TIME_ADL_TGL			0	//page program
#define TIME_AR_TGL 			1	//read ID
#define TIME_CH_TGL			2	//cmd latch, adr latch
#define TIME_CLR_TGL			3
#define TIME_CWAW_TGL		4
#define TIME_RR_TGL			5	//get feature, page read, page random read
#define TIME_WHR_TGL			6	//read ID, status read
#define TIME_WHR2_TGL		7	//page random read
#define TIME_WW_TGL			8
#define TIME_FPU_RESERVED_0	9
#define TIME_FPU_RESERVED_1	10

//REGISTER - TOGGLE
#define TIME_TGL_3_0	0
#define TIME_TGL_6_4	1
#define TIME_TGL_11_8	2
#define TIME_TGL_14_12	3
#define TIME_TGL_19_16	4
#define TIME_TGL_23_20	5
#define TIME_TGL_27_24	6
#define TIME_ONFI_26_24	7
#define TIME_ONFI_30_28	8
#define TIME_RTY_16	9
#define TIME_RTY_18_17	10
#define TIME_RTY_20_19	11
#define TIME_RTY_21	12
#define TIME_RTY_22	13
#define TIME_REG_RESERVED_0	14
#define TIME_REG_RESERVED_1	15

//MTP.ce_delay - TOGGLE
#define TIME_CR_TGL 0	//data output, page read, page random read
#define TIME_CS_TGL		1	//cmd latch, adr latch, data input, read ID, status read
#define TIME_CS2_0_TGL		2
#define TIME_MTP_RESERVED_0	3
#define TIME_MTP_RESERVED_1	4

//MTP.ce_delay - LEGACY
#define TIME_CS_LGC			0

//time number
#define TIME_NUM_FPU_TOGGLE 11
#define TIME_NUM_REGISTER_TOGGLE 16
#define TIME_NUM_MTP_CE_DELAY_TOGGLE 	5

#define ENABLE_AC_TIMING_SETTING 	1

//==============================================================================
// Structure Define
//==============================================================================
typedef union {
	U32 ulAll;
	struct {
		U32 btFPUTrig		:       1;     //0x00 1 Bit bit 0
		U32 btPollFailIntEn	:       1;     //     1 bit        bit 1
		U32 btForceReadFail	:       1;     //     1 Bit       bit 2
		U32 btForceWriteFail	:       1;     //     1 Bit       bit 3
		U32 btSetInternalBusy	:	1;	//	1 Bit	bit 4
		U32 btQueryCheck         	:	1;	//	1 Bit 	bit 5
		U32 btStaNoStop		:	1;	//	1 Bit  	bit 6
		U32 btEraseSuspendEn 	:       1;     //     1 Bit bit 7
		U32 btBCEn           		:	1;	//	1 Bit	bit 8
		U32 ubBCGroup		:       7;     //     7 Bit        bit 9-15
		U32 uwRsv		:       16;    //     16 Bit       bit 16-31
	} BitMap;
} ParaOffset0x00;

typedef union {
	U32 ulAll;
	struct {
		U32 uwConvLogicPage		:	9;	//0x04	9 Bit 	bit 0-8
		U32 btConvBypass		:	1;	//	1 Bit	bit 9
		U32 btConvLogicPageEn	:	1;	//	1 Bit 	bit 10
		U32 ubALUSel            :       2;     //     2 Bit        bit 11-12
		U32 btUltraDMADisable   :       1;     //     1 Bit        bit 13
		U32 btIntVctEn		:       1;     //     1 bit bit 14
		U32 btiFSAEn 		:       1;     //     1 bit bit 15
		U32 ubRSPageNum		:	8;	//	8 Bit	bit 16-23
		U32 btRSEncodeMode		:	1;	//	1 Bit 	bit 24
		U32 btRSLastPage		:	1;	//	1 Bit 	bit 25
		U32 btRSProg2ndParity 	:       1;     //     1 Bit bit 26
		U32 btRSDecreasePECCnt	:       1;     //     1 Bit        bit 27
		U32 btRSOneParityEn    	:       1;     //     1 Bit        bit 28
		U32 ubRsv0  		:       2;     //     2 Bit bit 29-30
		U32 btWriteRegEn		:	1;	//	1 Bit	bit 31
	} BitMap;

} ParaOffset0x04;

typedef union {
	U32 ulAll;
	struct {
		U32 ubNormalTargetCPUVct	:	3;	//0x08	3 bit 	bit 0-2
		U32 ubErrorTargetCPUVct	:	3;	//	3 bit	bit 3-5
		U32 ubRsv				:	2;	//	2 bit	bit 6-7
		U32 ulIntVct      	:       24;   //     8 bit        bit 8-31
	} BitMap;

} ParaOffset0x08;

typedef union {
	U32 ulAll;
	struct {
		U32 ubFSASel 		:       7;     //0x10     7 Bit        bit 0-6
		U32 ubFSA2Sel		:	7;	//	7 bit 	bit 7-13
		U32 ubFSA2Cnt		:	2;	// 	2 bit	bit 14-15
		U32 ubFW3			:	8;	//	8 bit	bit 16-23
		U32 ubRsv       	:       8;     //     8 bit        bit 24-31
	} BitMap;

} ParaOffset0x10;

typedef union {
	U32 ulAll;
	struct {
		U32 uwFPUPtr		:	16;	//0x1C	16 bit 	bit 0-15
		U32 ubCEValue		:	8;	//	8 bit	bit 16-23
		U32 ubCEDelay 		:       4;     //       4 Bit    bit 24-27
		U32 ubPolSeqSel 	:       3;     //       3 Bit    bit 28-30
		U32 btRsv  		:       1;     //       1 bit     bit 31
	} BitMap;
} ParaOffset0x1C;

typedef union {
	U32 ulAll;
	struct {
		U32 uw2ndFPUPtr 	:       16;   //0x20     16 bit      bit 0-15
		U32 ubFSASel2           : 	7; //       7 bit    bit 16-22
		U32 btRsv0  		:   	1;        //      1 bit     bit 23
		U32 btSetInternalBusy2		:	1;	//	1 bit 	bit 24
		U32 btbtIntVctEn2    	:  	1; //       1 bit    bit 25
		U32 ubRsv1   		:  	6; //       6 bit     bit 26-31
	} BitMap;
} ParaOffset0x20;

typedef union {
	U32 ulAll;
	struct {
		U32 uwL4KSparePtr		:	16;	//0x24	16 Bit 	bit 0-15
		U32 ubL4KNum			:	6;	//	6 Bit 	bit 16-21
		U32 ubRsv0    		: 	2; //     2 Bit      bit 22-23
		U32 ubFrameNum		:	3;	//	3 Bit	bit 24-26
		U32 btZipBypass			:	1;	//	1 Bit	bit 27
		U32 btCmpEn    			:	1;	//	1 Bit	bit 28
		U32 btL4kBADRContModeEn     :  	1; //     1 Bit       bit 29
		U32 btForceFixData		:	1;	//	1 Bit	bit 30
		U32 btBufModeEn		:	1;	//	1 Bit	bit 31
	} BitMap;
} ParaOffset0x24;

typedef union {
	U32 ulAll;
	struct {
		U32 btCESelMode  	:       1;     //0x28     1 Bit bit 0
		U32 btMTGroupEn		:	1;	//	1 Bit	bit 1
		U32 ubMTGroup			:	6;	//	6 Bit 	bit 2-7
		U32 btIOPadType			:	1;	//	1 Bit 	bit 8
		U32 btFClkDivEn			:	1;	//	1 Bit 	bit 9
		U32 ubFClkDiv			:	6;	//	6 Bit 	bit 10-15
		U32 ubFLHType			:	2;	//	2 Bit 	bit 16-17
		U32 btTimCfgSel			:	1;	//	1 Bit 	bit 18
		U32 btPolSeqUpdate    		:	1;	//	1 Bit	bit 19
		U32 btCEUpdate			:	1;	//	1 Bit	bit 20
		U32 ubRsv          	:       3;     //     3 Bit        bit 21-23
		U32 ubFTA1stByte     	:       8;     //     8 Bit        bit 24-31  //FTA_COL[7:0]
	} BitMap;
} ParaOffset0x28;

typedef union {
	U32 ulAll;
	U32 uliFSA0;
	struct {
		U32 ubFTA2ndByte 	:        8; //0x2C       8 bit     bit 0-7
		U32 uwFTARow 		:       24;   //     24 bit      bit 8-31
	} BitMap;

} ParaOffset0x2C;

typedef struct {
	ParaOffset0x00 Para0x00;			//0x00	4 Bytes
	ParaOffset0x04 Para0x04;			//0x04	4 Bytes
	ParaOffset0x08 Para0x08;			//0x08	4 Bytes
	U32 ulL4KBADR;                                //0x0C     4 Bytes
	ParaOffset0x10 Para0x10;			//0x10	4 Bytes
	U32 ulSeedInit;				//0x14	4 Bytes
	U32 ulZipMap;				//0x18	4 Bytes
	ParaOffset0x1C Para0x1C;			//0x1C   4 Bytes
	ParaOffset0x20 Para0x20;			//0x20   4 Bytes
	ParaOffset0x24 Para0x24;			//0x24	4 Bytes
	ParaOffset0x28 Para0x28;			//0x28	4 Bytes
	ParaOffset0x2C Para0x2C;                     //0x2C     4 Bytes
	U32 uliFSA1;					//0x30	4 Bytes
	U32 uliFSA2;				//0x34	4 Bytes
	U32 uliFSA3;                              //0x38     4 Bytes
	U32 uliFSA0_2;			//0x3C	4 Bytes
#if (TEST_MT_TABLE_72B)
	U32 ulRsv0x40;
	U32 ulRsv0x44;
#endif
} MTEvent64B;


typedef struct {
	MTEvent64B Depth[G_MT_QUEUE_DEPTH];
} MTQ;

typedef union {
	ULLONG uoAll;
	struct {
		U32 ubChannel		: 3;
		U32 ubQueue		: 3;
		U32 ubDepth		: 4;
		U32 ubErrorType		: 3; //0: No Error; 1: UECC; 2: Signoff Mask Occur; 3: Error Status; 4: CRC Fail; 5: Over ECC Occur; 6: Timeout
		U32 uwVector		: 19;
		U32 ulExtVector		: 32;
	} BitMap;
} MTHangInfo;

//==============================================================================
//Self Reg Mode
//==============================================================================
typedef union {
	U32 ulAll;
	struct {
		U32 uwwReg_a_addr		:	10;	//		10 Bits	bit 0-9
		U32 ubRsv0			:	2;	//		2 Bits	bit 10-11
		U32 ubwReg_a_wen		:	4;	//		4 Bits	bit 12-15
		U32 uwwReg_b_addr		:	10;	//		10 Bits	bit 16-25
		U32 ubRsv1			:	2;	//		2 Bits	bit 26-27
		U32 ubwReg_b_wen		:	4;	//		4 Bits	bit 28-31
	} BitMap;
} ParaSelfRegOffset0x00;

typedef union {
	U32 ulAll;
	struct {
		U32 uwRsv0			:	16;	//		16 Bits	bit 0-15
		U32 uwRsv1			:	15;	//		15 Bits	bit 16-30
		U32 btWrite_reg_mode		:	1;	//		1 Bit		bit 31
	} BitMap;
} ParaSelfRegOffset0x04;

typedef union {
	U32 ulAll;
	struct {
		U32 uwwReg_c_addr		:	10;	//		10 Bits	bit 0-9
		U32 ubRsv0			:	2;	//		2 Bits	bit 10-11
		U32 ubwReg_c_wen		:	4;	//		4 Bits	bit 12-15
		U32 uwwReg_d_addr		:	10;	//		10 Bits	bit 16-25
		U32 ubRsv1			:	2;	//		2 Bits	bit 26-27
		U32 ubwReg_d_wen		:	4;	//		4 Bits	bit 28-31
	} BitMap;

} ParaSelfRegOffset0x10;

typedef struct {
	ParaSelfRegOffset0x00 SelfPara0x00;		//0x00	4 Bytes

	ParaSelfRegOffset0x04 SelfPara0x04;		//0x04	4 Bytes

	U32 ulWREGA_VALUE;				//0x08	4 Bytes

	U32 ulWREGB_VALUE;				//0x0C	4 Bytes

	ParaSelfRegOffset0x10 SelfPara0x10;		//0x10	4 Bytes

	U32 ulWREGC_VALUE;				//0x18	4 Bytes

	U32 ulWREGD_VALUE;				//0x1C	4 Bytes

	U32 ulRsv0;					//0x20	4 Bytes

	U32 ulRsv1;					//0x24	4 Bytes

	U32 ulRsv2;					//0x28	4 Bytes

	U32 ulRsv3;					//0x2C	4 Bytes

	U32 ulRsv4;					//0x30	4 Bytes

	U32 ulRsv5;					//0x34	4 Bytes

	U32 ulRsv6;					//0x38	4 Bytes

	U32 ulRsv7;					//0x3C	4 Bytes
} MTSelfReg64B;

typedef union {
	U32 ulAll;
	struct {
		U32 ulL4K_FW		:	24;
		U32 ubL4K_SPRV		:	8;
	} BitMap;
} L4KTableOffset0x04;

typedef union {
	U32 ulAll;
	struct {
		//U32 ubL4K_ZCODE		:	8;
		U32 ubL4K_BUF_MASK	:	8;
		U32 uwNEXT_L4K_PTR	:	16;
		U32 ubL4K_FW2		:	8;
	} BitMap;
} L4KTableOffset0x08;

typedef union {
	U32 ulAll;
	struct {
		U32 ulL4K_BADR		:	24;
		U32 ubL4K_ZCODE		:	8;
		//U32 ubL4K_BUF_MASK	:	8;
	} BitMap;
} L4KTableOffset0x0C;

typedef struct {
	U32 ulL4K_LCA;				//0x00	4 Bytes

	L4KTableOffset0x04 Para0x04;			//0x04	4 Bytes

	L4KTableOffset0x08 Para0x08;			//0x08	4 Bytes

	L4KTableOffset0x0C Para0x0C;			//0x0C	4 Bytes
} L4KTable16B;

typedef union {
	U32 ulAll;
	struct {
		U32 ubFW3				:	8;
		U32 uwP4k_Head: 16;
		U32 ubFW2	: 8;
	} BitMap;
} P4KTableOffset0x08;

typedef struct {
	U32 ulL4K_LCA;				//0x00	4 Bytes

	L4KTableOffset0x04 Para0x04;			//0x04	4 Bytes

	P4KTableOffset0x08 Para0x08;			//0x08	4 Bytes

	U32 ulCRC32;				//0x0C	4 Bytes
} P4KTable16B;

typedef union {
	U32 L;
	struct {
		UWORD W0: 16;
		UWORD W1: 16;
	} W;
	struct {
		U32 B0: 8;
		U32 B1: 8;
		U32 B2: 8;
		U32 B3: 8;
	} B;
	struct {
		U32 BT0: 1;
		U32 BT1: 1;
		U32 BT2: 1;
		U32 BT3: 1;
		U32 BT4: 1;
		U32 BT5: 1;
		U32 BT6: 1;
		U32 BT7: 1;
		U32 BT8: 1;
		U32 BT9: 1;
		U32 BT10: 1;
		U32 BT11: 1;
		U32 BT12: 1;
		U32 BT13: 1;
		U32 BT14: 1;
		U32 BT15: 1;
		U32 BT16: 1;
		U32 BT17: 1;
		U32 BT18: 1;
		U32 BT19: 1;
		U32 BT20: 1;
		U32 BT21: 1;
		U32 BT22: 1;
		U32 BT23: 1;
		U32 BT24: 1;
		U32 BT25: 1;
		U32 BT26: 1;
		U32 BT27: 1;
		U32 BT28: 1;
		U32 BT29: 1;
		U32 BT30: 1;
		U32 BT31: 1;
	} BT;
	struct {
		UBYTE B[4];
	} S;

} REG_t;

typedef union {
	ULLONG uqword;
	struct {
		U32 L0 : 32;
		U32 L1 : 32;
	} ulong;
	struct {
		U32 W0 : 16;
		U32 W1 : 16;
		U32 W2 : 16;
		U32 W3 : 16;
	} uword;
	struct {
		U32 B0 : 8;
		U32 B1 : 8;
		U32 B2 : 8;
		U32 B3 : 8;
		U32 B4 : 8;
		U32 B5 : 8;
		U32 B6 : 8;
		U32 B7 : 8;
	} B;
} FSA_t;
#if ENABLE_READ_CHECK_FUNCTION
#if TLC
#define DefaultVC 0x3fffff
typedef union {
	U32 All;
	struct {
		U32 ulValidCount: 22;//vb : 4194303 sector , 2GB
		U32 ulReadCount: 8;//for D3 Read Cnt
		U32 btInD1Unit: 1;
		U32 btFastPage: 1;
	} B;
} ValidCount_t;
#else
#define DefaultVC 0x7FFFFF
typedef union {
	U32 All;
	struct {
		U32 ulValidCount: 23;
		U32 ulReadCount: 8;//for D3 Read Cnt
		U32 btFastPage: 1;
	} B;
} ValidCount_t;
#endif
#else
#if TLC
#define DefaultVC 0x3fffffff
typedef union {
	U32 All;
	struct {
		U32 ulValidCount: 30;
		U32 btInD1Unit: 1;
		U32 btFastPage: 1;
	} B;
} ValidCount_t;
#else
#define DefaultVC 0x7fffffff
typedef union {
	U32 All;
	struct {
		U32 ulValidCount: 31;
		U32 btFastPage: 1;
	} B;
} ValidCount_t;
#endif
#endif

#define DefaultEC 0x3fffffff
#define BIT_MixPlaneUnit BIT30
#define BIT_UsedDataUnit BIT31
typedef union {
	U32 All;
	struct {
		U32 ulEraseCount: 30;
		U32 btMixPlaneUnit: 1;
		U32 btUsedDataUnit: 1;
	} B;
} EraseCount_t;

typedef union {
	ULLONG All;
	struct {
		ULLONG ulVir4kIndex: 28;
		ULLONG ubZByteNum: 3;
		ULLONG btDummy: 1;
		ULLONG ulFEntry: 28;
		ULLONG ubZByteOffset: 3;
		ULLONG btValid: 1;
	} B;
} GCTable_t ;

typedef union {
	U32 All;
	struct {
		U32 ulVir4kIndex: 27;        // 512GB剛好
		U32 ubZByteNum: 3;
		U32 btSpecialData: 1;        // when this bit set, data is zero
		U32 btNeedCleanGRUpdateL2P: 1;         // 1: already update L2P in XdataIn. Don't need to update in clean GR
	} L2P_FORMAT_MODE0;
	struct {
		U32 ulVir4kIndex: 28;        // 1T剛好
		U32 ubZByteNum: 2;
		U32 btSpecialData: 1;        // when this bit set, data is zero
		U32 btNeedCleanGRUpdateL2P: 1;         // 1: already update L2P in XdataIn. Don't need to update in clean GR
	} L2P_FORMAT_MODE1;
} P2LTable_t ;

typedef union {
	U32 All;
	struct {
		U32 ulFEntry: 26; // 256G 剛好
		U32 ubZByteOffset: 3;
		U32 ubZByteNum: 3;
	} B;
} L2PTable_t ;

typedef union {
	ULLONG All;
	struct {
		L2PTable_t PhyEntry;
		UWORD uwNextUnit;
		UWORD uwNewUnitStartOffset;
	} B;
} Seq_Table_t ;

typedef union {
	U32 All;
	struct {
		L2PTable_t PhyEntry;
	} B;
} Seq_Table_1T_t ;

typedef union {
	U32 All;
	struct {
		U32 ulL2PGroupIndex: 16; // 1T/16M = 64k L2PGroup(16K Group)
		U32 btDirty: 1;
		U32 btValid: 1;
		U32 btReference: 1;
		U32 btWaiting_Update: 1;
		U32 btTableInRAMFlag: 1; // 1:load done 0:can load
		U32 btSaving: 1;
		U32 ubNoUse: 2;
		U32 uwWaitingTableCount: 8;
	} B;
} L2P_InRamInverse_t ;

typedef union {
	U32 All;
	struct {
		U32 uwFEntry: 17; // In Flash
		U32 ubFUnitIndex: 5; // In Flash,  actually use 5bit, 32 tables
		U32 btTrim_Update: 1;// For Trim Before Save P2L To Handle Special Data
		U32 ubIsInGR: 1; // In GR and not special data
		U32 ubAllZero_BM: 4;
		U32 btNeedCheckAllZero: 1;
		U32 btSequential: 1;
		U32 ubGRSearchLinkState: 2; //Level 0~2
	} B;
} Table_L2P_t ;

typedef struct {
	U32 ulFEntry: 28;
	U32 ubZByteOffset: 3;
	U32 btFromTrimBit: 1;
} GRSearchTablePhysical_t ;

typedef struct {
	U32 ulHead: 19; //GR 1056K:264k 4B
	U32 uwMemberCount: 13; //16K Group, member count 4096
} GRSearchTable_t ;

#if ENABLE_GC_GET_MAXEC_UNIT
typedef union {
	ULLONG All;
	struct {
		ULLONG ulHead: 20; //GCP2LBuffer 2M:512K* 4B
		ULLONG ulSortingGroup: 23;		// 千萬不要跟member count對調順序, 來自yh精巧的安排啊!
		ULLONG ulGroupAlreadyInQueue: 1;	//
		ULLONG ulMemberCount: 20; 		// 16K Group, member count 4096, max 32 victim unit, 不放在中間是因為會跨4B, 這樣減減的時候會出錯...
	} B;
} GCSearchTable_t ;
#else
typedef union {
	ULLONG All;
	struct {
		ULLONG ulHead: 20; //GCP2LBuffer 2M:512K* 4B
		ULLONG ulSortingGroup: 24;		// 千萬不要跟member count對調順序, 來自yh精巧的安排啊!
		ULLONG ulMemberCount: 20; 		// 16K Group, member count 4096, max 32 victim unit, 不放在中間是因為會跨4B, 這樣減減的時候會出錯...
	} B;
} GCSearchTable_t ;
#endif

typedef struct {
	UBYTE ubMask;
	UBYTE ubZCode: 3;
	UBYTE ubNoUse: 5;
	U32 ulVir4kIndex: 30;
	U32 btSpecialData: 1;
	U32 btDataValid: 1;
} SDR_DataEntryTable_t;

typedef union {
	UBYTE All;
	struct {
		UBYTE btPowerCycleStopCmd: 1;
		UBYTE btEnableSetPowerCycle: 1;
		UBYTE btPowerCycleCntStart: 1;
		UBYTE btPowerCycleWCmdLastBQHandled: 1;
		UBYTE btPowerCycleAtFTLEventDoning: 1;
		UBYTE btEnableCntDuplicateEvent: 1;
		UBYTE btNonUseBit: 2;
	} B;
} PowerCycleSetting;

typedef struct {
	U32 gulPowerCycleEventCount[128];//gulPowerCycleEventCount[320];
	U32 gulEventExecuteCount[128];
	U32 gulPowerCycleCntDuplicateEventProb;
	UBYTE gubMinBQNum[128];
	UWORD guwMaxFreeUnit;
	UBYTE gubPowerCycleEvent;
	UBYTE gubFormerPowerCycleEvent;
	UWORD guwWriteFuncLog[128];
	UWORD guwLogPtr;
	UWORD guwFormerLogPtr;
	UBYTE gubFormerSimulatePowerCycleMode;
} PowerCycleDebug_Info;

typedef struct {
	UBYTE ubNext;
	UBYTE ubPrevious;
	UWORD uwBufferIndex;

	U32 ulSectorCnt;
	U32 ulVir4kIndex;
	U32 ulDataMask;
	U32 ulSpareValid;

	union {
		UWORD uwDataInDRamIndex[4];
		struct {
			UBYTE ubLoadAlignSrcLink[4];
			UBYTE ubLoadAlignSrcLinks4kIndex[4];
		} BQ_link;
	} u;

	UBYTE ubZcode[4];

	UBYTE ubRBQIIndex;
	UBYTE ubSerialNum;
	UBYTE ubDataInDRamMap: 4;
	UBYTE btFlushWBQ: 1;
	UBYTE btPartialFromFlash: 1;
	UBYTE ubThisBQisBG: 1;
	UBYTE btWrite: 1;
	UBYTE btNeedLoadAlign: 1;
	UBYTE btSpecialData: 1;
	UBYTE btNeedCheckSpecialData: 1;
	UBYTE btPreRead: 1;
	UBYTE ub4kNum: 4;
	UBYTE btCopyFromWB: 1;
#if ENABLE_PFAIL
	UBYTE btNoUse: 3;
	UBYTE ubNeedLoadAlignCopyFromWB: 4;
#else
	UBYTE btNoUse: 7;
#endif
} BufferQueue_t;

typedef struct {
	UBYTE ubLinkFirst;
	UBYTE ubLinkLast;
	UBYTE ubLinkLastUse;
	UBYTE ubLinkFTLLast;
	UBYTE ubLinkActive;     // temp use
	UBYTE ubLinkNum;
	UBYTE ubWLinkNum;
	UBYTE ubRLinkNum;
#if ENABLE_PFAIL
	UWORD uwWrite_buffer_window; /* write buffer window in # EC. */
	UWORD uwWrite_buffer_remain;
	UWORD uwWrite_tq_2_bq;
	UWORD uwWrite_buffer_tq;
	UBYTE ubWrite_TQ_lock;
	UBYTE ubForce_clean_wbq_threshold;
	UBYTE ubLBAOverlap;
	UBYTE ubReserve;
#endif
	U32 ulHostWriteWaitBufferFlag;

	BufferQueue_t BufferQueue[BQ_NUM];
} BufferQueueInfo_t;

typedef struct {
	UBYTE ubNext;
	UBYTE ubPrevious;
	UBYTE ubRetryIndex;
	//UWORD uwRetryCount;
} HBRetryQueue_t;

typedef struct {
	UBYTE ubLinkFirst;
	UBYTE ubLinkLast;
	UBYTE ubLinkNum;
	HBRetryQueue_t HBRetryQueue[RETRY_QUEUE_MAX_COUNT];
} HBRetryQueueInfo_t;

typedef struct {
	UBYTE ubNext;
	UBYTE ubPrevious;
} L4kQueue_t;
typedef struct {
	UBYTE ubLinkFirst;
	UBYTE ubLinkLast;
	UBYTE ubLinkNum;

	L4kQueue_t L4kQueue[L4kQ_NUM];
} L4kFreeQueue_t;

typedef struct {
	UBYTE ubLinkFirst;
	UBYTE ubLinkNum;
	UBYTE DepthQueue[G_MT_QUEUE_DEPTH];
} MTDepth_t;

typedef struct {
	UBYTE ubNext;
	UBYTE ubPrevious;
	UBYTE ubPhase;
	UBYTE btSeq: 1;
	UBYTE btIsTempHit: 1;
	UBYTE ubNoUse: 6;

	UWORD uwTableInRamIndex;
	UWORD ubGRHitMap: 4;
	UWORD ub4kInBQMap: 4;
	UWORD ubSpecialDataMap: 4;
	UWORD ubContinuousMap: 4;

	L2PTable_t ulGR[4]; // 應該要隨壓縮率改變, 槫槫但現在DCCM會爆掉先用小
	U32 ulGRTableIndex[4];

	UBYTE ubUseBufferOrder[4];
} ReadBQInformation_t;

typedef struct {
	UBYTE ubNext;
	UBYTE ubPrevious;
	UWORD uwTableInRamIndex;
	UWORD uwGroupIndex;
} TempTable;

typedef struct {
	UBYTE ubLinkFirst;
	UBYTE ubLinkLast;
	UBYTE ubLinkNum;
	UBYTE ubLinkLastUse;
	UBYTE ubLinkPtr;
	TempTable gTempTableQueue[TempModeNum];
} TempTableInformation;

typedef union {
	U32 All;
	struct {
		U32 btNeedCloseTarget: 1;			// 0
		U32 btNeedCleanGR: 1;				// 1
		U32 btNeedCloseTableTarget: 1;		// 2
		U32 btFlushCache: 1;				// 3
		U32 btNeedWearLeveling: 1;			// 4
		U32 btNeedCleanGCGR: 1;			// 5
		U32 btNeedCopyData: 1;			// 6
		U32 btBG_Copy: 1;				// 7
		U32 btNeedRUTReplace: 1;			// 8
		U32 btNeedFreeUnitToFailHandle: 1;	// 9
		U32 btInitLoadTableHeader_Directly: 1;	// 10
		U32 btNeedFlushD1: 1;				// 11
		U32 btDoingAbnormalInitial: 1;		// 12
		U32 btDoingPreLoadL2P: 1;			// 13
		U32 btNeedWearLeveling_D1: 1;		// 14
		U32 btGC_WhenWrite_MiddleCleanGR: 1; // 15
		U32 btAbnormalInitialCopyDone: 1;		// 16
		U32 btBG_RetentionRead: 1;		// 17
		U32 btLast_CMD_Not_StandBy: 1;		// 18
		U32 btDoingFlushD1: 1;			// 19
#if	SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
		U32 btSATAFlushCommand: 1;
#endif
		U32 btCritical_GC: 1;
#if	SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
		U32 btNoUse: 10;
#else
		U32 btNoUse: 11;
#endif
	} B;
} FTL_t;

typedef union {
	UBYTE All;
	struct {
		UBYTE ubState: 3;
		UBYTE btVTLastPlaneState: 1; 	// 0 : erase page, 1 : UNC
		UBYTE btCleanRAMForWriteDone: 1;
		UBYTE btCleanRAMForGCDone: 1;
		UBYTE btCleanRAMForReadRetryDone: 1;
		UBYTE btSetDPSDone: 1;
	} B;
} InitState_t;

typedef union {
	U32 All;
	struct {
		UBYTE ubCount;
		UBYTE ubBad;
		FTL_t  ubFTLState;
		UBYTE uwNoUse;
	} B;
} OtherSpare_t;
typedef union {
	UBYTE All;
	struct {
		UBYTE btHandleTableFlag: 1;
		UBYTE btRWTableUnit: 1;
		UBYTE btRWTable_P2L: 1;
		UBYTE btPartialFromFlash: 1;
		UBYTE btUserData: 1;
		UBYTE btQueryCheck: 1;
		UBYTE btCanRemove: 1;
		UBYTE btConvBypass: 1;
	} B;
} FQ_Para;

typedef union {
	U32 All; // use for read error handle
	struct {
		UWORD uwSignoffMaskFailLog: 16;
		UWORD uwFipINTInfo: 16;
	} W;
} FIPError_t;

typedef union {
	UWORD All;
	struct {
		UWORD ubSpareValid: 2;
		UWORD uwNoUse: 14;

	} B;
} FillVirMark1_t ;

typedef union {
	UWORD All;
	struct {
		UWORD uwPlaneIndex;
	} B;
} RC_Info_t;

typedef struct {//環狀FIFO Queue
	UWORD uwQueue[MAX_PUSHCNT];//UnitIndex
	UWORD uwQueuePushCnt;//Queue裡面有多少Unit需要被ForceReadMove
	UWORD uwQueueQuota;//可以做的限額
} ForceRead_Info_t;//8192Bytes

typedef struct {
	//CopyBack Info
	UWORD uwStartProgramOrderIndex;
	UWORD uwEndProgramOrderIndex;
	UWORD uwProgramOrderIndex;	//for Next
	UBYTE ubProgramOrder;	//First, Foggy, Fine
	UBYTE ubLMUIndex;	//Lower, Middle, Upper
	UWORD uwD3TargetWL;

	UBYTE ubQueue[FQ_NUM];

	UBYTE ubQueueIndex;
	UBYTE ubQueueFirst;//紀錄最先放進去的Queue , 需要最先被檢查回收的Queue
	UBYTE ubQueuePushCnt;
	UBYTE ubCopyBackMode;	//BIT0: Nromal(3D1 to 1D3); BIT1:WL Skip , Run Even Page(6D1 to 1D3); BIT2:WL Skip , Run Odd Page(6D1 to 1D3),

	//below for B0KB
#if B0KB
	UWORD uwLPage;
	UWORD uwUPage;
#endif
} CB_Info_t;

typedef struct {
	UBYTE ubPrevious;			//4B_1
	UBYTE ubNext;
	UBYTE ubCEIndex;
	FQ_Para FQPara;

	UBYTE ubFJob: 4;				//4B_2
	UBYTE btUNServed: 1;
	UBYTE btCheckEmpty: 1;
	UBYTE btThisIsErasePage: 1;
	UBYTE btPEFail: 1; // program or erase fail
	UBYTE ubFQPhase;
	UWORD uwFJobInfo;

	UWORD uwDestTableInRAMIndex;		//4B_3
	UWORD uwSourceTableInRAMIndex;

	UBYTE ubL4kIndex;			//4B_4
	UBYTE ubL4kNum;
	UBYTE ubRSFrameReuseCount;
	UBYTE btRSPatchUpWritePhaseCmd: 1;
	UBYTE btUncCheckInRSTwoCh: 1;
	UBYTE ubFirstUncFrameNoMapInRSTwoCh: 4;
	UBYTE btSpareInvalid: 1;
	UBYTE btReadUNC: 1;

	UBYTE ubFSAIndex;			//4B_5
	UBYTE ubRSPageNum;
	UBYTE ubALUIndex: 2;
	UBYTE btRedoFQ: 1;
	UBYTE btNeedSetCEWhenTriggerDMA: 1;
	UBYTE btRSEncodeMode: 1;
	UBYTE btRSLastPage: 1;
	UBYTE btRSProg2ndParity: 1;
	UBYTE btRSOneParityEn: 1;
	UBYTE ubRSFrameIndex: 5;	// max 32 tags
	UBYTE ubMTDepthCnt: 3;

	UBYTE btCopyFromWB: 1;			//4B_6
	UBYTE btRecordFailLogByMyself: 1;		// if this bit is 0 (default), FLA will do record fail log if error occurred.
	UBYTE btLastProgram: 1;
	UBYTE btBunchRSParity: 1;
	UBYTE btIsMixPlaneUnit: 1;
	UBYTE btTableVerify: 1;
	UBYTE btDirectHandling : 1;
	UBYTE btTLCProgramCodeBlock: 1;
#if MicronFlashOnly
	UBYTE ubInExitSLCDepth;
#else

#if ENABLE_DO_AUTOPOL_TIMEOUT_HANDLING
	UBYTE btUsingAutoPol: 1;
#if ENABLE_READ_AUTO_POL_TIMEOUT_HANDLING
	UBYTE btPolFailTimeoutCnt: 7;
#else
	UBYTE btNoUseBit_2: 7;
#endif

#else
	UBYTE ubNoUseByte_1;
#endif
#endif
	UWORD uwFUnit;

	U32 ulFEntry;				//4B_7

	UBYTE ubDepth;				//4B_8
	UBYTE ubRelativeDepth;
	UBYTE ubRemainderDepth;
	UBYTE ubRedundantDepth;

	UBYTE ubUseBufferOrder[16];		//4B_9 ~ 4B_12 	//16B

	UWORD uw4KInFQMap;			//4B_13
	UWORD uwDataInSDRMap;//->1bit?

	UWORD uwDataInSDRIndex[4];		//4B_14 ~ 4B_15
	U32 ulRAMAddr;       			//4B_16
	U32 ulDataMask;				//4B_17

	U32 ulSpare;     				//4B_18

	FIPError_t ulFIPError;			//4B_19

	UBYTE ubBufferInedex_R; 		//4B_20
	UBYTE btNoWait: 1;
	UBYTE btRWVCTable: 1;
	UBYTE btPreRead: 1;
	UBYTE btNeedToClearWrite4kBufferFlag: 1;
	UBYTE btParity: 1;
	UBYTE ubWritePhaseRedoCmd: 2;
	UBYTE btNeedStopDMA: 1;
	UBYTE ubD3WriteMode;
	UBYTE btD3: 1;
	UBYTE btOutOfOrder: 1;
	UBYTE btReadMove: 1;
	UBYTE btGenFail: 1;
	UBYTE ubCheckStatusPhase: 4;	// for program fail plane check

#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
	U32 ulAutoPolStartTime;			//4B_21
#if (MicronFlashOnly && ENABLE_DO_AUTOPOL_TIMEOUT_HANDLING)
	UBYTE btUsingAutoPol: 1;			//4B_22
	UBYTE btNoUseBit_2: 7;
	UBYTE ubNoUseByte_2[3];
#endif
#endif
#if ENABLE_RESET_RETRY_LEVEL
	UBYTE ubBicsRetryLevelDepth;
#endif
} FlashQueue_t; 	// 目前Total 80 or 88 Bytes 1~20 or 1~22 組 4B

enum enumWriteWaitCnt {
    WaitCnt_Default = 255,
    WaitCnt_16CE = 40,
    WaitCnt_Less16CE = 200,
    WaitCnt_GC = 0
};

typedef struct {
	UBYTE ubXferPhase[MAX_CE];
	UBYTE ubFQFirst[MAX_CE + 1];
	UBYTE ubFQIndex[MAX_CE + 1][FQ_NUM];
	UBYTE ubFQDoingNumber[MAX_CE + 1];
	UBYTE ubFQSeqNumber[MAX_CE];
	FlashQueue_t gFQLink[gFQ_NUM];
	UBYTE ubFQLinkNumber;
	UBYTE ubFQLinkNumber_W;
	UBYTE ubFQLinkFirst;
	UBYTE ubFQLinkFree;
	U32 ulPreviousEntry[MAX_CE];
#if (TLC && (!BURNER))
	UBYTE ubNoUse[1]; // 4 Byte Alignment for Dump Sram tool
	CB_Info_t gCB_Info_t[MAX_LOGICAL_CE];
#endif
} FlashQueueInfo_t;

typedef union {
	UWORD All;
	struct {
		UWORD uwTarget: 15;
		UWORD btFastPage: 1;
	} B;
} FTarget_t;
//**************** D1 Unit Type ******************
typedef union {
	UBYTE All;
	struct {
		UBYTE ubD1Link;
	} B;
} D1LinkTable_t;

//************************************************
//if modify RUTLog_t struct, plz take care MarkFail function bit mapping (ulRUTLogTmp.All & 0xFF7FFFFF) by Steven
typedef union {
	U32 All;
	struct {
		UWORD uwUnit;
		UBYTE ubCEIndex: 5;
		UBYTE ubBurstBank: 2;
		UBYTE btNeedMarkBad: 1; // 1: UNC need to mark bad, 0: EOT only replace unit
		UBYTE btIsD1Unit: 1;
		UBYTE ubFJob: 3;
		UBYTE ubFailPhase: 2; //0: All UNC(HB, SB, RS fail), 1: HB fail, 2: HB & SB fail
		UBYTE ubD1UnitIndex: 2; //TLC : 0\1\2
	} B;
} RUTLog_t;

typedef union {
	U32 All;
	struct {
		UWORD uwUnit;
		UBYTE ubCEIndex: 5;
		UBYTE ubBurstBank: 2;
		UBYTE btIsD1Unit: 1;
		UBYTE ubD1UnitIndex: 2;
		UBYTE ubFJob: 3;
		UBYTE ubOccurCnt: 3;
	} B;
} MarkBadLog_t;

typedef union {
	struct {
		U32 ubLogType: 4;				// 4B
		U32 ubTemperature: 4;
		U32 ubPowerOnCnt: 8;
		U32 ulVir4kIndex_L: 8;
		U32 ubFLHErrorMap: 8;
		U32 uwFTLState: 16;			// 4B
		U32 ubFailType: 4;
		U32 ubTableSel: 4;
		U32 ubAbnormalLoc:  8;
		U32 uwFUnit: 16;				// 4B
		U32 uwFEntry: 16;
		U32 ubD1Link: 8;				// 4B
		U32 btD3: 1;
		U32 btIsFastPage: 1;
		U32 ulVir4kIndex_H: 22;
	} FLHLog;
	struct {
		U32 ubLogType: 4;				// 4B
		U32 ubTemperature: 4;
		U32 ubPowerOnCnt: 8;
		U32 ubLastPowerCycling: 8;
		U32 ubNoUse: 8;
		U32 ulFTLState; 				// 4B
		U32 ulFailAddr;					// 4B
		U32 ulDD3_ECC_CTRL;			// 4B
	} DDRErrLog;
	struct {
		U32 ubLogType: 4;				// 4B
		U32 ubTemperature: 4;
		U32 ubPowerOnCnt: 8;
		U32 ubLastPowerCycling: 8;
		U32 ubNoUse: 8;
		U32 ulFTLState; 			// 4B
		U32 ulNoUse;					// 4B
		U32 ulSYS1L_INT_STS3;		// 4B
	} ParityErrLog;
	struct {
		U32 ubLogType: 4;				// 4B
		U32 ubTemperature: 4;
		U32 ubPowerOnCnt: 8;
		U32 ubLastPowerCycling: 8;
		U32 ubAbnormalLoc: 8;
		U32 ulFTLState;		// 4B
		U32 ubFailType: 4;		// 4B
		U32 ulNoUse1 : 28;
		U32 ulNoUse2;		// 4B
	} FTLLog;
	struct {
		U32 ubLogType: 4;				// 4B
		U32 ubTemperature: 4;
		U32 ubPowerOnCnt: 8;
		U32 ubTag: 8;
		U32 btLBAOverlap: 1;
		U32 btIsWrite: 1;
		U32 btLargeSize: 1;
		U32 ubECCnt_MinusOne: 5;
		U32 ulFTLState;		// 4B
		U32 ulLBA;		// 4B
		U32 ulElapsedTime;		// 4B
	} NCQTimeoutLog;
	struct {
		U32 ubLogType: 4;				// 4B
		U32 ubTemperature: 4;
		U32 ubPowerOnCnt: 8;
		U32 ubCMD: 8;
		U32 ulNoUse1: 4;
		U32 ubErrorCode: 4;
		U32 ulFTLState;		// 4B
		U32 ulElapsedTime;		// 4B
		U32 ulNoUse2;		// 4B
	} ATATimeoutLog;
} FailLog_t;

//added by Victor
typedef struct {
	UWORD uwNextIndex;
	UWORD uwByteOffsetDBT;
	UBYTE ubDBTSetValue;
} DBTModifyList_t;

typedef union {
	ULLONG All;
	struct {
		UWORD uwUnit;//D3值.
		UWORD uwRUTLogCnt;
		UWORD ubUnitD1Link: 8;//D1值. ( 位於gubD1LinkTable[ ]裡的第幾個Unit )
		UWORD btNeedCopyData: 1;
		UWORD btIsTableUnit: 1;
		UWORD btIsD1Unit: 1;
		UWORD btSpecialCase: 1;// all erase fail 都要馬上換RUT,  或VT child/Initinfo unit發生read move/program fail
		UWORD btHandleLogDone: 1;
		UWORD btNeedCopyActiveTableUnit: 1;	// set when program table unit fail
#if ENABLE_AUTOPOL_TIMEOUT_HANDLING
		UWORD btAutoPolTimeout: 1;
		UWORD ubNoUse: 1;
#else
		UWORD ubNoUse: 2;
#endif
		UWORD uwNoUse;
	} B;
} RUTSummaryLog_t;

typedef union {
	UBYTE All;
	struct {
		UBYTE btAll_Done: 1;
		UBYTE btPartial_Done: 1;
		UBYTE btDMA_Done: 1;
		UBYTE btAbort_Done: 1;
		UBYTE btEOT: 1;
		UBYTE btSTA: 1;
		UBYTE btMT_STOP: 1;
		UBYTE btPFA: 1;
	} B;
} INT_t;

typedef union {
	UBYTE All;
	struct {
		UBYTE ubZCode: 3;
		UBYTE btSpecialData: 1;
		UBYTE ubNoUse: 4;
	} B;
} ZINFO_t;

typedef union {
	U32 All;
	struct {
		U32 ubCON_LBA_TAG: 6;
		U32 btCON_LBA: 1;
		U32 btCON_LBA_Trigger: 1;
		U32 ubNCQ_NEXT_Align_RD: 6;
		U32 ubNCQ_NEXT_RD: 6;
		U32 ubNCQ_NEXT: 6;
		U32 ubNCQ_CMD_VLD_SUM: 6;
	} B;
} RD_NCQ_INFO;

typedef union {
	U32 All;
	struct {
		U32 ubCON_LBA_TAG: 6;
		U32 btCON_LBA: 1;
		U32 btCON_LBA_Trigger: 1;
		U32 ubNCQ_NEXT_Align_WR: 6;
		U32 ubNCQ_NEXT_WR: 6;
		U32 ubNCQ_NEXT: 6;
		U32 ubNCQ_CMD_VLD_SUM: 6;
	} B;
} WR_NCQ_INFO;


typedef struct {
	U32 ulLBA;
	U32 ulSectorCnt;
	UWORD uwEC_BQDone;
	UWORD uwEC;
	UWORD uwEC_MultiTrig;
	UWORD uwBufferPTR;
	U32 ulTrigTime;
} WR_NCQ_CMD_INFO_t;

typedef union {
	U32 All;
	struct {
		UWORD uwReceived_EC;
		UBYTE ubCurrentTag;
		UBYTE ubLatestDoneTag;
	} B;
} SATA_MSG_TYPE3;

typedef struct {
	U32 ulLBA;
	U32 ulSectorCnt;
	U32 ulDataMask;
	UWORD uwBufferIndex;
} XferDataOut_t;
typedef struct {
	U32 ulVir4kIndex;
	UWORD uwBufferIndex;
	UBYTE ubToDoLink;
	UBYTE btSpecialData: 1;
	UBYTE btNeedLoadAlignment: 1;
	UBYTE ubSectorNum: 3;
	UBYTE ubNoUse: 3;
} XferDataIn_t;


typedef struct {
	U32 ulCmd;
	U32 ulLBA;
	U32 ulSectorCnt;
	U32 ulOther;
} DebugCommandLog_t;


typedef union {			//maybe change to 4 bytes
	ULLONG All[2];
	struct {
		ULLONG btEDO: 1;         //bit 0
		ULLONG btTwoPlane: 1;    //bit 1
		ULLONG btFlash4k: 1;     //bit 2
		ULLONG btFlash8k: 1;     //bit 3
		ULLONG btABL: 1;         //bit 4
		ULLONG btCacheProgram: 1; //bit 5
		ULLONG btExtendBlock: 1; //bit 6
		ULLONG btCopyBack2: 1;    //bit 7
		ULLONG btCacheRead: 1;   //bit 8
		ULLONG btToshiba: 1;     //bit 9
		ULLONG btHynix: 1;       //bit 10
		ULLONG btSLC: 1;         //bit 11
		ULLONG bt4LC: 1;         //bit 12
		ULLONG bt8LC: 1;         //bit 13
		ULLONG btFlash2k: 1;     //bit 14
		ULLONG btSamsung: 1;     //bit 15
		ULLONG btMultiDie: 1;    //bit 16
		ULLONG btSandisk: 1;     //bit 17
		ULLONG btIntelMicron: 1; //bit 18
		ULLONG bt8Bit: 1;        //bit 19
		ULLONG btNoSupport: 1;   //bit 20
		ULLONG btTwoPlaneRead: 1; //bit 21
		ULLONG btToshiba43nm: 1; //bit 22
		ULLONG btToshiba32nm: 1; //bit 23
		ULLONG btA2Cmd: 1;      //bit 24
		ULLONG bt256Page: 1;     //bit 25
		ULLONG btIntelMicronL74A: 1;   //bit 26
		ULLONG btToshiba24nm: 1; // bit27
		ULLONG btToshiba24nmABL: 1;  // bit28
		ULLONG btFlash16k: 1;    //bit29
		ULLONG btToshiba19nm: 1; // bit30
		ULLONG btToggle: 1; // bit31
		ULLONG btONFI: 1; // bit32
		ULLONG btRandCacheRead: 1; // bit33
		ULLONG bt512Page: 1; // bit34
		ULLONG btHynixF20Adie: 1; // bit35
		ULLONG btHynixF20Bdie: 1; // bit36
		ULLONG btIntelMicronL95B: 1; // bit37 , no use
		ULLONG btToshiba1Ynm: 1; // bit38
		ULLONG btFourPlane: 1; // bit39
		ULLONG btToshiba1Znm: 1; // bit40
		ULLONG bt128Page: 1; //bit41;
		ULLONG btPseudoPlaneEn: 1;	//bit42
		ULLONG bt258Page: 1; 		//bit43
		ULLONG bt384Page: 1; 		//bit44
		ULLONG btWordline_Skip: 1;	//bit45
		ULLONG bt1024Page: 1;      	//bit 46
		ULLONG btIntelMicronL06B: 1; // bit 47
		ULLONG btNVDDR: 1;			// bit 48
		ULLONG btNVDDR2: 1;			// bit 49
		ULLONG btToshibaBiCs2: 1;	//bit 50
		ULLONG bt576Page: 1;		//bit51
		ULLONG btToshibaBiCs3: 1; //bit52
		ULLONG bt768Page: 1; 		//bit53
		ULLONG btIntelMicronB0KB: 1; //bit54
		ULLONG btHynix14nm: 1; // bit55
		ULLONG btHynix16nm: 1; // bit56
		ULLONG btSNDK160G: 1; // bit57
		ULLONG btEnterprise: 1; // bit58
		ULLONG btIntelMicronB16A: 1; // bit 59
		ULLONG btHynix3D: 1;	//bit60
		ULLONG btIntelMicronB17A: 1;// bit 61
		ULLONG btIntelMicronB27A: 1;// bit 62
		ULLONG btToshibaBiCs4: 1; //bit63
		ULLONG btReservedCanNotUse3: 1;	// bit 64 //Taipei Named
		ULLONG bt16LC: 1;				// bit 65
		ULLONG btIntelMicronN18A: 1;	// bit 66
		ULLONG btIntelMicronB27B: 1;	// bit 67
		ULLONG btNVDDR3: 1;				// bit 68
		ULLONG btIntelMicronN28A: 1;    // bit 69
		ULLONG btHynix3DV5: 1;	// bit70
		ULLONG btHynix3DV6: 1;	// bit71
		ULLONG btYMTC: 1;	// bit72
		ULLONG btYMTCJGS: 1;    // bit73
	} B;
} FlashType_t;


struct CHS_struct {
	UBYTE Sec_Num;
	UBYTE Head;
	UWORD Cyl;
};



struct INT_struct {
	UWORD INT1;
	UWORD INT2;
};


struct BYTE_struct {
	UBYTE BYTE_1;  // LL
	UBYTE BYTE_2;  // LH
	UBYTE BYTE_3;  // HL
	UBYTE BYTE_4;  // HH
};


union IntInt {  // 4 Bytes
	unsigned int All;
	struct BYTE_struct BYTEMODE;
	struct INT_struct INTMODE;
};


union intlba {  // 4 Bytes
	unsigned int ulItem;
	unsigned int All;
	struct CHS_struct CHS;
	struct INT_struct INTMODE;
	struct BYTE_struct BYTEMODE;
};



typedef struct {
	U32 ulStartLBA[5];
	U32 ulEndLBA[5];
	UWORD uwCurrentSpan;
	UWORD uwPendingTime;
	UWORD uwFeatureFlags;
	UBYTE ubOffLineDoing;
	UBYTE ubNoUse;
} SMART_SELECTIVE_SELFTEST;

typedef struct {
	UBYTE ubCurrentCount;
	UBYTE ubEndCount;
	UBYTE ubNeedInvert;
	UBYTE ubSuspend;
	U32 ulPattern;
} SANITIZE_OVERWRITE;

typedef struct { //S5
	//security
	UBYTE ubSecMode; //0
	UBYTE ubSecCounter;// 1
	UWORD uwSecMasterID;// 2-3
	UWORD uwSecMasterPW[16];// 4-35
	UWORD uwSecUserPW[16]; // 36-67

	//HPA
	UBYTE ubHpaState; // 68
	UBYTE ubHpaSECounter; // 69
	UWORD uwHpaPW[16]; //70-101
	UWORD uwNoUse1;//102-103
	U32 ulHpaSize; //104-107
	U32 ulReadNativeMaxLBA; //108-111

	//DCO
	UWORD uwDCOState; //112-113
	UWORD uwNoUse2;//114-115
	U32 ulDcoSize; // 116-119
	UWORD IDM_W63;//120-121
	UWORD IDM_W69;//122-123
	UWORD IDM_W76;//124-125
	UWORD IDM_W78;//126-127
	UWORD IDM_W79;//128-129
	UWORD IDM_W82_85;//130-131
	UWORD IDM_W83_86;//132-133
	UWORD IDM_W84_87;//134-135
	UWORD IDM_W88;//136-137
	UWORD IDM_W_Security;//138-139
	UWORD IDM_W_Trim;//140-141

	//smart
	UBYTE ubSmartState;//142
	UBYTE ubOfflineStatus;//143
	UBYTE ubSelfTestStatus;//144
	UBYTE ubSmart01hIndex;//145
	UBYTE ubSmart02hIndex;//146
	UBYTE ubNoUse3;//147
	UWORD uwSmart03hIndex;//148-149
	UBYTE ubSmart06hIndex;//150
	UBYTE ubSmart07hIndex;//151
	UBYTE ubSmart02hPage; //152 valid page number
	UBYTE ubSmart03hPage; //153 valid page number
	UBYTE ubSmart06hPage; //154 valid page number
	UBYTE ubSmart07hPage; //155 valid page number
	UBYTE ubSmart09hPage; //156 valid page number
	UBYTE ubNoUse4;//157
	UWORD uwSmart02hErrCnt;//158-159
	UWORD uwSmart03hErrCnt;//160-161
	UWORD uwNoUse5;//162-163
	U32 ulSMARTSelfTestStartLBA;//164-167
	SMART_SELECTIVE_SELFTEST gSMARTSelective;//168-215

	//sanitize
	UBYTE ubSanitizeState;//216
	UBYTE ubSanitizeValue;//217
	UWORD uwNoUse6;//218-219
	SANITIZE_OVERWRITE gSanitizeOverwrite;//220-227

	//AMAX
	UBYTE ubAmaxState;//228
	UBYTE ubNoUse7;//229
	UBYTE ubNoUse8;//230
	UBYTE ubNoUse9;//231
	U32 ulAmaxSize;//232-235
	U32 ulNoUse10;//236-239

	//date and time
	ULLONG uoNoUse1;// 240-247
	ULLONG uoNoUse2; // 248-255

	//temperature
	UBYTE ubTemperatureMax; //256
	UBYTE ubTemperatureMin; //257

	UBYTE ubSecurityEraseIsDoing; //258
	UBYTE ubLastCmd; //259

	//dco temp
	UWORD uwPM_CTRL_Temp; //260-261
	UBYTE ubDevIPMON_Temp; //262

	// Last Security Erase Mode
	UBYTE ubLastSecurityEraseMode;//263

	U32 ulWriteNon4KAlignedCmdCnt;//264-267
	U32 ulWriteTotalCmdCnt;//268-271
} ATACONFIG;


typedef struct { //S5
	UWORD  W49;
	UWORD  W59;
	UWORD  W63;
	UWORD  W69;
	UWORD  W76;
	UWORD  W77;
	UWORD  W78;
	UWORD  W79;
	UWORD  W82;
	UWORD  W83;
	UWORD  W84;
	UWORD  W85;
	UWORD  W86;
	UWORD  W87;
	UWORD  W88;
	UWORD  W91;
	UWORD  W119;
	UWORD  W120;
	UWORD  W161;
	UWORD  W165;
	UWORD  W168;

} Identify_Table;

typedef struct {
	UBYTE ubNr;
	UBYTE ubPar;
	UBYTE ubPbr;
	UBYTE ubPcr;
} PLL;

typedef struct {
	UWORD uwFUnit;
	UWORD uwRSGroupTag;
	UWORD uwRSNormalModeBetterMap[RS_NORMAL_MODE_MAX_FPUCOR_SIZE];
} RS_NORMAL_MODE_COR_BETTER_MAP_t;

typedef union {
	U32 ulAll;
	struct {
		U32 uwFAIL_INT_INF: 16;
		U32 ubPlaneNoInPage: 8;
		U32 btReadErasePage: 1;
		U32 btReadUNC: 1;
		U32 btUNCFrameMap: 4;
		U32 btNoUse: 2;
	} BitMap;
} RS_FPU_FAIL_CONDITION_t;

typedef struct {
	// Read Fail : 13 * 4B
	U32 gulUNCandLCAfail_DebugCount;
	U32 gulECCOverTH_LogCount;
	U32 gulRetryECCOverTH_LogCount;
	U32 gulRetryLCAErrorInDummyFPUCOR_LogCount;
	U32 gulRetryLCAErrorInFPUCOR_LogCount;
	U32 gulRetryLCAErrorInCheckSendCmd_LogCount;
	U32 gulRetryCRC32InDummyFPUCOR_LogCount;
	U32 gulRetryCRC32InFPUCOR_LogCount;
	U32 gulRetryCRC32InCheckSendCmd_LogCount;
	U32 gulRetryCRC32InCheckRead_LogCount;
	U32 gulRetryFWSettingErrorInDummyFPUCOR_LogCount;
	U32 gulRetryFWSettingErrorInFPUCOR_LogCount;
	U32 gulRetryFWSettingErrorInCheckSendCmd_LogCount;
	// HB : 69 * 4B
	U32 gulHBRetry_NormalReadAgain_Pass_DebugCount;
	U32 gulRetryPassCount[68]; //ref RETRY_SEQUENCE_MAX_COUNT. +1 for normal read again
	// SB : 9 * 4B
	U32 gulSoftBit_Fail_DebugCount;
	U32 gulSoftBit_1_Pass_DebugCount;
	U32 gulSoftBit_2_Pass_DebugCount;
	U32 gulSoftBit_3_Pass_DebugCount;
	U32 gulSoftBit_4_Pass_DebugCount;
	U32 gulWP_DEBUG_sb_TARPINGset;
	U32 gulWP_DEBUG_RSdoSBCount;
	U32 gulWP_DEBUG_GenRSdoSBCount;
	U32 gulWP_DEBUG_SBReadParityCount;
	// RS : 13 * 4B
	U32 gulRSErrorInWrittingDB_DebugCount;
	U32 gulRSErrorInWrittingDB_RSREG_JUDGE_ENCCNT_DONE_DebugCount;
	U32 gulRSErrorInWrittingDB_RSREG_JUDGE_ENCCNT_NOTDONE_DebugCount;
	U32 gulRSErrorInCopingGB_DebugCount;
	U32 gulRSErrorInCopingGB_RSREG_JUDGE_ENCCNT_DONE_DebugCount;
	U32 gulRSErrorInCopingGB_RSREG_JUDGE_ENCCNT_NOTDONE_DebugCount;
	U32 gulRSErrorInSameArea_EncodeDone_WrittingTB_DebugCount;
	U32 gulRSErrorInSameArea_NotEncodeDone_WrittingTB_DebugCount;
	U32 gulRSErrorInLastArea_ParityInRAM_WrittingTB_DebugCount;
	U32 gulRSErrorInLastArea_ParityNotInRAM_WrittingTB_DebugCount;
	U32 gulRSErrorInLastTwoAreaAgo_WrittingTB_DebugCount;
	U32 gulRSErrorInNotUsingBlock_StaticTB_DebugCount;
	U32 gulRSErrorInNotUsingBlock_StaticBLK_DebugCount;
	// RS Normal mode : 6 * 4B
	U32 gubWP_DEBUG_RS_NORMAL_MODE_1_Count;
	U32 gubWP_DEBUG_RS_NORMAL_MODE_2_Count;
	U32 gubWP_DEBUG_RS_NORMAL_MODE_FPUCOR_Count;
	U32 gubWP_DEBUG_RS_NORMAL_MODE_FPUCOR_OKCount;
	U32 gubWP_DEBUG_RS_NORMAL_MODE_LACKTRUE_Count;
	U32 gulRSDebug_NormalModeGenParityPageError_DebugCount;
	// UNC : 4 * 4B
	U32 gulUNC_GetFailL4KNo_DebugCount;
	U32 gulRetry_Fail_NotRS_DebugCount;
	U32 gulReadUNC_DebugCount;
	U32 gulPreformatUNCNoRetry_LogCount;
} READ_FAIL_DEBUG_COUNT_t;

typedef struct {
	UWORD                     guwRUTFBlock[MAX_PLANE_NUM]; //0-127
	UWORD           guwDieStartSFUnitIndex[8]; //128-143          //各個Die起始的single free Unit編號.
	//    guwDieStartSFUnitIndex[gubDieNumber]=最後一個Die的最後 (最大) single free Unit編號.
	UWORD           guwRUTUnitIndexOfLastBlock[256]; //144-655

	FTarget_t           guwGRTarget[16]; //656-687
	FTarget_t           guwGCGRTarget[MAX_GCGRUNIT]; //688-695
	FTarget_t           guwTableTarget[MAX_TABLEUNIT]; //696-759
	FTarget_t              guwRUTTarget; //760-761
	FTarget_t           guwCloseTableTarget; //762-763
	FTarget_t         guwInitInfoTarget; //764-765
	FTarget_t            guwCopyTarget; //766-767
	FTarget_t           guwVTAreaFreeTarget[VTAREA_UNITS]; //768-775

	FTarget_t            guwVTChildTarget; //776-777
	FTarget_t              guwOldRUTTarget; //778-779                        // In power cycle flow, if guwOldRUTTarget != Default_Unit, Put it into free
	UWORD                 guwFreeBlockCount_D3; //780-781
	UWORD               guwFreeBlockCount_D1; //782-783

	UBYTE          gubVTAreaFreeCount;     //784
	UBYTE          gubVTAreaFreeQueueOutPTR; //785
	UBYTE          gubVTAreaFreeQueueInPTR; //786
	UBYTE                    gubGRTargetIndex; //787

	U32       		gulGRTargetPTR;    //788-791       			// Active GR physical plane PTR
	U32                  gulGRTargetMiddleP4kPTR; //792-795 // Record GR PTR before doing cleanGR in middle done, for power cycle scan

	U32                      gulGRTableL4kPTR; //796-799                            // Active GR Table L4K PTR
	U32                      gulGRTableL4kNum; //800-803                      // GR Table total L4K NUmber
	U32                  gulGRTableStartIndex[16]; //804-867            // GR Unit x start L4K Index
	U32                  gulGRTableTempStartIndex; //868-871
	U32                  gulGRTableTempL4KPTRIncNum; //872-875

	UWORD                     guwVTPTR; //876-877                                    // VT physical plane PTR
	UWORD                     guwVTChildPTR; //878-879                            // VT Child physical plane PTR

	U32                    gulCopyTargetPTR; //880-883                 // Copy physical plane PTR
	U32                  gulCopyTableL4kPTR; //884-887                    // Copy Table L4K PTR

	U32                   gulGCGRTargetPTR; //888-891                 // GCGR physical plane PTR
	U32                      gulGCGRTableL4kPTR; //892-895                // GCGR Table L4K PTR

	UBYTE             gubGCGRTargetNum; //896
	UBYTE             gubGCGRTargetIndex; //897
	UWORD                    guwInitInfoTargetPTR; //898-899                     // ParityUnit physical plane PTR

	UBYTE                  gubTableTargetIndex; //900
	UBYTE                    gubTableTargetSyncIndex; //901
	UWORD                    guwTableTargetPTR; //902-903                // TableUnit physical plane PTR

	UWORD           guwTableTargetSyncPTR; //904-905        // L2P & VC match plane PTR
	UWORD           guwCloseTargetCount; //906-907

	UWORD           guwGRLastP2LPTR; //908-909                        // Active GR P2L. Be saved in guwInitInfoTarget
	UWORD           guwTableInitInfoPTR; //910-911                     // Table_Header, Table_P2L, Seq_Table

	UBYTE		gubRSCurrentTag;//912
	UBYTE		gubRSIdleTag;//913
	UBYTE		gubRSShiftTagPhase;//914
	UBYTE		gubDPS_Stage;//915

	UWORD               guwRSFrameCount[32]; //916-979                // Max 32 tags......GR:(8~15)  GCGR:(0~3)  Table:4~7
	U32                  gulTotalEraseCount; //980-983
	U32                  gulNeedCleanGRL4KCount; //984-987           // for BG cleanGR judgement
	FTL_t                 gulFTLState; //988-991
	//=============================================================================================
	U32           		gulTotalValidLCACnt;//992-995
	U32           		gulPreviousMaxEraseCnt_D3;//996-999
	U32           		gulPowerOnCnt;//1000-1003
	U32           		gulPowerCyclingCnt;//1004-1007
	U32           		gulPowerOnTimeInMinutes;//1008-1011
	UWORD           		guwMinimumEC_UsedUnit;//1012-1013
	FTarget_t           guwOldVTChildTarget; //1014-1015
	ULLONG          	guoTotalHostWriteSectorCnt;//1016-1023
	ULLONG          	guoTotalHostReadSectorCnt;//1024-1031
#if TLC
	ULLONG           guoTotalNandWritePlaneCnt_D3TLCData;//1032-1039
#else
	ULLONG          	guoTotalNandWriteSectorCnt;//1032-1039
#endif
	ULLONG          	guoTotalNandReadSectorCnt;//1040-1047
#if TLC
	ULLONG           guoTotalNandWritePlaneCnt_D3SLCData;//1048-1055
	ULLONG           guoTotalNandWritePlaneCnt_D3SLCTable;//1056-1063
#else
	ULLONG           	guoTotalNandWriteData;//1048-1055
	ULLONG		guoTotalNandWriteTable;//1056-1063
#endif
	U32		gulSLCPoolSize;//1064-1067
	U32		gulSLCPoolSizeMax;//1068-1071
	//=============================================================================================716B
	U32           	gulTestCount[16];//1072-1135
	UBYTE       	gubDramType;   //1136                                         // 12 bytes for put ddr/sdr information
	UBYTE       	gubDramSize;//1137
	UWORD                    guwDDR_DQS_FINE; //1138-1139
	UBYTE             gubDDR_DQS_G; //1140
	UBYTE             gubSDR_Corase; //1141
	UBYTE            gubDram_Bank; //1142
	UBYTE	gubSkipCheckWLCount;//1143
	UWORD           guwSDR_Fine; //1144-1145
	UWORD           guwFreeBlockCountMax; //1146-1147
	//================error record==================
	U32                       gulReadRetryCnt; //1148-1151                      // HB Retry ++
	U32                  gulReadSBCnt; //1152-1155                           // SB Retry ++
	U32                  gulReadRSCnt; //1156-1159                           // RS Recover ++
	ULLONG           guoTotalNandWritePlaneCnt_D1Data; //1160-1167
	//U32       	gulNoUseMiddle4B_1[0];
	FailLog_t     guoFailLog[VTLogEntryNum]; //1168-2191			// 1K, 16B per entry
	UBYTE 	gubFailLogPTR; //2192
	UBYTE 	gubNoUseMiddleByte; //2193
	UWORD	guwPhyDieReadFailShiftTime; //2194-2195		// 記錄 read fail log爆了幾次
	UBYTE   gubD1DieCount[4]; //2196-2199
	UWORD   guwD1UnitsPerGroups[16]; //2200-2231
	UBYTE	gubPhyDieReadFail[64];	//2232-2295		// 16 CE * 4 Die
	U32		gulRUTMappingUpdateCnt; //2296-2299
	U32		gulRSGroupTagFailMapSetUpDebugCnt; //2300-2303
	U32			gulBunchInitInfoTarget;	//2304-2307				// store InitInfoTarget when received Flush command
	U32			gulBunchInitInfoTargetPTR; //2308-2311				// store InitInfoTargetPTR when received Flush command
	U32			gulGRTargetPTRBackup;	//2312-2315				// store GRTargetPTR when received Flush command
	U32			gulVCOfOldGRTarget;		//2316-2319
	UWORD		guwRemainVCOldGRTarget; //2320-2321
	UWORD		guwRemainVCOldGRTargetCnt; //2322-2323
	U32			gulBunchRSRecoverEnterCnt;	//2324-2327
	U32			gulBunchRSRecoverSuccessCnt; //2328-2331
	U32			gulBunchRSRecoverFailErrorPageOver2Cnt; //2332-2335
	U32			gulBunchDummyFlowCnt;	// 2336-2339
	U32			gulCleanGRInMiddleWhenGCDoingCnt;	// 2340-2343
	READ_FAIL_DEBUG_COUNT_t gsReadFailDebugCount; // 2344-2799

	ULLONG      guoD3UnitTotalReadCnt;  // 2800-2807
	ULLONG      guoD1UnitTotalReadCnt;  // 2808-2815
	U32		gulTableUNCResetCount;//2816-2819
	UWORD   guwForceRecordReadCountTime; // 2820-2821
	UBYTE   gubMarkLaterAsEarlyBadCount; // 2822-2822
	UBYTE   gubTimeoutLogPTR; // 2823-2823
	U32		gulVTOperationTime;//2824-2827
	UBYTE	gubSwapRUTMode; // 2828
	UBYTE	gubSwapRUTCopyUnitMode; // 2829
	FTarget_t	guwSwapRUTECCRUTTarget; //2830-2831
	FTarget_t	guwSwapRUTECCTarget; // 2832-2833
	FTarget_t	guwReadMoveD1Target; // 2834-2835
	U32		gulCOMResetCnt; //2836-2839
	UBYTE	gubFlashTypeFail; // 2840
	UBYTE	gubCountDDRINT; // 2841
	UWORD	guwTempNACKTimeoutCount; // 2842-2843
	MarkBadLog_t guoLastMarkBadLog;  // 2844-2847
	U32		gulTrimLCADoneCnt;	// 2848-2851
	U32		gulDebugInfo[19]; // 2852-2927
#if ENABLE_TABLE_VERIFY
	U32		gulNoUseMiddle4B_2[18]; // 2928-2999
	U32		gulProgramMismatchCnt;//3000-3003
	UWORD	guwTableProgramE3DCnt;//3004-3005
	UWORD	guwNoUseMiddle2B[1];//3006-3007
	UWORD   guwVerifyFailCNT;//3008-3009
	UWORD	guwVerifyPTRNoMatchCnt;//3010-3011
	UWORD	guwTableVerifyFailCnt;//3012-3013
	UWORD	guwVTVerifyFailCnt;//3014-3015
#else
	U32		gulNoUseMiddle4B_2[22]; // 2928-3015
#endif
	U32		gulMPRWCE_FailMap;//3016-3019
	U32		gulAbortDoneAfterReBuildFQ_DebugCnt;		// 3020-3023
	U32		gulTimeoutHandle_ResetFIPButTwoChannelAllTimeout_DebugCnt;	// 3024-3027
	UBYTE	gubReadDQSREMismatchCEMapCnt[16];	// 3028-3043
	U32		gulTimeoutHandle_RetryNonUncChannelAutoPollTimeout_DebugCnt;		// 3044-3047
	U32		gulTimeoutHandle_CheckCmdStatus_DebugCnt;		// 3048-3051
	U32		gulTimeoutHandle_Rebuild_DebugCnt;		// 3052-3055
	U32		gulFPUTimeoutHandleDebugCnt;				// 3056-3059
	U32		gulRetryFPUTimeoutDebugCnt;				// 3060-3063
	U32		gulReadFailRetryRSFailFPUCORagain_Pass_DebugCnt; // 3064-3067
	U32		gulReadFailRetryRSFailFPUCORagain_Fail_DebugCnt; // 3068-3071
	U32		gulReadFailRetryRSFailFPUCORagain_DebugCnt;	//3072-3075
	U32		gulCopyBackProgramFail_DebugCount; //3076-3079
	FTarget_t   guwOpenGRTarget;	//3080-3081                 /* VB w/o p2l, its p2l store in init info. */
	UWORD       guwInitInfoOpenGRP2LPtr;	//3082-3083	        /* Plane pointer of p2l store in init info. */
	UWORD          guwDebugActiveGR;  					//3084-3085
	UWORD          guwRetetionReadTarget; 			//3086-3087
	U32                gulRetetionReadFEntry; 			//3088-3091
	U32		gulLCA_Retry_FailDebugCount;		//3092-3095
	U32		gulLCA_Retry_PassDebugCount;		//3096-3099
	U32		gul00_30ReadAutoPolTimeoutReach5LimitCnt;		//3100-3103
	U32		gul00_30ReadAutoPolTimeoutCnt;		//3104-3107
	UBYTE	gubNewWEDriving;	//3108
	UBYTE	gubOldWEDriving;	//3109
	UBYTE	gubChangeWETime;	//3110
	UBYTE	gubSetRSEL;		//3111
	U32 		gulMTFPUPolTimeoutCnt; 				   //3112-3115
	U32                  gulDmaWriteLack8BytesDebugCnt; //3116-3119
	U32 gulSLCReadRetryCnt;	// 3120-3123
	U32	gulForceFlushD1Cnt;	//3124-3127
	U32	gulWrongUnit;		//3128-3131
	U32	gulRSNormalModeCorrectDataBeforeDebugCnt; //3132-3135
	U32 gulAutoPolTimeoutCnt; 				   //3136-3139	// autopol timeout handling ++
	//===================================================
	U32	gulReadMoveRetryCnt;                   //3140-3143  //SwapRUT CopyUnit Error Handle Retry Count
	U32	gulCopyD3UnitStTRetryCnt;              //3144-3147  //SwapRUT Copy D3 Unit Source To Target Error Handle Retry Count
	U32	gulFlushD1RetryCnt;                    //3148-3151  //FlushD1 Error Handle Retry Count
	U32	gulReadRSCnt_D1;                         //3152-3155
	U32	gulReadRSCnt_D3;	                 //3156-3159
	U32	gulReadUNCCnt_D1;                        //3160-3163
	U32	gulReadUNCCnt_D3;                        //3164-3167
	U32	gulProgramFailCnt_D1;                    //3168-3171
	U32	gulProgramFailCnt_D3;                    //3172-3175
	//===================================================
	U32		gulRSNormalModeCorrectCount;//3176-3179
	U32		gulRSNormalModeRecoverFailCnt; //3180-3183
	U32		gulRSNormalModeRecoverConvergeCnt; //3184-3187
	U32		gulRSNormalModeRecoverParityPageDebugCnt; //3188-3191
	U32		gulRSNormalModeNormalReadAbnormalPassCount; //3192-3195
	U32		gulRSNormalModeHBSB_PartialCorrectCount; //3196-3199

	U32		gulCodeBlkReplaceCnt; //3200-3203
	U32		gulCodeBlkReplaceFailCnt; //3204-3207
	UWORD                 guwEarlyBadCntPerPlane[256]; //3208-3719 //MaxDie*MaxPlane
	U32                  gulHostReadE3DCnt; //3720-3723
	U32                       gulDoneTagMiss; //3724-3727
	U32                       gulSynEscape; //3728-3731
	U32                         gulNoComeERR; //3732-3735
	U32                  gulReadMoveCnt; //3736-3739
	U32                  gulMarkBadCnt; //3740-3743
	U32                  gulReadDQSREMismatchCnt; //3744-3747
	U32                       gulReadCRCCnt; //3748-3751
	U32                       gulWriteCRCCnt; //3752-3755
	U32                       gulHostReadECCCnt; //3756-3759
	//===========================================
	UBYTE             gubReserveUnitsPerGroup[16]; //3760-3775 //MaxDie * MaxInterleave
	UWORD           guwUnitsPerGroups[16]; //3776-3807
	UWORD           guwExtendUnitsIndex[16]; //3808-3839
	U32                  gulReadCRC32ErrorCnt; //3840-3843
	U32                  gulReadLCAErrorCnt; //3844-3847
	UWORD          guwFWUpdateCount; //3848-3849
	UWORD          guwSystemUpdateCount; //3850-3851
	U32                         gulPECount; //3852-3855
	U32                  gulRSRecoverFailCnt; //3856-3859
	U32                    gulFPUCorrectFailCnt; //3860-3863
	U32                  gulReadErasePageErrorCnt; //3864-3867
	U32                  gulReadRetryErasePageRSFailCnt; //3868-3871
	U32                  gulReadFWSettingErrorCnt; //3872-3875
	U32                  gulReadRetryFPUCORUserDataSignOffFail; //3876-3879
	U32                  gulReadRetryEnterRSUserDataSignOffFail; //3880-3883
	U32                  gulReadRetryFPUCORSignOffFail; //3884-3887
	U32                  gulReadRetryEnterRSSignOffFail; //3888-3891
	U32                  gulSignOffFail; //3892-3895
	U32                  gulProgramCompressionFailCnt; //3896-3899
	U32                  gulProgramFWSettingErrorCnt; //3900-3903
	U32                       gulPreviousFQCacheProgramFailDebugCnt; //3904-3907
	U32                  gulReadFailNotRetryJobDebugCnt; //3908-3911
	U32		gulErrorDumpPC;//3912-3915
	U32		gulNonDataUNCCnt;//3916-3919
	U32                  gulProgramE3DErrorDebugCnt; //3920-3923
	U32                  gulCRC16ErrorDebugCnt; //3924-3927
	U32                  gulDeCompressionErrorDebugCnt; //3928-3931
	U32                  gulFIPNoHangDebugCnt; //3932-3935
	U32                      gulNonDataCRCErrCnt; //3936-3939

	UBYTE	gubInto_NMI_ISR_Count;//3940
	UBYTE          gubDebugUncPtr;//3941
	UBYTE          gubD1UnitNum; //3942 // TLC FW
	UBYTE           	gubSaveVTSerialNumber;//3943

	U32		gulTotal_Erase_Count_D1;  //3944-3947 // TLC FW
	U32           	gulLastPowerCycling;//3948-3951
	ULLONG	guoUnitTimeLine; //3952-3959		// 8 Byte align
	ULLONG	guoUnitTimeLineGCGR;//3960-3967
	U32                       gulMaxInitTime; //3968-3971
	U32                  gulPESYSINTSTS3; //3972-3975
	U32		gulRRAMPECount;//3976-3979
	U32		gulSATAPECount;//3980-3983
	U32		gulDBUF0PECount;//3984-3987
	U32                  gulDBUF1PECount; //3988-3991
	U32                  gulDBUF2PECount; //3992-3995
	U32                  gulDBUF3PECount; //3996-3999
	U32                  gulNMBUFPECount; //4000-4003
	U32                  gulPBUF0PECount; //4004-4007
	U32                  gulPBUF1PECount; //4008-4011
	U32                  gulPBUF2PECount; //4012-4015
	U32                  gulPBUF3PECount; //4016-4019
	U32                  gulBRAMPECount; //4020-4023
	U32                  gulIBFPECount; //4024-4027
	U32                  gulENCSRAMPECount; //4028-4031
	U32                  gulZIPPECount; //4032-4035
	U32                  gulRWTableUnitRetryFailEnterRS; //4036-4039
	U32                  gulTotalReadUNCCnt; //4040-4043
	UWORD	guwPutUserFreeToFailHandleAreaCnt[16];       //4044-4075
	UBYTE	gubWriteProtectState;                                   // 4076
	UBYTE	gubBiCS2HBSFFail;                                                  // 4077
	FTarget_t     guwOldGRTarget; //4078-4079
	U32             gulNonStandByPowerDownCnt; //4080-4083
	U32             gul1ZnmSpecialCaseRetryCnt; //4084-4087
	U32             gulTableUNCBitMap; //4088-4091
	U32           	gulVTLastMark; //4092-4095
} Variable_Table;

// byte 0~11 不能變動位置...
typedef struct {
	UBYTE ubID;  // 0
	UBYTE ubFlag[2];  // 1-2
	UBYTE ubValue; // 3
	UBYTE ubWorst; // 4
	UBYTE ubRaw[6]; // 5-10
	UBYTE ubReserved ; // 11
	UBYTE ubThreshold; // 12
} SMART_Attribute_Entry;

// byte 0~9 不能變動位置...
typedef struct {
	UBYTE ubID;  // 0
	UBYTE ubThreshold; // 1
	UBYTE ubFlag[2]; //2-3
	UWORD uwValue; //4-5
	UWORD uwWorst; //6-7
	UWORD uwData; //8-9
	UBYTE ubReserved[6];//10-15
} MP_SMART_Attribute_Entry;

// info block array
typedef struct {
	UBYTE u8bPhIsOnInfoMark[6];           //0-5
	UBYTE ubReserved1;                    //6
	UBYTE ubSetFlashDrive;                     //7
	UBYTE ubDrivingEnable;              //8
	UBYTE ubALEPads;              //9
	UBYTE ubCLEPads;              //10
	UBYTE ubnWEPads;            //11
	UBYTE ubnREPads;             //12
	UBYTE ubFRDYPads;                 //13
	UBYTE ubnWPPads;                //14
	UBYTE ubDQSPads;                  //15
	UBYTE ubDataPads;                //16
	UBYTE ubnCEPads;                    //17
	UBYTE ubSerialNumber[20];     //18-37
	UBYTE ubModelNumber[40];      //38-77
	UBYTE ubFWVersionEnable;      //78
	UBYTE ubFWVersion[8];    //79-86
	UBYTE ubCustomer;       //87
	UBYTE ubNoUse;                  //88  //dont need ubWWNSupport now [Sam]
	UBYTE ubCENumber;                              //89
	UWORD uwWWN[4];                               //90-97
	UBYTE ubFormFactor;                              //98
	UBYTE ubDisableEnableMicronSLCMode;     //99
	U32 ulSSDSize_LBA ;                              //100-103
	UBYTE ubILMode;                                   //104
	UBYTE ubDisableEnableMultiPlaneMode;     //105
	UBYTE ubDisableEnableA2Mode;           //106
	UBYTE ubFlashInterface;                        //107
	UBYTE ubFlashClk;                          //108
	UBYTE ubThermalSensorMode;//109
	UBYTE ubDEVSLPSupport;//110
	UBYTE ubNCQSupport;//111
	UBYTE ub48BitCmdSupport;//112
	UBYTE ubXferModeEnable;//113
	UBYTE ubMultiDMAMode[2];//114-115
	UBYTE ubUltraDMAMode[2];//116-117
	UBYTE ubUltraMLCEnable;//118
	UBYTE ubWriteCacheSupport;//119
	U32 ulMaxPECycle_D3;//120-123
	UBYTE ubPMICSel[12];//124-135
	U32 ulSLCPoolSizeIn4KEntry;//136-139
	UWORD uwIDW129_159[31]; //140-201
	UBYTE ubIDW129_159Enable;//202
	UBYTE ubSLCPoolEnable;//203
	UBYTE ubSSC_Enable;//204
	UBYTE ubHIPMSupport;               // 205
	UBYTE ubDIPMSupport;		//206
	UBYTE ubAPSSupport;		//207
	U32 ulSmartStatusRuleMap;//208-211
	U32 ulTimeToDoBG_Clean;                     // 212-215  (unit:10ms)
	U32 ulTimeToDoBG_Copy;                     // 216-219  (unit:10ms)
	UBYTE ubTimeToDoBGEnable;               // 220
	UBYTE ubSwiftFlushEnable;		//221
	UWORD uwPMASetting;		//222-223
	UBYTE ubNoPWICEnable;		//224
	UBYTE ubBGMLCCopyThreshold;		//225
	UBYTE ubBGMLCCopyReleaseThreshold;//226
	UBYTE ubSetBGThreshouldEnable;	//227
	U32 ulBGSLCPoolCopyThreshold;//228~231
	U32 ulBGSLCPoolReleaseThreshold;//232~235
	UBYTE ubSpecialDataEnable;		//236
	UBYTE ubPadODTEnable;				//237
	UBYTE ubPadODT;					//238
	UBYTE ubSlewRateEnable;			//239
	U32 ulTotalUserUnitNumber;			//240-243
	UBYTE ubSlewRate;				//244
	UBYTE ubDDRDriveEnable;			//245
	UBYTE ubDDRDrive;				//246
	UBYTE ubHBRetryEnable;			//247
	UWORD uwDIPM_AUTOP2S_THRESHOLD;				//248-249
	UBYTE ubDIPM_THRESHOLD ;		//250
	UBYTE ubDIPM_P2S_THRESHOLD ;		//251
	UBYTE ubSetEnterBG_Time_Enable;         //252
	UBYTE ubDCOSupport; //253
	UBYTE ubHPASupport; //254
	UBYTE ubAMAXSupport; //255
	U32 ulMaxPECycle_D1;//256-259
	UBYTE ubSanitizeSupport; //260
	UBYTE ubWUNCSupport; //261
	UBYTE ubZeroSupport; //262
	UBYTE ubSetFlashODT;//263
	UBYTE ubGuaranteeFlushEnable;//264
	UBYTE ubENABLE_FW_DIPM;//265
	UBYTE ubENABLE_HW_DIPM;//266
	UBYTE ubAP2SWaitIdle2Time;//267
	UWORD uwDataInOutThreshold; //268-269
	UBYTE ubTSB_SerialNumber_Customer[20]; //270-289
	UWORD uwSLCPool_PE_Threshold; //290-291
	UBYTE ubSecuritySupport; //292
	UBYTE ubDLMCSupport; //293
	UBYTE ubTrimSupport; //294
	UBYTE ubDLMCReturnDummyAbort; //295
	UWORD uwWearlevelingRange_D1;//296-297
	UWORD uwWearlevelingRange_D3;//298-299
	UBYTE ubRSEL_Enable;//300
	UBYTE ubCustomersCustomerID;//301
	UBYTE ubKeepSMARTwhenKeepEC;//302
	UBYTE ubWLCheckRate;//303
	UBYTE ub_ENABLE_DPS; //304
	UBYTE ubDPSEnableUseTimeOut; //305
	UWORD uwDPSWaitTimeOutMiliSec; //306-307
	UWORD uwReadSDLLOffsetCH0;//308-309
	UWORD uwReadSDLLOffsetCH1;//310-311
	UWORD uwProgramSDLLOffsetCH0;//312-313
	UWORD uwProgramSDLLOffsetCH1;//314-315
	UBYTE ubFlashInterface_Scan;//316
	UBYTE ubFlashClock_Scan;//317
	UBYTE ubACTimingTableEnable;//318
	UBYTE ubACTimingClock;//319
	UBYTE ubACTimingTable[32];//320-351
	UBYTE ubSMARTExecuteOfflineEnable;//352
	UBYTE ubWriteProtectWriteSupport;//353
	UBYTE ubCustomerGPIOSelectMode;//354
	UBYTE ubSMARTExtendScanAllEnable;//355
	UBYTE ubComWakeWorkaround; // 356
	UBYTE ubWPVersionEnable; //357
	UWORD uwWPVerTable[4]; //358-365
	UWORD uwWPModelTable[10]; //366-385
	UBYTE ubREBDuty[2];      //386-387
	UBYTE ubFlushD1VerifyMode;//388
	UBYTE ubUnitReadCheckThreshold;//389
	UBYTE ubUnitForceReadMoveThreshold;//390
	UBYTE ubSMARTBufferTimeSecond;//391
	UBYTE ubSMARTShortSelfTestTimeMin;//392
	UBYTE ubSMARTConveyanceSelfTestTimeMin;//393
	UWORD uwSMARTExtendedSelfTestTimeMin;//394-395
	UWORD uwSMARTOfflineTimeSecond;//396-397
	UBYTE ubSecurityEraseNormalNeedDoRealErase; //398
	UBYTE ubGPIOEdgeMode;//399
	UBYTE ubforceactiveGRSLC;//400
	UBYTE ubRecordEraseFail_EarlyOrLater;//401
	UBYTE ubCustomerGPIO3SelectMode;//402
	UBYTE ubGPIO3EdgeMode;//403
	UBYTE ubVenderLock;//404
	UBYTE ubHighTempConfigDefault ;//405
	UBYTE ubHVFlash_RDTSetting; //406, Bit_0=1:勾HV Flash, Bit_1=1:Translate IB to DBT, Bit_2=1:On Board Sorting Before RDT
	UBYTE ubEraseMode_RDTSetting; //407
	UBYTE ubHVFlash_FWSetting; //408, Bit_0=1:勾HV Flash, Bit_1=1:Translate IB to DBT, Bit_2=1:On Board Sorting Before RDT
	UBYTE ubEraseMode_FWSetting; //409
	UBYTE ubRMApreformat;//410
	UBYTE ubEnable_Special_Data_Workaround;//411
	UBYTE ubEnableMilitaryErase;//412
	UBYTE ubRUTMixPlaneEnable;//413
	UBYTE ubBlocked_CannotUse;//414, FSP take this bit for enable D1
	UBYTE ubEnable_CTRL_INT_For_CBitZeroCase;//415
	UBYTE ubNotUseReserve_1[1];//416 
	UBYTE ubSystemAreaRule_ByAP;//417, 0:FW decides, 1:AP decides Old PlaneA Rule, 2: AP decides New Rule
	UBYTE ubHV_Retention; //418
	UBYTE ubB16Write_Erase_Delay; //419
	UBYTE ubEnableTemperatureControl; //420
	UBYTE ubEnableCFastIdentifyDeviceWord0Request; //421
	UBYTE ubEnableOverClocking; //422
	UBYTE ubNoUse_1;
	UBYTE ubCFastHighTemprature; //424
	UBYTE ubCFastLowTemprature; //425
	UBYTE ubFakeTemperatureValue; //426
	UBYTE ubSupportSpuriousSignal; //427
	UBYTE ubPreformatNoLaterBad; //428
	UBYTE ubDisableFWCacheCmd;		//429
	UBYTE gubMarkLaterAsEarlyBadCountMax; // 430
	UBYTE ubNandTypeBiCS3;        //431
	U32   ulCEValidMap; // 432-435
	UBYTE ubDropSpeedTemperature[2]; //436-437
	UBYTE ubDelayCnt[2]; //438-439
	UWORD guwNCQTimeoutThreshold;	//440-441
	UWORD guwATATimeoutThreshold;	//442-443
	UBYTE gubDisableStatusFail;	//444
	UBYTE gubForceEnterprise;		//445
	UBYTE ubQuickEraseWaitTime; //446
	UBYTE ubNotUseReserve_3;        //447
	UBYTE ubTemperatureFailValue;	//448
	UBYTE ubSupportSATA;	//449
	UBYTE ubNotUseReserve_4[10];        //450-459
	UBYTE ubAPUseArray1[50];     //460-509
	UBYTE ubIndustrialSpec;		//510
	UBYTE ubAPUseArray2[1];     //511

} SYSTEM_GROUP_t;

// info block array
typedef struct {
	U32 ulReserve[5];		//0~19
	U32 ulTotalEC_D1;		//20~23
	U32 ulMaxEC_D1;		//24~27
	U32 ulAvgEC_D1;		//28~31
	U32 ulTotalEC_D3;		//32~35
	U32 ulMaxEC_D3;		//36~39
	U32 ulAvgEC_D3;		//40~43
	U32 ulHostUNC;			//44~47
	U32 ulEraseFail;			//48~51
	U32 ulProgeramFail;		//52~55
	U32 ulReadFail;			//56~59
	U32 ulEarlyBad;			//60~63
	U32 ulLaterBad;			//64~67
	U32 ulTotalPowerOn_Hour;	//68~71
	U32 ulPowerOnCnt;		//72~75
	U32 ulPowerCycleCnt;		//76~79
	U32 ulTotalWrite_1G_nand;	//80~83
	U32 ulTotalRead_1G_nand;	//84~87
	U32 ulTotalWrite_1G;		//88~91
	U32 ulTotalRead_1G;		//92~95
	U32 ulFWUpdate;			//96~99
	U32 ulWriteCRC;			//100~103
	U32 ulReadCRC;			//104~107
	UBYTE ubTableCheck;		//108
	UBYTE ubSaveVTSerialNumber;//109
} RMA_Info_t;

typedef struct {
	UBYTE ubDrivingEnable;
	UBYTE ubALEPads;
	UBYTE ubCLEPads;
	UBYTE ubnWEPads;
	UBYTE ubnREPads;
	UBYTE ubFRDYPads;
	UBYTE ubnWPPads;
	UBYTE ubDQSPads;
	UBYTE ubDataPads;
	UBYTE ubnCEPads;
} DRIVING_SETTING;

typedef struct {
	UBYTE ubODTEnable;
	UBYTE ubODT;
} ODT_SETTING;

typedef struct {
	UWORD uwReadSDLLOffsetCH0;
	UWORD uwReadSDLLOffsetCH1;
	UWORD uwProgramSDLLOffsetCH0;
	UWORD uwProgramSDLLOffsetCH1;
} SDLL_DELAY_SETTING;

typedef struct {
	UBYTE ubDMASelectMode[2];
	UBYTE ubDMASupportMode[2];
	UBYTE ubDMASupportMAP[2];
	UBYTE ubDefaultDMAType;   // 1:MDMA  0:UDMA
} XFERMODE;

typedef struct {
	U32 ulFirstMark;							// 4B
	//UBYTE ubSysCodeUnit[2];					// 4B
	UBYTE ubHostCmdIn;
	UBYTE ubSATACmdCode;
	UBYTE ubDeferredCodeSet[2];

	UWORD uwSysCodeBlock[2][MAX_CHANNEL];

	UWORD uwDBTBlock[MAX_CHANNEL];

	UWORD uwSystemBlock[MAX_CHANNEL];

	UBYTE ubSysCodeSet[2];
	UBYTE ubDBTSet;
	UBYTE ubSystemSet;

	UBYTE ubSystemSetFreeCount;
	UBYTE ubSystemSetFreeQueueOutPTR;
	UBYTE ubSystemSetFreeQueueInPTR;
	UBYTE ubVTIndex;

	UBYTE ubPowerMode;
	UBYTE ubDEVSLPSupport;
	UBYTE ubDoPreformatAfterDLMC;
	UBYTE ubSATAPhyStatus;

	UWORD uwVT_PTR;						//2B
	UWORD uwVTChild_PTR;				//2B

	UBYTE ubSystemSetFreeTable[SYSTEM_SETS];	// 8B

	XFERMODE uoXferMode;					// 8B

	UBYTE ubRTD_Enable;
	UBYTE ubDLMCStep;						// 4B
	UBYTE ubLastDLMCMode;
	UBYTE ubHeaderTransferSectorCnt;

	U32 ulBurnerTransferSectorCnt;				// 8B
	U32 ulMicroCodeTransferSectorCnt;


	UBYTE ubACTPMValue;
	UBYTE ubACTPMEnable;

	U32 ulLastMark;							// 4B
} DEVSLP_Table;

typedef union {			//maybe change to 4 bytes
	U32 All;
	struct {
		U32 ubNVDDRTimingMode: 8;  // bit 0-7, B141
		U32 ubNVDDR2TimingMode: 8; // bit 8-15  , B142
		U32 btNotSupport: 1; // bit 16,
		U32 btNandSupplyByEmulator: 1; // bit 17
		U32 btNandEmulatorNoSLCPool: 1; // bit 18
		U32 btNVDDR: 1; // bit19,  B6_b5
		U32 btNVDDR2: 1; // bit20  , B6_b10
		U32 btNVDDR2TimingMode8: 1;	// bit 21
		U32 btNouse: 2; // bit 22-23
		U32 ubAsynchronousTimingMode: 8; // bit 24-31, B129
	} ONFI;

	struct {
		U32 uwToggleDDRSpeedGrade: 16; // bit0-15
		U32 btNotSupport: 1; // bit 16
		U32 btNandSupplyByEmulator: 1; // bit 17
		U32 btNandEmulatorNoSLCPool: 1; // bit 18
		U32 btNoUse: 13; // bit19-bit31
	} JEDEC;
} FLASH_PARAMETER_PAGE;

typedef struct {
	UWORD uwMaxEarlyBadCntPerPlane;
	UWORD uwMaxLaterBadCntPerPlane;
	UWORD uwMaxEarlyLaterBadCntPerPlane;

	UWORD uwMaxEarlyBadCntPerCE;
	UWORD uwMaxLaterBadCntPerCE;
	UWORD uwMaxEarlyLaterBadCntPerCE;

	U32 ulTotalLaterBadCnt;
	U32 ulTotalEarlyBadCnt;
	U32 ulTotalEarlyLaterBadCnt;

	UBYTE ubMaxEarlyBadCntPerPlane_Index;
	UBYTE ubMaxLaterBadCntPerPlane_Index;
	UBYTE ubMaxEarlyLaterBadCntPerPlane_Index;

	UBYTE ubMaxEarlyBadCntPerCE_Index;
	UBYTE ubMaxLaterBadCntPerCE_Index;
	UBYTE ubMaxEarlyLaterBadCntPerCE_Index;

	UBYTE ubMaxLaterBadRatePerPlane;
	UBYTE ubMaxLaterBadRatePerPlane_Index;

} SMART_BADBLOCK_INFO;

typedef struct {
	U32 ulMaxEraseCnt ;
	U32 ulMinEraseCnt;
	U32 ulTotalEraseCnt;
	U32 ulAvgEraseCnt;
	U32 ulWorstAvgEraseCnt;

	UWORD uwMaxEraseCnt_Index;
	UWORD uwMinEraseCnt_Index;
} SMART_ERASECOUNT_INFO_STRUCT;

typedef struct {
	SMART_ERASECOUNT_INFO_STRUCT TypeD1;
	SMART_ERASECOUNT_INFO_STRUCT TypeD3;
} SMART_ERASECOUNT_INFO;


typedef struct {
	UWORD uwMaxReadFailCntPerPlane;
	UWORD uwMaxProgramFailCntPerPlane;
	UWORD uwMaxEraseFailCntPerPlane;
	U32 ulTotalReadFailCnt;
	U32 ulTotalProgramFailCnt;
	U32 ulTotalEraseFailCnt;
	U32 ulTotalRSRecoveryMarkBadCnt;

	UBYTE ubMaxReadFailCntPerPlane_Index;
	UBYTE ubMaxProgramFailCntPerPlane_Index;
	UBYTE ubMaxEraseFailCntPerPlane_Index;
} SMART_FAILCOUNT_INFO;

typedef union {			//maybe change to 4 bytes
	UWORD All;
	struct {
		UWORD uwHostTableIndex: 10;
		UWORD ubJob: 6;
	} B;
} HostTable_t;

typedef struct {
	UBYTE ubPageSignature[4]; // 0-3
	UBYTE ubRevisionNumber[2]; // 4-5
	UBYTE ubReserved0[26]; //6-31
	UBYTE ubDeviceManufacture[12]; // 32-46
	UBYTE ubDeviceModel[20]; // 44-63
	UBYTE ubJEDECManufactureID[6] ; // 64-69
	UBYTE ubReserved1[10]; // 70-79
	UBYTE ubDataBytesPerPage[4]; // 80-83
	UBYTE ubSpareBytesPerPage[2]; // 84-85
	UBYTE ubReserved2[6]; // 86-91
	UBYTE ubPagesPerBlock[4]; // 92-95
	UBYTE ubBlocksPerLogicalUnit[4]; //96-99
	UBYTE ubLogicalUnits; // 100
	UBYTE ubAddressCycles;// 101
	UBYTE ubBitsPerCell; // 102
	UBYTE ubReserved3; // 103
	UBYTE ubMultiplePlaneAddressing; // 104
	UBYTE ubReserved4[39]; // 104-143
	UBYTE ubReserved5[2]; // 144-145
	UBYTE ubToggleDDRSpeedGrade[2]; //146-147
	UBYTE ubReserved6[21]; // 148-168
	UBYTE ubDriverStrengthSupport; // 169
	UBYTE ubReserved7[38]; // 170-207
	UBYTE ubReserved8[212] ; // 208-419
	UBYTE ubVendorSpecific[92]; // 420-511
} FLASH_IDENTIFICATION_TABLE;

typedef struct {
	UBYTE	KeyHeader[10];
	WORD		KeyInfoVersion;
	UWORD	KeyIndex;
	UBYTE    Reserve[18];
	U32		Key[8];
} SecureKeyInfo_t;

typedef struct {
	U32 DynamicSessionKey[8];
} SecureSession_t;

#if PH_SQL_FUNC
typedef union {
	UBYTE All;
	struct {
		UBYTE btAdrTokenCnt: 2;
		UBYTE btIsEnableECC: 1;
		UBYTE btIsInversion: 1;
		UBYTE btIsConversion: 1;
		UBYTE btIsCheckSpare: 1;
		UBYTE btIsShowECC: 1;
		UBYTE btIsWrite: 1;
	} B;
} SQLHeadPara;
#endif

typedef union {
	UBYTE All;
	struct {
		UBYTE ubType: 4;
		UBYTE btNoRevision: 1;
		UBYTE btRevision: 1;
		UBYTE btRelaxMode: 1;
		UBYTE btMPSet: 1;
	} B;
} RRT_SETTING;

/* RS Related */
typedef struct {
	UBYTE ubTagMAP[VAR_RS_GROUP_NUM][RS_CHANGE_TAG_COUNT_PER_GROUP];
	U32 ulLastPlaneIndex[VAR_RS_GROUP_NUM];
	U32 ulLastPlaneIndexFastPage[VAR_RS_GROUP_NUM];
	U32 ulLastDataPlaneIndex[VAR_RS_GROUP_NUM];
	U32 ulLastDataPlaneIndexFastPage[VAR_RS_GROUP_NUM];
	UBYTE ubPagesPerGroup;
	/*
	  * 某些case下，unit結尾的那幾組RS Encode group會發生下列現象,
	  * A group 比 B group 早開始 encode ,但比B group 晚補parity
	  * 因此分開記最早開始跟最早結束的group是誰, 不記也可以, 但可能會多檢查幾次
	  */
	UBYTE ub1stStartLastGroup;
	UBYTE ub1stStartLastGroupFastPage;
	UBYTE ub1stEndLastGroup;
	UBYTE ub1stEndLastGroupFastPage;
	UBYTE ubIsParity;
	UWORD uwPlanesPerGroup;
	UWORD uwPlaneOffset;
	UBYTE ubGroupIndex;
	UWORD uwPageIndex;
	UBYTE ubTagIndex;
	UBYTE ubPlanesPerP2LRS;
	UBYTE ubPlanesPerP2LRSCopy;
	UBYTE ubPlanesPerP2LRSCopyUnit;
	UBYTE ubDummyPlanes;

} RS_PARAM;

typedef struct {
	U32 ulPageLinkList[VAR_RS_GROUP_NUM][RS_MAX_LINK_COUNT_PER_GROUP];
	UWORD uwPlaneOffset;
	UBYTE ubGroupIndex;
	UWORD uwPageIndex;
	UBYTE ubTagIndex;
} RS_FLAFAIL_PARAM;

#if EnableProductHistory
typedef struct {
	UWORD uwUnit;
	UBYTE ubPlanebank;
	UWORD uwPagePTR;
} PHstruct;
#endif




#if 1 || (!BURNER)
typedef struct {

	// ===== Polling =====
	UWORD FPU_C70_DAT_MK40_MF0F[8];
	UWORD FPU_C70_DAT_MK20_MF0F[8];
	UWORD FPU_C70_DAT_MK01[8];
	UWORD FPU_C70_DAT_MK04[8];
	UWORD FPU_C70_DAT_MK03[8];
	UWORD FPU_C70_DAT_MK0C[8];


	// Base

	// ===== Write =====
	// =====D3 Write (2P)=====
	// ===== First =====
	UWORD FPU_C09_C01_AG_C85_A5_DW[8];
	UWORD FPU_C11_POL_MK40_C09_C01_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	UWORD FPU_C09_C02_AG_C85_A5_DW[8];
	UWORD FPU_C11_POL_MK40_C09_C02_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	UWORD FPU_C09_C03_AG_C85_A5_DW[8];
	UWORD FPU_C11_POL_MK40_C09_C03_AG_C85_A5_DW_C15[13];//帶Auto Poll
	UWORD FPU_C11_POL_MK40_C09_C03_AG_C85_A5_DW_C10[13];//帶Auto Poll

	// ===== Foggy =====
	UWORD FPU_C0D_C01_AG_C85_A5_DW[8];
	UWORD FPU_C11_POL_MK40_C0D_C01_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	UWORD FPU_C0D_C02_AG_C85_A5_DW[8];
	UWORD FPU_C11_POL_MK40_C0D_C02_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	UWORD FPU_C0D_C03_AG_C85_A5_DW[8];
	UWORD FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_DW_C15[13];//帶Auto Poll
	UWORD FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_DW_C10[13];//帶Auto Poll

	// ===== Fine =====
	UWORD FPU_NOP_C01_AG_C85_A5_DW[8];
	UWORD FPU_C11_POL_MK40_NOP_C01_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	UWORD FPU_NOP_C02_AG_C85_A5_DW[8];
	UWORD FPU_C11_POL_MK40_NOP_C02_AG_C85_A5_DW_C1A[13];//帶Auto Poll
	UWORD FPU_NOP_C03_AG_C85_A5_DW[8];
	UWORD FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_DW_C15[13];//帶Auto Poll
	UWORD FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_DW_C10[13];//帶Auto Poll

	// ===== Write =====
	// =====D3 Write (1P)=====
	// ===== First =====
	//DMA
	UWORD FPU_C09_C01_AG_C85_A5_DW_C1A[9];
	UWORD FPU_C09_C02_AG_C85_A5_DW_C1A[9];
	UWORD FPU_C09_C03_AG_C85_A5_DW_C15[9];
	UWORD FPU_C09_C03_AG_C85_A5_DW_C10[9];

	// ===== Foggy =====
	UWORD FPU_C0D_C01_AG_C85_A5_DW_C1A[9];
	UWORD FPU_C0D_C02_AG_C85_A5_DW_C1A[9];
	UWORD FPU_C0D_C03_AG_C85_A5_DW_C15[9];
	UWORD FPU_C0D_C03_AG_C85_A5_DW_C10[9];

	// ===== Fine =====
	UWORD FPU_NOP_C01_AG_C85_A5_DW_C1A[9];
	UWORD FPU_NOP_C02_AG_C85_A5_DW_C1A[9];
	UWORD FPU_NOP_C03_AG_C85_A5_DW_C15[9];
	UWORD FPU_NOP_C03_AG_C85_A5_DW_C10[9];

	//Non-DMA
	// ===== First =====
	UWORD FPU_C09_C01_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_C09_C01_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	UWORD FPU_C09_C02_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_C09_C02_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	UWORD FPU_C09_C03_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_C09_C03_AG_C85_A5_NOP_C15[12];//帶Auto Poll
	UWORD FPU_C11_POL_MK40_C09_C03_AG_C85_A5_NOP_C10[12];//帶Auto Poll

	// ===== Foggy =====
	UWORD FPU_C0D_C01_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_C0D_C01_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	UWORD FPU_C0D_C02_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_C0D_C02_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	UWORD FPU_C0D_C03_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_NOP_C15[12];//帶Auto Poll
	UWORD FPU_C11_POL_MK40_C0D_C03_AG_C85_A5_NOP_C10[12];//帶Auto Poll

	// ===== Fine =====
	UWORD FPU_NOP_C01_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_NOP_C01_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	UWORD FPU_NOP_C02_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_NOP_C02_AG_C85_A5_NOP_C1A[12];//帶Auto Poll
	UWORD FPU_NOP_C03_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_NOP_C15[12];//帶Auto Poll
	UWORD FPU_C11_POL_MK40_NOP_C03_AG_C85_A5_NOP_C10[12];//帶Auto Poll
	UWORD FPU_CA2_C00_A5_C30_C70_POL_MK20_C05_A5_E0_DMARaw[2];
	UWORD FPU_C00_A5_C30_C70_POL_MK20_C05_A5_E0_DMARaw[16];
#if YMTC
	UWORD FPU_CDF_AG_C80_A6_DW[16];
	UWORD FPU_CDF_AG_C85_A6_NOP[16];

	UWORD FPU_C11_POL_MK40_AG_C80_A6_DW_C1A[16];
	UWORD FPU_C11_POL_MK40_AG_C80_A6_DW_C10[16];

	UWORD FPU_C11_POL_MK40_AG_C85_A6_NOP_C1A[16];
	UWORD FPU_C11_POL_MK40_AG_C85_A6_NOP_C10[16];

	UWORD FPU_CDF_AG_C80_A5_DW_C1A[16];
	UWORD FPU_CDF_AG_C80_A5_DW_C10[16];
	UWORD FPU_AG_C00_A6_C05_A2_CE0[8];
	UWORD FPU_AG_C00_A6_C34[8];
	UWORD FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_AG_CA2_C00_A5_C35[20];
	UWORD FPUEND[1];
#elif Hynix
	// ===== Read =====
	UWORD FPU_AG_C05_A5_CE0_DR[7];
	UWORD FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C30[16];
	UWORD FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C30[16];
	UWORD FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C30[16];
	UWORD FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C31[14];
	UWORD FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C31[14];
	UWORD FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C31[14];
	UWORD FPU_C01_AG_C00_A5_C30[8];
	UWORD FPU_C02_AG_C00_A5_C30[8];
	UWORD FPU_C03_AG_C00_A5_C30[8];
	UWORD FPU_C01_AG_C00_A5_C31[6];
	UWORD FPU_C02_AG_C00_A5_C31[6];
	UWORD FPU_C03_AG_C00_A5_C31[6];
#if Hynix
	UWORD FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_AG_CA2_C00_A5_C35[15];
#endif

	// ===== Soft Bit ====
	UWORD FPU_AG_C05_A5_CE0[5];
	UWORD FPU_C01_AG_C00_A5_C3C[6];
	UWORD FPU_C02_AG_C00_A5_C3C[6];
	UWORD FPU_C03_AG_C00_A5_C3C[6];
	UWORD FPU_AG_C00_A5_CAE[5];
#if Hynix
	UWORD FPU_AG_C00_A5_C05_A2_CE0[7];
#endif
	// ===== Write =====
	UWORD FPU_AG_C11_C70_POL_MK40_C80_A5_DW_C15[11];
	UWORD FPU_AG_C11_C70_POL_MK40_C80_A5_DW[10];

#if (Hynix3DV6 || Hynix3DV5)
	UWORD 			 FPU_C01_AG_C80_A5_DW[7];
	UWORD FPU_C11_POL_MK40_C01_AG_C81_A5_DW[11];
	UWORD FPU_C11_POL_MK40_C01_AG_C81_A5_DW_C22[12];
	UWORD FPU_C01_AG_C80_A5_DW_C22[8];


	UWORD 			 FPU_C02_AG_C80_A5_DW[7];
	UWORD FPU_C11_POL_MK40_C02_AG_C81_A5_DW[11];
	UWORD FPU_C11_POL_MK40_C02_AG_C81_A5_DW_C22[12];
	UWORD FPU_C02_AG_C80_A5_DW_C22[8];


	UWORD 			 FPU_C03_AG_C80_A5_DW[7];
	UWORD FPU_C11_POL_MK40_C03_AG_C81_A5_DW[11];
	UWORD FPU_C11_POL_MK40_C03_AG_C81_A5_DW_C23[12];
	UWORD FPU_C03_AG_C80_A5_DW_C23[8];


	UWORD 			 FPU_C01_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_C01_AG_C85_A5_NOP[11];
	UWORD FPU_C11_POL_MK40_C01_AG_C85_A5_NOP_C22[12];

	UWORD 			 FPU_C02_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_C02_AG_C85_A5_NOP[11];
	UWORD FPU_C11_POL_MK40_C02_AG_C85_A5_NOP_C22[12];

	UWORD 			 FPU_C03_AG_C85_A5_NOP[7];
	UWORD FPU_C11_POL_MK40_C03_AG_C85_A5_NOP[11];
	UWORD FPU_C11_POL_MK40_C03_AG_C85_A5_NOP_C23[12];

	UWORD FPU_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C30[32];
	UWORD FPU_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C30[32];
	UWORD FPU_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C30[32];
	UWORD FPU_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C31[32];
	UWORD FPU_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C31[32];
	UWORD FPU_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C31[32];
	UWORD FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_NOP_AG_C00_A5_C32_C70_POL_MK40_NOP_AG_C00_A5_C32_C70_POL_MK40_AG_NOP_C00_A5_C35[32];

	UWORD FPU_C78_A3_C60_A3_CD3[7];
	UWORD FPU_AG_C78_A3_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD3[15];
	UWORD FPU_AG_C78_A3_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD3[32];
	UWORD FPU_AG_C78_A3_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD0[32];
	UWORD FPU_AG_C78_A3_CA2_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD1_C70_POL_MK20_AG_C60_A3_CD0[32];

#endif

	// ===== END =====

	UWORD FPUEND[1];
#else
	// ===== Read =====
	UWORD FPU_AG_C05_A5_CE0_DR[7];
	UWORD FPU_AG_DLY_C05_A5_CE0_DR[12];
	UWORD FPU_AG_DLY_C00_A5_C05_A2_CE0_DR[14];
	UWORD FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C30[14];
	UWORD FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C30[14];
	UWORD FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C30[14];
	UWORD FPU_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C01_C00_A5_C31[14];
	UWORD FPU_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C02_C00_A5_C31[14];
	UWORD FPU_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C03_C00_A5_C31[14];
	UWORD FPU_C01_AG_C00_A5_C30[6];
	UWORD FPU_C02_AG_C00_A5_C30[6];
	UWORD FPU_C03_AG_C00_A5_C30[6];
	UWORD FPU_C01_AG_C00_A5_C31[6];
	UWORD FPU_C02_AG_C00_A5_C31[6];
	UWORD FPU_C03_AG_C00_A5_C31[6];

#if (TLC_BICS2 & (!BURNER))
	UWORD FPU_C5D_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C01_C00_A5_C30[16];
	UWORD FPU_C5D_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C02_C00_A5_C30[16];
	UWORD FPU_C5D_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C03_C00_A5_C30[16];
	UWORD FPU_C5D_C01_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C01_C00_A5_C31[16];
	UWORD FPU_C5D_C02_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C02_C00_A5_C31[16];
	UWORD FPU_C5D_C03_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C03_C00_A5_C31[16];
	UWORD FPU_C5D_C01_AG_C00_A5_C30[7];
	UWORD FPU_C5D_C02_AG_C00_A5_C30[7];
	UWORD FPU_C5D_C03_AG_C00_A5_C30[7];
	UWORD FPU_C5D_C01_AG_C00_A5_C31[7];
	UWORD FPU_C5D_C02_AG_C00_A5_C31[7];
	UWORD FPU_C5D_C03_AG_C00_A5_C31[7];
#endif

#if Hynix
	UWORD FPU_CA2_AG_C00_A5_C32_C70_POL_MK40_AG_CA2_C00_A5_C35[14];
#endif

	// ===== Soft Bit ====
	UWORD FPU_AG_C05_A5_CE0[5];
	UWORD FPU_C01_AG_C00_A5_C3C[6];
	UWORD FPU_C02_AG_C00_A5_C3C[6];
	UWORD FPU_C03_AG_C00_A5_C3C[6];
	UWORD FPU_C01_AG_CC2_C00_A5_C3C[7];
	UWORD FPU_C02_AG_CC2_C00_A5_C3C[7];
	UWORD FPU_C03_AG_CC2_C00_A5_C3C[7];
	UWORD FPU_AG_C00_A5_CAE[5];
#if Hynix
	UWORD FPU_AG_C00_A5_C05_A2_CE0[7];
#endif
	// ===== Write =====
	UWORD FPU_AG_C11_C70_POL_MK40_C80_A5_DW_C15[11];
	UWORD FPU_AG_C11_C70_POL_MK40_C80_A5_DW[10];
	UWORD FPU_AG_CA2_C80_A5_C10[9];

	// ===== END =====

#if ENABLE_RESET_RETRY_LEVEL
	UWORD FPU_CEF_A89_D00_D00_D00_D00_C70_POL_MK20_CEF_A8A_D00_D00_D00_D00_C70_POL_MK20[24];
	UWORD FPU_CEF_A12_D00_D00_D00_D00_C70_POL_MK20_CEF_A13_D00_D00_D00_D00_C70_POL_MK20[24];
	UWORD FPU_C5C_CC5_C55_A04_D00_C55_A05_D00_C55_A06_D00_C55_A07_D00_C55_A0D_D00[18];

#if (!TLC)
	UWORD FPU_C26_C5D_AG_C00_A5_C32_C70_POL_MK40_AG_C26_C5D_C00_A5_C30[16];
	UWORD FPU_PTR_AG_C26_C5D_C00_A5_C30[7];
	UWORD FPU_PTR_AG_C26_C5D_C00_A5_C31[7];
	UWORD FPU_C5D_AG_C00_A5_C32_C70_POL_MK40_AG_C5D_C00_A5_C30[14];
	UWORD FPU_PTR_AG_C5D_C00_A5_C30[6];
	UWORD FPU_PTR_AG_C5D_C00_A5_C31[6];
#endif
#endif

	UWORD FPUEND[1];
#endif
} RamSEQs_T;







typedef struct {
	UWORD PTR_CFF[4];
	UWORD PTR_C70_POL_MK40[6];
	UWORD PTR_C70_POL_MK40_C00[6];
	UWORD PTR_C70_POL_MK20[6];
	UWORD PTR_C70_POL_MK20_C00[6];
	UWORD PTR_C70_DAT_MK40[6];
	UWORD PTR_C70_DAT_MK20[6];
	UWORD PTR_C71_DAT_MK40[6];
	UWORD PTR_C71_DAT_MK20[6];
	UWORD PTR_C73_DAT_MK40[6];
	UWORD PTR_C73_DAT_MK20[6];
	UWORD PTR_C70_DAT_MK0F[6];
	UWORD PTR_C71_DAT_MK1F[6];
	UWORD PTR_C73_DAT_MK1F[6];

	UWORD PTR_AG_CA2_C60_A3_CD0[10];
	UWORD PTR_AG_C60_A3_CD0[10];
	UWORD PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0[22];
	UWORD PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_CD0[22];
	UWORD PTR_AG_CA2_C60_A3_AG_C60_A3_CD0[14];
	UWORD PTR_AG_C60_A3_AG_C60_A3_CD0[14];
	UWORD PTR_AG_CA2_C80_A5_DW[12];
	UWORD PTR_AG_C80_A5_DW[12];
	UWORD PTR_AG_CA2_C80_A5_DW_C11[12];
	UWORD PTR_AG_C80_A5_DW_C11[10];
	UWORD PTR_C11[2];
	UWORD PTR_AG_CA2_C80_A5_DW_C15[12];
	UWORD PTR_AG_C80_A5_DW_C15[12];
	UWORD PTR_AG_CA2_C80_A5_DW_C10[12];
	UWORD PTR_AG_C80_A5_DW_C10[10];
	UWORD PTR_C10[2];
	UWORD PTR_AG_CA2_C81_A5_DW_C11[9];
	UWORD PTR_AG_C81_A5_DW_C11[9];
	UWORD PTR_AG_CA2_C81_A5_DW_C15[9];
	UWORD PTR_AG_C81_A5_DW_C15[9];
	UWORD PTR_AG_CA2_C81_A5_DW_C10[9];
	UWORD PTR_AG_C81_A5_DW_C10[9];
	UWORD PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW_C15[13];
	UWORD PTR_AG_C11_C70_POL_MK40_C81_A5_DW_C15[1];
	UWORD PTR_C11_C70_POL_MK40_C81_A5_DW_C15[11];
	UWORD PTR_AG_CA2_C11_C70_POL_MK40_C81_A5_DW[12];
	UWORD PTR_AG_C11_C70_POL_MK40_C81_A5_DW[12];
	UWORD PTR_C11_C70_POL_MK40_C81_A5_DW[12];
	UWORD PTR_AG_CA2_C00_A5_C30[12];
	UWORD PTR_AG_C00_A5_C30[12];
	UWORD PTR_C00_A5_C30[12];
	UWORD PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30[36];
	UWORD PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30[36];
	UWORD PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30[20];
	UWORD PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C30[20];
	UWORD PTR_AG_CA2_C00_A5_C31[8];
	UWORD PTR_AG_C00_A5_C31[8];
	UWORD PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31[36];
	UWORD PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31[36];
	UWORD PTR_AG_CA2_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31[16];
	UWORD PTR_AG_C00_A5_C32_C70_POL_MK40_AG_C00_A5_C31[16];
	UWORD PTR_AG_CA2_C00_A5_C32[12];
	UWORD PTR_AG_C00_A5_C32[12];
	UWORD PTR_CA2_C31[8];
	UWORD PTR_C31[4];
	UWORD PTR_CA2_C3F[4];
	UWORD PTR_C3F[4];
	UWORD PTR_AG_CA2_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30[20];
	UWORD PTR_AG_C60_A3_AG_C60_A3_AG_C60_A3_AG_C60_A3_C30[20];
	UWORD PTR_AG_CA2_C60_A3_AG_C60_A3_C30[16];
	UWORD PTR_AG_C60_A3_AG_C60_A3_C30[16];
	UWORD PTR_AG_DR[3];
	UWORD PTR_AG_C00_DR[5];
	UWORD PTR_AG_C00_A5_C05_A2_CE0_DR[1];
	UWORD PTR_C00_A5_C05_A2_CE0_DR[13];
	UWORD PTR_NOP[2];
	UWORD PTR_AG_IBF_CORRECT[4];
	UWORD PTR_IBF_RESTORE_BACKUP_CUSTOMIZATION[4];
	UWORD PTR_AG_RAW[4];
	UWORD PTR_SOFTBIT_CORRECTION[4];
	UWORD PTR_CC6_AG_C60_A3_CD0[8];
	UWORD PTR_CC6_AG_C60_A3_AG_C60_A3_CD0[12];
	UWORD PTR_CA2_AG_C60_A3_CD1_AG_C60_A3_CD0[20];
	UWORD PTR_AG_C60_A3_CD1_AG_C60_A3_CD0[20];
	UWORD PTR_CFC[2];
	UWORD PTR_AG_CA2_C80_A5_C10[12];
} FPUSEQs_T;



extern RamSEQs_T guwFpuEntry;
extern FPUSEQs_T guwIRAMContent;

#endif
typedef struct { //S5
	U32   ulHeader;     // 0
	UBYTE ubSecMode; // 4
	UBYTE ubScanVTReboot; // 5
	UBYTE Pad[501]; 	// 6 - 507
	U32 CRC; // 508 - 511
} AON_SAVED_DATA;

////////////////////////////////////////////////////////////////////////////////
#if 1 || (!BURNER)
/* 16-bits base address */
#define FPU_SEQ_PTR16(x)	(((unsigned int)(offsetof(FPUSEQs_T, x)) )+FPU_ADR)
#define RAM_SEQ_PTR16(x)	(((unsigned int)(offsetof(RamSEQs_T, x)) )+FPU_ADR2)
#define RAM_SEQ_PTR_L(x)	(((offsetof(RamSEQs_T, x) )+FPU_ADR2) & 0xFF)
#define RAM_SEQ_PTR_H(x) 	(((offsetof(RamSEQs_T, x) )+FPU_ADR2) >> 8)
#else
#define RAM_SEQ_PTR16(x)    (((unsigned int)(x)) + FPU_ADR2)
#define RAM_SEQ_PTR_L(x)    ((x + FPU_ADR2) & 0xFF)
#define RAM_SEQ_PTR_H(x)    ((x + FPU_ADR2) >> 8)
#endif


//#define RAM_SEQ_PTR16(x)	(((unsigned int)(offsetof(RamSEQs_T, x)))+sizeof(gubIRAMContent))
//#define RAM_SEQ_PTR_L(x)	(((offsetof(RamSEQs_T, x))+sizeof(gubIRAMContent)) & 0xFF)
//#define RAM_SEQ_PTR_H(x) 	(((offsetof(RamSEQs_T, x))+sizeof(gubIRAMContent)) >> 8)

#define SEQ_PTR_L(x)		((unsigned char)(x & 0xFF))
#define SEQ_PTR_H(x)		((unsigned char)((x >> 8) & 0xFF))

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/



//For SourceUint TO TargetUint

#if TLC_BICS2 || Hynix3DV6 || Hynix3DV5 || YMTC_JGS
#define TLC_PROGRAM_SEQ_CNT		(1U)
#else
#define TLC_PROGRAM_SEQ_CNT		(3U)
#endif

#define FIRST_ORDER 0
#define FOGGY_ORDER 1
#define FINE_ORDER 2

#define LOWER 0		//	for C1A
#define MIDDLE 1	//	for C1A
#define UPPER 2		//	for C15
#define UPPER_END 3	//	for C10

#define WRITE_DONE		BIT4
#define SPARE_COPY_DONE BIT5
#define READ_FQ_GET_DONE BIT6
#define WRITE_FQ_GET_DONE BIT7
#define READ_FQ_GET_DONE2 BIT7

#define PLANEA 0
#define PLANEB 1
#define PLANEC 2
#define PLANED 3

#define DMA_EN 0
#define DMA_DIS 1


//BIT0: Nromal(3D1 to 1D3);
//BIT1: WL Skip , Run Even Page(6D1 to 1D3);
//BIT2: WL Skip , Run Odd Page(6D1 to 1D3),
#define COPYBACK_NORMAL BIT0		//0, 1, 2, 3, 4, 5, 6...
#define COPYBACK_EVEN_PAGE BIT1		//0, 2, 4, 6, 8...
#define COPYBACK_ODD_PAGE BIT2		//1, 3, 5, 7, 9...

#define COPYBACK_NON_DMA BIT3
#define COPYBACK_D3_TO_D3 BIT4
#define COPYBACK_PLANE_INDEX BIT6 //用掉BIT6 BIT7來記Plane


#define D1TOD3_UART 0
#define D1TOD3_ERR_UART 1

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//For FQ ubD3WriteMode

#define D3_WRITE_EN		BIT7
#define D3_SUSPEND_EN	BIT7	//Only For D1 to D3 Data In Out

#define D3_WRITE_FIRST	BIT4
#define D3_WRITE_FOGGY	BIT5
#define D3_WRITE_FINE	BIT6
#define D3_WRITE_FFF_MASK	(D3_WRITE_FIRST|D3_WRITE_FOGGY|D3_WRITE_FINE)
#define D3_WRITE_FFF_OFFSET	4


#define D3_WRITE_LOWDER	BIT0
#define D3_WRITE_MIDDLE	BIT1
#define D3_WRITE_UPPER	BIT2
#if Hynix3DV6 || YMTC
#define D3_WRITE_UPPER_END	BIT2
#else
#define D3_WRITE_UPPER_END	BIT3
#endif
#define D3_WRITE_LMU_MASK	(D3_WRITE_LOWDER|D3_WRITE_MIDDLE|D3_WRITE_UPPER|D3_WRITE_UPPER_END)
#define D3_WRITE_LMU_OFFSET	0

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

////////////////////////////////////////////////////////////////////////////////

#define FW_D1TOD3_COPYBACK BIT0
#define FW_D1TOD3_DATAINOUT BIT1


#define D3_ECC_VERIFY BIT2
#define D1_ECC_VERIFY BIT3
#define D3_ECC_VERIFY_L BIT4
#define D3_ECC_VERIFY_M BIT5
#define D3_ECC_VERIFY_U BIT6
#define D3_ECC_VERIFY_FIRST_FRAME_ALL_PAGE BIT7
#define D3_ECC_VERIFY_CHECK (D3_ECC_VERIFY_L|D3_ECC_VERIFY_M|D3_ECC_VERIFY_U)

#define FW_D1TOD3_COPYBACK_START_END_UART 0
#define FW_D1TOD3_DATAINOUT_START_END_UART 0
#define ECC_VERIFY_START_END_UART 0
#define FW_SOURCEUINT_TO_TARGETUINT_START_END_UART 0
#define FW_SOURCEUINT_TO_TARGETUINT_ERR_UART 1

#define D1D3_DOING_TIME_UART 0
#if (Hynix3DV6 || Hynix3DV5 || YMTC_JGS)
#define TLC_FLUSH_ORDER_NUM (8U)//(32U)
#elif Hynix_16nm
#define TLC_FLUSH_ORDER_NUM (43U)//(32U)
#else
#define TLC_FLUSH_ORDER_NUM (32U)
#endif

//----------------------
// Random Function
//----------------------
#if ENABLE_IOMETER_RAND
#define RAND_VEC_A   136204069ULL      // 3x7x11x13x17x23x29x4 + 1
#define RAND_VEC_B   28500701229ULL  // 3x7x11x13x17x23x27x29x31
#endif

//
//		True Table
//		btD3-Fast-Link
#define TURE_TABE_B000	0
#define TURE_TABE_B001	1
#define TURE_TABE_B010	2
#define TURE_TABE_B011	3
#define TURE_TABE_B100	4
#define TURE_TABE_B101	5
#define TURE_TABE_B110	6
#define TURE_TABE_B111	7
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//VT->gubSaveVTSerialNumber
#define VT_FTL_TASK_0x01 0x01
#define VT_FTL_TASK_0x02 0x02
#define VT_BG_FLOW_0x03 0x03
#define VT_XFER_DATA_IN_0x04 0x04
#define VT_XFER_DATA_IN_0x05 0x05
#define VT_GC_LOAD_P2L_0x06 0x06
#define VT_GC_LOAD_P2L_0x07 0x07
#define VT_FLUSH_D1_0x08 0x08
#define VT_CLOSE_TARGET_0x09 0x09
#define VT_CLEAN_GR_0x0A 0x0A
#define VT_CLEAN_GR_0x0B 0x0B
#define VT_CLEAN_GR_0x0C 0x0C
#define VT_CLEAN_GR_0x0D 0x0D
#define VT_CLOSE_TABLE_TARGET_0x0E 0x0E
#define VT_CLOSE_TABLE_TARGET_0x0F 0x0F
#define VT_SAVE_TABLE_0x10 0x10
#define VT_POWER_CYCLE_0x11 0x11
#define VT_SWAP_RUT_0x12 0x12
//#define VT_SWAP_RUT_0x13 0x13
#define VT_SWAP_RUT_0x14 0x14
#define VT_INIT_FLASH_0x15 0x15
#define VT_INIT_FLASH_0x16 0x16
#define VT_INIT_FLASH_0x17 0x17
#define VT_VEDER_RESET_0x18 0x18
#define VT_INIT_FLASH_0x19 0x19
#define VT_SWAP_RUT_0x1A 0x1A
#define VT_CLOSE_TARGET_0x1B 0x1B
#define VT_CLOSE_TARGET_STOP_0x1C 0x1C
#define VT_CLOSE_TABLE_TARGET_0x1E 0x1E
#define VT_SWAP_TABLE_0x1F 0x1F
#define VT_FTL_TASK_0x20 0x20
#define VT_INIT_FLASH_0x21 0x21

// AssertCriticalError Save VT
#define VT_ASSERT_FTL_TASK_0x22 0x22
#define VT_ASSERT_XFER_DATA_OUT_0x23 0x23
#define VT_ASSERT_XFER_DATA_IN_0x24 0x24
#define VT_ASSERT_SAVE_DUMMY_0x25 0x25
#define VT_ASSERT_COPY_DATA_0x26 0x26
#define VT_ASSERT_LOAD_P2L_TABLE_0x27 0x27
#define VT_ASSERT_COPY_UNIT_0x28 0x28
#define VT_ASSERT_LOAD_TABLE_P2L_0x29 0x29
#define VT_ASSERT_CHECK_VC_0x2A 0x2A
#define VT_ASSERT_CHECK_VC_0x2B 0x2B
#define VT_ASSERT_CHECK_TABLE_VC_0x2C 0x2C
#define VT_ASSERT_CHECK_TABLE_VC_0x2D 0x2D
#define VT_ASSERT_POWER_CYCLE_0x2E 0x2E
#define VT_ASSERT_INIT_FLASH_0x2F 0x2F
#define VT_ASSERT_INIT_FLASH_0x30 0x30
#define VT_ASSERT_INIT_FLASH_0x31 0x31
#define VT_ASSERT_INIT_FLASH_0x32 0x32
#define VT_ASSERT_INIT_FLASH_0x33 0x33
#define VT_ASSERT_INIT_FLASH_0x34 0x34
#define VT_ASSERT_INIT_FLASH_0x35 0x35
#define VT_ASSERT_INIT_FLASH_0x36 0x36
#define VT_ASSERT_INIT_FLASH_0x37 0x37
#define VT_ASSERT_INIT_FLASH_0x38 0x38
#define VT_ASSERT_SCAN_VT_0x39 0x39
#define VT_ASSERT_SCAN_VT_BINARY_0x3A 0x3A
#define VT_ASSERT_SCAN_VT_BINARY_0x3B 0x3B
#define VT_ASSERT_LOAD_P2L_READ_MODE_0x3C 0x3C

#define VT_FTL_TASK_0x3D 0x3D
#define VT_POWER_CYCLE_0x3E 0x3E
#define VT_INIT_FLASH_0x3F 0x3F
#define VT_INIT_FLASH_0x40 0x40
#define VT_ASSERT_RS_RECOVER_0x41 0x41
#define VT_ASSERT_SETFLASHSOURCEADDRCE_CH_0x42 0x42
#define VT_ASSERT_FLASHTOSRAM_CH_0x43 0x43
#define VT_ASSERT_SRAMTOFLASH_CH_0x44 0x44
#define VT_ASSERT_ERASETARGET_CH_0x45 0x45
#define VT_ASSERT_FLASHTOSRAMRANDCACHE_CH_0x46 0x46
#define VT_ASSERT_FLASHTOSRAM_0030_0x47 0x47
#define VT_ASSERT_LOAD_TABLE_0x48 0x48
#define VT_ASSERT_LOAD_TABLE_0x49 0x49
#define VT_ASSERT_SAVE_TABLE_0x4A 0x4A
#define VT_ASSERT_POWER_CYCLE_0x4B 0x4B
#define VT_ASSERT_GET_USER_FREE_BLOCK_0x4C 0x4C
#define VT_ASSERT_GET_USER_FREE_BLOCK_0x4D 0x4D
#define VT_ASSERT_GET_MAX_EC_FREE_BLOCK_0x4E 0x4E

#define VT_ASSERT_DDR_UNC_0x4F 0x4F
#define VT_ASSERT_STACK_OVERFLOW_0x50 0x50
#define VT_ASSERT_PARITY_ERROR_0x51 0x51
#define VT_ASSERT_HANDLE_SINGLE_FREE_0x52 0x52
#define VT_ASSERT_ERASE_SYSTEM_UNIT_0x53	0x53

#define VT_MAIN_CLEAR_LASTCMD_STANDBY_NCQ 0x54
#define VT_MAIN_CLEAR_LASTCMD_STANDBY_DOSTCMD 0x55
#define VT_ASSERT_NO_D1_TO_BE_FLUSHED_0x56 0x56
#if SAVE_BUNCH_PARITY_FOR_FLUSH_COMMAND==1
#define VT_ASSERT_RS_FPU_READ_INITINFO_PARITY_0x57 0x57
#define VT_ASSERT_RS_FPU_READ_INITINFO_PARITY_0x58 0x58
#define VT_ASSERT_Bunch_RS_Recover 0x59
#define VT_GUARANTEE_FLUSH_0x60 0x60
#define VT_BG_FLOW_0x61			0x61
#define VT_XFER_DATA_IN_0x62	0x62
#define VT_POWER_CYCLE_0x63		0x63
#endif
#define VT_ASSERT_GET_RSGROUPTAGFAILMAPINDEX_0x64 0x64
#define VT_SECURITY_ERASE_USERDATA_0x65 0x65
#define VT_SECURITY_ERASE_USERDATA_0x66 0x66
#define VT_BG_FLOW_0x67 0x67
#define VT_ASSERT_LOAD_TABLE_0x68 0x68
#define VT_CLEAN_GR_0x69 0x69
#define VT_ASSERT_VC_BOMB_0x6A 0x6A
#define VT_ASSERT_VC_BOMB_0x6B 0x6B
#define VT_ASSERT_VC_BOMB_0x6C 0x6C
#define VT_ASSERT_VC_BOMB_0x6D 0x6D
#define VT_ASSERT_GC_TOPVC_MISMATCH_0x6E 0x6E
#define VT_ASSERT_TABLEUNIT_READUNC_0x6F 0x6F
#define VT_ASSERT_SCAN_VT_0x70 0x70
#define VT_ULTRAMLC_CONSECUTIVE_RSENCDOE_00 0x71
#define VT_ULTRAMLC_CONSECUTIVE_RSENCDOE_01 0x72
#define VT_ULTRAMLC_CONSECUTIVE_RSENCDOE_02 0x73
#define VT_ULTRAMLC_BUILD_LASTDATAPLANE_00 0x74
#define VT_ULTRAMLC_BUILD_LASTDATAPLANE_01 0x75
#define VT_ASSERT_SAVE_DUMMY_WRONG_MODE_0x76 0x76
#define VT_SET_POWER_CYCLE_0x77 0x77
// Flush D1 Error Handle (@mark_liu)
#define VT_FLUSHD1_ERR_D1_UNC_0x78		0x78
#define VT_FLUSHD1_ERR_D3_PF_CB_0x79		0x79
#define VT_FLUSHD1_ERR_D3_PF_DIO_0x7A		0x7A
#define VT_FLUSHD1_ERR_D3_ECC_CB_0x7B		0x7B
#define VT_FLUSHD1_ERR_D3_ECC_DIO_0x7C		0x7C
#define VT_COPYD3UNIT_ERR_D1_UNC_0x7D		0x7D
#define VT_COPYD3UNIT_ERR_D3_PF_CB_0x7E		0x7E
#define VT_COPYD3UNIT_ERR_D3_PF_DIO_0x7F	0x7F
#define VT_COPYD3UNIT_ERR_D3_ECC_CB_0x80	0x80
#define VT_COPYD3UNIT_ERR_D3_ECC_DIO_0x81	0x81

//Read Disturb
#define VT_READ_DISTURB_MOVE_0x82 0x82
#define VT_READ_DISTURB_MOVE_0x83 0x83
// Fail Handling
#define VT_FAILHANDLEUNIT_NOT_ENOHGH_0x84	0x84
// FTL ABNORMAL LOCATION
#define VT_POWERCYCLE_CROSS_UNIT_0x85 0x85
#define VT_POWERCYCLE_CROSS_UNIT_0x86 0x86
#define VT_XFERDATAIN_CROSS_UNIT_0x87 0x87

//Security erase fail handle
#define VT_SECURITY_ERASE_USERDATA_0x88 0x88

#define VT_BACKUP_DBT_LOAD_DBT_FAIL_0x89 0x89
#define VT_REFRESH_DBT_LOAD_DBT_FAIL_0x8A 0x8A

// VT Dummy
#define VT_INIT_FLASH_VTDUMMY_0x8B 0x8B
#define VT_INIT_FLASH_VTCHILDDUMMY_0x8C 0x8C

#define VT_COPY_INIT_INFO_0x8D	0x8D

// Fail Handling
#define VT_UPDATE_RUT_NOSAVE_VTMOTHER_0x8E 0x8E
#define VT_ASSERT_NO_RUTSUMMARYLOG_0x8F 0x8F
#define VT_ASSERT_ABNORMAL_FAILUNIT_0x90 0x90
#define VT_ASSERT_UNKNOWN_DIE_0x91 0x91
#define VT_ASSERT_ABNORMAL_ABORTDONE_0x92 	0x92
#define VT_ASSERT_ABNORMAL_ABORTDONE_0x93 	0x93

// Quick Erase
#define VT_QUICK_ERASE_0x94 0x94

//TrimAll
#define VT_TRIM_ALL_DONE_0x95 0x95

//MP RW fail save error block
#define VT_ASSERT_MPRW_WRITEFAIL_0x96 	0x96
#define VT_ASSERT_MPRW_ERASEFAIL_0x97 	0x97
#define VT_ASSERT_MPRW_READFAIL_0x98 		0x98
#define VT_ASSERT_MPRW_RSFAIL_0x99 		0x99

#define VT_READ_DISTURB_MOVE_0x9A 0x9A
#define VT_ASSERT_RUTADDRESS_NOTMATCH_0x9B 0x9B
#define VT_TABLE_VERIFY_RETRY_0x9C 0x9C
#define VT_GCTOD3_ERR_D3_PF_0x9E	0x9E
#define VT_GCTOD3_ERR_D3_ECC_0x9F	0x9F

//SwapRUT
#define VT_ASSERT_FORCEREADMOVE_DONTCOPY_0xA0 0xA0
#define VT_ASSERT_FORCEREADMOVE_COPYUNIT_SAVEDUMMY_0xA1 0xA1
#define VT_ASSERT_FORCEREADMOVE_L2PCOLLISION_0xA2 0xA2
#define VT_ASSERT_FORCEREADMOVE_PARTIALMODE_0xA3 0xA3
#define VT_ASSERT_FORCEREADMOVE_QUEUE_MISMATCH_0xA4 0xA4
#define VT_ASSERT_FORCEREADMOVE_D3_NOTFOUND_0xA5 0xA5
#define VT_ASSERT_FORCEREADMOVE_FASTPAGE_CHANGED1_0xA6 0xA6

#define VT_ASSERT_TABLEGC_INDEX_BOMB_0xA7 0xA7
#define VT_ASSERT_ADD_FREEQ_UNIT0_0xA8	0xA8
#define VT_ASSERT_REFRESH_UPDATE_SYS_NOFREE_0xA9 0xA9


#define VT_ERASE_STATUS_MISMATCH_0xB0        0xB0
#define VT_PROGRAM_STATUS_MISMATCH_0xB1		 0xB1
#define VT_ASSERT_SEQUENCE_MARK_STATUS_BAD_0xB2	 0xB2
#define VT_RUTLOG_FULL_0xBF		 0xBF

// Force Save Dummy
#define VT_FORCE_DUMMY_0xC0 0xC0
#define VT_FORCE_DUMMY_0xC1 0xC1
#define VT_FORCE_DUMMY_0xC2 0xC2
#define VT_FORCE_DUMMY_0xC3 0xC3
#define VT_FORCE_DUMMY_0xC4 0xC4
#define VT_FORCE_DUMMY_0xC5 0xC5
#define VT_FORCE_DUMMY_0xC6 0xC6
#define VT_FORCE_DUMMY_0xC7 0xC7
#define VT_FORCE_DUMMY_0xC8 0xC8
#define VT_FORCE_DUMMY_0xC9 0xC9
#define VT_FORCE_DUMMY_0xCA 0xCA
#define VT_FORCE_DUMMY_0xCB 0xCB
#define VT_FORCE_DUMMY_0xCC 0xCC
#define VT_FORCE_DUMMY_0xCD 0xCD
#define VT_FORCE_DUMMY_0xCE 0xCE
#define VT_FORCE_DUMMY_0xCF 0xCF
#define VT_FORCE_DUMMY_0xD0 0xD0
#define VT_FORCE_DUMMY_0xD1 0xD1
#define VT_FORCE_DUMMY_0xD2 0xD2
#define VT_FORCE_DUMMY_0xD3 0xD3
#define VT_FORCE_DUMMY_0xD4 0xD4
#define VT_FORCE_DUMMY_0xD5 0xD5
#define VT_FORCE_DUMMY_0xD6 0xD6
#define VT_FORCE_DUMMY_0xD7 0xD7
#define VT_FORCE_DUMMY_0xD8 0xD8
#define VT_FORCE_DUMMY_0xD9 0xD9

#define VT_DLMC_ECUPDATE_0xDA 0xDA

// AssertCriticalError NON Save VT
#define VT_ASSERT_NON_SAVE_0xFF 0xFF
#if ENABLE_DEBUG_FREE_BLOCK_TABLE==1
#define VT_ASSERT_UNIT_LOST_0xFB 0xFB
#define VT_ASSERT_UNIT_LOST_0xFC 0xFC
#define VT_ASSERT_UNIT_LOST_0xFD 0xFD
#define VT_ASSERT_UNIT_LOST_0xFE 0xFE
#endif


// AssertCriticalError SRAM to Flash
#define DUMP_VARIABLE_STACK_BASE 0x5C0CA050
#define DUMP_VARIABLE_STACK_LENGTH 0x5FB0
#define DUMP_SRAM0_BASE 0x60000400
#define DUMP_SRAM0_LENGTH 0x6FA00


#define ReadDisturb_Base 4000
#define ReadCheck_Base 1000



//gubReadDisturbMoveLevel; //RDML
//0:Normal, 1:Reset Read , 2:HB[0], 3:HB[1], N:HB[N-2]
#define RDML_NOTHING_TO_DO 0
#define RDML_RESET_READ 1
#define RDML_HB_START 2
#define RDML_HB0 2
#define RDML_HB1 3
#define RDML_HB2 4
#define RDML_HB3 5


//flaCheckChannelFPUBusy (SerialNumber)
#define SN_0x1_FPU_POL_CheckWriteStatus			0x1
#define SN_0x2_FPU_POL_CheckWriteStatus 			0x2
#define SN_0x3_FPU_W_FixDQSREMismatch 			0x3
#define SN_0x4_FPU_POL_FixDQSREMismatch 		0x4
#define SN_0x5_FPU_R_FixDQSREMismatch 			0x5
#define SN_0x6_FPU_POL_TriggerFPU 				0x6
#define SN_0x7_FPU_R_TriggerFPU 				0x7
#define SN_0x8_FPU_POL_SendResetCommand 		0x8
#define SN_0x9_FPU_W_ReGenP4KCRC32 			0x9
#define SN_0xA_FPU_POL_ReadFailRetry 			0xA
#define SN_0xB_FPU_POL_MicronCoarseTuning 		0xB
#define SN_0xC_FPU_R_MicronCoarseTuning 			0xC
#define SN_0xD_FPU_R_SoftBitDataOut 			0xD
#define SN_0xE_FPU_R_SoftBitDataOut 			0xE
#define SN_0xF_FPU_POL_SandiskBics2SReadCom		0xF
#define SN_0x10_FPU_POL_TLCSendXorCom			0x10
#define SN_0x11_FPU_POL_TLCSendXorCom			0x11
#define SN_0x12_FPU_POL_TLCSendXorCom			0x12
#define SN_0x13_FPU_POL_TLCSendXorCom			0x13
#define SN_0x14_FPU_R_TLCSendXorCom			0x14
#define SN_0x15_FPU_R_TLCSendXorCom			0x15
#define SN_0x16_FPU_POL_SandiskBics2SendXorCom	0x16
#define SN_0x17_FPU_R_SandiskBics2SendXorCom		0x17
#define SN_0x18_FPU_R_SandiskBics2SendXorCom		0x18
#define SN_0x19_FPU_POL_TLCReadIndicator_Bics3	0x19
#define SN_0x1A_FPU_R_TLCReadIndicator_Bics3		0x1A
#define SN_0x1B_FPU_R_TLCReadIndicator_Bics3		0x1B
#define SN_0x1C_FPU_POL_TLCReadIndicator		0x1C
#define SN_0x1D_FPU_R_TLCReadIndicator			0x1D
#define SN_0x1E_FPU_R_TLCReadIndicator			0x1E
#define SN_0x1F_FPU_POL_FLHShiftReadDataFPU		0x1F
#define SN_0x20_FPU_R_FLHShiftReadDataFPU		0x20
#define SN_0x21_FPU_POL_FLHShiftReadDataFPU		0x21
#define SN_0x22_FPU_R_FLHShiftReadDataFPU		0x22
#define SN_0x23_FPU_R_FLHShiftReadDataFPU		0x23
#define SN_0x24_FPU_POL_FLHShiftReadDataFPU		0x24
#define SN_0x25_FPU_R_FLHShiftReadDataFPU		0x25
#define SN_0x26_FPU_R_FLHShiftReadDataFPU		0x26
#define SN_0x27_FPU_POL_FLHShiftReadDataFPU		0x27
#define SN_0x28_FPU_R_FLHShiftReadDataFPU		0x28
#define SN_0x29_FPU_R_FLHShiftReadDataFPU		0x29
#define SN_0x2A_FPU_POL_FLHShiftReadDataFPU		0x2A
#define SN_0x2B_FPU_R_FLHShiftReadDataFPU		0x2B
#define SN_0x2C_FPU_R_FLHShiftReadDataFPU		0x2C
#define SN_0x2D_FPU_POL_FLHShiftReadDataFPU		0x2D
#define SN_0x2E_FPU_R_FLHShiftReadDataFPU		0x2E
#define SN_0x2F_FPU_R_FLHShiftReadDataFPU		0x2F
#define SN_0x30_FPU_POL_FLHShiftReadDataFPU		0x30
#define SN_0x31_FPU_R_FLHShiftReadDataFPU		0x31
#define SN_0x32_FPU_R_FLHShiftReadDataFPU		0x32
#define SN_0x33_FPU_R_FLHShiftReadDataFPU		0x33
#define SN_0x34_FPU_R_FLHShiftReadDataFPU		0x34
#define SN_0x35_FPU_R_FLHShiftReadDataFPU		0x35
#define SN_0x36_FPU_R_FLHShiftReadDataFPU		0x36
#define SN_0x37_FPU_R_FLHShiftReadDataFPU		0x37
#define SN_0x38_FPU_R_FLHShiftReadDataFPU		0x38
#define SN_0x39_FPU_R_FLHShiftReadDataFPU		0x39
#define SN_0x3A_FPU_R_FLHShiftReadDataFPU		0x3A
#define SN_0x3B_FPU_R_FLHShiftReadDataFPU		0x3B
#define SN_0x3C_FPU_R_FLHShiftReadDataFPU		0x3C
#define SN_0x3D_FPU_R_FLHShiftReadDataFPU		0x3D
#define SN_0x3E_FPU_R_FLHShiftReadDataFPU		0x3E
#define SN_0x3F_FPU_R_FLHShiftReadDataFPU		0x3F
#define SN_0x40_FPU_R_FLHShiftReadDataFPU		0x40
#define SN_0x41_FPU_R_FLHShiftReadDataFPU		0x41
#define SN_0x42_FPU_R_FLHShiftReadDataFPU		0x42
#define SN_0x43_FPU_POL_FLHShiftReadDataFPU		0x43
#define SN_0x44_FPU_R_FLHShiftReadDataFPU		0x44
#define SN_0x45_FPU_POL_FLHShiftReadDataFPU		0x45
#define SN_0x46_FPU_R_FLHShiftReadDataFPU		0x46
#define SN_0x47_FPU_R_FLHShiftReadDataFPU		0x47
#define SN_0x48_FQ_RS_FPU_Read				0x48
#define SN_0x49_FPU_POL_RS_FPU_Read			0x49
#define SN_0x4A_FPU_POL_RS_FPU_Read			0x4A
#define SN_0x4B_FPU_R_RS_FPU_Read			0x4B
#define SN_0x4C_FPU_R_RS_FPU_Read			0x4C
#define SN_0x4D_FQ_RS_FPU_Read_InitInfo_Parity		0x4D
#define SN_0x4E_FPU_POL_RS_FPU_Read_InitInfo_Parity	0x4E
#define SN_0x4F_FPU_POL_RS_FPU_Read_InitInfo_Parity	0x4F
#define SN_0x50_FPU_R_RS_FPU_Read_InitInfo_Parity	0x50
#define SN_0x51_FPU_POL_RS_Flush_MultiPlaneProgram	0x51
#define SN_0x52_FPU_POL_RS_Flush_MultiPlaneProgram	0x52
#define SN_0x53_FPU_W_RS_Flush_MultiPlaneProgram	0x53
#define SN_0x54_FPU_POL_RS_Flush_MultiPlaneProgram	0x54
#define SN_0x55_FPU_POL_RS_Flush_MultiPlaneProgram	0x55
#define SN_0x56_FPU_POL_RS_Flush_MultiPlaneProgram	0x56
#define SN_0x57_FPU_W_RS_Flush_MultiPlaneProgram	0x57
#define SN_0x58_FPU_POL_RS_Flush_MultiPlaneProgram	0x58
#define SN_0x59_FPU_POL_RS_Flush_MultiPlaneProgram	0x59
#define SN_0x5A_FPU_POL_RS_Flush_MultiPlaneProgram	0x5A
#define SN_0x5B_FPU_POL_RS_Flush_MultiPlaneProgram	0x5B
#define SN_0x5C_FPU_POL_RS_Flush_MultiPlaneProgram	0x5C
#define SN_0x5D_FQ_RS_Recover				0x5D
#define SN_0x5E_FPU_POL_RS_Recover			0x5E
#define SN_0x5F_FPU_POL_Bunch_RS_Recover		0x5F
#define SN_0x60_FPU_POL_Bunch_RS_Recover		0x60
#define SN_0x61_FPU_W_Bunch_RS_Recover			0x61
#define SN_0x62_FPU_POL_Bunch_RS_Recover		0x62
#define SN_0x63_FPU_POL_Bunch_RS_Recover		0x63
#define SN_0x64_FPU_POL_Bunch_RS_Recover		0x64
#define SN_0x65_FPU_W_Bunch_RS_Recover			0x65
#define SN_0x66_FPU_POL_Bunch_RS_Recover		0x66
#define SN_0x67_FPU_POL_Bunch_RS_Recover		0x67
#define SN_0x68_FPU_POL_Bunch_RS_Recover		0x68
#define SN_0x69_FPU_POL_Bunch_RS_Recover		0x69
#define SN_0x6A_FPU_POL_Bunch_RS_Recover		0x6A
#define SN_0x6B_FPU_POL_flaNandEmulatorVenderFPUErase	0x6B
#define SN_0x6C_FPU_POL_flaNandEmulatorVenderFPUErase	0x6C
#define SN_0x6D_FPU_POL_flaEraseFailHandle		0x6D
#define SN_0x6E_FPU_POL_flaEraseFailHandle		0x6E
#define SN_0x6F_FPU_POL_flaDumpError			0x6F
#define SN_0x70_FQ_flaTask					0x70
#define SN_0x71_FPU_POL_flaTask				0x71
#define SN_0x72_FPU_POL_TLCSendReadCom		0x72
#define SN_0x73_FPU_R_YMTCSendXorCom		0x73


#define REQUEST_DENY 0x01
#define VERIFY_FAIL     0x02
#define PARAMETER_ERROR 0xFFFF
#define HANDSHAKE_VERIFICATION_FAIL 0x0F000000

#define MAX_AUTHENTICATION_RETRY 5
#define MPRW_CMDTIMEOUT  13500  //by tester , tester 更新時間的話要改
#define TABLE_UNC_RESET_THRESHOLD		5
