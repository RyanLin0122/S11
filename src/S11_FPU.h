#ifndef __S11_FPU_H__
#define __S11_FPU_H__

///////////////////////////////////////////////////////////////////////////////////////
/*
	Copyright (c) 2015 Phison Electronics Corps.
	Create By CH.

	[Description]
		Define for use FPU.

	[Modify History]
		#0001, CH, 2015.09.09, New release.

*/
///////////////////////////////////////////////////////////////////////////////////////

//============================================================================================================
//  FPU, Flash Process Unit
//============================================================================================================

#if 1
#define FPU_NOP()                           (0x0000)

#define FPU_DLY(CYCLE)                      (0x1000 | ((CYCLE) ))    /* delay x cycles */

#define FPU_CMD(DAT)                        (0x2000 | ((DAT) ))
#if Hynix
#define FPU_CMD_DQS(DQS_ENA, DQS_H, DAT)    (0x2000 | ((DAT) ))
#else
#define FPU_CMD_DQS(DQS_ENA, DQS_H, DAT)    (0x2000 | ((DQS_ENA)<< 8) | ((DQS_H) << 9) | ((DAT) ))
#endif
#define FPU_ADDR(ADR_NUM)                    (0x3000 | ((ADR_NUM)<< 8))
#define FPU_ADDR_INC(ADR_NUM)                (0x3800 | ((ADR_NUM)<< 8))
#define FPU_ADDR_DAT(ADR_NUM, DAT)           (0x3000 | ((ADR_NUM)<< 8) | ((DAT)))
#define FPU_ADDR_INC_DAT(ADR_NUM, DAT)       (0x3800 | ((ADR_NUM)<< 8) | ((DAT)))
    #define SEND_ONE_ADDRESS       (1U)
    #define SEND_COLUMN_ADDRESS    (2U) /* 2 col */
    #define SEND_ROW_ADDRESS       (3U) /* 3 row */
    #define SEND_4_ADDRESS         (4U) /* 2 row, 2 col */
    #define SEND_5_ADDRESS         (5U) /* all */

#define FPU_DAT_WR(DATA)				(0x4D00 | DATA)
#define FPU_DAT_RD()				(0x4E00)

#define FPU_DAT_RDY(MASK)			(0x4200 | (MASK ))

#define FPU_DAT_FAIL(MASK)			(0x4000 | (MASK ))


#define FPU_POL(MASK)            	(0x5000 | ((BIT0) << 8) | ((MASK)))
#define FPU_POL_RB()             	(0x5000)

#define FPU_DMA(DMY_ENA, RW)     	(0x6000 | ((DMY_ENA) << 9) | ((RW) << 8))
#define FPU_DMA_R()              	(0x6000)

#if Hynix || YMTC
#define FPU_DMA_W()              	(0x6300)
#else
#define FPU_DMA_W()              	(0x6100)
#endif

#define FPU_ADR_GEN()            	(0x7000)

#define FPU_BCC_TRIG()           	(0x8000)

#define FPU_RTY_DAT(DAT)         	(0x9000 | ((DAT)))

#define FPU_RAW()					(0xA000)

#define FPU_IBF_CORRECT()			(0xB000)

#define FPU_SBC()					(0xC000)

#define FPU_BR(IBF_PTR, FRAME_NUM, DIRECTION, MODE) (0xD000 | (((IBF_PTR) << 5) | ((FRAME_NUM) << 2) | ((DIRECTION) << 1) | (MODE)) )

#define FPU_END()					(0xF000)
#else

#define FPU_NOP()                           (0x0000)

#define FPU_DLY(CYCLE)                      (0x0010 | ((CYCLE) << 8))    /* delay x cycles */

#define FPU_CMD(DAT)                        (0x0020 | ((DAT) << 8))
#define FPU_CMD_DQS(DQS_ENA, DQS_H, DAT)    (0x0020 | (DQS_ENA) | ((DQS_H) << 1) | ((DAT) << 8))

#define FPU_ADDR(ADR_NUM)                    (0x0030 | (ADR_NUM))
#define FPU_ADDR_INC(ADR_NUM)                (0x0038  | (ADR_NUM))
#define FPU_ADDR_DAT(ADR_NUM, DAT)           (0x0030 | (ADR_NUM) | ((DAT) << 8))
#define FPU_ADDR_INC_DAT(ADR_NUM, DAT)       (0x0038 | (ADR_NUM)  | ((DAT) << 8))
    #define SEND_ONE_ADDRESS       (1U)
    #define SEND_COLUMN_ADDRESS    (2U) /* 2 col */
    #define SEND_ROW_ADDRESS       (3U) /* 3 row */
    #define SEND_4_ADDRESS         (4U) /* 2 row, 2 col */
    #define SEND_5_ADDRESS         (5U) /* all */

#define FPU_DAT_RD()			 (0x004E)

#define FPU_DAT_RDY(MASK)			 (0x0042 | (MASK << 8))

#define FPU_DAT_FAIL(MASK)			 (0x0040 | (MASK << 8))


#define FPU_POL(MASK)            (0x0050 | (BIT0) | ((MASK) << 8))
#define FPU_POL_RB()             (0x0050)

#define FPU_DMA(DMY_ENA, RW)     (0x0060 | ((DMY_ENA) << 1) | (RW))
#define FPU_DMA_R()              (0x0060)
#define FPU_DMA_W()              (0x0061)

#define FPU_ADR_GEN()            (0x0070)

#define FPU_BCC_TRIG()           (0x0080)

#define FPU_RTY_DAT(DAT)         (0x0090 | ((DAT) << 8))

#define FPU_RAW()								 (0x00A0)

#define FPU_SBC()								 (0x00C0)

#define FPU_BR(IBF_PTR, FRAME_NUM, DIRECTION, MODE) (0x00D0 | ((IBF_PTR << 13) | (FRAME_NUM << 10) | (DIRECTION << 9) | MODE << 8) )

#define FPU_END()                (0x00F0)
#endif
#endif /* __S11_FPU_H__ */
